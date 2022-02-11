#include "stdafx.h"
#include "constants.h"
#include "config.h"
#include "utils.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "protocol.h"
#include "char.h"
#include "char_manager.h"
#include "item.h"
#include "item_manager.h"
#include "cmd.h"
#include "shop.h"
#include "shop_manager.h"
#include "safebox.h"
#include "regen.h"
#include "battle.h"
#include "exchange.h"
#include "questmanager.h"
#include "profiler.h"
#include "messenger_manager.h"
#include "party.h"
#include "p2p.h"
#include "affect.h"
#include "guild.h"
#include "guild_manager.h"
#include "log.h"
#include "banword.h"
#include "empire_text_convert.h"
#include "unique_item.h"
#include "building.h"
#include "locale_service.h"
#include "gm.h"
#include "spam.h"
#include "ani.h"
#include "motion.h"
#include "OXEvent.h"
#include "locale_service.h"
#include "HackShield.h"
#include "XTrapManager.h"
#include "DragonSoul.h"
#include "offline_shop.h"
#include "offlineshop_manager.h"
#ifdef ENABLE_SWITCHBOT
#include "new_switchbot.h"
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
#include "New_PetSystem.h"
#endif
extern void SendShout(const char * szText, BYTE bEmpire);
extern int g_nPortalLimitTime;

static int __deposit_limit()
{
	return (1000*10000); // 1Ãµ¸¸
}

// #ifdef SHOP_SEARCH
	#include "target.h"
	class CFuncOfflineShopView
	{
		private:
			int dwViewRange;
			std::string word;

		public:
			int shopCount;
			LPENTITY m_me;
			long dwPrice;
			CFuncOfflineShopView(LPENTITY ent, const std::string& kelime, long price) :
				dwViewRange(VIEW_RANGE + VIEW_BONUS_RANGE),
				m_me(ent), word(kelime), dwPrice(price)
			{
				shopCount = 0;
			}
			void operator () (LPENTITY ent)
			{
				if (!m_me->GetDesc())
					return;
				
				if (ent->IsType(ENTITY_CHARACTER))
				{
					LPCHARACTER chMe = (LPCHARACTER)m_me;
					LPCHARACTER chEnt = (LPCHARACTER)ent;
					if (chMe && chEnt && chEnt->IsOfflineShopNPC())
					{
						if (dwPrice == 0 && word.length() == 1)
						{
							TargetInfo * pInfo_check = CTargetManager::instance().GetTargetInfo(chMe->GetPlayerID(), TARGET_TYPE_SHOP, chEnt->GetVID());
							if (pInfo_check)
								CTargetManager::Instance().DeleteTarget(chMe->GetPlayerID(), chEnt->GetVID(), "SHOP_SEARCH_TARGET");

							return;
						}
						LPOFFLINESHOP shop = chEnt->GetOfflineShop();
						if (!shop)
							return;

						if (!shop->SearchItem(word, dwPrice))
							return;

						if (!CTargetManager::instance().GetTargetInfo(chMe->GetPlayerID(), TARGET_TYPE_SHOP, chEnt->GetVID()))
							CTargetManager::Instance().CreateTarget(chMe->GetPlayerID(), chEnt->GetVID(), "SHOP_SEARCH_TARGET", TARGET_TYPE_SHOP, chEnt->GetVID(), 0, chEnt->GetMapIndex(), "shop");

						shopCount++;
						// sys_log(0, "Bulunan pazar %d", shopCount);
					}
				}
			}
	};

	void ClearShopSearch(LPCHARACTER ch)
	{
		CFuncOfflineShopView f(ch, "s", 0);
		if (ch && ch->GetSectree())
			ch->GetSectree()->ForEachAround(f);
	}
// #endif

#ifdef __SEND_TARGET_INFO__
void CInputMain::TargetInfoLoad(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGTargetInfoLoad* p = (TPacketCGTargetInfoLoad*)c_pData;
	TPacketGCTargetInfo pInfo;
	pInfo.header = HEADER_GC_TARGET_INFO;
	static std::vector<LPITEM> s_vec_item;
	s_vec_item.clear();
	LPITEM pkInfoItem;
	LPCHARACTER m_pkChrTarget = CHARACTER_MANAGER::instance().Find(p->dwVID);
	
	if (!ch || !m_pkChrTarget)
		return;

	// if (m_pkChrTarget && (m_pkChrTarget->IsMonster() || m_pkChrTarget->IsStone()))
	// {
		// if (thecore_heart->pulse - (int) ch->GetLastTargetInfoPulse() < passes_per_sec * 3)
			// return;

		// ch->SetLastTargetInfoPulse(thecore_heart->pulse);

	if (ITEM_MANAGER::instance().CreateDropItemVector(m_pkChrTarget, ch, s_vec_item) && (m_pkChrTarget->IsMonster() || m_pkChrTarget->IsStone()))
	{
		if (s_vec_item.size() == 0);
		else if (s_vec_item.size() == 1)
		{
			pkInfoItem = s_vec_item[0];
			pInfo.dwVID	= m_pkChrTarget->GetVID();
			pInfo.race = m_pkChrTarget->GetRaceNum();
			pInfo.dwVnum = pkInfoItem->GetVnum();
			pInfo.count = pkInfoItem->GetCount();
			ch->GetDesc()->Packet(&pInfo, sizeof(TPacketGCTargetInfo));
		}
		else
		{
			int iItemIdx = s_vec_item.size() - 1;
			while (iItemIdx >= 0)
			{
				pkInfoItem = s_vec_item[iItemIdx--];

				if (!pkInfoItem)
				{
					sys_err("pkInfoItem null in vector idx %d", iItemIdx + 1);
					continue;
				}

					pInfo.dwVID	= m_pkChrTarget->GetVID();
					pInfo.race = m_pkChrTarget->GetRaceNum();
					pInfo.dwVnum = pkInfoItem->GetVnum();
					pInfo.count = pkInfoItem->GetCount();
					ch->GetDesc()->Packet(&pInfo, sizeof(TPacketGCTargetInfo));
			}
		}
	}
	// }
}
#endif

void CInputMain::Shop2(LPCHARACTER ch, const char * data)
{	
	struct command_shop2 * p = (struct command_shop2 *) data;
	int shop2_subheader = (int)p->subheader;
	if (!ch || !ch->IsPC() || !ch->GetDesc())
		return;

	if (shop2_subheader == SHOP2_SUBHEADER_CG_SEARCH)
	{
		if (quest::CQuestManager::instance().GetPCForce(ch->GetPlayerID())->IsRunning())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("suan kullanamazsiniz"));
			return;
		}
		if (strlen(p->cItemNameForSearch))
		{
			if (get_global_time() < ch->GetQuestFlag("search.searchlast")) {
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("suan kullanamazsiniz"));
				return;
			}
			std::string itemName = p->cItemNameForSearch;
			if (strlen(p->cItemNameForSearch) == 1)
				ClearShopSearch(ch);
			else {
				ClearShopSearch(ch);
				LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(ch->GetMapIndex());
				if (pMap)
				{
					CFuncOfflineShopView f(ch, itemName, p->iMinPrice);
					pMap->for_each(f);
					if (f.shopCount > 0)
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s iceren %d adet pazar bulundu"), itemName.c_str(), f.shopCount);
					else
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s iceren pazar bulunamadi"), itemName.c_str());
				}
				ch->SetQuestFlag("search.searchlast", get_global_time() + 2);
			}
		}
	}
	else
	{
		sys_err("CInputMain::Shop2 : Unknown subheader '%d' - From: %s", shop2_subheader, ch->GetName());
		return;
	}
}

#ifdef ENABLE_CUBE_RENEWAL
void CInputMain::CubeRenewalSend(LPCHARACTER ch, const char* data)
{
	struct packet_send_cube_renewal* pinfo = (struct packet_send_cube_renewal*) data;
	switch (pinfo->subheader)
	{
	case CUBE_RENEWAL_SUB_HEADER_MAKE_ITEM:
	{
		int index_item = pinfo->index_item;
		int count_item = pinfo->count_item;
		int index_item_improve = pinfo->index_item_improve;

		Cube_Make(ch, index_item, count_item, index_item_improve);
	}
	break;

	case CUBE_RENEWAL_SUB_HEADER_CLOSE:
	{
		Cube_close(ch);
	}
	break;
	}
}
#endif
#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
bool CompareItemVnumAcPriceAC(COfflineShop::OFFLINE_SHOP_ITEM i, COfflineShop::OFFLINE_SHOP_ITEM j)
{
	return (i.vnum < j.vnum) && (i.price < j.price);
}
void CInputMain::ShopSearch(LPCHARACTER ch, const char * data)
{
	TPacketCGShopSearch * pinfo = (TPacketCGShopSearch *)data;

	if (!ch)
		return;

	if (!data)
		return;

	if (ch->IsOpenSafebox() || ch->GetShop() || ch->IsCubeOpen() || ch->IsDead() || ch->GetExchange() || ch->GetOfflineShop() || ch->GetMyShop())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("CANT_DO_THIS_BECAUSE_OTHER_WINDOW_OPEN"));
		return;
	}

	if (0 == quest::CQuestManager::instance().GetEventFlag("enable_shop_search"))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PRIVATE_SHOP_SEARCH_SYSTEM_DISABLED"));
		return;
	}

	int32_t Race = pinfo->Race;
	int32_t Cat = pinfo->ItemCat;
	int32_t SubCat = pinfo->SubCat;
	int32_t MinLevel = pinfo->MinLevel;
	int32_t MaxLevel = pinfo->MaxLevel;
	int32_t MinRefine = pinfo->MinRefine;
	int32_t MaxRefine = pinfo->MaxRefine;
	uint64_t MinGold = pinfo->MinGold;
	uint64_t MaxGold = pinfo->MaxGold;
	uint64_t MinCheque = pinfo->MinCheque;
	uint64_t MaxCheque = pinfo->MaxCheque;
	std::string itemName = pinfo->ItemName;

	if (Race < JOB_WARRIOR || Race > JOB_WOLFMAN)
	//if (Race < JOB_WARRIOR || Race > JOB_SHAMAN)
		return;

	switch (Cat)
	{
	case MASK_ITEM_TYPE_NONE:
		return;
		break;
	case MASK_ITEM_TYPE_MOUNT_PET:
		if (SubCat < MASK_ITEM_SUBTYPE_MOUNT_PET_MOUNT || SubCat > MASK_ITEM_SUBTYPE_MOUNT_PET_EGG)
			return;
		break;
	case MASK_ITEM_TYPE_EQUIPMENT_WEAPON:
		if (SubCat < MASK_ITEM_SUBTYPE_WEAPON_WEAPON_SWORD || SubCat > MASK_ITEM_SUBTYPE_WEAPON_WEAPON_QUIVER)
			return;
		break;
	case MASK_ITEM_TYPE_EQUIPMENT_ARMOR:
		if (SubCat < MASK_ITEM_SUBTYPE_ARMOR_ARMOR_BODY || SubCat > MASK_ITEM_SUBTYPE_ARMOR_ARMOR_SHIELD)
			return;
		break;
	case MASK_ITEM_TYPE_EQUIPMENT_JEWELRY:
		if (SubCat < MASK_ITEM_SUBTYPE_JEWELRY_ARMOR_WRIST || SubCat > MASK_ITEM_SUBTYPE_JEWELRY_ITEM_ELEMENT)
			return;
		break;
	case MASK_ITEM_TYPE_TUNING:
		if (SubCat < MASK_ITEM_SUBTYPE_TUNING_RESOURCE || SubCat > MASK_ITEM_SUBTYPE_TUNING_ETC)
			return;
		break;
	case MASK_ITEM_TYPE_POTION:
		if (SubCat < MASK_ITEM_SUBTYPE_POTION_ABILITY || SubCat > MASK_ITEM_SUBTYPE_POTION_ETC)
			return;
		break;
	case MASK_ITEM_TYPE_FISHING_PICK:
		if (SubCat < MASK_ITEM_SUBTYPE_FISHING_PICK_FISHING_POLE || SubCat > MASK_ITEM_SUBTYPE_FISHING_PICK_ETC)
			return;
		break;
	case MASK_ITEM_TYPE_DRAGON_STONE:
		if (SubCat < MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_DIAMOND || SubCat > MASK_ITEM_SUBTYPE_DRAGON_STONE_ETC)
			return;
		break;
	case MASK_ITEM_TYPE_COSTUMES:
		if (SubCat < MASK_ITEM_SUBTYPE_COSTUMES_COSTUME_WEAPON || SubCat > MASK_ITEM_SUBTYPE_COSTUMES_ETC)
			return;
		break;
	case MASK_ITEM_TYPE_SKILL:
		if (SubCat < MASK_ITEM_SUBTYPE_SKILL_PAHAE || SubCat > MASK_ITEM_SUBTYPE_SKILL_ETC)
			return;
		break;
	case MASK_ITEM_TYPE_UNIQUE:
		if (SubCat < MASK_ITEM_SUBTYPE_UNIQUE_ABILITY || SubCat > MASK_ITEM_SUBTYPE_UNIQUE_ETC)
			return;
		break;
	case MASK_ITEM_TYPE_ETC:
		if (SubCat < MASK_ITEM_SUBTYPE_ETC_GIFTBOX || SubCat > MASK_ITEM_SUBTYPE_ETC_ETC)
			return;
		break;
	case MASK_ITEM_TYPE_MAX:
		return;
	default:
		return;
	}

	if (MinLevel < 0 || MinLevel > PLAYER_MAX_LEVEL_CONST)
		return;
	if (MaxLevel < 0 || MaxLevel > PLAYER_MAX_LEVEL_CONST)
		return;
	if (MinRefine < 0 || MinRefine > 9)
		return;
	if (MaxRefine < 0 || MaxRefine > 9)
		return;
	if (MinGold < 0 || MinGold > GOLD_MAX)
		return;
	if (MaxGold < 0 || MaxGold > GOLD_MAX)
		return;
	if (MinCheque < 0 || MinCheque > 99999)
		return;
	if (MaxCheque < 0 || MaxCheque > 99999)
		return;
	if (MinLevel > MaxLevel)
		return;
	if (MinRefine > MaxRefine)
		return;
	if (MinGold > MaxGold)
		return;
	if (MinCheque > MaxCheque)
		return;

	bool bName = false;

	if (itemName.size() > 0)
	{
		bName = true;
	}

	if (bName)
	{
		itemName = pinfo->ItemName;
		std::replace(itemName.begin(), itemName.end(), '_', ' ');
	}

	quest::PC* pPC = quest::CQuestManager::instance().GetPC(ch->GetPlayerID());
	
	if (!pPC)
		return;

	DWORD dwShopSearchSecCycle = 2; // 1 sec
	DWORD dwNowSec = get_global_time();
	DWORD dwLastShopSearchAttrSec = pPC->GetFlag("ShopSearch.LastShopSearchSecAttr");

	if (dwLastShopSearchAttrSec + dwShopSearchSecCycle > dwNowSec)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PRIVATE_SHOP_SEARCH_NEED_WAIT_%d_LEFT_(%d)"),
			dwShopSearchSecCycle, dwShopSearchSecCycle - (dwNowSec - dwLastShopSearchAttrSec));
		return;
	}

	pPC->SetFlag("ShopSearch.LastShopSearchSecAttr", dwNowSec);

	if (bName)
		sys_log(0, "SHOP_SEARCH: CharName: %s Search: Race: %d Cat: %d SubCat: %d MinLevel: %d MaxLevel: %d MinRefine: %d MaxRefine: %d MinGold: %llu MaxGold: %llu itemName: %s", ch->GetName(), Race, Cat, SubCat, MinLevel, MaxLevel, MinRefine, MaxRefine, MinGold, MaxGold, itemName.c_str());
	else
		sys_log(0, "SHOP_SEARCH: CharName: %s Search: Race: %d Cat: %d SubCat: %d MinLevel: %d MaxLevel: %d MinRefine: %d MaxRefine: %d MinGold: %llu MaxGold: %llu", ch->GetName(), Race, Cat, SubCat, MinLevel, MaxLevel, MinRefine, MaxRefine, MinGold, MaxGold);

	std::vector<COfflineShop::OFFLINE_SHOP_ITEM> sendItems;
	std::vector<COfflineShop::OFFLINE_SHOP_ITEM> shop_items;
	std::vector<COfflineShop::OFFLINE_SHOP_ITEM>::iterator item;

	char szQuery[1024];
	if (bName)
	{
		sprintf(szQuery, "select of.*,(case when of.vnum in(50300,70037) then (select concat(convert(sp.szName using latin5),' ',convert(ip.locale_name using latin5)) from skill_proto sp where sp.dwVnum = of.socket0) else convert(ip.locale_name using latin5) end) as item_name from offline_shop_item of inner join item_proto ip on ip.vnum = of.vnum where (case when of.vnum in(50300,70037) then (select concat(convert(sp.szName using latin5),' ',convert(ip.locale_name using latin5)) from skill_proto sp where sp.dwVnum = of.socket0) else convert(ip.locale_name using latin5) end) like CONCAT('%%%%', '%s', '%%%%')", itemName.c_str());
	}
	else
	{
		sprintf(szQuery, "select of.*,(case when of.vnum in(50300,70037) then (select concat(convert(sp.szName using latin5),' ',convert(ip.locale_name using latin5)) from skill_proto sp where sp.dwVnum = of.socket0) else convert(ip.locale_name using latin5) end) as item_name from offline_shop_item of inner join item_proto ip on ip.vnum = of.vnum");
	}

	SQLMsg * pSearchQuery(DBManager::instance().DirectQuery(szQuery));

	// sys_err("veri boyutu: %u", sizeof(pSearchQuery));
	if (pSearchQuery->uiSQLErrno != 0)
	{
		sys_err("Item Search Query Failed, Error code: %ld", pSearchQuery->uiSQLErrno);
		delete pSearchQuery;
		return;
	}

	if (!pSearchQuery->Get()->uiNumRows)
	{
		delete pSearchQuery;
		return;
	}

	int i = 0;

	while (MYSQL_ROW row = mysql_fetch_row(pSearchQuery->Get()->pSQLResult))
	{
		shop_items.resize(i + 1);

		const TItemTable * item_table;

		DWORD vnum = 0;
		str_to_number(vnum, row[4]);

		item_table = ITEM_MANAGER::instance().GetTable(vnum);

		if (!item_table)
		{
			sys_err("OfflineShop: no item table by item vnum #%d", vnum);
			continue;
		}

		COfflineShop::OFFLINE_SHOP_ITEM & item = shop_items[i];

		str_to_number(item.id, row[0]);
		str_to_number(item.owner_id, row[1]);
		str_to_number(item.pos, row[2]);
		str_to_number(item.count, row[3]);
		str_to_number(item.price, row[38]);
		str_to_number(item.price_cheque, row[39]);
		str_to_number(item.vnum, row[4]);
		str_to_number(item.alSockets[0], row[5]);
		str_to_number(item.alSockets[1], row[6]);
		str_to_number(item.alSockets[2], row[7]);
		str_to_number(item.alSockets[3], row[41]);
		str_to_number(item.alSockets[4], row[42]);
		str_to_number(item.alSockets[5], row[43]);
		str_to_number(item.aAttr[0].bType, row[8]);
		str_to_number(item.aAttr[0].sValue, row[9]);
		str_to_number(item.aAttr[1].bType, row[10]);
		str_to_number(item.aAttr[1].sValue, row[11]);
		str_to_number(item.aAttr[2].bType, row[12]);
		str_to_number(item.aAttr[2].sValue, row[13]);
		str_to_number(item.aAttr[3].bType, row[14]);
		str_to_number(item.aAttr[3].sValue, row[15]);
		str_to_number(item.aAttr[4].bType, row[16]);
		str_to_number(item.aAttr[4].sValue, row[17]);
		str_to_number(item.aAttr[5].bType, row[18]);
		str_to_number(item.aAttr[5].sValue, row[19]);
		str_to_number(item.aAttr[6].bType, row[20]);
		str_to_number(item.aAttr[6].sValue, row[21]);
		str_to_number(item.aAttr[7].bType, row[22]);
		str_to_number(item.aAttr[7].sValue, row[23]);
		str_to_number(item.aAttr[8].bType, row[24]);
		str_to_number(item.aAttr[8].sValue, row[25]);
		str_to_number(item.aAttr[9].bType, row[26]);
		str_to_number(item.aAttr[9].sValue, row[27]);
		str_to_number(item.aAttr[10].bType, row[28]);
		str_to_number(item.aAttr[10].sValue, row[29]);
		str_to_number(item.aAttr[11].bType, row[30]);
		str_to_number(item.aAttr[11].sValue, row[31]);
		str_to_number(item.aAttr[12].bType, row[32]);
		str_to_number(item.aAttr[12].sValue, row[33]);
		str_to_number(item.aAttr[13].bType, row[34]);
		str_to_number(item.aAttr[13].sValue, row[35]);
		str_to_number(item.aAttr[14].bType, row[36]);
		str_to_number(item.aAttr[14].sValue, row[37]);
		str_to_number(item.status, row[40]);
		str_to_number(item.evolution, row[44]);
		strlcpy(item.szName, row[46], sizeof(item.szName));

		i++;
	}
	
	delete pSearchQuery;

	for (item = shop_items.begin(); item != shop_items.end(); ++item)
	{
		if (item->status != 0 || item->vnum == 0)
			continue;

		TItemTable* table = ITEM_MANAGER::instance().GetTable(item->vnum);

		if (!table)
			continue;
		
		if (bName)
		{
			if (!(item->refine_level >= MinRefine && item->refine_level <= MaxRefine))
				continue;

			if (MinLevel != 0) {
				for (int x = 0; x < ITEM_LIMIT_MAX_NUM; ++x) {
					if (table->aLimits[x].bType == LIMIT_LEVEL) {
						if (table->aLimits[x].lValue < MinLevel) { continue; }
					}
				}
			}

			if (MaxLevel != gPlayerMaxLevel) {
				for (int x = 0; x < ITEM_LIMIT_MAX_NUM; ++x) {
					if (table->aLimits[x].bType == LIMIT_LEVEL) {
						if (table->aLimits[x].lValue > MaxLevel) { continue; }
					}
				}
			}

			for (int x = 0; x < ITEM_LIMIT_MAX_NUM; ++x) {
				if (table->aLimits[x].bType == LIMIT_REAL_TIME) {
					if (table->aLimits[x].lValue == 0) { continue; }
				}
			}

			if (!(item->price >= MinGold && item->price <= MaxGold))
				continue;

			/*if (cont)
				continue;*/

			sendItems.push_back(*item);
		}
		else 
		{
			if (table->bMaskType == Cat && table->bMaskSubType == SubCat)
			{
			
				if ((table->bType == ITEM_WEAPON || table->bType == ITEM_ARMOR) && !(item->refine_level >= MinRefine && item->refine_level <= MaxRefine))
					continue;

				if (MinLevel != 0) {
					for (int x = 0; x < ITEM_LIMIT_MAX_NUM; ++x) {
						if (table->aLimits[x].bType == LIMIT_LEVEL) {
							if (table->aLimits[x].lValue < MinLevel) { continue; }
						}
					}
				}

				if (MaxLevel != gPlayerMaxLevel) {
					for (int x = 0; x < ITEM_LIMIT_MAX_NUM; ++x) {
						if (table->aLimits[x].bType == LIMIT_LEVEL) {
							if (table->aLimits[x].lValue > MaxLevel) { continue; }
						}
					}
				}

				for (int x = 0; x < ITEM_LIMIT_MAX_NUM; ++x) {
					if (table->aLimits[x].bType == LIMIT_REAL_TIME) {
						if (table->aLimits[x].lValue == 0) { continue; }
					}
				}

				if (!(item->price >= MinGold && item->price <= MaxGold))
					continue;


				bool cont = false;
			
				switch (Race)
				{
				case JOB_WARRIOR:
					if (IS_SET(table->dwAntiFlags, ITEM_ANTIFLAG_WARRIOR))
						cont = true;
					break;

				case JOB_ASSASSIN:
					if (IS_SET(table->dwAntiFlags, ITEM_ANTIFLAG_ASSASSIN))
						cont = true;
					break;

				case JOB_SURA:
					if (IS_SET(table->dwAntiFlags, ITEM_ANTIFLAG_SURA))
						cont = true;
					break;

				case JOB_SHAMAN:
					if (IS_SET(table->dwAntiFlags, ITEM_ANTIFLAG_SHAMAN))
						cont = true;
					break;
				case JOB_WOLFMAN:
					if (IS_SET(table->dwAntiFlags, ITEM_ANTIFLAG_WOLFMAN))
						cont = true;
					break;
				}
				
				if (cont)
					continue;

				sendItems.push_back(*item);
			}

		}
	}


	std::stable_sort(sendItems.begin(), sendItems.end(), CompareItemVnumAcPriceAC);

	int page = 0;

	for (item = sendItems.begin(); item != sendItems.end(); ++item)
	{
		
		
		if (item->status != 0 || item->vnum == 0)
			continue;
		
		
		TItemTable* table = ITEM_MANAGER::instance().GetTable(item->vnum);

		LPCHARACTER offlineShopNpc = CHARACTER_MANAGER::instance().Find(COfflineShopManager::instance().FindMyOfflineShop(item->owner_id));
		
		char szQuery[1024];
		char ownerName[CHARACTER_NAME_MAX_LEN + 1];
		snprintf(szQuery, sizeof(szQuery), "SELECT NAME FROM player%s WHERE id=%d", get_table_postfix(), item->owner_id);
		std::unique_ptr<SQLMsg> pmsg(DBManager::instance().DirectQuery(szQuery));

		if (pmsg->Get()->uiNumRows > 0)
		{
			MYSQL_ROW row = mysql_fetch_row(pmsg->Get()->pSQLResult);
			strlcpy(ownerName, row[0], sizeof(ownerName));
		}
		else
		{
			strlcpy(ownerName, "", sizeof(ownerName));
		}

		if (!offlineShopNpc || !pPC)
		{
			// return;
			continue;
		}

		if (table)
		{	
			LPDESC d = ch->GetDesc();

			if (!d)
				return;
			

			TPacketGCShopSearchItemSet pack;
			pack.header = HEADER_GC_SHOPSEARCH_SET;

			pack.vid = offlineShopNpc->GetVID();
			pack.ownerId = item->owner_id;
			strlcpy(pack.ownerName, ownerName, sizeof(pack.ownerName));
			//strlcpy(pack.ownerName, , sizeof(pack.ownerName));
			strlcpy(pack.itemName, item->szName, sizeof(pack.itemName));
			pack.Cell = item->pos;
			pack.price = item->price;
			pack.price_cheque = item->price_cheque;
			pack.vnum = item->vnum;
			pack.count = static_cast<short>(item->count);
			pack.flags = table->dwFlags;
			pack.anti_flags = table->dwAntiFlags;
			
			pack.evolution = item->evolution;

			pack.page = (page / 10) + 1;

			thecore_memcpy(pack.alSockets, item->alSockets, sizeof(pack.alSockets));
			thecore_memcpy(pack.aAttr, item->aAttr, sizeof(pack.aAttr));

			d->LargePacket(&pack, sizeof(TPacketGCShopSearchItemSet));
		}
		page++;
	}
}

void CInputMain::ShopSearchBuy(LPCHARACTER ch, const char * data)
{
	TPacketCGShopSearchBuy * pinfo = (TPacketCGShopSearchBuy *)data;

	int32_t shopVid = pinfo->shopVid;
	int32_t shopItemPos = pinfo->shopItemPos;

	if (!ch)
		return;

	if (0 == quest::CQuestManager::instance().GetEventFlag("enable_shop_search"))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PRIVATE_SHOP_SEARCH_SYSTEM_DISABLED"));
		return;
	}

	LPCHARACTER pkChrShop = CHARACTER_MANAGER::instance().Find(shopVid);

	if (!pkChrShop)
		return;

	LPOFFLINESHOP pkShop = pkChrShop->GetOfflineShop();

	if (!pkShop)
		return;

	ch->SetOfflineShopOwner(pkChrShop);
	pkShop->SetGuestMap(ch);
	int32_t returnHeader = pkShop->Buy(ch, shopItemPos);

	if (SHOP_SUBHEADER_GC_OK == returnHeader)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "ShopSearchBuy");
		ch->SetOfflineShop(NULL);
		ch->SetOfflineShopOwner(NULL);
		pkShop->RemoveGuestMap(ch);
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "ShopSearchError %d", returnHeader);
		ch->SetOfflineShop(NULL);
		ch->SetOfflineShopOwner(NULL);
		pkShop->RemoveGuestMap(ch);
	}
}
#endif
void SendBlockChatInfo(LPCHARACTER ch, int sec)
{
	if (sec <= 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã¤ÆÃ ±ÝÁö »óÅÂÀÔ´Ï´Ù."));
		return;
	}

	long hour = sec / 3600;
	sec -= hour * 3600;

	long min = (sec / 60);
	sec -= min * 60;

	char buf[128+1];

	if (hour > 0 && min > 0)
		snprintf(buf, sizeof(buf), LC_TEXT("%d ½Ã°£ %d ºÐ %d ÃÊ µ¿¾È Ã¤ÆÃ±ÝÁö »óÅÂÀÔ´Ï´Ù"), hour, min, sec);
	else if (hour > 0 && min == 0)
		snprintf(buf, sizeof(buf), LC_TEXT("%d ½Ã°£ %d ÃÊ µ¿¾È Ã¤ÆÃ±ÝÁö »óÅÂÀÔ´Ï´Ù"), hour, sec);
	else if (hour == 0 && min > 0)
		snprintf(buf, sizeof(buf), LC_TEXT("%d ºÐ %d ÃÊ µ¿¾È Ã¤ÆÃ±ÝÁö »óÅÂÀÔ´Ï´Ù"), min, sec);
	else
		snprintf(buf, sizeof(buf), LC_TEXT("%d ÃÊ µ¿¾È Ã¤ÆÃ±ÝÁö »óÅÂÀÔ´Ï´Ù"), sec);

	ch->ChatPacket(CHAT_TYPE_INFO, buf);
}

EVENTINFO(spam_event_info)
{
	char host[MAX_HOST_LENGTH+1];

	spam_event_info()
	{
		::memset( host, 0, MAX_HOST_LENGTH+1 );
	}
};

typedef boost::unordered_map<std::string, std::pair<unsigned int, LPEVENT> > spam_score_of_ip_t;
spam_score_of_ip_t spam_score_of_ip;

EVENTFUNC(block_chat_by_ip_event)
{
	spam_event_info* info = dynamic_cast<spam_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "block_chat_by_ip_event> <Factor> Null pointer" );
		return 0;
	}

	const char * host = info->host;

	spam_score_of_ip_t::iterator it = spam_score_of_ip.find(host);

	if (it != spam_score_of_ip.end())
	{
		it->second.first = 0;
		it->second.second = NULL;
	}

	return 0;
}

bool SpamBlockCheck(LPCHARACTER ch, const char* const buf, const size_t buflen)
{
	extern int g_iSpamBlockMaxLevel;

	if (ch->GetLevel() < g_iSpamBlockMaxLevel)
	{
		spam_score_of_ip_t::iterator it = spam_score_of_ip.find(ch->GetDesc()->GetHostName());

		if (it == spam_score_of_ip.end())
		{
			spam_score_of_ip.insert(std::make_pair(ch->GetDesc()->GetHostName(), std::make_pair(0, (LPEVENT) NULL)));
			it = spam_score_of_ip.find(ch->GetDesc()->GetHostName());
		}

		if (it->second.second)
		{
			SendBlockChatInfo(ch, event_time(it->second.second) / passes_per_sec);
			return true;
		}

		unsigned int score;
		const char * word = SpamManager::instance().GetSpamScore(buf, buflen, score);

		it->second.first += score;

		if (word)
			sys_log(0, "SPAM_SCORE: %s text: %s score: %u total: %u word: %s", ch->GetName(), buf, score, it->second.first, word);

		extern unsigned int g_uiSpamBlockScore;
		extern unsigned int g_uiSpamBlockDuration;

		if (it->second.first >= g_uiSpamBlockScore)
		{
			spam_event_info* info = AllocEventInfo<spam_event_info>();
			strlcpy(info->host, ch->GetDesc()->GetHostName(), sizeof(info->host));

			it->second.second = event_create(block_chat_by_ip_event, info, PASSES_PER_SEC(g_uiSpamBlockDuration));
			sys_log(0, "SPAM_IP: %s for %u seconds", info->host, g_uiSpamBlockDuration);

			LogManager::instance().CharLog(ch, 0, "SPAM", word);

			SendBlockChatInfo(ch, event_time(it->second.second) / passes_per_sec);

			return true;
		}
	}

	return false;
}

enum
{
	TEXT_TAG_PLAIN,
	TEXT_TAG_TAG, // ||
	TEXT_TAG_COLOR, // |cffffffff
	TEXT_TAG_HYPERLINK_START, // |H
	TEXT_TAG_HYPERLINK_END, // |h ex) |Hitem:1234:1:1:1|h
	TEXT_TAG_RESTORE_COLOR,
};

int GetTextTag(const char * src, int maxLen, int & tagLen, std::string & extraInfo)
{
	tagLen = 1;

	if (maxLen < 2 || *src != '|')
		return TEXT_TAG_PLAIN;

	const char * cur = ++src;

	if (*cur == '|') // ||´Â |·Î Ç¥½ÃÇÑ´Ù.
	{
		tagLen = 2;
		return TEXT_TAG_TAG;
	}
	else if (*cur == 'c') // color |cffffffffblahblah|r
	{
		tagLen = 2;
		return TEXT_TAG_COLOR;
	}
	else if (*cur == 'H') // hyperlink |Hitem:10000:0:0:0:0|h[ÀÌ¸§]|h
	{
		tagLen = 2;
		return TEXT_TAG_HYPERLINK_START;
	}
	else if (*cur == 'h') // end of hyperlink
	{
		tagLen = 2;
		return TEXT_TAG_HYPERLINK_END;
	}

	return TEXT_TAG_PLAIN;
}

void GetTextTagInfo(const char * src, int src_len, int & hyperlinks, bool & colored)
{
	colored = false;
	hyperlinks = 0;

	int len;
	std::string extraInfo;

	for (int i = 0; i < src_len;)
	{
		int tag = GetTextTag(&src[i], src_len - i, len, extraInfo);

		if (tag == TEXT_TAG_HYPERLINK_START)
			++hyperlinks;

		if (tag == TEXT_TAG_COLOR)
			colored = true;

		i += len;
	}
}

int ProcessTextTag(LPCHARACTER ch, const char * c_pszText, size_t len)
{
	return 0;
	//2012.05.17 ±è¿ë¿í
	//0 : Á¤»óÀûÀ¸·Î »ç¿ë
	//1 : ±Ý°­°æ ºÎÁ·
	//2 : ±Ý°­°æÀÌ ÀÖÀ¸³ª, °³ÀÎ»óÁ¡¿¡¼­ »ç¿ëÁß
	//3 : ±³È¯Áß
	//4 : ¿¡·¯
	int hyperlinks;
	bool colored;
	
	GetTextTagInfo(c_pszText, len, hyperlinks, colored);

	if (colored == true && hyperlinks == 0)
		return 4;

	if (ch->GetExchange())
	{
		if (hyperlinks == 0)
			return 0;
		else
			return 3;
	}

	int nPrismCount = ch->CountSpecifyItem(ITEM_PRISM);

	if (nPrismCount < hyperlinks)
		return 0;


	if (!ch->GetMyShop())
	{
		ch->RemoveSpecifyItem(ITEM_PRISM, hyperlinks);
		return 0;
	} else
	{
		int sellingNumber = ch->GetMyShop()->GetNumberByVnum(ITEM_PRISM);
		if(nPrismCount - sellingNumber < hyperlinks)
		{
			return 2;
		} else
		{
			ch->RemoveSpecifyItem(ITEM_PRISM, hyperlinks);
			return 0;
		}
	}
	
	return 4;
}

int CInputMain::Whisper(LPCHARACTER ch, const char * data, size_t uiBytes)
{
	const TPacketCGWhisper* pinfo = reinterpret_cast<const TPacketCGWhisper*>(data);

	if (uiBytes < pinfo->wSize)
		return -1;

	int iExtraLen = pinfo->wSize - sizeof(TPacketCGWhisper);

	if (iExtraLen < 0)
	{
		sys_err("invalid packet length (len %d size %u buffer %u)", iExtraLen, pinfo->wSize, uiBytes);
		ch->GetDesc()->SetPhase(PHASE_CLOSE);
		return -1;
	}
#ifdef WJ_SECURITY_SYSTEM
	if (ch->IsActivateSecurity())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GUVENLIK_AKTIF_HATA"));
		return (iExtraLen);
	}
#endif

	if (ch->FindAffect(AFFECT_BLOCK_CHAT))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã¤ÆÃ ±ÝÁö »óÅÂÀÔ´Ï´Ù."));
		return (iExtraLen);
	}

	LPCHARACTER pkChr = CHARACTER_MANAGER::instance().FindPC(pinfo->szNameTo);

	if (pkChr == ch)
		return (iExtraLen);

	LPDESC pkDesc = NULL;

	BYTE bOpponentEmpire = 0;

	if (test_server)
	{
		if (!pkChr)
			sys_log(0, "Whisper to %s(%s) from %s", "Null", pinfo->szNameTo, ch->GetName());
		else
			sys_log(0, "Whisper to %s(%s) from %s", pkChr->GetName(), pinfo->szNameTo, ch->GetName());
	}
		
	if (ch->IsBlockMode(BLOCK_WHISPER))
	{
		if (ch->GetDesc())
		{
			TPacketGCWhisper pack;
			pack.bHeader = HEADER_GC_WHISPER;
			pack.bType = WHISPER_TYPE_SENDER_BLOCKED;
			pack.wSize = sizeof(TPacketGCWhisper);
			strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
			ch->GetDesc()->Packet(&pack, sizeof(pack));
		}
		return iExtraLen;
	}

	if (!pkChr)
	{
		CCI * pkCCI = P2P_MANAGER::instance().Find(pinfo->szNameTo);

		if (pkCCI)
		{
			pkDesc = pkCCI->pkDesc;
			pkDesc->SetRelay(pinfo->szNameTo);
			bOpponentEmpire = pkCCI->bEmpire;

			if (test_server)
				sys_log(0, "Whisper to %s from %s (Channel %d Mapindex %d)", "Null", ch->GetName(), pkCCI->bChannel, pkCCI->lMapIndex);
		}
	}
	else
	{
		pkDesc = pkChr->GetDesc();
		bOpponentEmpire = pkChr->GetEmpire();
	}

	if (!pkDesc)
	{
		if (ch->GetDesc())
		{
			TPacketGCWhisper pack;

			pack.bHeader = HEADER_GC_WHISPER;
			pack.bType = WHISPER_TYPE_NOT_EXIST;
			pack.wSize = sizeof(TPacketGCWhisper);
			strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
			ch->GetDesc()->Packet(&pack, sizeof(TPacketGCWhisper));
			sys_log(0, "WHISPER: no player");
		}
	}
	else
	{
		if (ch->IsBlockMode(BLOCK_WHISPER))
		{
			if (ch->GetDesc())
			{
				TPacketGCWhisper pack;
				pack.bHeader = HEADER_GC_WHISPER;
				pack.bType = WHISPER_TYPE_SENDER_BLOCKED;
				pack.wSize = sizeof(TPacketGCWhisper);
				strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
		}
		else if (pkChr && pkChr->IsBlockMode(BLOCK_WHISPER))
		{
			if (ch->GetDesc())
			{
				TPacketGCWhisper pack;
				pack.bHeader = HEADER_GC_WHISPER;
				pack.bType = WHISPER_TYPE_TARGET_BLOCKED;
				pack.wSize = sizeof(TPacketGCWhisper);
				strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));
				ch->GetDesc()->Packet(&pack, sizeof(pack));
			}
		}
#if defined(ENABLE_MESSENGER_BLOCK)
		else if (pkChr && MessengerManager::instance().CheckMessengerList(ch->GetName(), pkChr->GetName(), MESSENGER_BLOCK))
		{
			if (ch->GetDesc())
			{
				TPacketGCWhisper pack;

				char msg[CHAT_MAX_LEN + 1];
				snprintf(msg, sizeof(msg), LC_TEXT("Unblock %s to continue."), pkChr->GetName());
				int len = MIN(CHAT_MAX_LEN, strlen(msg) + 1);

				pack.bHeader = HEADER_GC_WHISPER;
				pack.wSize = sizeof(TPacketGCWhisper) + len;
				pack.bType = WHISPER_TYPE_SYSTEM;
				strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));

				TEMP_BUFFER buf;

				buf.write(&pack, sizeof(TPacketGCWhisper));
				buf.write(msg, len);
				ch->GetDesc()->Packet(buf.read_peek(), buf.size());
			}
		}
		else if (pkChr && MessengerManager::instance().CheckMessengerList(pkChr->GetName(), ch->GetName(), MESSENGER_BLOCK))
		{
			if (ch->GetDesc())
			{
				TPacketGCWhisper pack;

				char msg[CHAT_MAX_LEN + 1];
				snprintf(msg, sizeof(msg), LC_TEXT("%s has blocked you."), pkChr->GetName());
				int len = MIN(CHAT_MAX_LEN, strlen(msg) + 1);

				pack.bHeader = HEADER_GC_WHISPER;
				pack.wSize = sizeof(TPacketGCWhisper) + len;
				pack.bType = WHISPER_TYPE_SYSTEM;
				strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));

				TEMP_BUFFER buf;

				buf.write(&pack, sizeof(TPacketGCWhisper));
				buf.write(msg, len);
				ch->GetDesc()->Packet(buf.read_peek(), buf.size());
			}
		}
#endif
		else
		{
			BYTE bType = WHISPER_TYPE_NORMAL;

			char buf[CHAT_MAX_LEN + 1];
			strlcpy(buf, data + sizeof(TPacketCGWhisper), MIN(iExtraLen + 1, sizeof(buf)));
			const size_t buflen = strlen(buf);

			if (true == SpamBlockCheck(ch, buf, buflen))
			{
				if (!pkChr)
				{
					CCI * pkCCI = P2P_MANAGER::instance().Find(pinfo->szNameTo);

					if (pkCCI)
					{
						pkDesc->SetRelay("");
					}
				}
				return iExtraLen;
			}

			if (LC_IsCanada() == false)
			{
				CBanwordManager::instance().ConvertString(buf, buflen);
			}

			if (g_bEmpireWhisper)
				if (!ch->IsEquipUniqueGroup(UNIQUE_GROUP_RING_OF_LANGUAGE))
					if (!(pkChr && pkChr->IsEquipUniqueGroup(UNIQUE_GROUP_RING_OF_LANGUAGE)))
						if (bOpponentEmpire != ch->GetEmpire() && ch->GetEmpire() && bOpponentEmpire // ¼­·Î Á¦±¹ÀÌ ´Ù¸£¸é¼­
								&& ch->GetGMLevel() == GM_PLAYER && gm_get_level(pinfo->szNameTo) == GM_PLAYER) // µÑ´Ù ÀÏ¹Ý ÇÃ·¹ÀÌ¾îÀÌ¸é
							// ÀÌ¸§ ¹Û¿¡ ¸ð¸£´Ï gm_get_level ÇÔ¼ö¸¦ »ç¿ë
						{
							if (!pkChr)
							{
								// ´Ù¸¥ ¼­¹ö¿¡ ÀÖÀ¸´Ï Á¦±¹ Ç¥½Ã¸¸ ÇÑ´Ù. bTypeÀÇ »óÀ§ 4ºñÆ®¸¦ Empire¹øÈ£·Î »ç¿ëÇÑ´Ù.
								bType = ch->GetEmpire() << 4;
							}
							else
							{
								ConvertEmpireText(ch->GetEmpire(), buf, buflen, 10 + 2 * pkChr->GetSkillPower(SKILL_LANGUAGE1 + ch->GetEmpire() - 1)/*º¯È¯È®·ü*/);
							}
						}

			int processReturn = ProcessTextTag(ch, buf, buflen);
			if (0!=processReturn)
			{
				if (ch->GetDesc())
				{
					TItemTable * pTable = ITEM_MANAGER::instance().GetTable(ITEM_PRISM);

					if (pTable)
					{
						char buf[128];
						int len;
						if (3==processReturn) //±³È¯Áß
							len = snprintf(buf, sizeof(buf), LC_TEXT("´Ù¸¥ °Å·¡Áß(Ã¢°í,±³È¯,»óÁ¡)¿¡´Â °³ÀÎ»óÁ¡À» »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."), pTable->szLocaleName);
						else
							len = snprintf(buf, sizeof(buf), LC_TEXT("%sÀÌ ÇÊ¿äÇÕ´Ï´Ù."), pTable->szLocaleName);
						

						if (len < 0 || len >= (int) sizeof(buf))
							len = sizeof(buf) - 1;

						++len;  // \0 ¹®ÀÚ Æ÷ÇÔ

						TPacketGCWhisper pack;

						pack.bHeader = HEADER_GC_WHISPER;
						pack.bType = WHISPER_TYPE_ERROR;
						pack.wSize = sizeof(TPacketGCWhisper) + len;
						strlcpy(pack.szNameFrom, pinfo->szNameTo, sizeof(pack.szNameFrom));

						ch->GetDesc()->BufferedPacket(&pack, sizeof(pack));
						ch->GetDesc()->Packet(buf, len);

						sys_log(0, "WHISPER: not enough %s: char: %s", pTable->szLocaleName, ch->GetName());
					}
				}

				// ¸±·¡ÀÌ »óÅÂÀÏ ¼ö ÀÖÀ¸¹Ç·Î ¸±·¡ÀÌ¸¦ Ç®¾îÁØ´Ù.
				pkDesc->SetRelay("");
				return (iExtraLen);
			}

			if (ch->IsGM())
				bType = (bType & 0xF0) | WHISPER_TYPE_GM;

			if (buflen > 0)
			{
				TPacketGCWhisper pack;

				pack.bHeader = HEADER_GC_WHISPER;
				pack.wSize = sizeof(TPacketGCWhisper) + buflen;
				pack.bType = bType;
				strlcpy(pack.szNameFrom, ch->GetName(), sizeof(pack.szNameFrom));

				// desc->BufferedPacketÀ» ÇÏÁö ¾Ê°í ¹öÆÛ¿¡ ½á¾ßÇÏ´Â ÀÌÀ¯´Â 
				// P2P relayµÇ¾î ÆÐÅ¶ÀÌ Ä¸½¶È­ µÉ ¼ö ÀÖ±â ¶§¹®ÀÌ´Ù.
				TEMP_BUFFER tmpbuf;

				tmpbuf.write(&pack, sizeof(pack));
				tmpbuf.write(buf, buflen);

				pkDesc->Packet(tmpbuf.read_peek(), tmpbuf.size());

				if (LC_IsEurope() != true)
				{
					sys_log(0, "WHISPER: %s -> %s : %s", ch->GetName(), pinfo->szNameTo, buf);
				}
			}
		}
	}
	if(pkDesc)
		pkDesc->SetRelay("");

	return (iExtraLen);
}

struct RawPacketToCharacterFunc
{
	const void * m_buf;
	int	m_buf_len;

	RawPacketToCharacterFunc(const void * buf, int buf_len) : m_buf(buf), m_buf_len(buf_len)
	{
	}

	void operator () (LPCHARACTER c)
	{
		if (!c->GetDesc())
			return;

		c->GetDesc()->Packet(m_buf, m_buf_len);
	}
};

struct FEmpireChatPacket
{
	packet_chat& p;
	const char* orig_msg;
	int orig_len;
	char converted_msg[CHAT_MAX_LEN+1];

	BYTE bEmpire;
	int iMapIndex;
	int namelen;

	FEmpireChatPacket(packet_chat& p, const char* chat_msg, int len, BYTE bEmpire, int iMapIndex, int iNameLen)
		: p(p), orig_msg(chat_msg), orig_len(len), bEmpire(bEmpire), iMapIndex(iMapIndex), namelen(iNameLen)
	{
		memset( converted_msg, 0, sizeof(converted_msg) );
	}

	void operator () (LPDESC d)
	{
		if (!d->GetCharacter())
			return;

		if (d->GetCharacter()->GetMapIndex() != iMapIndex)
			return;

		d->BufferedPacket(&p, sizeof(packet_chat));

		if (d->GetEmpire() == bEmpire ||
			bEmpire == 0 ||
			d->GetCharacter()->GetGMLevel() > GM_PLAYER ||
			d->GetCharacter()->IsEquipUniqueGroup(UNIQUE_GROUP_RING_OF_LANGUAGE))
		{
			d->Packet(orig_msg, orig_len);
		}
		else
		{
			// »ç¶÷¸¶´Ù ½ºÅ³·¹º§ÀÌ ´Ù¸£´Ï ¸Å¹ø ÇØ¾ßÇÕ´Ï´Ù
			size_t len = strlcpy(converted_msg, orig_msg, sizeof(converted_msg));

			if (len >= sizeof(converted_msg))
				len = sizeof(converted_msg) - 1;

			ConvertEmpireText(bEmpire, converted_msg + namelen, len - namelen, 10 + 2 * d->GetCharacter()->GetSkillPower(SKILL_LANGUAGE1 + bEmpire - 1));
			d->Packet(converted_msg, orig_len);
		}
	}
};

struct FYmirChatPacket
{
	packet_chat& packet;
	const char* m_szChat;
	size_t m_lenChat;
	const char* m_szName;
	
	int m_iMapIndex;
	BYTE m_bEmpire;
	bool m_ring;

	char m_orig_msg[CHAT_MAX_LEN+1];
	int m_len_orig_msg;
	char m_conv_msg[CHAT_MAX_LEN+1];
	int m_len_conv_msg;

	FYmirChatPacket(packet_chat& p, const char* chat, size_t len_chat, const char* name, size_t len_name, int iMapIndex, BYTE empire, bool ring)
		: packet(p),
		m_szChat(chat), m_lenChat(len_chat),
		m_szName(name), 
		m_iMapIndex(iMapIndex), m_bEmpire(empire),
		m_ring(ring)
	{
		m_len_orig_msg = snprintf(m_orig_msg, sizeof(m_orig_msg), "%s : %s", m_szName, m_szChat) + 1; // ³Î ¹®ÀÚ Æ÷ÇÔ

		if (m_len_orig_msg < 0 || m_len_orig_msg >= (int) sizeof(m_orig_msg))
			m_len_orig_msg = sizeof(m_orig_msg) - 1;

		m_len_conv_msg = snprintf(m_conv_msg, sizeof(m_conv_msg), "??? : %s", m_szChat) + 1; // ³Î ¹®ÀÚ ¹ÌÆ÷ÇÔ

		if (m_len_conv_msg < 0 || m_len_conv_msg >= (int) sizeof(m_conv_msg))
			m_len_conv_msg = sizeof(m_conv_msg) - 1;

		ConvertEmpireText(m_bEmpire, m_conv_msg + 6, m_len_conv_msg - 6, 10); // 6Àº "??? : "ÀÇ ±æÀÌ
	}

	void operator() (LPDESC d)
	{
		if (!d->GetCharacter())
			return;

		if (d->GetCharacter()->GetMapIndex() != m_iMapIndex)
			return;

		if (m_ring ||
			d->GetEmpire() == m_bEmpire ||
			d->GetCharacter()->GetGMLevel() > GM_PLAYER ||
			d->GetCharacter()->IsEquipUniqueGroup(UNIQUE_GROUP_RING_OF_LANGUAGE))
		{
			packet.size = m_len_orig_msg + sizeof(TPacketGCChat);

			d->BufferedPacket(&packet, sizeof(packet_chat));
			d->Packet(m_orig_msg, m_len_orig_msg);
		}
		else
		{
			packet.size = m_len_conv_msg + sizeof(TPacketGCChat);

			d->BufferedPacket(&packet, sizeof(packet_chat));
			d->Packet(m_conv_msg, m_len_conv_msg);
		}
	}
};

#ifdef ENABLE_NEW_PET_SYSTEM
void CInputMain::BraveRequestPetName(LPCHARACTER ch, const char* c_pData)
{
	if (!ch->GetDesc()) { return; }
	int vid = ch->GetEggVid();
	if (vid == 0) { return; }

	if (quest::CQuestManager::instance().GetEventFlag("pet_kapat") == 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_SYSTEM_DISABLED"));
		return;
	}

	TPacketCGRequestPetName* p = (TPacketCGRequestPetName*)c_pData;

	if (ch->GetGold() < 100000) {
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Pet-Kulucka] 100.000 Yang gerekir"));
		return;
	}

	if (!check_name(p->petname))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Pet-Kulucka] Hatali isim girdiniz"));
		return;
	}

	if (ch->CountSpecifyItem(vid) > 0) {
		DBManager::instance().SendMoneyLog(MONEY_LOG_QUEST, ch->GetPlayerID(), -100000);
#ifdef YANG_LIMIT
		ch->GoldChange(-100000, "BraveRequestPetName");
#else
		ch->PointChange(POINT_GOLD, -100000, true);
#endif
		
		ch->RemoveSpecifyItem(vid, 1);
		LPITEM item = ch->AutoGiveItem(vid + 300, 1);
		if (!item)
			return;
		int tmpslot = number(3, 3);
		int tmpskill[3] = { 0, 0, 0 };
		for (int i = 0; i < 3; ++i)
		{
			if (i > tmpslot - 1)
				tmpskill[i] = -1;
		}

		int pet_type = number(0, 3);
		int totaldur = number(1, 14) * 24 * 60 * 60;
		int tmpdur = time(0) + totaldur;
		int tmpagedur = time(0) - 86400;
		DWORD dwArtis1 = 0;
		DWORD dwArtis2 = 0;
		DWORD dwArtis3 = 0;
		if (pet_type == 0)
		{
			dwArtis1 = number(1, 7);
			dwArtis2 = number(1, 7);
			dwArtis3 = number(1, 7);
		}
		else if (pet_type == 1)
		{
			dwArtis1 = number(1, 2);
			dwArtis2 = number(1, 2);
			dwArtis3 = number(1, 2);
		}
		else if (pet_type == 2)
		{
			dwArtis1 = number(2, 3);
			dwArtis2 = number(2, 3);
			dwArtis3 = number(2, 3);
		}
		else if (pet_type == 3)
		{
			dwArtis1 = number(3, 4);
			dwArtis2 = number(3, 4);
			dwArtis3 = number(3, 4);
		}

		char szQuery1[1024];
		snprintf(szQuery1, sizeof(szQuery1), "INSERT INTO new_petsystem VALUES(%lu,'%s', 1, 0, 0, 0, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)", item->GetID(), p->petname, number(1, 30), number(1, 30), number(1, 30), tmpskill[0], 0, tmpskill[1], 0, tmpskill[2], 0, tmpdur, totaldur, tmpagedur, pet_type, dwArtis1, dwArtis2, dwArtis3);
		std::unique_ptr<SQLMsg> pmsg2(DBManager::instance().DirectQuery(szQuery1));
	}
}
#endif

int CInputMain::Chat(LPCHARACTER ch, const char * data, size_t uiBytes)
{
	const TPacketCGChat* pinfo = reinterpret_cast<const TPacketCGChat*>(data);

	if (uiBytes < pinfo->size)
		return -1;

	const int iExtraLen = pinfo->size - sizeof(TPacketCGChat);

	if (iExtraLen < 0)
	{
		sys_err("invalid packet length (len %d size %u buffer %u)", iExtraLen, pinfo->size, uiBytes);
		ch->GetDesc()->SetPhase(PHASE_CLOSE);
		return -1;
	}

	char buf[CHAT_MAX_LEN - (CHARACTER_NAME_MAX_LEN + 3) + 1];
	strlcpy(buf, data + sizeof(TPacketCGChat), MIN(iExtraLen + 1, sizeof(buf)));
	const size_t buflen = strlen(buf);

	if (buflen > 1 && *buf == '/')
	{
		interpret_command(ch, buf + 1, buflen - 1);
		return iExtraLen;
	}
#ifdef WJ_SECURITY_SYSTEM
	if (ch->IsActivateSecurity())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GUVENLIK_AKTIF_HATA"));
		return (iExtraLen);
	}
#endif
#ifdef ENABLE_CHAT_SPAMLIMIT
	if (ch->IncreaseChatCounter() >= 4)
	{
		if (ch->GetChatCounter() == 10)
			ch->GetDesc()->DelayedDisconnect(0);
		return iExtraLen;
	}
#else
	if (ch->IncreaseChatCounter() >= 10)
	{
		if (ch->GetChatCounter() == 10)
		{
			sys_log(0, "CHAT_HACK: %s", ch->GetName());
			ch->GetDesc()->DelayedDisconnect(5);
		}

		return iExtraLen;
	}
#endif

	
	const CAffect* pAffect = ch->FindAffect(AFFECT_BLOCK_CHAT);

	if (pAffect != NULL)
	{
		SendBlockChatInfo(ch, pAffect->lDuration);
		return iExtraLen;
	}

	if (true == SpamBlockCheck(ch, buf, buflen))
	{
		return iExtraLen;
	}

	// @fixme133 begin
	CBanwordManager::instance().ConvertString(buf, buflen);

	int processReturn = ProcessTextTag(ch, buf, buflen);
	if (0!=processReturn)
	{
		const TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ITEM_PRISM);

		if (NULL != pTable)
		{
			if (3==processReturn) 
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ç¿ëÇÒ¼ö ¾ø½À´Ï´Ù."), pTable->szLocaleName);
			else
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%sÀÌ ÇÊ¿äÇÕ´Ï´Ù."), pTable->szLocaleName);

		}

		return iExtraLen;
	}
	// @fixme133 end

	char chatbuf[CHAT_MAX_LEN + 1];
	int len = snprintf(chatbuf, sizeof(chatbuf), "%s : %s", ch->GetName(), buf);
	if (CHAT_TYPE_SHOUT == pinfo->type)
	{
		LogManager::instance().ShoutLog(g_bChannel, ch->GetEmpire(), chatbuf);
	}

	if (len < 0 || len >= (int) sizeof(chatbuf))
		len = sizeof(chatbuf) - 1;

	if (pinfo->type == CHAT_TYPE_SHOUT)
	{
		// const int SHOUT_LIMIT_LEVEL = 15;

		if (ch->GetLevel() < 15)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¿ÜÄ¡±â´Â ·¹º§ %d ÀÌ»ó¸¸ »ç¿ë °¡´É ÇÕ´Ï´Ù."), 15);
			return (iExtraLen);
		}

		// if (thecore_heart->pulse - (int) ch->GetLastShoutPulse() < passes_per_sec * g_iShoutLimitTime)
		if (thecore_heart->pulse - (int) ch->GetLastShoutPulse() < passes_per_sec * 5)
			return (iExtraLen);

		ch->SetLastShoutPulse(thecore_heart->pulse);

		TPacketGGShout p;

		p.bHeader = HEADER_GG_SHOUT;
		p.bEmpire = ch->GetEmpire();
		strlcpy(p.szText, chatbuf, sizeof(p.szText));

		P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGShout));

		SendShout(chatbuf, ch->GetEmpire());

		return (iExtraLen);
	}

	TPacketGCChat pack_chat;

	pack_chat.header = HEADER_GC_CHAT;
	pack_chat.size = sizeof(TPacketGCChat) + len;
	pack_chat.type = pinfo->type;
	pack_chat.id = ch->GetVID();

	switch (pinfo->type)
	{
		case CHAT_TYPE_TALKING:
			{
				const DESC_MANAGER::DESC_SET & c_ref_set = DESC_MANAGER::instance().GetClientSet();

				if (false)
				{
					std::for_each(c_ref_set.begin(), c_ref_set.end(),
							FYmirChatPacket(pack_chat,
								buf,
								strlen(buf),
								ch->GetName(),
								strlen(ch->GetName()),
								ch->GetMapIndex(),
								ch->GetEmpire(),
								ch->IsEquipUniqueGroup(UNIQUE_GROUP_RING_OF_LANGUAGE)));
				}
				else
				{
					std::for_each(c_ref_set.begin(), c_ref_set.end(),
							FEmpireChatPacket(pack_chat,
								chatbuf,
								len,
								(ch->GetGMLevel() > GM_PLAYER ||
								 ch->IsEquipUniqueGroup(UNIQUE_GROUP_RING_OF_LANGUAGE)) ? 0 : ch->GetEmpire(),
								ch->GetMapIndex(), strlen(ch->GetName())));
#ifdef ENABLE_CHAT_LOGGING
					if (ch->IsGM())
					{
						LogManager::instance().EscapeString(__escape_string, sizeof(__escape_string), chatbuf, len);
						LogManager::instance().ChatLog(ch->GetMapIndex(), ch->GetPlayerID(), ch->GetName(), 0, "", "NORMAL", __escape_string, ch->GetDesc() ? ch->GetDesc()->GetHostName() : "");
					}
#endif
				}
			}
			break;

		case CHAT_TYPE_PARTY:
			{
				if (!ch->GetParty())
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÆÄÆ¼ ÁßÀÌ ¾Æ´Õ´Ï´Ù."));
				else
				{
					TEMP_BUFFER tbuf;

					tbuf.write(&pack_chat, sizeof(pack_chat));
					tbuf.write(chatbuf, len);

					RawPacketToCharacterFunc f(tbuf.read_peek(), tbuf.size());
					ch->GetParty()->ForEachOnlineMember(f);
#ifdef ENABLE_CHAT_LOGGING
					if (ch->IsGM())
					{
						LogManager::instance().EscapeString(__escape_string, sizeof(__escape_string), chatbuf, len);
						LogManager::instance().ChatLog(ch->GetMapIndex(), ch->GetPlayerID(), ch->GetName(), ch->GetParty()->GetLeaderPID(), "", "PARTY", __escape_string, ch->GetDesc() ? ch->GetDesc()->GetHostName() : "");
					}
#endif
				}
			}
			break;

		case CHAT_TYPE_GUILD:
			{
				if (!ch->GetGuild())
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±æµå¿¡ °¡ÀÔÇÏÁö ¾Ê¾Ò½À´Ï´Ù."));
				else
				{
					ch->GetGuild()->Chat(chatbuf);
#ifdef ENABLE_CHAT_LOGGING
					if (ch->IsGM())
					{
						LogManager::instance().EscapeString(__escape_string, sizeof(__escape_string), chatbuf, len);
						LogManager::instance().ChatLog(ch->GetMapIndex(), ch->GetPlayerID(), ch->GetName(), ch->GetGuild()->GetID(), ch->GetGuild()->GetName(), "GUILD", __escape_string, ch->GetDesc() ? ch->GetDesc()->GetHostName() : "");
					}
#endif
				}
			}
			break;

		default:
			sys_err("Unknown chat type %d", pinfo->type);
			break;
	}

	return (iExtraLen);
}

void CInputMain::ItemUse(LPCHARACTER ch, const char * data)
{
	if (!ch || !ch->GetDesc()) return;

	TPacketCGItemUse * p = (TPacketCGItemUse *) data;
	ch->UseItem(p->Cell, NPOS, p->useall);
}

void CInputMain::ItemToItem(LPCHARACTER ch, const char * pcData)
{
	TPacketCGItemUseToItem * p = (TPacketCGItemUseToItem *) pcData;
	if (ch)
		ch->UseItem(p->Cell, p->TargetCell);
}

void CInputMain::ItemDrop(LPCHARACTER ch, const char * data)
{
	struct command_item_drop * pinfo = (struct command_item_drop *) data;

	//MONARCH_LIMIT
	//if (ch->IsMonarch())	
	//	return;
	//END_MONARCH_LIMIT
	if (!ch)
		return;

	// ¿¤Å©°¡ 0º¸´Ù Å©¸é ¿¤Å©¸¦ ¹ö¸®´Â °Í ÀÌ´Ù.
	if (pinfo->gold > 0)
		ch->DropGold(pinfo->gold);
#ifdef ENABLE_CHEQUE_SYSTEM
	else if (pinfo->cheque > 0)
		ch->DropCheque(pinfo->cheque);
#endif
	else
		ch->DropItem(pinfo->Cell);
}

void CInputMain::ItemDrop2(LPCHARACTER ch, const char * data)
{
	//MONARCH_LIMIT
	//if (ch->IsMonarch())	
	//	return;
	//END_MONARCH_LIMIT

	TPacketCGItemDrop2 * pinfo = (TPacketCGItemDrop2 *) data;

	// ¿¤Å©°¡ 0º¸´Ù Å©¸é ¿¤Å©¸¦ ¹ö¸®´Â °Í ÀÌ´Ù.
	
	if (!ch)
		return;
	if (pinfo->gold > 0)
		ch->DropGold(pinfo->gold);
#ifdef ENABLE_CHEQUE_SYSTEM
	else if (pinfo->cheque > 0)
		ch->DropCheque(pinfo->cheque);
#endif
	else
		ch->DropItem(pinfo->Cell, pinfo->count);
}

void CInputMain::ItemDestroy(LPCHARACTER ch, const char * data)
{
    struct command_item_destroy * pinfo = (struct command_item_destroy *) data;
    if (ch)
        ch->DestroyItem(pinfo->Cell);
}
void CInputMain::ItemSell(LPCHARACTER ch, const char * data)
{
	struct command_item_sell * pinfo = (struct command_item_sell *) data;
	if (ch)
		ch->SellItem(pinfo->Cell, pinfo->count);
}
void CInputMain::ItemMove(LPCHARACTER ch, const char * data)
{
	struct command_item_move * pinfo = (struct command_item_move *) data;

	if (ch)
		ch->MoveItem(pinfo->Cell, pinfo->CellTo, pinfo->count);
}

void CInputMain::ItemPickup(LPCHARACTER ch, const char * data)
{
	struct command_item_pickup * pinfo = (struct command_item_pickup*) data;
	if (ch)
		ch->PickupItem(pinfo->vid);
}

void CInputMain::QuickslotAdd(LPCHARACTER ch, const char * data)
{
	struct command_quickslot_add * pinfo = (struct command_quickslot_add *) data;
	ch->SetQuickslot(pinfo->pos, pinfo->slot);
}

void CInputMain::QuickslotDelete(LPCHARACTER ch, const char * data)
{
	struct command_quickslot_del * pinfo = (struct command_quickslot_del *) data;
	ch->DelQuickslot(pinfo->pos);
}

void CInputMain::QuickslotSwap(LPCHARACTER ch, const char * data)
{
	struct command_quickslot_swap * pinfo = (struct command_quickslot_swap *) data;
	ch->SwapQuickslot(pinfo->pos, pinfo->change_pos);
}

int CInputMain::Messenger(LPCHARACTER ch, const char* c_pData, size_t uiBytes)
{
	TPacketCGMessenger* p = (TPacketCGMessenger*) c_pData;
	
	if (uiBytes < sizeof(TPacketCGMessenger))
		return -1;

	c_pData += sizeof(TPacketCGMessenger);
	uiBytes -= sizeof(TPacketCGMessenger);

	switch (p->subheader)
	{
		case MESSENGER_SUBHEADER_CG_ADD_BY_VID:
			{
				if (uiBytes < sizeof(TPacketCGMessengerAddByVID))
					return -1;

				TPacketCGMessengerAddByVID * p2 = (TPacketCGMessengerAddByVID *) c_pData;
				LPCHARACTER ch_companion = CHARACTER_MANAGER::instance().Find(p2->vid);

				if (!ch_companion)
					return sizeof(TPacketCGMessengerAddByVID);

				if (ch->IsObserverMode())
					return sizeof(TPacketCGMessengerAddByVID);

				if (ch_companion->IsBlockMode(BLOCK_MESSENGER_INVITE))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÌ ¸Þ½ÅÁ® Ãß°¡ °ÅºÎ »óÅÂÀÔ´Ï´Ù."));
					return sizeof(TPacketCGMessengerAddByVID);
				}

				LPDESC d = ch_companion->GetDesc();

				if (!d)
					return sizeof(TPacketCGMessengerAddByVID);

				if (ch->GetGMLevel() == GM_PLAYER && ch_companion->GetGMLevel() != GM_PLAYER)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<¸Þ½ÅÁ®> ¿î¿µÀÚ´Â ¸Þ½ÅÁ®¿¡ Ãß°¡ÇÒ ¼ö ¾ø½À´Ï´Ù."));
					return sizeof(TPacketCGMessengerAddByVID);
				}

#if defined(ENABLE_MESSENGER_BLOCK)
				if (MessengerManager::instance().CheckMessengerList(ch->GetName(), ch_companion->GetName(), MESSENGER_BLOCK))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Unblock %s to continue."), ch_companion->GetName());
					return sizeof(TPacketCGMessengerAddByVID);
				}
				else if (MessengerManager::instance().CheckMessengerList(ch_companion->GetName(), ch->GetName(), MESSENGER_BLOCK))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s has blocked you."), ch_companion->GetName());
					return sizeof(TPacketCGMessengerAddByVID);
				}
#endif

				if (ch->GetDesc() == d) // ÀÚ½ÅÀº Ãß°¡ÇÒ ¼ö ¾ø´Ù.
					return sizeof(TPacketCGMessengerAddByVID);

				MessengerManager::instance().RequestToAdd(ch, ch_companion);
				//MessengerManager::instance().AddToList(ch->GetName(), ch_companion->GetName());
			}
			return sizeof(TPacketCGMessengerAddByVID);

		case MESSENGER_SUBHEADER_CG_ADD_BY_NAME:
			{
				if (uiBytes < CHARACTER_NAME_MAX_LEN)
					return -1;

				char name[CHARACTER_NAME_MAX_LEN + 1];
				strlcpy(name, c_pData, sizeof(name));

				if (ch->GetGMLevel() == GM_PLAYER && gm_get_level(name) != GM_PLAYER)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<¸Þ½ÅÁ®> ¿î¿µÀÚ´Â ¸Þ½ÅÁ®¿¡ Ãß°¡ÇÒ ¼ö ¾ø½À´Ï´Ù."));
					return CHARACTER_NAME_MAX_LEN;
				}

				LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(name);

				if (!tch)
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ´ÔÀº Á¢¼ÓµÇ ÀÖÁö ¾Ê½À´Ï´Ù."), name);
				else
				{
					if (tch == ch) // ÀÚ½ÅÀº Ãß°¡ÇÒ ¼ö ¾ø´Ù.
						return CHARACTER_NAME_MAX_LEN;

					if (tch->IsBlockMode(BLOCK_MESSENGER_INVITE) == true)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÌ ¸Þ½ÅÁ® Ãß°¡ °ÅºÎ »óÅÂÀÔ´Ï´Ù."));
					}
					else
					{
#if defined(ENABLE_MESSENGER_BLOCK)
						if (MessengerManager::instance().CheckMessengerList(ch->GetName(), tch->GetName(), MESSENGER_BLOCK))
						{
							ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Unblock %s to continue."), tch->GetName());
							return CHARACTER_NAME_MAX_LEN;
						}
						else if (MessengerManager::instance().CheckMessengerList(tch->GetName(), ch->GetName(), MESSENGER_BLOCK))
						{
							ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s has blocked you."), tch->GetName());
							return CHARACTER_NAME_MAX_LEN;
						}
#endif
						// ¸Þ½ÅÀú°¡ Ä³¸¯ÅÍ´ÜÀ§°¡ µÇ¸é¼­ º¯°æ
						MessengerManager::instance().RequestToAdd(ch, tch);
						//MessengerManager::instance().AddToList(ch->GetName(), tch->GetName());
					}
				}
			}
			return CHARACTER_NAME_MAX_LEN;

		case MESSENGER_SUBHEADER_CG_REMOVE:
			{
				if (uiBytes < CHARACTER_NAME_MAX_LEN)
					return -1;

				char char_name[CHARACTER_NAME_MAX_LEN + 1];
				strlcpy(char_name, c_pData, sizeof(char_name));

#if defined(ENABLE_MESSENGER_BLOCK)
				if (!MessengerManager::instance().CheckMessengerList(ch->GetName(), char_name, MESSENGER_FRIEND))
					return CHARACTER_NAME_MAX_LEN;
#endif

				MessengerManager::instance().RemoveFromList(ch->GetName(), char_name);
			}
			return CHARACTER_NAME_MAX_LEN;

#if defined(ENABLE_MESSENGER_BLOCK)
		case MESSENGER_SUBHEADER_CG_ADD_BLOCK_BY_VID:
		{
			if (uiBytes < sizeof(TPacketCGMessengerAddBlockByVID))
				return -1;

			auto p2 = (TPacketCGMessengerAddBlockByVID*) c_pData;
			LPCHARACTER ch_companion = CHARACTER_MANAGER::instance().Find(p2->vid);

			if (!ch_companion)
				return sizeof(TPacketCGMessengerAddBlockByVID);

			if (ch->IsObserverMode())
				return sizeof(TPacketCGMessengerAddBlockByVID);

			LPDESC d = ch_companion->GetDesc();

			if (!d)
				return sizeof(TPacketCGMessengerAddByVID);

			LPCHARACTER partner = ch->GetMarryPartner();
			if (partner)
			{
				if (ch_companion->GetName() == partner->GetName())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot block your spouse."));
					return sizeof(TPacketCGMessengerAddBlockByVID);
				}
			}

			if (ch_companion->GetGuild() == ch->GetGuild() && ch->GetGuild() != NULL && ch_companion->GetGuild() != NULL)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't block your guildmate."));
				return sizeof(TPacketCGMessengerAddBlockByVID);
			}

			if (MessengerManager::instance().CheckMessengerList(ch->GetName(), ch_companion->GetName(), MESSENGER_FRIEND))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Remove %s from your friends list to continue."), ch_companion->GetName());
				return sizeof(TPacketCGMessengerAddBlockByVID);
			}

			if (MessengerManager::instance().CheckMessengerList(ch->GetName(), ch_companion->GetName(), MESSENGER_BLOCK))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s is already being blocked."), ch_companion->GetName());
				return sizeof(TPacketCGMessengerAddBlockByVID);
			}

			if (ch->GetGMLevel() == GM_PLAYER && ch_companion->GetGMLevel() != GM_PLAYER && !test_server)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot block this player."));
				return sizeof(TPacketCGMessengerAddByVID);
			}

			if (ch->GetDesc() == d)
				return sizeof(TPacketCGMessengerAddBlockByVID);

			MessengerManager::instance().AddToBlockList(ch->GetName(), ch_companion->GetName());
		}
		return sizeof(TPacketCGMessengerAddBlockByVID);

		case MESSENGER_SUBHEADER_CG_ADD_BLOCK_BY_NAME:
		{
			if (uiBytes < CHARACTER_NAME_MAX_LEN)
				return -1;

			char name[CHARACTER_NAME_MAX_LEN + 1];
			strlcpy(name, c_pData, sizeof(name));

			if (gm_get_level(name) != GM_PLAYER && !test_server)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot block this player."));
				return CHARACTER_NAME_MAX_LEN;
			}

			LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(name);

			if (!tch)
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("THE_PLAYER_IS_NOT_ACTIVE"), name);
			else
			{
				if (tch == ch)
					return CHARACTER_NAME_MAX_LEN;

				LPCHARACTER partner = ch->GetMarryPartner();
				if (partner)
				{
					if (tch->GetName() == partner->GetName())
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot block your spouse."));
						return CHARACTER_NAME_MAX_LEN;
					}
				}

				if (tch->GetGuild() == ch->GetGuild() && ch->GetGuild() != NULL && tch->GetGuild() != NULL)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't block your guildmate."));
					return CHARACTER_NAME_MAX_LEN;
				}

				if (MessengerManager::instance().CheckMessengerList(ch->GetName(), tch->GetName(), MESSENGER_FRIEND))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Remove %s from your friends list to continue."), tch->GetName());
					return CHARACTER_NAME_MAX_LEN;
				}

				if (MessengerManager::instance().CheckMessengerList(ch->GetName(), tch->GetName(), MESSENGER_BLOCK))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s is already being blocked."), tch->GetName());
					return CHARACTER_NAME_MAX_LEN;
				}

				MessengerManager::instance().AddToBlockList(ch->GetName(), tch->GetName());
			}
		}
		return CHARACTER_NAME_MAX_LEN;

		case MESSENGER_SUBHEADER_CG_REMOVE_BLOCK:
		{
			if (uiBytes < CHARACTER_NAME_MAX_LEN)
				return -1;

			char char_name[CHARACTER_NAME_MAX_LEN + 1];
			strlcpy(char_name, c_pData, sizeof(char_name));

			if (!MessengerManager::instance().CheckMessengerList(ch->GetName(), char_name, MESSENGER_BLOCK))
				return CHARACTER_NAME_MAX_LEN;

			MessengerManager::instance().RemoveFromBlockList(ch->GetName(), char_name);
		}
		return CHARACTER_NAME_MAX_LEN;
#endif

		default:
			sys_err("CInputMain::Messenger : Unknown subheader %d : %s", p->subheader, ch->GetName());
			break;
	}

	return 0;
}

int CInputMain::Shop(LPCHARACTER ch, const char * data, size_t uiBytes)
{
	TPacketCGShop * p = (TPacketCGShop *) data;

	if (uiBytes < sizeof(TPacketCGShop))
		return -1;

	if (test_server)
		sys_log(0, "CInputMain::Shop() ==> SubHeader %d", p->subheader);

	const char * c_pData = data + sizeof(TPacketCGShop);
	uiBytes -= sizeof(TPacketCGShop);

	switch (p->subheader)
	{
		case SHOP_SUBHEADER_CG_END:
			sys_log(1, "INPUT: %s SHOP: END", ch->GetName());
			CShopManager::instance().StopShopping(ch);
			return 0;

		case SHOP_SUBHEADER_CG_BUY:
			{
				if (uiBytes < sizeof(BYTE) + sizeof(BYTE))
					return -1;

				BYTE bPos = *(c_pData + 1);
				sys_log(1, "INPUT: %s SHOP: BUY %d", ch->GetName(), bPos);
				CShopManager::instance().Buy(ch, bPos);
				return (sizeof(BYTE) + sizeof(BYTE));
			}

		case SHOP_SUBHEADER_CG_SELL:
			{
				if (uiBytes < sizeof(BYTE))
					return -1;

				BYTE pos = *c_pData;

				sys_log(0, "INPUT: %s SHOP: SELL", ch->GetName());
				CShopManager::instance().Sell(ch, pos);
				return sizeof(BYTE);
			}

#ifdef ENABLE_SPECIAL_STORAGE
		case SHOP_SUBHEADER_CG_SELL2:
			{
				if (uiBytes < sizeof(TPacketGCShopDetail))
					return -1;

				TPacketGCShopDetail* packet = (TPacketGCShopDetail*)c_pData;
				
				sys_log(0, "INPUT: %s SHOP: SELL2 pos : %d count : %d type : %d", ch->GetName(), packet->itemPos.cell, packet->byCount, packet->itemPos.window_type);
				CShopManager::instance().Sell(ch, packet->itemPos.cell, packet->byCount, packet->itemPos.window_type);

				return sizeof(TPacketGCShopDetail);
			}
#else
		case SHOP_SUBHEADER_CG_SELL2:
			{
				if (uiBytes < sizeof(BYTE) + sizeof(BYTE))
					return -1;

				BYTE pos = *(c_pData++);
				BYTE count = *(c_pData);

				sys_log(0, "INPUT: %s SHOP: SELL2", ch->GetName());
				CShopManager::instance().Sell(ch, pos, count);
				return sizeof(BYTE) + sizeof(BYTE);
			}
#endif

		default:
			sys_err("CInputMain::Shop : Unknown subheader %d : %s", p->subheader, ch->GetName());
			break;
	}

	return 0;
}

int CInputMain::OfflineShop(LPCHARACTER ch, const char * data, size_t uiBytes)
{
	TPacketCGShop * p = (TPacketCGShop *)data;

	if (uiBytes < sizeof(TPacketCGShop))
		return -1;

	if (test_server)
		sys_log(0, "CInputMain::OfflineShop ==> SubHeader %d", p->subheader);

	const char * c_pData = data + sizeof(TPacketCGShop);
	uiBytes -= sizeof(TPacketCGShop);
#ifdef WJ_SECURITY_SYSTEM
	if (ch->IsActivateSecurity() == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GUVENLIK_AKTIF_HATA"));
		return 0;
	}
#endif

	switch (p->subheader)
	{
		case SHOP_SUBHEADER_CG_END:
			sys_log(1, "INPUT: %s OFFLINE_SHOP: END", ch->GetName());
			COfflineShopManager::instance().StopShopping(ch);
			return 0;
		case SHOP_SUBHEADER_CG_BUY:
		{
			if (uiBytes < sizeof(BYTE) + sizeof(BYTE))
				return -1;

			BYTE bPos = *(c_pData + 1);
			sys_log(1, "INPUT: %s OFFLINE_SHOP: BUY %d", ch->GetName(), bPos);
			COfflineShopManager::instance().Buy(ch, bPos);
			return (sizeof(BYTE) + sizeof(BYTE));
		}
		case SHOP_SUBHEADER_CG_CHANGE_EDIT_TIME:
		{
			if (uiBytes < sizeof(BYTE))
				return -1;

			BYTE bTime = *c_pData;
			sys_log(0, "INPUT: %s EDIT_OFFLINE_SHOP_TIME", ch->GetName());
			COfflineShopManager::instance().ChangeOfflineShopTime(ch, bTime);
			return sizeof(BYTE);
		}
		case SHOP_SUBHEADER_CG_DESTROY_OFFLINE_SHOP:
			sys_log(1, "INPUT: %s OFFLINE_SHOP_DESTROY", ch->GetName());
			COfflineShopManager::instance().DestroyOfflineShop(ch, ch->GetOfflineShopVID(), true);
			return 0;
		case SHOP_SUBHEADER_CG_ADD_ITEM:
		{
			if (uiBytes < sizeof(TOfflineShopItemTable))
				return -1;
			
			
			if (ch->IsHack())
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("islemlerden sonra 10 saniye beklemelisin."));
				return (sizeof(TOfflineShopItemTable));
			}

			TOfflineShopItemTable * pTable = (TOfflineShopItemTable *)(c_pData);
			COfflineShopManager::instance().AddItem(ch, pTable->tDisplayPos, pTable->bPos, pTable->lPrice, pTable->lPriceCheque);
			sys_log(0, "INPUT: %s OFFLINE SHOP ADD_ITEM - Display Pos : %d - SrcPos : %d - Yang : %d - Won : %d ", ch->GetName(), pTable->tDisplayPos.cell, pTable->bPos,pTable->lPrice, pTable->lPriceCheque);

			return (sizeof(TOfflineShopItemTable));
		}
		case SHOP_SUBHEADER_CG_REMOVE_ITEM:
		{
			if (uiBytes < sizeof(BYTE))
				return -1;

			BYTE bPos = *c_pData;
			sys_log(0, "INPUT: %s REMOVE_ITEM : %d", ch->GetName(), bPos);
			COfflineShopManager::instance().RemoveItem(ch, bPos);
			return (sizeof(BYTE));
		}
		case SHOP_SUBHEADER_CG_CHANGE_PRICE:
		{
			if (uiBytes < sizeof(TOfflineShopItemTable))
				return -1;

			TOfflineShopItemTable * pTable = (TOfflineShopItemTable *)(c_pData);
			sys_log(0, "INPUT: %s OFFLINE_CHANGE_PRICE (%d) (%d) (%lld)", ch->GetName(), pTable->tDisplayPos, pTable->bPos, pTable->lPrice);
			COfflineShopManager::instance().ChangePrice(ch, pTable->bPos, pTable->lPrice, pTable->lPriceCheque);
			return (sizeof(TOfflineShopItemTable));
		}
		case SHOP_SUBHEADER_CG_REFRESH:
			sys_log(0, "INPUT: %s OFFLINE_SHOP_REFRESH_ITEM", ch->GetName());
			COfflineShopManager::instance().Refresh(ch);
			return 0;
		case SHOP_SUBHEADER_CG_REFRESH_MONEY:
		{
			sys_log(0, "INPUT: %s OFFLINE_SHOP_REFRESH_MONEY", ch->GetName());
			COfflineShopManager::instance().RefreshMoney(ch);
			return 0;
		}
		case SHOP_SUBHEADER_CG_WITHDRAW_MONEY:
		{
			if (uiBytes < sizeof(DWORD))
				return -1;

			const int gold = *reinterpret_cast<const int*>(c_pData);
			sys_log(0, "INPUT: %s(%u) OFFLINE_SHOP_WITHDRAW_MONEY", ch->GetName(), gold);
			COfflineShopManager::instance().WithdrawMoney(ch, gold);
			return (sizeof(DWORD));
		}
		case SHOP_SUBHEADER_CG_WITHDRAW_CHEQUE:
		{
			if (uiBytes < sizeof(DWORD))
				return -1;

			const int cheque = *reinterpret_cast<const int*>(c_pData);
			COfflineShopManager::instance().WithdrawCheque(ch, cheque);
			sys_log(0, "INPUT: %s(%u) OFFLINE_SHOP_WITHDRAW_CHEQUE", ch->GetName(), cheque);
			return (sizeof(DWORD));
		}
		default:
			sys_err("CInputMain::OfflineShop : Unknown subheader %d : %s", p->subheader, ch->GetName());
			break;
	}

	return 0;
}
void CInputMain::OnClick(LPCHARACTER ch, const char * data)
{
	struct command_on_click *	pinfo = (struct command_on_click *) data;
	LPCHARACTER			victim;
#ifdef WJ_SECURITY_SYSTEM
	if (ch->IsActivateSecurity() == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GUVENLIK_AKTIF_HATA"));
		return;
	}
#endif

	if ((victim = CHARACTER_MANAGER::instance().Find(pinfo->vid)))
		victim->OnClick(ch);
	else if (test_server)
	{
		sys_err("CInputMain::OnClick %s.Click.NOT_EXIST_VID[%d]", ch->GetName(), pinfo->vid);
	}
}

#ifdef ENABLE_AURA_SYSTEM
void CInputMain::Aura(LPCHARACTER pkChar, const char* c_pData)
{
	quest::PC* pPC = quest::CQuestManager::instance().GetPCForce(pkChar->GetPlayerID());
	if (pPC->IsRunning())
		return;
	
	/*if (!features::IsFeatureEnabled(features::AURA))
	{
		pkChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("AURA_SYSTEM_DISABLED"));
		return;		
	}*/

	TPacketAura* sPacket = (TPacketAura*)c_pData;
	switch (sPacket->subheader)
	{
	case AURA_SUBHEADER_CG_CLOSE:
	{
		pkChar->CloseAura();
	}
	break;
	case AURA_SUBHEADER_CG_ADD:
	{
		pkChar->AddAuraMaterial(sPacket->tPos, sPacket->bPos);
	}
	break;
	case AURA_SUBHEADER_CG_REMOVE:
	{
		pkChar->RemoveAuraMaterial(sPacket->bPos);
	}
	break;
	case AURA_SUBHEADER_CG_REFINE:
	{
		pkChar->RefineAuraMaterials();
	}
	break;
	default:
		break;
	}
}
#endif

void CInputMain::Exchange(LPCHARACTER ch, const char * data)
{
	struct command_exchange * pinfo = (struct command_exchange *) data;
	LPCHARACTER	to_ch = NULL;

	if (!ch->CanHandleItem())
		return;

	int iPulse = thecore_pulse(); 
	
	if ((to_ch = CHARACTER_MANAGER::instance().Find(pinfo->arg1)))
	{
		if (iPulse - to_ch->GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
		{
			to_ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°Å·¡ ÈÄ %dÃÊ ÀÌ³»¿¡ Ã¢°í¸¦ ¿­¼ö ¾ø½À´Ï´Ù."), g_nPortalLimitTime);
			return;
		}

		if( true == to_ch->IsDead() )
		{
			return;
		}
	}

	sys_log(0, "CInputMain()::Exchange()  SubHeader %d ", pinfo->sub_header);

	if (iPulse - ch->GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°Å·¡ ÈÄ %dÃÊ ÀÌ³»¿¡ Ã¢°í¸¦ ¿­¼ö ¾ø½À´Ï´Ù."), g_nPortalLimitTime);
		return;
	}


	switch (pinfo->sub_header)
	{
		case EXCHANGE_SUBHEADER_CG_START:	// arg1 == vid of target character
			if (!ch->GetExchange())
			{
				if ((to_ch = CHARACTER_MANAGER::instance().Find(pinfo->arg1)))
				{
					//MONARCH_LIMIT
					/*
					if (to_ch->IsMonarch() || ch->IsMonarch())
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±ºÁÖ¿Í´Â °Å·¡¸¦ ÇÒ¼ö°¡ ¾ø½À´Ï´Ù"), g_nPortalLimitTime);
						return;
					}
					//END_MONARCH_LIMIT
					*/
					if (iPulse - ch->GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã¢°í¸¦ ¿¬ÈÄ %dÃÊ ÀÌ³»¿¡´Â °Å·¡¸¦ ÇÒ¼ö ¾ø½À´Ï´Ù."), g_nPortalLimitTime);

						if (test_server)
							ch->ChatPacket(CHAT_TYPE_INFO, "[TestOnly][Safebox]Pulse %d LoadTime %d PASS %d", iPulse, ch->GetSafeboxLoadTime(), PASSES_PER_SEC(g_nPortalLimitTime));
						return; 
					}

					if (iPulse - to_ch->GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
					{
						to_ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã¢°í¸¦ ¿¬ÈÄ %dÃÊ ÀÌ³»¿¡´Â °Å·¡¸¦ ÇÒ¼ö ¾ø½À´Ï´Ù."), g_nPortalLimitTime);


						if (test_server)
							to_ch->ChatPacket(CHAT_TYPE_INFO, "[TestOnly][Safebox]Pulse %d LoadTime %d PASS %d", iPulse, to_ch->GetSafeboxLoadTime(), PASSES_PER_SEC(g_nPortalLimitTime));
						return; 
					}

#ifdef YANG_LIMIT
					if (ch->GetGold() >= ch->GetAllowedGold())
#else
					if (ch->GetGold() >= GOLD_MAX)
#endif
					{	
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾×¼ö°¡ 20¾ï ³ÉÀ» ÃÊ°úÇÏ¿© °Å·¡¸¦ ÇÒ¼ö°¡ ¾ø½À´Ï´Ù.."));

						sys_err("[OVERFLOG_GOLD] START (%u) id %u name %s ", ch->GetGold(), ch->GetPlayerID(), ch->GetName());
						return;
					}
#ifdef ENABLE_CHEQUE_SYSTEM
					if (ch->GetCheque() > CHEQUE_MAX)
					{	
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("??? 20? ?? ???? ??? ??? ????.."));

						sys_err("[OVERFLOW_CHEQUE] START (%lld) id %u name %s ", ch->GetCheque(), ch->GetPlayerID(), ch->GetName());
						//ch->SetGold(CHEQUE_MAX);
						return;
					}
#endif

					if (to_ch->IsPC())
					{
						if (quest::CQuestManager::instance().GiveItemToPC(ch->GetPlayerID(), to_ch))
						{
							sys_log(0, "Exchange canceled by quest %s %s", ch->GetName(), to_ch->GetName());
							return;
						}
					}

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
					if (ch->GetMyShop() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡ÁßÀÏ°æ¿ì °³ÀÎ»óÁ¡À» ¿­¼ö°¡ ¾ø½À´Ï´Ù."));
						return;
					}
#else
					if (ch->GetMyShop() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->IsCubeOpen())
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡ÁßÀÏ°æ¿ì °³ÀÎ»óÁ¡À» ¿­¼ö°¡ ¾ø½À´Ï´Ù."));
						return;
					}
#endif

					ch->ExchangeStart(to_ch);
				}
			}
			break;

		case EXCHANGE_SUBHEADER_CG_ITEM_ADD:	// arg1 == position of item, arg2 == position in exchange window
			if (ch->GetExchange())
			{
				if (ch->GetExchange()->GetCompany()->GetAcceptStatus() != true)
#if defined(ITEM_CHECKINOUT_UPDATE)
					ch->GetExchange()->AddItem(pinfo->Pos, pinfo->arg2, pinfo->SelectPosAuto);
#else
					ch->GetExchange()->AddItem(pinfo->Pos, pinfo->arg2);
#endif
			}
			break;

		case EXCHANGE_SUBHEADER_CG_ITEM_DEL:	// arg1 == position of item
			if (ch->GetExchange())
			{
				if (ch->GetExchange()->GetCompany()->GetAcceptStatus() != true)
					ch->GetExchange()->RemoveItem(pinfo->arg1);
			}
			break;

		case EXCHANGE_SUBHEADER_CG_ELK_ADD:	// arg1 == amount of gold
			if (ch->GetExchange())
			{
#ifdef YANG_LIMIT
				const LDWORD nTotalGold = static_cast<LDWORD>(ch->GetExchange()->GetCompany()->GetOwner()->GetGold()) + static_cast<LDWORD>(pinfo->arg1);
				if (ch->GetAllowedGold() <= nTotalGold)
#else
				const int64_t nTotalGold = static_cast<int64_t>(ch->GetExchange()->GetCompany()->GetOwner()->GetGold()) + static_cast<int64_t>(pinfo->arg1);
				if (GOLD_MAX <= nTotalGold)
#endif
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÇ ÃÑ±Ý¾×ÀÌ 20¾ï ³ÉÀ» ÃÊ°úÇÏ¿© °Å·¡¸¦ ÇÒ¼ö°¡ ¾ø½À´Ï´Ù.."));

					sys_err("[OVERFLOW_GOLD] ELK_ADD (%u) id %u name %s ",
							ch->GetExchange()->GetCompany()->GetOwner()->GetGold(),
							ch->GetExchange()->GetCompany()->GetOwner()->GetPlayerID(),
						   	ch->GetExchange()->GetCompany()->GetOwner()->GetName());

					return;
				}

				if (ch->GetExchange()->GetCompany()->GetAcceptStatus() != true)
					ch->GetExchange()->AddGold(pinfo->arg1);
			}
			break;
#ifdef ENABLE_CHEQUE_SYSTEM
		case EXCHANGE_SUBHEADER_CG_WON_ADD:	// arg1 == amount of cheque
			if (ch->GetExchange())
			{
				const int64_t nTotalCheque = static_cast<int64_t>(ch->GetExchange()->GetCompany()->GetOwner()->GetCheque()) + static_cast<int64_t>(pinfo->arg1);

				if (CHEQUE_MAX <= nTotalCheque)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("???? ???? 20? ?? ???? ??? ??? ????.."));

					sys_err("[OVERFLOW_CHEQUE] WON_ADD (%u) id %u name %s ",
							ch->GetExchange()->GetCompany()->GetOwner()->GetCheque(),
							ch->GetExchange()->GetCompany()->GetOwner()->GetPlayerID(),
						   	ch->GetExchange()->GetCompany()->GetOwner()->GetName());

					return;
				}

				if (ch->GetExchange()->GetCompany()->GetAcceptStatus() != true)
					ch->GetExchange()->AddCheque(pinfo->arg1);
			}
			break;
#endif

		case EXCHANGE_SUBHEADER_CG_ACCEPT:	// arg1 == not used
			if (ch->GetExchange())
			{
				sys_log(0, "CInputMain()::Exchange() ==> ACCEPT "); 
				ch->GetExchange()->Accept(true);
			}

			break;

		case EXCHANGE_SUBHEADER_CG_CANCEL:	// arg1 == not used
			if (ch->GetExchange())
				ch->GetExchange()->Cancel();
			break;
	}
}

void CInputMain::Position(LPCHARACTER ch, const char * data)
{
	struct command_position * pinfo = (struct command_position *) data;

	switch (pinfo->position)
	{
		case POSITION_GENERAL:
			ch->Standup();
			break;

		case POSITION_SITTING_CHAIR:
			ch->Sitdown(0);
			break;

		case POSITION_SITTING_GROUND:
			ch->Sitdown(1);
			break;
	}
}

static const int ComboSequenceBySkillLevel[3][8] = 
{
	// 0   1   2   3   4   5   6   7
	{ 14, 15, 16, 17,  0,  0,  0,  0 },
	{ 14, 15, 16, 18, 20,  0,  0,  0 },
	{ 14, 15, 16, 18, 19, 17,  0,  0 },
};

#define COMBO_HACK_ALLOWABLE_MS	100

// [2013 09 11 CYH]
DWORD ClacValidComboInterval( LPCHARACTER ch, BYTE bArg )
{
	int nInterval = 300;
	float fAdjustNum = 1.5f; // ÀÏ¹Ý À¯Àú°¡ speed hack ¿¡ °É¸®´Â °ÍÀ» ¸·±â À§ÇØ. 2013.09.10 CYH

	if( !ch )
	{
		sys_err( "ClacValidComboInterval() ch is NULL");
		return nInterval;
	}	

	if( bArg == 13 )
	{
		float normalAttackDuration = CMotionManager::instance().GetNormalAttackDuration(ch->GetRaceNum());
		nInterval = (int) (normalAttackDuration / (((float) ch->GetPoint(POINT_ATT_SPEED) / 100.f) * 900.f) + fAdjustNum );
	}
	else if( bArg == 14 )
	{		
		nInterval = (int)(ani_combo_speed(ch, 1 ) / ((ch->GetPoint(POINT_ATT_SPEED) / 100.f) + fAdjustNum) );
	}
	else if( bArg > 14 && bArg << 22 )
	{
		nInterval = (int)(ani_combo_speed(ch, bArg - 13 ) / ((ch->GetPoint(POINT_ATT_SPEED) / 100.f) + fAdjustNum) );
	}
	else
	{
		sys_err( "ClacValidComboInterval() Invalid bArg(%d) ch(%s)", bArg, ch->GetName() );		
	}	

	return nInterval;
}

bool CheckComboHack(LPCHARACTER ch, BYTE bArg, DWORD dwTime, bool CheckSpeedHack)
{
	//	Á×°Å³ª ±âÀý »óÅÂ¿¡¼­´Â °ø°ÝÇÒ ¼ö ¾øÀ¸¹Ç·Î, skipÇÑ´Ù.
	//	ÀÌ·¸°Ô ÇÏÁö ¸»°í, CHRACTER::CanMove()¿¡ 
	//	if (IsStun() || IsDead()) return false;
	//	¸¦ Ãß°¡ÇÏ´Â°Ô ¸Â´Ù°í »ý°¢ÇÏ³ª,
	//	ÀÌ¹Ì ´Ù¸¥ ºÎºÐ¿¡¼­ CanMove()´Â IsStun(), IsDead()°ú
	//	µ¶¸³ÀûÀ¸·Î Ã¼Å©ÇÏ°í ÀÖ±â ¶§¹®¿¡ ¼öÁ¤¿¡ ÀÇÇÑ ¿µÇâÀ»
	//	ÃÖ¼ÒÈ­ÇÏ±â À§ÇØ ÀÌ·¸°Ô ¶«»§ ÄÚµå¸¦ ½á³õ´Â´Ù.
	if (ch->IsStun() || ch->IsDead())
		return false;
	int ComboInterval = dwTime - ch->GetLastComboTime();
	int HackScalar = 0; // ±âº» ½ºÄ®¶ó ´ÜÀ§ 1

	// [2013 09 11 CYH] debugging log
		/*sys_log(0, "COMBO_TEST_LOG: %s arg:%u interval:%d valid:%u atkspd:%u riding:%s",
						ch->GetName(),
						bArg,
						ComboInterval,
						ch->GetValidComboInterval(),
						ch->GetPoint(POINT_ATT_SPEED),
						ch->IsRiding() ? "yes" : "no");*/

#if 0	
	sys_log(0, "COMBO: %s arg:%u seq:%u delta:%d checkspeedhack:%d",
			ch->GetName(), bArg, ch->GetComboSequence(), ComboInterval - ch->GetValidComboInterval(), CheckSpeedHack);
#endif
	// bArg 14 ~ 21¹ø ±îÁö ÃÑ 8ÄÞº¸ °¡´É
	// 1. Ã¹ ÄÞº¸(14)´Â ÀÏÁ¤ ½Ã°£ ÀÌÈÄ¿¡ ¹Ýº¹ °¡´É
	// 2. 15 ~ 21¹øÀº ¹Ýº¹ ºÒ°¡´É
	// 3. Â÷·Ê´ë·Î Áõ°¡ÇÑ´Ù.
	if (bArg == 14)
	{
		if (CheckSpeedHack && ComboInterval > 0 && ComboInterval < ch->GetValidComboInterval() - COMBO_HACK_ALLOWABLE_MS)
		{
			// FIXME Ã¹¹øÂ° ÄÞº¸´Â ÀÌ»óÇÏ°Ô »¡¸® ¿Ã ¼ö°¡ ÀÖ¾î¼­ 300À¸·Î ³ª´® -_-;
			// ´Ù¼öÀÇ ¸ó½ºÅÍ¿¡ ÀÇÇØ ´Ù¿îµÇ´Â »óÈ²¿¡¼­ °ø°ÝÀ» ÇÏ¸é
			// Ã¹¹øÂ° ÄÞº¸°¡ ¸Å¿ì ÀûÀº ÀÎÅÍ¹ú·Î µé¾î¿À´Â »óÈ² ¹ß»ý.
			// ÀÌ·Î ÀÎÇØ ÄÞº¸ÇÙÀ¸·Î Æ¨±â´Â °æ¿ì°¡ ÀÖ¾î ´ÙÀ½ ÄÚµå ºñ È°¼ºÈ­.
			//HackScalar = 1 + (ch->GetValidComboInterval() - ComboInterval) / 300;

			//sys_log(0, "COMBO_HACK: 2 %s arg:%u interval:%d valid:%u atkspd:%u riding:%s",
			//		ch->GetName(),
			//		bArg,
			//		ComboInterval,
			//		ch->GetValidComboInterval(),
			//		ch->GetPoint(POINT_ATT_SPEED),
			//	    ch->IsRiding() ? "yes" : "no");
		}

		ch->SetComboSequence(1);
		// 2013 09 11 CYH edited
		//ch->SetValidComboInterval((int) (ani_combo_speed(ch, 1) / (ch->GetPoint(POINT_ATT_SPEED) / 100.f)));
		ch->SetValidComboInterval( ClacValidComboInterval(ch, bArg) );
		ch->SetLastComboTime(dwTime);
	}
	else if (bArg > 14 && bArg < 22)
	{
		int idx = MIN(2, ch->GetComboIndex());

		if (ch->GetComboSequence() > 5) // ÇöÀç 6ÄÞº¸ ÀÌ»óÀº ¾ø´Ù.
		{
			HackScalar = 1;
			ch->SetValidComboInterval(300);
			sys_log(0, "COMBO_HACK: 5 %s combo_seq:%d", ch->GetName(), ch->GetComboSequence());
		}
		// ÀÚ°´ ½Ö¼ö ÄÞº¸ ¿¹¿ÜÃ³¸®
		else if (bArg == 21 &&
				 idx == 2 &&
				 ch->GetComboSequence() == 5 &&
				 ch->GetJob() == JOB_ASSASSIN &&
				 ch->GetWear(WEAR_WEAPON) &&
				 ch->GetWear(WEAR_WEAPON)->GetSubType() == WEAPON_DAGGER)
			ch->SetValidComboInterval(300);
		else if (bArg == 21 && idx == 2 && ch->GetComboSequence() == 5 && ch->GetJob() == JOB_WOLFMAN && ch->GetWear(WEAR_WEAPON)->GetSubType() == WEAPON_CLAW)
			ch->SetValidComboInterval(300);
		else if (ComboSequenceBySkillLevel[idx][ch->GetComboSequence()] != bArg)
		{
			HackScalar = 1;
			ch->SetValidComboInterval(300);

			sys_log(0, "COMBO_HACK: 3 %s arg:%u valid:%u combo_idx:%d combo_seq:%d",
					ch->GetName(),
					bArg,
					ComboSequenceBySkillLevel[idx][ch->GetComboSequence()],
					idx,
					ch->GetComboSequence());
		}
		else
		{
			if (CheckSpeedHack && ComboInterval < ch->GetValidComboInterval() - COMBO_HACK_ALLOWABLE_MS)
			{
				HackScalar = 1 + (ch->GetValidComboInterval() - ComboInterval) / 100;

				sys_log(0, "COMBO_HACK: 2 %s arg:%u interval:%d valid:%u atkspd:%u riding:%s",
						ch->GetName(),
						bArg,
						ComboInterval,
						ch->GetValidComboInterval(),
						ch->GetPoint(POINT_ATT_SPEED),
						ch->IsRiding() ? "yes" : "no");
			}

			// ¸»À» ÅÀÀ» ¶§´Â 15¹ø ~ 16¹øÀ» ¹Ýº¹ÇÑ´Ù
			//if (ch->IsHorseRiding())
			if (ch->IsRiding())
				ch->SetComboSequence(ch->GetComboSequence() == 1 ? 2 : 1);
			else
				ch->SetComboSequence(ch->GetComboSequence() + 1);

			// 2013 09 11 CYH edited
			//ch->SetValidComboInterval((int) (ani_combo_speed(ch, bArg - 13) / (ch->GetPoint(POINT_ATT_SPEED) / 100.f)));
			ch->SetValidComboInterval( ClacValidComboInterval(ch, bArg) );
			ch->SetLastComboTime(dwTime);
		}
	}
	else if (bArg == 13) // ±âº» °ø°Ý (µÐ°©(Polymorph)ÇßÀ» ¶§ ¿Â´Ù)
	{
		if (CheckSpeedHack && ComboInterval > 0 && ComboInterval < ch->GetValidComboInterval() - COMBO_HACK_ALLOWABLE_MS)
		{
			// ´Ù¼öÀÇ ¸ó½ºÅÍ¿¡ ÀÇÇØ ´Ù¿îµÇ´Â »óÈ²¿¡¼­ °ø°ÝÀ» ÇÏ¸é
			// Ã¹¹øÂ° ÄÞº¸°¡ ¸Å¿ì ÀûÀº ÀÎÅÍ¹ú·Î µé¾î¿À´Â »óÈ² ¹ß»ý.
			// ÀÌ·Î ÀÎÇØ ÄÞº¸ÇÙÀ¸·Î Æ¨±â´Â °æ¿ì°¡ ÀÖ¾î ´ÙÀ½ ÄÚµå ºñ È°¼ºÈ­.
			//HackScalar = 1 + (ch->GetValidComboInterval() - ComboInterval) / 100;

			//sys_log(0, "COMBO_HACK: 6 %s arg:%u interval:%d valid:%u atkspd:%u",
			//		ch->GetName(),
			//		bArg,
			//		ComboInterval,
			//		ch->GetValidComboInterval(),
			//		ch->GetPoint(POINT_ATT_SPEED));
		}

		if (ch->GetRaceNum() >= MAIN_RACE_MAX_NUM)
		{
			// POLYMORPH_BUG_FIX
			
			// DELETEME
			/*
			const CMotion * pkMotion = CMotionManager::instance().GetMotion(ch->GetRaceNum(), MAKE_MOTION_KEY(MOTION_MODE_GENERAL, MOTION_NORMAL_ATTACK));

			if (!pkMotion)
				sys_err("cannot find motion by race %u", ch->GetRaceNum());
			else
			{
				// Á¤»óÀû °è»êÀÌ¶ó¸é 1000.f¸¦ °öÇØ¾ß ÇÏÁö¸¸ Å¬¶óÀÌ¾ðÆ®°¡ ¾Ö´Ï¸ÞÀÌ¼Ç ¼ÓµµÀÇ 90%¿¡¼­
				// ´ÙÀ½ ¾Ö´Ï¸ÞÀÌ¼Ç ºí·»µùÀ» Çã¿ëÇÏ¹Ç·Î 900.f¸¦ °öÇÑ´Ù.
				int k = (int) (pkMotion->GetDuration() / ((float) ch->GetPoint(POINT_ATT_SPEED) / 100.f) * 900.f);
				ch->SetValidComboInterval(k);
				ch->SetLastComboTime(dwTime);
			}
			*/

			// 2013 09 11 CYH edited
			//float normalAttackDuration = CMotionManager::instance().GetNormalAttackDuration(ch->GetRaceNum());
			//int k = (int) (normalAttackDuration / ((float) ch->GetPoint(POINT_ATT_SPEED) / 100.f) * 900.f);			
			//ch->SetValidComboInterval(k);
			ch->SetValidComboInterval( ClacValidComboInterval(ch, bArg) );
			ch->SetLastComboTime(dwTime);
			// END_OF_POLYMORPH_BUG_FIX
		}
		else
		{
			// ¸»ÀÌ ¾ÈµÇ´Â ÄÞº¸°¡ ¿Ô´Ù ÇØÄ¿ÀÏ °¡´É¼º?
			//if (ch->GetDesc()->DelayedDisconnect(number(2, 9)))
			//{
			//	LogManager::instance().HackLog("Hacker", ch);
			//	sys_log(0, "HACKER: %s arg %u", ch->GetName(), bArg);
			//}

			// À§ ÄÚµå·Î ÀÎÇØ, Æú¸®¸ðÇÁ¸¦ Çª´Â Áß¿¡ °ø°Ý ÇÏ¸é,
			// °¡²û ÇÙÀ¸·Î ÀÎ½ÄÇÏ´Â °æ¿ì°¡ ÀÖ´Ù.

			// ÀÚ¼¼È÷ ¸»Çô¸é,
			// ¼­¹ö¿¡¼­ poly 0¸¦ Ã³¸®ÇßÁö¸¸,
			// Å¬¶ó¿¡¼­ ±× ÆÐÅ¶À» ¹Þ±â Àü¿¡, ¸÷À» °ø°Ý. <- Áï, ¸÷ÀÎ »óÅÂ¿¡¼­ °ø°Ý.
			//
			// ±×·¯¸é Å¬¶ó¿¡¼­´Â ¼­¹ö¿¡ ¸÷ »óÅÂ·Î °ø°ÝÇß´Ù´Â Ä¿¸Çµå¸¦ º¸³»°í (arg == 13)
			//
			// ¼­¹ö¿¡¼­´Â race´Â ÀÎ°£ÀÎµ¥ °ø°ÝÇüÅÂ´Â ¸÷ÀÎ ³ðÀÌ´Ù! ¶ó°í ÇÏ¿© ÇÙÃ¼Å©¸¦ Çß´Ù.

			// »ç½Ç °ø°Ý ÆÐÅÏ¿¡ ´ëÇÑ °ÍÀº Å¬¶óÀÌ¾ðÆ®¿¡¼­ ÆÇ´ÜÇØ¼­ º¸³¾ °ÍÀÌ ¾Æ´Ï¶ó,
			// ¼­¹ö¿¡¼­ ÆÇ´ÜÇØ¾ß ÇÒ °ÍÀÎµ¥... ¿Ö ÀÌ·¸°Ô ÇØ³ùÀ»±î...
			// by rtsummit
		}
	}
	else
	{
		// ¸»ÀÌ ¾ÈµÇ´Â ÄÞº¸°¡ ¿Ô´Ù ÇØÄ¿ÀÏ °¡´É¼º?
		if (ch->GetDesc()->DelayedDisconnect(number(2, 9)))
		{
			LogManager::instance().HackLog("Hacker", ch);
			sys_log(0, "HACKER: %s arg %u", ch->GetName(), bArg);
		}

		HackScalar = 10;
		ch->SetValidComboInterval(300);
	}

	if (HackScalar)
	{
		// ¸»¿¡ Å¸°Å³ª ³»·ÈÀ» ¶§ 1.5ÃÊ°£ °ø°ÝÀº ÇÙÀ¸·Î °£ÁÖÇÏÁö ¾ÊµÇ °ø°Ý·ÂÀº ¾ø°Ô ÇÏ´Â Ã³¸®
		if (get_dword_time() - ch->GetLastMountTime() > 1500)
			ch->IncreaseComboHackCount(1 + HackScalar);

		ch->SkipComboAttackByTime(ch->GetValidComboInterval());
	}

	return HackScalar;
}

void CInputMain::Move(LPCHARACTER ch, const char * data)
{
	if (!ch->CanMove())
		return;

	struct command_move * pinfo = (struct command_move *) data;
#ifdef URIEL_ANTI_CHEAT
	// Hicbir zaman herhangi biri ile asagidaki sayilari paylasmayiniz.
	// Sayilari el ile degistirmeye calismayiniz, tum yapiyi bozabilirsiniz.
	unsigned long a = 724550227, b = 1237831417, c = pinfo->uX, d = pinfo->uY;
	
	unsigned long verify_x = pinfo->lX ^ a;
	verify_x += b;
	verify_x >>= 3;
	verify_x |= 0xAA11;
	verify_x ^= b;
	
	if(verify_x != c)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Anti-Cheat error 49");
		ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY(), ch->GetZ());
		ch->Stop();
		return;
	}
	
	unsigned long verify_y = pinfo->lY ^ a;
	verify_y += b;
	verify_y >>= 3;
	verify_y |= 0xAA11;
	verify_y ^= b;
	
	if(verify_y != d)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Anti-Cheat error 50");
		ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY(), ch->GetZ());
		ch->Stop();
		return;
	}
#endif

	if (pinfo->bFunc >= FUNC_MAX_NUM && !(pinfo->bFunc & 0x80))
	{
		sys_err("invalid move type: %s", ch->GetName());
		return;
	}

	//enum EMoveFuncType
	//{   
	//	FUNC_WAIT,
	//	FUNC_MOVE,
	//	FUNC_ATTACK,
	//	FUNC_COMBO,
	//	FUNC_MOB_SKILL,
	//	_FUNC_SKILL,
	//	FUNC_MAX_NUM,
	//	FUNC_SKILL = 0x80,
	//};  

	// ÅÚ·¹Æ÷Æ® ÇÙ Ã¼Å©

//	if (!test_server)	//2012.05.15 ±è¿ë¿í : Å×¼·¿¡¼­ (¹«Àû»óÅÂ·Î) ´Ù¼ö ¸ó½ºÅÍ »ó´ë·Î ´Ù¿îµÇ¸é¼­ °ø°Ý½Ã ÄÞº¸ÇÙÀ¸·Î Á×´Â ¹®Á¦°¡ ÀÖ¾ú´Ù.
	{
		const float fDist = DISTANCE_SQRT((ch->GetX() - pinfo->lX) / 100, (ch->GetY() - pinfo->lY) / 100);

		if (((false == ch->IsRiding() && fDist > 575) || fDist > 600) && OXEVENT_MAP_INDEX != ch->GetMapIndex())
		{
			if( false == LC_IsEurope() )
			{
				const PIXEL_POSITION & warpPos = ch->GetWarpPosition();

				if (warpPos.x == 0 && warpPos.y == 0)
					LogManager::instance().HackLog("Teleport", ch); // ºÎÁ¤È®ÇÒ ¼ö ÀÖÀ½
			}

			sys_log(0, "MOVE: %s trying to move too far (dist: %.1fm) Riding(%d)", ch->GetName(), fDist, ch->IsRiding());

			ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY(), ch->GetZ());
			ch->Stop();
			return;
		}

		//
		// ½ºÇÇµåÇÙ(SPEEDHACK) Check
		//
		DWORD dwCurTime = get_dword_time();
		// ½Ã°£À» SyncÇÏ°í 7ÃÊ ÈÄ ºÎÅÍ °Ë»çÇÑ´Ù. (20090702 ÀÌÀü¿£ 5ÃÊ¿´À½)
		bool CheckSpeedHack = (false == ch->GetDesc()->IsHandshaking() && dwCurTime - ch->GetDesc()->GetClientTime() > 7000);

		/*if (CheckSpeedHack)
		{
			int iDelta = (int) (pinfo->dwTime - ch->GetDesc()->GetClientTime());
			int iServerDelta = (int) (dwCurTime - ch->GetDesc()->GetClientTime());

			iDelta = (int) (dwCurTime - pinfo->dwTime);

			// ½Ã°£ÀÌ ´Ê°Ô°£´Ù. ÀÏ´Ü ·Î±×¸¸ ÇØµÐ´Ù. ÁøÂ¥ ÀÌ·± »ç¶÷µéÀÌ ¸¹ÀºÁö Ã¼Å©ÇØ¾ßÇÔ. TODO
			if (iDelta >= 30000)
			{
				sys_log(0, "SPEEDHACK: slow timer name %s delta %d", ch->GetName(), iDelta);
				ch->GetDesc()->DelayedDisconnect(3);
			}
			// 1ÃÊ¿¡ 20msec »¡¸® °¡´Â°Å ±îÁö´Â ÀÌÇØÇÑ´Ù.
			else if (iDelta < -(iServerDelta / 50))
			{
				sys_log(0, "SPEEDHACK: DETECTED! %s (delta %d %d)", ch->GetName(), iDelta, iServerDelta);
				ch->GetDesc()->DelayedDisconnect(3);
			}
		}

		//
		// ÄÞº¸ÇÙ ¹× ½ºÇÇµåÇÙ Ã¼Å©
		//
		if (pinfo->bFunc == FUNC_COMBO && g_bCheckMultiHack)
		{
			CheckComboHack(ch, pinfo->bArg, pinfo->dwTime, CheckSpeedHack); // ÄÞº¸ Ã¼Å©
		}*/
	}

	if (pinfo->bFunc == FUNC_MOVE)
	{
		if (ch->GetLimitPoint(POINT_MOV_SPEED) == 0)
			return;

		ch->SetRotation(pinfo->bRot * 5);	// Áßº¹ ÄÚµå
		ch->ResetStopTime();				// ""

		ch->Goto(pinfo->lX, pinfo->lY);
	}
	else
	{
		if (pinfo->bFunc == FUNC_ATTACK || pinfo->bFunc == FUNC_COMBO)
			ch->OnMove(true);
		else if (pinfo->bFunc & FUNC_SKILL)
		{
			const int MASK_SKILL_MOTION = 0x7F;
			unsigned int motion = pinfo->bFunc & MASK_SKILL_MOTION;

			if (!ch->IsUsableSkillMotion(motion))
			{
				const char* name = ch->GetName();
				unsigned int job = ch->GetJob();
				unsigned int group = ch->GetSkillGroup();

				char szBuf[256];
				snprintf(szBuf, sizeof(szBuf), "SKILL_HACK: name=%s, job=%d, group=%d, motion=%d", name, job, group, motion);
				LogManager::instance().HackLog(szBuf, ch->GetDesc()->GetAccountTable().login, ch->GetName(), ch->GetDesc()->GetHostName());
				sys_log(0, "%s", szBuf);

				if (test_server)
				{
					ch->GetDesc()->DelayedDisconnect(number(2, 8));
					ch->ChatPacket(CHAT_TYPE_INFO, szBuf);
				}
				else
				{
					ch->GetDesc()->DelayedDisconnect(number(150, 500));
				}
			}

			ch->OnMove();
		}

		ch->SetRotation(pinfo->bRot * 5);	// Áßº¹ ÄÚµå
		ch->ResetStopTime();				// ""

		ch->Move(pinfo->lX, pinfo->lY);
		ch->Stop();
		ch->StopStaminaConsume();
	}

	TPacketGCMove pack;

	pack.bHeader      = HEADER_GC_MOVE;
	pack.bFunc        = pinfo->bFunc;
	pack.bArg         = pinfo->bArg;
	pack.bRot         = pinfo->bRot;
	pack.dwVID        = ch->GetVID();
	pack.lX           = pinfo->lX;
	pack.lY           = pinfo->lY;
	pack.dwTime       = pinfo->dwTime;
	pack.dwDuration   = (pinfo->bFunc == FUNC_MOVE) ? ch->GetCurrentMoveDuration() : 0;

	ch->PacketAround(&pack, sizeof(TPacketGCMove), ch);
/*
	if (pinfo->dwTime == 10653691) // µð¹ö°Å ¹ß°ß
	{
		if (ch->GetDesc()->DelayedDisconnect(number(15, 30)))
			LogManager::instance().HackLog("Debugger", ch);

	}
	else if (pinfo->dwTime == 10653971) // Softice ¹ß°ß
	{
		if (ch->GetDesc()->DelayedDisconnect(number(15, 30)))
			LogManager::instance().HackLog("Softice", ch);
	}
*/
	/*
	sys_log(0, 
			"MOVE: %s Func:%u Arg:%u Pos:%dx%d Time:%u Dist:%.1f",
			ch->GetName(),
			pinfo->bFunc,
			pinfo->bArg,
			pinfo->lX / 100,
			pinfo->lY / 100,
			pinfo->dwTime,
			fDist);
	*/
}

#ifdef __SKILL_COLOR_SYSTEM__
void CInputMain::SetSkillColor(LPCHARACTER ch, const char* pcData)
{
	TPacketCGSkillColor * p = (TPacketCGSkillColor*)pcData;

	if (p->skill >= ESkillColorLength::MAX_SKILL_COUNT)
		return;

	DWORD data[ESkillColorLength::MAX_SKILL_COUNT + ESkillColorLength::MAX_BUFF_COUNT][ESkillColorLength::MAX_EFFECT_COUNT];
	memcpy(data, ch->GetSkillColor(), sizeof(data));

	data[p->skill][0] = p->col1;
	data[p->skill][1] = p->col2;
	data[p->skill][2] = p->col3;
	data[p->skill][3] = p->col4;
	data[p->skill][4] = p->col5;

	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have changed the color of your skill."));

	ch->SetSkillColor(data[0]);

	TSkillColor db_pack;
	memcpy(db_pack.dwSkillColor, data, sizeof(data));
	db_pack.player_id = ch->GetPlayerID();
	db_clientdesc->DBPacketHeader(HEADER_GD_SKILL_COLOR_SAVE, 0, sizeof(TSkillColor));
	db_clientdesc->Packet(&db_pack, sizeof(TSkillColor));
}
#endif

void CInputMain::Attack(LPCHARACTER ch, const BYTE header, const char* data)
{
	if (NULL == ch)
		return;

	struct type_identifier
	{
		BYTE header;
		BYTE type;
	};

	const struct type_identifier* const type = reinterpret_cast<const struct type_identifier*>(data);

	if (type->type > 0)
	{
		if (false == ch->CanUseSkill(type->type))
		{
			return;
		}

		switch (type->type)
		{
			case SKILL_GEOMPUNG:
			case SKILL_SANGONG:
			case SKILL_YEONSA:
			case SKILL_KWANKYEOK:
			case SKILL_HWAJO:
			case SKILL_GIGUNG:
			case SKILL_PABEOB:
			case SKILL_MARYUNG:
			case SKILL_TUSOK:
			case SKILL_MAHWAN:
			case SKILL_BIPABU:
			case SKILL_NOEJEON:
			case SKILL_CHAIN:
			case SKILL_HORSE_WILDATTACK_RANGE:
				if (HEADER_CG_SHOOT != type->header)
				{
					if (test_server) 
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Attack :name[%s] Vnum[%d] can't use skill by attack(warning)"), type->type);
					return;
				}
				break;
		}
	}

	switch (header)
	{
		case HEADER_CG_ATTACK:
			{
				if (NULL == ch->GetDesc())
					return;

				const TPacketCGAttack* const packMelee = reinterpret_cast<const TPacketCGAttack*>(data);
#ifdef URIEL_ANTI_CHEAT
	// Hicbir zaman herhangi biri ile asagidaki sayilari paylasmayiniz.
	// Sayilari el ile degistirmeye calismayiniz, tum yapiyi bozabilirsiniz.
	unsigned long a = 172054597, b = 310715597;
	
	unsigned long verify_vid = packMelee->dwVID ^ a;
	verify_vid += b;
	verify_vid >>= 3;
	verify_vid |= 0xAA11;
	verify_vid ^= b;
	
	if(verify_vid != packMelee->dwVictimVIDVerify)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Anti-Cheat error 51");
		ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY(), ch->GetZ());
		ch->Stop();
		return;
	}
#endif

				ch->GetDesc()->AssembleCRCMagicCube(packMelee->bCRCMagicCubeProcPiece, packMelee->bCRCMagicCubeFilePiece);

				LPCHARACTER	victim = CHARACTER_MANAGER::instance().Find(packMelee->dwVID);

				if (NULL == victim || ch == victim)
					return;

				switch (victim->GetCharType())
				{
					case CHAR_TYPE_NPC:
					case CHAR_TYPE_WARP:
					case CHAR_TYPE_GOTO:
						return;
				}

				if (packMelee->bType > 0)
				{
					if (false == ch->CheckSkillHitCount(packMelee->bType, victim->GetVID()))
					{
						return;
					}
				}

				ch->Attack(victim, packMelee->bType);
			}
			break;

		case HEADER_CG_SHOOT:
			{
				const TPacketCGShoot* const packShoot = reinterpret_cast<const TPacketCGShoot*>(data);

				ch->Shoot(packShoot->bType);
			}
			break;
	}
}

int CInputMain::SyncPosition(LPCHARACTER ch, const char * c_pcData, size_t uiBytes)
{
	const TPacketCGSyncPosition* pinfo = reinterpret_cast<const TPacketCGSyncPosition*>( c_pcData );

	if (uiBytes < pinfo->wSize)
		return -1;

	int iExtraLen = pinfo->wSize - sizeof(TPacketCGSyncPosition);

	if (iExtraLen < 0)
	{
		sys_err("invalid packet length (len %d size %u buffer %u)", iExtraLen, pinfo->wSize, uiBytes);
		ch->GetDesc()->SetPhase(PHASE_CLOSE);
		return -1;
	}

	if (0 != (iExtraLen % sizeof(TPacketCGSyncPositionElement)))
	{
		sys_err("invalid packet length %d (name: %s)", pinfo->wSize, ch->GetName());
		return iExtraLen;
	}

	int iCount = iExtraLen / sizeof(TPacketCGSyncPositionElement);

	if (iCount <= 0)
		return iExtraLen;

	static const int nCountLimit = 16;

	if( iCount > nCountLimit )
	{
		//LogManager::instance().HackLog( "SYNC_POSITION_HACK", ch );
		sys_err( "Too many SyncPosition Count(%d) from Name(%s)", iCount, ch->GetName() );
		//ch->GetDesc()->SetPhase(PHASE_CLOSE);
		//return -1;
		iCount = nCountLimit;
	}

	TEMP_BUFFER tbuf;
	LPBUFFER lpBuf = tbuf.getptr();

	TPacketGCSyncPosition * pHeader = (TPacketGCSyncPosition *) buffer_write_peek(lpBuf);
	buffer_write_proceed(lpBuf, sizeof(TPacketGCSyncPosition));

	const TPacketCGSyncPositionElement* e = 
		reinterpret_cast<const TPacketCGSyncPositionElement*>(c_pcData + sizeof(TPacketCGSyncPosition));

	timeval tvCurTime;
	gettimeofday(&tvCurTime, NULL);

	for (int i = 0; i < iCount; ++i, ++e)
	{
		LPCHARACTER victim = CHARACTER_MANAGER::instance().Find(e->dwVID);

		if (!victim)
			continue;

		switch (victim->GetCharType())
		{
			case CHAR_TYPE_NPC:
			case CHAR_TYPE_WARP:
			case CHAR_TYPE_GOTO:
				continue;
		}

		// ¼ÒÀ¯±Ç °Ë»ç
		if (!victim->SetSyncOwner(ch))
			continue;

		const float fDistWithSyncOwner = DISTANCE_SQRT( (victim->GetX() - ch->GetX()) / 100, (victim->GetY() - ch->GetY()) / 100 );
		static const float fLimitDistWithSyncOwner = 2500.f + 1000.f;
		// victim°úÀÇ °Å¸®°¡ 2500 + a ÀÌ»óÀÌ¸é ÇÙÀ¸·Î °£ÁÖ.
		//	°Å¸® ÂüÁ¶ : Å¬¶óÀÌ¾ðÆ®ÀÇ __GetSkillTargetRange, __GetBowRange ÇÔ¼ö
		//	2500 : ½ºÅ³ proto¿¡¼­ °¡Àå »ç°Å¸®°¡ ±ä ½ºÅ³ÀÇ »ç°Å¸®, ¶Ç´Â È°ÀÇ »ç°Å¸®
		//	a = POINT_BOW_DISTANCE °ª... ÀÎµ¥ ½ÇÁ¦·Î »ç¿ëÇÏ´Â °ªÀÎÁö´Â Àß ¸ð¸£°ÚÀ½. ¾ÆÀÌÅÛÀÌ³ª Æ÷¼Ç, ½ºÅ³, Äù½ºÆ®¿¡´Â ¾ø´Âµ¥...
		//		±×·¡µµ È¤½Ã³ª ÇÏ´Â ¸¶À½¿¡ ¹öÆÛ·Î »ç¿ëÇÒ °âÇØ¼­ 1000.f ·Î µÒ...
		if (fDistWithSyncOwner > fLimitDistWithSyncOwner)
		{
			// g_iSyncHackLimitCount¹ø ±îÁö´Â ºÁÁÜ.
			/*if (ch->GetSyncHackCount() < g_iSyncHackLimitCount)
			{
				ch->SetSyncHackCount(ch->GetSyncHackCount() + 1);
				continue;
			}
			else
			{
				LogManager::instance().HackLog( "SYNC_POSITION_HACK", ch );

				sys_err( "Too far SyncPosition DistanceWithSyncOwner(%f)(%s) from Name(%s) CH(%d,%d) VICTIM(%d,%d) SYNC(%d,%d)",
					fDistWithSyncOwner, victim->GetName(), ch->GetName(), ch->GetX(), ch->GetY(), victim->GetX(), victim->GetY(),
					e->lX, e->lY );

				ch->GetDesc()->SetPhase(PHASE_CLOSE);

				return -1;
			}*/
		}
		
		const float fDist = DISTANCE_SQRT( (victim->GetX() - e->lX) / 100, (victim->GetY() - e->lY) / 100 );
		static const long g_lValidSyncInterval = 50 * 1000; // 100ms -> 50ms 2013 09 11 CYH
		const timeval &tvLastSyncTime = victim->GetLastSyncTime();
		timeval *tvDiff = timediff(&tvCurTime, &tvLastSyncTime);
		
		// SyncPositionÀ» ¾Ç¿ëÇÏ¿© Å¸À¯Àú¸¦ ÀÌ»óÇÑ °÷À¸·Î º¸³»´Â ÇÙ ¹æ¾îÇÏ±â À§ÇÏ¿©,
		// °°Àº À¯Àú¸¦ g_lValidSyncInterval ms ÀÌ³»¿¡ ´Ù½Ã SyncPositionÇÏ·Á°í ÇÏ¸é ÇÙÀ¸·Î °£ÁÖ.
		if (tvDiff->tv_sec == 0 && tvDiff->tv_usec < g_lValidSyncInterval)
		{
			// g_iSyncHackLimitCount¹ø ±îÁö´Â ºÁÁÜ.
			if (ch->GetSyncHackCount() < g_iSyncHackLimitCount)
			{
				ch->SetSyncHackCount(ch->GetSyncHackCount() + 1);
				continue;
			}
			else
			{
				LogManager::instance().HackLog( "SYNC_POSITION_HACK", ch );

				sys_err( "Too often SyncPosition Interval(%ldms)(%s) from Name(%s) VICTIM(%d,%d) SYNC(%d,%d)",
					tvDiff->tv_sec * 1000 + tvDiff->tv_usec / 1000, victim->GetName(), ch->GetName(), victim->GetX(), victim->GetY(),
					e->lX, e->lY );

				ch->GetDesc()->SetPhase(PHASE_CLOSE);

				return -1;
			}
		}
		else if( fDist > 25.0f )
		{
			LogManager::instance().HackLog( "SYNC_POSITION_HACK", ch );

			sys_err( "Too far SyncPosition Distance(%f)(%s) from Name(%s) CH(%d,%d) VICTIM(%d,%d) SYNC(%d,%d)",
				   	fDist, victim->GetName(), ch->GetName(), ch->GetX(), ch->GetY(), victim->GetX(), victim->GetY(),
				  e->lX, e->lY );

			ch->GetDesc()->SetPhase(PHASE_CLOSE);

			return -1;
		}
		else
		{
			victim->SetLastSyncTime(tvCurTime);
			victim->Sync(e->lX, e->lY);
			buffer_write(lpBuf, e, sizeof(TPacketCGSyncPositionElement));
		}
	}

	if (buffer_size(lpBuf) != sizeof(TPacketGCSyncPosition))
	{
		pHeader->bHeader = HEADER_GC_SYNC_POSITION;
		pHeader->wSize = buffer_size(lpBuf);

		ch->PacketAround(buffer_read_peek(lpBuf), buffer_size(lpBuf), ch);
	}

	return iExtraLen;
}

void CInputMain::FlyTarget(LPCHARACTER ch, const char * pcData, BYTE bHeader)
{
	TPacketCGFlyTargeting * p = (TPacketCGFlyTargeting *) pcData;
	ch->FlyTarget(p->dwTargetVID, p->x, p->y, bHeader);
}

void CInputMain::UseSkill(LPCHARACTER ch, const char * pcData)
{
#ifdef WJ_SECURITY_SYSTEM
	if (ch->IsActivateSecurity() == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GUVENLIK_AKTIF_HATA"));
		return;
	}
#endif
	TPacketCGUseSkill * p = (TPacketCGUseSkill *) pcData;
	ch->UseSkill(p->dwVnum, CHARACTER_MANAGER::instance().Find(p->dwVID));
}

void CInputMain::ScriptButton(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGScriptButton * p = (TPacketCGScriptButton *) c_pData;
	sys_log(0, "QUEST ScriptButton pid %d idx %u", ch->GetPlayerID(), p->idx);

	quest::PC* pc = quest::CQuestManager::instance().GetPCForce(ch->GetPlayerID());
	if (pc && pc->IsConfirmWait())
	{
		quest::CQuestManager::instance().Confirm(ch->GetPlayerID(), quest::CONFIRM_TIMEOUT);
	}
	else if (p->idx & 0x80000000)
	{
		quest::CQuestManager::Instance().QuestInfo(ch->GetPlayerID(), p->idx & 0x7fffffff);
	}
	else
	{
		quest::CQuestManager::Instance().QuestButton(ch->GetPlayerID(), p->idx);
	}
}

void CInputMain::ScriptAnswer(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGScriptAnswer * p = (TPacketCGScriptAnswer *) c_pData;
	sys_log(0, "QUEST ScriptAnswer pid %d answer %d", ch->GetPlayerID(), p->answer);

	if (p->answer > 250) // ´ÙÀ½ ¹öÆ°¿¡ ´ëÇÑ ÀÀ´äÀ¸·Î ¿Â ÆÐÅ¶ÀÎ °æ¿ì
	{
		quest::CQuestManager::Instance().Resume(ch->GetPlayerID());
	}
	else // ¼±ÅÃ ¹öÆ°À» °ñ¶ó¼­ ¿Â ÆÐÅ¶ÀÎ °æ¿ì
	{
		quest::CQuestManager::Instance().Select(ch->GetPlayerID(),  p->answer);
	}
}


// SCRIPT_SELECT_ITEM
void CInputMain::ScriptSelectItem(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGScriptSelectItem* p = (TPacketCGScriptSelectItem*) c_pData;
	sys_log(0, "QUEST ScriptSelectItem pid %d answer %d", ch->GetPlayerID(), p->selection);
	quest::CQuestManager::Instance().SelectItem(ch->GetPlayerID(), p->selection);
}
// END_OF_SCRIPT_SELECT_ITEM

void CInputMain::QuestInputString(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGQuestInputString * p = (TPacketCGQuestInputString*) c_pData;

	char msg[65];
	strlcpy(msg, p->msg, sizeof(msg));
	sys_log(0, "QUEST InputString pid %u msg %s", ch->GetPlayerID(), msg);

	quest::CQuestManager::Instance().Input(ch->GetPlayerID(), msg);
}

void CInputMain::QuestConfirm(LPCHARACTER ch, const void* c_pData)
{
	TPacketCGQuestConfirm* p = (TPacketCGQuestConfirm*) c_pData;
	LPCHARACTER ch_wait = CHARACTER_MANAGER::instance().FindByPID(p->requestPID);
	if (p->answer)
		p->answer = quest::CONFIRM_YES;
	sys_log(0, "QuestConfirm from %s pid %u name %s answer %d", ch->GetName(), p->requestPID, (ch_wait)?ch_wait->GetName():"", p->answer);
	if (ch_wait)
	{
		quest::CQuestManager::Instance().Confirm(ch_wait->GetPlayerID(), (quest::EQuestConfirmType) p->answer, ch->GetPlayerID());
	}
}

void CInputMain::Target(LPCHARACTER ch, const char * pcData)
{
	TPacketCGTarget * p = (TPacketCGTarget *) pcData;

	building::LPOBJECT pkObj = building::CManager::instance().FindObjectByVID(p->dwVID);

	if (pkObj)
	{
		TPacketGCTarget pckTarget;
		pckTarget.header = HEADER_GC_TARGET;
		pckTarget.dwVID = p->dwVID;
		ch->GetDesc()->Packet(&pckTarget, sizeof(TPacketGCTarget));
	}
	else
		ch->SetTarget(CHARACTER_MANAGER::instance().Find(p->dwVID));
}

void CInputMain::Warp(LPCHARACTER ch, const char * pcData)
{
	ch->WarpEnd();
}

void CInputMain::SafeboxCheckin(LPCHARACTER ch, const char * c_pData)
{
	if (quest::CQuestManager::instance().GetPCForce(ch->GetPlayerID())->IsRunning() == true)
		return;

	TPacketCGSafeboxCheckin * p = (TPacketCGSafeboxCheckin *) c_pData;

	if (!ch->CanHandleItem())
		return;

	CSafebox * pkSafebox = ch->GetSafebox();
	LPITEM pkItem = ch->GetItem(p->ItemPos);

	if (!pkSafebox || !pkItem)
		return;
	
#if defined(ITEM_CHECKINOUT_UPDATE)
	if (p->SelectPosAuto)
	{
		int AutoPos = pkSafebox->GetEmptySafebox(pkItem->GetSize());
		if (AutoPos == -1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "<SAFEBOX> You don't have enough space.");
			return;
		}
		p->bSafePos = AutoPos;
	}
#endif

	if (pkItem->GetCell() >= INVENTORY_MAX_NUM && IS_SET(pkItem->GetFlag(), ITEM_FLAG_IRREMOVABLE))
	{
	    ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Ã¢°í·Î ¿Å±æ ¼ö ¾ø´Â ¾ÆÀÌÅÛ ÀÔ´Ï´Ù."));
	    return;
	}

	if (!pkSafebox->IsEmpty(p->bSafePos, pkItem->GetSize()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
		return;
	}

	if (pkItem->GetVnum() == UNIQUE_ITEM_SAFEBOX_EXPAND)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ÀÌ ¾ÆÀÌÅÛÀº ³ÖÀ» ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if( IS_SET(pkItem->GetAntiFlag(), ITEM_ANTIFLAG_SAFEBOX) )
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ÀÌ ¾ÆÀÌÅÛÀº ³ÖÀ» ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (true == pkItem->isLocked())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ÀÌ ¾ÆÀÌÅÛÀº ³ÖÀ» ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (true == pkItem->IsEquipped())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Storage> You can't store an equiped item."));
			return;
	}
	pkItem->RemoveFromCharacter();
#ifdef ENABLE_SPECIAL_STORAGE
	if (!pkItem->IsDragonSoul() && !pkItem->IsUpgradeItem() && !pkItem->IsStone() && !pkItem->IsChest() && !pkItem->IsAttr())
#else
	if (!pkItem->IsDragonSoul())
#endif
		ch->SyncQuickslot(QUICKSLOT_TYPE_ITEM, p->ItemPos.cell, 255);
	pkSafebox->Add(p->bSafePos, pkItem);
	
	char szHint[128];
	snprintf(szHint, sizeof(szHint), "%s %u", pkItem->GetName(), pkItem->GetCount());
	LogManager::instance().ItemLog(ch, pkItem, "SAFEBOX PUT", szHint);
}

void CInputMain::SafeboxCheckout(LPCHARACTER ch, const char * c_pData, bool bMall)
{
	TPacketCGSafeboxCheckout * p = (TPacketCGSafeboxCheckout *) c_pData;

	if (!ch->CanHandleItem())
		return;

	CSafebox * pkSafebox;

	if (bMall)
		pkSafebox = ch->GetMall();
	else
		pkSafebox = ch->GetSafebox();

	if (!pkSafebox)
		return;

	LPITEM pkItem = pkSafebox->Get(p->bSafePos);

	if (!pkItem)
		return;
	
#if defined(ITEM_CHECKINOUT_UPDATE)
	if (p->SelectPosAuto)
	{
		int AutoPos = pkItem->IsDragonSoul() ? ch->GetEmptyDragonSoulInventory(pkItem) : ch->GetEmptyInventory(pkItem->GetSize());
		if (AutoPos == -1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "You don't have enough space.");
			return;
		}
		p->ItemPos = TItemPos(pkItem->IsDragonSoul() ? DRAGON_SOUL_INVENTORY : INVENTORY, AutoPos);
	}
#endif
	if (!ch->IsEmptyItemGrid(p->ItemPos, pkItem->GetSize()))
		return;

	// ¾ÆÀÌÅÛ ¸ô¿¡¼­ ÀÎº¥À¸·Î ¿Å±â´Â ºÎºÐ¿¡¼­ ¿ëÈ¥¼® Æ¯¼ö Ã³¸®
	// (¸ô¿¡¼­ ¸¸µå´Â ¾ÆÀÌÅÛÀº item_proto¿¡ Á¤ÀÇµÈ´ë·Î ¼Ó¼ºÀÌ ºÙ±â ¶§¹®¿¡,
	//  ¿ëÈ¥¼®ÀÇ °æ¿ì, ÀÌ Ã³¸®¸¦ ÇÏÁö ¾ÊÀ¸¸é ¼Ó¼ºÀÌ ÇÏ³ªµµ ºÙÁö ¾Ê°Ô µÈ´Ù.)
	if (pkItem->IsDragonSoul())
	{
		if (bMall)
		{
			DSManager::instance().DragonSoulItemInitialize(pkItem);
		}

		if (DRAGON_SOUL_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}
		
		TItemPos DestPos = p->ItemPos;
		if (!DSManager::instance().IsValidCellForThisItem(pkItem, DestPos))
		{
			int iCell = ch->GetEmptyDragonSoulInventory(pkItem);
			if (iCell < 0)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
				return ;
			}
			DestPos = TItemPos (DRAGON_SOUL_INVENTORY, iCell);
		}

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
#ifdef ENABLE_SPECIAL_STORAGE
	else if (pkItem->IsUpgradeItem())
	{
		if (UPGRADE_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptyUpgradeInventory(pkItem);
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return ;
		}
		DestPos = TItemPos (UPGRADE_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
	else if (pkItem->IsStone())
	{
		if (STONE_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptyStoneInventory(pkItem);
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return ;
		}
		DestPos = TItemPos (STONE_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
	else if (pkItem->IsChest())
	{
		if (CHEST_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ì°½ê?> ?®ê¸??˜ ?†?„ ?„ì¹˜?…?ˆ??"));
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptyChestInventory(pkItem);
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ì°½ê?> ?®ê¸??˜ ?†?„ ?„ì¹˜?…?ˆ??"));
			return ;
		}
		DestPos = TItemPos (CHEST_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
	else if (pkItem->IsAttr())
	{
		if (ATTR_INVENTORY != p->ItemPos.window_type)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ì°½ê?> ?®ê¸??˜ ?†?„ ?„ì¹˜?…?ˆ??"));
			return;
		}

		TItemPos DestPos = p->ItemPos;

		int iCell = ch->GetEmptyAttrInventory(pkItem);
		if (iCell < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ì°½ê?> ?®ê¸??˜ ?†?„ ?„ì¹˜?…?ˆ??"));
			return ;
		}
		DestPos = TItemPos (ATTR_INVENTORY, iCell);

		pkSafebox->Remove(p->bSafePos);
		pkItem->AddToCharacter(ch, DestPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}
#endif
	else
	{
#ifdef ENABLE_SPECIAL_STORAGE
		if (DRAGON_SOUL_INVENTORY == p->ItemPos.window_type ||
			UPGRADE_INVENTORY == p->ItemPos.window_type ||
			STONE_INVENTORY == p->ItemPos.window_type ||
			CHEST_INVENTORY == p->ItemPos.window_type ||
			ATTR_INVENTORY == p->ItemPos.window_type)
#else
		if (DRAGON_SOUL_INVENTORY == p->ItemPos.window_type)
#endif
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ¿Å±æ ¼ö ¾ø´Â À§Ä¡ÀÔ´Ï´Ù."));
			return;
		}

		pkSafebox->Remove(p->bSafePos);
		if (bMall)
		{
			if (NULL == pkItem->GetProto())
			{
				sys_err ("pkItem->GetProto() == NULL (id : %d)",pkItem->GetID());
				return ;
			}
			// 100% È®·ü·Î ¼Ó¼ºÀÌ ºÙ¾î¾ß ÇÏ´Âµ¥ ¾È ºÙ¾îÀÖ´Ù¸é »õ·Î ºÙÈù´Ù. ...............
			if (100 == pkItem->GetProto()->bAlterToMagicItemPct && 0 == pkItem->GetAttributeCount())
			{
				pkItem->AlterToMagicItem();
			}
		}
		pkItem->AddToCharacter(ch, p->ItemPos);
		ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
	}

	DWORD dwID = pkItem->GetID();
	db_clientdesc->DBPacketHeader(HEADER_GD_ITEM_FLUSH, 0, sizeof(DWORD));
	db_clientdesc->Packet(&dwID, sizeof(DWORD));

	char szHint[128];
	snprintf(szHint, sizeof(szHint), "%s %u", pkItem->GetName(), pkItem->GetCount());
	if (bMall)
		LogManager::instance().ItemLog(ch, pkItem, "MALL GET", szHint);
	else
		LogManager::instance().ItemLog(ch, pkItem, "SAFEBOX GET", szHint);
}

void CInputMain::SafeboxItemMove(LPCHARACTER ch, const char * data)
{
	struct command_item_move * pinfo = (struct command_item_move *) data;

	if (!ch->CanHandleItem())
		return;

	if (!ch->GetSafebox())
		return;

	ch->GetSafebox()->MoveItem(pinfo->Cell.cell, pinfo->CellTo.cell, pinfo->count);
}

// PARTY_JOIN_BUG_FIX
void CInputMain::PartyInvite(LPCHARACTER ch, const char * c_pData)
{
	if (!ch)
		return;

	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	TPacketCGPartyInvite * p = (TPacketCGPartyInvite*) c_pData;

	LPCHARACTER pInvitee = CHARACTER_MANAGER::instance().Find(p->vid);

	if (!pInvitee || !ch->GetDesc() || !pInvitee->GetDesc() || !pInvitee->IsPC() || !ch->IsPC())
	{
		sys_err("PARTY Cannot find invited character");
		return;
	}

#if defined(ENABLE_MESSENGER_BLOCK)
	if (MessengerManager::instance().CheckMessengerList(ch->GetName(), pInvitee->GetName(), MESSENGER_BLOCK))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Unblock %s to continue."), pInvitee->GetName());
		return;
	}
	else if (MessengerManager::instance().CheckMessengerList(pInvitee->GetName(), ch->GetName(), MESSENGER_BLOCK))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s has blocked you."), pInvitee->GetName());
		return;
	}
#endif

	ch->PartyInvite(pInvitee);
}

void CInputMain::PartyInviteAnswer(LPCHARACTER ch, const char * c_pData)
{
	if (!ch)
		return;

	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	TPacketCGPartyInviteAnswer * p = (TPacketCGPartyInviteAnswer*) c_pData;

	LPCHARACTER pInviter = CHARACTER_MANAGER::instance().Find(p->leader_vid);

	// pInviter °¡ ch ¿¡°Ô ÆÄÆ¼ ¿äÃ»À» Çß¾ú´Ù.

	if (!pInviter || !pInviter->IsPC())
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ÆÄÆ¼¿äÃ»À» ÇÑ Ä³¸¯ÅÍ¸¦ Ã£À»¼ö ¾ø½À´Ï´Ù."));
	else if (!p->accept)
		pInviter->PartyInviteDeny(ch->GetPlayerID());
	else
		pInviter->PartyInviteAccept(ch);
}
// END_OF_PARTY_JOIN_BUG_FIX

void CInputMain::PartySetState(LPCHARACTER ch, const char* c_pData)
{
	if (!CPartyManager::instance().IsEnablePCParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ¼­¹ö ¹®Á¦·Î ÆÄÆ¼ °ü·Ã Ã³¸®¸¦ ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	TPacketCGPartySetState* p = (TPacketCGPartySetState*) c_pData;

	if (!ch->GetParty())
		return;

	if (ch->GetParty()->GetLeaderPID() != ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ¸®´õ¸¸ º¯°æÇÒ ¼ö ÀÖ½À´Ï´Ù."));
		return;
	}

	if (!ch->GetParty()->IsMember(p->pid))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> »óÅÂ¸¦ º¯°æÇÏ·Á´Â »ç¶÷ÀÌ ÆÄÆ¼¿øÀÌ ¾Æ´Õ´Ï´Ù."));
		return;
	}

	DWORD pid = p->pid;
	sys_log(0, "PARTY SetRole pid %d to role %d state %s", pid, p->byRole, p->flag ? "on" : "off");

	switch (p->byRole)
	{
		case PARTY_ROLE_NORMAL:
			break;

		case PARTY_ROLE_ATTACKER: 
		case PARTY_ROLE_TANKER: 
		case PARTY_ROLE_BUFFER:
		case PARTY_ROLE_SKILL_MASTER:
		case PARTY_ROLE_HASTE:
		case PARTY_ROLE_DEFENDER:
			if (ch->GetParty()->SetRole(pid, p->byRole, p->flag))
			{
				TPacketPartyStateChange pack;
				pack.dwLeaderPID = ch->GetPlayerID();
				pack.dwPID = p->pid;
				pack.bRole = p->byRole;
				pack.bFlag = p->flag;
				db_clientdesc->DBPacket(HEADER_GD_PARTY_STATE_CHANGE, 0, &pack, sizeof(pack));
			}
			/* else
			   ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ¾îÅÂÄ¿ ¼³Á¤¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù.")); */
			break;

		default:
			sys_err("wrong byRole in PartySetState Packet name %s state %d", ch->GetName(), p->byRole);
			break;
	}
}

void CInputMain::PartyRemove(LPCHARACTER ch, const char* c_pData)
{
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (!CPartyManager::instance().IsEnablePCParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ¼­¹ö ¹®Á¦·Î ÆÄÆ¼ °ü·Ã Ã³¸®¸¦ ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (ch->GetDungeon())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ´øÀü ¾È¿¡¼­´Â ÆÄÆ¼¿¡¼­ Ãß¹æÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	TPacketCGPartyRemove* p = (TPacketCGPartyRemove*) c_pData;

	if (!ch->GetParty())
		return;

	LPPARTY pParty = ch->GetParty();
	if (pParty->GetLeaderPID() == ch->GetPlayerID())
	{
		if (ch->GetDungeon())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ´øÁ¯³»¿¡¼­´Â ÆÄÆ¼¿øÀ» Ãß¹æÇÒ ¼ö ¾ø½À´Ï´Ù."));
		}
		else
		{
			// leader can remove any member
			if (p->pid == ch->GetPlayerID() || pParty->GetMemberCount() == 2)
			{
				// party disband
				CPartyManager::instance().DeleteParty(pParty);
			}
			else
			{
				LPCHARACTER B = CHARACTER_MANAGER::instance().FindByPID(p->pid);
				if (B)
				{
					//pParty->SendPartyRemoveOneToAll(B);
					B->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ÆÄÆ¼¿¡¼­ Ãß¹æ´çÇÏ¼Ì½À´Ï´Ù."));
					//pParty->Unlink(B);
					//CPartyManager::instance().SetPartyMember(B->GetPlayerID(), NULL);
				}
				pParty->Quit(p->pid);
			}
		}
	}
	else
	{
		// otherwise, only remove itself
		if (p->pid == ch->GetPlayerID())
		{
			if (ch->GetDungeon())
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ´øÁ¯³»¿¡¼­´Â ÆÄÆ¼¸¦ ³ª°¥ ¼ö ¾ø½À´Ï´Ù."));
			}
			else
			{
				if (pParty->GetMemberCount() == 2)
				{
					// party disband
					CPartyManager::instance().DeleteParty(pParty);
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ÆÄÆ¼¿¡¼­ ³ª°¡¼Ì½À´Ï´Ù."));
					//pParty->SendPartyRemoveOneToAll(ch);
					pParty->Quit(ch->GetPlayerID());
					//pParty->SendPartyRemoveAllToOne(ch);
					//CPartyManager::instance().SetPartyMember(ch->GetPlayerID(), NULL);
				}
			}
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ´Ù¸¥ ÆÄÆ¼¿øÀ» Å»Åð½ÃÅ³ ¼ö ¾ø½À´Ï´Ù."));
		}
	}
}

void CInputMain::AnswerMakeGuild(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGAnswerMakeGuild* p = (TPacketCGAnswerMakeGuild*) c_pData;
	if (!ch) { return; }

#ifdef YANG_LIMIT
	const int neededGold = 200000;
	if (ch->GetGold() < static_cast<ULDWORD>(neededGold)) { return; }
#else
	if (ch->GetGold() < 200000){ return; }
#endif

	if (get_global_time() - ch->GetQuestFlag("guild_manage.new_disband_time") <
			CGuildManager::instance().GetDisbandDelay())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ÇØ»êÇÑ ÈÄ %dÀÏ ÀÌ³»¿¡´Â ±æµå¸¦ ¸¸µé ¼ö ¾ø½À´Ï´Ù."), 
				quest::CQuestManager::instance().GetEventFlag("guild_disband_delay"));
		return;
	}

	if (get_global_time() - ch->GetQuestFlag("guild_manage.new_withdraw_time") <
			CGuildManager::instance().GetWithdrawDelay())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Å»ÅðÇÑ ÈÄ %dÀÏ ÀÌ³»¿¡´Â ±æµå¸¦ ¸¸µé ¼ö ¾ø½À´Ï´Ù."), 
				quest::CQuestManager::instance().GetEventFlag("guild_withdraw_delay"));
		return;
	}

	if (ch->GetGuild())
		return;

	CGuildManager& gm = CGuildManager::instance();

	TGuildCreateParameter cp;
	memset(&cp, 0, sizeof(cp));

	cp.master = ch;
	strlcpy(cp.name, p->guild_name, sizeof(cp.name));

	if (cp.name[0] == 0 || !check_name(cp.name))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀûÇÕÇÏÁö ¾ÊÀº ±æµå ÀÌ¸§ ÀÔ´Ï´Ù."));
		return;
	}

	DWORD dwGuildID = gm.CreateGuild(cp);

	if (dwGuildID)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> [%s] ±æµå°¡ »ý¼ºµÇ¾ú½À´Ï´Ù."), cp.name);

		int GuildCreateFee;

		if (LC_IsBrazil())
		{
			GuildCreateFee = 500000;
		}
		else
		{
			GuildCreateFee = 200000;
		}

#ifdef YANG_LIMIT
		ch->GoldChange(static_cast<LDWORD>(-GuildCreateFee), "GUILD_FEE");
#else
		ch->PointChange(POINT_GOLD, -GuildCreateFee);
#endif
		DBManager::instance().SendMoneyLog(MONEY_LOG_GUILD, ch->GetPlayerID(), -GuildCreateFee);

		char Log[128];
		snprintf(Log, sizeof(Log), "GUILD_NAME %s MASTER %s", cp.name, ch->GetName());
		LogManager::instance().CharLog(ch, 0, "MAKE_GUILD", Log);

		if (g_iUseLocale)
			ch->RemoveSpecifyItem(GUILD_CREATE_ITEM_VNUM, 1);
		//ch->SendGuildName(dwGuildID);
	}
	else
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå »ý¼º¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù."));
}

void CInputMain::PartyUseSkill(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGPartyUseSkill* p = (TPacketCGPartyUseSkill*) c_pData; 
	if (!ch->GetParty())
		return;

	if (ch->GetPlayerID() != ch->GetParty()->GetLeaderPID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ÆÄÆ¼ ±â¼úÀº ÆÄÆ¼Àå¸¸ »ç¿ëÇÒ ¼ö ÀÖ½À´Ï´Ù."));
		return;
	}

	switch (p->bySkillIndex)
	{
		case PARTY_SKILL_HEAL:
			ch->GetParty()->HealParty();
			break;

		case PARTY_SKILL_WARP:
		{
			LPCHARACTER pch = CHARACTER_MANAGER::instance().Find(p->vid);
			if (pch)
			{
				if (pch->IsPC())
					ch->GetParty()->SummonToLeader(pch->GetPlayerID());
				else
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ¼ÒÈ¯ÇÏ·Á´Â ´ë»óÀ» Ã£À» ¼ö ¾ø½À´Ï´Ù."));
			}
		}
		break;
	}
}

void CInputMain::PartyParameter(LPCHARACTER ch, const char * c_pData)
{
	TPacketCGPartyParameter * p = (TPacketCGPartyParameter *) c_pData;

	if (ch->GetParty())
		ch->GetParty()->SetParameter(p->bDistributeMode);
}

size_t GetSubPacketSize(const GUILD_SUBHEADER_CG& header)
{
	switch (header)
	{
		case GUILD_SUBHEADER_CG_DEPOSIT_MONEY:				return sizeof(int);
		case GUILD_SUBHEADER_CG_WITHDRAW_MONEY:				return sizeof(int);
		case GUILD_SUBHEADER_CG_ADD_MEMBER:					return sizeof(DWORD);
		case GUILD_SUBHEADER_CG_REMOVE_MEMBER:				return sizeof(DWORD);
		case GUILD_SUBHEADER_CG_CHANGE_GRADE_NAME:			return 10;
		case GUILD_SUBHEADER_CG_CHANGE_GRADE_AUTHORITY:		return sizeof(BYTE) + sizeof(BYTE);
		case GUILD_SUBHEADER_CG_OFFER:						return sizeof(DWORD);
		case GUILD_SUBHEADER_CG_CHARGE_GSP:					return sizeof(int);
		case GUILD_SUBHEADER_CG_POST_COMMENT:				return 1;
		case GUILD_SUBHEADER_CG_DELETE_COMMENT:				return sizeof(DWORD);
		case GUILD_SUBHEADER_CG_REFRESH_COMMENT:			return 0;
		case GUILD_SUBHEADER_CG_CHANGE_MEMBER_GRADE:		return sizeof(DWORD) + sizeof(BYTE);
		case GUILD_SUBHEADER_CG_USE_SKILL:					return sizeof(TPacketCGGuildUseSkill);
		case GUILD_SUBHEADER_CG_CHANGE_MEMBER_GENERAL:		return sizeof(DWORD) + sizeof(BYTE);
		case GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER:		return sizeof(DWORD) + sizeof(BYTE);
	}

	return 0;
}

int CInputMain::Guild(LPCHARACTER ch, const char * data, size_t uiBytes)
{
	if (uiBytes < sizeof(TPacketCGGuild))
		return -1;

	const TPacketCGGuild* p = reinterpret_cast<const TPacketCGGuild*>(data);
	const char* c_pData = data + sizeof(TPacketCGGuild);

	uiBytes -= sizeof(TPacketCGGuild);

	const GUILD_SUBHEADER_CG SubHeader = static_cast<GUILD_SUBHEADER_CG>(p->subheader);
	const size_t SubPacketLen = GetSubPacketSize(SubHeader);

	if (uiBytes < SubPacketLen)
	{
		return -1;
	}

	CGuild* pGuild = ch->GetGuild();

	if (NULL == pGuild)
	{
		if (SubHeader != GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå¿¡ ¼ÓÇØÀÖÁö ¾Ê½À´Ï´Ù."));
			return SubPacketLen;
		}
	}

	switch (SubHeader)
	{
		case GUILD_SUBHEADER_CG_DEPOSIT_MONEY:
			{
				// by mhh : ±æµåÀÚ±ÝÀº ´çºÐ°£ ³ÖÀ» ¼ö ¾ø´Ù.
				return SubPacketLen;

				const int gold = MIN(*reinterpret_cast<const int*>(c_pData), __deposit_limit());

				if (gold < 0)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Àß¸øµÈ ±Ý¾×ÀÔ´Ï´Ù."));
					return SubPacketLen;
				}

				if (ch->GetGold() < gold)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> °¡Áö°í ÀÖ´Â µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù."));
					return SubPacketLen;
				}

				pGuild->RequestDepositMoney(ch, gold);
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_WITHDRAW_MONEY:
			{
				// by mhh : ±æµåÀÚ±ÝÀº ´çºÐ°£ »¬ ¼ö ¾ø´Ù.
				return SubPacketLen;

				const int gold = MIN(*reinterpret_cast<const int*>(c_pData), 500000);

				if (gold < 0)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Àß¸øµÈ ±Ý¾×ÀÔ´Ï´Ù."));
					return SubPacketLen;
				}

				pGuild->RequestWithdrawMoney(ch, gold);
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_ADD_MEMBER:
			{
				const DWORD vid = *reinterpret_cast<const DWORD*>(c_pData);
				LPCHARACTER newmember = CHARACTER_MANAGER::instance().Find(vid);

				if (!newmember)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±×·¯ÇÑ »ç¶÷À» Ã£À» ¼ö ¾ø½À´Ï´Ù."));
					return SubPacketLen;
				}

				if (!newmember->IsPC())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This BUG it's already fixed."));
					return SubPacketLen;
				}
				if (!ch->IsPC())
					return SubPacketLen;

				if (LC_IsCanada() == true)
				{
					if (newmember->GetQuestFlag("change_guild_master.be_other_member") > get_global_time())
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ¾ÆÁ÷ °¡ÀÔÇÒ ¼ö ¾ø´Â Ä³¸¯ÅÍÀÔ´Ï´Ù"));
						return SubPacketLen;
					}
				}

#if defined(ENABLE_MESSENGER_BLOCK)
				if (MessengerManager::instance().CheckMessengerList(ch->GetName(), newmember->GetName(), MESSENGER_BLOCK))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Unblock %s to continue."), newmember->GetName());
					return SubPacketLen;
				}
				else if (MessengerManager::instance().CheckMessengerList(newmember->GetName(), ch->GetName(), MESSENGER_BLOCK))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s has blocked you."), newmember->GetName());
					return SubPacketLen;
				}
#endif

				pGuild->Invite(ch, newmember);
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_REMOVE_MEMBER:
			{
				if (pGuild->UnderAnyWar() != 0)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀü Áß¿¡´Â ±æµå¿øÀ» Å»Åð½ÃÅ³ ¼ö ¾ø½À´Ï´Ù."));
					return SubPacketLen;
				}

				const DWORD pid = *reinterpret_cast<const DWORD*>(c_pData);
				const TGuildMember* m = pGuild->GetMember(ch->GetPlayerID());

				if (NULL == m)
					return -1;

				LPCHARACTER member = CHARACTER_MANAGER::instance().FindByPID(pid);

				if (member)
				{
					if (member->GetGuild() != pGuild)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æÀÌ °°Àº ±æµå°¡ ¾Æ´Õ´Ï´Ù."));
						return SubPacketLen;
					}

					if (!pGuild->HasGradeAuth(m->grade, GUILD_AUTH_REMOVE_MEMBER))
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå¿øÀ» °­Á¦ Å»Åð ½ÃÅ³ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
						return SubPacketLen;
					}

					member->SetQuestFlag("guild_manage.new_withdraw_time", get_global_time());
					pGuild->RequestRemoveMember(member->GetPlayerID());

					if (LC_IsBrazil() == true)
					{
						DBManager::instance().Query("REPLACE INTO guild_invite_limit VALUES(%d, %d)", pGuild->GetID(), get_global_time());
					}
				}
				else
				{
					if (!pGuild->HasGradeAuth(m->grade, GUILD_AUTH_REMOVE_MEMBER))
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå¿øÀ» °­Á¦ Å»Åð ½ÃÅ³ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
						return SubPacketLen;
					}

					if (pGuild->RequestRemoveMember(pid))
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå¿øÀ» °­Á¦ Å»Åð ½ÃÄ×½À´Ï´Ù."));
					else
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±×·¯ÇÑ »ç¶÷À» Ã£À» ¼ö ¾ø½À´Ï´Ù."));
				}
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_CHANGE_GRADE_NAME:
			{
				char gradename[GUILD_GRADE_NAME_MAX_LEN + 1];
				strlcpy(gradename, c_pData + 1, sizeof(gradename));

				const TGuildMember * m = pGuild->GetMember(ch->GetPlayerID());

				if (NULL == m)
					return -1;

				if (m->grade != GUILD_LEADER_GRADE)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Á÷À§ ÀÌ¸§À» º¯°æÇÒ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
				}
				else if (*c_pData == GUILD_LEADER_GRADE)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀåÀÇ Á÷À§ ÀÌ¸§Àº º¯°æÇÒ ¼ö ¾ø½À´Ï´Ù."));
				}
				else if (!check_name(gradename))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ÀûÇÕÇÏÁö ¾ÊÀº Á÷À§ ÀÌ¸§ ÀÔ´Ï´Ù."));
				}
				else
				{
					pGuild->ChangeGradeName(*c_pData, gradename);
				}
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_CHANGE_GRADE_AUTHORITY:
			{
				const TGuildMember* m = pGuild->GetMember(ch->GetPlayerID());

				if (NULL == m)
					return -1;

				if (m->grade != GUILD_LEADER_GRADE)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Á÷À§ ±ÇÇÑÀ» º¯°æÇÒ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
				}
				else if (*c_pData == GUILD_LEADER_GRADE)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀåÀÇ ±ÇÇÑÀº º¯°æÇÒ ¼ö ¾ø½À´Ï´Ù."));
				}
				else
				{
					pGuild->ChangeGradeAuth(*c_pData, *(c_pData + 1));
				}
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_OFFER:
			{
				DWORD offer = *reinterpret_cast<const DWORD*>(c_pData);

				if (pGuild->GetLevel() >= GUILD_MAX_LEVEL && LC_IsHongKong() == false)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå°¡ ÀÌ¹Ì ÃÖ°í ·¹º§ÀÔ´Ï´Ù."));
				}
				else
				{
					offer /= 100;
					offer *= 100;

					if (pGuild->OfferExp(ch, offer))
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> %uÀÇ °æÇèÄ¡¸¦ ÅõÀÚÇÏ¿´½À´Ï´Ù."), offer);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> °æÇèÄ¡ ÅõÀÚ¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù."));
					}
				}
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_CHARGE_GSP:
			{
				const int offer = *reinterpret_cast<const int*>(c_pData);
				const int gold = offer * 100;

				if (offer < 0 || gold < offer || gold < 0 || ch->GetGold() < gold)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù."));
					return SubPacketLen;
				}

				if (!pGuild->ChargeSP(ch, offer))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ¿ë½Å·Â È¸º¹¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù."));
				}
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_POST_COMMENT:
			{
				const size_t length = *c_pData;

				if (length > GUILD_COMMENT_MAX_LEN)
				{
					// Àß¸øµÈ ±æÀÌ.. ²÷¾îÁÖÀÚ.
					sys_err("POST_COMMENT: %s comment too long (length: %u)", ch->GetName(), length);
					ch->GetDesc()->SetPhase(PHASE_CLOSE);
					return -1;
				}

				if (uiBytes < 1 + length)
					return -1;

				const TGuildMember* m = pGuild->GetMember(ch->GetPlayerID());

				if (NULL == m)
					return -1;

				if (length && !pGuild->HasGradeAuth(m->grade, GUILD_AUTH_NOTICE) && *(c_pData + 1) == '!')
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> °øÁö±ÛÀ» ÀÛ¼ºÇÒ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
				}
				else
				{
					std::string str(c_pData + 1, length);
					pGuild->AddComment(ch, str);
				}

				return (1 + length);
			}

		case GUILD_SUBHEADER_CG_DELETE_COMMENT:
			{
				const DWORD comment_id = *reinterpret_cast<const DWORD*>(c_pData);

				pGuild->DeleteComment(ch, comment_id);
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_REFRESH_COMMENT:
			pGuild->RefreshComment(ch);
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_CHANGE_MEMBER_GRADE:
			{
				const DWORD pid = *reinterpret_cast<const DWORD*>(c_pData);
				const BYTE grade = *(c_pData + sizeof(DWORD));
				const TGuildMember* m = pGuild->GetMember(ch->GetPlayerID());

				if (NULL == m)
					return -1;

				if (m->grade != GUILD_LEADER_GRADE)
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Á÷À§¸¦ º¯°æÇÒ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
				else if (ch->GetPlayerID() == pid)
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀåÀÇ Á÷À§´Â º¯°æÇÒ ¼ö ¾ø½À´Ï´Ù."));
				else if (grade == 1)
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀåÀ¸·Î Á÷À§¸¦ º¯°æÇÒ ¼ö ¾ø½À´Ï´Ù."));
				else
					pGuild->ChangeMemberGrade(pid, grade);
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_USE_SKILL:
			{
				const TPacketCGGuildUseSkill* p = reinterpret_cast<const TPacketCGGuildUseSkill*>(c_pData);

				pGuild->UseSkill(p->dwVnum, ch, p->dwPID);
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_CHANGE_MEMBER_GENERAL:
			{
				const DWORD pid = *reinterpret_cast<const DWORD*>(c_pData);
				const BYTE is_general = *(c_pData + sizeof(DWORD));
				const TGuildMember* m = pGuild->GetMember(ch->GetPlayerID());

				if (NULL == m)
					return -1;

				if (m->grade != GUILD_LEADER_GRADE)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Àå±ºÀ» ÁöÁ¤ÇÒ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
				}
				else
				{
					if (!pGuild->ChangeMemberGeneral(pid, is_general))
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ´õÀÌ»ó Àå¼ö¸¦ ÁöÁ¤ÇÒ ¼ö ¾ø½À´Ï´Ù."));
					}
				}
			}
			return SubPacketLen;

		case GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER:
			{
				const DWORD guild_id = *reinterpret_cast<const DWORD*>(c_pData);
				const BYTE accept = *(c_pData + sizeof(DWORD));

				CGuild * g = CGuildManager::instance().FindGuild(guild_id);

				if (g)
				{
					if (accept)
						g->InviteAccept(ch);
					else
						g->InviteDeny(ch->GetPlayerID());
				}
			}
			return SubPacketLen;

	}

	return 0;
}

void CInputMain::Fishing(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGFishing* p = (TPacketCGFishing*)c_pData;
	ch->SetRotation(p->dir * 5);
	ch->fishing();
	return;
}

void CInputMain::ItemGive(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGGiveItem* p = (TPacketCGGiveItem*) c_pData;
	LPCHARACTER to_ch = CHARACTER_MANAGER::instance().Find(p->dwTargetVID);

	if (to_ch)
		ch->GiveItem(to_ch, p->ItemPos);
	else
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÀÌÅÛÀ» °Ç³×ÁÙ ¼ö ¾ø½À´Ï´Ù."));
}

void CInputMain::Hack(LPCHARACTER ch, const char * c_pData)
{
	TPacketCGHack * p = (TPacketCGHack *) c_pData;
	
	char buf[sizeof(p->szBuf)];
	strlcpy(buf, p->szBuf, sizeof(buf));

	sys_err("HACK_DETECT: %s %s", ch->GetName(), buf);

	// ÇöÀç Å¬¶óÀÌ¾ðÆ®¿¡¼­ ÀÌ ÆÐÅ¶À» º¸³»´Â °æ¿ì°¡ ¾øÀ¸¹Ç·Î ¹«Á¶°Ç ²÷µµ·Ï ÇÑ´Ù
	ch->GetDesc()->SetPhase(PHASE_CLOSE);
}

#ifdef ENABLE_SHOW_CHEST_DROP
void CInputMain::ChestDropInfo(LPCHARACTER ch, const char* c_pData)
{
	
	if (!ch)
		return;
	
	
	TPacketCGChestDropInfo* p = (TPacketCGChestDropInfo*) c_pData;

	if(p->wInventoryCell >= INVENTORY_MAX_NUM)
	{
		return;
	}
	
	// LPITEM pkItem = ch->GetInventoryItem(p->wInventoryCell);
	LPITEM pkItem = NULL;
	if (p->wInventoryType == 1)
	{
		pkItem = ch->GetInventoryItem(p->wInventoryCell);
	}
	else if (p->wInventoryType == 6)
	{
		pkItem = ch->GetUpgradeInventoryItem(p->wInventoryCell);
	}
	else if (p->wInventoryType == 7)
	{
		pkItem = ch->GetStoneInventoryItem(p->wInventoryCell);
	}
	else if (p->wInventoryType == 8)
	{
		pkItem = ch->GetChestInventoryItem(p->wInventoryCell);
	}
	else if (p->wInventoryType == 9)
	{
		pkItem = ch->GetAttrInventoryItem(p->wInventoryCell);
	}
	if (!pkItem)
	{
		return;
	}
	
	std::vector<TChestDropInfoTable> vec_ItemList;
	ITEM_MANAGER::instance().GetChestItemList(pkItem->GetVnum(), vec_ItemList);

	TPacketGCChestDropInfo packet;
	packet.bHeader = HEADER_GC_CHEST_DROP_INFO;
	packet.wSize = sizeof(packet) + sizeof(TChestDropInfoTable) * vec_ItemList.size();
	packet.dwChestVnum = pkItem->GetVnum();

	ch->GetDesc()->BufferedPacket(&packet, sizeof(packet));
	ch->GetDesc()->Packet(&vec_ItemList[0], sizeof(TChestDropInfoTable) * vec_ItemList.size());
}
#endif
int CInputMain::MyShop(LPCHARACTER ch, const char * c_pData, size_t uiBytes)
{
	TPacketCGMyShop * p = (TPacketCGMyShop *) c_pData;
	int iExtraLen = p->bCount * sizeof(TShopItemTable);

	 if (strstr(p->szSign, ("|c")) || strstr(p->szSign, ("|C"))){
		LogManager::instance().HackLog("COLORFUL_SHOP", ch);
		return (iExtraLen);
	}
	if (uiBytes < sizeof(TPacketCGMyShop) + iExtraLen)
		return -1;

	if (ch->GetGold() >= GOLD_MAX)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÀ¯ µ·ÀÌ 20¾ï³ÉÀ» ³Ñ¾î °Å·¡¸¦ ÇÛ¼ö°¡ ¾ø½À´Ï´Ù."));
		sys_log(0, "MyShop ==> OverFlow Gold id %u name %s ", ch->GetPlayerID(), ch->GetName());
		return (iExtraLen);
	}

#ifdef ENABLE_CHEQUE_SYSTEM
	if (ch->GetCheque() >= CHEQUE_MAX)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("?? ?? 20??? ?? ??? ??? ????."));
		sys_log(0, "MyShop ==> OverFlow Cheque id %u name %s ", ch->GetPlayerID(), ch->GetName());
		return (iExtraLen);
	}
#endif
	if (ch->IsStun() || ch->IsDead())
		return (iExtraLen);

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (ch->GetExchange() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡ÁßÀÏ°æ¿ì °³ÀÎ»óÁ¡À» ¿­¼ö°¡ ¾ø½À´Ï´Ù."));
		return (iExtraLen);
	}
#else
	if (ch->GetExchange() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->IsCubeOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡ÁßÀÏ°æ¿ì °³ÀÎ»óÁ¡À» ¿­¼ö°¡ ¾ø½À´Ï´Ù."));
		return (iExtraLen);
	}
#endif

	sys_log(0, "MyShop count %d", p->bCount);
	ch->OpenMyShop(p->szSign, (TShopItemTable *) (c_pData + sizeof(TPacketCGMyShop)), p->bCount);
	return (iExtraLen);
}

int CInputMain::MyOfflineShop(LPCHARACTER ch, const char * c_pData, size_t uiBytes)
{
	TPacketCGMyOfflineShop * p = (TPacketCGMyOfflineShop *)c_pData;
#ifdef ENABLE_MAXIMUM_YANG_FOR_OFFLINE_SHOP
	int iExtraLen = p->bCount * sizeof(TOfflineShopItemTable);
#else
	int iExtraLen = p->bCount * sizeof(TShopItemTable);
#endif

	if (uiBytes < sizeof(TPacketCGMyOfflineShop) + iExtraLen)
		return -1;

	if (ch->IsStun() || ch->IsDead())
		return (iExtraLen);

	if (ch->GetExchange() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡ÁßÀÏ°æ¿ì °³ÀÎ»óÁ¡À» ¿­¼ö°¡ ¾ø½À´Ï´Ù."));
		return (iExtraLen);
	}

	sys_log(0, "MyOfflineShop count %d", p->bCount);
#ifdef ENABLE_MAXIMUM_YANG_FOR_OFFLINE_SHOP
	ch->OpenMyOfflineShop(p->szSign, (TOfflineShopItemTable *)(c_pData + sizeof(TPacketCGMyOfflineShop)), p->bCount, p->bTime);
#else
	ch->OpenMyOfflineShop(p->szSign, (TShopItemTable *)(c_pData + sizeof(TPacketCGMyOfflineShop)), p->bCount, p->bTime);
#endif
	return (iExtraLen);
}


#ifdef ENABLE_PM_ALL_SEND_SYSTEM
void CInputMain::BulkWhisperManager(LPCHARACTER ch, const char* c_pData)
{
	TPacketCGBulkWhisper* f = (TPacketCGBulkWhisper*)c_pData;

	if (!ch)
		return;

	if (ch->GetGMLevel() != GM_IMPLEMENTOR)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "[SYSTEM] GM degilsin.");
		return;
	}

	TPacketGGBulkWhisper p;
	p.bHeader = HEADER_GG_BULK_WHISPER;
	p.lSize = strlen(f->szText) + 1;

	TEMP_BUFFER tmpbuf;
	tmpbuf.write(&p, sizeof(p));
	tmpbuf.write(f->szText, p.lSize);

	char szEscaped[CHAT_MAX_LEN * 2 + 1];
	DBManager::instance().EscapeString(szEscaped, sizeof(szEscaped), f->szText, strlen(f->szText));
	SendBulkWhisper(f->szText);
	P2P_MANAGER::instance().Send(tmpbuf.read_peek(), tmpbuf.size());
}
#endif


void CInputMain::Refine(LPCHARACTER ch, const char* c_pData)
{
	const TPacketCGRefine* p = reinterpret_cast<const TPacketCGRefine*>(c_pData);

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (ch->GetExchange() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->GetMyShop() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	{
		ch->ChatPacket(CHAT_TYPE_INFO,  LC_TEXT("Ã¢°í,°Å·¡Ã¢µîÀÌ ¿­¸° »óÅÂ¿¡¼­´Â °³·®À» ÇÒ¼ö°¡ ¾ø½À´Ï´Ù"));
		ch->ClearRefineMode();
		return;
	}
#else
	if (ch->GetExchange() || ch->IsOpenSafebox() || ch->GetShopOwner() || ch->GetMyShop() || ch->IsCubeOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ã¢°í,°Å·¡Ã¢µîÀÌ ¿­¸° »óÅÂ¿¡¼­´Â °³·®À» ÇÒ¼ö°¡ ¾ø½À´Ï´Ù"));
		ch->ClearRefineMode();
		return;
	}
#endif

	if (p->type == 255)
	{
		// DoRefine Cancel
		ch->ClearRefineMode();
		return;
	}

	if (p->pos >= INVENTORY_MAX_NUM)
	{
		ch->ClearRefineMode();
		return;
	}
#ifdef WJ_SECURITY_SYSTEM
	if (ch->IsActivateSecurity() == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "G?enlik sistemi aktifken bu iþlemi ger?kleþtiremezsin.");
		ch->ClearRefineMode();
		return;
	}
#endif

	LPITEM item = ch->GetInventoryItem(p->pos);

	if (!item)
	{
		ch->ClearRefineMode();
		return;
	}

	ch->SetRefineTime();

	if (p->type == REFINE_TYPE_NORMAL)
	{
		sys_log (0, "refine_type_noraml");
		ch->DoRefine(item);
	}
	else if (p->type == REFINE_TYPE_SCROLL || p->type == REFINE_TYPE_HYUNIRON || p->type == REFINE_TYPE_MUSIN || p->type == REFINE_TYPE_BDRAGON || p->type == REFINE_TYPE_RITUALS_SCROLL)
	{
		sys_log (0, "refine_type_scroll, ...");
		ch->DoRefineWithScroll(item);
	}
	else if (p->type == REFINE_TYPE_MONEY_ONLY)
	{
		const LPITEM item = ch->GetInventoryItem(p->pos);

		if (NULL != item)
		{
			if (500 <= item->GetRefineSet())
			{
				LogManager::instance().HackLog("DEVIL_TOWER_REFINE_HACK", ch);
			}
			else
			{
				if (ch->GetQuestFlag("deviltower_zone.can_refine"))
				{
					ch->DoRefine(item, true);
					ch->SetQuestFlag("deviltower_zone.can_refine", 0);
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "»ç±Í Å¸¿ö ¿Ï·á º¸»óÀº ÇÑ¹ø±îÁö »ç¿ë°¡´ÉÇÕ´Ï´Ù.");
				}
			}
		}
	}

	ch->ClearRefineMode();
}

#ifdef __SASH_SYSTEM__
void CInputMain::Sash(LPCHARACTER pkChar, const char* c_pData)
{
	quest::PC * pPC = quest::CQuestManager::instance().GetPCForce(pkChar->GetPlayerID());
	if (pPC->IsRunning())
		return;
	
	TPacketSash * sPacket = (TPacketSash*) c_pData;
	switch (sPacket->subheader)
	{
		case SASH_SUBHEADER_CG_CLOSE:
			{
				pkChar->CloseSash();
			}
			break;
		case SASH_SUBHEADER_CG_ADD:
			{
				pkChar->AddSashMaterial(sPacket->tPos, sPacket->bPos);
			}
			break;
		case SASH_SUBHEADER_CG_REMOVE:
			{
				pkChar->RemoveSashMaterial(sPacket->bPos);
			}
			break;
		case SASH_SUBHEADER_CG_REFINE:
			{
				pkChar->RefineSashMaterials();
			}
			break;
		default:
			break;
	}
}
#endif
#ifdef __ENABLE_NEW_OFFLINESHOP__
#include "new_offlineshop.h"
#include "new_offlineshop_manager.h"
template <class T>
bool CanDecode(T* p, int buffleft) {
	return buffleft >= (int)sizeof(T);
}

template <class T>
const char* Decode(T*& pObj, const char* data, int* pbufferLeng = nullptr, int* piBufferLeft=nullptr)
{
	pObj = (T*) data;

	if(pbufferLeng)
		*pbufferLeng += sizeof(T);

	if(piBufferLeft)
		*piBufferLeft -= sizeof(T);

	return data + sizeof(T);
}

int OfflineshopPacketCreateNewShop(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGShopCreate* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack, data, &iExtra, &iBufferLeft);

	offlineshop::TShopInfo& rShopInfo = pack->shop;

	//fix flooding
	if (rShopInfo.dwCount > 500 || rShopInfo.dwCount == 0) {
		sys_err("tried to open a shop with 500+ items.");
		return -1;
	}


	std::vector<offlineshop::TShopItemInfo> vec;
	vec.reserve(rShopInfo.dwCount);

	offlineshop::TShopItemInfo* pItem=nullptr;


	for (DWORD i = 0; i < rShopInfo.dwCount; ++i)
	{
		if(!CanDecode(pItem, iBufferLeft))
			return -1;

		data = Decode(pItem, data, &iExtra, &iBufferLeft);
		vec.push_back(*pItem);
	}


	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopCreateNewClientPacket(ch, rShopInfo, vec))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_CREATE_SHOP);

	return iExtra;
}


int OfflineshopPacketChangeShopName(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGShopChangeName* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);

	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopChangeNameClientPacket(ch, pack->szName))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_CHANGE_NAME);

	return iExtra;
}


int OfflineshopPacketForceCloseShop(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopForceCloseClientPacket(ch))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_FORCE_CLOSE);

	return 0;
}


int OfflineshopPacketRequestShopList(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	rManager.RecvShopRequestListClientPacket(ch);
	return 0;
}


int OfflineshopPacketOpenShop(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGShopOpen* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);

	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopOpenClientPacket(ch,pack->dwOwnerID))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_OPEN_SHOP);

	return iExtra;
}


int OfflineshopPacketOpenShowOwner(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopOpenMyShopClientPacket(ch))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_OPEN_SHOP_OWNER);

	return 0;
}


int OfflineshopPacketBuyItem(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGShopBuyItem* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);

	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopBuyItemClientPacket(ch , pack->dwOwnerID, pack->dwItemID, pack->bIsSearch))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_BUY_ITEM);
		//offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_OPEN_SHOP);

	return iExtra;
}


int OfflineshopPacketAddItem(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGAddItem* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);
	
	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopAddItemClientPacket(ch, pack->pos, pack->price))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_ADD_ITEM);

	return iExtra;
}


int OfflineshopPacketRemoveItem(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGRemoveItem* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);

	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopRemoveItemClientPacket(ch, pack->dwItemID))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_REMOVE_ITEM);

	return iExtra;
}


int OfflineshopPacketEditItem(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGEditItem* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);

	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopEditItemClientPacket(ch, pack->dwItemID, pack->price))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_EDIT_ITEM);

	return iExtra;
}


int OfflineshopPacketFilterRequest(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGFilterRequest* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);

	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopFilterRequestClientPacket(ch, pack->filter))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_FILTER);

	return iExtra;
}


int OfflineshopPacketCreateOffer(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGOfferCreate* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);

	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopCreateOfferClientPacket(ch, pack->offer))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_CREATE_OFFER);

	return iExtra;
}


int OfflineshopPacketAcceptOffer(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGOfferAccept* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);

	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopAcceptOfferClientPacket(ch, pack->dwOfferID))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_ACCEPT_OFFER);

	return iExtra;
}



int OfflineshopPacketOfferCancel(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGOfferCancel* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);

	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopCancelOfferClientPacket(ch, pack->dwOfferID, pack->dwOwnerID))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_ACCEPT_OFFER);

	return iExtra;
}


int OfflineshopPacketOfferListRequest(LPCHARACTER ch)
{
	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	rManager.RecvOfferListRequestPacket(ch);
	return 0;
}


int OfflineshopPacketOpenSafebox(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopSafeboxOpenClientPacket(ch))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_OPEN_SAFEBOX);

	return 0;
}


int OfflineshopPacketCloseBoard(LPCHARACTER ch)
{
	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	rManager.RecvCloseBoardClientPacket(ch);
	return 0;
}


int OfflineshopPacketGetItemSafebox(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGShopSafeboxGetItem* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);


	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopSafeboxGetItemClientPacket(ch, pack->dwItemID))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_SAFEBOX_GET_ITEM);

	return iExtra;

}


int OfflineshopPacketGetValutesSafebox(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGShopSafeboxGetValutes* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);

	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopSafeboxGetValutesClientPacket(ch, pack->valutes))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_SAFEBOX_GET_VALUTES);

	return iExtra;
}


int OfflineshopPacketCloseSafebox(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvShopSafeboxCloseClientPacket(ch))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_CANNOT_SAFEBOX_CLOSE);

	return 0;
}


int OfflineshopPacketListRequest(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvAuctionListRequestClientPacket(ch))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_AUCTION_CANNOT_SEND_LIST);

	return 0;
}



int OfflineshopPacketOpenAuctionRequest(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGAuctionOpenRequest* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);

	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvAuctionOpenRequestClientPacket(ch, pack->dwOwnerID))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_AUCTION_CANNOT_OPEN_AUCTION);

	return iExtra;
}



int OfflineshopPacketOpenMyAuctionRequest(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvMyAuctionOpenRequestClientPacket(ch))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_AUCTION_CANNOT_SEND_LIST);

	return 0;
}



int OfflineshopPacketCreateAuction(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGAuctionCreate* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);

	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvAuctionCreateClientPacket(ch, pack->dwDuration, pack->init_price, pack->pos))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_AUCTION_CANNOT_CREATE_AUCTION);

	return iExtra;
}



int OfflineshopPacketAddOffer(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGAuctionAddOffer* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);

	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	if(!rManager.RecvAuctionAddOfferClientPacket(ch, pack->dwOwnerID, pack->price))
		offlineshop::SendChatPacket(ch, offlineshop::CHAT_PACKET_AUCTION_CANNOT_ADD_OFFER);

	return iExtra;
}



int OfflineshopPacketExitFromAuction(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGAuctionExitFrom* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack,data, &iExtra, &iBufferLeft);

	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	rManager.RecvAuctionExitFromAuction(ch, pack->dwOwnerID);
	return iExtra;
}


#ifdef ENABLE_NEW_SHOP_IN_CITIES
int OfflineshopPacketClickEntity(LPCHARACTER ch, const char* data, int iBufferLeft)
{
	offlineshop::TSubPacketCGShopClickEntity* pack = nullptr;
	if(!CanDecode(pack, iBufferLeft))
		return -1;

	int iExtra=0;
	data = Decode(pack, data, &iExtra, &iBufferLeft);

	
	offlineshop::CShopManager& rManager = offlineshop::GetManager();
	rManager.RecvShopClickEntity(ch, pack->dwShopVID);
	return iExtra;
}

#endif



int OfflineshopPacket(const char* data , LPCHARACTER ch, long iBufferLeft)
{
	if(iBufferLeft < sizeof(TPacketCGNewOfflineShop))
		return -1;

	TPacketCGNewOfflineShop* pPack=nullptr;
	iBufferLeft -= sizeof(TPacketCGNewOfflineShop);
	data = Decode(pPack, data);

	

	switch (pPack->bSubHeader)
	{
	
	case offlineshop::SUBHEADER_CG_SHOP_CREATE_NEW:				return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketCreateNewShop(ch,data,iBufferLeft);
	case offlineshop::SUBHEADER_CG_SHOP_CHANGE_NAME:			return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketChangeShopName(ch,data,iBufferLeft);
	case offlineshop::SUBHEADER_CG_SHOP_FORCE_CLOSE:			return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketForceCloseShop(ch,data,iBufferLeft);
	case offlineshop::SUBHEADER_CG_SHOP_REQUEST_SHOPLIST:		return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketRequestShopList(ch,data,iBufferLeft);
	case offlineshop::SUBHEADER_CG_SHOP_OPEN:					return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketOpenShop(ch,data,iBufferLeft);
	case offlineshop::SUBHEADER_CG_SHOP_OPEN_OWNER:				return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketOpenShowOwner(ch,data,iBufferLeft);
	
	case offlineshop::SUBHEADER_CG_SHOP_BUY_ITEM:				return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketBuyItem(ch, data , iBufferLeft);
	case offlineshop::SUBHEADER_CG_SHOP_ADD_ITEM:				return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketAddItem(ch,data,iBufferLeft);
	case offlineshop::SUBHEADER_CG_SHOP_REMOVE_ITEM:			return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketRemoveItem(ch,data,iBufferLeft);
	case offlineshop::SUBHEADER_CG_SHOP_EDIT_ITEM:				return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketEditItem(ch,data,iBufferLeft);
	
	case offlineshop::SUBHEADER_CG_SHOP_FILTER_REQUEST:			return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketFilterRequest(ch,data,iBufferLeft);
	
	case offlineshop::SUBHEADER_CG_SHOP_OFFER_CREATE:			return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketCreateOffer(ch,data,iBufferLeft);
	case offlineshop::SUBHEADER_CG_SHOP_OFFER_ACCEPT:			return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketAcceptOffer(ch,data,iBufferLeft);
	case offlineshop::SUBHEADER_CG_SHOP_REQUEST_OFFER_LIST:		return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketOfferListRequest(ch);
	case offlineshop::SUBHEADER_CG_SHOP_OFFER_CANCEL:			return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketOfferCancel(ch, data,iBufferLeft);

	case offlineshop::SUBHEADER_CG_SHOP_SAFEBOX_OPEN:			return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketOpenSafebox(ch,data,iBufferLeft);
	case offlineshop::SUBHEADER_CG_SHOP_SAFEBOX_GET_ITEM:		return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketGetItemSafebox(ch,data,iBufferLeft);
	case offlineshop::SUBHEADER_CG_SHOP_SAFEBOX_GET_VALUTES:	return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketGetValutesSafebox(ch,data,iBufferLeft);
	case offlineshop::SUBHEADER_CG_SHOP_SAFEBOX_CLOSE:			return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketCloseSafebox(ch,data,iBufferLeft);

	case offlineshop::SUBHEADER_CG_AUCTION_LIST_REQUEST:		return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketListRequest(ch, data, iBufferLeft);
	case offlineshop::SUBHEADER_CG_AUCTION_OPEN_REQUEST:		return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketOpenAuctionRequest(ch, data, iBufferLeft);
	case offlineshop::SUBHEADER_CG_MY_AUCTION_OPEN_REQUEST:		return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketOpenMyAuctionRequest(ch, data, iBufferLeft);
	case offlineshop::SUBHEADER_CG_CREATE_AUCTION:				return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketCreateAuction(ch, data, iBufferLeft);
	case offlineshop::SUBHEADER_CG_AUCTION_ADD_OFFER:			return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketAddOffer(ch, data, iBufferLeft);
	case offlineshop::SUBHEADER_CG_EXIT_FROM_AUCTION:			return /*sizeof(TPacketCGNewOfflineShop) +*/ OfflineshopPacketExitFromAuction(ch, data, iBufferLeft);

	case offlineshop::SUBHEADER_CG_CLOSE_BOARD:					return /*sizeof(TPacketCGNewOfflineshop) +*/ OfflineshopPacketCloseBoard(ch);
#ifdef ENABLE_NEW_SHOP_IN_CITIES
	case offlineshop::SUBHEADER_CG_CLICK_ENTITY:				return /*sizeof(TPacketCGNewOfflineshop) +*/ OfflineshopPacketClickEntity(ch, data, iBufferLeft);
#endif


	default:
		sys_err("UNKNOWN SUBHEADER %d ",pPack->bSubHeader);
		return -1;
	}

}
#endif

#define STOP_IF_ITEM_DROPPED_TO_GROUND	//bir nesne yere dustugunde sandik acimini durdur
#define BLOCK_IN_CITIES	//koylerde sandik acmayi engelle
#define MAX_OPEN_COUNT 200	//en fazla 255 olarak ayarlayabilirsiniz
#define PCT_COUNT_MULTIPLIER 8
typedef struct ChestItem{
	DWORD itemCount;
	char szName[ITEM_NAME_MAX_LEN + 1];
} TChestItem;
void CInputMain::InstantChestOpen(LPCHARACTER ch, const char* c_pData) {
	if (!ch || !ch->GetDesc()) { return; }
	TInstantChestOpen * nPacket = (TInstantChestOpen*)c_pData;
		
	LPITEM chestItem = NULL;
	if (chestItem = ch->GetItem(nPacket->iPos))
	{
		const CSpecialItemGroup * pGroup = ITEM_MANAGER::Instance().GetSpecialItemGroup(chestItem->GetVnum());
		if (chestItem->GetType() == ITEM_GIFTBOX || (chestItem->GetVnum() == 27987) || pGroup)
		{
#ifdef BLOCK_IN_CITIES
			if (ch->GetMapIndex() == 41 || ch->GetMapIndex() == 21 || ch->GetMapIndex() == 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("KOYLERDE_SANDIK"));
				return;
			}
#endif
			for (BYTE i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
			{
				long limitValue = chestItem->GetProto()->aLimits[i].lValue;
				switch (chestItem->GetProto()->aLimits[i].bType)
				{
					case LIMIT_LEVEL:
						if (ch->GetLevel() < limitValue)
						{
							ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¨ú¨¡AIAUAC ¡¤©ö¨¬¡× A|CN¨¬¢¬¢¥U ¡¤©ö¨¬¡×AI ©ø¡¤¨öA¢¥I¢¥U."));
							return;
						}
						break;
				}
			}
			std::map<DWORD, TChestItem> map_Items;
			map_Items.clear();
			WORD openCount = chestItem->GetCount() < MAX_OPEN_COUNT ? chestItem->GetCount() : MAX_OPEN_COUNT;
			const WORD pctOpenCount = MAX_OPEN_COUNT / PCT_COUNT_MULTIPLIER;
			if(pGroup){
				if (pGroup->GetOpenType() == CSpecialItemGroup::PCT){ openCount = chestItem->GetCount() < pctOpenCount ? chestItem->GetCount() : pctOpenCount; }
			}
			bool isAddedToMap = chestItem->GetType() == ITEM_GIFTBOX;
			
			for (WORD i = 0; i < openCount; i++) {
				if (isAddedToMap) {
					DWORD dwBoxVnum = chestItem->GetVnum();
					std::vector<DWORD> dwVnums;
					std::vector<DWORD> dwCounts;
					std::vector<LPITEM> item_gets;
					item_gets.clear();
					int count = 0;
					if (ch->GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count)) {
						chestItem->SetCount(chestItem->GetCount() - 1);
						for (int i = 0; i < count; i++) {
							switch (dwVnums[i])
							{
								case CSpecialItemGroup::GOLD:
									ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d_1"), dwCounts[i]);
									break;
								case CSpecialItemGroup::EXP:
									ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("_1"));
									ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d_2"), dwCounts[i]);
									break;
								case CSpecialItemGroup::MOB:
									// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("_2"));
									break;
								case CSpecialItemGroup::SLOW:
									// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("_3"));
									break;
								case CSpecialItemGroup::DRAIN_HP:
									// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("_4"));
									break;
								case CSpecialItemGroup::POISON:
									// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("_5"));
									break;
								case CSpecialItemGroup::MOB_GROUP:
									// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("_6"));
									break;
								default:
									break;

							}

							TChestItem cItem;
							cItem.itemCount = dwCounts[i];
							strlcpy(cItem.szName, item_gets[i]->GetName(), sizeof(cItem.szName));
							//snprintf(cItem.szName, sizeof(cItem.szName), item_gets[i]->GetName());

							std::map<DWORD, TChestItem>::iterator it = map_Items.find(item_gets[i]->GetVnum());
							if (it == map_Items.end()) {
								map_Items.insert(std::map<DWORD, TChestItem>::value_type(item_gets[i]->GetVnum(), cItem));
							}
							else {
								it->second.itemCount = it->second.itemCount + cItem.itemCount;
							}
#ifdef STOP_IF_ITEM_DROPPED_TO_GROUND
							if (item_gets[i]->GetWindow() == GROUND) { return; }
#endif
						}
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("a_a_a_a"));
						return;
					}
				}
				else {
					if (!ch->UseItemEx(chestItem, NPOS)) { break; }
				}
			}
		
			if (isAddedToMap)
			{
				if (map_Items.size() > 0)
				{
					for (std::map<DWORD, TChestItem>::iterator it = map_Items.begin(); it != map_Items.end(); ++it)
					{
						if (it->second.itemCount > 1)
						{
							ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s_%d_"), it->second.szName, it->second.itemCount);
						}
						// else
						// {
							// ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("_%s_"), it->second.szName);
						// }
					}
				}
			}
		}
	}
}

int CInputMain::Analyze(LPDESC d, BYTE bHeader, const char * c_pData)
{
	LPCHARACTER ch;

	if (!(ch = d->GetCharacter()))
	{
		sys_err("no character on desc");
		d->SetPhase(PHASE_CLOSE);
		return (0);
	}

	int iExtraLen = 0;
	
	if (test_server && bHeader != HEADER_CG_MOVE)
		sys_log(0, "CInputMain::Analyze() ==> Header [%d] ", bHeader);

	switch (bHeader)
	{
		case HEADER_CG_PONG:
			Pong(d); 
			break;

		case HEADER_CG_TIME_SYNC:
			Handshake(d, c_pData);
			break;

		case HEADER_CG_CHAT:
			if (test_server)
			{
				char* pBuf = (char*)c_pData;
				sys_log(0, "%s", pBuf + sizeof(TPacketCGChat));
			}
	
			if ((iExtraLen = Chat(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;

		case HEADER_CG_WHISPER:
			if ((iExtraLen = Whisper(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;

		case HEADER_CG_MOVE:
			Move(ch, c_pData);

			if (LC_IsEurope())
			{
				if (g_bCheckClientVersion)
				{
					int version = atoi(g_stClientVersion.c_str());
					int date	= atoi(d->GetClientVersion());

					//if (0 != g_stClientVersion.compare(d->GetClientVersion()))
					if (version > date)
					{
						ch->ChatPacket(CHAT_TYPE_NOTICE, LC_TEXT("Å¬¶óÀÌ¾ðÆ® ¹öÀüÀÌ Æ²·Á ·Î±×¾Æ¿ô µË´Ï´Ù. Á¤»óÀûÀ¸·Î ÆÐÄ¡ ÈÄ Á¢¼ÓÇÏ¼¼¿ä."));
						d->DelayedDisconnect(10);
						LogManager::instance().HackLog("VERSION_CONFLICT", d->GetAccountTable().login, ch->GetName(), d->GetHostName());
					}
				}
			}
			else
			{
				if (!*d->GetClientVersion())
				{   
					sys_err("Version not recieved name %s", ch->GetName());
					d->SetPhase(PHASE_CLOSE);
				}
			}
			break;

		case HEADER_CG_CHARACTER_POSITION:
			Position(ch, c_pData);
			break;

		case HEADER_CG_ITEM_USE:
			if (!ch->IsObserverMode())
				ItemUse(ch, c_pData);
			break;

		case HEADER_CG_ITEM_DROP:
			if (!ch->IsObserverMode())
			{
				ItemDrop(ch, c_pData);
			}
			break;

		case HEADER_CG_ITEM_DROP2:
			if (!ch->IsObserverMode())
				ItemDrop2(ch, c_pData);
			break;
		case HEADER_CG_ITEM_SELL:
			if (!ch->IsObserverMode())
				ItemSell(ch, c_pData);
		break;

		case HEADER_CG_ITEM_MOVE:
			if (!ch->IsObserverMode())
				ItemMove(ch, c_pData);
			break;
		case HEADER_CG_ITEM_DESTROY:
            if (!ch->IsObserverMode())
                ItemDestroy(ch, c_pData);
			break;
#ifdef ENABLE_SWITCHBOT
		case HEADER_CG_SWITCHBOT:
			if ((iExtraLen = Switchbot(ch, c_pData, m_iBufferLeft)) < 0)
			{
				return -1;
			}
			break;
#endif
#ifdef ENABLE_WON_EXCHANGE
		case HEADER_CG_WON_EXCHANGE:
			WonExchange(ch, c_pData);
			break;
#endif
#ifdef ENABLE_CUBE_RENEWAL
		case HEADER_CG_CUBE_RENEWAL:
			CubeRenewalSend(ch, c_pData);
			break;
#endif

		case HEADER_CG_ITEM_PICKUP:
			if (!ch->IsObserverMode())
				ItemPickup(ch, c_pData);
			break;

		case HEADER_CG_ITEM_USE_TO_ITEM:
			if (!ch->IsObserverMode())
				ItemToItem(ch, c_pData);
			break;

		case HEADER_CG_ITEM_GIVE:
			if (!ch->IsObserverMode())
				ItemGive(ch, c_pData);
			break;

		case HEADER_CG_EXCHANGE:
			if (!ch->IsObserverMode())
				Exchange(ch, c_pData);
			break;

		case HEADER_CG_ATTACK:
		case HEADER_CG_SHOOT:
			if (!ch->IsObserverMode())
			{
				Attack(ch, bHeader, c_pData);
			}
			break;

#ifdef __SKILL_COLOR_SYSTEM__
		case HEADER_CG_SKILL_COLOR:
			SetSkillColor(ch, c_pData);
			break;
#endif

		case HEADER_CG_USE_SKILL:
			if (!ch->IsObserverMode())
				UseSkill(ch, c_pData);
			break;

		case HEADER_CG_QUICKSLOT_ADD:
			QuickslotAdd(ch, c_pData);
			break;

		case HEADER_CG_QUICKSLOT_DEL:
			QuickslotDelete(ch, c_pData);
			break;

		case HEADER_CG_QUICKSLOT_SWAP:
			QuickslotSwap(ch, c_pData);
			break;

		case HEADER_CG_SHOP:
			if ((iExtraLen = Shop(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		case HEADER_CG_OFFLINE_SHOP:
			if ((iExtraLen = OfflineShop(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;
#endif
		case HEADER_CG_MESSENGER:
			if ((iExtraLen = Messenger(ch, c_pData, m_iBufferLeft))<0)
				return -1;
			break;
			
		case HEADER_CG_SHOP2:
			Shop2(ch, c_pData);
			break;			

		case HEADER_CG_ON_CLICK:
			OnClick(ch, c_pData);
			break;

		case HEADER_CG_SYNC_POSITION:
			if ((iExtraLen = SyncPosition(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;

		case HEADER_CG_ADD_FLY_TARGETING:
		case HEADER_CG_FLY_TARGETING:
			FlyTarget(ch, c_pData, bHeader);
			break;

		case HEADER_CG_SCRIPT_BUTTON:
			ScriptButton(ch, c_pData);
			break;

			// SCRIPT_SELECT_ITEM
		case HEADER_CG_SCRIPT_SELECT_ITEM:
			ScriptSelectItem(ch, c_pData);
			break;
			// END_OF_SCRIPT_SELECT_ITEM

		case HEADER_CG_SCRIPT_ANSWER:
			ScriptAnswer(ch, c_pData);
			break;

		case HEADER_CG_QUEST_INPUT_STRING:
			QuestInputString(ch, c_pData);
			break;

		case HEADER_CG_QUEST_CONFIRM:
			QuestConfirm(ch, c_pData);
			break;

		case HEADER_CG_TARGET:
			Target(ch, c_pData);
			break;

		case HEADER_CG_WARP:
			Warp(ch, c_pData);
			break;

		case HEADER_CG_SAFEBOX_CHECKIN:
			SafeboxCheckin(ch, c_pData);
			break;

		case HEADER_CG_SAFEBOX_CHECKOUT:
			SafeboxCheckout(ch, c_pData, false);
			break;

		case HEADER_CG_SAFEBOX_ITEM_MOVE:
			SafeboxItemMove(ch, c_pData);
			break;

		case HEADER_CG_MALL_CHECKOUT:
			SafeboxCheckout(ch, c_pData, true);
			break;

		case HEADER_CG_PARTY_INVITE:
			PartyInvite(ch, c_pData);
			break;

		case HEADER_CG_PARTY_REMOVE:
			PartyRemove(ch, c_pData);
			break;

		case HEADER_CG_PARTY_INVITE_ANSWER:
			PartyInviteAnswer(ch, c_pData);
			break;

		case HEADER_CG_PARTY_SET_STATE:
			PartySetState(ch, c_pData);
			break;

		case HEADER_CG_PARTY_USE_SKILL:
			PartyUseSkill(ch, c_pData);
			break;

		case HEADER_CG_PARTY_PARAMETER:
			PartyParameter(ch, c_pData);
			break;

		case HEADER_CG_ANSWER_MAKE_GUILD:
			AnswerMakeGuild(ch, c_pData);
			break;

		case HEADER_CG_GUILD:
			if ((iExtraLen = Guild(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;

		case HEADER_CG_FISHING:
			Fishing(ch, c_pData);
			break;

		case HEADER_CG_HACK:
			Hack(ch, c_pData);
			break;

#ifdef ENABLE_NEW_PET_SYSTEM
		case HEADER_CG_PetSetName:
			BraveRequestPetName(ch, c_pData);
			break;
#endif

		case HEADER_CG_MYSHOP:
			if ((iExtraLen = MyShop(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		case HEADER_CG_MY_OFFLINE_SHOP:
			if ((iExtraLen = MyOfflineShop(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;
			break;
#endif
		case HEADER_CG_REFINE:
			Refine(ch, c_pData);
			break;

		case HEADER_CG_INSTANT_OPEN:
			{InstantChestOpen(ch, c_pData);}
			break;
			
		#ifdef __SASH_SYSTEM__
		case HEADER_CG_SASH:
			{
				Sash(ch, c_pData);
			}
			break;
		#endif			
#ifdef ENABLE_SHOW_CHEST_DROP
		case HEADER_CG_CHEST_DROP_INFO:
			ChestDropInfo(ch, c_pData);
			break;
#endif	
		case HEADER_CG_CLIENT_VERSION:
			Version(ch, c_pData);
			break;


#ifdef ENABLE_PM_ALL_SEND_SYSTEM
	case HEADER_CG_BULK_WHISPER:
		BulkWhisperManager(ch, c_pData);
		break;
#endif

#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
		case HEADER_CG_SHOP_SEARCH:
			ShopSearch(ch, c_pData);
			break;

		case HEADER_CG_SHOP_SEARCH_BUY:
			ShopSearchBuy(ch, c_pData);
			break;
#endif
		case HEADER_CG_HS_ACK:
			if (isHackShieldEnable)
			{
				CHackShieldManager::instance().VerifyAck(d->GetCharacter(), c_pData);
			}
			break;
#ifdef __SEND_TARGET_INFO__
		case HEADER_CG_TARGET_INFO_LOAD:
			{
				TargetInfoLoad(ch, c_pData);
			}
			break;
#endif
#ifdef ENABLE_AURA_SYSTEM
		case HEADER_CG_AURA:
			Aura(ch, c_pData);
			break;
#endif
		case HEADER_CG_XTRAP_ACK:
			{
				TPacketXTrapCSVerify* p = reinterpret_cast<TPacketXTrapCSVerify*>((void*)c_pData);
				CXTrapManager::instance().Verify_CSStep3(d->GetCharacter(), p->bPacketData);
			}
			break;
		case HEADER_CG_DRAGON_SOUL_REFINE:
			{
				TPacketCGDragonSoulRefine* p = reinterpret_cast <TPacketCGDragonSoulRefine*>((void*)c_pData);
				switch(p->bSubType)
				{
				case DS_SUB_HEADER_CLOSE:
					ch->DragonSoul_RefineWindow_Close();
					break;
				case DS_SUB_HEADER_DO_REFINE_GRADE:
					{
						DSManager::instance().DoRefineGrade(ch, p->ItemGrid);
					}
					break;
				case DS_SUB_HEADER_DO_REFINE_STEP:
					{
						DSManager::instance().DoRefineStep(ch, p->ItemGrid);
					}
					break;
				case DS_SUB_HEADER_DO_REFINE_STRENGTH:
					{
						DSManager::instance().DoRefineStrength(ch, p->ItemGrid);
					}
					break;
				}
			}

			break;
#ifdef __ENABLE_NEW_OFFLINESHOP__
		case HEADER_CG_NEW_OFFLINESHOP:
			if((iExtraLen = OfflineshopPacket(c_pData, ch, m_iBufferLeft))< 0)
				return -1;
			break;
#endif
	}
	return (iExtraLen);
}

int CInputDead::Analyze(LPDESC d, BYTE bHeader, const char * c_pData)
{
	LPCHARACTER ch;

	if (!(ch = d->GetCharacter()))
	{
		sys_err("no character on desc");
		return 0;
	}

	int iExtraLen = 0;

	switch (bHeader)
	{
		case HEADER_CG_PONG:
			Pong(d); 
			break;

		case HEADER_CG_TIME_SYNC:
			Handshake(d, c_pData);
			break;

		case HEADER_CG_CHAT:
			if ((iExtraLen = Chat(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;

			break;

		case HEADER_CG_WHISPER:
			if ((iExtraLen = Whisper(ch, c_pData, m_iBufferLeft)) < 0)
				return -1;

			break;

		case HEADER_CG_HACK:
			Hack(ch, c_pData);
			break;

		default:
			return (0);
	}

	return (iExtraLen);
}

#ifdef ENABLE_SWITCHBOT
int CInputMain::Switchbot(LPCHARACTER ch, const char* data, size_t uiBytes)
{
	const TPacketCGSwitchbot* p = reinterpret_cast<const TPacketCGSwitchbot*>(data);

	if (uiBytes < sizeof(TPacketCGSwitchbot))
	{
		return -1;
	}

	const char* c_pData = data + sizeof(TPacketCGSwitchbot);
	uiBytes -= sizeof(TPacketCGSwitchbot);

	switch (p->subheader)
	{
	case SUBHEADER_CG_SWITCHBOT_START:
	{
		size_t extraLen = sizeof(TSwitchbotAttributeAlternativeTable) * SWITCHBOT_ALTERNATIVE_COUNT;
		if (uiBytes < extraLen)
		{
			return -1;
		}

		std::vector<TSwitchbotAttributeAlternativeTable> vec_alternatives;

		for (BYTE alternative = 0; alternative < SWITCHBOT_ALTERNATIVE_COUNT; ++alternative)
		{
			const TSwitchbotAttributeAlternativeTable* pAttr = reinterpret_cast<const TSwitchbotAttributeAlternativeTable*>(c_pData);
			c_pData += sizeof(TSwitchbotAttributeAlternativeTable);

			vec_alternatives.emplace_back(*pAttr);
		}

		CSwitchbotManager::Instance().Start(ch->GetPlayerID(), p->slot, vec_alternatives);
		return extraLen;
	}

	case SUBHEADER_CG_SWITCHBOT_STOP:
	{
		CSwitchbotManager::Instance().Stop(ch->GetPlayerID(), p->slot);
		return 0;
	}
	}

	return 0;
}
#endif

#ifdef ENABLE_WON_EXCHANGE
void CInputMain::WonExchange(LPCHARACTER ch, const char* pcData)
{
	const TPacketCGWonExchange* p = reinterpret_cast<const TPacketCGWonExchange*>(pcData);
	const EWonExchangeCGSubHeader SubHeader = static_cast<EWonExchangeCGSubHeader>(p->bSubHeader);
	switch (SubHeader)
	{
	case WON_EXCHANGE_CG_SUBHEADER_SELL:
	case WON_EXCHANGE_CG_SUBHEADER_BUY:
		ch->WonExchange(SubHeader, p->wValue);
		break;
	default:
		sys_err("invalid won exchange subheader: %u value: %u", SubHeader, p->wValue);
		break;
	}
}
#endif

