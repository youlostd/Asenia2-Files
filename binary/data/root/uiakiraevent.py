"""
###############################################
# title_name		: Event System
# date_created		: 24.11.2017
# filename			: uiAkiraEvent.py
# author			: Akira
# version			: 0.0.3
# facebook			: fb.com/abdulkadir.iskan
# skype				: akiragame01
###############################################
"""
import ui, uicards

InfoText = {
	"ayisigi" : "Seviyenize uygun Oyun içerisinde bulunan |Canavarlardan ve metinlerden düþmektedir.|Sað týklayarak açýlýr.|",
	"futboltopu" : "Seviyenize uygun Oyun içerisinde bulunan |Canavarlardan ve merinlerden düþmektedir. |Envanterinde 10 adet Futbol Topu olduðunda |sistem tarafýndan size 1 adet |Altýn Futbol Top Hediye gelir.|Sað týklayarak açýlýr.|",
	"paskalya" : "Seviyenize uygun Oyun içerisinde bulunan|Canavarlardan ve merinlerden düþmektedir.|Topladýðýn yumurtalarýn bir rengi 20 tane |olduðunda Paskalya Tavsanýna götürerek| Büyülü yumurta alabilirsin.|Sað týklayarak açýlýr.|",
	"kostum" : "Seviyenize uygun Oyun içerisinde bulunan|Metin taþlarýndan düþmektedir.|16 farklý kostüm bulunmaktadýr.|Sað týklayarak açýlýr.|",
	"okey" : "Deneme bla bla bla bla| bla bla bla bla.|",
	"sertifika" : "Seviyenize uygun Oyun içerisinde bulunan|Patron boss ve metinlerden |5 farklý sertifika düþmektedir.|Seyise götürerek binek alabilirsin.|",
	"kuzeykutusu" : "Seviyenize uygun Oyun içerisinde bulunan|Patron boss ve metinlerden düþmektedir.|Içerisinden silah kostümleri çýkar.|Sað týklayarak açýlýr.|"
}
EventName = {"ayisigi" : "Ayýþýðý Etkinliði ", "futboltopu" : "Futbol Etkinliði ", "paskalya" : "Paskalya Etkinliði ", "kostum" : "Kostüm Etkinliði ", "okey" : "Okey Etkinliði ", "sertifika" : "Sertifika Etkinliði ", "kuzeykutusu" : "Kuzey Kutusu Etkinliði "}

class AkiraEventButton(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.akiraDialog = AkiraEventDialog()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/minigamewindow.py")
		except:
			import exception
			exception.Abort("AkiraEventWindow.LoadDialog.LoadScript")

		try:
			self.gameButton = self.GetChild("minigame_rumi_button")
		except:
			import exception
			exception.Abort("AkiraEventWindow.LoadDialog.BindObject")

		self.gameButton.SetEvent(ui.__mem_func__(self.__OnClickButton))

	def __OnClickButton(self):
		self.akiraDialog.Open()

	def Open(self, event_data):
		ui.ScriptWindow.Show(self)
		self.Show()
		self.akiraDialog.AkiraEventDataAppend(event_data)

	def Destroy(self):
		self.ClearDictionary()
		self.akiraDialog = None

class AkiraEventDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.akiraEventWindow = None
		self.akiraEventInfoWindow = None
		self.akiraCardsWindow = None
		self.akiraAutoEventData = {}
		self.akiraEventData = {}
		self.akiraButtonData = {}
		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.ClearDictionary()
		self.akiraAutoEventData = {}
		self.akiraEventData = {}
		self.akiraButtonData = {}
		self.akiraEventWindow = None
		self.akiraEventInfoWindow = None
		self.akiraCardsWindow = None

	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/akira_event_dialog.py")
		except:
			import exception
			exception.Abort("AkiraEventDialog.LoadDialog.LoadScript")

		try:
			self.board = self.GetChild("board")
			self.blackBoard = self.GetChild("BlackBoard")
			self.eventBar01 = self.GetChild("titleBar01")
			self.eventBar02 = self.GetChild("titleBar02")
			self.closeButton = self.GetChild("close_button")
		except:
			import exception
			exception.Abort("AkiraEventDialog.LoadDialog.BindObject")

		self.eventBar01.Hide()
		self.eventBar02.Hide()
		self.closeButton.SetEvent(ui.__mem_func__(self.Close))

		self.akiraEventWindow = AkiraEventWindow()
		self.akiraEventWindow.LoadWindow()
		self.akiraEventWindow.Close()

		self.akiraEventInfoWindow = AkiraEventInfoWindow()
		self.akiraEventInfoWindow.LoadWindow()
		self.akiraEventInfoWindow.Close()
		
		self.akiraCardsWindow = uicards.CardsInfoWindow()
		self.akiraCardsWindow.LoadWindow()
		self.akiraCardsWindow.Close()

	def __CreatButton(self):
		if len(self.akiraEventData) <= 0:
			return
		self.eventBar01.Show()
		for i in xrange(len(self.akiraEventData)):
			self.akiraButtonData[i] = ui.MakeButton(self, 16, 50+30*i, "", "d:/ymir work/ui/public/", "XLarge_Button_01.sub", "XLarge_Button_02.sub", "XLarge_Button_03.sub",)
			self.akiraButtonData[i].SetText(self.akiraEventData[i][1])
			self.akiraButtonData[i].SetEvent(ui.__mem_func__(self.__SelectButton),i)
			text = self.akiraEventData[i][1]+"Hakkýnda Bilgi Al"
			self.akiraButtonData[i].SetToolTipText(text, -(len(text)*2+120), 5)
		self.__SetPos(1, len(self.akiraButtonData)-1)

	def __CreatAutoButton(self):
		if len(self.akiraAutoEventData) <= 0:
			return
		self.__SetPos()
		self.eventBar02.Show()
		index = len(self.akiraButtonData)
		self.akiraButtonData[index] = ui.MakeButton(self, 16, 50-self.__Control(1)+30*(index+1), "Etkinlik Takvimi", "d:/ymir work/ui/public/", "XLarge_Button_01.sub", "XLarge_Button_02.sub", "XLarge_Button_03.sub",)
		self.akiraButtonData[index].SetText("Takvim")
		self.akiraButtonData[index].SetEvent(ui.__mem_func__(self.__SelectButton),999)
		self.akiraButtonData[index].SetToolTipText("Etkinlik Takvimi", -150, 5)
		self.__SetPos(1,index)

	def __Control(self,type):
		ret = 0
		if type == 1 and len(self.akiraEventData) <= 0:
			ret = 30
		if type == 2 and len(self.akiraAutoEventData) <=0:
			ret = 30
		return ret

	def __SetPos(self, type=0, pos=0):
		if type:
			self.SetSize(self.GetWidth(),self.GetHeight()+pos*30+50-self.__Control(1)-self.__Control(2))
			self.board.SetSize(self.board.GetWidth(),self.board.GetHeight()+pos*30+50-self.__Control(1)-self.__Control(2))
			self.blackBoard.SetSize(self.blackBoard.GetWidth(),self.blackBoard.GetHeight()+pos*30+50-self.__Control(1)-self.__Control(2))
			self.eventBar02.SetPosition(18,self.board.GetHeight()-105)
			self.closeButton.SetPosition(16, self.board.GetHeight()-45)
		else:
			self.SetSize(self.GetWidth(),120)
			self.board.SetSize(self.GetWidth(),105)
			self.blackBoard.SetSize(self.blackBoard.GetWidth(),105-20)
			self.closeButton.SetPosition(0,0)

	def Close(self):
		self.Hide()
		if len(self.akiraEventData) > 0 or len(self.akiraAutoEventData) > 0:
			self.__SetPos()
		self.akiraButtonData = {}
		return TRUE

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE

	def Open(self):
		if self.IsShow():
			self.Close()
		else:
			self.__CreatButton()
			self.__CreatAutoButton()
			self.Show()

	def __SelectButton(self, index):
		self.Close()
		if index == 999 and len(self.akiraAutoEventData) > 0:
			if self.akiraEventWindow.IsOpen():
				self.akiraEventWindow.Close()
			else:
				self.akiraEventWindow.Open(self.akiraAutoEventData)
			return
		if self.akiraEventData[index][0] == "okey":
			if self.akiraCardsWindow != None:
				self.akiraCardsWindow.Open()
				return
		if self.akiraEventInfoWindow.IsOpen():
			self.akiraEventInfoWindow.Close()
		self.akiraEventInfoWindow.Open(self.akiraEventData[index][0], self.akiraEventData[index][1])

	def __ExistsEvent(self,event_flag):
		for i in xrange(len(self.akiraEventData)):
			if event_flag in self.akiraEventData[i]:
				return TRUE
		return FALSE

	def AkiraEventDataAppend(self, event_data):
		event_data = event_data.split("|")
		if event_data[0] == "auto_event":
			if self.akiraAutoEventData.has_key(int(event_data[1])):
				return
			self.akiraAutoEventData[int(event_data[1])] = (event_data[2],event_data[3],event_data[4], EventName[event_data[4]])
			return
		elif event_data[0] == "event":
			if self.__ExistsEvent(event_data[1]):
				return
			self.akiraEventData[len(self.akiraEventData)] = (event_data[1], EventName[event_data[1]])

class AkiraEventWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__autoEventButtonData = {}
		self.event_data = {}
		self.dayName = ["Pazartesi", "Salý", "Çarþamba", "Perþembe", "Cuma", "Cumartesi", "Pazar"]
		self.akiraEventInfoWindow = None
		self.akiraCardsWindow = None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.ClearDictionary()
		self.__autoEventButtonData = {}
		self.event_data = {}
		self.akiraEventInfoWindow = None
		self.akiraCardsWindow = None

	def LoadWindow(self):
		try:
			ui.PythonScriptLoader().LoadScriptFile(self, "uiscript/akira_autoevent_window.py")
		except:
			import exception
			exception.Abort("AkiraEventWindow.LoadWindow.LoadObject")
		try:
			self.GetChild("TitleBar").SetCloseEvent(self.Close)
		except:
			import exception
			exception.Abort("AkiraEventWindow.LoadWindow.BindObject")

		self.akiraEventInfoWindow = AkiraEventInfoWindow()
		self.akiraEventInfoWindow.LoadWindow()
		self.akiraEventInfoWindow.Close()
		
		self.akiraCardsWindow = uicards.CardsInfoWindow()
		self.akiraCardsWindow.LoadWindow()
		self.akiraCardsWindow.Close()

	def __CreateButton(self, event_data):
		for i in xrange(len(self.dayName)):
			self.__autoEventButtonData[i] = ui.MakeButton(self, 22, 65 + i*24 , "", "d:/ymir work/ui/game/akira_event/", "akira_event_button_01.tga", "akira_event_button_02.tga", "akira_event_button_02.tga")

			self.__autoEventButtonData[i].Day = ui.TextLine()
			self.__autoEventButtonData[i].Day.SetParent(self.__autoEventButtonData[i])
			self.__autoEventButtonData[i].Day.SetText(self.dayName[i])
			self.__autoEventButtonData[i].Day.SetPosition(25, 2)
			self.__autoEventButtonData[i].Day.Show()

			self.__autoEventButtonData[i].EventName = ui.TextLine()
			self.__autoEventButtonData[i].EventName.SetParent(self.__autoEventButtonData[i])
			self.__autoEventButtonData[i].EventName.SetText("-")
			self.__autoEventButtonData[i].EventName.SetPosition(154, 2)
			self.__autoEventButtonData[i].EventName.Show()

			self.__autoEventButtonData[i].EventStartTime = ui.TextLine()
			self.__autoEventButtonData[i].EventStartTime.SetParent(self.__autoEventButtonData[i])
			self.__autoEventButtonData[i].EventStartTime.SetText("-")
			self.__autoEventButtonData[i].EventStartTime.SetPosition(250, 2)
			self.__autoEventButtonData[i].EventStartTime.Show()

			self.__autoEventButtonData[i].EventFinishTime = ui.TextLine()
			self.__autoEventButtonData[i].EventFinishTime.SetParent(self.__autoEventButtonData[i])
			self.__autoEventButtonData[i].EventFinishTime.SetText("-")
			self.__autoEventButtonData[i].EventFinishTime.SetPosition(310, 2)
			self.__autoEventButtonData[i].EventFinishTime.Show()

		if len(event_data) < 0:
			return
		for i in xrange(len(event_data)):
			self.__autoEventButtonData[i].SetEvent(ui.__mem_func__(self.__SelectButton),i)
			text = event_data[i][3]+"Hakkýnda Bilgi Al"
			self.__autoEventButtonData[i].SetToolTipText(text, -(len(text)+232), 3)
			text_ = event_data[i][3]
			if len(text_) > 19:
				text_ = text_[0:19]+"."
			self.__autoEventButtonData[i].EventName.SetText(text_)
			self.__autoEventButtonData[i].EventStartTime.SetText(event_data[i][0])
			self.__autoEventButtonData[i].EventFinishTime.SetText(event_data[i][1])
			self.__autoEventButtonData[i].EventName.SetPosition(-28-len(event_data[i][3]), 2)
			self.__autoEventButtonData[i].EventName.SetWindowHorizontalAlignCenter()
			self.__autoEventButtonData[i].EventStartTime.SetPosition(250 - len(event_data[i][0]), 2)
			self.__autoEventButtonData[i].EventFinishTime.SetPosition(310 - len(event_data[i][1]), 2)
		self.event_data = event_data

	def __SelectButton(self, index):
		self.Close()

		if self.event_data[index][2] == "okey":
			if None != self.akiraCardsWindow:
				self.akiraCardsWindow.Open()
				return
		if self.akiraEventInfoWindow.IsOpen():
			self.akiraEventInfoWindow.Close()
		self.akiraEventInfoWindow.Open(self.event_data[index][2], self.event_data[index][3])

	def IsOpen(self):
		return self.IsShow()

	def Open(self, event_data):
		ui.ScriptWindow.Show(self)
		self.SetCenterPosition()
		self.__CreateButton(event_data)

	def Close(self):
		self.Hide()
		return TRUE

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE

class AkiraEventInfoWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.event_list = {"ayisigi" : 50011, "futboltopu" : 50096, "paskalya" : 71150, "okey" : 79505, "kostum" : 41041, "sertifika" : 52701, "kuzeykutusu" : 38057}
		self.text = []

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.ClearDictionary()
		self.text = []

	def LoadWindow(self):
		try:
			ui.PythonScriptLoader().LoadScriptFile(self, "uiscript/akira_eventinfo_window.py")
		except:
			import exception
			exception.Abort("AkiraEventInfoWindow.LoadWindow.LoadObject")
		try:
			self.board = self.GetChild("board")
			self.BlackBoard = self.GetChild("BlackBoard")
			self.TitleName = self.GetChild("TitleName")
			self.GetChild("TitleBar").SetCloseEvent(self.Close)
			self.gridSlowWind = self.GetChild("ItemGrid")
		except:
			import exception
			exception.Abort("AkiraEventInfoWindow.LoadWindow.BindObject")

	def IsOpen(self):
		return self.IsShow()
	
	def Open(self, event_flag, event_name):
		self.TitleName.SetText(event_name+"Hakkýnda Bilgiler")
		self.gridSlowWind.SetItemSlot(0, self.event_list[event_flag])
		text_data = InfoText[event_flag].split("|")
		self.board.SetSize(self.board.GetWidth(),self.board.GetHeight()+len(text_data)*12)
		self.BlackBoard.SetSize(self.board.GetWidth()-28, self.board.GetHeight()-48)
		for i in xrange(len(text_data)):
			text = ui.TextLine()
			text.SetParent(self)
			text.SetText(text_data[i])
			text.SetOutline()
			text.SetFeather(FALSE)
			text.SetPosition(self.board.GetWidth()/2, 110+(2*32)+i*12)
			text.SetHorizontalAlignCenter()
			text.Show()
			self.text.append(text)
		ui.ScriptWindow.Show(self)
		self.SetCenterPosition()
		self.Show()

	def Close(self):
		self.text = []
		self.board.SetSize(300, 125+(2*32))
		self.BlackBoard.SetSize(self.board.GetWidth()-28, self.board.GetHeight()-48)
		self.Hide()
		return TRUE

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE