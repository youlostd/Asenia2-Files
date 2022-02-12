import ui
import chat
import app
import player
import snd
import item
import net
import game

class BonusBoardDialog(ui.ScriptWindow):
	MaxBoni = { "1": 16000, "2": 320, "3": 32, "4": 32, "5": 32, "6": 32, "7": 16, "9": 40, "10": 60, "11": 60, "12": 16, "13": 24, "14": 16, "15": 30, "16": 30, "17": 50, "18": 100, "19": 100, "20": 100, "21": 100, "22": 100, "23": 20, "24": 40, "27": 15, "28": 30, "29": 72, "30": 72, "31": 72, "32": 72, "33": 72, "34": 72, "35": 60, "36": 60, "37": 40, "38": 60, "39": 20, "41": 10, "43": 60, "44": 60, "45": 40, "48": 1, "53": 50 }
	BonusDict = ["PvP Bonus", "PvM Bonus", "Di�er"]
	BonusIDListe = [["", 0, 0],["Maks HP", 1, 0],["Maks SP", 2, 0],["HP", 3, 0],["Zeka", 4, 0],["G��", 5, 0],["�eviklik", 6, 0],["Sald�r� H�z�", 7, 0],["Hareket H�z�", 8, 0],["B�y� H�z�", 9, 0],["HP �retimi", 10, 32],["SP �retimi", 11, 33],["Zehirleme De�i�imi", 12, 37],["Sersemletme �ans�", 13, 38],["Yava�latma �ans�", 14, 39],["Kritik Vuru� �ans�", 15, 40],["Delici Vuru� �ans�", 16, 41],["Yar� �nsanlara Kar�� G��l�", 17, 43],["Hayvanlara Kar�� G��l�", 18, 44],["Orklara Kar�� G��l�", 19, 45],["Mistiklere Kar�� G��l�", 20, 46],["�l�ms�zlere Kar�� G��l�", 21, 47],["�eytanlara Kar�� G��l�", 22, 48],["HP Emilimi", 23, 63],["SP Emilimi", 24, 64],["SP �alma", 25, 65],["Vuru� �le SP �alma", 26, 66],["Beden Kar��s� Ataklar� Bloklama", 27, 67],["Oklardan Korunma �ans�", 28, 68],["K�l�� Savunmas�", 29, 69],["�ift El Savunmas�", 30, 70],["B��ak Savunmas�", 31, 71],["�an Savunmas�", 32, 72],["Yelpaze Savunmas�", 33, 73],["Okdan Korunma �ans�", 34, 74],["Ate�e Kar�� Dayan�kl�l�k", 35, 75],["�im�e�e Kar�� Dayan�kl�l�k", 36, 76],["B�y�ye Kar�� Dayan�kl�l�k", 37, 77],["R�zgara Kar�� Dayan�kl�l�k", 38, 78],["V�cut Darbelerini Yans�tma", 39, 79],["Lanet Yans�tmas�", 40, 80],["Zehire Kar�� Koyma", 41, 81],["SP Y�kselmesi De�i�imi", 42, 82],["Exp-Bonus", 43, 83],["Yang-Drop", 44, 84],["Item-Drop", 45, 85],["�ksir Etkisi Y�kseltme", 46, 86],["HP Y�kselmesi De�i�ti", 47, 87],["Sersemlik Kar��s�nda Ba����kl�k", 48, 88],["Yava�l�k Kar��s�nda Ba����kl�k", 49, 89],["Yere D��me Kar��s�nda Ba����kl�k", 50, 90],["APPLY_SKILL", 51, 0],["Yay Menzili", 52, 95],["Sald�r� De�eri", 53, 0],["Savunma", 54, 96],["B�y�l� Sald�r� De�eri", 55, 97],["B�y� Savunmas�", 56, 98],["", 57, 0],["Maks. Dayan�kl�k", 58, 0],["Sava���lara Kar�� G��l�", 59, 54],["Ninjalara Kar�� G��l�", 60, 55],["Suralara Kar�� G��l�", 61, 56],["�amanlara Kar�� G��l�", 62, 57],["Canavarlara Kar�� G��l�", 63, 53],["Sald�r�", 64, 114],["Savunma", 65, 115],["Itemshop Exp-Bonus", 66, 116],["Itemshop Item-Bonus", 67, 117],["Itemshop Yang-Bonus", 68, 118],["APPLY_MAX_HP_PCT", 69, 119],["APPLY_MAX_SP_PCT", 70, 120],["Beceri Hasar�", 71, 121],["Ortalama Hasar", 72, 122],["Beceri Hasar�na Direni�", 73, 123],["Ortalama Hasara Direni�", 74, 124],["", 75, 0],["iCafe EXP-Bonus", 76, 125],["iCafe Item-Bonus", 77, 126],["Sava��� Sald�r�lar�na Savunma", 78, 59],["Ninja Sald�r�lar�na Savunma", 79, 60],["Sura Sald�r�lar�na Savunma", 80, 61],["�aman Sald�r�lar�na Savunma", 81, 62]]
	SpecialBoni = { 1: "Norm.State", 2: "Norm.State", 3: "Norm.State", 4: "Norm.State", 5: "Norm.State", 6: "Norm.State", 55: "Norm.State", 56: "Norm.State", 58: "Norm.State" }
	PvPOffenseBoni = ["Yar� �nsanlara Kar�� G��l�", "Kritik Vuru� �ans�", "Delici Vuru� �ans�", "Ortalama Hasar", "Beceri Hasar�", "HP", "Zeka", "G��", "�eviklik", "B�y� H�z�"]
	PvPDefenseBoni = ["K�l�� Savunmas�", "�ift El Savunmas�", "B��ak Savunmas�", "�an Savunmas�", "Yelpaze Savunmas�", "Okdan Korunma �ans�", "Oklardan Korunma �ans�", "B�y�ye Kar�� Dayan�kl�l�k", "Beden Kar��s� Ataklar� Bloklama", "Sersemlik Kar��s�nda Ba����kl�k"]
	PvMOffenseBoni = ["Canavarlara Kar�� G��l�", "�eytanlara Kar�� G��l�", "�l�ms�zlere Kar�� G��l�", "Hayvanlara Kar�� G��l�", "Orklara Kar�� G��l�", "Mistiklere Kar�� G��l�", "Sersemletme �ans�", "Zehirleme De�i�imi", "Sald�r� H�z�", "Sald�r� De�eri"]
	PvMDefenseBoni = ["Maks HP", "Maks SP", "Beden Kar��s� Ataklar� Bloklama", "HP �retimi", "SP �retimi", "HP Emilimi", "SP Emilimi", "Exp-Bonus", "Yang-Drop", "Item-Drop"]
	LeftoversOffenseBoni = ["Sava���lara Kar�� G��l�", "Ninjalara Kar�� G��l�", "Suralara Kar�� G��l�", "�amanlara Kar�� G��l�"]
	LeftoversDefenseBoni = ["Sava��� Sald�r�lar�na Savunma", "Ninja Sald�r�lar�na Savunma", "Sura Sald�r�lar�na Savunma", "�aman Sald�r�lar�na Savunma", ]

	BonusList = []
	UI = []
	
	TestSystem = 0
	ProcessTimeStamp = 0
	
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.LoadUI()
		game.BPisLoaded = 1
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		self.Board.Hide()
		game.BPisLoaded = 0

	def LoadUI(self):
		self.Board = ui.BoardWithTitleBar()
		self.Board.SetSize(343, 418)
		self.Board.SetCenterPosition()
		self.Board.AddFlag("movable")
		self.Board.AddFlag("float")
		self.Board.SetTitleName("Efsun Tablosu")
		self.Board.SetCloseEvent(self.__del__)
		self.Board.Show()
		
		Vertical = ui.Line()
		Vertical.SetParent(self.Board)
		Vertical.SetPosition(23, 60)
		Vertical.SetSize(298, 0)
		Vertical.SetColor(0xff777777)
		Vertical.Show()
		self.UI.append(Vertical)
		
		x = 25
		for i in xrange(3):
			ChangeBonusDict = ui.Button()
			ChangeBonusDict.SetParent(self.Board)
			ChangeBonusDict.SetUpVisual("d:/ymir work/ui/public/large_button_01.sub")
			ChangeBonusDict.SetOverVisual("d:/ymir work/ui/public/large_button_02.sub")
			ChangeBonusDict.SetDownVisual("d:/ymir work/ui/public/large_button_03.sub")
			ChangeBonusDict.SetText(self.BonusDict[i])
			ChangeBonusDict.SetPosition(x, 380)
			ChangeBonusDict.SetEvent(lambda arg = ChangeBonusDict.GetText(): self.ChangeBonusDict(arg))
			ChangeBonusDict.Show()
			x += 97
			self.UI.append(ChangeBonusDict)
			
		x = 50
		Type = ["Sald�r�", "Defans"]
		for i in xrange(2):
			BonusDescription = ui.TextLine()
			BonusDescription.SetParent(self.Board)
			BonusDescription.SetPosition(x, 35)
			BonusDescription.SetText(str(Type[i]))
			BonusDescription.SetFontColor(4.0, 0.83, 0)
			BonusDescription.Show()			
			x += 150
			self.UI.append(BonusDescription)

		self.SetBoni(self.BonusDict[0])
		self.dict = self.BonusDict[0]
		
	def SetBoni(self, type):
		Offense = [[25, 70], [25, 100], [25, 130], [25, 160], [25, 190], [25, 220], [25, 250], [25, 280], [25, 310], [25, 340]]
		Defense = [[170, 70], [170, 100], [170, 130], [170, 160], [170, 190], [170, 220], [170, 250], [170, 280], [170, 310], [170, 340]]
		for bonus in self.BonusIDListe:
			if type == self.BonusDict[0]:
				self.CheckBonus(bonus, self.PvPOffenseBoni, Offense)
				self.CheckBonus(bonus, self.PvPDefenseBoni, Defense)
			elif type == self.BonusDict[1]:
				self.CheckBonus(bonus, self.PvMOffenseBoni, Offense)
				self.CheckBonus(bonus, self.PvMDefenseBoni, Defense)
			elif type == self.BonusDict[2]:
				self.CheckBonus(bonus, self.LeftoversOffenseBoni, Offense)
				self.CheckBonus(bonus, self.LeftoversDefenseBoni, Defense)
			else:
				return
				
	def CheckBonus(self, bonus, bonuslist, offset):
		for boni in bonuslist:
			if bonus[0] == boni:
				try:
					Index = bonuslist.index(boni)
					BonusDescription = ui.TextLine()
					BonusDescription.SetParent(self.Board)
					BonusDescription.SetPosition(offset[Index][0], offset[Index][1])
					BonusDescription.SetText(str(bonus[0]))
					BonusDescription.Show()
					
					BonusSlotBar = ui.SlotBar()
					BonusSlotBar.SetParent(self.Board)
					BonusSlotBar.SetSize(125, 15)
					BonusSlotBar.SetPosition(offset[Index][0], offset[Index][1] + 15)
					BonusSlotBar.Show()
					
					BonusAttrLine = ui.TextLine()
					BonusAttrLine.SetParent(self.Board)
					BonusAttrLine.SetPosition(offset[Index][0] + 5, offset[Index][1] + 15)
					
					try:
						Type = self.SpecialBoni[bonus[1]]
						Attribute = self.EquipAttribute(bonus)
					except:
						Attribute = player.GetStatus(int(bonus[2]))
					if self.TestSystem != 1:
						BonusAttrLine.SetText(str(Attribute))
						try:
							if int(Attribute) >= int(self.MaxBoni[str(bonus[1])]):
								BonusAttrLine.SetFontColor(1.0, 0.63, 0)
							else:
								BonusAttrLine.SetFontColor(1, 1, 1)
						except:
							BonusAttrLine.SetFontColor(1, 1, 1)
					else:
						BonusAttrLine.SetText("Test system is active")
						BonusAttrLine.SetFontColor(0.1, 0.7, 1.0)
					
					BonusAttrLine.Show()
					self.BonusList.append([BonusDescription, BonusAttrLine, BonusSlotBar])
				except:
					pass		
				
	def EquipAttribute(self, bonus):
		value = 0
		for slot in xrange(90, 101):
			for attr in xrange(0, 7):
				attr, val = player.GetItemAttribute(slot, attr)
				if int(attr) == bonus[1]:
					value += int(val)
		return int(value)

	def ChangeBonusDict(self, dict):
		self.dict = dict
		for bonus in self.BonusList:
			try:
				for array in bonus:
					array.Hide()
			except:
				pass			
		self.SetBoni(dict)
		
	def OnUpdate(self):
		import item
		if int(app.GetTime()) > int(self.ProcessTimeStamp) + 6:
			self.SetBoni(self.dict)
			self.ProcessTimeStamp = app.GetTime()

#BonusBoardDialog().Show()
