import wndMgr
import ui
import ime
import localeInfo
import chat
import app

class PickMoneyDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.unitValue = 0
		self.maxValue = 0
		if app.ENABLE_CHEQUE_SYSTEM:
			self.unitValue2 = 0
			self.maxValue2 = 0
		self.eventAccept = 0

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/PickMoneyDialog.py")
		except:
			import exception
			exception.Abort("MoneyDialog.LoadDialog.LoadScript")

		try:
			self.board = self.GetChild("board")
			self.maxValueTextLine = self.GetChild("max_value")
			self.pickValueEditLine = self.GetChild("money_value")
			if app.ENABLE_CHEQUE_SYSTEM:
				self.maxValueTextLine2 = self.GetChild("cheque_max_value")
				self.pickValueEditLine2 = self.GetChild("cheque_value")
			self.acceptButton = self.GetChild("accept_button")
			self.cancelButton = self.GetChild("cancel_button")
		except:
			import exception
			exception.Abort("MoneyDialog.LoadDialog.BindObject")

		self.pickValueEditLine.SetReturnEvent(ui.__mem_func__(self.OnAccept))
		self.pickValueEditLine.SetEscapeEvent(ui.__mem_func__(self.Close))
		if app.ENABLE_CHEQUE_SYSTEM:
			self.pickValueEditLine2.SetReturnEvent(ui.__mem_func__(self.OnAccept))
			self.pickValueEditLine2.SetEscapeEvent(ui.__mem_func__(self.Close))
		self.acceptButton.SetEvent(ui.__mem_func__(self.OnAccept))
		self.cancelButton.SetEvent(ui.__mem_func__(self.Close))
		self.board.SetCloseEvent(ui.__mem_func__(self.Close))

	def Destroy(self):
		self.ClearDictionary()
		self.eventAccept = 0
		self.maxValue = 0
		self.pickValueEditLine = 0
		if app.ENABLE_CHEQUE_SYSTEM:
			self.maxValue2 = 0
			self.pickValueEditLine2 = 0
		self.acceptButton = 0
		self.cancelButton = 0
		self.board = None

	def SetTitleName(self, text):
		self.board.SetTitleName(text)

	def SetAcceptEvent(self, event):
		self.eventAccept = event

	def SetMax(self, max):
		self.pickValueEditLine.SetMax(max)
		if app.ENABLE_CHEQUE_SYSTEM:
			self.pickValueEditLine2.SetMax(5)

	if app.ENABLE_CHEQUE_SYSTEM:
		def Open(self, maxValue, maxValue2, unitValue=0, unitValue2=0):
			if localeInfo.IsYMIR() or localeInfo.IsCHEONMA() or localeInfo.IsHONGKONG():
				unitValue = ""
				unitValue2 = ""

			width = self.GetWidth()
			(mouseX, mouseY) = wndMgr.GetMousePosition()

			if mouseX + width/2 > wndMgr.GetScreenWidth():
				xPos = wndMgr.GetScreenWidth() - width
			elif mouseX - width/2 < 0:
				xPos = 0
			else:
				xPos = mouseX - width/2

			self.SetPosition(xPos, mouseY - self.GetHeight() - 20)

			self.maxValueTextLine.SetText(" / " + str(maxValue))
			self.maxValueTextLine2.SetText(" / " + str(maxValue2))

			self.pickValueEditLine.SetText(str(unitValue))
			self.pickValueEditLine.SetFocus()
			
			self.pickValueEditLine2.SetText(str(unitValue2))
			self.pickValueEditLine2.SetFocus()

			ime.SetCursorPosition(1)

			self.unitValue = unitValue
			self.unitValue2 = unitValue2
			self.maxValue = maxValue
			self.maxValue2 = maxValue2
			self.Show()
			self.SetTop()
	else:
		def Open(self, maxValue, unitValue=1):
			if localeInfo.IsYMIR() or localeInfo.IsCHEONMA() or localeInfo.IsHONGKONG():
				unitValue = ""

			width = self.GetWidth()
			(mouseX, mouseY) = wndMgr.GetMousePosition()

			if mouseX + width/2 > wndMgr.GetScreenWidth():
				xPos = wndMgr.GetScreenWidth() - width
			elif mouseX - width/2 < 0:
				xPos = 0
			else:
				xPos = mouseX - width/2

			self.SetPosition(xPos, mouseY - self.GetHeight() - 20)

			self.maxValueTextLine.SetText(" / " + str(maxValue))

			self.pickValueEditLine.SetText(str(unitValue))
			self.pickValueEditLine.SetFocus()

			ime.SetCursorPosition(1)

			self.unitValue = unitValue
			self.maxValue = maxValue
			self.Show()
			self.SetTop()

	def Close(self):
		self.pickValueEditLine.KillFocus()
		if app.ENABLE_CHEQUE_SYSTEM:
			self.pickValueEditLine2.KillFocus()
		self.Hide()

	if app.ENABLE_CHEQUE_SYSTEM:
		def OnAccept(self):
			text = self.pickValueEditLine.GetText()
			text2 = self.pickValueEditLine2.GetText()

			if len(text) > 0 and text.isdigit() or len(text2) > 0 and text2.isdigit():
				money = int(text)
				money = min(money, self.maxValue)
				cheque = int(text2)
				cheque = min(cheque, self.maxValue2)

				if money > 0 or cheque > 0:
					if self.eventAccept:
						self.eventAccept(money, cheque)

			self.Close()
	else:
		def OnAccept(self):
			text = self.pickValueEditLine.GetText()

			if len(text) > 0 and text.isdigit():
				money = long(text)
				money = min(money, self.maxValue)

				if money > 0:
					if self.eventAccept:
						self.eventAccept(money)

			self.Close()

class PickMoneyDialogExp(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.unitValue = 1
		self.maxValue = 0
		self.eventAccept = 0

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/PickMoneyDialogExp.py")
		except:
			import exception
			exception.Abort("MoneyDialog.LoadDialog.LoadScript")

		try:
			self.board = self.GetChild("board")
			self.maxValueTextLine = self.GetChild("max_value")
			self.pickValueEditLine = self.GetChild("money_value")
			self.acceptButton = self.GetChild("accept_button")
			self.cancelButton = self.GetChild("cancel_button")
		except:
			import exception
			exception.Abort("MoneyDialog.LoadDialog.BindObject")

		self.pickValueEditLine.SetReturnEvent(ui.__mem_func__(self.OnAccept))
		self.pickValueEditLine.SetEscapeEvent(ui.__mem_func__(self.Close))
		self.acceptButton.SetEvent(ui.__mem_func__(self.OnAccept))
		self.cancelButton.SetEvent(ui.__mem_func__(self.Close))
		self.board.SetCloseEvent(ui.__mem_func__(self.Close))

	def Destroy(self):
		self.ClearDictionary()
		self.eventAccept = 0
		self.maxValue = 0
		self.pickValueEditLine = 0		
		self.acceptButton = 0
		self.cancelButton = 0
		self.board = None

	def SetTitleName(self, text):
		self.board.SetTitleName(text)

	def SetAcceptEvent(self, event):
		self.eventAccept = event

	def SetMax(self, max):
		self.pickValueEditLine.SetMax(max)

	def Open(self, maxValue, unitValue=1):

		if localeInfo.IsYMIR() or localeInfo.IsCHEONMA() or localeInfo.IsHONGKONG():
			unitValue = ""

		width = self.GetWidth()
		(mouseX, mouseY) = wndMgr.GetMousePosition()

		if mouseX + width/2 > wndMgr.GetScreenWidth():
			xPos = wndMgr.GetScreenWidth() - width
		elif mouseX - width/2 < 0:
			xPos = 0
		else:
			xPos = mouseX - width/2

		self.SetPosition(xPos, mouseY - self.GetHeight() - 20)

		if localeInfo.IsARABIC():
			self.maxValueTextLine.SetText("/" + str(maxValue))
		else:
			self.maxValueTextLine.SetText(" / " + str(maxValue))

		self.pickValueEditLine.SetText(str(unitValue))
		self.pickValueEditLine.SetFocus()

		ime.SetCursorPosition(1)

		self.unitValue = unitValue
		self.maxValue = maxValue
		self.Show()
		self.SetTop()

	def Close(self):
		self.pickValueEditLine.KillFocus()
		self.Hide()

	def OnAccept(self):

		text = self.pickValueEditLine.GetText()

		if len(text) > 0 and text.isdigit():

			money = int(text)
			money = min(money, self.maxValue)

			if money > 0:
				if self.eventAccept:
					self.eventAccept(money)

		self.Close()

class TopluItemAyir(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.unitValue = 1
		self.maxValue = 0
		self.eventAccept = 0

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/picktopluitem.py")
		except:
			import exception
			exception.Abort("MoneyDialog.LoadDialog.LoadScript")

		try:
			self.board = self.GetChild("board")
			self.maxValueTextLine = self.GetChild("max_value")
			self.pickValueEditLine = self.GetChild("money_value")
			self.acceptButton = self.GetChild("accept_button")
			self.cancelButton = self.GetChild("cancel_button")
			self.topluAyirBtn = self.GetChild("cBoxToplu")
		except:
			import exception
			exception.Abort("MoneyDialog.LoadDialog.BindObject")

		self.pickValueEditLine.SetReturnEvent(ui.__mem_func__(self.OnAccept))
		self.pickValueEditLine.SetEscapeEvent(ui.__mem_func__(self.Close))
		self.acceptButton.SetEvent(ui.__mem_func__(self.OnAccept))
		self.cancelButton.SetEvent(ui.__mem_func__(self.Close))
		self.board.SetCloseEvent(ui.__mem_func__(self.Close))

	def Destroy(self):
		self.ClearDictionary()
		self.eventAccept = 0
		self.maxValue = 0
		self.pickValueEditLine = 0		
		self.acceptButton = 0
		self.cancelButton = 0
		self.topluAyirBtn = 0
		self.board = None

	def SetTitleName(self, text):
		self.board.SetTitleName(text)

	def SetAcceptEvent(self, event):
		self.eventAccept = event

	def SetMax(self, max):
		self.pickValueEditLine.SetMax(max)

	def Open(self, maxValue, unitValue=1):
		if localeInfo.IsYMIR() or localeInfo.IsCHEONMA() or localeInfo.IsHONGKONG(): unitValue = ""

		width = self.GetWidth()
		(mouseX, mouseY) = wndMgr.GetMousePosition()

		if mouseX + width/2 > wndMgr.GetScreenWidth(): xPos = wndMgr.GetScreenWidth() - width
		elif mouseX - width/2 < 0: xPos = 0
		else: xPos = mouseX - width/2

		self.SetPosition(xPos, mouseY - self.GetHeight() - 20)

		self.maxValueTextLine.SetText("/" + str(maxValue))

		self.pickValueEditLine.SetText("")
		self.pickValueEditLine.SetFocus()

		ime.SetCursorPosition(1)

		self.unitValue = unitValue
		self.maxValue = maxValue
		self.Show()
		self.SetTop()
		self.topluAyirBtn.SetUp()

	def Close(self):
		self.pickValueEditLine.KillFocus()
		self.Hide()
	
	def TopluAyirmaMi(self):
		return self.topluAyirBtn.IsDown()

	def OnAccept(self):
		text = self.pickValueEditLine.GetText()
		if len(text) != 0 and text.isdigit():
			money = int(text)
			money = min(money, self.maxValue)
			if self.eventAccept:
				self.eventAccept(money)
		self.Close()