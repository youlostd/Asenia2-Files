#include "stdafx.h"
#include "utils.h"
#include "vector.h"
#include "char.h"
#include "sectree_manager.h"
#include "char_manager.h"
#include "mob_manager.h"
#include "PetSystem.h"
#include "../../common/VnumHelper.h"
#include "packet.h"
#include "item_manager.h"
#include "item.h"


extern int passes_per_sec;
EVENTINFO(petsystem_event_info)
{
	CPetSystem* pPetSystem;
};

// PetSystem聖 update 背爽澗 event.
// PetSystem精 CHRACTER_MANAGER拭辞 奄糎 FSM生稽 update 背爽澗 奄糎 chracters人 含軒,
// Owner税 STATE研 update 拝 凶 _UpdateFollowAI 敗呪稽 update 背層陥.
// 益訓汽 owner税 state研 update研 CHRACTER_MANAGER拭辞 背爽奄 凶庚拭,
// petsystem聖 update馬陥亜 pet聖 unsummon馬澗 採歳拭辞 庚薦亜 持医陥.
// (CHRACTER_MANAGER拭辞 update 馬檎 chracter destroy亜 pending鞠嬢, CPetSystem拭辞澗 dangling 匂昔斗研 亜走壱 赤惟 吉陥.)
// 魚虞辞 PetSystem幻 穣汽戚闘 背爽澗 event研 降持獣鉄.
EVENTFUNC(petsystem_update_event)
{
	petsystem_event_info* info = dynamic_cast<petsystem_event_info*>( event->info );
	if ( info == NULL )
	{
		sys_err( "check_speedhack_event> <Factor> Null pointer" );
		return 0;
	}

	CPetSystem*	pPetSystem = info->pPetSystem;

	if (NULL == pPetSystem)
		return 0;

	
	pPetSystem->Update(0);
	// 0.25段原陥 飴重.
	return PASSES_PER_SEC(1) / 4;
}

/// NOTE: 1蝶遣斗亜 護鯵税 楢聖 亜霜 呪 赤澗走 薦廃... 蝶遣斗原陥 鯵呪研 陥牽惟 拝暗虞檎 痕呪稽 隔去亜... 製..
/// 亜霜 呪 赤澗 鯵呪人 疑獣拭 社発拝 呪 赤澗 鯵呪亜 堂険 呪 赤澗汽 戚訓闇 奄塙 蒸生艦 析舘 巷獣
const float PET_COUNT_LIMIT = 3;

///////////////////////////////////////////////////////////////////////////////////////
//  CPetActor
///////////////////////////////////////////////////////////////////////////////////////

CPetActor::CPetActor(LPCHARACTER owner, DWORD vnum, DWORD options)
{
	m_dwVnum = vnum;
	m_dwVID = 0;
	m_dwOptions = options;
	m_dwLastActionTime = 0;

	m_pkChar = 0;
	m_pkOwner = owner;

	m_originalMoveSpeed = 0;
	
	m_dwSummonItemVID = 0;
	m_dwSummonItemVnum = 0;
}

CPetActor::~CPetActor()
{
	this->Unsummon();

	m_pkOwner = 0;
}

void CPetActor::SetName(const char* name)
{
	std::string petName = m_pkOwner->GetName();

	if (0 != m_pkOwner && 
		0 == name && 
		0 != m_pkOwner->GetName())
	{
		petName += "'s Pet";
	}
	else
		petName += name;

	if (true == IsSummoned())
		m_pkChar->SetName(petName);

	m_name = petName;
}

bool CPetActor::Mount()
{
	if (0 == m_pkOwner)
		return false;

	if (true == HasOption(EPetOption_Mountable))
		m_pkOwner->MountVnum(m_dwVnum);

	return m_pkOwner->GetMountVnum() == m_dwVnum;
}

void CPetActor::Unmount()
{
	if (0 == m_pkOwner)
		return;

	if (m_pkOwner->IsHorseRiding())
		m_pkOwner->StopRiding();
}

void CPetActor::Unsummon()
{
	if (true == this->IsSummoned())
	{
		// 獄覗 肢薦
		this->ClearBuff();
		this->SetSummonItem(NULL);
		if (NULL != m_pkOwner)
			m_pkOwner->ComputePoints();

		if (NULL != m_pkChar && NULL != CHARACTER_MANAGER::instance().Find(this->GetVID()))
			M2_DESTROY_CHARACTER(m_pkChar);

		m_pkChar = 0;
		m_dwVID = 0;
	}
}

DWORD CPetActor::Summon(const char* petName, LPITEM pSummonItem, bool bSpawnFar)
{
	long x = m_pkOwner->GetX();
	long y = m_pkOwner->GetY();
	long z = m_pkOwner->GetZ();

	if (true == bSpawnFar)
	{
		x += (number(0, 1) * 2 - 1) * number(2000, 2500);
		y += (number(0, 1) * 2 - 1) * number(2000, 2500);
	}
	else
	{
		x += number(-100, 100);
		y += number(-100, 100);
	}

	if (0 != m_pkChar)
	{
		m_pkChar->Show (m_pkOwner->GetMapIndex(), x, y);
		m_dwVID = m_pkChar->GetVID();

		return m_dwVID;
	}
	
	m_pkChar = CHARACTER_MANAGER::instance().SpawnMob(
				m_dwVnum, 
				m_pkOwner->GetMapIndex(), 
				x, y, z,
				false, (int)(m_pkOwner->GetRotation()+180), false);

	if (0 == m_pkChar)
	{
		sys_err("[CPetSystem::Summon] Failed to summon the pet. (vnum: %d)", m_dwVnum);
		return 0;
	}

	m_pkChar->SetPet();

//	m_pkOwner->DetailLog();
//	m_pkChar->DetailLog();

	//楢税 厩亜研 爽昔税 厩亜稽 竺舛敗.
	m_pkChar->SetEmpire(m_pkOwner->GetEmpire());

	m_dwVID = m_pkChar->GetVID();

	this->SetName(petName);

	// SetSummonItem(pSummonItem)研 採献 板拭 ComputePoints研 採牽檎 獄覗 旋遂喫.
	this->SetSummonItem(pSummonItem);
	m_pkOwner->ComputePoints();
	m_pkChar->Show(m_pkOwner->GetMapIndex(), x, y, z);

	return m_dwVID;
}

bool CPetActor::_UpdatAloneActionAI(float fMinDist, float fMaxDist)
{
	float fDist = number(fMinDist, fMaxDist);
	float r = (float)number (0, 359);
	float dest_x = GetOwner()->GetX() + fDist * cos(r);
	float dest_y = GetOwner()->GetY() + fDist * sin(r);

	//m_pkChar->SetRotation(number(0, 359));        // 号狽精 沓棋生稽 竺舛

	//GetDeltaByDegree(m_pkChar->GetRotation(), fDist, &fx, &fy);

	// 汗充廃 公姶 紗失 端滴; 置曽 是帖人 掻娃 是帖亜 哀呪蒸陥檎 亜走 省澗陥.
	//if (!(SECTREE_MANAGER::instance().IsMovablePosition(m_pkChar->GetMapIndex(), m_pkChar->GetX() + (int) fx, m_pkChar->GetY() + (int) fy) 
	//			&& SECTREE_MANAGER::instance().IsMovablePosition(m_pkChar->GetMapIndex(), m_pkChar->GetX() + (int) fx/2, m_pkChar->GetY() + (int) fy/2)))
	//	return true;

	m_pkChar->SetNowWalking(true);

	//if (m_pkChar->Goto(m_pkChar->GetX() + (int) fx, m_pkChar->GetY() + (int) fy))
	//	m_pkChar->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);
	if (!m_pkChar->IsStateMove() && m_pkChar->Goto(dest_x, dest_y))
		m_pkChar->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);

	m_dwLastActionTime = get_dword_time();

	return true;
}

// char_state.cpp StateHorse敗呪 益撹 C&P -_-;
bool CPetActor::_UpdateFollowAI()
{
	if (0 == m_pkChar->m_pkMobData)
	{
		//sys_err("[CPetActor::_UpdateFollowAI] m_pkChar->m_pkMobData is NULL");
		return false;
	}
	
	// NOTE: 蝶遣斗(楢)税 据掘 戚疑 紗亀研 硝焼醤 馬澗汽, 背雁 葵(m_pkChar->m_pkMobData->m_table.sMovingSpeed)聖 送羨旋生稽 羨悦背辞 硝焼馨 呪亀 赤走幻
	// m_pkChar->m_pkMobData 葵戚 invalid廃 井酔亜 切爽 降持敗. 薄仙 獣娃淫域雌 据昔精 陥製拭 督焦馬壱 析舘精 m_pkChar->m_pkMobData 葵聖 焼森 紫遂馬走 省亀系 敗.
	// 食奄辞 古腰 伊紫馬澗 戚政澗 置段 段奄鉢 拝 凶 舛雌 葵聖 薦企稽 公条嬢神澗 井酔亀 赤製.. -_-;; ばばばばばばばばば
	if (0 == m_originalMoveSpeed)
	{
		const CMob* mobData = CMobManager::Instance().Get(m_dwVnum);

		if (0 != mobData)
			m_originalMoveSpeed = mobData->m_table.sMovingSpeed;
	}
	float	START_FOLLOW_DISTANCE = 100.0f;		// 戚 暗軒 戚雌 恭嬢走檎 耐焼亜奄 獣拙敗
	float	START_RUN_DISTANCE = 300.0f;		// 戚 暗軒 戚雌 恭嬢走檎 禽嬢辞 耐焼姶.

	float	RESPAWN_DISTANCE = 1500.f;			// 戚 暗軒 戚雌 菰嬢走檎 爽昔 新生稽 社発敗.
	int		APPROACH = 200;						// 羨悦 暗軒

	bool bDoMoveAlone = true;					// 蝶遣斗人 亜猿戚 赤聖 凶 肇切 食奄煽奄 崇送析闇走 食採 -_-;
	bool bRun = false;							// 禽嬢醤 馬蟹?

	DWORD currentTime = get_dword_time();

	long ownerX = m_pkOwner->GetX();		long ownerY = m_pkOwner->GetY();
	long charX = m_pkChar->GetX();			long charY = m_pkChar->GetY();

	float fDist = DISTANCE_APPROX(charX - ownerX, charY - ownerY);

	if (fDist >= RESPAWN_DISTANCE)
	{
		float fOwnerRot = m_pkOwner->GetRotation() * 3.141592f / 180.f;
		float fx = -APPROACH * cos(fOwnerRot);
		float fy = -APPROACH * sin(fOwnerRot);
		if (m_pkChar->Show(m_pkOwner->GetMapIndex(), ownerX + fx, ownerY + fy))
		{
			return true;
		}
	}
	
	
	if (fDist >= START_FOLLOW_DISTANCE)
	{
		if( fDist >= START_RUN_DISTANCE)
		{
			bRun = true;
		}

		m_pkChar->SetNowWalking(!bRun);		// NOTE: 敗呪 戚硯左壱 誇蓄澗闇匝 硝紹澗汽 SetNowWalking(false) 馬檎 禽澗暗績.. -_-;
		
		Follow(APPROACH);

		m_pkChar->SetLastAttacked(currentTime);
		m_dwLastActionTime = currentTime;
	}
	//else
	//{
	//	if (fabs(m_pkChar->GetRotation() - GetDegreeFromPositionXY(charX, charY, ownerX, ownerX)) > 10.f || fabs(m_pkChar->GetRotation() - GetDegreeFromPositionXY(charX, charY, ownerX, ownerX)) < 350.f)
	//	{
	//		m_pkChar->Follow(m_pkOwner, APPROACH);
	//		m_pkChar->SetLastAttacked(currentTime);
	//		m_dwLastActionTime = currentTime;
	//	}
	//}
	// Follow 掻戚走幻 爽昔引 析舛 暗軒 戚鎧稽 亜猿趨然陥檎 誇茶
	else 
		m_pkChar->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);
	//else if (currentTime - m_dwLastActionTime > number(5000, 12000))
	//{
	//	this->_UpdatAloneActionAI(START_FOLLOW_DISTANCE / 2, START_FOLLOW_DISTANCE);
	//}

	return true;
}

bool CPetActor::Update(DWORD deltaTime)
{
	bool bResult = true;

	// 楢 爽昔戚 宋醸暗蟹, 社発吉 楢税 雌殿亜 戚雌馬陥檎 楢聖 蒸証. (NOTE: 亜懐亜陥 戚訓 煽訓 戚政稽 社発吉 楢戚 DEAD 雌殿拭 匙走澗 井酔亜 赤製-_-;)
	// 楢聖 社発廃 焼戚奴戚 蒸暗蟹, 鎧亜 亜遭 雌殿亜 焼艦虞檎 楢聖 蒸証.
	// if (m_pkOwner->IsDead() || (IsSummoned() && m_pkChar->IsDead()) 
		// || NULL == ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID())
		// || ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID())->GetOwner() != this->GetOwner()
		// )
	// {
		// this->Unsummon();
		// return true;
	// }

	if (this->IsSummoned() && HasOption(EPetOption_Followable))
		bResult = bResult && this->_UpdateFollowAI();

	return bResult;
}

//NOTE : 爽税!!! MinDistance研 滴惟 説生檎 益 痕是幻鏑税 痕鉢疑照精 follow馬走 省澗陥,
bool CPetActor::Follow(float fMinDistance)
{
	// 亜形澗 是帖研 郊虞坐醤 廃陥.
	if( !m_pkOwner || !m_pkChar) 
		return false;

	float fOwnerX = m_pkOwner->GetX();
	float fOwnerY = m_pkOwner->GetY();

	float fPetX = m_pkChar->GetX();
	float fPetY = m_pkChar->GetY();

	float fDist = DISTANCE_SQRT(fOwnerX - fPetX, fOwnerY - fPetY);
	if (fDist <= fMinDistance)
		return false;

	m_pkChar->SetRotationToXY(fOwnerX, fOwnerY);

	float fx, fy;

	float fDistToGo = fDist - fMinDistance;
	GetDeltaByDegree(m_pkChar->GetRotation(), fDistToGo, &fx, &fy);
	
	if (!m_pkChar->Goto((int)(fPetX+fx+0.5f), (int)(fPetY+fy+0.5f)) )
		return false;

	m_pkChar->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0, 0);

	return true;
}

void CPetActor::SetSummonItem (LPITEM pItem)
{
	if (NULL == pItem)
	{
		m_dwSummonItemVID = 0;
		m_dwSummonItemVnum = 0;
		return;
	}

	m_dwSummonItemVID = pItem->GetVID();
	m_dwSummonItemVnum = pItem->GetVnum();
}

void CPetActor::GiveBuff()
{
	// 督伐 楢 獄覗澗 揮穿拭辞幻 降持敗.
	// if (34004 == m_dwVnum || 34009 == m_dwVnum)
	// {
		// if (NULL == m_pkOwner->GetDungeon())
		// {
			// return;
		// }
	// }
	// LPITEM item = ITEM_MANAGER::instance().FindByVID(m_dwSummonItemVID);
	// if (NULL != item)
		// item->ModifyPoints(true);
	return ;
}

void CPetActor::ClearBuff()
{
	// if (NULL == m_pkOwner)
		// return ;
	// TItemTable* item_proto = ITEM_MANAGER::instance().GetTable(m_dwSummonItemVnum);
	// if (NULL == item_proto)
		// return;
	// for (int i = 0; i < ITEM_APPLY_MAX_NUM; i++)
	// {
		// if (item_proto->aApplies[i].bType == APPLY_NONE)
			// continue;
		// m_pkOwner->ApplyPoint(item_proto->aApplies[i].bType, -item_proto->aApplies[i].lValue);
	// }

	return ;
}

///////////////////////////////////////////////////////////////////////////////////////
//  CPetSystem
///////////////////////////////////////////////////////////////////////////////////////

CPetSystem::CPetSystem(LPCHARACTER owner)
{
//	assert(0 != owner && "[CPetSystem::CPetSystem] Invalid owner");

	m_pkOwner = owner;
	m_dwUpdatePeriod = 400;

	m_dwLastUpdateTime = 0;
}

CPetSystem::~CPetSystem()
{
	Destroy();
}

void CPetSystem::Destroy()
{
	for (TPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CPetActor* petActor = iter->second;

		if (0 != petActor)
		{
			delete petActor;
		}
	}
	event_cancel(&m_pkPetSystemUpdateEvent);
	m_petActorMap.clear();
}

/// 楢 獣什奴 穣汽戚闘. 去系吉 楢級税 AI 坦軒 去聖 敗.
bool CPetSystem::Update(DWORD deltaTime)
{
	bool bResult = true;

	DWORD currentTime = get_dword_time();

	// CHARACTER_MANAGER拭辞 蝶遣斗嫌 Update拝 凶 古鯵痕呪稽 爽澗 (Pulse虞壱 鞠嬢赤澗)葵戚 戚穿 覗傾績引税 獣娃託戚昔匝 硝紹澗汽
	// 穿粕 陥献 葵戚虞辞-_-; 食奄拭 脊径生稽 級嬢神澗 deltaTime精 税耕亜 蒸製ばば	
	
	if (m_dwUpdatePeriod > currentTime - m_dwLastUpdateTime)
		return true;
	
	std::vector <CPetActor*> v_garbageActor;

	for (TPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CPetActor* petActor = iter->second;

		if (0 != petActor && petActor->IsSummoned())
		{
			LPCHARACTER pPet = petActor->GetCharacter();
			
			if (NULL == CHARACTER_MANAGER::instance().Find(pPet->GetVID()))
			{
				v_garbageActor.push_back(petActor);
			}
			else
			{
				bResult = bResult && petActor->Update(deltaTime);
			}
		}
	}
	for (std::vector<CPetActor*>::iterator it = v_garbageActor.begin(); it != v_garbageActor.end(); it++)
		DeletePet(*it);

	m_dwLastUpdateTime = currentTime;

	return bResult;
}

/// 淫軒 鯉系拭辞 楢聖 走崇
void CPetSystem::DeletePet(DWORD mobVnum)
{
	TPetActorMap::iterator iter = m_petActorMap.find(mobVnum);

	if (m_petActorMap.end() == iter)
	{
		sys_err("[CPetSystem::DeletePet] Can't find pet on my list (VNUM: %d)", mobVnum);
		return;
	}

	CPetActor* petActor = iter->second;

	if (0 == petActor)
		sys_err("[CPetSystem::DeletePet] Null Pointer (petActor)");
	else
		delete petActor;

	m_petActorMap.erase(iter);	
}

/// 淫軒 鯉系拭辞 楢聖 走崇
void CPetSystem::DeletePet(CPetActor* petActor)
{
	for (TPetActorMap::iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		if (iter->second == petActor)
		{
			if (0 != petActor)
				delete petActor;
			m_petActorMap.erase(iter);

			return;
		}
	}

	sys_err("[CPetSystem::DeletePet] Can't find petActor(0x%x) on my list(size: %d) ", petActor, m_petActorMap.size());
}

void CPetSystem::Unsummon(DWORD vnum, bool bDeleteFromList)
{
	CPetActor* actor = this->GetByVnum(vnum);

	if (0 == actor)
	{
		sys_err("[CPetSystem::GetByVnum(%d)] Null Pointer (petActor)", vnum);
		return;
	}
	actor->Unsummon();

	if (true == bDeleteFromList)
		this->DeletePet(actor);

	bool bActive = false;
	for (TPetActorMap::iterator it = m_petActorMap.begin(); it != m_petActorMap.end(); it++)
	{
		bActive |= it->second->IsSummoned();
	}
	if (false == bActive)
	{
		event_cancel(&m_pkPetSystemUpdateEvent);
		m_pkPetSystemUpdateEvent = NULL;
	}
}


CPetActor* CPetSystem::Summon(DWORD mobVnum, LPITEM pSummonItem, const char* petName, bool bSpawnFar, DWORD options)
{
	CPetActor* petActor = this->GetByVnum(mobVnum);

	// 去系吉 楢戚 焼艦虞檎 歯稽 持失 板 淫軒 鯉系拭 去系敗.
	if (0 == petActor)
	{
		petActor = M2_NEW CPetActor(m_pkOwner, mobVnum, options);
		m_petActorMap.insert(std::make_pair(mobVnum, petActor));
	}

	DWORD petVID = petActor->Summon(petName, pSummonItem, bSpawnFar);

	if (NULL == m_pkPetSystemUpdateEvent)
	{
		petsystem_event_info* info = AllocEventInfo<petsystem_event_info>();

		info->pPetSystem = this;

		m_pkPetSystemUpdateEvent = event_create(petsystem_update_event, info, PASSES_PER_SEC(1) / 4);	// 0.25段	
	}

	return petActor;
}


CPetActor* CPetSystem::GetByVID(DWORD vid) const
{
	CPetActor* petActor = 0;

	bool bFound = false;

	for (TPetActorMap::const_iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		petActor = iter->second;

		if (0 == petActor)
		{
			sys_err("[CPetSystem::GetByVID(%d)] Null Pointer (petActor)", vid);
			continue;
		}

		bFound = petActor->GetVID() == vid;

		if (true == bFound)
			break;
	}

	return bFound ? petActor : 0;
}

/// 去系 吉 楢 掻拭辞 爽嬢遭 光 VNUM聖 亜遭 衝斗研 鋼発馬澗 敗呪.
CPetActor* CPetSystem::GetByVnum(DWORD vnum) const
{
	CPetActor* petActor = 0;

	TPetActorMap::const_iterator iter = m_petActorMap.find(vnum);

	if (m_petActorMap.end() != iter)
		petActor = iter->second;

	return petActor;
}

size_t CPetSystem::CountSummoned() const
{
	size_t count = 0;

	for (TPetActorMap::const_iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CPetActor* petActor = iter->second;

		if (0 != petActor)
		{
			if (petActor->IsSummoned())
				++count;
		}
	}

	return count;
}

void CPetSystem::RefreshBuff()
{
	for (TPetActorMap::const_iterator iter = m_petActorMap.begin(); iter != m_petActorMap.end(); ++iter)
	{
		CPetActor* petActor = iter->second;

		if (0 != petActor)
		{
			if (petActor->IsSummoned())
			{
				petActor->GiveBuff();
			}
		}
	}
}