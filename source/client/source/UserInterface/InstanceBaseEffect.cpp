#include "StdAfx.h"
#include "InstanceBase.h"
#include "PythonTextTail.h"
#include "AbstractApplication.h"
#include "AbstractPlayer.h"
#include "PythonPlayer.h"
#include "PythonSystem.h"

#include "../EffectLib/EffectManager.h"
#include "../EffectLib/ParticleSystemData.h"
#include "../EterLib/Camera.h"

float CInstanceBase::ms_fDustGap;
float CInstanceBase::ms_fHorseDustGap;
DWORD CInstanceBase::ms_adwCRCAffectEffect[CInstanceBase::EFFECT_NUM];
std::string CInstanceBase::ms_astAffectEffectAttachBone[EFFECT_NUM];

#define BYTE_COLOR_TO_D3DX_COLOR(r, g, b) D3DXCOLOR(float(r)/255.0f, float(g)/255.0f, float(b)/255.0f, 1.0f)

/*
D3DXCOLOR CInstanceBase::ms_kD3DXClrPC(0xFFFFD84D);//1.0f, 0.8470f, 0.3f, 1.0f
D3DXCOLOR CInstanceBase::ms_kD3DXClrNPC(0xFF7BE85E);//0.4823f, 0.9098f, 0.3686f, 1.0f
D3DXCOLOR CInstanceBase::ms_kD3DXClrMOB(0xFFEC170a);//0.9254f, 0.0901f, 0.0392f, 1.0f
D3DXCOLOR CInstanceBase::ms_kD3DXClrPVP(0xFF8532D9);
D3DXCOLOR CInstanceBase::ms_kD3DXClrPVPSelf(0xFFEE36DF);
D3DXCOLOR CInstanceBase::ms_kD3DXClrKiller = BYTE_COLOR_TO_D3DX_COLOR(180, 100, 0);
D3DXCOLOR CInstanceBase::ms_kD3DXClrTitle[CInstanceBase::TITLE_MAX_NUM] =
{
	BYTE_COLOR_TO_D3DX_COLOR(  0, 204, 255),
	BYTE_COLOR_TO_D3DX_COLOR(  0, 144, 255),
	BYTE_COLOR_TO_D3DX_COLOR( 92, 110, 255),
	BYTE_COLOR_TO_D3DX_COLOR(155, 155, 255),
	0xFFFFFFFF, // None
	BYTE_COLOR_TO_D3DX_COLOR(207, 117,   0),
	BYTE_COLOR_TO_D3DX_COLOR(235,  83,   0),
	BYTE_COLOR_TO_D3DX_COLOR(227,   0,   0),
	BYTE_COLOR_TO_D3DX_COLOR(255,   0,   0),
};
*/

D3DXCOLOR g_akD3DXClrTitle[CInstanceBase::TITLE_NUM];
#ifdef ENABLE_TITLE_SYSTEM
D3DXCOLOR g_akD3DXClrTitlePrestige[CInstanceBase::TITLE_NUM_PRESTIGE];
#endif
D3DXCOLOR g_akD3DXClrName[CInstanceBase::NAMECOLOR_NUM];

std::map<int, std::string> g_TitleNameMap;
#ifdef ENABLE_TITLE_SYSTEM
std::map<int, std::string> g_TitlePrestigeNameMap;
#endif
std::set<DWORD> g_kSet_dwPVPReadyKey;
std::set<DWORD> g_kSet_dwPVPKey;
std::set<DWORD> g_kSet_dwGVGKey;
std::set<DWORD> g_kSet_dwDUELKey;

bool g_isEmpireNameMode=false;

void  CInstanceBase::SetEmpireNameMode(bool isEnable)
{
	g_isEmpireNameMode=isEnable;

	if (isEnable)
	{
		g_akD3DXClrName[NAMECOLOR_MOB]=g_akD3DXClrName[NAMECOLOR_EMPIRE_MOB];
		g_akD3DXClrName[NAMECOLOR_NPC]=g_akD3DXClrName[NAMECOLOR_EMPIRE_NPC];
		g_akD3DXClrName[NAMECOLOR_PC]=g_akD3DXClrName[NAMECOLOR_NORMAL_PC];

		for (UINT uEmpire=1; uEmpire<EMPIRE_NUM; ++uEmpire)
			g_akD3DXClrName[NAMECOLOR_PC+uEmpire]=g_akD3DXClrName[NAMECOLOR_EMPIRE_PC+uEmpire];
		
	}
	else
	{
		g_akD3DXClrName[NAMECOLOR_MOB]=g_akD3DXClrName[NAMECOLOR_NORMAL_MOB];
		g_akD3DXClrName[NAMECOLOR_NPC]=g_akD3DXClrName[NAMECOLOR_NORMAL_NPC];

		for (UINT uEmpire=0; uEmpire<EMPIRE_NUM; ++uEmpire)
			g_akD3DXClrName[NAMECOLOR_PC+uEmpire]=g_akD3DXClrName[NAMECOLOR_NORMAL_PC];
	}
}

const D3DXCOLOR& CInstanceBase::GetIndexedNameColor(UINT eNameColor)
{
	if (eNameColor>=NAMECOLOR_NUM)
	{
		static D3DXCOLOR s_kD3DXClrNameDefault(0xffffffff);
		return s_kD3DXClrNameDefault;
	}

	return g_akD3DXClrName[eNameColor];
}

void CInstanceBase::AddDamageEffect(int64_t damage,BYTE flag,BOOL bSelf,BOOL bTarget)
{
	if(CPythonSystem::Instance().IsShowDamage())
	{		
		SEffectDamage sDamage;
		sDamage.bSelf = bSelf;
		sDamage.bTarget = bTarget;
		sDamage.damage = damage;
		sDamage.flag = flag;
		m_DamageQueue.push_back(sDamage);
	}
}

void CInstanceBase::ProcessDamage()
{
	if(m_DamageQueue.empty())
		return;

	SEffectDamage sDamage = m_DamageQueue.front();

	m_DamageQueue.pop_front();

	int64_t damage = sDamage.damage;
	BYTE flag = sDamage.flag;
	BOOL bSelf = sDamage.bSelf;
	BOOL bTarget = sDamage.bTarget;

	CCamera * pCamera = CCameraManager::Instance().GetCurrentCamera();	
	float cameraAngle = GetDegreeFromPosition2(pCamera->GetTarget().x,pCamera->GetTarget().y,pCamera->GetEye().x,pCamera->GetEye().y);

	DWORD FONT_WIDTH = 30;
	
	CEffectManager& rkEftMgr=CEffectManager::Instance();

	D3DXVECTOR3 v3Pos = m_GraphicThingInstance.GetPosition();
	v3Pos.z += float(m_GraphicThingInstance.GetHeight());

	D3DXVECTOR3 v3Rot = D3DXVECTOR3(0.0f, 0.0f, cameraAngle);

	if ( (flag & DAMAGE_DODGE) || (flag & DAMAGE_BLOCK) )
	{
		if(bSelf)
			rkEftMgr.CreateEffect(ms_adwCRCAffectEffect[EFFECT_DAMAGE_MISS],v3Pos,v3Rot);
		else
			rkEftMgr.CreateEffect(ms_adwCRCAffectEffect[EFFECT_DAMAGE_TARGETMISS],v3Pos,v3Rot);
		//__AttachEffect(EFFECT_DAMAGE_MISS);
		return;
	}
	else if (flag & DAMAGE_CRITICAL)
	{
		//rkEftMgr.CreateEffect(ms_adwCRCAffectEffect[EFFECT_DAMAGE_CRITICAL],v3Pos,v3Rot);
		//return; 숫자도 표시.
	}

	string strDamageType;
	DWORD rdwCRCEft = 0;
	/*
	if ( (flag & DAMAGE_POISON) )
	{
		strDamageType = "poison_";
		rdwCRCEft = EFFECT_DAMAGE_POISON;
	}
	else
	*/
	{
		if(bSelf)
		{
			strDamageType = "damage_";
			if(m_bDamageEffectType==0)
				rdwCRCEft = EFFECT_DAMAGE_SELFDAMAGE;
			else
				rdwCRCEft = EFFECT_DAMAGE_SELFDAMAGE2;
			m_bDamageEffectType = !m_bDamageEffectType;
		}
		else if(bTarget == false)
		{
			strDamageType = "nontarget_";
			rdwCRCEft = EFFECT_DAMAGE_NOT_TARGET;
			return;//현재 적용 안됨.
		}
		else
		{
			strDamageType = (flag & DAMAGE_CRITICAL) ? "target_crit_" : "target_";
			rdwCRCEft = EFFECT_DAMAGE_TARGET;
		}
	}
	
	int64_t index = 0;
	int64_t num = 0;
	vector<string> textures;
#ifdef DOTTED_DMG
	BYTE idxMultiplier = 0;
	#ifdef USE_M_K
	BYTE valIdx = 0;
	#endif
#endif
	while(damage>0)
	{
		if(index > 19)
		{
			TraceError("ProcessDamage무한루프 가능성");
			break;
		}
		num = damage%10;
		damage /= 10;
		char numBuf[MAX_PATH];
		sprintf(numBuf, "%lld.dds", num);
		textures.push_back("d:/ymir work/effect/affect/damagevalue/"+strDamageType+numBuf);
		
		rkEftMgr.SetEffectTextures(ms_adwCRCAffectEffect[rdwCRCEft],textures);
		
		D3DXMATRIX matrix,matTrans;
		D3DXMatrixIdentity(&matrix);
		matrix._41 = v3Pos.x;
		matrix._42 = v3Pos.y+300;
		matrix._43 = v3Pos.z;
		D3DXMatrixTranslation(&matrix,v3Pos.x,v3Pos.y,v3Pos.z);
		D3DXMatrixMultiply(&matrix,&pCamera->GetInverseViewMatrix(),&matrix);
#ifdef DOTTED_DMG
		D3DXMatrixTranslation(&matTrans, FONT_WIDTH*idxMultiplier, 0, 0);
#else
		D3DXMatrixTranslation(&matTrans,FONT_WIDTH*index,0,0);
#endif
		matTrans._41 = -matTrans._41;
		matrix = matTrans*matrix;
		D3DXMatrixMultiply(&matrix,&pCamera->GetViewMatrix(),&matrix);
		
		rkEftMgr.CreateEffect(ms_adwCRCAffectEffect[rdwCRCEft],D3DXVECTOR3(matrix._41,matrix._42,matrix._43)
			,v3Rot);	
		
		textures.clear();

		index++;

#ifdef DOTTED_DMG
		idxMultiplier++;
		if (damage > 0 && (index % 3) == 0){
	#ifdef USE_M_K
			if (valIdx > 0){
				if (valIdx > 1)
					textures.push_back("d:/ymir work/effect/affect/damagevalue/damage_t.dds");
				else
					textures.push_back("d:/ymir work/effect/affect/damagevalue/damage_m.dds");
			}
			else{
				textures.push_back("d:/ymir work/effect/affect/damagevalue/damage_k.dds");
			}
	#else
			textures.push_back("d:/ymir work/effect/affect/damagevalue/" + strDamageType + "dot.dds");
	#endif
			rkEftMgr.SetEffectTextures(ms_adwCRCAffectEffect[rdwCRCEft], textures);
			D3DXMATRIX matrix, matTrans;
			D3DXMatrixIdentity(&matrix);
			matrix._41 = v3Pos.x;
			matrix._42 = v3Pos.y;
			matrix._43 = v3Pos.z;
			D3DXMatrixTranslation(&matrix, v3Pos.x, v3Pos.y, v3Pos.z);
			D3DXMatrixMultiply(&matrix, &pCamera->GetInverseViewMatrix(), &matrix);
			D3DXMatrixTranslation(&matTrans, FONT_WIDTH*idxMultiplier, 0, 0);
			matTrans._41 = -matTrans._41;
			matrix = matTrans*matrix;
			D3DXMatrixMultiply(&matrix, &pCamera->GetViewMatrix(), &matrix);
			rkEftMgr.CreateEffect(ms_adwCRCAffectEffect[rdwCRCEft], D3DXVECTOR3(matrix._41, matrix._42, matrix._43), v3Rot);
			textures.clear();
			idxMultiplier++;
	#ifdef USE_M_K
			valIdx++;
	#endif
		}
#endif

	}	
}

void CInstanceBase::AttachSpecialEffect(DWORD effect)
{
	__AttachEffect(effect);
}

void CInstanceBase::LevelUp()
{
	__AttachEffect(EFFECT_LEVELUP);
}

void CInstanceBase::SkillUp()
{
	__AttachEffect(EFFECT_SKILLUP);
}

void CInstanceBase::CreateSpecialEffect(DWORD iEffectIndex)
{
	const D3DXMATRIX & c_rmatGlobal = m_GraphicThingInstance.GetTransform();

	DWORD dwEffectIndex = CEffectManager::Instance().GetEmptyIndex();
	DWORD dwEffectCRC = ms_adwCRCAffectEffect[iEffectIndex];
	CEffectManager::Instance().CreateEffectInstance(dwEffectIndex, dwEffectCRC);
	CEffectManager::Instance().SelectEffectInstance(dwEffectIndex);
	CEffectManager::Instance().SetEffectInstanceGlobalMatrix(c_rmatGlobal);
}

void CInstanceBase::__EffectContainer_Destroy()
{
	SEffectContainer::Dict& rkDctEftID=__EffectContainer_GetDict();

	SEffectContainer::Dict::iterator i;
	for (i=rkDctEftID.begin(); i!=rkDctEftID.end(); ++i)
		__DetachEffect(i->second);

	rkDctEftID.clear();
}

void CInstanceBase::__EffectContainer_Initialize()
{
	SEffectContainer::Dict& rkDctEftID=__EffectContainer_GetDict();
	rkDctEftID.clear();	
}

CInstanceBase::SEffectContainer::Dict& CInstanceBase::__EffectContainer_GetDict()
{
	return m_kEffectContainer.m_kDct_dwEftID;
}

// Return value 를 boolean 에서 ID 로 바꿉니다
DWORD CInstanceBase::__EffectContainer_AttachEffect(DWORD dwEftKey)
{
	SEffectContainer::Dict& rkDctEftID=__EffectContainer_GetDict();
	SEffectContainer::Dict::iterator f=rkDctEftID.find(dwEftKey);
	if (rkDctEftID.end()!=f)
		return 0;

	DWORD dwEftID=__AttachEffect(dwEftKey);
	rkDctEftID.insert(SEffectContainer::Dict::value_type(dwEftKey, dwEftID));
	return dwEftID;
}


void CInstanceBase::__EffectContainer_DetachEffect(DWORD dwEftKey)
{
	SEffectContainer::Dict& rkDctEftID=__EffectContainer_GetDict();
	SEffectContainer::Dict::iterator f=rkDctEftID.find(dwEftKey);
	if (rkDctEftID.end()==f)
		return;

	__DetachEffect(f->second);

	rkDctEftID.erase(f);
}
void CInstanceBase::__AttachEfektBossa()
{
	if (!__IsExistMainInstance())
		return;	
	
	CInstanceBase* pkInstMain=__GetMainInstancePtr();

	if (IsWarp())
		return;
	if (IsObject())
		return;
	if (IsFlag())
		return;
	if (IsResource())
		return;

	__EffectContainer_AttachEffect(EFEKT_BOSSA);
}
void CInstanceBase::__AttachEmpireEffect(DWORD eEmpire)
{
	if (!__IsExistMainInstance())
		return;	
	
	CInstanceBase* pkInstMain=__GetMainInstancePtr();

	if (IsWarp())
		return;
	if (IsObject())
		return;
	if (IsFlag())
		return;
	if (IsResource())
		return;
	if (IsNPC())
		return;
#ifdef LWT_BUFF_UPDATE
	if ((strstr(GetNameString(), "'s Buffi"))){
		if (pkInstMain->m_dwEmpireID == m_dwEmpireID)
			return;
	}
#endif

	if (pkInstMain->IsGameMaster())
	{
	}
	else
	{
		if (pkInstMain->IsSameEmpire(*this))
			return;

		// HIDE_OTHER_EMPIRE_EUNHYEONG_ASSASSIN
		if (IsAffect(AFFECT_EUNHYEONG))
			return;
		// END_OF_HIDE_OTHER_EMPIRE_EUNHYEONG_ASSASSIN
	}

	if (IsGameMaster())
		return;
#ifdef ENABLE_LEADER_ON_GUILD
	if (m_kAffectFlagContainer.IsSet(AFF_GUILD))
		return;
#endif
	__EffectContainer_AttachEffect(EFFECT_EMPIRE+eEmpire);
}

void CInstanceBase::__AttachSelectEffect()
{
	__EffectContainer_AttachEffect(EFFECT_SELECT);
}

void CInstanceBase::__DetachSelectEffect()
{
	__EffectContainer_DetachEffect(EFFECT_SELECT);
}

void CInstanceBase::__AttachTargetEffect()
{
	__EffectContainer_AttachEffect(EFFECT_TARGET);
}

void CInstanceBase::__DetachTargetEffect()
{
	__EffectContainer_DetachEffect(EFFECT_TARGET);
}


void CInstanceBase::__StoneSmoke_Inialize()
{
	m_kStoneSmoke.m_dwEftID=0;
}

void CInstanceBase::__StoneSmoke_Destroy()
{
	if (!m_kStoneSmoke.m_dwEftID)
		return;

	__DetachEffect(m_kStoneSmoke.m_dwEftID);
	m_kStoneSmoke.m_dwEftID=0;
}

void CInstanceBase::__StoneSmoke_Create(DWORD eSmoke)
{
	m_kStoneSmoke.m_dwEftID=m_GraphicThingInstance.AttachSmokeEffect(eSmoke);
}

void CInstanceBase::SetAlpha(float fAlpha)
{
	__SetBlendRenderingMode();
	__SetAlphaValue(fAlpha);
}

bool CInstanceBase::UpdateDeleting()
{
	Update();
	Transform();

	IAbstractApplication& rApp=IAbstractApplication::GetSingleton();

	float fAlpha = __GetAlphaValue() - (rApp.GetGlobalElapsedTime() * 1.5f);
	__SetAlphaValue(fAlpha);

	if (fAlpha < 0.0f)
		return false;

	return true;
}

void CInstanceBase::DeleteBlendOut()
{
	__SetBlendRenderingMode();
	__SetAlphaValue(1.0f);
	DetachTextTail();

	IAbstractPlayer& rkPlayer=IAbstractPlayer::GetSingleton();
	rkPlayer.NotifyDeletingCharacterInstance(GetVirtualID());
}

void CInstanceBase::ClearPVPKeySystem()
{
	g_kSet_dwPVPReadyKey.clear();
	g_kSet_dwPVPKey.clear();
	g_kSet_dwGVGKey.clear();
	g_kSet_dwDUELKey.clear();
}

void CInstanceBase::InsertPVPKey(DWORD dwVIDSrc, DWORD dwVIDDst)
{
	DWORD dwPVPKey=__GetPVPKey(dwVIDSrc, dwVIDDst);

	g_kSet_dwPVPKey.insert(dwPVPKey);
}

void CInstanceBase::InsertPVPReadyKey(DWORD dwVIDSrc, DWORD dwVIDDst)
{
	DWORD dwPVPReadyKey=__GetPVPKey(dwVIDSrc, dwVIDDst);

	g_kSet_dwPVPKey.insert(dwPVPReadyKey);
}

void CInstanceBase::RemovePVPKey(DWORD dwVIDSrc, DWORD dwVIDDst)
{
	DWORD dwPVPKey=__GetPVPKey(dwVIDSrc, dwVIDDst);

	g_kSet_dwPVPKey.erase(dwPVPKey);
}

void CInstanceBase::InsertGVGKey(DWORD dwSrcGuildVID, DWORD dwDstGuildVID)
{
	DWORD dwGVGKey = __GetPVPKey(dwSrcGuildVID, dwDstGuildVID);
	g_kSet_dwGVGKey.insert(dwGVGKey);
}

void CInstanceBase::RemoveGVGKey(DWORD dwSrcGuildVID, DWORD dwDstGuildVID)
{
	DWORD dwGVGKey = __GetPVPKey(dwSrcGuildVID, dwDstGuildVID);
	g_kSet_dwGVGKey.erase(dwGVGKey);
}

void CInstanceBase::InsertDUELKey(DWORD dwVIDSrc, DWORD dwVIDDst)
{
	DWORD dwPVPKey=__GetPVPKey(dwVIDSrc, dwVIDDst);

	g_kSet_dwDUELKey.insert(dwPVPKey);
}

DWORD CInstanceBase::__GetPVPKey(DWORD dwVIDSrc, DWORD dwVIDDst)
{
	if (dwVIDSrc>dwVIDDst)
		std::swap(dwVIDSrc, dwVIDDst);

	DWORD awSrc[2];
	awSrc[0]=dwVIDSrc;
	awSrc[1]=dwVIDDst;

    const BYTE * s = (const BYTE *) awSrc;
    const BYTE * end = s + sizeof(awSrc);
    unsigned long h = 0;

    while (s < end)
    {
        h *= 16777619;
        h ^= (BYTE) *(BYTE *) (s++);
    }

    return h;
}

bool CInstanceBase::__FindPVPKey(DWORD dwVIDSrc, DWORD dwVIDDst)
{
	DWORD dwPVPKey=__GetPVPKey(dwVIDSrc, dwVIDDst);

	if (g_kSet_dwPVPKey.end()==g_kSet_dwPVPKey.find(dwPVPKey))
		return false;

	return true;
}

bool CInstanceBase::__FindPVPReadyKey(DWORD dwVIDSrc, DWORD dwVIDDst)
{
	DWORD dwPVPKey=__GetPVPKey(dwVIDSrc, dwVIDDst);

	if (g_kSet_dwPVPReadyKey.end()==g_kSet_dwPVPReadyKey.find(dwPVPKey))
		return false;

	return true;
}
//길드전시 상대 길드인지 확인할때.
bool CInstanceBase::__FindGVGKey(DWORD dwSrcGuildID, DWORD dwDstGuildID)
{
	DWORD dwGVGKey=__GetPVPKey(dwSrcGuildID, dwDstGuildID);

	if (g_kSet_dwGVGKey.end()==g_kSet_dwGVGKey.find(dwGVGKey))
		return false;

	return true;
}
//대련 모드에서는 대련 상대만 공격할 수 있다.
bool CInstanceBase::__FindDUELKey(DWORD dwVIDSrc, DWORD dwVIDDst)
{
	DWORD dwDUELKey=__GetPVPKey(dwVIDSrc, dwVIDDst);

	if (g_kSet_dwDUELKey.end()==g_kSet_dwDUELKey.find(dwDUELKey))
		return false;

	return true;
}

bool CInstanceBase::IsPVPInstance(CInstanceBase& rkInstSel)
{
	DWORD dwVIDSrc=GetVirtualID();
	DWORD dwVIDDst=rkInstSel.GetVirtualID();

	DWORD dwGuildIDSrc=GetGuildID();
	DWORD dwGuildIDDst=rkInstSel.GetGuildID();

	if (GetDuelMode())	//대련 모드일때는 ~_~
		return true;	

	return __FindPVPKey(dwVIDSrc, dwVIDDst) || __FindGVGKey(dwGuildIDSrc, dwGuildIDDst);
											//__FindDUELKey(dwVIDSrc, dwVIDDst);
}

const D3DXCOLOR& CInstanceBase::GetNameColor()
{
	return GetIndexedNameColor(GetNameColorIndex());
}

UINT CInstanceBase::GetNameColorIndex()
{
	if (IsPC())
	{
		if (m_isKiller)
		{
			return NAMECOLOR_PK;
		}

		if (__IsExistMainInstance() && !__IsMainInstance())
		{			
			CInstanceBase* pkInstMain=__GetMainInstancePtr();
			if (!pkInstMain)
			{
				TraceError("CInstanceBase::GetNameColorIndex - MainInstance is NULL");
				return NAMECOLOR_PC;
			}
			DWORD dwVIDMain=pkInstMain->GetVirtualID();
			DWORD dwVIDSelf=GetVirtualID();

			if (pkInstMain->GetDuelMode())
			{
				switch(pkInstMain->GetDuelMode())
				{
				case DUEL_CANNOTATTACK:
					return NAMECOLOR_PC + GetEmpireID();
				case DUEL_START:
					if(__FindDUELKey(dwVIDMain, dwVIDSelf))
						return NAMECOLOR_PVP;
					else
						return NAMECOLOR_PC + GetEmpireID();
				}
			}

			if (pkInstMain->IsSameEmpire(*this))
			{
				if (__FindPVPKey(dwVIDMain, dwVIDSelf))
				{
					return NAMECOLOR_PVP;
				}

				DWORD dwGuildIDMain=pkInstMain->GetGuildID();
				DWORD dwGuildIDSelf=GetGuildID();
				if (__FindGVGKey(dwGuildIDMain, dwGuildIDSelf))
				{
					return NAMECOLOR_PVP;
				}
				/*
				if (__FindDUELKey(dwVIDMain, dwVIDSelf))
				{
					return NAMECOLOR_PVP;
				}
				*/
			}
			else
			{
				return NAMECOLOR_PVP;
			}
		}

		IAbstractPlayer& rPlayer=IAbstractPlayer::GetSingleton();
		if (rPlayer.IsPartyMemberByVID(GetVirtualID()))
			return NAMECOLOR_PARTY;

		return NAMECOLOR_PC + GetEmpireID();
		
	}
	else if (IsNPC())
	{
		if (GetRace() >= 30000 && GetRace() <= 30007)
			return NAMECOLOR_OFFLINE_SHOP;
		else
			return NAMECOLOR_NPC;

	}
	else if (IsEnemy())
	{
		return NAMECOLOR_MOB;
	}
	else if (IsPoly())
	{
		return NAMECOLOR_MOB;
	}
	else if (IsStone())
	{
		return NAMECOLOR_METIN;
	}

	return D3DXCOLOR(0xffffffff);
}

const D3DXCOLOR& CInstanceBase::GetTitleColor()
{
	UINT uGrade = GetAlignmentGrade();
	if ( uGrade >= TITLE_NUM)
	{
		static D3DXCOLOR s_kD3DXClrTitleDefault(0xffffffff);
		return s_kD3DXClrTitleDefault;
	}

	return g_akD3DXClrTitle[uGrade];
}

#ifdef ENABLE_TITLE_SYSTEM
const D3DXCOLOR& CInstanceBase::GetTitlePrestigeColor()
{
	UINT uGradePrestige = GetPrestigeGrade();
	if ( uGradePrestige >= TITLE_NUM_PRESTIGE)
	{
		static D3DXCOLOR s_kD3DXClrTitlePrestigeDefault(0xffffffff);
		return s_kD3DXClrTitlePrestigeDefault;
	}

	return g_akD3DXClrTitlePrestige[uGradePrestige];
}
#endif

void CInstanceBase::AttachTextTail()
{
	if (m_isTextTail)
	{
		TraceError("CInstanceBase::AttachTextTail - VID [%d] ALREADY EXIST", GetVirtualID());
		return;
	}

	m_isTextTail=true;

	DWORD dwVID=GetVirtualID();

#if defined(ENABLE_RACE_HEIGHT)
	float fTextTailHeight = GetBaseHeight() + 10.0f;
#else
	float fTextTailHeight = IsMountingHorse() ? 110.0f : 10.0f;
#endif

	static D3DXCOLOR s_kD3DXClrTextTail=D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CPythonTextTail::Instance().RegisterCharacterTextTail(m_dwGuildID, dwVID, s_kD3DXClrTextTail, fTextTailHeight);

	// CHARACTER_LEVEL
	if (m_dwLevel)
	{
		UpdateTextTailLevel(m_dwLevel);
	}
}

void CInstanceBase::DetachTextTail()
{
	if (!m_isTextTail)
		return;

	m_isTextTail=false;
	CPythonTextTail::Instance().DeleteCharacterTextTail(GetVirtualID());
}

void CInstanceBase::UpdateTextTailLevel(DWORD level)
{


	#ifdef ENABLE_REBIRT_SYSTEM
	static D3DXCOLOR s_kLevelColorAmk[21] = {
		D3DXCOLOR(152.0f / 255.0f, 255.0f / 255.0f, 51.0f / 255.0f, 1.0f),
		//-----------------------------------------------------------//
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 1 - Sari
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 2 - Mavi
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 3 - Pembe
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 4 - Gri
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 5 - Yesil
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 6 - Turuncu
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 7 - Mor
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 8 - Buz Mavisi
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 9 - Altin Sarisi
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 10 - Kirmizi
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 11 - Sari
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 12 - Mavi
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 13 - Pembe
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 14 - Gri
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 15 - Yesil
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 16 - Turuncu
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 17 - Mor
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 18 - Buz Mavisi
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f),	//Rebirt 19 - Altin Sarisi
		D3DXCOLOR(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f)	//Rebirt 20 - Kirmizi
	};
	#endif
	if (IsPC())
	{
		if (GetRebirt() > 0)
		{
			char szText[256];
			sprintf(szText, "|cff00ccff LeveL %d ", GetRebirt());
			CPythonTextTail::Instance().AttachLevel(GetVirtualID(), szText, s_kLevelColorAmk[GetRebirt()]);
			return;
		}
		else
		{
			static D3DXCOLOR s_kLevelColor = D3DXCOLOR(152.0f/255.0f, 255.0f/255.0f, 51.0f/255.0f, 1.0f);
			char szText[256];
			sprintf(szText, "Lv %d", level);
			CPythonTextTail::Instance().AttachLevel(GetVirtualID(), szText, s_kLevelColor);		
		}
    }
	else if (GetRace() == 30000 || GetRace() == 30001 || GetRace() == 30002 || GetRace() == 30003 || GetRace() == 30004 || GetRace() == 30005 || GetRace() == 30006 || GetRace() == 30007)
	{
        static D3DXCOLOR s_kLevelColor = D3DXCOLOR(152.0f/255.0f, 255.0f/255.0f, 51.0f/255.0f, 1.0f);

        char szText[256];
		sprintf(szText, "Lv %d", level);
		CPythonTextTail::Instance().AttachLevel(GetVirtualID(), szText, s_kLevelColor);		
	}
#ifdef ENABLE_NEW_PET_SYSTEM
	else if (IsNewPet())
	{
		static D3DXCOLOR s_kPetLevelColor = D3DCOLOR_XRGB(255, 255,   0);
		char szText[256];
		sprintf(szText, "Lv %d", level);
		CPythonTextTail::Instance().AttachLevel(GetVirtualID(), szText, s_kPetLevelColor);
	}
#endif
}

void CInstanceBase::RefreshTextTail()
{
	CPythonTextTail::Instance().SetCharacterTextTailColor(GetVirtualID(), GetNameColor());

	int iAlignmentGrade = GetAlignmentGrade();
	if (TITLE_NONE == iAlignmentGrade)
	{
		CPythonTextTail::Instance().DetachTitle(GetVirtualID());
	}
	else
	{
		std::map<int, std::string>::iterator itor = g_TitleNameMap.find(iAlignmentGrade);
		if (g_TitleNameMap.end() != itor)
		{
			const std::string & c_rstrTitleName = itor->second;
			CPythonTextTail::Instance().AttachTitle(GetVirtualID(), c_rstrTitleName.c_str(), GetTitleColor());
		}
	}
#ifdef ENABLE_TITLE_SYSTEM
	int iPrestigeGrade = GetPrestigeGrade();
	
	if (TITLE_NONE_PRESTIGE == iPrestigeGrade)
	{
		CPythonTextTail::Instance().DetachPrestige_(GetVirtualID());
	}
	else
		{
		std::map<int, std::string>::iterator itor2_vegas = g_TitlePrestigeNameMap.find(iPrestigeGrade);
		if (g_TitlePrestigeNameMap.end() != itor2_vegas)
		{
			const std::string & c_rstrTitlePrestigeName = itor2_vegas->second;
			CPythonTextTail::Instance().AttachPrestige_(GetVirtualID(), c_rstrTitlePrestigeName.c_str(), GetTitlePrestigeColor());
		}
	}
#endif
}

void CInstanceBase::RefreshTextTailTitle()
{
	RefreshTextTail();
}

// 2004.07.25.myevan.이펙트 안 붙는 문제 해결
/////////////////////////////////////////////////
void CInstanceBase::__ClearAffectFlagContainer()
{
	m_kAffectFlagContainer.Clear();
}

void CInstanceBase::__ClearAffects()
{
	#ifdef ENABLE_MELEY_LAIR_DUNGEON
	if ((IsStone()) && (GetVirtualNumber() != MELEY_LAIR_DUNGEON_STATUE))
	#else
	if (IsStone())
	#endif
	{
		__StoneSmoke_Destroy();
	}
	else
	{
		for (int iAffect=0; iAffect<AFFECT_NUM; ++iAffect)
		{
			__DetachEffect(m_adwCRCAffectEffect[iAffect]);
			m_adwCRCAffectEffect[iAffect]=0;
		}

		__ClearAffectFlagContainer();
	}

	m_GraphicThingInstance.__OnClearAffects();
}

/////////////////////////////////////////////////

void CInstanceBase::__SetNormalAffectFlagContainer(const CAffectFlagContainer& c_rkAffectFlagContainer)
{
	for (int i=0; i<CAffectFlagContainer::BIT_SIZE; ++i)
	{
		bool isOldSet=m_kAffectFlagContainer.IsSet(i);
		bool isNewSet=c_rkAffectFlagContainer.IsSet(i);

		if (isOldSet != isNewSet)
		{
			__SetAffect(i, isNewSet);

			if (isNewSet)
				m_GraphicThingInstance.__OnSetAffect(i);
			else
				m_GraphicThingInstance.__OnResetAffect(i);
		}
	}

	m_kAffectFlagContainer.CopyInstance(c_rkAffectFlagContainer);
}

void CInstanceBase::__SetStoneSmokeFlagContainer(const CAffectFlagContainer& c_rkAffectFlagContainer)
{
	m_kAffectFlagContainer.CopyInstance(c_rkAffectFlagContainer);

	DWORD eSmoke;
	if (m_kAffectFlagContainer.IsSet(STONE_SMOKE8))
		eSmoke=3;
	else if (m_kAffectFlagContainer.IsSet(STONE_SMOKE5)|m_kAffectFlagContainer.IsSet(STONE_SMOKE6)|m_kAffectFlagContainer.IsSet(STONE_SMOKE7))
		eSmoke=2;
	else if (m_kAffectFlagContainer.IsSet(STONE_SMOKE2)|m_kAffectFlagContainer.IsSet(STONE_SMOKE3)|m_kAffectFlagContainer.IsSet(STONE_SMOKE4))
		eSmoke=1;
	else
		eSmoke=0;

	__StoneSmoke_Destroy();
	__StoneSmoke_Create(eSmoke);
}

void CInstanceBase::SetAffectFlagContainer(const CAffectFlagContainer& c_rkAffectFlagContainer)
{
	if (IsBuilding())
	{
		return;		
	}
	#ifdef ENABLE_MELEY_LAIR_DUNGEON
	else if ((IsStone()) && (GetVirtualNumber() != MELEY_LAIR_DUNGEON_STATUE))
	#else
	else if (IsStone())
	#endif
	{
		__SetStoneSmokeFlagContainer(c_rkAffectFlagContainer);
	}
	else
	{
		__SetNormalAffectFlagContainer(c_rkAffectFlagContainer);
	}
}


void CInstanceBase::SCRIPT_SetAffect(UINT eAffect, bool isVisible)
{
	__SetAffect(eAffect, isVisible);
}

void CInstanceBase::__SetReviveInvisibilityAffect(bool isVisible)
{
	if (isVisible)
	{
		// NOTE : Dress 를 입고 있으면 Alpha 를 넣지 않는다.
		if (IsWearingDress())
			return;

		m_GraphicThingInstance.BlendAlphaValue(0.5f, 1.0f);
	}
	else
	{
		m_GraphicThingInstance.BlendAlphaValue(1.0f, 1.0f);	
	}
}

void CInstanceBase::__Assassin_SetEunhyeongAffect(bool isVisible)
{
	if (isVisible)
	{
		// NOTE : Dress 를 입고 있으면 Alpha 를 넣지 않는다.
		if (IsWearingDress())
			return;

		if (__IsMainInstance() || __MainCanSeeHiddenThing())
		{
			m_GraphicThingInstance.BlendAlphaValue(0.5f, 1.0f);
		}
		else
		{
			// 2004.10.16.myevan.은형법 완전 투명
			m_GraphicThingInstance.BlendAlphaValue(0.0f, 1.0f);
			m_GraphicThingInstance.HideAllAttachingEffect();
		}
	}
	else
	{
		m_GraphicThingInstance.BlendAlphaValue(1.0f, 1.0f);	
		m_GraphicThingInstance.ShowAllAttachingEffect();
	}
}

void CInstanceBase::__Shaman_SetParalysis(bool isParalysis)
{
	m_GraphicThingInstance.SetParalysis(isParalysis);
}



void CInstanceBase::__Warrior_SetGeomgyeongAffect(bool isVisible)
{
	if (isVisible)
	{
		if (IsWearingDress())
			return;

		if (m_kWarrior.m_dwGeomgyeongEffect)
			__DetachEffect(m_kWarrior.m_dwGeomgyeongEffect);

		m_GraphicThingInstance.SetReachScale(1.5f);
		if (m_GraphicThingInstance.IsTwoHandMode())
			m_kWarrior.m_dwGeomgyeongEffect=__AttachEffect(EFFECT_WEAPON+WEAPON_TWOHAND);
		else
			m_kWarrior.m_dwGeomgyeongEffect=__AttachEffect(EFFECT_WEAPON+WEAPON_ONEHAND);
	}
	else
	{
		m_GraphicThingInstance.SetReachScale(1.0f);

		__DetachEffect(m_kWarrior.m_dwGeomgyeongEffect);
		m_kWarrior.m_dwGeomgyeongEffect=0;
	}
}

void CInstanceBase::__SetAffect(UINT eAffect, bool isVisible)
{
	switch (eAffect)
	{
		case AFFECT_YMIR:
			if (IsAffect(AFFECT_INVISIBILITY))
				return;
			break;
/*
		case AFFECT_GWIGEOM: // 전기 속성 공격으로 바뀔 예정
			if (isVisible)
			{
				m_GraphicThingInstance.SetBattleHitEffect(ms_adwCRCAffectEffect[EFFECT_ELECTRIC_HIT]);
				m_GraphicThingInstance.SetBattleAttachEffect(ms_adwCRCAffectEffect[EFFECT_ELECTRIC_ATTACH]);
			}
			else
			{
				m_GraphicThingInstance.SetBattleHitEffect(ms_adwCRCAffectEffect[EFFECT_HIT]);
				m_GraphicThingInstance.SetBattleAttachEffect(0);
			}
			return;
			break;
		case AFFECT_HWAYEOM: // 화염 속성 공격으로 바뀔 예정
			if (isVisible)
			{
				m_GraphicThingInstance.SetBattleHitEffect(ms_adwCRCAffectEffect[EFFECT_FLAME_HIT]);
				m_GraphicThingInstance.SetBattleAttachEffect(ms_adwCRCAffectEffect[EFFECT_FLAME_ATTACH]);
			}
			else
			{
				m_GraphicThingInstance.SetBattleHitEffect(ms_adwCRCAffectEffect[EFFECT_HIT]);
				m_GraphicThingInstance.SetBattleAttachEffect(0);
			}
			// 화염참은 공격할 때만 일시적으로 Visible 합니다.
			return;
			break;
*/
		case AFFECT_CHEONGEUN:
			m_GraphicThingInstance.SetResistFallen(isVisible);
			break;
		case AFFECT_GEOMGYEONG:
			__Warrior_SetGeomgyeongAffect(isVisible);
			return;
			break;
		case AFFECT_REVIVE_INVISIBILITY:
			__Assassin_SetEunhyeongAffect(isVisible);
			break;
		case AFFECT_EUNHYEONG:
			__Assassin_SetEunhyeongAffect(isVisible);
			break;
		case AFFECT_GYEONGGONG:
		case AFFECT_KWAESOK:
			// 경공술, 쾌속은 뛸때만 Attaching 시킵니다. - [levites]
			if (isVisible)
				if (!IsWalking())
					return;
			break;
		case AFFECT_INVISIBILITY:
			// 2004.07.17.levites.isShow를 ViewFrustumCheck로 변경
			if (isVisible)
			{
				m_GraphicThingInstance.ClearAttachingEffect();
				__EffectContainer_Destroy();
				DetachTextTail();
			}
			else
			{
				m_GraphicThingInstance.BlendAlphaValue(1.0f, 1.0f);
				AttachTextTail();
				RefreshTextTail();
			}
			return;
			break;
//		case AFFECT_FAINT:
//			m_GraphicThingInstance.SetFaint(isVisible);
//			break;
//		case AFFECT_SLEEP:
//			m_GraphicThingInstance.SetSleep(isVisible);
//			break;
		case AFFECT_STUN:
			m_GraphicThingInstance.SetSleep(isVisible);
			break;
	}

	if (eAffect>=AFFECT_NUM)
	{
		TraceError("CInstanceBase[VID:%d]::SetAffect(eAffect:%d<AFFECT_NUM:%d, isVisible=%d)", GetVirtualID(), eAffect, isVisible);
		return;
	}

	if (isVisible)
	{
		if (!m_adwCRCAffectEffect[eAffect])
		{
			m_adwCRCAffectEffect[eAffect]=__AttachEffect(EFFECT_AFFECT+eAffect);
		}
	}
	else
	{
		if (m_adwCRCAffectEffect[eAffect])
		{
			__DetachEffect(m_adwCRCAffectEffect[eAffect]);
			m_adwCRCAffectEffect[eAffect]=0;
		}
	}
}

bool CInstanceBase::IsPossibleEmoticon()
{
	CEffectManager& rkEftMgr=CEffectManager::Instance();
	for(DWORD eEmoticon = 0; eEmoticon < EMOTICON_NUM; eEmoticon++)
	{
		DWORD effectID = ms_adwCRCAffectEffect[EFFECT_EMOTICON+eEmoticon];
		if( effectID &&	rkEftMgr.IsAliveEffect(effectID) )
			return false;
	}

	if(ELTimer_GetMSec() - m_dwEmoticonTime < 1000)
	{
		//TraceError("ELTimer_GetMSec() - m_dwEmoticonTime");
		return false;
	}

	return true;
}

void CInstanceBase::SetFishEmoticon()
{
	SetEmoticon(EMOTICON_FISH);
}

void CInstanceBase::SetEmoticon(UINT eEmoticon)
{
	if (eEmoticon>=EMOTICON_NUM)
	{
		TraceError("CInstanceBase[VID:%d]::SetEmoticon(eEmoticon:%d<EMOTICON_NUM:%d, isVisible=%d)",
			GetVirtualID(), eEmoticon);
		return;
	}
	if (IsPossibleEmoticon())
	{
		D3DXVECTOR3 v3Pos = m_GraphicThingInstance.GetPosition();
#if defined(ENABLE_RACE_HEIGHT)
		v3Pos.z += float(GetBaseHeight() + m_GraphicThingInstance.GetHeight());
#else
		v3Pos.z += float(m_GraphicThingInstance.GetHeight());
#endif

		//CEffectManager& rkEftMgr=CEffectManager::Instance();
		CCamera * pCamera = CCameraManager::Instance().GetCurrentCamera();
		
		D3DXVECTOR3 v3Dir = (pCamera->GetEye()-v3Pos)*9/10;	
		v3Pos = pCamera->GetEye()-v3Dir;

		v3Pos = D3DXVECTOR3(0,0,0);
#if defined(ENABLE_RACE_HEIGHT)
		v3Pos.z += float(GetBaseHeight() + m_GraphicThingInstance.GetHeight());
#else
		v3Pos.z += float(m_GraphicThingInstance.GetHeight());
#endif

		//rkEftMgr.CreateEffect(ms_adwCRCAffectEffect[EFFECT_EMOTICON+eEmoticon],v3Pos,D3DXVECTOR3(0,0,0));
		m_GraphicThingInstance.AttachEffectByID(0, NULL, ms_adwCRCAffectEffect[EFFECT_EMOTICON+eEmoticon],&v3Pos);
		m_dwEmoticonTime = ELTimer_GetMSec();
	}
}

void CInstanceBase::SetDustGap(float fDustGap)
{
	ms_fDustGap=fDustGap;
}

void CInstanceBase::SetHorseDustGap(float fDustGap)
{
	ms_fHorseDustGap=fDustGap;
}

void CInstanceBase::__DetachEffect(DWORD dwEID)
{
	m_GraphicThingInstance.DettachEffect(dwEID);
}

DWORD CInstanceBase::__AttachEffect(UINT eEftType)
{
#ifdef ENABLE_SKILL_COLOR_SYSTEM
	switch (eEftType)
	{
		case EFFECT_AFFECT + AFFECT_GONGPO:
			RegisterEffect(eEftType, "", "d:/ymir work/pc/sura/effect/fear_loop.mse", false, GetNameString());
			break;
		case EFFECT_AFFECT + AFFECT_JUMAGAP:
			RegisterEffect(eEftType, "", "d:/ymir work/pc/sura/effect/jumagap_loop.mse", false, GetNameString());
			break;
		case EFFECT_AFFECT + AFFECT_HOSIN:
			RegisterEffect(eEftType, "", "d:/ymir work/pc/shaman/effect/3hosin_loop.mse", false, GetNameString());
			break;
		case EFFECT_AFFECT + AFFECT_BOHO:
			RegisterEffect(eEftType, "", "d:/ymir work/pc/shaman/effect/boho_loop.mse", false, GetNameString());
			break;
		case EFFECT_AFFECT + AFFECT_KWAESOK:
			RegisterEffect(eEftType, "", "d:/ymir work/pc/shaman/effect/10kwaesok_loop.mse", false, GetNameString());
			break;
		case EFFECT_AFFECT + AFFECT_HEUKSIN:
			RegisterEffect(eEftType, "", "d:/ymir work/pc/sura/effect/heuksin_loop.mse", false, GetNameString());
			break;
		case EFFECT_AFFECT + AFFECT_MUYEONG:
			RegisterEffect(eEftType, "", "d:/ymir work/pc/sura/effect/muyeong_loop.mse", false, GetNameString());
			break;
		case EFFECT_AFFECT + AFFECT_FIRE:
			RegisterEffect(eEftType, "Bip01", "d:/ymir work/effect/hit/blow_flame/flame_loop.mse", false, GetNameString());
			break;
		case EFFECT_AFFECT + AFFECT_GICHEON:
			RegisterEffect(eEftType, "Bip01 R Hand", "d:/ymir work/pc/shaman/effect/6gicheon_hand.mse", false, GetNameString());
			break;
		case EFFECT_AFFECT + AFFECT_JEUNGRYEOK:
			RegisterEffect(eEftType, "Bip01 L Hand", "d:/ymir work/pc/shaman/effect/jeungryeok_hand.mse", false, GetNameString());
			break;
		case EFFECT_AFFECT + AFFECT_PABEOP:
			RegisterEffect(eEftType, "Bip01 Head", "d:/ymir work/pc/sura/effect/pabeop_loop.mse", false, GetNameString());
			break;
		case EFFECT_AFFECT + AFFECT_CHEONGEUN:
		case EFFECT_AFFECT + AFFECT_FALLEN_CHEONGEUN:
			RegisterEffect(eEftType, "", "d:/ymir work/pc/warrior/effect/gyeokgongjang_loop.mse", false, GetNameString());
			break;
		case EFFECT_AFFECT + AFFECT_GWIGEOM:
			RegisterEffect(eEftType, "Bip01 R Finger2", "d:/ymir work/pc/sura/effect/gwigeom_loop.mse", false, GetNameString());
			break;
		case EFFECT_AFFECT + AFFECT_GYEONGGONG:
			RegisterEffect(eEftType, "", "d:/ymir work/pc/assassin/effect/gyeonggong_loop.mse", false, GetNameString());
			break;
		case EFFECT_WEAPON + WEAPON_ONEHAND:
				RegisterEffect(eEftType, "equip_right_hand", "d:/ymir work/pc/warrior/effect/geom_sword_loop.mse", false, GetNameString());
				break;
		case EFFECT_WEAPON + WEAPON_TWOHAND:
			RegisterEffect(eEftType, "equip_right_hand", "d:/ymir work/pc/warrior/effect/geom_spear_loop.mse", false, GetNameString());
			break;
		default:
			break;
	}
#endif
	// 2004.07.17.levites.isShow를 ViewFrustumCheck로 변경
	if (IsAffect(AFFECT_INVISIBILITY))
		return 0;

	if (eEftType>=EFFECT_NUM)
		return 0;
#ifdef ENABLE_SKILL_COLOR_SYSTEM
	DWORD * dwSkillColor = m_GraphicThingInstance.GetSkillColorByEffectID(eEftType);
#endif
	if (ms_astAffectEffectAttachBone[eEftType].empty())
	{
#ifdef ENABLE_SKILL_COLOR_SYSTEM
		return m_GraphicThingInstance.AttachEffectByID(0, NULL, ms_adwCRCAffectEffect[eEftType], NULL, dwSkillColor);
#else
		return m_GraphicThingInstance.AttachEffectByID(0, NULL, ms_adwCRCAffectEffect[eEftType]);
#endif
	}
	else
	{
		std::string & rstrBoneName = ms_astAffectEffectAttachBone[eEftType];
		const char * c_szBoneName;
		// 양손에 붙일 때 사용한다.
		// 이런 식의 예외 처리를 해놓은 것은 캐릭터 마다 Equip 의 Bone Name 이 다르기 때문.
		if (0 == rstrBoneName.compare("PART_WEAPON"))
		{
			if (m_GraphicThingInstance.GetAttachingBoneName(CRaceData::PART_WEAPON, &c_szBoneName))
			{
#ifdef ENABLE_SKILL_COLOR_SYSTEM
				return m_GraphicThingInstance.AttachEffectByID(0, c_szBoneName, ms_adwCRCAffectEffect[eEftType], NULL, dwSkillColor);
#else
				return m_GraphicThingInstance.AttachEffectByID(0, c_szBoneName, ms_adwCRCAffectEffect[eEftType]);
#endif
			}
		}
		else if (0 == rstrBoneName.compare("PART_WEAPON_LEFT"))
		{
			if (m_GraphicThingInstance.GetAttachingBoneName(CRaceData::PART_WEAPON_LEFT, &c_szBoneName))
			{
#ifdef ENABLE_SKILL_COLOR_SYSTEM
				return m_GraphicThingInstance.AttachEffectByID(0, c_szBoneName, ms_adwCRCAffectEffect[eEftType], NULL, dwSkillColor);
#else
				return m_GraphicThingInstance.AttachEffectByID(0, c_szBoneName, ms_adwCRCAffectEffect[eEftType]);
#endif
			}
		}
		else
		{
#ifdef ENABLE_SKILL_COLOR_SYSTEM
			return m_GraphicThingInstance.AttachEffectByID(0, rstrBoneName.c_str(), ms_adwCRCAffectEffect[eEftType], NULL, dwSkillColor);
#else
			return m_GraphicThingInstance.AttachEffectByID(0, rstrBoneName.c_str(), ms_adwCRCAffectEffect[eEftType]);
#endif
		}
	}

	return 0;
}

void CInstanceBase::__ComboProcess()
{
	/*
	DWORD dwcurComboIndex = m_GraphicThingInstance.GetComboIndex();

	if (0 != dwcurComboIndex)
	{
		if (m_dwLastComboIndex != m_GraphicThingInstance.GetComboIndex())
		{
			if (!m_GraphicThingInstance.IsHandMode() & IsAffect(AFFECT_HWAYEOM))
			{
				__AttachEffect(EFFECT_FLAME_ATTACK);
			}
		}
	}

	m_dwLastComboIndex = dwcurComboIndex;
	*/
}

#ifdef ENABLE_SKILL_COLOR_SYSTEM
bool CInstanceBase::RegisterEffect(UINT eEftType, const char* c_szEftAttachBone, const char* c_szEftName, bool isCache, const char * name)
#else
bool CInstanceBase::RegisterEffect(UINT eEftType, const char* c_szEftAttachBone, const char* c_szEftName, bool isCache)
#endif
{
	if (eEftType>=EFFECT_NUM)
		return false;

	ms_astAffectEffectAttachBone[eEftType]=c_szEftAttachBone;

	DWORD& rdwCRCEft=ms_adwCRCAffectEffect[eEftType];
#ifdef ENABLE_SKILL_COLOR_SYSTEM
	if (!CEffectManager::Instance().RegisterEffect2(c_szEftName, &rdwCRCEft, isCache, name))
#else
	if (!CEffectManager::Instance().RegisterEffect2(c_szEftName, &rdwCRCEft, isCache))
#endif
	{
		TraceError("CInstanceBase::RegisterEffect(eEftType=%d, c_szEftAttachBone=%s, c_szEftName=%s, isCache=%d) - Error", eEftType, c_szEftAttachBone, c_szEftName, isCache);
		rdwCRCEft=0;
		return false;
	}

	return true;
}

void CInstanceBase::RegisterTitleName(int iIndex, const char * c_szTitleName)
{
	g_TitleNameMap.insert(make_pair(iIndex, c_szTitleName));
}

#ifdef ENABLE_TITLE_SYSTEM
void CInstanceBase::RegisterTitlePrestigeName(int iIndex, const char * c_szTitlePrestigeName)
{
	g_TitlePrestigeNameMap.insert(make_pair(iIndex, c_szTitlePrestigeName));
}
#endif

D3DXCOLOR __RGBToD3DXColoru(UINT r, UINT g, UINT b)
{
	DWORD dwColor=0xff;dwColor<<=8;
	dwColor|=r;dwColor<<=8;
	dwColor|=g;dwColor<<=8;
	dwColor|=b;

	return D3DXCOLOR(dwColor);
}

bool CInstanceBase::RegisterNameColor(UINT uIndex, UINT r, UINT g, UINT b)
{
	if (uIndex>=NAMECOLOR_NUM)
		return false;

	g_akD3DXClrName[uIndex]=__RGBToD3DXColoru(r, g, b);
	return true;
}

bool CInstanceBase::RegisterTitleColor(UINT uIndex, UINT r, UINT g, UINT b)
{
	if (uIndex>=TITLE_NUM)
		return false;

	g_akD3DXClrTitle[uIndex]=__RGBToD3DXColoru(r, g, b);
	return true;	
}

#ifdef ENABLE_TITLE_SYSTEM
bool CInstanceBase::RegisterTitlePrestigeColor(UINT uIndex, UINT r, UINT g, UINT b)
{
	if (uIndex>=TITLE_NUM_PRESTIGE)
		return false;

	g_akD3DXClrTitlePrestige[uIndex]=__RGBToD3DXColoru(r, g, b);
	return true;	
}
#endif