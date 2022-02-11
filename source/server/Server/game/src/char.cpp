#include "stdafx.h"

#include "../../common/teen_packet.h"
#include "../../common/VnumHelper.h"

#include "char.h"

#include "config.h"
#include "utils.h"
#include "crc32.h"
#include "char_manager.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "buffer_manager.h"
#include "item_manager.h"
#include "motion.h"
#include "vector.h"
#include "packet.h"
#include "cmd.h"
#include "fishing.h"
#include "exchange.h"
#include "battle.h"
#include "affect.h"
#include "shop.h"
#include "shop_manager.h"
#include "safebox.h"
#include "regen.h"
#include "pvp.h"
#include "party.h"
#include "start_position.h"
#include "questmanager.h"
#include "log.h"
#include "p2p.h"
#include "guild.h"
#include "guild_manager.h"
#include "dungeon.h"
#include "messenger_manager.h"
#include "unique_item.h"
#include "priv_manager.h"
#include "war_map.h"
#include "xmas_event.h"
#include "banword.h"
#include "target.h"
#include "wedding.h"
#include "mob_manager.h"
#include "mining.h"
#include "monarch.h"
#include "castle.h"
#include "arena.h"
#include "dev_log.h"
#include "horsename_manager.h"
#include "pcbang.h"
#include "gm.h"
#include "map_location.h"
#include "BlueDragon_Binder.h"
#include "HackShield.h"
#include "skill_power.h"
#include "XTrapManager.h"
#include "buff_on_attributes.h"
#ifdef LWT_BUFFI_SYSTEM
	#include "lwt_buffi.h"
#endif
#ifdef __ENABLE_NEW_OFFLINESHOP__
#include "new_offlineshop.h"
#include "new_offlineshop_manager.h"
#endif

#ifdef __PET_SYSTEM__
#include "PetSystem.h"
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
#include "New_PetSystem.h"
#endif
#ifdef __SEND_TARGET_INFO__
#include <algorithm>
#include <iterator>
using namespace std;
#endif
#include "DragonSoul.h"
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
#include "offline_shop.h"
#include "offlineshop_manager.h"
#include "offlineshop_config.h"
#endif
#ifdef ENABLE_BEVIS_BOSS_TRACKING_SYSTEM
#include "boss_tracking.h"
#endif
#ifdef ENABLE_SWITCHBOT
#include "new_switchbot.h"
#endif
extern bool offlineshop_map_allow_find(int mapIndex);

extern int time1gGold;
extern int time2gGold;
extern int time3gGold;
extern int time4gGold;
extern bool offlineshop_map_allow_find(int mapIndex);

extern const BYTE g_aBuffOnAttrPoints;
extern bool RaceToJob(unsigned race, unsigned *ret_job);

extern int g_nPortalLimitTime;
extern int test_server;

extern bool IS_SUMMONABLE_ZONE(int map_index); // char_item.cpp
bool CAN_ENTER_ZONE(const LPCHARACTER& ch, int map_index);

bool CAN_ENTER_ZONE(const LPCHARACTER& ch, int map_index)
{
	switch (map_index)
	{
	case 301:
	case 302:
	case 303:
	case 304:
		if (ch->GetLevel() < 90)
			return false;
	}
	return true;
}

// <Factor> DynamicCharacterPtr member function definitions

LPCHARACTER DynamicCharacterPtr::Get() const {
	LPCHARACTER p = NULL;
	if (is_pc) {
		p = CHARACTER_MANAGER::instance().FindByPID(id);
	} else {
		p = CHARACTER_MANAGER::instance().Find(id);
	}
	return p;
}

DynamicCharacterPtr& DynamicCharacterPtr::operator=(LPCHARACTER character) {
	if (character == NULL) {
		Reset();
		return *this;
	}
	if (character->IsPC()) {
		is_pc = true;
		id = character->GetPlayerID();
	} else {
		is_pc = false;
		id = character->GetVID();
	}
	return *this;
}

CHARACTER::CHARACTER()
{
	m_stateIdle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateIdle, &CHARACTER::EndStateEmpty);
	m_stateMove.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateMove, &CHARACTER::EndStateEmpty);
	m_stateBattle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateBattle, &CHARACTER::EndStateEmpty);

	Initialize();
}

CHARACTER::~CHARACTER()
{
	Destroy();
}

void CHARACTER::Initialize()
{
	CEntity::Initialize(ENTITY_CHARACTER);

	m_bNoOpenedShop = true;

	m_bOpeningSafebox = false;

	m_fSyncTime = get_float_time()-3;
	m_dwPlayerID = 0;
	m_dwKillerPID = 0;
#ifdef __SEND_TARGET_INFO__
	dwLastTargetInfoPulse = 0;
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
	m_stImmortalSt = 0;

	m_newpetskillcd[0] = 0;
	m_newpetskillcd[1] = 0;
	m_newpetskillcd[2] = 0;
	m_newpetSystem = 0;
	m_bIsNewPet = false;
	m_eggvid = 0;
#endif

	m_iMoveCount = 0;

	m_pkRegen = NULL;
	regen_id_ = 0;
	m_posRegen.x = m_posRegen.y = m_posRegen.z = 0;
	m_posStart.x = m_posStart.y = 0;
	m_posDest.x = m_posDest.y = 0;
	m_fRegenAngle = 0.0f;

	m_pkMobData		= NULL;
	m_pkMobInst		= NULL;

	m_pkShop		= NULL;
	m_pkChrShopOwner	= NULL;
	m_pkMyShop		= NULL;
	m_pkExchange	= NULL;
	m_pkParty		= NULL;
	m_pkPartyRequestEvent = NULL;
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	// Offline Shop
	m_pkOfflineShop = NULL;
	m_pkChrOfflineShopOwner = NULL;
	m_pkOfflineShopUpdateEvent = NULL;
	// End Of Offline Shop
#endif

	m_pGuild = NULL;

	m_pkChrTarget = NULL;

	m_pkMuyeongEvent = NULL;

	m_pkWarpNPCEvent = NULL;
	m_pkDeadEvent = NULL;
	m_pkStunEvent = NULL;
	m_pkSaveEvent = NULL;
	m_pkRecoveryEvent = NULL;
	m_pkTimedEvent = NULL;
	m_pkTimedEventCh = NULL;
	m_pkFishingEvent = NULL;
	m_pkWarpEvent = NULL;

	// MINING
	m_pkMiningEvent = NULL;
	// END_OF_MINING

	m_pkPoisonEvent = NULL;
	m_pkFireEvent = NULL;
	m_pkCheckSpeedHackEvent	= NULL;
	m_speed_hack_count	= 0;

	m_pkAffectEvent = NULL;
	m_afAffectFlag = TAffectFlag(0, 0);

	m_pkDestroyWhenIdleEvent = NULL;

	m_pkChrSyncOwner = NULL;

	memset(&m_points, 0, sizeof(m_points));
	memset(&m_pointsInstant, 0, sizeof(m_pointsInstant));
	memset(&m_quickslot, 0, sizeof(m_quickslot));
#ifdef ENABLE_KILL_STATISTICS
	memset(&m_killstatistics, 0, sizeof(m_killstatistics));
#endif
	memset(&character_cards, 0, sizeof(character_cards));
	memset(&randomized_cards, 0, sizeof(randomized_cards));
	
	m_bCharType = CHAR_TYPE_MONSTER;

	SetPosition(POS_STANDING);

	m_dwPlayStartTime = m_dwLastMoveTime = get_dword_time();

	GotoState(m_stateIdle);
	m_dwStateDuration = 1;

	m_dwLastAttackTime = get_dword_time() - 20000;

	m_bAddChrState = 0;

	m_pkChrStone = NULL;

	m_pkSafebox = NULL;
	m_iSafeboxSize = -1;
	m_iSafeboxLoadTime = 0;

	m_pkMall = NULL;
	m_iMallLoadTime = 0;

	m_posWarp.x = m_posWarp.y = m_posWarp.z = 0;
	m_lWarpMapIndex = 0;

	m_posExit.x = m_posExit.y = m_posExit.z = 0;
	m_lExitMapIndex = 0;

	m_pSkillLevels = NULL;

	m_dwMoveStartTime = 0;
	m_dwMoveDuration = 0;

	m_dwFlyTargetID = 0;

	m_dwNextStatePulse = 0;

	m_dwLastDeadTime = get_dword_time()-180000;

	m_bSkipSave = false;

	m_bItemLoaded = false;
	
	#ifdef ENABLE_BEVIS_BOSS_TRACKING_SYSTEM
	m_pkUpdateBossTracking = NULL;
	#endif

	m_bHasPoisoned = false;

	m_pkDungeon = NULL;
	m_iEventAttr = 0;

	m_kAttackLog.dwVID = 0;
	m_kAttackLog.dwTime = 0;

	m_bNowWalking = m_bWalking = false;
	ResetChangeAttackPositionTime();

	m_bDetailLog = false;
	m_bMonsterLog = false;

	m_bDisableCooltime = false;

	m_iAlignment = 0;
	m_iRealAlignment = 0;

#ifdef ENABLE_TITLE_SYSTEM
	m_iPrestige = 0;
	m_iRealPrestige = 0;
#endif

	m_iKillerModePulse = 0;
	m_bPKMode = PK_MODE_PEACE;

	m_dwQuestNPCVID = 0;
	m_dwQuestByVnum = 0;
	m_pQuestItem = NULL;

	m_szMobileAuth[0] = '\0';

	m_dwUnderGuildWarInfoMessageTime = get_dword_time()-60000;

	m_bUnderRefine = false;

	// REFINE_NPC
	m_dwRefineNPCVID = 0;
	// END_OF_REFINE_NPC

	m_dwPolymorphRace = 0;

	m_bStaminaConsume = false;

	ResetChainLightningIndex();

	#ifdef ENABLE_REBIRT_SYSTEM
	m_dwRebirtLastTime = 0;
	#endif
	
	#ifdef ENABLE_GUVENLI_PC
	m_dwGuvPcLastTime = 0;
	#endif
	
	m_dwMountVnum = 0;
	m_chHorse = NULL;
	m_chRider = NULL;
#ifdef __ENABLE_NEW_OFFLINESHOP__
	m_pkNewOfflineShop = NULL;
	m_pkShopSafebox	= NULL;
	m_pkAuction		= NULL;
	m_pkAuctionGuest= NULL;
	//offlineshop-updated 03/08/19
	m_pkOfflineShopGuest = NULL;

	//offlineshop-updated 05/08/19
	m_bIsLookingOfflineshopOfferList = false;
#endif
	m_pWarMap = NULL;
	m_pWeddingMap = NULL;
	m_bChatCounter = 0;
	m_biosistem = 0;

	ResetStopTime();

	m_dwLastVictimSetTime = get_dword_time() - 3000;
	m_iMaxAggro = -100;

	m_bSendHorseLevel = 0;
	m_bSendHorseHealthGrade = 0;
	m_bSendHorseStaminaGrade = 0;

	m_dwLoginPlayTime = 0;

	m_pkChrMarried = NULL;

	m_posSafeboxOpen.x = -1000;
	m_posSafeboxOpen.y = -1000;

	// EQUIP_LAST_SKILL_DELAY
	m_dwLastSkillTime = get_dword_time();
	// END_OF_EQUIP_LAST_SKILL_DELAY

	// MOB_SKILL_COOLTIME
	memset(m_adwMobSkillCooltime, 0, sizeof(m_adwMobSkillCooltime));
	// END_OF_MOB_SKILL_COOLTIME

	m_isinPCBang = false;

	// ARENA
	m_pArena = NULL;
	m_nPotionLimit = quest::CQuestManager::instance().GetEventFlag("arena_potion_limit_count");
	// END_ARENA

	//PREVENT_TRADE_WINDOW
	m_isOpenSafebox = 0;
	//END_PREVENT_TRADE_WINDOW
	
	//PREVENT_REFINE_HACK
	m_iRefineTime = 0;
	//END_PREVENT_REFINE_HACK
	
	//RESTRICT_USE_SEED_OR_MOONBOTTLE
	m_iSeedTime = 0;
	//END_RESTRICT_USE_SEED_OR_MOONBOTTLE
	//PREVENT_PORTAL_AFTER_EXCHANGE
	m_iExchangeTime = 0;
	//END_PREVENT_PORTAL_AFTER_EXCHANGE
	//
	m_iSafeboxLoadTime = 0;

	m_iMyShopTime = 0;

	InitMC();

	m_deposit_pulse = 0;

	SET_OVER_TIME(this, OT_NONE);

	m_strNewName = "";

	m_known_guild.clear();

#ifdef KYGN_ITEM_REMOVE_OR_SELL
	R_S_I_SlotIndex.clear();
#endif

	m_dwLogOffInterval = 0;

	m_bComboSequence = 0;
	m_dwLastComboTime = 0;
	m_bComboIndex = 0;
	m_iComboHackCount = 0;
	m_dwSkipComboAttackByTime = 0;

	m_dwMountTime = 0;

	m_dwLastGoldDropTime = 0;
#ifdef ENABLE_CHEQUE_SYSTEM
	m_dwLastChequeDropTime = 0;
#endif
	m_HackShieldCheckEvent = NULL;
	m_HackShieldCheckMode = false;

	m_bIsLoadedAffect = false;
	cannot_dead = false;
#ifdef LWT_BUFFI_SYSTEM
	m_supportSystem = 0;
	m_dwSupportHair = 0;
	m_dwSupportWeapon = 0;
	m_dwSupportCostume = 0;
	m_bIsSupport = false;
	m_Owner = NULL;
#endif

#ifdef __PET_SYSTEM__
	m_petSystem = 0;
	m_bIsPet = false;
#endif

	m_fAttMul = 1.0f;
	m_fDamMul = 1.0f;

	m_pointsInstant.iDragonSoulActiveDeck = -1;

	memset(&m_tvLastSyncTime, 0, sizeof(m_tvLastSyncTime));
#ifdef WJ_SECURITY_SYSTEM
	m_bActivateSecurity = false;
	m_bActivateSecurityGui = false;
#endif
	m_iSyncHackCount = 0;
	#ifdef __SASH_SYSTEM__
	m_bSashCombination = false;
	m_bSashAbsorption = false;
	#endif
#ifdef ENABLE_AURA_SYSTEM
	m_bAuraRefine = false;
	m_bAuraAbsorption = false;
#endif
#ifdef __SKILL_COLOR_SYSTEM__
	memset(&m_dwSkillColor, 0, sizeof(m_dwSkillColor));
#endif
	m_iMyOfflineShopTime = 0;
	//Shop Decoration
	m_npc_offline = 30000;
	m_npc_decoration = 0;
	//Shop Decoration
}

void CHARACTER::Create(const char * c_pszName, DWORD vid, bool isPC)
{
	static int s_crc = 172814;

	char crc_string[128+1];
	snprintf(crc_string, sizeof(crc_string), "%s%p%d", c_pszName, this, ++s_crc);
	m_vid = VID(vid, GetCRC32(crc_string, strlen(crc_string)));

	if (isPC)
		m_stName = c_pszName;
}

void CHARACTER::Destroy()
{
	CloseMyShop();

if (m_pkRegen)
	{
		if (m_pkDungeon) {
			// Dungeon regen may not be valid at this point
			if (m_pkDungeon->IsValidRegen(m_pkRegen, regen_id_)) {
				--m_pkRegen->count;
			}
		} else {
			// Is this really safe? NO IT ISNT! F*CK THAT SH!T!
			if (is_valid_regen(m_pkRegen))
				--m_pkRegen->count;
		}
		m_pkRegen = NULL;
	}

	if (m_pkDungeon)
	{
		SetDungeon(NULL);
	}

#ifdef LWT_BUFFI_SYSTEM
	if (m_supportSystem)
	{
		m_supportSystem->Destroy();
		delete m_supportSystem;

		m_supportSystem = 0;
	}
#endif

#ifdef __PET_SYSTEM__
	if (m_petSystem)
	{
		m_petSystem->Destroy();
		delete m_petSystem;

		m_petSystem = 0;
	}
#endif

#ifdef ENABLE_NEW_PET_SYSTEM
	if (m_newpetSystem)
	{
		m_newpetSystem->Destroy();
		delete m_newpetSystem;

		m_newpetSystem = 0;
	}
#endif

	HorseSummon(false);

	if (GetRider())
		GetRider()->ClearHorseInfo();

	if( IsPC() )
	{
		if (isHackShieldEnable)
		{
			CHackShieldManager::instance().DeleteClientHandle(GetPlayerID());
		}
	}

	if (GetDesc())
	{
		GetDesc()->BindCharacter(NULL);
//		BindDesc(NULL);
	}

	if (m_pkExchange)
		m_pkExchange->Cancel();

	SetVictim(NULL);

	if (GetShop())
	{
		GetShop()->RemoveGuest(this);
		SetShop(NULL);
	}

	ClearStone();
	ClearSync();
	ClearTarget();

	if (NULL == m_pkMobData)
	{
		DragonSoul_CleanUp();
		ClearItem();
	}

	// <Factor> m_pkParty becomes NULL after CParty destructor call!
	LPPARTY party = m_pkParty;
	if (party)
	{
		if (party->GetLeaderPID() == GetVID() && !IsPC())
		{
			M2_DELETE(party);
		}
		else
		{
			party->Unlink(this); 

			if (!IsPC())
				party->Quit(GetVID());
		}

		SetParty(NULL); // ????? ?????? ???????.
	}

	if (m_pkMobInst)
	{
		M2_DELETE(m_pkMobInst);
		m_pkMobInst = NULL;
	}

	m_pkMobData = NULL;

	if (m_pkSafebox)
	{
		M2_DELETE(m_pkSafebox);
		m_pkSafebox = NULL;
	}

	if (m_pkMall)
	{
		M2_DELETE(m_pkMall);
		m_pkMall = NULL;
	}

	m_set_pkChrSpawnedBy.clear();

	StopMuyeongEvent();
	event_cancel(&m_pkWarpNPCEvent);
	event_cancel(&m_pkRecoveryEvent);
	event_cancel(&m_pkDeadEvent);
	event_cancel(&m_pkSaveEvent);
	event_cancel(&m_pkTimedEvent);
	event_cancel(&m_pkTimedEventCh);
	event_cancel(&m_pkStunEvent);
	event_cancel(&m_pkFishingEvent);
	event_cancel(&m_pkPoisonEvent);
	event_cancel(&m_pkFireEvent);
	event_cancel(&m_pkPartyRequestEvent);
	#ifdef ENABLE_BEVIS_BOSS_TRACKING_SYSTEM
	event_cancel(&m_pkUpdateBossTracking);
	#endif
	//DELAYED_WARP
	event_cancel(&m_pkWarpEvent);
	event_cancel(&m_pkCheckSpeedHackEvent);
	//END_DELAYED_WARP

	// RECALL_DELAY
	//event_cancel(&m_pkRecallEvent);
	// END_OF_RECALL_DELAY

	// MINING
	event_cancel(&m_pkMiningEvent);
	// END_OF_MINING

	StopHackShieldCheckCycle();

	for (itertype(m_mapMobSkillEvent) it = m_mapMobSkillEvent.begin(); it != m_mapMobSkillEvent.end(); ++it)
	{
		LPEVENT pkEvent = it->second;
		event_cancel(&pkEvent);
	}
	m_mapMobSkillEvent.clear();

	//event_cancel(&m_pkAffectEvent);
	ClearAffect();

	for (TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.begin();  it != m_map_buff_on_attrs.end(); it++)
	{
		if (NULL != it->second)
		{
			M2_DELETE(it->second);
		}
	}
	m_map_buff_on_attrs.clear();

	event_cancel(&m_pkDestroyWhenIdleEvent);

	if (m_pSkillLevels)
	{
		M2_DELETE_ARRAY(m_pSkillLevels);
		m_pSkillLevels = NULL;
	}

	CEntity::Destroy();

	if (GetSectree())
		GetSectree()->RemoveEntity(this);

	if (m_bMonsterLog)
		CHARACTER_MANAGER::instance().UnregisterForMonsterLog(this);
}

const char * CHARACTER::GetName() const
{
	return m_stName.empty() ? (m_pkMobData ? m_pkMobData->m_table.szLocaleName : "") : m_stName.c_str();
}


void CHARACTER::KanalSwitch(int new_ch)
{
	long lAddr;
	long lMapIndex;
	WORD wPort;
	long x = this->GetX();
	long y = this->GetY();
	if (!CMapLocation::instance().Get(x, y, lMapIndex, lAddr, wPort))
	{
		sys_err("cannot find map location index %d x %d y %d name %s", lMapIndex, x, y, GetName());
		return;
	}
	if(lMapIndex >= 10000)
	{
		sys_err("hata 1");
		return;
	}
            
	std::map<WORD, int>micha;
	for(int i = 0; i < 6; i++)
	{ //replace with maximum channels -1   actual 1300x - 1330x
		for(int i2 = 2; i2 < 11; i2++)
		{ //replace with your core values actual  13x02 - 13x08
			micha[12*1000 + i*100 + i2] = i+1;
		}        
	}
	int chan;
	if(micha.find(wPort) != micha.end())
	{
		chan = micha[wPort];
	}
	else
	{
		sys_err("hata 2");
		return;
	}
			
	Stop();
	Save();
            
	if(GetSectree())
	{
		GetSectree()->RemoveEntity(this);
		ViewCleanup();
		EncodeRemovePacket(this);
	}

	TPacketGCWarp p;
	p.bHeader    = HEADER_GC_WARP;
	p.lX    = x;
	p.lY    = y;
	p.lAddr    = lAddr;
	p.wPort    = (wPort - 100*(chan-1) + 100*(new_ch-1));

	GetDesc()->Packet(&p, sizeof(TPacketGCWarp));
}

void CHARACTER::OpenMyShop(const char * c_pszSign, TShopItemTable * pTable, short bItemCount)
{
	if (GetPart(PART_MAIN) > 2)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("?????? ????? ???? ?????? ?? ?? ??????."));
		return;
	}

	if (m_pkTimedEventCh)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Hizli ch durduruldu."));
		event_cancel(&m_pkTimedEventCh);
	}	
	
	if (GetMyShop())	// ??? ???? ???? ?????? ??��?.
	{
		CloseMyShop();
		return;
	}

	// ???????? ??????? ?????? ?????? ?? ?? ????.
	quest::PC * pPC = quest::CQuestManager::instance().GetPCForce(GetPlayerID());

	// GetPCForce?? NULL?? ?? ??????? ???? ??????? ????
	if (pPC->IsRunning())
		return;

	if (bItemCount == 0)
		return;

	int64_t nTotalMoney = 0;
#ifdef ENABLE_CHEQUE_SYSTEM
	int64_t nTotalCheque = 0;
#endif
	for (int n = 0; n < bItemCount; ++n)
	{
		nTotalMoney += static_cast<int64_t>((pTable+n)->price);
#ifdef ENABLE_CHEQUE_SYSTEM
		nTotalCheque += static_cast<int64_t>((pTable+n)->price_cheque);
#endif
	}

	nTotalMoney += static_cast<int64_t>(GetGold());

#ifdef YANG_LIMIT
	if (GetAllowedGold() <= nTotalMoney)
#else
	if (GOLD_MAX <= nTotalMoney)
#endif
	{
		sys_err("[OVERFLOW_GOLD] Overflow (GOLD_MAX) id %u name %s", GetPlayerID(), GetName());
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("20?? ???? ?????? ?????? ?????? ???????"));
		return;
	}

#ifdef ENABLE_CHEQUE_SYSTEM
	nTotalCheque += static_cast<int64_t>(GetCheque());
	if (CHEQUE_MAX <= nTotalCheque)
	{
		sys_err("[OVERFLOW_GOLD] Overflow (CHEQUE_MAX) id %u name %s", GetPlayerID(), GetName());
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("20? ?? ???? ??? ??? ????"));
		return;
	}
#endif
	char szSign[SHOP_SIGN_MAX_LEN+1];
	strlcpy(szSign, c_pszSign, sizeof(szSign));

	 m_stShopSign = szSign;

	if (strlen(szSign) == 0)
		return;

	if (CBanwordManager::instance().CheckString(szSign, strlen(szSign)))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("???? ???? ????? ???? ??????? ?????? ?? ?? ???????."));	
		return;
	}

	// MYSHOP_PRICE_LIST
	std::map<DWORD, DWORD> itemkind;  // ?????? ?????? ????, first: vnum, second: ???? ???? ????
	// END_OF_MYSHOP_PRICE_LIST	

	std::set<TItemPos> cont;
	for (short i = 0; i < bItemCount; ++i)
	{
		if (cont.find((pTable + i)->pos) != cont.end())
		{
			sys_err("MYSHOP: duplicate shop item detected! (name: %s)", GetName());
			return;
		}

		// ANTI_GIVE, ANTI_MYSHOP check
		LPITEM pkItem = GetItem((pTable + i)->pos);

		if (pkItem)
		{
			const TItemTable * item_table = pkItem->GetProto();

			if (item_table && (IS_SET(item_table->dwAntiFlags, ITEM_ANTIFLAG_GIVE | ITEM_ANTIFLAG_MYSHOP)))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("????? ???????? ???��??????? ????? ?? ???????."));
				return;
			}

			if (pkItem->IsEquipped() == true)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("??????? ???????? ???��??????? ????? ?? ???????."));
				return;
			}

			if (true == pkItem->isLocked())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("??????? ???????? ???��??????? ????? ?? ???????."));
				return;
			}

			// MYSHOP_PRICE_LIST
			itemkind[pkItem->GetVnum()] = (pTable + i)->price / pkItem->GetCount();
#ifdef ENABLE_CHEQUE_SYSTEM
			itemkind[pkItem->GetVnum()] = (pTable + i)->price_cheque / pkItem->GetCount();
#endif
			// END_OF_MYSHOP_PRICE_LIST
		}

		cont.insert((pTable + i)->pos);
	}

	// MYSHOP_PRICE_LIST
	// ?????? ?????? ????????. 
	if (CountSpecifyItem(71049)) { // ??? ???????? ?????? ??? ?????????? ???????.

		//
		// ?????? ?????????? ??????? ???? ?????? ???????? ????? ????? DB ��?y? ??????.
		//
		TPacketMyshopPricelistHeader header;
		TItemPriceInfo info;
		
		header.dwOwnerID = GetPlayerID();
		header.byCount = itemkind.size();

		TEMP_BUFFER buf;
		buf.write(&header, sizeof(header));

		for (itertype(itemkind) it = itemkind.begin(); it != itemkind.end(); ++it)
		{
			info.dwVnum = it->first;
			info.dwPrice = it->second;
#ifdef ENABLE_CHEQUE_SYSTEM
			info.dwPriceCheque = it->second;
#endif

			buf.write(&info, sizeof(info));
		}

		db_clientdesc->DBPacket(HEADER_GD_MYSHOP_PRICELIST_UPDATE, 0, buf.read_peek(), buf.size());
	} 
	// END_OF_MYSHOP_PRICE_LIST
	else if (CountSpecifyItem(50200))
		RemoveSpecifyItem(50200, 1);
	else
		return; // ???????? ?????? ???.

	if (m_pkExchange)
		m_pkExchange->Cancel();

	//m_stShopSign = szSign;
	TPacketGCShopSign p;

	p.bHeader = HEADER_GC_SHOP_SIGN;
	p.dwVID = GetVID();
	strlcpy(p.szSign, szSign, sizeof(p.szSign));

	PacketAround(&p, sizeof(TPacketGCShopSign));

	m_pkMyShop = CShopManager::instance().CreatePCShop(this, pTable, bItemCount);

	if (IsPolymorphed() == true)
	{
		RemoveAffect(AFFECT_POLYMORPH);
	}

	if (GetHorse())
	{
		HorseSummon( false, true );
	}
	// new mount ??? ???, ???? ???? ???? ??? unmount
	// StopRiding???? ??????????? o????? ?????? ?? ????? ????????? ?? ?? ????.
	else if (GetMountVnum())
	{
		RemoveAffect(AFFECT_MOUNT);
		RemoveAffect(AFFECT_MOUNT_BONUS);
	}
	//if (!LC_IsNewCIBN())
		SetPolymorph(30000, true);

}

void CHARACTER::CloseMyShop()
{
	if (GetMyShop())
	{
		m_stShopSign.clear();
		CShopManager::instance().DestroyPCShop(this);
		m_pkMyShop = NULL;

		TPacketGCShopSign p;

		p.bHeader = HEADER_GC_SHOP_SIGN;
		p.dwVID = GetVID();
		p.szSign[0] = '\0';

		PacketAround(&p, sizeof(p));
		
		//if (!LC_IsNewCIBN())
			SetPolymorph(GetJob(), true);
	}
}

void EncodeMovePacket(TPacketGCMove & pack, DWORD dwVID, BYTE bFunc, BYTE bArg, DWORD x, DWORD y, DWORD dwDuration, DWORD dwTime, BYTE bRot)
{
	pack.bHeader = HEADER_GC_MOVE;
	pack.bFunc   = bFunc;
	pack.bArg    = bArg;
	pack.dwVID   = dwVID;
	pack.dwTime  = dwTime ? dwTime : get_dword_time();
	pack.bRot    = bRot;
	pack.lX		= x;
	pack.lY		= y;
	pack.dwDuration	= dwDuration;
}

void CHARACTER::RestartAtSamePos()
{
	if (m_bIsObserver)
		return;

	EncodeRemovePacket(this);
	EncodeInsertPacket(this);

	ENTITY_MAP::iterator it = m_map_view.begin();

	while (it != m_map_view.end())
	{
		LPENTITY entity = (it++)->first;

		EncodeRemovePacket(entity);
		if (!m_bIsObserver)
			EncodeInsertPacket(entity);

		if( entity->IsType(ENTITY_CHARACTER) )
		{
			LPCHARACTER lpChar = (LPCHARACTER)entity;
			if( lpChar->IsPC() || lpChar->IsNPC() || lpChar->IsMonster() )
			{
				if (!entity->IsObserverMode())
					entity->EncodeInsertPacket(this);
			}
		}
		else
		{
			if( !entity->IsObserverMode())
			{
				entity->EncodeInsertPacket(this);
			}
		}
	}
}


// Entity?? ???? ???????? ????? ??????.
void CHARACTER::EncodeInsertPacket(LPENTITY entity)
{

	LPDESC d;

	if (!(d = entity->GetDesc()))
		return;

	// ?????? ???? ???? ???
	LPCHARACTER ch = (LPCHARACTER) entity;
	ch->SendGuildName(GetGuild());
	// ?????? ???? ???? ???

	TPacketGCCharacterAdd pack;

	pack.header		= HEADER_GC_CHARACTER_ADD;
	pack.dwVID		= m_vid;
	pack.bType		= GetCharType();
	pack.angle		= GetRotation();
	pack.x		= GetX();
	pack.y		= GetY();
	pack.z		= GetZ();
	pack.wRaceNum	= GetRaceNum();
#ifdef ENABLE_NEW_PET_SYSTEM
	if (IsPet() || IsNewPet())
#else
	if (IsPet() || IsSupport())
#endif
	{
		pack.bMovingSpeed	= 150;
	}
	else
	{
		pack.bMovingSpeed	= GetLimitPoint(POINT_MOV_SPEED);
	}
	pack.bAttackSpeed	= GetLimitPoint(POINT_ATT_SPEED);
	pack.dwAffectFlag[0] = m_afAffectFlag.bits[0];
	pack.dwAffectFlag[1] = m_afAffectFlag.bits[1];

	pack.bStateFlag = m_bAddChrState;

	int iDur = 0;

	if (m_posDest.x != pack.x || m_posDest.y != pack.y)
	{
		iDur = (m_dwMoveStartTime + m_dwMoveDuration) - get_dword_time();

		if (iDur <= 0)
		{
			pack.x = m_posDest.x;
			pack.y = m_posDest.y;
		}
	}

	d->Packet(&pack, sizeof(pack));

	if (IsPC() == true || m_bCharType == CHAR_TYPE_NPC)
	{
		TPacketGCCharacterAdditionalInfo addPacket;
		memset(&addPacket, 0, sizeof(TPacketGCCharacterAdditionalInfo));

		addPacket.header = HEADER_GC_CHAR_ADDITIONAL_INFO;
		addPacket.dwVID = m_vid;

		addPacket.awPart[CHR_EQUIPPART_ARMOR] = GetPart(PART_MAIN);
		addPacket.awPart[CHR_EQUIPPART_WEAPON] = GetPart(PART_WEAPON);
		addPacket.awPart[CHR_EQUIPPART_HEAD] = GetPart(PART_HEAD);
		addPacket.awPart[CHR_EQUIPPART_HAIR] = GetPart(PART_HAIR);
		#ifdef __SASH_SYSTEM__
		addPacket.awPart[CHR_EQUIPPART_SASH] = GetPart(PART_SASH);
		#endif
#ifdef ENABLE_AURA_SYSTEM
		addPacket.awPart[CHR_EQUIPPART_AURA] = GetPart(PART_AURA);
#endif
#ifdef __NEW_ARROW_SYSTEM__
		addPacket.awPart[CHR_PART_ARROW_TYPE] = GetPart(PART_ARROW_TYPE);
#endif
#ifdef LWT_BUFFI_SYSTEM
		if(IsSupport())
			addPacket.bPKMode = PK_MODE_PROTECT;
		else
			addPacket.bPKMode = m_bPKMode;
#else
		addPacket.bPKMode = m_bPKMode;
#endif
		addPacket.dwMountVnum = GetMountVnum();
		addPacket.bEmpire = m_bEmpire;

		if (IsPC() == true || (IsOfflineShopNPC() && GetRaceNum() == 30000 || GetRaceNum() == 30001 || GetRaceNum() == 30002 || GetRaceNum() == 30003|| GetRaceNum() == 30004|| GetRaceNum() == 30005 || GetRaceNum() == 30006 || GetRaceNum() == 30007)
#ifdef ENABLE_NEW_PET_SYSTEM
			 || IsNewPet() == true || IsSupport()
#endif
		)
		{
			addPacket.dwLevel = GetLevel();
		}
		else
		{
			addPacket.dwLevel = 0;
		}
		
		LPITEM weapon = GetWear(WEAR_WEAPON);
		if (weapon)
			addPacket.dwWeaponEvolution = weapon->GetEvolution();
		else
			addPacket.dwWeaponEvolution = 0;

#ifdef LWT_BUFFI_SYSTEM
		if(IsSupport())
		{
			addPacket.awPart[CHR_EQUIPPART_ARMOR] = GetSupportArmor();
			addPacket.awPart[CHR_EQUIPPART_WEAPON] = GetSupportWeapon();
			addPacket.awPart[CHR_EQUIPPART_HAIR] = GetSupportHair();
		}
#endif

#ifdef ENABLE_KOSTUMPARLA
		LPITEM item = GetWear(WEAR_COSTUME_BODY);
		if (item)
			addPacket.dwCostumEvo = item->GetSocket(1);
		else
			addPacket.dwCostumEvo = 0;
#endif

#ifdef __SKILL_COLOR_SYSTEM__
		memcpy(addPacket.dwSkillColor, GetSkillColor(), sizeof(addPacket.dwSkillColor));
#endif

#ifdef ENABLE_REBIRT_SYSTEM
		if (IsPC() == true)
		{
			addPacket.bRebirt = GetRebirt();
		}
		else
		{
			addPacket.bRebirt = 0;
		}
#endif
		
		if (false)
		{
			LPCHARACTER ch = (LPCHARACTER) entity;

			if (GetEmpire() == ch->GetEmpire() || ch->GetGMLevel() > GM_PLAYER || m_bCharType == CHAR_TYPE_NPC)
			{
				goto show_all_info;
			}
			else
			{
				memset(addPacket.name, 0, CHARACTER_NAME_MAX_LEN);
				addPacket.dwGuildID = 0;
				addPacket.sAlignment = 0;
#ifdef ENABLE_TITLE_SYSTEM
				addPacket.sPrestige = 0;
#endif				
			}
		}
		else
		{
		show_all_info:
			strlcpy(addPacket.name, GetName(), sizeof(addPacket.name));

			if (GetGuild() != NULL)
			{	
				addPacket.dwGuildID = GetGuild()->GetID();
			}
			else
			{
				addPacket.dwGuildID = 0;
			}

			addPacket.sAlignment = m_iAlignment / 10;
#ifdef ENABLE_TITLE_SYSTEM
			addPacket.sPrestige = m_iPrestige;
#endif
		}

		d->Packet(&addPacket, sizeof(TPacketGCCharacterAdditionalInfo));
	}

	if (iDur)
	{
		TPacketGCMove pack;
		EncodeMovePacket(pack, GetVID(), FUNC_MOVE, 0, m_posDest.x, m_posDest.y, iDur, 0, (BYTE) (GetRotation() / 5));
		d->Packet(&pack, sizeof(pack));

		TPacketGCWalkMode p;
		p.vid = GetVID();
		p.header = HEADER_GC_WALK_MODE;
		p.mode = m_bNowWalking ? WALKMODE_WALK : WALKMODE_RUN;

		d->Packet(&p, sizeof(p));
	}

	if (entity->IsType(ENTITY_CHARACTER) && GetDesc())
	{
		LPCHARACTER ch = (LPCHARACTER) entity;
		if (ch->IsWalking())
		{
			TPacketGCWalkMode p;
			p.vid = ch->GetVID();
			p.header = HEADER_GC_WALK_MODE;
			p.mode = ch->m_bNowWalking ? WALKMODE_WALK : WALKMODE_RUN;
			GetDesc()->Packet(&p, sizeof(p));
		}
	}

	if (IsPC() && GetMyShop())
	{
		TPacketGCShopSign p;

		p.bHeader = HEADER_GC_SHOP_SIGN;
		p.dwVID = GetVID();
		strlcpy(p.szSign, m_stShopSign.c_str(), sizeof(p.szSign));

		d->Packet(&p, sizeof(TPacketGCShopSign));
	}

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (IsOfflineShopNPC() && GetRaceNum() == 30000 || GetRaceNum() == 30001 || GetRaceNum() == 30002 || GetRaceNum() == 30003|| GetRaceNum() == 30004|| GetRaceNum() == 30005 || GetRaceNum() == 30006 || GetRaceNum() == 30007)
	{
		TPacketGCShopSign p;

		p.bHeader = HEADER_GC_OFFLINE_SHOP_SIGN;
		p.dwVID = GetVID();
		if (m_stOfflineShopSign.empty())
		{
			std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT sign FROM player.offline_shop_npc WHERE owner_id = %u", GetOfflineShopRealOwner()));
			if (pMsg->Get()->uiNumRows > 0)
			{				
				MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);

				std::unique_ptr<SQLMsg> pMsg1(DBManager::instance().DirectQuery("SELECT npc_decoration FROM player.offline_shop_npc WHERE owner_id = %u", GetOfflineShopRealOwner()));
				MYSQL_ROW row1 = mysql_fetch_row(pMsg1->Get()->pSQLResult);
				std::string m_variable01 = row1[0];
				std::string m_variable02 = row[0];
				std::string m_variable03 = m_variable01+m_variable02;
				strlcpy(p.szSign, m_variable03.c_str() , sizeof(p.szSign));				
			}
		}
		else
		{
			strlcpy(p.szSign, m_stOfflineShopSign.c_str(), sizeof(p.szSign));
		}
		
		d->Packet(&p, sizeof(TPacketGCShopSign));
	}
#endif
	if (entity->IsType(ENTITY_CHARACTER))
	{
		sys_log(3, "EntityInsert %s (RaceNum %d) (%d %d) TO %s",
				GetName(), GetRaceNum(), GetX() / SECTREE_SIZE, GetY() / SECTREE_SIZE, ((LPCHARACTER)entity)->GetName());
	}
}

void CHARACTER::EncodeRemovePacket(LPENTITY entity)
{
	if (entity->GetType() != ENTITY_CHARACTER)
		return;

	LPDESC d;

	if (!(d = entity->GetDesc()))
		return;

	TPacketGCCharacterDelete pack;

	pack.header	= HEADER_GC_CHARACTER_DEL;
	pack.id	= m_vid;

	d->Packet(&pack, sizeof(TPacketGCCharacterDelete));

	if (entity->IsType(ENTITY_CHARACTER))
		sys_log(3, "EntityRemove %s(%d) FROM %s", GetName(), (DWORD) m_vid, ((LPCHARACTER) entity)->GetName());
}

void CHARACTER::UpdatePacket()
{
	if (!m_pointsInstant.computed)
		return;
	if (GetSectree() == NULL) return;
	if (!this)
	{
		//sys_err( "UpdateStateMachine - Core Engel!" );
		return;
	}
	
	if (IsPC() && (!GetDesc() || !GetDesc()->GetCharacter()))
	{
		//sys_err( "UpdateStateMachine - Core Engel 2!" );
		return;
	}

	TPacketGCCharacterUpdate pack;
	TPacketGCCharacterUpdate pack2;

	pack.header = HEADER_GC_CHARACTER_UPDATE;
	pack.dwVID = m_vid;

	pack.awPart[CHR_EQUIPPART_ARMOR] = GetPart(PART_MAIN);
	pack.awPart[CHR_EQUIPPART_WEAPON] = GetPart(PART_WEAPON);
	pack.awPart[CHR_EQUIPPART_HEAD] = GetPart(PART_HEAD);
	pack.awPart[CHR_EQUIPPART_HAIR] = GetPart(PART_HAIR);
	#ifdef __SASH_SYSTEM__
	pack.awPart[CHR_EQUIPPART_SASH] = GetPart(PART_SASH);
	#endif
#ifdef ENABLE_AURA_SYSTEM
	pack.awPart[CHR_EQUIPPART_AURA] = GetPart(PART_AURA);
#endif
#ifdef __NEW_ARROW_SYSTEM__
	pack.awPart[CHR_PART_ARROW_TYPE] = GetPart(PART_ARROW_TYPE);
#endif
#ifdef __SKILL_COLOR_SYSTEM__
	memcpy(pack.dwSkillColor, GetSkillColor(), sizeof(pack.dwSkillColor));
#endif
	pack.bMovingSpeed = GetLimitPoint(POINT_MOV_SPEED);
	pack.bMovingSpeed	= GetLimitPoint(POINT_MOV_SPEED);
	pack.bAttackSpeed	= GetLimitPoint(POINT_ATT_SPEED);
	pack.bStateFlag	= m_bAddChrState;
	pack.dwAffectFlag[0] = m_afAffectFlag.bits[0];
	pack.dwAffectFlag[1] = m_afAffectFlag.bits[1];
	pack.dwGuildID	= 0;
	pack.sAlignment	= m_iAlignment / 10;

#ifdef LWT_BUFFI_SYSTEM
	if(IsSupport())
	{
		pack.awPart[CHR_EQUIPPART_ARMOR] = GetSupportArmor();
		pack.awPart[CHR_EQUIPPART_WEAPON] = GetSupportWeapon();
		pack.awPart[CHR_EQUIPPART_HAIR] = GetSupportHair();
	}	
#endif

#ifdef ENABLE_TITLE_SYSTEM
	pack.sPrestige	= m_iPrestige;
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
	pack.dwLevel = GetLevel();
#endif
	pack.dwLevel = GetLevel();
#ifdef LWT_BUFFI_SYSTEM
	if(IsSupport())
		pack.bPKMode = PK_MODE_PROTECT;
	else
		pack.bPKMode = m_bPKMode;
#else
	pack.bPKMode	= m_bPKMode;
#endif

	if (GetGuild())
		pack.dwGuildID = GetGuild()->GetID();

	pack.dwMountVnum = GetMountVnum();

	LPITEM weapon = GetWear(WEAR_WEAPON);
	if (weapon)
		pack.dwWeaponEvolution = weapon->GetEvolution();
	else
		pack.dwWeaponEvolution = 0;

#ifdef ENABLE_KOSTUMPARLA
	LPITEM item = GetWear(WEAR_COSTUME_BODY);
	if (item)
		pack.dwCostumEvo = item->GetSocket(1);
	else
		pack.dwCostumEvo = 0;
#endif

#ifdef ENABLE_REBIRT_SYSTEM
	pack.bRebirt = GetRebirt();
#endif

	pack2 = pack;
#ifdef WJ_SECURITY_SYSTEM
	pack.bActivateSecurity = m_bActivateSecurity;
#endif
	pack2.dwGuildID = 0;
	pack2.sAlignment = 0;
#ifdef ENABLE_TITLE_SYSTEM
	pack2.sPrestige = 0;
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
	pack2.dwLevel = 0;
#endif

	#ifdef ENABLE_REBIRT_SYSTEM
	pack2.bRebirt = 0;
	#endif
	
	pack2.dwWeaponEvolution = 0;

	if (false)
	{
		if (m_bIsObserver != true)
		{
			for (ENTITY_MAP::iterator iter = m_map_view.begin(); iter != m_map_view.end(); iter++)
			{
				LPENTITY pEntity = iter->first;

				if (pEntity != NULL)
				{
					if (pEntity->IsType(ENTITY_CHARACTER) == true)
					{
						if (pEntity->GetDesc() != NULL)
						{
							LPCHARACTER pChar = (LPCHARACTER)pEntity;

							if (GetEmpire() == pChar->GetEmpire() || pChar->GetGMLevel() > GM_PLAYER)
							{
								pEntity->GetDesc()->Packet(&pack, sizeof(pack));
							}
							else
							{
								pEntity->GetDesc()->Packet(&pack2, sizeof(pack2));
							}
						}
					}
					else
					{
						if (pEntity->GetDesc() != NULL)
						{
							pEntity->GetDesc()->Packet(&pack, sizeof(pack));
						}
					}
				}
			}
		}

		if (GetDesc() != NULL)
		{
			GetDesc()->Packet(&pack, sizeof(pack));
		}
	}
	else
	{
		PacketAround(&pack, sizeof(pack));
	}
}

LPCHARACTER CHARACTER::FindCharacterInView(const char * c_pszName, bool bFindPCOnly)
{
	ENTITY_MAP::iterator it = m_map_view.begin();

	for (; it != m_map_view.end(); ++it)
	{
		if (!it->first->IsType(ENTITY_CHARACTER))
			continue;

		LPCHARACTER tch = (LPCHARACTER) it->first;

		if (bFindPCOnly && tch->IsNPC())
			continue;

		if (!strcasecmp(tch->GetName(), c_pszName))
			return (tch);
	}

	return NULL;
}

void CHARACTER::SetPosition(int pos)
{
	if (pos == POS_STANDING)
	{
		REMOVE_BIT(m_bAddChrState, ADD_CHARACTER_STATE_DEAD);
		REMOVE_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_STUN);

		event_cancel(&m_pkDeadEvent);
		event_cancel(&m_pkStunEvent);
	}
	else if (pos == POS_DEAD)
		SET_BIT(m_bAddChrState, ADD_CHARACTER_STATE_DEAD);

	if (!IsStone())
	{
		switch (pos)
		{
			case POS_FIGHTING:
				if (!IsState(m_stateBattle))
					MonsterLog("[BATTLE] �ο�� ����");

				GotoState(m_stateBattle);
				break;

			default:
				if (!IsState(m_stateIdle))
					MonsterLog("[IDLE] ���� ����");

				GotoState(m_stateIdle);
				break;
		}
	}

	m_pointsInstant.position = pos;
}

void CHARACTER::Save()
{
	if (!m_bSkipSave)
		CHARACTER_MANAGER::instance().DelayedSave(this);
}

void CHARACTER::PetGetir()
{
	
	CPetSystem* petSystem = GetPetSystem();
	if (!petSystem)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_GETIR_HATA"));
	}
	else
	{
		
		LPITEM petvnum1 = GetWear(WEAR_PET);
		if (NULL != petvnum1)
		{
			if (USE_PET == petvnum1->GetSubType() && petvnum1->GetType() == ITEM_UNIQUE)
			{
				CPetActor* pet = petSystem->Summon(GetPetAppearance(), petvnum1, " - Pet", false);
			}
		}
	}
}

void CHARACTER::CreatePlayerProto(TPlayerTable & tab)
{
	memset(&tab, 0, sizeof(TPlayerTable));

	if (GetNewName().empty())
	{
		strlcpy(tab.name, GetName(), sizeof(tab.name));
	}
	else
	{
		strlcpy(tab.name, GetNewName().c_str(), sizeof(tab.name));
	}

	strlcpy(tab.ip, GetDesc()->GetHostName(), sizeof(tab.ip));

	tab.id			= m_dwPlayerID;
	tab.voice		= GetPoint(POINT_VOICE);
	tab.level		= GetLevel();
	tab.level_step	= GetPoint(POINT_LEVEL_STEP);
	tab.exp			= GetExp();
	tab.gold		= GetGold();
	tab.job			= m_points.job;
	tab.part_base	= m_pointsInstant.bBasePart;
	tab.skill_group	= m_points.skill_group;
#ifdef ENABLE_TURN_BACK_SYSTEM
	tab.dungeonindex = m_points.dungeonindex;
#endif
	tab.horse_appearance = m_points.horse_appearance;
	tab.pet_appearance = m_points.pet_appearance;
	tab.petvnum = m_points.petvnum;
	tab.petslot = m_points.petslot;
#ifdef ENABLE_CHEQUE_SYSTEM
	tab.cheque		= GetCheque();
#endif
	#ifdef ENABLE_REBIRT_SYSTEM
	tab.rebirt		= GetRebirt();
	#endif
	tab.gaya		= GetGaya();
	tab.red_gaya		= GetRedGaya();
	DWORD dwPlayedTime = (get_dword_time() - m_dwPlayStartTime);

	if (dwPlayedTime > 60000)
	{
		if (GetSectree() && !GetSectree()->IsAttr(GetX(), GetY(), ATTR_BANPK))
		{
			if (GetRealAlignment() < 0)
			{
				if (IsEquipUniqueItem(UNIQUE_ITEM_FASTER_ALIGNMENT_UP_BY_TIME))
					UpdateAlignment(120 * (dwPlayedTime / 60000));
				else
					UpdateAlignment(60 * (dwPlayedTime / 60000));
			}
			else
				UpdateAlignment(5 * (dwPlayedTime / 60000));
		}

		SetRealPoint(POINT_PLAYTIME, GetRealPoint(POINT_PLAYTIME) + dwPlayedTime / 60000);
		ResetPlayTime(dwPlayedTime % 60000);
	}

	tab.playtime = GetRealPoint(POINT_PLAYTIME);
	tab.lAlignment = m_iRealAlignment;
#ifdef ENABLE_TITLE_SYSTEM
	tab.lPrestige = m_iRealPrestige;
#endif
	if (m_posWarp.x != 0 || m_posWarp.y != 0)
	{
		tab.x = m_posWarp.x;
		tab.y = m_posWarp.y;
		tab.z = 0;
		tab.lMapIndex = m_lWarpMapIndex;
	}
	else
	{
		tab.x = GetX();
		tab.y = GetY();
		tab.z = GetZ();
		tab.lMapIndex	= GetMapIndex();
	}

	if (m_lExitMapIndex == 0)
	{
		tab.lExitMapIndex	= tab.lMapIndex;
		tab.lExitX		= tab.x;
		tab.lExitY		= tab.y;
	}
	else
	{
		tab.lExitMapIndex	= m_lExitMapIndex;
		tab.lExitX		= m_posExit.x;
		tab.lExitY		= m_posExit.y;
	}

	sys_log(0, "SAVE: %s %dx%d", GetName(), tab.x, tab.y);

	tab.st = GetRealPoint(POINT_ST);
	tab.ht = GetRealPoint(POINT_HT);
	tab.dx = GetRealPoint(POINT_DX);
	tab.iq = GetRealPoint(POINT_IQ);

	tab.stat_point = GetPoint(POINT_STAT);
	tab.skill_point = GetPoint(POINT_SKILL);
	tab.sub_skill_point = GetPoint(POINT_SUB_SKILL);
	tab.horse_skill_point = GetPoint(POINT_HORSE_SKILL);

	tab.stat_reset_count = GetPoint(POINT_STAT_RESET_COUNT);

	tab.hp = GetHP();
	tab.sp = GetSP();

	tab.stamina = GetStamina();

	tab.sRandomHP = m_points.iRandomHP;
	tab.sRandomSP = m_points.iRandomSP;
#ifdef ENABLE_KILL_STATISTICS
	tab.iDuelsWon = GetDuelsWon();
	tab.iDuelsLost = GetDuelsLost();
	tab.iBossesKills = GetBossesKills();
	tab.iStonesKills = GetStonesKills();
#endif
	for (int i = 0; i < QUICKSLOT_MAX_NUM; ++i)
		tab.quickslot[i] = m_quickslot[i];

	if (m_stMobile.length() && !*m_szMobileAuth)
		strlcpy(tab.szMobile, m_stMobile.c_str(), sizeof(tab.szMobile));

	thecore_memcpy(tab.parts, m_pointsInstant.parts, sizeof(tab.parts));

	// REMOVE_REAL_SKILL_LEVLES
	thecore_memcpy(tab.skills, m_pSkillLevels, sizeof(TPlayerSkill) * SKILL_MAX_NUM);
	// END_OF_REMOVE_REAL_SKILL_LEVLES

	tab.horse = GetHorseData();
}


void CHARACTER::SaveReal()
{
	if (m_bSkipSave)
		return;

	if (!GetDesc())
	{
		sys_err("Character::Save : no descriptor when saving (name: %s)", GetName());
		return;
	}

	TPlayerTable table;
	CreatePlayerProto(table);

	db_clientdesc->DBPacket(HEADER_GD_PLAYER_SAVE, GetDesc()->GetHandle(), &table, sizeof(TPlayerTable));

	quest::PC * pkQuestPC = quest::CQuestManager::instance().GetPCForce(GetPlayerID());

	if (!pkQuestPC)
		sys_err("CHARACTER::Save : null quest::PC pointer! (name %s)", GetName());
	else
	{
		pkQuestPC->Save();
	}

	marriage::TMarriage* pMarriage = marriage::CManager::instance().Get(GetPlayerID());
	if (pMarriage)
		pMarriage->Save();
}

void CHARACTER::FlushDelayedSaveItem()
{
	// ???? ??? ??????? ???? ????????.
	LPITEM item;

	for (int i = 0; i < INVENTORY_AND_EQUIP_SLOT_MAX; ++i)
		if ((item = GetInventoryItem(i)))
			ITEM_MANAGER::instance().FlushDelayedSave(item);
}

void CHARACTER::Disconnect(const char * c_pszReason)
{
	assert(GetDesc() != NULL);

	sys_log(0, "DISCONNECT: %s (%s)", GetName(), c_pszReason ? c_pszReason : "unset" );

	if (GetShop())
	{
		GetShop()->RemoveGuest(this);
		SetShop(NULL);
	}

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (GetOfflineShop())
	{
		GetOfflineShop()->RemoveGuest(this);
		SetOfflineShop(NULL);
	}
#endif
	if (GetArena() != NULL)
	{
		GetArena()->OnDisconnect(GetPlayerID());
	}

	if (GetParty() != NULL)
	{
		GetParty()->UpdateOfflineState(GetPlayerID());
	}

	marriage::CManager::instance().Logout(this);
#ifdef OTOMATIK_AV
	if (IsAffectFlag(AFF_OTOAV_AKTIF)) { RemoveAffect(AFFECT_OTOAV_AKTIF); }
#endif
	// P2P Logout
	TPacketGGLogout p;
	p.bHeader = HEADER_GG_LOGOUT;
	strlcpy(p.szName, GetName(), sizeof(p.szName));
	P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGLogout));
	char buf[51];
	snprintf(buf, sizeof(buf), "%s %d %d %ld %d", 
		inet_ntoa(GetDesc()->GetAddr().sin_addr), GetGold(), g_bChannel, GetMapIndex(), GetAlignment());

	LogManager::instance().CharLog(this, 0, "LOGOUT", buf);

	if (LC_IsYMIR() || LC_IsKorea() || LC_IsBrazil())
	{
		long playTime = GetRealPoint(POINT_PLAYTIME) - m_dwLoginPlayTime;
		LogManager::instance().LoginLog(false, GetDesc()->GetAccountTable().id, GetPlayerID(), GetLevel(), GetJob(), playTime);

		if (LC_IsBrazil() != true)
			CPCBangManager::instance().Log(GetDesc()->GetHostName(), GetPlayerID(), playTime);
	}

	if (m_pWarMap)
		SetWarMap(NULL);

	if (m_pWeddingMap)
	{
		SetWeddingMap(NULL);
	}

#ifdef __ENABLE_NEW_OFFLINESHOP__
	offlineshop::GetManager().RemoveSafeboxFromCache(GetPlayerID());
	offlineshop::GetManager().RemoveGuestFromShops(this);
	
	if(m_pkAuctionGuest)
		m_pkAuctionGuest->RemoveGuest(this);

	if (GetOfflineShop())
		SetOfflineShop(NULL);

	SetShopSafebox(NULL);

	m_pkAuction		= NULL;
	m_pkAuctionGuest= NULL;

	//offlineshop-updated 05/08/19
	m_bIsLookingOfflineshopOfferList = false;
#endif
	if (GetGuild())
		GetGuild()->LogoutMember(this);

	quest::CQuestManager::instance().LogoutPC(this);

	if (GetParty())
		GetParty()->Unlink(this);

	// ????? ?? ????????? ????? ??? ???
	if (IsStun() || IsDead())
	{
		DeathPenalty(0);
		PointChange(POINT_HP, 50 - GetHP());
	}


	if (!CHARACTER_MANAGER::instance().FlushDelayedSave(this))
	{
		SaveReal();
	}

	FlushDelayedSaveItem();

	SaveAffect();
	m_bIsLoadedAffect = false;

	m_bSkipSave = true; // ?? ???��??? ????? ??????? ????.

	quest::CQuestManager::instance().DisconnectPC(this);

	CloseSafebox();

	CloseMall();

	CPVPManager::instance().Disconnect(this);

	CTargetManager::instance().Logout(GetPlayerID());

	MessengerManager::instance().Logout(GetName());

	if (g_TeenDesc)
	{
		int		offset = 0;
		char	buf[245] = {0};

		buf[0] = HEADER_GT_LOGOUT;
		offset += 1;

		memset(buf+offset, 0x00, 2);
		offset += 2;

		TAccountTable	&acc_table = GetDesc()->GetAccountTable();
		memcpy(buf+offset, &acc_table.id, 4);
		offset += 4;

		g_TeenDesc->Packet(buf, offset);
	}

	if (GetDesc())
	{
		GetDesc()->BindCharacter(NULL);
//		BindDesc(NULL);
	}

	CXTrapManager::instance().DestroyClientSession(this);

	M2_DESTROY_CHARACTER(this);
}

bool CHARACTER::Show(long lMapIndex, long x, long y, long z, bool bShowSpawnMotion/* = false */)
{
	LPSECTREE sectree = SECTREE_MANAGER::instance().Get(lMapIndex, x, y);

	if (!sectree)
	{
		sys_log(0, "cannot find sectree by %dx%d mapindex %d", x, y, lMapIndex);
		return false;
	}

	SetMapIndex(lMapIndex);

	bool bChangeTree = false;

	if (!GetSectree() || GetSectree() != sectree)
		bChangeTree = true;

	if (bChangeTree)
	{
		if (GetSectree())
			GetSectree()->RemoveEntity(this);

		ViewCleanup();
	}

	if (!IsNPC())
	{
		sys_log(0, "SHOW: %s %dx%dx%d", GetName(), x, y, z);
		if (GetStamina() < GetMaxStamina())
			StartAffectEvent();
	}
	else if (m_pkMobData)
	{
		m_pkMobInst->m_posLastAttacked.x = x;
		m_pkMobInst->m_posLastAttacked.y = y;
		m_pkMobInst->m_posLastAttacked.z = z;
	}

	if (bShowSpawnMotion)
	{
		SET_BIT(m_bAddChrState, ADD_CHARACTER_STATE_SPAWN);
		m_afAffectFlag.Set(AFF_SPAWN);
	}

	SetXYZ(x, y, z);

	m_posDest.x = x;
	m_posDest.y = y;
	m_posDest.z = z;

	m_posStart.x = x;
	m_posStart.y = y;
	m_posStart.z = z;

	if (bChangeTree)
	{
		EncodeInsertPacket(this);
		sectree->InsertEntity(this);

		UpdateSectree();
	}
	else
	{
		ViewReencode();
		sys_log(0, "      in same sectree");
	}

	REMOVE_BIT(m_bAddChrState, ADD_CHARACTER_STATE_SPAWN);
	
	SetValidComboInterval(0);
	return true;
}

// BGM_INFO
struct BGMInfo
{
	std::string	name;
	float		vol;
};

typedef std::map<unsigned, BGMInfo> BGMInfoMap;

static BGMInfoMap 	gs_bgmInfoMap;
static bool		gs_bgmVolEnable = false;

void CHARACTER_SetBGMVolumeEnable()
{
	gs_bgmVolEnable = true;	
	sys_log(0, "bgm_info.set_bgm_volume_enable");
}

void CHARACTER_AddBGMInfo(unsigned mapIndex, const char* name, float vol)
{
	BGMInfo newInfo;
	newInfo.name = name;
	newInfo.vol = vol;

	gs_bgmInfoMap[mapIndex] = newInfo;

	sys_log(0, "bgm_info.add_info(%d, '%s', %f)", mapIndex, name, vol);
}

const BGMInfo& CHARACTER_GetBGMInfo(unsigned mapIndex)
{
	BGMInfoMap::iterator f = gs_bgmInfoMap.find(mapIndex);
	if (gs_bgmInfoMap.end() == f)
	{
		static BGMInfo s_empty = {"", 0.0f};
		return s_empty;
	}
	return f->second;
}

bool CHARACTER_IsBGMVolumeEnable()
{
	return gs_bgmVolEnable;
}
// END_OF_BGM_INFO

void CHARACTER::MainCharacterPacket()
{
	const unsigned mapIndex = GetMapIndex();
	const BGMInfo& bgmInfo = CHARACTER_GetBGMInfo(mapIndex);

	// SUPPORT_BGM
	if (!bgmInfo.name.empty())
	{
		if (CHARACTER_IsBGMVolumeEnable())
		{
			sys_log(1, "bgm_info.play_bgm_vol(%d, name='%s', vol=%f)", mapIndex, bgmInfo.name.c_str(), bgmInfo.vol);
			TPacketGCMainCharacter4_BGM_VOL mainChrPacket;
			mainChrPacket.header = HEADER_GC_MAIN_CHARACTER4_BGM_VOL;
			mainChrPacket.dwVID = m_vid;
			mainChrPacket.wRaceNum = GetRaceNum();
			mainChrPacket.lx = GetX();
			mainChrPacket.ly = GetY();
			mainChrPacket.lz = GetZ();
			mainChrPacket.empire = GetDesc()->GetEmpire();
			mainChrPacket.skill_group = GetSkillGroup();
			strlcpy(mainChrPacket.szChrName, GetName(), sizeof(mainChrPacket.szChrName));

			mainChrPacket.fBGMVol = bgmInfo.vol;
			strlcpy(mainChrPacket.szBGMName, bgmInfo.name.c_str(), sizeof(mainChrPacket.szBGMName));
			GetDesc()->Packet(&mainChrPacket, sizeof(TPacketGCMainCharacter4_BGM_VOL));
		}
		else
		{
			sys_log(1, "bgm_info.play(%d, '%s')", mapIndex, bgmInfo.name.c_str());
			TPacketGCMainCharacter3_BGM mainChrPacket;
			mainChrPacket.header = HEADER_GC_MAIN_CHARACTER3_BGM;
			mainChrPacket.dwVID = m_vid;
			mainChrPacket.wRaceNum = GetRaceNum();
			mainChrPacket.lx = GetX();
			mainChrPacket.ly = GetY();
			mainChrPacket.lz = GetZ();
			mainChrPacket.empire = GetDesc()->GetEmpire();
			mainChrPacket.skill_group = GetSkillGroup();
			strlcpy(mainChrPacket.szChrName, GetName(), sizeof(mainChrPacket.szChrName));
			strlcpy(mainChrPacket.szBGMName, bgmInfo.name.c_str(), sizeof(mainChrPacket.szBGMName));
			GetDesc()->Packet(&mainChrPacket, sizeof(TPacketGCMainCharacter3_BGM));
		}
		//if (m_stMobile.length())
		//		ChatPacket(CHAT_TYPE_COMMAND, "sms");
	}
	// END_OF_SUPPORT_BGM
	else
	{
		sys_log(0, "bgm_info.play(%d, DEFAULT_BGM_NAME)", mapIndex);

		TPacketGCMainCharacter pack;
		pack.header = HEADER_GC_MAIN_CHARACTER;
		pack.dwVID = m_vid;
		pack.wRaceNum = GetRaceNum();
		pack.lx = GetX();
		pack.ly = GetY();
		pack.lz = GetZ();
		pack.empire = GetDesc()->GetEmpire();
		pack.skill_group = GetSkillGroup();
		strlcpy(pack.szName, GetName(), sizeof(pack.szName));
		GetDesc()->Packet(&pack, sizeof(TPacketGCMainCharacter));

		if (m_stMobile.length())
			ChatPacket(CHAT_TYPE_COMMAND, "sms");
	}
}

void CHARACTER::PointsPacket()
{
	if (!GetDesc())
		return;

	TPacketGCPoints pack;

	pack.header	= HEADER_GC_CHARACTER_POINTS;

	pack.points[POINT_LEVEL]		= GetLevel();
	pack.points[POINT_EXP]		= GetExp();
	pack.points[POINT_NEXT_EXP]		= GetNextExp();
	pack.points[POINT_HP]		= GetHP();
	pack.points[POINT_MAX_HP]		= GetMaxHP();
	pack.points[POINT_SP]		= GetSP();
	pack.points[POINT_MAX_SP]		= GetMaxSP();
#ifndef YANG_LIMIT
	pack.points[POINT_GOLD] = GetGold();
#endif
	pack.points[POINT_STAMINA]		= GetStamina();
	pack.points[POINT_MAX_STAMINA]	= GetMaxStamina();
#ifdef ENABLE_CHEQUE_SYSTEM
	pack.points[POINT_CHEQUE]		= GetCheque();
#endif
	#ifdef ENABLE_REBIRT_SYSTEM
	pack.points[POINT_REBIRT] = GetRebirt();
	#endif
	pack.points[POINT_GAYA] = GetGaya();
	pack.points[POINT_RED_GAYA] = GetRedGaya();
	for (int i = POINT_ST; i < POINT_MAX_NUM; ++i)
		pack.points[i] = GetPoint(i);

	GetDesc()->Packet(&pack, sizeof(TPacketGCPoints));

#ifdef YANG_LIMIT
	TPacketGCGold pGold;
	pGold.bHeader = HEADER_GC_GOLD;
	pGold.lGold = GetGold();
	GetDesc()->Packet(&pGold, sizeof(TPacketGCGold));
#endif
}

bool CHARACTER::ChangeSex()
{
	int src_race = GetRaceNum();

	switch (src_race)
	{
		case MAIN_RACE_WARRIOR_M:
			m_points.job = MAIN_RACE_WARRIOR_W;
			break;

		case MAIN_RACE_WARRIOR_W:
			m_points.job = MAIN_RACE_WARRIOR_M;
			break;

		case MAIN_RACE_ASSASSIN_M:
			m_points.job = MAIN_RACE_ASSASSIN_W;
			break;

		case MAIN_RACE_ASSASSIN_W:
			m_points.job = MAIN_RACE_ASSASSIN_M;
			break;

		case MAIN_RACE_SURA_M:
			m_points.job = MAIN_RACE_SURA_W;
			break;

		case MAIN_RACE_SURA_W:
			m_points.job = MAIN_RACE_SURA_M;
			break;

		case MAIN_RACE_SHAMAN_M:
			m_points.job = MAIN_RACE_SHAMAN_W;
			break;

		case MAIN_RACE_SHAMAN_W:
			m_points.job = MAIN_RACE_SHAMAN_M;
			break;

		case MAIN_RACE_WOLFMAN_M:
			m_points.job = MAIN_RACE_WOLFMAN_M;
			break;
		default:
			sys_err("CHANGE_SEX: %s unknown race %d", GetName(), src_race);
			return false;
	}

	sys_log(0, "CHANGE_SEX: %s (%d -> %d)", GetName(), src_race, m_points.job);
	return true;
}

WORD CHARACTER::GetRaceNum() const
{
	if (m_dwPolymorphRace)
		return m_dwPolymorphRace;

	if (m_pkMobData)
		return m_pkMobData->m_table.dwVnum;

	return m_points.job;
}

void CHARACTER::SetRace(BYTE race)
{
	if (race >= MAIN_RACE_MAX_NUM)
	{
		sys_err("CHARACTER::SetRace(name=%s, race=%d).OUT_OF_RACE_RANGE", GetName(), race);
		return;
	}

	m_points.job = race;
}

BYTE CHARACTER::GetJob() const
{
	unsigned race = m_points.job;
	unsigned job;

	if (RaceToJob(race, &job))
		return job;

	sys_err("CHARACTER::GetJob(name=%s, race=%d).OUT_OF_RACE_RANGE", GetName(), race);
	return JOB_WARRIOR;
}

void CHARACTER::SetLevel(BYTE level)
{
	m_points.level = level;

	if (IsPC())
	{
		if (level < PK_PROTECT_LEVEL)
			SetPKMode(PK_MODE_PROTECT);
		else if (GetGMLevel() != GM_PLAYER)
			SetPKMode(PK_MODE_PROTECT);
		else if (m_bPKMode == PK_MODE_PROTECT)
			SetPKMode(PK_MODE_PEACE);
	}
}

#ifdef ENABLE_REBIRT_SYSTEM
void CHARACTER::SetRebirt(BYTE rebirt)
{
	m_points.rebirt = rebirt;

	if (IsPC())
	{
		Save();
		UpdatePacket();
	}
	else
	{
		sys_err("rebirt hata");
	}
		
}
#endif

void CHARACTER::SetEmpire(BYTE bEmpire)
{
	m_bEmpire = bEmpire;
}

void CHARACTER::SetPlayerProto(const TPlayerTable * t)
{
	if (!GetDesc() || !*GetDesc()->GetHostName())
		sys_err("cannot get desc or hostname");
	else
		SetGMLevel();

	m_bCharType = CHAR_TYPE_PC;

	m_dwPlayerID = t->id;

	m_iAlignment = t->lAlignment;
	m_iRealAlignment = t->lAlignment;
#ifdef ENABLE_TITLE_SYSTEM
	m_iPrestige = t->lPrestige;
	m_iRealPrestige = t->lPrestige;
#endif
	m_points.voice = t->voice;

	m_points.skill_group = t->skill_group; 

	m_pointsInstant.bBasePart = t->part_base;
	SetPart(PART_HAIR, t->parts[PART_HAIR]);
	#ifdef __SASH_SYSTEM__
	SetPart(PART_SASH, t->parts[PART_SASH]);
	#endif
#ifdef ENABLE_AURA_SYSTEM
	SetPart(PART_AURA, t->parts[PART_AURA]);
#endif
	m_points.iRandomHP = t->sRandomHP;
	m_points.iRandomSP = t->sRandomSP;
#ifdef ENABLE_KILL_STATISTICS
	m_killstatistics.iDuelsWon = t->iDuelsWon;
	m_killstatistics.iDuelsLost = t->iDuelsLost;
	m_killstatistics.iBossesKills = t->iBossesKills;
	m_killstatistics.iStonesKills = t->iStonesKills;
#endif
#ifdef ENABLE_TURN_BACK_SYSTEM
	m_points.dungeonindex = t->dungeonindex;
#endif
	m_points.horse_appearance = t->horse_appearance;
	m_points.pet_appearance = t->pet_appearance;
	m_points.petvnum = t->petvnum;
	m_points.petslot = t->petslot;
	// REMOVE_REAL_SKILL_LEVLES
	if (m_pSkillLevels)
		M2_DELETE_ARRAY(m_pSkillLevels);

	m_pSkillLevels = M2_NEW TPlayerSkill[SKILL_MAX_NUM];
	thecore_memcpy(m_pSkillLevels, t->skills, sizeof(TPlayerSkill) * SKILL_MAX_NUM);
	// END_OF_REMOVE_REAL_SKILL_LEVLES

	if (t->lMapIndex >= 10000)
	{
		m_posWarp.x = t->lExitX;
		m_posWarp.y = t->lExitY;
		m_lWarpMapIndex = t->lExitMapIndex;
	}

	SetRealPoint(POINT_PLAYTIME, t->playtime);
	m_dwLoginPlayTime = t->playtime;
	SetRealPoint(POINT_ST, t->st);
	SetRealPoint(POINT_HT, t->ht);
	SetRealPoint(POINT_DX, t->dx);
	SetRealPoint(POINT_IQ, t->iq);

	SetPoint(POINT_ST, t->st);
	SetPoint(POINT_HT, t->ht);
	SetPoint(POINT_DX, t->dx);
	SetPoint(POINT_IQ, t->iq);

	SetPoint(POINT_STAT, t->stat_point);
	SetPoint(POINT_SKILL, t->skill_point);
	SetPoint(POINT_SUB_SKILL, t->sub_skill_point);
	SetPoint(POINT_HORSE_SKILL, t->horse_skill_point);

	SetPoint(POINT_STAT_RESET_COUNT, t->stat_reset_count);

	SetPoint(POINT_LEVEL_STEP, t->level_step);
	SetRealPoint(POINT_LEVEL_STEP, t->level_step);

	SetRace(t->job);

	SetLevel(t->level);
	SetExp(t->exp);
	SetGold(t->gold);
#ifdef ENABLE_CHEQUE_SYSTEM
	SetCheque(t->cheque);
#endif
	#ifdef ENABLE_REBIRT_SYSTEM
	SetRebirt(t->rebirt);
	#endif
	SetGaya(t->gaya);
	SetRedGaya(t->red_gaya);
	SetMapIndex(t->lMapIndex);
	SetXYZ(t->x, t->y, t->z);

	ComputePoints();

	SetHP(t->hp);
	SetSP(t->sp);
	SetStamina(t->stamina);

	//GM�϶� ��ȣ���
	if (!test_server)
	{
		if (GetGMLevel() > GM_LOW_WIZARD)
		{
			m_afAffectFlag.Set(AFF_YMIR);
			m_bPKMode = PK_MODE_PROTECT;
		}
	}

	if (GetLevel() < PK_PROTECT_LEVEL)
		m_bPKMode = PK_MODE_PROTECT;

	m_stMobile = t->szMobile;

	SetHorseData(t->horse);

	if (GetHorseLevel() > 0)
		UpdateHorseDataByLogoff(t->logoff_interval);

	thecore_memcpy(m_aiPremiumTimes, t->aiPremiumTimes, sizeof(t->aiPremiumTimes));

	m_dwLogOffInterval = t->logoff_interval;

	sys_log(0, "PLAYER_LOAD: %s PREMIUM %d %d, LOGGOFF_INTERVAL %u PTR: %p", t->name, m_aiPremiumTimes[0], m_aiPremiumTimes[1], t->logoff_interval, this);


	if (GetGMLevel() != GM_PLAYER) 
	{
		LogManager::instance().CharLog(this, GetGMLevel(), "GM_LOGIN", "");
		sys_log(0, "GM_LOGIN(gmlevel=%d, name=%s(%d), pos=(%d, %d)", GetGMLevel(), GetName(), GetPlayerID(), GetX(), GetY());
	}

#ifdef LWT_BUFFI_SYSTEM
	if (m_supportSystem)
	{
		m_supportSystem->Destroy();
		delete m_supportSystem;
	}

	m_supportSystem = M2_NEW CSupportSystem(this);
#endif

#ifdef __PET_SYSTEM__
	// NOTE: �ϴ� ĳ���Ͱ� PC�� ��쿡�� PetSystem�� ������ ��. ���� �ӽŴ� �޸� ���������� NPC���� �ϱ� ��..
	if (m_petSystem)
	{
		m_petSystem->Destroy();
		delete m_petSystem;
	}

	m_petSystem = M2_NEW CPetSystem(this);
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
	if (m_newpetSystem)
	{
		m_newpetSystem->Destroy();
		delete m_newpetSystem;
	}

	m_newpetSystem = M2_NEW CNewPetSystem(this);
#endif
	SetOfflineShopVID(COfflineShopManager::instance().FindMyOfflineShop(GetPlayerID()));
}

EVENTFUNC(kill_ore_load_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info );
	if ( info == NULL )
	{
		sys_err( "kill_ore_load_even> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER	ch = info->ch;
	if (ch == NULL) { // <Factor>
		return 0;
	}	

	ch->m_pkMiningEvent = NULL;
	M2_DESTROY_CHARACTER(ch);
	return 0;
}

void CHARACTER::SetProto(const CMob * pkMob)
{
	if (m_pkMobInst)
		M2_DELETE(m_pkMobInst);

	m_pkMobData = pkMob;
	m_pkMobInst = M2_NEW CMobInstance;

	m_bPKMode = PK_MODE_FREE;

	const TMobTable * t = &m_pkMobData->m_table;

	m_bCharType = t->bType;

	SetLevel(t->bLevel);
	SetEmpire(t->bEmpire);

	SetExp(t->dwExp);
	SetRealPoint(POINT_ST, t->bStr);
	SetRealPoint(POINT_DX, t->bDex);
	SetRealPoint(POINT_HT, t->bCon);
	SetRealPoint(POINT_IQ, t->bInt);

	ComputePoints();

	SetHP(GetMaxHP());
	SetSP(GetMaxSP());

	////////////////////
	m_pointsInstant.dwAIFlag = t->dwAIFlag;
	SetImmuneFlag(t->dwImmuneFlag);

	AssignTriggers(t);

	ApplyMobAttribute(t);

	if (IsStone())
	{
		DetermineDropMetinStone();
	}

	if (IsWarp() || IsGoto())
	{
		StartWarpNPCEvent();
	}

	CHARACTER_MANAGER::instance().RegisterRaceNumMap(this);

	// XXX X-mas santa hardcoding
	if (GetRaceNum() == xmas::MOB_SANTA_VNUM)
	{
		SetPoint(POINT_ATT_GRADE_BONUS, 10);
		if (g_iUseLocale)
			SetPoint(POINT_DEF_GRADE_BONUS, 6);
		else
			SetPoint(POINT_DEF_GRADE_BONUS, 15);

		//��Ÿ��
		//m_dwPlayStartTime = get_dword_time() + 10 * 60 * 1000;
		//�ż��� ���� 
		m_dwPlayStartTime = get_dword_time() + 30 * 1000;
		if (test_server)
			m_dwPlayStartTime = get_dword_time() + 30 * 1000;
	}

	// XXX CTF GuildWar hardcoding
	if (warmap::IsWarFlag(GetRaceNum()))
	{
		m_stateIdle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlag, &CHARACTER::EndStateEmpty);
		m_stateMove.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlag, &CHARACTER::EndStateEmpty);
		m_stateBattle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlag, &CHARACTER::EndStateEmpty);
	}

	if (warmap::IsWarFlagBase(GetRaceNum()))
	{
		m_stateIdle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlagBase, &CHARACTER::EndStateEmpty);
		m_stateMove.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlagBase, &CHARACTER::EndStateEmpty);
		m_stateBattle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateFlagBase, &CHARACTER::EndStateEmpty);
	}

	if (m_bCharType == CHAR_TYPE_HORSE || 
			GetRaceNum() == 20101 ||
			GetRaceNum() == 20102 ||
			GetRaceNum() == 20103 ||
			GetRaceNum() == 20104 ||
			GetRaceNum() == 20105 ||
			GetRaceNum() == 20106 ||
			GetRaceNum() == 20107 ||
			GetRaceNum() == 20108 ||
			GetRaceNum() == 20109 ||
			CMobVnumHelper::Binekmi(GetRaceNum())
	)
	{
		m_stateIdle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateHorse, &CHARACTER::EndStateEmpty);
		m_stateMove.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateMove, &CHARACTER::EndStateEmpty);
		m_stateBattle.Set(this, &CHARACTER::BeginStateEmpty, &CHARACTER::StateHorse, &CHARACTER::EndStateEmpty);
	}

	// MINING
	if (mining::IsVeinOfOre (GetRaceNum()))
	{
		char_event_info* info = AllocEventInfo<char_event_info>();

		info->ch = this;

		m_pkMiningEvent = event_create(kill_ore_load_event, info, PASSES_PER_SEC(number(7 * 60, 240 * 60)));
	}
	// END_OF_MINING
}

const TMobTable & CHARACTER::GetMobTable() const
{
	return m_pkMobData->m_table;
}

bool CHARACTER::IsRaceFlag(DWORD dwBit) const
{
	return m_pkMobData ? IS_SET(m_pkMobData->m_table.dwRaceFlag, dwBit) : 0;
}

DWORD CHARACTER::GetMobDamageMin() const
{
	return m_pkMobData->m_table.dwDamageRange[0];
}

DWORD CHARACTER::GetMobDamageMax() const
{
	return m_pkMobData->m_table.dwDamageRange[1];
}

float CHARACTER::GetMobDamageMultiply() const
{
	float fDamMultiply = GetMobTable().fDamMultiply;

	if (IsBerserk())
		fDamMultiply = fDamMultiply * 2.0f; // BALANCE: ����ȭ �� �ι�

	return fDamMultiply;
}

DWORD CHARACTER::GetMobDropItemVnum() const
{
	return m_pkMobData->m_table.dwDropItemVnum;
}

bool CHARACTER::IsSummonMonster() const
{
	return GetSummonVnum() != 0;
}

DWORD CHARACTER::GetSummonVnum() const
{
	return m_pkMobData ? m_pkMobData->m_table.dwSummonVnum : 0;
}

DWORD CHARACTER::GetPolymorphItemVnum() const
{
	return m_pkMobData ? m_pkMobData->m_table.dwPolymorphItemVnum : 0;
}

DWORD CHARACTER::GetMonsterDrainSPPoint() const
{
	return m_pkMobData ? m_pkMobData->m_table.dwDrainSP : 0;
}

BYTE CHARACTER::GetMobRank() const
{
	if (!m_pkMobData)
		return MOB_RANK_KNIGHT;	// PC�� ��� KNIGHT��

	return m_pkMobData->m_table.bRank;
}

BYTE CHARACTER::GetMobSize() const
{
	if (!m_pkMobData)
		return MOBSIZE_MEDIUM;

	return m_pkMobData->m_table.bSize;
}

WORD CHARACTER::GetMobAttackRange() const
{ 
	switch (GetMobBattleType())
	{
		case BATTLE_TYPE_RANGE:
		case BATTLE_TYPE_MAGIC:
			return m_pkMobData->m_table.wAttackRange + GetPoint(POINT_BOW_DISTANCE);  
		default:
			return m_pkMobData->m_table.wAttackRange; 
	}
}

BYTE CHARACTER::GetMobBattleType() const
{
	if (!m_pkMobData)
		return BATTLE_TYPE_MELEE;

	return (m_pkMobData->m_table.bBattleType);
}

void CHARACTER::ComputeBattlePoints()
{
	if (IsPolymorphed())
	{
		DWORD dwMobVnum = GetPolymorphVnum();
		const CMob * pMob = CMobManager::instance().Get(dwMobVnum);
		int iAtt = 0;
		int iDef = 0;

		if (pMob)
		{
			iAtt = GetLevel() * 3 + GetPolymorphPoint(POINT_ST) * 4;
			// lev + con
			iDef = GetLevel() + GetPolymorphPoint(POINT_HT) + pMob->m_table.wDef;
		}

		SetPoint(POINT_ATT_GRADE, iAtt);
		SetPoint(POINT_DEF_GRADE, iDef);
		SetPoint(POINT_MAGIC_ATT_GRADE, GetPoint(POINT_ATT_GRADE)); 
		SetPoint(POINT_MAGIC_DEF_GRADE, GetPoint(POINT_DEF_GRADE));
	}
	else if (IsPC())
	{
		SetPoint(POINT_ATT_GRADE, 0);
		SetPoint(POINT_DEF_GRADE, 0);
		SetPoint(POINT_CLIENT_DEF_GRADE, 0);
		SetPoint(POINT_MAGIC_ATT_GRADE, GetPoint(POINT_ATT_GRADE));
		SetPoint(POINT_MAGIC_DEF_GRADE, GetPoint(POINT_DEF_GRADE));

		//
		// �⺻ ATK = 2lev + 2str, ������ ���� 2str�� �ٲ� �� ����
		//
		int64_t iAtk = GetLevel() * 2;
		int64_t iStatAtk = 0;

		switch (GetJob())
		{
			case JOB_WARRIOR:
			case JOB_SURA:
				iStatAtk = (2 * GetPoint(POINT_ST));
				break;

			case JOB_ASSASSIN:
				iStatAtk = (4 * GetPoint(POINT_ST) + 2 * GetPoint(POINT_DX)) / 3;
				break;

			case JOB_SHAMAN:
				iStatAtk = (4 * GetPoint(POINT_ST) + 2 * GetPoint(POINT_IQ)) / 3;
				break;

			case JOB_WOLFMAN:
				iStatAtk = (2 * GetPoint(POINT_ST));
				break;
			default:
				sys_err("invalid job %d", GetJob());
				iStatAtk = (2 * GetPoint(POINT_ST));
				break;
		}

		// ���� Ÿ�� �ְ�, �������� ���� ���ݷ��� ST*2 ���� ������ ST*2�� �Ѵ�.
		// ������ �߸� ���� ��� ���ݷ��� �� ���� �ʰ� �ϱ� ���ؼ���.
		if (GetMountVnum() && iStatAtk < 2 * GetPoint(POINT_ST))
			iStatAtk = (2 * GetPoint(POINT_ST));

		iAtk += iStatAtk;

		// �¸�(��) : �˼��� ������ ����  
		if (GetMountVnum())
		{
			if (GetJob() == JOB_SURA && GetSkillGroup() == 1)
			{
				iAtk += (iAtk * GetHorseLevel()) / 60;
			}
			else
			{
				iAtk += (iAtk * GetHorseLevel()) / 30;
			}
		}
		
		//
		// ATK Setting
		//
		iAtk += GetPoint(POINT_ATT_GRADE_BONUS);

		PointChange(POINT_ATT_GRADE, iAtk);

		// DEF = LEV + CON + ARMOR
		int iShowDef = GetLevel() + GetPoint(POINT_HT); // For Ymir(õ��)
		int iDef = GetLevel() + (int) (GetPoint(POINT_HT) / 1.25); // For Other
		int iArmor = 0;

		LPITEM pkItem;

		for (int i = 0; i < WEAR_MAX_NUM; ++i)
			if ((pkItem = GetWear(i)) && pkItem->GetType() == ITEM_ARMOR)
			{
				if (pkItem->GetSubType() == ARMOR_BODY || pkItem->GetSubType() == ARMOR_HEAD || pkItem->GetSubType() == ARMOR_FOOTS || pkItem->GetSubType() == ARMOR_SHIELD)
				{
					iArmor += pkItem->GetValue(1);
					iArmor += (2 * pkItem->GetValue(5));
				}
			}

		// �� Ÿ�� ���� �� ������ ���� ���� ���º��� ������ ���� �������� ����
		if( true == IsHorseRiding() )
		{
			if (iArmor < GetHorseArmor())
				iArmor = GetHorseArmor();

			const char* pHorseName = CHorseNameManager::instance().GetHorseName(GetPlayerID());

			if (pHorseName != NULL && strlen(pHorseName))
			{
				iArmor += 20;
			}
		}

		iArmor += GetPoint(POINT_DEF_GRADE_BONUS);
		iArmor += GetPoint(POINT_PARTY_DEFENDER_BONUS);

		// INTERNATIONAL_VERSION
		if (LC_IsYMIR())
		{
			PointChange(POINT_DEF_GRADE, iShowDef + iArmor);
		}
		else
		{
			PointChange(POINT_DEF_GRADE, iDef + iArmor);
			PointChange(POINT_CLIENT_DEF_GRADE, (iShowDef + iArmor) - GetPoint(POINT_DEF_GRADE));
		}
		// END_OF_INTERNATIONAL_VERSION

		PointChange(POINT_MAGIC_ATT_GRADE, GetLevel() * 2 + GetPoint(POINT_IQ) * 2 + GetPoint(POINT_MAGIC_ATT_GRADE_BONUS));
		PointChange(POINT_MAGIC_DEF_GRADE, GetLevel() + (GetPoint(POINT_IQ) * 3 + GetPoint(POINT_HT)) / 3 + iArmor / 2 + GetPoint(POINT_MAGIC_DEF_GRADE_BONUS));
	}
	else
	{
		// 2lev + str * 2
		int iAtt = GetLevel() * 2 + GetPoint(POINT_ST) * 2;
		// lev + con
		int iDef = GetLevel() + GetPoint(POINT_HT) + GetMobTable().wDef;

		SetPoint(POINT_ATT_GRADE, iAtt);
		SetPoint(POINT_DEF_GRADE, iDef);
		SetPoint(POINT_MAGIC_ATT_GRADE, GetPoint(POINT_ATT_GRADE)); 
		SetPoint(POINT_MAGIC_DEF_GRADE, GetPoint(POINT_DEF_GRADE));
	}
}

void CHARACTER::ComputePoints()
{
	long lStat = GetPoint(POINT_STAT);
	long lStatResetCount = GetPoint(POINT_STAT_RESET_COUNT);
	long lSkillActive = GetPoint(POINT_SKILL);
	long lSkillSub = GetPoint(POINT_SUB_SKILL);
	long lSkillHorse = GetPoint(POINT_HORSE_SKILL);
	long lLevelStep = GetPoint(POINT_LEVEL_STEP);

	long lAttackerBonus = GetPoint(POINT_PARTY_ATTACKER_BONUS);
	long lTankerBonus = GetPoint(POINT_PARTY_TANKER_BONUS);
	long lBufferBonus = GetPoint(POINT_PARTY_BUFFER_BONUS);
	long lSkillMasterBonus = GetPoint(POINT_PARTY_SKILL_MASTER_BONUS);
	long lHasteBonus = GetPoint(POINT_PARTY_HASTE_BONUS);
	long lDefenderBonus = GetPoint(POINT_PARTY_DEFENDER_BONUS);

	long lHPRecovery = GetPoint(POINT_HP_RECOVERY);
	long lSPRecovery = GetPoint(POINT_SP_RECOVERY);
	m_pointsInstant.computed = false;
	
	/////pasif ekler
	
	long lkritikdef = GetPoint(POINT_RESIST_CRITICAL);
	long ldelicidef = GetPoint(POINT_RESIST_PENETRATE);
	long lcanavarsavunma = GetPoint(POINT_RESIST_MONSTER);
	long lcanavar = GetPoint(POINT_ATT_MONSTER_NEW);
	long lboss = GetPoint(POINT_ATT_BOSS);
	long lmetin = GetPoint(POINT_ATT_METIN);
	long lexp = GetPoint(POINT_EXP_DOUBLE_BONUS);
	long litem = GetPoint(POINT_ITEM_DROP_BONUS);
	
#ifdef ENABLE_CHEQUE_SYSTEM
	long lCheque = GetCheque();
#endif
	memset(m_pointsInstant.points, 0, sizeof(m_pointsInstant.points));
	BuffOnAttr_ClearAll();
	m_SkillDamageBonus.clear();

	SetPoint(POINT_STAT, lStat);
	SetPoint(POINT_SKILL, lSkillActive);
	SetPoint(POINT_SUB_SKILL, lSkillSub);
	SetPoint(POINT_HORSE_SKILL, lSkillHorse);
	SetPoint(POINT_LEVEL_STEP, lLevelStep);
	SetPoint(POINT_STAT_RESET_COUNT, lStatResetCount);

	SetPoint(POINT_ST, GetRealPoint(POINT_ST));
	SetPoint(POINT_HT, GetRealPoint(POINT_HT));
	SetPoint(POINT_DX, GetRealPoint(POINT_DX));
	SetPoint(POINT_IQ, GetRealPoint(POINT_IQ));

	SetPart(PART_MAIN, GetOriginalPart(PART_MAIN));
	SetPart(PART_WEAPON, GetOriginalPart(PART_WEAPON));
	SetPart(PART_HEAD, GetOriginalPart(PART_HEAD));
	SetPart(PART_HAIR, GetOriginalPart(PART_HAIR));
	#ifdef __SASH_SYSTEM__
	SetPart(PART_SASH, GetOriginalPart(PART_SASH));
	#endif
#ifdef ENABLE_AURA_SYSTEM
	SetPart(PART_AURA, GetOriginalPart(PART_AURA));
#endif
#ifdef __NEW_ARROW_SYSTEM__
	SetPart(PART_ARROW_TYPE, GetOriginalPart(PART_ARROW_TYPE));
#endif
	SetPoint(POINT_PARTY_ATTACKER_BONUS, lAttackerBonus);
	SetPoint(POINT_PARTY_TANKER_BONUS, lTankerBonus);
	SetPoint(POINT_PARTY_BUFFER_BONUS, lBufferBonus);
	SetPoint(POINT_PARTY_SKILL_MASTER_BONUS, lSkillMasterBonus);
	SetPoint(POINT_PARTY_HASTE_BONUS, lHasteBonus);
	SetPoint(POINT_PARTY_DEFENDER_BONUS, lDefenderBonus);

	SetPoint(POINT_HP_RECOVERY, lHPRecovery);
	SetPoint(POINT_SP_RECOVERY, lSPRecovery);

	// PC_BANG_ITEM_ADD
	SetPoint(POINT_PC_BANG_EXP_BONUS, 0);
	SetPoint(POINT_PC_BANG_DROP_BONUS, 0);
#ifdef ENABLE_CHEQUE_SYSTEM
	SetPoint(POINT_CHEQUE, lCheque);
#endif
	// END_PC_BANG_ITEM_ADD

	int64_t iMaxHP;
	int iMaxSP;
	int iMaxStamina;
	CSkillProto* pkSk;
	int ekle = 0;
	
	if (IsPC())
	{
		// �ִ� �����/���ŷ�
		iMaxHP = JobInitialPoints[GetJob()].max_hp + m_points.iRandomHP + GetPoint(POINT_HT) * JobInitialPoints[GetJob()].hp_per_ht;
		iMaxSP = JobInitialPoints[GetJob()].max_sp + m_points.iRandomSP + GetPoint(POINT_IQ) * JobInitialPoints[GetJob()].sp_per_iq;
		iMaxStamina = JobInitialPoints[GetJob()].max_stamina + GetPoint(POINT_HT) * JobInitialPoints[GetJob()].stamina_per_con;

		{
			pkSk = CSkillManager::instance().Get(SKILL_ADD_HP);

			if (NULL != pkSk)
			{
				pkSk->SetPointVar("k", 1.0f * GetSkillPower(SKILL_ADD_HP) / 100.0f);

				iMaxHP += static_cast<int>(pkSk->kPointPoly.Eval());
			}
		}
		{

			pkSk = CSkillManager::instance().Get(142);

			if (NULL != pkSk)
			{
				pkSk->SetPointVar("k", 1.0f * GetSkillPower(142) / 100.0f);

				ekle = static_cast<int>(pkSk->kPointPoly.Eval());
				// SetRealPoint(POINT_RESIST_CRITICAL, lkritikdef+ekle);
				PointChange(POINT_RESIST_CRITICAL,ekle);
			}
		}
		{
			
			
			pkSk = CSkillManager::instance().Get(143);

			if (NULL != pkSk)
			{
				pkSk->SetPointVar("k", 1.0f * GetSkillPower(143) / 100.0f);

				ekle = static_cast<int>(pkSk->kPointPoly.Eval());
				// SetRealPoint(POINT_RESIST_PENETRATE, ldelicidef+ekle);
				PointChange(POINT_RESIST_PENETRATE,ekle);
			}
		}
		{
			
			
			pkSk = CSkillManager::instance().Get(144);

			if (NULL != pkSk)
			{
				pkSk->SetPointVar("k", 1.0f * GetSkillPower(144) / 100.0f);

				ekle = static_cast<int>(pkSk->kPointPoly.Eval());
				// sys_err("Mevcut : %d Eklenecek : %d ",lcanavarsavunma,ekle);
				// SetRealPoint(POINT_RESIST_MONSTER, lcanavarsavunma+ekle);
				PointChange(POINT_RESIST_MONSTER,ekle);
			}
		}
		{
			
			
			pkSk = CSkillManager::instance().Get(145);

			if (NULL != pkSk)
			{
				pkSk->SetPointVar("k", 1.0f * GetSkillPower(145) / 100.0f);

				ekle = static_cast<int>(pkSk->kPointPoly.Eval());
				// SetRealPoint(POINT_ATT_MONSTER_NEW, lcanavar+ekle);
				PointChange(POINT_ATT_MONSTER_NEW,ekle);
			}
		}
		{
			
			
			pkSk = CSkillManager::instance().Get(146);

			if (NULL != pkSk)
			{
				pkSk->SetPointVar("k", 1.0f * GetSkillPower(146) / 100.0f);

				ekle = static_cast<int>(pkSk->kPointPoly.Eval());
				// SetRealPoint(POINT_ATT_BOSS, lboss+ekle);
				PointChange(POINT_ATT_BOSS,ekle);
			}
		}
		{
			
			
			pkSk = CSkillManager::instance().Get(147);

			if (NULL != pkSk)
			{
				pkSk->SetPointVar("k", 1.0f * GetSkillPower(147) / 100.0f);

				ekle = static_cast<int>(pkSk->kPointPoly.Eval());
				// SetRealPoint(POINT_ATT_METIN, lmetin+ekle);
				PointChange(POINT_ATT_METIN,ekle);
			}
		}
		{
			
			
			pkSk = CSkillManager::instance().Get(148);

			if (NULL != pkSk)
			{
				pkSk->SetPointVar("k", 1.0f * GetSkillPower(148) / 100.0f);

				ekle = static_cast<int>(pkSk->kPointPoly.Eval());
				// SetRealPoint(POINT_EXP_DOUBLE_BONUS, lexp+ekle);
				PointChange(POINT_EXP_DOUBLE_BONUS,ekle);
			}
		}
		{
			
			
			pkSk = CSkillManager::instance().Get(149);

			if (NULL != pkSk)
			{
				pkSk->SetPointVar("k", 1.0f * GetSkillPower(149) / 100.0f);

				ekle = static_cast<int>(pkSk->kPointPoly.Eval());
				// SetRealPoint(POINT_ITEM_DROP_BONUS, litem+ekle);
				PointChange(POINT_ITEM_DROP_BONUS,ekle);
			}
		}
		// �⺻ ����
		SetPoint(POINT_MOV_SPEED,	200);
		SetPoint(POINT_ATT_SPEED,	200);
		PointChange(POINT_ATT_SPEED, GetPoint(POINT_PARTY_HASTE_BONUS));
		SetPoint(POINT_CASTING_SPEED,	100);
	}
	else
	{
		iMaxHP = m_pkMobData->m_table.dwMaxHP;
		iMaxSP = 0;
		iMaxStamina = 0;

		SetPoint(POINT_ATT_SPEED, m_pkMobData->m_table.sAttackSpeed);
		SetPoint(POINT_MOV_SPEED, m_pkMobData->m_table.sMovingSpeed);
		SetPoint(POINT_CASTING_SPEED, m_pkMobData->m_table.sAttackSpeed);
	}

	if (IsPC())
	{
		// �� Ÿ�� ���� ���� �⺻ ������ ���� ���� ���Ⱥ��� ������ ���� �����.
		// ���� ���� ���� ������ ���� �����̹Ƿ�, ����/������ ��ü ���� ����
		// ��ä������ �� �ö󰡰� �� ���̴�.
		if (GetMountVnum()) 
		{
			if (GetHorseST() > GetPoint(POINT_ST))
				PointChange(POINT_ST, GetHorseST() - GetPoint(POINT_ST));

			if (GetHorseDX() > GetPoint(POINT_DX))
				PointChange(POINT_DX, GetHorseDX() - GetPoint(POINT_DX));

			if (GetHorseHT() > GetPoint(POINT_HT))
				PointChange(POINT_HT, GetHorseHT() - GetPoint(POINT_HT));

			if (GetHorseIQ() > GetPoint(POINT_IQ))
				PointChange(POINT_IQ, GetHorseIQ() - GetPoint(POINT_IQ));
		}

	}

	ComputeBattlePoints();

	// �⺻ HP/SP ����
	if (iMaxHP != GetMaxHP())
	{
		SetRealPoint(POINT_MAX_HP, iMaxHP); // �⺻HP�� RealPoint�� ������ ���´�.
	}

	PointChange(POINT_MAX_HP, 0);

	if (iMaxSP != GetMaxSP())
	{
		SetRealPoint(POINT_MAX_SP, iMaxSP); // �⺻SP�� RealPoint�� ������ ���´�.
	}

	PointChange(POINT_MAX_SP, 0);

	SetMaxStamina(iMaxStamina);
	int iCurHP = this->GetHP();
	int iCurSP = this->GetSP();

	m_pointsInstant.dwImmuneFlag = 0;

	for (int i = 0 ; i < WEAR_MAX_NUM; i++) 
	{
		LPITEM pItem = GetWear(i);
		if (pItem)
		{
			pItem->ModifyPoints(true);
			SET_BIT(m_pointsInstant.dwImmuneFlag, GetWear(i)->GetImmuneFlag());
		}
	}

	// ��ȥ�� �ý���
	// ComputePoints������ �ɸ����� ��� �Ӽ����� �ʱ�ȭ�ϰ�,
	// ������, ���� � ���õ� ��� �Ӽ����� �����ϱ� ������,
	// ��ȥ�� �ý��۵� ActiveDeck�� �ִ� ��� ��ȥ���� �Ӽ����� �ٽ� ������Ѿ� �Ѵ�.
	if (DragonSoul_IsDeckActivated())
	{
		for (int i = WEAR_MAX_NUM + DS_SLOT_MAX * DragonSoul_GetActiveDeck(); 
			i < WEAR_MAX_NUM + DS_SLOT_MAX * (DragonSoul_GetActiveDeck() + 1); i++)	
		{
			LPITEM pItem = GetWear(i);
			if (pItem)
			{
				if (DSManager::instance().IsTimeLeftDragonSoul(pItem))
					pItem->ModifyPoints(true);
			}
		}
	}

	if (GetHP() > GetMaxHP())
		PointChange(POINT_HP, GetMaxHP() - GetHP());

	if (GetSP() > GetMaxSP())
		PointChange(POINT_SP, GetMaxSP() - GetSP());

	ComputeSkillPoints();

	RefreshAffect();

	CPetSystem* pPetSystem = GetPetSystem();
	if (NULL != pPetSystem)
		pPetSystem->RefreshBuff();

	for (TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.begin(); it != m_map_buff_on_attrs.end(); it++)
		it->second->GiveAllAttributes();

	if (IsPC())
	{
		if (this->GetHP() != iCurHP)
			this->PointChange(POINT_HP, iCurHP-this->GetHP());
		if (this->GetSP() != iCurSP)
			this->PointChange(POINT_SP, iCurSP-this->GetSP());
	}
	m_pointsInstant.computed = true;

	UpdatePacket();
}

// m_dwPlayStartTime�� ������ milisecond��. �����ͺ��̽����� �д����� ����ϱ�
// ������ �÷��̽ð��� ����� �� / 60000 ���� ������ �ϴµ�, �� ������ ���� ����
// �� �� ���⿡ dwTimeRemain���� �־ ����� ���ǵ��� ���־�� �Ѵ�.
void CHARACTER::ResetPlayTime(DWORD dwTimeRemain)
{
	m_dwPlayStartTime = get_dword_time() - dwTimeRemain;
}

const int aiRecoveryPercents[10] = { 1, 5, 5, 5, 5, 5, 5, 5, 5, 5 };

EVENTFUNC(recovery_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info );
	if ( info == NULL )
	{
		sys_err( "recovery_event> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (ch == NULL) { // <Factor>
		return 0;
	}	

	if (!ch->IsPC())
	{
		//
		// ���� ȸ��
		//
		if (ch->IsAffectFlag(AFF_POISON))
			return PASSES_PER_SEC(MAX(1, ch->GetMobTable().bRegenCycle));

		if (2493 == ch->GetMobTable().dwVnum)
		{
			int64_t regenPct = BlueDragon_GetRangeFactor("hp_regen", ch->GetHPPct());
			regenPct += ch->GetMobTable().bRegenPercent;

			for (int i=1 ; i <= 4 ; ++i)
			{
				if (REGEN_PECT_BONUS == BlueDragon_GetIndexFactor("DragonStone", i, "effect_type"))
				{
					DWORD dwDragonStoneID = BlueDragon_GetIndexFactor("DragonStone", i, "vnum");
					size_t val = BlueDragon_GetIndexFactor("DragonStone", i, "val");
					size_t cnt = SECTREE_MANAGER::instance().GetMonsterCountInMap( ch->GetMapIndex(), dwDragonStoneID );

					regenPct += (val*cnt);

					break;
				}
			}

			ch->PointChange(POINT_HP, MAX(1, (ch->GetMaxHP() / 100) * regenPct));
		}
		else if (!ch->IsDoor())
		{
			ch->MonsterLog("HP_REGEN +%lld", MAX(1, (ch->GetMaxHP() / 100) * ch->GetMobTable().bRegenPercent));
			ch->PointChange(POINT_HP, MAX(1, (ch->GetMaxHP() / 100) * ch->GetMobTable().bRegenPercent));
		}

		if (ch->GetHP() >= ch->GetMaxHP())
		{
			ch->m_pkRecoveryEvent = NULL;
			return 0;
		}

		if (2493 == ch->GetMobTable().dwVnum)
		{
			for (int i=1 ; i <= 4 ; ++i)
			{
				if (REGEN_TIME_BONUS == BlueDragon_GetIndexFactor("DragonStone", i, "effect_type"))
				{
					DWORD dwDragonStoneID = BlueDragon_GetIndexFactor("DragonStone", i, "vnum");
					size_t val = BlueDragon_GetIndexFactor("DragonStone", i, "val");
					size_t cnt = SECTREE_MANAGER::instance().GetMonsterCountInMap( ch->GetMapIndex(), dwDragonStoneID );

					return PASSES_PER_SEC(MAX(1, (ch->GetMobTable().bRegenCycle - (val*cnt))));
				}
			}
		}

		return PASSES_PER_SEC(MAX(1, ch->GetMobTable().bRegenCycle));
	}
	else
	{
		//
		// PC ȸ��
		//
		ch->CheckTarget();
		//ch->UpdateSectree(); // ���⼭ �̰� ������?
		ch->UpdateKillerMode();

		if (ch->IsAffectFlag(AFF_POISON) == true)
		{
			// �ߵ��� ��� �ڵ�ȸ�� ����
			// �Ĺ����� ��� �ڵ�ȸ�� ����
			return 3;
		}

		int iSec = (get_dword_time() - ch->GetLastMoveTime()) / 3000;

		// SP ȸ�� ��ƾ.
		// �� �̰ɷ� �ؼ� �Լ��� �����°� ?!
		ch->DistributeSP(ch);

		if (ch->GetMaxHP() <= ch->GetHP())
			return PASSES_PER_SEC(3);

		int iPercent = 0;
		int iAmount = 0;

		{
			iPercent = aiRecoveryPercents[MIN(9, iSec)];
			iAmount = (ch->GetMaxHP() / 100 * iPercent) + 15;
		}

		iAmount += (iAmount / 100) * ch->GetPoint(POINT_HP_REGEN);
		sys_log(1, "RECOVERY_EVENT: %s %d HP_REGEN %d HP +%d", ch->GetName(), iPercent, ch->GetPoint(POINT_HP_REGEN), iAmount);

		ch->PointChange(POINT_HP, iAmount, false);
		return PASSES_PER_SEC(3);
	}
}

void CHARACTER::StartRecoveryEvent()
{
	if (m_pkRecoveryEvent)
		return;

	if (IsDead() || IsStun())
		return;

	if (IsNPC() && GetHP() >= GetMaxHP()) // ���ʹ� ü���� �� �������� ���� ���Ѵ�.
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;

	int iSec = IsPC() ? 3 : (MAX(1, GetMobTable().bRegenCycle));
	m_pkRecoveryEvent = event_create(recovery_event, info, PASSES_PER_SEC(iSec));
}

void CHARACTER::Standup()
{
	struct packet_position pack_position;

	if (!IsPosition(POS_SITTING))
		return;

	SetPosition(POS_STANDING);

	sys_log(1, "STANDUP: %s", GetName());

	pack_position.header	= HEADER_GC_CHARACTER_POSITION;
	pack_position.vid		= GetVID();
	pack_position.position	= POSITION_GENERAL;

	PacketAround(&pack_position, sizeof(pack_position));
}

void CHARACTER::Sitdown(int is_ground)
{
	struct packet_position pack_position;

	if (IsPosition(POS_SITTING))
		return;

	SetPosition(POS_SITTING);
	sys_log(1, "SITDOWN: %s", GetName());

	pack_position.header	= HEADER_GC_CHARACTER_POSITION;
	pack_position.vid		= GetVID();
	pack_position.position	= POSITION_SITTING_GROUND;
	PacketAround(&pack_position, sizeof(pack_position));
}

void CHARACTER::SetRotation(float fRot)
{
	m_pointsInstant.fRot = fRot;
}

// x, y �������� ���� ����.
void CHARACTER::SetRotationToXY(long x, long y)
{
	SetRotation(GetDegreeFromPositionXY(GetX(), GetY(), x, y));
}

bool CHARACTER::CannotMoveByAffect() const
{
	return (IsAffectFlag(AFF_STUN));
}

bool CHARACTER::CanMove() const
{
	if (CannotMoveByAffect())
		return false;

	if (GetMyShop())	// ���� �� ���¿����� ������ �� ����
		return false;

	// 0.2�� ���̶�� ������ �� ����.
	/*
	   if (get_float_time() - m_fSyncTime < 0.2f)
	   return false;
	 */
	return true;
}

// ������ x, y ��ġ�� �̵� ��Ų��.
bool CHARACTER::Sync(long x, long y)
{
	if (!GetSectree())
		return false;

	LPSECTREE new_tree = SECTREE_MANAGER::instance().Get(GetMapIndex(), x, y);

	if (!new_tree)
	{
		if (GetDesc())
		{
			sys_err("cannot find tree at %d %d (name: %s)", x, y, GetName());
			//GetDesc()->SetPhase(PHASE_CLOSE);
		}
		else
		{
			sys_err("no tree: %s %d %d %d", GetName(), x, y, GetMapIndex());
			Dead();
		}

		return false;
	}

	SetRotationToXY(x, y);
	SetXYZ(x, y, 0);

	if (GetDungeon())
	{
		// ������ �̺�Ʈ �Ӽ� ��ȭ
		int iLastEventAttr = m_iEventAttr;
		m_iEventAttr = new_tree->GetEventAttribute(x, y);

		if (m_iEventAttr != iLastEventAttr)
		{
			if (GetParty())
			{
				quest::CQuestManager::instance().AttrOut(GetParty()->GetLeaderPID(), this, iLastEventAttr);
				quest::CQuestManager::instance().AttrIn(GetParty()->GetLeaderPID(), this, m_iEventAttr);
			}
			else
			{
				quest::CQuestManager::instance().AttrOut(GetPlayerID(), this, iLastEventAttr);
				quest::CQuestManager::instance().AttrIn(GetPlayerID(), this, m_iEventAttr);
			}
		}
	}

	if (GetSectree() != new_tree)
	{
		if (!IsNPC())
		{
			SECTREEID id = new_tree->GetID();
			SECTREEID old_id = GetSectree()->GetID();

			sys_log(0, "SECTREE DIFFER: %s %dx%d was %dx%d",
					GetName(),
					id.coord.x,
					id.coord.y,
					old_id.coord.x,
					old_id.coord.y);
		}

		new_tree->InsertEntity(this);
	}

	return true;
}

void CHARACTER::Stop()
{
	if (!IsState(m_stateIdle))
		MonsterLog("[IDLE] ????");

	GotoState(m_stateIdle);

	m_posDest.x = m_posStart.x = GetX();
	m_posDest.y = m_posStart.y = GetY();
}

bool CHARACTER::Goto(long x, long y)
{
	// TODO �Ÿ�üũ �ʿ�
	// ���� ��ġ�� �̵��� �ʿ� ���� (�ڵ� ����)
	if (GetX() == x && GetY() == y)
		return false;

	if (m_posDest.x == x && m_posDest.y == y)
	{
		if (!IsState(m_stateMove))
		{
			m_dwStateDuration = 4;
			GotoState(m_stateMove);
		}
		return false;
	}

	m_posDest.x = x;
	m_posDest.y = y;

	CalculateMoveDuration();

	m_dwStateDuration = 4;

	
	if (!IsState(m_stateMove))
	{
		MonsterLog("[MOVE] %s", GetVictim() ? "�������" : "�׳��̵�");

		if (GetVictim())
		{
			//MonsterChat(MONSTER_CHAT_CHASE);
			MonsterChat(MONSTER_CHAT_ATTACK);
		}
	}

	GotoState(m_stateMove);

	return true;
}


DWORD CHARACTER::GetMotionMode() const
{
	DWORD dwMode = MOTION_MODE_GENERAL;

	if (IsPolymorphed())
		return dwMode;

	LPITEM pkItem;

	if ((pkItem = GetWear(WEAR_WEAPON)))
	{
		switch (pkItem->GetProto()->bSubType)
		{
			case WEAPON_SWORD:
				dwMode = MOTION_MODE_ONEHAND_SWORD;
				break;

			case WEAPON_TWO_HANDED:
				dwMode = MOTION_MODE_TWOHAND_SWORD;
				break;

			case WEAPON_DAGGER:
				dwMode = MOTION_MODE_DUALHAND_SWORD;
				break;

			case WEAPON_BOW:
				dwMode = MOTION_MODE_BOW;
				break;

			case WEAPON_BELL:
				dwMode = MOTION_MODE_BELL;
				break;

			case WEAPON_FAN:
				dwMode = MOTION_MODE_FAN;
				break;
			case WEAPON_CLAW:
				dwMode = MOTION_MODE_CLAW;
				break;
		}
	}
	return dwMode;
}

float CHARACTER::GetMoveMotionSpeed() const
{
	DWORD dwMode = GetMotionMode();

	const CMotion * pkMotion = NULL;

	if (!GetMountVnum())
		pkMotion = CMotionManager::instance().GetMotion(GetRaceNum(), MAKE_MOTION_KEY(dwMode, (IsWalking() && IsPC()) ? MOTION_WALK : MOTION_RUN));
	else
	{
		pkMotion = CMotionManager::instance().GetMotion(GetMountVnum(), MAKE_MOTION_KEY(MOTION_MODE_GENERAL, (IsWalking() && IsPC()) ? MOTION_WALK : MOTION_RUN));

		if (!pkMotion)
			pkMotion = CMotionManager::instance().GetMotion(GetRaceNum(), MAKE_MOTION_KEY(MOTION_MODE_HORSE, (IsWalking() && IsPC()) ? MOTION_WALK : MOTION_RUN));
	}

	if (pkMotion)
		return -pkMotion->GetAccumVector().y / pkMotion->GetDuration();
	else
	{
		//sys_err("cannot find motion (name %s race %d mode %d)", GetName(), GetRaceNum(), dwMode);
		return 300.0f;
	}
}

float CHARACTER::GetMoveSpeed() const
{
	return GetMoveMotionSpeed() * 10000 / CalculateDuration(GetLimitPoint(POINT_MOV_SPEED), 10000);
}

void CHARACTER::CalculateMoveDuration()
{
	m_posStart.x = GetX();
	m_posStart.y = GetY();

	float fDist = DISTANCE_SQRT(m_posStart.x - m_posDest.x, m_posStart.y - m_posDest.y);

	float motionSpeed = GetMoveMotionSpeed();

	m_dwMoveDuration = CalculateDuration(GetLimitPoint(POINT_MOV_SPEED),
			(int) ((fDist / motionSpeed) * 1000.0f));

	if (IsNPC())
		sys_log(1, "%s: GOTO: distance %f, spd %u, duration %u, motion speed %f pos %d %d -> %d %d",
				GetName(), fDist, GetLimitPoint(POINT_MOV_SPEED), m_dwMoveDuration, motionSpeed,
				m_posStart.x, m_posStart.y, m_posDest.x, m_posDest.y);

	m_dwMoveStartTime = get_dword_time();
}

// x y ��ġ�� �̵� �Ѵ�. (�̵��� �� �ִ� �� ���� ���� Ȯ�� �ϰ� Sync �޼ҵ�� ���� �̵� �Ѵ�)
// ������ char�� x, y ���� �ٷ� �ٲ�����,
// Ŭ�󿡼��� ���� ��ġ���� �ٲ� x, y���� interpolation�Ѵ�.
// �Ȱų� �ٴ� ���� char�� m_bNowWalking�� �޷��ִ�.
// Warp�� �ǵ��� ���̶�� Show�� ����� ��.
bool CHARACTER::Move(long x, long y)
{
	// ���� ��ġ�� �̵��� �ʿ� ���� (�ڵ� ����)
	if (GetX() == x && GetY() == y)
		return true;

	if (test_server)
		if (m_bDetailLog)
			sys_log(0, "%s position %u %u", GetName(), x, y);

	OnMove();
	return Sync(x, y);
}

void CHARACTER::SendMovePacket(BYTE bFunc, BYTE bArg, DWORD x, DWORD y, DWORD dwDuration, DWORD dwTime, int iRot)
{
	TPacketGCMove pack;

	if (bFunc == FUNC_WAIT)
	{
		x = m_posDest.x;
		y = m_posDest.y;
		dwDuration = m_dwMoveDuration;
	}

	EncodeMovePacket(pack, GetVID(), bFunc, bArg, x, y, dwDuration, dwTime, iRot == -1 ? (int) GetRotation() / 5 : iRot);
	PacketView(&pack, sizeof(TPacketGCMove), this);
}

#ifdef LWT_BUFFI_SYSTEM
void CHARACTER::SendSupportSkillPacket(DWORD skill_vnum, DWORD skillValue)
{
	TPacketGCSupportUseSkill pack;
	pack.bHeader = HEADER_GC_SUPPORT_SKILL;
	pack.dwVnum = skill_vnum;
	pack.dwVid = GetVID();
	pack.dwLevel = skillValue;
	
	PacketView(&pack, sizeof(TPacketGCSupportUseSkill), this);
}
#endif

int64_t CHARACTER::GetRealPoint(BYTE type) const
{
	return m_points.points[type];
}

void CHARACTER::SetRealPoint(BYTE type, int64_t val)
{
	m_points.points[type] = val;
}

int CHARACTER::GetPolymorphPoint(BYTE type) const
{
	if (IsPolymorphed() && !IsPolyMaintainStat())
	{
		DWORD dwMobVnum = GetPolymorphVnum();
		const CMob * pMob = CMobManager::instance().Get(dwMobVnum);
		int iPower = GetPolymorphPower();

		if (pMob)
		{
			switch (type)
			{
				case POINT_ST:
					if (GetJob() == JOB_SHAMAN || GetJob() == JOB_SURA && GetSkillGroup() == 2)
						return pMob->m_table.bStr * iPower / 100 + GetPoint(POINT_IQ);
					return pMob->m_table.bStr * iPower / 100 + GetPoint(POINT_ST);

				case POINT_HT:
					return pMob->m_table.bCon * iPower / 100 + GetPoint(POINT_HT);

				case POINT_IQ:
					return pMob->m_table.bInt * iPower / 100 + GetPoint(POINT_IQ);

				case POINT_DX:
					return pMob->m_table.bDex * iPower / 100 + GetPoint(POINT_DX);
			}
		}
	}

	return GetPoint(type);
}

int64_t CHARACTER::GetPoint(BYTE type) const
{
	if (type >= POINT_MAX_NUM)
	{
		sys_err("Point type overflow (type %u)", type);
		return 0;
	}

	int64_t val = m_pointsInstant.points[type];
	int64_t max_val = LLONG_MAX;

	switch (type)
	{
		case POINT_STEAL_HP:
		case POINT_STEAL_SP:
			max_val = 75;
			break;
	}

	if (val > max_val)
		sys_err("POINT_ERROR:%s type %d val %lld (max:%lld)", GetName(), type, val, max_val);

	return (val);
}

int64_t CHARACTER::GetLimitPoint(BYTE type) const
{
	if (type >= POINT_MAX_NUM)
	{
		sys_err("Point type overflow (type %u)", type);
		return 0;
	}

	int64_t val = m_pointsInstant.points[type];
	int64_t max_val = LLONG_MAX;
	int64_t limit = LLONG_MAX;
	int64_t min_limit = -LLONG_MAX;

	switch (type)
	{
		case POINT_ATT_SPEED:
			min_limit = 0;

			if (IsPC())
				limit = 300;
			else
				limit = 250;
			break;

		case POINT_MOV_SPEED:
			min_limit = 0;

			if (IsPC())
				limit = pc_max_movement_speed;
			else
				limit = mob_max_movement_speed;
			break;

		case POINT_STEAL_HP:
		case POINT_STEAL_SP:
			limit = 50;
			max_val = 50;
			break;

		case POINT_MALL_ATTBONUS:
		case POINT_MALL_DEFBONUS:
			limit = 20;
			max_val = 75;
			break;
	}

	if (val > max_val)
		sys_err("POINT_ERROR:%s type %d val %lld (max:%lld)", GetName(), type, val, max_val);

	if (val > limit)
		val = limit;

	if (val < min_limit)
		val = min_limit;

	return (val);
}

void CHARACTER::SetPoint(BYTE type, int64_t val)
{
	if (type >= POINT_MAX_NUM)
	{
		sys_err("Point type overflow (type %u)", type);
		return;
	}

	m_pointsInstant.points[type] = val;

	// ���� �̵��� �� �ȳ����ٸ� �̵� �ð� ����� �ٽ� �ؾ� �Ѵ�.
	if (type == POINT_MOV_SPEED && get_dword_time() < m_dwMoveStartTime + m_dwMoveDuration)
	{
		CalculateMoveDuration();
	}
}

#ifdef YANG_LIMIT
LDWORD CHARACTER::GetAllowedGold() const
{
	return 2000000000000LL;
}
void CHARACTER::GoldChange(LDWORD ldGold, std::string c_szReason)
{
	const LDWORD nTotalMoney = static_cast<LDWORD>(GetGold()) + static_cast<LDWORD>(ldGold);

	if (GetAllowedGold() <= nTotalMoney)
	{
		sys_err("[OVERFLOW_GOLD] OriGold %llu AddedGold %llu id %u Name %s ", GetGold(), ldGold, GetPlayerID(), GetName());
		LogManager::instance().CharLog(this, GetGold() + ldGold, "OVERFLOW_GOLD", "");
		return;
	}

	if (ldGold < 0) {
		if ((GetGold() + ldGold) <= 0) {
			//sys_err("%s, serefsiz. altina dusuyor: [%s]", GetName(), c_szReason.c_str());
			SetGold(0);
			TPacketGCGoldChange pack;

			pack.header = HEADER_GC_GOLD_CHANGE;
			pack.dwVID = m_vid;

			if (ldGold <= 0) { pack.ldAmount = 0; }
			else { pack.ldAmount = ldGold; }


			pack.ldValue = GetGold();
			GetDesc()->Packet(&pack, sizeof(TPacketGCGoldChange));
			return;
		}
	}

	sys_log(0, "GoldChange: [%s], [%d] [%lld - %lld], [%s]", GetName(), GetPlayerID(), ldGold, GetGold(), c_szReason.c_str());


	SetGold(GetGold() + ldGold);


	if (GetDesc())
	{
		TPacketGCGoldChange pack;

		pack.header = HEADER_GC_GOLD_CHANGE;
		pack.dwVID = m_vid;

		if (ldGold <= 0){ pack.ldAmount = 0; }
		else{ pack.ldAmount = ldGold; }
			

		pack.ldValue = GetGold();
		GetDesc()->Packet(&pack, sizeof(TPacketGCGoldChange));
	}
}
#else
INT CHARACTER::GetAllowedGold() const
{
	if (GetLevel() <= 10)
		return 100000;
	else if (GetLevel() <= 20)
		return 500000;
	else
		return 50000000;
}
#endif

void CHARACTER::CheckMaximumPoints()
{
	if (GetMaxHP() < GetHP())
		PointChange(POINT_HP, GetMaxHP() - GetHP());

	if (GetMaxSP() < GetSP())
		PointChange(POINT_SP, GetMaxSP() - GetSP());
}

void CHARACTER::PointChange(BYTE type, int64_t amount, bool bAmount, bool bBroadcast)
{
	int val = 0;

	//sys_log(0, "PointChange %d %d | %d -> %d cHP %d mHP %d", type, amount, GetPoint(type), GetPoint(type)+amount, GetHP(), GetMaxHP());

	switch (type)
	{
		case POINT_NONE:
			return;

		case POINT_LEVEL:
			if ((GetLevel() + amount) > gPlayerMaxLevel)
				return;

			SetLevel(GetLevel() + amount);
			val = GetLevel();

			sys_log(0, "LEVELUP: %s %d NEXT EXP %d", GetName(), GetLevel(), GetNextExp());

			PointChange(POINT_NEXT_EXP,	GetNextExp(), false);

			if (amount)
			{
				quest::CQuestManager::instance().LevelUp(GetPlayerID());
				int biodurum = GetQuestFlag("bio.durum");
				if (biodurum == 0 && GetLevel() >= 30)
				{
					SetQuestFlag("bio.durum",1);
					SetQuestFlag("bio.verilen",0);
					SetQuestFlag("bio.ruhtasi",0);
					SetQuestFlag("bio.kalan",get_global_time());
					int bioverilen =  GetQuestFlag("bio.verilen");
					int biokalan =  GetQuestFlag("bio.kalan");
					biodurum = GetQuestFlag("bio.durum");
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					
				}
				else if (biodurum == 31 &&  GetLevel() >= 40)
				{
					SetQuestFlag("bio.durum",2);
					SetQuestFlag("bio.verilen",0);
					SetQuestFlag("bio.ruhtasi",0);
					SetQuestFlag("bio.kalan",get_global_time());
					int bioverilen =  GetQuestFlag("bio.verilen");
					int biokalan =  GetQuestFlag("bio.kalan");
					biodurum = GetQuestFlag("bio.durum");
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				else if (biodurum == 41 &&  GetLevel() >= 50)
				{
					SetQuestFlag("bio.durum",3);
					SetQuestFlag("bio.verilen",0);
					SetQuestFlag("bio.ruhtasi",0);
					SetQuestFlag("bio.kalan",get_global_time());
					int bioverilen =  GetQuestFlag("bio.verilen");
					int biokalan =  GetQuestFlag("bio.kalan");
					biodurum = GetQuestFlag("bio.durum");
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				else if (biodurum == 51 &&  GetLevel() >= 60)
				{
					SetQuestFlag("bio.durum",4);
					SetQuestFlag("bio.verilen",0);
					SetQuestFlag("bio.ruhtasi",0);
					SetQuestFlag("bio.kalan",get_global_time());
					int bioverilen =  GetQuestFlag("bio.verilen");
					int biokalan =  GetQuestFlag("bio.kalan");
					biodurum = GetQuestFlag("bio.durum");
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				else if (biodurum == 61 &&  GetLevel() >= 70)
				{
					SetQuestFlag("bio.durum",5);
					SetQuestFlag("bio.verilen",0);
					SetQuestFlag("bio.ruhtasi",0);
					SetQuestFlag("bio.kalan",get_global_time());
					int bioverilen =  GetQuestFlag("bio.verilen");
					int biokalan =  GetQuestFlag("bio.kalan");
					biodurum = GetQuestFlag("bio.durum");
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				else if (biodurum == 71 &&  GetLevel() >= 80)
				{
					SetQuestFlag("bio.durum",6);
					SetQuestFlag("bio.verilen",0);
					SetQuestFlag("bio.ruhtasi",0);
					SetQuestFlag("bio.kalan",get_global_time());
					int bioverilen =  GetQuestFlag("bio.verilen");
					int biokalan =  GetQuestFlag("bio.kalan");
					biodurum = GetQuestFlag("bio.durum");
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				else if (biodurum == 81 &&  GetLevel() >= 85)
				{
					SetQuestFlag("bio.durum",7);
					SetQuestFlag("bio.verilen",0);
					SetQuestFlag("bio.ruhtasi",0);
					SetQuestFlag("bio.kalan",get_global_time());
					int bioverilen =  GetQuestFlag("bio.verilen");
					int biokalan =  GetQuestFlag("bio.kalan");
					biodurum = GetQuestFlag("bio.durum");
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				else if (biodurum == 86 &&  GetLevel() >= 90)
				{
					SetQuestFlag("bio.durum",8);
					SetQuestFlag("bio.verilen",0);
					SetQuestFlag("bio.ruhtasi",0);
					SetQuestFlag("bio.kalan",get_global_time());
					int bioverilen =  GetQuestFlag("bio.verilen");
					int biokalan =  GetQuestFlag("bio.kalan");
					biodurum = GetQuestFlag("bio.durum");
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				else if (biodurum == 91 &&  GetLevel() >= 92)
				{
					SetQuestFlag("bio.durum",9);
					SetQuestFlag("bio.verilen",0);
					SetQuestFlag("bio.ruhtasi",0);
					SetQuestFlag("bio.kalan",get_global_time());
					int bioverilen =  GetQuestFlag("bio.verilen");
					int biokalan =  GetQuestFlag("bio.kalan");
					biodurum = GetQuestFlag("bio.durum");
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				else if (biodurum == 93 &&  GetLevel() >= 94)
				{
					SetQuestFlag("bio.durum",10);
					SetQuestFlag("bio.verilen",0);
					SetQuestFlag("bio.ruhtasi",0);
					SetQuestFlag("bio.kalan",get_global_time());
					int bioverilen =  GetQuestFlag("bio.verilen");
					int biokalan =  GetQuestFlag("bio.kalan");
					biodurum = GetQuestFlag("bio.durum");
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				else if (biodurum == 100 &&  GetLevel() >= 100)
				{
					SetQuestFlag("bio.durum",11);
					SetQuestFlag("bio.verilen",0);
					SetQuestFlag("bio.ruhtasi",0);
					SetQuestFlag("bio.kalan",get_global_time());
					int bioverilen =  GetQuestFlag("bio.verilen");
					int biokalan =  GetQuestFlag("bio.kalan");
					biodurum = GetQuestFlag("bio.durum");
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				else if (biodurum == 102 &&  GetLevel() >= 102)
				{
					SetQuestFlag("bio.durum",12);
					SetQuestFlag("bio.verilen",0);
					SetQuestFlag("bio.ruhtasi",0);
					SetQuestFlag("bio.kalan",get_global_time());
					int bioverilen =  GetQuestFlag("bio.verilen");
					int biokalan =  GetQuestFlag("bio.kalan");
					biodurum = GetQuestFlag("bio.durum");
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				else if (biodurum == 105 &&  GetLevel() >= 105)
				{
					SetQuestFlag("bio.durum",13);
					SetQuestFlag("bio.verilen",0);
					SetQuestFlag("bio.ruhtasi",0);
					SetQuestFlag("bio.kalan",get_global_time());
					int bioverilen =  GetQuestFlag("bio.verilen");
					int biokalan =  GetQuestFlag("bio.kalan");
					biodurum = GetQuestFlag("bio.durum");
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				else if (biodurum == 108 &&  GetLevel() >= 108)
				{
					SetQuestFlag("bio.durum",14);
					SetQuestFlag("bio.verilen",0);
					SetQuestFlag("bio.ruhtasi",0);
					SetQuestFlag("bio.kalan",get_global_time());
					int bioverilen =  GetQuestFlag("bio.verilen");
					int biokalan =  GetQuestFlag("bio.kalan");
					biodurum = GetQuestFlag("bio.durum");
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				else if (biodurum == 110 &&  GetLevel() >= 110)
				{
					SetQuestFlag("bio.durum",15);
					SetQuestFlag("bio.verilen",0);
					SetQuestFlag("bio.ruhtasi",0);
					SetQuestFlag("bio.kalan",get_global_time());
					int bioverilen =  GetQuestFlag("bio.verilen");
					int biokalan =  GetQuestFlag("bio.kalan");
					biodurum = GetQuestFlag("bio.durum");
					ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}

				LogManager::instance().LevelLog(this, val, GetRealPoint(POINT_PLAYTIME) + (get_dword_time() - m_dwPlayStartTime) / 60000);

				if (GetGuild())
				{
					GetGuild()->LevelChange(GetPlayerID(), GetLevel());
				}

				if (GetParty())
				{
					GetParty()->RequestSetMemberLevel(GetPlayerID(), GetLevel());
				}
			}
			break;

		case POINT_NEXT_EXP:
			val = GetNextExp();
			bAmount = false;	// ������ bAmount�� false ���� �Ѵ�.
			break;

		case POINT_EXP:
			{
				if (FindAffect(AFFECT_EXP_CURSE) && amount > 0)
					return;
				DWORD exp = GetExp();
				DWORD next_exp = GetNextExp();

				if (LC_IsNewCIBN())
				{
					if (IsOverTime(OT_NONE))
					{
						dev_log(LOG_DEB0, "<EXP_LOG> %s = NONE", GetName());
					}
					else if (IsOverTime(OT_3HOUR))
					{
						amount = (amount / 2);
						dev_log(LOG_DEB0, "<EXP_LOG> %s = 3HOUR", GetName());
					}
					else if (IsOverTime(OT_5HOUR))
					{
						amount = 0;
						dev_log(LOG_DEB0, "<EXP_LOG> %s = 5HOUR", GetName());
					}
				}

				if (amount < 0 && (int)exp < -amount)
				{
					sys_log(1, "%s AMOUNT < 0 %d, CUR EXP: %d", GetName(), -amount, exp);
					amount = -static_cast<int>(exp);
					SetExp(exp + amount);
					val = GetExp();
				}
				else
				{

					// #ifdef ENABLE_BIYOLOG_SYSTEM
					// int biodurum = GetQuestFlag("bio.durum");
					// if (biodurum == 0 && GetLevel() == 30)
					// {
						// LPCHARACTER pkVictim = CHARACTER_MANAGER::instance().FindByPID(GetPlayerID());
						// if(pkVictim)
						// {
							// LPDESC pkVictimDesc = pkVictim->GetDesc();
							// if (pkVictimDesc)
							// {
								// char msg[CHAT_MAX_LEN + 1];
								// snprintf(msg, sizeof(msg), LC_TEXT("T_SATICIMESAJ2"));
								// TPacketGCWhisper pack;
								// int len = MIN(CHAT_MAX_LEN, strlen(msg) + 1);
								// pack.bHeader = HEADER_GC_WHISPER;
								// pack.wSize = sizeof(TPacketGCWhisper) + len;
								// pack.bType = WHISPER_TYPE_NORMAL;
								// strlcpy(pack.szNameFrom, LC_TEXT("TEZGAHIN2"), sizeof(pack.szNameFrom));
								// TEMP_BUFFER buf;
								// buf.write(&pack, sizeof(TPacketGCWhisper));
								// buf.write(msg, len);
								// pkVictimDesc->Packet(buf.read_peek(), buf.size());
							// }
						// }
					// }
					// #endif

					#ifdef ENABLE_REBIRT_SYSTEM
					if (GetLevel() >= gPlayerMaxLevel)
					{
						//DWORD next_exp = GetNextRebirtExp();
						if (GetRebirt() < REBIRT_MAX)
						{
							if (GetRebirt() < 6)
								amount /= 8;
							else
								amount /= 12;

							if (GetLevel() >= gPlayerMaxLevel && (exp + amount) >= next_exp)
							{
								SetRebirt(GetRebirt()+1);
								SetExp(0);

								if (IsPC())
								{
									
									if (GetRebirt() == 1 || 
										GetRebirt() == 2 || 
										GetRebirt() == 3 || 
										GetRebirt() == 4 || 
										GetRebirt() == 5)
									{
										EffectPacket(SE_DUEL_SENDER);
									}

									else if (GetRebirt() == 6 || 
										GetRebirt() == 7 || 
										GetRebirt() == 8 || 
										GetRebirt() == 9 || 
										GetRebirt() == 10)
									{
										EffectPacket(SE_DUEL_RECEIVER);
									}

									else if (GetRebirt() == 11 || 
										GetRebirt() == 7 || 
										GetRebirt() == 8 || 
										GetRebirt() == 9 || 
										GetRebirt() == 15)
									{
										EffectPacket(SE_DUEL_RECEIVER);
									}

									else if (GetRebirt() == 16 || 
										GetRebirt() == 7 || 
										GetRebirt() == 8 || 
										GetRebirt() == 9 || 
										GetRebirt() == 20)
									{
										EffectPacket(SE_DUEL_RECEIVER);
									}

									if (GetRebirt() >= 1 && GetRebirt() <= 5)
									{
										BYTE bRebirt = GetRebirt();
										DWORD PointCanavar = bRebirt*5;
										DWORD PointMetin = bRebirt*5;

										AddAffect(AFFECT_COLLECT, POINT_ATTBONUS_MONSTER, PointCanavar, 0, 60*60*24*365*60, 0, false);
										AddAffect(AFFECT_COLLECT, POINT_ATT_METIN, PointMetin, 0, 60*60*24*365*60, 0, false);

										ChatPacket(CHAT_TYPE_COMMAND, "rebirtodul2 %u %u", PointCanavar, PointMetin);
										PointsPacket();
									}
									else if (GetRebirt() >= 6 && GetRebirt() <= 10)
									{
										BYTE bRebirt = GetRebirt();
										DWORD PointCanavar = bRebirt*10;
										DWORD PointBoss = bRebirt*10;

										AddAffect(AFFECT_COLLECT, POINT_ATTBONUS_MONSTER, PointCanavar, 0, 60*60*24*365*60, 0, false);
										AddAffect(AFFECT_COLLECT, POINT_ATT_BOSS, PointBoss, 0, 60*60*24*365*60, 0, false);

										ChatPacket(CHAT_TYPE_COMMAND, "rebirtodul %u %u", PointCanavar, PointBoss);
										PointsPacket();
									}
									else if (GetRebirt() >= 11 && GetRebirt() <= 15)
									{
										BYTE bRebirt = GetRebirt();
										DWORD PointCanavar = bRebirt*15;
										DWORD PointGrade = bRebirt*10;

										AddAffect(AFFECT_COLLECT, POINT_ATTBONUS_MONSTER, PointCanavar, 0, 60*60*24*365*60, 0, false);
										AddAffect(AFFECT_COLLECT, POINT_ATT_GRADE_BONUS, PointGrade, 0, 60*60*24*365*60, 0, false);

										ChatPacket(CHAT_TYPE_COMMAND, "rebirtodul3 %u %u", PointCanavar, PointGrade);
										PointsPacket();
									}
									else if (GetRebirt() >= 16 && GetRebirt() <= 20)
									{
										BYTE bRebirt = GetRebirt();
										DWORD PointCanavar = bRebirt*20;
										DWORD PointMelee = bRebirt*2;

										AddAffect(AFFECT_COLLECT, POINT_ATTBONUS_MONSTER, PointCanavar, 0, 60*60*24*365*60, 0, false);
										AddAffect(AFFECT_COLLECT, POINT_MELEE_MAGIC_ATT_BONUS_PER, PointMelee, 0, 60*60*24*365*60, 0, false);

										ChatPacket(CHAT_TYPE_COMMAND, "rebirtodul4 %u %u", PointCanavar, PointMelee);
										PointsPacket();
									}
								}

								return;
							}
						}
						else
						{
							sys_log(1, "%s rebirt son seviyede.", GetName());
							return;
						}
					}
					else if (gPlayerMaxLevel <= GetLevel() && REBIRT_MAX <= GetRebirt())
					{
						if (GetExp() > 0)
							SetExp(0);

						return;
					}
					#else
					if (gPlayerMaxLevel <= GetLevel())
						return;
					#endif

					//if (IsBlockMode(BLOCK_POINT_EXP))
						//return;

#if defined(__CHATTING_WINDOW_RENEWAL__)
					if (amount > 0)
						ChatPacket(CHAT_TYPE_EXP_INFO, LC_TEXT("You have gained %d exp."), amount);
#else
					if (test_server)
						ChatPacket(CHAT_TYPE_INFO, "You have gained %d exp.", amount);
#endif

					DWORD iExpBalance = 0;
					if (exp + amount >= next_exp)
					{
						iExpBalance = (exp + amount) - next_exp;
						amount = next_exp - exp;

						SetExp(0);
						exp = next_exp;
					}
					else
					{
						SetExp(exp + amount);
						exp = GetExp();
					}

					DWORD q = DWORD(next_exp / 4.0f);
					int iLevStep = GetRealPoint(POINT_LEVEL_STEP);
					if (iLevStep >= 4)
					{
						sys_err("%s LEVEL_STEP bigger than 4! (%d)", GetName(), iLevStep);
						iLevStep = 4;
					}

					if (exp >= next_exp && iLevStep < 4)
					{
						for (int i = 0; i < 4 - iLevStep; ++i)
							PointChange(POINT_LEVEL_STEP, 1, false, true);
					}
					else if (exp >= q * 3 && iLevStep < 3)
					{
						for (int i = 0; i < 3 - iLevStep; ++i)
							PointChange(POINT_LEVEL_STEP, 1, false, true);
					}
					else if (exp >= q * 2 && iLevStep < 2)
					{
						for (int i = 0; i < 2 - iLevStep; ++i)
							PointChange(POINT_LEVEL_STEP, 1, false, true);
					}
					else if (exp >= q && iLevStep < 1)
						PointChange(POINT_LEVEL_STEP, 1);

					if (iExpBalance)
					{
						PointChange(POINT_EXP, iExpBalance);
					}

					val = GetExp();
				}
			}
			break;
			
		case POINT_LEVEL_STEP:
			if (amount > 0)
			{
				val = GetPoint(POINT_LEVEL_STEP) + amount;

				switch (val)
				{
					case 1:
					case 2:
					case 3:
						//if (GetLevel() < 100) PointChange(POINT_STAT, 1);
						if (GetLevel() < 91) PointChange(POINT_STAT, 1);
						break;

					case 4:
						{
							int iHP = number(JobInitialPoints[GetJob()].hp_per_lv_begin, JobInitialPoints[GetJob()].hp_per_lv_end);
							int iSP = number(JobInitialPoints[GetJob()].sp_per_lv_begin, JobInitialPoints[GetJob()].sp_per_lv_end);

							m_points.iRandomHP += iHP;
							m_points.iRandomSP += iSP;

							if (GetSkillGroup())
							{
								if (GetLevel() >= 5)
									PointChange(POINT_SKILL, 1);

								if (GetLevel() >= 9)
									PointChange(POINT_SUB_SKILL, 1);
							}

							PointChange(POINT_MAX_HP, iHP);
							PointChange(POINT_MAX_SP, iSP);
							PointChange(POINT_LEVEL, 1, false, true);

							val = 0;
						}
						break;
				}

				/*if (GetLevel() <= 10)
					AutoGiveItem(27001, 2);
				else if (GetLevel() <= 30)
					AutoGiveItem(27002, 2);
				else
				{
					AutoGiveItem(27002, 2);
//					AutoGiveItem(27003, 2);
				}*/

				PointChange(POINT_HP, GetMaxHP() - GetHP());
				PointChange(POINT_SP, GetMaxSP() - GetSP());
				PointChange(POINT_STAMINA, GetMaxStamina() - GetStamina());

				SetPoint(POINT_LEVEL_STEP, val);
				SetRealPoint(POINT_LEVEL_STEP, val);

				Save();
			}
			else
				val = GetPoint(POINT_LEVEL_STEP);

			break;

		case POINT_HP:
			{
				if (IsDead() || IsStun())
					return;

				int prev_hp = GetHP();

				amount = MIN(GetMaxHP() - GetHP(), amount);
				SetHP(GetHP() + amount);
				val = GetHP();

				BroadcastTargetPacket();

				if (GetParty() && IsPC() && val != prev_hp)
					GetParty()->SendPartyInfoOneToAll(this);
			}
			break;

		case POINT_SP:
			{
				if (IsDead() || IsStun())
					return;

				amount = MIN(GetMaxSP() - GetSP(), amount);
				SetSP(GetSP() + amount);
				val = GetSP();
			}
			break;

		case POINT_STAMINA:
			{
				if (IsDead() || IsStun())
					return;

				int prev_val = GetStamina();
				amount = MIN(GetMaxStamina() - GetStamina(), amount);
				SetStamina(GetStamina() + amount);
				val = GetStamina();
				
				if (val == 0)
				{
					// Stamina�� ������ ����!
					SetNowWalking(true);
				}
				else if (prev_val == 0)
				{
					// ���� ���׹̳��� �������� ���� ��� ����
					ResetWalking();
				}

				if (amount < 0 && val != 0) // ���Ҵ� �������ʴ´�.
					return;
			}
			break;

		case POINT_MAX_HP:
			{
				SetPoint(type, GetPoint(type) + amount);

				//SetMaxHP(GetMaxHP() + amount);
				// �ִ� ����� = (�⺻ �ִ� ����� + �߰�) * �ִ�����%
				int64_t hp = GetRealPoint(POINT_MAX_HP);
				int64_t add_hp = MIN(3500, (hp / 100) * GetPoint(POINT_MAX_HP_PCT));
				add_hp += GetPoint(POINT_MAX_HP);
				add_hp += GetPoint(POINT_PARTY_TANKER_BONUS);

				SetMaxHP(hp + add_hp);

				val = GetMaxHP();
			}
			break;

		case POINT_MAX_SP:
			{
				SetPoint(type, GetPoint(type) + amount);

				//SetMaxSP(GetMaxSP() + amount);
				// �ִ� ���ŷ� = (�⺻ �ִ� ���ŷ� + �߰�) * �ִ����ŷ�%
				int sp = GetRealPoint(POINT_MAX_SP);
				int add_sp = MIN(800, sp * GetPoint(POINT_MAX_SP_PCT) / 100);
				add_sp += GetPoint(POINT_MAX_SP);
				add_sp += GetPoint(POINT_PARTY_SKILL_MASTER_BONUS);

				SetMaxSP(sp + add_sp);

				val = GetMaxSP();
			}
			break;

		case POINT_MAX_HP_PCT:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);

			PointChange(POINT_MAX_HP, 0);
			break;

		case POINT_MAX_SP_PCT:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);

			PointChange(POINT_MAX_SP, 0);
			break;

		case POINT_MAX_STAMINA:
			SetMaxStamina(GetMaxStamina() + amount);
			val = GetMaxStamina();
			break;
#ifndef YANG_LIMIT
		case POINT_GOLD:
			{
				const int64_t nTotalMoney = static_cast<int64_t>(GetGold()) + static_cast<int64_t>(amount);

				if (GOLD_MAX <= nTotalMoney)
				{
					sys_err("[OVERFLOW_GOLD] OriGold %d AddedGold %d id %u Name %s ", GetGold(), amount, GetPlayerID(), GetName());
					LogManager::instance().CharLog(this, GetGold() + amount, "OVERFLOW_GOLD", "");
					return;
				}

				// u????
				if (LC_IsNewCIBN() && amount > 0)
				{
					if (IsOverTime(OT_NONE))
					{
						dev_log(LOG_DEB0, "<GOLD_LOG> %s = NONE", GetName());
					}
					else if (IsOverTime(OT_3HOUR))
					{
						amount = (amount / 2);
						dev_log(LOG_DEB0, "<GOLD_LOG> %s = 3HOUR", GetName());
					}
					else if (IsOverTime(OT_5HOUR))
					{
						amount = 0;
						dev_log(LOG_DEB0, "<GOLD_LOG> %s = 5HOUR", GetName());
					}
				}

				SetGold(GetGold() + amount);
				val = GetGold();
			}
			break;
#endif
#ifdef ENABLE_CHEQUE_SYSTEM
		case POINT_CHEQUE:
			{
				const int64_t nTotalCheque = static_cast<int64_t>(GetCheque()) + static_cast<int64_t>(amount);
				if (CHEQUE_MAX <= nTotalCheque)
				{
					sys_err("[OVERFLOW_CHEQUE] OriCheque %d AddedCheque %d id %u Name %s ", GetCheque(), amount, GetPlayerID(), GetName());
					LogManager::instance().CharLog(this, GetCheque() + amount, "OVERFLOW_CHEQUE", "");
					return;
				}
				SetCheque(GetCheque() + amount);
				val = GetCheque();
			}
			break;
#endif
		#ifdef ENABLE_REBIRT_SYSTEM
		case POINT_REBIRT:
			{
				const int64_t nTotalRebirt = static_cast<int64_t>(GetRebirt()) + static_cast<int64_t>(amount);

				if (nTotalRebirt < 0)
				{
					sys_err("[OVERFLOW_CHEQUE] Rebirt eksi deger rebirti %u Name %s ", GetRebirt(), GetName());
					return;
				}

				if (nTotalRebirt >= REBIRT_MAX)
				{
					sys_err("[OVERFLOW_CHEQUE] Rebirt max seviyede rebirti %u Name %s ", GetRebirt(), GetName());
					return;
				}

				if (LC_IsNewCIBN() && amount > 0)
				{
					if (IsOverTime(OT_NONE))
					{
						dev_log(LOG_DEB0, "<REBIRT_LOG> %s = NONE", GetName());
					}
					else if (IsOverTime(OT_3HOUR))
					{
						amount = (amount / 2);
						dev_log(LOG_DEB0, "<REBIRT_LOG> %s = 3HOUR", GetName());
					}
					else if (IsOverTime(OT_5HOUR))
					{
						amount = 0;
						dev_log(LOG_DEB0, "<REBIRT_LOG> %s = 5HOUR", GetName());
					}
				}

				SetRebirt(GetRebirt() + amount);
				val = GetRebirt();
			}
			break;
		#endif
		case POINT_GAYA:
			{
				const int64_t nTotalGaya = static_cast<int64_t>(GetGaya()) + static_cast<int64_t>(amount);
				if (GAYA_MAX <= nTotalGaya)
				{
					sys_err("[OVERFLOW_GAYA] OriGaya %d AddedGaya %d id %u Name %s ", GetGaya(), amount, GetPlayerID(), GetName());
					LogManager::instance().CharLog(this, GetGaya() + amount, "OVERFLOW_GAYA", "");
					return;
				}
				SetGaya(GetGaya() + amount);
				val = GetGaya();
			}
			break;
		case POINT_RED_GAYA:
			{
				const int64_t nTotalRedGaya = static_cast<int64_t>(GetRedGaya()) + static_cast<int64_t>(amount);
				if (RED_GAYA_MAX <= nTotalRedGaya)
				{
					sys_err("[OVERFLOW_RED_GAYA] OriRedGaya %d AddedRedGaya %d id %u Name %s ", GetRedGaya(), amount, GetPlayerID(), GetName());
					LogManager::instance().CharLog(this, GetRedGaya() + amount, "OVERFLOW_RED_GAYA", "");
					return;
				}
				SetRedGaya(GetRedGaya() + amount);
				val = GetRedGaya();
			}
			break;
		case POINT_SKILL:
		case POINT_STAT:
		case POINT_SUB_SKILL:
		case POINT_STAT_RESET_COUNT:
		case POINT_HORSE_SKILL:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);

			SetRealPoint(type, val);
			break;

		case POINT_DEF_GRADE:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);

			PointChange(POINT_CLIENT_DEF_GRADE, amount);
			break;

		case POINT_CLIENT_DEF_GRADE:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			break;

		case POINT_ST:
		case POINT_HT:
		case POINT_DX:
		case POINT_IQ:
		case POINT_HP_REGEN:
		case POINT_SP_REGEN:
		case POINT_ATT_SPEED:
		case POINT_ATT_GRADE:
		case POINT_MOV_SPEED:
		case POINT_CASTING_SPEED:
		case POINT_MAGIC_ATT_GRADE:
		case POINT_MAGIC_DEF_GRADE:
		case POINT_BOW_DISTANCE:
		case POINT_HP_RECOVERY:
		case POINT_SP_RECOVERY:

		case POINT_ATTBONUS_HUMAN:	// 42 �ΰ����� ����
		case POINT_ATTBONUS_ANIMAL:	// 43 �������� ������ % ����
		case POINT_ATTBONUS_ORC:		// 44 ���Ϳ��� ������ % ����
		case POINT_ATTBONUS_MILGYO:	// 45 �б����� ������ % ����
		case POINT_ATTBONUS_UNDEAD:	// 46 ��ü���� ������ % ����
		case POINT_ATTBONUS_DEVIL:	// 47 ����(�Ǹ�)���� ������ % ����
#ifdef ADD_NEW_BONUS
		case POINT_ATT_METIN:
		case POINT_RESIST_MONSTER:
		case POINT_ATTBONUS_ELEC:
		case POINT_ATTBONUS_FIRE:
		case POINT_ATTBONUS_ICE:
		case POINT_ATTBONUS_WIND:
		case POINT_ATTBONUS_EARTH:
		case POINT_ATTBONUS_DARK:
		case POINT_ANTI_SWORD:
		case POINT_ANTI_TWOHAND:
		case POINT_ANTI_DAGGER:
		case POINT_ANTI_BELL:
		case POINT_ANTI_FAN:
		case POINT_ANTI_BOW:
		case POINT_ANTI_HUMAN:
		case POINT_ATT_MONSTER_NEW:
		case POINT_ATT_BOSS:		
		case POINT_ATTBONUS_ELEMENT:
#endif
		case POINT_ATTBONUS_MONSTER:
		case POINT_ATTBONUS_SURA:
		case POINT_ATTBONUS_ASSASSIN:
		case POINT_ATTBONUS_WARRIOR:
		case POINT_ATTBONUS_SHAMAN:
		case POINT_ATTBONUS_WOLFMAN:

		case POINT_POISON_PCT:
		case POINT_STUN_PCT:
		case POINT_SLOW_PCT:

		case POINT_BLOCK:
		case POINT_DODGE:

		case POINT_CRITICAL_PCT:
		case POINT_RESIST_CRITICAL:
		case POINT_PENETRATE_PCT:
		case POINT_RESIST_PENETRATE:
		case POINT_CURSE_PCT:

		case POINT_STEAL_HP:		// 48 ����� ���
		case POINT_STEAL_SP:		// 49 ���ŷ� ���

		case POINT_MANA_BURN_PCT:	// 50 ���� ��
		case POINT_DAMAGE_SP_RECOVER:	// 51 ���ݴ��� �� ���ŷ� ȸ�� Ȯ��
		case POINT_RESIST_NORMAL_DAMAGE:
		case POINT_RESIST_SWORD:
		case POINT_RESIST_TWOHAND:
		case POINT_RESIST_DAGGER:
		case POINT_RESIST_BELL: 
		case POINT_RESIST_FAN: 
		case POINT_RESIST_BOW:
		case POINT_RESIST_CLAW:
		case POINT_RESIST_FIRE:
		case POINT_RESIST_ELEC:
		case POINT_RESIST_MAGIC:
		case POINT_RESIST_WIND:
		case POINT_RESIST_ICE:
		case POINT_RESIST_EARTH:
		case POINT_RESIST_DARK:
		case POINT_REFLECT_MELEE:	// 67 ���� �ݻ�
		case POINT_REFLECT_CURSE:	// 68 ���� �ݻ�
		case POINT_POISON_REDUCE:	// 69 �������� ����
		case POINT_KILL_SP_RECOVER:	// 70 �� �Ҹ�� MP ȸ��
		case POINT_KILL_HP_RECOVERY:	// 75  
		case POINT_HIT_HP_RECOVERY:
		case POINT_HIT_SP_RECOVERY:
		case POINT_MANASHIELD:
		case POINT_ATT_BONUS:
		case POINT_DEF_BONUS:
		case POINT_SKILL_DAMAGE_BONUS:
		case POINT_NORMAL_HIT_DAMAGE_BONUS:


			// DEPEND_BONUS_ATTRIBUTES 
		case POINT_SKILL_DEFEND_BONUS:
		case POINT_NORMAL_HIT_DEFEND_BONUS:
#ifdef ENABLE_GLOVE_SYSTEM
		case POINT_RANDOM:
#endif
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			break;
			// END_OF_DEPEND_BONUS_ATTRIBUTES

#ifdef WJ_MAGIC_REDUCION_BONUS
		case POINT_ANTI_RESIST_MAGIC:
			if (GetRaceNum() == 2 or GetRaceNum() == 6)
				amount = amount / 2;
			
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			break;
#endif						
			
		case POINT_PARTY_ATTACKER_BONUS:
		case POINT_PARTY_TANKER_BONUS:
		case POINT_PARTY_BUFFER_BONUS:
		case POINT_PARTY_SKILL_MASTER_BONUS:
		case POINT_PARTY_HASTE_BONUS:
		case POINT_PARTY_DEFENDER_BONUS:

		case POINT_RESIST_WARRIOR :
		case POINT_RESIST_ASSASSIN :
		case POINT_RESIST_SURA :
		case POINT_RESIST_SHAMAN :
		case POINT_RESIST_WOLFMAN:

			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			break;

		case POINT_MALL_ATTBONUS:
		case POINT_MALL_DEFBONUS:
		case POINT_MALL_EXPBONUS:
		case POINT_MALL_ITEMBONUS:
		case POINT_MALL_GOLDBONUS:
		case POINT_MELEE_MAGIC_ATT_BONUS_PER:
#ifdef ENABLE_YMIR_EXP_BONUS_EDIT
			if (GetPoint(type) + amount > 400)
			{
				sys_err("MALL_BONUS exceeded over 400!! point type: %d name: %s amount %d", type, GetName(), amount);
				amount = 400 - GetPoint(type);
			}
#else
			if (GetPoint(type) + amount > 100)
			{
				sys_err("MALL_BONUS exceeded over 100!! point type: %d name: %s amount %d", type, GetName(), amount);
			amount = 100 - GetPoint(type);
			}
#endif

			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			break;

			// PC_BANG_ITEM_ADD		
		case POINT_PC_BANG_EXP_BONUS :
		case POINT_PC_BANG_DROP_BONUS :
		case POINT_RAMADAN_CANDY_BONUS_EXP:
			SetPoint(type, amount);
			val = GetPoint(type);
			break;
			// END_PC_BANG_ITEM_ADD		

		case POINT_EXP_DOUBLE_BONUS:	// 71  
		case POINT_GOLD_DOUBLE_BONUS:	// 72  
		case POINT_ITEM_DROP_BONUS:	// 73  
		case POINT_POTION_BONUS:	// 74
#ifdef ENABLE_YMIR_EXP_BONUS_EDIT
			if (GetPoint(type) + amount > 400)
			{
				sys_err("BONUS exceeded over 400!! point type: %d name: %s amount %d", type, GetName(), amount);
				amount = 400 - GetPoint(type);
			}
#else
			if (GetPoint(type) + amount > 100)
			{
				sys_err("BONUS exceeded over 100!! point type: %d name: %s amount %d", type, GetName(), amount);
				amount = 100 - GetPoint(type);
			}
#endif

			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			break;

		case POINT_IMMUNE_STUN:		// 76 
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			if (val)
			{
				SET_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_STUN);
			}
			else
			{
				REMOVE_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_STUN);
			}
			break;

		case POINT_IMMUNE_SLOW:		// 77  
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			if (val)
			{
				SET_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_SLOW);
			}
			else
			{
				REMOVE_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_SLOW);
			}
			break;

		case POINT_IMMUNE_FALL:	// 78   
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			if (val)
			{
				SET_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_FALL);
			}
			else
			{
				REMOVE_BIT(m_pointsInstant.dwImmuneFlag, IMMUNE_FALL);
			}
			break;

		case POINT_ATT_GRADE_BONUS:
			SetPoint(type, GetPoint(type) + amount);
			PointChange(POINT_ATT_GRADE, amount);
			val = GetPoint(type);
			break;

		case POINT_DEF_GRADE_BONUS:
			SetPoint(type, GetPoint(type) + amount);
			PointChange(POINT_DEF_GRADE, amount);
			val = GetPoint(type);
			break;

		case POINT_MAGIC_ATT_GRADE_BONUS:
			SetPoint(type, GetPoint(type) + amount);
			PointChange(POINT_MAGIC_ATT_GRADE, amount);
			val = GetPoint(type);
			break;

		case POINT_MAGIC_DEF_GRADE_BONUS:
			SetPoint(type, GetPoint(type) + amount);
			PointChange(POINT_MAGIC_DEF_GRADE, amount);
			val = GetPoint(type);
			break;

		case POINT_VOICE:
		case POINT_EMPIRE_POINT:
			//sys_err("CHARACTER::PointChange: %s: point cannot be changed. use SetPoint instead (type: %d)", GetName(), type);
			val = GetRealPoint(type);
			break;

		case POINT_POLYMORPH:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			SetPolymorph(val);
			break;

		case POINT_MOUNT:
			SetPoint(type, GetPoint(type) + amount);
			val = GetPoint(type);
			MountVnum(val);
			break;

		case POINT_ENERGY:
		case POINT_COSTUME_ATTR_BONUS:
			{
				int old_val = GetPoint(type);
				SetPoint(type, old_val + amount);
				val = GetPoint(type);
				BuffOnAttr_ValueChange(type, old_val, val);
			}
			break;

		default:
			sys_err("CHARACTER::PointChange: %s: unknown point change type %d", GetName(), type);
			return;
	}

	switch (type)
	{
		case POINT_LEVEL:
		case POINT_ST:
		case POINT_DX:
		case POINT_IQ:
		case POINT_HT:
			ComputeBattlePoints();
			break;
		case POINT_MAX_HP:
		case POINT_MAX_SP:
		case POINT_MAX_STAMINA:
			break;
	}

	if (type == POINT_HP && amount == 0)
		return;

	if (GetDesc())
	{
		struct packet_point_change pack;

		pack.header = HEADER_GC_CHARACTER_POINT_CHANGE;
		pack.dwVID = m_vid;
		pack.type = type;
		pack.value = val;

		if (bAmount)
			pack.amount = amount;
		else
			pack.amount = 0;

		if (!bBroadcast)
			GetDesc()->Packet(&pack, sizeof(struct packet_point_change));
		else
			PacketAround(&pack, sizeof(pack));
	}
}

#ifdef ENABLE_NEW_PET_SYSTEM
void CHARACTER::SendPetLevelUpEffect(int vid, int type, int value, int amount)
{
	struct packet_point_change pack;

	pack.header = HEADER_GC_CHARACTER_POINT_CHANGE;
	pack.dwVID = vid;
	pack.type = type;
	pack.value = value;
	pack.amount = amount;
	PacketAround(&pack, sizeof(pack));
}
#endif

void CHARACTER::ApplyPoint(BYTE bApplyType, int64_t iVal)
{
	switch (bApplyType)
	{
		case APPLY_NONE:			// 0
			break;

		case APPLY_CON:
			PointChange(POINT_HT, iVal);
			PointChange(POINT_MAX_HP, (iVal * JobInitialPoints[GetJob()].hp_per_ht));
			PointChange(POINT_MAX_STAMINA, (iVal * JobInitialPoints[GetJob()].stamina_per_con));
			break;

		case APPLY_INT: 
			PointChange(POINT_IQ, iVal);
			PointChange(POINT_MAX_SP, (iVal * JobInitialPoints[GetJob()].sp_per_iq));
			break;

		case APPLY_SKILL:
			// SKILL_DAMAGE_BONUS
			{
				// �ֻ��� ��Ʈ �������� 8��Ʈ vnum, 9��Ʈ add, 15��Ʈ change
				// 00000000 00000000 00000000 00000000
				// ^^^^^^^^  ^^^^^^^^^^^^^^^^^^^^^^^^^
				// vnum     ^ add       change
				BYTE bSkillVnum = (BYTE) (((DWORD)iVal) >> 24);
				int iAdd = iVal & 0x00800000;
				int iChange = iVal & 0x007fffff;

				sys_log(1, "APPLY_SKILL skill %d add? %d change %d", bSkillVnum, iAdd ? 1 : 0, iChange);

				if (0 == iAdd)
					iChange = -iChange;

				boost::unordered_map<BYTE, int>::iterator iter = m_SkillDamageBonus.find(bSkillVnum);

				if (iter == m_SkillDamageBonus.end())
					m_SkillDamageBonus.insert(std::make_pair(bSkillVnum, iChange));
				else
					iter->second += iChange;
			}
			// END_OF_SKILL_DAMAGE_BONUS
			break;

		case APPLY_STR:
		case APPLY_DEX:
		case APPLY_MAX_HP:
		case APPLY_MAX_SP:
		case APPLY_MAX_HP_PCT:
		case APPLY_MAX_SP_PCT:
		case APPLY_ATT_SPEED:
		case APPLY_MOV_SPEED:
		case APPLY_CAST_SPEED:
		case APPLY_HP_REGEN:
		case APPLY_SP_REGEN:
		case APPLY_POISON_PCT:
		case APPLY_STUN_PCT:
		case APPLY_SLOW_PCT:
		case APPLY_CRITICAL_PCT:
		case APPLY_PENETRATE_PCT:
		case APPLY_ATTBONUS_HUMAN:
		case APPLY_ATTBONUS_ANIMAL:
		case APPLY_ATTBONUS_ORC:
		case APPLY_ATTBONUS_MILGYO:
		case APPLY_ATTBONUS_UNDEAD:
		case APPLY_ATTBONUS_DEVIL:
		case APPLY_ATTBONUS_WARRIOR:	// 59
		case APPLY_ATTBONUS_ASSASSIN:	// 60
		case APPLY_ATTBONUS_SURA:	// 61
		case APPLY_ATTBONUS_SHAMAN:	// 62
		case APPLY_ATTBONUS_MONSTER:	// 63
		case APPLY_STEAL_HP:
		case APPLY_STEAL_SP:
		case APPLY_MANA_BURN_PCT:
		case APPLY_DAMAGE_SP_RECOVER:
		case APPLY_BLOCK:
		case APPLY_DODGE:
		case APPLY_RESIST_SWORD:
		case APPLY_RESIST_TWOHAND:
		case APPLY_RESIST_DAGGER:
		case APPLY_RESIST_BELL:
		case APPLY_RESIST_FAN:
		case APPLY_RESIST_BOW:
		case APPLY_RESIST_FIRE:
		case APPLY_RESIST_ELEC:
		case APPLY_RESIST_MAGIC:
		case APPLY_RESIST_WIND:
		case APPLY_RESIST_ICE:
		case APPLY_RESIST_EARTH:
		case APPLY_RESIST_DARK:
		case APPLY_REFLECT_MELEE:
		case APPLY_REFLECT_CURSE:
		case APPLY_ANTI_CRITICAL_PCT:
		case APPLY_ANTI_PENETRATE_PCT:
		case APPLY_POISON_REDUCE:
		case APPLY_KILL_SP_RECOVER:
		case APPLY_EXP_DOUBLE_BONUS:
		case APPLY_GOLD_DOUBLE_BONUS:
		case APPLY_ITEM_DROP_BONUS:
		case APPLY_POTION_BONUS:
		case APPLY_KILL_HP_RECOVER:
		case APPLY_IMMUNE_STUN:	
		case APPLY_IMMUNE_SLOW:	
		case APPLY_IMMUNE_FALL:	
		case APPLY_BOW_DISTANCE:
		case APPLY_ATT_GRADE_BONUS:
		case APPLY_DEF_GRADE_BONUS:
		case APPLY_MAGIC_ATT_GRADE:
		case APPLY_MAGIC_DEF_GRADE:
		case APPLY_CURSE_PCT:
		case APPLY_MAX_STAMINA:
		case APPLY_MALL_ATTBONUS:
		case APPLY_MALL_DEFBONUS:
		case APPLY_MALL_EXPBONUS:
		case APPLY_MALL_ITEMBONUS:
		case APPLY_MALL_GOLDBONUS:
		case APPLY_SKILL_DAMAGE_BONUS:
		case APPLY_NORMAL_HIT_DAMAGE_BONUS:

			// DEPEND_BONUS_ATTRIBUTES
		case APPLY_SKILL_DEFEND_BONUS:
		case APPLY_NORMAL_HIT_DEFEND_BONUS:
			// END_OF_DEPEND_BONUS_ATTRIBUTES

		case APPLY_PC_BANG_EXP_BONUS :
		case APPLY_PC_BANG_DROP_BONUS :

		case APPLY_RESIST_WARRIOR :
		case APPLY_RESIST_ASSASSIN :
		case APPLY_RESIST_SURA :
		case APPLY_RESIST_SHAMAN :	
		case APPLY_ENERGY:					// 82 ���
		case APPLY_DEF_GRADE:				// 83 ����. DEF_GRADE_BONUS�� Ŭ�󿡼� �ι�� �������� �ǵ��� ����(...)�� �ִ�.
		case APPLY_COSTUME_ATTR_BONUS:		// 84 �ڽ�Ƭ �����ۿ� ���� �Ӽ�ġ ���ʽ�
		case APPLY_MAGIC_ATTBONUS_PER:		// 85 ���� ���ݷ� +x%
		case APPLY_MELEE_MAGIC_ATTBONUS_PER:			// 86 ���� + �и� ���ݷ� +x%
		case APPLY_RESIST_WOLFMAN:
		case APPLY_RESIST_CLAW:
#ifdef ENABLE_GLOVE_SYSTEM
		case APPLY_RANDOM:
#endif
		case APPLY_ATTBONUS_WOLFMAN:
#ifdef WJ_MAGIC_REDUCION_BONUS
		case APPLY_ANTI_RESIST_MAGIC:
#endif			
#ifdef ADD_NEW_BONUS	
		case APPLY_ATTBONUS_METIN:
		case APPLY_RESIST_MONSTER:
		case APPLY_ATTBONUS_ELEC:
		case APPLY_ATTBONUS_FIRE:
		case APPLY_ATTBONUS_ICE:
		case APPLY_ATTBONUS_WIND:
		case APPLY_ATTBONUS_EARTH:
		case APPLY_ATTBONUS_DARK:
		case APPLY_ANTI_SWORD:
		case APPLY_ANTI_TWOHAND:
		case APPLY_ANTI_DAGGER:
		case APPLY_ANTI_BELL:
		case APPLY_ANTI_FAN:
		case APPLY_ANTI_BOW:
		case APPLY_ANTI_HUMAN:
		case APPLY_ATT_MONSTER_NEW:
		case APPLY_ATT_BOSS:		
		case APPLY_ATTBONUS_ELEMENT:
#endif
			PointChange(aApplyInfo[bApplyType].bPointType, iVal);
			break;

		default:
			sys_err("Unknown apply type %d name %s", bApplyType, GetName());
			break;
	}
}

void CHARACTER::MotionPacketEncode(BYTE motion, LPCHARACTER victim, struct packet_motion * packet)
{
	packet->header	= HEADER_GC_MOTION;
	packet->vid		= m_vid;
	packet->motion	= motion;

	if (victim)
		packet->victim_vid = victim->GetVID();
	else
		packet->victim_vid = 0;
}

void CHARACTER::Motion(BYTE motion, LPCHARACTER victim)
{
	struct packet_motion pack_motion;
	MotionPacketEncode(motion, victim, &pack_motion);
	PacketAround(&pack_motion, sizeof(struct packet_motion));
}

EVENTFUNC(save_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info );
	if ( info == NULL )
	{
		sys_err( "save_event> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER ch = info->ch;

	if (ch == NULL) { // <Factor>
		return 0;
	}	
	sys_log(1, "SAVE_EVENT: %s", ch->GetName());
	ch->Save();
	ch->FlushDelayedSaveItem();
	return (save_event_second_cycle);
}

void CHARACTER::StartSaveEvent()
{
	if (m_pkSaveEvent)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;
	m_pkSaveEvent = event_create(save_event, info, save_event_second_cycle);
}

void CHARACTER::MonsterLog(const char* format, ...)
{
	if (!test_server)
		return;

	if (IsPC())
		return;

	char chatbuf[CHAT_MAX_LEN + 1];
	int len = snprintf(chatbuf, sizeof(chatbuf), "%u)", (DWORD)GetVID());

	if (len < 0 || len >= (int) sizeof(chatbuf))
		len = sizeof(chatbuf) - 1;

	va_list args;

	va_start(args, format);

	int len2 = vsnprintf(chatbuf + len, sizeof(chatbuf) - len, format, args);

	if (len2 < 0 || len2 >= (int) sizeof(chatbuf) - len)
		len += (sizeof(chatbuf) - len) - 1;
	else
		len += len2;

	// \0 ���� ����
	++len;

	va_end(args);

	TPacketGCChat pack_chat;

	pack_chat.header    = HEADER_GC_CHAT;
	pack_chat.size		= sizeof(TPacketGCChat) + len;
	pack_chat.type      = CHAT_TYPE_TALKING;
	pack_chat.id        = (DWORD)GetVID();
	pack_chat.bEmpire	= 0;

	TEMP_BUFFER buf;
	buf.write(&pack_chat, sizeof(TPacketGCChat));
	buf.write(chatbuf, len);

	CHARACTER_MANAGER::instance().PacketMonsterLog(this, buf.read_peek(), buf.size());
}

void CHARACTER::ChatPacket(BYTE type, const char * format, ...)
{
	LPDESC d = GetDesc();

	if (!d || !format)
		return;

	char chatbuf[CHAT_MAX_LEN + 1];
	va_list args;

	va_start(args, format);
	int len = vsnprintf(chatbuf, sizeof(chatbuf), format, args);
	va_end(args);

	struct packet_chat pack_chat;

	pack_chat.header    = HEADER_GC_CHAT;
	pack_chat.size      = sizeof(struct packet_chat) + len;
	pack_chat.type      = type;
	pack_chat.id        = 0;
	pack_chat.bEmpire   = d->GetEmpire();

	TEMP_BUFFER buf;
	buf.write(&pack_chat, sizeof(struct packet_chat));
	buf.write(chatbuf, len);

	d->Packet(buf.read_peek(), buf.size());

	if (type == CHAT_TYPE_COMMAND && test_server)
		sys_log(0, "SEND_COMMAND %s %s", GetName(), chatbuf);
}

// MINING
void CHARACTER::mining_take()
{
	m_pkMiningEvent = NULL;
}

void CHARACTER::mining_cancel()
{
	if (m_pkMiningEvent)
	{
		sys_log(0, "XXX MINING CANCEL");
		event_cancel(&m_pkMiningEvent);
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ä���� �ߴ��Ͽ����ϴ�."));
	}
}

void CHARACTER::mining(LPCHARACTER chLoad)
{
	if (m_pkMiningEvent)
	{
		mining_cancel();
		return;
	}

	if (!chLoad)
		return;

	if (mining::GetRawOreFromLoad(chLoad->GetRaceNum()) == 0)
		return;

	LPITEM pick = GetWear(WEAR_WEAPON);

	if (!pick || pick->GetType() != ITEM_PICK)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��̸� �����ϼ���."));
		return;
	}

	int count = number(5, 15); // ���� Ƚ��, �� ���۴� 2��

	// ä�� ������ ������
	TPacketGCDigMotion p;
	p.header = HEADER_GC_DIG_MOTION;
	p.vid = GetVID();
	p.target_vid = chLoad->GetVID();
	p.count = count;

	PacketAround(&p, sizeof(p));

	m_pkMiningEvent = mining::CreateMiningEvent(this, chLoad, count);
}
// END_OF_MINING

void CHARACTER::fishing()
{
	if (m_pkFishingEvent)
	{
		fishing_take();
		return;
	}

	// ���� �Ӽ����� ���ø� �õ��Ѵ�?
	{
		LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap(GetMapIndex());

		int	x = GetX();
		int y = GetY();

		LPSECTREE tree = pkSectreeMap->Find(x, y);
		DWORD dwAttr = tree->GetAttribute(x, y);

		if (IS_SET(dwAttr, ATTR_BLOCK))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���ø� �� �� �ִ� ���� �ƴմϴ�"));
			return;
		}
	}

	LPITEM rod = GetWear(WEAR_WEAPON);

	// ���ô� ����
	if (!rod || rod->GetType() != ITEM_ROD)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���ô븦 ���� �ϼ���."));
		return;
	}

	if (0 == rod->GetSocket(2))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̳��� ���� ���� �ּ���."));
		return;
	}

	float fx, fy;
	GetDeltaByDegree(GetRotation(), 400.0f, &fx, &fy);

	m_pkFishingEvent = fishing::CreateFishingEvent(this);
}

void CHARACTER::fishing_take()
{
	LPITEM rod = GetWear(WEAR_WEAPON);
	if (rod && rod->GetType() == ITEM_ROD)
	{
		using fishing::fishing_event_info;
		if (m_pkFishingEvent)
		{
			struct fishing_event_info* info = dynamic_cast<struct fishing_event_info*>(m_pkFishingEvent->info);

			if (info)
				fishing::Take(info, this);
		}
	}
	else
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���ô밡 �ƴ� �������� ���ø� �� �� �����ϴ�!"));
	}

	event_cancel(&m_pkFishingEvent);
}

bool CHARACTER::StartStateMachine(int iNextPulse)
{
	if (CHARACTER_MANAGER::instance().AddToStateList(this))
	{
		m_dwNextStatePulse = thecore_heart->pulse + iNextPulse;
		return true;
	}

	return false;
}

void CHARACTER::StopStateMachine()
{
	CHARACTER_MANAGER::instance().RemoveFromStateList(this);
}

void CHARACTER::UpdateStateMachine(DWORD dwPulse)
{
	if (dwPulse < m_dwNextStatePulse)
		return;

	if (IsDead())
		return;
	if (!this)
	{
		//sys_err( "UpdateStateMachine - Core Engel!" );
		return;
	}
	
	if (IsPC() && (!GetDesc() || !GetDesc()->GetCharacter()))
	{
		//sys_err( "UpdateStateMachine - Core Engel 2!" );
		return;
	}

	Update();
	m_dwNextStatePulse = dwPulse + m_dwStateDuration;
}

void CHARACTER::SetNextStatePulse(int iNextPulse)
{
	CHARACTER_MANAGER::instance().AddToStateList(this);
	m_dwNextStatePulse = iNextPulse;

	if (iNextPulse < 10)
		MonsterLog("�������·ξ����");
}

// ĳ���� �ν��Ͻ� ������Ʈ �Լ�.
void CHARACTER::UpdateCharacter(DWORD dwPulse)
{
	if (!this)
	{
		//sys_err( "UpdateStateMachine - Core Engel!" );
		return;
	}
	CFSM::Update();
}

void CHARACTER::SetShop(LPSHOP pkShop)
{
	if ((m_pkShop = pkShop))
		SET_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_SHOP);
	else
	{
		REMOVE_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_SHOP); 
		SetShopOwner(NULL);
	}
}

void CHARACTER::SetOfflineShop(LPOFFLINESHOP pkOfflineShop)
{
	if ((m_pkOfflineShop = pkOfflineShop))
		SET_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_OFFLINE_SHOP);
	else{
		REMOVE_BIT(m_pointsInstant.instant_flag, INSTANT_FLAG_OFFLINE_SHOP);
		SetOfflineShopOwner(NULL);
	}
}

void CHARACTER::OpenMyOfflineShop(const char * c_pszSign, TShopItemTable * pTable, short bItemCount, BYTE bTime)
{
	if (thecore_pulse() - GetMyOfflineShopTime() < PASSES_PER_SEC(10))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You're too fast! Slow down!"));
		return;
	}

	SetMyOfflineShopTime();
	
	quest::PC * pPC = quest::CQuestManager::Instance().GetPC(GetPlayerID());

	if (pPC->IsRunning()) {return;}
	if (bItemCount == 0) {return;}
	if (GetShop() || GetOfflineShop()) {return;}
	if (IsWarping()) {return;}
	
	if (m_pkTimedEventCh)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Hizli ch durduruldu."));
		event_cancel(&m_pkTimedEventCh);
	}
	
#ifdef WJ_SECURITY_SYSTEM
	if (IsActivateSecurity() == true)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GUVENLIK_AKTIF_HATA"));
		return;
	}
#endif
	if (GetOfflineShopCount() > g_maxOfflineShopCount && !FindAffect(AFFECT_SHOP_DECO))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bu haritada pazar sinirina ulasildi."));
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("2. koy veya baska bir kanalda kurabilirsiiniz."));
		return;
	}

#define __OFFLINE_SHOP_FIX_OPEN
#define __OFFLINE_SHOP_VALUE_MAX 2000000000
#ifdef __OFFLINE_SHOP_FIX_OPEN
	int64_t sTotalMoneyVegas = 0;

	for (int n = 0; n < bItemCount; ++n){
		sTotalMoneyVegas += static_cast<int64_t>((pTable+n)->price);
	}

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT money2 FROM player.player WHERE id = %u", GetPlayerID()));
	if (pMsg->Get()->uiNumRows == 0) {return;}

	DWORD dwCurrentMoney = 0;
	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	str_to_number(dwCurrentMoney, row[0]);

	sTotalMoneyVegas += static_cast<int64_t>(dwCurrentMoney);

    if (__OFFLINE_SHOP_VALUE_MAX <= sTotalMoneyVegas){ChatPacket(CHAT_TYPE_INFO, LC_TEXT("20�� ���� �ʰ��Ͽ� ������ ������ �����ϴ�")); return;}
#else    
	if (!LC_IsBrazil()){
		DWORD nTotalMoney = 0;
		for (BYTE n = 0; n < bItemCount; ++n)
			nTotalMoney += (pTable + n)->price;

		if (GOLD_MAX - 1 <= nTotalMoney){
			sys_err("[OVERFLOW_GOLD] Overflow (GOLD_MAX) id %u name %s", GetPlayerID(), GetName());
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("20�� ���� �ʰ��Ͽ� ������ ������ �����ϴ�"));
			return;
		}
	}
#endif   

		if (g_bEnableEmpireLimit)
	{
		if (GetEmpire() == 1 && (GetMapIndex() == 41 || GetMapIndex() == 21))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("map_shop_block"));
			return;
		}
		else if (GetEmpire() == 2 && (GetMapIndex() == 1 || GetMapIndex() == 41))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("map_shop_block"));
			return;
		}
		else if (GetEmpire() == 3 && (GetMapIndex() == 1 || GetMapIndex() == 21))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("map_shop_block"));
			return;
		}
	}
	if (IsSashCombinationOpen() || IsSashAbsorptionOpen())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Bu Pencerede Offline Shop Acilamaz"));
		return;
	}
	{
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT COUNT(*) FROM %soffline_shop_item WHERE owner_id = %u and status = 1", get_table_postfix(), GetPlayerID()));
		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);

		BYTE bResult = 0;
		str_to_number(bResult, row[0]);

		if (bResult)
		{
			ChatPacket(CHAT_TYPE_INFO, "<Offline Shop> Oncelikle Satilmayan Itemlerinizi Admin Panelinden Aliniz.");
			return;
		}
	}

	{
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT COUNT(*) FROM %soffline_shop_npc WHERE owner_id = %u", get_table_postfix(), GetPlayerID()));
		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
		
		BYTE bResult = 0;
		str_to_number(bResult, row[0]);
		
		if (bResult){ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have already offline shop. You can't open again!"));return;}
	}

	if (g_bOfflineShopMapAllowLimit){
		if (!offlineshop_map_allow_find(GetMapIndex())){ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't open offline shop in this place!"));return;}
	}
	
	if (tezgahPaketi == 1){
		if (CountSpecifyItem(50299) == 0)
		{
			LPITEM pItem = ITEM_MANAGER::instance().CreateItem(50299);
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot open an offline-shop at the moment. You need to x%d %s"), 1, pItem->GetName());
			return;		
		}
	}

	char szSign[SHOP_SIGN_MAX_LEN+1];
	strlcpy(szSign, c_pszSign, sizeof(szSign));
	
	if (strlen(szSign) == 0)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot open an offline-shop without title!"));
		return;
	}
	
	if (strstr(szSign, "%") || strstr(szSign, "'"))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot open an offline-shop without title!"));
		return;		
	}
	
	m_stOfflineShopSign = szSign;
	
	if (CBanwordManager::instance().CheckString(m_stOfflineShopSign.c_str(), m_stOfflineShopSign.length()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��Ӿ ��� ���Ե� ���� �̸����� ������ �� �� �����ϴ�."));	
		return;
	}

	LPCHARACTER npc = CHARACTER_MANAGER::instance().SpawnMob(m_npc_offline, GetMapIndex(), GetX(), GetY(), GetZ(), false, -1, false, true, GetPlayerID());

	if (!npc){ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Offline shop npc can not create! Please try again."));return;}

	int iTime = 0;
	switch (bTime){
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

#ifdef YANG_LIMIT
	LDWORD time1g = static_cast<LDWORD>(time1gGold), time2g = static_cast<LDWORD>(time2gGold), time3g = static_cast<LDWORD>(time3gGold), time4g = static_cast<LDWORD>(time4gGold);
#else
	int time1g = time1gGold, time2g = time2gGold, time3g = time3gGold, time4g = time4gGold;
#endif
	if (bTime == 1 && GetGold() < time1g){
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ct_yetersiz_yang"));
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ihtiyaciniz olan yang: %d"), time1gGold);
		return;
	}
	else if (bTime == 2 && GetGold() < time2g){
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ct_yetersiz_yang"));
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ihtiyaciniz olan yang: %d"), time2gGold);
		return;
	}
	else if (bTime == 3 && GetGold() < time3g){
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ct_yetersiz_yang"));
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ihtiyaciniz olan yang: %d"), time3gGold);
		return;
	}
	else if (bTime == 4 && GetGold() < time4g){
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ct_yetersiz_yang"));
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ihtiyaciniz olan yang: %d"), time4gGold);
		return;
	}

	std::map<DWORD, DWORD> itemkind;
	std::set<TItemPos> cont;

	for (short i = 0; i < bItemCount; ++i){
		if (cont.find((pTable + i)->pos) != cont.end()){
			sys_err("MY_OFFLINE_SHOP: duplicate shop item detected! (name: %s)", GetName());
			return;
		}

		// ANTI_GIVE, ANTI_MYSHOP check
		LPITEM pkItem = GetItem((pTable + i)->pos);

		if (pkItem){
			const TItemTable * item_table = pkItem->GetProto();
			if (item_table && (IS_SET(item_table->dwAntiFlags, ITEM_ANTIFLAG_GIVE | ITEM_ANTIFLAG_MY_OFFLINE_SHOP))){ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This item can not sell!"));return;}
			if (pkItem->IsEquipped() == true) {return;}
			if (pkItem->isLocked()){ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can not sell locked items!"));return;}
		
			int iFiyat = pkItem->GetProto()->dwGold;	
			int	iadet = pkItem->GetCount();
			int itotal = iFiyat * iadet;
			int iPrice = (pTable + i)->price;
			int iPriceCheque = (pTable + i)->price_cheque;
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
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("esya_fiyat_blocks"));
							return;
						}
					}
					if (iPriceCheque > 0)
					{
						if(total2 < itotal)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("esya_fiyat_blocks"));
							return;
						}
					}
				}
			}
		
		
			itemkind[pkItem->GetVnum()] = (pTable + i)->price / pkItem->GetCount();

			char szColumns[QUERY_MAX_LEN], szValues[QUERY_MAX_LEN];

			int iLen = snprintf(szColumns, sizeof(szColumns), "id,owner_id,pos,count,price,price_cheque,vnum,evolution");
			int iUpdateLen = snprintf(szValues, sizeof(szValues), "%u,%u,%d,%u,%u,%u,%u,%d", pkItem->GetID(), GetPlayerID(), (pTable + i)->display_pos, pkItem->GetCount(), (pTable + i)->price, (pTable + i)->price_cheque, pkItem->GetVnum(), pkItem->GetEvolution());

			iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen, ",socket0,socket1,socket2,socket3,socket4,socket5");
			iUpdateLen += snprintf(szValues + iUpdateLen, sizeof(szValues) - iUpdateLen, ",%ld,%ld,%ld,%ld,%ld,%ld", pkItem->GetSocket(0), pkItem->GetSocket(1), pkItem->GetSocket(2), pkItem->GetSocket(3), pkItem->GetSocket(4), pkItem->GetSocket(5));

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
				pkItem->GetAttributeType(13), pkItem->GetAttributeValue(13),
				pkItem->GetAttributeType(14), pkItem->GetAttributeValue(14));

			char szInsertQuery[QUERY_MAX_LEN];
			snprintf(szInsertQuery, sizeof(szInsertQuery), "INSERT INTO %soffline_shop_item (%s) VALUES (%s)", get_table_postfix(), szColumns, szValues);
			std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szInsertQuery));
			//pkItem->RemoveFromCharacter();
			ITEM_MANAGER::instance().RemoveItem(pkItem);
		}

		cont.insert((pTable + i)->pos);
	}
#ifdef YANG_LIMIT
		 if (bTime == 1){ GoldChange(-time1g); ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ct_yang_kesildi"), time1g); }
	else if (bTime == 2){ GoldChange(-time2g); ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ct_yang_kesildi"), time2g);}
	else if (bTime == 3){ GoldChange(-time3g); ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ct_yang_kesildi"), time3g);}
	else if (bTime == 4){ GoldChange(-time4g); ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ct_yang_kesildi"), time4g);}
#else
	
	if (bTime == 1){ PointChange(POINT_GOLD, -time1g, true);ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ct_yang_kesildi"), time1g);}
	else if (bTime == 2){PointChange(POINT_GOLD, -time2g, true);ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ct_yang_kesildi"), time2g);}
	else if (bTime == 3){PointChange(POINT_GOLD, -time3g, true);ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ct_yang_kesildi"), time3g);}
	else if (bTime == 4){PointChange(POINT_GOLD, -time4g, true);ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ct_yang_kesildi"), time4g);}
#endif
	if (bTime != 4){npc->SetOfflineShopTimer(iTime);npc->StartOfflineShopUpdateEvent();}

	// Insert
	char szQuery2[512];
	if (GetGuild())
		snprintf(szQuery2, sizeof(szQuery2), "INSERT INTO %soffline_shop_npc(owner_id, sign, name, time, x, y, z, mapIndex, channel, level, align, guild, npc, npc_decoration) VALUES(%u, '%s', '%s', %d, %ld, %ld, %ld, %ld, %d, %d, %d, '%s', %d, %d)", get_table_postfix(), GetPlayerID(), c_pszSign, GetName(), iTime, GetX(), GetY(), GetZ(), GetMapIndex(), g_bChannel, GetLevel(), GetAlignment(), GetGuild()->GetName(), m_npc_offline, m_npc_decoration);
	else
		snprintf(szQuery2, sizeof(szQuery2), "INSERT INTO %soffline_shop_npc(owner_id, sign, name, time, x, y, z, mapIndex, channel, level, align, guild, npc, npc_decoration) VALUES(%u, '%s', '%s', %d, %ld, %ld, %ld, %ld, %d, %d, %d, '%s', %d, %d)", get_table_postfix(), GetPlayerID(), c_pszSign, GetName(), iTime, GetX(), GetY(), GetZ(), GetMapIndex(), g_bChannel, GetLevel(), GetAlignment(), 0, m_npc_offline, m_npc_decoration);
	DBManager::Instance().DirectQuery(szQuery2);

	// Create Offline Shop
	LPOFFLINESHOP pkOfflineShop = COfflineShopManager::instance().CreateOfflineShop(npc, GetPlayerID());

	if (!pkOfflineShop){
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Offline shop npc can not create. Please try again."));
		return;
	}
	

	npc->SetOfflineShop(pkOfflineShop);
	npc->SetOfflineShopChannel(g_bChannel);
	pkOfflineShop->SetShopItems(npc, npc->GetVID());

	// Set Name
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	snprintf(szName, sizeof(szName), "%s", GetName());
	npc->SetName(szName);
	
	npc->SetLevel(this->GetLevel());
	npc->UpdateAlignment(this->GetAlignment());
	CGuild* pGuild = this->GetGuild();
	if (pGuild != NULL)
		npc->SetGuild(pGuild);

	SetOfflineShopVID(npc->GetVID());
	npc->Show(GetMapIndex(), GetX(), GetY(), GetZ(), true);

	m_stOfflineShopSign = szSign;
	TPacketGCShopSign p;
	p.bHeader = HEADER_GC_OFFLINE_SHOP_SIGN;
	p.dwVID = npc->GetVID();
	char decoration[50];
	sprintf(decoration, "%d", m_npc_decoration);
	std::string m_variable0 = decoration;
	std::string m_variable1 = m_variable0+c_pszSign;
	strlcpy(p.szSign, m_variable1.c_str(), sizeof(p.szSign));

	PacketAround(&p, sizeof(TPacketGCShopSign));

	//if (bTime != 4)
	//	ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Offline Shop> Your offline shop's time is expired after %d hour(s)"), bTime);
	//else
		//ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Offline Shop> Your offline shop's time is expired after %d hour(s)"), bTime);
	if (bTime == 1)
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pazar_4_saat"));
	else if (bTime == 2)
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pazar_6_saat"));
	else if (bTime ==3)
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pazar_12_saat"));
	else if (bTime == 4)
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pazar_24_saat"));
	else
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pazar_xx_saat"));

	npc->ViewReencode();
}

EVENTFUNC(offline_shop_update_event)
{
	char_event_info* pInfo = dynamic_cast<char_event_info*>(event->info);

	if (pInfo == NULL){sys_err("offline_shop_update_event> <Factor> Null pointer");return 0;}

	LPCHARACTER npc = pInfo->ch;

	if (npc == NULL) {return 0;}

	int remain_time = npc->GetOfflineShopTimer() - processing_time / passes_per_sec;
	if (remain_time <= 0){
		sys_log(0, "OFFLINE SHOP EXPIRED : expired %s", npc->GetName());
		LPCHARACTER pChar = CHARACTER_MANAGER::instance().Find(npc->GetOfflineShopRealOwner());			
		
		if (pChar){
			pChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Your offline shop timer is over. Selling items will give you again."));			
			COfflineShopManager::instance().Giveback(pChar);
		}

		COfflineShopManager::Instance().DestroyOfflineShop(NULL, npc->GetVID());
		DBManager::instance().DirectQuery("UPDATE %soffline_shop_item SET status = 1 WHERE owner_id = %u and status = 0", get_table_postfix(), npc->GetOfflineShopRealOwner());
		DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_npc WHERE owner_id = %u", get_table_postfix(), npc->GetOfflineShopRealOwner());
		M2_DESTROY_CHARACTER(npc);
		return 0;
	}

	// If the variable is reach to maximum, start to save it
	if (npc->GetOfflineShopSaveTime() >= g_bOfflineShopSaveTime){
		DBManager::instance().DirectQuery("UPDATE %soffline_shop_npc SET time = %d WHERE owner_id = %u", get_table_postfix(), npc->GetOfflineShopTimer(), npc->GetOfflineShopRealOwner());
		npc->SetOfflineShopTimer(1);
	}

	npc->SetOfflineShopTimer(remain_time);
	npc->SetOfflineShopSaveTime(npc->GetOfflineShopSaveTime() + 1);
	return PASSES_PER_SEC(MIN(60, remain_time));
}

void CHARACTER::SatilmayanlariGeriAl(LPCHARACTER ch)
{
	COfflineShopManager::instance().Giveback3(ch);
}

void CHARACTER::StartOfflineShopUpdateEvent()
{
	if (m_pkOfflineShopUpdateEvent) {return;}
	if (IsPC() || IsMonster()) {return;}

	char_event_info * pInfo = AllocEventInfo<char_event_info>();
	pInfo->ch = this;
	m_pkOfflineShopUpdateEvent = event_create(offline_shop_update_event, pInfo, PASSES_PER_SEC(60));
}

void CHARACTER::StopOfflineShopUpdateEvent()
{
	m_pointsInstant.bSaveTime = 0;
	m_pointsInstant.leftTime = 0;
	event_cancel(&m_pkOfflineShopUpdateEvent);
}
void CHARACTER::SetExchange(CExchange * pkExchange)
{
	m_pkExchange = pkExchange;
}

void CHARACTER::SetPart(BYTE bPartPos, WORD wVal)
{
	assert(bPartPos < PART_MAX_NUM);
	m_pointsInstant.parts[bPartPos] = wVal;
}

WORD CHARACTER::GetPart(BYTE bPartPos) const
{
	assert(bPartPos < PART_MAX_NUM);
	return m_pointsInstant.parts[bPartPos];
}

WORD CHARACTER::GetOriginalPart(BYTE bPartPos) const
{
	switch (bPartPos)
	{
		case PART_MAIN:
			if (!IsPC()) // PC�� �ƴ� ��� ���� ��Ʈ�� �״�� ����
				return GetPart(PART_MAIN);
			else
				return m_pointsInstant.bBasePart;

		case PART_HAIR:
			return GetPart(PART_HAIR);

		#ifdef __SASH_SYSTEM__
		case PART_SASH:
			return GetPart(PART_SASH);
		#endif	
#ifdef ENABLE_AURA_SYSTEM
		case PART_AURA:
			return GetPart(PART_AURA);
#endif
#ifdef __WEAPON_COSTUME_SYSTEM__
		case PART_WEAPON:
			return GetPart(PART_WEAPON);
#endif			
			
		default:
			return 0;
	}
}

BYTE CHARACTER::GetCharType() const
{
	return m_bCharType;
}

bool CHARACTER::SetSyncOwner(LPCHARACTER ch, bool bRemoveFromList)
{
	// TRENT_MONSTER
	if (IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_NOMOVE))
		return false;
	// END_OF_TRENT_MONSTER

	if (ch == this)
	{
		sys_err("SetSyncOwner owner == this (%p)", this);
		return false;
	}

	if (!ch)
	{
		if (bRemoveFromList && m_pkChrSyncOwner)
		{
			m_pkChrSyncOwner->m_kLst_pkChrSyncOwned.remove(this);
		}

		if (m_pkChrSyncOwner)
			sys_log(1, "SyncRelease %s %p from %s", GetName(), this, m_pkChrSyncOwner->GetName());

		// ����Ʈ���� �������� �ʴ��� �����ʹ� NULL�� ���õǾ�� �Ѵ�.
		m_pkChrSyncOwner = NULL;
	}
	else
	{
		if (!IsSyncOwner(ch))
			return false;

		// �Ÿ��� 200 �̻��̸� SyncOwner�� �� �� ����.
		if (DISTANCE_APPROX(GetX() - ch->GetX(), GetY() - ch->GetY()) > 250)
		{
			sys_log(1, "SetSyncOwner distance over than 250 %s %s", GetName(), ch->GetName());

			// SyncOwner�� ��� Owner�� ǥ���Ѵ�.
			if (m_pkChrSyncOwner == ch)
				return true;

			return false;
		}

		if (m_pkChrSyncOwner != ch)
		{
			if (m_pkChrSyncOwner)
			{
				sys_log(1, "SyncRelease %s %p from %s", GetName(), this, m_pkChrSyncOwner->GetName());
				m_pkChrSyncOwner->m_kLst_pkChrSyncOwned.remove(this);
			}

			m_pkChrSyncOwner = ch;
			m_pkChrSyncOwner->m_kLst_pkChrSyncOwned.push_back(this);

			// SyncOwner�� �ٲ�� LastSyncTime�� �ʱ�ȭ�Ѵ�.
			static const timeval zero_tv = {0, 0};
			SetLastSyncTime(zero_tv);

			sys_log(1, "SetSyncOwner set %s %p to %s", GetName(), this, ch->GetName());
		}

		m_fSyncTime = get_float_time();
	}

	// TODO: Sync Owner�� ������ ��� ��Ŷ�� ������ �����Ƿ�,
	//       ����ȭ �� �ð��� 3�� �̻� ������ �� Ǯ���ִ� ��Ŷ��
	//       ������ ������� �ϸ� ��Ŷ�� ���� �� �ִ�.
	TPacketGCOwnership pack;

	pack.bHeader	= HEADER_GC_OWNERSHIP;
	pack.dwOwnerVID	= ch ? ch->GetVID() : 0;
	pack.dwVictimVID	= GetVID();

	PacketAround(&pack, sizeof(TPacketGCOwnership));
	return true;
}

struct FuncClearSync
{
	void operator () (LPCHARACTER ch)
	{
		assert(ch != NULL);
		ch->SetSyncOwner(NULL, false);	// false �÷��׷� �ؾ� for_each �� ����� ����.
	}
};

void CHARACTER::ClearSync()
{
	SetSyncOwner(NULL);

	// �Ʒ� for_each���� ���� m_pkChrSyncOwner�� ���� �ڵ��� �����͸� NULL�� �Ѵ�.
	std::for_each(m_kLst_pkChrSyncOwned.begin(), m_kLst_pkChrSyncOwned.end(), FuncClearSync());
	m_kLst_pkChrSyncOwned.clear();
}

bool CHARACTER::IsSyncOwner(LPCHARACTER ch) const
{
	if (m_pkChrSyncOwner == ch)
		return true;

	// ���������� ����ȭ �� �ð��� 3�� �̻� �����ٸ� �������� �ƹ����Ե�
	// ����. ���� �ƹ��� SyncOwner�̹Ƿ� true ����
	if (get_float_time() - m_fSyncTime >= 3.0f)
		return true;

	return false;
}

void CHARACTER::SetParty(LPPARTY pkParty)
{
	if (pkParty == m_pkParty)
		return;

	if (pkParty && m_pkParty)
		sys_err("%s is trying to reassigning party (current %p, new party %p)", GetName(), get_pointer(m_pkParty), get_pointer(pkParty));

	sys_log(1, "PARTY set to %p", get_pointer(pkParty));

	if (m_pkDungeon && IsPC() && !pkParty)
		SetDungeon(NULL);

	m_pkParty = pkParty;

	if (IsPC())
	{
		if (m_pkParty)
			SET_BIT(m_bAddChrState, ADD_CHARACTER_STATE_PARTY);
		else
			REMOVE_BIT(m_bAddChrState, ADD_CHARACTER_STATE_PARTY);

		UpdatePacket();
	}
}

// PARTY_JOIN_BUG_FIX
/// ��Ƽ ���� event ����
EVENTINFO(TPartyJoinEventInfo)
{
	DWORD	dwGuestPID;		///< ��Ƽ�� ������ ĳ������ PID
	DWORD	dwLeaderPID;		///< ��Ƽ ������ PID

	TPartyJoinEventInfo() 
	: dwGuestPID( 0 )
	, dwLeaderPID( 0 )
	{
	}
} ;

EVENTFUNC(party_request_event)
{
	TPartyJoinEventInfo * info = dynamic_cast<TPartyJoinEventInfo *>(  event->info );

	if ( info == NULL )
	{
		sys_err( "party_request_event> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(info->dwGuestPID);

	if (ch)
	{
		sys_log(0, "PartyRequestEvent %s", ch->GetName());
		ch->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequestDenied");
		ch->SetPartyRequestEvent(NULL);
	}

	return 0;
}

bool CHARACTER::RequestToParty(LPCHARACTER leader)
{
	if (leader->GetParty())
		leader = leader->GetParty()->GetLeaderCharacter();

	if (!leader)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��Ƽ���� ���� ���°� �ƴ϶� ��û�� �� �� �����ϴ�."));
		return false;
	}

	if (m_pkPartyRequestEvent)
		return false; 

	if (!IsPC() || !leader->IsPC())
		return false;

	if (leader->IsBlockMode(BLOCK_PARTY_REQUEST))
		return false;

	PartyJoinErrCode errcode = IsPartyJoinableCondition(leader, this);

	switch (errcode)
	{
		case PERR_NONE:
			break;

		case PERR_SERVER:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ���� ������ ��Ƽ ���� ó���� �� �� �����ϴ�."));
			return false;

		case PERR_DIFFEMPIRE:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> �ٸ� ������ ��Ƽ�� �̷� �� �����ϴ�."));
			return false;

		case PERR_DUNGEON:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ���� �ȿ����� ��Ƽ �ʴ븦 �� �� �����ϴ�.")); 
			return false;

		case PERR_OBSERVER:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ���� ��忡�� ��Ƽ �ʴ븦 �� �� �����ϴ�.")); 
			return false;

		case PERR_LVBOUNDARY:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> -30 ~ +30 ���� �̳��� ���游 �ʴ��� �� �ֽ��ϴ�.")); 
			return false;

		case PERR_LOWLEVEL:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ��Ƽ�� �ְ� ���� ���� 30������ ���� �ʴ��� �� �����ϴ�."));
			return false;

		case PERR_HILEVEL:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ��Ƽ�� ���� ���� ���� 30������ ���� �ʴ��� �� �����ϴ�.")); 
			return false;

		case PERR_ALREADYJOIN: 	
			return false;

		case PERR_PARTYISFULL:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> �� �̻� ��Ƽ���� �ʴ��� �� �����ϴ�.")); 
			return false;

		default:
			sys_err("Do not process party join error(%d)", errcode); 
			return false;
	}

	TPartyJoinEventInfo* info = AllocEventInfo<TPartyJoinEventInfo>();

	info->dwGuestPID = GetPlayerID();
	info->dwLeaderPID = leader->GetPlayerID();

	SetPartyRequestEvent(event_create(party_request_event, info, PASSES_PER_SEC(10)));

	leader->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequest %u", (DWORD) GetVID());
	ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s �Կ��� ��Ƽ���� ��û�� �߽��ϴ�."), leader->GetName());
	return true;
}

void CHARACTER::DenyToParty(LPCHARACTER member)
{
	sys_log(1, "DenyToParty %s member %s %p", GetName(), member->GetName(), get_pointer(member->m_pkPartyRequestEvent));

	if (!member->m_pkPartyRequestEvent)
		return;

	TPartyJoinEventInfo * info = dynamic_cast<TPartyJoinEventInfo *>(member->m_pkPartyRequestEvent->info);

	if (!info)
	{
		sys_err( "CHARACTER::DenyToParty> <Factor> Null pointer" );
		return;
	}

	if (info->dwGuestPID != member->GetPlayerID())
		return;

	if (info->dwLeaderPID != GetPlayerID())
		return;

	event_cancel(&member->m_pkPartyRequestEvent);

	member->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequestDenied");
}

void CHARACTER::AcceptToParty(LPCHARACTER member)
{
	sys_log(1, "AcceptToParty %s member %s %p", GetName(), member->GetName(), get_pointer(member->m_pkPartyRequestEvent));

	if (!member->m_pkPartyRequestEvent)
		return;

	TPartyJoinEventInfo * info = dynamic_cast<TPartyJoinEventInfo *>(member->m_pkPartyRequestEvent->info);

	if (!info)
	{
		sys_err( "CHARACTER::AcceptToParty> <Factor> Null pointer" );
		return;
	}

	if (info->dwGuestPID != member->GetPlayerID())
		return;

	if (info->dwLeaderPID != GetPlayerID())
		return;

	event_cancel(&member->m_pkPartyRequestEvent);

	if (!GetParty())
		member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("?????? ????? ???????? ??????."));
	else 
	{
		if (GetPlayerID() != GetParty()->GetLeaderPID())
			return;

		PartyJoinErrCode errcode = IsPartyJoinableCondition(this, member);
		switch (errcode) 
		{
			case PERR_NONE: 		member->PartyJoin(this); return;
			case PERR_SERVER:		member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ���� ������ ��Ƽ ���� ó���� �� �� �����ϴ�.")); break;
			case PERR_DUNGEON:		member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ���� �ȿ����� ��Ƽ �ʴ븦 �� �� �����ϴ�.")); break;
			case PERR_OBSERVER: 	member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ���� ��忡�� ��Ƽ �ʴ븦 �� �� �����ϴ�.")); break;
			case PERR_LVBOUNDARY:	member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> -30 ~ +30 ���� �̳��� ���游 �ʴ��� �� �ֽ��ϴ�.")); break;
			case PERR_LOWLEVEL: 	member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ��Ƽ�� �ְ� ���� ���� 30������ ���� �ʴ��� �� �����ϴ�.")); break;
			case PERR_HILEVEL: 		member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ��Ƽ�� ���� ���� ���� 30������ ���� �ʴ��� �� �����ϴ�.")); break;
			case PERR_ALREADYJOIN: 	break;
			case PERR_PARTYISFULL: {
									   ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> �� �̻� ��Ƽ���� �ʴ��� �� �����ϴ�."));
									   member->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ��Ƽ�� �ο������� �ʰ��Ͽ� ��Ƽ�� ������ �� �����ϴ�."));
									   break;
								   }
			default: sys_err("Do not process party join error(%d)", errcode);
		}
	}

	member->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequestDenied");
}

/**
 * ��Ƽ �ʴ� event callback �Լ�.
 * event �� �ߵ��ϸ� �ʴ� ������ ó���Ѵ�.
 */
EVENTFUNC(party_invite_event)
{
	TPartyJoinEventInfo * pInfo = dynamic_cast<TPartyJoinEventInfo *>(  event->info );

	if ( pInfo == NULL )
	{
		sys_err( "party_invite_event> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER pchInviter = CHARACTER_MANAGER::instance().FindByPID(pInfo->dwLeaderPID);

	if (pchInviter)
	{
		sys_log(1, "PartyInviteEvent %s", pchInviter->GetName());
		pchInviter->PartyInviteDeny(pInfo->dwGuestPID);
	}

	return 0;
}

void CHARACTER::PartyInvite(LPCHARACTER pchInvitee)
{
	if (GetParty() && GetParty()->GetLeaderPID() != GetPlayerID())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ��Ƽ���� �ʴ��� �� �ִ� ������ �����ϴ�."));
		return;
	}
	else if (pchInvitee->IsBlockMode(BLOCK_PARTY_INVITE))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> %s ���� ��Ƽ �ź� �����Դϴ�."), pchInvitee->GetName());
		return;
	}

	PartyJoinErrCode errcode = IsPartyJoinableCondition(this, pchInvitee);

	switch (errcode)
	{
		case PERR_NONE:
			break;

		case PERR_SERVER:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ���� ������ ��Ƽ ���� ó���� �� �� �����ϴ�."));
			return;

		case PERR_DIFFEMPIRE:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> �ٸ� ������ ��Ƽ�� �̷� �� �����ϴ�."));
			return;

		case PERR_DUNGEON:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ���� �ȿ����� ��Ƽ �ʴ븦 �� �� �����ϴ�."));
			return;

		case PERR_OBSERVER:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ���� ��忡�� ��Ƽ �ʴ븦 �� �� �����ϴ�."));
			return;

		case PERR_LVBOUNDARY:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> -30 ~ +30 ���� �̳��� ���游 �ʴ��� �� �ֽ��ϴ�."));
			return;

		case PERR_LOWLEVEL:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ��Ƽ�� �ְ� ���� ���� 30������ ���� �ʴ��� �� �����ϴ�."));
			return;

		case PERR_HILEVEL:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ��Ƽ�� ���� ���� ���� 30������ ���� �ʴ��� �� �����ϴ�."));
			return;

		case PERR_ALREADYJOIN:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> �̹� %s���� ��Ƽ�� ���� �ֽ��ϴ�."), pchInvitee->GetName());
			return;

		case PERR_PARTYISFULL:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> �� �̻� ��Ƽ���� �ʴ��� �� �����ϴ�."));
			return;

		default:
			sys_err("Do not process party join error(%d)", errcode);
			return;
	}

	if (m_PartyInviteEventMap.end() != m_PartyInviteEventMap.find(pchInvitee->GetPlayerID()))
		return;

	//
	// EventMap �� �̺�Ʈ �߰�
	// 
	TPartyJoinEventInfo* info = AllocEventInfo<TPartyJoinEventInfo>();

	info->dwGuestPID = pchInvitee->GetPlayerID();
	info->dwLeaderPID = GetPlayerID();

	m_PartyInviteEventMap.insert(EventMap::value_type(pchInvitee->GetPlayerID(), event_create(party_invite_event, info, PASSES_PER_SEC(10))));

	//
	// �ʴ� �޴� character ���� �ʴ� ��Ŷ ����
	// 

	TPacketGCPartyInvite p;
	p.header = HEADER_GC_PARTY_INVITE;
	p.leader_vid = GetVID();
	pchInvitee->GetDesc()->Packet(&p, sizeof(p));
}

void CHARACTER::PartyInviteAccept(LPCHARACTER pchInvitee)
{
	EventMap::iterator itFind = m_PartyInviteEventMap.find(pchInvitee->GetPlayerID());

	if (itFind == m_PartyInviteEventMap.end())
	{
		sys_log(1, "PartyInviteAccept from not invited character(%s)", pchInvitee->GetName());
		return;
	}

	event_cancel(&itFind->second);
	m_PartyInviteEventMap.erase(itFind);

	if (GetParty() && GetParty()->GetLeaderPID() != GetPlayerID())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ��Ƽ���� �ʴ��� �� �ִ� ������ �����ϴ�."));
		return;
	}

	PartyJoinErrCode errcode = IsPartyJoinableMutableCondition(this, pchInvitee);

	switch (errcode)
	{
		case PERR_NONE:
			break;

		case PERR_SERVER:
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ���� ������ ��Ƽ ���� ó���� �� �� �����ϴ�."));
			return;

		case PERR_DUNGEON:
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ���� �ȿ����� ��Ƽ �ʴ뿡 ���� �� �����ϴ�."));
			return;

		case PERR_OBSERVER:
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ���� ��忡�� ��Ƽ �ʴ븦 �� �� �����ϴ�."));
			return;

		case PERR_LVBOUNDARY:
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> -30 ~ +30 ���� �̳��� ���游 �ʴ��� �� �ֽ��ϴ�."));
			return;

		case PERR_LOWLEVEL:
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ��Ƽ�� �ְ� ���� ���� 30������ ���� �ʴ��� �� �����ϴ�."));
			return;

		case PERR_HILEVEL:
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ��Ƽ�� ���� ���� ���� 30������ ���� �ʴ��� �� �����ϴ�."));
			return;

		case PERR_ALREADYJOIN:
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ��Ƽ �ʴ뿡 ���� �� �����ϴ�."));
			return;

		case PERR_PARTYISFULL:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> �� �̻� ��Ƽ���� �ʴ��� �� �����ϴ�."));
			pchInvitee->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> ��Ƽ�� �ο������� �ʰ��Ͽ� ��Ƽ�� ������ �� �����ϴ�."));
			return;

		default:
			sys_err("ignore party join error(%d)", errcode);
			return;
	}

	//
	// ��Ƽ ���� ó��
	// 

	if (GetParty())
		pchInvitee->PartyJoin(this);
	else
	{
		LPPARTY pParty = CPartyManager::instance().CreateParty(this);

		pParty->Join(pchInvitee->GetPlayerID());
		pParty->Link(pchInvitee);
		pParty->SendPartyInfoAllToOne(this);
	}
}

void CHARACTER::PartyInviteDeny(DWORD dwPID)
{
	EventMap::iterator itFind = m_PartyInviteEventMap.find(dwPID);

	if (itFind == m_PartyInviteEventMap.end())
	{
		sys_log(1, "PartyInviteDeny to not exist event(inviter PID: %d, invitee PID: %d)", GetPlayerID(), dwPID);
		return;
	}

	event_cancel(&itFind->second);
	m_PartyInviteEventMap.erase(itFind);

	LPCHARACTER pchInvitee = CHARACTER_MANAGER::instance().FindByPID(dwPID);
	if (pchInvitee)
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> %s���� ��Ƽ �ʴ븦 �����ϼ̽��ϴ�."), pchInvitee->GetName());
}

void CHARACTER::PartyJoin(LPCHARACTER pLeader)
{
	pLeader->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> %s���� ��Ƽ�� �����ϼ̽��ϴ�."), GetName());
	ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<��Ƽ> %s���� ��Ƽ�� �����ϼ̽��ϴ�."), pLeader->GetName());

	pLeader->GetParty()->Join(GetPlayerID());
	pLeader->GetParty()->Link(this);
}

CHARACTER::PartyJoinErrCode CHARACTER::IsPartyJoinableCondition(const LPCHARACTER pchLeader, const LPCHARACTER pchGuest)
{
	if (pchLeader->GetEmpire() != pchGuest->GetEmpire())
		return PERR_DIFFEMPIRE;

	return IsPartyJoinableMutableCondition(pchLeader, pchGuest);
}

static bool __party_can_join_by_level(LPCHARACTER leader, LPCHARACTER quest)
{
	int	level_limit = 30;

	if (LC_IsCanada())
		level_limit = 15;
	else if (LC_IsBrazil() == true)
	{
		level_limit = 10;
	}
	else
		level_limit = 30;

	return (abs(leader->GetLevel() - quest->GetLevel()) <= level_limit);
}

CHARACTER::PartyJoinErrCode CHARACTER::IsPartyJoinableMutableCondition(const LPCHARACTER pchLeader, const LPCHARACTER pchGuest)
{
	if (!CPartyManager::instance().IsEnablePCParty())
		return PERR_SERVER;
	else if (pchLeader->GetDungeon())
		return PERR_DUNGEON;
	else if (pchGuest->IsObserverMode())
		return PERR_OBSERVER;
	else if (false == __party_can_join_by_level(pchLeader, pchGuest))
		return PERR_LVBOUNDARY;
	else if (pchGuest->GetParty())
		return PERR_ALREADYJOIN;
	else if (pchLeader->GetParty())
   	{
	   	if (pchLeader->GetParty()->GetMemberCount() == PARTY_MAX_MEMBER)
			return PERR_PARTYISFULL;
	}

	return PERR_NONE;
}
// END_OF_PARTY_JOIN_BUG_FIX

void CHARACTER::SetDungeon(LPDUNGEON pkDungeon)
{
	if (pkDungeon && m_pkDungeon)
		sys_err("%s is trying to reassigning dungeon (current %p, new party %p)", GetName(), get_pointer(m_pkDungeon), get_pointer(pkDungeon));

	if (m_pkDungeon == pkDungeon) {
		return;
	}

	if (m_pkDungeon)
	{
		if (IsPC())
		{
			if (GetParty())
				m_pkDungeon->DecPartyMember(GetParty(), this);
			else
				m_pkDungeon->DecMember(this);
		}
		else if (IsMonster() || IsStone())
		{
			m_pkDungeon->DecMonster();
		}
	}

	m_pkDungeon = pkDungeon;

	if (pkDungeon)
	{
		sys_log(0, "%s DUNGEON set to %p, PARTY is %p", GetName(), get_pointer(pkDungeon), get_pointer(m_pkParty));

		if (IsPC())
		{
			if (GetParty())
				m_pkDungeon->IncPartyMember(GetParty(), this);
			else
				m_pkDungeon->IncMember(this);
		}
		else if (IsMonster() || IsStone())
		{
			m_pkDungeon->IncMonster();
		}
	}
}

void CHARACTER::SetWarMap(CWarMap * pWarMap)
{
	if (m_pWarMap)
		m_pWarMap->DecMember(this);

	m_pWarMap = pWarMap;

	if (m_pWarMap)
		m_pWarMap->IncMember(this);
}

void CHARACTER::SetWeddingMap(marriage::WeddingMap* pMap)
{
	if (m_pWeddingMap)
		m_pWeddingMap->DecMember(this);

	m_pWeddingMap = pMap;

	if (m_pWeddingMap)
		m_pWeddingMap->IncMember(this);
}

void CHARACTER::SetRegen(LPREGEN pkRegen)
{
	m_pkRegen = pkRegen;
	if (pkRegen != NULL) {
		regen_id_ = pkRegen->id;
	}
	m_fRegenAngle = GetRotation();
	m_posRegen = GetXYZ();
}

bool CHARACTER::OnIdle()
{
	return false;
}

void CHARACTER::OnMove(bool bIsAttack)
{
	m_dwLastMoveTime = get_dword_time();

	if (bIsAttack)
	{
		m_dwLastAttackTime = m_dwLastMoveTime;

		if (IsAffectFlag(AFF_REVIVE_INVISIBLE))
			RemoveAffect(AFFECT_REVIVE_INVISIBLE);

		if (IsAffectFlag(AFF_EUNHYUNG))
		{
			RemoveAffect(SKILL_EUNHYUNG);
			SetAffectedEunhyung();
		}
		else
		{
			ClearAffectedEunhyung();
		}

		/*if (IsAffectFlag(AFF_JEONSIN))
		  RemoveAffect(SKILL_JEONSINBANGEO);*/
	}

	/*if (IsAffectFlag(AFF_GUNGON))
	  RemoveAffect(SKILL_GUNGON);*/

	// MINING
	mining_cancel();
	// END_OF_MINING
}

void CHARACTER::OnClick(LPCHARACTER pkChrCauser)
{
	if (!pkChrCauser)
	{
		sys_err("OnClick %s by NULL", GetName());
		return;
	}

	DWORD vid = GetVID();
	sys_log(0, "OnClick %s[vnum %d ServerUniqueID %d, pid %d] by %s", GetName(), GetRaceNum(), vid, GetPlayerID(), pkChrCauser->GetName());

	// ������ �����·� ����Ʈ�� ������ �� ����.
	{
		// ��, �ڽ��� �ڽ��� ������ Ŭ���� �� �ִ�.
		if (pkChrCauser->GetMyShop() && pkChrCauser != this) 
		{
			sys_err("OnClick Fail (%s->%s) - pc has shop", pkChrCauser->GetName(), GetName());
			return;
		}
	}

	// ��ȯ���϶� ����Ʈ�� ������ �� ����.
	{
		if (pkChrCauser->GetExchange())
		{
			sys_err("OnClick Fail (%s->%s) - pc is exchanging", pkChrCauser->GetName(), GetName());
			return;
		}
	}

	if (IsPC())
	{
		// Ÿ������ ������ ���� PC�� ���� Ŭ���� ����Ʈ�� ó���ϵ��� �մϴ�.
		if (!CTargetManager::instance().GetTargetInfo(pkChrCauser->GetPlayerID(), TARGET_TYPE_VID, GetVID()))
		{
			// 2005.03.17.myevan.Ÿ���� �ƴ� ���� ���� ���� ó�� ����� �۵���Ų��.
			if (GetMyShop())
			{
				if (pkChrCauser->IsDead() == true) return;

				//PREVENT_TRADE_WINDOW
				if (pkChrCauser == this) // �ڱ�� ����
				{
					if ((GetExchange() || IsOpenSafebox() || GetShopOwner()) || IsCubeOpen() || GetOfflineShopOwner())
					{
						pkChrCauser->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�ٸ� �ŷ���(â��,��ȯ,����)���� ���λ����� ����� �� �����ϴ�."));
						return;
					}
				}
				else // �ٸ� ����� Ŭ��������
				{
					// Ŭ���� ����� ��ȯ/â��/���λ���/�����̿����̶�� �Ұ�
					if ((pkChrCauser->GetExchange() || pkChrCauser->IsOpenSafebox() || pkChrCauser->GetMyShop() || pkChrCauser->GetShopOwner()) || pkChrCauser->IsCubeOpen() || pkChrCauser->GetOfflineShopOwner())
					{
						pkChrCauser->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�ٸ� �ŷ���(â��,��ȯ,����)���� ���λ����� ����� �� �����ϴ�."));
						return;
					}

					// Ŭ���� ����� ��ȯ/â��/�����̿����̶�� �Ұ�
					//if ((GetExchange() || IsOpenSafebox() || GetShopOwner()))
					if ((GetExchange() || IsOpenSafebox() || IsCubeOpen() || GetOfflineShopOwner()))
					{
						pkChrCauser->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �ٸ� �ŷ��� �ϰ� �ִ� ���Դϴ�."));
						return;
					}
				}
				//END_PREVENT_TRADE_WINDOW

				if (pkChrCauser->GetShop())
				{
					pkChrCauser->GetShop()->RemoveGuest(pkChrCauser);
					pkChrCauser->SetShop(NULL);
				}

				GetMyShop()->AddGuest(pkChrCauser, GetVID(), false);
				pkChrCauser->SetShopOwner(this);
				return;
			}

			if (test_server)
				sys_err("%s.OnClickFailure(%s) - target is PC", pkChrCauser->GetName(), GetName());

			return;
		}
	}

	// Offline Shop
	if (IsOfflineShopNPC() && GetOfflineShop())
	{
		if ((GetExchange() || IsOpenSafebox() || GetShopOwner()) || IsCubeOpen() || GetOfflineShopOwner())
		{
			pkChrCauser->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�ٸ� �ŷ���(â��,��ȯ,����)���� ���λ����� ����� �� �����ϴ�."));
			return;
		}
		pkChrCauser->SetOfflineShopOwner(this);
		GetOfflineShop()->AddGuest(pkChrCauser, this);
		CTargetManager::Instance().DeleteTarget(pkChrCauser->GetPlayerID(), GetVID(), "SHOP_SEARCH_TARGET");
		return;
	}
	// û�ҳ��� ����Ʈ ����
	if (LC_IsNewCIBN())
	{
		if (pkChrCauser->IsOverTime(OT_3HOUR))
		{
			sys_log(0, "Teen OverTime : name = %s, hour = %d)", pkChrCauser->GetName(), 3);
			return;
		}
		else if (pkChrCauser->IsOverTime(OT_5HOUR))
		{
			sys_log(0, "Teen OverTime : name = %s, hour = %d)", pkChrCauser->GetName(), 5);
			return;
		}
	}


	pkChrCauser->SetQuestNPCID(GetVID());

	if (quest::CQuestManager::instance().Click(pkChrCauser->GetPlayerID(), this))
	{
		return;
	}


	// NPC ���� ��� ���� : ���� ���� ��
	if (!IsPC())
	{
		if (!m_triggerOnClick.pFunc)
		{
			// NPC Ʈ���� �ý��� �α� ����
			//sys_err("%s.OnClickFailure(%s) : triggerOnClick.pFunc is EMPTY(pid=%d)", 
			//			pkChrCauser->GetName(),
			//			GetName(),
			//			pkChrCauser->GetPlayerID());
			return;
		}

		m_triggerOnClick.pFunc(this, pkChrCauser);
	}

}

BYTE CHARACTER::GetGMLevel() const
{
	if (test_server)
		return GM_IMPLEMENTOR;
	return m_pointsInstant.gm_level;
}

void CHARACTER::SetGMLevel()
{
	if (GetDesc())
	{
	    m_pointsInstant.gm_level =  gm_get_level(GetName(), GetDesc()->GetHostName(), GetDesc()->GetAccountTable().login);
	}
	else
	{
	    m_pointsInstant.gm_level = GM_PLAYER;
	}
}

BOOL CHARACTER::IsGM() const
{
	if (m_pointsInstant.gm_level != GM_PLAYER)
		return true;
	if (test_server)
		return true;
	return false;
}

void CHARACTER::SetStone(LPCHARACTER pkChrStone)
{
	m_pkChrStone = pkChrStone;

	if (m_pkChrStone)
	{
		if (pkChrStone->m_set_pkChrSpawnedBy.find(this) == pkChrStone->m_set_pkChrSpawnedBy.end())
			pkChrStone->m_set_pkChrSpawnedBy.insert(this);
	}
}

struct FuncDeadSpawnedByStone
{
	void operator () (LPCHARACTER ch)
	{
		ch->Dead(NULL);
		ch->SetStone(NULL);
	}
};

void CHARACTER::ClearStone()
{
	if (!m_set_pkChrSpawnedBy.empty())
	{
		// ���� ������Ų ���͵��� ��� ���δ�.
		FuncDeadSpawnedByStone f;
		std::for_each(m_set_pkChrSpawnedBy.begin(), m_set_pkChrSpawnedBy.end(), f);
		m_set_pkChrSpawnedBy.clear();
	}

	if (!m_pkChrStone)
		return;

	m_pkChrStone->m_set_pkChrSpawnedBy.erase(this);
	m_pkChrStone = NULL;
}

void CHARACTER::ClearTarget()
{
	if (m_pkChrTarget)
	{
		m_pkChrTarget->m_set_pkChrTargetedBy.erase(this);
		m_pkChrTarget = NULL;
	}

	TPacketGCTarget p;
	p.header = HEADER_GC_TARGET;
	p.dwVID = 0;
	p.bHPPercent = 0;
#ifdef __VIEW_TARGET_DECIMAL_HP__
	p.iMinHP = 0;
	p.iMaxHP = 0;
#endif

	CHARACTER_SET::iterator it = m_set_pkChrTargetedBy.begin();
	while (it != m_set_pkChrTargetedBy.end())
	{
		LPCHARACTER pkChr = *(it++);
		pkChr->m_pkChrTarget = NULL;

		if (!pkChr->GetDesc())
		{
			sys_err("%s %p does not have desc", pkChr->GetName(), get_pointer(pkChr));
			abort();
		}

		pkChr->GetDesc()->Packet(&p, sizeof(TPacketGCTarget));
	}

	m_set_pkChrTargetedBy.clear();
}

void CHARACTER::SetTarget(LPCHARACTER pkChrTarget)
{
	if (m_pkChrTarget == pkChrTarget)
		return;

	if (IS_CASTLE_MAP(GetMapIndex()) && !IsGM())
		return;

	if (m_pkChrTarget)
		m_pkChrTarget->m_set_pkChrTargetedBy.erase(this);

	m_pkChrTarget = pkChrTarget;

	TPacketGCTarget p;
	p.header = HEADER_GC_TARGET;
	if (m_pkChrTarget)
	{
		m_pkChrTarget->m_set_pkChrTargetedBy.insert(this);
		p.dwVID = m_pkChrTarget->GetVID();
#ifdef __VIEW_TARGET_PLAYER_HP__
		if ((m_pkChrTarget->GetMaxHP() <= 0))
		{
			p.bHPPercent = 0;
#ifdef __VIEW_TARGET_DECIMAL_HP__
			p.iMinHP = 0;
			p.iMaxHP = 0;
#endif
		}
		else if (m_pkChrTarget->IsPC() && !m_pkChrTarget->IsPolymorphed())
		{
			p.bHPPercent = MINMAX(0, (float(m_pkChrTarget->GetHP()) / float(m_pkChrTarget->GetMaxHP()) * 100.0f), 100);
#ifdef __VIEW_TARGET_DECIMAL_HP__
			p.iMinHP = m_pkChrTarget->GetHP();
			p.iMaxHP = m_pkChrTarget->GetMaxHP();
#endif
#else
		if ((m_pkChrTarget->IsPC() && !m_pkChrTarget->IsPolymorphed()) || (m_pkChrTarget->GetMaxHP() <= 0))
		{
			p.bHPPercent = 0;
#ifdef __VIEW_TARGET_DECIMAL_HP__
			p.iMinHP = 0;
			p.iMaxHP = 0;
#endif
#endif
		}
		else
		{
			if (m_pkChrTarget->GetRaceNum() == 20101 || m_pkChrTarget->GetRaceNum() == 20102 || m_pkChrTarget->GetRaceNum() == 20103 || m_pkChrTarget->GetRaceNum() == 20104 || m_pkChrTarget->GetRaceNum() == 20105 || m_pkChrTarget->GetRaceNum() == 20106 || m_pkChrTarget->GetRaceNum() == 20107 || m_pkChrTarget->GetRaceNum() == 20108 || m_pkChrTarget->GetRaceNum() == 20109)
			{
				LPCHARACTER owner = m_pkChrTarget->GetVictim();
				if (owner)
				{
					int iHorseHealth = owner->GetHorseHealth();
					int iHorseMaxHealth = owner->GetHorseMaxHealth();
					if (iHorseMaxHealth)
					{
						p.bHPPercent = MINMAX(0, (float(iHorseHealth) / float(iHorseMaxHealth) * 100.0f), 100);
#ifdef __VIEW_TARGET_DECIMAL_HP__
						p.iMinHP = 100;
						p.iMaxHP = 100;
#endif
					}
					else
					{
						p.bHPPercent = 100;
#ifdef __VIEW_TARGET_DECIMAL_HP__
						p.iMinHP = 100;
						p.iMaxHP = 100;
#endif
					}
				}
				else
				{
					p.bHPPercent = 100;
#ifdef __VIEW_TARGET_DECIMAL_HP__
					p.iMinHP = 100;
					p.iMaxHP = 100;
#endif
				}
			}
			else
			{
				if (m_pkChrTarget->GetMaxHP() <= 0)
				{
					p.bHPPercent = 0;
#ifdef __VIEW_TARGET_DECIMAL_HP__
					p.iMinHP = 0;
					p.iMaxHP = 0;
#endif
				}
				else
				{
					p.bHPPercent = MINMAX(0, (float(m_pkChrTarget->GetHP()) / float(m_pkChrTarget->GetMaxHP()) * 100.0f), 100);
#ifdef __VIEW_TARGET_DECIMAL_HP__
					p.iMinHP = m_pkChrTarget->GetHP();
					p.iMaxHP = m_pkChrTarget->GetMaxHP();
#endif
				}
			}
		}
	}
	else
	{
		p.dwVID = 0;
		p.bHPPercent = 0;
#ifdef __VIEW_TARGET_DECIMAL_HP__
		p.iMinHP = 0;
		p.iMaxHP = 0;
#endif
	}

#ifdef ELEMENT_TARGET
	const int ELEMENT_BASE = 11;
	DWORD curElementBase = ELEMENT_BASE;
	DWORD raceFlag;
	if (m_pkChrTarget && m_pkChrTarget->IsMonster() && (raceFlag = m_pkChrTarget->GetMobTable().dwRaceFlag) >= RACE_FLAG_ATT_ELEC)
	{
		for (int i = RACE_FLAG_ATT_ELEC; i <= RACE_FLAG_ATT_DARK; i *= 2)
		{
			curElementBase++;
			int diff = raceFlag - i;
			if (abs(diff) <= 1024)
				break;
		}

		p.bElement = curElementBase - ELEMENT_BASE;
	}
	else
		p.bElement = 0;
#endif

	GetDesc()->Packet(&p, sizeof(TPacketGCTarget));
}

void CHARACTER::BroadcastTargetPacket()
{
	if (m_set_pkChrTargetedBy.empty())
		return;

	TPacketGCTarget p;
	p.header = HEADER_GC_TARGET;
	p.dwVID = GetVID();
	if (GetMaxHP() <= 0)
	{
		p.bHPPercent = 0;
#ifdef __VIEW_TARGET_DECIMAL_HP__
		p.iMinHP = 0;
		p.iMaxHP = 0;
#endif
	}
	else
	{
#ifdef __VIEW_TARGET_PLAYER_HP__
		p.bHPPercent = MINMAX(0, (float(GetHP()) / float(GetMaxHP()) * 100.0f), 100);
#ifdef __VIEW_TARGET_DECIMAL_HP__
		p.iMinHP = GetHP();
		p.iMaxHP = GetMaxHP();
#endif
#else
		if (IsPC())
		{
			p.bHPPercent = 0;
#ifdef __VIEW_TARGET_DECIMAL_HP__
			p.iMinHP = 0;
			p.iMaxHP = 0;
#endif
		}
		else
		{
			p.bHPPercent = MINMAX(0, (float(GetHP()) / float(GetMaxHP()) * 100.0f), 100);
#ifdef __VIEW_TARGET_DECIMAL_HP__
			p.iMinHP = GetHP();
			p.iMaxHP = GetMaxHP();
#endif
		}
#endif
	}

	CHARACTER_SET::iterator it = m_set_pkChrTargetedBy.begin();
	while (it != m_set_pkChrTargetedBy.end())
	{
		LPCHARACTER pkChr = *it++;
		if (!pkChr->GetDesc())
		{
			sys_err("%s %p does not have desc", pkChr->GetName(), get_pointer(pkChr));
			abort();
		}

		pkChr->GetDesc()->Packet(&p, sizeof(TPacketGCTarget));
	}
}

void CHARACTER::CheckTarget()
{
	if (!m_pkChrTarget)
		return;

	if (DISTANCE_APPROX(GetX() - m_pkChrTarget->GetX(), GetY() - m_pkChrTarget->GetY()) >= 4800)
		SetTarget(NULL);
}

void CHARACTER::SetWarpLocation(long lMapIndex, long x, long y)
{
	m_posWarp.x = x * 100;
	m_posWarp.y = y * 100;
	m_lWarpMapIndex = lMapIndex;
}

void CHARACTER::SaveExitLocation()
{
	m_posExit = GetXYZ();
	m_lExitMapIndex = GetMapIndex();
}

void CHARACTER::ExitToSavedLocation()
{
	sys_log (0, "ExitToSavedLocation");
	WarpSet(m_posWarp.x, m_posWarp.y, m_lWarpMapIndex);

	m_posExit.x = m_posExit.y = m_posExit.z = 0;
	m_lExitMapIndex = 0;
}

// fixme 
// ���ݱ��� privateMapIndex �� ���� �� �ε����� ������ üũ �ϴ� ���� �ܺο��� �ϰ�,
// �ٸ��� warpset�� �ҷ��µ�
// �̸� warpset ������ ����.
bool CHARACTER::WarpSet(long x, long y, long lPrivateMapIndex)
{
	if (!IsPC())
		return false;

	long lAddr;
	long lMapIndex;
	WORD wPort;

	if (!CMapLocation::instance().Get(x, y, lMapIndex, lAddr, wPort))
	{
		sys_err("cannot find map location index %d x %d y %d name %s", lMapIndex, x, y, GetName());
		return false;
	}

	//Send Supplementary Data Block if new map requires security packages in loading this map
	{
		long lCurAddr;
		long lCurMapIndex = 0;
		WORD wCurPort;

		CMapLocation::instance().Get(GetX(), GetY(), lCurMapIndex, lCurAddr, wCurPort);

		//do not send SDB files if char is in the same map
		if( lCurMapIndex != lMapIndex )
		{
			const TMapRegion * rMapRgn = SECTREE_MANAGER::instance().GetMapRegion(lMapIndex);
			{
				DESC_MANAGER::instance().SendClientPackageSDBToLoadMap( GetDesc(), rMapRgn->strMapName.c_str() );	
			}
		}
	}

	if (lPrivateMapIndex >= 10000)
	{
		if (lPrivateMapIndex / 10000 != lMapIndex)
		{
			sys_err("Invalid map inedx %d, must be child of %d", lPrivateMapIndex, lMapIndex);
			return false;
		}

		lMapIndex = lPrivateMapIndex;
	}

	Stop();
	Save();

	if (GetSectree())
	{
		GetSectree()->RemoveEntity(this);
		ViewCleanup();

		EncodeRemovePacket(this);
	}

	m_lWarpMapIndex = lMapIndex;
	m_posWarp.x = x;
	m_posWarp.y = y;

	sys_log(0, "WarpSet %s %d %d current map %d target map %d", GetName(), x, y, GetMapIndex(), lMapIndex);

	TPacketGCWarp p;

	p.bHeader	= HEADER_GC_WARP;
	p.lX	= x;
	p.lY	= y;
	p.lAddr	= lAddr;
	p.wPort	= wPort;

#ifdef ENABLE_SWITCHBOT
	CSwitchbotManager::Instance().SetIsWarping(GetPlayerID(), true);

	if (p.wPort != mother_port)
	{
		CSwitchbotManager::Instance().P2PSendSwitchbot(GetPlayerID(), p.wPort);
	}
#endif

	GetDesc()->Packet(&p, sizeof(TPacketGCWarp));

	//if (!LC_IsNewCIBN())
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "%s MapIdx %ld DestMapIdx%ld DestX%ld DestY%ld Empire%d", GetName(), GetMapIndex(), lPrivateMapIndex, x, y, GetEmpire());
		LogManager::instance().CharLog(this, 0, "WARP", buf);
	}

	return true;
}

void CHARACTER::WarpEnd()
{
	if (test_server)
		sys_log(0, "WarpEnd %s", GetName());

	if (m_posWarp.x == 0 && m_posWarp.y == 0)
		return;

	int index = m_lWarpMapIndex;

	if (index > 10000)
		index /= 10000;

	if (!map_allow_find(index))
	{
		// �� ������ ������ �� �����Ƿ� �����ϱ� �� ��ǥ�� �ǵ�����.
		sys_err("location %d %d not allowed to login this server", m_posWarp.x, m_posWarp.y);
		GetDesc()->SetPhase(PHASE_CLOSE);
		return;
	}

	sys_log(0, "WarpEnd %s %d %u %u", GetName(), m_lWarpMapIndex, m_posWarp.x, m_posWarp.y);

	Show(m_lWarpMapIndex, m_posWarp.x, m_posWarp.y, 0);
	Stop();

	m_lWarpMapIndex = 0;
	m_posWarp.x = m_posWarp.y = m_posWarp.z = 0;

	{
		// P2P Login
		TPacketGGLogin p;

		p.bHeader = HEADER_GG_LOGIN;
		strlcpy(p.szName, GetName(), sizeof(p.szName));
		p.dwPID = GetPlayerID();
		p.bEmpire = GetEmpire();
		p.lMapIndex = SECTREE_MANAGER::instance().GetMapIndex(GetX(), GetY());
		p.bChannel = g_bChannel;

		P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGLogin));
	}
}


bool CHARACTER::Return()
{
	if (!IsNPC())
		return false;

	int x, y;
	/*
	   float fDist = DISTANCE_SQRT(m_pkMobData->m_posLastAttacked.x - GetX(), m_pkMobData->m_posLastAttacked.y - GetY());
	   float fx, fy;
	   GetDeltaByDegree(GetRotation(), fDist, &fx, &fy);
	   x = GetX() + (int) fx;
	   y = GetY() + (int) fy;
	 */
	SetVictim(NULL);

	x = m_pkMobInst->m_posLastAttacked.x;
	y = m_pkMobInst->m_posLastAttacked.y;

	SetRotationToXY(x, y);

	if (!Goto(x, y))
		return false;

	SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);

	if (test_server)
		sys_log(0, "%s %p �����ϰ� ���ư���! %d %d", GetName(), this, x, y);

	if (GetParty())
		GetParty()->SendMessage(this, PM_RETURN, x, y);

	return true;
}

bool CHARACTER::Follow(LPCHARACTER pkChr, float fMinDistance)
{
	if (IsPC())
	{
		sys_err("CHARACTER::Follow : PC cannot use this method", GetName());
		return false;
	}
	
	bool yenibinek = false;
	
	if (IsNPC())
	{
		if (CMobVnumHelper::Binekmi(GetRaceNum()))
		{
			yenibinek = true;
		}
	}

	// TRENT_MONSTER
	if (IS_SET(m_pointsInstant.dwAIFlag, AIFLAG_NOMOVE) && yenibinek == false)
	{
		if (pkChr->IsPC()) // �Ѿư��� ��밡 PC�� ��
		{
			// If i'm in a party. I must obey party leader's AI.
			if (!GetParty() || !GetParty()->GetLeader() || GetParty()->GetLeader() == this)
			{
				if (get_dword_time() - m_pkMobInst->m_dwLastAttackedTime >= 15000) // ���������� ���ݹ����� 15�ʰ� ������
				{
					// ������ ���� ������ ���� 50���� �̻� ���̳��� �����ϰ� ���ư���.
					if (m_pkMobData->m_table.wAttackRange < DISTANCE_APPROX(pkChr->GetX() - GetX(), pkChr->GetY() - GetY()))
						if (Return())
							return true;
				}
			}
		}
		return false;
	}
	// END_OF_TRENT_MONSTER

	long x = pkChr->GetX();
	long y = pkChr->GetY();

	if (pkChr->IsPC() && yenibinek == false) // �Ѿư��� ��밡 PC�� ��
	{
		// If i'm in a party. I must obey party leader's AI.
		if (!GetParty() || !GetParty()->GetLeader() || GetParty()->GetLeader() == this)
		{
			if (get_dword_time() - m_pkMobInst->m_dwLastAttackedTime >= 15000) // ���������� ���ݹ����� 15�ʰ� ������
			{
				// ������ ���� ������ ���� 50���� �̻� ���̳��� �����ϰ� ���ư���.
				if (5000 < DISTANCE_APPROX(m_pkMobInst->m_posLastAttacked.x - GetX(), m_pkMobInst->m_posLastAttacked.y - GetY()))
					if (Return())
						return true;
			}
		}
	}

	if (IsGuardNPC())
	{
		if (5000 < DISTANCE_APPROX(m_pkMobInst->m_posLastAttacked.x - GetX(), m_pkMobInst->m_posLastAttacked.y - GetY()))
			if (Return())
				return true;
	}

#ifdef ENABLE_NEW_PET_SYSTEM
	if (pkChr->IsState(pkChr->m_stateMove) &&
		GetMobBattleType() != BATTLE_TYPE_RANGE &&
		GetMobBattleType() != BATTLE_TYPE_MAGIC &&
		false == IsPet() && false == IsNewPet())
#else
#ifdef LWT_BUFFI_SYSTEM
	if (pkChr->IsState(pkChr->m_stateMove) &&
		GetMobBattleType() != BATTLE_TYPE_RANGE &&
		GetMobBattleType() != BATTLE_TYPE_MAGIC &&
		false == IsPet() && false == IsSupport())
#else
	if (pkChr->IsState(pkChr->m_stateMove) && 
		GetMobBattleType() != BATTLE_TYPE_RANGE && 
		GetMobBattleType() != BATTLE_TYPE_MAGIC &&
		false == IsPet()
#endif
#endif
	{
		// ����� �̵����̸� ���� �̵��� �Ѵ�
		// ���� ������ �ӵ����� �Ÿ��κ��� ���� �ð��� ������ ��
		// ������ �� �ð����� �������� �̵��Ѵٰ� �����Ͽ� �ű�� �̵��Ѵ�.
		float rot = pkChr->GetRotation();
		float rot_delta = GetDegreeDelta(rot, GetDegreeFromPositionXY(GetX(), GetY(), pkChr->GetX(), pkChr->GetY()));

		float yourSpeed = pkChr->GetMoveSpeed();
		float mySpeed = GetMoveSpeed();

		float fDist = DISTANCE_SQRT(x - GetX(), y - GetY());
		float fFollowSpeed = mySpeed - yourSpeed * cos(rot_delta * M_PI / 180);

		if (fFollowSpeed >= 0.1f)
		{
			float fMeetTime = fDist / fFollowSpeed;
			float fYourMoveEstimateX, fYourMoveEstimateY;

			if( fMeetTime * yourSpeed <= 100000.0f )
			{
				GetDeltaByDegree(pkChr->GetRotation(), fMeetTime * yourSpeed, &fYourMoveEstimateX, &fYourMoveEstimateY);

				x += (long) fYourMoveEstimateX;
				y += (long) fYourMoveEstimateY;

				float fDistNew = sqrt(((double)x - GetX())*(x-GetX())+((double)y - GetY())*(y-GetY()));
				if (fDist < fDistNew)
				{
					x = (long)(GetX() + (x - GetX()) * fDist / fDistNew);
					y = (long)(GetY() + (y - GetY()) * fDist / fDistNew);
				}
			}
		}
	}

	// ������ ��ġ�� �ٶ���� �Ѵ�.
	SetRotationToXY(x, y);

	float fDist = DISTANCE_SQRT(x - GetX(), y - GetY());

	if (fDist <= fMinDistance)
		return false;

	float fx, fy;

	if (IsChangeAttackPosition(pkChr) && GetMobRank() < MOB_RANK_BOSS)
	{
		// ���� �ֺ� ������ ������ �̵�
		SetChangeAttackPositionTime();

		int retry = 16;
		int dx, dy;
		int rot = (int) GetDegreeFromPositionXY(x, y, GetX(), GetY());

		while (--retry)
		{
			if (fDist < 500.0f)
				GetDeltaByDegree((rot + number(-90, 90) + number(-90, 90)) % 360, fMinDistance, &fx, &fy);
			else
				GetDeltaByDegree(number(0, 359), fMinDistance, &fx, &fy);

			dx = x + (int) fx;
			dy = y + (int) fy;

			LPSECTREE tree = SECTREE_MANAGER::instance().Get(GetMapIndex(), dx, dy);

			if (NULL == tree)
				break;

			if (0 == (tree->GetAttribute(dx, dy) & (ATTR_BLOCK | ATTR_OBJECT)))
				break;
		}

		//sys_log(0, "��ó ��򰡷� �̵� %s retry %d", GetName(), retry);
		if (!Goto(dx, dy))
			return false;
	}
	else
	{
		// ���� ���󰡱�
		float fDistToGo = fDist - fMinDistance;
		GetDeltaByDegree(GetRotation(), fDistToGo, &fx, &fy);

		//sys_log(0, "�������� �̵� %s", GetName());
		if (!Goto(GetX() + (int) fx, GetY() + (int) fy))
			return false;
	}

	SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);
	//MonsterLog("�Ѿư���; %s", pkChr->GetName());
	return true;
}

float CHARACTER::GetDistanceFromSafeboxOpen() const
{
	return DISTANCE_APPROX(GetX() - m_posSafeboxOpen.x, GetY() - m_posSafeboxOpen.y);
}

void CHARACTER::SetSafeboxOpenPosition()
{
	m_posSafeboxOpen = GetXYZ();
}

CSafebox * CHARACTER::GetSafebox() const
{
	return m_pkSafebox;
}

void CHARACTER::ReqSafeboxLoad(const char* pszPassword)
{
	if (!*pszPassword || strlen(pszPassword) > SAFEBOX_PASSWORD_MAX_LEN)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<â��> �߸��� ��ȣ�� �Է��ϼ̽��ϴ�."));
		return;
	}
	else if (m_pkSafebox)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<â��> â�� �̹� �����ֽ��ϴ�."));
		return;
	}

	int iPulse = thecore_pulse();

	if (iPulse - GetSafeboxLoadTime()  < PASSES_PER_SEC(10))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<â��> â�� ������ 10�� �ȿ��� �� �� �����ϴ�."));
		return;
	}
	//else if (GetDistanceFromSafeboxOpen() > 1000)
//	{
	//	ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<â��> �Ÿ��� �־ â�� �� �� �����ϴ�."));
	//	return;
//	}
	else if (m_bOpeningSafebox)
	{
		sys_log(0, "Overlapped safebox load request from %s", GetName());
		return;
	}

	SetSafeboxLoadTime();
	m_bOpeningSafebox = true;

	TSafeboxLoadPacket p;
	p.dwID = GetDesc()->GetAccountTable().id;
	strlcpy(p.szLogin, GetDesc()->GetAccountTable().login, sizeof(p.szLogin));
	strlcpy(p.szPassword, pszPassword, sizeof(p.szPassword));

	db_clientdesc->DBPacket(HEADER_GD_SAFEBOX_LOAD, GetDesc()->GetHandle(), &p, sizeof(p));
}

void CHARACTER::LoadSafebox(int iSize, DWORD dwGold, int iItemCount, TPlayerItem * pItems)
{
	bool bLoaded = false;

	//PREVENT_TRADE_WINDOW
	SetOpenSafebox(true);
	//END_PREVENT_TRADE_WINDOW

	if (m_pkSafebox)
		bLoaded = true;

	if (!m_pkSafebox)
		m_pkSafebox = M2_NEW CSafebox(this, iSize, dwGold);
	else
		m_pkSafebox->ChangeSize(iSize);

	m_iSafeboxSize = iSize;

	TPacketCGSafeboxSize p;

	p.bHeader = HEADER_GC_SAFEBOX_SIZE;
	p.bSize = iSize;

	GetDesc()->Packet(&p, sizeof(TPacketCGSafeboxSize));

	if (!bLoaded)
	{
		for (int i = 0; i < iItemCount; ++i, ++pItems)
		{
			if (!m_pkSafebox->IsValidPosition(pItems->pos))
				continue;

			LPITEM item = ITEM_MANAGER::instance().CreateItem(pItems->vnum, pItems->count, pItems->id);

			if (!item)
			{
				sys_err("cannot create item vnum %d id %u (name: %s)", pItems->vnum, pItems->id, GetName());
				continue;
			}

			item->SetSkipSave(true);
			item->SetSockets(pItems->alSockets);
			item->SetAttributes(pItems->aAttr);
			item->SetEvolution(pItems->evolution);

			if (!m_pkSafebox->Add(pItems->pos, item))
			{
				M2_DESTROY_ITEM(item);
			}
			else
				item->SetSkipSave(false);
		}
	}
}

void CHARACTER::ChangeSafeboxSize(BYTE bSize)
{
	//if (!m_pkSafebox)
	//return;

	TPacketCGSafeboxSize p;

	p.bHeader = HEADER_GC_SAFEBOX_SIZE;
	p.bSize = bSize;

	GetDesc()->Packet(&p, sizeof(TPacketCGSafeboxSize));

	if (m_pkSafebox)
		m_pkSafebox->ChangeSize(bSize);

	m_iSafeboxSize = bSize;
}

void CHARACTER::CloseSafebox()
{
	if (!m_pkSafebox)
		return;

	//PREVENT_TRADE_WINDOW
	SetOpenSafebox(false);
	//END_PREVENT_TRADE_WINDOW

	m_pkSafebox->Save();

	M2_DELETE(m_pkSafebox);
	m_pkSafebox = NULL;

	ChatPacket(CHAT_TYPE_COMMAND, "CloseSafebox");

	SetSafeboxLoadTime();
	m_bOpeningSafebox = false;

	Save();
}

CSafebox * CHARACTER::GetMall() const
{
	return m_pkMall;
}

void CHARACTER::LoadMall(int iItemCount, TPlayerItem * pItems)
{
	bool bLoaded = false;

	if (m_pkMall)
		bLoaded = true;

	if (!m_pkMall)
		m_pkMall = M2_NEW CSafebox(this, 3 * SAFEBOX_PAGE_SIZE, 0);
	else
		m_pkMall->ChangeSize(3 * SAFEBOX_PAGE_SIZE);

	m_pkMall->SetWindowMode(MALL);

	TPacketCGSafeboxSize p;

	p.bHeader = HEADER_GC_MALL_OPEN;
	p.bSize = 3 * SAFEBOX_PAGE_SIZE;

	GetDesc()->Packet(&p, sizeof(TPacketCGSafeboxSize));

	if (!bLoaded)
	{
		for (int i = 0; i < iItemCount; ++i, ++pItems)
		{
			if (!m_pkMall->IsValidPosition(pItems->pos))
				continue;

			LPITEM item = ITEM_MANAGER::instance().CreateItem(pItems->vnum, pItems->count, pItems->id);

			if (!item)
			{
				sys_err("cannot create item vnum %d id %u (name: %s)", pItems->vnum, pItems->id, GetName());
				continue;
			}

			item->SetSkipSave(true);
			item->SetSockets(pItems->alSockets);
			item->SetAttributes(pItems->aAttr);
			item->SetEvolution(pItems->evolution);

			if (!m_pkMall->Add(pItems->pos, item))
				M2_DESTROY_ITEM(item);
			else
				item->SetSkipSave(false);
		}
	}
}

void CHARACTER::CloseMall()
{
	if (!m_pkMall)
		return;

	m_pkMall->Save();

	M2_DELETE(m_pkMall);
	m_pkMall = NULL;

	ChatPacket(CHAT_TYPE_COMMAND, "CloseMall");
}

bool CHARACTER::BuildUpdatePartyPacket(TPacketGCPartyUpdate & out)
{
	if (!GetParty())
		return false;

	memset(&out, 0, sizeof(out));

	out.header		= HEADER_GC_PARTY_UPDATE;
	out.pid		= GetPlayerID();
	out.percent_hp	= MINMAX(0, (float(GetHP()) / float(GetMaxHP()) * 100.0f), 100);
	out.role		= GetParty()->GetRole(GetPlayerID());

	sys_log(1, "PARTY %s role is %d", GetName(), out.role);

	LPCHARACTER l = GetParty()->GetLeaderCharacter();

	if (l && DISTANCE_APPROX(GetX() - l->GetX(), GetY() - l->GetY()) < PARTY_DEFAULT_RANGE)
	{
		if (g_iUseLocale)
			out.affects[0] = GetParty()->GetPartyBonusExpPercent();
		else
			out.affects[0] = GetParty()->GetExpBonusPercent();
		out.affects[1] = GetPoint(POINT_PARTY_ATTACKER_BONUS);
		out.affects[2] = GetPoint(POINT_PARTY_TANKER_BONUS);
		out.affects[3] = GetPoint(POINT_PARTY_BUFFER_BONUS);
		out.affects[4] = GetPoint(POINT_PARTY_SKILL_MASTER_BONUS);
		out.affects[5] = GetPoint(POINT_PARTY_HASTE_BONUS);
		out.affects[6] = GetPoint(POINT_PARTY_DEFENDER_BONUS);
	}

	return true;
}

int CHARACTER::GetLeadershipSkillLevel() const
{ 
	return GetSkillLevel(SKILL_LEADERSHIP);
}

void CHARACTER::QuerySafeboxSize()
{
	if (m_iSafeboxSize == -1)
	{
		DBManager::instance().ReturnQuery(QID_SAFEBOX_SIZE,
				GetPlayerID(),
				NULL, 
				"SELECT size FROM safebox%s WHERE account_id = %u",
				get_table_postfix(),
				GetDesc()->GetAccountTable().id);
	}
}

void CHARACTER::SetSafeboxSize(int iSize)
{
	sys_log(1, "SetSafeboxSize: %s %d", GetName(), iSize);
	m_iSafeboxSize = iSize;
	DBManager::instance().Query("UPDATE safebox%s SET size = %d WHERE account_id = %u", get_table_postfix(), iSize / SAFEBOX_PAGE_SIZE, GetDesc()->GetAccountTable().id);
}

int CHARACTER::GetSafeboxSize() const
{
	return m_iSafeboxSize;
}

void CHARACTER::SetNowWalking(bool bWalkFlag)
{
	//if (m_bNowWalking != bWalkFlag || IsNPC())
	if (m_bNowWalking != bWalkFlag)
	{
		if (bWalkFlag)
		{
			m_bNowWalking = true;
			m_dwWalkStartTime = get_dword_time();
		}
		else
		{
			m_bNowWalking = false;
		}

		//if (m_bNowWalking)
		{
			TPacketGCWalkMode p;
			p.vid = GetVID();
			p.header = HEADER_GC_WALK_MODE;
			p.mode = m_bNowWalking ? WALKMODE_WALK : WALKMODE_RUN;

			PacketView(&p, sizeof(p));
		}

		if (IsNPC())
		{
			if (m_bNowWalking)
				MonsterLog("??��?");
			else
				MonsterLog("???");
		}

		//sys_log(0, "%s is now %s", GetName(), m_bNowWalking?"walking.":"running.");
	}
}

void CHARACTER::StartStaminaConsume()
{
	if (m_bStaminaConsume)
		return;
	PointChange(POINT_STAMINA, 0);
	m_bStaminaConsume = true;
	//ChatPacket(CHAT_TYPE_COMMAND, "StartStaminaConsume %d %d", STAMINA_PER_STEP * passes_per_sec, GetStamina());
	if (IsStaminaHalfConsume())
		ChatPacket(CHAT_TYPE_COMMAND, "StartStaminaConsume %d %d", STAMINA_PER_STEP * passes_per_sec / 2, GetStamina());
	else
		ChatPacket(CHAT_TYPE_COMMAND, "StartStaminaConsume %d %d", STAMINA_PER_STEP * passes_per_sec, GetStamina());
}

void CHARACTER::StopStaminaConsume()
{
	if (!m_bStaminaConsume)
		return;
	PointChange(POINT_STAMINA, 0);
	m_bStaminaConsume = false;
	ChatPacket(CHAT_TYPE_COMMAND, "StopStaminaConsume %d", GetStamina());
}

bool CHARACTER::IsStaminaConsume() const
{
	return m_bStaminaConsume;
}

bool CHARACTER::IsStaminaHalfConsume() const
{
	return IsEquipUniqueItem(UNIQUE_ITEM_HALF_STAMINA);
}

void CHARACTER::ResetStopTime()
{
	m_dwStopTime = get_dword_time();
}

DWORD CHARACTER::GetStopTime() const
{
	return m_dwStopTime;
}

void CHARACTER::ResetPoint(int iLv)
{
	BYTE bJob = GetJob();

	PointChange(POINT_LEVEL, iLv - GetLevel());

	SetRealPoint(POINT_ST, JobInitialPoints[bJob].st);
	SetPoint(POINT_ST, GetRealPoint(POINT_ST));

	SetRealPoint(POINT_HT, JobInitialPoints[bJob].ht);
	SetPoint(POINT_HT, GetRealPoint(POINT_HT));

	SetRealPoint(POINT_DX, JobInitialPoints[bJob].dx);
	SetPoint(POINT_DX, GetRealPoint(POINT_DX));

	SetRealPoint(POINT_IQ, JobInitialPoints[bJob].iq);
	SetPoint(POINT_IQ, GetRealPoint(POINT_IQ));

	SetRandomHP((iLv - 1) * number(JobInitialPoints[GetJob()].hp_per_lv_begin, JobInitialPoints[GetJob()].hp_per_lv_end));
	SetRandomSP((iLv - 1) * number(JobInitialPoints[GetJob()].sp_per_lv_begin, JobInitialPoints[GetJob()].sp_per_lv_end));

	//PointChange(POINT_STAT, ((MINMAX(1, iLv, 99) - 1) * 3) + GetPoint(POINT_LEVEL_STEP) - GetPoint(POINT_STAT));
	PointChange(POINT_STAT, ((MINMAX(1, iLv, 90) - 1) * 3) + GetPoint(POINT_LEVEL_STEP) - GetPoint(POINT_STAT));

	ComputePoints();

	// ȸ��
	PointChange(POINT_HP, GetMaxHP() - GetHP());
	PointChange(POINT_SP, GetMaxSP() - GetSP());

	PointsPacket();

	LogManager::instance().CharLog(this, 0, "RESET_POINT", "");
}

bool CHARACTER::IsChangeAttackPosition(LPCHARACTER target) const
{ 
	if (!IsNPC())
		return true;

	DWORD dwChangeTime = AI_CHANGE_ATTACK_POISITION_TIME_NEAR;

	if (DISTANCE_APPROX(GetX() - target->GetX(), GetY() - target->GetY()) > 
		AI_CHANGE_ATTACK_POISITION_DISTANCE + GetMobAttackRange())
		dwChangeTime = AI_CHANGE_ATTACK_POISITION_TIME_FAR;

	return get_dword_time() - m_dwLastChangeAttackPositionTime > dwChangeTime; 
}

void CHARACTER::GiveRandomSkillBook()
{
	LPITEM item = AutoGiveItem(50300);

	if (NULL != item)
	{
		BYTE bJob = 0;

		if (!number(0, 1))
			bJob = GetJob() + 1;

		DWORD dwSkillVnum = 0;

		do
		{
			dwSkillVnum = number(1, 111);
			const CSkillProto* pkSk = CSkillManager::instance().Get(dwSkillVnum);

			if (NULL == pkSk)
				continue;

			if (bJob && bJob != pkSk->dwType)
				continue;

			break;
		} while (true);

		item->SetSocket(0, dwSkillVnum);
	}
}

void CHARACTER::ReviveInvisible(int iDur)
{
	AddAffect(AFFECT_REVIVE_INVISIBLE, POINT_NONE, 0, AFF_REVIVE_INVISIBLE, iDur, 0, true);
}

void CHARACTER::ToggleMonsterLog()
{
	m_bMonsterLog = !m_bMonsterLog;

	if (m_bMonsterLog)
	{
		CHARACTER_MANAGER::instance().RegisterForMonsterLog(this);
	}
	else
	{
		CHARACTER_MANAGER::instance().UnregisterForMonsterLog(this);
	}
}

#ifdef __LEADER_ON_GUILD__
void CHARACTER::CheckGuildAffect()
{
	if (IsPC())
	{
		bool update = false;
		if (m_pGuild)
		{
			if (GetPlayerID() == m_pGuild->GetMasterPID())
			{
				if (!m_afAffectFlag.IsSet(AFF_GUILD))
				{
					m_afAffectFlag.Set(AFF_GUILD);
					update = true;
				}
			}
			else
			{
				if (m_afAffectFlag.IsSet(AFF_GUILD))
				{
					m_afAffectFlag.Reset(AFF_GUILD);
					update = true;
				}
			}
		}
		else if (!m_pGuild)
		{
			if (m_afAffectFlag.IsSet(AFF_GUILD))
			{
				m_afAffectFlag.Reset(AFF_GUILD);
				update = true;
			}
		}

		if (update)
			UpdatePacket();
	}
	
}
#endif

void CHARACTER::SetGuild(CGuild* pGuild)
{
	if (m_pGuild != pGuild)
	{
		m_pGuild = pGuild;
		UpdatePacket();
	}
#ifdef __LEADER_ON_GUILD__
	CheckGuildAffect();
#endif
}

void CHARACTER::SendGreetMessage()
{
	__typeof  (DBManager::instance().GetGreetMessage()) v = DBManager::instance().GetGreetMessage();

	for (itertype(v) it = v.begin(); it != v.end(); ++it)
	{
		ChatPacket(CHAT_TYPE_NOTICE, it->c_str());
	}
}

void CHARACTER::BeginStateEmpty()
{
	MonsterLog("!");
}

void CHARACTER::EffectPacket(int enumEffectType)
{
	TPacketGCSpecialEffect p;

	p.header = HEADER_GC_SEPCIAL_EFFECT;
	p.type = enumEffectType;
	p.vid = GetVID();

	PacketAround(&p, sizeof(TPacketGCSpecialEffect));
}

void CHARACTER::SpecificEffectPacket(const char filename[MAX_EFFECT_FILE_NAME])
{
	TPacketGCSpecificEffect p;

	p.header = HEADER_GC_SPECIFIC_EFFECT;
	p.vid = GetVID();
	memcpy (p.effect_file, filename, MAX_EFFECT_FILE_NAME);

	PacketAround(&p, sizeof(TPacketGCSpecificEffect));
}

void CHARACTER::MonsterChat(BYTE bMonsterChatType)
{
	if (IsPC())
		return;

	char sbuf[256+1];

	if (IsMonster())
	{
		if (number(0, 60))
			return;

		snprintf(sbuf, sizeof(sbuf), 
				"(locale.monster_chat[%i] and locale.monster_chat[%i][%d] or '')",
				GetRaceNum(), GetRaceNum(), bMonsterChatType*3 + number(1, 3));
	}
	else
	{
		if (bMonsterChatType != MONSTER_CHAT_WAIT)
			return;

		if (IsGuardNPC())
		{
			if (number(0, 6))
				return;
		}
		else
		{
			if (number(0, 30))
				return;
		}

		snprintf(sbuf, sizeof(sbuf), "(locale.monster_chat[%i] and locale.monster_chat[%i][number(1, table.getn(locale.monster_chat[%i]))] or '')", GetRaceNum(), GetRaceNum(), GetRaceNum());
	}

	std::string text = quest::ScriptToString(sbuf);

	if (text.empty())
		return;

	struct packet_chat pack_chat;

	pack_chat.header    = HEADER_GC_CHAT;
	pack_chat.size	= sizeof(struct packet_chat) + text.size() + 1;
	pack_chat.type      = CHAT_TYPE_TALKING;
	pack_chat.id        = GetVID();
	pack_chat.bEmpire	= 0;

	TEMP_BUFFER buf;
	buf.write(&pack_chat, sizeof(struct packet_chat));
	buf.write(text.c_str(), text.size() + 1);

	PacketAround(buf.read_peek(), buf.size());
}

void CHARACTER::SetQuestNPCID(DWORD vid)
{
	m_dwQuestNPCVID = vid;
}

LPCHARACTER CHARACTER::GetQuestNPC() const
{
	return CHARACTER_MANAGER::instance().Find(m_dwQuestNPCVID);
}

void CHARACTER::SetQuestItemPtr(LPITEM item)
{
	m_pQuestItem = item;
}

void CHARACTER::ClearQuestItemPtr()
{
	m_pQuestItem = NULL;
}

LPITEM CHARACTER::GetQuestItemPtr() const
{
	return m_pQuestItem;
}

LPDUNGEON CHARACTER::GetDungeonForce() const
{ 
	if (m_lWarpMapIndex > 10000)
		return CDungeonManager::instance().FindByMapIndex(m_lWarpMapIndex);

	return m_pkDungeon;
}

void CHARACTER::SetBlockMode(BYTE bFlag)
{
	m_pointsInstant.bBlockMode = bFlag;

	ChatPacket(CHAT_TYPE_COMMAND, "setblockmode %d", m_pointsInstant.bBlockMode);

	SetQuestFlag("game_option.block_exchange", bFlag & BLOCK_EXCHANGE ? 1 : 0);
	SetQuestFlag("game_option.block_party_invite", bFlag & BLOCK_PARTY_INVITE ? 1 : 0);
	SetQuestFlag("game_option.block_guild_invite", bFlag & BLOCK_GUILD_INVITE ? 1 : 0);
	SetQuestFlag("game_option.block_whisper", bFlag & BLOCK_WHISPER ? 1 : 0);
	SetQuestFlag("game_option.block_messenger_invite", bFlag & BLOCK_MESSENGER_INVITE ? 1 : 0);
	SetQuestFlag("game_option.block_party_request", bFlag & BLOCK_PARTY_REQUEST ? 1 : 0);
}

void CHARACTER::SetBlockModeForce(BYTE bFlag)
{
	m_pointsInstant.bBlockMode = bFlag;
	ChatPacket(CHAT_TYPE_COMMAND, "setblockmode %d", m_pointsInstant.bBlockMode);
}

bool CHARACTER::IsGuardNPC() const
{
	return IsNPC() && (GetRaceNum() == 11000 || GetRaceNum() == 11002 || GetRaceNum() == 11004);
}

int CHARACTER::GetPolymorphPower() const
{
	if (test_server)
	{
		int value = quest::CQuestManager::instance().GetEventFlag("poly");
		if (value)
			return value;
	}
	return aiPolymorphPowerByLevel[MINMAX(0, GetSkillLevel(SKILL_POLYMORPH), 40)];
}

void CHARACTER::SetPolymorph(DWORD dwRaceNum, bool bMaintainStat)
{
	if (dwRaceNum < JOB_MAX_NUM)
	{
		dwRaceNum = 0;
		bMaintainStat = false;
	}

	if (m_dwPolymorphRace == dwRaceNum)
		return;

	m_bPolyMaintainStat = bMaintainStat;
	m_dwPolymorphRace = dwRaceNum;

	sys_log(0, "POLYMORPH: %s race %u ", GetName(), dwRaceNum);

	if (dwRaceNum != 0)
		StopRiding();

	SET_BIT(m_bAddChrState, ADD_CHARACTER_STATE_SPAWN);
	m_afAffectFlag.Set(AFF_SPAWN);

	ViewReencode();

	REMOVE_BIT(m_bAddChrState, ADD_CHARACTER_STATE_SPAWN);

	if (!bMaintainStat)
	{
		PointChange(POINT_ST, 0);
		PointChange(POINT_DX, 0);
		PointChange(POINT_IQ, 0);
		PointChange(POINT_HT, 0);
	}

	// �������� ���¿��� �״� ���, ���������� Ǯ���� �Ǵµ�
	// ���� ���� ���ķ� valid combo interval�� �ٸ��� ������
	// Combo �� �Ǵ� Hacker�� �ν��ϴ� ��찡 �ִ�.
	// ���� ���������� Ǯ�ų� �������� �ϰ� �Ǹ�,
	// valid combo interval�� reset�Ѵ�.
	SetValidComboInterval(0);
	SetComboSequence(0);

	ComputeBattlePoints();
}

int CHARACTER::GetQuestFlag(const std::string& flag) const
{
	if (!IsPC())
	{
		sys_err("Trying to get qf %s from non player character", flag.c_str());
		return 0;
	}

	// DWORD pid = GetPlayerID();
	auto &q = quest::CQuestManager::instance();
	auto pPC = q.GetPC(GetPlayerID());

	if (!pPC)
	{
		// sys_err("Nullpointer when trying to access questflag %s for player with pid %u", flag.c_str(), pid);
		return 0;
	}

	return pPC->GetFlag(flag);
}

void CHARACTER::SetQuestFlag(const std::string& flag, int value)
{
	// DWORD pid = GetPlayerID();
	auto &q = quest::CQuestManager::instance();
	auto pPC = q.GetPC(GetPlayerID());

	if (!pPC)
	{
		// sys_err("Nullpointer when trying to set questflag %s for player with pid %u", flag.c_str(), pid);
		return;
	}

	pPC->SetFlag(flag, value);
}

void CHARACTER::DetermineDropMetinStone()
{
	const int METIN_STONE_NUM = 17;
	static DWORD c_adwMetin[METIN_STONE_NUM] = 
	{
		28030,
		28031,
		28032,
		28033,
		28034,
		28035,
		28036,
		28037,
		28038,
		28039,
		28040,
		28041,
		28042,
		28043,
		28044,
		28045,
		28046,
	};
	DWORD stone_num = GetRaceNum();
	int idx = std::lower_bound(aStoneDrop, aStoneDrop+STONE_INFO_MAX_NUM, stone_num) - aStoneDrop;
	if (idx >= STONE_INFO_MAX_NUM || aStoneDrop[idx].dwMobVnum != stone_num)
	{
		m_dwDropMetinStone = 0;
	}
	else
	{
		const SStoneDropInfo & info = aStoneDrop[idx];
		m_bDropMetinStonePct = info.iDropPct;
		{
			m_dwDropMetinStone = c_adwMetin[number(0, METIN_STONE_NUM - 1)];
			int iGradePct = number(1, 100);
			for (int iStoneLevel = 0; iStoneLevel < STONE_LEVEL_MAX_NUM; iStoneLevel ++)
			{
				int iLevelGradePortion = info.iLevelPct[iStoneLevel];
				if (iGradePct <= iLevelGradePortion)
				{
					break;
				}
				else
				{
					iGradePct -= iLevelGradePortion;
					m_dwDropMetinStone += 100; // �� +a -> +(a+1)�� �ɶ����� 100�� ����
				}
			}
		}
	}
}

void CHARACTER::SendEquipment(LPCHARACTER ch)
{
	TPacketViewEquip p;
	p.header = HEADER_GC_VIEW_EQUIP;
	p.vid    = GetVID();
	for (int i = 0; i<WEAR_MAX_NUM; i++)
	{
		LPITEM item = GetWear(i);
		if (item)
		{
			p.equips[i].vnum = item->GetVnum();
			p.equips[i].count = item->GetCount();

			thecore_memcpy(p.equips[i].alSockets, item->GetSockets(), sizeof(p.equips[i].alSockets));
			thecore_memcpy(p.equips[i].aAttr, item->GetAttributes(), sizeof(p.equips[i].aAttr));
		}
		else
		{
			p.equips[i].vnum = 0;
		}
	}
	ch->GetDesc()->Packet(&p, sizeof(p));
}

bool CHARACTER::CanSummon(int iLeaderShip)
{
	return (iLeaderShip >= 20 || iLeaderShip >= 12 && m_dwLastDeadTime + 180 > get_dword_time());
}


void CHARACTER::AlignmentBonus()
{
	
	bool bos = false;

#ifdef MERLIS
		bos = false;
#else
		if (FindAffect(ALIGNMENT_BONUS_1_1))
		{
			if (GetRealAlignment() >= 120000 && GetRealAlignment() < 200000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_1_1);
			}
		}
#endif
		if (FindAffect(ALIGNMENT_BONUS_1_2))
		{
			if (GetRealAlignment() >= 120000 && GetRealAlignment() < 200000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_1_2);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_1_3))
		{
			if (GetRealAlignment() >= 120000 && GetRealAlignment() < 200000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_1_3);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_1_4))
		{
			if (GetRealAlignment() >= 120000 && GetRealAlignment() < 200000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_1_4);
			}
		}
		
		////////////////////////////////////////////////

#ifdef MERLIS
		bos = false;
#else
		if (FindAffect(ALIGNMENT_BONUS_3_1))
		{
			if (GetRealAlignment() >= 200000 && GetRealAlignment() < 240000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_3_1);
			}
		}
#endif
		if (FindAffect(ALIGNMENT_BONUS_3_2))
		{
			if (GetRealAlignment() >= 200000 && GetRealAlignment() < 240000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_3_2);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_3_3))
		{
			if (GetRealAlignment() >= 200000 && GetRealAlignment() < 240000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_3_3);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_3_4))
		{
			if (GetRealAlignment() >= 200000 && GetRealAlignment() < 240000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_3_4);
			}
		}

		
		////////////////////////////////////////////////
		
#ifdef MERLIS
		bos = false;
#else	
		if (FindAffect(ALIGNMENT_BONUS_4_1))
		{
			if (GetRealAlignment() >= 240000 && GetRealAlignment() < 300000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_4_1);
			}
		}
#endif
		if (FindAffect(ALIGNMENT_BONUS_4_2))
		{
			if (GetRealAlignment() >= 240000 && GetRealAlignment() < 300000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_4_2);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_4_3))
		{
			if (GetRealAlignment() >= 240000 && GetRealAlignment() < 300000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_4_3);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_4_4))
		{
			if (GetRealAlignment() >= 240000 && GetRealAlignment() < 300000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_4_4);
			}
		}
		
		////////////////////////////////////////////
#ifdef MERLIS
		bos = false;
#else	
		if (FindAffect(ALIGNMENT_BONUS_5_1))
		{
			if (GetRealAlignment() >= 300000 && GetRealAlignment() < 500000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_5_1);
			}
		}
#endif
		if (FindAffect(ALIGNMENT_BONUS_5_2))
		{
			if (GetRealAlignment() >= 300000 && GetRealAlignment() < 500000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_5_2);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_5_3))
		{
			if (GetRealAlignment() >= 300000 && GetRealAlignment() < 500000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_5_3);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_5_4))
		{
			if (GetRealAlignment() >= 300000 && GetRealAlignment() < 500000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_5_4);
			}
		}
		
		////////////////////////////////////////////
#ifdef MERLIS
		bos = false;
#else	
		if (FindAffect(ALIGNMENT_BONUS_6_1))
		{
			if (GetRealAlignment() >= 500000 && GetRealAlignment() < 750000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_6_1);
			}
		}
#endif
		if (FindAffect(ALIGNMENT_BONUS_6_2))
		{
			if (GetRealAlignment() >= 500000 && GetRealAlignment() < 750000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_6_2);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_6_3))
		{
			if (GetRealAlignment() >= 500000 && GetRealAlignment() < 750000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_6_3);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_6_4))
		{
			if (GetRealAlignment() >= 500000 && GetRealAlignment() < 750000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_6_4);
			}
		}
		
		////////////////////////////////////////////
#ifdef MERLIS
		bos = false;
#else	
		if (FindAffect(ALIGNMENT_BONUS_7_1))
		{
			if (GetRealAlignment() >= 750000 && GetRealAlignment() < 1000000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_7_1);
			}
		}
#endif
		if (FindAffect(ALIGNMENT_BONUS_7_2))
		{
			if (GetRealAlignment() >= 750000 && GetRealAlignment() < 1000000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_7_2);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_7_3))
		{
			if (GetRealAlignment() >= 750000 && GetRealAlignment() < 1000000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_7_3);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_7_4))
		{
			if (GetRealAlignment() >= 750000 && GetRealAlignment() < 1000000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_7_4);
			}
		}
		
		////////////////////////////////////////////
#ifdef MERLIS
		bos = false;
#else	
		if (FindAffect(ALIGNMENT_BONUS_8_1))
		{
			if (GetRealAlignment() >= 1000000 && GetRealAlignment() < 1500000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_8_1);
			}
		}
#endif
		if (FindAffect(ALIGNMENT_BONUS_8_2))
		{
			if (GetRealAlignment() >= 1000000 && GetRealAlignment() < 1500000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_8_2);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_8_3))
		{
			if (GetRealAlignment() >= 1000000 && GetRealAlignment() < 1500000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_8_3);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_8_4))
		{
			if (GetRealAlignment() >= 1000000 && GetRealAlignment() < 1500000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_8_4);
			}
		}
		
		////////////////////////////////////////////
#ifdef MERLIS
		bos = false;
#else
		if (FindAffect(ALIGNMENT_BONUS_9_1))
		{
			if (GetRealAlignment() >= 1500000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_9_1);
			}
		}
#endif
		if (FindAffect(ALIGNMENT_BONUS_9_2))
		{
			if (GetRealAlignment() >= 1500000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_9_2);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_9_3))
		{
			if (GetRealAlignment() >= 1500000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_9_3);
			}
		}
		if (FindAffect(ALIGNMENT_BONUS_9_4))
		{
			if (GetRealAlignment() >= 1500000)
			{ 
			}
			else
			{
				RemoveAffect(ALIGNMENT_BONUS_9_4);
			}
		}
		
		//////////////////////////////////////////////

		
		
		if (GetRealAlignment() >= 120000 && GetRealAlignment() < 200000)
		{
#ifdef MERLIS
			bos = false;
#else
			if (!FindAffect(ALIGNMENT_BONUS_1_1))
			{
				AddAffect(ALIGNMENT_BONUS_1_1, POINT_MAX_HP, 1000, 0, 60*60*24*365, 0, true);
			}
#endif
			if (!FindAffect(ALIGNMENT_BONUS_1_2))
			{
				AddAffect(ALIGNMENT_BONUS_1_2, POINT_MELEE_MAGIC_ATT_BONUS_PER, 5, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_1_3))
			{
				AddAffect(ALIGNMENT_BONUS_1_3, POINT_ATT_BOSS, 10, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_1_4))
			{
				AddAffect(ALIGNMENT_BONUS_1_4, POINT_ATT_METIN, 10, 0, 60*60*24*365, 0, true);
			}
		}
		else if (GetRealAlignment() >= 200000 && GetRealAlignment() < 240000)
		{
			
#ifdef MERLIS
			bos = false;
#else
			if (!FindAffect(ALIGNMENT_BONUS_3_1))
			{
				AddAffect(ALIGNMENT_BONUS_3_1, POINT_MAX_HP, 2000, 0, 60*60*24*365, 0, true);
			}
#endif
			if (!FindAffect(ALIGNMENT_BONUS_3_2))
			{
				AddAffect(ALIGNMENT_BONUS_3_2, POINT_MELEE_MAGIC_ATT_BONUS_PER, 10, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_3_3))
			{
				AddAffect(ALIGNMENT_BONUS_3_3, POINT_ATT_BOSS, 20, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_3_4))
			{
				AddAffect(ALIGNMENT_BONUS_3_4, POINT_ATT_METIN, 20, 0, 60*60*24*365, 0, true);
			}
		}
		else if (GetRealAlignment() >= 240000 && GetRealAlignment() < 300000)
		{
#ifdef MERLIS
			bos = false;
#else
			if (!FindAffect(ALIGNMENT_BONUS_4_1))
			{
				AddAffect(ALIGNMENT_BONUS_4_1, POINT_MAX_HP, 3000, 0, 60*60*24*365, 0, true);
			}
#endif
			if (!FindAffect(ALIGNMENT_BONUS_4_2))
			{
				AddAffect(ALIGNMENT_BONUS_4_2, POINT_MELEE_MAGIC_ATT_BONUS_PER, 15, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_4_3))
			{
				AddAffect(ALIGNMENT_BONUS_4_3, POINT_ATT_BOSS, 30, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_4_4))
			{
				AddAffect(ALIGNMENT_BONUS_4_4, POINT_ATT_METIN, 30, 0, 60*60*24*365, 0, true);
			}
		}
		else if (GetRealAlignment() >= 300000 && GetRealAlignment() < 500000)
		{
#ifdef MERLIS
			bos = false;
#else
			if (!FindAffect(ALIGNMENT_BONUS_5_1))
			{
				AddAffect(ALIGNMENT_BONUS_5_1, POINT_MAX_HP, 4000, 0, 60*60*24*365, 0, true);
			}
#endif
			if (!FindAffect(ALIGNMENT_BONUS_5_2))
			{
				AddAffect(ALIGNMENT_BONUS_5_2, POINT_MELEE_MAGIC_ATT_BONUS_PER, 20, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_5_3))
			{
				AddAffect(ALIGNMENT_BONUS_5_3, POINT_ATT_BOSS, 40, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_5_4))
			{
				AddAffect(ALIGNMENT_BONUS_5_4, POINT_ATT_METIN, 40, 0, 60*60*24*365, 0, true);
			}
		}

		else if (GetRealAlignment() >= 500000 && GetRealAlignment() < 750000)
		{
#ifdef MERLIS
			bos = false;
#else
			if (!FindAffect(ALIGNMENT_BONUS_6_1))
			{
				AddAffect(ALIGNMENT_BONUS_6_1, POINT_MAX_HP, 7500, 0, 60*60*24*365, 0, true);
			}
#endif
			if (!FindAffect(ALIGNMENT_BONUS_6_2))
			{
				AddAffect(ALIGNMENT_BONUS_6_2, POINT_MELEE_MAGIC_ATT_BONUS_PER, 25, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_6_3))
			{
				AddAffect(ALIGNMENT_BONUS_6_3, POINT_ATT_BOSS, 75, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_6_4))
			{
				AddAffect(ALIGNMENT_BONUS_6_4, POINT_ATT_METIN, 75, 0, 60*60*24*365, 0, true);
			}
		}


		else if (GetRealAlignment() >= 750000 && GetRealAlignment() < 1000000)
		{
#ifdef MERLIS
			bos = false;
#else
			if (!FindAffect(ALIGNMENT_BONUS_7_1))
			{
				AddAffect(ALIGNMENT_BONUS_7_1, POINT_MAX_HP, 10000, 0, 60*60*24*365, 0, true);
			}
#endif
			if (!FindAffect(ALIGNMENT_BONUS_7_2))
			{
				AddAffect(ALIGNMENT_BONUS_7_2, POINT_MELEE_MAGIC_ATT_BONUS_PER, 30, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_7_3))
			{
				AddAffect(ALIGNMENT_BONUS_7_3, POINT_ATT_BOSS, 120, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_7_4))
			{
				AddAffect(ALIGNMENT_BONUS_7_4, POINT_ATT_METIN, 120, 0, 60*60*24*365, 0, true);
			}
		}


		else if (GetRealAlignment() >= 1000000 && GetRealAlignment() < 1500000)
		{
#ifdef MERLIS
			bos = false;
#else
			if (!FindAffect(ALIGNMENT_BONUS_8_1))
			{
				AddAffect(ALIGNMENT_BONUS_8_1, POINT_MAX_HP, 15000, 0, 60*60*24*365, 0, true);
			}
#endif
			if (!FindAffect(ALIGNMENT_BONUS_8_2))
			{
				AddAffect(ALIGNMENT_BONUS_8_2, POINT_MELEE_MAGIC_ATT_BONUS_PER, 35, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_8_3))
			{
				AddAffect(ALIGNMENT_BONUS_8_3, POINT_ATT_BOSS, 175, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_8_4))
			{
				AddAffect(ALIGNMENT_BONUS_8_4, POINT_ATT_METIN, 175, 0, 60*60*24*365, 0, true);
			}
		}

		else if (GetRealAlignment() >= 1500000)
		{
#ifdef MERLIS
			bos = false;
#else
			if (!FindAffect(ALIGNMENT_BONUS_9_1))
			{
				AddAffect(ALIGNMENT_BONUS_9_1, POINT_MAX_HP, 20000, 0, 60*60*24*365, 0, true);
			}
#endif
			if (!FindAffect(ALIGNMENT_BONUS_9_2))
			{
				AddAffect(ALIGNMENT_BONUS_9_2, POINT_MELEE_MAGIC_ATT_BONUS_PER, 40, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_9_3))
			{
				AddAffect(ALIGNMENT_BONUS_9_3, POINT_ATT_BOSS, 250, 0, 60*60*24*365, 0, true);
			}
			if (!FindAffect(ALIGNMENT_BONUS_9_4))
			{
				AddAffect(ALIGNMENT_BONUS_9_4, POINT_ATT_METIN, 250, 0, 60*60*24*365, 0, true);
			}
		}
}

void CHARACTER::MountVnum(DWORD vnum)
{
	if (m_dwMountVnum == vnum)
		return;

	m_dwMountVnum = vnum;
	m_dwMountTime = get_dword_time();

	if (m_bIsObserver)
		return;

	//NOTE : Mount�Ѵٰ� �ؼ� Client Side�� ��ü�� �������� �ʴ´�.
	//�׸��� ����Side���� ������ ��ġ �̵��� ���� �ʴ´�. �ֳ��ϸ� Client Side���� Coliision Adjust�� �Ҽ� �ִµ�
	//��ü�� �Ҹ���״ٰ� ������ġ�� �̵���Ű�� �̶� collision check�� ������ �����Ƿ� ��濡 ���ų� �հ� ������ ������ �����Ѵ�.
	m_posDest.x = m_posStart.x = GetX();
	m_posDest.y = m_posStart.y = GetY();
	//EncodeRemovePacket(this);
	EncodeInsertPacket(this);

	ENTITY_MAP::iterator it = m_map_view.begin();

	while (it != m_map_view.end())
	{
		LPENTITY entity = (it++)->first;

		//Mount�Ѵٰ� �ؼ� Client Side�� ��ü�� �������� �ʴ´�.
		//EncodeRemovePacket(entity);
		//if (!m_bIsObserver)
		EncodeInsertPacket(entity);

		//if (!entity->IsObserverMode())
		//	entity->EncodeInsertPacket(this);
	}

	SetValidComboInterval(0);
	SetComboSequence(0);

	ComputePoints();
}

namespace {
	class FuncCheckWarp
	{
		public:
			FuncCheckWarp(LPCHARACTER pkWarp)
			{
				m_lTargetY = 0;
				m_lTargetX = 0;

				m_lX = pkWarp->GetX();
				m_lY = pkWarp->GetY();

				m_bInvalid = false;
				m_bEmpire = pkWarp->GetEmpire();

				char szTmp[64];

				if (3 != sscanf(pkWarp->GetName(), " %s %ld %ld ", szTmp, &m_lTargetX, &m_lTargetY))
				{
					if (number(1, 100) < 5)
						sys_err("Warp NPC name wrong : vnum(%d) name(%s)", pkWarp->GetRaceNum(), pkWarp->GetName());

					m_bInvalid = true;

					return;
				}

				m_lTargetX *= 100;
				m_lTargetY *= 100;

				m_bUseWarp = true;

				if (pkWarp->IsGoto())
				{
					LPSECTREE_MAP pkSectreeMap = SECTREE_MANAGER::instance().GetMap(pkWarp->GetMapIndex());
					m_lTargetX += pkSectreeMap->m_setting.iBaseX;
					m_lTargetY += pkSectreeMap->m_setting.iBaseY;
					m_bUseWarp = false;
				}
			}

			bool Valid()
			{
				return !m_bInvalid;
			}

			void operator () (LPENTITY ent)
			{
				if (!Valid())
					return;

				if (!ent->IsType(ENTITY_CHARACTER))
					return;

				LPCHARACTER pkChr = (LPCHARACTER) ent;

				if (!pkChr->IsPC())
					return;

				int iDist = DISTANCE_APPROX(pkChr->GetX() - m_lX, pkChr->GetY() - m_lY);

				if (iDist > 300)
					return;

				if (m_bEmpire && pkChr->GetEmpire() && m_bEmpire != pkChr->GetEmpire())
					return;

				if (pkChr->IsHack())
					return;

				if (!pkChr->CanHandleItem(false, true))
					return;	
				
				if (m_bUseWarp)
					pkChr->WarpSet(m_lTargetX, m_lTargetY);
				else
				{
					pkChr->Show(pkChr->GetMapIndex(), m_lTargetX, m_lTargetY);
					pkChr->Stop();
				}
			}

			bool m_bInvalid;
			bool m_bUseWarp;

			long m_lX;
			long m_lY;
			long m_lTargetX;
			long m_lTargetY;

			BYTE m_bEmpire;
	};
}

EVENTFUNC(warp_npc_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info );
	if ( info == NULL )
	{
		sys_err( "warp_npc_event> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (ch == NULL) { // <Factor>
		return 0;
	}	

	if (!ch->GetSectree())
	{
		ch->m_pkWarpNPCEvent = NULL;
		return 0;
	}

	FuncCheckWarp f(ch);
	if (f.Valid())
		ch->GetSectree()->ForEachAround(f);

	return passes_per_sec / 2;
}


void CHARACTER::StartWarpNPCEvent()
{
	if (m_pkWarpNPCEvent)
		return;

	if (!IsWarp() && !IsGoto())
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;

	m_pkWarpNPCEvent = event_create(warp_npc_event, info, passes_per_sec / 2);
}

void CHARACTER::SyncPacket()
{
	TEMP_BUFFER buf;

	TPacketCGSyncPositionElement elem;

	elem.dwVID = GetVID();
	elem.lX = GetX();
	elem.lY = GetY();

	TPacketGCSyncPosition pack;

	pack.bHeader = HEADER_GC_SYNC_POSITION;
	pack.wSize = sizeof(TPacketGCSyncPosition) + sizeof(elem);

	buf.write(&pack, sizeof(pack));
	buf.write(&elem, sizeof(elem));

	PacketAround(buf.read_peek(), buf.size());
}

LPCHARACTER CHARACTER::GetMarryPartner() const
{
	return m_pkChrMarried;
}
#ifdef WJ_SECURITY_SYSTEM
const char * CHARACTER::GetSecurityPassword()
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT security_password FROM account.account WHERE id = '%d';", GetDesc()->GetAccountTable().id));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	return row[0];
}

void CHARACTER::ActivateSecurity()
{
	m_bActivateSecurity = true;
}

void CHARACTER::DeactivateSecurity()
{
	m_bActivateSecurity = false;
}

void CHARACTER::SetSecurityPassword(const char * passwd)
{
	DBManager::instance().DirectQuery("UPDATE account.account SET security_password = '%s' WHERE id = '%d';", passwd, GetDesc()->GetAccountTable().id);
}
#endif

#ifdef ENABLE_AURA_SYSTEM
void CHARACTER::OpenAura(bool bCombination)
{
	if (isAuraOpened(bCombination))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The aura window it's already opened."));
		return;
	}

	if (bCombination)
	{
		if (m_bAuraAbsorption)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Before you may close the aura absorption window."));
			return;
		}

		m_bAuraRefine = true;
	}
	else
	{
		if (m_bAuraRefine)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Before you may close the aura combine window."));
			return;
		}

		m_bAuraAbsorption = true;
	}

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketAura sPacket;
	sPacket.header = HEADER_GC_AURA;
	sPacket.subheader = AURA_SUBHEADER_GC_OPEN;
	sPacket.bWindow = bCombination;
	sPacket.dwPrice = 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	GetDesc()->Packet(&sPacket, sizeof(TPacketAura));

	ClearAuraMaterials();
}

void CHARACTER::CloseAura()
{
	if ((!m_bAuraRefine) && (!m_bAuraAbsorption))
		return;

	bool bWindow = (m_bAuraRefine == true ? true : false);

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketAura sPacket;
	sPacket.header = HEADER_GC_AURA;
	sPacket.subheader = AURA_SUBHEADER_GC_CLOSE;
	sPacket.bWindow = bWindow;
	sPacket.dwPrice = 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	GetDesc()->Packet(&sPacket, sizeof(TPacketAura));

	if (bWindow)
		m_bAuraRefine = false;
	else
		m_bAuraAbsorption = false;

	ClearAuraMaterials();
}

void CHARACTER::ClearAuraMaterials()
{
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetAuraMaterials();
	for (int i = 0; i < AURA_WINDOW_MAX_MATERIALS; ++i)
	{
		if (!pkItemMaterial[i])
			continue;

		pkItemMaterial[i]->Lock(false);
		pkItemMaterial[i] = NULL;
	}
}

bool CHARACTER::AuraIsSameGrade(long lGrade)
{
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetAuraMaterials();
	if (!pkItemMaterial[0])
		return false;

	bool bReturn = (pkItemMaterial[0]->GetValue(AURA_GRADE_VALUE_FIELD) == lGrade ? true : false);
	return bReturn;
}

DWORD CHARACTER::GetAuraCombinePrice(long lGrade)
{
	DWORD dwPrice = AURA_REFINE_PRICE;
	return dwPrice;
}

BYTE CHARACTER::CheckAuraEmptyMaterialSlot()
{
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetAuraMaterials();
	for (int i = 0; i < AURA_WINDOW_MAX_MATERIALS; ++i)
	{
		if (!pkItemMaterial[i])
			return i;
	}

	return 255;
}

void CHARACTER::GetAuraCombineResult(DWORD & dwItemVnum, DWORD & dwMinAbs, DWORD & dwMaxAbs)
{
	LPITEM* pkItemMaterial;
	pkItemMaterial = GetAuraMaterials();

	if (m_bAuraRefine)
	{
		if ((pkItemMaterial[0]) && (pkItemMaterial[1]))
		{
			long lVal = pkItemMaterial[0]->GetValue(AURA_TYPE_VALUE_FIELD);
			if (lVal == 6)
			{
				dwItemVnum = pkItemMaterial[0]->GetOriginalVnum();
			}
			else
			{
				DWORD dwMaskVnum = pkItemMaterial[0]->GetOriginalVnum();
				TItemTable* pTable = ITEM_MANAGER::instance().GetTable(dwMaskVnum + 1);
				if (pTable)
					dwMaskVnum += 1;

				dwItemVnum = dwMaskVnum;
			}
		}
		else
		{
			dwItemVnum = 0;
			dwMinAbs = 0;
			dwMaxAbs = 0;
		}
	}
	else
	{
		if ((pkItemMaterial[0]) && (pkItemMaterial[1]))
		{
			dwItemVnum = pkItemMaterial[0]->GetOriginalVnum();
			dwMinAbs = pkItemMaterial[0]->GetSocket(AURA_ABSORPTION_SOCKET);
			dwMaxAbs = dwMinAbs;
		}
		else
		{
			dwItemVnum = 0;
			dwMinAbs = 0;
			dwMaxAbs = 0;
		}
	}
}

void CHARACTER::AddAuraMaterial(TItemPos tPos, BYTE bPos)
{
	if (bPos >= AURA_WINDOW_MAX_MATERIALS)
	{
		if (bPos == 255)
		{
			bPos = CheckEmptyMaterialSlot();
			if (bPos >= AURA_WINDOW_MAX_MATERIALS)
				return;
		}
		else
			return;
	}

	LPITEM pkItem = GetItem(tPos);
	if (!pkItem)
		return;
	else if ((pkItem->GetCell() >= INVENTORY_MAX_NUM) || (pkItem->IsEquipped()) || (tPos.IsBeltInventoryPosition()) || (pkItem->IsDragonSoul()))
		return;
	else if ((pkItem->GetType() != ITEM_COSTUME && pkItem->GetType() != COSTUME_AURA) && (bPos == 0) && (m_bAuraRefine))
		return;
	else if ((pkItem->GetType() != ITEM_COSTUME && pkItem->GetType() != COSTUME_AURA) && (bPos == 0) && (m_bAuraAbsorption))
		return;
	else if (pkItem->isLocked())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't add locked items."));
		return;
	}
#ifdef ENABLE_ITEM_SEALBIND_SYSTEM
	else if ((pkItem->IsSealed()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Aura] You can't add binded items."));
		return;
	}
#endif
	// else if (pkItem->IsBasicItem())
	// {
		// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
		// return;
	// }
	else if ((m_bAuraRefine) && (pkItem->GetSocket(AURA_ABSORPTION_SOCKET) >= AURA_MAX_ABS))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This aura got already maximum absorption chance."));
		return;
	}
	else if ((bPos == 1) && (m_bAuraAbsorption))
	{
		if ((pkItem->GetType() != ITEM_ARMOR))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura You can absorb just the bonuses from armors and weapons."));
			return;
		}
		else if ((pkItem->GetType() == ITEM_ARMOR) && (pkItem->GetSubType() != ARMOR_SHIELD) && (pkItem->GetSubType() != ARMOR_EAR) && (pkItem->GetSubType() != ARMOR_WRIST) && (pkItem->GetSubType() != ARMOR_NECK))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura You can absorb just the bonuses from armors and weapons."));
			return;
		}
	}
	else if ((bPos == 1) && (m_bAuraRefine))
	{
		if ((pkItem->GetVnum() != AURA_ICE_RUNIC))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura Only Refine Material Ice Runic"));
			return;
		}
	}
	else if ((pkItem->GetSubType() != COSTUME_AURA) && (m_bAuraAbsorption))
		return;
	else if ((pkItem->GetSubType() != COSTUME_AURA) && (bPos == 0) && (m_bAuraAbsorption))
		return;
	else if ((pkItem->GetSocket(AURA_ABSORBED_SOCKET) > 0) && (bPos == 0) && (m_bAuraAbsorption))
		return;

	LPITEM * pkItemMaterial;
	pkItemMaterial = GetAuraMaterials();
	if ((bPos == 1) && (!pkItemMaterial[0]))
		return;

	if (pkItemMaterial[bPos])
		return;

	pkItemMaterial[bPos] = pkItem;
	pkItemMaterial[bPos]->Lock(true);

	DWORD dwItemVnum, dwMinAbs, dwMaxAbs;
	GetAuraCombineResult(dwItemVnum, dwMinAbs, dwMaxAbs);

	TPacketAura sPacket;
	sPacket.header = HEADER_GC_AURA;
	sPacket.subheader = AURA_SUBHEADER_GC_ADDED;
	sPacket.bWindow = m_bAuraRefine == true ? true : false;
	sPacket.dwPrice = GetAuraCombinePrice(pkItem->GetValue(AURA_GRADE_VALUE_FIELD));
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = dwItemVnum;
	sPacket.dwMinAbs = dwMinAbs;
	sPacket.dwMaxAbs = dwMaxAbs;
	GetDesc()->Packet(&sPacket, sizeof(TPacketAura));
}

void CHARACTER::RemoveAuraMaterial(BYTE bPos)
{
	if (bPos >= AURA_WINDOW_MAX_MATERIALS)
		return;

	LPITEM * pkItemMaterial;
	pkItemMaterial = GetAuraMaterials();

	DWORD dwPrice = 0;

	if (bPos == 1)
	{
		if (pkItemMaterial[bPos])
		{
			pkItemMaterial[bPos]->Lock(false);
			pkItemMaterial[bPos] = NULL;
		}

		if (pkItemMaterial[0])
			dwPrice = GetAuraCombinePrice(pkItemMaterial[0]->GetValue(AURA_GRADE_VALUE_FIELD));
	}
	else
		ClearAuraMaterials();

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketAura sPacket;
	sPacket.header = HEADER_GC_AURA;
	sPacket.subheader = AURA_SUBHEADER_GC_REMOVED;
	sPacket.bWindow = m_bAuraRefine == true ? true : false;
	sPacket.dwPrice = dwPrice;
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	GetDesc()->Packet(&sPacket, sizeof(TPacketAura));
}

BYTE CHARACTER::CanRefineAuraMaterials()
{
	BYTE bReturn = 0;
	LPITEM* pkItemMaterial = GetAuraMaterials();
	if (m_bAuraRefine)
	{
		for (int i = 0; i < AURA_WINDOW_MAX_MATERIALS; ++i)
		{
			if (pkItemMaterial[i] != NULL)
			{
				if ((pkItemMaterial[i]->GetType() == ITEM_COSTUME) && (pkItemMaterial[i]->GetSubType() == COSTUME_AURA))
					bReturn = 1;
				else if ((pkItemMaterial[i]->GetVnum() == AURA_ICE_RUNIC))
					bReturn = 1;
				else
				{
					bReturn = 0;
					break;
				}
			}
			else
			{
				bReturn = 0;
				break;
			}
		}
	}
	else if (m_bAuraAbsorption)
	{
		if ((pkItemMaterial[0]) && (pkItemMaterial[1]))
		{
			if ((pkItemMaterial[0]->GetType() == ITEM_COSTUME) && (pkItemMaterial[0]->GetSubType() == COSTUME_AURA))
				bReturn = 2;
			else
				bReturn = 0;

			if ((pkItemMaterial[1]->GetType() == ITEM_ARMOR) && ((pkItemMaterial[1]->GetSubType() == ARMOR_EAR) || (pkItemMaterial[1]->GetSubType() == ARMOR_WRIST) || (pkItemMaterial[1]->GetSubType() == ARMOR_NECK) || (pkItemMaterial[1]->GetSubType() == ARMOR_SHIELD)))
				bReturn = 2;
			else
				bReturn = 0;

			if (pkItemMaterial[0]->GetSocket(AURA_ABSORBED_SOCKET) > 0)
				bReturn = 0;
		}
		else
			bReturn = 0;
	}

	return bReturn;
}

void CHARACTER::RefineAuraMaterials()
{
	BYTE bCan = CanRefineAuraMaterials();
	if (bCan == 0)
		return;

	LPITEM * pkItemMaterial;
	pkItemMaterial = GetAuraMaterials();

	if (!pkItemMaterial[0] || !pkItemMaterial[1])
		return;

	DWORD dwItemVnum, dwMinAbs, dwMaxAbs;
	GetAuraCombineResult(dwItemVnum, dwMinAbs, dwMaxAbs);
#ifdef YANG_LIMIT
	LDWORD dwPrice = static_cast<LDWORD>(AURA_REFINE_PRICE);
#else
	DWORD dwPrice = AURA_REFINE_PRICE;
#endif
	if (bCan == 1)
	{
		int iSuccessChance = 0;
		long lVal = pkItemMaterial[0]->GetValue(AURA_TYPE_VALUE_FIELD);
		switch (lVal)
		{
		case 2:
		{
			iSuccessChance = AURA_GRADE_2_REFINE;
		}
		break;
		case 3:
		{
			iSuccessChance = AURA_GRADE_3_REFINE;
		}
		break;
		case 4:
		{
			iSuccessChance = AURA_GRADE_4_REFINE;
		}
		break;
		case 5:
		{
			iSuccessChance = AURA_GRADE_5_REFINE;
		}
		break;
		case 6:
		{
			iSuccessChance = AURA_GRADE_6_REFINE;
		}
		break;
		default:
		{
			iSuccessChance = AURA_GRADE_1_REFINE;
		}
		break;
		}

		if (GetGold() < dwPrice)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura You don't have enough Yang."));
			return;
		}

		int iChance = number(1, 100);
		bool bSucces = (iChance <= iSuccessChance ? true : false);
		if (pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 49 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 99 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 149 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 199 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 249)
			bSucces = true;
		int oldGradeValue = pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD);
		bool bPacket = true;
		if (bSucces)
		{
			if (pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 49 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 99 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 149 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 199 && pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) != 249)
			{
#ifdef YANG_LIMIT
				GoldChange(static_cast<LDWORD>(-dwPrice),"Aura");
#else
				PointChange(POINT_GOLD, -dwPrice);
#endif	
				pkItemMaterial[0]->SetSocket(AURA_ABSORPTION_SOCKET, pkItemMaterial[0]->GetSocket(AURA_ABSORPTION_SOCKET)+1);
				pkItemMaterial[0]->SetSocket(AURA_GRADE_VALUE_FIELD, pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD)+1);
				bool bDelete = false;
				if (pkItemMaterial[1]->GetCount() - 1 < 1)
					bDelete = true;
				pkItemMaterial[1]->SetCount(pkItemMaterial[1]->GetCount()-1);
				if (bDelete == true)
					pkItemMaterial[1] = NULL;
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura Success."));
			}
			else
			{
				LPITEM pkItem = ITEM_MANAGER::instance().CreateItem(dwItemVnum, 1, 0, false);
				if (!pkItem)
				{
					sys_err("%d can't be created.", dwItemVnum);
					return;
				}

				ITEM_MANAGER::CopyAllAttrTo(pkItemMaterial[0], pkItem);
				LogManager::instance().ItemLog(this, pkItem, "AURA REFINE SUCCESS", pkItem->GetName());
				pkItem->SetSocket(AURA_ABSORPTION_SOCKET, pkItem->GetSocket(AURA_ABSORPTION_SOCKET)+1);
				pkItem->SetSocket(AURA_ABSORBED_SOCKET, pkItemMaterial[0]->GetSocket(AURA_ABSORBED_SOCKET));
				pkItem->SetSocket(AURA_GRADE_VALUE_FIELD, pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD)+1);

#ifdef YANG_LIMIT
				GoldChange(static_cast<LDWORD>(-dwPrice),"Aura");
#else
				PointChange(POINT_GOLD, -dwPrice);
#endif	
				DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, pkItemMaterial[0]->GetVnum(), -dwPrice);

				WORD wCell = pkItemMaterial[0]->GetCell();
				ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[0], "AURA (REFINE SUCCESS)");
				pkItemMaterial[1]->SetCount(pkItemMaterial[1]->GetCount()-1);

				pkItem->AddToCharacter(this, TItemPos(INVENTORY, wCell));
				ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
				pkItem->AttrLog();

				if (lVal == 6)
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura New absorption rate: %2f%"), float(pkItem->GetSocket(AURA_ABSORPTION_SOCKET)/10.0f));
				else
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura Success."));

				ClearAuraMaterials();
				bPacket = false;
			}
		}
		else
		{
#ifdef YANG_LIMIT
			GoldChange(static_cast<LDWORD>(-dwPrice),"Aura");
#else
			PointChange(POINT_GOLD, -dwPrice);
#endif	
			pkItemMaterial[1]->SetCount(pkItemMaterial[1]->GetCount()-1);
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, pkItemMaterial[0]->GetVnum(), -dwPrice);

			//ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[1], "COMBINE (REFINE FAIL)");

			if (lVal == 6)
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura New absorption rate: %d%"), pkItemMaterial[0]->GetSocket(AURA_ABSORPTION_SOCKET));
			else
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura Failed."));
			
			if (pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) == 50 || pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) == 100 || pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) == 150 || pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) == 200 || pkItemMaterial[0]->GetSocket(AURA_GRADE_VALUE_FIELD) == 250)
				ClearAuraMaterials();
			bPacket = false;
		}

		TItemPos tPos;
		tPos.window_type = INVENTORY;
		tPos.cell = 0;

		TPacketAura sPacket;
		sPacket.header = HEADER_GC_AURA;
		sPacket.subheader = AURA_SUBHEADER_GC_REFINED;
		sPacket.bWindow = m_bAuraRefine == true ? true : false;
		sPacket.dwPrice = dwPrice;
		if (pkItemMaterial[0])
		{
			if (bPacket)
				if (oldGradeValue != 49 && oldGradeValue != 99 && oldGradeValue != 149 && oldGradeValue != 199 && oldGradeValue != 249)
					sPacket.bPos = 255;
				else
					sPacket.bPos = 0;
			else
				sPacket.bPos = 0;
		}
		else
			sPacket.bPos = 0;
		sPacket.tPos = tPos;
		sPacket.dwItemVnum = 0;
		sPacket.dwMinAbs = 0;
		if (bSucces)
			sPacket.dwMaxAbs = 100;
		else
			sPacket.dwMaxAbs = 0;

		GetDesc()->Packet(&sPacket, sizeof(TPacketAura));
	}
	else
	{
		pkItemMaterial[1]->CopyAttributeTo(pkItemMaterial[0]);
		LogManager::instance().ItemLog(this, pkItemMaterial[0], "ABSORB (REFINE SUCCESS)", pkItemMaterial[0]->GetName());
		pkItemMaterial[0]->SetSocket(AURA_ABSORBED_SOCKET, pkItemMaterial[1]->GetOriginalVnum());
		for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		{
			if (pkItemMaterial[0]->GetAttributeValue(i) < 0)
				pkItemMaterial[0]->SetForceAttribute(i, pkItemMaterial[0]->GetAttributeType(i), 0);
		}

		ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[1], "ABSORBED (REFINE SUCCESS)");

		ITEM_MANAGER::instance().FlushDelayedSave(pkItemMaterial[0]);
		pkItemMaterial[0]->AttrLog();

		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Aura Success."));

		ClearAuraMaterials();

		TItemPos tPos;
		tPos.window_type = INVENTORY;
		tPos.cell = 0;

		TPacketAura sPacket;
		sPacket.header = HEADER_GC_AURA;
		sPacket.subheader = AURA_SUBHEADER_GC_REFINED;
		sPacket.bWindow = m_bAuraRefine == true ? true : false;
		sPacket.dwPrice = dwPrice;
		sPacket.bPos = 255;
		sPacket.tPos = tPos;
		sPacket.dwItemVnum = 0;
		sPacket.dwMinAbs = 0;
		sPacket.dwMaxAbs = 1;
		GetDesc()->Packet(&sPacket, sizeof(TPacketAura));
	}
}

bool CHARACTER::CleanAuraAttr(LPITEM pkItem, LPITEM pkTarget)
{
	if (!CanHandleItem())
		return false;
	else if ((!pkItem) || (!pkTarget))
		return false;
	else if ((pkTarget->GetType() != ITEM_COSTUME) && (pkTarget->GetSubType() != COSTUME_AURA))
		return false;

	if (pkTarget->GetSocket(AURA_ABSORBED_SOCKET) <= 0)
		return false;
	
	pkTarget->SetSocket(AURA_ABSORBED_SOCKET, 0);
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		pkTarget->SetForceAttribute(i, 0, 0);

	pkItem->SetCount(pkItem->GetCount() - 1);
	LogManager::instance().ItemLog(this, pkTarget, "USE_DETACHMENT (CLEAN ATTR)", pkTarget->GetName());
	return true;
}
#endif

void CHARACTER::SetMarryPartner(LPCHARACTER ch)
{
	m_pkChrMarried = ch;
}

int CHARACTER::GetMarriageBonus(DWORD dwItemVnum, bool bSum)
{
	if (IsNPC())
		return 0;

	marriage::TMarriage* pMarriage = marriage::CManager::instance().Get(GetPlayerID());

	if (!pMarriage)
		return 0;

	return pMarriage->GetBonus(dwItemVnum, bSum, this);
}

void CHARACTER::ConfirmWithMsg(const char* szMsg, int iTimeout, DWORD dwRequestPID)
{
	if (!IsPC())
		return;

	TPacketGCQuestConfirm p;

	p.header = HEADER_GC_QUEST_CONFIRM;
	p.requestPID = dwRequestPID;
	p.timeout = iTimeout;
	strlcpy(p.msg, szMsg, sizeof(p.msg));

	GetDesc()->Packet(&p, sizeof(p));
}

int CHARACTER::GetPremiumRemainSeconds(BYTE bType) const
{
	if (bType >= PREMIUM_MAX_NUM)
		return 0;

	return m_aiPremiumTimes[bType] - get_global_time();
}

bool CHARACTER::WarpToPID(DWORD dwPID)
{
	LPCHARACTER victim;
	if ((victim = (CHARACTER_MANAGER::instance().FindByPID(dwPID))))
	{
		int mapIdx = victim->GetMapIndex();
		if (IS_SUMMONABLE_ZONE(mapIdx))
		{
			if (CAN_ENTER_ZONE(this, mapIdx))
			{
				WarpSet(victim->GetX(), victim->GetY());
			}
			else
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �ִ� ������ ������ �� �����ϴ�."));
				return false;
			}
		}
		else
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �ִ� ������ ������ �� �����ϴ�."));
			return false;
		}
	}
	else
	{
		// �ٸ� ������ �α��ε� ����� ���� -> �޽��� ���� ��ǥ�� �޾ƿ���
		// 1. A.pid, B.pid �� �Ѹ�
		// 2. B.pid�� ���� ������ �Ѹ��������� A.pid, ��ǥ �� ����
		// 3. ����
		CCI * pcci = P2P_MANAGER::instance().FindByPID(dwPID);

		if (!pcci)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �¶��� ���°� �ƴմϴ�."));
			return false;
		}

		if (pcci->bChannel != g_bChannel)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ %d ä�ο� �ֽ��ϴ�. (���� ä�� %d)"), pcci->bChannel, g_bChannel);
			return false;
		}
		else if (false == IS_SUMMONABLE_ZONE(pcci->lMapIndex))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �ִ� ������ ������ �� �����ϴ�."));
			return false;
		}
		else
		{
			if (!CAN_ENTER_ZONE(this, pcci->lMapIndex))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �ִ� ������ ������ �� �����ϴ�."));
				return false;
			}

			TPacketGGFindPosition p;
			p.header = HEADER_GG_FIND_POSITION;
			p.dwFromPID = GetPlayerID();
			p.dwTargetPID = dwPID;
			pcci->pkDesc->Packet(&p, sizeof(TPacketGGFindPosition));

			if (test_server) 
				ChatPacket(CHAT_TYPE_PARTY, "sent find position packet for teleport");
		}
	}
	return true;
}

// ADD_REFINE_BUILDING
CGuild* CHARACTER::GetRefineGuild() const
{
	LPCHARACTER chRefineNPC = CHARACTER_MANAGER::instance().Find(m_dwRefineNPCVID);

	return (chRefineNPC ? chRefineNPC->GetGuild() : NULL);
}

bool CHARACTER::IsRefineThroughGuild() const
{
	return GetRefineGuild() != NULL;
}

int CHARACTER::ComputeRefineFee(int iCost, int iMultiply) const
{
	CGuild* pGuild = GetRefineGuild();
	if (pGuild)
	{
		if (pGuild == GetGuild())
			return iCost * iMultiply * 9 / 10;

		// �ٸ� ���� ����� �õ��ϴ� ��� �߰��� 3�� ��
		LPCHARACTER chRefineNPC = CHARACTER_MANAGER::instance().Find(m_dwRefineNPCVID);
		if (chRefineNPC && chRefineNPC->GetEmpire() != GetEmpire())
			return iCost * iMultiply * 3;

		return iCost * iMultiply;
	}
	else
		return iCost;
}

void CHARACTER::PayRefineFee(int iTotalMoney)
{
	int iFee = iTotalMoney / 10;
	CGuild* pGuild = GetRefineGuild();

	int iRemain = iTotalMoney;

	if (pGuild)
	{
		// �ڱ� ����̸� iTotalMoney�� �̹� 10%�� ���ܵǾ��ִ�
		if (pGuild != GetGuild())
		{
			pGuild->RequestDepositMoney(this, iFee);
			iRemain -= iFee;
		}
	}

#ifdef YANG_LIMIT
	GoldChange(static_cast<long long>(-iRemain),"RefineFee");
#else
	PointChange(POINT_GOLD, -iRemain);
#endif
}
// END_OF_ADD_REFINE_BUILDING

//Hack ������ ���� üũ.
bool CHARACTER::IsHack(bool bSendMsg, bool bCheckShopOwner, int limittime)
{
	const int iPulse = thecore_pulse();

	if (test_server)
		bSendMsg = true;

	//â�� ���� üũ
	if (iPulse - GetSafeboxLoadTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("â�� ���� %d�� �̳����� �ٸ������� �̵��Ҽ� �����ϴ�."), limittime);

		if (test_server)
			ChatPacket(CHAT_TYPE_INFO, "[TestOnly]Pulse %d LoadTime %d PASS %d", iPulse, GetSafeboxLoadTime(), PASSES_PER_SEC(limittime));
		return true; 
	}

	//�ŷ����� â üũ
	if (bCheckShopOwner)
	{
		if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || GetOfflineShopOwner())
		{
			if (bSendMsg)
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�ŷ�â,â�� ���� �� ���¿����� �ٸ������� �̵�,���� �Ҽ� �����ϴ�"));

			return true;
		}
	}
	else
	{
		if (GetExchange() || GetMyShop() || IsOpenSafebox() || IsCubeOpen())
		{
			if (bSendMsg)
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�ŷ�â,â�� ���� �� ���¿����� �ٸ������� �̵�,���� �Ҽ� �����ϴ�"));

			return true;
		}
	}

#ifdef WJ_SECURITY_SYSTEM
	if (iPulse - IsActivateSecurity() == true)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GUVENLIK_AKTIF_HATA"));
		return true;
	}
#endif

	//PREVENT_PORTAL_AFTER_EXCHANGE
	//��ȯ �� �ð�üũ
	if (iPulse - GetExchangeTime()  < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�ŷ� �� %d�� �̳����� �ٸ��������� �̵� �� �� �����ϴ�."), limittime );
		return true;
	}
	//END_PREVENT_PORTAL_AFTER_EXCHANGE

	//PREVENT_ITEM_COPY
	if (iPulse - GetMyShopTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�ŷ� �� %d�� �̳����� �ٸ��������� �̵� �� �� �����ϴ�."), limittime);
		return true;
	}

	if (iPulse - GetRefineTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ������ %d�� �̳����� ��ȯ��,��ȯ���θ� ����� �� �����ϴ�."), limittime);
		return true; 
	}
	if (iPulse - GetMyOfflineShopTime() < PASSES_PER_SEC(limittime))
	{
		if (bSendMsg)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("??? ??? %d? ???? ???,?????? ??? ? ????."), limittime);
		return true; 
	}
	//END_PREVENT_ITEM_COPY

	return false;
}

BOOL CHARACTER::IsMonarch() const
{
	//MONARCH_LIMIT
	if (CMonarch::instance().IsMonarch(GetPlayerID(), GetEmpire()))
		return true;

	return false;

	//END_MONARCH_LIMIT
}
void CHARACTER::Say(const std::string & s)
{
	struct ::packet_script packet_script;

	packet_script.header = HEADER_GC_SCRIPT;
	packet_script.skin = 1;
	packet_script.src_size = s.size();
	packet_script.size = packet_script.src_size + sizeof(struct packet_script);
	
	TEMP_BUFFER buf;

	buf.write(&packet_script, sizeof(struct packet_script));
	buf.write(&s[0], s.size());

	if (IsPC())
	{
		GetDesc()->Packet(buf.read_peek(), buf.size());
	}
}

//
// Monarch
//
void CHARACTER::InitMC()
{
	for (int n = 0; n < MI_MAX; ++n)
	{
		m_dwMonarchCooltime[n] = thecore_pulse(); 
	}

	m_dwMonarchCooltimelimit[MI_HEAL] = PASSES_PER_SEC(MC_HEAL);
	m_dwMonarchCooltimelimit[MI_WARP] = PASSES_PER_SEC(MC_WARP);
	m_dwMonarchCooltimelimit[MI_TRANSFER] = PASSES_PER_SEC(MC_TRANSFER);
	m_dwMonarchCooltimelimit[MI_TAX] = PASSES_PER_SEC(MC_TAX);
	m_dwMonarchCooltimelimit[MI_SUMMON] = PASSES_PER_SEC(MC_SUMMON);

	m_dwMonarchCooltime[MI_HEAL] -= PASSES_PER_SEC(GetMCL(MI_HEAL));
	m_dwMonarchCooltime[MI_WARP] -= PASSES_PER_SEC(GetMCL(MI_WARP));
	m_dwMonarchCooltime[MI_TRANSFER] -= PASSES_PER_SEC(GetMCL(MI_TRANSFER));
	m_dwMonarchCooltime[MI_TAX] -= PASSES_PER_SEC(GetMCL(MI_TAX));
	m_dwMonarchCooltime[MI_SUMMON] -= PASSES_PER_SEC(GetMCL(MI_SUMMON));
}

DWORD CHARACTER::GetMC(enum MONARCH_INDEX e) const
{
	return m_dwMonarchCooltime[e];
}

void CHARACTER::SetMC(enum MONARCH_INDEX e)
{
	m_dwMonarchCooltime[e] = thecore_pulse();
}

bool CHARACTER::IsMCOK(enum MONARCH_INDEX e) const
{
	int iPulse = thecore_pulse();

	if ((iPulse -  GetMC(e)) <  GetMCL(e))
	{
		if (test_server)
			sys_log(0, " Pulse %d cooltime %d, limit %d", iPulse, GetMC(e), GetMCL(e));
		
		return false;
	}
	
	if (test_server)
		sys_log(0, " Pulse %d cooltime %d, limit %d", iPulse, GetMC(e), GetMCL(e));

	return true;
}

DWORD CHARACTER::GetMCL(enum MONARCH_INDEX e) const
{
	return m_dwMonarchCooltimelimit[e];
}

DWORD CHARACTER::GetMCLTime(enum MONARCH_INDEX e) const
{
	int iPulse = thecore_pulse();

	if (test_server)
		sys_log(0, " Pulse %d cooltime %d, limit %d", iPulse, GetMC(e), GetMCL(e));

	return  (GetMCL(e)) / passes_per_sec   -  (iPulse - GetMC(e)) / passes_per_sec;
}

bool CHARACTER::IsSiegeNPC() const
{
	return IsNPC() && (GetRaceNum() == 11000 || GetRaceNum() == 11002 || GetRaceNum() == 11004);
}

//------------------------------------------------
void CHARACTER::UpdateDepositPulse()
{
	m_deposit_pulse = thecore_pulse() + PASSES_PER_SEC(60*5);	// 5??
}

bool CHARACTER::CanDeposit() const
{
	return (m_deposit_pulse == 0 || (m_deposit_pulse < thecore_pulse()));
}
//------------------------------------------------

ESex GET_SEX(LPCHARACTER ch)
{
	switch (ch->GetRaceNum())
	{
		case MAIN_RACE_WARRIOR_M:
		case MAIN_RACE_SURA_M:
		case MAIN_RACE_ASSASSIN_M:
		case MAIN_RACE_SHAMAN_M:
		case MAIN_RACE_WOLFMAN_M:
			return SEX_MALE;

		case MAIN_RACE_ASSASSIN_W:
		case MAIN_RACE_SHAMAN_W:
		case MAIN_RACE_WARRIOR_W:
		case MAIN_RACE_SURA_W:
			return SEX_FEMALE;
	}

	/* default sex = male */
	return SEX_MALE;
}

int64_t CHARACTER::GetHPPct() const
{
	return (GetHP() * 100) / GetMaxHP();
}

bool CHARACTER::IsBerserk() const
{
	if (m_pkMobInst != NULL)
		return m_pkMobInst->m_IsBerserk;
	else
		return false;
}

void CHARACTER::SetBerserk(bool mode)
{
	if (m_pkMobInst != NULL)
		m_pkMobInst->m_IsBerserk = mode;
}

bool CHARACTER::IsGodSpeed() const
{
	if (m_pkMobInst != NULL)
	{
		return m_pkMobInst->m_IsGodSpeed;
	}
	else
	{
		return false;
	}
}

void CHARACTER::SetGodSpeed(bool mode)
{
	if (m_pkMobInst != NULL)
	{
		m_pkMobInst->m_IsGodSpeed = mode;

		if (mode == true)
		{
			SetPoint(POINT_ATT_SPEED, 250);
		}
		else
		{
			SetPoint(POINT_ATT_SPEED, m_pkMobData->m_table.sAttackSpeed);
		}
	}
}

bool CHARACTER::IsDeathBlow() const
{
	if (number(1, 100) <= m_pkMobData->m_table.bDeathBlowPoint)
	{
		return true;
	}
	else
	{
		return false;
	}
}

struct FFindReviver
{
	FFindReviver()
	{
		pChar = NULL;
		HasReviver = false;
	}
	
	void operator() (LPCHARACTER ch)
	{
		if (ch->IsMonster() != true)
		{
			return;
		}

		if (ch->IsReviver() == true && pChar != ch && ch->IsDead() != true)
		{
			if (number(1, 100) <= ch->GetMobTable().bRevivePoint)
			{
				HasReviver = true;
				pChar = ch;
			}
		}
	}

	LPCHARACTER pChar;
	bool HasReviver;
};

bool CHARACTER::HasReviverInParty() const
{
	LPPARTY party = GetParty();

	if (party != NULL)
	{
		if (party->GetMemberCount() == 1) return false;

		FFindReviver f;
		party->ForEachMemberPtr(f);
		return f.HasReviver;
	}

	return false;
}

bool CHARACTER::IsRevive() const
{
	if (m_pkMobInst != NULL)
	{
		return m_pkMobInst->m_IsRevive;
	}

	return false;
}

void CHARACTER::SetRevive(bool mode)
{
	if (m_pkMobInst != NULL)
	{
		m_pkMobInst->m_IsRevive = mode;
	}
}

#define IS_SPEED_HACK_PLAYER(ch) (ch->m_speed_hack_count > SPEEDHACK_LIMIT_COUNT)

EVENTFUNC(check_speedhack_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info );
	if ( info == NULL )
	{
		sys_err( "check_speedhack_event> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (NULL == ch || ch->IsNPC())
		return 0;

	if (IS_SPEED_HACK_PLAYER(ch))
	{
		// write hack log
		LogManager::instance().SpeedHackLog(ch->GetPlayerID(), ch->GetX(), ch->GetY(), ch->m_speed_hack_count);

		if (false == LC_IsEurope())
		{
			// close connection
			LPDESC desc = ch->GetDesc();

			if (desc)
			{
				DESC_MANAGER::instance().DestroyDesc(desc);
				return 0;
			}
		}
	}

	ch->m_speed_hack_count = 0;

	ch->ResetComboHackCount();
	return PASSES_PER_SEC(60);
}

void CHARACTER::StartCheckSpeedHackEvent()
{
	if (m_pkCheckSpeedHackEvent)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;

	m_pkCheckSpeedHackEvent = event_create(check_speedhack_event, info, PASSES_PER_SEC(60));	// 1??
}

void CHARACTER::GoHome()
{
	WarpSet(EMPIRE_START_X(GetEmpire()), EMPIRE_START_Y(GetEmpire()));
}

void CHARACTER::SendGuildName(CGuild* pGuild)
{
	if (NULL == pGuild) return;

	DESC	*desc = GetDesc();

	if (NULL == desc) return;
	if (m_known_guild.find(pGuild->GetID()) != m_known_guild.end()) return;

	m_known_guild.insert(pGuild->GetID());

	TPacketGCGuildName	pack;
	memset(&pack, 0x00, sizeof(pack));

	pack.header		= HEADER_GC_GUILD;
	pack.subheader	= GUILD_SUBHEADER_GC_GUILD_NAME;
	pack.size		= sizeof(TPacketGCGuildName);
	pack.guildID	= pGuild->GetID();
	memcpy(pack.guildName, pGuild->GetName(), GUILD_NAME_MAX_LEN);

	desc->Packet(&pack, sizeof(pack));
}

void CHARACTER::SendGuildName(DWORD dwGuildID)
{
	SendGuildName(CGuildManager::instance().FindGuild(dwGuildID));
}

EVENTFUNC(destroy_when_idle_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info );
	if ( info == NULL )
	{
		sys_err( "destroy_when_idle_event> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER ch = info->ch;
	if (ch == NULL) { // <Factor>
		return 0;
	}	

	if (ch->GetVictim())
	{
		return PASSES_PER_SEC(300);
	}

	sys_log(1, "DESTROY_WHEN_IDLE: %s", ch->GetName());

	ch->m_pkDestroyWhenIdleEvent = NULL;
	M2_DESTROY_CHARACTER(ch);
	return 0;
}

void CHARACTER::StartDestroyWhenIdleEvent()
{
	if (m_pkDestroyWhenIdleEvent)
		return;

	char_event_info* info = AllocEventInfo<char_event_info>();

	info->ch = this;

	m_pkDestroyWhenIdleEvent = event_create(destroy_when_idle_event, info, PASSES_PER_SEC(300));
}

void CHARACTER::SetComboSequence(BYTE seq)
{
	m_bComboSequence = seq;
}

BYTE CHARACTER::GetComboSequence() const
{
	return m_bComboSequence;
}

void CHARACTER::SetLastComboTime(DWORD time)
{
	m_dwLastComboTime = time;
}

DWORD CHARACTER::GetLastComboTime() const
{
	return m_dwLastComboTime;
}

void CHARACTER::SetValidComboInterval(int interval)
{
	m_iValidComboInterval = interval;
}

int CHARACTER::GetValidComboInterval() const
{
	return m_iValidComboInterval;
}

BYTE CHARACTER::GetComboIndex() const
{
	return m_bComboIndex;
}

void CHARACTER::IncreaseComboHackCount(int k)
{
	m_iComboHackCount += k;

	if (m_iComboHackCount >= 10)
	{
		if (GetJob() == JOB_WOLFMAN && m_iComboHackCount <= 20)
		{
			return;
		}

		if (GetDesc())
		{
			if (GetDesc()->DelayedDisconnect(number(2, 7)))
			{
				sys_log(0, "COMBO_HACK_DISCONNECT: %s count: %d", GetName(), m_iComboHackCount);
				LogManager::instance().HackLog("Combo", this);
			}
		}
	}
}

void CHARACTER::ResetComboHackCount()
{
	m_iComboHackCount = 0;
}

void CHARACTER::SkipComboAttackByTime(int interval)
{
	m_dwSkipComboAttackByTime = get_dword_time() + interval;
}

DWORD CHARACTER::GetSkipComboAttackByTime() const
{
	return m_dwSkipComboAttackByTime;
}

void CHARACTER::ResetChatCounter()
{
	m_bChatCounter = 0;
}

BYTE CHARACTER::IncreaseChatCounter()
{
	return ++m_bChatCounter;
}

BYTE CHARACTER::GetChatCounter() const
{
	return m_bChatCounter;
}

// ���̳� �ٸ����� Ÿ�� �ֳ�?
bool CHARACTER::IsRiding() const
{
	return IsHorseRiding() || GetMountVnum();
}

bool CHARACTER::CanWarp() const
{
	const int iPulse = thecore_pulse();
	const int limit_time = PASSES_PER_SEC(g_nPortalLimitTime);

	if ((iPulse - GetSafeboxLoadTime()) < limit_time)
		return false;

	if ((iPulse - GetExchangeTime()) < limit_time)
		return false;

	if ((iPulse - GetMyShopTime()) < limit_time)
		return false;

	if ((iPulse - GetRefineTime()) < limit_time)
		return false;
	if ((iPulse - GetMyOfflineShopTime()) < limit_time)
		return false;

	if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || GetOfflineShopOwner())
		return false;

	return true;
}

DWORD CHARACTER::GetNextExp() const
{
	if (PLAYER_EXP_TABLE_MAX < GetLevel())
		return 2500000000;
	else
		return exp_table[GetLevel()];
}

#ifdef ENABLE_NEW_PET_SYSTEM
DWORD CHARACTER::PetGetNextExp() const
{
	if (IsNewPet())
	{
		if (PLAYER_EXP_TABLE_MAX < GetLevel())
			return 2500000000;
		else
			return exppet_table[GetLevel()];
	}

	return 0;
}
#endif

#ifdef BINDSYSTEM_WITH_PASSWORD
const char * CHARACTER::GetBindPassword()
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT password FROM player.safebox WHERE account_id = '%d';", GetDesc()->GetAccountTable().id));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;
	
	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	return row[0];	
}

void CHARACTER::SetBindPassword(const char * passwd)
{
	DBManager::instance().DirectQuery("UPDATE account.account SET bindpw = '%s' WHERE id = '%d';", passwd, GetDesc()->GetAccountTable().id);
}
#endif

#ifdef ENABLE_REBIRT_SYSTEM
DWORD CHARACTER::GetNextRebirtExp() const
{
	if (REBIRT_MAX < GetRebirt())
		return 2100000000;
	else if (GetRebirt() == 0)
		return exp_rebirt_table[1];
	else
		return exp_rebirt_table[GetRebirt()];
}
#endif

int	CHARACTER::GetSkillPowerByLevel(int level, bool bMob) const
{
	return CTableBySkill::instance().GetSkillPowerByLevelFromType(GetJob(), GetSkillGroup(), MINMAX(0, level, SKILL_MAX_LEVEL), bMob); 
}

#ifdef __ENABLE_NEW_OFFLINESHOP__
void CHARACTER::SetShopSafebox(offlineshop::CShopSafebox* pk) 
{
	if(m_pkShopSafebox && pk==NULL)
		m_pkShopSafebox->SetOwner(NULL);

	else if(m_pkShopSafebox==NULL && pk)
		pk->SetOwner(this);

	m_pkShopSafebox  = pk;
}
#endif

void CHARACTER::SetOfflineShopSign(const char * c_szSign)
{
	char szSign[SHOP_SIGN_MAX_LEN + 1];
	strlcpy(szSign, c_szSign, sizeof(szSign));
	m_stOfflineShopSign = szSign;
}

void CHARACTER::DestroyOfflineShop()
{
	DBManager::instance().DirectQuery("DELETE FROM %soffline_shop_npc WHERE owner_id = %u", get_table_postfix(), GetOfflineShopRealOwner());
	LPOFFLINESHOP pkOfflineShop = GetOfflineShop();
	if (pkOfflineShop)
		pkOfflineShop->RemoveAllGuest();
	M2_DESTROY_CHARACTER(this);	
}

	struct FCountOfflineShop
	{
		int n;
		FCountOfflineShop() : n(0) {};
		void operator()(LPENTITY ent)
		{
			if (ent->IsType(ENTITY_CHARACTER))
			{
				LPCHARACTER ch = (LPCHARACTER) ent;
				if (ch && ch->IsOfflineShopNPC())
					n++;
			}
		}
	};

int CHARACTER::GetOfflineShopCount()
{
	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(GetMapIndex());
	if (pMap)
	{
		FCountOfflineShop f;
		pMap->for_each(f);
		return f.n;
	}
	return 0;
}
#ifdef __SASH_SYSTEM__
void CHARACTER::OpenSash(bool bCombination)
{
	if (isSashOpened(bCombination))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The sash window it's already opened."));
		return;
	}
	
	if (bCombination)
	{
		if (m_bSashAbsorption)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Before you may close the sash absorption window."));
			return;
		}
		
		m_bSashCombination = true;
	}
	else
	{
		if (m_bSashCombination)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Before you may close the sash combine window."));
			return;
		}
		
		m_bSashAbsorption = true;
	}
	
	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;
	
	TPacketSash sPacket;
	sPacket.header = HEADER_GC_SASH;
	sPacket.subheader = SASH_SUBHEADER_GC_OPEN;
	sPacket.bWindow = bCombination;
	sPacket.dwPrice = 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
	
	ClearSashMaterials();
}

void CHARACTER::CloseSash()
{
	if ((!m_bSashCombination) && (!m_bSashAbsorption))
		return;
	
	bool bWindow = (m_bSashCombination == true ? true : false);
	
	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;
	
	TPacketSash sPacket;
	sPacket.header = HEADER_GC_SASH;
	sPacket.subheader = SASH_SUBHEADER_GC_CLOSE;
	sPacket.bWindow = bWindow;
	sPacket.dwPrice = 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
	
	if (bWindow)
		m_bSashCombination = false;
	else
		m_bSashAbsorption = false;
	
	ClearSashMaterials();
}

void CHARACTER::ClearSashMaterials()
{
	LPITEM * pkItemMaterial;
	pkItemMaterial = GetSashMaterials();
	for (int i = 0; i < SASH_WINDOW_MAX_MATERIALS; ++i)
	{
		if (!pkItemMaterial[i])
			continue;
		
		pkItemMaterial[i]->Lock(false);
		pkItemMaterial[i] = NULL;
	}
}

bool CHARACTER::SashIsSameGrade(long lGrade)
{
	LPITEM * pkItemMaterial;
	pkItemMaterial = GetSashMaterials();
	if (!pkItemMaterial[0])
		return false;
	
	bool bReturn = (pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD) == lGrade ? true : false);
	return bReturn;
}

DWORD CHARACTER::GetSashCombinePrice(long lGrade)
{
	DWORD dwPrice = 0;
	switch (lGrade)
	{
		case 2:
			{
				dwPrice = SASH_GRADE_2_PRICE;
			}
			break;
		case 3:
			{
				dwPrice = SASH_GRADE_3_PRICE;
			}
			break;
		case 4:
			{
				dwPrice = SASH_GRADE_4_PRICE;
			}
			break;
		default:
			{
				dwPrice = SASH_GRADE_1_PRICE;
			}
			break;
	}
	
	return dwPrice;
}

BYTE CHARACTER::CheckEmptyMaterialSlot()
{
	LPITEM * pkItemMaterial;
	pkItemMaterial = GetSashMaterials();
	for (int i = 0; i < SASH_WINDOW_MAX_MATERIALS; ++i)
	{
		if (!pkItemMaterial[i])
			return i;
	}
	
	return 255;
}

void CHARACTER::GetSashCombineResult(DWORD & dwItemVnum, DWORD & dwMinAbs, DWORD & dwMaxAbs)
{
	LPITEM * pkItemMaterial;
	pkItemMaterial = GetSashMaterials();
	
	if (m_bSashCombination)
	{
		if ((pkItemMaterial[0]) && (pkItemMaterial[1]))
		{
			long lVal = pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD);
			if (lVal == 4)
			{
				dwItemVnum = pkItemMaterial[0]->GetOriginalVnum();
				dwMinAbs = pkItemMaterial[0]->GetSocket(SASH_ABSORPTION_SOCKET);
				DWORD dwMaxAbsCalc = (dwMinAbs + SASH_GRADE_4_ABS_RANGE > SASH_GRADE_4_ABS_MAX ? SASH_GRADE_4_ABS_MAX : (dwMinAbs + SASH_GRADE_4_ABS_RANGE));
				dwMaxAbs = dwMaxAbsCalc;
			}
			else
			{
				DWORD dwMaskVnum = pkItemMaterial[0]->GetOriginalVnum();
				TItemTable * pTable = ITEM_MANAGER::instance().GetTable(dwMaskVnum + 1);
				if (pTable)
					dwMaskVnum += 1;
				
				dwItemVnum = dwMaskVnum;
				switch (lVal)
				{
					case 2:
						{
							dwMinAbs = SASH_GRADE_3_ABS;
							dwMaxAbs = SASH_GRADE_3_ABS;
						}
						break;
					case 3:
						{
							dwMinAbs = SASH_GRADE_4_ABS_MIN;
							dwMaxAbs = SASH_GRADE_4_ABS_MAX_COMB;
						}
						break;
					default:
						{
							dwMinAbs = SASH_GRADE_2_ABS;
							dwMaxAbs = SASH_GRADE_2_ABS;
						}
						break;
				}
			}
		}
		else
		{
			dwItemVnum = 0;
			dwMinAbs = 0;
			dwMaxAbs = 0;
		}
	}
	else
	{
		if ((pkItemMaterial[0]) && (pkItemMaterial[1]))
		{
			dwItemVnum = pkItemMaterial[0]->GetOriginalVnum();
			dwMinAbs = pkItemMaterial[0]->GetSocket(SASH_ABSORPTION_SOCKET);
			dwMaxAbs = dwMinAbs;
		}
		else
		{
			dwItemVnum = 0;
			dwMinAbs = 0;
			dwMaxAbs = 0;
		}
	}
}

void CHARACTER::AddSashMaterial(TItemPos tPos, BYTE bPos)
{
	if (bPos >= SASH_WINDOW_MAX_MATERIALS)
	{
		if (bPos == 255)
		{
			bPos = CheckEmptyMaterialSlot();
			if (bPos >= SASH_WINDOW_MAX_MATERIALS)
				return;
		}
		else
			return;
	}
	
	LPITEM pkItem = GetItem(tPos);
	if (!pkItem)
		return;
	else if ((pkItem->GetCell() >= INVENTORY_MAX_NUM) || (pkItem->IsEquipped()) || (tPos.IsBeltInventoryPosition()) || (pkItem->IsDragonSoul()))
		return;
	else if ((pkItem->GetType() != ITEM_COSTUME) && (m_bSashCombination))
		return;
	else if ((pkItem->GetType() != ITEM_COSTUME) && (bPos == 0) && (m_bSashAbsorption))
		return;
	else if (pkItem->isLocked())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't add locked items."));
		return;
	}
	#ifdef __SOULBINDING_SYSTEM__
	else if ((pkItem->IsBind()) || (pkItem->IsUntilBind()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't add binded items."));
		return;
	}
	#endif
	else if ((m_bSashCombination) && (bPos == 1) && (!SashIsSameGrade(pkItem->GetValue(SASH_GRADE_VALUE_FIELD))))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can combine just sashes of same grade."));
		return;
	}
	else if ((m_bSashCombination) && (pkItem->GetSocket(SASH_ABSORPTION_SOCKET) >= SASH_GRADE_4_ABS_MAX))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This sash got already maximum absorption chance."));
		return;
	}
	else if ((bPos == 1) && (m_bSashAbsorption))
	{
		if ((pkItem->GetType() != ITEM_WEAPON) && (pkItem->GetType() != ITEM_ARMOR))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can absorb just the bonuses from armors and weapons."));
			return;
		}
		else if ((pkItem->GetType() == ITEM_ARMOR) && (pkItem->GetSubType() != ARMOR_BODY))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can absorb just the bonuses from armors and weapons."));
			return;
		}
	}
	else if ((pkItem->GetSubType() != COSTUME_SASH) && (m_bSashCombination))
		return;
	else if ((pkItem->GetSubType() != COSTUME_SASH) && (bPos == 0) && (m_bSashAbsorption))
		return;
	else if ((pkItem->GetSocket(SASH_ABSORBED_SOCKET) > 0) && (bPos == 0) && (m_bSashAbsorption))
		return;
	
	if (pkItem->GetEvolution() > 0)
	{
		pkItem->SetEvolution(pkItem->GetEvolution());
	}
	
	LPITEM * pkItemMaterial;
	pkItemMaterial = GetSashMaterials();
	if ((bPos == 1) && (!pkItemMaterial[0]))
		return;
	
	if (pkItemMaterial[bPos])
		return;
	
	pkItemMaterial[bPos] = pkItem;
	pkItemMaterial[bPos]->Lock(true);
	
	DWORD dwItemVnum, dwMinAbs, dwMaxAbs;
	GetSashCombineResult(dwItemVnum, dwMinAbs, dwMaxAbs);
	
	TPacketSash sPacket;
	sPacket.header = HEADER_GC_SASH;
	sPacket.subheader = SASH_SUBHEADER_GC_ADDED;
	sPacket.bWindow = m_bSashCombination == true ? true : false;
	sPacket.dwPrice = GetSashCombinePrice(pkItem->GetValue(SASH_GRADE_VALUE_FIELD));
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = dwItemVnum;
	sPacket.dwMinAbs = dwMinAbs;
	sPacket.dwMaxAbs = dwMaxAbs;
	GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
}

void CHARACTER::RemoveSashMaterial(BYTE bPos)
{
	if (bPos >= SASH_WINDOW_MAX_MATERIALS)
		return;
	
	LPITEM * pkItemMaterial;
	pkItemMaterial = GetSashMaterials();
	
	DWORD dwPrice = 0;
	
	if (bPos == 1)
	{
		if (pkItemMaterial[bPos])
		{
			pkItemMaterial[bPos]->Lock(false);
			pkItemMaterial[bPos] = NULL;
		}
		
		if (pkItemMaterial[0])
			dwPrice = GetSashCombinePrice(pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD));
	}
	else
		ClearSashMaterials();
	
	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;
	
	TPacketSash sPacket;
	sPacket.header = HEADER_GC_SASH;
	sPacket.subheader = SASH_SUBHEADER_GC_REMOVED;
	sPacket.bWindow = m_bSashCombination == true ? true : false;
	sPacket.dwPrice = dwPrice;
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
}

BYTE CHARACTER::CanRefineSashMaterials()
{
	BYTE bReturn = 0;
	LPITEM * pkItemMaterial;
	pkItemMaterial = GetSashMaterials();
	if (m_bSashCombination)
	{
		for (int i = 0; i < SASH_WINDOW_MAX_MATERIALS; ++i)
		{
			if (pkItemMaterial[i])
			{
				if ((pkItemMaterial[i]->GetType() == ITEM_COSTUME) && (pkItemMaterial[i]->GetSubType() == COSTUME_SASH))
					bReturn = 1;
				else
				{
					bReturn = 0;
					break;
				}
			}
			else
			{
				bReturn = 0;
				break;
			}
		}
	}
	else if (m_bSashAbsorption)
	{
		if ((pkItemMaterial[0]) && (pkItemMaterial[1]))
		{
			if ((pkItemMaterial[0]->GetType() == ITEM_COSTUME) && (pkItemMaterial[0]->GetSubType() == COSTUME_SASH))
				bReturn = 2;
			else
				bReturn = 0;
			
			if ((pkItemMaterial[1]->GetType() == ITEM_WEAPON) || ((pkItemMaterial[1]->GetType() == ITEM_ARMOR) && (pkItemMaterial[1]->GetSubType() == ARMOR_BODY)))
				bReturn = 2;
			else
				bReturn = 0;
			
			if (pkItemMaterial[0]->GetSocket(SASH_ABSORBED_SOCKET) > 0)
				bReturn = 0;
		}
		else
			bReturn = 0;
	}
	
	return bReturn;
}

void CHARACTER::RefineSashMaterials()
{
	BYTE bCan = CanRefineSashMaterials();
	if (bCan == 0)
		return;
	
	LPITEM * pkItemMaterial;
	pkItemMaterial = GetSashMaterials();
	
	DWORD dwItemVnum, dwMinAbs, dwMaxAbs;
	GetSashCombineResult(dwItemVnum, dwMinAbs, dwMaxAbs);
#ifdef YANG_LIMIT
	LDWORD dwPrice = GetSashCombinePrice(pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD));
#else
	DWORD dwPrice = GetSashCombinePrice(pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD));
#endif
	if (bCan == 1)
	{
		int iSuccessChance = 0;
		long lVal = pkItemMaterial[0]->GetValue(SASH_GRADE_VALUE_FIELD);
		switch (lVal)
		{
			case 2:
				{
					iSuccessChance = SASH_COMBINE_GRADE_2;
				}
				break;
			case 3:
				{
					iSuccessChance = SASH_COMBINE_GRADE_3;
				}
				break;
			case 4:
				{
					iSuccessChance = SASH_COMBINE_GRADE_4;
				}
				break;
			default:
				{
					iSuccessChance = SASH_COMBINE_GRADE_1;
				}
				break;
		}
		
#ifdef YANG_LIMIT
		if (GetGold() < static_cast<LDWORD>(dwPrice))
#else
		if (GetGold() < dwPrice)
#endif
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You don't have enough Yang."));
			return;
		}
		
		int iChance = number(1, 100);
		bool bSucces = (iChance <= iSuccessChance ? true : false);
		if (bSucces)
		{
			LPITEM pkItem = ITEM_MANAGER::instance().CreateItem(dwItemVnum, 1, 0, false);
			if (!pkItem)
			{
				sys_err("%d can't be created.", dwItemVnum);
				return;
			}
			
			ITEM_MANAGER::CopyAllAttrTo(pkItemMaterial[0], pkItem);
			LogManager::instance().ItemLog(this, pkItem, "COMBINE SUCCESS", pkItem->GetName());
			DWORD dwAbs = (dwMinAbs == dwMaxAbs ? dwMinAbs : number(dwMinAbs + 1, dwMaxAbs));
			pkItem->SetSocket(SASH_ABSORPTION_SOCKET, dwAbs);
			pkItem->SetSocket(SASH_ABSORBED_SOCKET, pkItemMaterial[0]->GetSocket(SASH_ABSORBED_SOCKET));
#ifdef YANG_LIMIT
			GoldChange(static_cast<LDWORD>(-dwPrice),"Sash");
#else
			PointChange(POINT_GOLD, -dwPrice);
#endif	
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, pkItemMaterial[0]->GetVnum(), -dwPrice);
			
			WORD wCell = pkItemMaterial[0]->GetCell();
			ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[0], "COMBINE (REFINE SUCCESS)");
			ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[1], "COMBINE (REFINE SUCCESS)");
			
			pkItem->AddToCharacter(this, TItemPos(INVENTORY, wCell));
			ITEM_MANAGER::instance().FlushDelayedSave(pkItem);
			pkItem->AttrLog();
			
			if (lVal == 4)
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("New absorption rate: %d%"), dwAbs);
			else
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Success."));
			
			EffectPacket(SE_EFFECT_SASH_SUCCEDED);
			LogManager::instance().SashLog(GetPlayerID(), GetX(), GetY(), dwItemVnum, pkItem->GetID(), 1, dwAbs, 1);
			
			ClearSashMaterials();
		}
		else
		{
#ifdef YANG_LIMIT
			GoldChange(static_cast<LDWORD>(-dwPrice),"Sash");
#else
			PointChange(POINT_GOLD, -dwPrice);
#endif	
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, pkItemMaterial[0]->GetVnum(), -dwPrice);
			
			ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[1], "COMBINE (REFINE FAIL)");
			
			if (lVal == 4)
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("New absorption rate: %d%"), pkItemMaterial[0]->GetSocket(SASH_ABSORPTION_SOCKET));
			else
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Failed."));
			
			LogManager::instance().SashLog(GetPlayerID(), GetX(), GetY(), dwItemVnum, 0, 0, 0, 0);
			
			pkItemMaterial[1] = NULL;
		}
		
		TItemPos tPos;
		tPos.window_type = INVENTORY;
		tPos.cell = 0;
		
		TPacketSash sPacket;
		sPacket.header = HEADER_GC_SASH;
		sPacket.subheader = SASH_SUBHEADER_CG_REFINED;
		sPacket.bWindow = m_bSashCombination == true ? true : false;
		sPacket.dwPrice = dwPrice;
		sPacket.bPos = 0;
		sPacket.tPos = tPos;
		sPacket.dwItemVnum = 0;
		sPacket.dwMinAbs = 0;
		if (bSucces)
			sPacket.dwMaxAbs = 100;
		else
			sPacket.dwMaxAbs = 0;
		
		GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
	}
	else
	{
		pkItemMaterial[1]->CopyAttributeTo(pkItemMaterial[0]);
		LogManager::instance().ItemLog(this, pkItemMaterial[0], "ABSORB (REFINE SUCCESS)", pkItemMaterial[0]->GetName());
		pkItemMaterial[0]->SetSocket(SASH_ABSORBED_SOCKET, pkItemMaterial[1]->GetOriginalVnum());
		for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		{
			if (pkItemMaterial[0]->GetAttributeValue(i) < 0)
				pkItemMaterial[0]->SetForceAttribute(i, pkItemMaterial[0]->GetAttributeType(i), 0);
		}
		
		ITEM_MANAGER::instance().RemoveItem(pkItemMaterial[1], "ABSORBED (REFINE SUCCESS)");
		
		ITEM_MANAGER::instance().FlushDelayedSave(pkItemMaterial[0]);
		pkItemMaterial[0]->AttrLog();
		
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Success."));
		
		ClearSashMaterials();
		
		TItemPos tPos;
		tPos.window_type = INVENTORY;
		tPos.cell = 0;
		
		TPacketSash sPacket;
		sPacket.header = HEADER_GC_SASH;
		sPacket.subheader = SASH_SUBHEADER_CG_REFINED;
		sPacket.bWindow = m_bSashCombination == true ? true : false;
		sPacket.dwPrice = dwPrice;
		sPacket.bPos = 255;
		sPacket.tPos = tPos;
		sPacket.dwItemVnum = 0;
		sPacket.dwMinAbs = 0;
		sPacket.dwMaxAbs = 1;
		GetDesc()->Packet(&sPacket, sizeof(TPacketSash));
	}
}

bool CHARACTER::CleanSashAttr(LPITEM pkItem, LPITEM pkTarget)
{
	if (!CanHandleItem())
		return false;
	else if ((!pkItem) || (!pkTarget))
		return false;
	else if ((pkTarget->GetType() != ITEM_COSTUME) && (pkTarget->GetSubType() != COSTUME_SASH))
		return false;
	
	if (pkTarget->GetSocket(SASH_ABSORBED_SOCKET) <= 0)
		return false;
	
	pkTarget->SetSocket(SASH_ABSORBED_SOCKET, 0);
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		pkTarget->SetForceAttribute(i, 0, 0);
	
	pkItem->SetCount(pkItem->GetCount() - 1);
	LogManager::instance().ItemLog(this, pkTarget, "USE_DETACHMENT (CLEAN ATTR)", pkTarget->GetName());
	return true;
}
#endif
void CHARACTER::CheckMount()
{
	LPITEM item = GetWear(WEAR_UNIQUE1);
	LPITEM item2 = GetWear(WEAR_UNIQUE2);
	if (item)
	{
		if (!item->IsRideItem())
			return;
		else
		{
			AddAffect(AFFECT_MOUNT, POINT_MOUNT, item->GetValue(4), AFF_NONE, 86400, 0, true);
		}
	}
	else if (item2)
	{
		if (!item2->IsRideItem())
			return;
		else
		{
			AddAffect(AFFECT_MOUNT, POINT_MOUNT, item->GetValue(4), AFF_NONE, 86400, 0, true);
		}		
	}
	else
		return;
}

struct FCountBoss
{
	int n;
	int vnum;
	FCountBoss() : n(0) {};
	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (ch->GetRaceNum() == vnum)
				n++;
		}
	}
};

int CHARACTER::CountBossByVnum(int vnum)
{
	LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(m_lMapIndex);
	if (!pMap)
	{
		sys_err("cannot find map by index %d", m_lMapIndex);
		return 0;
	}

	FCountBoss f;
	f.vnum = vnum;

	pMap->for_each(f);
	return f.n;
}

#ifdef ENABLE_BEVIS_BOSS_TRACKING_SYSTEM
EVENTFUNC(boss_tracking_data_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info );
	if (!info)
		return 0;
	
	LPCHARACTER	ch = info->ch;
	if (!ch || !ch->GetDesc())
		return 0;

	if (ch->IsPC() == true)
	{
		if (ch->IsBossTrackingWindow == true)
		{
			for (int i = 0; i < 7; i++)
			{
				if (ch && ch->GetDesc())
					CBossTracking::instance().SendClientPacket(ch, i, ch->BossTrackingMobVnum);
			}
		}
	}
	return PASSES_PER_SEC(1);
}

void CHARACTER::StartBossTrackingDataEvent()
{
	if (m_pkUpdateBossTracking)
		return;
	
	char_event_info* info = AllocEventInfo<char_event_info>();
	info->ch = this;
	m_pkUpdateBossTracking = event_create(boss_tracking_data_event, info, PASSES_PER_SEC(1));
}

void CHARACTER::StopBossTrackingDataEvent()
{
	event_cancel(&m_pkUpdateBossTracking);
	m_pkUpdateBossTracking = NULL;
	BossTrackingMobVnum = 0;
	IsBossTrackingWindow = false;
}
#endif

#ifdef __SKILL_COLOR_SYSTEM__
void CHARACTER::SetSkillColor(DWORD * dwSkillColor)
{
	memcpy(m_dwSkillColor, dwSkillColor, sizeof(m_dwSkillColor));
	UpdatePacket();
}
#endif


//__ENABLE_NEW_OFFLINESHOP__
bool CHARACTER::CanTakeInventoryItem(LPITEM item, TItemPos* cell)
{
	int iEmpty = -1;
	if (item->IsDragonSoul())
	{
		cell->window_type = DRAGON_SOUL_INVENTORY;
		cell->cell = iEmpty = GetEmptyDragonSoulInventory(item);
	}
#ifdef ENABLE_SPECIAL_STORAGE
	else if (item->IsUpgradeItem())
	{
		cell->window_type = UPGRADE_INVENTORY;
		cell->cell = iEmpty = GetEmptyUpgradeInventory(item);
	}
	else if (item->IsStone())
	{
		cell->window_type = STONE_INVENTORY;
		cell->cell = iEmpty = GetEmptyStoneInventory(item);
	}
	else if (item->IsChest())
	{
		cell->window_type = CHEST_INVENTORY;
		cell->cell = iEmpty = GetEmptyChestInventory(item);
	}
	else if (item->IsAttr())
	{
		cell->window_type = ATTR_INVENTORY;
		cell->cell = iEmpty = GetEmptyAttrInventory(item);
	}
#endif
	else
	{
		cell->window_type = INVENTORY;
		cell->cell = iEmpty = GetEmptyInventory(item->GetSize());
	}

	return iEmpty != -1;
}

#ifdef KYGN_ITEM_REMOVE_OR_SELL
void CHARACTER::RemoveORSellOperation(BYTE value)
{
	if (!value)
		return;
	switch(value)
	{
		case 1://item silme
		{
			if (R_S_I_SlotIndex.empty())
				return;
			LPITEM item;
			for (std::vector <SItemRemoveORSellList>::iterator it = R_S_I_SlotIndex.begin(); it != R_S_I_SlotIndex.end(); it++)
			{
				if (it->window_type == 0)
					item = GetInventoryItem(it->SlotIndex);
				else if (it->window_type == 1)
					item = GetUpgradeInventoryItem(it->SlotIndex);
				else if (it->window_type == 2)
					item = GetStoneInventoryItem(it->SlotIndex);
				else if (it->window_type == 3)
					item = GetChestInventoryItem(it->SlotIndex);
				else if (it->window_type == 4)
					item = GetAttrInventoryItem(it->SlotIndex);
				if (item != NULL)
				{
					item->Lock(false);
					ITEM_MANAGER::instance().RemoveItem(item);
				}
			}
			R_S_I_SlotIndex.clear();
			break;
		}
		case 2:// Pencereyi Kapatma
		{
			if (R_S_I_SlotIndex.empty())
				return;
			LPITEM item;
			for (std::vector <SItemRemoveORSellList>::iterator it = R_S_I_SlotIndex.begin(); it != R_S_I_SlotIndex.end(); it++)
			{
				if (it->window_type == 0)
				{
					if((item = GetInventoryItem(it->SlotIndex)))
						item->Lock(false);
				}
				else if (it->window_type == 1)
				{
					if((item = GetUpgradeInventoryItem(it->SlotIndex)))
						item->Lock(false);
				}
				else if (it->window_type == 2)
				{
					if((item = GetStoneInventoryItem(it->SlotIndex)))
						item->Lock(false);
				}
				else if (it->window_type == 3)
				{
					if((item = GetChestInventoryItem(it->SlotIndex)))
						item->Lock(false);
				}
				else if (it->window_type == 4)
				{
					if((item = GetAttrInventoryItem(it->SlotIndex)))
						item->Lock(false);
				}
			}
			R_S_I_SlotIndex.clear();
			break;
		}
		case 3:// itemi Satma
		{
			if (R_S_I_SlotIndex.empty())
				return;
			LPITEM item;
#ifdef YANG_LIMIT
			LDWORD dwPrice;
#else
			long long dwPrice;
#endif
			short bCount;
			for (std::vector <SItemRemoveORSellList>::iterator it = R_S_I_SlotIndex.begin(); it != R_S_I_SlotIndex.end(); it++)
			{
				if (it->window_type == 0)
				{
					if((item = GetInventoryItem(it->SlotIndex))){
						bCount = item->GetCount();
						dwPrice = item->GetShopBuyPrice() * bCount;
						dwPrice /= 5;
					}
				}
				else if (it->window_type == 1)
				{
					if((item = GetUpgradeInventoryItem(it->SlotIndex))){
						bCount = item->GetCount();
						dwPrice = item->GetShopBuyPrice() * bCount;
						dwPrice /= 5;
					}
				}
				else if (it->window_type == 2)
				{
					if((item = GetStoneInventoryItem(it->SlotIndex))){
						bCount = item->GetCount();
						dwPrice = item->GetShopBuyPrice() * bCount;
						dwPrice /= 5;
					}
					
				}
				else if (it->window_type == 3)
				{
					if((item = GetChestInventoryItem(it->SlotIndex))){
						bCount = item->GetCount();
						dwPrice = item->GetShopBuyPrice() * bCount;
						dwPrice /= 5;
					}
				}
				else if (it->window_type == 4)
				{
					if((item = GetAttrInventoryItem(it->SlotIndex))){
						bCount = item->GetCount();
						dwPrice = item->GetShopBuyPrice() * bCount;
						dwPrice /= 5;
					}
				}
				if (item != NULL)
				{
#ifdef YANG_LIMIT
					const LDWORD nTotalMoney = static_cast<LDWORD>(GetGold()) + static_cast<LDWORD>(dwPrice);
					if (GOLD_MAX <= nTotalMoney)
#else
					const long long nTotalMoney = static_cast<long long>(GetGold()) + static_cast<long long>(dwPrice);
					if (GetAllowedGold() <= nTotalMoney)
#endif
					{
						ChatPacket(CHAT_TYPE_INFO, "Max Yang Limitine Ulastin");
						item->Lock(false);
					}
					else
					{
						item->Lock(false);
						ITEM_MANAGER::instance().RemoveItem(item);
#ifdef YANG_LIMIT
						GoldChange(dwPrice);
#else
						PointChange(POINT_GOLD, static_cast<long long>(dwPrice));
#endif
					}
				}
			}
			R_S_I_SlotIndex.clear();
			break;
		}
	}
}
#endif

#ifdef _ITEM_SHOP_SYSTEM
DWORD CHARACTER::GetDragonCoin()
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT cash FROM account.account WHERE id = '%d'", GetDesc()->GetAccountTable().id));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	DWORD dc = 0;
	str_to_number(dc, row[0]);
	return dc;
}

DWORD CHARACTER::GetDragonMark()
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT coins FROM account.account WHERE id = '%d'", GetDesc()->GetAccountTable().id));
	if (pMsg->Get()->uiNumRows == 0)
		return 0;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
	DWORD mark = 0;
	str_to_number(mark, row[0]);
	return mark;
}

void CHARACTER::SetDragonCoin(DWORD amount)
{
	delete DBManager::instance().DirectQuery("UPDATE account.account SET cash = '%d' WHERE id = '%d'", amount, GetDesc()->GetAccountTable().id);
	RefreshDragonCoin();
}

void CHARACTER::SetDragonMark(DWORD amount)
{
	delete DBManager::instance().DirectQuery("UPDATE account.account SET coins = '%d' WHERE id = '%d'", amount, GetDesc()->GetAccountTable().id);
	RefreshDragonCoin();
}

void CHARACTER::RefreshDragonCoin()
{
	ChatPacket(CHAT_TYPE_COMMAND, "RefreshDragonCoin %d", GetDragonCoin());
	ChatPacket(CHAT_TYPE_COMMAND, "RefreshDragonMark %d", GetDragonMark());
}
#endif

#ifdef ENABLE_WON_EXCHANGE
void CHARACTER::WonExchange(BYTE bOption, int wValue)
{
	if (!CanWarp())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("diger islemler gerceklestirilirken degisim islemi yurutulemez"));
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("lutfen her seyi kapatin veya bekleyin ve tekrar deneyin."));
		return;
	}

	float fMul = (bOption == WON_EXCHANGE_CG_SUBHEADER_BUY) ? 1.0f + (static_cast<float>(TAX_MUL) / 100.0f) : 1.0f;
#ifdef YANG_LIMIT
	LDWORD wVal = static_cast<LDWORD>(abs(wValue));
	LDWORD useVar = static_cast<LDWORD>(wVal * static_cast<int64_t>(CHEQUE_NAME_VALUE * fMul));
#else
	int wVal = abs(wValue);
#endif
	if (bOption == WON_EXCHANGE_CG_SUBHEADER_SELL)
	{
		if (GetCheque() < wVal)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("isleme devam etmek icin yeterli para yok."));
			return;
		}
#ifdef YANG_LIMIT
		if (GetGold() + useVar > GetAllowedGold())
#else
		if (GetGold() + static_cast<int64_t>(wVal) * static_cast<int64_t>(CHEQUE_NAME_VALUE * fMul) > GOLD_MAX)
#endif
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Takas islemi devam edemez cunku Yang limitinizi asacaktir."));
			return;
		}
		
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("islem basarili"));
#ifdef YANG_LIMIT
		GoldChange(useVar);
#else
		PointChange(POINT_GOLD, static_cast<int64_t>(wVal) * static_cast<int64_t>(CHEQUE_NAME_VALUE * fMul), true);
#endif
		PointChange(POINT_CHEQUE, -wVal, true);
	}
	else if (bOption == WON_EXCHANGE_CG_SUBHEADER_BUY)
	{
#ifdef YANG_LIMIT
		if (GetGold() < useVar)
#else
		if (GetGold() < static_cast<int64_t>(wVal) * static_cast<int64_t>(CHEQUE_NAME_VALUE * fMul))
#endif
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("isleme devam etmek icin yeterli miktar yok."));
			return;
		}
		if (GetCheque() + wVal >= CHEQUE_MAX)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Siniri astigi icin islem devam edemez."));
			return;
		}

		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("islem basarili"));
#ifdef YANG_LIMIT
		GoldChange(-useVar);
#else
		PointChange(POINT_GOLD, -(static_cast<int64_t>(wVal) * static_cast<int64_t>(CHEQUE_NAME_VALUE * fMul)), true);
#endif
		PointChange(POINT_CHEQUE, wVal, true);
	}
	else
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("malesef bir seyler ters gitti. Bilinmeyen takas islemi."));
	}
}
#endif
#ifdef ENABLE_KILL_STATISTICS
void CHARACTER::SendKillStatisticsPacket()
{
	if (!GetDesc())
	{
		return;
	}
	TPacketGCKillStatistics KillStatisticsGC;
	KillStatisticsGC.bHeader = HEADER_GC_KILL_STATISTICS;
	KillStatisticsGC.iDuelsWon = GetDuelsWon();
	KillStatisticsGC.iDuelsLost = GetDuelsLost();
	KillStatisticsGC.iBossesKills = GetBossesKills();
	KillStatisticsGC.iStonesKills = GetStonesKills();
	
	GetDesc()->Packet(&KillStatisticsGC, sizeof(TPacketGCKillStatistics));
}
#endif