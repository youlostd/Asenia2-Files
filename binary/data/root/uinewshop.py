import player
import net
import app
import item
import chat
import ime
import shop
import os
import dbg
import localeInfo
import time
import ui
import uiCommon
import uiToolTip
import colorinfo
import snd
import grp
import mouseModule
import wndMgr
import uiPrivateShopBuilder as uiPrivateShopBuilder
import constInfo
import uiScriptLocale
import localeInfo
import chr
c = colorinfo.CHAT_RGB_NOTICE
titleColor = ui.GenerateColor(c[0],c[1],c[2])
NEGATIVE_COLOR = grp.GenerateColor(0.9, 0.4745, 0.4627, 1.0)
POSITIVE_COLOR = grp.GenerateColor(0.6911, 0.8754, 0.7068, 1.0)
##Should be disabled becouse this is only for my client UI ##
POSITION_FIX=False
		
class DropDown(ui.Window):
	dropped  = 0
	dropstat = 0
	last = 0
	lastS = 0
	maxh = 95
	tt = ""
	OnChange = None
	class Item(ui.ListBoxEx.Item):
		def __init__(self,parent, text,value=0):
			ui.ListBoxEx.Item.__init__(self)

			self.textBox=ui.TextLine()
			self.textBox.SetParent(self)
			self.textBox.SetText(text)
			# self.textBox.SetLimitWidth(parent.GetWidth()-132)
			self.textBox.Show()
			self.value = value
		def GetValue(self):
			return self.value
		def __del__(self):
			ui.ListBoxEx.Item.__del__(self)
			
	def __init__(self,parent,tt = "",down=1):
		ui.Window.__init__(self,"TOP_MOST")
		self.tt=tt
		self.down = down
		self.SetParentProxy(parent)
		self.bg = ui.Bar("TOP_MOST")
		self.bg.SetParent(self)
		self.bg.SetPosition(0,0)
		self.bg.SetColor(0xc0000000)
		self.bg.OnMouseOverIn = self.bgMouseIn
		self.bg.OnMouseOverOut = self.bgMouseOut
		self.bg.OnMouseLeftButtonDown = self.ExpandMe
		self.bg.Show()
		self.act = ui.TextLine()
		self.act.SetParent(self.bg)
		self.act.SetPosition(4,2)
		self.act.SetText(self.tt)
		self.act.Show()
		self.GetText = self.act.GetText
		
		self.Drop = ui.Bar("TOP_MOST")
		self.Drop.SetParent(self.GetParentProxy())
		self.Drop.SetPosition(0,21)
		# self.Drop.SetSize(150,95)
		self.Drop.SetSize(150,0)
		# self.Drop.SetColor(0xc00a0a0a)
		self.Drop.SetColor(0xff0a0a0a)
		
		
		self.ScrollBar = ui.ThinScrollBar()
		self.ScrollBar.SetParent(self.Drop)
		self.ScrollBar.SetPosition(132,0)
		# self.ScrollBar.SetScrollBarSize(95)
		self.ScrollBar.SetScrollBarSize(0)
		# self.ScrollBar.Show()
		
		self.DropList = ui.ListBoxEx()
		self.DropList.SetParent(self.Drop)
		self.DropList.itemHeight = 12
		self.DropList.itemStep = 13
		self.DropList.SetPosition(0,0)
		# self.DropList.SetSize(132,self.maxh)
		self.DropList.SetSize(132,13) 
		self.DropList.SetScrollBar(self.ScrollBar)
		self.DropList.SetSelectEvent(self.SetTitle)
		self.DropList.SetViewItemCount(0)
		self.DropList.Show()
		if self.tt != "":
			self.AppendItemAndSelect(self.tt)
		self.selected = self.DropList.GetSelectedItem()
		
			
		self.SetSize(120,20)
	def __del__(self): 
		ui.Window.__del__(self)
	c = 1
	def AppendItem(self,text,value=0):
		self.c+=1   
		self.DropList.AppendItem(self.Item(self,text,value))
		self.maxh = min(95,13*self.c)
		if self.c > 7:
			self.ScrollBar.Show()
			
		
	def AppendItemAndSelect(self,text,value=0):
		self.DropList.AppendItem(self.Item(self,text,value))
		self.DropList.SelectIndex(len(self.DropList.itemList)-1)
		
		
	def ClearItems(self):
		self.DropList.RemoveAllItems()
		self.AppendItemAndSelect(self.tt)
		self.act.SetText(self.tt)
	def Clear(self):
		self.DropList.SelectIndex(0)
	def SelectByAffectId(self,id):
		for x in self.DropList.itemList:
			if x.value == id:
				self.DropList.SelectItem(x)
				break
				
	def SetTitle(self,item):
		self.act.SetText(str(item.textBox.GetText()))
		self.last = self.DropList.basePos
		self.lastS = self.ScrollBar.GetPos()
		self.dropped = 0
		self.selected = item
		if self.OnChange:
			self.OnChange()
		# self.Drop.Hide()
		
	def SetPosition(self,w,h):
		ui.Window.SetPosition(self,w,h)
		if self.down == 1:
			self.Drop.SetPosition(w,h+21)
		else:
			self.Drop.SetPosition(w,h-self.Drop.GetHeight())
		
	def SetSize(self,w,h):
		ui.Window.SetSize(self,w,h)
		self.bg.SetSize(w,h)
		self.Drop.SetSize(w,0)
		self.DropList.SetSize(w-18,self.maxh)
		for x in self.DropList.itemList:
			x.SetSize(w-18,12)
		self.ScrollBar.SetPosition(w-18,0)
		
		
	def ExpandMe(self):
		if self.dropped == 1:
			# self.Drop.Hide()
			self.dropped = 0
		else:
			# self.Drop.Show()
			self.dropped = 1
			
	def OnUpdate(self):
		iter = 6
		if self.Drop.GetHeight() < 50:
			self.ScrollBar.Hide()
		else:
			self.ScrollBar.Show()
			
		if self.dropped == 0 and self.dropstat == 1:
			if self.Drop.GetHeight() <=0:
				self.dropstat = 0
				self.Drop.SetSize(self.Drop.GetWidth(),0)
				self.ScrollBar.SetScrollBarSize(self.Drop.GetHeight())
				self.Drop.Hide()
			else:
				if self.Drop.GetHeight()-iter < 0:
					self.Drop.SetSize(self.Drop.GetWidth(),0)
				else:
					self.Drop.SetSize(self.Drop.GetWidth(),self.Drop.GetHeight()-iter)
					(w,h) = self.GetLocalPosition()
					self.SetPosition(w,h)
						
					
				self.ScrollBar.SetScrollBarSize(self.Drop.GetHeight())
			self.DropList.SetViewItemCount(int(self.Drop.GetHeight()/13))
			self.DropList.SetBasePos(self.last+1)
			self.DropList.SetBasePos(self.last)
		elif self.dropped == 1 and self.dropstat == 0:
			self.Drop.Show()
			self.SetTop()
			if self.Drop.GetHeight() >=self.maxh:
				self.Drop.SetSize(self.Drop.GetWidth(),self.maxh)
				self.ScrollBar.SetScrollBarSize(self.maxh)
				self.dropstat = 1
				self.DropList.SetViewItemCount(7)
				self.ScrollBar.SetPos(self.lastS)
			else:
				self.ScrollBar.SetScrollBarSize(self.Drop.GetHeight()+iter)
				self.Drop.SetSize(self.Drop.GetWidth(),self.Drop.GetHeight()+iter)
				(w,h) = self.GetLocalPosition()
				self.SetPosition(w,h)
			self.DropList.SetViewItemCount(int(self.Drop.GetHeight()/13))
			self.DropList.SetBasePos(self.last+1)
			self.DropList.SetBasePos(self.last)
		
	## BG Hover
	def bgMouseIn(self):
		self.bg.SetColor(0xc00a0a0a)
	def bgMouseOut(self):
		self.bg.SetColor(0xc0000000)

