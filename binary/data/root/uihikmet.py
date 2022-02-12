import ui
import background
import mouseModule
import player
import net
import snd
import safebox
import chat
import app
import localeInfo
import uiScriptLocale
import shop
import event
import game
import time

class HikmetDialog(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadDialog()
		self.isCreate = False
		self.Bekle = 0

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadDialog(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/hikmetdialog.py")
		except:
			import exception
			exception.Abort("GuvenlikDialog.__LoadDialog.LoadObject")

		try:
			self.titleName = self.GetChild("TitleName")
			self.GetChild("titlebar").SetCloseEvent(ui.__mem_func__(self.OnCancel))
			self.cancelButton = self.GetChild("cancel_button")

			self.guvenlipcaktif = self.GetChild("guvenliaktif_button")
			self.guvenlipcpasif = self.GetChild("guvenlipasif_button")
			self.pcekle = self.GetChild("pcekle_button")
			self.pcsil = self.GetChild("pcsil_button")
		except:
			import exception
			exception.Abort("GuvenlikDialog.__LoadDialog.BindObject")

		self.cancelButton.SetEvent(ui.__mem_func__(self.OnCancel))
		self.cancelButton.Hide()

		self.guvenlipcaktif.SetEvent(ui.__mem_func__(self.OnGuvenliPcAktif))
		self.guvenlipcpasif.SetEvent(ui.__mem_func__(self.OnGuvenliPcPaktif))
		self.pcekle.SetEvent(ui.__mem_func__(self.PcEkle))
		self.pcsil.SetEvent(ui.__mem_func__(self.PcSil))

	def Destroy(self):
		self.ClearDictionary()
		self.titleName = None
		self.cancelButton = None

		self.guvenlipcaktif = None
		self.guvenlipcpasif = None
		self.pcekle = None
		self.pcsil = None

	def SetTitle(self, title):
		self.titleName.SetText(title)

	def ShowDialog(self, isCreate):
		self.SetCenterPosition()
		self.Show()

	def CloseDialog(self):
		self.passwordValue.KillFocus()
		self.Hide()

	def OnGuvenliPcAktif(self):
		if app.GetTime() < self.Bekle - 5:
			pass
		else:
			self.Bekle = app.GetTime()
			net.SendChatPacket("/guvenli_pc AktifEt %s" % (player.GetMacAddress()))
			return True

	def OnGuvenliPcPaktif(self):
		if app.GetTime() < self.Bekle - 5:
			pass
		else:
			self.Bekle = app.GetTime()
			net.SendChatPacket("/guvenli_pc PasifEt %s" % (player.GetMacAddress()))
			return True

	def PcEkle(self):
		if app.GetTime() < self.Bekle - 5:
			pass
		else:
			self.Bekle = app.GetTime()
			net.SendChatPacket("/guvenli_pc PcEkle %s" % (player.GetMacAddress()))
			return True

	def PcSil(self):
		if app.GetTime() < self.Bekle - 5:
			pass
		else:
			self.Bekle = app.GetTime()
			net.SendChatPacket("/guvenli_pc PcSil %s" % (player.GetMacAddress()))
			return True

	def OnCancel(self):
		self.Hide()
		return True
