#include "StdAfx.h"
#include "PythonSystem.h"

PyObject * systemGetWidth(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetWidth());
}

PyObject * systemGetHeight(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetHeight());
}

PyObject * systemSetInterfaceHandler(PyObject* poSelf, PyObject* poArgs)
{
	PyObject* poHandler;
	if (!PyTuple_GetObject(poArgs, 0, &poHandler))
		return Py_BuildException();

	CPythonSystem::Instance().SetInterfaceHandler(poHandler);
	return Py_BuildNone();
}

PyObject * systemDestroyInterfaceHandler(PyObject* poSelf, PyObject* poArgs)
{
	CPythonSystem::Instance().DestroyInterfaceHandler();
	return Py_BuildNone();
}

PyObject * systemReserveResource(PyObject* poSelf, PyObject* poArgs)
{
	std::set<std::string> ResourceSet;
	CResourceManager::Instance().PushBackgroundLoadingSet(ResourceSet);
	return Py_BuildNone();
}

PyObject * systemisInterfaceConfig(PyObject* poSelf, PyObject* poArgs)
{
	int isInterfaceConfig = CPythonSystem::Instance().isInterfaceConfig();
	return Py_BuildValue("i", isInterfaceConfig);
}

PyObject * systemSaveWindowStatus(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	int iVisible;
	if (!PyTuple_GetInteger(poArgs, 1, &iVisible))
		return Py_BuildException();

	int iMinimized;
	if (!PyTuple_GetInteger(poArgs, 2, &iMinimized))
		return Py_BuildException();

	int ix;
	if (!PyTuple_GetInteger(poArgs, 3, &ix))
		return Py_BuildException();

	int iy;
	if (!PyTuple_GetInteger(poArgs, 4, &iy))
		return Py_BuildException();

	int iHeight;
	if (!PyTuple_GetInteger(poArgs, 5, &iHeight))
		return Py_BuildException();

	CPythonSystem::Instance().SaveWindowStatus(iIndex, iVisible, iMinimized, ix, iy, iHeight);
	return Py_BuildNone();
}

PyObject * systemGetWindowStatus(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	const CPythonSystem::TWindowStatus & c_rWindowStatus = CPythonSystem::Instance().GetWindowStatusReference(iIndex);
	return Py_BuildValue("iiiii", c_rWindowStatus.isVisible,
								  c_rWindowStatus.isMinimized,
								  c_rWindowStatus.ixPosition,
								  c_rWindowStatus.iyPosition,
								  c_rWindowStatus.iHeight);
}

PyObject * systemGetConfig(PyObject * poSelf, PyObject * poArgs)
{
	CPythonSystem::TConfig *tmp = CPythonSystem::Instance().GetConfig();

	int iRes = CPythonSystem::Instance().GetResolutionIndex(tmp->width, tmp->height, tmp->bpp);
	int iFrequency = CPythonSystem::Instance().GetFrequencyIndex(iRes, tmp->frequency);

	return Py_BuildValue("iiiiiiii",  iRes,
									  iFrequency,
									  tmp->is_software_cursor,
									  tmp->is_object_culling,
									  tmp->music_volume,
									  tmp->voice_volume,
									  tmp->gamma,
									  tmp->iDistance);
}

PyObject * systemSetSaveID(PyObject * poSelf, PyObject * poArgs)
{
	int iValue;
	if (!PyTuple_GetInteger(poArgs, 0, &iValue))
		return Py_BuildException();

	char * szSaveID;
	if (!PyTuple_GetString(poArgs, 1, &szSaveID))
		return Py_BuildException();

	CPythonSystem::Instance().SetSaveID(iValue, szSaveID);
	return Py_BuildNone();
}

PyObject * systemisSaveID(PyObject * poSelf, PyObject * poArgs)
{
	int value = CPythonSystem::Instance().IsSaveID();
	return Py_BuildValue("i", value);
}

#ifdef ENABLE_CHAT_COLOR_SYSTEM
PyObject * systemGetChatColor(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetChatColor());
}

PyObject * systemSetChatColor(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetChatColor(iFlag);

	return Py_BuildNone();
}
#endif

PyObject * systemGetSaveID(PyObject * poSelf, PyObject * poArgs)
{
	const char * c_szSaveID = CPythonSystem::Instance().GetSaveID();
	return Py_BuildValue("s", c_szSaveID);
}

PyObject * systemGetMusicVolume(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("f", CPythonSystem::Instance().GetMusicVolume());
}

PyObject * systemGetSoundVolume(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetSoundVolume());
}

PyObject * systemSetMusicVolume(PyObject * poSelf, PyObject * poArgs)
{
	float fVolume;
	if (!PyTuple_GetFloat(poArgs, 0, &fVolume))
		return Py_BuildException();

	CPythonSystem::Instance().SetMusicVolume(fVolume);
	return Py_BuildNone();
}

#ifdef ENABLE_FOV_OPTION
PyObject * systemSetFOVLevel(PyObject * poSelf, PyObject * poArgs)
{
float fFOV;
if (!PyTuple_GetFloat(poArgs, 0, &fFOV))
return Py_BuildException();
CPythonSystem::Instance().SetFOVLevel(fFOV);
return Py_BuildNone();
}
PyObject * systemGetFOVLevel(PyObject * poSelf, PyObject * poArgs)
{
return Py_BuildValue("f", CPythonSystem::Instance().GetFOVLevel());
}
#endif

PyObject * systemSetSoundVolumef(PyObject * poSelf, PyObject * poArgs)
{
	float fVolume;
	if (!PyTuple_GetFloat(poArgs, 0, &fVolume))
		return Py_BuildException();

	CPythonSystem::Instance().SetSoundVolumef(fVolume);
	return Py_BuildNone();
}

PyObject * systemIsSoftwareCursor(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsSoftwareCursor());
}

PyObject * systemSetViewChatFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetViewChatFlag(iFlag);

	return Py_BuildNone();
}

PyObject * systemIsViewChat(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsViewChat());
}

PyObject * systemSetAlwaysShowNameFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetAlwaysShowNameFlag(iFlag);

	return Py_BuildNone();
}

PyObject * systemSetShowDamageFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetShowDamageFlag(iFlag);

	return Py_BuildNone();
}

PyObject * systemSetShowSalesTextFlag(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetShowSalesTextFlag(iFlag);

	return Py_BuildNone();
}

PyObject * systemIsAlwaysShowName(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsAlwaysShowName());
}

PyObject * systemIsShowDamage(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsShowDamage());
}

PyObject * systemIsShowSalesText(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsShowSalesText());
}

PyObject * systemEfektGG(PyObject * poSelf, PyObject * poArgs)
{
	BYTE iFlag;
	if (!PyTuple_GetByte(poArgs, 0, &iFlag)){ return Py_BuildException(); }
	CPythonSystem::Instance().EfektGG(iFlag);
	return Py_BuildNone();
}
PyObject * systemSetConfig(PyObject * poSelf, PyObject * poArgs)
{
	int res_index;
	int width;
	int height;
	int bpp;
	int frequency_index;
	int frequency;
	int software_cursor;
	int shadow;
	int object_culling;
	int music_volume;
	int voice_volume;
	int gamma;
	int distance;

	if (!PyTuple_GetInteger(poArgs, 0, &res_index))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 1, &frequency_index))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 2, &software_cursor))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 3, &shadow))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 4, &object_culling))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 5, &music_volume))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 6, &voice_volume))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 7, &gamma))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 8, &distance))
		return Py_BuildException();

	if (!CPythonSystem::Instance().GetResolution(res_index, (DWORD *) &width, (DWORD *) &height, (DWORD *) &bpp))
		return Py_BuildNone();

	if (!CPythonSystem::Instance().GetFrequency(res_index,frequency_index, (DWORD *) &frequency))
		return Py_BuildNone();

	CPythonSystem::TConfig tmp;

	memcpy(&tmp, CPythonSystem::Instance().GetConfig(), sizeof(tmp));

	tmp.width				= width;
	tmp.height				= height;
	tmp.bpp					= bpp;
	tmp.frequency			= frequency;
	tmp.is_software_cursor	= software_cursor ? true : false;
	tmp.is_object_culling	= object_culling ? true : false;
	tmp.music_volume		= (char) music_volume;
	tmp.voice_volume		= (char) voice_volume;
	tmp.gamma				= gamma;
	tmp.iDistance			= distance;

	CPythonSystem::Instance().SetConfig(&tmp);
	return Py_BuildNone();
}

PyObject * systemApplyConfig(PyObject * poSelf, PyObject * poArgs)
{
	CPythonSystem::Instance().ApplyConfig();
	return Py_BuildNone();
}

PyObject * systemSaveConfig(PyObject * poSelf, PyObject * poArgs)
{
	int ret = CPythonSystem::Instance().SaveConfig();
	return Py_BuildValue("i", ret);
}

PyObject * systemGetResolutionCount(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetResolutionCount());
}

PyObject * systemGetFrequencyCount(PyObject * poSelf, PyObject * poArgs)
{
	int	index;

	if (!PyTuple_GetInteger(poArgs, 0, &index))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonSystem::Instance().GetFrequencyCount(index));
}

PyObject * systemGetResolution(PyObject * poSelf, PyObject * poArgs)
{
	int	index;
	DWORD width = 0, height = 0, bpp = 0;

	if (!PyTuple_GetInteger(poArgs, 0, &index))
		return Py_BuildException();

	CPythonSystem::Instance().GetResolution(index, &width, &height, &bpp);
	return Py_BuildValue("iii", width, height, bpp);
}

PyObject * systemGetCurrentResolution(PyObject * poSelf, PyObject *poArgs)
{
	CPythonSystem::TConfig *tmp = CPythonSystem::Instance().GetConfig();
	return Py_BuildValue("iii", tmp->width, tmp->height, tmp->bpp);
}

PyObject * systemGetFrequency(PyObject * poSelf, PyObject * poArgs)
{
	int	index, frequency_index;
	DWORD frequency = 0;

	if (!PyTuple_GetInteger(poArgs, 0, &index))
		return Py_BuildException();

	if (!PyTuple_GetInteger(poArgs, 1, &frequency_index))
		return Py_BuildException();

	CPythonSystem::Instance().GetFrequency(index, frequency_index, &frequency);
	return Py_BuildValue("i", frequency);
}

PyObject * systemGetShadowLevel(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetShadowLevel());
}

PyObject * systemSetShadowLevel(PyObject * poSelf, PyObject * poArgs)
{
	int level;

	if (!PyTuple_GetInteger(poArgs, 0, &level))
		return Py_BuildException();

	if (level > 0)
		CPythonSystem::Instance().SetShadowLevel(level);

	return Py_BuildNone();
}

#ifdef PERFORMANCE_FEATURES
PyObject * systemGetEffectHide(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetEffectHide());
}

PyObject* systemSetDogMode(PyObject* poSelf, PyObject* poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetDogMode(iFlag);

	return Py_BuildNone();
}

PyObject* systemGetDogMode(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetDogMode());
}
#endif

PyObject * systemSetShowOutline(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetShowOutline(iFlag);
	return Py_BuildNone();
}

PyObject * systemIsShowOutline(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsShowOutline());
}

PyObject * systemSetHideNewPets(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetHideNewPets(iFlag);

	return Py_BuildNone();
}
PyObject * systemIsHideNewPets(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsHideNewPets());
}
PyObject * systemSetHidePets(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetHidePets(iFlag);

	return Py_BuildNone();
}
PyObject * systemIsHidePets(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsHidePets());
}
PyObject * systemSetHideMounts(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetHideMounts(iFlag);

	return Py_BuildNone();
}
PyObject * systemIsHideMounts(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsHideMounts());
}

PyObject * systemSetShowOfflineShop(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetShowOfflineShop(iFlag);

	return Py_BuildNone();
}

PyObject * systemIsShowOfflineShop(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetShowOfflineShopFlag());
}

#ifdef ENABLE_ENVIRONMENT_EFFECT_OPTION
PyObject * systemSetNightModeOption(PyObject * poSelf, PyObject * poArgs)
{
	int iOpt;
	if (!PyTuple_GetInteger(poArgs, 0, &iOpt))
		return Py_BuildException();

	CPythonSystem::Instance().SetNightModeOption(iOpt);

	return Py_BuildNone();
}

PyObject * systemSetSnowModeOption(PyObject * poSelf, PyObject * poArgs)
{
	int iOpt;
	if (!PyTuple_GetInteger(poArgs, 0, &iOpt))
		return Py_BuildException();

	CPythonSystem::Instance().SetSnowModeOption(iOpt);

	return Py_BuildNone();
}

PyObject * systemSetSnowTextureModeOption(PyObject * poSelf, PyObject * poArgs)
{
	int iOpt;
	if (!PyTuple_GetInteger(poArgs, 0, &iOpt))
		return Py_BuildException();

	CPythonSystem::Instance().SetSnowTextureModeOption(iOpt);

	return Py_BuildNone();
}

PyObject * systemGetNightModeOption(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetNightModeOption());
}

PyObject * systemGetSnowModeOption(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetSnowModeOption());
}

PyObject * systemGetSnowTextureModeOption(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().GetSnowTextureModeOption());
}
#endif

#ifdef ENABLE_REFINE_RENEWAL
PyObject * systemSetRefineStatus(PyObject * poSelf, PyObject * poArgs)
{
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 0, &iFlag))
		return Py_BuildException();

	CPythonSystem::Instance().SetRefineStatus(iFlag);

	return Py_BuildNone();
}

PyObject * systemIsRefineStatusShow(PyObject * poSelf, PyObject * poArgs)
{
	return Py_BuildValue("i", CPythonSystem::Instance().IsRefineStatusShow());
}
#endif
void initsystem()
{
	static PyMethodDef s_methods[] =
	{
		{ "GetWidth",					systemGetWidth,					METH_VARARGS },
		{ "GetHeight",					systemGetHeight,				METH_VARARGS },

		{ "SetInterfaceHandler",		systemSetInterfaceHandler,		METH_VARARGS },
		{ "DestroyInterfaceHandler",	systemDestroyInterfaceHandler,	METH_VARARGS },
		{ "ReserveResource",			systemReserveResource,			METH_VARARGS },

		{ "isInterfaceConfig",			systemisInterfaceConfig,		METH_VARARGS },
		{ "SaveWindowStatus",			systemSaveWindowStatus,			METH_VARARGS },
		{ "GetWindowStatus",			systemGetWindowStatus,			METH_VARARGS },

		{ "GetResolutionCount",			systemGetResolutionCount,		METH_VARARGS },
		{ "GetFrequencyCount",			systemGetFrequencyCount,		METH_VARARGS },

		{ "GetCurrentResolution",		systemGetCurrentResolution,		METH_VARARGS },

		{ "GetResolution",				systemGetResolution,			METH_VARARGS },
		{ "GetFrequency",				systemGetFrequency,				METH_VARARGS },

		{ "ApplyConfig",				systemApplyConfig,				METH_VARARGS },
		{ "SetConfig",					systemSetConfig,				METH_VARARGS },
		{ "SaveConfig",					systemSaveConfig,				METH_VARARGS },
		{ "GetConfig",					systemGetConfig,				METH_VARARGS },

		{ "SetSaveID",					systemSetSaveID,				METH_VARARGS },
		{ "isSaveID",					systemisSaveID,					METH_VARARGS },
		{ "GetSaveID",					systemGetSaveID,				METH_VARARGS },

		{ "GetMusicVolume",				systemGetMusicVolume,			METH_VARARGS },
		{ "GetSoundVolume",				systemGetSoundVolume,			METH_VARARGS },

		{ "SetMusicVolume",				systemSetMusicVolume,			METH_VARARGS },
		{ "SetSoundVolumef",			systemSetSoundVolumef,			METH_VARARGS },
		{ "IsSoftwareCursor",			systemIsSoftwareCursor,			METH_VARARGS },

		{ "SetViewChatFlag",			systemSetViewChatFlag,			METH_VARARGS },
		{ "IsViewChat",					systemIsViewChat,				METH_VARARGS },

		{ "SetAlwaysShowNameFlag",		systemSetAlwaysShowNameFlag,	METH_VARARGS },
		{ "IsAlwaysShowName",			systemIsAlwaysShowName,			METH_VARARGS },

		{ "SetShowDamageFlag",			systemSetShowDamageFlag,		METH_VARARGS },
		{ "IsShowDamage",				systemIsShowDamage,				METH_VARARGS },
#ifdef ENABLE_REFINE_RENEWAL
		{ "SetRefineStatus",			systemSetRefineStatus,			METH_VARARGS },
		{ "IsRefineShow",				systemIsRefineStatusShow,		METH_VARARGS },
#endif	
#ifdef ENABLE_CHAT_COLOR_SYSTEM
		{ "GetChatColor",				systemGetChatColor,				METH_VARARGS },
		{ "SetChatColor",				systemSetChatColor,				METH_VARARGS },
#endif
		{ "SetShowSalesTextFlag",		systemSetShowSalesTextFlag,		METH_VARARGS },
		{ "IsShowSalesText",			systemIsShowSalesText,			METH_VARARGS },
#ifdef ENABLE_ENVIRONMENT_EFFECT_OPTION
		{ "SetNightModeOption",			systemSetNightModeOption,		METH_VARARGS },
		{ "GetNightModeOption",			systemGetNightModeOption,		METH_VARARGS },
		{ "SetSnowModeOption",			systemSetSnowModeOption,		METH_VARARGS },
		{ "GetSnowModeOption",			systemGetSnowModeOption,		METH_VARARGS },
		{ "SetSnowTextureModeOption",	systemSetSnowTextureModeOption,	METH_VARARGS },
		{ "GetSnowTextureModeOption",	systemGetSnowTextureModeOption,	METH_VARARGS },
#endif
		{ "SetHideNewPets",				systemSetHideNewPets,				METH_VARARGS },
		{ "SetHidePets",				systemSetHidePets,				METH_VARARGS },
		{ "SetHideMounts",				systemSetHideMounts,			METH_VARARGS },
		{ "IsHideNewPets",				systemIsHideNewPets,			METH_VARARGS },
		{ "IsHidePets",					systemIsHidePets,				METH_VARARGS },
		{ "IsHideMounts",				systemIsHideMounts,				METH_VARARGS },
		{ "SetShowOfflineShop",			systemSetShowOfflineShop,		METH_VARARGS },
		{ "IsShowOfflineShop",			systemIsShowOfflineShop,		METH_VARARGS },


		{ "GetShadowLevel",				systemGetShadowLevel,			METH_VARARGS },
		{ "SetShadowLevel",				systemSetShadowLevel,			METH_VARARGS },
#ifdef ENABLE_FOV_OPTION
		{ "GetFOVLevel",				systemGetFOVLevel,				METH_VARARGS },
		{ "SetFOVLevel",				systemSetFOVLevel,				METH_VARARGS },
#endif
		{ "EfektGG",					systemEfektGG,					METH_VARARGS },

#ifdef PERFORMANCE_FEATURES
		{ "GetEffectHide",				systemGetEffectHide,			METH_VARARGS },
		{ "SetDogMode",					systemSetDogMode,				METH_VARARGS },
		{ "GetDogMode",					systemGetDogMode,				METH_VARARGS },
#endif

		{"SetShowOutline",				systemSetShowOutline,			METH_VARARGS},
		{"IsShowOutline",				systemIsShowOutline,			METH_VARARGS},

		{ NULL,							NULL,							NULL }
	};

	PyObject * poModule = Py_InitModule("systemSetting", s_methods);

	PyModule_AddIntConstant(poModule, "WINDOW_STATUS",		CPythonSystem::WINDOW_STATUS);
	PyModule_AddIntConstant(poModule, "WINDOW_INVENTORY",	CPythonSystem::WINDOW_INVENTORY);
	PyModule_AddIntConstant(poModule, "WINDOW_ABILITY",		CPythonSystem::WINDOW_ABILITY);
	PyModule_AddIntConstant(poModule, "WINDOW_SOCIETY",		CPythonSystem::WINDOW_SOCIETY);
	PyModule_AddIntConstant(poModule, "WINDOW_JOURNAL",		CPythonSystem::WINDOW_JOURNAL);
	PyModule_AddIntConstant(poModule, "WINDOW_COMMAND",		CPythonSystem::WINDOW_COMMAND);

	PyModule_AddIntConstant(poModule, "WINDOW_QUICK",		CPythonSystem::WINDOW_QUICK);
	PyModule_AddIntConstant(poModule, "WINDOW_GAUGE",		CPythonSystem::WINDOW_GAUGE);
	PyModule_AddIntConstant(poModule, "WINDOW_MINIMAP",		CPythonSystem::WINDOW_MINIMAP);
	PyModule_AddIntConstant(poModule, "WINDOW_CHAT",		CPythonSystem::WINDOW_CHAT);
}
