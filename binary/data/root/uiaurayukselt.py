import net
import chat
import app
import event
import ui
import uitaskbar
import time
import localeInfo
import chrmgr
import background
import chr
import systemSetting
import player
import interfacemodule
import uiCommon
import uiToolTip
import grp
import guild
import item
import mouseModule
import constInfo
import uiInventory
import snd
import uiScriptLocale


class KygnAuraYukselt(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Ekran()
		self.tooltipItem1 = uiToolTip.ItemToolTip()
		self.tooltipItem1.Hide()
		self.tooltipItem2 = uiToolTip.ItemToolTip()
		self.tooltipItem2.Hide()


	def __del__(self):
		ui.ScriptWindow.__del__(self)
		

	def __Initialize(self):
		self.titleBar = 0
		self.AuraLevel = 0
		self.AuraPos = 753
		self.MaterialPos = 753
		self.Window = 0

	def __Ekran(self):
		KygnPyYukle = ui.PythonScriptLoader()
		KygnPyYukle.LoadScriptFile(self, "UIScript/aurayukselt.py" )

		KygnObject = self.GetChild
		self.titleBar = KygnObject("TitleBar")
		self.YukseltButon = KygnObject("YukseltButon")
		self.KapatButon = KygnObject("KapatButon")
		self.AuraText1 = KygnObject("AuraText1")
		self.AuraText2 = KygnObject("AuraText2")
		self.AuraText3 = KygnObject("AuraText3")
		self.AuraText4 = KygnObject("AuraText4")
		self.AuraText5 = KygnObject("AuraText5")
		self.AuraText6 = KygnObject("AuraText6")
		self.AuraSlot = KygnObject("AuraSlot")

		self.AuraSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectEmptySlot))
		self.AuraSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.__OnSelectItemSlot))
		self.AuraSlot.SetUnselectItemSlotEvent(ui.__mem_func__(self.__OnSelectItemSlot))
		self.AuraSlot.SetOverInItemEvent(ui.__mem_func__(self.__OnOverInItem))
		self.AuraSlot.SetOverOutItemEvent(ui.__mem_func__(self.__OnOverOutItem))
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		self.YukseltButon.SetEvent(ui.__mem_func__(self.__YukseltButon))
		self.KapatButon.SetEvent(ui.__mem_func__(self.__KapatButon))
	
		if self.AuraPos == 753:
			self.AuraText1.Hide()
			self.AuraText2.Hide()
			self.AuraText3.Hide()
			self.AuraText4.Hide()
			self.AuraText5.Hide()
			self.AuraText6.Hide()


	def __YukseltButon(self):
		if self.Window == 1:
			net.SendChatPacket("/aura_yukselt_button")
		elif self.Window == 2:
			net.SendChatPacket("/aura_gelistir_button")

	def __KapatButon(self):
		self.Close()

	def __OnSelectEmptySlot(self):
		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			itemIndex = player.GetItemIndex(attachedSlotPos)
			item.SelectItem(itemIndex)
			mouseModule.mouseController.DeattachObject()
			if ((item.GetItemType() == item.ITEM_TYPE_COSTUME) and (item.GetItemSubType() == item.COSTUME_TYPE_AURA) and (itemIndex != 49006)):
				net.SendChatPacket("/aura_add %d" % (attachedSlotPos))
				self.AuraSlot.SetItemSlot(0, player.GetItemIndex(player.INVENTORY, attachedSlotPos), player.GetItemCount(player.INVENTORY, attachedSlotPos))
			elif (itemIndex == 49990):
				net.SendChatPacket("/aura_add %d" % (attachedSlotPos))
				self.AuraSlot.SetItemSlot(1, player.GetItemIndex(player.INVENTORY, attachedSlotPos), player.GetItemCount(player.INVENTORY, attachedSlotPos))
			else:
				return

	def __OnSelectItemSlot(self, selectedSlotPos):
		self.AuraSlot.ClearSlot(selectedSlotPos)
		net.SendChatPacket("/aura_clear %d" % (selectedSlotPos))


	def __OnOverInItem(self, slotIndex):
		if self.AuraPos != 753 and slotIndex == 0:
			self.tooltipItem1.SetInventoryItem(self.AuraPos)
			self.tooltipItem1.Show()
		else:
			self.tooltipItem1.HideToolTip()
		if self.MaterialPos != 753 and slotIndex == 1:
			self.tooltipItem2.SetInventoryItem(self.MaterialPos)
			self.tooltipItem2.Show()
		else:
			self.tooltipItem2.HideToolTip()

	def __OnOverOutItem(self):
		if self.tooltipItem1:
			self.tooltipItem1.HideToolTip()
		if self.tooltipItem2:
			self.tooltipItem2.HideToolTip()


	def AuraBoard(self, Pencere, AuraLevel, AuraPos, MaterialPos):
		self.AuraPos = AuraPos
		self.MaterialPos = MaterialPos
		self.AuraLevel = AuraLevel + 1
		self.Window = Pencere
		if self.AuraPos != 753:
			Sayi = 0
			Sayi2 = 0
			self.AuraSlot.SetItemSlot(0, player.GetItemIndex(player.INVENTORY, self.AuraPos), player.GetItemCount(player.INVENTORY, self.AuraPos))
			if self.AuraLevel > 1:
				self.AuraText1.SetText("Yeni Aura:")
				self.AuraText1.Show()
				self.AuraText2.SetText(localeInfo.AURA_LEVEL % (self.AuraLevel, self.AuraLevel))
				self.AuraText2.Show()
				self.AuraText4.SetText(localeInfo.AURA_TUKETILEN_NESNE_BASLIK)
				self.AuraText4.Show()
				self.AuraText5.SetText(localeInfo.AURA_TUKETILEN_NESNE + "1")
				self.AuraText5.Show()
				self.AuraText6.SetText("25.000.000 Yang")
				self.AuraText6.Show()
				for i in xrange(0,250,10):
					if self.AuraLevel >= 10:
						self.AuraLevel -= 10
						Sayi += 1
					else:
						continue;
				Sayi2 = self.AuraLevel
				self.AuraText3.SetText(str(localeInfo.AURA_EMIS) + str(Sayi) + str(".") + str(Sayi2))
				self.AuraText3.Show()
			else:
				self.AuraText1.Hide()
				self.AuraText2.Hide()
				self.AuraText3.Hide()
				self.AuraText4.Hide()
				self.AuraText5.Hide()
				self.AuraText6.Hide()

		else:
			self.AuraSlot.ClearSlot(0)

		if self.MaterialPos != 753:
			self.AuraSlot.SetItemSlot(1, player.GetItemIndex(player.INVENTORY, self.MaterialPos), player.GetItemCount(player.INVENTORY, self.MaterialPos))
		else:
			self.AuraSlot.ClearSlot(1)
		self.Show()

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE
	
	def Show(self):
		self.__Ekran
		ui.ScriptWindow.Show(self)

	def Close(self):
		if self.tooltipItem1:
			self.tooltipItem1.HideToolTip()
		if self.tooltipItem2:
			self.tooltipItem2.HideToolTip()
		net.SendChatPacket("/aura_paneli_kapat")
		self.Hide()
		

class KygnAuraBonusEmis(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Ekran()
		self.tooltipItem1 = uiToolTip.ItemToolTip()
		self.tooltipItem1.Hide()
		self.tooltipItem2 = uiToolTip.ItemToolTip()
		self.tooltipItem2.Hide()
		self.tooltipItem3 = uiToolTip.ItemToolTip()
		self.tooltipItem3.Hide()


	def __del__(self):
		ui.ScriptWindow.__del__(self)
		

	def __Initialize(self):
		self.titleBar = 0
		self.AuraLevel = 0
		self.AuraLevelIki = 0
		self.AuraPos = 753
		self.MaterialPos = 753

	def __Ekran(self):
		KygnPyYukle = ui.PythonScriptLoader()
		KygnPyYukle.LoadScriptFile(self, "UIScript/aurabonusemis.py" )

		KygnObject = self.GetChild
		self.titleBar = KygnObject("TitleBar")
		self.EmdirButon = KygnObject("EmdirButon")
		self.KapatButon = KygnObject("KapatButon")
		self.AuraSlot = KygnObject("AuraSlot")

		self.AuraSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.__OnSelectEmptySlot))
		self.AuraSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.__OnSelectItemSlot))
		self.AuraSlot.SetUnselectItemSlotEvent(ui.__mem_func__(self.__OnSelectItemSlot))
		self.AuraSlot.SetOverInItemEvent(ui.__mem_func__(self.__OnOverInItem))
		self.AuraSlot.SetOverOutItemEvent(ui.__mem_func__(self.__OnOverOutItem))
		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		self.EmdirButon.SetEvent(ui.__mem_func__(self.__EmdirButon))
		self.KapatButon.SetEvent(ui.__mem_func__(self.__KapatButon))


	def __EmdirButon(self):
		net.SendChatPacket("/aura_emis_button")

	def __KapatButon(self):
		self.Close()

	def __OnSelectEmptySlot(self):
		isAttached = mouseModule.mouseController.isAttached()
		if isAttached:
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			itemIndex = player.GetItemIndex(attachedSlotPos)
			item.SelectItem(itemIndex)
			mouseModule.mouseController.DeattachObject()
			if ((item.GetItemType() == item.ITEM_TYPE_COSTUME) and (item.GetItemSubType() == item.COSTUME_TYPE_AURA)):
				net.SendChatPacket("/aura_add %d" % (attachedSlotPos))
				self.AuraSlot.SetItemSlot(0, player.GetItemIndex(player.INVENTORY, attachedSlotPos), player.GetItemCount(player.INVENTORY, attachedSlotPos))
			elif ((item.GetItemType() == item.ITEM_TYPE_ARMOR) and (item.GetItemSubType() == item.ARMOR_HEAD or item.GetItemSubType() == item.ARMOR_SHIELD or item.GetItemSubType() == item.ARMOR_WRIST or item.GetItemSubType() == item.ARMOR_FOOTS or item.GetItemSubType() == item.ARMOR_NECK or item.GetItemSubType() == item.ARMOR_EAR)):
				net.SendChatPacket("/aura_add %d" % (attachedSlotPos))
				self.AuraSlot.SetItemSlot(1, player.GetItemIndex(player.INVENTORY, attachedSlotPos), player.GetItemCount(player.INVENTORY, attachedSlotPos))
			else:
				return

	def __OnSelectItemSlot(self, selectedSlotPos):
		if selectedSlotPos < 2:
			self.AuraSlot.ClearSlot(selectedSlotPos)
			net.SendChatPacket("/aura_clear %d" % (selectedSlotPos))


	def __OnOverInItem(self, slotIndex):
		if self.AuraPos != 753 and slotIndex == 0:
			self.tooltipItem1.SetInventoryItem(self.AuraPos)
			self.tooltipItem1.Show()
		if self.MaterialPos != 753 and slotIndex == 1:
			self.tooltipItem2.SetInventoryItem(self.MaterialPos)
			self.tooltipItem2.Show()
		if self.MaterialPos != 753 and self.AuraPos != 753 and slotIndex == 2:
			self.tooltipItem3.SetAuraResultTooltip(self.AuraPos, self.MaterialPos, self.AuraLevel)
			self.tooltipItem3.Show()

	def __OnOverOutItem(self):
		if self.tooltipItem1:
			self.tooltipItem1.HideToolTip()
		if self.tooltipItem2:
			self.tooltipItem2.HideToolTip()
		if self.tooltipItem3:
			self.tooltipItem3.HideToolTip()


	def AuraBonusEmisBoard(self, Pencere, AuraLevel, AuraPos, MaterialPos):
		self.AuraPos = AuraPos
		self.MaterialPos = MaterialPos
		self.AuraLevel = AuraLevel + 1
		self.AuraLevelIki = AuraLevel
		self.Window = Pencere
		if self.AuraPos != 753:
			self.AuraSlot.SetItemSlot(0, player.GetItemIndex(player.INVENTORY, self.AuraPos), player.GetItemCount(player.INVENTORY, self.AuraPos))
		else:
			self.AuraSlot.ClearSlot(0)
		if self.MaterialPos != 753:
			self.AuraSlot.SetItemSlot(1, player.GetItemIndex(player.INVENTORY, self.MaterialPos), player.GetItemCount(player.INVENTORY, self.MaterialPos))
		else:
			self.AuraSlot.ClearSlot(1)
		if self.AuraPos != 753 and self.MaterialPos != 753:
			self.AuraSlot.SetItemSlot(2, player.GetItemIndex(player.INVENTORY, self.AuraPos), player.GetItemCount(player.INVENTORY, self.AuraPos))
		else:
			self.AuraSlot.ClearSlot(2)
		self.Show()

	def OnPressEscapeKey(self):
		self.Close()
		return TRUE
	
	def Show(self):
		self.__Ekran
		ui.ScriptWindow.Show(self)

	def Close(self):
		if self.tooltipItem1:
			self.tooltipItem1.HideToolTip()
		if self.tooltipItem2:
			self.tooltipItem2.HideToolTip()
		if self.tooltipItem3:
			self.tooltipItem3.HideToolTip()

		net.SendChatPacket("/aura_paneli_kapat")
		self.Hide()
		
