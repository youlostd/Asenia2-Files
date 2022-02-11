#include "stdafx.h"
#include "utils.h"
#include "config.h"
#include "char.h"
#include "desc.h"
#include "sectree_manager.h"
#include "packet.h"
#include "protocol.h"
#include "log.h"
#include "skill.h"
#include "unique_item.h"
#include "profiler.h"
#include "marriage.h"
#include "item_addon.h"
#include "dev_log.h"
#include "locale_service.h"
#include "item.h"
#include "item_manager.h"
#include "affect.h"
#include "DragonSoul.h"
#include "buff_on_attributes.h"
#include "belt_inventory_helper.h"
#include "../../common/VnumHelper.h"
#include "mob_manager.h"
#include "PetSystem.h"

CItem::CItem(DWORD dwVnum)
	: m_dwVnum(dwVnum), m_bWindow(0), m_dwID(0), m_bEquipped(false), m_dwVID(0), m_wCell(0), m_dwCount(0), m_lFlag(0), m_dwLastOwnerPID(0),
	m_bExchanging(false), m_pkDestroyEvent(NULL), m_pkUniqueExpireEvent(NULL), m_pkTimerBasedOnWearExpireEvent(NULL), m_pkRealTimeExpireEvent(NULL),
	m_pkExpireEvent(NULL),
   	m_pkAccessorySocketExpireEvent(NULL), m_pkOwnershipEvent(NULL), m_dwOwnershipPID(0), m_bSkipSave(false), m_isLocked(false),
	m_dwMaskVnum(0), m_dwSIGVnum (0)
	,m_dwEvolution(0)
{
	memset( &m_alSockets, 0, sizeof(m_alSockets) );
	memset( &m_aAttr, 0, sizeof(m_aAttr) );
}

CItem::~CItem()
{
	Destroy();
}

void CItem::Initialize()
{
	CEntity::Initialize(ENTITY_ITEM);

	m_bWindow = RESERVED_WINDOW;
	m_pOwner = NULL;
	m_dwID = 0;
	m_bEquipped = false;
	m_dwVID = m_wCell = m_dwCount = m_lFlag = 0;
	m_pProto = NULL;
	m_bExchanging = false;
	memset(&m_alSockets, 0, sizeof(m_alSockets));
	memset(&m_aAttr, 0, sizeof(m_aAttr));
	m_dwEvolution = 0;
	m_pkDestroyEvent = NULL;
	m_pkOwnershipEvent = NULL;
	m_dwOwnershipPID = 0;
	m_pkUniqueExpireEvent = NULL;
	m_pkTimerBasedOnWearExpireEvent = NULL;
	m_pkRealTimeExpireEvent = NULL;

	m_pkAccessorySocketExpireEvent = NULL;

	m_bSkipSave = false;
	m_dwLastOwnerPID = 0;
}

void CItem::Destroy()
{
	event_cancel(&m_pkDestroyEvent);
	event_cancel(&m_pkOwnershipEvent);
	event_cancel(&m_pkUniqueExpireEvent);
	event_cancel(&m_pkTimerBasedOnWearExpireEvent);
	event_cancel(&m_pkRealTimeExpireEvent);
	event_cancel(&m_pkAccessorySocketExpireEvent);

	CEntity::Destroy();

	if (GetSectree())
		GetSectree()->RemoveEntity(this);
}

EVENTFUNC(item_destroy_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "item_destroy_event> <Factor> Null pointer" );
		return 0;
	}

	LPITEM pkItem = info->item;

	if (pkItem->GetOwner())
		sys_err("item_destroy_event: Owner exist. (item %s owner %s)", pkItem->GetName(), pkItem->GetOwner()->GetName());

	pkItem->SetDestroyEvent(NULL);
	M2_DESTROY_ITEM(pkItem);
	return 0;
}

void CItem::SetDestroyEvent(LPEVENT pkEvent)
{
	m_pkDestroyEvent = pkEvent;
}

void CItem::StartDestroyEvent(int iSec)
{
	if (m_pkDestroyEvent)
		return;

	item_event_info* info = AllocEventInfo<item_event_info>();
	info->item = this;

	SetDestroyEvent(event_create(item_destroy_event, info, PASSES_PER_SEC(iSec)));
}

void CItem::EncodeInsertPacket(LPENTITY ent)
{
	LPDESC d;

	if (!(d = ent->GetDesc()))
		return;

	const PIXEL_POSITION & c_pos = GetXYZ();

	struct packet_item_ground_add pack;

	pack.bHeader	= HEADER_GC_ITEM_GROUND_ADD;
	pack.x		= c_pos.x;
	pack.y		= c_pos.y;
	pack.z		= c_pos.z;
	pack.dwVnum		= GetVnum();
	pack.dwVID		= m_dwVID;
#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
	for (size_t i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		pack.alSockets[i] = GetSocket(i);

	thecore_memcpy(pack.aAttrs, GetAttributes(), sizeof(pack.aAttrs));
#endif
	//pack.count	= m_dwCount;

	d->Packet(&pack, sizeof(pack));

	if (m_pkOwnershipEvent != NULL)
	{
		item_event_info * info = dynamic_cast<item_event_info *>(m_pkOwnershipEvent->info);

		if ( info == NULL )
		{
			sys_err( "CItem::EncodeInsertPacket> <Factor> Null pointer" );
			return;
		}

		TPacketGCItemOwnership p;

		p.bHeader = HEADER_GC_ITEM_OWNERSHIP;
		p.dwVID = m_dwVID;
		strlcpy(p.szName, info->szOwnerName, sizeof(p.szName));

		d->Packet(&p, sizeof(TPacketGCItemOwnership));
	}
}

void CItem::EncodeRemovePacket(LPENTITY ent)
{
	LPDESC d;

	if (!(d = ent->GetDesc()))
		return;

	struct packet_item_ground_del pack;

	pack.bHeader	= HEADER_GC_ITEM_GROUND_DEL;
	pack.dwVID		= m_dwVID;

	d->Packet(&pack, sizeof(pack));
	sys_log(2, "Item::EncodeRemovePacket %s to %s", GetName(), ((LPCHARACTER) ent)->GetName());
}

void CItem::SetProto(const TItemTable * table)
{
	assert(table != NULL);
	m_pProto = table;
	SetFlag(m_pProto->dwFlags);
}

void CItem::UsePacketEncode(LPCHARACTER ch, LPCHARACTER victim, struct packet_item_use *packet)
{
	if (!GetVnum())
		return;

	packet->header 	= HEADER_GC_ITEM_USE;
	packet->ch_vid 	= ch->GetVID();
	packet->victim_vid 	= victim->GetVID();
	packet->Cell = TItemPos(GetWindow(), m_wCell);
	packet->vnum	= GetVnum();
}

void CItem::RemoveFlag(long bit)
{
	REMOVE_BIT(m_lFlag, bit);
}

void CItem::AddFlag(long bit)
{
	SET_BIT(m_lFlag, bit);
}

void CItem::UpdatePacket()
{
	if (!m_pOwner || !m_pOwner->GetDesc())
		return;

#ifdef ENABLE_SWITCHBOT
	if (m_bWindow == SWITCHBOT)
		return;
#endif

	TPacketGCItemUpdate pack;

	pack.header = HEADER_GC_ITEM_UPDATE;
	pack.Cell = TItemPos(GetWindow(), m_wCell);
	pack.count	= m_dwCount;
	pack.evolution = m_dwEvolution;

	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		pack.alSockets[i] = m_alSockets[i];

	thecore_memcpy(pack.aAttr, GetAttributes(), sizeof(pack.aAttr));

	sys_log(2, "UpdatePacket %s -> %s", GetName(), m_pOwner->GetName());
	m_pOwner->GetDesc()->Packet(&pack, sizeof(pack));
}

DWORD CItem::GetEvolution()
{
	return m_dwEvolution;
}

const char * CItem::GetNames()
{
	if(m_pProto){
		if(m_dwEvolution >=1 && m_dwEvolution <=4){
			std::string evolutionMergeText = "";
			char evolutionText[4][10] = {"Yaygýn ", "Nadir ", "Efsane ", "Eþsiz " };
			evolutionMergeText += evolutionText[(m_dwEvolution - 1)];
			evolutionMergeText += m_pProto->szLocaleName;
			return  evolutionMergeText.c_str();
		}
		return m_pProto->szLocaleName;
	}
	return NULL;
}

void CItem::SetEvolution(DWORD evolution)
{
	m_dwEvolution = evolution;
	UpdatePacket();
	Save();
}


DWORD CItem::GetCount()
{
	if (GetType() == ITEM_ELK) return MIN(m_dwCount, INT_MAX);
#ifdef ENABLE_CHEQUE_SYSTEM
	else if (GetType() == ITEM_WON) return MIN(m_dwCount, INT_MAX);
#endif
	else
	{
		return MIN(m_dwCount, ITEM_MAX_COUNT);
	}
}

bool CItem::SetCount(DWORD count)
{
	if (GetType() == ITEM_ELK)
	{
		m_dwCount = MIN(count, INT_MAX);
	}
#ifdef ENABLE_CHEQUE_SYSTEM
	else if (GetType() == ITEM_WON)
	{
		m_dwCount = MIN(count, INT_MAX);
	}
#endif
	else
	{
		m_dwCount = MIN(count, ITEM_MAX_COUNT);
	}

	if (count == 0 && m_pOwner)
	{
		if (GetSubType() == USE_ABILITY_UP || GetSubType() == USE_POTION || GetVnum() == 70020)
		{
			LPCHARACTER pOwner = GetOwner();
			WORD wCell = GetCell();

			RemoveFromCharacter();

			if (!IsDragonSoul())
			{
				LPITEM pItem = pOwner->FindSpecifyItem(GetVnum());

				if (NULL != pItem)
				{
					pOwner->ChainQuickslotItem(pItem, QUICKSLOT_TYPE_ITEM, wCell);
				}
				else
				{
					pOwner->SyncQuickslot(QUICKSLOT_TYPE_ITEM, wCell, 255);
				}
			}

			M2_DESTROY_ITEM(this);
		}
		else
		{
			if (!IsDragonSoul())
			{
				m_pOwner->SyncQuickslot(QUICKSLOT_TYPE_ITEM, m_wCell, 255);
			}
			M2_DESTROY_ITEM(RemoveFromCharacter());
		}

		return false;
	}

	UpdatePacket();

	Save();
	return true;
}

LPITEM CItem::RemoveFromCharacter()
{
	if (!m_pOwner)
	{
		sys_err("Item::RemoveFromCharacter owner null");
		return (this);
	}

	LPCHARACTER pOwner = m_pOwner;

	if (m_bEquipped)	// ÀåÂøµÇ¾ú´Â°¡?
	{
		Unequip();
		//pOwner->UpdatePacket();

		SetWindow(RESERVED_WINDOW);
		Save();
		return (this);
	}
	else
	{
		if (GetWindow() != SAFEBOX && GetWindow() != MALL)
		{
			if (IsDragonSoul())
			{
				if (m_wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
					sys_err("CItem::RemoveFromCharacter: pos >= DRAGON_SOUL_INVENTORY_MAX_NUM");
				else
					pOwner->SetItem(TItemPos(m_bWindow, m_wCell), NULL);
			}
#ifdef ENABLE_SPECIAL_STORAGE
			else if (IsUpgradeItem() || IsStone() || IsChest() || IsAttr())
			{
				if (m_wCell >= INVENTORY_MAX_NUM) //if (m_wCell >= SPECIAL_INVENTORY_MAX_NUM)
					sys_err("CItem::RemoveFromCharacter: pos >= SPECIAL_INVENTORY_MAX_NUM");
				else
					pOwner->SetItem(TItemPos(m_bWindow, m_wCell), NULL);
			}
#endif
#ifdef ENABLE_SWITCHBOT
			else if (m_bWindow == SWITCHBOT)
			{
				if (m_wCell >= SWITCHBOT_SLOT_COUNT)
				{
					sys_err("CItem::RemoveFromCharacter: pos >= SWITCHBOT_SLOT_COUNT");
				}
				else
				{
					pOwner->SetItem(TItemPos(SWITCHBOT, m_wCell), NULL);
				}
			}
#endif
			else
			{
				TItemPos cell(INVENTORY, m_wCell);

				if (false == cell.IsDefaultInventoryPosition() && false == cell.IsBeltInventoryPosition()) // ¾Æ´Ï¸é ¼ÒÁöÇ°¿¡?
					sys_err("CItem::RemoveFromCharacter: Invalid Item Position");
				else
				{
					pOwner->SetItem(cell, NULL);
				}
			}
		}

		m_pOwner = NULL;
		m_wCell = 0;

		SetWindow(RESERVED_WINDOW);
		Save();
		return (this);
	}
}

bool CItem::AddToCharacter(LPCHARACTER ch, TItemPos Cell)
{
	assert(GetSectree() == NULL);
	assert(m_pOwner == NULL);
	WORD pos = Cell.cell;
	BYTE window_type = Cell.window_type;
	
	if (INVENTORY == window_type)
	{
		if (m_wCell >= INVENTORY_MAX_NUM && BELT_INVENTORY_SLOT_START > m_wCell)
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
	else if (DRAGON_SOUL_INVENTORY == window_type)
	{
		if (m_wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
#ifdef ENABLE_SPECIAL_STORAGE
	else if (UPGRADE_INVENTORY == window_type || STONE_INVENTORY == window_type || CHEST_INVENTORY == window_type || ATTR_INVENTORY == window_type)
	{
		if (m_wCell >= SPECIAL_INVENTORY_MAX_NUM)
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
#endif
#ifdef ENABLE_SWITCHBOT
	else if (SWITCHBOT == window_type)
	{
		if (m_wCell >= SWITCHBOT_SLOT_COUNT)
		{
			sys_err("CItem::AddToCharacter:switchbot cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
#endif
	if (ch->GetDesc())
		m_dwLastOwnerPID = ch->GetPlayerID();

	#ifdef __SASH_SYSTEM__
	if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_SASH) && (GetSocket(SASH_ABSORPTION_SOCKET) == 0))
	{
		long lVal = GetValue(SASH_GRADE_VALUE_FIELD);
		switch (lVal)
		{
			case 2:
				{
					lVal = SASH_GRADE_2_ABS;
				}
				break;
			case 3:
				{
					lVal = SASH_GRADE_3_ABS;
				}
				break;
			case 4:
				{
					lVal = number(SASH_GRADE_4_ABS_MIN, SASH_GRADE_4_ABS_MAX_COMB);
				}
				break;
			default:
				{
					lVal = SASH_GRADE_1_ABS;
				}
				break;
		}
		
		SetSocket(SASH_ABSORPTION_SOCKET, lVal);
	}
	#endif	
	
#ifdef ENABLE_AURA_SYSTEM
	if ((GetType() == ITEM_COSTUME && GetSubType() == COSTUME_AURA) && (GetSocket(AURA_ABSORPTION_SOCKET) == 0))
	{
		long lVal = GetValue(AURA_GRADE_VALUE_FIELD);
		if (lVal < 1)
		{
			SetSocket(AURA_GRADE_VALUE_FIELD, 1);
			SetSocket(AURA_ABSORPTION_SOCKET, 1);
		}
		else
			SetSocket(AURA_ABSORPTION_SOCKET, lVal);
	}
#endif
	
	event_cancel(&m_pkDestroyEvent);

	ch->SetItem(TItemPos(window_type, pos), this);
	m_pOwner = ch;

	Save();
	return true;
}

LPITEM CItem::RemoveFromGround()
{
	if (GetSectree())
	{
		SetOwnership(NULL);
		
		GetSectree()->RemoveEntity(this);
		
		ViewCleanup();

		Save();
	}

	return (this);
}

bool CItem::AddToGround(long lMapIndex, const PIXEL_POSITION & pos, bool skipOwnerCheck)
{
	if (0 == lMapIndex)
	{
		sys_err("wrong map index argument: %d", lMapIndex);
		return false;
	}

	if (GetSectree())
	{
		sys_err("sectree already assigned");
		return false;
	}

	if (!skipOwnerCheck && m_pOwner)
	{
		sys_err("owner pointer not null");
		return false;
	}

	LPSECTREE tree = SECTREE_MANAGER::instance().Get(lMapIndex, pos.x, pos.y);

	if (!tree)
	{
		sys_err("cannot find sectree by %dx%d", pos.x, pos.y);
		return false;
	}

	//tree->Touch();

	SetWindow(GROUND);
	SetXYZ(pos.x, pos.y, pos.z);
	tree->InsertEntity(this);
	UpdateSectree();
	Save();
	return true;
}

bool CItem::DistanceValid(LPCHARACTER ch)
{
	if (!GetSectree())
		return false;

	int iDist = DISTANCE_APPROX(GetX() - ch->GetX(), GetY() - ch->GetY());

	if (iDist > 300)
		return false;

	return true;
}

bool CItem::CanUsedBy(LPCHARACTER ch)
{
	// Anti flag check
	switch (ch->GetJob())
	{
		case JOB_WARRIOR:
			if (GetAntiFlag() & ITEM_ANTIFLAG_WARRIOR)
				return false;
			break;

		case JOB_ASSASSIN:
			if (GetAntiFlag() & ITEM_ANTIFLAG_ASSASSIN)
				return false;
			break;

		case JOB_SHAMAN:
			if (GetAntiFlag() & ITEM_ANTIFLAG_SHAMAN)
				return false;
			break;

		case JOB_SURA:
			if (GetAntiFlag() & ITEM_ANTIFLAG_SURA)
				return false;
			break;
		case JOB_WOLFMAN:
			if (GetAntiFlag() & ITEM_ANTIFLAG_WOLFMAN)
				return false;
			break;
	}

	return true;
}

int CItem::FindEquipCell(LPCHARACTER ch, int iCandidateCell)
{
	// ÄÚ½ºÃõ ¾ÆÀÌÅÛ(ITEM_COSTUME)Àº WearFlag ¾ø¾îµµ µÊ. (sub typeÀ¸·Î Âø¿ëÀ§Ä¡ ±¸ºÐ. ±ÍÂú°Ô ¶Ç wear flag ÁÙ ÇÊ¿ä°¡ ÀÖ³ª..)
	// ¿ëÈ¥¼®(ITEM_DS, ITEM_SPECIAL_DS)µµ  SUB_TYPEÀ¸·Î ±¸ºÐ. ½Å±Ô ¹ÝÁö, º§Æ®´Â ITEM_TYPEÀ¸·Î ±¸ºÐ -_-
	if ((0 == GetWearFlag() || ITEM_TOTEM == GetType()) && ITEM_COSTUME != GetType() && ITEM_DS != GetType() && ITEM_SPECIAL_DS != GetType() && ITEM_RING != GetType() && ITEM_BELT != GetType())
		return -1;

	// ¿ëÈ¥¼® ½½·ÔÀ» WEAR·Î Ã³¸®ÇÒ ¼ö°¡ ¾ø¾î¼­(WEAR´Â ÃÖ´ë 32°³±îÁö °¡´ÉÇÑµ¥ ¿ëÈ¥¼®À» Ãß°¡ÇÏ¸é 32°¡ ³Ñ´Â´Ù.)
	// ÀÎº¥Åä¸®ÀÇ Æ¯Á¤ À§Ä¡((INVENTORY_MAX_NUM + WEAR_MAX_NUM)ºÎÅÍ (INVENTORY_MAX_NUM + WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX - 1)±îÁö)¸¦
	// ¿ëÈ¥¼® ½½·ÔÀ¸·Î Á¤ÇÔ.
	// return ÇÒ ¶§¿¡, INVENTORY_MAX_NUMÀ» »« ÀÌÀ¯´Â,
	// º»·¡ WearCellÀÌ INVENTORY_MAX_NUM¸¦ »©°í return ÇÏ±â ¶§¹®.
	if (GetType() == ITEM_DS || GetType() == ITEM_SPECIAL_DS)
	{
		if (iCandidateCell < 0)
		{
			return WEAR_MAX_NUM + GetSubType();
		}
		else
		{
			for (int i = 0; i < DRAGON_SOUL_DECK_MAX_NUM; i++)
			{
				if (WEAR_MAX_NUM + i * DS_SLOT_MAX + GetSubType() == iCandidateCell)
				{
					return iCandidateCell;
				}
			}
			return -1;
		}
	}
	else if (GetType() == ITEM_COSTUME)
	{
		if (GetSubType() == COSTUME_BODY)
			return WEAR_COSTUME_BODY;
		else if (GetSubType() == COSTUME_HAIR)
			return WEAR_COSTUME_HAIR;
		#ifdef __SASH_SYSTEM__
		else if (GetSubType() == COSTUME_SASH)
			return WEAR_COSTUME_SASH;
		#endif

#ifdef __WEAPON_COSTUME_SYSTEM__
		else if (GetSubType() == COSTUME_WEAPON)
			return WEAR_COSTUME_WEAPON;
#endif
#ifdef ENABLE_AURA_SYSTEM
		else if (GetSubType() == COSTUME_AURA)
			return WEAR_COSTUME_AURA;
#endif

		else if (GetSubType() == COSTUME_MOUNT)
			return WEAR_COSTUME_MOUNT;
		
#ifdef NEW_COSTUME_SOCKET_RING
		else if (GetSubType() == COSTUME_RING_SOCKET)
			return WEAR_COSTUME_RING_SOCKET;
		else if (GetSubType() == COSTUME_RING_SOCKET_2)
			return WEAR_COSTUME_RING_SOCKET_2;
		else if (GetSubType() == COSTUME_RING_SOCKET_3)
			return WEAR_COSTUME_RING_SOCKET_3;
		else if (GetSubType() == COSTUME_RING_SOCKET_4)
			return WEAR_COSTUME_RING_SOCKET_4;
		else if (GetSubType() == COSTUME_RING_SOCKET_5)
			return WEAR_COSTUME_RING_SOCKET_5;
		else if (GetSubType() == COSTUME_RING_SOCKET_6)
			return WEAR_COSTUME_RING_SOCKET_6;
		
		else if (GetSubType() == COSTUME_RING_SOCKET_7)
			return WEAR_COSTUME_RING_SOCKET_7;
		else if (GetSubType() == COSTUME_RING_SOCKET_8)
			return WEAR_COSTUME_RING_SOCKET_8;
		else if (GetSubType() == COSTUME_RING_SOCKET_9)
			return WEAR_COSTUME_RING_SOCKET_9;
		else if (GetSubType() == COSTUME_RING_SOCKET_10)
			return WEAR_COSTUME_RING_SOCKET_10;
		else if (GetSubType() == COSTUME_RING_SOCKET_11)
			return WEAR_COSTUME_RING_SOCKET_11;
		else if (GetSubType() == COSTUME_RING_SOCKET_12)
			return WEAR_COSTUME_RING_SOCKET_12;
		
		else if (GetSubType() == COSTUME_RING_SOCKET_13)
			return WEAR_COSTUME_RING_SOCKET_13;
		else if (GetSubType() == COSTUME_RING_SOCKET_14)
			return WEAR_COSTUME_RING_SOCKET_14;
		else if (GetSubType() == COSTUME_RING_SOCKET_15)
			return WEAR_COSTUME_RING_SOCKET_15;
		else if (GetSubType() == COSTUME_RING_SOCKET_16)
			return WEAR_COSTUME_RING_SOCKET_16;
		else if (GetSubType() == COSTUME_RING_SOCKET_17)
			return WEAR_COSTUME_RING_SOCKET_17;
		else if (GetSubType() == COSTUME_RING_SOCKET_18)
			return WEAR_COSTUME_RING_SOCKET_18;
		
		else if (GetSubType() == COSTUME_RING_SOCKET_19)
			return WEAR_COSTUME_RING_SOCKET_19;
		else if (GetSubType() == COSTUME_RING_SOCKET_20)
			return WEAR_COSTUME_RING_SOCKET_20;
		else if (GetSubType() == COSTUME_RING_SOCKET_21)
			return WEAR_COSTUME_RING_SOCKET_21;
		else if (GetSubType() == COSTUME_RING_SOCKET_22)
			return WEAR_COSTUME_RING_SOCKET_22;
		else if (GetSubType() == COSTUME_RING_SOCKET_23)
			return WEAR_COSTUME_RING_SOCKET_23;
		else if (GetSubType() == COSTUME_RING_SOCKET_24)
			return WEAR_COSTUME_RING_SOCKET_24;
		
		else if (GetSubType() == COSTUME_RING_SOCKET_25)
			return WEAR_COSTUME_RING_SOCKET_25;
		else if (GetSubType() == COSTUME_RING_SOCKET_26)
			return WEAR_COSTUME_RING_SOCKET_26;
		else if (GetSubType() == COSTUME_RING_SOCKET_27)
			return WEAR_COSTUME_RING_SOCKET_27;
		else if (GetSubType() == COSTUME_RING_SOCKET_28)
			return WEAR_COSTUME_RING_SOCKET_28;
		else if (GetSubType() == COSTUME_RING_SOCKET_29)
			return WEAR_COSTUME_RING_SOCKET_29;
		else if (GetSubType() == COSTUME_RING_SOCKET_30)
			return WEAR_COSTUME_RING_SOCKET_30;
#endif

	}
	else if (GetType() == ITEM_RING)
	{
		if (ch->GetWear(WEAR_RING1))
			return WEAR_RING2;
		else
			return WEAR_RING1;
	}
	else if (GetType() == ITEM_BELT)
		return WEAR_BELT;

#ifdef ENABLE_GLOVE_SYSTEM
	else if (GetType() == ITEM_ARMOR && GetSubType() == ARMOR_GLOVE)
		return WEAR_GLOVE;
#endif
	else if (IsPetItem())
		return WEAR_PET;
	else if (GetWearFlag() & WEARABLE_BODY)
		return WEAR_BODY;
	else if (GetWearFlag() & WEARABLE_HEAD)
		return WEAR_HEAD;
	else if (GetWearFlag() & WEARABLE_FOOTS)
		return WEAR_FOOTS;
	else if (GetWearFlag() & WEARABLE_WRIST)
		return WEAR_WRIST;
	else if (GetWearFlag() & WEARABLE_WEAPON)
		return WEAR_WEAPON;
	else if (GetWearFlag() & WEARABLE_SHIELD)
		return WEAR_SHIELD;
	else if (GetWearFlag() & WEARABLE_NECK)
		return WEAR_NECK;
	else if (GetWearFlag() & WEARABLE_EAR)
		return WEAR_EAR;
	else if (GetWearFlag() & WEARABLE_ELEMENT)
		return WEAR_ELEMENT;	
	else if (GetWearFlag() & WEARABLE_ARROW)
		return WEAR_ARROW;
	else if (GetWearFlag() & WEARABLE_UNIQUE)
	{
		if (ch->GetWear(WEAR_UNIQUE1))
			return WEAR_UNIQUE2;
		else
			return WEAR_UNIQUE1;		
	}

	// ¼öÁý Äù½ºÆ®¸¦ À§ÇÑ ¾ÆÀÌÅÛÀÌ ¹ÚÈ÷´Â°÷À¸·Î ÇÑ¹ø ¹ÚÈ÷¸é Àý´ë –E¼ö ¾ø´Ù.
	else if (GetWearFlag() & WEARABLE_ABILITY)
	{
		if (!ch->GetWear(WEAR_ABILITY1))
		{
			return WEAR_ABILITY1;
		}
		else if (!ch->GetWear(WEAR_ABILITY2))
		{
			return WEAR_ABILITY2;
		}
		else if (!ch->GetWear(WEAR_ABILITY3))
		{
			return WEAR_ABILITY3;
		}
		else if (!ch->GetWear(WEAR_ABILITY4))
		{
			return WEAR_ABILITY4;
		}
		else if (!ch->GetWear(WEAR_ABILITY5))
		{
			return WEAR_ABILITY5;
		}
		else if (!ch->GetWear(WEAR_ABILITY6))
		{
			return WEAR_ABILITY6;
		}
		else if (!ch->GetWear(WEAR_ABILITY7))
		{
			return WEAR_ABILITY7;
		}
		else if (!ch->GetWear(WEAR_ABILITY8))
		{
			return WEAR_ABILITY8;
		}
		else
		{
			return -1;
		}
	}
	return -1;
}

void CItem::ModifyPoints(bool bAdd)
{
	int accessoryGrade;

	// ¹«±â¿Í °©¿Ê¸¸ ¼ÒÄÏÀ» Àû¿ë½ÃÅ²´Ù.
	if (false == IsAccessoryForSocket())
	{
		if (m_pProto->bType == ITEM_WEAPON || m_pProto->bType == ITEM_ARMOR)
		{
			// ¼ÒÄÏÀÌ ¼Ó¼º°­È­¿¡ »ç¿ëµÇ´Â °æ¿ì Àû¿ëÇÏÁö ¾Ê´Â´Ù (ARMOR_WRIST ARMOR_NECK ARMOR_EAR)
			for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
			{
				DWORD dwVnum;

				if ((dwVnum = GetSocket(i)) <= 2)
					continue;

				TItemTable * p = ITEM_MANAGER::instance().GetTable(dwVnum);

				if (!p)
				{
					#ifdef __NEW_ARROW_SYSTEM__
					if (m_pProto->bSubType != WEAPON_UNLIMITED_ARROW)
					#endif
						sys_err("cannot find table by vnum %u", dwVnum);

					continue;
				}

				if (ITEM_METIN == p->bType)
				{
					//m_pOwner->ApplyPoint(p->alValues[0], bAdd ? p->alValues[1] : -p->alValues[1]);
					for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
					{
						if (p->aApplies[i].bType == APPLY_NONE)
							continue;

						if (p->aApplies[i].bType == APPLY_SKILL)
							m_pOwner->ApplyPoint(p->aApplies[i].bType, bAdd ? p->aApplies[i].lValue : p->aApplies[i].lValue ^ 0x00800000);
						else
							m_pOwner->ApplyPoint(p->aApplies[i].bType, bAdd ? p->aApplies[i].lValue : -p->aApplies[i].lValue);
					}
				}
			}
		}

		accessoryGrade = 0;
	}
	else
	{
		accessoryGrade = MIN(GetAccessorySocketGrade(), ITEM_ACCESSORY_SOCKET_MAX_NUM);
	}
#ifdef __SASH_SYSTEM__
	if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_SASH) && (GetSocket(SASH_ABSORBED_SOCKET)))
	{
		TItemTable * pkItemAbsorbed = ITEM_MANAGER::instance().GetTable(GetSocket(SASH_ABSORBED_SOCKET));
		if (pkItemAbsorbed)
		{
			if ((pkItemAbsorbed->bType == ITEM_ARMOR) && (pkItemAbsorbed->bSubType == ARMOR_BODY))
			{
				long lDefGrade = pkItemAbsorbed->alValues[1] + long(pkItemAbsorbed->alValues[5] * 2);
				double dValue = lDefGrade * GetSocket(SASH_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lDefGrade = (long) dValue;
				if ((pkItemAbsorbed->alValues[1] > 0) && (lDefGrade <= 0) || (pkItemAbsorbed->alValues[5] > 0) && (lDefGrade < 1))
					lDefGrade += 1;
				else if ((pkItemAbsorbed->alValues[1] > 0) || (pkItemAbsorbed->alValues[5] > 0))
					lDefGrade += 1;
				
				m_pOwner->ApplyPoint(APPLY_DEF_GRADE_BONUS, bAdd ? lDefGrade : -lDefGrade);
				
				long lDefMagicBonus = pkItemAbsorbed->alValues[0];
				dValue = lDefMagicBonus * GetSocket(SASH_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lDefMagicBonus = (long) dValue;
				if ((pkItemAbsorbed->alValues[0] > 0) && (lDefMagicBonus < 1))
					lDefMagicBonus += 1;
				else if (pkItemAbsorbed->alValues[0] > 0)
					lDefMagicBonus += 1;
				
				m_pOwner->ApplyPoint(APPLY_MAGIC_DEF_GRADE, bAdd ? lDefMagicBonus : -lDefMagicBonus);
			}
			else if (pkItemAbsorbed->bType == ITEM_WEAPON)
			{
				long lAttGrade = pkItemAbsorbed->alValues[4] + pkItemAbsorbed->alValues[5];
				if (pkItemAbsorbed->alValues[3] > pkItemAbsorbed->alValues[4])
					lAttGrade = pkItemAbsorbed->alValues[3] + pkItemAbsorbed->alValues[5];
				
				double dValue = lAttGrade * GetSocket(SASH_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lAttGrade = (long) dValue;
				if (((pkItemAbsorbed->alValues[3] > 0) && (lAttGrade < 1)) || ((pkItemAbsorbed->alValues[4] > 0) && (lAttGrade < 1)))
					lAttGrade += 1;
				else if ((pkItemAbsorbed->alValues[3] > 0) || (pkItemAbsorbed->alValues[4] > 0))
					lAttGrade += 1;
				
				m_pOwner->ApplyPoint(APPLY_ATT_GRADE_BONUS, bAdd ? lAttGrade : -lAttGrade);
				
				long lAttMagicGrade = pkItemAbsorbed->alValues[2] + pkItemAbsorbed->alValues[5];
				if (pkItemAbsorbed->alValues[1] > pkItemAbsorbed->alValues[2])
					lAttMagicGrade = pkItemAbsorbed->alValues[1] + pkItemAbsorbed->alValues[5];
				
				dValue = lAttMagicGrade * GetSocket(SASH_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lAttMagicGrade = (long) dValue;
				if (((pkItemAbsorbed->alValues[1] > 0) && (lAttMagicGrade < 1)) || ((pkItemAbsorbed->alValues[2] > 0) && (lAttMagicGrade < 1)))
					lAttMagicGrade += 1;
				else if ((pkItemAbsorbed->alValues[1] > 0) || (pkItemAbsorbed->alValues[2] > 0))
					lAttMagicGrade += 1;
				
				m_pOwner->ApplyPoint(APPLY_MAGIC_ATT_GRADE, bAdd ? lAttMagicGrade : -lAttMagicGrade);
			}
		}
	}
	#endif	
	
#ifdef ENABLE_AURA_SYSTEM
	if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_AURA) && (GetSocket(AURA_ABSORBED_SOCKET)))
	{
		TItemTable* pkItemAbsorbed = ITEM_MANAGER::instance().GetTable(GetSocket(AURA_ABSORBED_SOCKET));
		if (pkItemAbsorbed)
		{
			if ((pkItemAbsorbed->bType == ITEM_ARMOR) && ((pkItemAbsorbed->bSubType == ARMOR_SHIELD) || (pkItemAbsorbed->bSubType == ARMOR_HEAD)))
			{
				long lDefGrade = pkItemAbsorbed->alValues[1] + long(pkItemAbsorbed->alValues[5] * 2);
				double dValue = lDefGrade * (float(GetSocket(AURA_ABSORPTION_SOCKET) / 10.0f));
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lDefGrade = (long)dValue;
				if (((pkItemAbsorbed->alValues[1] > 0) && (lDefGrade <= 0)) || ((pkItemAbsorbed->alValues[5] > 0) && (lDefGrade < 1)))
					lDefGrade += 1;
				else if ((pkItemAbsorbed->alValues[1] > 0) || (pkItemAbsorbed->alValues[5] > 0))
					lDefGrade += 1;

				m_pOwner->ApplyPoint(APPLY_DEF_GRADE_BONUS, bAdd ? lDefGrade : -lDefGrade);

				long lDefMagicBonus = pkItemAbsorbed->alValues[0];
				dValue = lDefMagicBonus * (float(GetSocket(AURA_ABSORPTION_SOCKET) / 10.0f));
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lDefMagicBonus = (long)dValue;
				if ((pkItemAbsorbed->alValues[0] > 0) && (lDefMagicBonus < 1))
					lDefMagicBonus += 1;
				else if (pkItemAbsorbed->alValues[0] > 0)
					lDefMagicBonus += 1;

				m_pOwner->ApplyPoint(APPLY_MAGIC_DEF_GRADE, bAdd ? lDefMagicBonus : -lDefMagicBonus);
			}
		}
	}
#endif

	for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
	{
		#ifdef __SASH_SYSTEM__
		if ((m_pProto->aApplies[i].bType == APPLY_NONE) && (GetType() != ITEM_COSTUME) && (GetSubType() != COSTUME_SASH) && (GetSubType() != COSTUME_AURA))
		#else
		if (m_pProto->aApplies[i].bType == APPLY_NONE)
		#endif
			continue;
		
		BYTE bType = m_pProto->aApplies[i].bType;
		long value = m_pProto->aApplies[i].lValue;
		#ifdef __SASH_SYSTEM__
		if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_SASH))
		{
			TItemTable * pkItemAbsorbed = ITEM_MANAGER::instance().GetTable(GetSocket(SASH_ABSORBED_SOCKET));
			if (pkItemAbsorbed)
			{
				if (pkItemAbsorbed->aApplies[i].bType == APPLY_NONE)
					continue;
				
				bType = pkItemAbsorbed->aApplies[i].bType;
				value = pkItemAbsorbed->aApplies[i].lValue;
				if (value < 0)
					continue;
				
				double dValue = value * GetSocket(SASH_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				value = (long) dValue;
				if ((pkItemAbsorbed->aApplies[i].lValue > 0) && (value <= 0))
					value += 1;
			}
			else
				continue;
		}
		#endif
		
#ifdef ENABLE_AURA_SYSTEM
		if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_AURA))
		{
			TItemTable* pkItemAbsorbed = ITEM_MANAGER::instance().GetTable(GetSocket(AURA_ABSORBED_SOCKET));
			if (pkItemAbsorbed)
			{
				if (pkItemAbsorbed->aApplies[i].bType == APPLY_NONE)
					continue;

				bType = pkItemAbsorbed->aApplies[i].bType;
				value = pkItemAbsorbed->aApplies[i].lValue;
				if (value < 0)
					continue;

				double dValue = value * (float(GetSocket(AURA_ABSORPTION_SOCKET) / 10.0f));
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				dValue = (double)dValue / 10;
				value = (long)dValue;
				if ((pkItemAbsorbed->aApplies[i].lValue > 0) && (value <= 0))
					value += 1;
			}
			else
				continue;
		}
#endif
		
		if (bType != APPLY_SKILL)
		{
			if (accessoryGrade != 0)
				value += MAX(accessoryGrade, value * aiAccessorySocketEffectivePct[accessoryGrade] / 100);
			
			m_pOwner->ApplyPoint(bType, bAdd ? value : -value);
		}
		else
			m_pOwner->ApplyPoint(bType, bAdd ? value : value ^ 0x00800000);
	}
	// ÃÊ½Â´ÞÀÇ ¹ÝÁö, ÇÒ·ÎÀ© »çÅÁ, Çàº¹ÀÇ ¹ÝÁö, ¿µ¿øÇÑ »ç¶ûÀÇ Ææ´øÆ®ÀÇ °æ¿ì
	// ±âÁ¸ÀÇ ÇÏµå ÄÚµùÀ¸·Î °­Á¦·Î ¼Ó¼ºÀ» ºÎ¿©ÇßÁö¸¸,
	// ±× ºÎºÐÀ» Á¦°ÅÇÏ°í special item group Å×ÀÌºí¿¡¼­ ¼Ó¼ºÀ» ºÎ¿©ÇÏµµ·Ï º¯°æÇÏ¿´´Ù.
	// ÇÏÁö¸¸ ÇÏµå ÄÚµùµÇ¾îÀÖÀ» ¶§ »ý¼ºµÈ ¾ÆÀÌÅÛÀÌ ³²¾ÆÀÖÀ» ¼öµµ ÀÖ¾î¼­ Æ¯¼öÃ³¸® ÇØ³õ´Â´Ù.
	// ÀÌ ¾ÆÀÌÅÛµéÀÇ °æ¿ì, ¹Ø¿¡ ITEM_UNIQUEÀÏ ¶§ÀÇ Ã³¸®·Î ¼Ó¼ºÀÌ ºÎ¿©µÇ±â ¶§¹®¿¡,
	// ¾ÆÀÌÅÛ¿¡ ¹ÚÇôÀÖ´Â attribute´Â Àû¿ëÇÏÁö ¾Ê°í ³Ñ¾î°£´Ù.
	if (true == CItemVnumHelper::IsRamadanMoonRing(GetVnum()) || true == CItemVnumHelper::IsHalloweenCandy(GetVnum())
		|| true == CItemVnumHelper::IsHappinessRing(GetVnum()) || true == CItemVnumHelper::IsLovePendant(GetVnum()))
	{
		// Do not anything.
	}
	else
	{
		for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		{
			if (GetAttributeType(i))
			{
				const TPlayerItemAttribute& ia = GetAttribute(i);
				long sValue = ia.sValue;
#ifdef __SASH_SYSTEM__
				if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_SASH))
				{
					double dValue = sValue * GetSocket(SASH_ABSORPTION_SOCKET);
					dValue = (double)dValue / 100;
					dValue = (double)dValue + .5;
					sValue = (long) dValue;
					if ((ia.sValue > 0) && (sValue <= 0))
						sValue += 1;
				}
#endif
#ifdef ENABLE_AURA_SYSTEM
				else if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_AURA))
				{
					double dValue = sValue * GetSocket(AURA_ABSORPTION_SOCKET);
					dValue = (double)dValue / 100;
					dValue = (double)dValue / 10;
					dValue = (double)dValue + .5;
					sValue = (long) dValue;
					if ((ia.sValue > 0) && (sValue <= 0))
						sValue += 1;
				}
#endif
#ifdef __DS_SET__
				CAffect* pkAffect = m_pOwner->FindAffect(NEW_AFFECT_DS_SET);
				if (pkAffect && IsDragonSoul())
					sValue += DSManager::instance().GetDSSetValue(i, ia.bType, GetVnum(), pkAffect->lApplyValue);
#endif
				if (ia.bType == APPLY_SKILL)
					m_pOwner->ApplyPoint(ia.bType, bAdd ? sValue : sValue ^ 0x00800000);
				else
					m_pOwner->ApplyPoint(ia.bType, bAdd ? sValue : -sValue);
			}
		}
	}

	switch (m_pProto->bType)
	{
		case ITEM_PICK:
		case ITEM_ROD:
			{
				if (bAdd)
				{
					if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
						m_pOwner->SetPart(PART_WEAPON, GetVnum());
				}
				else
				{
					if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
#ifdef __WEAPON_COSTUME_SYSTEM__
						m_pOwner->SetPart(PART_WEAPON, 0);
#else
						m_pOwner->SetPart(PART_WEAPON, m_pOwner->GetOriginalPart(PART_WEAPON));
#endif
				}
			}
			break;

		case ITEM_WEAPON:
			{
				const DWORD itemEvoSTRPlus[] = {0,120,240,360,480};
				m_pOwner->ApplyPoint(APPLY_ATT_GRADE_BONUS,bAdd ? itemEvoSTRPlus[GetEvolution()] : -itemEvoSTRPlus[GetEvolution()]);
				if(GetValue(1) > 0 && GetValue(2) > 0)
					m_pOwner->ApplyPoint(APPLY_MAGIC_ATT_GRADE,bAdd ? itemEvoSTRPlus[GetEvolution()] : -itemEvoSTRPlus[GetEvolution()]);

#ifdef __NEW_ARROW_SYSTEM__
				if (m_pProto->bSubType == WEAPON_ARROW || m_pProto->bSubType == WEAPON_UNLIMITED_ARROW)
				{
					if (bAdd)
					{
						if (m_wCell == INVENTORY_MAX_NUM + WEAR_ARROW)
						{
							m_pOwner->SetPart(PART_ARROW_TYPE, m_pProto->bSubType);
							const CItem* pWeapon = m_pOwner->GetWear(WEAR_WEAPON);
							if (pWeapon != NULL && pWeapon->GetSubType() == WEAPON_BOW)
							{
#ifdef __WEAPON_COSTUME_SYSTEM__
								const CItem* pCostumeWeapon = m_pOwner->GetWear(WEAR_COSTUME_WEAPON);
								if (pCostumeWeapon != NULL)
								{
									m_pOwner->SetPart(PART_WEAPON, pCostumeWeapon->GetVnum());
								}
								else
								{
									m_pOwner->SetPart(PART_WEAPON, pWeapon->GetVnum());
								}
#else
								m_pOwner->SetPart(PART_WEAPON, pWeapon->GetVnum());
#endif
							}
						}
					}
					else
					{
						if (m_wCell == INVENTORY_MAX_NUM + WEAR_ARROW)
							m_pOwner->SetPart(PART_ARROW_TYPE, m_pOwner->GetOriginalPart(PART_ARROW_TYPE));
					}
					
					break;
				}
#endif
#ifdef __NEW_ARROW_SYSTEM__
				const CItem* pArrow = m_pOwner->GetWear(WEAR_ARROW);
#endif
#ifdef __WEAPON_COSTUME_SYSTEM__
#ifdef ENABLE_COSTUME_HIDE_SYSTEM
		if (m_pOwner->GetWear(WEAR_COSTUME_WEAPON) != 0 && m_pOwner->GetQuestFlag("game_option.hide_costume_w") == 0)
			break;
#else
		if (0 != m_pOwner->GetWear(WEAR_COSTUME_WEAPON))
			break;
#endif
#endif
				if (bAdd)
				{
#ifdef __NEW_ARROW_SYSTEM__
					if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
						if (pArrow != NULL)
							m_pOwner->SetPart(PART_ARROW_TYPE, pArrow->GetSubType());
#endif
					if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
						m_pOwner->SetPart(PART_WEAPON, GetVnum());
				}
				else
				{
					if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
#ifdef __WEAPON_COSTUME_SYSTEM__
						m_pOwner->SetPart(PART_WEAPON, 0);
#else
						m_pOwner->SetPart(PART_WEAPON, m_pOwner->GetOriginalPart(PART_WEAPON));
#endif
				}
			}
			break;

		case ITEM_ARMOR:
			{
				// ÄÚ½ºÃõ body¸¦ ÀÔ°íÀÖ´Ù¸é armor´Â ¹þ´ø ÀÔ´ø »ó°ü ¾øÀÌ ºñÁÖ¾ó¿¡ ¿µÇâÀ» ÁÖ¸é ¾È µÊ.
#ifdef ENABLE_COSTUME_HIDE_SYSTEM
		if (m_pOwner->GetWear(WEAR_COSTUME_BODY) != 0 && m_pOwner->GetQuestFlag("game_option.hide_costume") == 0)
			break;
#else
		if (0 != m_pOwner->GetWear(WEAR_COSTUME_BODY))
			break;
#endif

				if (GetSubType() == ARMOR_BODY || GetSubType() == ARMOR_HEAD || GetSubType() == ARMOR_FOOTS || GetSubType() == ARMOR_SHIELD)
				{
					if (bAdd)
					{
						if (GetProto()->bSubType == ARMOR_BODY)
							m_pOwner->SetPart(PART_MAIN, GetVnum());
					}
					else
					{
						if (GetProto()->bSubType == ARMOR_BODY)
							m_pOwner->SetPart(PART_MAIN, m_pOwner->GetOriginalPart(PART_MAIN));
					}
				}
			}
			break;

		// ÄÚ½ºÃõ ¾ÆÀÌÅÛ ÀÔ¾úÀ» ¶§ Ä³¸¯ÅÍ parts Á¤º¸ ¼¼ÆÃ. ±âÁ¸ ½ºÅ¸ÀÏ´ë·Î Ãß°¡ÇÔ..
		case ITEM_COSTUME:
			{
				DWORD toSetValue = this->GetVnum();
				EParts toSetPart = PART_MAX_NUM;

				// °©¿Ê ÄÚ½ºÃõ
				if (GetSubType() == COSTUME_BODY)
				{
#ifdef ENABLE_COSTUME_HIDE_SYSTEM
			if (bAdd && m_pOwner->GetQuestFlag("game_option.hide_costume") == 1)
				break;
#endif
					toSetPart = PART_MAIN;

					if (false == bAdd)
					{
						// ÄÚ½ºÃõ °©¿ÊÀ» ¹þ¾úÀ» ¶§ ¿ø·¡ °©¿ÊÀ» ÀÔ°í ÀÖ¾ú´Ù¸é ±× °©¿ÊÀ¸·Î look ¼¼ÆÃ, ÀÔÁö ¾Ê¾Ò´Ù¸é default look
						const CItem* pArmor = m_pOwner->GetWear(WEAR_BODY);
						toSetValue = (NULL != pArmor) ? pArmor->GetVnum() : m_pOwner->GetOriginalPart(PART_MAIN);						
					}
					
				}
#ifdef __WEAPON_COSTUME_SYSTEM__
				else if (GetSubType() == COSTUME_WEAPON)
				{


#ifdef ENABLE_COSTUME_HIDE_SYSTEM
					if (bAdd && m_pOwner->GetQuestFlag("game_option.hide_costume_w") == 1)
						break;
#endif

					toSetPart = PART_WEAPON;
					if (!bAdd)
					{
						const CItem* pWeapon = m_pOwner->GetWear(WEAR_WEAPON);
						toSetValue = (NULL != pWeapon) ? pWeapon->GetVnum() : m_pOwner->GetPart(PART_WEAPON);						
					}
				}
#endif
				// Çì¾î ÄÚ½ºÃõ
				else if (GetSubType() == COSTUME_HAIR)
				{
#ifdef ENABLE_COSTUME_HIDE_SYSTEM
			if (bAdd && m_pOwner->GetQuestFlag("game_option.hide_costume_h") == 1)
				break;
#endif
					toSetPart = PART_HAIR;

					// ÄÚ½ºÃõ Çì¾î´Â shape°ªÀ» item protoÀÇ value3¿¡ ¼¼ÆÃÇÏµµ·Ï ÇÔ. Æ¯º°ÇÑ ÀÌÀ¯´Â ¾ø°í ±âÁ¸ °©¿Ê(ARMOR_BODY)ÀÇ shape°ªÀÌ ÇÁ·ÎÅäÀÇ value3¿¡ ÀÖ¾î¼­ Çì¾îµµ °°ÀÌ value3À¸·Î ÇÔ.
					// [NOTE] °©¿ÊÀº ¾ÆÀÌÅÛ vnumÀ» º¸³»°í Çì¾î´Â shape(value3)°ªÀ» º¸³»´Â ÀÌÀ¯´Â.. ±âÁ¸ ½Ã½ºÅÛÀÌ ±×·¸°Ô µÇ¾îÀÖÀ½...
					toSetValue = (true == bAdd) ? this->GetValue(3) : 0;
				}
				#ifdef __SASH_SYSTEM__
				else if (GetSubType() == COSTUME_SASH)
				{
					toSetValue -= 85000;
					if (GetSocket(SASH_ABSORPTION_SOCKET) >= SASH_EFFECT_FROM_ABS)
						toSetValue += 2000;
					
					toSetValue = (bAdd == true) ? toSetValue : 0;
					toSetPart = PART_SASH;
				}
				#endif
#ifdef ENABLE_AURA_SYSTEM
				else if (GetSubType() == COSTUME_AURA)
				{
					toSetValue = (bAdd == true) ? toSetValue : 0;
					toSetPart = PART_AURA;
				}
#endif
				if (PART_MAX_NUM != toSetPart)
				{
					m_pOwner->SetPart((BYTE)toSetPart, toSetValue);
					m_pOwner->UpdatePacket();
				}
			}
			break;
		case ITEM_UNIQUE:
			{
				if (0 != GetSIGVnum())
				{
					const CSpecialItemGroup* pItemGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(GetSIGVnum());
					if (NULL == pItemGroup)
						break;
					DWORD dwAttrVnum = pItemGroup->GetAttrVnum(GetVnum());
					const CSpecialAttrGroup* pAttrGroup = ITEM_MANAGER::instance().GetSpecialAttrGroup(dwAttrVnum);
					if (NULL == pAttrGroup)
						break;
					for (itertype (pAttrGroup->m_vecAttrs) it = pAttrGroup->m_vecAttrs.begin(); it != pAttrGroup->m_vecAttrs.end(); it++)
					{
						m_pOwner->ApplyPoint(it->apply_type, bAdd ? it->apply_value : -it->apply_value);
					}
				}
			}
			break;
	}
}

bool CItem::IsEquipable() const
{
	switch (this->GetType())
	{
	case ITEM_COSTUME:
	case ITEM_ARMOR:
	case ITEM_WEAPON:
	case ITEM_ROD:
	case ITEM_PICK:
	case ITEM_UNIQUE:
	case ITEM_DS:
	case ITEM_SPECIAL_DS:
	case ITEM_RING:
	case ITEM_BELT:

		return true;
	}

	return false;
}

// return false on error state
bool CItem::EquipTo(LPCHARACTER ch, BYTE bWearCell)
{
	if (!ch)
	{
		sys_err("EquipTo: nil character");
		return false;
	}

	// ¿ëÈ¥¼® ½½·Ô index´Â WEAR_MAX_NUM º¸´Ù Å­.
	if (IsDragonSoul())
	{
		if (bWearCell < WEAR_MAX_NUM || bWearCell >= WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX)
		{
			sys_err("EquipTo: invalid dragon soul cell (this: #%d %s wearflag: %d cell: %d)", GetOriginalVnum(), GetName(), GetSubType(), bWearCell - WEAR_MAX_NUM);
			return false;
		}
	}
	else
	{
		if (bWearCell >= WEAR_MAX_NUM)
		{
			sys_err("EquipTo: invalid wear cell (this: #%d %s wearflag: %d cell: %d)", GetOriginalVnum(), GetName(), GetWearFlag(), bWearCell);
			return false;
		}
	}

	if (ch->GetWear(bWearCell))
	{
		sys_err("EquipTo: item already exist (this: #%d %s cell: %d %s)", GetOriginalVnum(), GetName(), bWearCell, ch->GetWear(bWearCell)->GetName());
		return false;
	}

	if (GetOwner())
		RemoveFromCharacter();

	ch->SetWear(bWearCell, this); // ¿©±â¼­ ÆÐÅ¶ ³ª°¨

	m_pOwner = ch;
	m_bEquipped = true;
	m_wCell	= INVENTORY_MAX_NUM + bWearCell;

	DWORD dwImmuneFlag = 0;

	for (int i = 0; i < WEAR_MAX_NUM; ++i)
		if (m_pOwner->GetWear(i))
			SET_BIT(dwImmuneFlag, m_pOwner->GetWear(i)->m_pProto->dwImmuneFlag);

	m_pOwner->SetImmuneFlag(dwImmuneFlag);

	if (IsDragonSoul())
	{
		DSManager::instance().ActivateDragonSoul(this);
#ifdef __DS_SET__
		ch->DragonSoul_HandleSetBonus();
#endif
	}
	else
	{
		ModifyPoints(true);	
		StartUniqueExpireEvent();
		if (-1 != GetProto()->cLimitTimerBasedOnWearIndex)
			StartTimerBasedOnWearExpireEvent();

		// ACCESSORY_REFINE
		StartAccessorySocketExpireEvent();
		// END_OF_ACCESSORY_REFINE
	}

	ch->BuffOnAttr_AddBuffsFromItem(this);

	m_pOwner->ComputeBattlePoints();

	m_pOwner->UpdatePacket();

	Save();

	return (true);
}

bool CItem::Unequip()
{
	if (!m_pOwner || GetCell() < INVENTORY_MAX_NUM)
	{
		// ITEM_OWNER_INVALID_PTR_BUG
		sys_err("%s %u m_pOwner %p, GetCell %d", 
				GetName(), GetID(), get_pointer(m_pOwner), GetCell());
		// END_OF_ITEM_OWNER_INVALID_PTR_BUG
		return false;
	}

	if (this != m_pOwner->GetWear(GetCell() - INVENTORY_MAX_NUM))
	{
		sys_err("m_pOwner->GetWear() != this");
		return false;
	}

	//½Å±Ô ¸» ¾ÆÀÌÅÛ Á¦°Å½Ã Ã³¸®
	if (IsRideItem())
		ClearMountAttributeAndAffect();

	if (IsDragonSoul())
	{
		DSManager::instance().DeactivateDragonSoul(this);
	}
	else
	{
		ModifyPoints(false);
	}

	StopUniqueExpireEvent();

	if (-1 != GetProto()->cLimitTimerBasedOnWearIndex)
		StopTimerBasedOnWearExpireEvent();

	// ACCESSORY_REFINE
	StopAccessorySocketExpireEvent();
	// END_OF_ACCESSORY_REFINE


	m_pOwner->BuffOnAttr_RemoveBuffsFromItem(this);

	m_pOwner->SetWear(GetCell() - INVENTORY_MAX_NUM, NULL);

	DWORD dwImmuneFlag = 0;

	for (int i = 0; i < WEAR_MAX_NUM; ++i)
		if (m_pOwner->GetWear(i))
			SET_BIT(dwImmuneFlag, m_pOwner->GetWear(i)->m_pProto->dwImmuneFlag);

	m_pOwner->SetImmuneFlag(dwImmuneFlag);

	m_pOwner->ComputeBattlePoints();

	m_pOwner->UpdatePacket();

	m_pOwner = NULL;
	m_wCell = 0;
	m_bEquipped	= false;

	return true;
}

long CItem::GetValue(DWORD idx)
{
	assert(idx < ITEM_VALUES_MAX_NUM);
	return GetProto()->alValues[idx];
}

void CItem::SetExchanging(bool bOn)
{
	m_bExchanging = bOn;
}

void CItem::Save()
{
	if (m_bSkipSave)
		return;

	ITEM_MANAGER::instance().DelayedSave(this);
}

bool CItem::CreateSocket(BYTE bSlot, BYTE bGold)
{
	assert(bSlot < ITEM_SOCKET_MAX_NUM);

	if (m_alSockets[bSlot] != 0)
	{
		sys_err("Item::CreateSocket : socket already exist %s %d", GetName(), bSlot);
		return false;
	}

	if (bGold)
		m_alSockets[bSlot] = 2;
	else
		m_alSockets[bSlot] = 1;

	UpdatePacket();

	Save();
	return true;
}

void CItem::SetSockets(const long * c_al)
{
	thecore_memcpy(m_alSockets, c_al, sizeof(m_alSockets));
	Save();
}

void CItem::SetSocket(int i, long v, bool bLog)
{
	assert(i < ITEM_SOCKET_MAX_NUM);
	m_alSockets[i] = v;
	UpdatePacket();
	Save();
	if (bLog)
		LogManager::instance().ItemLog(i, v, 0, GetID(), "SET_SOCKET", "", "", GetOriginalVnum());
}

int CItem::GetGold()
{
	if (IS_SET(GetFlag(), ITEM_FLAG_COUNT_PER_1GOLD))
	{
		if (GetProto()->dwGold == 0)
			return GetCount();
		else
			return GetCount() / GetProto()->dwGold;
	}
	else
		return GetProto()->dwGold;
}

long long CItem::GetShopBuyPrice()
{
	return GetProto()->dwShopBuyPrice;
}

bool CItem::IsOwnership(LPCHARACTER ch)
{
	if (!m_pkOwnershipEvent)
		return true;

	return m_dwOwnershipPID == ch->GetPlayerID() ? true : false;
}

EVENTFUNC(ownership_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "ownership_event> <Factor> Null pointer" );
		return 0;
	}

	LPITEM pkItem = info->item;

	pkItem->SetOwnershipEvent(NULL);

	TPacketGCItemOwnership p;

	p.bHeader	= HEADER_GC_ITEM_OWNERSHIP;
	p.dwVID	= pkItem->GetVID();
	p.szName[0]	= '\0';

	pkItem->PacketAround(&p, sizeof(p));
	return 0;
}

void CItem::SetOwnershipEvent(LPEVENT pkEvent)
{
	m_pkOwnershipEvent = pkEvent;
}

void CItem::SetOwnership(LPCHARACTER ch, int iSec)
{
	if (!ch)
	{
		if (m_pkOwnershipEvent)
		{
			event_cancel(&m_pkOwnershipEvent);
			m_dwOwnershipPID = 0;

			TPacketGCItemOwnership p;

			p.bHeader	= HEADER_GC_ITEM_OWNERSHIP;
			p.dwVID	= m_dwVID;
			p.szName[0]	= '\0';

			PacketAround(&p, sizeof(p));
		}
		return;
	}

	if (m_pkOwnershipEvent)
		return;

	if (true == LC_IsEurope())
	{
		if (iSec <= 10)
			iSec = 30;
	}

	m_dwOwnershipPID = ch->GetPlayerID();

	item_event_info* info = AllocEventInfo<item_event_info>();
	strlcpy(info->szOwnerName, ch->GetName(), sizeof(info->szOwnerName));
	info->item = this;

	SetOwnershipEvent(event_create(ownership_event, info, PASSES_PER_SEC(iSec)));

	TPacketGCItemOwnership p;

	p.bHeader = HEADER_GC_ITEM_OWNERSHIP;
	p.dwVID = m_dwVID;
	strlcpy(p.szName, ch->GetName(), sizeof(p.szName));

	PacketAround(&p, sizeof(p));
}

int CItem::GetSocketCount()
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
	{
		if (GetSocket(i) == 0)
			return i;
	}
	return ITEM_SOCKET_MAX_NUM;
}

bool CItem::AddSocket()
{
	int count = GetSocketCount();
	if (count == ITEM_SOCKET_MAX_NUM)
		return false;
	m_alSockets[count] = 1;
	return true;
}

void CItem::AlterToSocketItem(int iSocketCount)
{
	if (iSocketCount >= ITEM_SOCKET_MAX_NUM)
	{
		sys_log(0, "Invalid Socket Count %d, set to maximum", ITEM_SOCKET_MAX_NUM);
		iSocketCount = ITEM_SOCKET_MAX_NUM;
	}

	for (int i = 0; i < iSocketCount; ++i)
		SetSocket(i, 1);
}

void CItem::AlterToMagicItem( int iSecondPct /*= 0*/, int iThirdPct /*= 0 */ )

{
	int idx = GetAttributeSetIndex();

	if (idx < 0)
		return;

	//      Appeariance Second Third
	// Weapon 50        20     5
	// Armor  30        10     2
	// Acc    20        10     1

	if (g_iUseLocale)
	{
		switch (GetType())
		{
			case ITEM_WEAPON:
				iSecondPct = 30;
				iThirdPct = 30;
				break;

			case ITEM_ARMOR:
			case ITEM_COSTUME:
				if (GetSubType() == ARMOR_BODY)
				{
					iSecondPct = 40;
					iThirdPct = 30;
				}
				else if (GetSubType() == COSTUME_WEAPON)
				{
					iSecondPct = 40;
					iThirdPct = 30;
				}	
				else
				{
					iSecondPct = 10;
					iThirdPct = 1;
				}
				break;

			default:
				return;
		}
	}
	else
	{
		switch (GetType())
		{
			case ITEM_WEAPON:
				iSecondPct = 30;
				iThirdPct = 15;
				break;

			case ITEM_ARMOR:
			case ITEM_COSTUME:
				if (GetSubType() == ARMOR_BODY)
				{
					iSecondPct = 40;
					iThirdPct = 30;
				}
				else if (GetSubType() == COSTUME_WEAPON)
				{
					iSecondPct = 40;
					iThirdPct = 30;
				}				
				else
				{
					iSecondPct = 10;
					iThirdPct = 5;
				}
				break;

			default:
				return;
		}
	}

	// 100% È®·ü·Î ÁÁÀº ¼Ó¼º ÇÏ³ª
	PutAttribute(aiItemMagicAttributePercentHigh);

	if (number(1, 100) <= iSecondPct)
		PutAttribute(aiItemMagicAttributePercentLow);

	if (number(1, 100) <= iThirdPct)
		PutAttribute(aiItemMagicAttributePercentLow);
}

DWORD CItem::GetRefineFromVnum()
{
	return ITEM_MANAGER::instance().GetRefineFromVnum(GetVnum());
}

int CItem::GetRefineLevel()
{
	const char* name = GetBaseName();
	char* p = const_cast<char*>(strrchr(name, '+'));

	if (!p)
		return 0;

	int	rtn = 0;
	str_to_number(rtn, p+1);

	const char* locale_name = GetName();
	p = const_cast<char*>(strrchr(locale_name, '+'));

	if (p)
	{
		int	locale_rtn = 0;
		str_to_number(locale_rtn, p+1);
		if (locale_rtn != rtn)
		{
			sys_err("refine_level_based_on_NAME(%d) is not equal to refine_level_based_on_LOCALE_NAME(%d).", rtn, locale_rtn);
		}
	}

	return rtn;
}

bool CItem::IsPolymorphItem()
{
	return GetType() == ITEM_POLYMORPH;
}

void CItem::BinekYolla()
{
	LPCHARACTER ch = GetOwner();
	
	if (!ch || !ch->GetDesc()) return;

	if (IsEquipped())
	{
		ch->StopRiding();		
		ch->SetHorseAppearance(0);
		ch->HorseSummon(false);
	}
}

void CItem::PetYolla()
{
	LPCHARACTER ch = GetOwner();
	
	if (!ch || !ch->GetDesc()) return;
	
	if (IsEquipped())
	{
		CPetSystem* petSystem = ch->GetPetSystem();

		if (0 != petSystem)
		{
			petSystem->Unsummon(ch->GetPetAppearance());
			ch->SetPetAppearance(0);	
		}
	}
}

EVENTFUNC(unique_expire_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "unique_expire_event> <Factor> Null pointer" );
		return 0;
	}

	LPITEM pkItem = info->item;

	if (pkItem->GetValue(2) == 0)
	{
		if (pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) <= 1)
		{
			sys_log(0, "UNIQUE_ITEM: expire %s %u", pkItem->GetName(), pkItem->GetID());
			pkItem->SetUniqueExpireEvent(NULL);
			ITEM_MANAGER::instance().RemoveItem(pkItem, "UNIQUE_EXPIRE");
			return 0;
		}
		else
		{
			pkItem->SetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME, pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) - 1);
			return PASSES_PER_SEC(60);
		}
	}
	else
	{
		time_t cur = get_global_time();
		
		if (pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) <= cur)
		{
			pkItem->SetUniqueExpireEvent(NULL);
			ITEM_MANAGER::instance().RemoveItem(pkItem, "UNIQUE_EXPIRE");
			return 0;
		}
		else
		{
			// °ÔÀÓ ³»¿¡ ½Ã°£Á¦ ¾ÆÀÌÅÛµéÀÌ ºü¸´ºü¸´ÇÏ°Ô »ç¶óÁöÁö ¾Ê´Â ¹ö±×°¡ ÀÖ¾î
			// ¼öÁ¤
			// by rtsummit
			if (pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) - cur < 600)
				return PASSES_PER_SEC(pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) - cur);
			else
				return PASSES_PER_SEC(600);
		}
	}
}

// ½Ã°£ ÈÄºÒÁ¦
// timer¸¦ ½ÃÀÛÇÒ ¶§¿¡ ½Ã°£ Â÷°¨ÇÏ´Â °ÍÀÌ ¾Æ´Ï¶ó, 
// timer°¡ ¹ßÈ­ÇÒ ¶§¿¡ timer°¡ µ¿ÀÛÇÑ ½Ã°£ ¸¸Å­ ½Ã°£ Â÷°¨À» ÇÑ´Ù.
EVENTFUNC(timer_based_on_wear_expire_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "expire_event <Factor> Null pointer" );
		return 0;
	}

	LPITEM pkItem = info->item;
	int remain_time = pkItem->GetSocket(ITEM_SOCKET_REMAIN_SEC) - processing_time/passes_per_sec;
	if (remain_time <= 0)
	{
		sys_log(0, "ITEM EXPIRED : expired %s %u", pkItem->GetName(), pkItem->GetID());
		pkItem->SetTimerBasedOnWearExpireEvent(NULL);
		pkItem->SetSocket(ITEM_SOCKET_REMAIN_SEC, 0);
	
		// ÀÏ´Ü timer based on wear ¿ëÈ¥¼®Àº ½Ã°£ ´Ù µÇ¾ú´Ù°í ¾ø¾ÖÁö ¾Ê´Â´Ù.
		if (pkItem->IsDragonSoul())
		{
			DSManager::instance().DeactivateDragonSoul(pkItem);
		}
		else
		{
			ITEM_MANAGER::instance().RemoveItem(pkItem, "TIMER_BASED_ON_WEAR_EXPIRE");
		}
		return 0;
	}
	pkItem->SetSocket(ITEM_SOCKET_REMAIN_SEC, remain_time);
	return PASSES_PER_SEC (MIN (60, remain_time));
}

void CItem::SetUniqueExpireEvent(LPEVENT pkEvent)
{
	m_pkUniqueExpireEvent = pkEvent;
}

void CItem::SetTimerBasedOnWearExpireEvent(LPEVENT pkEvent)
{
	m_pkTimerBasedOnWearExpireEvent = pkEvent;
}

EVENTFUNC(real_time_expire_event)
{
	const item_vid_event_info* info = reinterpret_cast<const item_vid_event_info*>(event->info);

	if (NULL == info)
		return 0;

	const LPITEM item = ITEM_MANAGER::instance().FindByVID( info->item_vid );

	if (NULL == item)
		return 0;

	const time_t current = get_global_time();

	if (current > item->GetSocket(0))
	{
		if (item->GetVnum() && item->IsNewMountItem())
			item->ClearMountAttributeAndAffect();

		if (COSTUME_MOUNT == item->GetSubType() && item->GetType() == ITEM_COSTUME)
		{
			item->BinekYolla();
		}
		if (USE_PET == item->GetSubType() && item->GetType() == ITEM_UNIQUE)
		{
			item->PetYolla();
		}
		ITEM_MANAGER::instance().RemoveItem(item, "REAL_TIME_EXPIRE");

		return 0;
	}

	return PASSES_PER_SEC(1);
}

void CItem::StartRealTimeExpireEvent()
{
	if (m_pkRealTimeExpireEvent)
		return;
	for (int i=0 ; i < ITEM_LIMIT_MAX_NUM ; i++)
	{
		if (LIMIT_REAL_TIME == GetProto()->aLimits[i].bType || LIMIT_REAL_TIME_START_FIRST_USE == GetProto()->aLimits[i].bType)
		{
			item_vid_event_info* info = AllocEventInfo<item_vid_event_info>();
			info->item_vid = GetVID();

			m_pkRealTimeExpireEvent = event_create( real_time_expire_event, info, PASSES_PER_SEC(1));

			sys_log(0, "REAL_TIME_EXPIRE: StartRealTimeExpireEvent");

			return;
		}
	}
}

bool CItem::IsRealTimeItem()
{
	if(!GetProto())
		return false;
	for (int i=0 ; i < ITEM_LIMIT_MAX_NUM ; i++)
	{
		if (LIMIT_REAL_TIME == GetProto()->aLimits[i].bType)
			return true;
	}
	return false;
}

void CItem::StartUniqueExpireEvent()
{
	if (GetType() != ITEM_UNIQUE)
		return;

	if (m_pkUniqueExpireEvent)
		return;

	//±â°£Á¦ ¾ÆÀÌÅÛÀÏ °æ¿ì ½Ã°£Á¦ ¾ÆÀÌÅÛÀº µ¿ÀÛÇÏÁö ¾Ê´Â´Ù
	if (IsRealTimeItem())
		return;

	// HARD CODING
	if (GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
		m_pOwner->ShowAlignment(false);

	int iSec = GetSocket(ITEM_SOCKET_UNIQUE_SAVE_TIME);

	if (iSec == 0)
		iSec = 60;
	else
		iSec = MIN(iSec, 60);

	SetSocket(ITEM_SOCKET_UNIQUE_SAVE_TIME, 0);

	item_event_info* info = AllocEventInfo<item_event_info>();
	info->item = this;

	SetUniqueExpireEvent(event_create(unique_expire_event, info, PASSES_PER_SEC(iSec)));
}

// ½Ã°£ ÈÄºÒÁ¦
// timer_based_on_wear_expire_event ¼³¸í ÂüÁ¶
void CItem::StartTimerBasedOnWearExpireEvent()
{
	if (m_pkTimerBasedOnWearExpireEvent)
		return;

	//±â°£Á¦ ¾ÆÀÌÅÛÀÏ °æ¿ì ½Ã°£Á¦ ¾ÆÀÌÅÛÀº µ¿ÀÛÇÏÁö ¾Ê´Â´Ù
	if (IsRealTimeItem())
		return;

	if (-1 == GetProto()->cLimitTimerBasedOnWearIndex)
		return;

	int iSec = GetSocket(0);
	
	// ³²Àº ½Ã°£À» ºÐ´ÜÀ§·Î ²÷±â À§ÇØ...
	if (0 != iSec)
	{
		iSec %= 60;
		if (0 == iSec)
			iSec = 60;
	}

	item_event_info* info = AllocEventInfo<item_event_info>();
	info->item = this;

	SetTimerBasedOnWearExpireEvent(event_create(timer_based_on_wear_expire_event, info, PASSES_PER_SEC(iSec)));
}

void CItem::StopUniqueExpireEvent()
{
	if (!m_pkUniqueExpireEvent)
		return;

	if (GetValue(2) != 0) // °ÔÀÓ½Ã°£Á¦ ÀÌ¿ÜÀÇ ¾ÆÀÌÅÛÀº UniqueExpireEvent¸¦ Áß´ÜÇÒ ¼ö ¾ø´Ù.
		return;

	// HARD CODING
	if (GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
		m_pOwner->ShowAlignment(true);

	SetSocket(ITEM_SOCKET_UNIQUE_SAVE_TIME, event_time(m_pkUniqueExpireEvent) / passes_per_sec);
	event_cancel(&m_pkUniqueExpireEvent);

	ITEM_MANAGER::instance().SaveSingleItem(this);
}

void CItem::StopTimerBasedOnWearExpireEvent()
{
	if (!m_pkTimerBasedOnWearExpireEvent)
		return;

	int remain_time = GetSocket(ITEM_SOCKET_REMAIN_SEC) - event_processing_time(m_pkTimerBasedOnWearExpireEvent) / passes_per_sec;

	SetSocket(ITEM_SOCKET_REMAIN_SEC, remain_time);
	event_cancel(&m_pkTimerBasedOnWearExpireEvent);

	ITEM_MANAGER::instance().SaveSingleItem(this);
}

void CItem::ApplyAddon(int iAddonType)
{
	CItemAddonManager::instance().ApplyAddonTo(iAddonType, this);
}

void CItem::ApplyAddon2(int iAddonType)
{
	CItemAddonManager::instance().ApplyAddonTo2(iAddonType, this);
}

int CItem::GetSpecialGroup() const
{ 
	return ITEM_MANAGER::instance().GetSpecialGroupFromItem(GetVnum()); 
}

//
// ¾Ç¼¼¼­¸® ¼ÒÄÏ Ã³¸®.
//
bool CItem::IsAccessoryForSocket()
{
	return (m_pProto->bType == ITEM_ARMOR && (m_pProto->bSubType == ARMOR_WRIST || m_pProto->bSubType == ARMOR_NECK || m_pProto->bSubType == ARMOR_EAR)) ||
		(m_pProto->bType == ITEM_BELT);				// 2013³â 2¿ù »õ·Î Ãß°¡µÈ 'º§Æ®' ¾ÆÀÌÅÛÀÇ °æ¿ì ±âÈ¹ÆÀ¿¡¼­ ¾Ç¼¼¼­¸® ¼ÒÄÏ ½Ã½ºÅÛÀ» ±×´ë·Î ÀÌ¿ëÇÏÀÚ°í ÇÔ.
}

void CItem::SetAccessorySocketGrade(int iGrade) 
{ 
	SetSocket(0, MINMAX(0, iGrade, GetAccessorySocketMaxGrade())); 

	int iDownTime = aiAccessorySocketDegradeTime[GetAccessorySocketGrade()];

	//if (test_server)
	//	iDownTime /= 60;

	SetAccessorySocketDownGradeTime(iDownTime);
}

void CItem::SetAccessorySocketMaxGrade(int iMaxGrade) 
{ 
	SetSocket(1, MINMAX(0, iMaxGrade, ITEM_ACCESSORY_SOCKET_MAX_NUM)); 
}

void CItem::SetAccessorySocketDownGradeTime(DWORD time) 
{ 
	SetSocket(2, time); 

	if (test_server && GetOwner())
		GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s¿¡¼­ ¼ÒÄÏ ºüÁú¶§±îÁö ³²Àº ½Ã°£ %d"), GetName(), time);
}

EVENTFUNC(accessory_socket_expire_event)
{
	item_vid_event_info* info = dynamic_cast<item_vid_event_info*>( event->info );

	if ( info == NULL )
	{
		sys_err( "accessory_socket_expire_event> <Factor> Null pointer" );
		return 0;
	}

	LPITEM item = ITEM_MANAGER::instance().FindByVID(info->item_vid);

	if (item->GetAccessorySocketDownGradeTime() <= 1)
	{
degrade:
		item->SetAccessorySocketExpireEvent(NULL);
		item->AccessorySocketDegrade();
		return 0;
	}
	else
	{
		int iTime = item->GetAccessorySocketDownGradeTime() - 60;

		if (iTime <= 1)
			goto degrade;

		item->SetAccessorySocketDownGradeTime(iTime);

		if (iTime > 60)
			return PASSES_PER_SEC(60);
		else
			return PASSES_PER_SEC(iTime);
	}
}

void CItem::StartAccessorySocketExpireEvent()
{
	if (!IsAccessoryForSocket())
		return;

	if (m_pkAccessorySocketExpireEvent)
		return;

	if (GetAccessorySocketMaxGrade() == 0)
		return;

	if (GetAccessorySocketGrade() == 0)
		return;
	
	if (GetSocket(4) == 999)
		return;

	int iSec = GetAccessorySocketDownGradeTime();
	SetAccessorySocketExpireEvent(NULL);

	if (iSec <= 1)
		iSec = 5;
	else
		iSec = MIN(iSec, 60);

	item_vid_event_info* info = AllocEventInfo<item_vid_event_info>();
	info->item_vid = GetVID();

	SetAccessorySocketExpireEvent(event_create(accessory_socket_expire_event, info, PASSES_PER_SEC(iSec)));
}

void CItem::StopAccessorySocketExpireEvent()
{
	if (!m_pkAccessorySocketExpireEvent)
		return;

	if (!IsAccessoryForSocket())
		return;

	int new_time = GetAccessorySocketDownGradeTime() - (60 - event_time(m_pkAccessorySocketExpireEvent) / passes_per_sec);

	event_cancel(&m_pkAccessorySocketExpireEvent);

	if (new_time <= 1)
	{
		AccessorySocketDegrade();
	}
	else
	{
		SetAccessorySocketDownGradeTime(new_time);
	}
}
		
bool CItem::IsRideItem()
{
	if (ITEM_UNIQUE == GetType() && UNIQUE_SPECIAL_RIDE == GetSubType())
		return true;
	if (ITEM_UNIQUE == GetType() && UNIQUE_SPECIAL_MOUNT_RIDE == GetSubType())
		return true;
	if (ITEM_COSTUME == GetType() && COSTUME_MOUNT == GetSubType())
		return true;
	return false;
}

bool CItem::IsPetItem()
{
	if (GetType() == ITEM_UNIQUE && GetSubType() == USE_PET)
		return true;
	
	return false;
}

bool CItem::IsRamadanRing()
{
	if (GetVnum() == UNIQUE_ITEM_RAMADAN_RING)
		return true;
	return false;
}

bool CItem::PermaCevher()
{
	if (GetVnum() >= 50641 && GetVnum() <= 50663)
		return true;
	return false;
}
void CItem::ClearMountAttributeAndAffect()
{
	LPCHARACTER ch = GetOwner();

	ch->RemoveAffect(AFFECT_MOUNT);
	ch->RemoveAffect(AFFECT_MOUNT_BONUS);

	ch->MountVnum(0);

	ch->PointChange(POINT_ST, 0);
	ch->PointChange(POINT_DX, 0);
	ch->PointChange(POINT_HT, 0);
	ch->PointChange(POINT_IQ, 0);
}

// fixme
// ÀÌ°Å Áö±ÝÀº ¾È¾´µ¥... ±Ùµ¥ È¤½Ã³ª ½Í¾î¼­ ³²°ÜµÒ.
// by rtsummit
bool CItem::IsNewMountItem()
{
	return (
		(ITEM_UNIQUE == GetType() && UNIQUE_SPECIAL_RIDE == GetSubType() && IS_SET(GetFlag(), ITEM_FLAG_QUEST_USE))
		|| (ITEM_UNIQUE == GetType() && UNIQUE_SPECIAL_MOUNT_RIDE == GetSubType() && IS_SET(GetFlag(), ITEM_FLAG_QUEST_USE))
	);
}

void CItem::SetAccessorySocketExpireEvent(LPEVENT pkEvent)
{
	m_pkAccessorySocketExpireEvent = pkEvent;
}

void CItem::AccessorySocketDegrade()
{
	if (GetAccessorySocketGrade() > 0)
	{
		LPCHARACTER ch = GetOwner();

		if (ch)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s¿¡ ¹ÚÇôÀÖ´ø º¸¼®ÀÌ »ç¶óÁý´Ï´Ù."), GetName());
		}

		ModifyPoints(false);
		SetAccessorySocketGrade(GetAccessorySocketGrade()-1);
		ModifyPoints(true);

		int iDownTime = aiAccessorySocketDegradeTime[GetAccessorySocketGrade()];

		if (test_server)
			iDownTime /= 60;

		SetAccessorySocketDownGradeTime(iDownTime);

		if (iDownTime)
			StartAccessorySocketExpireEvent();
	}
}

// ring¿¡ itemÀ» ¹ÚÀ» ¼ö ÀÖ´ÂÁö ¿©ºÎ¸¦ Ã¼Å©ÇØ¼­ ¸®ÅÏ
static const bool CanPutIntoRing(LPITEM ring, LPITEM item)
{
	const DWORD vnum = item->GetVnum();
	return false;
}

bool CItem::CanPutInto(LPITEM item)
{
	if (item->GetType() == ITEM_BELT)
		return this->GetSubType() == USE_PUT_INTO_BELT_SOCKET;

	else if(item->GetType() == ITEM_RING)
		return CanPutIntoRing(item, this);

	else if (item->GetType() != ITEM_ARMOR)
		return false;

	DWORD vnum = item->GetVnum();

	struct JewelAccessoryInfo
	{
		DWORD jewel;
		DWORD wrist;
		DWORD neck;
		DWORD ear;
	};
	const static JewelAccessoryInfo infos[] = { 
		{ 50634, 14420, 16220, 17220 }, 
		{ 50635, 14500, 16500, 17500 }, 
		{ 50636, 14520, 16520, 17520 }, 
		{ 50637, 14540, 16540, 17540 }, 
		{ 50638, 14560, 16560, 17560 }, 
		{ 50639, 14570, 16570, 17570 }, 
		{ 50640, 14230, 16230, 17230 }, 
		{ 50662, 14240, 16240, 17240 }, 
		{ 50661, 14580, 16580, 17580 }, 
		{ 50635, 24500, 19500, 18500 }, 
		{ 50636, 24520, 19520, 18520 }, 
		{ 50637, 24540, 19540, 18540 }, 
		{ 50638, 24560, 19560, 18560 }, 
		{ 50639, 24570, 19570, 18570 }, 
	};
	
	DWORD item_type = (item->GetVnum() / 10) * 10;
	for (unsigned int i = 0; i < sizeof(infos) / sizeof(infos[0]); i++)
	{
		const JewelAccessoryInfo& info = infos[i];
		switch(item->GetSubType())
		{
		case ARMOR_WRIST:
			if (info.wrist == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		case ARMOR_NECK:
			if (info.neck == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		case ARMOR_EAR:
			if (info.ear == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		}
	}
	if (item->GetSubType() == ARMOR_WRIST)
		vnum -= 14000;
	else if (item->GetSubType() == ARMOR_NECK)
		vnum -= 16000;
	else if (item->GetSubType() == ARMOR_EAR)
		vnum -= 17000;
	else
		return false;

	DWORD type = vnum / 20;

	if (type < 0 || type > 11)
	{
		type = (vnum - 170) / 20;

		if (50623 + type != GetVnum())
			return false;
		else
			return true;
	}
	else if (item->GetVnum() >= 16210 && item->GetVnum() <= 16219)
	{
		if (50625 != GetVnum())
			return false;
		else
			return true;
	}

	return 50623 + type == GetVnum();
}

bool CItem::CanPutInto2(LPITEM item)
{
	if (item->GetType() == ITEM_BELT)
		return this->GetSubType() == USE_PUT_INTO_BELT_SOCKET;

	else if(item->GetType() == ITEM_RING)
		return CanPutIntoRing(item, this);

	else if (item->GetType() != ITEM_ARMOR)
		return false;

	DWORD vnum = item->GetVnum();

	struct JewelAccessoryInfo
	{
		DWORD jewel;
		DWORD wrist;
		DWORD neck;
		DWORD ear;
	};
	const static JewelAccessoryInfo infos[] = { 
		{ 50641, 14020, 16020, 17020 }, ////bak?
		{ 50642, 14040, 16040, 17040 }, ////g?üþ
		{ 50643, 14100, 16100, 17100 }, ////abanoz
		{ 50644, 14120, 16120, 17120 }, ////inci
		{ 50645, 14200, 16200, 17200 }, ////cennetin
		{ 50646, 14220, 16220, 17220 }, ////ruh
		{ 50647, 14500, 16500, 17500 }, ////yakut
		{ 50648, 14520, 16520, 17520 }, ////grena
		{ 50649, 14540, 16540, 17540 }, ////z?r?
		{ 50650, 14560, 16560, 17560 }, ////safir
		{ 50651, 14570, 16570, 17570 }, ////turmalin
		{ 50654, 14140, 16140, 17140 }, ////beyazaltin
		{ 50655, 14230, 16230, 17230 }, ////Antik
		{ 50656, 14060, 16060, 17060 }, ////altin
		{ 50657, 14080, 16080, 17080 }, ////yesim
		{ 50658, 14160, 16160, 17160 }, ////kristal
		{ 50659, 14180, 16180, 17180 }, ////ametist
		{ 50660, 14580, 16580, 17580 }, ////permalit
		{ 50647, 24500, 19500, 18500 }, ////yakut2
		{ 50648, 24520, 19520, 18520 }, ////grena2
		{ 50649, 24540, 19540, 17500 }, ////zümrüt2
		{ 50650, 24560, 19560, 18560 }, ////safir2
		{ 50651, 24570, 19570, 18570 }, ////turmalin
		{ 50663, 14240, 16240, 17240 }, ////gök
		{ 50662, 14240, 16240, 17240 }, ////gök
	};
	
	DWORD item_type = (item->GetVnum() / 10) * 10;
	for (unsigned int i = 0; i < sizeof(infos) / sizeof(infos[0]); i++)
	{
		const JewelAccessoryInfo& info = infos[i];
		switch(item->GetSubType())
		{
		case ARMOR_WRIST:
			if (info.wrist == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		case ARMOR_NECK:
			if (info.neck == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		case ARMOR_EAR:
			if (info.ear == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		}
	}
	if (item->GetSubType() == ARMOR_WRIST)
		vnum -= 14000;
	else if (item->GetSubType() == ARMOR_NECK)
		vnum -= 16000;
	else if (item->GetSubType() == ARMOR_EAR)
		vnum -= 17000;
	else
		return false;

	DWORD type = vnum / 20;

	if (type < 0 || type > 11)
	{
		type = (vnum - 170) / 20;

		if (50623 + type != GetVnum())
			return false;
		else
			return true;
	}
	else if (item->GetVnum() >= 16210 && item->GetVnum() <= 16219)
	{
		if (50625 != GetVnum())
			return false;
		else
			return true;
	}
	else if (item->GetVnum() >= 16230 && item->GetVnum() <= 16239)
	{
		if (50626 != GetVnum())
			return false;
		else
			return true;
	}
	else if (item->GetVnum() >= 16240 && item->GetVnum() <= 16249)
	{
		if (50626 != GetVnum())
			return false;
		else
			return true;
	}
	return 50623 + type == GetVnum();
}

// PC_BANG_ITEM_ADD
bool CItem::IsPCBangItem()
{
	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		if (m_pProto->aLimits[i].bType == LIMIT_PCBANG)
			return true;
	}
	return false;
}
// END_PC_BANG_ITEM_ADD

bool CItem::CheckItemUseLevel(int nLevel)
{
	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		if (this->m_pProto->aLimits[i].bType == LIMIT_LEVEL)
		{
			if (this->m_pProto->aLimits[i].lValue > nLevel) return false;
			else return true;
		}
	}
	return true;
}

long CItem::FindApplyValue(BYTE bApplyType)
{
	if (m_pProto == NULL)
		return 0;

	for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
	{
		if (m_pProto->aApplies[i].bType == bApplyType)
			return m_pProto->aApplies[i].lValue;
	}

	return 0;
}

void CItem::CopySocketTo(LPITEM pItem)
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		pItem->m_alSockets[i] = m_alSockets[i];
	}
}

int CItem::GetAccessorySocketGrade()
{
   	return MINMAX(0, GetSocket(0), GetAccessorySocketMaxGrade()); 
}

int CItem::GetAccessorySocketMaxGrade()
{
   	return MINMAX(0, GetSocket(1), ITEM_ACCESSORY_SOCKET_MAX_NUM);
}

int CItem::GetAccessorySocketDownGradeTime()
{
	return MINMAX(0, GetSocket(2), aiAccessorySocketDegradeTime[GetAccessorySocketGrade()]);
}

void CItem::AttrLog()
{
	const char * pszIP = NULL;

	if (GetOwner() && GetOwner()->GetDesc())
		pszIP = GetOwner()->GetDesc()->GetHostName();

	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		if (m_alSockets[i])
		{
			LogManager::instance().ItemLog(i, m_alSockets[i], 0, GetID(), "INFO_SOCKET", "", pszIP ? pszIP : "", GetOriginalVnum());
		}
	}

	for (int i = 0; i<ITEM_ATTRIBUTE_MAX_NUM; ++i)
	{
		int	type	= m_aAttr[i].bType;
		int value	= m_aAttr[i].sValue;

		if (type)
			LogManager::instance().ItemLog(i, type, value, GetID(), "INFO_ATTR", "", pszIP ? pszIP : "", GetOriginalVnum());
	}
}

#ifdef ENABLE_EXTENDED_ITEMNAME_ON_GROUND
const char * CItem::GetName()
{
	static char szItemName[128];
	memset(szItemName, 0, sizeof(szItemName));
	if (GetProto())
	{
		int len = 0;
		switch (GetType())
		{
			case ITEM_POLYMORPH:
			{
				const DWORD dwMobVnum = GetSocket(0);
				const CMob* pMob = CMobManager::instance().Get(dwMobVnum);
				if (pMob)
					len = snprintf(szItemName, sizeof(szItemName), "%s", pMob->m_table.szLocaleName);

				break;
			}
			case ITEM_SKILLBOOK:
			case ITEM_SKILLFORGET:
			{
				const DWORD dwSkillVnum = (GetVnum() == ITEM_SKILLBOOK_VNUM || GetVnum() == ITEM_SKILLFORGET_VNUM) ? GetSocket(0) : 0;
				const CSkillProto* pSkill = (dwSkillVnum != 0) ? CSkillManager::instance().Get(dwSkillVnum) : NULL;
				if (pSkill)
					len = snprintf(szItemName, sizeof(szItemName), "%s", pSkill->szName);

				break;
			}
		}
		len += snprintf(szItemName + len, sizeof(szItemName) - len, (len>0)?" %s":"%s", GetProto()->szLocaleName);
	}

	return szItemName;
}
#endif
int CItem::GetLevelLimit()
{
	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		if (this->m_pProto->aLimits[i].bType == LIMIT_LEVEL)
		{
			return this->m_pProto->aLimits[i].lValue;
		}
	}
	return 0;
}

bool CItem::OnAfterCreatedItem()
{
	// ¾ÆÀÌÅÛÀ» ÇÑ ¹øÀÌ¶óµµ »ç¿ëÇß´Ù¸é, ±× ÀÌÈÄ¿£ »ç¿ë ÁßÀÌÁö ¾Ê¾Æµµ ½Ã°£ÀÌ Â÷°¨µÇ´Â ¹æ½Ä
	if (-1 != this->GetProto()->cLimitRealTimeFirstUseIndex)
	{
		// Socket1¿¡ ¾ÆÀÌÅÛÀÇ »ç¿ë È½¼ö°¡ ±â·ÏµÇ¾î ÀÖÀ¸´Ï, ÇÑ ¹øÀÌ¶óµµ »ç¿ëÇÑ ¾ÆÀÌÅÛÀº Å¸ÀÌ¸Ó¸¦ ½ÃÀÛÇÑ´Ù.
		if (0 != GetSocket(1))
		{
			StartRealTimeExpireEvent();
		}
	}

	return true;
}


#ifdef __AUCTION__

// °æ¸ÅÀå
// window¸¦ °æ¸ÅÀåÀ¸·Î ÇÑ´Ù.

bool CItem::MoveToAuction()
{
	LPCHARACTER owner = GetOwner();
	if (owner == NULL)
	{
		sys_err ("Item those owner is not exist cannot regist in auction");
		return false;
	}
	
	if (GetWindow() == AUCTION)
	{
		sys_err ("Item is already in auction.");
	}

	SetWindow(AUCTION);
	owner->SetItem(m_bCell, NULL);
	Save();
	ITEM_MANAGER::instance().FlushDelayedSave(this);

	return true;
}

void CItem::CopyToRawData (TPlayerItem* new_item)
{
	if (new_item != NULL)
		return;

	new_item->id = m_dwID;
	new_item->window = m_bWindow;
	new_item->pos = m_bCell;
	new_item->count = m_dwCount;

	new_item->vnum = GetVnum();
	thecore_memcpy (new_item->alSockets, m_alSockets, sizeof (m_alSockets));
	thecore_memcpy (new_item->aAttr, m_aAttr, sizeof (m_aAttr));

	new_item->owner = m_pOwner->GetPlayerID();
}
#endif

bool CItem::IsDragonSoul()
{
	return GetType() == ITEM_DS;
}

#ifdef ENABLE_SPECIAL_STORAGE
bool CItem::IsUpgradeItem()
{
	if (GetType() == ITEM_MATERIAL && GetSubType() == MATERIAL_LEATHER)
	{
		return true;
	}
	switch (GetVnum())
	{
		case 27992:
		case 27993:
		case 27994:
		case 71520:
		case 71521:
		case 71522:
		case 27987:
		case 51001:
		case 55003:
		case 55004:
		case 51005:
		case 55010:
		case 55011:
		case 55012:
		case 55013:
		case 55014:
		case 55015:
		case 55016:
		case 55017:
		case 55018:
		case 55019:
		case 55020:
		case 55021:
		case 55022:
		case 55023:
		case 55024:
		case 55025:
		case 55026:
		case 55027:
		case 55034:
		case 55035:
		case 55036:
			return true;
		
	}
	
	return false;
	
	// return (GetType() == ITEM_MATERIAL && GetSubType() == MATERIAL_LEATHER);
}
bool CItem::IsStone()
{
	//return (GetType() == ITEM_METIN && GetSubType() == METIN_NORMAL);
	switch (GetVnum())
	{
		case 50926:
		case 50927:
			return true;
		
	}
	return false;
}
bool CItem::IsChest()
{
	return GetType() == ITEM_GIFTBOX || GetVnum() == 30300 || GetVnum() == 50255 || GetVnum() == 51510 || GetVnum() == 49015 || GetVnum() == 55009;
}
bool CItem::IsAttr()
{
	switch (GetVnum())
	{
		case 18900:
		case 50621:
		case 50628:
		case 50629:
		case 50630:
		case 50631:
		case 50632:
		case 50633:
		case 50643:
		case 50644:
		case 50645:
		case 50646:
		case 50647:
		case 50648:
		case 50649:
		case 50650:
		case 50651:
		case 50652:
		case 50654:
		case 50658:
		case 50659:
		case 50635:
		case 50661:
		case 50634:
		case 50636:
		case 50660:
		case 50637:
		case 50638:
		case 50639:
		case 50640:
		case 50617:
		case 50655:
		case 50662:
		case 50663:
		
			return true;
	}

	return false;
}
#endif
bool CItem::IsAttrMount()
{
	switch (GetVnum())
	{
		case 71209:
		case 71210:
		case 71211:
		case 71213:
		case 71214:
		case 71218:
		case 71219:
		case 71221:
		case 71252:
		case 71256:
		case 71257:
		case 71260:
		case 71262:
		case 71263:
		case 71264:
		case 71265:
		case 71267:
		case 71268:
		case 71269:
		case 71270:
		case 71271:
		case 71273:
		case 56075:
		case 56076:
		case 56077:
		case 56078:
		case 71226:
		case 71227:
		case 71228:
		case 71229:
		case 71230:
		case 71231:
		case 71232:
		case 71250:
		case 71251:
		case 71236:
		case 71234:
		case 71235:
		case 71233:
		case 71537:
		case 71538:
		case 71539:
		case 71540:
		case 71541:
		case 71542:
		case 71543:
		case 71544:
		case 71545:
		case 71546:
		case 71547:
		case 71548:
		case 71549:
		case 71550:
		case 71551:
		case 71552:
		case 71553:
		case 71554:
		case 71555:
		case 71556:
		case 71557:
		case 71558:
		case 71559:
		case 71560:
		case 71561:
		case 71562:
		case 71563:
		case 71564:
		case 71565:
		case 71566:
		case 71567:
		case 71568:
		case 71569:
		case 71570:
		case 71571:
		case 71572:
		case 71573:
		case 71574:
			return true;
		
	}
	return false;
}
int CItem::GiveMoreTime_Per(float fPercent)
{
	if (IsDragonSoul())
	{
		DWORD duration = DSManager::instance().GetDuration(this);
		unsigned int remain_sec = GetSocket(ITEM_SOCKET_REMAIN_SEC);
		int given_time = fPercent * duration / 100;
		if (remain_sec == duration)
			return false;
		if ((given_time + remain_sec) >= duration)
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, duration);
			return duration - remain_sec;
		}
		else
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, given_time + remain_sec);
			return given_time;
		}
	}
	// ¿ì¼± ¿ëÈ¥¼®¿¡ °üÇØ¼­¸¸ ÇÏµµ·Ï ÇÑ´Ù.
	else
		return 0;
}

int CItem::GiveMoreTime_Fix(DWORD dwTime)
{
	if (IsDragonSoul())
	{
		DWORD duration = DSManager::instance().GetDuration(this);
		unsigned int remain_sec = GetSocket(ITEM_SOCKET_REMAIN_SEC);
		if (remain_sec == duration)
			return false;
		if ((dwTime + remain_sec) >= duration)
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, duration);
			return duration - remain_sec; 
		}
		else
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, dwTime + remain_sec);
			return dwTime;
		}
	}
	// ¿ì¼± ¿ëÈ¥¼®¿¡ °üÇØ¼­¸¸ ÇÏµµ·Ï ÇÑ´Ù.
	else
		return 0;
}


int	CItem::GetDuration()
{
	if(!GetProto())
		return -1;

	for (int i=0 ; i < ITEM_LIMIT_MAX_NUM ; i++)
	{
		if (LIMIT_REAL_TIME == GetProto()->aLimits[i].bType)
			return GetProto()->aLimits[i].lValue;
	}
	
	if (-1 != GetProto()->cLimitTimerBasedOnWearIndex)
		return GetProto()->aLimits[GetProto()->cLimitTimerBasedOnWearIndex].lValue;	

	return -1;
}

bool CItem::IsSameSpecialGroup(const LPITEM item) const
{
	// ¼­·Î VNUMÀÌ °°´Ù¸é °°Àº ±×·ìÀÎ °ÍÀ¸·Î °£ÁÖ
	if (this->GetVnum() == item->GetVnum())
		return true;

	if (GetSpecialGroup() && (item->GetSpecialGroup() == GetSpecialGroup()))
		return true;

	return false;
}


