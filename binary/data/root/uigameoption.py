import ui
import snd
import systemSetting
import net
import chat
import app
import localeInfo
import constInfo
import chrmgr
import player
import uiPrivateShopBuilder # ±Ë¡ÿ»£
import interfaceModule # ±Ë¡ÿ»£
import uiOfflineShopBuilder

if app.ENABLE_RENDER_TARGET_SYSTEM:
	import cfg

blockMode = 0
viewChatMode = 0

MOBILE = False

if localeInfo.IsYMIR():
	MOBILE = True


class OptionDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Load()
		self.RefreshViewChat()
		self.RefreshAlwaysShowName()
		self.RefreshShowDamage()
		self.RefreshShowSalesText()
		self.RefreshObjeler()
		if app.PERFORMANCE_FEATURES:
			self.RefreshDogMode()
			self.RefreshEffectHide()
		if app.ENABLE_RENDER_TARGET_SYSTEM:
			self.RefreshShowRenderTarget()
		self.RefreshShowOutline()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print " -------------------------------------- DELETE GAME OPTION DIALOG"

	def __Initialize(self):
		self.titleBar = 0
		self.nameColorModeButtonList = []
		self.viewTargetBoardButtonList = []
		self.pvpModeButtonDict = {}
		self.blockButtonList = []
		self.viewChatButtonList = []
		self.alwaysShowNameButtonList = []
		self.showDamageButtonList = []
		self.showsalesTextButtonList = []
		self.AutoPickUpButtonList = []
		self.premiumbuton = []
		self.YangdropButtonList = []
		self.gizlenecekobjeler = []
		if app.PERFORMANCE_FEATURES:
			self.efektGGButtonlar = []
			self.dogModeButtonList = []
		if app.ENABLE_RENDER_TARGET_SYSTEM:
			self.showRenderTarget = []
		self.showOutline = []

	def Destroy(self):
		self.ClearDictionary()

		self.__Initialize()
		print " -------------------------------------- DESTROY GAME OPTION DIALOG"
	
	def __Load_LoadScript(self, fileName):
		try:
			pyScriptLoader = ui.PythonScriptLoader()
			pyScriptLoader.LoadScriptFile(self, fileName)
		except:
			import exception
			exception.Abort("OptionDialog.__Load_LoadScript")

	def __Load_BindObject(self):
		try:
			GetObject = self.GetChild
			self.titleBar = GetObject("titlebar")
			self.nameColorModeButtonList.append(GetObject("name_color_normal"))
			self.nameColorModeButtonList.append(GetObject("name_color_empire"))
			self.viewTargetBoardButtonList.append(GetObject("target_board_no_view"))
			self.viewTargetBoardButtonList.append(GetObject("target_board_view"))
			self.pvpModeButtonDict[player.PK_MODE_PEACE] = GetObject("pvp_peace")
			self.pvpModeButtonDict[player.PK_MODE_REVENGE] = GetObject("pvp_revenge")
			self.pvpModeButtonDict[player.PK_MODE_GUILD] = GetObject("pvp_guild")
			self.pvpModeButtonDict[player.PK_MODE_FREE] = GetObject("pvp_free")
			self.blockButtonList.append(GetObject("block_exchange_button"))
			self.blockButtonList.append(GetObject("block_party_button"))
			self.blockButtonList.append(GetObject("block_guild_button"))
			self.blockButtonList.append(GetObject("block_whisper_button"))
			self.blockButtonList.append(GetObject("block_friend_button"))
			self.blockButtonList.append(GetObject("block_party_request_button"))
			self.viewChatButtonList.append(GetObject("view_chat_on_button"))
			self.viewChatButtonList.append(GetObject("view_chat_off_button"))
			self.alwaysShowNameButtonList.append(GetObject("always_show_name_on_button"))
			self.alwaysShowNameButtonList.append(GetObject("always_show_name_off_button"))
			self.showDamageButtonList.append(GetObject("show_damage_on_button"))
			self.showDamageButtonList.append(GetObject("show_damage_off_button"))
			self.showsalesTextButtonList.append(GetObject("salestext_on_button"))
			self.showsalesTextButtonList.append(GetObject("salestext_off_button"))
			self.AutoPickUpButtonList.append(GetObject("auto_pick_up_on_button"))
			self.AutoPickUpButtonList.append(GetObject("auto_pick_up_off_button"))
			self.premiumbuton.append(GetObject("affecton"))		
			self.premiumbuton.append(GetObject("affectoff"))
			#yangdrop
			self.YangdropButtonList.append(GetObject("yangdrop_on_button"))
			self.YangdropButtonList.append(GetObject("yangdrop_off_button"))
			#yangdrop			
			self.gizlenecekobjeler.append(GetObject("obje_pazar"))
			self.gizlenecekobjeler.append(GetObject("obje_binek"))
			self.gizlenecekobjeler.append(GetObject("obje_pet"))
			self.gizlenecekobjeler.append(GetObject("obje_lvpet"))
			
			if app.PERFORMANCE_FEATURES:
				self.efektGGButtonlar.append(GetObject("efekt_Gizle"))
				self.efektGGButtonlar.append(GetObject("efekt_Goster"))
				self.dogModeButtonList.append(GetObject("dog_mode_open"))
				self.dogModeButtonList.append(GetObject("dog_mode_close"))

			if app.ENABLE_RENDER_TARGET_SYSTEM:
				self.showRenderTarget.append(GetObject("RenderTarget_on_button"))
				self.showRenderTarget.append(GetObject("RenderTarget_off_button"))

			self.showOutline.append(GetObject("Outline_on_button"))
			self.showOutline.append(GetObject("Outline_off_button"))

			global MOBILE
			if MOBILE:
				self.inputMobileButton = GetObject("input_mobile_button")
				self.deleteMobileButton = GetObject("delete_mobile_button")


		except:
			import exception
			exception.Abort("OptionDialog.__Load_BindObject")

	def __Load(self):
		global MOBILE
		if MOBILE:
			self.__Load_LoadScript("uiscript/gameoptiondialog_formobile.py")
		else:
			self.__Load_LoadScript("uiscript/gameoptiondialog.py")

		self.__Load_BindObject()

		self.SetCenterPosition()

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

		self.nameColorModeButtonList[0].SAFE_SetEvent(self.__OnClickNameColorModeNormalButton)
		self.nameColorModeButtonList[1].SAFE_SetEvent(self.__OnClickNameColorModeEmpireButton)

		self.viewTargetBoardButtonList[0].SAFE_SetEvent(self.__OnClickTargetBoardViewButton)
		self.viewTargetBoardButtonList[1].SAFE_SetEvent(self.__OnClickTargetBoardNoViewButton)

		self.pvpModeButtonDict[player.PK_MODE_PEACE].SAFE_SetEvent(self.__OnClickPvPModePeaceButton)
		self.pvpModeButtonDict[player.PK_MODE_REVENGE].SAFE_SetEvent(self.__OnClickPvPModeRevengeButton)
		self.pvpModeButtonDict[player.PK_MODE_GUILD].SAFE_SetEvent(self.__OnClickPvPModeGuildButton)
		self.pvpModeButtonDict[player.PK_MODE_FREE].SAFE_SetEvent(self.__OnClickPvPModeFreeButton)

		self.blockButtonList[0].SetToggleUpEvent(self.__OnClickBlockExchangeButton)
		self.blockButtonList[1].SetToggleUpEvent(self.__OnClickBlockPartyButton)
		self.blockButtonList[2].SetToggleUpEvent(self.__OnClickBlockGuildButton)
		self.blockButtonList[3].SetToggleUpEvent(self.__OnClickBlockWhisperButton)
		self.blockButtonList[4].SetToggleUpEvent(self.__OnClickBlockFriendButton)
		self.blockButtonList[5].SetToggleUpEvent(self.__OnClickBlockPartyRequest)

		self.blockButtonList[0].SetToggleDownEvent(self.__OnClickBlockExchangeButton)
		self.blockButtonList[1].SetToggleDownEvent(self.__OnClickBlockPartyButton)
		self.blockButtonList[2].SetToggleDownEvent(self.__OnClickBlockGuildButton)
		self.blockButtonList[3].SetToggleDownEvent(self.__OnClickBlockWhisperButton)
		self.blockButtonList[4].SetToggleDownEvent(self.__OnClickBlockFriendButton)
		self.blockButtonList[5].SetToggleDownEvent(self.__OnClickBlockPartyRequest)

		self.viewChatButtonList[0].SAFE_SetEvent(self.__OnClickViewChatOnButton)
		self.viewChatButtonList[1].SAFE_SetEvent(self.__OnClickViewChatOffButton)

		self.alwaysShowNameButtonList[0].SAFE_SetEvent(self.__OnClickAlwaysShowNameOnButton)
		self.alwaysShowNameButtonList[1].SAFE_SetEvent(self.__OnClickAlwaysShowNameOffButton)

		self.showDamageButtonList[0].SAFE_SetEvent(self.__OnClickShowDamageOnButton)
		self.showDamageButtonList[1].SAFE_SetEvent(self.__OnClickShowDamageOffButton)
		self.AutoPickUpButtonList[0].SAFE_SetEvent(self.__OnClickEnableAutoPickUp)
		self.AutoPickUpButtonList[1].SAFE_SetEvent(self.__OnClickDisableAutoPickUp)	
		
		self.showsalesTextButtonList[0].SAFE_SetEvent(self.__OnClickSalesTextOnButton)
		self.showsalesTextButtonList[1].SAFE_SetEvent(self.__OnClickSalesTextOffButton)		

		
		self.premiumbuton[0].SAFE_SetEvent(self.__premiumfuncac)
		self.premiumbuton[1].SAFE_SetEvent(self.__premiumfunckapa)			
		
		#yangdrop
		self.YangdropButtonList[0].SAFE_SetEvent(self.__OnClickEnableYangdrop)
		self.YangdropButtonList[1].SAFE_SetEvent(self.__OnClickDisableYangdrop)
		#yangdrop		
		
		self.gizlenecekobjeler[0].SetToggleUpEvent(self.obje_pazar)
		self.gizlenecekobjeler[1].SetToggleUpEvent(self.obje_binek)
		self.gizlenecekobjeler[2].SetToggleUpEvent(self.obje_pet)
		self.gizlenecekobjeler[3].SetToggleUpEvent(self.obje_lvpet)		
		
		self.gizlenecekobjeler[0].SetToggleDownEvent(self.obje_pazar)
		self.gizlenecekobjeler[1].SetToggleDownEvent(self.obje_binek)
		self.gizlenecekobjeler[2].SetToggleDownEvent(self.obje_pet)
		self.gizlenecekobjeler[3].SetToggleDownEvent(self.obje_lvpet)
		
		self.__ClickRadioButton(self.nameColorModeButtonList, constInfo.GET_CHRNAME_COLOR_INDEX())
		self.__ClickRadioButton(self.viewTargetBoardButtonList, constInfo.GET_VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD())
		self.__SetPeacePKMode()
		
		if app.PERFORMANCE_FEATURES:
			self.dogModeButtonList[0].SAFE_SetEvent(self.__OnClickDogButton)
			self.dogModeButtonList[1].SAFE_SetEvent(self.__OffClickDogButton)
			self.efektGGButtonlar[0].SetEvent(ui.__mem_func__(self.EfektAyar), 0)
			self.efektGGButtonlar[1].SetEvent(ui.__mem_func__(self.EfektAyar), 1)

		if app.ENABLE_RENDER_TARGET_SYSTEM:
			self.showRenderTarget[0].SAFE_SetEvent(self.__OnClickRenderTargetOnButton)
			self.showRenderTarget[1].SAFE_SetEvent(self.__OnClickRenderTargetOffButton)

		self.showOutline[0].SAFE_SetEvent(self.__OnClickOutlineOnButton)
		self.showOutline[1].SAFE_SetEvent(self.__OnClickOutlineOffButton)

		#global MOBILE
		if MOBILE:
			self.inputMobileButton.SetEvent(ui.__mem_func__(self.__OnChangeMobilePhoneNumber))
			self.deleteMobileButton.SetEvent(ui.__mem_func__(self.__OnDeleteMobilePhoneNumber))

	def __ClickRadioButton(self, buttonList, buttonIndex):
		try:
			selButton=buttonList[buttonIndex]
		except IndexError:
			return

		for eachButton in buttonList:
			eachButton.SetUp()

		selButton.Down()

	def __SetNameColorMode(self, index):
		constInfo.SET_CHRNAME_COLOR_INDEX(index)
		self.__ClickRadioButton(self.nameColorModeButtonList, index)

	def __SetTargetBoardViewMode(self, flag):
		constInfo.SET_VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD(flag)
		self.__ClickRadioButton(self.viewTargetBoardButtonList, flag)

	def __OnClickNameColorModeNormalButton(self):
		self.__SetNameColorMode(0)

	def __OnClickNameColorModeEmpireButton(self):
		self.__SetNameColorMode(1)

	def __OnClickTargetBoardViewButton(self):
		self.__SetTargetBoardViewMode(0)

	def __OnClickTargetBoardNoViewButton(self):
		self.__SetTargetBoardViewMode(1)

	def __OnClickCameraModeShortButton(self):
		self.__SetCameraMode(0)

	def __OnClickCameraModeLongButton(self):
		self.__SetCameraMode(1)

	def __OnClickFogModeLevel0Button(self):
		self.__SetFogLevel(0)

	def __OnClickFogModeLevel1Button(self):
		self.__SetFogLevel(1)

	def __OnClickFogModeLevel2Button(self):
		self.__SetFogLevel(2)

	def __OnClickBlockExchangeButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_EXCHANGE))
	def __OnClickBlockPartyButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_PARTY))
	def __OnClickBlockGuildButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_GUILD))
	def __OnClickBlockWhisperButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_WHISPER))
	def __OnClickBlockFriendButton(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_FRIEND))
	def __OnClickBlockPartyRequest(self):
		self.RefreshBlock()
		global blockMode
		net.SendChatPacket("/setblockmode " + str(blockMode ^ player.BLOCK_PARTY_REQUEST))

	def __OnClickViewChatOnButton(self):
		global viewChatMode
		viewChatMode = 1
		systemSetting.SetViewChatFlag(viewChatMode)
		self.RefreshViewChat()
	def __OnClickViewChatOffButton(self):
		global viewChatMode
		viewChatMode = 0
		systemSetting.SetViewChatFlag(viewChatMode)
		self.RefreshViewChat()

	def __OnClickAlwaysShowNameOnButton(self):
		systemSetting.SetAlwaysShowNameFlag(True)
		self.RefreshAlwaysShowName()

	def __OnClickAlwaysShowNameOffButton(self):
		systemSetting.SetAlwaysShowNameFlag(False)
		self.RefreshAlwaysShowName()

	def __OnClickShowDamageOnButton(self):
		systemSetting.SetShowDamageFlag(True)
		self.RefreshShowDamage()

	def __OnClickShowDamageOffButton(self):
		systemSetting.SetShowDamageFlag(False)
		self.RefreshShowDamage()
		
	def __OnClickSalesTextOnButton(self):
		systemSetting.SetShowSalesTextFlag(True)
		self.RefreshShowSalesText()
		uiPrivateShopBuilder.UpdateADBoard()
		uiOfflineShopBuilder.UpdateADBoard()
		
	def __OnClickSalesTextOffButton(self):
		systemSetting.SetShowSalesTextFlag(False)
		self.RefreshShowSalesText()		
		
	def __OnClickSalesTextOffButton(self):
		systemSetting.SetShowSalesTextFlag(FALSE)
		self.RefreshShowSalesText()		
		
	def __OnClickEnableAutoPickUp(self):
		self.AutoPickUpButtonList[0].Down()
		self.AutoPickUpButtonList[1].SetUp()

		constInfo.AUTO_PICK_UP = 1
		chat.AppendChat(chat.CHAT_TYPE_INFO, "Oto toplama aktif.")

	def __OnClickDisableAutoPickUp(self):
		self.AutoPickUpButtonList[0].SetUp()
		self.AutoPickUpButtonList[1].Down()

		constInfo.AUTO_PICK_UP = 0
		chat.AppendChat(chat.CHAT_TYPE_INFO, "Oto toplama kapali.")		
		
		
	def __premiumfuncac(self):
		constInfo.Premium = 0
		self.RefreshPremium()
		
	def __premiumfunckapa(self):
		constInfo.Premium = 1
		self.RefreshPremium()		
		
	def RefreshPremium(self):
		if constInfo.Premium == 0:
			self.premiumbuton[0].Down()
			self.premiumbuton[1].SetUp()
		else:
			self.premiumbuton[1].Down()
			self.premiumbuton[0].SetUp()		
			
	#Yangdrop	
	def __OnClickSalesTextOffButton(self):
		systemSetting.SetShowSalesTextFlag(FALSE)
		self.RefreshShowSalesText()		
		
	def __OnClickEnableYangdrop(self):
		self.YangdropButtonList[0].Down()
		self.YangdropButtonList[1].SetUp()

		constInfo.YangDrop = 1
		chat.AppendChat(chat.CHAT_TYPE_INFO, "|cffFFC125Yang Drop Bilgisi Gosterme Aktif.")

	def __OnClickDisableYangdrop(self):
		self.YangdropButtonList[0].SetUp()
		self.YangdropButtonList[1].Down()

		constInfo.YangDrop = 0
		chat.AppendChat(chat.CHAT_TYPE_INFO, "|cffFFC125Yang Drop Gosterme Deaktif.")
	#Yangdrop			
		
	def __CheckPvPProtectedLevelPlayer(self):	
		if player.GetStatus(player.LEVEL)<constInfo.PVPMODE_PROTECTED_LEVEL:
			self.__SetPeacePKMode()
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_PROTECT % (constInfo.PVPMODE_PROTECTED_LEVEL))
			return 1

		return 0

	def __SetPKMode(self, mode):
		for btn in self.pvpModeButtonDict.values():
			btn.SetUp()
		if self.pvpModeButtonDict.has_key(mode):
			self.pvpModeButtonDict[mode].Down()

	def __SetPeacePKMode(self):
		self.__SetPKMode(player.PK_MODE_PEACE)

	def __RefreshPVPButtonList(self):
		self.__SetPKMode(player.GetPKMode())

	def __OnClickPvPModePeaceButton(self):
		if self.__CheckPvPProtectedLevelPlayer():
			return

		self.__RefreshPVPButtonList()

		if constInfo.PVPMODE_ENABLE:
			net.SendChatPacket("/pkmode 0", chat.CHAT_TYPE_TALKING)
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_NOT_SUPPORT)

	def __OnClickPvPModeRevengeButton(self):
		if self.__CheckPvPProtectedLevelPlayer():
			return

		self.__RefreshPVPButtonList()

		if constInfo.PVPMODE_ENABLE:
			net.SendChatPacket("/pkmode 1", chat.CHAT_TYPE_TALKING)
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_NOT_SUPPORT)

	def __OnClickPvPModeFreeButton(self):
		if self.__CheckPvPProtectedLevelPlayer():
			return

		self.__RefreshPVPButtonList()

		if constInfo.PVPMODE_ENABLE:
			net.SendChatPacket("/pkmode 2", chat.CHAT_TYPE_TALKING)
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_NOT_SUPPORT)

	def __OnClickPvPModeGuildButton(self):
		if self.__CheckPvPProtectedLevelPlayer():
			return

		self.__RefreshPVPButtonList()

		if 0 == player.GetGuildID():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_CANNOT_SET_GUILD_MODE)
			return

		if constInfo.PVPMODE_ENABLE:
			net.SendChatPacket("/pkmode 4", chat.CHAT_TYPE_TALKING)
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_NOT_SUPPORT)

	def OnChangePKMode(self):
		self.__RefreshPVPButtonList()

	def __OnChangeMobilePhoneNumber(self):
		global MOBILE
		if not MOBILE:
			return

		import uiCommon
		inputDialog = uiCommon.InputDialog()
		inputDialog.SetTitle(localeInfo.MESSENGER_INPUT_MOBILE_PHONE_NUMBER_TITLE)
		inputDialog.SetMaxLength(13)
		inputDialog.SetAcceptEvent(ui.__mem_func__(self.OnInputMobilePhoneNumber))
		inputDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseInputDialog))
		inputDialog.Open()
		self.inputDialog = inputDialog

	def __OnDeleteMobilePhoneNumber(self):
		global MOBILE
		if not MOBILE:
			return

		import uiCommon
		questionDialog = uiCommon.QuestionDialog()
		questionDialog.SetText(localeInfo.MESSENGER_DO_YOU_DELETE_PHONE_NUMBER)
		questionDialog.SetAcceptEvent(ui.__mem_func__(self.OnDeleteMobile))
		questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
		questionDialog.Open()
		self.questionDialog = questionDialog

	def OnInputMobilePhoneNumber(self):
		global MOBILE
		if not MOBILE:
			return

		text = self.inputDialog.GetText()

		if not text:
			return

		text.replace('-', '')
		net.SendChatPacket("/mobile " + text)
		self.OnCloseInputDialog()
		return True

	def OnInputMobileAuthorityCode(self):
		global MOBILE
		if not MOBILE:
			return

		text = self.inputDialog.GetText()
		net.SendChatPacket("/mobile_auth " + text)
		self.OnCloseInputDialog()
		return True

	def OnDeleteMobile(self):
		global MOBILE
		if not MOBILE:
			return

		net.SendChatPacket("/mobile")
		self.OnCloseQuestionDialog()
		return True

	def OnCloseInputDialog(self):
		self.inputDialog.Close()
		self.inputDialog = None
		return True

	def OnCloseQuestionDialog(self):
		self.questionDialog.Close()
		self.questionDialog = None
		return True

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def RefreshMobile(self):
		global MOBILE
		if not MOBILE:
			return

		if player.HasMobilePhoneNumber():
			self.inputMobileButton.Hide()
			self.deleteMobileButton.Show()
		else:
			self.inputMobileButton.Show()
			self.deleteMobileButton.Hide()

	def OnMobileAuthority(self):
		global MOBILE
		if not MOBILE:
			return

		import uiCommon
		inputDialog = uiCommon.InputDialogWithDescription()
		inputDialog.SetTitle(localeInfo.MESSENGER_INPUT_MOBILE_AUTHORITY_TITLE)
		inputDialog.SetDescription(localeInfo.MESSENGER_INPUT_MOBILE_AUTHORITY_DESCRIPTION)
		inputDialog.SetAcceptEvent(ui.__mem_func__(self.OnInputMobileAuthorityCode))
		inputDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseInputDialog))
		inputDialog.SetMaxLength(4)
		inputDialog.SetBoardWidth(310)
		inputDialog.Open()
		self.inputDialog = inputDialog

	def RefreshBlock(self):
		global blockMode
		for i in xrange(len(self.blockButtonList)):
			if 0 != (blockMode & (1 << i)):
				self.blockButtonList[i].Down()
			else:
				self.blockButtonList[i].SetUp()

	def RefreshViewChat(self):
		if systemSetting.IsViewChat():
			self.viewChatButtonList[0].Down()
			self.viewChatButtonList[1].SetUp()
		else:
			self.viewChatButtonList[0].SetUp()
			self.viewChatButtonList[1].Down()

	def RefreshAlwaysShowName(self):
		if systemSetting.IsAlwaysShowName():
			self.alwaysShowNameButtonList[0].Down()
			self.alwaysShowNameButtonList[1].SetUp()
		else:
			self.alwaysShowNameButtonList[0].SetUp()
			self.alwaysShowNameButtonList[1].Down()

	def RefreshShowDamage(self):
		if systemSetting.IsShowDamage():
			self.showDamageButtonList[0].Down()
			self.showDamageButtonList[1].SetUp()
		else:
			self.showDamageButtonList[0].SetUp()
			self.showDamageButtonList[1].Down()
			
	def RefreshShowSalesText(self):
		if systemSetting.IsShowSalesText():
			self.showsalesTextButtonList[0].Down()
			self.showsalesTextButtonList[1].SetUp()
		else:
			self.showsalesTextButtonList[0].SetUp()
			self.showsalesTextButtonList[1].Down()
			
	def obje_pazar(self):
		if systemSetting.IsShowOfflineShop() == 1:
			systemSetting.SetShowOfflineShop(0)
			systemSetting.SetShowSalesTextFlag(False)
		else:
			systemSetting.SetShowOfflineShop(1)
			systemSetting.SetShowSalesTextFlag(True)
		
		self.RefreshObjeler()
		self.RefreshShowSalesText()
		uiPrivateShopBuilder.UpdateADBoard()
		uiOfflineShopBuilder.UpdateADBoard()
		
		
	def obje_binek(self):
		if systemSetting.IsHideMounts() == 1:
			systemSetting.SetHideMounts(False)
		else:
			systemSetting.SetHideMounts(True)
		
		self.RefreshObjeler()
		
	def obje_pet(self):
		if systemSetting.IsHidePets() == 1:
			systemSetting.SetHidePets(False)
		else:
			systemSetting.SetHidePets(True)
		
		self.RefreshObjeler()
		
	def obje_lvpet(self):
		if systemSetting.IsHideNewPets() == 1:
			systemSetting.SetHideNewPets(False)
		else:
			systemSetting.SetHideNewPets(True)
		
		self.RefreshObjeler()
		
	def RefreshObjeler(self):
		if systemSetting.IsShowOfflineShop() == 1:
			self.gizlenecekobjeler[0].SetUp()
		else:
			self.gizlenecekobjeler[0].Down()
			
		if systemSetting.IsHideMounts() == 1:
			self.gizlenecekobjeler[1].Down()
		else:
			self.gizlenecekobjeler[1].SetUp()
			
		if systemSetting.IsHidePets() == 1:
			self.gizlenecekobjeler[2].Down()
		else:
			self.gizlenecekobjeler[2].SetUp()
			
		if systemSetting.IsHideNewPets() == 1:
			self.gizlenecekobjeler[3].Down()
		else:
			self.gizlenecekobjeler[3].SetUp()

	def OnBlockMode(self, mode):
		global blockMode
		blockMode = mode
		self.RefreshBlock()

	def Show(self):
		self.RefreshMobile()
		self.RefreshBlock()
		ui.ScriptWindow.Show(self)

	def Close(self):
		self.Hide()

	if app.PERFORMANCE_FEATURES:
		def __OnClickDogButton(self):
			systemSetting.SetDogMode(True)
			self.RefreshDogMode()

		def __OffClickDogButton(self):
			systemSetting.SetDogMode(False)
			self.RefreshDogMode()

		def RefreshDogMode(self):
			if systemSetting.GetDogMode():
				self.dogModeButtonList[0].Down()
				self.dogModeButtonList[1].SetUp()
			else:
				self.dogModeButtonList[0].SetUp()
				self.dogModeButtonList[1].Down()

		def RefreshEffectHide(self):
			if not systemSetting.GetEffectHide():
				self.efektGGButtonlar[0].Down()
				self.efektGGButtonlar[1].SetUp()
			else:
				self.efektGGButtonlar[0].SetUp()
				self.efektGGButtonlar[1].Down()

		def EfektAyar(self, gelenVeri):
			systemSetting.EfektGG(gelenVeri)
			self.efektGGButtonlar[gelenVeri].Down()
			self.RefreshEffectHide()

	if app.ENABLE_RENDER_TARGET_SYSTEM:
		def __OnClickRenderTargetOnButton(self):
			cfg.Set(cfg.SAVE_OPTION, "RENDER_TARGET", 1)
			self.RefreshShowRenderTarget()

		def __OnClickRenderTargetOffButton(self):
			cfg.Set(cfg.SAVE_OPTION, "RENDER_TARGET", 0)
			self.RefreshShowRenderTarget()

		def RefreshShowRenderTarget(self):
			if cfg.Get(cfg.SAVE_OPTION, "RENDER_TARGET", "0") == "1":
				self.showRenderTarget[0].Down()
				self.showRenderTarget[1].SetUp()
			else:
				self.showRenderTarget[0].SetUp()
				self.showRenderTarget[1].Down()

	def __OnClickOutlineOnButton(self):
		systemSetting.SetShowOutline(TRUE)
		self.RefreshShowOutline()

	def __OnClickOutlineOffButton(self):
		systemSetting.SetShowOutline(FALSE)
		self.RefreshShowOutline()

	def RefreshShowOutline(self):
		if systemSetting.IsShowOutline():
			self.showOutline[0].Down()
			self.showOutline[1].SetUp()
		else:
			self.showOutline[0].SetUp()
			self.showOutline[1].Down()
