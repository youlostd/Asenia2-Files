#yeni
import ui
import player 
import constInfo
import snd
import app
import uiToolTip
import net
import dbg
import playerSettingModule
import uicharacter
import event
import chat
import grp
import item
import time
import wndMgr
import uiCommon
import os
import chr
import chrmgr
import quest
import ime
import interfaceModule
import uiScriptLocale
#yeni
import dbg
import ui
import snd
import systemSetting
import net
import chat
import app
import localeInfo
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
import background
import serverinfo
import uiOfflineShopBuilder
import constInfo
import playerSettingModule
import chrmgr
import background
import player
import musicInfo

import uiSelectMusic
import background

MUSIC_FILENAME_MAX_LEN = 25

blockMode = 0


class OptionDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Load()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print " -------------------------------------- DELETE SYSTEM OPTION DIALOG"

	def __Initialize(self):
		self.tilingMode = 0
		self.titleBar = 0
		self.skybox_button1 = []
		
	def Destroy(self):
		self.ClearDictionary()

		self.__Initialize()
		print " -------------------------------------- DESTROY SYSTEM OPTION DIALOG"

	def __Load_LoadScript(self, fileName):
		try:
			pyScriptLoader = ui.PythonScriptLoader()
			pyScriptLoader.LoadScriptFile(self, fileName)
		except:
			import exception
			exception.Abort("System.OptionDialog.__Load_LoadScript")

	def __Load_BindObject(self):
		try:
			GetObject = self.GetChild
			self.titleBar = GetObject("titlebar")
			self.skybox_button1.append(GetObject("skybox_button1"))
			self.skybox_button1.append(GetObject("skybox_button2"))
			self.skybox_button1.append(GetObject("skybox_button3"))
			self.skybox_button1.append(GetObject("skybox_button4"))
			self.skybox_button1.append(GetObject("skybox_button5"))
			self.skybox_button1.append(GetObject("skybox_button6"))
			self.skybox_button1.append(GetObject("skybox_button7"))
			self.skybox_button1.append(GetObject("skybox_button8"))
			self.skybox_button1.append(GetObject("skybox_button9"))
			self.skybox_button1.append(GetObject("skybox_button10"))
			self.skybox_button1.append(GetObject("skybox_button11"))
			self.skybox_button1.append(GetObject("skybox_button12"))
			self.skybox_button1.append(GetObject("skybox_button13"))

		except:
			import exception
			exception.Abort("OptionDialog.__Load_BindObject")

	def __Load(self):
		self.__Load_LoadScript("uiscript/skybox.py")
		self.__Load_BindObject()

		self.SetCenterPosition()
		
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

		self.skybox_button1[0].SAFE_SetEvent(self.__OnClickOneSkyboxSelectButton)
		self.skybox_button1[1].SAFE_SetEvent(self.__OnClickTwoSkyboxSelectButton)
		self.skybox_button1[2].SAFE_SetEvent(self.__OnClickTreeSkyboxSelectButton)
		self.skybox_button1[3].SAFE_SetEvent(self.__OnClick4SkyboxSelectButton)
		self.skybox_button1[4].SAFE_SetEvent(self.__OnClick5SkyboxSelectButton)
		self.skybox_button1[5].SAFE_SetEvent(self.__OnClick6SkyboxSelectButton)
		self.skybox_button1[6].SAFE_SetEvent(self.__OnClick7SkyboxSelectButton)
		self.skybox_button1[7].SAFE_SetEvent(self.__OnClick8SkyboxSelectButton)
		self.skybox_button1[8].SAFE_SetEvent(self.__OnClick9SkyboxSelectButton)
		self.skybox_button1[9].SAFE_SetEvent(self.__OnClick10SkyboxSelectButton)
		self.skybox_button1[10].SAFE_SetEvent(self.__OnClick11SkyboxSelectButton)
		self.skybox_button1[11].SAFE_SetEvent(self.__OnClick12SkyboxSelectButton)
		self.skybox_button1[12].SAFE_SetEvent(self.__OnClick13SkyboxSelectButton)

		self.renk = {
			"beyaz" : "|cffffffff|h",
			"mavi" : "|cff00C78C",
			"kirmizi" : "|cffff0000|h",
			"sari"	:	"|cffffff00|h",
			"gold" : "|cffffcc00|h"
		}

	def __ClickRadioButton(self, buttonList, buttonIndex):
		try:
			selButton=buttonList[buttonIndex]
		except IndexError:
			return

		for eachButton in buttonList:
			eachButton.SetUp()

		selButton.Down()

	def __ibrahimgokalp(self, index):
		self.__ClickRadioButton(self.skybox_button1, index)

	def __OnClickOneSkyboxSelectButton(self):
		background.SetEnvironmentData(0)
		constInfo.Night = 0
		#self.RefreshShowNightText()
		self.__ibrahimgokalp(0)

	def __OnClickTwoSkyboxSelectButton(self):
		background.RegisterEnvironmentData(4, constInfo.ENVIRONMENT_NIGHT)
		background.SetEnvironmentData(4)
		self.__ibrahimgokalp(1)

	def __OnClickTreeSkyboxSelectButton(self):
		background.RegisterEnvironmentData(1, constInfo.ibrahim_aksam)
		background.SetEnvironmentData(1)
		self.__ibrahimgokalp(2)

	def __OnClick4SkyboxSelectButton(self):
		background.RegisterEnvironmentData(6, constInfo.ibrahim_sky_001)
		background.SetEnvironmentData(6)
		self.__ibrahimgokalp(3)

	def __OnClick5SkyboxSelectButton(self):
		background.RegisterEnvironmentData(7, constInfo.ibrahim_sky_002)
		background.SetEnvironmentData(7)
		self.__ibrahimgokalp(4)

	def __OnClick6SkyboxSelectButton(self):
		background.RegisterEnvironmentData(8, constInfo.ibrahim_sky_003)
		background.SetEnvironmentData(8)
		self.__ibrahimgokalp(5)

	def __OnClick7SkyboxSelectButton(self):
		background.RegisterEnvironmentData(9, constInfo.ibrahim_sky_004)
		background.SetEnvironmentData(9)
		self.__ibrahimgokalp(6)
		
	def __OnClick8SkyboxSelectButton(self):
		background.RegisterEnvironmentData(10, constInfo.ibrahim_sky_005)
		background.SetEnvironmentData(10)
		self.__ibrahimgokalp(7)

	def __OnClick9SkyboxSelectButton(self):
		background.RegisterEnvironmentData(11, constInfo.ibrahim_sky_006)
		background.SetEnvironmentData(11)
		self.__ibrahimgokalp(8)

	def __OnClick10SkyboxSelectButton(self):
		background.RegisterEnvironmentData(12, constInfo.ibrahim_sky_007)
		background.SetEnvironmentData(12)
		self.__ibrahimgokalp(9)

	def __OnClick11SkyboxSelectButton(self):
		background.RegisterEnvironmentData(13, constInfo.ibrahim_sky_008)
		background.SetEnvironmentData(13)
		self.__ibrahimgokalp(10)
		
	def __OnClick12SkyboxSelectButton(self):
		background.RegisterEnvironmentData(14, constInfo.ibrahim_sky_009)
		background.SetEnvironmentData(14)
		self.__ibrahimgokalp(11)

	def __OnClick13SkyboxSelectButton(self):
		background.RegisterEnvironmentData(15, constInfo.ibrahim_sky_010)
		background.SetEnvironmentData(15)
		self.__ibrahimgokalp(12)

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
	
	def Show(self):
		ui.ScriptWindow.Show(self)

	def Close(self):
		self.Hide()

	def __NotifyChatLine(self, text):
		chat.AppendChat(chat.CHAT_TYPE_INFO, text)
		
