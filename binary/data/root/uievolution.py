import app
import net
import player
import item
import ui
import chat
import uiToolTip
import mousemodule
import localeInfo
import translate
import uicommon
import constInfo

class EvolutionDialog(ui.ScriptWindow):

	makeEvolutionPercentage = (0, 50, 30, 20, 10)

	makeEvolutionGold = (0, 5000000, 10000000, 15000000, 20000000)
	
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.children = []
		self.__LoadScript()
		self.scrollItemPos = 0
		self.targetItemPos = 0
		self.dialogHeight = 0

	def __LoadScript(self):
		self.__LoadQuestionDialog()
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/refinedialog2.py")
		except:
			import exception
			exception.Abort("EvolutionDialog.__LoadScript.LoadObject")
		try:
			self.board = self.GetChild("Board")
			self.titleBar = self.GetChild("TitleBar")
			self.successPercentage = self.GetChild("SuccessPercentage")
			self.probText = self.GetChild("SuccessPercentage")
			self.costText = self.GetChild("Cost")
			self.GetChild("AcceptButton").SetEvent(self.OpenQuestionDialog)
			self.GetChild("CancelButton").SetEvent(self.Close)
		except:
			import exception
			exception.Abort("EvolutionDialog.__LoadScript.BindObject")

		self.successPercentage.Show()

		toolTip = uiToolTip.ItemToolTip()
		toolTip.SetParent(self)
		toolTip.SetPosition(15, 38)
		toolTip.SetFollow(False)
		toolTip.Show()
		self.toolTip = toolTip
		self.slotList = []
		for i in xrange(3):
			slot = self.__MakeSlot()
			slot.SetParent(toolTip)
			slot.SetWindowVerticalAlignCenter()
			self.slotList.append(slot)

		itemImage = self.__MakeItemImage()
		itemImage.SetParent(toolTip)
		itemImage.SetWindowVerticalAlignCenter()
		itemImage.SetPosition(-35, 0)
		self.itemImage = itemImage

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __MakeSlot(self):
		slot = ui.ImageBox()
		slot.LoadImage("d:/ymir work/ui/public/slot_base.sub")
		slot.Show()
		self.children.append(slot)
		return slot

	def __MakeItemImage(self):
		itemImage = ui.ImageBox()
		itemImage.Show()
		self.children.append(itemImage)
		return itemImage

	def __MakeThinBoard(self):
		thinBoard = ui.ThinBoard()
		thinBoard.SetParent(self)
		thinBoard.Show()
		self.children.append(thinBoard)
		return thinBoard

	def __LoadQuestionDialog(self):
		self.dlgQuestion = ui.ScriptWindow()

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self.dlgQuestion, "uiscript/questiondialog2.py")
		except:
			import exception
			exception.Abort("EvolutionDialog.__LoadQuestionDialog.LoadScript")

		try:
			GetObject=self.dlgQuestion.GetChild
			GetObject("message1").SetText(translate.EVOLUTION_WARNING)
			GetObject("message2").SetText(localeInfo.REFINE_WARNING2)
			GetObject("accept").SetEvent(ui.__mem_func__(self.Accept))
			GetObject("cancel").SetEvent(ui.__mem_func__(self.dlgQuestion.Hide))
		except:
			import exception
			exception.Abort("SelectCharacterWindow.__LoadQuestionDialog.BindObject")

	def Destroy(self):
		self.ClearDictionary()
		self.board = 0
		self.successPercentage = 0
		self.titleBar = 0
		self.toolTip = 0
		self.dlgQuestion = 0
		self.successPercentage = None
		self.slotList = []
		self.children = []

	def GetEvolSuccessPercentage(self, scrollSlotIndex, itemSlotIndex):
		evolution = int(player.GetItemEvolution(itemSlotIndex))
		evograde=evolution

		return self.makeEvolutionPercentage[int(evograde)+1]

	def GetEvolGold(self, scrollSlotIndex, itemSlotIndex):
		evolution = int(player.GetItemEvolution(itemSlotIndex))
		evograde=evolution

		return self.makeEvolutionGold[int(evograde)+1]

	def Open(self, scrollItemPos, targetItemPos):
		self.scrollItemPos = scrollItemPos
		self.targetItemPos = targetItemPos

		percentage = self.GetEvolSuccessPercentage(scrollItemPos, targetItemPos)
		evogold = self.GetEvolGold(scrollItemPos, targetItemPos)
	
		if int(percentage) > 100:
			percentage=100
		if int(evogold) > 1000000001:
			evogold=1000000001

		self.probText.SetText(translate.EVOLUTION_SUCCESS_PROBALITY % int(percentage))
		self.costText.SetText(translate.EVOLUTION_GOLD % int(evogold))

		itemIndex = player.GetItemIndex(targetItemPos)
		self.toolTip.ClearToolTip()
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(player.GetItemMetinSocket(targetItemPos, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(player.GetItemAttribute(targetItemPos, i))
		evolution = player.GetItemEvolution(targetItemPos)
		skillSlot = []

		self.toolTip.AddItemData(itemIndex, metinSlot, attrSlot, evolution+1, player.INVENTORY, -1,)
		item.SelectItem(itemIndex)
		self.itemImage.LoadImage(item.GetIconImageFileName())
		xSlotCount, ySlotCount = item.GetItemSize()
		for slot in self.slotList:
			slot.Hide()
		for i in xrange(min(3, ySlotCount)):
			self.slotList[i].SetPosition(-35, i*32 - (ySlotCount-1)*16)
			self.slotList[i].Show()

		self.dialogHeight = self.toolTip.GetHeight() + 46
		self.UpdateDialog()
		self.SetTop()
		self.Show()
		evolution += 1
		self.AppendMaterial(70251, evolution)
		self.AppendMaterial(70252, evolution)
		self.AppendMaterial(70253, evolution)
		self.AppendMaterial(70254, evolution)

	def AppendMaterial(self, vnum, count):
		slot = self.__MakeSlot()
		slot.SetParent(self)
		slot.SetPosition(15, self.dialogHeight)

		itemImage = self.__MakeItemImage()
		itemImage.SetParent(slot)
		item.SelectItem(vnum)
		itemImage.LoadImage(item.GetIconImageFileName())

		thinBoard = self.__MakeThinBoard()
		thinBoard.SetPosition(50, self.dialogHeight)
		thinBoard.SetSize(191, 20)

		textLine = ui.TextLine()
		textLine.SetParent(thinBoard)
		textLine.SetFontName(localeInfo.UI_DEF_FONT)
		textLine.SetPackedFontColor(0xffdddddd)
		textLine.SetText("%s x %02d" % (item.GetItemName(), count))
		textLine.SetOutline()
		textLine.SetFeather(False)
		textLine.SetWindowVerticalAlignCenter()
		textLine.SetVerticalAlignCenter()

		textLine.SetPosition(15, 0)
		textLine.Show()
		self.children.append(textLine)

		self.dialogHeight += 34
		self.UpdateDialog()

	def UpdateDialog(self):
		newWidth = self.toolTip.GetWidth() + 60
		newHeight = self.dialogHeight + 105

		self.board.SetSize(newWidth, newHeight)
		self.toolTip.SetPosition(15 + 35, 38)
		self.titleBar.SetWidth(newWidth-15)
		self.SetSize(newWidth, newHeight)

		(x, y) = self.GetLocalPosition()
		self.SetPosition(x, y)

	def OpenQuestionDialog(self):
		self.dlgQuestion.SetTop()
		self.dlgQuestion.Show()

	def Accept(self):
		net.SendItemUseToItemPacket(self.scrollItemPos, self.targetItemPos)
		self.Close()

	def Close(self):
		self.dlgQuestion.Hide()
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()
		return True