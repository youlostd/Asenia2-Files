import ui, player, chat

MAX_SKILL_COUNT = 2

class BuffiSkillDialogIyilestirme(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.toolTipSkill = None

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def Destroy(self):
		self.ClearDictionary()
		self.toolTipSkill = None

	def LoadDialog(self):
		try:
			ui.PythonScriptLoader().LoadScriptFile(self, "uiscript/BuffiSkillDialogv2.py")
		except:
			import exception
			exception.Abort("BuffiSkillDialogv2.LoadDialog.LoadObject")
		try:
			self.GetChild("TitleBar").SetCloseEvent(self.Close)
			self.skillSlot = self.GetChild("skillSlot")
		except:
			import exception
			exception.Abort("BuffiSkillDialogv2.LoadDialog.BindObject")

		self.skillSlot.SetOverInItemEvent(ui.__mem_func__(self.__OverInToolTip))
		self.skillSlot.SetOverOutItemEvent(ui.__mem_func__(self.__OverOutToolTip))

	def SetSkillToolTip(self, skillToolTip):
		self.toolTipSkill = skillToolTip

	def Open(self):
		self.RefreshSkillData()
		self.SetCenterPosition()
		x,y = self.GetLocalPosition()
		self.SetPosition(50, y)
		ui.ScriptWindow.Show(self)

	def Close(self):
		if self.toolTipSkill:
			self.toolTipSkill.Hide()

		ui.ScriptWindow.Hide(self)
		return True

	#def OnPressEscapeKey(self):
	#	self.Close()
	#	return True

	def RefreshSkillData(self):
		for i in xrange(MAX_SKILL_COUNT):
			skillIndex = i+248
			slotIndex = player.GetSkillSlotIndex(skillIndex)
			skillGrade = player.GetSkillGrade(slotIndex)
			skillLevel = player.GetSkillLevel(slotIndex)

			self.skillSlot.SetSkillSlotNew(i, skillIndex, skillGrade, skillLevel)
			self.skillSlot.SetSlotCountNew(i, skillGrade, skillLevel)
			self.skillSlot.SetCoverButton(i, "d:/ymir work/ui/public/slot_cover_button_01.sub", "d:/ymir work/ui/public/slot_cover_button_02.sub", "d:/ymir work/ui/public/slot_cover_button_03.sub", "d:/ymir work/ui/public/slot_cover_button_04.sub", False, False)
			self.skillSlot.ShowSlotBaseImage(i)

	def __OverInToolTip(self, slotIndex):
		if not self.toolTipSkill:
			return

		#skillGrade = player.GetSkillGrade(slotIndex)
		self.toolTipSkill.SetSkillOnlyName(slotIndex, slotIndex+248, 0)

	def __OverOutToolTip(self):
		if self.toolTipSkill:
			self.toolTipSkill.HideToolTip()