#########################################
# title_name		: System Pack		#
# filename			: root				#
# author			: Bvural41			#
# version			: Version 0.0.2		#
# date				: 2015 04 11		#
# update			: 2019 02 05		#
#########################################

import dbg
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
import musicInfo
from _weakref import proxy
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
		self.changeMusicButton = 0
		self.changeMusicButton2 = 0
		
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
			self.changeMusicButton = GetObject("bgm_button")
			self.changeMusicButton2 = GetObject("bgm_button2")

		except:
			import exception
			exception.Abort("OptionDialog.__Load_BindObject")

	def __Load(self):
		self.__Load_LoadScript("uiscript/efsunbotgui.py")
		self.__Load_BindObject()

		self.SetCenterPosition()
		
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

		self.changeMusicButton.SAFE_SetEvent(self.__OnClickChangeMusicButton)
		self.changeMusicButton2.SAFE_SetEvent(self.__OnClickChangeMusicButton2)

	def __OnClickChangeMusicButton(self):
		self.Hide()
		if not constInfo.normalefsunbotu:
			execfile('uiefsunbotnorm.py',{})
			constInfo.normalefsunbotu = 1
		else:
			chat.AppendChat(1, "Normal Efsun botu zaten aktif.")

	def __OnClickChangeMusicButton2(self):
		self.Hide()
		if not constInfo.yesilefsunbotu:
			execfile('uiefsunbotyesil.py',{})
			constInfo.yesilefsunbotu = 1
		else:
			chat.AppendChat(1, "Yeþil Efsun botu zaten aktif.")

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
