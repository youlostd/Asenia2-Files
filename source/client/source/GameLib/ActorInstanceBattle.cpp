#include "StdAfx.h"
#include "../effectLib/EffectManager.h"
#include "../milesLib/SoundManager.h"
#include "ActorInstance.h"
#include "RaceData.h"

void CActorInstance::SetBattleHitEffect(DWORD dwID)
{
	m_dwBattleHitEffectID = dwID;
}

void CActorInstance::SetBattleAttachEffect(DWORD dwID)
{
	m_dwBattleAttachEffectID = dwID;
}

bool CActorInstance::CanAct()
{
	if (IsDead())
		return false;

	if (IsStun())
		return false;

	if (IsParalysis())
		return false;

	if (IsFaint())
		return false;

	if (IsSleep())
		return false;

	if (IsKnockDown()){ return false; }
	if (__IsStandUpMotion()){ return false; }

	return true;
}

bool CActorInstance::CanUseSkill()
{
	if (!CanAct())
		return false;

	DWORD dwCurMotionIndex=__GetCurrentMotionIndex();
	
	// Locked during attack
	switch (dwCurMotionIndex)
	{
		case CRaceMotionData::NAME_FISHING_THROW:
		case CRaceMotionData::NAME_FISHING_WAIT:
		case CRaceMotionData::NAME_FISHING_STOP:
		case CRaceMotionData::NAME_FISHING_REACT:
		case CRaceMotionData::NAME_FISHING_CATCH:
		case CRaceMotionData::NAME_FISHING_FAIL:
			return TRUE;
			break;
	}

	// Locked during using skill
	if (IsUsingSkill())
	{
		if (m_pkCurRaceMotionData->IsCancelEnableSkill())
			return TRUE;

		return FALSE;
	}	
	
	return true;
}

bool CActorInstance::CanMove()
{
	if (!CanAct())
		return false;

	if (isLock())
		return false;

	return true;
}

bool CActorInstance::CanAttack()
{
	if (!CanAct())
		return false;

	if (IsUsingSkill())
	{
		if (!CanCancelSkill())
			return false;
	}

	return true;
}

bool CActorInstance::CanFishing()
{
	if (!CanAct())
		return false;

	if (IsUsingSkill())
		return false;

	switch (__GetCurrentMotionIndex())
	{
		case CRaceMotionData::NAME_WAIT:
		case CRaceMotionData::NAME_WALK:
		case CRaceMotionData::NAME_RUN:
			break;
		default:
			return false;
			break;
	}

	return true;
}

BOOL CActorInstance::IsClickableDistanceDestInstance(CActorInstance & rkInstDst, float fDistance)
{
	TPixelPosition kPPosSrc;
	GetPixelPosition(&kPPosSrc);

	D3DXVECTOR3 kD3DVct3Src(kPPosSrc);

	TCollisionPointInstanceList& rkLstkDefPtInst=rkInstDst.m_DefendingPointInstanceList;
	TCollisionPointInstanceList::iterator i;

	for (i=rkLstkDefPtInst.begin(); i!=rkLstkDefPtInst.end(); ++i)
	{
		CDynamicSphereInstanceVector& rkVctkDefSphere = (*i).SphereInstanceVector;

		CDynamicSphereInstanceVector::iterator j;
		for (j=rkVctkDefSphere.begin(); j!=rkVctkDefSphere.end(); ++j)
		{
			CDynamicSphereInstance& rkSphere=(*j);

			float fMovDistance=D3DXVec3Length(&D3DXVECTOR3(rkSphere.v3Position-kD3DVct3Src));
			float fAtkDistance=rkSphere.fRadius+fDistance;

			if (fAtkDistance>fMovDistance)
				return TRUE;
		}
	}

	return FALSE;
}

void CActorInstance::InputNormalAttackCommand(float fDirRot)
{
	if (!__CanInputNormalAttackCommand())
		return;

	m_fAtkDirRot=fDirRot;
	NormalAttack(m_fAtkDirRot);
}

bool CActorInstance::InputComboAttackCommand(float fDirRot)
{
	m_fAtkDirRot=fDirRot;

	if (m_isPreInput)
		return false;

	/////////////////////////////////////////////////////////////////////////////////

	// Process Input
  	if (0 == m_dwcurComboIndex)
	{
 		__RunNextCombo();
		return true;
	}
	else if (m_pkCurRaceMotionData->IsComboInputTimeData())
	{
		// ���� ��� �ð�
 		float fElapsedTime = GetAttackingElapsedTime();	

		// �̹� �Է� �Ѱ� �ð��� �����ٸ�..
		if (fElapsedTime > m_pkCurRaceMotionData->GetComboInputEndTime())
		{
			//Tracen("�Է� �Ѱ� �ð� ����");
			if (IsBowMode())
				m_isNextPreInput = TRUE;
			return false;
		}

		if (fElapsedTime > m_pkCurRaceMotionData->GetNextComboTime()) // �޺� �ߵ� �ð� �� �Ķ��
		{
			//Tracen("���� �޺� ����");
			// args : BlendingTime
			__RunNextCombo();
			return true;
		}
		else if (fElapsedTime > m_pkCurRaceMotionData->GetComboInputStartTime()) // �� �Է� �ð� ���� ���..
		{
			//Tracen("�� �Է� ����");
			m_isPreInput = TRUE;
			return false;
		}
	}
	else
	{
		float fElapsedTime = GetAttackingElapsedTime();	
		if (fElapsedTime > m_pkCurRaceMotionData->GetMotionDuration()*0.9f) // �޺� �ߵ� �ð� �� �Ķ��
		{
			//Tracen("���� �޺� ����");
			// args : BlendingTime
			__RunNextCombo();
			return true;
		}
	}
	// Process Input

	return false;
}

void CActorInstance::ComboProcess()
{
	// If combo is on action
	if (0 != m_dwcurComboIndex)
	{
		if (!m_pkCurRaceMotionData)
		{
			Tracef("Attacking motion data is NULL! : %d\n", m_dwcurComboIndex);
			__ClearCombo();
			return;
		}

		float fElapsedTime = GetAttackingElapsedTime();

		// Process PreInput
		if (m_isPreInput)
		{
			//Tracenf("���Է� %f �����޺��ð� %f", fElapsedTime, m_pkCurRaceMotionData->GetNextComboTime());
			if (fElapsedTime > m_pkCurRaceMotionData->GetNextComboTime())
			{
  				__RunNextCombo();
				m_isPreInput = FALSE;

				return;
			}
		}
	}
	else
	{
		m_isPreInput = FALSE;

		if (!IsUsingSkill())	// m_isNextPreInput�� Ȱ��� �϶��� ����ϴ� ����
		if (m_isNextPreInput)	// Ȱ�϶��� ��ų�� ĵ�� �Ǵ°� �̰� ������
		{
			__RunNextCombo();
			m_isNextPreInput = FALSE;
		}
	}
}

void CActorInstance::__RunNextCombo()
{
 	++m_dwcurComboIndex;
	///////////////////////////

	WORD wComboIndex = m_dwcurComboIndex;
	WORD wComboType = __GetCurrentComboType();

	if (wComboIndex==0)
	{
		TraceError("CActorInstance::__RunNextCombo(wComboType=%d, wComboIndex=%d)", wComboType, wComboIndex);
		return;
	}

	DWORD dwComboArrayIndex = wComboIndex - 1;

	CRaceData::TComboData * pComboData;

	if (!m_pkCurRaceData->GetComboDataPointer(m_wcurMotionMode, wComboType, &pComboData))
	{
		TraceError("CActorInstance::__RunNextCombo(wComboType=%d, wComboIndex=%d) - m_pkCurRaceData->GetComboDataPointer(m_wcurMotionMode=%d, &pComboData) == NULL", 
			wComboType, wComboIndex, m_wcurMotionMode);
		return;
	}

	if (dwComboArrayIndex >= pComboData->ComboIndexVector.size())
	{
		TraceError("CActorInstance::__RunNextCombo(wComboType=%d, wComboIndex=%d) - (dwComboArrayIndex=%d) >= (pComboData->ComboIndexVector.size()=%d)", 
			wComboType, wComboIndex, dwComboArrayIndex, pComboData->ComboIndexVector.size());
		return;
	}

	WORD wcurComboMotionIndex = pComboData->ComboIndexVector[dwComboArrayIndex];
	ComboAttack(wcurComboMotionIndex, m_fAtkDirRot, 0.1f);

	////////////////////////////////
	// �޺��� �����ٸ�
	if (m_dwcurComboIndex == pComboData->ComboIndexVector.size())
	{
		__OnEndCombo();
	}
}

void CActorInstance::__OnEndCombo()
{
	if (__IsMountingHorse())
	{
		m_dwcurComboIndex = 1;
	}

	// ���⼭ �޺��� �ʱ�ȭ �ؼ� �ȵȴ�.
	// �޺��� �ʱ�ȭ �Ǵ� ���� ������ �޺��� ������ Motion �� �ڵ����� Wait ���� ���ư��� �����̴�.
}

void CActorInstance::__ClearCombo()
{
	m_dwcurComboIndex = 0;
	m_isPreInput = FALSE;
	m_pkCurRaceMotionData = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CActorInstance::isAttacking()
{
	if (isNormalAttacking())
		return TRUE;

	if (isComboAttacking())
		return TRUE;

	if (IsSplashAttacking())
		return TRUE;

	return FALSE;
}

BOOL CActorInstance::isValidAttacking()
{
	if (!m_pkCurRaceMotionData)
		return FALSE;

	if (!m_pkCurRaceMotionData->isAttackingMotion())
		return FALSE;

	const NRaceData::TMotionAttackData * c_pData = m_pkCurRaceMotionData->GetMotionAttackDataPointer();
	float fElapsedTime = GetAttackingElapsedTime();
	NRaceData::THitDataContainer::const_iterator itor = c_pData->HitDataContainer.begin();
	for (; itor != c_pData->HitDataContainer.end(); ++itor)
	{
		const NRaceData::THitData & c_rHitData = *itor;
		if (fElapsedTime > c_rHitData.fAttackStartTime &&
			fElapsedTime < c_rHitData.fAttackEndTime)
			return TRUE;
	}

	return TRUE;
}

BOOL CActorInstance::CanCheckAttacking()
{
	if (isAttacking())
		return true;

	return false;
}

bool CActorInstance::__IsInSplashTime()
{
	if (m_kSplashArea.fDisappearingTime>GetLocalTime())
		return true;

	return false;
}

BOOL CActorInstance::isNormalAttacking()
{
	if (!m_pkCurRaceMotionData)
		return FALSE;

	if (!m_pkCurRaceMotionData->isAttackingMotion())
		return FALSE;

	const NRaceData::TMotionAttackData * c_pData = m_pkCurRaceMotionData->GetMotionAttackDataPointer();
	if (NRaceData::MOTION_TYPE_NORMAL != c_pData->iMotionType)
		return FALSE;

	return TRUE;
}

BOOL CActorInstance::isComboAttacking()
{
	if (!m_pkCurRaceMotionData)
		return FALSE;

	if (!m_pkCurRaceMotionData->isAttackingMotion())
		return FALSE;

	const NRaceData::TMotionAttackData * c_pData = m_pkCurRaceMotionData->GetMotionAttackDataPointer();
	if (NRaceData::MOTION_TYPE_COMBO != c_pData->iMotionType)
		return FALSE;

	return TRUE;
}

BOOL CActorInstance::IsSplashAttacking()
{
	if (!m_pkCurRaceMotionData)
		return FALSE;

	if (m_pkCurRaceMotionData->HasSplashMotionEvent())
		return TRUE;

	return FALSE;
}

BOOL CActorInstance::__IsMovingSkill(WORD wSkillNumber)
{
	enum
	{
		HORSE_DASH_SKILL_NUMBER = 137,
	};

	return HORSE_DASH_SKILL_NUMBER == wSkillNumber;
}

BOOL CActorInstance::IsActEmotion()
{
	DWORD dwCurMotionIndex=__GetCurrentMotionIndex();
	switch (dwCurMotionIndex)
	{
		case CRaceMotionData::NAME_FRENCH_KISS_START+0:
		case CRaceMotionData::NAME_FRENCH_KISS_START+1:
		case CRaceMotionData::NAME_FRENCH_KISS_START+2:
		case CRaceMotionData::NAME_FRENCH_KISS_START+3:
		case CRaceMotionData::NAME_FRENCH_KISS_START+4:
		case CRaceMotionData::NAME_KISS_START+0:
		case CRaceMotionData::NAME_KISS_START+1:
		case CRaceMotionData::NAME_KISS_START+2:
		case CRaceMotionData::NAME_KISS_START+3:
		case CRaceMotionData::NAME_KISS_START+4:
			return TRUE;
			break;
	}

	return FALSE;
}

BOOL CActorInstance::IsUsingMovingSkill()
{
	return __IsMovingSkill(m_kCurMotNode.uSkill);
}

DWORD CActorInstance::GetComboIndex()
{
	return m_dwcurComboIndex;
}

float CActorInstance::GetAttackingElapsedTime()
{
	return (GetLocalTime() - m_kCurMotNode.fStartTime) * m_kCurMotNode.fSpeedRatio;
//	return (GetLocalTime() - m_kCurMotNode.fStartTime) * __GetAttackSpeed();
}

bool CActorInstance::__CanInputNormalAttackCommand()
{
	if (IsWaiting())
		return true;

	if (isNormalAttacking())
	{
		float fElapsedTime = GetAttackingElapsedTime();	

		if (fElapsedTime > m_pkCurRaceMotionData->GetMotionDuration()*0.9f)
			return true;
	}

	return false;
}

BOOL CActorInstance::NormalAttack(float fDirRot, float fBlendTime)
{
	WORD wMotionIndex;
	if (!m_pkCurRaceData->GetNormalAttackIndex(m_wcurMotionMode, &wMotionIndex))
		return FALSE;

	BlendRotation(fDirRot, fBlendTime);
	SetAdvancingRotation(fDirRot);
	InterceptOnceMotion(wMotionIndex, 0.1f, 0, __GetAttackSpeed());

	__OnAttack(wMotionIndex);

	NEW_SetAtkPixelPosition(NEW_GetCurPixelPositionRef());

	return TRUE;
}

BOOL CActorInstance::ComboAttack(DWORD dwMotionIndex, float fDirRot, float fBlendTime)
{
	BlendRotation(fDirRot, fBlendTime);
	SetAdvancingRotation(fDirRot);

	InterceptOnceMotion(dwMotionIndex, fBlendTime, 0, __GetAttackSpeed());

	__OnAttack(dwMotionIndex);

	NEW_SetAtkPixelPosition(NEW_GetCurPixelPositionRef());

	return TRUE;
}

void CActorInstance::__ProcessMotionEventAttackSuccess(DWORD dwMotionKey, BYTE byEventIndex, CActorInstance & rVictim)
{
	CRaceMotionData * pMotionData;

	if (!m_pkCurRaceData->GetMotionDataPointer(dwMotionKey, &pMotionData))
		return;

	if (byEventIndex >= pMotionData->GetMotionEventDataCount())
		return;

	const CRaceMotionData::TMotionAttackingEventData * pMotionEventData;
	if (!pMotionData->GetMotionAttackingEventDataPointer(byEventIndex, &pMotionEventData))
		return;

	const D3DXVECTOR3& c_rv3VictimPos=rVictim.GetPositionVectorRef();
	__ProcessDataAttackSuccess(pMotionEventData->AttackData, rVictim, c_rv3VictimPos);
}


void CActorInstance::__ProcessMotionAttackSuccess(DWORD dwMotionKey, CActorInstance & rVictim)
{
	CRaceMotionData * c_pMotionData;

	if (!m_pkCurRaceData->GetMotionDataPointer(dwMotionKey, &c_pMotionData))
		return;

	const D3DXVECTOR3& c_rv3VictimPos=rVictim.GetPositionVectorRef();
	__ProcessDataAttackSuccess(c_pMotionData->GetMotionAttackDataReference(), rVictim, c_rv3VictimPos);
}


DWORD CActorInstance::__GetOwnerVID()
{
	return m_dwOwnerVID;
}

float CActorInstance::__GetOwnerTime()
{
	return GetLocalTime()-m_fOwnerBaseTime;
}

bool IS_HUGE_RACE(unsigned int vnum)
{
	switch (vnum)
	{
		case 191:
		case 192:
		case 193:
		case 194:
		case 491:
		case 492:
		case 493:
		case 494:
		case 531:
		case 532:
		case 533:
		case 534:
		case 591:
		case 691:
		case 692:
		case 791:
		case 792:
		case 793:
		case 794:
		case 993:
		case 1091:
		case 1092:
		case 1093:
		case 1094:
		case 1095:
		case 1191:
		case 1192:
		case 1304:
		case 1306:
		case 1307:
		case 1308:
		case 1309:
		case 1310:
		case 1334:
		case 1401:
		case 1402:
		case 1403:
		case 1501:
		case 1502:
		case 1503:
		case 1601:
		case 1602:
		case 1603:
		case 1901:
		case 1902:
		case 1903:
		case 1904:
		case 1905:
		case 1906:
		case 2091:
		case 2092:
		case 2093:
		case 2094:
		case 2191:
		case 2192:
		case 2206:
		case 2207:
		case 2291:
		case 2306:
		case 2307:
		case 2491:
		case 2492:
		case 2493:
		case 2494:
		case 2495:
		case 2591:
		case 2595:
		case 2597:
		case 2598:
		case 3090:
		case 3091:
		case 3190:
		case 3191:
		case 3290:
		case 3291:
		case 3390:
		case 3391:
		case 3490:
		case 3491:
		case 3590:
		case 3591:
		case 3595:
		case 3596:
		case 3690:
		case 3691:
		case 3790:
		case 3789:
		case 3890:
		case 3891:
		case 3901:
		case 3902:
		case 3903:
		case 5002:
		case 5161:
		case 5162:
		case 5163:
		case 6051:
		case 6091:
		case 6151:
		case 6191:
		case 6192:
		case 6390:
		case 6391:
		case 6407:
		case 6406:
		case 6408:
		case 6414:
		case 6418:
		case 6400:
		case 6116:
		case 6117:
		case 6421:
		case 4204:
		case 4209:
		case 4210:
		case 6415:
		case 6416:
		case 6417:
		case 6419:
		case 6420:
		case 2751:
		case 2752:
		case 2761:
		case 2762:
		case 2771:
		case 2772:
		case 2781:
		case 2782:
		case 2791:
		case 2792:
		case 2801:
		case 2802:
		case 2811:
		case 2812:
		case 2821:
		case 2822:
		case 2831:
		case 2832:
		case 2841:
		case 2842:
		case 2851:
		case 2852:
		case 2861:
		case 2862:
		case 694:
		case 1997:
		case 852:
		case 856:
		case 853:
		case 1998:
		case 854:
		case 4024:
		case 988:
		case 719:
		case 851:
		case 20420:
		case 858:
		case 855:
		case 6193:
		case 850:
		case 1999:
		case 857:
		case 859:
		case 1371:
		case 2022:
		case 2023:
		case 2024:
		case 2025:
		case 2026:
		case 2027:
		case 1996:
		case 2000:
		case 3960:
		case 1372:
			return true;
	}

	return false;
}

bool CActorInstance::__CanPushDestActor(CActorInstance& rkActorDst)
{
	if (rkActorDst.IsBuilding())
		return false;

	if (rkActorDst.IsDoor())
		return false;

	if (rkActorDst.IsStone())
		return false;

	if (rkActorDst.IsNPC())
		return false;

	// �Ŵ� ���� �и� ����
	extern bool IS_HUGE_RACE(unsigned int vnum);
	if (IS_HUGE_RACE(rkActorDst.GetRace()))
		return false;

	if (rkActorDst.IsStun())
		return true;
	
	if (rkActorDst.__GetOwnerVID()!=GetVirtualID())
		return false;

	if (rkActorDst.__GetOwnerTime()>3.0f)
		return false;

	return true;
}

bool IS_PARTY_HUNTING_RACE(unsigned int vnum)
{
	return true;

	// ��� ���� ��Ƽ ��� ����
	/*
	if (vnum < 8) // �÷��̾�
		return true;

	if (vnum >= 8000 && vnum <= 8112) // ��ƾ��
		return true;

	if (vnum >= 2400 && vnum <  5000) // õ�� ���� ���� ����
		return true;

	return false;
	*/
}

void CActorInstance::__ProcessDataAttackSuccess(const NRaceData::TAttackData & c_rAttackData, CActorInstance & rVictim, const D3DXVECTOR3 & c_rv3Position, UINT uiSkill, BOOL isSendPacket)
{
	if (NRaceData::HIT_TYPE_NONE == c_rAttackData.iHittingType)
		return;	

	InsertDelay(c_rAttackData.fStiffenTime);

	if (__CanPushDestActor(rVictim) && c_rAttackData.fExternalForce > 0.0f)
	{
		__PushCircle(rVictim);
		
		// VICTIM_COLLISION_TEST
		const D3DXVECTOR3& kVictimPos = rVictim.GetPosition();
		rVictim.m_PhysicsObject.IncreaseExternalForce(kVictimPos, c_rAttackData.fExternalForce); //*nForceRatio/100.0f);

		// VICTIM_COLLISION_TEST_END
	}

	// Invisible Time
	if (IS_PARTY_HUNTING_RACE(rVictim.GetRace())){
		if (uiSkill){ rVictim.m_fInvisibleTime = CTimer::Instance().GetCurrentSecond() + (c_rAttackData.fInvisibleTime - CalculateInvisibleTime(uiSkill, c_rAttackData)); }
		if (m_isMain){ rVictim.m_fInvisibleTime = CTimer::Instance().GetCurrentSecond() + (c_rAttackData.fInvisibleTime - CalculateInvisibleTime(uiSkill, c_rAttackData)); }
	}
	else {
		rVictim.m_fInvisibleTime = CTimer::Instance().GetCurrentSecond() + (c_rAttackData.fInvisibleTime - CalculateInvisibleTime(uiSkill, c_rAttackData));
	}
		
	// Stiffen Time
	rVictim.InsertDelay(c_rAttackData.fStiffenTime);

	// Hit Effect
	D3DXVECTOR3 vec3Effect(rVictim.m_x, rVictim.m_y, rVictim.m_z);
	
	// #0000780: [M2KR] ���� Ÿ�ݱ� ����
	extern bool IS_HUGE_RACE(unsigned int vnum);
	if (IS_HUGE_RACE(rVictim.GetRace()))
	{
		vec3Effect = c_rv3Position;
	}
	
	const D3DXVECTOR3 & v3Pos = GetPosition();

	float fHeight = D3DXToDegree(atan2(-vec3Effect.x + v3Pos.x,+vec3Effect.y - v3Pos.y));

	// 2004.08.03.myevan.�����̳� ���� ��� Ÿ�� ȿ���� ������ �ʴ´�
	if (rVictim.IsBuilding()||rVictim.IsDoor())
	{
		D3DXVECTOR3 vec3Delta=vec3Effect-v3Pos;
		D3DXVec3Normalize(&vec3Delta, &vec3Delta);
		vec3Delta*=30.0f;

		CEffectManager& rkEftMgr=CEffectManager::Instance();
		if (m_dwBattleHitEffectID)
			rkEftMgr.CreateEffect(m_dwBattleHitEffectID, v3Pos+vec3Delta, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else
	{
		CEffectManager& rkEftMgr=CEffectManager::Instance();
		if (m_dwBattleHitEffectID)
			rkEftMgr.CreateEffect(m_dwBattleHitEffectID, vec3Effect, D3DXVECTOR3(0.0f, 0.0f, fHeight));
		if (m_dwBattleAttachEffectID)
			rVictim.AttachEffectByID(0, NULL, m_dwBattleAttachEffectID);
	}

	if (rVictim.IsBuilding())
	{
		// 2004.08.03.������ ��� ��鸮�� �̻��ϴ�
	}
	else if (rVictim.IsStone() || rVictim.IsDoor())
	{
		__HitStone(rVictim);
	}
	else
	{
		///////////
		// Motion
		if (NRaceData::HIT_TYPE_GOOD == c_rAttackData.iHittingType || rVictim.IsResistFallen())
		{
			__HitGood(rVictim);
		}
		else if (NRaceData::HIT_TYPE_GREAT == c_rAttackData.iHittingType)
		{
			__HitGreate(rVictim);
		}
		else
		{
			TraceError("ProcessSucceedingAttacking: Unknown AttackingData.iHittingType %d", c_rAttackData.iHittingType);
		}
	}

#ifdef URIEL_ANTI_CHEAT
	if(urielac::SetAttackVictim(&rVictim))
		__OnHit(uiSkill, rVictim, isSendPacket);
#else
	__OnHit(uiSkill, rVictim, isSendPacket);
#endif
}

void CActorInstance::OnShootDamage()
{
	if (IsStun())
	{
		Die();
	}
	else
	{
		__Shake(100);

		if (!isLock() && !__IsKnockDownMotion() && !__IsStandUpMotion())
		{
			if (InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE))
				PushLoopMotion(CRaceMotionData::NAME_WAIT);
		}
	}
}

void CActorInstance::__Shake(DWORD dwDuration)
{
	DWORD dwCurTime=ELTimer_GetMSec();
	m_dwShakeTime=dwCurTime+dwDuration;
}

void CActorInstance::ShakeProcess()
{
	if (m_dwShakeTime)
	{
		D3DXVECTOR3 v3Pos(0.0f, 0.0f, 0.0f);

		DWORD dwCurTime=ELTimer_GetMSec();

		if (m_dwShakeTime<dwCurTime)
		{
			m_dwShakeTime=0;
		}
		else
		{
			int nShakeSize=10;

			switch (rand()%2)
			{
				case 0:v3Pos.x+=rand()%nShakeSize;break;
				case 1:v3Pos.x-=rand()%nShakeSize;break;
			}

			switch (rand()%2)
			{
				case 0:v3Pos.y+=rand()%nShakeSize;break;
				case 1:v3Pos.y-=rand()%nShakeSize;break;
			}

			switch (rand()%2)
			{
				case 0:v3Pos.z+=rand()%nShakeSize;break;
				case 1:v3Pos.z-=rand()%nShakeSize;break;
			}
		}

		m_worldMatrix._41	+= v3Pos.x;
		m_worldMatrix._42	+= v3Pos.y;
		m_worldMatrix._43	+= v3Pos.z;
	}
}

void CActorInstance::__HitStone(CActorInstance& rVictim)
{
	if (rVictim.IsStun())
	{
		rVictim.Die();
	}
	else
	{
		rVictim.__Shake(100);
	}
}

void CActorInstance::__HitGood(CActorInstance& rVictim)
{
	if (rVictim.IsKnockDown())
		return;

	if (rVictim.IsStun())
	{
		rVictim.Die();
	}
	else
	{
		rVictim.__Shake(100);

		if (!rVictim.isLock())
		{
			float fRotRad = D3DXToRadian(GetRotation());
			float fVictimRotRad = D3DXToRadian(rVictim.GetRotation());

			D3DXVECTOR2 v2Normal(sin(fRotRad), cos(fRotRad));
			D3DXVECTOR2 v2VictimNormal(sin(fVictimRotRad), cos(fVictimRotRad));

			D3DXVec2Normalize(&v2Normal, &v2Normal);
			D3DXVec2Normalize(&v2VictimNormal, &v2VictimNormal);

			float fScalar = D3DXVec2Dot(&v2Normal, &v2VictimNormal);

			if (fScalar < 0.0f)
			{
				if (rVictim.InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE))
					rVictim.PushLoopMotion(CRaceMotionData::NAME_WAIT);
			}
			else
			{
				if (rVictim.InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE_BACK))
					rVictim.PushLoopMotion(CRaceMotionData::NAME_WAIT);
				else if (rVictim.InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE))
					rVictim.PushLoopMotion(CRaceMotionData::NAME_WAIT);
			}
		}
	}
}

void CActorInstance::__HitGreate(CActorInstance& rVictim)
{
	// DISABLE_KNOCKDOWN_ATTACK
	if (rVictim.IsKnockDown())
		return;
	if (rVictim.__IsStandUpMotion())
		return;
	// END_OF_DISABLE_KNOCKDOWN_ATTACK

	extern bool IS_HUGE_RACE(unsigned int vnum);
	if (IS_HUGE_RACE(rVictim.GetRace()))
		return;

	float fRotRad = D3DXToRadian(GetRotation());
	float fVictimRotRad = D3DXToRadian(rVictim.GetRotation());

	D3DXVECTOR2 v2Normal(sin(fRotRad), cos(fRotRad));
	D3DXVECTOR2 v2VictimNormal(sin(fVictimRotRad), cos(fVictimRotRad));

	D3DXVec2Normalize(&v2Normal, &v2Normal);
	D3DXVec2Normalize(&v2VictimNormal, &v2VictimNormal);

	float fScalar = D3DXVec2Dot(&v2Normal, &v2VictimNormal);

	rVictim.__Shake(100);

	if (rVictim.IsUsingSkill())
		return;

	if (rVictim.IsStun())
	{
		if (fScalar < 0.0f)
			rVictim.InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE_FLYING);
		else
		{
			if (!rVictim.InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE_FLYING_BACK))
				rVictim.InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE_FLYING);
		}

		rVictim.m_isRealDead=true;
	}
	else
	{
		if (fScalar < 0.0f)
		{
			if (rVictim.InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE_FLYING))
			{
				rVictim.PushOnceMotion(CRaceMotionData::NAME_STAND_UP);
				rVictim.PushLoopMotion(CRaceMotionData::NAME_WAIT);
			}
		}
		else
		{
			if (!rVictim.InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE_FLYING_BACK))
			{
				if (rVictim.InterceptOnceMotion(CRaceMotionData::NAME_DAMAGE_FLYING))
				{
					rVictim.PushOnceMotion(CRaceMotionData::NAME_STAND_UP);
					rVictim.PushLoopMotion(CRaceMotionData::NAME_WAIT);
				}
			}
			else
			{
				rVictim.PushOnceMotion(CRaceMotionData::NAME_STAND_UP_BACK);
				rVictim.PushLoopMotion(CRaceMotionData::NAME_WAIT);
			}
		}
	}
}

void CActorInstance::SetBlendingPosition(const TPixelPosition & c_rPosition, float fBlendingTime)
{
	//return;
	TPixelPosition Position;

	Position.x = c_rPosition.x - m_x;
	Position.y = c_rPosition.y - m_y;
	Position.z = 0;

	m_PhysicsObject.SetLastPosition(Position, fBlendingTime);
}

void CActorInstance::ResetBlendingPosition()
{
	m_PhysicsObject.Initialize();
}

void CActorInstance::GetBlendingPosition(TPixelPosition * pPosition)
{
	if (m_PhysicsObject.isBlending())
	{
		m_PhysicsObject.GetLastPosition(pPosition);
		pPosition->x += m_x;
		pPosition->y += m_y;
		pPosition->z += m_z;
	}
	else
	{
		pPosition->x = m_x;
		pPosition->y = m_y;
		pPosition->z = m_z;
	}
}

void CActorInstance::__PushCircle(CActorInstance & rVictim)
{
	const TPixelPosition& c_rkPPosAtk=NEW_GetAtkPixelPositionRef();

	D3DXVECTOR3 v3SrcPos(c_rkPPosAtk.x, -c_rkPPosAtk.y, c_rkPPosAtk.z);

	const D3DXVECTOR3& c_rv3SrcPos = v3SrcPos;
	const D3DXVECTOR3& c_rv3DstPos = rVictim.GetPosition();

	D3DXVECTOR3 v3Direction;
	v3Direction.x = c_rv3DstPos.x - c_rv3SrcPos.x;
	v3Direction.y = c_rv3DstPos.y - c_rv3SrcPos.y;
	v3Direction.z = 0.0f;
	D3DXVec3Normalize(&v3Direction, &v3Direction);

	rVictim.__SetFallingDirection(v3Direction.x, v3Direction.y);
}

void CActorInstance::__PushDirect(CActorInstance & rVictim)
{
	D3DXVECTOR3 v3Direction;
	v3Direction.x = cosf(D3DXToRadian(m_fcurRotation + 270.0f));
	v3Direction.y = sinf(D3DXToRadian(m_fcurRotation + 270.0f));
	v3Direction.z = 0.0f;

	rVictim.__SetFallingDirection(v3Direction.x, v3Direction.y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CActorInstance::__isInvisible()
{
	if (IsDead())
		return true;

	if (CTimer::Instance().GetCurrentSecond() >= m_fInvisibleTime)
		return false;

	return true;
}

void CActorInstance::__SetFallingDirection(float fx, float fy)
{
	m_PhysicsObject.SetDirection(D3DXVECTOR3(fx, fy, 0.0f));
}

enum{
	RACE_WARRIOR_M,
	RACE_ASSASSIN_W,
	RACE_SURA_M,
	RACE_SHAMAN_W,
	RACE_WARRIOR_W,
	RACE_ASSASSIN_M,
	RACE_SURA_W,
	RACE_SHAMAN_M,
	RACE_WOLFMAN_M,
	RACE_MAX_NUM,
};
float CActorInstance::CalculateInvisibleTime(const UINT uiSkill, const NRaceData::TAttackData& c_rAttackData) {

	const float currentTime = CTimer::Instance().GetCurrentSecond();
	const float invisibleTime = c_rAttackData.fInvisibleTime;
	if ((GetRace() != RACE_SURA_M  && GetRace()!= RACE_SURA_W  && GetRace()!= RACE_WARRIOR_M  && GetRace()!= RACE_WARRIOR_W  && GetRace()!= RACE_WOLFMAN_M  && GetRace() != RACE_SHAMAN_M && GetRace() != RACE_SHAMAN_W) || m_fAtkSpd < 1.3 || uiSkill != 0) { return 0.0f; }

	const double timeScale = (m_fAtkSpd - 1.3) / 1.3;
	const auto invTime = c_rAttackData.fInvisibleTime * 0.5;
	return invTime * timeScale;
}

