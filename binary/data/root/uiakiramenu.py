import ui, net, constInfo, event, uiCommon, chat, item

class AkiraMenu(ui.ScriptWindow):
	def __init__(self, wndInventory):
		import exception
		if not wndInventory:
			exception.Abort("wndInventory parameter must be set to InventoryWindow")
			return

		ui.ScriptWindow.__init__(self)
		self.isLoaded = 0
		self.wndInventory = wndInventory;
		self.wndAkiraMenuLayer = None
		self.expandBtn = None
		self.minBtn = None
		self.menuButton = {}
		self.interface = None
		self.textList=["Güvenlik Aktif/Pasif", "At Çaðýrma", "Iþýnlan", "Anti Exp", "Log Sistemi", "Oto item ve Yang Topla", "Hýzlý Ekipman Deðiþ"]
		self.iconList=["d:/ymir work/ui/game/akira_log/lock_btn_0","icon/item/50053.tga", "icon/item/71149.tga", "icon/item/70005.tga", "icon/item/50200.tga", "icon/item/70043.tga", "d:/ymir work/ui/game/taskbar/costume_Button_0"]

		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self, openMenu = FALSE):
		self.__LoadWindow()
		ui.ScriptWindow.Show(self)
		if openMenu:
			self.OpenMenu()
		else:
			self.CloseMenu()

	def Close(self):
		self.Hide()

	def IsOpeningMenu(self):
		return self.wndAkiraMenuLayer.IsShow()

	def OpenMenu(self):
		self.wndAkiraMenuLayer.Show()
		self.expandBtn.Hide()
		self.AdjustPositionAndSize()

	def CloseMenu(self):
		self.wndAkiraMenuLayer.Hide()
		self.expandBtn.Show()
		self.AdjustPositionAndSize()

	def GetBasePosition(self):
		x, y = self.wndInventory.GetGlobalPosition()
		return x - 45, y + 32*7+5

	def AdjustPositionAndSize(self):
		bx, by = self.GetBasePosition()

		if self.IsOpeningMenu():
			self.SetPosition(bx-5, by)
		else:
			self.SetPosition(bx + 35, by);

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return
		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "AkiraModules/AkiraMenuWindow.py")
		except:
			import exception
			exception.Abort("AkiraMenu.LoadWindow.LoadObject")

		try:
			self.ORIGINAL_WIDTH = self.GetWidth()
			self.wndAkiraMenuLayer = self.GetChild("AkiraMenuLayer")
			self.expandBtn = self.GetChild("ExpandBtn")
			self.minBtn = self.GetChild("MinimizeBtn")
			if len(self.textList) != len(self.iconList):
				return
			for i in xrange(len(self.textList)):
				if i == 0 or i == 6:
					x=20
					y=5+32*i
					if i == 0:
						x=25
						y=10+32*i
					self.menuButton[i] = ui.MakeButton(self, x, y, "", "", self.iconList[i]+"1.tga", self.iconList[i]+"2.tga", self.iconList[i]+"3.tga")
				else:
					self.menuButton[i] = ui.MakeButton(self, 20, 5+32*i, "", "", self.iconList[i], self.iconList[i], self.iconList[i])
				self.menuButton[i].SetEvent(ui.__mem_func__(self.__ClickMenuButton),i)
				self.menuButton[i].SetToolTipText(self.textList[i], -50, 5)

			self.expandBtn.SetEvent(ui.__mem_func__(self.OpenMenu))
			self.minBtn.SetEvent(ui.__mem_func__(self.CloseMenu))
			self.expandBtn.SetToolTipText("Hýzlý Menü Aç", -40, 20)
			self.minBtn.SetToolTipText("Hýzlý Menü Kapat", -50, 20)
		except:
			import exception
			exception.Abort("AkiraMenu.LoadWindow.BindObject")
	
	def __ClickMenuButton(self, index):
		iPacket = [0,1,2,3]
		if index in iPacket:
			constInfo.AkiraMenu[2] = index
			event.QuestButtonClick(constInfo.AkiraMenu[1])
		else:
			if index == 4:
				self.__LogWindow()
			elif index == 5:
				self.__AutoPickWindow()
			elif index == 6:
				self.__ChangeEquipWindow()

	def __LogWindow(self):
		LogWindowQuestionDialog = uiCommon.NewQuestionDialog()
		LogWindowQuestionDialog.SetText("Hangi kayýtlara bakmak istiyorsun?")
		LogWindowQuestionDialog.SetAcceptText("Ticaret Kayýtlarý")
		LogWindowQuestionDialog.SetAcceptText2("Pazar Kayýtlarý")
		LogWindowQuestionDialog.SetCancelText("Kapat")
		LogWindowQuestionDialog.SetAcceptEvent(lambda arg = 1: self.__LogWindowQuestionDialog(arg))
		LogWindowQuestionDialog.SetAcceptEvent2(lambda arg = 2: self.__LogWindowQuestionDialog(arg))
		LogWindowQuestionDialog.SetCancelEvent(lambda arg = 0: self.__LogWindowQuestionDialog(arg))
		LogWindowQuestionDialog.Open()
		self.LogWindowQuestionDialog = LogWindowQuestionDialog

	def __LogWindowQuestionDialog(self, arg):
		if not self.LogWindowQuestionDialog:
			return
		if arg:
			net.SendChatPacket("/log_open "+str(arg))
		self.LogWindowQuestionDialog.Close()
		self.LogWindowQuestionDialog = None

	def __AutoPickWindow(self):
		AutoPickQuestionDialog = uiCommon.NewQuestionDialog()
		AutoPickQuestionDialog.SetText("Ne toplamak istiyorsun?")
		AutoPickQuestionDialog.SetAcceptText("Ýtem ve Yang")
		AutoPickQuestionDialog.SetAcceptText2("Yang")
		AutoPickQuestionDialog.SetCancelText("Oto Toplama Kapat")
		AutoPickQuestionDialog.SetAcceptEvent(lambda arg = 1: self.__AutoPickQuestionDialog(arg))
		AutoPickQuestionDialog.SetAcceptEvent2(lambda arg = 2: self.__AutoPickQuestionDialog(arg))
		AutoPickQuestionDialog.SetCancelEvent(lambda arg = 0: self.__AutoPickQuestionDialog(arg))
		AutoPickQuestionDialog.Open()
		self.AutoPickQuestionDialog = AutoPickQuestionDialog

	def __AutoPickQuestionDialog(self, arg):
		if not self.AutoPickQuestionDialog:
			return
		textList=["item ve yang toplama sistemi deaktif", "item ve yang toplama sistemi aktif", "yang toplama sistemi aktif"]
		chat.AppendChat(chat.CHAT_TYPE_INFO, "Otomatik "+textList[arg]+" edildi.")
		constInfo.AkiraMenu[3] = arg
		self.AutoPickQuestionDialog.Close()
		self.AutoPickQuestionDialog = None

	def __ChangeEquipWindow(self):
		if constInfo.AkiraMenu[4] == 0:
			constInfo.AkiraMenu[4] = 1
			import uifastequip 
			uifastequip.changeequip().Show()
		else:
			constInfo.AkiraMenu[4] = 2
			
class AkiraLog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__akiraLogButtonData = {0:{},1:{}}
		self.__akiraLogData = {0:{},1:{}}

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.ClearDictionary()
		self.__akiraLogButtonData = {0:{},1:{}}

	def LoadWindow(self):
		try:
			ui.PythonScriptLoader().LoadScriptFile(self, "AkiraModules/akira_log_window.py")
		except:
			import exception
			exception.Abort("AkiraEventWindow.LoadWindow.LoadObject")
		try:
			self.board = self.GetChild("board")
			self.blackBoard = self.GetChild("BlackBoard")
			self.titleText = self.GetChild("TitleName")
			self.titleRefresh = self.GetChild("TitleRefresh")
			self.titleBar = self.GetChild("TitleBar")
			self.titleBar.SetCloseEvent(self.Close)
		except:
			import exception
			exception.Abort("AkiraEventWindow.LoadWindow.BindObject")

	def __CreateExchangeLog(self):
		self.SetSize(425,self.GetHeight())
		self.board.SetSize(self.GetWidth(),self.GetHeight())
		self.blackBoard.SetSize(self.GetWidth()-28,self.blackBoard.GetHeight())
		self.titleBar.SetWidth(self.GetWidth()-13)
		self.titleRefresh.SetPosition(self.GetWidth()-57,1)
		self.titleText.SetText("Ticaret Kayýtlarý")
		self.titleText.SetPosition(self.GetWidth()/2-len("Ticaret Kayýtlarý")/2, 3)
		def CreateText(log):
			return log[0]+" ile "+log[1]+" adlý oyuncu "+log[2]+" tarihinde ticaret yaptý."

		for i in xrange(len(self.__akiraLogData[0])):
			self.__akiraLogButtonData[0][i] = ui.MakeButton(self, 20, 40 + i*24 , "", "d:/ymir work/ui/game/akira_log/", "exchange_tab_01.tga", "exchange_tab_02.tga", "exchange_tab_02.tga")
			self.__akiraLogButtonData[0][i].Log = ui.TextLine()
			self.__akiraLogButtonData[0][i].Log.SetParent(self.__akiraLogButtonData[0][i])
			self.__akiraLogButtonData[0][i].Log.SetText(CreateText(self.__akiraLogData[0][i]))
			self.__akiraLogButtonData[0][i].Log.SetPosition(10, 3)
			self.__akiraLogButtonData[0][i].Log.Show()
	
	def __CreateShopLog(self):
		self.SetSize(575+47,self.GetHeight())
		self.board.SetSize(self.GetWidth(),self.GetHeight())
		self.blackBoard.SetSize(self.GetWidth()-28,self.blackBoard.GetHeight())
		self.titleRefresh.SetPosition(self.GetWidth()-57,1)
		self.titleBar.SetWidth(self.GetWidth()-13)
		self.titleText.SetText("Pazar Kayýtlarý")
		self.titleText.SetPosition(self.GetWidth()/2-len("Pazar Kayýtlarý")/2, 3)
		def CreateText(log):
			item.SelectItem(int(log[2]))
			return log[0]+" "+item.GetItemName()+" itemini "+log[3]+" won / "+log[4]+" yang karþýlýðýnda "+log[1]+" adlý oyuncuya "+log[5]+" tarihinde sattý."

		for i in xrange(len(self.__akiraLogData[1])):
			self.__akiraLogButtonData[1][i] = ui.MakeButton(self, 20, 40 + i*24 , "", "d:/ymir work/ui/game/akira_log/", "shop_tab_01.tga", "shop_tab_02.tga", "shop_tab_02.tga")
			self.__akiraLogButtonData[1][i].Log = ui.TextLine()
			self.__akiraLogButtonData[1][i].Log.SetParent(self.__akiraLogButtonData[1][i])
			self.__akiraLogButtonData[1][i].Log.SetText(CreateText(self.__akiraLogData[1][i]))
			self.__akiraLogButtonData[1][i].Log.SetPosition(10, 3)
			self.__akiraLogButtonData[1][i].Log.Show()

	def AkiraLogDataAppend(self, log_data):
		if log_data[1] == "Exchange":
			if log_data[2] == "Temizle":
				self.__akiraLogData[0] = {}
			else:
				self.__akiraLogData[0][len(self.__akiraLogData[0])] = (log_data[2], log_data[3], log_data[4])
		else:
			if log_data[2] == "Temizle":
				self.__akiraLogData[1] = {}
			else:
				self.__akiraLogData[1][len(self.__akiraLogData[1])] = (log_data[2], log_data[3], log_data[4], log_data[5], log_data[6], log_data[7])

	def Open(self, type):
		for j in xrange(len(self.__akiraLogButtonData)):
			for i in xrange(len(self.__akiraLogButtonData[j])):
				self.__akiraLogButtonData[j][i].Hide()
		if type == 1:
			self.titleRefresh.SetEvent(ui.__mem_func__(self.__RefreshData),1)
			self.__CreateExchangeLog()
		else:
			self.titleRefresh.SetEvent(ui.__mem_func__(self.__RefreshData),2)
			self.__CreateShopLog()
		ui.ScriptWindow.Show(self)
		self.SetCenterPosition()

	def __RefreshData(self, type):
		net.SendChatPacket("/log_open %d" % type)
	
	def Close(self):
		self.Hide()
		return TRUE

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE
