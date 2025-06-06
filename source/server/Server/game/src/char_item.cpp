#include "stdafx.h"
#include "lwt_buffi.h"
#include <stack>
#include "utils.h"
#include "config.h"
#include "char.h"
#include "char_manager.h"
#include "item_manager.h"
#include "desc.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "packet.h"
#include "affect.h"
#include "skill.h"
#include "start_position.h"
#include "mob_manager.h"
#include "db.h"
#include "log.h"
#include "vector.h"
#include "buffer_manager.h"
#include "questmanager.h"
#include "fishing.h"
#include "party.h"
#include "dungeon.h"
#include "refine.h"
#include "unique_item.h"
#include "war_map.h"
#include "xmas_event.h"
#include "marriage.h"
#include "monarch.h"
#include "polymorph.h"
#include "blend_item.h"
#include "castle.h"
#include "BattleArena.h"
#include "arena.h"
#include "dev_log.h"
#include "pcbang.h"
#include "threeway_war.h"
#include "PetSystem.h"
#include "safebox.h"
#include "shop.h"

#include "../../common/VnumHelper.h"
#include "DragonSoul.h"
#include "buff_on_attributes.h"
#include "belt_inventory_helper.h"
#include "offline_shop.h"
#include "offlineshop_manager.h"
#include "offlineshop_config.h"
#ifdef ENABLE_SWITCHBOT
#include "new_switchbot.h"
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
#include "New_PetSystem.h"
#endif
//auction_temp
#ifdef __AUCTION__
#include "auction_manager.h"
#endif
#ifdef ENABLE_TITLE_SYSTEM
#include "title.h"
#endif
const int ITEM_BROKEN_METIN_VNUM = 28960;

// CHANGE_ITEM_ATTRIBUTES
const DWORD CHARACTER::msc_dwDefaultChangeItemAttrCycle = 10;
const char CHARACTER::msc_szLastChangeItemAttrFlag[] = "Item.LastChangeItemAttr";
const char CHARACTER::msc_szChangeItemAttrCycleFlag[] = "change_itemattr_cycle";
// END_OF_CHANGE_ITEM_ATTRIBUTES
const BYTE g_aBuffOnAttrPoints[] = { POINT_ENERGY, POINT_COSTUME_ATTR_BONUS };

struct FFindStone
{
	std::map<DWORD, LPCHARACTER> m_mapStone;

	void operator()(LPENTITY pEnt)
	{
		if (pEnt->IsType(ENTITY_CHARACTER) == true)
		{
			LPCHARACTER pChar = (LPCHARACTER)pEnt;

			if (pChar->IsStone() == true)
			{
				m_mapStone[(DWORD)pChar->GetVID()] = pChar;
			}
		}
	}
};


//귀환부, 귀환기억부, 결혼반지
static bool IS_SUMMON_ITEM(int vnum)
{
	switch (vnum)
	{
		case 22000:
		case 22010:
		case 22011:
		case 22020:
		case ITEM_MARRIAGE_RING:
			return true;
	}

	return false;
}

static bool IS_MONKEY_DUNGEON(int map_index)
{
	switch (map_index)
	{
		case 5:
		case 25:
		case 45:
		case 108:
		case 109:
			return true;
	}

	return false;
}

bool IS_SUMMONABLE_ZONE(int map_index)
{
	// 몽키던전
	if (IS_MONKEY_DUNGEON(map_index))
		return false;
	// 성
	if (IS_CASTLE_MAP(map_index))
		return false;

	switch (map_index)
	{
		case 66 : // 사귀타워
		case 71 : // 거미 던전 2층
		case 72 : // 천의 동굴
		case 73 : // 천의 동굴 2층
		case 193 : // 거미 던전 2-1층
#if 0
		case 184 : // 천의 동굴(신수)
		case 185 : // 천의 동굴 2층(신수)
		case 186 : // 천의 동굴(천조)
		case 187 : // 천의 동굴 2층(천조)
		case 188 : // 천의 동굴(진노)
		case 189 : // 천의 동굴 2층(진노)
#endif
//		case 206 : // 아귀동굴
		case 216 : // 아귀동굴
		case 217 : // 거미 던전 3층
		case 208 : // 천의 동굴 (용방)
			return false;
	}

	if (CBattleArena::IsBattleArenaMap(map_index)) return false;

	// 모든 private 맵으론 워프 불가능
	if (map_index > 10000) return false;

	return true;
}

bool IS_BOTARYABLE_ZONE(int nMapIndex)
{
	if (LC_IsYMIR() == false && LC_IsKorea() == false) return true;

	switch (nMapIndex)
	{
		case 1 :
		case 3 :
		case 21 :
		case 23 :
		case 41 :
		case 43 :
			return true;
	}
	
	return false;
}

// item socket 이 프로토타입과 같은지 체크 -- by mhh
static bool FN_check_item_socket(LPITEM item)
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		if (item->GetSocket(i) != item->GetProto()->alSockets[i])
			return false;
	}

	return true;
}

// item socket 복사 -- by mhh
static void FN_copy_item_socket(LPITEM dest, LPITEM src)
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		dest->SetSocket(i, src->GetSocket(i));
	}
}
static bool FN_check_item_sex(LPCHARACTER ch, LPITEM item)
{
	// 남자 금지
	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_MALE))
	{
		if (SEX_MALE==GET_SEX(ch))
			return false;
	}
	// 여자금지
	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_FEMALE)) 
	{
		if (SEX_FEMALE==GET_SEX(ch))
			return false;
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////
// ITEM HANDLING
/////////////////////////////////////////////////////////////////////////////
bool CHARACTER::CanHandleItem(bool bSkipCheckRefine, bool bSkipObserver)
{
	if (!bSkipObserver)
		if (m_bIsObserver)
			return false;

	if (GetMyShop())
		return false;

	if (!bSkipCheckRefine)
		if (m_bUnderRefine)
			return false;

	if (IsCubeOpen() || NULL != DragonSoul_RefineWindow_GetOpener())
		return false;

	if (IsWarping())
		return false;

	#ifdef __SASH_SYSTEM__
	if ((m_bSashCombination) || (m_bSashAbsorption))
		return false;
	#endif	
#ifdef ENABLE_AURA_SYSTEM
	if ((m_bAuraRefine) || (m_bAuraAbsorption))
		return false;
#endif
	return true;
}

LPITEM CHARACTER::GetInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(INVENTORY, wCell));
}
#ifdef ENABLE_SPECIAL_STORAGE
LPITEM CHARACTER::GetUpgradeInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(UPGRADE_INVENTORY, wCell));
}
LPITEM CHARACTER::GetStoneInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(STONE_INVENTORY, wCell));
}
LPITEM CHARACTER::GetChestInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(CHEST_INVENTORY, wCell));
}
LPITEM CHARACTER::GetAttrInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(ATTR_INVENTORY, wCell));
}
#endif
LPITEM CHARACTER::GetItem(TItemPos Cell) const
{
	if (!IsValidItemPosition(Cell))
		return NULL;
	WORD wCell = Cell.cell;
	BYTE window_type = Cell.window_type;
	switch (window_type)
	{
	case INVENTORY:
	case EQUIPMENT:
		if (wCell >= INVENTORY_AND_EQUIP_SLOT_MAX)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid item cell %d", wCell);
			return NULL;
		}
		return m_pointsInstant.pItems[wCell];
	case DRAGON_SOUL_INVENTORY:
		if (wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid DS item cell %d", wCell);
			return NULL;
		}
		return m_pointsInstant.pDSItems[wCell];
#ifdef ENABLE_SPECIAL_STORAGE
	case UPGRADE_INVENTORY:
		if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid SSU item cell %d", wCell);
			return NULL;
		}
		return m_pointsInstant.pSSUItems[wCell];
	case STONE_INVENTORY:
		if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid SSS item cell %d", wCell);
			return NULL;
		}
		return m_pointsInstant.pSSSItems[wCell];
	case CHEST_INVENTORY:
		if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid SSS item cell %d", wCell);
			return NULL;
		}
		return m_pointsInstant.pSSCItems[wCell];
	case ATTR_INVENTORY:
		if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid SSA item cell %d", wCell);
			return NULL;
		}
		return m_pointsInstant.pSSAItems[wCell];
#endif
#ifdef ENABLE_SWITCHBOT
	case SWITCHBOT:
		if (wCell >= SWITCHBOT_SLOT_COUNT)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid switchbot item cell %d", wCell);
			return NULL;
		}
		return m_pointsInstant.pSwitchbotItems[wCell];
#endif
	default:
		return NULL;
	}
	return NULL;
}

void CHARACTER::SetItem(TItemPos Cell, LPITEM pItem)
{
	WORD wCell = Cell.cell;
	BYTE window_type = Cell.window_type;
	if ((unsigned long)((CItem*)pItem) == 0xff || (unsigned long)((CItem*)pItem) == 0xffffffff)
	{
		sys_err("!!! FATAL ERROR !!! item == 0xff (char: %s cell: %u)", GetName(), wCell);
		core_dump();
		return;
	}

	if (pItem && pItem->GetOwner())
	{
		assert(!"GetOwner exist");
		return;
	}
	// 기본 인벤토리
	switch(window_type)
	{
	case INVENTORY:
	case EQUIPMENT:
		{
			if (wCell >= INVENTORY_AND_EQUIP_SLOT_MAX)
			{
				sys_err("CHARACTER::SetItem: invalid item cell %d", wCell);
				return;
			}

			LPITEM pOld = m_pointsInstant.pItems[wCell];

			if (pOld)
			{
				if (wCell < INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pOld->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= INVENTORY_MAX_NUM)
							continue;

						if (m_pointsInstant.pItems[p] && m_pointsInstant.pItems[p] != pOld)
							continue;

						m_pointsInstant.bItemGrid[p] = 0;
					}
				}
				else
					m_pointsInstant.bItemGrid[wCell] = 0;
			}

			if (pItem)
			{
				if (wCell < INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pItem->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= INVENTORY_MAX_NUM)
							continue;

						// wCell + 1 로 하는 것은 빈곳을 체크할 때 같은
						// 아이템은 예외처리하기 위함
						m_pointsInstant.bItemGrid[p] = wCell + 1;
					}
				}
				else
					m_pointsInstant.bItemGrid[wCell] = wCell + 1;
			}

			m_pointsInstant.pItems[wCell] = pItem;
		}
		break;
	// 용혼석 인벤토리
	case DRAGON_SOUL_INVENTORY:
		{
			LPITEM pOld = m_pointsInstant.pDSItems[wCell];

			if (pOld)
			{
				if (wCell < DRAGON_SOUL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pOld->GetSize(); ++i)
					{
						int p = wCell + (i * DRAGON_SOUL_BOX_COLUMN_NUM);

						if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
							continue;

						if (m_pointsInstant.pDSItems[p] && m_pointsInstant.pDSItems[p] != pOld)
							continue;

						m_pointsInstant.wDSItemGrid[p] = 0;
					}
				}
				else
					m_pointsInstant.wDSItemGrid[wCell] = 0;
			}

			if (pItem)
			{
				if (wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
				{
					sys_err("CHARACTER::SetItem: invalid DS item cell %d", wCell);
					return;
				}

				if (wCell < DRAGON_SOUL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pItem->GetSize(); ++i)
					{
						int p = wCell + (i * DRAGON_SOUL_BOX_COLUMN_NUM);

						if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
							continue;

						// wCell + 1 로 하는 것은 빈곳을 체크할 때 같은
						// 아이템은 예외처리하기 위함
						m_pointsInstant.wDSItemGrid[p] = wCell + 1;
					}
				}
				else
					m_pointsInstant.wDSItemGrid[wCell] = wCell + 1;
			}

			m_pointsInstant.pDSItems[wCell] = pItem;
		}
		break;
#ifdef ENABLE_SPECIAL_STORAGE	
	case UPGRADE_INVENTORY:
		{
			LPITEM pOld = m_pointsInstant.pSSUItems[wCell];

			if (pOld)
			{
				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pOld->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
							continue;

						if (m_pointsInstant.pSSUItems[p] && m_pointsInstant.pSSUItems[p] != pOld)
							continue;

						m_pointsInstant.wSSUItemGrid[p] = 0;
					}
				}
				else
					m_pointsInstant.wSSUItemGrid[wCell] = 0;
			}

			if (pItem)
			{
				if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
				{
					sys_err("CHARACTER::SetItem: invalid SSU item cell %d", wCell);
					return;
				}

				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pItem->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
							continue;

						m_pointsInstant.wSSUItemGrid[p] = wCell + 1;
					}
				}
				else
					m_pointsInstant.wSSUItemGrid[wCell] = wCell + 1;
			}

			m_pointsInstant.pSSUItems[wCell] = pItem;
		}
		break;
	case STONE_INVENTORY:
		{
			LPITEM pOld = m_pointsInstant.pSSSItems[wCell];

			if (pOld)
			{
				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pOld->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
							continue;

						if (m_pointsInstant.pSSSItems[p] && m_pointsInstant.pSSSItems[p] != pOld)
							continue;

						m_pointsInstant.wSSSItemGrid[p] = 0;
					}
				}
				else
					m_pointsInstant.wSSSItemGrid[wCell] = 0;
			}

			if (pItem)
			{
				if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
				{
					sys_err("CHARACTER::SetItem: invalid SSB item cell %d", wCell);
					return;
				}

				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pItem->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
							continue;

						m_pointsInstant.wSSSItemGrid[p] = wCell + 1;
					}
				}
				else
					m_pointsInstant.wSSSItemGrid[wCell] = wCell + 1;
			}

			m_pointsInstant.pSSSItems[wCell] = pItem;
		}
		break;
	case CHEST_INVENTORY:
		{
			LPITEM pOld = m_pointsInstant.pSSCItems[wCell];
			if (pOld)
			{
				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pOld->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
							continue;

						if (m_pointsInstant.pSSCItems[p] && m_pointsInstant.pSSCItems[p] != pOld)
							continue;

						m_pointsInstant.wSSCItemGrid[p] = 0;
					}
				}
				else
					m_pointsInstant.wSSCItemGrid[wCell] = 0;
			}

			if (pItem)
			{
				if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
				{
					sys_err("CHARACTER::SetItem: invalid SSB item cell %d", wCell);
					return;
				}

				if (wCell < SPECIAL_INVENTORY_MAX_NUM)
				{
					for (int i = 0; i < pItem->GetSize(); ++i)
					{
						int p = wCell + (i * 5);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
							continue;

						m_pointsInstant.wSSCItemGrid[p] = wCell + 1;
					}
				}
				else
					m_pointsInstant.wSSCItemGrid[wCell] = wCell + 1;
			}

			m_pointsInstant.pSSCItems[wCell] = pItem;
		}
		break;
	case ATTR_INVENTORY:
	{
		LPITEM pOld = m_pointsInstant.pSSAItems[wCell];
		if (pOld)
		{
			if (wCell < SPECIAL_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pOld->GetSize(); ++i)
				{
					int p = wCell + (i * 5);
					if (p >= SPECIAL_INVENTORY_MAX_NUM)
						continue;
					if (m_pointsInstant.pSSAItems[p] && m_pointsInstant.pSSAItems[p] != pOld)
						continue;
					m_pointsInstant.wSSAItemGrid[p] = 0;
				}
			}
			else
				m_pointsInstant.wSSAItemGrid[wCell] = 0;
		}
		if (pItem)
		{
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
			{
				sys_err("CHARACTER::SetItem: invalid SSA item cell %d", wCell);
				return;
			}
			if (wCell < SPECIAL_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pItem->GetSize(); ++i)
				{
					int p = wCell + (i * 5);
					if (p >= SPECIAL_INVENTORY_MAX_NUM)
						continue;
					m_pointsInstant.wSSAItemGrid[p] = wCell + 1;
				}
			}
			else
				m_pointsInstant.wSSAItemGrid[wCell] = wCell + 1;
		}
		m_pointsInstant.pSSAItems[wCell] = pItem;
	}
	break;
#endif
#ifdef ENABLE_SWITCHBOT
	case SWITCHBOT:
	{
		LPITEM pOld = m_pointsInstant.pSwitchbotItems[wCell];
		if (pItem && pOld)
		{
			return;
		}

		if (wCell >= SWITCHBOT_SLOT_COUNT)
		{
			sys_err("CHARACTER::SetItem: invalid switchbot item cell %d", wCell);
			return;
		}

		if (pItem)
		{
			CSwitchbotManager::Instance().RegisterItem(GetPlayerID(), pItem->GetID(), wCell);
		}
		else
		{
			CSwitchbotManager::Instance().UnregisterItem(GetPlayerID(), wCell);
		}

		m_pointsInstant.pSwitchbotItems[wCell] = pItem;
	}
	break;
#endif
	default:
		sys_err ("Invalid Inventory type %d", window_type);
		return;
	}

	if (GetDesc())
	{
		// 확장 아이템: 서버에서 아이템 플래그 정보를 보낸다
		if (pItem)
		{
			TPacketGCItemSet pack;
			pack.header = HEADER_GC_ITEM_SET;
			pack.Cell = Cell;

			pack.count = pItem->GetCount();
			
			pack.vnum = pItem->GetVnum();
			pack.flags = pItem->GetFlag();
			pack.anti_flags	= pItem->GetAntiFlag();
			pack.highlight = (Cell.window_type == DRAGON_SOUL_INVENTORY);


			thecore_memcpy(pack.alSockets, pItem->GetSockets(), sizeof(pack.alSockets));
			thecore_memcpy(pack.aAttr, pItem->GetAttributes(), sizeof(pack.aAttr));
			pack.evolution = pItem->GetEvolution();
			GetDesc()->Packet(&pack, sizeof(TPacketGCItemSet));
		}
		else
		{
			TPacketGCItemDelDeprecated pack;
			pack.header = HEADER_GC_ITEM_DEL;
			pack.Cell = Cell;
			pack.count = 0;
			pack.evolution = 0;
			pack.vnum = 0;
			memset(pack.alSockets, 0, sizeof(pack.alSockets));
			memset(pack.aAttr, 0, sizeof(pack.aAttr));

			GetDesc()->Packet(&pack, sizeof(TPacketGCItemDelDeprecated));
		}
	}

	if (pItem)
	{
		pItem->SetCell(this, wCell);
		switch (window_type)
		{
		case INVENTORY:
		case EQUIPMENT:
			if ((wCell < INVENTORY_MAX_NUM) || (BELT_INVENTORY_SLOT_START <= wCell && BELT_INVENTORY_SLOT_END > wCell))
				pItem->SetWindow(INVENTORY);
			else
				pItem->SetWindow(EQUIPMENT);
			break;
		case DRAGON_SOUL_INVENTORY:
			pItem->SetWindow(DRAGON_SOUL_INVENTORY);
			break;
#ifdef ENABLE_SPECIAL_STORAGE
		case UPGRADE_INVENTORY:
			pItem->SetWindow(UPGRADE_INVENTORY);
			break;
		case STONE_INVENTORY:
			pItem->SetWindow(STONE_INVENTORY);
			break;
		case CHEST_INVENTORY:
			pItem->SetWindow(CHEST_INVENTORY);
			break;
		case ATTR_INVENTORY:
			pItem->SetWindow(ATTR_INVENTORY);
			break;
#endif
#ifdef ENABLE_SWITCHBOT
		case SWITCHBOT:
			pItem->SetWindow(SWITCHBOT);
			break;
#endif
		}
	}
}

LPITEM CHARACTER::GetWear(BYTE bCell) const
{
	// > WEAR_MAX_NUM : 용혼석 슬롯들.
	if (bCell >= WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX)
	{
		sys_err("CHARACTER::GetWear: invalid wear cell %d", bCell);
		return NULL;
	}

	return m_pointsInstant.pItems[INVENTORY_MAX_NUM + bCell];
}

void CHARACTER::SetWear(BYTE bCell, LPITEM item)
{
	// > WEAR_MAX_NUM : 용혼석 슬롯들.
	if (bCell >= WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX)
	{
		sys_err("CHARACTER::SetItem: invalid item cell %d", bCell);
		return;
	}

	SetItem(TItemPos (INVENTORY, INVENTORY_MAX_NUM + bCell), item);

	/*if (!item && bCell == WEAR_WEAPON)
	{
		// 귀검 사용 시 벗는 것이라면 효과를 없애야 한다.
		if (IsAffectFlag(AFF_GWIGUM))
			RemoveAffect(SKILL_GWIGEOM);

		if (IsAffectFlag(AFF_GEOMGYEONG))
			RemoveAffect(SKILL_GEOMKYUNG);
	}*/
}

void CHARACTER::ClearItem()
{
	int		i;
	LPITEM	item;
	
	for (i = 0; i < INVENTORY_AND_EQUIP_SLOT_MAX; ++i)
	{
		if ((item = GetInventoryItem(i)))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);

			SyncQuickslot(QUICKSLOT_TYPE_ITEM, i, 255);
		}
	}
	for (i = 0; i < DRAGON_SOUL_INVENTORY_MAX_NUM; ++i)
	{
		if ((item = GetItem(TItemPos(DRAGON_SOUL_INVENTORY, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
#ifdef ENABLE_SPECIAL_STORAGE
	for (i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if ((item = GetItem(TItemPos(UPGRADE_INVENTORY, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
	for (i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if ((item = GetItem(TItemPos(STONE_INVENTORY, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
	for (i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if ((item = GetItem(TItemPos(CHEST_INVENTORY, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
	for (i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if ((item = GetItem(TItemPos(ATTR_INVENTORY, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);
			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
#endif
#ifdef ENABLE_SWITCHBOT
	for (i = 0; i < SWITCHBOT_SLOT_COUNT; ++i)
	{
		if ((item = GetItem(TItemPos(SWITCHBOT, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
#endif
}

bool CHARACTER::IsEmptyItemGrid(TItemPos Cell, BYTE bSize, int iExceptionCell) const
{
	switch (Cell.window_type)
	{
	case INVENTORY:
		{
			BYTE bCell = Cell.cell;

			// bItemCell은 0이 false임을 나타내기 위해 + 1 해서 처리한다.
			// 따라서 iExceptionCell에 1을 더해 비교한다.
			++iExceptionCell;

			if (Cell.IsBeltInventoryPosition())
			{
				LPITEM beltItem = GetWear(WEAR_BELT);

				if (NULL == beltItem)
					return false;

				if (false == CBeltInventoryHelper::IsAvailableCell(bCell - BELT_INVENTORY_SLOT_START, beltItem->GetValue(0)))
					return false;

				if (m_pointsInstant.bItemGrid[bCell])
				{
					if (m_pointsInstant.bItemGrid[bCell] == iExceptionCell)
						return true;

					return false;
				}

				if (bSize == 1)
					return true;

			}
			else if (bCell >= INVENTORY_MAX_NUM)
				return false;

			if (m_pointsInstant.bItemGrid[bCell])
			{
				if (m_pointsInstant.bItemGrid[bCell] == iExceptionCell)
				{
					if (bSize == 1)
						return true;

					int j = 1;
					BYTE bPage = bCell / (INVENTORY_MAX_NUM / 4);

					do
					{
						BYTE p = bCell + (5 * j);

						if (p >= INVENTORY_MAX_NUM)
							return false;

						if (p / (INVENTORY_MAX_NUM / 4) != bPage)
							return false;

						if (m_pointsInstant.bItemGrid[p])
							if (m_pointsInstant.bItemGrid[p] != iExceptionCell)
								return false;
					}
					while (++j < bSize);

					return true;
				}
				else
					return false;
			}

			// 크기가 1이면 한칸을 차지하는 것이므로 그냥 리턴
			if (1 == bSize)
				return true;
			else
			{
				int j = 1;
				BYTE bPage = bCell / (INVENTORY_MAX_NUM / 4);

				do
				{
					BYTE p = bCell + (5 * j);

					if (p >= INVENTORY_MAX_NUM)
						return false;

					if (p / (INVENTORY_MAX_NUM / 4) != bPage)
						return false;

					if (m_pointsInstant.bItemGrid[p])
						if (m_pointsInstant.bItemGrid[p] != iExceptionCell)
							return false;
				}
				while (++j < bSize);

				return true;
			}
		}
		break;
	case DRAGON_SOUL_INVENTORY:
		{
			WORD wCell = Cell.cell;
			if (wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
				return false;

			// bItemCell은 0이 false임을 나타내기 위해 + 1 해서 처리한다.
			// 따라서 iExceptionCell에 1을 더해 비교한다.
			iExceptionCell++;

			if (m_pointsInstant.wDSItemGrid[wCell])
			{
				if (m_pointsInstant.wDSItemGrid[wCell] == iExceptionCell)
				{
					if (bSize == 1)
						return true;

					int j = 1;

					do
					{
						BYTE p = wCell + (DRAGON_SOUL_BOX_COLUMN_NUM * j);

						if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
							return false;

						if (m_pointsInstant.wDSItemGrid[p])
							if (m_pointsInstant.wDSItemGrid[p] != iExceptionCell)
								return false;
					}
					while (++j < bSize);

					return true;
				}
				else
					return false;
			}

			// 크기가 1이면 한칸을 차지하는 것이므로 그냥 리턴
			if (1 == bSize)
				return true;
			else
			{
				int j = 1;

				do
				{
					BYTE p = wCell + (DRAGON_SOUL_BOX_COLUMN_NUM * j);

					if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
						return false;

					if (m_pointsInstant.bItemGrid[p])
						if (m_pointsInstant.wDSItemGrid[p] != iExceptionCell)
							return false;
				}
				while (++j < bSize);

				return true;
			}
		}
#ifdef ENABLE_SPECIAL_STORAGE
		break;
	case UPGRADE_INVENTORY:
		{
			WORD wCell = Cell.cell;
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
				return false;

			iExceptionCell++;

			if (m_pointsInstant.wSSUItemGrid[wCell])
			{
				if (m_pointsInstant.wSSUItemGrid[wCell] == iExceptionCell)
				{
					if (bSize == 1)
						return true;

					int j = 1;

					do
					{
						int p = wCell + (5 * j);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
							return false;

						if (m_pointsInstant.wSSUItemGrid[p])
							if (m_pointsInstant.wSSUItemGrid[p] != iExceptionCell)
								return false;
					}
					while (++j < bSize);

					return true;
				}
				else
					return false;
			}

			if (1 == bSize)
				return true;
			else
			{
				int j = 1;

				do
				{
					int p = wCell + (5 * j);

					if (p >= SPECIAL_INVENTORY_MAX_NUM)
						return false;

					if (m_pointsInstant.bItemGrid[p]) // old bItemGrid
						if (m_pointsInstant.wSSUItemGrid[p] != iExceptionCell)
							return false;
				}
				while (++j < bSize);

				return true;
			}
		}
		break;
	case STONE_INVENTORY:
		{
			WORD wCell = Cell.cell;
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
				return false;

			iExceptionCell++;

			if (m_pointsInstant.wSSSItemGrid[wCell])
			{
				if (m_pointsInstant.wSSSItemGrid[wCell] == iExceptionCell)
				{
					if (bSize == 1)
						return true;

					int j = 1;

					do
					{
						int p = wCell + (5 * j);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
							return false;

						if (m_pointsInstant.wSSSItemGrid[p])
							if (m_pointsInstant.wSSSItemGrid[p] != iExceptionCell)
								return false;
					}
					while (++j < bSize);

					return true;
				}
				else
					return false;
			}

			if (1 == bSize)
				return true;
			else
			{
				int j = 1;

				do
				{
					int p = wCell + (5 * j);

					if (p >= SPECIAL_INVENTORY_MAX_NUM)
						return false;

					if (m_pointsInstant.bItemGrid[p]) // old bItemGrid
						if (m_pointsInstant.wSSSItemGrid[p] != iExceptionCell)
							return false;
				}
				while (++j < bSize);

				return true;
			}
		}
	case CHEST_INVENTORY:
		{
			WORD wCell = Cell.cell;
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
				return false;

			iExceptionCell++;

			if (m_pointsInstant.wSSCItemGrid[wCell])
			{
				if (m_pointsInstant.wSSCItemGrid[wCell] == iExceptionCell)
				{
					if (bSize == 1)
						return true;

					int j = 1;

					do
					{
						int p = wCell + (5 * j);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
							return false;

						if (m_pointsInstant.wSSCItemGrid[p])
							if (m_pointsInstant.wSSCItemGrid[p] != iExceptionCell)
								return false;
					}
					while (++j < bSize);

					return true;
				}
				else
					return false;
			}

			if (1 == bSize)
				return true;
			else
			{
				int j = 1;

				do
				{
					int p = wCell + (5 * j);

					if (p >= SPECIAL_INVENTORY_MAX_NUM)
						return false;

					if (m_pointsInstant.bItemGrid[p]) // old bItemGrid
						if (m_pointsInstant.wSSCItemGrid[p] != iExceptionCell)
							return false;
				}
				while (++j < bSize);

				return true;
			}
		}
	case ATTR_INVENTORY:
		{
			WORD wCell = Cell.cell;
			if (wCell >= SPECIAL_INVENTORY_MAX_NUM)
				return false;

			iExceptionCell++;

			if (m_pointsInstant.wSSAItemGrid[wCell])
			{
				if (m_pointsInstant.wSSAItemGrid[wCell] == iExceptionCell)
				{
					if (bSize == 1)
						return true;

					int j = 1;

					do
					{
						int p = wCell + (5 * j);

						if (p >= SPECIAL_INVENTORY_MAX_NUM)
							return false;

						if (m_pointsInstant.wSSAItemGrid[p])
							if (m_pointsInstant.wSSAItemGrid[p] != iExceptionCell)
								return false;
					}
					while (++j < bSize);

					return true;
				}
				else
					return false;
			}

			if (1 == bSize)
				return true;
			else
			{
				int j = 1;

				do
				{
					int p = wCell + (5 * j);

					if (p >= SPECIAL_INVENTORY_MAX_NUM)
						return false;

					if (m_pointsInstant.bItemGrid[p]) // old bItemGrid
						if (m_pointsInstant.wSSAItemGrid[p] != iExceptionCell)
							return false;
				}
				while (++j < bSize);

				return true;
			}
		}
#endif
#ifdef ENABLE_SWITCHBOT
	case SWITCHBOT:
		{
		WORD wCell = Cell.cell;
		if (wCell >= SWITCHBOT_SLOT_COUNT)
		{
			return false;
		}

		if (m_pointsInstant.pSwitchbotItems[wCell])
		{
			return false;
		}

		return true;
		}
		break;
#endif
	}
	
	return false;
}

int CHARACTER::GetEmptyInventory(BYTE size) const
{
	// NOTE: 현재 이 함수는 아이템 지급, 획득 등의 행위를 할 때 인벤토리의 빈 칸을 찾기 위해 사용되고 있는데,
	//		벨트 인벤토리는 특수 인벤토리이므로 검사하지 않도록 한다. (기본 인벤토리: INVENTORY_MAX_NUM 까지만 검사)
	for ( int i = 0; i < INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos (INVENTORY, i), size))
			return i;
	return -1;
}

int CHARACTER::GetEmptyDragonSoulInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsDragonSoul())
		return -1;
	if (!DragonSoul_IsQualified())
	{
		return -1;
	}
	BYTE bSize = pItem->GetSize();
	WORD wBaseCell = DSManager::instance().GetBasePosition(pItem);

	if (WORD_MAX == wBaseCell)
		return -1;

#ifdef ENABLE_EXTENDED_DS_INVENTORY
	for (int i = 0; i < (DRAGON_SOUL_BOX_SIZE* DRAGON_SOUL_INVENTORY_PAGE_COUNT); ++i)
#else
	for (int i = 0; i < DRAGON_SOUL_BOX_SIZE; ++i)
#endif
	{
		if (IsEmptyItemGrid(TItemPos(DRAGON_SOUL_INVENTORY, i + wBaseCell), bSize))
			return i + wBaseCell;
	}

	return -1;
}

void CHARACTER::CopyDragonSoulItemGrid(std::vector<WORD>& vDragonSoulItemGrid) const
{
	vDragonSoulItemGrid.resize(DRAGON_SOUL_INVENTORY_MAX_NUM);

	std::copy(m_pointsInstant.wDSItemGrid, m_pointsInstant.wDSItemGrid + DRAGON_SOUL_INVENTORY_MAX_NUM, vDragonSoulItemGrid.begin());
}

#ifdef ENABLE_SPECIAL_STORAGE
int CHARACTER::GetSameUpgradeInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsUpgradeItem())
		return -1;

	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (GetUpgradeInventoryItem(i)->GetVnum() == pItem->GetVnum())
			return i;

	return -1;
}
int CHARACTER::GetSameStoneInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsStone())
		return -1;

	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (GetStoneInventoryItem(i)->GetVnum() == pItem->GetVnum())
			return i;

	return -1;
}
int CHARACTER::GetSameChestInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsChest())
		return -1;

	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (GetChestInventoryItem(i)->GetVnum() == pItem->GetVnum())
			return i;

	return -1;
}
int CHARACTER::GetSameAttrInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsAttr())
		return -1;

	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (GetAttrInventoryItem(i)->GetVnum() == pItem->GetVnum())
			return i;

	return -1;
}
int CHARACTER::GetEmptyUpgradeInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsUpgradeItem())
		return -1;
	
	BYTE bSize = pItem->GetSize();
	
	for ( int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos (UPGRADE_INVENTORY, i), bSize))
			return i;
		
	return -1;
}
int CHARACTER::GetEmptyStoneInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsStone())
		return -1;
	
	BYTE bSize = pItem->GetSize();
	
	for ( int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos (STONE_INVENTORY, i), bSize))
			return i;
		
	return -1;
}
int CHARACTER::GetEmptyChestInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsChest())
		return -1;
	BYTE bSize = pItem->GetSize();
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos(CHEST_INVENTORY, i), bSize))
			return i;
	return -1;
}

int CHARACTER::GetEmptyAttrInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsAttr())
		return -1;
	BYTE bSize = pItem->GetSize();
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos(ATTR_INVENTORY, i), bSize))
			return i;
	return -1;
}
//////////////////////////////////////////////////////
int CHARACTER::GetNewEmptyUpgradeInventory() const
{
	
	BYTE bSize = 1;
	int bosluk = 0;
	for ( int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos (UPGRADE_INVENTORY, i), bSize))
			bosluk = bosluk+1;
		
	return bosluk;
}
int CHARACTER::GetNewEmptyStoneInventory() const
{
	
	BYTE bSize = 1;
	int bosluk = 0;
	for ( int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos (STONE_INVENTORY, i), bSize))
			bosluk = bosluk+1;
		
	return bosluk;
}
int CHARACTER::GetNewEmptyChestInventory() const
{
	
	BYTE bSize = 1;
	int bosluk = 0;
	for ( int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos (CHEST_INVENTORY, i), bSize))
			bosluk = bosluk+1;
		
	return bosluk;
}
int CHARACTER::GetNewEmptyAttrInventory() const
{
	
	BYTE bSize = 1;
	int bosluk = 0;
	for ( int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos (ATTR_INVENTORY, i), bSize))
			bosluk = bosluk+1;
		
	return bosluk;
}
#endif
int CHARACTER::CountEmptyInventory() const
{
	int	count = 0;

	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
		if (GetInventoryItem(i))
			count += GetInventoryItem(i)->GetSize();

	return (INVENTORY_MAX_NUM - count);
}

void TransformRefineItem(LPITEM pkOldItem, LPITEM pkNewItem)
{
	// ACCESSORY_REFINE
	if (pkOldItem->IsAccessoryForSocket())
	{
		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		{
			pkNewItem->SetSocket(i, pkOldItem->GetSocket(i));
		}
		//pkNewItem->StartAccessorySocketExpireEvent();
	}
	// END_OF_ACCESSORY_REFINE
	else
	{
		// 여기서 깨진석이 자동적으로 청소 됨
		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		{
			if (!pkOldItem->GetSocket(i))
				break;
			else
				pkNewItem->SetSocket(i, 1);
		}

		// 소켓 설정
		int slot = 0;

		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		{
			long socket = pkOldItem->GetSocket(i);

			if (socket > 2 && socket != ITEM_BROKEN_METIN_VNUM)
				pkNewItem->SetSocket(slot++, socket);
		}

	}

	// 매직 아이템 설정
	pkOldItem->CopyAttributeTo(pkNewItem);
}

void NotifyRefineSuccess(LPCHARACTER ch, LPITEM item, const char* way)
{
	if (NULL != ch && item != NULL)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "RefineSuceeded");

		LogManager::instance().RefineLog(ch->GetPlayerID(), item->GetName(), item->GetID(), item->GetRefineLevel(), 1, way);
	}
}

void NotifyRefineFail(LPCHARACTER ch, LPITEM item, const char* way, int success = 0)
{
	if (NULL != ch && NULL != item)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "RefineFailed");

		LogManager::instance().RefineLog(ch->GetPlayerID(), item->GetName(), item->GetID(), item->GetRefineLevel(), success, way);
	}
}

void CHARACTER::SetRefineNPC(LPCHARACTER ch)
{
	if ( ch != NULL )
	{
		m_dwRefineNPCVID = ch->GetVID();
	}
	else
	{
		m_dwRefineNPCVID = 0;
	}
}

bool CHARACTER::DoRefine(LPITEM item, bool bMoneyOnly)
{
	if (!CanHandleItem(true))
	{
		ClearRefineMode();
		return false;
	}
	
	//개량 시간제한 : upgrade_refine_scroll.quest 에서 개량후 5분이내에 일반 개량을 
	//진행할수 없음
	if (quest::CQuestManager::instance().GetEventFlag("update_refine_time") != 0)
	{
		if (get_global_time() < quest::CQuestManager::instance().GetEventFlag("update_refine_time") + (60 * 5))
		{
			sys_log(0, "can't refine %d %s", GetPlayerID(), GetName());
			return false;
		}
	}

	const TRefineTable * prt = CRefineManager::instance().GetRefineRecipe(item->GetRefineSet());

	if (!prt)
		return false;

	DWORD result_vnum = item->GetRefinedVnum();

	// REFINE_COST
	int cost = ComputeRefineFee(prt->cost);

	int RefineChance = GetQuestFlag("main_quest_lv7.refine_chance");

	if (RefineChance > 0)
	{
		if (!item->CheckItemUseLevel(20) || item->GetType() != ITEM_WEAPON)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("무료 개량 기회는 20 이하의 무기만 가능합니다"));
			return false;
		}

		cost = 0;
		SetQuestFlag("main_quest_lv7.refine_chance", RefineChance - 1);
	}
	// END_OF_REFINE_COST

	if (result_vnum == 0)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("더 이상 개량할 수 없습니다."));
		return false;
	}

	if (item->GetType() == ITEM_USE && item->GetSubType() == USE_TUNING)
		return false;

	TItemTable * pProto = ITEM_MANAGER::instance().GetTable(item->GetRefinedVnum());

	if (!pProto)
	{
		sys_err("DoRefine NOT GET ITEM PROTO %d", item->GetRefinedVnum());
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 아이템은 개량할 수 없습니다."));
		return false;
	}

	// Check level limit in korea only
	if (!g_iUseLocale)
	{
		for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
		{
			long limit = pProto->aLimits[i].lValue;

			switch (pProto->aLimits[i].bType)
			{
				case LIMIT_LEVEL:
					if (GetLevel() < limit)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("개량된 후 아이템의 레벨 제한보다 레벨이 낮습니다."));
						return false;
					}
					break;
			}
		}
	}

	// REFINE_COST
	if (GetGold() < cost)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("개량을 하기 위한 돈이 부족합니다."));
		return false;
	}

	if (!bMoneyOnly && !RefineChance)
	{
		for (int i = 0; i < prt->material_count; ++i)
		{
			if (CountSpecifyItem(prt->materials[i].vnum) < prt->materials[i].count)
			{
				if (test_server)
				{
					ChatPacket(CHAT_TYPE_INFO, "Find %d, count %d, require %d", prt->materials[i].vnum, CountSpecifyItem(prt->materials[i].vnum), prt->materials[i].count);
				}
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("개량을 하기 위한 재료가 부족합니다."));
				return false;
			}
		}

		for (int i = 0; i < prt->material_count; ++i)
			RemoveSpecifyItem(prt->materials[i].vnum, prt->materials[i].count);
	}

	int prob = number(1, 100);
	if (IsRefineThroughGuild())
		prob -= 10;

	// END_OF_REFINE_COST

	if (prob <= prt->prob)
	{
		// 성공! 모든 아이템이 사라지고, 같은 속성의 다른 아이템 획득
		LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(result_vnum, 1, 0, false);

		if (pkNewItem)
		{
			ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
			
			if (item->GetEvolution() > 0)
				pkNewItem->SetEvolution(item->GetEvolution());
			
			LogManager::instance().ItemLog(this, pkNewItem, "REFINE SUCCESS", pkNewItem->GetName());

			BYTE bCell = item->GetCell();

			// DETAIL_REFINE_LOG
			NotifyRefineSuccess(this, item, IsRefineThroughGuild() ? "GUILD" : "POWER");
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -cost);
			ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE SUCCESS)");
			// END_OF_DETAIL_REFINE_LOG

			pkNewItem->AddToCharacter(this, TItemPos(INVENTORY, bCell)); 
			ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);

			sys_log(0, "Refine Success %d", cost);
			pkNewItem->AttrLog();
			//PointChange(POINT_GOLD, -cost);
			sys_log(0, "PayPee %d", cost);
			PayRefineFee(cost);
			sys_log(0, "PayPee End %d", cost);
		}
		else
		{
			// DETAIL_REFINE_LOG
			// 아이템 생성에 실패 -> 개량 실패로 간주
			sys_err("cannot create item %u", result_vnum);
			NotifyRefineFail(this, item, IsRefineThroughGuild() ? "GUILD" : "POWER");
			// END_OF_DETAIL_REFINE_LOG
		}
	}
	else
	{
		// 실패! 모든 아이템이 사라짐.
		DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -cost);
		NotifyRefineFail(this, item, IsRefineThroughGuild() ? "GUILD" : "POWER");
		item->AttrLog();
		ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE FAIL)");

		//PointChange(POINT_GOLD, -cost);
		PayRefineFee(cost);
	}

	return true;
}

enum enum_RefineScrolls
{
	CHUKBOK_SCROLL = 0,
	HYUNIRON_CHN   = 1, // 중국에서만 사용
	YONGSIN_SCROLL = 2,
	MUSIN_SCROLL   = 3,
	YAGONG_SCROLL  = 4,
	MEMO_SCROLL	   = 5,
	BDRAGON_SCROLL	= 6,
	RITUALS_SCROLL = 7,
};

bool CHARACTER::DoRefineWithScroll(LPITEM item)
{
	if (!CanHandleItem(true))
	{
		ClearRefineMode();
		return false;
	}

	ClearRefineMode();

	//개량 시간제한 : upgrade_refine_scroll.quest 에서 개량후 5분이내에 일반 개량을 
	//진행할수 없음
	if (quest::CQuestManager::instance().GetEventFlag("update_refine_time") != 0)
	{
		if (get_global_time() < quest::CQuestManager::instance().GetEventFlag("update_refine_time") + (60 * 5))
		{
			sys_log(0, "can't refine %d %s", GetPlayerID(), GetName());
			return false;
		}
	}

	const TRefineTable * prt = CRefineManager::instance().GetRefineRecipe(item->GetRefineSet());

	if (!prt)
		return false;

	LPITEM pkItemScroll;

	// 개량서 체크
	if (m_iRefineAdditionalCell < 0)
		return false;

	pkItemScroll = GetInventoryItem(m_iRefineAdditionalCell);

	if (!pkItemScroll)
		return false;

	if (!(pkItemScroll->GetType() == ITEM_USE && pkItemScroll->GetSubType() == USE_TUNING))
		return false;

	if (pkItemScroll->GetVnum() == item->GetVnum())
		return false;

	DWORD result_vnum = item->GetRefinedVnum();
	DWORD result_fail_vnum = item->GetRefineFromVnum();

	if (result_vnum == 0)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("더 이상 개량할 수 없습니다."));
		return false;
	}

	// MUSIN_SCROLL
	if (pkItemScroll->GetValue(0) == MUSIN_SCROLL)
	{
		if (item->GetRefineLevel() >= 4)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 개량서로 더 이상 개량할 수 없습니다."));
			return false;
		}
	}
	// END_OF_MUSIC_SCROLL

	else if (pkItemScroll->GetValue(0) == MEMO_SCROLL)
	{
		if (item->GetRefineLevel() != pkItemScroll->GetValue(1))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 개량서로 개량할 수 없습니다."));
			return false;
		}
	}
	else if (pkItemScroll->GetValue(0) == BDRAGON_SCROLL)
	{
		if (item->GetType() != ITEM_METIN || item->GetRefineLevel() != 4)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 아이템으로 개량할 수 없습니다."));
			return false;
		}
	}
	else if (pkItemScroll->GetValue(0) == RITUALS_SCROLL) {}

	TItemTable * pProto = ITEM_MANAGER::instance().GetTable(item->GetRefinedVnum());

	if (!pProto)
	{
		sys_err("DoRefineWithScroll NOT GET ITEM PROTO %d", item->GetRefinedVnum());
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 아이템은 개량할 수 없습니다."));
		return false;
	}

	// Check level limit in korea only
	if (!g_iUseLocale)
	{
		for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
		{
			long limit = pProto->aLimits[i].lValue;

			switch (pProto->aLimits[i].bType)
			{
				case LIMIT_LEVEL:
					if (GetLevel() < limit)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("개량된 후 아이템의 레벨 제한보다 레벨이 낮습니다."));
						return false;
					}
					break;
			}
		}
	}

	if (GetGold() < prt->cost)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("개량을 하기 위한 돈이 부족합니다."));
		return false;
	}

	for (int i = 0; i < prt->material_count; ++i)
	{
		if (CountSpecifyItem(prt->materials[i].vnum) < prt->materials[i].count)
		{
			if (test_server)
			{
				ChatPacket(CHAT_TYPE_INFO, "Find %d, count %d, require %d", prt->materials[i].vnum, CountSpecifyItem(prt->materials[i].vnum), prt->materials[i].count);
			}
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("개량을 하기 위한 재료가 부족합니다."));
			return false;
		}
	}

	for (int i = 0; i < prt->material_count; ++i)
		RemoveSpecifyItem(prt->materials[i].vnum, prt->materials[i].count);

	int prob = number(1, 100);
	int success_prob = prt->prob;
	bool bDestroyWhenFail = false;

	const char* szRefineType = "SCROLL";

	if (pkItemScroll->GetValue(0) == HYUNIRON_CHN || 
		pkItemScroll->GetValue(0) == YONGSIN_SCROLL || 
		pkItemScroll->GetValue(0) == YAGONG_SCROLL || 
		pkItemScroll->GetValue(0) == RITUALS_SCROLL) // 현철, 용신의 축복서, 야공의 비전서  처리
	{
		const char hyuniron_prob[9] = { 100, 75, 65, 55, 45, 40, 35, 25, 20 };
		const char hyuniron_prob_euckr[9] = { 100, 75, 65, 55, 45, 40, 35, 30, 25 };

		const char yagong_prob[9] = { 100, 100, 90, 80, 70, 60, 50, 30, 20 };
		const char yagong_prob_euckr[9] = { 100, 100, 90, 80, 70, 60, 50, 40, 30 };

		if (pkItemScroll->GetValue(0) == YONGSIN_SCROLL)
		{
			if (LC_IsYMIR() == true || LC_IsKorea() == true)
				success_prob = hyuniron_prob_euckr[MINMAX(0, item->GetRefineLevel(), 8)];
			else
				success_prob = hyuniron_prob[MINMAX(0, item->GetRefineLevel(), 8)];
		}
		else if (pkItemScroll->GetValue(0) == YAGONG_SCROLL)
		{
			if (LC_IsYMIR() == true || LC_IsKorea() == true)
				success_prob = yagong_prob_euckr[MINMAX(0, item->GetRefineLevel(), 8)];
			else
				success_prob = yagong_prob[MINMAX(0, item->GetRefineLevel(), 8)];
		}
		else if (pkItemScroll->GetValue(0) == RITUALS_SCROLL) {}
		else
		{
			sys_err("REFINE : Unknown refine scroll item. Value0: %d", pkItemScroll->GetValue(0));
		}

		if (test_server) 
		{
			ChatPacket(CHAT_TYPE_INFO, "[Only Test] Success_Prob %d, RefineLevel %d ", success_prob, item->GetRefineLevel());
		}
		if (pkItemScroll->GetValue(0) == HYUNIRON_CHN || pkItemScroll->GetValue(0) == RITUALS_SCROLL) // 현철은 아이템이 부서져야 한다.
			bDestroyWhenFail = true;

		// DETAIL_REFINE_LOG
		if (pkItemScroll->GetValue(0) == HYUNIRON_CHN)
		{
			szRefineType = "HYUNIRON";
		}
		else if (pkItemScroll->GetValue(0) == YONGSIN_SCROLL)
		{
			szRefineType = "GOD_SCROLL";
		}
		else if (pkItemScroll->GetValue(0) == YAGONG_SCROLL)
		{
			szRefineType = "YAGONG_SCROLL";
		}
		else if (pkItemScroll->GetValue(0) == RITUALS_SCROLL)
		{
			success_prob += pkItemScroll->GetValue(2);
			szRefineType = "RITUALS_SCROLL";
		}
		// END_OF_DETAIL_REFINE_LOG
	}

	// DETAIL_REFINE_LOG
	if (pkItemScroll->GetValue(0) == MUSIN_SCROLL) // 무신의 축복서는 100% 성공 (+4까지만)
	{
		success_prob = 100;

		szRefineType = "MUSIN_SCROLL";
	}
	// END_OF_DETAIL_REFINE_LOG
	else if (pkItemScroll->GetValue(0) == MEMO_SCROLL)
	{
		success_prob = 100;
		szRefineType = "MEMO_SCROLL";
	}
	else if (pkItemScroll->GetValue(0) == BDRAGON_SCROLL)
	{
		success_prob = 80;
		szRefineType = "BDRAGON_SCROLL";
	}

	pkItemScroll->SetCount(pkItemScroll->GetCount() - 1);

	if (prob <= success_prob)
	{
		// 성공! 모든 아이템이 사라지고, 같은 속성의 다른 아이템 획득
		LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(result_vnum, 1, 0, false);

		if (pkNewItem)
		{
			ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
			
			if (item->GetEvolution() > 0)
				pkNewItem->SetEvolution(item->GetEvolution());
			
			LogManager::instance().ItemLog(this, pkNewItem, "REFINE SUCCESS", pkNewItem->GetName());

			BYTE bCell = item->GetCell();

			NotifyRefineSuccess(this, item, szRefineType);
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -prt->cost);
			ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE SUCCESS)");

			pkNewItem->AddToCharacter(this, TItemPos(INVENTORY, bCell)); 
			ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);
			pkNewItem->AttrLog();
			//PointChange(POINT_GOLD, -prt->cost);
			PayRefineFee(prt->cost);
		}
		else
		{
			// 아이템 생성에 실패 -> 개량 실패로 간주
			sys_err("cannot create item %u", result_vnum);
			NotifyRefineFail(this, item, szRefineType);
		}
	}
	else if (!bDestroyWhenFail && result_fail_vnum)
	{
		// 실패! 모든 아이템이 사라지고, 같은 속성의 낮은 등급의 아이템 획득
		LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(result_fail_vnum, 1, 0, false);

		if (pkNewItem)
		{
			ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
			
			if (item->GetEvolution() > 0)
				pkNewItem->SetEvolution(item->GetEvolution());
			
			LogManager::instance().ItemLog(this, pkNewItem, "REFINE FAIL", pkNewItem->GetName());

			BYTE bCell = item->GetCell();

			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -prt->cost);
			NotifyRefineFail(this, item, szRefineType, -1);
			ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE FAIL)");

			pkNewItem->AddToCharacter(this, TItemPos(INVENTORY, bCell)); 
			ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);

			pkNewItem->AttrLog();

			//PointChange(POINT_GOLD, -prt->cost);
			PayRefineFee(prt->cost);
		}
		else
		{
			// 아이템 생성에 실패 -> 개량 실패로 간주
			sys_err("cannot create item %u", result_fail_vnum);
			NotifyRefineFail(this, item, szRefineType);
		}
	}
	else
	{
		NotifyRefineFail(this, item, szRefineType); // 개량시 아이템 사라지지 않음
		
		PayRefineFee(prt->cost);
	}

	return true;
}

bool CHARACTER::RefineInformation(BYTE bCell, BYTE bType, int iAdditionalCell)
{
	if (bCell > INVENTORY_MAX_NUM)
		return false;

	LPITEM item = GetInventoryItem(bCell);

	if (!item)
		return false;

	// REFINE_COST
	if (bType == REFINE_TYPE_MONEY_ONLY && !GetQuestFlag("deviltower_zone.can_refine"))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("사귀 타워 완료 보상은 한번까지 사용가능합니다."));
		return false;
	}
	// END_OF_REFINE_COST

	TPacketGCRefineInformation p;

	p.header = HEADER_GC_REFINE_INFORMATION;
	p.pos = bCell;
	p.src_vnum = item->GetVnum();
	p.result_vnum = item->GetRefinedVnum();
	p.type = bType;

	if (p.result_vnum == 0)
	{
		sys_err("RefineInformation p.result_vnum == 0");
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 아이템은 개량할 수 없습니다."));
		return false;
	}

	if (item->GetType() == ITEM_USE && item->GetSubType() == USE_TUNING)
	{
		if (bType == 0)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 아이템은 이 방식으로는 개량할 수 없습니다."));
			return false;
		}
		else
		{
			LPITEM itemScroll = GetInventoryItem(iAdditionalCell);
			if (!itemScroll || item->GetVnum() == itemScroll->GetVnum())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("같은 개량서를 합칠 수는 없습니다."));
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("축복의 서와 현철을 합칠 수 있습니다."));
				return false;
			}
		}
	}

	CRefineManager & rm = CRefineManager::instance();

	const TRefineTable* prt = rm.GetRefineRecipe(item->GetRefineSet());

	if (!prt)
	{
		sys_err("RefineInformation NOT GET REFINE SET %d", item->GetRefineSet());
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 아이템은 개량할 수 없습니다."));
		return false;
	}

	// REFINE_COST
	
	//MAIN_QUEST_LV7
	if (GetQuestFlag("main_quest_lv7.refine_chance") > 0)
	{
		// 일본은 제외
		if (!item->CheckItemUseLevel(20) || item->GetType() != ITEM_WEAPON)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("무료 개량 기회는 20 이하의 무기만 가능합니다"));
			return false;
		}
		p.cost = 0;
	}
	else
		p.cost = ComputeRefineFee(prt->cost);
	
	//END_MAIN_QUEST_LV7
	p.prob = prt->prob;
	if (bType == REFINE_TYPE_MONEY_ONLY)
	{
		p.material_count = 0;
		memset(p.materials, 0, sizeof(p.materials));
	}
	else
	{
		p.material_count = prt->material_count;
		thecore_memcpy(&p.materials, prt->materials, sizeof(prt->materials));
	}
	// END_OF_REFINE_COST

	GetDesc()->Packet(&p, sizeof(TPacketGCRefineInformation));

	SetRefineMode(iAdditionalCell);
	return true;
}

bool CHARACTER::RefineEvrimItem(LPITEM item)
{
	if (item->GetType() == ITEM_WEAPON)
	{
		char szEventFlag[30];
		snprintf(szEventFlag, sizeof(szEventFlag), "%d.Engel", item->GetID());
		if (*szEventFlag)
		{
			if (quest::CQuestManager::instance().GetEventFlag(szEventFlag))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("item_engel"));
				return false;
			}
		}
		
		if (!CanHandleItem(true))
		{
			return false;
		}

		if (item->GetEvolution() == 4)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("evrimsonseviye"));
			return false;
		}

									
		DWORD itemVnum = item->GetVnum();

		DWORD itemEvo = item->GetEvolution();

		DWORD needItemCount = itemEvo + 1;

		const DWORD rrNeedMoney[] = { 5000000, 10000000, 15000000, 20000000};
		const DWORD rrNeedItemVnums[] = { 70251, 70252, 70253, 70254 };
		const DWORD rrNeedPercents[] = { 50, 30, 20, 10};
		
		int rrPercent = rrNeedPercents[itemEvo];

		if (GetGold() < rrNeedMoney[itemEvo])
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("evrimyeterliparayok"));
			return false;
		}

		for (int i = 0; i < 4; i++)
		{
			if (CountSpecifyItem(rrNeedItemVnums[i]) < needItemCount)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("evrimneeditem"));
				return false;
				break;
			}
		}

		int prob = number(1, 100);

		if (prob <= rrPercent)
		{
			LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(itemVnum, 1, 0, false);

			if (pkNewItem)
			{
				ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);

				pkNewItem->SetEvolution(itemEvo + 1);

				LogManager::instance().ItemLog(this, pkNewItem, "REFINE SUCCESS", pkNewItem->GetName());

				UINT bCell = item->GetCell();

				DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -rrNeedMoney[itemEvo]);

				ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE SUCCESS)");

				pkNewItem->AddToCharacter(this, TItemPos(INVENTORY, bCell));

				ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);

				pkNewItem->AttrLog();

				PayRefineFee(rrNeedMoney[itemEvo]);

				for (int i = 0; i < 4; i++)
				{
					RemoveSpecifyItem(rrNeedItemVnums[i], needItemCount);
				}

				ChatPacket(CHAT_TYPE_COMMAND, "RefineSuceeded");
			}
		}
		else
		{
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -rrNeedMoney[itemEvo]);

			item->AttrLog();

			ChatPacket(CHAT_TYPE_COMMAND, "RefineFailed");

			PayRefineFee(rrNeedMoney[itemEvo]);
	
			for (int i = 0; i < 4; i++)
			{
				RemoveSpecifyItem(rrNeedItemVnums[i], needItemCount);
			}
		}

		RemoveSpecifyItem(72749, 1);
	}

	return true;
}

bool CHARACTER::RefineItem(LPITEM pkItem, LPITEM pkTarget)
{
	if (!CanHandleItem())
		return false;

	if (pkItem->GetSubType() == USE_TUNING)
	{
		// XXX 성능, 소켓 개량서는 사라졌습니다...
		// XXX 성능개량서는 축복의 서가 되었다!
		// MUSIN_SCROLL
		if (pkItem->GetValue(0) == MUSIN_SCROLL)
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_MUSIN, pkItem->GetCell());
		// END_OF_MUSIN_SCROLL
		else if (pkItem->GetValue(0) == HYUNIRON_CHN)
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_HYUNIRON, pkItem->GetCell());
		else if (pkItem->GetValue(0) == BDRAGON_SCROLL)
		{
			if (pkTarget->GetRefineSet() != 702) return false;
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_BDRAGON, pkItem->GetCell());
		}
		else if (pkItem->GetValue(0) == RITUALS_SCROLL)
		{
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_RITUALS_SCROLL, pkItem->GetCell());
		}
		else
		{
			if (pkTarget->GetRefineSet() == 501) return false;
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_SCROLL, pkItem->GetCell());
		}
	}
	else if (pkItem->GetSubType() == USE_DETACHMENT && IS_SET(pkTarget->GetFlag(), ITEM_FLAG_REFINEABLE))
	{
		LogManager::instance().ItemLog(this, pkTarget, "USE_DETACHMENT", pkTarget->GetName());

		bool bHasMetinStone = false;

		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
		{
			long socket = pkTarget->GetSocket(i);
			if (socket > 2 && socket != ITEM_BROKEN_METIN_VNUM)
			{
				bHasMetinStone = true;
				break;
			}
		}

		if (bHasMetinStone)
		{
			for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
			{
				long socket = pkTarget->GetSocket(i);
				if (socket > 2 && socket != ITEM_BROKEN_METIN_VNUM)
				{
					AutoGiveItem(socket);
					//TItemTable* pTable = ITEM_MANAGER::instance().GetTable(pkTarget->GetSocket(i));
					//pkTarget->SetSocket(i, pTable->alValues[2]);
					// 깨진돌로 대체해준다
					pkTarget->SetSocket(i, ITEM_BROKEN_METIN_VNUM);
				}
			}
			pkItem->SetCount(pkItem->GetCount() - 1);
			return true;
		}
		else
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("빼낼 수 있는 메틴석이 없습니다."));
			return false;
		}
	}

	return false;
}

EVENTFUNC(kill_campfire_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "kill_campfire_event> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER	ch = info->ch;

	if (ch == NULL) { // <Factor>
		return 0;
	}
	ch->m_pkMiningEvent = NULL;
	M2_DESTROY_CHARACTER(ch);
	return 0;
}

bool CHARACTER::GiveRecallItem(LPITEM item)
{
	int idx = GetMapIndex();
	int iEmpireByMapIndex = -1;

	if (idx < 20)
		iEmpireByMapIndex = 1;
	else if (idx < 40)
		iEmpireByMapIndex = 2;
	else if (idx < 60)
		iEmpireByMapIndex = 3;
	else if (idx < 10000)
		iEmpireByMapIndex = 0;

	switch (idx)
	{
		case 66:
		case 216:
			iEmpireByMapIndex = -1;
			break;
	}

	if (iEmpireByMapIndex && GetEmpire() != iEmpireByMapIndex)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("기억해 둘 수 없는 위치 입니다."));
		return false;
	}

	int pos;

	if (item->GetCount() == 1)	// 아이템이 하나라면 그냥 셋팅.
	{
		item->SetSocket(0, GetX());
		item->SetSocket(1, GetY());
	}
	else if ((pos = GetEmptyInventory(item->GetSize())) != -1) // 그렇지 않다면 다른 인벤토리 슬롯을 찾는다.
	{
		LPITEM item2 = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), 1);

		if (NULL != item2)
		{
			item2->SetSocket(0, GetX());
			item2->SetSocket(1, GetY());
			item2->AddToCharacter(this, TItemPos(INVENTORY, pos));

			item->SetCount(item->GetCount() - 1);
		}
	}
	else
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소지품에 빈 공간이 없습니다."));
		return false;
	}

	return true;
}

void CHARACTER::ProcessRecallItem(LPITEM item)
{
	int idx;

	if ((idx = SECTREE_MANAGER::instance().GetMapIndex(item->GetSocket(0), item->GetSocket(1))) == 0)
		return;

	int iEmpireByMapIndex = -1;

	if (idx < 20)
		iEmpireByMapIndex = 1;
	else if (idx < 40)
		iEmpireByMapIndex = 2;
	else if (idx < 60)
		iEmpireByMapIndex = 3;
	else if (idx < 10000)
		iEmpireByMapIndex = 0;

	switch (idx)
	{
		case 66:
		case 216:
			iEmpireByMapIndex = -1;
			break;
		// 악룡군도 일때
		case 301:
		case 302:
		case 303:
		case 304:
			if( GetLevel() < 90 )
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템의 레벨 제한보다 레벨이 낮습니다."));
				return;
			}
			else
				break;
	}

	if (iEmpireByMapIndex && GetEmpire() != iEmpireByMapIndex)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("기억된 위치가 타제국에 속해 있어서 귀환할 수 없습니다."));
		item->SetSocket(0, 0);
		item->SetSocket(1, 0);
	}
	else
	{
		sys_log(1, "Recall: %s %d %d -> %d %d", GetName(), GetX(), GetY(), item->GetSocket(0), item->GetSocket(1));
		WarpSet(item->GetSocket(0), item->GetSocket(1));
		//item->SetCount(item->GetCount() - 1);
	}
}

void CHARACTER::__OpenPrivateShop()
{
	unsigned bodyPart = GetPart(PART_MAIN);
	switch (bodyPart)
	{
		case 0:
		case 1:
		case 2:
			ChatPacket(CHAT_TYPE_COMMAND, "OpenPrivateShop");
			break;
		default:
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("갑옷을 벗어야 개인 상점을 열 수 있습니다."));
			break;
	}
}

void CHARACTER::__OpenOfflineShop()
{
	// If character is dead, return false
	if (IsDead())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Dead man can not open offline shop"));
		return;
	}

	if (IsSashCombinationOpen())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("kombinasyon_penceresi_acik"));
		return;
	}
	
	if (IsSashAbsorptionOpen())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bonus_emis_penceresi_acik"));
		return;
	}
	
	// If character is exchanging with someone, return false
	if (GetExchange())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You are exchanging with someone. At first close to the window!"));
		return;
	}

	// If character has a private shop, return false
	if (GetMyShop())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You had open private shop. At first you must be close this private shop!"));
		return;
	}

	// If character is look at one offline shop, return false
	if (GetOfflineShop())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't open offline shop because you are already look at one offline shop."));
		return;
	}

	// If cube window is open, return false
	if (IsCubeOpen())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't open offline shop because your cube window is open"));
		return;
	}

	// If character's safebox is open, return false
	if (IsOpenSafebox())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't open offline shop because your safebox window is open!"));
		return;
	}

	// If character's shop window is open, return false
	if (GetShop())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't open offline shop because your shop window is open!"));
		return;
	}
	
	if (GetMapIndex() == 1 || GetMapIndex() == 21 || GetMapIndex() == 41)
	{
		if (GetOfflineShopCount() > g_maxOfflineShopCount)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bu haritada pazar sinirina ulasildi."));
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("2. koy veya baska bir kanalda kurabilirsiiniz."));
			return;
		}
	}
	
	ChatPacket(CHAT_TYPE_COMMAND, "OpenOfflineShop");
}
// MYSHOP_PRICE_LIST
#ifdef ENABLE_CHEQUE_SYSTEM
void CHARACTER::SendMyShopPriceListCmd(DWORD dwItemVnum, DWORD dwItemPrice, DWORD dwItemPriceCheque)
#else
void CHARACTER::SendMyShopPriceListCmd(DWORD dwItemVnum, DWORD dwItemPrice)
#endif
{
	char szLine[256];
#ifdef ENABLE_CHEQUE_SYSTEM
	snprintf(szLine, sizeof(szLine), "MyShopPriceList %u %u %u", dwItemVnum, dwItemPrice, dwItemPriceCheque);
#else
	snprintf(szLine, sizeof(szLine), "MyShopPriceList %u %u %u", dwItemVnum, dwItemPrice);
#endif
	ChatPacket(CHAT_TYPE_COMMAND, szLine);
	sys_log(0, szLine);
}

#ifdef ENABLE_NEW_AFFECT_POTION
void CHARACTER::SetAffectPotion(LPITEM item)
{
	int listAffectPotion[] =
	{
		AFFECT_POTION_1, AFFECT_POTION_2, AFFECT_POTION_3, AFFECT_POTION_4, AFFECT_POTION_5, AFFECT_POTION_6,AFFECT_ENERGY, AFFECT_POTION_7, AFFECT_POTION_8, AFFECT_POTION_9};

	int listVnums[] =
	{
		50821, 50822, 50823, 50824, 50825, 50826,51002, 50827, 50828, 50829};
	
	for (int i = 0; i < _countof(listVnums); i++)
	{
		if (item->GetVnum() == listVnums[i])
		{
			AddAffect(listAffectPotion[i], APPLY_NONE, item->GetSocket(1), item->GetID(), item->GetSocket(2), 0, false, false);
		}
	}
	
	int listAffectPotion2[] =
	{
		AFFECT_POTION_1, AFFECT_POTION_2, AFFECT_POTION_3, AFFECT_POTION_4, AFFECT_POTION_5, AFFECT_POTION_6,AFFECT_ENERGY, AFFECT_KRITIK, AFFECT_DELICI, AFFECT_E1, AFFECT_E2, AFFECT_E3, AFFECT_E4, AFFECT_YESIL, AFFECT_MOR, AFFECT_POTION_7, AFFECT_POTION_8, AFFECT_POTION_9};

	int listVnums2[] =
	{
		71510, 71511, 71512, 71513, 71514, 71515, 71525, 71516, 71517, 71518, 71519, 71523, 71524, 71526, 71527, 71528, 71529, 71530};
	
	for (int i = 0; i < _countof(listVnums2); i++)
	{
		if (item->GetVnum() == listVnums2[i])
		{
			AddAffect(listAffectPotion2[i], APPLY_NONE, item->GetValue(1), item->GetID(), INFINITE_AFFECT_DURATION, 0, false, false);
		}
	}
}

void CHARACTER::RemoveAffectPotion(LPITEM item)
{
	int listAffectPotion[] =
	{
		AFFECT_POTION_1, AFFECT_POTION_2, AFFECT_POTION_3, AFFECT_POTION_4, AFFECT_POTION_5, AFFECT_POTION_6,AFFECT_ENERGY, AFFECT_POTION_7, AFFECT_POTION_8, AFFECT_POTION_9};

	int listVnums[] =
	{
		50821, 50822, 50823, 50824, 50825, 50826,51002, 50827, 50828, 50829};
	
	for (int i = 0; i < _countof(listVnums); i++)
	{
		if (item->GetVnum() == listVnums[i])
		{
			RemoveAffect(listAffectPotion[i]);
		}
	}
	
	int listAffectPotion2[] =
	{
		AFFECT_POTION_1, AFFECT_POTION_2, AFFECT_POTION_3, AFFECT_POTION_4, AFFECT_POTION_5, AFFECT_POTION_6,AFFECT_ENERGY, AFFECT_KRITIK, AFFECT_DELICI, AFFECT_E1, AFFECT_E2, AFFECT_E3, AFFECT_E4, AFFECT_YESIL, AFFECT_MOR, AFFECT_POTION_7, AFFECT_POTION_8, AFFECT_POTION_9};

	int listVnums2[] =
	{
		71510, 71511, 71512, 71513, 71514, 71515, 71525, 71516, 71517, 71518, 71519, 71523, 71524, 71526, 71527, 71528, 71529, 71530};
	
	for (int i = 0; i < _countof(listVnums2); i++)
	{
		if (item->GetVnum() == listVnums2[i])
		{
			RemoveAffect(listAffectPotion2[i]);
		}
	}
}

bool CHARACTER::IsAffectItem(DWORD vnum)
{
  
	if (vnum == 71044)
    {
        return true;
	}
	else if (vnum == 71045)	
	{
        return true;
	}
	else if (vnum == 71027)	
	{
        return true;
	}
	else if (vnum == 71028)	
	{
        return true;
	}
	else if (vnum == 71029)	
	{
        return true;
	}
	else if (vnum == 71030)	
	{
        return true;
	}
	else if (vnum == 51002)	
	{
        return true;
	}
	else if (vnum == 71518)	
	{
        return true;
	}
	else if (vnum == 71519)	
	{
        return true;
	}
	else if (vnum == 71523)	
	{
        return true;
	}
	else if (vnum == 71524)	
	{
        return true;
	}
	else if (vnum == 71526)	
	{
        return true;
	}
	else if (vnum == 71527)	
	{
        return true;
	}
	else if (vnum == 71516)	
	{
        return true;
	}
	else if (vnum == 71517)	
	{
        return true;
	}
	return false;
}

int GetAffectBonus(DWORD vnum)
{
  
	if (vnum == 71044)
    {
        return AFFECT_KRITIK;
	}
	else if (vnum == 71045)	
	{
        return AFFECT_DELICI;
	}
	else if (vnum == 71516)
    {
        return AFFECT_KRITIK;
	}
	else if (vnum == 71517)	
	{
        return AFFECT_DELICI;
	}
	else if (vnum == 71518)	
	{
        return AFFECT_E1;
	}
	else if (vnum == 71519)	
	{
        return AFFECT_E2;
	}
	else if (vnum == 71523)	
	{
        return AFFECT_E3;
	}
	else if (vnum == 71524)	
	{
        return AFFECT_E4;
	}
	else if (vnum == 71027)	
	{
        return AFFECT_E1;
	}
	else if (vnum == 71028)	
	{
        return AFFECT_E2;
	}
	else if (vnum == 71029)	
	{
        return AFFECT_E3;
	}
	else if (vnum == 71030)	
	{
        return AFFECT_E4;
	}
	else if (vnum == 50821)	
	{
        return AFFECT_POTION_1;
	}
	else if (vnum == 50822)	
	{
        return AFFECT_POTION_2;
	}
	else if (vnum == 50823)	
	{
        return AFFECT_POTION_3;
	}
	else if (vnum == 50824)	
	{
        return AFFECT_POTION_4;
	}
	else if (vnum == 50825)	
	{
        return AFFECT_POTION_5;
	}
	else if (vnum == 50826)	
	{
        return AFFECT_POTION_6;
	}
	else if (vnum == 50827)	
	{
        return AFFECT_POTION_7;
	}
	else if (vnum == 50828)	
	{
        return AFFECT_POTION_8;
	}
	else if (vnum == 50829)	
	{
        return AFFECT_POTION_9;
	}
	else if (vnum == 51002)	
	{
        return AFFECT_ENERGY;
	}
	else if (vnum == 71526)	
	{
        return AFFECT_YESIL;
	}
	else if (vnum == 71527)	
	{
        return AFFECT_MOR;
	}
	return 0;
}

#endif


//
// DB 캐시로 부터 받은 리스트를 User 에게 전송하고 상점을 열라는 커맨드를 보낸다.
//
void CHARACTER::UseSilkBotaryReal(const TPacketMyshopPricelistHeader* p)
{
	const TItemPriceInfo* pInfo = (const TItemPriceInfo*)(p + 1);

#ifdef ENABLE_CHEQUE_SYSTEM
	if (!p->byCount)
		SendMyShopPriceListCmd(1, 0, 0);
	else {
		for (int idx = 0; idx < p->byCount; idx++)
			SendMyShopPriceListCmd(pInfo[ idx ].dwVnum, pInfo[ idx ].dwPrice, pInfo[ idx ].dwPriceCheque);
	}
#else
	if (!p->byCount)
		SendMyShopPriceListCmd(1, 0);
	else {
		for (int idx = 0; idx < p->byCount; idx++)
			SendMyShopPriceListCmd(pInfo[ idx ].dwVnum, pInfo[ idx ].dwPrice);
	}
#endif

	__OpenOfflineShop();
}

//
// 이번 접속 후 처음 상점을 Open 하는 경우 리스트를 Load 하기 위해 DB 캐시에 가격정보 리스트 요청 패킷을 보낸다.
// 이후부터는 바로 상점을 열라는 응답을 보낸다.
//
void CHARACTER::UseSilkBotary(void)
{
	if (m_bNoOpenedShop) {
		DWORD dwPlayerID = GetPlayerID();
		db_clientdesc->DBPacket(HEADER_GD_MYSHOP_PRICELIST_REQ, GetDesc()->GetHandle(), &dwPlayerID, sizeof(DWORD));
		m_bNoOpenedShop = false;
	} else {
		__OpenOfflineShop();
	}
}
// END_OF_MYSHOP_PRICE_LIST

int CalculateConsume(LPCHARACTER ch)
{
	static const int WARP_NEED_LIFE_PERCENT	= 30;
	static const int WARP_MIN_LIFE_PERCENT	= 10;
	// CONSUME_LIFE_WHEN_USE_WARP_ITEM
	int consumeLife = 0;
	{
		// CheckNeedLifeForWarp
		const int curLife = ch->GetHP();
		const int needPercent = WARP_NEED_LIFE_PERCENT;
		const int needLife = (ch->GetMaxHP() / 100) * needPercent;
		if (curLife < needLife)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("남은 생명력 양이 모자라 사용할 수 없습니다."));
			return -1;
		}

		consumeLife = needLife;

		// CheckMinLifeForWarp: 독에 의해서 죽으면 안되므로 생명력 최소량는 남겨준다
		const int minPercent = WARP_MIN_LIFE_PERCENT;
		const int minLife = (ch->GetMaxHP() / 100) * minPercent;
		if (curLife - needLife < minLife)
			consumeLife = curLife - minLife;

		if (consumeLife < 0)
			consumeLife = 0;
	}

	// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM
	return consumeLife;
}

int CalculateConsumeSP(LPCHARACTER lpChar)
{
	static const int NEED_WARP_SP_PERCENT = 30;

	const int curSP = lpChar->GetSP();
	const int needSP = lpChar->GetMaxSP() * NEED_WARP_SP_PERCENT / 100;

	if (curSP < needSP)
	{
		lpChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("남은 정신력 양이 모자라 사용할 수 없습니다."));
		return -1;
	}

	return needSP;
}
#ifdef ENABLE_YMIR_AFFECT_FIX
bool CHARACTER::CheckTimeUsed(LPITEM item)
{
    switch (item->GetVnum())
    {
/****************
* Type = 27 -> ITEM_BLEND (Delete the other case you do not want for check)
*/  
        case 50821:    case 50822:    case 50823:    case 50824:    case 50825:    case 50826:
        case 50827:    case 50828:    case 50829:

/****************
* SubType = 7 -> USE_ABILITY_UP (Delete the other case you do not want for check)
*/  
        case 27866:    case 27868:    case 27870:    case 27873:    case 39026:    case 50093:    case 50094:      
        case 50123:    case 50801:    case 50802:    case 50817:    case 50818:    case 50819:    case 50820:  
		case 27863:
		case 27864:
		case 27865:
		case 27867:
		case 27869:
		case 27871:
		case 27872:
		case 27874:
		case 27875:
		case 27876:
		case 27877:
		case 27878:
		case 27879:
		case 27880:
		case 27881:
		case 27882:
		case 27883:
/****************
* SubType = 8 -> USE_AFFECT (Delete the other case you do not want for check)
*/  
        case 39010:    case 39017:    case 39018:    case 39019:    case 39020:    case 39024:    case 39025:
        case 39031:    case 50813:    case 50814:    case 71014:    case 71015:    case 71016:    case 71017:
        case 71027:    case 71028:    case 71029:    case 71030:    case 71034:    case 71044:    case 71045:
        case 71101:    case 71102:    case 71153:    case 71154:    case 71155:    case 71156:    case 72025:
        case 72026:    case 72027:    case 72031:    case 72032:    case 72033:    case 72034:    case 72035:  
        case 72036:    case 72037:    case 72038:    case 72039:    case 72040:    case 72041:    case 72042:  
        case 72046:    case 72047:    case 72048:    case 72312:    case 72313:    case 72501:    case 72502:  
        case 76003:    case 76017:    case 76018:

            int pGetTime[] = {10}; // Set timer for how you need a long after you login for can use item
           
            int pGetFlag = GetQuestFlag("item.last_time"); // Get questflag where him set from input_login.cpp (Not change)
           
            const char* pGetMessage[] = {"cokhizlipot"}; // Get message when you use time so fast
           
            if (pGetFlag) // Initializate questflag on item
            {
                if (get_global_time() < pGetFlag + pGetTime[0]) // Initializate to get a + second for questflag 
                {
                    ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cokhizlipot"));    // Get message
                    return false; // Returns false if you use the item quicker than those seconds
                }
            }  
        break;  
    }  
        return true;
}
#endif
bool CHARACTER::UseItemEx(LPITEM item, TItemPos DestCell, bool bUseAll)
{
	int iLimitRealtimeStartFirstUseFlagIndex = -1;
	int iLimitTimerBasedOnWearFlagIndex = -1;

	WORD wDestCell = DestCell.cell;
	BYTE bDestInven = DestCell.window_type;
	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		long limitValue = item->GetProto()->aLimits[i].lValue;

		switch (item->GetProto()->aLimits[i].bType)
		{
			case LIMIT_LEVEL:
				if (GetLevel() < limitValue)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템의 레벨 제한보다 레벨이 낮습니다."));
					return false;
				}
				break;

			case LIMIT_REAL_TIME_START_FIRST_USE:
				iLimitRealtimeStartFirstUseFlagIndex = i;
				break;

			case LIMIT_TIMER_BASED_ON_WEAR:
				iLimitTimerBasedOnWearFlagIndex = i;
				break;
		}
	}

	if (test_server)
	{
		sys_log(0, "USE_ITEM %s, Inven %d, Cell %d, ItemType %d, SubType %d", item->GetName(), bDestInven, wDestCell, item->GetType(), item->GetSubType());
	}

	if ( CArenaManager::instance().IsLimitedItem( GetMapIndex(), item->GetVnum() ) == true )
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련 중에는 이용할 수 없는 물품입니다."));
		return false;
	}

	// 아이템 최초 사용 이후부터는 사용하지 않아도 시간이 차감되는 방식 처리. 
	if (-1 != iLimitRealtimeStartFirstUseFlagIndex)
	{
		// 한 번이라도 사용한 아이템인지 여부는 Socket1을 보고 판단한다. (Socket1에 사용횟수 기록)
		if (0 == item->GetSocket(1))
		{
			// 사용가능시간은 Default 값으로 Limit Value 값을 사용하되, Socket0에 값이 있으면 그 값을 사용하도록 한다. (단위는 초)
			long duration = (0 != item->GetSocket(0)) ? item->GetSocket(0) : item->GetProto()->aLimits[iLimitRealtimeStartFirstUseFlagIndex].lValue;

			if (0 == duration)
				duration = 60 * 60 * 24 * 7;

			item->SetSocket(0, time(0) + duration);
			item->StartRealTimeExpireEvent();
		}	

		if (false == item->IsEquipped())
			item->SetSocket(1, item->GetSocket(1) + 1);
	}

#ifdef ENABLE_NEW_PET_SYSTEM
	if (item->GetVnum() == 55009)
	{
		int number1461 = number(55010, 55027);
		AutoGiveItem(number1461);
		item->SetCount(item->GetCount() - 1);
	}
	if (item->GetVnum() == 55400)
	{
		int number1461 = number(55401, 55410);
		AutoGiveItem(number1461);
		item->SetCount(item->GetCount() - 1);
	}
	if (item->GetVnum() == 55029)
	{
		CNewPetSystem* petSystem;
		if (!(petSystem = GetNewPetSystem()))
			return false;

		for (int i = 0; i < 3; i++)
			petSystem->ResetSkill(i);

		petSystem->RefreshBuff();

		if (petSystem->IsActivePet())
		{
			item->SetCount(item->GetCount() - 1);
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_REVERTUS_OKEY"));
		}
		else
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_WITH_PET_SUMMON"));

		return true;
	}
	if ((item->GetVnum() >= 55010 && item->GetVnum() <= 55027) || (item->GetVnum() >= 55034 && item->GetVnum() <= 55039))
	{
		int skill = item->GetValue(0);
		CNewPetSystem* petSystem;
		if (!(petSystem = GetNewPetSystem()))
			return false;

		bool ret = petSystem->IncreasePetSkill(skill);
		if (ret && petSystem->IsActivePet())
			item->SetCount(item->GetCount() - 1);
		else
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_WITH_PET_SUMMON"));
	}
	if (item->GetVnum() == 55001) {
		LPITEM item2;

		if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
			return false;

		if (item2->IsExchanging())
			return false;

		if (item2->GetVnum() > 55710 || item2->GetVnum() < 55701)
			return false;

		char szQuery1[1024];
		snprintf(szQuery1, sizeof(szQuery1), "SELECT duration,tduration FROM new_petsystem WHERE id = %lu ", item2->GetID());
		std::unique_ptr<SQLMsg> pmsg2(DBManager::instance().DirectQuery(szQuery1));
		if (pmsg2->Get()->uiNumRows > 0)
		{
			MYSQL_ROW row = mysql_fetch_row(pmsg2->Get()->pSQLResult);
			int suankiDuration = atoi(row[0]);

			if (suankiDuration >= get_global_time())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_ALREADY_YOUNG"));
				return false;
			}

			int insertduration0 = time(0) + atoi(row[1]);
			int insertduration1 = time(0) + (atoi(row[2]) / 2);
			if (atoi(row[0]) > 0)
			{
				std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET duration =%d WHERE id = %lu", insertduration0, item2->GetID()));
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PSS_PROTEIN_D1"));
			}
			else
			{
				std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET duration =%d WHERE id = %lu", insertduration1, item2->GetID()));
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PSS_PROTEIN_D2"));
			}

			item->SetCount(item->GetCount() - 1);
			return true;
		}
		else
			return false;
	}
	if (item->GetVnum() == 55101) {
		LPITEM item2;

		if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
			return false;

		if (item2->IsExchanging())
			return false;

		if (item2->GetVnum() > 55710 || item2->GetVnum() < 55701)
			return false;

		char szQuery1[1024];
		snprintf(szQuery1, sizeof(szQuery1), "SELECT duration,tduration FROM new_petsystem WHERE id = %lu ", item2->GetID());
		std::unique_ptr<SQLMsg> pmsg2(DBManager::instance().DirectQuery(szQuery1));
		if (pmsg2->Get()->uiNumRows > 0)
		{
			MYSQL_ROW row = mysql_fetch_row(pmsg2->Get()->pSQLResult);
			int suankiDuration = atoi(row[0]);

			if (suankiDuration >= get_global_time())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_ALREADY_YOUNG"));
				return false;
			}

			int insertduration0 = time(0) + atoi(row[1]) * 2;
			int insertduration1 = time(0) + (atoi(row[2]));
			if (atoi(row[0]) > 0)
			{
				std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET duration =%d WHERE id = %lu", insertduration0, item2->GetID()));
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PSS_PROTEIN_D1"));
			}
			else
			{
				std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET duration =%d WHERE id = %lu", insertduration1, item2->GetID()));
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PSS_PROTEIN_D2"));
			}

			item->SetCount(item->GetCount() - 1);
			return true;
		}
		else
			return false;
	}

	if (item->GetVnum() >= 55701 && item->GetVnum() <= 55710) {
		LPITEM item2 = GetItem(DestCell);

		if (item2) {
			if (item2->GetVnum() == 55002) {
				if (item2->GetAttributeValue(0) > 0) {
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Zaten iceride bir hayvan kutusu bulunuyor."));
				}
				else if (GetLevel() < item->GetSocket(1))
				{
					ChatPacket(CHAT_TYPE_INFO, "Pet seviyesi senin seviyenden yuksek.");
					ChatPacket(CHAT_TYPE_INFO, "Pet ile ayni seviyeye ulasana dek peti kutuya koyamazsin!");
					return false;
				}
				else if (item->GetSocket(2) < 1)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_SURESI_BITMISKEN_BUNU_YAPAMASSIN"));
					return false;
				}
				else {
					if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
						return false;

					if (item2->IsExchanging())
						return false;

					if (GetNewPetSystem()->IsActivePet()) {
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Once diger pet'i gonder."));
						return false;
					}

					for (int b = 0; b < 3; b++) {
						item2->SetForceAttribute(b, 1, item->GetAttributeValue(b));
					}

					item2->SetForceAttribute(4, 1, item->GetAttributeValue(4));
					item2->SetForceAttribute(5, 1, item->GetAttributeValue(5));
					item2->SetForceAttribute(6, 1, item->GetAttributeValue(6));
					item2->SetForceAttribute(7, 1, item->GetAttributeValue(7));
					item2->SetForceAttribute(8, 1, item->GetAttributeValue(8));
					item2->SetForceAttribute(9, 1, item->GetAttributeValue(9));
					item2->SetForceAttribute(10, 1, item->GetAttributeValue(10));
					item2->SetForceAttribute(11, 1, item->GetAttributeValue(11));
					item2->SetForceAttribute(12, 1, item->GetAttributeValue(12));
					item2->SetForceAttribute(13, 1, item->GetAttributeValue(13));
					item2->SetForceAttribute(14, 1, item->GetAttributeValue(14));
					DWORD vnum1 = item->GetVnum() - 55700;
					item2->SetSocket(0, vnum1);
					item2->SetSocket(2, item->GetSocket(2));
					item2->SetSocket(1, item->GetSocket(1));
					item2->SetSocket(3, item->GetSocket(3));
					
					//fixing pet system in offlineshop
					item2->SetSocket(4, item2->GetID()); // storing id in socket
					
					std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET id =%lu WHERE id = %lu", item2->GetID(), item->GetID()));
					ITEM_MANAGER::instance().RemoveItem(item);
					return true;
				}
			}
		}
	}

	if (item->GetVnum() == 55002 && item->GetAttributeValue(0) > 0) {
		int pos = GetEmptyInventory(item->GetSize());
		if (pos == -1)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeterli alan yok!"));
			return false;
		}

		if (GetLevel() < item->GetSocket(1))
		{
			ChatPacket(CHAT_TYPE_INFO, "Pet seviyesi senin seviyenden yuksek.");
			ChatPacket(CHAT_TYPE_INFO, "Pet ile ayni seviyeye ulasana dek peti kutudan cikartamazsin!");
			return false;
		}

		if (item->IsExchanging())
			return false;
		DWORD vnum2 = 55700 + item->GetSocket(0);
		LPITEM item2 = AutoGiveItem(vnum2, 1);
		for (int b = 0; b < 3; b++) {
			item2->SetForceAttribute(b, 1, item->GetAttributeValue(b));
		}
		item2->SetForceAttribute(3, 1, item->GetAttributeValue(3));
		item2->SetForceAttribute(4, 1, item->GetAttributeValue(4));
		item2->SetForceAttribute(5, 1, item->GetAttributeValue(5));
		item2->SetForceAttribute(6, 1, item->GetAttributeValue(6));
		item2->SetForceAttribute(7, 1, item->GetAttributeValue(7));
		item2->SetForceAttribute(8, 1, item->GetAttributeValue(8));
		item2->SetForceAttribute(9, 1, item->GetAttributeValue(9));
		item2->SetForceAttribute(10, 1, item->GetAttributeValue(10));
		item2->SetForceAttribute(11, 1, item->GetAttributeValue(11));
		item2->SetForceAttribute(12, 1, item->GetAttributeValue(12));
		item2->SetForceAttribute(13, 1, item->GetAttributeValue(13));
		item2->SetForceAttribute(14, 1, item->GetAttributeValue(14));
		item2->SetSocket(1, item->GetSocket(1));
		item2->SetSocket(2, item->GetSocket(2));
		item2->SetSocket(3, item->GetSocket(3));
		
		//fixing pet system in offlineshop
		DWORD old_id = item->GetSocket(4); // getting old id from socket
		
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET id =%lu WHERE id = %lu", item2->GetID(), old_id));
//		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET id =%lu WHERE id = %lu", item2->GetID(), item->GetID()));
		ITEM_MANAGER::instance().RemoveItem(item);
		return true;
	}
#endif

#ifdef ENABLE_BATTLE_PASS
	if (!v_counts.empty())
	{
		for (int i = 0; i<missions_bp.size(); ++i)
		{
			if (missions_bp[i].type == 1 && item->GetVnum() == missions_bp[i].vnum)
			{
				DoMission(i, 1);
			}
		}
	}
#endif

	switch (item->GetType())
	{
	case ITEM_PET:
	{
		switch (item->GetSubType())
		{
			case PET_EXPFOOD:
			{
				if (quest::CQuestManager::instance().GetEventFlag("DISABLE_GROWTH") == 1)
				{
					ChatPacket(CHAT_TYPE_INFO, "Bu Sistem Bakimdadir !");
					return false;
				}

				CNewPetSystem* petSystem = GetNewPetSystem();
				if (petSystem && petSystem->ItemFeed(item))
					item->SetCount(item->GetCount() - 1);
			}
			break;
		}
	}
	break;

		case ITEM_HAIR:
			return ItemProcess_Hair(item, wDestCell);

		case ITEM_POLYMORPH:
			return ItemProcess_Polymorph(item);

		case ITEM_QUEST:
			if (GetArena() != NULL || IsObserverMode() == true)
			{
				if (item->GetVnum() == 50051 || item->GetVnum() == 50052 || item->GetVnum() == 50053)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련 중에는 이용할 수 없는 물품입니다."));
					return false;
				}
			}

			if (!IS_SET(item->GetFlag(), ITEM_FLAG_QUEST_USE | ITEM_FLAG_QUEST_USE_MULTIPLE))
			{
				if (item->GetSIGVnum() == 0)
				{
					quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);
				}
				else
				{
					quest::CQuestManager::instance().SIGUse(GetPlayerID(), item->GetSIGVnum(), item, false);
				}
			}
			break;

		case ITEM_CAMPFIRE:
			{
				float fx, fy;
				GetDeltaByDegree(GetRotation(), 100.0f, &fx, &fy);

				LPSECTREE tree = SECTREE_MANAGER::instance().Get(GetMapIndex(), (long)(GetX()+fx), (long)(GetY()+fy));

				if (!tree)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("모닥불을 피울 수 없는 지점입니다."));
					return false;
				}

				if (tree->IsAttr((long)(GetX()+fx), (long)(GetY()+fy), ATTR_WATER))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("물 속에 모닥불을 피울 수 없습니다."));
					return false;
				}

				LPCHARACTER campfire = CHARACTER_MANAGER::instance().SpawnMob(fishing::CAMPFIRE_MOB, GetMapIndex(), (long)(GetX()+fx), (long)(GetY()+fy), 0, false, number(0, 359));

				char_event_info* info = AllocEventInfo<char_event_info>();

				info->ch = campfire;

				campfire->m_pkMiningEvent = event_create(kill_campfire_event, info, PASSES_PER_SEC(40));

				item->SetCount(item->GetCount() - 1);
			}
			break;

		case ITEM_UNIQUE:
			{
				switch (item->GetSubType())
				{
					case USE_ABILITY_UP:
						{
							switch (item->GetValue(0))
							{
								case APPLY_MOV_SPEED:
									AddAffect(AFFECT_UNIQUE_ABILITY, POINT_MOV_SPEED, item->GetValue(2), AFF_MOV_SPEED_POTION, item->GetValue(1), 0, true, true);
									break;

								case APPLY_ATT_SPEED:
									AddAffect(AFFECT_UNIQUE_ABILITY, POINT_ATT_SPEED, item->GetValue(2), AFF_ATT_SPEED_POTION, item->GetValue(1), 0, true, true);
									break;

								case APPLY_STR:
									AddAffect(AFFECT_UNIQUE_ABILITY, POINT_ST, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
									break;

								case APPLY_DEX:
									AddAffect(AFFECT_UNIQUE_ABILITY, POINT_DX, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
									break;

								case APPLY_CON:
									AddAffect(AFFECT_UNIQUE_ABILITY, POINT_HT, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
									break;

								case APPLY_INT:
									AddAffect(AFFECT_UNIQUE_ABILITY, POINT_IQ, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
									break;

								case APPLY_CAST_SPEED:
									AddAffect(AFFECT_UNIQUE_ABILITY, POINT_CASTING_SPEED, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
									break;

								case APPLY_RESIST_MAGIC:
									AddAffect(AFFECT_UNIQUE_ABILITY, POINT_RESIST_MAGIC, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
									break;

								case APPLY_ATT_GRADE_BONUS:
									AddAffect(AFFECT_UNIQUE_ABILITY, POINT_ATT_GRADE_BONUS, 
											item->GetValue(2), 0, item->GetValue(1), 0, true, true);
									break;

								case APPLY_DEF_GRADE_BONUS:
									AddAffect(AFFECT_UNIQUE_ABILITY, POINT_DEF_GRADE_BONUS,
											item->GetValue(2), 0, item->GetValue(1), 0, true, true);
									break;
							}
						}

						if (GetDungeon())
							GetDungeon()->UsePotion(this);

						if (GetWarMap())
							GetWarMap()->UsePotion(this, item);

						item->SetCount(item->GetCount() - 1);
						break;

					default:
						{
							if (item->GetSubType() == USE_SPECIAL)
							{
								sys_log(0, "ITEM_UNIQUE: USE_SPECIAL %u", item->GetVnum());

								switch (item->GetVnum())
								{
									case 710499: // 비단보따리
										if (LC_IsYMIR() == true || LC_IsKorea() == true)
										{
											if (IS_BOTARYABLE_ZONE(GetMapIndex()) == true)
											{
												UseSilkBotary();
											}
											else
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("개인 상점을 열 수 없는 지역입니다"));
											}
										}
										else
										{
											UseSilkBotary();
										}
										break;
								}
							}
							else
							{
								if (!item->IsEquipped())
									EquipItem(item);
								else
									UnequipItem(item);
							}
						}
						break;
				}
			}
			break;
			
case USE_PET:
						{
							if (!item->IsEquipped())
							{
								if (GetPetSystem() && GetPetSystem()->CountSummoned() > 0)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pet_cikar_once"));
									return false;
								}
								EquipItem(item);
							}
							else
							{
								UnequipItem(item);
							}
						}
						break;

		case ITEM_COSTUME:
		case ITEM_WEAPON:
		case ITEM_ARMOR:
		case ITEM_ROD:
		case ITEM_RING:		// 신규 반지 아이템
		case ITEM_BELT:		// 신규 벨트 아이템

			// MINING
		case ITEM_PICK:
			// END_OF_MINING
			if (!item->IsEquipped())
				EquipItem(item);
			else
				UnequipItem(item);
			break;
			// 착용하지 않은 용혼석은 사용할 수 없다.
			// 정상적인 클라라면, 용혼석에 관하여 item use 패킷을 보낼 수 없다.
			// 용혼석 착용은 item move 패킷으로 한다.
			// 착용한 용혼석은 추출한다.
		case ITEM_DS:
			{
				if (!item->IsEquipped())
					return false;
				return DSManager::instance().PullOut(this, NPOS, item);
			
			}
		break;
		case ITEM_SPECIAL_DS:
			if (!item->IsEquipped())
				EquipItem(item);
			else
				UnequipItem(item);
			break;
		
		case ITEM_FISH:
			{
				if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련 중에는 이용할 수 없는 물품입니다."));
					return false;
				}

				if (item->GetSubType() == FISH_ALIVE)
					fishing::UseFish(this, item);
			}
			break;

		case ITEM_TREASURE_BOX:
			{
				return false;
				//ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("열쇠로 잠겨 있어서 열리지 않는것 같다. 열쇠를 구해보자."));
			}
			break;

		case ITEM_TREASURE_KEY:
			{
				LPITEM item2;

				if (!GetItem(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->IsExchanging() || item2->IsEquipped()) // @fixme114
					return false;

				if (item2->GetType() != ITEM_TREASURE_BOX)
				{
					ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("열쇠로 여는 물건이 아닌것 같다."));
					return false;
				}

				if (item->GetValue(0) == item2->GetValue(0))
				{
					//ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("열쇠는 맞으나 아이템 주는 부분 구현이 안되었습니다."));
					DWORD dwBoxVnum = item2->GetVnum();
					std::vector <DWORD> dwVnums;
					std::vector <DWORD> dwCounts;
					std::vector <LPITEM> item_gets(0);
					int count = 0;

					if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
					{
						//ITEM_MANAGER::instance().RemoveItem(item);
						//ITEM_MANAGER::instance().RemoveItem(item2);
						
						item->SetCount(item->GetCount()-1);
						item2->SetCount(item2->GetCount()-1);
						for (int i = 0; i < count; i++){
							switch (dwVnums[i])
							{
								case CSpecialItemGroup::GOLD:
#if defined(__CHATTING_WINDOW_RENEWAL__)
									ChatPacket(CHAT_TYPE_MONEY_INFO, LC_TEXT("돈 %d 냥을 획득했습니다."), dwCounts[i]);
#else
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("돈 %d 냥을 획득했습니다."), dwCounts[i]);
#endif
									break;
								case CSpecialItemGroup::EXP:
#if defined(__CHATTING_WINDOW_RENEWAL__)
									ChatPacket(CHAT_TYPE_EXP_INFO, LC_TEXT("상자에서 부터 신비한 빛이 나옵니다."));
									ChatPacket(CHAT_TYPE_EXP_INFO, LC_TEXT("%d의 경험치를 획득했습니다."), dwCounts[i]);
#else
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 부터 신비한 빛이 나옵니다."));
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d의 경험치를 획득했습니다."), dwCounts[i]);
#endif
									break;
								case CSpecialItemGroup::MOB:
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 몬스터가 나타났습니다!"));
									break;
								case CSpecialItemGroup::SLOW:
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 나온 빨간 연기를 들이마시자 움직이는 속도가 느려졌습니다!"));
									break;
								case CSpecialItemGroup::DRAIN_HP:
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자가 갑자기 폭발하였습니다! 생명력이 감소했습니다."));
									break;
								case CSpecialItemGroup::POISON:
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 나온 녹색 연기를 들이마시자 독이 온몸으로 퍼집니다!"));
									break;
								case CSpecialItemGroup::MOB_GROUP:
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 몬스터가 나타났습니다!"));
									break;
								default:
									if (item_gets[i])
									{
#if defined(__CHATTING_WINDOW_RENEWAL__)
										if (dwCounts[i] > 1)
											ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("상자에서 %s 가 %d 개 나왔습니다."), item_gets[i]->GetName(), dwCounts[i]);
										else
											ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("상자에서 %s 가 나왔습니다."), item_gets[i]->GetName());
#else
										if (dwCounts[i] > 1)
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 %s 가 %d 개 나왔습니다."), item_gets[i]->GetName(), dwCounts[i]);
										else
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 %s 가 나왔습니다."), item_gets[i]->GetName());
#endif
									}
							}
						}
					}
					else
					{
						ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("열쇠가 맞지 않는 것 같다."));
						return false;
					}
				}
				else
				{
					ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("열쇠가 맞지 않는 것 같다."));
					return false;
				}
			}
			break;

		case ITEM_GIFTBOX:
			{
				DWORD dwBoxVnum = item->GetVnum();
				std::vector <DWORD> dwVnums;
				std::vector <DWORD> dwCounts;
				std::vector <LPITEM> item_gets(0);
				int count = 0;

				if (dwBoxVnum == 50033 && LC_IsYMIR()) // 알수없는 상자
				{
					if (GetLevel() < 15)
					{
						ChatPacket(CHAT_TYPE_INFO, "15레벨 이하에서는 사용할 수 없습니다.");
						return false;
					}
				}

				if( (dwBoxVnum > 51500 && dwBoxVnum < 52000) || (dwBoxVnum >= 50255 && dwBoxVnum <= 50260) )	// 용혼원석들
				{
					if( !(this->DragonSoul_IsQualified()) )
					{
						ChatPacket(CHAT_TYPE_INFO,LC_TEXT("먼저 용혼석 퀘스트를 완료하셔야 합니다."));
						return false;
					}
				}

				if(GetWarMap())
					return false;
				
				if (bUseAll)
				{
					
					if (GetMapIndex() == 1 || GetMapIndex() == 21 || GetMapIndex() == 41 || GetMapIndex() == 3 || GetMapIndex() == 23 || GetMapIndex() == 43)
					{
						if (g_bChannel == 1)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("toplu_sandik_block"));
							return false;
						}
					}
					
					if (GetMapIndex() == 113 )
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("toplu_sandik_block_ox"));
						return false;
					}					
					
					short itemCount = item->GetCount();
					if (itemCount >= 50)
					{
						itemCount = 50;
					}
					
					while(itemCount--) 
					{
						if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
						{
							bool isHave = item->SetCount(item->GetCount()-1); 
							
							for (int i = 0; i < count; i++)
							{
								switch (dwVnums[i])
								{
								case CSpecialItemGroup::GOLD:
									// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("돈 %d 냥을 획득했습니다."), dwCounts[i]);
									continue;
								case CSpecialItemGroup::EXP:
									// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 부터 신비한 빛이 나옵니다."));
									// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d의 경험치를 획득했습니다."), dwCounts[i]);
									continue;
								case CSpecialItemGroup::MOB:
									// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 몬스터가 나타났습니다!"));
									continue;
								case CSpecialItemGroup::SLOW:
									// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 나온 빨간 연기를 들이마시자 움직이는 속도가 느려졌습니다!"));
									continue;
								case CSpecialItemGroup::DRAIN_HP:
									// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자가 갑자기 폭발하였습니다! 생명력이 감소했습니다."));
									continue;
								case CSpecialItemGroup::POISON:
									// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 나온 녹색 연기를 들이마시자 독이 온몸으로 퍼집니다!"));
									continue;
								case CSpecialItemGroup::MOB_GROUP:
									// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 몬스터가 나타났습니다!"));
									continue;
								// default:
									// if (item_gets[i])
									// {
										// if (dwCounts[i] > 1)
											// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 %s 가 %d 개 나왔습니다."), item_gets[i]->GetName(), dwCounts[i]);
										// else
											// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 %s 가 나왔습니다."), item_gets[i]->GetName());
									// }
								}
							}
							
							
							if (!isHave)
								break;
							
						}
						else
						{
							ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("아무것도 얻을 수 없었습니다."));
							break;
						}
					}
				}
				else
				{
					if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
					{
						item->SetCount(item->GetCount()-1);

						for (int i = 0; i < count; i++){
							switch (dwVnums[i])
							{
							case CSpecialItemGroup::GOLD:
#if defined(__CHATTING_WINDOW_RENEWAL__)
								ChatPacket(CHAT_TYPE_MONEY_INFO, LC_TEXT("돈 %d 냥을 획득했습니다."), dwCounts[i]);
#else
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("돈 %d 냥을 획득했습니다."), dwCounts[i]);
#endif
								break;
							case CSpecialItemGroup::EXP:
#if defined(__CHATTING_WINDOW_RENEWAL__)
								ChatPacket(CHAT_TYPE_EXP_INFO, LC_TEXT("상자에서 부터 신비한 빛이 나옵니다."));
								ChatPacket(CHAT_TYPE_EXP_INFO, LC_TEXT("%d의 경험치를 획득했습니다."), dwCounts[i]);
#else
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 부터 신비한 빛이 나옵니다."));
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d의 경험치를 획득했습니다."), dwCounts[i]);
#endif
								break;
							case CSpecialItemGroup::MOB:
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 몬스터가 나타났습니다!"));
								break;
							case CSpecialItemGroup::SLOW:
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 나온 빨간 연기를 들이마시자 움직이는 속도가 느려졌습니다!"));
								break;
							case CSpecialItemGroup::DRAIN_HP:
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자가 갑자기 폭발하였습니다! 생명력이 감소했습니다."));
								break;
							case CSpecialItemGroup::POISON:
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 나온 녹색 연기를 들이마시자 독이 온몸으로 퍼집니다!"));
								break;
							case CSpecialItemGroup::MOB_GROUP:
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 몬스터가 나타났습니다!"));
								break;
							default:
								if (item_gets[i])
								{
#if defined(__CHATTING_WINDOW_RENEWAL__)
									if (dwCounts[i] > 1)
										ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("상자에서 %s 가 %d 개 나왔습니다."), item_gets[i]->GetName(), dwCounts[i]);
									else
										ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("상자에서 %s 가 나왔습니다."), item_gets[i]->GetName());
#else
									if (dwCounts[i] > 1)
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 %s 가 %d 개 나왔습니다."), item_gets[i]->GetName(), dwCounts[i]);
									else
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 %s 가 나왔습니다."), item_gets[i]->GetName());
#endif
								}
							}
						}
					}
					else
					{
						ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("아무것도 얻을 수 없었습니다."));
						return false;
					}
				}
			}
			break;

		case ITEM_SKILLFORGET:
			{
				if (!item->GetSocket(0))
				{
						item->SetCount(item->GetCount() - 1);

					return false;
				}

				DWORD dwVnum = item->GetSocket(0);

				if (SkillLevelDown(dwVnum))
				{
						item->SetCount(item->GetCount() - 1);

					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("스킬 레벨을 내리는데 성공하였습니다."));
				}
				else
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("스킬 레벨을 내릴 수 없습니다."));
			}
			break;

		case ITEM_SKILLBOOK:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("변신중에는 책을 읽을수 없습니다."));
					return false;
				}

#ifdef LWT_BUFFI_SYSTEM
				if (item->GetVnum() == BUFF_SOULSTONE) // LWT
				{
					DWORD okunacakSkill = item->GetSocket(0);
					if (GetSkillLevel(okunacakSkill) >= 40 || GetSkillLevel(okunacakSkill) < 30){
						ChatPacket(CHAT_TYPE_INFO, "Bu beceri gelisemez.");
						return false;}
					LearnGrandMasterSkill(okunacakSkill);
					item->SetCount(item->GetCount() -1);
					return false;
				}
#endif

				DWORD dwVnum = 0;

				if (item->GetVnum() == 50300)
				{
					dwVnum = item->GetSocket(0);
				}
#ifdef LWT_BUFFI_SYSTEM
				else if(item->GetVnum() == BUFF_BK)
				{
					dwVnum = item->GetSocket(0);
				}
#endif
				else
				{
					// 새로운 수련서는 value 0 에 스킬 번호가 있으므로 그것을 사용.
					dwVnum = item->GetValue(0);
				}

				if (0 == dwVnum)
				{
					item->SetCount(item->GetCount() - 1);

					return false;
				}

				if (true == LearnSkillByBook(dwVnum))
				{
					item->SetCount(item->GetCount() - 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);

					if (distribution_test_server)
						iReadDelay /= 3;

					//한국 본섭의 경우에는 시간을 24시간 고정
					if (LC_IsKorea())
						iReadDelay = 86400;

					SetSkillNextReadTime(dwVnum, get_global_time() + iReadDelay);
				}
			}
			break;

		case ITEM_USE:
			{
				if (item->GetVnum() >= 27863 && item->GetVnum() <= 27883 || item->GetVnum() > 50800 && item->GetVnum() <= 50820)
				{
					if (test_server)
						sys_log (0, "ADD addtional effect : vnum(%d) subtype(%d)", item->GetOriginalVnum(), item->GetSubType());

					int affect_type = AFFECT_EXP_BONUS_EURO_FREE;
					int apply_type = aApplyInfo[item->GetValue(0)].bPointType;
					int apply_value = item->GetValue(2);
					int apply_duration = item->GetValue(1);

					switch (item->GetSubType())
					{
						case USE_ABILITY_UP:
#ifdef ENABLE_YMIR_AFFECT_FIX
                           if ((CheckTimeUsed(item) == false))    {    return false;    }
#endif
							if (FindAffect(affect_type, apply_type))
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 효과가 걸려 있습니다."));
								return false;
							}

							{
								switch (item->GetValue(0))
								{
									case APPLY_MOV_SPEED:
										AddAffect(affect_type, apply_type, apply_value, AFF_MOV_SPEED_POTION, apply_duration, 0, true, true);
										break;

									case APPLY_ATT_SPEED:
										AddAffect(affect_type, apply_type, apply_value, AFF_ATT_SPEED_POTION, apply_duration, 0, true, true);
										break;

									case APPLY_STR:
									case APPLY_DEX:
									case APPLY_CON:
									case APPLY_INT:
									case APPLY_CAST_SPEED:
									case APPLY_RESIST_MAGIC:
									case APPLY_ATT_GRADE_BONUS:
									case APPLY_DEF_GRADE_BONUS:
									case APPLY_MAX_HP:
									case APPLY_ATTBONUS_HUMAN:
									case APPLY_ATT_BOSS:
									case APPLY_ATTBONUS_METIN:
									case APPLY_ATTBONUS_MILGYO:
									case APPLY_ATTBONUS_ORC:
									case APPLY_ATTBONUS_DEVIL:
									case APPLY_ATTBONUS_UNDEAD:
									case APPLY_CRITICAL_PCT:
									case APPLY_PENETRATE_PCT:
									case APPLY_STEAL_HP:
									case APPLY_BLOCK:
									case APPLY_POISON_PCT:
									case APPLY_POISON_REDUCE:
									case APPLY_EXP_DOUBLE_BONUS:
									case APPLY_ITEM_DROP_BONUS:
										AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, true, true);
										break;
								}
							}

							if (GetDungeon())
								GetDungeon()->UsePotion(this);

							if (GetWarMap())
								GetWarMap()->UsePotion(this, item);

							item->SetCount(item->GetCount() - 1);
							break;

					case USE_AFFECT :
						{
#ifdef ENABLE_YMIR_AFFECT_FIX
                           if ((CheckTimeUsed(item) == false))    {    return false;    }
#endif
							if (FindAffect(AFFECT_EXP_BONUS_EURO_FREE, aApplyInfo[item->GetValue(1)].bPointType))
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 효과가 걸려 있습니다."));
							}
							else
							{
								// PC_BANG_ITEM_ADD
								if (item->IsPCBangItem() == true)
								{
									// PC방인지 체크해서 처리
									if (CPCBangManager::instance().IsPCBangIP(GetDesc()->GetHostName()) == false)
									{
										// PC방이 아님!
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 아이템은 PC방에서만 사용할 수 있습니다."));
										return false;
									}
								}
								// END_PC_BANG_ITEM_ADD

								AddAffect(AFFECT_EXP_BONUS_EURO_FREE, aApplyInfo[item->GetValue(1)].bPointType, item->GetValue(2), 0, item->GetValue(3), 0, false, true);
								item->SetCount(item->GetCount() - 1);
							}
						}
						break;

					case USE_POTION_NODELAY:
						{
							if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
							{
								if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit") > 0)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련장에서 사용하실 수 없습니다."));
									return false;
								}

								switch (item->GetVnum())
								{
									case 70020 :
									case 71018 :
									case 71019 :
									case 71020 :
										if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit_count") < 10000)
										{
											if (m_nPotionLimit <= 0)
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("사용 제한량을 초과하였습니다."));
												return false;
											}
										}
										break;

									default :
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련장에서 사용하실 수 없습니다."));
										return false;
										break;
								}
							}

							bool used = false;

							if (item->GetValue(0) != 0) // HP 절대값 회복
							{
								if (GetHP() < GetMaxHP())
								{
									PointChange(POINT_HP, item->GetValue(0) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
									EffectPacket(SE_HPUP_RED);
									used = TRUE;
								}
							}

							if (item->GetValue(1) != 0)	// SP 절대값 회복
							{
								if (GetSP() < GetMaxSP())
								{
									PointChange(POINT_SP, item->GetValue(1) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
									EffectPacket(SE_SPUP_BLUE);
									used = TRUE;
								}
							}

							if (item->GetValue(3) != 0) // HP % 회복
							{
								if (GetHP() < GetMaxHP())
								{
									PointChange(POINT_HP, item->GetValue(3) * GetMaxHP() / 100);
									EffectPacket(SE_HPUP_RED);
									used = TRUE;
								}
							}

							if (item->GetValue(4) != 0) // SP % 회복
							{
								if (GetSP() < GetMaxSP())
								{
									PointChange(POINT_SP, item->GetValue(4) * GetMaxSP() / 100);
									EffectPacket(SE_SPUP_BLUE);
									used = TRUE;
								}
							}

							if (used)
							{
								if (item->GetVnum() == 50085 || item->GetVnum() == 50086)
								{
									if (test_server)
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("월병 또는 종자 를 사용하였습니다"));
									SetUseSeedOrMoonBottleTime();
								}
								if (GetDungeon())
									GetDungeon()->UsePotion(this);

								if (GetWarMap())
									GetWarMap()->UsePotion(this, item);

								m_nPotionLimit--;

								//RESTRICT_USE_SEED_OR_MOONBOTTLE
								item->SetCount(item->GetCount() - 1);
								//END_RESTRICT_USE_SEED_OR_MOONBOTTLE
							}
						}
						break;
					}

					return true;
				}


				if (item->GetVnum() >= 27863 && item->GetVnum() <= 27883)
				{
					if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련 중에는 이용할 수 없는 물품입니다."));
						return false;
					}
				}

				if (test_server)
				{
					 sys_log (0, "USE_ITEM %s Type %d SubType %d vnum %d", item->GetName(), item->GetType(), item->GetSubType(), item->GetOriginalVnum());
				}

				switch (item->GetSubType())
				{
					case USE_TIME_CHARGE_PER:
						{
							LPITEM pDestItem = GetItem(DestCell);
							if (NULL == pDestItem)
							{
								return false;
							}
							// 우선 용혼석에 관해서만 하도록 한다.
							if (pDestItem->IsDragonSoul())
							{
								int ret;
								char buf[128];
								if (item->GetVnum() == DRAGON_HEART_VNUM)
								{
									ret = pDestItem->GiveMoreTime_Per((float)item->GetSocket(ITEM_SOCKET_CHARGING_AMOUNT_IDX));
								}
								else
								{
									ret = pDestItem->GiveMoreTime_Per((float)item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
								}
								if (ret > 0)
								{
									if (item->GetVnum() == DRAGON_HEART_VNUM)
									{
										sprintf(buf, "Inc %ds by item{VN:%d SOC%d:%d}", ret, item->GetVnum(), ITEM_SOCKET_CHARGING_AMOUNT_IDX, item->GetSocket(ITEM_SOCKET_CHARGING_AMOUNT_IDX));
									}
									else
									{
										sprintf(buf, "Inc %ds by item{VN:%d VAL%d:%d}", ret, item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
									}

									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d초 만큼 충전되었습니다."), ret);
									item->SetCount(item->GetCount() - 1);
									LogManager::instance().ItemLog(this, item, "DS_CHARGING_SUCCESS", buf);
									return true;
								}
								else
								{
									if (item->GetVnum() == DRAGON_HEART_VNUM)
									{
										sprintf(buf, "No change by item{VN:%d SOC%d:%d}", item->GetVnum(), ITEM_SOCKET_CHARGING_AMOUNT_IDX, item->GetSocket(ITEM_SOCKET_CHARGING_AMOUNT_IDX));
									}
									else
									{
										sprintf(buf, "No change by item{VN:%d VAL%d:%d}", item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
									}

									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("충전할 수 없습니다."));
									LogManager::instance().ItemLog(this, item, "DS_CHARGING_FAILED", buf);
									return false;
								}
							}
							else
								return false;
						}
						break;
					case USE_TIME_CHARGE_FIX:
						{
							LPITEM pDestItem = GetItem(DestCell);
							if (NULL == pDestItem)
							{
								return false;
							}
							// 우선 용혼석에 관해서만 하도록 한다.
							if (pDestItem->IsDragonSoul())
							{
								int ret = pDestItem->GiveMoreTime_Fix(item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
								char buf[128];
								if (ret)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d초 만큼 충전되었습니다."), ret);
									sprintf(buf, "Increase %ds by item{VN:%d VAL%d:%d}", ret, item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
									LogManager::instance().ItemLog(this, item, "DS_CHARGING_SUCCESS", buf);
									item->SetCount(item->GetCount() - 1);
									return true;
								}
								else
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("충전할 수 없습니다."));
									sprintf(buf, "No change by item{VN:%d VAL%d:%d}", item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
									LogManager::instance().ItemLog(this, item, "DS_CHARGING_FAILED", buf);
									return false;
								}
							}
							else
								return false;
						}
						break;
					case USE_COSTUME_MOUNT_SKIN:
						{
							LPITEM item2;
							if (!IsValidItemPosition(DestCell) || !(item2 = GetInventoryItem(wDestCell)))
								return false;

							if (!(item2->GetType() == ITEM_COSTUME && item2->GetSubType() == COSTUME_MOUNT))
								return false;

							if (item2->IsExchanging() || item->IsExchanging())
								return false;

							if (item2->isLocked() || item->isLocked() || item2->IsEquipped())
								return false;
							
							if (50053 == item2->GetVnum())
								return false;

							if (NULL != GetWear(WEAR_COSTUME_MOUNT))
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("binek_cikar_once"));
								return false;
							}
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Yeni gorunum basariyla islendi."));
							item2->SetSocket(3, item->GetValue(0));
							item->SetCount(item->GetCount() - 1);
						}
						break;
					case USE_SPECIAL:
						
						switch (item->GetVnum())
						{
							
							case 72749:
								{
									if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("evrimengel"));
										return false;
									}

									LPITEM item2;

									if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
										return false;

									if (item2->IsExchanging())
										return false;

									if (item2->IsEquipped())
										return false;

									if (item2->GetEvolution() == 4)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("evrimsonseviye"));
										return false;
									}

									if (item2->GetType() == ITEM_WEAPON)
									{
										RefineEvrimItem(item2);
									}
								}
								break;
							//크리스마스 란주
							case ITEM_NOG_POCKET:
								{
									/*
									란주능력치 : item_proto value 의미
										이동속도  value 1
										공격력	  value 2
										경험치    value 3
										지속시간  value 0 (단위 초)

									*/
									if (FindAffect(AFFECT_NOG_ABILITY))
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 효과가 걸려 있습니다."));
										return false;
									}
									long time = item->GetValue(0);
									long moveSpeedPer	= item->GetValue(1);
									long attPer	= item->GetValue(2);
									long expPer			= item->GetValue(3);
									AddAffect(AFFECT_NOG_ABILITY, POINT_MOV_SPEED, moveSpeedPer, AFF_MOV_SPEED_POTION, time, 0, true, true);
									AddAffect(AFFECT_NOG_ABILITY, POINT_MALL_ATTBONUS, attPer, AFF_NONE, time, 0, true, true);
									AddAffect(AFFECT_NOG_ABILITY, POINT_MALL_EXPBONUS, expPer, AFF_NONE, time, 0, true, true);
									item->SetCount(item->GetCount() - 1);
								}
								break;
								
							//라마단용 사탕
							case 51350:
								{
									int affect_type = AFFECT_BUFF_EXTRACT;
									int apply_type = aApplyInfo[item->GetValue(0)].bPointType;
									int apply_value = item->GetValue(2);
									int apply_duration = item->GetValue(1);
									
									if (FindAffect(affect_type, apply_type))
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 효과가 걸려 있습니다."));
										return false;
									}
									
									AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, true, true);
									item->SetCount(item->GetCount() - 1);
								}
								break;
							case 51351:
								{
									int affect_type = AFFECT_BUFF_EXTRACT_2;
									int apply_type = aApplyInfo[item->GetValue(0)].bPointType;
									int apply_value = item->GetValue(2);
									int apply_duration = item->GetValue(1);
									
									if (FindAffect(affect_type, apply_type))
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 효과가 걸려 있습니다."));
										return false;
									}
									
									AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, true, true);
									item->SetCount(item->GetCount() - 1);
								}
								break;
							case 51352:
								{
									int affect_type = AFFECT_BUFF_EXTRACT_3;
									int apply_type = aApplyInfo[item->GetValue(0)].bPointType;
									int apply_value = item->GetValue(2);
									int apply_duration = item->GetValue(1);
									
									if (FindAffect(affect_type, apply_type))
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 효과가 걸려 있습니다."));
										return false;
									}
									
									AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, true, true);
									item->SetCount(item->GetCount() - 1);
								}
								break;
							case ITEM_RAMADAN_CANDY:
								{
									/*
									사탕능력치 : item_proto value 의미
										이동속도  value 1
										공격력	  value 2
										경험치    value 3
										지속시간  value 0 (단위 초)

									*/
									long time = item->GetValue(0);
									long moveSpeedPer	= item->GetValue(1);
									long attPer	= item->GetValue(2);
									long expPer			= item->GetValue(3);
									AddAffect(AFFECT_RAMADAN_ABILITY, POINT_MOV_SPEED, moveSpeedPer, AFF_MOV_SPEED_POTION, time, 0, true, true);
									AddAffect(AFFECT_RAMADAN_ABILITY, POINT_MALL_ATTBONUS, attPer, AFF_NONE, time, 0, true, true);
									AddAffect(AFFECT_RAMADAN_ABILITY, POINT_MALL_EXPBONUS, expPer, AFF_NONE, time, 0, true, true);
									item->SetCount(item->GetCount() - 1);
								}
								break;
							case ITEM_MARRIAGE_RING:
								{
									if (IsDead() || GetExchange() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || GetOfflineShopOwner())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
										break;
									}
									
									if (IsSashCombinationOpen())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("kombinasyon_penceresi_acik"));
										break;
									}
									
									if (IsSashAbsorptionOpen())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bonus_emis_penceresi_acik"));
										break;
									}
									
									marriage::TMarriage* pMarriage = marriage::CManager::instance().Get(GetPlayerID());
									if (pMarriage)
									{
										if (pMarriage->ch1 != NULL)
										{
											if (CArenaManager::instance().IsArenaMap(pMarriage->ch1->GetMapIndex()) == true)
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련 중에는 이용할 수 없는 물품입니다."));
												break;
											}
										}

										if (pMarriage->ch2 != NULL)
										{
											if (CArenaManager::instance().IsArenaMap(pMarriage->ch2->GetMapIndex()) == true)
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련 중에는 이용할 수 없는 물품입니다."));
												break;
											}
											if (pMarriage->ch2->GetMapIndex() == 357 && pMarriage->ch2->GetMapIndex() == 358 && pMarriage->ch2->GetMapIndex() == 355 && pMarriage->ch2->GetMapIndex() == 352 && pMarriage->ch2->GetMapIndex() == 351 && pMarriage->ch2->GetMapIndex() == 304 && pMarriage->ch2->GetMapIndex() == 303 && pMarriage->ch2->GetMapIndex() == 302 && pMarriage->ch2->GetMapIndex() == 301 && pMarriage->ch2->GetMapIndex() == 217 && pMarriage->ch2->GetMapIndex() == 216 && pMarriage->ch2->GetMapIndex() == 213 && pMarriage->ch2->GetMapIndex() == 212 && pMarriage->ch2->GetMapIndex() == 210 && pMarriage->ch2->GetMapIndex() == 209 && pMarriage->ch2->GetMapIndex() == 72 && pMarriage->ch2->GetMapIndex() == 73 && pMarriage->ch2->GetMapIndex() == 71)
												return false;
										}

										int consumeSP = CalculateConsumeSP(this);

										if (consumeSP < 0)
											return false;

										PointChange(POINT_SP, -consumeSP, false);

										WarpToPID(pMarriage->GetOther(GetPlayerID()));
									}
									else
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("결혼 상태가 아니면 결혼반지를 사용할 수 없습니다."));
								}
								break;

							//Shop Decoration
							case 71049:
								{
									if (FindAffect(AFFECT_SHOP_DECO))
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pazar_deco_zaten_aktif"));
										return false;
									}
									int nShopDuration = 60*60*24*7;
									AddAffect(AFFECT_SHOP_DECO, 0, 0, 0, nShopDuration, 0, true);
									item->SetCount(item->GetCount()-1);
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pazar_deco_aktif"));
								}
								break;
							//Shop Decoration
							
							
							case 72701:
								{
									if (FindAffect(AFFECT_RUZGAR))
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruzgar_zaten_aktif"));
										return false;
									}
									int nRuzgarDuration = 60*60*24*365;
									AddAffect(AFFECT_RUZGAR, POINT_MOV_SPEED, 30, 0, nRuzgarDuration, 0, true);
									item->SetCount(item->GetCount()-1);
									//ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruzgar_aktif"));
								}
								break;
							//Shop Decoration
							
							case 40004:
							{
								if ( FindAffect(AFFECT_EXP_CURSE) )
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Anti_exp_deactive"));
									RemoveAffect(AFFECT_EXP_CURSE);
									item->Lock(false);
									item->SetSocket(0, false);
								}
									
								else
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Anti_exp_active"));
									AddAffect(AFFECT_EXP_CURSE, POINT_NONE, 0, AFF_NONE, INFINITE_AFFECT_DURATION, 0, true);
									item->Lock(true);
									item->SetSocket(0, true);
								}
									
							}
							break;	

								//기존 용기의 망토
							case UNIQUE_ITEM_CAPE_OF_COURAGE:
								//라마단 보상용 용기의 망토
							case 70057:
							case REWARD_BOX_UNIQUE_ITEM_CAPE_OF_COURAGE:
								AggregateMonster();
								//item->SetCount(item->GetCount()-1);
								break;

							case UNIQUE_ITEM_WHITE_FLAG:
								ForgetMyAttacker();
								item->SetCount(item->GetCount()-1);
								break;

							case UNIQUE_ITEM_TREASURE_BOX:
								break;

							case 30093:
							case 30094:
							case 30095:
							case 30096:
								// 복주머니
								{
									const int MAX_BAG_INFO = 26;
									static struct LuckyBagInfo
									{
										DWORD count;
										int prob;
										DWORD vnum;
									} b1[MAX_BAG_INFO] =
									{
										{ 1000,	302,	1 },
										{ 10,	150,	27002 },
										{ 10,	75,	27003 },
										{ 10,	100,	27005 },
										{ 10,	50,	27006 },
										{ 10,	80,	27001 },
										{ 10,	50,	27002 },
										{ 10,	80,	27004 },
										{ 10,	50,	27005 },
										{ 1,	10,	50300 },
										{ 1,	6,	92 },
										{ 1,	2,	132 },
										{ 1,	6,	1052 },
										{ 1,	2,	1092 },
										{ 1,	6,	2082 },
										{ 1,	2,	2122 },
										{ 1,	6,	3082 },
										{ 1,	2,	3122 },
										{ 1,	6,	5052 },
										{ 1,	2,	5082 },
										{ 1,	6,	7082 },
										{ 1,	2,	7122 },
										{ 1,	1,	11282 },
										{ 1,	1,	11482 },
										{ 1,	1,	11682 },
										{ 1,	1,	11882 },
									};

									struct LuckyBagInfo b2[MAX_BAG_INFO] =
									{
										{ 1000,	302,	1 },
										{ 10,	150,	27002 },
										{ 10,	75,	27002 },
										{ 10,	100,	27005 },
										{ 10,	50,	27005 },
										{ 10,	80,	27001 },
										{ 10,	50,	27002 },
										{ 10,	80,	27004 },
										{ 10,	50,	27005 },
										{ 1,	10,	50300 },
										{ 1,	6,	92 },
										{ 1,	2,	132 },
										{ 1,	6,	1052 },
										{ 1,	2,	1092 },
										{ 1,	6,	2082 },
										{ 1,	2,	2122 },
										{ 1,	6,	3082 },
										{ 1,	2,	3122 },
										{ 1,	6,	5052 },
										{ 1,	2,	5082 },
										{ 1,	6,	7082 },
										{ 1,	2,	7122 },
										{ 1,	1,	11282 },
										{ 1,	1,	11482 },
										{ 1,	1,	11682 },
										{ 1,	1,	11882 },
									};
	
									LuckyBagInfo * bi = NULL;
									if (LC_IsHongKong())
										bi = b2;
									else
										bi = b1;

									int pct = number(1, 1000);

									int i;
									for (i=0;i<MAX_BAG_INFO;i++)
									{
										if (pct <= bi[i].prob)
											break;
										pct -= bi[i].prob;
									}
									if (i>=MAX_BAG_INFO)
										return false;

									if (bi[i].vnum == 50300)
									{
										// 스킬수련서는 특수하게 준다.
										GiveRandomSkillBook();
									}
									else if (bi[i].vnum == 1)
									{
#ifdef YANG_LIMIT
										GoldChange(1000, "GiveRandomSkillBook");
#else
										PointChange(POINT_GOLD, 1000);
#endif
									}
									else
									{
										AutoGiveItem(bi[i].vnum, bi[i].count);
									}
									item->SetCount(item->GetCount() - 1);
								}
								break;

							case 50004: // 이벤트용 감지기
								{
									if (item->GetSocket(0))
									{
										item->SetSocket(0, item->GetSocket(0) + 1);
									}
									else
									{
										// 처음 사용시
										int iMapIndex = GetMapIndex();

										PIXEL_POSITION pos;

										if (SECTREE_MANAGER::instance().GetRandomLocation(iMapIndex, pos, 700))
										{
											item->SetSocket(0, 1);
											item->SetSocket(1, pos.x);
											item->SetSocket(2, pos.y);
										}
										else
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 곳에선 이벤트용 감지기가 동작하지 않는것 같습니다."));
											return false;
										}
									}

									int dist = 0;
									float distance = (DISTANCE_SQRT(GetX()-item->GetSocket(1), GetY()-item->GetSocket(2)));

									if (distance < 1000.0f)
									{
										// 발견!
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이벤트용 감지기가 신비로운 빛을 내며 사라집니다."));

										// 사용횟수에 따라 주는 아이템을 다르게 한다.
										struct TEventStoneInfo
										{
											DWORD dwVnum;
											int count;
											int prob;
										};
										const int EVENT_STONE_MAX_INFO = 15;
										TEventStoneInfo info_10[EVENT_STONE_MAX_INFO] =
										{ 
											{ 27001, 10,  8 },
											{ 27004, 10,  6 },
											{ 27002, 10, 12 },
											{ 27005, 10, 12 },
											{ 27100,  1,  9 },
											{ 27103,  1,  9 },
											{ 27101,  1, 10 },
											{ 27104,  1, 10 },
											{ 27999,  1, 12 },

											{ 25040,  1,  4 },

											{ 27410,  1,  0 },
											{ 27600,  1,  0 },
											{ 25100,  1,  0 },

											{ 50001,  1,  0 },
											{ 50003,  1,  1 },
										};
										TEventStoneInfo info_7[EVENT_STONE_MAX_INFO] =
										{ 
											{ 27001, 10,  1 },
											{ 27004, 10,  1 },
											{ 27004, 10,  9 },
											{ 27005, 10,  9 },
											{ 27100,  1,  5 },
											{ 27103,  1,  5 },
											{ 27101,  1, 10 },
											{ 27104,  1, 10 },
											{ 27999,  1, 14 },

											{ 25040,  1,  5 },

											{ 27410,  1,  5 },
											{ 27600,  1,  5 },
											{ 25100,  1,  5 },

											{ 50001,  1,  0 },
											{ 50003,  1,  5 },

										};
										TEventStoneInfo info_4[EVENT_STONE_MAX_INFO] =
										{ 
											{ 27001, 10,  0 },
											{ 27004, 10,  0 },
											{ 27002, 10,  0 },
											{ 27005, 10,  0 },
											{ 27100,  1,  0 },
											{ 27103,  1,  0 },
											{ 27101,  1,  0 },
											{ 27104,  1,  0 },
											{ 27999,  1, 25 },

											{ 25040,  1,  0 },

											{ 27410,  1,  0 },
											{ 27600,  1,  0 },
											{ 25100,  1, 15 },

											{ 50001,  1, 10 },
											{ 50003,  1, 50 },

										};

										{
											TEventStoneInfo* info;
											if (item->GetSocket(0) <= 4)
												info = info_4;
											else if (item->GetSocket(0) <= 7)
												info = info_7;
											else
												info = info_10;

											int prob = number(1, 100);

											for (int i = 0; i < EVENT_STONE_MAX_INFO; ++i)
											{
												if (!info[i].prob)
													continue;

												if (prob <= info[i].prob)
												{
													if (info[i].dwVnum == 50001)
													{
														DWORD * pdw = M2_NEW DWORD[2];

														pdw[0] = info[i].dwVnum;
														pdw[1] = info[i].count;

														// 추첨서는 소켓을 설정한다
														DBManager::instance().ReturnQuery(QID_LOTTO, GetPlayerID(), pdw,
																"INSERT INTO lotto_list VALUES(0, 'server%s', %u, NOW())", 
																get_table_postfix(), GetPlayerID());
													}
													else
														AutoGiveItem(info[i].dwVnum, info[i].count);

													break;
												}
												prob -= info[i].prob;
											}
										}

										char chatbuf[CHAT_MAX_LEN + 1];
										int len = snprintf(chatbuf, sizeof(chatbuf), "StoneDetect %u 0 0", (DWORD)GetVID());

										if (len < 0 || len >= (int) sizeof(chatbuf))
											len = sizeof(chatbuf) - 1;

										++len;  // \0 문자까지 보내기

										TPacketGCChat pack_chat;
										pack_chat.header	= HEADER_GC_CHAT;
										pack_chat.size		= sizeof(TPacketGCChat) + len;
										pack_chat.type		= CHAT_TYPE_COMMAND;
										pack_chat.id		= 0;
										pack_chat.bEmpire	= GetDesc()->GetEmpire();
										//pack_chat.id	= vid;

										TEMP_BUFFER buf;
										buf.write(&pack_chat, sizeof(TPacketGCChat));
										buf.write(chatbuf, len);

										PacketAround(buf.read_peek(), buf.size());

										item->SetCount(item->GetCount() - 1);
										return true;
									}
									else if (distance < 20000)
										dist = 1;
									else if (distance < 70000)
										dist = 2;
									else
										dist = 3;

									// 많이 사용했으면 사라진다.
									const int STONE_DETECT_MAX_TRY = 10;
									if (item->GetSocket(0) >= STONE_DETECT_MAX_TRY)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이벤트용 감지기가 흔적도 없이 사라집니다."));
										item->SetCount(item->GetCount() - 1);
										AutoGiveItem(27002);
										return true;
									}

									if (dist)
									{
										char chatbuf[CHAT_MAX_LEN + 1];
										int len = snprintf(chatbuf, sizeof(chatbuf),
												"StoneDetect %u %d %d",
											   	(DWORD)GetVID(), dist, (int)GetDegreeFromPositionXY(GetX(), item->GetSocket(2), item->GetSocket(1), GetY()));

										if (len < 0 || len >= (int) sizeof(chatbuf))
											len = sizeof(chatbuf) - 1;

										++len;  // \0 문자까지 보내기

										TPacketGCChat pack_chat;
										pack_chat.header	= HEADER_GC_CHAT;
										pack_chat.size		= sizeof(TPacketGCChat) + len;
										pack_chat.type		= CHAT_TYPE_COMMAND;
										pack_chat.id		= 0;
										pack_chat.bEmpire	= GetDesc()->GetEmpire();
										//pack_chat.id		= vid;

										TEMP_BUFFER buf;
										buf.write(&pack_chat, sizeof(TPacketGCChat));
										buf.write(chatbuf, len);

										PacketAround(buf.read_peek(), buf.size());
									}

								}
								break;
			case 26000:
			case 26001:
			case 26002:
			case 26003:
			case 27989:
			case 76006:
			{
				LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(GetMapIndex());

				if (pMap != NULL)
				{
					item->SetSocket(0, item->GetSocket(0) + 1);
					FFindStone f;

					pMap->for_each(f);

					if (f.m_mapStone.size() > 0)
					{
						std::map<DWORD, LPCHARACTER>::iterator stone = f.m_mapStone.begin();

						DWORD max = UINT_MAX;
						LPCHARACTER pTarget = stone->second;

						while (stone != f.m_mapStone.end())
						{
							DWORD dist = (DWORD)DISTANCE_SQRT(GetX() - stone->second->GetX(), GetY() - stone->second->GetY());

							if (dist != 0 && max > dist)
							{
								max = dist;
								pTarget = stone->second;
							}
							stone++;
						}

						if (pTarget != NULL)
						{
#ifdef ENABLE_STONE_DEDECT_RENEWAL
							Show(pTarget->GetMapIndex(), pTarget->GetX(), pTarget->GetY(), pTarget->GetZ());
#else
							int val = 3;

							if (max < 10000) val = 2;
							else if (max < 70000) val = 1;

							ChatPacket(CHAT_TYPE_COMMAND, "StoneDetect %u %d %d", (DWORD)GetVID(), val,
								(int)GetDegreeFromPositionXY(GetX(), pTarget->GetY(), pTarget->GetX(), GetY()));
#endif
						}
						else
						{
#ifdef ENABLE_STONE_DEDECT_RENEWAL
							item->SetSocket(0, item->GetSocket(0) - 1);
#endif
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("감지기를 작용하였으나 감지되는 영석이 없습니다."));
						}
					}
					else
					{
#ifdef ENABLE_STONE_DEDECT_RENEWAL
						item->SetSocket(0, item->GetSocket(0) - 1);
#endif
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("감지기를 작용하였으나 감지되는 영석이 없습니다."));
					}

					// if (item->GetSocket(0) >= 99999)
					// {
						// ChatPacket(CHAT_TYPE_COMMAND, "StoneDetect %u 0 0", (DWORD)GetVID());
						// ITEM_MANAGER::instance().RemoveItem(item);
					// }
				}
				break;
			}
			break;
							case 71035:
								if(int(GetQuestFlag("bio.sans")) == 1)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosanszatenaktif"));
								}
								else if(GetQuestFlag("bio.durum") > 20)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevinyok"));
									ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
								}
								else if(GetQuestFlag("bio.ruhtasi") == 1)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhdayapamazsin"));
								}
								else
								{
									item->SetCount(item->GetCount() - 1);
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosansverildi"));
									SetQuestFlag("bio.sans",1);
								}

								break;
							case 96053:
								if(int(GetQuestFlag("bio.sure")) == 1)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosurezatenaktif"));
								}
								else if(GetQuestFlag("bio.durum") > 20)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevinyok"));
									ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
								}
								else if(GetQuestFlag("bio.ruhtasi") == 1)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhdayapamazsin"));
								}
								else
								{
									item->SetCount(item->GetCount() - 1);
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosureverildi"));
									SetQuestFlag("bio.sure",1);
									SetQuestFlag("bio.kalan",0);
									ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d %d", BiyologSistemi[GetQuestFlag("bio.durum")][0], GetQuestFlag("bio.verilen"), BiyologSistemi[GetQuestFlag("bio.durum")][1], GetQuestFlag("bio.kalan"),BiyologSistemi[GetQuestFlag("bio.durum")][4],BiyologSistemi[GetQuestFlag("bio.durum")][5],GetQuestFlag("bio.ruhtasi"));
								}
								break;
							case 96054:
								if(int(GetQuestFlag("bio.sure")) == 1)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosurezatenaktif"));
								}
								else if(GetQuestFlag("bio.durum") > 20)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevinyok"));
									ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
								}
								else
								{
									item->SetCount(item->GetCount() - 1);
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosureverildi"));
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosansverildi"));
									SetQuestFlag("bio.sure",1);
									SetQuestFlag("bio.sans",1);
									SetQuestFlag("bio.kalan",0);
									ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d %d", BiyologSistemi[GetQuestFlag("bio.durum")][0], GetQuestFlag("bio.verilen"), BiyologSistemi[GetQuestFlag("bio.durum")][1], GetQuestFlag("bio.kalan"), BiyologSistemi[GetQuestFlag("bio.durum")][4],BiyologSistemi[GetQuestFlag("bio.durum")][5], GetQuestFlag("bio.ruhtasi"));
								}
								break;
						case 80020:
								{
									if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || GetOfflineShopOwner())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("거래창,창고 등을 연 상태에서는 귀환부,귀환기억부 를 사용할수 없습니다."));
										break;
									}
									if (IsSashCombinationOpen() || IsSashAbsorptionOpen())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
										break;
									}
									if (GetCheque() + 1 >= 99999)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Uzerinde cok fazla won tasiyorsun."));
										return false;
									}
									if (1 + GetCheque() < 0)
										sys_err("QUEST wrong ChangeCheque %d (now %d)", 1, GetCheque());
									else
									{
										DBManager::instance().SendMoneyLog(MONEY_LOG_QUEST, GetPlayerID(), 1);
										PointChange(POINT_CHEQUE, 1, true);
									}
									// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("1 won kazandin."));

									item->SetCount(item->GetCount() - 1);
								}
								break;
							case 80005:
								{
									if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || GetOfflineShopOwner())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("거래창,창고 등을 연 상태에서는 귀환부,귀환기억부 를 사용할수 없습니다."));
										break;
									}
									if (IsSashCombinationOpen() || IsSashAbsorptionOpen())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
										break;
									}
									const int64_t nTotalMoney = static_cast<int64_t>(GetGold()) + static_cast<int64_t>(100000000);
									if (GOLD_MAX <= nTotalMoney || nTotalMoney < 0)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Uzerinde cok fazla yang tasiyorsun."));
										return false;
									}
									if (nTotalMoney < 0)
									{
										sys_err("QUEST wrong ChangeCheque %d (now %d)", 1, GetGold());
									}
									else
									{
										DBManager::instance().SendMoneyLog(MONEY_LOG_QUEST, GetPlayerID(), 100000000);
#ifdef YANG_LIMIT
										GoldChange(100000000);
#else
										PointChange(POINT_GOLD, 100000000);
#endif
									}
									// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("100000000 yang kazandin."));
									item->SetCount(item->GetCount() - 1);
								}
								break;
							case 27996: // 독병
								//item->SetCount(item->GetCount() - 1);
								/*if (GetSkillLevel(SKILL_CREATE_POISON))
								  AddAffect(AFFECT_ATT_GRADE, POINT_ATT_GRADE, 3, AFF_DRINK_POISON, 15*60, 0, true);
								  else
								  {
								// 독다루기가 없으면 50% 즉사 50% 공격력 +2
								if (number(0, 1))
								{
								if (GetHP() > 100)
								PointChange(POINT_HP, -(GetHP() - 1));
								else
								Dead();
								}
								else
								AddAffect(AFFECT_ATT_GRADE, POINT_ATT_GRADE, 2, AFF_DRINK_POISON, 15*60, 0, true);
								}*/
								break;

							case 27987: // 조개
								// 50  돌조각 47990
								// 30  꽝
								// 10  백진주 47992
								// 7   청진주 47993
								// 3   피진주 47994
								{
									item->SetCount(item->GetCount() - 1);

									int r = number(1, 100);

									if (r <= 50)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("조개에서 돌조각이 나왔습니다."));
										AutoGiveItem(27990);
									}
									else
									{
										const int prob_table_euckr[] =
										{
											80, 90, 97
										};

										const int prob_table_gb2312[] =
										{
											95, 97, 99
										};

										const int * prob_table = !g_iUseLocale ? prob_table_euckr : prob_table_gb2312;

										if (r <= prob_table[0])
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("조개가 흔적도 없이 사라집니다."));
										}
										else if (r <= prob_table[1])
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("조개에서 백진주가 나왔습니다."));
											AutoGiveItem(27992);
										}
										else if (r <= prob_table[2])
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("조개에서 청진주가 나왔습니다."));
											AutoGiveItem(27993);
										}
										else
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("조개에서 피진주가 나왔습니다."));
											AutoGiveItem(27994);
										}
									}
								}
								break;

#ifdef ENABLE_TITLE_SYSTEM
			case 79022:
				if (!this)
					return false;
				if (FindAffect(PRESTIGE_AFFECT_1))
					RemoveAffect(PRESTIGE_AFFECT_1);
				if (FindAffect(PRESTIGE_AFFECT_2))
					RemoveAffect(PRESTIGE_AFFECT_2);
				if (FindAffect(PRESTIGE_AFFECT_3))
					RemoveAffect(PRESTIGE_AFFECT_3);
				ITEM_MANAGER::instance().RemoveItem(item);
				AddAffect(PRESTIGE_AFFECT_1, POINT_ATTBONUS_MONSTER, 100, 0, 60 * 60 * 24 * 15, 0, true);
				TitleManager::instance().UpdateTitle(this, 14, 0);
				break;
			case 79023:
				if (!this)
					return false;
				if (FindAffect(PRESTIGE_AFFECT_1))
					RemoveAffect(PRESTIGE_AFFECT_1);
				if (FindAffect(PRESTIGE_AFFECT_2))
					RemoveAffect(PRESTIGE_AFFECT_2);
				if (FindAffect(PRESTIGE_AFFECT_3))
					RemoveAffect(PRESTIGE_AFFECT_3);
				ITEM_MANAGER::instance().RemoveItem(item);
				AddAffect(PRESTIGE_AFFECT_1, POINT_ATTBONUS_MONSTER, 200, 0, 60 * 60 * 24 * 15, 0, true);
				TitleManager::instance().UpdateTitle(this, 15, 0);
				break;
			case 79024:
				if (!this)
					return false;
				if (FindAffect(PRESTIGE_AFFECT_1))
					RemoveAffect(PRESTIGE_AFFECT_1);
				if (FindAffect(PRESTIGE_AFFECT_2))
					RemoveAffect(PRESTIGE_AFFECT_2);
				if (FindAffect(PRESTIGE_AFFECT_3))
					RemoveAffect(PRESTIGE_AFFECT_3);
				ITEM_MANAGER::instance().RemoveItem(item);
				AddAffect(PRESTIGE_AFFECT_1, POINT_ATTBONUS_MONSTER, 300, 0, 60 * 60 * 24 * 15, 0, true);
				TitleManager::instance().UpdateTitle(this, 16, 0);
				break;
			case 79025:
				if (!this)
					return false;
				if (FindAffect(PRESTIGE_AFFECT_1))
					RemoveAffect(PRESTIGE_AFFECT_1);
				if (FindAffect(PRESTIGE_AFFECT_2))
					RemoveAffect(PRESTIGE_AFFECT_2);
				if (FindAffect(PRESTIGE_AFFECT_3))
					RemoveAffect(PRESTIGE_AFFECT_3);
				ITEM_MANAGER::instance().RemoveItem(item);
				AddAffect(PRESTIGE_AFFECT_1, POINT_ATTBONUS_MONSTER, 400, 0, 60 * 60 * 24 * 15, 0, true);
				TitleManager::instance().UpdateTitle(this, 17, 0);
				break;
			case 79026:
				if (!this)
					return false;
				if (FindAffect(PRESTIGE_AFFECT_1))
					RemoveAffect(PRESTIGE_AFFECT_1);
				if (FindAffect(PRESTIGE_AFFECT_2))
					RemoveAffect(PRESTIGE_AFFECT_2);
				if (FindAffect(PRESTIGE_AFFECT_3))
					RemoveAffect(PRESTIGE_AFFECT_3);
				ITEM_MANAGER::instance().RemoveItem(item);
				AddAffect(PRESTIGE_AFFECT_2, POINT_ATTBONUS_MONSTER, 500, 0, 60 * 60 * 24 * 15, 0, true);
				TitleManager::instance().UpdateTitle(this, 18, 0);
				break;
			case 79027:
				if (!this)
					return false;
				if (FindAffect(PRESTIGE_AFFECT_1))
					RemoveAffect(PRESTIGE_AFFECT_1);
				if (FindAffect(PRESTIGE_AFFECT_2))
					RemoveAffect(PRESTIGE_AFFECT_2);
				if (FindAffect(PRESTIGE_AFFECT_3))
					RemoveAffect(PRESTIGE_AFFECT_3);
				ITEM_MANAGER::instance().RemoveItem(item);
				AddAffect(PRESTIGE_AFFECT_3, POINT_ATTBONUS_MONSTER, 600, 0, 60 * 60 * 24 * 15, 0, true);
				TitleManager::instance().UpdateTitle(this, 19, 0);
				break;
#endif

								
							case 60004:
								ChatPacket(CHAT_TYPE_COMMAND, "OpenShopSearch 0");
								break;

							case 60005:
								ChatPacket(CHAT_TYPE_COMMAND, "OpenShopSearch 1");
								break;
								
							case 71107: 
								{
									if (quest::CQuestManager::instance().GetEventFlag("rebirtlvl") == 1)
									{
										ChatPacket(CHAT_TYPE_INFO, "Sistem suan icin devre disi!");
										return false;
									}
									
									int iPulse = thecore_pulse();

									if (iPulse - GetLastRebirtLastTime() < passes_per_sec * 3)
									{
										ChatPacket(CHAT_TYPE_INFO, "Biraz beklemelisin.");
										return false;
									}

									SetRebirtLastTime(iPulse);

									if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("다른 거래중(창고,교환,상점)에는 개인상점을 사용할 수 없습니다."));
										return false;
									}

									if (GetLevel() < 120)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("lvlkucuk"));
										return false;
									}

									if (GetRebirt() >= 20)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("rebirtfull"));
										return false;
									}

									if (GetRebirt() < REBIRT_MAX)
									{
										SetRebirt(GetRebirt()+1);
										SetExp(0);


										if (GetRebirt() == 1 || GetRebirt() == 2 || GetRebirt() == 3 || GetRebirt() == 4 || GetRebirt() == 5)
										{
											EffectPacket(SE_DUEL_SENDER);
										}
										else if (GetRebirt() == 6 || GetRebirt() == 7 || GetRebirt() == 8 || GetRebirt() == 9 || GetRebirt() == 10)
										{
											EffectPacket(SE_DUEL_RECEIVER);
										}

										else if (GetRebirt() == 11 || GetRebirt() == 12 || GetRebirt() == 13 || GetRebirt() == 14 || GetRebirt() == 15)
										{
											EffectPacket(SE_DUEL_RECEIVER);
										}

										else if (GetRebirt() == 16 || GetRebirt() == 17 || GetRebirt() == 18 || GetRebirt() == 19 || GetRebirt() == 20)
										{
											EffectPacket(SE_DUEL_RECEIVER);
										}

									if (GetRebirt() >= 1 && GetRebirt() <= 5)
									{
										BYTE bRebirt = GetRebirt();
										DWORD PointCanavar = bRebirt*5;
										DWORD PointMetin = bRebirt*5;

										AddAffect(AFFECT_COLLECT, POINT_ATTBONUS_MONSTER, PointCanavar, 0, 60*60*24*365*60, 0, false);
										AddAffect(AFFECT_COLLECT, POINT_ATT_METIN, PointMetin, 0, 60*60*24*365*60, 0, false);

										ChatPacket(CHAT_TYPE_COMMAND, "rebirtodul2 %u %u", PointCanavar, PointMetin);
										PointsPacket();
									}
									else if (GetRebirt() >= 6 && GetRebirt() <= 10)
									{
										BYTE bRebirt = GetRebirt();
										DWORD PointCanavar = bRebirt*10;
										DWORD PointBoss = bRebirt*10;

										AddAffect(AFFECT_COLLECT, POINT_ATTBONUS_MONSTER, PointCanavar, 0, 60*60*24*365*60, 0, false);
										AddAffect(AFFECT_COLLECT, POINT_ATT_BOSS, PointBoss, 0, 60*60*24*365*60, 0, false);

										ChatPacket(CHAT_TYPE_COMMAND, "rebirtodul %u %u", PointCanavar, PointBoss);
										PointsPacket();
									}
									else if (GetRebirt() >= 11 && GetRebirt() <= 15)
									{
										BYTE bRebirt = GetRebirt();
										DWORD PointCanavar = bRebirt*15;
										DWORD PointGrade = bRebirt*10;

										AddAffect(AFFECT_COLLECT, POINT_ATTBONUS_MONSTER, PointCanavar, 0, 60*60*24*365*60, 0, false);
										AddAffect(AFFECT_COLLECT, POINT_ATT_GRADE_BONUS, PointGrade, 0, 60*60*24*365*60, 0, false);

										ChatPacket(CHAT_TYPE_COMMAND, "rebirtodul3 %u %u", PointCanavar, PointGrade);
										PointsPacket();
									}
									else if (GetRebirt() >= 16 && GetRebirt() <= 20)
									{
										BYTE bRebirt = GetRebirt();
										DWORD PointCanavar = bRebirt*20;
										DWORD PointMelee = bRebirt*2;

										AddAffect(AFFECT_COLLECT, POINT_ATTBONUS_MONSTER, PointCanavar, 0, 60*60*24*365*60, 0, false);
										AddAffect(AFFECT_COLLECT, POINT_MELEE_MAGIC_ATT_BONUS_PER, PointMelee, 0, 60*60*24*365*60, 0, false);

										ChatPacket(CHAT_TYPE_COMMAND, "rebirtodul4 %u %u", PointCanavar, PointMelee);
										PointsPacket();
									}
									}
									item->SetCount(item->GetCount() - 1);
								}
							break;

							case 50513:
								ChatPacket(CHAT_TYPE_COMMAND, "ruhtasiekranac");
								break;

							case 71013: // 축제용폭죽
								CreateFly(number(FLY_FIREWORK1, FLY_FIREWORK6), this);
								item->SetCount(item->GetCount() - 1);
								break;

							case 50100: // 폭죽
							case 50101:
							case 50102:
							case 50103:
							case 50104:
							case 50105:
							case 50106:
								CreateFly(item->GetVnum() - 50100 + FLY_FIREWORK1, this);
								item->SetCount(item->GetCount() - 1);
								break;

							case 50200: // 보따리
								if (LC_IsYMIR() == true || LC_IsKorea() == true)
								{
									if (IS_BOTARYABLE_ZONE(GetMapIndex()) == true)
									{
										__OpenOfflineShop();
									}
									else
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("개인 상점을 열 수 없는 지역입니다"));
									}
								}
								else
								{
									__OpenOfflineShop();
								}
								break;

							case fishing::FISH_MIND_PILL_VNUM:
								AddAffect(AFFECT_FISH_MIND_PILL, POINT_NONE, 0, AFF_FISH_MIND, 20*60, 0, true);
								item->SetCount(item->GetCount() - 1);
								break;

							case 50301: // 통솔력 수련서
							case 50302:
							case 50303:
								{
									if (IsPolymorphed() == true)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("둔갑 중에는 능력을 올릴 수 없습니다."));
										return false;
									}

									int lv = GetSkillLevel(SKILL_LEADERSHIP);

									if (lv < item->GetValue(0))
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 책은 너무 어려워 이해하기가 힘듭니다."));
										return false;
									}

									if (lv >= item->GetValue(1))
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 책은 아무리 봐도 도움이 될 것 같지 않습니다."));
										return false;
									}

									if (LearnSkillByBook(SKILL_LEADERSHIP))
									{
										item->SetCount(item->GetCount() - 1);

										int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
										if (distribution_test_server) iReadDelay /= 3;

										SetSkillNextReadTime(SKILL_LEADERSHIP, get_global_time() + iReadDelay);
									}
								}
								break;

							case 50331: // ??? ???
							case 50332:
							case 50333:
							case 50334: 
							case 50335:
							case 50336:
							case 50337:
							case 50338:
							case 50339:
							case 50340:
								{
									if (IsPolymorphed() == true)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("변신중에는 책을 읽을수 없습니다."));
										return false;
									}
									
									if (item->GetValue(0) == 0)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("hata_var"));
										return false;
									}

									int lv = GetSkillLevel(item->GetValue(0));
									
									if (lv == 0)
									{
										lv = 10;
									}

									if (lv >= 40)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("beceri_zaten_son_seviyede"));
										return false;
									}
									
									// SetSkillLevel(item->GetValue(0), 40);
									SetSkillLevel(item->GetValue(0), lv+10);
									ComputePoints();
									SkillLevelPacket();
									PointsPacket();
									item->SetCount(item->GetCount() - 1);
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("skillpoldu"));
									
								}
								break;
							case 50304: // 연계기 수련서
							case 50305:
							case 50306:
								{
									if (IsPolymorphed())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("변신중에는 책을 읽을수 없습니다."));
										return false;
										
									}
									if (GetSkillLevel(SKILL_COMBO) == 0 && GetLevel() < 30)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("레벨 30이 되기 전에는 습득할 수 있을 것 같지 않습니다."));
										return false;
									}

									if (GetSkillLevel(SKILL_COMBO) == 1 && GetLevel() < 50)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("레벨 50이 되기 전에는 습득할 수 있을 것 같지 않습니다."));
										return false;
									}

									if (GetSkillLevel(SKILL_COMBO) >= 2)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("연계기는 더이상 수련할 수 없습니다."));
										return false;
									}

									int iPct = item->GetValue(0);

									if (LearnSkillByBook(SKILL_COMBO, iPct))
									{
										item->SetCount(item->GetCount() - 1);

										int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
										if (distribution_test_server) iReadDelay /= 3;

										SetSkillNextReadTime(SKILL_COMBO, get_global_time() + iReadDelay);
									}
								}
								break;
							case 50311: // 언어 수련서
							case 50312:
							case 50313:
								{
									if (IsPolymorphed())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("변신중에는 책을 읽을수 없습니다."));
										return false;
										
									}
									DWORD dwSkillVnum = item->GetValue(0);
									int iPct = MINMAX(0, item->GetValue(1), 100);
									if (GetSkillLevel(dwSkillVnum)>=20 || dwSkillVnum-SKILL_LANGUAGE1+1 == GetEmpire())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 완벽하게 알아들을 수 있는 언어이다."));
										return false;
									}

									if (LearnSkillByBook(dwSkillVnum, iPct))
									{
										item->SetCount(item->GetCount() - 1);

										int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
										if (distribution_test_server) iReadDelay /= 3;

										SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
									}
								}
								break;

							case 50061 : // 일본 말 소환 스킬 수련서
								{
									if (IsPolymorphed())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("변신중에는 책을 읽을수 없습니다."));
										return false;
										
									}
									DWORD dwSkillVnum = item->GetValue(0);
									int iPct = MINMAX(0, item->GetValue(1), 100);

									if (GetSkillLevel(dwSkillVnum) >= 10)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("더 이상 수련할 수 없습니다."));
										return false;
									}

									if (LearnSkillByBook(dwSkillVnum, iPct))
									{
										item->SetCount(item->GetCount() - 1);

										int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
										if (distribution_test_server) iReadDelay /= 3;

										SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
									}
								}
								break;

							case 50314: case 50315: case 50316: // 변신 수련서
							case 50323: case 50324: // 증혈 수련서
							case 50325: case 50326: // 철통 수련서
								{
									if (IsPolymorphed() == true)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("둔갑 중에는 능력을 올릴 수 없습니다."));
										return false;
									}
									
									int iSkillLevelLowLimit = item->GetValue(0);
									int iSkillLevelHighLimit = item->GetValue(1);
									int iPct = MINMAX(0, item->GetValue(2), 100);
									int iLevelLimit = item->GetValue(3);
									DWORD dwSkillVnum = 0;
									
									switch (item->GetVnum())
									{
										case 50314: case 50315: case 50316:
											dwSkillVnum = SKILL_POLYMORPH;
											break;

										case 50323: case 50324:
											dwSkillVnum = SKILL_ADD_HP;
											break;

										case 50325: case 50326:
											dwSkillVnum = SKILL_RESIST_PENETRATE;
											break;

										default:
											return false;
									}

									if (0 == dwSkillVnum)
										return false;

									if (GetLevel() < iLevelLimit)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 책을 읽으려면 레벨을 더 올려야 합니다."));
										return false;
									}

									if (GetSkillLevel(dwSkillVnum) >= 40)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("더 이상 수련할 수 없습니다."));
										return false;
									}

									if (GetSkillLevel(dwSkillVnum) < iSkillLevelLowLimit)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 책은 너무 어려워 이해하기가 힘듭니다."));
										return false;
									}

									if (GetSkillLevel(dwSkillVnum) >= iSkillLevelHighLimit)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 책으로는 더 이상 수련할 수 없습니다."));
										return false;
									}

									if (LearnSkillByBook(dwSkillVnum, iPct))
									{
										item->SetCount(item->GetCount() - 1);

										int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
										if (distribution_test_server) iReadDelay /= 3;

										SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
									}
								}
								break;

							case 50902:
							case 50903:
							case 50904:
								{
									if (IsPolymorphed())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("변신중에는 책을 읽을수 없습니다."));
										return false;
										
									}
									DWORD dwSkillVnum = SKILL_CREATE;
									int iPct = MINMAX(0, item->GetValue(1), 100);

									if (GetSkillLevel(dwSkillVnum)>=40)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("더 이상 수련할 수 없습니다."));
										return false;
									}

									if (LearnSkillByBook(dwSkillVnum, iPct))
									{
										item->SetCount(item->GetCount() - 1);

										int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
										if (distribution_test_server) iReadDelay /= 3;

										SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);

										if (test_server) 
										{
											ChatPacket(CHAT_TYPE_INFO, "[TEST_SERVER] Success to learn skill ");
										}
									}
									else
									{
										if (test_server) 
										{
											ChatPacket(CHAT_TYPE_INFO, "[TEST_SERVER] Failed to learn skill ");
										}
									}
								}
								break;

								// MINING
							case ITEM_MINING_SKILL_TRAIN_BOOK:
								{
									if (IsPolymorphed())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("변신중에는 책을 읽을수 없습니다."));
										return false;
										
									}
									DWORD dwSkillVnum = SKILL_MINING;
									int iPct = MINMAX(0, item->GetValue(1), 100);

									if (GetSkillLevel(dwSkillVnum)>=40)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("더 이상 수련할 수 없습니다."));
										return false;
									}

									if (LearnSkillByBook(dwSkillVnum, iPct))
									{
										item->SetCount(item->GetCount() - 1);

										int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
										if (distribution_test_server) iReadDelay /= 3;

										SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
									}
								}
								break;
								// END_OF_MINING

							case ITEM_HORSE_SKILL_TRAIN_BOOK:
								{
									if (IsPolymorphed())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("변신중에는 책을 읽을수 없습니다."));
										return false;
										
									}
									DWORD dwSkillVnum = SKILL_HORSE;
									int iPct = MINMAX(0, item->GetValue(1), 100);

									if (GetLevel() < 50)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아직 승마 스킬을 수련할 수 있는 레벨이 아닙니다."));
										return false;
									}

									if (!test_server && get_global_time() < GetSkillNextReadTime(dwSkillVnum))
									{
										if (FindAffect(AFFECT_SKILL_NO_BOOK_DELAY))
										{
											// 주안술서 사용중에는 시간 제한 무시
											RemoveAffect(AFFECT_SKILL_NO_BOOK_DELAY);
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("주안술서를 통해 주화입마에서 빠져나왔습니다."));
										}
										else
										{
											SkillLearnWaitMoreTimeMessage(GetSkillNextReadTime(dwSkillVnum) - get_global_time());
											return false;
										}
									}

									if (GetPoint(POINT_HORSE_SKILL) >= 20 || 
											GetSkillLevel(SKILL_HORSE_WILDATTACK) + GetSkillLevel(SKILL_HORSE_CHARGE) + GetSkillLevel(SKILL_HORSE_ESCAPE) >= 60 ||
											GetSkillLevel(SKILL_HORSE_WILDATTACK_RANGE) + GetSkillLevel(SKILL_HORSE_CHARGE) + GetSkillLevel(SKILL_HORSE_ESCAPE) >= 60)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("더 이상 승마 수련서를 읽을 수 없습니다."));
										return false;
									}

									if (number(1, 100) <= iPct)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("승마 수련서를 읽어 승마 스킬 포인트를 얻었습니다."));
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("얻은 포인트로는 승마 스킬의 레벨을 올릴 수 있습니다."));
										PointChange(POINT_HORSE_SKILL, 1);

										int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
										if (distribution_test_server) iReadDelay /= 3;

										if (!test_server)
											SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
									}
									else
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("승마 수련서 이해에 실패하였습니다."));
									}

									item->SetCount(item->GetCount() - 1);
								}
								break;

							case 70102: // 선두
							case 70103: // 선두
								{
									if (GetAlignment() >= 0)
										return false;

									int delta = MIN(-GetAlignment(), item->GetValue(0));

									sys_log(0, "%s ALIGNMENT ITEM %d", GetName(), delta);

									UpdateAlignment(delta);
									item->SetCount(item->GetCount() - 1);

									if (delta / 10 > 0)
									{
										ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("마음이 맑아지는군. 가슴을 짓누르던 무언가가 좀 가벼워진 느낌이야."));
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("선악치가 %d 증가하였습니다."), delta/10);
									}
								}
								break;
							
							case 39032:
							case 40008: // 천도복숭아
								{
									int val = item->GetValue(0);
									// int interval = item->GetValue(1);
									// quest::PC* pPC = quest::CQuestManager::instance().GetPC(GetPlayerID());
									// int last_use_time = pPC->GetFlag("mythical_peach.last_use_time");

									// if (get_global_time() - last_use_time < interval * 60 * 60)
									// {
										// if (test_server == false)
										// {
											// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아직 사용할 수 없습니다."));
											// return false;
										// }
										// else
										// {
											// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("테스트 서버 시간제한 통과"));
										// }
									// }
									
									if (GetAlignment() == 1500000)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("yasammeyvesiyeter"));
										return false;
									}
									
									if (1500000 - GetAlignment() < val * 10)
									{
										val = (1500000 - GetRealAlignment());

									}
									else
									{
										val = val*10;
									}
									
									int old_alignment = GetAlignment() / 10;

									UpdateAlignment(val);
									
									item->SetCount(item->GetCount()-1);

									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("선악치가 %d 증가하였습니다."), val);

									char buf[256 + 1];
									snprintf(buf, sizeof(buf), "%d %d", old_alignment, GetAlignment() / 10);
									LogManager::instance().CharLog(this, val, "MYTHICAL_PEACH", buf);
								}
								break;

							case 71109: // 탈석서
							case 72719:
								{
									LPITEM item2;

									if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
										return false;

									if (item2->IsExchanging() || item2->IsEquipped()) // @fixme114
										return false;

									if (item2->GetSocketCount() == 0)
										return false;

									switch( item2->GetType() )
									{
										case ITEM_WEAPON:
											break;
										case ITEM_ARMOR:
											switch (item2->GetSubType())
											{
											case ARMOR_EAR:
											case ARMOR_WRIST:
											case ARMOR_NECK:
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("빼낼 영석이 없습니다"));
												return false;
											}
											break;

										default:
											return false;
									}

									std::stack<long> socket;

									for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
										socket.push(item2->GetSocket(i));

									int idx = ITEM_SOCKET_MAX_NUM - 1;

									while (socket.size() > 0)
									{
										if (socket.top() > 2 && socket.top() != ITEM_BROKEN_METIN_VNUM)
											break;

										idx--;
										socket.pop();
									}

									if (socket.size() == 0)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("빼낼 영석이 없습니다"));
										return false;
									}

									LPITEM pItemReward = AutoGiveItem(socket.top());

									if (pItemReward != NULL)
									{
										item2->SetSocket(idx, 1);

										char buf[256+1];
										snprintf(buf, sizeof(buf), "%s(%u) %s(%u)", 
												item2->GetName(), item2->GetID(), pItemReward->GetName(), pItemReward->GetID());
										LogManager::instance().ItemLog(this, item, "USE_DETACHMENT_ONE", buf);

										item->SetCount(item->GetCount() - 1);
									}
								}
								break;

							case 70201:   // 탈색제
							case 70202:   // 염색약(흰색)
							case 70203:   // 염색약(금색)
							case 70204:   // 염색약(빨간색)
							case 70205:   // 염색약(갈색)
							case 70206:   // 염색약(검은색)
								{
									// NEW_HAIR_STYLE_ADD
									if (GetPart(PART_HAIR) >= 1001)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("현재 헤어스타일에서는 염색과 탈색이 불가능합니다."));
									}
									// END_NEW_HAIR_STYLE_ADD
									else
									{
										quest::CQuestManager& q = quest::CQuestManager::instance();
										quest::PC* pPC = q.GetPC(GetPlayerID());

										if (pPC)
										{
											int last_dye_level = pPC->GetFlag("dyeing_hair.last_dye_level");

											if (last_dye_level == 0 ||
													last_dye_level+3 <= GetLevel() ||
													item->GetVnum() == 70201)
											{
												SetPart(PART_HAIR, item->GetVnum() - 70201);

												if (item->GetVnum() == 70201)
													pPC->SetFlag("dyeing_hair.last_dye_level", 0);
												else
													pPC->SetFlag("dyeing_hair.last_dye_level", GetLevel());

												item->SetCount(item->GetCount() - 1);
												UpdatePacket();
											}
											else
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d 레벨이 되어야 다시 염색하실 수 있습니다."), last_dye_level+3);
											}
										}
									}
								}
								break;

#ifdef LWT_BUFFI_SYSTEM
							case BUFF_P_RING: // buffi p ring
							{
							if (GetExchange() || IsOpenSafebox() || GetShopOwner() || GetOfflineShopOwner() || IsCubeOpen())
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("acik_pencere_engel"));
								return false;
							}
							if (IsHack())
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("tic_10sn_engel"));
								return false;
							}
								SetSkillLevel(245,40);
								SetSkillLevel(246,40);
								SetSkillLevel(247,40);
								SetSkillLevel(248,40);
								SetSkillLevel(249,40);
								//ComputePoints();
								SkillLevelPacket();
								item->SetCount(item->GetCount()-1);
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("buff_basarili"));
							}
							break;
#endif

							case ITEM_NEW_YEAR_GREETING_VNUM:
								{
									DWORD dwBoxVnum = ITEM_NEW_YEAR_GREETING_VNUM;
									std::vector <DWORD> dwVnums;
									std::vector <DWORD> dwCounts;
									std::vector <LPITEM> item_gets;
									int count = 0;

									if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
									{
										for (int i = 0; i < count; i++)
										{
											if (dwVnums[i] == CSpecialItemGroup::GOLD)
#if defined(__CHATTING_WINDOW_RENEWAL__)
												ChatPacket(CHAT_TYPE_MONEY_INFO, LC_TEXT("돈 %d 냥을 획득했습니다."), dwCounts[i]);
#else
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("돈 %d 냥을 획득했습니다."), dwCounts[i]);
#endif
										}

										item->SetCount(item->GetCount() - 1);
									}
								}
								break;

							case ITEM_VALENTINE_ROSE:
							case ITEM_VALENTINE_CHOCOLATE:
								{
									DWORD dwBoxVnum = item->GetVnum();
									std::vector <DWORD> dwVnums;
									std::vector <DWORD> dwCounts;
									std::vector <LPITEM> item_gets(0);
									int count = 0;


									if (item->GetVnum() == ITEM_VALENTINE_ROSE && SEX_MALE==GET_SEX(this) ||
										item->GetVnum() == ITEM_VALENTINE_CHOCOLATE && SEX_FEMALE==GET_SEX(this))
									{
										// 성별이 맞지않아 쓸 수 없다.
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("성별이 맞지않아 이 아이템을 열 수 없습니다."));
										return false;
									}


									if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
										item->SetCount(item->GetCount()-1);
								}
								break;

							case ITEM_WHITEDAY_CANDY:
							case ITEM_WHITEDAY_ROSE:
								{
									DWORD dwBoxVnum = item->GetVnum();
									std::vector <DWORD> dwVnums;
									std::vector <DWORD> dwCounts;
									std::vector <LPITEM> item_gets(0);
									int count = 0;


									if (item->GetVnum() == ITEM_WHITEDAY_CANDY && SEX_MALE==GET_SEX(this) ||
										item->GetVnum() == ITEM_WHITEDAY_ROSE && SEX_FEMALE==GET_SEX(this))
									{
										// 성별이 맞지않아 쓸 수 없다.
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("성별이 맞지않아 이 아이템을 열 수 없습니다."));
										return false;
									}


									if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
										item->SetCount(item->GetCount()-1);
								}
								break;

							case 50011: // 월광보합
								{
									DWORD dwBoxVnum = 50011;
									std::vector <DWORD> dwVnums;
									std::vector <DWORD> dwCounts;
									std::vector <LPITEM> item_gets(0);
									int count = 0;

									if (GiveItemFromSpecialItemGroup(dwBoxVnum, dwVnums, dwCounts, item_gets, count))
									{
										for (int i = 0; i < count; i++)
										{
											char buf[50 + 1];
											snprintf(buf, sizeof(buf), "%u %u", dwVnums[i], dwCounts[i]);
											LogManager::instance().ItemLog(this, item, "MOONLIGHT_GET", buf);

											//ITEM_MANAGER::instance().RemoveItem(item);
											item->SetCount(item->GetCount() - 1);

											switch (dwVnums[i])
											{
											case CSpecialItemGroup::GOLD:
#if defined(__CHATTING_WINDOW_RENEWAL__)
												ChatPacket(CHAT_TYPE_MONEY_INFO, LC_TEXT("돈 %d 냥을 획득했습니다."), dwCounts[i]);
#else
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("돈 %d 냥을 획득했습니다."), dwCounts[i]);
#endif
												break;

											case CSpecialItemGroup::EXP:
#if defined(__CHATTING_WINDOW_RENEWAL__)
												ChatPacket(CHAT_TYPE_EXP_INFO, LC_TEXT("상자에서 부터 신비한 빛이 나옵니다."));
												ChatPacket(CHAT_TYPE_EXP_INFO, LC_TEXT("%d의 경험치를 획득했습니다."), dwCounts[i]);
#else
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 부터 신비한 빛이 나옵니다."));
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d의 경험치를 획득했습니다."), dwCounts[i]);
#endif
												break;

											case CSpecialItemGroup::MOB:
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 몬스터가 나타났습니다!"));
												break;

											case CSpecialItemGroup::SLOW:
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 나온 빨간 연기를 들이마시자 움직이는 속도가 느려졌습니다!"));
												break;

											case CSpecialItemGroup::DRAIN_HP:
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자가 갑자기 폭발하였습니다! 생명력이 감소했습니다."));
												break;

											case CSpecialItemGroup::POISON:
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 나온 녹색 연기를 들이마시자 독이 온몸으로 퍼집니다!"));
												break;

											case CSpecialItemGroup::MOB_GROUP:
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 몬스터가 나타났습니다!"));
												break;

											default:
												if (item_gets[i])
												{
#if defined(__CHATTING_WINDOW_RENEWAL__)
													if (dwCounts[i] > 1)
														ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("상자에서 %s 가 %d 개 나왔습니다."), item_gets[i]->GetName(), dwCounts[i]);
													else
														ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("상자에서 %s 가 나왔습니다."), item_gets[i]->GetName());
#else
													if (dwCounts[i] > 1)
														ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 %s 가 %d 개 나왔습니다."), item_gets[i]->GetName(), dwCounts[i]);
													else
														ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 %s 가 나왔습니다."), item_gets[i]->GetName());
#endif
												}
												break;
											}
										}
									}
									else
									{
										ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("아무것도 얻을 수 없었습니다."));
										return false;
									}
								}
								break;

							case ITEM_GIVE_STAT_RESET_COUNT_VNUM:
								{
									//PointChange(POINT_GOLD, -iCost);
									PointChange(POINT_STAT_RESET_COUNT, 1);
									item->SetCount(item->GetCount()-1);
								}
								break;

							case 50107:
								{
									EffectPacket(SE_CHINA_FIREWORK);
									// 스턴 공격을 올려준다
									AddAffect(AFFECT_CHINA_FIREWORK, POINT_STUN_PCT, 30, AFF_CHINA_FIREWORK, 5*60, 0, true);
									item->SetCount(item->GetCount()-1);
								}
								break;

							case 50108:
								{
									if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련 중에는 이용할 수 없는 물품입니다."));
										return false;
									}

									EffectPacket(SE_SPIN_TOP);
									// 스턴 공격을 올려준다
									AddAffect(AFFECT_CHINA_FIREWORK, POINT_STUN_PCT, 30, AFF_CHINA_FIREWORK, 5*60, 0, true);
									item->SetCount(item->GetCount()-1);
								}
								break;

							case ITEM_WONSO_BEAN_VNUM:
								PointChange(POINT_HP, GetMaxHP() - GetHP());
								item->SetCount(item->GetCount()-1);
								break;

							case ITEM_WONSO_SUGAR_VNUM:
								PointChange(POINT_SP, GetMaxSP() - GetSP());
								item->SetCount(item->GetCount()-1);
								break;

							case ITEM_WONSO_FRUIT_VNUM:
								PointChange(POINT_STAMINA, GetMaxStamina()-GetStamina());
								item->SetCount(item->GetCount()-1);
								break;

							case 90008: // VCARD
							case 90009: // VCARD
								VCardUse(this, this, item);
								break;

							case ITEM_ELK_VNUM: // 돈꾸러미
								{
									int iGold = item->GetSocket(0);
									ITEM_MANAGER::instance().RemoveItem(item);
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("돈 %d 냥을 획득했습니다."), iGold);
#ifdef YANG_LIMIT
									GoldChange(static_cast<LDWORD>(iGold), "ITEM_ELK");
#else
									PointChange(POINT_GOLD, iGold);
#endif
								}
								break;

#ifdef ENABLE_BATTLE_PASS
							case ITEM_BATTLE_PASS:
							{
								if (!v_counts.empty())
								{
									ChatPacket(CHAT_TYPE_INFO, "Zaten aktif durumda");
									return false;
								}

								FILE 	*fileID;
								char file_name[256 + 1];

								snprintf(file_name, sizeof(file_name), "%s/battlepass_players/%s.txt", LocaleService_GetBasePath().c_str(), GetName());
								fileID = fopen(file_name, "w");

								if (NULL == fileID)
									return false;

								for (int i = 0; i<missions_bp.size(); ++i)
								{
									fprintf(fileID, "MISSION	%d	%d\n", 0, 0);
								}

								fclose(fileID);

								Load_BattlePass();
								ChatPacket(CHAT_TYPE_INFO, "Bu ay icin savas karti etkinlestirildi!");
								item->SetCount(item->GetCount() - 1);
							}
							break;
#endif

case 80041:
								{
									LPITEM weapon = GetWear(WEAR_WEAPON);
									if (weapon)
									{
										ITEM_MANAGER::instance().RemoveItem(item);
										weapon->SetEvolution(weapon->GetEvolution()+1000);
										ChatPacket(CHAT_TYPE_INFO, "Silahina 1000 evrim puani yuklendi.");
									}
									else
									{
										ChatPacket(CHAT_TYPE_INFO, "Silahin giyili olmadigi icin evrim puani yukleyemedim.");
									}
								}
								break;

							case 80042:
								{
									LPITEM weapon = GetWear(WEAR_WEAPON);
									if (weapon)
									{
										ITEM_MANAGER::instance().RemoveItem(item);
										weapon->SetEvolution(weapon->GetEvolution()+6000);
										ChatPacket(CHAT_TYPE_INFO, "Silahina 6000 evrim puani yuklendi.");
									}
									else
									{
										ChatPacket(CHAT_TYPE_INFO, "Silahin giyili olmadigi icin evrim puani yukleyemedim.");
									}
								}
								break;
								
								//군주의 증표 
							case 70021:
								{
									int HealPrice = quest::CQuestManager::instance().GetEventFlag("MonarchHealGold");
									if (HealPrice == 0)
										HealPrice = 2000000;

									if (CMonarch::instance().HealMyEmpire(this, HealPrice))
									{
										char szNotice[256];
										snprintf(szNotice, sizeof(szNotice), LC_TEXT("군주의 축복으로 이지역 %s 유저는 HP,SP가 모두 채워집니다."), EMPIRE_NAME(GetEmpire()));
										SendNoticeMap(szNotice, GetMapIndex(), false);
										
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("군주의 축복을 사용하였습니다."));
									}
								}
								break;

							case 27995:
								{
								}
								break;

							case 71092 : // 변신 해체부 임시
								{
									if (m_pkChrTarget != NULL)
									{
										if (m_pkChrTarget->IsPolymorphed())
										{
											m_pkChrTarget->SetPolymorph(0);
											m_pkChrTarget->RemoveAffect(AFFECT_POLYMORPH);
										}
									}
									else
									{
										if (IsPolymorphed())
										{
											SetPolymorph(0);
											RemoveAffect(AFFECT_POLYMORPH);
										}
									}
								}
								break;
/////////////////////////////////////////////////////////////////////////////yardimci saman s?e ekleme/////////////////////////////////////////////
							case 91014:
								{
									LPITEM item2;
									#ifdef LWT_BUFFI_SYSTEM
									if (GetSupportSystem()->IsActiveSupport())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Once samani gonder."));
										return false;
									}
									#endif
									if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
										return false;
									
									if (item2->IsExchanging() || item2->IsEquipped())
										return false;
									if (item2->GetType() != ITEM_QUEST)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_error1"));
										return false;
									}
									if (item2->GetVnum() < 91010 || item2->GetVnum() > 91014)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_error1"));
										return false;
									}
									// if (item2->IsSealed())
									// {
										// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_sealed"));
										// return false;
									// }
									int oldsure = item2->GetSocket(0) - get_global_time();
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_success"));
									item2->SetSocket(0, get_global_time()+oldsure+86400);
									item->SetCount(item->GetCount()-1);
								}
								break;
							case 91015:
								{
									LPITEM item2;
									#ifdef LWT_BUFFI_SYSTEM
									if (GetSupportSystem()->IsActiveSupport())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Once samani gonder."));
										return false;
									}
									#endif
									if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
										return false;
									
									if (item2->IsExchanging() || item2->IsEquipped())
										return false;
									if (item2->GetType() != ITEM_QUEST)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_error1"));
										return false;
									}
									if (item2->GetVnum() < 91010 || item2->GetVnum() > 91014)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_error1"));
										return false;
									}
									// if (item2->IsSealed())
									// {
										// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_sealed"));
										// return false;
									// }
									int oldsure = item2->GetSocket(0) - get_global_time();
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_success1"));
									item2->SetSocket(0, get_global_time()+oldsure+(86400*3));
									item->SetCount(item->GetCount()-1);
								}
								break;
							case 91016:
								{
									LPITEM item2;
									#ifdef LWT_BUFFI_SYSTEM
									if (GetSupportSystem()->IsActiveSupport())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Once samani gonder."));
										return false;
									}
									#endif
									if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
										return false;
									
									if (item2->IsExchanging() || item2->IsEquipped())
										return false;
									if (item2->GetType() != ITEM_QUEST)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_error1"));
										return false;
									}
									if (item2->GetVnum() < 91010 || item2->GetVnum() > 91014)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_error1"));
										return false;
									}
									// if (item2->IsSealed())
									// {
										// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_sealed"));
										// return false;
									// }
									int oldsure = item2->GetSocket(0) - get_global_time();
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_success2"));
									item2->SetSocket(0, get_global_time()+oldsure+(86400*7));
									item->SetCount(item->GetCount()-1);
								}
								break;
							case 91017:
								{
									LPITEM item2;
									#ifdef LWT_BUFFI_SYSTEM
									if (GetSupportSystem()->IsActiveSupport())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Once samani gonder."));
										return false;
									}
									#endif
									if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
										return false;
									
									if (item2->IsExchanging() || item2->IsEquipped())
										return false;
									if (item2->GetType() != ITEM_QUEST)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_error1"));
										return false;
									}
									if (item2->GetVnum() < 91010 || item2->GetVnum() > 91014)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_error1"));
										return false;
									}
									// if (item2->IsSealed())
									// {
										// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_sealed"));
										// return false;
									// }
									int oldsure = item2->GetSocket(0) - get_global_time();
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("time_success3"));
									item2->SetSocket(0, get_global_time()+oldsure+(86400*14));
									item->SetCount(item->GetCount()-1);
								}
								break;
//////////////////////////////////////////// yardimci s?e sifirlama //////////////////////////////////////////////////////////////////////////////////////////
							case 71051 : // 진재가
								{
									// 유럽, 싱가폴, 베트남 진재가 사용금지
									if (LC_IsEurope() || LC_IsSingapore() || LC_IsVietnam())
										return false;

									LPITEM item2;

									if (!IsValidItemPosition(DestCell) || !(item2 = GetInventoryItem(wDestCell)))
										return false;

									if (item2->IsExchanging() == true)
										return false;

									if (item2->GetAttributeSetIndex() == -1)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 변경할 수 없는 아이템입니다."));
										return false;
									}

									if (item2->AddRareAttribute() == true)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("성공적으로 속성이 추가 되었습니다"));

										int iAddedIdx = item2->GetRareAttrCount() + 4;
										char buf[21];
										snprintf(buf, sizeof(buf), "%u", item2->GetID());

										LogManager::instance().ItemLog(
												GetPlayerID(),
												item2->GetAttributeType(iAddedIdx),
												item2->GetAttributeValue(iAddedIdx),
												item->GetID(),
												"ADD_RARE_ATTR",
												buf,
												GetDesc()->GetHostName(),
												item->GetOriginalVnum());

										item->SetCount(item->GetCount() - 1);
									}
									else
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("더 이상 이 아이템으로 속성을 추가할 수 없습니다"));
									}
								}
								break;

							case 71052 : // 진재경
								{
									// 유럽, 싱가폴, 베트남 진재가 사용금지
									if (LC_IsEurope() || LC_IsSingapore() || LC_IsVietnam())
										return false;

									LPITEM item2;

									if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
										return false;

									if (item2->IsExchanging() == true)
										return false;

									if (item2->GetAttributeSetIndex() == -1)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 변경할 수 없는 아이템입니다."));
										return false;
									}

									if (item2->ChangeRareAttribute() == true)
									{
										char buf[21];
										snprintf(buf, sizeof(buf), "%u", item2->GetID());
										LogManager::instance().ItemLog(this, item, "CHANGE_RARE_ATTR", buf);

										item->SetCount(item->GetCount() - 1);
									}
									else
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("변경 시킬 속성이 없습니다"));
									}
								}
								break;
								
							case 72720 :
								{
									LPITEM item2;

									if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
										return false;

									if (item2->IsExchanging() == true)
										return false;

									if (item2->IsEquipped())
										return false;
									
									if (item2->GetType() == ITEM_ARMOR && item2->GetSubType() == ARMOR_BODY)
									{
										
										int maxsocket = 6;
										
										if (item2->GetSocketCount() < 3)
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("tas_eklemeye_uygun_degil"));
											return false;
										}
										else if (item2->GetSocketCount() == maxsocket)
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("max_tas_%d"),maxsocket);
											return false;
										}
										int percs = 50;
										if (number(1, 100) <= percs)
										{
											item2->AddSocket();
											item2->UpdatePacket();
											item2->Save();
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("socket_acildi"));
										}
										else
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("delikbasarisiz."));

										item->SetCount(item->GetCount() - 1);
									}
									else
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("sadece_zirh"));
										break;
									}
								}
								break;
								
							case 72721 : 
								{
									LPITEM item2;

									if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
										return false;

									if (item2->IsExchanging() == true)
										return false;

									if (item2->IsEquipped())
										return false;

									if (item2->GetType() == ITEM_WEAPON)
									{
										int maxsocket = 6;
										
										if (item2->GetSocketCount() < 3)
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("tas_eklemeye_uygun_degil"));
											return false;
										}
										else if (item2->GetSocketCount() == maxsocket)
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("max_tas_%d"),maxsocket);
											return false;
										}
										int percs = 50;
										if (number(1, 100) <= percs)
										{
											item2->AddSocket();
											item2->UpdatePacket();
											item2->Save();
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("socket_acildi"));
										}
										else
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("delikbasarisiz."));

										item->SetCount(item->GetCount() - 1);
									}
									else
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("sadece_silah"));
										break;
									}
								}
								break;	
								
							case 71516:
							case 71517:
							case 71518:
							case 71519:
							case 71523:
							case 71524:
							case 71526:
							case 71527:
								{
									

									int		affect_type		= 510;
									int		apply_type		= aApplyInfo[item->GetValue(0)].bPointType;
									int		apply_value		= item->GetValue(1);
									
									if (IsAffectItem(item->GetVnum()) == true)
									{
										affect_type = GetAffectBonus(item->GetVnum());
									}
									
									CAffect* pAffect = FindAffect2(affect_type, apply_type, apply_value);
									
									
									if (NULL == pAffect)
									{

										item->Lock(true);
										item->SetSocket(0, true);

										// #ifdef ENABLE_NEW_AFFECT_POTION
											// SetAffectPotion(item);
										// #endif
										AddAffect(affect_type, apply_type, apply_value, item->GetID(), INFINITE_AFFECT_DURATION, 0, false);

									}
									else
									{
										if (item->GetID() == pAffect->dwFlag)
										{
											
											item->Lock(false);
											item->SetSocket(0, false);
											
											// #ifdef ENABLE_NEW_AFFECT_POTION
												// RemoveAffectPotion(item);
											// #endif
											RemoveAffect( pAffect );

										}
										else
										{
											LPITEM old = FindItemByID( pAffect->dwFlag );

											if (NULL != old)
											{
												old->Lock(false);
												old->SetSocket(0, false);
											}

											RemoveAffect( pAffect );


											
											AddAffect(affect_type, apply_type, apply_value, item->GetID(), INFINITE_AFFECT_DURATION, 0, false);

											item->Lock(true);
											item->SetSocket(0, true);


										}
									}
								}
								break;
							
							
							
							
							case 71510:
							case 71511:
							case 71512:
							case 71513:
							case 71514:
							case 71515:
							case 71525:
							case 71528:
							case 71529:
							case 71530:
								{
									int		affect_type		= AFFECT_BLEND;
									int		apply_type		= aApplyInfo[item->GetValue(0)].bPointType;
									int		apply_value		= item->GetValue(1);

									CAffect* pAffect = FindAffect2(affect_type, apply_type, apply_value);
									if (NULL == pAffect)
									{
										item->Lock(true);
										item->SetSocket(0, true);

										#ifdef ENABLE_NEW_AFFECT_POTION
											SetAffectPotion(item);
										#endif
										AddAffect(affect_type, apply_type, apply_value, item->GetID(), INFINITE_AFFECT_DURATION, 0, false);

									}
									else
									{
										if (item->GetID() == pAffect->dwFlag)
										{
											
											item->Lock(false);
											item->SetSocket(0, false);

											#ifdef ENABLE_NEW_AFFECT_POTION
												RemoveAffectPotion(item);
											#endif
											RemoveAffect( pAffect );

										}
										else
										{
											LPITEM old = FindItemByID( pAffect->dwFlag );

											if (NULL != old)
											{
												old->Lock(false);
												old->SetSocket(0, false);
											}

											RemoveAffect( pAffect );


											
											AddAffect(affect_type, apply_type, apply_value, item->GetID(), INFINITE_AFFECT_DURATION, 0, false);

											item->Lock(true);
											item->SetSocket(0, true);


										}
									}
								}
								break;

							case ITEM_AUTO_HP_RECOVERY_S:
							case ITEM_AUTO_HP_RECOVERY_M:
							case ITEM_AUTO_HP_RECOVERY_L:
							case ITEM_AUTO_HP_RECOVERY_X:
							case ITEM_AUTO_SP_RECOVERY_S:
							case ITEM_AUTO_SP_RECOVERY_M:
							case ITEM_AUTO_SP_RECOVERY_L:
							case ITEM_AUTO_SP_RECOVERY_X:
							// 무시무시하지만 이전에 하던 걸 고치기는 무섭고...
							// 그래서 그냥 하드 코딩. 선물 상자용 자동물약 아이템들.
							case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_XS: 
							case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_S: 
							case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_XS: 
							case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_S:
							case FUCKING_BRAZIL_ITEM_AUTO_SP_RECOVERY_S:
							case FUCKING_BRAZIL_ITEM_AUTO_HP_RECOVERY_S:
								{
									if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련장에서 사용하실 수 없습니다."));
										return false;
									}
									if (FindAffect(AFFECT_ITEM_BLOCK))
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("oto_pot_block!"));
										return false;
									}
									EAffectTypes type = AFFECT_NONE;
									bool isSpecialPotion = false;

									switch (item->GetVnum())
									{
										case ITEM_AUTO_HP_RECOVERY_X:
											isSpecialPotion = true;

										case ITEM_AUTO_HP_RECOVERY_S:
										case ITEM_AUTO_HP_RECOVERY_M:
										case ITEM_AUTO_HP_RECOVERY_L:
										case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_XS:
										case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_S:
										case FUCKING_BRAZIL_ITEM_AUTO_HP_RECOVERY_S:
											type = AFFECT_AUTO_HP_RECOVERY;
											break;

										case ITEM_AUTO_SP_RECOVERY_X:
											isSpecialPotion = true;

										case ITEM_AUTO_SP_RECOVERY_S:
										case ITEM_AUTO_SP_RECOVERY_M:
										case ITEM_AUTO_SP_RECOVERY_L:
										case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_XS:
										case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_S:
										case FUCKING_BRAZIL_ITEM_AUTO_SP_RECOVERY_S:
											type = AFFECT_AUTO_SP_RECOVERY;
											break;
									}

									if (AFFECT_NONE == type)
										break;

									if (item->GetCount() > 1)
									{
										int pos = GetEmptyInventory(item->GetSize());

										if (-1 == pos)
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소지품에 빈 공간이 없습니다."));
											break;
										}

										item->SetCount( item->GetCount() - 1 );

										LPITEM item2 = ITEM_MANAGER::instance().CreateItem( item->GetVnum(), 1 );
										item2->AddToCharacter(this, TItemPos(INVENTORY, pos));

										if (item->GetSocket(1) != 0)
										{
											item2->SetSocket(1, item->GetSocket(1));
										}

										item = item2;
									}

									CAffect* pAffect = FindAffect( type );

									if (NULL == pAffect)
									{
										EPointTypes bonus = POINT_NONE;

										if (true == isSpecialPotion)
										{
											if (type == AFFECT_AUTO_HP_RECOVERY)
											{
												bonus = POINT_MAX_HP_PCT;
											}
											else if (type == AFFECT_AUTO_SP_RECOVERY)
											{
												bonus = POINT_MAX_SP_PCT;
											}
										}

										AddAffect( type, bonus, 4, item->GetID(), INFINITE_AFFECT_DURATION, 0, true, false);

										item->Lock(true);
										item->SetSocket(0, true);

										AutoRecoveryItemProcess( type );
										AddAffect(AFFECT_ITEM_BLOCK, POINT_NONE, 0, 0, 5, 0, true, false);
									}
									else
									{
										if (item->GetID() == pAffect->dwFlag)
										{
											RemoveAffect( pAffect );

											item->Lock(false);
											item->SetSocket(0, false);
										}
										else
										{
											LPITEM old = FindItemByID( pAffect->dwFlag );

											if (NULL != old)
											{
												old->Lock(false);
												old->SetSocket(0, false);
											}

											RemoveAffect( pAffect );

											EPointTypes bonus = POINT_NONE;

											if (true == isSpecialPotion)
											{
												if (type == AFFECT_AUTO_HP_RECOVERY)
												{
													bonus = POINT_MAX_HP_PCT;
												}
												else if (type == AFFECT_AUTO_SP_RECOVERY)
												{
													bonus = POINT_MAX_SP_PCT;
												}
											}

											AddAffect( type, bonus, 4, item->GetID(), INFINITE_AFFECT_DURATION, 0, true, false);

											item->Lock(true);
											item->SetSocket(0, true);

											AutoRecoveryItemProcess( type );
											AddAffect(AFFECT_ITEM_BLOCK, POINT_NONE, 0, 0, 5, 0, true, false);
										}
									}
								}
								break;
						}
						break;

					case USE_CLEAR:
						{
							RemoveBadAffect();
							item->SetCount(item->GetCount() - 1);
						}
						break;

					case USE_INVISIBILITY:
						{
							if (item->GetVnum() == 70026)
							{
								quest::CQuestManager& q = quest::CQuestManager::instance();
								quest::PC* pPC = q.GetPC(GetPlayerID());

								if (pPC != NULL)
								{
									int last_use_time = pPC->GetFlag("mirror_of_disapper.last_use_time");

									if (get_global_time() - last_use_time < 10*60)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아직 사용할 수 없습니다."));
										return false;
									}

									pPC->SetFlag("mirror_of_disapper.last_use_time", get_global_time());
								}
							}

							AddAffect(AFFECT_INVISIBILITY, POINT_NONE, 0, AFF_INVISIBILITY, 300, 0, true);
							item->SetCount(item->GetCount() - 1);
						}
						break;

					case USE_POTION_NODELAY:
						{
							if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
							{
								if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit") > 0)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련장에서 사용하실 수 없습니다."));
									return false;
								}

								switch (item->GetVnum())
								{
									case 70020 :
									case 71018 :
									case 71019 :
									case 71020 :
										if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit_count") < 10000)
										{
											if (m_nPotionLimit <= 0)
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("사용 제한량을 초과하였습니다."));
												return false;
											}
										}
										break;

									default :
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련장에서 사용하실 수 없습니다."));
										return false;
								}
							}

							bool used = false;

							if (item->GetValue(0) != 0) // HP 절대값 회복
							{
								if (GetHP() < GetMaxHP())
								{
									PointChange(POINT_HP, item->GetValue(0) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
									EffectPacket(SE_HPUP_RED);
									used = TRUE;
								}
							}

							if (item->GetValue(1) != 0)	// SP 절대값 회복
							{
								if (GetSP() < GetMaxSP())
								{
									PointChange(POINT_SP, item->GetValue(1) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
									EffectPacket(SE_SPUP_BLUE);
									used = TRUE;
								}
							}

							if (item->GetValue(3) != 0) // HP % 회복
							{
								if (GetHP() < GetMaxHP())
								{
									PointChange(POINT_HP, item->GetValue(3) * GetMaxHP() / 100);
									EffectPacket(SE_HPUP_RED);
									used = TRUE;
								}
							}

							if (item->GetValue(4) != 0) // SP % 회복
							{
								if (GetSP() < GetMaxSP())
								{
									PointChange(POINT_SP, item->GetValue(4) * GetMaxSP() / 100);
									EffectPacket(SE_SPUP_BLUE);
									used = TRUE;
								}
							}

							if (used)
							{
								if (item->GetVnum() == 50085 || item->GetVnum() == 50086)
								{
									if (test_server)
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("월병 또는 종자 를 사용하였습니다"));
									SetUseSeedOrMoonBottleTime();
								}
								if (GetDungeon())
									GetDungeon()->UsePotion(this);

								if (GetWarMap())
									GetWarMap()->UsePotion(this, item);

								m_nPotionLimit--;

								//RESTRICT_USE_SEED_OR_MOONBOTTLE
								item->SetCount(item->GetCount() - 1);
								//END_RESTRICT_USE_SEED_OR_MOONBOTTLE
							}
						}
						break;

					case USE_POTION:
						if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
						{
							if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit") > 0)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련장에서 사용하실 수 없습니다."));
								return false;
							}
						
							switch (item->GetVnum())
							{
								case 27001 :
								case 27002 :
								case 27003 :
								case 27004 :
								case 27005 :
								case 27006 :
									if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit_count") < 10000)
									{
										if (m_nPotionLimit <= 0)
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("사용 제한량을 초과하였습니다."));
											return false;
										}
									}
									break;

								default :
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련장에서 사용하실 수 없습니다."));
									return false;
							}
						}
						
						if (item->GetValue(1) != 0)
						{
							if (GetPoint(POINT_SP_RECOVERY) + GetSP() >= GetMaxSP())
							{
								return false;
							}

							PointChange(POINT_SP_RECOVERY, item->GetValue(1) * MIN(200, (100 + GetPoint(POINT_POTION_BONUS))) / 100);
							StartAffectEvent();
							EffectPacket(SE_SPUP_BLUE);
						}

						if (item->GetValue(0) != 0)
						{
							if (GetPoint(POINT_HP_RECOVERY) + GetHP() >= GetMaxHP())
							{
								return false;
							}

							PointChange(POINT_HP_RECOVERY, item->GetValue(0) * MIN(200, (100 + GetPoint(POINT_POTION_BONUS))) / 100);
							StartAffectEvent();
							EffectPacket(SE_HPUP_RED);
						}

						if (GetDungeon())
							GetDungeon()->UsePotion(this);

						if (GetWarMap())
							GetWarMap()->UsePotion(this, item);

						//item->SetCount(item->GetCount() - 1);
						m_nPotionLimit--;
						break;

					case USE_POTION_CONTINUE:
						{
							if (item->GetValue(0) != 0)
							{
								AddAffect(AFFECT_HP_RECOVER_CONTINUE, POINT_HP_RECOVER_CONTINUE, item->GetValue(0), 0, item->GetValue(2), 0, true);
							}
							else if (item->GetValue(1) != 0)
							{
								AddAffect(AFFECT_SP_RECOVER_CONTINUE, POINT_SP_RECOVER_CONTINUE, item->GetValue(1), 0, item->GetValue(2), 0, true);
							}
							else
								return false;
						}

						if (GetDungeon())
							GetDungeon()->UsePotion(this);

						if (GetWarMap())
							GetWarMap()->UsePotion(this, item);

						item->SetCount(item->GetCount() - 1);
						break;

					case USE_ABILITY_UP:
						{
							switch (item->GetValue(0))
							{
								case APPLY_MOV_SPEED:
									AddAffect(AFFECT_MOV_SPEED, POINT_MOV_SPEED, item->GetValue(2), AFF_MOV_SPEED_POTION, item->GetValue(1), 0, true);
									break;

								case APPLY_ATT_SPEED:
									AddAffect(AFFECT_ATT_SPEED, POINT_ATT_SPEED, item->GetValue(2), AFF_ATT_SPEED_POTION, item->GetValue(1), 0, true);
									break;

								case APPLY_STR:
									AddAffect(AFFECT_STR, POINT_ST, item->GetValue(2), 0, item->GetValue(1), 0, true);
									break;

								case APPLY_DEX:
									AddAffect(AFFECT_DEX, POINT_DX, item->GetValue(2), 0, item->GetValue(1), 0, true);
									break;

								case APPLY_CON:
									AddAffect(AFFECT_CON, POINT_HT, item->GetValue(2), 0, item->GetValue(1), 0, true);
									break;

								case APPLY_INT:
									AddAffect(AFFECT_INT, POINT_IQ, item->GetValue(2), 0, item->GetValue(1), 0, true);
									break;

								case APPLY_CAST_SPEED:
									AddAffect(AFFECT_CAST_SPEED, POINT_CASTING_SPEED, item->GetValue(2), 0, item->GetValue(1), 0, true);
									break;

								case APPLY_ATT_GRADE_BONUS:
									AddAffect(AFFECT_ATT_GRADE, POINT_ATT_GRADE_BONUS, 
											item->GetValue(2), 0, item->GetValue(1), 0, true);
									break;

								case APPLY_DEF_GRADE_BONUS:
									AddAffect(AFFECT_DEF_GRADE, POINT_DEF_GRADE_BONUS,
											item->GetValue(2), 0, item->GetValue(1), 0, true);
									break;
							}
						}

						if (GetDungeon())
							GetDungeon()->UsePotion(this);

						if (GetWarMap())
							GetWarMap()->UsePotion(this, item);

						item->SetCount(item->GetCount() - 1);
						break;

					case USE_TALISMAN:
						{
							const int TOWN_PORTAL	= 1;
							const int MEMORY_PORTAL = 2;


							// gm_guild_build, oxevent 맵에서 귀환부 귀환기억부 를 사용못하게 막음
							if (GetMapIndex() == 200 || GetMapIndex() == 113)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("현재 위치에서 사용할 수 없습니다."));
								return false;
							}

							if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("대련 중에는 이용할 수 없는 물품입니다."));
								return false;
							}

							if (m_pkWarpEvent)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이동할 준비가 되어있음으로 귀환부를 사용할수 없습니다"));
								return false;
							}

							// CONSUME_LIFE_WHEN_USE_WARP_ITEM
							int consumeLife = CalculateConsume(this);
							if (consumeLife < 0)
								return false;
							// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM

							if (item->GetValue(0) == TOWN_PORTAL) // 귀환부
							{
								if (item->GetSocket(0) == 0)
								{
									if (!GetDungeon())
										if (!GiveRecallItem(item))
											return false;

									PIXEL_POSITION posWarp;

									if (SECTREE_MANAGER::instance().GetRecallPositionByEmpire(GetMapIndex(), GetEmpire(), posWarp))
									{
										// CONSUME_LIFE_WHEN_USE_WARP_ITEM
										PointChange(POINT_HP, -consumeLife, false);
										// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM

										WarpSet(posWarp.x, posWarp.y);
									}
									else
									{
										sys_err("CHARACTER::UseItem : cannot find spawn position (name %s, %d x %d)", GetName(), GetX(), GetY());
									}
								}
								else
								{
									if (test_server)
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("원래 위치로 복귀"));	

									ProcessRecallItem(item);
								}
							}
							else if (item->GetValue(0) == MEMORY_PORTAL) // 귀환기억부
							{
								if (item->GetSocket(0) == 0)
								{
									if (GetDungeon())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("던전 안에서는 %s%s 사용할 수 없습니다."),
												item->GetName(),
												g_iUseLocale ? "" : (under_han(item->GetName()) ? LC_TEXT("을") : LC_TEXT("를")));
										return false;
									}

									if (!GiveRecallItem(item))
										return false;
								}
								else
								{
									// CONSUME_LIFE_WHEN_USE_WARP_ITEM
									PointChange(POINT_HP, -consumeLife, false);
									// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM

									ProcessRecallItem(item);
								}
							}
						}
						break;

					case USE_TUNING:
					case USE_DETACHMENT:
						{
							LPITEM item2;

							if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
								return false;

							if (item2->IsExchanging() || item2->IsEquipped()) // @fixme114
								return false;

							if (item->GetValue(0) == RITUALS_SCROLL)
								RefineItem(item, item2);

#ifdef __SASH_SYSTEM__
							if (item->GetValue(0) == SASH_CLEAN_ATTR_VALUE0)
							{
								if (!CleanSashAttr(item, item2))
									return false;

								return true;
							}
#endif

#ifdef ENABLE_AURA_SYSTEM
							if (item->GetValue(0) == AURA_CLEAN_ATTR_VALUE0)
							{
								if (!CleanAuraAttr(item, item2))
									return false;

								return true;
							}
#endif

							RefineItem(item, item2);
						}
						break;

						//  ACCESSORY_REFINE & ADD/CHANGE_ATTRIBUTES
					case USE_PUT_INTO_BELT_SOCKET:
					case USE_PUT_INTO_RING_SOCKET:
					case USE_PUT_INTO_ACCESSORY_SOCKET:
					case USE_ADD_ACCESSORY_SOCKET:
					case USE_CLEAN_SOCKET:
					case USE_CHANGE_ATTRIBUTE:
					case USE_CHANGE_ATTRIBUTE2 :
					case USE_ADD_ATTRIBUTE:
					case USE_ADD_ATTRIBUTE2:
					case USE_ADD_ATTRIBUTE_ELEMENT:
					case USE_CHANGE_ATTRIBUTE_ELEMENT:
						{
							LPITEM item2;
							if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
								return false;

							if (item2->IsEquipped())
							{
								BuffOnAttr_RemoveBuffsFromItem(item2);
							}

							// [NOTE] 코스튬 아이템에는 아이템 최초 생성시 랜덤 속성을 부여하되, 재경재가 등등은 막아달라는 요청이 있었음.
							// 원래 ANTI_CHANGE_ATTRIBUTE 같은 아이템 Flag를 추가하여 기획 레벨에서 유연하게 컨트롤 할 수 있도록 할 예정이었으나
							// 그딴거 필요없으니 닥치고 빨리 해달래서 그냥 여기서 막음... -_-
							if (ITEM_COSTUME == item2->GetType())
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 변경할 수 없는 아이템입니다."));
								return false;
							}

							if (item2->IsExchanging() || item2->IsEquipped()) // @fixme114
								return false;

							switch (item->GetSubType())
							{
								case USE_CLEAN_SOCKET:
									{
										int i;
										for (i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
										{
											if (item2->GetSocket(i) == ITEM_BROKEN_METIN_VNUM)
												break;
										}

										if (i == ITEM_SOCKET_MAX_NUM)
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("청소할 석이 박혀있지 않습니다."));
											return false;
										}

										int j = 0;

										for (i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
										{
											if (item2->GetSocket(i) != ITEM_BROKEN_METIN_VNUM && item2->GetSocket(i) != 0)
												item2->SetSocket(j++, item2->GetSocket(i));
										}

										for (; j < ITEM_SOCKET_MAX_NUM; ++j)
										{
											if (item2->GetSocket(j) > 0)
												item2->SetSocket(j, 1);
										}

										{
											char buf[21];
											snprintf(buf, sizeof(buf), "%u", item2->GetID());
											LogManager::instance().ItemLog(this, item, "CLEAN_SOCKET", buf);
										}

										item->SetCount(item->GetCount() - 1);

									}
									break;

								case USE_CHANGE_ATTRIBUTE :
									if (item2->GetAttributeSetIndex() == -1)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 변경할 수 없는 아이템입니다."));
										return false;
									}

									if (item2->GetSubType() == ARMOR_ELEMENT)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 변경할 수 없는 아이템입니다."));
										return false;
									}
									if (item2->GetAttributeCount() == 0)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("변경할 속성이 없습니다."));
										return false;
									}

									if (GM_PLAYER == GetGMLevel() && false == test_server)
									{
										//
										// Event Flag 를 통해 이전에 아이템 속성 변경을 한 시간으로 부터 충분한 시간이 흘렀는지 검사하고
										// 시간이 충분히 흘렀다면 현재 속성변경에 대한 시간을 설정해 준다.
										//

										DWORD dwChangeItemAttrCycle = quest::CQuestManager::instance().GetEventFlag(msc_szChangeItemAttrCycleFlag);
										if (dwChangeItemAttrCycle < msc_dwDefaultChangeItemAttrCycle)
											dwChangeItemAttrCycle = msc_dwDefaultChangeItemAttrCycle;

										quest::PC* pPC = quest::CQuestManager::instance().GetPC(GetPlayerID());

										/*if (pPC)
										{
											DWORD dwNowMin = get_global_time() / 60;

											DWORD dwLastChangeItemAttrMin = pPC->GetFlag(msc_szLastChangeItemAttrFlag);

											if (dwLastChangeItemAttrMin + dwChangeItemAttrCycle > dwNowMin)
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 바꾼지 %d분 이내에는 다시 변경할 수 없습니다.(%d 분 남음)"),
														dwChangeItemAttrCycle, dwChangeItemAttrCycle - (dwNowMin - dwLastChangeItemAttrMin));
												return false;
											}

											pPC->SetFlag(msc_szLastChangeItemAttrFlag, dwNowMin);
										}*/
									}

									if (item->GetSubType() == USE_CHANGE_ATTRIBUTE2)
									{
										int aiChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] = 
										{
											0, 0, 30, 40, 3
										};

										item2->ChangeAttribute(aiChangeProb);
									}
									else if (item->GetVnum() == 76014)
									{
										int aiChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] = 
										{
											0, 10, 50, 39, 1
										};

										item2->ChangeAttributeAddon(aiChangeProb);
									}
									else if (item->GetVnum() == 11126)
									{
										if (item2->GetAttributeType(0) != 71 && item2->GetAttributeType(0) != 72)
										{
											ChatPacket(CHAT_TYPE_INFO, "Bu nesneyi sadece ortalama hasara sahip olabilecek esyalarda kullanabilirsin !");
											return false;
										}
										if (item2->GetAttributeType(0) != 72 && item2->GetAttributeType(1) != 71)
										{
											ChatPacket(CHAT_TYPE_INFO, "Esyanizda ortalama zarar ve beceri hasari efsunu yok bu sekilde bu esyayi kullanmak efsun kaybina sebep olur !");
											return false;
										}
										else
										{
											BYTE attrtype3 = item2->GetAttributeType(2);
											BYTE attrtype4 = item2->GetAttributeType(3);
											BYTE attrtype5 = item2->GetAttributeType(4);
											BYTE attrtype6 = item2->GetAttributeType(5);
											BYTE attrtype7 = item2->GetAttributeType(6);
										
											DWORD attrvalue1 = number(75, 100); // esyaya gelecebilecek ortalama zarar araligi min-max
											DWORD attrvalue2 = number(18,23); // - beceri hasari ayarligi min-max
											DWORD attrvalue3 = item2->GetAttributeValue(2);
											DWORD attrvalue4 = item2->GetAttributeValue(3);
											DWORD attrvalue5 = item2->GetAttributeValue(4);
											DWORD attrvalue6 = item2->GetAttributeValue(5);
											DWORD attrvalue7 = item2->GetAttributeValue(6);
											DWORD becerihasari = attrvalue2;
											becerihasari -= attrvalue2;
											becerihasari -= attrvalue2;

											item2->ClearAttribute();
											item2->SetForceAttribute(0,72,attrvalue1);
											item2->SetForceAttribute(1,71,becerihasari);
											item2->SetForceAttribute(2,attrtype3,attrvalue3);
											item2->SetForceAttribute(3,attrtype4,attrvalue4);
											item2->SetForceAttribute(4,attrtype5,attrvalue5);
											item2->SetForceAttribute(5,attrtype6,attrvalue6);
											item2->SetForceAttribute(6,attrtype7,attrvalue7);
										}
									}
									else if (item->GetVnum() == 11127)
									{
										if (item2->GetAttributeType(0) != 71 && item2->GetAttributeType(0) != 72)
										{
											ChatPacket(CHAT_TYPE_INFO, "Bu nesneyi sadece beceri hasara sahip olabilecek esyalarda kullanabilirsin !");
											return false;
										}
										if (item2->GetAttributeType(0) != 72 && item2->GetAttributeType(1) != 71)
										{
											ChatPacket(CHAT_TYPE_INFO, "Esyanizda ortalama zarar ve beceri hasari efsunu yok bu sekilde bu esyayi kullanmak efsun kaybina sebep olur !");
											return false;
										}
										else
										{

											BYTE attrtype3 = item2->GetAttributeType(2);
											BYTE attrtype4 = item2->GetAttributeType(3);
											BYTE attrtype5 = item2->GetAttributeType(4);
											BYTE attrtype6 = item2->GetAttributeType(5);
											BYTE attrtype7 = item2->GetAttributeType(6);

											DWORD attrvalue1 = number(20, 45); // esyaya gelecebilecek  - ortalama hasar araligi min-max
											DWORD attrvalue2 = number(25,35); // beceri hasari ayarligi min-max
											DWORD attrvalue3 = item2->GetAttributeValue(2);
											DWORD attrvalue4 = item2->GetAttributeValue(3);
											DWORD attrvalue5 = item2->GetAttributeValue(4);
											DWORD attrvalue6 = item2->GetAttributeValue(5);
											DWORD attrvalue7 = item2->GetAttributeValue(6);

										  

											DWORD ortalamazarar = attrvalue1;
											ortalamazarar -= attrvalue1;
											ortalamazarar -= attrvalue1;

											item2->ClearAttribute();
											item2->SetForceAttribute(0,72,ortalamazarar);
											item2->SetForceAttribute(1,71,attrvalue2);
											item2->SetForceAttribute(2,attrtype3,attrvalue3);
											item2->SetForceAttribute(3,attrtype4,attrvalue4);
											item2->SetForceAttribute(4,attrtype5,attrvalue5);
											item2->SetForceAttribute(5,attrtype6,attrvalue6);
											item2->SetForceAttribute(6,attrtype7,attrvalue7);
										}
									}

									else
									{
										// 연재경 특수처리
										// 절대로 연재가 추가 안될거라 하여 하드 코딩함.
										if (item->GetVnum() == 71151 || item->GetVnum() == 76023)
										{
											if ((item2->GetType() == ITEM_WEAPON)
												|| (item2->GetType() == ITEM_ARMOR))
											{
												bool bCanUse = true;
												for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
												{
													if (item2->GetLimitType(i) == LIMIT_LEVEL && item2->GetLimitValue(i) > 20)
													{
														bCanUse = false;
														break;
													}
												}
												if (false == bCanUse)
												{
													ChatPacket(CHAT_TYPE_INFO, LC_TEXT("적용 레벨보다 높아 사용이 불가능합니다."));
													break;
												}
											}
											else
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("무기와 갑옷에만 사용 가능합니다."));
												break;
											}
										}
										item2->ChangeAttribute();
									}

									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 변경하였습니다."));
									{
										char buf[21];
										snprintf(buf, sizeof(buf), "%u", item2->GetID());
										LogManager::instance().ItemLog(this, item, "CHANGE_ATTRIBUTE", buf);
									}

									item->SetCount(item->GetCount() - 1);
									break;

								case USE_ADD_ATTRIBUTE :
									if (item2->GetSubType() == ARMOR_ELEMENT)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 변경할 수 없는 아이템입니다."));
										return false;
									}
									if (item2->GetAttributeSetIndex() == -1)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 변경할 수 없는 아이템입니다."));
										return false;
									}
									
									if (item2->IsEquipped())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("giyiliitemeatamazsin"));
										return false;
									}
									
									
									if (item2->GetAttributeCount() < 4)
									{
										// 연재가 특수처리
										// 절대로 연재가 추가 안될거라 하여 하드 코딩함.
										if (item->GetVnum() == 71152 || item->GetVnum() == 76024)
										{
											if ((item2->GetType() == ITEM_WEAPON)
												|| (item2->GetType() == ITEM_ARMOR && item2->GetSubType() == ARMOR_BODY))
											{
												bool bCanUse = true;
												for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
												{
													if (item2->GetLimitType(i) == LIMIT_LEVEL && item2->GetLimitValue(i) > 20)
													{
														bCanUse = false;
														break;
													}
												}
												if (false == bCanUse)
												{
													ChatPacket(CHAT_TYPE_INFO, LC_TEXT("적용 레벨보다 높아 사용이 불가능합니다."));
													break;
												}
											}
											else
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("무기와 갑옷에만 사용 가능합니다."));
												break;
											}
										}
										char buf[21];
										snprintf(buf, sizeof(buf), "%u", item2->GetID());

										if (number(1, 100) <= aiItemAttributeAddPercent[item2->GetAttributeCount()])
										{
											item2->AddAttribute();
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성 추가에 성공하였습니다."));

											int iAddedIdx = item2->GetAttributeCount() - 1;
											LogManager::instance().ItemLog(
													GetPlayerID(), 
													item2->GetAttributeType(iAddedIdx),
													item2->GetAttributeValue(iAddedIdx),
													item->GetID(), 
													"ADD_ATTRIBUTE_SUCCESS",
													buf,
													GetDesc()->GetHostName(),
													item->GetOriginalVnum());
										}
										else
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성 추가에 실패하였습니다."));
											LogManager::instance().ItemLog(this, item, "ADD_ATTRIBUTE_FAIL", buf);
										}

										item->SetCount(item->GetCount() - 1);
									}
									else
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("더이상 이 아이템을 이용하여 속성을 추가할 수 없습니다."));
									}
									break;

								case USE_ADD_ATTRIBUTE2 :
									// 축복의 구슬 
									// 재가비서를 통해 속성을 4개 추가 시킨 아이템에 대해서 하나의 속성을 더 붙여준다.
									if (item2->IsEquipped())
									{
										ChatPacket(CHAT_TYPE_INFO, "Hata!");
										return false;
									}
									if (item2->GetAttributeSetIndex() == -1)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 변경할 수 없는 아이템입니다."));
										return false;
									}

									// 속성이 이미 4개 추가 되었을 때만 속성을 추가 가능하다.
									if (item2->GetAttributeCount() == 4)
									{
										char buf[21];
										snprintf(buf, sizeof(buf), "%u", item2->GetID());

										if (number(1, 100) <= aiItemAttributeAddPercent[item2->GetAttributeCount()])
										{
											item2->AddAttribute();
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성 추가에 성공하였습니다."));

											int iAddedIdx = item2->GetAttributeCount() - 1;
											LogManager::instance().ItemLog(
													GetPlayerID(),
													item2->GetAttributeType(iAddedIdx),
													item2->GetAttributeValue(iAddedIdx),
													item->GetID(),
													"ADD_ATTRIBUTE2_SUCCESS",
													buf,
													GetDesc()->GetHostName(),
													item->GetOriginalVnum());
										}
										else
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성 추가에 실패하였습니다."));
											LogManager::instance().ItemLog(this, item, "ADD_ATTRIBUTE2_FAIL", buf);
										}

									item->SetCount(item->GetCount() - 1);
									}
									else if (item2->GetAttributeCount() == 5)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("더 이상 이 아이템을 이용하여 속성을 추가할 수 없습니다."));
									}
									else if (item2->GetAttributeCount() < 4)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("먼저 재가비서를 이용하여 속성을 추가시켜 주세요."));
									}
									else
									{
										// wtf ?!
										sys_err("ADD_ATTRIBUTE2 : Item has wrong AttributeCount(%d)", item2->GetAttributeCount());
									}
									break;
								case USE_ADD_ATTRIBUTE_ELEMENT :
#ifdef ENABLE_SOULBIND_SYSTEM
									if(item2->IsSealed()){
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Can't change attr an sealbind item."));
										return false;
									}
#endif
									if (item2->GetAttributeSetIndex() == -1)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 변경할 수 없는 아이템입니다."));
										return false;
									}	
									if (item2->GetAttributeCount() < 4)
									{
										// 연재가 특수처리
										// 절대로 연재가 추가 안될거라 하여 하드 코딩함.
										if (item->GetVnum() == 71152 || item->GetVnum() == 76024)
										{
											if ((item2->GetType() == ITEM_WEAPON)
												|| (item2->GetType() == ITEM_ARMOR && item2->GetSubType() == ARMOR_BODY))
											{
												bool bCanUse = true;
												for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
												{
													if (item2->GetLimitType(i) == LIMIT_LEVEL && item2->GetLimitValue(i) > 20)
													{
														bCanUse = false;
														break;
													}
												}
												if (false == bCanUse)
												{
													ChatPacket(CHAT_TYPE_INFO, LC_TEXT("적용 레벨보다 높아 사용이 불가능합니다."));
													break;
												}
											}
											else
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("무기와 갑옷에만 사용 가능합니다."));
												break;
											}
										}
										char buf[21];
										snprintf(buf, sizeof(buf), "%u", item2->GetID());

										if (number(1, 100) <= aiItemAttributeAddPercent[item2->GetAttributeCount()])
										{
											item2->AddAttribute();
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성 추가에 성공하였습니다."));

											int iAddedIdx = item2->GetAttributeCount() - 1;
											LogManager::instance().ItemLog(
													GetPlayerID(), 
													item2->GetAttributeType(iAddedIdx),
													item2->GetAttributeValue(iAddedIdx),
													item->GetID(), 
													"ADD_ATTRIBUTE_SUCCESS",
													buf,
													GetDesc()->GetHostName(),
													item->GetOriginalVnum());
										}
										else
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성 추가에 실패하였습니다."));
											LogManager::instance().ItemLog(this, item, "ADD_ATTRIBUTE_FAIL", buf);
										}

										item->SetCount(item->GetCount() - 1);
									}
									else
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("더이상 이 아이템을 이용하여 속성을 추가할 수 없습니다."));
									}
									break;

								case USE_CHANGE_ATTRIBUTE_ELEMENT :
								
								
#ifdef ENABLE_SOULBIND_SYSTEM
									if(item2->IsSealed()){
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Can't change attr an sealbind item."));
										return false;
									}
#endif
									if (item2->GetAttributeSetIndex() == -1)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 변경할 수 없는 아이템입니다."));
										return false;
									}

									if (item2->GetAttributeCount() == 0)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("변경할 속성이 없습니다."));
										return false;
									}

									if (GM_PLAYER == GetGMLevel() && false == test_server)
									{
										//
										// Event Flag 를 통해 이전에 아이템 속성 변경을 한 시간으로 부터 충분한 시간이 흘렀는지 검사하고
										// 시간이 충분히 흘렀다면 현재 속성변경에 대한 시간을 설정해 준다.
										//

										DWORD dwChangeItemAttrCycle = quest::CQuestManager::instance().GetEventFlag(msc_szChangeItemAttrCycleFlag);
										if (dwChangeItemAttrCycle < msc_dwDefaultChangeItemAttrCycle)
											dwChangeItemAttrCycle = msc_dwDefaultChangeItemAttrCycle;

										quest::PC* pPC = quest::CQuestManager::instance().GetPC(GetPlayerID());

										/*if (pPC)
										{
											DWORD dwNowMin = get_global_time() / 60;

											DWORD dwLastChangeItemAttrMin = pPC->GetFlag(msc_szLastChangeItemAttrFlag);

											if (dwLastChangeItemAttrMin + dwChangeItemAttrCycle > dwNowMin)
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 바꾼지 %d분 이내에는 다시 변경할 수 없습니다.(%d 분 남음)"),
														dwChangeItemAttrCycle, dwChangeItemAttrCycle - (dwNowMin - dwLastChangeItemAttrMin));
												return false;
											}

											pPC->SetFlag(msc_szLastChangeItemAttrFlag, dwNowMin);
										}*/
									}

									if (item->GetSubType() == USE_CHANGE_ATTRIBUTE2)
									{
										int aiChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] = 
										{
											0, 0, 30, 40, 3
										};

										item2->ChangeAttribute(aiChangeProb);
									}
									else if (item->GetVnum() == 76014)
									{
										int aiChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] = 
										{
											0, 10, 50, 39, 1
										};

										item2->ChangeAttribute(aiChangeProb);
									}

									else
									{
										// 연재경 특수처리
										// 절대로 연재가 추가 안될거라 하여 하드 코딩함.
										if (item->GetVnum() == 71151 || item->GetVnum() == 76023)
										{
											if ((item2->GetType() == ITEM_WEAPON)
												|| (item2->GetType() == ITEM_ARMOR))
											{
												bool bCanUse = true;
												for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
												{
													if (item2->GetLimitType(i) == LIMIT_LEVEL && item2->GetLimitValue(i) > 20)
													{
														bCanUse = false;
														break;
													}
												}
												if (false == bCanUse)
												{
													ChatPacket(CHAT_TYPE_INFO, LC_TEXT("적용 레벨보다 높아 사용이 불가능합니다."));
													break;
												}
											}
											else
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("무기와 갑옷에만 사용 가능합니다."));
												break;
											}
										}
										item2->ChangeAttribute();
									}

									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 변경하였습니다."));
									{
										char buf[21];
										snprintf(buf, sizeof(buf), "%u", item2->GetID());
										LogManager::instance().ItemLog(this, item, "CHANGE_ATTRIBUTE", buf);
									}

									item->SetCount(item->GetCount() - 1);
									break;
								case USE_ADD_ACCESSORY_SOCKET:
									{
										char buf[21];
										snprintf(buf, sizeof(buf), "%u", item2->GetID());

										if (item2->IsAccessoryForSocket())
										{
											if (item2->GetAccessorySocketMaxGrade() < ITEM_ACCESSORY_SOCKET_MAX_NUM)
											{
												if (number(1, 100) <= 100)
												{
													item2->SetAccessorySocketMaxGrade(item2->GetAccessorySocketMaxGrade() + 1);
													ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소켓이 성공적으로 추가되었습니다."));
													LogManager::instance().ItemLog(this, item, "ADD_SOCKET_SUCCESS", buf);
												}
												else
												{
													ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소켓 추가에 실패하였습니다."));
													LogManager::instance().ItemLog(this, item, "ADD_SOCKET_FAIL", buf);
												}

												item->SetCount(item->GetCount() - 1);
											}
											else
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 액세서리에는 더이상 소켓을 추가할 공간이 없습니다."));
											}
										}
										else
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 아이템으로 소켓을 추가할 수 없는 아이템입니다."));
										}
									}
									break;

								case USE_PUT_INTO_BELT_SOCKET:
								case USE_PUT_INTO_ACCESSORY_SOCKET:
									// if (item2->IsAccessoryForSocket() && item->CanPutInto(item2))
									if (item2->IsAccessoryForSocket())
									{
										
										if (item->PermaCevher())
										{
											if (item->CanPutInto2(item2) == false)
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 아이템을 장착할 수 없습니다."));
												break;
											}
											if (item2->GetAccessorySocketGrade() > 0 && item2->GetSocket(4) != 999)
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("surelicevhervar"));
												break;
											}
										}
										else
										{
											if (item->CanPutInto(item2) == false)
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 아이템을 장착할 수 없습니다."));
												break;
											}
											if (item2->GetSocket(4) == 999)
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("permecevhervar"));
												break;
											}
										}
										
										char buf[21];
										snprintf(buf, sizeof(buf), "%u", item2->GetID());
										if (item2->GetAccessorySocketGrade() < item2->GetAccessorySocketMaxGrade())
										{
											// if (number(1, 100) <= aiAccessorySocketPutPct[item2->GetAccessorySocketGrade()])
											if (number(1, 100) <= 1000)
											{
												item2->SetAccessorySocketGrade(item2->GetAccessorySocketGrade() + 1);
												if (item->PermaCevher())
												{
													item2->SetSocket(4,999);
												}
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("장착에 성공하였습니다."));
												LogManager::instance().ItemLog(this, item, "PUT_SOCKET_SUCCESS", buf);
											}
											else
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("장착에 실패하였습니다."));
												LogManager::instance().ItemLog(this, item, "PUT_SOCKET_FAIL", buf);
											}

											item->SetCount(item->GetCount() - 1);
										}
										else
										{
											if (item2->GetAccessorySocketMaxGrade() == 0)
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("먼저 다이아몬드로 악세서리에 소켓을 추가해야합니다."));
											else if (item2->GetAccessorySocketMaxGrade() < ITEM_ACCESSORY_SOCKET_MAX_NUM)
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 액세서리에는 더이상 장착할 소켓이 없습니다."));
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("다이아몬드로 소켓을 추가해야합니다."));
											}
											else
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 액세서리에는 더이상 보석을 장착할 수 없습니다."));
										}
									}
									else
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 아이템을 장착할 수 없습니다."));
									}
									break;
							}
							if (item2->IsEquipped())
							{
								BuffOnAttr_AddBuffsFromItem(item2);
							}
						}
						break;
						//  END_OF_ACCESSORY_REFINE & END_OF_ADD_ATTRIBUTES & END_OF_CHANGE_ATTRIBUTES

						case USE_ADD_ATTRIBUTE_MOUNT :
						
								LPITEM item2;
								if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
									return false;

								if (item2->IsEquipped())
								{
									BuffOnAttr_RemoveBuffsFromItem(item2);
								}
#ifdef ENABLE_SOULBIND_SYSTEM
								if(item2->IsSealed()){
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Can't change attr an sealbind item."));
									return false;
								}
#endif


								if (item2->IsExchanging() || item2->IsEquipped()) // @fixme114
									return false;
									
									if (!item2->IsAttrMount())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("not mount item"));
										return false;
									}
										
									if (item2->GetAttributeSetIndex() == -1)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 변경할 수 없는 아이템입니다."));
										return false;
									}	
									if (item2->GetAttributeCount() < 5)
									{
										// 연재가 특수처리
										// 절대로 연재가 추가 안될거라 하여 하드 코딩함.
										char buf[21];
										snprintf(buf, sizeof(buf), "%u", item2->GetID());

										if (number(1, 100) <= aiItemAttributeAddPercent[item2->GetAttributeCount()])
										{
											item2->AddAttribute();
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성 추가에 성공하였습니다."));

											int iAddedIdx = item2->GetAttributeCount() - 1;
											LogManager::instance().ItemLog(
													GetPlayerID(), 
													item2->GetAttributeType(iAddedIdx),
													item2->GetAttributeValue(iAddedIdx),
													item->GetID(), 
													"ADD_ATTRIBUTE_SUCCESS",
													buf,
													GetDesc()->GetHostName(),
													item->GetOriginalVnum());
										}
										else
										{
											ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성 추가에 실패하였습니다."));
											LogManager::instance().ItemLog(this, item, "ADD_ATTRIBUTE_FAIL", buf);
										}

										item->SetCount(item->GetCount() - 1);
									}
									else
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("더이상 이 아이템을 이용하여 속성을 추가할 수 없습니다."));
									}
									break;

							case USE_CHANGE_ATTRIBUTE_MOUNT :
								
								if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
									return false;

								if (item2->IsEquipped())
								{
									BuffOnAttr_RemoveBuffsFromItem(item2);
								}
#ifdef ENABLE_SOULBIND_SYSTEM
								if(item2->IsSealed()){
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Can't change attr an sealbind item."));
									return false;
								}
#endif


								if (item2->IsExchanging() || item2->IsEquipped()) // @fixme114
									return false;	
									
									if (!item2->IsAttrMount())
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("not mount item"));
										return false;
									}
									
									if (item2->GetAttributeSetIndex() == -1)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 변경할 수 없는 아이템입니다."));
										return false;
									}

									if (item2->GetAttributeCount() == 0)
									{
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("변경할 속성이 없습니다."));
										return false;
									}

									if (GM_PLAYER == GetGMLevel() && false == test_server)
									{
										//
										// Event Flag 를 통해 이전에 아이템 속성 변경을 한 시간으로 부터 충분한 시간이 흘렀는지 검사하고
										// 시간이 충분히 흘렀다면 현재 속성변경에 대한 시간을 설정해 준다.
										//

										DWORD dwChangeItemAttrCycle = quest::CQuestManager::instance().GetEventFlag(msc_szChangeItemAttrCycleFlag);
										if (dwChangeItemAttrCycle < msc_dwDefaultChangeItemAttrCycle)
											dwChangeItemAttrCycle = msc_dwDefaultChangeItemAttrCycle;

										quest::PC* pPC = quest::CQuestManager::instance().GetPC(GetPlayerID());

										/*if (pPC)
										{
											DWORD dwNowMin = get_global_time() / 60;

											DWORD dwLastChangeItemAttrMin = pPC->GetFlag(msc_szLastChangeItemAttrFlag);

											if (dwLastChangeItemAttrMin + dwChangeItemAttrCycle > dwNowMin)
											{
												ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 바꾼지 %d분 이내에는 다시 변경할 수 없습니다.(%d 분 남음)"),
														dwChangeItemAttrCycle, dwChangeItemAttrCycle - (dwNowMin - dwLastChangeItemAttrMin));
												return false;
											}

											pPC->SetFlag(msc_szLastChangeItemAttrFlag, dwNowMin);
										}*/
									}
										// 연재경 특수처리
										// 절대로 연재가 추가 안될거라 하여 하드 코딩함.
										item2->ChangeAttribute();

									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("속성을 변경하였습니다."));
									{
										char buf[21];
										snprintf(buf, sizeof(buf), "%u", item2->GetID());
										LogManager::instance().ItemLog(this, item, "CHANGE_ATTRIBUTE", buf);
									}

									item->SetCount(item->GetCount() - 1);
									break;
						
#ifdef __COSTUME_ATTR_SYSTEM__
					case USE_COSTUME_ENCHANT:
					case USE_COSTUME_TRANSFORM:
						{
							LPITEM item2;
							if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
								return false;
							
							if (item2->IsEquipped())
							{
								BuffOnAttr_RemoveBuffsFromItem(item2);
							}
							
							if (item2->IsExchanging() || item2->IsEquipped())
								return false;
							
							if (item2->GetType() != ITEM_COSTUME || item2->GetSubType() == COSTUME_SASH)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can use this just on costumes."));
								return false;
							}
							
							if (item2->GetAttributeCount() == 0)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This costume doesn't have any bonus."));
								return false;
							}
							
							switch (item->GetSubType())
							{
								case USE_COSTUME_ENCHANT:
									{
										item2->ChangeAttribute();
										item->SetCount(item->GetCount() - 1);
									}
									break;
								case USE_COSTUME_TRANSFORM:
									{
										item2->ClearAttribute();
										item2->AlterToMagicItem(number(40,50), number(10,15));
										item->SetCount(item->GetCount() - 1);
									}
									break;
							}
						}
						break;
#endif
					case USE_BAIT:
						{

							if (m_pkFishingEvent)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("낚시 중에 미끼를 갈아끼울 수 없습니다."));
								return false;
							}

							LPITEM weapon = GetWear(WEAR_WEAPON);

							if (!weapon || weapon->GetType() != ITEM_ROD)
								return false;

							if (weapon->GetSocket(2))
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 꽂혀있던 미끼를 빼고 %s를 끼웁니다."), item->GetName());
							}
							else
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("낚시대에 %s를 미끼로 끼웁니다."), item->GetName());
							}

							weapon->SetSocket(2, item->GetValue(0));
							item->SetCount(item->GetCount() - 1);
						}
						break;

					case USE_MOVE:
					case USE_TREASURE_BOX:
					case USE_MONEYBAG:
						break;

					case USE_AFFECT :
						{
							if (FindAffect(item->GetValue(0), aApplyInfo[item->GetValue(1)].bPointType))
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 효과가 걸려 있습니다."));
							}
							else
							{
								// PC_BANG_ITEM_ADD
								if (item->IsPCBangItem() == true)
								{
									// PC방인지 체크해서 처리
									if (CPCBangManager::instance().IsPCBangIP(GetDesc()->GetHostName()) == false)
									{
										// PC방이 아님!
										ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 아이템은 PC방에서만 사용할 수 있습니다."));
										return false;
									}
								}
								// END_PC_BANG_ITEM_ADD

								AddAffect(item->GetValue(0), aApplyInfo[item->GetValue(1)].bPointType, item->GetValue(2), 0, item->GetValue(3), 0, false);
								item->SetCount(item->GetCount() - 1);
							}
						}
						break;

					case USE_CREATE_STONE:
						AutoGiveItem(number(28000, 28013));
						item->SetCount(item->GetCount() - 1);
						break;

					// 물약 제조 스킬용 레시피 처리	
					case USE_RECIPE :
						{
							LPITEM pSource1 = FindSpecifyItem(item->GetValue(1));
							DWORD dwSourceCount1 = item->GetValue(2);

							LPITEM pSource2 = FindSpecifyItem(item->GetValue(3));
							DWORD dwSourceCount2 = item->GetValue(4);

							if (dwSourceCount1 != 0)
							{
								if (pSource1 == NULL)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("물약 조합을 위한 재료가 부족합니다."));
									return false;
								}
							}

							if (dwSourceCount2 != 0)
							{
								if (pSource2 == NULL)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("물약 조합을 위한 재료가 부족합니다."));
									return false;
								}
							}

							if (pSource1 != NULL)
							{
								if (pSource1->GetCount() < dwSourceCount1)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("재료(%s)가 부족합니다."), pSource1->GetName());
									return false;
								}

								pSource1->SetCount(pSource1->GetCount() - dwSourceCount1);
							}

							if (pSource2 != NULL)
							{
								if (pSource2->GetCount() < dwSourceCount2)
								{
									ChatPacket(CHAT_TYPE_INFO, LC_TEXT("재료(%s)가 부족합니다."), pSource2->GetName());
									return false;
								}

								pSource2->SetCount(pSource2->GetCount() - dwSourceCount2);
							}

							LPITEM pBottle = FindSpecifyItem(50901);

							if (!pBottle || pBottle->GetCount() < 1)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("빈 병이 모자릅니다."));
								return false;
							}

							pBottle->SetCount(pBottle->GetCount() - 1);

							if (number(1, 100) > item->GetValue(5))
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("물약 제조에 실패했습니다."));
								return false;
							}

							AutoGiveItem(item->GetValue(0));
						}
						break;
				}
			}
			break;

		case ITEM_METIN:
			{
				LPITEM item2;

				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->IsExchanging() || item2->IsEquipped()) // @fixme114
					return false;

				if (item2->GetType() == ITEM_PICK) return false;
				if (item2->GetType() == ITEM_ROD) return false;

				int i;

				for (i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
				{
					DWORD dwVnum;   

					if ((dwVnum = item2->GetSocket(i)) <= 2)
						continue;

					TItemTable * p = ITEM_MANAGER::instance().GetTable(dwVnum);

					if (!p)
						continue;

					if (item->GetValue(5) == p->alValues[5])
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("같은 종류의 메틴석은 여러개 부착할 수 없습니다."));
						return false;
					}
				}

				if (item2->GetType() == ITEM_ARMOR)
				{
					if (!IS_SET(item->GetWearFlag(), WEARABLE_BODY) || !IS_SET(item2->GetWearFlag(), WEARABLE_BODY))
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 메틴석은 장비에 부착할 수 없습니다."));
						return false;
					}
				}
				else if (item2->GetType() == ITEM_WEAPON)
				{
					if (!IS_SET(item->GetWearFlag(), WEARABLE_WEAPON))
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 메틴석은 무기에 부착할 수 없습니다."));
						return false;
					}
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("부착할 수 있는 슬롯이 없습니다."));
					return false;
				}

				for (i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
					if (item2->GetSocket(i) >= 1 && item2->GetSocket(i) <= 2 && item2->GetSocket(i) >= item->GetValue(2))
					{
						// 석 확률
						if (number(1, 100) <= 100)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("메틴석 부착에 성공하였습니다."));
							item2->SetSocket(i, item->GetVnum());
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("메틴석 부착에 실패하였습니다."));
							item2->SetSocket(i, ITEM_BROKEN_METIN_VNUM);
						}

						LogManager::instance().ItemLog(this, item2, "SOCKET", item->GetName());
						//ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (METIN)");
						item->SetCount(item->GetCount() - 1);
						break;
					}

				if (i == ITEM_SOCKET_MAX_NUM)
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("부착할 수 있는 슬롯이 없습니다."));
			}
			break;

		case ITEM_AUTOUSE:
		case ITEM_MATERIAL:
		case ITEM_SPECIAL:
		case ITEM_TOOL:
		case ITEM_LOTTERY:
			break;

		case ITEM_TOTEM:
			{
				if (!item->IsEquipped())
					EquipItem(item);
			}
			break;

		case ITEM_BLEND:
			// 새로운 약초들
#ifdef ENABLE_YMIR_AFFECT_FIX
                           if ((CheckTimeUsed(item) == false))    {    return false;    }
#endif
			sys_log(0,"ITEM_BLEND!!");
			if (Blend_Item_find(item->GetVnum()))
			{
				int		affect_type		= AFFECT_BLEND;
				if (item->GetSocket(0) >= _countof(aApplyInfo))
				{
					sys_err ("INVALID BLEND ITEM(id : %d, vnum : %d). APPLY TYPE IS %d.", item->GetID(), item->GetVnum(), item->GetSocket(0));
					return false;
				}
				int		apply_type		= aApplyInfo[item->GetSocket(0)].bPointType;
				int		apply_value		= item->GetSocket(1);
				int		apply_duration	= item->GetSocket(2);
				
				if (FindAffect(affect_type, apply_type))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 효과가 걸려 있습니다."));
				}
				else
				{
					if (FindAffect(AFFECT_EXP_BONUS_EURO_FREE, POINT_RESIST_MAGIC))
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 효과가 걸려 있습니다."));
					}
					else
					{
						AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, false);
						item->SetCount(item->GetCount() - 1);
					}
				}
			}
			break;
		case ITEM_EXTRACT:
			{
				LPITEM pDestItem = GetItem(DestCell);
				if (NULL == pDestItem)
				{
					return false;
				}
				switch (item->GetSubType())
				{
				case EXTRACT_DRAGON_SOUL:
					if (pDestItem->IsDragonSoul())
					{
						return DSManager::instance().PullOut(this, NPOS, pDestItem, item);
					}
					return false;
				case EXTRACT_DRAGON_HEART:
					if (item->GetVnum() == 71053)
					{
						if (pDestItem->IsDragonSoul())
						{
							if (DSManager::instance().IsActiveDragonSoul(pDestItem) == true)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SIMYA_GIYILIYOR"));
								return false;
							}

							pDestItem->SetForceAttribute(0, 0, 0);
							pDestItem->SetForceAttribute(1, 0, 0);
							pDestItem->SetForceAttribute(2, 0, 0);
							pDestItem->SetForceAttribute(3, 0, 0);
							pDestItem->SetForceAttribute(4, 0, 0);
							pDestItem->SetForceAttribute(5, 0, 0);
							pDestItem->SetForceAttribute(6, 0, 0);
							
							bool ret = DSManager::instance().PutAttributes(pDestItem);
							if (ret == true)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SIMYA_DEGISTIRME_BASARILI!"));
								item->SetCount(item->GetCount()-1);
							}
							else
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("SIMYA_DEGISTIRME_BASARISIZ!"));
							}
						}
					}
					else
					{
						if (pDestItem->IsDragonSoul())
						{
							return DSManager::instance().ExtractDragonHeart(this, pDestItem, item);
						}
					}
					return false;
				default:
					return false;
				}
			}
			break;

		case ITEM_NONE:
			sys_err("Item type NONE %s", item->GetName());
			break;

		default:
			sys_log(0, "UseItemEx: Unknown type %s %d", item->GetName(), item->GetType());
			return false;
	}

	return true;
}

int g_nPortalLimitTime = 10;

bool CHARACTER::UseItem(TItemPos Cell, TItemPos DestCell, bool bUseAll)
{
	WORD wCell = Cell.cell;
	BYTE window_type = Cell.window_type;
	WORD wDestCell = DestCell.cell;
	BYTE bDestInven = DestCell.window_type;
	LPITEM item;

	if (!CanHandleItem())
		return false;

	if (!IsValidItemPosition(Cell) || !(item = GetItem(Cell)))
			return false;

	if (item->GetType() == ITEM_BELT && CBeltInventoryHelper::IsExistItemInBeltInventory(this))
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이는에러 메시지 이다"));
	sys_log(0, "%s: USE_ITEM %s (inven %d, cell: %d)", GetName(), item->GetName(), window_type, wCell);

	if (item->IsExchanging())
		return false;
		
#ifdef ENABLE_SWITCHBOT
	if (Cell.IsSwitchbotPosition())
	{
		CSwitchbot* pkSwitchbot = CSwitchbotManager::Instance().FindSwitchbot(GetPlayerID());
		if (pkSwitchbot && pkSwitchbot->IsActive(Cell.cell))
		{
			return false;
		}

		int iEmptyCell = GetEmptyInventory(item->GetSize());

		if (iEmptyCell == -1)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Cannot remove item from switchbot. Inventory is full."));
			return false;
		}

		MoveItem(Cell, TItemPos(INVENTORY, iEmptyCell), item->GetCount());
		return true;
	}
#endif

	if (!item->CanUsedBy(this))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("군직이 맞지않아 이 아이템을 사용할 수 없습니다."));
		return false;
	}

	if (IsStun())
		return false;

	if (false == FN_check_item_sex(this, item))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("성별이 맞지않아 이 아이템을 사용할 수 없습니다."));
		return false;
	}

	//PREVENT_TRADE_WINDOW
	if (IS_SUMMON_ITEM(item->GetVnum()))
	{
		if (false == IS_SUMMONABLE_ZONE(GetMapIndex()))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("사용할수 없습니다."));
			return false;
		}

		// 경혼반지 사용지 상대방이 SUMMONABLE_ZONE에 있는가는 WarpToPC()에서 체크
		
		//삼거리 관려 맵에서는 귀환부를 막아버린다.
		if (CThreeWayWar::instance().IsThreeWayWarMapIndex(GetMapIndex()))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("삼거리 전투 참가중에는 귀환부,귀환기억부를 사용할수 없습니다."));
			return false;
		}
		int iPulse = thecore_pulse();

		//창고 연후 체크
		if (iPulse - GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("창고를 연후 %d초 이내에는 귀환부,귀환기억부를 사용할 수 없습니다."), g_nPortalLimitTime);

			if (test_server)
				ChatPacket(CHAT_TYPE_INFO, "[TestOnly]Pulse %d LoadTime %d PASS %d", iPulse, GetSafeboxLoadTime(), PASSES_PER_SEC(g_nPortalLimitTime));
			return false; 
		}

		//거래관련 창 체크
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
		if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || GetOfflineShopOwner())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("거래창,창고 등을 연 상태에서는 귀환부,귀환기억부 를 사용할수 없습니다."));
			return false;
		}
#else
		if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("거래창,창고 등을 연 상태에서는 귀환부,귀환기억부 를 사용할수 없습니다."));
			return false;
		}
#endif

		//PREVENT_REFINE_HACK
		//개량후 시간체크 
		{
			if (iPulse - GetRefineTime() < PASSES_PER_SEC(g_nPortalLimitTime))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 개량후 %d초 이내에는 귀환부,귀환기억부를 사용할 수 없습니다."), g_nPortalLimitTime);
				return false;
			}
		}
		//END_PREVENT_REFINE_HACK
		

		//PREVENT_ITEM_COPY
		{
			if (iPulse - GetMyShopTime() < PASSES_PER_SEC(g_nPortalLimitTime))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("개인상점 사용후 %d초 이내에는 귀환부,귀환기억부를 사용할 수 없습니다."), g_nPortalLimitTime);
				return false;
			}
			
			if (iPulse - GetMyOfflineShopTime() < PASSES_PER_SEC(g_nPortalLimitTime))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("개인상점 사용후 %d초 이내에는 귀환부,귀환기억부를 사용할 수 없습니다."), g_nPortalLimitTime);
				return false;
			}
		}
		//END_PREVENT_ITEM_COPY
		

		//귀환부 거리체크
		if (item->GetVnum() != 70302)
		{
			PIXEL_POSITION posWarp;

			int x = 0;
			int y = 0;

			double nDist = 0;
			const double nDistant = 5000.0;
			//귀환기억부 
			if (item->GetVnum() == 22010)
			{
				x = item->GetSocket(0) - GetX();
				y = item->GetSocket(1) - GetY();
			}
			//귀환부
			else if (item->GetVnum() == 22000) 
			{
				SECTREE_MANAGER::instance().GetRecallPositionByEmpire(GetMapIndex(), GetEmpire(), posWarp);

				if (item->GetSocket(0) == 0)
				{
					x = posWarp.x - GetX();
					y = posWarp.y - GetY();
				}
				else
				{
					x = item->GetSocket(0) - GetX();
					y = item->GetSocket(1) - GetY();
				}
			}

			nDist = sqrt(pow((float)x,2) + pow((float)y,2));

			if (nDistant > nDist)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이동 되어질 위치와 너무 가까워 귀환부를 사용할수 없습니다."));				
				if (test_server)
					ChatPacket(CHAT_TYPE_INFO, "PossibleDistant %f nNowDist %f", nDistant,nDist); 
				return false;
			}
		}

		//PREVENT_PORTAL_AFTER_EXCHANGE
		//교환 후 시간체크
		if (iPulse - GetExchangeTime()  < PASSES_PER_SEC(g_nPortalLimitTime))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("거래 후 %d초 이내에는 귀환부,귀환기억부등을 사용할 수 없습니다."), g_nPortalLimitTime);
			return false;
		}
		//END_PREVENT_PORTAL_AFTER_EXCHANGE

	}

	//보따리 비단 사용시 거래창 제한 체크 
	if (item->GetVnum() == 50200 | item->GetVnum() == 71049)
	{
		if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || GetOfflineShopOwner())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("거래창,창고 등을 연 상태에서는 보따리,비단보따리를 사용할수 없습니다."));
			return false;
		}

	}
	//END_PREVENT_TRADE_WINDOW

	if (IS_SET(item->GetFlag(), ITEM_FLAG_LOG)) // 사용 로그를 남기는 아이템 처리
	{
		DWORD vid = item->GetVID();
		DWORD oldCount = item->GetCount();
		DWORD vnum = item->GetVnum();

		char hint[ITEM_NAME_MAX_LEN + 32 + 1];
		int len = snprintf(hint, sizeof(hint) - 32, "%s", item->GetName());

		if (len < 0 || len >= (int) sizeof(hint) - 32)
			len = (sizeof(hint) - 32) - 1;

		bool ret = UseItemEx(item, DestCell, bUseAll);

		if (NULL == ITEM_MANAGER::instance().FindByVID(vid)) // UseItemEx에서 아이템이 삭제 되었다. 삭제 로그를 남김
		{
			LogManager::instance().ItemLog(this, vid, vnum, "REMOVE", hint);
		}
		else if (oldCount != item->GetCount())
		{
			snprintf(hint + len, sizeof(hint) - len, " %u", oldCount - 1);
			LogManager::instance().ItemLog(this, vid, vnum, "USE_ITEM", hint);
		}
		return (ret);
	}
	else
		return UseItemEx(item, DestCell, bUseAll);
}

bool CHARACTER::DropItem(TItemPos Cell, short bCount)
{
	LPITEM item = NULL; 

#ifdef WJ_SECURITY_SYSTEM
	if (IsActivateSecurity() == true)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("guvenlik_aktif"));
		return false;
	}
#endif
	if (!CanHandleItem())
	{
		if (NULL != DragonSoul_RefineWindow_GetOpener())
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("강화창을 연 상태에서는 아이템을 옮길 수 없습니다."));
		return false;
	}

	if (IsDead())
		return false;

	if (!IsValidItemPosition(Cell) || !(item = GetItem(Cell)))
		return false;

	if (item->IsExchanging())
		return false;

	if (true == item->isLocked())
		return false;

#ifdef NEW_PET_SYSTEM
	if (item->GetVnum() == 55701 || item->GetVnum() == 55702 || item->GetVnum() == 55703 || item->GetVnum() == 55704 || item->GetVnum() == 55705 || item->GetVnum() == 55706)
	{
		if (GetNewPetSystem()->IsActivePet())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pet_gonder"));
			return false;
		}
	}
#endif

	if (quest::CQuestManager::instance().GetPCForce(GetPlayerID())->IsRunning() == true)
		return false;

	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_DROP | ITEM_ANTIFLAG_GIVE))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("버릴 수 없는 아이템입니다."));
		return false;
	}

	if (bCount == 0 || bCount > item->GetCount())
		bCount = item->GetCount();

	SyncQuickslot(QUICKSLOT_TYPE_ITEM, Cell.cell, 255);	// Quickslot 에서 지움

	LPITEM pkItemToDrop;

	if (bCount == item->GetCount())
	{
		item->RemoveFromCharacter();
		pkItemToDrop = item;
	}
	else
	{
		if (bCount == 0)
		{
			if (test_server)
				sys_log(0, "[DROP_ITEM] drop item count == 0");
			return false;
		}
	
		// check non-split items for china
		//if (LC_IsNewCIBN())
		//	if (item->GetVnum() == 71095 || item->GetVnum() == 71050 || item->GetVnum() == 70038)
		//		return false;

		item->SetCount(item->GetCount() - bCount);
		ITEM_MANAGER::instance().FlushDelayedSave(item);

		pkItemToDrop = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), bCount);

		// copy item socket -- by mhh
		FN_copy_item_socket(pkItemToDrop, item);

		char szBuf[51 + 1];
		snprintf(szBuf, sizeof(szBuf), "%u %u", pkItemToDrop->GetID(), pkItemToDrop->GetCount());
		LogManager::instance().ItemLog(this, item, "ITEM_SPLIT", szBuf);
	}

	PIXEL_POSITION pxPos = GetXYZ();

	if (pkItemToDrop->AddToGround(GetMapIndex(), pxPos))
	{
		// 한국에는 아이템을 버리고 복구해달라는 진상유저들이 많아서
		// 아이템을 바닥에 버릴 시 속성로그를 남긴다.
		if (LC_IsYMIR())
			item->AttrLog();

		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("떨어진 아이템은 3분 후 사라집니다."));
		pkItemToDrop->StartDestroyEvent(10);

		ITEM_MANAGER::instance().FlushDelayedSave(pkItemToDrop);
		
		char szHint[32 + 1];
		snprintf(szHint, sizeof(szHint), "%s %u %u", pkItemToDrop->GetName(), pkItemToDrop->GetCount(), pkItemToDrop->GetOriginalVnum());
		LogManager::instance().ItemLog(this, pkItemToDrop, "DROP", szHint);
		//Motion(MOTION_PICKUP);
	}

	return true;
}

bool CHARACTER::DestroyItem(TItemPos Cell)
{
    LPITEM item = NULL;
    if (IsDead())
        return false;
    if (!IsValidItemPosition(Cell) || !(item = GetItem(Cell)))
        return false;
    if (item->IsExchanging())
        return false;
    if (true == item->isLocked())
        return false;
    if (quest::CQuestManager::instance().GetPCForce(GetPlayerID())->IsRunning() == true)
        return false;
    if (item->GetCount() <= 0)
        return false;
#ifdef ENABLE_SOULBIND_SYSTEM
	if(item->IsSealed() || item->GetSealBindTime() == -2)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Can't delete item."));
		return false;
	}
#endif
#ifdef NEW_PET_SYSTEM
	if (item->GetVnum() == 55701 || item->GetVnum() == 55702 || item->GetVnum() == 55703 || item->GetVnum() == 55704 || item->GetVnum() == 55705 || item->GetVnum() == 55706)
	{
		if (GetNewPetSystem()->IsActivePet())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pet_gonder"));
			return false;
		}
	}
#endif
	if (item->GetVnum() == 80005 || item->GetVnum() == 80020)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("silemezsinbarwon"));
		return false;
	}
    SyncQuickslot(QUICKSLOT_TYPE_ITEM, Cell.cell, 255);
    ITEM_MANAGER::instance().RemoveItem(item);
    ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s has succesfully deleted"), item->GetName());
    return true;
}

bool CHARACTER::SellItem(TItemPos Cell, short count)
{
	if (IsActivateSecurity() == true)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Cannot drop item with security key activate"));
		return false;
	}

	LPITEM item = NULL;

	if (!CanHandleItem())
	{
		if (NULL != DragonSoul_RefineWindow_GetOpener())
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°*E*A￠A≫ ¿￢ ≫oAA¿¡¼*´A ¾ÆAIAUA≫ ¿A±æ ¼o ¾ø½A´I´U."));
		return false;
	}

	if (IsDead())
		return false;

	if (!IsValidItemPosition(Cell) || !(item = GetItem(Cell)))
		return false;

	if (item->IsExchanging())
		return false;

#ifdef ENABLE_NEW_PET_SYSTEM
	if (GetNewPetSystem()->IsActivePet() && item->GetVnum() >= 55701 && item->GetVnum() <= 55709)
	{
		ChatPacket(CHAT_TYPE_INFO, "Once petini gonder. ");
		return false;
	}
#endif

	// if (item->IsBasicItem())
	// {
		// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_IS_BASIC_CANNOT_DO"));
		// return false;
	// }

	if(m_pkMyShop)
	{
		bool isItemSelling = m_pkMyShop->IsSellingItem(item->GetID());
		if (isItemSelling)
			return false;
	}

	if (true == item->isLocked())
		return false;

	#ifdef __SOULBINDING_SYSTEM__
	if (item->IsBind() || item->IsUntilBind())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't drop this item because is binded!"));
		return false;
	}
	#endif

	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_SELL))
	{
		ChatPacket(CHAT_TYPE_INFO, "Bu esya satilamaz!");
		return false;
	}

	if (quest::CQuestManager::instance().GetPCForce(GetPlayerID())->IsRunning() == true)
		return false;

	if (count <= 0 || count > item->GetCount())
		count = item->GetCount();

	long long dwPrice = 0;
	dwPrice = item->GetShopBuyPrice();

	// if(dwPrice > (DWORD)item->GetGold())
		// dwPrice = item->GetGold();

	dwPrice *= count;

	dwPrice /= 5;
	// DWORD dwTax = 0;
	// dwTax = dwPrice * 3/100;
	// dwPrice -= dwTax;
	const long long nTotalMoney = static_cast<long long>(GetGold()) + static_cast<long long>(dwPrice);

#ifdef YANG_LIMIT
	if (GetAllowedGold() <= static_cast<LDWORD>(nTotalMoney))
#else
	if (GOLD_MAX <= nTotalMoney)
#endif
	{
		sys_err("[OVERFLOW_GOLD] id %u name %s gold %u", GetPlayerID(), GetName(), GetGold());
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("20억냥이 초과하여 물품을 팔수 없습니다."));
		return false;
	}

	quest::CQuestManager::instance().ClearCurrentItem(); 
	// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Satis icin %d%% (%d yang) vergi uygulandi."), 3, dwTax);
	ITEM_MANAGER::instance().RemoveItem(item, "SELL");
	ChatPacket(CHAT_TYPE_INFO, "%s satildi.", item->GetName());
#ifdef YANG_LIMIT
	GoldChange(static_cast<LDWORD>(dwPrice));
#else
	PointChange(POINT_GOLD, dwPrice);
#endif
	return true;
}


#ifdef ENABLE_CHEQUE_SYSTEM
bool CHARACTER::DropCheque(int cheque)
{
	//--By: Edson.Jr
	if (int(GetQuestFlag("BlockItem.Enable")) == 1)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("INFO: Proibido remover o Won. Char protegido por senha!"));
		return false;
	}
	//---------------------------------------------------------------------------------

	if (cheque <= 0 || (long long)cheque > GetCheque())
		return false;

	if (!CanHandleItem())
		return false;

	if (0 != g_ChequeDropTimeLimitValue)
	{
		if (get_dword_time() < m_dwLastChequeDropTime+g_ChequeDropTimeLimitValue)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("?? ??? ?? ? ????."));
			return false;
		}
	}

	m_dwLastChequeDropTime = get_dword_time();

	LPITEM item = ITEM_MANAGER::instance().CreateItem(80020, cheque);

	if (item)
	{
		PIXEL_POSITION pos = GetXYZ();

		if (item->AddToGround(GetMapIndex(), pos))
		{
			PointChange(POINT_CHEQUE, -cheque, true);

			if (cheque > 100000)
				LogManager::instance().CharLog(this, cheque, "DROP_CHEQUE", "");

			if (false == LC_IsBrazil())
			{
				item->StartDestroyEvent(150);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("??? ???? %d? ? ?????."), 150/60);
			}
			else
			{
				item->StartDestroyEvent(60);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("??? ???? %d? ? ?????."), 1);
			}
		}

		Save();
		return true;
	}

	return false;
}
#endif


bool CHARACTER::MoveItem(TItemPos Cell, TItemPos DestCell, short count)
{
	LPITEM item = NULL;
#ifdef WJ_SECURITY_SYSTEM
	if (IsActivateSecurity() == true)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("guvenlik_aktif"));
		return false;
	}
#endif

	if (!IsValidItemPosition(Cell))
		return false;

	if (!(item = GetItem(Cell)))
		return false;

	if (item->IsExchanging())
		return false;

	if (item->GetCount() < count)
		return false;

	if (INVENTORY == Cell.window_type && Cell.cell >= INVENTORY_MAX_NUM && IS_SET(item->GetFlag(), ITEM_FLAG_IRREMOVABLE))
		return false;

	if (true == item->isLocked())
		return false;

	if (!IsValidItemPosition(DestCell))
	{
		return false;
	}

	if (!CanHandleItem())
	{
		if (NULL != DragonSoul_RefineWindow_GetOpener())
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("강화창을 연 상태에서는 아이템을 옮길 수 없습니다."));
		return false;
	}

	if (DestCell.window_type != DRAGON_SOUL_INVENTORY && DestCell.IsBeltInventoryPosition() && !CBeltInventoryHelper::CanMoveIntoBeltInventory(item))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ITEM_CANNOT_BE_MOVED_INTO_THE_BELT_INVENTORY"));
		return false;
	}

#ifdef ENABLE_SWITCHBOT
	if (Cell.IsSwitchbotPosition() && CSwitchbotManager::Instance().IsActive(GetPlayerID(), Cell.cell))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Cannot move active switchbot item."));
		return false;
	}

	if ((DestCell.IsSwitchbotPosition() && item->IsEquipped()) || (Cell.IsSwitchbotPosition() && DestCell.IsEquipPosition()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("giyilen_itemi_neden_koyuyorsun_aq."));
		return false;
	}

	if (DestCell.IsSwitchbotPosition() && !SwitchbotHelper::IsValidItem(item))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Invalid item type for switchbot."));
		return false;
	}
#endif

	// 이미 착용중인 아이템을 다른 곳으로 옮기는 경우, '장책 해제' 가능한 지 확인하고 옮김
	if (Cell.IsEquipPosition() && !CanUnequipNow(item))
		return false;

	if (DestCell.IsEquipPosition())
	{
		if (GetItem(DestCell))	// 장비일 경우 한 곳만 검사해도 된다.
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 장비를 착용하고 있습니다."));
			
			return false;
		}

		EquipItem(item, DestCell.cell - INVENTORY_MAX_NUM);
	}
	else
	{
		if (item->IsDragonSoul())
		{
			if (item->IsEquipped())
			{
				return DSManager::instance().PullOut(this, DestCell, item);
			}
			else
			{
				if (DestCell.window_type != DRAGON_SOUL_INVENTORY)
				{
					return false;
				}

				if (!DSManager::instance().IsValidCellForThisItem(item, DestCell))
					return false;
			}
		}
		// 용혼석이 아닌 아이템은 용혼석 인벤에 들어갈 수 없다.
		else if (DRAGON_SOUL_INVENTORY == DestCell.window_type)
			return false;
#ifdef ENABLE_SPECIAL_STORAGE
		if (!item->IsUpgradeItem() && UPGRADE_INVENTORY == DestCell.window_type)
			return false;
		if (!item->IsStone() && STONE_INVENTORY == DestCell.window_type)
			return false;
		if (!item->IsChest() && CHEST_INVENTORY == DestCell.window_type)
			return false;
		if (!item->IsAttr() && ATTR_INVENTORY == DestCell.window_type)
			return false;
#endif
		LPITEM item2;

		if ((item2 = GetItem(DestCell)) && item != item2 && item2->IsStackable() &&
				!IS_SET(item2->GetAntiFlag(), ITEM_ANTIFLAG_STACK) &&
				item2->GetVnum() == item->GetVnum()) // 합칠 수 있는 아이템의 경우
		{
			for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
				if (item2->GetSocket(i) != item->GetSocket(i))
					return false;

			if (count == 0)
				count = item->GetCount();

			sys_log(0, "%s: ITEM_STACK %s (window: %d, cell : %d) -> (window:%d, cell %d) count %d", GetName(), item->GetName(), Cell.window_type, Cell.cell, 
				DestCell.window_type, DestCell.cell, count);

			count = MIN(ITEM_MAX_COUNT - item2->GetCount(), count);

			item->SetCount(item->GetCount() - count);
			item2->SetCount(item2->GetCount() + count);
			return true;
		}

		if (!IsEmptyItemGrid(DestCell, item->GetSize(), Cell.cell))
			return false;

		if (count == 0 || count >= item->GetCount() || !item->IsStackable() || IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
		{
			sys_log(0, "%s: ITEM_MOVE %s (window: %d, cell : %d) -> (window:%d, cell %d) count %d", GetName(), item->GetName(), Cell.window_type, Cell.cell, 
				DestCell.window_type, DestCell.cell, count);
			
			if (COSTUME_MOUNT == item->GetSubType() && item->GetType() == ITEM_COSTUME)
			{
				if (item->IsEquipped())
				{
					StopRiding();		
					SetHorseAppearance(0);
					HorseSummon(false);
				}
			}
			else if (USE_PET == item->GetSubType() && item->GetType() == ITEM_UNIQUE)
			{
				if (item->IsEquipped())
				{
					CPetSystem* petSystem = GetPetSystem();

					if (0 != petSystem)
					{
						petSystem->Unsummon(GetPetAppearance());
						SetPetAppearance(0);	
					}
				}
			}

			item->RemoveFromCharacter();

						
			
			if (item->GetType() == ITEM_ARMOR && (item->GetSubType() == ARMOR_EAR || item->GetSubType() == ARMOR_WRIST || item->GetSubType() == ARMOR_NECK) && item->IsEquipped() == true)
			{
				int currentvalue = 0;
				int currentcount = 0;
				int braceletvalue = 0;
				int earvalue = 0;
				int necklacevalue = 0;
				
				RemoveAffect(AFFECT_BONUSSET);
			
				LPITEM bracelett = GetWear(WEAR_WRIST);
				LPITEM necklacee = GetWear(WEAR_NECK);
				LPITEM earr = GetWear(WEAR_EAR);
			
				if (bracelett)
				{
					braceletvalue = bracelett->GetValue(1);
				}
				if (necklacee)
				{
					necklacevalue = necklacee->GetValue(1);
				}
				if (earr)
				{
					earvalue = earr->GetValue(1);
				}
				 
				std::map<int, int> m_mapSetItems;
				m_mapSetItems.insert(std::make_pair(braceletvalue, 1));
				std::map<int, int>::iterator iter = m_mapSetItems.find(necklacevalue);
				if (iter == m_mapSetItems.end())
				{
					m_mapSetItems.insert(std::make_pair(necklacevalue, 1));
				}
				else
				{
					iter->second++;
				}
				std::map<int, int>::iterator iter2 = m_mapSetItems.find(earvalue);
				if (iter2 == m_mapSetItems.end())
				{
					m_mapSetItems.insert(std::make_pair(earvalue, 1));
				}
				else
				{
					iter2->second++;
				}
	
				std::map<int, int>::iterator data = m_mapSetItems.begin();
				while (data != m_mapSetItems.end())
				{
					if (currentcount < data->second)
					{
						currentcount = data->second;
						currentvalue = data->first;
					}
					data++;
				}
				int setaffectvalue = AccSet[currentvalue][currentcount];
				int setaffecttype = AccSet[currentvalue][0];
				if (currentcount > 1 && currentvalue !=0)
				{
					AddAffect(AFFECT_BONUSSET, setaffecttype, setaffectvalue, 0, INFINITE_AFFECT_DURATION, 0, true);
				}
				m_mapSetItems.clear();				
			}

			SetItem(DestCell, item);

			if (INVENTORY == Cell.window_type && INVENTORY == DestCell.window_type)
				SyncQuickslot(QUICKSLOT_TYPE_ITEM, Cell.cell, DestCell.cell);
		}
		else if (count < item->GetCount())
		{
			//check non-split items 
			//if (LC_IsNewCIBN())
			//{
			//	if (item->GetVnum() == 71095 || item->GetVnum() == 71050 || item->GetVnum() == 70038)
			//	{
			//		return false;
			//	}
			//}

			sys_log(0, "%s: ITEM_SPLIT %s (window: %d, cell : %d) -> (window:%d, cell %d) count %d", GetName(), item->GetName(), Cell.window_type, Cell.cell, 
				DestCell.window_type, DestCell.cell, count);

			item->SetCount(item->GetCount() - count);
			LPITEM item2 = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), count);

			// copy socket -- by mhh
			FN_copy_item_socket(item2, item);

			item2->AddToCharacter(this, DestCell);

			char szBuf[51+1];
			snprintf(szBuf, sizeof(szBuf), "%u %u %u %u ", item2->GetID(), item2->GetCount(), item->GetCount(), item->GetCount() + item2->GetCount());
			LogManager::instance().ItemLog(this, item, "ITEM_SPLIT", szBuf);
		}
	}

	return true;
}

namespace NPartyPickupDistribute
{
	struct FFindOwnership
	{
		LPITEM item;
		LPCHARACTER owner;

		FFindOwnership(LPITEM item) 
			: item(item), owner(NULL)
		{
		}

		void operator () (LPCHARACTER ch)
		{
			if (item->IsOwnership(ch))
				owner = ch;
		}
	};

	struct FCountNearMember
	{
		int		total;
		int		x, y;

		FCountNearMember(LPCHARACTER center )
			: total(0), x(center->GetX()), y(center->GetY())
		{
		}

		void operator () (LPCHARACTER ch)
		{
			if (DISTANCE_APPROX(ch->GetX() - x, ch->GetY() - y) <= PARTY_DEFAULT_RANGE)
				total += 1;
		}
	};

	struct FMoneyDistributor
	{
		int		total;
		LPCHARACTER	c;
		int		x, y;
		int		iMoney;

		FMoneyDistributor(LPCHARACTER center, int iMoney) 
			: total(0), c(center), x(center->GetX()), y(center->GetY()), iMoney(iMoney) 
		{
		}

		void operator ()(LPCHARACTER ch)
		{
			if (ch!=c)
				if (DISTANCE_APPROX(ch->GetX() - x, ch->GetY() - y) <= PARTY_DEFAULT_RANGE)
				{
#ifdef YANG_LIMIT
					ch->GoldChange(static_cast<LDWORD>(iMoney),"PARTY");
#else
					ch->PointChange(POINT_GOLD, iMoney, true);
#endif

					if (iMoney > 1000) // 천원 이상만 기록한다.
						LogManager::instance().CharLog(ch, iMoney, "GET_GOLD", "");
				}
		}
	};
#ifdef ENABLE_CHEQUE_SYSTEM
	struct FChequeDistributor
	{
		int		total;
		LPCHARACTER	c;
		int		x, y;
		int		iCheque;

		FChequeDistributor(LPCHARACTER center, int iCheque) 
			: total(0), c(center), x(center->GetX()), y(center->GetY()), iCheque(iCheque) 
		{
		}

		void operator ()(LPCHARACTER ch)
		{
			if (ch!=c)
				if (DISTANCE_APPROX(ch->GetX() - x, ch->GetY() - y) <= PARTY_DEFAULT_RANGE)
				{
					ch->PointChange(POINT_CHEQUE, iCheque, true);

					if (iCheque > 100000) // ?? ??? ????.
						LogManager::instance().CharLog(ch, iCheque, "GET_CHEQUE", "");
				}
		}
	};
#endif
}

#ifdef YANG_LIMIT
void CHARACTER::GiveGold(LDWORD iAmount)
{
	if (iAmount <= 0)
		return;

	//sys_log(0, "GIVE_GOLD: %s %d", GetName(), iAmount);

	if (GetParty())
	{
		LPPARTY pParty = GetParty();

		// ??? ?? ?? ??? ???.
		DWORD dwTotal = iAmount;
		DWORD dwMyAmount = dwTotal;

		NPartyPickupDistribute::FCountNearMember funcCountNearMember(this);
		pParty->ForEachOnlineMember(funcCountNearMember);

		if (funcCountNearMember.total > 1)
		{
			DWORD dwShare = dwTotal / funcCountNearMember.total;
			dwMyAmount -= dwShare * (funcCountNearMember.total - 1);

			NPartyPickupDistribute::FMoneyDistributor funcMoneyDist(this, dwShare);

			pParty->ForEachOnlineMember(funcMoneyDist);
		}

		GoldChange(static_cast<LDWORD>(dwMyAmount),"GiveGold - 1");
		if (dwMyAmount > 1000) // ?? ??? ????.
			LogManager::instance().CharLog(this, dwMyAmount, "GET_GOLD", "");
	}
	else
	{
		GoldChange(static_cast<LDWORD>(iAmount), "GiveGold - 2");

		// ???? ?? ????? ??? ???,
		// ??? ???? ?? ???,
		// ????, ?? ?? 1000? ??? ?? ?? ?? ??? 0?? ???, 
		// ?? ????? ?? ???? ?? ?? ??.
		// ??? ?? ??? ?? ?? ?? ??? ??? ???? ??? ??.
		if (LC_IsBrazil() == true)
		{
			if (iAmount >= 213)
				LogManager::instance().CharLog(this, iAmount, "GET_GOLD", "");
		}
		else
		{
			if (iAmount > 1000) // ?? ??? ????.
				LogManager::instance().CharLog(this, iAmount, "GET_GOLD", "");
		}
	}
}
bool CHARACTER::DropGold(LDWORD gold)
{
	return false;

	if (!CanHandleItem())
		return false;

	if (0 != g_GoldDropTimeLimitValue)
	{
		if (get_dword_time() < m_dwLastGoldDropTime + g_GoldDropTimeLimitValue)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("?? ??? ?? ? ????."));
			return false;
		}
	}

	m_dwLastGoldDropTime = get_dword_time();

	LPITEM item = ITEM_MANAGER::instance().CreateItem(1, gold);

	if (item)
	{
		PIXEL_POSITION pos = GetXYZ();

		if (item->AddToGround(GetMapIndex(), pos))
		{
			GoldChange(static_cast<LDWORD>(-gold), "DropGold");

			// ???? ?? ????? ??? ???,
			// ??? ???? ?? ???,
			// ????, ?? ?? 1000? ??? ?? ?? ?? ??? 0?? ???, 
			// ?? ????? ?? ???? ?? ?? ??.
			// ??? ?? ??? ?? ?? ?? ??? ??? ???? ??? ??.
			if (LC_IsBrazil() == true)
			{
				if (gold >= 213)
					LogManager::instance().CharLog(this, gold, "DROP_GOLD", "");
			}
			else
			{
				if (gold > 1000) // ?? ??? ????.
					LogManager::instance().CharLog(this, gold, "DROP_GOLD", "");
			}

			if (false == LC_IsBrazil())
			{
				item->StartDestroyEvent(150);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("??? ???? %d? ? ?????."), 150 / 60);
			}
			else
			{
				item->StartDestroyEvent(60);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("??? ???? %d? ? ?????."), 1);
			}
		}

		Save();
		return true;
	}

	return false;
}
#else
bool CHARACTER::DropGold(int gold)
{
	ChatPacket(CHAT_TYPE_INFO, "GOLD_BLOCK");
		return false;
}
void CHARACTER::GiveGold(int iAmount)
{
	if (iAmount <= 0)
		return;

	sys_log(0, "GIVE_GOLD: %s %lld", GetName(), iAmount);

	if (GetParty())
	{
		LPPARTY pParty = GetParty();

		// 파티가 있는 경우 나누어 가진다.
		DWORD dwTotal = iAmount;
		DWORD dwMyAmount = dwTotal;

		NPartyPickupDistribute::FCountNearMember funcCountNearMember(this);
		pParty->ForEachOnlineMember(funcCountNearMember);

		if (funcCountNearMember.total > 1)
		{
			DWORD dwShare = dwTotal / funcCountNearMember.total;
			dwMyAmount -= dwShare * (funcCountNearMember.total - 1);

			NPartyPickupDistribute::FMoneyDistributor funcMoneyDist(this, dwShare);

			pParty->ForEachOnlineMember(funcMoneyDist);
		}

		PointChange(POINT_GOLD, dwMyAmount, true);

		if (dwMyAmount > 1000) // 천원 이상만 기록한다.
			LogManager::instance().CharLog(this, dwMyAmount, "GET_GOLD", "");
	}
	else
	{
		PointChange(POINT_GOLD, iAmount, true);

		// 브라질에 돈이 없어진다는 버그가 있는데,
		// 가능한 시나리오 중에 하나는,
		// 메크로나, 핵을 써서 1000원 이하의 돈을 계속 버려 골드를 0으로 만들고, 
		// 돈이 없어졌다고 복구 신청하는 것일 수도 있다.
		// 따라서 그런 경우를 잡기 위해 낮은 수치의 골드에 대해서도 로그를 남김.
		if (LC_IsBrazil() == true)
		{
			if (iAmount >= 213)
				LogManager::instance().CharLog(this, iAmount, "GET_GOLD", "");
		}
		else
		{
			if (iAmount > 1000) // 천원 이상만 기록한다.
				LogManager::instance().CharLog(this, iAmount, "GET_GOLD", "");
		}
	}
}
#endif

#ifdef ENABLE_CHEQUE_SYSTEM
void CHARACTER::GiveCheque(int iAmount)
{
	if (iAmount <= 0)
		return;

	//sys_log(0, "GIVE_GOLD: %s %lld", GetName(), iAmount);

	if (GetParty())
	{
		LPPARTY pParty = GetParty();

		// ??? ?? ?? ??? ???.
		DWORD dwTotal = iAmount;
		DWORD dwMyAmount = dwTotal;

		NPartyPickupDistribute::FCountNearMember funcCountNearMember(this);
		pParty->ForEachOnlineMember(funcCountNearMember);

		if (funcCountNearMember.total > 1)
		{
			DWORD dwShare = dwTotal / funcCountNearMember.total;
			dwMyAmount -= dwShare * (funcCountNearMember.total - 1);

			NPartyPickupDistribute::FChequeDistributor funcChequeDist(this, dwShare);

			pParty->ForEachOnlineMember(funcChequeDist);
		}

		PointChange(POINT_CHEQUE, dwMyAmount, true);

		if (dwMyAmount > 100000) // ?? ??? ????.
			LogManager::instance().CharLog(this, dwMyAmount, "GET_CHEQUE", "");
	}
	else
	{
		PointChange(POINT_CHEQUE, iAmount, true);

		// ???? ?? ????? ??? ???,
		// ??? ???? ?? ???,
		// ????, ?? ?? 1000? ??? ?? ?? ?? ??? 0?? ???, 
		// ?? ????? ?? ???? ?? ?? ??.
		// ??? ?? ??? ?? ?? ?? ??? ??? ???? ??? ??.
		if (LC_IsBrazil() == true)
		{
			if (iAmount >= 213)
				LogManager::instance().CharLog(this, iAmount, "GET_CHEQUE", "");
		}
		else
		{
			if (iAmount > 100000) // ?? ??? ????.
				LogManager::instance().CharLog(this, iAmount, "GET_CHEQUE", "");
		}
	}
}
#endif
bool CHARACTER::PickupItem(DWORD dwVID)
{
#ifdef WJ_SECURITY_SYSTEM
	if (IsActivateSecurity() == true)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("guvenlik_aktif"));
		return false;
	}
#endif
	LPITEM item = ITEM_MANAGER::instance().FindByVID(dwVID);

	if (IsObserverMode())
		return false;

	if (!item || !item->GetSectree())
		return false;

	if (item->DistanceValid(this))
	{
		if (item->IsOwnership(this))
		{
			// 만약 주으려 하는 아이템이 엘크라면
			if (item->GetType() == ITEM_ELK)
			{
				GiveGold(item->GetCount());
				item->RemoveFromGround();

				M2_DESTROY_ITEM(item);

				Save();
			}
#ifdef ENABLE_CHEQUE_SYSTEM
			else if (item->GetType() == ITEM_WON)
			{
				GiveCheque(item->GetCount());
				item->RemoveFromGround();

				M2_DESTROY_ITEM(item);

				Save();
			}
#endif
			// 평범한 아이템이라면
			else
			{
				if (item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					short bCount = item->GetCount();

					for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
					{
						LPITEM item2 = GetInventoryItem(i);

						if (!item2)
							continue;

						if (item2->GetVnum() == item->GetVnum())
						{
							int j;

							for (j = 0; j < ITEM_SOCKET_MAX_NUM; ++j)
								if (item2->GetSocket(j) != item->GetSocket(j))
									break;

							if (j != ITEM_SOCKET_MAX_NUM)
								continue;

							short bCount2 = MIN(ITEM_MAX_COUNT - item2->GetCount(), bCount);
							bCount -= bCount2;

							item2->SetCount(item2->GetCount() + bCount2);

							if (bCount == 0)
							{
#if defined(__CHATTING_WINDOW_RENEWAL__)
								ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("아이템 획득: %s"), item2->GetName());
#else
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 획득: %s"), item2->GetName());
#endif
								M2_DESTROY_ITEM(item);
								if (item2->GetType() == ITEM_QUEST)
									quest::CQuestManager::instance().PickupItem (GetPlayerID(), item2);
								return true;
							}
						}
					}

					item->SetCount(bCount);
				}
#ifdef ENABLE_SPECIAL_STORAGE
				if (item->IsUpgradeItem() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					short bCount = item->GetCount();

					for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
					{
						LPITEM item2 = GetUpgradeInventoryItem(i);

						if (!item2)
							continue;

						if (item2->GetVnum() == item->GetVnum())
						{

							short bCount2 = MIN(ITEM_MAX_COUNT - item2->GetCount(), bCount);
							bCount -= bCount2;

							item2->SetCount(item2->GetCount() + bCount2);

							if (bCount == 0)
							{
#if defined(__CHATTING_WINDOW_RENEWAL__)
								ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("아이템 획득: %s"), item2->GetName());
#else
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 획득: %s"), item2->GetName());
#endif
								M2_DESTROY_ITEM(item);
								return true;
							}
						}
					}

					item->SetCount(bCount);
				}
				else if (item->IsStone() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					short bCount = item->GetCount();

					for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
					{
						LPITEM item2 = GetStoneInventoryItem(i);

						if (!item2)
							continue;

						if (item2->GetVnum() == item->GetVnum())
						{

							short bCount2 = MIN(ITEM_MAX_COUNT - item2->GetCount(), bCount);
							bCount -= bCount2;

							item2->SetCount(item2->GetCount() + bCount2);

							if (bCount == 0)
							{
#if defined(__CHATTING_WINDOW_RENEWAL__)
								ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("아이템 획득: %s"), item2->GetName());
#else
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 획득: %s"), item2->GetName());
#endif
								M2_DESTROY_ITEM(item);
								return true;
							}
						}
					}

					item->SetCount(bCount);
				}
				else if (item->IsChest() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					short bCount = item->GetCount();

					for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
					{
						LPITEM item2 = GetChestInventoryItem(i);

						if (!item2)
							continue;

						if (item2->GetVnum() == item->GetVnum())
						{

							short bCount2 = MIN(ITEM_MAX_COUNT - item2->GetCount(), bCount);
							bCount -= bCount2;

							item2->SetCount(item2->GetCount() + bCount2);

							if (bCount == 0)
							{
#if defined(__CHATTING_WINDOW_RENEWAL__)
								ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("아이템 획득: %s"), item2->GetName());
#else
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 획득: %s"), item2->GetName());
#endif
								M2_DESTROY_ITEM(item);
								return true;
							}
						}
					}

					item->SetCount(bCount);
				}
				else if (item->IsAttr() && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					short bCount = item->GetCount();

					for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
					{
						LPITEM item2 = GetAttrInventoryItem(i);

						if (!item2)
							continue;

						if (item2->GetVnum() == item->GetVnum())
						{

							short bCount2 = MIN(ITEM_MAX_COUNT - item2->GetCount(), bCount);
							bCount -= bCount2;

							item2->SetCount(item2->GetCount() + bCount2);

							if (bCount == 0)
							{
#if defined(__CHATTING_WINDOW_RENEWAL__)
								ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("아이템 획득: %s"), item2->GetName());
#else
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 획득: %s"), item2->GetName());
#endif
								M2_DESTROY_ITEM(item);
								return true;
							}
						}
					}

					item->SetCount(bCount);
				}
#endif

				int iEmptyCell;
				if (item->IsDragonSoul())
				{
					if ((iEmptyCell = GetEmptyDragonSoulInventory(item)) == -1)
					{
						sys_log(0, "No empty ds inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소지하고 있는 아이템이 너무 많습니다."));
						return false;
					}
				}
#ifdef ENABLE_SPECIAL_STORAGE
				else if (item->IsUpgradeItem())
				{
					if ((iEmptyCell = GetEmptyUpgradeInventory(item)) == -1)
					{
						sys_log(0, "No empty ssu inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소지하고 있는 아이템이 너무 많습니다."));
						return false;
					}
				}
				else if (item->IsStone())
				{
					if ((iEmptyCell = GetEmptyStoneInventory(item)) == -1)
					{
						sys_log(0, "No empty ssu inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소지하고 있는 아이템이 너무 많습니다."));
						return false;
					}
				}
				else if (item->IsChest())
				{
					if ((iEmptyCell = GetEmptyChestInventory(item)) == -1)
					{
						sys_log(0, "No empty ssu inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소지하고 있는 아이템이 너무 많습니다."));
						return false;
					}
				}
				else if (item->IsAttr())
				{
					if ((iEmptyCell = GetEmptyAttrInventory(item)) == -1)
					{
						sys_log(0, "No empty ssu inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소지하고 있는 아이템이 너무 많습니다."));
						return false;
					}
				}
#endif
				else
				{
					if ((iEmptyCell = GetEmptyInventory(item->GetSize())) == -1)
					{
						sys_log(0, "No empty inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소지하고 있는 아이템이 너무 많습니다."));
						return false;
					}
				}

				item->RemoveFromGround();
				
				if (item->IsDragonSoul())
					item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyCell));
#ifdef ENABLE_SPECIAL_STORAGE
				else if (item->IsUpgradeItem())
					item->AddToCharacter(this, TItemPos(UPGRADE_INVENTORY, iEmptyCell));
				else if (item->IsStone())
					item->AddToCharacter(this, TItemPos(STONE_INVENTORY, iEmptyCell));
				else if (item->IsChest())
					item->AddToCharacter(this, TItemPos(CHEST_INVENTORY, iEmptyCell));
				else if (item->IsAttr())
					item->AddToCharacter(this, TItemPos(ATTR_INVENTORY, iEmptyCell));
#endif
				else
					item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));

				char szHint[32+1];
				snprintf(szHint, sizeof(szHint), "%s %u %u", item->GetName(), item->GetCount(), item->GetOriginalVnum());
				LogManager::instance().ItemLog(this, item, "GET", szHint);
#if defined(__CHATTING_WINDOW_RENEWAL__)
				ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#else
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#endif

				if (item->GetType() == ITEM_QUEST)
					quest::CQuestManager::instance().PickupItem (GetPlayerID(), item);
			}

			//Motion(MOTION_PICKUP);
			return true;
		}
		else if (!IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_GIVE | ITEM_ANTIFLAG_DROP) && GetParty())
		{
			// 다른 파티원 소유권 아이템을 주으려고 한다면
			NPartyPickupDistribute::FFindOwnership funcFindOwnership(item);

			GetParty()->ForEachOnlineMember(funcFindOwnership);

			LPCHARACTER owner = funcFindOwnership.owner;
			if (!owner)
				return false;

			int iEmptyCell;

			if (item->IsDragonSoul())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyDragonSoulInventory(item)) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyDragonSoulInventory(item)) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소지하고 있는 아이템이 너무 많습니다."));
						return false;
					}
				}
			}
#ifdef ENABLE_SPECIAL_STORAGE
			else if (item->IsUpgradeItem())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyUpgradeInventory(item)) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyUpgradeInventory(item)) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소지하고 있는 아이템이 너무 많습니다."));
						return false;
					}
				}
			}
			else if (item->IsStone())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyStoneInventory(item)) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyStoneInventory(item)) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소지하고 있는 아이템이 너무 많습니다."));
						return false;
					}
				}
			}
			else if (item->IsChest())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyChestInventory(item)) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyChestInventory(item)) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소지하고 있는 아이템이 너무 많습니다."));
						return false;
					}
				}
			}
			else if (item->IsAttr())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyAttrInventory(item)) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyAttrInventory(item)) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소지하고 있는 아이템이 너무 많습니다."));
						return false;
					}
				}
			}
#endif
			else
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyInventory(item->GetSize())) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyInventory(item->GetSize())) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("소지하고 있는 아이템이 너무 많습니다."));
						return false;
					}
				}
			}

			item->RemoveFromGround();

			if (item->IsDragonSoul())
				item->AddToCharacter(owner, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyCell));
#ifdef ENABLE_SPECIAL_STORAGE
			else if (item->IsUpgradeItem())
				item->AddToCharacter(owner, TItemPos(UPGRADE_INVENTORY, iEmptyCell));
			else if (item->IsStone())
				item->AddToCharacter(owner, TItemPos(STONE_INVENTORY, iEmptyCell));
			else if (item->IsChest())
				item->AddToCharacter(owner, TItemPos(CHEST_INVENTORY, iEmptyCell));
			else if (item->IsAttr())
				item->AddToCharacter(owner, TItemPos(ATTR_INVENTORY, iEmptyCell));
#endif
			else
				item->AddToCharacter(owner, TItemPos(INVENTORY, iEmptyCell));

			char szHint[32+1];
			snprintf(szHint, sizeof(szHint), "%s %u %u", item->GetName(), item->GetCount(), item->GetOriginalVnum());
			LogManager::instance().ItemLog(owner, item, "GET", szHint);

			if (owner == this)
#if defined(__CHATTING_WINDOW_RENEWAL__)
				ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#else
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#endif
			else
			{
				owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 획득: %s 님으로부터 %s"), GetName(), item->GetName());
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 전달: %s 님에게 %s"), owner->GetName(), item->GetName());
			}

			if (item->GetType() == ITEM_QUEST)
				quest::CQuestManager::instance().PickupItem (owner->GetPlayerID(), item);

			return true;
		}
	}

	return false;
}

bool CHARACTER::SwapItem(BYTE bCell, BYTE bDestCell)
{
	if (!CanHandleItem())
		return false;

	TItemPos srcCell(INVENTORY, bCell), destCell(INVENTORY, bDestCell);

	// 올바른 Cell 인지 검사
	// 용혼석은 Swap할 수 없으므로, 여기서 걸림.
	//if (bCell >= INVENTORY_MAX_NUM + WEAR_MAX_NUM || bDestCell >= INVENTORY_MAX_NUM + WEAR_MAX_NUM)
	if (srcCell.IsDragonSoulEquipPosition() || destCell.IsDragonSoulEquipPosition())
		return false;

	// 같은 CELL 인지 검사
	if (bCell == bDestCell)
		return false;

	// 둘 다 장비창 위치면 Swap 할 수 없다.
	if (srcCell.IsEquipPosition() && destCell.IsEquipPosition())
		return false;

	LPITEM item1, item2;

	// item2가 장비창에 있는 것이 되도록.
	if (srcCell.IsEquipPosition())
	{
		item1 = GetInventoryItem(bDestCell);
		item2 = GetInventoryItem(bCell);
	}
	else
	{
		item1 = GetInventoryItem(bCell);
		item2 = GetInventoryItem(bDestCell);
	}

	if (!item1 || !item2)
		return false;
	
	if (item1 == item2)
	{
	    sys_log(0, "[WARNING][WARNING][HACK USER!] : %s %d %d", m_stName.c_str(), bCell, bDestCell);
	    return false;
	}

	// item2가 bCell위치에 들어갈 수 있는지 확인한다.
	if (!IsEmptyItemGrid(TItemPos (INVENTORY, item1->GetCell()), item2->GetSize(), item1->GetCell()))
		return false;

	// 바꿀 아이템이 장비창에 있으면
	if (TItemPos(EQUIPMENT, item2->GetCell()).IsEquipPosition())
	{
		BYTE bEquipCell = item2->GetCell() - INVENTORY_MAX_NUM;
		BYTE bInvenCell = item1->GetCell();

		// 착용중인 아이템을 벗을 수 있고, 착용 예정 아이템이 착용 가능한 상태여야만 진행
		if (false == CanEquipNow(item1))  
			return false;

		if (item2->IsDragonSoul() && false == CanUnequipNow(item2))
            return false;

		item2->RemoveFromCharacter();

		if (item1->EquipTo(this, bEquipCell)) 
		{
			item2->AddToCharacter(this, TItemPos(INVENTORY, bInvenCell));
			item2->ModifyPoints(false); //item_swap fix ds_aim
			ComputePoints();			// item_swap fix ds_aim
		} 
		else
		{
			sys_err("SwapItem cannot equip %s! item1 %s", item2->GetName(), item1->GetName());
		}
	}
	else
	{
		BYTE bCell1 = item1->GetCell();
		BYTE bCell2 = item2->GetCell();
		
		item1->RemoveFromCharacter();
		item2->RemoveFromCharacter();

		item1->AddToCharacter(this, TItemPos(INVENTORY, bCell2));
		item2->AddToCharacter(this, TItemPos(INVENTORY, bCell1));
	}

	return true;
}

bool CHARACTER::UnequipItem(LPITEM item)
{
	int pos;

	if (false == CanUnequipNow(item))
		return false;

	if (item->IsDragonSoul())
		pos = GetEmptyDragonSoulInventory(item);
	else
		pos = GetEmptyInventory(item->GetSize());

	// HARD CODING
	if (item->GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
		ShowAlignment(true);

	item->RemoveFromCharacter();

	if (item->GetType() == ITEM_ARMOR && (item->GetSubType() == ARMOR_EAR || item->GetSubType() == ARMOR_WRIST || item->GetSubType() == ARMOR_NECK))
	{
		int currentvalue = 0;
		int currentcount = 0;
		int braceletvalue = 0;
		int earvalue = 0;
		int necklacevalue = 0;
				
		RemoveAffect(AFFECT_BONUSSET);
		
		LPITEM bracelett = GetWear(WEAR_WRIST);
		LPITEM necklacee = GetWear(WEAR_NECK);
		LPITEM earr = GetWear(WEAR_EAR);
			
		if (bracelett)
		{
			braceletvalue = bracelett->GetValue(1);
		}
		if (necklacee)
		{
			necklacevalue = necklacee->GetValue(1);
		}
		if (earr)
		{
			earvalue = earr->GetValue(1);
		}
				 
		std::map<int, int> m_mapSetItems;
		m_mapSetItems.insert(std::make_pair(braceletvalue, 1));
		std::map<int, int>::iterator iter = m_mapSetItems.find(necklacevalue);
		if (iter == m_mapSetItems.end())
		{
			m_mapSetItems.insert(std::make_pair(necklacevalue, 1));
		}
		else
		{
			iter->second++;
		}
		std::map<int, int>::iterator iter2 = m_mapSetItems.find(earvalue);
		if (iter2 == m_mapSetItems.end())
		{
			m_mapSetItems.insert(std::make_pair(earvalue, 1));
		}
		else
		{
			iter2->second++;
		}
	
		std::map<int, int>::iterator data = m_mapSetItems.begin();
		while (data != m_mapSetItems.end())
		{
			if (currentcount < data->second)
			{
				currentcount = data->second;
				currentvalue = data->first;
			}
			data++;
		}
		int setaffectvalue = AccSet[currentvalue][currentcount];
		int setaffecttype = AccSet[currentvalue][0];
		if (currentcount > 1 && currentvalue !=0)
		{
			AddAffect(AFFECT_BONUSSET, setaffecttype, setaffectvalue, 0, INFINITE_AFFECT_DURATION, 0, true);
		}
		m_mapSetItems.clear();		
	}			

	if (item->IsDragonSoul())
	{
		item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, pos));
	}
	else
		item->AddToCharacter(this, TItemPos(INVENTORY, pos));
	
	if (COSTUME_MOUNT == item->GetSubType() && item->GetType() == ITEM_COSTUME)
	{
		StopRiding();		
		SetHorseAppearance(0);
		HorseSummon(false);
	}
	else if (USE_PET == item->GetSubType() && item->GetType() == ITEM_UNIQUE)
	{
		CPetSystem* petSystem = GetPetSystem();

		if (0 != petSystem)
		{
			petSystem->Unsummon(GetPetAppearance());
			SetPetAppearance(0);	
		}		
	}

	CheckMaximumPoints();

	return true;
}

//
// @version	05/07/05 Bang2ni - Skill 사용후 1.5 초 이내에 장비 착용 금지
//
bool CHARACTER::EquipItem(LPITEM item, int iCandidateCell)
{
	if (item->IsExchanging())
		return false;

	if (false == item->IsEquipable())
		return false;

	if (false == CanEquipNow(item))
		return false;

	
	if (ITEM_BELT == item->GetType() && CBeltInventoryHelper::IsExistItemInBeltInventory(this))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("KEMER_ENVANTERINI_BOSALT"));
		return false;
	}
	int iWearCell = item->FindEquipCell(this, iCandidateCell);

	if ((iWearCell == WEAR_BODY) && IsRiding() && get_global_time() < GetQuestFlag("zirh.engel"))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("binek_engel"));
		return false;
	}

	if ((iWearCell == WEAR_COSTUME_BODY) && IsRiding() && get_global_time() < GetQuestFlag("zirh.engel1"))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("binek_engel"));
		return false;
	}
	if (iWearCell < 0)
		return false;

	// 무언가를 탄 상태에서 턱시도 입기 금지
	if (iWearCell == WEAR_BODY && IsRiding() && (item->GetVnum() >= 11901 && item->GetVnum() <= 11904))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("말을 탄 상태에서 예복을 입을 수 없습니다."));
		return false;
	}

	if (iWearCell != WEAR_ARROW && IsPolymorphed())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("둔갑 중에는 착용중인 장비를 변경할 수 없습니다."));
		return false;
	}

	if (FN_check_item_sex(this, item) == false)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("성별이 맞지않아 이 아이템을 사용할 수 없습니다."));
		return false;
	}

	//신규 탈것 사용시 기존 말 사용여부 체크
	if(item->IsRideItem() && IsRiding())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 탈것을 이용중입니다."));
		return false;
	}

	// 화살 이외에는 마지막 공격 시간 또는 스킬 사용 1.5 후에 장비 교체가 가능
	DWORD dwCurTime = get_dword_time();

	/*if (iWearCell != WEAR_ARROW 
		&& (dwCurTime - GetLastAttackTime() <= 1500 || dwCurTime - m_dwLastSkillTime <= 1500))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("가만히 있을 때만 착용할 수 있습니다."));
		return false;
	}*/

	// 용혼석 특수 처리
	if (item->IsDragonSoul())
	{
		// 같은 타입의 용혼석이 이미 들어가 있다면 착용할 수 없다.
		// 용혼석은 swap을 지원하면 안됨.
		if(GetInventoryItem(INVENTORY_MAX_NUM + iWearCell))
		{
			ChatPacket(CHAT_TYPE_INFO, "이미 같은 종류의 용혼석을 착용하고 있습니다.");
			return false;
		}
		
		if (!item->EquipTo(this, iWearCell))
		{
			return false;
		}
	}
	// 용혼석이 아님.
	else
	{
		if (COSTUME_MOUNT == item->GetSubType() && item->GetType() == ITEM_COSTUME)
		{
			LPITEM binekvnum = GetWear(WEAR_COSTUME_MOUNT);
			if (NULL != binekvnum)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("binek_cikar_once"));
				return false;
			}
		}
		else if (USE_PET == item->GetSubType() && item->GetType() == ITEM_UNIQUE)
		{
			LPITEM petvnum1 = GetWear(WEAR_PET);
			if (NULL != petvnum1)
			{
				if (USE_PET == petvnum1->GetSubType() && petvnum1->GetType() == ITEM_UNIQUE)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pet_cikar_once"));
					return false;
				}
			}
		}
		// 착용할 곳에 아이템이 있다면,
		if (GetWear(iWearCell) && !IS_SET(GetWear(iWearCell)->GetFlag(), ITEM_FLAG_IRREMOVABLE))
		{
			// 이 아이템은 한번 박히면 변경 불가. swap 역시 완전 불가
			if (item->GetWearFlag() == WEARABLE_ABILITY) 
				return false;

			if (false == SwapItem(item->GetCell(), INVENTORY_MAX_NUM + iWearCell))
			{
				return false;
			}
		}
		else
		{
			BYTE bOldCell = item->GetCell();

			if (item->EquipTo(this, iWearCell))
			{
				SyncQuickslot(QUICKSLOT_TYPE_ITEM, bOldCell, iWearCell);
			}
		}
	}

	if (true == item->IsEquipped())
	{
		// 아이템 최초 사용 이후부터는 사용하지 않아도 시간이 차감되는 방식 처리. 
		if (-1 != item->GetProto()->cLimitRealTimeFirstUseIndex)
		{
			// 한 번이라도 사용한 아이템인지 여부는 Socket1을 보고 판단한다. (Socket1에 사용횟수 기록)
			if (0 == item->GetSocket(1))
			{
				// 사용가능시간은 Default 값으로 Limit Value 값을 사용하되, Socket0에 값이 있으면 그 값을 사용하도록 한다. (단위는 초)
				long duration = (0 != item->GetSocket(0)) ? item->GetSocket(0) : item->GetProto()->aLimits[item->GetProto()->cLimitRealTimeFirstUseIndex].lValue;

				if (0 == duration)
					duration = 60 * 60 * 24 * 7;

				item->SetSocket(0, time(0) + duration);
				item->StartRealTimeExpireEvent();
			}

			item->SetSocket(1, item->GetSocket(1) + 1);
		}

		if (item->GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
			ShowAlignment(false);

		const DWORD& dwVnum = item->GetVnum();

		// 라마단 이벤트 초승달의 반지(71135) 착용시 이펙트 발동
		if (true == CItemVnumHelper::IsRamadanMoonRing(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_RAMADAN_RING);
		}
		// 할로윈 사탕(71136) 착용시 이펙트 발동
		else if (true == CItemVnumHelper::IsHalloweenCandy(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_HALLOWEEN_CANDY);
		}
		// 행복의 반지(71143) 착용시 이펙트 발동
		else if (true == CItemVnumHelper::IsHappinessRing(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_HAPPINESS_RING);
		}
		// 사랑의 팬던트(71145) 착용시 이펙트 발동
		else if (true == CItemVnumHelper::IsLovePendant(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_LOVE_PENDANT);
		}
		// ITEM_UNIQUE의 경우, SpecialItemGroup에 정의되어 있고, (item->GetSIGVnum() != NULL)
		// 


		if (item->GetType() == ITEM_ARMOR && (item->GetSubType() == ARMOR_EAR || item->GetSubType() == ARMOR_WRIST || item->GetSubType() == ARMOR_NECK))
		{
			int currentvalue = 0;
			int currentcount = 0;
			int braceletvalue = 0;
			int earvalue = 0;
			int necklacevalue = 0;
					
			RemoveAffect(AFFECT_BONUSSET);
			
			LPITEM bracelett = GetWear(WEAR_WRIST);
			LPITEM necklacee = GetWear(WEAR_NECK);
			LPITEM earr = GetWear(WEAR_EAR);
				
			if (bracelett)
			{
				braceletvalue = bracelett->GetValue(1);
			}
			if (necklacee)
			{
				necklacevalue = necklacee->GetValue(1);
			}
			if (earr)
			{
				earvalue = earr->GetValue(1);
			}
					 
			std::map<int, int> m_mapSetItems;
			m_mapSetItems.insert(std::make_pair(braceletvalue, 1));
			std::map<int, int>::iterator iter = m_mapSetItems.find(necklacevalue);
			if (iter == m_mapSetItems.end())
			{
				m_mapSetItems.insert(std::make_pair(necklacevalue, 1));
			}
			else
			{
				iter->second++;
			}
			std::map<int, int>::iterator iter2 = m_mapSetItems.find(earvalue);
			if (iter2 == m_mapSetItems.end())
			{
				m_mapSetItems.insert(std::make_pair(earvalue, 1));
			}
			else
			{
				iter2->second++;
			}
		
			std::map<int, int>::iterator data = m_mapSetItems.begin();
			while (data != m_mapSetItems.end())
			{
				if (currentcount < data->second)
				{
					currentcount = data->second;
					currentvalue = data->first;
				}
				data++;
			}
			int setaffectvalue = AccSet[currentvalue][currentcount];
			int setaffecttype = AccSet[currentvalue][0];
			if (currentcount > 1 && currentvalue !=0)
			{
				AddAffect(AFFECT_BONUSSET, setaffecttype, setaffectvalue, 0, INFINITE_AFFECT_DURATION, 0, true);
			}
			m_mapSetItems.clear();		
		}	
		else if (ITEM_UNIQUE == item->GetType() && 0 != item->GetSIGVnum())
		{
			const CSpecialItemGroup* pGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(item->GetSIGVnum());
			if (NULL != pGroup)
			{
				const CSpecialAttrGroup* pAttrGroup = ITEM_MANAGER::instance().GetSpecialAttrGroup(pGroup->GetAttrVnum(item->GetVnum()));
				if (NULL != pAttrGroup)
				{
					const std::string& std = pAttrGroup->m_stEffectFileName;
					SpecificEffectPacket(std.c_str());
				}
			}
		}
		#ifdef __SASH_SYSTEM__
		else if ((item->GetType() == ITEM_COSTUME) && (item->GetSubType() == COSTUME_SASH))
			this->EffectPacket(SE_EFFECT_SASH_EQUIP);
		#endif

		if (item->IsNewMountItem())
			quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);

		if (UNIQUE_SPECIAL_RIDE == item->GetSubType() && IS_SET(item->GetFlag(), ITEM_FLAG_QUEST_USE))
		{
			quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);
		}
		if (COSTUME_MOUNT == item->GetSubType())
		{
			quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);
		}
		if ((iWearCell == WEAR_BODY) && IsRiding())
		{
			SetQuestFlag("zirh.engel", get_global_time() + 4);	
		}
		if ((iWearCell == WEAR_COSTUME_BODY) && IsRiding())
		{
			SetQuestFlag("zirh.engel1", get_global_time() + 4);	
		}
		if (COSTUME_MOUNT == item->GetSubType() && item->GetType() == ITEM_COSTUME)
		{
			const time_t current = get_global_time();

			if (current > item->GetSocket(0) && item->GetVnum() != 50053)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("suresidolubinek"));
				UnequipItem(GetWear(WEAR_COSTUME_MOUNT));
				return false;
			}
			else
			{
				SetHorseAppearance((item->GetSocket(3) != 0) ? item->GetSocket(3) : item->GetValue(0));
				HorseSummon(true);
			}
		}
		if (USE_PET == item->GetSubType() && item->GetType() == ITEM_UNIQUE)
		{
			// SetPetAppearance(item->GetValue(0));
			SetPetAppearance((item->GetSocket(3) != 0) ? item->GetSocket(3) : item->GetValue(0));
			CPetSystem* petSystem = GetPetSystem();
			if (!petSystem)
			{
				ChatPacket(CHAT_TYPE_INFO, "hata");
				return false;
			}
			CPetActor* pet = petSystem->Summon(GetPetAppearance(), item, " - Pet", false);			
		}
	}

	return true;
}

void CHARACTER::BuffOnAttr_AddBuffsFromItem(LPITEM pItem)
{
	for (int i = 0; i < sizeof(g_aBuffOnAttrPoints)/sizeof(g_aBuffOnAttrPoints[0]); i++)
	{
		TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.find(g_aBuffOnAttrPoints[i]);
		if (it != m_map_buff_on_attrs.end())
		{
			it->second->AddBuffFromItem(pItem);
		}
	}
}

void CHARACTER::BuffOnAttr_RemoveBuffsFromItem(LPITEM pItem)
{
	for (int i = 0; i < sizeof(g_aBuffOnAttrPoints)/sizeof(g_aBuffOnAttrPoints[0]); i++)
	{
		TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.find(g_aBuffOnAttrPoints[i]);
		if (it != m_map_buff_on_attrs.end())
		{
			it->second->RemoveBuffFromItem(pItem);
		}
	}
}

void CHARACTER::BuffOnAttr_ClearAll()
{
	for (TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.begin(); it != m_map_buff_on_attrs.end(); it++)
	{
		CBuffOnAttributes* pBuff = it->second;
		if (pBuff)
		{
			pBuff->Initialize();
		}
	}
}

void CHARACTER::BuffOnAttr_ValueChange(BYTE bType, BYTE bOldValue, BYTE bNewValue)
{
	TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.find(bType);

	if (0 == bNewValue)
	{
		if (m_map_buff_on_attrs.end() == it)
			return;
		else
			it->second->Off();
	}
	else if(0 == bOldValue)
	{
		CBuffOnAttributes* pBuff;
		if (m_map_buff_on_attrs.end() == it)
		{
			switch (bType)
			{
			case POINT_ENERGY:
				{
					static BYTE abSlot[] = { WEAR_BODY, WEAR_HEAD, WEAR_FOOTS, WEAR_WRIST, WEAR_WEAPON, WEAR_NECK, WEAR_EAR, WEAR_SHIELD };
					static std::vector <BYTE> vec_slots (abSlot, abSlot + _countof(abSlot));
					pBuff = M2_NEW CBuffOnAttributes(this, bType, &vec_slots);
				}
				break;
			case POINT_COSTUME_ATTR_BONUS:
				{
					static BYTE abSlot[] = { WEAR_COSTUME_BODY, WEAR_COSTUME_HAIR, WEAR_COSTUME_WEAPON, WEAR_COSTUME_MOUNT };
					static std::vector <BYTE> vec_slots (abSlot, abSlot + _countof(abSlot));
					pBuff = M2_NEW CBuffOnAttributes(this, bType, &vec_slots);
				}
				break;
			default:
				break;
			}
			m_map_buff_on_attrs.insert(TMapBuffOnAttrs::value_type(bType, pBuff));

		}
		else
			pBuff = it->second;
			
		pBuff->On(bNewValue);
	}
	else
	{
		if (m_map_buff_on_attrs.end() == it)
			return;
		else
			it->second->ChangeBuffValue(bNewValue);
	}
}


LPITEM CHARACTER::FindSpecifyItem(DWORD vnum) const
{
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
		if (GetInventoryItem(i) && GetInventoryItem(i)->GetVnum() == vnum)
			return GetInventoryItem(i);

	return NULL;
}

LPITEM CHARACTER::FindItemByID(DWORD id) const
{
	for (int i=0 ; i < INVENTORY_MAX_NUM ; ++i)
	{
		if (NULL != GetInventoryItem(i) && GetInventoryItem(i)->GetID() == id)
			return GetInventoryItem(i);
	}

	for (int i=BELT_INVENTORY_SLOT_START; i < BELT_INVENTORY_SLOT_END ; ++i)
	{
		if (NULL != GetInventoryItem(i) && GetInventoryItem(i)->GetID() == id)
			return GetInventoryItem(i);
	}

	return NULL;
}

int CHARACTER::CountSpecifyItem(DWORD vnum) const
{
	int	count = 0;
	LPITEM item;

	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		item = GetInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			// 개인 상점에 등록된 물건이면 넘어간다.
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
			{
				continue;
			}
			else
			{
				count += item->GetCount();
			}
		}
	}
#ifdef ENABLE_SPECIAL_STORAGE
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		item = GetUpgradeInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
				continue;
			else
				count += item->GetCount();
		}
	}
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		item = GetStoneInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
				continue;
			else
				count += item->GetCount();
		}
	}
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		item = GetChestInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
				continue;
			else
				count += item->GetCount();
		}
	}
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		item = GetAttrInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
				continue;
			else
				count += item->GetCount();
		}
	}
#endif

	return count;
}

void CHARACTER::RemoveSpecifyItem(DWORD vnum, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		if (NULL == GetInventoryItem(i))
			continue;

		if (GetInventoryItem(i)->GetVnum() != vnum)
			continue;

		//개인 상점에 등록된 물건이면 넘어간다. (개인 상점에서 판매될때 이 부분으로 들어올 경우 문제!)
		if(m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (vnum >= 80003 && vnum <= 80007)
			LogManager::instance().GoldBarLog(GetPlayerID(), GetInventoryItem(i)->GetID(), QUEST, "RemoveSpecifyItem");

		if (count >= GetInventoryItem(i)->GetCount())
		{
			count -= GetInventoryItem(i)->GetCount();
			GetInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetInventoryItem(i)->SetCount(GetInventoryItem(i)->GetCount() - count);
			return;
		}
	}
#ifdef ENABLE_SPECIAL_STORAGE
	for (UINT i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if (NULL == GetUpgradeInventoryItem(i))
			continue;

		if (GetUpgradeInventoryItem(i)->GetVnum() != vnum)
			continue;

		if(m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetUpgradeInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetUpgradeInventoryItem(i)->GetCount())
		{
			count -= GetUpgradeInventoryItem(i)->GetCount();
			GetUpgradeInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetUpgradeInventoryItem(i)->SetCount(GetUpgradeInventoryItem(i)->GetCount() - count);
			return;
		}
	}
	for (UINT i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if (NULL == GetStoneInventoryItem(i))
			continue;

		if (GetStoneInventoryItem(i)->GetVnum() != vnum)
			continue;

		if(m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetStoneInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetStoneInventoryItem(i)->GetCount())
		{
			count -= GetStoneInventoryItem(i)->GetCount();
			GetStoneInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetStoneInventoryItem(i)->SetCount(GetStoneInventoryItem(i)->GetCount() - count);
			return;
		}
	}
	for (UINT i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if (NULL == GetChestInventoryItem(i))
			continue;

		if (GetChestInventoryItem(i)->GetVnum() != vnum)
			continue;

		if(m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetChestInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetChestInventoryItem(i)->GetCount())
		{
			count -= GetChestInventoryItem(i)->GetCount();
			GetChestInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetChestInventoryItem(i)->SetCount(GetChestInventoryItem(i)->GetCount() - count);
			return;
		}
	}
	for (UINT i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if (NULL == GetAttrInventoryItem(i))
			continue;

		if (GetAttrInventoryItem(i)->GetVnum() != vnum)
			continue;

		if(m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetAttrInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetAttrInventoryItem(i)->GetCount())
		{
			count -= GetAttrInventoryItem(i)->GetCount();
			GetAttrInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetAttrInventoryItem(i)->SetCount(GetAttrInventoryItem(i)->GetCount() - count);
			return;
		}
	}
#endif
	// 예외처리가 약하다.
	if (count)
		sys_log(0, "CHARACTER::RemoveSpecifyItem cannot remove enough item vnum %u, still remain %d", vnum, count);
}

int CHARACTER::CountSpecifyTypeItem(BYTE type) const
{
	int	count = 0;

	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		LPITEM pItem = GetInventoryItem(i);
		if (pItem != NULL && pItem->GetType() == type)
		{
			count += pItem->GetCount();
		}
	}

	return count;
}

void CHARACTER::RemoveSpecifyTypeItem(BYTE type, short count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		if (NULL == GetInventoryItem(i))
			continue;

		if (GetInventoryItem(i)->GetType() != type)
			continue;

		//개인 상점에 등록된 물건이면 넘어간다. (개인 상점에서 판매될때 이 부분으로 들어올 경우 문제!)
		if(m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetInventoryItem(i)->GetCount())
		{
			count -= GetInventoryItem(i)->GetCount();
			GetInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetInventoryItem(i)->SetCount(GetInventoryItem(i)->GetCount() - count);
			return;
		}
	}
}

TItemPos CHARACTER::FindSpecifyItemPos(DWORD vnum) const
{
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		LPITEM pItem = GetInventoryItem(i);
		if (pItem != NULL && pItem->GetVnum() == vnum)
		{
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(pItem->GetID()))
				continue;
			else if (pItem->isLocked() || pItem->IsExchanging())
				continue;
			else
				return TItemPos(INVENTORY, i);
		}
	}

#ifdef ENABLE_SPECIAL_STORAGE
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		LPITEM pItem = GetStoneInventoryItem(i);
		if (pItem != NULL && pItem->GetVnum() == vnum)
		{
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(pItem->GetID()))
				continue;
			else if (pItem->isLocked() || pItem->IsExchanging())
				continue;
			else
				return TItemPos(STONE_INVENTORY, i);
		}
	}

	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		LPITEM pItem = GetUpgradeInventoryItem(i);
		if (pItem != NULL && pItem->GetVnum() == vnum)
		{
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(pItem->GetID()))
				continue;
			else if (pItem->isLocked() || pItem->IsExchanging())
				continue;
			else
				return TItemPos(UPGRADE_INVENTORY, i);
		}
	}
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		LPITEM pItem = GetChestInventoryItem(i);
		if (pItem != NULL && pItem->GetVnum() == vnum)
		{
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(pItem->GetID()))
				continue;
			else if (pItem->isLocked() || pItem->IsExchanging())
				continue;
			else
				return TItemPos(CHEST_INVENTORY, i);
		}
	}
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		LPITEM pItem = GetAttrInventoryItem(i);
		if (pItem != NULL && pItem->GetVnum() == vnum)
		{
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(pItem->GetID()))
				continue;
			else if (pItem->isLocked() || pItem->IsExchanging())
				continue;
			else
				return TItemPos(ATTR_INVENTORY, i);
		}
	}
#endif

	return NPOS;
}
void CHARACTER::AutoGiveItem(LPITEM item, bool longOwnerShip)
{
	if (NULL == item)
	{
		sys_err ("NULL point.");
		return;
	}
	if (item->GetOwner())
	{
		sys_err ("item %d 's owner exists!",item->GetID());
		return;
	}
	
	int cell;
	if (item->IsDragonSoul())
	{
		cell = GetEmptyDragonSoulInventory(item);
	}
#ifdef ENABLE_SPECIAL_STORAGE
	else if (item->IsUpgradeItem())
	{
		cell = GetEmptyUpgradeInventory(item);
	}
	else if (item->IsStone())
	{
		cell = GetEmptyStoneInventory(item);
	}
	else if (item->IsChest())
	{
		cell = GetEmptyChestInventory(item);
	}
	else if (item->IsAttr())
	{
		cell = GetEmptyAttrInventory(item);
	}
#endif
	else
	{
		cell = GetEmptyInventory (item->GetSize());
	}

	if (cell != -1)
	{
		if (item->IsDragonSoul())
			item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, cell));
#ifdef ENABLE_SPECIAL_STORAGE
		else if (item->IsUpgradeItem())
			item->AddToCharacter(this, TItemPos(UPGRADE_INVENTORY, cell));
		else if (item->IsStone())
			item->AddToCharacter(this, TItemPos(STONE_INVENTORY, cell));
		else if (item->IsChest())
			item->AddToCharacter(this, TItemPos(CHEST_INVENTORY, cell));
		else if (item->IsAttr())
			item->AddToCharacter(this, TItemPos(ATTR_INVENTORY, cell));
#endif
		else
			item->AddToCharacter(this, TItemPos(INVENTORY, cell));

		LogManager::instance().ItemLog(this, item, "SYSTEM", item->GetName());

		if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION)
		{
			TQuickslot * pSlot;

			if (GetQuickslot(0, &pSlot) && pSlot->type == QUICKSLOT_TYPE_NONE)
			{
				TQuickslot slot;
				slot.type = QUICKSLOT_TYPE_ITEM;
				slot.pos = cell;
				SetQuickslot(0, slot);
			}
		}
	}
	else
	{
		item->AddToGround (GetMapIndex(), GetXYZ());
		item->StartDestroyEvent();

		if (longOwnerShip)
			item->SetOwnership (this, 300);
		else
			item->SetOwnership (this, 60);
		LogManager::instance().ItemLog(this, item, "SYSTEM_DROP", item->GetName());
	}
}

LPITEM CHARACTER::AutoGiveItem(DWORD dwItemVnum, short bCount, int iRarePct, bool bMsg)
{
	TItemTable * p = ITEM_MANAGER::instance().GetTable(dwItemVnum);

	if (!p)
		return NULL;

	DBManager::instance().SendMoneyLog(MONEY_LOG_DROP, dwItemVnum, bCount);

	if (p->dwFlags & ITEM_FLAG_STACKABLE && p->bType != ITEM_BLEND) 
	{
#ifdef ENABLE_SPECIAL_STORAGE
		if (p->bType == ITEM_MATERIAL && p->bSubType == MATERIAL_LEATHER) //upgrade item
		{
			for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item = GetUpgradeInventoryItem(i);
				
				if (!item)
					continue;
				
				if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
				{
					if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
					{
						if (bCount < p->alValues[1])
							bCount = p->alValues[1];
					}

					short bCount2 = MIN(ITEM_MAX_COUNT - item->GetCount(), bCount);
					bCount -= bCount2;

					item->SetCount(item->GetCount() + bCount2);

					if (bCount == 0)
					{
						if (bMsg)
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("?????? ??μ?: %s"), item->GetName());

						return item;
					}
				}
			}
		}
		else if (dwItemVnum == 50300) //book item
		{
		}
		else if (p->bType == ITEM_METIN && p->bSubType == METIN_NORMAL) //stone item
		{
			for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item = GetStoneInventoryItem(i);
				
				if (!item)
					continue;
				
				if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
				{
					if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
					{
						if (bCount < p->alValues[1])
							bCount = p->alValues[1];
					}

					short bCount2 = MIN(ITEM_MAX_COUNT - item->GetCount(), bCount);
					bCount -= bCount2;

					item->SetCount(item->GetCount() + bCount2);

					if (bCount == 0)
					{
						if (bMsg)
#if defined(__CHATTING_WINDOW_RENEWAL__)
							ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#else
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#endif

						return item;
					}
				}
			}
		}
		else if (p->bType == ITEM_GIFTBOX || dwItemVnum == 30300) //chest item
		{
			for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item = GetChestInventoryItem(i);
				
				if (!item)
					continue;
				
				if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
				{
					if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
					{
						if (bCount < p->alValues[1])
							bCount = p->alValues[1];
					}

					short bCount2 = MIN(ITEM_MAX_COUNT - item->GetCount(), bCount);
					bCount -= bCount2;

					item->SetCount(item->GetCount() + bCount2);

					if (bCount == 0)
					{
						if (bMsg)
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("?????? ??μ?: %s"), item->GetName());

						return item;
					}
				}
			}
		}
		else if (dwItemVnum == 71084) //attr item
		{
			for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item = GetAttrInventoryItem(i);
				
				if (!item)
					continue;
				
				if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
				{
					if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
					{
						if (bCount < p->alValues[1])
							bCount = p->alValues[1];
					}

					short bCount2 = MIN(ITEM_MAX_COUNT - item->GetCount(), bCount);
					bCount -= bCount2;

					item->SetCount(item->GetCount() + bCount2);

					if (bCount == 0)
					{
						if (bMsg)
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("?????? ??μ?: %s"), item->GetName());

						return item;
					}
				}
			}
		}
		else
		{
#endif
		for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
		{
			LPITEM item = GetInventoryItem(i);

			if (!item)
				continue;

			if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (bCount < p->alValues[1])
						bCount = p->alValues[1];
				}

				short bCount2 = MIN(ITEM_MAX_COUNT - item->GetCount(), bCount);
				bCount -= bCount2;

				item->SetCount(item->GetCount() + bCount2);

				if (bCount == 0)
				{
					if (bMsg)
#if defined(__CHATTING_WINDOW_RENEWAL__)
						ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#else
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#endif

					return item;
				}
			}
		}
	}
#ifdef ENABLE_SPECIAL_STORAGE
		}
#endif
	LPITEM item = ITEM_MANAGER::instance().CreateItem(dwItemVnum, bCount, 0, true);

	if (!item)
	{
		sys_err("cannot create item by vnum %u (name: %s)", dwItemVnum, GetName());
		return NULL;
	}

	if (item->GetType() == ITEM_BLEND)
	{
		for (int i=0; i < INVENTORY_MAX_NUM; i++)
		{
			LPITEM inv_item = GetInventoryItem(i);

			if (inv_item == NULL) continue;

			if (inv_item->GetType() == ITEM_BLEND)
			{
				if (inv_item->GetVnum() == item->GetVnum())
				{
					if (inv_item->GetSocket(0) == item->GetSocket(0) &&
							inv_item->GetSocket(1) == item->GetSocket(1) &&
							inv_item->GetSocket(2) == item->GetSocket(2) &&
							inv_item->GetCount() < ITEM_MAX_COUNT)
					{
						inv_item->SetCount(inv_item->GetCount() + item->GetCount());
						return inv_item;
					}
				}
			}
		}
	}

	int iEmptyCell;
	if (item->IsDragonSoul())
	{
		iEmptyCell = GetEmptyDragonSoulInventory(item);
	}
#ifdef ENABLE_SPECIAL_STORAGE
	else if (item->IsUpgradeItem())
	{
		iEmptyCell = GetEmptyUpgradeInventory(item);
	}
	else if (item->IsStone())
	{
		iEmptyCell = GetEmptyStoneInventory(item);
	}
	else if (item->IsChest())
	{
		iEmptyCell = GetEmptyChestInventory(item);
	}
	else if (item->IsAttr())
	{
		iEmptyCell = GetEmptyAttrInventory(item);
	}
#endif
	else
		iEmptyCell = GetEmptyInventory(item->GetSize());

	if (iEmptyCell != -1)
	{
		if (bMsg)
#if defined(__CHATTING_WINDOW_RENEWAL__)
			ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#else
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#endif

		if (item->IsDragonSoul())
			item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyCell));
#ifdef ENABLE_SPECIAL_STORAGE
		else if (item->IsUpgradeItem())
		{
			item->AddToCharacter(this, TItemPos(UPGRADE_INVENTORY, iEmptyCell));
		}
		else if (item->IsStone())
		{
			item->AddToCharacter(this, TItemPos(STONE_INVENTORY, iEmptyCell));
		}
		else if (item->IsChest())
		{
			item->AddToCharacter(this, TItemPos(CHEST_INVENTORY, iEmptyCell));
		}
		else if (item->IsAttr())
		{
			item->AddToCharacter(this, TItemPos(ATTR_INVENTORY, iEmptyCell));
		}
#endif
		else
			item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));
		LogManager::instance().ItemLog(this, item, "SYSTEM", item->GetName());

		if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION)
		{
			TQuickslot * pSlot;

			if (GetQuickslot(0, &pSlot) && pSlot->type == QUICKSLOT_TYPE_NONE)
			{
				TQuickslot slot;
				slot.type = QUICKSLOT_TYPE_ITEM;
				slot.pos = iEmptyCell;
				SetQuickslot(0, slot);
			}
		}
	}
	else
	{
		item->AddToGround(GetMapIndex(), GetXYZ());
		item->StartDestroyEvent();
		// 안티 드랍 flag가 걸려있는 아이템의 경우, 
		// 인벤에 빈 공간이 없어서 어쩔 수 없이 떨어트리게 되면,
		// ownership을 아이템이 사라질 때까지(300초) 유지한다.
		if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_DROP))
			item->SetOwnership(this, 300);
		else
			item->SetOwnership(this, 60);
		LogManager::instance().ItemLog(this, item, "SYSTEM_DROP", item->GetName());
	}

	sys_log(0, 
		"7: %d %d", dwItemVnum, bCount);
	return item;
}

LPITEM CHARACTER::GiveItem(DWORD dwItemVnum, short bCount, int iRarePct, bool bMsg, bool bagli)
{
	if (!CanHandleItem())
		return NULL;

	TItemTable * p = ITEM_MANAGER::instance().GetTable(dwItemVnum);

	if (!p)
		return NULL;

	DBManager::instance().SendMoneyLog(MONEY_LOG_DROP, dwItemVnum, bCount);

	if (p->dwFlags & ITEM_FLAG_STACKABLE && p->bType != ITEM_BLEND) 
	{
#ifdef ENABLE_SPECIAL_STORAGE
		// if (p->bType == ITEM_MATERIAL && p->bSubType == MATERIAL_LEATHER) //upgrade item
		if (p->IsUpgradeItem()) //upgrade item
		{
			for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item = GetUpgradeInventoryItem(i);
				
				if (!item)
					continue;
				
				if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
				{
					if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
					{
						if (bCount < p->alValues[1])
							bCount = p->alValues[1];
					}

					short bCount2 = MIN(ITEM_MAX_COUNT - item->GetCount(), bCount);
					bCount -= bCount2;

					item->SetCount(item->GetCount() + bCount2);

					if (bCount == 0)
					{
						if (bMsg)
#if defined(__CHATTING_WINDOW_RENEWAL__)
							ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#else
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#endif

						return item;
					}
				}
			}
		}
		// else if (p->bType == ITEM_METIN && p->bSubType == METIN_NORMAL) //stone item
		else if (p->IsStone()) //stone item
		{
			for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item = GetStoneInventoryItem(i);
				
				if (!item)
					continue;
				
				if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
				{
					if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
					{
						if (bCount < p->alValues[1])
							bCount = p->alValues[1];
					}

					short bCount2 = MIN(ITEM_MAX_COUNT - item->GetCount(), bCount);
					bCount -= bCount2;

					item->SetCount(item->GetCount() + bCount2);

					if (bCount == 0)
					{
						if (bMsg)
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 아이템 을 획득하셨습니다"), item->GetName());

						return item;
					}
				}
			}
		}
		else if (p->IsChest()) //chest item
		{
			for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item = GetChestInventoryItem(i);
				
				if (!item)
					continue;
				
				if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
				{
					if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
					{
						if (bCount < p->alValues[1])
							bCount = p->alValues[1];
					}

					short bCount2 = MIN(ITEM_MAX_COUNT - item->GetCount(), bCount);
					bCount -= bCount2;

					item->SetCount(item->GetCount() + bCount2);

					if (bCount == 0)
					{
						if (bMsg)
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 아이템 을 획득하셨습니다"), item->GetName());

						return item;
					}
				}
			}
		}
		else if (p->IsAttr()) //attr item
		{
			for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
			{
				LPITEM item = GetAttrInventoryItem(i);
				
				if (!item)
					continue;
				
				if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
				{
					if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
					{
						if (bCount < p->alValues[1])
							bCount = p->alValues[1];
					}

					short bCount2 = MIN(ITEM_MAX_COUNT - item->GetCount(), bCount);
					bCount -= bCount2;

					item->SetCount(item->GetCount() + bCount2);

					if (bCount == 0)
					{
						if (bMsg)
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 아이템 을 획득하셨습니다"), item->GetName());

						return item;
					}
				}
			}
		}
		else
		{
#endif
		for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
		{
			LPITEM item = GetInventoryItem(i);

			if (!item)
				continue;

			if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (bCount < p->alValues[1])
						bCount = p->alValues[1];
				}

				short bCount2 = MIN(ITEM_MAX_COUNT - item->GetCount(), bCount);
				bCount -= bCount2;

				item->SetCount(item->GetCount() + bCount2);

				if (bCount == 0)
				{
					if (bMsg)
#if defined(__CHATTING_WINDOW_RENEWAL__)
						ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#else
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#endif

					return item;
				}
			}
		}
	}
#ifdef ENABLE_SPECIAL_STORAGE
		}
#endif
	LPITEM item = ITEM_MANAGER::instance().CreateItem(dwItemVnum, bCount, 0, true);

	if (!item)
	{
		sys_err("cannot create item by vnum %u (name: %s)", dwItemVnum, GetName());
		return NULL;
	}
#ifdef ENABLE_SPECIAL_STORAGE
	if (item->IsUpgradeItem())
	{
		for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		{
			LPITEM upgrrde = GetUpgradeInventoryItem(i);
			
			if (!upgrrde)
				continue;
			
			if (upgrrde->GetVnum() == dwItemVnum && upgrrde->GetSocket(0) == item->GetSocket(0))
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (bCount < p->alValues[1])
						bCount = p->alValues[1];
				}

				short bCount2 = MIN(ITEM_MAX_COUNT - upgrrde->GetCount(), bCount);
				bCount -= bCount2;

				upgrrde->SetCount(upgrrde->GetCount() + bCount2);

				if (bCount == 0)
				{
					if (bMsg)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 아이템 을 획득하셨습니다"), upgrrde->GetName());

					M2_DESTROY_ITEM(item);
					return upgrrde;
				}
			}
		}
	}
	else if (item->IsStone())
	{
		for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		{
			LPITEM stoneeee = GetStoneInventoryItem(i);
			
			if (!stoneeee)
				continue;
			
			if (stoneeee->GetVnum() == dwItemVnum && stoneeee->GetSocket(0) == item->GetSocket(0))
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (bCount < p->alValues[1])
						bCount = p->alValues[1];
				}

				short bCount2 = MIN(ITEM_MAX_COUNT - stoneeee->GetCount(), bCount);
				bCount -= bCount2;

				stoneeee->SetCount(stoneeee->GetCount() + bCount2);

				if (bCount == 0)
				{
					if (bMsg)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 아이템 을 획득하셨습니다"), stoneeee->GetName());

					M2_DESTROY_ITEM(item);
					return stoneeee;
				}
			}
		}
	}
	else if (item->IsChest())
	{
		for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		{
			LPITEM chesttt = GetChestInventoryItem(i);
			
			if (!chesttt)
				continue;
			
			if (chesttt->GetVnum() == dwItemVnum && chesttt->GetSocket(0) == item->GetSocket(0))
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (bCount < p->alValues[1])
						bCount = p->alValues[1];
				}

				short bCount2 = MIN(ITEM_MAX_COUNT - chesttt->GetCount(), bCount);
				bCount -= bCount2;

				chesttt->SetCount(chesttt->GetCount() + bCount2);

				if (bCount == 0)
				{
					if (bMsg)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 아이템 을 획득하셨습니다"), chesttt->GetName());

					M2_DESTROY_ITEM(item);
					return chesttt;
				}
			}
		}
	}
	else if (item->IsAttr())
	{
		for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		{
			LPITEM attrrr = GetAttrInventoryItem(i);
			
			if (!attrrr)
				continue;
			
			if (attrrr->GetVnum() == dwItemVnum && attrrr->GetSocket(0) == item->GetSocket(0))
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (bCount < p->alValues[1])
						bCount = p->alValues[1];
				}

				short bCount2 = MIN(ITEM_MAX_COUNT - attrrr->GetCount(), bCount);
				bCount -= bCount2;

				attrrr->SetCount(attrrr->GetCount() + bCount2);

				if (bCount == 0)
				{
					if (bMsg)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 아이템 을 획득하셨습니다"), attrrr->GetName());

					M2_DESTROY_ITEM(item);
					return attrrr;
				}
			}
		}
	}
#endif

	if (item->GetType() == ITEM_BLEND)
	{
		for (int i=0; i < INVENTORY_MAX_NUM; i++)
		{
			LPITEM inv_item = GetInventoryItem(i);

			if (inv_item == NULL) continue;

			if (inv_item->GetType() == ITEM_BLEND)
			{
				if (inv_item->GetVnum() == item->GetVnum())
				{
					if (inv_item->GetSocket(0) == item->GetSocket(0) &&
							inv_item->GetSocket(1) == item->GetSocket(1) &&
							inv_item->GetSocket(2) == item->GetSocket(2) &&
							inv_item->GetCount() < ITEM_MAX_COUNT)
					{
						inv_item->SetCount(inv_item->GetCount() + item->GetCount());
						M2_DESTROY_ITEM(item); ////memoryleak
						return inv_item;
					}
				}
			}
		}
	}

	int iEmptyCell;
	if (item->IsDragonSoul())
	{
		iEmptyCell = GetEmptyDragonSoulInventory(item);
	}
#ifdef ENABLE_SPECIAL_STORAGE
	else if (item->IsUpgradeItem())
	{
		iEmptyCell = GetEmptyUpgradeInventory(item);
	}
	else if (item->IsStone())
	{
		iEmptyCell = GetEmptyStoneInventory(item);
	}
	else if (item->IsChest())
	{
		iEmptyCell = GetEmptyChestInventory(item);
	}
	else if (item->IsAttr())
	{
		iEmptyCell = GetEmptyAttrInventory(item);
	}
#endif
	else
		iEmptyCell = GetEmptyInventory(item->GetSize());

	if (iEmptyCell != -1)
	{
		if (bMsg)
#if defined(__CHATTING_WINDOW_RENEWAL__)
			ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#else
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아이템 획득: %s"), item->GetName());
#endif

		if (item->IsDragonSoul())
			item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyCell));
#ifdef ENABLE_SPECIAL_STORAGE
		else if (item->IsUpgradeItem())
		{
			item->AddToCharacter(this, TItemPos(UPGRADE_INVENTORY, iEmptyCell));
		}
		else if (item->IsStone())
		{
			item->AddToCharacter(this, TItemPos(STONE_INVENTORY, iEmptyCell));
		}
		else if (item->IsChest())
		{
			item->AddToCharacter(this, TItemPos(CHEST_INVENTORY, iEmptyCell));
		}
		else if (item->IsAttr())
		{
			item->AddToCharacter(this, TItemPos(ATTR_INVENTORY, iEmptyCell));
		}
#endif
		else
			item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));
		LogManager::instance().ItemLog(this, item, "SYSTEM", item->GetName());

		if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION)
		{
			TQuickslot * pSlot;

			if (GetQuickslot(0, &pSlot) && pSlot->type == QUICKSLOT_TYPE_NONE)
			{
				TQuickslot slot;
				slot.type = QUICKSLOT_TYPE_ITEM;
				slot.pos = iEmptyCell;
				SetQuickslot(0, slot);
			}
		}
	}
	else
	{
		item->AddToGround(GetMapIndex(), GetXYZ());
		item->StartDestroyEvent();
		// 안티 드랍 flag가 걸려있는 아이템의 경우, 
		// 인벤에 빈 공간이 없어서 어쩔 수 없이 떨어트리게 되면,
		// ownership을 아이템이 사라질 때까지(300초) 유지한다.
		if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_DROP))
			item->SetOwnership(this, 300);
		else
			item->SetOwnership(this, 60);
		LogManager::instance().ItemLog(this, item, "SYSTEM_DROP", item->GetName());
	}
	return item;
}

bool CHARACTER::GiveItem(LPCHARACTER victim, TItemPos Cell)
{
	if (!CanHandleItem())
		return false;
	if (quest::CQuestManager::instance().GetPCForce(GetPlayerID())->IsRunning() == true)
	{
		ChatPacket(CHAT_TYPE_INFO, "Quest penceresi acikken ogeyi alamazsin.");
		return false;
	}
	LPITEM item = GetItem(Cell);

	if (item && !item->IsExchanging())
	{
		if (victim->CanReceiveItem(this, item))
		{
			victim->ReceiveItem(this, item);
			return true;
		}
	}

	return false;
}

bool CHARACTER::CanReceiveItem(LPCHARACTER from, LPITEM item) const
{
	if (IsPC())
		return false;

	// TOO_LONG_DISTANCE_EXCHANGE_BUG_FIX
	if (DISTANCE_APPROX(GetX() - from->GetX(), GetY() - from->GetY()) > 2000)
		return false;
	// END_OF_TOO_LONG_DISTANCE_EXCHANGE_BUG_FIX

	switch (GetRaceNum())
	{
		case fishing::CAMPFIRE_MOB:
			if (item->GetType() == ITEM_FISH && 
					(item->GetSubType() == FISH_ALIVE || item->GetSubType() == FISH_DEAD))
				return true;
			break;

		case fishing::FISHER_MOB:
			if (item->GetType() == ITEM_ROD)
				return true;
			break;

			// BUILDING_NPC
		case BLACKSMITH_WEAPON_MOB:
		case DEVILTOWER_BLACKSMITH_WEAPON_MOB:
			if (item->GetType() == ITEM_WEAPON && 
					item->GetRefinedVnum())
				return true;
			else
				return false;
			break;

		case BLACKSMITH_ARMOR_MOB:
		case DEVILTOWER_BLACKSMITH_ARMOR_MOB:
			if (item->GetType() == ITEM_ARMOR && 
					(item->GetSubType() == ARMOR_BODY || item->GetSubType() == ARMOR_SHIELD || item->GetSubType() == ARMOR_HEAD) &&
					item->GetRefinedVnum())
				return true;
			else
				return false;
			break;

		case BLACKSMITH_ACCESSORY_MOB:
		case DEVILTOWER_BLACKSMITH_ACCESSORY_MOB:
			if (item->GetType() == ITEM_ARMOR &&
					!(item->GetSubType() == ARMOR_BODY || item->GetSubType() == ARMOR_SHIELD || item->GetSubType() == ARMOR_HEAD) &&
					item->GetRefinedVnum())
				return true;
			else
				return false;
			break;
			// END_OF_BUILDING_NPC

		case BLACKSMITH_MOB:
			if (item->GetRefinedVnum() && item->GetRefineSet() < 500)
			{
				return true;
			}
			else
			{
				return false;
			}

		case BLACKSMITH2_MOB:
			if (item->GetRefineSet() >= 500)
			{
				return true;
			}
			else
			{
				return false;
			}

		case ALCHEMIST_MOB:
			if (item->GetRefinedVnum())
				return true;
			break;

		case 20101:
		case 20102:
		case 20103:
			// 초급 말
			if (item->GetVnum() == ITEM_REVIVE_HORSE_1)
			{
				if (!IsDead())
				{
					from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("죽지 않은 말에게 선초를 먹일 수 없습니다."));
					return false;
				}
				return true;
			}
			else if (item->GetVnum() == ITEM_HORSE_FOOD_1)
			{
				if (IsDead())
				{
					from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("죽은 말에게 사료를 먹일 수 없습니다."));
					return false;
				}
				return true;
			}
			else if (item->GetVnum() == ITEM_HORSE_FOOD_2 || item->GetVnum() == ITEM_HORSE_FOOD_3)
			{
				return false;
			}
			break;
		case 20104:
		case 20105:
		case 20106:
			// 중급 말
			if (item->GetVnum() == ITEM_REVIVE_HORSE_2)
			{
				if (!IsDead())
				{
					from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("죽지 않은 말에게 선초를 먹일 수 없습니다."));
					return false;
				}
				return true;
			}
			else if (item->GetVnum() == ITEM_HORSE_FOOD_2)
			{
				if (IsDead())
				{
					from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("죽은 말에게 사료를 먹일 수 없습니다."));
					return false;
				}
				return true;
			}
			else if (item->GetVnum() == ITEM_HORSE_FOOD_1 || item->GetVnum() == ITEM_HORSE_FOOD_3)
			{
				return false;
			}
			break;
		case 20107:
		case 20108:
		case 20109:
			// 고급 말
			if (item->GetVnum() == ITEM_REVIVE_HORSE_3)
			{
				if (!IsDead())
				{
					from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("죽지 않은 말에게 선초를 먹일 수 없습니다."));
					return false;
				}
				return true;
			}
			else if (item->GetVnum() == ITEM_HORSE_FOOD_3)
			{
				if (IsDead())
				{
					from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("죽은 말에게 사료를 먹일 수 없습니다."));
					return false;
				}
				return true;
			}
			else if (item->GetVnum() == ITEM_HORSE_FOOD_1 || item->GetVnum() == ITEM_HORSE_FOOD_2)
			{
				return false;
			}
			break;
		break;
	}

	//if (IS_SET(item->GetFlag(), ITEM_FLAG_QUEST_GIVE))
	{
		return true;
	}

	return false;
}

void CHARACTER::ReceiveItem(LPCHARACTER from, LPITEM item)
{
	if (IsPC())
		return;
	
	if (GetMobRank() >= MOB_RANK_BOSS)
	{
		if (item->GetVnum() == 27996)
		{
			
			if (GetExchange() || GetMyShop() || GetShopOwner() || IsOpenSafebox() || IsCubeOpen() || GetOfflineShopOwner())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("거래창,창고 등을 연 상태에서는 귀환부,귀환기억부 를 사용할수 없습니다."));
				return;
			}
			
			if (IsSashCombinationOpen())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("kombinasyon_penceresi_acik"));
				return;
			}
			
			if (IsSashAbsorptionOpen())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bonus_emis_penceresi_acik"));
				return;
			}
			
			
			if (IsPoisoned())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("boss_zaten_zehiryemis"));
				return;
			}
			
			AttackedByPoison(NULL);
			item->SetCount(item->GetCount()-1);
			from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("boss_zehirlendi"));
		}
	}
	
	switch (GetRaceNum())
	{
		case fishing::CAMPFIRE_MOB:
			if (item->GetType() == ITEM_FISH && (item->GetSubType() == FISH_ALIVE || item->GetSubType() == FISH_DEAD))
				fishing::Grill(from, item);
			else
			{
				// TAKE_ITEM_BUG_FIX
				from->SetQuestNPCID(GetVID());
				// END_OF_TAKE_ITEM_BUG_FIX
				quest::CQuestManager::instance().TakeItem(from->GetPlayerID(), GetRaceNum(), item);
			}
			break;

			// DEVILTOWER_NPC 
		case DEVILTOWER_BLACKSMITH_WEAPON_MOB:
		case DEVILTOWER_BLACKSMITH_ARMOR_MOB:
		case DEVILTOWER_BLACKSMITH_ACCESSORY_MOB:
			if (item->GetRefinedVnum() != 0 && item->GetRefineSet() != 0 && item->GetRefineSet() < 500)
			{
				from->SetRefineNPC(this);
				from->RefineInformation(item->GetCell(), REFINE_TYPE_MONEY_ONLY);
			}
			else
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 아이템은 개량할 수 없습니다."));
			}
			break;
			// END_OF_DEVILTOWER_NPC

		case BLACKSMITH_MOB:
		case BLACKSMITH2_MOB:
		case BLACKSMITH_WEAPON_MOB:
		case BLACKSMITH_ARMOR_MOB:
		case BLACKSMITH_ACCESSORY_MOB:
			if (item->GetRefinedVnum())
			{
				from->SetRefineNPC(this);
				from->RefineInformation(item->GetCell(), REFINE_TYPE_NORMAL);
			}
			else
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이 아이템은 개량할 수 없습니다."));
			}
			break;

		case 20101:
		case 20102:
		case 20103:
		case 20104:
		case 20105:
		case 20106:
		case 20107:
		case 20108:
		case 20109:
			if (item->GetVnum() == ITEM_REVIVE_HORSE_1 ||
					item->GetVnum() == ITEM_REVIVE_HORSE_2 ||
					item->GetVnum() == ITEM_REVIVE_HORSE_3)
			{
				from->ReviveHorse();
				item->SetCount(item->GetCount()-1);
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("말에게 선초를 주었습니다."));
			}
			else if (item->GetVnum() == ITEM_HORSE_FOOD_1 ||
					item->GetVnum() == ITEM_HORSE_FOOD_2 ||
					item->GetVnum() == ITEM_HORSE_FOOD_3)
			{
				from->FeedHorse();
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("말에게 사료를 주었습니다."));
				item->SetCount(item->GetCount()-1);
				EffectPacket(SE_HPUP_RED);
			}
			break;

		default:
			sys_log(0, "TakeItem %s %d %s", from->GetName(), GetRaceNum(), item->GetName());
			from->SetQuestNPCID(GetVID());
			quest::CQuestManager::instance().TakeItem(from->GetPlayerID(), GetRaceNum(), item);
			break;
	}
}

bool CHARACTER::IsEquipUniqueItem(DWORD dwItemVnum) const
{
	{
		LPITEM u = GetWear(WEAR_UNIQUE1);

		if (u && u->GetVnum() == dwItemVnum)
			return true;
	}

	{
		LPITEM u = GetWear(WEAR_UNIQUE2);

		if (u && u->GetVnum() == dwItemVnum)
			return true;
	}

	// 언어반지인 경우 언어반지(견본) 인지도 체크한다.
	{
		LPITEM u = GetWear(WEAR_COSTUME_MOUNT);

		if (u && u->GetVnum() == dwItemVnum)
			return true;
	}
	if (dwItemVnum == UNIQUE_ITEM_RING_OF_LANGUAGE)
		return IsEquipUniqueItem(UNIQUE_ITEM_RING_OF_LANGUAGE_SAMPLE);

	return false;
}

// CHECK_UNIQUE_GROUP
bool CHARACTER::IsEquipUniqueGroup(DWORD dwGroupVnum) const
{
	{
		LPITEM u = GetWear(WEAR_UNIQUE1);

		if (u && u->GetSpecialGroup() == (int) dwGroupVnum)
			return true;
	}

	{
		LPITEM u = GetWear(WEAR_UNIQUE2);

		if (u && u->GetSpecialGroup() == (int)dwGroupVnum)
			return true;
	}

	{
		LPITEM u = GetWear(WEAR_COSTUME_MOUNT);

		if (u && u->GetSpecialGroup() == (int)dwGroupVnum)
			return true;
	}

	return false;
}
// END_OF_CHECK_UNIQUE_GROUP

void CHARACTER::SetRefineMode(int iAdditionalCell)
{
	m_iRefineAdditionalCell = iAdditionalCell;
	m_bUnderRefine = true;
}

void CHARACTER::ClearRefineMode()
{
	m_bUnderRefine = false;
	SetRefineNPC( NULL );
}

bool CHARACTER::GiveItemFromSpecialItemGroup(DWORD dwGroupNum, std::vector<DWORD> &dwItemVnums, 
											std::vector<DWORD> &dwItemCounts, std::vector <LPITEM> &item_gets, int &count)
{
	const CSpecialItemGroup* pGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(dwGroupNum);

	if (!pGroup)
	{
		sys_err("cannot find special item group %d", dwGroupNum);
		return false;
	}

	std::vector <int> idxes;
	int n = pGroup->GetMultiIndex(idxes);

	bool bSuccess;

	for (int i = 0; i < n; i++)
	{
		bSuccess = false;
		int idx = idxes[i];
		DWORD dwVnum = pGroup->GetVnum(idx);
		DWORD dwCount = pGroup->GetCount(idx);
		int	iRarePct = pGroup->GetRarePct(idx);
		LPITEM item_get = NULL;
		switch (dwVnum)
		{
			case CSpecialItemGroup::GOLD:
#ifdef YANG_LIMIT
				GoldChange(static_cast<LDWORD>(dwCount),"KUTU");
#else
				PointChange(POINT_GOLD, dwCount);
#endif
				LogManager::instance().CharLog(this, dwCount, "TREASURE_GOLD", "");

				bSuccess = true;
				break;
			case CSpecialItemGroup::EXP:
				{
					PointChange(POINT_EXP, dwCount);
					LogManager::instance().CharLog(this, dwCount, "TREASURE_EXP", "");

					bSuccess = true;
				}
				break;

			case CSpecialItemGroup::MOB:
				{
					sys_log(0, "CSpecialItemGroup::MOB %d", dwCount);
					int x = GetX() + number(-500, 500);
					int y = GetY() + number(-500, 500);

					LPCHARACTER ch = CHARACTER_MANAGER::instance().SpawnMob(dwCount, GetMapIndex(), x, y, 0, true, -1);
					if (ch)
						ch->SetAggressive();
					bSuccess = true;
				}
				break;
			case CSpecialItemGroup::SLOW:
				{
					sys_log(0, "CSpecialItemGroup::SLOW %d", -(int)dwCount);
					AddAffect(AFFECT_SLOW, POINT_MOV_SPEED, -(int)dwCount, AFF_SLOW, 300, 0, true);
					bSuccess = true;
				}
				break;
			case CSpecialItemGroup::DRAIN_HP:
				{
					int iDropHP = GetMaxHP()*dwCount/100;
					sys_log(0, "CSpecialItemGroup::DRAIN_HP %d", -iDropHP);
					iDropHP = MIN(iDropHP, GetHP()-1);
					sys_log(0, "CSpecialItemGroup::DRAIN_HP %d", -iDropHP);
					PointChange(POINT_HP, -iDropHP);
					bSuccess = true;
				}
				break;
			case CSpecialItemGroup::POISON:
				{
					AttackedByPoison(NULL);
					bSuccess = true;
				}
				break;

		case CSpecialItemGroup::MOB_GROUP:
		{
			int sx = GetX() - number(300, 500);
			int sy = GetY() - number(300, 500);
			int ex = GetX() + number(300, 500);
			int ey = GetY() + number(300, 500);
			CHARACTER_MANAGER::instance().SpawnGroup(dwCount, GetMapIndex(), sx, sy, ex, ey, NULL, true);

			bSuccess = true;
		}
		break;
		default:
		{
			item_get = GiveItem(dwVnum, dwCount, iRarePct, false);

			if (item_get)
			{
				bSuccess = true;
			}
		}
		break;
		}

		if (bSuccess)
		{
			dwItemVnums.push_back(dwVnum);
			dwItemCounts.push_back(dwCount);
			item_gets.push_back(item_get);
			count++;

		}
		else
		{
			return false;
		}
	}
	return bSuccess;
}

// NEW_HAIR_STYLE_ADD
bool CHARACTER::ItemProcess_Hair(LPITEM item, int iDestCell)
{
	if (item->CheckItemUseLevel(GetLevel()) == false)
	{
		// 레벨 제한에 걸림
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("아직 이 머리를 사용할 수 없는 레벨입니다."));
		return false;
	}

	DWORD hair = item->GetVnum();

	switch (GetJob())
	{
		case JOB_WARRIOR :
			hair -= 72000; // 73001 - 72000 = 1001 부터 헤어 번호 시작
			break;

		case JOB_ASSASSIN :
			hair -= 71250;
			break;

		case JOB_SURA :
			hair -= 70500;
			break;

		case JOB_SHAMAN :
			hair -= 69750;
			break;

		case JOB_WOLFMAN:
			break;
		default :
			return false;
			break;
	}

	if (hair == GetPart(PART_HAIR))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("동일한 머리 스타일로는 교체할 수 없습니다."));
		return true;
	}

	item->SetCount(item->GetCount() - 1);

	SetPart(PART_HAIR, hair);
	UpdatePacket();

	return true;
}
// END_NEW_HAIR_STYLE_ADD

bool CHARACTER::ItemProcess_Polymorph(LPITEM item)
{
	if (IsPolymorphed())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("이미 둔갑중인 상태입니다."));
		return false;
	}

	if (true == IsRiding())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("둔갑할 수 없는 상태입니다."));
		return false;
	}

	DWORD dwVnum = item->GetSocket(0);

	if (dwVnum == 0)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("잘못된 둔갑 아이템입니다."));
		item->SetCount(item->GetCount()-1);
		return false;
	}

	const CMob* pMob = CMobManager::instance().Get(dwVnum);

	if (pMob == NULL)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("잘못된 둔갑 아이템입니다."));
		item->SetCount(item->GetCount()-1);
		return false;
	}

	switch (item->GetVnum())
	{
		case 70104 :
		case 70105 :
		case 70106 :
		case 70107 :
		case 71093 :
			{
				// 둔갑구 처리
				sys_log(0, "USE_POLYMORPH_BALL PID(%d) vnum(%d)", GetPlayerID(), dwVnum);

				// 레벨 제한 체크
				int iPolymorphLevelLimit = MAX(0, 20 - GetLevel() * 3 / 10);
				if (pMob->m_table.bLevel >= GetLevel() + iPolymorphLevelLimit)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("나보다 너무 높은 레벨의 몬스터로는 변신 할 수 없습니다."));
					return false;
				}

				int iDuration = GetSkillLevel(POLYMORPH_SKILL_ID) == 0 ? 5 : (5 + (5 + GetSkillLevel(POLYMORPH_SKILL_ID)/40 * 25));
				iDuration *= 60;

				DWORD dwBonus = 0;
				
				if (true == LC_IsYMIR() || true == LC_IsKorea())
				{
					dwBonus = GetSkillLevel(POLYMORPH_SKILL_ID) + 60;
				}
				else
				{
					dwBonus = (2 + GetSkillLevel(POLYMORPH_SKILL_ID)/40) * 100;
				}

				AddAffect(AFFECT_POLYMORPH, POINT_POLYMORPH, dwVnum, AFF_POLYMORPH, iDuration, 0, true);
				AddAffect(AFFECT_POLYMORPH, POINT_ATT_BONUS, dwBonus, AFF_POLYMORPH, iDuration, 0, false);
				
				item->SetCount(item->GetCount()-1);
			}
			break;

		case 50322:
			{
				// 보류

				// 둔갑서 처리
				// 소켓0                소켓1           소켓2   
				// 둔갑할 몬스터 번호   수련정도        둔갑서 레벨
				sys_log(0, "USE_POLYMORPH_BOOK: %s(%u) vnum(%u)", GetName(), GetPlayerID(), dwVnum);

				if (CPolymorphUtils::instance().PolymorphCharacter(this, item, pMob) == true)
				{
					CPolymorphUtils::instance().UpdateBookPracticeGrade(this, item);
				}
				else
				{
				}
			}
			break;

		default :
			sys_err("POLYMORPH invalid item passed PID(%d) vnum(%d)", GetPlayerID(), item->GetOriginalVnum());
			return false;
	}

	return true;
}

bool CHARACTER::CanDoCube() const
{
	if (m_bIsObserver)	return false;
	if (GetShop())		return false;
	if (GetMyShop())	return false;
	if (m_bUnderRefine)	return false;
	if (IsWarping())	return false;
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (GetOfflineShop()) return false;
#endif

	return true;
}

bool CHARACTER::UnEquipSpecialRideUniqueItem()
{
	LPITEM Unique1 = GetWear(WEAR_UNIQUE1);
	LPITEM Unique2 = GetWear(WEAR_UNIQUE2);
	//LPITEM Unique3 = GetWear(WEAR_COSTUME_MOUNT);

	if (NULL != Unique1)
	{
		if (UNIQUE_GROUP_SPECIAL_RIDE == Unique1->GetSpecialGroup())
		{
			return UnequipItem(Unique1);
		}
	}

	if (NULL != Unique2)
	{
		if (UNIQUE_GROUP_SPECIAL_RIDE == Unique2->GetSpecialGroup())
		{
			return UnequipItem(Unique2);
		}
	}

	//if (NULL != Unique3)
	//{
		//if (UNIQUE_GROUP_SPECIAL_RIDE == Unique3->GetSpecialGroup())
	//	{
			//return UnequipItem(Unique3);
		//}
	//}

	return true;
}

void CHARACTER::AutoRecoveryItemProcess(const EAffectTypes type)
{
   if ((true == IsDead()) || (true == IsStun()) || (false == IsPC()) || (AFFECT_AUTO_HP_RECOVERY != type && AFFECT_AUTO_SP_RECOVERY != type) || (NULL != FindAffect(AFFECT_STUN)))
       return;
 
 
   {
       const DWORD stunSkills[] = { SKILL_TANHWAN, SKILL_GEOMPUNG, SKILL_BYEURAK, SKILL_GIGUNG };
       for (size_t i=0 ; i < sizeof(stunSkills)/sizeof(DWORD) ; ++i)
       {
           const CAffect* p = FindAffect(stunSkills[i]);
 
 
           if (NULL != p && AFF_STUN == p->dwFlag)
               return;
       }
   }
   const CAffect* pAffect = FindAffect(type);
   if (NULL != pAffect)
   {
       LPITEM pItem = FindItemByID(pAffect->dwFlag);
       if (NULL != pItem && true == pItem->GetSocket(0))
       {
           if (false == CArenaManager::instance().IsArenaMap(GetMapIndex()))
           {
               int32_t gelendeger = 0;
 
 
               if (AFFECT_AUTO_HP_RECOVERY == type)
               {
                   gelendeger = GetMaxHP() - (GetHP() + GetPoint(POINT_HP_RECOVERY));
               }
               else if (AFFECT_AUTO_SP_RECOVERY == type)
               {
                   gelendeger = GetMaxSP() - (GetSP() + GetPoint(POINT_SP_RECOVERY));
               }
 
 
               if (gelendeger > 0)
               {
                   if (AFFECT_AUTO_HP_RECOVERY == type)
                   {
                       PointChange(POINT_HP_RECOVERY,gelendeger);
                       EffectPacket(SE_AUTO_HPUP);
                   }
                   else if (AFFECT_AUTO_SP_RECOVERY == type)
                   {
                       PointChange(POINT_SP_RECOVERY,gelendeger);
                       EffectPacket(SE_AUTO_SPUP);
                   }
               }
           }
           else
           {
               pItem->Lock(false);
               pItem->SetSocket(0, false);
               RemoveAffect( const_cast<CAffect*>(pAffect) );
           }
       }
       else
       {
           RemoveAffect( const_cast<CAffect*>(pAffect) );
       }
   }
}

bool CHARACTER::IsValidItemPosition(TItemPos Pos) const
{
	BYTE window_type = Pos.window_type;
	WORD cell = Pos.cell;
	
	switch (window_type)
	{
	case RESERVED_WINDOW:
		return false;

	case INVENTORY:
	case EQUIPMENT:
		return cell < (INVENTORY_AND_EQUIP_SLOT_MAX);

	case DRAGON_SOUL_INVENTORY:
		return cell < (DRAGON_SOUL_INVENTORY_MAX_NUM);

#ifdef ENABLE_SPECIAL_STORAGE
	case UPGRADE_INVENTORY:
	case STONE_INVENTORY:
	case CHEST_INVENTORY:
	case ATTR_INVENTORY:
		return cell < (SPECIAL_INVENTORY_MAX_NUM);
#endif		
		
	case SAFEBOX:
		if (NULL != m_pkSafebox)
			return m_pkSafebox->IsValidPosition(cell);
		else
			return false;

	case MALL:
		if (NULL != m_pkMall)
			return m_pkMall->IsValidPosition(cell);
		else
			return false;
#ifdef ENABLE_SWITCHBOT
	case SWITCHBOT:
		return cell < SWITCHBOT_SLOT_COUNT;
#endif
	default:
		return false;
	}
}


// 귀찮아서 만든 매크로.. exp가 true면 msg를 출력하고 return false 하는 매크로 (일반적인 verify 용도랑은 return 때문에 약간 반대라 이름때문에 헷갈릴 수도 있겠다..)
#define VERIFY_MSG(exp, msg)  \
	if (true == (exp)) { \
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT(msg)); \
			return false; \
	}

		
/// 현재 캐릭터의 상태를 바탕으로 주어진 item을 착용할 수 있는 지 확인하고, 불가능 하다면 캐릭터에게 이유를 알려주는 함수
bool CHARACTER::CanEquipNow(const LPITEM item, const TItemPos& srcCell, const TItemPos& destCell) /*const*/
{
	const TItemTable* itemTable = item->GetProto();
	BYTE itemType = item->GetType();
	BYTE itemSubType = item->GetSubType();

	switch (GetJob())
	{
		case JOB_WARRIOR:
			if (item->GetAntiFlag() & ITEM_ANTIFLAG_WARRIOR)
				return false;
			break;

		case JOB_ASSASSIN:
			if (item->GetAntiFlag() & ITEM_ANTIFLAG_ASSASSIN)
				return false;
			break;

		case JOB_SHAMAN:
			if (item->GetAntiFlag() & ITEM_ANTIFLAG_SHAMAN)
				return false;
			break;

		case JOB_SURA:
			if (item->GetAntiFlag() & ITEM_ANTIFLAG_SURA)
				return false;
			break;
		case JOB_WOLFMAN:
			if (item->GetAntiFlag() & ITEM_ANTIFLAG_WOLFMAN)
				return false;
			break;
	}

	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		long limit = itemTable->aLimits[i].lValue;
		switch (itemTable->aLimits[i].bType)
		{
			case LIMIT_LEVEL:
				if (GetLevel() < limit)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("레벨이 낮아 착용할 수 없습니다."));
					return false;
				}
				break;

			case LIMIT_STR:
				if (GetPoint(POINT_ST) < limit)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("근력이 낮아 착용할 수 없습니다."));
					return false;
				}
				break;

			case LIMIT_INT:
				if (GetPoint(POINT_IQ) < limit)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("지능이 낮아 착용할 수 없습니다."));
					return false;
				}
				break;

			case LIMIT_DEX:
				if (GetPoint(POINT_DX) < limit)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("민첩이 낮아 착용할 수 없습니다."));
					return false;
				}
				break;

			case LIMIT_CON:
				if (GetPoint(POINT_HT) < limit)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("체력이 낮아 착용할 수 없습니다."));
					return false;
				}
				break;
		}
	}

	if (item->GetWearFlag() & WEARABLE_UNIQUE)
	{
		if ((GetWear(WEAR_UNIQUE1) && GetWear(WEAR_UNIQUE1)->IsSameSpecialGroup(item)) ||
			(GetWear(WEAR_UNIQUE2) && GetWear(WEAR_UNIQUE2)->IsSameSpecialGroup(item)) ||
			(GetWear(WEAR_COSTUME_MOUNT) && GetWear(WEAR_COSTUME_MOUNT)->IsSameSpecialGroup(item)))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("kostum_slot_dolu"));
			return false;
		}

		if (marriage::CManager::instance().IsMarriageUniqueItem(item->GetVnum()) &&
			!marriage::CManager::instance().IsMarried(GetPlayerID()))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("???? ?? ???? ??? ??? ? ????."));
			return false;
		}

	}
#ifdef __WEAPON_COSTUME_SYSTEM__
#ifdef __NEW_ARROW_SYSTEM__
	if (item->GetType() == ITEM_WEAPON && item->GetSubType() != WEAPON_ARROW && item->GetSubType() != WEAPON_UNLIMITED_ARROW)
#else
	if (item->GetType() == ITEM_WEAPON && item->GetSubType() != WEAPON_ARROW)
#endif
	{
		LPITEM pkItem = GetWear(WEAR_COSTUME_WEAPON);
		if (pkItem)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("For can do this unwear the costume weapon."));
			return false;
		}
	}
	else if (item->GetType() == ITEM_COSTUME && item->GetSubType() == COSTUME_WEAPON)
	{
		LPITEM pkItem = GetWear(WEAR_WEAPON);
		if (!pkItem)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't wear a costume weapon without have a weapon weared."));
			return false;
		}
		else if (item->GetValue(3) != pkItem->GetSubType())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't wear a costume weapon who has different type of your weapon."));
			return false;
		}
		else if (pkItem->GetType() == ITEM_ROD || pkItem->GetType() == ITEM_PICK)
		{
			ChatPacket(CHAT_TYPE_INFO, "HATA");
			return false;
		}
	}
	
	if (item->GetType() == ITEM_ROD || item->GetType() == ITEM_PICK)
	{
		LPITEM pkItem = GetWear(WEAR_COSTUME_WEAPON);
		if (pkItem)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("For can do this unwear the costume weapon."));
			return false;
		}
	}
#endif
	// @fixme47
	LPITEM ring1;
	LPITEM ring2;
	
	if (item->GetType() == ITEM_RING)
	{
		LPITEM ring1 = GetWear(WEAR_RING1);
		LPITEM ring2 = GetWear(WEAR_RING2);
		
		if (ring1 && (ring1->GetVnum() == item->GetVnum()))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("같은 종류의 유니크 아이템 두 개를 동시에 장착할 수 없습니다."));
			return false;			
		}
		
		if (ring2 && (ring2->GetVnum() == item->GetVnum()))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("같은 종류의 유니크 아이템 두 개를 동시에 장착할 수 없습니다."));
			return false;			
		}
	}

	return true;
}

/// 현재 캐릭터의 상태를 바탕으로 착용 중인 item을 벗을 수 있는 지 확인하고, 불가능 하다면 캐릭터에게 이유를 알려주는 함수
bool CHARACTER::CanUnequipNow(const LPITEM item, const TItemPos& srcCell, const TItemPos& destCell) /*const*/
{	

	if (ITEM_BELT == item->GetType())
		VERIFY_MSG(CBeltInventoryHelper::IsExistItemInBeltInventory(this), "벨트 인벤토리에 아이템이 존재하면 해제할 수 없습니다.");

	// 영원히 해제할 수 없는 아이템
	if (IS_SET(item->GetFlag(), ITEM_FLAG_IRREMOVABLE))
		return false;

	// 아이템 unequip시 인벤토리로 옮길 때 빈 자리가 있는 지 확인
	{
		int pos = -1;

		if (item->IsDragonSoul())
			pos = GetEmptyDragonSoulInventory(item);
		else
			pos = GetEmptyInventory(item->GetSize());

		VERIFY_MSG( -1 == pos, "소지품에 빈 공간이 없습니다." );
	}

#ifdef __WEAPON_COSTUME_SYSTEM__
#ifdef __NEW_ARROW_SYSTEM__
	if (item->GetType() == ITEM_WEAPON && item->GetSubType() != WEAPON_ARROW && item->GetSubType() != WEAPON_UNLIMITED_ARROW)
#else
	if (item->GetType() == ITEM_WEAPON && item->GetSubType() != WEAPON_ARROW)
#endif
	{
		LPITEM pkItem = GetWear(WEAR_COSTUME_WEAPON);
		if (pkItem)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("For can do this unwear the costume weapon."));
			return false;
		}
	}
#endif
	return true;
}

bool CHARACTER::UnEquipMountItem()
{
	LPITEM Unique = GetWear(WEAR_COSTUME_MOUNT);
	if (NULL != Unique)
		return UnequipItem(Unique);
	return false;
}