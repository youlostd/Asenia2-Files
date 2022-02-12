import net
import player
import item
import snd
import shop
import wndMgr
import app
import chat
import chr

import ui
import uiCommon
import uiToolTip
import mouseModule
import localeInfo
import constInfo

g_isEditingOfflineShop = FALSE

def IsEditingOfflineShop():
	global g_isEditingOfflineShop
	if (g_isEditingOfflineShop):
		return TRUE
	else:
		return FALSE

###################################################################################################
## Offline Shop Admin Panel
class OfflineShopAdminPanelWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.isLoaded = 0
		self.wndOfflineShopChangePrice = None
		self.wndOfflineTezgahaBak = None
		self.wndOfflineShopMyBank = None
		self.closeQuestionDialog = None
		self.LoadWindow()
		self.LoadOtherWindows()
		
	def __del__(self):	
		ui.ScriptWindow.__del__(self)
		
	def Show(self):
		self.SetCenterPosition()
		self.SetTop()
		self.LoadWindow()
		self.userName.SetText(localeInfo.CT_ROOT_MERHABA + "[ " + player.GetName() + " ]")
		self.yazi1.SetText(localeInfo.CT_ROOT_YAZI_1)
		self.yazi2.SetText(localeInfo.CT_ROOT_YAZI_2)
		net.SendRefreshOfflineShopMoney()
		self.yazi3.SetText(localeInfo.CT_ROOT_YAZI_3 + "|cffFF6600" + localeInfo.NumberToMoneyString(player.GetCurrentOfflineShopMoney()) + " , " + localeInfo.NumberToWonString(player.GetCurrentOfflineShopCheque()))
		self.yazi4.SetText(localeInfo.CT_ROOT_YAZI_4)
		self.yazi5.SetText(localeInfo.CT_ROOT_YAZI_5)
		ui.ScriptWindow.Show(self)
		
	def LoadWindow(self):
		if (self.isLoaded == 1):
			return
			
		self.isLoaded = 1
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/OfflineShopAdminPanel.py")
		except:
			import exception
			exception.Abort("OfflineShopAdminPanelWindow.LoadWindow.LoadObject")
			
		try:
			self.board = self.GetChild("Board")
#			self.openOfflineShopButton = self.GetChild("OpenOfflineShopButton")
			self.closeOfflineShopButton = self.GetChild("CloseOfflineShopButton")
			self.tezgahabakevent = self.GetChild("TezgahaBakButton")
			self.tezgahabakevent1 = self.GetChild("TezgahaBakButton1")
			self.satilmayanlarButton = self.GetChild("SatilmayanlarButtons")
			self.myBankButton = self.GetChild("MyBankButton")
			self.userName = self.GetChild("UserName")
			self.yazi1 = self.GetChild("yazi1")
			self.yazi2 = self.GetChild("yazi2")
			self.yazi3 = self.GetChild("yazi3")
			self.yazi4 = self.GetChild("yazi4")
			self.yazi5 = self.GetChild("yazi5")
		except:
			import exception
			exception.Abort("OfflineShopAdminPanelWindow.LoadWindow.BindObject")
			
		self.board.SetCloseEvent(ui.__mem_func__(self.Close))
#		self.openOfflineShopButton.SetEvent(ui.__mem_func__(self.ClickOpenOfflineShopButton))
		self.closeOfflineShopButton.SetEvent(ui.__mem_func__(self.ClickCloseOfflineShopButton))
		self.myBankButton.SetEvent(ui.__mem_func__(self.ClickMyBankButton))
		self.tezgahabakevent.SetEvent(ui.__mem_func__(self.ClickTezgahaBak))
		self.tezgahabakevent1.SetEvent(ui.__mem_func__(self.ClickTezgahaBak1))
		self.satilmayanlarButton.SetEvent(ui.__mem_func__(self.ClickSatilmayanlar))
			
	def LoadOtherWindows(self):		
		wndOfflineShopMyBank = OfflineShopBankDialog()
		self.wndOfflineShopMyBank = wndOfflineShopMyBank
		
		wndOfflineShopChangePrice = OfflineShopChangePriceWindow()
		self.wndOfflineShopChangePrice = wndOfflineShopChangePrice

		wndOfflineTezgahaBak = OfflineTezgahaBak()
		self.wndOfflineTezgahaBak = wndOfflineTezgahaBak
		
#	def ClickOpenOfflineShopButton(self):
#		self.Close()
#		net.SendChatPacket("/open_offlineshop")
#		return TRUE
		
	def ClickCloseOfflineShopButton(self):
		self.Close()
		closeQuestionDialog = uiCommon.QuestionDialog()
		closeQuestionDialog.SetText(localeInfo.DO_YOU_WANT_TO_CLOSE_OFFLINE_SHOP)
		closeQuestionDialog.SetAcceptEvent(lambda arg = TRUE: self.AnswerCloseOfflineShop(arg))
		closeQuestionDialog.SetCancelEvent(lambda arg = FALSE: self.AnswerCloseOfflineShop(arg))
		closeQuestionDialog.Open()
		self.closeQuestionDialog = closeQuestionDialog
	
	def AnswerCloseOfflineShop(self, flag):
		if (flag):
			net.SendDestroyOfflineShop()
			shop.ClearOfflineShopStock()
		else:
			self.Show()
			
		self.closeQuestionDialog = None
		
	def ClickMyBankButton(self):
		self.Close()
		self.wndOfflineShopMyBank.Open()
		return TRUE

	def ClickTezgahaBak(self):
		self.Close()
		self.wndOfflineTezgahaBak.SetTop()
		self.wndOfflineTezgahaBak.SetCenterPosition()
		self.wndOfflineTezgahaBak.Open(player.GetName())
		return True
		
	def ClickTezgahaBak1(self):
		self.Close()
		self.wndOfflineTezgahaBak.SetTop()
		self.wndOfflineTezgahaBak.SetCenterPosition()
		self.wndOfflineTezgahaBak.Open(player.GetName())
		return True	
	
	def ClickSatilmayanlar(self):
		self.Close()
		net.SendChatPacket("/ct_satilmayanlar")
		
	def BindInterfaceClass(self, interface):
		self.interface = interface
	
	def Destroy(self):
		self.ClearDictionary()
		self.interface = None
		
	def Close(self):
		self.Hide()
		
	def OnPressEscapeKey(self):
		self.Close()
		return TRUE
		
	def OnPressExitKey(self):
		self.Close()
		return TRUE

	def OnUpdate(self):
		# All time update.
		try:
			if (app.GetTime() < self.updateTime + 5):
				return
			
			self.updateTime = app.GetTime()
			net.SendRefreshOfflineShopMoney()
			self.yazi3.SetText(localeInfo.CT_ROOT_YAZI_3 + "|cffFF6600" + localeInfo.NumberToMoneyString(player.GetCurrentOfflineShopMoney()))
		except:
			pass

###################################################################################################
## Offline Shop Change Price
class OfflineShopChangePriceWindow(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.LoadWindow()
		self.tooltipItem = None
		self.priceInputBoard = None
		self.title = ""
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def Refresh(self):
		net.SendRefreshOfflineShop()
		for i in xrange(shop.OFFLINE_SHOP_SLOT_COUNT):
			itemCount = shop.GetOfflineShopItemCount(i)
			if (itemCount <= 1):
				itemCount = 0
					
			self.itemSlot.SetItemSlot(i, shop.GetOfflineShopItemID(i))

			self.itemSlot.EnableCoverButton(i)
		
		wndMgr.RefreshSlot(self.itemSlot.GetWindowHandle())
		
	def SetItemData(self, pos, itemID, itemCount, itemPrice):
		shop.SetOfflineShopItemData(pos, itemID, itemCount, itemPrice)
		
	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/OfflineShopBuilder.py")
		except:
			import exception
			exception.Abort("OfflineShopChangePriceWindow.LoadWindow.LoadObject")
			
		try:
			self.nameLine = self.GetChild("NameLine")
			self.itemSlot = self.GetChild("ItemSlot")
			self.btnRefresh = self.GetChild("OkButton")
			self.btnClose = self.GetChild("CloseButton")
			self.board = self.GetChild("Board")
		except:
			import exception
			exception.Abort("OfflineShopAddItemWindow.LoadWindow.BindObject")
			
		self.btnRefresh.SetText(localeInfo.CT_ROOT_YENILE)
		self.btnClose.Hide()
		self.itemSlot.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
		self.itemSlot.SAFE_SetButtonEvent("RIGHT", "EXIST", self.UnselectItemSlot)
		self.itemSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.itemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))			
		self.btnRefresh.SetEvent(ui.__mem_func__(self.Refresh))
		self.board.SetCloseEvent(ui.__mem_func__(self.Close))
		
	def Destroy(self):
		self.ClearDictionary()
		
		self.nameLine = None
		self.itemSlot = None
		self.btnOk = None
		self.btnClose = None
		self.board = None
		self.priceInputBoard = None	
		
	def Open(self, title):
		self.title = title
		
		if (len(title) > 25):
			self.title = title[:22] + "..."
			
		self.tooltipItem = uiToolTip.ItemToolTip()
		self.tooltipItem.Hide()
		self.board.SetTitleName(localeInfo.CT_ROOT_FIYAT_DEGIS)	
		self.Refresh()
		self.Show()
		
		self.nameLine.SetText(title)
		global g_isEditingOfflineShop
		g_isEditingOfflineShop = True
		
	def Close(self):
		global g_isEditingOfflineShop
		g_isEditingOfflineShop = False
		
		self.title = ""
		self.Hide()
		
	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def OnPressExitKey(self):
		self.Close()
		return True
		
	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem
		
	def UnselectItemSlot(self, selectedSlotPos):
		if (constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1):
			return
			
		itemIndex = shop.GetOfflineShopItemID(selectedSlotPos)
		evo = shop.GetItemEvolution(selectedSlotPos)
		item.SelectItem(itemIndex)
		itemName = localeInfo.SILAH_EVRIM_TEXT[evolution] + item.GetItemName()
		
		priceInputBoard = uiCommon.MoneyInputDialog()
		priceInputBoard.SetTitle(itemName)
		priceInputBoard.SetAcceptEvent(ui.__mem_func__(self.AcceptInputPrice))
		priceInputBoard.SetCancelEvent(ui.__mem_func__(self.CancelInputPrice))
		priceInputBoard.Open()
		self.priceInputBoard = priceInputBoard
		self.priceInputBoard.pos = selectedSlotPos
		
	def AcceptInputPrice(self):
		if (not self.priceInputBoard):
			return True
			
		text = self.priceInputBoard.GetText()
		if (not text):
			return True
			
		if (not text.isdigit()):	
			return True
			
		if (int(text) <= 0):
			return True
			
		pos = self.priceInputBoard.pos
		price = int(self.priceInputBoard.GetText())
		net.SendChangePriceOfflineShopItem(pos, price)
		net.SendRefreshOfflineShop()
		self.priceInputBoard = None
		return True
		
	def CancelInputPrice(self):
		self.priceInputBoard = None
		return True
		
	def OverInItem(self, slotIndex):
		if (mouseModule.mouseController.isAttached()):
			return
			
		if (self.tooltipItem != 0):
			self.tooltipItem.SetOfflineShopItem(slotIndex)
			
	def OverOutItem(self):
		if (self.tooltipItem != 0):
			self.tooltipItem.HideToolTip()
			
	def OnUpdate(self):
		for i in xrange(shop.OFFLINE_SHOP_SLOT_COUNT):
			itemCount = shop.GetOfflineShopItemCount(i)
			if (itemCount <= 1):
				itemCount
				
			self.itemSlot.SetItemSlot(i, shop.GetOfflineShopItemID(i))
		
		wndMgr.RefreshSlot(self.itemSlot.GetWindowHandle())

###################################################################################################
## Tezgaha Bak
class OfflineTezgahaBak(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.LoadWindow()
		self.tooltipItem = None
		self.priceInputBoard = None
		self.title = ""
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def Refresh(self):
		net.SendRefreshOfflineShop()
		for i in xrange(shop.OFFLINE_SHOP_SLOT_COUNT):
			itemCount = shop.GetOfflineShopItemCount(i)
			if (itemCount <= 1):
				itemCount = 0
					
			self.itemSlot.SetItemSlot(i, shop.GetOfflineShopItemID(i))
			self.itemSlot.EnableCoverButton(i)
		
		wndMgr.RefreshSlot(self.itemSlot.GetWindowHandle())
		
	def SetItemData(self, pos, itemID, itemCount, itemPrice):
		shop.SetOfflineShopItemData(pos, itemID, itemCount, itemPrice)
		
	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/OfflineShopBuilder.py")
		except:
			import exception
			exception.Abort("OfflineShopChangePriceWindow.LoadWindow.LoadObject")
			
		try:
			self.nameLine = self.GetChild("NameLine")
			self.itemSlot = self.GetChild("ItemSlot")
			self.btnRefresh = self.GetChild("OkButton")
			self.btnClose = self.GetChild("CloseButton")
			self.board = self.GetChild("Board")
		except:
			import exception
			exception.Abort("OfflineShopAddItemWindow.LoadWindow.BindObject")
			
		self.btnRefresh.SetText(localeInfo.CT_ROOT_YENILE)
		self.btnClose.Hide()
		self.itemSlot.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
		#self.itemSlot.SAFE_SetButtonEvent("RIGHT", "EXIST", self.UnselectItemSlot)
		self.itemSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.OnSelectEmptySlot))
		self.itemSlot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.itemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))			
		self.btnRefresh.SetEvent(ui.__mem_func__(self.Refresh))
		self.board.SetCloseEvent(ui.__mem_func__(self.Close))
		
	def Destroy(self):
		self.ClearDictionary()
		
		self.nameLine = None
		self.itemSlot = None
		self.btnOk = None
		self.btnClose = None
		self.board = None
		self.priceInputBoard = None	
		
	def Open(self, title):
		self.title = title
		
		if (len(title) > 25):
			self.title = title[:22] + "..."
			
		self.tooltipItem = uiToolTip.ItemToolTip()
		self.tooltipItem.Hide()
		self.board.SetTitleName(localeInfo.CT_ROOT_TEZGAHIM)	
		self.Refresh()
		self.Show()
		
		self.nameLine.SetText(title)
		global g_isEditingOfflineShop
		g_isEditingOfflineShop = True
		
	def Close(self):
		global g_isEditingOfflineShop
		g_isEditingOfflineShop = False
		
		self.title = ""
		self.Hide()
		
	def OnPressEscapeKey(self):
		self.Close()
		return True
		
	def OnPressExitKey(self):
		self.Close()
		return True
		
	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem
		
	def OnSelectEmptySlot(self, selectedSlotPos):

		isAttached = mouseModule.mouseController.isAttached()
		if (isAttached):
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()

			if (player.SLOT_TYPE_INVENTORY != attachedSlotType and player.SLOT_TYPE_DRAGON_SOUL_INVENTORY != attachedSlotType and player.SLOT_TYPE_UPGRADE_INVENTORY != attachedSlotType and player.SLOT_TYPE_STONE_INVENTORY != attachedSlotType and player.SLOT_TYPE_CHEST_INVENTORY != attachedSlotType and player.SLOT_TYPE_ATTR_INVENTORY != attachedSlotType):
				return
				
			attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
				
			itemVNum = player.GetItemIndex(attachedInvenType, attachedSlotPos)
			#itemSealDate = player.GetItemSealDate(attachedInvenType, attachedSlotPos)
			item.SelectItem(itemVNum)
			
			#if itemSealDate == item.E_SEAL_DATE_UNLIMITED_TIMESTAMP:
				#chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OFFLINE_SHOP_CANNOT_SELL_ITEM)
				#return

			if item.IsAntiFlag(item.ANTIFLAG_GIVE) or item.IsAntiFlag(item.ANTIFLAG_MYSHOP):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OFFLINE_SHOP_CANNOT_SELL_ITEM)
				return

			priceInputBoard = uiCommon.MoneyInputDialog()
			priceInputBoard.SetTitle(localeInfo.OFFLINE_SHOP_INPUT_PRICE_DIALOG_TITLE)
			priceInputBoard.SetAcceptEvent(ui.__mem_func__(self.AcceptInputPrice))
			priceInputBoard.SetCancelEvent(ui.__mem_func__(self.CancelInputPrice))
			priceInputBoard.Open()
			
			self.priceInputBoard = priceInputBoard
			self.priceInputBoard.itemVNum = itemVNum
			self.priceInputBoard.sourceWindowType = attachedInvenType
			self.priceInputBoard.sourceSlotPos = attachedSlotPos
			self.priceInputBoard.targetSlotPos = selectedSlotPos
		
	# def OnSelectItemSlot(self, selectedSlotPos):
		# pass
		
	# def UnselectItemSlot(self, selectedSlotPos):
		# if (constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1):
			# return
			
		# itemIndex = shop.GetOfflineShopItemID(selectedSlotPos)
		# item.SelectItem(itemIndex)
		# itemName = item.GetItemName()
		
		# questionDialog = uiCommon.QuestionDialog()
		# questionDialog.SetText(localeInfo.DO_YOU_WANT_TO_REMOVE_ITEM % (itemName))
		# questionDialog.SetAcceptEvent(lambda arg = True : self.AnswerRemoveItem(arg))
		# questionDialog.SetCancelEvent(lambda arg = False : self.AnswerRemoveItem(arg))
		# questionDialog.Open()
		# questionDialog.pos = selectedSlotPos
		# self.questionDialog = questionDialog
		
		# constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)
		
	# def AnswerRemoveItem(self, flag):
		# if (flag):
			# pos = self.questionDialog.pos
			# net.SendRemoveOfflineShopItem(pos)
			# net.SendRefreshOfflineShop()
			
		# self.questionDialog.Close()
		# self.questionDialog = None
		# constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)
		# self.Refresh()

		
	def AcceptInputPrice(self):
		if (not self.priceInputBoard):
			return True
			
		if app.ENABLE_CHEQUE_SYSTEM:
			text = self.priceInputBoard.inputValue.GetText()
			text2 = self.priceInputBoard.InputValue_Cheque.GetText()

			if not text and not text2:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.YANG_VE_WON_MIKTARI_BOS_OLAMAZ)
				return True

			if not text.isdigit() and not text2.isdigit():
				# chat.AppendChat(chat.CHAT_TYPE_INFO, "Info: 02")
				return True

			if int(text) <= 0:
				if int(text2) > 0:
					pass
				else:
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.LUTFEN_YANG_MIKTARI_GIRINIZ)
					return True

		else:
			text = self.priceInputBoard.GetText()

			if not text:
				return True

			if not text.isdigit():
				return True

			if int(text) <= 0:
				return True
			
		attachedInvenType = self.priceInputBoard.sourceWindowType
		sourceSlotPos = self.priceInputBoard.sourceSlotPos
		targetSlotPos = self.priceInputBoard.targetSlotPos

				
		if app.ENABLE_CHEQUE_SYSTEM:
			price = int(self.priceInputBoard.inputValue.GetText())
			price_cheque = int(self.priceInputBoard.InputValue_Cheque.GetText())

		net.SendAddOfflineShopItem(attachedInvenType, sourceSlotPos, targetSlotPos, price, price_cheque)
		snd.PlaySound("sound/ui/drop.wav")
		
		self.Refresh()

		self.priceInputBoard = None
		return True
		
	def CancelInputPrice(self):
		self.priceInputBoard = None
		return True
		
	def OverInItem(self, slotIndex):
		if (mouseModule.mouseController.isAttached()):
			return
			
		if (self.tooltipItem != 0):
			self.tooltipItem.SetOfflineShopItem(slotIndex)
			
	def OverOutItem(self):
		if (self.tooltipItem != 0):
			self.tooltipItem.HideToolTip()
			
	# def OnUpdate(self):
		# for i in xrange(shop.OFFLINE_SHOP_SLOT_COUNT):
			# itemCount = shop.GetOfflineShopItemCount(i)
			# if (itemCount <= 1):
				# itemCount
			# self.itemSlot.SetItemSlot(i, shop.GetOfflineShopItemID(i),itemCount)
		# wndMgr.RefreshSlot(self.itemSlot.GetWindowHandle())	
		
###################################################################################################
## Offline Shop Bank Dialog
class OfflineShopBankDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.updateTime = 0
		self.withdrawMoneyTime = 0
		self.LoadWindow()
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/OfflineShop_BankDialog.py")
		except:
			import exception
			exception.Abort("OfflineShopBankDialog.LoadWindow.LoadScript")
			
		try:
			self.Board = self.GetChild("Board")
			self.currentMoneyLine = self.GetChild("CurrentMoneyLine")
			self.requiredMoneyLine = self.GetChild("RequiredMoneyLine")
			self.WithdrawMoneyButton = self.GetChild("withdraw_button")
			self.currentChequeLine = self.GetChild("CurrentChequeLine")
			self.requiredChequeLine = self.GetChild("RequiredChequeLine")
			self.CancelButton = self.GetChild("cancel_button")
		except:
			import exception
			exception.Abort("OfflineShopBankDialog.LoadWindow.BindObject")
			
		self.Board.SetCloseEvent(ui.__mem_func__(self.Close))
		self.CancelButton.SetEvent(ui.__mem_func__(self.Close))
		self.WithdrawMoneyButton.SetEvent(ui.__mem_func__(self.WithdrawMoney))
		
	def Close(self):
		self.currentMoneyLine.SetText("")
		self.requiredMoneyLine.SetText("")
		self.Hide()
		
	def Open(self):
		self.SetCenterPosition()
		self.SetTop()
		
		net.SendRefreshOfflineShopMoney()
		self.currentMoneyLine.SetText(localeInfo.NumberToMoneyString(player.GetCurrentOfflineShopMoney()))
		self.requiredMoneyLine.SetText("0")
		self.currentChequeLine.SetText(localeInfo.NumberToWonString(player.GetCurrentOfflineShopCheque()))
		self.requiredChequeLine.SetText("0")
		self.Show()
		
	def WithdrawMoney(self):
		try:
			currentMoney = player.GetCurrentOfflineShopMoney()
			requiredMoney = int(self.requiredMoneyLine.GetText())
			currentCheque = player.GetCurrentOfflineShopCheque()
			requiredCheque = int(self.requiredChequeLine.GetText())
			
			if (requiredMoney > currentMoney):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CT_ROOT_MEVCUT_PARANDAN_YUKSEK_MIKTAR)
				return
				
			if (requiredMoney < 0):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CT_ROOT_GECERLI_MIKTAR_GIR)
				return
				
			if (requiredCheque > currentCheque):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CT_ROOT_MEVCUT_PARANDAN_YUKSEK_MIKTAR)
				return
				
			if (requiredCheque < 0):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CT_ROOT_GECERLI_MIKTAR_GIR)
				return
				
			if (app.GetTime() < self.withdrawMoneyTime + 5):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CT_ROOT_TEKRAR_YANG_CEKMEK_ICIN_5_SANIYE_BEKLE)
				return
				
			if requiredMoney > 0:
				net.SendOfflineShopWithdrawMoney(requiredMoney)
			if requiredCheque > 0:
				net.SendOfflineShopWithdrawCheque(requiredCheque)
			# net.SendRefreshOfflineShopMoney()
			# self.currentMoneyLine.SetText(localeInfo.NumberToMoneyString(player.GetCurrentOfflineShopMoney()))
			self.withdrawMoneyTime = app.GetTime()
		except ValueError:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CT_ROOT_SADECE_RAKAM_KULLAN)
		
	def OnUpdate(self):
		# All time update.
		try:
			if (app.GetTime() < self.updateTime + 5):
				return
			
			self.updateTime = app.GetTime()
			net.SendRefreshOfflineShopMoney()
			self.currentMoneyLine.SetText(localeInfo.NumberToMoneyString(player.GetCurrentOfflineShopMoney()))
			self.currentChequeLine.SetText(localeInfo.NumberToWonString(player.GetCurrentOfflineShopCheque()))
		except:
			pass
	
###################################################################################################
## Offline Shop Input
class OfflineShopInputDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.hour = -1
		self.LoadWindow()
	
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def LoadWindow(self):
		try:
			PythonScriptLoader = ui.PythonScriptLoader()
			PythonScriptLoader.LoadScriptFile(self, "UIScript/OfflineShopInputDialog.py")
		except:
			import exception
			exception.Abort("OfflineShopInputDialog.LoadWindow.LoadObject")
			
		try:
			self.acceptButton = self.GetChild("AgreeButton")
			self.cancelButton = self.GetChild("CancelButton")
			self.inputSlot = self.GetChild("InputSlot")
			self.inputValue = self.GetChild("InputValue")
			#Shop Decoration
			self.buttonDecoration = self.GetChild("DecorationButton")
			self.buttonDecoration.SetEvent(ui.__mem_func__(self.OpenDecoration))
			#Shop Decoration			
			
			self.hourDict = {
				0 : self.GetChild("oneHour"),
				1 : self.GetChild("twoHours"),
				2 : self.GetChild("fourHours"),
				3 : self.GetChild("unlimited"),
			}
		except:
			import exception
			exception.Abort("OfflineShopInputDialog.LoadWindow.BindObject")
			
		for (tabKey, tabButton) in self.hourDict.items():
			tabButton.SetEvent(ui.__mem_func__(self.SelectHour), tabKey)
			
	def Open(self):
		self.inputValue.SetFocus()
		self.SetCenterPosition()
		self.SetTop()
		self.Show()
	
	def Close(self):
		self.ClearDictionary()
		self.acceptButton = None
		self.cancelButton = None
		self.inputSlot = None
		self.inputValue = None
		self.Hide()
		
	def SelectHour(self, hour):
		if (self.hour == hour):
			return
			
		self.hour = hour
		for (tabKey, tabButton) in self.hourDict.items():
			if (tabKey != hour):
				tabButton.SetUp()
				
		self.hourDict[self.hour].Down()
		
	def SetAcceptEvent(self, event):
		self.acceptButton.SetEvent(event)
		self.inputValue.OnIMEReturn = event
	#Shop Decoration
	def OpenDecoration(self):
		net.SendChatPacket("/open_decoration")
	#Shop Decoration
		
	def SetCancelEvent(self, event):
		self.cancelButton.SetEvent(event)
		self.inputValue.OnPressEscapeKey = event
		
	def GetTitle(self):
		return self.inputValue.GetText()
		
	def GetTime(self):
		return self.hour + 1
		
###################################################################################################
## Offline Shop
class OfflineShopDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.tooltipItem = 0
		self.xShopStart = 0
		self.yShopStart = 0
		self.questionDialog = None
		self.popup = None
		self.itemBuyQuestionDialog = None

		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def Refresh(self):
		for i in xrange(shop.OFFLINE_SHOP_SLOT_COUNT):
			itemCount = shop.GetOfflineShopItemCount(i)
			if (itemCount <= 1):
				itemCount = 0
			self.itemSlotWindow.SetItemSlot(i, shop.GetOfflineShopItemID(i), itemCount)
			self.itemSlotWindow.EnableCoverButton(i)
			
		wndMgr.RefreshSlot(self.itemSlotWindow.GetWindowHandle())
		
	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/OfflineShopDialog.py")
		except:
			import exception
			exception.Abort("OfflineShopDialog.LoadDialog.LoadObject")
			
		try:
			self.itemSlotWindow = self.GetChild("ItemSlot")
			self.btnBuy = self.GetChild("BuyButton")
			self.titleBar = self.GetChild("TitleBar")
			self.titleName = self.GetChild("TitleName")
		except:	
			import exception
			exception.Abort("OfflineShopDialog.LoadDialog.BindObject")			
			
		self.itemSlotWindow.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
		self.itemSlotWindow.SAFE_SetButtonEvent("LEFT", "EXIST", self.SelectItemSlot)
		self.itemSlotWindow.SAFE_SetButtonEvent("RIGHT", "EXIST", self.UnselectItemSlot)
		
		self.itemSlotWindow.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		self.itemSlotWindow.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		
		self.btnBuy.SetToggleUpEvent(ui.__mem_func__(self.CancelShopping))
		self.btnBuy.SetToggleDownEvent(ui.__mem_func__(self.OnBuy))
		
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))			
		self.Refresh()
			
	def Destroy(self):
		self.Close()
		self.ClearDictionary()
		
		self.tooltipItem = 0
		self.itemSlotWindow = 0
		self.btnBuy = 0
		self.titleBar = 0
		self.questionDialog = None
		self.popup = None
		
	def Open(self, vid):
		shop.Open(FALSE, FALSE, TRUE)
		self.Refresh()
		self.SetTop()
		self.Show()
		
		# Set Title Name
		self.titleName.SetText(chr.GetNameByVID(vid))		
		
		(self.xShopStart, self.yShopStart, z) = player.GetMainCharacterPosition()

	def Close(self):
			if self.itemBuyQuestionDialog:
				self.itemBuyQuestionDialog.Close()
				self.itemBuyQuestionDialog = None
				constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

			self.OnCloseQuestionDialog()
			shop.Close()
			net.SendOfflineShopEndPacket()
			self.CancelShopping()
			self.tooltipItem.HideToolTip()
			self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE
		
	def OnPressExitKey(self):
		self.Close()
		return TRUE
		
	def OnBuy(self):
		# chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OFFLINE_SHOP_WARNING1)
		# chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OFFLINE_SHOP_WARNING2)
		app.SetCursor(app.BUY)
		
	def CancelShopping(self):
		self.btnBuy.SetUp()
		app.SetCursor(app.NORMAL)
		
	def OnCloseQuestionDialog(self):
		if (not self.questionDialog):
			return
			
		self.questionDialog.Close()
		self.questionDialog = None
		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)
		
	def UnselectItemSlot(self, selectedSlotPos):
		if (constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1):
			return
			
		self.AskBuyItem(selectedSlotPos)
		
	def SelectItemSlot(self, selectedSlotPos):
		if (constInfo.GET_ITEM_QUESTION_DIALOG_STATUS() == 1):
			return
			
		isAttached = mouseModule.mouseController.isAttached()
		if (not isAttached):
			curCursorNum = app.GetCursor()
			if (app.BUY == curCursorNum):
				net.SendOfflineShopBuyPacket(selectedSlotPos)
			else:
				selectedItemID = shop.GetOfflineShopItemID(selectedSlotPos)
				itemCount = shop.GetOfflineShopItemCount(selectedSlotPos)
				
				type = player.SLOT_TYPE_OFFLINE_SHOP
				mouseModule.mouseController.AttachObject(self, type, selectedSlotPos, selectedItemID, itemCount)
				mouseModule.mouseController.SetCallBack("INVENTORY", ui.__mem_func__(self.DropToInventory))
				snd.PlaySound("sound/ui/pick.wav")

	def DropToInventory(self):
		attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
		self.AskBuyItem(attachedSlotPos)

	def AskBuyItem(self, slotPos):
		itemIndex = shop.GetOfflineShopItemID(slotPos)
		itemPrice = shop.GetOfflineShopItemPrice(slotPos)
		itemPriceCheque = shop.GetOfflineShopItemPriceCheque(slotPos)
		itemCount = shop.GetOfflineShopItemCount(slotPos)
		evolution = shop.GetItemEvolution(slotPos)
		price_cheque = localeInfo.NumberToWonString(itemPriceCheque)
		price = localeInfo.NumberToMoneyString(itemPrice)

		item.SelectItem(itemIndex)
		itemName = localeInfo.SILAH_EVRIM_TEXT[evolution] + item.GetItemName()

		itemBuyQuestionDialog = uiCommon.QuestionDialog()
		itemBuyQuestionDialog.SetText(localeInfo.DO_YOU_BUY_ITEM_CHEQUE(itemName, itemCount, price_cheque, price))
		itemBuyQuestionDialog.SetAcceptEvent(lambda arg=TRUE: self.AnswerBuyItem(arg))
		itemBuyQuestionDialog.SetCancelEvent(lambda arg=FALSE: self.AnswerBuyItem(arg))
		itemBuyQuestionDialog.Open()
		itemBuyQuestionDialog.pos = slotPos
		self.itemBuyQuestionDialog = itemBuyQuestionDialog
		
		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)
		
	def AnswerBuyItem(self, flag):
		if (flag):
			pos = self.itemBuyQuestionDialog.pos
			net.SendOfflineShopBuyPacket(pos)

		self.itemBuyQuestionDialog.Close()
		self.itemBuyQuestionDialog = None
		
		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)		
		
	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem
		
	def OverInItem(self, slotIndex):
		if (mouseModule.mouseController.isAttached()):
			return
			
		if (self.tooltipItem != 0):
			self.tooltipItem.SetOfflineShopItem(slotIndex)
			
	def OverOutItem(self):
		if (self.tooltipItem != 0):
			self.tooltipItem.HideToolTip()
		
	def OnUpdate(self):
		USE_SHOP_LIMIT_RANGE = 1500
		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.xShopStart) > USE_SHOP_LIMIT_RANGE or abs(y - self.yShopStart) > USE_SHOP_LIMIT_RANGE:
			self.Close()


		def OnTop(self):
				self.interface.SetOnTopWindow(player.ON_TOP_WND_SHOP)
				self.interface.RefreshMarkInventoryBag()
