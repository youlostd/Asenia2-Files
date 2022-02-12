import ui
import net
import app
import chrmgr
import grp
import localeInfo

class NPCEkran(ui.ScriptWindow):

	ModelPreviewBoard = None
	ModelPreview = None
	ModelPreviewText = None

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.__Load()

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		print " -------------------------------------- DELETE GAME OPTION DIALOG"

	def __Initialize(self):
		self.titleBar = 0

	def Destroy(self):
		self.ClearDictionary()

		self.__Initialize()
		print " -------------------------------------- DESTROY GAME OPTION DIALOG"
	
	def __Load_LoadScript(self, fileName):
		try:
			pyScriptLoader = ui.PythonScriptLoader()
			pyScriptLoader.LoadScriptFile(self, fileName)
		except:
			import exception
			exception.Abort("OptionDialog.__Load_LoadScript")

	def __Load_BindObject(self):
		try:
			GetObject = self.GetChild
			self.titleBar = GetObject("titlebar")

		except:
			import exception
			exception.Abort("OptionDialog.__Load_BindObject")

	def __Load(self):
		self.__Load_LoadScript("uiscript/npcekran.py")

		self.__Load_BindObject()

		self.SetCenterPosition()

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))
		
	def __Load_BindObject(self):

		try:
			GetObject = self.GetChild
			self.titleBar = GetObject("titlebar")
			
			self.genel = GetObject("genel")
			self.bilet = GetObject("bilet")
			self.arti = GetObject("arti")
			self.iksir = GetObject("iksir")
			self.tas = GetObject("tas")
			self.bio = GetObject("bio")
			self.gaya = GetObject("gaya")
			self.yil = GetObject("yil")
			self.silah = GetObject("silah")
			self.zirh = GetObject("zirh")
			self.balikci = GetObject("balikci")
			self.gaya2 = GetObject("gaya2")
			
			
			self.genel.SAFE_SetEvent(self.butonlar,3)
			self.bilet.SAFE_SetEvent(self.butonlar2,9010)
			self.arti.SAFE_SetEvent(self.butonlar3,20381)
			self.iksir.SAFE_SetEvent(self.butonlar4,9014)
			self.tas.SAFE_SetEvent(self.butonlar5,9016)
			self.bio.SAFE_SetEvent(self.butonlar6,8)
			self.gaya.SAFE_SetEvent(self.butonlar7,20504)
			self.yil.SAFE_SetEvent(self.butonlar8,13)
			self.silah.SAFE_SetEvent(self.butonlar9,1)
			self.zirh.SAFE_SetEvent(self.butonlar10,4)
			self.balikci.SAFE_SetEvent(self.butonlar11,2)
			self.gaya2.SAFE_SetEvent(self.butonlar12,20505)

		except:
			import exception
			exception.Abort("OptionDialog.__Load_BindObject")

		self.ModelPreviewBoard = ui.ThinBoard()
		self.ModelPreviewBoard.SetParent(self)
		self.ModelPreviewBoard.SetSize(190+10, 210+30)
		self.ModelPreviewBoard.SetPosition(176, 60)
		self.ModelPreviewBoard.Show()

		self.ModelPreview = ui.RenderTarget()
		self.ModelPreview.SetParent(self.ModelPreviewBoard)
		self.ModelPreview.SetSize(190, 210)
		self.ModelPreview.SetPosition(6, 20)
		self.ModelPreview.SetRenderTarget(3)
		self.ModelPreview.SetAutoRotation(0.0)
		self.ModelPreview.Show()

	def butonlar(self, npc):
		self.ModelPreviewBoard = ui.ThinBoard()
		self.ModelPreviewBoard.SetParent(self)
		self.ModelPreviewBoard.SetSize(190+10, 210+30)
		self.ModelPreviewBoard.SetPosition(176, 60)
		self.ModelPreviewBoard.Show()

		self.ModelPreview = ui.RenderTarget()
		self.ModelPreview.SetParent(self.ModelPreviewBoard)
		self.ModelPreview.SetSize(190, 210)
		self.ModelPreview.SetPosition(6, 20)
		self.ModelPreview.SetRenderTarget(3)
		self.ModelPreview.SetAutoRotation(0.0)
		self.ModelPreview.SetRenderTargetData(9003)
		self.ModelPreview.Show()

		net.SendChatPacket("/npcisopen " + str(npc))

	def butonlar2(self, npc):
		self.ModelPreviewBoard = ui.ThinBoard()
		self.ModelPreviewBoard.SetParent(self)
		self.ModelPreviewBoard.SetSize(190+10, 210+30)
		self.ModelPreviewBoard.SetPosition(176, 60)
		self.ModelPreviewBoard.Show()

		self.ModelPreview = ui.RenderTarget()
		self.ModelPreview.SetParent(self.ModelPreviewBoard)
		self.ModelPreview.SetSize(190, 210)
		self.ModelPreview.SetPosition(6, 20)
		self.ModelPreview.SetRenderTarget(3)
		self.ModelPreview.SetAutoRotation(0.0)
		self.ModelPreview.SetRenderTargetData(9010)
		self.ModelPreview.Show()

		net.SendChatPacket("/npcisopen " + str(npc))

	def butonlar3(self, npc):
		self.ModelPreviewBoard = ui.ThinBoard()
		self.ModelPreviewBoard.SetParent(self)
		self.ModelPreviewBoard.SetSize(190+10, 210+30)
		self.ModelPreviewBoard.SetPosition(176, 60)
		self.ModelPreviewBoard.Show()

		self.ModelPreview = ui.RenderTarget()
		self.ModelPreview.SetParent(self.ModelPreviewBoard)
		self.ModelPreview.SetSize(190, 210)
		self.ModelPreview.SetPosition(6, 20)
		self.ModelPreview.SetRenderTarget(3)
		self.ModelPreview.SetAutoRotation(0.0)
		self.ModelPreview.SetRenderTargetData(20381)
		self.ModelPreview.Show()

		net.SendChatPacket("/npcisopen " + str(npc))

	def butonlar4(self, npc):
		self.ModelPreviewBoard = ui.ThinBoard()
		self.ModelPreviewBoard.SetParent(self)
		self.ModelPreviewBoard.SetSize(190+10, 210+30)
		self.ModelPreviewBoard.SetPosition(176, 60)
		self.ModelPreviewBoard.Show()

		self.ModelPreview = ui.RenderTarget()
		self.ModelPreview.SetParent(self.ModelPreviewBoard)
		self.ModelPreview.SetSize(190, 210)
		self.ModelPreview.SetPosition(6, 20)
		self.ModelPreview.SetRenderTarget(3)
		self.ModelPreview.SetAutoRotation(0.0)
		self.ModelPreview.SetRenderTargetData(9014)
		self.ModelPreview.Show()

		net.SendChatPacket("/npcisopen " + str(npc))

	def butonlar5(self, npc):
		self.ModelPreviewBoard = ui.ThinBoard()
		self.ModelPreviewBoard.SetParent(self)
		self.ModelPreviewBoard.SetSize(190+10, 210+30)
		self.ModelPreviewBoard.SetPosition(176, 60)
		self.ModelPreviewBoard.Show()

		self.ModelPreview = ui.RenderTarget()
		self.ModelPreview.SetParent(self.ModelPreviewBoard)
		self.ModelPreview.SetSize(190, 210)
		self.ModelPreview.SetPosition(6, 20)
		self.ModelPreview.SetRenderTarget(3)
		self.ModelPreview.SetAutoRotation(0.0)
		self.ModelPreview.SetRenderTargetData(9016)
		self.ModelPreview.Show()

		net.SendChatPacket("/npcisopen " + str(npc))

	def butonlar6(self, npc):
		self.ModelPreviewBoard = ui.ThinBoard()
		self.ModelPreviewBoard.SetParent(self)
		self.ModelPreviewBoard.SetSize(190+10, 210+30)
		self.ModelPreviewBoard.SetPosition(176, 60)
		self.ModelPreviewBoard.Show()

		self.ModelPreview = ui.RenderTarget()
		self.ModelPreview.SetParent(self.ModelPreviewBoard)
		self.ModelPreview.SetSize(190, 210)
		self.ModelPreview.SetPosition(6, 20)
		self.ModelPreview.SetRenderTarget(3)
		self.ModelPreview.SetAutoRotation(0.0)
		self.ModelPreview.SetRenderTargetData(9004)
		self.ModelPreview.Show()

		net.SendChatPacket("/npcisopen " + str(npc))


	def butonlar7(self, npc):
		self.ModelPreviewBoard = ui.ThinBoard()
		self.ModelPreviewBoard.SetParent(self)
		self.ModelPreviewBoard.SetSize(190+10, 210+30)
		self.ModelPreviewBoard.SetPosition(176, 60)
		self.ModelPreviewBoard.Show()

		self.ModelPreview = ui.RenderTarget()
		self.ModelPreview.SetParent(self.ModelPreviewBoard)
		self.ModelPreview.SetSize(190, 210)
		self.ModelPreview.SetPosition(6, 20)
		self.ModelPreview.SetRenderTarget(3)
		self.ModelPreview.SetAutoRotation(0.0)
		self.ModelPreview.SetRenderTargetData(20504)
		self.ModelPreview.Show()

		net.SendChatPacket("/npcisopen " + str(npc))

	def butonlar8(self, npc):
		self.ModelPreviewBoard = ui.ThinBoard()
		self.ModelPreviewBoard.SetParent(self)
		self.ModelPreviewBoard.SetSize(190+10, 210+30)
		self.ModelPreviewBoard.SetPosition(176, 60)
		self.ModelPreviewBoard.Show()

		self.ModelPreview = ui.RenderTarget()
		self.ModelPreview.SetParent(self.ModelPreviewBoard)
		self.ModelPreview.SetSize(190, 210)
		self.ModelPreview.SetPosition(6, 20)
		self.ModelPreview.SetRenderTarget(3)
		self.ModelPreview.SetAutoRotation(0.0)
		self.ModelPreview.SetRenderTargetData(20001)
		self.ModelPreview.Show()

		net.SendChatPacket("/npcisopen " + str(npc))

	def butonlar9(self, npc):
		self.ModelPreviewBoard = ui.ThinBoard()
		self.ModelPreviewBoard.SetParent(self)
		self.ModelPreviewBoard.SetSize(190+10, 210+30)
		self.ModelPreviewBoard.SetPosition(176, 60)
		self.ModelPreviewBoard.Show()

		self.ModelPreview = ui.RenderTarget()
		self.ModelPreview.SetParent(self.ModelPreviewBoard)
		self.ModelPreview.SetSize(190, 210)
		self.ModelPreview.SetPosition(6, 20)
		self.ModelPreview.SetRenderTarget(3)
		self.ModelPreview.SetAutoRotation(0.0)
		self.ModelPreview.SetRenderTargetData(9001)
		self.ModelPreview.Show()

		net.SendChatPacket("/npcisopen " + str(npc))

	def butonlar10(self, npc):
		self.ModelPreviewBoard = ui.ThinBoard()
		self.ModelPreviewBoard.SetParent(self)
		self.ModelPreviewBoard.SetSize(190+10, 210+30)
		self.ModelPreviewBoard.SetPosition(176, 60)
		self.ModelPreviewBoard.Show()

		self.ModelPreview = ui.RenderTarget()
		self.ModelPreview.SetParent(self.ModelPreviewBoard)
		self.ModelPreview.SetSize(190, 210)
		self.ModelPreview.SetPosition(6, 20)
		self.ModelPreview.SetRenderTarget(3)
		self.ModelPreview.SetAutoRotation(0.0)
		self.ModelPreview.SetRenderTargetData(9002)
		self.ModelPreview.Show()

		net.SendChatPacket("/npcisopen " + str(npc))

	def butonlar11(self, npc):
		self.ModelPreviewBoard = ui.ThinBoard()
		self.ModelPreviewBoard.SetParent(self)
		self.ModelPreviewBoard.SetSize(190+10, 210+30)
		self.ModelPreviewBoard.SetPosition(176, 60)
		self.ModelPreviewBoard.Show()

		self.ModelPreview = ui.RenderTarget()
		self.ModelPreview.SetParent(self.ModelPreviewBoard)
		self.ModelPreview.SetSize(190, 210)
		self.ModelPreview.SetPosition(6, 20)
		self.ModelPreview.SetRenderTarget(3)
		self.ModelPreview.SetAutoRotation(0.0)
		self.ModelPreview.SetRenderTargetData(9009)
		self.ModelPreview.Show()


		net.SendChatPacket("/npcisopen " + str(npc))

	def butonlar12(self, npc):
		self.ModelPreviewBoard = ui.ThinBoard()
		self.ModelPreviewBoard.SetParent(self)
		self.ModelPreviewBoard.SetSize(190+10, 210+30)
		self.ModelPreviewBoard.SetPosition(176, 60)
		self.ModelPreviewBoard.Show()

		self.ModelPreview = ui.RenderTarget()
		self.ModelPreview.SetParent(self.ModelPreviewBoard)
		self.ModelPreview.SetSize(190, 210)
		self.ModelPreview.SetPosition(6, 20)
		self.ModelPreview.SetRenderTarget(3)
		self.ModelPreview.SetAutoRotation(0.0)
		self.ModelPreview.SetRenderTargetData(20504)
		self.ModelPreview.Show()

		net.SendChatPacket("/npcisopen " + str(npc))

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def Show(self):
		ui.ScriptWindow.Show(self)

	def Close(self):
		self.Hide()
