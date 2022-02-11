#include "stdafx.h"
#include "../../libgame/src/grid.h"
#include "utils.h"
#include "desc.h"
#include "desc_client.h"
#include "char.h"
#include "item.h"
#include "item_manager.h"
#include "packet.h"
#include "log.h"
#include "db.h"
#include "locale_service.h"
#include "../../common/length.h"
#include "exchange.h"
#include "DragonSoul.h"
#if defined(ENABLE_MESSENGER_BLOCK)
#include "messenger_manager.h"
#endif

#ifdef YANG_LIMIT
void exchange_packet(LPCHARACTER ch, BYTE sub_header, bool is_me, ULDWORD arg1, TItemPos arg2, DWORD arg3, void * pvData = NULL);
void exchange_packet(LPCHARACTER ch, BYTE sub_header, bool is_me, ULDWORD arg1, TItemPos arg2, DWORD arg3, void * pvData)
#else
void exchange_packet(LPCHARACTER ch, BYTE sub_header, bool is_me, DWORD arg1, TItemPos arg2, DWORD arg3, void * pvData = NULL);
void exchange_packet(LPCHARACTER ch, BYTE sub_header, bool is_me, DWORD arg1, TItemPos arg2, DWORD arg3, void * pvData)
#endif
{
	if (!ch->GetDesc())
		return;

	struct packet_exchange pack_exchg;

	pack_exchg.header 		= HEADER_GC_EXCHANGE;
	pack_exchg.sub_header 	= sub_header;
	pack_exchg.is_me		= is_me;
	pack_exchg.arg1		= arg1;
	pack_exchg.arg2		= arg2;
	pack_exchg.arg3		= arg3;

	if (sub_header == EXCHANGE_SUBHEADER_GC_ITEM_ADD && pvData)
	{
		thecore_memcpy(&pack_exchg.alSockets, ((LPITEM) pvData)->GetSockets(), sizeof(pack_exchg.alSockets));
		thecore_memcpy(&pack_exchg.aAttr, ((LPITEM) pvData)->GetAttributes(), sizeof(pack_exchg.aAttr));
		pack_exchg.evolution = ((LPITEM) pvData)->GetEvolution();
	}
	else
	{
		memset(&pack_exchg.alSockets, 0, sizeof(pack_exchg.alSockets));
		memset(&pack_exchg.aAttr, 0, sizeof(pack_exchg.aAttr));
		pack_exchg.evolution = 0;
	}

	ch->GetDesc()->Packet(&pack_exchg, sizeof(pack_exchg));
}

// ��ȯ�� ����
bool CHARACTER::ExchangeStart(LPCHARACTER victim)
{
	if (this == victim)	// �ڱ� �ڽŰ��� ��ȯ�� ���Ѵ�.
		return false;

	if (IsObserverMode())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ���¿����� ��ȯ�� �� �� �����ϴ�."));
		return false;
	}

	if (victim->IsNPC())
		return false;

#if defined(ENABLE_MESSENGER_BLOCK)
	if (MessengerManager::instance().CheckMessengerList(GetName(), victim->GetName(), MESSENGER_BLOCK))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Unblock %s to continue."), victim->GetName());
		return false;
	}
	else if (MessengerManager::instance().CheckMessengerList(victim->GetName(), GetName(), MESSENGER_BLOCK))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s has blocked you."), victim->GetName());
		return false;
	}
#endif

	//PREVENT_TRADE_WINDOW
#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if ( IsOpenSafebox() || GetShopOwner() || GetMyShop() || IsCubeOpen() || GetOfflineShopOwner())
	{
		ChatPacket( CHAT_TYPE_INFO, LC_TEXT("�ٸ� �ŷ�â�� ����������� �ŷ��� �Ҽ� �����ϴ�." ) );
		return false;
	}

	if ( victim->IsOpenSafebox() || victim->GetShopOwner() || victim->GetMyShop() || victim->IsCubeOpen() || victim->GetOfflineShopOwner())
	{
		ChatPacket( CHAT_TYPE_INFO, LC_TEXT("������ �ٸ� �ŷ����̶� �ŷ��� �Ҽ� �����ϴ�." ) );
		return false;
	}
#else
	if (IsOpenSafebox() || GetShopOwner() || GetMyShop() || IsCubeOpen())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�ٸ� �ŷ�â�� ����������� �ŷ��� �Ҽ� �����ϴ�."));
		return false;
	}

	if (victim->IsOpenSafebox() || victim->GetShopOwner() || victim->GetMyShop() || victim->IsCubeOpen())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �ٸ� �ŷ����̶� �ŷ��� �Ҽ� �����ϴ�."));
		return false;
	}
#endif

	if (m_pkTimedEventCh)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Hizli ch durduruldu."));
		event_cancel(&m_pkTimedEventCh);
	}
#ifdef WJ_SECURITY_SYSTEM
	if (IsActivateSecurity() == true)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GUVENLIK_AKTIF_HATA"));
		return false;
	}

	if (victim->IsActivateSecurity() == true)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("GUVENLIK_AKTIF_HATA"));
		return false;
	}
#endif

	//END_PREVENT_TRADE_WINDOW
	int iDist = DISTANCE_APPROX(GetX() - victim->GetX(), GetY() - victim->GetY());

	// �Ÿ� üũ
	if (iDist >= EXCHANGE_MAX_DISTANCE)
		return false;

	if (GetExchange())
		return false;

	if (victim->GetExchange())
	{
		exchange_packet(this, EXCHANGE_SUBHEADER_GC_ALREADY, 0, 0, NPOS, 0);
		return false;
	}

	if (victim->IsBlockMode(BLOCK_EXCHANGE))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ��ȯ �ź� �����Դϴ�."));
		return false;
	}

	SetExchange(M2_NEW CExchange(this));
	victim->SetExchange(M2_NEW CExchange(victim));

	victim->GetExchange()->SetCompany(GetExchange());
	GetExchange()->SetCompany(victim->GetExchange());

	//
	SetExchangeTime();
	victim->SetExchangeTime();

	exchange_packet(victim, EXCHANGE_SUBHEADER_GC_START, 0, GetVID(), NPOS, 0);
	exchange_packet(this, EXCHANGE_SUBHEADER_GC_START, 0, victim->GetVID(), NPOS, 0);

	return true;
}

CExchange::CExchange(LPCHARACTER pOwner)
{
	m_pCompany = NULL;

	m_bAccept = false;

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		m_apItems[i] = NULL;
		m_aItemPos[i] = NPOS;
		m_abItemDisplayPos[i] = 0;
	}

	m_lGold = 0;
#ifdef ENABLE_CHEQUE_SYSTEM
	m_lCheque = 0;
#endif
	m_pOwner = pOwner;
	pOwner->SetExchange(this);

#ifdef __NEW_EXCHANGE_WINDOW__
	m_pGrid = M2_NEW CGrid(6, 4);
#else
	m_pGrid = M2_NEW CGrid(4, 3);
#endif
}

CExchange::~CExchange()
{
	M2_DELETE(m_pGrid);
}

#if defined(ITEM_CHECKINOUT_UPDATE)
int CExchange::GetEmptyExchange(BYTE size)
{
	for (unsigned int i = 0; m_pGrid && i < m_pGrid->GetSize(); i++)
		if (m_pGrid->IsEmpty(i, 1, size))
			return i;

	return -1;
}
bool CExchange::AddItem(TItemPos item_pos, BYTE display_pos, bool SelectPosAuto)
#else
bool CExchange::AddItem(TItemPos item_pos, BYTE display_pos)
#endif
{
	assert(m_pOwner != NULL && GetCompany());

	if (!item_pos.IsValidItemPosition())
		return false;

	// ���� ��ȯ�� �� ����
	if (item_pos.IsEquipPosition())
		return false;

	LPITEM item;

	if (!(item = m_pOwner->GetItem(item_pos)))
		return false;

	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_GIVE))
	{
		m_pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�������� �ǳ��� �� �����ϴ�."));
		return false;
	}

	if (true == item->isLocked())
	{
		return false;
	}

	// �̹� ��ȯâ�� �߰��� �������ΰ�?
	if (item->IsExchanging())
	{
		sys_log(0, "EXCHANGE under exchanging");
		return false;
	}


#if defined(ITEM_CHECKINOUT_UPDATE)
	if (SelectPosAuto)
	{
		int AutoPos = GetEmptyExchange(item->GetSize());
		if (AutoPos == -1)
		{
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, "<EXCHANGE> You don't have enough space.");
			return false;
		}
		display_pos = AutoPos;
	}
#endif

	if (!m_pGrid->IsEmpty(display_pos, 1, item->GetSize()))
	{
		sys_log(0, "EXCHANGE not empty item_pos %d %d %d", display_pos, 1, item->GetSize());
		return false;
	}

	Accept(false);
	GetCompany()->Accept(false);

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (m_apItems[i])
			continue;

		m_apItems[i]		= item;
		m_aItemPos[i]		= item_pos;
		m_abItemDisplayPos[i]	= display_pos;
		m_pGrid->Put(display_pos, 1, item->GetSize());

		item->SetExchanging(true);

		exchange_packet(m_pOwner, 
				EXCHANGE_SUBHEADER_GC_ITEM_ADD,
				true,
				item->GetVnum(),
				TItemPos(RESERVED_WINDOW, display_pos),
				item->GetCount(),
				item);

		exchange_packet(GetCompany()->GetOwner(),
				EXCHANGE_SUBHEADER_GC_ITEM_ADD, 
				false, 
				item->GetVnum(),
				TItemPos(RESERVED_WINDOW, display_pos),
				item->GetCount(),
				item);

		sys_log(0, "EXCHANGE AddItem success %s pos(%d, %d) %d", item->GetName(), item_pos.window_type, item_pos.cell, display_pos);

		return true;
	}

	// �߰��� ������ ����
	return false;
}

bool CExchange::RemoveItem(BYTE pos)
{
	if (pos >= EXCHANGE_ITEM_MAX_NUM)
		return false;

	if (!m_apItems[pos])
		return false;

	TItemPos PosOfInventory = m_aItemPos[pos];
	m_apItems[pos]->SetExchanging(false);

	m_pGrid->Get(m_abItemDisplayPos[pos], 1, m_apItems[pos]->GetSize());

	exchange_packet(GetOwner(),	EXCHANGE_SUBHEADER_GC_ITEM_DEL, true, pos, NPOS, 0);
	exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_GC_ITEM_DEL, false, pos, PosOfInventory, 0);

	Accept(false);
	GetCompany()->Accept(false);

	m_apItems[pos]	    = NULL;
	m_aItemPos[pos]	    = NPOS;
	m_abItemDisplayPos[pos] = 0;
	return true;
}

#ifdef YANG_LIMIT
bool CExchange::AddGold(LDWORD lGold)
#else
bool CExchange::AddGold(long lGold)
#endif
{
	if (lGold <= 0)
		return false;

	if (GetOwner()->GetGold() < lGold)
	{
		// ������ �ִ� ���� ����.
		exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_LESS_GOLD, 0, 0, NPOS, 0);
		return false;
	}

	if ( LC_IsCanada() == true || LC_IsEurope() == true )
	{
		if ( m_lGold > 0 )
		{
			return false;
		}
	}

	Accept(false);
	GetCompany()->Accept(false);

	m_lGold = lGold;

	exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_GOLD_ADD, true, m_lGold, NPOS, 0);
	exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_GC_GOLD_ADD, false, m_lGold, NPOS, 0);
	return true;
}

#ifdef ENABLE_CHEQUE_SYSTEM
bool CExchange::AddCheque(long cheque)
{
	if (cheque <= 0)
		return false;

	if (GetOwner()->GetCheque() < cheque)
	{
		// ??? ?? ?? ??.
		exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_LESS_CHEQUE, 0, 0, NPOS, 0);
		return false;
	}

	LPCHARACTER	victim = GetCompany()->GetOwner();

	if(m_lCheque)
	{
		long vic_cheque = victim->GetCheque() + m_lCheque;
		if(vic_cheque > CHEQUE_MAX)
		{
			exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_LESS_CHEQUE, 0, 0, NPOS, 0);
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This user can not accept that much cheque."));
			return false;
		}
	}

	if ( LC_IsCanada() == true || LC_IsEurope() == true )
	{
		if ( m_lCheque > 0 )
		{
			return false;
		}
	}

	Accept(false);
	GetCompany()->Accept(false);

	m_lCheque = cheque;

	exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_CHEQUE_ADD, true, m_lCheque, NPOS, 0);
	exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_GC_CHEQUE_ADD, false, m_lCheque, NPOS, 0);
	return true;
}
#endif
// ���� ����� �ִ���, ��ȯ�Ϸ��� �������� ������ �ִ��� Ȯ�� �Ѵ�.
bool CExchange::Check(int * piItemCount)
{
	if (GetOwner()->GetGold() < m_lGold)
		return false;
#ifdef ENABLE_CHEQUE_SYSTEM
	if (GetOwner()->GetCheque() < m_lCheque)
		return false;
#endif

	int item_count = 0;

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!m_apItems[i])
			continue;

		if (!m_aItemPos[i].IsValidItemPosition())
			return false;

		if (m_apItems[i] != GetOwner()->GetItem(m_aItemPos[i]))
			return false;

		++item_count;
	}

	*piItemCount = item_count;
	return true;
}

int CExchange::CheckSpaceUpgrade()
{

	int item_count = 0;
	LPITEM item;
	
	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!m_apItems[i])
			continue;

		if (!m_aItemPos[i].IsValidItemPosition())
			return false;

		if (m_apItems[i] != GetOwner()->GetItem(m_aItemPos[i]))
			return false;
		
		item = GetOwner()->GetItem(m_aItemPos[i]);
		if (!item->IsUpgradeItem())
		{
			continue;
			
		}

		++item_count;
	}
	
	return item_count;
}

int CExchange::CheckSpaceStone()
{

	int item_count = 0;
	LPITEM item;
	
	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!m_apItems[i])
			continue;

		if (!m_aItemPos[i].IsValidItemPosition())
			return false;

		if (m_apItems[i] != GetOwner()->GetItem(m_aItemPos[i]))
			return false;
		
		item = GetOwner()->GetItem(m_aItemPos[i]);
		if (!item->IsStone())
		{
			continue;
			
		}

		++item_count;
	}
	
	return item_count;
}
int CExchange::CheckSpaceChest()
{

	int item_count = 0;
	LPITEM item;
	
	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!m_apItems[i])
			continue;

		if (!m_aItemPos[i].IsValidItemPosition())
			return false;

		if (m_apItems[i] != GetOwner()->GetItem(m_aItemPos[i]))
			return false;
		
		item = GetOwner()->GetItem(m_aItemPos[i]);
		if (!item->IsChest())
		{
			continue;
			
		}

		++item_count;
	}
	
	return item_count;
}
int CExchange::CheckSpaceAttr()
{

	int item_count = 0;
	LPITEM item;
	
	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!m_apItems[i])
			continue;

		if (!m_aItemPos[i].IsValidItemPosition())
			return false;

		if (m_apItems[i] != GetOwner()->GetItem(m_aItemPos[i]))
			return false;
		
		item = GetOwner()->GetItem(m_aItemPos[i]);
		if (!item->IsAttr())
		{
			continue;
			
		}

		++item_count;
	}
	
	return item_count;
}
bool CExchange::CheckSpace()
{
	static CGrid s_grid1(5, INVENTORY_MAX_NUM / 5 / 4); // inven page 1   9 Rows a 5 Columns
	static CGrid s_grid2(5, INVENTORY_MAX_NUM / 5 / 4); // inven page 2   9 Rows a 5 Columns
	static CGrid s_grid3(5, INVENTORY_MAX_NUM / 5 / 4); // inven page 3   9 Rows a 5 Columns
	static CGrid s_grid4(5, INVENTORY_MAX_NUM / 5 / 4); // inven page 4   9 Rows a 5 Columns

	s_grid1.Clear();
	s_grid2.Clear();
	s_grid3.Clear();
	s_grid4.Clear();

	LPCHARACTER	victim = GetCompany()->GetOwner();
	LPITEM item;

	int i;

	const int perPageSlotCount = INVENTORY_MAX_NUM / 4;

	for (i = 0; i < INVENTORY_MAX_NUM; ++i) {
		if (!(item = victim->GetInventoryItem(i)))
			continue;

		BYTE itemSize = item->GetSize();

		if (i < perPageSlotCount) // Notice: This is adjusted for 4 Pages only!
			s_grid1.Put(i, 1, itemSize);
		else if (i < perPageSlotCount * 2)
			s_grid2.Put(i - perPageSlotCount, 1, itemSize);
		else if (i < perPageSlotCount * 3)
			s_grid3.Put(i - perPageSlotCount * 2, 1, itemSize);
		else
			s_grid4.Put(i - perPageSlotCount * 3, 1, itemSize);
	}
#ifdef ENABLE_SPECIAL_STORAGE
	LPITEM item2,item3;
	static CGrid s_upp_grid1(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 4);
	static CGrid s_upp_grid2(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 4);
	static CGrid s_upp_grid3(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 4);
	static CGrid s_upp_grid4(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 4);
	static CGrid s_stone_grid1(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 4);
	static CGrid s_stone_grid2(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 4);
	static CGrid s_stone_grid3(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 4);
	static CGrid s_stone_grid4(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 4);
	static CGrid s_chest_grid1(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 4);
	static CGrid s_chest_grid2(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 4);
	static CGrid s_chest_grid3(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 4);
	static CGrid s_chest_grid4(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 4);
	static CGrid s_attr_grid1(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 4);
	static CGrid s_attr_grid2(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 4);
	static CGrid s_attr_grid3(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 4);
	static CGrid s_attr_grid4(5, SPECIAL_INVENTORY_MAX_NUM / 5 / 2);

	s_upp_grid1.Clear();
	s_upp_grid2.Clear();
	s_upp_grid3.Clear();
	s_upp_grid4.Clear();
	s_stone_grid1.Clear();
	s_stone_grid2.Clear();
	s_stone_grid3.Clear();
	s_stone_grid4.Clear();
	s_chest_grid1.Clear();
	s_chest_grid2.Clear();
	s_chest_grid3.Clear();
	s_chest_grid4.Clear();
	s_attr_grid1.Clear();
	s_attr_grid2.Clear();
	s_attr_grid3.Clear();
	s_attr_grid4.Clear();

	int upgde_cnt = 0;
	const int perPageSlotCount2 = SPECIAL_INVENTORY_MAX_NUM / 4;
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if (!(item = victim->GetUpgradeInventoryItem(i)))
			continue;
		
		if (i < perPageSlotCount2)
		{
			s_upp_grid1.Put(i, 1, item->GetSize());
			upgde_cnt++;
		}
		else if (i < perPageSlotCount2*2)
		{
			s_upp_grid2.Put(i, 1, item->GetSize());
			upgde_cnt++;
		}
		else if (i < perPageSlotCount2*3)
		{
			s_upp_grid3.Put(i, 1, item->GetSize());
			upgde_cnt++;
		}
		else
		{
			s_upp_grid4.Put(i, 1, item->GetSize());
			upgde_cnt++;
		}
	}
	int stne_cnt = 0;
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if (!(item = victim->GetStoneInventoryItem(i)))
			continue;
		
		if (i < perPageSlotCount2)
		{
			s_stone_grid1.Put(i, 1, item->GetSize());
			stne_cnt++;
		}
		else if (i < perPageSlotCount2*2)
		{
			s_stone_grid2.Put(i, 1, item->GetSize());
			stne_cnt++;
		}
		else if (i < perPageSlotCount2*3)
		{
			s_stone_grid3.Put(i, 1, item->GetSize());
			stne_cnt++;
		}
		else
		{
			s_stone_grid4.Put(i, 1, item->GetSize());
			stne_cnt++;
		}
	}
	int chest_cnt = 0;
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if (!(item = victim->GetChestInventoryItem(i)))
			continue;
		
		if (i < perPageSlotCount2)
		{
			s_chest_grid1.Put(i, 1, item->GetSize());
			chest_cnt++;
		}
		else if (i < perPageSlotCount2*2)
		{
			s_chest_grid2.Put(i, 1, item->GetSize());
			chest_cnt++;
		}
		else if (i < perPageSlotCount2*3)
		{
			s_chest_grid3.Put(i, 1, item->GetSize());
			chest_cnt++;
		}
		else
		{
			s_chest_grid4.Put(i, 1, item->GetSize());
			chest_cnt++;
		}
	}
	int attr_cnt = 0;
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		if (!(item = victim->GetAttrInventoryItem(i)))
			continue;
		
		if (i < perPageSlotCount2)
		{
			s_attr_grid1.Put(i, 1, item->GetSize());
			attr_cnt++;
		}
		else if (i < perPageSlotCount2*2)
		{
			s_attr_grid2.Put(i, 1, item->GetSize());
			attr_cnt++;
		}
		else if (i < perPageSlotCount2*3)
		{
			s_attr_grid3.Put(i, 1, item->GetSize());
			attr_cnt++;
		}
		else
		{
			s_attr_grid4.Put(i, 1, item->GetSize());
			attr_cnt++;
		}
	}
#endif
	static std::vector <WORD> s_vDSGrid(DRAGON_SOUL_INVENTORY_MAX_NUM);

	bool bDSInitialized = false;
	int iEmptyCell;

	for (i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!(item = m_apItems[i]))
			continue;

		BYTE itemSize = item->GetSize();

		if (item->IsDragonSoul())
		{
			if (!victim->DragonSoul_IsQualified())
				return false;

			if (!bDSInitialized) {
				bDSInitialized = true;
				victim->CopyDragonSoulItemGrid(s_vDSGrid);
			}

			bool bExistEmptySpace = false;
			WORD wBasePos = DSManager::instance().GetBasePosition(item);
			if (wBasePos >= DRAGON_SOUL_INVENTORY_MAX_NUM)
				return false;

#ifdef ENABLE_EXTENDED_DS_INVENTORY
			for (int i = 0; i < (DRAGON_SOUL_BOX_SIZE* DRAGON_SOUL_INVENTORY_PAGE_COUNT); i++)
#else
			for (int i = 0; i < DRAGON_SOUL_BOX_SIZE; i++)
#endif
			{
				WORD wPos = wBasePos + i;
				if (0 == s_vDSGrid[wPos])
				{
					bool bEmpty = true;
					for (int j = 1; j < item->GetSize(); j++)
					{
						if (s_vDSGrid[wPos + j * DRAGON_SOUL_BOX_COLUMN_NUM])
						{
							bEmpty = false;
							break;
						}
					}
					if (bEmpty)
					{
						for (int j = 0; j < item->GetSize(); j++)
						{
							s_vDSGrid[wPos + j * DRAGON_SOUL_BOX_COLUMN_NUM] = wPos + 1;
						}
						bExistEmptySpace = true;
						break;
					}
				}
				if (bExistEmptySpace)
					break;
			}
			if (!bExistEmptySpace)
				return false;
		}
#ifdef ENABLE_SPECIAL_STORAGE
		else if(item->IsUpgradeItem())
		{
			if (upgde_cnt == 180)
				return false;
			
			int iPos = s_upp_grid1.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_upp_grid1.Put(iPos, 1, item->GetSize());
				continue;
			}

			iPos = s_upp_grid2.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_upp_grid2.Put(iPos, 1, item->GetSize());
				continue;
			}
			
			iPos = s_upp_grid3.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_upp_grid3.Put(iPos, 1, item->GetSize());
				continue;
			}
			
			iPos = s_upp_grid4.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_upp_grid4.Put(iPos, 1, item->GetSize());
				continue;
			}
			
			return false;
		}
		else if(item->IsStone())
		{
			if (stne_cnt == 180)
				return false;
			
			int iPos = s_stone_grid1.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_stone_grid1.Put(iPos, 1, item->GetSize());
				continue;
			}

			iPos = s_stone_grid2.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_stone_grid2.Put(iPos, 1, item->GetSize());
				continue;
			}
			
			iPos = s_stone_grid3.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_stone_grid3.Put(iPos, 1, item->GetSize());
				continue;
			}
			
			
			iPos = s_stone_grid4.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_stone_grid4.Put(iPos, 1, item->GetSize());
				continue;
			}
			
			
			return false;
		}
		else if(item->IsChest())
		{
			if (chest_cnt == 180)
				return false;
			
			int iPos = s_chest_grid1.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_chest_grid1.Put(iPos, 1, item->GetSize());
				continue;
			}

			iPos = s_chest_grid2.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_chest_grid2.Put(iPos, 1, item->GetSize());
				continue;
			}
			
			iPos = s_chest_grid3.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_chest_grid3.Put(iPos, 1, item->GetSize());
				continue;
			}
			
			
			iPos = s_chest_grid4.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_chest_grid4.Put(iPos, 1, item->GetSize());
				continue;
			}
			
			
			return false;
		}
		else if(item->IsAttr())
		{
			if (attr_cnt == 180)
				return false;
			
			int iPos = s_attr_grid1.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_attr_grid1.Put(iPos, 1, item->GetSize());
				continue;
			}

			iPos = s_attr_grid2.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_attr_grid2.Put(iPos, 1, item->GetSize());
				continue;
			}
			
			iPos = s_attr_grid3.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_attr_grid3.Put(iPos, 1, item->GetSize());
				continue;
			}
			
			
			iPos = s_attr_grid4.FindBlank(1, item->GetSize());
			if (iPos >= 0)
			{
				s_attr_grid4.Put(iPos, 1, item->GetSize());
				continue;
			}
			
			
			return false;
		}
#endif
		else
		{
			int iPos = s_grid1.FindBlank(1, itemSize);
			if (iPos >= 0) {
				s_grid1.Put(iPos, 1, itemSize);
				continue;
			}

			iPos = s_grid2.FindBlank(1, itemSize);
			if (iPos >= 0) {
				s_grid2.Put(iPos, 1, itemSize);
				continue;
			}

			iPos = s_grid3.FindBlank(1, itemSize);
			if (iPos >= 0) {
				s_grid3.Put(iPos, 1, itemSize);
				continue;
			}

			iPos = s_grid4.FindBlank(1, itemSize);
			if (iPos >= 0) {
				s_grid4.Put(iPos, 1, itemSize);
				continue;
			}

			return false;  // No space left in inventory
		}
	}

	return true;
}

// ��ȯ �� (�����۰� �� ���� ������ �ű��)
bool CExchange::Done()
{
	int		empty_pos, i;
	LPITEM	item;

	LPCHARACTER	victim = GetCompany()->GetOwner();
#ifdef ENABLE_CHEQUE_SYSTEM
	if(m_lCheque)
	{
		long vic_cheque = victim->GetCheque() + m_lCheque;
		if(vic_cheque > CHEQUE_MAX)
		{
//			exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_LESS_CHEQUE, 0, 0, NPOS, 0);
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("O usu?io n? pode atingir o limite m?imo de Won."));
			return false;
		}
	}
#endif
	for (i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!(item = m_apItems[i]))
			continue;

		if (item->IsDragonSoul())
			empty_pos = victim->GetEmptyDragonSoulInventory(item);
#ifdef ENABLE_SPECIAL_STORAGE
		else if(item->IsUpgradeItem())
			empty_pos = victim->GetEmptyUpgradeInventory(item);
		else if(item->IsStone())
			empty_pos = victim->GetEmptyStoneInventory(item);
		else if(item->IsChest())
			empty_pos = victim->GetEmptyChestInventory(item);
		else if(item->IsAttr())
			empty_pos = victim->GetEmptyAttrInventory(item);
#endif

		else
			empty_pos = victim->GetEmptyInventory(item->GetSize());

		if (empty_pos < 0)
		{
			sys_err("Exchange::Done : Cannot find blank position in inventory %s <-> %s item %s", 
					m_pOwner->GetName(), victim->GetName(), item->GetName());
			continue;
		}

		assert(empty_pos >= 0);

		if (item->GetVnum() == 90008 || item->GetVnum() == 90009) // VCARD
		{
			VCardUse(m_pOwner, victim, item);
			continue;
		}

		m_pOwner->SyncQuickslot(QUICKSLOT_TYPE_ITEM, item->GetCell(), 255);

		item->RemoveFromCharacter();
		if (item->IsDragonSoul())
			item->AddToCharacter(victim, TItemPos(DRAGON_SOUL_INVENTORY, empty_pos));
#ifdef ENABLE_SPECIAL_STORAGE
		else if(item->IsUpgradeItem())
			item->AddToCharacter(victim, TItemPos(UPGRADE_INVENTORY, empty_pos));
		else if(item->IsStone())
			item->AddToCharacter(victim, TItemPos(STONE_INVENTORY, empty_pos));
		else if(item->IsChest())
			item->AddToCharacter(victim, TItemPos(CHEST_INVENTORY, empty_pos));
		else if(item->IsAttr())
			item->AddToCharacter(victim, TItemPos(ATTR_INVENTORY, empty_pos));
#endif
		else
			item->AddToCharacter(victim, TItemPos(INVENTORY, empty_pos));
		ITEM_MANAGER::instance().FlushDelayedSave(item);

		item->SetExchanging(false);
		{

			char exchange_buf[51];

			snprintf(exchange_buf, sizeof(exchange_buf), "%s %u %u %u %u", item->GetName(), GetOwner()->GetPlayerID(), item->GetCount(),item->GetID(), item->GetVnum());
			LogManager::instance().ItemLog(victim, item, "EXCHANGE_TAKE", exchange_buf);

			snprintf(exchange_buf, sizeof(exchange_buf), "%s %u %u", item->GetName(), victim->GetPlayerID(), item->GetCount()); 
			LogManager::instance().ItemLog(GetOwner(), item, "EXCHANGE_GIVE", exchange_buf);

			if (item->GetVnum() >= 80003 && item->GetVnum() <= 80007)
			{
				LogManager::instance().GoldBarLog(victim->GetPlayerID(), item->GetID(), EXCHANGE_TAKE, "");
				LogManager::instance().GoldBarLog(GetOwner()->GetPlayerID(), item->GetID(), EXCHANGE_GIVE, "");
			}
		}

		m_apItems[i] = NULL;
	}

	if (m_lGold)
	{
#ifdef YANG_LIMIT
		GetOwner()->GoldChange(static_cast<LDWORD>(-m_lGold), "EXCHANGE_1");
		victim->GoldChange(static_cast<LDWORD>(m_lGold), "EXCHANGE_2");
#else
		GetOwner()->PointChange(POINT_GOLD, -m_lGold, true);
		victim->PointChange(POINT_GOLD, m_lGold, true);
#endif

		if (m_lGold > 1000)
		{
			char exchange_buf[51];
			snprintf(exchange_buf, sizeof(exchange_buf), "%u %s", GetOwner()->GetPlayerID(), GetOwner()->GetName());
			LogManager::instance().CharLog(victim, m_lGold, "EXCHANGE_GOLD_TAKE", exchange_buf);

			snprintf(exchange_buf, sizeof(exchange_buf), "%u %s", victim->GetPlayerID(), victim->GetName());
			LogManager::instance().CharLog(GetOwner(), m_lGold, "EXCHANGE_GOLD_GIVE", exchange_buf);
		}
	}
#ifdef ENABLE_CHEQUE_SYSTEM
	if (m_lCheque)
	{
		GetOwner()->PointChange(POINT_CHEQUE, -m_lCheque, true);
		victim->PointChange(POINT_CHEQUE, m_lCheque, true);

		if (m_lCheque > 100000)
		{
			char exchange_buf[51];
			snprintf(exchange_buf, sizeof(exchange_buf), "%u %s", GetOwner()->GetPlayerID(), GetOwner()->GetName());
			LogManager::instance().CharLog(victim, m_lCheque, "EXCHANGE_GOLD_TAKE", exchange_buf);

			snprintf(exchange_buf, sizeof(exchange_buf), "%u %s", victim->GetPlayerID(), victim->GetName());
			LogManager::instance().CharLog(GetOwner(), m_lCheque, "EXCHANGE_GOLD_GIVE", exchange_buf);
		}
	}
#endif
	m_pGrid->Clear();
	return true;
}

// ��ȯ�� ����
bool CExchange::Accept(bool bAccept)
{
	if (m_bAccept == bAccept)
		return true;

	m_bAccept = bAccept;

	// �� �� ���� �����Ƿ� ��ȯ ����
	if (m_bAccept && GetCompany()->m_bAccept)
	{
		int	iItemCount;

		LPCHARACTER victim = GetCompany()->GetOwner();

		//PREVENT_PORTAL_AFTER_EXCHANGE
		GetOwner()->SetExchangeTime();
		victim->SetExchangeTime();		
		//END_PREVENT_PORTAL_AFTER_EXCHANGE

		// exchange_check ������ ��ȯ�� �����۵��� ���ڸ��� �ֳ� Ȯ���ϰ�,
		// ��ũ�� ����� �ֳ� Ȯ���Ѵ�, �ι�° ���ڷ� ��ȯ�� ������ ����
		// �� �����Ѵ�.
		if (!Check(&iItemCount))
		{
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �����ϰų� �������� ���ڸ��� �����ϴ�."));
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ���� �����ϰų� �������� ���ڸ��� �����ϴ�."));
			goto EXCHANGE_END;
		}

		// ���� ���� ������ ������ ������ ����ǰ�� ���� �ڸ��� �ֳ� Ȯ���Ѵ�.
		if (!CheckSpace())
		{
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ����ǰ�� �� ������ �����ϴ�."));
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����ǰ�� �� ������ �����ϴ�."));
			goto EXCHANGE_END;
		}

		// ���浵 ����������..
		if (!GetCompany()->CheckSpace())
		{
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ����ǰ�� �� ������ �����ϴ�."));
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����ǰ�� �� ������ �����ϴ�."));
			goto EXCHANGE_END;
		}

		if (victim->GetNewEmptyUpgradeInventory() < CheckSpaceUpgrade())
		{
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ����ǰ�� �� ������ �����ϴ�."));
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����ǰ�� �� ������ �����ϴ�."));
			goto EXCHANGE_END;
		}
		
		if (GetOwner()->GetNewEmptyUpgradeInventory() < GetCompany()->CheckSpaceUpgrade())
		{
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ����ǰ�� �� ������ �����ϴ�."));
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����ǰ�� �� ������ �����ϴ�."));
			goto EXCHANGE_END;
		}

		if (victim->GetNewEmptyStoneInventory() < CheckSpaceStone())
		{
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ����ǰ�� �� ������ �����ϴ�."));
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����ǰ�� �� ������ �����ϴ�."));
			goto EXCHANGE_END;
		}
		
		if (GetOwner()->GetNewEmptyStoneInventory() < GetCompany()->CheckSpaceStone())
		{
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ����ǰ�� �� ������ �����ϴ�."));
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����ǰ�� �� ������ �����ϴ�."));
			goto EXCHANGE_END;
		}
		
		if (victim->GetNewEmptyChestInventory() < CheckSpaceChest())
		{
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ����ǰ�� �� ������ �����ϴ�."));
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����ǰ�� �� ������ �����ϴ�."));
			goto EXCHANGE_END;
		}
		
		if (GetOwner()->GetNewEmptyAttrInventory() < GetCompany()->CheckSpaceAttr())
		{
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ����ǰ�� �� ������ �����ϴ�."));
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����ǰ�� �� ������ �����ϴ�."));
			goto EXCHANGE_END;
		}

		if (db_clientdesc->GetSocket() == INVALID_SOCKET)
		{
			sys_err("Cannot use exchange feature while DB cache connection is dead.");
			victim->ChatPacket(CHAT_TYPE_INFO, "Unknown error");
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, "Unknown error");
			goto EXCHANGE_END;
		}

		if (Done())
		{
			if (m_lGold) // ���� ���� ���� ����
				GetOwner()->Save();
#ifdef ENABLE_CHEQUE_SYSTEM
			if (m_lCheque) // ?? ?? ??? ??
				GetOwner()->Save();
#endif
			if (GetCompany()->Done())
			{
				if (GetCompany()->m_lGold) // ���� ���� ���� ����
					victim->Save();
#ifdef ENABLE_CHEQUE_SYSTEM
				if (GetCompany()->m_lCheque) // ?? ?? ?? ??
					victim->Save();
#endif
				// INTERNATIONAL_VERSION
				GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s �԰��� ��ȯ�� ���� �Ǿ����ϴ�."), victim->GetName());
				victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s �԰��� ��ȯ�� ���� �Ǿ����ϴ�."), GetOwner()->GetName());
				// END_OF_INTERNATIONAL_VERSION
			}
		}

EXCHANGE_END:
		Cancel();
		return false;
	}
	else
	{
		// �ƴϸ� accept�� ���� ��Ŷ�� ������.
		exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_ACCEPT, true, m_bAccept, NPOS, 0);
		exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_GC_ACCEPT, false, m_bAccept, NPOS, 0);
		return true;
	}
}

// ��ȯ ���
void CExchange::Cancel()
{
	exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_END, 0, 0, NPOS, 0);
	GetOwner()->SetExchange(NULL);

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (m_apItems[i])
			m_apItems[i]->SetExchanging(false);
	}

	if (GetCompany())
	{
		GetCompany()->SetCompany(NULL);
		GetCompany()->Cancel();
	}

	M2_DELETE(this);
}

