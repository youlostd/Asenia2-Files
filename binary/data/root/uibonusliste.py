import ui
import snd
import systemSetting
import net
import chat
import app
import locale
import chrmgr
import player
import chr
import game
import background
import uiPhaseCurtain
import interfaceModule
import shop
import uiSwitchBonus

class OptionDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Load()
		self.curtain = uiPhaseCurtain.PhaseCurtain()
		self.curtain.speed = 0.01
		self.curtain.Hide()
		race = net.GetMainActorRace()
		if race != 1000:
			self.bonus0.InsertItem(1, "Max HP")
			self.bonus0.InsertItem(2, "K�l�� Savunmas�")
			self.bonus0.InsertItem(3, "�ift-El Savunma")
			self.bonus0.InsertItem(4, "B��ak Savunmas�")
			self.bonus0.InsertItem(5, "�an Savunmas�")
			self.bonus0.InsertItem(6, "Yelpaze Savunmas�")
			self.bonus0.InsertItem(7, "Oka kar�� dayanakl�l�k")
			self.bonus0.InsertItem(8, "B�y�ye kar�� dayan�kl�l�k")
			self.bonus0.InsertItem(9, "Ya�am Enerjisi")
			self.bonus0.InsertItem(10, "Zeka")
			self.bonus0.InsertItem(11, "G��")
			self.bonus0.InsertItem(12, "�eviklik")
			self.bonus0.InsertItem(13, "Sald�r� de�eri")
			self.bonus0.InsertItem(14, "Ortalama Zarar")
			self.bonus0.InsertItem(15, "Beceri Hasar�")
			self.bonus0.InsertItem(16, "Sald�r� H�z�")
			self.bonus0.InsertItem(17, "Hareket H�z�")
			self.bonus0.InsertItem(18, "B�y� H�z�")
			self.bonus0.InsertItem(19, "HP �retimi")
			self.bonus0.InsertItem(20, "Zehirlenme De�i�imi")
			self.bonus0.InsertItem(21, "Sersemletme �ans�")
			self.bonus0.InsertItem(22, "Kritik Vuru� �ans�")
			self.bonus0.InsertItem(23, "Delici Vuru� �ans�")
			self.bonus0.InsertItem(24, "Yar� insanlara kar�� g��l�")
			self.bonus0.InsertItem(25, "�l�ms�zlere kar�� g��l�")
			self.bonus0.InsertItem(26, "�eytanlara kar�� g��l�")
			self.bonus0.InsertItem(27, "Sersemlik kar��s�nda ba����kl�k")
			self.bonus0.InsertItem(28, "Yava�lama kar��s�nda ba����kl�k")
			self.bonus0.InsertItem(29, "Beden kar��s�nda ataklar�n bloklanmas�")
			self.bonus0.InsertItem(30, "Oklardan korunma �ans�")
			self.bonus0.InsertItem(31, "V�cut darbelerini yans�tma �ans�")
			self.bonus0.InsertItem(32, "Zehire Kar�� Koyma")
			self.bonus0.InsertItem(33, "EXP Bonus �ans�")
			self.bonus0.InsertItem(34, "�ki kat yang d��me �ans�")
			self.bonus0.InsertItem(35, "�ki kat e�ya d��me �ans�")
		else:		
			self.chooseBonus0.SetText("Niciun Bonus Disponibil.")

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
			self.setbonus0			= GetObject("Ok_button")
			self.bonus0			= GetObject("Bonus0_List")
			self.chooseBonus0			= GetObject("ChooseBonus0")
			self.cancel			= GetObject("stop_button")
		except:
			import exception
			exception.Abort("OptionDialog.__Load_BindObject")

	def __Load(self):
		self.__Load_LoadScript("UIscript/boniliste.py")

		self.__Load_BindObject()

		self.SetCenterPosition()

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

		self.setbonus0.SetEvent(ui.__mem_func__(self.__Setbonus0))
		self.cancel.SetEvent(ui.__mem_func__(self.__Cancelbonus0))
		
	def __Cancelbonus0(self):
		self.Hide()
		
	def __Setbonus0(self):
		race = net.GetMainActorRace()
		group = net.GetMainActorSkillGroup()
		getIndex = self.bonus0.GetSelectedItem()
		if getIndex == 1 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 1
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Max. HP")
		elif getIndex == 1 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 1
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Max. HP")
		elif getIndex == 1 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 1
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Max. HP")
		elif getIndex == 1 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 1
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Max. HP")
		elif getIndex == 1 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 1
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Max. HP")
		elif getIndex == 1 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 1
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Max. HP")
		elif getIndex == 1 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 1
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Max. HP")
		elif getIndex == 1 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 1
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Max. HP")
		elif getIndex == 1 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 1
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Max. HP")
		elif getIndex == 1 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 1
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Max. HP")
		elif getIndex == 2 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 29
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus K�l�� Savunmas�")
		elif getIndex == 2 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 29
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus K�l�� Savunmas�")
		elif getIndex == 2 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 29
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus K�l�� Savunmas�")
		elif getIndex == 2 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 29
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus K�l�� Savunmas�")
		elif getIndex == 2 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 29
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus K�l�� Savunmas�")
		elif getIndex == 2 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 29
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus K�l�� Savunmas�")
		elif getIndex == 2 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 29
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus K�l�� Savunmas�")
		elif getIndex == 2 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 29
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus K�l�� Savunmas�")
		elif getIndex == 2 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 29
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus K�l�� Savunmas�")
		elif getIndex == 2 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 29
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus K�l�� Savunmas�")
		elif getIndex == 3 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 30
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus �ift-El Savunma")
		elif getIndex == 3 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 30
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus �ift-El Savunma")
		elif getIndex == 3 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 30
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus �ift-El Savunma")
		elif getIndex == 3 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 30
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus �ift-El Savunma")
		elif getIndex == 3 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 30
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus �ift-El Savunma")
		elif getIndex == 3 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 30
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus �ift-El Savunma")
		elif getIndex == 3 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 30
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus �ift-El Savunma")
		elif getIndex == 3 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 30
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus �ift-El Savunma")
		elif getIndex == 3 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 30
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus �ift-El Savunma")
		elif getIndex == 3 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 30
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus �ift-El Savunma")
		elif getIndex == 4 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 31
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus B��ak Savunmas�")
		elif getIndex == 4 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 31
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus B��ak Savunmas�")
		elif getIndex == 4 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 31
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus B��ak Savunmas�")
		elif getIndex == 4 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 31
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus B��ak Savunmas�")
		elif getIndex == 4 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 31
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus B��ak Savunmas�")
		elif getIndex == 4 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 31
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus B��ak Savunmas�")
		elif getIndex == 4 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 31
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus B��ak Savunmas�")
		elif getIndex == 4 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 31
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus B��ak Savunmas�")
		elif getIndex == 4 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 31
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus B��ak Savunmas�")
		elif getIndex == 4 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 31
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus B��ak Savunmas�")
		elif getIndex == 5 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 32
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus �an Savunmas�")
		elif getIndex == 5 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 32
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus �an Savunmas�")
		elif getIndex == 5 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 32
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus �an Savunmas�")
		elif getIndex == 5 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 32
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus �an Savunmas�")
		elif getIndex == 5 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 32
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus �an Savunmas�")
		elif getIndex == 5 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 32
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus �an Savunmas�")
		elif getIndex == 5 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 32
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus �an Savunmas�")
		elif getIndex == 5 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 32
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus �an Savunmas�")
		elif getIndex == 5 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 32
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus �an Savunmas�")
		elif getIndex == 5 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 32
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus �an Savunmas�")
		elif getIndex == 6 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 33
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Yelpaze Savunmas�")
		elif getIndex == 6 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 33
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Yelpaze Savunmas�")
		elif getIndex == 6 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 33
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Yelpaze Savunmas�")
		elif getIndex == 6 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 33
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Yelpaze Savunmas�")
		elif getIndex == 6 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 33
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Yelpaze Savunmas�")
		elif getIndex == 6 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 33
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Yelpaze Savunmas�")
		elif getIndex == 6 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 33
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Yelpaze Savunmas�")
		elif getIndex == 6 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 33
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Yelpaze Savunmas�")
		elif getIndex == 6 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 33
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Yelpaze Savunmas�")
		elif getIndex == 6 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 33
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Yelpaze Savunmas�")
		elif getIndex == 7 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 34
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Oka kar�� dayanakl�l�k")
		elif getIndex == 7 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 34
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Oka kar�� dayanakl�l�k")
		elif getIndex == 7 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 34
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Oka kar�� dayanakl�l�k")
		elif getIndex == 7 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 34
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Oka kar�� dayanakl�l�k")
		elif getIndex == 7 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 34
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Oka kar�� dayanakl�l�k")
		elif getIndex == 7 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 34
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Oka kar�� dayanakl�l�k")
		elif getIndex == 7 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 34
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Oka kar�� dayanakl�l�k")
		elif getIndex == 7 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 34
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Oka kar�� dayanakl�l�k")
		elif getIndex == 7 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 34
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Oka kar�� dayanakl�l�k")
		elif getIndex == 7 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 34
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Oka kar�� dayanakl�l�k")
		elif getIndex == 8 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 37
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus B�y�ye kar�� dayan�kl�l�k")
		elif getIndex == 8 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 37
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus B�y�ye kar�� dayan�kl�l�k")
		elif getIndex == 8 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 37
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus B�y�ye kar�� dayan�kl�l�k")
		elif getIndex == 8 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 37
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus B�y�ye kar�� dayan�kl�l�k")
		elif getIndex == 8 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 37
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus B�y�ye kar�� dayan�kl�l�k")
		elif getIndex == 8 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 37
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus B�y�ye kar�� dayan�kl�l�k")
		elif getIndex == 8 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 37
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus B�y�ye kar�� dayan�kl�l�k")
		elif getIndex == 8 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 37
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus B�y�ye kar�� dayan�kl�l�k")
		elif getIndex == 8 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 37
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus B�y�ye kar�� dayan�kl�l�k")
		elif getIndex == 8 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 37
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus B�y�ye kar�� dayan�kl�l�k")
		elif getIndex == 9 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 3
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Ya�am Enerjisi")
		elif getIndex == 9 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 3
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Ya�am Enerjisi")
		elif getIndex == 9 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 3
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Ya�am Enerjisi")
		elif getIndex == 9 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 3
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Ya�am Enerjisi")
		elif getIndex == 9 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 3
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Ya�am Enerjisi")
		elif getIndex == 9 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 3
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Ya�am Enerjisi")
		elif getIndex == 9 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 3
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Ya�am Enerjisi")
		elif getIndex == 9 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 3
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Ya�am Enerjisi")
		elif getIndex == 9 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 3
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Ya�am Enerjisi")
		elif getIndex == 9 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 3
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Ya�am Enerjisi")
		elif getIndex == 10 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 4
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Zeka")
		elif getIndex == 10 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 4
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Zeka")
		elif getIndex == 10 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 4
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Zeka")
		elif getIndex == 10 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 4
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Zeka")
		elif getIndex == 10 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 4
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Zeka")
		elif getIndex == 10 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 4
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Zeka")
		elif getIndex == 10 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 4
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Zeka")
		elif getIndex == 10 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 4
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Zeka")
		elif getIndex == 10 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 4
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Zeka")
		elif getIndex == 10 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 4
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Zeka")
		elif getIndex == 11 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 5
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus G��")
		elif getIndex == 11 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 5
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus G��")
		elif getIndex == 11 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 5
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus G��")
		elif getIndex == 11 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 5
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus G��")
		elif getIndex == 11 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 5
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus G��")
		elif getIndex == 11 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 5
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus G��")
		elif getIndex == 11 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 5
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus G��")
		elif getIndex == 11 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 5
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus G��")
		elif getIndex == 11 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 5
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus G��")
		elif getIndex == 11 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 5
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus G��")
		elif getIndex == 12 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 6
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus �eviklik")
		elif getIndex == 12 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 6
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus �eviklik")
		elif getIndex == 12 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 6
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus �eviklik")
		elif getIndex == 12 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 6
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus �eviklik")
		elif getIndex == 12 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 6
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus �eviklik")
		elif getIndex == 12 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 6
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus �eviklik")
		elif getIndex == 12 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 6
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus �eviklik")
		elif getIndex == 12 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 6
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus �eviklik")
		elif getIndex == 12 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 6
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus �eviklik")
		elif getIndex == 12 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 6
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus �eviklik")
		elif getIndex == 13 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 53
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Sald�r� de�eri")
		elif getIndex == 13 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 53
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Sald�r� de�eri")
		elif getIndex == 13 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 53
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Sald�r� de�eri")
		elif getIndex == 13 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 53
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Sald�r� de�eri")
		elif getIndex == 13 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 53
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Sald�r� de�eri")
		elif getIndex == 13 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 53
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Sald�r� de�eri")
		elif getIndex == 13 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 53
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Sald�r� de�eri")
		elif getIndex == 13 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 53
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Sald�r� de�eri")
		elif getIndex == 13 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 53
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Sald�r� de�eri")
		elif getIndex == 13 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 53
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Sald�r� de�eri")
		elif getIndex == 14 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 72
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Ortalama Zarar")
		elif getIndex == 14 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 72
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Ortalama Zarar")
		elif getIndex == 14 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 72
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Ortalama Zarar")
		elif getIndex == 14 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 72
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Ortalama Zarar")
		elif getIndex == 14 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 72
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Ortalama Zarar")
		elif getIndex == 14 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 72
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Ortalama Zarar")
		elif getIndex == 14 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 72
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Ortalama Zarar")
		elif getIndex == 14 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 72
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Ortalama Zarar")
		elif getIndex == 14 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 72
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Ortalama Zarar")
		elif getIndex == 14 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 72
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Ortalama Zarar")
		elif getIndex == 15 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 71
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Beceri Hasar�")
		elif getIndex == 15 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 71
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Beceri Hasar�")
		elif getIndex == 15 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 71
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Beceri Hasar�")
		elif getIndex == 15 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 71
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Beceri Hasar�")
		elif getIndex == 15 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 71
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Beceri Hasar�")
		elif getIndex == 15 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 71
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Beceri Hasar�")
		elif getIndex == 15 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 71
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Beceri Hasar�")
		elif getIndex == 15 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 71
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Beceri Hasar�")
		elif getIndex == 15 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 71
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Beceri Hasar�")
		elif getIndex == 15 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 71
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Beceri Hasar�")
		elif getIndex == 16 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 7
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Sald�r� H�z�")
		elif getIndex == 16 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 7
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Sald�r� H�z�")
		elif getIndex == 16 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 7
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Sald�r� H�z�")
		elif getIndex == 16 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 7
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Sald�r� H�z�")
		elif getIndex == 16 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 7
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Sald�r� H�z�")
		elif getIndex == 16 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 7
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Sald�r� H�z�")
		elif getIndex == 16 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 7
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Sald�r� H�z�")
		elif getIndex == 16 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 7
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Sald�r� H�z�")
		elif getIndex == 16 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 7
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Sald�r� H�z�")
		elif getIndex == 16 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 7
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Sald�r� H�z�")
		elif getIndex == 17 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 8
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Hareket H�z�")
		elif getIndex == 17 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 8
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Hareket H�z�")
		elif getIndex == 17 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 8
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Hareket H�z�")
		elif getIndex == 17 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 8
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Hareket H�z�")
		elif getIndex == 17 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 8
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Hareket H�z�")
		elif getIndex == 17 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 8
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Hareket H�z�")
		elif getIndex == 17 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 8
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Hareket H�z�")
		elif getIndex == 17 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 8
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Hareket H�z�")
		elif getIndex == 17 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 8
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Hareket H�z�")
		elif getIndex == 17 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 8
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Hareket H�z�")
		elif getIndex == 18 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 9
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus B�y� H�z�")
		elif getIndex == 18 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 9
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus B�y� H�z�")
		elif getIndex == 18 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 9
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus B�y� H�z�")
		elif getIndex == 18 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 9
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus B�y� H�z�")
		elif getIndex == 18 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 9
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus B�y� H�z�")
		elif getIndex == 18 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 9
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus B�y� H�z�")
		elif getIndex == 18 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 9
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus B�y� H�z�")
		elif getIndex == 18 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 9
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus B�y� H�z�")
		elif getIndex == 18 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 9
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus B�y� H�z�")
		elif getIndex == 18 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 9
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus B�y� H�z�")
		elif getIndex == 19 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 10
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus HP �retimi")
		elif getIndex == 19 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 10
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus HP �retimi")
		elif getIndex == 19 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 10
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus HP �retimi")
		elif getIndex == 19 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 10
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus HP �retimi")
		elif getIndex == 19 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 10
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus HP �retimi")
		elif getIndex == 19 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 10
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus HP �retimi")
		elif getIndex == 19 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 10
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus HP �retimi")
		elif getIndex == 19 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 10
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus HP �retimi")
		elif getIndex == 19 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 10
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus HP �retimi")
		elif getIndex == 19 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 10
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus HP �retimi")
		elif getIndex == 20 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 12
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Zehirlenme De�i�imi")
		elif getIndex == 20 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 12
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Zehirlenme De�i�imi")
		elif getIndex == 20 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 12
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Zehirlenme De�i�imi")
		elif getIndex == 20 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 12
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Zehirlenme De�i�imi")
		elif getIndex == 20 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 12
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Zehirlenme De�i�imi")
		elif getIndex == 20 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 12
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Zehirlenme De�i�imi")
		elif getIndex == 20 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 12
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Zehirlenme De�i�imi")
		elif getIndex == 20 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 12
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Zehirlenme De�i�imi")
		elif getIndex == 20 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 12
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Zehirlenme De�i�imi")
		elif getIndex == 20 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 12
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Zehirlenme De�i�imi")
		elif getIndex == 21 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 13
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Sersemletme �ans�")
		elif getIndex == 21 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 13
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Sersemletme �ans�")
		elif getIndex == 21 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 13
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Sersemletme �ans�")
		elif getIndex == 21 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 13
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Sersemletme �ans�")
		elif getIndex == 21 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 13
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Sersemletme �ans�")
		elif getIndex == 21 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 13
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Sersemletme �ans�")
		elif getIndex == 21 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 13
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Sersemletme �ans�")
		elif getIndex == 21 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 13
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Sersemletme �ans�")
		elif getIndex == 21 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 13
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Sersemletme �ans�")
		elif getIndex == 21 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 13
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Sersemletme �ans�")
		elif getIndex == 22 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 15
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Kritik Vuru� �ans�")
		elif getIndex == 22 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 15
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Kritik Vuru� �ans�")
		elif getIndex == 22 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 15
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Kritik Vuru� �ans�")
		elif getIndex == 22 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 15
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Kritik Vuru� �ans�")
		elif getIndex == 22 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 15
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Kritik Vuru� �ans�")
		elif getIndex == 22 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 15
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Kritik Vuru� �ans�")
		elif getIndex == 22 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 15
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Kritik Vuru� �ans�")
		elif getIndex == 22 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 15
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Kritik Vuru� �ans�")
		elif getIndex == 22 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 15
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Kritik Vuru� �ans�")
		elif getIndex == 22 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 15
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Kritik Vuru� �ans�")
		elif getIndex == 23 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 16
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Delici Vuru� �ans�")
		elif getIndex == 23 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 16
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Delici Vuru� �ans�")
		elif getIndex == 23 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 16
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Delici Vuru� �ans�")
		elif getIndex == 23 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 16
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Delici Vuru� �ans�")
		elif getIndex == 23 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 16
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Delici Vuru� �ans�")
		elif getIndex == 23 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 16
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Delici Vuru� �ans�")
		elif getIndex == 23 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 16
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Delici Vuru� �ans�")
		elif getIndex == 23 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 16
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Delici Vuru� �ans�")
		elif getIndex == 23 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 16
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Delici Vuru� �ans�")
		elif getIndex == 23 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 16
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Delici Vuru� �ans�")
		elif getIndex == 24 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 17
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Yar� insanlara kar�� g��l�")
		elif getIndex == 24 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 17
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Yar� insanlara kar�� g��l�")
		elif getIndex == 24 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 17
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Yar� insanlara kar�� g��l�")
		elif getIndex == 24 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 17
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Yar� insanlara kar�� g��l�")
		elif getIndex == 24 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 17
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Yar� insanlara kar�� g��l�")
		elif getIndex == 24 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 17
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Yar� insanlara kar�� g��l�")
		elif getIndex == 24 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 17
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Yar� insanlara kar�� g��l�")
		elif getIndex == 24 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 17
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Yar� insanlara kar�� g��l�")
		elif getIndex == 24 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 17
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Yar� insanlara kar�� g��l�")
		elif getIndex == 24 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 17
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Yar� insanlara kar�� g��l�")
		elif getIndex == 25 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 21
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus �l�ms�zlere kar�� g��l�")
		elif getIndex == 25 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 21
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus �l�ms�zlere kar�� g��l�")
		elif getIndex == 25 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 21
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus �l�ms�zlere kar�� g��l�")
		elif getIndex == 25 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 21
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus �l�ms�zlere kar�� g��l�")
		elif getIndex == 25 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 21
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus �l�ms�zlere kar�� g��l�")
		elif getIndex == 25 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 21
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus �l�ms�zlere kar�� g��l�")
		elif getIndex == 25 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 21
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus �l�ms�zlere kar�� g��l�")
		elif getIndex == 25 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 21
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus �l�ms�zlere kar�� g��l�")
		elif getIndex == 25 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 21
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus �l�ms�zlere kar�� g��l�")
		elif getIndex == 25 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 21
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus �l�ms�zlere kar�� g��l�")
		elif getIndex == 26 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 22
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus �eytanlara kar�� g��l�")
		elif getIndex == 26 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 22
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus �eytanlara kar�� g��l�")
		elif getIndex == 26 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 22
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus �eytanlara kar�� g��l�")
		elif getIndex == 26 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 22
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus �eytanlara kar�� g��l�")
		elif getIndex == 26 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 22
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus �eytanlara kar�� g��l�")
		elif getIndex == 26 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 22
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus �eytanlara kar�� g��l�")
		elif getIndex == 26 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 22
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus �eytanlara kar�� g��l�")
		elif getIndex == 26 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 22
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus �eytanlara kar�� g��l�")
		elif getIndex == 26 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 22
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus �eytanlara kar�� g��l�")
		elif getIndex == 26 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 22
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus �eytanlara kar�� g��l�")
		elif getIndex == 27 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 48
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Sersemlik kar��s�nda ba����kl�k")
		elif getIndex == 27 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 48
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Sersemlik kar��s�nda ba����kl�k")
		elif getIndex == 27 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 48
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Sersemlik kar��s�nda ba����kl�k")
		elif getIndex == 27 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 48
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Sersemlik kar��s�nda ba����kl�k")
		elif getIndex == 27 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 48
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Sersemlik kar��s�nda ba����kl�k")
		elif getIndex == 27 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 48
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Sersemlik kar��s�nda ba����kl�k")
		elif getIndex == 27 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 48
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Sersemlik kar��s�nda ba����kl�k")
		elif getIndex == 27 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 48
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Sersemlik kar��s�nda ba����kl�k")
		elif getIndex == 27 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 48
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Sersemlik kar��s�nda ba����kl�k")
		elif getIndex == 27 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 48
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Sersemlik kar��s�nda ba����kl�k")
		elif getIndex == 28 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 49
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Yava�lama kar��s�nda ba����kl�k")
		elif getIndex == 28 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 49
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Yava�lama kar��s�nda ba����kl�k")
		elif getIndex == 28 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 49
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Yava�lama kar��s�nda ba����kl�k")
		elif getIndex == 28 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 49
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Yava�lama kar��s�nda ba����kl�k")
		elif getIndex == 28 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 49
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Yava�lama kar��s�nda ba����kl�k")
		elif getIndex == 28 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 49
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Yava�lama kar��s�nda ba����kl�k")
		elif getIndex == 28 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 49
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Yava�lama kar��s�nda ba����kl�k")
		elif getIndex == 28 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 49
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Yava�lama kar��s�nda ba����kl�k")
		elif getIndex == 28 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 49
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Yava�lama kar��s�nda ba����kl�k")
		elif getIndex == 28 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 49
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Yava�lama kar��s�nda ba����kl�k")
		elif getIndex == 29 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 27
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Beden kar��s�nda ataklar�n bloklanmas�")
		elif getIndex == 29 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 27
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Beden kar��s�nda ataklar�n bloklanmas�")
		elif getIndex == 29 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 27
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Beden kar��s�nda ataklar�n bloklanmas�")
		elif getIndex == 29 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 27
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Beden kar��s�nda ataklar�n bloklanmas�")
		elif getIndex == 29 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 27
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Beden kar��s�nda ataklar�n bloklanmas�")
		elif getIndex == 29 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 27
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Beden kar��s�nda ataklar�n bloklanmas�")
		elif getIndex == 29 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 27
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Beden kar��s�nda ataklar�n bloklanmas�")
		elif getIndex == 29 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 27
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Beden kar��s�nda ataklar�n bloklanmas�")
		elif getIndex == 29 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 27
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Beden kar��s�nda ataklar�n bloklanmas�")
		elif getIndex == 29 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 27
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Beden kar��s�nda ataklar�n bloklanmas�")
		elif getIndex == 30 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 28
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Oklardan korunma �ans�")
		elif getIndex == 30 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 28
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Oklardan korunma �ans�")
		elif getIndex == 30 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 28
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Oklardan korunma �ans�")
		elif getIndex == 30 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 28
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Oklardan korunma �ans�")
		elif getIndex == 30 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 28
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Oklardan korunma �ans�")
		elif getIndex == 30 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 28
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Oklardan korunma �ans�")
		elif getIndex == 30 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 28
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Oklardan korunma �ans�")
		elif getIndex == 30 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 28
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Oklardan korunma �ans�")
		elif getIndex == 30 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 28
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Oklardan korunma �ans�")
		elif getIndex == 30 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 28
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Oklardan korunma �ans�")
		elif getIndex == 31 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 39
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus V�cut darbelerini yans�tma �ans�")
		elif getIndex == 31 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 39
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus V�cut darbelerini yans�tma �ans�")
		elif getIndex == 31 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 39
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus V�cut darbelerini yans�tma �ans�")
		elif getIndex == 31 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 39
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus V�cut darbelerini yans�tma �ans�")
		elif getIndex == 31 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 39
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus V�cut darbelerini yans�tma �ans�")
		elif getIndex == 31 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 39
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus V�cut darbelerini yans�tma �ans�")
		elif getIndex == 31 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 39
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus V�cut darbelerini yans�tma �ans�")
		elif getIndex == 31 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 39
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus V�cut darbelerini yans�tma �ans�")
		elif getIndex == 31 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 39
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus V�cut darbelerini yans�tma �ans�")
		elif getIndex == 31 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 39
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus V�cut darbelerini yans�tma �ans�")
		elif getIndex == 32 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 41
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus Zehire Kar�� Koyma")
		elif getIndex == 32 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 41
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus Zehire Kar�� Koyma")
		elif getIndex == 32 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 41
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus Zehire Kar�� Koyma")
		elif getIndex == 32 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 41
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus Zehire Kar�� Koyma")
		elif getIndex == 32 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 41
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus Zehire Kar�� Koyma")
		elif getIndex == 32 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 41
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus Zehire Kar�� Koyma")
		elif getIndex == 32 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 41
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus Zehire Kar�� Koyma")
		elif getIndex == 32 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 41
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus Zehire Kar�� Koyma")
		elif getIndex == 32 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 41
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus Zehire Kar�� Koyma")
		elif getIndex == 32 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 41
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus Zehire Kar�� Koyma")
		elif getIndex == 33 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 43
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus EXP Bonus �ans�")
		elif getIndex == 33 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 43
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus EXP Bonus �ans�")
		elif getIndex == 33 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 43
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus EXP Bonus �ans�")
		elif getIndex == 33 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 43
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus EXP Bonus �ans�")
		elif getIndex == 33 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 43
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus EXP Bonus �ans�")
		elif getIndex == 33 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 43
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus EXP Bonus �ans�")
		elif getIndex == 33 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 43
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus EXP Bonus �ans�")
		elif getIndex == 33 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 43
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus EXP Bonus �ans�")
		elif getIndex == 33 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 43
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus EXP Bonus �ans�")
		elif getIndex == 33 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 43
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus EXP Bonus �ans�")
		elif getIndex == 34 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 44
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus �ki kat yang d��me �ans�")
		elif getIndex == 34 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 44
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus �ki kat yang d��me �ans�")
		elif getIndex == 34 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 44
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus �ki kat yang d��me �ans�")
		elif getIndex == 34 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 44
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus �ki kat yang d��me �ans�")
		elif getIndex == 34 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 44
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus �ki kat yang d��me �ans�")
		elif getIndex == 34 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 44
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus �ki kat yang d��me �ans�")
		elif getIndex == 34 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 44
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus �ki kat yang d��me �ans�")
		elif getIndex == 34 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 44
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus �ki kat yang d��me �ans�")
		elif getIndex == 34 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 44
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus �ki kat yang d��me �ans�")
		elif getIndex == 34 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 44
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus �ki kat yang d��me �ans�")
		elif getIndex == 35 and uiSwitchBonus.Bonus0 == 0 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 45
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "1.Bonus �ki kat e�ya d��me �ans�")
		elif getIndex == 35 and uiSwitchBonus.Bonus1 == 0 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Bonus1 = 45
			uiSwitchBonus.Presswish1 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "2.Bonus �ki kat e�ya d��me �ans�")
		elif getIndex == 35 and uiSwitchBonus.Bonus2 == 0 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Bonus2 = 45
			uiSwitchBonus.Presswish2 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "3.Bonus �ki kat e�ya d��me �ans�")
		elif getIndex == 35 and uiSwitchBonus.Bonus3 == 0 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Bonus3 = 45
			uiSwitchBonus.Presswish3 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "4.Bonus �ki kat e�ya d��me �ans�")
		elif getIndex == 35 and uiSwitchBonus.Bonus4 == 0 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Bonus4 = 45
			uiSwitchBonus.Presswish4 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "5.Bonus �ki kat e�ya d��me �ans�")
		elif getIndex == 35 and uiSwitchBonus.Overwrite0 == 1 and uiSwitchBonus.Presswish0 == 1:
			uiSwitchBonus.Bonus0 = 45
			uiSwitchBonus.Presswish0 = 0
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 1.Bonus �ki kat e�ya d��me �ans�")
		elif getIndex == 35 and uiSwitchBonus.Overwrite1 == 1 and uiSwitchBonus.Presswish1 == 1:
			uiSwitchBonus.Presswish1 = 0
			uiSwitchBonus.Bonus1 = 45
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 2.Bonus �ki kat e�ya d��me �ans�")
		elif getIndex == 35 and uiSwitchBonus.Overwrite2 == 1 and uiSwitchBonus.Presswish2 == 1:
			uiSwitchBonus.Presswish2 = 0
			uiSwitchBonus.Bonus2 = 45
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 3.Bonus �ki kat e�ya d��me �ans�")
		elif getIndex == 35 and uiSwitchBonus.Overwrite3 == 1 and uiSwitchBonus.Presswish3 == 1:
			uiSwitchBonus.Presswish3 = 0
			uiSwitchBonus.Bonus3 = 45
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 4.Bonus �ki kat e�ya d��me �ans�")
		elif getIndex == 35 and uiSwitchBonus.Overwrite4 == 1 and uiSwitchBonus.Presswish4 == 1:
			uiSwitchBonus.Presswish4 = 0
			uiSwitchBonus.Bonus4 = 45
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Yeni 5.Bonus �ki kat e�ya d��me �ans�")
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Hi�bir bonus se�ili de�il.")		
		self.Hide()
	
	def OnCloseInputDialog(self):
		self.inputDialog.Close()
		self.inputDialog = None
		return TRUE

	def OnCloseQuestionDialog(self):
		self.questionDialog.Close()
		self.questionDialog = None
		return TRUE

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE

	def Show(self):
		ui.ScriptWindow.Show(self)

	def Close(self):
		self.Hide()
