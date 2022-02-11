#include "stdafx.h"
#include "utils.h"
#include "vector.h"
#include "char.h"
#include "sectree_manager.h"
#include "char_manager.h"
#include "mob_manager.h"
#include "lwt_buffi.h"
#include "../../common/VnumHelper.h"
#include "packet.h"
#include "item_manager.h"
#include "item.h"
#include "affect.h"
#include "skill.h"
#include "questlua.h"
#include "questmanager.h"
#include "affect.h"
#include "utils.h"
#include "../../common/service.h"


extern int passes_per_sec;
EVENTINFO(supportSystem_event_info)
{
	CSupportSystem* psupportSystem;
};

EVENTFUNC(supportSystem_update_event)
{
	supportSystem_event_info* info = dynamic_cast<supportSystem_event_info*>( event->info );
	if ( info == NULL )
	{
		sys_err( "check_speedhack_event> <Factor> Null pointer" );
		return 0;
	}

	CSupportSystem*	psupportSystem = info->psupportSystem;

	if (NULL == psupportSystem)
		return 0;

	
	psupportSystem->Update(0);
	return PASSES_PER_SEC(1) / 4;
}

const float SUPPORT_COUNT_LIMIT = 3;

#define GIYILECEK_SAYI 2
#define GIYILECEK_ESYA_SAYI 13

const DWORD GiyilecekListe[GIYILECEK_SAYI][GIYILECEK_ESYA_SAYI] =
{
	{ 11809, 11819, 11829, 11839, 11849, 11859, 11869, 11879, 1188, 11899, 12049, 20769, 21279}, // zýrh veya kostum kendinize gore ayarlayýn
	{ 7009, 7019, 7029, 7039, 7049, 7059, 7129, 7159, 7179, 7189, 7199, 7379, 5179}, // silahlar
};

///////////////////////////////////////////////////////////////////////////////////////
//  CSupportActor
///////////////////////////////////////////////////////////////////////////////////////

CSupportActor::CSupportActor(LPCHARACTER owner, DWORD vnum, DWORD options)
{
	m_dwVnum = vnum;
	m_dwVID = 0;
	m_dwOptions = options;
	m_dwLastActionTime = 0;
	
	m_dwlevel = owner->GetLevel();
	m_pkChar = 0;
	m_pkOwner = owner;

	m_originalMoveSpeed = 0;
	
	m_dwSummonItemVID = 0;
	m_dwSummonItemVnum = 0;
}

CSupportActor::~CSupportActor()
{
	this->Unsummon();

	m_pkOwner = 0;
}

void CSupportActor::SetName()
{
	std::string supportName = m_pkChar->GetOwner()->GetName();
	supportName+="'s Buffi";
	std::string supportnames = "'s Buffi";

	if (true == IsSummoned())
		m_pkChar->SetName(supportName);
}


void CSupportActor::SetLevel(DWORD level)
{
	m_pkChar->SetLevel(static_cast<char>(level));
	m_dwlevel = level;
}

void CSupportActor::SetCostume()
{
	m_pkChar->SetSupportArmor(GiyilecekListe[0][m_dwlevel/10]);
}

void CSupportActor::SetWeapon()
{
	m_pkChar->SetSupportWeapon(GiyilecekListe[1][m_dwlevel/10]);
}


void CSupportActor::SetHair()
{
	LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
	if(pSummonItem != NULL)
	{
		if(pSummonItem->GetSocket(3) != 0)
		{
			m_pkChar->SetSupportHair(pSummonItem->GetSocket(3));
		}
	}
}

void CSupportActor::Unsummon()
{
	if (true == this->IsSummoned())
	{
		this->ClearBuff();

		LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());

		this->SetSummonItem(NULL);
		// if (NULL != m_pkOwner)
			// m_pkOwner->ComputePoints();

		if (NULL != m_pkChar)
			M2_DESTROY_CHARACTER(m_pkChar);

		m_pkChar = 0;
		m_dwVID = 0;
		//pSummonItem->SetSocket(2,0);
		// pSummonItem->SetSocket(2, 0);
		if (pSummonItem)
		{
			if (pSummonItem->GetVnum() == 91012 || pSummonItem->GetVnum() == 91013)
				m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "buffkapa 1");
			else
				m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "buffkapa 0");
		}
		//
	}
}


DWORD CSupportActor::Summon(const char* supportName, LPITEM pSummonItem, bool bSpawnFar)
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
		sys_err("[CSupportSystem::Summon] Failed to summon the shaman. (vnum: %d)", m_dwVnum);
		return 0;
	}

	m_pkChar->SetSupport();
	
	m_pkChar->SetOwner(m_pkOwner);
	m_pkChar->SetEmpire(m_pkOwner->GetEmpire());
	m_dwlevel = m_pkChar->GetOwner()->GetLevel();
	m_dwVID = m_pkChar->GetVID();
	this->SetSummonItem(pSummonItem);

	this->SetCostume();
	this->SetHair();
	this->SetWeapon();
	this->SetLevel(m_dwlevel);
	this->SetName();
	// m_pkOwner->ComputePoints();
	m_pkChar->Show(m_pkOwner->GetMapIndex(), x, y, z);
	m_pkChar->UpdatePacket();
	if (pSummonItem->GetVnum() == 91012 || pSummonItem->GetVnum() == 91013)
		m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "buffiac %d", (int)1);
	else
		m_pkOwner->ChatPacket(CHAT_TYPE_COMMAND, "buffiac 0");
	// pSummonItem->SetSocket(2,1);
	return m_dwVID;
}
void CSupportActor::RefreshCostume()
{
	SetCostume();
	SetHair();
	SetWeapon();
	m_pkChar->UpdatePacket();
}
void CSupportActor::UpdatePacketsupportActor()
{
	m_pkChar->UpdatePacket();
}
bool CSupportActor::_UpdatAloneActionAI(float fMinDist, float fMaxDist)
{
	float fDist = number(fMinDist, fMaxDist);
	float r = (float)number (0, 359);
	float dest_x = GetOwner()->GetX() + fDist * cos(r);
	float dest_y = GetOwner()->GetY() + fDist * sin(r);

	m_pkChar->SetNowWalking(true);

	if (!m_pkChar->IsStateMove() && m_pkChar->Goto(dest_x, dest_y))
		m_pkChar->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);

	m_dwLastActionTime = get_dword_time();

	return true;
}

bool CSupportActor::_UpdateFollowAI()
{
	if (0 == m_pkChar->m_pkMobData)
	{
		return false;
	}
	
	if (0 == m_originalMoveSpeed)
	{
		const CMob* mobData = CMobManager::Instance().Get(m_dwVnum);

		if (0 != mobData)
			m_originalMoveSpeed = mobData->m_table.sMovingSpeed;
	}
	float	START_FOLLOW_DISTANCE = 300.0f;		
	float	START_RUN_DISTANCE = 400.0f;		

	float	RESPAWN_DISTANCE = 4500.f;			
	int		APPROACH = 200;						

	//bool bDoMoveAlone = true;					
	bool bRun = false;							

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

		m_pkChar->SetNowWalking(!bRun);
		
		Follow(APPROACH);

		m_pkChar->SetLastAttacked(currentTime);
		m_dwLastActionTime = currentTime;
	}
	else 
		m_pkChar->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0);

	return true;
}

bool CSupportActor::Update(DWORD deltaTime)
{
	bool bResult = true;

	if (m_pkOwner->IsDead() || (IsSummoned() && m_pkChar->IsDead()) 
		|| NULL == ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID())
		|| ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID())->GetOwner() != this->GetOwner()
		)
	{
		this->Unsummon();
		return true;
	}
	
	this->UseSkill();
	
	if (this->IsSummoned() && HasOption(ESupportOption_Followable))
		bResult = bResult && this->_UpdateFollowAI();

	return bResult;
}
void CSupportActor::UseSkill()
{
	LPITEM pSummonItem = ITEM_MANAGER::instance().FindByVID(this->GetSummonItemVID());
	if (pSummonItem->GetVnum() == 91010 || pSummonItem->GetVnum() == 91011)
	{
		if(m_pkChar->GetOwner()->IsAffectFlag(AFF_HOSIN) == false && get_dword_time() - this->GetLastSkillTime() >= 3000)
		{
			m_pkChar->ComputeSkill(245,m_pkChar->GetOwner(),m_pkChar->GetOwner()->GetSkillLevel(245));
			this->SetLastSkillTime(get_dword_time());
			m_pkChar->SendSupportSkillPacket(SKILL_HOSIN, m_pkChar->GetOwner()->GetSkillLevel(245));
		}
		else if(m_pkChar->GetOwner()->IsAffectFlag(AFF_BOHO) == false && get_dword_time() - this->GetLastSkillTime() >= 3000)
		{
			m_pkChar->ComputeSkill(246,m_pkChar->GetOwner(),m_pkChar->GetOwner()->GetSkillLevel(246));		
			this->SetLastSkillTime(get_dword_time());
			m_pkChar->SendSupportSkillPacket(SKILL_REFLECT, m_pkChar->GetOwner()->GetSkillLevel(246));
		}
		else if(m_pkChar->GetOwner()->IsAffectFlag(AFF_GICHEON) == false && get_dword_time() - this->GetLastSkillTime() >= 3000)
		{
			this->SetLastSkillTime(get_dword_time());
			m_pkChar->ComputeSkill(247,m_pkChar->GetOwner(),m_pkChar->GetOwner()->GetSkillLevel(247));		
			m_pkChar->SendSupportSkillPacket(SKILL_GICHEON,m_pkChar->GetOwner()->GetSkillLevel(247));
		}
	}
	else if (pSummonItem->GetVnum() == 91012 || pSummonItem->GetVnum() == 91013)
	{
		if(m_pkChar->GetOwner()->IsAffectFlag(AFF_KWAESOK) == false && get_dword_time() - this->GetLastSkillTime() >= 3000)
		{
			m_pkChar->ComputeSkill(248,m_pkChar->GetOwner(),m_pkChar->GetOwner()->GetSkillLevel(248));
			this->SetLastSkillTime(get_dword_time());
			m_pkChar->SendSupportSkillPacket(SKILL_KWAESOK, m_pkChar->GetOwner()->GetSkillLevel(248));
		}
		else if(m_pkChar->GetOwner()->IsAffectFlag(AFF_JEUNGRYEOK) == false && get_dword_time() - this->GetLastSkillTime() >= 3000)
		{
			m_pkChar->ComputeSkill(249,m_pkChar->GetOwner(),m_pkChar->GetOwner()->GetSkillLevel(249));		
			this->SetLastSkillTime(get_dword_time());
			m_pkChar->SendSupportSkillPacket(SKILL_JEUNGRYEOK, m_pkChar->GetOwner()->GetSkillLevel(249));
		}
	}

}

bool CSupportActor::Follow(float fMinDistance)
{
	if( !m_pkOwner || !m_pkChar) 
		return false;

	float fOwnerX = m_pkOwner->GetX();
	float fOwnerY = m_pkOwner->GetY();

	float fSupportX = m_pkChar->GetX();
	float fSupportY = m_pkChar->GetY();

	float fDist = DISTANCE_SQRT(fOwnerX - fSupportX, fOwnerY - fSupportY);
	if (fDist <= fMinDistance)
		return false;

	m_pkChar->SetRotationToXY(fOwnerX, fOwnerY);

	float fx, fy;

	float fDistToGo = fDist - fMinDistance;
	GetDeltaByDegree(m_pkChar->GetRotation(), fDistToGo, &fx, &fy);
	
	if (!m_pkChar->Goto((int)(fSupportX+fx+0.5f), (int)(fSupportY+fy+0.5f)) )
		return false;

	m_pkChar->SendMovePacket(FUNC_WAIT, 0, 0, 0, 0, 0);

	return true;
}

void CSupportActor::SetSummonItem (LPITEM pItem)
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

void CSupportActor::GiveBuff()
{
	if (m_dwVnum == 34077)
	{
		if (NULL == m_pkOwner->GetDungeon())
		{
			return;
		}
	}
	LPITEM item = ITEM_MANAGER::instance().FindByVID(m_dwSummonItemVID);
	if (NULL != item)
		item->ModifyPoints(true);
	return ;
}

void CSupportActor::ClearBuff()
{
	if (NULL == m_pkOwner)
		return ;
	TItemTable* item_proto = ITEM_MANAGER::instance().GetTable(m_dwSummonItemVnum);
	if (NULL == item_proto)
		return;
	for (int i = 0; i < ITEM_APPLY_MAX_NUM; i++)
	{
		if (item_proto->aApplies[i].bType == APPLY_NONE)
			continue;
		m_pkOwner->ApplyPoint(item_proto->aApplies[i].bType, -item_proto->aApplies[i].lValue);
	}

	return ;
}

///////////////////////////////////////////////////////////////////////////////////////
//  CSupportSystem
///////////////////////////////////////////////////////////////////////////////////////

CSupportSystem::CSupportSystem(LPCHARACTER owner)
{

	m_pkOwner = owner;
	m_dwUpdatePeriod = 400;

	m_dwLastUpdateTime = 0;
}

CSupportSystem::~CSupportSystem()
{
	Destroy();
}

void CSupportSystem::Destroy()
{
	for (TsupportActorMap::iterator iter = m_supportActorMap.begin(); iter != m_supportActorMap.end(); ++iter)
	{
		CSupportActor* supportActor = iter->second;

		if (0 != supportActor)
		{
			delete supportActor;
		}
	}
	event_cancel(&m_pksupportSystemUpdateEvent);
	m_supportActorMap.clear();
}

/// Æê ½Ã½ºÅÛ ¾÷µ¥ÀÌÆ®. µî·ÏµÈ ÆêµéÀÇ AI Ã³¸® µîÀ» ÇÔ.
bool CSupportSystem::Update(DWORD deltaTime)
{
	bool bResult = true;

	DWORD currentTime = get_dword_time();	
	
	if (m_dwUpdatePeriod > currentTime - m_dwLastUpdateTime)
		return true;
	
	std::vector <CSupportActor*> v_garbageActor;

	for (TsupportActorMap::iterator iter = m_supportActorMap.begin(); iter != m_supportActorMap.end(); ++iter)
	{
		CSupportActor* supportActor = iter->second;

		if (0 != supportActor && supportActor->IsSummoned())
		{
			LPCHARACTER pSupport = supportActor->GetCharacter();
			
			if (NULL == CHARACTER_MANAGER::instance().Find(pSupport->GetVID()))
			{
				v_garbageActor.push_back(supportActor);
			}
			else
			{
				bResult = bResult && supportActor->Update(deltaTime);
			}
		}
	}
	for (std::vector<CSupportActor*>::iterator it = v_garbageActor.begin(); it != v_garbageActor.end(); it++)
		DeleteSupport(*it);

	m_dwLastUpdateTime = currentTime;

	return bResult;
}

void CSupportSystem::DeleteSupport(DWORD mobVnum)
{
	TsupportActorMap::iterator iter = m_supportActorMap.find(mobVnum);

	if (m_supportActorMap.end() == iter)
	{
		sys_err("[CSupportSystem::DeleteSupport] Can't find shaman on my list (VNUM: %d)", mobVnum);
		return;
	}

	CSupportActor* supportActor = iter->second;

	if (0 == supportActor)
		sys_err("[CSupportSystem::DeleteSupport] Null Pointer (supportActor)");
	else
		delete supportActor;

	m_supportActorMap.erase(iter);	
}

/// °ü¸® ¸ñ·Ï¿¡¼­ ÆêÀ» Áö¿ò
void CSupportSystem::DeleteSupport(CSupportActor* supportActor)
{
	for (TsupportActorMap::iterator iter = m_supportActorMap.begin(); iter != m_supportActorMap.end(); ++iter)
	{
		if (iter->second == supportActor)
		{
			delete supportActor;
			m_supportActorMap.erase(iter);

			return;
		}
	}

	sys_err("[CSupportSystem::DeleteSupport] Can't find supportActor(0x%x) on my list(size: %d) ", supportActor, m_supportActorMap.size());
}

void CSupportSystem::Unsummon(DWORD vnum, bool bDeleteFromList)
{
	CSupportActor* actor = this->GetByVnum(vnum);

	if (0 == actor)
	{
		sys_err("[CSupportSystem::GetByVnum(%d)] Null Pointer (supportActor)", vnum);
		return;
	}
	actor->Unsummon();

	if (true == bDeleteFromList)
		this->DeleteSupport(actor);

	bool bActive = false;
	for (TsupportActorMap::iterator it = m_supportActorMap.begin(); it != m_supportActorMap.end(); it++)
	{
		bActive |= it->second->IsSummoned();
	}
	if (false == bActive)
	{
		event_cancel(&m_pksupportSystemUpdateEvent);
		m_pksupportSystemUpdateEvent = NULL;
	}
}


bool CSupportSystem::IsActiveSupport()
{
	bool state = false;
	for (TsupportActorMap::iterator iter = m_supportActorMap.begin(); iter != m_supportActorMap.end(); ++iter)
	{
		CSupportActor* supportActor = iter->second;
		if (supportActor != 0)
		{
			if (supportActor->IsSummoned()) {
				state = true;
				break;
			}			
		}
	}
	return state;

}

CSupportActor* CSupportSystem::GetActiveSupport()
{
	for (TsupportActorMap::iterator iter = m_supportActorMap.begin(); iter != m_supportActorMap.end(); ++iter)
	{
		CSupportActor* supportActor = iter->second;
		if (supportActor != 0)
		{
			if (supportActor->IsSummoned()) {
				return supportActor;
			}			
		}
	}
	{
	return NULL;
	}
}

CSupportActor* CSupportSystem::Summon(DWORD mobVnum, LPITEM pSummonItem, const char* supportName, bool bSpawnFar, DWORD options)
{
	CSupportActor* supportActor = this->GetByVnum(mobVnum);

	if (0 == supportActor)
	{
		supportActor = M2_NEW CSupportActor(m_pkOwner, mobVnum, options);
		m_supportActorMap.insert(std::make_pair(mobVnum, supportActor));
	}

	supportActor->Summon(supportName, pSummonItem, bSpawnFar);

	if (NULL == m_pksupportSystemUpdateEvent)
	{
		supportSystem_event_info* info = AllocEventInfo<supportSystem_event_info>();

		info->psupportSystem = this;

		m_pksupportSystemUpdateEvent = event_create(supportSystem_update_event, info, PASSES_PER_SEC(1) / 4);	// 0.25ÃÊ	
	}

	return supportActor;
}


CSupportActor* CSupportSystem::GetByVID(DWORD vid) const
{
	CSupportActor* supportActor = 0;

	bool bFound = false;

	for (TsupportActorMap::const_iterator iter = m_supportActorMap.begin(); iter != m_supportActorMap.end(); ++iter)
	{
		supportActor = iter->second;

		if (0 == supportActor)
		{
			sys_err("[CSupportSystem::GetByVID(%d)] Null Pointer (supportActor)", vid);
			continue;
		}

		bFound = supportActor->GetVID() == vid;

		if (true == bFound)
			break;
	}

	return bFound ? supportActor : 0;
}

CSupportActor* CSupportSystem::GetByVnum(DWORD vnum) const
{
	CSupportActor* supportActor = 0;

	TsupportActorMap::const_iterator iter = m_supportActorMap.find(vnum);

	if (m_supportActorMap.end() != iter)
		supportActor = iter->second;

	return supportActor;
}

size_t CSupportSystem::CountSummoned() const
{
	size_t count = 0;

	for (TsupportActorMap::const_iterator iter = m_supportActorMap.begin(); iter != m_supportActorMap.end(); ++iter)
	{
		CSupportActor* supportActor = iter->second;

		if (0 != supportActor)
		{
			if (supportActor->IsSummoned())
				++count;
		}
	}

	return count;
}

void CSupportSystem::RefreshBuff()
{
	for (TsupportActorMap::const_iterator iter = m_supportActorMap.begin(); iter != m_supportActorMap.end(); ++iter)
	{
		CSupportActor* supportActor = iter->second;

		if (0 != supportActor)
		{
			if (supportActor->IsSummoned())
			{
				supportActor->GiveBuff();
			}
		}
	}
}

extern int (*check_name) (const char * str);

namespace quest
{

#ifdef LWT_BUFFI_SYSTEM
	// syntax in LUA: support.summon(mob_vnum, support's name, (bool)run to me from far away)
	int support_summon(lua_State * L)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		CSupportSystem* supportSystem = ch->GetSupportSystem();
		LPITEM pItem = CQuestManager::instance().GetCurrentItem();
		if (!ch || !supportSystem || !pItem)
		{
			lua_pushnumber (L, 0);
			return 1;
		}

		if (0 == supportSystem)
		{
			lua_pushnumber (L, 0);
			return 1;
		}

		// ¼ÒÈ¯¼öÀÇ vnum
		DWORD mobVnum= lua_isnumber(L, 1) ? lua_tonumber(L, 1) : 0;

		// ¼ÒÈ¯¼öÀÇ ÀÌ¸§
		const char* supportName = lua_isstring(L, 2) ? lua_tostring(L, 2) : 0;

		// ¼ÒÈ¯ÇÏ¸é ¸Ö¸®¼­ºÎÅÍ ´Þ·Á¿À´ÂÁö ¿©ºÎ
		bool bFromFar = lua_isboolean(L, 3) ? lua_toboolean(L, 3) : false;

		CSupportActor* support = supportSystem->Summon(mobVnum, pItem, supportName, bFromFar);

		if (support != NULL)
			lua_pushnumber (L, support->GetVID());
		else
			lua_pushnumber (L, 0);

		return 1;
	}

	// syntax: support.unsummon(mob_vnum)
	int support_unsummon(lua_State * L)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		CSupportSystem* supportSystem = ch->GetSupportSystem();

		if (0 == supportSystem)
			return 0;

		// ¼ÒÈ¯¼öÀÇ vnum
		DWORD mobVnum= lua_isnumber(L, 1) ? lua_tonumber(L, 1) : 0;

		supportSystem->Unsummon(mobVnum);
		return 1;
	}

	// syntax: support.unsummon(mob_vnum)
	int support_count_summoned(lua_State * L)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		CSupportSystem* supportSystem = ch->GetSupportSystem();

		lua_Number count = 0;

		if (0 != supportSystem)
			count = (lua_Number)supportSystem->CountSummoned();

		lua_pushnumber(L, count);

		return 1;
	}

	// syntax: support.is_summon(mob_vnum)
	int support_is_summon(lua_State * L)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		CSupportSystem* supportSystem = ch->GetSupportSystem();

		if (0 == supportSystem)
			return 0;

		// ¼ÒÈ¯¼öÀÇ vnum
		DWORD mobVnum= lua_isnumber(L, 1) ? lua_tonumber(L, 1) : 0;

		CSupportActor* supportActor = supportSystem->GetByVnum(mobVnum);

		if (NULL == supportActor)
			lua_pushboolean(L, false);
		else
			lua_pushboolean(L, supportActor->IsSummoned());

		return 1;
	}

	int support_spawn_effect(lua_State * L)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		CSupportSystem* supportSystem = ch->GetSupportSystem();

		if (0 == supportSystem)
			return 0;

		DWORD mobVnum = lua_isnumber(L, 1) ? lua_tonumber(L, 1) : 0;

		CSupportActor* supportActor = supportSystem->GetByVnum(mobVnum);
		if (NULL == supportActor)
			return 0;
		LPCHARACTER support_ch = supportActor->GetCharacter();
		if (NULL == support_ch)
			return 0;

		if (lua_isstring(L, 2))
		{
			support_ch->SpecificEffectPacket (lua_tostring(L, 2));
		}
		return 0;
	}

	void RegisterSupportFunctionTable()
	{
		luaL_reg support_functions[] =
		{
			{ "summon",			support_summon			},
			{ "unsummon",		support_unsummon		},
			{ "is_summon",		support_is_summon		},
			{ "count_summoned",	support_count_summoned	},
			{ "spawn_effect",	support_spawn_effect	},
			{ NULL,				NULL				}
		};

		CQuestManager::instance().AddLuaFunctionTable("supports", support_functions);
	}
#endif

}
