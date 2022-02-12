import ui
import mouseModule
import player
import net
import snd
import safebox
import chat
import app
import localeInfo
import uiScriptLocale
import constInfo
if app.__ENABLE_NEW_OFFLINESHOP__:
	import uinewofflineshop


import constInfo

if app.WJ_SECURITY_SYSTEM:
	class SecurityDialog(ui.ScriptWindow):
		def __init__(self):
			ui.ScriptWindow.__init__(self)
			self.__LoadDialog()
			self.isCreate = False

		def __del__(self):
			ui.ScriptWindow.__del__(self)

		def __LoadDialog(self):
			try:
				pyScrLoader = ui.PythonScriptLoader()
				pyScrLoader.LoadScriptFile(self, "uiscript/securitydialog.py")
			except:
				import exception
				exception.Abort("GuvenlikDialog.__LoadDialog.LoadObject")

			try:
				self.passwordValue = self.GetChild("password_value")
				self.acceptButton = self.GetChild("accept_button")
				self.cancelButton = self.GetChild("cancel_button")
				self.titleName = self.GetChild("TitleName")
				self.securtiyControl = self.GetChild("security_control")
				self.rememberButton = self.GetChild("remember_button")
				self.kapatbutton = self.GetChild("kapat")
				self.button1 = self.GetChild("1")
				self.button2 = self.GetChild("2")
				self.button3 = self.GetChild("3")
				self.button4 = self.GetChild("4")
				self.button5 = self.GetChild("5")
				self.button6 = self.GetChild("6")
				self.button7 = self.GetChild("7")
				self.button8 = self.GetChild("8")
				self.button9 = self.GetChild("9")
				self.button0 = self.GetChild("0")
				self.clear_button = self.GetChild("clear_button")
				self.sistemikapa = self.GetChild("sistemikapa")
				self.kapatext = self.GetChild("kapatext")
				self.rememberButton.Show()
				self.GetChild("titlebar").SetCloseEvent(ui.__mem_func__(self.OnCancel))
			except:
				import exception
				exception.Abort("GuvenlikDialog.__LoadDialog.BindObject")

			self.passwordValue.OnIMEReturn = self.OnAccept
			self.passwordValue.OnPressEscapeKey = self.OnCancel
			self.acceptButton.SetEvent(ui.__mem_func__(self.OnAccept))
			self.cancelButton.SetEvent(ui.__mem_func__(self.OnCancel))
			self.kapatbutton.SetEvent(ui.__mem_func__(self.kapatgitsin))
			self.clear_button.SetEvent(ui.__mem_func__(self.temizle))
			self.sistemikapa.SetEvent(ui.__mem_func__(self.__sistemikapa))
			self.button1.SetEvent(lambda arg=1: self.num_click(arg))
			self.button2.SetEvent(lambda arg=2: self.num_click(arg))
			self.button3.SetEvent(lambda arg=3: self.num_click(arg))
			self.button4.SetEvent(lambda arg=4: self.num_click(arg))
			self.button5.SetEvent(lambda arg=5: self.num_click(arg))
			self.button6.SetEvent(lambda arg=6: self.num_click(arg))
			self.button7.SetEvent(lambda arg=7: self.num_click(arg))
			self.button8.SetEvent(lambda arg=8: self.num_click(arg))
			self.button9.SetEvent(lambda arg=9: self.num_click(arg))
			self.button0.SetEvent(lambda arg=0: self.num_click(arg))
			self.cancelButton.Hide()
			self.rememberButton.SetEvent(ui.__mem_func__(self.OnChangePassword))
			self.wndChangePassword2 = ChangePasswordDialog2()
				
			import constInfo
			if constInfo.open_security_button == 1:
				self.sistemikapa.SetUpVisual("d:/ymir work/interface/checkbox/filled_01_normal.png")
				self.sistemikapa.SetOverVisual("d:/ymir work/interface/checkbox/filled_02_hover.png")
				self.sistemikapa.SetDownVisual("d:/ymir work/interface/checkbox/filled_03_active.png")
				self.kapatext.SetText("Sistem Kapalý")
			else:
				self.sistemikapa.SetUpVisual("d:/ymir work/interface/checkbox/empty_01_normal.png")
				self.sistemikapa.SetOverVisual("d:/ymir work/interface/checkbox/empty_02_hover.png")
				self.sistemikapa.SetDownVisual("d:/ymir work/interface/checkbox/empty_03_active.png")
				self.kapatext.SetText("Sistemi Açýk")
			
			

		def kapatgitsin(self):
			import app
			app.Exit()
		
		def temizle(self):
			self.passwordValue.SetText("")
			
			
		def __sistemikapa(self):
			import constInfo
			if constInfo.open_security_button == 0:
				constInfo.open_security_button = 1
				self.sistemikapa.SetUpVisual("d:/ymir work/interface/checkbox/filled_01_normal.png")
				self.sistemikapa.SetOverVisual("d:/ymir work/interface/checkbox/filled_02_hover.png")
				self.sistemikapa.SetDownVisual("d:/ymir work/interface/checkbox/filled_03_active.png")
				self.kapatext.SetText("Sistemi Kapalý")
			else:
				constInfo.open_security_button = 0
				self.sistemikapa.SetUpVisual("d:/ymir work/interface/checkbox/empty_01_normal.png")
				self.sistemikapa.SetOverVisual("d:/ymir work/interface/checkbox/empty_02_hover.png")
				self.sistemikapa.SetDownVisual("d:/ymir work/interface/checkbox/empty_03_active.png")
				self.kapatext.SetText("Sistemi Açýk")

		def num_click(self,num):
			val = int(len(self.passwordValue.GetText()))
			if (val < 6):
				val2 = self.passwordValue.GetText()+str(num)
				self.passwordValue.SetText(val2)

		
		def Destroy(self):
			self.ClearDictionary()
			self.passwordValue = None
			self.acceptButton = None
			self.cancelButton = None
			self.titleName = None
			self.dlgChangePassword = None

		def SetTitle(self, title):
			self.titleName.SetText(title)
			
		def ShowDialog2(self, stat):			
			self.passwordValue.SetText("")
			self.passwordValue.SetFocus()
			self.SetCenterPosition()
			import constInfo
			if int(stat) == 0:
				constInfo.open_security_button = 0
				self.securtiyControl.SetText("|cff00ff00" + "Güvenlik aktif")
			else:
				constInfo.open_security_button = 1
				self.securtiyControl.SetText("|cffff0000" + "Güvenlik kapalý")
			self.rememberButton.SetText("Þ. Deðiþtir")
			self.rememberButton.SetToolTipText(localeInfo.SECURTY_IF_YOU_REMEMBER_PASSWORD)
			self.Show()

		def ShowDialog(self, isCreate):			
			self.passwordValue.SetText("")
			self.passwordValue.SetFocus()
			self.SetCenterPosition()
			if isCreate == True:
				self.securtiyControl.SetText("|cffff0000" + "Ýlk Giriþ")
				self.acceptButton.SetText("Tamam")
				self.rememberButton.Hide()
				self.rememberButton.SetToolTipText(uiScriptLocale.SECURTY_INFO)
				import chat
				chat.AppendChat(1,"Depo þifreniz karakter þifrenizle aynýdýr.")
				self.isCreate = True
			else:
				self.securtiyControl.SetText("|cff00ff00" + "Güvenlik aktif")
				self.rememberButton.SetText("Þ. Deðiþtir")
				self.rememberButton.SetToolTipText(localeInfo.SECURTY_IF_YOU_REMEMBER_PASSWORD)
				self.isCreate = False
			self.Show()

		def CloseDialog(self):
			constInfo.open_security = 1
			self.passwordValue.KillFocus()
			self.Hide()

		def OnAccept(self):
			if self.isCreate == True:
				net.SendChatPacket("/create_security " + str(self.passwordValue.GetText()) + " " + str(constInfo.open_security_button))
			else:
				net.SendChatPacket("/input_security " + str(self.passwordValue.GetText()) + " " + str(constInfo.open_security_button))
			return True

		def OnCancel(self):
			pass
			
		def OnChangePassword(self):
			if self.isCreate == False:
				self.wndChangePassword2.Open()
			else:
				pass
				
	class ChangePasswordDialog2(ui.ScriptWindow):
		def __init__(self):
			ui.ScriptWindow.__init__(self)
			self.LoadDialog()

		def __del__(self):
			ui.ScriptWindow.__del__(self)

		def __LoadDialog(self):
			self.dlgMessage = ui.ScriptWindow()
			try:
				pyScrLoader = ui.PythonScriptLoader()
				pyScrLoader.LoadScriptFile(self.dlgMessage, "uiscript/popupdialog.py")
				self.dlgMessage.GetChild("message").SetText(localeInfo.SAFEBOX_WRONG_PASSWORD)
				self.dlgMessage.GetChild("accept").SetEvent(ui.__mem_func__(self.OnCloseMessageDialog))
			except:
				import exception
				exception.Abort("SafeboxWindow.__LoadDialog.LoadObject")

		def LoadDialog(self):
			try:
				pyScrLoader = ui.PythonScriptLoader()
				pyScrLoader.LoadScriptFile(self, "uiscript/changepassworddialog2.py")

			except:
				import exception
				exception.Abort("ChangePasswordDialog.LoadDialog.LoadObject")

			try:
				self.GetChild("accept_button").SetEvent(ui.__mem_func__(self.OnAccept))
				self.GetChild("cancel_button").SetEvent(ui.__mem_func__(self.OnCancel))
				self.GetChild("titlebar").SetCloseEvent(ui.__mem_func__(self.OnCancel))
				oldPassword = self.GetChild("old_password_value")
				newPassword = self.GetChild("new_password_value")
				newPasswordCheck = self.GetChild("new_password_check_value")
			except:
				import exception
				exception.Abort("ChangePasswordDialog.LoadDialog.BindObject")

			oldPassword.SetTabEvent(lambda arg=1: self.OnNextFocus(arg))
			newPassword.SetTabEvent(lambda arg=2: self.OnNextFocus(arg))
			newPasswordCheck.SetTabEvent(lambda arg=3: self.OnNextFocus(arg))
			oldPassword.SetReturnEvent(lambda arg=1: self.OnNextFocus(arg))
			newPassword.SetReturnEvent(lambda arg=2: self.OnNextFocus(arg))
			newPasswordCheck.SetReturnEvent(ui.__mem_func__(self.OnAccept))
			oldPassword.OnPressEscapeKey = self.OnCancel
			newPassword.OnPressEscapeKey = self.OnCancel
			newPasswordCheck.OnPressEscapeKey = self.OnCancel

			self.oldPassword = oldPassword
			self.newPassword = newPassword
			self.newPasswordCheck = newPasswordCheck

		def OnNextFocus(self, arg):
			if 1 == arg:
				self.oldPassword.KillFocus()
				self.newPassword.SetFocus()
			elif 2 == arg:
				self.newPassword.KillFocus()
				self.newPasswordCheck.SetFocus()
			elif 3 == arg:
				self.newPasswordCheck.KillFocus()
				self.oldPassword.SetFocus()

		def Destroy(self):
			self.ClearDictionary()
			self.dlgMessage.ClearDictionary()
			self.oldPassword = None
			self.newPassword = None
			self.newPasswordCheck = None

		def Open(self):
			self.oldPassword.SetText("")
			self.newPassword.SetText("")
			self.newPasswordCheck.SetText("")
			self.oldPassword.SetFocus()
			self.SetCenterPosition()
			self.SetTop()
			self.Show()

		def Close(self):
			self.oldPassword.SetText("")
			self.newPassword.SetText("")
			self.newPasswordCheck.SetText("")
			self.oldPassword.KillFocus()
			self.newPassword.KillFocus()
			self.newPasswordCheck.KillFocus()
			self.Hide()

		def OnAccept(self):
			oldPasswordText = self.oldPassword.GetText()
			newPasswordText = self.newPassword.GetText()
			newPasswordCheckText = self.newPasswordCheck.GetText()
			if newPasswordText != newPasswordCheckText:
				self.dlgMessage.SetCenterPosition()
				self.dlgMessage.SetTop()
				self.dlgMessage.Show()
				return True
			net.SendChatPacket("/change_security %s %s %s" % (oldPasswordText, newPasswordText, newPasswordCheckText))
			self.Close()
			return True

		def OnCancel(self):
			self.Close()
			return True

		def OnCloseMessageDialog(self):
			self.newPassword.SetText("")
			self.newPasswordCheck.SetText("")
			self.newPassword.SetFocus()
			self.dlgMessage.Hide()

class PasswordDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadDialog()
		self.Refreshdepolar()

		self.sendMessage = "/safebox_password "

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
#			if localeInfo.IsEUROPE()and app.GetLocalePath() != "locale/ca"and app.GetLocalePath() != "locale/sg" :
			pyScrLoader.LoadScriptFile(self, uiScriptLocale.LOCALE_UISCRIPT_PATH + "passworddialog.py")
#			else:
#				pyScrLoader.LoadScriptFile(self, "uiscript/passworddialog.py")
		except:
			import exception
			exception.Abort("PasswordDialog.__LoadDialog.LoadObject")

		try:
			self.passwordValue = self.GetChild("password_value")
			self.acceptButton = self.GetChild("accept_button")
			self.cancelButton = self.GetChild("cancel_button")
			self.NormalDepo = self.GetChild("normal_button")
			self.NesneDepo = self.GetChild("nesne_button")
			self.titleName = self.GetChild("TitleName")
			self.GetChild("titlebar").SetCloseEvent(ui.__mem_func__(self.CloseDialog))
		except:
			import exception
			exception.Abort("PasswordDialog.__LoadDialog.BindObject")

		self.passwordValue.OnIMEReturn = self.OnAccept
		self.passwordValue.OnPressEscapeKey = self.OnCancel
		self.acceptButton.SetEvent(ui.__mem_func__(self.OnAccept))
		self.cancelButton.SetEvent(ui.__mem_func__(self.OnCancel))
		self.NormalDepo.SetEvent(ui.__mem_func__(self.__NormalDepo))
		self.NesneDepo.SetEvent(ui.__mem_func__(self.__NesneDepo))

	def Destroy(self):
		self.ClearDictionary()
		self.passwordValue = None
		self.acceptButton = None
		self.cancelButton = None
		self.titleName = None

	def SetTitle(self, title):
		self.titleName.SetText(title)

	def SetSendMessage(self, msg):
		self.sendMessage = msg

	def ShowDialog(self):
		self.passwordValue.SetText("")
		self.passwordValue.SetFocus()
		self.SetCenterPosition()
		self.Show()

	def CloseDialog(self):
		self.passwordValue.KillFocus()
		self.Hide()

	def OnAccept(self):
		if constInfo.depolar == 1:
			net.SendChatPacket(self.sendMessage + self.passwordValue.GetText())
		elif constInfo.depolar == 0:
			net.SendChatPacket("/mall_password " + self.passwordValue.GetText())
			
		self.CloseDialog()
		return TRUE

	def OnCancel(self):
		self.CloseDialog()
		return True

	def __NormalDepo(self):
		constInfo.depolar = 1
		self.Refreshdepolar()

	def __NesneDepo(self):
		constInfo.depolar = 0
		self.Refreshdepolar()

	def Refreshdepolar(self):
		if constInfo.depolar == 1:
			self.NormalDepo.Down()
			self.NesneDepo.SetUp()
		else:
			self.NesneDepo.Down()
			self.NormalDepo.SetUp()

class ChangePasswordDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadDialog()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		self.dlgMessage = ui.ScriptWindow()
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self.dlgMessage, "uiscript/popupdialog.py")
			self.dlgMessage.GetChild("message").SetText(localeInfo.SAFEBOX_WRONG_PASSWORD)
			self.dlgMessage.GetChild("accept").SetEvent(ui.__mem_func__(self.OnCloseMessageDialog))
		except:
			import exception
			exception.Abort("SafeboxWindow.__LoadDialog.LoadObject")

	def LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/changepassworddialog.py")

		except:
			import exception
			exception.Abort("ChangePasswordDialog.LoadDialog.LoadObject")

		try:
			self.GetChild("accept_button").SetEvent(ui.__mem_func__(self.OnAccept))
			self.GetChild("cancel_button").SetEvent(ui.__mem_func__(self.OnCancel))
			self.GetChild("titlebar").SetCloseEvent(ui.__mem_func__(self.OnCancel))
			oldPassword = self.GetChild("old_password_value")
			newPassword = self.GetChild("new_password_value")
			newPasswordCheck = self.GetChild("new_password_check_value")
		except:
			import exception
			exception.Abort("ChangePasswordDialog.LoadDialog.BindObject")

		oldPassword.SetTabEvent(lambda arg=1: self.OnNextFocus(arg))
		newPassword.SetTabEvent(lambda arg=2: self.OnNextFocus(arg))
		newPasswordCheck.SetTabEvent(lambda arg=3: self.OnNextFocus(arg))
		oldPassword.SetReturnEvent(lambda arg=1: self.OnNextFocus(arg))
		newPassword.SetReturnEvent(lambda arg=2: self.OnNextFocus(arg))
		newPasswordCheck.SetReturnEvent(ui.__mem_func__(self.OnAccept))
		oldPassword.OnPressEscapeKey = self.OnCancel
		newPassword.OnPressEscapeKey = self.OnCancel
		newPasswordCheck.OnPressEscapeKey = self.OnCancel

		self.oldPassword = oldPassword
		self.newPassword = newPassword
		self.newPasswordCheck = newPasswordCheck

	def OnNextFocus(self, arg):
		if 1 == arg:
			self.oldPassword.KillFocus()
			self.newPassword.SetFocus()
		elif 2 == arg:
			self.newPassword.KillFocus()
			self.newPasswordCheck.SetFocus()
		elif 3 == arg:
			self.newPasswordCheck.KillFocus()
			self.oldPassword.SetFocus()

	def Destroy(self):
		self.ClearDictionary()
		self.dlgMessage.ClearDictionary()
		self.oldPassword = None
		self.newPassword = None
		self.newPasswordCheck = None

	def Open(self):
		self.oldPassword.SetText("")
		self.newPassword.SetText("")
		self.newPasswordCheck.SetText("")
		self.oldPassword.SetFocus()
		self.SetCenterPosition()
		self.SetTop()
		self.Show()

	def Close(self):
		self.oldPassword.SetText("")
		self.newPassword.SetText("")
		self.newPasswordCheck.SetText("")
		self.oldPassword.KillFocus()
		self.newPassword.KillFocus()
		self.newPasswordCheck.KillFocus()
		self.Hide()

	def OnAccept(self):
		oldPasswordText = self.oldPassword.GetText()
		newPasswordText = self.newPassword.GetText()
		newPasswordCheckText = self.newPasswordCheck.GetText()
		if newPasswordText != newPasswordCheckText:
			self.dlgMessage.SetCenterPosition()
			self.dlgMessage.SetTop()
			self.dlgMessage.Show()
			return True
		net.SendChatPacket("/safebox_change_password %s %s" % (oldPasswordText, newPasswordText))
		self.Close()
		return True

	def OnCancel(self):
		self.Close()
		return True

	def OnCloseMessageDialog(self):
		self.newPassword.SetText("")
		self.newPasswordCheck.SetText("")
		self.newPassword.SetFocus()
		self.dlgMessage.Hide()

class SafeboxWindow(ui.ScriptWindow):

	BOX_WIDTH = 176

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.tooltipItem = None
		self.sellingSlotNumber = -1
		self.pageButtonList = []
		self.curPageIndex = 0
		self.isLoaded = 0
		self.xSafeBoxStart = 0
		self.ySafeBoxStart = 0

		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self):
		self.__LoadWindow()

		ui.ScriptWindow.Show(self)		

	def Destroy(self):
		self.ClearDictionary()

		self.dlgPickMoney.Destroy()
		self.dlgPickMoney = None
		self.dlgChangePassword.Destroy()
		self.dlgChangePassword = None

		self.tooltipItem = None
		self.wndMoneySlot = None
		self.wndMoney = None
		self.wndBoard = None
		self.wndItem = None

		self.pageButtonList = []

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/SafeboxWindow.py")

		from _weakref import proxy

		## Item
		wndItem = ui.GridSlotWindow()
		wndItem.SetParent(self)
		wndItem.SetPosition(8, 35)
		wndItem.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
		wndItem.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
		wndItem.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndItem.SetUseSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndItem.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		wndItem.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		wndItem.Show()

		## PickMoneyDialog
		import uiPickMoney
		dlgPickMoney = uiPickMoney.PickMoneyDialog()
		dlgPickMoney.LoadDialog()
		dlgPickMoney.SetAcceptEvent(ui.__mem_func__(self.OnPickMoney))
		dlgPickMoney.Hide()

		## ChangePasswrod
		dlgChangePassword = ChangePasswordDialog()
		dlgChangePassword.LoadDialog()
		dlgChangePassword.Hide()

		## Close Button
		self.GetChild("TitleBar").SetCloseEvent(ui.__mem_func__(self.Close))
		self.GetChild("ChangePasswordButton").SetEvent(ui.__mem_func__(self.OnChangePassword))
		self.GetChild("ExitButton").SetEvent(ui.__mem_func__(self.Close))

		self.wndItem = wndItem
		self.dlgPickMoney = dlgPickMoney
		self.dlgChangePassword = dlgChangePassword
		self.wndBoard = self.GetChild("board")
		#self.wndMoney = self.GetChild("Money")
		#self.wndMoneySlot = self.GetChild("Money_Slot")
		#self.wndMoneySlot.SetEvent(ui.__mem_func__(self.OpenPickMoneyDialog))

		## Initialize
		self.SetTableSize(3)
		self.RefreshSafeboxMoney()

	def OpenPickMoneyDialog(self):

		if mouseModule.mouseController.isAttached():

			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			if player.SLOT_TYPE_INVENTORY == mouseModule.mouseController.GetAttachedType():

				if player.ITEM_MONEY == mouseModule.mouseController.GetAttachedItemIndex():
					net.SendSafeboxSaveMoneyPacket(mouseModule.mouseController.GetAttachedItemCount())
					snd.PlaySound("sound/ui/money.wav")

			mouseModule.mouseController.DeattachObject()

		else:
			curMoney = safebox.GetMoney()

			if curMoney <= 0:
				return

			self.dlgPickMoney.Open(curMoney)

	def ShowWindow(self, size):

		(self.xSafeBoxStart, self.ySafeBoxStart, z) = player.GetMainCharacterPosition()

		self.SetTableSize(size)
		self.Show()

	def __MakePageButton(self, pageCount):

		self.curPageIndex = 0
		self.pageButtonList = []

		text = "I"
		pos = -int(float(pageCount-1)/2 * 52)
		for i in xrange(pageCount):
			button = ui.RadioButton()
			button.SetParent(self)
			button.SetUpVisual("d:/ymir work/ui/game/windows/tab_button_middle_01.sub")
			button.SetOverVisual("d:/ymir work/ui/game/windows/tab_button_middle_02.sub")
			button.SetDownVisual("d:/ymir work/ui/game/windows/tab_button_middle_03.sub")
			button.SetWindowHorizontalAlignCenter()
			button.SetWindowVerticalAlignBottom()
			button.SetPosition(pos, 85)
			button.SetText(text)
			button.SetEvent(lambda arg=i: self.SelectPage(arg))
			button.Show()
			self.pageButtonList.append(button)

			pos += 52
			text += "I"

		self.pageButtonList[0].Down()

	def SelectPage(self, index):

		self.curPageIndex = index

		for btn in self.pageButtonList:
			btn.SetUp()

		self.pageButtonList[index].Down()
		self.RefreshSafebox()

	def __LocalPosToGlobalPos(self, local):
		return self.curPageIndex*safebox.SAFEBOX_PAGE_SIZE + local

	def SetTableSize(self, size):

		pageCount = max(1, size / safebox.SAFEBOX_SLOT_Y_COUNT)
		pageCount = min(3, pageCount)
		size = safebox.SAFEBOX_SLOT_Y_COUNT

		self.__MakePageButton(pageCount)

		self.wndItem.ArrangeSlot(0, safebox.SAFEBOX_SLOT_X_COUNT, size, 32, 32, 0, 0)
		self.wndItem.RefreshSlot()
		self.wndItem.SetSlotBaseImage("d:/ymir work/ui/public/Slot_Base.sub", 1.0, 1.0, 1.0, 1.0)

		wnd_height = 130 + 32 * size
		self.wndBoard.SetSize(self.BOX_WIDTH, wnd_height)
		self.SetSize(self.BOX_WIDTH, wnd_height)
		self.UpdateRect()

	def RefreshSafebox(self):
		getItemID=safebox.GetItemID
		getItemCount=safebox.GetItemCount
		setItemID=self.wndItem.SetItemSlot

		for i in xrange(safebox.SAFEBOX_PAGE_SIZE):
			slotIndex = self.__LocalPosToGlobalPos(i)
			itemCount = getItemCount(slotIndex)
			if itemCount <= 1:
				itemCount = 0
			setItemID(i, getItemID(slotIndex), itemCount)

		self.wndItem.RefreshSlot()

	def RefreshSafeboxMoney(self):
		pass
		#self.wndMoney.SetText(str(safebox.GetMoney()))

	def SetItemToolTip(self, tooltip):
		self.tooltipItem = tooltip

	def Close(self):
		net.SendChatPacket("/safebox_close")

	def CommandCloseSafebox(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

		self.dlgPickMoney.Close()
		self.dlgChangePassword.Close()
		self.Hide()

	## Slot Event
	def SelectEmptySlot(self, selectedSlotPos):

		selectedSlotPos = self.__LocalPosToGlobalPos(selectedSlotPos)

		if mouseModule.mouseController.isAttached():

			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()

			if player.SLOT_TYPE_SAFEBOX == attachedSlotType:

				net.SendSafeboxItemMovePacket(attachedSlotPos, selectedSlotPos, 0)
				#snd.PlaySound("sound/ui/drop.wav")
			else:
				attachedInvenType = player.SlotTypeToInvenType(attachedSlotType)
				if player.RESERVED_WINDOW == attachedInvenType:
					return
				if app.__ENABLE_NEW_OFFLINESHOP__:
					if uinewofflineshop.IsBuildingShop() and uinewofflineshop.IsSaleSlot(attachedInvenType, attachedSlotPos):
						chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OFFLINESHOP_CANT_SELECT_ITEM_DURING_BUILING)
						return


					
				if player.ITEM_MONEY == mouseModule.mouseController.GetAttachedItemIndex():
					net.SendSafeboxSaveMoneyPacket(mouseModule.mouseController.GetAttachedItemCount())
					snd.PlaySound("sound/ui/money.wav")

				else:
					net.SendSafeboxCheckinPacket(attachedInvenType, attachedSlotPos, selectedSlotPos)
					#snd.PlaySound("sound/ui/drop.wav")
			
			mouseModule.mouseController.DeattachObject()

	def SelectItemSlot(self, selectedSlotPos):

		selectedSlotPos = self.__LocalPosToGlobalPos(selectedSlotPos)

		if mouseModule.mouseController.isAttached():

			attachedSlotType = mouseModule.mouseController.GetAttachedType()

			if player.SLOT_TYPE_INVENTORY == attachedSlotType:

				if player.ITEM_MONEY == mouseModule.mouseController.GetAttachedItemIndex():
					net.SendSafeboxSaveMoneyPacket(mouseModule.mouseController.GetAttachedItemCount())
					snd.PlaySound("sound/ui/money.wav")

				else:
					attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
					#net.SendSafeboxCheckinPacket(attachedSlotPos, selectedSlotPos)
					#snd.PlaySound("sound/ui/drop.wav")

			mouseModule.mouseController.DeattachObject()

		else:

			curCursorNum = app.GetCursor()
			if app.SELL == curCursorNum:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SAFEBOX_SELL_DISABLE_SAFEITEM)

			elif app.BUY == curCursorNum:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SHOP_BUY_INFO)

			else:
				selectedItemID = safebox.GetItemID(selectedSlotPos)
				mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_SAFEBOX, selectedSlotPos, selectedItemID)
				snd.PlaySound("sound/ui/pick.wav")

	def UseItemSlot(self, slotIndex):
		mouseModule.mouseController.DeattachObject()
		if app.ITEM_CHECKINOUT_UPDATE:
			slotIndex = self.__LocalPosToGlobalPos(slotIndex)
			net.SendSafeboxCheckoutPacket(slotIndex)
			


	def __ShowToolTip(self, slotIndex):
		if self.tooltipItem:
			self.tooltipItem.SetSafeBoxItem(slotIndex)

	def OverInItem(self, slotIndex):
		slotIndex = self.__LocalPosToGlobalPos(slotIndex)
		self.wndItem.SetUsableItem(False)
		self.__ShowToolTip(slotIndex)

	def OverOutItem(self):
		self.wndItem.SetUsableItem(False)
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OnPickMoney(self, money):
		mouseModule.mouseController.AttachMoney(self, player.SLOT_TYPE_SAFEBOX, money)

	def OnChangePassword(self):
		self.dlgChangePassword.Open()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnUpdate(self):

		USE_SAFEBOX_LIMIT_RANGE = 1000

		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.xSafeBoxStart) > USE_SAFEBOX_LIMIT_RANGE or abs(y - self.ySafeBoxStart) > USE_SAFEBOX_LIMIT_RANGE:
			self.Close()

class MallWindow(ui.ScriptWindow):

	BOX_WIDTH = 176

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.tooltipItem = None
		self.sellingSlotNumber = -1
		self.pageButtonList = []
		self.curPageIndex = 0
		self.isLoaded = 0
		self.xSafeBoxStart = 0
		self.ySafeBoxStart = 0

		self.__LoadWindow()

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Show(self):
		self.__LoadWindow()

		ui.ScriptWindow.Show(self)		

	def Destroy(self):
		self.ClearDictionary()

		self.tooltipItem = None
		self.wndBoard = None
		self.wndItem = None

		self.pageButtonList = []

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		pyScrLoader = ui.PythonScriptLoader()
		pyScrLoader.LoadScriptFile(self, "UIScript/MallWindow.py")

		from _weakref import proxy

		## Item
		wndItem = ui.GridSlotWindow()
		wndItem.SetParent(self)
		wndItem.SetPosition(8, 35)
		wndItem.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
		wndItem.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
		wndItem.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndItem.SetUseSlotEvent(ui.__mem_func__(self.UseItemSlot))
		wndItem.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
		wndItem.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
		wndItem.Show()

		## Close Button
		self.GetChild("TitleBar").SetCloseEvent(ui.__mem_func__(self.Close))
		self.GetChild("ExitButton").SetEvent(ui.__mem_func__(self.Close))

		self.wndItem = wndItem
		self.wndBoard = self.GetChild("board")

		## Initialize
		self.SetTableSize(3)

	def ShowWindow(self, size):

		(self.xSafeBoxStart, self.ySafeBoxStart, z) = player.GetMainCharacterPosition()

		self.SetTableSize(size)
		self.Show()

	def SetTableSize(self, size):

		pageCount = max(1, size / safebox.SAFEBOX_SLOT_Y_COUNT)
		pageCount = min(3, pageCount)
		size = safebox.SAFEBOX_SLOT_Y_COUNT

		self.wndItem.ArrangeSlot(0, safebox.SAFEBOX_SLOT_X_COUNT, size, 32, 32, 0, 0)
		self.wndItem.RefreshSlot()
		self.wndItem.SetSlotBaseImage("d:/ymir work/ui/public/Slot_Base.sub", 1.0, 1.0, 1.0, 1.0)

		self.wndBoard.SetSize(self.BOX_WIDTH, 82 + 32*size)
		self.SetSize(self.BOX_WIDTH, 85 + 32*size)
		self.UpdateRect()

	def RefreshMall(self):
		getItemID=safebox.GetMallItemID
		getItemCount=safebox.GetMallItemCount
		setItemID=self.wndItem.SetItemSlot

		for i in xrange(safebox.GetMallSize()):
			itemID = getItemID(i)
			itemCount = getItemCount(i)
			if itemCount <= 1:
				itemCount = 0
			setItemID(i, itemID, itemCount)

		self.wndItem.RefreshSlot()

	def SetItemToolTip(self, tooltip):
		self.tooltipItem = tooltip

	def Close(self):
		net.SendChatPacket("/mall_close")

	def CommandCloseMall(self):
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

		self.Hide()

	## Slot Event
	def SelectEmptySlot(self, selectedSlotPos):

		if mouseModule.mouseController.isAttached():

			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MALL_CANNOT_INSERT)
			mouseModule.mouseController.DeattachObject()

	def SelectItemSlot(self, selectedSlotPos):

		if mouseModule.mouseController.isAttached():

			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MALL_CANNOT_INSERT)
			mouseModule.mouseController.DeattachObject()

		else:

			curCursorNum = app.GetCursor()
			selectedItemID = safebox.GetMallItemID(selectedSlotPos)
			mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_MALL, selectedSlotPos, selectedItemID)
			snd.PlaySound("sound/ui/pick.wav")

	def UseItemSlot(self, slotIndex):
		mouseModule.mouseController.DeattachObject()
		if app.ITEM_CHECKINOUT_UPDATE:
			net.SendMallCheckoutPacket(slotIndex)


	def __ShowToolTip(self, slotIndex):
		if self.tooltipItem:
			self.tooltipItem.SetMallItem(slotIndex)

	def OverInItem(self, slotIndex):
		self.__ShowToolTip(slotIndex)

	def OverOutItem(self):
		self.wndItem.SetUsableItem(False)
		if self.tooltipItem:
			self.tooltipItem.HideToolTip()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def OnUpdate(self):

		USE_SAFEBOX_LIMIT_RANGE = 1000

		(x, y, z) = player.GetMainCharacterPosition()
		if abs(x - self.xSafeBoxStart) > USE_SAFEBOX_LIMIT_RANGE or abs(y - self.ySafeBoxStart) > USE_SAFEBOX_LIMIT_RANGE:
			self.Close()


if __name__ == "__main__":

	import app
	import wndMgr
	import systemSetting
	import mouseModule
	import grp
	import ui
	import chr
	import background
	import player

	#wndMgr.SetOutlineFlag(True)

	app.SetMouseHandler(mouseModule.mouseController)
	app.SetHairColorEnable(True)
	wndMgr.SetMouseHandler(mouseModule.mouseController)
	wndMgr.SetScreenSize(systemSetting.GetWidth(), systemSetting.GetHeight())
	app.Create("METIN2 CLOSED BETA", systemSetting.GetWidth(), systemSetting.GetHeight(), 1)
	mouseModule.mouseController.Create()


	wnd = SafeboxWindow()
	wnd.ShowWindow(1)
	
	app.Loop()
