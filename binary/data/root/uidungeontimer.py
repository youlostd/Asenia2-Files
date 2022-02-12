import app

import ui
import uiScriptLocale
import uiToolTip
import mouseModule
import constInfo
import localeInfo
import skill



import net
import player

import item
import chr
import effect
import dbg
import background

from _weakref import proxy
from itertools import islice
import math
import constInfo

class DungeonTimer(ui.ScriptWindow):

	class SearchResultItem(ui.Window):

		def __init__(self, parent, index):
			ui.Window.__init__(self)
			
			self.parent = parent
			
			self.isLoad = True
			self.isSelected = False
			
			self.index = index
			


			self.SetParent(parent)
			self.InitItem()

			
		def InitItem(self):
			startX = 18
			yPos = 3
			
			self.dungeonImage = ui.MakeImageBox(self, "dungeontimer/normal.tga", 3, yPos+2)
			self.dungeonImage.SAFE_SetStringEvent("MOUSE_LEFT_UP",self.__OnSelect)
			self.dungeonImage.SetTop()
			self.dungeonImage.Show()
			
			self.text = ui.TextLine()
			self.text.SetParent(self)
			self.text.SetPosition(startX+67-40, yPos+12)
			# self.text.SetHorizontalAlignCenter()
			self.text.Show()
			
			
			self.count = ui.TextLine()
			self.count.SetParent(self)
			self.count.SetPosition(startX+303-90, yPos+12)
			self.count.SetHorizontalAlignCenter()
			self.count.Show()
			
			self.bossImage = ui.ImageBox()
			self.bossImage.SetParent(self.dungeonImage)
			self.bossImage.SetPosition(1, 1)
			self.bossImage.Show()

						
			self.SetSize(self.dungeonImage.GetWidth(), self.dungeonImage.GetHeight())
			

			
		def SetDungeonName(self, name):
			self.text.SetText(name)

		def SetDungeonTime(self, count):
			self.count.SetText(count)
			
		def SetBossImage(self, boss):
			self.bossImage.LoadImage(boss)
		


			

		
		
		def __OnSelect(self):
			self.parent.OnSearchResultItemSelect(self.index)

		def Select(self):

			self.isSelected = True
			self.isLoad = True

		def UnSelect(self):
			self.isSelected = False
			self.isLoad = True
			


		def OnUpdate(self):
			
			if player.GetStatus(player.LEVEL) < int(constInfo.dungeontimerinfo[int(self.index)][3]):
				self.SetDungeonTime(constInfo.kirmizirenk + localeInfo.DUNGEON_INFO_TEXT10)
			elif player.GetStatus(player.LEVEL) > int(constInfo.dungeontimerinfo[int(self.index)][4]):
				self.SetDungeonTime(constInfo.kirmizirenk + localeInfo.DUNGEON_INFO_TEXT11)
			else:
				if int(constInfo.dungeontimerinfo[int(self.index)][13]) > app.GetGlobalTimeStamp():
					self.SetDungeonTime(constInfo.sarirenk + localeInfo.sureverdungeon(int(constInfo.dungeontimerinfo[int(self.index)][13])-app.GetGlobalTimeStamp()))
				else:
					self.SetDungeonTime(constInfo.yesilrenk + "Uygun")
				
			

		def OnRender(self):
			if self.isLoad:
				if self.isSelected:
					self.dungeonImage.LoadImage("dungeontimer/active.tga")
				else:
					self.dungeonImage.LoadImage("dungeontimer/normal.tga")
				self.isLoad = False


	def __init__(self):
		ui.ScriptWindow.__init__(self)
		

		
		
		self.selectedItemIndex = -1
		self.board = None
		self.secilen = None

		self.searchResultItems = []

		self.itemDataList = []
		
		self.currentPage = 1
		self.pageCount = 1
		self.perPage = 10
		self.itemCount = 0
		
		self.LoadWindow()
		



	def __del__(self):
		self.Destroy()
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/dungeontimer.py")
		except:
			import exception
			exception.Abort("DungeonTimer.LoadDialog.LoadScript")
#15
		try:
			GetObject=self.GetChild
			self.board = GetObject("board")
			self.paravarcarevar = GetObject("Amakafamiznasilguzel")
			self.dungeontype = GetObject("dungeontype")
			self.dungeontype2 = GetObject("dungeontype2")
			self.dungeonlevel = GetObject("dungeonlevel")
			self.dungeongroup = GetObject("dungeongroup")
			self.dungeoncd = GetObject("dungeoncd")
			self.dungeoncd2 = GetObject("dungeoncd2")
			self.dungeongiris = GetObject("dungeongiris")
			self.dungeonguclu = GetObject("dungeonguclu")
			self.dungeondirenc = GetObject("dungeondirenc")
			self.patron_name = GetObject("ModelName_text")
			self.mapname = GetObject("mapname")
			self.girisslot = GetObject("girisslot")
			self.questScrollBar = self.GetChild("info_ScrollBar")
			self.tpbutton = self.GetChild("tp_button")
			self.girisitemtext = self.GetChild("girisitemtext")
			self.questShowingStartIndex = 0
			self.tpbutton.SetEvent(ui.__mem_func__(self.OnTpButton))
			self.questScrollBar.SetScrollEvent(ui.__mem_func__(self.OnQuestScroll))
			
			self.board.SetCloseEvent(ui.__mem_func__(self.__OnCloseButtonClick))

			self.renderTarget = ui.RenderTarget()
			self.renderTarget.SetParent(self.paravarcarevar)
			self.renderTarget.SetSize(190,220)
			self.renderTarget.SetPosition(2,40)
			self.renderTarget.SetRenderTarget(2)
			self.renderTarget.SetAutoRotation(0.0)
			self.renderTarget.Show()

		except:
			import exception
			exception.Abort("DungeonTimer.LoadDialog.BindObject")


	def Destroy(self):
		self.ClearDictionary()
		self.searchResultItems[:] = [] 
		self.titleBar = None
		self.questScrollBar = None
		self.questShowingStartIndex = None
		
		
	def OnQuestScroll(self):
		questCount = len(constInfo.dungeontimerinfo)
		scrollLineCount = max(0, questCount - 8)
		startIndex = int(scrollLineCount * self.questScrollBar.GetPos())

		if startIndex != self.questShowingStartIndex:
			self.questShowingStartIndex = startIndex
			self.RefreshInfo()

		
	def RefreshInfo(self):
		questCount = len(constInfo.dungeontimerinfo)
		questRange = range(8)
		self.searchResultItems[:] = []
		
		if questCount > 8:
			self.questScrollBar.Show()
		else:
			self.questScrollBar.Hide()

		
		
		basePos = 38
		for i in range(0+self.questShowingStartIndex, 8+self.questShowingStartIndex):
			resultItem = DungeonTimer.SearchResultItem(self, i)
			resultItem.SetPosition(136-112, basePos+((i-self.questShowingStartIndex)*36))
			resultItem.SetDungeonName(constInfo.dungeontimerinfo[i][0])
			resultItem.SetDungeonTime(localeInfo.sureverdungeon(app.GetGlobalTimeStamp()-int(constInfo.dungeontimerinfo[i][13])))
			resultItem.SetBossImage(constInfo.dungeontimerinfo[i][12])
			resultItem.Show()
			
			self.searchResultItems.append(resultItem)
		
		self.Children.append(self.searchResultItems)
		# self.OnSearchResultItemSelect(0+self.questShowingStartIndex)




	def Open(self):	
		self.selectedItemIndex = -1
		self.Show()
		self.SetCenterPosition()


		basePos = 38
		for i in range(0, 8):
		#for i in xrange(len(constInfo.dungeontimerinfo)):
			resultItem = DungeonTimer.SearchResultItem(self, i)
			resultItem.SetPosition(136-112, basePos+(i*36))
			resultItem.SetDungeonName(constInfo.dungeontimerinfo[i][0])
			resultItem.SetDungeonTime(localeInfo.sureverdungeon(app.GetGlobalTimeStamp()-int(constInfo.dungeontimerinfo[i][13])))
			resultItem.SetBossImage(constInfo.dungeontimerinfo[i][12])
			resultItem.Show()
			
			self.searchResultItems.append(resultItem)
			

		self.Children.append(self.searchResultItems)
		self.OnSearchResultItemSelect(0+self.questShowingStartIndex)
		
		if len(constInfo.dungeontimerinfo) > 8:
			self.questScrollBar.Show()
		else:
			self.questScrollBar.Hide()
		
		self.questScrollBar.SetPos(0)
		
		self.RefreshInfo()
		self.OnSearchResultItemSelect(0+self.questShowingStartIndex)
		


	
	def OnUpdate(self):
		pass

	
	def OnTpButton(self):
		net.SendChatPacket("/dungeontp "+str(self.secilen)) 
		self.Close()
	
	
	def OnSearchResultItemSelect(self, index):
		if self.questShowingStartIndex > 0:
			self.selectedItemIndex = index - self.questShowingStartIndex
		else:
			self.selectedItemIndex = index
		self.secilen = index

		self.tpbutton.SetText(localeInfo.DUNGEON_INFO_WARP)
		self.girisslot.Show()
		self.girisitemtext.Show()
		map(DungeonTimer.SearchResultItem.UnSelect,  self.searchResultItems)
		self.searchResultItems[self.selectedItemIndex].Select()
		self.dungeontype.SetText(localeInfo.DUNGEON_INFO_TEXT1+str(constInfo.dungeontimerinfo[index][1]))
		self.dungeontype2.SetText(localeInfo.DUNGEON_INFO_TEXT2+str(constInfo.dungeontimerinfo[index][2]))
		self.dungeonlevel.SetText(localeInfo.DUNGEON_INFO_TEXT3+str(constInfo.dungeontimerinfo[index][3]) + " - " + str(constInfo.dungeontimerinfo[index][4]))
		self.dungeongroup.SetText(localeInfo.DUNGEON_INFO_TEXT4+str(constInfo.dungeontimerinfo[index][5]))
		self.dungeoncd.SetText(localeInfo.DUNGEON_INFO_TEXT5+str(constInfo.dungeontimerinfo[index][6]))
		self.dungeoncd2.SetText(localeInfo.DUNGEON_INFO_TEXT6+str(constInfo.dungeontimerinfo[index][7]))
		self.dungeongiris.SetText(localeInfo.DUNGEON_INFO_TEXT7+str(constInfo.dungeontimerinfo[index][8]))
		self.dungeonguclu.SetText(localeInfo.DUNGEON_INFO_TEXT8+str(constInfo.dungeontimerinfo[index][9]))
		self.dungeondirenc.SetText(localeInfo.DUNGEON_INFO_TEXT9+str(constInfo.dungeontimerinfo[index][10]))
		self.renderTarget.SetRenderTargetData(constInfo.dungeontimerinfo[index][15])
		self.patron_name.SetText(constInfo.dungeontimerinfo[index][16])

		self.mapname.SetText(str(constInfo.dungeontimerinfo[index][0]))
		if int(constInfo.dungeontimerinfo[index][11]) == 0:
			self.girisslot.SetItemSlot(0, 0)
		else:
			self.girisslot.SetItemSlot(0, int(constInfo.dungeontimerinfo[index][11]),int(constInfo.dungeontimerinfo[index][14]))
		self.girisslot.RefreshSlot()
		

	def Close(self):
		map(DungeonTimer.SearchResultItem.Hide, self.searchResultItems)
		self.Hide()


	def Clear(self):
		self.Refresh()

	def Refresh(self):
		pass

	def __OnCloseButtonClick(self):
		self.Hide()

	def OnPressEscapeKey(self):
		self.Close()

