#include "stdafx.h"

#ifdef _ITEM_SHOP_SYSTEM
#include "char.h"
#include "utils.h"
#include "log.h"
#include "db.h"
#include "config.h"
#include "desc.h"
#include "desc_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "desc_client.h"
#include "p2p.h"
#include "item.h"
#include "item_manager.h"
#include "item_shop.h"

const CItemShopManager::TItemShopTable* CItemShopManager::GetTable(DWORD id)
{
	TItemShopDataMap::iterator itor = m_ItemShopDataMap.find(id);

	if (itor == m_ItemShopDataMap.end())
		return NULL;

	return itor->second;
}

bool CItemShopManager::LoadItemShopTable()
{
	SQLMsg * pMsg(DBManager::instance().DirectQuery("SELECT * FROM player.item_shop_table"));
	SQLResult * pRes = pMsg->Get();
	if (pRes->uiAffectedRows <= 0)
	{
		fprintf(stderr, "QUERY_ERROR: SELECT * FROM settings.item_shop_table \n");
		return false;
	}


	if (pRes->uiNumRows)
	{

		MYSQL_ROW row;
		while ((row = mysql_fetch_row(pRes->pSQLResult)))
		{
			DWORD	id, category, sub_category, vnum, count, coinsold, coins, socketzero;

			str_to_number(id, row[0]);
			str_to_number(category, row[1]);
			str_to_number(sub_category, row[2]);
			str_to_number(vnum, row[3]);
			str_to_number(coins, row[4]);
			str_to_number(coinsold, row[5]);
			str_to_number(count, row[6]);
			str_to_number(socketzero, row[7]);
			
			
			const TItemShopTable* p = GetTable(id);

			if (p)
			{
				sys_log(0, "Already Inserted List %d (ItemShop Table)", id);
				continue;
			}

			TItemShopTable* pItemShopData = new TItemShopTable;
			pItemShopData->category = category;
			pItemShopData->sub_category = sub_category;
			pItemShopData->id = id;
			pItemShopData->vnum = vnum;
			pItemShopData->count = count;
			pItemShopData->coinsold = coinsold;
			pItemShopData->coins = coins;
			pItemShopData->socketzero = socketzero;
			m_ItemShopDataMap.insert(TItemShopDataMap::value_type(id, pItemShopData));
			sys_log(0, "ItemShop Insert ID:%d VNUM:%d COUNT:%d", id, vnum, count);
		}
	}
	delete pMsg;
	return true;
}

// Send item data list client 
void CItemShopManager::SendClientPacket(LPCHARACTER ch)
{
	if (NULL == ch)
		return;

	if (!ch || !ch->GetDesc())
		return;
	
	ch->ChatPacket(CHAT_TYPE_COMMAND, "ItemShopDataClear");

	for (auto itor=m_ItemShopDataMap.begin(); itor!=m_ItemShopDataMap.end(); ++itor)
	{
		TItemShopTable * pTable = itor->second;
		if (pTable)
		{
			
			TPacketItemShopData pack;
			pack.header = HEADER_GC_ITEM_SHOP;

			pack.id = pTable->id;
			pack.category = pTable->category;
			pack.sub_category = pTable->sub_category;
			pack.vnum = pTable->vnum;
			pack.count = pTable->count;
			pack.coinsold = pTable->coinsold;
			pack.coins = pTable->coins;
			pack.socketzero = pTable->socketzero;

			ch->GetDesc()->Packet(&pack, sizeof(pack)); 
		}
	}
}

bool CItemShopManager::Buy(LPCHARACTER ch, DWORD id, DWORD count)
{
	if (!ch)
		return false;

	if (count <= 0)
		return false;

	const TItemShopTable* c_pTable = GetTable(id);

	if (!c_pTable)
	{
		sys_err("%s has request buy unknown id(%d) item", ch->GetName(), id);
		return false;
	}

	DWORD dwCoins = c_pTable->coins;
	DWORD dwVnum = c_pTable->vnum;
	DWORD dwCount = c_pTable->count;
	DWORD dwSocketZero = c_pTable->socketzero;

	DWORD dwRealCount = dwCount * count;

	if (ch->GetDragonCoin() < (dwCoins * count))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("nesnemarketyeterliepyok"));
		return false;
	}

	LPITEM pkItem = ITEM_MANAGER::instance().CreateItem(dwVnum, dwRealCount, 0, true, -1);

	if (!pkItem)
		return false;

	int iEmptyPos;
	if (pkItem->IsDragonSoul())
	{
		iEmptyPos = ch->GetEmptyDragonSoulInventory(pkItem);
	}
#ifdef ENABLE_SPECIAL_STORAGE
	else if (pkItem->IsUpgradeItem())
		iEmptyPos = ch->GetEmptyUpgradeInventory(pkItem);
	else if (pkItem->IsStone())
		iEmptyPos = ch->GetEmptyStoneInventory(pkItem);
	else if (pkItem->IsChest())
		iEmptyPos = ch->GetEmptyChestInventory(pkItem);
	else if (pkItem->IsAttr())
		iEmptyPos = ch->GetEmptyAttrInventory(pkItem);
#endif
	else
	{
		iEmptyPos = ch->GetEmptyInventory(pkItem->GetSize());
	}

	if (iEmptyPos < 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("nesnemarketenvanterdebosyer"));
		M2_DESTROY_ITEM(pkItem);
		return false;
	}

	ch->SetDragonCoin(ch->GetDragonCoin() - (dwCoins * count));

	if (pkItem->IsRealTimeItem())
		pkItem->SetSocket(0, get_global_time() + dwSocketZero);
	else if (pkItem->GetType() == ITEM_ARMOR || pkItem->GetType() == ITEM_WEAPON)
		pkItem->SetSocket(0, 1);
	else if (pkItem->GetType() == ITEM_COSTUME && pkItem->GetSubType() == COSTUME_SASH)
		pkItem->SetSocket(0, 0);
	else if (pkItem->GetType() == ITEM_COSTUME && pkItem->GetSubType() == COSTUME_AURA)
		pkItem->SetSocket(0, 0);
	else
	{
		if (pkItem->GetType() != ITEM_BLEND)
			pkItem->SetSocket(0, dwSocketZero);
	}

	if (pkItem->IsDragonSoul())
		pkItem->AddToCharacter(ch, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyPos));
	else
		pkItem->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));

	ITEM_MANAGER::instance().FlushDelayedSave(pkItem);

	return true;
}

void CItemShopManager::Destroy()
{
	for (TItemShopDataMap::iterator itor = m_ItemShopDataMap.begin(); itor != m_ItemShopDataMap.end(); ++itor)
	{
		delete itor->second;
	}
	m_ItemShopDataMap.clear();
}

CItemShopManager::CItemShopManager()
{
}

CItemShopManager::~CItemShopManager()
{
	Destroy();
}
#endif