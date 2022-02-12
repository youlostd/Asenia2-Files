import ui
import snd
import shop
import mouseModule
import player
import chr
import net
import uiCommon
import localeInfo
import chat
import systemSetting
import item
import app
import constInfo

g_isBuildingOfflineShop = FALSE
g_itemPriceDict = {}
g_offlineShopAdvertisementBoardDict = {}

def Clear():
	global g_itemPriceDict
	global g_isBuildingOfflineShop
	g_itemPriceDict = {}
	g_isBuildingOfflineShop = FALSE
	global g_itemPriceChequeDict
	g_itemPriceChequeDict={}
	
def IsOfflineShopItemPriceList():
	global g_itemPriceDict
	global g_itemPriceChequeDict
	if g_itemPriceDict or g_itemPriceChequeDict:
		return True
	else:
		return FALSE
	
def IsBuildingOfflineShop():
	global g_isBuildingOfflineShop
	if (g_isBuildingOfflineShop):
		return TRUE
	else:
		return FALSE
		
def SetOfflineShopItemPrice(itemVNum, itemPrice, itemPriceCheque):
	global g_itemPriceDict
	g_itemPriceDict[int(itemVNum)]=itemPrice
	global g_itemPriceChequeDict
	g_itemPriceChequeDict[int(itemVNum)]=itemPriceCheque
		
def GetOfflineShopItemPrice(itemVNum):
	try:
		global g_itemPriceDict
		return g_itemPriceDict[itemVNum]
	except KeyError:
		return 0

def GetOfflineShopItemPriceCheque(itemVNum):
	try:
		global g_itemPriceChequeDict
		return g_itemPriceChequeDict[itemVNum]
	except KeyError:
		return 0
		

def UpdateADBoard():
	for key in g_offlineShopAdvertisementBoardDict.keys():
		g_offlineShopAdvertisementBoardDict[key].Show()
		
def DeleteADBoard():
	if (not g_offlineShopAdvertisementBoardDict.has_key(vid)):
		return
		
	del g_offlineShopAdvertisementBoardDict[vid]
	
class OfflineShopAdvertisementBoard(ui.ThinBoardNorm):
	def __init__(self):	
		ui.ThinBoardNorm.__init__(self, "UI_BOTTOM")
		self.vid = None
		self.__MakeTextLine()
		
	def __del__(self):
		ui.ThinBoardNorm.__del__(self)
		
	def __MakeTextLine(self):
		self.textLine = ui.TextLine()
		self.textLine.SetParent(self)
		self.textLine.SetWindowHorizontalAlignCenter()
		self.textLine.SetWindowVerticalAlignCenter()
		self.textLine.SetHorizontalAlignCenter()
		self.textLine.SetVerticalAlignCenter()
		self.textLine.SetPosition(0,-2)
		self.textLine.Show()
	
	def Open(self, vid, text):
		self.vid = vid
		#text = text[1:]
		self.textLine.SetText(text)
		self.textLine.UpdateRect()
		self.SetSize(len(text) * 6 + 80 * 2)
		self.Show()
		
		g_offlineShopAdvertisementBoardDict[vid] = self
		
	def OnMouseLeftButtonUp(self):
		if (not self.vid):
			return
			
		net.SendOnClickPacket(self.vid)
		return True
		
	def OnUpdate(self):
		if (not self.vid):
			return
		
		if (systemSetting.IsShowSalesText()):
			self.Show()
			(x, y) = chr.GetProjectPosition(self.vid, 220)
			self.SetPosition(x - self.GetWidth() / 2, y - self.GetHeight() / 2)
		else:
			for key in g_offlineShopAdvertisementBoardDict.keys():
				g_offlineShopAdvertisementBoardDict[key].Hide()

class OfflineShopAdvertisementBoard0(ui.ThinBoardFire):
	def __init__(self):	
		ui.ThinBoardFire.__init__(self, "UI_BOTTOM")
		self.vid = None
		self.__MakeTextLine()
		
	def __del__(self):
		ui.ThinBoardFire.__del__(self)
		
	def __MakeTextLine(self):
		self.textLine = ui.TextLine()
		self.textLine.SetParent(self)
		self.textLine.SetWindowHorizontalAlignCenter()
		self.textLine.SetWindowVerticalAlignCenter()
		self.textLine.SetHorizontalAlignCenter()
		self.textLine.SetVerticalAlignCenter()
		self.textLine.SetPosition(-8,6)
		self.textLine.Show()
	
	def Open(self, vid, text):
		self.vid = vid
		#text = text[1:]
		self.textLine.SetText(text)
		self.textLine.UpdateRect()
		self.SetSize(len(text) * 6 + 80 * 2)
		self.Show()
		
		g_offlineShopAdvertisementBoardDict[vid] = self
		
	def OnMouseLeftButtonUp(self):
		if (not self.vid):
			return
			
		net.SendOnClickPacket(self.vid)
		return True
		
	def OnUpdate(self):
		if (not self.vid):
			return
		
		if (systemSetting.IsShowSalesText()):
			self.Show()
			(x, y) = chr.GetProjectPosition(self.vid, 220)
			self.SetPosition(x - self.GetWidth() / 2, y - self.GetHeight() / 2)
		else:
			for key in g_offlineShopAdvertisementBoardDict.keys():
				g_offlineShopAdvertisementBoardDict[key].Hide()


class OfflineShopAdvertisementBoard1(ui.ThinBoardIce):
	def __init__(self):	
		ui.ThinBoardIce.__init__(self, "UI_BOTTOM")
		self.vid = None
		self.__MakeTextLine()
		
	def __del__(self):
		ui.ThinBoardIce.__del__(self)
		
	def __MakeTextLine(self):
		self.textLine = ui.TextLine()
		self.textLine.SetParent(self)
		self.textLine.SetWindowHorizontalAlignCenter()
		self.textLine.SetWindowVerticalAlignCenter()
		self.textLine.SetHorizontalAlignCenter()
		self.textLine.SetVerticalAlignCenter()
		self.textLine.SetPosition(-8,6)
		self.textLine.Show()
	
	def Open(self, vid, text):
		self.vid = vid
		#text = text[1:]
		self.textLine.SetText(text)
		self.textLine.UpdateRect()
		self.SetSize(len(text) * 6 + 80 * 2)
		self.Show()
		
		g_offlineShopAdvertisementBoardDict[vid] = self
		
	def OnMouseLeftButtonUp(self):
		if (not self.vid):
			return
			
		net.SendOnClickPacket(self.vid)
		return True
		
	def OnUpdate(self):
		if (not self.vid):
			return
		
		if (systemSetting.IsShowSalesText()):
			self.Show()
			(x, y) = chr.GetProjectPosition(self.vid, 220)
			self.SetPosition(x - self.GetWidth() / 2, y - self.GetHeight() / 2)
		else:
			for key in g_offlineShopAdvertisementBoardDict.keys():
				g_offlineShopAdvertisementBoardDict[key].Hide()

class OfflineShopAdvertisementBoard2(ui.ThinBoardPaper):
	def __init__(self):	
		ui.ThinBoardPaper.__init__(self, "UI_BOTTOM")
		self.vid = None
		self.__MakeTextLine()
		
	def __del__(self):
		ui.ThinBoardPaper.__del__(self)
		
	def __MakeTextLine(self):
		self.textLine = ui.TextLine()
		self.textLine.SetParent(self)
		self.textLine.SetWindowHorizontalAlignCenter()
		self.textLine.SetWindowVerticalAlignCenter()
		self.textLine.SetHorizontalAlignCenter()
		self.textLine.SetVerticalAlignCenter()
		self.textLine.SetPosition(-8,6)
		self.textLine.Show()
	
	def Open(self, vid, text):
		self.vid = vid
		#text = text[1:]
		self.textLine.SetText(text)
		self.textLine.UpdateRect()
		self.SetSize(len(text) * 6 + 80 * 2)
		self.Show()
		
		g_offlineShopAdvertisementBoardDict[vid] = self
		
	def OnMouseLeftButtonUp(self):
		if (not self.vid):
			return
			
		net.SendOnClickPacket(self.vid)
		return True
		
	def OnUpdate(self):
		if (not self.vid):
			return
		
		if (systemSetting.IsShowSalesText()):
			self.Show()
			(x, y) = chr.GetProjectPosition(self.vid, 220)
			self.SetPosition(x - self.GetWidth() / 2, y - self.GetHeight() / 2)
		else:
			for key in g_offlineShopAdvertisementBoardDict.keys():
				g_offlineShopAdvertisementBoardDict[key].Hide()
				
class OfflineShopAdvertisementBoard3(ui.ThinBoardRibon):
	def __init__(self):	
		ui.ThinBoardRibon.__init__(self, "UI_BOTTOM")
		self.vid = None
		self.__MakeTextLine()
		
	def __del__(self):
		ui.ThinBoardRibon.__del__(self)
		
	def __MakeTextLine(self):
		self.textLine = ui.TextLine()
		self.textLine.SetParent(self)
		self.textLine.SetWindowHorizontalAlignCenter()
		self.textLine.SetWindowVerticalAlignCenter()
		self.textLine.SetHorizontalAlignCenter()
		self.textLine.SetVerticalAlignCenter()
		self.textLine.SetPosition(-8,6)
		self.textLine.Show()
	
	def Open(self, vid, text):
		self.vid = vid
		#text = text[1:]
		self.textLine.SetText(text)
		self.textLine.UpdateRect()
		self.SetSize(len(text) * 6 + 80 * 2)
		self.Show()
		
		g_offlineShopAdvertisementBoardDict[vid] = self
		
	def OnMouseLeftButtonUp(self):
		if (not self.vid):
			return
			
		net.SendOnClickPacket(self.vid)
		return True
		
	def OnUpdate(self):
		if (not self.vid):
			return
		
		if (systemSetting.IsShowSalesText()):
			self.Show()
			(x, y) = chr.GetProjectPosition(self.vid, 220)
			self.SetPosition(x - self.GetWidth() / 2, y - self.GetHeight() / 2)
		else:
			for key in g_offlineShopAdvertisementBoardDict.keys():
				g_offlineShopAdvertisementBoardDict[key].Hide()

class OfflineShopAdvertisementBoard4(ui.ThinBoardWing):
	def __init__(self):	
		ui.ThinBoardWing.__init__(self, "UI_BOTTOM")
		self.vid = None
		self.__MakeTextLine()
		
	def __del__(self):
		ui.ThinBoardWing.__del__(self)
		
	def __MakeTextLine(self):
		self.textLine = ui.TextLine()
		self.textLine.SetParent(self)
		self.textLine.SetWindowHorizontalAlignCenter()
		self.textLine.SetWindowVerticalAlignCenter()
		self.textLine.SetHorizontalAlignCenter()
		self.textLine.SetVerticalAlignCenter()
		self.textLine.SetPosition(-8,6)
		self.textLine.Show()
	
	def Open(self, vid, text):
		self.vid = vid
		#text = text[1:]
		self.textLine.SetText(text)
		self.textLine.UpdateRect()
		self.SetSize(len(text) * 6 + 80 * 2)
		self.Show()
		
		g_offlineShopAdvertisementBoardDict[vid] = self
		
	def OnMouseLeftButtonUp(self):
		if (not self.vid):
			return
			
		net.SendOnClickPacket(self.vid)
		return True
		
	def OnUpdate(self):
		if (not self.vid):
			return
		
		if (systemSetting.IsShowSalesText()):
			self.Show()
			(x, y) = chr.GetProjectPosition(self.vid, 220)
			self.SetPosition(x - self.GetWidth() / 2, y - self.GetHeight() / 2)
		else:
			for key in g_offlineShopAdvertisementBoardDict.keys():
				g_offlineShopAdvertisementBoardDict[key].Hide()
#Shop Decoration

#Shop Decoration
			
class OfflineShopBuilder(ui.ScriptWindow):
	def __init__(self):	
		ui.ScriptWindow.__init__(self)
		self.LoadWindow()
		self.itemStock = {}
		self.tooltipItem = None
		self.priceInputBoard = None
		self.title = ""
		self.time = -1
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/OfflineShopBuilder.py")
		except:
			import exception
			exception.Abort("OfflineShopBuilderWindow.LoadWindow.LoadObject")
			
		try:
			self.nameLine = self.GetChild("NameLine")
			self.itemSlot = self.GetChild("ItemSlot")
			self.btnOk = self.GetChild("OkButton")
			self.btnClose = self.GetChild("CloseButton")
			self.board = self.GetChild("Board")
		except:
			import exception
			exception.Abort("OfflineShopBuilderWindow.LoadWindow.BindObject")
			
		self.btnOk.SetEvent(ui.__mem_func__(self.OnOk))
		self.btnClose.SetEvent(ui.__mem_func__(self.OnClose))
		self.board.SetCloseEvent(ui.__mem_func__(self.OnClose))
		
		self.itemSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.OnSelectEmptySlot))
		self.itemSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.OnSelectItemSlot))
		self.itemSlot.SetOverInItemEvent(ui.__mem_func__(self.OnOverInItem))
		self.itemSlot.SetOverOutItemEvent(ui.__mem_func__(self.OnOverOutItem))
	
	def Destroy(self):
		self.ClearDictionary()
		
		self.nameLine = None
		self.itemSlot = None
		self.btnOk = None
		self.btnClose = None
		self.board = None
		self.priceInputBoard = None
		
	def Open(self, title, time):
		self.title = title
		self.time = time
		
		if (len(title) > 25):
			self.title = title[:22] + "..."
			
		self.itemStock = {}
		shop.ClearOfflineShopStock()
		self.nameLine.SetText(title)
		self.SetCenterPosition()
		self.Refresh()
		self.Show()
	
		global g_isBuildingOfflineShop
		g_isBuildingOfflineShop = TRUE
		
	def Close(self):
		global g_isBuildingOfflineShop
		g_isBuildingOfflineShop = FALSE
		
		self.title = ""
		self.time = -1
		shop.ClearOfflineShopStock()
		self.Hide()
		if self.priceInputBoard:
			self.priceInputBoard.Close()
			self.priceInputBoard = None

			self.lockedItems = {i:(-1,-1) for i in range(shop.SHOP_SLOT_COUNT)}
			self.interface.SetOnTopWindow(player.ON_TOP_WND_NONE)
			self.interface.RefreshMarkInventoryBag()
		
	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem
		
	def Refresh(self):
		for i in xrange(shop.OFFLINE_SHOP_SLOT_COUNT):
			if (not self.itemStock.has_key(i)):
				self.itemSlot.ClearSlot(i)
				continue
				
			pos = self.itemStock[i]
			itemCount = player.GetItemCount(*pos)
			if (itemCount <= 1):
				itemCount = 0
			self.itemSlot.SetItemSlot(i, player.GetItemIndex(*pos), itemCount)
			
			self.itemSlot.EnableCoverButton(i)
			
		self.itemSlot.RefreshSlot()
	
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
			item.SelectItem(itemVNum)

			if item.IsAntiFlag(item.ANTIFLAG_GIVE) or item.IsAntiFlag(item.ANTIFLAG_MYSHOP):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OFFLINE_SHOP_CANNOT_SELL_ITEM)
				return

			priceInputBoard = uiCommon.MoneyInputDialog()
			priceInputBoard.SetTitle(localeInfo.OFFLINE_SHOP_INPUT_PRICE_DIALOG_TITLE)
			priceInputBoard.SetAcceptEvent(ui.__mem_func__(self.AcceptInputPrice))
			priceInputBoard.SetCancelEvent(ui.__mem_func__(self.CancelInputPrice))
			priceInputBoard.Open()

			
			itemPrice = GetOfflineShopItemPrice(itemVNum)
			if (itemPrice > 0):
				priceInputBoard.SetValue(itemPrice)
				
			if app.ENABLE_CHEQUE_SYSTEM:
				itemPriceCheque=GetOfflineShopItemPriceCheque(itemVNum)

				if itemPriceCheque>0:
					priceInputBoard.SetValueCheque(itemPriceCheque)
			
			self.priceInputBoard = priceInputBoard
			self.priceInputBoard.itemVNum = itemVNum
			self.priceInputBoard.sourceWindowType = attachedInvenType
			self.priceInputBoard.sourceSlotPos = attachedSlotPos
			self.priceInputBoard.targetSlotPos = selectedSlotPos

	def OnSelectItemSlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if (isAttached):
			snd.PlaySound("sound/ui/loginfail.wav")
			mouseModule.mouseController.DeattachObject()

		else:
			if (not selectedSlotPos in self.itemStock):
				return

			invenType, invenPos = self.itemStock[selectedSlotPos]
			shop.DelOfflineShopItemStock(invenType, invenPos)
			snd.PlaySound("sound/ui/drop.wav")

			del self.itemStock[selectedSlotPos]
			self.Refresh()
			
	def AcceptInputPrice(self):
		if (not self.priceInputBoard):
			return TRUE
			
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

		for privatePos, (itemWindowType, itemSlotIndex) in self.itemStock.items():
			if (itemWindowType == attachedInvenType and itemSlotIndex == sourceSlotPos):
				shop.DelOfflineShopItemStock(itemWindowType, itemSlotIndex)
				del self.itemStock[privatePos]
				
		if app.ENABLE_CHEQUE_SYSTEM:
			price = int(self.priceInputBoard.inputValue.GetText())
			price_cheque = int(self.priceInputBoard.InputValue_Cheque.GetText())

			# if IsOfflineShopItemPriceList():
			SetOfflineShopItemPrice(self.priceInputBoard.itemVNum, price, price_cheque)

			shop.AddOfflineShopItemStock(attachedInvenType, sourceSlotPos, targetSlotPos, price, price_cheque)
		self.itemStock[targetSlotPos] = (attachedInvenType, sourceSlotPos)
		snd.PlaySound("sound/ui/drop.wav")
		
		self.Refresh()
		self.priceInputBoard = None
		return TRUE
		
	def CancelInputPrice(self):

		if self.priceInputBoard:
			self.priceInputBoard.Close()
		self.priceInputBoard = None
		return 1
		
	def OnOk(self):
		if (not self.title):	
			return
		
		if (len(self.itemStock) == 0):
			return
		
		if (self.time < 0 or self.time == 0):
			return
			
		shop.BuildOfflineShop(self.title, self.time)
		self.Close()
		
	def OnClose(self):
		self.Close()
		
	def OnPressEscapeKey(self):
		self.Close()
		return TRUE
		
	def OnOverInItem(self, slotIndex):
		if (self.tooltipItem):
			if (self.itemStock.has_key(slotIndex)):
				self.tooltipItem.SetOfflineShopBuilderItem(*self.itemStock[slotIndex] + (slotIndex,))
				
	def OnOverOutItem(self):
		if (self.tooltipItem):
			self.tooltipItem.HideToolTip()

		def BindInterface(self, interface):
			self.interface = interface

		def OnTop(self):
			if self.interface:
				self.interface.SetOnTopWindow(player.ON_TOP_WND_PRIVATE_SHOP)
				self.interface.RefreshMarkInventoryBag()

		def SetInven(self, wndInventory):
			from _weakref import proxy
			self.wndInventory = proxy(wndInventory)
