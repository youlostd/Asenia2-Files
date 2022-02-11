/*
	* Filename : offline_shop.cpp
	* Version : 0.1
	* Description : --
*/

#include "stdafx.h"
#include "../../libgame/src/grid.h"
#include "constants.h"
#include "utils.h"
#include "config.h"
#include "desc.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "item.h"
#include "item_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "log.h"
#include "db.h"
#include "questmanager.h"
#include "monarch.h"
#include "mob_manager.h"
#include "locale_service.h"
#include "offline_shop.h"
#include "p2p.h"
#include <locale>
#include <iostream>
#include "boost/algorithm/string.hpp"

using namespace std;
using namespace boost;


COfflineShop::COfflineShop() : m_pkOfflineShopNPC(NULL)
{
	m_pGrid = M2_NEW CGrid(20, 10);
	m_itemVector.resize(OFFLINE_SHOP_HOST_ITEM_MAX_NUM);
	memset(&m_itemVector[0], 0, sizeof(OFFLINE_SHOP_ITEM) * m_itemVector.size());
}


COfflineShop::~COfflineShop()
{
	TPacketGCShop pack;
	pack.header = HEADER_GC_OFFLINE_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_END;
	pack.size = sizeof(TPacketGCShop);

	Broadcast(&pack, sizeof(pack));

	for (GuestMapType::iterator it = m_map_guest.begin(); it != m_map_guest.end(); ++it)
	{
		LPCHARACTER ch = it->first;
		ch->SetOfflineShop(NULL);
	}

	M2_DELETE(m_pGrid);
}

void COfflineShop::SetOfflineShopNPC(LPCHARACTER npc)
{
	m_pkOfflineShopNPC = npc;
}

bool COfflineShop::SetShopItems(LPCHARACTER npc, DWORD shopVID)
{
	m_itemVector.resize(OFFLINE_SHOP_HOST_ITEM_MAX_NUM);
	memset(&m_itemVector[0], 0, sizeof(OFFLINE_SHOP_ITEM) * m_itemVector.size());

	// sys_log(0, "owner_id %d", npc ? npc->GetOfflineShopRealOwner() : 0);
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT of.pos, of.count, of.vnum, of.price, of.price_cheque, of.socket0, of.socket1, of. socket2, of. socket3, of. socket4, of. socket5, of. evolution, of. attrtype0, of. attrvalue0, of. attrtype1, of. attrvalue1, of. attrtype2, of. attrvalue2, of. attrtype3, of. attrvalue3, of. attrtype4, of. attrvalue4, of. attrtype5, of. attrvalue5, of. attrtype6, of. attrvalue6,(case when of.vnum in(50300, 70037, 90066, 90067) then (select concat(convert(sp.szName using latin5),' ',convert(ip.locale_name using latin5)) from skill_proto sp where sp.dwVnum = of.socket0) else convert(ip.locale_name using latin5) end) as item_name from offline_shop_item of inner join item_proto ip on ip.vnum = of.vnum where owner_id = %u", npc ? npc->GetOfflineShopRealOwner() : 0));
	if (pMsg->Get()->uiNumRows == 0)
	{
		// sys_log(0, "Pazarda item yok");
		return false;
	}

	MYSQL_ROW row;
	while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
	{
		BYTE bPos = 0;
		str_to_number(bPos, row[0]);

		OFFLINE_SHOP_ITEM & offShopItem = m_itemVector[bPos];
		str_to_number(offShopItem.count, row[1]);
		str_to_number(offShopItem.vnum, row[2]);
		str_to_number(offShopItem.price, row[3]);
		str_to_number(offShopItem.price_cheque, row[4]);

		//Sockets
		for (int i = 0, n = 5; i < ITEM_SOCKET_MAX_NUM; ++i, n++)
			str_to_number(offShopItem.alSockets[i], row[n]);

		str_to_number(offShopItem.evolution, row[11]);

		//Attrs
		for (int i = 0, iStartAttributeType = 12, iStartAttributeValue = 13; i < ITEM_ATTRIBUTE_MAX_NUM; ++i, iStartAttributeType += 2, iStartAttributeValue += 2)
		{
			str_to_number(offShopItem.aAttr[i].bType, row[iStartAttributeType]);
			str_to_number(offShopItem.aAttr[i].sValue, row[iStartAttributeValue]);
		}
		strncpy(offShopItem.item_name, row[26], sizeof(offShopItem.item_name));
		// sys_log(0, "AddItem itemVector %d %s", offShopItem.vnum, offShopItem.item_name);
	}
	return true;
}

bool COfflineShop::AddGuest(LPCHARACTER ch, LPCHARACTER npc)
{
	// If the npc or ch is nullptr, return false
	if (!ch || !npc)
		return false;

	// If ch is exchanging-shop-myshop-offlineshop, return false
	if (ch->GetExchange() || ch->GetShop() || ch->GetMyShop() || ch->GetOfflineShop())
		return false;

	// Start process
	ch->SetOfflineShop(this);
	m_map_guest.insert(GuestMapType::value_type(ch, false));

#ifdef GECICI_FIX	
	if (find(guestlist.begin(), guestlist.end(), ch->GetPlayerID()) == guestlist.end())
		guestlist.push_back(ch->GetPlayerID());

#endif

	TPacketGCShop pack;
	pack.header = HEADER_GC_OFFLINE_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_START;

	TPacketGCOfflineShopStart pack2;
	memset(&pack2, 0, sizeof(pack2));
	pack2.owner_vid = npc->GetVID();

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT pos,count,vnum,price,price_cheque,socket0,socket1,socket2, socket3, socket4, socket5, evolution, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), npc ? npc->GetOfflineShopRealOwner() : 0));
	if (pMsg->Get()->uiNumRows == 0)
	{
		DBManager::instance().DirectQuery("DELETE FROM player.offline_shop_npc WHERE owner_id = %u", npc->GetOfflineShopRealOwner());
		ch->SetOfflineShop(NULL);
		ch->SetOfflineShopOwner(NULL);
		M2_DESTROY_CHARACTER(npc);
		return false;
	}

	MYSQL_ROW row;
	while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
	{
		BYTE bPos = 0;
		str_to_number(bPos, row[0]);

		str_to_number(pack2.items[bPos].count, row[1]);
		str_to_number(pack2.items[bPos].vnum, row[2]);
		str_to_number(pack2.items[bPos].price, row[3]);
		str_to_number(pack2.items[bPos].price_cheque, row[4]);


		DWORD alSockets[ITEM_SOCKET_MAX_NUM];
		for (int i = 0, n = 5; i < ITEM_SOCKET_MAX_NUM; ++i, n++)
			str_to_number(alSockets[i], row[n]);

		str_to_number(pack2.items[bPos].evolution, row[11]);
		TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
		for (int i = 0, iStartType = 12, iStartValue = 13; i < ITEM_ATTRIBUTE_MAX_NUM; ++i, iStartType += 2, iStartValue += 2)
		{
			str_to_number(aAttr[i].bType, row[iStartType]);
			str_to_number(aAttr[i].sValue, row[iStartValue]);
		}

		thecore_memcpy(pack2.items[bPos].alSockets, alSockets, sizeof(pack2.items[bPos].alSockets));
		thecore_memcpy(pack2.items[bPos].aAttr, aAttr, sizeof(pack2.items[bPos].aAttr));
	}

	pack.size = sizeof(pack) + sizeof(pack2);

	if (ch->GetDesc())
	{
		ch->GetDesc()->BufferedPacket(&pack, sizeof(TPacketGCShop));
		ch->GetDesc()->Packet(&pack2, sizeof(TPacketGCOfflineShopStart));
	}
	
	return true;
}

void COfflineShop::RemoveGuest(LPCHARACTER ch)
{
	// If this offline shop is not equal to this, break it
	if (ch->GetOfflineShop() != this)
		return;

	m_map_guest.erase(ch);
	ch->SetOfflineShop(NULL);

	TPacketGCShop pack;
	pack.header = HEADER_GC_OFFLINE_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_END;
	pack.size = sizeof(TPacketGCShop);

	if (ch->GetDesc())
		ch->GetDesc()->Packet(&pack, sizeof(pack));

#ifdef GECICI_FIX
	std::vector<DWORD>::iterator bul = std::remove(guestlist.begin(), guestlist.end(), ch->GetPlayerID());
	guestlist.erase(bul, guestlist.end());
#endif
}

void COfflineShop::RemoveAllGuest()
{
	TPacketGCShop pack;
	pack.header = HEADER_GC_OFFLINE_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_END;
	pack.size = sizeof(TPacketGCShop);

	Broadcast(&pack, sizeof(pack));

#ifdef GECICI_FIX
	for (std::vector<DWORD>::iterator it = guestlist.begin(); it != guestlist.end(); it++)
	{
		LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(*it);
		if (ch && ch->GetDesc())
			ch->SetOfflineShop(NULL);			
	}
#else
	for (GuestMapType::iterator it = m_map_guest.begin(); it != m_map_guest.end(); ++it)
	{
		LPCHARACTER ch = it->first;
		ch->SetOfflineShop(NULL);
	}
#endif
}

void COfflineShop::Destroy(LPCHARACTER npc)
{
	DBManager::instance().Query("DELETE FROM %soffline_shop_npc WHERE owner_id = %u", get_table_postfix(), npc->GetOfflineShopRealOwner());
	RemoveAllGuest();
	M2_DESTROY_CHARACTER(npc);
}

int COfflineShop::Buy(LPCHARACTER ch, BYTE bPos)
{
	if (ch->GetOfflineShopOwner()->GetOfflineShopRealOwner() == ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't buy anything from your offline shop."));
		return SHOP_SUBHEADER_GC_OK;
	}

	if (bPos >= OFFLINE_SHOP_HOST_ITEM_MAX_NUM)
	{
		sys_log(0, "OfflineShop::Buy : invalid position %d : %s", bPos, ch->GetName());
		return SHOP_SUBHEADER_GC_INVALID_POS;
	}

	sys_log(0, "OfflineShop::Buy : name %s pos %d", ch->GetName(), bPos);

	GuestMapType::iterator it = m_map_guest.find(ch);
	if (it == m_map_guest.end())
		return SHOP_SUBHEADER_GC_END;
	
	char szQuery[1024];
	snprintf(szQuery, sizeof(szQuery), "SELECT pos,count,vnum,price,price_cheque,socket0,socket1,socket2, socket3,socket4,socket5, evolution, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u and pos = %d", get_table_postfix(), ch->GetOfflineShopOwner()->GetOfflineShopRealOwner(), bPos);
	std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery(szQuery));

	MYSQL_ROW row;

	DWORD dwPrice = 0;
	DWORD dwPriceCheque = 0;
	DWORD dwItemVnum = 0;
	DWORD dwEvolution = 0;
	short bCount = 0;
	DWORD alSockets[ITEM_SOCKET_MAX_NUM];
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];


	while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
	{
		str_to_number(bCount, row[1]);
		str_to_number(dwItemVnum, row[2]);
		str_to_number(dwPrice, row[3]);
		str_to_number(dwPriceCheque, row[4]);

		// Set Sockets
		for (int i = 0, n = 5; i < ITEM_SOCKET_MAX_NUM; ++i, n++)
			str_to_number(alSockets[i], row[n]);
		// End Of Sockets

		str_to_number(dwEvolution, row[11]);
		// Set Attributes
		for (int i = 0, iStartAttributeType = 12, iStartAttributeValue = 13; i < ITEM_ATTRIBUTE_MAX_NUM; ++i, iStartAttributeType += 2, iStartAttributeValue += 2)
		{
			str_to_number(aAttr[i].bType, row[iStartAttributeType]);
			str_to_number(aAttr[i].sValue, row[iStartAttributeValue]);
		}
		// End Of Set Attributes
	}

	// Brazil server is not use gold option.
	if (!LC_IsBrazil())
	{
		if (ch->GetGold() < (int) dwPrice && ch->GetCheque() >= (int) dwPriceCheque)
		{
			sys_log(1, "Shop::Buy : Not enough money : %s has %d, price %d", ch->GetName(), ch->GetGold(), dwPrice);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;
		}
	
		if (ch->GetCheque() < (int) dwPriceCheque && ch->GetGold() >= (int) dwPrice)
		{
			sys_log(1, "Shop::Buy : Not enough won : %s has %d, price_cheque %d", ch->GetName(), ch->GetCheque(), dwPriceCheque);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE;
		}

		if (ch->GetGold() < (int) dwPrice && ch->GetCheque() < (int) dwPriceCheque)
		{
			sys_log(1, "Shop::Buy : Not enough won_money : %s has %d and %d, price %d and price_cheque %d", ch->GetName(), ch->GetGold(), ch->GetCheque(), dwPrice, dwPriceCheque);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE_MONEY;
		}
	}
	else
	{
		int iItemCount = quest::CQuestManager::instance().GetCurrentCharacterPtr()->CountSpecifyItem(30183);
		if (iItemCount < static_cast<int>(dwPrice))
		{
			sys_log(1, "OfflineShop::Buy : Not enough gold mask : %s has %d, gold mask %u", ch->GetName(), iItemCount, dwPrice);
			return SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY;
		}
	}

	LPITEM pItem = ITEM_MANAGER::Instance().CreateItem(dwItemVnum, bCount);
	if (!pItem)
		return SHOP_SUBHEADER_GC_SOLD_OUT;
	
	// Set Attributes, Sockets
	pItem->SetAttributes(aAttr);
	pItem->SetEvolution(dwEvolution);
	for (BYTE i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		pItem->SetSocket(i, alSockets[i]);
	// End Of Set Attributes, Sockets
	
	
	// If item is a dragon soul item or normal item
	int iEmptyPos = 0;
	if (pItem->IsDragonSoul())
		iEmptyPos = ch->GetEmptyDragonSoulInventory(pItem);
	else if (pItem->IsUpgradeItem())
		iEmptyPos = ch->GetEmptyUpgradeInventory(pItem);
	else if (pItem->IsStone())
		iEmptyPos = ch->GetEmptyStoneInventory(pItem);
	else if (pItem->IsChest())
		iEmptyPos = ch->GetEmptyChestInventory(pItem);
	else if (pItem->IsAttr())
		iEmptyPos = ch->GetEmptyAttrInventory(pItem);
	else
		iEmptyPos = ch->GetEmptyInventory(pItem->GetSize());

	// If iEmptyPos is less than 0, return inventory is full
	if (iEmptyPos < 0)
		return SHOP_SUBHEADER_GC_INVENTORY_FULL;

	// If item is a dragon soul, add this item in dragon soul inventory
	if (pItem->IsDragonSoul())
		pItem->AddToCharacter(ch, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyPos));
	else if (pItem->IsUpgradeItem())
		pItem->AddToCharacter(ch, TItemPos(UPGRADE_INVENTORY,iEmptyPos));
	else if (pItem->IsStone())
		pItem->AddToCharacter(ch, TItemPos(STONE_INVENTORY,iEmptyPos));
	else if (pItem->IsChest())
		pItem->AddToCharacter(ch, TItemPos(CHEST_INVENTORY,iEmptyPos));
	else if (pItem->IsAttr())
		pItem->AddToCharacter(ch, TItemPos(ATTR_INVENTORY,iEmptyPos));
	else 
		pItem->AddToCharacter(ch, TItemPos(INVENTORY,iEmptyPos));

	if (pItem)
		sys_log(0, "OFFLINE_SHOP: BUY: name %s %s(x %u):%u price %u", ch->GetName(), pItem->GetName(), pItem->GetCount(), pItem->GetID(), dwPrice);

	OFFLINE_SHOP_ITEM & r_item = m_itemVector[bPos];
	r_item.vnum = 0;

	// Check if the player is online
	LPCHARACTER tch = CHARACTER_MANAGER::instance().FindByPID(ch->GetOfflineShopOwner()->GetOfflineShopRealOwner());
	if (!LC_IsBrazil())
		DBManager::instance().DirectQuery("UPDATE player.player SET money2 = money2 + %u WHERE id = %u", dwPrice, ch->GetOfflineShopOwner()->GetOfflineShopRealOwner());
	
	DBManager::instance().Query("UPDATE player.player SET money_cheque = money_cheque + %u WHERE id = %u", dwPriceCheque, ch->GetOfflineShopOwner()->GetOfflineShopRealOwner());

	RemoveItem(ch->GetOfflineShopOwner()->GetOfflineShopRealOwner(), bPos);
	BroadcastUpdateItem(bPos, ch->GetOfflineShopOwner()->GetOfflineShopRealOwner(), true);
	ch->PointChange(POINT_GOLD, -dwPrice, false);
	ch->PointChange(POINT_CHEQUE, -dwPriceCheque, false);
	ch->Save();
	LogManager::instance().ItemLog(ch, pItem, "BUY ITEM FROM OFFLINE SHOP", "");

	BYTE bLeftItemCount = GetLeftItemCount(ch->GetOfflineShopOwner()->GetOfflineShopRealOwner());
	if (bLeftItemCount == 0)
		Destroy(ch->GetOfflineShopOwner());

	return (SHOP_SUBHEADER_GC_OK);
}

void COfflineShop::BroadcastUpdateItem(BYTE bPos, DWORD dwPID, bool bDestroy)
{
	TPacketGCShop pack;
	TPacketGCShopUpdateItem pack2;

	TEMP_BUFFER buf;

	pack.header = HEADER_GC_OFFLINE_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_UPDATE_ITEM;
	pack.size = sizeof(pack) + sizeof(pack2);
	pack2.pos = bPos;

	if (bDestroy)
	{
		pack2.item.vnum = 0;
		pack2.item.count = 0;
		pack2.item.price = 0;
		pack2.item.price_cheque = 0;

		memset(pack2.item.alSockets, 0, sizeof(pack2.item.alSockets));
		memset(pack2.item.aAttr, 0, sizeof(pack2.item.aAttr));
	}
	else
	{
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT of.pos, of.count, of.vnum, of.price, of.price_cheque, of.socket0, of.socket1, of. socket2, of. socket3, of. socket4, of. socket5, of. evolution, of. attrtype0, of. attrvalue0, of. attrtype1, of. attrvalue1, of. attrtype2, of. attrvalue2, of. attrtype3, of. attrvalue3, of. attrtype4, of. attrvalue4, of. attrtype5, of. attrvalue5, of. attrtype6, of. attrvalue6,(case when of.vnum in(50300, 70037, 90066, 90067) then (select concat(convert(sp.szName using latin5),' ',convert(ip.locale_name using latin5)) from skill_proto sp where sp.dwVnum = of.socket0) else convert(ip.locale_name using latin5) end) as item_name from offline_shop_item of inner join item_proto ip on ip.vnum = of.vnum where owner_id = %u and pos = %d", dwPID, bPos));

		MYSQL_ROW row;
		while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
		{
			str_to_number(pack2.item.count, row[1]);
			str_to_number(pack2.item.vnum, row[2]);
			str_to_number(pack2.item.price, row[3]);
			str_to_number(pack2.item.price_cheque, row[4]);
			OFFLINE_SHOP_ITEM & offShopItem = m_itemVector[bPos];
			if (offShopItem.vnum == 0)
			{
				str_to_number(offShopItem.count, row[1]);
				str_to_number(offShopItem.vnum, row[2]);
				str_to_number(offShopItem.price, row[3]);
				str_to_number(offShopItem.price_cheque, row[4]);

				//Sockets
				for (int i = 0, n = 5; i < ITEM_SOCKET_MAX_NUM; ++i, n++)
					str_to_number(offShopItem.alSockets[i], row[n]);

				str_to_number(offShopItem.evolution, row[11]);
				//Attrs
				for (int i = 0, iStartAttributeType = 12, iStartAttributeValue = 13; i < ITEM_ATTRIBUTE_MAX_NUM; ++i, iStartAttributeType += 2, iStartAttributeValue += 2)
				{
					str_to_number(offShopItem.aAttr[i].bType, row[iStartAttributeType]);
					str_to_number(offShopItem.aAttr[i].sValue, row[iStartAttributeValue]);
				}
				strncpy(offShopItem.item_name, row[26], sizeof(offShopItem.item_name));

			}
			

			// Set Sockets
			for (int i = 0, n = 5; i < ITEM_SOCKET_MAX_NUM; ++i, n++)
				str_to_number(pack2.item.alSockets[i], row[n]);
			// End Of Sockets

			str_to_number(pack2.item.evolution, row[11]);
			// Set Attributes
			for (int i = 0, iStartAttributeType = 12, iStartAttributeValue = 13; i < ITEM_ATTRIBUTE_MAX_NUM; ++i, iStartAttributeType += 2, iStartAttributeValue += 2)
			{
				str_to_number(pack2.item.aAttr[i].bType, row[iStartAttributeType]);
				str_to_number(pack2.item.aAttr[i].sValue, row[iStartAttributeValue]);
			}
			// End Of Set Attributes
		}
	}

	buf.write(&pack, sizeof(pack));
	buf.write(&pack2, sizeof(pack2));
	Broadcast(buf.read_peek(), buf.size());
}

void COfflineShop::BroadcastUpdatePrice(BYTE bPos, DWORD dwPrice)
{
	TPacketGCShop pack;
	TPacketGCShopUpdatePrice pack2;

	TEMP_BUFFER buf;
	
	pack.header = HEADER_GC_OFFLINE_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_UPDATE_PRICE;
	pack.size = sizeof(pack) + sizeof(pack2);

	pack2.bPos = bPos;
	pack2.iPrice = dwPrice;

	buf.write(&pack, sizeof(pack));
	buf.write(&pack2, sizeof(pack2));

	Broadcast(buf.read_peek(), buf.size());
}

void COfflineShop::Refresh(LPCHARACTER ch)
{
	TPacketGCShop pack;
	pack.header = HEADER_GC_OFFLINE_SHOP;
	pack.subheader = SHOP_SUBHEADER_GC_UPDATE_ITEM2;

	TPacketGCOfflineShopStart pack2;
	memset(&pack2, 0, sizeof(pack2));
	pack2.owner_vid = 0;

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT pos,count,vnum,price,price_cheque,socket0,socket1,socket2,socket3,socket4,socket5, evolution, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID()));
	
	MYSQL_ROW row;
	while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
	{
		BYTE bPos = 0;
		str_to_number(bPos, row[0]);

		str_to_number(pack2.items[bPos].count, row[1]);
		str_to_number(pack2.items[bPos].vnum, row[2]);
		str_to_number(pack2.items[bPos].price, row[3]);
		str_to_number(pack2.items[bPos].price_cheque, row[4]);


		DWORD alSockets[ITEM_SOCKET_MAX_NUM];
		for (int i = 0, n = 5; i < ITEM_SOCKET_MAX_NUM; ++i, n++)
			str_to_number(alSockets[i], row[n]);

		str_to_number(pack2.items[bPos].evolution, row[11]);
		TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
		for (int i = 0, iStartType = 12, iStartValue = 13; i < ITEM_ATTRIBUTE_MAX_NUM; ++i, iStartType += 2, iStartValue += 2)
		{
			str_to_number(aAttr[i].bType, row[iStartType]);
			str_to_number(aAttr[i].sValue, row[iStartValue]);
		}

		thecore_memcpy(pack2.items[bPos].alSockets, alSockets, sizeof(pack2.items[bPos].alSockets));
		thecore_memcpy(pack2.items[bPos].aAttr, aAttr, sizeof(pack2.items[bPos].aAttr));
	}

	pack.size = sizeof(pack) + sizeof(pack2);

	if (ch->GetDesc())
	{
		ch->GetDesc()->BufferedPacket(&pack, sizeof(TPacketGCShop));
		ch->GetDesc()->Packet(&pack2, sizeof(TPacketGCOfflineShopStart));
	}
}

bool COfflineShop::RemoveItem(DWORD dwVID, BYTE bPos)
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_item WHERE owner_id = %u and pos = %d", get_table_postfix(), dwVID, bPos));
	return pMsg->Get()->uiAffectedRows > 0;
}

short COfflineShop::GetLeftItemCount(DWORD dwPID)
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT COUNT(*) FROM %soffline_shop_item WHERE owner_id = %u and status = 0", get_table_postfix(), dwPID));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	short bCount = 0;
	str_to_number(bCount, row[0]);
	return bCount;
}

void COfflineShop::Broadcast(const void * data, int bytes)
{
	sys_log(1, "OfflineShop::Broadcast %p %d", data, bytes);

#ifdef GECICI_FIX
	for (std::vector<DWORD>::iterator it = guestlist.begin(); it != guestlist.end(); it++)
	{
		LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(*it);
		if (ch && ch->GetDesc())
			ch->GetDesc()->Packet(data, bytes);
	}
#else
	for (GuestMapType::iterator it = m_map_guest.begin(); it != m_map_guest.end(); ++it)
	{
		LPCHARACTER ch = it->first;
		if (ch->GetDesc())
			ch->GetDesc()->Packet(data, bytes);
	}
#endif
}

void COfflineShop::SetGuestMap(LPCHARACTER ch)
{
	GuestMapType::iterator it = m_map_guest.find(ch);
	if (it != m_map_guest.end())
		return;
	m_map_guest.insert(GuestMapType::value_type(ch, false));
}

void COfflineShop::RemoveGuestMap(LPCHARACTER ch)
{
	if (ch->GetOfflineShop() != this)
		return;

	m_map_guest.erase(ch);
}

// #ifdef SHOP_SEARCH

	inline bool SHOP_SEARCH_PRICE_VAILD_MIN(long item_price, long price) {return (item_price <= price || price == 0);}
	inline bool SHOP_SEARCH_PRICE_VAILD_MAX(long item_price, long price) {return (item_price >= price);}

	bool COfflineShop::SearchItem(const std::string& isim, long price)
	{
		std::string isim2(isim);
		stl_lowers(isim2);

		for (int bPos = 0; bPos < OFFLINE_SHOP_HOST_ITEM_MAX_NUM; bPos++)
		{
			const OFFLINE_SHOP_ITEM & r_item = m_itemVector[bPos];
			// sys_log(0, "Pos %d [%d]", bPos, r_item.vnum);
			if (!r_item.vnum || (r_item.price < 1 && r_item.price_cheque < 1))
				continue;

			std::string name (r_item.item_name);
			stl_lowers(name);
			// sys_log(0, "Taranan %s ", name.c_str());
			if (boost::contains(name, isim2))
				return true;
		}
		return false;
	}
// #endif
