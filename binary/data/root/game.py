import os
import uimaintenance
import app
import dbg
import grp
import item
import background
import chr
import chrmgr
import player
import snd
import chat
import event
import textTail
import net
import effect
import wndMgr
import fly
import systemSetting
import quest
import guild
import skill
import messenger
import localeInfo
import constInfo
import uiruhtasi
import exchange
import ime
import uiakiraevent
import uiAkiraEvent
import uiAkiraMenu
import skybox
# -------------------
# 5Lv Oto Bec Sistemi
import savbec
import surabec
import ninjabec
import samanbec
# -------------------
import ui
import uiCommon
import uiPhaseCurtain
import uiMapNameShower
import uiAffectShower
import uiPlayerGauge
import uiCharacter
import uiTarget
import uibiyolog
import uibiyologodul
import uiDungeonTimer
import uipetfeed
# PRIVATE_SHOP_PRICE_LIST
import uiPrivateShopBuilder
# END_OF_PRIVATE_SHOP_PRICE_LIST

# OFFLINE_SHOP_PRICE_LIST
import uiOfflineShopBuilder
# END_OF_OFFLINE_SHOP_PRICE_LIST
import uiOfflineShop
import mouseModule
import consoleModule
import localeInfo
import playerSettingModule
import interfaceModule
import uiWhisperAdmin
import musicInfo
import debugInfo
import stringCommander

svsidedia = None
svsidedi_cp =  ""
from svsideoi import SvsideDialog
import binascii
from _weakref import proxy
import uibosstracking
import uipricetrans
import uiTip
# TEXTTAIL_LIVINGTIME_CONTROL
#if localeInfo.IsJAPAN():
#	app.SetTextTailLivingTime(8.0)
# END_OF_TEXTTAIL_LIVINGTIME_CONTROL

# SCREENSHOT_CWDSAVE
if app.__ENABLE_NEW_OFFLINESHOP__:
	import uinewofflineshop
	import offlineshop

if app.ENABLE_NEW_PET_SYSTEM:
	import uipetsystem

SCREENSHOT_CWDSAVE = False
SCREENSHOT_DIR = None

if localeInfo.IsEUROPE():
	SCREENSHOT_CWDSAVE = True

if localeInfo.IsCIBN10():
	SCREENSHOT_CWDSAVE = False
	SCREENSHOT_DIR = "YT2W"

cameraDistance = 1550.0
cameraPitch = 27.0
cameraRotation = 0.0
cameraHeight = 100.0

testAlignment = 0

class GameWindow(ui.ScriptWindow):
	def __init__(self, stream):
		ui.ScriptWindow.__init__(self, "GAME")
		self.SetWindowName("game")
		net.SetPhaseWindow(net.PHASE_WINDOW_GAME, self)
		player.SetGameWindow(self)
		self.ruhtasi = uiruhtasi.RuhTasi()
		self.ruhtasi.Hide()
		
		global svsidedia
		if svsidedia == None:
			svsidedia = SvsideDialog()
		svsidedia.Board.Hide()

		self.quickSlotPageIndex = 0
		self.lastPKModeSendedTime = 0
		self.pressNumber = None

		self.guildWarQuestionDialog = None
		self.interface = None
		self.skybox = None
		self.targetBoard = None
		self.console = None
		self.mapNameShower = None
		self.affectShower = None
		self.playerGauge = None
		if app.__ENABLE_NEW_OFFLINESHOP__:
			offlineshop.HideShopNames()
			self.Offlineshop = uinewofflineshop.NewOfflineShopBoard()
			self.Offlineshop.Hide()
		self.UiSaplingSwitchbot = None
		self.DungeonTimer = None
		if app.AUTO_SHOUT:
			self.shouttime = 0
		
		self.stream=stream
		self.interface = interfaceModule.Interface()
		self.interface.SetStream(self.stream)
		self.interface.MakeInterface()
		self.interface.ShowDefaultWindows()
		self.stream.isAutoSelect = 0

		self.curtain = uiPhaseCurtain.PhaseCurtain()
		self.curtain.speed = 0.03
		self.curtain.Hide()
		self.biyoekran = uibiyologodul.BiyologEkran()
		self.biyoekran.Hide()

		self.targetBoard = uiTarget.TargetBoard()
		self.targetBoard.SetWhisperEvent(ui.__mem_func__(self.interface.OpenWhisperDialog))
		self.targetBoard.Hide()
		self.akiraEventButton = uiakiraevent.AkiraEventButton()
		self.akiraEventButton.LoadWindow()
		self.akiraEventButton.Hide()

		if app.ENABLE_NEW_PET_SYSTEM:
			self.petmain = uipetsystem.PetSystemMain()
			if (app.ENABLE_PET_ATTR_DETERMINE):
				self.petmain.SetItemToolTip(self.interface.tooltipItem)
				self.petmain.BindInterface(self.interface)
				self.petmini = uipetsystem.PetSystemMini()

		self.console = consoleModule.ConsoleWindow()
		self.console.BindGameClass(self)
		self.console.SetConsoleSize(wndMgr.GetScreenWidth(), 200)
		self.console.Hide()

		self.mapNameShower = uiMapNameShower.MapNameShower()
		self.affectShower = uiAffectShower.AffectShower()
		self.wndMaintenance = uimaintenance.MaintenanceClass()
		self.playerGauge = uiPlayerGauge.PlayerGauge(self)
		self.playerGauge.Hide()

		self.whisperAdmin = uiWhisperAdmin.AdminWhisperManager()
		self.whisperAdmin.Close()

		self.bosstracking = uibosstracking.BossTrackingInfoWindow()
		self.bosstracking.Hide()
		
		self.chequetogold = uipricetrans.ChequeToGoldWindow()
		self.chequetogold.Hide()
		
		self.goldtocheque = uipricetrans.GoldToChequeWindow()
		self.goldtocheque.Hide()
		
		if app.ENABLE_TITLE_SYSTEM:
			import title_system
			self.wndTitleSystem = title_system.Title_System()

		#wj 2014.1.2. ESCÅ°¸¦ ´©¸¦ ½Ã ¿ì¼±ÀûÀ¸·Î DropQuestionDialog¸¦ ²ôµµ·Ï ¸¸µé¾ú´Ù. ÇÏÁö¸¸ Ã³À½¿¡ itemDropQuestionDialog°¡ ¼±¾ğµÇ¾î ÀÖÁö ¾Ê¾Æ ERROR°¡ ¹ß»ıÇÏ¿© init¿¡¼­ ¼±¾ğ°ú µ¿½Ã¿¡ ÃÊ±âÈ­ ½ÃÅ´.
		self.itemDropQuestionDialog = None

		self.__SetQuickSlotMode()
		self.__ServerCommand_Build()
		self.__ProcessPreservedServerCommand()
		self.pingLine = None
		if app.ENABLE_PINGTIME:
			self.pingLine = ui.TextLine()
			self.pingLine.SetFontName(localeInfo.UI_DEF_FONT)
			self.pingLine.SetFontColor(1.0, 1.0, 1.0)
			self.pingLine.SetPosition((wndMgr.GetScreenWidth() - 110) / 2, 157)

	if app.ENABLE_KILL_STATISTICS:
		def ReceiveKillStatisticsPacket(self, dw, dl, b, st):
			constInfo.KILL_STATISTICS_DATA = [int(dw), int(dl), int(b), int(st),]

	def __del__(self):
		player.SetGameWindow(0)
		net.ClearPhaseWindow(net.PHASE_WINDOW_GAME, self)
		ui.ScriptWindow.__del__(self)

	def Open(self):
		app.SetFrameSkip(1)

		self.SetSize(wndMgr.GetScreenWidth(), wndMgr.GetScreenHeight())

		self.BoardMessage = uiTip.BigBoard()
		
		self.quickSlotPageIndex = 0
		self.PickingCharacterIndex = -1
		self.PickingItemIndex = -1
		self.consoleEnable = False
		self.isShowDebugInfo = False
		self.ShowNameFlag = False
		
		self.enableXMasBoom = False
		self.startTimeXMasBoom = 0.0
		self.indexXMasBoom = 0
		self.mapNameTextLine = ui.TextLine()
		self.mapNameTextLine.SetWindowHorizontalAlignCenter()
		self.mapNameTextLine.SetHorizontalAlignCenter()
		self.mapNameTextLine.SetFontName(localeInfo.UI_DEF_FONT)
		self.mapNameTextLine.SetFontColor(10, 10, 0)
		self.mapNameTextLine.SetFeather()
		self.mapNameTextLine.SetOutline()
		self.mapNameTextLine.Show()
		global cameraDistance, cameraPitch, cameraRotation, cameraHeight

		app.SetCamera(cameraDistance, cameraPitch, cameraRotation, cameraHeight)

		constInfo.SET_DEFAULT_CAMERA_MAX_DISTANCE()
		constInfo.SET_DEFAULT_CHRNAME_COLOR()
		constInfo.SET_DEFAULT_FOG_LEVEL()
		constInfo.SET_DEFAULT_CONVERT_EMPIRE_LANGUAGE_ENABLE()
		constInfo.SET_DEFAULT_USE_ITEM_WEAPON_TABLE_ATTACK_BONUS()
		constInfo.SET_DEFAULT_USE_SKILL_EFFECT_ENABLE()

		# TWO_HANDED_WEAPON_ATTACK_SPEED_UP
		constInfo.SET_TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE()
		# END_OF_TWO_HANDED_WEAPON_ATTACK_SPEED_UP

		import event
		event.SetLeftTimeString(localeInfo.UI_LEFT_TIME)

		textTail.EnablePKTitle(constInfo.PVPMODE_ENABLE)

		if constInfo.PVPMODE_TEST_ENABLE:
			self.testPKMode = ui.TextLine()
			self.testPKMode.SetFontName(localeInfo.UI_DEF_FONT)
			self.testPKMode.SetPosition(0, 15)
			self.testPKMode.SetWindowHorizontalAlignCenter()
			self.testPKMode.SetHorizontalAlignCenter()
			self.testPKMode.SetFeather()
			self.testPKMode.SetOutline()
			self.testPKMode.Show()

			self.testAlignment = ui.TextLine()
			self.testAlignment.SetFontName(localeInfo.UI_DEF_FONT)
			self.testAlignment.SetPosition(0, 35)
			self.testAlignment.SetWindowHorizontalAlignCenter()
			self.testAlignment.SetHorizontalAlignCenter()
			self.testAlignment.SetFeather()
			self.testAlignment.SetOutline()
			self.testAlignment.Show()

		self.__BuildKeyDict()
		self.__BuildDebugInfo()

		# PRIVATE_SHOP_PRICE_LIST
		uiPrivateShopBuilder.Clear()
		# END_OF_PRIVATE_SHOP_PRICE_LIST

		# OFFLINE_SHOP_PRICE_LIST
		uiOfflineShopBuilder.Clear()
		# END_OF_OFFLINE_SHOP_PRICE_LIST
		# UNKNOWN_UPDATE
		exchange.InitTrading()
		# END_OF_UNKNOWN_UPDATE

		if debugInfo.IsDebugMode():
			self.ToggleDebugInfo()

		## Sound
		snd.SetMusicVolume(systemSetting.GetMusicVolume()*net.GetFieldMusicVolume())
		snd.SetSoundVolume(systemSetting.GetSoundVolume())

		netFieldMusicFileName = net.GetFieldMusicFileName()
		if netFieldMusicFileName:
			snd.FadeInMusic("BGM/" + netFieldMusicFileName)
		elif musicInfo.fieldMusic != "":						
			snd.FadeInMusic("BGM/" + musicInfo.fieldMusic)

		self.__SetQuickSlotMode()
		self.__SelectQuickPage(self.quickSlotPageIndex)

		self.SetFocus()
		self.Show()
		app.ShowCursor()

		net.SendEnterGamePacket()
		if app.WJ_SECURITY_SYSTEM:
			if constInfo.open_security == 0:
				net.SendChatPacket("/open_security 0")

		# START_GAME_ERROR_EXIT
		try:
			self.StartGame()
		except:
			import exception
			exception.Abort("GameWindow.Open")
		# END_OF_START_GAME_ERROR_EXIT
		
		# NPC°¡ Å¥ºê½Ã½ºÅÛÀ¸·Î ¸¸µé ¼ö ÀÖ´Â ¾ÆÀÌÅÛµéÀÇ ¸ñ·ÏÀ» Ä³½Ì
		# ex) cubeInformation[20383] = [ {"rewordVNUM": 72723, "rewordCount": 1, "materialInfo": "101,1&102,2", "price": 999 }, ... ]
		self.cubeInformation = {}
		self.currentCubeNPC = 0
		if app.AUTO_SHOUT:
			self.shouttime = app.GetTime()+15
		if app.ENABLE_CHAT_COLOR_SYSTEM:
			if systemSetting.GetChatColor():
				systemSetting.SetChatColor(True)
				if not self.open_colors:
					return
				if constInfo.chat_color_page_open == 0:
					return False
				if arg == 1:
					constInfo.chat_color = "|cFFff1a1a|H|h"
				if arg == 2:
					constInfo.chat_color = "|cFF00ffe4|H|h"
				if arg == 3:
					constInfo.chat_color = "|cFF33ff33|H|h"
				if arg == 4:
					constInfo.chat_color = "|cFFFFF200|H|h"
				if arg == 5:
					constInfo.chat_color = "|cFFFF62FF|H|h"
				if arg == 6:
					constInfo.chat_color = "|cFFFF7F27|H|h"
				if arg == 7:
					constInfo.chat_color = "|cFFff9966|H|h"
				if arg == 8:
					constInfo.chat_color = "|cFFbf80ff|H|h"
			else:
				if constInfo.chat_color_page_open == 0:
					return False
		
	def Close(self):
		self.Hide()
		#AKIRA_EVENT_SYSTEM
		self.akiraEventButton.Hide()
		#END_OF_AKIRA_EVENT_SYSTEM

		global cameraDistance, cameraPitch, cameraRotation, cameraHeight
		(cameraDistance, cameraPitch, cameraRotation, cameraHeight) = app.GetCamera()

		if musicInfo.fieldMusic != "":
			snd.FadeOutMusic("BGM/"+ musicInfo.fieldMusic)

		self.onPressKeyDict = None
		self.onClickKeyDict = None

		chat.Close()
		snd.StopAllSound()
		grp.InitScreenEffect()
		chr.Destroy()
		textTail.Clear()
		quest.Clear()
		background.Destroy()
		guild.Destroy()
		messenger.Destroy()
		skill.ClearSkillData()
		wndMgr.Unlock()
		mouseModule.mouseController.DeattachObject()

		if self.guildWarQuestionDialog:
			self.guildWarQuestionDialog.Close()

		self.guildNameBoard = None
		self.partyRequestQuestionDialog = None
		self.partyInviteQuestionDialog = None
		self.guildInviteQuestionDialog = None
		self.guildWarQuestionDialog = None
		self.messengerAddFriendQuestion = None
		if app.__ENABLE_NEW_OFFLINESHOP__:
			if self.Offlineshop:
				self.Offlineshop.Destroy()
				self.Offlineshop = None
		


		# UNKNOWN_UPDATE
		self.itemDropQuestionDialog = None
		# END_OF_UNKNOWN_UPDATE

		# QUEST_CONFIRM
		self.confirmDialog = None
		# END_OF_QUEST_CONFIRM

		self.PrintCoord = None
		self.FrameRate = None
		self.Pitch = None
		self.Splat = None
		self.TextureNum = None
		self.ObjectNum = None
		self.ViewDistance = None
		self.PrintMousePos = None

		self.ClearDictionary()
		if app.ENABLE_NEW_PET_SYSTEM:
			self.petmain.Close()
			self.petmini.Close()

		self.playerGauge = None
		self.mapNameShower = None
		self.affectShower = None

		if self.whisperAdmin:
			self.whisperAdmin.Close()
			self.whisperAdmin = None

		if self.bosstracking:
			self.bosstracking.Destroy()
			self.bosstracking = None
			
		if self.chequetogold:
			self.chequetogold.Destroy()
			self.chequetogold = None
			
		if self.goldtocheque:
			self.goldtocheque.Destroy()
			self.goldtocheque = None

		if app.ENABLE_TITLE_SYSTEM:
			self.wndTitleSystem.Close()

		if self.console:
			self.console.BindGameClass(0)
			self.console.Close()
			self.console=None
		
		if self.ruhtasi:
			self.ruhtasi.Destroy()
			self.ruhtasi = None
			
		if self.wndMaintenance.IsShow():
			self.wndMaintenance.Hide()

		if self.targetBoard:
			self.targetBoard.Destroy()
			self.targetBoard = None

	
		if self.interface:
			self.interface.HideAllWindows()
			self.interface.Close()
			self.interface=None
		if app.ENABLE_PINGTIME:
			self.pingLine = None

		player.ClearSkillDict()
		player.ResetCameraRotation()

		self.KillFocus()
		app.HideCursor()

		print "---------------------------------------------------------------------------- CLOSE GAME WINDOW"

	def SpeedButtonWindowShow(self):
		self.speedButton.Show()

	def ruhcac(self):
		if constInfo.ITEM_REMOVE_WINDOW_STATUS == 1:
			return
		self.ruhtasi.Show()

	def __BuildKeyDict(self):
		onPressKeyDict = {}

		##PressKey ´Â ´©¸£°í ÀÖ´Â µ¿¾È °è¼Ó Àû¿ëµÇ´Â Å°ÀÌ´Ù.
		
		## ¼ıÀÚ ´ÜÃàÅ° Äü½½·Ô¿¡ ÀÌ¿ëµÈ´Ù.(ÀÌÈÄ ¼ıÀÚµéµµ Äü ½½·Ô¿ë ¿¹¾à)
		## F12 ´Â Å¬¶ó µğ¹ö±×¿ë Å°ÀÌ¹Ç·Î ¾²Áö ¾Ê´Â °Ô ÁÁ´Ù.
		onPressKeyDict[app.DIK_1]	= lambda : self.__PressNumKey(1)
		onPressKeyDict[app.DIK_2]	= lambda : self.__PressNumKey(2)
		onPressKeyDict[app.DIK_3]	= lambda : self.__PressNumKey(3)
		onPressKeyDict[app.DIK_4]	= lambda : self.__PressNumKey(4)
		onPressKeyDict[app.DIK_5]	= lambda : self.__PressNumKey(5)
		onPressKeyDict[app.DIK_6]	= lambda : self.__PressNumKey(6)
		onPressKeyDict[app.DIK_7]	= lambda : self.__PressNumKey(7)
		onPressKeyDict[app.DIK_8]	= lambda : self.__PressNumKey(8)
		onPressKeyDict[app.DIK_9]	= lambda : self.__PressNumKey(9)
		onPressKeyDict[app.DIK_F1]	= lambda : self.__PressQuickSlot(4)
		onPressKeyDict[app.DIK_F2]	= lambda : self.__PressQuickSlot(5)
		onPressKeyDict[app.DIK_F3]	= lambda : self.__PressQuickSlot(6)
		onPressKeyDict[app.DIK_F4]	= lambda : self.__PressQuickSlot(7)
		onPressKeyDict[app.DIK_H]	= lambda : self.Kamera_func_on()
		onPressKeyDict[app.DIK_Y]	= lambda : self.Kamera_func_off()
		onPressKeyDict[app.DIK_F11]	= lambda : self.SkyboxSelectMode()
		onPressKeyDict[app.DIK_F12]	= lambda : self.whisperAdmin.OpenWindow()
		onPressKeyDict[app.DIK_F5]	= lambda : self.__PressYKey()
		onPressKeyDict[app.DIK_F6]	= lambda : self.interface.NPCAC()
		onPressKeyDict[app.DIK_TAB]	= lambda : self.OpenDungeonWindow()	
		onPressKeyDict[app.DIK_F8]	= lambda : self.interface.OpenBiyologTable()
		onPressKeyDict[app.DIK_F7]	= lambda : self.__ItemSatVEYASil()
		onPressKeyDict[app.DIK_F8]		= lambda : self.interface.OpenShoutWindow()
#		if app.ENABLE_TITLE_SYSTEM:
#			onPressKeyDict[app.DIK_F9]		= lambda : self.OpenTitleSystem()
		onPressKeyDict[app.DIK_F9]	= lambda : self.interface.ToggleSwitchbotWindow()
#		onPressKeyDict[app.DIK_TAB]	= lambda : self.SpeedButtonWindowOpen()
		onPressKeyDict[app.DIK_LALT]		= lambda : self.ShowName()
		onPressKeyDict[app.DIK_LCONTROL]	= lambda : self.ShowMouseImage()
		onPressKeyDict[app.DIK_SYSRQ]		= lambda : self.SaveScreen()
		onPressKeyDict[app.DIK_SPACE]		= lambda : self.StartAttack()

		#Ä³¸¯ÅÍ ÀÌµ¿Å°
		onPressKeyDict[app.DIK_UP]			= lambda : self.MoveUp()
		onPressKeyDict[app.DIK_DOWN]		= lambda : self.MoveDown()
		onPressKeyDict[app.DIK_LEFT]		= lambda : self.MoveLeft()
		onPressKeyDict[app.DIK_RIGHT]		= lambda : self.MoveRight()
		onPressKeyDict[app.DIK_W]			= lambda : self.MoveUp()
		onPressKeyDict[app.DIK_S]			= lambda : self.MoveDown()
		onPressKeyDict[app.DIK_A]			= lambda : self.MoveLeft()
		onPressKeyDict[app.DIK_D]			= lambda : self.MoveRight()

		onPressKeyDict[app.DIK_E]			= lambda: app.RotateCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[app.DIK_R]			= lambda: app.ZoomCamera(app.CAMERA_TO_NEGATIVE)
		#onPressKeyDict[app.DIK_F]			= lambda: app.ZoomCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[app.DIK_T]			= lambda: app.PitchCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[app.DIK_G]			= self.__PressGKey
		onPressKeyDict[app.DIK_Q]			= self.__PressQKey

		onPressKeyDict[app.DIK_NUMPAD9]		= lambda: app.MovieResetCamera()
		onPressKeyDict[app.DIK_NUMPAD4]		= lambda: app.MovieRotateCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[app.DIK_NUMPAD6]		= lambda: app.MovieRotateCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[app.DIK_PGUP]		= lambda: app.MovieZoomCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[app.DIK_PGDN]		= lambda: app.MovieZoomCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[app.DIK_NUMPAD8]		= lambda: app.MoviePitchCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[app.DIK_NUMPAD2]		= lambda: app.MoviePitchCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[app.DIK_GRAVE]		= lambda : self.PickUpItem()
		onPressKeyDict[app.DIK_Z]			= lambda : self.PickUpItem()
		onPressKeyDict[app.DIK_C]			= lambda state = "STATUS": self.interface.ToggleCharacterWindow(state)
		onPressKeyDict[app.DIK_V]			= lambda state = "SKILL": self.interface.ToggleCharacterWindow(state)
		#onPressKeyDict[app.DIK_B]			= lambda state = "EMOTICON": self.interface.ToggleCharacterWindow(state)
		onPressKeyDict[app.DIK_N]			= lambda state = "QUEST": self.interface.ToggleCharacterWindow(state)
		onPressKeyDict[app.DIK_I]			= lambda : self.interface.ToggleInventoryWindow()
		onPressKeyDict[app.DIK_O]			= lambda : self.interface.ToggleDragonSoulWindowWithNoInfo()
		onPressKeyDict[app.DIK_M]			= lambda : self.interface.PressMKey()
		#onPressKeyDict[app.DIK_H]			= lambda : self.interface.OpenHelpWindow()
		onPressKeyDict[app.DIK_ADD]			= lambda : self.interface.MiniMapScaleUp()
		onPressKeyDict[app.DIK_SUBTRACT]	= lambda : self.interface.MiniMapScaleDown()
		onPressKeyDict[app.DIK_L]			= lambda : self.interface.ToggleChatLogWindow()
		onPressKeyDict[app.DIK_COMMA]		= lambda : self.ShowConsole()		# "`" key
		onPressKeyDict[app.DIK_LSHIFT]		= lambda : self.__SetQuickPageMode()
		# if app.OTOMATIK_AV:
			# onPressKeyDict[app.DIK_U]		= lambda : self.interface.ToggleAutoWindow()

		onPressKeyDict[app.DIK_J]			= lambda : self.__PressJKey()
		# onPressKeyDict[app.DIK_H]			= lambda : self.__PressHKey()
		onPressKeyDict[app.DIK_B]			= lambda : self.__PressBKey()
		onPressKeyDict[app.DIK_F]			= lambda : self.__PressFKey()
		# if app.__ENABLE_NEW_OFFLINESHOP__:
			# onPressKeyDict[app.DIK_Y]		= lambda : self.__PressYKey()


		onPressKeyDict[app.DIK_K]			= lambda : self.OpenSpecialStorage()

		# CUBE_TEST
		#onPressKeyDict[app.DIK_K]			= lambda : self.interface.OpenCubeWindow()
		# CUBE_TEST_END

		self.onPressKeyDict = onPressKeyDict

		onClickKeyDict = {}
		onClickKeyDict[app.DIK_UP] = lambda : self.StopUp()
		onClickKeyDict[app.DIK_DOWN] = lambda : self.StopDown()
		onClickKeyDict[app.DIK_LEFT] = lambda : self.StopLeft()
		onClickKeyDict[app.DIK_RIGHT] = lambda : self.StopRight()
		onClickKeyDict[app.DIK_SPACE] = lambda : self.EndAttack()

		onClickKeyDict[app.DIK_W] = lambda : self.StopUp()
		onClickKeyDict[app.DIK_S] = lambda : self.StopDown()
		onClickKeyDict[app.DIK_A] = lambda : self.StopLeft()
		onClickKeyDict[app.DIK_D] = lambda : self.StopRight()
		onClickKeyDict[app.DIK_Q] = lambda: app.RotateCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_E] = lambda: app.RotateCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_R] = lambda: app.ZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_F] = lambda: app.ZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_T] = lambda: app.PitchCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_G] = lambda: self.__ReleaseGKey()
		onClickKeyDict[app.DIK_NUMPAD4] = lambda: app.MovieRotateCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_NUMPAD6] = lambda: app.MovieRotateCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_PGUP] = lambda: app.MovieZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_PGDN] = lambda: app.MovieZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_NUMPAD8] = lambda: app.MoviePitchCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_NUMPAD2] = lambda: app.MoviePitchCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_LALT] = lambda: self.HideName()
		onClickKeyDict[app.DIK_LCONTROL] = lambda: self.HideMouseImage()
		onClickKeyDict[app.DIK_LSHIFT] = lambda: self.__SetQuickSlotMode()
		if app.ENABLE_NEW_PET_SYSTEM:
			onClickKeyDict[app.DIK_P] = lambda: self.OpenPetMainGui()

		#if constInfo.PVPMODE_ACCELKEY_ENABLE:
		#	onClickKeyDict[app.DIK_B] = lambda: self.ChangePKMode()

		self.onClickKeyDict=onClickKeyDict

	def __ItemSatVEYASil(self):
		self.interface.OpenKygnItemRemoveWindow()

	def OpenBiyolog(self):
		if self.interface:
			self.interface.OpenBiyologTable()
			
	if app.ENABLE_TITLE_SYSTEM:
		def OpenTitleSystem(self):
			self.wndTitleSystem.OpenWindow()
			
	if app.__ENABLE_NEW_OFFLINESHOP__:
		def __PressYKey(self):
			if self.Offlineshop:
				if not self.Offlineshop.IsShow():
					self.Offlineshop.Open()
				else:
					self.Offlineshop.Close()




	def __PressNumKey(self,num):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			
			if num >= 1 and num <= 9:
				if(chrmgr.IsPossibleEmoticon(-1)):				
					chrmgr.SetEmoticon(-1,int(num)-1)
					net.SendEmoticon(int(num)-1)
		else:
			if num >= 1 and num <= 4:
				self.pressNumber(num-1)

	def __ClickBKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			return
		else:
			if constInfo.PVPMODE_ACCELKEY_ENABLE:
				self.ChangePKMode()


	def	__PressJKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			if player.IsMountingHorse():
				net.SendChatPacket("/unmount")
			else:
				#net.SendChatPacket("/user_horse_ride")
				if not uiPrivateShopBuilder.IsBuildingPrivateShop() or not uiOfflineShopBuilder.IsBuildingOfflineShop():
					for i in xrange(player.INVENTORY_PAGE_SIZE):
						if player.GetItemIndex(i) in (71114, 71116, 71118, 71120):
							net.SendItemUsePacket(i)
							break
	def	__PressHKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			net.SendChatPacket("/user_horse_ride")
		else:
			self.interface.OpenHelpWindow()

	def	__PressBKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			net.SendChatPacket("/user_horse_back")
		else:
			state = "EMOTICON"
			self.interface.ToggleCharacterWindow(state)

	def	__PressFKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			net.SendChatPacket("/user_horse_feed")	
		else:
			app.ZoomCamera(app.CAMERA_TO_POSITIVE)

	def __PressGKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			net.SendChatPacket("/ride")	
		else:
			if self.ShowNameFlag:
				self.interface.ToggleGuildWindow()
			else:
				app.PitchCamera(app.CAMERA_TO_POSITIVE)

	def	__ReleaseGKey(self):
		app.PitchCamera(app.CAMERA_STOP)

	def __PressQKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			if 0==interfaceModule.IsQBHide:
				interfaceModule.IsQBHide = 1
				self.interface.HideAllQuestButton()
			else:
				interfaceModule.IsQBHide = 0
				self.interface.ShowAllQuestButton()
		else:
			app.RotateCamera(app.CAMERA_TO_NEGATIVE)

	def __SetQuickSlotMode(self):
		self.pressNumber=ui.__mem_func__(self.__PressQuickSlot)

	def __SetQuickPageMode(self):
		self.pressNumber=ui.__mem_func__(self.__SelectQuickPage)

	def __PressQuickSlot(self, localSlotIndex):
		if localeInfo.IsARABIC():
			if 0 <= localSlotIndex and localSlotIndex < 4:
				player.RequestUseLocalQuickSlot(3-localSlotIndex)
			else:
				player.RequestUseLocalQuickSlot(11-localSlotIndex)
		else:
			player.RequestUseLocalQuickSlot(localSlotIndex)			


	def Kamera_func_on(self):
		x, y, z = player.GetMainCharacterPosition()
		app.SetCameraSetting(int(x), int(-y), int(z), 5000, 10, 30)

	def Kamera_func_off(self):
		app.SetDefaultCamera()

	def __SelectQuickPage(self, pageIndex):
		self.quickSlotPageIndex = pageIndex
		player.SetQuickPage(pageIndex)
	if app.OTOMATIK_AV:
		def SetAutoCooltime(self, slotindex, cooltime):
			self.interface.SetAutoCooltime(slotindex, cooltime)
		def OtomatikAvSlotYenile(self):
			import uiOtomatikAv
			otoAv = uiOtomatikAv.AutoWindow()
			otoAv.IksirSlotlariYenile()
			otoAv.BeceriSlotlariYenile()
		def SetCloseGame(self):
			self.interface.SetCloseGame()
	def ToggleDebugInfo(self):
		self.isShowDebugInfo = not self.isShowDebugInfo

		if self.isShowDebugInfo:
			self.PrintCoord.Show()
			self.FrameRate.Show()
			self.Pitch.Show()
			self.Splat.Show()
			self.TextureNum.Show()
			self.ObjectNum.Show()
			self.ViewDistance.Show()
			self.PrintMousePos.Show()
		else:
			self.PrintCoord.Hide()
			self.FrameRate.Hide()
			self.Pitch.Hide()
			self.Splat.Hide()
			self.TextureNum.Hide()
			self.ObjectNum.Hide()
			self.ViewDistance.Hide()
			self.PrintMousePos.Hide()

	def __BuildDebugInfo(self):
		## Character Position Coordinate
		self.PrintCoord = ui.TextLine()
		self.PrintCoord.SetFontName(localeInfo.UI_DEF_FONT)
		self.PrintCoord.SetPosition(wndMgr.GetScreenWidth() - 270, 0)
		
		## Frame Rate
		self.FrameRate = ui.TextLine()
		self.FrameRate.SetFontName(localeInfo.UI_DEF_FONT)
		self.FrameRate.SetPosition(wndMgr.GetScreenWidth() - 270, 20)

		## Camera Pitch
		self.Pitch = ui.TextLine()
		self.Pitch.SetFontName(localeInfo.UI_DEF_FONT)
		self.Pitch.SetPosition(wndMgr.GetScreenWidth() - 270, 40)

		## Splat
		self.Splat = ui.TextLine()
		self.Splat.SetFontName(localeInfo.UI_DEF_FONT)
		self.Splat.SetPosition(wndMgr.GetScreenWidth() - 270, 60)
		
		##
		self.PrintMousePos = ui.TextLine()
		self.PrintMousePos.SetFontName(localeInfo.UI_DEF_FONT)
		self.PrintMousePos.SetPosition(wndMgr.GetScreenWidth() - 270, 80)

		# TextureNum
		self.TextureNum = ui.TextLine()
		self.TextureNum.SetFontName(localeInfo.UI_DEF_FONT)
		self.TextureNum.SetPosition(wndMgr.GetScreenWidth() - 270, 100)

		# ¿ÀºêÁ§Æ® ±×¸®´Â °³¼ö
		self.ObjectNum = ui.TextLine()
		self.ObjectNum.SetFontName(localeInfo.UI_DEF_FONT)
		self.ObjectNum.SetPosition(wndMgr.GetScreenWidth() - 270, 120)

		# ½Ã¾ß°Å¸®
		self.ViewDistance = ui.TextLine()
		self.ViewDistance.SetFontName(localeInfo.UI_DEF_FONT)
		self.ViewDistance.SetPosition(0, 0)
		if app.ENABLE_PINGTIME:
			self.pingLine.SetWindowHorizontalAlignCenter()
			self.pingLine.SetHorizontalAlignCenter()
			self.pingLine.SetFeather()
			self.pingLine.SetOutline()
			self.pingLine.Show()
			

	def __NotifyError(self, msg):
		chat.AppendChat(chat.CHAT_TYPE_INFO, msg)

	def ChangePKMode(self):

		if not app.IsPressed(app.DIK_LCONTROL):
			return

		if player.GetStatus(player.LEVEL)<constInfo.PVPMODE_PROTECTED_LEVEL:
			self.__NotifyError(localeInfo.OPTION_PVPMODE_PROTECT % (constInfo.PVPMODE_PROTECTED_LEVEL))
			return

		curTime = app.GetTime()
		if curTime - self.lastPKModeSendedTime < constInfo.PVPMODE_ACCELKEY_DELAY:
			return

		self.lastPKModeSendedTime = curTime

		curPKMode = player.GetPKMode()
		nextPKMode = curPKMode + 1
		if nextPKMode == player.PK_MODE_PROTECT:
			if 0 == player.GetGuildID():
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_CANNOT_SET_GUILD_MODE)
				nextPKMode = 0
			else:
				nextPKMode = player.PK_MODE_GUILD

		elif nextPKMode == player.PK_MODE_MAX_NUM:
			nextPKMode = 0

		net.SendChatPacket("/PKMode " + str(nextPKMode))
		print "/PKMode " + str(nextPKMode)

	def OnChangePKMode(self):

		self.interface.OnChangePKMode()

		try:
			self.__NotifyError(localeInfo.OPTION_PVPMODE_MESSAGE_DICT[player.GetPKMode()])
		except KeyError:
			print "UNKNOWN PVPMode[%d]" % (player.GetPKMode())

		if constInfo.PVPMODE_TEST_ENABLE:
			curPKMode = player.GetPKMode()
			alignment, grade = chr.testGetPKData()
			self.pkModeNameDict = { 0 : "PEACE", 1 : "REVENGE", 2 : "FREE", 3 : "PROTECT", }
			self.testPKMode.SetText("Current PK Mode : " + self.pkModeNameDict.get(curPKMode, "UNKNOWN"))
			self.testAlignment.SetText("Current Alignment : " + str(alignment) + " (" + localeInfo.TITLE_NAME_LIST[grade] + ")")

	###############################################################################################
	###############################################################################################
	## Game Callback Functions

	# Start
	def StartGame(self):
		self.RefreshInventory()
		self.RefreshEquipment()
		self.RefreshCharacter()
		self.RefreshSkill()
		if app.ENABLE_ENVIRONMENT_EFFECT_OPTION:
			systemSetting.SetNightModeOption(systemSetting.GetNightModeOption())
			systemSetting.SetSnowModeOption(systemSetting.GetSnowModeOption())
			systemSetting.SetSnowTextureModeOption(systemSetting.GetSnowTextureModeOption())
			
	if app.ENABLE_ENVIRONMENT_EFFECT_OPTION:
		def BINARY_Recv_Night_Mode(self, mode):
			self.__DayMode_Update(mode)		

	# Refresh
	def CheckGameButton(self):
		if self.interface:
			self.interface.CheckGameButton()

	def RefreshAlignment(self):
		self.interface.RefreshAlignment()

	def RefreshStatus(self):
		self.CheckGameButton()

		if self.interface:
			self.interface.RefreshStatus()

		if self.playerGauge:
			self.playerGauge.RefreshGauge()

	def RefreshStamina(self):
		self.interface.RefreshStamina()

	def RefreshSkill(self):
		self.CheckGameButton()
		if self.interface:
			self.interface.RefreshSkill()

	def RefreshQuest(self):
		self.interface.RefreshQuest()

	def RefreshMessenger(self):
		self.interface.RefreshMessenger()

	def RefreshGuildInfoPage(self):
		self.interface.RefreshGuildInfoPage()

	def RefreshGuildBoardPage(self):
		self.interface.RefreshGuildBoardPage()

	def RefreshGuildMemberPage(self):
		self.interface.RefreshGuildMemberPage()

	def RefreshGuildMemberPageGradeComboBox(self):
		self.interface.RefreshGuildMemberPageGradeComboBox()

	def RefreshGuildSkillPage(self):
		self.interface.RefreshGuildSkillPage()

	def RefreshGuildGradePage(self):
		self.interface.RefreshGuildGradePage()

	def RefreshMobile(self):
		if self.interface:
			self.interface.RefreshMobile()

	def OnMobileAuthority(self):
		self.interface.OnMobileAuthority()

	def OnBlockMode(self, mode):
		self.interface.OnBlockMode(mode)

	def OpenQuestWindow(self, skin, idx):
		if constInfo.AkiraMenu[0]:
			return
		if constInfo.CApiSetHide == 1:
			net.SendQuestInputStringPacket(str(constInfo.SendString))
			constInfo.CApiSetHide = 0
			return

		if constInfo.INPUT_IGNORE == 1:
			return
	
		self.interface.OpenQuestWindow(skin, idx)

	def HideAllQuestWindow(self):
		self.interface.HideAllQuestWindow()

	def AskGuildName(self):

		guildNameBoard = uiCommon.InputDialog()
		guildNameBoard.SetTitle(localeInfo.GUILD_NAME)
		guildNameBoard.SetAcceptEvent(ui.__mem_func__(self.ConfirmGuildName))
		guildNameBoard.SetCancelEvent(ui.__mem_func__(self.CancelGuildName))
		guildNameBoard.Open()

		self.guildNameBoard = guildNameBoard

	def ConfirmGuildName(self):
		guildName = self.guildNameBoard.GetText()
		if not guildName:
			return

		if net.IsInsultIn(guildName):
			self.PopupMessage(localeInfo.GUILD_CREATE_ERROR_INSULT_NAME)
			return

		net.SendAnswerMakeGuildPacket(guildName)
		self.guildNameBoard.Close()
		self.guildNameBoard = None
		return True

	def CancelGuildName(self):
		self.guildNameBoard.Close()
		self.guildNameBoard = None
		return True

	## Refine
	def PopupMessage(self, msg):
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(msg, 0, localeInfo.UI_OK)

	def OpenRefineDialog(self, targetItemPos, nextGradeItemVnum, cost, prob, type=0):
		self.interface.OpenRefineDialog(targetItemPos, nextGradeItemVnum, cost, prob, type)

	def AppendMaterialToRefineDialog(self, vnum, count):
		self.interface.AppendMaterialToRefineDialog(vnum, count)

	def RunUseSkillEvent(self, slotIndex, coolTime):
		self.interface.OnUseSkill(slotIndex, coolTime)

	def ClearAffects(self):
		self.affectShower.ClearAffects()

	def SetAffect(self, affect):
		self.affectShower.SetAffect(affect)

	def ResetAffect(self, affect):
		self.affectShower.ResetAffect(affect)

	def ajan_uyari(self, isim,olme,oldurme):
		ajanOlabilir = uiCommon.QuestionGuildWar()
		ajanOlabilir.SetText1(str(isim) + " isimli oyuncu ajan olabilir.")
		ajanOlabilir.SetText2(localeInfo.kill_info % (int(olme),int(oldurme)))
		ajanOlabilir.SetText3(localeInfo.asktotepik)
		ajanOlabilir.SetAcceptEvent(lambda arg=True: self.ajanCevap(arg, isim, ajanOlabilir))
		ajanOlabilir.SetCancelEvent(lambda arg=False: self.ajanCevap(arg, isim, ajanOlabilir))
		ajanOlabilir.Open()
		ajanOlabilir.isim = str(isim)

	def ajanCevap(self, answer, isim, func):
		if answer:
			net.SendChatPacket("/kov %s" % (isim))
		if (func):
			func.Close()
	# UNKNOWN_UPDATE
	def BINARY_NEW_AddAffect(self, type, pointIdx, value, duration):
		self.affectShower.BINARY_NEW_AddAffect(type, pointIdx, value, duration)
		if chr.NEW_AFFECT_DRAGON_SOUL_DECK1 == type or chr.NEW_AFFECT_DRAGON_SOUL_DECK2 == type:
			self.interface.DragonSoulActivate(type - chr.NEW_AFFECT_DRAGON_SOUL_DECK1)
		elif chr.NEW_AFFECT_DRAGON_SOUL_QUALIFIED == type:
			self.BINARY_DragonSoulGiveQuilification()
		elif app.ENABLE_DS_SET and chr.NEW_AFFECT_DS_SET == type:
			self.interface.DragonSoulSetGrade(value)

	def BINARY_NEW_RemoveAffect(self, type, pointIdx):
		self.affectShower.BINARY_NEW_RemoveAffect(type, pointIdx)
		if chr.NEW_AFFECT_DRAGON_SOUL_DECK1 == type or chr.NEW_AFFECT_DRAGON_SOUL_DECK2 == type:
			self.interface.DragonSoulDeactivate()
		elif app.ENABLE_DS_SET and chr.NEW_AFFECT_DS_SET == type:
			self.interface.DragonSoulSetGrade(0)
 
 
	# END_OF_UNKNOWN_UPDATE

	def ActivateSkillSlot(self, slotIndex):
		if self.interface:
			self.interface.OnActivateSkill(slotIndex)

	def DeactivateSkillSlot(self, slotIndex):
		if self.interface:
			self.interface.OnDeactivateSkill(slotIndex)

	def RefreshEquipment(self):
		if self.interface:
			self.interface.RefreshInventory()

	def RefreshInventory(self):
		if self.interface:
			self.interface.RefreshInventory()

			if app.OTOMATIK_AV:
				self.interface.RefreshAutoPositionSlot()

	def RefreshInventory(self):
		if self.interface:
			self.interface.RefreshInventory()
			if app.OTOMATIK_AV:
				self.interface.RefreshAutoPositionSlot()

	def RefreshCharacter(self):
		if self.interface:
			self.interface.RefreshCharacter()

	def OnGameOver(self):
		self.CloseTargetBoard()
		self.OpenRestartDialog()

	def OpenRestartDialog(self):
		self.interface.OpenRestartDialog()

	def ChangeCurrentSkill(self, skillSlotNumber):
		self.interface.OnChangeCurrentSkill(skillSlotNumber)

	## TargetBoard
	def SetPCTargetBoard(self, vid, name):
		self.targetBoard.Open(vid, name)
		
		if app.IsPressed(app.DIK_LCONTROL):
			
			if not player.IsSameEmpire(vid):
				return

			if player.IsMainCharacterIndex(vid):
				return		
			elif chr.INSTANCE_TYPE_BUILDING == chr.GetInstanceType(vid):
				return

			self.interface.OpenWhisperDialog(name)
			

	def RefreshTargetBoardByVID(self, vid):
		self.targetBoard.RefreshByVID(vid)

	def RefreshTargetBoardByName(self, name):
		self.targetBoard.RefreshByName(name)
		
	def __RefreshTargetBoard(self):
		self.targetBoard.Refresh()
		
	if app.ENABLE_VIEW_ELEMENT:
		def SetHPTargetBoard(self, vid, hpPercentage, iMinHP, iMaxHP, bElement):
			if vid != self.targetBoard.GetTargetVID():
				self.targetBoard.ResetTargetBoard()
				self.targetBoard.SetEnemyVID(vid)

			self.targetBoard.SetHP(hpPercentage, iMinHP, iMaxHP)
			self.targetBoard.SetElementImage(bElement)
			self.targetBoard.Show()
	else:
		def SetHPTargetBoard(self, vid, hpPercentage):
			if vid != self.targetBoard.GetTargetVID():
				self.targetBoard.ResetTargetBoard()
				self.targetBoard.SetEnemyVID(vid)

			self.targetBoard.SetHP(hpPercentage)
			self.targetBoard.Show()
			
	def CloseTargetBoardIfDifferent(self, vid):
		if vid != self.targetBoard.GetTargetVID():
			self.targetBoard.Close()

	def CloseTargetBoard(self):
		self.targetBoard.Close()
        
	def OpenBossTracking(self):
		self.bosstracking.Open()
		
	def CloseBossTracking(self):
		self.bosstracking.__OnClose()
		
	def BINARY_BEVIS_BOSS_TRACKING(self, kill_time, start_time, channel, mob_vnum, map_index):
		self.bosstracking.SetData(kill_time, start_time, channel, mob_vnum, map_index)

	## View Equipment
	def OpenEquipmentDialog(self, vid):
		self.interface.OpenEquipmentDialog(vid)

	def SetEquipmentDialogItem(self, vid, slotIndex, vnum, count):
		self.interface.SetEquipmentDialogItem(vid, slotIndex, vnum, count)

	def SetEquipmentDialogSocket(self, vid, slotIndex, socketIndex, value):
		self.interface.SetEquipmentDialogSocket(vid, slotIndex, socketIndex, value)

	def SetEquipmentDialogAttr(self, vid, slotIndex, attrIndex, type, value):
		self.interface.SetEquipmentDialogAttr(vid, slotIndex, attrIndex, type, value)

	# SHOW_LOCAL_MAP_NAME
	def ShowMapName(self, mapName, x, y):
		if self.interface:
			self.interface.SetMapName(mapName)
		try:
			mapNameT = localeInfo.MINIMAP_ZONE_NAME_DICT[mapName]
			self.mapNameTextLine.SetText(mapNameT)
			self.mapNameTextLine.SetPosition((wndMgr.GetScreenWidth() - 130) / 2, 152)
		except:
			self.mapNameTextLine.SetText(mapName)
	# END_OF_SHOW_LOCAL_MAP_NAME	

	def BINARY_OpenAtlasWindow(self):
		self.interface.BINARY_OpenAtlasWindow()

	## Chat
	def OnRecvWhisper(self, mode, name, line):
		if name.find("<svside>") != -1:
			global svsidedia
			if line[line.find(",")-4:line.find(",")].isdigit():
				svsidedia.nm_updateimgoffline(line[line.find(", ")-4:line.find(", ")])
			else:
				svsidedia.Board.Hide()
			return
		if mode == chat.WHISPER_TYPE_GM:
			self.interface.RegisterGameMasterName(name)
		chat.AppendWhisper(mode, name, line)
		self.interface.RecvWhisper(name)

	def OnRecvWhisperSystemMessage(self, mode, name, line):
		chat.AppendWhisper(chat.WHISPER_TYPE_SYSTEM, name, line)
		self.interface.RecvWhisper(name)

	def OnRecvWhisperError(self, mode, name, line):
		if localeInfo.WHISPER_ERROR.has_key(mode):
			chat.AppendWhisper(chat.WHISPER_TYPE_SYSTEM, name, localeInfo.WHISPER_ERROR[mode](name))
		else:
			chat.AppendWhisper(chat.WHISPER_TYPE_SYSTEM, name, "Whisper Unknown Error(mode=%d, name=%s)" % (mode, name))
		self.interface.RecvWhisper(name)

	def RecvWhisper(self, name):
		self.interface.RecvWhisper(name)

	def BINARY_OnRecvBulkWhisper(self, content):
		content = content.replace("$", " ")

		self.interface.RegisterGameMasterName("<Sistem>")
		chat.AppendWhisper(chat.WHISPER_TYPE_CHAT, "<Sistem>", content)
		
		self.interface.RecvWhisper("<Sistem>")

	def OnPickMoney(self, money):
		if app.ENABLE_CHATTING_WINDOW_RENEWAL:
			chat.AppendChat(chat.CHAT_TYPE_MONEY_INFO, localeInfo.GAME_PICK_MONEY % (money))
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GAME_PICK_MONEY % (money))

	def OnPickCheque(self, cheque):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHEQUE_SYSTEM_PICK_WON % (cheque))

	def OnShopError(self, type):
		try:
			self.PopupMessage(localeInfo.SHOP_ERROR_DICT[type])
		except KeyError:
			self.PopupMessage(localeInfo.SHOP_ERROR_UNKNOWN % (type))

	def OnSafeBoxError(self):
		self.PopupMessage(localeInfo.SAFEBOX_ERROR)

	def OnFishingSuccess(self, isFish, fishName):
		chat.AppendChatWithDelay(chat.CHAT_TYPE_INFO, localeInfo.FISHING_SUCCESS(isFish, fishName), 2000)

	# ADD_FISHING_MESSAGE
	def OnFishingNotifyUnknown(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.FISHING_UNKNOWN)

	def OnFishingWrongPlace(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.FISHING_WRONG_PLACE)
	# END_OF_ADD_FISHING_MESSAGE

	def OnFishingNotify(self, isFish, fishName):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.FISHING_NOTIFY(isFish, fishName))

	def OnFishingFailure(self):
		chat.AppendChatWithDelay(chat.CHAT_TYPE_INFO, localeInfo.FISHING_FAILURE, 2000)

	def OnCannotPickItem(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GAME_CANNOT_PICK_ITEM)


	# MINING
	def OnCannotMining(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GAME_CANNOT_MINING)
	# END_OF_MINING

	def OnCannotUseSkill(self, vid, type):
		if localeInfo.USE_SKILL_ERROR_TAIL_DICT.has_key(type):
			textTail.RegisterInfoTail(vid, localeInfo.USE_SKILL_ERROR_TAIL_DICT[type])

		if localeInfo.USE_SKILL_ERROR_CHAT_DICT.has_key(type):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_SKILL_ERROR_CHAT_DICT[type])

	def	OnCannotShotError(self, vid, type):
		textTail.RegisterInfoTail(vid, localeInfo.SHOT_ERROR_TAIL_DICT.get(type, localeInfo.SHOT_ERROR_UNKNOWN % (type)))

	## PointReset
	def StartPointReset(self):
		self.interface.OpenPointResetDialog()

	## Shop
	if app.ENABLE_REMOTE_SHOP:
		def StartShop(self, vid, uzakmarket):
			self.interface.OpenShopDialog(vid, uzakmarket)
	else:
		def StartShop(self, vid):
			self.interface.OpenShopDialog(vid)

	def EndShop(self):
		self.interface.CloseShopDialog()

	def RefreshShop(self):
		self.interface.RefreshShopDialog()

	def SetShopSellingPrice(self, Price):
		pass

	## OfflineShop
	def StartOfflineShop(self, vid):
		self.interface.OpenOfflineShopDialog(vid)
		
	def EndOfflineShop(self):
		self.interface.CloseOfflineShopDialog()
		
	def RefreshOfflineShop(self):
		self.interface.RefreshOfflineShopDialog()
	## Exchange
	def StartExchange(self):
		self.interface.StartExchange()

	def EndExchange(self):
		self.interface.EndExchange()

	def RefreshExchange(self):
		self.interface.RefreshExchange()

	## Party
	def RecvPartyInviteQuestion(self, leaderVID, leaderName):
		partyInviteQuestionDialog = uiCommon.QuestionDialog()
		partyInviteQuestionDialog.SetText(leaderName + localeInfo.PARTY_DO_YOU_JOIN)
		partyInviteQuestionDialog.SetAcceptEvent(lambda arg=True: self.AnswerPartyInvite(arg))
		partyInviteQuestionDialog.SetCancelEvent(lambda arg=False: self.AnswerPartyInvite(arg))
		partyInviteQuestionDialog.Open()
		partyInviteQuestionDialog.partyLeaderVID = leaderVID
		self.partyInviteQuestionDialog = partyInviteQuestionDialog

	def AnswerPartyInvite(self, answer):

		if not self.partyInviteQuestionDialog:
			return

		partyLeaderVID = self.partyInviteQuestionDialog.partyLeaderVID

		distance = player.GetCharacterDistance(partyLeaderVID)
		if distance < 0.0 or distance > 5000:
			answer = False

		net.SendPartyInviteAnswerPacket(partyLeaderVID, answer)

		self.partyInviteQuestionDialog.Close()
		self.partyInviteQuestionDialog = None

	def AddPartyMember(self, pid, name):
		self.interface.AddPartyMember(pid, name)

	def UpdatePartyMemberInfo(self, pid):
		self.interface.UpdatePartyMemberInfo(pid)

	def RemovePartyMember(self, pid):
		self.interface.RemovePartyMember(pid)
		self.__RefreshTargetBoard()

	def LinkPartyMember(self, pid, vid):
		self.interface.LinkPartyMember(pid, vid)

	def UnlinkPartyMember(self, pid):
		self.interface.UnlinkPartyMember(pid)

	def UnlinkAllPartyMember(self):
		self.interface.UnlinkAllPartyMember()

	def ExitParty(self):
		self.interface.ExitParty()
		self.RefreshTargetBoardByVID(self.targetBoard.GetTargetVID())

	def ChangePartyParameter(self, distributionMode):
		self.interface.ChangePartyParameter(distributionMode)

	## Messenger
	def OnMessengerAddFriendQuestion(self, name):
		messengerAddFriendQuestion = uiCommon.QuestionDialog2()
		messengerAddFriendQuestion.SetText1(localeInfo.MESSENGER_DO_YOU_ACCEPT_ADD_FRIEND_1 % (name))
		messengerAddFriendQuestion.SetText2(localeInfo.MESSENGER_DO_YOU_ACCEPT_ADD_FRIEND_2)
		messengerAddFriendQuestion.SetAcceptEvent(ui.__mem_func__(self.OnAcceptAddFriend))
		messengerAddFriendQuestion.SetCancelEvent(ui.__mem_func__(self.OnDenyAddFriend))
		messengerAddFriendQuestion.Open()
		messengerAddFriendQuestion.name = name
		self.messengerAddFriendQuestion = messengerAddFriendQuestion

	def OnAcceptAddFriend(self):
		name = self.messengerAddFriendQuestion.name
		net.SendChatPacket("/messenger_auth y " + name)
		self.OnCloseAddFriendQuestionDialog()
		return True

	def OnDenyAddFriend(self):
		name = self.messengerAddFriendQuestion.name
		net.SendChatPacket("/messenger_auth n " + name)
		self.OnCloseAddFriendQuestionDialog()
		return True

	def OnCloseAddFriendQuestionDialog(self):
		self.messengerAddFriendQuestion.Close()
		self.messengerAddFriendQuestion = None
		return True

	## SafeBox
	def OpenSafeboxWindow(self, size):
		self.interface.OpenSafeboxWindow(size)

	def RefreshSafebox(self):
		self.interface.RefreshSafebox()

	def RefreshSafeboxMoney(self):
		self.interface.RefreshSafeboxMoney()

	# ITEM_MALL
	def OpenMallWindow(self, size):
		self.interface.OpenMallWindow(size)

	def RefreshMall(self):
		self.interface.RefreshMall()
	# END_OF_ITEM_MALL

	## Guild
	def RecvGuildInviteQuestion(self, guildID, guildName):
		guildInviteQuestionDialog = uiCommon.QuestionDialog()
		guildInviteQuestionDialog.SetText(guildName + localeInfo.GUILD_DO_YOU_JOIN)
		guildInviteQuestionDialog.SetAcceptEvent(lambda arg=True: self.AnswerGuildInvite(arg))
		guildInviteQuestionDialog.SetCancelEvent(lambda arg=False: self.AnswerGuildInvite(arg))
		guildInviteQuestionDialog.Open()
		guildInviteQuestionDialog.guildID = guildID
		self.guildInviteQuestionDialog = guildInviteQuestionDialog

	def AnswerGuildInvite(self, answer):

		if not self.guildInviteQuestionDialog:
			return

		guildLeaderVID = self.guildInviteQuestionDialog.guildID
		net.SendGuildInviteAnswerPacket(guildLeaderVID, answer)

		self.guildInviteQuestionDialog.Close()
		self.guildInviteQuestionDialog = None

	
	def DeleteGuild(self):
		self.interface.DeleteGuild()

	## Clock
	def ShowClock(self, second):
		self.interface.ShowClock(second)

	def HideClock(self):
		self.interface.HideClock()

	## Emotion
	def BINARY_ActEmotion(self, emotionIndex):
		if self.interface.wndCharacter:
			self.interface.wndCharacter.ActEmotion(emotionIndex)

	###############################################################################################
	###############################################################################################
	## Keyboard Functions

	def CheckFocus(self):
		if False == self.IsFocus():
			if True == self.interface.IsOpenChat():
				self.interface.ToggleChat()

			self.SetFocus()

	def SpeedButtonWindowOpen(self):
		constInfo.SPEED_BUTTON = 1


	def SaveScreen(self):
		print "save screen"

		# SCREENSHOT_CWDSAVE
		if SCREENSHOT_CWDSAVE:
			if not os.path.exists(os.getcwd()+os.sep+"screenshot"):
				os.mkdir(os.getcwd()+os.sep+"screenshot")

			(succeeded, name) = grp.SaveScreenShotToPath(os.getcwd()+os.sep+"screenshot"+os.sep)
		elif SCREENSHOT_DIR:
			(succeeded, name) = grp.SaveScreenShot(SCREENSHOT_DIR)
		else:
			(succeeded, name) = grp.SaveScreenShot()
		# END_OF_SCREENSHOT_CWDSAVE

		if succeeded:
			pass
			"""
			chat.AppendChat(chat.CHAT_TYPE_INFO, name + localeInfo.SCREENSHOT_SAVE1)
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SCREENSHOT_SAVE2)
			"""
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SCREENSHOT_SAVE_FAILURE)

	def ShowConsole(self):
		if debugInfo.IsDebugMode() or True == self.consoleEnable:
			player.EndKeyWalkingImmediately()
			self.console.OpenWindow()

	def ShowName(self):
		self.ShowNameFlag = True
		if app.__ENABLE_NEW_OFFLINESHOP__:
			offlineshop.ShowShopNames()

		self.playerGauge.EnableShowAlways()
		player.SetQuickPage(self.quickSlotPageIndex+1)

	# ADD_ALWAYS_SHOW_NAME
	def __IsShowName(self):

		if systemSetting.IsAlwaysShowName():
			return True

		if self.ShowNameFlag:
			return True

		return False
	# END_OF_ADD_ALWAYS_SHOW_NAME
	
	def HideName(self):
		self.ShowNameFlag = False
		self.playerGauge.DisableShowAlways()
		player.SetQuickPage(self.quickSlotPageIndex)
		if app.__ENABLE_NEW_OFFLINESHOP__:
			offlineshop.HideShopNames()


	def ShowMouseImage(self):
		self.interface.ShowMouseImage()

	def HideMouseImage(self):
		self.interface.HideMouseImage()

	def StartAttack(self):
		player.SetAttackKeyState(True)

	def EndAttack(self):
		player.SetAttackKeyState(False)

	def MoveUp(self):
		player.SetSingleDIKKeyState(app.DIK_UP, True)

	def MoveDown(self):
		player.SetSingleDIKKeyState(app.DIK_DOWN, True)

	def MoveLeft(self):
		player.SetSingleDIKKeyState(app.DIK_LEFT, True)

	def MoveRight(self):
		player.SetSingleDIKKeyState(app.DIK_RIGHT, True)

	def StopUp(self):
		player.SetSingleDIKKeyState(app.DIK_UP, False)

	def StopDown(self):
		player.SetSingleDIKKeyState(app.DIK_DOWN, False)

	def StopLeft(self):
		player.SetSingleDIKKeyState(app.DIK_LEFT, False)

	def StopRight(self):
		player.SetSingleDIKKeyState(app.DIK_RIGHT, False)

	def PickUpItem(self):
		##player.PickCloseItem()
		player.PickCloseItemVector()

	###############################################################################################
	###############################################################################################
	## Event Handler

	def OnKeyDown(self, key):
		if self.interface.wndWeb and self.interface.wndWeb.IsShow():
			return

		if key == app.DIK_ESC:
			self.RequestDropItem(False)
			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

		try:
			self.onPressKeyDict[key]()
		except KeyError:
			pass
		except:
			raise

		return True

	def OnKeyUp(self, key):
		try:
			self.onClickKeyDict[key]()
		except KeyError:
			pass
		except:
			raise

		return True

	def OnMouseLeftButtonDown(self):
		if self.interface.BUILD_OnMouseLeftButtonDown():
			return

		if mouseModule.mouseController.isAttached():
			self.CheckFocus()
		else:
			hyperlink = ui.GetHyperlink()
			if hyperlink:
				return
			else:
				self.CheckFocus()
				player.SetMouseState(player.MBT_LEFT, player.MBS_PRESS);

		return True

	def OnMouseLeftButtonUp(self):

		if self.interface.BUILD_OnMouseLeftButtonUp():
			return

		if mouseModule.mouseController.isAttached():

			attachedType = mouseModule.mouseController.GetAttachedType()
			attachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()
			attachedItemSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			attachedItemCount = mouseModule.mouseController.GetAttachedItemCount()
			if app.__ENABLE_NEW_OFFLINESHOP__:
				if uinewofflineshop.IsBuildingShop() and uinewofflineshop.IsSaleSlot(player.SlotTypeToInvenType(attachedType), attachedItemSlotPos): #toupdate
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OFFLINESHOP_CANT_SELECT_ITEM_DURING_BUILING)
					return



			## QuickSlot
			if player.SLOT_TYPE_QUICK_SLOT == attachedType:
				player.RequestDeleteGlobalQuickSlot(attachedItemSlotPos)

			## Inventory
			elif player.SLOT_TYPE_INVENTORY == attachedType:

				if player.ITEM_MONEY == attachedItemIndex:
					self.__PutMoney(attachedType, attachedItemCount, self.PickingCharacterIndex)
				elif player.ITEM_CHEQUE == attachedItemIndex:
					self.__PutCheque(attachedType, attachedItemCount, self.PickingCharacterIndex)

				else:
					self.__PutItem(attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount, self.PickingCharacterIndex)

			## DragonSoul
			elif player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedType:
				self.__PutItem(attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount, self.PickingCharacterIndex)
			if app.ENABLE_SPECIAL_STORAGE:
				if player.SLOT_TYPE_UPGRADE_INVENTORY == attachedType or\
					player.SLOT_TYPE_STONE_INVENTORY == attachedType or\
					player.SLOT_TYPE_CHEST_INVENTORY == attachedType or \
					player.SLOT_TYPE_ATTR_INVENTORY == attachedType:
					self.__PutItem(attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount, self.PickingCharacterIndex)
			mouseModule.mouseController.DeattachObject()
		else:
			hyperlink = ui.GetHyperlink()
			if hyperlink:
				if app.IsPressed(app.DIK_LALT):
					link = chat.GetLinkFromHyperlink(hyperlink)
					ime.PasteString(link)
				else:
					self.interface.MakeHyperlinkTooltip(hyperlink)
				return
			else:
				player.SetMouseState(player.MBT_LEFT, player.MBS_CLICK)

		#player.EndMouseWalking()
		return True

	def __PutItem(self, attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount, dstChrID):
		if app.ENABLE_SPECIAL_STORAGE:
			if player.SLOT_TYPE_INVENTORY == attachedType or\
				player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedType or\
				player.SLOT_TYPE_UPGRADE_INVENTORY == attachedType or\
				player.SLOT_TYPE_STONE_INVENTORY == attachedType or\
				player.SLOT_TYPE_CHEST_INVENTORY == attachedType or\
				player.SLOT_TYPE_ATTR_INVENTORY == attachedType:
				attachedInvenType = player.SlotTypeToInvenType(attachedType)
				if True == chr.HasInstance(self.PickingCharacterIndex) and player.GetMainCharacterIndex() != dstChrID:
					if player.IsEquipmentSlot(attachedItemSlotPos) and\
						player.SLOT_TYPE_DRAGON_SOUL_INVENTORY != attachedType and\
						player.SLOT_TYPE_UPGRADE_INVENTORY != attachedType and\
						player.SLOT_TYPE_STONE_INVENTORY != attachedType and\
						player.SLOT_TYPE_CHEST_INVENTORY != attachedType and\
						player.SLOT_TYPE_ATTR_INVENTORY != attachedType:
						self.stream.popupWindow.Close()
						self.stream.popupWindow.Open(localeInfo.EXCHANGE_FAILURE_EQUIP_ITEM, 0, localeInfo.UI_OK)
					else:
						if chr.IsNPC(dstChrID):
							if app.ENABLE_REFINE_RENEWAL:
								constInfo.AUTO_REFINE_TYPE = 2
								constInfo.AUTO_REFINE_DATA["NPC"][0] = dstChrID
								constInfo.AUTO_REFINE_DATA["NPC"][1] = attachedInvenType
								constInfo.AUTO_REFINE_DATA["NPC"][2] = attachedItemSlotPos
								constInfo.AUTO_REFINE_DATA["NPC"][3] = attachedItemCount
							net.SendGiveItemPacket(dstChrID, attachedInvenType, attachedItemSlotPos, attachedItemCount)
						# elif chr.IsEnemy(dstChrID) or chr.IsStone(dstChrID):
							# net.SendGiveItemPacket(dstChrID, attachedInvenType, attachedItemSlotPos, attachedItemCount)
						else:
							net.SendExchangeStartPacket(dstChrID)
							net.SendExchangeItemAddPacket(attachedInvenType, attachedItemSlotPos, 0)
				else:
					self.__DropItem(attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount)
		else:
			if player.SLOT_TYPE_INVENTORY == attachedType or player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedType:
				attachedInvenType = player.SlotTypeToInvenType(attachedType)
				if True == chr.HasInstance(self.PickingCharacterIndex) and player.GetMainCharacterIndex() != dstChrID:
					if player.IsEquipmentSlot(attachedItemSlotPos) and player.SLOT_TYPE_DRAGON_SOUL_INVENTORY != attachedType:
						self.stream.popupWindow.Close()
						self.stream.popupWindow.Open(localeInfo.EXCHANGE_FAILURE_EQUIP_ITEM, 0, localeInfo.UI_OK)
					else:
						if chr.IsNPC(dstChrID):
							# if app.ENABLE_REFINE_RENEWAL:
								# constInfo.AUTO_REFINE_TYPE = 2
								# constInfo.AUTO_REFINE_DATA["NPC"][0] = dstChrID
								# constInfo.AUTO_REFINE_DATA["NPC"][1] = attachedInvenType
								# constInfo.AUTO_REFINE_DATA["NPC"][2] = attachedItemSlotPos
								# constInfo.AUTO_REFINE_DATA["NPC"][3] = attachedItemCount
							net.SendGiveItemPacket(dstChrID, attachedInvenType, attachedItemSlotPos, attachedItemCount)
						else:
							net.SendExchangeStartPacket(dstChrID)
							net.SendExchangeItemAddPacket(attachedInvenType, attachedItemSlotPos, 0)
				else:
					self.__DropItem(attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount)

	def __PutMoney(self, attachedType, attachedMoney, dstChrID):
		if True == chr.HasInstance(dstChrID) and player.GetMainCharacterIndex() != dstChrID:
			net.SendExchangeStartPacket(dstChrID)
			net.SendExchangeElkAddPacket(attachedMoney)
		else:
			self.__DropMoney(attachedType, attachedMoney)

	def __DropMoney(self, attachedType, attachedMoney):
		# PRIVATESHOP_DISABLE_ITEM_DROP - °³ÀÎ»óÁ¡ ¿­°í ÀÖ´Â µ¿¾È ¾ÆÀÌÅÛ ¹ö¸² ¹æÁö
		if uiPrivateShopBuilder.IsBuildingPrivateShop():			
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
			return
		# END_OF_PRIVATESHOP_DISABLE_ITEM_DROP
		# OFFLINESHOP_DISABLE_ITEM_DROP
		if (uiOfflineShopBuilder.IsBuildingOfflineShop()):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_OFFLINE_SHOP)
			return
		# END_OF_OFFLINESHOP_DISABLE_ITEM_DROP
		

		# END_OF_OFFLINESHOP_DISABLE_ITEM_DROP2
		
		if attachedMoney>=1000:
			self.stream.popupWindow.Close()
			self.stream.popupWindow.Open(localeInfo.DROP_MONEY_FAILURE_1000_OVER, 0, localeInfo.UI_OK)
			return

		itemDropQuestionDialog = uiCommon.QuestionDialog()
		itemDropQuestionDialog.SetText(localeInfo.DO_YOU_DROP_MONEY % (attachedMoney))
		itemDropQuestionDialog.SetAcceptEvent(lambda arg=True: self.RequestDropItem(arg))
		itemDropQuestionDialog.SetCancelEvent(lambda arg=False: self.RequestDropItem(arg))
		itemDropQuestionDialog.Open()
		itemDropQuestionDialog.dropType = attachedType
		itemDropQuestionDialog.dropCount = attachedMoney
		itemDropQuestionDialog.dropNumber = player.ITEM_MONEY
		self.itemDropQuestionDialog = itemDropQuestionDialog

	def __PutCheque(self, attachedType, attachedCheque, dstChrID):
		if True == chr.HasInstance(dstChrID) and player.GetMainCharacterIndex() != dstChrID:
			net.SendExchangeStartPacket(dstChrID)
			net.SendExchangePsgAddPacket(attachedCheque)
		else:
			self.__DropCheque(attachedType, attachedCheque)		
		
	def __DropCheque(self, attachedType, attachedCheque):
		if attachedCheque>=100000:
			self.stream.popupWindow.Close()
			self.stream.popupWindow.Open("S??poss?el descartar at?99 Won por vez.", 0, localeInfo.UI_OK)
			return
		itemDropQuestionDialog = uiCommon.QuestionDialog()
		itemDropQuestionDialog.SetText(localeInfo.DO_YOU_DROP_CHEQUE % (attachedCheque))
		itemDropQuestionDialog.SetAcceptEvent(lambda arg=True: self.RequestDropItem(arg))
		itemDropQuestionDialog.SetCancelEvent(lambda arg=False: self.RequestDropItem(arg))
		itemDropQuestionDialog.Open()
		itemDropQuestionDialog.dropType = attachedType
		itemDropQuestionDialog.dropCount = attachedCheque
		itemDropQuestionDialog.dropNumber = player.ITEM_CHEQUE
		self.itemDropQuestionDialog = itemDropQuestionDialog		
		
	def __DropItem(self, attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount):
		# PRIVATESHOP_DISABLE_ITEM_DROP - °³ÀÎ»óÁ¡ ¿­°í ÀÖ´Â µ¿¾È ¾ÆÀÌÅÛ ¹ö¸² ¹æÁö
		if uiPrivateShopBuilder.IsBuildingPrivateShop():			
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
			return
		# END_OF_PRIVATESHOP_DISABLE_ITEM_DROP
		# OFFLINESHOP_DISABLE_ITEM_DROP
		if (uiOfflineShopBuilder.IsBuildingOfflineShop()):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_OFFLINE_SHOP)
			return
		# END_OFF_OFFLINESHOP_DISABLE_ITEM_DROP
		
		# OFFLINESHOP_DISABLE_ITEM_DROP2

		# END_OF_OFFLINESHOP_DISABLE_ITEM_DROP2
		
		if player.SLOT_TYPE_INVENTORY == attachedType and player.IsEquipmentSlot(attachedItemSlotPos):
			self.stream.popupWindow.Close()
			self.stream.popupWindow.Open(localeInfo.DROP_ITEM_FAILURE_EQUIP_ITEM, 0, localeInfo.UI_OK)

		else:
			if player.SLOT_TYPE_INVENTORY == attachedType:
				dropItemIndex = player.GetItemIndex(attachedItemSlotPos)

				item.SelectItem(dropItemIndex)
				dropItemName = item.GetItemName()
				evolution = player.GetItemEvolution(attachedItemSlotPos)
				
				dropItemName = localeInfo.SILAH_EVRIM_TEXT[evolution] + item.GetItemName()
				
				## Question Text
				questionText = localeInfo.HOW_MANY_ITEM_DO_YOU_DROP(dropItemName, attachedItemCount)

				## Dialog
				# itemDropQuestionDialog = uiCommon.QuestionDialog()
				# if app.HERAKLES:
				questionText = localeInfo.HOW_MANY_ITEM_DO_YOU_DROP_YENI(dropItemName, attachedItemCount)
				itemDropQuestionDialog = uiCommon.QuestionDialog()
				# else:
					# questionText = localeInfo.HOW_MANY_ITEM_DO_YOU_DROP(dropItemName, attachedItemCount)
					# itemDropQuestionDialog = uiCommon.QuestionDialogItem()

				itemDropQuestionDialog.SetText(questionText)

				# if app.HERAKLES:
				itemDropQuestionDialog.SetAcceptEvent(lambda arg=True: self.RequestDestroyItem(arg))
				itemDropQuestionDialog.SetAcceptText(localeInfo.silbutton)
				# else:
					# itemDropQuestionDialog.SetAcceptEvent(lambda arg=True: self.RequestDropItem(arg))

				itemDropQuestionDialog.SetCancelEvent(lambda arg=False: self.RequestDropItem(arg))
				# if not app.HERAKLES:
					# itemDropQuestionDialog.SetDestroyEvent(lambda arg=True: self.RequestDestroyItem(arg))
				# else:
				itemDropQuestionDialog.SetCancelText(localeInfo.vazgecbutton)
				itemDropQuestionDialog.Open()
				itemDropQuestionDialog.dropType = attachedType
				itemDropQuestionDialog.dropNumber = attachedItemSlotPos
				itemDropQuestionDialog.dropCount = attachedItemCount
				self.itemDropQuestionDialog = itemDropQuestionDialog

				constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)
			elif player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedType:
				dropItemIndex = player.GetItemIndex(player.DRAGON_SOUL_INVENTORY, attachedItemSlotPos)

				item.SelectItem(dropItemIndex)
				dropItemName = item.GetItemName()

				## Question Text
				questionText = localeInfo.HOW_MANY_ITEM_DO_YOU_DROP(dropItemName, attachedItemCount)

				## Dialog
				itemDropQuestionDialog = uiCommon.QuestionDialog()
				itemDropQuestionDialog.SetText(questionText)
				itemDropQuestionDialog.SetAcceptEvent(lambda arg=True: self.RequestDropItem(arg))
				itemDropQuestionDialog.SetCancelEvent(lambda arg=False: self.RequestDropItem(arg))
				itemDropQuestionDialog.Open()
				itemDropQuestionDialog.dropType = attachedType
				itemDropQuestionDialog.dropNumber = attachedItemSlotPos
				itemDropQuestionDialog.dropCount = attachedItemCount
				self.itemDropQuestionDialog = itemDropQuestionDialog

				constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)

			if app.ENABLE_SPECIAL_STORAGE:
				if player.SLOT_TYPE_UPGRADE_INVENTORY == attachedType or\
					player.SLOT_TYPE_STONE_INVENTORY == attachedType or\
					player.SLOT_TYPE_CHEST_INVENTORY == attachedType or\
					player.SLOT_TYPE_ATTR_INVENTORY == attachedType:
					dropItemIndex = player.GetItemIndex(player.SlotTypeToInvenType(attachedType), attachedItemSlotPos)
	
					item.SelectItem(dropItemIndex)
					dropItemName = item.GetItemName()
	
					## Question Text
					questionText = localeInfo.HOW_MANY_ITEM_DO_YOU_DROP(dropItemName, attachedItemCount)
	
					## Dialog
					# if app.HERAKLES:
					questionText = localeInfo.HOW_MANY_ITEM_DO_YOU_DROP_YENI(dropItemName, attachedItemCount)
					itemDropQuestionDialog = uiCommon.QuestionDialog()
					# else:
						# questionText = localeInfo.HOW_MANY_ITEM_DO_YOU_DROP(dropItemName, attachedItemCount)
						# itemDropQuestionDialog = uiCommon.QuestionDialogItem()

					# itemDropQuestionDialog = uiCommon.QuestionDialog()
					itemDropQuestionDialog.SetText(questionText)
					# if app.HERAKLES:
					itemDropQuestionDialog.SetAcceptEvent(lambda arg=True: self.RequestDestroyItem(arg))
					itemDropQuestionDialog.SetAcceptText(localeInfo.silbutton)
					# else:
						# itemDropQuestionDialog.SetAcceptEvent(lambda arg=True: self.RequestDropItem(arg))
						
					# if not app.HERAKLES:
						# itemDropQuestionDialog.SetDestroyEvent(lambda arg=True: self.RequestDestroyItem(arg))
					# else:
					itemDropQuestionDialog.SetCancelText(localeInfo.vazgecbutton)

					itemDropQuestionDialog.SetCancelEvent(lambda arg=False: self.RequestDropItem(arg))
					itemDropQuestionDialog.Open()
					itemDropQuestionDialog.dropType = attachedType
					itemDropQuestionDialog.dropNumber = attachedItemSlotPos
					itemDropQuestionDialog.dropCount = attachedItemCount
					self.itemDropQuestionDialog = itemDropQuestionDialog
	
					constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)
				
	def RequestDestroyItem(self, answer):
		if not self.itemDropQuestionDialog:
			return
		if answer:
			dropType = self.itemDropQuestionDialog.dropType
			dropNumber = self.itemDropQuestionDialog.dropNumber
			dropCount = self.itemDropQuestionDialog.dropCount
			# if app.HERAKLES:
			itemDropQuestionDialog2 = uiCommon.QuestionDialog()
			itemDropQuestionDialog2.SetText(localeInfo.silcenmi)
			itemDropQuestionDialog2.SetAcceptEvent(lambda arg=True: self.RequestDestroyItem2(arg))
			itemDropQuestionDialog2.SetCancelEvent(lambda arg=False: self.RequestDropItem2(arg))
			itemDropQuestionDialog2.Open()
			itemDropQuestionDialog2.dropNumber2 = dropNumber
			itemDropQuestionDialog2.dropType2 = dropType
			itemDropQuestionDialog2.dropCount2 = dropCount
			self.itemDropQuestionDialog2 = itemDropQuestionDialog2

			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)
			# else:
				# constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)
				# self.__SendDestroyItemPacket(dropNumber, dropType)
						
		else:
			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)
	
		self.itemDropQuestionDialog.Close()
		self.itemDropQuestionDialog = None

		
		
		
	def RequestDestroyItem2(self, answer):
		if not self.itemDropQuestionDialog2:
			return
		if answer:
			dropNumber = self.itemDropQuestionDialog2.dropNumber2
			dropType = self.itemDropQuestionDialog2.dropType2
			self.__SendDestroyItemPacket(dropNumber, dropType)
	
		self.itemDropQuestionDialog2.Close()
		self.itemDropQuestionDialog2 = None
		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)
		
		
		
	def __SendDestroyItemPacket(self, itemVNum, itemInvenType = player.INVENTORY):
		if uiPrivateShopBuilder.IsBuildingPrivateShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
			return
			
		net.SendItemDestroyPacket(itemVNum,player.SlotTypeToInvenType(itemInvenType))
		
	def RequestDropItem2(self, answer):
		if not self.itemDropQuestionDialog2:
			return

		if answer:
			dropType = self.itemDropQuestionDialog2.dropType2
			dropCount = self.itemDropQuestionDialog2.dropCount2
			dropNumber = self.itemDropQuestionDialog2.dropNumber2

			if playerSLOT_TYPE_INVENTORY == dropType:
				if dropNumber == playerITEM_MONEY:
					net.SendGoldDropPacketNew(dropCount)
					snd.PlaySound("sound/ui/money.wav")
				elif dropNumber == playerITEM_CHEQUE:
					net.SendChequeDropPacketNew(dropCount)
					snd.PlaySound("sound/ui/money.wav")
				else:
					# PRIVATESHOP_DISABLE_ITEM_DROP
					self.__SendDropItemPacket(dropNumber, dropCount)
					# END_OF_PRIVATESHOP_DISABLE_ITEM_DROP
			elif playerSLOT_TYPE_DRAGON_SOUL_INVENTORY == dropType:
					# PRIVATESHOP_DISABLE_ITEM_DROP
					self.__SendDropItemPacket(dropNumber, dropCount, playerDRAGON_SOUL_INVENTORY)
					# END_OF_PRIVATESHOP_DISABLE_ITEM_DROP
			if app.ENABLE_SPECIAL_STORAGE:
				if player.SLOT_TYPE_UPGRADE_INVENTORY == dropType or\
					player.SLOT_TYPE_STONE_INVENTORY == dropType or\
					player.SLOT_TYPE_CHEST_INVENTORY == dropType or\
					player.SLOT_TYPE_ATTR_INVENTORY == dropType:
					self.__SendDropItemPacket(dropNumber, dropCount, player.SlotTypeToInvenType(dropType))
		self.itemDropQuestionDialog2.Close()
		self.itemDropQuestionDialog2 = None

		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)
	
	def RequestDropItem(self, answer):
		if not self.itemDropQuestionDialog:
			return

		if answer:
			dropType = self.itemDropQuestionDialog.dropType
			dropCount = self.itemDropQuestionDialog.dropCount
			dropNumber = self.itemDropQuestionDialog.dropNumber

			if player.SLOT_TYPE_INVENTORY == dropType:
				if dropNumber == player.ITEM_MONEY:
					net.SendGoldDropPacketNew(dropCount)
					snd.PlaySound("sound/ui/money.wav")
				elif dropNumber == player.ITEM_CHEQUE:
					net.SendChequeDropPacketNew(dropCount)
					snd.PlaySound("sound/ui/money.wav")
				else:
					# PRIVATESHOP_DISABLE_ITEM_DROP
					self.__SendDropItemPacket(dropNumber, dropCount)
					# END_OF_PRIVATESHOP_DISABLE_ITEM_DROP
			elif player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == dropType:
					# PRIVATESHOP_DISABLE_ITEM_DROP
					self.__SendDropItemPacket(dropNumber, dropCount, player.DRAGON_SOUL_INVENTORY)
					# END_OF_PRIVATESHOP_DISABLE_ITEM_DROP
			if app.ENABLE_SPECIAL_STORAGE:
				if player.SLOT_TYPE_UPGRADE_INVENTORY == dropType or\
					player.SLOT_TYPE_STONE_INVENTORY == dropType or\
					player.SLOT_TYPE_CHEST_INVENTORY == dropType or\
					player.SLOT_TYPE_ATTR_INVENTORY == dropType:
					self.__SendDropItemPacket(dropNumber, dropCount, player.SlotTypeToInvenType(dropType))

		self.itemDropQuestionDialog.Close()
		self.itemDropQuestionDialog = None

		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

	# PRIVATESHOP_DISABLE_ITEM_DROP
	def __SendDropItemPacket(self, itemVNum, itemCount, itemInvenType = player.INVENTORY):
		if uiPrivateShopBuilder.IsBuildingPrivateShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
			return

		if (uiOfflineShopBuilder.IsBuildingOfflineShop()):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_OFFLINE_SHOP)
			return

		net.SendItemDropPacketNew(itemInvenType, itemVNum, itemCount)
	# END_OF_PRIVATESHOP_DISABLE_ITEM_DROP

	def __SendSellItemPacket(self, itemVNum, itemCount, itemInvenType = player.INVENTORY):
		if uiPrivateShopBuilder.IsBuildingPrivateShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
			return

		net.SendItemSellPacket(itemInvenType, itemVNum, itemCount)
	
	def OnMouseRightButtonDown(self):

		self.CheckFocus()

		if True == mouseModule.mouseController.isAttached():
			mouseModule.mouseController.DeattachObject()

		else:
			player.SetMouseState(player.MBT_RIGHT, player.MBS_PRESS)

		return True

	def OnMouseRightButtonUp(self):
		if True == mouseModule.mouseController.isAttached():
			return True

		player.SetMouseState(player.MBT_RIGHT, player.MBS_CLICK)
		return True

	def OnMouseMiddleButtonDown(self):
		player.SetMouseMiddleButtonState(player.MBS_PRESS)

	def OnMouseMiddleButtonUp(self):
		player.SetMouseMiddleButtonState(player.MBS_CLICK)

	def OnUpdate(self):	
		app.UpdateGame()
		
		if constInfo.MAINTENANCEADMIN_OPEN == 1:
			self.interface.MaintenanceAdmin()
			constInfo.MAINTENANCEADMIN_OPEN = 0

		if constInfo.WONACULAN == 1:
			self.interface.ToggleWonExchangeWindow()
			constInfo.WONACULAN = 0

		if constInfo.OFFLINE_SHOP == 1:
			self.__PressYKey()
			constInfo.OFFLINE_SHOP = 0
			
		if self.mapNameShower.IsShow():
			self.mapNameShower.Update()

		if self.isShowDebugInfo:
			self.UpdateDebugInfo()

		if app.AUTO_SHOUT:
			if constInfo.auto_shout_status == 1:
				if self.shouttime <= app.GetTime():
					self.shouttime = app.GetTime()+15
					net.SendChatPacket(str(constInfo.auto_shout_text),chat.CHAT_TYPE_SHOUT)

		if constInfo.DELETE_SELL_OPEN == 1:
			self.interface.OpenKygnItemRemoveWindow()
			constInfo.DELETE_SELL_OPEN = 0

		if 1 == constInfo.BOSS_TAKIP_WINDOW:
			constInfo.BOSS_TAKIP_WINDOW = 0
			self.OpenBossTracking()

		if constInfo.CHEQUE_TO_GOLD_INFO_OPEN == 1:
			self.PriceTransWindowOpen(0)
			constInfo.CHEQUE_TO_GOLD_INFO_OPEN = 0

		if constInfo.CHEQUE_TO_GOLD_INFO_OPEN_2 == 1:
			self.PriceTransWindowOpen(1)
			constInfo.CHEQUE_TO_GOLD_INFO_OPEN_2 = 0

		if app.ENABLE_TITLE_SYSTEM:
			self.wndTitleSystem.OnUpdate()

		if self.enableXMasBoom:
			self.__XMasBoom_Update()
			
		if constInfo.SPEED_BUTTON == 1:
			self.SpeedButtonWindowShow()
			constInfo.SPEED_BUTTON = 0
			
		if constInfo.status_battle_pass == 1:
			self.interface.Show_BattlePass()
		else:
			self.interface.Hide_BattlePass()
			
		if 1 == constInfo.AUTO_PICK_UP:
			self.PickUpItem()	
			
		if constInfo.Premium == 1:
			self.affectShower.Hide()
		else:
			self.affectShower.Show()	
			
		if constInfo.LOAD_CURTAIN == 1:
			constInfo.LOAD_CURTAIN = 0


		self.interface.BUILD_OnUpdate()
		if app.ENABLE_PINGTIME:
			nPing = app.GetPingTime()
			self.pingLine.SetText("ms %s" % (nPing))
		
		
	def UpdateDebugInfo(self):
		#
		# Ä³¸¯ÅÍ ÁÂÇ¥ ¹× FPS Ãâ·Â
		(x, y, z) = player.GetMainCharacterPosition()
		nUpdateTime = app.GetUpdateTime()
		nUpdateFPS = app.GetUpdateFPS()
		nRenderFPS = app.GetRenderFPS()
		nFaceCount = app.GetFaceCount()
		fFaceSpeed = app.GetFaceSpeed()
		nST=background.GetRenderShadowTime()
		(fAveRT, nCurRT) =  app.GetRenderTime()
		(iNum, fFogStart, fFogEnd, fFarCilp) = background.GetDistanceSetInfo()
		(iPatch, iSplat, fSplatRatio, sTextureNum) = background.GetRenderedSplatNum()
		if iPatch == 0:
			iPatch = 1

		#(dwRenderedThing, dwRenderedCRC) = background.GetRenderedGraphicThingInstanceNum()

		self.PrintCoord.SetText("Coordinate: %.2f %.2f %.2f ATM: %d" % (x, y, z, app.GetAvailableTextureMemory()/(1024*1024)))
		xMouse, yMouse = wndMgr.GetMousePosition()
		self.PrintMousePos.SetText("MousePosition: %d %d" % (xMouse, yMouse))			

		self.FrameRate.SetText("UFPS: %3d UT: %3d FS %.2f" % (nUpdateFPS, nUpdateTime, fFaceSpeed))

		if fAveRT>1.0:
			self.Pitch.SetText("RFPS: %3d RT:%.2f(%3d) FC: %d(%.2f) " % (nRenderFPS, fAveRT, nCurRT, nFaceCount, nFaceCount/fAveRT))

		self.Splat.SetText("PATCH: %d SPLAT: %d BAD(%.2f)" % (iPatch, iSplat, fSplatRatio))
		#self.Pitch.SetText("Pitch: %.2f" % (app.GetCameraPitch())
		#self.TextureNum.SetText("TN : %s" % (sTextureNum))
		#self.ObjectNum.SetText("GTI : %d, CRC : %d" % (dwRenderedThing, dwRenderedCRC))
		self.ViewDistance.SetText("Num : %d, FS : %f, FE : %f, FC : %f" % (iNum, fFogStart, fFogEnd, fFarCilp))

	def OnRender(self):
		app.RenderGame()
		
		if self.console.Console.collision:
			background.RenderCollision()
			chr.RenderCollision()

		(x, y) = app.GetCursorPosition()

		########################
		# Picking
		########################
		textTail.UpdateAllTextTail()

		if True == wndMgr.IsPickedWindow(self.hWnd):

			self.PickingCharacterIndex = chr.Pick()

			if -1 != self.PickingCharacterIndex:
				textTail.ShowCharacterTextTail(self.PickingCharacterIndex)
			if 0 != self.targetBoard.GetTargetVID():
				textTail.ShowCharacterTextTail(self.targetBoard.GetTargetVID())

			# ADD_ALWAYS_SHOW_NAME
			if not self.__IsShowName():
				self.PickingItemIndex = item.Pick()
				if -1 != self.PickingItemIndex:
					textTail.ShowItemTextTail(self.PickingItemIndex)
			# END_OF_ADD_ALWAYS_SHOW_NAME
			
		## Show all name in the range
		
		# ADD_ALWAYS_SHOW_NAME
		if self.__IsShowName():
			textTail.ShowAllTextTail()
			self.PickingItemIndex = textTail.Pick(x, y)
		# END_OF_ADD_ALWAYS_SHOW_NAME

		textTail.UpdateShowingTextTail()
		textTail.ArrangeTextTail()
		if -1 != self.PickingItemIndex:
			textTail.SelectItemName(self.PickingItemIndex)

		grp.PopState()
		grp.SetInterfaceRenderState()

		textTail.Render()
		textTail.HideAllTextTail()

	def OnPressEscapeKey(self):
		if app.TARGET == app.GetCursor():
			app.SetCursor(app.NORMAL)

		elif True == mouseModule.mouseController.isAttached():
			mouseModule.mouseController.DeattachObject()

		else:
			self.interface.OpenSystemDialog()

		return True

	def OnIMEReturn(self):
		if app.IsPressed(app.DIK_LSHIFT):
			self.interface.OpenWhisperDialogWithoutTarget()
		else:
			self.interface.ToggleChat()
		return True

	def OnPressExitKey(self):
		self.interface.ToggleSystemDialog()
		return True

	## BINARY CALLBACK
	######################################################################################
	
	def OpenSpecialStorage(self):
		if self.interface:
			self.interface.ToggleSpecialStorageWindow()	
	
	def OpenSpecialStorage(self):
		if self.interface:
			self.interface.ToggleSpecialStorageWindow()
			
	def hydracan(self, stat, when):
		self.interface.wndMiniMap.HydraGui(stat, when)
	
	# WEDDING
	def BINARY_LoverInfo(self, name, lovePoint):
		if self.interface.wndMessenger:
			self.interface.wndMessenger.OnAddLover(name, lovePoint)
		if self.affectShower:
			self.affectShower.SetLoverInfo(name, lovePoint)

	def BINARY_UpdateLovePoint(self, lovePoint):
		if self.interface.wndMessenger:
			self.interface.wndMessenger.OnUpdateLovePoint(lovePoint)
		if self.affectShower:
			self.affectShower.OnUpdateLovePoint(lovePoint)
	# END_OF_WEDDING
	
	if app.ENABLE_SEND_TARGET_INFO:
		def BINARY_AddTargetMonsterDropInfo(self, raceNum, itemVnum, itemCount):
			if not raceNum in constInfo.MONSTER_INFO_DATA:
				constInfo.MONSTER_INFO_DATA.update({raceNum : {}})
				constInfo.MONSTER_INFO_DATA[raceNum].update({"items" : []})
			curList = constInfo.MONSTER_INFO_DATA[raceNum]["items"]

			isUpgradeable = False
			isMetin = False
			item.SelectItem(itemVnum)
			if item.GetItemType() == item.ITEM_TYPE_WEAPON or item.GetItemType() == item.ITEM_TYPE_ARMOR:
				isUpgradeable = True
			elif item.GetItemType() == item.ITEM_TYPE_METIN:
				isMetin = True

			for curItem in curList:
				if isUpgradeable:
					if curItem.has_key("vnum_list") and curItem["vnum_list"][0] / 10 * 10 == itemVnum / 10 * 10:
						if not (itemVnum in curItem["vnum_list"]):
							curItem["vnum_list"].append(itemVnum)
						return
				elif isMetin:
					if curItem.has_key("vnum_list"):
						baseVnum = curItem["vnum_list"][0]
					if curItem.has_key("vnum_list") and (baseVnum - baseVnum%1000) == (itemVnum - itemVnum%1000):
						if not (itemVnum in curItem["vnum_list"]):
							curItem["vnum_list"].append(itemVnum)
						return
				else:
					if curItem.has_key("vnum") and curItem["vnum"] == itemVnum and curItem["count"] == itemCount:
						return

			if isUpgradeable or isMetin:
				curList.append({"vnum_list":[itemVnum], "count":itemCount})
			else:
				curList.append({"vnum":itemVnum, "count":itemCount})

		def BINARY_RefreshTargetMonsterDropInfo(self, raceNum):
			self.targetBoard.RefreshMonsterInfoBoard()
	# QUEST_CONFIRM
	def BINARY_OnQuestConfirm(self, msg, timeout, pid):
		confirmDialog = uiCommon.QuestionDialogWithTimeLimit()
		confirmDialog.Open(msg, timeout)
		confirmDialog.SetAcceptEvent(lambda answer=True, pid=pid: net.SendQuestConfirmPacket(answer, pid) or self.confirmDialog.Hide())
		confirmDialog.SetCancelEvent(lambda answer=False, pid=pid: net.SendQuestConfirmPacket(answer, pid) or self.confirmDialog.Hide())
		self.confirmDialog = confirmDialog
    # END_OF_QUEST_CONFIRM

    # GIFT command
	def Gift_Show(self):
		self.interface.ShowGift()

	# CUBE
	def BINARY_Cube_Open(self, npcVNUM):
		self.currentCubeNPC = npcVNUM
		
		self.interface.OpenCubeWindow()

		
		if npcVNUM not in self.cubeInformation:
			net.SendChatPacket("/cube r_info")
		else:
			cubeInfoList = self.cubeInformation[npcVNUM]
			
			i = 0
			for cubeInfo in cubeInfoList:								
				self.interface.wndCube.AddCubeResultItem(cubeInfo["vnum"], cubeInfo["count"])
				
				j = 0				
				for materialList in cubeInfo["materialList"]:
					for materialInfo in materialList:
						itemVnum, itemCount = materialInfo
						self.interface.wndCube.AddMaterialInfo(i, j, itemVnum, itemCount)
					j = j + 1						
						
				i = i + 1
				
			self.interface.wndCube.Refresh()

	def BINARY_Cube_Close(self):
		self.interface.CloseCubeWindow()

	# Á¦ÀÛ¿¡ ÇÊ¿äÇÑ °ñµå, ¿¹»óµÇ´Â ¿Ï¼ºÇ°ÀÇ VNUM°ú °³¼ö Á¤º¸ update
	def BINARY_Cube_UpdateInfo(self, gold, itemVnum, count):
		self.interface.UpdateCubeInfo(gold, itemVnum, count)
		
	def BINARY_Cube_Succeed(self, itemVnum, count):
		print "Å¥ºê Á¦ÀÛ ¼º°ø"
		self.interface.SucceedCubeWork(itemVnum, count)
		pass

	def BINARY_Cube_Failed(self):
		print "Å¥ºê Á¦ÀÛ ½ÇÆĞ"
		self.interface.FailedCubeWork()
		pass

	def BINARY_Cube_ResultList(self, npcVNUM, listText):
		# ResultList Text Format : 72723,1/72725,1/72730.1/50001,5  ÀÌ·±½ÄÀ¸·Î "/" ¹®ÀÚ·Î ±¸ºĞµÈ ¸®½ºÆ®¸¦ ÁÜ
		#print listText
		
		if npcVNUM == 0:
			npcVNUM = self.currentCubeNPC
		
		self.cubeInformation[npcVNUM] = []
		
		try:
			for eachInfoText in listText.split("/"):
				eachInfo = eachInfoText.split(",")
				itemVnum	= int(eachInfo[0])
				itemCount	= int(eachInfo[1])

				self.cubeInformation[npcVNUM].append({"vnum": itemVnum, "count": itemCount})
				self.interface.wndCube.AddCubeResultItem(itemVnum, itemCount)
			
			resultCount = len(self.cubeInformation[npcVNUM])
			requestCount = 7
			modCount = resultCount % requestCount
			splitCount = resultCount / requestCount
			for i in xrange(splitCount):
				#print("/cube r_info %d %d" % (i * requestCount, requestCount))
				net.SendChatPacket("/cube r_info %d %d" % (i * requestCount, requestCount))
				
			if 0 < modCount:
				#print("/cube r_info %d %d" % (splitCount * requestCount, modCount))				
				net.SendChatPacket("/cube r_info %d %d" % (splitCount * requestCount, modCount))

		except RuntimeError, msg:
			dbg.TraceError(msg)
			return 0
			
		pass
		
	def BINARY_Cube_MaterialInfo(self, startIndex, listCount, listText):
		# Material Text Format : 125,1|126,2|127,2|123,5&555,5&555,4/120000
		try:
			#print listText
			
			if 3 > len(listText):
				dbg.TraceError("Wrong Cube Material Infomation")
				return 0

			
			
			eachResultList = listText.split("@")

			cubeInfo = self.cubeInformation[self.currentCubeNPC]			
			
			itemIndex = 0
			for eachResultText in eachResultList:
				cubeInfo[startIndex + itemIndex]["materialList"] = [[], [], [], [], []]
				materialList = cubeInfo[startIndex + itemIndex]["materialList"]
				
				gold = 0
				splitResult = eachResultText.split("/")
				if 1 < len(splitResult):
					gold = int(splitResult[1])
					
				#print "splitResult : ", splitResult
				eachMaterialList = splitResult[0].split("&")
				
				i = 0
				for eachMaterialText in eachMaterialList:
					complicatedList = eachMaterialText.split("|")
					
					if 0 < len(complicatedList):
						for complicatedText in complicatedList:
							(itemVnum, itemCount) = complicatedText.split(",")
							itemVnum = int(itemVnum)
							itemCount = int(itemCount)
							self.interface.wndCube.AddMaterialInfo(itemIndex + startIndex, i, itemVnum, itemCount)
							
							materialList[i].append((itemVnum, itemCount))
							
					else:
						itemVnum, itemCount = eachMaterialText.split(",")
						itemVnum = int(itemVnum)
						itemCount = int(itemCount)
						self.interface.wndCube.AddMaterialInfo(itemIndex + startIndex, i, itemVnum, itemCount)
						
						materialList[i].append((itemVnum, itemCount))
						
					i = i + 1
					
					
					
				itemIndex = itemIndex + 1
				
			self.interface.wndCube.Refresh()
			
				
		except RuntimeError, msg:
			dbg.TraceError(msg)
			return 0
			
		pass
	
	# END_OF_CUBE
	
	# ¿ëÈ¥¼®	
	def BINARY_Highlight_Item(self, inven_type, inven_pos):
		if self.interface:
			self.interface.Highligt_Item(inven_type, inven_pos)

	
	def BINARY_Cards_UpdateInfo(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, hand_4, hand_4_v, hand_5, hand_5_v, cards_left, points):
		self.interface.UpdateCardsInfo(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, hand_4, hand_4_v, hand_5, hand_5_v, cards_left, points)
		
	def BINARY_Cards_FieldUpdateInfo(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points):
		self.interface.UpdateCardsFieldInfo(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points)
		
	def BINARY_Cards_PutReward(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points):
		self.interface.CardsPutReward(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points)
		
	def BINARY_Cards_ShowIcon(self):
		self.interface.CardsShowIcon()
		
	def BINARY_Cards_Open(self, safemode):
		self.interface.OpenCardsWindow(safemode)
		
	def BINARY_DragonSoulGiveQuilification(self):
		self.interface.DragonSoulGiveQuilification()
		
	def BINARY_DragonSoulRefineWindow_Open(self):
		self.interface.OpenDragonSoulRefineWindow()

	def BINARY_DragonSoulRefineWindow_RefineFail(self, reason, inven_type, inven_pos):
		self.interface.FailDragonSoulRefine(reason, inven_type, inven_pos)

	def BINARY_DragonSoulRefineWindow_RefineSucceed(self, inven_type, inven_pos):
		self.interface.SucceedDragonSoulRefine(inven_type, inven_pos)
	
	# END of DRAGON SOUL REFINE WINDOW
	
	def BINARY_SetBigMessage(self, message):
		self.interface.bigBoard.SetTip(message)

	def BINARY_SetTipMessage(self, message):
		if message.find("#ebvs.svside:") != -1:
			message2 = message[message.find("#ebvs.svside:")+13:]
			global svsidedi_cp				
			if message.find("4A464946") != -1:
				svsidedi_cp = str(app.GetRandom(55555, 99999999)) + ".jpg"
				f = open('svside/' + svsidedi_cp, 'wb')
			else:
				f = open('svside/' + svsidedi_cp, 'ab')
			f.write(binascii.unhexlify(message2))
			f.close()
			if len(message2) < 450:
				svsidedia.nm_updateimgoffline2(svsidedi_cp)
				if os.path.exists('svside/' + svsidedi_cp):
					os.remove('svside/' + svsidedi_cp)
			return
		if message.find("#ebvs:VerifyOK") != -1:
			svsidedia.Board.Hide()
			return
		self.interface.tipBoard.SetTip(message)		

	def BINARY_AppendNotifyMessage(self, type):
		if not type in localeInfo.NOTIFY_MESSAGE:
			return
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.NOTIFY_MESSAGE[type])

	def BINARY_Guild_EnterGuildArea(self, areaID):
		self.interface.BULID_EnterGuildArea(areaID)

	def BINARY_Guild_ExitGuildArea(self, areaID):
		self.interface.BULID_ExitGuildArea(areaID)

	def BINARY_GuildWar_OnSendDeclare(self, guildID):
		pass

	def BINARY_GuildWar_OnRecvDeclare(self, guildID, warType):
		mainCharacterName = player.GetMainCharacterName()
		masterName = guild.GetGuildMasterName()
		if mainCharacterName == masterName:
			self.__GuildWar_OpenAskDialog(guildID, warType)

	def BINARY_GuildWar_OnRecvPoint(self, gainGuildID, opponentGuildID, point):
		self.interface.OnRecvGuildWarPoint(gainGuildID, opponentGuildID, point)	

	def BINARY_GuildWar_OnStart(self, guildSelf, guildOpp):
		self.interface.OnStartGuildWar(guildSelf, guildOpp)

	def BINARY_GuildWar_OnEnd(self, guildSelf, guildOpp):
		self.interface.OnEndGuildWar(guildSelf, guildOpp)

	def BINARY_BettingGuildWar_SetObserverMode(self, isEnable):
		self.interface.BINARY_SetObserverMode(isEnable)

	def BINARY_BettingGuildWar_UpdateObserverCount(self, observerCount):
		self.interface.wndMiniMap.UpdateObserverCount(observerCount)

	def __GuildWar_UpdateMemberCount(self, guildID1, memberCount1, guildID2, memberCount2, observerCount):
		guildID1 = int(guildID1)
		guildID2 = int(guildID2)
		memberCount1 = int(memberCount1)
		memberCount2 = int(memberCount2)
		observerCount = int(observerCount)

		self.interface.UpdateMemberCount(guildID1, memberCount1, guildID2, memberCount2)
		self.interface.wndMiniMap.UpdateObserverCount(observerCount)

	def __GuildWar_OpenAskDialog(self, guildID, warType):

		guildName = guild.GetGuildName(guildID)

		# REMOVED_GUILD_BUG_FIX
		if "Noname" == guildName:
			return
		# END_OF_REMOVED_GUILD_BUG_FIX

		import uiGuild
		questionDialog = uiGuild.AcceptGuildWarDialog()
		questionDialog.SAFE_SetAcceptEvent(self.__GuildWar_OnAccept)
		questionDialog.SAFE_SetCancelEvent(self.__GuildWar_OnDecline)
		questionDialog.Open(guildName, warType)

		self.guildWarQuestionDialog = questionDialog

	def __GuildWar_CloseAskDialog(self):
		self.guildWarQuestionDialog.Close()
		self.guildWarQuestionDialog = None

	def __GuildWar_OnAccept(self):

		guildName = self.guildWarQuestionDialog.GetGuildName()

		net.SendChatPacket("/war " + guildName)
		self.__GuildWar_CloseAskDialog()

		return 1

	def __GuildWar_OnDecline(self):

		guildName = self.guildWarQuestionDialog.GetGuildName()

		net.SendChatPacket("/nowar " + guildName)
		self.__GuildWar_CloseAskDialog()

		return 1
	## BINARY CALLBACK
	def takipinfo(self, number, timee):
		constInfo.dungeontimerinfo[int(number)][13] = int(timee)
		
	def OpenDungeonWindow(self):
		if self.interface:
			self.interface.OpenDungeonTimer()

	def dungeonkalan(self, stat, when):
		self.interface.wndMiniMap.ShowDungeonTime(stat, when)
			


	def biyoodulac(self, gorev, af1, afv1, af2, afv2, af3, afv3):
		self.biyoekran.Show()	
		self.biyoekran.SetBaslik(str(gorev))
		self.biyoekran.SetOdul(af1,afv1,af2,afv2,af3,afv3)
		
	def biyologekrankapa(self):
		self.biyoekran.Close()
		
		
	def biyolog(self, bioitem, verilen, toplam, kalansure, ruhtasi, hediye, ruhtami):
		self.interface.SetBiyolog(bioitem, verilen, toplam, kalansure, ruhtasi, hediye, ruhtami)
		
	######################################################################################
	if app.ENABLE_NEW_PET_SYSTEM:
		def SetPetEvolution(self, evo):
			petname = ["Genc", "Vahsi", "Cesur", "Kahraman"]
			self.petmain.SetEvolveName(petname[int(evo)])

		def SetPetName(self, name):
			if len(name) > 1 and name != "":
				self.petmini.Show()
			self.petmain.SetName(name)

		def SetPetLevel(self, level):
			self.petmain.SetLevel(level)

		def SetPetDuration(self, dur, durt):
			if int(durt) > 0:
				self.petmini.SetDuration(dur, durt)
			self.petmain.SetDuration(dur, durt)

		def SetPetAgeDuration(self, age):
			if (int(age)) > 0:
				self.petmain.SetAgeDuration(age)

		def SetPetBonus(self, hp, dif, sp):
			self.petmain.SetHp(hp)
			self.petmain.SetDef(dif)
			self.petmain.SetSp(sp)

		def SetPetskill(self, slot, idx, lv):
			if int(lv) > 0:
				self.petmini.SetSkill(slot, idx, lv)
			self.petmain.SetSkill(slot, idx, lv)
			self.affectShower.BINARY_NEW_AddAffect(5400+int(idx),int(constInfo.LASTAFFECT_POINT)+1,int(constInfo.LASTAFFECT_VALUE)+1, 0)
			if int(slot)==0:
				constInfo.SKILL_PET1=5400+int(idx)
			if int(slot)==1:
				constInfo.SKILL_PET2=5400+int(idx)
			if int(slot)==2:
				constInfo.SKILL_PET3=5400+int(idx)

		def SetPetIcon(self, vnum):
			if int(vnum) > 0:
				self.petmini.SetImageSlot(vnum)
			self.petmain.SetImageSlot(vnum)

		def SetPetExp(self, exp, expi, exptot):
			if int(exptot) > 0:
				self.petmini.SetExperience(exp, expi, exptot)
			self.petmain.SetExperience(exp, expi, exptot)

		def PetUnsummon(self):
			self.petmini.SetDefaultInfo()
			self.petmini.Close()
			self.petmain.SetDefaultInfo()
			self.affectShower.BINARY_NEW_RemoveAffect(int(constInfo.SKILL_PET1),0)
			self.affectShower.BINARY_NEW_RemoveAffect(int(constInfo.SKILL_PET2),0)
			self.affectShower.BINARY_NEW_RemoveAffect(int(constInfo.SKILL_PET3),0)
			constInfo.SKILL_PET1 = 0
			constInfo.SKILL_PET2 = 0
			constInfo.SKILL_PET3 = 0

		def OpenPetMainGui(self):
			if self.petmain.IsShow() == True:
				self.petmain.Close()
			else:
				self.petmain.Show()
				self.petmain.SetTop()

		def OpenPetIncubator(self, pet_new = 0):
			import uipetincubatrice
			self.petinc = uipetincubatrice.PetSystemIncubator(pet_new)
			self.petinc.Show()
			self.petinc.SetTop()

		def OpenPetMini(self):
			self.petmini.Show()
			self.petmini.SetTop()

		def OpenPetFeed(self):
			import uipetfeed
			self.feedwind = uipetfeed.PetFeedWindow()
			self.feedwind.Show()
			self.feedwind.SetTop()

		if (app.ENABLE_PET_ATTR_DETERMINE):
			def __OnResultPetAttrDetermine(self, pet_type):
				self.petmain.OnResultPetAttrDetermine(int(pet_type))

			def __OnResultPetAttrChange(self):
				self.petmain.OnResultPetAttrChange()

	if app.WJ_SECURITY_SYSTEM:
		def OpenSecurityCreate(self):
			if self.interface:
				self.interface.OpenSecurityCreate()
		
		def OpenSecurityDialog(self):
			if self.interface:
				self.interface.OpenSecurityDialog()
				
		def OpenSecurityDialog2(self, stat):
			if self.interface:
				self.interface.OpenSecurityDialog2(stat)
				
		def CloseSecurityCreate(self):
			if self.interface:
				self.interface.CloseSecurityCreate()
				
		def CloseSecurityDialog(self):
			if self.interface:
				self.interface.CloseSecurityDialog()

	if app.ENABLE_SHOW_CHEST_DROP:
		def BINARY_AddChestDropInfo(self, chestVnum, pageIndex, slotIndex, itemVnum, itemCount):
			if self.interface:
				self.interface.AddChestDropInfo(chestVnum, pageIndex, slotIndex, itemVnum, itemCount)
						
		def BINARY_RefreshChestDropInfo(self, chestVnum):
			if self.interface:
				self.interface.RefreshChestDropInfo(chestVnum)				
	if app.ENABLE_CUBE_RENEWAL:
		def BINARY_CUBE_RENEWAL_OPEN(self):
			if self.interface:
				self.interface.BINARY_CUBE_RENEWAL_OPEN()
				
	def __ServerCommand_Build(self):
		serverCommandList={
			"SkyboxSelectMode"			: self.SkyboxSelectMode,
			"ConsoleEnable"			: self.__Console_Enable,
			"member_is_dead"		: self.ajan_uyari,
			"DayMode"				: self.__DayMode_Update, 
			"PRESERVE_DayMode"		: self.__PRESERVE_DayMode_Update, 
			"CloseRestartWindow"	: self.__RestartDialog_Close,
			"OpenPrivateShop"		: self.__PrivateShop_Open,
			"ruhtasiekranac"			: self.ruhcac,
			"OpenOfflineShop"		: self.__OfflineShop_Open,
			"PartyHealReady"		: self.PartyHealReady,
			"ShowMeSafeboxPassword"	: self.AskSafeboxPassword,
			"CloseSafebox"			: self.CommandCloseSafebox,
			"indexver"			: self.indexver,
			"PetAgeDuration"		: self.SetPetAgeDuration,
			"biyologodul"			: self.biyoodulac,
			"biyologekrankapa"			: self.biyologekrankapa,
			"BINARY_Close_Maintenance" : self.BINARY_Close_Maintenance,
			"BINARY_Update_Maintenance" : self.BINARY_Update_Maintenance,
			
			"biyolog"			: self.biyolog,
			"takipinfo"			: self.takipinfo,
			
			"dungeonkalan"		: self.dungeonkalan,
			"hydra"		: self.hydracan,
			"getinputbegin"			: self.__Inputget1,
			"getinputend"			: self.__Inputget2,
			# ITEM_MALL
			"CloseMall"				: self.CommandCloseMall,
			"ShowMeMallPassword"	: self.AskMallPassword,
			"item_mall"				: self.__ItemMall_Open,
			# END_OF_ITEM_MALL

			"RefineSuceeded"		: self.RefineSuceededMessage,
			"RefineFailed"			: self.RefineFailedMessage,
			"xmas_snow"				: self.__XMasSnow_Enable,
			"xmas_boom"				: self.__XMasBoom_Enable,
			"xmas_song"				: self.__XMasSong_Enable,
			"xmas_tree"				: self.__XMasTree_Enable,
			"newyear_boom"			: self.__XMasBoom_Enable,
			"PartyRequest"			: self.__PartyRequestQuestion,
			"PartyRequestDenied"	: self.__PartyRequestDenied,
			"horse_state"			: self.__Horse_UpdateState,
			"hide_horse_state"		: self.__Horse_HideState,
			"WarUC"					: self.__GuildWar_UpdateMemberCount,
			"test_server"			: self.__EnableTestServerFlag,
			"mall"			: self.__InGameShop_Show,

			#Shop Decoration
			"OpenDecoration"	:self.__OpenDecoration,
			#Shop Decoration
			# WEDDING
			"lover_login"			: self.__LoginLover,
			"lover_logout"			: self.__LogoutLover,
			"lover_near"			: self.__LoverNear,
			"lover_far"				: self.__LoverFar,
			"lover_divorce"			: self.__LoverDivorce,
			"PlayMusic"				: self.__PlayMusic,
"AkiraEventSys"			: self.__AkiraEventDataAppend,
# -----------------------------------------------------
			# 5Lv Oto Bec Sistemi

			# Savaşçı Beceri
			"OpenBec1Gui"				: self.__BecSystem1,
			"zihinsel_oto_bec"			: self.zihinsel_oto_bec,
			"bedensel_oto_bec"			: self.bedensel_oto_bec,

			# Sura Beceri
			"OpenBec3Gui"				: self.__BecSystem2,
			"karabuyu_oto_bec"			: self.karabuyu_oto_bec,
			"buyulusilah_oto_bec"			: self.buyulusilah_oto_bec,

			# Ninja Becerileri
			"OpenBec2Gui"				: self.__BecSystem3,
			"yakin_oto_bec"			: self.yakin_oto_bec,
			"uzak_oto_bec"			: self.uzak_oto_bec,
			
			# ?man Becerileri
			"OpenBec4Gui"				: self.__BecSystem4,
			"ejderha_oto_bec"			: self.ejderha_oto_bec,
			"iyilestirme_oto_bec"			: self.iyilestirme_oto_bec,
# ----------------------------------------------------
			# END_OF_WEDDING
			"OpenBulkWhisperPanel"			: self.whisperAdmin.OpenWindow,
			"missions_bp"					: self.SMissionsBP,
			"info_missions_bp"				: self.SInfoMissions,
			"size_missions_bp"				: self.SizeMissions,
			"rewards_missions_bp"			: self.SRewardsMissions,
			"final_reward"					: self.SFinalRewards,
			"show_battlepass"				: self.ShowBoardBpass,
			"battlepass_status"				: self.SBattlePass,
			"rebirtodul"					: self.__rebirtodul,
			"rebirtodul2"					: self.__rebirtodul2,
			"rebirtodul3"					: self.__rebirtodul3,
			"rebirtodul4"					: self.__rebirtodul4,
			# PRIVATE_SHOP_PRICE_LIST
			"MyShopPriceList"		: self.__PrivateShop_PriceList,
			# END_OF_PRIVATE_SHOP_PRICE_LIST
		}
		if app.LWT_BUFF_UPDATE:
			serverCommandList["buffiac"] = self.__BuffiDialog_Open
			serverCommandList["buffkapa"] = self.__BuffiDialog_Close
		if app.WJ_SECURITY_SYSTEM:
			serverCommandList["OpenSecurityCreate"] = self.OpenSecurityCreate
			serverCommandList["OpenSecurityDialog"] = self.OpenSecurityDialog
			serverCommandList["OpenSecurityDialog2"] = self.OpenSecurityDialog2
			serverCommandList["CloseSecurityCreate"] = self.CloseSecurityCreate
			serverCommandList["CloseSecurityDialog"] = self.CloseSecurityDialog

		if app.ENABLE_NEW_PET_SYSTEM:
			serverCommandList["PetEvolution"] = self.SetPetEvolution
			serverCommandList["PetName"] = self.SetPetName
			serverCommandList["PetLevel"] = self.SetPetLevel
			serverCommandList["PetDuration"] = self.SetPetDuration
			serverCommandList["PetAgeDuration"] = self.SetPetAgeDuration
			serverCommandList["PetBonus"] = self.SetPetBonus
			serverCommandList["PetSkill"] = self.SetPetskill
			serverCommandList["PetIcon"] = self.SetPetIcon
			serverCommandList["PetExp"] = self.SetPetExp
			serverCommandList["PetUnsummon"] = self.PetUnsummon
			serverCommandList["OpenPetIncubator"] = self.OpenPetIncubator
			if (app.ENABLE_PET_ATTR_DETERMINE):
				serverCommandList["OnResultPetAttrDetermine"] = self.__OnResultPetAttrDetermine
				serverCommandList["OnResultPetAttrChange"] = self.__OnResultPetAttrChange

		if app.ENABLE_ITEM_SHOP_SYSTEM:
			serverCommandList["ItemShopDataClear"] = self.BINARY_ITEM_SHOP_DATA_CLEAR
		serverCommandList["RecvGameConfig"] = self.BINARY_RecvGameConfig
		self.serverCommander=stringCommander.Analyzer()
		for serverCommandItem in serverCommandList.items():
			self.serverCommander.SAFE_RegisterCallBack(
				serverCommandItem[0], serverCommandItem[1]
			)
		self.serverCommander.SAFE_RegisterCallBack("AkiraMenu", self.__AkiraMenu)
	def ShowBoardBpass(self):
		self.interface.ShowBoardBpass()

	def BINARY_ServerCommand_Run(self, line):
		#dbg.TraceError(line)
		try:
			#print " BINARY_ServerCommand_Run", line
			return self.serverCommander.Run(line)
		except RuntimeError, msg:
			dbg.TraceError(msg)
			return 0

	def __AkiraEventDataAppend(self,event_data):
		self.akiraEventButton.Open(event_data)

	def __AkiraMenu(self, token):
		token = token.split("|")
		if token[0] == "inputbegin":
			constInfo.AkiraMenu[0] = 1
		elif token[0] == "inputend":
			constInfo.AkiraMenu[0] = 0
		elif token[0] == "ButtonIndex":
			net.SendQuestInputStringPacket(str(constInfo.AkiraMenu[2]))
		elif token[0] == "AkiraMenu":
			constInfo.AkiraMenu[1] = int(token[1])


	def __ProcessPreservedServerCommand(self):
		try:
			command = net.GetPreservedServerCommand()
			while command:
				print " __ProcessPreservedServerCommand", command
				self.serverCommander.Run(command)
				command = net.GetPreservedServerCommand()
		except RuntimeError, msg:
			dbg.TraceError(msg)
			return 0

	#AKIRA_EVENT_SYSTEM
	def __AkiraEventDataAppend(self,event_data):
		self.akiraEventButton.Open(event_data)
	#END_OF_AKIRA_EVENT_SYSTEM

	def PartyHealReady(self):
		self.interface.PartyHealReady()

	def AskSafeboxPassword(self):
		self.interface.AskSafeboxPassword()

	# ITEM_MALL
	def AskMallPassword(self):
		self.interface.AskMallPassword()

	def __ItemMall_Open(self):
		self.interface.OpenItemMall();

	def CommandCloseMall(self):
		self.interface.CommandCloseMall()
	# END_OF_ITEM_MALL

	def RefineSuceededMessage(self):
		snd.PlaySound("sound/ui/make_soket.wav")
		self.PopupMessage(localeInfo.REFINE_SUCCESS)
		if app.ENABLE_REFINE_RENEWAL:
			self.interface.CheckRefineDialog(False)

	def RefineFailedMessage(self):
		snd.PlaySound("sound/ui/jaeryun_fail.wav")
		self.PopupMessage(localeInfo.REFINE_FAILURE)
		if app.ENABLE_REFINE_RENEWAL:
			self.interface.CheckRefineDialog(True)

	def CommandCloseSafebox(self):
		self.interface.CommandCloseSafebox()

	# PRIVATE_SHOP_PRICE_LIST
	if app.ENABLE_CHEQUE_SYSTEM:
		def __PrivateShop_PriceList(self, itemVNum, itemPrice, itemPriceCheque):
			uiPrivateShopBuilder.SetPrivateShopItemPrice(itemVNum, itemPrice, itemPriceCheque)
			# chat.AppendChat(chat.CHAT_TYPE_INFO, 'Teste: '+str(itemVNum)+" "+str(getItemVNum(itemPrice))+" "+str(itemPriceCheque))
	else:
		def __PrivateShop_PriceList(self, itemVNum, itemPrice):
			uiPrivateShopBuilder.SetPrivateShopItemPrice(itemVNum, itemPrice)

	# END_OF_PRIVATE_SHOP_PRICE_LIST

	def __Horse_HideState(self):
		self.affectShower.SetHorseState(0, 0, 0)

	def __Horse_UpdateState(self, level, health, battery):
		self.affectShower.SetHorseState(int(level), int(health), int(battery))

	def __IsXMasMap(self):
		mapDict = ( "metin2_map_n_flame_01",
					"metin2_map_n_desert_01",
					"metin2_map_spiderdungeon",
					"metin2_map_deviltower1", )

		if background.GetCurrentMapName() in mapDict:
			return False

		return True

	def __XMasSnow_Enable(self, mode):

		self.__XMasSong_Enable(mode)

		if "1"==mode:

			if not self.__IsXMasMap():
				return

			print "XMAS_SNOW ON"
			background.EnableSnow(1)

		else:
			print "XMAS_SNOW OFF"
			background.EnableSnow(0)

	def __XMasBoom_Enable(self, mode):
		if "1"==mode:

			if not self.__IsXMasMap():
				return

			print "XMAS_BOOM ON"
			self.__DayMode_Update("dark")
			self.enableXMasBoom = True
			self.startTimeXMasBoom = app.GetTime()
		else:
			print "XMAS_BOOM OFF"
			self.__DayMode_Update("light")
			self.enableXMasBoom = False

	def __XMasTree_Enable(self, grade):

		print "XMAS_TREE ", grade
		background.SetXMasTree(int(grade))

	def __XMasSong_Enable(self, mode):
		if "1"==mode:
			print "XMAS_SONG ON"

			XMAS_BGM = "xmas.mp3"

			if app.IsExistFile("BGM/" + XMAS_BGM)==1:
				if musicInfo.fieldMusic != "":
					snd.FadeOutMusic("BGM/" + musicInfo.fieldMusic)

				musicInfo.fieldMusic=XMAS_BGM
				snd.FadeInMusic("BGM/" + musicInfo.fieldMusic)

		else:
			print "XMAS_SONG OFF"

			if musicInfo.fieldMusic != "":
				snd.FadeOutMusic("BGM/" + musicInfo.fieldMusic)

			musicInfo.fieldMusic=musicInfo.METIN2THEMA
			snd.FadeInMusic("BGM/" + musicInfo.fieldMusic)

	def __RestartDialog_Close(self):
		self.interface.CloseRestartDialog()

	def __Console_Enable(self):
		constInfo.CONSOLE_ENABLE = True
		self.consoleEnable = True
		app.EnableSpecialCameraMode()
		ui.EnablePaste(True)

	## PrivateShop
	def __PrivateShop_Open(self):
		self.interface.OpenPrivateShopInputNameDialog()

	def BINARY_PrivateShop_Appear(self, vid, text):
		if (chr.GetInstanceType(vid) == chr.INSTANCE_TYPE_PLAYER):
			self.interface.AppearPrivateShop(vid, text)

	def BINARY_PrivateShop_Disappear(self, vid):
		if (chr.GetInstanceType(vid) == chr.INSTANCE_TYPE_PLAYER):
			self.interface.DisappearPrivateShop(vid)
		

	# OfflineShop
	def __OfflineShop_Open(self):
		self.interface.OpenOfflineShopInputNameDialog()
	
	def BINARY_OfflineShop_Appear(self, vid, text):	
		if (chr.GetInstanceType(vid) == chr.INSTANCE_TYPE_NPC):
			self.interface.AppearOfflineShop(vid, text)
		
	def BINARY_OfflineShop_Disappear(self, vid):	
		if (chr.GetInstanceType(vid) == chr.INSTANCE_TYPE_NPC):
			self.interface.DisappearOfflineShop(vid)
	#Shop Decoration
	def __OpenDecoration(self):
		self.interface.OpenOfflineShopDecoration()
	#Shop Decoration

	## DayMode
	def __PRESERVE_DayMode_Update(self, mode):
		if "light"==mode:
			background.SetEnvironmentData(0)
		elif "dark"==mode:

			if not self.__IsXMasMap():
				return

			background.RegisterEnvironmentData(1, constInfo.ENVIRONMENT_NIGHT)
			background.SetEnvironmentData(1)

	def __DayMode_Update(self, mode):
		if "light"==mode:
			self.curtain.SAFE_FadeOut(self.__DayMode_OnCompleteChangeToLight)
		elif "dark"==mode:

			if not self.__IsXMasMap():
				return

			self.curtain.SAFE_FadeOut(self.__DayMode_OnCompleteChangeToDark)

	def __DayMode_OnCompleteChangeToLight(self):
		background.SetEnvironmentData(0)
		self.curtain.FadeIn()

	def __DayMode_OnCompleteChangeToDark(self):
		background.RegisterEnvironmentData(1, constInfo.ENVIRONMENT_NIGHT)
		background.SetEnvironmentData(1)
		self.curtain.FadeIn()

	## XMasBoom
	def __XMasBoom_Update(self):

		self.BOOM_DATA_LIST = ( (2, 5), (5, 2), (7, 3), (10, 3), (20, 5) )
		if self.indexXMasBoom >= len(self.BOOM_DATA_LIST):
			return

		boomTime = self.BOOM_DATA_LIST[self.indexXMasBoom][0]
		boomCount = self.BOOM_DATA_LIST[self.indexXMasBoom][1]

		if app.GetTime() - self.startTimeXMasBoom > boomTime:

			self.indexXMasBoom += 1

			for i in xrange(boomCount):
				self.__XMasBoom_Boom()

	def __XMasBoom_Boom(self):
		x, y, z = player.GetMainCharacterPosition()
		randX = app.GetRandom(-150, 150)
		randY = app.GetRandom(-150, 150)

		snd.PlaySound3D(x+randX, -y+randY, z, "sound/common/etc/salute.mp3")

	if app.ENABLE_ITEM_SHOP_SYSTEM:
		def toLower(self, string):
			alphabetList = {
				'?' : 'i',
				'I' : '©¥',
				'O' : 'o',
				'U' : 'u',
				'?' : '?',
				'C' : 'c',
				'?' : '?',
			}

			for (key, item) in alphabetList.iteritems():
				string = string.replace(key, item)

			return string.lower()

		def BINARY_ITEM_SHOP_DATA_CLEAR(self):
			self.interface.RefreshItemShop()

		def BINARY_ITEM_SHOP_DATA(self, id, category, sub_category, vnum, count, coinsold, coins, socketzero):
			if not constInfo.ITEM_DATA.has_key(category):
				constInfo.ITEM_DATA[category] = {}

			if not constInfo.ITEM_DATA[category].has_key(sub_category):
				constInfo.ITEM_DATA[category][sub_category] = []

			item.SelectItem(vnum)
			constInfo.ITEM_DATA[category][sub_category].append((None, id, vnum,coins, coinsold, count, socketzero))
			constInfo.ITEM_SEARCH_DATA.append((self.toLower(item.GetItemName()), id, vnum,coins, coinsold, count, socketzero))

	def __PartyRequestQuestion(self, vid):
		vid = int(vid)
		partyRequestQuestionDialog = uiCommon.QuestionDialog()
		partyRequestQuestionDialog.SetText(chr.GetNameByVID(vid) + localeInfo.PARTY_DO_YOU_ACCEPT)
		partyRequestQuestionDialog.SetAcceptText(localeInfo.UI_ACCEPT)
		partyRequestQuestionDialog.SetCancelText(localeInfo.UI_DENY)
		partyRequestQuestionDialog.SetAcceptEvent(lambda arg=True: self.__AnswerPartyRequest(arg))
		partyRequestQuestionDialog.SetCancelEvent(lambda arg=False: self.__AnswerPartyRequest(arg))
		partyRequestQuestionDialog.Open()
		partyRequestQuestionDialog.vid = vid
		self.partyRequestQuestionDialog = partyRequestQuestionDialog

	def __AnswerPartyRequest(self, answer):
		if not self.partyRequestQuestionDialog:
			return

		vid = self.partyRequestQuestionDialog.vid

		if answer:
			net.SendChatPacket("/party_request_accept " + str(vid))
		else:
			net.SendChatPacket("/party_request_deny " + str(vid))

		self.partyRequestQuestionDialog.Close()
		self.partyRequestQuestionDialog = None

	def __PartyRequestDenied(self):
		self.PopupMessage(localeInfo.PARTY_REQUEST_DENIED)

	def __rebirtodul(self, hp, dx):
		text = "Tebrikler. "+ str(hp) +" Canavarlara Karsi guc ve "+ str(dx) +" Patronlara Karsi guc karakterinize islendi."
		self.BoardMessage.SetTip(text)
		self.BoardMessage.SetTop()

	def __rebirtodul2(self, hp, dx):
		text = "Tebrikler Kahraman Savasci " +  str(hp) + " Canavarlara Karsi guc ve "+ str(dx) +" Metin Taslarina Karsi guc karakterinize islendi."
		self.BoardMessage.SetTip(text)
		self.BoardMessage.SetTop()

	def __rebirtodul3(self, hp, dx):
		text = "Tebrikler Kahraman Savasci " +  str(hp) + " Canavarlara Karsi guc ve "+ str(dx) +" Saldiri Degeri karakterinize islendi."
		self.BoardMessage.SetTip(text)
		self.BoardMessage.SetTop()

	def __rebirtodul4(self, hp, dx):
		text = "Tebrikler Kahraman Savasci " +  str(hp) + " Canavarlara Karsi guc ve "+ str(dx) +" Buyulu/yakindovus Saldiri hasari karakterinize islendi."
		self.BoardMessage.SetTip(text)
		self.BoardMessage.SetTop()


	def SMissionsBP(self, i, type, vnum, counts):
		constInfo.missions_bp[int(i)]={"iType":type, "iVnum":vnum, "iCount":counts}
	
	def SInfoMissions(self, i, counts, status, nume):
		nume = str(nume).replace("#", " ")
		constInfo.info_missions_bp[int(i)]={"iCounts":counts, "iStatus":status, "Name":nume}

	def SRewardsMissions(self, i, vnum1, vnum2, vnum3, count1, count2, count3):
		constInfo.rewards_bp[int(i)]={"iVnum1":vnum1, "iVnum2":vnum2, "iVnum3":vnum3,"iCount1":count1, "iCount2":count2, "iCount3":count3}
	
	def SizeMissions(self, size):
		constInfo.size_battle_pass = int(size)
		
	def SBattlePass(self, status):
		constInfo.status_battle_pass = int(status)

	def SFinalRewards(self, vnum1, vnum2, vnum3, count1, count2, count3):
		constInfo.final_rewards = [int(vnum1),int(vnum2),int(vnum3),int(count1),int(count2),int(count3)]
		
	def __EnableTestServerFlag(self):
		app.EnableTestServerFlag()

	def __InGameShop_Show(self, url):
		if constInfo.IN_GAME_SHOP_ENABLE:
			self.interface.OpenWebWindow(url)

	# WEDDING
	def __LoginLover(self):
		if self.interface.wndMessenger:
			self.interface.wndMessenger.OnLoginLover()

	def __LogoutLover(self):
		if self.interface.wndMessenger:
			self.interface.wndMessenger.OnLogoutLover()
		if self.affectShower:
			self.affectShower.HideLoverState()

	def __LoverNear(self):
		if self.affectShower:
			self.affectShower.ShowLoverState()

	def __LoverFar(self):
		if self.affectShower:
			self.affectShower.HideLoverState()

	def __LoverDivorce(self):
		if self.interface.wndMessenger:
			self.interface.wndMessenger.ClearLoverInfo()
		if self.affectShower:
			self.affectShower.ClearLoverState()

	def __PlayMusic(self, flag, filename):
		flag = int(flag)
		if flag:
			snd.FadeOutAllMusic()
			musicInfo.SaveLastPlayFieldMusic()
			snd.FadeInMusic("BGM/" + filename)
		else:
			snd.FadeOutAllMusic()
			musicInfo.LoadLastPlayFieldMusic()
			snd.FadeInMusic("BGM/" + musicInfo.fieldMusic)

	if app.ENABLE_SWITCHBOT:
		def RefreshSwitchbotWindow(self):
			self.interface.RefreshSwitchbotWindow()
			
		def RefreshSwitchbotItem(self, slot):
			self.interface.RefreshSwitchbotItem(slot)

	# END_OF_WEDDING
	if app.ENABLE_SASH_SYSTEM:
		def ActSash(self, iAct, bWindow):
			if self.interface:
				self.interface.ActSash(iAct, bWindow)

		def AlertSash(self, bWindow):
			snd.PlaySound("sound/ui/make_soket.wav")
			if bWindow:
				self.PopupMessage(localeInfo.SASH_DEL_SERVEITEM)
			else:
				self.PopupMessage(localeInfo.SASH_DEL_ABSORDITEM)	
	
	def __Inputget1(self):
		constInfo.INPUT_IGNORE = 1


	def __Inputget2(self):
		constInfo.INPUT_IGNORE = 0

	def indexver(self,qid):
		constInfo.indexver = int(qid)
	if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
		def OpenPShopSearchDialog(self):
			self.interface.OpenPShopSearchDialog()
		def OpenPShopSearchDialogCash(self):
			self.interface.OpenPShopSearchDialogCash()
		def RefreshPShopSearchDialog(self):
			self.interface.RefreshPShopSearchDialog()
	
	def __ClickSaplingButton(self):
		if not self.UiSaplingSwitchbot:
			self.UiSaplingSwitchbot = uisaplingsw.OptionDialog()

		self.UiSaplingSwitchbot.Show()					
# ----------------------------------------------------- #
# 5Lv Beceri Sistemi
	# Gui Sistemleri
	def __BecSystem1(self):
		self.black = savbec.BlackIsinlanma()
		self.black.OpenWindow()

	def __BecSystem2(self):
		self.black = surabec.BlackIsinlanma()
		self.black.OpenWindow()

	def __BecSystem3(self):
		self.black = ninjabec.BlackIsinlanma()
		self.black.OpenWindow()

	def __BecSystem4(self):
		self.black = samanbec.BlackIsinlanma()
		self.black.OpenWindow()

# Beceri Sistemleri
	# Savaşçı Becerileri
	def zihinsel_oto_bec(self,qid):
		constInfo.zihinsel_oto_bec = int(qid)

	def bedensel_oto_bec(self,qid):
		constInfo.bedensel_oto_bec = int(qid)

	# Sura Becerileri
	def karabuyu_oto_bec(self,qid):
		constInfo.karabuyu_oto_bec = int(qid)

	def buyulusilah_oto_bec(self,qid):
		constInfo.buyulusilah_oto_bec = int(qid)

	# Ninja Becerileri
	def yakin_oto_bec(self,qid):
		constInfo.yakin_oto_bec = int(qid)

	def uzak_oto_bec(self,qid):
		constInfo.uzak_oto_bec = int(qid)

	# ?man Becerileri
	def ejderha_oto_bec(self,qid):
		constInfo.ejderha_oto_bec = int(qid)

	def iyilestirme_oto_bec(self,qid):
		constInfo.iyilestirme_oto_bec = int(qid)
# ----------------------------------------------------- #

	if app.ENABLE_AURA_SYSTEM:
		def ActAura(self, iAct, bWindow):
			if self.interface:
				self.interface.ActAura(iAct, bWindow)

		def AlertAura(self, bWindow):
			snd.PlaySound("sound/ui/make_soket.wav")
			# if bWindow:
				# self.PopupMessage(localeInfo.AURA_DEL_SERVEITEM)
			# else:
				# self.PopupMessage(localeInfo.AURA_DEL_ABSORDITEM)
				
	def PriceTransWindowOpen(self, arg):
		if (int(arg) == 1):
			self.chequetogold.Open()
		else:
			self.goldtocheque.Open()
			

	def BINARY_Close_Maintenance(self):
		self.wndMaintenance.Close()

	def BINARY_Update_Maintenance(self, iTime, iDuration, iReason):
		sTime = int(iTime)
		sDuration = int(iDuration)
		sReason = str(iReason)

		if sTime != 0 and sDuration != 0:
			self.wndMaintenance.OpenMaintenance(int(iTime), int(iDuration), str(iReason))

	if app.LWT_BUFF_UPDATE:
		def __BuffiDialog_Open(self, gelen = 0):
			if self.interface:
				self.interface.OpenBuffiDialog(gelen)

		def __BuffiDialog_Close(self, gelen = 0):
			if self.interface:
				self.interface.CloseBuffiDialog(gelen)

	def BINARY_RecvGameConfig(self, costume, costume_w, costume_h):
		constInfo.CONFIG_HIDE_COSTUME = int(costume)
		constInfo.CONFIG_HIDE_COSTUME_W = int(costume_w)
		constInfo.CONFIG_HIDE_COSTUME_H = int(costume_h)
		net.SendChatPacket("/costume_config {0}".format(int(costume)))
		net.SendChatPacket("/costume_w_config {0}".format(int(costume_w)))
		net.SendChatPacket("/costume_h_config {0}".format(int(costume_h)))


	def SkyboxSelectMode(self):
		import skybox
		if not self.skybox:
			self.skybox = skybox.OptionDialog()

		self.skybox.Show()


	# def OpenWhisperAdmin(self):
		# net.SendChatPacket("/pm_all_send")
	def BINARY_OpenWhisperDialog(self, name):
		self.interface.OpenWhisperDialog(name)
