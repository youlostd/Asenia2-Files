# import BeviSPythonApiGuvenlik2019 as pythonApi
import nonplayer
import app
import chr
import chrmgr
import player
import net
import pack
import background
import chat
import textTail
import event
import effect
import systemSetting
import quest
import guild
import skill
import messenger
import ime
import item
import wndMgr, uiCommon, ui, time, playerSettingModule, localeInfo, snd, mouseModule, constInfo, uiScriptLocale, interfacemodule, dbg, uiToolTip
from uiNewShop import DropDown
import nonplayer
import grp

# ELEMENT_NAME_LIST	= [localeInfo.BOSS_TRACKING_ELEMENT_NAME1, localeInfo.BOSS_TRACKING_ELEMENT_NAME2, localeInfo.BOSS_TRACKING_ELEMENT_NAME3, localeInfo.BOSS_TRACKING_ELEMENT_NAME4, localeInfo.BOSS_TRACKING_ELEMENT_NAME5, localeInfo.BOSS_TRACKING_ELEMENT_NAME6]
MOB_VNUM_LIST = [
691,
792,
1901,
2191,
2091,
1304,
2206,
2307,
1192,
2491,
2492,
2494,
2495,
3090,
3091,
3290,
3291,
3590,
3591,
3890,
3891,
3390,
3391,
3595,
3596,
3790,
3791,
3190,
3191,
3490,
3491,
3690,
3691,
6407,
2022,
2023,
2024,
2025,
2026,
2027]


MAP_INDEX_LIST =  \
{
	MOB_VNUM_LIST[0] : 64,
	MOB_VNUM_LIST[1] : 65,
	MOB_VNUM_LIST[2] : 61,
	MOB_VNUM_LIST[3] : 63,
	MOB_VNUM_LIST[4] : 104,
	MOB_VNUM_LIST[5] : 65,
	MOB_VNUM_LIST[6] : 62,
	MOB_VNUM_LIST[7] : 68,
	MOB_VNUM_LIST[8] : 72,
	MOB_VNUM_LIST[9] : 73,
	MOB_VNUM_LIST[10] : 73,
	MOB_VNUM_LIST[11] : 73,
	MOB_VNUM_LIST[12] : 73,
	MOB_VNUM_LIST[13] : 301,
	MOB_VNUM_LIST[14] : 301,
	MOB_VNUM_LIST[15] : 301,
	MOB_VNUM_LIST[16] : 301,
	MOB_VNUM_LIST[17] : 301,
	MOB_VNUM_LIST[18] : 301,
	MOB_VNUM_LIST[19] : 302,
	MOB_VNUM_LIST[20] : 302,
	MOB_VNUM_LIST[21] : 302,
	MOB_VNUM_LIST[22] : 302,
	MOB_VNUM_LIST[23] : 304,
	MOB_VNUM_LIST[24] : 304,
	MOB_VNUM_LIST[25] : 304,
	MOB_VNUM_LIST[26] : 304,
	MOB_VNUM_LIST[27] : 304,
	MOB_VNUM_LIST[28] : 304,
	MOB_VNUM_LIST[29] : 303,
	MOB_VNUM_LIST[30] : 303,
	MOB_VNUM_LIST[31] : 303,
	MOB_VNUM_LIST[32] : 303,
	MOB_VNUM_LIST[33] : 210,
	MOB_VNUM_LIST[34] : 47,
	MOB_VNUM_LIST[35] : 353,
	MOB_VNUM_LIST[36] : 355,
	MOB_VNUM_LIST[37] : 7,
	MOB_VNUM_LIST[38] : 2,
	MOB_VNUM_LIST[39] : 354,
}

ELEMENT_LIST =  \
{
	MOB_VNUM_LIST[0] : localeInfo.BOSS_TRACKING_ELEMENT_NAME4, # buz elementi
	MOB_VNUM_LIST[1] : localeInfo.BOSS_TRACKING_ELEMENT_NAME6, # ateþ elementi
	MOB_VNUM_LIST[2] : localeInfo.BOSS_TRACKING_ELEMENT_NAME3,
	MOB_VNUM_LIST[3] : localeInfo.BOSS_TRACKING_ELEMENT_NAME5,
	MOB_VNUM_LIST[4] : localeInfo.BOSS_TRACKING_ELEMENT_NAME4,
	MOB_VNUM_LIST[5] : localeInfo.BOSS_TRACKING_ELEMENT_NAME5,
	MOB_VNUM_LIST[6] : localeInfo.BOSS_TRACKING_ELEMENT_NAME2,
	MOB_VNUM_LIST[7] : localeInfo.BOSS_TRACKING_ELEMENT_NAME4,
	MOB_VNUM_LIST[8] : localeInfo.BOSS_TRACKING_ELEMENT_NAME3,
	MOB_VNUM_LIST[9] : localeInfo.BOSS_TRACKING_ELEMENT_NAME8,
	MOB_VNUM_LIST[10] : localeInfo.BOSS_TRACKING_ELEMENT_NAME8,
	MOB_VNUM_LIST[11] : localeInfo.BOSS_TRACKING_ELEMENT_NAME8,
	MOB_VNUM_LIST[12] : localeInfo.BOSS_TRACKING_ELEMENT_NAME8,
	MOB_VNUM_LIST[13] : localeInfo.BOSS_TRACKING_ELEMENT_NAME6,
	MOB_VNUM_LIST[14] : localeInfo.BOSS_TRACKING_ELEMENT_NAME6,
	MOB_VNUM_LIST[15] : localeInfo.BOSS_TRACKING_ELEMENT_NAME2,
	MOB_VNUM_LIST[16] : localeInfo.BOSS_TRACKING_ELEMENT_NAME2,
	MOB_VNUM_LIST[17] : localeInfo.BOSS_TRACKING_ELEMENT_NAME2,
	MOB_VNUM_LIST[18] : localeInfo.BOSS_TRACKING_ELEMENT_NAME2,
	MOB_VNUM_LIST[19] : localeInfo.BOSS_TRACKING_ELEMENT_NAME6,
	MOB_VNUM_LIST[20] : localeInfo.BOSS_TRACKING_ELEMENT_NAME6,
	MOB_VNUM_LIST[21] : localeInfo.BOSS_TRACKING_ELEMENT_NAME6,
	MOB_VNUM_LIST[22] : localeInfo.BOSS_TRACKING_ELEMENT_NAME6,
	MOB_VNUM_LIST[23] : localeInfo.BOSS_TRACKING_ELEMENT_NAME2,
	MOB_VNUM_LIST[24] : localeInfo.BOSS_TRACKING_ELEMENT_NAME2,
	MOB_VNUM_LIST[25] : localeInfo.BOSS_TRACKING_ELEMENT_NAME5,
	MOB_VNUM_LIST[26] : localeInfo.BOSS_TRACKING_ELEMENT_NAME5,
	MOB_VNUM_LIST[27] : localeInfo.BOSS_TRACKING_ELEMENT_NAME5,
	MOB_VNUM_LIST[28] : localeInfo.BOSS_TRACKING_ELEMENT_NAME5,
	MOB_VNUM_LIST[29] : localeInfo.BOSS_TRACKING_ELEMENT_NAME3,
	MOB_VNUM_LIST[30] : localeInfo.BOSS_TRACKING_ELEMENT_NAME3,
	MOB_VNUM_LIST[31] : localeInfo.BOSS_TRACKING_ELEMENT_NAME3,
	MOB_VNUM_LIST[32] : localeInfo.BOSS_TRACKING_ELEMENT_NAME3,
	MOB_VNUM_LIST[33] : localeInfo.BOSS_TRACKING_ELEMENT_NAME5,
	MOB_VNUM_LIST[34] : localeInfo.BOSS_TRACKING_ELEMENT_NAME8,
	MOB_VNUM_LIST[35] : localeInfo.BOSS_TRACKING_ELEMENT_NAME6,
	MOB_VNUM_LIST[36] : localeInfo.BOSS_TRACKING_ELEMENT_NAME4,
	MOB_VNUM_LIST[37] : localeInfo.BOSS_TRACKING_ELEMENT_NAME3,
	MOB_VNUM_LIST[38] : localeInfo.BOSS_TRACKING_ELEMENT_NAME5,
	MOB_VNUM_LIST[39] : localeInfo.BOSS_TRACKING_ELEMENT_NAME6,
}

MOB_REBORN_TIME =  \
{
	MOB_VNUM_LIST[0] : 1800,# Buz cadýsý çýkýþ süresi 2 saat
	MOB_VNUM_LIST[1] : 1800,# Alev Kral 15 dakika
	MOB_VNUM_LIST[2] : 1800,
	MOB_VNUM_LIST[3] : 1800,
	MOB_VNUM_LIST[4] : 1800,
	MOB_VNUM_LIST[5] : 1800,
	MOB_VNUM_LIST[6] : 1800,
	MOB_VNUM_LIST[7] : 1800,
	MOB_VNUM_LIST[8] : 3600,
	MOB_VNUM_LIST[9] : 1800,
	MOB_VNUM_LIST[10] : 1800,
	MOB_VNUM_LIST[11] : 1800,
	MOB_VNUM_LIST[12] : 1800,
	MOB_VNUM_LIST[13] : 3600,
	MOB_VNUM_LIST[14] : 3600,
	MOB_VNUM_LIST[15] : 3600,
	MOB_VNUM_LIST[16] : 3600,
	MOB_VNUM_LIST[17] : 3600,
	MOB_VNUM_LIST[18] : 3600,
	MOB_VNUM_LIST[19] : 3600,
	MOB_VNUM_LIST[20] : 3600,
	MOB_VNUM_LIST[21] : 3600,
	MOB_VNUM_LIST[22] : 3600,
	MOB_VNUM_LIST[23] : 3600,
	MOB_VNUM_LIST[24] : 3600,
	MOB_VNUM_LIST[25] : 3600,
	MOB_VNUM_LIST[26] : 3600,
	MOB_VNUM_LIST[27] : 3600,
	MOB_VNUM_LIST[28] : 3600,
	MOB_VNUM_LIST[29] : 3600,
	MOB_VNUM_LIST[30] : 3600,
	MOB_VNUM_LIST[31] : 3600,
	MOB_VNUM_LIST[32] : 3600,
	MOB_VNUM_LIST[33] : 1800,
	MOB_VNUM_LIST[34] : 7200,
	MOB_VNUM_LIST[35] : 10800,
	MOB_VNUM_LIST[36] : 14400,
	MOB_VNUM_LIST[37] : 18000,
	MOB_VNUM_LIST[38] : 21600,
	MOB_VNUM_LIST[39] : 21600,
}

def SecondToHM(time):
	second = int(time % 60)
	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60) % 24

	if hour <= 0:
		return "%d Dakika %02d Saniye" % (minute, second)
	else:
		return "%d Saat %02d Dakika %02d Saniye" % (hour, minute, second)

def GetRebornTime(time):
	second = int(time % 60)
	minute = int((time / 60) % 60)
	hour = int((time / 60) / 60) % 24
	
	if hour <= 0 and minute <= 0:
		return "%d saniye'de" % (second)
	elif hour <= 0:
		return "%d dakika'da" % (minute)
	else:
		return "%d saat'de" % (hour)
	
class BossTrackingInfoWindow(ui.ScriptWindow):
	SELECT_MOB_TIMER = 0
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.data = []
		self.__LoadWindow()
		
	def __del__(self):
		ui.ScriptWindow.__del__(self)
		

	def Destroy(self):
		self.__OnClose()
		self.ClearDictionary()
		
	def __LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/BossTrackingInfoWindow.py")
		except:
			import exception
			exception.Abort("BossTrackingInfoWindow.__LoadWindow.LoadScriptFile")
			
		try:
			self.backGround 		= self.GetChild("Background")
			self.avatarImage		= self.GetChild("AvatarImage")
			self.info1				= self.GetChild("Info1")
			#self.info2				= self.GetChild("Info2")
			self.respawnInfos		= [self.GetChild("CH%dRespawnInfo" % int(i+1)) for i in xrange(6)]
			self.channelInfo		= self.GetChild("ChannelInfo")
			self.elementInfo		= self.GetChild("ElementInfo")
			self.mapInfo			= self.GetChild("MapInfo")
			self.mobLevelInfo		= self.GetChild("MobLevelInfo")
			self.btnToolTip				= self.GetChild("ToolTipButton")
		except:
			import exception
			exception.Abort("BossTrackingInfoWindow.__LoadWindow.BindObject")
			
		try:
			self.dropDown		= DropDown(self.backGround, "Bir patron seçiniz. ")
			self.dropDown.SetPosition(136, 14)
			self.dropDown.SetSize(137, 21)
			self.dropDown.SetTop()
			self.dropDown.OnChange = self.__OnDropdownChange
			self.dropDown.ClearItems()
			self.dropDown.Show()
			self.GetChild("board").SetCloseEvent(ui.__mem_func__(self.ButtonCloseEvent))
			self.btnToolTip.SetToolTipWindow(self.__CreateToolTip(localeInfo.BOSS_TRACKING_INFO_WINDOW_TITLE, [localeInfo.BOSS_TRACKING_INFO_WINDOW_TOOLTIP1, localeInfo.BOSS_TRACKING_INFO_WINDOW_TOOLTIP2, localeInfo.BOSS_TRACKING_INFO_WINDOW_TOOLTIP3]))			
		except:		
			import exception
			exception.Abort("BossTrackingInfoWindow.__LoadWindow.LoadingObject")
			


	def __CreateToolTip(self, title, descList):
		toolTip = uiToolTip.ToolTip()
		toolTip.SetTitle(title)
		toolTip.AppendSpace(5)

		for desc in descList:
			toolTip.AutoAppendTextLine(desc)

		toolTip.AlignHorizonalCenter()
		return toolTip			
			
	def __OnDropdownChange(self):
		if app.GetTime() > self.SELECT_MOB_TIMER:
			if (0 == self.dropDown.DropList.GetSelectedItem().value):
				self.ClearGui()
				
			self.data = []
			net.SendChatPacket("/open_boss_tracking %d" % int(self.dropDown.DropList.GetSelectedItem().value))
		else:
			NewGetTime = self.SELECT_MOB_TIMER - app.GetTime()
			chat.AppendChat(chat.CHAT_TYPE_INFO, "Tekrar seçim yapabilmek için %d saniye beklemelisin." % (NewGetTime))
			
	def SetData(self, b_kill_time, b_start_time, b_channel, b_mob_vnum, map_index):
		data_tuple = (b_kill_time,b_start_time,b_channel,b_mob_vnum, map_index)
		self.data.append(data_tuple)
		
	def ConvertToTimeInfo(self):
		(arg1, arg2, arg3, arg4, arg5) = self.data[0]
		if (int(arg2) <= 0):
			convert_time = GetRebornTime(int(MOB_REBORN_TIME[int(arg4)]))
		else:
			convert_time = GetRebornTime(int(arg2))

		return convert_time

	def GetMapIndex(self):
		(arg1, arg2, arg3, arg4, arg5) = self.data[0]
		if (int(arg5) <= 0):
			return MAP_INDEX_LIST[int(arg4)]
		else:
			return int(arg5)
		
	def GetElementName(self, arg4):
		return ELEMENT_LIST[int(arg4)]
		
	def ClearGui(self):
		constInfo.DATA_KEY_BOSS_TRACKING = 0
		self.avatarImage.LoadImage("d:/ymir work/ui/boss_follow/none.tga")
		self.data = []
		self.mapInfo.SetText("Patron seçimi yapýlmadý.")
		#self.info2.SetText("Patron seçimi yapýlmadý.")
		self.info1.SetText("Patron seçimi yapýlmadý.")
		self.mobLevelInfo.SetText("Patron seçimi yapýlmadý.")
		self.channelInfo.SetText("Patron seçimi yapýlmadý.")
		self.elementInfo.SetText("Patron seçimi yapýlmadý.")
		for i in xrange(6):
			self.respawnInfos[i-1].SetText("Bir patron seçiniz.")
		
	def OnUpdate(self):
		for key in xrange(len(self.data)):
			(arg1, arg2, arg3, arg4, arg5) = self.data[key]
			format_time = (int(arg2)+int(arg1))-app.GetGlobalTimeStamp()

			if arg3 == 99:
				continue
			CHANNEL_COUNT = 6

			# if CHANNEL_COUNT < 5:
				# self.respawnInfos[6-1].SetText("CH6: Bu kanal devredýþý.")
				# self.respawnInfos[5-1].SetText("CH5: Bu kanal devredýþý.")
			
			if (int(arg4) == self.dropDown.DropList.GetSelectedItem().value):
				self.avatarImage.LoadImage("d:/ymir work/ui/boss_follow/%d.tga" % (arg4))
			if int(arg1) == 0:
				self.respawnInfos[arg3-1].SetText("CH%d: Boss þuanda yaþýyor" % (int(arg3)))
			elif (format_time < 0):
				self.respawnInfos[arg3-1].SetText("CH%d: Boss þuanda yaþýyor" % (int(arg3)))
			else:
				self.respawnInfos[arg3-1].SetText("CH%d: %s" % (int(arg3), SecondToHM(format_time)))
				
			# if CHANNEL_COUNT < 5:
				# self.respawnInfos[6-1].SetText("CH6: Bu kanal devredýþý.")
				# self.respawnInfos[5-1].SetText("CH5: Bu kanal devredýþý.")
		
			self.mapInfo.SetText(localeInfo.MINIMAP_ZONE_NAME_DICT_BY_IDX[int(self.GetMapIndex())])
			#self.info2.SetText(nonplayer.GetMonsterName(int(arg4)))
			self.info1.SetText(localeInfo.BOSS_TRACKING_INFO_REBORN_TIME % (nonplayer.GetMonsterName(int(arg4)), self.ConvertToTimeInfo()))
			self.mobLevelInfo.SetText(localeInfo.BOSS_TRACKING_INFO_LEVEL % (nonplayer.GetMonsterLevel(int(arg4))))
			self.channelInfo.SetText(uiScriptLocale.BOSS_TRACKING_INFO_CHANNEL)
			self.elementInfo.SetText(str(self.GetElementName(int(arg4))))

	def __OnClose(self):
		self.data = []
		constInfo.DATA_KEY_BOSS_TRACKING = 0
		#constInfo.OPEN_BOSS_TRACKING = 0
		self.dropDown.ClearItems()
		self.dropDown.Clear()
		self.Hide()
		
	def ButtonCloseEvent(self):
		net.SendChatPacket("/close_boss_tracking")
		self.data = []
		constInfo.DATA_KEY_BOSS_TRACKING = 0
		#constInfo.OPEN_BOSS_TRACKING = 0
		self.dropDown.ClearItems()
		self.dropDown.Clear()
		self.Hide()
		
	def OnPressEscapeKey(self):
		self.ButtonCloseEvent()
		return True
		
	def Open(self):
		self.ClearGui()
		self.dropDown.ClearItems()
		self.dropDown.Clear()
		for i in xrange(len(MOB_VNUM_LIST)):
			self.dropDown.AppendItem(nonplayer.GetMonsterName(int(MOB_VNUM_LIST[i])), int(MOB_VNUM_LIST[i]))
		self.SetCenterPosition()
		self.Show()
		constInfo.DATA_KEY_BOSS_TRACKING = 1
		self.SetTop()