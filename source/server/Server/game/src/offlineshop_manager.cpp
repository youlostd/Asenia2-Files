#include "stdafx.h"
#include "../../libgame/src/grid.h"
#include "constants.h"
#include "utils.h"
#include "config.h"
#include "offline_shop.h"
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
#include "desc_client.h"
#include "group_text_parse_tree.h"
#include <boost/algorithm/string/predicate.hpp>
#include <cctype>
#include "offlineshop_manager.h"
#include "p2p.h"
#include "entity.h"
#include "sectree_manager.h"
#include "offlineshop_config.h"


COfflineShopManager::COfflineShopManager()
{
}


COfflineShopManager::~COfflineShopManager()
{
}

struct FFindOfflineShop
{
	const char * szName;

	DWORD dwVID, dwRealOwner;
	FFindOfflineShop(const char * c_szName) : szName(c_szName), dwVID(0), dwRealOwner(0) {};

	void operator()(LPENTITY ent)
	{
		if (!ent)
			return;

		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;
			if (ch->IsOfflineShopNPC() && !strcmp(szName, ch->GetName()))
			{
				dwVID = ch->GetVID();
				dwRealOwner = ch->GetOfflineShopRealOwner();
				M2_DESTROY_CHARACTER(ch);
			}
		}
	}
};

bool COfflineShopManager::StartShopping(LPCHARACTER pkChr, LPCHARACTER pkChrShopKeeper)
{
	if (pkChr->GetOfflineShopOwner() == pkChrShopKeeper)
		return false;

	if (pkChrShopKeeper->IsPC())
		return false;

	sys_log(0, "OFFLINE_SHOP: START: %s", pkChr->GetName());
	return true;
}

LPOFFLINESHOP COfflineShopManager::CreateOfflineShop(LPCHARACTER npc, DWORD dwOwnerPID)
{
	if (FindOfflineShop(npc->GetVID()))
		return NULL;

	LPOFFLINESHOP pkOfflineShop = M2_NEW COfflineShop;
	pkOfflineShop->SetOfflineShopNPC(npc);

	m_map_pkOfflineShopByNPC.insert(TShopMap::value_type(npc->GetVID(), pkOfflineShop));
	m_Map_pkOfflineShopByNPC2.insert(TOfflineShopMap::value_type(dwOwnerPID, npc->GetVID()));
	return pkOfflineShop;
}

LPOFFLINESHOP COfflineShopManager::FindOfflineShop(DWORD dwVID)
{
	TShopMap::iterator it = m_map_pkOfflineShopByNPC.find(dwVID);

	if (it == m_map_pkOfflineShopByNPC.end())
		return NULL;

	return it->second;
}

void COfflineShopManager::DestroyOfflineShop(LPCHARACTER ch, DWORD dwVID, bool bDestroyAll)
{	
	if (dwVID == 0 && ch)
	{
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT mapIndex, mapIndex,channel FROM %soffline_shop_npc WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID()));
		if (pMsg->Get()->uiNumRows == 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't use this option because you don't have offline shop npc!"));
			return;			
		}
		
		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
		
		long lMapIndex = 0;
		str_to_number(lMapIndex, row[0]);
		
		TPacketGGRemoveOfflineShop p;
		p.bHeader = HEADER_GG_REMOVE_OFFLINE_SHOP;
		p.lMapIndex = lMapIndex;
		
		// Set offline shop name
		char szNpcName[CHARACTER_NAME_MAX_LEN + 1];
		snprintf(szNpcName, sizeof(szNpcName), "%s", ch->GetName());
		strlcpy(p.szNpcName, szNpcName, sizeof(p.szNpcName));
		// End Of Set offline shop name
		
		P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGRemoveOfflineShop));	
		DBManager::instance().DirectQuery("UPDATE %soffline_shop_item SET status = 1 WHERE owner_id = %u and status = 0", get_table_postfix(), ch->GetPlayerID());

	}
	else if (dwVID && ch)
	{
		LPCHARACTER npc;
		if (!ch)
			npc = CHARACTER_MANAGER::instance().Find(dwVID);
		else
			npc = CHARACTER_MANAGER::instance().Find(FindMyOfflineShop(ch->GetPlayerID()));

		if (!npc)
		{
			std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT mapIndex,channel FROM %soffline_shop_npc WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID()));
			if (pMsg->Get()->uiNumRows == 0)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't use this option because you don't have offline shop npc!"));
				return;
			}

			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);

			long lMapIndex = 0;
			str_to_number(lMapIndex, row[0]);

			BYTE bChannel = 0;
			str_to_number(bChannel, row[1]);

			if (g_bChannel != bChannel)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't close your offline shop from %d channel. You must login %d channel"), g_bChannel, bChannel);
				return;
			}

			char szName[CHARACTER_NAME_MAX_LEN + 1];
			snprintf(szName, sizeof(szName), "%s", ch->GetName());
			LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(lMapIndex);
			FFindOfflineShop offlineShop(szName);
			pMap->for_each(offlineShop);

			if (bDestroyAll)
			{
				DBManager::instance().DirectQuery("UPDATE %soffline_shop_item SET status = 1 WHERE owner_id = %u and status = 0", get_table_postfix(), ch->GetPlayerID());
				m_map_pkOfflineShopByNPC.erase(offlineShop.dwVID);
				m_Map_pkOfflineShopByNPC2.erase(offlineShop.dwRealOwner);
				DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_npc WHERE owner_id = %u", get_table_postfix(), offlineShop.dwRealOwner);
				return;
			}
		}

		LPOFFLINESHOP pkOfflineShop;
		if (!ch)
			pkOfflineShop = FindOfflineShop(dwVID);
		else
			pkOfflineShop = FindOfflineShop(FindMyOfflineShop(ch->GetPlayerID()));
		
		if (!pkOfflineShop)
			return;

		if (npc->GetOfflineShopChannel() != g_bChannel)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't close your offline shop from %d channel. You must login %d channel"), g_bChannel, npc->GetOfflineShopChannel());
			return;
		}

		if (bDestroyAll)
		{
			DBManager::instance().DirectQuery("UPDATE %soffline_shop_item SET status = 1 WHERE owner_id = %u and status = 0", get_table_postfix(), ch->GetPlayerID());
			pkOfflineShop->Destroy(npc);
		}

		m_map_pkOfflineShopByNPC.erase(npc->GetVID());
		m_Map_pkOfflineShopByNPC2.erase(npc->GetOfflineShopRealOwner());
		M2_DELETE(pkOfflineShop);
	}
}

void COfflineShopManager::Giveback(LPCHARACTER ch)
{
	if (!ch)
		return;
	
	if (thecore_pulse() - ch->GetMyOfflineShopTime() < PASSES_PER_SEC(10))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You're too fast! Slow down!"));
		return;
	}

	char szQuery[1024];
	if (g_bOfflineShopSocketMax == 3)
		snprintf(szQuery, sizeof(szQuery), "SELECT pos,count,vnum,socket0,socket1,socket2, socket3,socket4,socket5, evolution, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID());
	else if(g_bOfflineShopSocketMax == 4)
		snprintf(szQuery, sizeof(szQuery), "SELECT pos,count,vnum,socket0,socket1,socket2,socket3, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID());
	else if(g_bOfflineShopSocketMax == 5)
		snprintf(szQuery, sizeof(szQuery), "SELECT pos,count,vnum,socket0,socket1,socket2,socket3,socket4, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID());
	else if(g_bOfflineShopSocketMax == 6)
		snprintf(szQuery, sizeof(szQuery), "SELECT pos,count,vnum,socket0,socket1,socket2,socket3,socket4,socket5, evolution, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID());
		
	std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery(szQuery));

	if (pMsg->Get()->uiNumRows == 0)
	{
		// sys_err("COfflineShopManager::GiveBack - There is nothing for this player [%s]", ch->GetName());
		return;
	}

	MYSQL_ROW row;
	while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
	{
		TPlayerItem item;

		str_to_number(item.pos, row[0]);
		str_to_number(item.count, row[1]);
		str_to_number(item.vnum, row[2]);
		
		// Set Sockets
		for (int i = 0, n = 3; i < ITEM_SOCKET_MAX_NUM; ++i, n++)
			str_to_number(item.alSockets[i], row[n]);
		// End Of Set Sockets

		str_to_number(item.evolution, row[9]);
		// Set Attributes
		for (int i = 0, iStartAttributeType = 10, iStartAttributeValue = 11 ; i < ITEM_ATTRIBUTE_MAX_NUM; ++i, iStartAttributeType += 2, iStartAttributeValue += 2)
		{
			str_to_number(item.aAttr[i].bType, row[iStartAttributeType]);
			str_to_number(item.aAttr[i].sValue, row[iStartAttributeValue]);
		}
		// End Of Set Attributes

		LPITEM pItem = ITEM_MANAGER::instance().CreateItem(item.vnum, item.count);
		if (pItem)
		{
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

			if (iEmptyPos < 0)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("There is no enough slot for store the items!"));
				return;
			}

			pItem->SetSockets(item.alSockets);
			pItem->SetAttributes(item.aAttr);
			pItem->SetEvolution(item.evolution);

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
				pItem->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
			
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You are gain %s item from your offline shop."), pItem->GetName());
			DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_item WHERE owner_id = %u and pos = %d", get_table_postfix(), ch->GetPlayerID(), item.pos);
			pItem->Save();
		}
	}
	
	ch->Save();
}

void COfflineShopManager::Giveback3(LPCHARACTER ch)
{
	if (!ch)
		return;
	
	if (thecore_pulse() - ch->GetMyOfflineShopTime() < PASSES_PER_SEC(10))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You're too fast! Slow down!"));
		return;
	}

	char szQuery[1024];
	if (g_bOfflineShopSocketMax == 3)
		snprintf(szQuery, sizeof(szQuery), "SELECT pos,count,vnum,socket0,socket1,socket2, socket3,socket4,socket5,evolution, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID());
	else if(g_bOfflineShopSocketMax == 4)
		snprintf(szQuery, sizeof(szQuery), "SELECT pos,count,vnum,socket0,socket1,socket2,socket3, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID());
	else if(g_bOfflineShopSocketMax == 5)
		snprintf(szQuery, sizeof(szQuery), "SELECT pos,count,vnum,socket0,socket1,socket2,socket3,socket4, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID());
	else if(g_bOfflineShopSocketMax == 6)
		snprintf(szQuery, sizeof(szQuery), "SELECT pos,count,vnum,socket0,socket1,socket2,socket3,socket4,socket5, evolution, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID());
		
	std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery(szQuery));

	if (pMsg->Get()->uiNumRows == 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("geri_verilecek_nesne_yok"));
		return;
	}
	
	std::unique_ptr<SQLMsg> pMsg1(DBManager::instance().DirectQuery("SELECT mapIndex, mapIndex,channel FROM %soffline_shop_npc WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID()));
	if (pMsg1->Get()->uiNumRows != 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ct_oldugu_icin_kullanamazsin"));
		return;			
	}

	MYSQL_ROW row;
	while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
	{
		TPlayerItem item;

		str_to_number(item.pos, row[0]);
		str_to_number(item.count, row[1]);
		str_to_number(item.vnum, row[2]);
		
		// Set Sockets
		for (int i = 0, n = 3; i < ITEM_SOCKET_MAX_NUM; ++i, n++)
			str_to_number(item.alSockets[i], row[n]);
		// End Of Set Sockets

		str_to_number(item.evolution, row[9]);
		// Set Attributes
		for (int i = 0, iStartAttributeType = 10, iStartAttributeValue = 11 ; i < ITEM_ATTRIBUTE_MAX_NUM; ++i, iStartAttributeType += 2, iStartAttributeValue += 2)
		{
			str_to_number(item.aAttr[i].bType, row[iStartAttributeType]);
			str_to_number(item.aAttr[i].sValue, row[iStartAttributeValue]);
		}
		// End Of Set Attributes

		LPITEM pItem = ITEM_MANAGER::instance().CreateItem(item.vnum, item.count);
		if (pItem)
		{
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

			if (iEmptyPos < 0)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("There is no enough slot for store the items!"));
				return;
			}

			pItem->SetSockets(item.alSockets);
			pItem->SetAttributes(item.aAttr);
			pItem->SetEvolution(item.evolution);

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
				pItem->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
			
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You are gain %s item from your offline shop."), pItem->GetName());
			DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_item WHERE owner_id = %u and pos = %d", get_table_postfix(), ch->GetPlayerID(), item.pos);
			pItem->Save();
		}
	}
	
	ch->Save();
}

void COfflineShopManager::Giveback2(LPCHARACTER ch)
{
	if (!ch)
		return;
	
	if (thecore_pulse() - ch->GetMyOfflineShopTime() < PASSES_PER_SEC(10))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You're too fast! Slow down!"));
		return;
	}
	
	char szQuery[1024];
	snprintf(szQuery, sizeof(szQuery), "SELECT pos,count,vnum,socket0,socket1,socket2, socket3,socket4,socket5, evolution, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u and status = 1", get_table_postfix(), ch->GetPlayerID());
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));
	
	if (pMsg->Get()->uiNumRows == 0)
		return;
	
	MYSQL_ROW row;
	while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
	{
		TPlayerItem item;

		str_to_number(item.pos, row[0]);
		str_to_number(item.count, row[1]);
		str_to_number(item.vnum, row[2]);
		
		// Set Sockets
		for (int i = 0, n = 3; i < ITEM_SOCKET_MAX_NUM; ++i, n++)
			str_to_number(item.alSockets[i], row[n]);
		// End Of Set Sockets

		str_to_number(item.evolution, row[9]);
		// Set Attributes
		for (int i = 0, iStartAttributeType = 10, iStartAttributeValue = 11; i < ITEM_ATTRIBUTE_MAX_NUM; ++i, iStartAttributeType += 2, iStartAttributeValue += 2)
		{
			str_to_number(item.aAttr[i].bType, row[iStartAttributeType]);
			str_to_number(item.aAttr[i].sValue, row[iStartAttributeValue]);
		}
		// End Of Set Attributes

		LPITEM pItem = ITEM_MANAGER::instance().CreateItem(item.vnum, item.count);
		if (pItem)
		{
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

			if (iEmptyPos < 0)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("There is no enough slot for store the items!"));
				return;
			}

			pItem->SetSockets(item.alSockets);
			pItem->SetAttributes(item.aAttr);
			pItem->SetEvolution(item.evolution);

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
				pItem->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
			
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You are gain %s item from your offline shop."), pItem->GetName());
			DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_item WHERE owner_id = %u and pos = %d", get_table_postfix(), ch->GetPlayerID(), item.pos);
			pItem->Save();
		}
	}
	
	ch->Save();
}

void COfflineShopManager::AddItem(LPCHARACTER ch, TItemPos tDisplayPos, BYTE bPos, int iPrice, int iPriceCheque)
{
	if (!ch)
		return;
	
	if (thecore_pulse() - ch->GetMyOfflineShopTime() < PASSES_PER_SEC(10))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You're too fast! Slow down!"));
		return;
	}
	if (ch->IsOpenSafebox() || ch->GetShop() || ch->IsCubeOpen() || ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->IsSashCombinationOpen() || ch->IsSashAbsorptionOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
		return;
	}
	if (ch->IsWarping())
	{
		return;
	}

	// Fixed bug 6.21.2015
	if (bPos >= OFFLINE_SHOP_HOST_ITEM_MAX_NUM)
	{
		sys_err("Overflow offline shop slot count [%s]", ch->GetName());
		return;
	}
	// End Of fixed bug 6.21.2015
	
	// Check player has offline shop or not
	std::unique_ptr<SQLMsg> pmsg(DBManager::instance().DirectQuery("SELECT COUNT(*) FROM player.offline_shop_npc WHERE owner_id = %u", ch->GetPlayerID()));
	MYSQL_ROW row = mysql_fetch_row(pmsg->Get()->pSQLResult);
	
	BYTE bResult = 0;
	str_to_number(bResult, row[0]);
	
	if (!bResult)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't use this option because you don't have a offline shop!"));
		return;
	}
	// End Of Check player has offline shop or not

	LPITEM pkItem = ch->GetItem(tDisplayPos);

	
	if (!pkItem)
		return;

	// Check
	const TItemTable * itemTable = pkItem->GetProto();
	if (IS_SET(itemTable->dwAntiFlags, ITEM_ANTIFLAG_GIVE | ITEM_ANTIFLAG_MY_OFFLINE_SHOP))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This item is not tradeable!"));
		return;
	}

	if (pkItem->isLocked())
		return;
	
#ifdef ENABLE_SOULBIND_SYSTEM
	if(pkItem->IsSealed()){
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Can't sell sealed item."));
		return;
	}
#endif

	if (pkItem->IsEquipped()) 
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't sell equipped items!"));
		return;
	}
	
/*#ifdef ENABLE_PRICE_RANGE
	// item fiyat aralik test
	
	int iFiyat = pkItem->GetProto()->dwGold;
	
	if (iPrice < iFiyat)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Esyayi bu fiyata koyamazsin."));
		return;
	}
	
	// item fiyat aralik test
#endif*/

	int iFiyat = pkItem->GetProto()->dwGold;	
	int	iadet = pkItem->GetCount();
	int itotal = iFiyat * iadet;
	int item1 = pkItem->GetVnum() == 50513;
	int item2 = pkItem->GetVnum() == 25041;
	int item3 = pkItem->GetVnum() == 27992;
	int item4 = pkItem->GetVnum() == 27993;
	int item5 = pkItem->GetVnum() == 27994;
	int won = 100000000;
	int total2 = iPriceCheque * won + iPrice;

	if (item1 || item2 || item3 || item4 || item5 )
	{
		if (1 == quest::CQuestManager::instance().GetEventFlag("fiyataralik"))
		{
			if (iPriceCheque < 1)
			{
				if (iPrice < itotal)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("esya_fiyat_block"));
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("esya_min_%d_yang"),iFiyat);
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d_adet_yang_%d"),iadet,itotal);
					return;
				}
			}
			if (iPriceCheque > 0)
			{
				if(total2 < itotal)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("esya_fiyat_block"));
					return;
				}
			}	
		}	
	}


	char szColumns[QUERY_MAX_LEN], szValues[QUERY_MAX_LEN];

	int iLen = snprintf(szColumns, sizeof(szColumns), "id,owner_id,pos,count,price,price_cheque,vnum, evolution");
	int iUpdateLen = snprintf(szValues, sizeof(szValues), "%u,%u,%d,%u,%d,%d,%u,%d", pkItem->GetID(), ch->GetPlayerID(), bPos, pkItem->GetCount(), iPrice, iPriceCheque, pkItem->GetVnum(), pkItem->GetEvolution());

	if (g_bOfflineShopSocketMax == 3)
	{
		iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ",socket0,socket1,socket2,socket3,socket4,socket5");
		iUpdateLen += snprintf(szValues + iUpdateLen, sizeof(szValues) - iUpdateLen, ",%ld,%ld,%ld,%ld,%ld,%ld", pkItem->GetSocket(0), pkItem->GetSocket(1), pkItem->GetSocket(2), pkItem->GetSocket(3), pkItem->GetSocket(4), pkItem->GetSocket(5));
	}
	else if(g_bOfflineShopSocketMax == 4)
	{
		iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ",socket0,socket1,socket2,socket3");
		iUpdateLen += snprintf(szValues + iUpdateLen, sizeof(szValues) - iUpdateLen, ",%ld,%ld,%ld,%ld", pkItem->GetSocket(0), pkItem->GetSocket(1), pkItem->GetSocket(2), pkItem->GetSocket(3));		
	}
	else if(g_bOfflineShopSocketMax == 5)
	{
		iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ",socket0,socket1,socket2,socket3,socket4");
		iUpdateLen += snprintf(szValues + iUpdateLen, sizeof(szValues) - iUpdateLen, ",%ld,%ld,%ld,%ld,%ld", pkItem->GetSocket(0), pkItem->GetSocket(1), pkItem->GetSocket(2), pkItem->GetSocket(3), pkItem->GetSocket(4));		
	}
	else if(g_bOfflineShopSocketMax == 6)
	{
		iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ",socket0,socket1,socket2,socket3,socket4,socket5");
		iUpdateLen += snprintf(szValues + iUpdateLen, sizeof(szValues) - iUpdateLen, ",%ld,%ld,%ld,%ld,%ld,%ld", pkItem->GetSocket(0), pkItem->GetSocket(1), pkItem->GetSocket(2), pkItem->GetSocket(3), pkItem->GetSocket(4), pkItem->GetSocket(5));		
	}

	iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ", attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7");
	iUpdateLen += snprintf(szValues + iUpdateLen, sizeof(szValues) - iUpdateLen, ",%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
		pkItem->GetAttributeType(0), pkItem->GetAttributeValue(0),
		pkItem->GetAttributeType(1), pkItem->GetAttributeValue(1),
		pkItem->GetAttributeType(2), pkItem->GetAttributeValue(2),
		pkItem->GetAttributeType(3), pkItem->GetAttributeValue(3),
		pkItem->GetAttributeType(4), pkItem->GetAttributeValue(4),
		pkItem->GetAttributeType(5), pkItem->GetAttributeValue(5),
		pkItem->GetAttributeType(6), pkItem->GetAttributeValue(6),
		pkItem->GetAttributeType(7), pkItem->GetAttributeValue(7),
		pkItem->GetAttributeType(8), pkItem->GetAttributeValue(8),
		pkItem->GetAttributeType(9), pkItem->GetAttributeValue(9),
		pkItem->GetAttributeType(10), pkItem->GetAttributeValue(10),
		pkItem->GetAttributeType(11), pkItem->GetAttributeValue(11),
		pkItem->GetAttributeType(12), pkItem->GetAttributeValue(12),
		pkItem->GetAttributeType(12), pkItem->GetAttributeValue(13),
		pkItem->GetAttributeType(14), pkItem->GetAttributeValue(14));

	char szInsertQuery[QUERY_MAX_LEN];
	snprintf(szInsertQuery, sizeof(szInsertQuery), "INSERT INTO %soffline_shop_item (%s) VALUES (%s)", get_table_postfix(), szColumns, szValues);
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szInsertQuery));
	//pkItem->RemoveFromCharacter();
	ITEM_MANAGER::instance().RemoveItem(pkItem);
	
	LPCHARACTER npc = CHARACTER_MANAGER::instance().Find(FindMyOfflineShop(ch->GetPlayerID()));
	if (!npc)
		return;

	LPOFFLINESHOP pkOfflineShop = FindOfflineShop(npc->GetVID());
	if (!pkOfflineShop)
		return;

	pkOfflineShop->BroadcastUpdateItem(bPos, ch->GetPlayerID());
	LogManager::instance().ItemLog(ch, pkItem, "ADD ITEM OFFLINE SHOP", "");
}

void COfflineShopManager::RemoveItem(LPCHARACTER ch, BYTE bPos)
{
	if (!ch)
		return;
	
	if (thecore_pulse() - ch->GetMyOfflineShopTime() < PASSES_PER_SEC(1))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You're too fast! Slow down!"));
		return;
	}

	if (bPos >= OFFLINE_SHOP_HOST_ITEM_MAX_NUM)
	{
		sys_log(0, "OfflineShopManager::RemoveItem - Overflow slot! [%s]", ch->GetName());
		return;
	}

	LPCHARACTER npc = CHARACTER_MANAGER::instance().Find(FindMyOfflineShop(ch->GetPlayerID()));

	// Check npc
	if (!npc)
	{
		char szQuery[1024];
		
		if (g_bOfflineShopSocketMax == 3)
			snprintf(szQuery, sizeof(szQuery), "SELECT pos,count,vnum,socket0,socket1,socket2, socket3,socket4,socket5, evolution, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u and pos = %d", get_table_postfix(), ch->GetPlayerID(), bPos);
		else if(g_bOfflineShopSocketMax == 4)
			snprintf(szQuery, sizeof(szQuery), "SELECT pos,count,vnum,socket0,socket1,socket2,socket3, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u and pos = %d and status = 1", get_table_postfix(), ch->GetPlayerID(), bPos);			
		else if(g_bOfflineShopSocketMax == 5)
			snprintf(szQuery, sizeof(szQuery), "SELECT pos,count,vnum,socket0,socket1,socket2,socket3,socket4, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u and pos = %d and status = 1", get_table_postfix(), ch->GetPlayerID(), bPos);			
		else if(g_bOfflineShopSocketMax == 6)
			snprintf(szQuery, sizeof(szQuery), "SELECT pos,count,vnum,socket0,socket1,socket2,socket3,socket4,socket5, evolution, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u and pos = %d and status = 1", get_table_postfix(), ch->GetPlayerID(), bPos);			
			
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));
		if (pMsg->Get()->uiNumRows == 0)
		{
			sys_log(0, "OfflineShopManager::RemoveItem - This slot is empty! [%s]", ch->GetName());
			return;
		}

		TPlayerItem item;
		int rows;
		if (!(rows = mysql_num_rows(pMsg->Get()->pSQLResult)))
			return;

		for (int i = 0; i < rows; ++i)
		{
			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
			int cur = 0;

			str_to_number(item.pos, row[cur++]);
			str_to_number(item.count, row[cur++]);
			str_to_number(item.vnum, row[cur++]);
			str_to_number(item.alSockets[0], row[cur++]);
			str_to_number(item.alSockets[1], row[cur++]);
			str_to_number(item.alSockets[2], row[cur++]);
			str_to_number(item.alSockets[3], row[cur++]);
			str_to_number(item.alSockets[4], row[cur++]);
			str_to_number(item.alSockets[5], row[cur++]);
			str_to_number(item.evolution, row[cur++]);

			for (int j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; j++)
			{
				str_to_number(item.aAttr[j].bType, row[cur++]);
				str_to_number(item.aAttr[j].sValue, row[cur++]);
			}
		}

		LPITEM pItem = ITEM_MANAGER::instance().CreateItem(item.vnum, item.count);
		if (!pItem)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("There is problem in the system at the moment. Please try again!"));
			return;
		}

		pItem->SetID(item.id);
		pItem->SetAttributes(item.aAttr);
		pItem->SetSockets(item.alSockets);
		pItem->SetEvolution(item.evolution);

		int iEmptyPos;
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

		if (iEmptyPos < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You don't have enough space for store the item!"));
			return;
		}

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
			pItem->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));

		DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_item WHERE owner_id = %u and pos = %d", get_table_postfix(), ch->GetPlayerID(), bPos);
		LogManager::instance().ItemLog(ch, pItem, "DELETE OFFLINE SHOP ITEM", "");
	}
	else
	{
		LPOFFLINESHOP pkOfflineShop = npc->GetOfflineShop();

		// Check pkOfflineShop
		if (!pkOfflineShop)
			return;

		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT pos,count,vnum,socket0,socket1,socket2, socket3,socket4,socket5, evolution, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u and pos = %d", get_table_postfix(), ch->GetPlayerID(), bPos));
		if (pMsg->Get()->uiNumRows == 0)
		{
			sys_log(0, "OfflineShopManager::RemoveItem - This slot is empty! [%s]", ch->GetName());
			return;
		}

		TPlayerItem item;
		int rows;
		if (!(rows = mysql_num_rows(pMsg->Get()->pSQLResult)))
			return;

		for (int i = 0; i < rows; ++i)
		{
			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
			int cur = 0;

			str_to_number(item.pos, row[cur++]);
			str_to_number(item.count, row[cur++]);
			str_to_number(item.vnum, row[cur++]);
			str_to_number(item.alSockets[0], row[cur++]);
			str_to_number(item.alSockets[1], row[cur++]);
			str_to_number(item.alSockets[2], row[cur++]);
			str_to_number(item.alSockets[3], row[cur++]);
			str_to_number(item.alSockets[4], row[cur++]);
			str_to_number(item.alSockets[5], row[cur++]);
			str_to_number(item.evolution, row[cur++]);

			for (int j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; j++)
			{
				str_to_number(item.aAttr[j].bType, row[cur++]);
				str_to_number(item.aAttr[j].sValue, row[cur++]);
			}
		}

		LPITEM pItem = ITEM_MANAGER::instance().CreateItem(item.vnum, item.count);
		if (!pItem)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("There is problem in the system at the moment. Please try again!"));
			return;
		}

		pItem->SetAttributes(item.aAttr);
		pItem->SetSockets(item.alSockets);
		pItem->SetEvolution(item.evolution);
		int iEmptyPos;
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

		if (iEmptyPos < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You don't have enough space for store the item!"));
			return;
		}

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
			pItem->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));

		DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_item WHERE owner_id = %u and pos = %d", get_table_postfix(), ch->GetPlayerID(), bPos);
		pkOfflineShop->BroadcastUpdateItem(bPos, ch->GetPlayerID(), true);

		if (LeftItemCount(ch) == 0)	
			pkOfflineShop->Destroy(npc);
		
		LogManager::instance().ItemLog(ch, pItem, "DELETE OFFLINE SHOP ITEM", "");
	}
}

void COfflineShopManager::ChangePrice(LPCHARACTER ch, BYTE bPos, DWORD dwPrice, DWORD dwPriceCheque)
{
	if (!ch)
		return;
	
	if (thecore_pulse() - ch->GetMyOfflineShopTime() < PASSES_PER_SEC(1))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You're too fast! Slow down!"));
		return;
	}

	if (bPos >= OFFLINE_SHOP_HOST_ITEM_MAX_NUM)
	{
		sys_err("Offlineshop overflow slot count [%s][%d]", ch->GetName(), bPos);
		return;
	}

	LPOFFLINESHOP pkOfflineShop = FindOfflineShop(FindMyOfflineShop(ch->GetPlayerID()));
	if (pkOfflineShop)
		pkOfflineShop->BroadcastUpdatePrice(bPos, dwPrice);

	DBManager::instance().DirectQuery("UPDATE %soffline_shop_item SET price = %u WHERE owner_id = %u and pos = %d", get_table_postfix(), dwPrice, ch->GetPlayerID(), bPos);
	// pkOfflineShop->RemoveGuest(ch);
	sys_log(0, "PRET: %u | ITEM ID: %u | POZITIE: %d", dwPrice, ch->GetPlayerID(), bPos);
}

void COfflineShopManager::Refresh(LPCHARACTER ch)
{
	if (!ch)
		return;

	LPCHARACTER npc = CHARACTER_MANAGER::instance().Find(FindMyOfflineShop(ch->GetPlayerID()));
	if (!npc)
	{
		TPacketGCShop pack;
		pack.header = HEADER_GC_OFFLINE_SHOP;
		pack.subheader = SHOP_SUBHEADER_GC_UPDATE_ITEM2;

		TPacketGCOfflineShopStart pack2;
		memset(&pack2, 0, sizeof(pack2));
		pack2.owner_vid = 0;

		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT pos,count,vnum,price,price_cheque,socket0,socket1,socket2, socket3,socket4,socket5, evolution, attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3, attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6, applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7 FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID()));

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
	else
	{
		LPOFFLINESHOP pkOfflineShop = npc->GetOfflineShop();
		if (!pkOfflineShop)
			return;

		pkOfflineShop->Refresh(ch);
	}
}

void COfflineShopManager::RefreshMoney(LPCHARACTER ch)
{
	if (!ch)
		return;
	
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT money2, money_cheque FROM player.player WHERE id = %u", ch->GetPlayerID()));
	
	TPacketGCShop p;
	TPacketGCOfflineShopMoney p2;
	
	p.header = HEADER_GC_OFFLINE_SHOP;
	p.subheader = SHOP_SUBHEADER_GC_REFRESH_MONEY;
	
	if (pMsg->Get()->uiNumRows == 0)
	{
		p2.dwMoney = 0;
		p2.dwCheque = 0;
		p.size = sizeof(p) + sizeof(p2);
		ch->GetDesc()->BufferedPacket(&p, sizeof(TPacketGCShop));
		ch->GetDesc()->Packet(&p2, sizeof(TPacketGCOfflineShopMoney));
	}
	else
	{
		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
		str_to_number(p2.dwMoney, row[0]);
		str_to_number(p2.dwCheque, row[1]);
		p.size = sizeof(p) + sizeof(p2);
		ch->GetDesc()->BufferedPacket(&p, sizeof(TPacketGCShop));
		ch->GetDesc()->Packet(&p2, sizeof(TPacketGCOfflineShopMoney));
	}
}

DWORD COfflineShopManager::FindMyOfflineShop(DWORD dwPID)
{
	TOfflineShopMap::iterator it = m_Map_pkOfflineShopByNPC2.find(dwPID);
	if (m_Map_pkOfflineShopByNPC2.end() == it)
		return 0;
	
	return it->second;
}

void COfflineShopManager::ChangeOfflineShopTime(LPCHARACTER ch, BYTE bTime)
{
	if (!ch)
		return;
	
	if (thecore_pulse() - ch->GetMyOfflineShopTime() < PASSES_PER_SEC(1))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You're too fast! Slow down!"));
		return;
	}

	// Remember
	DWORD dwOfflineShopVID = FindMyOfflineShop(ch->GetPlayerID());

	if (!dwOfflineShopVID)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't use this option because you don't have offline shop yet!"));
		return;
	}

	LPCHARACTER npc = CHARACTER_MANAGER::instance().Find(FindMyOfflineShop(ch->GetPlayerID()));
	if (npc)
	{
		if (test_server)
			SendNotice("#DEBUG - Offline Shop NPC Found!");

		if (bTime == 4)
		{
			npc->StopOfflineShopUpdateEvent();
		}
		else
		{
			int iTime = 0;
			switch (bTime)
			{
				case 1:
					iTime = 12 * 60 * 60;
					break;
				case 2:
					iTime = 24 * 60 * 60; // 1 gun
					break;
				case 3:
					iTime = 72 * 60 * 60; // 3 gun
					break;
				case 4:
					iTime = 24 * 7 * 60 * 60; // 1 h fta
					break;
			}
			std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE %soffline_shop_npc SET time = %d WHERE owner_id = %u", get_table_postfix(), iTime, ch->GetPlayerID()));
			// ch->ChatPacket(CHAT_TYPE_INFO, "Timp schimbat: %u ora(e)", iTime / 3600);
			sys_log(0, "TIMP: %u | PLAYER ID: %u", iTime, ch->GetPlayerID());
			npc->StopOfflineShopUpdateEvent();
			npc->SetOfflineShopTimer(iTime);
			npc->StartOfflineShopUpdateEvent();
			LogManager::instance().CharLog(ch, 0, "OFFLINE SHOP", "CHANGE TIME");
		}
	}
	else
	{
		TPacketGGChangeOfflineShopTime p;
		p.bHeader = HEADER_GG_CHANGE_OFFLINE_SHOP_TIME;
		p.bTime = bTime;		
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT mapIndex FROM %soffline_shop_npc WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID()));
		if (pMsg->Get()->uiNumRows == 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't use this option!"));
			return;
		}
		
		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
		str_to_number(p.lMapIndex, row[0]);
		p.dwOwnerPID = ch->GetPlayerID();
		
		P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGChangeOfflineShopTime));
	}
}

void COfflineShopManager::StopShopping(LPCHARACTER ch)
{
	LPOFFLINESHOP pkOfflineShop;

	if (!(pkOfflineShop = ch->GetOfflineShop()))
		return;

	pkOfflineShop->RemoveGuest(ch);
	sys_log(0, "OFFLINE_SHOP: END: %s", ch->GetName());
}

void COfflineShopManager::Buy(LPCHARACTER ch, BYTE pos)
{
	if (!ch || !ch->IsPC() || !ch->GetDesc() || ch->GetExchange())
		return;
	if (!ch->GetOfflineShop())
		return;

	if (!ch->GetOfflineShopOwner())
		return;

	if (DISTANCE_APPROX(ch->GetX() - ch->GetOfflineShopOwner()->GetX(), ch->GetY() - ch->GetOfflineShopOwner()->GetY()) > 1500)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상점과의 거리가 너무 멀어 물건을 살 수 없습니다."));
		return;
	}
	
	LPOFFLINESHOP pkOfflineShop = ch->GetOfflineShop();

	if (!pkOfflineShop)
		return;

	int ret = pkOfflineShop->Buy(ch, pos);

	// The result is not equal to SHOP_SUBHEADER_GC_OK, send the error to the character.
	if (SHOP_SUBHEADER_GC_OK != ret)
	{
		TPacketGCShop pack;
		pack.header = HEADER_GC_OFFLINE_SHOP;
		pack.subheader	= ret;
		pack.size	= sizeof(TPacketGCShop);

		if (ch->GetDesc())		
			ch->GetDesc()->Packet(&pack, sizeof(pack));
	}
}

void COfflineShopManager::WithdrawMoney(LPCHARACTER ch, DWORD dwRequiredMoney)
{
	if (!ch)
		return;
	
	if (thecore_pulse() - ch->GetMyOfflineShopTime() < PASSES_PER_SEC(1))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You're too fast! Slow down!"));
		return;
	}

	if (dwRequiredMoney < 0)
		return;

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT money2 FROM player.player WHERE id = %u", ch->GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0)
		return;

	DWORD dwCurrentMoney = 0;
	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	str_to_number(dwCurrentMoney, row[0]);

	if (dwRequiredMoney > dwCurrentMoney)
	{
		// if (test_server)
			// ch->ChatPacket(CHAT_TYPE_INFO, "dwCurrentMoney(%lu) - dwRequiredMoney(%lu)", dwCurrentMoney, dwRequiredMoney);
		
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't enter big number than current money!"));
		return;
	}

	bool isOverFlow = ch->GetGold() + dwRequiredMoney > GOLD_MAX - 1 ? true : false;
	if (isOverFlow)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't withdraw this money at the moment!"));
		return;
	}

	ch->PointChange(POINT_GOLD, dwRequiredMoney, false);
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You received %u yang"), dwRequiredMoney);
	DBManager::instance().DirectQuery("UPDATE player.player SET money2 = money2 - %u WHERE id = %u", dwRequiredMoney, ch->GetPlayerID());
	LogManager::instance().CharLog(ch, 0, "OFFLINE SHOP", "WITHDRAW MONEY");
}

void COfflineShopManager::WithdrawCheque(LPCHARACTER ch, DWORD dwRequiredCheque)
{
	if (!ch)
		return;

	if (thecore_pulse() - ch->GetMyOfflineShopTime() < PASSES_PER_SEC(1))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You're too fast! Slow down!"));
		return;
	}
	
	if (dwRequiredCheque < 0)
		return;

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT money_cheque FROM player.player WHERE id = %u", ch->GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0)
		return;

	DWORD dwCurrentCheque = 0;
	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	str_to_number(dwCurrentCheque, row[0]);

	if (dwRequiredCheque > dwCurrentCheque)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't enter big number than current money!"));
		return;
	}

	bool isOverFlow = ch->GetCheque() + dwRequiredCheque > 99999 - 1 ? true : false;
	if (isOverFlow)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't withdraw this money at the moment!"));
		return;
	}

	ch->PointChange(POINT_CHEQUE, dwRequiredCheque, false);
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You received %lld won"), dwRequiredCheque);
	DBManager::instance().DirectQuery("UPDATE player.player SET money_cheque = money_cheque - %d WHERE id = %u", dwRequiredCheque, ch->GetPlayerID());
	LogManager::instance().CharLog(ch, 0, "OFFLINE SHOP", "WITHDRAW CHEQUE");
}

short COfflineShopManager::LeftItemCount(LPCHARACTER ch)
{
	if (!ch)
		return -1;
	
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT COUNT(*) FROM %soffline_shop_item WHERE owner_id = %u", get_table_postfix(), ch->GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	short bCount = 0;
	str_to_number(bCount, row[0]);
	return bCount;
}