import player
import exchange
import net
import localeInfo
import chat
import item
import playerSettingModule
import ui
import mouseModule
import uiPickMoney
import wndMgr
import constInfo
import app


###################################################################################################
## Exchange
if app.__ENABLE_NEW_OFFLINESHOP__:
	import uinewofflineshop



class ExchangeDialog(ui.ScriptWindow):
	FACE_IMAGE_DICT = {
						playerSettingModule.RACE_WARRIOR_M : "icon/face/warrior_m.tga",
						playerSettingModule.RACE_WARRIOR_W : "icon/face/warrior_w.tga",
						playerSettingModule.RACE_ASSASSIN_M : "icon/face/assassin_m.tga",
						playerSettingModule.RACE_ASSASSIN_W : "icon/face/assassin_w.tga",
						playerSettingModule.RACE_SURA_M : "icon/face/sura_m.tga",
						playerSettingModule.RACE_SURA_W : "icon/face/sura_w.tga",
						playerSettingModule.RACE_SHAMAN_M : "icon/face/shaman_m.tga",
						playerSettingModule.RACE_SHAMAN_W : "icon/face/shaman_w.tga",
						playerSettingModule.RACE_WOLFMAN_M : "icon/face/wolfman_m.tga",
	}

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.TitleName = 0
		self.tooltipItem = 0
		self.xStart = 0
		self.yStart = 0

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	class Item(ui.ListBoxEx.Item):
		def __init__(self,parent, text, value=0):
			ui.ListBoxEx.Item.__init__(self)
			self.textBox=ui.TextLine()
			self.textBox.SetParent(self)
			self.textBox.SetText(text)
			self.textBox.Show()
			self.value = value

		def GetValue(self):
			return self.value

		def __del__(self):
			ui.ListBoxEx.Item.__del__(self)
	def LoadDialog(self):
		PythonScriptLoader = ui.PythonScriptLoader()
		if app.ENABLE_NEW_EXCHANGE_WINDOW:
			PythonScriptLoader.LoadScriptFile(self, "UIScript/exchangedialog_new.py")
		else:
			PythonScriptLoader.LoadScriptFile(self, "UIScript/exchangedialog.py")

		## Owner
		self.OwnerSlot = self.GetChild("Owner_Slot")
		self.OwnerSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectOwnerEmptySlot))
		self.OwnerSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectOwnerItemSlot))
		self.OwnerSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInOwnerItem))
		self.OwnerSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		self.OwnerSlot.SetUnselectItemSlotEvent(ui.__mem_func__(self.RightClickItemSlot))
		self.OwnerMoney = self.GetChild("Owner_Money_Value")
		# self.OwnerAcceptLight = self.GetChild("Owner_Accept_Light")
		# self.OwnerAcceptLight.Disable()
		if app.ENABLE_CHEQUE_SYSTEM:
			self.OwnerCheque = self.GetChild("Owner_Cheque_Value")
			self.OwnerChequeButton = self.GetChild("Owner_Cheque")
			self.OwnerChequeButton.SetEvent(ui.__mem_func__(self.OpenPickMoneyDialog))
		self.OwnerMoneyButton = self.GetChild("Owner_Money")
		self.OwnerMoneyButton.SetEvent(ui.__mem_func__(self.OpenPickMoneyDialog))

		## Target
		self.TargetSlot = self.GetChild("Target_Slot")
		self.TargetSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInTargetItem))
		self.TargetSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		self.TargetMoney = self.GetChild("Target_Money_Value")
		if app.ENABLE_CHEQUE_SYSTEM:
			self.TargetCheque = self.GetChild("Target_Cheque_Value")
		# self.TargetAcceptLight = self.GetChild("Target_Accept_Light")
		self.TargetAcceptLight = self.GetChild("Target_Accept_Button")
		self.TargetAcceptLight.Disable()

		## PickMoneyDialog
		dlgPickMoney = uiPickMoney.PickMoneyDialog()
		dlgPickMoney.LoadDialog()
		dlgPickMoney.SetAcceptEvent(ui.__mem_func__(self.OnPickMoney))
		dlgPickMoney.SetTitleName(localeInfo.EXCHANGE_MONEY)
		dlgPickMoney.SetMax(15)
		dlgPickMoney.Hide()
		self.dlgPickMoney = dlgPickMoney

		## Button
		self.AcceptButton = self.GetChild("Owner_Accept_Button")
		self.AcceptButton.SetToggleDownEvent(ui.__mem_func__(self.AcceptExchange))

		self.TitleName = self.GetChild("TitleName")
		self.GetChild("TitleBar").SetCloseEvent(net.SendExchangeExitPacket)


		# if app.ENABLE_NEW_EXCHANGE_WINDOW:
			# self.FaceOwnerImage = self.GetChild("FaceOwner_Image")
			# self.FaceTargetImage = self.GetChild("FaceTarget_Image")
			# self.TargetName = self.GetChild("target_NameText")
			# self.TargetLevel = self.GetChild("target_LvText")
			# self.ExchangeLogs = self.GetChild("ExchangeLogs")
			# self.LogsScrollBar = ui.ThinScrollBar()
			# self.LogsScrollBar.SetParent(self.ExchangeLogs)
			# self.LogsScrollBar.SetPosition(442 - 75, 17)
			# self.LogsScrollBar.SetScrollBarSize(50)
			# self.LogsScrollBar.Show()
			# self.LogsDropList = ui.ListBoxEx()
			# self.LogsDropList.SetParent(self.ExchangeLogs)
			# self.LogsDropList.itemHeight = 12
			# self.LogsDropList.itemStep = 13
			# self.LogsDropList.SetPosition(35, 27)
			# self.LogsDropList.SetSize(0, 45)
			# self.LogsDropList.SetScrollBar(self.LogsScrollBar)
			# self.LogsDropList.SetViewItemCount(2)
			# self.LogsDropList.Show()
			# self.LogsScrollBar.Show()
			# self.listOwnerSlot = []
			# self.listTargetSlot = []


	def Destroy(self):
		print "---------------------------------------------------------------------------- DESTROY EXCHANGE"
		self.ClearDictionary()
		self.dlgPickMoney.Destroy()
		self.dlgPickMoney = 0
		self.OwnerSlot = 0
		self.OwnerMoney = 0
		self.OwnerAcceptLight = 0
		self.OwnerMoneyButton = 0
		self.TargetSlot = 0
		self.TargetMoney = 0
		self.TargetAcceptLight = 0
		self.TitleName = 0
		self.AcceptButton = 0
		self.tooltipItem = 0
		if app.ENABLE_CHEQUE_SYSTEM:
			self.OwnerCheque = 0
			self.OwnerChequeButton = 0
			self.TargetCheque = 0

	def OpenDialog(self):
		self.TitleName.SetText(localeInfo.EXCHANGE_TITLE % (exchange.GetNameFromTarget()))
		if exchange.GetNameFromTarget() == "":
			import constInfo
			self.TitleName.SetText(localeInfo.EXCHANGE_TITLE % (str(constInfo.NegotFereastraNume)))
		self.AcceptButton.Enable()
		self.AcceptButton.SetUp()
		self.Show()
		(self.xStart, self.yStart, z) = player.GetMainCharacterPosition()

	def CloseDialog(self):
		wndMgr.OnceIgnoreMouseLeftButtonUpEvent()

		if 0 != self.tooltipItem:
			self.tooltipItem.HideToolTip()

		self.dlgPickMoney.Close()
		self.Hide()
		constInfo.mainNegDistancia = 0

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem


	def RightClickItemSlot(self, SlotIndex):
		net.SendExchangeItemDelPacket(SlotIndex)



	def OpenPickMoneyDialog(self):
		if app.ENABLE_CHEQUE_SYSTEM:
			if exchange.GetElkFromSelf() > 0 or exchange.GetWonFromSelf() > 0:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXCHANGE_CANT_EDIT_MONEY)
				return
			self.dlgPickMoney.Open(player.GetElk(), player.GetWon())
		else:
			if exchange.GetElkFromSelf() > 0:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXCHANGE_CANT_EDIT_MONEY)
				return
			self.dlgPickMoney.Open(player.GetElk())

	def OnPickMoney(self, money, cheque):
		if app.ENABLE_CHEQUE_SYSTEM:
			net.SendExchangeElkAddPacket(long(money))
			net.SendExchangeWonAddPacket(cheque)
		else:
			net.SendExchangeElkAddPacket(long(money))

	def AcceptExchange(self):
		net.SendExchangeAcceptPacket()
		self.AcceptButton.Disable()

	def SelectOwnerEmptySlot(self, SlotIndex):
		if FALSE == mouseModule.mouseController.isAttached():
			return
		if app.ENABLE_CHEQUE_SYSTEM:
			if mouseModule.mouseController.IsAttachedMoney():
				net.SendExchangeElkAddPacket(mouseModule.mouseController.GetAttachedMoneyAmount())
			elif mouseModule.mouseController.IsAttachedCheque():
				net.SendExchangeWonAddPacket(mouseModule.mouseController.GetAttachedChequeAmount())
			else:
				attachedSlotType = mouseModule.mouseController.GetAttachedType()
				if (player.SLOT_TYPE_INVENTORY == attachedSlotType
					or player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedSlotType):
					attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
					SrcSlotNumber = mouseModule.mouseController.GetAttachedSlotNumber()
					DstSlotNumber = SlotIndex
					itemID = player.GetItemIndex(attachedInvenType, SrcSlotNumber)
					item.SelectItem(itemID)
					if item.IsAntiFlag(item.ANTIFLAG_GIVE):
						chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXCHANGE_CANNOT_GIVE)
						mouseModule.mouseController.DeattachObject()
						return
					net.SendExchangeItemAddPacket(attachedInvenType, SrcSlotNumber, DstSlotNumber)
			if app.ENABLE_SPECIAL_STORAGE:
				if player.SLOT_TYPE_UPGRADE_INVENTORY == attachedSlotType or\
					player.SLOT_TYPE_STONE_INVENTORY == attachedSlotType or\
					player.SLOT_TYPE_CHEST_INVENTORY == attachedSlotType or\
					player.SLOT_TYPE_ATTR_INVENTORY == attachedSlotType:
					attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
					SrcSlotNumber = mouseModule.mouseController.GetAttachedSlotNumber()
					DstSlotNumber = SlotIndex
	
					itemID = player.GetItemIndex(attachedSlotType, SrcSlotNumber)
					item.SelectItem(itemID)
					
					if item.IsAntiFlag(item.ANTIFLAG_GIVE):
						chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXCHANGE_CANNOT_GIVE)
						mouseModule.mouseController.DeattachObject()
						return
						
					net.SendExchangeItemAddPacket(attachedInvenType, SrcSlotNumber, DstSlotNumber)
			mouseModule.mouseController.DeattachObject()
		else:
			if mouseModule.mouseController.IsAttachedMoney():
				net.SendExchangeElkAddPacket(mouseModule.mouseController.GetAttachedMoneyAmount())
			else:
				attachedSlotType = mouseModule.mouseController.GetAttachedType()
				if (player.SLOT_TYPE_INVENTORY == attachedSlotType
					or player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedSlotType):
					attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
				if app.__ENABLE_NEW_OFFLINESHOP__:
					if uinewofflineshop.IsBuildingShop() and uinewofflineshop.IsSaleSlot(attachedInvenType, mouseModule.mouseController.GetAttachedSlotNumber()):
						chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OFFLINESHOP_CANT_SELECT_ITEM_DURING_BUILING)
						return
				
					SrcSlotNumber = mouseModule.mouseController.GetAttachedSlotNumber()
					DstSlotNumber = SlotIndex
					itemID = player.GetItemIndex(attachedInvenType, SrcSlotNumber)
					item.SelectItem(itemID)
					if item.IsAntiFlag(item.ANTIFLAG_GIVE):
						chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.EXCHANGE_CANNOT_GIVE)
						mouseModule.mouseController.DeattachObject()
						return
					net.SendExchangeItemAddPacket(attachedInvenType, SrcSlotNumber, DstSlotNumber)
			mouseModule.mouseController.DeattachObject()

	def SelectOwnerItemSlot(self, SlotIndex):
		if player.ITEM_MONEY == mouseModule.mouseController.GetAttachedItemIndex():
			money = mouseModule.mouseController.GetAttachedItemCount()
			net.SendExchangeElkAddPacket(money)
		if app.ENABLE_CHEQUE_SYSTEM:
			if player.ITEM_CHEQUE == mouseModule.mouseController.GetAttachedItemIndex():
				cheque = mouseModule.mouseController.GetAttachedItemCount()
				net.SendExchangeWonAddPacket(cheque)

	def RefreshOwnerSlot(self):
		for i in xrange(exchange.EXCHANGE_ITEM_MAX_NUM):
			itemIndex = exchange.GetItemVnumFromSelf(i)
			itemCount = exchange.GetItemCountFromSelf(i)
			if 1 == itemCount:
				itemCount = 0
			self.OwnerSlot.SetItemSlot(i, itemIndex, itemCount)
		self.OwnerSlot.RefreshSlot()

	def RefreshTargetSlot(self):
		for i in xrange(exchange.EXCHANGE_ITEM_MAX_NUM):
			itemIndex = exchange.GetItemVnumFromTarget(i)
			itemCount = exchange.GetItemCountFromTarget(i)
			if 1 == itemCount:
				itemCount = 0
			self.TargetSlot.SetItemSlot(i, itemIndex, itemCount)
		self.TargetSlot.RefreshSlot()

	def Refresh(self):
		self.RefreshOwnerSlot()
		self.RefreshTargetSlot()
		self.OwnerMoney.SetText(str(localeInfo.NumberToMoneyString(exchange.GetElkFromSelf())))
		self.TargetMoney.SetText(str(localeInfo.NumberToMoneyString(exchange.GetElkFromTarget())))
		if app.ENABLE_CHEQUE_SYSTEM:
			self.OwnerCheque.SetText(str(localeInfo.NumberToWonString(exchange.GetWonFromSelf())))
			self.TargetCheque.SetText(str(localeInfo.NumberToWonString(exchange.GetWonFromTarget())))
		if TRUE == exchange.GetAcceptFromSelf():
			self.AcceptButton.Enable()
			self.AcceptButton.SetUp()

		if TRUE == exchange.GetAcceptFromTarget():
			self.TargetAcceptLight.Down()
		else:
			self.TargetAcceptLight.SetUp()

	def OverInOwnerItem(self, slotIndex):

		if 0 != self.tooltipItem:
			self.tooltipItem.SetExchangeOwnerItem(slotIndex)

	def OverInTargetItem(self, slotIndex):

		if 0 != self.tooltipItem:
			self.tooltipItem.SetExchangeTargetItem(slotIndex)

	def OverOutItem(self):

		if 0 != self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OnTop(self):
		self.tooltipItem.SetTop()

	def OnUpdate(self):

		USE_EXCHANGE_LIMIT_RANGE = 1000

		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.xStart) > USE_EXCHANGE_LIMIT_RANGE or abs(y - self.yStart) > USE_EXCHANGE_LIMIT_RANGE:
			(self.xStart, self.yStart, z) = player.GetMainCharacterPosition()
			net.SendExchangeExitPacket()
