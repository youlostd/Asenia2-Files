import app, pysettings
import player
import item
import ui
import localeInfo
import uiToolTip
import constInfo
import wndMgr

class CharacterDetailsUI(ui.ScriptWindow):
	def __init__(self, parent):
		self.uiCharacterStatus = parent		
		ui.ScriptWindow.__init__(self)
		self.toolTip = uiToolTip.ToolTip()
		
		self.__LoadScript()
			
	def __del__(self):
		self.uiCharacterStatus = None
		self.toolTip = None
		ui.ScriptWindow.__del__(self)
		
	def __LoadScript(self):

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/CharacterDetailsWindow.py")
		except:
			import exception
			exception.Abort("CharacterDetailsUI.__LoadScript")

		self.Width = 253 - 3

		self.ScrollBar = self.GetChild("ScrollBar")
		self.karakterbonus		= self.GetChild("karakterbonus")
		self.istatistik		= self.GetChild("killbonus")
		self.karakterbonuspage		= self.GetChild("karakterbonuspage")
		self.istatistikpage		= self.GetChild("killbonuspage")
		self.karakterbonus.SAFE_SetEvent(self.karakterbonussayfa)
		self.istatistik.SAFE_SetEvent(self.istatistiksayfa)
		self.istatistikpage.Hide()

		self.ScrollBar.SetScrollEvent(ui.__mem_func__(self.OnScroll))
		
		## Ãâ·ÂµÇ´Â UI ÃÖ´ë ¼ýÀÚ
		self.UI_MAX_COUNT = 13
		self.UI_MAX_VIEW_COUNT = 6
		
		## UI KEY & VALUE
		self.INFO_TEXT	= 0
		self.INFO_TOOLTIP = 1
		self.INFO_VALUE	= 2
		self.CATEGORY_STARTLINE	= -1
		self.CATEGORY_ENDLINE	= -2
		
		## Child ¼ÂÆÃ
		self.labelList		= []
		self.labelValueList	= []
		self.labelTextList	= []
		self.horizonBarList	= []
		self.horizonBarNameList = []
		
		for i in xrange(self.UI_MAX_COUNT):
			self.labelList.append( self.GetChild("label%s"%i) )
			self.labelValueList.append( self.GetChild("labelvalue%s"%i) )
			self.labelTextList.append( self.GetChild("labelname%s"%i) )
			self.horizonBarList.append( self.GetChild("horizontalbar%s"%i) )
			self.horizonBarNameList.append( self.GetChild("horizontalbarName%s"%i) )
			
		for i in xrange(self.UI_MAX_COUNT):
			self.labelTextList[i].SetShowToolTipEvent( ui.__mem_func__(self.__ButtonOverIn), i )
			self.labelTextList[i].SetHideToolTipEvent( ui.__mem_func__(self.__ButtonOverOut), i )
			
		self.__Initialize()
		
 	def __Initialize(self):
		self.InfoList = []
		self.titleBar = 0

		self.bosses_kills_obj = self.GetChild("bosses_kills")
		self.stones_kills_obj = self.GetChild("stones_kills")

		self.InfoList.append( [ "|cffFFC125PvM Bonuslar", "", self.CATEGORY_STARTLINE ] )
		self.InfoList.append( [ localeInfo.DETAILS_5, localeInfo.DETAILS_TOOLTIP_5, 53 ] )
		self.InfoList.append( [ "Metinlere Kar. Güç", "Metinlere Kar. Güç", 161 ] )
		self.InfoList.append( [ "Patronlara Kar. Güç", "Patronlara Kar. Güç", 162 ] )
		self.InfoList.append( [ "Zindan Kar. Güç", "Zindan Kar. Güç", 172 ] )
		self.InfoList.append( [ localeInfo.DETAILS_4, localeInfo.DETAILS_TOOLTIP_4, 47 ] )

		self.InfoList.append( [ "|cffFFC125PvP Bonuslar", "", self.CATEGORY_STARTLINE ] )
		self.InfoList.append( [ localeInfo.DETAILS_1, localeInfo.DETAILS_TOOLTIP_1, 43 ] )
		self.InfoList.append( [ localeInfo.DETAILS_36, localeInfo.DETAILS_TOOLTIP_36, 54 ] )
		self.InfoList.append( [ localeInfo.DETAILS_41, localeInfo.DETAILS_TOOLTIP_41, 59 ] )
		self.InfoList.append( [ localeInfo.DETAILS_37, localeInfo.DETAILS_TOOLTIP_37, 55 ] )
		self.InfoList.append( [ localeInfo.DETAILS_42, localeInfo.DETAILS_TOOLTIP_42, 60 ] )
		self.InfoList.append( [ localeInfo.DETAILS_38, localeInfo.DETAILS_TOOLTIP_38, 56 ] )
		self.InfoList.append( [ localeInfo.DETAILS_43, localeInfo.DETAILS_TOOLTIP_43, 61 ] )
		self.InfoList.append( [ localeInfo.DETAILS_39, localeInfo.DETAILS_TOOLTIP_39, 57 ] )
		self.InfoList.append( [ localeInfo.DETAILS_44, localeInfo.DETAILS_TOOLTIP_44, 62 ] )

		self.InfoList.append( [ "|cffFFC125Toplu PvP Bonuslar", "", self.CATEGORY_STARTLINE ] )
		self.InfoList.append( [ "Tüm Kar. Karþý Güç", "Tüm Kar. Karþý Güç", 165 ] )
		self.InfoList.append( [ "Tüm Kar. Karþý Sav.", "Tüm Kar. Karþý Sav.", 166 ] )

		self.InfoList.append( [ "|cffFFC125Hasar/Güç Bonuslarý", "", self.CATEGORY_STARTLINE ] )
		self.InfoList.append( [ localeInfo.DETAILS_14, localeInfo.DETAILS_TOOLTIP_14, 122 ] )
		self.InfoList.append( [ localeInfo.DETAILS_16, localeInfo.DETAILS_TOOLTIP_16, 121 ] )
		self.InfoList.append( [ localeInfo.DETAILS_20, localeInfo.DETAILS_TOOLTIP_20, 40 ] )
		self.InfoList.append( [ localeInfo.DETAILS_21, localeInfo.DETAILS_TOOLTIP_21, 41 ] )

		## ??????Z ???
		self.InfoList.append( [ "|cffFFC125Direnç/Savunma Bonuslarý", "", self.CATEGORY_STARTLINE ] )
		self.InfoList.append( [ localeInfo.DETAILS_46, localeInfo.DETAILS_TOOLTIP_46, 69 ] )
		self.InfoList.append( [ localeInfo.DETAILS_47, localeInfo.DETAILS_TOOLTIP_47, 70 ] )
		self.InfoList.append( [ localeInfo.DETAILS_48, localeInfo.DETAILS_TOOLTIP_48, 71 ] )
		self.InfoList.append( [ localeInfo.DETAILS_50, localeInfo.DETAILS_TOOLTIP_50, 72 ] )
		self.InfoList.append( [ localeInfo.DETAILS_52, localeInfo.DETAILS_TOOLTIP_52, 74 ] )
		self.InfoList.append( [ localeInfo.DETAILS_15, localeInfo.DETAILS_TOOLTIP_15, 124 ] )
		self.InfoList.append( [ localeInfo.DETAILS_17, localeInfo.DETAILS_TOOLTIP_17, 123 ] )
		self.InfoList.append( [ localeInfo.DETAILS_76, localeInfo.DETAILS_TOOLTIP_76, 77 ] )
		self.InfoList.append( [ "", "", self.CATEGORY_ENDLINE ] )

		self.Diff = len(self.InfoList) - self.UI_MAX_COUNT
		stepSize = 1.0 / self.Diff
		self.ScrollBar.SetScrollStep( stepSize )
		self.ScollPos = 0
		self.RefreshLabel()
		
	def Show(self):
		ui.ScriptWindow.Show(self)
		self.SetTop()
		
	def Close(self):
		self.Hide()
	
	def AdjustPosition(self, x, y):
		self.SetPosition(x + self.Width, y)
	
	def OnScroll(self):
		self.RefreshLabel()


	def Refresh(self):
		kd_zero_fix = 0
		if constInfo.KILL_STATISTICS_DATA[4] == 0:
			kd_zero_fix = 1
			
	
		self.bosses_kills_obj.SetText("%i" % constInfo.KILL_STATISTICS_DATA[7])
		self.stones_kills_obj.SetText("%i" % constInfo.KILL_STATISTICS_DATA[8])
		
	def OnUpdate(self):
		self.Refresh()

	def RefreshLabel(self):
		self.ScollPos = int(self.ScrollBar.GetPos() * self.Diff)
		
		for i in xrange(self.UI_MAX_COUNT) :
			idx = i + self.ScollPos
			
			text = self.InfoList[idx][self.INFO_TEXT]
			type = self.InfoList[idx][self.INFO_VALUE]
			
			if type == self.CATEGORY_STARTLINE:
				self.__LabelTitleLine(i, text)
			elif type == self.CATEGORY_ENDLINE:
				self.__EmptyLine(i)
			else:
				value = player.GetStatus(type)
				
				self.__LabelLine(i, text, value)


	def karakterbonussayfa(self):
		self.istatistikpage.Hide()
		self.karakterbonuspage.Show()

	def istatistiksayfa(self):
		self.istatistikpage.Show()
		self.karakterbonuspage.Hide()

	def __LabelTitleLine(self, idx, text):
		self.labelList[idx].Hide()
		self.labelTextList[idx].Hide()
		self.horizonBarList[idx].Show()
		self.horizonBarNameList[idx].SetText( text )
				
	def __EmptyLine(self, idx):
		self.labelList[idx].Hide()
		self.labelTextList[idx].Hide()
		self.horizonBarList[idx].Hide()
		
	def __LabelLine(self, idx, text, value):
		self.labelList[idx].Show()
		self.labelTextList[idx].Show()
		self.horizonBarList[idx].Hide()
		
		self.labelTextList[idx].SetText( text )
		self.labelValueList[idx].SetText( str(value) )
		
	def __ButtonOverIn(self, i):
		idx = i + self.ScollPos
		tooltip = self.InfoList[idx][self.INFO_TOOLTIP]
		
		arglen = len(str(tooltip))
		pos_x, pos_y = wndMgr.GetMousePosition()
		
		self.toolTip.ClearToolTip()
		self.toolTip.SetThinBoardSize(11 * arglen)
		self.toolTip.SetToolTipPosition(pos_x + 50, pos_y + 50)
		self.toolTip.AppendTextLine(tooltip, 0xffffff00)
		self.toolTip.Show()		
		
	def __ButtonOverOut(self, idx):
		self.toolTip.Hide()
	def OnRunMouseWheel(self, nLen):
		if self.ScrollBar.IsShow():
			if nLen > 0:
				self.ScrollBar.OnUp()
			else:
				self.ScrollBar.OnDown()


		
	def OnTop(self):
		if self.uiCharacterStatus:
			self.uiCharacterStatus.SetTop()
			
	def OnUpdate(self):
		self.bosses_kills_obj.SetText("%i" % constInfo.KILL_STATISTICS_DATA[2])
		self.stones_kills_obj.SetText("%i" % constInfo.KILL_STATISTICS_DATA[3])
