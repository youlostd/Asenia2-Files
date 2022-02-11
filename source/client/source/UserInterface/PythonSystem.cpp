#include "StdAfx.h"
#include "PythonSystem.h"
#include "PythonApplication.h"

#define DEFAULT_VALUE_ALWAYS_SHOW_NAME		true

void CPythonSystem::SetInterfaceHandler(PyObject * poHandler)
{
// NOTE : ???? ???? ??? ???. ????? ?? ?? Python?? ??? ???? ?? ??.
//        ??? __del__? Destroy? ??? Handler? NULL? ????. - [levites]
//	if (m_poInterfaceHandler)
//		Py_DECREF(m_poInterfaceHandler);

	m_poInterfaceHandler = poHandler;

//	if (m_poInterfaceHandler)
//		Py_INCREF(m_poInterfaceHandler);
}

void CPythonSystem::DestroyInterfaceHandler()
{
	m_poInterfaceHandler = NULL;
}

void CPythonSystem::SaveWindowStatus(int iIndex, int iVisible, int iMinimized, int ix, int iy, int iHeight)
{
	m_WindowStatus[iIndex].isVisible	= iVisible;
	m_WindowStatus[iIndex].isMinimized	= iMinimized;
	m_WindowStatus[iIndex].ixPosition	= ix;
	m_WindowStatus[iIndex].iyPosition	= iy;
	m_WindowStatus[iIndex].iHeight		= iHeight;
}

void CPythonSystem::GetDisplaySettings()
{
	memset(m_ResolutionList, 0, sizeof(TResolution) * RESOLUTION_MAX_NUM);
	m_ResolutionCount = 0;

	LPDIRECT3D8 lpD3D = CPythonGraphic::Instance().GetD3D();

	D3DADAPTER_IDENTIFIER8 d3dAdapterIdentifier;
	D3DDISPLAYMODE d3ddmDesktop;

	lpD3D->GetAdapterIdentifier(0, D3DENUM_NO_WHQL_LEVEL, &d3dAdapterIdentifier);
	lpD3D->GetAdapterDisplayMode(0, &d3ddmDesktop);

	// ? ???? ??? ?? ????? ????? ????..
	DWORD dwNumAdapterModes = lpD3D->GetAdapterModeCount(0);

	for (UINT iMode = 0; iMode < dwNumAdapterModes; iMode++)
	{
		D3DDISPLAYMODE DisplayMode;
		lpD3D->EnumAdapterModes(0, iMode, &DisplayMode);
		DWORD bpp = 0;

		// 800 600 ??? ????.
		if (DisplayMode.Width < 800 || DisplayMode.Height < 600)
			continue;

		// ?? 16bbp ? 32bbp? ????.
		// 16bbp? ????? ?? - [levites]
		if (DisplayMode.Format == D3DFMT_R5G6B5)
			bpp = 16;
		else if (DisplayMode.Format == D3DFMT_X8R8G8B8)
			bpp = 32;
		else
			continue;

		int check_res = false;

		for (int i = 0; !check_res && i < m_ResolutionCount; ++i)
		{
			if (m_ResolutionList[i].bpp != bpp ||
				m_ResolutionList[i].width != DisplayMode.Width ||
				m_ResolutionList[i].height != DisplayMode.Height)
				continue;

			int check_fre = false;

			// ????? ???? ????? ?????.
			for (int j = 0; j < m_ResolutionList[i].frequency_count; ++j)
			{
				if (m_ResolutionList[i].frequency[j] == DisplayMode.RefreshRate)
				{
					check_fre = true;
					break;
				}
			}

			if (!check_fre)
				if (m_ResolutionList[i].frequency_count < FREQUENCY_MAX_NUM)
					m_ResolutionList[i].frequency[m_ResolutionList[i].frequency_count++] = DisplayMode.RefreshRate;

			check_res = true;
		}

		if (!check_res)
		{
			// ??? ??? ?????.
			if (m_ResolutionCount < RESOLUTION_MAX_NUM)
			{
				m_ResolutionList[m_ResolutionCount].width			= DisplayMode.Width;
				m_ResolutionList[m_ResolutionCount].height			= DisplayMode.Height;
				m_ResolutionList[m_ResolutionCount].bpp				= bpp;
				m_ResolutionList[m_ResolutionCount].frequency[0]	= DisplayMode.RefreshRate;
				m_ResolutionList[m_ResolutionCount].frequency_count	= 1;

				++m_ResolutionCount;
			}
		}
	}
}

int	CPythonSystem::GetResolutionCount()
{
	return m_ResolutionCount;
}

int CPythonSystem::GetFrequencyCount(int index)
{
	if (index >= m_ResolutionCount)
		return 0;

    return m_ResolutionList[index].frequency_count;
}

bool CPythonSystem::GetResolution(int index, OUT DWORD *width, OUT DWORD *height, OUT DWORD *bpp)
{
	if (index >= m_ResolutionCount)
		return false;

	*width = m_ResolutionList[index].width;
	*height = m_ResolutionList[index].height;
	*bpp = m_ResolutionList[index].bpp;
	return true;
}

bool CPythonSystem::GetFrequency(int index, int freq_index, OUT DWORD *frequncy)
{
	if (index >= m_ResolutionCount)
		return false;

	if (freq_index >= m_ResolutionList[index].frequency_count)
		return false;

	*frequncy = m_ResolutionList[index].frequency[freq_index];
	return true;
}

int	CPythonSystem::GetResolutionIndex(DWORD width, DWORD height, DWORD bit)
{
	DWORD re_width, re_height, re_bit;
	int i = 0;

	while (GetResolution(i, &re_width, &re_height, &re_bit))
	{
		if (re_width == width)
			if (re_height == height)
				if (re_bit == bit)
					return i;
		i++;
	}

	return 0;
}

int	CPythonSystem::GetFrequencyIndex(int res_index, DWORD frequency)
{
	DWORD re_frequency;
	int i = 0;

	while (GetFrequency(res_index, i, &re_frequency))
	{
		if (re_frequency == frequency)
			return i;

		i++;
	}

	return 0;
}

DWORD CPythonSystem::GetWidth()
{
	return m_Config.width;
}

DWORD CPythonSystem::GetHeight()
{
	return m_Config.height;
}
DWORD CPythonSystem::GetBPP()
{
	return m_Config.bpp;
}
DWORD CPythonSystem::GetFrequency()
{
	return m_Config.frequency;
}

bool CPythonSystem::IsNoSoundCard()
{
	return m_Config.bNoSoundCard;
}

bool CPythonSystem::IsSoftwareCursor()
{
	return m_Config.is_software_cursor;
}

float CPythonSystem::GetMusicVolume()
{
	return m_Config.music_volume;
}

int CPythonSystem::GetSoundVolume()
{
	return m_Config.voice_volume;
}

void CPythonSystem::SetMusicVolume(float fVolume)
{
	m_Config.music_volume = fVolume;
}

void CPythonSystem::SetSoundVolumef(float fVolume)
{
	m_Config.voice_volume = int(5 * fVolume);
}

#ifdef ENABLE_FOV_OPTION
void CPythonSystem::SetFOVLevel(float fFOV)
{
m_Config.iFOVLevel = fMINMAX(30.0f, fFOV, 120.0f);
}

float CPythonSystem::GetFOVLevel()
{
return m_Config.iFOVLevel;
}
#endif

int CPythonSystem::GetDistance()
{
	return m_Config.iDistance;
}

int CPythonSystem::GetShadowLevel()
{
	return m_Config.iShadowLevel;
}

void CPythonSystem::SetShadowLevel(unsigned int level)
{
	m_Config.iShadowLevel = MIN(level, 5);
	CPythonBackground::instance().RefreshShadowLevel();
}

#ifdef PERFORMANCE_FEATURES
bool CPythonSystem::GetEffectHide()
{
	return m_Config.effektGG == 1;
}

void CPythonSystem::SetDogMode(int iFlag)
{
	m_Config.bDogMode = iFlag == 1 ? true : false;

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	rkChrMgr.SetDogMode(m_Config.bDogMode);
}

bool CPythonSystem::GetDogMode()
{
	return m_Config.bDogMode;
}
#endif

bool CPythonSystem::IsShowOutline()
{
	return m_Config.bShowOutline;
}

void CPythonSystem::SetShowOutline(int iFlag)
{
	m_Config.bShowOutline = iFlag == 1 ? true : false;
}

int CPythonSystem::IsSaveID()
{
	return m_Config.isSaveID;
}

const char * CPythonSystem::GetSaveID()
{
	return m_Config.SaveID;
}

bool CPythonSystem::isViewCulling()
{
	return m_Config.is_object_culling;
}

void CPythonSystem::SetSaveID(int iValue, const char * c_szSaveID)
{
	if (iValue != 1)
		return;
	
	m_Config.isSaveID = iValue;
	strncpy(m_Config.SaveID, c_szSaveID, sizeof(m_Config.SaveID) - 1);
}

CPythonSystem::TConfig * CPythonSystem::GetConfig()
{
	return &m_Config;
}

void CPythonSystem::SetConfig(TConfig * pNewConfig)
{
	m_Config = *pNewConfig;
}

void CPythonSystem::SetDefaultConfig()
{
	memset(&m_Config, 0, sizeof(m_Config));

	m_Config.width				= 1024;
	m_Config.height				= 768;
	m_Config.bpp				= 32;

#if defined( LOCALE_SERVICE_WE_JAPAN )
	m_Config.bWindowed			= true;
#else
	m_Config.bWindowed			= false;
#endif

	m_Config.is_software_cursor	= false;
	m_Config.is_object_culling	= true;
	m_Config.iDistance			= 3;

	m_Config.gamma				= 3;
	m_Config.music_volume		= 1.0f;
	m_Config.voice_volume		= 5;

	m_Config.bDecompressDDS		= 0;
	m_Config.bSoftwareTiling	= 0;
	m_Config.iShadowLevel		= 3;
#ifdef ENABLE_FOV_OPTION
	m_Config.iFOVLevel = 30.0f;
#endif
#ifdef ENABLE_REFINE_RENEWAL	
	m_Config.bRefineStatus		= true;
#endif
	m_Config.bViewChat			= true;
	m_Config.bAlwaysShowName	= DEFAULT_VALUE_ALWAYS_SHOW_NAME;
	m_Config.bShowDamage		= true;
	m_Config.bShowSalesText		= true;
#ifdef ENABLE_ENVIRONMENT_EFFECT_OPTION
	m_Config.bNightMode			= true;
	m_Config.bSnowMode			= true;
	m_Config.bSnowTextureMode	= true;
#endif
	m_Config.bHidePets = false;
	m_Config.bHideNewPets = false;
	m_Config.bHideMounts = false;
	m_Config.iShowOfflineShop	= 1;
#ifdef PERFORMANCE_FEATURES
	m_Config.effektGG = 0;
	m_Config.bDogMode = false;
#endif
	m_Config.bShowOutline = true;
}

bool CPythonSystem::IsWindowed()
{
	return m_Config.bWindowed;
}

bool CPythonSystem::IsViewChat()
{
	return m_Config.bViewChat;
}

void CPythonSystem::SetViewChatFlag(int iFlag)
{
	m_Config.bViewChat = iFlag == 1 ? true : false;
}

bool CPythonSystem::IsAlwaysShowName()
{
	return m_Config.bAlwaysShowName;
}

void CPythonSystem::SetAlwaysShowNameFlag(int iFlag)
{
	m_Config.bAlwaysShowName = iFlag == 1 ? true : false;
}

bool CPythonSystem::IsShowDamage()
{
	return m_Config.bShowDamage;
}

void CPythonSystem::SetShowDamageFlag(int iFlag)
{
	m_Config.bShowDamage = iFlag == 1 ? true : false;
}

bool CPythonSystem::IsShowSalesText()
{
	return m_Config.bShowSalesText;
}

void CPythonSystem::SetShowSalesTextFlag(int iFlag)
{
	m_Config.bShowSalesText = iFlag == 1 ? true : false;
}


#ifdef ENABLE_CHAT_COLOR_SYSTEM
bool CPythonSystem::GetChatColor()
{
	return m_Config.bChatColor;
}

void CPythonSystem::SetChatColor(int iFlag)
{
	m_Config.bChatColor = iFlag == 1 ? true : false;
}
#endif


int CPythonSystem::GetShowOfflineShopFlag()
{
	return m_Config.iShowOfflineShop;
}

void CPythonSystem::SetShowOfflineShop(int iFlag)
{
	m_Config.iShowOfflineShop = iFlag;
	// CPythonCharacterManager & rkChrMgr = CPythonCharacterManager::Instance();
	// rkChrMgr.SetVisibleForOfflineShops(iFlag);
}


bool CPythonSystem::IsAutoTiling()
{
	if (m_Config.bSoftwareTiling == 0)
		return true;

	return false;
}

#ifdef ENABLE_ENVIRONMENT_EFFECT_OPTION
void CPythonSystem::SetNightModeOption(int iOpt)
{
	m_Config.bNightMode = iOpt == 1 ? true : false;
	IAbstractPlayer& rPlayer = IAbstractPlayer::GetSingleton();

	if (m_Config.bNightMode)
		// CPythonBackground::Instance().ChangeToNight();
		rPlayer.SendDayNightUpdate("dark");
	else
		// CPythonBackground::Instance().ChangeToDay();
		rPlayer.SendDayNightUpdate("light");
}

bool CPythonSystem::GetNightModeOption()
{
	return m_Config.bNightMode;
}

void CPythonSystem::SetSnowModeOption(int iOpt)
{
	m_Config.bSnowMode = iOpt == 1 ? true : false;

	if (m_Config.bSnowMode)
		CPythonBackground::Instance().EnableSnowEnvironment();
	else
		CPythonBackground::Instance().DisableSnowEnvironment();
}

bool CPythonSystem::GetSnowModeOption()
{
	return m_Config.bSnowMode;
}

/*char* rem_path(string str, int iOpt)
{
	string to_rem_path[4] = { "textureset\\", "textureset/", "snow\\", "snow/" };
	string out = str, outstr, x = "metin2", y = "snow_metin2";

	char path_real[254] = "textureset\\";
	char path_snow[254] = "textureset\\snow\\";

	// Remove Texture Path
	for (int i = 0; i <= 3; i++)
	{
		std::size_t re_path = out.find(to_rem_path[i]);
		if (i == 0 || i == 1)
		{
			if (re_path != std::string::npos)
				out.erase(0, 11);
		}
		else if (i == 2 || i == 3)
		{
			if (re_path != std::string::npos)
				out.erase(0, 5);
		}
	}

	if (iOpt == 1)
	{
		out.replace(out.find(x), x.length(), y);
		strcat(path_snow, out.c_str());
		outstr = path_snow;

		char * file = new char[outstr.length() + 1];
		strcpy(file, outstr.c_str());
	}
	else
	{
		size_t re_path = out.find(y);
		if (re_path != std::string::npos)
			out.replace(out.find(y), y.length(), x);
		strcat(path_real, out.c_str());
		outstr = path_real;
	}

	char * file = new char[outstr.length() + 1];
	strcpy(file, outstr.c_str());

	return file;
}*/

void CPythonSystem::SetSnowTextureModeOption(int iOpt)
{
	return;
	// m_Config.bSnowTextureMode = iOpt == 1 ? true : false;

	// CPythonBackground& rkBG = CPythonBackground::Instance();
	// CMapOutdoor& rkMap = rkBG.GetMapOutdoorRef();

	// const char* GetTexture = CTerrainImpl::GetTextureSet()->GetFileName();
	// string str = GetTexture;

	// rkMap.SnowTexture(rem_path(str, iOpt));
}

bool CPythonSystem::GetSnowTextureModeOption()
{
	return false;
	// return m_Config.bSnowTextureMode;
}
#endif

#ifdef ENABLE_REFINE_RENEWAL
bool CPythonSystem::IsRefineStatusShow()
{
	return m_Config.bRefineStatus;
}

void CPythonSystem::SetRefineStatus(int iFlag)
{
	m_Config.bRefineStatus = iFlag == 1 ? true : false;
}
#endif
void CPythonSystem::SetSoftwareTiling(bool isEnable)
{
	if (isEnable)
		m_Config.bSoftwareTiling=1;
	else
		m_Config.bSoftwareTiling=2;
}

bool CPythonSystem::IsSoftwareTiling()
{
	if (m_Config.bSoftwareTiling==1)
		return true;

	return false;
}

bool CPythonSystem::IsUseDefaultIME()
{
	return m_Config.bUseDefaultIME;
}

bool CPythonSystem::LoadConfig()
{
	FILE * fp = NULL;

	if (NULL == (fp = fopen("metin2.cfg", "rt")))
		return false;

	char buf[256];
	char command[256];
	char value[256];

	while (fgets(buf, 256, fp))
	{
		if (sscanf(buf, " %s %s\n", command, value) == EOF)
			break;

		if (!stricmp(command, "WIDTH"))
			m_Config.width		= atoi(value);
		else if (!stricmp(command, "HEIGHT"))
			m_Config.height	= atoi(value);
		else if (!stricmp(command, "BPP"))
			m_Config.bpp		= atoi(value);
		else if (!stricmp(command, "FREQUENCY"))
			m_Config.frequency = atoi(value);
		else if (!stricmp(command, "SOFTWARE_CURSOR"))
			m_Config.is_software_cursor = atoi(value) ? true : false;
		else if (!stricmp(command, "OBJECT_CULLING"))
			m_Config.is_object_culling = atoi(value) ? true : false;
		else if (!stricmp(command, "VISIBILITY"))
			m_Config.iDistance = atoi(value);
		else if (!stricmp(command, "MUSIC_VOLUME")) {
			if(strchr(value, '.') == 0) { // Old compatiability
				m_Config.music_volume = pow(10.0f, (-1.0f + (((float) atoi(value)) / 5.0f)));
				if(atoi(value) == 0)
					m_Config.music_volume = 0.0f;
			} else
				m_Config.music_volume = atof(value);
		} else if (!stricmp(command, "VOICE_VOLUME"))
			m_Config.voice_volume = (char) atoi(value);
		else if (!stricmp(command, "GAMMA"))
			m_Config.gamma = atoi(value);
		else if (!stricmp(command, "IS_SAVE_ID"))
			m_Config.isSaveID = atoi(value);
		else if (!stricmp(command, "SAVE_ID"))
			strncpy(m_Config.SaveID, value, 20);
		else if (!stricmp(command, "PRE_LOADING_DELAY_TIME"))
			g_iLoadingDelayTime = atoi(value);
		else if (!stricmp(command, "WINDOWED"))
		{
			m_Config.bWindowed = atoi(value) == 1 ? true : false;
		}
		else if (!stricmp(command, "USE_DEFAULT_IME"))
			m_Config.bUseDefaultIME = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "SOFTWARE_TILING"))
			m_Config.bSoftwareTiling = atoi(value);
		else if (!stricmp(command, "SHADOW_LEVEL"))
			m_Config.iShadowLevel = atoi(value);
		else if (!stricmp(command, "DECOMPRESSED_TEXTURE"))
			m_Config.bDecompressDDS = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "NO_SOUND_CARD"))
			m_Config.bNoSoundCard = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "VIEW_CHAT"))
			m_Config.bViewChat = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "ALWAYS_VIEW_NAME"))
			m_Config.bAlwaysShowName = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "SHOW_DAMAGE"))
			m_Config.bShowDamage = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "SHOW_SALESTEXT"))
			m_Config.bShowSalesText = atoi(value) == 1 ? true : false;

#ifdef ENABLE_CHAT_COLOR_SYSTEM
		else if (!stricmp(command, "MAVIRUH_CHAT_COLOR"))
			m_Config.bChatColor = atoi(value) == 1 ? true : false;
#endif

#ifdef ENABLE_FOV_OPTION
		else if (!stricmp(command, "FIELD_OF_VIEW"))
		m_Config.iFOVLevel = atoi(value);
#endif
#ifdef ENABLE_ENVIRONMENT_EFFECT_OPTION
		else if (!stricmp(command, "NIGHT_MODE_ON"))
			m_Config.bNightMode = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "SNOW_MODE_ON"))
			m_Config.bSnowMode = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "SNOW_TEXTURE_MODE"))
			m_Config.bSnowTextureMode = atoi(value) == 1 ? true : false;
#endif
#ifdef ENABLE_REFINE_RENEWAL
		else if (!stricmp(command, "SHOW_REFINE_DIALOG"))
			m_Config.bRefineStatus = atoi(value) == 1 ? true : false;
#endif
		else if (!stricmp(command, "HIDE_PETS"))
			m_Config.bHidePets = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "HIDE_NEWPETS"))
			m_Config.bHideNewPets = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "HIDE_MOUNTS"))
			m_Config.bHideMounts = atoi(value) == 1 ? true : false;
		else if (!stricmp(command, "SHOW_OFFLINE_SHOP"))
			m_Config.iShowOfflineShop = atoi(value);
#ifdef PERFORMANCE_FEATURES
		else if (!stricmp(command, "EFFECT_HIDE"))
			m_Config.effektGG = atoi(value);
		else if (!stricmp(command, "DOG_MODE_ON"))
			m_Config.bDogMode = atoi(value) == 1 ? true : false;
#endif
		else if (!stricmp(command, "SHOW_OUTLINE"))
			m_Config.bShowOutline = atoi(value) == 1 ? true : false;
	}

	if (m_Config.bWindowed)
	{
		unsigned screen_width = GetSystemMetrics(SM_CXFULLSCREEN);
		unsigned screen_height = GetSystemMetrics(SM_CYFULLSCREEN);

		if (m_Config.width >= screen_width)
		{
			m_Config.width = screen_width;
		}
		if (m_Config.height >= screen_height)
		{
			m_Config.height = screen_height;
		}
	}
	
	m_OldConfig = m_Config;

	fclose(fp);

//	Tracef("LoadConfig: Resolution: %dx%d %dBPP %dHZ Software Cursor: %d, Music/Voice Volume: %d/%d Gamma: %d\n",
//		m_Config.width,
//		m_Config.height,
//		m_Config.bpp,
//		m_Config.frequency,
//		m_Config.is_software_cursor,
//		m_Config.music_volume,
//		m_Config.voice_volume,
//		m_Config.gamma);

	return true;
}

bool CPythonSystem::SaveConfig()
{
	FILE *fp;

	if (NULL == (fp = fopen("metin2.cfg", "wt")))
		return false;

	fprintf(fp, "WIDTH						%d\n"
				"HEIGHT						%d\n"
				"BPP						%d\n"
				"FREQUENCY					%d\n"
				"SOFTWARE_CURSOR			%d\n"
				"OBJECT_CULLING				%d\n"
				"VISIBILITY					%d\n"
				"MUSIC_VOLUME				%.3f\n"
				"VOICE_VOLUME				%d\n"
				"GAMMA						%d\n"
				"IS_SAVE_ID					%d\n"
				"SAVE_ID					%s\n"
				"PRE_LOADING_DELAY_TIME		%d\n"
				"DECOMPRESSED_TEXTURE		%d\n",
				m_Config.width,
				m_Config.height,
				m_Config.bpp,
				m_Config.frequency,
				m_Config.is_software_cursor,
				m_Config.is_object_culling,
				m_Config.iDistance,
				m_Config.music_volume,
				m_Config.voice_volume,
				m_Config.gamma,
				m_Config.isSaveID,
				m_Config.SaveID,
				g_iLoadingDelayTime,
				m_Config.bDecompressDDS);
#ifdef ENABLE_ENVIRONMENT_EFFECT_OPTION
	fprintf(fp, "NIGHT_MODE_ON			%d\n", m_Config.bNightMode);
	fprintf(fp, "SNOW_MODE_ON			%d\n", m_Config.bSnowMode);
	fprintf(fp, "SNOW_TEXTURE_MODE		%d\n", m_Config.bSnowTextureMode);
#endif

	if (m_Config.bWindowed == 1)
		fprintf(fp, "WINDOWED				%d\n", m_Config.bWindowed);
	if (m_Config.bViewChat == 0)
		fprintf(fp, "VIEW_CHAT				%d\n", m_Config.bViewChat);
	if (m_Config.bAlwaysShowName != DEFAULT_VALUE_ALWAYS_SHOW_NAME)
		fprintf(fp, "ALWAYS_VIEW_NAME		%d\n", m_Config.bAlwaysShowName);
	if (m_Config.bShowDamage == 0)
		fprintf(fp, "SHOW_DAMAGE		%d\n", m_Config.bShowDamage);
	if (m_Config.bShowSalesText == 0)
		fprintf(fp, "SHOW_SALESTEXT		%d\n", m_Config.bShowSalesText);

#ifdef ENABLE_CHAT_COLOR_SYSTEM
	fprintf(fp, "MAVIRUH_CHAT_COLOR			%d\n", m_Config.bChatColor);
#endif

#ifdef ENABLE_REFINE_RENEWAL
	fprintf(fp, "SHOW_REFINE_DIALOG		%d\n", m_Config.bRefineStatus);
#endif
	fprintf(fp, "HIDE_PETS				%d\n", m_Config.bHidePets);
	fprintf(fp, "HIDE_NEWPETS				%d\n", m_Config.bHideNewPets);
	fprintf(fp, "HIDE_MOUNTS			%d\n", m_Config.bHideMounts);
	fprintf(fp, "SHOW_OFFLINE_SHOP	%d\n",		m_Config.iShowOfflineShop);
	fprintf(fp, "USE_DEFAULT_IME		%d\n", m_Config.bUseDefaultIME);
	fprintf(fp, "SOFTWARE_TILING		%d\n", m_Config.bSoftwareTiling);
	fprintf(fp, "SHADOW_LEVEL			%d\n", m_Config.iShadowLevel);
#ifdef ENABLE_FOV_OPTION
	fprintf(fp, "FIELD_OF_VIEW %.1f\n", m_Config.iFOVLevel);
#endif
#ifdef PERFORMANCE_FEATURES
	fprintf(fp, "EFFECT_HIDE			%d\n", m_Config.effektGG);
	fprintf(fp, "DOG_MODE_ON			%d\n", m_Config.bDogMode);
#endif
	fprintf(fp, "SHOW_OUTLINE			%d\n", m_Config.bShowOutline);
	fprintf(fp, "\n");

	fclose(fp);
	return true;
}

bool CPythonSystem::LoadInterfaceStatus()
{
	FILE * File;
	File = fopen("interface.cfg", "rb");

	if (!File)
		return false;

	fread(m_WindowStatus, 1, sizeof(TWindowStatus) * WINDOW_MAX_NUM, File);
	fclose(File);
	return true;
}

void CPythonSystem::SaveInterfaceStatus()
{
	if (!m_poInterfaceHandler)
		return;

	PyCallClassMemberFunc(m_poInterfaceHandler, "OnSaveInterfaceStatus", Py_BuildValue("()"));

	FILE * File;

	File = fopen("interface.cfg", "wb");

	if (!File)
	{
		TraceError("Cannot open interface.cfg");
		return;
	}

	fwrite(m_WindowStatus, 1, sizeof(TWindowStatus) * WINDOW_MAX_NUM, File);
	fclose(File);
}

bool CPythonSystem::isInterfaceConfig()
{
	return m_isInterfaceConfig;
}

const CPythonSystem::TWindowStatus & CPythonSystem::GetWindowStatusReference(int iIndex)
{
	return m_WindowStatus[iIndex];
}

void CPythonSystem::ApplyConfig() // ?? ??? ?? ??? ???? ?? ??? ?? ??.
{
	if (m_OldConfig.gamma != m_Config.gamma)
	{
		float val = 1.0f;
		
		switch (m_Config.gamma)
		{
			case 0: val = 0.4f;	break;
			case 1: val = 0.7f; break;
			case 2: val = 1.0f; break;
			case 3: val = 1.2f; break;
			case 4: val = 1.4f; break;
		}
		
		CPythonGraphic::Instance().SetGamma(val);
	}

	if (m_OldConfig.is_software_cursor != m_Config.is_software_cursor)
	{
		if (m_Config.is_software_cursor)
			CPythonApplication::Instance().SetCursorMode(CPythonApplication::CURSOR_MODE_SOFTWARE);
		else
			CPythonApplication::Instance().SetCursorMode(CPythonApplication::CURSOR_MODE_HARDWARE);
	}

	m_OldConfig = m_Config;

	ChangeSystem();
}

void CPythonSystem::ChangeSystem()
{
	// Shadow
	/*
	if (m_Config.is_shadow)
		CScreen::SetShadowFlag(true);
	else
		CScreen::SetShadowFlag(false);
	*/
	CSoundManager& rkSndMgr = CSoundManager::Instance();
	/*
	float fMusicVolume;
	if (0 == m_Config.music_volume)
		fMusicVolume = 0.0f;
	else
		fMusicVolume= (float)pow(10.0f, (-1.0f + (float)m_Config.music_volume / 5.0f));
		*/
	rkSndMgr.SetMusicVolume(m_Config.music_volume);

	/*
	float fVoiceVolume;
	if (0 == m_Config.voice_volume)
		fVoiceVolume = 0.0f;
	else
		fVoiceVolume = (float)pow(10.0f, (-1.0f + (float)m_Config.voice_volume / 5.0f));
	*/
	rkSndMgr.SetSoundVolumeGrade(m_Config.voice_volume);	
}

void CPythonSystem::EfektGG(BYTE iFlag)
{
	m_Config.effektGG = iFlag;
	CPythonCharacterManager & rkChrMgr = CPythonCharacterManager::Instance();
	rkChrMgr.EfektGG(iFlag);
}

void CPythonSystem::Clear()
{
	SetInterfaceHandler(NULL);
}

CPythonSystem::CPythonSystem()
{
	memset(&m_Config, 0, sizeof(TConfig));

	m_poInterfaceHandler = NULL;

	SetDefaultConfig();

	LoadConfig();

	ChangeSystem();

	if (LoadInterfaceStatus())
		m_isInterfaceConfig = true;
	else
		m_isInterfaceConfig = false;
}

CPythonSystem::~CPythonSystem()
{
	assert(m_poInterfaceHandler==NULL && "CPythonSystem MUST CLEAR!");
}