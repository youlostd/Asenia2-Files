#pragma once

#pragma warning(disable:4702)
#pragma warning(disable:4100)
#pragma warning(disable:4201)
#pragma warning(disable:4511)
#pragma warning(disable:4663)
#pragma warning(disable:4018)
#pragma warning(disable:4245)
#pragma warning(disable:4715)

#if _MSC_VER >= 1400
//if don't use below, time_t is 64bit
#define _USE_32BIT_TIME_T
#endif

// #define WINVER 0x06010000
// #define _WIN32_WINNT 0x06010000

#include "../eterLib/StdAfx.h"
#include "../eterPythonLib/StdAfx.h"
#include "../gameLib/StdAfx.h"
#include "../scriptLib/StdAfx.h"
#include "../milesLib/StdAfx.h"
#include "../EffectLib/StdAfx.h"
#include "../PRTerrainLib/StdAfx.h"
#include "../SpeedTreeLib/StdAfx.h"

#ifndef __D3DRM_H__
#define __D3DRM_H__
#endif

#include <dshow.h>
#include <qedit.h>

#include "Locale.h"
#ifdef URIEL_ANTI_CHEAT
#include "urielac.h"
#endif

#include "GameType.h"
#include "Locale_inc.h"
extern DWORD __DEFAULT_CODE_PAGE__;

#define APP_NAME	"Metin 2"

enum
{
	POINT_MAX_NUM = 255,	
	CHARACTER_NAME_MAX_LEN = 24,
#if defined(LOCALE_SERVICE_JAPAN)
	PLAYER_NAME_MAX_LEN = 16,
#else
	PLAYER_NAME_MAX_LEN = 12,
#endif
#ifdef __ENABLE_NEW_OFFLINESHOP__
	OFFLINE_SHOP_NAME_MAX_LEN = 40 + CHARACTER_NAME_MAX_LEN +1,
	OFFLINE_SHOP_ITEM_MAX_LEN = 30,
#endif
};

void initudp();
void initapp();
void initime();
void initsystem();
void initchr();
void initchrmgr();
void initChat();
void initTextTail();
void initime();
void initItem();
void initNonPlayer();
void initnet();
void initPlayer();
void initSectionDisplayer();
void initServerStateChecker();
void initTrade();
void initMiniMap();
void initProfiler();
void initEvent();
void initeffect();
void initsnd();
void initeventmgr();
void initBackground();
void initwndMgr();
void initshop();
void initpack();
void initskill();
#ifdef ENABLE_NEW_PET_SYSTEM
void initskillpet();
#endif
void initfly();
void initquest();
#ifdef ENABLE_CONFIG_MODULE
void initcfg();
#endif
void initsafebox();
#ifdef ENABLE_CONFIG_MODULE
void initcfg();
#endif
void initguild();
void initMessenger();
#ifdef ENABLE_CPP_PSM
void initplayersettingsmodule();
#endif
#ifdef ENABLE_SASH_SYSTEM
void	initSash();
#endif
#ifdef ENABLE_PRIVATESHOP_SEARCH_SYSTEM
void initprivateShopSearch();
#endif
#ifdef ENABLE_AURA_SYSTEM
void initAura();
#endif
#ifdef ENABLE_CUBE_RENEWAL
void intcuberenewal();
#endif
#ifdef ENABLE_SWITCHBOT
void initSwitchbot();
#endif
#ifdef __ENABLE_NEW_OFFLINESHOP__
void initofflineshop();
#endif
extern const std::string& ApplicationStringTable_GetString(DWORD dwID);
extern const std::string& ApplicationStringTable_GetString(DWORD dwID, LPCSTR szKey);

extern const char* ApplicationStringTable_GetStringz(DWORD dwID);
extern const char* ApplicationStringTable_GetStringz(DWORD dwID, LPCSTR szKey);

extern void ApplicationSetErrorString(const char* szErrorString);

