import ui
import grp
import chat
import wndMgr
import net
import app
import ime
import localeInfo
import colorInfo
import constInfo
import systemSetting
import uiScriptLocale
import player

if app.ENABLE_CHATTING_WINDOW_RENEWAL:
	import os
	import uiCommon
	try: import cPickle as pickle
	except (ImportError, TypeError) : import pickle as pickle

if app.ENABLE_PM_ALL_SEND_SYSTEM:
	import uiWhisperAdmin

ENABLE_CHAT_COMMAND = True
ENABLE_LAST_SENTENCE_STACK = True
ENABLE_INSULT_CHECK = True

if localeInfo.IsHONGKONG():
	ENABLE_LAST_SENTENCE_STACK = True

if localeInfo.IsEUROPE():
	ENABLE_CHAT_COMMAND = False

if localeInfo.IsCANADA():
	ENABLE_LAST_SENTENCE_STACK = False

chatInputSetList = []
def InsertChatInputSetWindow(wnd):
	global chatInputSetList
	chatInputSetList.append(wnd)
def RefreshChatMode():
	global chatInputSetList
	map(lambda wnd:wnd.OnRefreshChatMode(), chatInputSetList)
def DestroyChatInputSetWindow():
	global chatInputSetList
	chatInputSetList = []

if app.ENABLE_CHATTING_WINDOW_RENEWAL:
	CHECK_BOX_X_POS = 145

	OPTION_CHECKBOX_TALKING = 1
	OPTION_CHECKBOX_PARTY = 2
	OPTION_CHECKBOX_GUILD = 3
	OPTION_CHECKBOX_SHOUT = 4
	OPTION_CHECKBOX_INFO = 5
	OPTION_CHECKBOX_NOTICE = 6
	OPTION_CHECKBOX_DICE = 7
	OPTION_CHECKBOX_EXP_INFO = 8
	OPTION_CHECKBOX_ITEM_INFO = 9
	OPTION_CHECKBOX_MONEY_INFO = 10

	OPTION_CHECKBOX_MODE = {
		chat.CHAT_TYPE_TALKING : OPTION_CHECKBOX_TALKING,
		chat.CHAT_TYPE_INFO : OPTION_CHECKBOX_INFO,
		chat.CHAT_TYPE_NOTICE : OPTION_CHECKBOX_NOTICE,
		chat.CHAT_TYPE_PARTY : OPTION_CHECKBOX_PARTY,
		chat.CHAT_TYPE_GUILD : OPTION_CHECKBOX_GUILD,
		chat.CHAT_TYPE_SHOUT : OPTION_CHECKBOX_SHOUT,
		chat.CHAT_TYPE_DICE_INFO : OPTION_CHECKBOX_DICE,
		chat.CHAT_TYPE_EXP_INFO : OPTION_CHECKBOX_EXP_INFO,
		chat.CHAT_TYPE_ITEM_INFO : OPTION_CHECKBOX_ITEM_INFO,
		chat.CHAT_TYPE_MONEY_INFO : OPTION_CHECKBOX_MONEY_INFO,
	}

	class ChatSettingWindow(ui.ScriptWindow):
		class MouseReflector(ui.Window):
			def __init__(self, parent):
				ui.Window.__init__(self)
				self.SetParent(parent)
				self.AddFlag("not_pick")
				self.width = self.height = 0
				self.isDown = False

			def __del__(self):
				ui.Window.__del__(self)

			def Down(self):
				self.isDown = True

			def Up(self):
				self.isDown = False

			def OnRender(self):
				if self.isDown:
					grp.SetColor(ui.WHITE_COLOR)
				else:
					grp.SetColor(ui.HALF_WHITE_COLOR)

				x, y = self.GetGlobalPosition()
				grp.RenderBar(x + 2, y + 2, self.GetWidth() - 4, self.GetHeight() - 4)

		class CheckBox(ui.ImageBox):
			def __init__(self, parent, x, y, event, filename = "d:/ymir work/ui/chat/chattingoption_check_box_off.sub"):
				ui.ImageBox.__init__(self)
				self.SetParent(parent)
				self.SetPosition(x, y)
				self.LoadImage(filename)

				self.mouseReflector = parent.MouseReflector(self)
				self.mouseReflector.SetSize(self.GetWidth(), self.GetHeight())

				image = ui.MakeImageBox(self, "d:/ymir work/ui/public/check_image.sub", 0, 0)
				image.AddFlag("not_pick")
				image.SetWindowHorizontalAlignCenter()
				image.SetWindowVerticalAlignCenter()
				image.Hide()

				self.check = False
				self.enable = True
				self.image = image
				self.event = event
				self.Show()

				self.mouseReflector.UpdateRect()

			def __del__(self):
				ui.ImageBox.__del__(self)

			def GetCheck(self):
				return self.check

			def SetCheck(self, flag):
				if flag:
					self.check = True
					self.image.Show()
				else:
					self.check = False
					self.image.Hide()

			def Disable(self):
				self.enable = False

			def OnMouseOverIn(self):
				if not self.enable:
					return
				self.mouseReflector.Show()

			def OnMouseOverOut(self):
				if not self.enable:
					return
				self.mouseReflector.Hide()

			def OnMouseLeftButtonDown(self):
				if not self.enable:
					return
				self.mouseReflector.Down()

			def OnMouseLeftButtonUp(self):
				if not self.enable:
					return
				self.mouseReflector.Up()
				self.event()

		def __init__(self, parent):
			ui.ScriptWindow.__init__(self)
			self.isLoaded = False

			self.parent = parent
			self.questionDialog = None

			self.checkBoxSlotDict = {}
			self.tmpCheckBoxSettingDict = {}

			self.__LoadWindow()

		def __del__(self):
			ui.ScriptWindow.__del__(self)
			self.isLoaded = False
			self.parent = None
			self.questionDialog = None
			self.checkBoxSlotDict = {}
			self.tmpCheckBoxSettingDict = {}

		def __LoadWindow(self):
			if self.isLoaded:
				return

			self.isLoaded = 1

			try:
				pyScrLoader = ui.PythonScriptLoader()
				pyScrLoader.LoadScriptFile(self, "UIScript/ChatSettingWindow.py")
			except:
				import exception
				exception.Abort("ChatSettingWindow.LoadWindow.LoadScript")

			try:
				self.__BindObject()
			except:
				import exception
				exception.Abort("ChatSettingWindow.LoadWindow.BindObject")

			try:
				self.__CreateObject()
			except:
				import exception
				exception.Abort("ChatSettingWindow.LoadWindow.CreateObject")

			try:
				self.__LoadChattingOptionFile()
			except:
				self.__SaveDefault()

		def __BindObject(self):
			self.GetChild("board").SetCloseEvent(ui.__mem_func__(self.Close))

			self.resetBtn = self.GetChild("reset_button")
			self.resetBtn.SetEvent(ui.__mem_func__(self.__OnClickPopUpSetting), localeInfo.CHATTING_SETTING_CLEAR_QUESTION)

			self.saveBtn = self.GetChild("save_button")
			self.saveBtn.SetEvent(ui.__mem_func__(self.__OnClickSave))

			self.cancelBtn = self.GetChild("cancle_button")
			self.cancelBtn.SetEvent(ui.__mem_func__(self.Close))

		def __CreateObject(self):
			for key in xrange(1, len(OPTION_CHECKBOX_MODE) + 1):
				event = lambda index = key : ui.__mem_func__(self.SetCurrentChatOption)(index)

				yPos = 64 + (31 * 0)
				if key >= OPTION_CHECKBOX_DICE:
					yPos = 64 + (31 * 1)
				if key >= OPTION_CHECKBOX_EXP_INFO:
					yPos = 64 + (31 * 2)

				self.checkBoxSlotDict[key] = self.CheckBox(self, CHECK_BOX_X_POS, yPos + (18 * (key - 1)), event)

		def __OnClickSave(self):
			self.__SaveFile()

			if self.parent:
				self.parent.RefreshChatWindow()

			self.Close()

		def __GetChattingFile(self):
			path = ["lib/UserData", "chatting"]
			try:
				if not os.path.exists(os.getcwd() + os.sep + path[0] + os.sep + path[1]):
					os.makedirs(os.getcwd() + os.sep + "lib/UserData" + os.sep + "chatting")
			except WindowsError as error: pass
			return "%s/%s/%s" % (path[0], path[1], player.GetName())

		def __LoadChattingOptionFile(self):
			load = False
			try:
				fileName = self.__GetChattingFile()
				file = open(fileName)
				try:
					load = True
					self.tmpCheckBoxSettingDict = pickle.load(file)
				except (ValueError, EOFError, pickle.PicklingError, pickle.UnpicklingError) : pass
			except IOError: pass

			for key in xrange(1, len(OPTION_CHECKBOX_MODE) + 1):
				if not load:
					value = True
					self.tmpCheckBoxSettingDict[key] = True
				else:
					value = self.tmpCheckBoxSettingDict[key]
				self.checkBoxSlotDict[key].SetCheck(value)

			if not load:
				self.__SaveDefault()

		def __SaveFile(self):
			if not self.tmpCheckBoxSettingDict:
				return

			try:
				fileName = self.__GetChattingFile()
				file = open(fileName, 'wb')
				pickle.dump(self.tmpCheckBoxSettingDict, file)
			except IOError:
				return

		def __SaveDefault(self):
			for key in xrange(1, len(OPTION_CHECKBOX_MODE) + 1):
				self.tmpCheckBoxSettingDict[key] = True

			try:
				fileName = self.__GetChattingFile()
				file = open(fileName, 'wb')
				pickle.dump(self.tmpCheckBoxSettingDict, file)
			except IOError:
				return

		def __OnClickPopUpSetting(self, text):
			questionDialog = uiCommon.QuestionDialog()
			questionDialog.SetText(text)
			questionDialog.SetAcceptEvent(ui.__mem_func__(self.__QuestionPopupAccept))
			questionDialog.SetCancelEvent(ui.__mem_func__(self.__QuestionPopupCancle))
			questionDialog.Open()
			self.questionDialog = questionDialog

		def __QuestionPopupAccept(self):
			if not self.questionDialog:
				return

			self.__SaveDefault()

			if self.parent:
				self.parent.RefreshChatWindow()

			self.__QuestionPopupCancle()
			self.Close()

		def __QuestionPopupCancle(self):
			self.questionDialog.Close()
			self.questionDialog = None

		def SetCurrentChatOption(self, index):
			value = False
			if not self.checkBoxSlotDict[index].GetCheck():
				value = True

			self.checkBoxSlotDict[index].SetCheck(value)
			self.tmpCheckBoxSettingDict.update({index : value})

		def GetChatModeSetting(self, mode):
			try:
				value = OPTION_CHECKBOX_MODE[mode]
				return self.tmpCheckBoxSettingDict[value]
			except KeyError:
				return True

		def OnPressEscapeKey(self):
			self.Close()
			return True

		def Open(self):
			if not self.isLoaded:
				self.__LoadWindow()

			try:
				self.__LoadChattingOptionFile()
			except:
				self.__SaveDefault()

			self.Show()

		def Close(self):
			if self.questionDialog:
				self.questionDialog.Close()

			self.Hide()

## ChatModeButton
class ChatModeButton(ui.Window):

	OUTLINE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 1.0)
	OVER_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 0.3)
	BUTTON_STATE_UP = 0
	BUTTON_STATE_OVER = 1
	BUTTON_STATE_DOWN = 2

	def __init__(self):
		ui.Window.__init__(self)
		self.state = None
		self.buttonText = None
		self.event = None
		self.SetWindowName("ChatModeButton")

		net.EnableChatInsultFilter(ENABLE_INSULT_CHECK)

	def __del__(self):
		ui.Window.__del__(self)

	def SAFE_SetEvent(self, event):
		self.event=ui.__mem_func__(event)

	def SetText(self, text):
		if None == self.buttonText:
			textLine = ui.TextLine()
			textLine.SetParent(self)
			textLine.SetWindowHorizontalAlignCenter()
			textLine.SetWindowVerticalAlignCenter()
			textLine.SetVerticalAlignCenter()
			textLine.SetHorizontalAlignCenter()
			textLine.SetPackedFontColor(self.OUTLINE_COLOR)
			textLine.Show()
			self.buttonText = textLine

		self.buttonText.SetText(text)

	def SetSize(self, width, height):
		self.width = width
		self.height = height
		ui.Window.SetSize(self, width, height)

	def OnMouseOverIn(self):
		self.state = self.BUTTON_STATE_OVER

	def OnMouseOverOut(self):
		self.state = self.BUTTON_STATE_UP

	def OnMouseLeftButtonDown(self):
		self.state = self.BUTTON_STATE_DOWN

	def OnMouseLeftButtonUp(self):
		self.state = self.BUTTON_STATE_UP
		if self.IsIn():
			self.state = self.BUTTON_STATE_OVER

		if None != self.event:
			self.event()

	def OnRender(self):

		(x, y) = self.GetGlobalPosition()

		grp.SetColor(self.OUTLINE_COLOR)
		grp.RenderRoundBox(x, y, self.width, self.height)

		if self.state >= self.BUTTON_STATE_OVER:
			grp.RenderRoundBox(x+1, y, self.width-2, self.height)
			grp.RenderRoundBox(x, y+1, self.width, self.height-2)

			if self.BUTTON_STATE_DOWN == self.state:
				grp.SetColor(self.OVER_COLOR)
				grp.RenderBar(x+1, y+1, self.width-2, self.height-2)

## ChatLine
class ChatLine(ui.EditLine):

	CHAT_MODE_NAME = {	chat.CHAT_TYPE_TALKING : localeInfo.CHAT_NORMAL,
						chat.CHAT_TYPE_PARTY : localeInfo.CHAT_PARTY,
						chat.CHAT_TYPE_GUILD : localeInfo.CHAT_GUILD,
						chat.CHAT_TYPE_SHOUT : localeInfo.CHAT_SHOUT, }

	def __init__(self):
		ui.EditLine.__init__(self)
		self.SetWindowName("Chat Line")
		self.lastShoutTime = 0
		self.eventEscape = lambda *arg: None
		self.eventReturn = lambda *arg: None
		self.eventTab = None
		self.chatMode = chat.CHAT_TYPE_TALKING
		self.bCodePage = True

		self.overTextLine = ui.TextLine()
		self.overTextLine.SetParent(self)
		self.overTextLine.SetPosition(-1, 0)
		self.overTextLine.SetFontColor(1.0, 1.0, 0.0)
		self.overTextLine.SetOutline()
		self.overTextLine.Hide()

		self.lastSentenceStack = []
		self.lastSentencePos = 0

	def SetChatMode(self, mode):
		self.chatMode = mode

	def GetChatMode(self):
		return self.chatMode

	def ChangeChatMode(self):
		if chat.CHAT_TYPE_TALKING == self.GetChatMode():
			self.SetChatMode(chat.CHAT_TYPE_PARTY)
			self.SetText("#")
			self.SetEndPosition()

		elif chat.CHAT_TYPE_PARTY == self.GetChatMode():
			self.SetChatMode(chat.CHAT_TYPE_GUILD)
			self.SetText("%")
			self.SetEndPosition()

		elif chat.CHAT_TYPE_GUILD == self.GetChatMode():
			self.SetChatMode(chat.CHAT_TYPE_SHOUT)
			self.SetText("!")
			self.SetEndPosition()

		elif chat.CHAT_TYPE_SHOUT == self.GetChatMode():
			self.SetChatMode(chat.CHAT_TYPE_TALKING)
			self.SetText("")

		self.__CheckChatMark()

	if app.LINK_IN_CHAT:
		def GetLink(self, text):
			link = ""
			start = text.find("http://")
			if start == -1:
				start = text.find("https://")
			if start == -1:
				return ""

			return text[start:len(text)].split(" ")[0]

	def GetCurrentChatModeName(self):
		try:
			return self.CHAT_MODE_NAME[self.chatMode]
		except:
			import exception
			exception.Abort("ChatLine.GetCurrentChatModeName")

	def SAFE_SetEscapeEvent(self, event):
		self.eventReturn = ui.__mem_func__(event)

	def SAFE_SetReturnEvent(self, event):
		self.eventEscape = ui.__mem_func__(event)

	def SAFE_SetTabEvent(self, event):
		self.eventTab = ui.__mem_func__(event)

	def SetTabEvent(self, event):
		self.eventTab = event

	def OpenChat(self):
		self.SetFocus()
		self.__ResetChat()

	def __ClearChat(self):
		self.SetText("")
		self.lastSentencePos = 0

	def __ResetChat(self):
		if chat.CHAT_TYPE_PARTY == self.GetChatMode():
			self.SetText("#")
			self.SetEndPosition()
		elif chat.CHAT_TYPE_GUILD == self.GetChatMode():
			self.SetText("%")
			self.SetEndPosition()
		elif chat.CHAT_TYPE_SHOUT == self.GetChatMode():
			self.SetText("!")
			self.SetEndPosition()
		else:
			self.__ClearChat()

		self.__CheckChatMark()
		

	def __SendChatPacket(self, text, type):
		if text == "/i_ver1996":        # KONUDA  item_verr efsunu kullan©¥?m©¥? siz ne kulland©¥ysan©¥z onu yaz©¥n
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Bu duzenleme mevcut degil.")
			return

		if text == "/i_efsun_ver1996":  # KONUDA iteme_efsun_ver2 efsunu kullan©¥?m©¥? siz ne kulland©¥ysan©¥z onu yaz©¥n
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Bu duzenleme mevcut degil.")
			return
		if app.ENABLE_PM_ALL_SEND_SYSTEM:
			if str(text) == "/pm_all_send":
				self.whisperadmin = uiWhisperAdmin.AdminWhisperManager()
				self.whisperadmin.OpenWindow()
				return
		if net.IsChatInsultIn(text):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHAT_INSULT_STRING)
		else:
			if app.LINK_IN_CHAT:
				link = self.GetLink(text)
				if link != "":
					import chr
					if not chr.IsGameMaster():
						text = text.replace(link, "|cFF00C0FC|h|Hweb:" + link.replace("://", "XxX") + "|h" + link + "|h|r")
					else:
						text = text.replace(link, "|cFF00C0FC|h|Hsysweb:" + link.replace("://", "XxX") + "|h" + link + "|h|r")
			emoji_list = [":)", ":(", ":O", ":bored:", ":poop:", "B-)", ".l.", ":corona:", ":D", ":P", ":hot:", ";)", ":facepalm:", ":fire:", ":muie:", ":cenzurat:",":goodbye:","<3",":inlove:",":*",":,(",":thinking:",":monkey:",":speriat:", "=))", ":serios:", "=((", ":clown:", ":rip:", ":rusine:", ":nebunie:", ":shh:", ":fantoma:", ":vomit:", ":corona:", ":banana:"]
			emoji_keys = ["|Eemoji/e_happy|e", "|Eemoji/e_sad|e", "|Eemoji/e_surprised|e", "|Eemoji/e_bored|e", "|Eemoji/e_cacat|e", "|Eemoji/e_cocalar|e","|Eemoji/e_cadoufa|e", "|Eemoji/e_coronafrt|e", "|Eemoji/e_d|e", "|Eemoji/e_p|e","|Eemoji/e_estihotvtm|e","|Eemoji/e_faccuochiufrt|e", "|Eemoji/e_facepalm|e","|Eemoji/e_fire|e","|Eemoji/e_fuckyou|e","|Eemoji/e_fututirasamatii|e","|Eemoji/e_haipa|e","|Eemoji/e_inima|e", "|Eemoji/e_inlove|e", "|Eemoji/e_kissyou|e","|Eemoji/e_lakrima|e","|Eemoji/e_magandesclamata|e","|Eemoji/e_maimuta|e","|Eemoji/e_maisperiat|e","|Eemoji/e_mapispatn|e","|Eemoji/nicimatanutesuporta|e","|Eemoji/e_pling|e","|Eemoji/e_pozacuatr|e","|Eemoji/e_rip|e","|Eemoji/e_rusine|e","|Eemoji/e_santnebundupatnfa|e","|Eemoji/e_shh|e", "|Eemoji/e_svfantomacaspai2|e", "|Eemoji/e_vomitpatn|e", "|Eemoji/e_anticorona|e", "|Eemoji/e_banana|e"]
			i = 0
			while i >= 0 and i <= 35:
				if emoji_list[i] in text:
					newtext = text.replace(emoji_list[i], emoji_keys[i])
					text = newtext
				i = i+1
		if app.ENABLE_CHAT_COLOR_SYSTEM:
			if text[0] != "/":
				text = text + " "
				if len(constInfo.chat_color) > 0:
					text = constInfo.chat_color + text
					text = text.replace("|h|r","|h|r %s" % constInfo.chat_color)
			net.SendChatPacket(text, type)
	
		
	def __SendPartyChatPacket(self, text):

		if 1 == len(text):
			self.RunCloseEvent()
			return

		self.__SendChatPacket(text[1:], chat.CHAT_TYPE_PARTY)
		self.__ResetChat()

	def __SendGuildChatPacket(self, text):

		if 1 == len(text):
			self.RunCloseEvent()
			return

		self.__SendChatPacket(text[1:], chat.CHAT_TYPE_GUILD)
		self.__ResetChat()

	def __SendShoutChatPacket(self, text):

		if 1 == len(text):
			self.RunCloseEvent()
			return

		if app.GetTime() < self.lastShoutTime + 5:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHAT_SHOUT_LIMIT)
			self.__ResetChat()
			return
		chInfo = net.GetServerInfo()
		myChannel = chInfo[-3:]


		self.__SendChatPacket(text[1:], chat.CHAT_TYPE_SHOUT)
		self.__ResetChat()

		self.lastShoutTime = app.GetTime()

	def __SendTalkingChatPacket(self, text):
		self.__SendChatPacket(text, chat.CHAT_TYPE_TALKING)
		self.__ResetChat()

	def OnIMETab(self):
		#if None != self.eventTab:
		#	self.eventTab()
		#return True
		return False

	def OnIMEUpdate(self):
		ui.EditLine.OnIMEUpdate(self)
		self.__CheckChatMark()

	def __CheckChatMark(self):

		self.overTextLine.Hide()

		text = self.GetText()
		if len(text) > 0:
			if '#' == text[0]:
				self.overTextLine.SetText("#")
				self.overTextLine.Show()
			elif '%' == text[0]:
				self.overTextLine.SetText("%")
				self.overTextLine.Show()
			elif '!' == text[0]:
				self.overTextLine.SetText("!")
				self.overTextLine.Show()

	def OnIMEKeyDown(self, key):
		# LAST_SENTENCE_STACK
		if app.VK_UP == key:
			self.__PrevLastSentenceStack()
			return True

		if app.VK_DOWN == key:
			self.__NextLastSentenceStack()				
			return True			
		# END_OF_LAST_SENTENCE_STACK

		ui.EditLine.OnIMEKeyDown(self, key)

	# LAST_SENTENCE_STACK
	def __PrevLastSentenceStack(self):
		global ENABLE_LAST_SENTENCE_STACK
		if not ENABLE_LAST_SENTENCE_STACK:
			return

		if self.lastSentenceStack and self.lastSentencePos < len(self.lastSentenceStack):
			self.lastSentencePos += 1
			lastSentence = self.lastSentenceStack[-self.lastSentencePos]
			self.SetText(lastSentence)				
			self.SetEndPosition()			

	def __NextLastSentenceStack(self):
		global ENABLE_LAST_SENTENCE_STACK
		if not ENABLE_LAST_SENTENCE_STACK:
			return

		if self.lastSentenceStack and self.lastSentencePos > 1:
			self.lastSentencePos -= 1
			lastSentence = self.lastSentenceStack[-self.lastSentencePos]
			self.SetText(lastSentence)				
			self.SetEndPosition()			

	def __PushLastSentenceStack(self, text):		
		global ENABLE_LAST_SENTENCE_STACK
		if not ENABLE_LAST_SENTENCE_STACK:
			return

		if len(text) <= 0:
			return
			
		LAST_SENTENCE_STACK_SIZE = 32
		if len(self.lastSentenceStack) > LAST_SENTENCE_STACK_SIZE:
			self.lastSentenceStack.pop(0)

		self.lastSentenceStack.append(text)
	# END_OF_LAST_SENTENCE_STACK

	def OnIMEReturn(self):
		text = self.GetText()
		textLen=len(text)

		# LAST_SENTENCE_STACK
		self.__PushLastSentenceStack(text)
		# END_OF_LAST_SENTENCE_STACK
				
		textSpaceCount=text.count(' ')

		if (textLen > 0) and (textLen != textSpaceCount):
			if '#' == text[0]:
				self.__SendPartyChatPacket(text)
			elif '%' == text[0]:
				self.__SendGuildChatPacket(text)
			elif '!' == text[0]:
				self.__SendShoutChatPacket(text)
			else:
				self.__SendTalkingChatPacket(text)
		else:
			self.__ClearChat()
			self.eventReturn()

		return True

	def OnPressEscapeKey(self):
		self.__ClearChat()
		self.eventEscape()
		return True

	def RunCloseEvent(self):
		self.eventEscape()

	def BindInterface(self, interface):
		self.interface = interface

	def OnMouseLeftButtonDown(self):
		hyperlink = ui.GetHyperlink()
		if hyperlink:
			if app.IsPressed(app.DIK_LALT):
				link = chat.GetLinkFromHyperlink(hyperlink)
				ime.PasteString(link)
			else:
				self.interface.MakeHyperlinkTooltip(hyperlink)
		else:
			ui.EditLine.OnMouseLeftButtonDown(self)

class ChatInputSet(ui.Window):

	CHAT_OUTLINE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 1.0)

	def __init__(self):
		ui.Window.__init__(self)
		self.SetWindowName("ChatInputSet")

		InsertChatInputSetWindow(self)
		self.__Create()

	def __del__(self):
		ui.Window.__del__(self)

	def __Create(self):
		chatModeButton = ChatModeButton()
		chatModeButton.SetParent(self)
		chatModeButton.SetSize(40, 17)
		chatModeButton.SetText(localeInfo.CHAT_NORMAL)
		chatModeButton.SetPosition(7, 2)
		chatModeButton.SAFE_SetEvent(self.OnChangeChatMode)
		self.chatModeButton = chatModeButton

		chatLine = ChatLine()
		chatLine.SetParent(self)
		chatLine.SetMax(512)
		chatLine.SetUserMax(76)
		chatLine.SetText("")
		chatLine.SAFE_SetTabEvent(self.OnChangeChatMode)
		chatLine.x = 0
		chatLine.y = 0
		chatLine.width = 0
		chatLine.height = 0
		self.chatLine = chatLine

		btnSend = ui.Button()
		btnSend.SetParent(self)
		btnSend.SetUpVisual("d:/ymir work/ui/game/taskbar/Send_Chat_Button_01.sub")
		btnSend.SetOverVisual("d:/ymir work/ui/game/taskbar/Send_Chat_Button_02.sub")
		btnSend.SetDownVisual("d:/ymir work/ui/game/taskbar/Send_Chat_Button_03.sub")
		btnSend.SetToolTipText(localeInfo.CHAT_SEND_CHAT)
		btnSend.SAFE_SetEvent(self.chatLine.OnIMEReturn)
		self.btnSend = btnSend

	def Destroy(self):
		self.chatModeButton = None
		self.chatLine = None
		self.btnSend = None

	def Open(self):
		self.chatLine.Show()
		self.chatLine.SetPosition(57, 5)
		self.chatLine.SetFocus()
		self.chatLine.OpenChat()

		self.chatModeButton.SetPosition(7, 2)
		self.chatModeButton.Show()

		self.btnSend.Show()
		self.Show()

		self.RefreshPosition()
		return True

	def Close(self):
		self.chatLine.KillFocus()
		self.chatLine.Hide()
		self.chatModeButton.Hide()
		self.btnSend.Hide()
		self.Hide()
		return True

	def SetEscapeEvent(self, event):
		self.chatLine.SetEscapeEvent(event)

	def SetReturnEvent(self, event):
		self.chatLine.SetReturnEvent(event)

	def OnChangeChatMode(self):
		RefreshChatMode()

	def OnRefreshChatMode(self):
		self.chatLine.ChangeChatMode()
		self.chatModeButton.SetText(self.chatLine.GetCurrentChatModeName())

	def SetChatFocus(self):
		self.chatLine.SetFocus()

	def KillChatFocus(self):
		self.chatLine.KillFocus()

	def SetChatMax(self, max):
		self.chatLine.SetUserMax(max)

	def RefreshPosition(self):
		if localeInfo.IsARABIC():
			self.chatLine.SetSize(self.GetWidth() - 93, 18)
		else:
			self.chatLine.SetSize(self.GetWidth() - 93, 13)

		self.btnSend.SetPosition(self.GetWidth() - 25, 2)

		(self.chatLine.x, self.chatLine.y, self.chatLine.width, self.chatLine.height) = self.chatLine.GetRect()

	def BindInterface(self, interface):
		self.chatLine.BindInterface(interface)

	def OnRender(self):
		(x, y, width, height) = self.chatLine.GetRect()
		ui.RenderRoundBox(x-4, y-3, width+7, height+4, self.CHAT_OUTLINE_COLOR)

## ChatWindow
class ChatWindow(ui.Window):

	BOARD_START_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.0)
	BOARD_END_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.8)
	BOARD_MIDDLE_COLOR = grp.GenerateColor(0.0, 0.0, 0.0, 0.5)
	CHAT_OUTLINE_COLOR = grp.GenerateColor(1.0, 1.0, 1.0, 1.0)

	EDIT_LINE_HEIGHT = 25
	if app.ENABLE_CHATTING_WINDOW_RENEWAL:
		EDIT_LINE_HIDE_HEIGHT = 20

	if app.ENABLE_CHAT_COLOR_SYSTEM:
		CHAT_WINDOW_WIDTH = 600 + 25
	else:
		CHAT_WINDOW_WIDTH = 600

	class ChatBackBoard(ui.Window):
		def __init__(self):
			ui.Window.__init__(self)
		def __del__(self):
			ui.Window.__del__(self)

	class ChatButton(ui.DragButton):

		def __init__(self):
			ui.DragButton.__init__(self)
			self.AddFlag("float")
			self.AddFlag("movable")
			self.AddFlag("restrict_x")
			self.topFlag = False
			self.SetWindowName("ChatWindow:ChatButton")
		

		def __del__(self):
			ui.DragButton.__del__(self)

		def SetOwner(self, owner):
			self.owner = owner

		def OnMouseOverIn(self):
			app.SetCursor(app.VSIZE)

		def OnMouseOverOut(self):
			app.SetCursor(app.NORMAL)

		def OnTop(self):
			if True == self.topFlag:
				return

			self.topFlag = True
			self.owner.SetTop()
			self.topFlag = False

	def __init__(self):
		ui.Window.__init__(self)
		self.AddFlag("float")

		self.SetWindowName("ChatWindow")
		self.__RegisterChatColorDict()

		self.boardState = chat.BOARD_STATE_VIEW
		self.chatID = chat.CreateChatSet(chat.CHAT_SET_CHAT_WINDOW)
		chat.SetBoardState(self.chatID, chat.BOARD_STATE_VIEW)

		self.xBar = 0
		self.yBar = 0
		self.widthBar = 0
		self.heightBar = 0
		self.curHeightBar = 0
		self.visibleLineCount = 0
		self.scrollBarPos = 1.0
		self.scrollLock = False

		chatInputSet = ChatInputSet()
		chatInputSet.SetParent(self)
		chatInputSet.SetEscapeEvent(ui.__mem_func__(self.CloseChat))
		chatInputSet.SetReturnEvent(ui.__mem_func__(self.CloseChat))
		chatInputSet.SetSize(550, 25)
		self.chatInputSet = chatInputSet

		
		
		if app.ENABLE_CHAT_COLOR_SYSTEM:
			self.open_colors = ui.BoardWithTitleBar()
			self.open_colors.SetCloseEvent(ui.__mem_func__(self.ColorsClose))
			self.open_colors.SetTitleName(localeInfo.SKILL_COLOR_SELECT_PRESET)
			self.open_colors.SetSize(117*2,159)
			self.open_colors.SetTop()
			self.open_colors.Hide()
			
			self.reset_colors = ui.Button()
			self.reset_colors.SetParent(self.open_colors)
			self.reset_colors.SetPosition(22, 8)
			self.reset_colors.SetUpVisual("d:/ymir work/ui/pattern/q_mark_01.tga")
			self.reset_colors.SetOverVisual("d:/ymir work/ui/pattern/q_mark_02.tga")
			self.reset_colors.SetDownVisual("d:/ymir work/ui/pattern/q_mark_01.tga")
			self.reset_colors.SetToolTipText(uiScriptLocale.SKILL_COLOR_DEFAULT)
			self.reset_colors.SAFE_SetEvent(self.ResetColors,1)
			self.reset_colors.Show()
			
			self.colors_line_down = 43
			self.colors_to_right = 20
			self.colors_to_right_rand_2 = 125
			
			self.colors_fundal = ui.MakeThinBoard(self.open_colors,10,31, self.open_colors.GetWidth()-19, self.open_colors.GetHeight()-40)
			self.colors_fundal.Show()
			
			self.btn_colors_1 = ui.Button()
			self.btn_colors_1.SAFE_SetEvent(self.SetColorInChat,1)
			self.btn_colors_1.SetParent(self.open_colors)
			self.btn_colors_1.SetPosition(self.colors_to_right, self.colors_line_down)
			self.btn_colors_1.SetUpVisual("d:/ymir work/ui/colors/chat_red.tga")
			self.btn_colors_1.SetOverVisual("d:/ymir work/ui/colors/chat_red.tga")
			self.btn_colors_1.SetDownVisual("d:/ymir work/ui/colors/chat_red.tga")
			self.btn_colors_1.Show()
			
			self.btn_colors_2 = ui.Button()
			self.btn_colors_2.SAFE_SetEvent(self.SetColorInChat,2)
			self.btn_colors_2.SetParent(self.open_colors)
			self.btn_colors_2.SetPosition(self.colors_to_right, self.colors_line_down + 22)
			self.btn_colors_2.SetUpVisual("d:/ymir work/ui/colors/chat_blue.tga")
			self.btn_colors_2.SetOverVisual("d:/ymir work/ui/colors/chat_blue.tga")
			self.btn_colors_2.SetDownVisual("d:/ymir work/ui/colors/chat_blue.tga")
			self.btn_colors_2.Show()
			
			self.btn_colors_3 = ui.Button()
			self.btn_colors_3.SAFE_SetEvent(self.SetColorInChat,3)
			self.btn_colors_3.SetParent(self.open_colors)
			self.btn_colors_3.SetPosition(self.colors_to_right, self.colors_line_down + 22*2)
			self.btn_colors_3.SetUpVisual("d:/ymir work/ui/colors/chat_green.tga")
			self.btn_colors_3.SetOverVisual("d:/ymir work/ui/colors/chat_green.tga")
			self.btn_colors_3.SetDownVisual("d:/ymir work/ui/colors/chat_green.tga")
			self.btn_colors_3.Show()
			
			self.btn_colors_4 = ui.Button()
			self.btn_colors_4.SAFE_SetEvent(self.SetColorInChat,4)
			self.btn_colors_4.SetParent(self.open_colors)
			self.btn_colors_4.SetPosition(self.colors_to_right, self.colors_line_down + 22*3)
			self.btn_colors_4.SetUpVisual("d:/ymir work/ui/colors/chat_yellow.tga")
			self.btn_colors_4.SetOverVisual("d:/ymir work/ui/colors/chat_yellow.tga")
			self.btn_colors_4.SetDownVisual("d:/ymir work/ui/colors/chat_yellow.tga")
			self.btn_colors_4.Show()
			
			self.btn_colors_5 = ui.Button()
			self.btn_colors_5.SAFE_SetEvent(self.SetColorInChat,5)
			self.btn_colors_5.SetParent(self.open_colors)
			self.btn_colors_5.SetPosition(self.colors_to_right_rand_2, self.colors_line_down)
			self.btn_colors_5.SetUpVisual("d:/ymir work/ui/colors/chat_roz_inchis.tga")
			self.btn_colors_5.SetOverVisual("d:/ymir work/ui/colors/chat_roz_inchis.tga")
			self.btn_colors_5.SetDownVisual("d:/ymir work/ui/colors/chat_roz_inchis.tga")
			self.btn_colors_5.Show()
			
			self.btn_colors_6 = ui.Button()
			self.btn_colors_6.SAFE_SetEvent(self.SetColorInChat,6)
			self.btn_colors_6.SetParent(self.open_colors)
			self.btn_colors_6.SetPosition(self.colors_to_right_rand_2, self.colors_line_down + 22)
			self.btn_colors_6.SetUpVisual("d:/ymir work/ui/colors/chat_portocaliu.tga")
			self.btn_colors_6.SetOverVisual("d:/ymir work/ui/colors/chat_portocaliu.tga")
			self.btn_colors_6.SetDownVisual("d:/ymir work/ui/colors/chat_portocaliu.tga")
			self.btn_colors_6.Show()
			
			self.btn_colors_7 = ui.Button()
			self.btn_colors_7.SAFE_SetEvent(self.SetColorInChat,7)
			self.btn_colors_7.SetParent(self.open_colors)
			self.btn_colors_7.SetPosition(self.colors_to_right_rand_2, self.colors_line_down + 22*2)
			self.btn_colors_7.SetUpVisual("d:/ymir work/ui/colors/chat_maro.tga")
			self.btn_colors_7.SetOverVisual("d:/ymir work/ui/colors/chat_maro.tga")
			self.btn_colors_7.SetDownVisual("d:/ymir work/ui/colors/chat_maro.tga")
			self.btn_colors_7.Show()
			
			self.btn_colors_8 = ui.Button()
			self.btn_colors_8.SAFE_SetEvent(self.SetColorInChat,8)
			self.btn_colors_8.SetParent(self.open_colors)
			self.btn_colors_8.SetPosition(self.colors_to_right_rand_2, self.colors_line_down + 22*3)
			self.btn_colors_8.SetUpVisual("d:/ymir work/ui/colors/chat_mov.tga")
			self.btn_colors_8.SetOverVisual("d:/ymir work/ui/colors/chat_mov.tga")
			self.btn_colors_8.SetDownVisual("d:/ymir work/ui/colors/chat_mov.tga")
			self.btn_colors_8.Show()
		
		
		
		btnSendWhisper = ui.Button()
		btnSendWhisper.SetParent(self)
		btnSendWhisper.SetUpVisual("d:/ymir work/ui/game/taskbar/Send_Whisper_Button_01.sub")
		btnSendWhisper.SetOverVisual("d:/ymir work/ui/game/taskbar/Send_Whisper_Button_02.sub")
		btnSendWhisper.SetDownVisual("d:/ymir work/ui/game/taskbar/Send_Whisper_Button_03.sub")
		btnSendWhisper.SetToolTipText(localeInfo.CHAT_SEND_MEMO)
		btnSendWhisper.Hide()
		self.btnSendWhisper = btnSendWhisper

		btnChatLog = ui.Button()
		btnChatLog.SetParent(self)
		btnChatLog.SetUpVisual("d:/ymir work/ui/game/taskbar/Open_Chat_Log_Button_01.sub")
		btnChatLog.SetOverVisual("d:/ymir work/ui/game/taskbar/Open_Chat_Log_Button_02.sub")
		btnChatLog.SetDownVisual("d:/ymir work/ui/game/taskbar/Open_Chat_Log_Button_03.sub")
		btnChatLog.SetToolTipText(localeInfo.CHAT_LOG)
		btnChatLog.Hide()
		self.btnChatLog = btnChatLog

		btnChatSizing = self.ChatButton()
		btnChatSizing.SetOwner(self)
		btnChatSizing.SetMoveEvent(ui.__mem_func__(self.Refresh))
		btnChatSizing.Hide()
		self.btnChatSizing = btnChatSizing

		## colors
		if app.ENABLE_CHAT_COLOR_SYSTEM:
			self.btnSendColors = ui.Button()
			self.btnSendColors.SetParent(self)
			self.btnSendColors.SetUpVisual("d:/ymir work/ui/colors/iconita.tga")
			self.btnSendColors.SetOverVisual("d:/ymir work/ui/colors/iconita.tga")
			self.btnSendColors.SetDownVisual("d:/ymir work/ui/colors/iconita.tga")
			self.btnSendColors.SetToolTipText(localeInfo.SKILL_COLOR_SELECT_PRESET)
			self.btnSendColors.SAFE_SetEvent(self.OpenColorsPage)
			self.btnSendColors.Hide()

		if app.ENABLE_CHATTING_WINDOW_RENEWAL:
			imgChatBarLeft = ui.ImageBox()
			imgChatBarLeft.SetParent(self.btnChatSizing)
			imgChatBarLeft.AddFlag("not_pick")
			imgChatBarLeft.LoadImage("d:/ymir work/ui/chat/chat_linebar_left.tga")
			imgChatBarLeft.Show()
			self.imgChatBarLeft = imgChatBarLeft

			imgChatBarRight = ui.ImageBox()
			imgChatBarRight.SetParent(self.btnChatSizing)
			imgChatBarRight.AddFlag("not_pick")
			imgChatBarRight.LoadImage("d:/ymir work/ui/chat/chat_linebar_right.tga")
			imgChatBarRight.Show()
			self.imgChatBarRight = imgChatBarRight

			imgChatBarMiddle = ui.ExpandedImageBox()
			imgChatBarMiddle.SetParent(self.btnChatSizing)
			imgChatBarMiddle.AddFlag("not_pick")
			imgChatBarMiddle.LoadImage("d:/ymir work/ui/chat/chatmenutab_line.tga")
			imgChatBarMiddle.Show()
			self.imgChatBarMiddle = imgChatBarMiddle

			btnChatTab = ui.Button()
			btnChatTab.SetParent(self.btnChatSizing)
			btnChatTab.SetUpVisual("d:/ymir work/ui/chat/chatmenutab_down.tga")
			btnChatTab.SetOverVisual("d:/ymir work/ui/chat/chatmenutab_down.tga")
			btnChatTab.SetDownVisual("d:/ymir work/ui/chat/chatmenutab_down.tga")
			btnChatTab.SetToolTipText(uiScriptLocale.CHATTING_SETTING_TALKING, 0, -23)
			btnChatTab.Show()
			btnChatTab.Down()
			self.btnChatTab = btnChatTab

			btnChatSettingOption = ui.Button()
			btnChatSettingOption.SetParent(self.btnChatSizing)
			btnChatSettingOption.SetUpVisual("d:/ymir work/ui/chat/btn_option01_default.tga")
			btnChatSettingOption.SetOverVisual("d:/ymir work/ui/chat/btn_option01_over.tga")
			btnChatSettingOption.SetDownVisual("d:/ymir work/ui/chat/btn_option01_down.tga")
			btnChatSettingOption.SetToolTipText(localeInfo.CHATTING_SETTING_SETTING, 0, -23)
			btnChatSettingOption.SetEvent(ui.__mem_func__(self.__SettingOptionWndOpen))
			btnChatSettingOption.Show()
			self.btnChatSettingOption = btnChatSettingOption

			self.wndChatSettingOption = ChatSettingWindow(self)
		else:
			imgChatBarLeft = ui.ImageBox()
			imgChatBarLeft.SetParent(self.btnChatSizing)
			imgChatBarLeft.AddFlag("not_pick")
			imgChatBarLeft.LoadImage("d:/ymir work/ui/pattern/chat_bar_left.tga")
			imgChatBarLeft.Show()
			self.imgChatBarLeft = imgChatBarLeft

			imgChatBarRight = ui.ImageBox()
			imgChatBarRight.SetParent(self.btnChatSizing)
			imgChatBarRight.AddFlag("not_pick")
			imgChatBarRight.LoadImage("d:/ymir work/ui/pattern/chat_bar_right.tga")
			imgChatBarRight.Show()
			self.imgChatBarRight = imgChatBarRight

			imgChatBarMiddle = ui.ExpandedImageBox()
			imgChatBarMiddle.SetParent(self.btnChatSizing)
			imgChatBarMiddle.AddFlag("not_pick")
			imgChatBarMiddle.LoadImage("d:/ymir work/ui/pattern/chat_bar_middle.tga")
			imgChatBarMiddle.Show()
			self.imgChatBarMiddle = imgChatBarMiddle

		scrollBar = ui.ScrollBar()
		scrollBar.AddFlag("float")
		scrollBar.SetScrollEvent(ui.__mem_func__(self.OnScroll))
		self.scrollBar = scrollBar

		self.Refresh()
		if app.ENABLE_CHATTING_WINDOW_RENEWAL:
			self.RefreshChatWindow()
		self.chatInputSet.RefreshPosition()

	def __del__(self):
		ui.Window.__del__(self)

	def __RegisterChatColorDict(self):
		CHAT_COLOR_DICT = {
			chat.CHAT_TYPE_TALKING : colorInfo.CHAT_RGB_TALK,
			chat.CHAT_TYPE_INFO : colorInfo.CHAT_RGB_INFO,
			chat.CHAT_TYPE_NOTICE : colorInfo.CHAT_RGB_NOTICE,
			chat.CHAT_TYPE_PARTY : colorInfo.CHAT_RGB_PARTY,
			chat.CHAT_TYPE_GUILD : colorInfo.CHAT_RGB_GUILD,
			chat.CHAT_TYPE_COMMAND : colorInfo.CHAT_RGB_COMMAND,
			chat.CHAT_TYPE_SHOUT : colorInfo.CHAT_RGB_SHOUT,
			chat.CHAT_TYPE_WHISPER : colorInfo.CHAT_RGB_WHISPER,
			chat.CHAT_TYPE_BIG_NOTICE : colorInfo.CHAT_RGB_NOTICE,
			chat.CHAT_TYPE_DICE_INFO : colorInfo.CHAT_RGB_INFO,
		}

		if app.ENABLE_CHATTING_WINDOW_RENEWAL:
			CHAT_COLOR_DICT[chat.CHAT_TYPE_EXP_INFO] = colorInfo.CHAT_RGB_INFO
			CHAT_COLOR_DICT[chat.CHAT_TYPE_ITEM_INFO] = colorInfo.CHAT_RGB_INFO
			CHAT_COLOR_DICT[chat.CHAT_TYPE_MONEY_INFO] = colorInfo.CHAT_RGB_INFO

		for colorItem in CHAT_COLOR_DICT.items():
			type = colorItem[0]
			rgb = colorItem[1]
			chat.SetChatColor(type, rgb[0], rgb[1], rgb[2])

	def Destroy(self):
		self.chatInputSet.Destroy()
		self.chatInputSet = None

		self.btnSendWhisper = 0
		self.btnChatLog = 0
		self.btnChatSizing = 0

		if app.ENABLE_CHATTING_WINDOW_RENEWAL:
			if self.wndChatSettingOption:
				self.wndChatSettingOption.Close()
				self.wndChatSettingOption = None

		## colors
		if app.ENABLE_CHAT_COLOR_SYSTEM:
			self.btnSendColors = 0
			self.open_colors.Hide()

	## colors
	if app.ENABLE_CHAT_COLOR_SYSTEM:
		def OpenColorsPage(self):
			if not self.open_colors:
				return
				
			if not self.open_colors.IsShow():
				if wndMgr.GetScreenWidth() < 1024:
					self.open_colors.AddFlag('movable')
					self.open_colors.SetPosition(wndMgr.GetScreenWidth() / 2 + 130, wndMgr.GetScreenHeight()-380)
				elif wndMgr.GetScreenWidth() == 1024:
					self.open_colors.AddFlag('movable')
					self.open_colors.SetPosition(wndMgr.GetScreenWidth() / 2 + 200, wndMgr.GetScreenHeight()-380)	
				else:
					self.open_colors.SetPosition(wndMgr.GetScreenWidth() / 2 + 330, wndMgr.GetScreenHeight()-177)
				self.open_colors.Show()
				constInfo.chat_color_page_open = 1
			else:
				self.open_colors.Hide()
				constInfo.chat_color_page_open = 0
				
		def ResetColors(self,arg):
			if not self.open_colors:
				return
			if constInfo.chat_color_page_open == 0:
				return False
				
			if arg == 1:
				constInfo.chat_color = ""
				
		def ColorsClose(self):
			if not self.open_colors:
				return
			if self.open_colors.IsShow():
				self.open_colors.Hide()
				constInfo.chat_color_page_open = 0
		
		def SetColorInChat(self,arg):
			if not self.open_colors:
				return
			if constInfo.chat_color_page_open == 0:
				return False
			if arg == 1:
				constInfo.chat_color = "|cFFff1a1a|H|h"
			if arg == 2:
				constInfo.chat_color = "|cFF00ffe4|H|h"
			if arg == 3:
				constInfo.chat_color = "|cFF33ff33|H|h"
			if arg == 4:
				constInfo.chat_color = "|cFFFFF200|H|h"
			if arg == 5:
				constInfo.chat_color = "|cFFFF62FF|H|h"
			if arg == 6:
				constInfo.chat_color = "|cFFFF7F27|H|h"
			if arg == 7:
				constInfo.chat_color = "|cFFff9966|H|h"
			if arg == 8:
				constInfo.chat_color = "|cFFbf80ff|H|h"
		

	################
	## Open & Close
	def OpenChat(self):
		self.SetSize(self.CHAT_WINDOW_WIDTH, 25)
		chat.SetBoardState(self.chatID, chat.BOARD_STATE_EDIT)
		self.boardState = chat.BOARD_STATE_EDIT

		(x, y, width, height) = self.GetRect()
		(btnX, btnY) = self.btnChatSizing.GetGlobalPosition()

		if localeInfo.IsARABIC():
			chat.SetPosition(self.chatID, x + width - 10, y)
		else:	
			chat.SetPosition(self.chatID, x + 10, y)

		chat.SetHeight(self.chatID, y - btnY - self.EDIT_LINE_HEIGHT + 100)

		if self.IsShow():
			self.btnChatSizing.Show()

		self.Refresh()
		if app.ENABLE_CHATTING_WINDOW_RENEWAL:
			self.RefreshChatWindow()

		self.btnSendWhisper.SetPosition(self.GetWidth() - 50, 2)
		self.btnSendWhisper.Show()

		if app.ENABLE_CHAT_COLOR_SYSTEM:
			self.btnChatLog.SetPosition(self.GetWidth() - 75, 2)
		else:
			self.btnChatLog.SetPosition(self.GetWidth() - 25, 2)
		self.btnChatLog.Show()

		## colors
		if app.ENABLE_CHAT_COLOR_SYSTEM:
			self.btnSendColors.SetPosition(self.GetWidth() - 25, 1)
			self.btnSendColors.Show()

		self.chatInputSet.Open()
		self.chatInputSet.SetTop()
		self.SetTop()

	def CloseChat(self):
		chat.SetBoardState(self.chatID, chat.BOARD_STATE_VIEW)
		self.boardState = chat.BOARD_STATE_VIEW

		(x, y, width, height) = self.GetRect()

		if localeInfo.IsARABIC():
			chat.SetPosition(self.chatID, x + width - 10, y + self.EDIT_LINE_HEIGHT)
		else:
			chat.SetPosition(self.chatID, x + 10, y + self.EDIT_LINE_HEIGHT)

		self.SetSize(self.CHAT_WINDOW_WIDTH, 0)

		self.chatInputSet.Close()
		self.btnSendWhisper.Hide()
		self.btnChatLog.Hide()
		self.btnChatSizing.Hide()
		## colors
		if app.ENABLE_CHAT_COLOR_SYSTEM:
			self.btnSendColors.Hide()

		self.Refresh()
		if app.ENABLE_CHATTING_WINDOW_RENEWAL:
			self.RefreshChatWindow()

	def SetSendWhisperEvent(self, event):
		self.btnSendWhisper.SetEvent(event)

	def SetOpenChatLogEvent(self, event):
		self.btnChatLog.SetEvent(event)

	def IsEditMode(self):
		if chat.BOARD_STATE_EDIT == self.boardState:
			return True

		return False

	def __RefreshSizingBar(self):
		(x, y, width, height) = self.GetRect()
		gxChat, gyChat = self.btnChatSizing.GetGlobalPosition()
		self.btnChatSizing.SetPosition(x, gyChat)
		self.btnChatSizing.SetSize(width, 22)
		if app.ENABLE_CHATTING_WINDOW_RENEWAL:
			self.imgChatBarLeft.SetPosition(0, 17)
			self.imgChatBarRight.SetPosition(width - 57, 0)

			self.btnChatTab.SetTextAddPos(uiScriptLocale.CHATTING_SETTING_DEFAULT_TITLE, -2)
			self.btnChatTab.SetPosition(4, 0)
			self.btnChatSettingOption.SetPosition(width - 27, 3)

			self.imgChatBarMiddle.SetPosition(57.0, 0)
			self.imgChatBarMiddle.SetRenderingRect(0.0, 0.0, float(width - 57.0 * 2) / 57.0 - 1.0, 0.0)
		else:
			self.imgChatBarLeft.SetPosition(0, 0)
			self.imgChatBarRight.SetPosition(width - 64, 0)
			self.imgChatBarMiddle.SetPosition(64, 0)
			self.imgChatBarMiddle.SetRenderingRect(0.0, 0.0, float(width - 128) / 64.0 - 1.0, 0.0)

	def SetPosition(self, x, y):
		ui.Window.SetPosition(self, x, y)
		self.__RefreshSizingBar()

	def SetSize(self, width, height):
		ui.Window.SetSize(self, width, height)
		self.__RefreshSizingBar()

	def SetHeight(self, height):
		gxChat, gyChat = self.btnChatSizing.GetGlobalPosition()
		self.btnChatSizing.SetPosition(gxChat, wndMgr.GetScreenHeight() - height)

	###########
	## Refresh
	def Refresh(self):
		if self.boardState == chat.BOARD_STATE_EDIT:
			self.RefreshBoardEditState()
		elif self.boardState == chat.BOARD_STATE_VIEW:
			self.RefreshBoardViewState()

	def RefreshBoardEditState(self):

		(x, y, width, height) = self.GetRect()
		(btnX, btnY) = self.btnChatSizing.GetGlobalPosition()

		self.xBar = x
		self.yBar = btnY
		self.widthBar = width
		self.heightBar = y - btnY + self.EDIT_LINE_HEIGHT
		self.curHeightBar = self.heightBar

		if localeInfo.IsARABIC():
			chat.SetPosition(self.chatID, x + width - 10, y)
		else:
			chat.SetPosition(self.chatID, x + 10, y)

		chat.SetHeight(self.chatID, y - btnY - self.EDIT_LINE_HEIGHT)
		chat.ArrangeShowingChat(self.chatID)

		if btnY > y:
			self.btnChatSizing.SetPosition(btnX, y)
			self.heightBar = self.EDIT_LINE_HEIGHT

	def RefreshBoardViewState(self):
		(x, y, width, height) = self.GetRect()
		(btnX, btnY) = self.btnChatSizing.GetGlobalPosition()
		textAreaHeight = self.visibleLineCount * chat.GetLineStep(self.chatID)

		if localeInfo.IsARABIC():
			chat.SetPosition(self.chatID, x + width - 10, y + self.EDIT_LINE_HEIGHT)
		else:
			chat.SetPosition(self.chatID, x + 10, y + self.EDIT_LINE_HEIGHT)

		chat.SetHeight(self.chatID, y - btnY - self.EDIT_LINE_HEIGHT + 100)

		if self.boardState == chat.BOARD_STATE_EDIT:
			textAreaHeight += 45
		elif self.visibleLineCount != 0:
			textAreaHeight += 10 + 10
		
		self.xBar = x
		self.yBar = y + self.EDIT_LINE_HEIGHT - textAreaHeight
		self.widthBar = width
		self.heightBar = textAreaHeight

		self.scrollBar.Hide()

	##########
	## Render
	def OnUpdate(self):
		if self.boardState == chat.BOARD_STATE_EDIT:
			chat.Update(self.chatID)
		elif self.boardState == chat.BOARD_STATE_VIEW:
			if systemSetting.IsViewChat():
				chat.Update(self.chatID)

	def OnRender(self):
		if chat.GetVisibleLineCount(self.chatID) != self.visibleLineCount:
			self.visibleLineCount = chat.GetVisibleLineCount(self.chatID)
			self.Refresh()

		if self.curHeightBar != self.heightBar:
			self.curHeightBar += (self.heightBar - self.curHeightBar) / 10

		if self.boardState == chat.BOARD_STATE_EDIT:
			grp.SetColor(self.BOARD_MIDDLE_COLOR)
			if app.ENABLE_CHATTING_WINDOW_RENEWAL:
				grp.RenderBar(self.xBar, self.yBar + (self.heightBar - self.curHeightBar) + self.EDIT_LINE_HIDE_HEIGHT, self.widthBar, self.curHeightBar)
			else:
				grp.RenderBar(self.xBar, self.yBar + (self.heightBar - self.curHeightBar) + 10, self.widthBar, self.curHeightBar)

			chat.Render(self.chatID)
		elif self.boardState == chat.BOARD_STATE_VIEW:
			if systemSetting.IsViewChat():
				grp.RenderGradationBar(self.xBar, self.yBar + (self.heightBar - self.curHeightBar), self.widthBar, self.curHeightBar, self.BOARD_START_COLOR, self.BOARD_END_COLOR)
				chat.Render(self.chatID)

	##########
	## Event
	def OnTop(self):
		self.btnChatSizing.SetTop()
		self.scrollBar.SetTop()

	def OnScroll(self):
		if not self.scrollLock:
			self.scrollBarPos = self.scrollBar.GetPos()

		lineCount = chat.GetLineCount(self.chatID)
		visibleLineCount = chat.GetVisibleLineCount(self.chatID)
		endLine = visibleLineCount + int(float(lineCount - visibleLineCount) * self.scrollBarPos)

		chat.SetEndPos(self.chatID, self.scrollBarPos)

	def OnChangeChatMode(self):
		self.chatInputSet.OnChangeChatMode()

	def SetChatFocus(self):
		self.chatInputSet.SetChatFocus()

	def BindInterface(self, interface):
		self.chatInputSet.BindInterface(interface)

	if app.ENABLE_CHATTING_WINDOW_RENEWAL:
		def __SettingOptionWndOpen(self):
			if self.wndChatSettingOption:
				if self.wndChatSettingOption.IsShow():
					self.wndChatSettingOption.Close()
				else:
					self.wndChatSettingOption.Open()

		def RefreshChatWindow(self):
			if not self.wndChatSettingOption:
				return

			for mode in OPTION_CHECKBOX_MODE.iterkeys():
				enable = self.wndChatSettingOption.GetChatModeSetting(mode)
				if enable:
					chat.EnableChatMode(self.chatID, mode)
				else:
					chat.DisableChatMode(self.chatID, mode)

## ChatLogWindow
class ChatLogWindow(ui.Window):

	BLOCK_WIDTH = 32
	CHAT_MODE_NAME = ( localeInfo.CHAT_NORMAL, localeInfo.CHAT_PARTY, localeInfo.CHAT_GUILD, localeInfo.CHAT_SHOUT, localeInfo.CHAT_INFORMATION, localeInfo.CHAT_NOTICE, )
	CHAT_MODE_INDEX = ( chat.CHAT_TYPE_TALKING,
						chat.CHAT_TYPE_PARTY,
						chat.CHAT_TYPE_GUILD,
						chat.CHAT_TYPE_SHOUT,
						chat.CHAT_TYPE_INFO,
						chat.CHAT_TYPE_NOTICE, )

	CHAT_LOG_WINDOW_MINIMUM_WIDTH = 450
	CHAT_LOG_WINDOW_MINIMUM_HEIGHT = 120

	class ResizeButton(ui.DragButton):

		def __init__(self):
			ui.DragButton.__init__(self)

		def __del__(self):
			ui.DragButton.__del__(self)

		def OnMouseOverIn(self):
			app.SetCursor(app.HVSIZE)

		def OnMouseOverOut(self):
			app.SetCursor(app.NORMAL)

	def __init__(self):

		self.allChatMode = True
		self.chatInputSet = None

		ui.Window.__init__(self)
		self.AddFlag("float")
		self.AddFlag("movable")
		self.SetWindowName("ChatLogWindow")
		self.__CreateChatInputSet()
		self.__CreateWindow()
		self.__CreateButton()
		self.__CreateScrollBar()

		self.chatID = chat.CreateChatSet(chat.CHAT_SET_LOG_WINDOW)
		chat.SetBoardState(self.chatID, chat.BOARD_STATE_LOG)
		for i in self.CHAT_MODE_INDEX:
			chat.EnableChatMode(self.chatID, i)

		if app.ENABLE_CHATTING_WINDOW_RENEWAL:
			chat.EnableChatMode(self.chatID, chat.CHAT_TYPE_EXP_INFO)
			chat.EnableChatMode(self.chatID, chat.CHAT_TYPE_ITEM_INFO)
			chat.EnableChatMode(self.chatID, chat.CHAT_TYPE_MONEY_INFO)

		self.SetPosition(20, 20)
		self.SetSize(self.CHAT_LOG_WINDOW_MINIMUM_WIDTH, self.CHAT_LOG_WINDOW_MINIMUM_HEIGHT)
		self.btnSizing.SetPosition(self.CHAT_LOG_WINDOW_MINIMUM_WIDTH-self.btnSizing.GetWidth(), self.CHAT_LOG_WINDOW_MINIMUM_HEIGHT-self.btnSizing.GetHeight()+2)

		self.OnResize()

	def __CreateChatInputSet(self):
		chatInputSet = ChatInputSet()
		chatInputSet.SetParent(self)
		chatInputSet.SetEscapeEvent(ui.__mem_func__(self.Close))
		chatInputSet.SetWindowVerticalAlignBottom()
		chatInputSet.Open()
		self.chatInputSet = chatInputSet

	def __CreateWindow(self):
		imgLeft = ui.ImageBox()
		imgLeft.AddFlag("not_pick")
		imgLeft.SetParent(self)				

		imgCenter = ui.ExpandedImageBox()
		imgCenter.AddFlag("not_pick")
		imgCenter.SetParent(self)
		
		imgRight = ui.ImageBox()
		imgRight.AddFlag("not_pick")
		imgRight.SetParent(self)			
		
		if localeInfo.IsARABIC():
			imgLeft.LoadImage("locale/ae/ui/pattern/titlebar_left.tga")
			imgCenter.LoadImage("locale/ae/ui/pattern/titlebar_center.tga")
			imgRight.LoadImage("locale/ae/ui/pattern/titlebar_right.tga")
		else:
			imgLeft.LoadImage("d:/ymir work/ui/pattern/chatlogwindow_titlebar_left.tga")
			imgCenter.LoadImage("d:/ymir work/ui/pattern/chatlogwindow_titlebar_middle.tga")
			imgRight.LoadImage("d:/ymir work/ui/pattern/chatlogwindow_titlebar_right.tga")		

		imgLeft.Show()
		imgCenter.Show()
		imgRight.Show()

		btnClose = ui.Button()
		btnClose.SetParent(self)
		btnClose.SetUpVisual("d:/ymir work/ui/public/close_button_01.sub")
		btnClose.SetOverVisual("d:/ymir work/ui/public/close_button_02.sub")
		btnClose.SetDownVisual("d:/ymir work/ui/public/close_button_03.sub")
		btnClose.SetToolTipText(localeInfo.UI_CLOSE, 0, -23)
		btnClose.SetEvent(ui.__mem_func__(self.Close))
		btnClose.Show()

		btnSizing = self.ResizeButton()
		btnSizing.SetParent(self)
		btnSizing.SetMoveEvent(ui.__mem_func__(self.OnResize))
		btnSizing.SetSize(16, 16)
		btnSizing.Show()

		titleName = ui.TextLine()
		titleName.SetParent(self)
		
		if localeInfo.IsARABIC():
			titleName.SetPosition(self.GetWidth()-20, 6)
		else:
			titleName.SetPosition(20, 6)
			
		titleName.SetText(localeInfo.CHAT_LOG_TITLE)
		titleName.Show()

		self.imgLeft = imgLeft
		self.imgCenter = imgCenter
		self.imgRight = imgRight
		self.btnClose = btnClose
		self.btnSizing = btnSizing
		self.titleName = titleName

	def __CreateButton(self):
	
		if localeInfo.IsARABIC():
			bx = 20
		else:
			bx = 13

		btnAll = ui.RadioButton()
		btnAll.SetParent(self)
		btnAll.SetPosition(bx, 24)
		btnAll.SetUpVisual("d:/ymir work/ui/public/xsmall_button_01.sub")
		btnAll.SetOverVisual("d:/ymir work/ui/public/xsmall_button_02.sub")
		btnAll.SetDownVisual("d:/ymir work/ui/public/xsmall_button_03.sub")
		btnAll.SetText(localeInfo.CHAT_ALL)
		btnAll.SetEvent(ui.__mem_func__(self.ToggleAllChatMode))
		btnAll.Down()
		btnAll.Show()
		self.btnAll = btnAll

		x = bx + 48
		i = 0
		self.modeButtonList = []
		for name in self.CHAT_MODE_NAME:
			btn = ui.ToggleButton()
			btn.SetParent(self)
			btn.SetPosition(x, 24)
			btn.SetUpVisual("d:/ymir work/ui/public/xsmall_button_01.sub")
			btn.SetOverVisual("d:/ymir work/ui/public/xsmall_button_02.sub")
			btn.SetDownVisual("d:/ymir work/ui/public/xsmall_button_03.sub")
			btn.SetText(name)
			btn.Show()

			mode = self.CHAT_MODE_INDEX[i]
			btn.SetToggleUpEvent(lambda arg=mode: self.ToggleChatMode(arg))
			btn.SetToggleDownEvent(lambda arg=mode: self.ToggleChatMode(arg))
			self.modeButtonList.append(btn)

			x += 48
			i += 1

	def __CreateScrollBar(self):
		scrollBar = ui.SmallThinScrollBar()
		scrollBar.SetParent(self)
		scrollBar.Show()
		scrollBar.SetScrollEvent(ui.__mem_func__(self.OnScroll))
		self.scrollBar = scrollBar
		self.scrollBarPos = 1.0

	def __del__(self):
		ui.Window.__del__(self)

	def Destroy(self):
		self.imgLeft = None
		self.imgCenter = None
		self.imgRight = None
		self.btnClose = None
		self.btnSizing = None
		self.modeButtonList = []
		self.scrollBar = None
		self.chatInputSet = None

	def ToggleAllChatMode(self):
		if self.allChatMode:
			return

		self.allChatMode = True

		for i in self.CHAT_MODE_INDEX:
			chat.EnableChatMode(self.chatID, i)

		if app.ENABLE_CHATTING_WINDOW_RENEWAL:
			chat.EnableChatMode(self.chatID, chat.CHAT_TYPE_EXP_INFO)
			chat.EnableChatMode(self.chatID, chat.CHAT_TYPE_ITEM_INFO)
			chat.EnableChatMode(self.chatID, chat.CHAT_TYPE_MONEY_INFO)

		for btn in self.modeButtonList:
			btn.SetUp()

	def ToggleChatMode(self, mode):
		if self.allChatMode:
			self.allChatMode = False
			for i in self.CHAT_MODE_INDEX:
				chat.DisableChatMode(self.chatID, i)

			chat.EnableChatMode(self.chatID, mode)
			if app.ENABLE_CHATTING_WINDOW_RENEWAL:
				if mode == chat.CHAT_TYPE_INFO:
					chat.EnableChatMode(self.chatID, chat.CHAT_TYPE_EXP_INFO)
					chat.EnableChatMode(self.chatID, chat.CHAT_TYPE_ITEM_INFO)
					chat.EnableChatMode(self.chatID, chat.CHAT_TYPE_MONEY_INFO)

			self.btnAll.SetUp()
		else:
			chat.ToggleChatMode(self.chatID, mode)

	def SetSize(self, width, height):
		self.imgCenter.SetRenderingRect(0.0, 0.0, float((width - self.BLOCK_WIDTH*2) - self.BLOCK_WIDTH) / self.BLOCK_WIDTH, 0.0)
		self.imgCenter.SetPosition(self.BLOCK_WIDTH, 0)
		self.imgRight.SetPosition(width - self.BLOCK_WIDTH, 0)
		
		if localeInfo.IsARABIC():
			self.titleName.SetPosition(self.GetWidth()-20, 3)
			self.btnClose.SetPosition(3, 3)
			self.scrollBar.SetPosition(1, 45)
		else:
			self.btnClose.SetPosition(width - self.btnClose.GetWidth() - 5, 5)			
			self.scrollBar.SetPosition(width - 15, 45)
			
		self.scrollBar.SetScrollBarSize(height - 45 - 12)
		self.scrollBar.SetPos(self.scrollBarPos)
		ui.Window.SetSize(self, width, height)

	def Open(self):
		self.OnResize()
		self.chatInputSet.SetChatFocus()
		self.Show()

	def Close(self):
		if self.chatInputSet:
			self.chatInputSet.KillChatFocus()
		self.Hide()

	def OnResize(self):
		x, y = self.btnSizing.GetLocalPosition()
		width = self.btnSizing.GetWidth()
		height = self.btnSizing.GetHeight()

		if x < self.CHAT_LOG_WINDOW_MINIMUM_WIDTH - width:
			self.btnSizing.SetPosition(self.CHAT_LOG_WINDOW_MINIMUM_WIDTH - width, y)
			return
		if y < self.CHAT_LOG_WINDOW_MINIMUM_HEIGHT - height:
			self.btnSizing.SetPosition(x, self.CHAT_LOG_WINDOW_MINIMUM_HEIGHT - height)
			return

		self.scrollBar.LockScroll()
		self.SetSize(x + width, y + height)
		self.scrollBar.UnlockScroll()

		if localeInfo.IsARABIC():
			self.chatInputSet.SetPosition(20, 25)
		else:
			self.chatInputSet.SetPosition(0, 25)
			
		self.chatInputSet.SetSize(self.GetWidth() - 20, 20)
		self.chatInputSet.RefreshPosition()
		self.chatInputSet.SetChatMax(self.GetWidth() / 8)

	def OnScroll(self):
		self.scrollBarPos = self.scrollBar.GetPos()

		lineCount = chat.GetLineCount(self.chatID)
		visibleLineCount = chat.GetVisibleLineCount(self.chatID)
		endLine = visibleLineCount + int(float(lineCount - visibleLineCount) * self.scrollBarPos)

		chat.SetEndPos(self.chatID, self.scrollBarPos)

	def OnRender(self):
		(x, y, width, height) = self.GetRect()
		
		if localeInfo.IsARABIC():
			grp.SetColor(0x77000000)
			grp.RenderBar(x+2, y+45, 13, height-45)
			
			grp.SetColor(0x77000000)
			grp.RenderBar(x, y, width, height)
			grp.SetColor(0x77000000)
			grp.RenderBox(x, y, width-2, height)
			grp.SetColor(0x77000000)
			grp.RenderBox(x+1, y+1, width-2, height)

			grp.SetColor(0xff989898)
			grp.RenderLine(x+width-13, y+height-1, 11, -11)
			grp.RenderLine(x+width-9, y+height-1, 7, -7)
			grp.RenderLine(x+width-5, y+height-1, 3, -3)
		else:
			grp.SetColor(0x77000000)
			grp.RenderBar(x+width-15, y+45, 13, height-45)

			grp.SetColor(0x77000000)
			grp.RenderBar(x, y, width, height)
			grp.SetColor(0x77000000)
			grp.RenderBox(x, y, width-2, height)
			grp.SetColor(0x77000000)
			grp.RenderBox(x+1, y+1, width-2, height)

			grp.SetColor(0xff989898)
			grp.RenderLine(x+width-13, y+height-1, 11, -11)
			grp.RenderLine(x+width-9, y+height-1, 7, -7)
			grp.RenderLine(x+width-5, y+height-1, 3, -3)

		#####

		chat.ArrangeShowingChat(self.chatID)

		if localeInfo.IsARABIC():
			chat.SetPosition(self.chatID, x + width - 10, y + height - 25)
		else:
			chat.SetPosition(self.chatID, x + 10, y + height - 25)

		chat.SetHeight(self.chatID, height - 45 - 25)
		chat.Update(self.chatID)
		chat.Render(self.chatID)

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def BindInterface(self, interface):
		self.interface = interface
		
	def OnMouseLeftButtonDown(self):
		hyperlink = ui.GetHyperlink()
		if hyperlink:
			if app.IsPressed(app.DIK_LALT):
				link = chat.GetLinkFromHyperlink(hyperlink)
				ime.PasteString(link)
			else:
				self.interface.MakeHyperlinkTooltip(hyperlink)

