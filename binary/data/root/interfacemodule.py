##
## Interface
##
import constInfo
import uikygnitemsil
import uimaintenanceadmin
import systemSetting
import wndMgr
import chat
import app
import player
import uiTaskBar
import uiCharacter
import uiInventory
import uiDragonSoul
import net
import uiChat
import uiMessenger
import guild
import uimyshopdeco
import ui
import uiHelp
import uiWhisper
import uiPointReset
import uiShop
import uiExchange
import uiSystem
import uiRestart
import uiToolTip
import uiMiniMap
import uiParty
import uiSafebox
import uiGuild
import uiQuest
import uiPrivateShopBuilder
import uiCommon
import uiRefine
import uiEquipmentDialog
import uiGameButton
import uiTip
import uiCube
import uiOfflineShop
import uiOfflineShopBuilder
import uiCards
import miniMap
# ACCESSORY_REFINE_ADD_METIN_STONE
import uiselectitem
# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE
import uiScriptLocale
import uibiyolog
import uiDungeonTimer
import event
import localeInfo
import uinpcekran
import uistonesell
import uigosterge
import uibattlepass
import uiWonExchange
if app.LWT_BUFF_UPDATE:
	import uiBuffiSkill
	import uiBuffiSkillv2
if app.OTOMATIK_AV:
	import uiOtomatikAv
if app.AUTO_SHOUT:
	import uishout
if app.ENABLE_ITEM_SHOP_SYSTEM:
	import uiItemShop
if app.LINK_IN_CHAT:
	import os
if app.ENABLE_SPECIAL_STORAGE:
	import uiSpecialStorage
if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
	import uiPrivateShopSearch
if app.ENABLE_SHOW_CHEST_DROP:
	import uichestdrop	
IsQBHide = 0
if app.ENABLE_SASH_SYSTEM:
	import uisash
if app.ENABLE_AURA_SYSTEM:
	import uiaura
if app.ENABLE_CUBE_RENEWAL:
	import uicuberenewal
if app.ENABLE_SWITCHBOT:
	import uiSwitchbot
class Interface(object):
	CHARACTER_STATUS_TAB = 1
	CHARACTER_SKILL_TAB = 2


	class chatackapa(ui.Window):
		def __init__(self, parent = None, x = 0, y = 0):
			ui.Window.__init__(self)
			self.parent = parent
			self.x = x
			self.y = y
			self.ColorValue = 0xff40EF37
			self.show = self.checkBox(x,y)
			self.Show()


		def checkBox(self, x,y):
			checkBox = ui.CheckBoxIbo()
			checkBox.SetParent(self.parent)
			checkBox.SetPosition(x, y)
			checkBox.SetEvent(ui.__mem_func__(self.__OnClickCheckBox), "ON_CHECK", True)
			checkBox.SetEvent(ui.__mem_func__(self.__OnClickCheckBox), "ON_UNCKECK", False)
			checkBox.SetCheckStatus(systemSetting.IsViewChat())
			checkBox.SetTextInfo("Chat")
			checkBox.Show()
			return checkBox

		def __OnClickCheckBox(self, checkType, autoFlag):
			systemSetting.SetViewChatFlag(autoFlag)
			
	def __init__(self):
		systemSetting.SetInterfaceHandler(self)
		self.windowOpenPosition = 0
		self.dlgWhisperWithoutTarget = None
		self.inputDialog = None
		self.offlineDecoration = None
		self.tipBoard = None
		self.bigBoard = None

		# ITEM_MALL
		self.mallPageDlg = None
		# END_OF_ITEM_MALL

		if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
			self.wndPrivateShopSearch = uiPrivateShopSearch.PrivateShopSeachWindow()
		self.wndWeb = None
		self.wndTaskBar = None
		self.wndCharacter = None
		self.wndInventory = None
		self.wndExpandedTaskBar = None
		self.wndExpandedMoneyTaskbar = None
		self.wndDragonSoul = None
		self.wndDragonSoulRefine = None
		self.wndChat = None
		self.ChatKapat = None
		self.wndMessenger = None
		self.wndMiniMap = None
		self.wndGuild = None
		self.npcekran = None
		self.stonesell = None
		self.wndGuildBuilding = None
		if app.LINK_IN_CHAT:
			self.OpenLinkQuestionDialog = None
		if app.ENABLE_SPECIAL_STORAGE:
			self.wndSpecialStorage = None
		if app.ENABLE_SHOW_CHEST_DROP:
			self.dlgChestDrop = None	
		if app.ENABLE_CUBE_RENEWAL:
			self.wndCubeRenewal = None
		if app.ENABLE_SWITCHBOT:
			self.wndSwitchbot = None
		# OFFLINE_SHOP
		self.wndOfflineShopAdminPanel = None
		# END_OF_OFFLINE_SHOP
		self.npcekran = uinpcekran.NPCEkran()
		self.npcekran.Hide()
		self.stonesell = uistonesell.StoneSell()
		self.stonesell.Hide()
		self.listGMName = {}
		self.wndKygnItemSil = None
		self.wndQuestWindow = {}
		self.wndQuestWindowNewKey = 0
		self.privateShopAdvertisementBoardDict = {}
		self.guildScoreBoardDict = {}
		self.equipmentDialogDict = {}
		if app.OTOMATIK_AV:
			self.wndOtoAv = None

		self.offlineShopAdvertisementBoardDict = {}
		event.SetInterfaceWindow(self)

	def __del__(self):
		systemSetting.DestroyInterfaceHandler()
		event.SetInterfaceWindow(None)
	def SetStream(self, stream):
		self.stream = stream	
		
	def NPCAC(self):
		if self.npcekran.IsShow():
			self.npcekran.Hide()	
		else:
			self.npcekran.Show()		

	if app.AUTO_SHOUT:
		def __MakeShoutWindow(self):
			self.wndShout = uishout.ShoutManager()
			self.wndShout.LoadWindow()
			self.wndShout.Hide()
			
	def StoneSell(self):
		if self.stonesell.IsShow():
			self.stonesell.Hide()	
		else:
			self.stonesell.Show()					
			
	################################
	## Make Windows & Dialogs
	def __MakeUICurtain(self):
		wndUICurtain = ui.Bar("TOP_MOST")
		wndUICurtain.SetSize(wndMgr.GetScreenWidth(), wndMgr.GetScreenHeight())
		wndUICurtain.SetColor(0x77000000)
		wndUICurtain.Hide()
		self.wndUICurtain = wndUICurtain

	def __MakeMessengerWindow(self):
		self.wndMessenger = uiMessenger.MessengerWindow()

		from _weakref import proxy
		self.wndMessenger.SetWhisperButtonEvent(lambda n,i=proxy(self):i.OpenWhisperDialog(n))
		self.wndMessenger.SetGuildButtonEvent(ui.__mem_func__(self.ToggleGuildWindow))

	def __MakeGuildWindow(self):
		self.wndGuild = uiGuild.GuildWindow()

	def MaintenanceAdmin(self):
		self.dlgMaintenanceAdmin.OpenDialog()

	def __MakeChatWindow(self):
		
		wndChat = uiChat.ChatWindow()
		
		wndChat.SetSize(wndChat.CHAT_WINDOW_WIDTH, 0)
		wndChat.SetPosition(wndMgr.GetScreenWidth()/2 - wndChat.CHAT_WINDOW_WIDTH/2, wndMgr.GetScreenHeight() - wndChat.EDIT_LINE_HEIGHT - 37)
		wndChat.SetHeight(200)
		wndChat.Refresh()
		wndChat.Show()

		self.wndChat = wndChat
		self.wndChat.BindInterface(self)
		self.wndChat.SetSendWhisperEvent(ui.__mem_func__(self.OpenWhisperDialogWithoutTarget))
		self.wndChat.SetOpenChatLogEvent(ui.__mem_func__(self.ToggleChatLogWindow))
		if self.ChatKapat:
			self.ChatKapat.Hide()
		ChatKapat = self.chatackapa(None, wndMgr.GetScreenWidth()/2 - wndChat.CHAT_WINDOW_WIDTH/2 - 45, wndMgr.GetScreenHeight() - wndChat.EDIT_LINE_HEIGHT - 35 + 9)
		self.ChatKapat = ChatKapat
	def __MakeTaskBar(self):
		wndTaskBar = uiTaskBar.TaskBar()
		wndTaskBar.LoadWindow()
		self.wndTaskBar = wndTaskBar
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_CHARACTER, ui.__mem_func__(self.ToggleCharacterWindowStatusPage))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_INVENTORY, ui.__mem_func__(self.ToggleInventoryWindow))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_MESSENGER, ui.__mem_func__(self.ToggleMessenger))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_SYSTEM, ui.__mem_func__(self.ToggleSystemDialog))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_EXP, ui.__mem_func__(self.ToggleExp))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_DELETE_SELL, ui.__mem_func__(self.ToggleDeleteSellButton))
		if uiTaskBar.TaskBar.IS_EXPANDED:
			self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_EXPAND, ui.__mem_func__(self.ToggleExpandedButton))
			self.wndExpandedTaskBar = uiTaskBar.ExpandedTaskBar()
			self.wndExpandedTaskBar.LoadWindow()
			self.wndExpandedTaskBar.SetToggleButtonEvent(uiTaskBar.ExpandedTaskBar.BUTTON_DRAGON_SOUL, ui.__mem_func__(self.ToggleDragonSoulWindow))

		else:
			self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_CHAT, ui.__mem_func__(self.ToggleChat))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_EXPAND_MONEY, ui.__mem_func__(self.ToggleExpandedMoneyButton))

		self.wndExpandedMoneyTaskbar = uiTaskBar.ExpandedMoneyTaskBar()
		self.wndExpandedMoneyTaskbar.LoadWindow()
        
		self.wndEnergyBar = None
		import app
		if app.ENABLE_ENERGY_SYSTEM:
			wndEnergyBar = uiTaskBar.EnergyBar()
			wndEnergyBar.LoadWindow()
			self.wndEnergyBar = wndEnergyBar	

		if app.ENABLE_ITEM_SHOP_SYSTEM:
			self.wndTaskBar.BindInterface(self)



	def __MakeBiyologWindow(self):
		self.wndBiyologTable = uibiyolog.BiyologEkran()
		self.wndBiyologTable.LoadWindow()
		self.wndBiyologTable.Hide()
	def __MakeParty(self):
		wndParty = uiParty.PartyWindow()
		wndParty.Hide()
		self.wndParty = wndParty

	def __MakeGameButtonWindow(self):
		wndGameButton = uiGameButton.GameButtonWindow()
		wndGameButton.SetTop()
		wndGameButton.Show()
		wndGameButton.SetButtonEvent("STATUS", ui.__mem_func__(self.__OnClickStatusPlusButton))
		wndGameButton.SetButtonEvent("SKILL", ui.__mem_func__(self.__OnClickSkillPlusButton))
		wndGameButton.SetButtonEvent("QUEST", ui.__mem_func__(self.__OnClickQuestButton))
		wndGameButton.SetButtonEvent("HELP", ui.__mem_func__(self.__OnClickHelpButton))
		wndGameButton.SetButtonEvent("BUILD", ui.__mem_func__(self.__OnClickBuildButton))

		self.wndGameButton = wndGameButton

	def __IsChatOpen(self):
		return True
		
	def __MakeWindows(self):
		wndCharacter = uiCharacter.CharacterWindow()
		wndInventory = uiInventory.InventoryWindow()
		wndInventory.BindInterfaceClass(self)
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			wndDragonSoul = uiDragonSoul.DragonSoulWindow()	
			wndDragonSoul.BindInterfaceClass(self)
			wndDragonSoulRefine = uiDragonSoul.DragonSoulRefineWindow()
		else:
			wndDragonSoul = None
			wndDragonSoulRefine = None
 
		wndMiniMap = uiMiniMap.MiniMap()
		wndSafebox = uiSafebox.SafeboxWindow()
		
		# ITEM_MALL
		wndMall = uiSafebox.MallWindow()
		self.wndMall = wndMall
		# END_OF_ITEM_MALL

		wndChatLog = uiChat.ChatLogWindow()
		wndChatLog.BindInterface(self)

		self.wndCharacter = wndCharacter
		self.wndInventory = wndInventory
		self.wndDragonSoul = wndDragonSoul
		self.wndDragonSoulRefine = wndDragonSoulRefine
		self.wndMiniMap = wndMiniMap
		self.wndSafebox = wndSafebox
		self.wndChatLog = wndChatLog
		if app.OTOMATIK_AV:
			self.wndOtoAv = uiOtomatikAv.AutoWindow()
		self.wndKygnItemSil = uikygnitemsil.KygnItemSil()
		self.wndWonExchange = uiWonExchange.WonExchangeWindow()
		self.wndWonExchange.BindInterface(self)
		
		if app.ENABLE_SHOW_CHEST_DROP:
			self.dlgChestDrop = uichestdrop.ChestDropWindow()
		
		offlineDecoration = uimyshopdeco.UiShopDeco()
		self.offlineDecoration = offlineDecoration
		
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.SetDragonSoulRefineWindow(self.wndDragonSoulRefine)
			self.wndDragonSoulRefine.SetInventoryWindows(self.wndInventory, self.wndDragonSoul)
			self.wndInventory.SetDragonSoulRefineWindow(self.wndDragonSoulRefine)
		if app.ITEM_CHECKINOUT_UPDATE:
			self.wndInventory.SetSafeboxWindow(self.wndSafebox)

		if app.ENABLE_SWITCHBOT:
			self.wndSwitchbot = uiSwitchbot.SwitchbotWindow()

		if app.ENABLE_SPECIAL_STORAGE:
			self.wndSpecialStorage = uiSpecialStorage.SpecialStorageWindow()
			self.wndSpecialStorage.BindInterfaceClass(self)
		else:
			self.wndSpecialStorage = None
	def __MakeDialogs(self):
		self.dlgExchange = uiExchange.ExchangeDialog()
		self.dlgExchange.LoadDialog()
		self.dlgExchange.SetCenterPosition()
		self.dlgExchange.Hide()
		if app.LWT_BUFF_UPDATE:
			self.dlgBuffiSkill = uiBuffiSkill.BuffiSkillDialog()
			self.dlgBuffiSkill.LoadDialog()
			self.dlgBuffiSkill.SetCenterPosition()
			self.dlgBuffiSkill.Hide()
			
			self.dlgBuffiSkillv2 = uiBuffiSkillv2.BuffiSkillDialogIyilestirme()
			self.dlgBuffiSkillv2.LoadDialog()
			self.dlgBuffiSkillv2.SetCenterPosition()
			self.dlgBuffiSkillv2.Hide()


		self.dlgPointReset = uiPointReset.PointResetDialog()
		self.dlgPointReset.LoadDialog()
		self.dlgPointReset.Hide()

		self.dlgShop = uiShop.ShopDialog()
		self.dlgShop.LoadDialog()
		self.dlgShop.Hide()

		self.dlgMaintenanceAdmin = uimaintenanceadmin.MaintenanceAdminWindow()
		self.dlgMaintenanceAdmin.LoadDialog()
		self.dlgMaintenanceAdmin.Hide()


		self.dlgRestart = uiRestart.RestartDialog()

		self.dlgRestart = uiRestart.RestartDialog()
		self.dlgRestart.LoadDialog()
		self.dlgRestart.Hide()

		self.dlgSystem = uiSystem.SystemDialog(self.stream)

		self.dlgSystem.LoadDialog()
		self.dlgSystem.SetOpenHelpWindowEvent(ui.__mem_func__(self.OpenHelpWindow))

		self.dlgSystem.Hide()

		self.dlgPassword = uiSafebox.PasswordDialog()
		self.dlgPassword.Hide()

		self.hyperlinkItemTooltip = uiToolTip.HyperlinkItemToolTip()
		self.hyperlinkItemTooltip.Hide()

		self.tooltipItem = uiToolTip.ItemToolTip()
		if app.ENABLE_DS_SET:
			self.tooltipItem.BindInterface(self)
		self.tooltipItem.Hide()

		if app.ENABLE_ITEM_SHOP_SYSTEM:
			self.ItemShop = uiItemShop.ItemShopWindow(self)
			self.ItemShop.LoadWindow()
			self.ItemShop.Close()

		self.tooltipSkill = uiToolTip.SkillToolTip()
		self.tooltipSkill.Hide()

		self.privateShopBuilder = uiPrivateShopBuilder.PrivateShopBuilder()
		self.privateShopBuilder.Hide()

		self.offlineShopBuilder = uiOfflineShopBuilder.OfflineShopBuilder()
		self.offlineShopBuilder.Hide()	
		self.dlgRefineNew = uiRefine.RefineDialogNew()
		self.dlgRefineNew.Hide()

		self.itemgui = uigosterge.SrcItemWindow()
		
		
	def __MakeHelpWindow(self):
		self.wndHelp = uiHelp.HelpWindow()
		self.wndHelp.LoadDialog()
		self.wndHelp.SetCloseEvent(ui.__mem_func__(self.CloseHelpWindow))
		self.wndHelp.Hide()

	def __MakeTipBoard(self):
		self.tipBoard = uiTip.TipBoard()
		self.tipBoard.Hide()

		self.bigBoard = uiTip.BigBoard()
		self.bigBoard.Hide()

	def __MakeWebWindow(self):
		if constInfo.IN_GAME_SHOP_ENABLE:
			import uiWeb
			self.wndWeb = uiWeb.WebWindow()
			self.wndWeb.LoadWindow()
			self.wndWeb.Hide()
	if app.ENABLE_SASH_SYSTEM:
		def __MakeSashWindow(self):
			self.wndSashCombine = uisash.CombineWindow()
			self.wndSashCombine.LoadWindow()
			self.wndSashCombine.Hide()
			
			self.wndSashAbsorption = uisash.AbsorbWindow()
			self.wndSashAbsorption.LoadWindow()
			self.wndSashAbsorption.Hide()
			
			if self.wndInventory:
				self.wndInventory.SetSashWindow(self.wndSashCombine, self.wndSashAbsorption)

	if app.ENABLE_AURA_SYSTEM:
		def __MakeAuraWindow(self):
			self.wndAuraRefine = uiaura.RefineWindow(self.wndInventory, self)
			self.wndAuraRefine.LoadWindow()
			self.wndAuraRefine.Hide()
	
			self.wndAuraAbsorption = uiaura.AbsorbWindow(self.wndInventory, self)
			self.wndAuraAbsorption.LoadWindow()
			self.wndAuraAbsorption.Hide()
			
			if self.wndInventory:
				self.wndInventory.SetAuraWindow(self.wndAuraAbsorption, self.wndAuraRefine)
	if app.ENABLE_CUBE_RENEWAL:
		def __MakeCubeRenewal(self):
			self.wndCubeRenewal = uicuberenewal.CubeRenewalWindows()
			self.wndCubeRenewal.Hide()
			self.wndCubeRenewal.BindInterfaceClass(self)
			if self.wndInventory:
				self.wndInventory.SetCubeRenewalDlg(self.wndCubeRenewal)
			if app.ENABLE_SPECIAL_STORAGE:
				if self.wndSpecialStorage:
					self.wndSpecialStorage.SetCubeRenewalDlg(self.wndCubeRenewal)

				
	def __MakeCubeWindow(self):
		self.wndCube = uiCube.CubeWindow()
		self.wndCube.LoadWindow()
		self.wndCube.Hide()

	def __MakeCubeResultWindow(self):
		self.wndCubeResult = uiCube.CubeResultWindow()
		self.wndCubeResult.LoadWindow()
		self.wndCubeResult.Hide()
		
	def __BoardBpass(self):
		self.wndbpass = uibattlepass.Battlepass()
		self.wndbpass.LoadWindow()
		self.wndbpass.Hide()

	def __MakeCardsInfoWindow(self):
		self.wndCardsInfo = uiCards.CardsInfoWindow()
		self.wndCardsInfo.LoadWindow()
		self.wndCardsInfo.Hide()
		
	def __MakeCardsWindow(self):
		self.wndCards = uiCards.CardsWindow()
		self.wndCards.LoadWindow()
		self.wndCards.Hide()
		
	def __MakeCardsIconWindow(self):
		self.wndCardsIcon = uiCards.IngameWindow()
		self.wndCardsIcon.LoadWindow()
		self.wndCardsIcon.Hide()
		
	if app.WJ_SECURITY_SYSTEM:
		def __MakeSecurityWindow(self):
			self.wndSecurityWindow = uiSafebox.SecurityDialog()
			self.wndSecurityWindow.Hide()
		
	def __MakeDungeonTimerWindow(self):
		self.wndDungeonTimer = uiDungeonTimer.DungeonTimer()
		self.wndDungeonTimer.LoadWindow()
		self.wndDungeonTimer.Hide()

	# ACCESSORY_REFINE_ADD_METIN_STONE
	def __MakeItemSelectWindow(self):
		self.wndItemSelect = uiselectitem.SelectItemWindow()
		self.wndItemSelect.Hide()
	# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE
				
	def MakeInterface(self):
		self.__MakeMessengerWindow()
		self.__MakeGuildWindow()
		self.__MakeChatWindow()
		self.__MakeParty()
		self.__MakeWindows()
		self.__MakeDialogs()

		self.__MakeUICurtain()
		self.__MakeTaskBar()
		self.__MakeGameButtonWindow()
		self.__MakeHelpWindow()
		self.__MakeTipBoard()
		self.__MakeWebWindow()
		if app.ENABLE_SASH_SYSTEM:
			self.__MakeSashWindow()
		if app.ENABLE_AURA_SYSTEM:
			self.__MakeAuraWindow()
		self.__MakeCubeWindow()
		self.__MakeDungeonTimerWindow()
		self.__MakeCubeResultWindow()
		self.__BoardBpass()
		self.__MakeCardsInfoWindow()
		self.__MakeCardsWindow()
		self.__MakeCardsIconWindow()
		if app.AUTO_SHOUT:
			self.__MakeShoutWindow()
		if app.WJ_SECURITY_SYSTEM:
			self.__MakeSecurityWindow()
		self.__MakeBiyologWindow()
		
		if app.ENABLE_CUBE_RENEWAL:
			self.__MakeCubeRenewal()
		# ACCESSORY_REFINE_ADD_METIN_STONE
		self.__MakeItemSelectWindow()
		# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE

		self.questButtonList = []
		self.whisperButtonList = []
		self.whisperDialogDict = {}
		self.privateShopAdvertisementBoardDict = {}

		self.wndInventory.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_ITEM_SHOP_SYSTEM:
			self.ItemShop.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.SetItemToolTip(self.tooltipItem)
			self.wndDragonSoulRefine.SetItemToolTip(self.tooltipItem)
		self.wndSafebox.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_SASH_SYSTEM:
			self.wndSashCombine.SetItemToolTip(self.tooltipItem)
			self.wndSashAbsorption.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_AURA_SYSTEM:
			self.wndAuraAbsorption.SetItemToolTip(self.tooltipItem)
			self.wndAuraRefine.SetItemToolTip(self.tooltipItem)
		self.wndCube.SetItemToolTip(self.tooltipItem)
		self.wndCubeResult.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_SPECIAL_STORAGE:
			self.wndSpecialStorage.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
			self.wndPrivateShopSearch.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_SWITCHBOT:
			self.wndSwitchbot.SetItemToolTip(self.tooltipItem)
		# ITEM_MALL
		self.wndMall.SetItemToolTip(self.tooltipItem)
		# END_OF_ITEM_MALL

		self.wndCharacter.SetSkillToolTip(self.tooltipSkill)
		self.wndTaskBar.SetItemToolTip(self.tooltipItem)
		self.wndTaskBar.SetSkillToolTip(self.tooltipSkill)
		self.wndGuild.SetSkillToolTip(self.tooltipSkill)
		if app.LWT_BUFF_UPDATE:
			self.dlgBuffiSkill.SetSkillToolTip(self.tooltipSkill)	
			self.dlgBuffiSkillv2.SetSkillToolTip(self.tooltipSkill)	

		# ACCESSORY_REFINE_ADD_METIN_STONE
		self.wndItemSelect.SetItemToolTip(self.tooltipItem)
		# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE

		if app.ENABLE_SHOW_CHEST_DROP:
			self.dlgChestDrop.SetItemToolTip(self.tooltipItem)
		
		self.dlgShop.SetItemToolTip(self.tooltipItem)
		self.dlgExchange.SetItemToolTip(self.tooltipItem)
		self.privateShopBuilder.SetItemToolTip(self.tooltipItem)
		if app.OTOMATIK_AV:
			self.wndOtoAv.SetSkillToolTip(self.tooltipSkill)
			self.wndOtoAv.SetItemToolTip(self.tooltipItem)
		self.itemgui.SetItemToolTip(self.tooltipItem)
		self.__InitWhisper()
		self.DRAGON_SOUL_IS_QUALIFIED = False

	if app.LINK_IN_CHAT:
		def AnswerOpenLink(self, answer):
			if not self.OpenLinkQuestionDialog:
				return

			self.OpenLinkQuestionDialog.Close()
			self.OpenLinkQuestionDialog = None

			if not answer:
				return

			link = constInfo.link
			os.system(link)		

	def MakeHyperlinkTooltip(self, hyperlink):
		tokens = hyperlink.split(":")
		if tokens and len(tokens):
			type = tokens[0]
			if "item" == type:
				self.hyperlinkItemTooltip.SetHyperlinkItem(tokens)
			elif app.LINK_IN_CHAT and "web" == type and tokens[1].startswith("httpXxX") or "web" == type and tokens[1].startswith("httpsXxX"):
					OpenLinkQuestionDialog = uiCommon.QuestionDialog2()
					OpenLinkQuestionDialog.SetText1(localeInfo.CHAT_OPEN_LINK_DANGER)
					OpenLinkQuestionDialog.SetText2(localeInfo.CHAT_OPEN_LINK)
					OpenLinkQuestionDialog.SetAcceptEvent(lambda arg=TRUE: self.AnswerOpenLink(arg))
					OpenLinkQuestionDialog.SetCancelEvent(lambda arg=FALSE: self.AnswerOpenLink(arg))
					constInfo.link = "start " + tokens[1].replace("XxX", "://").replace("&","^&")
					OpenLinkQuestionDialog.Open()
					self.OpenLinkQuestionDialog = OpenLinkQuestionDialog
			elif app.LINK_IN_CHAT and "sysweb" == type:
				os.system("start " + tokens[1].replace("XxX", "://"))
			elif "pm_button" == type:
				if player.GetName() == tokens[1]:
					chat.AppendChat(chat.CHAT_TYPE_INFO, "<Sistem> kendine pm atamazsin.")
					return
				self.OpenWhisperDialog(str(tokens[1]))

	## Make Windows & Dialogs
	################################

	def Close(self):
		if self.dlgWhisperWithoutTarget:
			self.dlgWhisperWithoutTarget.Destroy()
			del self.dlgWhisperWithoutTarget

		if self.wndWonExchange:
			self.wndWonExchange.Destroy()
		#[...] a bit below
			del self.wndWonExchange

		if uiQuest.QuestDialog.__dict__.has_key("QuestCurtain"):
			uiQuest.QuestDialog.QuestCurtain.Close()

		if self.wndQuestWindow:
			for key, eachQuestWindow in self.wndQuestWindow.items():
				eachQuestWindow.nextCurtainMode = -1
				eachQuestWindow.CloseSelf()
				eachQuestWindow = None
		self.wndQuestWindow = {}

		if self.wndChat:
			self.wndChat.Destroy()
			
		if self.ChatKapat:
			self.ChatKapat.Destroy()
			
		if self.wndCharacter:
			self.wndCharacter.Close()
		else:
			if self.wndCharacter:
				self.wndCharacter.Hide()

		if self.wndTaskBar:
			self.wndTaskBar.Destroy()
		
		if self.wndExpandedTaskBar:
			self.wndExpandedTaskBar.Destroy()

		if app.LWT_BUFF_UPDATE and self.dlgBuffiSkill:
			self.dlgBuffiSkill.Destroy()
		if app.LWT_BUFF_UPDATE and self.dlgBuffiSkillv2:
			self.dlgBuffiSkillv2.Destroy()
		if self.wndExpandedMoneyTaskbar:
			self.wndExpandedMoneyTaskbar.Destroy()
			
		if self.wndEnergyBar:
			self.wndEnergyBar.Destroy()

		if self.wndCharacter:
			self.wndCharacter.Destroy()

		if self.wndInventory:
			self.wndInventory.Destroy()
			
		if self.wndDragonSoul:
			self.wndDragonSoul.Destroy()

		if self.wndDragonSoulRefine:
			self.wndDragonSoulRefine.Destroy()

		if app.ENABLE_ITEM_SHOP_SYSTEM:
			if self.ItemShop:
				self.ItemShop.Destroy()
				self.ItemShop.Close()
				self.ItemShop.Destroy()
				self.ItemShop = None
				del self.ItemShop

		if app.ENABLE_SPECIAL_STORAGE:
			if self.wndSpecialStorage:
				self.wndSpecialStorage.Destroy()
		if app.ENABLE_CUBE_RENEWAL:
			if self.wndCubeRenewal:
				self.wndCubeRenewal.Hide()
				self.wndCubeRenewal.Destroy()
				self.wndCubeRenewal = None
				del self.wndCubeRenewal
		if self.dlgExchange:
			self.dlgExchange.Destroy()

		if self.dlgPointReset:
			self.dlgPointReset.Destroy()

		if self.dlgShop:
			self.dlgShop.Destroy()

		if self.dlgMaintenanceAdmin:
			self.dlgMaintenanceAdmin.Destroy()

		if self.dlgRestart:
			self.dlgRestart.Destroy()

		if self.dlgSystem:
			self.dlgSystem.Destroy()

		if self.dlgPassword:
			self.dlgPassword.Destroy()

		if self.wndMiniMap:
			self.wndMiniMap.Destroy()

		if self.wndSafebox:
			self.wndSafebox.Destroy()

		if self.wndWeb:
			self.wndWeb.Destroy()
			self.wndWeb = None

		if self.wndMall:
			self.wndMall.Destroy()

		if self.wndParty:
			self.wndParty.Destroy()
		if self.wndBiyologTable:
			self.wndBiyologTable.Destroy()

		if self.wndHelp:
			self.wndHelp.Destroy()

		if self.wndbpass:
			self.wndbpass.Destroy()
			
		if self.wndCardsInfo:
			self.wndCardsInfo.Destroy()

		if self.wndCards:
			self.wndCards.Destroy()

		if self.wndCardsIcon:
			self.wndCardsIcon.Destroy()
			
		if app.ENABLE_SASH_SYSTEM:
			if self.wndSashCombine:
				self.wndSashCombine.Destroy()
			
			if self.wndSashAbsorption:
				self.wndSashAbsorption.Destroy()	
		if app.ENABLE_AURA_SYSTEM:
			if self.wndAuraAbsorption:
				self.wndAuraAbsorption.Destroy()

			if self.wndAuraRefine:
				self.wndAuraRefine.Destroy()
		if self.wndDungeonTimer:
			self.wndDungeonTimer.Destroy()
		if self.wndCube:
			self.wndCube.Destroy()
			
		if self.wndCubeResult:
			self.wndCubeResult.Destroy()
		if app.WJ_SECURITY_SYSTEM:
			if self.wndSecurityWindow:
				self.wndSecurityWindow.Destroy()

		if self.wndMessenger:
			self.wndMessenger.Destroy()

		if self.wndGuild:
			self.wndGuild.Destroy()
		if app.AUTO_SHOUT:
			if self.wndShout:
				self.wndShout.Destroy()
		if self.wndKygnItemSil:
			self.wndKygnItemSil.Destroy()

		if self.privateShopBuilder:
			self.privateShopBuilder.Destroy()

		if self.dlgRefineNew:
			self.dlgRefineNew.Destroy()

		if self.wndGuildBuilding:
			self.wndGuildBuilding.Destroy()

		if self.wndGameButton:
			self.wndGameButton.Destroy()

		# ITEM_MALL
		if self.mallPageDlg:
			self.mallPageDlg.Destroy()
		# END_OF_ITEM_MALL
		
		if app.ENABLE_SHOW_CHEST_DROP:
			if self.dlgChestDrop:
				self.dlgChestDrop.Destroy()

		if app.ENABLE_SWITCHBOT:
			if self.wndSwitchbot:
				self.wndSwitchbot.Destroy()

		# ACCESSORY_REFINE_ADD_METIN_STONE
		if self.wndItemSelect:
			self.wndItemSelect.Destroy()
		if app.OTOMATIK_AV:
			if self.wndOtoAv:
				self.wndOtoAv.Destroy()
		# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE


			
		# OFFLINE_SHOP_ADMIN_PANEL
		if (self.wndOfflineShopAdminPanel):
			self.wndOfflineShopAdminPanel.Destroy()
		# END_OF_OFFLINE_SHOP_ADMIN_PANEL

		if self.itemgui:
			self.itemgui.Close()

		if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
			if self.wndPrivateShopSearch:
				self.wndPrivateShopSearch.Destroy()
				del self.wndPrivateShopSearch
	
		
		self.wndChatLog.Destroy()
		for btn in self.questButtonList:
			btn.SetEvent(0)
		for btn in self.whisperButtonList:
			btn.SetEvent(0)
		for dlg in self.whisperDialogDict.itervalues():
			dlg.Destroy()
		for brd in self.guildScoreBoardDict.itervalues():
			brd.Destroy()
		for dlg in self.equipmentDialogDict.itervalues():
			dlg.Destroy()

		# ITEM_MALL
		del self.mallPageDlg
		# END_OF_ITEM_MALL

		del self.wndGuild
		del self.wndKygnItemSil
		del self.wndMessenger
		del self.wndUICurtain
		del self.wndChat
		del self.ChatKapat
		del self.wndTaskBar
		if self.wndExpandedTaskBar:
			del self.wndExpandedTaskBar
		if self.wndExpandedMoneyTaskbar:
			del self.wndExpandedMoneyTaskbar
		del self.wndEnergyBar
		del self.wndCharacter
		del self.wndInventory
		if self.wndDragonSoul:
			del self.wndDragonSoul
		if self.wndDragonSoulRefine:
			del self.wndDragonSoulRefine
		if app.LWT_BUFF_UPDATE:
			del self.dlgBuffiSkill
			del self.dlgBuffiSkillv2
		if app.ENABLE_SPECIAL_STORAGE:
			if self.wndSpecialStorage:
				del self.wndSpecialStorage
		del self.dlgExchange
		del self.dlgPointReset
		del self.dlgShop
		del self.dlgMaintenanceAdmin
		del self.dlgRestart
		del self.dlgSystem
		del self.dlgPassword
		del self.hyperlinkItemTooltip
		del self.tooltipItem
		del self.tooltipSkill
		del self.wndMiniMap
		del self.npcekran
		del self.stonesell
		del self.wndSafebox
		del self.wndMall
		del self.wndParty
		del self.wndBiyologTable
		del self.wndHelp
		del self.wndCardsInfo
		del self.wndCards
		del self.wndCardsIcon
		if app.AUTO_SHOUT:
			del self.wndShout
			
		if app.ENABLE_SASH_SYSTEM:
			del self.wndSashCombine
			del self.wndSashAbsorption
		if app.ENABLE_AURA_SYSTEM:
			del self.wndAuraAbsorption
			del self.wndAuraRefine
		del self.wndCube
		del self.wndDungeonTimer
		del self.wndCubeResult
		if app.WJ_SECURITY_SYSTEM:
			del self.wndSecurityWindow
		del self.privateShopBuilder
		del self.inputDialog
		del self.offlineDecoration
		del self.wndChatLog
		del self.dlgRefineNew
		del self.wndGuildBuilding
		del self.wndGameButton
		del self.wndbpass
		del self.tipBoard
		del self.bigBoard
		del self.wndItemSelect
		if app.OTOMATIK_AV:
			del self.wndOtoAv
		del self.itemgui
		
		if app.ENABLE_SWITCHBOT:
			del self.wndSwitchbot	
		
		if app.ENABLE_SHOW_CHEST_DROP:
			if self.dlgChestDrop:
				del self.dlgChestDrop


		self.questButtonList = []
		
		self.whisperButtonList = []
		self.whisperDialogDict = {}
		self.privateShopAdvertisementBoardDict = {}
		self.guildScoreBoardDict = {}
		self.equipmentDialogDict = {}
		self.offlineShopAdvertisementBoardDict = {}

		uiChat.DestroyChatInputSetWindow()

	def SetBiyolog(self, bioitem, verilen, toplam, kalansure, ruhtasi, hediye, ruhtami):
		self.wndBiyologTable.SetBiyolog(bioitem, verilen, toplam, kalansure, ruhtasi, hediye, ruhtami)
	## Skill
	def OnUseSkill(self, slotIndex, coolTime):
		self.wndCharacter.OnUseSkill(slotIndex, coolTime)
		self.wndTaskBar.OnUseSkill(slotIndex, coolTime)
		self.wndGuild.OnUseSkill(slotIndex, coolTime)
		if app.OTOMATIK_AV:
			self.wndOtoAv.OnUseSkill(slotIndex,coolTime)
	def OnActivateSkill(self, slotIndex):
		self.wndCharacter.OnActivateSkill(slotIndex)
		self.wndTaskBar.OnActivateSkill(slotIndex)
		if app.OTOMATIK_AV:
			self.wndOtoAv.OnActivateSkill()
	def OnDeactivateSkill(self, slotIndex):
		self.wndCharacter.OnDeactivateSkill(slotIndex)
		self.wndTaskBar.OnDeactivateSkill(slotIndex)
		if app.OTOMATIK_AV:
			self.wndOtoAv.OnDeactivateSkill(slotIndex)
	def OnChangeCurrentSkill(self, skillSlotNumber):
		self.wndTaskBar.OnChangeCurrentSkill(skillSlotNumber)

	def SelectMouseButtonEvent(self, dir, event):
		self.wndTaskBar.SelectMouseButtonEvent(dir, event)

	## Refresh
	def RefreshAlignment(self):
		self.wndCharacter.RefreshAlignment()

	def RefreshStatus(self):
		self.wndTaskBar.RefreshStatus()
		self.wndCharacter.RefreshStatus()
		if self.wndEnergyBar:
			self.wndEnergyBar.RefreshStatus()
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.RefreshStatus()
            
		if self.wndExpandedMoneyTaskbar:
			self.wndExpandedMoneyTaskbar.RefreshStatus()

	def RefreshStamina(self):
		self.wndTaskBar.RefreshStamina()

	def RefreshSkill(self):
		self.wndCharacter.RefreshSkill()
		self.wndTaskBar.RefreshSkill()
		if app.LWT_BUFF_UPDATE and self.dlgBuffiSkill:
			self.dlgBuffiSkill.RefreshSkillData()
		if app.LWT_BUFF_UPDATE and self.dlgBuffiSkillv2:
			self.dlgBuffiSkillv2.RefreshSkillData()
	def RefreshInventory(self):
		self.wndTaskBar.RefreshQuickSlot()
		self.wndInventory.RefreshItemSlot()
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.RefreshItemSlot()

		if app.ENABLE_SPECIAL_STORAGE:
			self.wndSpecialStorage.RefreshItemSlot()
		if app.ENABLE_SPECIAL_STORAGE:
			self.wndSpecialStorage.RefreshItemSlot()
	def RefreshCharacter(self): ## Character 페이지의 얼굴, Inventory 페이지의 전신 그림 등의 Refresh
		self.wndCharacter.RefreshCharacter()
		self.wndTaskBar.RefreshQuickSlot()

	def RefreshQuest(self):
		self.wndCharacter.RefreshQuest()

	def RefreshSafebox(self):
		self.wndSafebox.RefreshSafebox()

	# ITEM_MALL
	def RefreshMall(self):
		self.wndMall.RefreshMall()

	def OpenItemMall(self):
		if not self.mallPageDlg:
			self.mallPageDlg = uiShop.MallPageDialog()

		self.mallPageDlg.Open()
	# END_OF_ITEM_MALL

	def RefreshMessenger(self):
		self.wndMessenger.RefreshMessenger()

	def RefreshGuildInfoPage(self):
		self.wndGuild.RefreshGuildInfoPage()

	def RefreshGuildBoardPage(self):
		self.wndGuild.RefreshGuildBoardPage()

	def RefreshGuildMemberPage(self):
		self.wndGuild.RefreshGuildMemberPage()

	def RefreshGuildMemberPageGradeComboBox(self):
		self.wndGuild.RefreshGuildMemberPageGradeComboBox()

	def RefreshGuildSkillPage(self):
		self.wndGuild.RefreshGuildSkillPage()

	def RefreshGuildGradePage(self):
		self.wndGuild.RefreshGuildGradePage()

	def DeleteGuild(self):
		self.wndMessenger.ClearGuildMember()
		self.wndGuild.DeleteGuild()

	def RefreshMobile(self):
		self.dlgSystem.RefreshMobile()

	def OnMobileAuthority(self):
		self.dlgSystem.OnMobileAuthority()

	def OnBlockMode(self, mode):
		self.dlgSystem.OnBlockMode(mode)

	## Calling Functions
	# PointReset
	def OpenPointResetDialog(self):
		self.dlgPointReset.Show()
		self.dlgPointReset.SetTop()

	def ClosePointResetDialog(self):
		self.dlgPointReset.Close()

	# Shop
	if app.ENABLE_REMOTE_SHOP:
		def OpenShopDialog(self, vid, uzakmarket):
			if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
				return
			self.wndInventory.Show()
			self.wndInventory.SetTop()
			self.dlgShop.Open(vid, uzakmarket)
			self.dlgShop.SetTop()
	else:
		def OpenShopDialog(self, vid):
			if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
				return
			self.wndInventory.Show()
			self.wndInventory.SetTop()
			self.dlgShop.Open(vid)
			self.dlgShop.SetTop()

	def CloseShopDialog(self):
		self.dlgShop.Close()

	def RefreshShopDialog(self):
		self.dlgShop.Refresh()

	if app.ENABLE_SHOW_CHEST_DROP:
		def AddChestDropInfo(self, chestVnum, pageIndex, slotIndex, itemVnum, itemCount):
			self.dlgChestDrop.AddChestDropItem(int(chestVnum), int(pageIndex), int(slotIndex), int(itemVnum), int(itemCount))

		def RefreshChestDropInfo(self, chestVnum):
			if constInfo.ITEM_REMOVE_WINDOW_STATUS == 1:
				return
			self.dlgChestDrop.RefreshItems(chestVnum)

	## Quest
	def OpenCharacterWindowQuestPage(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		self.wndCharacter.Show()
		self.wndCharacter.SetState("QUEST")

	def OpenQuestWindow(self, skin, idx):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return

		wnds = ()

		q = uiQuest.QuestDialog(skin, idx)
		q.SetWindowName("QuestWindow" + str(idx))
		q.Show()
		if skin:
			q.Lock()
			wnds = self.__HideWindows()

			# UNKNOWN_UPDATE
			q.AddOnDoneEvent(lambda tmp_self, args=wnds: self.__ShowWindows(args))
			# END_OF_UNKNOWN_UPDATE

		if skin:
			q.AddOnCloseEvent(q.Unlock)
		q.AddOnCloseEvent(lambda key = self.wndQuestWindowNewKey:ui.__mem_func__(self.RemoveQuestDialog)(key))
		self.wndQuestWindow[self.wndQuestWindowNewKey] = q

		self.wndQuestWindowNewKey = self.wndQuestWindowNewKey + 1

		# END_OF_UNKNOWN_UPDATE
	def HideAllQuestWindow(self):
		tempList = []
		for i,v in self.wndQuestWindow.iteritems():
			tempList.append(v)

		for i in tempList:
			i.OnCancel()
		
	def RemoveQuestDialog(self, key):
		del self.wndQuestWindow[key]

	## Exchange
	def StartExchange(self):
		self.dlgExchange.OpenDialog()
		self.dlgExchange.Refresh()

	def EndExchange(self):
		self.dlgExchange.CloseDialog()

	def RefreshExchange(self):
		self.dlgExchange.Refresh()

	if app.LWT_BUFF_UPDATE:
		def OpenBuffiDialog(self, gelen):
			if int(gelen) > 0:
				self.dlgBuffiSkillv2.Open()
			else:
				self.dlgBuffiSkill.Open()

		def CloseBuffiDialog(self, gelen):
			if int(gelen) > 0:
				self.dlgBuffiSkillv2.Close()
			else:
				self.dlgBuffiSkill.Close()

	## Party
	def AddPartyMember(self, pid, name):
		self.wndParty.AddPartyMember(pid, name)

		self.__ArrangeQuestButton()

	def UpdatePartyMemberInfo(self, pid):
		self.wndParty.UpdatePartyMemberInfo(pid)

	def RemovePartyMember(self, pid):
		self.wndParty.RemovePartyMember(pid)

		##!! 20061026.levites.퀘스트_위치_보정
		self.__ArrangeQuestButton()

	def LinkPartyMember(self, pid, vid):
		self.wndParty.LinkPartyMember(pid, vid)

	def UnlinkPartyMember(self, pid):
		self.wndParty.UnlinkPartyMember(pid)

	def UnlinkAllPartyMember(self):
		self.wndParty.UnlinkAllPartyMember()

	def ExitParty(self):
		self.wndParty.ExitParty()

		##!! 20061026.levites.퀘스트_위치_보정
		self.__ArrangeQuestButton()

	def PartyHealReady(self):
		self.wndParty.PartyHealReady()

	def ChangePartyParameter(self, distributionMode):
		self.wndParty.ChangePartyParameter(distributionMode)

	## Safebox
	def AskSafeboxPassword(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		if self.wndSafebox.IsShow():
			return

		# SAFEBOX_PASSWORD
		self.dlgPassword.SetTitle(localeInfo.PASSWORD_TITLE)
		self.dlgPassword.SetSendMessage("/safebox_password ")
		# END_OF_SAFEBOX_PASSWORD

		self.dlgPassword.ShowDialog()

	def OpenSafeboxWindow(self, size):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		self.dlgPassword.CloseDialog()
		self.wndSafebox.ShowWindow(size)

	def RefreshSafeboxMoney(self):
		self.wndSafebox.RefreshSafeboxMoney()

	def CommandCloseSafebox(self):
		self.wndSafebox.CommandCloseSafebox()

	if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
		def OpenPShopSearchDialog(self):
			self.wndPrivateShopSearch.Open(0)
		def OpenPShopSearchDialogCash(self):
			self.wndPrivateShopSearch.Open(1)
		def RefreshPShopSearchDialog(self):
			self.wndPrivateShopSearch.RefreshList()
	# ITEM_MALL
	def AskMallPassword(self):
		if self.wndMall.IsShow():
			return
		self.dlgPassword.SetTitle(localeInfo.MALL_PASSWORD_TITLE)
		self.dlgPassword.SetSendMessage("/mall_password ")
		self.dlgPassword.ShowDialog()

	def OpenMallWindow(self, size):
		self.dlgPassword.CloseDialog()
		self.wndMall.ShowWindow(size)

	def CommandCloseMall(self):
		self.wndMall.CommandCloseMall()
	# END_OF_ITEM_MALL

	## Guild
	def OnStartGuildWar(self, guildSelf, guildOpp):
		self.wndGuild.OnStartGuildWar(guildSelf, guildOpp)

		guildWarScoreBoard = uiGuild.GuildWarScoreBoard()
		guildWarScoreBoard.Open(guildSelf, guildOpp)
		guildWarScoreBoard.Show()
		self.guildScoreBoardDict[uiGuild.GetGVGKey(guildSelf, guildOpp)] = guildWarScoreBoard

	def OnEndGuildWar(self, guildSelf, guildOpp):
		self.wndGuild.OnEndGuildWar(guildSelf, guildOpp)

		key = uiGuild.GetGVGKey(guildSelf, guildOpp)

		if not self.guildScoreBoardDict.has_key(key):
			return

		self.guildScoreBoardDict[key].Destroy()
		del self.guildScoreBoardDict[key]

	# GUILDWAR_MEMBER_COUNT
	def UpdateMemberCount(self, gulidID1, memberCount1, guildID2, memberCount2):
		key = uiGuild.GetGVGKey(gulidID1, guildID2)

		if not self.guildScoreBoardDict.has_key(key):
			return

		self.guildScoreBoardDict[key].UpdateMemberCount(gulidID1, memberCount1, guildID2, memberCount2)
	# END_OF_GUILDWAR_MEMBER_COUNT

	def OnRecvGuildWarPoint(self, gainGuildID, opponentGuildID, point):
		key = uiGuild.GetGVGKey(gainGuildID, opponentGuildID)
		if not self.guildScoreBoardDict.has_key(key):
			return

		guildBoard = self.guildScoreBoardDict[key]
		guildBoard.SetScore(gainGuildID, opponentGuildID, point)

	## PK Mode
	def OnChangePKMode(self):
		self.wndCharacter.RefreshAlignment()
		self.dlgSystem.OnChangePKMode()

	## Refine
	def OpenRefineDialog(self, targetItemPos, nextGradeItemVnum, cost, prob, type):
		self.dlgRefineNew.Open(targetItemPos, nextGradeItemVnum, cost, prob, type)

	def AppendMaterialToRefineDialog(self, vnum, count):
		self.dlgRefineNew.AppendMaterial(vnum, count)

	if app.ENABLE_REFINE_RENEWAL:
		def CheckRefineDialog(self, isFail):
			self.dlgRefineNew.CheckRefine(isFail)

	## Show & Hide
	def ShowDefaultWindows(self):
		self.wndTaskBar.Show()
		self.wndMiniMap.Show()
		self.wndMiniMap.ShowMiniMap()
		if self.wndEnergyBar:
			self.wndEnergyBar.Show()

	def ShowAllWindows(self):
		self.wndTaskBar.Show()
		self.wndCharacter.Show()
		self.wndInventory.Show()
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.Show()
			self.wndDragonSoulRefine.Show()
		self.wndChat.Show()
		self.ChatKapat.Show()
		self.wndMiniMap.Show()
		if self.wndEnergyBar:
			self.wndEnergyBar.Show()
		if self.wndWonExchange:
			self.wndWonExchange.Show()
		if self.wndExpandedTaskBar:
			self.wndExpandedTaskBar.Show()
			self.wndExpandedTaskBar.SetTop()
		if self.wndExpandedMoneyTaskbar:
			self.wndExpandedMoneyTaskbar.Show()
			self.wndExpandedMoneyTaskbar.SetTop()

	def HideAllWindows(self):
		if self.wndTaskBar:
			self.wndTaskBar.Hide()
		if self.wndWonExchange:
			self.wndWonExchange.Hide()
		if self.wndEnergyBar:
			self.wndEnergyBar.Hide()

		if app.ENABLE_SWITCHBOT:
			if self.wndSwitchbot:
				self.wndSwitchbot.Hide()

		if app.ENABLE_DETAILS_UI:
			if self.wndCharacter:
				self.wndCharacter.Close()
		else:
			if self.wndCharacter:
				self.wndCharacter.Hide()

		if self.wndCharacter:
			self.wndCharacter.Hide()

		if self.wndInventory:
			self.wndInventory.Hide()
			
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.Hide()
			self.wndDragonSoulRefine.Hide()

		if self.wndChat:
			self.wndChat.Hide()
			
		if self.ChatKapat:
			self.ChatKapat.Hide()

		if self.dlgMaintenanceAdmin:
			self.dlgMaintenanceAdmin.Hide()

		if self.wndMiniMap:
			self.wndMiniMap.Hide()

		if self.wndMessenger:
			self.wndMessenger.Hide()

		if self.wndGuild:
			self.wndGuild.Hide()
			
		if self.wndExpandedTaskBar:
			self.wndExpandedTaskBar.Hide()
		if app.OTOMATIK_AV:
			if self.wndOtoAv:
				self.wndOtoAv.Hide()
		if self.wndExpandedMoneyTaskbar:
			self.wndExpandedMoneyTaskbar.Hide()

	def ShowMouseImage(self):
		self.wndTaskBar.ShowMouseImage()

	def HideMouseImage(self):
		self.wndTaskBar.HideMouseImage()

	def ToggleChat(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		if True == self.wndChat.IsEditMode():
			self.wndChat.CloseChat()
		else:
			# 웹페이지가 열렸을때는 채팅 입력이 안됨
			if self.wndWeb and self.wndWeb.IsShow():
				pass
			else:
				self.wndChat.OpenChat()

	def IsOpenChat(self):
		return self.wndChat.IsEditMode()

	def SetChatFocus(self):
		self.wndChat.SetChatFocus()

	def OpenRestartDialog(self):
		self.dlgRestart.OpenDialog()
		self.dlgRestart.SetTop()

	def CloseRestartDialog(self):
		self.dlgRestart.Close()

	def ToggleSystemDialog(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		if False == self.dlgSystem.IsShow():
			self.dlgSystem.OpenDialog()
			self.dlgSystem.SetTop()
		else:
			self.dlgSystem.Close()

	def OpenSystemDialog(self):
		self.dlgSystem.OpenDialog()
		self.dlgSystem.SetTop()

	def ToggleMessenger(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		if self.wndMessenger.IsShow():
			self.wndMessenger.Hide()
		else:
			self.wndMessenger.SetTop()
			self.wndMessenger.Show()


	def IsShowDlgQuestionWindow(self):
		if self.wndInventory.IsDlgQuestionShow():
			return True
		elif self.wndDragonSoul.IsDlgQuestionShow():
			return True
		elif self.dlgShop.IsDlgQuestionShow():
			return True
		elif self.wndWonExchange.IsDlgQuestionShow():
			return True
		else:
			return False

	def CloseDlgQuestionWindow(self):
		if self.wndInventory.IsDlgQuestionShow():
			self.wndInventory.ExternQuestionDialog_Close()
		if self.wndDragonSoul.IsDlgQuestionShow():
			self.wndDragonSoul.ExternQuestionDialog_Close()
		if self.dlgShop.IsDlgQuestionShow():
			self.dlgShop.ExternQuestionDialog_Close()
		if self.wndWonExchange.IsDlgQuestionShow():
			self.wndWonExchange.ExternQuestionDialog_Close()


	def ToggleMiniMap(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		if app.IsPressed(app.DIK_LSHIFT) or app.IsPressed(app.DIK_RSHIFT):
			if False == self.wndMiniMap.isShowMiniMap():
				self.wndMiniMap.ShowMiniMap()
				self.wndMiniMap.SetTop()
			else:
				self.wndMiniMap.HideMiniMap()

		else:
			self.wndMiniMap.ToggleAtlasWindow()

	def PressMKey(self):
		if app.IsPressed(app.DIK_LALT) or app.IsPressed(app.DIK_RALT):
			self.ToggleMessenger()

		else:
			self.ToggleMiniMap()

	def SetMapName(self, mapName):
		self.wndMiniMap.SetMapName(mapName)

	def MiniMapScaleUp(self):
		self.wndMiniMap.ScaleUp()

	def MiniMapScaleDown(self):
		self.wndMiniMap.ScaleDown()

	def ToggleCharacterWindow(self, state):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		if False == player.IsObserverMode():

			if False == self.wndCharacter.IsShow():
				self.OpenCharacterWindowWithState(state)
			else:
				if state == self.wndCharacter.GetState():
					self.wndCharacter.OverOutItem()
					if app.ENABLE_DETAILS_UI:
						self.wndCharacter.Close()
					else:
						self.wndCharacter.Hide()
				else:
					self.wndCharacter.SetState(state)

	def OpenCharacterWindowWithState(self, state):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		if False == player.IsObserverMode():

			self.wndCharacter.SetState(state)
			self.wndCharacter.Show()
			self.wndCharacter.SetTop()

	def ToggleCharacterWindowStatusPage(self):
		self.ToggleCharacterWindow("STATUS")

	def ToggleInventoryWindow(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		if False == player.IsObserverMode():
			if constInfo.INV_WITH_SPLIT == 1:
				if False == self.wndInventory.IsShow():
					self.wndInventory.Show()
					self.wndInventory.SetTop()
					if not self.wndSpecialStorage.IsShow():
						self.wndSpecialStorage.Show()
					if not self.wndExpandedMoneyTaskbar.IsShow():
						self.wndExpandedMoneyTaskbar.Show()
				else:
					self.wndInventory.OverOutItem()
					self.wndInventory.Close()
					if self.wndSpecialStorage.IsShow():
						self.wndSpecialStorage.OverOutItem()
						self.wndSpecialStorage.Close()			
					if self.wndExpandedMoneyTaskbar.IsShow():
						self.wndExpandedMoneyTaskbar.Hide()
			else:
				if False == self.wndInventory.IsShow():
					self.wndInventory.Show()
					self.wndInventory.SetTop()
					if not self.wndExpandedMoneyTaskbar.IsShow():
						self.wndExpandedMoneyTaskbar.Show()
				else:
					self.wndInventory.OverOutItem()
					self.wndInventory.Close()
					if self.wndExpandedMoneyTaskbar.IsShow():
						self.wndExpandedMoneyTaskbar.Hide()

	def ToggleExpandedMoneyButton(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return

		if False == player.IsObserverMode():
			if False == self.wndExpandedMoneyTaskbar.IsShow():
				self.wndExpandedMoneyTaskbar.Show()
				self.wndExpandedMoneyTaskbar.SetTop()
			else:
				self.wndExpandedMoneyTaskbar.Close()
	def ToggleExp(self):
		if constInfo.EXPKAZAN == 0:
			constInfo.EXPKAZAN = 1
			net.SendChatPacket("/antiexpac")	
		else:
			constInfo.EXPKAZAN = 0
			net.SendChatPacket("/antiexpkapa")	

	def ToggleExpandedButton(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		if False == player.IsObserverMode():

			if False == self.wndExpandedTaskBar.IsShow():
				self.wndExpandedTaskBar.Show()
				self.wndExpandedTaskBar.SetTop()
			else:
				self.wndExpandedTaskBar.Close()

	def ToggleDeleteSellButton(self):
		constInfo.DELETE_SELL_OPEN = 1

	# 용혼석
	def DragonSoulActivate(self, deck):
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.ActivateDragonSoulByExtern(deck)

	def DragonSoulDeactivate(self):
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.DeactivateDragonSoul()
		
	if app.ENABLE_DS_SET:
		def DragonSoulSetGrade(self, grade):
			self.wndDragonSoul.SetDSSetGrade(grade)
		
	def Highligt_Item(self, inven_type, inven_pos):
		if player.DRAGON_SOUL_INVENTORY == inven_type:
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				self.wndDragonSoul.HighlightSlot(inven_pos)
			
	def DragonSoulGiveQuilification(self):
		self.DRAGON_SOUL_IS_QUALIFIED = True
		self.wndExpandedTaskBar.SetToolTipText(uiTaskBar.ExpandedTaskBar.BUTTON_DRAGON_SOUL, uiScriptLocale.TASKBAR_DRAGON_SOUL)

	def ToggleDragonSoulWindow(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		if False == player.IsObserverMode():

			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if False == self.wndDragonSoul.IsShow():
					if self.DRAGON_SOUL_IS_QUALIFIED:
						self.wndDragonSoul.Show()
					else:
						try:
							self.wndPopupDialog.SetText(localeInfo.DRAGON_SOUL_UNQUALIFIED)
							self.wndPopupDialog.Open()
						except:
							self.wndPopupDialog = uiCommon.PopupDialog()
							self.wndPopupDialog.SetText(localeInfo.DRAGON_SOUL_UNQUALIFIED)
							self.wndPopupDialog.Open()
				else:
					self.wndDragonSoul.Close()
		
	def ToggleDragonSoulWindowWithNoInfo(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		if False == player.IsObserverMode():

			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if False == self.wndDragonSoul.IsShow():
					if self.DRAGON_SOUL_IS_QUALIFIED:
						self.wndDragonSoul.Show()
				else:
					self.wndDragonSoul.Close()
				
	if app.ENABLE_SPECIAL_STORAGE:
		def ToggleSpecialStorageWindow(self):
			if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
				return
			if False == player.IsObserverMode():

				if False == self.wndSpecialStorage.IsShow():
					self.wndSpecialStorage.Show()
				else:
					self.wndSpecialStorage.Close()
	def FailDragonSoulRefine(self, reason, inven_type, inven_pos):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if True == self.wndDragonSoulRefine.IsShow():
					self.wndDragonSoulRefine.RefineFail(reason, inven_type, inven_pos)
 
	def SucceedDragonSoulRefine(self, inven_type, inven_pos):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if True == self.wndDragonSoulRefine.IsShow():
					self.wndDragonSoulRefine.RefineSucceed(inven_type, inven_pos)
 
	def OpenDragonSoulRefineWindow(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		if False == player.IsObserverMode():

			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if False == self.wndDragonSoulRefine.IsShow():
					self.wndDragonSoulRefine.Show()
					if None != self.wndDragonSoul:
						if False == self.wndDragonSoul.IsShow():
							self.wndDragonSoul.Show()

	def CloseDragonSoulRefineWindow(self):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if True == self.wndDragonSoulRefine.IsShow():
					self.wndDragonSoulRefine.Close()

	
	
	def ToggleGuildWindow(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		if not self.wndGuild.IsShow():
			if self.wndGuild.CanOpen():
				self.wndGuild.Open()
			else:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GUILD_YOU_DO_NOT_JOIN)
		else:
			self.wndGuild.OverOutItem()
			self.wndGuild.Hide()

	def ToggleChatLogWindow(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		if self.wndChatLog.IsShow():
			self.wndChatLog.Hide()
		else:
			self.wndChatLog.Show()

	if app.ENABLE_SWITCHBOT:
		def ToggleSwitchbotWindow(self):
			if self.wndSwitchbot.IsShow():
				self.wndSwitchbot.Close()
			else:
				self.wndSwitchbot.Open()
				
		def RefreshSwitchbotWindow(self):
			if self.wndSwitchbot and self.wndSwitchbot.IsShow():
				self.wndSwitchbot.RefreshSwitchbotWindow()

		def RefreshSwitchbotItem(self, slot):
			if self.wndSwitchbot and self.wndSwitchbot.IsShow():
				self.wndSwitchbot.RefreshSwitchbotItem(slot)

	def CheckGameButton(self):
		if self.wndGameButton:
			self.wndGameButton.CheckGameButton()

	def __OnClickStatusPlusButton(self):
		self.ToggleCharacterWindow("STATUS")

	def __OnClickSkillPlusButton(self):
		self.ToggleCharacterWindow("SKILL")

	def __OnClickQuestButton(self):
		self.ToggleCharacterWindow("QUEST")

	def __OnClickHelpButton(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		player.SetPlayTime(1)
		self.CheckGameButton()
		self.OpenHelpWindow()

	def __OnClickBuildButton(self):
		self.BUILD_OpenWindow()

	def OpenHelpWindow(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return

		self.wndUICurtain.Show()
		self.wndHelp.Open()

	def CloseHelpWindow(self):
		self.wndUICurtain.Hide()
		self.wndHelp.Close()

	def OpenWebWindow(self, url):
		self.wndWeb.Open(url)

		# 웹페이지를 열면 채팅을 닫는다
		self.wndChat.CloseChat()

	if app.ENABLE_ITEM_SHOP_SYSTEM:
		def RefreshItemShop(self):
			constInfo.ITEM_DATA = {}
			constInfo.ITEM_SEARCH_DATA = []
			if self.ItemShop:
				self.ItemShop.RefreshProcess()
				self.ItemShop.Destroy()

		def OpenItemShop(self):
			if self.ItemShop:
				self.ItemShop.LoadWindow()
				self.ItemShop.Open()

	# show GIFT
	def ShowGift(self):
		self.wndTaskBar.ShowGift()
	    	
	def CloseWbWindow(self):
		self.wndWeb.Close()

	def OpenCardsInfoWindow(self):
		self.wndCardsInfo.Open()
		
	def OpenCardsWindow(self, safemode):
		self.wndCards.Open(safemode)
		
	def UpdateCardsInfo(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, hand_4, hand_4_v, hand_5, hand_5_v, cards_left, points):
		self.wndCards.UpdateCardsInfo(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, hand_4, hand_4_v, hand_5, hand_5_v, cards_left, points)
		
	def UpdateCardsFieldInfo(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points):
		self.wndCards.UpdateCardsFieldInfo(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points)
		
	def CardsPutReward(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points):
		self.wndCards.CardsPutReward(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points)
		
	def CardsShowIcon(self):
		self.wndCardsIcon.Show()
		
	if app.ENABLE_SASH_SYSTEM:
		def ActSash(self, iAct, bWindow):
			if iAct == 1:
				if bWindow == True:
					if not self.wndSashCombine.IsOpened():
						self.wndSashCombine.Open()
					
					if not self.wndInventory.IsShow():
						self.wndInventory.Show()
				else:
					if not self.wndSashAbsorption.IsOpened():
						self.wndSashAbsorption.Open()
					
					if not self.wndInventory.IsShow():
						self.wndInventory.Show()
				
				self.wndInventory.RefreshBagSlotWindow()
			elif iAct == 2:
				if bWindow == True:
					if self.wndSashCombine.IsOpened():
						self.wndSashCombine.Close()
				else:
					if self.wndSashAbsorption.IsOpened():
						self.wndSashAbsorption.Close()
				
				self.wndInventory.RefreshBagSlotWindow()
			elif iAct == 3 or iAct == 4:
				if bWindow == True:
					if self.wndSashCombine.IsOpened():
						self.wndSashCombine.Refresh(iAct)
				else:
					if self.wndSashAbsorption.IsOpened():
						self.wndSashAbsorption.Refresh(iAct)
				
				self.wndInventory.RefreshBagSlotWindow()		
				
	if app.ENABLE_AURA_SYSTEM:
		def ActAura(self, iAct, bWindow):
			if iAct == 1:
				if bWindow == True:
					if not self.wndAuraRefine.IsOpened():
						self.wndAuraRefine.Open()
					
					if not self.wndInventory.IsShow():
						self.wndInventory.Show()
				else:
					if not self.wndAuraAbsorption.IsOpened():
						self.wndAuraAbsorption.Open()
					
					if not self.wndInventory.IsShow():
						self.wndInventory.Show()
				
				self.wndInventory.RefreshBagSlotWindow()
			elif iAct == 2:
				if bWindow == True:
					if self.wndAuraRefine.IsOpened():
						self.wndAuraRefine.Close()
				else:
					if self.wndAuraAbsorption.IsOpened():
						self.wndAuraAbsorption.Close()
				
				self.wndInventory.RefreshBagSlotWindow()
			elif iAct == 3 or iAct == 4:
				if bWindow == True:
					if self.wndAuraRefine.IsOpened():
						self.wndAuraRefine.Refresh(iAct)
				else:
					if self.wndAuraAbsorption.IsOpened():
						self.wndAuraAbsorption.Refresh(iAct)
				
				self.wndInventory.RefreshBagSlotWindow()
				
	def OpenCubeWindow(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		self.wndCube.Open()

		if FALSE == self.wndInventory.IsShow():
			self.wndInventory.Show()
			
	def ShowBoardBpass(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		self.wndbpass.Show()

			
	def OpenDungeonTimer(self):
		if self.wndDungeonTimer.IsShow():
			self.wndDungeonTimer.Hide()
		else:
			self.wndDungeonTimer.Open()


	def OpenBiyologTable(self):
		if app.WJ_SECURITY_SYSTEM and player.IsSecurityActivate():
			return
		if self.wndBiyologTable.IsShow():
			self.wndBiyologTable.Hide()
		else:
			self.wndBiyologTable.Open()

	def UpdateCubeInfo(self, gold, itemVnum, count):
		self.wndCube.UpdateInfo(gold, itemVnum, count)

	def CloseCubeWindow(self):
		self.wndCube.Close()

	def FailedCubeWork(self):
		self.wndCube.Refresh()

	def SucceedCubeWork(self, itemVnum, count):
		self.wndCube.Clear()
		
		print "큐브 제작 성공! [%d:%d]" % (itemVnum, count)

		if 0: # 결과 메시지 출력은 생략 한다
			self.wndCubeResult.SetPosition(*self.wndCube.GetGlobalPosition())
			self.wndCubeResult.SetCubeResultItem(itemVnum, count)
			self.wndCubeResult.Open()
			self.wndCubeResult.SetTop()

	if app.WJ_SECURITY_SYSTEM:
		def OpenSecurityCreate(self):
			self.wndSecurityWindow.ShowDialog(True)
		
		def OpenSecurityDialog2(self, stat):
			self.wndSecurityWindow.ShowDialog2(stat)
			
		def OpenSecurityDialog(self):
			self.wndSecurityWindow.ShowDialog(False)
			
		def CloseSecurityCreate(self):
			self.wndSecurityWindow.CloseDialog()
			
		def CloseSecurityDialog(self):
			self.wndSecurityWindow.CloseDialog()
	def __HideWindows(self):
		hideWindows = self.wndTaskBar,\
						self.wndCharacter,\
						self.wndInventory,\
						self.wndMiniMap,\
						self.wndGuild,\
						self.wndMessenger,\
						self.wndChat,\
						self.wndParty,\
						self.ChatKapat,\
						self.wndGameButton,
		hideWindows += self.wndWonExchange,
		if self.wndEnergyBar:
			hideWindows += self.wndEnergyBar,
		if self.wndbpass:
			hideWindows += self.wndbpass,
			
		if app.ENABLE_SWITCHBOT and self.wndSwitchbot:
			hideWindows += self.wndSwitchbot,
			
		if self.wndExpandedTaskBar:
			hideWindows += self.wndExpandedTaskBar,

		if self.dlgMaintenanceAdmin:
			hideWindows += self.dlgMaintenanceAdmin,

		if self.wndExpandedMoneyTaskbar:
			hideWindows += self.wndExpandedMoneyTaskbar,
 			
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			hideWindows += self.wndDragonSoul,\
						self.wndDragonSoulRefine,

		hideWindows = filter(lambda x:x.IsShow(), hideWindows)
		map(lambda x:x.Hide(), hideWindows)
		import sys

		self.HideAllQuestButton()
		self.HideAllWhisperButton()

		if self.wndChat.IsEditMode():
			self.wndChat.CloseChat()

		return hideWindows

	def __ShowWindows(self, wnds):
		import sys
		map(lambda x:x.Show(), wnds)
		global IsQBHide
		if not IsQBHide:
			self.ShowAllQuestButton()
		else:
			self.HideAllQuestButton()

		self.ShowAllWhisperButton()

	def BINARY_OpenAtlasWindow(self):
		if self.wndMiniMap:
			self.wndMiniMap.ShowAtlas()

	def BINARY_SetObserverMode(self, flag):
		self.wndGameButton.SetObserverMode(flag)

	# ACCESSORY_REFINE_ADD_METIN_STONE
	def BINARY_OpenSelectItemWindow(self):
		self.wndItemSelect.Open()
	# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE
	#####################################################################################
	### Private Shop ###

	def OpenPrivateShopInputNameDialog(self):
		#if player.IsInSafeArea():
		#	chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CANNOT_OPEN_PRIVATE_SHOP_IN_SAFE_AREA)
		#	return

		inputDialog = uiCommon.InputDialog()
		inputDialog.SetTitle(localeInfo.PRIVATE_SHOP_INPUT_NAME_DIALOG_TITLE)
		inputDialog.SetMaxLength(32)
		inputDialog.SetAcceptEvent(ui.__mem_func__(self.OpenPrivateShopBuilder))
		inputDialog.SetCancelEvent(ui.__mem_func__(self.ClosePrivateShopInputNameDialog))
		inputDialog.Open()
		self.inputDialog = inputDialog

	def ClosePrivateShopInputNameDialog(self):
		self.inputDialog = None
		return True

	def OpenPrivateShopBuilder(self):

		if not self.inputDialog:
			return True

		if not len(self.inputDialog.GetText()):
			return True

		self.privateShopBuilder.Open(self.inputDialog.GetText())
		self.ClosePrivateShopInputNameDialog()
		return True

	def ToggleWonExchangeWindow(self):
		if player.IsObserverMode():
			return

		if False == self.wndWonExchange.IsShow():
			self.wndWonExchange.SetPage(uiWonExchange.WonExchangeWindow.PAGE_SELL)
			if False == self.wndExpandedMoneyTaskbar.IsShow():
				self.wndExpandedMoneyTaskbar.Show()
				self.wndExpandedMoneyTaskbar.SetTop()
			self.wndWonExchange.Show()
			self.wndWonExchange.SetTop()
		else:
			self.wndWonExchange.Hide()

	def AppearPrivateShop(self, vid, text):

		board = uiPrivateShopBuilder.PrivateShopAdvertisementBoard()
		board.Open(vid, text)

		self.privateShopAdvertisementBoardDict[vid] = board

	def DisappearPrivateShop(self, vid):

		if not self.privateShopAdvertisementBoardDict.has_key(vid):
			return

		del self.privateShopAdvertisementBoardDict[vid]
		uiPrivateShopBuilder.DeleteADBoard(vid)

	#####################################################################################
	### Offline Shop ###
	
	def OpenOfflineShopInputNameDialog(self):
		inputDialog = uiOfflineShop.OfflineShopInputDialog()
		inputDialog.SetAcceptEvent(ui.__mem_func__(self.OpenOfflineShopBuilder))
		inputDialog.SetCancelEvent(ui.__mem_func__(self.CloseOfflineShopInputNameDialog))
		inputDialog.Open()
		self.inputDialog = inputDialog
		
		
	def OpenOfflineShopDecoration(self):
		offlineDecoration = uimyshopdeco.UiShopDeco()
		self.offlineDecoration = offlineDecoration
		self.offlineDecoration.Show()
		
	def CloseOfflineShopInputNameDialog(self):
		self.inputDialog = None
		if self.offlineDecoration.IsShow():
			self.offlineDecoration.Hide()
		return True
	
	def OpenOfflineShopBuilder(self):
		if (not self.inputDialog):
			return True
			
		if (not len(self.inputDialog.GetTitle())):
			return True
			
		if (self.inputDialog.GetTime() < 0 or self.inputDialog.GetTime() == 0):
			return True
			
		self.offlineShopBuilder.Open(self.inputDialog.GetTitle(), self.inputDialog.GetTime())
		self.CloseOfflineShopInputNameDialog()
		return True
	
	def AppearOfflineShop(self, vid, text):	
		#//*Shop Decoration
		if text[0] == "0":
			board = uiOfflineShopBuilder.OfflineShopAdvertisementBoard()
		elif text[0] == "1":
			board = uiOfflineShopBuilder.OfflineShopAdvertisementBoard0()
		elif text[0] == "2":
			board = uiOfflineShopBuilder.OfflineShopAdvertisementBoard1()
		elif text[0] == "3":
			board = uiOfflineShopBuilder.OfflineShopAdvertisementBoard2()
		elif text[0] == "4":
			board = uiOfflineShopBuilder.OfflineShopAdvertisementBoard3()
		elif text[0] == "5":
			board = uiOfflineShopBuilder.OfflineShopAdvertisementBoard4()
			
		text = text[1:]
		board.Open(vid, text)
		self.offlineShopAdvertisementBoardDict[vid] = board	
		
	def DisappearOfflineShop(self, vid):
		if (not self.offlineShopAdvertisementBoardDict.has_key(vid)):
			return
			
		del self.offlineShopAdvertisementBoardDict[vid]
		uiOfflineShopBuilder.DeleteADBoard(vid)		

	#####################################################################################
	### Equipment ###

	def OpenEquipmentDialog(self, vid):
		dlg = uiEquipmentDialog.EquipmentDialog()
		dlg.SetItemToolTip(self.tooltipItem)
		dlg.SetCloseEvent(ui.__mem_func__(self.CloseEquipmentDialog))
		dlg.Open(vid)

		self.equipmentDialogDict[vid] = dlg

	def SetEquipmentDialogItem(self, vid, slotIndex, vnum, count):
		if not vid in self.equipmentDialogDict:
			return
		self.equipmentDialogDict[vid].SetEquipmentDialogItem(slotIndex, vnum, count)

	def SetEquipmentDialogSocket(self, vid, slotIndex, socketIndex, value):
		if not vid in self.equipmentDialogDict:
			return
		self.equipmentDialogDict[vid].SetEquipmentDialogSocket(slotIndex, socketIndex, value)

	def SetEquipmentDialogAttr(self, vid, slotIndex, attrIndex, type, value):
		if not vid in self.equipmentDialogDict:
			return
		self.equipmentDialogDict[vid].SetEquipmentDialogAttr(slotIndex, attrIndex, type, value)

	def CloseEquipmentDialog(self, vid):
		if not vid in self.equipmentDialogDict:
			return
		del self.equipmentDialogDict[vid]

	#####################################################################################

	#####################################################################################
	### Quest ###	
	def BINARY_ClearQuest(self, index):
		btn = self.__FindQuestButton(index)
		if 0 != btn:
			self.__DestroyQuestButton(btn)		
	
	def RecvQuest(self, index, name):
		# QUEST_LETTER_IMAGE
		self.BINARY_RecvQuest(index, name, "file", localeInfo.GetLetterImageName())
		# END_OF_QUEST_LETTER_IMAGE

	def BINARY_RecvQuest(self, index, name, iconType, iconName):

		btn = self.__FindQuestButton(index)
		if 0 != btn:
			self.__DestroyQuestButton(btn)

		btn = uiWhisper.WhisperButton()

		# QUEST_LETTER_IMAGE
		##!! 20061026.levites.퀘스트_이미지_교체
		import item
		if "item"==iconType:
			item.SelectItem(int(iconName))
			buttonImageFileName=item.GetIconImageFileName()
		else:
			buttonImageFileName=iconName

		if localeInfo.IsEUROPE():
			if "highlight" == iconType:
				btn.SetUpVisual("locale/ymir_ui/highlighted_quest.tga")
				btn.SetOverVisual("locale/ymir_ui/highlighted_quest_r.tga")
				btn.SetDownVisual("locale/ymir_ui/highlighted_quest_r.tga")
			else:
				btn.SetUpVisual(localeInfo.GetLetterCloseImageName())
				btn.SetOverVisual(localeInfo.GetLetterOpenImageName())
				btn.SetDownVisual(localeInfo.GetLetterOpenImageName())				
		else:
			btn.SetUpVisual(buttonImageFileName)
			btn.SetOverVisual(buttonImageFileName)
			btn.SetDownVisual(buttonImageFileName)
			btn.Flash()
		# END_OF_QUEST_LETTER_IMAGE

		if localeInfo.IsARABIC():
			btn.SetToolTipText(name, 0, 35)
			btn.ToolTipText.SetHorizontalAlignCenter()
		else:
			btn.SetToolTipText(name, -20, 35)
			btn.ToolTipText.SetHorizontalAlignLeft()
			
		btn.SetEvent(ui.__mem_func__(self.__StartQuest), btn)
		btn.Show()

		btn.index = index
		btn.name = name

		self.questButtonList.insert(0, btn)
		self.__ArrangeQuestButton()

		#chat.AppendChat(chat.CHAT_TYPE_NOTICE, localeInfo.QUEST_APPEND)

	def __ArrangeQuestButton(self):

		screenWidth = wndMgr.GetScreenWidth()
		screenHeight = wndMgr.GetScreenHeight()

		##!! 20061026.levites.퀘스트_위치_보정
		if self.wndParty.IsShow():
			xPos = 100 + 30
		else:
			xPos = 20

		if localeInfo.IsARABIC():
			xPos = xPos + 15

		yPos = 170 * screenHeight / 600
		yCount = (screenHeight - 330) / 63

		count = 0
		for btn in self.questButtonList:

			btn.SetPosition(xPos + (int(count/yCount) * 100), yPos + (count%yCount * 63))
			count += 1
			global IsQBHide
			if IsQBHide:
				btn.Hide()
			else:
				btn.Show()

	def __StartQuest(self, btn):
		event.QuestButtonClick(btn.index)
		self.__DestroyQuestButton(btn)

	def __FindQuestButton(self, index):
		for btn in self.questButtonList:
			if btn.index == index:
				return btn

		return 0

	def __DestroyQuestButton(self, btn):
		btn.SetEvent(0)
		self.questButtonList.remove(btn)
		self.__ArrangeQuestButton()

	def HideAllQuestButton(self):
		for btn in self.questButtonList:
			btn.Hide()

	def ShowAllQuestButton(self):
		for btn in self.questButtonList:
			btn.Show()
	#####################################################################################

	#####################################################################################
	### Whisper ###

	def __InitWhisper(self):
		chat.InitWhisper(self)

	## 채팅창의 "메시지 보내기"를 눌렀을때 이름 없는 대화창을 여는 함수
	## 이름이 없기 때문에 기존의 WhisperDialogDict 와 별도로 관리된다.
	def OpenWhisperDialogWithoutTarget(self):
		if not self.dlgWhisperWithoutTarget:
			dlgWhisper = uiWhisper.WhisperDialog(self.MinimizeWhisperDialog, self.CloseWhisperDialog)
			dlgWhisper.BindInterface(self)
			dlgWhisper.LoadDialog()
			dlgWhisper.OpenWithoutTarget(self.RegisterTemporaryWhisperDialog)
			dlgWhisper.SetPosition(self.windowOpenPosition*30,self.windowOpenPosition*30)
			dlgWhisper.Show()
			self.dlgWhisperWithoutTarget = dlgWhisper

			self.windowOpenPosition = (self.windowOpenPosition+1) % 5

		else:
			self.dlgWhisperWithoutTarget.SetTop()
			self.dlgWhisperWithoutTarget.OpenWithoutTarget(self.RegisterTemporaryWhisperDialog)

	## 이름 없는 대화창에서 이름을 결정했을때 WhisperDialogDict에 창을 넣어주는 함수
	def RegisterTemporaryWhisperDialog(self, name):
		if not self.dlgWhisperWithoutTarget:
			return

		btn = self.__FindWhisperButton(name)
		if 0 != btn:
			self.__DestroyWhisperButton(btn)

		elif self.whisperDialogDict.has_key(name):
			oldDialog = self.whisperDialogDict[name]
			oldDialog.Destroy()
			del self.whisperDialogDict[name]

		self.whisperDialogDict[name] = self.dlgWhisperWithoutTarget
		self.dlgWhisperWithoutTarget.OpenWithTarget(name)
		self.dlgWhisperWithoutTarget = None
		self.__CheckGameMaster(name)

	## 캐릭터 메뉴의 1:1 대화 하기를 눌렀을때 이름을 가지고 바로 창을 여는 함수
	def OpenWhisperDialog(self, name):
		if not self.whisperDialogDict.has_key(name):
			dlg = self.__MakeWhisperDialog(name)
			dlg.OpenWithTarget(name)
			dlg.chatLine.SetFocus()
			dlg.Show()

			self.__CheckGameMaster(name)
			btn = self.__FindWhisperButton(name)
			if 0 != btn:
				self.__DestroyWhisperButton(btn)

	## 다른 캐릭터로부터 메세지를 받았을때 일단 버튼만 띄워 두는 함수
	def RecvWhisper(self, name):
		if not self.whisperDialogDict.has_key(name):
			btn = self.__FindWhisperButton(name)
			if 0 == btn:
				btn = self.__MakeWhisperButton(name)
				btn.Flash()
				#app.FlashApplication()

				chat.AppendChat(chat.CHAT_TYPE_NOTICE, localeInfo.RECEIVE_MESSAGE % (name))

			else:
				btn.Flash()
		elif self.IsGameMasterName(name):
			dlg = self.whisperDialogDict[name]
			dlg.SetGameMasterLook()

	def MakeWhisperButton(self, name):
		self.__MakeWhisperButton(name)

	## 버튼을 눌렀을때 창을 여는 함수
	def ShowWhisperDialog(self, btn):
		try:
			self.__MakeWhisperDialog(btn.name)
			dlgWhisper = self.whisperDialogDict[btn.name]
			dlgWhisper.OpenWithTarget(btn.name)
			dlgWhisper.Show()
			self.__CheckGameMaster(btn.name)
		except:
			import dbg
			dbg.TraceError("interface.ShowWhisperDialog - Failed to find key")

		## 버튼 초기화
		self.__DestroyWhisperButton(btn)

	## WhisperDialog 창에서 최소화 명령을 수행했을때 호출되는 함수
	## 창을 최소화 합니다.
	def MinimizeWhisperDialog(self, name):

		if 0 != name:
			self.__MakeWhisperButton(name)

		self.CloseWhisperDialog(name)

	## WhisperDialog 창에서 닫기 명령을 수행했을때 호출되는 함수
	## 창을 지웁니다.
	def CloseWhisperDialog(self, name):

		if 0 == name:

			if self.dlgWhisperWithoutTarget:
				self.dlgWhisperWithoutTarget.Destroy()
				self.dlgWhisperWithoutTarget = None

			return

		try:
			dlgWhisper = self.whisperDialogDict[name]
			dlgWhisper.Destroy()
			del self.whisperDialogDict[name]
		except:
			import dbg
			dbg.TraceError("interface.CloseWhisperDialog - Failed to find key")

	## 버튼의 개수가 바뀌었을때 버튼을 재정렬 하는 함수
	def __ArrangeWhisperButton(self):

		screenWidth = wndMgr.GetScreenWidth()
		screenHeight = wndMgr.GetScreenHeight()

		xPos = screenWidth - 70
		yPos = 170 * screenHeight / 600
		yCount = (screenHeight - 330) / 63
		#yCount = (screenHeight - 285) / 63

		count = 0
		for button in self.whisperButtonList:

			button.SetPosition(xPos + (int(count/yCount) * -50), yPos + (count%yCount * 63))
			count += 1

	## 이름으로 Whisper 버튼을 찾아 리턴해 주는 함수
	## 버튼은 딕셔너리로 하지 않는 것은 정렬 되어 버려 순서가 유지 되지 않으며
	## 이로 인해 ToolTip들이 다른 버튼들에 의해 가려지기 때문이다.
	def __FindWhisperButton(self, name):
		for button in self.whisperButtonList:
			if button.name == name:
				return button

		return 0

	## 창을 만듭니다.
	def __MakeWhisperDialog(self, name):
		dlgWhisper = uiWhisper.WhisperDialog(self.MinimizeWhisperDialog, self.CloseWhisperDialog)
		dlgWhisper.BindInterface(self)
		dlgWhisper.LoadDialog()
		dlgWhisper.SetPosition(self.windowOpenPosition*30,self.windowOpenPosition*30)
		self.whisperDialogDict[name] = dlgWhisper

		self.windowOpenPosition = (self.windowOpenPosition+1) % 5

		return dlgWhisper

	## 버튼을 만듭니다.
	def __MakeWhisperButton(self, name):
		whisperButton = uiWhisper.WhisperButton()
		whisperButton.SetUpVisual("d:/ymir work/ui/game/windows/btn_mail_up.sub")
		whisperButton.SetOverVisual("d:/ymir work/ui/game/windows/btn_mail_up.sub")
		whisperButton.SetDownVisual("d:/ymir work/ui/game/windows/btn_mail_up.sub")
		if self.IsGameMasterName(name):
			whisperButton.SetToolTipTextWithColor(name, 0xffffa200)
		else:
			whisperButton.SetToolTipText(name)
		whisperButton.ToolTipText.SetHorizontalAlignCenter()
		whisperButton.SetEvent(ui.__mem_func__(self.ShowWhisperDialog), whisperButton)
		whisperButton.Show()
		whisperButton.name = name

		self.whisperButtonList.insert(0, whisperButton)
		self.__ArrangeWhisperButton()

		return whisperButton

	def __DestroyWhisperButton(self, button):
		button.SetEvent(0)
		self.whisperButtonList.remove(button)
		self.__ArrangeWhisperButton()

	def HideAllWhisperButton(self):
		for btn in self.whisperButtonList:
			btn.Hide()

	def ShowAllWhisperButton(self):
		for btn in self.whisperButtonList:
			btn.Show()

	def __CheckGameMaster(self, name):
		if not self.listGMName.has_key(name):
			return
		if self.whisperDialogDict.has_key(name):
			dlg = self.whisperDialogDict[name]
			dlg.SetGameMasterLook()

	def RegisterGameMasterName(self, name):
		if self.listGMName.has_key(name):
			return
		self.listGMName[name] = "GM"

	def IsGameMasterName(self, name):
		if self.listGMName.has_key(name):
			return True
		else:
			return False

	#####################################################################################

	#####################################################################################
	### Guild Building ###

	def BUILD_OpenWindow(self):
		self.wndGuildBuilding = uiGuild.BuildGuildBuildingWindow()
		self.wndGuildBuilding.Open()
		self.wndGuildBuilding.wnds = self.__HideWindows()
		self.wndGuildBuilding.SetCloseEvent(ui.__mem_func__(self.BUILD_CloseWindow))

	def BUILD_CloseWindow(self):
		self.__ShowWindows(self.wndGuildBuilding.wnds)
		self.wndGuildBuilding = None

	def BUILD_OnUpdate(self):
		if not self.wndGuildBuilding:
			return

		if self.wndGuildBuilding.IsPositioningMode():
			import background
			x, y, z = background.GetPickingPoint()
			self.wndGuildBuilding.SetBuildingPosition(x, y, z)

	def BUILD_OnMouseLeftButtonDown(self):
		if not self.wndGuildBuilding:
			return

		# GUILD_BUILDING
		if self.wndGuildBuilding.IsPositioningMode():
			self.wndGuildBuilding.SettleCurrentPosition()
			return True
		elif self.wndGuildBuilding.IsPreviewMode():
			pass
		else:
			return True
		# END_OF_GUILD_BUILDING
		return False

	def OpenKygnItemRemoveWindow(self):
		if self.wndKygnItemSil.IsShow():
			self.wndKygnItemSil.Hide()
			constInfo.ITEM_REMOVE_WINDOW_STATUS = 0
			return
		constInfo.ITEM_REMOVE_WINDOW_STATUS = 1
		if self.wndKygnItemSil:
			self.wndKygnItemSil.Show2()
		if self.wndInventory:
			self.wndInventory.OpenRemoveItemWindow(self.wndKygnItemSil)
		if self.wndSpecialStorage:
			self.wndSpecialStorage.OpenRemoveItemWindow(self.wndKygnItemSil)

	def BUILD_OnMouseLeftButtonUp(self):
		if not self.wndGuildBuilding:
			return

		if not self.wndGuildBuilding.IsPreviewMode():
			return True

		return False

	def BULID_EnterGuildArea(self, areaID):
		# GUILD_BUILDING
		mainCharacterName = player.GetMainCharacterName()
		masterName = guild.GetGuildMasterName()

		if mainCharacterName != masterName:
			return

		if areaID != player.GetGuildID():
			return
		# END_OF_GUILD_BUILDING

		self.wndGameButton.ShowBuildButton()

	def BULID_ExitGuildArea(self, areaID):
		self.wndGameButton.HideBuildButton()

	def Show_BattlePass(self):
		self.wndGameButton.ShowPassButton()

	def Hide_BattlePass(self):
		self.wndGameButton.HidePassButton()
		
	def OpenSearchBox(self):
		if self.itemgui:
			self.itemgui.Show()		
	if app.ENABLE_CUBE_RENEWAL:
		def BINARY_CUBE_RENEWAL_OPEN(self):
			if self.wndCubeRenewal.IsShow():
				self.wndCubeRenewal.Close()
			else:
				self.wndCubeRenewal.Show()
		
	def GetInventoryPtr(self):
		if self.wndInventory:
			return self.wndInventory
		else:
			return -1

	def GetDragonSoulInventoryPtr(self):
		if self.wndDragonSoul:
			return self.wndDragonSoul
		else:
			return -1

	if app.ENABLE_SPECIAL_STORAGE:
		def GetSpecialStoragePtr(self):
			if self.wndSpecialStorage:
				return self.wndSpecialStorage
			else:
				return -1

	#####################################################################################

	def IsEditLineFocus(self):
		if self.ChatWindow.chatLine.IsFocus():
			return 1

		if self.ChatWindow.chatToLine.IsFocus():
			return 1

		return 0

	def EmptyFunction(self):
		pass

	if app.AUTO_SHOUT:
		def OpenShoutWindow(self):
			if self.wndShout.IsShow():
				self.wndShout.Hide()
			else:
				self.wndShout.Open()
				
	if app.ENABLE_NEW_PET_SYSTEM:
		def GetInventoryPageIndex(self):
			if self.wndInventory:
				return self.wndInventory.GetInventoryPageIndex()
			else:
				return -1

		def GetInventoryHandle(self):
			if self.wndInventory:
				return self.wndInventory
			else:
				return -1

		def TogglePetMain(self):
			net.SendChatPacket("/gift")

		def OpenInputNameDialogPet(self, slot):
			inputDialog = uiCommon.InputDialogName()
			inputDialog.SetTitle("Isim Degistir")
			inputDialog.SetAcceptEvent(ui.__mem_func__(self.ChangePetName))
			inputDialog.SetCancelEvent(ui.__mem_func__(self.ClosePrivateShopInputNameDialog))
			inputDialog.Open()
			inputDialog.slot = slot	
			self.inputDialog = inputDialog

		def ChangePetName(self):
			net.SendChatPacket("/pet_change_name {0} {1}".format(str(self.inputDialog.slot), str(self.inputDialog.GetText())))
			self.ClosePrivateShopInputNameDialog()
	if app.ENABLE_DSS_ACTIVE_EFFECT_BUTTON:
		def UseDSSButtonEffect(self, enable):
			if self.wndInventory:
				self.wndInventory.UseDSSButtonEffect(enable)
	if app.OTOMATIK_AV:
		def ToggleAutoWindow(self):
			if False == player.IsObserverMode():
				if not self.wndOtoAv.IsShow(): self.wndOtoAv.Show()
				else: self.wndOtoAv.Close()
		def SetAutoCooltime(self, slotindex, cooltime):
			self.wndOtoAv.SetAutoCooltime(slotindex, cooltime)
		def SetCloseGame(self):
			self.wndOtoAv.SetCloseGame()
		def GetAutoStartonoff(self):
			return self.wndOtoAv.GetAutoStartonoff()
		def RefreshAutoSkillSlot(self):
			if self.wndOtoAv:
				self.wndOtoAv.BeceriSlotlariYenile()
		def RefreshAutoPositionSlot(self):
			if self.wndOtoAv:
				self.wndOtoAv.IksirSlotlariYenile()
		def AutoOff(self):
			if self.wndOtoAv:
				self.wndOtoAv.AutoOnOff(0,self.wndOtoAv.AUTO_ONOFF_START,1,True)
			if self.wndExpandedTaskBar:
				self.wndExpandedTaskBar.EnableAutoButton(False)
		def AutoOn(self):
			if self.wndExpandedTaskBar:
				self.wndExpandedTaskBar.EnableAutoButton(True)

if __name__ == "__main__":

	import app
	import wndMgr
	import systemSetting
	import mouseModule
	import grp
	import ui
	import localeInfo

	app.SetMouseHandler(mouseModule.mouseController)
	app.SetHairColorEnable(True)
	wndMgr.SetMouseHandler(mouseModule.mouseController)
	wndMgr.SetScreenSize(systemSetting.GetWidth(), systemSetting.GetHeight())
	app.Create(localeInfo.APP_TITLE, systemSetting.GetWidth(), systemSetting.GetHeight(), 1)
	mouseModule.mouseController.Create()

	class TestGame(ui.Window):
		def __init__(self):
			ui.Window.__init__(self)

			localeInfo.LoadLocaleData()
			player.SetItemData(0, 27001, 10)
			player.SetItemData(1, 27004, 10)

			self.interface = Interface()
			self.interface.MakeInterface()
			self.interface.ShowDefaultWindows()
			self.interface.RefreshInventory()
			#self.interface.OpenCubeWindow()

		def __del__(self):
			ui.Window.__del__(self)

		def OnUpdate(self):
			app.UpdateGame()

		def OnRender(self):
			app.RenderGame()
			grp.PopState()
			grp.SetInterfaceRenderState()

	game = TestGame()
	game.SetSize(systemSetting.GetWidth(), systemSetting.GetHeight())
	game.Show()

	app.Loop()