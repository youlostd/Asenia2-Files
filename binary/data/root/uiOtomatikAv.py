import ui,app ,localeInfo ,constInfo ,player ,net ,chr ,wndMgr
import mouseModule, guild, skill, item, uiToolTip
DOSYA_YOLU = "UserData"


class AutoWindow(ui.ScriptWindow):
	AUTO_COOLTIME_POS_Y = 4
	AUTO_COOLTIME_POS_X = 4
	AUTO_COOLTIME_MAX = AUTO_COOLTIME_POS_Y * AUTO_COOLTIME_POS_X
	AUTO_ONOFF_START = 1
	AUTO_ONOFF_ATTACK = 2
	AUTO_ONOFF_SKILL = 3
	AUTO_ONOFF_POSITION = 4
	AUTO_ONOFF_AUTO_RANGE = 5
	AUTO_ONOFF_ATTACK_TYPE = 6

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.isloded = 0
		self.acikMi = 0
		self.tooltipSkill = 0
		self.tooltipItem = 0
		self.otoAvAcKapa = 0
		self.otoAvSlotIndex = {}
		self.timeeditlist = {}
		self.acKapaButtonlar =[]
		self.saveButton = 0
		self.otoAvSlot = None
		self.AutoSkillClearButton = None
		self.AutoPositionClearButton = None
		self.AutoAllClearButton = None
		self.AutoToolTipButton = None
		self.AutoToolTip = None
		for i in xrange(player.OTOMATIK_AV_SKILL_MAX_NUM):
			self.otoAvSlotIndex[i] = 0
		for i in range(player.OTOMATIK_AV_ITEM_SLOT_START,player.OTOMATIK_AV_ITEM_SLOT_END):
			self.otoAvSlotIndex[i] = 0
		self.AutoSystemToolTipList = [localeInfo.AUTO_TOOLTIP_LINE1, 
		localeInfo.AUTO_TOOLTIP_LINE2, 
		localeInfo.AUTO_TOOLTIP_LINE3,
		localeInfo.AUTO_TOOLTIP_LINE4,
		localeInfo.AUTO_TOOLTIP_LINE5]
		self.closegame = False
		self.LoadAutoWindow()
		self.isFirstReadFile = False

	def __del__(self):
		ui.ScriptWindow.__del__(self)
		self.isloded = 0
		self.acikMi = 0
		self.tooltipSkill = 0
		self.tooltipItem = 0
		self.otoAvAcKapa = 0
		self.otoAvSlotIndex = {}
		self.timeeditlist = {}
		self.acKapaButtonlar =[]
		self.saveButton = 0
		self.otoAvSlot = None
		self.AutoSkillClearButton = None
		self.AutoPositionClearButton = None
		self.AutoAllClearButton = None
		self.AutoToolTipButton = None
		self.AutoToolTip = None
		self.closegame = False
		self.isFirstReadFile = False

	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/AutoWindow.py")
			self.GetChild("board").SetCloseEvent(ui.__mem_func__(self.Close))

			self.saveButton = self.GetChild("AutoSaveButton")
			self.saveButton.SetEvent(ui.__mem_func__(self.SaveAutoInfo))

			baslatButton = self.GetChild("AutoStartOnButton")
			baslatButton.SetEvent(ui.__mem_func__(self.AutoOnOff), 1, self.AUTO_ONOFF_START, 0)
			self.acKapaButtonlar.append(baslatButton)

			durdurButton = self.GetChild("AutoStartOffButton")
			durdurButton.SetEvent(ui.__mem_func__(self.AutoOnOff), 0, self.AUTO_ONOFF_START, 1)
			durdurButton.Down()
			durdurButton.Disable()
			self.acKapaButtonlar.append(durdurButton)

			saldiriAcButton = self.GetChild("AutoAttackOnButton")
			saldiriAcButton.SetEvent(ui.__mem_func__(self.AutoOnOff), 1, self.AUTO_ONOFF_ATTACK, 2)
			self.acKapaButtonlar.append(saldiriAcButton)			
			#
			saldiriKapatButton = self.GetChild("AutoAttackOffButton")
			saldiriKapatButton.SetEvent(ui.__mem_func__(self.AutoOnOff), 0, self.AUTO_ONOFF_ATTACK, 3)
			saldiriKapatButton.Down()
			saldiriKapatButton.Disable()
			self.acKapaButtonlar.append(saldiriKapatButton)

			beceriAcButton = self.GetChild("AutoSkillOnButton")
			beceriAcButton.SetEvent(ui.__mem_func__(self.AutoOnOff), 1, self.AUTO_ONOFF_SKILL, 4)
			self.acKapaButtonlar.append(beceriAcButton)
			#
			beceriKapatButton = self.GetChild("AutoSkillOffButton")
			beceriKapatButton.SetEvent(ui.__mem_func__(self.AutoOnOff), 0, self.AUTO_ONOFF_SKILL, 5)
			beceriKapatButton.Down()
			beceriKapatButton.Disable()
			self.acKapaButtonlar.append(beceriKapatButton)
			
			iksirAcButton = self.GetChild("AutoPositionlOnButton")
			iksirAcButton.SetEvent(ui.__mem_func__(self.AutoOnOff), 1, self.AUTO_ONOFF_POSITION, 6)
			self.acKapaButtonlar.append(iksirAcButton)
			#
			iksirKapatButton = self.GetChild("AutoPositionlOffButton")
			iksirKapatButton.SetEvent(ui.__mem_func__(self.AutoOnOff), 0, self.AUTO_ONOFF_POSITION, 7)
			iksirKapatButton.Down()
			iksirKapatButton.Disable()
			self.acKapaButtonlar.append(iksirKapatButton)

			odakAcButton = self.GetChild("AutoRangeOnButton")
			odakAcButton.SetEvent(ui.__mem_func__(self.AutoOnOff), 1, self.AUTO_ONOFF_AUTO_RANGE, 8)
			self.acKapaButtonlar.append(odakAcButton)
			#
			odakKapaButton = self.GetChild("AutoRangeOffButton")
			odakKapaButton.SetEvent(ui.__mem_func__(self.AutoOnOff), 0, self.AUTO_ONOFF_AUTO_RANGE, 9)
			odakKapaButton.Down()
			odakKapaButton.Disable()
			self.acKapaButtonlar.append(odakKapaButton)
			
			slotOnButton = self.GetChild("AutoSlotOnButton")
			slotOnButton.SetEvent(ui.__mem_func__(self.AutoOnOff), 1, self.AUTO_ONOFF_ATTACK_TYPE, 10)
			slotOnButton.Down()
			slotOnButton.Disable()
			self.acKapaButtonlar.append(slotOnButton)
			#
			metinOnButton = self.GetChild("AutoMetinOnButton")
			metinOnButton.SetEvent(ui.__mem_func__(self.AutoOnOff), 0, self.AUTO_ONOFF_ATTACK_TYPE, 11)
			self.acKapaButtonlar.append(metinOnButton)

			self.AutoSkillClearButton = self.GetChild("AutoSkillClearButton")
			self.AutoSkillClearButton.SetEvent(ui.__mem_func__(self.AutoSkillClear))
			self.AutoPositionClearButton = self.GetChild("AutoPositionClearButton")
			self.AutoPositionClearButton.SetEvent(ui.__mem_func__(self.AutoPositionClear))
			self.AutoAllClearButton = self.GetChild("AutoAllClearButton")
			self.AutoAllClearButton.SetEvent(ui.__mem_func__(self.AutoAllClear))

			for x in xrange(self.AUTO_COOLTIME_MAX):
				childname = "editline" + str(x)
				self.timeeditlist[x] = self.GetChild(childname)
				self.timeeditlist[x].SetEscapeEvent(ui.__mem_func__(self.Close))

			self.otoAvSlot = self.GetChild("Auto_Active_Skill_Slot_Table")
			self.otoAvSlot.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
			self.otoAvSlot.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectActiveSkillEmptySlot))
			self.otoAvSlot.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectActiveSkillSlot))
			self.otoAvSlot.SetOverInItemEvent(ui.__mem_func__(self.OverActiveSkillSlot))
			self.otoAvSlot.SetOverOutItemEvent(ui.__mem_func__(self.OverSkillSlotOutItem))
			self.otoAvSlot.Show()

			self.AutoToolTipButton = self.GetChild("AutoToolTIpButton")
			self.AutoToolTip = self.__CreateGameTypeToolTip(localeInfo.AUTO_TOOLTIP_TITLE,self.AutoSystemToolTipList)
			self.AutoToolTip.SetTop()
			self.AutoToolTipButton.SetToolTipWindow(self.AutoToolTip)

		except:
			import exception
			exception.Abort("AutoWindow.__LoadWindow.UIScript/AutoWindow.py")

	def __CreateGameTypeToolTip(self, title, descList):
		toolTip = uiToolTip.ToolTip()
		toolTip.SetTitle(title)
		toolTip.AppendSpace(5)

		for desc in descList:toolTip.AutoAppendTextLine(desc)

		toolTip.AlignHorizonalCenter()
		toolTip.SetTop()
		return toolTip

	def AutoSkillClear(self):
		if self.GetAutoStartonoff() == False:
			for i in xrange(player.OTOMATIK_AV_SKILL_MAX_NUM):
				player.OtoAvSlottanSil(i)
				self.otoAvSlotIndex[i] = 0
			self.BeceriSlotlariYenile()

	def AutoPositionClear(self):
		if self.GetAutoStartonoff() == False:
			for i in range(player.OTOMATIK_AV_ITEM_SLOT_START,player.OTOMATIK_AV_ITEM_SLOT_END):
				player.OtoAvSlottanSil(i)
				self.otoAvSlotIndex[i] = 0
			self.IksirSlotlariYenile()

	def AutoAllClear(self):
		if self.GetAutoStartonoff() == False:
			self.AutoSkillClear()
			self.AutoPositionClear()
			for i in xrange(player.OTOMATIK_AV_SKILL_MAX_NUM):self.otoAvSlotIndex[i] = 0
			for i in range(player.OTOMATIK_AV_ITEM_SLOT_START,player.OTOMATIK_AV_ITEM_SLOT_END):self.otoAvSlotIndex[i] = 0

	def IsNumberic(self, text) :
		try :
			int(text)
			return True
		except ValueError :
			return False

	def CheckCooltimeText(self, cooltime):
		if cooltime == "":return 0
		if not self.IsNumberic(cooltime):return 0
		return cooltime

	def AutoOnOff(self, onoff,type,number,command = False):
		if not self.isloded:return
		if type == self.AUTO_ONOFF_START:
			if onoff == 1:
				if not player.OtoAvBaslayabilirMi(): return
				for i in xrange(player.OTOMATIK_AV_SKILL_MAX_NUM):
					cooltime = self.timeeditlist[i].GetText()
					cooltime = self.CheckCooltimeText(cooltime)
					cooltime = player.SlotDolumSuresiKontrol(i,self.otoAvSlotIndex[i],int(cooltime))
					# if self.otoAvSlotIndex[i] == 0:self.timeeditlist[i].SetText("")
					if not cooltime == 0:
						player.OtoAvSlotDSAta(i,int(cooltime))
						self.timeeditlist[i].SetText(str(cooltime))
				for i in range(player.OTOMATIK_AV_ITEM_SLOT_START,player.OTOMATIK_AV_ITEM_SLOT_END):
					cooltime = self.timeeditlist[i-1].GetText()
					cooltime = self.CheckCooltimeText(cooltime)
					cooltime = player.SlotDolumSuresiKontrol(i,self.otoAvSlotIndex[i],int(cooltime))
					if not cooltime == 0:
						player.OtoAvSlotDSAta(i,int(cooltime))
						self.timeeditlist[i-1].SetText(str(cooltime))
			else:
				for i in range(player.OTOMATIK_AV_ITEM_SLOT_START,player.OTOMATIK_AV_ITEM_SLOT_END):
					self.SetAutoCooltime(i,0)

			player.OtoAvDurumAta(onoff,command)
			self.otoAvAcKapa = onoff

		elif type == self.AUTO_ONOFF_ATTACK:player.OtoAvSaldiriAta(onoff)
		elif type == self.AUTO_ONOFF_SKILL:player.OtoAvBeceriAta(onoff)
		elif type == self.AUTO_ONOFF_POSITION:player.OtoAvIksirAta(onoff)
		elif type == self.AUTO_ONOFF_AUTO_RANGE:player.OtoAvOdakAta(onoff)
		elif type == self.AUTO_ONOFF_ATTACK_TYPE:player.OtoAvSaldiriTipi(onoff)

		if command == True:
			if onoff == False:
				self.Close()
				return

		self.acKapaButtonlar[number].Down()
		self.acKapaButtonlar[number].Disable()
		if onoff == 1:number = number+1
		else:number = number-1
		self.acKapaButtonlar[number].SetUp()
		self.acKapaButtonlar[number].Enable()

	def LoadAutoWindow(self):
		if self.isloded == 0:
			self.isloded = 1
			self.__LoadWindow()
			self.SetCenterPosition()
			self.AutoOnOff(1, self.AUTO_ONOFF_ATTACK_TYPE, 10)
			# self.ReadAutoInfo()

	def Show(self):
		if self.isloded == 0:
			self.isloded = 1
			self.__LoadWindow()
			self.SetCenterPosition()

		self.SetTop()
		self.ReadAutoInfo()
		self.IksirSlotlariYenile()
		self.BeceriSlotlariYenile()
		self.acikMi = 1
		
		# if not item.CheckAffect(chr.NEW_AFFECT_AUTO_USE,0):
			# for i in range(4,7):
				# self.acKapaButtonlar[i].Down()
				# self.acKapaButtonlar[i].Disable()
			# chr.AutoSkillOnOff(0)
			# chr.AutoPositionOnOff(0)
		# if not chr.GetAutoOnOff():
			# return
		# else:
		ui.ScriptWindow.Show(self)

	def ReadAutoInfo(self):
		import os
		if (str)(chr.GetName()) == "0": return
		dosyaAdi = DOSYA_YOLU + "/" + chr.GetName()

		if os.path.exists(dosyaAdi): veriTokenler = open(dosyaAdi, "r").read().split()
		else: return

		satirSayi = len(veriTokenler) / 2
		sIndex = 0
		if satirSayi > 0:
			for xSatir in xrange(satirSayi):
				# import chat
				slotID = int(str(veriTokenler[sIndex]))
				slotCD = int(str(veriTokenler[sIndex+1]))
				# chat.AppendChat(1,"xSatir: %s , slotID: %s, cD: %s" % (str(xSatir), str(slotID), str(slotCD)))
				if slotID != 0:
					if xSatir < player.OTOMATIK_AV_SKILL_MAX_NUM: player.OtoAvSlotaBeceriEkle(xSatir, slotID)
					else: player.OtoAvSlotaPotEkle(xSatir, slotID)
				player.OtoAvSlotDSAta(xSatir, slotCD)
				sIndex += 2

		# app.CloseTextFile(handle)
		self.isFirstReadFile = True

		self.IksirSlotlariYenile()
		self.BeceriSlotlariYenile()
		
	def SlotKontrol(self):
		for i in xrange(player.OTOMATIK_AV_SKILL_MAX_NUM):
			cooltime = self.timeeditlist[i].GetText()
			cooltime = self.CheckCooltimeText(cooltime)
			cooltime = player.SlotDolumSuresiKontrol(i,self.otoAvSlotIndex[i],int(cooltime))
			import dbg
			if self.otoAvSlotIndex[i] == 0:self.timeeditlist[i].SetText("")
			if not cooltime == 0:
				player.OtoAvSlotDSAta(i,int(cooltime))
				self.timeeditlist[i].SetText(str(cooltime))
		for i in range(player.OTOMATIK_AV_ITEM_SLOT_START+1, player.OTOMATIK_AV_ITEM_SLOT_END):
			cooltime = self.timeeditlist[i-1].GetText()
			cooltime = self.CheckCooltimeText(cooltime)
			cooltime = player.SlotDolumSuresiKontrol(i,self.otoAvSlotIndex[i],int(cooltime))
			if not cooltime == 0:
				player.OtoAvSlotDSAta(i,int(cooltime))
				self.timeeditlist[i-1].SetText(str(cooltime))

	def SaveAutoInfo(self):
		if (str)(chr.GetName()) == "0":return
		self.SlotKontrol()
		import os
		if os.path.exists(DOSYA_YOLU) is False:os.makedirs(DOSYA_YOLU)
		otoAvDosya = open(DOSYA_YOLU + "/" + chr.GetName(), 'w')
		
		for slotIdx in xrange(player.OTOMATIK_AV_ITEM_SLOT_END):
			iPos = player.OtoAvSlotAl(slotIdx)
			if iPos != 0:
				strLine = str(iPos) + '\n'
				otoAvDosya.write(strLine)
				dSure = player.OtoAvSlotDolumSuresiAl(slotIdx)
				if dSure != 0: 
					strLine = str(dSure) + '\n'
					otoAvDosya.write(strLine)
			else:
				otoAvDosya.write('0\n')
				otoAvDosya.write('0\n')

		otoAvDosya.close()

	def Close(self):
		self.Hide()
		self.acikMi = 0
		# self.SaveAutoInfo()
		self.EditLineKillFocus()

	def EditLineKillFocus(self):
		for x in xrange(self.AUTO_COOLTIME_MAX):self.timeeditlist[x].KillFocus()

	def Destroy(self):
		self.isloded = 0
		self.Hide()
		if 0 != self.tooltipSkill:self.tooltipSkill.HideToolTip()

	def OnActivateSkill(self):
		if self.acikMi:self.BeceriSlotlariYenile()

	def OnDeactivateSkill(self, slotindex):
		if self.acikMi:
			for i in xrange(player.OTOMATIK_AV_SKILL_MAX_NUM):
				(Position) = player.OtoAvSlotAl(i)
				if slotindex == Position:self.otoAvSlot.DeactivateSlot(i)

	def OnUseSkill(self, slotindex, coolTime):
		if self.acikMi:self.BeceriSlotlariYenile()

	def SetSkillToolTip(self, tooltip):
		self.tooltipSkill = tooltip

	def SetItemToolTip(self, tooltip):
		self.tooltipItem = tooltip

	def SetAutoCooltime(self, slotindex, cooltime):
		self.otoAvSlot.SetSlotCoolTime(slotindex, cooltime, 0)

	def SetCloseGame(self):
		self.closegame = True

	def GetAutoStartonoff(self):
		return self.otoAvAcKapa

	def IksirSlotlariYenile(self):
		# if not self.otoAvSlot:return
		# if self.closegame:return

		for slotindex in range(player.OTOMATIK_AV_ITEM_SLOT_START + 1,player.OTOMATIK_AV_ITEM_SLOT_END):
			iPos = player.OtoAvSlotAl(slotindex)
			if iPos == 0:
				self.otoAvSlot.ClearSlot(slotindex)
				self.timeeditlist[slotindex-1].SetText("")
				self.otoAvSlotIndex[slotindex] = 0
				# import chat
				# chat.AppendChat(1,str(slotindex))
				continue


			itemIndex = player.GetItemIndex(iPos)
			itemCount = player.GetItemCount(iPos)

			if itemCount <= 1:itemCount = 0

			self.otoAvSlot.SetItemSlot(slotindex, itemIndex, itemCount)
			self.otoAvSlotIndex[slotindex] = iPos

			coolTime = player.OtoAvSlotDolumSuresiAl(slotindex)
			if self.timeeditlist[slotindex-1].GetText() == "":self.timeeditlist[slotindex-1].SetText(str(coolTime))

			if itemIndex == 0:
				self.otoAvSlot.ClearSlot(slotindex)
				self.timeeditlist[slotindex-1].SetText("")
				player.OtoAvSlottanSil(slotindex)
				# self.IksirSlotlariYenile()
		self.otoAvSlot.RefreshSlot()

		# if self.isFirstReadFile:self.SaveAutoInfo()
		# else:self.ReadAutoInfo()

	def BeceriSlotlariYenile(self):
		for slotindex in xrange(player.OTOMATIK_AV_SKILL_MAX_NUM):
			sPos = player.OtoAvSlotAl(slotindex)
			if sPos == 0:
				self.otoAvSlot.ClearSlot(slotindex)
				self.timeeditlist[slotindex].SetText("")
				# import chat
				# chat.AppendChat(1,str(sPos)+" | "+str(slotindex))
				self.otoAvSlotIndex[slotindex] = 0
				continue
	
			skillIndex = player.GetSkillIndex(sPos)
			if 0 == skillIndex:self.otoAvSlot.ClearSlot(slotindex)

			skillType = skill.GetSkillType(skillIndex)
			if skill.SKILL_TYPE_GUILD == skillType:
				import guild
				skillGrade = 0
				skillLevel = guild.GetSkillLevel(sPos)
			else:
				skillGrade = player.GetSkillGrade(sPos)
				skillLevel = player.GetSkillLevel(sPos)

			self.otoAvSlot.SetSkillSlotNew(slotindex, skillIndex, skillGrade, skillLevel)
			self.otoAvSlot.SetSlotCountNew(slotindex, skillGrade, skillLevel)
			self.otoAvSlot.SetCoverButton(slotindex)

			if player.IsSkillCoolTime(sPos):
				(coolTime, elapsedTime) = player.GetSkillCoolTime(sPos)
				self.otoAvSlot.SetSlotCoolTime(slotindex, coolTime, elapsedTime)

			if player.IsSkillActive(sPos):self.otoAvSlot.ActivateSlot(slotindex)
			self.otoAvSlotIndex[slotindex] = sPos

			coolTime = player.OtoAvSlotDolumSuresiAl(slotindex)
			if self.timeeditlist[slotindex].GetText() == "":
				self.timeeditlist[slotindex].SetText(str(coolTime))

		self.otoAvSlot.RefreshSlot()

	def AddAutoSlot(self, slotindex):
		AttachedSlotType = mouseModule.mouseController.GetAttachedType()
		AttachedSlotNumber = mouseModule.mouseController.GetAttachedSlotNumber()
		AttachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()

		if slotindex <= player.OTOMATIK_AV_SKILL_MAX_NUM:
			if player.SLOT_TYPE_SKILL == AttachedSlotType:
				player.OtoAvSlotaBeceriEkle(slotindex,AttachedSlotNumber)
				self.BeceriSlotlariYenile()
			elif player.SLOT_TYPE_OTOMATIK_AV == AttachedSlotType:
				if slotindex == AttachedSlotNumber:return
				if AttachedSlotNumber >= player.OTOMATIK_AV_SKILL_MAX_NUM:return
				player.OtoAvSlotaBeceriEkle(slotindex,AttachedItemIndex)
				self.BeceriSlotlariYenile()
		else:
			if player.SLOT_TYPE_INVENTORY == AttachedSlotType:
				itemIndex = player.GetItemIndex(AttachedSlotNumber)
				item.SelectItem(itemIndex)
				ItemType		= item.GetItemType()
				ItemSubType	= item.GetItemSubType()
				itemRemaintime = 0

				if not ItemType == item.ITEM_TYPE_USE:return;
				if ItemSubType == item.USE_ABILITY_UP:itemRemaintime = item.GetValue(1)
				elif ItemSubType == item.USE_AFFECT:itemRemaintime = item.GetValue(3)

				if ItemSubType == item.USE_POTION \
				or ItemSubType == item.USE_ABILITY_UP \
				or ItemSubType == item.USE_POTION_NODELAY \
				or ItemSubType == item.USE_AFFECT:
					if itemRemaintime < 9999:
						player.OtoAvSlotaPotEkle(slotindex,AttachedSlotNumber)
						self.IksirSlotlariYenile()

			elif player.SLOT_TYPE_OTOMATIK_AV == AttachedSlotType:
				if slotindex == AttachedSlotNumber:return
				if AttachedSlotNumber <= player.OTOMATIK_AV_SKILL_MAX_NUM:return
				player.OtoAvSlotaPotEkle(slotindex,AttachedItemIndex)
				self.IksirSlotlariYenile()
				
		mouseModule.mouseController.DeattachObject()

	def SelectActiveSkillEmptySlot(self, slotindex):
		if self.otoAvAcKapa:return
		if True == mouseModule.mouseController.isAttached():self.AddAutoSlot(slotindex)

	def SelectActiveSkillSlot(self,slotindex):
		mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_OTOMATIK_AV, slotindex, self.otoAvSlotIndex[slotindex])

	def OverActiveSkillSlot(self,slotindex):
		if mouseModule.mouseController.isAttached():return
		if slotindex <= player.OTOMATIK_AV_SKILL_MAX_NUM:
			sPos = player.OtoAvSlotAl(slotindex)
			if sPos == 0:return
			skillIndex = player.GetSkillIndex(sPos)
			skillType = skill.GetSkillType(skillIndex)
			if skill.SKILL_TYPE_GUILD == skillType:
				import guild
				skillGrade = 0
				skillLevel = guild.GetSkillLevel(sPos)
			else:
				skillGrade = player.GetSkillGrade(sPos)
				skillLevel = player.GetSkillLevel(sPos)
			self.tooltipSkill.SetSkillNew(sPos, skillIndex, skillGrade, skillLevel)
		else:
			sPos_ = player.OtoAvSlotAl(slotindex)
			self.tooltipItem.SetInventoryItem(sPos_)
			self.tooltipSkill.HideToolTip()

	def OverSkillSlotOutItem(self):
		if 0 != self.tooltipSkill:self.tooltipSkill.HideToolTip()
		if 0 != self.tooltipItem:self.tooltipItem.HideToolTip()

	def OnPressEscapeKey(self):
		self.Close()
		return True


