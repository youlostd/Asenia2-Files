
#include "stdafx.h"
#include "Cache.h"

#include "QID.h"
#include "ClientManager.h"
#ifdef __AUCTION__
#include "AuctionManager.h"
#endif
#include "Main.h"

extern CPacketInfo g_item_info;
extern int g_iPlayerCacheFlushSeconds;
extern int g_iItemCacheFlushSeconds;
extern int g_test_server;
// MYSHOP_PRICE_LIST
extern int g_iItemPriceListTableCacheFlushSeconds;
// END_OF_MYSHOP_PRICE_LIST
//
extern int g_item_count;
const int auctionMinFlushSec = 1800;

#ifdef __SKILL_COLOR_SYSTEM__
extern int g_iSkillColorCacheFlushSeconds;

CSKillColorCache::CSKillColorCache()
{
	m_expireTime = MIN(1800, g_iSkillColorCacheFlushSeconds);
}

CSKillColorCache::~CSKillColorCache()
{
}

void CSKillColorCache::OnFlush()
{
	char query[QUERY_MAX_LEN];
	snprintf(query, sizeof(query),
		"REPLACE INTO skill_color%s (`player_id`, "
		"`s1_col1`, `s1_col2`, `s1_col3`, `s1_col4`, `s1_col5`, "
		"`s2_col1`, `s2_col2`, `s2_col3`, `s2_col4`, `s2_col5`, "
		"`s3_col1`, `s3_col2`, `s3_col3`, `s3_col4`, `s3_col5`, "
		"`s4_col1`, `s4_col2`, `s4_col3`, `s4_col4`, `s4_col5`, "
		"`s5_col1`, `s5_col2`, `s5_col3`, `s5_col4`, `s5_col5`, "
		"`s6_col1`, `s6_col2`, `s6_col3`, `s6_col4`, `s6_col5`, " // end of skills
		"`s7_col1`, `s7_col2`, `s7_col3`, `s7_col4`, `s7_col5`, " // begin buffs
		"`s8_col1`, `s8_col2`, `s8_col3`, `s8_col4`, `s8_col5`, "
		"`s9_col1`, `s9_col2`, `s9_col3`, `s9_col4`, `s9_col5`, "
		"`s10_col1`, `s10_col2`, `s10_col3`, `s10_col4`, `s10_col5`, "
		"`s11_col1`, `s11_col2`, `s11_col3`, `s11_col4`, `s11_col5`) "
		"VALUES (%d, "
		"%d, %d, %d, %d, %d, "
		"%d, %d, %d, %d, %d, "
		"%d, %d, %d, %d, %d, "
		"%d, %d, %d, %d, %d, "
		"%d, %d, %d, %d, %d, "
		"%d, %d, %d, %d, %d, " // end of skills
		"%d, %d, %d, %d, %d, " // begin buffs
		"%d, %d, %d, %d, %d, "
		"%d, %d, %d, %d, %d, "
		"%d, %d, %d, %d, %d, "
		"%d, %d, %d, %d, %d)", GetTablePostfix(), m_data.player_id,
		m_data.dwSkillColor[0][0], m_data.dwSkillColor[0][1], m_data.dwSkillColor[0][2], m_data.dwSkillColor[0][3], m_data.dwSkillColor[0][4],
		m_data.dwSkillColor[1][0], m_data.dwSkillColor[1][1], m_data.dwSkillColor[1][2], m_data.dwSkillColor[1][3], m_data.dwSkillColor[1][4],
		m_data.dwSkillColor[2][0], m_data.dwSkillColor[2][1], m_data.dwSkillColor[2][2], m_data.dwSkillColor[2][3], m_data.dwSkillColor[2][4],
		m_data.dwSkillColor[3][0], m_data.dwSkillColor[3][1], m_data.dwSkillColor[3][2], m_data.dwSkillColor[3][3], m_data.dwSkillColor[3][4],
		m_data.dwSkillColor[4][0], m_data.dwSkillColor[4][1], m_data.dwSkillColor[4][2], m_data.dwSkillColor[4][3], m_data.dwSkillColor[4][4],
		m_data.dwSkillColor[5][0], m_data.dwSkillColor[5][1], m_data.dwSkillColor[5][2], m_data.dwSkillColor[5][3], m_data.dwSkillColor[5][4], // end of skills
		m_data.dwSkillColor[6][0], m_data.dwSkillColor[6][1], m_data.dwSkillColor[6][2], m_data.dwSkillColor[6][3], m_data.dwSkillColor[6][4], // begin buffs
		m_data.dwSkillColor[7][0], m_data.dwSkillColor[7][1], m_data.dwSkillColor[7][2], m_data.dwSkillColor[7][3], m_data.dwSkillColor[7][4],
		m_data.dwSkillColor[8][0], m_data.dwSkillColor[8][1], m_data.dwSkillColor[8][2], m_data.dwSkillColor[8][3], m_data.dwSkillColor[8][4],
		m_data.dwSkillColor[9][0], m_data.dwSkillColor[9][1], m_data.dwSkillColor[9][2], m_data.dwSkillColor[9][3], m_data.dwSkillColor[9][4],
		m_data.dwSkillColor[10][0], m_data.dwSkillColor[10][1], m_data.dwSkillColor[10][2], m_data.dwSkillColor[10][3], m_data.dwSkillColor[10][4]);

	CDBManager::instance().ReturnQuery(query, QID_SKILL_COLOR_SAVE, 0, NULL);

	if (g_test_server)
		sys_log(0, "SkillColorCache::Flush :REPLACE %u (%s)", m_data.player_id, query);

	m_bNeedQuery = false;
}
#endif

CItemCache::CItemCache()
{
	m_expireTime = MIN(1800, g_iItemCacheFlushSeconds);
}

CItemCache::~CItemCache()
{
}

// �̰� �̻��ѵ�...
// Delete�� ������, Cache�� �����ؾ� �ϴ°� �ƴѰ�???
// �ٵ� Cache�� �����ϴ� �κ��� ����.
// �� ã�� �ǰ�?
// �̷��� �س�����, ��� �ð��� �� ������ �������� ��� ����...
// �̹� ����� �������ε�... Ȯ�λ��??????
// fixme
// by rtsummit
void CItemCache::Delete()
{
	if (m_data.vnum == 0)
		return;

	//char szQuery[QUERY_MAX_LEN];
	//szQuery[QUERY_MAX_LEN] = '\0';
	if (g_test_server)
		sys_log(0, "ItemCache::Delete : DELETE %u", m_data.id);

	m_data.vnum = 0;
	m_bNeedQuery = true;
	m_lastUpdateTime = time(0);
	OnFlush();
	
	//m_bNeedQuery = false;
	//m_lastUpdateTime = time(0) - m_expireTime; // �ٷ� Ÿ�Ӿƿ� �ǵ��� ����.
}

void CItemCache::OnFlush()
{
	if (m_data.vnum == 0) // vnum�� 0�̸� �����϶�� ǥ�õ� ���̴�.
	{
		char szQuery[QUERY_MAX_LEN];
		snprintf(szQuery, sizeof(szQuery), "DELETE FROM item%s WHERE id=%u", GetTablePostfix(), m_data.id);
		CDBManager::instance().ReturnQuery(szQuery, QID_ITEM_DESTROY, 0, NULL);

		if (g_test_server)
			sys_log(0, "ItemCache::Flush : DELETE %u %s", m_data.id, szQuery);
	}
	else
	{
		long alSockets[ITEM_SOCKET_MAX_NUM];
		TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
		bool isSocket = false, isAttr = false;

		memset(&alSockets, 0, sizeof(long) * ITEM_SOCKET_MAX_NUM);
		memset(&aAttr, 0, sizeof(TPlayerItemAttribute) * ITEM_ATTRIBUTE_MAX_NUM);

		TPlayerItem * p = &m_data;

		if (memcmp(alSockets, p->alSockets, sizeof(long) * ITEM_SOCKET_MAX_NUM))
			isSocket = true;

		if (memcmp(aAttr, p->aAttr, sizeof(TPlayerItemAttribute) * ITEM_ATTRIBUTE_MAX_NUM))
			isAttr = true;

		char szColumns[QUERY_MAX_LEN];
		char szValues[QUERY_MAX_LEN];
		char szUpdate[QUERY_MAX_LEN];

		int iLen = snprintf(szColumns, sizeof(szColumns), "id, owner_id, window, pos, count, vnum, evolution");

		int iValueLen = snprintf(szValues, sizeof(szValues), "%u, %u, %d, %d, %u, %u, %d",
				p->id, p->owner, p->window, p->pos, p->count, p->vnum, p->evolution);

		int iUpdateLen = snprintf(szUpdate, sizeof(szUpdate), "owner_id=%u, window=%d, pos=%d, count=%u, vnum=%u, evolution=%d",
				p->owner, p->window, p->pos, p->count, p->vnum, p->evolution);

		if (isSocket)
		{
			iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ", socket0, socket1, socket2, socket3, socket4, socket5");
			iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen,
					", %lu, %lu, %lu, %lu, %lu, %lu", p->alSockets[0], p->alSockets[1], p->alSockets[2], p->alSockets[3], p->alSockets[4], p->alSockets[5]);
			iUpdateLen += snprintf(szUpdate + iUpdateLen, sizeof(szUpdate) - iUpdateLen,
					", socket0=%lu, socket1=%lu, socket2=%lu, socket3=%lu, socket4=%lu, socket5=%lu", p->alSockets[0], p->alSockets[1], p->alSockets[2], p->alSockets[3], p->alSockets[4], p->alSockets[5]);
		}

		if (isAttr)
		{
			iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, 
				", attrtype0, attrvalue0, attrtype1, attrvalue1, attrtype2, attrvalue2, attrtype3, attrvalue3"
				", attrtype4, attrvalue4, attrtype5, attrvalue5, attrtype6, attrvalue6"
#ifdef ENABLE_NEW_PET_SYSTEM
				", applytype0, applyvalue0, applytype1, applyvalue1, applytype2, applyvalue2, applytype3, applyvalue3, applytype4, applyvalue4, applytype5, applyvalue5, applytype6, applyvalue6, applytype7, applyvalue7"
#endif
			);

			iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen,
				", %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d"
#ifdef ENABLE_NEW_PET_SYSTEM
				", %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d"
#endif
				,
				p->aAttr[0].bType, p->aAttr[0].sValue,
				p->aAttr[1].bType, p->aAttr[1].sValue,
				p->aAttr[2].bType, p->aAttr[2].sValue,
				p->aAttr[3].bType, p->aAttr[3].sValue,
				p->aAttr[4].bType, p->aAttr[4].sValue,
				p->aAttr[5].bType, p->aAttr[5].sValue,
				p->aAttr[6].bType, p->aAttr[6].sValue
#ifdef ENABLE_NEW_PET_SYSTEM
				, p->aAttr[7].bType, p->aAttr[7].sValue,
				p->aAttr[8].bType, p->aAttr[8].sValue,
				p->aAttr[9].bType, p->aAttr[9].sValue,
				p->aAttr[10].bType, p->aAttr[10].sValue,
				p->aAttr[11].bType, p->aAttr[11].sValue,
				p->aAttr[12].bType, p->aAttr[12].sValue,
				p->aAttr[13].bType, p->aAttr[13].sValue,
				p->aAttr[14].bType, p->aAttr[14].sValue
#endif
			);

			iUpdateLen += snprintf(szUpdate + iUpdateLen, sizeof(szUpdate) - iUpdateLen,
				", attrtype0=%d, attrvalue0=%d"
				", attrtype1=%d, attrvalue1=%d"
				", attrtype2=%d, attrvalue2=%d"
				", attrtype3=%d, attrvalue3=%d"
				", attrtype4=%d, attrvalue4=%d"
				", attrtype5=%d, attrvalue5=%d"
				", attrtype6=%d, attrvalue6=%d"
#ifdef ENABLE_NEW_PET_SYSTEM
				", applytype0=%d, applyvalue0=%d"
				", applytype1=%d, applyvalue1=%d"
				", applytype2=%d, applyvalue2=%d"
				", applytype3=%d, applyvalue3=%d"
				", applytype4=%d, applyvalue4=%d"
				", applytype5=%d, applyvalue5=%d"
				", applytype6=%d, applyvalue6=%d"
				", applytype7=%d, applyvalue7=%d"
#endif
				,
				p->aAttr[0].bType, p->aAttr[0].sValue,
				p->aAttr[1].bType, p->aAttr[1].sValue,
				p->aAttr[2].bType, p->aAttr[2].sValue,
				p->aAttr[3].bType, p->aAttr[3].sValue,
				p->aAttr[4].bType, p->aAttr[4].sValue,
				p->aAttr[5].bType, p->aAttr[5].sValue,
				p->aAttr[6].bType, p->aAttr[6].sValue
#ifdef ENABLE_NEW_PET_SYSTEM
				, p->aAttr[7].bType, p->aAttr[7].sValue,
				p->aAttr[8].bType, p->aAttr[8].sValue,
				p->aAttr[9].bType, p->aAttr[9].sValue,
				p->aAttr[10].bType, p->aAttr[10].sValue,
				p->aAttr[11].bType, p->aAttr[11].sValue,
				p->aAttr[12].bType, p->aAttr[12].sValue,
				p->aAttr[13].bType, p->aAttr[13].sValue,
				p->aAttr[14].bType, p->aAttr[14].sValue
#endif
			);
		}

		char szItemQuery[QUERY_MAX_LEN + QUERY_MAX_LEN];
		snprintf(szItemQuery, sizeof(szItemQuery), "REPLACE INTO item%s (%s) VALUES(%s)", GetTablePostfix(), szColumns, szValues);

		if (g_test_server)	
			sys_log(0, "ItemCache::Flush :REPLACE  (%s)", szItemQuery);

		CDBManager::instance().ReturnQuery(szItemQuery, QID_ITEM_SAVE, 0, NULL);

		//g_item_info.Add(p->vnum);
		++g_item_count;
	}

	m_bNeedQuery = false;
}

//
// CPlayerTableCache
//
CPlayerTableCache::CPlayerTableCache()
{
	m_expireTime = MIN(1800, g_iPlayerCacheFlushSeconds);
}

CPlayerTableCache::~CPlayerTableCache()
{
}

void CPlayerTableCache::OnFlush()
{
	if (g_test_server)
		sys_log(0, "PlayerTableCache::Flush : %s", m_data.name);

	char szQuery[QUERY_MAX_LEN];
	CreatePlayerSaveQuery(szQuery, sizeof(szQuery), &m_data);
	CDBManager::instance().ReturnQuery(szQuery, QID_PLAYER_SAVE, 0, NULL);
}

// MYSHOP_PRICE_LIST
//
// CItemPriceListTableCache class implementation
//

const int CItemPriceListTableCache::s_nMinFlushSec = 1800;

CItemPriceListTableCache::CItemPriceListTableCache()
{
	m_expireTime = MIN(s_nMinFlushSec, g_iItemPriceListTableCacheFlushSeconds);
}

void CItemPriceListTableCache::UpdateList(const TItemPriceListTable* pUpdateList)
{
	//
	// �̹� ĳ�̵� �����۰� �ߺ��� �������� ã�� �ߺ����� �ʴ� ���� ������ tmpvec �� �ִ´�.
	//

	std::vector<TItemPriceInfo> tmpvec;

	for (uint idx = 0; idx < m_data.byCount; ++idx)
	{
		const TItemPriceInfo* pos = pUpdateList->aPriceInfo;
		for (; pos != pUpdateList->aPriceInfo + pUpdateList->byCount && m_data.aPriceInfo[idx].dwVnum != pos->dwVnum; ++pos)
			;

		if (pos == pUpdateList->aPriceInfo + pUpdateList->byCount)
			tmpvec.push_back(m_data.aPriceInfo[idx]);
	}

	//
	// pUpdateList �� m_data �� �����ϰ� ���� ������ tmpvec �� �տ��� ���� ���� ��ŭ �����Ѵ�.
	// 

	if (pUpdateList->byCount > SHOP_PRICELIST_MAX_NUM)
	{
		sys_err("Count overflow!");
		return;
	}

	m_data.byCount = pUpdateList->byCount;

	thecore_memcpy(m_data.aPriceInfo, pUpdateList->aPriceInfo, sizeof(TItemPriceInfo) * pUpdateList->byCount);

	int nDeletedNum;	// ������ ���������� ����

	if (pUpdateList->byCount < SHOP_PRICELIST_MAX_NUM)
	{
		size_t sizeAddOldDataSize = SHOP_PRICELIST_MAX_NUM - pUpdateList->byCount;

		if (tmpvec.size() < sizeAddOldDataSize)
			sizeAddOldDataSize = tmpvec.size();

		thecore_memcpy(m_data.aPriceInfo + pUpdateList->byCount, &tmpvec[0], sizeof(TItemPriceInfo) * sizeAddOldDataSize);
		m_data.byCount += sizeAddOldDataSize;

		nDeletedNum = tmpvec.size() - sizeAddOldDataSize;
	}
	else
		nDeletedNum = tmpvec.size();

	m_bNeedQuery = true;

	sys_log(0, 
			"ItemPriceListTableCache::UpdateList : OwnerID[%u] Update [%u] Items, Delete [%u] Items, Total [%u] Items", 
			m_data.dwOwnerID, pUpdateList->byCount, nDeletedNum, m_data.byCount);
}

void CItemPriceListTableCache::OnFlush()
{
	char szQuery[QUERY_MAX_LEN];

	//
	// �� ĳ���� �����ڿ� ���� ������ DB �� ����� ������ ���������� ��� �����Ѵ�.
	//

	snprintf(szQuery, sizeof(szQuery), "DELETE FROM myshop_pricelist%s WHERE owner_id = %u", GetTablePostfix(), m_data.dwOwnerID);
	CDBManager::instance().ReturnQuery(szQuery, QID_ITEMPRICE_DESTROY, 0, NULL);

	//
	// ĳ���� ������ ��� DB �� ����.
	//

	for (int idx = 0; idx < m_data.byCount; ++idx)
	{
		snprintf(szQuery, sizeof(szQuery),
#ifdef ENABLE_CHEQUE_SYSTEM
				"INSERT INTO myshop_pricelist%s(owner_id, item_vnum, price) VALUES(%u, %u, %u, %u)", 
				GetTablePostfix(), m_data.dwOwnerID, m_data.aPriceInfo[idx].dwVnum, m_data.aPriceInfo[idx].dwPrice, m_data.aPriceInfo[idx].dwPriceCheque);
#else
				"INSERT INTO myshop_pricelist%s(owner_id, item_vnum, price) VALUES(%u, %u, %u)", 
				GetTablePostfix(), m_data.dwOwnerID, m_data.aPriceInfo[idx].dwVnum, m_data.aPriceInfo[idx].dwPrice);
#endif
		CDBManager::instance().ReturnQuery(szQuery, QID_ITEMPRICE_SAVE, 0, NULL);
	}

	sys_log(0, "ItemPriceListTableCache::Flush : OwnerID[%u] Update [%u]Items", m_data.dwOwnerID, m_data.byCount);
	
	m_bNeedQuery = false;
}
// END_OF_MYSHOP_PRICE_LIST
#ifdef __AUCTION__
CAuctionItemInfoCache::CAuctionItemInfoCache()
{
	m_expireTime = MIN (auctionMinFlushSec, g_iItemCacheFlushSeconds);
}

CAuctionItemInfoCache::~CAuctionItemInfoCache()
{

}

void CAuctionItemInfoCache::Delete()
{
	if (m_data.item_num == 0)
		return;

	if (g_test_server)
		sys_log(0, "CAuctionItemInfoCache::Delete : DELETE %u", m_data.item_id);

	m_data.item_num = 0;
	m_bNeedQuery = true;
	m_lastUpdateTime = time(0);
	OnFlush();
	delete this;
}

void CAuctionItemInfoCache::OnFlush()
{
	char szQuery[QUERY_MAX_LEN];
	
	if (m_data.item_num == 0)
	{
		snprintf(szQuery, sizeof(szQuery), "DELETE FROM auction where item_id = %d", m_data.item_id);
		CDBManager::instance().AsyncQuery(szQuery);
	}
	else
	{
		snprintf(szQuery, sizeof(szQuery), "REPLACE INTO auction VALUES (%u, %d, %d, %u, \"%s\", %u, %u, %u, %u)", 
			m_data.item_num, m_data.offer_price, m_data.price, m_data.offer_id, m_data.shown_name, (DWORD)m_data.empire, (DWORD)m_data.expired_time, 
			m_data.item_id, m_data.bidder_id);

		CDBManager::instance().AsyncQuery(szQuery);
	}
}

CSaleItemInfoCache::CSaleItemInfoCache()
{
	m_expireTime = MIN (auctionMinFlushSec, g_iItemCacheFlushSeconds);
}

CSaleItemInfoCache::~CSaleItemInfoCache()
{
}

void CSaleItemInfoCache::Delete()
{
}

void CSaleItemInfoCache::OnFlush()
{
	char szQuery[QUERY_MAX_LEN];
	
	snprintf(szQuery, sizeof(szQuery), "REPLACE INTO sale VALUES (%u, %d, %d, %u, \"%s\", %u, %u, %u, %u)", 
		m_data.item_num, m_data.offer_price, m_data.price, m_data.offer_id, m_data.shown_name, (DWORD)m_data.empire, (DWORD)m_data.expired_time,
		m_data.item_id, m_data.wisher_id);
	
	CDBManager::instance().AsyncQuery(szQuery);
}

CWishItemInfoCache::CWishItemInfoCache()
{
	m_expireTime = MIN (auctionMinFlushSec, g_iItemCacheFlushSeconds);
}

CWishItemInfoCache::~CWishItemInfoCache()
{
}

void CWishItemInfoCache::Delete()
{
}

void CWishItemInfoCache::OnFlush()
{
	char szQuery[QUERY_MAX_LEN];
	
	snprintf(szQuery, sizeof(szQuery), "REPLACE INTO wish VALUES (%u, %d, %d, %u, \"%s\", %u, %d)", 
		m_data.item_num, m_data.offer_price, m_data.price, m_data.offer_id, m_data.shown_name, (DWORD)m_data.empire, (DWORD)m_data.expired_time);
	
	CDBManager::instance().AsyncQuery(szQuery);
}
#endif

