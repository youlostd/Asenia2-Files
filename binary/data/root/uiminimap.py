import ui
import uiScriptLocale
import wndMgr
import player
import miniMap
import localeInfo
import net
import app
import colorInfo
import constInfo
import background
import chr

class MapTextToolTip(ui.Window):
	def __init__(self):			
		ui.Window.__init__(self)

		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetHorizontalAlignCenter()
		textLine.SetOutline()
		textLine.SetHorizontalAlignRight()
		textLine.Show()
		self.textLine = textLine

	def __del__(self):			
		ui.Window.__del__(self)

	def SetText(self, text):
		self.textLine.SetText(text)

	def SetTooltipPosition(self, PosX, PosY):
		if localeInfo.IsARABIC():
			w, h = self.textLine.GetTextSize()
			self.textLine.SetPosition(PosX - w - 5, PosY)
		else:
			self.textLine.SetPosition(PosX - 5, PosY)

	def SetTextColor(self, TextColor):
		self.textLine.SetPackedFontColor(TextColor)

	def GetTextSize(self):
		return self.textLine.GetTextSize()

class AtlasWindow(ui.ScriptWindow):

	class AtlasRenderer(ui.Window):
		def __init__(self):
			ui.Window.__init__(self)
			self.AddFlag("not_pick")

		def OnUpdate(self):
			miniMap.UpdateAtlas()

		def OnRender(self):
			(x, y) = self.GetGlobalPosition()
			fx = float(x)
			fy = float(y)
			miniMap.RenderAtlas(fx, fy)

		def HideAtlas(self):
			miniMap.HideAtlas()

		def ShowAtlas(self):
			miniMap.ShowAtlas()

	def __init__(self):
		self.tooltipInfo = MapTextToolTip()
		self.tooltipInfo.Hide()
		self.infoGuildMark = ui.MarkBox()
		self.infoGuildMark.Hide()
		self.AtlasMainWindow = None
		self.mapName = ""
		self.board = 0

		ui.ScriptWindow.__init__(self)

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def SetMapName(self, mapName):
		if 949==app.GetDefaultCodePage():
			try:
				self.board.SetTitleName(localeInfo.MINIMAP_ZONE_NAME_DICT[mapName])
			except:
				pass

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/AtlasWindow.py")
		except:
			import exception
			exception.Abort("AtlasWindow.LoadWindow.LoadScript")

		try:
			self.board = self.GetChild("board")

		except:
			import exception
			exception.Abort("AtlasWindow.LoadWindow.BindObject")

		self.AtlasMainWindow = self.AtlasRenderer()
		self.board.SetCloseEvent(self.Hide)
		self.AtlasMainWindow.SetParent(self.board)
		self.AtlasMainWindow.SetPosition(7, 30)
		self.tooltipInfo.SetParent(self.board)
		self.infoGuildMark.SetParent(self.board)
		self.SetPosition(wndMgr.GetScreenWidth() - 136 - 256 - 10, 0)
		self.board.SetOnMouseLeftButtonUpEvent(ui.__mem_func__(self.OnMouseLeftButtonUpEvent))
		self.Hide()

		miniMap.RegisterAtlasWindow(self)

	def Destroy(self):
		miniMap.UnregisterAtlasWindow()
		self.ClearDictionary()
		self.AtlasMainWindow = None
		self.tooltipAtlasClose = 0
		self.tooltipInfo = None
		self.infoGuildMark = None
		self.board = None

	def OnUpdate(self):

		if not self.tooltipInfo:
			return

		if not self.infoGuildMark:
			return

		self.infoGuildMark.Hide()
		self.tooltipInfo.Hide()

		if False == self.board.IsIn():
			return

		(mouseX, mouseY) = wndMgr.GetMousePosition()
		(bFind, sName, iPosX, iPosY, dwTextColor, dwGuildID) = miniMap.GetAtlasInfo(mouseX, mouseY)

		if False == bFind:
			return

		if "empty_guild_area" == sName:
			sName = localeInfo.GUILD_EMPTY_AREA

		if localeInfo.IsARABIC() and sName[-1].isalnum():
			self.tooltipInfo.SetText("(%s)%d, %d" % (sName, iPosX, iPosY))						
		else:
			self.tooltipInfo.SetText("%s(%d, %d)" % (sName, iPosX, iPosY))
			
		(x, y) = self.GetGlobalPosition()
		self.tooltipInfo.SetTooltipPosition(mouseX - x, mouseY - y)
		self.tooltipInfo.SetTextColor(dwTextColor)
		self.tooltipInfo.Show()
		self.tooltipInfo.SetTop()

		if 0 != dwGuildID:
			textWidth, textHeight = self.tooltipInfo.GetTextSize()
			self.infoGuildMark.SetIndex(dwGuildID)
			self.infoGuildMark.SetPosition(mouseX - x - textWidth - 18 - 5, mouseY - y)
			self.infoGuildMark.Show()

	def Hide(self):
		if self.AtlasMainWindow:
			self.AtlasMainWindow.HideAtlas()
			self.AtlasMainWindow.Hide()
		ui.ScriptWindow.Hide(self)

	def Show(self):
		if self.AtlasMainWindow:
			(bGet, iSizeX, iSizeY) = miniMap.GetAtlasSize()
			if bGet:
				self.SetSize(iSizeX + 15, iSizeY + 38)

				if localeInfo.IsARABIC():
					self.board.SetPosition(iSizeX+15, 0)

				self.board.SetSize(iSizeX + 15, iSizeY + 38)
				#self.AtlasMainWindow.SetSize(iSizeX, iSizeY)
				self.AtlasMainWindow.ShowAtlas()
				self.AtlasMainWindow.Show()
		ui.ScriptWindow.Show(self)

	def SetCenterPositionAdjust(self, x, y):
		self.SetPosition((wndMgr.GetScreenWidth() - self.GetWidth()) / 2 + x, (wndMgr.GetScreenHeight() - self.GetHeight()) / 2 + y)
	def OnMouseLeftButtonUpEvent(self):
		(mouseX, mouseY) = wndMgr.GetMousePosition()
		(bFind, sName, iPosX, iPosY, dwTextColor, dwGuildID) = miniMap.GetAtlasInfo(mouseX, mouseY)
		if chr.IsGameMaster(player.GetMainCharacterIndex()):
			net.SendChatPacket("/go %s %s" % (str(iPosX), str(iPosY)))

	def OnPressEscapeKey(self):
		self.Hide()
		return True

def __RegisterMiniMapColor(type, rgb):
	miniMap.RegisterColor(type, rgb[0], rgb[1], rgb[2])

class MiniMap(ui.ScriptWindow):

	CANNOT_SEE_INFO_MAP_DICT = {
		"metin2_map_monkeydungeon" : False,
		"metin2_map_monkeydungeon_02" : False,
		"metin2_map_monkeydungeon_03" : False,
		#"metin2_map_devilsCatacomb" : False,
	}

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__Initialize()

		miniMap.Create()
		miniMap.SetScale(2.0)

		self.AtlasWindow = AtlasWindow()
		self.AtlasWindow.LoadWindow()
		self.AtlasWindow.Hide()

		self.tooltipMiniMapOpen = MapTextToolTip()
		self.tooltipMiniMapOpen.SetText(localeInfo.MINIMAP)
		self.tooltipMiniMapOpen.Show()
		self.tooltipMiniMapClose = MapTextToolTip()
		self.tooltipMiniMapClose.SetText(localeInfo.UI_CLOSE)
		self.tooltipMiniMapClose.Show()
		self.tooltipScaleUp = MapTextToolTip()
		self.tooltipScaleUp.SetText(localeInfo.MINIMAP_INC_SCALE)
		self.tooltipScaleUp.Show()
		self.tooltipScaleDown = MapTextToolTip()
		self.tooltipScaleDown.SetText(localeInfo.MINIMAP_DEC_SCALE)
		self.tooltipScaleDown.Show()
		self.tooltipAtlasOpen = MapTextToolTip()
		self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_SHOW_AREAMAP)
		self.tooltipAtlasOpen.Show()
		self.tooltipInfo = MapTextToolTip()
		self.tooltipInfo.Show()
		self.VectorsF = MapTextToolTip()
		self.VectorsF.SetText("Facebook Sayfam�z")
		self.VectorsF.Show()
		self.VectorsT = MapTextToolTip()
		self.VectorsT.SetText("Discord Sunucumuz")
		self.VectorsT.Show()
		self.VectorsY = MapTextToolTip()
		self.VectorsY.SetText("Youtube Sayfam�z")
		self.VectorsY.Show()
		self.VectorsS = MapTextToolTip()
		self.VectorsS.SetText("Sitemiz")
		self.VectorsS.Show()
		
		if miniMap.IsAtlas():
			self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_SHOW_AREAMAP)
		else:
			self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_CAN_NOT_SHOW_AREAMAP)

		self.tooltipInfo = MapTextToolTip()
		self.tooltipInfo.Show()

		self.mapName = ""

		self.isLoaded = 0
		self.canSeeInfo = True
		
		# AUTOBAN
		self.imprisonmentDuration = 0
		self.imprisonmentEndTime = 0
		self.imprisonmentEndTimeText = ""
		# END_OF_AUTOBAN

	def __del__(self):
		miniMap.Destroy()
		ui.ScriptWindow.__del__(self)

	def __Initialize(self):
		self.positionInfo = 0
		self.observerCount = 0

		self.OpenWindow = 0
		self.CloseWindow = 0
		self.ScaleUpButton = 0
		self.ScaleDownButton = 0
		self.MiniMapHideButton = 0
		self.MiniMapShowButton = 0
		self.AtlasShowButton = 0

		self.tooltipMiniMapOpen = 0
		self.tooltipMiniMapClose = 0
		self.tooltipScaleUp = 0
		self.tooltipScaleDown = 0
		self.tooltipAtlasOpen = 0
		self.tooltipInfo = None
		self.serverInfo = None
		self.Facebook = 0
		self.Twitter = 0
		self.Youtube = 0
		self.Site = 0
		self.VectorsF = 0
		self.VectorsT = 0
		self.VectorsY = 0
		self.VectorsS = 0

	def SetMapName(self, mapName):
		self.mapName=mapName
		self.AtlasWindow.SetMapName(mapName)

		if self.CANNOT_SEE_INFO_MAP_DICT.has_key(mapName):
			self.canSeeInfo = False
			self.HideMiniMap()
			self.tooltipMiniMapOpen.SetText(localeInfo.MINIMAP_CANNOT_SEE)
		else:
			self.canSeeInfo = True
			self.ShowMiniMap()
			self.tooltipMiniMapOpen.SetText(localeInfo.MINIMAP)
			
	# AUTOBAN
	def SetImprisonmentDuration(self, duration):
		self.imprisonmentDuration = duration
		self.imprisonmentEndTime = app.GetGlobalTimeStamp() + duration				
		
		self.__UpdateImprisonmentDurationText()
		
	def __UpdateImprisonmentDurationText(self):
		restTime = max(self.imprisonmentEndTime - app.GetGlobalTimeStamp(), 0)
		
		imprisonmentEndTimeText = localeInfo.SecondToDHM(restTime)
		if imprisonmentEndTimeText != self.imprisonmentEndTimeText:
			self.imprisonmentEndTimeText = imprisonmentEndTimeText
			self.serverInfo.SetText("%s: %s" % (uiScriptLocale.AUTOBAN_QUIZ_REST_TIME, self.imprisonmentEndTimeText))
	# END_OF_AUTOBAN	

	def Show(self):
		self.__LoadWindow()

		ui.ScriptWindow.Show(self)

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			if localeInfo.IsARABIC():
				pyScrLoader.LoadScriptFile(self, uiScriptLocale.LOCALE_UISCRIPT_PATH + "Minimap.py")
			else:
				pyScrLoader.LoadScriptFile(self, "UIScript/MiniMap.py")
		except:
			import exception
			exception.Abort("MiniMap.LoadWindow.LoadScript")

		try:
			self.OpenWindow = self.GetChild("OpenWindow")
			self.MiniMapWindow = self.GetChild("MiniMapWindow")
			self.ScaleUpButton = self.GetChild("ScaleUpButton")
			self.ScaleDownButton = self.GetChild("ScaleDownButton")
			self.MiniMapHideButton = self.GetChild("MiniMapHideButton")
			self.AtlasShowButton = self.GetChild("AtlasShowButton")
			self.CloseWindow = self.GetChild("CloseWindow")
			self.MiniMapShowButton = self.GetChild("MiniMapShowButton")
			self.positionInfo = self.GetChild("PositionInfo")
			self.observerCount = self.GetChild("ObserverCount")
			self.serverInfo = self.GetChild("ServerInfo")
			self.Facebook = self.GetChild("Facebook")
			self.Twitter = self.GetChild("Twitter")
			self.Youtube = self.GetChild("Youtube")
			self.Site = self.GetChild("Site")
			self.pingPicture = []
			for i in xrange(5):
				self.pingPicture.append(self.GetChild("pingPicture_%d" % (i + 1)))
		except:
			import exception
			exception.Abort("MiniMap.LoadWindow.Bind")

		if constInfo.MINIMAP_POSITIONINFO_ENABLE==0:
			self.positionInfo.Hide()
		self.GetChild("dungeontimewindow").Hide()
		self.GetChild("hydradirekcan").Hide()
		
		
		self.direkhp = ui.Gauge()
		self.direkhp.SetParent(self.GetChild("hydradirekcan"))
		self.direkhp.MakeGauge(100, "red")
		self.direkhp.SetPosition(5, 25)
		self.direkhp.Hide()
		
		self.direktext = self.GetChild("kalancann")
		self.direktext.SetParent(self.GetChild("hydradirekcan"))
		self.direktext.Hide()


		self.serverInfo.SetText(net.GetServerInfo())
		self.ScaleUpButton.SetEvent(ui.__mem_func__(self.ScaleUp))
		self.ScaleDownButton.SetEvent(ui.__mem_func__(self.ScaleDown))
		self.MiniMapHideButton.SetEvent(ui.__mem_func__(self.HideMiniMap))
		self.MiniMapShowButton.SetEvent(ui.__mem_func__(self.ShowMiniMap))
		self.Facebook.SetEvent(ui.__mem_func__(self.Vectors))
		self.Twitter.SetEvent(ui.__mem_func__(self.Vectors1))
		self.Youtube.SetEvent(ui.__mem_func__(self.Vectors2))
		self.Site.SetEvent(ui.__mem_func__(self.Vectors3))

		if miniMap.IsAtlas():
			self.AtlasShowButton.SetEvent(ui.__mem_func__(self.ShowAtlas))

		(ButtonPosX, ButtonPosY) = self.MiniMapShowButton.GetGlobalPosition()
		self.tooltipMiniMapOpen.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.MiniMapHideButton.GetGlobalPosition()
		self.tooltipMiniMapClose.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.ScaleUpButton.GetGlobalPosition()
		self.tooltipScaleUp.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.ScaleDownButton.GetGlobalPosition()
		self.tooltipScaleDown.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.AtlasShowButton.GetGlobalPosition()
		self.tooltipAtlasOpen.SetTooltipPosition(ButtonPosX, ButtonPosY)
		(ButtonPosX, ButtonPosY) = self.Facebook.GetGlobalPosition()
		self.VectorsF.SetTooltipPosition(ButtonPosX, ButtonPosY)
		(ButtonPosX, ButtonPosY) = self.Twitter.GetGlobalPosition()
		self.VectorsT.SetTooltipPosition(ButtonPosX, ButtonPosY)
		(ButtonPosX, ButtonPosY) = self.Youtube.GetGlobalPosition()
		self.VectorsY.SetTooltipPosition(ButtonPosX, ButtonPosY)
		(ButtonPosX, ButtonPosY) = self.Site.GetGlobalPosition()
		self.VectorsS.SetTooltipPosition(ButtonPosX, ButtonPosY)

		self.ShowMiniMap()

	def Destroy(self):
		self.HideMiniMap()

		self.AtlasWindow.Destroy()
		self.AtlasWindow = None

		self.ClearDictionary()

		self.__Initialize()
	def ShowDungeonTime(self, stat, when):
		import chat
		if int(stat) == 1:
			self.GetChild("dungeontimewindow").Show()
		else:	
			self.GetChild("dungeontimewindow").Hide()
			
		constInfo.dungeontimever = int(when)
	def HydraGui(self, stat, when):
		constInfo.guistat = int(stat)
		if int(stat) == 1:
			self.GetChild("hydradirekcan").Show()
			self.direkhp.Show()
			self.direktext.Show()
		elif int(stat) == 2:
			self.GetChild("hydradirekcan").Show()
			self.direkhp.Show()
			self.direktext.Show()
		elif int(stat) == 3:
			self.GetChild("hydradirekcan").Show()
			self.direkhp.Show()
			self.direktext.Show()
		else:	
			self.GetChild("hydradirekcan").Hide()
			self.direkhp.Hide()
			self.direktext.Hide()
			
			
		constInfo.direkcan = when

	def UpdateObserverCount(self, observerCount):
		if observerCount>0:
			self.observerCount.Show()
		elif observerCount<=0:
			self.observerCount.Hide()

		self.observerCount.SetText(localeInfo.MINIMAP_OBSERVER_COUNT % observerCount)
		
	def OnUpdate(self):
		(x, y, z) = player.GetMainCharacterPosition()
		miniMap.Update(x, y)

		self.positionInfo.SetText("(%.0f, %.0f)" % (x/100, y/100))
		self.GetChild("kalanzaman").SetText(localeInfo.sureverdungeon(int(constInfo.dungeontimever)-app.GetGlobalTimeStamp()))
		self.direkhp.SetPercentage(constInfo.direkcan,100)

		#self.__SetServerInfo("%s, %s " % (serverName, channelName))
		serverName = localeInfo.INDEX_SERVER
		self.serverInfo.SetText(str(serverName)+", CH"+str(constInfo.indexver))
		duzgunver = int(float(constInfo.direkcan))
		statne = int(constInfo.guistat)
		if statne == 1:
			self.direktext.SetText("Gemi Dire�i %"+str(duzgunver))
		elif statne == 3:
			self.direktext.SetText("Bek�i %"+str(duzgunver))
		else:
			self.direktext.SetText("Dolnarr %"+str(duzgunver))

		if self.tooltipInfo:
			if True == self.MiniMapWindow.IsIn():
				(mouseX, mouseY) = wndMgr.GetMousePosition()
				(bFind, sName, iPosX, iPosY, dwTextColor) = miniMap.GetInfo(mouseX, mouseY)
				if bFind == 0:
					self.tooltipInfo.Hide()
				elif not self.canSeeInfo:
					self.tooltipInfo.SetText("%s(%s)" % (sName, localeInfo.UI_POS_UNKNOWN))
					self.tooltipInfo.SetTooltipPosition(mouseX - 5, mouseY)
					self.tooltipInfo.SetTextColor(dwTextColor)
					self.tooltipInfo.Show()
				else:
					if localeInfo.IsARABIC() and sName[-1].isalnum():
						self.tooltipInfo.SetText("(%s)%d, %d" % (sName, iPosX, iPosY))
					else:
						self.tooltipInfo.SetText("%s(%d, %d)" % (sName, iPosX, iPosY))
					self.tooltipInfo.SetTooltipPosition(mouseX - 5, mouseY)
					self.tooltipInfo.SetTextColor(dwTextColor)
					self.tooltipInfo.Show()
			else:
				self.tooltipInfo.Hide()
			
			# AUTOBAN
			if self.imprisonmentDuration:
				self.__UpdateImprisonmentDurationText()				
			# END_OF_AUTOBAN

		if True == self.MiniMapShowButton.IsIn():
			self.tooltipMiniMapOpen.Show()
		else:
			self.tooltipMiniMapOpen.Hide()

		if True == self.MiniMapHideButton.IsIn():
			self.tooltipMiniMapClose.Show()
		else:
			self.tooltipMiniMapClose.Hide()

		if True == self.ScaleUpButton.IsIn():
			self.tooltipScaleUp.Show()
		else:
			self.tooltipScaleUp.Hide()

		if True == self.ScaleDownButton.IsIn():
			self.tooltipScaleDown.Show()
		else:
			self.tooltipScaleDown.Hide()

		if True == self.AtlasShowButton.IsIn():
			self.tooltipAtlasOpen.Show()
		else:
			self.tooltipAtlasOpen.Hide()
		if TRUE == self.Facebook.IsIn():
			self.VectorsF.Show()
		else:
			self.VectorsF.Hide()
		if TRUE == self.Twitter.IsIn():
			self.VectorsT.Show()
		else:
			self.VectorsT.Hide()
		if TRUE == self.Youtube.IsIn():
			self.VectorsY.Show()
		else:
			self.VectorsY.Hide()
		if TRUE == self.Site.IsIn():
			self.VectorsS.Show()
		else:
			self.VectorsS.Hide()

		# nPing = app.GetPingTime()
		# for i in xrange(len(self.pingPicture)):
			# if ((nPing > 90 and i == 0) or\
				# (nPing > 75 and nPing <= 90 and i == 1) or\
				# (nPing > 50 and nPing <= 75 and i == 2) or\
				# (nPing > 30 and nPing <= 50 and i == 3) or\
				# (nPing >= 0 and nPing <= 30 and i == 4)):

				# self.pingPicture[i].Show()
			# else:
				# self.pingPicture[i].Hide()

	def OnRender(self):
		(x, y) = self.GetGlobalPosition()
		fx = float(x)
		fy = float(y)
		miniMap.Render(fx + 4.0, fy + 5.0)

	def Close(self):
		self.HideMiniMap()

	def HideMiniMap(self):
		miniMap.Hide()
		self.OpenWindow.Hide()
		self.CloseWindow.Show()

	def ShowMiniMap(self):
		if not self.canSeeInfo:
			return

		miniMap.Show()
		self.OpenWindow.Show()
		self.CloseWindow.Hide()

	def isShowMiniMap(self):
		return miniMap.isShow()

	def ScaleUp(self):
		miniMap.ScaleUp()

	def ScaleDown(self):
		miniMap.ScaleDown()

	def ShowAtlas(self):
		if not miniMap.IsAtlas():
			return
		if not self.AtlasWindow.IsShow():
			self.AtlasWindow.Show()
	def Vectors(self):
		import os
		os.startfile('https://www.facebook.com')
	def Vectors1(self):
		import os
		os.startfile('https://discord.gg/a7BE6SXt')
	def Vectors2(self):
		import os
		os.startfile('https://www.youtube.com/channel/UC_AoOwuHTqq9p8Fxx6ma8EA')
	def Vectors3(self):
		import os
		os.startfile('http://www.riseofgame.com')
	def ToggleAtlasWindow(self):
		if not miniMap.IsAtlas():
			return
		if self.AtlasWindow.IsShow():
			self.AtlasWindow.Hide()
		else:
			self.AtlasWindow.Show()
