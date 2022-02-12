import ui, net, os, app, item, chat, ime, skill, nonplayer
import wndMgr

g_isSearchCount = 120
class PopupDialog(ui.ScriptWindow):
	def __init__(self, parent):
		ui.ScriptWindow.__init__(self)
		self.__Load()
		self.__Bind()
	def __del__(self):
		ui.ScriptWindow.__del__(self)
	def __Load(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/PopupDialog.py")
		except:
			import exception
			exception.Abort("PopupDialog.__Load")
	def __Bind(self):
		try:
			self.textLine=self.GetChild("message")
			self.okButton=self.GetChild("accept")
		except:
			import exception
			exception.Abort("PopupDialog.__Bind")
		self.okButton.SAFE_SetEvent(self.__OnOK)
	def Open(self, msg):
		self.textLine.SetText(msg)
		self.SetCenterPosition()
		self.Show()
		self.SetTop()
	def __OnOK(self):
		self.Hide()


class SrcItemWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		# self.fileListBox=None
		self.tooltipItem = None
		self.__LoadWindow()
		self.ItemList = []
		self.ItemCount = len(self.ItemList)

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Close(self):
		if self.tooltipItem:
			self.tooltipItem.Hide()
		self.Hide()

	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/gosterge.py")
		except:
			import exception
			exception.Abort("PetItemWindow.LoadWindow")			
		try:
			self.petfeed = self.GetChild("board")
			self.closebtn = self.GetChild("TitleBarPet")
			self.TitleName = self.GetChild("TitleName")
			self.TitleName.SetText("Nesne Arama")
			self.okButton = self.GetChild("okButton")
			self.temizle = self.GetChild("temizle")
			self.editline = self.GetChild("InputValue")
			self.button3 = self.GetChild("button3")
			self.arama = self.GetChild("arama")
			self.okButton.SAFE_SetEvent(self.Close)
			# self.fileListBox = self.GetChild("ListBox")
			self.button3.SAFE_SetEvent(self.Close)
			self.editline.OnPressEscapeKey = self.Close
		except:
			import exception
			exception.Abort("PetItemWindow.LoadWindow")

		self.editline.SetReturnEvent(ui.__mem_func__(self.Ara))
		self.arama.SetEvent(ui.__mem_func__(self.Ara))
		self.temizle.SetEvent(ui.__mem_func__(self.temizlefunc))
		self.GetChild("TitleBarPet").SetCloseEvent(ui.__mem_func__(self.Close))
		self.button3.Hide()
		self.popupDialog=PopupDialog(self)
		# self.fileListBox.SetScrollBar(self.GetChild("ScrollBar"))
		self.SetTop()
		self.SetCenterPosition()

	def temizlefunc(self):
		self.editline.SetText("")
	
	
	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def Show(self):
		ui.ScriptWindow.Show(self)
		self.SetTop()
		self.editline.SetFocus()

	def Ara(self):
		val = int(len(self.editline.GetText()))
		if val > 1:
			string=self.toparla(self.editline.GetText())
			net.SendShopSearchFindPacket(0, str(string))
		else:
			chat.AppendChat(5, "Aranacak kelime çok kýsa")

	def toparla(self, string):
		t=string[0]
		if string[0] == "ý":
			t = "I"
		elif string[0] == "i":
			t = "Ý"
		elif string[0] == "ö":
			t = "Ö"
		elif string[0] == "ü":
			t = "Ü"
		elif string[0] == "þ":
			t = "Þ"
		elif string[0] == "ç":
			t = "Ç"
		string = t + string[1:]
		return string

	def __PopupMessage(self, msg):
		self.popupDialog.Open(msg)
