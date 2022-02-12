
import chat
import item
import net
import player
import snd
import localeInfo
import shop
import ui
import uiTip

Bonus0 = 0
Bonus1 = 0
Bonus2 = 0
Bonus3 = 0
Bonus4 = 0
SwitchButton = 0	
Boniswitchvalue = 71084
PRESSWISH0 = 0
PRESSWISH1 = 0
PRESSWISH2 = 0
PRESSWISH3 = 0
PRESSWISH4 = 0

class SwitchBotDialog(ui.ThinBoard):
	def __init__(self):
		ui.ThinBoard.__init__(self)
		self.LoadSwitchbotDialog()
		
	def __del__(self):
		ui.ThinBoard.__del__(self)

	def Destroy(self):
		self.Hide()
		return TRUE
		
	def Bonuschangevalue(self):
		global Boniswitchvalue
		for i in xrange(player.INVENTORY_PAGE_SIZE*2):
			itemIndex = player.GetItemIndex(i)
			item.SelectItem(itemIndex)
			ItemValue = player.GetItemIndex(i)
			if item.IsAntiFlag(74112) and item.IsFlag(8196) and item.GetItemSubType() == 18:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Efsun nesnesi kodu : " + str(ItemValue))
				Boniswitchvalue = int(ItemValue)
				break
			elif str(item.GetItemName()) == "Efsun Nesnesi":
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Efsun nesnesi kodu : " + str(ItemValue))
				Boniswitchvalue = int(ItemValue)
				break
		
	def LoadSwitchbotDialog(self):
		self.SetCenterPosition()
		self.SetSize(410, 265)
		self.Show()
		self.AddFlag("movable")
		self.AddFlag("float")
		snd.PlaySound("sound/ui/type.wav")
		
		self.LoadTextLines()
		self.LoadButtons()
		self.LoadEditLines()
		self.Bonuschangevalue()
		
		self.BoardMessage = uiTip.BigBoard()
	
	def LoadEditLines(self):
		self.SlotwahlSlotBar = ui.SlotBar()
		self.SlotwahlSlotBar.SetParent(self)
		self.SlotwahlSlotBar.SetSize(29, 14)
		self.SlotwahlSlotBar.SetPosition(172, 110)
		self.SlotwahlSlotBar.SetWindowHorizontalAlignCenter()
		self.SlotwahlSlotBar.Show()
		
		self.Slotbar = ui.EditLine()
		self.Slotbar.SetParent(self.SlotwahlSlotBar)
		self.Slotbar.SetSize(29, 18)
		self.Slotbar.SetPosition(6, 0)
		self.Slotbar.SetMax(2)
		self.Slotbar.SetNumberMode()
		self.Slotbar.SetText("0")
		self.Slotbar.SetTabEvent(ui.__mem_func__(self.StartSwitchBot))
		self.Slotbar.SetReturnEvent(ui.__mem_func__(self.StartSwitchBot))
		self.Slotbar.Show()
		
		self.BonusValue5SlotBar = ui.SlotBar()
		self.BonusValue5SlotBar.SetParent(self)
		self.BonusValue5SlotBar.SetSize(29, 14)
		self.BonusValue5SlotBar.SetPosition(172, 79)
		self.BonusValue5SlotBar.SetWindowHorizontalAlignCenter()
		self.BonusValue5SlotBar.Show()
		
		self.Bvalue5 = ui.EditLine()
		self.Bvalue5.SetParent(self.BonusValue5SlotBar)
		self.Bvalue5.SetSize(29, 18)
		self.Bvalue5.SetPosition(6, 0)
		self.Bvalue5.SetMax(4)
		self.Bvalue5.SetNumberMode()
		self.Bvalue5.SetText("0")
		self.Bvalue5.SetTabEvent(ui.__mem_func__(self.Slotbar.SetFocus))
		self.Bvalue5.SetReturnEvent(ui.__mem_func__(self.Slotbar.SetFocus))
		self.Bvalue5.Show()

		self.BonusValue4SlotBar = ui.SlotBar()
		self.BonusValue4SlotBar.SetParent(self)
		self.BonusValue4SlotBar.SetSize(29, 14)
		self.BonusValue4SlotBar.SetPosition(172, 44)
		self.BonusValue4SlotBar.SetWindowHorizontalAlignCenter()
		self.BonusValue4SlotBar.Show()
		
		self.Bvalue4 = ui.EditLine()
		self.Bvalue4.SetParent(self.BonusValue4SlotBar)
		self.Bvalue4.SetSize(29, 18)
		self.Bvalue4.SetPosition(6, 0)
		self.Bvalue4.SetMax(4)
		self.Bvalue4.SetNumberMode()
		self.Bvalue4.SetFocus()
		self.Bvalue4.SetText("0")
		self.Bvalue4.SetTabEvent(ui.__mem_func__(self.Bvalue5.SetFocus))
		self.Bvalue4.SetReturnEvent(ui.__mem_func__(self.Bvalue5.SetFocus))
		self.Bvalue4.Show()

		self.BonusValue3SlotBar = ui.SlotBar()
		self.BonusValue3SlotBar.SetParent(self)
		self.BonusValue3SlotBar.SetSize(29, 14)
		self.BonusValue3SlotBar.SetPosition(-27, 110)
		self.BonusValue3SlotBar.SetWindowHorizontalAlignCenter()
		self.BonusValue3SlotBar.Show()
		
		self.Bvalue3 = ui.EditLine()
		self.Bvalue3.SetParent(self.BonusValue3SlotBar)
		self.Bvalue3.SetSize(29, 18)
		self.Bvalue3.SetPosition(6, 0)
		self.Bvalue3.SetMax(4)
		self.Bvalue3.SetNumberMode()
		self.Bvalue3.SetText("0")
		self.Bvalue3.SetTabEvent(ui.__mem_func__(self.Bvalue4.SetFocus))
		self.Bvalue3.SetReturnEvent(ui.__mem_func__(self.Bvalue4.SetFocus))
		self.Bvalue3.Show()

		self.BonusValue2SlotBar = ui.SlotBar()
		self.BonusValue2SlotBar.SetParent(self)
		self.BonusValue2SlotBar.SetSize(29, 14)
		self.BonusValue2SlotBar.SetPosition(-27, 79)
		self.BonusValue2SlotBar.SetWindowHorizontalAlignCenter()
		self.BonusValue2SlotBar.Show()
		
		self.Bvalue2 = ui.EditLine()
		self.Bvalue2.SetParent(self.BonusValue2SlotBar)
		self.Bvalue2.SetSize(29, 18)
		self.Bvalue2.SetPosition(6, 0)
		self.Bvalue2.SetMax(4)
		self.Bvalue2.SetNumberMode()
		self.Bvalue2.SetText("0")
		self.Bvalue2.SetTabEvent(ui.__mem_func__(self.Bvalue3.SetFocus))
		self.Bvalue2.SetReturnEvent(ui.__mem_func__(self.Bvalue3.SetFocus))
		self.Bvalue2.Show()

		self.BonusValue1SlotBar = ui.SlotBar()
		self.BonusValue1SlotBar.SetParent(self)
		self.BonusValue1SlotBar.SetSize(29, 14)
		self.BonusValue1SlotBar.SetPosition(-27, 44)
		self.BonusValue1SlotBar.SetWindowHorizontalAlignCenter()
		self.BonusValue1SlotBar.Show()
		
		self.Bvalue1 = ui.EditLine()
		self.Bvalue1.SetParent(self.BonusValue1SlotBar)
		self.Bvalue1.SetSize(29, 18)
		self.Bvalue1.SetPosition(6, 0)
		self.Bvalue1.SetMax(4)
		self.Bvalue1.SetNumberMode()
		self.Bvalue1.SetText("0")
		self.Bvalue1.SetFocus()
		self.Bvalue1.SetTabEvent(ui.__mem_func__(self.Bvalue2.SetFocus))
		self.Bvalue1.SetReturnEvent(ui.__mem_func__(self.Bvalue2.SetFocus))
		self.Bvalue1.Show()
		
	def LoadButtons(self):
		self.CloseButton = ui.Button()
		self.CloseButton.SetParent(self)
		self.CloseButton.SetPosition(378, 18)
		self.CloseButton.SetUpVisual("d:/ymir work/ui/public/close_button_01.sub")
		self.CloseButton.SetOverVisual("d:/ymir work/ui/public/close_button_02.sub")
		self.CloseButton.SetDownVisual("d:/ymir work/ui/public/close_button_03.sub")
		self.CloseButton.SetToolTipText(localeInfo.UI_CLOSE, 0, - 23)
		self.CloseButton.SetEvent(ui.__mem_func__(self.Close))
		self.CloseButton.Show()

		self.Wunschbonus01 = ui.Button()
		self.Wunschbonus01.SetParent(self)
		self.Wunschbonus01.SetPosition(15, 40)
		self.Wunschbonus01.SetUpVisual("d:/ymir work/ui/public/Large_Button_01.sub")
		self.Wunschbonus01.SetOverVisual("d:/ymir work/ui/public/Large_Button_02.sub")
		self.Wunschbonus01.SetDownVisual("d:/ymir work/ui/public/Large_Button_03.sub")
		self.Wunschbonus01.SetText("1. Efsun")
		self.Wunschbonus01.SetEvent(ui.__mem_func__(self.__Wish_1_Option))
		self.Wunschbonus01.Show()

		self.Wunschbonus02 = ui.Button()
		self.Wunschbonus02.SetParent(self)
		self.Wunschbonus02.SetPosition(15, 75)
		self.Wunschbonus02.SetUpVisual("d:/ymir work/ui/public/Large_Button_01.sub")
		self.Wunschbonus02.SetOverVisual("d:/ymir work/ui/public/Large_Button_02.sub")
		self.Wunschbonus02.SetDownVisual("d:/ymir work/ui/public/Large_Button_03.sub")
		self.Wunschbonus02.SetText("2. Efsun")
		self.Wunschbonus02.SetEvent(ui.__mem_func__(self.__Wish_2_Option))
		self.Wunschbonus02.Show()

		self.Wunschbonus03 = ui.Button()
		self.Wunschbonus03.SetParent(self)
		self.Wunschbonus03.SetPosition(15, 110)
		self.Wunschbonus03.SetUpVisual("d:/ymir work/ui/public/Large_Button_01.sub")
		self.Wunschbonus03.SetOverVisual("d:/ymir work/ui/public/Large_Button_02.sub")
		self.Wunschbonus03.SetDownVisual("d:/ymir work/ui/public/Large_Button_03.sub")
		self.Wunschbonus03.SetText("3. Efsun")
		self.Wunschbonus03.SetEvent(ui.__mem_func__(self.__Wish_3_Option))
		self.Wunschbonus03.Show()

		self.Wunschbonus04 = ui.Button()
		self.Wunschbonus04.SetParent(self)
		self.Wunschbonus04.SetPosition(15 + 180 + 21, 40)
		self.Wunschbonus04.SetUpVisual("d:/ymir work/ui/public/Large_Button_01.sub")
		self.Wunschbonus04.SetOverVisual("d:/ymir work/ui/public/Large_Button_02.sub")
		self.Wunschbonus04.SetDownVisual("d:/ymir work/ui/public/Large_Button_03.sub")
		self.Wunschbonus04.SetText("4. Efsun")
		self.Wunschbonus04.SetEvent(ui.__mem_func__(self.__Wish_4_Option))
		self.Wunschbonus04.Show()

		self.Wunschbonus05 = ui.Button()
		self.Wunschbonus05.SetParent(self)
		self.Wunschbonus05.SetPosition(15 + 180 + 21, 75)
		self.Wunschbonus05.SetUpVisual("d:/ymir work/ui/public/Large_Button_01.sub")
		self.Wunschbonus05.SetOverVisual("d:/ymir work/ui/public/Large_Button_02.sub")
		self.Wunschbonus05.SetDownVisual("d:/ymir work/ui/public/Large_Button_03.sub")
		self.Wunschbonus05.SetText("5. Efsun")
		self.Wunschbonus05.SetEvent(ui.__mem_func__(self.__Wish_5_Option))
		self.Wunschbonus05.Show()

		self.ResetbonusallButton = ui.Button()
		self.ResetbonusallButton.SetParent(self)
		self.ResetbonusallButton.SetPosition(90 + 45 + 80, 225)
		self.ResetbonusallButton.SetUpVisual("d:/ymir work/ui/public/XLarge_Button_01.sub")
		self.ResetbonusallButton.SetOverVisual("d:/ymir work/ui/public/XLarge_Button_02.sub")
		self.ResetbonusallButton.SetDownVisual("d:/ymir work/ui/public/XLarge_Button_03.sub")
		self.ResetbonusallButton.SetText("Se�imleri Temizle")
		self.ResetbonusallButton.SetEvent(ui.__mem_func__(self.__Resetbonusall))
		self.ResetbonusallButton.Show()

		self.Switchtingabbruchbutton = ui.Button()
		self.Switchtingabbruchbutton.SetParent(self)
		self.Switchtingabbruchbutton.SetPosition(90 + 45 + 80, 195)
		self.Switchtingabbruchbutton.SetUpVisual("d:/ymir work/ui/public/XLarge_Button_01.sub")
		self.Switchtingabbruchbutton.SetOverVisual("d:/ymir work/ui/public/XLarge_Button_02.sub")
		self.Switchtingabbruchbutton.SetDownVisual("d:/ymir work/ui/public/XLarge_Button_03.sub")
		self.Switchtingabbruchbutton.SetEvent(ui.__mem_func__(self.__BreakSwitching))
		self.Switchtingabbruchbutton.SetText("�ptal")
		self.Switchtingabbruchbutton.Show()

		self.StartButton = ui.Button()
		self.StartButton.SetParent(self)
		self.StartButton.SetPosition(90 + 45 - 122, 225)
		self.StartButton.SetUpVisual("d:/ymir work/ui/public/XLarge_Button_01.sub")
		self.StartButton.SetOverVisual("d:/ymir work/ui/public/XLarge_Button_02.sub")
		self.StartButton.SetDownVisual("d:/ymir work/ui/public/XLarge_Button_03.sub")
		self.StartButton.SetEvent(ui.__mem_func__(self.StartSwitchBot))
		self.StartButton.SetText("Ba�lat")
		self.StartButton.Show()

		self.SlotButton = ui.Button()
		self.SlotButton.SetParent(self)
		self.SlotButton.SetPosition(15 + 201, 110)
		self.SlotButton.SetUpVisual("d:/ymir work/ui/public/Large_Button_01.sub")
		self.SlotButton.SetOverVisual("d:/ymir work/ui/public/Large_Button_01.sub")
		self.SlotButton.SetDownVisual("d:/ymir work/ui/public/Large_Button_01.sub")
		self.SlotButton.SetText("Yuvan�n Yeri")
		self.SlotButton.Show()		
		
	def LoadTextLines(self):
		self.Headline = ui.TextLine()
		self.Headline.SetParent(self)
		self.Headline.SetDefaultFontName()
		self.Headline.SetPosition(120, 15)
		self.Headline.SetFeather()
		self.Headline.SetFontColor(0.2, 0.7, 0.8)
		self.Headline.SetOutline()
		self.Headline.Show()

		self.LastChangeText = ui.TextLine()
		self.LastChangeText.SetParent(self)
		self.LastChangeText.SetDefaultFontName()
		self.LastChangeText.SetPosition(220, 142)
		self.LastChangeText.SetFeather()
		self.LastChangeText.SetText("Olay G�ncesi:")
		self.LastChangeText.SetFontColor(0.6, 0.7, 1)
		self.LastChangeText.SetOutline()
		self.LastChangeText.Show()	

		self.LastChange = ui.TextLine()
		self.LastChange.SetParent(self)
		self.LastChange.SetDefaultFontName()
		self.LastChange.SetPosition(220, 165)
		self.LastChange.SetFeather()
		self.LastChange.SetFontColor(1.0, 1.0, 1.0)
		self.LastChange.SetOutline()
		self.LastChange.Show()	

		self.SlotwahlText = ui.TextLine()
		self.SlotwahlText.SetParent(self)
		self.SlotwahlText.SetPosition(15 + 112 + 201, 110)
		self.SlotwahlText.SetFeather()
		self.SlotwahlText.SetDefaultFontName()
		self.SlotwahlText.SetText("Konum:")
		self.SlotwahlText.SetFontColor(1.0, 1.0, 1.0)
		self.SlotwahlText.SetOutline()
		self.SlotwahlText.Show()	

		self.BonusValue1Text = ui.TextLine()
		self.BonusValue1Text.SetParent(self)
		self.BonusValue1Text.SetPosition(15 + 92, 44)
		self.BonusValue1Text.SetFeather()
		self.BonusValue1Text.SetDefaultFontName()
		self.BonusValue1Text.SetText("Oran:")
		self.BonusValue1Text.SetFontColor(1.0, 1.0, 1.0)
		self.BonusValue1Text.SetOutline()
		self.BonusValue1Text.Show()	

		self.BonusValue2Text = ui.TextLine()
		self.BonusValue2Text.SetParent(self)
		self.BonusValue2Text.SetPosition(15 + 92, 79)
		self.BonusValue2Text.SetFeather()
		self.BonusValue2Text.SetDefaultFontName()
		self.BonusValue2Text.SetText("Oran:")
		self.BonusValue2Text.SetFontColor(1.0, 1.0, 1.0)
		self.BonusValue2Text.SetOutline()
		self.BonusValue2Text.Show()	

		self.BonusValue3Text = ui.TextLine()
		self.BonusValue3Text.SetParent(self)
		self.BonusValue3Text.SetPosition(15 + 92, 110)
		self.BonusValue3Text.SetFeather()
		self.BonusValue3Text.SetDefaultFontName()
		self.BonusValue3Text.SetText("Oran:")
		self.BonusValue3Text.SetFontColor(1.0, 1.0, 1.0)
		self.BonusValue3Text.SetOutline()
		self.BonusValue3Text.Show()	

		self.BonusValue4Text = ui.TextLine()
		self.BonusValue4Text.SetParent(self)
		self.BonusValue4Text.SetPosition(15 + 272 + 21, 44)
		self.BonusValue4Text.SetFeather()
		self.BonusValue4Text.SetDefaultFontName()
		self.BonusValue4Text.SetText("Oran:")
		self.BonusValue4Text.SetFontColor(1.0, 1.0, 1.0)
		self.BonusValue4Text.SetOutline()
		self.BonusValue4Text.Show()	

		self.BonusValue5Text = ui.TextLine()
		self.BonusValue5Text.SetParent(self)
		self.BonusValue5Text.SetPosition(15 + 272 + 21, 79)
		self.BonusValue5Text.SetFeather()
		self.BonusValue5Text.SetDefaultFontName()
		self.BonusValue5Text.SetText("Oran:")
		self.BonusValue5Text.SetFontColor(1.0, 1.0, 1.0)
		self.BonusValue5Text.SetOutline()
		self.BonusValue5Text.Show()	

		self.BonusHeadline = ui.TextLine()
		self.BonusHeadline.SetParent(self)
		self.BonusHeadline.SetDefaultFontName()
		self.BonusHeadline.SetPosition(18, 142)
		self.BonusHeadline.SetFeather()
		self.BonusHeadline.SetText("�stenilen Efsunlar:")
		self.BonusHeadline.SetFontColor(0.6, 0.7, 1)
		self.BonusHeadline.SetOutline()
		self.BonusHeadline.Show()	

		self.Bonus1Attr = ui.TextLine()
		self.Bonus1Attr.SetParent(self)
		self.Bonus1Attr.SetDefaultFontName()
		self.Bonus1Attr.SetPosition(18, 157 + 13*0)
		self.Bonus1Attr.SetFeather()
		self.Bonus1Attr.SetText("se�iniz...")
		self.Bonus1Attr.SetFontColor(1.0, 1.0, 1.0)
		self.Bonus1Attr.SetOutline()
		self.Bonus1Attr.Show()	

		self.Bonus1Var = ui.TextLine()
		self.Bonus1Var.SetParent(self)
		self.Bonus1Var.SetDefaultFontName()
		self.Bonus1Var.SetPosition(168, 157 + 13*0)
		self.Bonus1Var.SetFeather()
		self.Bonus1Var.SetText("0")
		self.Bonus1Var.SetFontColor(1.0, 1.0, 1.0)
		self.Bonus1Var.SetOutline()
		self.Bonus1Var.Show()	

		self.Bonus2Attr = ui.TextLine()
		self.Bonus2Attr.SetParent(self)
		self.Bonus2Attr.SetDefaultFontName()
		self.Bonus2Attr.SetPosition(18, 157 + 13*1)
		self.Bonus2Attr.SetFeather()
		self.Bonus2Attr.SetText("se�iniz...")
		self.Bonus2Attr.SetFontColor(1.0, 1.0, 1.0)
		self.Bonus2Attr.SetOutline()
		self.Bonus2Attr.Show()	

		self.Bonus2Var = ui.TextLine()
		self.Bonus2Var.SetParent(self)
		self.Bonus2Var.SetDefaultFontName()
		self.Bonus2Var.SetPosition(168, 157 + 13*1)
		self.Bonus2Var.SetFeather()
		self.Bonus2Var.SetText("0")
		self.Bonus2Var.SetFontColor(1.0, 1.0, 1.0)
		self.Bonus2Var.SetOutline()
		self.Bonus2Var.Show()	

		self.Bonus3Attr = ui.TextLine()
		self.Bonus3Attr.SetParent(self)
		self.Bonus3Attr.SetDefaultFontName()
		self.Bonus3Attr.SetPosition(18, 157 + 13*2)
		self.Bonus3Attr.SetFeather()
		self.Bonus3Attr.SetText("se�iniz...")
		self.Bonus3Attr.SetFontColor(1.0, 1.0, 1.0)
		self.Bonus3Attr.SetOutline()
		self.Bonus3Attr.Show()	

		self.Bonus3Var = ui.TextLine()
		self.Bonus3Var.SetParent(self)
		self.Bonus3Var.SetDefaultFontName()
		self.Bonus3Var.SetPosition(168, 157 + 13*2)
		self.Bonus3Var.SetFeather()
		self.Bonus3Var.SetText("0")
		self.Bonus3Var.SetFontColor(1.0, 1.0, 1.0)
		self.Bonus3Var.SetOutline()
		self.Bonus3Var.Show()	

		self.Bonus4Attr = ui.TextLine()
		self.Bonus4Attr.SetParent(self)
		self.Bonus4Attr.SetDefaultFontName()
		self.Bonus4Attr.SetPosition(18, 157 + 13*3)
		self.Bonus4Attr.SetFeather()
		self.Bonus4Attr.SetText("se�iniz...")
		self.Bonus4Attr.SetFontColor(1.0, 1.0, 1.0)
		self.Bonus4Attr.SetOutline()
		self.Bonus4Attr.Show()	

		self.Bonus4Var = ui.TextLine()
		self.Bonus4Var.SetParent(self)
		self.Bonus4Var.SetDefaultFontName()
		self.Bonus4Var.SetPosition(168, 157 + 13*3)
		self.Bonus4Var.SetFeather()
		self.Bonus4Var.SetText("0")
		self.Bonus4Var.SetFontColor(1.0, 1.0, 1.0)
		self.Bonus4Var.SetOutline()
		self.Bonus4Var.Show()	

		self.Bonus5Attr = ui.TextLine()
		self.Bonus5Attr.SetParent(self)
		self.Bonus5Attr.SetDefaultFontName()
		self.Bonus5Attr.SetPosition(18, 157 + 13*4)
		self.Bonus5Attr.SetFeather()
		self.Bonus5Attr.SetText("se�iniz...")
		self.Bonus5Attr.SetFontColor(1.0, 1.0, 1.0)
		self.Bonus5Attr.SetOutline()
		self.Bonus5Attr.Show()	

		self.Bonus5Var = ui.TextLine()
		self.Bonus5Var.SetParent(self)
		self.Bonus5Var.SetDefaultFontName()
		self.Bonus5Var.SetPosition(168, 157 + 13*4)
		self.Bonus5Var.SetFeather()
		self.Bonus5Var.SetText("0")
		self.Bonus5Var.SetFontColor(1.0, 1.0, 1.0)
		self.Bonus5Var.SetOutline()
		self.Bonus5Var.Show()	
		
	def __BreakSwitching(self):
		global SwitchButton
		if SwitchButton == 1:
			self.LastChange.SetText("Efsunlama i�lemi iptal edildi")
			self.Switchtingabbruchbutton.SetText("�ptal")
			SwitchButton = 0		
		else:
			self.Hide()
			
	def StartSwitchBot(self):
		global SwitchButton
		SwitchButton = 1		
		self.LastChange.SetText("Efsunlama i�lemine ba�land�")
		self.Switchtingabbruchbutton.SetText("Efsunlamay� B�rak")
		self.__Switchtingdialog()
		
	def __Switchtingdialog(self):
		global BoniSwitchvalue
		global Bonus0
		global Bonus1
		global Bonus2
		global Bonus3
		global Bonus4
		global SwitchButton
		Slot = self.Slotbar.GetText()
		val0, bon0 = player.GetItemAttribute((int(Slot)), 0) #(itemposition, atrribute)
		val1, bon1 = player.GetItemAttribute((int(Slot)), 1) #(itemposition, atrribute)
		val2, bon2 = player.GetItemAttribute((int(Slot)), 2) #(itemposition, atrribute)
		val3, bon3 = player.GetItemAttribute((int(Slot)), 3) #(itemposition, atrribute)
		val4, bon4 = player.GetItemAttribute((int(Slot)), 4) #(itemposition, atrribute)
		Switchvalue = Boniswitchvalue
		Search0 = self.Bvalue1.GetText()
		Search1 = self.Bvalue2.GetText()
		Search2 = self.Bvalue3.GetText()
		Search3 = self.Bvalue4.GetText()
		Search4 = self.Bvalue5.GetText()
		DELAY_SEC = 0.3

#1 Bonus switchen:
		if SwitchButton == 1:
			if (int(Bonus1) == 0) and (val0 == int(Bonus0) and bon0 >= int(Search0) or (val1 == int(Bonus0) and bon1 >= int(Search0)) or (val2 == int(Bonus0) and bon2 >= int(Search0)) or (val3 == int(Bonus0) and bon3 >= int(Search0)) or (val4 == int(Bonus0) and bon4 >= int(Search0))):
				self.BoardMessage.SetTip("Efsunlar Getirildi *")
				self.BoardMessage.SetTop()
				self.LastChange.SetText("Efsunlama ba�ar�yla tamamland�")
				self.Switchtingabbruchbutton.SetText("�ptal")
				SwitchButton = 0		
#2 Bonis switchen:
			elif (int(Bonus2) == 0) and (val0 == int(Bonus0) and bon0 >= int(Search0) or (val1 == int(Bonus0) and bon1 >= int(Search0)) or (val2 == int(Bonus0) and bon2 >= int(Search0)) or (val3 == int(Bonus0) and bon3 >= int(Search0)) or (val4 == int(Bonus0) and bon4 >= int(Search0))) and ((val0 == int(Bonus1) and bon0 >= int(Search1)) or (val1 == int(Bonus1) and bon1 >= int(Search1)) or (val2 == int(Bonus1) and bon2 >= int(Search1)) or (val3 == int(Bonus1) and bon3 >= int(Search1)) or (val4 == int(Bonus1) and bon4 >= int(Search1))):
				self.BoardMessage.SetTip("Efsunlar Getirildi *")
				self.BoardMessage.SetTop()
				self.LastChange.SetText("Efsunlama ba�ar�yla tamamland�")
				self.Switchtingabbruchbutton.SetText("�ptal")
				SwitchButton = 0		
#3 Bonis switchen:
			elif (int(Bonus3) == 0) and (val0 == int(Bonus0) and bon0 >= int(Search0) or (val1 == int(Bonus0) and bon1 >= int(Search0)) or (val2 == int(Bonus0) and bon2 >= int(Search0)) or (val3 == int(Bonus0) and bon3 >= int(Search0)) or (val4 == int(Bonus0) and bon4 >= int(Search0))) and ((val0 == int(Bonus1) and bon0 >= int(Search1)) or (val1 == int(Bonus1) and bon1 >= int(Search1)) or (val2 == int(Bonus1) and bon2 >= int(Search1)) or (val3 == int(Bonus1) and bon3 >= int(Search1)) or (val4 == int(Bonus1) and bon4 >= int(Search1))) and ((val0 == int(Bonus2) and bon0 >= int(Search2)) or (val1 == int(Bonus2) and bon1 >= int(Search2)) or (val2 == int(Bonus2) and bon2 >= int(Search2)) or (val3 == int(Bonus2) and bon3 >= int(Search2)) or (val4 == int(Bonus2) and bon4 >= int(Search2))):
				self.BoardMessage.SetTip("*Efsunlar Getirildi *")
				self.BoardMessage.SetTop()
				self.LastChange.SetText("Efsunlama ba�ar�yla tamamland�")
				self.Switchtingabbruchbutton.SetText("�ptal")
				SwitchButton = 0		
#4 Bonis switchen:
			elif (int(Bonus4) == 0) and (val0 == int(Bonus0) and bon0 >= int(Search0) or (val1 == int(Bonus0) and bon1 >= int(Search0)) or (val2 == int(Bonus0) and bon2 >= int(Search0)) or (val3 == int(Bonus0) and bon3 >= int(Search0)) or (val4 == int(Bonus0) and bon4 >= int(Search0))) and ((val0 == int(Bonus1) and bon0 >= int(Search1)) or (val1 == int(Bonus1) and bon1 >= int(Search1)) or (val2 == int(Bonus1) and bon2 >= int(Search1)) or (val3 == int(Bonus1) and bon3 >= int(Search1)) or (val4 == int(Bonus1) and bon4 >= int(Search1))) and ((val0 == int(Bonus2) and bon0 >= int(Search2)) or (val1 == int(Bonus2) and bon1 >= int(Search2)) or (val2 == int(Bonus2) and bon2 >= int(Search2)) or (val3 == int(Bonus2) and bon3 >= int(Search2)) or (val4 == int(Bonus2) and bon4 >= int(Search2))) and ((val0 == int(Bonus3) and bon0 >= int(Search3)) or (val1 == int(Bonus3) and bon1 >= int(Search3)) or (val2 == int(Bonus3) and bon2 >= int(Search3)) or (val3 == int(Bonus3) and bon3 >= int(Search3)) or (val4 == int(Bonus3) and bon4 >= int(Search3))):
				self.BoardMessage.SetTip("Efsunlar Getirildi *")
				self.BoardMessage.SetTop()
				self.LastChange.SetText("Efsunlama ba�ar�yla tamamland�")
				self.Switchtingabbruchbutton.SetText("�ptal")
				SwitchButton = 0		
#5 Bonis switchen:
			elif (int(Bonus4) != 0) and (val0 == int(Bonus0) and bon0 >= int(Search0) or (val1 == int(Bonus0) and bon1 >= int(Search0)) or (val2 == int(Bonus0) and bon2 >= int(Search0)) or (val3 == int(Bonus0) and bon3 >= int(Search0)) or (val4 == int(Bonus0) and bon4 >= int(Search0))) and ((val0 == int(Bonus1) and bon0 >= int(Search1)) or (val1 == int(Bonus1) and bon1 >= int(Search1)) or (val2 == int(Bonus1) and bon2 >= int(Search1)) or (val3 == int(Bonus1) and bon3 >= int(Search1)) or (val4 == int(Bonus1) and bon4 >= int(Search1))) and ((val0 == int(Bonus2) and bon0 >= int(Search2)) or (val1 == int(Bonus2) and bon1 >= int(Search2)) or (val2 == int(Bonus2) and bon2 >= int(Search2)) or (val3 == int(Bonus2) and bon3 >= int(Search2)) or (val4 == int(Bonus2) and bon4 >= int(Search2))) and ((val0 == int(Bonus3) and bon0 >= int(Search3)) or (val1 == int(Bonus3) and bon1 >= int(Search3)) or (val2 == int(Bonus3) and bon2 >= int(Search3)) or (val3 == int(Bonus3) and bon3 >= int(Search3)) or (val4 == int(Bonus3) and bon4 >= int(Search3))) and ((val0 == int(Bonus4) and bon0 >= int(Search4)) or (val1 == int(Bonus4) and bon1 >= int(Search4)) or (val2 == int(Bonus4) and bon2 >= int(Search4)) or (val3 == int(Bonus4) and bon3 >= int(Search4)) or (val4 == int(Bonus4) and bon4 >= int(Search4))):
				self.BoardMessage.SetTip("Efsunlar Getirildi *")
				self.BoardMessage.SetTop()
				self.LastChange.SetText("Efsunlama ba�ar�yla tamamland�")
				self.Switchtingabbruchbutton.SetText("�ptal")
				SwitchButton = 0		
			elif Bonus0 == 0:
				self.Switchtingabbruchbutton.SetText("�ptal")
				SwitchButton = 0		
				self.LastChange.SetText("Efsunlama i�lemi iptal edildi!")
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Efsun i�leminin devam edebilmesi")		
				chat.AppendChat(chat.CHAT_TYPE_INFO, "i�in gerekli nesneye sahip de�ilsiniz!")		
			else:
				self.WaitingDelay = WaitingDialog()
				self.WaitingDelay.Open(float(DELAY_SEC))
				self.WaitingDelay.SAFE_SetTimeOverEvent(self.__Switchtingdialog)
				for eachSlot in xrange(player.INVENTORY_PAGE_SIZE*2):
					itemVNum = player.GetItemIndex(eachSlot)

					if itemVNum == int(Switchvalue):
						net.SendItemUseToItemPacket(eachSlot, (int(Slot)))
						break
			if player.GetItemCountByVnum(int(Switchvalue)) <= 1:
				for eachSlot in xrange(shop.SHOP_SLOT_COUNT):
					getShopItemID = shop.GetItemID(eachSlot)
					if getShopItemID == int(Switchvalue) and not itemVNum == int(Switchvalue):
						net.SendShopBuyPacket(eachSlot)
			
	def __Resetbonusall(self):
		global Bonus0
		global Bonus1
		global Bonus2
		global Bonus3
		global Bonus4
		Bonus0 = 0
		Bonus1 = 0
		Bonus2 = 0
		Bonus3 = 0
		Bonus4 = 0
		self.Bvalue1.SetText("0")
		self.Bvalue2.SetText("0")
		self.Bvalue3.SetText("0")
		self.Bvalue4.SetText("0")
		self.Bvalue5.SetText("0")
		self.Bonus1Attr.SetText("se�iniz...")
		self.Bonus2Attr.SetText("se�iniz...")
		self.Bonus3Attr.SetText("se�iniz...")
		self.Bonus4Attr.SetText("se�iniz...")
		self.Bonus5Attr.SetText("se�iniz...")
		self.LastChange.SetText("Efsun se�imleri temizlendi")
	
	def __Wish_1_Option(self):
		global Bonus0
		global PRESSWISH0
		PRESSWISH0 = 1
		self.BonusListBox = FileListDialog()
		
	def __Wish_2_Option(self):
		global Bonus1
		global PRESSWISH1
		PRESSWISH1 = 1
		self.BonusListBox = FileListDialog()
		
	def __Wish_3_Option(self):
		global Bonus2
		global PRESSWISH2
		PRESSWISH2 = 1
		self.BonusListBox = FileListDialog()

	def __Wish_4_Option(self):
		global Bonus3
		global PRESSWISH3
		PRESSWISH3 = 1
		self.BonusListBox = FileListDialog()
		
	def __Wish_5_Option(self):
		global Bonus4
		global PRESSWISH4
		PRESSWISH4 = 1
		self.BonusListBox = FileListDialog()
	
	def OnUpdate(self):
		global Bonus0
		global Bonus1
		global Bonus2
		global Bonus3
		global Bonus4
		if self.Bonus1Attr.GetText() != str(BonusListe[int(Bonus0)]) and int(Bonus0) != 0:
			self.Bonus1Attr.SetText(str(BonusListe[int(Bonus0)]))
		elif self.Bonus1Attr.GetText() != "" and int(Bonus0) == 0:
			self.Bonus1Attr.SetText("se�iniz...")		
		if self.Bonus2Attr.GetText() != str(BonusListe[int(Bonus1)]) and int(Bonus1) != 0:
			self.Bonus2Attr.SetText(str(BonusListe[int(Bonus1)]))
		elif self.Bonus2Attr.GetText() != "" and int(Bonus1) == 0:
			self.Bonus2Attr.SetText("se�iniz...")		
		if self.Bonus3Attr.GetText() != str(BonusListe[int(Bonus2)]) and int(Bonus2) != 0:
			self.Bonus3Attr.SetText(str(BonusListe[int(Bonus2)]))
		elif self.Bonus3Attr.GetText() != "" and int(Bonus2) == 0:
			self.Bonus3Attr.SetText("se�iniz...")		
		if self.Bonus4Attr.GetText() != str(BonusListe[int(Bonus3)]) and int(Bonus3) != 0:
			self.Bonus4Attr.SetText(str(BonusListe[int(Bonus3)]))
		elif self.Bonus4Attr.GetText() != "" and int(Bonus3) == 0:
			self.Bonus4Attr.SetText("se�iniz...")		
		if self.Bonus5Attr.GetText() != str(BonusListe[int(Bonus4)]) and int(Bonus4) != 0:
			self.Bonus5Attr.SetText(str(BonusListe[int(Bonus4)]))
		elif self.Bonus5Attr.GetText() != "" and int(Bonus4) == 0:
			self.Bonus5Attr.SetText("se�iniz...")
			
		if self.Bonus1Var.GetText() != self.Bvalue1.GetText():
			self.Bonus1Var.SetText(str(self.Bvalue1.GetText()))
		if self.Bonus2Var.GetText() != self.Bvalue2.GetText():
			self.Bonus2Var.SetText(str(self.Bvalue2.GetText()))
		if self.Bonus3Var.GetText() != self.Bvalue3.GetText():
			self.Bonus3Var.SetText(str(self.Bvalue3.GetText()))
		if self.Bonus4Var.GetText() != self.Bvalue4.GetText():
			self.Bonus4Var.SetText(str(self.Bvalue4.GetText()))
		if self.Bonus5Var.GetText() != self.Bvalue5.GetText():
			self.Bonus5Var.SetText(str(self.Bvalue5.GetText()))
		
	def Show(self):
		ui.ThinBoard.Show(self)
		
	def Close(self):
		self.Hide()
		return TRUE
		
	def OnPressEscapeKey(self):
		self.Hide()
		return TRUE

BonusListe = ( 
	"",
	"Max. HP",
	"Max. SP",
	"Ya�am Enerjisi",
	"Zeka", 
	"G��",
	"�eviklik",
	"Sald�r� H�z�",
	"Hareket H�z�",
	"B�y� H�z�",
	"HP �retimi",
	"SP �retimi",
	"Zehirleme San��",
	"Sersemletme San��",
	"Yava�latma San��",
	"Kritik Vuru�",
	"Delici Vuru�",
	"Yar�insanlara Kar�� G��l�",
	"Hayvanlara Kar�� G��l�",
	"Orklara Kar�� G��l�",
	"Mistiklere Kar�� G��l�",
	"�l�ms�zlere Kar�� G��l�",
	"�eytanlara Kar�� G��l�",
	"Hasar HP'den Emilecek",
	"Hasar SP'den Emilecek",
	"D��man�n SP'sini �alma",
	"Vuru� Yap�ld���nda SPyi geri kazanma",
	"Bedensel Ataklar�n Bloklanmas�",
	"Oklardan Korunma San��",
	"K�l�� Savunmas�",
	"�ift El Savunmas�",
	"B��ak Savunmas�",
	"�an Savunmas�",
	"Yelpaze Savunmas�",
	"Oka Kar�� Dayan�kl�l�k",
	"Ate�e Kar�� Dayan�kl�l�k",
	"�im�e�e Kar�� Dayan�kl�l�k",
	"B�y�ye Kar�� Dayan�kl�l�k",
	"R�zgara Kar�� Dayan�kl�l�k",
	"Vuc�t Darbelerini Yans�tma",
	"Lanet Yans�mas�",
	"Zehire Kar�� Koyma",
	"Sp Y�kselmesi De�i�imi",
	"EXP Bonusu",
	"�ki Kat Yang D��me �ans�",
	"�ki Kat E�ya D��me �ans�",
	"�ksir Etkisi Y�kseldi",
	"Hp Y�kselmesi De�i�ti",
	"Sersemlik Kar��s�nda Ba����kl�k",
	"Yava�lama Kar��s�nda Ba����kl�k",
	"Yere D��meye Kar�� Ba����kl�l�k",
	"APPLY_SKILL",
	"Yay Menzili m",
	"Sald�r� De�eri",
	"Savunma",
	"B�y�l� Sald�r� De�eri",
	"B�y� Savunmas�",
	"",
	"Max. Dayan�kl�l�k",
	"Sava���lara Kar�� G��l�",
	"Ninjalara Kar�� G��l�",
	"Suralara Kar�� G��l�",
	"�amanlara Kar�� G��l�",
	"Yarat�klara Kar�� G��l�",
	"Sald�r� De�eri",
	"Itemshop Savunma",
	"Itemshop Exp Bonus",
	"Itemshop Item Bonus",
	"Itemshop Yang Bonus",
	"APPLY_MAX_HP_PCT",
	"APPLY_MAX_SP_PCT",
	"Beceri Hasar�",
	"Ortalama Zarar",
	"Beceri Hasar�na Kar�� Koyma",
	"Ortalama Zarara Direni�",
	"",
	"iCafe EXP-Bonus",
	"iCafe Item-Bonus",
	"Sava��� Sald�r�lar�na Kar�� Savunma �ans�",
	"Ninja Sald�r�lar�na Kar�� Savunma �ans�",
	"Sura Saldr�r�lar�na Kar�� Savunma �ans�",
	"�aman Sald�r�lar�na Kar�� Savunma �ans�",
	)

BonusIDListe = { 
	"" : 0,
	"Max. HP" : 1,
	"Max. SP" : 2,
	"Ya�am Enerjisi" : 3,
	"Zeka" : 4, 
	"G��" : 5,
	"�eviklik" : 6,
	"Sald�r� H�z�" : 7,
	"Hareket H�z�" : 8,
	"B�y� H�z�" : 9,
	"HP �retimi" : 10,
	"SP �retimi" : 11,
	"Zehirleme San��" : 12,
	"Sersemletme San��" : 13,
	"Yava�latma San��" : 14,
	"Kritik Vuru�" : 15,
	"Delici Vuru�" : 16,
	"Yar�insanlara Kar�� G��l�" : 17,
	"Hayvanlara Kar�� G��l�" : 18,
	"Orklara Kar�� G��l�" : 19,
	"Mistiklere Kar�� G��l�" : 20,
	"�l�ms�zlere Kar�� G��l�" : 21,
	"�eytanlara Kar�� G��l�" : 22,
	"Hasar HP'den Emilecek" : 23,
	"Hasar SP'den Emilecek" : 24,
	"D��man�n SP'sini �alma" : 25,
	"Vuru� Yap�ld���nda SPyi geri kazanma" : 26,
	"Bedensel Ataklar�n Bloklanmas�" : 27,
	"Oklardan Korunma San��" : 28,
	"K�l�� Savunmas�" : 29,
	"�ift El Savunmas�" : 30,
	"B��ak Savunmas�" : 31,
	"�an Savunmas�" : 32,
	"Yelpaze Savunmas�" : 33,
	"Oka Kar�� Dayan�kl�l�k" : 34,
	"Ate�e Kar�� Dayan�kl�l�k" : 35,
	"�im�e�e Kar�� Dayan�kl�l�k" : 36,
	"B�y�ye Kar�� Dayan�kl�l�k" : 37,
	"R�zgara Kar�� Dayan�kl�l�k" : 38,
	"Vuc�t Darbelerini Yans�tma" : 39,
	"Lanet Yans�mas�" : 40,
	"Zehire Kar�� Koyma" : 41,
	"Zehire Kar�� Koyma" : 42,
	"EXP Bonusu" : 43,
	"�ki Kat Yang D��me �ans�" : 44,
	"�ki Kat E�ya D��me �ans�" : 45,
	"�ksir Etkisi Y�kseldi" : 46,
	"Hp Y�kselmesi De�i�ti" : 47,
	"Sersemlik Kar��s�nda Ba����kl�k" : 48,
	"Yava�lama Kar��s�nda Ba����kl�k" : 49,
	"Yere D��meye Kar�� Ba����kl�l�k" : 50,
	"APPLY_SKILL" : 51,
	"Yay Menzili m" : 52,
	"Sald�r� De�eri" : 53,
	"Savunma" : 54,
	"B�y�l� Sald�r� De�eri" : 55,
	"B�y� Savunmas�" : 56,
	"" : 57,
	"Max. Dayan�kl�l�k" : 58,
	"Sava���lara Kar�� G��l�" : 59,
	"Ninjalara Kar�� G��l�" : 60,
	"Suralara Kar�� G��l�" : 61,
	"�amanlara Kar�� G��l�" : 62,
	"Yarat�klara Kar�� G��l�" : 63,
	"Sald�r� De�eri" : 64,
	"Sald�r� De�eri" : 65,
	"Itemshop Exp Bonus" : 66,
	"Itemshop Item Bonus" : 67,
	"Itemshop Yang Bonus" : 68,
	"APPLY_MAX_HP_PCT" : 69,
	"APPLY_MAX_SP_PCT" : 70,
	"Beceri Hasar�" : 71,
	"Ortalama Zarar" : 72,
	"Beceri Hasar�na Kar�� Koyma" : 73,
	"Ortalama Zarara Direni�" : 74,
	"" : 75,
	"iCafe EXP-Bonus" : 76,
	"iCafe Item-Bonus" : 77,
	"Sava��� Sald�r�lar�na Kar�� Savunma �ans�" : 78,
	"Ninja Sald�r�lar�na Kar�� Savunma �ans�" : 79,
	"Sura Saldr�r�lar�na Kar�� Savunma �ans�" : 80,
	"�aman Sald�r�lar�na Kar�� Savunma �ans�" : 81,
	}
	
class FileListDialog(ui.ThinBoard):
	def __init__(self):
		ui.ThinBoard.__init__(self)

		self.isLoaded=0
		self.selectEvent=None
		self.fileListBox=None

		self.SetCenterPosition()
		self.SetSize(170, 305)
		self.Show()
		self.AddFlag("movable")
		self.AddFlag("float")
		
	def __del__(self):
		ui.ThinBoard.__del__(self)

	def Show(self):
		if self.isLoaded==0:
			self.isLoaded=1

			self.__Load()

		ui.ThinBoard.Show(self)

	def Open(self):
		self.Show()

		self.SetCenterPosition()
		self.SetTop()
		self.UpdateFileList()

	def Close(self):
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE

	def __CreateFileListBox(self):
		fileListBox = ui.ListBoxEx()
		fileListBox.SetParent(self)
		fileListBox.SetPosition(15, 50)
		fileListBox.Show()
		return fileListBox

	def __Load(self):
		self.__Load_BindObject()

		self.UpdateFileList()

	def __Load_BindObject(self):
		self.fileListBox = self.__CreateFileListBox()
		self.LoadFuckingScrollBar()
		self.LoadTextLines()
		self.fileListBox.SetScrollBar(self.ScrollBar)

		self.UpdateButton = ui.Button()
		self.UpdateButton.SetParent(self)
		self.UpdateButton.SetUpVisual("d:/ymir work/ui/public/Large_button_01.sub")
		self.UpdateButton.SetOverVisual("d:/ymir work/ui/public/Large_button_02.sub")
		self.UpdateButton.SetDownVisual("d:/ymir work/ui/public/Large_button_03.sub")
		self.UpdateButton.SetText("G�ncelle�tirme")
		self.UpdateButton.SetPosition(15, 265)
		self.UpdateButton.SetEvent(ui.__mem_func__(self.UpdateFileList))
		self.UpdateButton.Show()
		self.UpdateButton.Hide()
		
		self.SelectBonus = ui.Button()
		self.SelectBonus.SetParent(self)
		self.SelectBonus.SetPosition(19, 265)
		self.SelectBonus.SetUpVisual("d:/ymir work/ui/public/Middle_Button_01.sub")
		self.SelectBonus.SetOverVisual("d:/ymir work/ui/public/Middle_Button_02.sub")
		self.SelectBonus.SetDownVisual("d:/ymir work/ui/public/Middle_Button_03.sub")
		self.SelectBonus.SetText("Tamam")
		self.SelectBonus.SetEvent(ui.__mem_func__(self.SetBonis))
		self.SelectBonus.Show()

		self.CancelBonus = ui.Button()
		self.CancelBonus.SetParent(self)
		self.CancelBonus.SetPosition(89, 265)
		self.CancelBonus.SetUpVisual("d:/ymir work/ui/public/Middle_Button_01.sub")
		self.CancelBonus.SetOverVisual("d:/ymir work/ui/public/Middle_Button_02.sub")
		self.CancelBonus.SetDownVisual("d:/ymir work/ui/public/Middle_Button_03.sub")
		self.CancelBonus.SetText("�ptal")
		self.CancelBonus.SetEvent(ui.__mem_func__(self.Close))
		self.CancelBonus.Show()

		
	def LoadTextLines(self):
		self.copyright = ui.TextLine()
		self.copyright.SetParent(self)
		self.copyright.SetDefaultFontName()
		self.copyright.SetPosition(50, 29)
		self.copyright.SetFeather()
		self.copyright.SetText("Efsun Se�in:")
		self.copyright.SetFontColor(0.2, 0.2, 1.0)
		self.copyright.SetOutline()
		self.copyright.Show()


	def LoadFuckingScrollBar(self):
		self.ScrollBar = ui.ScrollBar()
		self.ScrollBar.SetParent(self)
		self.ScrollBar.SetPosition(140, 40)
		self.ScrollBar.SetScrollBarSize(220)
		self.ScrollBar.Show()

	def CancelGuildName(self):
		self.guildNameBoard.Close()
		self.guildNameBoard = None
		return TRUE

	def UpdateFileList(self):
		self.__RefreshFileList()
		for BonusType in BonusListe:
			if BonusType == "":
				self.fileListBox.AppendItem(Item("bo�"))
			elif BonusType != "":
				self.fileListBox.AppendItem(Item(BonusType))
			#chat.AppendChat(chat.CHAT_TYPE_INFO, str(BonusIDListe[BonusType]))		
		
	def __RefreshFileList(self):
		self.fileListBox.RemoveAllItems()
		
	def SetBonis(self):
		global Bonus0
		global Bonus1
		global Bonus2
		global Bonus3
		global Bonus4
		global PRESSWISH0
		global PRESSWISH1
		global PRESSWISH2
		global PRESSWISH3
		global PRESSWISH4
		SelectedIndex = self.fileListBox.GetSelectedItem()
		SelectedIndex = SelectedIndex.GetText()
		if str(SelectedIndex) != "bo�" and str(SelectedIndex) != "":
			if PRESSWISH0 == 1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "1. efsun : " + str(SelectedIndex))
				Bonus0 = BonusIDListe[str(SelectedIndex)]
				PRESSWISH0 = 0
			elif PRESSWISH1 == 1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "2. efsun : " + str(SelectedIndex))
				Bonus1 = int(BonusIDListe[SelectedIndex])
				PRESSWISH1 = 0
			elif PRESSWISH2 == 1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "3. efsun : " + str(SelectedIndex))
				Bonus2 = int(BonusIDListe[SelectedIndex])
				PRESSWISH2 = 0
			elif PRESSWISH3 == 1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "4. efsun : " + str(SelectedIndex))
				Bonus3 = int(BonusIDListe[SelectedIndex])
				PRESSWISH3 = 0
			elif PRESSWISH4 == 1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "5. efsun : " + str(SelectedIndex))
				Bonus4 = int(BonusIDListe[SelectedIndex])
				PRESSWISH4 = 0
				
		elif str(SelectedIndex) == "bo�" and str(SelectedIndex) != "":
			if PRESSWISH0 == 1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "1. efsun silindi")
				Bonus0 = 0
				PRESSWISH0 = 0
			elif PRESSWISH1 == 1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "2. efsun silindi")
				Bonus1 = 0
				PRESSWISH1 = 0
			elif PRESSWISH2 == 1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "3. efsun silindi")
				Bonus2 = 0
				PRESSWISH2 = 0
			elif PRESSWISH3 == 1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "4. efsun silindi")
				Bonus3 = 0
				PRESSWISH3 = 0
			elif PRESSWISH4 == 1:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "5. efsun silindi")
				Bonus4 = 0
				PRESSWISH4 = 0	
				
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Hi� efsun se�memi�siniz?!")		
		self.Close()

class WaitingDialog(ui.ScriptWindow):

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.eventTimeOver = lambda *arg: None
		self.eventExit = lambda *arg: None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Open(self, waitTime):
		import time
		curTime = time.clock()
		self.endTime = curTime + waitTime

		self.Show()		

	def Close(self):
		self.Hide()

	def Destroy(self):
		self.Hide()

	def SAFE_SetTimeOverEvent(self, event):
		self.eventTimeOver = ui.__mem_func__(event)

	def SAFE_SetExitEvent(self, event):
		self.eventExit = ui.__mem_func__(event)
		
	def OnUpdate(self):
		import time
		lastTime = max(0, self.endTime - time.clock())
		if 0 == lastTime:
			self.Close()
			self.eventTimeOver()
		else:
			return
		
	def OnPressExitKey(self):
		self.Close()
		return TRUE
		
FILE_NAME_LEN = 40 

class Item(ui.ListBoxEx.Item):
	def __init__(self, fileName):
		ui.ListBoxEx.Item.__init__(self)
		self.canLoad=0
		self.text=fileName
		self.textLine=self.__CreateTextLine(fileName[:FILE_NAME_LEN])          

	def __del__(self):
		ui.ListBoxEx.Item.__del__(self)

	def GetText(self):
		return self.text

	def SetSize(self, width, height):
		ui.ListBoxEx.Item.SetSize(self, 6*len(self.textLine.GetText()) + 4, height)

	def __CreateTextLine(self, fileName):
		textLine=ui.TextLine()
		textLine.SetParent(self)
		textLine.SetPosition(0, 0)
		textLine.SetText(fileName)
		textLine.Show()
		return textLine
		
StartDialog = SwitchBotDialog()
StartDialog.Show()  
		