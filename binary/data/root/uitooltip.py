import dbg
import player
import item
import grp
import wndMgr
import skill
import shop
import exchange
import grpText
import safebox
import localeInfo
import app
import background
import nonplayer
import chr

import ui
import mouseModule
import constInfo
if app.__ENABLE_NEW_OFFLINESHOP__:
	import uinewofflineshop

if app.ENABLE_SASH_SYSTEM:
	import sash
if app.ENABLE_AURA_SYSTEM:
	import aura
if app.ENABLE_DS_SET:
	import uiDragonSoul
if app.ENABLE_NEW_PET_SYSTEM:
	import petsinfo

WARP_SCROLLS = [22011, 22000, 22010]

DESC_DEFAULT_MAX_COLS = 26 
DESC_WESTERN_MAX_COLS = 35
DESC_WESTERN_MAX_WIDTH = 220

def chop(n):
	return round(n - 0.5, 1)

def pointop(n):
	t = int(n)
	if t / 10 < 1:
		return "0."+n
	else:		
		return n[0:len(n)-1]+"."+n[len(n)-1:]

def SplitDescription(desc, limit):
	total_tokens = desc.split()
	line_tokens = []
	line_len = 0
	lines = []
	for token in total_tokens:
		if "|" in token:
			sep_pos = token.find("|")
			line_tokens.append(token[:sep_pos])

			lines.append(" ".join(line_tokens))
			line_len = len(token) - (sep_pos + 1)
			line_tokens = [token[sep_pos+1:]]
		else:
			line_len += len(token)
			if len(line_tokens) + line_len > limit:
				lines.append(" ".join(line_tokens))
				line_len = len(token)
				line_tokens = [token]
			else:
				line_tokens.append(token)
	
	if line_tokens:
		lines.append(" ".join(line_tokens))

	return lines

###################################################################################################
## ToolTip
##
##   NOTE : 현재는 Item과 Skill을 상속으로 특화 시켜두었음
##          하지만 그다지 의미가 없어 보임
##
class ToolTip(ui.ThinBoard):

	TOOL_TIP_WIDTH = 190
	TOOL_TIP_HEIGHT = 10

	TEXT_LINE_HEIGHT = 17

	TITLE_COLOR = grp.GenerateColor(0.9490, 0.9058, 0.7568, 1.0)
	SPECIAL_TITLE_COLOR = grp.GenerateColor(1.0, 0.7843, 0.0, 1.0)
	NORMAL_COLOR = grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0)
	FONT_COLOR = grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0)
	PRICE_COLOR = 0xffFFB96D

	HIGH_PRICE_COLOR = SPECIAL_TITLE_COLOR
	MIDDLE_PRICE_COLOR = grp.GenerateColor(0.85, 0.85, 0.85, 1.0)
	LOW_PRICE_COLOR = grp.GenerateColor(0.7, 0.7, 0.7, 1.0)
	WON_PRICE_COLOR = grp.GenerateColor(0.11, 0.56, 1.0, 1.0)
	PRICE_INFO_COLOR = grp.GenerateColor(1.0, 0.88, 0.0, 1.0)
	
	BINEK_COLOR = grp.GenerateColor(0.8, 0.4, 0.0, 1.0)

	ENABLE_COLOR = grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0)
	DISABLE_COLOR = grp.GenerateColor(0.9, 0.4745, 0.4627, 1.0)

	NEGATIVE_COLOR = grp.GenerateColor(0.9, 0.4745, 0.4627, 1.0)
	POSITIVE_COLOR = grp.GenerateColor(0.5411, 0.7254, 0.5568, 1.0)
	SPECIAL_POSITIVE_COLOR = grp.GenerateColor(0.6911, 0.8754, 0.7068, 1.0)
	SPECIAL_POSITIVE_COLOR2 = grp.GenerateColor(0.8824, 0.9804, 0.8824, 1.0)

	if app.ENABLE_DS_SET:
		TEXTLINE_2ND_COLOR_DEFAULT = grp.GenerateColor(1.0, 1.0, 0.6078, 1.0)

	SILAH_EVRIM_VALUE = {
		0	: '',
		1	: ' |cff00ff00|HVectors:|h(+120)|h|r',
		2	: ' |cff00ff00|HVectors:|h(+240)|h|r',
		3	: ' |cff00ff00|HVectors:|h(+360)|h|r',
		4	: ' |cff00ff00|HVectors:|h(+480)|h|r',
		5	: ' |cff00ff00|HVectors:|h(+600)|h|r',
	}
	SILAH_EVRIM_TITLE = {
		0	: localeInfo.SILAH_EVRIM_SISTEMI_TITLE_0,
		1	: localeInfo.SILAH_EVRIM_SISTEMI_TITLE_1,
		2	: localeInfo.SILAH_EVRIM_SISTEMI_TITLE_2,
		3	: localeInfo.SILAH_EVRIM_SISTEMI_TITLE_3,
		4	: localeInfo.SILAH_EVRIM_SISTEMI_TITLE_4,
		5	: localeInfo.SILAH_EVRIM_SISTEMI_TITLE_5
	}
	itemEvolution = 0

	SHOP_ITEM_COLOR = 0xfffff64e
	RED_COLOR = grp.GenerateColor(051,255,255, 1.0)
	BLUE_COLOR = grp.GenerateColor(0,0.75,1, 1.0)
	GREEN_COLOR = grp.GenerateColor(0.23,0.83,0.24, 1.0)
	YELLOW_COLOR = 0xffFFFF00
	ORANGE_COLOR = grp.GenerateColor(24,116,205, 1.0)
	PINK_COLOR = grp.GenerateColor(1.0,0.0,1.0, 1.0)
	PURPLE_COLOR = grp.GenerateColor(0.63,0.13,0.94, 1.0)
	CONDITION_COLOR = 0xffBEB47D
	CAN_LEVEL_UP_COLOR = 0xff8EC292
	CANNOT_LEVEL_UP_COLOR = DISABLE_COLOR
	NEED_SKILL_POINT_COLOR = 0xff9A9CDB
	if app.ENABLE_CHANGELOOK_SYSTEM:
		UNDER_LOOK_COLOR = 0xffADFF2F
		BEFORE_LOOK_COLOR = 0xff9A9CDB

	if app.ENABLE_NEW_PET_SYSTEM:
		ITEM_BUFF_LEVEL_COLOR = 0xffffd300
		ITEM_BUFF_TYPE_COLOR = 0xfffc9c3a
		ITEM_BUFF_RATE_COLOR = 0xff40e0d0
		ITEM_BUFF_DURATION_COLOR = 0xffadff00
		ITEM_BUFF_USAGE_COLOR = 0xffffffff

	def __init__(self, width = TOOL_TIP_WIDTH, isPickable=False):
		ui.ThinBoard.__init__(self, "TOP_MOST")

		if isPickable:
			pass
		else:
			self.AddFlag("not_pick")

		self.AddFlag("float")

		self.followFlag = True
		self.toolTipWidth = width

		self.xPos = -1
		self.yPos = -1
		

		if app.ENABLE_DS_SET:
			self.window_type = player.INVENTORY
			
		self.Board = ui.ThinBoard()
		self.Board.SetSize(200, 250)
		self.Board.AddFlag('float')
		self.Board.Hide()

		self.defFontName = localeInfo.UI_DEF_FONT
		self.ClearToolTip()

	def __del__(self):
		ui.ThinBoard.__del__(self)

	def ClearToolTip(self):
		self.toolTipHeight = 12
		self.childrenList = []

	def SetFollow(self, flag):
		self.followFlag = flag

	def SetDefaultFontName(self, fontName):
		self.defFontName = fontName

	def AppendSpace(self, size):
		self.toolTipHeight += size
		self.ResizeToolTip()

	def AppendHorizontalLine(self):

		for i in xrange(2):
			horizontalLine = ui.Line()
			horizontalLine.SetParent(self)
			horizontalLine.SetPosition(0, self.toolTipHeight + 3 + i)
			horizontalLine.SetWindowHorizontalAlignCenter()
			horizontalLine.SetSize(150, 0)
			horizontalLine.Show()

			if 0 == i:
				horizontalLine.SetColor(0xff555555)
			else:
				horizontalLine.SetColor(0xff000000)

			self.childrenList.append(horizontalLine)

		self.toolTipHeight += 11
		self.ResizeToolTip()

	def AlignHorizonalCenter(self):
		for child in self.childrenList:
			(x, y)=child.GetLocalPosition()
			child.SetPosition(self.toolTipWidth/2, y)

		self.ResizeToolTip()

	def AutoAppendTextLine(self, text, color = FONT_COLOR, centerAlign = True):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPackedFontColor(color)
		textLine.SetText(text)
		textLine.SetOutline()
		textLine.SetFeather(False)
		textLine.Show()

		if centerAlign:
			textLine.SetPosition(self.toolTipWidth/2, self.toolTipHeight)
			textLine.SetHorizontalAlignCenter()

		else:
			textLine.SetPosition(10, self.toolTipHeight)

		self.childrenList.append(textLine)

		(textWidth, textHeight)=textLine.GetTextSize()

		textWidth += 40
		textHeight += 5

		if self.toolTipWidth < textWidth:
			self.toolTipWidth = textWidth

		self.toolTipHeight += textHeight

		return textLine

	def AutoAppendNewTextLine(self, text, color = FONT_COLOR, centerAlign = True):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPackedFontColor(color)
		textLine.SetText(text)
		textLine.SetOutline()
		textLine.SetFeather(False)
		textLine.Show()
		textLine.SetPosition(15, self.toolTipHeight)

		self.childrenList.append(textLine)
		(textWidth, textHeight) = textLine.GetTextSize()

		textWidth += 30
		textHeight += 10

		if self.toolTipWidth < textWidth:
			self.toolTipWidth = textWidth

		self.toolTipHeight += textHeight
		self.ResizeToolTipText(textWidth, self.toolTipHeight)

		return textLine


	def AutoAppendNewTextLine(self, text, color = FONT_COLOR, centerAlign = True):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPackedFontColor(color)
		textLine.SetText(text)
		textLine.SetOutline()
		textLine.SetFeather(False)
		textLine.Show()
		textLine.SetPosition(15, self.toolTipHeight)

		self.childrenList.append(textLine)
		(textWidth, textHeight) = textLine.GetTextSize()

		textWidth += 30
		textHeight += 10

		if self.toolTipWidth < textWidth:
			self.toolTipWidth = textWidth

		self.toolTipHeight += textHeight
		self.ResizeToolTipText(textWidth, self.toolTipHeight)

		return textLine

	if app.RADIO_BUTTON_TOOLTIP_UPDATE:
		def SetThinBoardSize(self, width, height = 12):
			self.toolTipWidth = width
			self.toolTipHeight = height

	def StoneAppendTextLine(self, text, color = FONT_COLOR, centerAlign = True):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPackedFontColor(color)
		textLine.SetText(text)
		textLine.SetOutline()
		textLine.SetFeather(False)
		textLine.Show()

		if centerAlign:
			textLine.SetPosition(52, self.toolTipHeight)

		else:
			textLine.SetPosition(10, self.toolTipHeight)

		self.childrenList.append(textLine)

		self.childrenList.append(textLine)
		self.ResizeToolTip()

	def AppendTextLine(self, text, color = FONT_COLOR, centerAlign = True):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPackedFontColor(color)
		textLine.SetText(text)
		textLine.SetOutline()
		textLine.SetFeather(False)
		textLine.Show()

		if centerAlign:
			textLine.SetPosition(self.toolTipWidth/2, self.toolTipHeight)
			textLine.SetHorizontalAlignCenter()

		else:
			textLine.SetPosition(10, self.toolTipHeight)

		self.childrenList.append(textLine)

		self.toolTipHeight += self.TEXT_LINE_HEIGHT
		self.ResizeToolTip()

		return textLine
		
	if app.ENABLE_DS_SET:
		def AppendTwoColorTextLine(self, text, color, text2, color2 = TEXTLINE_2ND_COLOR_DEFAULT, centerAlign = True):
			textLine = ui.TextLine()
			textLine.SetParent(self)
			textLine.SetFontName(self.defFontName)
			textLine.SetPackedFontColor(color)
			textLine.SetText(text)
			textLine.SetOutline()
			textLine.SetFeather(False)
			textLine.Show()

			textLine2 = ui.TextLine()
			textLine2.SetParent(textLine)
			textLine2.SetFontName(self.defFontName)
			textLine2.SetPackedFontColor(color2)
			textLine2.SetText(text2)
			textLine2.SetOutline()
			textLine2.SetFeather(False)
			textLine2.Show()

			w, h = textLine.GetTextSize()
			w2, h2 = textLine2.GetTextSize()

			if centerAlign:
				textLine.SetPosition(self.toolTipWidth / 2 - w2 / 2, self.toolTipHeight)
				textLine.SetHorizontalAlignCenter()
				textLine2.SetPosition(w / 2, 0)
			else:
				textLine.SetPosition(10, self.toolTipHeight)

			self.childrenList.append(textLine)
			self.childrenList.append(textLine2)

			self.toolTipHeight += self.TEXT_LINE_HEIGHT
			self.ResizeToolTip()

			return textLine

	def AppendDescription(self, desc, limit, color = FONT_COLOR):
		if localeInfo.IsEUROPE():
			self.__AppendDescription_WesternLanguage(desc, color)
		else:
			self.__AppendDescription_EasternLanguage(desc, limit, color)

	def __AppendDescription_EasternLanguage(self, description, characterLimitation, color=FONT_COLOR):
		length = len(description)
		if 0 == length:
			return

		lineCount = grpText.GetSplitingTextLineCount(description, characterLimitation)
		for i in xrange(lineCount):
			if 0 == i:
				self.AppendSpace(5)
			self.AppendTextLine(grpText.GetSplitingTextLine(description, characterLimitation, i), color)

	def __AppendDescription_WesternLanguage(self, desc, color=FONT_COLOR):
		lines = SplitDescription(desc, DESC_WESTERN_MAX_COLS)
		if not lines:
			return

		self.AppendSpace(5)
		for line in lines:
			self.AppendTextLine(line, color)
			

	def ResizeToolTip(self):
		self.SetSize(self.toolTipWidth, self.TOOL_TIP_HEIGHT + self.toolTipHeight)

	def ResizeToolTipText(self, x, y):
		self.SetSize(x, y)

	def SetTitle(self, name):
		self.AppendTextLine(name, self.TITLE_COLOR)

	def GetLimitTextLineColor(self, curValue, limitValue):
		if curValue < limitValue:
			return self.DISABLE_COLOR

		return self.ENABLE_COLOR

	def GetChangeTextLineColor(self, value, isSpecial=False):
		if value > 0:
			if isSpecial:
				return self.SPECIAL_POSITIVE_COLOR
			else:
				return self.POSITIVE_COLOR

		if 0 == value:
			return self.NORMAL_COLOR

		return self.NEGATIVE_COLOR

	def SetToolTipPosition(self, x = -1, y = -1):
		self.xPos = x
		self.yPos = y

	def ShowToolTip(self):
		self.SetTop()
		self.Show()

		self.OnUpdate()

	def HideToolTip(self):
		self.Hide()
		self.Board.Hide()

	def OnUpdate(self):

		if not self.followFlag:
			return

		x = 0
		y = 0
		width = self.GetWidth()
		height = self.toolTipHeight

		if -1 == self.xPos and -1 == self.yPos:

			(mouseX, mouseY) = wndMgr.GetMousePosition()

			if mouseY < wndMgr.GetScreenHeight() - 300:
				y = mouseY + 40
			else:
				y = mouseY - height - 30

			x = mouseX - width/2				

		else:

			x = self.xPos - width/2
			y = self.yPos - height

		x = max(x, 0)
		y = max(y, 0)
		x = min(x + width/2, wndMgr.GetScreenWidth() - width/2) - width/2
		y = min(y + self.GetHeight(), wndMgr.GetScreenHeight()) - self.GetHeight()

		parentWindow = self.GetParentProxy()
		if parentWindow:
			(gx, gy) = parentWindow.GetGlobalPosition()
			x -= gx
			y -= gy

		self.SetPosition(x, y)

class ItemToolTip(ToolTip):

	if app.ENABLE_SEND_TARGET_INFO:
		isStone = False
		isBook = False
		isBook2 = False

	if app.ENABLE_RENDER_TARGET_SYSTEM:
		ModelWindow = None
		ModelWindowBoard = None
		CacheData = (0, 0, 0, 0, 0, chr.MOTION_MODE_GENERAL)

	CHARACTER_NAMES = (
		"|Eemoji/warrior_m|e",
		"|Eemoji/assassin_w|e",
		"|Eemoji/sura_m|e",
		"|Eemoji/shaman_w|e",
		"|Eemoji/wolfman_m|e",
	)

	CHARACTER_COUNT = len(CHARACTER_NAMES)
	WEAR_NAMES = ( 
		localeInfo.TOOLTIP_ARMOR, 
		localeInfo.TOOLTIP_HELMET, 
		localeInfo.TOOLTIP_SHOES, 
		localeInfo.TOOLTIP_WRISTLET, 
		localeInfo.TOOLTIP_WEAPON, 
		localeInfo.TOOLTIP_NECK,
		localeInfo.TOOLTIP_EAR,
		localeInfo.TOOLTIP_UNIQUE,
		localeInfo.TOOLTIP_SHIELD,
		localeInfo.TOOLTIP_ARROW,
	)
	WEAR_COUNT = len(WEAR_NAMES)

	if app.ITEM_SET_BONUS:
		ITEM_SET = {
			# id(sıra) : [description(acıklama),value1(de?er 1),value2(de?er 2), value3(de?er 3)],
			0 : [localeInfo.itemsetbonus0,0,0,0],
			1 : [localeInfo.itemsetbonus1,1,2,5],
			2 : [localeInfo.itemsetbonus1,2,3,5],
			3 : [localeInfo.itemsetbonus1,3,4,8],
			4 : [localeInfo.itemsetbonus1,5,5,10],
			5 : [localeInfo.itemsetbonus2,1000,1500,2000],
			
		}
		
	AFFECT_DICT = {
		item.APPLY_MAX_HP : localeInfo.TOOLTIP_MAX_HP,
		item.APPLY_MAX_SP : localeInfo.TOOLTIP_MAX_SP,
		item.APPLY_CON : localeInfo.TOOLTIP_CON,
		item.APPLY_INT : localeInfo.TOOLTIP_INT,
		item.APPLY_STR : localeInfo.TOOLTIP_STR,
		item.APPLY_DEX : localeInfo.TOOLTIP_DEX,
		item.APPLY_ATT_SPEED : localeInfo.TOOLTIP_ATT_SPEED,
		item.APPLY_MOV_SPEED : localeInfo.TOOLTIP_MOV_SPEED,
		item.APPLY_CAST_SPEED : localeInfo.TOOLTIP_CAST_SPEED,
		item.APPLY_HP_REGEN : localeInfo.TOOLTIP_HP_REGEN,
		item.APPLY_SP_REGEN : localeInfo.TOOLTIP_SP_REGEN,
		item.APPLY_POISON_PCT : localeInfo.TOOLTIP_APPLY_POISON_PCT,
		item.APPLY_STUN_PCT : localeInfo.TOOLTIP_APPLY_STUN_PCT,
		item.APPLY_SLOW_PCT : localeInfo.TOOLTIP_APPLY_SLOW_PCT,
		item.APPLY_CRITICAL_PCT : localeInfo.TOOLTIP_APPLY_CRITICAL_PCT,
		item.APPLY_PENETRATE_PCT : localeInfo.TOOLTIP_APPLY_PENETRATE_PCT,

		item.APPLY_ATTBONUS_WARRIOR : localeInfo.TOOLTIP_APPLY_ATTBONUS_WARRIOR,
		item.APPLY_ATTBONUS_ASSASSIN : localeInfo.TOOLTIP_APPLY_ATTBONUS_ASSASSIN,
		item.APPLY_ATTBONUS_SURA : localeInfo.TOOLTIP_APPLY_ATTBONUS_SURA,
		item.APPLY_ATTBONUS_SHAMAN : localeInfo.TOOLTIP_APPLY_ATTBONUS_SHAMAN,
		item.APPLY_ATTBONUS_WOLFMAN : localeInfo.TOOLTIP_APPLY_ATTBONUS_WOLFMAN,
		item.APPLY_ATTBONUS_MONSTER : localeInfo.TOOLTIP_APPLY_ATTBONUS_MONSTER,

		item.APPLY_ATTBONUS_HUMAN : localeInfo.TOOLTIP_APPLY_ATTBONUS_HUMAN,
		item.APPLY_ATTBONUS_ANIMAL : localeInfo.TOOLTIP_APPLY_ATTBONUS_ANIMAL,
		item.APPLY_ATTBONUS_ORC : localeInfo.TOOLTIP_APPLY_ATTBONUS_ORC,
		item.APPLY_ATTBONUS_MILGYO : localeInfo.TOOLTIP_APPLY_ATTBONUS_MILGYO,
		item.APPLY_ATTBONUS_UNDEAD : localeInfo.TOOLTIP_APPLY_ATTBONUS_UNDEAD,
		item.APPLY_ATTBONUS_DEVIL : localeInfo.TOOLTIP_APPLY_ATTBONUS_DEVIL,
		item.APPLY_STEAL_HP : localeInfo.TOOLTIP_APPLY_STEAL_HP,
		item.APPLY_STEAL_SP : localeInfo.TOOLTIP_APPLY_STEAL_SP,
		item.APPLY_MANA_BURN_PCT : localeInfo.TOOLTIP_APPLY_MANA_BURN_PCT,
		item.APPLY_DAMAGE_SP_RECOVER : localeInfo.TOOLTIP_APPLY_DAMAGE_SP_RECOVER,
		item.APPLY_BLOCK : localeInfo.TOOLTIP_APPLY_BLOCK,
		item.APPLY_DODGE : localeInfo.TOOLTIP_APPLY_DODGE,
		item.APPLY_RESIST_SWORD : localeInfo.TOOLTIP_APPLY_RESIST_SWORD,
		item.APPLY_RESIST_TWOHAND : localeInfo.TOOLTIP_APPLY_RESIST_TWOHAND,
		item.APPLY_RESIST_DAGGER : localeInfo.TOOLTIP_APPLY_RESIST_DAGGER,
		item.APPLY_RESIST_BELL : localeInfo.TOOLTIP_APPLY_RESIST_BELL,
		item.APPLY_RESIST_FAN : localeInfo.TOOLTIP_APPLY_RESIST_FAN,
		item.APPLY_RESIST_BOW : localeInfo.TOOLTIP_RESIST_BOW,
		item.APPLY_RESIST_FIRE : localeInfo.TOOLTIP_RESIST_FIRE,
		item.APPLY_RESIST_ELEC : localeInfo.TOOLTIP_RESIST_ELEC,
		item.APPLY_RESIST_MAGIC : localeInfo.TOOLTIP_RESIST_MAGIC,
		item.APPLY_RESIST_WIND : localeInfo.TOOLTIP_APPLY_RESIST_WIND,
		item.APPLY_RESIST_CLAW : localeInfo.TOOLTIP_APPLY_RESIST_CLAW,
		item.APPLY_REFLECT_MELEE : localeInfo.TOOLTIP_APPLY_REFLECT_MELEE,
		item.APPLY_REFLECT_CURSE : localeInfo.TOOLTIP_APPLY_REFLECT_CURSE,
		item.APPLY_POISON_REDUCE : localeInfo.TOOLTIP_APPLY_POISON_REDUCE,
		item.APPLY_KILL_SP_RECOVER : localeInfo.TOOLTIP_APPLY_KILL_SP_RECOVER,
		item.APPLY_EXP_DOUBLE_BONUS : localeInfo.TOOLTIP_APPLY_EXP_DOUBLE_BONUS,
		item.APPLY_GOLD_DOUBLE_BONUS : localeInfo.TOOLTIP_APPLY_GOLD_DOUBLE_BONUS,
		item.APPLY_ITEM_DROP_BONUS : localeInfo.TOOLTIP_APPLY_ITEM_DROP_BONUS,
		item.APPLY_POTION_BONUS : localeInfo.TOOLTIP_APPLY_POTION_BONUS,
		item.APPLY_KILL_HP_RECOVER : localeInfo.TOOLTIP_APPLY_KILL_HP_RECOVER,
		item.APPLY_IMMUNE_STUN : localeInfo.TOOLTIP_APPLY_IMMUNE_STUN,
		item.APPLY_IMMUNE_SLOW : localeInfo.TOOLTIP_APPLY_IMMUNE_SLOW,
		item.APPLY_IMMUNE_FALL : localeInfo.TOOLTIP_APPLY_IMMUNE_FALL,
		item.APPLY_BOW_DISTANCE : localeInfo.TOOLTIP_BOW_DISTANCE,
		item.APPLY_DEF_GRADE_BONUS : localeInfo.TOOLTIP_DEF_GRADE,
		item.APPLY_ATT_GRADE_BONUS : localeInfo.TOOLTIP_ATT_GRADE,
		item.APPLY_MAGIC_ATT_GRADE : localeInfo.TOOLTIP_MAGIC_ATT_GRADE,
		item.APPLY_MAGIC_DEF_GRADE : localeInfo.TOOLTIP_MAGIC_DEF_GRADE,
		item.APPLY_MAX_STAMINA : localeInfo.TOOLTIP_MAX_STAMINA,
		item.APPLY_MALL_ATTBONUS : localeInfo.TOOLTIP_MALL_ATTBONUS,
		item.APPLY_MALL_DEFBONUS : localeInfo.TOOLTIP_MALL_DEFBONUS,
		item.APPLY_MALL_EXPBONUS : localeInfo.TOOLTIP_MALL_EXPBONUS,
		item.APPLY_MALL_ITEMBONUS : localeInfo.TOOLTIP_MALL_ITEMBONUS,
		item.APPLY_MALL_GOLDBONUS : localeInfo.TOOLTIP_MALL_GOLDBONUS,
		item.APPLY_SKILL_DAMAGE_BONUS : localeInfo.TOOLTIP_SKILL_DAMAGE_BONUS,
		item.APPLY_NORMAL_HIT_DAMAGE_BONUS : localeInfo.TOOLTIP_NORMAL_HIT_DAMAGE_BONUS,
		item.APPLY_SKILL_DEFEND_BONUS : localeInfo.TOOLTIP_SKILL_DEFEND_BONUS,
		item.APPLY_NORMAL_HIT_DEFEND_BONUS : localeInfo.TOOLTIP_NORMAL_HIT_DEFEND_BONUS,
		item.APPLY_PC_BANG_EXP_BONUS : localeInfo.TOOLTIP_MALL_EXPBONUS_P_STATIC,
		item.APPLY_PC_BANG_DROP_BONUS : localeInfo.TOOLTIP_MALL_ITEMBONUS_P_STATIC,
		item.APPLY_RESIST_WARRIOR : localeInfo.TOOLTIP_APPLY_RESIST_WARRIOR,
		item.APPLY_RESIST_ASSASSIN : localeInfo.TOOLTIP_APPLY_RESIST_ASSASSIN,
		item.APPLY_RESIST_SURA : localeInfo.TOOLTIP_APPLY_RESIST_SURA,
		item.APPLY_RESIST_SHAMAN : localeInfo.TOOLTIP_APPLY_RESIST_SHAMAN,
		item.APPLY_RESIST_WOLFMAN : localeInfo.TOOLTIP_APPLY_RESIST_WOLFMAN,
		item.APPLY_MAX_HP_PCT : localeInfo.TOOLTIP_APPLY_MAX_HP_PCT,
		item.APPLY_MAX_SP_PCT : localeInfo.TOOLTIP_APPLY_MAX_SP_PCT,
		item.APPLY_ENERGY : localeInfo.TOOLTIP_ENERGY,
		item.APPLY_COSTUME_ATTR_BONUS : localeInfo.TOOLTIP_COSTUME_ATTR_BONUS,
		
		item.APPLY_MAGIC_ATTBONUS_PER : localeInfo.TOOLTIP_MAGIC_ATTBONUS_PER,
		item.APPLY_MELEE_MAGIC_ATTBONUS_PER : localeInfo.TOOLTIP_MELEE_MAGIC_ATTBONUS_PER,
		item.APPLY_RESIST_ICE : localeInfo.TOOLTIP_RESIST_ICE,
		item.APPLY_RESIST_EARTH : localeInfo.TOOLTIP_RESIST_EARTH,
		item.APPLY_RESIST_DARK : localeInfo.TOOLTIP_RESIST_DARK,
		item.APPLY_ANTI_CRITICAL_PCT : localeInfo.TOOLTIP_ANTI_CRITICAL_PCT,
		item.APPLY_ANTI_PENETRATE_PCT : localeInfo.TOOLTIP_ANTI_PENETRATE_PCT,
		
		item.APPLY_ANTI_RESIST_MAGIC : localeInfo.APPLY_ANTI_RESIST_MAGIC,
		item.APPLY_ATTBONUS_METIN : localeInfo.APPLY_ATTBONUS_METIN,
		item.APPLY_ATTBONUS_ELEC : localeInfo.TOOLTIP_APPLY_ATTBONUS_ELEC,
		item.APPLY_ATTBONUS_FIRE : localeInfo.TOOLTIP_APPLY_ATTBONUS_FIRE,
		item.APPLY_ATTBONUS_ICE : localeInfo.TOOLTIP_APPLY_ATTBONUS_ICE,
		item.APPLY_ATTBONUS_WIND : localeInfo.TOOLTIP_APPLY_ATTBONUS_WIND,
		item.APPLY_ATTBONUS_EARTH : localeInfo.TOOLTIP_APPLY_ATTBONUS_EARTH,
		item.APPLY_ATTBONUS_DARK : localeInfo.TOOLTIP_APPLY_ATTBONUS_DARK,
		item.APPLY_ANTI_SWORD : localeInfo.TOOLTIP_APPLY_ANTI_SWORD,
		item.APPLY_ANTI_TWOHAND : localeInfo.TOOLTIP_APPLY_ANTI_TWOHAND,
		item.APPLY_ANTI_DAGGER : localeInfo.TOOLTIP_APPLY_ANTI_DAGGER,
		item.APPLY_ANTI_BELL : localeInfo.TOOLTIP_APPLY_ANTI_BELL,
		item.APPLY_ANTI_FAN : localeInfo.TOOLTIP_APPLY_ANTI_FAN,
		item.APPLY_ANTI_BOW : localeInfo.TOOLTIP_APPLY_ANTI_BOW,
		item.APPLY_ATT_MONSTER_NEW : localeInfo.TOOLTIP_APPLY_ATT_MONSTER_NEW,
		item.APPLY_ATT_BOSS : localeInfo.TOOLTIP_APPLY_ATT_BOSS,
		item.APPLY_ANTI_HUMAN : localeInfo.TOOLTIP_APPLY_ANTI_HUMAN,
		item.APPLY_RESIST_MONSTER : localeInfo.APPLY_RESIST_MONSTER,
		item.APPLY_ATTBONUS_ELEMENT : localeInfo.TOOLTIP_APPLY_ATTBONUS_ELEMENT,
	}
	if app.ENABLE_GLOVE_SYSTEM:
		AFFECT_DICT[item.APPLY_RANDOM]	= localeInfo.TOOLTIP_APPLY_RANDOM
		
	ATTRIBUTE_NEED_WIDTH = {
		23 : 230,
		24 : 230,
		25 : 230,
		26 : 220,
		27 : 210,

		35 : 210,
		36 : 210,
		37 : 210,
		38 : 210,
		39 : 210,
		40 : 210,
		41 : 210,

		42 : 220,
		43 : 230,
		45 : 230,
	}

	ANTI_FLAG_DICT = {
		0 : item.ITEM_ANTIFLAG_WARRIOR,
		1 : item.ITEM_ANTIFLAG_ASSASSIN,
		2 : item.ITEM_ANTIFLAG_SURA,
		3 : item.ITEM_ANTIFLAG_SHAMAN,
		4 : item.ITEM_ANTIFLAG_WOLFMAN,
	}

	FONT_COLOR = grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0)

	def __init__(self, *args, **kwargs):
		ToolTip.__init__(self, *args, **kwargs)
		self.itemVnum = 0
		self.isShopItem = False
		self.isOfflineShopItem = FALSE	

		if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
			self.isPrivateSearchItem = False
		if app.ENABLE_DS_SET:
			self.interface = None
		# 아이템 툴팁을 표시할 때 현재 캐릭터가 착용할 수 없는 아이템이라면 강제로 Disable Color로 설정 (이미 그렇게 작동하고 있으나 꺼야 할 필요가 있어서)
		self.bCannotUseItemForceSetDisableColor = True 

	def __del__(self):
		ToolTip.__del__(self)

	if app.ENABLE_DS_SET:
		def BindInterface(self, interface):
			from _weakref import proxy
			self.interface = proxy(interface)

	def SetCannotUseItemForceSetDisableColor(self, enable):
		self.bCannotUseItemForceSetDisableColor = enable

	def CanEquip(self):
		if not item.IsEquipmentVID(self.itemVnum):
			return True

		race = player.GetRace()
		job = chr.RaceToJob(race)
		if not self.ANTI_FLAG_DICT.has_key(job):
			return False

		if item.IsAntiFlag(self.ANTI_FLAG_DICT[job]):
			return False

		sex = chr.RaceToSex(race)

		MALE = 1
		FEMALE = 0

		if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE) and sex == MALE:
			return False

		if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE) and sex == FEMALE:
			return False

		for i in xrange(item.LIMIT_MAX_NUM):
			(limitType, limitValue) = item.GetLimit(i)

			if item.LIMIT_LEVEL == limitType:
				if player.GetStatus(player.LEVEL) < limitValue:
					return False
			"""
			elif item.LIMIT_STR == limitType:
				if player.GetStatus(player.ST) < limitValue:
					return False
			elif item.LIMIT_DEX == limitType:
				if player.GetStatus(player.DX) < limitValue:
					return False
			elif item.LIMIT_INT == limitType:
				if player.GetStatus(player.IQ) < limitValue:
					return False
			elif item.LIMIT_CON == limitType:
				if player.GetStatus(player.HT) < limitValue:
					return False
			"""

		return True

	def AppendTextLine(self, text, color = FONT_COLOR, centerAlign = True):
		if not self.CanEquip() and self.bCannotUseItemForceSetDisableColor:
			color = self.DISABLE_COLOR

		return ToolTip.AppendTextLine(self, text, color, centerAlign)

	def ClearToolTip(self):
		self.isShopItem = False
		self.isOfflineShopItem = FALSE	
		self.toolTipWidth = self.TOOL_TIP_WIDTH
		ToolTip.ClearToolTip(self)

		if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
			self.isPrivateSearchItem = False
	if app.ENABLE_SEND_TARGET_INFO:
		def SetItemToolTipStone(self, itemVnum):
			self.itemVnum = itemVnum
			item.SelectItem(itemVnum)
			itemType = item.GetItemType()

			itemDesc = item.GetItemDescription()
			itemSummary = item.GetItemSummary()
			attrSlot = 0
			self.__AdjustMaxWidth(attrSlot, itemDesc)
			itemName = item.GetItemName()
			realName = itemName[:itemName.find("+")]
			self.SetTitle(realName + " +0 - +4")

			## Description ###
			self.AppendDescription(itemDesc, 26)
			self.AppendDescription(itemSummary, 26, self.CONDITION_COLOR)
			self.AppendDescription(itemRed, 26, self.RED_COLOR)
			self.AppendDescription(itemBlue, 26, self.BLUE_COLOR)
			self.AppendDescription(itemGreen, 26, self.GREEN_COLOR)
			self.AppendDescription(itemYellow, 26, self.YELLOW_COLOR)
			self.AppendDescription(itemOrange, 26, self.ORANGE_COLOR)
			self.AppendDescription(itemPink, 26, self.PINK_COLOR)
			self.AppendDescription(itemPurple, 26, self.PURPLE_COLOR)

			if item.ITEM_TYPE_METIN == itemType:
				self.AppendMetinInformation()
				self.AppendMetinWearInformation()

			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)

				if item.LIMIT_REAL_TIME_START_FIRST_USE == limitType:
					self.AppendRealTimeStartFirstUseLastTime(item, metinSlot, i)

				elif item.LIMIT_TIMER_BASED_ON_WEAR == limitType:
					self.AppendTimerBasedOnWearLastTime(metinSlot)

			self.ShowToolTip()
	def SetOfflineShopBuilderItem(self, invenType, invenPos, offlineShopIndex):
		itemVnum = player.GetItemIndex(invenType, invenPos)
		if (itemVnum == 0):
			return

		item.SelectItem(itemVnum)
		self.ClearToolTip()

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(player.GetItemMetinSocket(invenPos, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(player.GetItemAttribute(invenPos, i))
		evo = shop.GetItemEvolutionOffline(invenType, invenPos)
		self.AddItemData(itemVnum, metinSlot, attrSlot,0,0,0,evo)
		
		price = shop.GetOfflineShopItemPrice2(invenType, invenPos)

		if app.ENABLE_CHEQUE_SYSTEM:
			price_cheque = shop.GetOfflineShopItemPriceCheque2(invenType, invenPos)
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.CHEQUE_SYSTEM_SELL_PRICE)
			if price_cheque > 0:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.NumberToWonString(price_cheque))
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.NumberToGoldString(price))
		else:
			self.AppendSellingPrice(shop.GetOfflineShopItemPrice2(invenType, invenPos))
		
	def SetOfflineShopItem(self, slotIndex):
		itemVnum = shop.GetOfflineShopItemID(slotIndex)
		if (itemVnum == 0):
			return

		self.ClearToolTip()
		self.isOfflineShopItem = True

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(shop.GetOfflineShopItemMetinSocket(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(shop.GetOfflineShopItemAttribute(slotIndex, i))

		itemevo = shop.GetItemEvolutionOffline(slotIndex)
		self.AddItemData(itemVnum, metinSlot, attrSlot, 0,0,0,itemevo)

		price = shop.GetOfflineShopItemPrice(slotIndex)
		price_cheque = shop.GetOfflineShopItemPriceCheque(slotIndex)

		if app.ENABLE_CHEQUE_SYSTEM:
			if price_cheque > 0:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.CHEQUE_SYSTEM_SELL_PRICE, grp.GenerateColor(255./255, 225./255, 0./255, 1.0))
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.NumberToWonString(price_cheque), grp.GenerateColor(30./255, 144./255, 255./255, 1.0))
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.NumberToGoldString(price), self.GetPriceColor(price))
			else:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.CHEQUE_SYSTEM_SELL_PRICE, grp.GenerateColor(255./255, 215./255, 0./255, 1.0))
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.NumberToGoldString(price), self.GetPriceColor(price))
		else:
			self.AppendPrice(price)
	
	if app.ENABLE_CHEQUE_SYSTEM:
		def AppendSellingChequePrice(self, cheque):
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.NumberToWonString(cheque), grp.GenerateColor(30./255, 144./255, 255./255, 1.0))
	
	
	def SetShopItem(self, slotIndex, addprice = 0, waltype = 0):
		import localeInfo
		itemVnum = shop.GetItemID(slotIndex)
		if 0 == itemVnum:
			return

		price = shop.GetItemPrice(slotIndex)
		if app.ENABLE_CHEQUE_SYSTEM:
			price_cheque = shop.GetItemPriceCheque(slotIndex)
			price_yang = shop.GetItemPrice(slotIndex)
			if app.PRICE_TYPE:
				price_type = shop.GetItemPriceType(slotIndex)
		self.ClearToolTip()
		self.isShopItem = True
		item.SelectItem(itemVnum)
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(shop.GetItemMetinSocket(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(shop.GetItemAttribute(slotIndex, i)) 
		self.AddItemData(itemVnum, metinSlot, attrSlot) 
		if waltype == 0:
			if app.ENABLE_CHEQUE_SYSTEM:
				if shop.IsPrivateShop():
					if app.PRICE_TYPE:
						if price_type == 0:
							self.AppendSpace(5)
							self.AppendTextLine(localeInfo.CHEQUE_SYSTEM_SELL_PRICE, grp.GenerateColor(255./255, 246./255, 78./255, 1.0))
							if price_cheque > 0:
								self.AppendSpace(5)
								self.AppendTextLine(localeInfo.NumberToWonString(price_cheque), grp.GenerateColor(0./255, 215./255, 255./255, 1.0))
							if price_yang > 0:
								self.AppendSpace(5)
								self.AppendTextLine(localeInfo.NumberToGoldString(price), self.GetPriceColor(price))
						elif price_type == 1:
							self.AppendPriceNP(price)
						elif price_type == 2:
							self.AppendPriceEP(price)
						elif price_type == 3:
							self.AppendPriceWon(price)
						else:
							self.AppendPriceWithType(price, price_type)
					else:
						self.AppendSpace(5)
						self.AppendTextLine(localeInfo.CHEQUE_SYSTEM_SELL_PRICE, grp.GenerateColor(255./255, 246./255, 78./255, 1.0))
						if price_cheque > 0:
							self.AppendSpace(5)
							self.AppendTextLine(localeInfo.NumberToWonString(price_cheque), grp.GenerateColor(0./255, 215./255, 255./255, 1.0))
						if price_yang > 0:
							self.AppendSpace(5)
							self.AppendTextLine(localeInfo.NumberToGoldString(price), self.GetPriceColor(price))
				else:
					if app.PRICE_TYPE:
						if price_type == 0:
							self.AppendPrice(price)
						elif price_type == 1:
							self.AppendPriceNP(price)
						elif price_type == 2:
							self.AppendPriceEP(price)
						elif price_type == 3:
							self.AppendPriceWon(price)
						else:
							self.AppendPriceWithType(price, price_type)
					else:
						self.AppendPrice(price)
							

		else:
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_BUYPRICE % (localeInfo.NumberToMoneyString(addprice)[:-5]), self.SPECIAL_TITLE_COLOR)
	def SetInventoryItem(self, slotIndex, window_type = player.INVENTORY):
	
		if app.ENABLE_DS_SET:
			self.window_type = window_type
	
		itemVnum = player.GetItemIndex(window_type, slotIndex)
		if 0 == itemVnum:
			return

		self.ClearToolTip()
		if shop.IsOpen():
			if not shop.IsPrivateShop() or not shop.IsOfflineShop():
				item.SelectItem(itemVnum)
				self.AppendSellingPrice(player.GetISellItemPrice(window_type, slotIndex))

		metinSlot = [player.GetItemMetinSocket(window_type, slotIndex, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]
		attrSlot = [player.GetItemAttribute(window_type, slotIndex, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]

		self.AddItemData(itemVnum, metinSlot, attrSlot, 0, 0, 1, player.GetItemEvolution(window_type, slotIndex), window_type, slotIndex)
		if not player.IsEquipmentSlot(slotIndex):
			##Ctrl + X + SagTik
			itemImage = ui.ImageBox()
			itemImage.SetParent(self)
			itemImage.Show()
			itemImage.LoadImage("d:/ymir work/ozelButtonlar/ctrl.tga")
			itemImage.SetPosition(20, self.toolTipHeight+5)
			self.childrenList.append(itemImage)

			itemImage = ui.ImageBox()
			itemImage.SetParent(self)
			itemImage.Show()
			itemImage.LoadImage("d:/ymir work/ozelButtonlar/x.tga")
			itemImage.SetPosition(75, self.toolTipHeight+5)
			self.childrenList.append(itemImage)

			itemImage = ui.ImageBox()
			itemImage.SetParent(self)
			itemImage.Show()
			itemImage.LoadImage("d:/ymir work/ozelButtonlar/rclick.tga")
			itemImage.SetPosition(104, self.toolTipHeight+5)
			self.childrenList.append(itemImage)

			textLine=ui.TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(66, self.toolTipHeight+7)
			textLine.SetText("+       +         "+localeInfo.HIZLI_SAT)
			self.childrenList.append(textLine)
			textLine.Show()

			self.toolTipHeight += itemImage.GetHeight()
			self.ResizeToolTip()

	if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
		def SetPrivateSearchItem(self, slotIndex):
			import privateShopSearch
			itemVnum = privateShopSearch.GetSearchItemVnum(slotIndex)

			if 0 == itemVnum:
				return

			self.ClearToolTip()
			self.isPrivateSearchItem = True
		
			metinSlot = []
			for i in xrange(player.METIN_SOCKET_MAX_NUM):
				metinSlot.append(privateShopSearch.GetSearchItemMetinSocket(slotIndex, i))
			attrSlot = []
			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
				attrSlot.append(privateShopSearch.GetSearchItemAttribute(slotIndex, i))
	
			evo = privateShopSearch.GetSearchItemEvolution(slotIndex)
			self.AddItemData(itemVnum, metinSlot, attrSlot, 0,0,0,evo)
			
			#if app.ENABLE_CHANGE_LOOK_SYSTEM:
				#self.AppendChangeLookInfoPrivateShopWIndow(slotIndex)
	def SetShopItemBySecondaryCoin(self, slotIndex):
		itemVnum = shop.GetItemID(slotIndex)
		if 0 == itemVnum:
			return

		price = shop.GetItemPrice(slotIndex)
		self.ClearToolTip()
		self.isShopItem = True

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(shop.GetItemMetinSocket(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(shop.GetItemAttribute(slotIndex, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot)
		self.AppendPriceBySecondaryCoin(price)

	def SetShopItemBySecondaryCoin(self, slotIndex):
		itemVnum = shop.GetItemID(slotIndex)
		if 0 == itemVnum:
			return

		price = shop.GetItemPrice(slotIndex)
		self.ClearToolTip()
		self.isShopItem = True

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(shop.GetItemMetinSocket(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(shop.GetItemAttribute(slotIndex, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot)
		self.AppendPriceBySecondaryCoin(price)

	def SetExchangeOwnerItem(self, slotIndex):
		itemVnum = exchange.GetItemVnumFromSelf(slotIndex)
		if 0 == itemVnum:
			return

		self.ClearToolTip()

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(exchange.GetItemMetinSocketFromSelf(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(exchange.GetItemAttributeFromSelf(slotIndex, i))
		self.AddItemData(itemVnum, metinSlot, attrSlot,0,0,0, exchange.GetItemEvolutionFromSelf(slotIndex))

	def SetExchangeTargetItem(self, slotIndex):
		itemVnum = exchange.GetItemVnumFromTarget(slotIndex)
		if 0 == itemVnum:
			return

		self.ClearToolTip()

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(exchange.GetItemMetinSocketFromTarget(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(exchange.GetItemAttributeFromTarget(slotIndex, i))
		self.AddItemData(itemVnum, metinSlot, attrSlot,0,0,0,exchange.GetItemEvolutionFromTarget(slotIndex))

	def SetPrivateShopBuilderItem(self, invenType, invenPos, privateShopSlotIndex):
		itemVnum = player.GetItemIndex(invenType, invenPos)
		if 0 == itemVnum:
			return

		item.SelectItem(itemVnum)
		self.ClearToolTip()
		self.AppendSellingPrice(shop.GetPrivateShopItemPrice(invenType, invenPos))

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(player.GetItemMetinSocket(invenPos, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(player.GetItemAttribute(invenPos, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot)

		price = shop.GetPrivateShopItemPrice(invenType, invenPos)

		if app.ENABLE_CHEQUE_SYSTEM:
			price_cheque = shop.GetPrivateShopItemPriceCheque(invenType, invenPos)
			price_yang = shop.GetPrivateShopItemPrice(invenType, invenPos)
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.CHEQUE_SYSTEM_SELL_PRICE, grp.GenerateColor(255./255, 246./255, 78./255, 1.0))
			if price_cheque > 0:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.NumberToWonString(price_cheque), grp.GenerateColor(0./255, 215./255, 255./255, 1.0))
			if price_yang > 0:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.NumberToGoldString(price), self.GetPriceColor(price))
		else:
			self.AppendTextLine(localeInfo.NumberToGoldString(price), self.GetPriceColor(price))
	def SetSafeBoxItem(self, slotIndex):
		itemVnum = safebox.GetItemID(slotIndex)
		if 0 == itemVnum:
			return

		self.ClearToolTip()
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(safebox.GetItemMetinSocket(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(safebox.GetItemAttribute(slotIndex, i))
		
		self.AddItemData(itemVnum, metinSlot, attrSlot, 0,0, safebox.GetItemFlags(slotIndex), safebox.GetItemEvolution(slotIndex))

	def SetMallItem(self, slotIndex):
		itemVnum = safebox.GetMallItemID(slotIndex)
		if 0 == itemVnum:
			return

		self.ClearToolTip()
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(safebox.GetMallItemMetinSocket(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(safebox.GetMallItemAttribute(slotIndex, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot,0,0,0, safebox.GetMallItemEvolution(slotIndex))

	def SetItemToolTip(self, itemVnum):
		self.ClearToolTip()
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(0)
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append((0, 0))

		self.AddItemData(itemVnum, metinSlot, attrSlot)

	def __AppendAttackSpeedInfo(self, item):
		atkSpd = item.GetValue(0)

		if atkSpd < 80:
			stSpd = localeInfo.TOOLTIP_ITEM_VERY_FAST
		elif atkSpd <= 95:
			stSpd = localeInfo.TOOLTIP_ITEM_FAST
		elif atkSpd <= 105:
			stSpd = localeInfo.TOOLTIP_ITEM_NORMAL
		elif atkSpd <= 120:
			stSpd = localeInfo.TOOLTIP_ITEM_SLOW
		else:
			stSpd = localeInfo.TOOLTIP_ITEM_VERY_SLOW

		self.AppendTextLine(localeInfo.TOOLTIP_ITEM_ATT_SPEED % stSpd, self.NORMAL_COLOR)

	def __AppendAttackGradeInfo(self):
		atkGrade = item.GetValue(1)
		self.AppendTextLine(localeInfo.TOOLTIP_ITEM_ATT_GRADE % atkGrade, self.GetChangeTextLineColor(atkGrade))

	if app.ENABLE_SASH_SYSTEM:
		def CalcSashValue(self, value, abs):
			if not value:
				return 0
			
			valueCalc = int((round(value * abs) / 100) - .5) + int(int((round(value * abs) / 100) - .5) > 0)
			if valueCalc <= 0 and value > 0:
				value = 1
			else:
				value = valueCalc
			
			return value
		
	def __AppendAttackPowerInfo(self, itemAbsChance = 0):
		minPower = item.GetValue(3)
		maxPower = item.GetValue(4)
		addPower = item.GetValue(5)
		
		if app.ENABLE_SASH_SYSTEM:
			if itemAbsChance:
				minPower = self.CalcSashValue(minPower, itemAbsChance)
				maxPower = self.CalcSashValue(maxPower, itemAbsChance)
				addPower = self.CalcSashValue(addPower, itemAbsChance)
		
		if maxPower > minPower:
			self.AppendTextLine(localeInfo.TOOLTIP_ITEM_ATT_POWER % (minPower+addPower, maxPower+addPower)+self.SILAH_EVRIM_VALUE[self.itemEvolution], self.POSITIVE_COLOR)
		else:
			self.AppendTextLine(localeInfo.TOOLTIP_ITEM_ATT_POWER_ONE_ARG % (minPower+addPower)+self.SILAH_EVRIM_VALUE[self.itemEvolution], self.POSITIVE_COLOR)
			
	def __AppendMagicAttackInfo(self, itemAbsChance = 0):
		minMagicAttackPower = item.GetValue(1)
		maxMagicAttackPower = item.GetValue(2)
		addPower = item.GetValue(5)
		
		if app.ENABLE_SASH_SYSTEM:
			if itemAbsChance:
				minMagicAttackPower = self.CalcSashValue(minMagicAttackPower, itemAbsChance)
				maxMagicAttackPower = self.CalcSashValue(maxMagicAttackPower, itemAbsChance)
				addPower = self.CalcSashValue(addPower, itemAbsChance)
		
		if minMagicAttackPower > 0 or maxMagicAttackPower > 0:
			if maxMagicAttackPower > minMagicAttackPower:
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_MAGIC_ATT_POWER % (minMagicAttackPower+addPower, maxMagicAttackPower+addPower)+self.SILAH_EVRIM_VALUE[self.itemEvolution], self.POSITIVE_COLOR)
			else:
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_MAGIC_ATT_POWER_ONE_ARG % (minMagicAttackPower+addPower)+self.SILAH_EVRIM_VALUE[self.itemEvolution], self.POSITIVE_COLOR)
				
	def __AppendMagicDefenceInfo(self, itemAbsChance = 0):
		magicDefencePower = item.GetValue(0)
		
		if app.ENABLE_SASH_SYSTEM:
			if itemAbsChance:
				magicDefencePower = self.CalcSashValue(magicDefencePower, itemAbsChance)
		
		if magicDefencePower > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_ITEM_MAGIC_DEF_POWER % magicDefencePower, self.GetChangeTextLineColor(magicDefencePower))
			
	def __AppendAttributeInformation(self, attrSlot, itemAbsChance = 0):
		if 0 != attrSlot:
			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
				type = attrSlot[i][0]
				value = attrSlot[i][1]
				if 0 == value:
					continue
				
				affectString = self.__GetAffectString(type, value)
				if app.ENABLE_SASH_SYSTEM:
					if item.GetItemType() == item.ITEM_TYPE_COSTUME and item.GetItemSubType() == item.COSTUME_TYPE_SASH and itemAbsChance:
						value = self.CalcSashValue(value, itemAbsChance)
						affectString = self.__GetAffectString(type, value)
				
				if app.ENABLE_AURA_SYSTEM:
					if item.GetItemType() == item.ITEM_TYPE_COSTUME and item.GetItemSubType() == item.COSTUME_TYPE_AURA and itemAbsChance:
						value = self.CalcSashValue(value, itemAbsChance)
						affectString = self.__GetAffectString(type, value)
				
				if affectString:
					affectColor = self.__GetAttributeColor(i, value)
					self.AppendTextLine(affectString, affectColor)

	if app.ENABLE_DS_SET:
		def __AppendDragonSoulAttributeInformation(self, attrSlot, dsType = 0, setGrade = 0):
			if 0 != attrSlot:
				if setGrade != 0:
					setWeightValue = item.GetDSSetWeight(setGrade)
					basicApplyCount = item.GetDSBasicApplyCount(dsType)

					for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
						type = attrSlot[i][0]
						value = attrSlot[i][1]

						if 0 == value:
							continue

						affectString = self.__GetAffectString(type, value)
						if affectString:
							affectColor = self.__GetAttributeColor(i, value)

							setValue = 0
							if i < basicApplyCount:
								setValue = item.GetDSBasicApplyValue(dsType, type)
							else:
								setValue = item.GetDSAdditionalApplyValue(dsType, type)

							if setValue != 0:
								setValue = (setValue * setWeightValue - 1) / 100 + 1
								if affectString.find('%') == -1:
									self.AppendTwoColorTextLine(affectString, affectColor, " (+{})".format(setValue))
								else:
									self.AppendTwoColorTextLine(affectString, affectColor, " (+{}%)".format(setValue))
							else:
								self.AppendTextLine(affectString, affectColor)
				else:
					for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
						type = attrSlot[i][0]
						value = attrSlot[i][1]

						if 0 == value:
							continue

						affectString = self.__GetAffectString(type, value)
						if affectString:
							affectColor = self.__GetAttributeColor(i, value)
							self.AppendTextLine(affectString, affectColor)

	def __GetAttributeColor(self, index, value):
		if value > 0:
			if index >= 5:
				return self.SPECIAL_POSITIVE_COLOR2
			else:
				return self.SPECIAL_POSITIVE_COLOR
		elif value == 0:
			return self.NORMAL_COLOR
		else:
			return self.NEGATIVE_COLOR

	def __IsPolymorphItem(self, itemVnum):
		if itemVnum >= 70103 and itemVnum <= 70106:
			return 1
		return 0

	def __SetPolymorphItemTitle(self, monsterVnum):
		if localeInfo.IsVIETNAM():
			itemName =item.GetItemName()
			itemName+=" "
			itemName+=nonplayer.GetMonsterName(monsterVnum)
		else:
			itemName =nonplayer.GetMonsterName(monsterVnum)
			itemName+=" "
			itemName+=item.GetItemName()
		self.SetTitle(itemName)

	def __SetNormalItemTitle(self):
		if app.ENABLE_SEND_TARGET_INFO:
			if self.isStone:
				itemName = item.GetItemName()
				realName = itemName[:itemName.find("+")]
				self.SetTitle(realName + " +0 - +4")
			else:
				self.SetTitle(self.SILAH_EVRIM_TITLE[self.itemEvolution] % (localeInfo.SILAH_EVRIM_TEXT.get(self.itemEvolution,"") + item.GetItemName()))
		else:
			self.SetTitle(item.GetItemName())


	def __SetSpecialItemTitle(self):
		self.AppendTextLine(self.SILAH_EVRIM_TITLE[self.itemEvolution] % (localeInfo.SILAH_EVRIM_TEXT.get(self.itemEvolution,"") + item.GetItemName()), self.SPECIAL_TITLE_COLOR)

	def __SetItemTitle(self, itemVnum, metinSlot, attrSlot):
		if localeInfo.IsCANADA():
			if 72726 == itemVnum or 72730 == itemVnum:
				self.AppendTextLine(item.GetItemName(), grp.GenerateColor(1.0, 0.7843, 0.0, 1.0))
				return
			
		if self.__IsPolymorphItem(itemVnum):
			self.__SetPolymorphItemTitle(metinSlot[0])
		else:
			if self.__IsAttr(attrSlot):
				self.__SetSpecialItemTitle()
				return

			self.__SetNormalItemTitle()

	def __IsAttr(self, attrSlot):
		if not attrSlot:
			return False

		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			type = attrSlot[i][0]
			if 0 != type:
				return True

		return False
	
	def AddRefineItemData(self, itemVnum, metinSlot, attrSlot = 0):
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlotData=metinSlot[i]
			if self.GetMetinItemIndex(metinSlotData) == constInfo.ERROR_METIN_STONE:
				metinSlot[i]=player.METIN_SOCKET_TYPE_SILVER

		self.AddItemData(itemVnum, metinSlot, attrSlot, 0, 0, 0)

	def AddItemData_Offline(self, itemVnum, itemDesc, itemSummary, metinSlot, attrSlot):
		self.__AdjustMaxWidth(attrSlot, itemDesc)
		self.__SetItemTitle(itemVnum, metinSlot, attrSlot)
		
		if self.__IsHair(itemVnum):	
			self.__AppendHairIcon(itemVnum)

		### Description ###
		self.AppendDescription(itemDesc, 26)
		self.AppendDescription(itemSummary, 26, self.CONDITION_COLOR)
		self.AppendDescription(itemRed, 26, self.RED_COLOR)
		self.AppendDescription(itemBlue, 26, self.BLUE_COLOR)
		self.AppendDescription(itemGreen, 26, self.GREEN_COLOR)
		self.AppendDescription(itemYellow, 26, self.YELLOW_COLOR)
		self.AppendDescription(itemOrange, 26, self.ORANGE_COLOR)
		self.AppendDescription(itemPink, 26, self.PINK_COLOR)
		self.AppendDescription(itemPurple, 26, self.PURPLE_COLOR)

	if app.ENABLE_RENDER_TARGET_SYSTEM:
		def __AppendModel(self, race, armor = 0, weapon = 0, hair = 0, sash = 0, motion = chr.MOTION_MODE_GENERAL):
			import cfg
			if cfg.Get(cfg.SAVE_OPTION, "RENDER_TARGET", "0") == "1":
				self.ModelWindowBoard.Hide()
				self.ModelWindow.Hide()
				self.ModelWindowBoard = None
				self.ModelWindow = None
				return

			if self.CacheData != (race, armor, weapon, hair, sash, motion):
				self.ModelWindow.SetRenderTargetData(race, armor, weapon, hair, sash, motion)
				self.CacheData = (race, armor, weapon, hair, sash, motion)

			if not self.ModelWindow.IsShow():
				self.ModelWindow.Show()

		def __DeleteModel(self):
			if self.ModelWindowBoard:
				self.ModelWindowBoard.Hide()
				self.ModelWindow.Hide()
				self.ModelWindowBoard = None
				self.ModelWindow = None

			self.ResizeToolTip()
			self.CacheData = (0, 0, 0, 0, 0, chr.MOTION_MODE_GENERAL)

		def __CreateModelWindow(self):
			self.ModelWindowBoard = ui.Board()
			self.ModelWindowBoard.SetParent(self)
			self.ModelWindowBoard.SetSize(205, 240)
			self.ModelWindowBoard.SetPosition(-202, 0)
			self.ModelWindowBoard.Show()
			self.ModelWindow = ui.RenderTarget()
			self.ModelWindow.SetParent(self.ModelWindowBoard)
			self.ModelWindow.SetSize(190, 226)
			self.ModelWindow.SetPosition(7, 7)
			self.ModelWindow.SetRenderTarget(3)
			self.ModelWindow.SetAutoRotation(1.0)
			self.ModelWindow.Show()
			self.childrenList.append(self.ModelWindow)
			self.ResizeToolTip()

		def GetRaceS(self, race, flag):
			if race < 8 and flag != self.GetS(race):
				if race < 4:
					return race + 4
				else:
					return race - 4
			else:
				return race

		def GetS(self, race):
			if race == 0 or race == 2 or race == 5 or race == 7 or race == 8:
				return 0
			else:
				return 1

	if app.__ENABLE_NEW_OFFLINESHOP__:
		def AddRightClickForSale(self):
			self.AppendTextLine(localeInfo.OFFLINESHOP_TOOLTIP_RIGHT_CLICK_FOR_SALE, uinewofflineshop.COLOR_TEXT_SHORTCUT)

	def check_sigillo(self, item_vnum):
		for x in range(55701,55711):
			if x == item_vnum:
				return TRUE
		if item_vnum == 55801:
			return TRUE
		return FALSE

	def AddItemData(self, itemVnum, metinSlot, attrSlot = 0, flags = 0, unbindTime = 0, preview = 1, evolution = 0, window_type = player.INVENTORY, slotIndex = -1, transmutation = -1):
		self.itemVnum = itemVnum
		item.SelectItem(itemVnum)
		itemType = item.GetItemType()
		self.itemEvolution = evolution
		itemSubType = item.GetItemSubType()

		if chr.IsGameMaster(player.GetMainCharacterIndex()):
			self.AppendTextLine("Vnum: "+str(itemVnum)+ " Type: "+str(itemType) + " SubType: "+str(itemSubType), self.CONDITION_COLOR)

		
		if 50026 == itemVnum:
			if 0 != metinSlot:
				name = item.GetItemName()
				if metinSlot[0] > 0:
					name += " "
					name += localeInfo.NumberToMoneyString(metinSlot[0])
				self.SetTitle(name)

				self.ShowToolTip()
				if app.ENABLE_RENDER_TARGET_SYSTEM:
					if self.CacheData != (0, 0, 0, 0, 0, chr.MOTION_MODE_GENERAL):
						self.__DeleteModel()
			return
		### Skill Book ###
		if app.ENABLE_SEND_TARGET_INFO:
			if 50300 == itemVnum and not self.isBook:
				if 0 != metinSlot and not self.isBook:
					self.__SetSkillBookToolTip(metinSlot[0], localeInfo.TOOLTIP_SKILLBOOK_NAME, 1)
					self.ShowToolTip()
					if app.ENABLE_RENDER_TARGET_SYSTEM:
						if self.CacheData != (0, 0, 0, 0, 0, chr.MOTION_MODE_GENERAL):
							self.__DeleteModel()
				elif self.isBook:
					self.SetTitle(item.GetItemName())
					self.AppendDescription(item.GetItemDescription(), 26)
					self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
					self.ShowToolTip()
					if app.ENABLE_RENDER_TARGET_SYSTEM:
						if self.CacheData != (0, 0, 0, 0, 0, chr.MOTION_MODE_GENERAL):
							self.__DeleteModel()
				return
			elif 70037 == itemVnum :
				if 0 != metinSlot and not self.isBook2:
					self.__SetSkillBookToolTip(metinSlot[0], localeInfo.TOOLTIP_SKILL_FORGET_BOOK_NAME, 0)
					self.AppendDescription(item.GetItemDescription(), 26)
					self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
					self.ShowToolTip()
					if app.ENABLE_RENDER_TARGET_SYSTEM:
						if self.CacheData != (0, 0, 0, 0, 0, chr.MOTION_MODE_GENERAL):
							self.__DeleteModel()
				elif self.isBook2:
					self.SetTitle(item.GetItemName())
					self.AppendDescription(item.GetItemDescription(), 26)
					self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
					self.ShowToolTip()
					if app.ENABLE_RENDER_TARGET_SYSTEM:
						if self.CacheData != (0, 0, 0, 0, 0, chr.MOTION_MODE_GENERAL):
							self.__DeleteModel()
				return
			elif 70055 == itemVnum:
				if 0 != metinSlot:
					self.__SetSkillBookToolTip(metinSlot[0], localeInfo.TOOLTIP_SKILL_FORGET_BOOK_NAME, 0)
					self.AppendDescription(item.GetItemDescription(), 26)
					self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
					self.ShowToolTip()
					if app.ENABLE_RENDER_TARGET_SYSTEM:
						if self.CacheData != (0, 0, 0, 0, 0, chr.MOTION_MODE_GENERAL):
							self.__DeleteModel()
				return
			if app.LWT_BUFF_UPDATE:
				if 92010 == itemVnum or 92011 == itemVnum and not self.isBook:
					if 0 != metinSlot and not self.isBook:
						self.__SetSkillBookToolTip(metinSlot[0], item.GetItemName(), 0)
						self.AppendDescription(item.GetItemDescription(), 26)
						self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
						self.ShowToolTip()
		else:
			if 50300 == itemVnum:
				if 0 != metinSlot:
					self.__SetSkillBookToolTip(metinSlot[0], localeInfo.TOOLTIP_SKILLBOOK_NAME, 1)
					self.ShowToolTip()
					if app.ENABLE_RENDER_TARGET_SYSTEM:
						if self.CacheData != (0, 0, 0, 0, 0, chr.MOTION_MODE_GENERAL):
							self.__DeleteModel()
				return
			elif 70037 == itemVnum:
				if 0 != metinSlot:
					self.__SetSkillBookToolTip(metinSlot[0], localeInfo.TOOLTIP_SKILL_FORGET_BOOK_NAME, 0)
					self.AppendDescription(item.GetItemDescription(), 26)
					self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
					self.ShowToolTip()
					if app.ENABLE_RENDER_TARGET_SYSTEM:
						if self.CacheData != (0, 0, 0, 0, 0, chr.MOTION_MODE_GENERAL):
							self.__DeleteModel()
				return
			elif 70055 == itemVnum:
				if 0 != metinSlot:
					self.__SetSkillBookToolTip(metinSlot[0], localeInfo.TOOLTIP_SKILL_FORGET_BOOK_NAME, 0)
					self.AppendDescription(item.GetItemDescription(), 26)
					self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
					self.ShowToolTip()
					if app.ENABLE_RENDER_TARGET_SYSTEM:
						if self.CacheData != (0, 0, 0, 0, 0, chr.MOTION_MODE_GENERAL):
							self.__DeleteModel()
				return
		###########################################################################################


		itemDesc = item.GetItemDescription()
		itemSummary = item.GetItemSummary()
		itemRed = item.GetItemRed()
		itemBlue = item.GetItemBlue()
		itemGreen = item.GetItemGreen()
		itemYellow = item.GetItemYellow()
		itemOrange = item.GetItemOrange()
		itemPink = item.GetItemPink()
		itemPurple = item.GetItemPurple()

		isCostumeItem = 0
		isCostumeHair = 0
		isCostumeBody = 0
		if app.ENABLE_SASH_SYSTEM:
			isCostumeSash = 0
		if app.ENABLE_AURA_SYSTEM:
			isCostumeAura = 0
		if app.ENABLE_COSTUME_WEAPON_SYSTEM:
			isCostumeWeapon = 0
		isCostumeMount = 0
		isCostumeAura = 0
		if app.ENABLE_COSTUME_SYSTEM:
			if item.ITEM_TYPE_COSTUME == itemType:
				isCostumeItem = 1
				isCostumeHair = item.COSTUME_TYPE_HAIR == itemSubType
				isCostumeBody = item.COSTUME_TYPE_BODY == itemSubType
				if app.ENABLE_SASH_SYSTEM:
					isCostumeSash = itemSubType == item.COSTUME_TYPE_SASH
				if app.ENABLE_AURA_SYSTEM:
					isCostumeAura = item.COSTUME_TYPE_AURA == itemSubType
				if app.ENABLE_COSTUME_WEAPON_SYSTEM:
					isCostumeWeapon = item.COSTUME_TYPE_WEAPON == itemSubType
				isCostumeMount = item.COSTUME_TYPE_MOUNT == itemSubType
				isCostumeAura = item.COSTUME_TYPE_AURA == itemSubType
				#dbg.TraceError("IS_COSTUME_ITEM! body(%d) hair(%d)" % (isCostumeBody, isCostumeHair))

		self.__AdjustMaxWidth(attrSlot, itemDesc)
		self.__SetItemTitle(itemVnum, metinSlot, attrSlot)

		if app.ENABLE_RENDER_TARGET_SYSTEM:
			if preview != 0:
				if self.CacheData != (0, 0, 0, 0, 0, chr.MOTION_MODE_GENERAL):
					self.__DeleteModel()

				if constInfo.IsSwitchBotActive == 0:
					if not self.ModelWindow:
						self.__CreateModelWindow()

					if item.ITEM_TYPE_COSTUME == itemType and item.COSTUME_TYPE_MOUNT == itemSubType:
						if metinSlot[3] == 0:
							self.__AppendModel(item.GetValue(0))
						else:
							self.__AppendModel(metinSlot[3])
					elif item.ITEM_TYPE_UNIQUE == itemType and itemSubType == item.USE_PET:
						if metinSlot[3] == 0:
							self.__AppendModel(item.GetValue(0))
						else:
							self.__AppendModel(metinSlot[3])
					elif item.ITEM_TYPE_USE == itemType and item.USE_COSTUME_MOUNT_SKIN == itemSubType:
						self.__AppendModel(item.GetValue(0))
					elif self.check_sigillo(itemVnum):
						petlv = metinSlot[1]
						renderlanacak = 0
						if petlv >= 80:
							renderlanacak = constInfo.petnum[itemVnum][1]
						else:
							renderlanacak = constInfo.petnum[itemVnum][0]
						self.__AppendModel(renderlanacak)
					elif itemType == item.ITEM_TYPE_WEAPON:
						if itemSubType == item.WEAPON_SWORD:
							if not item.IsAntiFlag(item.ANTIFLAG_SURA) and item.IsAntiFlag(item.ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ANTIFLAG_WARRIOR):
								self.__AppendModel(2, 0, itemVnum, 0, 0, chr.MOTION_MODE_ONEHAND_SWORD)
							elif item.IsAntiFlag(item.ANTIFLAG_SURA) and item.IsAntiFlag(item.ANTIFLAG_ASSASSIN) and not item.IsAntiFlag(item.ANTIFLAG_WARRIOR):
								self.__AppendModel(0, 0, itemVnum, 0, 0, chr.MOTION_MODE_ONEHAND_SWORD)
							else:
								self.__AppendModel(player.GetRace(), 0, itemVnum, 0, 0, chr.MOTION_MODE_ONEHAND_SWORD)
						elif itemSubType == item.WEAPON_BOW:
							self.__AppendModel(5, 0, itemVnum, 0, 0, chr.MOTION_MODE_BOW)
						elif itemSubType == item.WEAPON_DAGGER:
							self.__AppendModel(5, 0, itemVnum, 0, 0, chr.MOTION_MODE_DUALHAND_SWORD)
						elif itemSubType == item.WEAPON_TWO_HANDED:
							self.__AppendModel(0, 0, itemVnum, 0, 0, chr.MOTION_MODE_TWOHAND_SWORD)
						elif itemSubType == item.WEAPON_FAN:
							self.__AppendModel(7, 0, itemVnum, 0, 0, chr.MOTION_MODE_FAN)
						elif itemSubType == item.WEAPON_BELL:
							self.__AppendModel(7, 0, itemVnum, 0, 0, chr.MOTION_MODE_BELL)
						elif itemSubType == item.WEAPON_CLAW:
							self.__AppendModel(8, 0, itemVnum, 0, 0, chr.MOTION_MODE_CLAW)
						else:
							self.__DeleteModel()
					elif (itemType == item.ITEM_TYPE_ARMOR and itemSubType == item.ARMOR_BODY):
						if not item.IsAntiFlag(item.ANTIFLAG_SURA):
							self.__AppendModel(2, itemVnum, 0, 0)
						elif not item.IsAntiFlag(item.ANTIFLAG_ASSASSIN):
							self.__AppendModel(5, itemVnum, 0, 0)
						elif not item.IsAntiFlag(item.ANTIFLAG_SHAMAN):
							self.__AppendModel(7, itemVnum, 0, 0)
						elif not item.IsAntiFlag(item.ANTIFLAG_WOLFMAN):
							self.__AppendModel(8, itemVnum, 0, 0)
						elif not item.IsAntiFlag(item.ANTIFLAG_WARRIOR):
							self.__AppendModel(0, itemVnum, 0, 0)
						else:
							self.__DeleteModel()
					elif item.ITEM_TYPE_COSTUME == itemType:
						if isCostumeWeapon:
							if item.GetValue(3) == item.WEAPON_SWORD:
								if not item.IsAntiFlag(item.ANTIFLAG_SURA) and item.IsAntiFlag(item.ANTIFLAG_ASSASSIN) and item.IsAntiFlag(item.ANTIFLAG_WARRIOR):
									self.__AppendModel(2, 0, itemVnum, 0, 0, chr.MOTION_MODE_ONEHAND_SWORD)
								elif item.IsAntiFlag(item.ANTIFLAG_SURA) and item.IsAntiFlag(item.ANTIFLAG_ASSASSIN) and not item.IsAntiFlag(item.ANTIFLAG_WARRIOR):
									self.__AppendModel(0, 0, itemVnum, 0, 0, chr.MOTION_MODE_ONEHAND_SWORD)
								else:
									self.__AppendModel(player.GetRace(), 0, itemVnum, 0, 0, chr.MOTION_MODE_ONEHAND_SWORD)
							elif item.GetValue(3) == item.WEAPON_BOW:
								self.__AppendModel(5, 0, itemVnum, 0, 0, chr.MOTION_MODE_BOW)
							elif item.GetValue(3) == item.WEAPON_DAGGER:
								self.__AppendModel(5, 0, itemVnum, 0, 0, chr.MOTION_MODE_DUALHAND_SWORD)
							elif item.GetValue(3) == item.WEAPON_TWO_HANDED:
								self.__AppendModel(0, 0, itemVnum, 0, 0, chr.MOTION_MODE_TWOHAND_SWORD)
							elif item.GetValue(3) == item.WEAPON_FAN:
								self.__AppendModel(7, 0, itemVnum, 0, 0, chr.MOTION_MODE_FAN)
							elif item.GetValue(3) == item.WEAPON_BELL:
								self.__AppendModel(7, 0, itemVnum, 0, 0, chr.MOTION_MODE_BELL)
							elif item.GetValue(3) == item.WEAPON_CLAW:
								self.__AppendModel(8, 0, itemVnum, 0, 0, chr.MOTION_MODE_CLAW)
							else:
								self.__DeleteModel()
						elif isCostumeBody:
							self.__AppendModel(self.GetRaceS(player.GetRace(), item.IsAntiFlag(item.ANTIFLAG_MALE)), itemVnum, 0, 0)
						elif isCostumeHair:
							self.__AppendModel(self.GetRaceS(player.GetRace(), item.IsAntiFlag(item.ANTIFLAG_MALE)), 0, 0, item.GetValue(3))
						elif isCostumeSash:
							self.__AppendModel(player.GetRace(), 0, 0, 0, itemVnum - 85000)
						else:
							self.__DeleteModel()
					else:
						self.__DeleteModel()

		### Hair Preview Image ###
		if self.__IsHair(itemVnum):
			self.__AppendHairIcon(itemVnum)

		if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
			if self.isPrivateSearchItem:
				if not self.__IsHair(itemVnum):
					self.__AppendPrivateSearchItemicon(itemVnum)
		### Description ###
		self.AppendDescription(itemDesc, 26)
		self.AppendDescription(itemSummary, 26, self.CONDITION_COLOR)
		self.AppendDescription(itemRed, 26, self.RED_COLOR)
		self.AppendDescription(itemBlue, 26, self.BLUE_COLOR)
		self.AppendDescription(itemGreen, 26, self.GREEN_COLOR)
		self.AppendDescription(itemYellow, 26, self.YELLOW_COLOR)
		self.AppendDescription(itemOrange, 26, self.ORANGE_COLOR)
		self.AppendDescription(itemPink, 26, self.PINK_COLOR)
		self.AppendDescription(itemPurple, 26, self.PURPLE_COLOR)

		if app.ENABLE_NEW_PET_SYSTEM:
			if self.check_sigillo(itemVnum) or itemVnum == 55002:
				if attrSlot[0][1] != 0:
					if metinSlot != 0:
						if itemVnum != 55002:
							self.AppendSpace(2)
						petevobl = (int(attrSlot[6][1]))
						petagedur = (app.GetGlobalTimeStamp()-int(metinSlot[3]))/86400
						petagedurtmp = abs(petagedur)
						if itemVnum == 55002:
							self.AppendTextLine(localeInfo.GetNamePetTooltip(int(metinSlot[0])), self.NORMAL_COLOR)
						self.AppendTextLine(localeInfo.PET_SYSTEM_ZILE_TOOLTIP % (metinSlot[1],petagedurtmp), self.SPECIAL_POSITIVE_COLOR)
						self.AppendSpace(2)
						getskillslot = (int(attrSlot[13][1]))
						if (str(petevobl)) == "0":
							self.AppendTextLine(localeInfo.PET_SYSTEM_EVO1 % (getskillslot), self.ITEM_BUFF_USAGE_COLOR)
						elif (str(petevobl)) == "1":
							self.AppendTextLine(localeInfo.PET_SYSTEM_EVO2 % (getskillslot), self.ITEM_BUFF_USAGE_COLOR)
						elif (str(petevobl)) == "2":
							self.AppendTextLine(localeInfo.PET_SYSTEM_EVO3 % (getskillslot), self.ITEM_BUFF_USAGE_COLOR)
						elif (str(petevobl)) == "3":
							self.AppendTextLine(localeInfo.PET_SYSTEM_EVO4 % (getskillslot), self.ITEM_BUFF_USAGE_COLOR)
						self.AppendSpace(2)
						self.AppendTextLine("HP: +"+pointop(str(attrSlot[0][1]))+"%", self.SPECIAL_POSITIVE_COLOR)
						self.AppendTextLine("Savunma: +"+pointop(str(attrSlot[1][1]))+"%", self.SPECIAL_POSITIVE_COLOR)
						self.AppendTextLine("Atak: +"+pointop(str(attrSlot[2][1]))+"%", self.SPECIAL_POSITIVE_COLOR)
						self.AppendSpace(2)
						self.AppendTextLine(petsinfo.typepet % (str(petsinfo.pettype[int(attrSlot[14][1])]), int(attrSlot[14][1]) + 1), self.ITEM_BUFF_RATE_COLOR)
						# self.AppendTextLine("Art詰 Oran?+"+pointop(str(attrSlot[5][1])), self.ITEM_BUFF_RATE_COLOR)
						self.AppendSpace(5)
						if itemVnum != 55002:
							if int(petevobl) >= 3 and metinSlot[1] >= 81:
								if attrSlot[7][1] > 0 or attrSlot[9][1] > 0 or attrSlot[11][1] > 0:
									self.AppendTextLine("Aktif Beceriler", self.ITEM_BUFF_TYPE_COLOR)
								if attrSlot[7][1] > 0:
									self.AppendTextLine(petsinfo.skilpet % (str(petsinfo.petskill[int(attrSlot[7][1])]), int(attrSlot[8][1])), self.ITEM_BUFF_LEVEL_COLOR)
								if attrSlot[9][1] > 0:
									self.AppendTextLine(petsinfo.skilpet % (str(petsinfo.petskill[int(attrSlot[9][1])]), int(attrSlot[10][1])), self.ITEM_BUFF_LEVEL_COLOR)
								if attrSlot[11][1] > 0:
									self.AppendTextLine(petsinfo.skilpet % (str(petsinfo.petskill[int(attrSlot[11][1])]), int(attrSlot[12][1])), self.ITEM_BUFF_LEVEL_COLOR)
							self.AppendSpace(5)
							petdur = app.GetGlobalTimeStamp()-(int(metinSlot[2]))
							tmppetdur = abs(petdur)
							days = (int(petdur) / 86400)
							hours = (int(petdur) - (days * 86400)) / 3600
							mins = (int(petdur) - (days * 86400) - (hours * 3600)) / 60
							if int(metinSlot[2]) <= app.GetGlobalTimeStamp():
								self.AppendTextLine(localeInfo.PET_SYSTEM_NO_MORE_TIME, self.NEGATIVE_COLOR)
							else:
								self.AppendTextLine(localeInfo.PET_SYSTEM_LEFT_TIME + localeInfo.SecondToDHM(int(tmppetdur)), self.ITEM_BUFF_DURATION_COLOR)
						if itemVnum == 55002:
							if int(petevobl) >= 3 and metinSlot[1] >= 81:
								if attrSlot[7][1] == 0:
									self.AppendTextLine(localeInfo.PET_SYSTEM_CAN_OPEN_SKILL % (1), self.NORMAL_COLOR)
								elif attrSlot[7][1] == -1:
									self.AppendTextLine(localeInfo.PET_SYSTEM_CAN_NOT_OPEN_SKILL % (1), self.NEGATIVE_COLOR)
								elif attrSlot[7][1] > 0:
									self.AppendTextLine(petsinfo.skilpet % (str(petsinfo.petskill[int(attrSlot[7][1])]), int(attrSlot[8][1])), self.SPECIAL_POSITIVE_COLOR)
								if attrSlot[9][1] == 0:
									self.AppendTextLine(localeInfo.PET_SYSTEM_CAN_OPEN_SKILL % (2), self.NORMAL_COLOR)
								elif attrSlot[9][1] == -1:
									self.AppendTextLine(localeInfo.PET_SYSTEM_CAN_NOT_OPEN_SKILL % (2), self.NEGATIVE_COLOR)
								elif attrSlot[9][1] > 0:
									self.AppendTextLine(petsinfo.skilpet % (str(petsinfo.petskill[int(attrSlot[9][1])]), int(attrSlot[10][1])), self.SPECIAL_POSITIVE_COLOR)
								if attrSlot[11][1] == 0:
									self.AppendTextLine(localeInfo.PET_SYSTEM_CAN_OPEN_SKILL % (3), self.NORMAL_COLOR)
								elif attrSlot[11][1] == -1:
									self.AppendTextLine(localeInfo.PET_SYSTEM_CAN_NOT_OPEN_SKILL % (3), self.NEGATIVE_COLOR)
								elif attrSlot[11][1] > 0:
									self.AppendTextLine(petsinfo.skilpet % (str(petsinfo.petskill[int(attrSlot[11][1])]), int(attrSlot[12][1])), self.SPECIAL_POSITIVE_COLOR)
					else:
						if itemVnum != 55002:
							self.AppendTextLine(localeInfo.PET_SYSTEM_NOT_SUMMON_YET, self.NEGATIVE_COLOR)

		### Weapon ###
		if item.ITEM_TYPE_WEAPON == itemType:

			self.__AppendLimitInformation()

			self.AppendSpace(5)

			## 부채일 경우 마공을 먼저 표시한다.
			if item.WEAPON_FAN == itemSubType:
				self.__AppendMagicAttackInfo()
				self.__AppendAttackPowerInfo()

			else:
				self.__AppendAttackPowerInfo()
				self.__AppendMagicAttackInfo()

			self.__AppendAffectInformation()
			self.__AppendAttributeInformation(attrSlot)
			if app.ENABLE_CHANGELOOK_SYSTEM:
				self.AppendTransmutation(window_type, slotIndex, transmutation)

			self.AppendWearableInformation()
			if app.ENABLE_NEW_ARROW_SYSTEM:
				if itemSubType != item.WEAPON_UNLIMITED_ARROW:
					self.__AppendMetinSlotInfo(metinSlot)
				else:
					bHasRealtimeFlag = 0
					for i in xrange(item.LIMIT_MAX_NUM):
						(limitType, limitValue) = item.GetLimit(i)
						if item.LIMIT_REAL_TIME == limitType:
							bHasRealtimeFlag = 1
					
					if bHasRealtimeFlag == 1:
						self.AppendMallItemLastTime(metinSlot[0])
			else:
				self.__AppendMetinSlotInfo(metinSlot)


		### Armor ###
		elif item.ITEM_TYPE_ARMOR == itemType:
			self.__AppendLimitInformation()

			## 방어력
			defGrade = item.GetValue(1)
			defBonus = item.GetValue(5)*2 ## 방어력 표시 잘못 되는 문제를 수정
			if defGrade > 0:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_DEF_GRADE % (defGrade+defBonus), self.GetChangeTextLineColor(defGrade))

			self.__AppendMagicDefenceInfo()
			self.__AppendAffectInformation()
			self.__AppendAttributeInformation(attrSlot)
			if app.ENABLE_CHANGELOOK_SYSTEM:
				self.AppendTransmutation(window_type, slotIndex, transmutation)
			if app.ITEM_SET_BONUS:
				if itemSubType in (item.ARMOR_WRIST, item.ARMOR_NECK, item.ARMOR_EAR):
					if item.GetValue(1) > 0:
						self.AppendSpace(5)
						self.AppendTextLine(localeInfo.itemsetbonusdesc1+self.__GetItemSet(item.GetValue(1),2), self.CONDITION_COLOR)
						self.AppendTextLine(localeInfo.itemsetbonusdesc2+self.__GetItemSet(item.GetValue(1),3), self.CONDITION_COLOR)
						self.AppendSpace(5)
			self.AppendWearableInformation()

			if itemSubType in (item.ARMOR_WRIST, item.ARMOR_NECK, item.ARMOR_EAR):
				if metinSlot[4] == 999:
					self.__AppendAccessoryMetinSlotInfo(metinSlot, constInfo.GET_ACCESSORY_MATERIAL_VNUM2(itemVnum, itemSubType))
				else:
					self.__AppendAccessoryMetinSlotInfo(metinSlot, constInfo.GET_ACCESSORY_MATERIAL_VNUM(itemVnum, itemSubType))
			else:
				self.__AppendMetinSlotInfo(metinSlot)

		### Ring Slot Item (Not UNIQUE) ###
		elif item.ITEM_TYPE_RING == itemType:
			self.__AppendLimitInformation()
			self.__AppendAffectInformation()
			self.__AppendAttributeInformation(attrSlot)

			#반지 소켓 시스템 관련해선 아직 기획 미정
			#self.__AppendAccessoryMetinSlotInfo(metinSlot, 99001)
			

		### Belt Item ###
		elif item.ITEM_TYPE_BELT == itemType:
			self.__AppendLimitInformation()
			self.__AppendAffectInformation()
			self.__AppendAttributeInformation(attrSlot)
			if metinSlot[4] == 999:
				self.__AppendAccessoryMetinSlotInfo(metinSlot, constInfo.GET_BELT_MATERIAL_VNUM2(itemVnum))
			else:
				self.__AppendAccessoryMetinSlotInfo(metinSlot, constInfo.GET_BELT_MATERIAL_VNUM(itemVnum))




		## 코스츔 아이템 ##
		elif 0 != isCostumeItem:
			self.__AppendLimitInformation()
			if app.ENABLE_SASH_SYSTEM:
				if isCostumeSash:
					## ABSORPTION RATE
					absChance = int(metinSlot[sash.ABSORPTION_SOCKET])
					self.AppendTextLine(localeInfo.SASH_ABSORB_CHANCE % (absChance), self.CONDITION_COLOR)
					## END ABSOPRTION RATE
					
					itemAbsorbedVnum = int(metinSlot[sash.ABSORBED_SOCKET])
					if itemAbsorbedVnum:
						## ATTACK / DEFENCE
						item.SelectItem(itemAbsorbedVnum)
						if item.GetItemType() == item.ITEM_TYPE_WEAPON:
							if item.GetItemSubType() == item.WEAPON_FAN:
								self.__AppendMagicAttackInfo(metinSlot[sash.ABSORPTION_SOCKET])
								item.SelectItem(itemAbsorbedVnum)
								self.__AppendAttackPowerInfo(metinSlot[sash.ABSORPTION_SOCKET])
							else:
								self.__AppendAttackPowerInfo(metinSlot[sash.ABSORPTION_SOCKET])
								item.SelectItem(itemAbsorbedVnum)
								self.__AppendMagicAttackInfo(metinSlot[sash.ABSORPTION_SOCKET])
						elif item.GetItemType() == item.ITEM_TYPE_ARMOR:
							defGrade = item.GetValue(1)
							defBonus = item.GetValue(5) * 2
							defGrade = self.CalcSashValue(defGrade, metinSlot[sash.ABSORPTION_SOCKET])
							defBonus = self.CalcSashValue(defBonus, metinSlot[sash.ABSORPTION_SOCKET])
							
							if defGrade > 0:
								self.AppendSpace(5)
								self.AppendTextLine(localeInfo.TOOLTIP_ITEM_DEF_GRADE % (defGrade + defBonus), self.GetChangeTextLineColor(defGrade))
							
							item.SelectItem(itemAbsorbedVnum)
							self.__AppendMagicDefenceInfo(metinSlot[sash.ABSORPTION_SOCKET])
						## END ATTACK / DEFENCE
						
						## EFFECT
						item.SelectItem(itemAbsorbedVnum)
						for i in xrange(item.ITEM_APPLY_MAX_NUM):
							(affectType, affectValue) = item.GetAffect(i)
							affectValue = self.CalcSashValue(affectValue, metinSlot[sash.ABSORPTION_SOCKET])
							affectString = self.__GetAffectString(affectType, affectValue)
							if affectString and affectValue > 0:
								self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))
							
							item.SelectItem(itemAbsorbedVnum)
						# END EFFECT
						
						item.SelectItem(itemVnum)
						## ATTR
						self.__AppendAttributeInformation(attrSlot, metinSlot[sash.ABSORPTION_SOCKET])
						# END ATTR
					else:
						# ATTR
						self.__AppendAttributeInformation(attrSlot)
						# END ATTR
				elif isCostumeAura:
					## ABSORPTION RATE
					absChance = (float(metinSlot[aura.ABSORPTION_SOCKET]) / 10.0)
					self.AppendTextLine(localeInfo.AURA_LEVEL_STEP % (int(metinSlot[aura.LEVEL_SOCKET]), int(metinSlot[aura.LEVEL_SOCKET])), self.CONDITION_COLOR)
					self.AppendTextLine(localeInfo.AURA_DRAIN_PER % (absChance), self.CONDITION_COLOR)
					## END ABSOPRTION RATE
					
					itemAbsorbedVnum = int(metinSlot[aura.ABSORBED_SOCKET])
					if itemAbsorbedVnum:
						## ATTACK / DEFENCE
						item.SelectItem(itemAbsorbedVnum)
						if item.GetItemType() == item.ITEM_TYPE_WEAPON:
							if item.GetItemSubType() == item.WEAPON_FAN:
								self.__AppendMagicAttackInfo(absChance)
								item.SelectItem(itemAbsorbedVnum)
								self.__AppendAttackPowerInfo(absChance)
							else:
								self.__AppendAttackPowerInfo(absChance)
								item.SelectItem(itemAbsorbedVnum)
								self.__AppendMagicAttackInfo(absChance)
						elif item.GetItemType() == item.ITEM_TYPE_ARMOR:
							defGrade = item.GetValue(1)
							defBonus = item.GetValue(5) * 2
							defGrade = self.CalcSashValue(defGrade, absChance)
							defBonus = self.CalcSashValue(defBonus, absChance)
							
							if defGrade > 0:
								self.AppendSpace(5)
								self.AppendTextLine(localeInfo.TOOLTIP_ITEM_DEF_GRADE % (defGrade + defBonus), self.GetChangeTextLineColor(defGrade))
							
							item.SelectItem(itemAbsorbedVnum)
							self.__AppendMagicDefenceInfo(absChance)
						## END ATTACK / DEFENCE
						
						## EFFECT
						item.SelectItem(itemAbsorbedVnum)
						for i in xrange(item.ITEM_APPLY_MAX_NUM):
							(affectType, affectValue) = item.GetAffect(i)
							affectValue = self.CalcSashValue(affectValue, absChance)
							affectString = self.__GetAffectString(affectType, affectValue)
							if affectString and affectValue > 0:
								self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))
							
							item.SelectItem(itemAbsorbedVnum)
						# END EFFECT
						
						item.SelectItem(itemVnum)
						## ATTR
						self.__AppendAttributeInformation(attrSlot, absChance)
						# END ATTR
					else:
						# ATTR
						self.__AppendAttributeInformation(attrSlot)
						# END ATTR
				else:
					self.__AppendAffectInformation()
					self.__AppendAttributeInformation(attrSlot)
			else:
				self.__AppendAffectInformation()
				self.__AppendAttributeInformation(attrSlot)


			if isCostumeMount:
				if metinSlot[3] == 0:
					self.AppendSpace(5)
					self.AppendTextLine("["+str(nonplayer.GetMonsterName(item.GetValue(0)))+"]",self.BINEK_COLOR)
				else:
					self.AppendSpace(5)
					self.AppendTextLine("["+str(nonplayer.GetMonsterName(metinSlot[3]))+"]",self.BINEK_COLOR)

			if app.ENABLE_KOSTUMPARLA:
				if metinSlot != 0 and isCostumeBody != 0:
					if int(metinSlot[1]) > 0:
						self.AppendSpace(5)
						self.AppendTextLine(localeInfo.TOOLTIP_COSTUME_EVO_TITLE, grp.GenerateColor(1.0, 0.7843, 0.0, 1.0))

						if int(metinSlot[1]) == 1:
							self.AppendTextLine(localeInfo.TOOLTIP_COSTUME_EVO_YELLOW, self.NORMAL_COLOR)
						elif int(metinSlot[1]) == 2:
							self.AppendTextLine(localeInfo.TOOLTIP_COSTUME_EVO_WHITE, self.NORMAL_COLOR)
						elif int(metinSlot[1]) == 3:
							self.AppendTextLine(localeInfo.TOOLTIP_COSTUME_EVO_PURPLE, self.NORMAL_COLOR)
						elif int(metinSlot[1]) == 4:
							self.AppendTextLine(localeInfo.TOOLTIP_COSTUME_EVO_RED, self.NORMAL_COLOR)
						elif int(metinSlot[1]) == 5:
							self.AppendTextLine(localeInfo.TOOLTIP_COSTUME_EVO_ORANGE, self.NORMAL_COLOR)
						elif int(metinSlot[1]) == 6:
							self.AppendTextLine(localeInfo.TOOLTIP_COSTUME_EVO_GREEN, self.NORMAL_COLOR)
						elif int(metinSlot[1]) == 7:
							self.AppendTextLine(localeInfo.TOOLTIP_COSTUME_EVO_BLUE, self.NORMAL_COLOR)

			self.AppendWearableInformation()

			bHasRealtimeFlag = 0

			## 사용가능 시간 제한이 있는지 찾아보고
			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)

				if item.LIMIT_REAL_TIME == limitType:
					bHasRealtimeFlag = 1
			
			## 있다면 관련 정보를 표시함. ex) 남은 시간 : 6일 6시간 58분 
			if 1 == bHasRealtimeFlag:
				self.AppendMallItemLastTime(metinSlot[0])
				#dbg.TraceError("1) REAL_TIME flag On ")
				
		## Rod ##
		elif item.ITEM_TYPE_ROD == itemType:

			if 0 != metinSlot:
				curLevel = item.GetValue(0) / 10
				curEXP = metinSlot[0]
				maxEXP = item.GetValue(2)
				self.__AppendLimitInformation()
				self.__AppendRodInformation(curLevel, curEXP, maxEXP)

		## Pick ##
		elif item.ITEM_TYPE_PICK == itemType:

			if 0 != metinSlot:
				curLevel = item.GetValue(0) / 10
				curEXP = metinSlot[0]
				maxEXP = item.GetValue(2)
				self.__AppendLimitInformation()
				self.__AppendPickInformation(curLevel, curEXP, maxEXP)

		## Lottery ##
		elif item.ITEM_TYPE_LOTTERY == itemType:
			if 0 != metinSlot:

				ticketNumber = int(metinSlot[0])
				stepNumber = int(metinSlot[1])

				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_LOTTERY_STEP_NUMBER % (stepNumber), self.NORMAL_COLOR)
				self.AppendTextLine(localeInfo.TOOLTIP_LOTTO_NUMBER % (ticketNumber), self.NORMAL_COLOR);

		### Metin ###
		elif item.ITEM_TYPE_METIN == itemType:
			self.AppendMetinInformation()
			self.AppendMetinWearInformation()

		### Fish ###
		elif item.ITEM_TYPE_FISH == itemType:
			if 0 != metinSlot:
				self.__AppendFishInfo(metinSlot[0])
		
		## item.ITEM_TYPE_BLEND
		elif item.ITEM_TYPE_BLEND == itemType:
			self.__AppendLimitInformation()

			if metinSlot:
				affectType = metinSlot[0]
				affectValue = metinSlot[1]
				time = metinSlot[2]
				self.AppendSpace(5)
				affectText = self.__GetAffectString(affectType, affectValue)

				self.AppendTextLine(affectText, self.NORMAL_COLOR)

				if time > 0:
					minute = (time / 60)
					second = (time % 60)
					timeString = localeInfo.TOOLTIP_POTION_TIME

					if minute > 0:
						timeString += str(minute) + localeInfo.TOOLTIP_POTION_MIN
					if second > 0:
						timeString += " " + str(second) + localeInfo.TOOLTIP_POTION_SEC

					self.AppendTextLine(timeString)
				else:
					self.AppendTextLine(localeInfo.BLEND_POTION_NO_TIME)
			else:
				self.AppendTextLine("BLEND_POTION_NO_INFO")

		elif item.ITEM_TYPE_UNIQUE == itemType:
			self.__AppendLimitInformation()
			self.__AppendAffectInformation()
			self.__AppendAttributeInformation(attrSlot)		
			if 0 != metinSlot:
				bHasRealtimeFlag = 0
				
				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)

					if item.LIMIT_REAL_TIME == limitType:
						bHasRealtimeFlag = 1
				
				if 1 == bHasRealtimeFlag:
					self.AppendMallItemLastTime(metinSlot[0])		
				else:
					time = metinSlot[player.METIN_SOCKET_MAX_NUM-1]

					if 1 == item.GetValue(2): ## 실시간 이용 Flag / 장착 안해도 준다
						self.AppendMallItemLastTime(time)
					else:
						self.AppendUniqueItemLastTime(time)

		### Use ###

		elif item.ITEM_TYPE_USE == itemType:
			if (itemVnum >= 71510 and itemVnum <= 71519) or itemVnum == 71523 or itemVnum == 71524 or itemVnum == 71525 or itemVnum == 71528 or itemVnum == 71529 or itemVnum == 71530:
				affectType = item.GetValue(0)
				affectValue = item.GetValue(1)
				affectText = self.__GetAffectString(affectType, affectValue)
				
				if affectText:
					self.AppendSpace(5)
					self.AppendTextLine(affectText, self.NORMAL_COLOR)
					
			
			self.__AppendLimitInformation()

			if item.USE_POTION == itemSubType or item.USE_POTION_NODELAY == itemSubType:
				self.__AppendPotionInformation()

			elif item.USE_ABILITY_UP == itemSubType:
				self.__AppendAbilityPotionInformation()
			if 71526 == itemVnum or 71527 == itemVnum or 71516 == itemVnum or 71517  == itemVnum or 71518 == itemVnum or 71519 == itemVnum or 71523 == itemVnum or 71524 == itemVnum or 72726 == itemVnum or 72730 == itemVnum or 71510  == itemVnum or 71511  == itemVnum or 71512  == itemVnum or 71513  == itemVnum or 71514  == itemVnum or 71515 == itemVnum or 71525 == itemVnum or 71528  == itemVnum or 71529 == itemVnum or 71530  == itemVnum:
				self.AppendSpace(5)
				self.AppendTextLine("|cFF00FFFF[PERMA]", self.NORMAL_COLOR)

			if item.USE_COSTUME_MOUNT_SKIN == itemSubType:
				if metinSlot[3] == 0:
					self.AppendSpace(5)
					self.AppendTextLine("["+str(nonplayer.GetMonsterName(item.GetValue(0)))+"]",self.BINEK_COLOR)
				else:
					self.AppendSpace(5)
					self.AppendTextLine("["+str(nonplayer.GetMonsterName(metinSlot[3]))+"]",self.BINEK_COLOR)

			## 영석 감지기
			# if 27989 == itemVnum or 76006 == itemVnum:
				# if 0 != metinSlot:
					# useCount = int(metinSlot[0])

					# self.AppendSpace(5)
					# self.AppendTextLine(localeInfo.TOOLTIP_REST_USABLE_COUNT % (6 - useCount), self.NORMAL_COLOR)

			## 이벤트 감지기
			elif 50004 == itemVnum:
				if 0 != metinSlot:
					useCount = int(metinSlot[0])

					self.AppendSpace(5)
					self.AppendTextLine(localeInfo.TOOLTIP_REST_USABLE_COUNT % (10 - useCount), self.NORMAL_COLOR)

			## 자동물약
			elif constInfo.IS_AUTO_POTION(itemVnum):
				if 0 != metinSlot:
					## 0: 활성화, 1: 사용량, 2: 총량
					isActivated = int(metinSlot[0])
					usedAmount = float(metinSlot[1])
					totalAmount = float(metinSlot[2])
					
					if 0 == totalAmount:
						totalAmount = 1
					
					self.AppendSpace(5)

					if 0 != isActivated:
						self.AppendTextLine("(%s)" % (localeInfo.TOOLTIP_AUTO_POTION_USING), self.SPECIAL_POSITIVE_COLOR)
						self.AppendSpace(5)
						
					##self.AppendTextLine(localeInfo.TOOLTIP_AUTO_POTION_REST % (100.0 - ((usedAmount / totalAmount) * 100.0)), self.POSITIVE_COLOR)
								
			## 귀환 기억부
			elif itemVnum in WARP_SCROLLS:
				if 0 != metinSlot:
					xPos = int(metinSlot[0])
					yPos = int(metinSlot[1])

					if xPos != 0 and yPos != 0:
						(mapName, xBase, yBase) = background.GlobalPositionToMapInfo(xPos, yPos)
						
						localeMapName=localeInfo.MINIMAP_ZONE_NAME_DICT.get(mapName, "")

						self.AppendSpace(5)

						if localeMapName!="":						
							self.AppendTextLine(localeInfo.TOOLTIP_MEMORIZED_POSITION % (localeMapName, int(xPos-xBase)/100, int(yPos-yBase)/100), self.NORMAL_COLOR)
						else:
							self.AppendTextLine(localeInfo.TOOLTIP_MEMORIZED_POSITION_ERROR % (int(xPos)/100, int(yPos)/100), self.NORMAL_COLOR)
							dbg.TraceError("NOT_EXIST_IN_MINIMAP_ZONE_NAME_DICT: %s" % mapName)

			#####
			if item.USE_SPECIAL == itemSubType:
				bHasRealtimeFlag = 0
				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)

					if item.LIMIT_REAL_TIME == limitType:
						bHasRealtimeFlag = 1
		
				## 있다면 관련 정보를 표시함. ex) 남은 시간 : 6일 6시간 58분 
				if 1 == bHasRealtimeFlag:
					self.AppendMallItemLastTime(metinSlot[0])
				else:
					# ... 이거... 서버에는 이런 시간 체크 안되어 있는데...
					# 왜 이런게 있는지 알지는 못하나 그냥 두자...
					if 0 != metinSlot:
						time = metinSlot[player.METIN_SOCKET_MAX_NUM-1]

						## 실시간 이용 Flag
						if 1 == item.GetValue(2):
							self.AppendMallItemLastTime(time)
			
			elif item.USE_TIME_CHARGE_PER == itemSubType:
				bHasRealtimeFlag = 0
				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)

					if item.LIMIT_REAL_TIME == limitType:
						bHasRealtimeFlag = 1
				if metinSlot[2]:
					self.AppendTextLine(localeInfo.TOOLTIP_TIME_CHARGER_PER(metinSlot[2]))
				else:
					self.AppendTextLine(localeInfo.TOOLTIP_TIME_CHARGER_PER(item.GetValue(0)))
 		
				## 있다면 관련 정보를 표시함. ex) 남은 시간 : 6일 6시간 58분 
				if 1 == bHasRealtimeFlag:
					self.AppendMallItemLastTime(metinSlot[0])

			elif item.USE_TIME_CHARGE_FIX == itemSubType:
				bHasRealtimeFlag = 0
				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)

					if item.LIMIT_REAL_TIME == limitType:
						bHasRealtimeFlag = 1
				if metinSlot[2]:
					self.AppendTextLine(localeInfo.TOOLTIP_TIME_CHARGER_FIX(metinSlot[2]))
				else:
					self.AppendTextLine(localeInfo.TOOLTIP_TIME_CHARGER_FIX(item.GetValue(0)))
		
				## 있다면 관련 정보를 표시함. ex) 남은 시간 : 6일 6시간 58분 
				if 1 == bHasRealtimeFlag:
					self.AppendMallItemLastTime(metinSlot[0])

		elif item.ITEM_TYPE_QUEST == itemType:
			if itemVnum >= 53001 and itemVnum <= 53065 or itemVnum >= 53218 and itemVnum <= 53244 or itemVnum >= 54545 and itemVnum <= 54700:
				self.AppendSpace(5)
				for g in xrange(item.ITEM_APPLY_MAX_NUM):
					(affectType, affectValue) = item.GetAffect(g)
					affectString = self.__GetAffectString(affectType, affectValue)
					if affectString:
						affectColor = self.GetChangeTextLineColor(affectValue)
						self.AppendTextLine(affectString, affectColor)
			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)

				if item.LIMIT_REAL_TIME == limitType:
					self.AppendMallItemLastTime(metinSlot[0])
					
		elif item.ITEM_TYPE_DS == itemType:
			self.AppendTextLine(self.__DragonSoulInfoString(itemVnum))

			if app.ENABLE_DS_SET:
				if self.window_type == player.EQUIPMENT and self.interface and self.interface.wndDragonSoul:
					self.__AppendDragonSoulAttributeInformation(attrSlot, itemVnum / 10000, self.interface.wndDragonSoul.GetDSSetGrade())
				else:
					self.__AppendDragonSoulAttributeInformation(attrSlot)
			else:
				self.__AppendAttributeInformation(attrSlot)

		for i in xrange(item.LIMIT_MAX_NUM):
			(limitType, limitValue) = item.GetLimit(i)
			#dbg.TraceError("LimitType : %d, limitValue : %d" % (limitType, limitValue))
			
			if item.LIMIT_REAL_TIME_START_FIRST_USE == limitType:
				self.AppendRealTimeStartFirstUseLastTime(item, metinSlot, i)
				#dbg.TraceError("2) REAL_TIME_START_FIRST_USE flag On ")
				
			elif item.LIMIT_TIMER_BASED_ON_WEAR == limitType:
				self.AppendTimerBasedOnWearLastTime(metinSlot)
				#dbg.TraceError("1) REAL_TIME flag On ")


		if item.GetItemType() == item.ITEM_TYPE_GIFTBOX:
			 self.AppendSpace(5)
			 self.__AppendExtraInfo("icon/item/info_ac.png")
			 self.AppendSpace(5)
			 self.__AppendExtraInfo("icon/item/info_bak.tga")
			#self.AppendTextLine(localeInfo.TOOLTIP_CHEST_BOX2, self.NORMAL_COLOR, False);
		
		
		
		self.AppendAntiFlagInformation()
		self.ShowToolTip()

	def __DragonSoulInfoString (self, dwVnum):
		step = (dwVnum / 100) % 10
		refine = (dwVnum / 10) % 10
		if 0 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL1 + " " + localeInfo.DRAGON_SOUL_STRENGTH(refine)
		elif 1 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL2 + " " + localeInfo.DRAGON_SOUL_STRENGTH(refine)
		elif 2 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL3 + " " + localeInfo.DRAGON_SOUL_STRENGTH(refine)
		elif 3 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL4 + " " + localeInfo.DRAGON_SOUL_STRENGTH(refine)
		elif 4 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL5 + " " + localeInfo.DRAGON_SOUL_STRENGTH(refine)
		elif 5 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL6 + " " + localeinfo.DRAGON_SOUL_STRENGTH(refine)
		else:
			return ""


	## 헤어인가?
	def __IsHair(self, itemVnum):
		if app.ENABLE_DS_GRADE_MYTH:
			return (self.__IsOldHair(itemVnum) or
				self.__IsNewHair(itemVnum) or
				self.__IsNewHair2(itemVnum) or
				self.__IsNewHair3(itemVnum))
		else:
			return (self.__IsOldHair(itemVnum) or
				self.__IsNewHair(itemVnum) or
				self.__IsNewHair2(itemVnum) or
				self.__IsNewHair3(itemVnum) or
				self.__IsCostumeHair(itemVnum))

	def __IsOldHair(self, itemVnum):
		return itemVnum > 73000 and itemVnum < 74000	

	def __IsNewHair(self, itemVnum):
		return itemVnum > 74000 and itemVnum < 75000	

	def __IsNewHair2(self, itemVnum):
		return itemVnum > 75000 and itemVnum < 76000	

	def __IsNewHair3(self, itemVnum):
		return ((74012 < itemVnum and itemVnum < 74022) or
			(74262 < itemVnum and itemVnum < 74272) or
			(74512 < itemVnum and itemVnum < 74522) or
			(74521 < itemVnum and itemVnum < 74772) or
			(74750 < itemVnum and itemVnum < 74773) or
			(45000 < itemVnum and itemVnum < 47000))

	if not app.ENABLE_DS_GRADE_MYTH:
		def __IsCostumeHair(self, itemVnum):
			return app.ENABLE_COSTUME_SYSTEM and self.__IsNewHair3(itemVnum)

	if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
		def __AppendPrivateSearchItemicon(self, itemVnum):
			itemImage = ui.ImageBox()
			itemImage.SetParent(self)
			itemImage.Show()
			item.SelectItem(itemVnum)
			itemImage.LoadImage(item.GetIconImageFileName())
			itemImage.SetPosition((self.toolTipWidth/2)-16, self.toolTipHeight)
			self.toolTipHeight += itemImage.GetHeight()
			self.childrenList.append(itemImage)
			self.ResizeToolTip()
	def __AppendHairIcon(self, itemVnum):
		itemImage = ui.ImageBox()
		itemImage.SetParent(self)
		itemImage.Show()			

		if self.__IsOldHair(itemVnum):
			itemImage.LoadImage("d:/ymir work/item/quest/"+str(itemVnum)+".tga")
		elif self.__IsNewHair3(itemVnum):
			itemImage.LoadImage("icon/hair/%d.sub" % (itemVnum))
		elif self.__IsNewHair(itemVnum): # 기존 헤어 번호를 연결시켜서 사용한다. 새로운 아이템은 1000만큼 번호가 늘었다.
			itemImage.LoadImage("d:/ymir work/item/quest/"+str(itemVnum-1000)+".tga")
		elif self.__IsNewHair2(itemVnum):
			itemImage.LoadImage("icon/hair/%d.sub" % (itemVnum))
		elif not app.ENABLE_DS_GRADE_MYTH and self.__IsCostumeHair(itemVnum):
			itemImage.LoadImage("icon/hair/%d.sub" % (itemVnum - 100000))
		if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
			if self.isPrivateSearchItem:
				itemImage.SetPosition((self.toolTipWidth/2)-48, self.toolTipHeight)
			else:
				itemImage.SetPosition((self.toolTipWidth/2)-48, self.toolTipHeight)
		else:
			itemImage.SetPosition(itemImage.GetWidth()/2, self.toolTipHeight)

		self.toolTipHeight += itemImage.GetHeight()
		#self.toolTipWidth += itemImage.GetWidth()/2
		self.childrenList.append(itemImage)
		self.ResizeToolTip()

	## 사이즈가 큰 Description 일 경우 툴팁 사이즈를 조정한다
	def __AdjustMaxWidth(self, attrSlot, desc):
		newToolTipWidth = self.toolTipWidth
		newToolTipWidth = max(self.__AdjustAttrMaxWidth(attrSlot), newToolTipWidth)
		newToolTipWidth = max(self.__AdjustDescMaxWidth(desc), newToolTipWidth)
		if newToolTipWidth > self.toolTipWidth:
			self.toolTipWidth = newToolTipWidth
			self.ResizeToolTip()

	def __AdjustAttrMaxWidth(self, attrSlot):
		if 0 == attrSlot:
			return self.toolTipWidth

		maxWidth = self.toolTipWidth
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			type = attrSlot[i][0]
			value = attrSlot[i][1]
			if self.ATTRIBUTE_NEED_WIDTH.has_key(type):
				if value > 0:
					maxWidth = max(self.ATTRIBUTE_NEED_WIDTH[type], maxWidth)

					# ATTR_CHANGE_TOOLTIP_WIDTH
					#self.toolTipWidth = max(self.ATTRIBUTE_NEED_WIDTH[type], self.toolTipWidth)
					#self.ResizeToolTip()
					# END_OF_ATTR_CHANGE_TOOLTIP_WIDTH

		return maxWidth

	def __AdjustDescMaxWidth(self, desc):
		if len(desc) < DESC_DEFAULT_MAX_COLS:
			return self.toolTipWidth
	
		return DESC_WESTERN_MAX_WIDTH

	def __SetSkillBookToolTip(self, skillIndex, bookName, skillGrade):
		skillName = skill.GetSkillName(skillIndex)

		if not skillName:
			return

		if localeInfo.IsVIETNAM():
			itemName = bookName + " " + skillName
		else:
			itemName = skillName + " " + bookName
		self.SetTitle(itemName)

	def __AppendPickInformation(self, curLevel, curEXP, maxEXP):
		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_PICK_LEVEL % (curLevel), self.NORMAL_COLOR)
		self.AppendTextLine(localeInfo.TOOLTIP_PICK_EXP % (curEXP, maxEXP), self.NORMAL_COLOR)

		if curEXP == maxEXP:
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_PICK_UPGRADE1, self.NORMAL_COLOR)
			self.AppendTextLine(localeInfo.TOOLTIP_PICK_UPGRADE2, self.NORMAL_COLOR)
			self.AppendTextLine(localeInfo.TOOLTIP_PICK_UPGRADE3, self.NORMAL_COLOR)


	def __AppendRodInformation(self, curLevel, curEXP, maxEXP):
		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_FISHINGROD_LEVEL % (curLevel), self.NORMAL_COLOR)
		self.AppendTextLine(localeInfo.TOOLTIP_FISHINGROD_EXP % (curEXP, maxEXP), self.NORMAL_COLOR)

		if curEXP == maxEXP:
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_FISHINGROD_UPGRADE1, self.NORMAL_COLOR)
			self.AppendTextLine(localeInfo.TOOLTIP_FISHINGROD_UPGRADE2, self.NORMAL_COLOR)
			self.AppendTextLine(localeInfo.TOOLTIP_FISHINGROD_UPGRADE3, self.NORMAL_COLOR)

	def __AppendLimitInformation(self):

		appendSpace = False

		for i in xrange(item.LIMIT_MAX_NUM):

			(limitType, limitValue) = item.GetLimit(i)

			if limitValue > 0:
				if False == appendSpace:
					self.AppendSpace(5)
					appendSpace = True

			else:
				continue

			if item.LIMIT_LEVEL == limitType:
				color = self.GetLimitTextLineColor(player.GetStatus(player.LEVEL), limitValue)
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_LIMIT_LEVEL % (limitValue), color)
			"""
			elif item.LIMIT_STR == limitType:
				color = self.GetLimitTextLineColor(player.GetStatus(player.ST), limitValue)
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_LIMIT_STR % (limitValue), color)
			elif item.LIMIT_DEX == limitType:
				color = self.GetLimitTextLineColor(player.GetStatus(player.DX), limitValue)
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_LIMIT_DEX % (limitValue), color)
			elif item.LIMIT_INT == limitType:
				color = self.GetLimitTextLineColor(player.GetStatus(player.IQ), limitValue)
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_LIMIT_INT % (limitValue), color)
			elif item.LIMIT_CON == limitType:
				color = self.GetLimitTextLineColor(player.GetStatus(player.HT), limitValue)
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_LIMIT_CON % (limitValue), color)
			"""












	def __GetAffectString(self, affectType, affectValue):
		if 0 == affectType:
			return None

		if 0 == affectValue:
			return None

		try:
			return self.AFFECT_DICT[affectType](affectValue)
		except TypeError:
			return "UNKNOWN_VALUE[%s] %s" % (affectType, affectValue)
		except KeyError:
			return "UNKNOWN_TYPE[%s] %s" % (affectType, affectValue)

	if app.ITEM_SET_BONUS:
		def __GetItemSet(self, value, index):
			type = str(self.ITEM_SET[value][0])
			value = str(self.ITEM_SET[value][index])
			return type + "" + value
	def __AppendAffectInformation(self):
		for i in xrange(item.ITEM_APPLY_MAX_NUM):

			(affectType, affectValue) = item.GetAffect(i)

			affectString = self.__GetAffectString(affectType, affectValue)
			if affectString:
				self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))

	def AppendWearableInformation(self):

		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_ITEM_WEARABLE_JOB, self.NORMAL_COLOR)

		flagList = (
			not item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR),
			not item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN),
			not item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA),
			not item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN),
			not item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN))


		characterNames = ""
		self.AppendSpace(2)
		for i in xrange(self.CHARACTER_COUNT):

			name = self.CHARACTER_NAMES[i]
			flag = flagList[i]

			if flag:
				characterNames += " "
				characterNames += name
		self.AppendSpace(2)

		textLine = self.AppendTextLine(characterNames, self.NORMAL_COLOR, True)
		textLine.SetFeather()

		if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE):
			textLine = self.AppendTextLine(localeInfo.FOR_FEMALE, self.NORMAL_COLOR, True)
			textLine.SetFeather()

		if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE):
			textLine = self.AppendTextLine(localeInfo.FOR_MALE, self.NORMAL_COLOR, True)
			textLine.SetFeather()

	def __AppendPotionInformation(self):
		self.AppendSpace(5)

		healHP = item.GetValue(0)
		healSP = item.GetValue(1)
		healStatus = item.GetValue(2)
		healPercentageHP = item.GetValue(3)
		healPercentageSP = item.GetValue(4)

		if healHP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_HP_POINT % healHP, self.GetChangeTextLineColor(healHP))
		if healSP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_SP_POINT % healSP, self.GetChangeTextLineColor(healSP))
		if healStatus != 0:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_CURE)
		if healPercentageHP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_HP_PERCENT % healPercentageHP, self.GetChangeTextLineColor(healPercentageHP))
		if healPercentageSP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_SP_PERCENT % healPercentageSP, self.GetChangeTextLineColor(healPercentageSP))


			
	def AppendAntiFlagInformation(self):
		antiFlagDict = {
			"|Eemoji/anti_drop|e"	 : item.ITEM_ANTIFLAG_DROP,    ## ++
			"|Eemoji/anti_sell|e"	 : item.ITEM_ANTIFLAG_SELL,    ## ++
			 #"|Eemoji/anti_give|e"	 : item.ITEM_ANTIFLAG_GIVE,
			 #"|Eemoji/anti_stack|e"	 : item.ITEM_ANTIFLAG_STACK,
			"|Eemoji/anti_shop|e"	 : item.ITEM_ANTIFLAG_MYSHOP,  ## ++
			"|Eemoji/anti_safebox|e" : item.ITEM_ANTIFLAG_SAFEBOX, ## ++
		}
			
		antiFlagNames = [name for name, flag in antiFlagDict.iteritems() if item.IsAntiFlag(flag)]
		if antiFlagNames:
			self.AppendSpace(5)

			textLine1 = self.AppendTextLine(localeInfo.NOT_POSSIBLE, self.DISABLE_COLOR)
			textLine1.SetFeather()
	
			self.AppendSpace(5)

			textLine2 = self.AppendTextLine('{}'.format(' '.join(antiFlagNames)), self.DISABLE_COLOR)
			textLine2.SetFeather()
			
	def __AppendAbilityPotionInformation(self):


		self.AppendSpace(5)

		abilityType = item.GetValue(0)
		time = item.GetValue(1)
		point = item.GetValue(2)

		if abilityType == item.APPLY_ATT_SPEED:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_ATTACK_SPEED % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_MOV_SPEED:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_MOVING_SPEED % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_STR:
			self.AppendTextLine(localeInfo.TOOLTIP_STR_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_DEX:
			self.AppendTextLine(localeInfo.TOOLTIP_DEX_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_CON:
			self.AppendTextLine(localeInfo.TOOLTIP_CON_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_INT:
			self.AppendTextLine(localeInfo.TOOLTIP_INT_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_CAST_SPEED:
			self.AppendTextLine(localeInfo.TOOLTIP_CAST_SPEED_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_RESIST_MAGIC:
			self.AppendTextLine(localeInfo.TOOLTIP_RESIST_MAGIC_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_ATT_GRADE_BONUS:
			self.AppendTextLine(localeInfo.TOOLTIP_ATT_GRADE_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_DEF_GRADE_BONUS:
			self.AppendTextLine(localeInfo.TOOLTIP_DEF_GRADE_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_MAX_HP:
			self.AppendTextLine(localeInfo.TOOLTIP_MAX_HP_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_ATTBONUS_HUMAN:
			self.AppendTextLine(localeInfo.TOOLTIP_APPLY_ATTBONUS_HUMAN_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_ATT_BOSS:
			self.AppendTextLine(localeInfo.TOOLTIP_APPLY_ATT_BOSS_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_ATTBONUS_METIN:
			self.AppendTextLine(localeInfo.TOOLTIP_APPLY_ATT_METIN_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_ATTBONUS_MILGYO:
			self.AppendTextLine(localeInfo.TOOLTIP_APPLY_ATTBONUS_MILGYO_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_ATTBONUS_ORC:
			self.AppendTextLine(localeInfo.TOOLTIP_APPLY_ATTBONUS_ORC_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_ATTBONUS_DEVIL:
			self.AppendTextLine(localeInfo.TOOLTIP_APPLY_ATTBONUS_DEVIL_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_ATTBONUS_UNDEAD:
			self.AppendTextLine(localeInfo.TOOLTIP_APPLY_ATTBONUS_UNDEAD_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_CRITICAL_PCT:
			self.AppendTextLine(localeInfo.TOOLTIP_CRITICAL_PCT_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_PENETRATE_PCT:
			self.AppendTextLine(localeInfo.TOOLTIP_PENETRATE_PCT_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_STEAL_HP:
			self.AppendTextLine(localeInfo.TOOLTIP_APPLY_STEAL_HP_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_BLOCK:
			self.AppendTextLine(localeInfo.TOOLTIP_APPLY_BLOCK_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_POISON_PCT:
			self.AppendTextLine(localeInfo.TOOLTIP_APPLY_POISON_PCT_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_POISON_REDUCE:
			self.AppendTextLine(localeInfo.TOOLTIP_APPLY_POISON_REDUCE_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_EXP_DOUBLE_BONUS:
			self.AppendTextLine(localeInfo.TOOLTIP_APPLY_EXP_DOUBLE_BONUS_FISH % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_ITEM_DROP_BONUS:
			self.AppendTextLine(localeInfo.TOOLTIP_APPLY_ITEM_DROP_BONUS_FISH % point, self.GetChangeTextLineColor(point))

		if time > 0:
			minute = (time / 60)
			second = (time % 60)
			timeString = localeInfo.TOOLTIP_POTION_TIME

			if minute > 0:
				timeString += str(minute) + localeInfo.TOOLTIP_POTION_MIN
			if second > 0:
				timeString += " " + str(second) + localeInfo.TOOLTIP_POTION_SEC

			self.AppendTextLine(timeString)

	def GetPriceColor(self, price):
		if price>=constInfo.HIGH_PRICE:
			return self.HIGH_PRICE_COLOR
		if price>=constInfo.MIDDLE_PRICE:
			return self.MIDDLE_PRICE_COLOR
		else:
			return self.LOW_PRICE_COLOR
						
	def AppendPriceWithType(self, price, price_type):	
		self.AppendSpace(5)
		if price < 1:
			self.AppendTextLine(localeInfo.ucretsiz, self.POSITIVE_COLOR)
		else:
			self.AppendTextLineWithItem(localeInfo.TOOLTIP_BUYPRICE2  % (localeInfo.NumberToItemString(price, price_type)), price_type,self.GetPriceColor(price))
		
	def __AppendExtraInfo(self, file):
		iconFile = str(file)

		itemImage = ui.ImageBox()
		itemImage.SetParent(self)
		itemImage.Show()

		itemImage.LoadImage(iconFile)

		itemImage.SetPosition(itemImage.GetWidth()/ 2 - 50, self.toolTipHeight)
		self.toolTipHeight += itemImage.GetHeight()
		self.childrenList.append(itemImage)
		self.ResizeToolTip()		
		
	def AppendPrice(self, price):	
		self.AppendSpace(5)
		if price > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_BUYPRICE , self.SHOP_ITEM_COLOR)
			self.AppendTextLine((localeInfo.NumberToMoneyString(price)), self.GetPriceColor(price))
		else:
			self.AppendTextLine(localeInfo.NPC_0YANG_UCRETSIZ , self.SHOP_ITEM_COLOR)
            
	def AppendPriceWon(self, price):	
		self.AppendSpace(5)
		if price < 1:
			self.AppendTextLine(localeInfo.NPC_0YANG_UCRETSIZ , self.SHOP_ITEM_COLOR)
		else:
			self.AppendTextLine(localeInfo.TOOLTIP_BUYPRICE  % (localeInfo.NumberToWonString(price)), self.HIGH_PRICE_COLOR)
			
		
	def AppendPriceBySecondaryCoin(self, price):
		self.AppendSpace(5)
		if price > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_BUYPRICE , self.SHOP_ITEM_COLOR) 
			self.AppendTextLine((localeInfo.NumberToMoneyString(price)), self.GetPriceColor(price))
		else:
			self.AppendTextLine(localeInfo.NPC_0YANG_UCRETSIZ , self.SHOP_ITEM_COLOR)	

	def AppendSellingPrice(self, price):
		if item.IsAntiFlag(item.ITEM_ANTIFLAG_SELL):
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_ANTI_SELL, self.DISABLE_COLOR)
		else:
			self.AppendSpace(5)
			if price > 0:
				self.AppendTextLine(localeInfo.TOOLTIP_BUYPRICE , self.SHOP_ITEM_COLOR) 
				self.AppendTextLine((localeInfo.NumberToMoneyString(price)), self.GetPriceColor(price))
			else:
				self.AppendTextLine(localeInfo.NPC_0YANG_UCRETSIZ , self.SHOP_ITEM_COLOR)

	def AppendPriceBySecondaryCoin(self, price):
		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_BUYPRICE  % (localeInfo.NumberToSecondaryCoinString(price)), self.GetPriceColor(price))

	def AppendSellingPrice(self, price):
		if item.IsAntiFlag(item.ITEM_ANTIFLAG_SELL):			
			self.AppendTextLine(localeInfo.TOOLTIP_ANTI_SELL, self.DISABLE_COLOR)
			self.AppendSpace(5)
		else:
			self.AppendTextLine(localeInfo.TOOLTIP_SELLPRICE % (localeInfo.NumberToMoneyString(price)), self.GetPriceColor(price))
			self.AppendSpace(5)

	def AppendMetinInformation(self):
		for i in xrange(item.ITEM_APPLY_MAX_NUM):
			(affectType, affectValue) = item.GetAffect(i)
			affectString = self.__GetAffectString(affectType, affectValue)
			if affectString:
				self.AppendSpace(5)
				self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))

	def AppendMetinWearInformation(self):

		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_SOCKET_REFINABLE_ITEM, self.NORMAL_COLOR)

		flagList = (item.IsWearableFlag(item.WEARABLE_BODY),
					item.IsWearableFlag(item.WEARABLE_HEAD),
					item.IsWearableFlag(item.WEARABLE_FOOTS),
					item.IsWearableFlag(item.WEARABLE_WRIST),
					item.IsWearableFlag(item.WEARABLE_WEAPON),
					item.IsWearableFlag(item.WEARABLE_NECK),
					item.IsWearableFlag(item.WEARABLE_EAR),
					item.IsWearableFlag(item.WEARABLE_UNIQUE),
					item.IsWearableFlag(item.WEARABLE_SHIELD),
					item.IsWearableFlag(item.WEARABLE_ARROW))

		wearNames = ""
		for i in xrange(self.WEAR_COUNT):

			name = self.WEAR_NAMES[i]
			flag = flagList[i]

			if flag:
				wearNames += "  "
				wearNames += name

		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPosition(self.toolTipWidth/2, self.toolTipHeight)
		textLine.SetHorizontalAlignCenter()
		textLine.SetPackedFontColor(self.NORMAL_COLOR)
		textLine.SetText(wearNames)
		textLine.Show()
		self.childrenList.append(textLine)

		self.toolTipHeight += self.TEXT_LINE_HEIGHT
		self.ResizeToolTip()
		
	def AppendTextLineWithItem(self, text, itemvnum, color = FONT_COLOR):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPackedFontColor(color)
		textLine.SetText(text)
		textLine.SetOutline()
		textLine.SetFeather(False)
		textLine.Show()

		textLine.SetPosition(25, self.toolTipHeight+15)
		self.childrenList.append(textLine)
		
		
		item.SelectItem(itemvnum)
		itemName = item.GetItemName()
		itemIcon = item.GetIconImageFileName()
		
		itemImage = ui.ImageBox()
		itemImage.SetParent(self)
		itemImage.Show()

		itemImage.LoadImage(itemIcon)

		itemImage.SetPosition(textLine.GetTextSize()[0]+25, self.toolTipHeight)		
		self.childrenList.append(itemImage)
		
		
		textLine2 = ui.TextLine()
		textLine2.SetParent(self)
		textLine2.SetFontName(self.defFontName)
		textLine2.SetPackedFontColor(color)
		textLine2.SetText(str(itemName))
		textLine2.SetOutline()
		textLine2.SetFeather(False)
		textLine2.Show()

		textLine2.SetPosition(textLine.GetTextSize()[0]+25+itemImage.GetWidth()+5, self.toolTipHeight+15)
		self.childrenList.append(textLine2)
		
		self.toolTipHeight += itemImage.GetHeight()
		
		toplam = int(textLine.GetTextSize()[0]+25+itemImage.GetWidth()+5+textLine2.GetTextSize()[0])
		
		if toplam > 190:
			self.toolTipWidth += 20
		
		
		
		self.ResizeToolTip()
		
		

		# self.toolTipHeight += self.TEXT_LINE_HEIGHT
		# self.ResizeToolTip()

		return textLine		

	def GetMetinSocketType(self, number):
		if player.METIN_SOCKET_TYPE_NONE == number:
			return player.METIN_SOCKET_TYPE_NONE
		elif player.METIN_SOCKET_TYPE_SILVER == number:
			return player.METIN_SOCKET_TYPE_SILVER
		elif player.METIN_SOCKET_TYPE_GOLD == number:
			return player.METIN_SOCKET_TYPE_GOLD
		else:
			item.SelectItem(number)
			if item.METIN_NORMAL == item.GetItemSubType():
				return player.METIN_SOCKET_TYPE_SILVER
			elif item.METIN_GOLD == item.GetItemSubType():
				return player.METIN_SOCKET_TYPE_GOLD
			elif "USE_PUT_INTO_ACCESSORY_SOCKET" == item.GetUseType(number):
				return player.METIN_SOCKET_TYPE_SILVER
			elif "USE_PUT_INTO_RING_SOCKET" == item.GetUseType(number):
				return player.METIN_SOCKET_TYPE_SILVER
			elif "USE_PUT_INTO_BELT_SOCKET" == item.GetUseType(number):
				return player.METIN_SOCKET_TYPE_SILVER

		return player.METIN_SOCKET_TYPE_NONE

	def GetMetinItemIndex(self, number):
		if player.METIN_SOCKET_TYPE_SILVER == number:
			return 0
		if player.METIN_SOCKET_TYPE_GOLD == number:
			return 0

		return number

	def __AppendAccessoryMetinSlotInfo(self, metinSlot, mtrlVnum):		
		ACCESSORY_SOCKET_MAX_SIZE = 4		

		cur=min(metinSlot[0], ACCESSORY_SOCKET_MAX_SIZE)
		end=min(metinSlot[1], ACCESSORY_SOCKET_MAX_SIZE)
		
		perma = metinSlot[4]


		affectType1, affectValue1 = item.GetAffect(0)
		affectList1=[0, max(1, affectValue1*10/100), max(2, affectValue1*20/100), max(3, affectValue1*40/100), max(4, affectValue1*60/100)]

		affectType2, affectValue2 = item.GetAffect(1)
		affectList2=[0, max(1, affectValue2*10/100), max(2, affectValue2*20/100), max(3, affectValue2*40/100), max(4, affectValue1*60/100)]
		
		affectType3, affectValue3 = item.GetAffect(2)
		affectList3=[0, max(1, affectValue3*10/100), max(2, affectValue3*20/100), max(3, affectValue3*40/100), max(4, affectValue1*60/100)]
		
		mtrlPos=0
		mtrlList=[mtrlVnum]*cur+[player.METIN_SOCKET_TYPE_SILVER]*(end-cur)
		for mtrl in mtrlList:
			affectString1 = self.__GetAffectString(affectType1, affectList1[mtrlPos+1]-affectList1[mtrlPos])			
			affectString2 = self.__GetAffectString(affectType2, affectList2[mtrlPos+1]-affectList2[mtrlPos])
			affectString3 = self.__GetAffectString(affectType3, affectList3[mtrlPos+1]-affectList3[mtrlPos])

			leftTime = 0
			if cur == mtrlPos+1:
				leftTime=metinSlot[2]

			self.__AppendMetinSlotInfo_AppendMetinSocketData(mtrlPos, mtrl, affectString1, affectString2, affectString3, leftTime, perma)
			mtrlPos+=1

	def __AppendMetinSlotInfo(self, metinSlot):
		if self.__AppendMetinSlotInfo_IsEmptySlotList(metinSlot):
			return

		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			self.__AppendMetinSlotInfo_AppendMetinSocketData(i, metinSlot[i])

	def __AppendMetinSlotInfo_IsEmptySlotList(self, metinSlot):
		if 0 == metinSlot:
			return 1

		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlotData=metinSlot[i]
			if 0 != self.GetMetinSocketType(metinSlotData):
				if 0 != self.GetMetinItemIndex(metinSlotData):
					return 0

		return 1

	def __AppendMetinSlotInfo_AppendMetinSocketData(self, index, metinSlotData, custumAffectString="", custumAffectString2="", custumAffectString3="", leftTime=0, perma =0):

		slotType = self.GetMetinSocketType(metinSlotData)
		itemIndex = self.GetMetinItemIndex(metinSlotData)
		


		if 0 == slotType:
			return

		self.AppendSpace(5)

		slotImage = ui.ImageBox()
		slotImage.SetParent(self)
		slotImage.Show()

		## Name
		nameTextLine = ui.TextLine()
		nameTextLine.SetParent(self)
		nameTextLine.SetFontName(self.defFontName)
		nameTextLine.SetPackedFontColor(self.NORMAL_COLOR)
		nameTextLine.SetOutline()
		nameTextLine.SetFeather()
		nameTextLine.Show()			

		self.childrenList.append(nameTextLine)

		if player.METIN_SOCKET_TYPE_SILVER == slotType and perma == 0:
			slotImage.LoadImage("d:/ymir work/ui/game/windows/metin_slot_silver.sub")
		elif player.METIN_SOCKET_TYPE_GOLD == slotType and perma == 0:
			slotImage.LoadImage("d:/ymir work/ui/game/windows/metin_slot_gold.sub")
		elif perma > 0:
			slotImage.LoadImage("d:/ymir work/ui/game/windows/metin_slot_gold.sub")

		self.childrenList.append(slotImage)
		
		if localeInfo.IsARABIC():
			slotImage.SetPosition(self.toolTipWidth - slotImage.GetWidth() - 9, self.toolTipHeight-1)
			nameTextLine.SetPosition(self.toolTipWidth - 50, self.toolTipHeight + 2)
		else:
			slotImage.SetPosition(9, self.toolTipHeight-1)
			nameTextLine.SetPosition(50, self.toolTipHeight + 2)

		metinImage = ui.ImageBox()
		metinImage.SetParent(self)
		metinImage.Show()
		self.childrenList.append(metinImage)

		if itemIndex:

			item.SelectItem(itemIndex)

			## Image
			try:
				metinImage.LoadImage(item.GetIconImageFileName())
			except:
				dbg.TraceError("ItemToolTip.__AppendMetinSocketData() - Failed to find image file %d:%s" % 
					(itemIndex, item.GetIconImageFileName())
				)

			nameTextLine.SetText(item.GetItemName())
			
			## Affect		
			affectTextLine = ui.TextLine()
			affectTextLine.SetParent(self)
			affectTextLine.SetFontName(self.defFontName)
			affectTextLine.SetPackedFontColor(self.POSITIVE_COLOR)
			affectTextLine.SetOutline()
			affectTextLine.SetFeather()
			affectTextLine.Show()			
				
			if localeInfo.IsARABIC():
				metinImage.SetPosition(self.toolTipWidth - metinImage.GetWidth() - 10, self.toolTipHeight)
				affectTextLine.SetPosition(self.toolTipWidth - 50, self.toolTipHeight + 16 + 2)
			else:
				metinImage.SetPosition(10, self.toolTipHeight)
				affectTextLine.SetPosition(50, self.toolTipHeight + 16 + 2)
							
			if custumAffectString:
				affectTextLine.SetText(custumAffectString)
			elif itemIndex!=constInfo.ERROR_METIN_STONE:
				for i in xrange(item.ITEM_APPLY_MAX_NUM):
					(affectType, affectValue) = item.GetAffect(i)
					affectString = self.__GetAffectString(affectType, affectValue)
					if affectString:
						self.AppendSpace(20)
						self.StoneAppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))
			else:
				affectTextLine.SetText(localeInfo.TOOLTIP_APPLY_NOAFFECT)
			
			self.childrenList.append(affectTextLine)			

			if custumAffectString2:
				affectTextLine = ui.TextLine()
				affectTextLine.SetParent(self)
				affectTextLine.SetFontName(self.defFontName)
				affectTextLine.SetPackedFontColor(self.POSITIVE_COLOR)
				affectTextLine.SetPosition(50, self.toolTipHeight + 16 + 2 + 16 + 2)
				affectTextLine.SetOutline()
				affectTextLine.SetFeather()
				affectTextLine.Show()
				affectTextLine.SetText(custumAffectString2)
				self.childrenList.append(affectTextLine)
				self.toolTipHeight += 16 + 2
				
			if custumAffectString3:
				affectTextLine = ui.TextLine()
				affectTextLine.SetParent(self)
				affectTextLine.SetFontName(self.defFontName)
				affectTextLine.SetPackedFontColor(self.POSITIVE_COLOR)
				affectTextLine.SetPosition(50, self.toolTipHeight + 16 + 2 + 16 + 2)
				affectTextLine.SetOutline()
				affectTextLine.SetFeather()
				affectTextLine.Show()
				affectTextLine.SetText(custumAffectString3)
				self.childrenList.append(affectTextLine)
				self.toolTipHeight += 16 + 2
				
				

			if 0 != leftTime and perma != 999:
				timeText = (localeInfo.LEFT_TIME + " : " + localeInfo.SecondToDHM(leftTime))

				timeTextLine = ui.TextLine()
				timeTextLine.SetParent(self)
				timeTextLine.SetFontName(self.defFontName)
				timeTextLine.SetPackedFontColor(self.POSITIVE_COLOR)
				timeTextLine.SetPosition(50, self.toolTipHeight + 16 + 2 + 16 + 2)
				timeTextLine.SetOutline()
				timeTextLine.SetFeather()
				timeTextLine.Show()
				timeTextLine.SetText(timeText)
				self.childrenList.append(timeTextLine)
				self.toolTipHeight += 16 + 2

		else:
			nameTextLine.SetText(localeInfo.TOOLTIP_SOCKET_EMPTY)

		self.toolTipHeight += 35
		self.ResizeToolTip()

	def __AppendFishInfo(self, size):
		if size > 0:
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_FISH_LEN % (float(size) / 100.0), self.NORMAL_COLOR)

	def AppendUniqueItemLastTime(self, restMin):
		restSecond = restMin*60
		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.LEFT_TIME + " : " + localeInfo.SecondToDHM(restSecond), self.NORMAL_COLOR)

	def AppendMallItemLastTime(self, endTime):
		leftSec = max(0, endTime - app.GetGlobalTimeStamp())
		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.LEFT_TIME + " : " + localeInfo.SecondToDHM(leftSec), self.NORMAL_COLOR)
		
	def AppendTimerBasedOnWearLastTime(self, metinSlot):
		if 0 == metinSlot[0]:
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.CANNOT_USE, self.DISABLE_COLOR)
		else:
			endTime = app.GetGlobalTimeStamp() + metinSlot[0]
			self.AppendMallItemLastTime(endTime)		
	
	def AppendRealTimeStartFirstUseLastTime(self, item, metinSlot, limitIndex):		
		useCount = metinSlot[1]
		endTime = metinSlot[0]
		
		# 한 번이라도 사용했다면 Socket0에 종료 시간(2012년 3월 1일 13시 01분 같은..) 이 박혀있음.
		# 사용하지 않았다면 Socket0에 이용가능시간(이를테면 600 같은 값. 초단위)이 들어있을 수 있고, 0이라면 Limit Value에 있는 이용가능시간을 사용한다.
		if 0 == useCount:
			if 0 == endTime:
				(limitType, limitValue) = item.GetLimit(limitIndex)
				endTime = limitValue

			endTime += app.GetGlobalTimeStamp()
	
		self.AppendMallItemLastTime(endTime)
	if app.ENABLE_CHANGELOOK_SYSTEM:
		def AppendTransmutation(self, window_type, slotIndex, transmutation):
			itemVnum = 0
			if transmutation == -1:
				if window_type == player.INVENTORY:
					itemVnum = player.GetItemTransmutation(window_type, slotIndex)
				elif window_type == player.SAFEBOX:
					itemVnum = safebox.GetItemTransmutation(slotIndex)
				elif window_type == player.MALL:
					itemVnum = safebox.GetMallItemTransmutation(slotIndex)
			else:
				itemVnum = transmutation
			
			if not itemVnum:
				return
			
			item.SelectItem(itemVnum)
			itemName = item.GetItemName()
			if not itemName or itemName == "":
				return
			
			self.AppendSpace(5)
			title = "[ " + localeInfo.CHANGE_LOOK_TITLE + " ]"
			self.AppendTextLine(title, self.BEFORE_LOOK_COLOR)
			textLine = self.AppendTextLine(itemName, self.UNDER_LOOK_COLOR, True)
			textLine.SetFeather()
			
		def AppendTransmutationEx(self, window_type, slotIndex, transmutation):
			mobVnum = 0
			if transmutation == -1:
				if window_type == player.INVENTORY:
					mobVnum = player.GetItemTransmutation(window_type, slotIndex)
				elif window_type == player.SAFEBOX:
					mobVnum = safebox.GetItemTransmutation(slotIndex)
				elif window_type == player.MALL:
					mobVnum = safebox.GetMallItemTransmutation(slotIndex)
			else:
				mobVnum = transmutation

			if not mobVnum:
				return
				
			mobName = nonplayer.GetMonsterName(mobVnum)
			if not mobName or mobName == "":
				return
				
			self.AppendSpace(5)
			title = "[ " + localeInfo.CHANGE_LOOK_TITLE + " ]"
			self.AppendTextLine(title, self.BEFORE_LOOK_COLOR)
			textLine = self.AppendTextLine(mobName, self.UNDER_LOOK_COLOR, True)
			textLine.SetFeather()
	if app.ENABLE_SASH_SYSTEM:
		def SetSashResultItem(self, slotIndex, window_type = player.INVENTORY):
			(itemVnum, MinAbs, MaxAbs) = sash.GetResultItem()

	if app.ENABLE_AURA_SYSTEM:
		def SetAuraResultItem(self, slotIndex, window_type = player.INVENTORY):
			(itemVnum, MinAbs, MaxAbs) = aura.GetResultItem()

			if not itemVnum:
				return
			
			self.ClearToolTip()
			
			metinSlot = [player.GetItemMetinSocket(window_type, slotIndex, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]
			attrSlot = [player.GetItemAttribute(window_type, slotIndex, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]
			
			item.SelectItem(itemVnum)
			itemType = item.GetItemType()
			itemSubType = item.GetItemSubType()
			if itemType != item.ITEM_TYPE_COSTUME and itemSubType != item.COSTUME_TYPE_SASH:
				return
			
			absChance = MaxAbs
			itemDesc = item.GetItemDescription()
			self.__AdjustMaxWidth(attrSlot, itemDesc)
			self.__SetItemTitle(itemVnum, metinSlot, attrSlot)
			self.AppendDescription(itemDesc, 26)
			self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
			self.__AppendLimitInformation()
			
			## ABSORPTION RATE
			if MinAbs == MaxAbs:
				self.AppendTextLine(localeInfo.SASH_ABSORB_CHANCE % (MinAbs), self.CONDITION_COLOR)
			else:
				self.AppendTextLine(localeInfo.SASH_ABSORB_CHANCE2 % (MinAbs, MaxAbs), self.CONDITION_COLOR)
			## END ABSOPRTION RATE
			
			itemAbsorbedVnum = int(metinSlot[sash.ABSORBED_SOCKET])
			if itemAbsorbedVnum:
				## ATTACK / DEFENCE
				item.SelectItem(itemAbsorbedVnum)
				if item.GetItemType() == item.ITEM_TYPE_WEAPON:
					if item.GetItemSubType() == item.WEAPON_FAN:
						self.__AppendMagicAttackInfo(absChance)
						item.SelectItem(itemAbsorbedVnum)
						self.__AppendAttackPowerInfo(absChance)
					else:
						self.__AppendAttackPowerInfo(absChance)
						item.SelectItem(itemAbsorbedVnum)
						self.__AppendMagicAttackInfo(absChance)
				elif item.GetItemType() == item.ITEM_TYPE_ARMOR:
					defGrade = item.GetValue(1)
					defBonus = item.GetValue(5) * 2
					defGrade = self.CalcSashValue(defGrade, absChance)
					defBonus = self.CalcSashValue(defBonus, absChance)
					
					if defGrade > 0:
						self.AppendSpace(5)
						self.AppendTextLine(localeInfo.TOOLTIP_ITEM_DEF_GRADE % (defGrade + defBonus), self.GetChangeTextLineColor(defGrade))
					
					item.SelectItem(itemAbsorbedVnum)
					self.__AppendMagicDefenceInfo(absChance)
				## END ATTACK / DEFENCE
				
				## EFFECT
				item.SelectItem(itemAbsorbedVnum)
				for i in xrange(item.ITEM_APPLY_MAX_NUM):
					(affectType, affectValue) = item.GetAffect(i)
					affectValue = self.CalcSashValue(affectValue, absChance)
					affectString = self.__GetAffectString(affectType, affectValue)
					if affectString and affectValue > 0:
						self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))
					
					item.SelectItem(itemAbsorbedVnum)
				# END EFFECT
				
			item.SelectItem(itemVnum)
			## ATTR
			self.__AppendAttributeInformation(attrSlot, MaxAbs)
			# END ATTR
			
			self.AppendWearableInformation()
			self.ShowToolTip()

		def SetAuraResultAbsItem(self, slotIndex1, slotIndex2, window_type = player.INVENTORY):
			itemVnumAura = player.GetItemIndex(window_type, slotIndex1)
			itemVnumTarget = player.GetItemIndex(window_type, slotIndex2)
			if not itemVnumAura or not itemVnumTarget:
				return
			
			self.ClearToolTip()
			
			item.SelectItem(itemVnumAura)
			itemType = item.GetItemType()
			itemSubType = item.GetItemSubType()
			if itemType != item.ITEM_TYPE_COSTUME and itemSubType != item.COSTUME_TYPE_AURA:
				return
			
			metinSlot = [player.GetItemMetinSocket(window_type, slotIndex1, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]
			attrSlot = [player.GetItemAttribute(window_type, slotIndex2, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]
			
			itemDesc = item.GetItemDescription()
			self.__AdjustMaxWidth(attrSlot, itemDesc)
			self.__SetItemTitle(itemVnumAura, metinSlot, attrSlot)
			self.AppendDescription(itemDesc, 26)
			self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
			item.SelectItem(itemVnumAura)
			self.__AppendLimitInformation()
			
			absChance = float(metinSlot[aura.ABSORPTION_SOCKET]) / 10.0
			
			## ABSORPTION RATE
			self.AppendTextLine(localeInfo.AURA_LEVEL_STEP % (int(metinSlot[aura.LEVEL_SOCKET]), int(metinSlot[aura.LEVEL_SOCKET])), self.CONDITION_COLOR)
			self.AppendTextLine(localeInfo.AURA_DRAIN_PER % (absChance), self.CONDITION_COLOR)
			## END ABSOPRTION RATE
			
			## ATTACK / DEFENCE
			itemAbsorbedVnum = itemVnumTarget
			item.SelectItem(itemAbsorbedVnum)
			if item.GetItemType() == item.ITEM_TYPE_WEAPON:
				if item.GetItemSubType() == item.WEAPON_FAN:
					self.__AppendMagicAttackInfo(absChance)
					item.SelectItem(itemAbsorbedVnum)
					self.__AppendAttackPowerInfo(absChance)
				else:
					self.__AppendAttackPowerInfo(absChance)
					item.SelectItem(itemAbsorbedVnum)
					self.__AppendMagicAttackInfo(absChance)
			elif item.GetItemType() == item.ITEM_TYPE_ARMOR:
				defGrade = item.GetValue(1)
				defBonus = item.GetValue(5) * 2
				defGrade = self.CalcSashValue(defGrade, absChance)
				defBonus = self.CalcSashValue(defBonus, absChance)
				
				if defGrade > 0:
					self.AppendSpace(5)
					self.AppendTextLine(localeInfo.TOOLTIP_ITEM_DEF_GRADE % (defGrade + defBonus), self.GetChangeTextLineColor(defGrade))
				
				item.SelectItem(itemAbsorbedVnum)
				self.__AppendMagicDefenceInfo(absChance)
			## END ATTACK / DEFENCE
			
			## EFFECT
			item.SelectItem(itemAbsorbedVnum)
			for i in xrange(item.ITEM_APPLY_MAX_NUM):
				(affectType, affectValue) = item.GetAffect(i)
				affectValue = self.CalcSashValue(affectValue, absChance)
				affectString = self.__GetAffectString(affectType, affectValue)
				if affectString and affectValue > 0:
					self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))
				
				item.SelectItem(itemAbsorbedVnum)
			## END EFFECT
			
			## ATTR
			item.SelectItem(itemAbsorbedVnum)
			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
				type = attrSlot[i][0]
				value = attrSlot[i][1]
				if not value:
					continue
				
				value = self.CalcSashValue(value, absChance)
				affectString = self.__GetAffectString(type, value)
				if affectString and value > 0:
					affectColor = self.__GetAttributeColor(i, value)
					self.AppendTextLine(affectString, affectColor)
				
				item.SelectItem(itemAbsorbedVnum)
			## END ATTR
			
			## WEARABLE
			item.SelectItem(itemVnumAura)
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_ITEM_WEARABLE_JOB, self.NORMAL_COLOR)
			
			item.SelectItem(itemVnumAura)
			flagList = (
						not item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR),
						not item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN),
						not item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA),
						not item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN)
			)
			flagList += (not item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN),)
			
			characterNames = ""
			for i in xrange(self.CHARACTER_COUNT):
				name = self.CHARACTER_NAMES[i]
				flag = flagList[i]
				if flag:
					characterNames += " "
					characterNames += name
			
			textLine = self.AppendTextLine(characterNames, self.NORMAL_COLOR, True)
			textLine.SetFeather()
			
			item.SelectItem(itemVnumAura)
			if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE):
				textLine = self.AppendTextLine(localeInfo.FOR_FEMALE, self.NORMAL_COLOR, True)
				textLine.SetFeather()
			
			if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE):
				textLine = self.AppendTextLine(localeInfo.FOR_MALE, self.NORMAL_COLOR, True)
				textLine.SetFeather()
			## END WEARABLE
			
			self.ShowToolTip()


		def SetSashResultAbsItem(self, slotIndex1, slotIndex2, window_type = player.INVENTORY):
			itemVnumSash = player.GetItemIndex(window_type, slotIndex1)
			itemVnumTarget = player.GetItemIndex(window_type, slotIndex2)
			if not itemVnumSash or not itemVnumTarget:
				return
			
			self.ClearToolTip()
			
			item.SelectItem(itemVnumSash)
			itemType = item.GetItemType()
			itemSubType = item.GetItemSubType()
			if itemType != item.ITEM_TYPE_COSTUME and itemSubType != item.COSTUME_TYPE_SASH:
				return
			
			metinSlot = [player.GetItemMetinSocket(window_type, slotIndex1, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]
			attrSlot = [player.GetItemAttribute(window_type, slotIndex2, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]
			
			itemDesc = item.GetItemDescription()
			self.__AdjustMaxWidth(attrSlot, itemDesc)
			self.__SetItemTitle(itemVnumSash, metinSlot, attrSlot)
			self.AppendDescription(itemDesc, 26)
			self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
			item.SelectItem(itemVnumSash)
			self.__AppendLimitInformation()
			
			## ABSORPTION RATE
			self.AppendTextLine(localeInfo.SASH_ABSORB_CHANCE % (metinSlot[sash.ABSORPTION_SOCKET]), self.CONDITION_COLOR)
			## END ABSOPRTION RATE
			
			## ATTACK / DEFENCE
			itemAbsorbedVnum = itemVnumTarget
			item.SelectItem(itemAbsorbedVnum)
			if item.GetItemType() == item.ITEM_TYPE_WEAPON:
				if item.GetItemSubType() == item.WEAPON_FAN:
					self.__AppendMagicAttackInfo(metinSlot[sash.ABSORPTION_SOCKET])
					item.SelectItem(itemAbsorbedVnum)
					self.__AppendAttackPowerInfo(metinSlot[sash.ABSORPTION_SOCKET])
				else:
					self.__AppendAttackPowerInfo(metinSlot[sash.ABSORPTION_SOCKET])
					item.SelectItem(itemAbsorbedVnum)
					self.__AppendMagicAttackInfo(metinSlot[sash.ABSORPTION_SOCKET])
			elif item.GetItemType() == item.ITEM_TYPE_ARMOR:
				defGrade = item.GetValue(1)
				defBonus = item.GetValue(5) * 2
				defGrade = self.CalcSashValue(defGrade, metinSlot[sash.ABSORPTION_SOCKET])
				defBonus = self.CalcSashValue(defBonus, metinSlot[sash.ABSORPTION_SOCKET])
				
				if defGrade > 0:
					self.AppendSpace(5)
					self.AppendTextLine(localeInfo.TOOLTIP_ITEM_DEF_GRADE % (defGrade + defBonus), self.GetChangeTextLineColor(defGrade))
				
				item.SelectItem(itemAbsorbedVnum)
				self.__AppendMagicDefenceInfo(metinSlot[sash.ABSORPTION_SOCKET])
			## END ATTACK / DEFENCE
			
			## EFFECT
			item.SelectItem(itemAbsorbedVnum)
			for i in xrange(item.ITEM_APPLY_MAX_NUM):
				(affectType, affectValue) = item.GetAffect(i)
				affectValue = self.CalcSashValue(affectValue, metinSlot[sash.ABSORPTION_SOCKET])
				affectString = self.__GetAffectString(affectType, affectValue)
				if affectString and affectValue > 0:
					self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))
				
				item.SelectItem(itemAbsorbedVnum)
			## END EFFECT
			
			## ATTR
			item.SelectItem(itemAbsorbedVnum)
			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
				type = attrSlot[i][0]
				value = attrSlot[i][1]
				if not value:
					continue
				
				value = self.CalcSashValue(value, metinSlot[sash.ABSORPTION_SOCKET])
				affectString = self.__GetAffectString(type, value)
				if affectString and value > 0:
					affectColor = self.__GetAttributeColor(i, value)
					self.AppendTextLine(affectString, affectColor)
				
				item.SelectItem(itemAbsorbedVnum)
			## END ATTR
			
			## WEARABLE
			item.SelectItem(itemVnumSash)
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_ITEM_WEARABLE_JOB, self.NORMAL_COLOR)
			
			item.SelectItem(itemVnumSash)
			flagList = (
						not item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR),
						not item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN),
						not item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA),
						not item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN),
						not item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN)
			)

			
			characterNames = ""
			for i in xrange(self.CHARACTER_COUNT):
				name = self.CHARACTER_NAMES[i]
				flag = flagList[i]
				if flag:
					characterNames += " "
					characterNames += name
			
			textLine = self.AppendTextLine(characterNames, self.NORMAL_COLOR, True)
			textLine.SetFeather()
			
			item.SelectItem(itemVnumSash)
			if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE):
				textLine = self.AppendTextLine(localeInfo.FOR_FEMALE, self.NORMAL_COLOR, True)
				textLine.SetFeather()
			
			if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE):
				textLine = self.AppendTextLine(localeInfo.FOR_MALE, self.NORMAL_COLOR, True)
				textLine.SetFeather()
			## END WEARABLE
			
			self.ShowToolTip()	
	
class HyperlinkItemToolTip(ItemToolTip):
	def __init__(self):
		ItemToolTip.__init__(self, isPickable=True)

	def SetHyperlinkItem(self, tokens):
		minTokenCount = 3 + player.METIN_SOCKET_MAX_NUM
		maxTokenCount = minTokenCount + 2 * player.ATTRIBUTE_SLOT_MAX_NUM
		maxTokenCount += 1 #Evolution
		if tokens and len(tokens) >= minTokenCount and len(tokens) <= maxTokenCount:
			head, vnum, flag = tokens[:3]
			itemVnum = int(vnum, 16)
			metinSlot = [int(metin, 16) for metin in tokens[3:10]]

			rests = tokens[10:]
			if rests:
				attrSlot = []

				rests.reverse()
				while rests:
					key = int(rests.pop(), 16)
					if rests:
						val = int(rests.pop())
						attrSlot.append((key, val))

				attrSlot += [(0, 0)] * (player.ATTRIBUTE_SLOT_MAX_NUM - len(attrSlot))
			else:
				attrSlot = [(0, 0)] * player.ATTRIBUTE_SLOT_MAX_NUM
			evo=0
			evolu = tokens[23:]
			if evolu:
				evo=int(tokens[23])

			self.ClearToolTip()
			self.AddItemData(itemVnum, metinSlot, attrSlot,0,0,0,evo)

			ItemToolTip.OnUpdate(self)

	def OnUpdate(self):
		pass

	def OnMouseLeftButtonDown(self):
		self.Hide()

class SkillToolTip(ToolTip):

	POINT_NAME_DICT = {
		player.LEVEL : localeInfo.SKILL_TOOLTIP_LEVEL,
		player.IQ : localeInfo.SKILL_TOOLTIP_INT,
	}

	SKILL_TOOL_TIP_WIDTH = 200
	PARTY_SKILL_TOOL_TIP_WIDTH = 340

	PARTY_SKILL_EXPERIENCE_AFFECT_LIST = (	( 2, 2,  10,),
											( 8, 3,  20,),
											(14, 4,  30,),
											(22, 5,  45,),
											(28, 6,  60,),
											(34, 7,  80,),
											(38, 8, 100,), )

	PARTY_SKILL_PLUS_GRADE_AFFECT_LIST = (	( 4, 2, 1, 0,),
											(10, 3, 2, 0,),
											(16, 4, 2, 1,),
											(24, 5, 2, 2,), )

	PARTY_SKILL_ATTACKER_AFFECT_LIST = (	( 36, 3, ),
											( 26, 1, ),
											( 32, 2, ), )

	SKILL_GRADE_NAME = {	player.SKILL_GRADE_MASTER : localeInfo.SKILL_GRADE_NAME_MASTER,
							player.SKILL_GRADE_GRAND_MASTER : localeInfo.SKILL_GRADE_NAME_GRAND_MASTER,
							player.SKILL_GRADE_PERFECT_MASTER : localeInfo.SKILL_GRADE_NAME_PERFECT_MASTER, }

	AFFECT_NAME_DICT =	{
							"HP" : localeInfo.TOOLTIP_SKILL_AFFECT_ATT_POWER,
							"ATT_GRADE" : localeInfo.TOOLTIP_SKILL_AFFECT_ATT_GRADE,
							"DEF_GRADE" : localeInfo.TOOLTIP_SKILL_AFFECT_DEF_GRADE,
							"ATT_SPEED" : localeInfo.TOOLTIP_SKILL_AFFECT_ATT_SPEED,
							"MOV_SPEED" : localeInfo.TOOLTIP_SKILL_AFFECT_MOV_SPEED,
							"DODGE" : localeInfo.TOOLTIP_SKILL_AFFECT_DODGE,
							"RESIST_NORMAL" : localeInfo.TOOLTIP_SKILL_AFFECT_RESIST_NORMAL,
							"REFLECT_MELEE" : localeInfo.TOOLTIP_SKILL_AFFECT_REFLECT_MELEE,
						}
	AFFECT_APPEND_TEXT_DICT =	{
									"DODGE" : "%",
									"RESIST_NORMAL" : "%",
									"REFLECT_MELEE" : "%",
								}

	def __init__(self):
		ToolTip.__init__(self, self.SKILL_TOOL_TIP_WIDTH)
	def __del__(self):
		ToolTip.__del__(self)

	def SetSkill(self, skillIndex, skillLevel = -1):

		if 0 == skillIndex:
			return

		if skill.SKILL_TYPE_GUILD == skill.GetSkillType(skillIndex):

			if self.SKILL_TOOL_TIP_WIDTH != self.toolTipWidth:
				self.toolTipWidth = self.SKILL_TOOL_TIP_WIDTH
				self.ResizeToolTip()

			self.AppendDefaultData(skillIndex)
			self.AppendSkillConditionData(skillIndex)
			self.AppendGuildSkillData(skillIndex, skillLevel)

		else:

			if self.SKILL_TOOL_TIP_WIDTH != self.toolTipWidth:
				self.toolTipWidth = self.SKILL_TOOL_TIP_WIDTH
				self.ResizeToolTip()

			slotIndex = player.GetSkillSlotIndex(skillIndex)
			skillGrade = player.GetSkillGrade(slotIndex)
			skillLevel = player.GetSkillLevel(slotIndex)
			skillCurrentPercentage = player.GetSkillCurrentEfficientPercentage(slotIndex)
			skillNextPercentage = player.GetSkillNextEfficientPercentage(slotIndex)

			self.AppendDefaultData(skillIndex)
			self.AppendSkillConditionData(skillIndex)
			self.AppendSkillDataNew(slotIndex, skillIndex, skillGrade, skillLevel, skillCurrentPercentage, skillNextPercentage)
			self.AppendSkillRequirement(skillIndex, skillLevel)

		self.ShowToolTip()

	def SetSkillNew(self, slotIndex, skillIndex, skillGrade, skillLevel):

		if 0 == skillIndex:
			return

		if player.SKILL_INDEX_TONGSOL == skillIndex:

			slotIndex = player.GetSkillSlotIndex(skillIndex)
			skillLevel = player.GetSkillLevel(slotIndex)

			self.AppendDefaultData(skillIndex)
			self.AppendPartySkillData(skillGrade, skillLevel)

		elif player.SKILL_INDEX_RIDING == skillIndex:

			slotIndex = player.GetSkillSlotIndex(skillIndex)
			self.AppendSupportSkillDefaultData(skillIndex, skillGrade, skillLevel, 30)

		elif player.SKILL_INDEX_SUMMON == skillIndex:

			maxLevel = 10

			self.ClearToolTip()
			self.__SetSkillTitle(skillIndex, skillGrade)

			## Description
			description = skill.GetSkillDescription(skillIndex)
			self.AppendDescription(description, 25)

			if skillLevel == 10:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL_MASTER % (skillLevel), self.NORMAL_COLOR)
				self.AppendTextLine(localeInfo.SKILL_SUMMON_DESCRIPTION % (skillLevel*10), self.NORMAL_COLOR)

			else:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL % (skillLevel), self.NORMAL_COLOR)
				self.__AppendSummonDescription(skillLevel, self.NORMAL_COLOR)

				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL % (skillLevel+1), self.NEGATIVE_COLOR)
				self.__AppendSummonDescription(skillLevel+1, self.NEGATIVE_COLOR)

		elif skill.SKILL_TYPE_GUILD == skill.GetSkillType(skillIndex):

			if self.SKILL_TOOL_TIP_WIDTH != self.toolTipWidth:
				self.toolTipWidth = self.SKILL_TOOL_TIP_WIDTH
				self.ResizeToolTip()

			self.AppendDefaultData(skillIndex)
			self.AppendSkillConditionData(skillIndex)
			self.AppendGuildSkillData(skillIndex, skillLevel)

		else:

			if self.SKILL_TOOL_TIP_WIDTH != self.toolTipWidth:
				self.toolTipWidth = self.SKILL_TOOL_TIP_WIDTH
				self.ResizeToolTip()

			slotIndex = player.GetSkillSlotIndex(skillIndex)

			skillCurrentPercentage = player.GetSkillCurrentEfficientPercentage(slotIndex)
			skillNextPercentage = player.GetSkillNextEfficientPercentage(slotIndex)

			self.AppendDefaultData(skillIndex, skillGrade)
			self.AppendSkillConditionData(skillIndex)
			self.AppendSkillDataNew(slotIndex, skillIndex, skillGrade, skillLevel, skillCurrentPercentage, skillNextPercentage)
			self.AppendSkillRequirement(skillIndex, skillLevel)

		self.ShowToolTip()

	def __SetSkillTitle(self, skillIndex, skillGrade):
		self.SetTitle(skill.GetSkillName(skillIndex, skillGrade))
		self.__AppendSkillGradeName(skillIndex, skillGrade)
	def __AppendSkillGradeName(self, skillIndex, skillGrade):		
		if self.SKILL_GRADE_NAME.has_key(skillGrade):
			self.AppendSpace(5)
			self.AppendTextLine(self.SKILL_GRADE_NAME[skillGrade] % (skill.GetSkillName(skillIndex, 0)), self.CAN_LEVEL_UP_COLOR)

	def SetSkillOnlyName(self, slotIndex, skillIndex, skillGrade):
		if 0 == skillIndex:
			return

		slotIndex = player.GetSkillSlotIndex(skillIndex)

		self.toolTipWidth = self.SKILL_TOOL_TIP_WIDTH
		self.ResizeToolTip()

		self.ClearToolTip()
		self.__SetSkillTitle(skillIndex, skillGrade)		
		self.AppendDefaultData(skillIndex, skillGrade)
		self.AppendSkillConditionData(skillIndex)		
		self.ShowToolTip()

	def AppendDefaultData(self, skillIndex, skillGrade = 0):
		self.ClearToolTip()
		self.__SetSkillTitle(skillIndex, skillGrade)

		## Level Limit
		levelLimit = skill.GetSkillLevelLimit(skillIndex)
		if levelLimit > 0:

			color = self.NORMAL_COLOR
			if player.GetStatus(player.LEVEL) < levelLimit:
				color = self.NEGATIVE_COLOR

			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_ITEM_LIMIT_LEVEL % (levelLimit), color)

		## Description
		description = skill.GetSkillDescription(skillIndex)
		self.AppendDescription(description, 25)

	def AppendSupportSkillDefaultData(self, skillIndex, skillGrade, skillLevel, maxLevel):
		self.ClearToolTip()
		self.__SetSkillTitle(skillIndex, skillGrade)

		## Description
		description = skill.GetSkillDescription(skillIndex)
		self.AppendDescription(description, 25)

		if 1 == skillGrade:
			skillLevel += 19
		elif 2 == skillGrade:
			skillLevel += 29
		elif 3 == skillGrade:
			skillLevel = 40

		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL_WITH_MAX % (skillLevel, maxLevel), self.NORMAL_COLOR)

	def AppendSkillConditionData(self, skillIndex):
		conditionDataCount = skill.GetSkillConditionDescriptionCount(skillIndex)
		if conditionDataCount > 0:
			self.AppendSpace(5)
			for i in xrange(conditionDataCount):
				self.AppendTextLine(skill.GetSkillConditionDescription(skillIndex, i), self.CONDITION_COLOR)

	def AppendGuildSkillData(self, skillIndex, skillLevel):
		skillMaxLevel = 7
		skillCurrentPercentage = float(skillLevel) / float(skillMaxLevel)
		skillNextPercentage = float(skillLevel+1) / float(skillMaxLevel)
		## Current Level
		if skillLevel > 0:
			if self.HasSkillLevelDescription(skillIndex, skillLevel):
				self.AppendSpace(5)
				if skillLevel == skillMaxLevel:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL_MASTER % (skillLevel), self.NORMAL_COLOR)
				else:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL % (skillLevel), self.NORMAL_COLOR)

				#####

				for i in xrange(skill.GetSkillAffectDescriptionCount(skillIndex)):
					self.AppendTextLine(skill.GetSkillAffectDescription(skillIndex, i, skillCurrentPercentage), self.ENABLE_COLOR)

				## Cooltime
				coolTime = skill.GetSkillCoolTime(skillIndex, skillCurrentPercentage)
				if coolTime > 0:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_COOL_TIME + str(coolTime), self.ENABLE_COLOR)

				## SP
				needGSP = skill.GetSkillNeedSP(skillIndex, skillCurrentPercentage)
				if needGSP > 0:
					self.AppendTextLine(localeInfo.TOOLTIP_NEED_GSP % (needGSP), self.ENABLE_COLOR)

		## Next Level
		if skillLevel < skillMaxLevel:
			if self.HasSkillLevelDescription(skillIndex, skillLevel+1):
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_NEXT_SKILL_LEVEL_1 % (skillLevel+1, skillMaxLevel), self.DISABLE_COLOR)

				#####

				for i in xrange(skill.GetSkillAffectDescriptionCount(skillIndex)):
					self.AppendTextLine(skill.GetSkillAffectDescription(skillIndex, i, skillNextPercentage), self.DISABLE_COLOR)

				## Cooltime
				coolTime = skill.GetSkillCoolTime(skillIndex, skillNextPercentage)
				if coolTime > 0:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_COOL_TIME + str(coolTime), self.DISABLE_COLOR)

				## SP
				needGSP = skill.GetSkillNeedSP(skillIndex, skillNextPercentage)
				if needGSP > 0:
					self.AppendTextLine(localeInfo.TOOLTIP_NEED_GSP % (needGSP), self.DISABLE_COLOR)

	def AppendSkillDataNew(self, slotIndex, skillIndex, skillGrade, skillLevel, skillCurrentPercentage, skillNextPercentage):
		self.skillMaxLevelStartDict = { 0 : 17, 1 : 7, 2 : 10, }
		self.skillMaxLevelEndDict = { 0 : 20, 1 : 10, 2 : 10, }

		skillLevelUpPoint = 1
		realSkillGrade = player.GetSkillGrade(slotIndex)
		skillMaxLevelStart = self.skillMaxLevelStartDict.get(realSkillGrade, 15)
		skillMaxLevelEnd = self.skillMaxLevelEndDict.get(realSkillGrade, 20)

		## Current Level
		if skillLevel > 0:
			if self.HasSkillLevelDescription(skillIndex, skillLevel):
				self.AppendSpace(5)
				if skillGrade == skill.SKILL_GRADE_COUNT:
					pass
				elif skillLevel == skillMaxLevelEnd:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL_MASTER % (skillLevel), self.NORMAL_COLOR)
				else:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL % (skillLevel), self.NORMAL_COLOR)
				self.AppendSkillLevelDescriptionNew(skillIndex, skillCurrentPercentage, self.ENABLE_COLOR)

		## Next Level
		if skillGrade != skill.SKILL_GRADE_COUNT:
			if skillLevel < skillMaxLevelEnd:
				if self.HasSkillLevelDescription(skillIndex, skillLevel+skillLevelUpPoint):
					self.AppendSpace(5)
					## HP보강, 관통회피 보조스킬의 경우
					if skillIndex == 141 or skillIndex == 142:
						self.AppendTextLine(localeInfo.TOOLTIP_NEXT_SKILL_LEVEL_3 % (skillLevel+1), self.DISABLE_COLOR)
					else:
						self.AppendTextLine(localeInfo.TOOLTIP_NEXT_SKILL_LEVEL_1 % (skillLevel+1, skillMaxLevelEnd), self.DISABLE_COLOR)
					self.AppendSkillLevelDescriptionNew(skillIndex, skillNextPercentage, self.DISABLE_COLOR)

	def AppendSkillLevelDescriptionNew(self, skillIndex, skillPercentage, color):

		affectDataCount = skill.GetNewAffectDataCount(skillIndex)
		if affectDataCount > 0:
			for i in xrange(affectDataCount):
				type, minValue, maxValue = skill.GetNewAffectData(skillIndex, i, skillPercentage)

				if not self.AFFECT_NAME_DICT.has_key(type):
					continue

				minValue = int(minValue)
				maxValue = int(maxValue)
				affectText = self.AFFECT_NAME_DICT[type]

				if "HP" == type:
					if minValue < 0 and maxValue < 0:
						minValue *= -1
						maxValue *= -1

					else:
						affectText = localeInfo.TOOLTIP_SKILL_AFFECT_HEAL

				affectText += str(minValue)
				if minValue != maxValue:
					affectText += " - " + str(maxValue)
				affectText += self.AFFECT_APPEND_TEXT_DICT.get(type, "")

				#import debugInfo
				#if debugInfo.IsDebugMode():
				#	affectText = "!!" + affectText

				self.AppendTextLine(affectText, color)

		else:
			for i in xrange(skill.GetSkillAffectDescriptionCount(skillIndex)):
				self.AppendTextLine(skill.GetSkillAffectDescription(skillIndex, i, skillPercentage), color)


		## Duration
		duration = skill.GetDuration(skillIndex, skillPercentage)
		if duration > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_SKILL_DURATION % (duration), color)

		## Cooltime
		coolTime = skill.GetSkillCoolTime(skillIndex, skillPercentage)
		if coolTime > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_SKILL_COOL_TIME + str(coolTime), color)

		## SP
		needSP = skill.GetSkillNeedSP(skillIndex, skillPercentage)
		if needSP != 0:
			continuationSP = skill.GetSkillContinuationSP(skillIndex, skillPercentage)

			if skill.IsUseHPSkill(skillIndex):
				self.AppendNeedHP(needSP, continuationSP, color)
			else:
				self.AppendNeedSP(needSP, continuationSP, color)


	def AppendSkillRequirement(self, skillIndex, skillLevel):

		skillMaxLevel = skill.GetSkillMaxLevel(skillIndex)

		if skillLevel >= skillMaxLevel:
			return

		isAppendHorizontalLine = False

		## Requirement
		if skill.IsSkillRequirement(skillIndex):

			if not isAppendHorizontalLine:
				isAppendHorizontalLine = True
				self.AppendHorizontalLine()

			requireSkillName, requireSkillLevel = skill.GetSkillRequirementData(skillIndex)

			color = self.CANNOT_LEVEL_UP_COLOR
			if skill.CheckRequirementSueccess(skillIndex):
				color = self.CAN_LEVEL_UP_COLOR
			self.AppendTextLine(localeInfo.TOOLTIP_REQUIREMENT_SKILL_LEVEL % (requireSkillName, requireSkillLevel), color)

		## Require Stat
		requireStatCount = skill.GetSkillRequireStatCount(skillIndex)
		if requireStatCount > 0:

			for i in xrange(requireStatCount):
				type, level = skill.GetSkillRequireStatData(skillIndex, i)
				if self.POINT_NAME_DICT.has_key(type):

					if not isAppendHorizontalLine:
						isAppendHorizontalLine = True
						self.AppendHorizontalLine()

					name = self.POINT_NAME_DICT[type]
					color = self.CANNOT_LEVEL_UP_COLOR
					if player.GetStatus(type) >= level:
						color = self.CAN_LEVEL_UP_COLOR
					self.AppendTextLine(localeInfo.TOOLTIP_REQUIREMENT_STAT_LEVEL % (name, level), color)

	def HasSkillLevelDescription(self, skillIndex, skillLevel):
		if skill.GetSkillAffectDescriptionCount(skillIndex) > 0:
			return True
		if skill.GetSkillCoolTime(skillIndex, skillLevel) > 0:
			return True
		if skill.GetSkillNeedSP(skillIndex, skillLevel) > 0:
			return True

		return False

	def AppendMasterAffectDescription(self, index, desc, color):
		self.AppendTextLine(desc, color)

	def AppendNextAffectDescription(self, index, desc):
		self.AppendTextLine(desc, self.DISABLE_COLOR)

	def AppendNeedHP(self, needSP, continuationSP, color):

		self.AppendTextLine(localeInfo.TOOLTIP_NEED_HP % (needSP), color)

		if continuationSP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_NEED_HP_PER_SEC % (continuationSP), color)

	def AppendNeedSP(self, needSP, continuationSP, color):

		if -1 == needSP:
			self.AppendTextLine(localeInfo.TOOLTIP_NEED_ALL_SP, color)

		else:
			self.AppendTextLine(localeInfo.TOOLTIP_NEED_SP % (needSP), color)

		if continuationSP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_NEED_SP_PER_SEC % (continuationSP), color)

	def AppendPartySkillData(self, skillGrade, skillLevel):
		def comma_fix(vl):
			return vl.replace("%,0f", "%.0f")

		if 1 == skillGrade:
			skillLevel += 19
		elif 2 == skillGrade:
			skillLevel += 29
		elif 3 == skillGrade:
			skillLevel = 40

		if skillLevel <= 0:
			return

		skillIndex = player.SKILL_INDEX_TONGSOL
		slotIndex = player.GetSkillSlotIndex(skillIndex)
		skillPower = player.GetSkillCurrentEfficientPercentage(slotIndex)
		k = player.GetSkillLevel(skillIndex) / 100.0
		self.AppendSpace(5)
		self.AutoAppendTextLine(localeInfo.TOOLTIP_PARTY_SKILL_LEVEL % skillLevel, self.NORMAL_COLOR)

		if skillLevel >= 10:
			self.AutoAppendTextLine(comma_fix(localeInfo.PARTY_SKILL_ATTACKER) % chop( 10 + 60 * k ))

		if skillLevel >= 20:
			self.AutoAppendTextLine(comma_fix(localeInfo.PARTY_SKILL_BERSERKER) % chop(1 + 5 * k))
			self.AutoAppendTextLine(comma_fix(localeInfo.PARTY_SKILL_TANKER) % chop(50 + 1450 * k))

		if skillLevel >= 25:
			self.AutoAppendTextLine(comma_fix(localeInfo.PARTY_SKILL_BUFFER) % chop(5 + 45 * k ))

		if skillLevel >= 35:
			self.AutoAppendTextLine(comma_fix(localeInfo.PARTY_SKILL_SKILL_MASTER) % chop(25 + 600 * k ))

		if skillLevel >= 40:
			self.AutoAppendTextLine(comma_fix(localeInfo.PARTY_SKILL_DEFENDER) % chop( 5 + 30 * k ))

		self.AlignHorizonalCenter()

	def __AppendSummonDescription(self, skillLevel, color):
		if skillLevel > 1:
			self.AppendTextLine(localeInfo.SKILL_SUMMON_DESCRIPTION % (skillLevel * 10), color)
		elif 1 == skillLevel:
			self.AppendTextLine(localeInfo.SKILL_SUMMON_DESCRIPTION % (15), color)
		elif 0 == skillLevel:
			self.AppendTextLine(localeInfo.SKILL_SUMMON_DESCRIPTION % (10), color)


if __name__ == "__main__":	
	import app
	import wndMgr
	import systemSetting
	import mouseModule
	import grp
	import ui
	
	#wndMgr.SetOutlineFlag(True)

	app.SetMouseHandler(mouseModule.mouseController)
	app.SetHairColorEnable(True)
	wndMgr.SetMouseHandler(mouseModule.mouseController)
	wndMgr.SetScreenSize(systemSetting.GetWidth(), systemSetting.GetHeight())
	app.Create("METIN2 CLOSED BETA", systemSetting.GetWidth(), systemSetting.GetHeight(), 1)
	mouseModule.mouseController.Create()

	toolTip = ItemToolTip()
	toolTip.ClearToolTip()
	#toolTip.AppendTextLine("Test")
	desc = "Item descriptions:|increase of width of display to 35 digits per row AND installation of function that the displayed words are not broken up in two parts, but instead if one word is too long to be displayed in this row, this word will start in the next row."
	summ = ""

	toolTip.AddItemData_Offline(10, desc, summ, 0, 0) 
	toolTip.Show()
	
	app.Loop()
