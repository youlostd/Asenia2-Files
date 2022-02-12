"""
	Intro Login Developed By .Rui
"""
import dbg
import app
import uiTarget
import net
import ui
import ime
import snd
import wndMgr
import musicInfo
import serverInfo
import systemSetting
import ServerStateChecker
import localeInfo
import datetime
import constInfo
import uiCommon
import time
import serverCommandParser
import ime
import uiScriptLocale
import _winreg
import os
import grp
import translate

def SecondToHM(time):
	second = int(time % 60)
	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60) % 24

	if hour <= 0:
		return "%d Dakika %02d Saniye" % (minute, second)
	else:
		return "%d Saat %02d Dakika %02d Saniye" % (hour, minute, second)

FAIL_LOGIN_COUNTER = 0

BASIC_DEF = {
	"CH_NUM" : 4,
	"SAVE_ACCOUNT" : 6,
	"SOCIAL" : 3,
	"REDE_SOCIAL_1" : "https://lodos2.com/",
	"REDE_SOCIAL_2" : "https://tanitim.lodos2.com/",
	"REDE_SOCIAL_3" : "https://discord.gg/lodos2",
	"CREATE_ACC" : "https://lodos2.com/Register",
	"LOSE_PW" : "https://lodos2.com/Forget/Password",
	"CHANNEL_FILE" : "channel.inf",
}

DEFAULTS_TEXT = {
	"TEXT_1" : "Kanal {}",
	"MSG_SUPLOGIN" : "Ne yapmak istiyorsun?",
	"MSG_1" : "Hesabý kaydedemezsiniz.",
	"MSG_2" : "Hesap baþarýyla kaydedildi.",
	"MSG_3" : "Bu kullanýcý adýnýn kayýtlý olduðu bir hesabýnýz zaten var.",
	"MSG_4" : "Hesabý kaydet",
}

IMAGE_STATE = {
	0 : "d:/ymir work/login/state_button_ch/state_none.tga",
	1 : "d:/ymir work/login/state_button_ch/state_on.tga",
	2 : "d:/ymir work/login/state_button_ch/state_busy.tga",
	3 : "d:/ymir work/login/state_button_ch/state_full.tga",
}

IMAGE_SAVE_ACCOUNT = {
	0 : ["d:/ymir work/login/save_acc/save_button_1.tga","d:/ymir work/login/save_acc/save_button_2.tga","d:/ymir work/login/save_acc/save_button_3.tga"],
	1 : ["d:/ymir work/login/save_acc/add_acc_1.tga","d:/ymir work/login/save_acc/add_acc_2.tga","d:/ymir work/login/save_acc/add_acc_3.tga"],
}

REG_PATH = r"SOFTWARE\M2uAccounts" #Localização onde as contas vão ser guardadas - Registos do Windows


class LoginWindow(ui.ScriptWindow):
	def __init__(self, stream):
		ui.ScriptWindow.__init__(self)
		
		net.SetPhaseWindow(net.PHASE_WINDOW_LOGIN, self)
		net.SetAccountConnectorHandler(self)
		
		self.button_ch = {
			"channel" : [],
			"text_channel" : [],
			
			"channel_state" : [],
			"channel_state_text" : [],
		}
		self.save_acc = {
			"buttons" : [],
			"save_acc_buttons" : [],
			"del_account_buttons" : [],
			"text_buttons" : [],
			
			"image_toggle" : [],
		}
		self.r_s = []
		self.exit = None
		self.login = None
		self.lost_pw = None
		
		self.act_ch = None
		
		self.stream = stream
		
		self.onPressKeyDict = None
		self.__BuildKeyDict()
	
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		
		net.ClearPhaseWindow(net.PHASE_WINDOW_LOGIN, self)
		net.SetAccountConnectorHandler(0)

		self.button_ch = {}
		self.save_acc = {}
		self.r_s = []
		self.act_ch = None
		self.onPressKeyDict = None
	
	def Close(self):
		ServerStateChecker.Initialize(self)
		
		if musicInfo.loginMusic != "" and musicInfo.selectMusic != "":
			snd.FadeOutMusic("BGM/"+musicInfo.loginMusic)
		
		if self.stream.popupWindow:
			self.stream.popupWindow.Close()

		self.KillFocus()
		self.Username.KillFocus()
		self.Password.KillFocus()
		self.pin.KillFocus()
		
		self.Hide()
		app.HideCursor()
		ime.ClearExceptKey()
	
	def Open(self):
		ServerStateChecker.Create(self)
		
		self.loginFailureMsgDict={
			#"DEFAULT" : localeInfo.LOGIN_FAILURE_UNKNOWN,

			"ALREADY"	: localeInfo.LOGIN_FAILURE_ALREAY,
			"NOID"		: localeInfo.LOGIN_FAILURE_NOT_EXIST_ID,
			"WRONGPWD"	: localeInfo.LOGIN_FAILURE_WRONG_PASSWORD,
			"FULL"		: localeInfo.LOGIN_FAILURE_TOO_MANY_USER,
			"SHUTDOWN"	: localeInfo.LOGIN_FAILURE_SHUTDOWN,
			"REPAIR"	: localeInfo.LOGIN_FAILURE_REPAIR_ID,
			"BLOCK"		: localeInfo.LOGIN_FAILURE_BLOCK_ID,
			"WRONGMAT"	: localeInfo.LOGIN_FAILURE_WRONG_MATRIX_CARD_NUMBER,
			"QUIT"		: localeInfo.LOGIN_FAILURE_WRONG_MATRIX_CARD_NUMBER_TRIPLE,
			"BESAMEKEY"	: localeInfo.LOGIN_FAILURE_BE_SAME_KEY,
			"NOTAVAIL"	: localeInfo.LOGIN_FAILURE_NOT_AVAIL,
			"NOBILL"	: localeInfo.LOGIN_FAILURE_NOBILL,
			"BLKLOGIN"	: localeInfo.LOGIN_FAILURE_BLOCK_LOGIN,
			"WEBBLK"	: localeInfo.LOGIN_FAILURE_WEB_BLOCK,
			"BADSCLID"	: localeInfo.LOGIN_FAILURE_WRONG_SOCIALID,
			"AGELIMIT"	: localeInfo.LOGIN_FAILURE_SHUTDOWN_TIME,
			# "VERSION" : localeInfo.LOGIN_FAILURE_WRONG_VERSION,
			# "ATIV" : localeInfo.LOGIN_FAILURE_NEED_CONFIRM_EMAIL,
			# "BAN" : localeInfo.LOGIN_FAILURE_ACCOUNT_BAN,
		}

		self.loginFailureFuncDict = {
			"WRONGPWD"	: localeInfo.LOGIN_FAILURE_WRONG_PASSWORD,
			"WRONGMAT"	: localeInfo.LOGIN_FAILURE_WRONG_MATRIX_CARD_NUMBER,
			"QUIT"		: app.Exit,
		}
		
		self.SetSize(wndMgr.GetScreenWidth(), wndMgr.GetScreenHeight())
		self.SetWindowName("LoginWindow")
		
		self.__LoadScript()
		self.__RequestServerStateList()

		if musicInfo.loginMusic != "":
			snd.SetMusicVolume(systemSetting.GetMusicVolume())
			snd.FadeInMusic("BGM/" + musicInfo.loginMusic)
		
		self.ChannelButtonArray(int(self.__LoadChannelInfo()[2])) ##CH Load
		self.Username.SetFocus()
		
		self.Username.UpdatePlaceHolder()
		self.Password.UpdatePlaceHolder()
		self.pin.UpdatePlaceHolder()
		
		ime.AddExceptKey(91)
		ime.AddExceptKey(93)
		
		self.Show()
		app.ShowCursor()

# número de atualizações do cliente
		# clientversion = open("pack/cfg/client_version.cfg","r")
		# cvvalue = clientversion.read()
		# if clientversion:
			# self.GetChild("Client_Text").SetText("Versão do Cliente : "+str(cvvalue))
		# patchversion = open("pack/cfg/patch_version.cfg","r")
		# pvvalue = patchversion.read()
		# if patchversion:
			# self.GetChild("Neu_Text").SetText("Cupom Cash : "+str(pvvalue))
# número de atualizações do cliente

	def OnConnectFailure(self):
		snd.PlaySound("sound/ui/loginfail.wav")
		self.PopupNotifyMessage(localeInfo.LOGIN_CONNECT_FAILURE, self.PassFunc)
	
	def OnHandShake(self):
		snd.PlaySound("sound/ui/loginok.wav")
		self.PopupDisplayMessage(localeInfo.LOGIN_CONNECT_SUCCESS)
	
	def OnLoginStart(self):
		self.PopupDisplayMessage(localeInfo.LOGIN_PROCESSING)
	
	def OnLoginFailure(self, error):
		try:
			loginFailureMsg = self.loginFailureMsgDict[error]
		except KeyError:
			loginFailureMsg = localeInfo.LOGIN_FAILURE_UNKNOWN  + error
		loginFailureFunc = self.loginFailureFuncDict.get(error, self.PassFunc)
		self.PopupNotifyMessage(loginFailureMsg, loginFailureFunc)
		snd.PlaySound("sound/ui/loginfail.wav")

		global FAIL_LOGIN_COUNTER
		FAIL_LOGIN_COUNTER += 1
		if (FAIL_LOGIN_COUNTER >= 7):
			xxx = app.GetGlobalTimeStamp()+600
			fail = open("lib/login_time", "w")
			fail.write(str(xxx))
			FAIL_LOGIN_COUNTER = 0
	
	def SendEmpireDataInfo(self, blue_players, red_players, yellow_players):
		constInfo.blue_players = blue_players
		constInfo.red_players = red_players
		constInfo.yellow_players = yellow_players
	
	def __LoadScript(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/loginwindow.py")
		except:
			import exception
			exception.Abort("LoginWindow.__LoadScript.LoadObject")
		
		try:
			for i in xrange(BASIC_DEF["CH_NUM"]):
				self.button_ch["channel"].append(self.GetChild("button_ch_%d" % (i+1)))
				self.button_ch["channel"][i].SetEvent(ui.__mem_func__(self.buttonchannel), i)
				
				self.button_ch["channel_state"].append(self.GetChild("ch_%d_state" % (i+1)))
				self.button_ch["channel_state"][i].LoadImage(IMAGE_STATE[0])
				
				self.button_ch["text_channel"].append(self.GetChild("ch_name_button_%d" % (i+1)))
				self.button_ch["text_channel"][i].SetText(str(DEFAULTS_TEXT["TEXT_1"]).format(i+1))
				
				self.button_ch["channel_state_text"].append(self.GetChild("ch_state_button_%d" % (i+1)))
				self.button_ch["channel_state_text"][i].SetText(str(serverInfo.STATE_NONE))
				
			for i in xrange(BASIC_DEF["SAVE_ACCOUNT"]):
				self.save_acc["buttons"].append(self.GetChild("save_button_%d" % (i+1)))
				self.save_acc["buttons"][i].SetEvent(ui.__mem_func__(self.buttonsave), i)

				self.save_acc["save_acc_buttons"].append(self.GetChild("add_acc_button_%d" % (i+1)))
				self.save_acc["save_acc_buttons"][i].SetEvent(ui.__mem_func__(self.buttonsave), i)
				
				self.save_acc["del_account_buttons"].append(self.GetChild("delete_account_button_%d" % (i+1)))
				self.save_acc["del_account_buttons"][i].SetEvent(ui.__mem_func__(self.DelAcc), i)
				self.save_acc["del_account_buttons"][i].Hide()
				
				self.save_acc["text_buttons"].append(self.GetChild("acc_name_text_%d" % (i+1)))
				self.save_acc["text_buttons"][i].Hide()	

				self.save_acc["image_toggle"].append(self.GetChild("save_image_f%d" % (i+1)))
				self.save_acc["image_toggle"][i].Hide()
				
			
			for i in xrange(BASIC_DEF["SOCIAL"]):
				self.r_s.append(self.GetChild("rs_%d" % (i+1)))
				self.r_s[i].SetEvent(ui.__mem_func__(self.__Link), BASIC_DEF["REDE_SOCIAL_%d" % (i+1)])
			
			self.exit = self.GetChild("exit")
			self.exit.SetEvent(ui.__mem_func__(self.exitFunc))
			
			self.login = self.GetChild("button_login")
			self.login.SetEvent(ui.__mem_func__(self.LoginFunc))

			self.create_account = self.GetChild("create_account")
			self.create_account.SetEvent(ui.__mem_func__(self.__Link), BASIC_DEF["CREATE_ACC"])
			
			self.lost_pw = self.GetChild("lost_pw")
			self.lost_pw.SetEvent(ui.__mem_func__(self.__Link), BASIC_DEF["LOSE_PW"])
			
			self.Username = self.GetChild("ID_EditLine")
			self.Password = self.GetChild("Password_EditLine")
			self.pin = self.GetChild("PIN_EditLine")
			
			self.Username.SetReturnEvent(ui.__mem_func__(self.Password.SetFocus))
			self.Username.SetTabEvent(ui.__mem_func__(self.Password.SetFocus))
			self.Username.SetPlaceHolder(" Username")
			
			self.Password.SetReturnEvent(ui.__mem_func__(self.Password.SetFocus))
			self.Password.SetTabEvent(ui.__mem_func__(self.pin.SetFocus))
			self.Password.SetPlaceHolder(" Password")

			self.pin.SetReturnEvent(ui.__mem_func__(self.LoginFunc))
			self.pin.SetTabEvent(ui.__mem_func__(self.Username.SetFocus))
			self.pin.SetPlaceHolder(" Pin", (0, -2))
			
		except:
			import exception
			exception.Abort("LoginWindow.__LoadScript.BindObject")
	
	def LoginFunc(self):
		id = self.Username.GetText()
		pwd = self.Password.GetText()
		pin = self.pin.GetText()
		if len(id)==0:
			self.PopupNotifyMessage(localeInfo.LOGIN_INPUT_ID, self.PassFunc)
			return
		if len(pwd)==0:
			self.PopupNotifyMessage(localeInfo.LOGIN_INPUT_PASSWORD, self.PassFunc)
			return
		if len(pin)==0:
			self.PopupNotifyMessage(localeInfo.LOGIN_INPUT_PASSWORD, self.PassFunc)
			return
		self.Connect(id, pwd, serverInfo.CLIENT_VERSION, pin)
	
	def Connect(self, id, pwd, cv, pin):
		dt = datetime.datetime.now()
		timenow = app.GetGlobalTimeStamp()
		fail_time = open("lib/login_time", "r").read()
		if (str(fail_time) != ""):
			waitLogin = int(fail_time) - int(timenow)
			if (waitLogin <= 600):
				if int(fail_time) >= int(timenow):
					loginFailureFunc=self.loginFailureFuncDict.get("NOID", self.Password)
					self.PopupNotifyMessage(translate.girisban % str(SecondToHM(waitLogin)))
					return

		if constInfo.SEQUENCE_PACKET_ENABLE:
			net.SetPacketSequenceMode()
			
		constInfo.LastAccount = id.lower()
		
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(localeInfo.LOGIN_CONNETING, self.PassFunc, localeInfo.UI_CANCEL)
		self.stream.SetLoginInfo(id, pwd, cv, pin)
		self.stream.Connect()
	
	def __BuildKeyDict(self):
		onPressKeyDict = {}
		onPressKeyDict[app.DIK_F1]	= lambda : self.buttonsave(0, True)
		onPressKeyDict[app.DIK_F2]	= lambda : self.buttonsave(1, True)
		onPressKeyDict[app.DIK_F3]	= lambda : self.buttonsave(2, True)
		onPressKeyDict[app.DIK_F4]	= lambda : self.buttonsave(3, True)
		onPressKeyDict[app.DIK_F5]	= lambda : self.buttonsave(4, True)
		onPressKeyDict[app.DIK_F6]	= lambda : self.buttonsave(5, True)
		self.onPressKeyDict = onPressKeyDict

	def OnKeyDown(self, key):
		try:
			self.onPressKeyDict[key]()
		except KeyError:
			pass
		except:
			raise

		return True
	
	def buttonsave(self, arg, dikCut = False):
		if self.get_reg("account%d.cfg" % (arg+1)):
			self.Username.SetText("%s" % str(self.get_reg("account%d.cfg" % (arg+1)).split("|")[0]))
			self.Password.SetText("%s" % str(self.get_reg("account%d.cfg" % (arg+1)).split("|")[1]))
			self.pin.SetText("%s" % str(self.get_reg("account%d.cfg" % (arg+1)).split("|")[2]))
			self.LoginFunc()
			return
		
		if dikCut:	return
		
		if len(self.Username.GetText()) == 0 or len(self.Password.GetText()) == 0 or len(self.pin.GetText()) == 0:
			self.PopupNotifyMessage(DEFAULTS_TEXT["MSG_1"], False)
			return
		for i in xrange(BASIC_DEF["SAVE_ACCOUNT"]):
			if self.get_reg("account%d.cfg" % (i+1)):
				if str(self.get_reg("account%d.cfg" % (i+1)).split("|")[0]) == str(self.Username.GetText()):
					self.PopupNotifyMessage(DEFAULTS_TEXT["MSG_3"], False)
					return
		self.set_reg("account%d.cfg" % (arg+1), self.Username.GetText() + "|" + self.Password.GetText() + "|"+ self.pin.GetText() )
		self.PopupNotifyMessage(DEFAULTS_TEXT["MSG_2"], False)
	
	def DelAcc(self, arg):
		try:
			if str(self.Username.GetText()) == str(self.get_reg("account%d.cfg" % (arg+1)).split("|")[0]):
				self.Username.SetText("")
				self.Password.SetText("")
				self.pin.SetText("")
				self.Username.SetFocus()
			self.del_reg("account%d.cfg" % (arg+1))
			self.save_acc["text_buttons"][arg]
		except:
			pass
	
	def LoadSavedAccount(self):
		for i in xrange(BASIC_DEF["SAVE_ACCOUNT"]):
			try:
				self.save_acc["save_acc_buttons"][i].Hide()
				self.save_acc["buttons"][i].Show()
				self.save_acc["del_account_buttons"][i].Show()
				self.save_acc["text_buttons"][i].SetText("%s" % str(self.get_reg("account%d.cfg" % (i+1)).split("|")[0]))
				self.save_acc["text_buttons"][i].Show()
				self.__CheckNameLenght(i) #@Fix-Check name size
				self.save_acc["image_toggle"][i].Show()
			except:
				self.save_acc["text_buttons"][i].Hide()
				self.save_acc["del_account_buttons"][i].Hide()
				self.save_acc["image_toggle"][i].Hide()
				self.save_acc["buttons"][i].Hide()
				self.save_acc["save_acc_buttons"][i].Show()
	
	def set_reg(self, name, value):
		try:
			_winreg.CreateKey(_winreg.HKEY_CURRENT_USER, REG_PATH)
			registry_key = _winreg.OpenKey(_winreg.HKEY_CURRENT_USER, REG_PATH, 0, _winreg.KEY_WRITE)
			_winreg.SetValueEx(registry_key, name, 0, _winreg.REG_SZ, value)
			_winreg.CloseKey(registry_key)
			return True
		except WindowsError:
			return False
	
	def get_reg(self, name):
		try:
			registry_key = _winreg.OpenKey(_winreg.HKEY_CURRENT_USER, REG_PATH, 0, _winreg.KEY_READ)
			value, regtype = _winreg.QueryValueEx(registry_key, name)
			_winreg.CloseKey(registry_key)
			return value
		except WindowsError:
			return None
	
	def del_reg(self, name):
		try:
			key = _winreg.OpenKey(_winreg.HKEY_CURRENT_USER, REG_PATH, 0, _winreg.KEY_ALL_ACCESS)
			_winreg.DeleteValue(key, name) 
			_winreg.CloseKey(key)
		except WindowsError:
			return None
	
	def __SaveChannelInfo(self):
		try:
			file=open(BASIC_DEF["CHANNEL_FILE"], "w")
			file.write("1 %d 0" % self.act_ch)
		except:
			print "LoginWindow.__SaveChannelInfo - SaveError"
	
	def __LoadChannelInfo(self):
		try:
			file=open(BASIC_DEF["CHANNEL_FILE"])
			lines=file.readlines()
			
			if len(lines)>0:
				tokens=lines[0].split()
			
				selServerID=int(tokens[0])
				selChannelID=int(tokens[1])
				
				if len(tokens) == 3:
					regionID = int(tokens[2])
				
				return regionID, selServerID, selChannelID
		except:
			print "LoginWindow.__LoadChannelInfo - OpenError"
			return 1, 0, 0
	
	def OnUpdate(self):
		ServerStateChecker.Update()
		self.LoadSavedAccount()
	
	def __RequestServerStateList(self):
		try:
			channelDict = serverInfo.REGION_DICT[0][1]["channel"]
		except:
			print " __RequestServerStateList - serverInfo.REGION_DICT(%d, %d)" % (0, 1)
			return
		
		ServerStateChecker.Initialize();
		for id, channelDataDict in channelDict.items():
			key=channelDataDict["key"]
			ip=channelDataDict["ip"]
			udp_port=channelDataDict["udp_port"]
			ServerStateChecker.AddChannel(key, ip, udp_port)
		ServerStateChecker.Request()
	
	def NotifyChannelState(self, addrKey, state):
		try:
			stateName=serverInfo.STATE_DICT[state]
		except:
			stateName=serverInfo.STATE_NONE
		
		self.button_ch["channel_state_text"][int((addrKey % 10) - 1)].SetText(str(stateName))
		self.button_ch["channel_state"][int((addrKey % 10) - 1)].LoadImage(IMAGE_STATE[state])
	
	def buttonchannel(self, arg):
		self.ChannelButtonArray(arg)
		self.__SaveChannelInfo()
	
	def ChannelButtonArray(self, arg):
		if not arg or arg < 0 or arg >= BASIC_DEF["CH_NUM"]:
			arg = 0
		
		self.act_ch = arg
		for i in xrange(BASIC_DEF["CH_NUM"]):
			if i != arg:
				self.button_ch["channel"][i].SetUp()
		self.button_ch["channel"][arg].Down()
		
		try:
			serverName = serverInfo.REGION_DICT[0][1]["name"]
			channelName = serverInfo.REGION_DICT[0][1]["channel"][int(arg + 1)]["name"]
		except:
			print " ERROR __OnClickSelectServerButton(%d, %d, %d)" % (0, 1, (arg + 1))
			serverName = localeInfo.CHANNEL_EMPTY_SERVER
			channelName = localeInfo.CHANNEL_NORMAL % int(arg + 1)
		
		name_tos = ("|cFFFF8C00%s |cFFFFFFFF- |cFFFFD700%s " % (serverName, channelName))
		net.SetServerInfo(name_tos.strip())
		
		try:
			ip = serverInfo.REGION_DICT[0][1]["channel"][int(arg + 1)]["ip"]
			tcp_port = serverInfo.REGION_DICT[0][1]["channel"][int(arg + 1)]["tcp_port"]
		except:
			import exception
			exception.Abort("LoginWindow.__OnClickSelectServerButton")
		
		try:
			account_ip = serverInfo.REGION_AUTH_SERVER_DICT[0][1]["ip"]
			account_port = serverInfo.REGION_AUTH_SERVER_DICT[0][1]["port"]
		except:
			account_ip = 0
			account_port = 0
		
		self.stream.SetConnectInfo(ip, tcp_port, account_ip, account_port)
		constInfo.ch = int(arg + 1)
		
		try:
			markAddrValue=serverInfo.MARKADDR_DICT[10]
			net.SetMarkServer(markAddrValue["ip"], markAddrValue["tcp_port"])
			app.SetGuildMarkPath(markAddrValue["mark"])
			app.SetGuildSymbolPath(markAddrValue["symbol_path"])
		except:
			import exception
			exception.Abort("LoginWindow.__OnClickSelectServerButton")
	
	def __CheckNameLenght(self, arg):
		count = len(self.save_acc["text_buttons"][arg].GetText())
		addSup = False
		while (self.save_acc["text_buttons"][arg].GetTextSize()[0] >= 40):
			base_text = self.save_acc["text_buttons"][arg].GetText()
			self.save_acc["text_buttons"][arg].SetText(base_text[:count] + base_text[count+1:])
			count -= 1
			addSup = True
		
		if addSup:	self.save_acc["text_buttons"][arg].SetText(base_text + "(...)")
	
	def __Link(self, arg):
		os.system("start \"\" %s" % arg)
	
	def PassFunc(self):
		pass
	
	def exitFunc(self):
		app.Exit()
		return False
	
	def PopupNotifyMessage(self, msg, func=0):
		if not func:
			func=self.PassFunc
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(msg, func, localeInfo.UI_OK)
	
	def PopupDisplayMessage(self, msg):
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(msg)

	def OnPressExitKey(self):
		app.Exit()
		return True

