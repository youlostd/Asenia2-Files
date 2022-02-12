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
from switchbot import Bot
from switchbot2 import Bot2

import uiSelectMusic
import background

MUSIC_FILENAME_MAX_LEN = 25

blockMode = 0

class OptionDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Load()
		
		self.switchbot = Bot()
		self.switchbot.Hide()
		
		self.switchbot2 = Bot2()
		self.switchbot2.Hide()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print " -------------------------------------- DELETE SYSTEM OPTION DIALOG"

	def __Initialize(self):
		self.tilingMode = 0
		self.titleBar = 0
		self.changeMusicButton = 0
		
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
			#self.changeMusicButton = GetObject("bgm_button")
			self.change2MusicButton = GetObject("bgm_button2")
			self.change3MusicButton = GetObject("bgm_button3")

		except:
			import exception
			exception.Abort("OptionDialog.__Load_BindObject")

	def __Load(self):
		self.__Load_LoadScript("uiscript/saplingbotoption.py")
		self.__Load_BindObject()

		self.SetCenterPosition()
		
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

		#self.changeMusicButton.SAFE_SetEvent(self.__OnClickChangeMusicButton)
		self.change2MusicButton.SAFE_SetEvent(self.__OnClick2ChangeMusicButton)
		self.change3MusicButton.SAFE_SetEvent(self.__OnClick3ChangeMusicButton)
	
	#def __OnClickChangeMusicButton(self):
		#self.Hide()
		#if self.switchbot.sunRise_bot_shown == 1:
			#self.switchbot.Hide()	
		#else:
			#self.switchbot.Show()
			
	def __OnClick2ChangeMusicButton(self):
		self.Hide()
		if self.switchbot2.sunRise_bot_shown == 1:
			self.switchbot2.Hide()	
		else:
			self.switchbot2.Show()		
			
	def __OnClick3ChangeMusicButton(self):
		self.Hide()
		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "efsunbot.pyc")

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
		
