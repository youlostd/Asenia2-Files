#pragma once

#ifdef URIEL_ANTI_CHEAT
#include <Windows.h>
#pragma comment(lib, "prime.lib")

#pragma pack(push, 1)
typedef struct _InitData
{
	void* Login;
	void* AddInputEvent;
	void* CheckInputEvent;
	void* ClearInputEvent;
	void* SetAttackKeyState;
	void* CheckAttackKeyState;
	void* GetEncryptedTargetVID;
	void* SetAttackSpeed;
	void* CheckAttackSpeed;
	void* GetCharacterStatePos;
	void* SetAttackVictim;
	void* ClearInstance;
	void* AddPackFile;
	void* CheckPackFile;
	void* AddPointer;
} InitData, * PInitData;
#pragma pack(pop)

extern "C" __declspec(dllimport) bool Init(PInitData init_data);
typedef bool 	(WINAPI* _Login)(const char* username, DWORD, DWORD, DWORD*);
typedef bool 	(WINAPI* _AddInputEvent)(DWORD);
typedef bool 	(WINAPI* _CheckInputEvent)(DWORD);
typedef void 	(WINAPI* _ClearInputEvent)(DWORD);
typedef void 	(WINAPI* _SetAttackKeyState)(bool);
typedef void 	(WINAPI* _CheckAttackKeyState)(bool);
typedef bool 	(WINAPI* _GetEncryptedTargetVID)(DWORD*, void*, DWORD, DWORD);
typedef bool 	(WINAPI* _SetAttackSpeed)(DWORD, DWORD);
typedef bool 	(WINAPI* _CheckAttackSpeed)(float, DWORD);
typedef bool 	(WINAPI* _GetCharacterStatePos)(LONG*, LONG*, DWORD, DWORD);
typedef bool 	(WINAPI* _SetAttackVictim)(void*);
typedef void 	(WINAPI* _ClearInstance)(DWORD);
typedef bool 	(WINAPI* _AddPackFile)(const char*);
typedef bool 	(WINAPI* _CheckPackFile)();
typedef bool 	(WINAPI* _AddPointer)(void*, void*);

namespace urielac
{
	DWORD Initialize();
	extern _Login					Login;
	extern _AddInputEvent			AddInputEvent;
	extern _CheckInputEvent			CheckInputEvent;
	extern _ClearInputEvent			ClearInputEvent;
	extern _SetAttackKeyState		SetAttackKeyState;
	extern _CheckAttackKeyState		CheckAttackKeyState;
	extern _GetEncryptedTargetVID	GetEncryptedTargetVID;
	extern _SetAttackSpeed			SetAttackSpeed;
	extern _CheckAttackSpeed		CheckAttackSpeed;
	extern _GetCharacterStatePos	GetCharacterStatePos;
	extern _SetAttackVictim			SetAttackVictim;
	extern _ClearInstance			ClearInstance;
	extern _AddPackFile				AddPackFile;
	extern _CheckPackFile			CheckPackFile;
	extern _AddPointer				AddPointer;
};
#endif
