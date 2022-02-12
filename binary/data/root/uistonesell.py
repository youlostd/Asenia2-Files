import ui
import net
import app
import constInfo
import uiCommon
import chrmgr


class StoneSell(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Load()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print " -------------------------------------- DELETE GAME OPTION DIALOG"

	def __Initialize(self):
		self.titleBar = 0

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
			
			self.tas0 = GetObject("stonesell0")
			self.tas1 = GetObject("stonesell1")
			self.tas2 = GetObject("stonesell2")
			self.tas3 = GetObject("stonesell3")
			self.tas4 = GetObject("stonesell4")
			
			
			self.tas0.SAFE_SetEvent(self.stonesell0)
			self.tas1.SAFE_SetEvent(self.stonesell1)
			self.tas2.SAFE_SetEvent(self.stonesell2)
			self.tas3.SAFE_SetEvent(self.stonesell3)
			self.tas4.SAFE_SetEvent(self.stonesell4)



		except:
			import exception
			exception.Abort("OptionDialog.__Load_BindObject")

	def __Load(self):
		self.__Load_LoadScript("uiscript/stonesell.py")

		self.__Load_BindObject()

		self.SetCenterPosition()

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		
		
	def stonesell0(self):
		net.SendChatPacket("/stonesell0")
		
	def stonesell1(self):
		net.SendChatPacket("/stonesell1")

	def stonesell2(self):
		net.SendChatPacket("/stonesell2")
		
	def stonesell3(self):
		net.SendChatPacket("/stonesell3")	

	def stonesell4(self):
		net.SendChatPacket("/stonesell4")	

	def OnPressEscapeKey(self):
		self.Close()
		return True


	def Show(self):
		ui.ScriptWindow.Show(self)

	def Close(self):
		self.Hide()
