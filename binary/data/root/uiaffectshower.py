import ui
import localeInfo
import chr
import item
import app
import skill
import player
import uiToolTip
import math
import chat

if app.ENABLE_NEW_AFFECT_POTION:
	AFFECT_POTION = {
		"affect"  : {
			0 : 310,
			1 : 311,
			2 : 312,
			3 : 313,
			4 : 314,
			5 : 315,
			6 : 336,
			7 : 337,
			8 : 338,
			9 : 339,
			10 : 340,
			11 : 341,
			12 : 342,
			13 : 543,
			14 : 220,
			15 : 355,
			16 : 356,
			17 : 351,
			18 : 352,
			19 : 353,
			20 : 354,
			21 : 357,
			22 : 358,
			23 : 359,
			24 : 704,
		},
		"image"  : {
			0 : "affect/50821.tga",
			1 : "affect/50822.tga",
			2 : "affect/50823.tga",
			3 : "affect/50824.tga",
			4 : "affect/50825.tga",
			5 : "affect/50826.tga",
			6 : "affect/71044.tga",
			7 : "affect/71045.tga",
			8 : "affect/71027.tga",
			9 : "affect/71028.tga",
			10 : "affect/71029.tga",
			11 : "affect/71029.tga",
			12 : "affect/enerji.png",
			13 : "icon/item/exp.png",
			14 : "icon/item/donusum.tga",
			15 : "affect/GreenPotion.tga",
			16 : "affect/PurplePotion.tga",
			17 : "affect/kasmir.tga",
			18 : "affect/50827.tga",
			19 : "affect/50828.tga",
			20 : "affect/50829.tga",
			21 : "affect/51350.tga",
			22 : "affect/51351.tga",
			23 : "affect/51352.tga",
			24 : "affect/shoesofwind.tga",
		},
	}
# WEDDING
class LovePointImage(ui.ExpandedImageBox):

	FILE_PATH = "d:/ymir work/ui/pattern/LovePoint/"
	FILE_DICT = {
		0 : FILE_PATH + "01.dds",
		1 : FILE_PATH + "02.dds",
		2 : FILE_PATH + "02.dds",
		3 : FILE_PATH + "03.dds",
		4 : FILE_PATH + "04.dds",
		5 : FILE_PATH + "05.dds",
	}

	def __init__(self):
		ui.ExpandedImageBox.__init__(self)

		self.loverName = ""
		self.lovePoint = 0

		self.toolTip = uiToolTip.ToolTip(100)
		self.toolTip.HideToolTip()

	def __del__(self):
		ui.ExpandedImageBox.__del__(self)

	def SetLoverInfo(self, name, lovePoint):
		self.loverName = name
		self.lovePoint = lovePoint
		self.__Refresh()

	def OnUpdateLovePoint(self, lovePoint):
		self.lovePoint = lovePoint
		self.__Refresh()

	def __Refresh(self):
		self.lovePoint = max(0, self.lovePoint)
		self.lovePoint = min(100, self.lovePoint)

		if 0 == self.lovePoint:
			loveGrade = 0
		else:
			loveGrade = self.lovePoint / 25 + 1
		fileName = self.FILE_DICT.get(loveGrade, self.FILE_PATH+"00.dds")

		try:
			self.LoadImage(fileName)
		except:
			import dbg
			dbg.TraceError("LovePointImage.SetLoverInfo(lovePoint=%d) - LoadError %s" % (self.lovePoint, fileName))

		self.SetScale(0.7, 0.7)

		self.toolTip.ClearToolTip()
		self.toolTip.SetTitle(self.loverName)
		self.toolTip.AppendTextLine(localeInfo.AFF_LOVE_POINT % (self.lovePoint))
		self.toolTip.ResizeToolTip()

	def OnMouseOverIn(self):
		self.toolTip.ShowToolTip()

	def OnMouseOverOut(self):
		self.toolTip.HideToolTip()
# END_OF_WEDDING


class HorseImage(ui.ExpandedImageBox):

	FILE_PATH = "d:/ymir work/ui/pattern/HorseState/"

	FILE_DICT = {
		00 : FILE_PATH+"00.dds",
		01 : FILE_PATH+"00.dds",
		02 : FILE_PATH+"00.dds",
		03 : FILE_PATH+"00.dds",
		10 : FILE_PATH+"10.dds",
		11 : FILE_PATH+"11.dds",
		12 : FILE_PATH+"12.dds",
		13 : FILE_PATH+"13.dds",
		20 : FILE_PATH+"20.dds",
		21 : FILE_PATH+"21.dds",
		22 : FILE_PATH+"22.dds",
		23 : FILE_PATH+"23.dds",
		30 : FILE_PATH+"30.dds",
		31 : FILE_PATH+"31.dds",
		32 : FILE_PATH+"32.dds",
		33 : FILE_PATH+"33.dds",
	}

	def __init__(self):
		ui.ExpandedImageBox.__init__(self)

		#self.textLineList = []
		self.toolTip = uiToolTip.ToolTip(100)
		self.toolTip.HideToolTip()

	def __GetHorseGrade(self, level):
		if 0 == level:
			return 0

		return (level-1)/10 + 1

	def SetState(self, level, health, battery):
		#self.textLineList=[]
		self.toolTip.ClearToolTip()

		if level>0:

			try:
				grade = self.__GetHorseGrade(level)
				self.__AppendText(localeInfo.LEVEL_LIST[grade])
			except IndexError:
				print "HorseImage.SetState(level=%d, health=%d, battery=%d) - Unknown Index" % (level, health, battery)
				return

			try:
				healthName=localeInfo.HEALTH_LIST[health]
				if len(healthName)>0:
					self.__AppendText(healthName)
			except IndexError:
				print "HorseImage.SetState(level=%d, health=%d, battery=%d) - Unknown Index" % (level, health, battery)
				return

			if health>0:
				if battery==0:
					self.__AppendText(localeInfo.NEEFD_REST)

			try:
				fileName=self.FILE_DICT[health*10+battery]
			except KeyError:
				print "HorseImage.SetState(level=%d, health=%d, battery=%d) - KeyError" % (level, health, battery)

			try:
				self.LoadImage(fileName)
			except:
				print "HorseImage.SetState(level=%d, health=%d, battery=%d) - LoadError %s" % (level, health, battery, fileName)

		self.SetScale(0.7, 0.7)

	def __AppendText(self, text):

		self.toolTip.AppendTextLine(text)
		self.toolTip.ResizeToolTip()

		#x=self.GetWidth()/2
		#textLine = ui.TextLine()
		#textLine.SetParent(self)
		#textLine.SetSize(0, 0)
		#textLine.SetOutline()
		#textLine.Hide()
		#textLine.SetPosition(x, 40+len(self.textLineList)*16)
		#textLine.SetText(text)
		#self.textLineList.append(textLine)

	def OnMouseOverIn(self):
		#for textLine in self.textLineList:
		#	textLine.Show()

		self.toolTip.ShowToolTip()

	def OnMouseOverOut(self):
		#for textLine in self.textLineList:
		#	textLine.Hide()

		self.toolTip.HideToolTip()


# AUTO_POTION
class AutoPotionImage(ui.ExpandedImageBox):

	FILE_PATH_HP = "d:/ymir work/ui/pattern/auto_hpgauge/"
	FILE_PATH_SP = "d:/ymir work/ui/pattern/auto_spgauge/"

	def __init__(self):
		ui.ExpandedImageBox.__init__(self)

		self.loverName = ""
		self.lovePoint = 0
		self.potionType = player.AUTO_POTION_TYPE_HP
		self.filePath = ""

		self.toolTip = uiToolTip.ToolTip(100)
		self.toolTip.HideToolTip()

	def __del__(self):
		ui.ExpandedImageBox.__del__(self)

	def SetPotionType(self, type):
		self.potionType = type
		
		if player.AUTO_POTION_TYPE_HP == type:
			self.filePath = self.FILE_PATH_HP
		elif player.AUTO_POTION_TYPE_SP == type:
			self.filePath = self.FILE_PATH_SP
			

	def OnUpdateAutoPotionImage(self):
		self.__Refresh()

	def __Refresh(self):
		print "__Refresh"
	
		isActivated, currentAmount, totalAmount, slotIndex = player.GetAutoPotionInfo(self.potionType)
		
		amountPercent = (float(currentAmount) / totalAmount) * 100.0
		grade = math.ceil(amountPercent / 20)
		
		if 5.0 > amountPercent:
			grade = 0
			
		if 80.0 < amountPercent:
			grade = 4
			if 90.0 < amountPercent:
				grade = 5			

		fmt = self.filePath + "%.2d.dds"
		fileName = fmt % grade
		
		print self.potionType, amountPercent, fileName

		try:
			self.LoadImage(fileName)
		except:
			import dbg
			dbg.TraceError("AutoPotionImage.__Refresh(potionType=%d) - LoadError %s" % (self.potionType, fileName))

		self.SetScale(0.7, 0.7)

		self.toolTip.ClearToolTip()
		
		if player.AUTO_POTION_TYPE_HP == type:
			self.toolTip.SetTitle(localeInfo.TOOLTIP_AUTO_POTION_HP)
		else:
			self.toolTip.SetTitle(localeInfo.TOOLTIP_AUTO_POTION_SP)
			
		self.toolTip.AppendTextLine(localeInfo.TOOLTIP_AUTO_POTION_REST	% (amountPercent))
		self.toolTip.ResizeToolTip()

	def OnMouseOverIn(self):
		self.toolTip.ShowToolTip()

	def OnMouseOverOut(self):
		self.toolTip.HideToolTip()
# END_OF_AUTO_POTION


class AffectImage(ui.ExpandedImageBox):

	def __init__(self):
		ui.ExpandedImageBox.__init__(self)

		self.toolTipText = None
		self.isSkillAffect = TRUE
		self.description = None
		self.endTime = 0
		self.affect = None
		self.isClocked = TRUE
		if app.ENABLE_AFFECT_BUFF_REMOVE:
			self.buffQuestionDialog = None
			self.skillIndex = None
			
		if (app.ENABLE_AFFECT_POLYMORPH_REMOVE):
			self.polymorphQuestionDialog = None

	def SetAffect(self, affect):
		self.affect = affect

	def GetAffect(self):
		return self.affect

	def SetToolTipText(self, text, x = 0, y = -19):

		if not self.toolTipText:
			textLine = ui.TextLine()
			textLine.SetParent(self)
			textLine.SetSize(0, 0)
			textLine.SetOutline()
			textLine.Hide()
			self.toolTipText = textLine
			
		self.toolTipText.SetText(text)
		w, h = self.toolTipText.GetTextSize()
		self.toolTipText.SetPosition(max(0, x + self.GetWidth()/2 - w/2), y)

	def SetDescription(self, description):
		self.description = description

	def SetDuration(self, duration):
		self.endTime = 0
		if duration > 0:
			self.endTime = app.GetGlobalTimeStamp() + duration

	def UpdateAutoPotionDescription(self):		
		
		potionType = 0
		if self.affect == chr.NEW_AFFECT_AUTO_HP_RECOVERY:
			potionType = player.AUTO_POTION_TYPE_HP
		else:
			potionType = player.AUTO_POTION_TYPE_SP	
		
		isActivated, currentAmount, totalAmount, slotIndex = player.GetAutoPotionInfo(potionType)
		
		#print "UpdateAutoPotionDescription ", isActivated, currentAmount, totalAmount, slotIndex
		
		amountPercent = 0.0
		
		try:
			amountPercent = (float(currentAmount) / totalAmount) * 100.0		
		except:
			amountPercent = 100.0
		
		self.SetToolTipText(self.description % amountPercent, 0, 40)
		
	def SetClock(self, isClocked):
		self.isClocked = isClocked
		
	def UpdateDescription(self):
		if not self.isClocked:
			self.__UpdateDescription2()
			return
	
		if not self.description:
			return
			
		toolTip = self.description
		if self.endTime > 0:
			leftTime = localeInfo.SecondToDHM(self.endTime - app.GetGlobalTimeStamp())
			toolTip += " (%s : %s)" % (localeInfo.LEFT_TIME, leftTime)
		self.SetToolTipText(toolTip, 0, 40)
		
	#?????? ??? ???? ??? ?? 
	def __UpdateDescription2(self):
		if not self.description:
			return

		toolTip = self.description
		self.SetToolTipText(toolTip, 0, 40)

	def SetSkillAffectFlag(self, flag):
		self.isSkillAffect = flag

	def IsSkillAffect(self):
		return self.isSkillAffect

	if app.ENABLE_AFFECT_BUFF_REMOVE:
		def SetSkillIndex(self, skillIndex):
			self.skillIndex = skillIndex
			


	if (app.ENABLE_AFFECT_POLYMORPH_REMOVE):
		def OnPolymorphQuestionDialog(self):
			import uiCommon
			self.polymorphQuestionDialog = uiCommon.QuestionDialog()
			self.polymorphQuestionDialog.SetText(localeInfo.POLYMORPH_AFFECT_REMOVE_QUESTION)
			self.polymorphQuestionDialog.SetWidth(350)
			self.polymorphQuestionDialog.SetAcceptEvent(lambda arg = TRUE: self.OnClosePolymorphQuestionDialog(arg))
			self.polymorphQuestionDialog.SetCancelEvent(lambda arg = FALSE: self.OnClosePolymorphQuestionDialog(arg))
			self.polymorphQuestionDialog.Open()
			
	if app.ENABLE_AFFECT_BUFF_REMOVE:
		def OnBuffQuestionDialog(self, skillIndex):
			import uiCommon
			self.buffQuestionDialog = uiCommon.QuestionDialog()
			self.buffQuestionDialog.SetWidth(350)
			self.buffQuestionDialog.SetText(localeInfo.BUFF_AFFECT_REMOVE_QUESTION % (skill.GetSkillName(skillIndex)))
			self.buffQuestionDialog.SetAcceptEvent(lambda arg = skillIndex: self.OnCloseBuffQuestionDialog(arg))
			self.buffQuestionDialog.SetCancelEvent(lambda arg = 0: self.OnCloseBuffQuestionDialog(arg))
			self.buffQuestionDialog.Open()
			
		def OnCloseBuffQuestionDialog(self, answer):
			import net
			if not self.buffQuestionDialog:
				return

			self.buffQuestionDialog.Close()
			self.buffQuestionDialog = None

			if not answer:
				return

			net.SendChatPacket("/remove_buff %d" % answer)
			return TRUE
			
			
		def OnClosePolymorphQuestionDialog(self, answer):
			import net

			if not self.polymorphQuestionDialog:
				return

			self.polymorphQuestionDialog.Close()
			self.polymorphQuestionDialog = None
					
			if not answer:
				return

			net.SendChatPacket("/remove_polymorph")
			return TRUE	

	def OnMouseLeftButtonUp(self):
		if self.toolTipText:
			self.toolTipText.Show()
			
		if app.ENABLE_AFFECT_BUFF_REMOVE:
			if self.skillIndex:
				self.OnBuffQuestionDialog(self.skillIndex)
	def OnMouseOverIn(self):
		if self.toolTipText:
			self.toolTipText.Show()
			
		# if app.ENABLE_AFFECT_BUFF_REMOVE:
			# if self.skillIndex:
				# self.OnBuffQuestionDialog(self.skillIndex)
				
			
		if (app.ENABLE_AFFECT_POLYMORPH_REMOVE):	
			if self.affect == chr.NEW_AFFECT_POLYMORPH:
				self.OnPolymorphQuestionDialog()	

	def OnMouseOverOut(self):
		if self.toolTipText:
			self.toolTipText.Hide()

class AffectShower(ui.Window):

	MALL_DESC_IDX_START = 1000
	IMAGE_STEP = 25
	AFFECT_MAX_NUM = 32

	INFINITE_AFFECT_DURATION = 0x1FFFFFFF 

	AFFECT_NEW_AFFECT ={
		310 : ("Kýrmýzý Þebnem"),
		311 : ("Pembe Þebnem"),
		312 : ("Sarý Þebnem"),
		313 : ("Yeþil Þebnem"),
		314 : ("Mavi Þebnem"),
		315 : ("Beyaz Þebnem"),
		336 : ("Kritik Ýsabet"),
		337 : ("Delici Ýsabet"),
		338 : ("Ejderha Tanrý Yaþam"),
		339 : ("Ejderha Tanrý Saldýrý"),
		340 : ("Ejderha Tanrý Zeka"),
		341 : ("Ejderha Tanrý Savunma"),
		342 : ("Enerji"),
		543 : ("Anti EXP"),
		220 : ("Dönüþüm"),
		735 : ("Kostüm Set"),
		355 : ("Yeþil Ýksir"),
		356 : ("Mor Ýksir"),
		351 : ("Kaþmir Paket"),
		352 : ("Mor Þebnem "),
		353 : ("Siyah Þebnem "),
		354 : ("Turkuaz Þebnem"),
		347 : ("VIP "),
		357 : ("Buff Özütü(Canavar) "),
		358 : ("Buff Özütü(Metin) "),
		359 : ("Buff Özütü(Boss) "),
		704 : ("Rüzgarýn Ayakkabýsý"),
	}

	AFFECT_DATA_DICT =	{
			chr.AFFECT_POISON : (localeInfo.SKILL_TOXICDIE, "d:/ymir work/ui/skill/common/affect/poison.sub"),
			chr.AFFECT_SLOW : (localeInfo.SKILL_SLOW, "d:/ymir work/ui/skill/common/affect/slow.sub"),
			chr.AFFECT_STUN : (localeInfo.SKILL_STUN, "d:/ymir work/ui/skill/common/affect/stun.sub"),

			chr.AFFECT_ATT_SPEED_POTION : (localeInfo.SKILL_INC_ATKSPD, "d:/ymir work/ui/skill/common/affect/Increase_Attack_Speed.sub"),
			chr.AFFECT_MOV_SPEED_POTION : (localeInfo.SKILL_INC_MOVSPD, "d:/ymir work/ui/skill/common/affect/Increase_Move_Speed.sub"),
			chr.AFFECT_FISH_MIND : (localeInfo.SKILL_FISHMIND, "d:/ymir work/ui/skill/common/affect/fishmind.sub"),

			# chr.AFFECT_JEONGWI : (localeInfo.SKILL_JEONGWI, "d:/ymir work/ui/skill/warrior/jeongwi_03.sub",),
			# chr.AFFECT_GEOMGYEONG : (localeInfo.SKILL_GEOMGYEONG, "d:/ymir work/ui/skill/warrior/geomgyeong_03.sub",),
			# chr.AFFECT_CHEONGEUN : (localeInfo.SKILL_CHEONGEUN, "d:/ymir work/ui/skill/warrior/cheongeun_03.sub",),
			# chr.AFFECT_GYEONGGONG : (localeInfo.SKILL_GYEONGGONG, "d:/ymir work/ui/skill/assassin/gyeonggong_03.sub",),
			# chr.AFFECT_EUNHYEONG : (localeInfo.SKILL_EUNHYEONG, "d:/ymir work/ui/skill/assassin/eunhyeong_03.sub",),
			# chr.AFFECT_GWIGEOM : (localeInfo.SKILL_GWIGEOM, "d:/ymir work/ui/skill/sura/gwigeom_03.sub",),
			# chr.AFFECT_GONGPO : (localeInfo.SKILL_GONGPO, "d:/ymir work/ui/skill/sura/gongpo_03.sub",),
			# chr.AFFECT_JUMAGAP : (localeInfo.SKILL_JUMAGAP, "d:/ymir work/ui/skill/sura/jumagap_03.sub"),
			# chr.AFFECT_HOSIN : (localeInfo.SKILL_HOSIN, "d:/ymir work/ui/skill/shaman/hosin_03.sub",),
			# chr.AFFECT_BOHO : (localeInfo.SKILL_BOHO, "d:/ymir work/ui/skill/shaman/boho_03.sub",),
			# chr.AFFECT_KWAESOK : (localeInfo.SKILL_KWAESOK, "d:/ymir work/ui/skill/shaman/kwaesok_03.sub",),
			# chr.AFFECT_HEUKSIN : (localeInfo.SKILL_HEUKSIN, "d:/ymir work/ui/skill/sura/heuksin_03.sub",),
			# chr.AFFECT_MUYEONG : (localeInfo.SKILL_MUYEONG, "d:/ymir work/ui/skill/sura/muyeong_03.sub",),
			# chr.AFFECT_GICHEON : (localeInfo.SKILL_GICHEON, "d:/ymir work/ui/skill/shaman/gicheon_03.sub",),
			# chr.AFFECT_JEUNGRYEOK : (localeInfo.SKILL_JEUNGRYEOK, "d:/ymir work/ui/skill/shaman/jeungryeok_03.sub",),
			# chr.AFFECT_PABEOP : (localeInfo.SKILL_PABEOP, "d:/ymir work/ui/skill/sura/pabeop_03.sub",),
			# chr.AFFECT_FALLEN_CHEONGEUN : (localeInfo.SKILL_CHEONGEUN, "d:/ymir work/ui/skill/warrior/cheongeun_03.sub",),
			# 28 : (localeInfo.SKILL_FIRE, "d:/ymir work/ui/skill/sura/hwayeom_03.sub",),
			
			chr.AFFECT_JEONGWI : (localeInfo.SKILL_JEONGWI, "d:/ymir work/ui/skill/warrior/jeongwi_03.sub",),
			chr.AFFECT_GEOMGYEONG : (localeInfo.SKILL_GEOMGYEONG, "d:/ymir work/ui/skill/warrior/geomgyeong_03.sub",),
			chr.AFFECT_CHEONGEUN : (localeInfo.SKILL_CHEONGEUN, "d:/ymir work/ui/skill/warrior/cheongeun_03.sub",),
			chr.AFFECT_GYEONGGONG : (localeInfo.SKILL_GYEONGGONG, "d:/ymir work/ui/skill/assassin/gyeonggong_03.sub",),
			chr.AFFECT_EUNHYEONG : (localeInfo.SKILL_EUNHYEONG, "d:/ymir work/ui/skill/assassin/eunhyeong_03.sub",),
			chr.AFFECT_GWIGEOM : (localeInfo.SKILL_GWIGEOM, "d:/ymir work/ui/skill/sura/gwigeom_03.sub",),
			chr.AFFECT_GONGPO : (localeInfo.SKILL_GONGPO, "d:/ymir work/ui/skill/sura/gongpo_03.sub",),
			chr.AFFECT_JUMAGAP : (localeInfo.SKILL_JUMAGAP, "d:/ymir work/ui/skill/sura/jumagap_03.sub"),
			chr.AFFECT_HOSIN : (localeInfo.SKILL_HOSIN, "d:/ymir work/ui/skill/shaman/hosin_03.sub",),
			chr.AFFECT_BOHO : (localeInfo.SKILL_BOHO, "d:/ymir work/ui/skill/shaman/boho_03.sub",),
			chr.AFFECT_KWAESOK : (localeInfo.SKILL_KWAESOK, "d:/ymir work/ui/skill/shaman/kwaesok_03.sub",),
			chr.AFFECT_HEUKSIN : (localeInfo.SKILL_HEUKSIN, "d:/ymir work/ui/skill/sura/heuksin_03.sub",),
			chr.AFFECT_MUYEONG : (localeInfo.SKILL_MUYEONG, "d:/ymir work/ui/skill/sura/muyeong_03.sub",),
			chr.AFFECT_GICHEON : (localeInfo.SKILL_GICHEON, "d:/ymir work/ui/skill/shaman/gicheon_03.sub",),
			chr.AFFECT_JEUNGRYEOK : (localeInfo.SKILL_JEUNGRYEOK, "d:/ymir work/ui/skill/shaman/jeungryeok_03.sub",),
			chr.AFFECT_PABEOP : (localeInfo.SKILL_PABEOP, "d:/ymir work/ui/skill/sura/pabeop_03.sub",),
			chr.AFFECT_FALLEN_CHEONGEUN : (localeInfo.SKILL_CHEONGEUN, "d:/ymir work/ui/skill/warrior/cheongeun_03.sub",),
			28 : (localeInfo.SKILL_FIRE, "d:/ymir work/ui/skill/sura/hwayeom_03.sub",),

			chr.AFFECT_CHINA_FIREWORK : (localeInfo.SKILL_POWERFUL_STRIKE, "d:/ymir work/ui/skill/common/affect/powerfulstrike.sub",),
			chr.AFFECT_JEOKRANG : (localeInfo.SKILL_JEOKRANG, "d:/ymir work/ui/skill/wolfman/red_possession_03.sub",),
			chr.AFFECT_CHEONGRANG : (localeInfo.SKILL_CHEONGRANG, "d:/ymir work/ui/skill/wolfman/blue_possession_03.sub",),

			42 : (localeInfo.TOOLTIP_AFFECT_SHOP_DECO, "d:/ymir work/ui/skill/common/affect/gold_premium.sub"),
			#64 - END
			chr.NEW_AFFECT_EXP_BONUS : (localeInfo.TOOLTIP_MALL_EXPBONUS_STATIC, "d:/ymir work/ui/skill/common/affect/exp_bonus.sub",),

			chr.NEW_AFFECT_ITEM_BONUS : (localeInfo.TOOLTIP_MALL_ITEMBONUS_STATIC, "d:/ymir work/ui/skill/common/affect/item_bonus.sub",),
			chr.NEW_AFFECT_SAFEBOX : (localeInfo.TOOLTIP_MALL_SAFEBOX, "d:/ymir work/ui/skill/common/affect/safebox.sub",),
			chr.NEW_AFFECT_AUTOLOOT : (localeInfo.TOOLTIP_MALL_AUTOLOOT, "d:/ymir work/ui/skill/common/affect/autoloot.sub",),
			chr.NEW_AFFECT_FISH_MIND : (localeInfo.TOOLTIP_MALL_FISH_MIND, "d:/ymir work/ui/skill/common/affect/fishmind.sub",),
			chr.NEW_AFFECT_MARRIAGE_FAST : (localeInfo.TOOLTIP_MALL_MARRIAGE_FAST, "d:/ymir work/ui/skill/common/affect/marriage_fast.sub",),
			chr.NEW_AFFECT_GOLD_BONUS : (localeInfo.TOOLTIP_MALL_GOLDBONUS_STATIC, "d:/ymir work/ui/skill/common/affect/gold_bonus.sub",),

			chr.NEW_AFFECT_NO_DEATH_PENALTY : (localeInfo.TOOLTIP_APPLY_NO_DEATH_PENALTY, "d:/ymir work/ui/skill/common/affect/gold_premium.sub"),
			chr.NEW_AFFECT_SKILL_BOOK_BONUS : (localeInfo.TOOLTIP_APPLY_SKILL_BOOK_BONUS, "d:/ymir work/ui/skill/common/affect/gold_premium.sub"),
			chr.NEW_AFFECT_SKILL_BOOK_NO_DELAY : (localeInfo.TOOLTIP_APPLY_SKILL_BOOK_NO_DELAY, "d:/ymir work/ui/skill/common/affect/gold_premium.sub"),
			chr.NEW_AFFECT_SHOP_DECO : (localeInfo.TOOLTIP_AFFECT_SHOP_DECO, "d:/ymir work/ui/skill/common/affect/gold_premium.sub"),
			# ???? hp, sp
			chr.NEW_AFFECT_AUTO_HP_RECOVERY : (localeInfo.TOOLTIP_AUTO_POTION_REST, "d:/ymir work/ui/pattern/auto_hpgauge/05.dds"),			
			chr.NEW_AFFECT_AUTO_SP_RECOVERY : (localeInfo.TOOLTIP_AUTO_POTION_REST, "d:/ymir work/ui/pattern/auto_spgauge/05.dds"),
			#chr.NEW_AFFECT_AUTO_HP_RECOVERY : (localeInfo.TOOLTIP_AUTO_POTION_REST, "d:/ymir work/ui/skill/common/affect/gold_premium.sub"),			
			#chr.NEW_AFFECT_AUTO_SP_RECOVERY : (localeInfo.TOOLTIP_AUTO_POTION_REST, "d:/ymir work/ui/skill/common/affect/gold_bonus.sub"),			

			MALL_DESC_IDX_START+player.POINT_MALL_ATTBONUS : (localeInfo.TOOLTIP_MALL_ATTBONUS_STATIC, "d:/ymir work/ui/skill/common/affect/att_bonus.sub",),
			MALL_DESC_IDX_START+player.POINT_MALL_DEFBONUS : (localeInfo.TOOLTIP_MALL_DEFBONUS_STATIC, "d:/ymir work/ui/skill/common/affect/def_bonus.sub",),
			MALL_DESC_IDX_START+player.POINT_MALL_EXPBONUS : (localeInfo.TOOLTIP_MALL_EXPBONUS, "d:/ymir work/ui/skill/common/affect/exp_bonus.sub",),
			MALL_DESC_IDX_START+player.POINT_MALL_ITEMBONUS : (localeInfo.TOOLTIP_MALL_ITEMBONUS, "d:/ymir work/ui/skill/common/affect/item_bonus.sub",),
			MALL_DESC_IDX_START+player.POINT_MALL_GOLDBONUS : (localeInfo.TOOLTIP_MALL_GOLDBONUS, "d:/ymir work/ui/skill/common/affect/gold_bonus.sub",),
			MALL_DESC_IDX_START+player.POINT_CRITICAL_PCT : (localeInfo.TOOLTIP_APPLY_CRITICAL_PCT,"d:/ymir work/ui/skill/common/affect/critical.sub"),
			MALL_DESC_IDX_START+player.POINT_PENETRATE_PCT : (localeInfo.TOOLTIP_APPLY_PENETRATE_PCT, "d:/ymir work/ui/skill/common/affect/gold_premium.sub"),
			MALL_DESC_IDX_START+player.POINT_MAX_HP_PCT : (localeInfo.TOOLTIP_MAX_HP_PCT, "d:/ymir work/ui/skill/common/affect/gold_premium.sub"),
			MALL_DESC_IDX_START+player.POINT_MAX_SP_PCT : (localeInfo.TOOLTIP_MAX_SP_PCT, "d:/ymir work/ui/skill/common/affect/gold_premium.sub"),	

			MALL_DESC_IDX_START+player.POINT_PC_BANG_EXP_BONUS : (localeInfo.TOOLTIP_MALL_EXPBONUS_P_STATIC, "d:/ymir work/ui/skill/common/affect/EXP_Bonus_p_on.sub",),
			MALL_DESC_IDX_START+player.POINT_PC_BANG_DROP_BONUS: (localeInfo.TOOLTIP_MALL_ITEMBONUS_P_STATIC, "d:/ymir work/ui/skill/common/affect/Item_Bonus_p_on.sub",),
	}
	if app.ENABLE_DRAGON_SOUL_SYSTEM:
		# ??? ?, ? ?.
		AFFECT_DATA_DICT[chr.NEW_AFFECT_DRAGON_SOUL_DECK1] = (localeInfo.TOOLTIP_DRAGON_SOUL_DECK1, "d:/ymir work/ui/dragonsoul/buff_ds_sky1.tga")
		AFFECT_DATA_DICT[chr.NEW_AFFECT_DRAGON_SOUL_DECK2] = (localeInfo.TOOLTIP_DRAGON_SOUL_DECK2, "d:/ymir work/ui/dragonsoul/buff_ds_land1.tga")

	if app.ENABLE_DS_SET:
		AFFECT_DATA_DICT[chr.NEW_AFFECT_DS_SET] = (localeInfo.TOOLTIP_DS_SET, "d:/ymir work/ui/dragonsoul/buff_ds_completeset.tga")

	if (app.ENABLE_AFFECT_POLYMORPH_REMOVE):
		AFFECT_DATA_DICT[chr.NEW_AFFECT_POLYMORPH] = (localeInfo.POLYMORPH_AFFECT_TOOLTIP, "d:/ymir work/ui/polymorph_marble_icon.tga")

	if app.ENABLE_NEW_AFFECT_POTION: 
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][0]] = (localeInfo.TOOLTIP_AFFECT_POTION_1, AFFECT_POTION["image"][0])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][1]] = (localeInfo.TOOLTIP_AFFECT_POTION_2, AFFECT_POTION["image"][1])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][2]] = (localeInfo.TOOLTIP_AFFECT_POTION_3, AFFECT_POTION["image"][2])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][3]] = (localeInfo.TOOLTIP_AFFECT_POTION_4, AFFECT_POTION["image"][3])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][4]] = (localeInfo.TOOLTIP_AFFECT_POTION_5, AFFECT_POTION["image"][4])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][5]] = (localeInfo.TOOLTIP_AFFECT_POTION_6, AFFECT_POTION["image"][5])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][6]] = ("Delici Ýsabet", AFFECT_POTION["image"][6])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][7]] = ("Kritik Ýsabet", AFFECT_POTION["image"][7])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][8]] = ("Ejderha Tanrý Yaþam", AFFECT_POTION["image"][8])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][9]] = ("Ejderha Tanrý Saldýrý", AFFECT_POTION["image"][9])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][10]] = ("Ejderha Tanrý Zeka", AFFECT_POTION["image"][10])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][11]] = ("Ejderha Tanrý Savunma", AFFECT_POTION["image"][11])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][12]] = ("Enerji", AFFECT_POTION["image"][12])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][13]] = ("Anti Exp", AFFECT_POTION["image"][13])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][14]] = ("Dönüþüm", AFFECT_POTION["image"][14])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][15]] = ("Yeþil Ýksir", AFFECT_POTION["image"][15])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][16]] = ("Mor Ýksir", AFFECT_POTION["image"][16])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][17]] = ("Kaþmir Paket", AFFECT_POTION["image"][17])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][18]] = (localeInfo.TOOLTIP_AFFECT_POTION_7, AFFECT_POTION["image"][18])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][19]] = (localeInfo.TOOLTIP_AFFECT_POTION_8, AFFECT_POTION["image"][19])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][20]] = (localeInfo.TOOLTIP_AFFECT_POTION_9, AFFECT_POTION["image"][20])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][21]] = (localeInfo.TOOLTIP_AFFECT_BUFF_EXTRACT, AFFECT_POTION["image"][21])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][22]] = (localeInfo.TOOLTIP_AFFECT_BUFF_EXTRACT_2, AFFECT_POTION["image"][22])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][23]] = (localeInfo.TOOLTIP_AFFECT_BUFF_EXTRACT_3, AFFECT_POTION["image"][23])
		AFFECT_DATA_DICT[AFFECT_POTION["affect"][24]] = (localeInfo.TOOLTIP_AFFECT_RUZGAR, AFFECT_POTION["image"][24])

	if app.ENABLE_NEW_PET_SYSTEM:
		AFFECT_PET_DATA_DICT ={
			5401 : (localeInfo.PET_AFFECT_RESIST_WARRIOR, "d:/ymir work/ui/skill/pet/jijoong.sub"),
			5402 : (localeInfo.PET_AFFECT_RESIST_SURA, "d:/ymir work/ui/skill/pet/jijoong.sub"),
			5403 : (localeInfo.PET_AFFECT_RESIST_NINJA, "d:/ymir work/ui/skill/pet/jijoong.sub"),
			5404 : (localeInfo.PET_AFFECT_RESIST_SHAMAN, "d:/ymir work/ui/skill/pet/jijoong.sub"),
			5405 : (localeInfo.PET_AFFECT_RESIST_LYCAN, "d:/ymir work/ui/skill/pet/jijoong.sub"),
			5406 : (localeInfo.PET_AFFECT_BERSERKER, "d:/ymir work/ui/skill/pet/pacheon.sub"),
			5407 : (localeInfo.PET_AFFECT_ANTI_MAGIC, "d:/ymir work/ui/skill/pet/cheonryeong.sub"),
			5408 : (localeInfo.PET_AFFECT_MAGIC_SPEED, "d:/ymir work/ui/skill/pet/banya.sub"),
			5409 : (localeInfo.PET_AFFECT_PENETRATE, "d:/ymir work/ui/skill/pet/choehoenbimu.sub"),
			5410 : (localeInfo.PET_AFFECT_HEAL, "d:/ymir work/ui/skill/pet/heal.sub"),
			5411 : (localeInfo.PET_AFFECT_VAMPIRISM, "d:/ymir work/ui/skill/pet/stealhp.sub"),
			5412 : (localeInfo.PET_AFFECT_SPRITISM, "d:/ymir work/ui/skill/pet/stealmp.sub"),
			5413 : (localeInfo.PET_AFFECT_BLOCK, "d:/ymir work/ui/skill/pet/block.sub"),
			5414 : (localeInfo.PET_AFFECT_REFLECT_MELEE, "d:/ymir work/ui/skill/pet/reflect_melee.sub"),
			5415 : (localeInfo.PET_AFFECT_DROP_GOLD, "d:/ymir work/ui/skill/pet/gold_drop.sub"),
			5416 : (localeInfo.PET_AFFECT_BOW_DISTANCE, "d:/ymir work/ui/skill/pet/bow_distance.sub"),
			5417 : (localeInfo.PET_AFFECT_IMMORTAL, "d:/ymir work/ui/skill/pet/invincibility.sub"),
			5418 : (localeInfo.PET_AFFECT_HEAL2, "d:/ymir work/ui/skill/pet/removal.sub"),
			5419 : (localeInfo.PET_AFFECT_STONE, "d:/ymir work/ui/skill/pet/mob_bonus.sub"),
			5420 : (localeInfo.PET_AFFECT_MOBS, "d:/ymir work/ui/skill/pet/mob_bonus.sub"),
			5421 : (localeInfo.PET_AFFECT_BOSS, "d:/ymir work/ui/skill/pet/mob_bonus.sub"),
			5422 : (localeInfo.PET_AFFECT_HPRE, "d:/ymir work/ui/skill/pet/hp_recover.sub"),
			5423 : (localeInfo.PET_AFFECT_FEAT, "d:/ymir work/ui/skill/pet/feather.sub"),
			5424 : (localeInfo.PET_AFFECT_POTI, "d:/ymir work/ui/skill/pet/potion.sub"),
			5425 : (localeInfo.PET_AFFECT_DEXP, "d:/ymir work/ui/skill/pet/exp.sub"),
		}

	if app.OTOMATIK_AV:
		AFFECT_DATA_DICT[chr.NEW_AFFECT_OTO_AV] = (localeInfo.TOOLTIP_OTOMATIK_AV, "d:/ymir work/ui/skill/common/affect/auto_premium.png")

	def __init__(self):
		ui.Window.__init__(self)

		self.serverPlayTime=0
		self.clientPlayTime=0
		
		self.lastUpdateTime=0
		self.affectImageDict={}
		self.horseImage=None
		self.lovePointImage=None
		self.autoPotionImageHP = AutoPotionImage()
		self.autoPotionImageSP = AutoPotionImage()
		self.SetPosition(10, 10)
		self.Show()

	def ClearAllAffects(self):
		self.horseImage=None
		self.lovePointImage=None
		self.affectImageDict={}
		self.__ArrangeImageList()

	def ClearAffects(self): ## ?? ???? ????.
		self.living_affectImageDict={}
		for key, image in self.affectImageDict.items():
			if not image.IsSkillAffect():
				self.living_affectImageDict[key] = image
		self.affectImageDict = self.living_affectImageDict
		self.__ArrangeImageList()

	def BINARY_NEW_AddAffect(self, type, pointIdx, value, duration):
		print "BINARY_NEW_AddAffect", type, pointIdx, value, duration

		if type == chr.NEW_AFFECT_MALL:
			affect = self.MALL_DESC_IDX_START + pointIdx
		else:
			affect = type

		if self.affectImageDict.has_key(affect):
			return
			
		if type < 500 and type != chr.NEW_AFFECT_POLYMORPH and not self.AFFECT_NEW_AFFECT.has_key(affect):
			return 

		if app.ENABLE_NEW_PET_SYSTEM:
			if not self.AFFECT_DATA_DICT.has_key(affect) and not self.AFFECT_PET_DATA_DICT.has_key(affect):
				return
		else:
			if not self.AFFECT_DATA_DICT.has_key(affect):
				return

		## ??? ??, ??? ??? Duration ? 0 ?? ????.
		if affect == chr.NEW_AFFECT_NO_DEATH_PENALTY or\
		   affect == chr.NEW_AFFECT_SKILL_BOOK_BONUS or\
		   affect == chr.NEW_AFFECT_AUTO_SP_RECOVERY or\
		   affect == chr.NEW_AFFECT_AUTO_HP_RECOVERY or\
		   affect == chr.NEW_AFFECT_SKILL_BOOK_NO_DELAY:
			duration = 0

		if affect >= 5400 and affect <= 5425 and app.ENABLE_NEW_PET_SYSTEM:
			affectData = self.AFFECT_PET_DATA_DICT[affect]
		else:
			affectData = self.AFFECT_DATA_DICT[affect]

		description = affectData[0]
		filename = affectData[1]

		if pointIdx == player.POINT_MALL_ITEMBONUS or\
		   pointIdx == player.POINT_MALL_GOLDBONUS:
			value = 1 + float(value) / 100.0

		trashValue = 123
		#if affect == chr.NEW_AFFECT_AUTO_SP_RECOVERY or affect == chr.NEW_AFFECT_AUTO_HP_RECOVERY:
		if trashValue == 1:
			try:
				#image = AutoPotionImage()
				#image.SetParent(self)
				image = None
				
				if affect == chr.NEW_AFFECT_AUTO_SP_RECOVERY:
					image.SetPotionType(player.AUTO_POTION_TYPE_SP)
					image = self.autoPotionImageSP
					#self.autoPotionImageSP = image;
				else:
					image.SetPotionType(player.AUTO_POTION_TYPE_HP)
					image = self.autoPotionImageHP
					#self.autoPotionImageHP = image;
				
				image.SetParent(self)
				image.Show()
				image.OnUpdateAutoPotionImage()
				
				self.affectImageDict[affect] = image
				self.__ArrangeImageList()
				
			except Exception, e:
				print "except Aff auto potion affect ", e
				pass				
			
		else:
			if app.ENABLE_NEW_PET_SYSTEM:
				if affect != chr.NEW_AFFECT_AUTO_SP_RECOVERY and affect != chr.NEW_AFFECT_AUTO_HP_RECOVERY and not self.AFFECT_NEW_AFFECT.has_key(affect) and not self.AFFECT_PET_DATA_DICT.has_key(affect):
					description = description(float(value))
			else:
				if affect != chr.NEW_AFFECT_AUTO_SP_RECOVERY and affect != chr.NEW_AFFECT_AUTO_HP_RECOVERY and not self.AFFECT_NEW_AFFECT.has_key(affect):
					description = description(float(value))
				else:
					if self.AFFECT_NEW_AFFECT.has_key(affect):
						if value != 0 and affect != 220 and affect != 347:
							if affect < 336:
								description = str(self.AFFECT_NEW_AFFECT[affect]) + "+" + str(float(value))
							elif affect == 735:
								description = str(self.AFFECT_NEW_AFFECT[affect])
							else:
								description = str(self.AFFECT_NEW_AFFECT[affect]) + "+%" + str(float(value))
						else:
							description = str(self.AFFECT_NEW_AFFECT[affect])
					else:	
						description = ""

			try:
				print "Add affect %s" % affect
				image = AffectImage()
				image.SetParent(self)
				image.LoadImage(filename)
				image.SetDescription(description)
				image.SetDuration(duration)
				image.SetAffect(affect)
				if affect == chr.NEW_AFFECT_EXP_BONUS_EURO_FREE or\
					affect == chr.NEW_AFFECT_EXP_BONUS_EURO_FREE_UNDER_15 or\
					self.INFINITE_AFFECT_DURATION < duration:
					image.SetClock(FALSE)
					image.UpdateDescription()
				elif affect == chr.NEW_AFFECT_AUTO_SP_RECOVERY or affect == chr.NEW_AFFECT_AUTO_HP_RECOVERY:
					image.UpdateAutoPotionDescription()
				else:
					image.UpdateDescription()
					
				if affect == chr.NEW_AFFECT_DRAGON_SOUL_DECK1 or affect == chr.NEW_AFFECT_DRAGON_SOUL_DECK2:
					image.SetScale(1, 1)
				else:
					image.SetScale(0.7, 0.7)
				image.SetSkillAffectFlag(FALSE)
				image.Show()
				self.affectImageDict[affect] = image
				self.__ArrangeImageList()
			except Exception, e:
				print "except Aff affect ", e
				pass

	def BINARY_NEW_RemoveAffect(self, type, pointIdx):
		if type == chr.NEW_AFFECT_MALL:
			affect = self.MALL_DESC_IDX_START + pointIdx
		else:
			affect = type
	
		print "Remove Affect %s %s" % ( type , pointIdx )
		self.__RemoveAffect(affect)
		self.__ArrangeImageList()

	def SetAffect(self, affect):
		self.__AppendAffect(affect)
		self.__ArrangeImageList()

	def ResetAffect(self, affect):
		self.__RemoveAffect(affect)
		self.__ArrangeImageList()

	def SetLoverInfo(self, name, lovePoint):
		image = LovePointImage()
		image.SetParent(self)
		image.SetLoverInfo(name, lovePoint)
		self.lovePointImage = image
		self.__ArrangeImageList()

	def ShowLoverState(self):
		if self.lovePointImage:
			self.lovePointImage.Show()
			self.__ArrangeImageList()

	def HideLoverState(self):
		if self.lovePointImage:
			self.lovePointImage.Hide()
			self.__ArrangeImageList()

	def ClearLoverState(self):
		self.lovePointImage = None
		self.__ArrangeImageList()

	def OnUpdateLovePoint(self, lovePoint):
		if self.lovePointImage:
			self.lovePointImage.OnUpdateLovePoint(lovePoint)

	def SetHorseState(self, level, health, battery):
		if level==0:
			self.horseImage=None
		else:
			image = HorseImage()
			image.SetParent(self)
			image.SetState(level, health, battery)
			image.Show()

			self.horseImage=image
			self.__ArrangeImageList()

	def SetPlayTime(self, playTime):
		self.serverPlayTime = playTime
		self.clientPlayTime = app.GetTime()
		
		if localeInfo.IsVIETNAM():		
			image = PlayTimeImage()
			image.SetParent(self)
			image.SetPlayTime(playTime)
			image.Show()

			self.playTimeImage=image
			self.__ArrangeImageList()

	def __AppendAffect(self, affect):

		if self.affectImageDict.has_key(affect):
			return

		try:
			affectData = self.AFFECT_DATA_DICT[affect]
		except KeyError:
			return

		name = affectData[0]
		filename = affectData[1]

		skillIndex = player.AffectIndexToSkillIndex(affect)
		if 0 != skillIndex:
			name = skill.GetSkillName(skillIndex)

		image = AffectImage()
		image.SetParent(self)
		image.SetSkillAffectFlag(TRUE)

		if app.ENABLE_AFFECT_BUFF_REMOVE:
			image.SetSkillIndex(skillIndex)

		try:
			image.LoadImage(filename)
		except:
			pass

		image.SetToolTipText(name, 0, 40)
		image.SetScale(0.7, 0.7)
		image.Show()
		self.affectImageDict[affect] = image

	def __RemoveAffect(self, affect):
		"""
		if affect == chr.NEW_AFFECT_AUTO_SP_RECOVERY:
			self.autoPotionImageSP.Hide()

		if affect == chr.NEW_AFFECT_AUTO_HP_RECOVERY:
			self.autoPotionImageHP.Hide()
		"""
			
		if not self.affectImageDict.has_key(affect):
			print "__RemoveAffect %s ( No Affect )" % affect
			return

		print "__RemoveAffect %s ( Affect )" % affect
		del self.affectImageDict[affect]
		
		self.__ArrangeImageList()

	def __ArrangeImageList(self):

		# width = len(self.affectImageDict) * self.IMAGE_STEP
		width = 12 * self.IMAGE_STEP

		if self.lovePointImage:
			width+=self.IMAGE_STEP
		if self.horseImage:
			width+=self.IMAGE_STEP

		self.SetSize(width, 26*3)

		xPos = 0

		if self.lovePointImage:
			if self.lovePointImage.IsShow():
				self.lovePointImage.SetPosition(xPos, 0)
				xPos += self.IMAGE_STEP

		if self.horseImage:
			self.horseImage.SetPosition(xPos, 0)
			xPos += self.IMAGE_STEP
		i = 0
		y = 0
		kactane = len(self.affectImageDict)

		for image in self.affectImageDict.values():
			if i == 12:
				y = 26
				xPos = 0
			elif i == 24:
				y = 52
				xPos = 0
			
			i+=1
			image.SetPosition(xPos, y)
			xPos += self.IMAGE_STEP

	def OnUpdate(self):		
		try:
			if app.GetGlobalTime() - self.lastUpdateTime > 500:
			#if 0 < app.GetGlobalTime():
				self.lastUpdateTime = app.GetGlobalTime()

				for image in self.affectImageDict.values():
					if image.GetAffect() == chr.NEW_AFFECT_AUTO_HP_RECOVERY or image.GetAffect() == chr.NEW_AFFECT_AUTO_SP_RECOVERY:
						image.UpdateAutoPotionDescription()
						continue
						
					if not image.IsSkillAffect():
						image.UpdateDescription()
		except Exception, e:
			print "AffectShower::OnUpdate error : ", e
