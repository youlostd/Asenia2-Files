import ui
import item
import net
import mouseModule
import player
import constInfo
import chat

class KygnItemSil(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Ekran()

	def __Initialize(self):
		self.SlotArray = []
		self.VnumArray = []
		self.WindowArray = []

	def __Ekran(self):
		KygnPyYukle = ui.PythonScriptLoader()
		KygnPyYukle.LoadScriptFile(self, "UIScript/kygnitemsil.py")

		KygnObject = self.GetChild
		self.titleBar = KygnObject("titlebar")
		self.ItemSlot = KygnObject("itemslot")
		self.ItemSilmeButonu = KygnObject("ItemSilmeButonu")
		self.ItemSatmaButonu = KygnObject("ItemSatmaButonu")


		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close2))
		self.ItemSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectEmptySlot))
		self.ItemSilmeButonu.SetEvent(ui.__mem_func__(self.__RemoveButton))
		self.ItemSatmaButonu.SetEvent(ui.__mem_func__(self.__SellButton))

	def __RemoveButton(self):
		net.SendChatPacket("/remove_item_system_button")
		self.ClearAllSlot()
		self.SlotArray = []
		self.VnumArray = []
		self.WindowArray = []

	def __SellButton(self):
		net.SendChatPacket("/sell_item_system_button")
		self.ClearAllSlot()
		self.SlotArray = []
		self.VnumArray = []
		self.WindowArray = []

	def __OnSelectEmptySlot(self, selectedSlotPos):
		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			Window_Type = 0
			if attachedSlotType == player.SLOT_TYPE_UPGRADE_INVENTORY:
				Window_Type = 1
			elif attachedSlotType == player.SLOT_TYPE_STONE_INVENTORY:
				Window_Type = 2
			elif attachedSlotType == player.SLOT_TYPE_CHEST_INVENTORY:
				Window_Type = 3
			elif attachedSlotType == player.SLOT_TYPE_ATTR_INVENTORY:
				Window_Type = 4

			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			self.PutSocket(selectedSlotPos, attachedSlotPos, Window_Type)
			mouseModule.mouseController.DeattachObject()

	def InventoryRightClick(self, NewItemIndex, Window_Type):
		for i in xrange(59):
			if i not in self.SlotArray:
				self.PutSocket(i, NewItemIndex, Window_Type)
				break

	def PutSocket(self, PutNewSlot, NewItemSlot, Window_Type):
		if NewItemSlot not in self.VnumArray:
			itemVNum = player.GetItemIndex(NewItemSlot)
			item.SelectItem(itemVNum)
			sizeX, sizeY = item.GetItemSize()
			if sizeY == 3:
				if PutNewSlot in self.SlotArray or PutNewSlot + 10 in self.SlotArray or PutNewSlot + 20 in self.SlotArray:
					chat.AppendChat(1, "HEYYY!! Bu itemi eklemezsin!")
					return
				if PutNewSlot > 59 or PutNewSlot + 10 > 59 or PutNewSlot + 20 > 59:
					chat.AppendChat(1, "HEYYY!! Bu itemi eklemezsin!")
					return
				self.SlotArray += [int(PutNewSlot)]
				self.SlotArray += [int(PutNewSlot + 10)]
				self.SlotArray += [int(PutNewSlot + 20)]
			elif sizeY == 2:
				if PutNewSlot in self.SlotArray or PutNewSlot + 10 in self.SlotArray:
					chat.AppendChat(1, "HEYYY!! Bu itemi eklemezsin!")
					return
				if PutNewSlot > 59 or PutNewSlot + 10 > 59:
					chat.AppendChat(1, "HEYYY!! Bu itemi eklemezsin!")
					return
				self.SlotArray += [int(PutNewSlot)]
				self.SlotArray += [int(PutNewSlot + 10)]
			elif sizeY == 1:
				if PutNewSlot in self.SlotArray:
					chat.AppendChat(1, "HEYYY!! Bu itemi eklemezsin!")
					return
				if PutNewSlot > 59:
					chat.AppendChat(1, "HEYYY!! Bu itemi eklemezsin!")
					return
				self.SlotArray += [int(PutNewSlot)]
			if Window_Type == 0:
				self.ItemSlot.SetItemSlot(PutNewSlot, player.GetItemIndex(player.INVENTORY, NewItemSlot), player.GetItemCount(player.INVENTORY, NewItemSlot))
			elif Window_Type == 1:
				self.ItemSlot.SetItemSlot(PutNewSlot, player.GetItemIndex(player.UPGRADE_INVENTORY, NewItemSlot), player.GetItemCount(player.UPGRADE_INVENTORY, NewItemSlot))
			elif Window_Type == 2:
				self.ItemSlot.SetItemSlot(PutNewSlot, player.GetItemIndex(player.STONE_INVENTORY, NewItemSlot), player.GetItemCount(player.STONE_INVENTORY, NewItemSlot))
			elif Window_Type == 3:
				self.ItemSlot.SetItemSlot(PutNewSlot, player.GetItemIndex(player.CHEST_INVENTORY, NewItemSlot), player.GetItemCount(player.CHEST_INVENTORY, NewItemSlot))
			elif Window_Type == 4:
				self.ItemSlot.SetItemSlot(PutNewSlot, player.GetItemIndex(player.ATTR_INVENTORY, NewItemSlot), player.GetItemCount(player.ATTR_INVENTORY, NewItemSlot))

			self.VnumArray += [int(NewItemSlot)]
			self.WindowArray += [int(Window_Type)]
			net.SendChatPacket("/add_remove_or_sell_item_index %d %d" % (NewItemSlot, Window_Type))
		else:
			return

	def Show(self):
		ui.ScriptWindow.Show(self)

	def Show2(self):
		ui.ScriptWindow.Show(self)
		net.SendChatPacket("/remove_or_sell_item_system_close")
		self.ClearAllSlot()

	def OnPressEscapeKey(self):
		self.Close2()
		return True

	def OnUpdate(self):
		self.ItemSlot.RefreshSlot()

	def ClearAllSlot(self):
		if self.SlotArray:
			for i in xrange(len(self.SlotArray)):
				self.ItemSlot.ClearSlot(self.SlotArray[i])

	def Close(self):
		self.Hide()

	def Close2(self):
		net.SendChatPacket("/remove_or_sell_item_system_close")
		constInfo.ITEM_REMOVE_WINDOW_STATUS = 0
		self.ClearAllSlot()
		self.SlotArray = []
		self.VnumArray = []
		self.Hide()
