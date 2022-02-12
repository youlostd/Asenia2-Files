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
import item
import systemSetting
import player
import app
import constInfo

g_isBuildingPrivateShop = False

g_itemPriceDict={}
if app.ENABLE_CHEQUE_SYSTEM:
	g_itemPriceChequeDict={}

g_privateShopAdvertisementBoardDict={}

def Clear():
	global g_itemPriceDict
	global g_isBuildingPrivateShop
	g_itemPriceDict={}
	g_isBuildingPrivateShop = False
	if app.ENABLE_CHEQUE_SYSTEM:
		global g_itemPriceChequeDict
		g_itemPriceChequeDict={}

if app.ENABLE_CHEQUE_SYSTEM:
	def IsPrivateShopItemPriceList():
		global g_itemPriceDict
		global g_itemPriceChequeDict
		if g_itemPriceDict or g_itemPriceChequeDict:
			return True
		else:
			return False
else:
	def IsPrivateShopItemPriceList():
		global g_itemPriceDict
		if g_itemPriceDict:
			return True
		else:
			return False

def IsBuildingPrivateShop():
	global g_isBuildingPrivateShop
	if player.IsOpenPrivateShop() or g_isBuildingPrivateShop:
		return True
	else:
		return False

if app.ENABLE_CHEQUE_SYSTEM:
	def SetPrivateShopItemPrice(itemVNum, itemPrice, itemPriceCheque):
		global g_itemPriceDict
		g_itemPriceDict[int(itemVNum)]=itemPrice
		global g_itemPriceChequeDict
		g_itemPriceChequeDict[int(itemVNum)]=itemPriceCheque
		
	def GetPrivateShopItemPrice(itemVNum):
		try:
			global g_itemPriceDict
			return g_itemPriceDict[itemVNum]
		except KeyError:
			return 0

	def GetPrivateShopItemPriceCheque(itemVNum):
		try:
			global g_itemPriceChequeDict
			return g_itemPriceChequeDict[itemVNum]
		except KeyError:
			return 0
else:
	def SetPrivateShopItemPrice(itemVNum, itemPrice):
		global g_itemPriceDict
		g_itemPriceDict[int(itemVNum)]=itemPrice
		
	def GetPrivateShopItemPrice(itemVNum):
		try:
			global g_itemPriceDict
			return g_itemPriceDict[itemVNum]
		except KeyError:
			return 0
		
def UpdateADBoard():	
	for key in g_privateShopAdvertisementBoardDict.keys():
		g_privateShopAdvertisementBoardDict[key].Show()
		
def DeleteADBoard(vid):
	if not g_privateShopAdvertisementBoardDict.has_key(vid):
		return
			
	del g_privateShopAdvertisementBoardDict[vid]
		

class PrivateShopAdvertisementBoard(ui.ThinBoard):
	def __init__(self):
		ui.ThinBoard.__init__(self, "UI_BOTTOM")
		self.vid = None
		self.__MakeTextLine()

	def __del__(self):
		ui.ThinBoard.__del__(self)

	def __MakeTextLine(self):
		self.textLine = ui.TextLine()
		self.textLine.SetParent(self)
		self.textLine.SetWindowHorizontalAlignCenter()
		self.textLine.SetWindowVerticalAlignCenter()
		self.textLine.SetHorizontalAlignCenter()
		self.textLine.SetVerticalAlignCenter()
		self.textLine.Show()

	def Open(self, vid, text):
		self.vid = vid

		self.textLine.SetText(text)
		self.textLine.UpdateRect()
		self.SetSize(len(text)*6 + 10*2, 20)
		self.Show() 
				
		g_privateShopAdvertisementBoardDict[vid] = self
		
	def OnMouseLeftButtonUp(self):
		if not self.vid:
			return
		net.SendOnClickPacket(self.vid)
		
		return True
		
	def OnUpdate(self):
		if not self.vid:
			return

		if systemSetting.IsShowSalesText():
			if not app.ENABLE_SHOPNAMES_RANGE:
				self.Show()
				(x, y) = chr.GetProjectPosition(self.vid, 220)
				self.SetPosition(x - self.GetWidth() / 2, y - self.GetHeight() / 2)
			else:
				if systemSetting.GetShopNamesRange() == 1.000:
					self.Show()
					(x, y) = chr.GetProjectPosition(self.vid, 220)
					self.SetPosition(x - self.GetWidth() / 2, y - self.GetHeight() / 2)
				else:
					LIMIT_RANGE = abs(constInfo.SHOPNAMES_RANGE * systemSetting.GetShopNamesRange())
					(to_x, to_y, to_z) = chr.GetPixelPosition(self.vid)
					(my_x, my_y, my_z) = player.GetMainCharacterPosition()
					if abs(my_x - to_x) <= LIMIT_RANGE and abs(my_y - to_y) <= LIMIT_RANGE:
						(x, y) = chr.GetProjectPosition(self.vid, 220)
						self.SetPosition(x - self.GetWidth() / 2, y - self.GetHeight() / 2)
						self.Show()
					else:
						self.Hide()
						self.SetPosition(-70, 0)
		else:
			for key in g_privateShopAdvertisementBoardDict.keys():
				if  player.GetMainCharacterIndex() == key:  #상점풍선을 안보이게 감추는 경우에도, 플레이어 자신의 상점 풍선은 보이도록 함. by 김준호
					g_privateShopAdvertisementBoardDict[key].Show() 	
					x, y = chr.GetProjectPosition(player.GetMainCharacterIndex(), 220)
					g_privateShopAdvertisementBoardDict[key].SetPosition(x - self.GetWidth()/2, y - self.GetHeight()/2)
				else:
					g_privateShopAdvertisementBoardDict[key].Hide()

class PrivateShopBuilder(ui.ScriptWindow):

	def __init__(self):
		#print "NEW MAKE_PRIVATE_SHOP_WINDOW ----------------------------------------------------------------"
		ui.ScriptWindow.__init__(self)

		self.__LoadWindow()
		self.itemStock = {}
		self.tooltipItem = None
		self.priceInputBoard = None
		self.title = ""

	def __del__(self):
		#print "------------------------------------------------------------- DELETE MAKE_PRIVATE_SHOP_WINDOW"
		ui.ScriptWindow.__del__(self)

	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/PrivateShopBuilder.py")
		except:
			import exception
			exception.Abort("PrivateShopBuilderWindow.LoadWindow.LoadObject")

		try:
			GetObject = self.GetChild
			self.nameLine = GetObject("NameLine")
			self.itemSlot = GetObject("ItemSlot")
			self.btnOk = GetObject("OkButton")
			self.btnClose = GetObject("CloseButton")
			self.titleBar = GetObject("TitleBar")
		except:
			import exception
			exception.Abort("PrivateShopBuilderWindow.LoadWindow.BindObject")

		self.btnOk.SetEvent(ui.__mem_func__(self.OnOk))
		self.btnClose.SetEvent(ui.__mem_func__(self.OnClose))
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.OnClose))

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
		self.titleBar = None
		self.priceInputBoard = None

	def Open(self, title):

		self.title = title

		if len(title) > 25:
			title = title[:22] + "..."

		self.itemStock = {}
		shop.ClearPrivateShopStock()
		self.nameLine.SetText(title)
		self.SetCenterPosition()
		self.Refresh()
		self.Show()

		global g_isBuildingPrivateShop
		g_isBuildingPrivateShop = True

	def Close(self):
		global g_isBuildingPrivateShop
		g_isBuildingPrivateShop = False

		self.title = ""
		self.itemStock = {}
		shop.ClearPrivateShopStock()
		self.Hide()

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def Refresh(self):
		getitemVNum=player.GetItemIndex
		getItemCount=player.GetItemCount
		setitemVNum=self.itemSlot.SetItemSlot
		delItem=self.itemSlot.ClearSlot

		for i in xrange(shop.SHOP_SLOT_COUNT):

			if not self.itemStock.has_key(i):
				delItem(i)
				continue

			pos = self.itemStock[i]

			itemCount = getItemCount(*pos)
			if itemCount <= 1:
				itemCount = 0
			setitemVNum(i, getitemVNum(*pos), itemCount)

		self.itemSlot.RefreshSlot()

	def OnSelectEmptySlot(self, selectedSlotPos):

		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			mouseModule.mouseController.DeattachObject()

			if app.ENABLE_SPECIAL_STORAGE:
				if player.SLOT_TYPE_INVENTORY != attachedSlotType and\
					player.SLOT_TYPE_DRAGON_SOUL_INVENTORY != attachedSlotType and\
					player.SLOT_TYPE_UPGRADE_INVENTORY != attachedSlotType and\
					player.SLOT_TYPE_STONE_INVENTORY != attachedSlotType and\
					player.SLOT_TYPE_CHEST_INVENTORY != attachedSlotType and\
					player.SLOT_TYPE_ATTR_INVENTORY != attachedSlotType:
					return
			else:
				if player.SLOT_TYPE_INVENTORY != attachedSlotType and player.SLOT_TYPE_DRAGON_SOUL_INVENTORY != attachedSlotType:
					return
			attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
				
			itemVNum = player.GetItemIndex(attachedInvenType, attachedSlotPos)
			item.SelectItem(itemVNum)

			if item.IsAntiFlag(item.ANTIFLAG_GIVE) or item.IsAntiFlag(item.ANTIFLAG_MYSHOP):
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.PRIVATE_SHOP_CANNOT_SELL_ITEM)
				return

			priceInputBoard = uiCommon.MoneyInputDialog()
			priceInputBoard.SetTitle(localeInfo.PRIVATE_SHOP_INPUT_PRICE_DIALOG_TITLE)
			priceInputBoard.SetAcceptEvent(ui.__mem_func__(self.AcceptInputPrice))
			priceInputBoard.SetCancelEvent(ui.__mem_func__(self.CancelInputPrice))
			priceInputBoard.Open()

			itemPrice=GetPrivateShopItemPrice(itemVNum)

			if itemPrice>0:
				priceInputBoard.SetValue(itemPrice)

			if app.ENABLE_CHEQUE_SYSTEM:
				itemPriceCheque=GetPrivateShopItemPriceCheque(itemVNum)

				if itemPriceCheque>0:
					priceInputBoard.SetValueCheque(itemPriceCheque)

			self.priceInputBoard = priceInputBoard
			self.priceInputBoard.itemVNum = itemVNum
			self.priceInputBoard.sourceWindowType = attachedInvenType
			self.priceInputBoard.sourceSlotPos = attachedSlotPos
			self.priceInputBoard.targetSlotPos = selectedSlotPos

	def OnSelectItemSlot(self, selectedSlotPos):

		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			snd.PlaySound("sound/ui/loginfail.wav")
			mouseModule.mouseController.DeattachObject()

		else:
			if not selectedSlotPos in self.itemStock:
				return

			invenType, invenPos = self.itemStock[selectedSlotPos]
			shop.DelPrivateShopItemStock(invenType, invenPos)
			snd.PlaySound("sound/ui/drop.wav")

			del self.itemStock[selectedSlotPos]

			self.Refresh()

	def AcceptInputPrice(self):

		if not self.priceInputBoard:
			return True

		if app.ENABLE_CHEQUE_SYSTEM:
			text = self.priceInputBoard.inputValue.GetText()
			text2 = self.priceInputBoard.InputValue_Cheque.GetText()

			if not text and not text2:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Info: 01")
				return True

			if not text.isdigit() and not text2.isdigit():
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Info: 02")
				return True

			if int(text) <= 0:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Info: 03")
				return True
		else:
			text = self.priceInputBoard.GetText()

			if not text:
				return True

			if not text.isdigit():
				return True

			if long(text) <= 0:
				return True

			if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
				if int(text) > 2000000000 - 1:
					return True			

		attachedInvenType = self.priceInputBoard.sourceWindowType
		sourceSlotPos = self.priceInputBoard.sourceSlotPos
		targetSlotPos = self.priceInputBoard.targetSlotPos

		for privatePos, (itemWindowType, itemSlotIndex) in self.itemStock.items():
			if itemWindowType == attachedInvenType and itemSlotIndex == sourceSlotPos:
				shop.DelPrivateShopItemStock(itemWindowType, itemSlotIndex)
				del self.itemStock[privatePos]

		if app.ENABLE_CHEQUE_SYSTEM:
			price = int(self.priceInputBoard.inputValue.GetText())
			price_cheque = int(self.priceInputBoard.InputValue_Cheque.GetText())

			if IsPrivateShopItemPriceList():
				SetPrivateShopItemPrice(self.priceInputBoard.itemVNum, price, price_cheque)

			shop.AddPrivateShopItemStock(attachedInvenType, sourceSlotPos, targetSlotPos, price, price_cheque)
		else:
			price = int(self.priceInputBoard.GetText())

			if IsPrivateShopItemPriceList():
				SetPrivateShopItemPrice(self.priceInputBoard.itemVNum, price)

			shop.AddPrivateShopItemStock(attachedInvenType, sourceSlotPos, targetSlotPos, price)

		self.itemStock[targetSlotPos] = (attachedInvenType, sourceSlotPos)
		snd.PlaySound("sound/ui/drop.wav")

		self.Refresh()		

		#####

		self.priceInputBoard = None
		return True

	def CancelInputPrice(self):
		self.priceInputBoard = None
		return True

	def OnOk(self):

		if not self.title:
			return

		if 0 == len(self.itemStock):
			return

		shop.BuildPrivateShop(self.title)
		self.Close()

	def OnClose(self):
		self.Close()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnOverInItem(self, slotIndex):

		if self.tooltipItem:
			if self.itemStock.has_key(slotIndex):
				self.tooltipItem.SetPrivateShopBuilderItem(*self.itemStock[slotIndex] + (slotIndex,))

	def OnOverOutItem(self):

		if self.tooltipItem:
			self.tooltipItem.HideToolTip()
