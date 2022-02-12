import app
import ime
import grp
import snd
import wndMgr
import item
import skill
import localeInfo
import dbg
# MARK_BUG_FIX
import guild
# END_OF_MARK_BUG_FIX

from _weakref import proxy
import uiScriptLocale
import constInfo
import translate

import player
import chat

BACKGROUND_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 1.0)
DARK_COLOR = grp.GenerateColor(0.2, 0.2, 0.2, 1.0)
BRIGHT_COLOR = grp.GenerateColor(0.7, 0.7, 0.7, 1.0)

if localeInfo.IsCANADA():
	SELECT_COLOR = grp.GenerateColor(0.9, 0.03, 0.01, 0.4)
else:
	SELECT_COLOR = grp.GenerateColor(0.0, 0.0, 0.5, 0.3)

WHITE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.5)
HALF_WHITE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.2)

createToolTipWindowDict = {}

LOCALE_PATH = uiScriptLocale.WINDOWS_PATH
interfacelist = (
						"d:/ymir work/ui/pattern/Board_Corner_LeftTop.tga",
						"d:/ymir work/ui/pattern/Board_Corner_LeftBottom.tga",
						"d:/ymir work/ui/pattern/Board_Corner_RightTop.tga",
						"d:/ymir work/ui/pattern/Board_Corner_RightBottom.tga",
						"d:/ymir work/ui/pattern/Board_Line_Left.tga",
						"d:/ymir work/ui/pattern/Board_Line_Right.tga",
						"d:/ymir work/ui/pattern/Board_Line_Top.tga",
						"d:/ymir work/ui/pattern/Board_Line_Bottom.tga",
						"d:/ymir work/ui/pattern/TaskBar_Base.tga",
						"d:/ymir work/ui/game/windows/equipment_base.sub",
						"d:/ymir work/ui/equipment_bg_without_ring.tga",
						"d:/ymir work/ui/pattern/titlebar_left.tga",
						"d:/ymir work/ui/pattern/titlebar_center.tga",
						"d:/ymir work/ui/pattern/titlebar_right.tga",
						"d:/ymir work/ui/pattern/Board_Base.tga",
						"d:/ymir work/ui/minimap/minimap.sub",
						LOCALE_PATH+"tab_1.sub",
						LOCALE_PATH+"tab_2.sub",
						LOCALE_PATH+"tab_3.sub",
						LOCALE_PATH+"tab_4.sub",
						LOCALE_PATH+"label_std_item1.sub",
						LOCALE_PATH+"label_std_item2.sub",
						LOCALE_PATH+"label_ext_item1.sub",
						LOCALE_PATH+"label_ext_item2.sub",
						"d:/ymir work/ui/pattern/ThinBoard_Corner_LeftTop.tga",
						"d:/ymir work/ui/pattern/ThinBoard_Corner_LeftBottom.tga",
						"d:/ymir work/ui/pattern/ThinBoard_Corner_RightBottom.tga",
						"d:/ymir work/ui/pattern/ThinBoard_Corner_RightTop.tga",
						"d:/ymir work/ui/pattern/ThinBoard_Line_Left.tga",
						"d:/ymir work/ui/pattern/ThinBoard_Line_Right.tga",
						"d:/ymir work/ui/pattern/ThinBoard_Line_Top.tga",
						"d:/ymir work/ui/pattern/ThinBoard_Line_Bottom.tga",
)
interfacelist2 = []

def zmiengrafike(r,g,b,a):
	for x in interfacelist2:
		if x:
			wndMgr.SetDiffuseColor(x.hWnd, r, g, b, a)
	pass

def RegisterCandidateWindowClass(codePage, candidateWindowClass):
	EditLine.candidateWindowClassDict[codePage]=candidateWindowClass
def RegisterToolTipWindow(type, createToolTipWindow):
	createToolTipWindowDict[type]=createToolTipWindow

app.SetDefaultFontName(localeInfo.UI_DEF_FONT)

## Window Manager Event List##
##############################
## "OnMouseLeftButtonDown"
## "OnMouseLeftButtonUp"
## "OnMouseLeftButtonDoubleClick"
## "OnMouseRightButtonDown"
## "OnMouseRightButtonUp"
## "OnMouseRightButtonDoubleClick"
## "OnMouseDrag"
## "OnSetFocus"
## "OnKillFocus"
## "OnMouseOverIn"
## "OnMouseOverOut"
## "OnRender"
## "OnUpdate"
## "OnKeyDown"
## "OnKeyUp"
## "OnTop"
## "OnIMEUpdate" ## IME Only
## "OnIMETab"    ## IME Only
## "OnIMEReturn" ## IME Only
##############################
## Window Manager Event List##


class __mem_func__:
    class __noarg_call__:
        def __init__(self, cls, obj, func):
            self.cls=cls
            self.obj=proxy(obj)
            self.func=proxy(func)

        def __call__(self, *arg):
            return self.func(self.obj)

    class __arg_call__:
        def __init__(self, cls, obj, func):
            self.cls=cls
            self.obj=proxy(obj)
            self.func=proxy(func)

        def __call__(self, *arg):
            return self.func(self.obj, *arg)

    def __init__(self, mfunc):
        if mfunc.im_func.func_code.co_argcount>1:
            self.call=__mem_func__.__arg_call__(mfunc.im_class, mfunc.im_self, mfunc.im_func)
        else:
            self.call=__mem_func__.__noarg_call__(mfunc.im_class, mfunc.im_self, mfunc.im_func)

    def __call__(self, *arg):
        return self.call(*arg)


class Window(object):

	def SetClickEvent(self, event):
		self.clickEvent = __mem_func__(event)

	def OnMouseLeftButtonDown(self):
		if self.clickEvent:
			self.clickEvent()

	def NoneMethod(cls):
		pass

	NoneMethod = classmethod(NoneMethod)

	def __init__(self, layer = "UI"):
		self.clickEvent = None
		self.hWnd = None
		self.parentWindow = 0
		self.onMouseLeftButtonUpEvent = None
		self.onRunMouseWheelEvent = None
		self.RegisterWindow(layer)
		self.Hide()
		if app.ENABLE_SEND_TARGET_INFO:
			self.mouseLeftButtonDownEvent = None
			self.mouseLeftButtonDownArgs = None
			self.mouseLeftButtonUpEvent = None
			self.mouseLeftButtonUpArgs = None
			self.mouseLeftButtonDoubleClickEvent = None
			self.mouseRightButtonDownEvent = None
			self.mouseRightButtonDownArgs = None
			self.moveWindowEvent = None
			self.renderEvent = None
			self.renderArgs = None

			self.overInEvent = None
			self.overInArgs = None

			self.overOutEvent = None
			self.overOutArgs = None

			self.baseX = 0
			self.baseY = 0

			self.SetWindowName("NONAME_Window")

	def __del__(self):
		wndMgr.Destroy(self.hWnd)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.Register(self, layer)

	def Destroy(self):
		pass

	def GetWindowHandle(self):
		return self.hWnd

	def AddFlag(self, style):
		wndMgr.AddFlag(self.hWnd, style)

	def IsRTL(self):
		return wndMgr.IsRTL(self.hWnd)

	def SetWindowName(self, Name):
		wndMgr.SetName(self.hWnd, Name)

	def GetWindowName(self):
		return wndMgr.GetName(self.hWnd)

	if app.ENABLE_SEND_TARGET_INFO:
		def SetParent(self, parent):
			if parent:
				wndMgr.SetParent(self.hWnd, parent.hWnd)
			else:
				wndMgr.SetParent(self.hWnd, 0)
	
		def SetAttachParent(self, parent):
			wndMgr.SetAttachParent(self.hWnd, parent.hWnd)
	else:
		def SetParent(self, parent):
			wndMgr.SetParent(self.hWnd, parent.hWnd)

	def SetParentProxy(self, parent):
		self.parentWindow=proxy(parent)
		wndMgr.SetParent(self.hWnd, parent.hWnd)

	
	def GetParentProxy(self):
		return self.parentWindow

	def SetPickAlways(self):
		wndMgr.SetPickAlways(self.hWnd)

	def SetWindowHorizontalAlignLeft(self):
		wndMgr.SetWindowHorizontalAlign(self.hWnd, wndMgr.HORIZONTAL_ALIGN_LEFT)

	def SetWindowHorizontalAlignCenter(self):
		wndMgr.SetWindowHorizontalAlign(self.hWnd, wndMgr.HORIZONTAL_ALIGN_CENTER)

	def SetWindowHorizontalAlignRight(self):
		wndMgr.SetWindowHorizontalAlign(self.hWnd, wndMgr.HORIZONTAL_ALIGN_RIGHT)

	def SetWindowVerticalAlignTop(self):
		wndMgr.SetWindowVerticalAlign(self.hWnd, wndMgr.VERTICAL_ALIGN_TOP)

	def SetWindowVerticalAlignCenter(self):
		wndMgr.SetWindowVerticalAlign(self.hWnd, wndMgr.VERTICAL_ALIGN_CENTER)

	def SetWindowVerticalAlignBottom(self):
		wndMgr.SetWindowVerticalAlign(self.hWnd, wndMgr.VERTICAL_ALIGN_BOTTOM)

	def SetTop(self):
		wndMgr.SetTop(self.hWnd)

	def Show(self):
		wndMgr.Show(self.hWnd)

	def Hide(self):
		wndMgr.Hide(self.hWnd)

	if app.ENABLE_SEND_TARGET_INFO:
		def SetVisible(self, is_show):
			if is_show:
				self.Show()
			else:
				self.Hide()

	def Lock(self):
		wndMgr.Lock(self.hWnd)

	def Unlock(self):
		wndMgr.Unlock(self.hWnd)

	def IsShow(self):
		return wndMgr.IsShow(self.hWnd)

	def UpdateRect(self):
		wndMgr.UpdateRect(self.hWnd)

	def SetSize(self, width, height):
		wndMgr.SetWindowSize(self.hWnd, width, height)

	def GetWidth(self):
		return wndMgr.GetWindowWidth(self.hWnd)

	def GetHeight(self):
		return wndMgr.GetWindowHeight(self.hWnd)

	def GetLocalPosition(self):
		return wndMgr.GetWindowLocalPosition(self.hWnd)

	if app.ENABLE_SEND_TARGET_INFO:
		def GetLeft(self):
			x, y = self.GetLocalPosition()
			return x
	
		def GetGlobalLeft(self):
			x, y = self.GetGlobalPosition()
			return x
	
		def GetTop(self):
			x, y = self.GetLocalPosition()
			return y
	
		def GetGlobalTop(self):
			x, y = self.GetGlobalPosition()
			return y
	
		def GetRight(self):
			return self.GetLeft() + self.GetWidth()
	
		def GetBottom(self):
			return self.GetTop() + self.GetHeight()

	def GetGlobalPosition(self):
		return wndMgr.GetWindowGlobalPosition(self.hWnd)

	def GetMouseLocalPosition(self):
		return wndMgr.GetMouseLocalPosition(self.hWnd)

	def GetRect(self):
		return wndMgr.GetWindowRect(self.hWnd)

	if app.ENABLE_SEND_TARGET_INFO:
		def SetLeft(self, x):
			wndMgr.SetWindowPosition(self.hWnd, x, self.GetTop())

	def SetPosition(self, x, y):
		wndMgr.SetWindowPosition(self.hWnd, x, y)

	def SetCenterPosition(self, x = 0, y = 0):
		self.SetPosition((wndMgr.GetScreenWidth() - self.GetWidth()) / 2 + x, (wndMgr.GetScreenHeight() - self.GetHeight()) / 2 + y)

	if app.ENABLE_SEND_TARGET_INFO:
		def SavePosition(self):
			self.baseX = self.GetLeft()
			self.baseY = self.GetTop()
	
		def UpdatePositionByScale(self, scale):
			self.SetPosition(self.baseX * scale, self.baseY * scale)

	def IsFocus(self):
		return wndMgr.IsFocus(self.hWnd)

	def SetFocus(self):
		wndMgr.SetFocus(self.hWnd)

	def KillFocus(self):
		wndMgr.KillFocus(self.hWnd)

	def GetChildCount(self):
		return wndMgr.GetChildCount(self.hWnd)

	def IsIn(self):
		return wndMgr.IsIn(self.hWnd)

	if app.ENABLE_SEND_TARGET_INFO:
		def IsInPosition(self):
			xMouse, yMouse = wndMgr.GetMousePosition()
			x, y = self.GetGlobalPosition()
			return xMouse >= x and xMouse < x + self.GetWidth() and yMouse >= y and yMouse < y + self.GetHeight()
	
		def SetMouseLeftButtonDownEvent(self, event, *args):
			self.mouseLeftButtonDownEvent = event
			self.mouseLeftButtonDownArgs = args
	
		def OnMouseLeftButtonDown(self):
			if self.mouseLeftButtonDownEvent:
				apply(self.mouseLeftButtonDownEvent, self.mouseLeftButtonDownArgs)

	if app.ENABLE_SEND_TARGET_INFO:
		def SetMouseLeftButtonUpEvent(self, event, *args):
			self.mouseLeftButtonUpEvent = event
			self.mouseLeftButtonUpArgs = args
	else:
		def SetOnMouseLeftButtonUpEvent(self, event):
			self.onMouseLeftButtonUpEvent = ev

	def SetOnMouseLeftButtonUpEvent(self, event):
		self.onMouseLeftButtonUpEvent = event
	
	if app.ENABLE_SEND_TARGET_INFO:
		def SetMouseLeftButtonDoubleClickEvent(self, event):
			self.mouseLeftButtonDoubleClickEvent = event
	
		def OnMouseLeftButtonDoubleClick(self):
			if self.mouseLeftButtonDoubleClickEvent:
				self.mouseLeftButtonDoubleClickEvent()
	
		def SetMouseRightButtonDownEvent(self, event, *args):
			self.mouseRightButtonDownEvent = event
			self.mouseRightButtonDownArgs = args
	
		def OnMouseRightButtonDown(self):
			if self.mouseRightButtonDownEvent:
				apply(self.mouseRightButtonDownEvent, self.mouseRightButtonDownArgs)
	
		def SetMoveWindowEvent(self, event):
			self.moveWindowEvent = event
	
		def OnMoveWindow(self, x, y):
			if self.moveWindowEvent:
				self.moveWindowEvent(x, y)
	
		def SAFE_SetOverInEvent(self, func, *args):
			self.overInEvent = __mem_func__(func)
			self.overInArgs = args
	
		def SetOverInEvent(self, func, *args):
			self.overInEvent = func
			self.overInArgs = args
	
		def SAFE_SetOverOutEvent(self, func, *args):
			self.overOutEvent = __mem_func__(func)
			self.overOutArgs = args
	
		def SetOverOutEvent(self, func, *args):
			self.overOutEvent = func
			self.overOutArgs = args
	
		def OnMouseOverIn(self):
			if self.overInEvent:
				apply(self.overInEvent, self.overInArgs)
	
		def OnMouseOverOut(self):
			if self.overOutEvent:
				apply(self.overOutEvent, self.overOutArgs)
	
		def SAFE_SetRenderEvent(self, event, *args):
			self.renderEvent = __mem_func__(event)
			self.renderArgs = args
	
		def ClearRenderEvent(self):
			self.renderEvent = None
			self.renderArgs = None
	
		def OnRender(self):
			if self.renderEvent:
				apply(self.renderEvent, self.renderArgs)

	def OnRunMouseWheel(self, nLen):
		if not self.onRunMouseWheelEvent:
			return False

		apply(self.onRunMouseWheelEvent, (bool(nLen < 0), ))
		return True

	def SetOnRunMouseWheelEvent(self, event):
		self.onRunMouseWheelEvent = __mem_func__(event)

	def OnMouseLeftButtonUp(self):
		if self.onMouseLeftButtonUpEvent:
			self.onMouseLeftButtonUpEvent()

class ListBoxEx(Window):
	class Item(Window):
		def __init__(self):
			Window.__init__(self)

		def __del__(self):
			Window.__del__(self)

		def SetParent(self, parent):
			Window.SetParent(self, parent)
			self.parent=proxy(parent)

		def OnMouseLeftButtonDown(self):
			self.parent.SelectItem(self)

		def OnRender(self):
			if self.parent.GetSelectedItem()==self:
				self.OnSelectedRender()

		def OnSelectedRender(self):
			x, y = self.GetGlobalPosition()
			grp.SetColor(grp.GenerateColor(0.0, 0.0, 0.7, 0.7))
			grp.RenderBar(x, y, self.GetWidth(), self.GetHeight())

	def __init__(self):
		Window.__init__(self)

		self.viewItemCount=10
		self.basePos=0
		self.itemHeight=16
		self.itemStep=20
		self.selItem=0
		self.change=0
		self.itemList=[]
		self.onSelectItemEvent = lambda *arg: None

		if localeInfo.IsARABIC():
			self.itemWidth=130
		else:
			self.itemWidth=100

		self.scrollBar=None
		self.__UpdateSize()

	def __del__(self):
		Window.__del__(self)

	def __UpdateSize(self):
		height=self.itemStep*self.__GetViewItemCount()

		self.SetSize(self.itemWidth, height)

	def IsEmpty(self):
		if len(self.itemList)==0:
			return 1
		return 0

	def SetItemStep(self, itemStep):
		self.itemStep=itemStep
		self.__UpdateSize()

	def SetItemSize(self, itemWidth, itemHeight):
		self.itemWidth=itemWidth
		self.itemHeight=itemHeight
		self.__UpdateSize()

	def SetViewItemCount(self, viewItemCount):
		self.viewItemCount=viewItemCount

	def SetSelectEvent(self, event):
		self.onSelectItemEvent = event

	def SetBasePos(self, basePos):
		for oldItem in self.itemList[self.basePos:self.basePos+self.viewItemCount]:
			oldItem.Hide()

		self.basePos=basePos

		pos=basePos
		for newItem in self.itemList[self.basePos:self.basePos+self.viewItemCount]:
			(x, y)=self.GetItemViewCoord(pos, newItem.GetWidth())
			newItem.SetPosition(x, y)
			newItem.Show()
			pos+=1

	def scroolpos(self,pos):
		if self.scrollBar:
			self.scrollBar.SetPos(0)

	def GetItemIndex(self, argItem):
		return self.itemList.index(argItem)

	def GetSelectedItem(self):
		self.change=0
		return self.selItem

	def Degisim(self):
		return self.change

	def SelectIndex(self, index):

		if index >= len(self.itemList) or index < 0:
			self.selItem = None
			return

		try:
			self.selItem=self.itemList[index]
			self.change=1
		except:
			pass

	def SelectItem(self, selItem):
		self.change=1
		self.selItem=selItem
		self.onSelectItemEvent(selItem)
	
	def RemoveAllItems(self):
		self.change=0
		self.selItem=None
		self.itemList=[]

		if self.scrollBar:
			self.scrollBar.SetPos(0)

	def RemoveItem2(self, id=-1):
		if id >= 0:
			cek = self.itemList[id]
			cek.Hide()
			self.itemList.remove(cek)
			return
		if len(self.itemList)==0:
			return
		for newItem in self.itemList[self.basePos:self.basePos+self.viewItemCount]:
			# newItem.Hide()
			self.itemList.remove(newItem)
		self.itemList=[]

	def RemoveItem(self, delItem):
		self.change=1
		if delItem==self.selItem:
			self.selItem=None

		self.itemList.remove(delItem)

	def AppendItem(self, newItem):
		newItem.SetParent(self)
		newItem.SetSize(self.itemWidth, self.itemHeight)

		pos=len(self.itemList)
		if self.__IsInViewRange(pos):
			(x, y)=self.GetItemViewCoord(pos, newItem.GetWidth())
			newItem.SetPosition(x, y)
			newItem.Show()
		else:
			newItem.Hide()

		self.itemList.append(newItem)

	def SetScrollBar(self, scrollBar):
		scrollBar.SetScrollEvent(__mem_func__(self.__OnScroll))
		self.scrollBar=scrollBar

	def __OnScroll(self):
		self.SetBasePos(int(self.scrollBar.GetPos()*self.__GetScrollLen()))

	def __GetScrollLen(self):
		scrollLen=self.__GetItemCount()-self.__GetViewItemCount()
		if scrollLen<0:
			return 0

		return scrollLen

	def __GetViewItemCount(self):
		return self.viewItemCount

	def __GetItemCount(self):
		return len(self.itemList)

	def ItemCount(self):
		return len(self.itemList)

	def GetItemViewCoord(self, pos, itemWidth):
		if localeInfo.IsARABIC():
			return (self.GetWidth()-itemWidth-10, (pos-self.basePos)*self.itemStep)
		else:
			return (0, (pos-self.basePos)*self.itemStep)

	def __IsInViewRange(self, pos):
		if pos<self.basePos:
			return 0
		if pos>=self.basePos+self.viewItemCount:
			return 0
		return 1

class CandidateListBox(ListBoxEx):

	HORIZONTAL_MODE = 0
	VERTICAL_MODE = 1

	class Item(ListBoxEx.Item):
		def __init__(self, text):
			ListBoxEx.Item.__init__(self)

			self.textBox=TextLine()
			self.textBox.SetParent(self)
			self.textBox.SetText(text)
			self.textBox.Show()

		def __del__(self):
			ListBoxEx.Item.__del__(self)

	def __init__(self, mode = HORIZONTAL_MODE):
		ListBoxEx.__init__(self)
		self.itemWidth=32
		self.itemHeight=32
		self.mode = mode

	def __del__(self):
		ListBoxEx.__del__(self)

	def SetMode(self, mode):
		self.mode = mode

	def AppendItem(self, newItem):
		ListBoxEx.AppendItem(self, newItem)

	def GetItemViewCoord(self, pos):
		if self.mode == self.HORIZONTAL_MODE:
			return ((pos-self.basePos)*self.itemStep, 0)
		elif self.mode == self.VERTICAL_MODE:
			return (0, (pos-self.basePos)*self.itemStep)

if app.ENABLE_SEND_TARGET_INFO:
	class ListBoxExNew(Window):
		class Item(Window):
			def __init__(self):
				Window.__init__(self)

				self.realWidth = 0
				self.realHeight = 0

				self.removeTop = 0
				self.removeBottom = 0

				self.SetWindowName("NONAME_ListBoxExNew_Item")

			def __del__(self):
				Window.__del__(self)

			def SetParent(self, parent):
				Window.SetParent(self, parent)
				self.parent=proxy(parent)

			def SetSize(self, width, height):
				self.realWidth = width
				self.realHeight = height
				Window.SetSize(self, width, height)

			def SetRemoveTop(self, height):
				self.removeTop = height
				self.RefreshHeight()

			def SetRemoveBottom(self, height):
				self.removeBottom = height
				self.RefreshHeight()

			def SetCurrentHeight(self, height):
				Window.SetSize(self, self.GetWidth(), height)

			def GetCurrentHeight(self):
				return Window.GetHeight(self)

			def ResetCurrentHeight(self):
				self.removeTop = 0
				self.removeBottom = 0
				self.RefreshHeight()

			def RefreshHeight(self):
				self.SetCurrentHeight(self.GetHeight() - self.removeTop - self.removeBottom)

			def GetHeight(self):
				return self.realHeight

		def __init__(self, stepSize, viewSteps):
			Window.__init__(self)

			self.viewItemCount=10
			self.basePos=0
			self.baseIndex=0
			self.maxSteps=0
			self.viewSteps = viewSteps
			self.stepSize = stepSize
			self.itemList=[]

			self.scrollBar=None

			self.SetWindowName("NONAME_ListBoxEx")

		def __del__(self):
			Window.__del__(self)

		def IsEmpty(self):
			if len(self.itemList)==0:
				return 1
			return 0

		def __CheckBasePos(self, pos):
			self.viewItemCount = 0

			start_pos = pos

			height = 0
			while height < self.GetHeight():
				if pos >= len(self.itemList):
					return start_pos == 0
				height += self.itemList[pos].GetHeight()
				pos += 1
				self.viewItemCount += 1
			return height == self.GetHeight()

		def SetBasePos(self, basePos, forceRefresh = TRUE):
			if forceRefresh == FALSE and self.basePos == basePos:
				return

			for oldItem in self.itemList[self.baseIndex:self.baseIndex+self.viewItemCount]:
				oldItem.ResetCurrentHeight()
				oldItem.Hide()

			self.basePos=basePos

			baseIndex = 0
			while basePos > 0:
				basePos -= self.itemList[baseIndex].GetHeight() / self.stepSize
				if basePos < 0:
					self.itemList[baseIndex].SetRemoveTop(self.stepSize * abs(basePos))
					break
				baseIndex += 1
			self.baseIndex = baseIndex

			stepCount = 0
			self.viewItemCount = 0
			while baseIndex < len(self.itemList):
				stepCount += self.itemList[baseIndex].GetCurrentHeight() / self.stepSize
				self.viewItemCount += 1
				if stepCount > self.viewSteps:
					self.itemList[baseIndex].SetRemoveBottom(self.stepSize * (stepCount - self.viewSteps))
					break
				elif stepCount == self.viewSteps:
					break
				baseIndex += 1

			y = 0
			for newItem in self.itemList[self.baseIndex:self.baseIndex+self.viewItemCount]:
				newItem.SetPosition(0, y)
				newItem.Show()
				y += newItem.GetCurrentHeight()

		def GetItemIndex(self, argItem):
			return self.itemList.index(argItem)

		def GetSelectedItem(self):
			return self.selItem

		def GetSelectedItemIndex(self):
			return self.selItemIdx

		def RemoveAllItems(self):
			self.itemList=[]
			self.maxSteps=0

			if self.scrollBar:
				self.scrollBar.SetPos(0)

		def RemoveItem(self, delItem):
			self.maxSteps -= delItem.GetHeight() / self.stepSize
			self.itemList.remove(delItem)

		def AppendItem(self, newItem):
			if newItem.GetHeight() % self.stepSize != 0:
				import dbg
				dbg.TraceError("Invalid AppendItem height %d stepSize %d" % (newItem.GetHeight(), self.stepSize))
				return

			self.maxSteps += newItem.GetHeight() / self.stepSize
			newItem.SetParent(self)
			self.itemList.append(newItem)

		def SetScrollBar(self, scrollBar):
			scrollBar.SetScrollEvent(__mem_func__(self.__OnScroll))
			self.scrollBar=scrollBar

		def __OnScroll(self):
			self.SetBasePos(int(self.scrollBar.GetPos()*self.__GetScrollLen()), FALSE)

		def __GetScrollLen(self):
			scrollLen=self.maxSteps-self.viewSteps
			if scrollLen<0:
				return 0

			return scrollLen

		def __GetViewItemCount(self):
			return self.viewItemCount

		def __GetItemCount(self):
			return len(self.itemList)

		def GetViewItemCount(self):
			return self.viewItemCount

		def GetItemCount(self):
			return len(self.itemList)

class ResizableTextValue(Window):

	BACKGROUND_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 1.0)
	LINE_COLOR = grp.GenerateColor(0.4, 0.4, 0.4, 1.0)
	
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)
		
		self.isBackground = TRUE
		self.LineText = None
		self.ToolTipText = None
		
		self.width = 0
		self.height = 0
		self.lines = []
		
	def __del__(self):
		Window.__del__(self)
		
	def SetSize(self, width, height):
		Window.SetSize(self, width, height)
		self.width = width
		self.height = height
		
	def SetToolTipText(self, tooltiptext, x = 0, y = 0):
		if not self.ToolTipText:		
			toolTip=createToolTipWindowDict["TEXT"]()
			toolTip.SetParent(self)
			toolTip.SetSize(0, 0)
			toolTip.SetHorizontalAlignCenter()
			toolTip.SetOutline()
			toolTip.Hide()
			toolTip.SetPosition(x + self.GetWidth()/2, y-20)
			self.ToolTipText=toolTip

		self.ToolTipText.SetText(tooltiptext)
		
	def SetText(self, text):
		if not self.LineText:
			textLine = TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(self.GetWidth()/2, (self.GetHeight()/2)-1)
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.SetOutline()
			textLine.Show()
			self.LineText = textLine

		self.LineText.SetText(text)
		
	def SetTextColor(self, color):
		if not self.LineText:
			return
		self.LineText.SetPackedFontColor(color)
		
	def GetText(self):
		if not self.LineText:
			return
		return self.LineText.GetText()
		
	def SetLineColor(self, color):
		self.LINE_COLOR = color
		
	def SetLine(self, line_value):
		self.lines.append(line_value)
		
	def SetBackgroundColor(self, color):
		self.BACKGROUND_COLOR = color
		
	def SetNoBackground(self):
		self.isBackground = FALSE
	
	def OnRender(self):
		xRender, yRender = self.GetGlobalPosition()
		
		widthRender = self.width
		heightRender = self.height
		if self.isBackground:
			grp.SetColor(self.BACKGROUND_COLOR)
			grp.RenderBar(xRender, yRender, widthRender, heightRender)
		grp.SetColor(self.LINE_COLOR)
		if 'top' in self.lines:
			grp.RenderLine(xRender, yRender, widthRender, 0)
		if 'left' in self.lines:
			grp.RenderLine(xRender, yRender, 0, heightRender)
		if 'bottom' in self.lines:
			grp.RenderLine(xRender, yRender+heightRender, widthRender+1, 0)
		if 'right' in self.lines:	
			grp.RenderLine(xRender+widthRender, yRender, 0, heightRender)

class Table(Window):

	ROW_HEIGHT = 18
	HEADER_EXTRA_HEIGHT = 0

	class TableLine(Window):

		def __init__(self, mouseLeftButtonDownEvent, mouseLeftButtonDoubleClickEvent):
			Window.__init__(self)

			self.colSize = []
			self.textLines = []

			self.mouseLeftButtonDownEvent = mouseLeftButtonDownEvent
			self.mouseLeftButtonDoubleClickEvent = mouseLeftButtonDoubleClickEvent

			self.SetSize(0, Table.ROW_HEIGHT)

			self.SetWindowName("NONAME_Table_TableLine")

		def __del__(self):
			Window.__del__(self)

		def __UpdateWidth(self, appendSize):
			self.SetSize(self.GetWidth() + appendSize, self.GetHeight())

		def __CheckLength(self, line, maxWidth):
			if line.GetTextWidth() <= maxWidth:
				return line.GetText()

			text = line.GetText()
			pos = len(text)
			while pos > 1:
				pos = pos - 1

				line.SetText(text[:pos] + "..")
				if line.GetTextWidth() <= maxWidth and text[pos-1] != " ":
					return text[:pos] + ".."

			return ".."

		def AppendCol(self, wnd, width, checkLength):
			lineWnd = Window()
			lineWnd.SetParent(self)
			lineWnd.SetMouseLeftButtonDownEvent(self.mouseLeftButtonDownEvent)
			lineWnd.SetMouseLeftButtonDoubleClickEvent(self.mouseLeftButtonDoubleClickEvent)
			lineWnd.SetSize(width, self.GetHeight())
			lineWnd.SetPosition(self.GetWidth(), 0)
			lineWnd.Show()

			if type(wnd) == type("") or type(wnd) == type(0) or type(wnd) == type(0.0):
				line = TextLine()
				line.SetParent(lineWnd)
				line.SetWindowHorizontalAlignCenter()
				line.SetHorizontalAlignCenter()
				line.SetWindowVerticalAlignCenter()
				line.SetVerticalAlignCenter()
				line.SetText(str(wnd))
				line.Show()
				if checkLength == True:
					line.SetText(self.__CheckLength(line, width))
				lineWnd.wnd = line
			else:
				wnd.SetParent(lineWnd)
				wnd.SetPosition(0, 0)
				wnd.SetWindowHorizontalAlignCenter()
				wnd.SetWindowVerticalAlignCenter()
				wnd.Show()
				lineWnd.wnd = wnd

			self.textLines.append(lineWnd)

			self.__UpdateWidth(width)

		def OnMouseLeftButtonDown(self):
			self.mouseLeftButtonDownEvent()

		def OnMouseLeftButtonDoubleClick(self):
			self.mouseLeftButtonDoubleClickEvent()

	def __init__(self):
		Window.__init__(self)

		self.cols = 0
		self.rows = 0
		self.basePos = 0
		self.viewLineCount = 0

		self.overLine = -1
		self.overHeader = -1
		self.selectedKey = -1
		self.selectedLine = -1

		self.overRender = None
		self.selectRender = None

		self.colSizePct = []
		self.checkLengthIndexes = []
		self.maxColSizePct = 100
		self.headerLine = None
		self.lines = []
		self.keys = []
		self.keyDict = {}

		self.headerClickEvent = lambda *arg: None
		self.doubleClickEvent = lambda *arg: None

		self.SetWindowName("NONAME_Table")

	def __del__(self):
		Window.__del__(self)

	def Destroy(self):
		self.lines = []

	def SetWidth(self, width):
		self.SetSize(width, self.GetHeight())

	def SetColSizePct(self, colSizeList):
		self.colSizePct = []
		self.maxColSizePct = 0
		for size in colSizeList:
			self.colSizePct.append(size)
			self.maxColSizePct += size
		self.LocateLines()

	def AddCheckLengthIndex(self, index):
		self.checkLengthIndexes.append(index)

	def GetColSize(self, index):
		return int(self.GetWidth() * self.colSizePct[index] / self.maxColSizePct)

	def __BuildLine(self, colList):
		line = self.TableLine(__mem_func__(self.OnMouseLeftButtonDown), __mem_func__(self.OnMouseLeftButtonDoubleClick))
		line.SetParent(self)
		for i in xrange(len(colList)):
			line.AppendCol(colList[i], self.GetColSize(i), i in self.checkLengthIndexes)
		return line

	def SetHeader(self, colList, extraHeight = 0):
		self.headerLine = self.__BuildLine(colList)
		self.HEADER_EXTRA_HEIGHT = extraHeight
		self.LocateLines()

	def ClearHeader(self):
		self.headerLine = None
		self.LocateLines()

	def Clear(self):
		self.lines = []
		self.keys = []
		self.basePos = 0
		self.overLine = -1
		self.overHeader = -1
		self.selectedKey = -1
		self.selectedLine = -1
		self.LocateLines()

	def GetLineCount(self):
		return len(self.lines)

	def GetMaxLineCount(self):
		if self.GetHeight() < self.ROW_HEIGHT:
			return 0

		maxHeight = self.GetHeight()
		if self.headerLine != None:
			maxHeight -= self.ROW_HEIGHT

		return int(maxHeight / self.ROW_HEIGHT)

	def GetViewLineCount(self):
		return self.viewLineCount

	def Append(self, index, colList, refresh = True):
		self.keyDict[index] = len(self.lines)
		self.lines.append(self.__BuildLine(colList))
		self.keys.append(index)
		if refresh == True:
			self.LocateLines()

	def Erase(self, index):
		if not self.keyDict.has_key(index):
			return

		listIndex = self.keyDict[index]

		for i in xrange(listIndex + 1, len(self.lines)):
			self.keyDict[self.keys[i]] -= 1

		del self.lines[listIndex]
		del self.keys[listIndex]
		del self.keyDict[index]

		if self.selectedLine != -1:
			if self.selectedKey == index:
				self.selectedKey = -1
				self.selectedLine = -1
			else:
				self.selectedLine = self.keyDict[self.selectedKey]
			self.__RefreshSelectedLineRender()

		if listIndex >= self.basePos and listIndex < self.basePos + self.viewLineCount:
			if self.basePos > 0 and self.GetLineCount() < self.basePos + self.viewLineCount:
				self.basePos -= 1
			self.LocateLines()

	def LocateLines(self):
		maxHeight = self.GetHeight()
		if maxHeight < self.ROW_HEIGHT:
			maxHeight = 0

		height = 0

		if self.headerLine != None:
			self.headerLine.SetPosition(0, height)
			self.headerLine.Show()
			height += self.ROW_HEIGHT + self.HEADER_EXTRA_HEIGHT

		self.viewLineCount = 0

		for i in xrange(len(self.lines)):
			if i < self.basePos or (maxHeight != 0 and height + self.ROW_HEIGHT >= maxHeight):
				self.lines[i].Hide()
				continue

			self.lines[i].SetPosition(0, height)
			self.lines[i].Show()
			height += self.ROW_HEIGHT

			self.viewLineCount += 1

	def SetBasePos(self, basePos):
		if basePos < 0:
			basePos = 0
		elif basePos + self.GetMaxLineCount() > self.GetLineCount():
			basePos = max(0, self.GetLineCount() - self.GetMaxLineCount())

		self.basePos = basePos
		self.LocateLines()
		self.__RefreshSelectedLineRender()

	def GetBasePos(self):
		return self.basePos

	def SelectLine(self, line):
		if line < 0 or line >= self.GetViewLineCount():
			line = -1
		else:
			line += self.basePos

		self.selectedKey = self.keys[line]
		self.selectedLine = line
		self.__RefreshSelectedLineRender()

	def __RefreshSelectedLineRender(self):
		self.selectRender = None

		if self.selectedLine == -1:
			return

		if self.selectedLine < self.basePos or self.selectedLine >= self.basePos + self.viewLineCount:
			return

		x, y = self.GetGlobalPosition()
		if self.headerLine != None:
			y += self.ROW_HEIGHT + self.HEADER_EXTRA_HEIGHT

		self.selectRender = {
			"x" : x,
			"y":  y + self.ROW_HEIGHT * (self.selectedLine - self.basePos),
			"width" : self.GetWidth(),
			"height" : self.ROW_HEIGHT,
		}

	def OnMouseLeftButtonDown(self):
		if self.overLine != -1:
			self.SelectLine(self.overLine)
		elif self.overHeader != -1:
			self.headerClickEvent(self.overHeader)

	def SetDoubleClickEvent(self, event):
		self.doubleClickEvent = event

	def OnMouseLeftButtonDoubleClick(self):
		if self.selectedLine != -1 and self.overLine == self.selectedLine:
			self.doubleClickEvent(self.selectedKey)

	def SetHeaderClickEvent(self, event):
		self.headerClickEvent = event

	def OnUpdate(self):
		self.__RefreshSelectedLineRender()

		self.overLine = -1
		self.overHeader = -1
		self.overRender = None

		x, y = self.GetGlobalPosition()
		xMouse, yMouse = wndMgr.GetMousePosition()

		if xMouse < x or xMouse > x + self.GetWidth():
			return

		if self.headerLine != None:
			y += self.ROW_HEIGHT + self.HEADER_EXTRA_HEIGHT

		overLine = int((yMouse - y) / self.ROW_HEIGHT)
		if overLine < 0 or overLine >= self.viewLineCount:
			if yMouse >= y - (self.ROW_HEIGHT + self.HEADER_EXTRA_HEIGHT) and yMouse < y - self.HEADER_EXTRA_HEIGHT and self.headerLine != None:
				width = 0
				headerColIndex = 0
				for i in xrange(len(self.colSizePct)):
					width = int(self.GetWidth() * self.colSizePct[i] / self.maxColSizePct)
					if xMouse <= x + width:
						break
					headerColIndex += 1
					x += width
					if headerColIndex >= len(self.colSizePct):
						return

				self.overHeader = headerColIndex
				self.overRender = {
					"x" : x,
					"y" : y - self.ROW_HEIGHT - self.HEADER_EXTRA_HEIGHT,
					"width" : width,
					"height" : self.ROW_HEIGHT,
				}

			return

		self.overLine = overLine
		self.overRender = {
			"x" : x,
			"y" : y + overLine * self.ROW_HEIGHT,
			"width" : self.GetWidth(),
			"height" : self.ROW_HEIGHT,
		}

	def __DrawRender(self, color, render):
		grp.SetColor(color)
		grp.RenderBar(render["x"], render["y"], render["width"], render["height"])

	def OnRender(self):
		if self.overRender:
			self.__DrawRender(HALF_WHITE_COLOR, self.overRender)
		if self.selectRender:
			self.__DrawRender(SELECT_COLOR, self.selectRender)

class Input(Window):
	def __init__(self):
		Window.__init__(self)

	def __del__(self):
		Window.__del__(self)

	def MakeInput(self, width):
		imgLeft = ImageBox()
		imgCenter = ExpandedImageBox()
		imgRight = ImageBox()
		imgLeft.AddFlag("not_pick")
		imgCenter.AddFlag("not_pick")
		imgRight.AddFlag("not_pick")
		imgLeft.SetParent(self)
		imgCenter.SetParent(self)
		imgRight.SetParent(self)

		imgLeft.LoadImage("d:/ymir work/ui/pattern/border_c_left.tga")
		imgCenter.LoadImage("d:/ymir work/ui/pattern/border_c_middle.tga")
		imgRight.LoadImage("d:/ymir work/ui/pattern/border_c_right.tga")

		imgLeft.Show()
		imgCenter.Show()
		imgRight.Show()

		self.imgLeft = imgLeft
		self.imgCenter = imgCenter
		self.imgRight = imgRight

		self.SetWidth(width)

	def SetWidth(self, width):
		self.imgCenter.SetRenderingRect(0.0, 0.0, float((width - 40) - 20) / 20, 0.0)
		self.imgCenter.SetPosition(20, 0)
		self.imgRight.SetPosition(width - 20, 0)

		self.SetSize(width, 21)

class TextLine(Window):
	def __init__(self, font = None):
		Window.__init__(self)
		self.max = 0
		if font == None:
			self.SetFontName(localeInfo.UI_DEF_FONT)
		else:
			self.SetFontName(font)

	def __del__(self):
		Window.__del__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterTextLine(self, layer)

	def SetMax(self, max):
		wndMgr.SetMax(self.hWnd, max)

	def SetLimitWidth(self, width):
		wndMgr.SetLimitWidth(self.hWnd, width)

	def SetMultiLine(self):
		wndMgr.SetMultiLine(self.hWnd, TRUE)

	def SetHorizontalAlignArabic(self):
		wndMgr.SetHorizontalAlign(self.hWnd, wndMgr.TEXT_HORIZONTAL_ALIGN_ARABIC)

	def SetHorizontalAlignLeft(self):
		wndMgr.SetHorizontalAlign(self.hWnd, wndMgr.TEXT_HORIZONTAL_ALIGN_LEFT)

	def SetHorizontalAlignRight(self):
		wndMgr.SetHorizontalAlign(self.hWnd, wndMgr.TEXT_HORIZONTAL_ALIGN_RIGHT)

	def SetHorizontalAlignCenter(self):
		wndMgr.SetHorizontalAlign(self.hWnd, wndMgr.TEXT_HORIZONTAL_ALIGN_CENTER)

	def SetVerticalAlignTop(self):
		wndMgr.SetVerticalAlign(self.hWnd, wndMgr.TEXT_VERTICAL_ALIGN_TOP)

	def SetVerticalAlignBottom(self):
		wndMgr.SetVerticalAlign(self.hWnd, wndMgr.TEXT_VERTICAL_ALIGN_BOTTOM)

	def SetVerticalAlignCenter(self):
		wndMgr.SetVerticalAlign(self.hWnd, wndMgr.TEXT_VERTICAL_ALIGN_CENTER)

	def SetSecret(self, Value=TRUE):
		wndMgr.SetSecret(self.hWnd, Value)

	def SetOutline(self, Value=TRUE):
		wndMgr.SetOutline(self.hWnd, Value)

	def SetFeather(self, value=TRUE):
		wndMgr.SetFeather(self.hWnd, value)

	def SetFontName(self, fontName):
		wndMgr.SetFontName(self.hWnd, fontName)

	def SetDefaultFontName(self):
		wndMgr.SetFontName(self.hWnd, localeInfo.UI_DEF_FONT)

	def SetFontColor(self, red, green, blue):
		wndMgr.SetFontColor(self.hWnd, red, green, blue)

	def SetPackedFontColor(self, color):
		wndMgr.SetFontColor(self.hWnd, color)

	def SetText(self, text):
		wndMgr.SetText(self.hWnd, text)

	def GetText(self):
		return wndMgr.GetText(self.hWnd)

	def GetTextSize(self):
		return wndMgr.GetTextSize(self.hWnd)

class MultiTextLine(TextLine):
	def __init__(self, parent, text, x, y, range = 15):
		TextLine.__init__(self)
		
		self.TextList = []
		self.CreateUI(parent, text, x, y, range)
		
	def __del__(self):
		del self.TextList
		TextLine.__del__(self)
	
	def CreateUI(self, parent, text, x, y, range):
		if "\n" in str(text):
			self.first_text = TextLine()
			self.first_text.SetParent(parent)
			self.first_text.SetPosition(x, y)		
			self.first_text.SetText(text.split("\n")[0])
			self.first_text.Show()
			self.TextList.append(self.first_text)
			for i in xrange(text.count("\n")):
				self.i = TextLine()
				self.i.SetParent(parent)
				self.i.SetPosition(x, (i + 1) * range + y)		
				self.i.SetText(text.split("\n")[i+1])
				self.i.Show()
				self.TextList.append(self.i)
				
	def SetText(self, text):
		tmpMiktar = 0
		if "\n" in str(text):
			for i in self.TextList:
				if i == self.TextList[0]:
					i.SetText(text.split("\n")[0])
				else:
					i.SetText(text.split("\n")[tmpMiktar])
				tmpMiktar += 1
	
	def Close(self):
		for i in self.TextList: i.Hide()
		self.__del__()

class TextLinkInterface(Window):
	COLOR_OVER = None
	COLOR_NORM = None
	COLOR_DOWN = None
	LINK = None
	
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)
		self.__txtText = None
		self.SetTextLink("")

	def __del__(self):
		Window.__del__(self)

	def SetTextLink(self, text):
		if not self.__txtText:
			self.__txtText = TextLine()
			self.__txtText.SetParent(self)
			self.__txtText.AddFlag("not_pick")
			self.__txtText.SetVerticalAlignCenter()
			self.__txtText.SetFontName(localeInfo.UI_DEF_FONT_LARGE)
			self.__txtText.Show()

		self.__txtText.SetText(text)

	def GetTextLink(self):
		if not self.__txtText:
			return ""
		
		return self.__txtText.GetText()
		
	def SetLink(self, link):	
		self.LINK = link
		
	def SetTextColors(self, colorNorm, colorOver, colorDown):
		self.COLOR_NORM = colorNorm
		self.COLOR_OVER = colorOver
		self.COLOR_DOWN = colorDown
		
	def OnMouseOverIn(self):
		self.__txtText.SetPackedFontColor(self.COLOR_OVER)
		
	def OnMouseOverOut(self):
		self.__txtText.SetPackedFontColor(self.COLOR_NORM)
		
	def OnMouseLeftButtonDown(self):
		self.__txtText.SetPackedFontColor(self.COLOR_DOWN)
		
	def OnMouseLeftButtonUp(self):
		import webbrowser
		webbrowser.open_new(self.LINK)
		
class TextLink(Window):

	NORMAL_COLOR =  0xffa08784
	OVER_COLOR = 0xffe6d0a2
	DOWN_COLOR = 0xffefe4cd

	def __init__(self):
		Window.__init__(self)
		
		self.eventFunc = None
		self.eventArgs = None
		
		self.text = TextLine()
		self.text.SetParent(self)
		self.text.SetPackedFontColor(self.NORMAL_COLOR)
		self.text.Show()
		
		self.underline = TextLine()
		self.underline.SetParent(self)
		self.underline.SetPackedFontColor(self.NORMAL_COLOR)
		self.underline.Hide()

	def __del__(self):
		Window.__del__(self)

	def SetText(self, text):
		self.text.SetText(text)
		self.SetSize(self.text.GetTextSize()[0], self.text.GetTextSize()[1])
		self.underline.SetPosition(-20, self.text.GetTextSize()[1])
		self.underline.SetWindowHorizontalAlignCenter()
		self.underline.SetSize(self.text.GetTextSize()[0], 0)

	def OnMouseOverIn(self):
		self.text.SetPackedFontColor(self.OVER_COLOR)
		self.underline.Show()

	def OnMouseOverOut(self):
		self.text.SetPackedFontColor(self.NORMAL_COLOR)
		derline.Hide()

	def OnMouseLeftButtonDown(self):
		self.text.SetPackedFontColor(self.DOWN_COLOR)
		self.underline.SetPackedFontColor(self.DOWN_COLOR)
		self.underline.Show()

	def OnMouseLeftButtonUp(self):
		if self.eventFunc:
			apply(self.eventFunc, self.eventArgs)
		self.OnMouseOverOut()

	def SetEvent(self, event, *args):
		self.eventFunc = event
		self.eventArgs = args

class EmptyCandidateWindow(Window):
	def __init__(self):
		Window.__init__(self)

	def __del__(self):
		Window.__init__(self)

	def Load(self):
		pass

	def SetCandidatePosition(self, x, y, textCount):
		pass

	def Clear(self):
		pass

	def Append(self, text):
		pass

	def Refresh(self):
		pass

	def Select(self):
		pass

class EditLine(TextLine):
	candidateWindowClassDict = {}

	def __init__(self):
		TextLine.__init__(self)

		self.eventReturn = Window.NoneMethod
		self.eventEscape = Window.NoneMethod
		self.eventTab = None
		self.numberMode = FALSE
		self.useIME = TRUE

		self.bCodePage = FALSE

		self.candidateWindowClass = None
		self.candidateWindow = None
		self.SetCodePage(app.GetDefaultCodePage())

		self.readingWnd = ReadingWnd()
		self.readingWnd.Hide()

	def __del__(self):
		TextLine.__del__(self)

		self.eventReturn = Window.NoneMethod
		self.eventEscape = Window.NoneMethod
		self.eventTab = None


	def SetCodePage(self, codePage):
		candidateWindowClass=EditLine.candidateWindowClassDict.get(codePage, EmptyCandidateWindow)
		self.__SetCandidateClass(candidateWindowClass)

	def __SetCandidateClass(self, candidateWindowClass):
		if self.candidateWindowClass==candidateWindowClass:
			return

		self.candidateWindowClass = candidateWindowClass
		self.candidateWindow = self.candidateWindowClass()
		self.candidateWindow.Load()
		self.candidateWindow.Hide()

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterTextLine(self, layer)

	def SAFE_SetReturnEvent(self, event):
		self.eventReturn = __mem_func__(event)		

	def SetReturnEvent(self, event):
		self.eventReturn = event

	def SetEscapeEvent(self, event):
		self.eventEscape = event

	def SetTabEvent(self, event):
		self.eventTab = event

	def SetMax(self, max):
		self.max = max
		wndMgr.SetMax(self.hWnd, self.max)
		ime.SetMax(self.max)
		self.SetUserMax(self.max)
		
	def SetUserMax(self, max):
		self.userMax = max
		ime.SetUserMax(self.userMax)

	def SetNumberMode(self):
		self.numberMode = TRUE

	#def AddExceptKey(self, key):
	#	ime.AddExceptKey(key)

	#def ClearExceptKey(self):
	#	ime.ClearExceptKey()

	def SetIMEFlag(self, flag):
		self.useIME = flag

	def SetText(self, text):
		wndMgr.SetText(self.hWnd, text)

		if self.IsFocus():
			ime.SetText(text)

	def Enable(self):
		wndMgr.ShowCursor(self.hWnd)

	def Disable(self):
		wndMgr.HideCursor(self.hWnd)

	def SetEndPosition(self):
		ime.MoveEnd()

	def OnSetFocus(self):
		Text = self.GetText()
		ime.SetText(Text)
		ime.SetMax(self.max)
		ime.SetUserMax(self.userMax)
		ime.SetCursorPosition(-1)
		if self.numberMode:
			ime.SetNumberMode()
		else:
			ime.SetStringMode()
		ime.EnableCaptureInput()
		if self.useIME:
			ime.EnableIME()
		else:
			ime.DisableIME()
		wndMgr.ShowCursor(self.hWnd, TRUE)

	def OnKillFocus(self):
		self.SetText(ime.GetText(self.bCodePage))
		self.OnIMECloseCandidateList()
		self.OnIMECloseReadingWnd()
		ime.DisableIME()
		ime.DisableCaptureInput()
		wndMgr.HideCursor(self.hWnd)

	def OnIMEChangeCodePage(self):
		self.SetCodePage(ime.GetCodePage())

	def OnIMEOpenCandidateList(self):
		self.candidateWindow.Show()
		self.candidateWindow.Clear()
		self.candidateWindow.Refresh()

		gx, gy = self.GetGlobalPosition()
		self.candidateWindow.SetCandidatePosition(gx, gy, len(self.GetText()))

		return TRUE

	def OnIMECloseCandidateList(self):
		self.candidateWindow.Hide()
		return TRUE

	def OnIMEOpenReadingWnd(self):
		gx, gy = self.GetGlobalPosition()
		textlen = len(self.GetText())-2		
		reading = ime.GetReading()
		readinglen = len(reading)
		self.readingWnd.SetReadingPosition( gx + textlen*6-24-readinglen*6, gy )
		self.readingWnd.SetText(reading)
		if ime.GetReadingError() == 0:
			self.readingWnd.SetTextColor(0xffffffff)
		else:
			self.readingWnd.SetTextColor(0xffff0000)
		self.readingWnd.SetSize(readinglen * 6 + 4, 19)
		self.readingWnd.Show()
		return TRUE

	def OnIMECloseReadingWnd(self):
		self.readingWnd.Hide()
		return TRUE

	def OnIMEUpdate(self):
		snd.PlaySound("sound/ui/type.wav")
		TextLine.SetText(self, ime.GetText(self.bCodePage))

	def OnIMETab(self):
		if self.eventTab:
			self.eventTab()
			return TRUE

		return FALSE

	def OnIMEReturn(self):
		snd.PlaySound("sound/ui/click.wav")
		self.eventReturn()

		return TRUE

	def OnPressEscapeKey(self):
		self.eventEscape()
		return TRUE

	def OnKeyDown(self, key):
		if app.DIK_F1 == key:
			return FALSE
		if app.DIK_F2 == key:
			return FALSE
		if app.DIK_F3 == key:
			return FALSE
		if app.DIK_F4 == key:
			return FALSE
		if app.DIK_LALT == key:
			return FALSE
		if app.DIK_SYSRQ == key:
			return FALSE
		if app.DIK_LCONTROL == key:
			return FALSE
		if app.DIK_V == key:
			if app.IsPressed(app.DIK_LCONTROL):
				ime.PasteTextFromClipBoard()

		return TRUE

	def OnKeyUp(self, key):
		if app.DIK_F1 == key:
			return FALSE
		if app.DIK_F2 == key:
			return FALSE
		if app.DIK_F3 == key:
			return FALSE
		if app.DIK_F4 == key:
			return FALSE
		if app.DIK_LALT == key:
			return FALSE
		if app.DIK_SYSRQ == key:
			return FALSE
		if app.DIK_LCONTROL == key:
			return FALSE

		return TRUE

	def OnIMEKeyDown(self, key):		
		# Left
		if app.VK_LEFT == key:
			ime.MoveLeft()
			return TRUE
		# Right
		if app.VK_RIGHT == key:
			ime.MoveRight()
			return TRUE

		# Home
		if app.VK_HOME == key:
			ime.MoveHome()
			return TRUE
		# End
		if app.VK_END == key:
			ime.MoveEnd()
			return TRUE

		# Delete
		if app.VK_DELETE == key:
			ime.Delete()
			TextLine.SetText(self, ime.GetText(self.bCodePage))
			return TRUE
			
		return TRUE

	#def OnMouseLeftButtonDown(self):
	#	self.SetFocus()
	def OnMouseLeftButtonDown(self):
		if FALSE == self.IsIn():
			return FALSE

		self.SetFocus()
		PixelPosition = wndMgr.GetCursorPosition(self.hWnd)
		ime.SetCursorPosition(PixelPosition)

class MarkBox(Window):
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

	def __del__(self):
		Window.__del__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterMarkBox(self, layer)

	def Load(self):
		wndMgr.MarkBox_Load(self.hWnd)

	def SetScale(self, scale):
		wndMgr.MarkBox_SetScale(self.hWnd, scale)

	def SetIndex(self, guildID):
		MarkID = guild.GuildIDToMarkID(guildID)
		wndMgr.MarkBox_SetImageFilename(self.hWnd, guild.GetMarkImageFilenameByMarkID(MarkID))
		wndMgr.MarkBox_SetIndex(self.hWnd, guild.GetMarkIndexByMarkID(MarkID))

	def SetAlpha(self, alpha):
		wndMgr.MarkBox_SetDiffuseColor(self.hWnd, 1.0, 1.0, 1.0, alpha)

class ImageBox(Window):
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		self.name=""
		self.eventDict={}
		self.eventFunc = {"mouse_click" : None, "mouse_over_in" : None, "mouse_over_out" : None}
		self.eventArgs = {"mouse_click" : None, "mouse_over_in" : None, "mouse_over_out" : None}
		self.argDict={}

	def __del__(self):
		Window.__del__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterImageBox(self, layer)

	def LoadImage(self, imageName):
		self.name=imageName
		return wndMgr.LoadImage(self.hWnd, imageName)

	def GetImageName(self):
		return self.name

	def SetAlpha(self, alpha):
		wndMgr.SetDiffuseColor(self.hWnd, 1.0, 1.0, 1.0, alpha)

	def GetWidth(self):
		return wndMgr.GetWidth(self.hWnd)

	def GetHeight(self):
		return wndMgr.GetHeight(self.hWnd)

	def ForceRender(self):
		wndMgr.ImageForceRender(self.hWnd)

	def OnMouseOverIn(self) :
		if self.eventFunc["mouse_over_in"] :
			apply(self.eventFunc["mouse_over_in"], self.eventArgs["mouse_over_in"])
		else:
			try:
				self.eventDict["MOUSE_OVER_IN"]()
			except KeyError:
				pass

	def OnMouseOverOut(self) :
		if self.eventFunc["mouse_over_out"] :
			apply(self.eventFunc["mouse_over_out"], self.eventArgs["mouse_over_out"])
		else :
			try:
				self.eventDict["MOUSE_OVER_OUT"]()
			except KeyError:
				pass

	def OnMouseLeftButtonUp(self):
		try:
			apply(self.eventDict["MOUSE_LEFT_UP"], self.argDict["MOUSE_LEFT_UP"])
		except KeyError:
			pass

	def OnMouseLeftButtonDown(self):
		try:
			apply(self.eventDict["MOUSE_LEFT_DOWN"], self.argDict["MOUSE_LEFT_DOWN"])
		except KeyError:
			pass

	def SAFE_SetStringEvent(self, event, func, *args):
		self.eventDict[event]=__mem_func__(func)
		self.argDict[event]=args

	def SAFE_SetMouseClickEvent(self, func, *args):
		self.eventDict["MOUSE_LEFT_DOWN"]=__mem_func__(func)
		self.argDict["MOUSE_LEFT_DOWN"]=args
		
	def SetEvent(self, func, *args) :
		result = self.eventFunc.has_key(args[0])		
		if result :
			self.eventFunc[args[0]] = func
			self.eventArgs[args[0]] = args
		else :
			print "[ERROR] ui.py SetEvent, Can`t Find has_key : %s" % args[0]

	"""def OnMouseLeftButtonUp(self) :
		if self.eventFunc["mouse_click"] :
			apply(self.eventFunc["mouse_click"], self.eventArgs["mouse_click"])"""

	def SetCoolTime(self, time):
		wndMgr.SetCoolTimeImageBox2(self.hWnd, time)

	def SetStartCoolTime(self, time):
		wndMgr.SetStartCoolTimeImageBox2(self.hWnd, time)
	
	
	def SetOnMouseLeftButtonUpEvent(self, event, *args):
		self.eventDict["MOUSE_LEFT_UP"] = __mem_func__(event)
		self.argDict["MOUSE_LEFT_UP"] = args

class ImageBox2(Window):
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		self.name=""
		self.eventDict={}
		self.eventFunc = {"mouse_click" : None, "mouse_over_in" : None, "mouse_over_out" : None}
		self.eventArgs = {"mouse_click" : None, "mouse_over_in" : None, "mouse_over_out" : None}
		self.argDict={}

	def __del__(self):
		Window.__del__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterImageBox(self, layer)

	def LoadImage(self, imageName):
		self.name=imageName
		return wndMgr.LoadImage(self.hWnd, imageName)

	def GetImageName(self):
		return self.name

	def SetAlpha(self, alpha):
		wndMgr.SetDiffuseColor(self.hWnd, 1.0, 1.0, 1.0, alpha)

	def GetWidth(self):
		return wndMgr.GetWidth(self.hWnd)

	def GetHeight(self):
		return wndMgr.GetHeight(self.hWnd)

	def ForceRender(self):
		wndMgr.ImageForceRender(self.hWnd)

	def OnMouseLeftButtonUp(self):
		try:
			apply(self.eventDict["MOUSE_LEFT_UP"], self.argDict["MOUSE_LEFT_UP"])
		except KeyError:
			pass

	def OnMouseLeftButtonDown(self):
		try:
			apply(self.eventDict["MOUSE_LEFT_DOWN"], self.argDict["MOUSE_LEFT_DOWN"])
		except KeyError:
			pass

	def SAFE_SetStringEvent(self, event, func, *args):
		self.eventDict[event]=__mem_func__(func)
		self.argDict[event]=args

	def SAFE_SetMouseClickEvent(self, func, *args):
		self.eventDict["MOUSE_LEFT_DOWN"]=__mem_func__(func)
		self.argDict["MOUSE_LEFT_DOWN"]=args
		
	def SetEvent(self, func, *args) :
		result = self.eventFunc.has_key(args[0])		
		if result :
			self.eventFunc[args[0]] = func
			self.eventArgs[args[0]] = args
		else :
			print "[ERROR] ui.py SetEvent, Can`t Find has_key : %s" % args[0]

	def OnMouseOverIn(self) :
		if self.eventFunc["mouse_over_in"] :
			apply(self.eventFunc["mouse_over_in"], self.eventArgs["mouse_over_in"])
		else:
			try:
				self.eventDict["MOUSE_OVER_IN"]()
			except KeyError:
				pass

	def OnMouseOverOut(self) :
		if self.eventFunc["mouse_over_out"] :
			apply(self.eventFunc["mouse_over_out"], self.eventArgs["mouse_over_out"])
		else :
			try:
				self.eventDict["MOUSE_OVER_OUT"]()
			except KeyError:
				pass

class ExpandedImageBox(ImageBox):
	def __init__(self, layer = "UI"):
		ImageBox.__init__(self, layer)

	def __del__(self):
		ImageBox.__del__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterExpandedImageBox(self, layer)

	def SetScale(self, xScale, yScale):
		wndMgr.SetScale(self.hWnd, xScale, yScale)

	def SetOrigin(self, x, y):
		wndMgr.SetOrigin(self.hWnd, x, y)

	def SetRotation(self, rotation):
		wndMgr.SetRotation(self.hWnd, rotation)

	def SetRenderingMode(self, mode):
		wndMgr.SetRenderingMode(self.hWnd, mode)

	# [0.0, 1.0] ������ ����ŭ �ۼ�Ʈ�� �׸��� �ʴ´�.
	def SetRenderingRect(self, left, top, right, bottom):
		wndMgr.SetRenderingRect(self.hWnd, left, top, right, bottom)

	def SetPercentage(self, curValue, maxValue):
		if maxValue:
			self.SetRenderingRect(0.0, 0.0, -1.0 + float(curValue) / float(maxValue), 0.0)
		else:
			self.SetRenderingRect(0.0, 0.0, 0.0, 0.0)

	def GetWidth(self):
		return wndMgr.GetWindowWidth(self.hWnd)

	def GetHeight(self):
		return wndMgr.GetWindowHeight(self.hWnd)

class AniImageBox(Window):
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)
		self.eventEndFrame = None

	def __del__(self):
		Window.__del__(self)
		self.eventEndFrame = None

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterAniImageBox(self, layer)

	def SetDelay(self, delay):
		wndMgr.SetDelay(self.hWnd, delay)

	def AppendImage(self, filename):
		wndMgr.AppendImage(self.hWnd, filename)
		
	def AppendImageScale(self, filename, scale_x, scale_y):
		wndMgr.AppendImageScale(self.hWnd, filename, scale_x, scale_y)

	def SetPercentage(self, curValue, maxValue):
		wndMgr.SetRenderingRect(self.hWnd, 0.0, 0.0, -1.0 + float(curValue) / float(maxValue), 0.0)

	def ResetFrame(self):
		wndMgr.ResetFrame(self.hWnd)
		
	def SetOnEndFrame(self, event):
		self.eventEndFrame = event

	def OnEndFrame(self):
		if self.eventEndFrame:
			self.eventEndFrame()

class CheckBox(Window):
	def __init__(self):
		Window.__init__(self)
		
		self.backgroundImage = None
		self.checkImage = None

		self.eventFunc = { "ON_CHECK" : None, "ON_UNCKECK" : None, }
		self.eventArgs = { "ON_CHECK" : None, "ON_UNCKECK" : None, }
	
		self.CreateElements()
		
	def __del__(self):
		Window.__del__(self)
		
		self.backgroundImage = None
		self.checkImage = None
		
		self.eventFunc = { "ON_CHECK" : None, "ON_UNCKECK" : None, }
		self.eventArgs = { "ON_CHECK" : None, "ON_UNCKECK" : None, }
		
	def CreateElements(self):
		self.backgroundImage = ImageBox()
		self.backgroundImage.SetParent(self)
		self.backgroundImage.AddFlag("not_pick")
		self.backgroundImage.LoadImage("d:/ymir work/ui/game/refine/checkbox.tga")
		self.backgroundImage.Show()
		
		self.checkImage = ImageBox()
		self.checkImage.SetParent(self)
		self.checkImage.AddFlag("not_pick")
		self.checkImage.SetPosition(0, -4)
		self.checkImage.LoadImage("d:/ymir work/ui/game/refine/checked.tga")
		self.checkImage.Hide()
		
		self.textInfo = TextLine()
		self.textInfo.SetParent(self)
		self.textInfo.SetPosition(20, -2)
		self.textInfo.Show()
		
		self.SetSize(self.backgroundImage.GetWidth() + self.textInfo.GetTextSize()[0], self.backgroundImage.GetHeight() + self.textInfo.GetTextSize()[1])
		
	def SetTextInfo(self, info):
		if self.textInfo:
			self.textInfo.SetText(info)
			
		self.SetSize(self.backgroundImage.GetWidth() + self.textInfo.GetTextSize()[0], self.backgroundImage.GetHeight() + self.textInfo.GetTextSize()[1])
		
	def SetCheckStatus(self, flag):
		if flag:
			self.checkImage.Show()
		else:
			self.checkImage.Hide()
	
	def GetCheckStatus(self):
		if self.checkImage:
			return self.checkImage.IsShow()
			
		return False
		
	def SetEvent(self, func, *args) :
		result = self.eventFunc.has_key(args[0])		
		if result :
			self.eventFunc[args[0]] = func
			self.eventArgs[args[0]] = args
		else :
			print "[ERROR] ui.py SetEvent, Can`t Find has_key : %s" % args[0]
		
	def OnMouseLeftButtonUp(self):
		if self.checkImage:
			if self.checkImage.IsShow():
				self.checkImage.Hide()

				if self.eventFunc["ON_UNCKECK"]:
					apply(self.eventFunc["ON_UNCKECK"], self.eventArgs["ON_UNCKECK"])
			else:
				self.checkImage.Show()

				if self.eventFunc["ON_CHECK"]:
					apply(self.eventFunc["ON_CHECK"], self.eventArgs["ON_CHECK"])

class MoveImageBox(ImageBox):
	def __init__(self, layer = "UI"):
		ImageBox.__init__(self, layer)
		self.end_move_event = None

	def __del__(self):
		ImageBox.__del__(self)
		self.end_move_event = None

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterMoveImageBox(self, layer)
		
	def MoveStart(self):
		wndMgr.MoveStart(self.hWnd)
	def MoveStop(self):
		wndMgr.MoveStop(self.hWnd)
	def GetMove(self):
		return wndMgr.GetMove(self.hWnd)
		
	def SetMovePosition(self, dst_x, dst_y):
		wndMgr.SetMovePosition(self.hWnd, dst_x, dst_y)
		
	def SetMoveSpeed(self, speed):
		wndMgr.SetMoveSpeed(self.hWnd, speed)
	
	def OnEndMove(self):
		if self.end_move_event:
			self.end_move_event()

	def SetEndMoveEvent(self, event):
		self.end_move_event = event
				
class Button(Window):
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		self.eventFunc = None
		self.eventArgs = None
		
		self.overFunc = None
		self.overArgs = None
		self.overOutFunc = None
		self.overOutArgs = None

		self.ButtonText = None
		self.ToolTipText = None

	def __del__(self):
		Window.__del__(self)

		self.eventFunc = None
		self.eventArgs = None

		self.overFunc = None
		self.overArgs = None
		self.overOutFunc = None
		self.overOutArgs = None

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterButton(self, layer)

	def SetUpVisual(self, filename):
		wndMgr.SetUpVisual(self.hWnd, filename)

	def SetOverVisual(self, filename):
		wndMgr.SetOverVisual(self.hWnd, filename)

	def SetDownVisual(self, filename):
		wndMgr.SetDownVisual(self.hWnd, filename)

	def SetDisableVisual(self, filename):
		wndMgr.SetDisableVisual(self.hWnd, filename)

	def GetUpVisualFileName(self):
		return wndMgr.GetUpVisualFileName(self.hWnd)

	def GetOverVisualFileName(self):
		return wndMgr.GetOverVisualFileName(self.hWnd)

	def GetDownVisualFileName(self):
		return wndMgr.GetDownVisualFileName(self.hWnd)

	def Flash(self):
		wndMgr.Flash(self.hWnd)

	def Enable(self):
		wndMgr.Enable(self.hWnd)

	def Disable(self):
		wndMgr.Disable(self.hWnd)

	def Down(self):
		wndMgr.Down(self.hWnd)

	def SetUp(self):
		wndMgr.SetUp(self.hWnd)

	def SAFE_SetEvent(self, func, *args):
		self.eventFunc = __mem_func__(func)
		self.eventArgs = args
		
	def SetEvent(self, func, *args):
		self.eventFunc = func
		self.eventArgs = args

	def SetTextColor(self, color):
		if not self.ButtonText:
			return
		self.ButtonText.SetPackedFontColor(color)

	def SetText(self, text, height = 4):

		if not self.ButtonText:
			textLine = TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(self.GetWidth()/2, self.GetHeight()/2)
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.Show()
			self.ButtonText = textLine

		self.ButtonText.SetText(text)

	def GetText(self):
		if not self.ButtonText:
			return ""
		return self.ButtonText.GetText()
	
	def SetText2(self, text, height = 4):

		if not self.ButtonText:
			textLine = TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(15, self.GetHeight()/2)
			textLine.SetHorizontalAlignCenter()
			textLine.Show()
			self.ButtonText = textLine

		self.ButtonText.SetText(text)

	def SetText3(self, text, height = 4):

		if not self.ButtonText:
			textLine = TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(self.GetWidth()/2, self.GetHeight()/2)
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.Hide()
			self.ButtonText = textLine

		self.ButtonText.SetText(text)

	def SetTextAlignLeft(self, text, height = 4):

		if not self.ButtonText:
			textLine = TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(27, self.GetHeight()/2)
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.Show()
			self.ButtonText = textLine

		#Quest Category
		self.ButtonText.SetText(text)
		self.ButtonText.SetPosition(27, self.GetHeight()/2)
		self.ButtonText.SetVerticalAlignCenter()
		self.ButtonText.SetHorizontalAlignLeft()
	
	def SetFormToolTipText(self, type, text, x, y):
		if not self.ToolTipText:		
			toolTip=createToolTipWindowDict[type]()
			toolTip.SetParent(self)
			toolTip.SetSize(0, 0)
			toolTip.SetHorizontalAlignCenter()
			toolTip.SetOutline()
			toolTip.Hide()
			toolTip.SetPosition(x + self.GetWidth()/2, y)
			self.ToolTipText=toolTip

		self.ToolTipText.SetText(text)

	def SetToolTipWindow(self, toolTip):		
		self.ToolTipText=toolTip		
		self.ToolTipText.SetParentProxy(self)

	def SetToolTipText(self, text, x=0, y = -19):
		self.SetFormToolTipText("TEXT", text, x, y)

	def CallEvent(self):
		snd.PlaySound("sound/ui/click.wav")

		if self.eventFunc:
			apply(self.eventFunc, self.eventArgs)

	def ShowToolTip(self):
		if self.ToolTipText:
			self.ToolTipText.Show()

	def HideToolTip(self):
		if self.ToolTipText:
			self.ToolTipText.Hide()
			
	def IsDown(self):
		return wndMgr.IsDown(self.hWnd)

	def OnMouseOverIn(self):
		if self.overFunc:
			apply(self.overFunc, self.overArgs)

	def OnMouseOverOut(self):
		if self.overOutFunc:
			apply(self.overOutFunc, self.overOutArgs)

	def SetOverEvent(self, func, *args):
		self.overFunc = func
		self.overArgs = args

	def SetOverOutEvent(self, func, *args):
		self.overOutFunc = func
		self.overOutArgs = args

class SelectButton(Button):
	def __init__(self):
		Button.__init__(self)

		self.eventUp = None
		self.eventDown = None
		self.eventDownArgs = None

	def __del__(self):
		Button.__del__(self)

		self.eventUp = None
		self.eventDown = None
		self.eventDownArgs = None

	def SetToggleDownEvent(self, event, args):
		self.eventDown = event
		self.eventDownArgs = args

	def SetToggleUpEvent(self, event):
		self.eventUp = event

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterToggleButton(self, layer)

	def OnToggleUp(self):
		if self.eventUp:
			self.eventUp()

	def OnToggleDown(self):
		if self.eventDown:
			self.eventDown(self.eventDownArgs)

class RadioButton(Button):
	def __init__(self):
		Button.__init__(self)

	def __del__(self):
		Button.__del__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterRadioButton(self, layer)

class RadioButtonGroup:
	def __init__(self):
		self.buttonGroup = []
		self.selectedBtnIdx = -1
	
	def __del__(self):
		for button, ue, de in self.buttonGroup:
			button.__del__()
 	
 	def Show(self):
 		for (button, selectEvent, unselectEvent) in self.buttonGroup:
			button.Show()
 	
 	def Hide(self):
 		for (button, selectEvent, unselectEvent) in self.buttonGroup:
			button.Hide()
 	
 	def SetText(self, idx, text):
 		if idx >= len(self.buttonGroup):
			return
		(button, selectEvent, unselectEvent) = self.buttonGroup[idx]
		button.SetText(text)
 	
 	def OnClick(self, btnIdx):
 		if btnIdx == self.selectedBtnIdx:
 			return
 		(button, selectEvent, unselectEvent) = self.buttonGroup[self.selectedBtnIdx]
 		if unselectEvent:
 			unselectEvent()
 		button.SetUp()
 		
 		self.selectedBtnIdx = btnIdx
 		(button, selectEvent, unselectEvent) = self.buttonGroup[btnIdx]
 		if selectEvent:
 			selectEvent()

 		button.Down()
 		
	def AddButton(self, button, selectEvent, unselectEvent):
		i = len(self.buttonGroup)
		button.SetEvent(lambda : self.OnClick(i))
		self.buttonGroup.append([button, selectEvent, unselectEvent])
		button.SetUp()

	def Create(rawButtonGroup):
		radioGroup = RadioButtonGroup()
		for (button, selectEvent, unselectEvent) in rawButtonGroup:
			radioGroup.AddButton(button, selectEvent, unselectEvent)
		
		radioGroup.OnClick(0)
		
		return radioGroup
		
	Create=staticmethod(Create)

class ToggleButton(Button):
	def __init__(self):
		Button.__init__(self)

		self.eventUp = None
		self.eventDown = None

	def __del__(self):
		Button.__del__(self)

		self.eventUp = None
		self.eventDown = None

	def SetToggleUpEvent(self, event):
		self.eventUp = event

	def SetToggleDownEvent(self, event):
		self.eventDown = event

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterToggleButton(self, layer)

	def OnToggleUp(self):
		if self.eventUp:
			self.eventUp()

	def OnToggleDown(self):
		if self.eventDown:
			self.eventDown()

class DragButton(Button):
	def __init__(self):
		Button.__init__(self)
		self.AddFlag("movable")

		self.callbackEnable = TRUE
		self.eventMove = lambda: None

	def __del__(self):
		Button.__del__(self)

		self.eventMove = lambda: None

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterDragButton(self, layer)

	def SetMoveEvent(self, event):
		self.eventMove = event

	def SetRestrictMovementArea(self, x, y, width, height):
		wndMgr.SetRestrictMovementArea(self.hWnd, x, y, width, height)

	def TurnOnCallBack(self):
		self.callbackEnable = TRUE

	def TurnOffCallBack(self):
		self.callbackEnable = FALSE

	def OnMove(self):
		if self.callbackEnable:
			self.eventMove()

class NumberLine(Window):

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

	def __del__(self):
		Window.__del__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterNumberLine(self, layer)

	def SetHorizontalAlignCenter(self):
		wndMgr.SetNumberHorizontalAlignCenter(self.hWnd)

	def SetHorizontalAlignRight(self):
		wndMgr.SetNumberHorizontalAlignRight(self.hWnd)

	def SetPath(self, path):
		wndMgr.SetPath(self.hWnd, path)

	def SetNumber(self, number):
		wndMgr.SetNumber(self.hWnd, number)

###################################################################################################
## PythonScript Element
###################################################################################################

class Box(Window):

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterBox(self, layer)

	def SetColor(self, color):
		wndMgr.SetColor(self.hWnd, color)

class Bar(Window):

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterBar(self, layer)

	def SetColor(self, color):
		wndMgr.SetColor(self.hWnd, color)

class Line(Window):

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterLine(self, layer)

	def SetColor(self, color):
		wndMgr.SetColor(self.hWnd, color)

class SlotBar(Window):

	def __init__(self):
		Window.__init__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterBar3D(self, layer)

## Same with SlotBar
class Bar3D(Window):

	def __init__(self):
		Window.__init__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterBar3D(self, layer)

	def SetColor(self, left, right, center):
		wndMgr.SetColor(self.hWnd, left, right, center)

class SlotWindow(Window):

	def __init__(self):
		Window.__init__(self)

		self.StartIndex = 0

		self.eventSelectEmptySlot = None
		self.eventSelectItemSlot = None
		self.eventUnselectEmptySlot = None
		self.eventUnselectItemSlot = None
		self.eventUseSlot = None
		self.eventOverInItem = None
		self.eventOverOutItem = None
		self.eventOverInItem2 = None
		self.eventOverInItem3 = None
		self.eventPressedSlotButton = None

	def __del__(self):
		Window.__del__(self)

		self.eventSelectEmptySlot = None
		self.eventSelectItemSlot = None
		self.eventUnselectEmptySlot = None
		self.eventUnselectItemSlot = None
		self.eventUseSlot = None
		self.eventOverInItem = None
		self.eventOverOutItem = None
		self.eventOverInItem2 = None
		self.eventOverInItem3 = None
		self.eventPressedSlotButton = None

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterSlotWindow(self, layer)

	def SetSlotStyle(self, style):
		wndMgr.SetSlotStyle(self.hWnd, style)

	def HasSlot(self, slotIndex):
		return wndMgr.HasSlot(self.hWnd, slotIndex)

	def SetSlotBaseImage(self, imageFileName, r, g, b, a):
		wndMgr.SetSlotBaseImage(self.hWnd, imageFileName, r, g, b, a)

	def SetSlotBaseImageScale(self, imageFileName, r, g, b, a, sx, sy):
		wndMgr.SetSlotBaseImageScale(self.hWnd, imageFileName, r, g, b, a, sx, sy)

	def SetCoverButton(self,\
						slotIndex,\
						upName="d:/ymir work/ui/public/slot_cover_button_01.sub",\
						overName="d:/ymir work/ui/public/slot_cover_button_02.sub",\
						downName="d:/ymir work/ui/public/slot_cover_button_03.sub",\
						disableName="d:/ymir work/ui/public/slot_cover_button_04.sub",\
						LeftButtonEnable = FALSE,\
						RightButtonEnable = TRUE):
		wndMgr.SetCoverButton(self.hWnd, slotIndex, upName, overName, downName, disableName, LeftButtonEnable, RightButtonEnable)

	def EnableCoverButton(self, slotIndex):
		wndMgr.EnableCoverButton(self.hWnd, slotIndex)

	def DisableCoverButton(self, slotIndex):
		wndMgr.DisableCoverButton(self.hWnd, slotIndex)

	def SetAlwaysRenderCoverButton(self, slotIndex, bAlwaysRender = True):
		wndMgr.SetAlwaysRenderCoverButton(self.hWnd, slotIndex, bAlwaysRender)

	def AppendSlotButton(self, upName, overName, downName):
		wndMgr.AppendSlotButton(self.hWnd, upName, overName, downName)

	def ShowSlotButton(self, slotNumber):
		wndMgr.ShowSlotButton(self.hWnd, slotNumber)

	def HideAllSlotButton(self):
		wndMgr.HideAllSlotButton(self.hWnd)

	def AppendRequirementSignImage(self, filename):
		wndMgr.AppendRequirementSignImage(self.hWnd, filename)

	def ShowRequirementSign(self, slotNumber):
		wndMgr.ShowRequirementSign(self.hWnd, slotNumber)

	def HideRequirementSign(self, slotNumber):
		wndMgr.HideRequirementSign(self.hWnd, slotNumber)

	def ActivateSlot(self, slotNumber, r = 1.0, g = 1.0, b = 1.0, a = 1.0):
		wndMgr.ActivateEffect(self.hWnd, slotNumber, r, g, b, a)

	def DeactivateSlot(self, slotNumber):
		wndMgr.DeactivateSlot(self.hWnd, slotNumber)

	def ShowSlotBaseImage(self, slotNumber):
		wndMgr.ShowSlotBaseImage(self.hWnd, slotNumber)

	def HideSlotBaseImage(self, slotNumber):
		wndMgr.HideSlotBaseImage(self.hWnd, slotNumber)

	def SAFE_SetButtonEvent(self, button, state, event):
		if "LEFT"==button:
			if "EMPTY"==state:
				self.eventSelectEmptySlot=__mem_func__(event)
			elif "EXIST"==state:
				self.eventSelectItemSlot=__mem_func__(event)
			elif "ALWAYS"==state:
				self.eventSelectEmptySlot=__mem_func__(event)
				self.eventSelectItemSlot=__mem_func__(event)
		elif "RIGHT"==button:
			if "EMPTY"==state:
				self.eventUnselectEmptySlot=__mem_func__(event)
			elif "EXIST"==state:
				self.eventUnselectItemSlot=__mem_func__(event)
			elif "ALWAYS"==state:
				self.eventUnselectEmptySlot=__mem_func__(event)
				self.eventUnselectItemSlot=__mem_func__(event)

	def SetSelectEmptySlotEvent(self, empty):
		self.eventSelectEmptySlot = empty
	
	def SetSelectItemSlotEvent(self, item):
		self.eventSelectItemSlot = item

	def SetUnselectEmptySlotEvent(self, empty):
		self.eventUnselectEmptySlot = empty

	def SetUnselectItemSlotEvent(self, item):
		self.eventUnselectItemSlot = item

	def SetUseSlotEvent(self, use):
		self.eventUseSlot = use

	def SetOverInItemEvent(self, event):
		self.eventOverInItem = event

	def SetOverInItemEvent2(self, event):
		self.eventOverInItem2 = event

	def SetOverInItemEvent3(self, event):
		self.eventOverInItem3 = event

	def SetOverOutItemEvent(self, event):
		self.eventOverOutItem = event

	def SetPressedSlotButtonEvent(self, event):
		self.eventPressedSlotButton = event

	def GetSlotCount(self):
		return wndMgr.GetSlotCount(self.hWnd)

	def SetUseMode(self, flag):
		"TRUE�϶��� ItemToItem �� �������� �����ش�"
		wndMgr.SetUseMode(self.hWnd, flag)

	def SetUsableItem(self, flag): 
		"TRUE�� ���� ����Ų �������� ItemToItem ���� �����ϴ�"
		wndMgr.SetUsableItem(self.hWnd, flag)

	## Slot
	def SetSlotCoolTime(self, slotIndex, coolTime, elapsedTime = 0.0):
		wndMgr.SetSlotCoolTime(self.hWnd, slotIndex, coolTime, elapsedTime)

	def StoreSlotCoolTime(self, key, slotIndex, coolTime, elapsedTime = 0.0):
		wndMgr.StoreSlotCoolTime(self.hWnd, key, slotIndex, coolTime, elapsedTime)

	def RestoreSlotCoolTime(self, key):
		wndMgr.RestoreSlotCoolTime(self.hWnd, key)

	def DisableSlot(self, slotIndex):
		wndMgr.DisableSlot(self.hWnd, slotIndex)

	def EnableSlot(self, slotIndex):
		wndMgr.EnableSlot(self.hWnd, slotIndex)

	def LockSlot(self, slotIndex):
		wndMgr.LockSlot(self.hWnd, slotIndex)

	def UnlockSlot(self, slotIndex):
		wndMgr.UnlockSlot(self.hWnd, slotIndex)

	def RefreshSlot(self):
		wndMgr.RefreshSlot(self.hWnd)

	def ClearSlot(self, slotNumber):
		wndMgr.ClearSlot(self.hWnd, slotNumber)

	def ClearAllSlot(self):
		wndMgr.ClearAllSlot(self.hWnd)

	def AppendSlot(self, index, x, y, width, height):
		wndMgr.AppendSlot(self.hWnd, index, x, y, width, height)

	def SetSlot(self, slotIndex, itemIndex, width, height, icon, diffuseColor = (1.0, 1.0, 1.0, 1.0)):
		wndMgr.SetSlot(self.hWnd, slotIndex, itemIndex, width, height, icon, diffuseColor)

	def SetSlotScale(self, slotIndex, itemIndex, width, height, icon, sx, sy, diffuseColor = (1.0, 1.0, 1.0, 1.0)):
		wndMgr.SetSlotScale(self.hWnd, slotIndex, itemIndex, width, height, icon, diffuseColor, sx, sy)

	def SetSlotCount(self, slotNumber, count):
		wndMgr.SetSlotCount(self.hWnd, slotNumber, count)
		
	def SetSlotCountNew(self, slotNumber, grade, count):
		wndMgr.SetSlotCountNew(self.hWnd, slotNumber, grade, count)

	def SetItemSlot(self, renderingSlotNumber, ItemIndex, ItemCount = 0, diffuseColor = (1.0, 1.0, 1.0, 1.0)):
		if 0 == ItemIndex or None == ItemIndex:
			wndMgr.ClearSlot(self.hWnd, renderingSlotNumber)
			return

		item.SelectItem(ItemIndex)
		itemIcon = item.GetIconImage()

		item.SelectItem(ItemIndex)
		(width, height) = item.GetItemSize()
		
		wndMgr.SetSlot(self.hWnd, renderingSlotNumber, ItemIndex, width, height, itemIcon, diffuseColor)
		wndMgr.SetSlotCount(self.hWnd, renderingSlotNumber, ItemCount)
		if item.GetItemType() in (item.ITEM_TYPE_WEAPON, item.ITEM_TYPE_ARMOR):
			attrSlot = []
			yeni_x2 = translate.inventoryPageIndexo*player.INVENTORY_PAGE_SIZE + renderingSlotNumber
			attrSlot2 = [player.GetItemAttribute(yeni_x2, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]
			if attrSlot2 and attrSlot2[10][1] >= 10:
				wndMgr.SetSlotLevelImage(self.hWnd, renderingSlotNumber, "system/ingame_convert_mark.tga")

	def SetSkillSlot(self, renderingSlotNumber, skillIndex, skillLevel):

		skillIcon = skill.GetIconImage(skillIndex)

		if 0 == skillIcon:
			wndMgr.ClearSlot(self.hWnd, renderingSlotNumber)
			return

		wndMgr.SetSlot(self.hWnd, renderingSlotNumber, skillIndex, 1, 1, skillIcon)
		wndMgr.SetSlotCount(self.hWnd, renderingSlotNumber, skillLevel)

	def SetSkillSlotNew(self, renderingSlotNumber, skillIndex, skillGrade, skillLevel):
		
		skillIcon = skill.GetIconImageNew(skillIndex, skillGrade)

		if 0 == skillIcon:
			wndMgr.ClearSlot(self.hWnd, renderingSlotNumber)
			return

		wndMgr.SetSlot(self.hWnd, renderingSlotNumber, skillIndex, 1, 1, skillIcon)

	def SetEmotionSlot(self, renderingSlotNumber, emotionIndex):
		import player
		icon = player.GetEmotionIconImage(emotionIndex)

		if 0 == icon:
			wndMgr.ClearSlot(self.hWnd, renderingSlotNumber)
			return

		wndMgr.SetSlot(self.hWnd, renderingSlotNumber, emotionIndex, 1, 1, icon)

	## Event
	def OnSelectEmptySlot(self, slotNumber):
		if self.eventSelectEmptySlot:
			self.eventSelectEmptySlot(slotNumber)
	
	def OnSelectItemSlot(self, slotNumber):
		if self.eventSelectItemSlot:
			self.eventSelectItemSlot(slotNumber)

	def OnUnselectEmptySlot(self, slotNumber):
		if self.eventUnselectEmptySlot:
			self.eventUnselectEmptySlot(slotNumber)

	def OnUnselectItemSlot(self, slotNumber):
		if self.eventUnselectItemSlot:
			self.eventUnselectItemSlot(slotNumber)

	def OnUseSlot(self, slotNumber):
		if self.eventUseSlot:
			self.eventUseSlot(slotNumber)

	def OnOverInItem(self, slotNumber,vnum=0,itemID=0):
		if self.eventOverInItem:
			self.eventOverInItem(slotNumber)
		if self.eventOverInItem2 and vnum>0:
			self.eventOverInItem2(vnum)
		if self.eventOverInItem3 and itemID>0:
			self.eventOverInItem3(itemID)

	def OnOverOutItem(self):
		if self.eventOverOutItem:
			self.eventOverOutItem()

	def OnPressedSlotButton(self, slotNumber):
		if self.eventPressedSlotButton:
			self.eventPressedSlotButton(slotNumber)

	def GetStartIndex(self):
		return 0

class GridSlotWindow(SlotWindow):

	def __init__(self):
		SlotWindow.__init__(self)

		self.startIndex = 0

	def __del__(self):
		SlotWindow.__del__(self)

	def RegisterWindow(self, layer):
		self.hWnd = wndMgr.RegisterGridSlotWindow(self, layer)

	def ArrangeSlot(self, StartIndex, xCount, yCount, xSize, ySize, xBlank, yBlank):

		self.startIndex = StartIndex

		wndMgr.ArrangeSlot(self.hWnd, StartIndex, xCount, yCount, xSize, ySize, xBlank, yBlank)
		self.startIndex = StartIndex

	def GetStartIndex(self):
		return self.startIndex

class TitleBar(Window):

	BLOCK_WIDTH = 32
	BLOCK_HEIGHT = 23

	def __init__(self):
		Window.__init__(self)
		self.AddFlag("attach")

	def __del__(self):
		Window.__del__(self)

	def MakeTitleBar(self, width, color):

		## ���� Color�� ����ϰ� ���� ����

		width = max(64, width)

		imgLeft = ImageBox()
		imgCenter = ExpandedImageBox()
		imgRight = ImageBox()
		imgLeft.AddFlag("not_pick")
		imgCenter.AddFlag("not_pick")
		imgRight.AddFlag("not_pick")
		imgLeft.SetParent(self)
		imgCenter.SetParent(self)
		imgRight.SetParent(self)

		if localeInfo.IsARABIC():
			imgLeft.LoadImage("locale/ae/ui/pattern/titlebar_left.tga")
			imgCenter.LoadImage("locale/ae/ui/pattern/titlebar_center.tga")
			imgRight.LoadImage("locale/ae/ui/pattern/titlebar_right.tga")
		else:
			imgLeft.LoadImage("d:/ymir work/ui/pattern/titlebar_left.tga")
			imgCenter.LoadImage("d:/ymir work/ui/pattern/titlebar_center.tga")
			imgRight.LoadImage("d:/ymir work/ui/pattern/titlebar_right.tga")

		imgLeft.Show()
		imgCenter.Show()
		imgRight.Show()

		btnClose = Button()
		btnClose.SetParent(self)
		btnClose.SetUpVisual("d:/ymir work/ui/public/close_button_01.sub")
		btnClose.SetOverVisual("d:/ymir work/ui/public/close_button_02.sub")
		btnClose.SetDownVisual("d:/ymir work/ui/public/close_button_03.sub")
		btnClose.SetToolTipText(localeInfo.UI_CLOSE, 0, -23)
		btnClose.Show()

		self.imgLeft = imgLeft
		self.imgCenter = imgCenter
		self.imgRight = imgRight
		self.btnClose = btnClose

		self.SetWidth(width)

	def SetWidth(self, width):
		self.imgCenter.SetRenderingRect(0.0, 0.0, float((width - self.BLOCK_WIDTH*2) - self.BLOCK_WIDTH) / self.BLOCK_WIDTH, 0.0)
		self.imgCenter.SetPosition(self.BLOCK_WIDTH, 0)
		self.imgRight.SetPosition(width - self.BLOCK_WIDTH, 0)

		if localeInfo.IsARABIC():
			self.btnClose.SetPosition(3, 3)
		else:
			self.btnClose.SetPosition(width - self.btnClose.GetWidth() - 3, 3)
			
		self.SetSize(width, self.BLOCK_HEIGHT)

	def SetCloseEvent(self, event):
		self.btnClose.SetEvent(event)

	def CloseButtonHide(self) :
		self.imgRight.LoadImage("d:/ymir work/ui/pattern/titlebar_right.tga")
		self.btnClose.Hide()

class TitleBarA(TitleBar):
	BLOCK_WIDTH = 32
	BLOCK_HEIGHT = 23
	IS_SCALE = True
	
	POSITIONS = {
		"Q" : [22, 3],
		"C" : [3, 3]
	}
	
	IMAGES = {
		'TITLE' : {
			'PATH' : "d:/ymir work/ui/game/shop_editor",
			'LEFT' : 'titlebar_a_left.sub',
			'CENTER' : 'titlebar_a_center.sub',
			'RIGHT' : 'titlebar_a_right.sub',
			'RIGHT_SPECIAL' : 'titlebar_a_right.sub'
		},
		
		'CLOSE' : {
			'PATH' : "d:/ymir work/ui/game/shop_editor",
			'NORMAL' : 'x_mark_01.sub',
			'OVER' : 'x_mark_02.sub',
			'DOWN' : 'x_mark_03.sub'
		},
		
		'INFO' : {
			'PATH' : "d:/ymir work/ui/pattern",
			'NORMAL' : 'q_mark_01.tga',
			'OVER' : 'q_mark_02.tga',
			'DOWN' : 'q_mark_02.tga'
		},
	}
	
	def __init__(self):
		TitleBar.__init__(self)

		
	def __del__(self):
		TitleBar.__del__(self)
		

	def SetCloseEvent(self, event):
		self.btnClose.SetEvent(event)

	def SetText(self, text):
		if not self.ButtonText:
			textLine = TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(self.GetWidth()/2, self.GetHeight()/2)
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.Show()
			self.ButtonText = textLine

		self.ButtonText.SetText(text)
		
		
class world_gui_ierik(Window):

	CORNER_WIDTH = 8
	CORNER_HEIGHT = 8
	LINE_WIDTH = 8
	LINE_HEIGHT = 8

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self):
		Window.__init__(self)
		
		self.MakeBoard("worldGui/sistemler/gui_popup/world_", "worldGui/sistemler/gui_popup/world_")
		self.MakeBase()

	def MakeBoard(self, cornerPath, linePath):

		CornerFileNames = [ cornerPath+dir+".png" for dir in ("popup_kategori", "popup_kategori","popup_kategori", "popup_kategori", ) ]
		LineFileNames = [ linePath+dir+".png" for dir in ("popup_kategori", "popup_kategori", "popup_kategori", "popup_kategori", ) ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)

	def MakeBase(self):
		self.Base = ExpandedImageBox()
		self.Base.AddFlag("not_pick")
		self.Base.LoadImage("worldGui/sistemler/gui_popup/world_popup_kategori.png")
		self.Base.SetParent(self)
		self.Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Base.Show()

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):

		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LB].SetPosition(0, height - self.CORNER_HEIGHT)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH, height - self.CORNER_HEIGHT)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)

		if self.Base:
			self.Base.SetRenderingRect(0, 0, horizontalShowingPercentage, verticalShowingPercentage)
		
		


class HorizontalBar(Window):

	BLOCK_WIDTH = 32
	BLOCK_HEIGHT = 17

	def __init__(self):
		Window.__init__(self)
		self.AddFlag("attach")

	def __del__(self):
		Window.__del__(self)

	def Create(self, width):

		width = max(96, width)

		imgLeft = ImageBox()
		imgLeft.SetParent(self)
		imgLeft.AddFlag("not_pick")
		imgLeft.LoadImage("d:/ymir work/ui/pattern/horizontalbar_left.tga")
		imgLeft.Show()

		imgCenter = ExpandedImageBox()
		imgCenter.SetParent(self)
		imgCenter.AddFlag("not_pick")
		imgCenter.LoadImage("d:/ymir work/ui/pattern/horizontalbar_center.tga")
		imgCenter.Show()

		imgRight = ImageBox()
		imgRight.SetParent(self)
		imgRight.AddFlag("not_pick")
		imgRight.LoadImage("d:/ymir work/ui/pattern/horizontalbar_right.tga")
		imgRight.Show()

		self.imgLeft = imgLeft
		self.imgCenter = imgCenter
		self.imgRight = imgRight
		self.SetWidth(width)

	def SetWidth(self, width):
		self.imgCenter.SetRenderingRect(0.0, 0.0, float((width - self.BLOCK_WIDTH*2) - self.BLOCK_WIDTH) / self.BLOCK_WIDTH, 0.0)
		self.imgCenter.SetPosition(self.BLOCK_WIDTH, 0)
		self.imgRight.SetPosition(width - self.BLOCK_WIDTH, 0)
		self.SetSize(width, self.BLOCK_HEIGHT)

class SubTitleBar(Button):
	def __init__(self):
		Button.__init__(self)

	def __del__(self):
		Button.__del__(self)

	def MakeSubTitleBar(self, width, color):
		quest = __import__(pythonApi.GetModuleName("quest"))
		
		## ?? Color? ???? ?? ??

		width = max(64, width)

		self.SetUpVisual("d:/ymir work/ui/game/quest/quest_category.tga")
		self.SetOverVisual("d:/ymir work/ui/game/quest/quest_category.tga")
		self.SetDownVisual("d:/ymir work/ui/game/quest/quest_category.tga")
		self.Show()
		
		#??? ??
		isOpenImage = ImageBox()
		isOpenImage.SetParent(self)
		isOpenImage.LoadImage("d:/ymir work/ui/game/quest/quest_category_close.tga")
		isOpenImage.SetPosition(8,5)
		isOpenImage.AddFlag("not_pick")
		isOpenImage.Show()
		
		self.isOpenImage = isOpenImage
		
		self.SetWidth(width)
		self.isOpen = []
		for i in xrange(quest.QUEST_CATEGORY_MAX_NUM):
			self.isOpen.append(False)

	def SetWidth(self, width):
		self.SetPosition(32, 0)
		
		#if localeInfo.IsARABIC():
		#	self.btnImage.SetPosition(width - self.btnClose.GetWidth() - 3, 3)
		#else:
		#    self.btnImage.SetPosition(0, 3)
		
		self.SetSize(width, 23)

	def CallEvent(self):
		super(SubTitleBar, self).CallEvent("SubTitleBar")
		self.OnClickEvent()

	def OnClickEvent(self):
		#self.ToggleSubImage()
		pass

	def ToggleSubImage(self):
		import uiCharacter
		self.wndCharacter = uiCharacter.CharacterWindow()
		backup_category = -1
		isSameFlag = False
		backup_isOpenImage = ImageBox()
		
		now_selected_category = int(self.GetWindowName()[16:17])
		if backup_category > 0 and backup_category == now_selected_category:
			isSameFlag = True

		#print "=ToggleSubImage Test= backup_category, backup_isOpenImage, now_selected_category, self.isOpen[now_selected_category], isSameFlag",backup_category, backup_isOpenImage, now_selected_category, self.isOpen[now_selected_category], isSameFlag

		if self.isOpen[now_selected_category]:
			# ????? ???.
			self.isOpenImage.LoadImage("d:/ymir work/ui/game/quest/quest_category_close.tga")
			self.isOpenImage.SetWindowName('category_close')
			self.isOpen[now_selected_category] = False
		elif self.wndCharacter.CanOpenQuestCategory(now_selected_category) == True and self.isOpen[now_selected_category] == False:
			self.isOpenImage.LoadImage("d:/ymir work/ui/game/quest/quest_category_open.tga")
			self.isOpenImage.SetWindowName('category_open')
			self.isOpen[now_selected_category] = True
		self.isOpenImage.SetPosition(8,5)
		self.isOpenImage.AddFlag("not_pick")
		self.isOpenImage.Show()
		
		#?????
		backup_category = now_selected_category
		backup_isOpenImage = self.isOpenImage

	def OpenSubImage(self):		
		now_selected_category = int(self.GetWindowName()[16:17])	
 
		self.isOpenImage.LoadImage("d:/ymir work/ui/game/quest/quest_category_open.tga")
		self.isOpenImage.SetWindowName('category_open')
		self.isOpen[now_selected_category] = True

		self.isOpenImage.SetPosition(8,5)
		self.isOpenImage.AddFlag("not_pick")
		self.isOpenImage.Show()

	def CloseSubImage(self):		
		now_selected_category = int(self.GetWindowName()[16:17])
		
		self.isOpenImage.LoadImage("d:/ymir work/ui/game/quest/quest_category_close.tga")
		self.isOpenImage.SetWindowName('category_close')
		self.isOpen[now_selected_category] = False

		self.isOpenImage.SetPosition(8,5)
		self.isOpenImage.AddFlag("not_pick")
		self.isOpenImage.Show()
		
class ListBar(Button):

	def __init__(self):
		Button.__init__(self)

	def __del__(self):
		Button.__del__(self)

	def MakeListBar(self, width, color):

		## ?? Color? ???? ?? ??

		width = max(64, width)

		self.Show()
		
		checkbox = ImageBox()
		checkbox.SetParent(self)
		checkbox.LoadImage("d:/ymir work/ui/game/quest/quest_checkbox.tga")
		checkbox.SetPosition(10,5)
		checkbox.AddFlag("not_pick")
		checkbox.Show()

		self.checkbox = checkbox
		
		self.SetWidth(width)
		self.isChecked = False

	def SetWidth(self, width):
		self.SetPosition(32, 0)
		
		#if localeInfo.IsARABIC():
		#	self.btnImage.SetPosition(width - self.btnClose.GetWidth() - 3, 3)
		#else:
		#    self.btnImage.SetPosition(0, 3)
		
		self.SetSize(width, 23)

	def CallEvent(self):
		self.OnClickEvent()
		super(ListBar, self).CallEvent()

	def OnClickEvent(self):
		print "========================OnClickEvent========================",self.isChecked
		checked_image = ImageBox()
		checked_image.SetParent(self)
		checked_image.LoadImage("d:/ymir work/ui/game/quest/quest_checked.tga")
		checked_image.SetPosition(10,1)
		checked_image.AddFlag("not_pick")

		checked_image.Show()
		
		self.isChecked = True
		self.checked_image = checked_image

	def SetSlot(self, slotIndex, itemIndex, width, height, icon, diffuseColor = (1.0, 1.0, 1.0, 1.0)):
		wndMgr.SetSlot(self.hWnd, slotIndex, itemIndex, width, height, icon, diffuseColor)

class Gauge(Window):

	SLOT_WIDTH = 16
	SLOT_HEIGHT = 7

	GAUGE_TEMPORARY_PLACE = 12
	GAUGE_WIDTH = 16

	def __init__(self):
		Window.__init__(self)
		self.width = 0
		self.showtooltipevent = None
		self.showtooltiparg = None
		self.hidetooltipevent = None
		self.hidetooltiparg = None
		self.ToolTipText = None

	def __del__(self):
		Window.__del__(self)
		self.showtooltipevent = None
		self.showtooltiparg = None
		self.hidetooltipevent = None
		self.hidetooltiparg = None

	def MakeGauge(self, width, color):

		self.width = max(48, width)

		imgSlotLeft = ImageBox()
		imgSlotLeft.SetParent(self)
		imgSlotLeft.LoadImage("d:/ymir work/ui/pattern/gauge_slot_left.tga")
		imgSlotLeft.Show()

		imgSlotRight = ImageBox()
		imgSlotRight.SetParent(self)
		imgSlotRight.LoadImage("d:/ymir work/ui/pattern/gauge_slot_right.tga")
		imgSlotRight.Show()
		imgSlotRight.SetPosition(width - self.SLOT_WIDTH, 0)

		imgSlotCenter = ExpandedImageBox()
		imgSlotCenter.SetParent(self)
		imgSlotCenter.LoadImage("d:/ymir work/ui/pattern/gauge_slot_center.tga")
		imgSlotCenter.Show()
		imgSlotCenter.SetRenderingRect(0.0, 0.0, float((width - self.SLOT_WIDTH*2) - self.SLOT_WIDTH) / self.SLOT_WIDTH, 0.0)
		imgSlotCenter.SetPosition(self.SLOT_WIDTH, 0)

		imgGaugeBack = ExpandedImageBox()
		imgGaugeBack.SetParent(self)
		imgGaugeBack.LoadImage("d:/ymir work/ui/pattern/gauge_blue.tga")
		imgGaugeBack.Hide()
		imgGaugeBack.SetRenderingRect(0.0, 0.0, 0.0, 0.0)
		imgGaugeBack.SetPosition(self.GAUGE_TEMPORARY_PLACE, 0)

		imgGauge = ExpandedImageBox()
		imgGauge.SetParent(self)
		imgGauge.LoadImage("d:/ymir work/ui/pattern/gauge_" + color + ".tga")
		imgGauge.Show()
		imgGauge.SetRenderingRect(0.0, 0.0, 0.0, 0.0)
		imgGauge.SetPosition(self.GAUGE_TEMPORARY_PLACE, 0)

		imgSlotLeft.AddFlag("attach")
		imgSlotCenter.AddFlag("attach")
		imgSlotRight.AddFlag("attach")

		self.imgLeft = imgSlotLeft
		self.imgCenter = imgSlotCenter
		self.imgRight = imgSlotRight
		self.imgGauge = imgGauge
		self.imgGaugeBack = imgGaugeBack
		self.curValue2 = 100
		self.maxValue2 = 100
		self.curValue = 100
		self.maxValue = 100
		self.currentGaugeColor = color

		self.SetSize(width, self.SLOT_HEIGHT)

	def SetColor(self, color):
		if (self.currentGaugeColor == color):
			return
			
		self.currentGaugeColor = color
		self.imgGauge.LoadImage("d:/ymir work/ui/pattern/gauge_" + color + ".tga")
		self.SetPercentage(self.curValue, self.maxValue)

	def SetPercentage(self, curValue, maxValue):
		self.curValue2 = curValue
		self.maxValue2 = maxValue

		if maxValue > 0.0:
			percentage = min(1.0, float(curValue)/float(maxValue))
		else:
			percentage = 0.0
			
		self.lastCurValue = curValue
		self.lastMaxValue = maxValue

		gaugeSize = -1.0 + float(self.width - self.GAUGE_TEMPORARY_PLACE*2) * percentage / self.GAUGE_WIDTH
		self.imgGauge.SetRenderingRect(0.0, 0.0, gaugeSize, 0.0)
		
	def SetPercentageBack(self, curValue, maxValue):
		if not self.imgGaugeBack.IsShow():
			self.imgGaugeBack.Show()
			

		if maxValue > 0.0:
			percentage = min(1.0, float(curValue)/float(maxValue))
		else:
			percentage = 0.0


		gaugeSize = -1.0 + float(self.width - self.GAUGE_TEMPORARY_PLACE*2) * percentage / self.GAUGE_WIDTH
		self.imgGaugeBack.SetRenderingRect(0.0, 0.0, gaugeSize, 0.0)	

	def GetPercentage(self):
		return (self.curValue2, self.maxValue2)

	def SetShowToolTipEvent(self, func, *args):

		self.showtooltipevent = func
		self.showtooltiparg = args

	def SetHideToolTipEvent(self, func, *args):
		self.hidetooltipevent = func
		self.hidetooltiparg = args

	def ShowToolTip(self):
		if self.ToolTipText:
			self.ToolTipText.Show()

	def HideToolTip(self):
		if self.ToolTipText:
			self.ToolTipText.Hide()

	def SetToolTipText(self, text, x=0, y = -19):
		self.SetFormToolTipText("TEXT", text, x, y)

	def SetFormToolTipText(self, type, text, x, y):
		if not self.ToolTipText:       
			toolTip=createToolTipWindowDict[type]()
			toolTip.SetParent(self)
			toolTip.SetSize(0, 0)
			toolTip.SetHorizontalAlignCenter()
			toolTip.SetOutline()
			toolTip.Hide()
			toolTip.SetPosition(x + self.GetWidth()/2, y)
			self.ToolTipText=toolTip
		self.ToolTipText.SetText(text)

class BattlePassGauge(Window):

	SLOT_WIDTH = 16
	SLOT_HEIGHT = 7

	GAUGE_TEMPORARY_PLACE = 12
	GAUGE_WIDTH = 16

	def __init__(self):
		Window.__init__(self)
		self.width = 0
		self.showtooltipevent = None
		self.showtooltiparg = None
		self.hidetooltipevent = None
		self.hidetooltiparg = None
		self.ToolTipText = None

	def __del__(self):
		Window.__del__(self)
		self.showtooltipevent = None
		self.showtooltiparg = None
		self.hidetooltipevent = None
		self.hidetooltiparg = None

	def MakeGauge(self, width, color):

		self.width = max(48, width)

		imgSlotLeft = ImageBox()
		imgSlotLeft.SetParent(self)
		imgSlotLeft.LoadImage("d:/ymir work/battle_pass/gauge/gauge_slot_left.tga")
		imgSlotLeft.Show()

		imgSlotRight = ImageBox()
		imgSlotRight.SetParent(self)
		imgSlotRight.LoadImage("d:/ymir work/battle_pass/gauge/gauge_slot_right.tga")
		imgSlotRight.Show()
		imgSlotRight.SetPosition(width - self.SLOT_WIDTH, 0)

		imgSlotCenter = ExpandedImageBox()
		imgSlotCenter.SetParent(self)
		imgSlotCenter.LoadImage("d:/ymir work/battle_pass/gauge/gauge_slot_center.tga")
		imgSlotCenter.Show()
		imgSlotCenter.SetRenderingRect(0.0, 0.0, float((width - self.SLOT_WIDTH*2) - self.SLOT_WIDTH) / self.SLOT_WIDTH, 0.0)
		imgSlotCenter.SetPosition(self.SLOT_WIDTH, 0)

		imgGaugeBack = ExpandedImageBox()
		imgGaugeBack.SetParent(self)
		imgGaugeBack.LoadImage("d:/ymir work/battle_pass/gauge/gauge_" + color + ".tga")
		imgGaugeBack.Hide()
		imgGaugeBack.SetRenderingRect(0.0, 0.0, 0.0, 0.0)
		imgGaugeBack.SetPosition(self.GAUGE_TEMPORARY_PLACE, 0)

		imgGauge = ExpandedImageBox()
		imgGauge.SetParent(self)
		imgGauge.LoadImage("d:/ymir work/battle_pass/gauge/gauge_" + color + ".tga")
		imgGauge.Show()
		imgGauge.SetRenderingRect(0.0, 0.0, 0.0, 0.0)
		imgGauge.SetPosition(self.GAUGE_TEMPORARY_PLACE, 0)

		imgSlotLeft.AddFlag("attach")
		imgSlotCenter.AddFlag("attach")
		imgSlotRight.AddFlag("attach")

		self.imgLeft = imgSlotLeft
		self.imgCenter = imgSlotCenter
		self.imgRight = imgSlotRight
		self.imgGauge = imgGauge
		self.imgGaugeBack = imgGaugeBack
		self.curValue = 100
		self.maxValue = 100
		self.currentGaugeColor = color

		self.SetSize(width, self.SLOT_HEIGHT)
		
	def SetColor(self, color):
		if (self.currentGaugeColor == color):
			return
			
		self.currentGaugeColor = color
		self.imgGauge.LoadImage("d:/ymir work/battle_pass/gauge/gauge_" + color + ".tga")
		self.SetPercentage(self.curValue, self.maxValue)

	def SetPercentage(self, curValue, maxValue):
		if maxValue > 0.0:
			percentage = min(1.0, float(curValue)/float(maxValue))
		else:
			percentage = 0.0
			
		self.lastCurValue = curValue
		self.lastMaxValue = maxValue

		gaugeSize = -1.0 + float(self.width - self.GAUGE_TEMPORARY_PLACE*2) * percentage / self.GAUGE_WIDTH
		self.imgGauge.SetRenderingRect(0.0, 0.0, gaugeSize, 0.0)
		
	def SetPercentageBack(self, curValue, maxValue):
		if not self.imgGaugeBack.IsShow():
			self.imgGaugeBack.Show()
			

		if maxValue > 0.0:
			percentage = min(1.0, float(curValue)/float(maxValue))
		else:
			percentage = 0.0


		gaugeSize = -1.0 + float(self.width - self.GAUGE_TEMPORARY_PLACE*2) * percentage / self.GAUGE_WIDTH
		self.imgGaugeBack.SetRenderingRect(0.0, 0.0, gaugeSize, 0.0)	

	def SetShowToolTipEvent(self, func, *args):

		self.showtooltipevent = func
		self.showtooltiparg = args

	def SetHideToolTipEvent(self, func, *args):
		self.hidetooltipevent = func
		self.hidetooltiparg = args

	def ShowToolTip(self):
		if self.ToolTipText:
			self.ToolTipText.Show()

	def HideToolTip(self):
		if self.ToolTipText:
			self.ToolTipText.Hide()

	def SetToolTipText(self, text, x=0, y = -19):
		self.SetFormToolTipText("TEXT", text, x, y)

	def SetFormToolTipText(self, type, text, x, y):
		if not self.ToolTipText:       
			toolTip=createToolTipWindowDict[type]()
			toolTip.SetParent(self)
			toolTip.SetSize(0, 0)
			toolTip.SetHorizontalAlignCenter()
			toolTip.SetOutline()
			toolTip.Hide()
			toolTip.SetPosition(x + self.GetWidth()/2, y)
			self.ToolTipText=toolTip
		self.ToolTipText.SetText(text)

class Board(Window):
	CORNER_WIDTH = 32
	CORNER_HEIGHT = 32
	LINE_WIDTH = 128
	LINE_HEIGHT = 128

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3
	
	BASE_PATH = "d:/ymir work/ui/pattern"
	IMAGES = {
		'CORNER' : {
			0 : "Board_Corner_LeftTop",
			1 : "Board_Corner_LeftBottom",
			2 : "Board_Corner_RightTop",
			3 : "Board_Corner_RightBottom"
		},
		'BAR' : {
			0 : "Board_Line_Left",
			1 : "Board_Line_Right",
			2 : "Board_Line_Top",
			3 : "Board_Line_Bottom"
		},
		'FILL' : "Board_Base"
	}

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		self.MakeBoard()
		
	def MakeBoard(self):
		CornerFileNames = [ ]
		LineFileNames = [ ]
		
		for imageDictKey in (['CORNER', 'BAR']):
			for x in xrange(len(self.IMAGES[imageDictKey])):
				if imageDictKey == "CORNER":
					CornerFileNames.append("%s/%s.tga" % (self.BASE_PATH, self.IMAGES[imageDictKey][x]))
				elif imageDictKey == "BAR":
					LineFileNames.append("%s/%s.tga" % (self.BASE_PATH, self.IMAGES[imageDictKey][x]))
		
		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)

		self.Base = ExpandedImageBox()
		self.Base.AddFlag("not_pick")
		self.Base.LoadImage("%s/%s.tga" % (self.BASE_PATH, self.IMAGES['FILL']))
		self.Base.SetParent(self)
		self.Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Base.Show()

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):
		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LB].SetPosition(0, height - self.CORNER_HEIGHT)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH, height - self.CORNER_HEIGHT)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)

		if self.Base:
			self.Base.SetRenderingRect(0, 0, horizontalShowingPercentage, verticalShowingPercentage)

class BoardWithTitleBar(Board):
	def __init__(self):
		Board.__init__(self)

		titleBar = TitleBar()
		titleBar.SetParent(self)
		titleBar.MakeTitleBar(0, "red")
		titleBar.SetPosition(8, 7)
		titleBar.Show()

		titleName = TextLine()
		titleName.SetParent(titleBar)
		titleName.SetPosition(0, 4)
		titleName.SetWindowHorizontalAlignCenter()
		titleName.SetHorizontalAlignCenter()
		titleName.Show()

		self.titleBar = titleBar
		self.titleName = titleName

		self.SetCloseEvent(self.Hide)

	def __del__(self):
		Board.__del__(self)
		self.titleBar = None
		self.titleName = None

	def SetSize(self, width, height):
		self.titleBar.SetWidth(width - 15)
		#self.pickRestrictWindow.SetSize(width, height - 30)
		Board.SetSize(self, width, height)
		self.titleName.UpdateRect()

	def SetTitleColor(self, color):
		self.titleName.SetPackedFontColor(color)

	def SetTitleName(self, name):
		self.titleName.SetText(name)

	def SetCloseEvent(self, event):
		self.titleBar.SetCloseEvent(event)

class MiddleBoard(Window):
	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.51)

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "d:/ymir work/ui/pattern/ThinBoard_Corner_"+dir+".tga" for dir in ["LeftTop","LeftBottom","RightTop","RightBottom"] ]
		LineFileNames = [ "d:/ymir work/ui/pattern/ThinBoard_Line_"+dir+".tga" for dir in ["Left","Right","Top","Bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		Base = Bar()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.SetColor(self.BOARD_COLOR)
		Base.Show()
		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):

		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LB].SetPosition(0, height - self.CORNER_HEIGHT)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH, height - self.CORNER_HEIGHT)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Base.SetSize(width - self.CORNER_WIDTH*2, height - self.CORNER_HEIGHT*2)

	def ShowInternal(self):
		self.Base.Show()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()

class BorderA(Board):
	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	
	BASE_PATH = "d:/ymir work/ui/pattern"
	IMAGES = {
		'CORNER' : {
			0 : "border_a_left_top",
			1 : "border_a_left_bottom",
			2 : "border_a_right_top",
			3 : "border_a_right_bottom"
		},
		'BAR' : {
			0 : "border_a_left",
			1 : "border_a_right",
			2 : "border_a_top",
			3 : "border_a_bottom"
		},
		'FILL' : "border_a_center"
	}
	
	def __init__(self, layer = "UI"):
		Board.__init__(self, layer)

	def __del__(self):
		Board.__del__(self)

	def SetSize(self, width, height):
		Board.SetSize(self, width, height)

class BorderB(Board):
	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	
	BASE_PATH = "d:/ymir work/ui/pattern"
	IMAGES = {
		'CORNER' : {
			0 : "border_b_left_top",
			1 : "border_b_left_bottom",
			2 : "border_b_right_top",
			3 : "border_b_right_bottom"
		},
		'BAR' : {
			0 : "border_b_left",
			1 : "border_b_right",
			2 : "border_b_top",
			3 : "border_b_bottom"
		},
		'FILL' : "border_b_center"
	}
	
	def __init__(self):
		Board.__init__(self)

	def __del__(self):
		Board.__del__(self)

	def SetSize(self, width, height):
		Board.SetSize(self, width, height)

class ThinBoard(Window):

	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.51)

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "d:/ymir work/ui/pattern/ThinBoard_Corner_"+dir+".tga" for dir in ["LeftTop","LeftBottom","RightTop","RightBottom"] ]
		LineFileNames = [ "d:/ymir work/ui/pattern/ThinBoard_Line_"+dir+".tga" for dir in ["Left","Right","Top","Bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		Base = Bar()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.SetColor(self.BOARD_COLOR)
		Base.Show()
		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)

	if app.ENABLE_SEND_TARGET_INFO:
		def ShowCorner(self, corner):
			self.Corners[corner].Show()
			self.SetSize(self.GetWidth(), self.GetHeight())

		def HideCorners(self, corner):
			self.Corners[corner].Hide()
			self.SetSize(self.GetWidth(), self.GetHeight())

		def ShowLine(self, line):
			self.Lines[line].Show()
			self.SetSize(self.GetWidth(), self.GetHeight())

		def HideLine(self, line):
			self.Lines[line].Hide()
			self.SetSize(self.GetWidth(), self.GetHeight())

	def SetSize(self, width, height):

		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LB].SetPosition(0, height - self.CORNER_HEIGHT)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH, height - self.CORNER_HEIGHT)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Base.SetSize(width - self.CORNER_WIDTH*2, height - self.CORNER_HEIGHT*2)

	def ShowInternal(self):
		self.Base.Show()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()
			
class ThinBoard2(Window):

	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.51)

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = ["d:/ymir work/ui/pattern/ThinBoard_Corner_" + dir + "_new.tga" for dir in ["LeftTop", "LeftBottom", "RightTop", "RightBottom"]]
		LineFileNames = ["d:/ymir work/ui/pattern/ThinBoard_Line_" + dir + "_new.tga" for dir in ["Left", "Right", "Top", "Bottom"]]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()

			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()

			self.Lines.append(Line)

		Base = Bar()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.SetColor(self.BOARD_COLOR)
		Base.Show()

		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)

	def SetPattern(self, fileName):
		CornerFileNames = ["d:/ymir work/ui/pattern/ThinBoard_Corner_" + dir + "_" + fileName + ".tga" for dir in ["LeftTop", "LeftBottom", "RightTop", "RightBottom"]]
		LineFileNames = ["d:/ymir work/ui/pattern/ThinBoard_Line_" + dir + "_" + fileName + ".tga" for dir in ["Left", "Right", "Top", "Bottom"]]
		
		for i in xrange(len(self.Corners)):
			self.Corners[i].LoadImage(CornerFileNames[i])
		
		for i in xrange(len(self.Lines)):
			self.Lines[i].LoadImage(LineFileNames[i])

	def SetSize(self, width, height):
		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LB].SetPosition(0, height - self.CORNER_HEIGHT)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH, height - self.CORNER_HEIGHT)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2) - self.LINE_WIDTH) / self.LINE_WIDTH

		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Base.SetSize(width - self.CORNER_WIDTH*2, height - self.CORNER_HEIGHT*2)

	def ShowInternal(self):
		self.Base.Show()

		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()

		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()		

class ThinBoardDungeon(Window):
	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.51)

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)
		CornerFileNames = [ "d:/ymir work/ui/pattern/thinboarddungeon/ThinBoard_Corner_"+dir+".tga" for dir in ["LeftTop_gold", "LeftBottom_gold","RightTop_gold", "RightBottom_gold"]]
		LineFileNames = [ "d:/ymir work/ui/pattern/thinboarddungeon/ThinBoard_Line_"+dir+".tga" for dir in ["Left_gold", "Right_gold", "Top_gold", "Bottom_gold"]]
		
		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		Base = ExpandedImageBox()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.LoadImage("d:/ymir work/ui/pattern/thinboarddungeon/thinboard_bg_gold.tga")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.Show()
		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):

		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LB].SetPosition(0, height - self.CORNER_HEIGHT)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH, height - self.CORNER_HEIGHT)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		if self.Base:
			self.Base.SetRenderingRect(0, 0, horizontalShowingPercentage, verticalShowingPercentage)

	def ShowInternal(self):
		self.Base.Show()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()

class ThinBoardGold(Window):
	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.51)

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)
		CornerFileNames = [ "d:/ymir work/ui/pattern/thinboardgold/ThinBoard_Corner_"+dir+".tga" for dir in ["LeftTop_gold", "LeftBottom_gold","RightTop_gold", "RightBottom_gold"]]
		LineFileNames = [ "d:/ymir work/ui/pattern/thinboardgold/ThinBoard_Line_"+dir+".tga" for dir in ["Left_gold", "Right_gold", "Top_gold", "Bottom_gold"]]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		Base = ExpandedImageBox()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.LoadImage("d:/ymir work/ui/pattern/thinboardgold/thinboard_bg_gold.tga")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.Show()
		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):

		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LB].SetPosition(0, height - self.CORNER_HEIGHT)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH, height - self.CORNER_HEIGHT)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		if self.Base:
			self.Base.SetRenderingRect(0, 0, horizontalShowingPercentage, verticalShowingPercentage)

	def ShowInternal(self):
		self.Base.Show()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()

class ThinBoardCircle(Window):
	CORNER_WIDTH = 4
	CORNER_HEIGHT = 4
	LINE_WIDTH = 4
	LINE_HEIGHT = 4
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 1.0)

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "d:/ymir work/ui/pattern/thinboardcircle/ThinBoard_Corner_"+dir+"_Circle.tga" for dir in ["LeftTop","LeftBottom","RightTop","RightBottom"] ]
		LineFileNames = [ "d:/ymir work/ui/pattern/thinboardcircle/ThinBoard_Line_"+dir+"_Circle.tga" for dir in ["Left","Right","Top","Bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		Base = Bar()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.SetColor(self.BOARD_COLOR)
		Base.Show()
		self.Base = Base
		
		self.ButtonText = None
		self.BonusId = 0

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):

		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LB].SetPosition(0, height - self.CORNER_HEIGHT)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH, height - self.CORNER_HEIGHT)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Base.SetSize(width - self.CORNER_WIDTH*2, height - self.CORNER_HEIGHT*2)
		
	def SetText(self, text):
		if not self.ButtonText:
			textLine = TextLine()
			textLine.SetParent(self)
			textLine.SetPosition(self.GetWidth()/2, self.GetHeight()/2)
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.Show()
			self.ButtonText = textLine

		self.ButtonText.SetText(text)
		
	def GetText(self):
		if not self.ButtonText:
			return ""
		return self.ButtonText.GetText()
		
	def SetBonusId(self, bnsId):
		self.BonusId = bnsId
		
	def GetBonusId(self):
		if self.BonusId != 0:
			return self.BonusId

	def ShowInternal(self):
		self.Base.Show()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()

class ThinBoard_yeni(Window):

	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.51)

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "d:/ymir work/yeniboard/border_b_"+dir+".tga" for dir in ["LeftTop","LeftBottom","RightTop","RightBottom"] ]
		LineFileNames = [ "d:/ymir work/yeniboard/border_a_"+dir+".tga" for dir in ["Left","Right","Top","Bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)
			
		self.orta = []	
		orta = ExpandedImageBox()
		orta.AddFlag("attach")
		orta.AddFlag("not_pick")
		orta.LoadImage("d:/ymir work/yeniboard/border_a_center.tga")
		orta.SetParent(self)
		orta.SetPosition(0, 0)
		orta.Show()
		self.orta.append(orta)

		Base = Bar()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.SetColor(self.BOARD_COLOR)
		Base.Hide()
		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)
		self.orta[0].SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):

		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LB].SetPosition(0, height - self.CORNER_HEIGHT)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH, height - self.CORNER_HEIGHT)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		
		self.orta[0].SetRenderingRect(0, 0, horizontalShowingPercentage, verticalShowingPercentage)
		
		self.Base.SetSize(width - self.CORNER_WIDTH*2, height - self.CORNER_HEIGHT*2)

	def ShowInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()

class ThinBoardNorm(Window):

	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.51)

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "d:/ymir work/ui/pattern/ThinBoard_Corner_"+dir+".tga" for dir in ["LeftTop","LeftBottom","RightTop","RightBottom"] ]
		LineFileNames = [ "d:/ymir work/ui/pattern/ThinBoard_Line_"+dir+".tga" for dir in ["Left","Right","Top","Bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		Base = Bar()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.SetColor(self.BOARD_COLOR)
		Base.Show()
		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height=20):

		width = max(self.CORNER_WIDTH*2, width-50)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LB].SetPosition(0, height - self.CORNER_HEIGHT)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH, height - self.CORNER_HEIGHT)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Base.SetSize(width - self.CORNER_WIDTH*2, height - self.CORNER_HEIGHT*2)

	def ShowInternal(self):
		self.Base.Show()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()

class ThinBoardFire(Window):

	CORNER_1_WIDTH = 48
	CORNER_1_HEIGHT = 48
	LINE_WIDTH = 32
	LINE_HEIGHT = 32

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "D:/ymir work/ui/pattern/myshop/fire/p_fire_"+dir+".tga" for dir in ["left_top","left_bottom","right_top","right_bottom"] ]
		LineFileNames = [ "D:/ymir work/ui/pattern/myshop/fire/p_fire_"+dir+".tga" for dir in ["left","right","top","bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		self.Lines[self.T].SetPosition(self.CORNER_1_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width):
		width = max(100, width)
		Window.SetSize(self, width, self.CORNER_1_HEIGHT)

		horizontalShowingPercentage = float((width - self.CORNER_1_WIDTH*2-4) - 32) / 16

		self.Corners[self.LB].SetPosition(0, self.CORNER_1_HEIGHT-16)
		self.Corners[self.RT].SetPosition(width - self.CORNER_1_WIDTH-20, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_1_WIDTH-20,self.CORNER_1_HEIGHT-16)
		self.Lines[self.B].SetPosition(self.CORNER_1_HEIGHT, self.CORNER_1_HEIGHT-16)
		self.Lines[self.L].Hide()
		self.Lines[self.R].Hide()

		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)

class ThinBoardIce(Window):

	CORNER_1_WIDTH = 48
	CORNER_1_HEIGHT = 48
	LINE_WIDTH = 32
	LINE_HEIGHT = 32

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "D:/ymir work/ui/pattern/myshop/ice/p_ice_"+dir+".tga" for dir in ["left_top","left_bottom","right_top","right_bottom"] ]
		LineFileNames = [ "D:/ymir work/ui/pattern/myshop/ice/p_ice_"+dir+".tga" for dir in ["left","right","top","bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		self.Lines[self.T].SetPosition(self.CORNER_1_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width):
		width = max(100, width)
		Window.SetSize(self, width, self.CORNER_1_HEIGHT)

		horizontalShowingPercentage = float((width - self.CORNER_1_WIDTH*2-4) - 32) / 16

		self.Corners[self.LB].SetPosition(0, self.CORNER_1_HEIGHT-16)
		self.Corners[self.RT].SetPosition(width - self.CORNER_1_WIDTH-20, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_1_WIDTH-20,self.CORNER_1_HEIGHT-16)
		self.Lines[self.B].SetPosition(self.CORNER_1_HEIGHT, self.CORNER_1_HEIGHT-16)
		self.Lines[self.L].Hide()
		self.Lines[self.R].Hide()

		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)

class ThinBoardPaper(Window):

	CORNER_1_WIDTH = 48
	CORNER_1_HEIGHT = 48
	LINE_WIDTH = 32
	LINE_HEIGHT = 32

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "D:/ymir work/ui/pattern/myshop/paper/p_paper_"+dir+".tga" for dir in ["left_top","left_bottom","right_top","right_bottom"] ]
		LineFileNames = [ "D:/ymir work/ui/pattern/myshop/paper/p_paper_"+dir+".tga" for dir in ["left","right","top","bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		self.Lines[self.T].SetPosition(self.CORNER_1_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width):
		width = max(100, width)
		Window.SetSize(self, width, self.CORNER_1_HEIGHT)

		horizontalShowingPercentage = float((width - self.CORNER_1_WIDTH*2-4) - 32) / 16

		self.Corners[self.LB].SetPosition(0, self.CORNER_1_HEIGHT-16)
		self.Corners[self.RT].SetPosition(width - self.CORNER_1_WIDTH-20, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_1_WIDTH-20,self.CORNER_1_HEIGHT-16)
		self.Lines[self.B].SetPosition(self.CORNER_1_HEIGHT, self.CORNER_1_HEIGHT-16)
		self.Lines[self.L].Hide()
		self.Lines[self.R].Hide()

		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)

class ThinBoardRibon(Window):

	CORNER_1_WIDTH = 48
	CORNER_1_HEIGHT = 48
	LINE_WIDTH = 32
	LINE_HEIGHT = 32

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "D:/ymir work/ui/pattern/myshop/ribon/p_ribon_"+dir+".tga" for dir in ["left_top","left_bottom","right_top","right_bottom"] ]
		LineFileNames = [ "D:/ymir work/ui/pattern/myshop/ribon/p_ribon_"+dir+".tga" for dir in ["left","right","top","bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		self.Lines[self.T].SetPosition(self.CORNER_1_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width):
		width = max(100, width)
		Window.SetSize(self, width, self.CORNER_1_HEIGHT)

		horizontalShowingPercentage = float((width - self.CORNER_1_WIDTH*2-4) - 32) / 16

		self.Corners[self.LB].SetPosition(0, self.CORNER_1_HEIGHT-16)
		self.Corners[self.RT].SetPosition(width - self.CORNER_1_WIDTH-20, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_1_WIDTH-20,self.CORNER_1_HEIGHT-16)
		self.Lines[self.B].SetPosition(self.CORNER_1_HEIGHT, self.CORNER_1_HEIGHT-16)
		self.Lines[self.L].Hide()
		self.Lines[self.R].Hide()

		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)

class ThinBoardWing(Window):

	CORNER_1_WIDTH = 48
	CORNER_1_HEIGHT = 48
	LINE_WIDTH = 32
	LINE_HEIGHT = 32

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "D:/ymir work/ui/pattern/myshop/wing/p_wing_"+dir+".tga" for dir in ["left_top","left_bottom","right_top","right_bottom"] ]
		LineFileNames = [ "D:/ymir work/ui/pattern/myshop/wing/p_wing_"+dir+".tga" for dir in ["left","right","top","bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		self.Lines[self.T].SetPosition(self.CORNER_1_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width):
		width = max(100, width)
		Window.SetSize(self, width, self.CORNER_1_HEIGHT)

		horizontalShowingPercentage = float((width - self.CORNER_1_WIDTH*2-4) - 32) / 16

		self.Corners[self.LB].SetPosition(0, self.CORNER_1_HEIGHT-16)
		self.Corners[self.RT].SetPosition(width - self.CORNER_1_WIDTH-20, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_1_WIDTH-20,self.CORNER_1_HEIGHT-16)
		self.Lines[self.B].SetPosition(self.CORNER_1_HEIGHT, self.CORNER_1_HEIGHT-16)
		self.Lines[self.L].Hide()
		self.Lines[self.R].Hide()

		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)

class ThinBoard_KasmirPaket_Normal(Window):
	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.51)
	BOARD_NO = 5

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "d:/ymir work/ui/pattern/ThinBoard_Corner_"+dir+".tga" for dir in ["LeftTop","LeftBottom","RightTop","RightBottom"] ]
		LineFileNames = [ "d:/ymir work/ui/pattern/ThinBoard_Line_"+dir+".tga" for dir in ["Left","Right","Top","Bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		Base = Bar()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.SetColor(self.BOARD_COLOR)
		Base.Show()
		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, 0)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):
		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LB].SetPosition(0, height - self.CORNER_HEIGHT)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH, 0)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH, height - self.CORNER_HEIGHT)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0, 0, horizontalShowingPercentage, 0)
		self.Base.SetSize(width - self.CORNER_WIDTH*2, height - self.CORNER_HEIGHT*2)

	def ShowInternal(self):
		self.Base.Show()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()

class ThinBoard_KasmirPaket_Fire(Window):
	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	CORNER_WIDTH_KasmirPaket = -43
	CORNER_HEIGHT_KasmirPaket = -15
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.51)
	BOARD_NO = 5

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "d:/ymir work/ui/pattern/myshop/fire/ThinBoard_Corner_"+dir+".tga" for dir in ["LeftTop","LeftBottom","RightTop","RightBottom"] ]
		LineFileNames = [ "d:/ymir work/ui/pattern/myshop/fire/ThinBoard_Line_"+dir+".tga" for dir in ["Left","Right","Top","Bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		Base = Bar()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.SetColor(self.BOARD_COLOR)
		Base.Show()
		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT_KasmirPaket)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):
		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LT].SetPosition(self.CORNER_WIDTH_KasmirPaket, self.CORNER_HEIGHT_KasmirPaket)
		self.Corners[self.LB].SetPosition(self.CORNER_WIDTH_KasmirPaket, height - self.CORNER_HEIGHT + 1)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH + 10, self.CORNER_HEIGHT_KasmirPaket)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH + 10, height - self.CORNER_HEIGHT + 1)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT + 1)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2+10) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0.7, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0.7, 0, horizontalShowingPercentage, 0)
		self.Base.SetSize(width - self.CORNER_WIDTH*2, height - self.CORNER_HEIGHT*2)

	def ShowInternal(self):
		self.Base.Show()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()

class ThinBoard_KasmirPaket_Ice(Window):
	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	CORNER_WIDTH_KasmirPaket = -43
	CORNER_HEIGHT_KasmirPaket = -15
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.51)
	BOARD_NO = 5

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "d:/ymir work/ui/pattern/myshop/ice/ThinBoard_Corner_"+dir+".tga" for dir in ["LeftTop","LeftBottom","RightTop","RightBottom"] ]
		LineFileNames = [ "d:/ymir work/ui/pattern/myshop/ice/ThinBoard_Line_"+dir+".tga" for dir in ["Left","Right","Top","Bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		Base = Bar()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.SetColor(self.BOARD_COLOR)
		Base.Show()
		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT_KasmirPaket)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):
		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LT].SetPosition(self.CORNER_WIDTH_KasmirPaket, self.CORNER_HEIGHT_KasmirPaket)
		self.Corners[self.LB].SetPosition(self.CORNER_WIDTH_KasmirPaket, height - self.CORNER_HEIGHT + 1)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH + 10, self.CORNER_HEIGHT_KasmirPaket)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH + 10, height - self.CORNER_HEIGHT + 1)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT + 1)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2+10) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0.7, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0.7, 0, horizontalShowingPercentage, 0)
		self.Base.SetSize(width - self.CORNER_WIDTH*2, height - self.CORNER_HEIGHT*2)

	def ShowInternal(self):
		self.Base.Show()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()

class ThinBoard_KasmirPaket_Paper(Window):
	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	CORNER_WIDTH_KasmirPaket = -43
	CORNER_HEIGHT_KasmirPaket = -15
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.51)
	BOARD_NO = 5

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "d:/ymir work/ui/pattern/myshop/paper/ThinBoard_Corner_"+dir+".tga" for dir in ["LeftTop","LeftBottom","RightTop","RightBottom"] ]
		LineFileNames = [ "d:/ymir work/ui/pattern/myshop/paper/ThinBoard_Line_"+dir+".tga" for dir in ["Left","Right","Top","Bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		Base = Bar()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.SetColor(self.BOARD_COLOR)
		Base.Show()
		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT_KasmirPaket)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):
		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LT].SetPosition(self.CORNER_WIDTH_KasmirPaket, self.CORNER_HEIGHT_KasmirPaket)
		self.Corners[self.LB].SetPosition(self.CORNER_WIDTH_KasmirPaket, height - self.CORNER_HEIGHT + 1)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH + 10, self.CORNER_HEIGHT_KasmirPaket)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH + 10, height - self.CORNER_HEIGHT + 1)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT + 1)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2+10) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0.7, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0.7, 0, horizontalShowingPercentage, 0)
		self.Base.SetSize(width - self.CORNER_WIDTH*2, height - self.CORNER_HEIGHT*2)

	def ShowInternal(self):
		self.Base.Show()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()

class ThinBoard_KasmirPaket_Ribon(Window):
	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	CORNER_WIDTH_KasmirPaket = -43
	CORNER_HEIGHT_KasmirPaket = -15
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.51)
	BOARD_NO = 5

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "d:/ymir work/ui/pattern/myshop/ribon/ThinBoard_Corner_"+dir+".tga" for dir in ["LeftTop","LeftBottom","RightTop","RightBottom"] ]
		LineFileNames = [ "d:/ymir work/ui/pattern/myshop/ribon/ThinBoard_Line_"+dir+".tga" for dir in ["Left","Right","Top","Bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		Base = Bar()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.SetColor(self.BOARD_COLOR)
		Base.Show()
		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT_KasmirPaket)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):
		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LT].SetPosition(self.CORNER_WIDTH_KasmirPaket, self.CORNER_HEIGHT_KasmirPaket)
		self.Corners[self.LB].SetPosition(self.CORNER_WIDTH_KasmirPaket, height - self.CORNER_HEIGHT + 1)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH + 10, self.CORNER_HEIGHT_KasmirPaket)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH + 10, height - self.CORNER_HEIGHT + 1)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT + 1)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2+10) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0.7, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0.7, 0, horizontalShowingPercentage, 0)
		self.Base.SetSize(width - self.CORNER_WIDTH*2, height - self.CORNER_HEIGHT*2)

	def ShowInternal(self):
		self.Base.Show()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()

class ThinBoard_KasmirPaket_Wing(Window):
	CORNER_WIDTH = 16
	CORNER_HEIGHT = 16
	CORNER_WIDTH_KasmirPaket = -43
	CORNER_HEIGHT_KasmirPaket = -15
	LINE_WIDTH = 16
	LINE_HEIGHT = 16
	BOARD_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.51)
	BOARD_NO = 5

	LT = 0
	LB = 1
	RT = 2
	RB = 3
	L = 0
	R = 1
	T = 2
	B = 3
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)

		CornerFileNames = [ "d:/ymir work/ui/pattern/myshop/wing/ThinBoard_Corner_"+dir+".tga" for dir in ["LeftTop","LeftBottom","RightTop","RightBottom"] ]
		LineFileNames = [ "d:/ymir work/ui/pattern/myshop/wing/ThinBoard_Line_"+dir+".tga" for dir in ["Left","Right","Top","Bottom"] ]

		self.Corners = []
		for fileName in CornerFileNames:
			Corner = ExpandedImageBox()
			Corner.AddFlag("attach")
			Corner.AddFlag("not_pick")
			Corner.LoadImage(fileName)
			Corner.SetParent(self)
			Corner.SetPosition(0, 0)
			Corner.Show()
			self.Corners.append(Corner)

		self.Lines = []
		for fileName in LineFileNames:
			Line = ExpandedImageBox()
			Line.AddFlag("attach")
			Line.AddFlag("not_pick")
			Line.LoadImage(fileName)
			Line.SetParent(self)
			Line.SetPosition(0, 0)
			Line.Show()
			self.Lines.append(Line)

		Base = Bar()
		Base.SetParent(self)
		Base.AddFlag("attach")
		Base.AddFlag("not_pick")
		Base.SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT)
		Base.SetColor(self.BOARD_COLOR)
		Base.Show()
		self.Base = Base

		self.Lines[self.L].SetPosition(0, self.CORNER_HEIGHT)
		self.Lines[self.T].SetPosition(self.CORNER_WIDTH, self.CORNER_HEIGHT_KasmirPaket)

	def __del__(self):
		Window.__del__(self)

	def SetSize(self, width, height):
		width = max(self.CORNER_WIDTH*2, width)
		height = max(self.CORNER_HEIGHT*2, height)
		Window.SetSize(self, width, height)

		self.Corners[self.LT].SetPosition(self.CORNER_WIDTH_KasmirPaket, self.CORNER_HEIGHT_KasmirPaket)
		self.Corners[self.LB].SetPosition(self.CORNER_WIDTH_KasmirPaket, height - self.CORNER_HEIGHT + 1)
		self.Corners[self.RT].SetPosition(width - self.CORNER_WIDTH + 10, self.CORNER_HEIGHT_KasmirPaket)
		self.Corners[self.RB].SetPosition(width - self.CORNER_WIDTH + 10, height - self.CORNER_HEIGHT + 1)
		self.Lines[self.R].SetPosition(width - self.CORNER_WIDTH, self.CORNER_HEIGHT)
		self.Lines[self.B].SetPosition(self.CORNER_HEIGHT, height - self.CORNER_HEIGHT + 1)

		verticalShowingPercentage = float((height - self.CORNER_HEIGHT*2) - self.LINE_HEIGHT) / self.LINE_HEIGHT
		horizontalShowingPercentage = float((width - self.CORNER_WIDTH*2+10) - self.LINE_WIDTH) / self.LINE_WIDTH
		self.Lines[self.L].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.R].SetRenderingRect(0, 0, 0, verticalShowingPercentage)
		self.Lines[self.T].SetRenderingRect(0.7, 0, horizontalShowingPercentage, 0)
		self.Lines[self.B].SetRenderingRect(0.7, 0, horizontalShowingPercentage, 0)
		self.Base.SetSize(width - self.CORNER_WIDTH*2, height - self.CORNER_HEIGHT*2)

	def ShowInternal(self):
		self.Base.Show()
		for wnd in self.Lines:
			wnd.Show()
		for wnd in self.Corners:
			wnd.Show()

	def HideInternal(self):
		self.Base.Hide()
		for wnd in self.Lines:
			wnd.Hide()
		for wnd in self.Corners:
			wnd.Hide()

class ScrollBar(Window):

	SCROLLBAR_WIDTH = 17
	SCROLLBAR_MIDDLE_HEIGHT = 9
	SCROLLBAR_BUTTON_WIDTH = 17
	SCROLLBAR_BUTTON_HEIGHT = 17
	MIDDLE_BAR_POS = 5
	MIDDLE_BAR_UPPER_PLACE = 3
	MIDDLE_BAR_DOWNER_PLACE = 4
	TEMP_SPACE = MIDDLE_BAR_UPPER_PLACE + MIDDLE_BAR_DOWNER_PLACE

	class MiddleBar(DragButton):
		def __init__(self):
			DragButton.__init__(self)
			self.AddFlag("movable")
			#self.AddFlag("restrict_x")

		def MakeImage(self):
			top = ImageBox()
			top.SetParent(self)
			top.LoadImage("d:/ymir work/ui/pattern/ScrollBar_Top.tga")
			top.SetPosition(0, 0)
			top.AddFlag("not_pick")
			top.Show()
			bottom = ImageBox()
			bottom.SetParent(self)
			bottom.LoadImage("d:/ymir work/ui/pattern/ScrollBar_Bottom.tga")
			bottom.AddFlag("not_pick")
			bottom.Show()

			middle = ExpandedImageBox()
			middle.SetParent(self)
			middle.LoadImage("d:/ymir work/ui/pattern/ScrollBar_Middle.tga")
			middle.SetPosition(0, 4)
			middle.AddFlag("not_pick")
			middle.Show()

			self.top = top
			self.bottom = bottom
			self.middle = middle

		def SetSize(self, height):
			height = max(12, height)
			DragButton.SetSize(self, 10, height)
			self.bottom.SetPosition(0, height-4)

			height -= 4*3
			self.middle.SetRenderingRect(0, 0, 0, float(height)/4.0)

	def __init__(self):
		Window.__init__(self)

		self.pageSize = 1
		self.curPos = 0.0
		self.eventScroll = lambda *arg: None
		self.lockFlag = FALSE
		self.scrollStep = 0.20


		self.CreateScrollBar()

	def __del__(self):
		Window.__del__(self)

	def CreateScrollBar(self):
		barSlot = Bar3D()
		barSlot.SetParent(self)
		barSlot.AddFlag("not_pick")
		barSlot.Show()

		middleBar = self.MiddleBar()
		middleBar.SetParent(self)
		middleBar.SetMoveEvent(__mem_func__(self.OnMove))
		middleBar.Show()
		middleBar.MakeImage()
		middleBar.SetSize(12)

		upButton = Button()
		upButton.SetParent(self)
		upButton.SetEvent(__mem_func__(self.OnUp))
		upButton.SetUpVisual("d:/ymir work/ui/public/scrollbar_up_button_01.sub")
		upButton.SetOverVisual("d:/ymir work/ui/public/scrollbar_up_button_02.sub")
		upButton.SetDownVisual("d:/ymir work/ui/public/scrollbar_up_button_03.sub")
		upButton.Show()

		downButton = Button()
		downButton.SetParent(self)
		downButton.SetEvent(__mem_func__(self.OnDown))
		downButton.SetUpVisual("d:/ymir work/ui/public/scrollbar_down_button_01.sub")
		downButton.SetOverVisual("d:/ymir work/ui/public/scrollbar_down_button_02.sub")
		downButton.SetDownVisual("d:/ymir work/ui/public/scrollbar_down_button_03.sub")
		downButton.Show()

		self.upButton = upButton
		self.downButton = downButton
		self.middleBar = middleBar
		self.barSlot = barSlot

		self.SCROLLBAR_WIDTH = self.upButton.GetWidth()
		self.SCROLLBAR_MIDDLE_HEIGHT = self.middleBar.GetHeight()
		self.SCROLLBAR_BUTTON_WIDTH = self.upButton.GetWidth()
		self.SCROLLBAR_BUTTON_HEIGHT = self.upButton.GetHeight()

	def Destroy(self):
		self.middleBar = None
		self.upButton = None
		self.downButton = None
		self.eventScroll = lambda *arg: None

	def SetScrollEvent(self, event):
		self.eventScroll = event

	def SetMiddleBarSize(self, pageScale):
		realHeight = self.GetHeight() - self.SCROLLBAR_BUTTON_HEIGHT*2
		self.SCROLLBAR_MIDDLE_HEIGHT = int(pageScale * float(realHeight))
		self.middleBar.SetSize(self.SCROLLBAR_MIDDLE_HEIGHT)
		self.pageSize = (self.GetHeight() - self.SCROLLBAR_BUTTON_HEIGHT*2) - self.SCROLLBAR_MIDDLE_HEIGHT - (self.TEMP_SPACE)

	def SetScrollBarSize(self, height):
		self.pageSize = (height - self.SCROLLBAR_BUTTON_HEIGHT*2) - self.SCROLLBAR_MIDDLE_HEIGHT - (self.TEMP_SPACE)
		self.SetSize(self.SCROLLBAR_WIDTH, height)
		self.upButton.SetPosition(0, 0)
		self.downButton.SetPosition(0, height - self.SCROLLBAR_BUTTON_HEIGHT)
		self.middleBar.SetRestrictMovementArea(self.MIDDLE_BAR_POS, self.SCROLLBAR_BUTTON_HEIGHT + self.MIDDLE_BAR_UPPER_PLACE, self.MIDDLE_BAR_POS+2, height - self.SCROLLBAR_BUTTON_HEIGHT*2 - self.TEMP_SPACE)
		self.middleBar.SetPosition(self.MIDDLE_BAR_POS, 0)

		self.UpdateBarSlot()

	def UpdateBarSlot(self):
		self.barSlot.SetPosition(0, self.SCROLLBAR_BUTTON_HEIGHT)
		self.barSlot.SetSize(self.GetWidth() - 2, self.GetHeight() - self.SCROLLBAR_BUTTON_HEIGHT*2 - 2)

	def GetPos(self):
		return self.curPos

	def SetPos(self, pos):
		pos = max(0.0, pos)
		pos = min(1.0, pos)

		newPos = float(self.pageSize) * pos
		self.middleBar.SetPosition(self.MIDDLE_BAR_POS, int(newPos) + self.SCROLLBAR_BUTTON_HEIGHT + self.MIDDLE_BAR_UPPER_PLACE)
		self.OnMove()

	def SetScrollStep(self, step):
		self.scrollStep = step
	
	def GetScrollStep(self):
		return self.scrollStep
		
	def OnUp(self):
		self.SetPos(self.curPos-self.scrollStep)

	def OnDown(self):
		self.SetPos(self.curPos+self.scrollStep)

	def OnMove(self):

		if self.lockFlag:
			return

		if 0 == self.pageSize:
			return

		(xLocal, yLocal) = self.middleBar.GetLocalPosition()
		self.curPos = float(yLocal - self.SCROLLBAR_BUTTON_HEIGHT - self.MIDDLE_BAR_UPPER_PLACE) / float(self.pageSize)

		self.eventScroll()

	def OnMouseLeftButtonDown(self):
		(xMouseLocalPosition, yMouseLocalPosition) = self.GetMouseLocalPosition()
		pickedPos = yMouseLocalPosition - self.SCROLLBAR_BUTTON_HEIGHT - self.SCROLLBAR_MIDDLE_HEIGHT/2
		newPos = float(pickedPos) / float(self.pageSize)
		self.SetPos(newPos)

	def LockScroll(self):
		self.lockFlag = TRUE

	def UnlockScroll(self):
		self.lockFlag = FALSE

class ThinScrollBar(ScrollBar):

	def CreateScrollBar(self):
		middleBar = self.MiddleBar()
		middleBar.SetParent(self)
		middleBar.SetMoveEvent(__mem_func__(self.OnMove))
		middleBar.Show()
		middleBar.SetUpVisual("d:/ymir work/ui/public/scrollbar_thin_middle_button_01.sub")
		middleBar.SetOverVisual("d:/ymir work/ui/public/scrollbar_thin_middle_button_02.sub")
		middleBar.SetDownVisual("d:/ymir work/ui/public/scrollbar_thin_middle_button_03.sub")

		upButton = Button()
		upButton.SetParent(self)
		upButton.SetUpVisual("d:/ymir work/ui/public/scrollbar_thin_up_button_01.sub")
		upButton.SetOverVisual("d:/ymir work/ui/public/scrollbar_thin_up_button_02.sub")
		upButton.SetDownVisual("d:/ymir work/ui/public/scrollbar_thin_up_button_03.sub")
		upButton.SetEvent(__mem_func__(self.OnUp))
		upButton.Show()

		downButton = Button()
		downButton.SetParent(self)
		downButton.SetUpVisual("d:/ymir work/ui/public/scrollbar_thin_down_button_01.sub")
		downButton.SetOverVisual("d:/ymir work/ui/public/scrollbar_thin_down_button_02.sub")
		downButton.SetDownVisual("d:/ymir work/ui/public/scrollbar_thin_down_button_03.sub")
		downButton.SetEvent(__mem_func__(self.OnDown))
		downButton.Show()

		self.middleBar = middleBar
		self.upButton = upButton
		self.downButton = downButton

		self.SCROLLBAR_WIDTH = self.upButton.GetWidth()
		self.SCROLLBAR_MIDDLE_HEIGHT = self.middleBar.GetHeight()
		self.SCROLLBAR_BUTTON_WIDTH = self.upButton.GetWidth()
		self.SCROLLBAR_BUTTON_HEIGHT = self.upButton.GetHeight()
		self.MIDDLE_BAR_POS = 0
		self.MIDDLE_BAR_UPPER_PLACE = 0
		self.MIDDLE_BAR_DOWNER_PLACE = 0
		self.TEMP_SPACE = 0

	def UpdateBarSlot(self):
		pass

class SmallThinScrollBar(ScrollBar):

	def CreateScrollBar(self):
		middleBar = self.MiddleBar()
		middleBar.SetParent(self)
		middleBar.SetMoveEvent(__mem_func__(self.OnMove))
		middleBar.Show()
		middleBar.SetUpVisual("d:/ymir work/ui/public/scrollbar_small_thin_middle_button_01.sub")
		middleBar.SetOverVisual("d:/ymir work/ui/public/scrollbar_small_thin_middle_button_01.sub")
		middleBar.SetDownVisual("d:/ymir work/ui/public/scrollbar_small_thin_middle_button_01.sub")

		upButton = Button()
		upButton.SetParent(self)
		upButton.SetUpVisual("d:/ymir work/ui/public/scrollbar_small_thin_up_button_01.sub")
		upButton.SetOverVisual("d:/ymir work/ui/public/scrollbar_small_thin_up_button_02.sub")
		upButton.SetDownVisual("d:/ymir work/ui/public/scrollbar_small_thin_up_button_03.sub")
		upButton.SetEvent(__mem_func__(self.OnUp))
		upButton.Show()

		downButton = Button()
		downButton.SetParent(self)
		downButton.SetUpVisual("d:/ymir work/ui/public/scrollbar_small_thin_down_button_01.sub")
		downButton.SetOverVisual("d:/ymir work/ui/public/scrollbar_small_thin_down_button_02.sub")
		downButton.SetDownVisual("d:/ymir work/ui/public/scrollbar_small_thin_down_button_03.sub")
		downButton.SetEvent(__mem_func__(self.OnDown))
		downButton.Show()

		self.middleBar = middleBar
		self.upButton = upButton
		self.downButton = downButton

		self.SCROLLBAR_WIDTH = self.upButton.GetWidth()
		self.SCROLLBAR_MIDDLE_HEIGHT = self.middleBar.GetHeight()
		self.SCROLLBAR_BUTTON_WIDTH = self.upButton.GetWidth()
		self.SCROLLBAR_BUTTON_HEIGHT = self.upButton.GetHeight()
		self.MIDDLE_BAR_POS = 0
		self.MIDDLE_BAR_UPPER_PLACE = 0
		self.MIDDLE_BAR_DOWNER_PLACE = 0
		self.TEMP_SPACE = 0

	def UpdateBarSlot(self):
		pass

class SliderBar(Window):

	def __init__(self):
		Window.__init__(self)

		self.curPos = 1.0
		self.pageSize = 1.0
		self.eventChange = None

		self.__CreateBackGroundImage()
		self.__CreateCursor()

	def __del__(self):
		Window.__del__(self)

	def __CreateBackGroundImage(self):
		img = ImageBox()
		img.SetParent(self)
		img.LoadImage("d:/ymir work/ui/game/windows/sliderbar.sub")
		img.Show()
		self.backGroundImage = img

		##
		self.SetSize(self.backGroundImage.GetWidth(), self.backGroundImage.GetHeight())

	def __CreateCursor(self):
		cursor = DragButton()
		cursor.AddFlag("movable")
		cursor.AddFlag("restrict_y")
		cursor.SetParent(self)
		cursor.SetMoveEvent(__mem_func__(self.__OnMove))
		cursor.SetUpVisual("d:/ymir work/ui/game/windows/sliderbar_cursor.sub")
		cursor.SetOverVisual("d:/ymir work/ui/game/windows/sliderbar_cursor.sub")
		cursor.SetDownVisual("d:/ymir work/ui/game/windows/sliderbar_cursor.sub")
		cursor.Show()
		self.cursor = cursor

		##
		self.cursor.SetRestrictMovementArea(0, 0, self.backGroundImage.GetWidth(), 0)
		self.pageSize = self.backGroundImage.GetWidth() - self.cursor.GetWidth()

	def __OnMove(self):
		(xLocal, yLocal) = self.cursor.GetLocalPosition()
		self.curPos = float(xLocal) / float(self.pageSize)

		if self.eventChange:
			self.eventChange()

	def SetSliderPos(self, pos):
		self.curPos = pos
		self.cursor.SetPosition(int(self.pageSize * pos), 0)

	def GetSliderPos(self):
		return self.curPos

	def SetEvent(self, event):
		self.eventChange = event

	def Enable(self):
		self.cursor.Show()

	def Disable(self):
		self.cursor.Hide()

	class CustomScrollBar(Window):

		HORIZONTAL 	= 1
		VERTICAL	= 2

		BOTTOM		= 1
		TOP			= 2
		RIGHT		= 3
		LEFT		= 4

		def __init__(self, template):
			Window.__init__(self)

			self.baseImage = None
			self.button1 = None
			self.button2 = None
			self.middleButton = None

			self.onScroll = None
			self.parent = None
			self.orientation = 0
			self.pos = 0.0
			self.middleScale = 0.1
			self.step = 0.1

			self.baseInfo = {}
			self.mouseOffset = {}


			base	= template.get('base', 			"")
			button1 = template.get('button1', 		{})
			button2 = template.get('button2', 		{})
			middle	= template.get('middle', 		{})
			onscroll= template.get('onscroll', 		None)
			orient	= template.get('orientation',	0)
			align	= template.get('align',			{})
			parent	= template.get('parent',		None)
			position= template.get('position',		{})

			if not base or not button1 or not button2 or not middle or not onscroll or not orient or not parent:
				dbg.TraceError("CustomScrollBar : cannot set template [%s]"%str(template))
				return

			self.__SetParent(parent)
			self.__SetOrientation(orient)
			self.__LoadBaseImage(base)
			self.__LoadButton1(button1)
			self.__LoadButton2(button2)
			self.__LoadMiddleButton(middle)

			self.__SetOnScrollEvent(onscroll)
			if template.has_key('align'):
				self.__SetAlign(align)

			elif template.has_key('position'):
				self.__SetPosition(position)

		def __SetParent(self, parent):
			if parent:
				self.parent = parent
				self.SetParent(parent)

		def __SetOrientation(self, orient):
			self.orientation = orient

		def __LoadBaseImage(self, base):
			bg = ExpandedImageBox()
			bg.LoadImage(base)
			bg.SetParent(self)
			bg.SetPosition(0,0)
			bg.Show()

			w , h = (bg.GetWidth() , bg.GetHeight())
			self.baseInfo = {'base' : {'width':w, 'height':h,}}
			self.SetSize(w,h)

			bg.OnMouseLeftButtonDown = self.__OnClickBaseImage
			self.baseImage = bg

		def __LoadButton1(self, button1):
			button1['event'] = self.__OnClickButton1

			btn = ExpandedButton(button1)
			btn.SetParent(self.baseImage)
			btn.SetPosition(0,0)
			btn.Show()

			self.button1 = btn

		def __LoadButton2(self, button2):
			button2['event'] = self.__OnClickButton2

			btn = ExpandedButton(button2)
			btn.SetParent(self.baseImage)

			if self.orientation == self.HORIZONTAL:
				leng = btn.GetWidth()
				btn.SetPosition(self.GetWidth() - leng , 0)

			elif self.orientation == self.VERTICAL:
				leng = btn.GetHeight()
				btn.SetPosition(0, self.GetHeight() - leng)
			btn.Show()

			self.button2 = btn

		def __LoadMiddleButton(self, middle):
			middle['downevent'] = self.__OnClickMiddle
			middle['update']	= self.__OnUpdateMiddleBar
			btn = ExpandedButton(middle)
			btn.SetParent(self.baseImage)


			if self.orientation == self.HORIZONTAL:
				btn.SetPosition(self.button1.GetWidth(), 0)

			elif self.orientation == self.VERTICAL:
				btn.SetPosition(0, self.button1.GetHeight())

			btn.Show()
			self.middleButton = btn

		def __SetOnScrollEvent(self, onscroll):
			self.onScroll = onscroll

		def __SetAlign(self, align):
			mode	= align['mode']
			offset1	= align.get('offset1',0)
			offset2	= align.get('offset2',0)

			if not self.parent:
				return

			if self.orientation == self.HORIZONTAL:
				if mode == self.TOP:
					self.SetPosition(offset1, 0)

				if mode == self.BOTTOM:
					self.SetPosition(offset1, self.parent.GetHeight() - self.GetHeight())

				self.SetScrollBarLength(self.parent.GetWidth() - (offset1 + offset2))


			elif self.orientation == self.VERTICAL:
				if mode == self.RIGHT:
					self.SetPosition(self.parent.GetWidth()-self.GetWidth(),  offset1 )

				elif mode == self.LEFT:
					self.SetPosition(0, offset1)

				self.SetScrollBarLength(self.parent.GetHeight() - (offset1 + offset2))

		def __SetPosition(self, position):
			self.SetPosition(position['x'] , position['y'])

		def SetScrollBarLength(self, leng):
			if self.orientation == self.VERTICAL:
				self.SetSize(self.GetWidth(), leng)


				baseScale = float(leng) / float(self.baseInfo['base']['height'])
				self.baseImage.SetScale(1.0, baseScale)

				scrollsize  = leng - (self.__GetElementLength(self.button1) + self.__GetElementLength(self.button2))
				middle_leng = int(self.middleScale * scrollsize)
				init_middle = float(self.middleButton.baseInfo['default']['height'])

				self.middleButton.SetScale(1.0, float(middle_leng)/init_middle)
				self.middleButton.SetPosition(0,self.__GetElementLength(self.button1) + int((scrollsize - self.__GetElementLength(self.middleButton))* self.pos))

				self.button2.SetPosition(0, self.GetHeight()-self.button2.GetHeight())

			elif self.orientation == self.HORIZONTAL:
				self.SetSize(leng, self.GetHeight())

				baseScale = float(leng) / float(self.baseInfo['base']['width'])
				self.baseImage.SetScale(baseScale, 1.0)


				scrollsize = leng - (self.__GetElementLength(self.button1) + self.__GetElementLength(self.button2))
				middle_leng = int(self.middleScale * scrollsize)
				init_middle = float(self.middleButton.baseInfo['default']['width'])

				self.middleButton.SetScale(float(middle_leng) / init_middle, 1.0)
				self.middleButton.SetPosition(self.__GetElementLength(self.button1) + int((scrollsize - self.__GetElementLength(self.middleButton)) * self.pos),0)

				self.button2.SetPosition(self.GetWidth() - self.button2.GetWidth(), 0)

		def __GetElementLength(self, element):
			if self.orientation == self.VERTICAL:
				return element.GetHeight()

			if self.orientation == self.HORIZONTAL:
				return element.GetWidth()
			return 0

		def __OnUpdateMiddleBar(self):
			if self.middleButton.status != ExpandedButton.STATUS_DOWN:
				return

			x,y 	= wndMgr.GetMousePosition()
			gx,gy	= self.middleButton.GetGlobalPosition()

			gx += self.mouseOffset.get('x',0)
			gy += self.mouseOffset.get('y',0)

			if self.orientation == self.VERTICAL:
				if y == gy:
					return

			elif self.orientation == self.HORIZONTAL:
				if x == gx:
					return

			self.__OnMoveMiddleBar(x,y)

		def __OnClickBaseImage(self):
			x,y 	= wndMgr.GetMousePosition()
			gx,gy	= self.middleButton.GetGlobalPosition()

			offset = self.__GetElementLength(self.middleButton)/2

			gx += offset
			gy += offset

			if self.orientation == self.VERTICAL:
				if y == gy:
					return

			elif self.orientation == self.HORIZONTAL:
				if x == gx:
					return

			self.mouseOffset = {'x' : offset, 'y': offset}
			self.__OnMoveMiddleBar(x,y)

		def __OnClickButton2(self):
			self.mouseOffset={'x' : 0, 'y' :0}
			gx,gy = self.middleButton.GetGlobalPosition()
			if self.orientation == self.VERTICAL:
				gy += self.__GetElementLength(self.middleButton)
			elif self.orientation == self.HORIZONTAL:
				gx += self.__GetElementLength(self.middleButton)

			self.__OnMoveMiddleBar(gx,gy)

		def __OnClickButton1(self):
			self.mouseOffset={'x' : 0, 'y' :0}
			gx, gy = self.middleButton.GetGlobalPosition()
			if self.orientation == self.VERTICAL:
				gy -= self.__GetElementLength(self.middleButton)
			elif self.orientation == self.HORIZONTAL:
				gx -= self.__GetElementLength(self.middleButton)

			self.__OnMoveMiddleBar(gx, gy)

		def __OnMoveMiddleBar(self, x , y):
			gx, gy = self.GetGlobalPosition()
			x -= self.mouseOffset.get('x', 0)
			y -= self.mouseOffset.get('y', 0)


			if self.orientation == self.VERTICAL:
				min_ = gy  + self.__GetElementLength(self.button1)
				max_ = min_ + (self.GetHeight() - (self.__GetElementLength(self.button1) + self.__GetElementLength(self.button2) + self.__GetElementLength(self.middleButton)))

				if max_ < y and self.pos == 1.0:
					return

				if min_ > y and self.pos == 0.0:
					return

				realy = max(y, min_)
				realy = min(realy, max_)
				scroll= max_-min_

				if scroll == 0.0:
					return


				self.pos = float(realy-min_) / float(scroll)
				self.middleButton.SetPosition(0, realy-gy)

				self.__OnScroll()

			elif self.orientation == self.HORIZONTAL:
				min_ = gx + self.__GetElementLength(self.button1)
				max_ = min_ + (self.GetWidth() - (self.__GetElementLength(self.button1) + self.__GetElementLength(self.button2) + self.__GetElementLength(self.middleButton)))

				if max_ < x and self.pos == 1.0:
					return

				if min_ > x and self.pos == 0.0:
					return

				realx = max(x, min_)
				realx = min(realx, max_)
				scroll = max_ - min_

				if scroll == 0.0:
					return

				self.pos = float(realx - min_) / float(scroll)
				self.middleButton.SetPosition(realx-gx, 0)

				self.__OnScroll()

		def __OnScroll(self):
			if self.onScroll:
				self.onScroll()

		def __OnClickMiddle(self):
			x,y 	= wndMgr.GetMousePosition()
			gx,gy	= self.middleButton.GetGlobalPosition()

			x-= gx
			y-= gy

			self.mouseOffset = {"x" : x, "y": y ,}

		def GetPos(self):
			return self.pos

		def GetStep(self):
			return self.step

		def SetScrollStep(self, step):
			step = min(1.0, max(0.1 , step))
			self.middleScale = step
			self.step = step

			self.SetScrollBarLength(self.__GetElementLength(self.baseImage))

class ListBox(Window):

	def GetSelectedItemText(self):
		return self.textDict.get(self.selectedLine, "")

	TEMPORARY_PLACE = 3

	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)
		self.overLine = -1
		self.selectedLine = -1
		self.width = 0
		self.height = 0
		self.stepSize = 17
		self.basePos = 0
		self.showLineCount = 0
		self.itemCenterAlign = TRUE
		self.itemList = []
		self.keyDict = {}
		self.textDict = {}
		self.event = lambda *arg: None
	def __del__(self):
		Window.__del__(self)

	def SetWidth(self, width):
		self.SetSize(width, self.height)

	def SetSize(self, width, height):
		Window.SetSize(self, width, height)
		self.width = width
		self.height = height

	def SetTextCenterAlign(self, flag):
		self.itemCenterAlign = flag

	def SetBasePos(self, pos):
		self.basePos = pos
		self._LocateItem()

	def ClearItem(self):
		self.keyDict = {}
		self.textDict = {}
		self.itemList = []
		self.overLine = -1
		self.selectedLine = -1

	def InsertItem(self, number, text):
		self.keyDict[len(self.itemList)] = number
		self.textDict[len(self.itemList)] = text

		textLine = TextLine()
		textLine.SetParent(self)
		textLine.SetText(text)
		textLine.Show()

		if self.itemCenterAlign:
			textLine.SetWindowHorizontalAlignCenter()
			textLine.SetHorizontalAlignCenter()

		self.itemList.append(textLine)

		self._LocateItem()

	def ChangeItem(self, number, text):
		for key, value in self.keyDict.items():
			if value == number:
				self.textDict[key] = text

				if number < len(self.itemList):
					self.itemList[key].SetText(text)

				return

	def LocateItem(self):
		self._LocateItem()

	def _LocateItem(self):

		skipCount = self.basePos
		yPos = 0
		self.showLineCount = 0

		for textLine in self.itemList:
			textLine.Hide()

			if skipCount > 0:
				skipCount -= 1
				continue

			if localeInfo.IsARABIC():
				w, h = textLine.GetTextSize()
				textLine.SetPosition(w+10, yPos + 3)
			else:
				textLine.SetPosition(0, yPos + 3)

			yPos += self.stepSize

			if yPos <= self.GetHeight():
				self.showLineCount += 1
				textLine.Show()

	def ArrangeItem(self):
		self.SetSize(self.width, len(self.itemList) * self.stepSize)
		self._LocateItem()

	def GetViewItemCount(self):
		return int(self.GetHeight() / self.stepSize)

	def GetItemCount(self):
		return len(self.itemList)

	def SetEvent(self, event):
		self.event = event

	def SelectItem(self, line):

		if not self.keyDict.has_key(line):
			return

		if line == self.selectedLine:
			return

		self.selectedLine = line
		self.event(self.keyDict.get(line, 0), self.textDict.get(line, "None"))

	def GetSelectedItem(self):
		return self.keyDict.get(self.selectedLine, 0)

	def OnMouseLeftButtonDown(self):
		if self.overLine < 0:
			return

	def OnMouseLeftButtonUp(self):
		if self.overLine >= 0:
			self.SelectItem(self.overLine+self.basePos)

	def OnUpdate(self):

		self.overLine = -1

		if self.IsIn():
			x, y = self.GetGlobalPosition()
			height = self.GetHeight()
			xMouse, yMouse = wndMgr.GetMousePosition()

			if yMouse - y < height - 1:
				self.overLine = (yMouse - y) / self.stepSize

				if self.overLine < 0:
					self.overLine = -1
				if self.overLine >= len(self.itemList):
					self.overLine = -1

	def OnRender(self):
		xRender, yRender = self.GetGlobalPosition()
		yRender -= self.TEMPORARY_PLACE
		widthRender = self.width
		heightRender = self.height + self.TEMPORARY_PLACE*2

		if localeInfo.IsCIBN10:
			if -1 != self.overLine and self.keyDict[self.overLine] != -1:
				grp.SetColor(HALF_WHITE_COLOR)
				grp.RenderBar(xRender + 2, yRender + self.overLine*self.stepSize + 4, self.width - 3, self.stepSize)				

			if -1 != self.selectedLine and self.keyDict[self.selectedLine] != -1:
				if self.selectedLine >= self.basePos:
					if self.selectedLine - self.basePos < self.showLineCount:
						grp.SetColor(SELECT_COLOR)
						grp.RenderBar(xRender + 2, yRender + (self.selectedLine-self.basePos)*self.stepSize + 4, self.width - 3, self.stepSize)

		else:		
			if -1 != self.overLine:
				grp.SetColor(HALF_WHITE_COLOR)
				grp.RenderBar(xRender + 2, yRender + self.overLine*self.stepSize + 4, self.width - 3, self.stepSize)				

			if -1 != self.selectedLine:
				if self.selectedLine >= self.basePos:
					if self.selectedLine - self.basePos < self.showLineCount:
						grp.SetColor(SELECT_COLOR)
						grp.RenderBar(xRender + 2, yRender + (self.selectedLine-self.basePos)*self.stepSize + 4, self.width - 3, self.stepSize)
						

class ListBox2(ListBox):
	def __init__(self, *args, **kwargs):
		ListBox.__init__(self, *args, **kwargs)
		self.rowCount = 10
		self.barWidth = 0
		self.colCount = 0

	def SetRowCount(self, rowCount):
		self.rowCount = rowCount

	def SetSize(self, width, height):
		ListBox.SetSize(self, width, height)
		self._RefreshForm()

	def ClearItem(self):
		ListBox.ClearItem(self)
		self._RefreshForm()

	def InsertItem(self, *args, **kwargs):
		ListBox.InsertItem(self, *args, **kwargs)
		self._RefreshForm()

	def OnUpdate(self):
		mpos = wndMgr.GetMousePosition()
		self.overLine = self._CalcPointIndex(mpos)

	def OnRender(self):
		x, y = self.GetGlobalPosition()
		pos = (x + 2, y)

		if -1 != self.overLine:
			grp.SetColor(HALF_WHITE_COLOR)
			self._RenderBar(pos, self.overLine)

		if -1 != self.selectedLine:
			if self.selectedLine >= self.basePos:
				if self.selectedLine - self.basePos < self.showLineCount:
					grp.SetColor(SELECT_COLOR)
					self._RenderBar(pos, self.selectedLine-self.basePos)

	

	def _CalcPointIndex(self, mpos):
		if self.IsIn():
			px, py = mpos
			gx, gy = self.GetGlobalPosition()
			lx, ly = px - gx, py - gy

			col = lx / self.barWidth
			row = ly / self.stepSize
			idx = col * self.rowCount + row
			if col >= 0 and col < self.colCount:
				if row >= 0 and row < self.rowCount:
					if idx >= 0 and idx < len(self.itemList):
						return idx
		
		return -1

	def _CalcRenderPos(self, pos, idx):
		x, y = pos
		row = idx % self.rowCount
		col = idx / self.rowCount
		return (x + col * self.barWidth, y + row * self.stepSize)

	def _RenderBar(self, basePos, idx):
		x, y = self._CalcRenderPos(basePos, idx)
		grp.RenderBar(x, y, self.barWidth - 3, self.stepSize)

	def _LocateItem(self):
		pos = (0, self.TEMPORARY_PLACE)

		self.showLineCount = 0
		for textLine in self.itemList:
			x, y = self._CalcRenderPos(pos, self.showLineCount)
			textLine.SetPosition(x, y)
			textLine.Show()

			self.showLineCount += 1

	def _RefreshForm(self):
		if len(self.itemList) % self.rowCount:
			self.colCount = len(self.itemList) / self.rowCount + 1
		else:
			self.colCount = len(self.itemList) / self.rowCount

		if self.colCount:
			self.barWidth = self.width / self.colCount
		else:
			self.barWidth = self.width


class ComboBox(Window):

	class ListBoxWithBoard(ListBox):

		def __init__(self, layer):
			ListBox.__init__(self, layer)

		def OnRender(self):
			xRender, yRender = self.GetGlobalPosition()
			yRender -= self.TEMPORARY_PLACE
			widthRender = self.width
			heightRender = self.height + self.TEMPORARY_PLACE*2
			grp.SetColor(BACKGROUND_COLOR)
			grp.RenderBar(xRender, yRender, widthRender, heightRender)
			grp.SetColor(DARK_COLOR)
			grp.RenderLine(xRender, yRender, widthRender, 0)
			grp.RenderLine(xRender, yRender, 0, heightRender)
			grp.SetColor(BRIGHT_COLOR)
			grp.RenderLine(xRender, yRender+heightRender, widthRender, 0)
			grp.RenderLine(xRender+widthRender, yRender, 0, heightRender)

			ListBox.OnRender(self)

	def __init__(self):
		Window.__init__(self)
		self.x = 0
		self.y = 0
		self.width = 0
		self.height = 0
		self.isSelected = FALSE
		self.isOver = FALSE
		self.isListOpened = FALSE
		self.event = lambda *arg: None
		self.enable = TRUE

		self.textLine = MakeTextLine(self)
		self.textLine.SetText(localeInfo.UI_ITEM)

		self.listBox = self.ListBoxWithBoard("TOP_MOST")
		self.listBox.SetPickAlways()
		self.listBox.SetParent(self)
		self.listBox.SetEvent(__mem_func__(self.OnSelectItem))
		self.listBox.Hide()

	def __del__(self):
		Window.__del__(self)

	def Destroy(self):
		self.textLine = None
		self.listBox = None

	def SetPosition(self, x, y):
		Window.SetPosition(self, x, y)
		self.x = x
		self.y = y
		self.__ArrangeListBox()

	def SetSize(self, width, height):
		Window.SetSize(self, width, height)
		self.width = width
		self.height = height
		self.textLine.UpdateRect()
		self.__ArrangeListBox()

	def __ArrangeListBox(self):
		self.listBox.SetPosition(0, self.height + 5)
		self.listBox.SetWidth(self.width)

	def Enable(self):
		self.enable = TRUE

	def Disable(self):
		self.enable = FALSE
		self.textLine.SetText("")
		self.CloseListBox()

	def SetEvent(self, event):
		self.event = event

	def ClearItem(self):
		self.CloseListBox()
		self.listBox.ClearItem()

	def InsertItem(self, index, name):
		self.listBox.InsertItem(index, name)
		self.listBox.ArrangeItem()

	def SetCurrentItem(self, text):
		self.textLine.SetText(text)

	def SelectItem(self, key):
		self.listBox.SelectItem(key)

	def OnSelectItem(self, index, name):

		self.CloseListBox()
		self.event(index)

	def CloseListBox(self):
		self.isListOpened = FALSE
		self.listBox.Hide()

	def OnMouseLeftButtonDown(self):

		if not self.enable:
			return

		self.isSelected = TRUE

	def OnMouseLeftButtonUp(self):

		if not self.enable:
			return

		self.isSelected = FALSE

		if self.isListOpened:
			self.CloseListBox()
		else:
			if self.listBox.GetItemCount() > 0:
				self.isListOpened = TRUE
				self.listBox.Show()
				self.__ArrangeListBox()

	def OnUpdate(self):

		if not self.enable:
			return

		if self.IsIn():
			self.isOver = TRUE
		else:
			self.isOver = FALSE

	def OnRender(self):
		self.x, self.y = self.GetGlobalPosition()
		xRender = self.x
		yRender = self.y
		widthRender = self.width
		heightRender = self.height
		grp.SetColor(BACKGROUND_COLOR)
		grp.RenderBar(xRender, yRender, widthRender, heightRender)
		grp.SetColor(DARK_COLOR)
		grp.RenderLine(xRender, yRender, widthRender, 0)
		grp.RenderLine(xRender, yRender, 0, heightRender)
		grp.SetColor(BRIGHT_COLOR)
		grp.RenderLine(xRender, yRender+heightRender, widthRender, 0)
		grp.RenderLine(xRender+widthRender, yRender, 0, heightRender)

		if self.isOver:
			grp.SetColor(HALF_WHITE_COLOR)
			grp.RenderBar(xRender + 2, yRender + 3, self.width - 3, heightRender - 5)

			if self.isSelected:
				grp.SetColor(WHITE_COLOR)
				grp.RenderBar(xRender + 2, yRender + 3, self.width - 3, heightRender - 5)

###################################################################################################
## Python Script Loader
###################################################################################################

class ScriptWindow(Window):
	def __init__(self, layer = "UI"):
		Window.__init__(self, layer)
		self.Children = []
		self.ElementDictionary = {}
	def __del__(self):
		Window.__del__(self)

	def ClearDictionary(self):
		self.Children = []
		self.ElementDictionary = {}
	def InsertChild(self, name, child):
		self.ElementDictionary[name] = child

	def IsChild(self, name):
		return self.ElementDictionary.has_key(name)
	def GetChild(self, name):
		return self.ElementDictionary[name]

	def GetChild2(self, name):
		return self.ElementDictionary.get(name, None)

class ListBoxScroll(ListBox):
	def __init__(self):
		ListBox.__init__(self)

		self.scrollBar = ScrollBar()
		self.scrollBar.SetParent(self)
		self.scrollBar.SetScrollEvent(self.__OnScroll)
		self.scrollBar.Hide()


	def SetSize(self, width, height):
		ListBox.SetSize(self, width - ScrollBar.SCROLLBAR_WIDTH, height)
		Window.SetSize(self, width, height)

		self.scrollBar.SetPosition(width - ScrollBar.SCROLLBAR_WIDTH, 0)
		self.scrollBar.SetScrollBarSize(height)


	def ClearItem(self):
		ListBox.ClearItem(self)
		self.scrollBar.SetPos(0)


	def _LocateItem(self):
		ListBox._LocateItem(self)

		if self.showLineCount < len(self.itemList):
			self.scrollBar.SetMiddleBarSize(float(self.GetViewItemCount())/self.GetItemCount())
			self.scrollBar.Show()
		else:
			self.scrollBar.Hide()


	def __OnScroll(self):
		scrollLen = self.GetItemCount()-self.GetViewItemCount()
		if scrollLen < 0:
			scrollLen = 0
		self.SetBasePos(int(self.scrollBar.GetPos()*scrollLen))

class PythonScriptLoader(object):

	BODY_KEY_LIST = ( "x", "y", "width", "height" )

	#####

	DEFAULT_KEY_LIST = ( "type", "x", "y", )
	WINDOW_KEY_LIST = ( "width", "height", )
	IMAGE_KEY_LIST = ( "image", )
	EXPANDED_IMAGE_KEY_LIST = ( "image", )
	ANI_IMAGE_KEY_LIST = ( "images", )
	SLOT_KEY_LIST = ( "width", "height", "slot", )
	CANDIDATE_LIST_KEY_LIST = ( "item_step", "item_xsize", "item_ysize", )
	GRID_TABLE_KEY_LIST = ( "start_index", "x_count", "y_count", "x_step", "y_step", )
	EDIT_LINE_KEY_LIST = ( "width", "height", "input_limit", )
	COMBO_BOX_KEY_LIST = ( "width", "height", "item", )
	TITLE_BAR_KEY_LIST = ( "width", )
	HORIZONTAL_BAR_KEY_LIST = ( "width", )
	BOARD_KEY_LIST = ( "width", "height", )
	BOARD_WITH_TITLEBAR_KEY_LIST = ( "width", "height", "title", )
	BOX_KEY_LIST = ( "width", "height", )
	BAR_KEY_LIST = ( "width", "height", )
	LINE_KEY_LIST = ( "width", "height", )
	SLOTBAR_KEY_LIST = ( "width", "height", )
	GAUGE_KEY_LIST = ( "width", "color", )
	SCROLLBAR_KEY_LIST = ( "size", )
	LIST_BOX_KEY_LIST = ( "width", "height", )

	def __init__(self):
		self.Clear()

	def Clear(self):
		self.ScriptDictionary = { "SCREEN_WIDTH" : wndMgr.GetScreenWidth(), "SCREEN_HEIGHT" : wndMgr.GetScreenHeight() }
		self.InsertFunction = 0

	def LoadScriptFile(self, window, FileName):
		import exception
		import exceptions
		import os
		import errno
		self.Clear()

		print "===== Load Script File : %s" % (FileName)

		try:
			# chr, player ���� sandbox ������ import�� ������ �ʱ� ������,(���� �ǿ��� ������ �ſ� ŭ.)
			#  �̸� script dictionary�� �ʿ��� ����� �־���´�.
			import chr
			import player
			import app
			self.ScriptDictionary["PLAYER_NAME_MAX_LEN"] = chr.PLAYER_NAME_MAX_LEN
			self.ScriptDictionary["DRAGON_SOUL_EQUIPMENT_SLOT_START"] = player.DRAGON_SOUL_EQUIPMENT_SLOT_START
			self.ScriptDictionary["LOCALE_PATH"] = app.GetLocalePath()
			execfile(FileName, self.ScriptDictionary)
		except IOError, err:
			import sys
			import dbg			
			dbg.TraceError("Failed to load script file : %s" % (FileName))
			dbg.TraceError("error  : %s" % (err))
			exception.Abort("LoadScriptFile1")
		except RuntimeError,err:
			import sys
			import dbg			
			dbg.TraceError("Failed to load script file : %s" % (FileName))
			dbg.TraceError("error  : %s" % (err))
			exception.Abort("LoadScriptFile2")
		except:
			import sys
			import dbg			
			dbg.TraceError("Failed to load script file : %s" % (FileName))
			exception.Abort("LoadScriptFile!!!!!!!!!!!!!!")
		
		#####

		Body = self.ScriptDictionary["window"]
		self.CheckKeyList("window", Body, self.BODY_KEY_LIST)

		window.ClearDictionary()
		self.InsertFunction = window.InsertChild

		window.SetPosition(int(Body["x"]), int(Body["y"]))

		if localeInfo.IsARABIC(): 
			w = wndMgr.GetScreenWidth()
			h = wndMgr.GetScreenHeight()
			if Body.has_key("width"):
				w = int(Body["width"])
			if Body.has_key("height"):
				h = int(Body["height"])

			window.SetSize(w, h)
		else:
			window.SetSize(int(Body["width"]), int(Body["height"]))
			if TRUE == Body.has_key("style"):
				for StyleList in Body["style"]:
					window.AddFlag(StyleList)
		

		self.LoadChildren(window, Body)

	def LoadChildren(self, parent, dicChildren):

		if localeInfo.IsARABIC():
			parent.AddFlag( "rtl" )

		if TRUE == dicChildren.has_key("style"):
			for style in dicChildren["style"]:
				parent.AddFlag(style)

		if FALSE == dicChildren.has_key("children"):
			return FALSE

		Index = 0

		ChildrenList = dicChildren["children"]
		parent.Children = range(len(ChildrenList))
		for ElementValue in ChildrenList:
			try:
				Name = ElementValue["name"]				
			except KeyError:
				Name = ElementValue["name"] = "NONAME"
				
			try:
				Type = ElementValue["type"]
			except KeyError:								
				Type = ElementValue["type"] = "window"				

			if FALSE == self.CheckKeyList(Name, ElementValue, self.DEFAULT_KEY_LIST):
				del parent.Children[Index]
				continue

			if Type == "window":
				parent.Children[Index] = ScriptWindow()
				parent.Children[Index].SetParent(parent)
				self.LoadElementWindow(parent.Children[Index], ElementValue, parent)

			elif Type == "button":
				parent.Children[Index] = Button()
				parent.Children[Index].SetParent(parent)
				self.LoadElementButton(parent.Children[Index], ElementValue, parent)

			elif Type == "radio_button":
				parent.Children[Index] = RadioButton()
				parent.Children[Index].SetParent(parent)
				self.LoadElementButton(parent.Children[Index], ElementValue, parent)

			elif Type == "toggle_button":
				parent.Children[Index] = ToggleButton()
				parent.Children[Index].SetParent(parent)
				self.LoadElementButton(parent.Children[Index], ElementValue, parent)

			elif Type == "mark":
				parent.Children[Index] = MarkBox()
				parent.Children[Index].SetParent(parent)
				self.LoadElementMark(parent.Children[Index], ElementValue, parent)

			elif Type == "image":
				parent.Children[Index] = ImageBox()
				parent.Children[Index].SetParent(parent)
				self.LoadElementImage(parent.Children[Index], ElementValue, parent)

			elif Type == "expanded_image":
				parent.Children[Index] = ExpandedImageBox()
				parent.Children[Index].SetParent(parent)
				self.LoadElementExpandedImage(parent.Children[Index], ElementValue, parent)

			elif Type == "ani_image":
				parent.Children[Index] = AniImageBox()
				parent.Children[Index].SetParent(parent)
				self.LoadElementAniImage(parent.Children[Index], ElementValue, parent)

			elif Type == "slot":
				parent.Children[Index] = SlotWindow()
				parent.Children[Index].SetParent(parent)
				self.LoadElementSlot(parent.Children[Index], ElementValue, parent)

			elif Type == "candidate_list":
				parent.Children[Index] = CandidateListBox()
				parent.Children[Index].SetParent(parent)
				self.LoadElementCandidateList(parent.Children[Index], ElementValue, parent)

			elif Type == "grid_table":
				parent.Children[Index] = GridSlotWindow()
				parent.Children[Index].SetParent(parent)
				self.LoadElementGridTable(parent.Children[Index], ElementValue, parent)

			elif Type == "text":
				parent.Children[Index] = TextLine()
				parent.Children[Index].SetParent(parent)
				self.LoadElementText(parent.Children[Index], ElementValue, parent)

			elif Type == "multitext":
				parent.Children[Index] = MultiTextLine()
				parent.Children[Index].SetParent(parent)
				self.LoadElementText(parent.Children[Index], ElementValue, parent)

			elif Type == "editline":
				parent.Children[Index] = EditLine()
				parent.Children[Index].SetParent(parent)
				self.LoadElementEditLine(parent.Children[Index], ElementValue, parent)

			elif Type == "titlebar":
				parent.Children[Index] = TitleBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementTitleBar(parent.Children[Index], ElementValue, parent)

			elif Type == "subtitlebar":
				parent.Children[Index] = SubTitleBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementSubTitleBar(parent.Children[Index], ElementValue, parent)
				
			elif Type == "listbar":
				parent.Children[Index] = ListBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementListBar(parent.Children[Index], ElementValue, parent)	
			
			elif Type == "horizontalbar":
				parent.Children[Index] = HorizontalBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementHorizontalBar(parent.Children[Index], ElementValue, parent)
				
			elif Type == "world_gui_ierik":
				parent.Children[Index] = world_gui_ierik()
				parent.Children[Index].SetParent(parent)
				self.world_gui_ierik(parent.Children[Index], ElementValue, parent)

			elif Type == "check_box":
				parent.Children[Index] = CheckBox()
				parent.Children[Index].SetParent(parent)
				self.LoadElementBoard(parent.Children[Index], ElementValue, parent)
			
			elif Type == "board":
				parent.Children[Index] = Board()
				parent.Children[Index].SetParent(parent)
				self.LoadElementBoard(parent.Children[Index], ElementValue, parent)

			elif Type == "board_with_titlebar":
				parent.Children[Index] = BoardWithTitleBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementBoardWithTitleBar(parent.Children[Index], ElementValue, parent)

			elif Type == "middleboard":
				parent.Children[Index] = MiddleBoard()
				parent.Children[Index].SetParent(parent)
				self.LoadElementMiddleBoard(parent.Children[Index], ElementValue, parent)

			elif Type == "thinboard":
				parent.Children[Index] = ThinBoard()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)

			elif Type == "thinboard_gold":
				parent.Children[Index] = ThinBoardGold()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoardGold(parent.Children[Index], ElementValue, parent)

			elif Type == "thinboard_circle":
				parent.Children[Index] = ThinBoardCircle()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)

			elif Type == "thinboard_yeni":
				parent.Children[Index] = ThinBoard_yeni()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoardYeni(parent.Children[Index], ElementValue, parent)

			elif Type == "thinboardwing":
				parent.Children[Index] = ThinBoardWing()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)

			elif Type == "thinboardribon":
				parent.Children[Index] = ThinBoardRibon()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)

			elif Type == "thinboardpaper":
				parent.Children[Index] = ThinBoardPaper()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)

			elif Type == "thinboardice":
				parent.Children[Index] = ThinBoardIce()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)

			elif Type == "thinboardfire":
				parent.Children[Index] = ThinBoardFire()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)

			elif Type == "thinboardnorm":
				parent.Children[Index] = ThinBoardNorm()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)

			elif Type == "thinboard_KasmirPaket_normal":
				parent.Children[Index] = ThinBoard_KasmirPaket_Normal()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)
			
			elif Type == "thinboard_KasmirPaket_fire":
				parent.Children[Index] = ThinBoard_KasmirPaket_Fire()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)
			
			elif Type == "thinboard_KasmirPaket_ice":
				parent.Children[Index] = ThinBoard_KasmirPaket_Ice()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)
			
			elif Type == "thinboard_KasmirPaket_paper":
				parent.Children[Index] = ThinBoard_KasmirPaket_Paper()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)
			
			elif Type == "thinboard_KasmirPaket_ribon":
				parent.Children[Index] = ThinBoard_KasmirPaket_Ribon()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)
			
			elif Type == "thinboard_KasmirPaket_wing":
				parent.Children[Index] = ThinBoard_KasmirPaket_Wing()
				parent.Children[Index].SetParent(parent)
				self.LoadElementThinBoard(parent.Children[Index], ElementValue, parent)

			elif Type == "box":
				parent.Children[Index] = Box()
				parent.Children[Index].SetParent(parent)
				self.LoadElementBox(parent.Children[Index], ElementValue, parent)

			elif Type == "bar":
				parent.Children[Index] = Bar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementBar(parent.Children[Index], ElementValue, parent)

			elif Type == "line":
				parent.Children[Index] = Line()
				parent.Children[Index].SetParent(parent)
				self.LoadElementLine(parent.Children[Index], ElementValue, parent)

			elif Type == "slotbar":
				parent.Children[Index] = SlotBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementSlotBar(parent.Children[Index], ElementValue, parent)

			elif Type == "gauge":
				parent.Children[Index] = Gauge()
				parent.Children[Index].SetParent(parent)
				self.LoadElementGauge(parent.Children[Index], ElementValue, parent)

			elif Type == "scrollbar":
				parent.Children[Index] = ScrollBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementScrollBar(parent.Children[Index], ElementValue, parent)

			elif Type == "thin_scrollbar":
				parent.Children[Index] = ThinScrollBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementScrollBar(parent.Children[Index], ElementValue, parent)

			elif Type == "small_thin_scrollbar":
				parent.Children[Index] = SmallThinScrollBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementScrollBar(parent.Children[Index], ElementValue, parent)

			elif Type == "sliderbar":
				parent.Children[Index] = SliderBar()
				parent.Children[Index].SetParent(parent)
				self.LoadElementSliderBar(parent.Children[Index], ElementValue, parent)

			elif Type == "listbox":
				parent.Children[Index] = ListBox()
				parent.Children[Index].SetParent(parent)
				self.LoadElementListBox(parent.Children[Index], ElementValue, parent)

			elif Type == "listbox_scroll": 
				parent.Children[Index] = ListBoxScroll()
				parent.Children[Index].SetParent(parent)
				self.LoadElementListBox(parent.Children[Index], ElementValue, parent)
			
			elif Type == "listbox2":
				parent.Children[Index] = ListBox2()
				parent.Children[Index].SetParent(parent)
				self.LoadElementListBox2(parent.Children[Index], ElementValue, parent)
			elif Type == "numberline":
				parent.Children[Index] = NumberLine()
				parent.Children[Index].SetParent(parent)
				self.LoadElementNumberLine(parent.Children[Index], ElementValue, parent)
			elif Type == "listboxex":
				parent.Children[Index] = ListBoxEx()
				parent.Children[Index].SetParent(parent)
				self.LoadElementListBoxEx(parent.Children[Index], ElementValue, parent)

			elif Type == "resizable_text_value":
				parent.Children[Index] = ResizableTextValue()
				parent.Children[Index].SetParent(parent)
				self.LoadElementResizableTextValue(parent.Children[Index], ElementValue, parent)

			elif Type == "table":
				parent.Children[Index] = Table()
				parent.Children[Index].SetParent(parent)
				self.LoadElementTable(parent.Children[Index], ElementValue, parent)

			elif Type == "border_a":
				parent.Children[Index] = BorderA()
				parent.Children[Index].SetParent(parent)
				self.LoadElementBoard(parent.Children[Index], ElementValue, parent)

			elif Type == "border_b":
				parent.Children[Index] = BorderB()
				parent.Children[Index].SetParent(parent)
				self.LoadElementBoard(parent.Children[Index], ElementValue, parent)

			elif Type == "titlebar_a":
				parent.Children[Index] = TitleBarA()
				parent.Children[Index].SetParent(parent)
				self.LoadElementTitleBar(parent.Children[Index], ElementValue, parent)

			elif Type == "input":
				parent.Children[Index] = Input()
				parent.Children[Index].SetParent(parent)
				self.LoadElementInput(parent.Children[Index], ElementValue, parent)

			else:
				Index += 1
				continue

			parent.Children[Index].SetWindowName(Name)
			if 0 != self.InsertFunction:
				self.InsertFunction(Name, parent.Children[Index])

			self.LoadChildren(parent.Children[Index], ElementValue)
			Index += 1

	def CheckKeyList(self, name, value, key_list):

		for DataKey in key_list:
			if FALSE == value.has_key(DataKey):
				print "Failed to find data key", "[" + name + "/" + DataKey + "]"
				return FALSE

		return TRUE

	def LoadDefaultData(self, window, value, parentWindow):
		loc_x = int(value["x"])
		loc_y = int(value["y"])
		if value.has_key("vertical_align"):
			if "center" == value["vertical_align"]:
				window.SetWindowVerticalAlignCenter()
			elif "bottom" == value["vertical_align"]:
				window.SetWindowVerticalAlignBottom()

		if parentWindow.IsRTL():
			loc_x = int(value["x"]) + window.GetWidth()
			if value.has_key("horizontal_align"):
				if "center" == value["horizontal_align"]:
					window.SetWindowHorizontalAlignCenter()
					loc_x = - int(value["x"])
				elif "right" == value["horizontal_align"]:
					window.SetWindowHorizontalAlignLeft()
					loc_x = int(value["x"]) - window.GetWidth()
					## loc_x = parentWindow.GetWidth() - int(value["x"]) + window.GetWidth()
			else:
				window.SetWindowHorizontalAlignRight()

			if value.has_key("all_align"):
				window.SetWindowVerticalAlignCenter()
				window.SetWindowHorizontalAlignCenter()
				loc_x = - int(value["x"])
		else:
			if value.has_key("horizontal_align"):
				if "center" == value["horizontal_align"]:
					window.SetWindowHorizontalAlignCenter()
				elif "right" == value["horizontal_align"]:
					window.SetWindowHorizontalAlignRight()

		window.SetPosition(loc_x, loc_y)
		window.Show()

	## Window
	def LoadElementWindow(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.WINDOW_KEY_LIST):
			return FALSE

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## Button
	def LoadElementButton(self, window, value, parentWindow):

		if value.has_key("width") and value.has_key("height"):
			window.SetSize(int(value["width"]), int(value["height"]))

		if TRUE == value.has_key("default_image"):
			window.SetUpVisual(value["default_image"])
		if TRUE == value.has_key("over_image"):
			window.SetOverVisual(value["over_image"])
		if TRUE == value.has_key("down_image"):
			window.SetDownVisual(value["down_image"])
		if TRUE == value.has_key("disable_image"):
			window.SetDisableVisual(value["disable_image"])

		if TRUE == value.has_key("text"):
			if TRUE == value.has_key("text_height"):
				window.SetText(value["text"], value["text_height"])
			else:
				window.SetText(value["text"])

			if value.has_key("text_color"):
				window.SetTextColor(value["text_color"])

		if TRUE == value.has_key("tooltip_text"):
			if TRUE == value.has_key("tooltip_x") and TRUE == value.has_key("tooltip_y"):
				window.SetToolTipText(value["tooltip_text"], int(value["tooltip_x"]), int(value["tooltip_y"]))
			else:
				window.SetToolTipText(value["tooltip_text"])

		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## Mark
	def LoadElementMark(self, window, value, parentWindow):

		#if FALSE == self.CheckKeyList(value["name"], value, self.MARK_KEY_LIST):
		#	return FALSE

		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## Image
	def LoadElementImage(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.IMAGE_KEY_LIST):
			return FALSE

		window.LoadImage(value["image"])
		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## AniImage
	def LoadElementAniImage(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.ANI_IMAGE_KEY_LIST):
			return FALSE

		if TRUE == value.has_key("delay"):
			window.SetDelay(value["delay"])
		
		if TRUE == value.has_key("x_scale") and TRUE == value.has_key("y_scale"):
			for image in value["images"]:
				window.AppendImageScale(image, float(value["x_scale"]), float(value["y_scale"]))
		else:
			for image in value["images"]:
				window.AppendImage(image)

		if value.has_key("width") and value.has_key("height"):
			window.SetSize(value["width"], value["height"])

		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## Expanded Image
	def LoadElementExpandedImage(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.EXPANDED_IMAGE_KEY_LIST):
			return FALSE

		window.LoadImage(value["image"])

		if TRUE == value.has_key("x_origin") and TRUE == value.has_key("y_origin"):
			window.SetOrigin(float(value["x_origin"]), float(value["y_origin"]))

		if TRUE == value.has_key("x_scale") and TRUE == value.has_key("y_scale"):
			window.SetScale(float(value["x_scale"]), float(value["y_scale"]))

		if TRUE == value.has_key("rect"):
			RenderingRect = value["rect"]
			window.SetRenderingRect(RenderingRect[0], RenderingRect[1], RenderingRect[2], RenderingRect[3])

		if TRUE == value.has_key("mode"):
			mode = value["mode"]
			if "MODULATE" == mode:
				window.SetRenderingMode(wndMgr.RENDERING_MODE_MODULATE)

		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## Slot
	def LoadElementSlot(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.SLOT_KEY_LIST):
			return FALSE

		global_x = int(value["x"])
		global_y = int(value["y"])
		global_width = int(value["width"])
		global_height = int(value["height"])

		window.SetPosition(global_x, global_y)
		window.SetSize(global_width, global_height)
		window.Show()

		r = 1.0
		g = 1.0
		b = 1.0
		a = 1.0

		if TRUE == value.has_key("image_r") and \
			TRUE == value.has_key("image_g") and \
			TRUE == value.has_key("image_b") and \
			TRUE == value.has_key("image_a"):
			r = float(value["image_r"])
			g = float(value["image_g"])
			b = float(value["image_b"])
			a = float(value["image_a"])

		SLOT_ONE_KEY_LIST = ("index", "x", "y", "width", "height")

		for slot in value["slot"]:
			if TRUE == self.CheckKeyList(value["name"] + " - one", slot, SLOT_ONE_KEY_LIST):
				wndMgr.AppendSlot(window.hWnd,
									int(slot["index"]),
									int(slot["x"]),
									int(slot["y"]),
									int(slot["width"]),
									int(slot["height"]))

		if TRUE == value.has_key("image"):
			if TRUE == value.has_key("x_scale") and TRUE == value.has_key("y_scale"):
				wndMgr.SetSlotBaseImageScale(window.hWnd,
										value["image"],
										r, g, b, a, float(value["x_scale"]), float(value["y_scale"]))
			else:
				wndMgr.SetSlotBaseImage(window.hWnd,
										value["image"],
										r, g, b, a)

		return TRUE

	def LoadElementCandidateList(self, window, value, parentWindow):
		if FALSE == self.CheckKeyList(value["name"], value, self.CANDIDATE_LIST_KEY_LIST):
			return FALSE

		window.SetPosition(int(value["x"]), int(value["y"]))
		window.SetItemSize(int(value["item_xsize"]), int(value["item_ysize"]))
		window.SetItemStep(int(value["item_step"]))		
		window.Show()

		return TRUE
				
	## Table
	def LoadElementGridTable(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.GRID_TABLE_KEY_LIST):
			return FALSE

		xBlank = 0
		yBlank = 0
		if TRUE == value.has_key("x_blank"):
			xBlank = int(value["x_blank"])
		if TRUE == value.has_key("y_blank"):
			yBlank = int(value["y_blank"])

		if localeInfo.IsARABIC():
			pass
		else:
			window.SetPosition(int(value["x"]), int(value["y"]))

		window.ArrangeSlot(	int(value["start_index"]),
							int(value["x_count"]),
							int(value["y_count"]),
							int(value["x_step"]),
							int(value["y_step"]),
							xBlank,
							yBlank)
		if TRUE == value.has_key("image"):
			r = 1.0
			g = 1.0
			b = 1.0
			a = 1.0
			if TRUE == value.has_key("image_r") and \
				TRUE == value.has_key("image_g") and \
				TRUE == value.has_key("image_b") and \
				TRUE == value.has_key("image_a"):
				r = float(value["image_r"])
				g = float(value["image_g"])
				b = float(value["image_b"])
				a = float(value["image_a"])
			wndMgr.SetSlotBaseImage(window.hWnd, value["image"], r, g, b, a)

		if TRUE == value.has_key("style"):
			if "select" == value["style"]:
				wndMgr.SetSlotStyle(window.hWnd, wndMgr.SLOT_STYLE_SELECT)
		if localeInfo.IsARABIC():
			self.LoadDefaultData(window, value, parentWindow)
		else:
			window.Show()

		return TRUE

	## Text
	def LoadElementText(self, window, value, parentWindow):

		if value.has_key("fontsize"):
			fontSize = value["fontsize"]

			if "LARGE" == fontSize:
				window.SetFontName(localeInfo.UI_DEF_FONT_LARGE)

		elif value.has_key("fontname"):
			fontName = value["fontname"]
			window.SetFontName(fontName)

		if value.has_key("text_horizontal_align"):
			if "left" == value["text_horizontal_align"]:
				window.SetHorizontalAlignLeft()
			elif "center" == value["text_horizontal_align"]:
				window.SetHorizontalAlignCenter()
			elif "right" == value["text_horizontal_align"]:
				window.SetHorizontalAlignRight()

		if value.has_key("text_vertical_align"):
			if "top" == value["text_vertical_align"]:
				window.SetVerticalAlignTop()
			elif "center" == value["text_vertical_align"]:
				window.SetVerticalAlignCenter()
			elif "bottom" == value["text_vertical_align"]:
				window.SetVerticalAlignBottom()

		if value.has_key("all_align"):
			window.SetHorizontalAlignCenter()
			window.SetVerticalAlignCenter()
			window.SetWindowHorizontalAlignCenter()
			window.SetWindowVerticalAlignCenter()

		if value.has_key("r") and value.has_key("g") and value.has_key("b"):
			window.SetFontColor(float(value["r"]), float(value["g"]), float(value["b"]))
		elif value.has_key("color"):
			window.SetPackedFontColor(value["color"])
		else:
			window.SetFontColor(0.8549, 0.8549, 0.8549)

		if value.has_key("outline"):
			if value["outline"]:
				window.SetOutline()
		if TRUE == value.has_key("text"):
			window.SetText(value["text"])

		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	def LoadElementMultiText(self, window, value, parentWindow):

		if value.has_key("fontsize"):
			fontSize = value["fontsize"]

			if "LARGE" == fontSize:
				window.SetFontName(localeInfo.UI_DEF_FONT_LARGE)

		elif value.has_key("fontname"):
			fontName = value["fontname"]
			window.SetFontName(fontName)

		if value.has_key("text_horizontal_align"):
			if "left" == value["text_horizontal_align"]:
				window.SetHorizontalAlignLeft()
			elif "center" == value["text_horizontal_align"]:
				window.SetHorizontalAlignCenter()
			elif "right" == value["text_horizontal_align"]:
				window.SetHorizontalAlignRight()

		if value.has_key("text_vertical_align"):
			if "top" == value["text_vertical_align"]:
				window.SetVerticalAlignTop()
			elif "center" == value["text_vertical_align"]:
				window.SetVerticalAlignCenter()
			elif "bottom" == value["text_vertical_align"]:
				window.SetVerticalAlignBottom()

		if value.has_key("all_align"):
			window.SetHorizontalAlignCenter()
			window.SetVerticalAlignCenter()
			window.SetWindowHorizontalAlignCenter()
			window.SetWindowVerticalAlignCenter()

		if value.has_key("r") and value.has_key("g") and value.has_key("b"):
			window.SetFontColor(float(value["r"]), float(value["g"]), float(value["b"]))
		elif value.has_key("color"):
			window.SetPackedFontColor(value["color"])
		else:
			window.SetFontColor(0.8549, 0.8549, 0.8549)

		if value.has_key("outline"):
			if value["outline"]:
				window.SetOutline()
		if TRUE == value.has_key("text"):
			window.SetText(value["text"])

		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## EditLine
	def LoadElementEditLine(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.EDIT_LINE_KEY_LIST):
			return FALSE


		if value.has_key("secret_flag"):
			window.SetSecret(value["secret_flag"])
		if value.has_key("with_codepage"):
			if value["with_codepage"]:
				window.bCodePage = TRUE
		if value.has_key("only_number"):
			if value["only_number"]:
				window.SetNumberMode()
		if value.has_key("enable_codepage"):
			window.SetIMEFlag(value["enable_codepage"])
		if value.has_key("enable_ime"):
			window.SetIMEFlag(value["enable_ime"])
		if value.has_key("limit_width"):
			window.SetLimitWidth(value["limit_width"])
		if value.has_key("multi_line"):
			if value["multi_line"]:
				window.SetMultiLine()

		window.SetMax(int(value["input_limit"]))
		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadElementText(window, value, parentWindow)

		return TRUE

	## SubTitleBar
	def LoadElementSubTitleBar(self, window, value, parentWindow):
		if False == self.CheckKeyList(value["name"], value, self.SUB_TITLE_BAR_KEY_LIST):
			return False

		window.MakeSubTitleBar(int(value["width"]), value.get("color", "red"))
		#self.LoadDefaultData(window, value, parentWindow)
		self.LoadElementButton(window, value, parentWindow)
		#test
		window.Show()
		return True

	## ListBar
	def LoadElementListBar(self, window, value, parentWindow):
		if False == self.CheckKeyList(value["name"], value, self.LIST_BAR_KEY_LIST):
			return False

		window.MakeListBar(int(value["width"]), value.get("color", "red"))
		#self.LoadDefaultData(window, value, parentWindow)
		self.LoadElementButton(window, value, parentWindow)

		return True

	## TitleBar
	def LoadElementTitleBar(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.TITLE_BAR_KEY_LIST):
			return FALSE

		window.MakeTitleBar(int(value["width"]), value.get("color", "red"))
		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## HorizontalBar
	def LoadElementHorizontalBar(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.HORIZONTAL_BAR_KEY_LIST):
			return FALSE

		window.Create(int(value["width"]))
		self.LoadDefaultData(window, value, parentWindow)

		return TRUE
		
	def world_gui_ierik(self, window, value, parentWindow):
		if False == self.CheckKeyList(value["name"], value, self.BOARD_KEY_LIST):
			return False
		
		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)
		return True

	## Board
	def LoadElementBoard(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.BOARD_KEY_LIST):
			return FALSE

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## Board With TitleBar
	def LoadElementBoardWithTitleBar(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.BOARD_WITH_TITLEBAR_KEY_LIST):
			return FALSE

		window.SetSize(int(value["width"]), int(value["height"]))
		window.SetTitleName(value["title"])
		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## ThinBoard
	def LoadElementThinBoard(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.BOARD_KEY_LIST):
			return FALSE

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	def LoadElementMiddleBoard(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.BOARD_KEY_LIST):
			return FALSE

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return True

	def LoadElementThinBoardGold(self, window, value, parentWindow):

		if False == self.CheckKeyList(value["name"], value, self.BOARD_KEY_LIST):
			return False

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return True

	def LoadElementThinBoardCircle(self, window, value, parentWindow):
		if FALSE == self.CheckKeyList(value["name"], value, self.BOARD_KEY_LIST):
			return FALSE

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)
		return TRUE

	def LoadElementThinBoardYeni(self, window, value, parentWindow):
		if FALSE == self.CheckKeyList(value["name"], value, self.BOARD_KEY_LIST):
			return FALSE

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## Box
	def LoadElementBox(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.BOX_KEY_LIST):
			return FALSE

		if TRUE == value.has_key("color"):
			window.SetColor(value["color"])

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## Bar
	def LoadElementBar(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.BAR_KEY_LIST):
			return FALSE

		if TRUE == value.has_key("color"):
			window.SetColor(value["color"])

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## Line
	def LoadElementLine(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.LINE_KEY_LIST):
			return FALSE

		if TRUE == value.has_key("color"):
			window.SetColor(value["color"])

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## Slot
	def LoadElementSlotBar(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.SLOTBAR_KEY_LIST):
			return FALSE

		window.SetSize(int(value["width"]), int(value["height"]))
		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## Gauge
	def LoadElementGauge(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.GAUGE_KEY_LIST):
			return FALSE

		window.MakeGauge(value["width"], value["color"])
		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## ScrollBar
	def LoadElementScrollBar(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.SCROLLBAR_KEY_LIST):
			return FALSE

		window.SetScrollBarSize(value["size"])
		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## SliderBar
	def LoadElementSliderBar(self, window, value, parentWindow):

		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## ListBox
	def LoadElementListBox(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.LIST_BOX_KEY_LIST):
			return FALSE

		if value.has_key("item_align"):
			window.SetTextCenterAlign(value["item_align"])

		window.SetSize(value["width"], value["height"])
		self.LoadDefaultData(window, value, parentWindow)

		return TRUE

	## ListBox2
	def LoadElementListBox2(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.LIST_BOX_KEY_LIST):
			return FALSE

		window.SetRowCount(value.get("row_count", 10)) 
		window.SetSize(value["width"], value["height"])
		self.LoadDefaultData(window, value, parentWindow)

		if value.has_key("item_align"):
			window.SetTextCenterAlign(value["item_align"])

		return TRUE
	def LoadElementListBoxEx(self, window, value, parentWindow):

		if FALSE == self.CheckKeyList(value["name"], value, self.LIST_BOX_KEY_LIST):
			return FALSE

		window.SetSize(value["width"], value["height"])
		self.LoadDefaultData(window, value, parentWindow)

		if value.has_key("itemsize_x") and value.has_key("itemsize_y"):
			window.SetItemSize(int(value["itemsize_x"]), int(value["itemsize_y"]))

		if value.has_key("itemstep"):
			window.SetItemStep(int(value["itemstep"]))

		if value.has_key("viewcount"):
			window.SetViewItemCount(int(value["viewcount"]))

		return TRUE

	def LoadElementNumberLine(self, window, value, parentWindow):
		self.LoadDefaultData(window, value, parentWindow)
		return True

	def LoadElementResizableTextValue(self, window, value, parentWindow):

		if value.has_key("width") and value.has_key("height"):
			window.SetSize(int(value["width"]), int(value["height"]))

		if TRUE == value.has_key("text"):
			window.SetText(value["text"])
			
		if value.has_key("line_color"):
			window.SetLineColor(value["line_color"])
			
		if value.has_key("color"):
			window.SetBackgroundColor(value["color"])
			
		if value.has_key("line_top"):
			window.SetLine('top')
		if value.has_key("line_bottom"):
			window.SetLine('bottom')
		if value.has_key("line_left"):
			window.SetLine('left')
		if value.has_key("line_right"):
			window.SetLine('right')
			
		if value.has_key('all_lines'):
			window.SetLine('top')
			window.SetLine('bottom')
			window.SetLine('left')
			window.SetLine('right')
			
		if value.has_key('without_background'):
			window.SetNoBackground()
			
		if value.has_key("text"):
			window.SetText(value["text"])

		self.LoadDefaultData(window, value, parentWindow)

		return TRUE
		
	def LoadElementTable(self, window, value, parentWindow):
		if False == self.CheckKeyList(value["name"], value, self.TABLE_KEY_LIST):
			return False

		if value.has_key("height"):
			window.SetSize(value["width"], value["height"])
		else:
			window.SetWidth(value["width"])
		self.LoadDefaultData(window, value, parentWindow)

		if value.has_key("col_length_check"):
			for index in value["col_length_check"]:
				window.AddCheckLengthIndex(index)

		if value.has_key("col_size"):
			window.SetColSizePct(value["col_size"])

			if value.has_key("header"):
				if value.has_key("header_extra"):
					window.SetHeader(value["header"], int(value["header_extra"]))
				else:
					window.SetHeader(value["header"])

			if value.has_key("content"):
				i = 0
				for colList in value["content"]:
					window.Append(i, colList, False)
					i += 1
				window.LocateLines()

		return True
		
	def LoadElementInput(self, window, value, parentWindow):
		if False == self.CheckKeyList(value["name"], value, self.TITLE_BAR_KEY_LIST):
			return False

		window.MakeInput(int(value["width"]))
		self.LoadDefaultData(window, value, parentWindow)

		return True

class ReadingWnd(Bar):

	def __init__(self):
		Bar.__init__(self,"TOP_MOST")

		self.__BuildText()
		self.SetSize(80, 19)
		self.Show()

	def __del__(self):
		Bar.__del__(self)

	def __BuildText(self):
		self.text = TextLine()
		self.text.SetParent(self)
		self.text.SetPosition(4, 3)
		self.text.Show()

	def SetText(self, text):
		self.text.SetText(text)

	def SetReadingPosition(self, x, y):
		xPos = x + 2
		yPos = y  - self.GetHeight() - 2
		self.SetPosition(xPos, yPos)

	def SetTextColor(self, color):
		self.text.SetPackedFontColor(color)


def MakeSlotBar(parent, x, y, width, height):
	slotBar = SlotBar()
	slotBar.SetParent(parent)
	slotBar.SetSize(width, height)
	slotBar.SetPosition(x, y)
	slotBar.Show()
	return slotBar

def MakeImageBox(parent, name, x, y):
	image = ImageBox()
	image.SetParent(parent)
	image.LoadImage(name)
	image.SetPosition(x, y)
	image.Show()
	return image

def MakeText(parent, textlineText, x, y, color):
	textline = TextLine()
	if parent != None:
		textline.SetParent(parent)
	textline.SetPosition(x, y)
	if color != None:
		textline.SetFontColor(color[0], color[1], color[2])
	textline.SetText(textlineText)
	textline.Show()
	return textline

def MakeTextLine(parent, horizontalAlign = True, verticalAlgin = True, x = 0, y = 0):
	textLine = TextLine()
	textLine.SetParent(parent)
	
	if horizontalAlign == True:
		textLine.SetWindowHorizontalAlignCenter()
		
	if verticalAlgin == True:
		textLine.SetWindowVerticalAlignCenter()
		
	textLine.SetHorizontalAlignCenter()
	textLine.SetVerticalAlignCenter()
	
	if x != 0 and y != 0:
		textLine.SetPosition(x, y)
		
	textLine.Show()
	return textLine

def MakeButton(parent, x, y, tooltipText, path, up, over, down):
	button = Button()
	button.SetParent(parent)
	button.SetPosition(x, y)
	button.SetUpVisual(path + up)
	button.SetOverVisual(path + over)
	button.SetDownVisual(path + down)
	button.SetToolTipText(tooltipText)
	button.Show()
	return button

def MakeGauge(parent, x, y, size):
	gauge_make = BattlePassGauge()
	gauge_make.SetParent(parent)
	gauge_make.MakeGauge(size, "bpass")
	gauge_make.SetPosition(x, y)
	gauge_make.Show()
	return gauge_make

def MakeListBox(parent, x, y, width, height, rowCount, event):
	listBox = ListBox2()
	listBox.SetParent(parent)
	listBox.SetPosition(x, y)
	listBox.SetSize(width, height)
	listBox.SetRowCount(rowCount)
	listBox.SetEvent(event)
	listBox.Show()
	return listBox

def RenderRoundBox(x, y, width, height, color):
	grp.SetColor(color)
	grp.RenderLine(x+2, y, width-3, 0)
	grp.RenderLine(x+2, y+height, width-3, 0)
	grp.RenderLine(x, y+2, 0, height-4)
	grp.RenderLine(x+width, y+1, 0, height-3)
	grp.RenderLine(x, y+2, 2, -2)
	grp.RenderLine(x, y+height-2, 2, 2)
	grp.RenderLine(x+width-2, y, 2, 2)
	grp.RenderLine(x+width-2, y+height, 2, -2)

def MakeImageBoxNoImg(parent, x, y):
	image = ImageBox()
	image.SetParent(parent)
	image.SetPosition(x, y)
	image.Show()
	return image

def MakeGridSlot(parent, x, y , vnum, count):
	grid = GridSlotWindow()
	grid.SetParent(parent)
	grid.SetPosition(x, y)
	grid.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
	grid.ArrangeSlot(0, 1, 1, 32, 32, 0, 3)
	grid.SetItemSlot(0, vnum, count)
	grid.RefreshSlot()
	grid.Show()
	return grid

def GenerateColor(r, g, b):
	r = float(r) / 255.0
	g = float(g) / 255.0
	b = float(b) / 255.0
	return grp.GenerateColor(r, g, b, 1.0)

def EnablePaste(flag):
	ime.EnablePaste(flag)

def GetHyperlink():
	return wndMgr.GetHyperlink()


RegisterToolTipWindow("TEXT", TextLine)


