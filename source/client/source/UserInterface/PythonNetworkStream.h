#pragma once

#include "../eterLib/FuncObject.h"
#include "../eterlib/NetStream.h"
#include "../eterLib/NetPacketHeaderMap.h"

#include "InsultChecker.h"

#include "packet.h"

#ifdef ENABLE_SWITCHBOT
#include "PythonSwitchbot.h"
#endif

class CInstanceBase;
class CNetworkActorManager;
struct SNetworkActorData;
struct SNetworkUpdateActorData;

class CPythonNetworkStream : public CNetworkStream, public CSingleton<CPythonNetworkStream>
{
	public:
		enum
		{
			SERVER_COMMAND_LOG_OUT = 0,
			SERVER_COMMAND_RETURN_TO_SELECT_CHARACTER = 1,
			SERVER_COMMAND_QUIT = 2,

			MAX_ACCOUNT_PLAYER
		};
		
		enum
		{
			ERROR_NONE,
			ERROR_UNKNOWN,
			ERROR_CONNECT_MARK_SERVER,			
			ERROR_LOAD_MARK,
			ERROR_MARK_WIDTH,
			ERROR_MARK_HEIGHT,

			// MARK_BUG_FIX
			ERROR_MARK_UPLOAD_NEED_RECONNECT,
			ERROR_MARK_CHECK_NEED_RECONNECT,
			// END_OF_MARK_BUG_FIX
		};

		enum
		{
			ACCOUNT_CHARACTER_SLOT_ID,
			ACCOUNT_CHARACTER_SLOT_NAME,
			ACCOUNT_CHARACTER_SLOT_RACE,
			ACCOUNT_CHARACTER_SLOT_LEVEL,
			ACCOUNT_CHARACTER_SLOT_STR,
			ACCOUNT_CHARACTER_SLOT_DEX,
			ACCOUNT_CHARACTER_SLOT_HTH,
			ACCOUNT_CHARACTER_SLOT_INT,
			ACCOUNT_CHARACTER_SLOT_PLAYTIME,
			ACCOUNT_CHARACTER_SLOT_FORM,
			ACCOUNT_CHARACTER_SLOT_ADDR,
			ACCOUNT_CHARACTER_SLOT_PORT,
			ACCOUNT_CHARACTER_SLOT_GUILD_ID,
			ACCOUNT_CHARACTER_SLOT_GUILD_NAME,
			ACCOUNT_CHARACTER_SLOT_CHANGE_NAME_FLAG,
			ACCOUNT_CHARACTER_SLOT_HAIR,
			#ifdef ENABLE_SASH_SYSTEM
			ACCOUNT_CHARACTER_SLOT_SASH,
			#endif
		};

		enum
		{
			PHASE_WINDOW_LOGO,
			PHASE_WINDOW_LOGIN,
			PHASE_WINDOW_SELECT,
			PHASE_WINDOW_CREATE,
			PHASE_WINDOW_LOAD,
			PHASE_WINDOW_GAME,
			PHASE_WINDOW_EMPIRE,
			PHASE_WINDOW_NUM,
		};

	public:
		CPythonNetworkStream();
		virtual ~CPythonNetworkStream();
		
		bool SendSpecial(int nLen, void * pvBuf);

		void StartGame();
		void Warp(LONG lGlobalX, LONG lGlobalY);
		
		void NotifyHack(const char* c_szMsg);		
		void SetWaitFlag();
#ifdef ENABLE_DISCORD_RPC
		void Discord_Start();
		void Discord_Close();
		void Discord_Update(const bool ingame);
#endif

		void SendEmoticon(UINT eEmoticon);

		void ExitApplication();
		void ExitGame();
		void LogOutGame();
		void AbsoluteExitGame();
		void AbsoluteExitApplication();

		void EnableChatInsultFilter(bool isEnable);		
		bool IsChatInsultIn(const char* c_szMsg);
		bool IsInsultIn(const char* c_szMsg);

		DWORD GetGuildID();

		UINT UploadMark(const char* c_szImageFileName);
		UINT UploadSymbol(const char* c_szImageFileName);

		bool LoadInsultList(const char* c_szInsultListFileName);
		bool LoadConvertTable(DWORD dwEmpireID, const char* c_szFileName);

		UINT		GetAccountCharacterSlotDatau(UINT iSlot, UINT eType);
		const char* GetAccountCharacterSlotDataz(UINT iSlot, UINT eType);

		// SUPPORT_BGM
		const char*		GetFieldMusicFileName();
		float			GetFieldMusicVolume();
		// END_OF_SUPPORT_BGM

		bool IsSelectedEmpire();

		void ToggleGameDebugInfo();

		void SetMarkServer(const char* c_szAddr, UINT uPort);
		void ConnectLoginServer(const char* c_szAddr, UINT uPort);
		void ConnectGameServer(UINT iChrSlot);

		void SetLoginInfo(const char* c_szID, const char* c_szPassword, const char* c_szClientVersion, const char* c_szPin);
		std::string GetLoginID() const { return m_stID; }
		void SetLoginKey(DWORD dwLoginKey);
		void ClearLoginInfo( void );

		void SetHandler(PyObject* poHandler);
		void SetPhaseWindow(UINT ePhaseWnd, PyObject* poPhaseWnd);
		void ClearPhaseWindow(UINT ePhaseWnd, PyObject* poPhaseWnd);
		void SetServerCommandParserWindow(PyObject* poPhaseWnd);

		bool SendSyncPositionElementPacket(DWORD dwVictimVID, DWORD dwVictimX, DWORD dwVictimY);

#ifdef URIEL_ANTI_CHEAT
bool SendAttackPacket(UINT uMotAttack, DWORD dwVIDVictim, DWORD dwVIDVictimVerify);
#else
bool SendAttackPacket(UINT uMotAttack, DWORD dwVIDVictim);
#endif
		bool SendCharacterStatePacket(const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg);
		bool SendUseSkillPacket(DWORD dwSkillIndex, DWORD dwTargetVID=0);
#ifdef ENABLE_SKILL_COLOR_SYSTEM
		bool SendSkillColorPacket(BYTE bSkillSlot, DWORD dwColor1, DWORD dwColor2, DWORD dwColor3, DWORD dwColor4, DWORD dwColor5);
#endif
#ifdef ENABLE_WON_EXCHANGE
		bool SendWonExchangeSellPacket(int wValue);
		bool SendWonExchangeBuyPacket(int wValue);
#endif
		bool SendTargetPacket(DWORD dwVID);

		// OLDCODE:
		bool SendCharacterStartWalkingPacket(float fRotation, long lx, long ly);
		bool SendCharacterEndWalkingPacket(float fRotation, long lx, long ly);
		bool SendCharacterCheckWalkingPacket(float fRotation, long lx, long ly);

		bool SendCharacterPositionPacket(BYTE iPosition);

		bool SendItemUsePacket(TItemPos pos, bool useAll = false);
		bool SendItemUseToItemPacket(TItemPos source_pos, TItemPos target_pos);
#ifdef ENABLE_CHEQUE_SYSTEM
		bool SendItemDropPacket(TItemPos pos, DWORD elk, DWORD won);
		bool SendItemDropPacketNew(TItemPos pos, DWORD elk, DWORD won, DWORD count);
#else
		bool SendItemDropPacket(TItemPos pos, DWORD elk);
		bool SendItemDropPacketNew(TItemPos pos, DWORD elk, DWORD count);
#endif
		bool SendItemDestroyPacket(TItemPos pos);
		bool SendItemSellPacket(TItemPos pos, short count);

#ifdef ENABLE_EXTENDED_ITEM_COUNT
	bool SendItemMovePacket(TItemPos pos, TItemPos change_pos, short num);
#else
	bool SendItemMovePacket(TItemPos pos, TItemPos change_pos, BYTE num);
#endif
		bool SendItemPickUpPacket(DWORD vid);

		bool SendQuickSlotAddPacket(BYTE wpos, BYTE type, BYTE pos);
		bool SendQuickSlotDelPacket(BYTE wpos);
		bool SendQuickSlotMovePacket(BYTE wpos, BYTE change_pos);

		// PointReset 개 임시
		bool SendPointResetPacket();

		// Shop
		bool SendShopEndPacket();
		bool SendShopBuyPacket(BYTE byCount);
		bool SendShopSellPacket(BYTE bySlot);
		bool SendShopSearchFindPacket(int iMinPrice, const char* pcItemName);
#ifdef ENABLE_SPECIAL_STORAGE
		bool SendShopSellPacketNew(BYTE bySlot, short byCount, BYTE byType);
#else
		bool SendShopSellPacketNew(BYTE bySlot, BYTE byCount);
#endif
		#ifdef ENABLE_AURA_SYSTEM
		bool SendAuraClosePacket();
		bool SendAuraAddPacket(TItemPos tPos, BYTE bPos);
		bool SendAuraRemovePacket(BYTE bPos);
		bool SendAuraRefinePacket();
		#endif
		// OfflineShop
		bool SendOfflineShopEndPacket();
		bool SendOfflineShopBuyPacket(BYTE byCount);
		bool SendChangeOfflineShopTime(BYTE bTime);
		bool SendChangePriceOfflineShopItem(BYTE bPos, int iPrice, int iPriceCheque);
		bool SendAddOfflineShopItem(TItemPos tDisplayPos, BYTE bPos, long lPrice, long lCheque);
		bool SendRemoveOfflineShopItem(BYTE bPos);
		bool SendDestroyOfflineShop();
		bool SendRefreshOfflineShop();
		bool SendRefreshOfflineShopMoney();
		bool SendOfflineShopWithdrawMoney(DWORD dwMoney);
		bool SendOfflineShopWithdrawCheque(DWORD dwCheque);

		// Exchange
		bool SendExchangeStartPacket(DWORD vid);
#if defined(ITEM_CHECKINOUT_UPDATE)
		bool SendExchangeItemAddPacket(TItemPos ItemPos, BYTE byDisplayPos, bool SelectPosAuto);
#else
		bool SendExchangeItemAddPacket(TItemPos ItemPos, BYTE byDisplayPos);
#endif
#ifdef YANG_LIMIT
		bool SendExchangeElkAddPacket(ULDWORD elk);
#else
		bool SendExchangeElkAddPacket(DWORD elk);
#endif
#ifdef ENABLE_CHEQUE_SYSTEM
		bool SendExchangeWonAddPacket(DWORD won);
#endif
		bool SendExchangeItemDelPacket(BYTE pos);
		bool SendExchangeAcceptPacket();
		bool SendExchangeExitPacket();

		// Quest
		bool SendScriptAnswerPacket(int iAnswer);
		bool SendScriptButtonPacket(unsigned int iIndex);
		bool SendAnswerMakeGuildPacket(const char * c_szName);
		bool SendQuestInputStringPacket(const char * c_szString);
		bool SendQuestConfirmPacket(BYTE byAnswer, DWORD dwPID);

		// Event
		bool SendOnClickPacket(DWORD vid);

		// Fly
		bool SendFlyTargetingPacket(DWORD dwTargetVID, const TPixelPosition& kPPosTarget);
		bool SendAddFlyTargetingPacket(DWORD dwTargetVID, const TPixelPosition& kPPosTarget);
		bool SendShootPacket(UINT uSkill);

		// Command
		bool ClientCommand(const char * c_szCommand);
		void ServerCommand(char * c_szCommand);

		// Emoticon
		void RegisterEmoticonString(const char * pcEmoticonString);

		// Party
		bool SendPartyInvitePacket(DWORD dwVID);
		bool SendPartyInviteAnswerPacket(DWORD dwLeaderVID, BYTE byAccept);
		bool SendPartyRemovePacket(DWORD dwPID);
		bool SendPartySetStatePacket(DWORD dwVID, BYTE byState, BYTE byFlag);
		bool SendPartyUseSkillPacket(BYTE bySkillIndex, DWORD dwVID);
		bool SendPartyParameterPacket(BYTE byDistributeMode);

		// SafeBox
		bool SendSafeBoxMoneyPacket(BYTE byState, DWORD dwMoney);
#if defined(ITEM_CHECKINOUT_UPDATE)
		bool SendSafeBoxCheckinPacket(TItemPos InventoryPos, BYTE bySafeBoxPos, bool SelectPosAuto);
#else
		bool SendSafeBoxCheckinPacket(TItemPos InventoryPos, BYTE bySafeBoxPos);
#endif

#if defined(ITEM_CHECKINOUT_UPDATE)
		bool SendSafeBoxCheckoutPacket(BYTE bySafeBoxPos, TItemPos InventoryPos, bool SelectPosAuto);
#else
		bool SendSafeBoxCheckoutPacket(BYTE bySafeBoxPos, TItemPos InventoryPos);
#endif
#ifdef ENABLE_EXTENDED_ITEM_COUNT
		bool SendSafeBoxItemMovePacket(BYTE bySourcePos, BYTE byTargetPos, short byCount);
#else
		bool SendSafeBoxItemMovePacket(BYTE bySourcePos, BYTE byTargetPos, BYTE byCount);
#endif

		// Mall
#if defined(ITEM_CHECKINOUT_UPDATE)
		bool SendMallCheckoutPacket(BYTE byMallPos, TItemPos InventoryPos, bool SelectPosAuto);
#else
		bool SendMallCheckoutPacket(BYTE byMallPos, TItemPos InventoryPos);
#endif

		// Guild
		bool SendGuildAddMemberPacket(DWORD dwVID);
		bool SendGuildRemoveMemberPacket(DWORD dwPID);
		bool SendGuildChangeGradeNamePacket(BYTE byGradeNumber, const char * c_szName);
		bool SendGuildChangeGradeAuthorityPacket(BYTE byGradeNumber, BYTE byAuthority);
		bool SendGuildOfferPacket(DWORD dwExperience);
		bool SendGuildPostCommentPacket(const char * c_szMessage);
		bool SendGuildDeleteCommentPacket(DWORD dwIndex);
		bool SendGuildRefreshCommentsPacket(DWORD dwHighestIndex);
		bool SendGuildChangeMemberGradePacket(DWORD dwPID, BYTE byGrade);
		bool SendGuildUseSkillPacket(DWORD dwSkillID, DWORD dwTargetVID);
		bool SendGuildChangeMemberGeneralPacket(DWORD dwPID, BYTE byFlag);
		bool SendGuildInvitePacket(DWORD dwVID);
		bool SendGuildInviteAnswerPacket(DWORD dwGuildID, BYTE byAnswer);
		bool SendGuildChargeGSPPacket(DWORD dwMoney);
		bool SendGuildDepositMoneyPacket(DWORD dwMoney);
		bool SendGuildWithdrawMoneyPacket(DWORD dwMoney);
#ifdef ENABLE_NEW_PET_SYSTEM
		bool PetSetNamePacket(const char * petname);
#endif

		// Mall
		bool RecvMallOpenPacket();
#ifdef OTOMATIK_AV
		void OtoAvDolumSuresiGonder(int slotIndex, int gelenDS){ PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetAutoCoolTime", Py_BuildValue("(ii)", slotIndex, gelenDS)); };
#endif
		bool RecvMallItemSetPacket();
		bool RecvMallItemDelPacket();
#ifdef ENABLE_KILL_STATISTICS
		bool RecvKillStatistics();
#endif
#ifdef __ENABLE_NEW_OFFLINESHOP__
		bool RecvOfflineshopPacket();


		bool RecvOfflineshopShopList();
		bool RecvOfflineshopShopOpen();
		bool RecvOfflineshopShopOpenOwner();
		bool RecvOfflineshopShopOpenOwnerNoShop();
		bool RecvOfflineshopShopClose();
		bool RecvOfflineshopShopFilterResult();
		bool RecvOfflineshopOfferList();
		bool RecvOfflineshopShopSafeboxRefresh();
		bool RecvOfflineshopShopBuyItemFromSearch();

		bool RecvOfflineshopAuctionList();
		bool RecvOfflineshopOpenMyAuction();
		bool RecvOfflineshopOpenMyAuctionNoAuction();
		bool RecvOfflineshopOpenAuction();
#ifdef ENABLE_NEW_SHOP_IN_CITIES
		bool RecvOfflineshopInsertEntity();
		bool RecvOfflineshopRemoveEntity();

		void SendOfflineshopOnClickShopEntity(DWORD dwPickedShopVID);
#endif


		void SendOfflineshopShopCreate(const offlineshop::TShopInfo& shopInfo, const std::vector<offlineshop::TShopItemInfo>& items);
		void SendOfflineshopChangeName(const char* szName);
		void SendOfflineshopForceCloseShop();

		void SendOfflineshopRequestShopList();

		void SendOfflineshopOpenShop(DWORD dwOwnerID);
		void SendOfflineshopOpenShopOwner();

		void SendOfflineshopBuyItem(DWORD dwOwnerID, DWORD dwItemID, bool isSearch);
		
		void SendOfflineshopAddItem(offlineshop::TShopItemInfo& itemInfo);
		void SendOfflineshopRemoveItem(DWORD dwItemID);
		void SendOfflineShopEditItem(DWORD dwItemID, const offlineshop::TPriceInfo& price);

		void SendOfflineshopFilterRequest(const offlineshop::TFilterInfo& filter);
		
		void SendOfflineshopOfferCreate(const offlineshop::TOfferInfo& offer);
		void SendOfflineshopOfferAccept(DWORD dwOfferID);
		void SendOfflineshopOfferCancel(DWORD dwOfferID, DWORD dwOwnerID);
		void SendOfflineshopOfferListRequest();

		void SendOfflineshopSafeboxOpen();
		void SendOfflineshopSafeboxGetItem(DWORD dwItemID);
		void SendOfflineshopSafeboxGetValutes(const offlineshop::TValutesInfo& valutes);
		void SendOfflineshopSafeboxClose();

		//AUCTION
		void SendOfflineshopAuctionListRequest();
		void SendOfflineshopAuctionOpen(DWORD dwOwnerID);
		void SendOfflineshopAuctionAddOffer(DWORD dwOwnerID, const offlineshop::TPriceInfo& price);
		void SendOfflineshopAuctionExitFrom(DWORD dwOwnerID);
		void SendOfflineshopAuctionCreate(const TItemPos& pos, const offlineshop::TPriceInfo& price, DWORD dwDuration);
		void SendOfflineshopAuctionOpenMy();

		void SendOfflineshopCloseBoard();
#endif
		// Lover
		bool RecvLoverInfoPacket();
		bool RecvLovePointUpdatePacket();

		// Dig
		bool RecvDigMotionPacket();

		// Fishing
		bool SendFishingPacket(int iRotation);
		bool SendGiveItemPacket(DWORD dwTargetVID, TItemPos ItemPos, int iItemCount);

		// Private Shop
		bool SendBuildPrivateShopPacket(const char * c_szName, const std::vector<TShopItemTable> & c_rSellingItemStock);

		// OfflineShop

		bool SendBuildOfflineShopPacket(const char * c_szName, const std::vector<TShopItemTable> & c_rSellingItemStock, BYTE bTime);
		// Refine
		bool SendRefinePacket(BYTE byPos, BYTE byType);
		bool SendSelectItemPacket(DWORD dwItemPos);
		bool SendInstantChestOpenPacket(TItemPos iPos);


		// Client Version
		bool SendClientVersionPacket();

		// CRC Report
		bool __SendCRCReportPacket();

		// 용홍석 강화
		bool SendDragonSoulRefinePacket(BYTE bRefineType, TItemPos* pos);

		// Handshake
		bool RecvHandshakePacket();
		bool RecvHandshakeOKPacket();

		bool RecvHybridCryptKeyPacket();
		bool RecvHybridCryptSDBPacket();
#ifdef _IMPROVED_PACKET_ENCRYPTION_
		bool RecvKeyAgreementPacket();
		bool RecvKeyAgreementCompletedPacket();
#endif
		// ETC
		DWORD GetMainActorVID();
		DWORD GetMainActorRace();
		DWORD GetMainActorEmpire();
		DWORD GetMainActorSkillGroup();
		void SetEmpireID(DWORD dwEmpireID);
		DWORD GetEmpireID();
		void __TEST_SetSkillGroupFake(int iIndex);
#ifdef ENABLE_SHOW_CHEST_DROP
		bool 	SendChestDropInfo(WORD wInventoryCell, BYTE wInventoryType);
		bool 	RecvChestDropInfo();
#endif
		#ifdef ENABLE_SASH_SYSTEM
		bool	SendSashClosePacket();
		bool	SendSashAddPacket(TItemPos tPos, BYTE bPos);
		bool	SendSashRemovePacket(BYTE bPos);
		bool	SendSashRefinePacket();
		#endif
#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
		bool SendPrivateShopSearchInfo(int32_t iJob, int32_t iMasktype, int32_t iMasksub, int32_t iMinRefine, int32_t iMaxRefine, int32_t iMinLevel, int32_t iMaxLevel, uint64_t iMinGold, uint64_t iMaxGold, char* iItemName, uint64_t iMinCheque, uint64_t iMaxCheque);
		bool SendPrivateShopSerchBuyItem(int32_t shopVid, BYTE shopItemPos);
		bool RecvShopSearchSet();
#endif

	//////////////////////////////////////////////////////////////////////////
	// Phase 관련
	//////////////////////////////////////////////////////////////////////////
	public:
		void SetOffLinePhase();
		void SetHandShakePhase();
		void SetLoginPhase();
		void SetSelectPhase();
		void SetLoadingPhase();
		void SetGamePhase();
		void ClosePhase();

		// Login Phase
		bool SendLoginPacket(const char * c_szName, const char * c_szPassword);
		bool SendLoginPacketNew(const char * c_szName, const char * c_szPassword);
		bool SendChinaMatrixCardPacket(const char * c_szMatrixCardString);
		bool SendRunupMatrixAnswerPacket(const char * c_szMatrixCardString);
		bool SendNEWCIBNPasspodAnswerPacket(const char * answer);
		bool SendDirectEnterPacket(const char * c_szName, const char * c_szPassword, UINT uChrSlot);

		bool SendEnterGame();

		// Select Phase
		bool SendSelectEmpirePacket(DWORD dwEmpireID);
		bool SendSelectCharacterPacket(BYTE account_Index);
		bool SendChangeNamePacket(BYTE index, const char *name);
		bool SendCreateCharacterPacket(BYTE index, const char *name, BYTE job, BYTE shape, BYTE byStat1, BYTE byStat2, BYTE byStat3, BYTE byStat4);
		bool SendDestroyCharacterPacket(BYTE index, const char * szPrivateCode);

		// Main Game Phase
		bool SendC2CPacket(DWORD dwSize, void * pData);
		bool SendChatPacket(const char * c_szChat, BYTE byType = CHAT_TYPE_TALKING);
#ifdef ENABLE_PM_ALL_SEND_SYSTEM
		bool SendBulkWhisperPacket(const char* c_szContent);
#endif
		bool SendWhisperPacket(const char * name, const char * c_szChat);
		bool SendMobileMessagePacket(const char * name, const char * c_szChat);
		bool SendMessengerAddByVIDPacket(DWORD vid);
		bool SendMessengerAddByNamePacket(const char * c_szName);
		bool SendMessengerRemovePacket(const char * c_szKey, const char * c_szName);
#if defined(ENABLE_MESSENGER_BLOCK)
		bool SendMessengerBlockAddByVIDPacket(DWORD vid);
		bool SendMessengerAddBlockByNamePacket(const char * c_szName);
		bool SendMessengerBlockRemoveByVIDPacket(const char * c_szKey, const char * c_szName);
#endif

	protected:
		bool OnProcess();	// State들을 실제로 실행한다.
		void OffLinePhase();
		void HandShakePhase();
		void LoginPhase();
		void SelectPhase();
		void LoadingPhase();
		void GamePhase();

		bool __IsNotPing();

		void __DownloadMark();
		void __DownloadSymbol(const std::vector<DWORD> & c_rkVec_dwGuildID);

		void __PlayInventoryItemUseSound(TItemPos uSlotPos);
		void __PlayInventoryItemDropSound(TItemPos uSlotPos);
		//void __PlayShopItemDropSound(UINT uSlotPos);
		void __PlaySafeBoxItemDropSound(UINT uSlotPos);
		void __PlayMallItemDropSound(UINT uSlotPos);

		bool __CanActMainInstance();

		enum REFRESH_WINDOW_TYPE
		{
			RefreshStatus = (1 << 0),
			RefreshAlignmentWindow = (1 << 1),
			RefreshCharacterWindow = (1 << 2),
			RefreshEquipmentWindow = (1 << 3), 
			RefreshInventoryWindow = (1 << 4),
			RefreshExchangeWindow = (1 << 5),
			RefreshSkillWindow = (1 << 6),
			RefreshSafeboxWindow  = (1 << 7),
			RefreshMessengerWindow = (1 << 8),
			RefreshGuildWindowInfoPage = (1 << 9),
			RefreshGuildWindowBoardPage = (1 << 10),
			RefreshGuildWindowMemberPage = (1 << 11), 
			RefreshGuildWindowMemberPageGradeComboBox = (1 << 12),
			RefreshGuildWindowSkillPage = (1 << 13),
			RefreshGuildWindowGradePage = (1 << 14),
			RefreshTargetBoard = (1 << 15),
			RefreshMallWindow = (1 << 16),
		};

		void __RefreshStatus();
		void __RefreshAlignmentWindow();
		void __RefreshCharacterWindow();
		void __RefreshEquipmentWindow();
		void __RefreshInventoryWindow();
		void __RefreshExchangeWindow();
		void __RefreshSkillWindow();
		void __RefreshSafeboxWindow();
		void __RefreshMessengerWindow();
		void __RefreshGuildWindowInfoPage();
		void __RefreshGuildWindowBoardPage();
		void __RefreshGuildWindowMemberPage();
		void __RefreshGuildWindowMemberPageGradeComboBox();
		void __RefreshGuildWindowSkillPage();
		void __RefreshGuildWindowGradePage();
		void __RefreshTargetBoardByVID(DWORD dwVID);
		void __RefreshTargetBoardByName(const char * c_szName);
		void __RefreshTargetBoard();
		void __RefreshMallWindow();
#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
		void __RefreshShopSearchWindow();
#endif
	public:
		bool __SendHack(const char* c_szMsg);
		std::string GetPhase();

	protected:
		bool RecvObserverAddPacket();
		bool RecvObserverRemovePacket();
		bool RecvObserverMovePacket();

		// Common
		bool RecvErrorPacket(int header);
		bool RecvPingPacket();
		bool RecvDefaultPacket(int header);
		bool RecvPhasePacket();

		// Login Phase
		bool __RecvLoginSuccessPacket3();
		bool __RecvLoginSuccessPacket4();
		bool __RecvLoginFailurePacket();
		bool __RecvEmpirePacket();
		bool __RecvChinaMatrixCardPacket();
		bool __RecvRunupMatrixQuizPacket();
		bool __RecvNEWCIBNPasspodRequestPacket();
		bool __RecvNEWCIBNPasspodFailurePacket();
		bool __RecvLoginKeyPacket();

		// Select Phase
		bool __RecvPlayerCreateSuccessPacket();
		bool __RecvPlayerCreateFailurePacket();
		bool __RecvPlayerDestroySuccessPacket();
		bool __RecvPlayerDestroyFailurePacket();
		bool __RecvPreserveItemPacket();
		bool __RecvPlayerPoints();
		bool __RecvChangeName();
#ifdef YANG_LIMIT
		bool __RecvPlayerGold();
#endif

		// Loading Phase
		bool RecvMainCharacter();		
		bool RecvMainCharacter2_EMPIRE();
		bool RecvMainCharacter3_BGM();
		bool RecvMainCharacter4_BGM_VOL();

		void __SetFieldMusicFileName(const char* musicName);
		void __SetFieldMusicFileInfo(const char* musicName, float vol);
		// END_OF_SUPPORT_BGM

		// Main Game Phase
		bool RecvWarpPacket();
		bool RecvPVPPacket();
		bool RecvDuelStartPacket();
        bool RecvGlobalTimePacket();
		bool RecvCharacterAppendPacket();
		bool RecvCharacterAdditionalInfo();
		bool RecvCharacterAppendPacketNew();
		bool RecvCharacterUpdatePacket();
		bool RecvCharacterUpdatePacketNew();
		bool RecvCharacterDeletePacket();
		bool RecvChatPacket();
#ifdef ENABLE_PM_ALL_SEND_SYSTEM
		bool RecvBulkWhisperPacket();
#endif
		bool RecvOwnerShipPacket();
		bool RecvSyncPositionPacket();
		bool RecvWhisperPacket();
		bool RecvPointChange();					// Alarm to python
		bool RecvChangeSpeedPacket();
#ifdef YANG_LIMIT
		bool RecvGoldChange();
#endif
		bool RecvStunPacket();
		bool RecvDeadPacket();
		bool RecvCharacterMovePacket();

		bool RecvItemSetPacket();					// Alarm to python
		bool RecvItemSetPacket2();					// Alarm to python
		bool RecvItemUsePacket();					// Alarm to python
		bool RecvItemUpdatePacket();				// Alarm to python
		bool RecvItemGroundAddPacket();
		bool RecvItemGroundDelPacket();
		bool RecvItemOwnership();

#ifdef LWT_BUFF_UPDATE
		bool RecvSupportUseSkill();
#endif

		bool RecvQuickSlotAddPacket();				// Alarm to python
		bool RecvQuickSlotDelPacket();				// Alarm to python
		bool RecvQuickSlotMovePacket();				// Alarm to python

		bool RecvCharacterPositionPacket();
		bool RecvMotionPacket();

		bool RecvShopPacket();
		bool RecvShopSignPacket();
		bool RecvExchangePacket();

		bool RecvOfflineShopPacket();
		bool RecvOfflineShopSignPacket();
		// Quest
		bool RecvScriptPacket();
		bool RecvQuestInfoPacket();
		bool RecvQuestConfirmPacket();
		bool RecvRequestMakeGuild();

		// Skill
		bool RecvSkillLevel();
		bool RecvSkillLevelNew();
		bool RecvSkillCoolTimeEnd();

		// Target
		bool RecvTargetPacket();
		bool RecvViewEquipPacket();
		bool RecvDamageInfoPacket();
#ifdef ENABLE_BOSS_TRACKING
		bool RecvBossTracking();
#endif
#ifdef ENABLE_SEND_TARGET_INFO
		bool RecvTargetInfoPacket();

		public:
			bool SendTargetInfoLoadPacket(DWORD dwVID);

		protected:
#endif

		// Mount
		bool RecvMountPacket();

		// Fly
		bool RecvCreateFlyPacket();
		bool RecvFlyTargetingPacket();
		bool RecvAddFlyTargetingPacket();

		// Messenger
		bool RecvMessenger();

		// Guild
		bool RecvGuild();

#ifdef ENABLE_ITEM_SHOP_SYSTEM
		bool RecvItemShopData();
#endif

		// Party
		bool RecvPartyInvite();
		bool RecvPartyAdd();
		bool RecvPartyUpdate();
		bool RecvPartyRemove();
		bool RecvPartyLink();
		bool RecvPartyUnlink();
		bool RecvPartyParameter();

		// SafeBox
		bool RecvSafeBoxSetPacket();
		bool RecvSafeBoxDelPacket();
		bool RecvSafeBoxWrongPasswordPacket();
		bool RecvSafeBoxSizePacket();
		bool RecvSafeBoxMoneyChangePacket();

		// Fishing
		bool RecvFishing();

		// Dungeon
		bool RecvDungeon();

		// Time
		bool RecvTimePacket();

		// WalkMode
		bool RecvWalkModePacket();

		// ChangeSkillGroup
		bool RecvChangeSkillGroupPacket();

		// Refine
		bool RecvRefineInformationPacket();
		bool RecvRefineInformationPacketNew();

		// Use Potion
		bool RecvSpecialEffect();

		// 서버에서 지정한 이팩트 발동 패킷.
		bool RecvSpecificEffect();
		
		// 용혼석 관련
		bool RecvDragonSoulRefine();

		// MiniMap Info
		bool RecvNPCList();
		bool RecvLandPacket();
		bool RecvTargetCreatePacket();
		bool RecvTargetCreatePacketNew();
		bool RecvTargetUpdatePacket();
		bool RecvTargetDeletePacket();

		// Affect
		bool RecvAffectAddPacket();
		bool RecvAffectRemovePacket();

		// Channel
		bool RecvChannelPacket();
		#ifdef ENABLE_SASH_SYSTEM
		bool	RecvSashPacket(bool bReturn = false);
		#endif
		
#ifdef ENABLE_AURA_SYSTEM
		bool RecvAuraPacket(bool bReturn = false);
#endif
		//Security
		bool RecvHSCheckRequest();
		bool RecvXTrapVerifyRequest();

	protected:
		// 이모티콘
		bool ParseEmoticon(const char * pChatMsg, DWORD * pdwEmoticon);

		// 파이썬으로 보내는 콜들
		void OnConnectFailure();
		void OnScriptEventStart(int iSkin, int iIndex);
		void HideQuestWindows();
		
		void OnRemoteDisconnect();
		void OnDisconnect();

		void SetGameOnline();
		void SetGameOffline();
		BOOL IsGameOnline();

	protected:
		bool CheckPacket(TPacketHeader * pRetHeader);
		
		void __InitializeGamePhase();
		void __InitializeMarkAuth();
		void __GlobalPositionToLocalPosition(LONG& rGlobalX, LONG& rGlobalY);
		void __LocalPositionToGlobalPosition(LONG& rLocalX, LONG& rLocalY);

		bool __IsPlayerAttacking();
		bool __IsEquipItemInSlot(TItemPos Cell);

		void __ShowMapName(LONG lLocalX, LONG lLocalY);

		void __LeaveOfflinePhase() {}
		void __LeaveHandshakePhase() {}
		void __LeaveLoginPhase() {}
		void __LeaveSelectPhase() {}
		void __LeaveLoadingPhase() {}
		void __LeaveGamePhase();

		void __ClearNetworkActorManager();

		void __ClearSelectCharacterData();

		// DELETEME
		//void __SendWarpPacket();

		void __ConvertEmpireText(DWORD dwEmpireID, char* szText);

		void __RecvCharacterAppendPacket(SNetworkActorData * pkNetActorData);
		void __RecvCharacterUpdatePacket(SNetworkUpdateActorData * pkNetUpdateActorData);

		void __FilterInsult(char* szLine, UINT uLineLen);

		void __SetGuildID(DWORD id);

	protected:
		TPacketGCHandshake m_HandshakeData;
		DWORD m_dwChangingPhaseTime;
		DWORD m_dwBindupRetryCount;
		DWORD m_dwMainActorVID;
		DWORD m_dwMainActorRace;
		DWORD m_dwMainActorEmpire;
		DWORD m_dwMainActorSkillGroup;
		BOOL m_isGameOnline;
		BOOL m_isStartGame;

		DWORD m_dwGuildID;
		DWORD m_dwEmpireID;
		
		struct SServerTimeSync
		{
			DWORD m_dwChangeServerTime;
			DWORD m_dwChangeClientTime;
		} m_kServerTimeSync;

		void __ServerTimeSync_Initialize();
		//DWORD m_dwBaseServerTime;
		//DWORD m_dwBaseClientTime;

		DWORD m_dwLastGamePingTime;

		std::string	m_stID;
		std::string	m_stPassword;
		std::string	m_stClientVersion;
		std::string	m_stPin;
		std::string	m_strLastCommand;
		std::string	m_strPhase;
		DWORD m_dwLoginKey;
		BOOL m_isWaitLoginKey;

		std::string m_stMarkIP;

		CFuncObject<CPythonNetworkStream>	m_phaseProcessFunc;
		CFuncObject<CPythonNetworkStream>	m_phaseLeaveFunc;

		PyObject*							m_poHandler;
		PyObject*							m_apoPhaseWnd[PHASE_WINDOW_NUM];
		PyObject*							m_poSerCommandParserWnd;

		TSimplePlayerInformation			m_akSimplePlayerInfo[PLAYER_PER_ACCOUNT4];
		DWORD								m_adwGuildID[PLAYER_PER_ACCOUNT4];
		std::string							m_astrGuildName[PLAYER_PER_ACCOUNT4];
		bool m_bSimplePlayerInfo;

		CRef<CNetworkActorManager>			m_rokNetActorMgr;

		bool m_isRefreshStatus;
		bool m_isRefreshCharacterWnd;
		bool m_isRefreshEquipmentWnd;
		bool m_isRefreshInventoryWnd;
		bool m_isRefreshExchangeWnd;
		bool m_isRefreshSkillWnd;
		bool m_isRefreshSafeboxWnd;
		bool m_isRefreshMallWnd;
		bool m_isRefreshMessengerWnd;
		bool m_isRefreshGuildWndInfoPage;
		bool m_isRefreshGuildWndBoardPage;
		bool m_isRefreshGuildWndMemberPage;
		bool m_isRefreshGuildWndMemberPageGradeComboBox;
		bool m_isRefreshGuildWndSkillPage;
		bool m_isRefreshGuildWndGradePage;

#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
		bool m_isRefreshShopSearchWnd;
#endif
		// Emoticon
		std::vector<std::string> m_EmoticonStringVector;

		struct STextConvertTable 
		{
			char acUpper[26];
			char acLower[26];
			BYTE aacHan[5000][2];
		} m_aTextConvTable[3];



		struct SMarkAuth
		{
			CNetworkAddress m_kNetAddr;
			DWORD m_dwHandle;
			DWORD m_dwRandomKey;
		} m_kMarkAuth;



		DWORD m_dwSelectedCharacterIndex;

		CInsultChecker m_kInsultChecker;

		bool m_isEnableChatInsultFilter;
		bool m_bComboSkillFlag;

		std::deque<std::string> m_kQue_stHack;

	private:
		struct SDirectEnterMode
		{
			bool m_isSet;
			DWORD m_dwChrSlotIndex;
		} m_kDirectEnterMode;

		void __DirectEnterMode_Initialize();
		void __DirectEnterMode_Set(UINT uChrSlotIndex);
		bool __DirectEnterMode_IsSet();

	public:
		DWORD EXPORT_GetBettingGuildWarValue(const char* c_szValueName);

	private:
		struct SBettingGuildWar
		{
			DWORD m_dwBettingMoney;
			DWORD m_dwObserverCount;
		} m_kBettingGuildWar;

		CInstanceBase * m_pInstTarget;

		void __BettingGuildWar_Initialize();
		void __BettingGuildWar_SetObserverCount(UINT uObserverCount);
		void __BettingGuildWar_SetBettingMoney(UINT uBettingMoney);
#ifdef ENABLE_CUBE_RENEWAL
public:
	bool CubeRenewalMakeItem(int index_item, int count_item, int index_item_improve);
	bool CubeRenewalClose();
	bool RecvCubeRenewalPacket();
#endif
#ifdef ENABLE_SWITCHBOT
	public:
		bool RecvSwitchbotPacket();

		bool SendSwitchbotStartPacket(BYTE slot, std::vector<CPythonSwitchbot::TSwitchbotAttributeAlternativeTable> alternatives);
		bool SendSwitchbotStopPacket(BYTE slot);
#endif
};

