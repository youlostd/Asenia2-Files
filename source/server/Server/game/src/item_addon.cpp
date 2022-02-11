#include "stdafx.h"
#include "constants.h"
#include "utils.h"
#include "item.h"
#include "item_addon.h"

CItemAddonManager::CItemAddonManager()
{
}

CItemAddonManager::~CItemAddonManager()
{
}

void CItemAddonManager::ApplyAddonTo(int iAddonType, LPITEM pItem)
{
	if (!pItem)
	{
		sys_err("ITEM pointer null");
		return;
	}

	// TODO 일단 하드코딩으로 평타 스킬 수치 변경만 경우만 적용받게한다.

	int iSkillBonus = MINMAX(-30, (int) (gauss_random(0, 10) + 0.5f), 30);
	int iNormalHitBonus = 15;
	if (abs(iSkillBonus) <= 15)
		iNormalHitBonus = -2 * iSkillBonus + abs(number(-20, 20) + number(-20, 20)) + number(1, 4);
	else
		iNormalHitBonus = -2 * iSkillBonus + number(1, 10);

	pItem->RemoveAttributeType(APPLY_SKILL_DAMAGE_BONUS);
	pItem->RemoveAttributeType(APPLY_NORMAL_HIT_DAMAGE_BONUS);
	pItem->AddAttribute(APPLY_NORMAL_HIT_DAMAGE_BONUS, iNormalHitBonus);
	pItem->AddAttribute(APPLY_SKILL_DAMAGE_BONUS, iSkillBonus);
}

void CItemAddonManager::ApplyAddonTo2(int iAddonType, LPITEM pItem)
{
	if (!pItem)
	{
		sys_err("ITEM pointer null");
		return;
	}

	// TODO 일단 하드코딩으로 평타 스킬 수치 변경만 경우만 적용받게한다.

	int iSkillBonus = 0;
	int iNormalHitBonus = 0;
	int percent = number(1,16);
	
		if (percent == 1 )
		{
			iNormalHitBonus = 80;
			iSkillBonus = -18;
		}
		else if (percent == 4)
		{
			iNormalHitBonus = 82;
			iSkillBonus = -18;
		}
		else if (percent == 5)
		{
			iNormalHitBonus = 85;
			iSkillBonus = -18;
		}		
		else if (percent == 6 )
		{
			iNormalHitBonus = 87;
			iSkillBonus = -18;
		}
		else if (percent == 7 )
		{
			iNormalHitBonus = 89;
			iSkillBonus = -15;
		}
		else if (percent == 8)
		{
			iNormalHitBonus = 91;
			iSkillBonus = -18;
		}
		else if (percent == 9)
		{
			iNormalHitBonus = 93;
			iSkillBonus = -13;
		}
		else if (percent == 10)
		{
			iNormalHitBonus = 94;
			iSkillBonus = -14;
		}
		else if (percent == 11)
		{
			iNormalHitBonus = 96;
			iSkillBonus = -15;
		}
		else if (percent == 12)
		{
			iNormalHitBonus = 96;
			iSkillBonus = -18;
		}
		else if (percent == 13)
		{
			iNormalHitBonus = 97;
			iSkillBonus = -20;
		}
		else if (percent == 14)
		{
			iNormalHitBonus = 97;
			iSkillBonus = -23;
		}
		else if (percent == 15)
		{
			iNormalHitBonus = 99;
			iSkillBonus = -25;
		}
		else if (percent == 16)
		{
			iNormalHitBonus = 100;
			iSkillBonus = -30;
		}
		
	pItem->RemoveAttributeType(APPLY_SKILL_DAMAGE_BONUS);
	pItem->RemoveAttributeType(APPLY_NORMAL_HIT_DAMAGE_BONUS);
	pItem->AddAttribute(APPLY_NORMAL_HIT_DAMAGE_BONUS, iNormalHitBonus);
	pItem->AddAttribute(APPLY_SKILL_DAMAGE_BONUS, iSkillBonus);
}