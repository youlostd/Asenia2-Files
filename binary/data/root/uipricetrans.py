import ui
import uiCommon
import localeInfo
import player
import net
import chat
import constInfo

class ChequeToGoldWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.questionDlg 		= None
		self.__LoadWindow()
	
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/chequetogoldwindow.py")
		except:
			import exception
			exception.Abort("ChequeTicketWindow.__LoadWindow.LoadScriptFile")
			
		try:
			self.mainBoard		= self.GetChild("main_board")
			self.inputValue		= self.GetChild("InputValue")
			self.acceptBtn		= self.GetChild("Lbutton")
			self.goldtochequewindowBtn	= self.GetChild("goldtochequewindowBtn")
		except:
			import exception
			exception.Abort("ChequeTicketWindow.__LoadWindow.BindObject")
			
		try:
			self.inputValue.OnIMEReturn = self.__OnClickConfirmButton
			self.inputValue.OnPressEscapeKey = self.Close
			self.mainBoard.SetCloseEvent(ui.__mem_func__(self.Close))
			self.acceptBtn.SetEvent(ui.__mem_func__(self.__OnClickConfirmButton))
			self.goldtochequewindowBtn.SetEvent(ui.__mem_func__(self.GoldToChequeWindow))
		except:
			import exception
			exception.Abort("ChequeTicketWindow.__LoadWindow.SetEvent")
		
	def Close(self):
		self.Hide()
		
		if (self.inputValue is not None):
			self.inputValue.SetText("1")
		
		if (self.inputValue is not None and self.inputValue.IsFocus()):
			self.inputValue.KillFocus()
		
		if (self.questionDlg is not None):
			self.questionDlg.Close()
			self.questionDlg = None

	def GoldToChequeWindow(self):
		constInfo.CHEQUE_TO_GOLD_INFO_OPEN = 1
		self.Close()

	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def Open(self):
		self.SetCenterPosition()
		self.Show()
		
	def __OnClickConfirmButton(self):
		net.SendChatPacket("/use_gold_ticket {0}".format(int(self.inputValue.GetText())))

class GoldToChequeWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.questionDlg 		= None
		self.__LoadWindow()
	
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/goldtochequewindow.py")
		except:
			import exception
			exception.Abort("ChequeTicketWindow.__LoadWindow.LoadScriptFile")
			
		try:
			self.mainBoard		= self.GetChild("main_board")
			self.inputValue		= self.GetChild("InputValue")
			self.acceptBtn		= self.GetChild("Lbutton")
			self.chequetogoldwindowBtn	= self.GetChild("chequetogoldwindowBtn")
		except:
			import exception
			exception.Abort("ChequeTicketWindow.__LoadWindow.BindObject")
			
		try:
			self.inputValue.OnIMEReturn = self.__OnClickConfirmButton
			self.inputValue.OnPressEscapeKey = self.Close
			self.mainBoard.SetCloseEvent(ui.__mem_func__(self.Close))
			self.acceptBtn.SetEvent(ui.__mem_func__(self.__OnClickConfirmButton))
			self.chequetogoldwindowBtn.SetEvent(ui.__mem_func__(self.ChequeToGoldWindow))
		except:
			import exception
			exception.Abort("ChequeTicketWindow.__LoadWindow.SetEvent")

	def ChequeToGoldWindow(self):
		constInfo.CHEQUE_TO_GOLD_INFO_OPEN_2 = 1
		self.Close()
		
	def Close(self):
		self.Hide()
		
		if (self.inputValue is not None):
			self.inputValue.SetText("100000000")
		
		if (self.inputValue is not None and self.inputValue.IsFocus()):
			self.inputValue.KillFocus()
		
		if (self.questionDlg is not None):
			self.questionDlg.Close()
			self.questionDlg = None
		
	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def Open(self):
		self.SetCenterPosition()
		self.Show()
		
	def __OnClickConfirmButton(self):
		net.SendChatPacket("/use_cheque_ticket {0}".format(int(self.inputValue.GetText())))
		
