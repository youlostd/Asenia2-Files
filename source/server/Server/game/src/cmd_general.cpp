#include "stdafx.h"
#ifdef __FreeBSD__
#include <md5.h>
#else
#include "../../libthecore/src/xmd5.h"
#endif

#include "utils.h"
#include "maintenance.h"
#include "config.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "motion.h"
#include "packet.h"
#include "affect.h"
#include "pvp.h"
#include "start_position.h"
#include "party.h"
#include "guild_manager.h"
#include "p2p.h"
#include "dungeon.h"
#include "messenger_manager.h"
#include "war_map.h"
#include "questmanager.h"
#include "item_manager.h"
#include "monarch.h"
#include "mob_manager.h"
#include "dev_log.h"
#include "item.h"
#include "arena.h"
#include "buffer_manager.h"
#include "unique_item.h"
#include "threeway_war.h"
#include "log.h"
#include "../../common/VnumHelper.h"
#include "../../common/service.h"
#ifdef __AUCTION__
#include "auction_manager.h"
#endif
#include "offlineshop_manager.h"
#include <string>
#ifdef ENABLE_BEVIS_BOSS_TRACKING_SYSTEM
#include "boss_tracking.h"
#endif
#ifdef ENABLE_TITLE_SYSTEM
#include "title.h"
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
#include "New_PetSystem.h"
#endif
#ifdef _ITEM_SHOP_SYSTEM
#include "item_shop.h"
#endif
#if defined(ENABLE_REMOTE_SHOP)
#include "shop_manager.h"
#endif
extern int g_server_id;

extern int g_nPortalLimitTime;

#ifdef ENABLE_COSTUME_HIDE_SYSTEM
ACMD(do_costume_config)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (!ch)
		return;

	int iValue = 0;
	str_to_number(iValue, arg1);
	ch->SetQuestFlag("game_option.hide_costume", iValue);
	if (iValue == 1)
	{
		LPITEM pkBody = ch->GetWear(WEAR_BODY);
		if (pkBody)
		{
			DWORD dwRes = pkBody->GetVnum();
			ch->SetPart(PART_MAIN, dwRes);
		}
	}
	else
	{
		LPITEM pkBody = ch->GetWear(WEAR_COSTUME_BODY);
		if (pkBody)
		{
			DWORD dwRes = pkBody->GetVnum();
			ch->SetPart(PART_MAIN, dwRes);
		}
	}
	ch->UpdatePacket();
}

ACMD(do_costume_w_config)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (!ch)
		return;

	int iValue = 0;
	str_to_number(iValue, arg1);
	ch->SetQuestFlag("game_option.hide_costume_w", iValue);
	if (iValue == 1)
	{
		LPITEM pkWeapon = ch->GetWear(WEAR_WEAPON);
		if (pkWeapon)
		{
			DWORD dwRes = pkWeapon->GetVnum();
			ch->SetPart(PART_WEAPON, dwRes);
		}
	}
	else
	{
		LPITEM pkWeapon = ch->GetWear(WEAR_COSTUME_WEAPON);
		if (pkWeapon)
		{
			DWORD dwRes = pkWeapon->GetVnum();
			ch->SetPart(PART_WEAPON, dwRes);
		}
	}
	ch->UpdatePacket();
}

ACMD(do_costume_h_config)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (!ch)
		return;

	int iValue = 0;
	str_to_number(iValue, arg1);
	ch->SetQuestFlag("game_option.hide_costume_h", iValue);
	if (iValue == 1)
		ch->SetPart(PART_HAIR, 0);
	else
	{
		LPITEM pkHair = ch->GetWear(WEAR_COSTUME_HAIR);
		if (pkHair)
		{
			ch->SetPart(PART_HAIR, pkHair->GetValue(3));
		}
	}
	ch->UpdatePacket();
}
#endif


#ifdef KYGN_ITEM_REMOVE_OR_SELL
ACMD(do_add_remove_or_sell_item_index)
{
	if(!ch || !ch->IsPC())
		return;
	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	if (!*arg1 || !*arg2)
		return;
	int SlotIndex;
	BYTE window_type;
	str_to_number(SlotIndex, arg1);
	str_to_number(window_type, arg2);

	LPITEM item;
	if (window_type == 0)
		item = ch->GetInventoryItem(SlotIndex);
	else if (window_type == 1)
		item = ch->GetUpgradeInventoryItem(SlotIndex);
	else if (window_type == 2)
		item = ch->GetStoneInventoryItem(SlotIndex);
	else if (window_type == 3)
		item = ch->GetChestInventoryItem(SlotIndex);
	else if (window_type == 4)
		item = ch->GetAttrInventoryItem(SlotIndex);
	else
		return;

	if (!item || item->IsEquipped() || item->IsExchanging() || true == item->isLocked())
		return;
	item->Lock(true);
	ch->R_S_I_AddItemSlot(SlotIndex, window_type);
}
ACMD(do_remove_item_system_button)
{
	if(!ch || !ch->IsPC())
		return;
	ch->RemoveORSellOperation(1);
}
ACMD(do_remove_or_sell_item_system_close)
{
	if(!ch || !ch->IsPC())
		return;
	ch->RemoveORSellOperation(2);
}
ACMD(do_sell_item_system_button)
{
	if(!ch || !ch->IsPC())
		return;
	ch->RemoveORSellOperation(3);
}
#endif

ACMD (do_sort_items)
{
    
	
	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	{
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
        return;
	}
	
	if (ch->IsSashCombinationOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("kombinasyon_penceresi_acik"));
		return;
	}
	
	if (ch->IsSashAbsorptionOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bonus_emis_penceresi_acik"));
		return;
	}
	
	/*if (ch->GetEnvTime() > get_global_time())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cokhizlisin"));
		return;
	}*/
	
	if (ch->IsWarping()) {return;}
	
	//f (ch->PazarAcikmi() == 1) {return;}
	
	//ch->SetEnvTime(get_global_time()+5);
	
		
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		LPITEM item = ch->GetInventoryItem(i);
		
		if(!item)
			continue;
		
		if(item->isLocked())
			continue;
		
		if(item->GetCount() == ITEM_MAX_COUNT)
			continue;
		
		if (item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
		{
			for (int j = i; j < INVENTORY_MAX_NUM; ++j)
			{
				LPITEM item2 = ch->GetInventoryItem(j);
				
				if(!item2)
					continue;
				
				if(item2->isLocked())
					continue;
	
				if (item2->GetVnum() == item->GetVnum())
				{
					bool bStopSockets = false;
					
					for (int k = 0; k < ITEM_SOCKET_MAX_NUM; ++k)
					{
						if (item2->GetSocket(k) != item->GetSocket(k))
						{
							bStopSockets = true;
							break;
						}
					}
					
					if(bStopSockets)
						continue;
	
					short bAddCount = MIN(ITEM_MAX_COUNT - item->GetCount(), item2->GetCount());
	
					item->SetCount(item->GetCount() + bAddCount);
					item2->SetCount(item2->GetCount() - bAddCount);
					
					continue;
				}
			}
		}
	}
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("envanterdizildi"));
}

ACMD (do_sort_items_with_k)
{
    
	
	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	{
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
        return;
	}
	
	if (ch->IsSashCombinationOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("kombinasyon_penceresi_acik"));
		return;
	}
	
	if (ch->IsSashAbsorptionOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bonus_emis_penceresi_acik"));
		return;
	}
	
	/*if (ch->GetKEnvTime() > get_global_time())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cokhizlisin"));
		return;
	}*/
	
	if (ch->IsWarping()) {return;}
	
	//if (ch->PazarAcikmi() == 1) {return;}
	
	//ch->SetKEnvTime(get_global_time()+5);
	
		
	for (int i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
	{
		LPITEM stoneitem = NULL;
		LPITEM materialitem = NULL;
		LPITEM chestitem = NULL;
		LPITEM attritem = NULL;
		
		for (i = 0; i < SPECIAL_INVENTORY_MAX_NUM; ++i)
		{
			if (stoneitem = ch->GetStoneInventoryItem(i))
			{
				if(!stoneitem)
					continue;
				
				if(stoneitem->isLocked())
					continue;
				
				if(stoneitem->GetCount() == ITEM_MAX_COUNT)
					continue;
				
				
				if (stoneitem->IsStackable() && !IS_SET(stoneitem->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					for (int j = i; j < INVENTORY_MAX_NUM; ++j)
					{
						LPITEM item2 = ch->GetStoneInventoryItem(j);
						
						if(!item2)
							continue;
						
						if(item2->isLocked())
							continue;
			
						if (item2->GetVnum() == stoneitem->GetVnum())
						{
							bool bStopSockets = false;
							
							for (int k = 0; k < ITEM_SOCKET_MAX_NUM; ++k)
							{
								if (item2->GetSocket(k) != stoneitem->GetSocket(k))
								{
									bStopSockets = true;
									break;
								}
							}
							
							if(bStopSockets)
								continue;
			
							short bAddCount = MIN(ITEM_MAX_COUNT - stoneitem->GetCount(), item2->GetCount());
			
							stoneitem->SetCount(stoneitem->GetCount() + bAddCount);
							item2->SetCount(item2->GetCount() - bAddCount);
							
							continue;
						}
					}
				}
				
			}
			if (materialitem = ch->GetUpgradeInventoryItem(i))
			{
				if(!materialitem)
					continue;
				
				if(materialitem->isLocked())
					continue;
				
				if(materialitem->GetCount() == ITEM_MAX_COUNT)
					continue;
				
				
				if (materialitem->IsStackable() && !IS_SET(materialitem->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					for (int j = i; j < INVENTORY_MAX_NUM; ++j)
					{
						LPITEM item2 = ch->GetUpgradeInventoryItem(j);
						
						if(!item2)
							continue;
						
						if(item2->isLocked())
							continue;
			
						if (item2->GetVnum() == materialitem->GetVnum())
						{
							bool bStopSockets = false;
							
							for (int k = 0; k < ITEM_SOCKET_MAX_NUM; ++k)
							{
								if (item2->GetSocket(k) != materialitem->GetSocket(k))
								{
									bStopSockets = true;
									break;
								}
							}
							
							if(bStopSockets)
								continue;
			
							short bAddCount = MIN(ITEM_MAX_COUNT - materialitem->GetCount(), item2->GetCount());
			
							materialitem->SetCount(materialitem->GetCount() + bAddCount);
							item2->SetCount(item2->GetCount() - bAddCount);
							
							continue;
						}
					}
				}
				
				
			}
			if (chestitem = ch->GetChestInventoryItem(i))
			{
				if(!chestitem)
					continue;
				
				if(chestitem->isLocked())
					continue;
				
				if(chestitem->GetCount() == ITEM_MAX_COUNT)
					continue;
				
				
				if (chestitem->IsStackable() && !IS_SET(chestitem->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					for (int j = i; j < INVENTORY_MAX_NUM; ++j)
					{
						LPITEM item2 = ch->GetChestInventoryItem(j);
						
						if(!item2)
							continue;
						
						if(item2->isLocked())
							continue;
			
						if (item2->GetVnum() == chestitem->GetVnum())
						{
							bool bStopSockets = false;
							
							for (int k = 0; k < ITEM_SOCKET_MAX_NUM; ++k)
							{
								if (item2->GetSocket(k) != chestitem->GetSocket(k))
								{
									bStopSockets = true;
									break;
								}
							}
							
							if(bStopSockets)
								continue;
			
							short bAddCount = MIN(ITEM_MAX_COUNT - chestitem->GetCount(), item2->GetCount());
			
							chestitem->SetCount(chestitem->GetCount() + bAddCount);
							item2->SetCount(item2->GetCount() - bAddCount);
							
							continue;
						}
					}
				}
				
			}
			if (attritem = ch->GetAttrInventoryItem(i))
			{
				if(!attritem)
					continue;
				
				if(attritem->isLocked())
					continue;
				
				if(attritem->GetCount() == ITEM_MAX_COUNT)
					continue;
				
				
				if (attritem->IsStackable() && !IS_SET(attritem->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					for (int j = i; j < INVENTORY_MAX_NUM; ++j)
					{
						LPITEM item2 = ch->GetAttrInventoryItem(j);
						
						if(!item2)
							continue;
						
						if(item2->isLocked())
							continue;
			
						if (item2->GetVnum() == attritem->GetVnum())
						{
							bool bStopSockets = false;
							
							for (int k = 0; k < ITEM_SOCKET_MAX_NUM; ++k)
							{
								if (item2->GetSocket(k) != attritem->GetSocket(k))
								{
									bStopSockets = true;
									break;
								}
							}
							
							if(bStopSockets)
								continue;
			
							short bAddCount = MIN(ITEM_MAX_COUNT - attritem->GetCount(), item2->GetCount());
			
							attritem->SetCount(attritem->GetCount() + bAddCount);
							item2->SetCount(item2->GetCount() - bAddCount);
							
							continue;
						}
					}
				}
				
			}
		}

	}
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("envanterdizildi"));
}

ACMD(do_ds_refbutton)
{
	
	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	{
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
        return;
	}
	
	if (ch->IsSashCombinationOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("kombinasyon_penceresi_acik"));
		return;
	}
	
	if (ch->IsSashAbsorptionOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bonus_emis_penceresi_acik"));
		return;
	}
	
	ch->DragonSoul_RefineWindow_Open(ch);
	
}
ACMD(do_stonesell0)
{
	
	if (1 == quest::CQuestManager::instance().GetEventFlag("stonesell"))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("devredisibirakildi"));
		return;
	}
	
	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	{
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
        return;
	}
	
	int stonesellflood = ch->GetQuestFlag("stonesell_flood");
	if (stonesellflood)
	{
		if (get_global_time() < stonesellflood + 1 /* limit */) 
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cokhizlisin"));
			return;
		}
	}			
	
	int adet0 = ch->CountSpecifyItem(28044);
	int adet = ch->CountSpecifyItem(28043);
	int adet1 = ch->CountSpecifyItem(28042);
	int adet2 = ch->CountSpecifyItem(28041);
	int adet3 = ch->CountSpecifyItem(28040);
	int adet4 = ch->CountSpecifyItem(28039);
	int adet5 = ch->CountSpecifyItem(28038);
	int adet6 = ch->CountSpecifyItem(28037);
	int adet7 = ch->CountSpecifyItem(28036);
	int adet8 = ch->CountSpecifyItem(28035);
	int adet9 = ch->CountSpecifyItem(28034);
	int adet10 = ch->CountSpecifyItem(28033);
	int adet11 = ch->CountSpecifyItem(28032);
	int adet12 = ch->CountSpecifyItem(28031);
	int adet13 = ch->CountSpecifyItem(28030);
	int adet14 = ch->CountSpecifyItem(28045);
	int adet15 = ch->CountSpecifyItem(28046);
	int total = adet0 + adet + adet1 + adet2 + adet3 + adet4 + adet5 + adet6 + adet7 + adet8 + adet9 + adet10 + adet11 + adet12 + adet13 + adet14 + adet15;
	
	if (total < 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("adet_yok"));
		return;
	}
	
	int won = ch->GetCheque();
	int gold = ch->GetGold();
	int tasfiyat = 75000;
	unsigned long long newMoney = tasfiyat * total;
	unsigned long long score = newMoney + gold;
	
	if (GOLD_MAX <= score || total > 26666)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Üzerinde çok fazla para taþýyorsun.");
		return;
	}

	ch->RemoveSpecifyItem(28044,adet0);
	ch->RemoveSpecifyItem(28043,adet);
	ch->RemoveSpecifyItem(28042,adet1);
	ch->RemoveSpecifyItem(28041,adet2);
	ch->RemoveSpecifyItem(28040,adet3);
	ch->RemoveSpecifyItem(28039,adet4);
	ch->RemoveSpecifyItem(28038,adet5);
	ch->RemoveSpecifyItem(28037,adet6);
	ch->RemoveSpecifyItem(28036,adet7);
	ch->RemoveSpecifyItem(28035,adet8);
	ch->RemoveSpecifyItem(28034,adet9);
	ch->RemoveSpecifyItem(28033,adet10);
	ch->RemoveSpecifyItem(28032,adet11);
	ch->RemoveSpecifyItem(28031,adet12);
	ch->RemoveSpecifyItem(28030,adet13);
	ch->RemoveSpecifyItem(28045,adet14);
	ch->RemoveSpecifyItem(28046,adet15);
#ifdef YANG_LIMIT
	ch->GoldChange(static_cast<LDWORD>(newMoney), "stonesell");
#else
	ch->PointChange(POINT_GOLD, newMoney, true);
#endif
	ch->ChatPacket(CHAT_TYPE_INFO, "Baþarýyla satýþ yaptýn.");
	ch->SetQuestFlag("stonesell_flood", get_global_time() + 5);
	return;		
}

ACMD(do_stonesell1)
{
	
	if (1 == quest::CQuestManager::instance().GetEventFlag("stonesell"))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("devredisibirakildi"));
		return;
	}
	
	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	{
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
        return;
	}
	
	int stonesellflood = ch->GetQuestFlag("stonesell_flood");
	if (stonesellflood)
	{
		if (get_global_time() < stonesellflood + 1 /* limit */) 
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cokhizlisin"));
			return;
		}
	}		
	
	int adet0 = ch->CountSpecifyItem(28144);
	int adet = ch->CountSpecifyItem(28143);
	int adet1 = ch->CountSpecifyItem(28142);
	int adet2 = ch->CountSpecifyItem(28141);
	int adet3 = ch->CountSpecifyItem(28140);
	int adet4 = ch->CountSpecifyItem(28139);
	int adet5 = ch->CountSpecifyItem(28138);
	int adet6 = ch->CountSpecifyItem(28137);
	int adet7 = ch->CountSpecifyItem(28136);
	int adet8 = ch->CountSpecifyItem(28135);
	int adet9 = ch->CountSpecifyItem(28134);
	int adet10 = ch->CountSpecifyItem(28133);
	int adet11 = ch->CountSpecifyItem(28132);
	int adet12 = ch->CountSpecifyItem(28131);
	int adet13 = ch->CountSpecifyItem(28130);
	int adet14 = ch->CountSpecifyItem(28145);
	int adet15 = ch->CountSpecifyItem(28146);
	int total = adet0 + adet + adet1 + adet2 + adet3 + adet4 + adet5 + adet6 + adet7 + adet8 + adet9 + adet10 + adet11 + adet12 + adet13 + adet14 + adet15;
	
	if (total < 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("adet_yok"));
		return;
	}
	
	int won = ch->GetCheque();
	int gold = ch->GetGold();
	int tasfiyat = 175000;
	unsigned long long newMoney = tasfiyat * total;
	unsigned long long score = newMoney + gold;
	
	if (GOLD_MAX <= score || total > 11428)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Üzerinde çok fazla para taþýyorsun.");
		return;
	}
	
	ch->RemoveSpecifyItem(28144,adet0);
	ch->RemoveSpecifyItem(28143,adet);
	ch->RemoveSpecifyItem(28142,adet1);
	ch->RemoveSpecifyItem(28141,adet2);
	ch->RemoveSpecifyItem(28140,adet3);
	ch->RemoveSpecifyItem(28139,adet4);
	ch->RemoveSpecifyItem(28138,adet5);
	ch->RemoveSpecifyItem(28137,adet6);
	ch->RemoveSpecifyItem(28136,adet7);
	ch->RemoveSpecifyItem(28135,adet8);
	ch->RemoveSpecifyItem(28134,adet9);
	ch->RemoveSpecifyItem(28133,adet10);
	ch->RemoveSpecifyItem(28132,adet11);
	ch->RemoveSpecifyItem(28131,adet12);
	ch->RemoveSpecifyItem(28130,adet13);
	ch->RemoveSpecifyItem(28145,adet14);
	ch->RemoveSpecifyItem(28146,adet15);
#ifdef YANG_LIMIT
	ch->GoldChange(static_cast<LDWORD>(newMoney), "stonesell");
#else
	ch->PointChange(POINT_GOLD, newMoney, true);
#endif
	ch->ChatPacket(CHAT_TYPE_INFO, "Baþarýyla satýþ yaptýn.");
	ch->SetQuestFlag("stonesell_flood", get_global_time() + 5);
	return;		
}

ACMD(do_stonesell2)
{
	
	if (1 == quest::CQuestManager::instance().GetEventFlag("stonesell"))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("devredisibirakildi"));
		return;
	}
	
	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	{
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
        return;
	}
	
	int stonesellflood = ch->GetQuestFlag("stonesell_flood");
	if (stonesellflood)
	{
		if (get_global_time() < stonesellflood + 1 /* limit */) 
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cokhizlisin"));
			return;
		}
	}		
	
	int adet0 = ch->CountSpecifyItem(28244);
	int adet = ch->CountSpecifyItem(28243);
	int adet1 = ch->CountSpecifyItem(28242);
	int adet2 = ch->CountSpecifyItem(28241);
	int adet3 = ch->CountSpecifyItem(28240);
	int adet4 = ch->CountSpecifyItem(28239);
	int adet5 = ch->CountSpecifyItem(28238);
	int adet6 = ch->CountSpecifyItem(28237);
	int adet7 = ch->CountSpecifyItem(28236);
	int adet8 = ch->CountSpecifyItem(28235);
	int adet9 = ch->CountSpecifyItem(28234);
	int adet10 = ch->CountSpecifyItem(28233);
	int adet11 = ch->CountSpecifyItem(28232);
	int adet12 = ch->CountSpecifyItem(28231);
	int adet13 = ch->CountSpecifyItem(28230);
	int adet14 = ch->CountSpecifyItem(28245);
	int adet15 = ch->CountSpecifyItem(28246);
	int total = adet0 + adet + adet1 + adet2 + adet3 + adet4 + adet5 + adet6 + adet7 + adet8 + adet9 + adet10 + adet11 + adet12 + adet13 + adet14 + adet15;
	
	if (total < 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("adet_yok"));
		return;
	}
	
	int won = ch->GetCheque();
	int gold = ch->GetGold();
	int tasfiyat = 350000;
	unsigned long long newMoney = tasfiyat * total;
	unsigned long long score = newMoney + gold;
	
	if (GOLD_MAX <= score || total > 5714)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Üzerinde çok fazla para taþýyorsun.");
		return;
	}
	
	ch->RemoveSpecifyItem(28244,adet0);
	ch->RemoveSpecifyItem(28243,adet);
	ch->RemoveSpecifyItem(28242,adet1);
	ch->RemoveSpecifyItem(28241,adet2);
	ch->RemoveSpecifyItem(28240,adet3);
	ch->RemoveSpecifyItem(28239,adet4);
	ch->RemoveSpecifyItem(28238,adet5);
	ch->RemoveSpecifyItem(28237,adet6);
	ch->RemoveSpecifyItem(28236,adet7);
	ch->RemoveSpecifyItem(28235,adet8);
	ch->RemoveSpecifyItem(28234,adet9);
	ch->RemoveSpecifyItem(28233,adet10);
	ch->RemoveSpecifyItem(28232,adet11);
	ch->RemoveSpecifyItem(28231,adet12);
	ch->RemoveSpecifyItem(28230,adet13);
	ch->RemoveSpecifyItem(28245,adet14);
	ch->RemoveSpecifyItem(28246,adet15);
#ifdef YANG_LIMIT
	ch->GoldChange(static_cast<LDWORD>(newMoney), "stonesell");
#else
	ch->PointChange(POINT_GOLD, newMoney, true);
#endif
	ch->ChatPacket(CHAT_TYPE_INFO, "Baþarýyla satýþ yaptýn.");
	ch->SetQuestFlag("stonesell_flood", get_global_time() + 5);
	return;		
}

ACMD(do_stonesell3)
{
	
	if (1 == quest::CQuestManager::instance().GetEventFlag("stonesell"))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("devredisibirakildi"));
		return;
	}
	
	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	{
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
        return;
	}
	
	int stonesellflood = ch->GetQuestFlag("stonesell_flood");
	if (stonesellflood)
	{
		if (get_global_time() < stonesellflood + 1 /* limit */) 
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cokhizlisin"));
			return;
		}
	}		
	
	int adet0 = ch->CountSpecifyItem(28344);
	int adet = ch->CountSpecifyItem(28343);
	int adet1 = ch->CountSpecifyItem(28342);
	int adet2 = ch->CountSpecifyItem(28341);
	int adet3 = ch->CountSpecifyItem(28340);
	int adet4 = ch->CountSpecifyItem(28339);
	int adet5 = ch->CountSpecifyItem(28338);
	int adet6 = ch->CountSpecifyItem(28337);
	int adet7 = ch->CountSpecifyItem(28336);
	int adet8 = ch->CountSpecifyItem(28335);
	int adet9 = ch->CountSpecifyItem(28334);
	int adet10 = ch->CountSpecifyItem(28333);
	int adet11 = ch->CountSpecifyItem(28332);
	int adet12 = ch->CountSpecifyItem(28331);
	int adet13 = ch->CountSpecifyItem(28330);
	int adet14 = ch->CountSpecifyItem(28345);
	int adet15 = ch->CountSpecifyItem(28346);
	int total = adet0 + adet + adet1 + adet2 + adet3 + adet4 + adet5 + adet6 + adet7 + adet8 + adet9 + adet10 + adet11 + adet12 + adet13 + adet14 + adet15;
	
	if (total < 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("adet_yok"));
		return;
	}
	
	int won = ch->GetCheque();
	int gold = ch->GetGold();
	int tasfiyat = 725000;
	unsigned long long newMoney = tasfiyat * total;
	unsigned long long score = newMoney + gold;
	
	if (GOLD_MAX <= score || total > 2758)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Üzerinde çok fazla para taþýyorsun.");
		return;
	}
	
	ch->RemoveSpecifyItem(28344,adet0);
	ch->RemoveSpecifyItem(28343,adet);
	ch->RemoveSpecifyItem(28342,adet1);
	ch->RemoveSpecifyItem(28341,adet2);
	ch->RemoveSpecifyItem(28340,adet3);
	ch->RemoveSpecifyItem(28339,adet4);
	ch->RemoveSpecifyItem(28338,adet5);
	ch->RemoveSpecifyItem(28337,adet6);
	ch->RemoveSpecifyItem(28336,adet7);
	ch->RemoveSpecifyItem(28335,adet8);
	ch->RemoveSpecifyItem(28334,adet9);
	ch->RemoveSpecifyItem(28333,adet10);
	ch->RemoveSpecifyItem(28332,adet11);
	ch->RemoveSpecifyItem(28331,adet12);
	ch->RemoveSpecifyItem(28330,adet13);
	ch->RemoveSpecifyItem(28345,adet14);
	ch->RemoveSpecifyItem(28346,adet15);
#ifdef YANG_LIMIT
	ch->GoldChange(static_cast<LDWORD>(newMoney), "stonesell");
#else
	ch->PointChange(POINT_GOLD, newMoney, true);
#endif
	ch->ChatPacket(CHAT_TYPE_INFO, "Baþarýyla satýþ yaptýn.");
	ch->SetQuestFlag("stonesell_flood", get_global_time() + 5);
	return;		
}

ACMD(do_stonesell4)
{
	
	if (1 == quest::CQuestManager::instance().GetEventFlag("stonesell"))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("devredisibirakildi"));
		return;
	}
	
	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	{
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
        return;
	}
	
	int stonesellflood = ch->GetQuestFlag("stonesell_flood");
	if (stonesellflood)
	{
		if (get_global_time() < stonesellflood + 1 /* limit */) 
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cokhizlisin"));
			return;
		}
	}		
	
	int adet0 = ch->CountSpecifyItem(28444);
	int adet = ch->CountSpecifyItem(28443);
	int adet1 = ch->CountSpecifyItem(28442);
	int adet2 = ch->CountSpecifyItem(28441);
	int adet3 = ch->CountSpecifyItem(28440);
	int adet4 = ch->CountSpecifyItem(28439);
	int adet5 = ch->CountSpecifyItem(28438);
	int adet6 = ch->CountSpecifyItem(28437);
	int adet7 = ch->CountSpecifyItem(28436);
	int adet8 = ch->CountSpecifyItem(28435);
	int adet9 = ch->CountSpecifyItem(28434);
	int adet10 = ch->CountSpecifyItem(28433);
	int adet11 = ch->CountSpecifyItem(28432);
	int adet12 = ch->CountSpecifyItem(28431);
	int adet13 = ch->CountSpecifyItem(28430);
	int adet14 = ch->CountSpecifyItem(28445);
	int adet15 = ch->CountSpecifyItem(28446);
	int total = adet0 + adet + adet1 + adet2 + adet3 + adet4 + adet5 + adet6 + adet7 + adet8 + adet9 + adet10 + adet11 + adet12 + adet13 + adet14 + adet15;
	
	if (total < 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("adet_yok"));
		return;
	}
	
	int won = ch->GetCheque();
	int gold = ch->GetGold();
	int tasfiyat = 3000000;
	unsigned long long newMoney = tasfiyat * total;
	unsigned long long score = newMoney + gold;
	
	if (GOLD_MAX <= score || total > 666)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Üzerinde çok fazla para taþýyorsun.");
		return;
	}
	
	ch->RemoveSpecifyItem(28444,adet0);
	ch->RemoveSpecifyItem(28443,adet);
	ch->RemoveSpecifyItem(28442,adet1);
	ch->RemoveSpecifyItem(28441,adet2);
	ch->RemoveSpecifyItem(28440,adet3);
	ch->RemoveSpecifyItem(28439,adet4);
	ch->RemoveSpecifyItem(28438,adet5);
	ch->RemoveSpecifyItem(28437,adet6);
	ch->RemoveSpecifyItem(28436,adet7);
	ch->RemoveSpecifyItem(28435,adet8);
	ch->RemoveSpecifyItem(28434,adet9);
	ch->RemoveSpecifyItem(28433,adet10);
	ch->RemoveSpecifyItem(28432,adet11);
	ch->RemoveSpecifyItem(28431,adet12);
	ch->RemoveSpecifyItem(28430,adet13);
	ch->RemoveSpecifyItem(28445,adet14);
	ch->RemoveSpecifyItem(28446,adet15);
#ifdef YANG_LIMIT
	ch->GoldChange(static_cast<LDWORD>(newMoney), "stonesell");
#else
	ch->PointChange(POINT_GOLD, newMoney, true);
#endif
	
	ch->ChatPacket(CHAT_TYPE_INFO, "Baþarýyla satýþ yaptýn.");
	ch->SetQuestFlag("stonesell_flood", get_global_time() + 5);
	return;		
}

#if defined(ENABLE_REMOTE_SHOP)
DWORD shopvid;
int shopvnum;

struct NPCBul
{
	NPCBul() {};
	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER) ent;
			if (ch->IsNPC())
				shopvid = ch->GetVID();
		}
	}
};

ACMD(do_npcac)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	str_to_number(shopvnum, arg1);

	if(!shopvnum || shopvnum == 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You made a wrong transaction"));
		return;
	}

	if (!ch || !ch->IsPC() || ch->IsObserverMode())
		return;

	if (ch->GetExchange() || ch->GetShopOwner() || ch->GetMyShop() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->GetOfflineShopOwner()
#ifdef WJ_SECURITY_SYSTEM
		|| ch->IsActivateSecurity() == true
#endif
#ifdef __SASH_SYSTEM__
		|| ch->IsSashCombinationOpen() || ch->IsSashAbsorptionOpen()
#endif
#ifdef ENABLE_AURA_SYSTEM
		|| ch->isAuraOpened(true) || ch->isAuraOpened(false)
#endif
	)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You should close the open windows"));
		return;
	}

	if (ch->IsDead())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("in dead condition you can't use the market"));
		return;
	}

	if (ch->GetDungeon())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can not use the remote market in this area"));
		return;
	}

	LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(ch->GetMapIndex());
	if (NULL != pSecMap)
	{
		NPCBul f;
		pSecMap->for_each(f);

		LPCHARACTER yeninpc = CHARACTER_MANAGER::instance().Find(shopvid);
		if (yeninpc)
			CShopManager::instance().NPCAC(ch, yeninpc, shopvnum);
	}

	return;
}
#endif

ACMD(do_biyolog)
{
	// char arg1[256], arg2[256];
	// two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	
	char arg1[256];
	char arg2[256];
	char arg3[256];
	three_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2), arg3, sizeof(arg3));
	

	if (!ch->IsPC())
		return;

	if(ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡Áß(Ã¢°í,±³È¯,»óÁ¡)¿¡´Â °³ÀÎ»óÁ¡À» »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}
	
	if (ch->IsSashCombinationOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("kombinasyon_penceresi_acik"));
		return;
	}
	
	if (ch->IsSashAbsorptionOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bonus_emis_penceresi_acik"));
		return;
	}
	
	/*if (ch->GetBioTime() > get_global_time())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("cokhizlisin"));
		return;
	}*/
	
	ch->SetBioTime(get_global_time()+1);

	int biodurum = ch->GetQuestFlag("bio.durum");
	int bioverilen = ch->GetQuestFlag("bio.verilen");
	int biokalan = ch->GetQuestFlag("bio.kalan");
	int ruhmu = ch->GetQuestFlag("bio.ruhtasi");
	int gerekenitem =  BiyologSistemi[biodurum][0];
	int ruhtasi =  BiyologSistemi[biodurum][4];
	int odul =  BiyologSistemi[biodurum][5];
	int sans =  BiyologSistemi[biodurum][3];
	int bekleme =  (BiyologSistemi[biodurum][2]*60);
	int affectvnum =  BiyologSistemi[biodurum][6];
	int affectvalue =  BiyologSistemi[biodurum][7];
	int affectvnum2 =  BiyologSistemi[biodurum][8];
	int affectvalue2 =  BiyologSistemi[biodurum][9];
	int affectvnum3 =  BiyologSistemi[biodurum][10];
	int affectvalue3 =  BiyologSistemi[biodurum][11];
	int affectvnum4 =  BiyologSistemi[biodurum][12];
	int affectvalue4 =  BiyologSistemi[biodurum][13];
	int affectvnum5 =  BiyologSistemi[biodurum][14];
	int affectvalue5 =  BiyologSistemi[biodurum][15];
	int toplam =  BiyologSistemi[biodurum][1];
	int level =  ch->GetLevel();
	bool ozutmu = false;
	DWORD ozutmod = 0;
	
#ifdef STREAMER_SYSTEM
	if (ch->IsAffectFlag(AFF_VIP))
	{
		if (bekleme != 0)
		{
			bekleme = bekleme-(15*60);
		}
	}
#endif
	
	if (*arg3)
	{
		
		switch (LOWER(arg2[0]))
		{
			case 'o':    // open
				if (isdigit(*arg3))
				{
					str_to_number(ozutmod, arg3);
					ozutmu = true;
				}
				break;
			default:
				ozutmu = false;
		}
	}
	
	
	
	
	if (*arg1 && *arg2 && ozutmu == false)
	{
		if (ch->GetQuestFlag("bio.odulvakti") == 0)
		{
			return;
		}
		
		
		int secim = atoi(arg2);
		int seviye = atoi(arg1);
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
		if (secim == 1)
		{
			ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60*60*24*365*60, 0, false);
		}
		else if (secim == 2)
		{
			ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum2].bPointType, affectvalue2, 0, 60*60*24*365*60, 0, false);
		}
		else if (secim == 3)
		{
			ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum3].bPointType, affectvalue3, 0, 60*60*24*365*60, 0, false);
		}
		if (seviye == 92)
		{
			if (level >= 94)
			{
				ch->SetQuestFlag("bio.durum",10);
				ch->SetQuestFlag("bio.verilen",0);
				ch->SetQuestFlag("bio.kalan",0);
				ch->SetQuestFlag("bio.ruhtasi",0);
				ch->SetQuestFlag("bio.odulvakti",0);
				biodurum = ch->GetQuestFlag("bio.durum");
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyologekrankapa");
				return;
			}
			else
			{
				ch->SetQuestFlag("bio.durum",93);
				ch->SetQuestFlag("bio.verilen",0);
				ch->SetQuestFlag("bio.kalan",0);
				ch->SetQuestFlag("bio.ruhtasi",0);
				ch->SetQuestFlag("bio.odulvakti",0);
				biodurum = ch->GetQuestFlag("bio.durum");
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyologekrankapa");
				return;					
			}
		}
		if (seviye == 94)
		{			
			if (level >= 100)
			{
					ch->SetQuestFlag("bio.durum",11);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.odulvakti",0);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyologekrankapa"); // deneme
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum",100);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.odulvakti",0);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyologekrankapa");
					return;
				}			
			
		}
		return;
	}
	
	

	
	
	if(level < 30)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biolevelyok"));
		return;
	}
	
	if(biodurum == 125)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biyologbitti"));
		return;
	}
	
	if (ch->GetQuestFlag("bio.odulvakti") == 1)
	{
		if (biodurum == 9)
		{
			ch->ChatPacket(CHAT_TYPE_COMMAND, "biyologodul 92 %d %d %d %d %d %d", affectvnum, affectvalue, affectvnum2, affectvalue2, affectvnum3, affectvalue3);
		}
		else if (biodurum == 10)
		{
			ch->ChatPacket(CHAT_TYPE_COMMAND, "biyologodul 94 %d %d %d %d %d %d", affectvnum, affectvalue, affectvnum2, affectvalue2, affectvnum3, affectvalue3);
		}
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioodulsec"));
		return;
	}
	


	if(biokalan > get_global_time())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosuredolmadi"));
		return;
	}
	
	
	if (ozutmu && ozutmod > 0)
	{
		int gecicivnum = 0;
		if (ozutmod == 1)
		{
			if(ch->GetQuestFlag("bio.sans") == 1 )
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosanszatenaktif"));
			}
			else if(ch->GetQuestFlag("bio.durum")  > 20 )
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevinyok"));
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
			}
			else if(ch->GetQuestFlag("bio.ruhtasi")  == 1 )
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhdayapamazsin"));
			}
			else
			{
				ch->RemoveSpecifyItem(71035,1);
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosansverildi"));
				ch->SetQuestFlag("bio.sans",1);			}
		}
		else if (ozutmod == 2)
		{
			if(ch->GetQuestFlag("bio.sure") == 1 )
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosurezatenaktif"));
			}
			else if(ch->GetQuestFlag("bio.durum")  > 20 )
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevinyok"));
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
			}
			else if(ch->GetQuestFlag("bio.ruhtasi")  == 1 )
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhdayapamazsin"));
			}
			else
			{
				ch->RemoveSpecifyItem(96053,1);
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosureverildi"));
				ch->SetQuestFlag("bio.sure",1);
				ch->SetQuestFlag("bio.kalan",0);
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d %d", BiyologSistemi[ch->GetQuestFlag("bio.durum")][0], ch->GetQuestFlag("bio.verilen"), BiyologSistemi[ch->GetQuestFlag("bio.durum")][1], ch->GetQuestFlag("bio.kalan"), BiyologSistemi[ch->GetQuestFlag("bio.durum")][4],BiyologSistemi[ch->GetQuestFlag("bio.durum")][5], ch->GetQuestFlag("bio.ruhtasi"));
			}
		}
		else if (ozutmod == 3)
		{
			if(ch->GetQuestFlag("bio.sure") == 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosurezatenaktif"));
			}
			else if(ch->GetQuestFlag("bio.durum")  > 20)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevinyok"));
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
			}
			else
			{
				ch->RemoveSpecifyItem(96054,1);
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosureverildi"));
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biosansverildi"));
				ch->SetQuestFlag("bio.sure",1);
				ch->SetQuestFlag("bio.sans",1);
				ch->SetQuestFlag("bio.kalan",0);
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d %d", BiyologSistemi[ch->GetQuestFlag("bio.durum")][0], ch->GetQuestFlag("bio.verilen"), BiyologSistemi[ch->GetQuestFlag("bio.durum")][1], ch->GetQuestFlag("bio.kalan"), BiyologSistemi[ch->GetQuestFlag("bio.durum")][4],BiyologSistemi[ch->GetQuestFlag("bio.durum")][5], ch->GetQuestFlag("bio.ruhtasi"));
			}
		}
		
		TItemPos pos_delay = ch->FindSpecifyItemPos(gecicivnum);
		if (pos_delay != NPOS)
		{
			ch->UseItem(pos_delay);
		}
		else 
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutyok"));
		}
		
	}
	
	/////ork diþi
	if(biodurum == 1 and level >= 30)
	{
		
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi,1);
				if (odul != 0)
				{
					ch->AutoGiveItem(odul, 1);
				}
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60*60*24*365*60, 0, false);
				if (level >= 40)
				{
					ch->SetQuestFlag("bio.durum",2);
					ch->SetQuestFlag("bio.30",1);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum",31);
					ch->SetQuestFlag("bio.30",1);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1,100);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					sans = sans +100;
				}
				if(ch->GetQuestFlag("bio.sure") == 1)
				{
					ch->SetQuestFlag("bio.sure",0);
				}
				// ch->ChatPacket(CHAT_TYPE_INFO, "%d %d",sans,prob);
				if(sans >= prob)
				{

					ch->SetQuestFlag("bio.verilen",bioverilen+1);
					if(ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans",0);
					}
				
					bioverilen = ch->GetQuestFlag("bio.verilen");
					if(bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi",1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
						ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem,1);
				return;
			}
		}
	}
	//////lanet kitabý
	if(biodurum == 2 and level >= 40)
	{
		
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi,1);
				if (odul != 0)
				{
					ch->AutoGiveItem(odul, 1);
				}
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60*60*24*365*60, 0, false);
				if (level >= 50)
				{
					ch->SetQuestFlag("bio.durum",3);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.40",1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum",41);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.40",1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;
					
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1,100);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					sans = sans +100;
				}
				if(ch->GetQuestFlag("bio.sure") == 1)
				{
					ch->SetQuestFlag("bio.sure",0);
				}
				
				if(sans >= prob)
				{

					ch->SetQuestFlag("bio.verilen",bioverilen+1);
					if(ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans",0);
					}
				
					bioverilen = ch->GetQuestFlag("bio.verilen");
					if(bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi",1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
						ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem,1);
				return;
			}
		}
	}
	////////þeytan hatýrasý
	if(biodurum == 3 and level >= 50)
	{
		
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi,1);
				if (odul != 0)
				{
					ch->AutoGiveItem(odul, 1);
				}
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60*60*24*365*60, 0, false);
				if (level >= 60)
				{
					ch->SetQuestFlag("bio.durum",4);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.50",1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum",51);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.50",1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;					
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1,100);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					sans = sans +100;
				}
				if(ch->GetQuestFlag("bio.sure") == 1)
				{
					ch->SetQuestFlag("bio.sure",0);
				}
				
				if(sans >= prob)
				{

					ch->SetQuestFlag("bio.verilen",bioverilen+1);
					if(ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans",0);
					}
				
					bioverilen = ch->GetQuestFlag("bio.verilen");
					if(bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi",1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
						ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem,1);
				return;
			}
		}
	}
	////////buz topu
	if(biodurum == 4 and level >= 60)
	{
		
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi,1);
				if (odul != 0)
				{
					ch->AutoGiveItem(odul, 1);
				}
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60*60*24*365*60, 0, false);
				if (level >= 70)
				{
					ch->SetQuestFlag("bio.durum",5);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.60",1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum",61);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.60",1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;					
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1,100);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					sans = sans +100;
				}
				if(ch->GetQuestFlag("bio.sure") == 1)
				{
					ch->SetQuestFlag("bio.sure",0);
				}
				
				if(sans >= prob)
				{

					ch->SetQuestFlag("bio.verilen",bioverilen+1);
					if(ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans",0);
					}
				
					bioverilen = ch->GetQuestFlag("bio.verilen");
					if(bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi",1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
						ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem,1);
				return;
			}
		}
	}
	////////zelkova
	if(biodurum == 5 and level >= 70)
	{
		
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi,1);
				if (odul != 0)
				{
					ch->AutoGiveItem(odul, 1);
				}
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60*60*24*365*60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum2].bPointType, affectvalue2, 0, 60*60*24*365*60, 0, false);
				if (level >= 80)
				{
					ch->SetQuestFlag("bio.durum",6);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.70",1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum",71);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.70",1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1,100);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					sans = sans +100;
				}
				if(ch->GetQuestFlag("bio.sure") == 1)
				{
					ch->SetQuestFlag("bio.sure",0);
				}
				
				if(sans >= prob)
				{

					ch->SetQuestFlag("bio.verilen",bioverilen+1);
					if(ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans",0);
					}
				
					bioverilen = ch->GetQuestFlag("bio.verilen");
					if(bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi",1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
						ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem,1);
				return;
			}
		}
	}
	////////tabela
	if(biodurum == 6 and level >= 80)
	{
		
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi,1);
				if (odul != 0)
				{
					ch->AutoGiveItem(odul, 1);
				}
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60*60*24*365*60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum2].bPointType, affectvalue2, 0, 60*60*24*365*60, 0, false);
				if (level >= 85)
				{
					ch->SetQuestFlag("bio.durum",7);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.80",1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum",81);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.80",1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;					
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1,100);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					sans = sans +100;
				}
				if(ch->GetQuestFlag("bio.sure") == 1)
				{
					ch->SetQuestFlag("bio.sure",0);
				}
				
				if(sans >= prob)
				{

					ch->SetQuestFlag("bio.verilen",bioverilen+1);
					if(ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans",0);
					}
				
					bioverilen = ch->GetQuestFlag("bio.verilen");
					if(bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi",1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
						ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem,1);
				return;
			}
		}
	}
	
	////////kýrýk dal
	if(biodurum == 7 and level >= 85)
	{
		
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi,1);
				if (odul != 0)
				{
					ch->AutoGiveItem(odul, 1);
				}
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60*60*24*365*60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum2].bPointType, affectvalue2, 0, 60*60*24*365*60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum3].bPointType, affectvalue3, 0, 60*60*24*365*60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum4].bPointType, affectvalue4, 0, 60*60*24*365*60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum5].bPointType, affectvalue5, 0, 60*60*24*365*60, 0, false);
				if (level >= 90)
				{
					ch->SetQuestFlag("bio.durum",8);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.85",1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum",86);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.85",1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1,100);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					sans = sans +100;
				}
				if(ch->GetQuestFlag("bio.sure") == 1)
				{
					ch->SetQuestFlag("bio.sure",0);
				}
				
				if(sans >= prob)
				{

					ch->SetQuestFlag("bio.verilen",bioverilen+1);
					if(ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans",0);
					}
				
					bioverilen = ch->GetQuestFlag("bio.verilen");
					if(bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi",1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
						ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem,1);
				return;
			}
		}
	}
	
	////////lider not
	if(biodurum == 8 and level >= 90)
	{
		
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevtamamlandi"));
				ch->RemoveSpecifyItem(ruhtasi,1);
				if (odul != 0)
				{
					ch->AutoGiveItem(odul, 1);
				}
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60*60*24*365*60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum2].bPointType, affectvalue2, 0, 60*60*24*365*60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum3].bPointType, affectvalue3, 0, 60*60*24*365*60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum4].bPointType, affectvalue4, 0, 60*60*24*365*60, 0, false);
				ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum5].bPointType, affectvalue5, 0, 60*60*24*365*60, 0, false);
				if (level >= 92)
				{
					ch->SetQuestFlag("bio.durum",9);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.90",1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					return;
				}
				else
				{
					ch->SetQuestFlag("bio.durum",91);
					ch->SetQuestFlag("bio.verilen",0);
					ch->SetQuestFlag("bio.kalan",0);
					ch->SetQuestFlag("bio.ruhtasi",0);
					ch->SetQuestFlag("bio.90",1);
					biodurum = ch->GetQuestFlag("bio.durum");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
					return;
				}
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1,100);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					sans = sans +100;
				}
				if(ch->GetQuestFlag("bio.sure") == 1)
				{
					ch->SetQuestFlag("bio.sure",0);
				}
				
				if(sans >= prob)
				{

					ch->SetQuestFlag("bio.verilen",bioverilen+1);
					if(ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans",0);
					}
				
					bioverilen = ch->GetQuestFlag("bio.verilen");
					if(bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi",1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
						ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem,1);
				return;
			}
		}
	}
	
	////////kemgöz
	if(biodurum == 9 and level >= 92)
	{
		if (ch->CountSpecifyItem(gerekenitem) < 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
			return;
		}
		else
		{
			int prob = number(1,100);
			if(ch->GetQuestFlag("bio.sans") == 1)
			{
				sans = sans +100;
			}
			if(ch->GetQuestFlag("bio.sure") == 1)
			{
				ch->SetQuestFlag("bio.sure",0);
			}
			
			
			
			if(sans >= prob)
			{
				ch->SetQuestFlag("bio.verilen",bioverilen+1);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
					ch->SetQuestFlag("bio.sans",0);
				}
				
				bioverilen = ch->GetQuestFlag("bio.verilen");
				if(bioverilen == toplam)
				{
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyologodul 92 %d %d %d %d %d %d", affectvnum, affectvalue, affectvnum2, affectvalue2, affectvnum3, affectvalue3);
					ch->SetQuestFlag("bio.odulvakti",1);
					ch->SetQuestFlag("bio.92",1);
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioodulsec"));
					return;
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
				ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
				biokalan = ch->GetQuestFlag("bio.kalan");
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
			}
			ch->RemoveSpecifyItem(gerekenitem,1);
			return;
		}
	}
	
	////////kemgöz
	if(biodurum == 10 and level >= 94)
	{
		
		if (bioverilen >= BiyologSistemi[biodurum][1] && ruhmu == 1)
		{
			if (ch->CountSpecifyItem(ruhtasi) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioruhtasiyok"));
				return;
			}
			else
			{
				ch->RemoveSpecifyItem(ruhtasi,1);
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyologodul 94 %d %d %d %d %d %d", affectvnum, affectvalue, affectvnum2, affectvalue2, affectvnum3, affectvalue3);
				ch->SetQuestFlag("bio.odulvakti",1);
				ch->SetQuestFlag("bio.94",1);
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioodulsec"));
				return;
			}
		}
		else
		{
			if (ch->CountSpecifyItem(gerekenitem) < 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
				return;
			}
			else
			{
				int prob = number(1,100);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					sans = sans +100;
				}
				if(ch->GetQuestFlag("bio.sure") == 1)
				{
					ch->SetQuestFlag("bio.sure",0);
				}
				
				if(sans >= prob)
				{

					ch->SetQuestFlag("bio.verilen",bioverilen+1);
					if(ch->GetQuestFlag("bio.sans") == 1)
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
						ch->SetQuestFlag("bio.sans",0);
					}
				
					bioverilen = ch->GetQuestFlag("bio.verilen");
					if(bioverilen == toplam)
					{
						TItemTable* pTable = ITEM_MANAGER::instance().GetTable(ruhtasi);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biotoplamabittiruhtasibul %s"), pTable->szLocaleName);
						ch->SetQuestFlag("bio.ruhtasi",1);
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 1 0 %d %d 1", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
					}
					else
					{
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
						ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
						biokalan = ch->GetQuestFlag("bio.kalan");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
					}
					
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				ch->RemoveSpecifyItem(gerekenitem,1);
				return;
			}
		}
	}
	////////razador
	if(biodurum == 11 and level >= 100)
	{
		if (ch->CountSpecifyItem(gerekenitem) < 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
			return;
		}
		else
		{
			int prob = number(1,100);
			if(ch->GetQuestFlag("bio.sans") == 1)
			{
				sans = sans +100;
			}
			if(ch->GetQuestFlag("bio.sure") == 1)
			{
				ch->SetQuestFlag("bio.sure",0);
			}
			
			
			
			if(sans >= prob)
			{
				ch->SetQuestFlag("bio.verilen",bioverilen+1);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
					ch->SetQuestFlag("bio.sans",0);
				}
				
				bioverilen = ch->GetQuestFlag("bio.verilen");
				if(bioverilen == toplam)
				{
					ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60*60*24*365*60, 0, false);
					if (level >= 102)
					{
						ch->SetQuestFlag("bio.durum",12);
						ch->SetQuestFlag("bio.verilen",0);
						ch->SetQuestFlag("bio.kalan",0);
						ch->SetQuestFlag("bio.ruhtasi",0);
						ch->SetQuestFlag("bio.100",1);
						biodurum = ch->GetQuestFlag("bio.durum");
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
						return;
					}
					else
					{
						ch->SetQuestFlag("bio.durum",102);
						ch->SetQuestFlag("bio.verilen",0);
						ch->SetQuestFlag("bio.kalan",0);
						ch->SetQuestFlag("bio.ruhtasi",0);
						ch->SetQuestFlag("bio.100",1);
						biodurum = ch->GetQuestFlag("bio.durum");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
						return;
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
				ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
				biokalan = ch->GetQuestFlag("bio.kalan");
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
			}
			ch->RemoveSpecifyItem(gerekenitem,1);
			return;
		}
	}
	////////nemere
	if(biodurum == 12 and level >= 102)
	{
		if (ch->CountSpecifyItem(gerekenitem) < 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
			return;
		}
		else
		{
			int prob = number(1,100);
			if(ch->GetQuestFlag("bio.sans") == 1)
			{
				sans = sans +100;
			}
			if(ch->GetQuestFlag("bio.sure") == 1)
			{
				ch->SetQuestFlag("bio.sure",0);
			}
			
			
			
			if(sans >= prob)
			{
				ch->SetQuestFlag("bio.verilen",bioverilen+1);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
					ch->SetQuestFlag("bio.sans",0);
				}
				
				bioverilen = ch->GetQuestFlag("bio.verilen");
				if(bioverilen == toplam)
				{
					ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60*60*24*365*60, 0, false);
					if (level >= 105)
					{
						ch->SetQuestFlag("bio.durum",13);
						ch->SetQuestFlag("bio.verilen",0);
						ch->SetQuestFlag("bio.kalan",0);
						ch->SetQuestFlag("bio.ruhtasi",0);
						ch->SetQuestFlag("bio.102",1);
						biodurum = ch->GetQuestFlag("bio.durum");
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
						return;
					}
					else
					{
						ch->SetQuestFlag("bio.durum",105);
						ch->SetQuestFlag("bio.verilen",0);
						ch->SetQuestFlag("bio.kalan",0);
						ch->SetQuestFlag("bio.ruhtasi",0);
						ch->SetQuestFlag("bio.102",1);
						biodurum = ch->GetQuestFlag("bio.durum");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
						return;
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
				ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
				biokalan = ch->GetQuestFlag("bio.kalan");
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
			}
			ch->RemoveSpecifyItem(gerekenitem,1);
			return;
		}
	}
	
	////////meley
	if(biodurum == 13 and level >= 105)
	{
		if (ch->CountSpecifyItem(gerekenitem) < 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
			return;
		}
		else
		{
			int prob = number(1,100);
			if(ch->GetQuestFlag("bio.sans") == 1)
			{
				sans = sans +100;
			}
			if(ch->GetQuestFlag("bio.sure") == 1)
			{
				ch->SetQuestFlag("bio.sure",0);
			}
			
			
			
			if(sans >= prob)
			{
				ch->SetQuestFlag("bio.verilen",bioverilen+1);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
					ch->SetQuestFlag("bio.sans",0);
				}
				
				bioverilen = ch->GetQuestFlag("bio.verilen");
				if(bioverilen == toplam)
				{
					ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60*60*24*365*60, 0, false);
					if (level >= 108)
					{
						ch->SetQuestFlag("bio.durum",14);
						ch->SetQuestFlag("bio.verilen",0);
						ch->SetQuestFlag("bio.kalan",0);
						ch->SetQuestFlag("bio.ruhtasi",0);
						ch->SetQuestFlag("bio.105",1);
						biodurum = ch->GetQuestFlag("bio.durum");
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
						return;
					}
					else
					{
						ch->SetQuestFlag("bio.durum",108);
						ch->SetQuestFlag("bio.verilen",0);
						ch->SetQuestFlag("bio.kalan",0);
						ch->SetQuestFlag("bio.ruhtasi",0);
						ch->SetQuestFlag("bio.105",1);
						biodurum = ch->GetQuestFlag("bio.durum");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
						return;
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
				ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
				biokalan = ch->GetQuestFlag("bio.kalan");
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
			}
			ch->RemoveSpecifyItem(gerekenitem,1);
			return;
		}
	}
	
	////////hydra
	if(biodurum == 14 and level >= 108)
	{
		if (ch->CountSpecifyItem(gerekenitem) < 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
			return;
		}
		else
		{
			int prob = number(1,100);
			if(ch->GetQuestFlag("bio.sans") == 1)
			{
				sans = sans +100;
			}
			if(ch->GetQuestFlag("bio.sure") == 1)
			{
				ch->SetQuestFlag("bio.sure",0);
			}
			
			
			
			if(sans >= prob)
			{
				ch->SetQuestFlag("bio.verilen",bioverilen+1);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
					ch->SetQuestFlag("bio.sans",0);
				}
				
				bioverilen = ch->GetQuestFlag("bio.verilen");
				if(bioverilen == toplam)
				{
					ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60*60*24*365*60, 0, false);
					if (level >= 110)
					{
						ch->SetQuestFlag("bio.durum",15);
						ch->SetQuestFlag("bio.verilen",0);
						ch->SetQuestFlag("bio.kalan",0);
						ch->SetQuestFlag("bio.ruhtasi",0);
						ch->SetQuestFlag("bio.108",1);
						biodurum = ch->GetQuestFlag("bio.durum");
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
						return;
					}
					else
					{
						ch->SetQuestFlag("bio.durum",110);
						ch->SetQuestFlag("bio.verilen",0);
						ch->SetQuestFlag("bio.kalan",0);
						ch->SetQuestFlag("bio.ruhtasi",0);
						ch->SetQuestFlag("bio.108",1);
						biodurum = ch->GetQuestFlag("bio.durum");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
						return;
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
				ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
				biokalan = ch->GetQuestFlag("bio.kalan");
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
			}
			ch->RemoveSpecifyItem(gerekenitem,1);
			return;
		}
	}
	
	if(biodurum == 15 and level >= 110)
	{
		if (ch->CountSpecifyItem(gerekenitem) < 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
			return;
		}
		else
		{
			int prob = number(1,100);
			if(ch->GetQuestFlag("bio.sans") == 1)
			{
				sans = sans +100;
			}
			if(ch->GetQuestFlag("bio.sure") == 1)
			{
				ch->SetQuestFlag("bio.sure",0);
			}
			
			
			
			if(sans >= prob)
			{
				ch->SetQuestFlag("bio.verilen",bioverilen+1);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
					ch->SetQuestFlag("bio.sans",0);
				}
				
				bioverilen = ch->GetQuestFlag("bio.verilen");
				if(bioverilen == toplam)
				{
					ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60*60*24*365*60, 0, false);
					if (level >= 120)
					{
						ch->SetQuestFlag("bio.durum",16);
						ch->SetQuestFlag("bio.verilen",0);
						ch->SetQuestFlag("bio.kalan",0);
						ch->SetQuestFlag("bio.ruhtasi",0);
						ch->SetQuestFlag("bio.110",1);
						biodurum = ch->GetQuestFlag("bio.durum");
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
						return;
					}
					else
					{
						ch->SetQuestFlag("bio.durum",120);
						ch->SetQuestFlag("bio.verilen",0);
						ch->SetQuestFlag("bio.kalan",0);
						ch->SetQuestFlag("bio.ruhtasi",0);
						ch->SetQuestFlag("bio.110",1);
						biodurum = ch->GetQuestFlag("bio.durum");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
						return;
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
				ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
				biokalan = ch->GetQuestFlag("bio.kalan");
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
			}
			ch->RemoveSpecifyItem(gerekenitem,1);
			return;
		}
	}
	
	if(biodurum == 16 and level >= 120)
	{
		if (ch->CountSpecifyItem(gerekenitem) < 1)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioitemyok"));
			return;
		}
		else
		{
			int prob = number(1,100);
			if(ch->GetQuestFlag("bio.sans") == 1)
			{
				sans = sans +100;
			}
			if(ch->GetQuestFlag("bio.sure") == 1)
			{
				ch->SetQuestFlag("bio.sure",0);
			}
			
			
			
			if(sans >= prob)
			{
				ch->SetQuestFlag("bio.verilen",bioverilen+1);
				if(ch->GetQuestFlag("bio.sans") == 1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioozutgitti"));
					ch->SetQuestFlag("bio.sans",0);
				}
				
				bioverilen = ch->GetQuestFlag("bio.verilen");
				if(bioverilen == toplam)
				{
					ch->AddAffect(AFFECT_COLLECT, aApplyInfo[affectvnum].bPointType, affectvalue, 0, 60*60*24*365*60, 0, false);
					if (level >= 125)
					{
						ch->SetQuestFlag("bio.durum",16);
						ch->SetQuestFlag("bio.verilen",0);
						ch->SetQuestFlag("bio.kalan",0);
						ch->SetQuestFlag("bio.ruhtasi",0);
						ch->SetQuestFlag("bio.110",1);
						biodurum = ch->GetQuestFlag("bio.durum");
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bioyenigorev"));
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d 0 %d 0 %d %d 0", BiyologSistemi[biodurum][0], BiyologSistemi[biodurum][1], BiyologSistemi[biodurum][4],BiyologSistemi[biodurum][5]);
						return;
					}
					else
					{
						ch->SetQuestFlag("bio.durum",125);
						ch->SetQuestFlag("bio.verilen",0);
						ch->SetQuestFlag("bio.kalan",0);
						ch->SetQuestFlag("bio.ruhtasi",0);
						ch->SetQuestFlag("bio.110",1);
						ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogorevlerbitti"));
						biodurum = ch->GetQuestFlag("bio.durum");
						ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog 0 0 0 0 0 0 0");
						return;
					}
				}
				else
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biogecti %d"), (toplam-bioverilen));
					ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
					biokalan = ch->GetQuestFlag("bio.kalan");
					ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
				}
				
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("biobasarisiz"));
				ch->SetQuestFlag("bio.kalan",get_global_time()+bekleme);
				biokalan = ch->GetQuestFlag("bio.kalan");
				ch->ChatPacket(CHAT_TYPE_COMMAND, "biyolog %d %d %d %d %d %d 0", BiyologSistemi[biodurum][0], bioverilen, BiyologSistemi[biodurum][1], biokalan, BiyologSistemi[biodurum][4], BiyologSistemi[biodurum][5]);
			}
			ch->RemoveSpecifyItem(gerekenitem,1);
			return;
		}
	}
	return;
}
#ifdef WJ_SECURITY_SYSTEM
ACMD(do_create_security)
{
	char arg1[256];
	char arg2[255];
	int durum;
	two_arguments (argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	if (!*arg1)
		return;
	
	if (!*arg2)
		return;
	
	str_to_number(durum,arg2);
	
	int iFloodResult = ch->GetQuestFlag("input_security.last_input");
	if (iFloodResult){
		if (get_global_time() < iFloodResult + 1 /* limit */) {
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("HIZLI_YAPAMAZSIN"));
			return;
		}
	}	

	
	const char * sifrem = ch->GetBindPassword();
	
	
	if (!sifrem)
	{
		
		
		
		if (strlen(arg1) != 6)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("sifre6hanedegil"));
			return;
		}
		
		if (!is_digits(arg1))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("sadecesayigir"));
			return;
		}
		
		
		
		
		char szInsertQuery[QUERY_MAX_LEN];
		snprintf(szInsertQuery, sizeof(szInsertQuery), "INSERT INTO player.safebox (account_id, size, password, gold) VALUES (%d, 0, '%s', 0)",ch->GetDesc()->GetAccountTable().id, arg1);
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szInsertQuery));
			
		ch->DeactivateSecurity();
		ch->PrepareSecurityGui(false);
		ch->UpdatePacket();
		ch->SetQuestFlag("input_security.stat",durum);
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("sifreyaptim"));
		ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseSecurityCreate");

		// ch->ChatPacket(CHAT_TYPE_INFO, "Hata Kod 2");
		return;
	}
	
	/* if (sifrem)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Hata Kod 1");
		return;
	} */
	
	
	if (strlen(arg1) != 6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("sifre6hanedegil"));
		return;
	}
	
	if (!is_digits(arg1))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("sadecesayigir"));
		return;
	}
	
	if (strcmp(sifrem, arg1) == 0)
	{
		ch->DeactivateSecurity();
		ch->PrepareSecurityGui(false);
		ch->UpdatePacket();
		ch->SetQuestFlag("input_security.stat",durum);
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("sifreyaptim"));
		ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseSecurityCreate");
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO,LC_TEXT("hatali_sifre"));
	}

}

ACMD(do_input_security)
{
	char arg1[256];
	char arg2[255];
	int durum;
	two_arguments (argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	if (!*arg1)
		return;
	
	if (!*arg2)
		return;
	
	str_to_number(durum,arg2);
	
	int iFloodResult = ch->GetQuestFlag("input_security.last_input");
	if (iFloodResult){
		if (get_global_time() < iFloodResult + 1 /* limit */) {
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("HIZLI_YAPAMAZSIN"));
			return;
		}
	}	

	if (strlen(arg1) != 6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("sifre6hanedegil"));
		return;
	}
	
	if (!is_digits(arg1))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("sadecesayigir"));
		return;
	}
	
	const char * sifrem = ch->GetBindPassword();
	
	if (!sifrem) {
		ch->ChatPacket(CHAT_TYPE_INFO, "HATA");
		return;
	}
	
	if (strcmp(sifrem, arg1) == 0)
	{
		ch->DeactivateSecurity();
		ch->PrepareSecurityGui(false);
		ch->UpdatePacket();
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("kilitdevredisi"));
		ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseSecurityDialog");
		ch->SetQuestFlag("input_security.last_input", get_global_time() + 2);
		ch->SetQuestFlag("input_security.stat",durum);
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("hatali_sifre"));
		ch->SetQuestFlag("input_security.last_input", get_global_time() + 2);
	}
}

ACMD(do_change_security)
{
	char arg1[256];
	char arg2[256];
	char arg3[256];
	three_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2), arg3, sizeof(arg3));
	
	int iFloodResult = ch->GetQuestFlag("input_security.last_input");
	if (iFloodResult){
		if (get_global_time() < iFloodResult + 1 /* limit */) {
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("HIZLI_YAPAMAZSIN"));
			return;
		}
	}	

	if (strlen(arg1) < 4 || strlen(arg1) > 6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Security key has only 4 - 6 character numbers"));
		return;
	}

	if (strlen(arg2) < 4 || strlen(arg2) > 6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Security key has only 4 - 6 character numbers"));
		return;
	}
	
	if (strlen(arg3) < 4 || strlen(arg3) > 6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Security key has only 4 - 6 character numbers"));
		return;
	}
	
	if (!is_digits(arg1))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Security key has only number"));
		return;
	}
	
	if (!is_digits(arg2))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Security key has only number"));
		return;
	}
	
	if (!is_digits(arg3))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Security key has only number"));
		return;
	}
	
	const char * sifrem = ch->GetBindPassword();
	
	if (sifrem && strcmp(sifrem, arg1) == 0)
	{
		if (strcmp(arg2, arg3) == 0)
		{
			ch->SetSecurityPassword(arg2);
			ch->UpdatePacket();
			//ch->UpdateSecurityPacket();
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Security key has been successfully changed"));
			//ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseSecurityChange");
			ch->SetQuestFlag("input_security.last_input", get_global_time() + 2);
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("First New Password And Second Password Not Match"));
			ch->SetQuestFlag("input_security.last_input", get_global_time() + 2);
		}
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Old security key not match"));
		ch->SetQuestFlag("input_security.last_input", get_global_time() + 2);
	}
}

ACMD(do_open_security)
{
	char arg1[256];
	int gelen;
	one_argument(argument, arg1, sizeof(arg1));
	
	const char * sifrem = ch->GetBindPassword();

	if (*arg1)
	{
		str_to_number(gelen,arg1);
		if (gelen == 1)
		{
			ch->ActivateSecurity();
			ch->PrepareSecurityGui(true);
			if (sifrem)
				ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenSecurityDialog2 %d", ch->GetQuestFlag("input_security.stat"));
			else
				ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenSecurityCreate");
		}
		else
		{
			
			if(ch->GetQuestFlag("input_security.stat") == 0)
			{
				ch->ActivateSecurity();
				ch->PrepareSecurityGui(true);
				if (sifrem)
					ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenSecurityDialog");
				else
					ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenSecurityCreate");
			}
			else
			{
				return;
			}
		}
	}
	else
	{
		if(ch->GetQuestFlag("input_security.stat") == 0)
		{
			ch->ActivateSecurity();
			ch->PrepareSecurityGui(true);
			if (sifrem)
				ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenSecurityDialog");
			else
				ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenSecurityCreate");
		}
		else
		{
			return;
		}
	}
	
	
	
}
#endif

LPEVENT ruhtimer = NULL;

EVENTINFO(TMainEventInfo)
{
	DWORD dwPID;
	long skillindexx;

	TMainEventInfo() : dwPID(0), skillindexx(0) {}
};

EVENTFUNC(ruh_event)
{
	TMainEventInfo * info = dynamic_cast<TMainEventInfo * >(event->info);

	if (info == NULL)
	{
		return 0;
	}

	DWORD dwPID = info->dwPID;
	long skillindex = info->skillindexx;

	if(skillindex == 0 || dwPID == 0)
		return 0;

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindByPID(dwPID);
	if (ch == NULL){
		sys_err("ch WAS null > pid: %u", dwPID);
		return 0;
	}

	// if (!ch || NULL == ch || skillindex == 0)
		// return 0;

	// if (!ch->GetDesc())
		// return 0;

	if(ch->CountSpecifyItem(50513) < 1)
		return 0;

	int skilllevel = ch->GetSkillLevel(skillindex);
	if (skilllevel >= 40)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhskillson"));
		return 0;
	}

	int gerekenderece = (1000+500*(skilllevel-30));
	int derecem = (ch->GetRealAlignment()/10);
	int sonuc = (-29000+gerekenderece);
	if (derecem < 0)
	{
		gerekenderece = gerekenderece*2;
		sonuc = (-29000-gerekenderece);
	}

	if (derecem > sonuc)
	{
		int gerekliknk = gerekenderece;
		int kactane = gerekliknk/500;
		if (kactane < 0)
			kactane = 0 - kactane;

		if (derecem < 0)
		{
			if (ch->CountSpecifyItem(70102) < kactane)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhzenbitti %d"),kactane);
				return 0;
			}

			int delta = MIN(-(ch->GetAlignment()), 500);
			ch->UpdateAlignment(delta*kactane);
			ch->RemoveSpecifyItem(70102,kactane);
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhzenbastim"));
		}
	}

	if(ch->GetQuestFlag("ruh.sure") > get_global_time())
	{
		if (ch->CountSpecifyItem(71001) < 1 )
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhsuredolmadi"));
			return 0;
		}
		else
			ch->RemoveSpecifyItem(71001,1);
	}

	if (ch->CountSpecifyItem(71094) >= 1)
	{
		ch->AddAffect(512, aApplyInfo[0].bPointType, 0, 0, 536870911, 0, false);
		ch->RemoveSpecifyItem(71094,1);
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhmunzevikullandim"));
	}

	if (gerekenderece < 0)
		ch->UpdateAlignment(gerekenderece*10);
	else
		ch->UpdateAlignment(-gerekenderece*10);

	ch->LearnGrandMasterSkill(skillindex);
	ch->RemoveSpecifyItem(50513,1);
	ch->SetQuestFlag("ruh.sure",get_global_time()+60*60*24);

	return 1;
}

ACMD(do_ruhoku)
{
	int gelen;
	long skillindex;
	char arg1[256], arg2[256];

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	if (!*arg1 || !*arg2)
		return;

	str_to_number(gelen, arg1);
	str_to_number(skillindex, arg2);

	if (gelen < 0 || skillindex < 0)
		return;

	if (!ch || !ch->IsPC())
		return;

	if (ch->IsDead() || ch->IsStun())
		return;

	if (ch->IsHack())
		return;

	if (ch->IsOpenSafebox() || ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsCubeOpen()
#ifdef WJ_SECURITY_SYSTEM
		|| ch->IsActivateSecurity() == true
#endif
#ifdef __SASH_SYSTEM__
		|| ch->IsSashCombinationOpen() || ch->IsSashAbsorptionOpen()
#endif
#ifdef ENABLE_AURA_SYSTEM
		|| ch->isAuraOpened(true) || ch->isAuraOpened(false)
#endif
	)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
		return;
	}

	if(ch->CountSpecifyItem(50513) < 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhtasiyok"));
		return;
	}

	LPITEM slot1 = ch->GetWear(WEAR_UNIQUE1);
	LPITEM slot2 = ch->GetWear(WEAR_UNIQUE2);

	if (NULL != slot1)
	{
		if (slot1->GetVnum() == 70048)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pelerin_cikar"));
			return;
		}
	}

	if (NULL != slot2)
	{
		if (slot2->GetVnum() == 70048)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pelerin_cikar"));
			return;
		}
	}

	int skillgrup = ch->GetSkillGroup();
	if (skillgrup == 0)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhokuyamazsin"));
		return;
	}

	if (gelen == 1) ///tek
	{
		int skilllevel = ch->GetSkillLevel(skillindex);
		int gerekenderece = (1000+500*(skilllevel-30));
		int derecem = (ch->GetRealAlignment()/10);
		int sonuc = (-29000+gerekenderece);
		
		if (ch->GetQuestFlag("ruh.yenisure") > get_global_time())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruh1sn"));
			return;
		}

		if (derecem < 0)
		{
			gerekenderece = gerekenderece*2;
			sonuc = (-29000-gerekenderece);
		}

		if (derecem > sonuc)
		{
			int gerekliknk = gerekenderece;
			int kactane = gerekliknk/500;
			if (kactane < 0)
				kactane = 0 - kactane;

			if (derecem < 0)
			{
				if (ch->CountSpecifyItem(70102) < kactane)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhzenbitti %d"),kactane);
					return;
				}

				int delta = MIN(-(ch->GetAlignment()), 500);
				ch->UpdateAlignment(delta*kactane);
				ch->RemoveSpecifyItem(70102,kactane);
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhzenbastim"));
			}
		}

		if(ch->GetQuestFlag("ruh.sure") > get_global_time())
		{
			if (ch->CountSpecifyItem(71001) < 1 )
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhsuredolmadi"));
				return;
			}
			else
				ch->RemoveSpecifyItem(71001,1);
		}

		if (ch->CountSpecifyItem(71094) >= 1)
		{
			ch->AddAffect(512, aApplyInfo[0].bPointType, 0, 0, 536870911, 0, false);
			ch->RemoveSpecifyItem(71094,1);
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ruhmunzevikullandim"));
		}

		if (gerekenderece < 0)
			ch->UpdateAlignment(gerekenderece*10);
		else
			ch->UpdateAlignment(-gerekenderece*10);

		ch->LearnGrandMasterSkill(skillindex);
		ch->RemoveSpecifyItem(50513,1);
		ch->SetQuestFlag("ruh.sure",get_global_time()+60*60*24);
		ch->SetQuestFlag("ruh.yenisure",get_global_time()+1);
	}
	else if(gelen == 0) // hepsi
	{
		if (ruhtimer)
			event_cancel(&ruhtimer);

		TMainEventInfo* info = AllocEventInfo<TMainEventInfo>();
		info->dwPID = ch->GetPlayerID();
		info->skillindexx = skillindex;
		ruhtimer = event_create(ruh_event, info, PASSES_PER_SEC(1));
	}
	return;
}

ACMD(do_user_horse_ride)
{
	if (ch->IsObserverMode())
		return;

	if (ch->IsDead() || ch->IsStun())
		return;

	if (ch->IsHorseRiding() == false)
	{
		if (ch->GetMountVnum())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì Å»°ÍÀ» ÀÌ¿ëÁßÀÔ´Ï´Ù."));
			return;
		}

		if (ch->GetHorse() == NULL)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» ¸ÕÀú ¼ÒÈ¯ÇØÁÖ¼¼¿ä."));
			return;
		}

		ch->StartRiding();
	}
	else
	{
		ch->StopRiding();
	}
}

ACMD(do_dungeontp)
{
	char arg1[256];
	int gelen;
	
	int x;
	int y;
	
	one_argument (argument, arg1, sizeof(arg1));
	
	str_to_number(gelen, arg1);

	if (!ch->IsPC())
		return;

	if(ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ °Å·¡Áß(Ã¢°í,±³È¯,»óÁ¡)¿¡´Â °³ÀÎ»óÁ¡À» »ç¿ëÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}
	
	str_to_number(x, Takip[gelen][0]);
	str_to_number(y, Takip[gelen][1]);
	
	
	ch->WarpSet(x, y);
}
ACMD(do_user_horse_back)
{
	if (ch->GetHorse() != NULL)
	{
		
		LPITEM binekitem = ch->GetWear(WEAR_COSTUME_MOUNT);
		if (NULL != binekitem)
		{
			if (ch->GetEmptyInventory(binekitem->GetSize()) == -1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("envanter_dolu"));
				return;
			}
			ch->UnequipItem(binekitem);
		}
		ch->HorseSummon(false);
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» µ¹·Áº¸³Â½À´Ï´Ù."));
	}
	else if (ch->IsHorseRiding() == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»¿¡¼­ ¸ÕÀú ³»·Á¾ß ÇÕ´Ï´Ù."));
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» ¸ÕÀú ¼ÒÈ¯ÇØÁÖ¼¼¿ä."));
	}
}

ACMD(do_user_horse_feed)
{
	// °³ÀÎ»óÁ¡À» ¿¬ »óÅÂ¿¡¼­´Â ¸» ¸ÔÀÌ¸¦ ÁÙ ¼ö ¾ø´Ù.
	if (ch->GetMyShop())
		return;

	if (ch->GetHorse() == NULL)
	{
		if (ch->IsHorseRiding() == false)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» ¸ÕÀú ¼ÒÈ¯ÇØÁÖ¼¼¿ä."));
		else
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»À» Åº »óÅÂ¿¡¼­´Â ¸ÔÀÌ¸¦ ÁÙ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	DWORD dwFood = ch->GetHorseGrade() + 50054 - 1;

	if (ch->CountSpecifyItem(dwFood) > 0)
	{
		ch->RemoveSpecifyItem(dwFood, 1);
		ch->FeedHorse();
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¸»¿¡°Ô %s%s ÁÖ¾ú½À´Ï´Ù."), 
				ITEM_MANAGER::instance().GetTable(dwFood)->szLocaleName,
				g_iUseLocale ? "" : under_han(ITEM_MANAGER::instance().GetTable(dwFood)->szLocaleName) ? LC_TEXT("À»") : LC_TEXT("¸¦"));
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ¾ÆÀÌÅÛÀÌ ÇÊ¿äÇÕ´Ï´Ù"), ITEM_MANAGER::instance().GetTable(dwFood)->szLocaleName);
	}
}

#define MAX_REASON_LEN		128

EVENTINFO(TimedEventInfo)
{
	DynamicCharacterPtr ch;
	int		subcmd;
	int         	left_second;
	char		szReason[MAX_REASON_LEN];

	TimedEventInfo() 
	: ch()
	, subcmd( 0 )
	, left_second( 0 )
	{
		::memset( szReason, 0, MAX_REASON_LEN );
	}
};

struct SendDisconnectFunc
{
	void operator () (LPDESC d)
	{
		if (d->GetCharacter())
		{
			if (d->GetCharacter()->GetGMLevel() == GM_PLAYER)
				d->GetCharacter()->ChatPacket(CHAT_TYPE_COMMAND, "quit Shutdown(SendDisconnectFunc)");
		}
	}
};

struct DisconnectFunc
{
	void operator () (LPDESC d)
	{
		if (d->GetType() == DESC_TYPE_CONNECTOR)
			return;

		if (d->IsPhase(PHASE_P2P))
			return;

		if (d->GetCharacter())
			d->GetCharacter()->Disconnect("Shutdown(DisconnectFunc)");

		d->SetPhase(PHASE_CLOSE);
	}
};

EVENTINFO(shutdown_event_data)
{
	int seconds;

	shutdown_event_data()
	: seconds( 0 )
	{
	}
};

EVENTFUNC(shutdown_event)
{
	shutdown_event_data* info = dynamic_cast<shutdown_event_data*>( event->info );

	if ( info == NULL )
	{
		sys_err( "shutdown_event> <Factor> Null pointer" );
		return 0;
	}

	int * pSec = & (info->seconds);

	if (*pSec < 0)
	{
		sys_log(0, "shutdown_event sec %d", *pSec);

		if (--*pSec == -10)
		{
			const DESC_MANAGER::DESC_SET & c_set_desc = DESC_MANAGER::instance().GetClientSet();
			std::for_each(c_set_desc.begin(), c_set_desc.end(), DisconnectFunc());
			return passes_per_sec;
		}
		else if (*pSec < -10)
			return 0;

		return passes_per_sec;
	}
	else if (*pSec == 0)
	{
		const DESC_MANAGER::DESC_SET & c_set_desc = DESC_MANAGER::instance().GetClientSet();
		std::for_each(c_set_desc.begin(), c_set_desc.end(), SendDisconnectFunc());
		g_bNoMoreClient = true;
		--*pSec;
		return passes_per_sec;
	}
	else
	{
		char buf[64];
		snprintf(buf, sizeof(buf), LC_TEXT("¼Ë´Ù¿îÀÌ %dÃÊ ³²¾Ò½À´Ï´Ù."), *pSec);
		SendNotice(buf);

		--*pSec;
		return passes_per_sec;
	}
}

void Shutdown(int iSec)
{
	if (g_bNoMoreClient)
	{
		thecore_shutdown();
		return;
	}

	CWarMapManager::instance().OnShutdown();

	char buf[64];
	snprintf(buf, sizeof(buf), LC_TEXT("%dÃÊ ÈÄ °ÔÀÓÀÌ ¼Ë´Ù¿î µË´Ï´Ù."), iSec);

	SendNotice(buf);

	shutdown_event_data* info = AllocEventInfo<shutdown_event_data>();
	info->seconds = iSec;

	event_create(shutdown_event, info, 1);
}

ACMD(do_shutdown)
{
	if (NULL == ch)
	{
		sys_err("Accept shutdown command from %s.", ch->GetName());
	}
	TPacketGGShutdown p;
	p.bHeader = HEADER_GG_SHUTDOWN;
	P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGShutdown));

	Shutdown(10);
}

EVENTFUNC(timed_event)
{
	TimedEventInfo * info = dynamic_cast<TimedEventInfo *>( event->info );
	
	if ( info == NULL )
	{
		sys_err( "timed_event> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER	ch = info->ch;
	if (ch == NULL) { // <Factor>
		return 0;
	}
	LPDESC d = ch->GetDesc();

	if (info->left_second <= 0)
	{
		ch->m_pkTimedEvent = NULL;

		if (true == LC_IsEurope() || true == LC_IsYMIR() || true == LC_IsKorea())
		{
			switch (info->subcmd)
			{
				case SCMD_LOGOUT:
				case SCMD_QUIT:
				case SCMD_PHASE_SELECT:
					{
						TPacketNeedLoginLogInfo acc_info;
						acc_info.dwPlayerID = ch->GetDesc()->GetAccountTable().id;

						db_clientdesc->DBPacket( HEADER_GD_VALID_LOGOUT, 0, &acc_info, sizeof(acc_info) );

						LogManager::instance().DetailLoginLog( false, ch );
					}
					break;
			}
		}

		switch (info->subcmd)
		{
			case SCMD_LOGOUT:
				if (d)
					d->SetPhase(PHASE_CLOSE);
				break;

			case SCMD_QUIT:
				ch->ChatPacket(CHAT_TYPE_COMMAND, "quit");
				break;

			case SCMD_PHASE_SELECT:
				{
					ch->Disconnect("timed_event - SCMD_PHASE_SELECT");

					if (d)
					{
						d->SetPhase(PHASE_SELECT);
					}
				}
				break;
		}

		return 0;
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%dÃÊ ³²¾Ò½À´Ï´Ù."), info->left_second);
		--info->left_second;
	}

	return PASSES_PER_SEC(1);
}

ACMD(do_cmd)
{
	/* RECALL_DELAY
	   if (ch->m_pkRecallEvent != NULL)
	   {
	   ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ãë¼Ò µÇ¾ú½À´Ï´Ù."));
	   event_cancel(&ch->m_pkRecallEvent);
	   return;
	   }
	// END_OF_RECALL_DELAY */

	if (ch->m_pkTimedEvent)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ãë¼Ò µÇ¾ú½À´Ï´Ù."));
		event_cancel(&ch->m_pkTimedEvent);
		return;
	}
	
	if (ch->m_pkTimedEventCh)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Hizli ch durduruldu."));
		event_cancel(&ch->m_pkTimedEventCh);
	}

	switch (subcmd)
	{
		case SCMD_LOGOUT:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("·Î±×ÀÎ È­¸éÀ¸·Î µ¹¾Æ °©´Ï´Ù. Àá½Ã¸¸ ±â´Ù¸®¼¼¿ä."));
			break;

		case SCMD_QUIT:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°ÔÀÓÀ» Á¾·á ÇÕ´Ï´Ù. Àá½Ã¸¸ ±â´Ù¸®¼¼¿ä."));
			break;

		case SCMD_PHASE_SELECT:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Ä³¸¯ÅÍ¸¦ ÀüÈ¯ ÇÕ´Ï´Ù. Àá½Ã¸¸ ±â´Ù¸®¼¼¿ä."));
			break;
	}

	int nExitLimitTime = 10;

	if (ch->IsHack(false, true, nExitLimitTime) &&
		false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()) &&
	   	(!ch->GetWarMap() || ch->GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG))
	{
		return;
	}
	
	switch (subcmd)
	{
		case SCMD_LOGOUT:
		case SCMD_QUIT:
		case SCMD_PHASE_SELECT:
			{
				TimedEventInfo* info = AllocEventInfo<TimedEventInfo>();

				{
					if (ch->IsPosition(POS_FIGHTING))
						info->left_second = 10;
					else
						info->left_second = 3;
				}

				info->ch		= ch;
				info->subcmd		= subcmd;
				strlcpy(info->szReason, argument, sizeof(info->szReason));

				ch->m_pkTimedEvent	= event_create(timed_event, info, 1);
			}
			break;
	}
}

ACMD(do_mount)
{
	/*
	   char			arg1[256];
	   struct action_mount_param	param;

	// ÀÌ¹Ì Å¸°í ÀÖÀ¸¸é
	if (ch->GetMountingChr())
	{
	char arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || !*arg2)
	return;

	param.x		= atoi(arg1);
	param.y		= atoi(arg2);
	param.vid	= ch->GetMountingChr()->GetVID();
	param.is_unmount = true;

	float distance = DISTANCE_SQRT(param.x - (DWORD) ch->GetX(), param.y - (DWORD) ch->GetY());

	if (distance > 600.0f)
	{
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á» ´õ °¡±îÀÌ °¡¼­ ³»¸®¼¼¿ä."));
	return;
	}

	action_enqueue(ch, ACTION_TYPE_MOUNT, &param, 0.0f, true);
	return;
	}

	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	return;

	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(atoi(arg1));

	if (!tch->IsNPC() || !tch->IsMountable())
	{
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°Å±â¿¡´Â Å» ¼ö ¾ø¾î¿ä."));
	return;
	}

	float distance = DISTANCE_SQRT(tch->GetX() - ch->GetX(), tch->GetY() - ch->GetY());

	if (distance > 600.0f)
	{
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á» ´õ °¡±îÀÌ °¡¼­ Å¸¼¼¿ä."));
	return;
	}

	param.vid		= tch->GetVID();
	param.is_unmount	= false;

	action_enqueue(ch, ACTION_TYPE_MOUNT, &param, 0.0f, true);
	 */
}

ACMD(do_fishing)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	ch->SetRotation(atof(arg1));
	ch->fishing();
}

ACMD(do_console)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "ConsoleEnable");
}

ACMD(do_restart)
{
	if (false == ch->IsDead())
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseRestartWindow");
		ch->StartRecoveryEvent();
		return;
	}
#ifdef WJ_SECURITY_SYSTEM
	if (ch->IsActivateSecurity() == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("guvenlik_aktif"));
		return;
	}
#endif
	if (NULL == ch->m_pkDeadEvent)
		return;

	int iTimeToDead = (event_time(ch->m_pkDeadEvent) / passes_per_sec);

	if (subcmd != SCMD_RESTART_TOWN && (!ch->GetWarMap() || ch->GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG))
	{
		if (!test_server)
		{
			if (ch->IsHack())
			{
				//¼ºÁö ¸ÊÀÏ°æ¿ì¿¡´Â Ã¼Å© ÇÏÁö ¾Ê´Â´Ù.
				if (false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ Àç½ÃÀÛ ÇÒ ¼ö ¾ø½À´Ï´Ù. (%dÃÊ ³²À½)"), iTimeToDead - (180 - g_nPortalLimitTime));
					return;
				}
			}

			if (iTimeToDead > 170)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ Àç½ÃÀÛ ÇÒ ¼ö ¾ø½À´Ï´Ù. (%dÃÊ ³²À½)"), iTimeToDead - 170);
				return;
			}
		}
	}

	//PREVENT_HACK
	//DESC : Ã¢°í, ±³È¯ Ã¢ ÈÄ Æ÷Å»À» »ç¿ëÇÏ´Â ¹ö±×¿¡ ÀÌ¿ëµÉ¼ö ÀÖ¾î¼­
	//		ÄðÅ¸ÀÓÀ» Ãß°¡ 
	if (subcmd == SCMD_RESTART_TOWN)
	{
		if (ch->IsHack())
		{
			//±æµå¸Ê, ¼ºÁö¸Ê¿¡¼­´Â Ã¼Å© ÇÏÁö ¾Ê´Â´Ù.
			if ((!ch->GetWarMap() || ch->GetWarMap()->GetType() == GUILD_WAR_TYPE_FLAG) ||
			   	false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ Àç½ÃÀÛ ÇÒ ¼ö ¾ø½À´Ï´Ù. (%dÃÊ ³²À½)"), iTimeToDead - (180 - g_nPortalLimitTime));
				return;
			}
		}

		if (iTimeToDead > 173)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¾ÆÁ÷ ¸¶À»¿¡¼­ Àç½ÃÀÛ ÇÒ ¼ö ¾ø½À´Ï´Ù. (%d ÃÊ ³²À½)"), iTimeToDead - 173);
			return;
		}
	}
	//END_PREVENT_HACK

	ch->ChatPacket(CHAT_TYPE_COMMAND, "CloseRestartWindow");

	ch->GetDesc()->SetPhase(PHASE_GAME);
	ch->SetPosition(POS_STANDING);
	ch->StartRecoveryEvent();

	//FORKED_LOAD
	//DESC: »ï°Å¸® ÀüÅõ½Ã ºÎÈ°À» ÇÒ°æ¿ì ¸ÊÀÇ ÀÔ±¸°¡ ¾Æ´Ñ »ï°Å¸® ÀüÅõÀÇ ½ÃÀÛÁöÁ¡À¸·Î ÀÌµ¿ÇÏ°Ô µÈ´Ù.
	if (1 == quest::CQuestManager::instance().GetEventFlag("threeway_war"))
	{
		if (subcmd == SCMD_RESTART_TOWN || subcmd == SCMD_RESTART_HERE)
		{
			if (true == CThreeWayWar::instance().IsThreeWayWarMapIndex(ch->GetMapIndex()) &&
					false == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
			{
				ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));

				ch->ReviveInvisible(5);
				ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
				ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());

				return;
			}

			//¼ºÁö 
			if (true == CThreeWayWar::instance().IsSungZiMapIndex(ch->GetMapIndex()))
			{
				if (CThreeWayWar::instance().GetReviveTokenForPlayer(ch->GetPlayerID()) <= 0)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ºÁö¿¡¼­ ºÎÈ° ±âÈ¸¸¦ ¸ðµÎ ÀÒ¾ú½À´Ï´Ù! ¸¶À»·Î ÀÌµ¿ÇÕ´Ï´Ù!"));
					ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));
				}
				else
				{
					ch->Show(ch->GetMapIndex(), GetSungziStartX(ch->GetEmpire()), GetSungziStartY(ch->GetEmpire()));
				}

				ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
				ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
				ch->ReviveInvisible(5);

				return;
			}
		}
	}
	//END_FORKED_LOAD

	if (ch->GetDungeon())
		ch->GetDungeon()->UseRevive(ch);

	if (ch->GetWarMap() && !ch->IsObserverMode())
	{
		CWarMap * pMap = ch->GetWarMap();
		DWORD dwGuildOpponent = pMap ? pMap->GetGuildOpponent(ch) : 0;

		if (dwGuildOpponent)
		{
			switch (subcmd)
			{
				case SCMD_RESTART_TOWN:
					sys_log(0, "do_restart: restart town");
					PIXEL_POSITION pos;

					if (CWarMapManager::instance().GetStartPosition(ch->GetMapIndex(), ch->GetGuild()->GetID() < dwGuildOpponent ? 0 : 1, pos))
						ch->Show(ch->GetMapIndex(), pos.x, pos.y);
					else
						ch->ExitToSavedLocation();

					ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
					ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
					ch->ReviveInvisible(5);
					break;

				case SCMD_RESTART_HERE:
					sys_log(0, "do_restart: restart here");
					ch->RestartAtSamePos();
					//ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY());
					ch->PointChange(POINT_HP, ch->GetMaxHP() - ch->GetHP());
					ch->PointChange(POINT_SP, ch->GetMaxSP() - ch->GetSP());
					ch->ReviveInvisible(5);
					break;
			}

			return;
		}
	}

	switch (subcmd)
	{
		case SCMD_RESTART_TOWN:
			sys_log(0, "do_restart: restart town");
			PIXEL_POSITION pos;

			if (SECTREE_MANAGER::instance().GetRecallPositionByEmpire(ch->GetMapIndex(), ch->GetEmpire(), pos))
				ch->WarpSet(pos.x, pos.y);
			else
				ch->WarpSet(EMPIRE_START_X(ch->GetEmpire()), EMPIRE_START_Y(ch->GetEmpire()));

			ch->PointChange(POINT_HP, 50 - ch->GetHP());
			ch->DeathPenalty(1);
			break;

		case SCMD_RESTART_HERE:
			sys_log(0, "do_restart: restart here");
			ch->RestartAtSamePos();
			//ch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY());
			ch->PointChange(POINT_HP, 50 - ch->GetHP());
			ch->DeathPenalty(0);
			ch->ReviveInvisible(5);
			break;
	}
}

#define MAX_STAT 90

ACMD(do_stat_reset)
{
	ch->PointChange(POINT_STAT_RESET_COUNT, 12 - ch->GetPoint(POINT_STAT_RESET_COUNT));
}

ACMD(do_stat_minus)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (ch->IsPolymorphed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µÐ°© Áß¿¡´Â ´É·ÂÀ» ¿Ã¸± ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (ch->GetPoint(POINT_STAT_RESET_COUNT) <= 0)
		return;

	if (!strcmp(arg1, "st"))
	{
		if (ch->GetRealPoint(POINT_ST) <= JobInitialPoints[ch->GetJob()].st)
			return;

		ch->SetRealPoint(POINT_ST, ch->GetRealPoint(POINT_ST) - 1);
		ch->SetPoint(POINT_ST, ch->GetPoint(POINT_ST) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_ST, 0);
	}
	else if (!strcmp(arg1, "dx"))
	{
		if (ch->GetRealPoint(POINT_DX) <= JobInitialPoints[ch->GetJob()].dx)
			return;

		ch->SetRealPoint(POINT_DX, ch->GetRealPoint(POINT_DX) - 1);
		ch->SetPoint(POINT_DX, ch->GetPoint(POINT_DX) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_DX, 0);
	}
	else if (!strcmp(arg1, "ht"))
	{
		if (ch->GetRealPoint(POINT_HT) <= JobInitialPoints[ch->GetJob()].ht)
			return;

		ch->SetRealPoint(POINT_HT, ch->GetRealPoint(POINT_HT) - 1);
		ch->SetPoint(POINT_HT, ch->GetPoint(POINT_HT) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_HT, 0);
		ch->PointChange(POINT_MAX_HP, 0);
	}
	else if (!strcmp(arg1, "iq"))
	{
		if (ch->GetRealPoint(POINT_IQ) <= JobInitialPoints[ch->GetJob()].iq)
			return;

		ch->SetRealPoint(POINT_IQ, ch->GetRealPoint(POINT_IQ) - 1);
		ch->SetPoint(POINT_IQ, ch->GetPoint(POINT_IQ) - 1);
		ch->ComputePoints();
		ch->PointChange(POINT_IQ, 0);
		ch->PointChange(POINT_MAX_SP, 0);
	}
	else
		return;

	ch->PointChange(POINT_STAT, +1);
	ch->PointChange(POINT_STAT_RESET_COUNT, -1);
	ch->ComputePoints();
}

ACMD(do_stat)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (ch->IsPolymorphed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("µÐ°© Áß¿¡´Â ´É·ÂÀ» ¿Ã¸± ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (ch->GetPoint(POINT_STAT) <= 0)
		return;

	BYTE idx = 0;
	
	if (!strcmp(arg1, "st"))
		idx = POINT_ST;
	else if (!strcmp(arg1, "dx"))
		idx = POINT_DX;
	else if (!strcmp(arg1, "ht"))
		idx = POINT_HT;
	else if (!strcmp(arg1, "iq"))
		idx = POINT_IQ;
	else
		return;

	if (ch->GetRealPoint(idx) >= MAX_STAT)
		return;

	ch->SetRealPoint(idx, ch->GetRealPoint(idx) + 1);
	ch->SetPoint(idx, ch->GetPoint(idx) + 1);
	ch->ComputePoints();
	ch->PointChange(idx, 0);

	if (idx == POINT_IQ)
	{
		ch->PointChange(POINT_MAX_HP, 0);
	}
	else if (idx == POINT_HT)
	{
		ch->PointChange(POINT_MAX_SP, 0);
	}

	ch->PointChange(POINT_STAT, -1);
	ch->ComputePoints();
}

ACMD(do_pvp)
{
	if (ch->GetArena() != NULL || CArenaManager::instance().IsArenaMap(ch->GetMapIndex()) == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER pkVictim = CHARACTER_MANAGER::instance().Find(vid);

	if (!pkVictim)
		return;

	if (pkVictim->IsNPC())
		return;

#if defined(ENABLE_MESSENGER_BLOCK)
	if (MessengerManager::instance().CheckMessengerList(ch->GetName(), pkVictim->GetName(), MESSENGER_BLOCK))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Unblock %s to continue."), pkVictim->GetName());
		return;
	}
	else if (MessengerManager::instance().CheckMessengerList(pkVictim->GetName(), ch->GetName(), MESSENGER_BLOCK))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s has blocked you."), pkVictim->GetName());
		return;
	}
#endif

	if (pkVictim->GetArena() != NULL)
	{
		pkVictim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ó´ë¹æÀÌ ´ë·ÃÁßÀÔ´Ï´Ù."));
		return;
	}

	CPVPManager::instance().Insert(ch, pkVictim);
}

ACMD(do_guildskillup)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (!ch->GetGuild())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå¿¡ ¼ÓÇØÀÖÁö ¾Ê½À´Ï´Ù."));
		return;
	}

	CGuild* g = ch->GetGuild();
	TGuildMember* gm = g->GetMember(ch->GetPlayerID());
	if (gm->grade == GUILD_LEADER_GRADE)
	{
		DWORD vnum = 0;
		str_to_number(vnum, arg1);
		g->SkillLevelUp(vnum);
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµå ½ºÅ³ ·¹º§À» º¯°æÇÒ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
	}
}

ACMD(do_skillup)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vnum = 0;
	str_to_number(vnum, arg1);

	if (true == ch->CanUseSkill(vnum))
	{
		ch->SkillLevelUp(vnum);
	}
	else
	{
		switch(vnum)
		{
			case SKILL_HORSE_WILDATTACK:
			case SKILL_HORSE_CHARGE:
			case SKILL_HORSE_ESCAPE:
			case SKILL_HORSE_WILDATTACK_RANGE:
			case SKILL_ADD_HP:
			case SKILL_RESIST_PENETRATE:
#ifdef __7AND8TH_SKILLS__
			case SKILL_ANTI_PALBANG:
			case SKILL_ANTI_AMSEOP:
			case SKILL_ANTI_SWAERYUNG:
			case SKILL_ANTI_YONGBI:
			case SKILL_ANTI_GIGONGCHAM:
			case SKILL_ANTI_HWAJO:
			case SKILL_ANTI_MAHWAN:
			case SKILL_ANTI_BYEURAK:
#ifdef __WOLFMAN_CHARACTER__
			case SKILL_ANTI_PASWAE:
#endif
			case SKILL_HELP_PALBANG:
			case SKILL_HELP_AMSEOP:
			case SKILL_HELP_SWAERYUNG:
			case SKILL_HELP_YONGBI:
			case SKILL_HELP_GIGONGCHAM:
			case SKILL_HELP_HWAJO:
			case SKILL_HELP_MAHWAN:
			case SKILL_HELP_BYEURAK:
#ifdef __WOLFMAN_CHARACTER__
			case SKILL_HELP_PASWAE:
#endif
#endif
				ch->SkillLevelUp(vnum);
				break;
		}
	}
}

//
// @version	05/06/20 Bang2ni - Ä¿¸Çµå Ã³¸® Delegate to CHARACTER class
//
ACMD(do_safebox_close)
{
	ch->CloseSafebox();
}

EVENTINFO(TimedEventChInfo)
{
	DynamicCharacterPtr ch;
	int     left_second;
	int		kanal;

	TimedEventChInfo() 
	: ch()
	, kanal( 0 )
	, left_second( 0 )
	{
	}
};

EVENTFUNC(kanal_event)
{
	TimedEventChInfo * info = dynamic_cast<TimedEventChInfo *>( event->info );
	
	if ( info == NULL )
	{
		sys_err( "timed_event> <Factor> Null pointer" );
		return 0;
	}

	LPCHARACTER	ch = info->ch;
	if (ch == NULL) { // <Factor>
		return 0;
	}
	
	LPDESC d = ch->GetDesc();

	if (info->left_second <= 0)
	{
		ch->m_pkTimedEventCh = NULL;
		ch->KanalSwitch(info->kanal);
		return 0;
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d_chdegis"), info->left_second);
		--info->left_second;
	}
	return PASSES_PER_SEC(1);
}

ACMD(do_click_safebox)
{
	if (ch->GetMapIndex() == 66)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("depo_yok_burda"));
		return;
	}
	if (ch->GetMapIndex() == 235)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("depo_yok_burda"));
		return;
	}
	if (ch->GetMapIndex() == 240)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("depo_yok_burda"));
		return;
	}
	if (ch->GetMapIndex() == 216)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("depo_yok_burda"));
		return;
	}
	if (ch->GetMapIndex() == 212)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("depo_yok_burda"));
		return;
	}
	if (ch->GetMapIndex() == 356)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("depo_yok_burda"));
		return;
	}
    ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowMeSafeboxPassword");
}

ACMD(do_kanal_switch)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;
	
	if (ch->m_pkTimedEventCh)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("hizlichdurdu"));
		event_cancel(&ch->m_pkTimedEventCh);
		return;
	}
	
	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	{
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
        return;
	}
	
	
	if (ch->IsSashCombinationOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("kombinasyon_penceresi_acik"));
		return;
	}
	
	if (ch->IsSashAbsorptionOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bonus_emis_penceresi_acik"));
		return;
	}
	
	
	if (ch->IsHack())
		return;

	int iPulse = thecore_pulse();	
	if ( iPulse - ch->GetExchangeTime()  < PASSES_PER_SEC(g_nPortalLimitTime) )
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°Å·¡ ÈÄ %dÃÊ ÀÌ³»¿¡´Â ´Ù¸¥Áö¿ªÀ¸·Î ÀÌµ¿ ÇÒ ¼ö ¾ø½À´Ï´Ù."), g_nPortalLimitTime);
		return;
	}
	if ( iPulse - ch->GetMyShopTime() < PASSES_PER_SEC(g_nPortalLimitTime) )
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("°Å·¡ ÈÄ %dÃÊ ÀÌ³»¿¡´Â ´Ù¸¥Áö¿ªÀ¸·Î ÀÌµ¿ ÇÒ ¼ö ¾ø½À´Ï´Ù."), g_nPortalLimitTime);
		return;
	}
	int new_ch;
	str_to_number(new_ch, arg1);
	if( new_ch <1 || new_ch >6)   // REPLACE 2 WITH YOUR MAX_CHANNEL 
		return;

	if (!ch->IsPC() || ch->GetDungeon())
		return;

	if (g_bChannel == 99)
		return;
	
	if (g_bChannel == new_ch)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("zatenochdesin"));
		return;
	}
	
	TimedEventChInfo* info = AllocEventInfo<TimedEventChInfo>();
	info->left_second = 3;
	info->ch		= ch;
	info->kanal		= new_ch;

	ch->m_pkTimedEventCh	= event_create(kanal_event, info, 1);
}

//
// @version	05/06/20 Bang2ni - Ä¿¸Çµå Ã³¸® Delegate to CHARACTER class
//
ACMD(do_safebox_password)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	ch->ReqSafeboxLoad(arg1);
}

ACMD(do_safebox_change_password)
{
	char arg1[256];
	char arg2[256];

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || strlen(arg1)>6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Àß¸øµÈ ¾ÏÈ£¸¦ ÀÔ·ÂÇÏ¼Ì½À´Ï´Ù."));
		return;
	}

	if (!*arg2 || strlen(arg2)>6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Àß¸øµÈ ¾ÏÈ£¸¦ ÀÔ·ÂÇÏ¼Ì½À´Ï´Ù."));
		return;
	}

	if (LC_IsBrazil() == true)
	{
		for (int i = 0; i < 6; ++i)
		{
			if (arg2[i] == '\0')
				break;

			if (isalpha(arg2[i]) == false)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> ºñ¹Ð¹øÈ£´Â ¿µ¹®ÀÚ¸¸ °¡´ÉÇÕ´Ï´Ù."));
				return;
			}
		}
	}

	TSafeboxChangePasswordPacket p;

	p.dwID = ch->GetDesc()->GetAccountTable().id;
	strlcpy(p.szOldPassword, arg1, sizeof(p.szOldPassword));
	strlcpy(p.szNewPassword, arg2, sizeof(p.szNewPassword));

	db_clientdesc->DBPacket(HEADER_GD_SAFEBOX_CHANGE_PASSWORD, ch->GetDesc()->GetHandle(), &p, sizeof(p));
}

ACMD(do_mall_password)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1 || strlen(arg1) > 6)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Àß¸øµÈ ¾ÏÈ£¸¦ ÀÔ·ÂÇÏ¼Ì½À´Ï´Ù."));
		return;
	}

	int iPulse = thecore_pulse();

	if (ch->GetMall())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Ã¢°í°¡ ÀÌ¹Ì ¿­·ÁÀÖ½À´Ï´Ù."));
		return;
	}

	if (iPulse - ch->GetMallLoadTime() < passes_per_sec * 10) // 10ÃÊ¿¡ ÇÑ¹ø¸¸ ¿äÃ» °¡´É
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Ã¢°í> Ã¢°í¸¦ ´ÝÀºÁö 10ÃÊ ¾È¿¡´Â ¿­ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	ch->SetMallLoadTime(iPulse);

	TSafeboxLoadPacket p;
	p.dwID = ch->GetDesc()->GetAccountTable().id;
	strlcpy(p.szLogin, ch->GetDesc()->GetAccountTable().login, sizeof(p.szLogin));
	strlcpy(p.szPassword, arg1, sizeof(p.szPassword));

	db_clientdesc->DBPacket(HEADER_GD_MALL_LOAD, ch->GetDesc()->GetHandle(), &p, sizeof(p));
}

ACMD(do_mall_close)
{
	if (ch->GetMall())
	{
		ch->SetMallLoadTime(thecore_pulse());
		ch->CloseMall();
		ch->Save();
	}
}

ACMD(do_ungroup)
{
	if (!ch->GetParty())
		return;

	if (!CPartyManager::instance().IsEnablePCParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ¼­¹ö ¹®Á¦·Î ÆÄÆ¼ °ü·Ã Ã³¸®¸¦ ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (ch->GetDungeon())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ´øÀü ¾È¿¡¼­´Â ÆÄÆ¼¿¡¼­ ³ª°¥ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	LPPARTY pParty = ch->GetParty();

	if (pParty->GetMemberCount() == 2)
	{
		// party disband
		CPartyManager::instance().DeleteParty(pParty);
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<ÆÄÆ¼> ÆÄÆ¼¿¡¼­ ³ª°¡¼Ì½À´Ï´Ù."));
		//pParty->SendPartyRemoveOneToAll(ch);
		pParty->Quit(ch->GetPlayerID());
		//pParty->SendPartyRemoveAllToOne(ch);
	}
}

ACMD(do_close_shop)
{
	if (ch->GetMyShop())
	{
		ch->CloseMyShop();
		return;
	}
}

ACMD(do_set_walk_mode)
{
	ch->SetNowWalking(true);
	ch->SetWalking(true);
}

ACMD(do_set_run_mode)
{
	ch->SetNowWalking(false);
	ch->SetWalking(false);
}

#if defined(ENABLE_AFFECT_POLYMORPH_REMOVE)
ACMD(do_remove_polymorph)
{
	if (!ch)
		return;
	
	if (!ch->IsPolymorphed())
		return;
	
	ch->SetPolymorph(0);
	ch->RemoveAffect(AFFECT_POLYMORPH);
}
#endif

ACMD(do_war)
{
	//³» ±æµå Á¤º¸¸¦ ¾ò¾î¿À°í
	CGuild * g = ch->GetGuild();

	if (!g)
		return;

	//ÀüÀïÁßÀÎÁö Ã¼Å©ÇÑ¹ø!
	if (g->UnderAnyWar())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ÀÌ¹Ì ´Ù¸¥ ÀüÀï¿¡ ÂüÀü Áß ÀÔ´Ï´Ù."));
		return;
	}

	//ÆÄ¶ó¸ÞÅÍ¸¦ µÎ¹è·Î ³ª´©°í
	char arg1[256], arg2[256];
	int type = GUILD_WAR_TYPE_FIELD;
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1)
		return;

	if (*arg2)
   	{
      str_to_number(type, arg2);

      if (type >= GUILD_WAR_TYPE_MAX_NUM)
         type = GUILD_WAR_TYPE_FIELD;
         
 		if (type < 0)
        {
            sys_err("%s[%d] using a /war hack", ch->GetName(), ch->GetPlayerID());
            return;
        } 
	}

	//±æµåÀÇ ¸¶½ºÅÍ ¾ÆÀÌµð¸¦ ¾ò¾î¿ÂµÚ
	DWORD gm_pid = g->GetMasterPID();

	//¸¶½ºÅÍÀÎÁö Ã¼Å©(±æÀüÀº ±æµåÀå¸¸ÀÌ °¡´É)
	if (gm_pid != ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀü¿¡ ´ëÇÑ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
		return;
	}

	//»ó´ë ±æµå¸¦ ¾ò¾î¿À°í
	CGuild * opp_g = CGuildManager::instance().FindGuildByName(arg1);

	if (!opp_g)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±×·± ±æµå°¡ ¾ø½À´Ï´Ù."));
		return;
	}

	//»ó´ë±æµå¿ÍÀÇ »óÅÂ Ã¼Å©
	switch (g->GetGuildWarState(opp_g->GetID()))
	{
		case GUILD_WAR_NONE:
			{
				if (opp_g->UnderAnyWar())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµå°¡ ÀÌ¹Ì ÀüÀï Áß ÀÔ´Ï´Ù."));
					return;
				}

				int iWarPrice = KOR_aGuildWarInfo[type].iWarPrice;

				if (g->GetGuildMoney() < iWarPrice)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> Àüºñ°¡ ºÎÁ·ÇÏ¿© ±æµåÀüÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
					return;
				}

				if (opp_g->GetGuildMoney() < iWarPrice)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµåÀÇ Àüºñ°¡ ºÎÁ·ÇÏ¿© ±æµåÀüÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
					return;
				}
			}
			break;

		case GUILD_WAR_SEND_DECLARE:
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì ¼±ÀüÆ÷°í ÁßÀÎ ±æµåÀÔ´Ï´Ù."));
				return;
			}
			break;

		case GUILD_WAR_RECV_DECLARE:
			{
				if (opp_g->UnderAnyWar())
				{
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµå°¡ ÀÌ¹Ì ÀüÀï Áß ÀÔ´Ï´Ù."));
					g->RequestRefuseWar(opp_g->GetID());
					return;
				}
			}
			break;

		case GUILD_WAR_RESERVE:
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ÀÌ¹Ì ÀüÀïÀÌ ¿¹¾àµÈ ±æµå ÀÔ´Ï´Ù."));
				return;
			}
			break;

		case GUILD_WAR_END:
			return;

		default:
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ÀÌ¹Ì ÀüÀï ÁßÀÎ ±æµåÀÔ´Ï´Ù."));
			g->RequestRefuseWar(opp_g->GetID());
			return;
	}

	if (!g->CanStartWar(type))
	{
		// ±æµåÀüÀ» ÇÒ ¼ö ÀÖ´Â Á¶°ÇÀ» ¸¸Á·ÇÏÁö¾Ê´Â´Ù.
		if (g->GetLadderPoint() == 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ·¹´õ Á¡¼ö°¡ ¸ðÀÚ¶ó¼­ ±æµåÀüÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
			sys_log(0, "GuildWar.StartError.NEED_LADDER_POINT");
		}
		else if (g->GetMemberCount() < GUILD_WAR_MIN_MEMBER_COUNT)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀüÀ» ÇÏ±â À§ÇØ¼± ÃÖ¼ÒÇÑ %d¸íÀÌ ÀÖ¾î¾ß ÇÕ´Ï´Ù."), GUILD_WAR_MIN_MEMBER_COUNT);
			sys_log(0, "GuildWar.StartError.NEED_MINIMUM_MEMBER[%d]", GUILD_WAR_MIN_MEMBER_COUNT);
		}
		else
		{
			sys_log(0, "GuildWar.StartError.UNKNOWN_ERROR");
		}
		return;
	}

	// ÇÊµåÀü Ã¼Å©¸¸ ÇÏ°í ¼¼¼¼ÇÑ Ã¼Å©´Â »ó´ë¹æÀÌ ½Â³«ÇÒ¶§ ÇÑ´Ù.
	if (!opp_g->CanStartWar(GUILD_WAR_TYPE_FIELD))
	{
		if (opp_g->GetLadderPoint() == 0)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµåÀÇ ·¹´õ Á¡¼ö°¡ ¸ðÀÚ¶ó¼­ ±æµåÀüÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		else if (opp_g->GetMemberCount() < GUILD_WAR_MIN_MEMBER_COUNT)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµåÀÇ ±æµå¿ø ¼ö°¡ ºÎÁ·ÇÏ¿© ±æµåÀüÀ» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	do
	{
		if (g->GetMasterCharacter() != NULL)
			break;

		CCI *pCCI = P2P_MANAGER::instance().FindByPID(g->GetMasterPID());

		if (pCCI != NULL)
			break;

		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµåÀÇ ±æµåÀåÀÌ Á¢¼ÓÁßÀÌ ¾Æ´Õ´Ï´Ù."));
		g->RequestRefuseWar(opp_g->GetID());
		return;

	} while (false);

	do
	{
		if (opp_g->GetMasterCharacter() != NULL)
			break;
		
		CCI *pCCI = P2P_MANAGER::instance().FindByPID(opp_g->GetMasterPID());
		
		if (pCCI != NULL)
			break;

		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> »ó´ë¹æ ±æµåÀÇ ±æµåÀåÀÌ Á¢¼ÓÁßÀÌ ¾Æ´Õ´Ï´Ù."));
		g->RequestRefuseWar(opp_g->GetID());
		return;

	} while (false);

	g->RequestDeclareWar(opp_g->GetID(), type);
}

ACMD(do_nowar)
{
	CGuild* g = ch->GetGuild();
	if (!g)
		return;

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD gm_pid = g->GetMasterPID();

	if (gm_pid != ch->GetPlayerID())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±æµåÀü¿¡ ´ëÇÑ ±ÇÇÑÀÌ ¾ø½À´Ï´Ù."));
		return;
	}

	CGuild* opp_g = CGuildManager::instance().FindGuildByName(arg1);

	if (!opp_g)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<±æµå> ±×·± ±æµå°¡ ¾ø½À´Ï´Ù."));
		return;
	}

	g->RequestRefuseWar(opp_g->GetID());
}

ACMD(do_detaillog)
{
	ch->DetailLog();
}

ACMD(do_monsterlog)
{
	ch->ToggleMonsterLog();
}

ACMD(do_pkmode)
{
if (!ch->IsGM() && ch->GetMapIndex() == 41,1,21,64,63,61,65,104,71,62,70,67,68,73,72,301,302,304,303,210,47,353,355,7,354,2)
{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Alpar2'de Farm ve Þehir bölgelerinde serbest açýlamaz!"));
		return;
    }
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	BYTE mode = 0;
	str_to_number(mode, arg1);

	if (mode == PK_MODE_PROTECT)
		return;

	if (ch->GetLevel() < PK_PROTECT_LEVEL && mode != 0)
		return;

	ch->SetPKMode(mode);
}

ACMD(do_messenger_auth)
{
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || !*arg2)
		return;

	char answer = LOWER(*arg1);

	bool bIsDenied = answer != 'y';
	bool bIsAdded = MessengerManager::instance().AuthToAdd(ch->GetName(), arg2, bIsDenied); // DENY
	if (bIsAdded && bIsDenied)
	{
		LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(arg2);

		if (tch)
			tch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ´ÔÀ¸·Î ºÎÅÍ Ä£±¸ µî·ÏÀ» °ÅºÎ ´çÇß½À´Ï´Ù."), ch->GetName());
	}

	//MessengerManager::instance().AuthToAdd(ch->GetName(), arg2, answer == 'y' ? false : true); // DENY
}

ACMD(do_setblockmode)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (*arg1)
	{
		BYTE flag = 0;
		str_to_number(flag, arg1);
		ch->SetBlockMode(flag);
	}
}

ACMD(do_unmount)
{
	LPITEM item = ch->GetWear(WEAR_UNIQUE1);
	LPITEM item2 = ch->GetWear(WEAR_UNIQUE2);
	LPITEM item3 = ch->GetWear(WEAR_COSTUME_MOUNT);

	if (item && item->IsRideItem())
		ch->UnequipItem(item);

	if (item2 && item2->IsRideItem())
		ch->UnequipItem(item2);

	if (item3 && item3->IsRideItem())
		ch->UnequipItem(item3);

	if (true == ch->UnEquipSpecialRideUniqueItem())
	{
		ch->RemoveAffect(AFFECT_MOUNT);
		ch->RemoveAffect(AFFECT_MOUNT_BONUS);

		if (ch->IsHorseRiding())
		{
			ch->StopRiding();
		}
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("AI??Aa¢¬¢ç¡Æ? ?E A¡À?¡© ??¢¬¡¾ ?o ???A¢¥I¢¥U."));
	}

}

ACMD(do_observer_exit)
{
	if (ch->IsObserverMode())
	{
		if (ch->GetWarMap())
			ch->SetWarMap(NULL);

		if (ch->GetArena() != NULL || ch->GetArenaObserverMode() == true)
		{
			ch->SetArenaObserverMode(false);

			if (ch->GetArena() != NULL)
				ch->GetArena()->RemoveObserver(ch->GetPlayerID());

			ch->SetArena(NULL);
			ch->WarpSet(ARENA_RETURN_POINT_X(ch->GetEmpire()), ARENA_RETURN_POINT_Y(ch->GetEmpire()));
		}
		else
		{
			ch->ExitToSavedLocation();
		}
		ch->SetObserverMode(false);
	}
}

ACMD(do_view_equip)
{
	if (ch->GetGMLevel() <= GM_PLAYER)
		return;

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (*arg1)
	{
		DWORD vid = 0;
		str_to_number(vid, arg1);
		LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

		if (!tch)
			return;

		if (!tch->IsPC())
			return;
		/*
		   int iSPCost = ch->GetMaxSP() / 3;

		   if (ch->GetSP() < iSPCost)
		   {
		   ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Á¤½Å·ÂÀÌ ºÎÁ·ÇÏ¿© ´Ù¸¥ »ç¶÷ÀÇ Àåºñ¸¦ º¼ ¼ö ¾ø½À´Ï´Ù."));
		   return;
		   }
		   ch->PointChange(POINT_SP, -iSPCost);
		 */
		tch->SendEquipment(ch);
	}
}

ACMD(do_party_request)
{
	if (ch->GetArena())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´ë·ÃÀå¿¡¼­ »ç¿ëÇÏ½Ç ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (ch->GetParty())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÌ¹Ì ÆÄÆ¼¿¡ ¼ÓÇØ ÀÖÀ¸¹Ç·Î °¡ÀÔ½ÅÃ»À» ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		if (!ch->RequestToParty(tch))
			ch->ChatPacket(CHAT_TYPE_COMMAND, "PartyRequestDenied");
}

ACMD(do_party_request_accept)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		ch->AcceptToParty(tch);
}

ACMD(do_party_request_deny)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD vid = 0;
	str_to_number(vid, arg1);
	LPCHARACTER tch = CHARACTER_MANAGER::instance().Find(vid);

	if (tch)
		ch->DenyToParty(tch);
}

ACMD(do_monarch_warpto)
{
	if (true == LC_IsYMIR() || true == LC_IsKorea())
		return;

	if (!CMonarch::instance().IsMonarch(ch->GetPlayerID(), ch->GetEmpire()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±ºÁÖ¸¸ÀÌ »ç¿ë °¡´ÉÇÑ ±â´ÉÀÔ´Ï´Ù"));
		return;
	}
	
	//±ºÁÖ ÄðÅ¸ÀÓ °Ë»ç
	if (!ch->IsMCOK(CHARACTER::MI_WARP))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d ÃÊ°£ ÄðÅ¸ÀÓÀÌ Àû¿ëÁßÀÔ´Ï´Ù."), ch->GetMCLTime(CHARACTER::MI_WARP));
		return;
	}

	//±ºÁÖ ¸÷ ¼ÒÈ¯ ºñ¿ë 
	const int WarpPrice = 10000;
	
	//±ºÁÖ ±¹°í °Ë»ç 
	if (!CMonarch::instance().IsMoneyOk(WarpPrice, ch->GetEmpire()))
	{
		int NationMoney = CMonarch::instance().GetMoney(ch->GetEmpire());
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±¹°í¿¡ µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù. ÇöÀç : %u ÇÊ¿ä±Ý¾× : %u"), NationMoney, WarpPrice);
		return;	
	}

	int x = 0, y = 0;
	char arg1[256];

	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ç¿ë¹ý: warpto <character name>"));
		return;
	}

	LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(arg1);

	if (!tch)
	{
		CCI * pkCCI = P2P_MANAGER::instance().Find(arg1);

		if (pkCCI)
		{
			if (pkCCI->bEmpire != ch->GetEmpire())
			{
				ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("Å¸Á¦±¹ À¯Àú¿¡°Ô´Â ÀÌµ¿ÇÒ¼ö ¾ø½À´Ï´Ù"));
				return;
			}

			if (pkCCI->bChannel != g_bChannel)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÇØ´ç À¯Àú´Â %d Ã¤³Î¿¡ ÀÖ½À´Ï´Ù. (ÇöÀç Ã¤³Î %d)"), pkCCI->bChannel, g_bChannel);
				return;
			}
			if (!IsMonarchWarpZone(pkCCI->lMapIndex))
			{
				ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("ÇØ´ç Áö¿ªÀ¸·Î ÀÌµ¿ÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return;
			}

			PIXEL_POSITION pos;
	
			if (!SECTREE_MANAGER::instance().GetCenterPositionOfMap(pkCCI->lMapIndex, pos))
				ch->ChatPacket(CHAT_TYPE_INFO, "Cannot find map (index %d)", pkCCI->lMapIndex);
			else
			{
				//ch->ChatPacket(CHAT_TYPE_INFO, "You warp to (%d, %d)", pos.x, pos.y);
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ¿¡°Ô·Î ÀÌµ¿ÇÕ´Ï´Ù"), arg1);
				ch->WarpSet(pos.x, pos.y);
				
				//±ºÁÖ µ· »è°¨	
				CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);

				//ÄðÅ¸ÀÓ ÃÊ±âÈ­ 
				ch->SetMC(CHARACTER::MI_WARP);
			}
		}
		else if (NULL == CHARACTER_MANAGER::instance().FindPC(arg1))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "There is no one by that name");
		}

		return;
	}
	else
	{
		if (tch->GetEmpire() != ch->GetEmpire())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Å¸Á¦±¹ À¯Àú¿¡°Ô´Â ÀÌµ¿ÇÒ¼ö ¾ø½À´Ï´Ù"));
			return;
		}
		if (!IsMonarchWarpZone(tch->GetMapIndex()))
		{
			ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("ÇØ´ç Áö¿ªÀ¸·Î ÀÌµ¿ÇÒ ¼ö ¾ø½À´Ï´Ù."));
			return;
		}
		x = tch->GetX();
		y = tch->GetY();
	}

	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ¿¡°Ô·Î ÀÌµ¿ÇÕ´Ï´Ù"), arg1);
	ch->WarpSet(x, y);
	ch->Stop();

	//±ºÁÖ µ· »è°¨	
	CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);

	//ÄðÅ¸ÀÓ ÃÊ±âÈ­ 
	ch->SetMC(CHARACTER::MI_WARP);
}

ACMD(do_monarch_transfer)
{
	if (true == LC_IsYMIR() || true == LC_IsKorea())
		return;

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("»ç¿ë¹ý: transfer <name>"));
		return;
	}
	
	if (!CMonarch::instance().IsMonarch(ch->GetPlayerID(), ch->GetEmpire()))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±ºÁÖ¸¸ÀÌ »ç¿ë °¡´ÉÇÑ ±â´ÉÀÔ´Ï´Ù"));
		return;
	}
	
	//±ºÁÖ ÄðÅ¸ÀÓ °Ë»ç
	if (!ch->IsMCOK(CHARACTER::MI_TRANSFER))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d ÃÊ°£ ÄðÅ¸ÀÓÀÌ Àû¿ëÁßÀÔ´Ï´Ù."), ch->GetMCLTime(CHARACTER::MI_TRANSFER));	
		return;
	}

	//±ºÁÖ ¿öÇÁ ºñ¿ë 
	const int WarpPrice = 10000;

	//±ºÁÖ ±¹°í °Ë»ç 
	if (!CMonarch::instance().IsMoneyOk(WarpPrice, ch->GetEmpire()))
	{
		int NationMoney = CMonarch::instance().GetMoney(ch->GetEmpire());
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±¹°í¿¡ µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù. ÇöÀç : %u ÇÊ¿ä±Ý¾× : %u"), NationMoney, WarpPrice);
		return;	
	}


	LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(arg1);

	if (!tch)
	{
		CCI * pkCCI = P2P_MANAGER::instance().Find(arg1);

		if (pkCCI)
		{
			if (pkCCI->bEmpire != ch->GetEmpire())
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ Á¦±¹ À¯Àú´Â ¼ÒÈ¯ÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return;
			}
			if (pkCCI->bChannel != g_bChannel)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ´ÔÀº %d Ã¤³Î¿¡ Á¢¼Ó Áß ÀÔ´Ï´Ù. (ÇöÀç Ã¤³Î: %d)"), arg1, pkCCI->bChannel, g_bChannel);
				return;
			}
			if (!IsMonarchWarpZone(pkCCI->lMapIndex))
			{
				ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("ÇØ´ç Áö¿ªÀ¸·Î ÀÌµ¿ÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return;
			}
			if (!IsMonarchWarpZone(ch->GetMapIndex()))
			{
				ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("ÇØ´ç Áö¿ªÀ¸·Î ¼ÒÈ¯ÇÒ ¼ö ¾ø½À´Ï´Ù."));
				return;
			}

			TPacketGGTransfer pgg;

			pgg.bHeader = HEADER_GG_TRANSFER;
			strlcpy(pgg.szName, arg1, sizeof(pgg.szName));
			pgg.lX = ch->GetX();
			pgg.lY = ch->GetY();

			P2P_MANAGER::instance().Send(&pgg, sizeof(TPacketGGTransfer));
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s ´ÔÀ» ¼ÒÈ¯ÇÏ¿´½À´Ï´Ù."), arg1);
			
			//±ºÁÖ µ· »è°¨	
			CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);
			//ÄðÅ¸ÀÓ ÃÊ±âÈ­ 
			ch->SetMC(CHARACTER::MI_TRANSFER);
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÔ·ÂÇÏ½Å ÀÌ¸§À» °¡Áø »ç¿ëÀÚ°¡ ¾ø½À´Ï´Ù."));
		}

		return;
	}


	if (ch == tch)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÚ½ÅÀ» ¼ÒÈ¯ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	if (tch->GetEmpire() != ch->GetEmpire())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("´Ù¸¥ Á¦±¹ À¯Àú´Â ¼ÒÈ¯ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}
	if (!IsMonarchWarpZone(tch->GetMapIndex()))
	{
		ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("ÇØ´ç Áö¿ªÀ¸·Î ÀÌµ¿ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}
	if (!IsMonarchWarpZone(ch->GetMapIndex()))
	{
		ch->ChatPacket (CHAT_TYPE_INFO, LC_TEXT("ÇØ´ç Áö¿ªÀ¸·Î ¼ÒÈ¯ÇÒ ¼ö ¾ø½À´Ï´Ù."));
		return;
	}

	//tch->Show(ch->GetMapIndex(), ch->GetX(), ch->GetY(), ch->GetZ());
	tch->WarpSet(ch->GetX(), ch->GetY(), ch->GetMapIndex());
	
	//±ºÁÖ µ· »è°¨	
	CMonarch::instance().SendtoDBDecMoney(WarpPrice, ch->GetEmpire(), ch);
	//ÄðÅ¸ÀÓ ÃÊ±âÈ­ 
	ch->SetMC(CHARACTER::MI_TRANSFER);
}

ACMD(do_monarch_info)
{
	if (CMonarch::instance().IsMonarch(ch->GetPlayerID(), ch->GetEmpire()))	
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("³ªÀÇ ±ºÁÖ Á¤º¸"));
		TMonarchInfo * p = CMonarch::instance().GetMonarch();
		for (int n = 1; n < 4; ++n)
		{
			if (n == ch->GetEmpire())
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[%s±ºÁÖ] : %s  º¸À¯±Ý¾× %lld "), EMPIRE_NAME(n), p->name[n], p->money[n]);
			else
				ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[%s±ºÁÖ] : %s  "), EMPIRE_NAME(n), p->name[n]);
				
		}
	}
	else
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±ºÁÖ Á¤º¸"));
		TMonarchInfo * p = CMonarch::instance().GetMonarch();
		for (int n = 1; n < 4; ++n)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[%s±ºÁÖ] : %s  "), EMPIRE_NAME(n), p->name[n]);
				
		}
	}
	
}	

ACMD(do_elect)
{
	db_clientdesc->DBPacketHeader(HEADER_GD_COME_TO_VOTE, ch->GetDesc()->GetHandle(), 0);
}

// LUA_ADD_GOTO_INFO
struct GotoInfo
{
	std::string 	st_name;

	BYTE 	empire;
	int 	mapIndex;
	DWORD 	x, y;

	GotoInfo()
	{
		st_name 	= "";
		empire 		= 0;
		mapIndex 	= 0;

		x = 0;
		y = 0;
	}

	GotoInfo(const GotoInfo& c_src)
	{
		__copy__(c_src);
	}

	void operator = (const GotoInfo& c_src)
	{
		__copy__(c_src);
	}

	void __copy__(const GotoInfo& c_src)
	{
		st_name 	= c_src.st_name;
		empire 		= c_src.empire;
		mapIndex 	= c_src.mapIndex;

		x = c_src.x;
		y = c_src.y;
	}
};

#ifndef __FULL_NOTICE_SYSTEM__
extern void BroadcastNotice(const char * c_pszBuf);
#endif

ACMD(do_monarch_tax)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: monarch_tax <1-50>");
		return;
	}

	// ±ºÁÖ °Ë»ç	
	if (!ch->IsMonarch())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±ºÁÖ¸¸ÀÌ »ç¿ëÇÒ¼ö ÀÖ´Â ±â´ÉÀÔ´Ï´Ù"));
		return;
	}

	// ¼¼±Ý¼³Á¤ 
	int tax = 0;
	str_to_number(tax,  arg1);

	if (tax < 1 || tax > 50)
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("1-50 »çÀÌÀÇ ¼öÄ¡¸¦ ¼±ÅÃÇØÁÖ¼¼¿ä"));

	quest::CQuestManager::instance().SetEventFlag("trade_tax", tax); 

	// ±ºÁÖ¿¡°Ô ¸Þ¼¼Áö ÇÏ³ª
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼¼±ÝÀÌ %d %·Î ¼³Á¤µÇ¾ú½À´Ï´Ù"));

	// °øÁö 
	char szMsg[1024];	

	snprintf(szMsg, sizeof(szMsg), "±ºÁÖÀÇ ¸íÀ¸·Î ¼¼±ÝÀÌ %d %% ·Î º¯°æµÇ¾ú½À´Ï´Ù", tax);
	BroadcastNotice(szMsg);

	snprintf(szMsg, sizeof(szMsg), "¾ÕÀ¸·Î´Â °Å·¡ ±Ý¾×ÀÇ %d %% °¡ ±¹°í·Î µé¾î°¡°ÔµË´Ï´Ù.", tax);
	BroadcastNotice(szMsg);

	// ÄðÅ¸ÀÓ ÃÊ±âÈ­ 
	ch->SetMC(CHARACTER::MI_TAX); 
}

static const DWORD cs_dwMonarchMobVnums[] =
{
	191, //	»ê°ß½Å
	192, //	Àú½Å
	193, //	¿õ½Å
	194, //	È£½Å
	391, //	¹ÌÁ¤
	392, //	ÀºÁ¤
	393, //	¼¼¶û
	394, //	ÁøÈñ
	491, //	¸ÍÈ¯
	492, //	º¸¿ì
	493, //	±¸ÆÐ
	494, //	ÃßÈç
	591, //	ºñ·ù´Ü´ëÀå
	691, //	¿õ±Í Á·Àå
	791, //	¹Ð±³±³ÁÖ
	1304, // ´©··¹ü±Í
	1901, // ±¸¹ÌÈ£
	2091, // ¿©¿Õ°Å¹Ì
	2191, // °Å´ë»ç¸·°ÅºÏ
	2206, // È­¿°¿Õi
	0,
};

ACMD(do_monarch_mob)
{
	char arg1[256];
	LPCHARACTER	tch;

	one_argument(argument, arg1, sizeof(arg1));

	if (!ch->IsMonarch())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±ºÁÖ¸¸ÀÌ »ç¿ëÇÒ¼ö ÀÖ´Â ±â´ÉÀÔ´Ï´Ù"));
		return;
	}
	
	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: mmob <mob name>");
		return;
	}

	BYTE pcEmpire = ch->GetEmpire();
	BYTE mapEmpire = SECTREE_MANAGER::instance().GetEmpireFromMapIndex(ch->GetMapIndex());

	if (LC_IsYMIR() == true || LC_IsKorea() == true)
	{
		if (mapEmpire != pcEmpire && mapEmpire != 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ÀÚ±¹ ¿µÅä¿¡¼­¸¸ »ç¿ëÇÒ ¼ö ÀÖ´Â ±â´ÉÀÔ´Ï´Ù"));
			return;
		}
	}

	// ±ºÁÖ ¸÷ ¼ÒÈ¯ ºñ¿ë 
	const int SummonPrice = 5000000;

	// ±ºÁÖ ÄðÅ¸ÀÓ °Ë»ç
	if (!ch->IsMCOK(CHARACTER::MI_SUMMON))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d ÃÊ°£ ÄðÅ¸ÀÓÀÌ Àû¿ëÁßÀÔ´Ï´Ù."), ch->GetMCLTime(CHARACTER::MI_SUMMON));	
		return;
	}
	
	// ±ºÁÖ ±¹°í °Ë»ç 
	if (!CMonarch::instance().IsMoneyOk(SummonPrice, ch->GetEmpire()))
	{
		int NationMoney = CMonarch::instance().GetMoney(ch->GetEmpire());
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("±¹°í¿¡ µ·ÀÌ ºÎÁ·ÇÕ´Ï´Ù. ÇöÀç : %u ÇÊ¿ä±Ý¾× : %u"), NationMoney, SummonPrice);
		return;	
	}

	const CMob * pkMob;
	DWORD vnum = 0;

	if (isdigit(*arg1))
	{
		str_to_number(vnum, arg1);

		if ((pkMob = CMobManager::instance().Get(vnum)) == NULL)
			vnum = 0;
	}
	else
	{
		pkMob = CMobManager::Instance().Get(arg1, true);

		if (pkMob)
			vnum = pkMob->m_table.dwVnum;
	}

	DWORD count;

	// ¼ÒÈ¯ °¡´É ¸÷ °Ë»ç
	for (count = 0; cs_dwMonarchMobVnums[count] != 0; ++count)
		if (cs_dwMonarchMobVnums[count] == vnum)
			break;

	if (0 == cs_dwMonarchMobVnums[count])
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("¼ÒÈ¯ÇÒ¼ö ¾ø´Â ¸ó½ºÅÍ ÀÔ´Ï´Ù. ¼ÒÈ¯°¡´ÉÇÑ ¸ó½ºÅÍ´Â È¨ÆäÀÌÁö¸¦ ÂüÁ¶ÇÏ¼¼¿ä"));
		return;
	}

	tch = CHARACTER_MANAGER::instance().SpawnMobRange(vnum, 
			ch->GetMapIndex(),
			ch->GetX() - number(200, 750), 
			ch->GetY() - number(200, 750), 
			ch->GetX() + number(200, 750), 
			ch->GetY() + number(200, 750), 
			true,
			pkMob->m_table.bType == CHAR_TYPE_STONE,
			true);

	if (tch)
	{
		// ±ºÁÖ µ· »è°¨	
		CMonarch::instance().SendtoDBDecMoney(SummonPrice, ch->GetEmpire(), ch);

		// ÄðÅ¸ÀÓ ÃÊ±âÈ­ 
		ch->SetMC(CHARACTER::MI_SUMMON); 
	}
}

static const char* FN_point_string(int apply_number)
{
	switch (apply_number)
	{
		case POINT_MAX_HP:	return LC_TEXT("ÃÖ´ë »ý¸í·Â +%d");
		case POINT_MAX_SP:	return LC_TEXT("ÃÖ´ë Á¤½Å·Â +%d");
		case POINT_HT:		return LC_TEXT("Ã¼·Â +%d");
		case POINT_IQ:		return LC_TEXT("Áö´É +%d");
		case POINT_ST:		return LC_TEXT("±Ù·Â +%d");
		case POINT_DX:		return LC_TEXT("¹ÎÃ¸ +%d");
		case POINT_ATT_SPEED:	return LC_TEXT("°ø°Ý¼Óµµ +%d");
		case POINT_MOV_SPEED:	return LC_TEXT("ÀÌµ¿¼Óµµ %d");
		case POINT_CASTING_SPEED:	return LC_TEXT("ÄðÅ¸ÀÓ -%d");
		case POINT_HP_REGEN:	return LC_TEXT("»ý¸í·Â È¸º¹ +%d");
		case POINT_SP_REGEN:	return LC_TEXT("Á¤½Å·Â È¸º¹ +%d");
		case POINT_POISON_PCT:	return LC_TEXT("µ¶°ø°Ý %d");
		case POINT_STUN_PCT:	return LC_TEXT("½ºÅÏ +%d");
		case POINT_SLOW_PCT:	return LC_TEXT("½½·Î¿ì +%d");
		case POINT_CRITICAL_PCT:	return LC_TEXT("%d%% È®·ü·Î Ä¡¸íÅ¸ °ø°Ý");
		case POINT_RESIST_CRITICAL:	return LC_TEXT("»ó´ëÀÇ Ä¡¸íÅ¸ È®·ü %d%% °¨¼Ò");
		case POINT_PENETRATE_PCT:	return LC_TEXT("%d%% È®·ü·Î °üÅë °ø°Ý");
		case POINT_RESIST_PENETRATE: return LC_TEXT("»ó´ëÀÇ °üÅë °ø°Ý È®·ü %d%% °¨¼Ò");
		case POINT_ATTBONUS_HUMAN:	return LC_TEXT("ÀÎ°£·ù ¸ó½ºÅÍ Å¸°ÝÄ¡ +%d%%");
		case POINT_ATTBONUS_ANIMAL:	return LC_TEXT("µ¿¹°·ù ¸ó½ºÅÍ Å¸°ÝÄ¡ +%d%%");
		case POINT_ATTBONUS_ORC:	return LC_TEXT("¿õ±ÍÁ· Å¸°ÝÄ¡ +%d%%");
		case POINT_ATTBONUS_MILGYO:	return LC_TEXT("¹Ð±³·ù Å¸°ÝÄ¡ +%d%%");
		case POINT_ATTBONUS_UNDEAD:	return LC_TEXT("½ÃÃ¼·ù Å¸°ÝÄ¡ +%d%%");
		case POINT_ATTBONUS_DEVIL:	return LC_TEXT("¾Ç¸¶·ù Å¸°ÝÄ¡ +%d%%");
		case POINT_STEAL_HP:		return LC_TEXT("Å¸°ÝÄ¡ %d%% ¸¦ »ý¸í·ÂÀ¸·Î Èí¼ö");
		case POINT_STEAL_SP:		return LC_TEXT("Å¸·ÂÄ¡ %d%% ¸¦ Á¤½Å·ÂÀ¸·Î Èí¼ö");
		case POINT_MANA_BURN_PCT:	return LC_TEXT("%d%% È®·ü·Î Å¸°Ý½Ã »ó´ë Àü½Å·Â ¼Ò¸ð");
		case POINT_DAMAGE_SP_RECOVER:	return LC_TEXT("%d%% È®·ü·Î ÇÇÇØ½Ã Á¤½Å·Â È¸º¹");
		case POINT_BLOCK:			return LC_TEXT("¹°¸®Å¸°Ý½Ã ºí·° È®·ü %d%%");
		case POINT_DODGE:			return LC_TEXT("È° °ø°Ý È¸ÇÇ È®·ü %d%%");
		case POINT_RESIST_SWORD:	return LC_TEXT("ÇÑ¼Õ°Ë ¹æ¾î %d%%");
		case POINT_RESIST_TWOHAND:	return LC_TEXT("¾ç¼Õ°Ë ¹æ¾î %d%%");
		case POINT_RESIST_DAGGER:	return LC_TEXT("µÎ¼Õ°Ë ¹æ¾î %d%%");
		case POINT_RESIST_BELL:		return LC_TEXT("¹æ¿ï ¹æ¾î %d%%");
		case POINT_RESIST_FAN:		return LC_TEXT("ºÎÃ¤ ¹æ¾î %d%%");
		case POINT_RESIST_BOW:		return LC_TEXT("È°°ø°Ý ÀúÇ× %d%%");
		case POINT_RESIST_CLAW:		return LC_TEXT("Defense chance against claws +%d%%");
		case POINT_RESIST_FIRE:		return LC_TEXT("È­¿° ÀúÇ× %d%%");
		case POINT_RESIST_ELEC:		return LC_TEXT("Àü±â ÀúÇ× %d%%");
		case POINT_RESIST_MAGIC:	return LC_TEXT("¸¶¹ý ÀúÇ× %d%%");
		case POINT_RESIST_WIND:		return LC_TEXT("¹Ù¶÷ ÀúÇ× %d%%");
		case POINT_RESIST_ICE:		return LC_TEXT("³Ã±â ÀúÇ× %d%%");
		case POINT_RESIST_EARTH:	return LC_TEXT("´ëÁö ÀúÇ× %d%%");
		case POINT_RESIST_DARK:		return LC_TEXT("¾îµÒ ÀúÇ× %d%%");
		case POINT_REFLECT_MELEE:	return LC_TEXT("Á÷Á¢ Å¸°ÝÄ¡ ¹Ý»ç È®·ü : %d%%");
		case POINT_REFLECT_CURSE:	return LC_TEXT("ÀúÁÖ µÇµ¹¸®±â È®·ü %d%%");
		case POINT_POISON_REDUCE:	return LC_TEXT("µ¶ ÀúÇ× %d%%");
		case POINT_KILL_SP_RECOVER:	return LC_TEXT("%d%% È®·ü·Î ÀûÅðÄ¡½Ã Á¤½Å·Â È¸º¹");
		case POINT_EXP_DOUBLE_BONUS:	return LC_TEXT("%d%% È®·ü·Î ÀûÅðÄ¡½Ã °æÇèÄ¡ Ãß°¡ »ó½Â");
		case POINT_GOLD_DOUBLE_BONUS:	return LC_TEXT("%d%% È®·ü·Î ÀûÅðÄ¡½Ã µ· 2¹è µå·Ó");
		case POINT_ITEM_DROP_BONUS:	return LC_TEXT("%d%% È®·ü·Î ÀûÅðÄ¡½Ã ¾ÆÀÌÅÛ 2¹è µå·Ó");
		case POINT_POTION_BONUS:	return LC_TEXT("¹°¾à »ç¿ë½Ã %d%% ¼º´É Áõ°¡");
		case POINT_KILL_HP_RECOVERY:	return LC_TEXT("%d%% È®·ü·Î ÀûÅðÄ¡½Ã »ý¸í·Â È¸º¹");
//		case POINT_IMMUNE_STUN:	return LC_TEXT("±âÀýÇÏÁö ¾ÊÀ½ %d%%");
//		case POINT_IMMUNE_SLOW:	return LC_TEXT("´À·ÁÁöÁö ¾ÊÀ½ %d%%");
//		case POINT_IMMUNE_FALL:	return LC_TEXT("³Ñ¾îÁöÁö ¾ÊÀ½ %d%%");
//		case POINT_SKILL:	return LC_TEXT("");
//		case POINT_BOW_DISTANCE:	return LC_TEXT("");
		case POINT_ATT_GRADE_BONUS:	return LC_TEXT("°ø°Ý·Â +%d");
		case POINT_DEF_GRADE_BONUS:	return LC_TEXT("¹æ¾î·Â +%d");
		case POINT_MAGIC_ATT_GRADE:	return LC_TEXT("¸¶¹ý °ø°Ý·Â +%d");
		case POINT_MAGIC_DEF_GRADE:	return LC_TEXT("¸¶¹ý ¹æ¾î·Â +%d");
//		case POINT_CURSE_PCT:	return LC_TEXT("");
		case POINT_MAX_STAMINA:	return LC_TEXT("ÃÖ´ë Áö±¸·Â +%d");
		case POINT_ATTBONUS_WARRIOR:	return LC_TEXT("¹«»ç¿¡°Ô °­ÇÔ +%d%%");
		case POINT_ATTBONUS_ASSASSIN:	return LC_TEXT("ÀÚ°´¿¡°Ô °­ÇÔ +%d%%");
		case POINT_ATTBONUS_SURA:		return LC_TEXT("¼ö¶ó¿¡°Ô °­ÇÔ +%d%%");
		case POINT_ATTBONUS_SHAMAN:		return LC_TEXT("¹«´ç¿¡°Ô °­ÇÔ +%d%%");
		case POINT_ATTBONUS_WOLFMAN:	return LC_TEXT("Strong against Lycans +%d%%");
		case POINT_ATTBONUS_MONSTER:	return LC_TEXT("¸ó½ºÅÍ¿¡°Ô °­ÇÔ +%d%%");
		case POINT_MALL_ATTBONUS:		return LC_TEXT("°ø°Ý·Â +%d%%");
		case POINT_MALL_DEFBONUS:		return LC_TEXT("¹æ¾î·Â +%d%%");
		case POINT_MALL_EXPBONUS:		return LC_TEXT("°æÇèÄ¡ %d%%");
		case POINT_MALL_ITEMBONUS:		return LC_TEXT("¾ÆÀÌÅÛ µå·ÓÀ² %.1f¹è");
		case POINT_MALL_GOLDBONUS:		return LC_TEXT("µ· µå·ÓÀ² %.1f¹è");
		case POINT_MAX_HP_PCT:			return LC_TEXT("ÃÖ´ë »ý¸í·Â +%d%%");
		case POINT_MAX_SP_PCT:			return LC_TEXT("ÃÖ´ë Á¤½Å·Â +%d%%");
		case POINT_SKILL_DAMAGE_BONUS:	return LC_TEXT("½ºÅ³ µ¥¹ÌÁö %d%%");
		case POINT_NORMAL_HIT_DAMAGE_BONUS:	return LC_TEXT("ÆòÅ¸ µ¥¹ÌÁö %d%%");
		case POINT_SKILL_DEFEND_BONUS:		return LC_TEXT("½ºÅ³ µ¥¹ÌÁö ÀúÇ× %d%%");
		case POINT_NORMAL_HIT_DEFEND_BONUS:	return LC_TEXT("ÆòÅ¸ µ¥¹ÌÁö ÀúÇ× %d%%");
//		case POINT_PC_BANG_EXP_BONUS:	return LC_TEXT("");
//		case POINT_PC_BANG_DROP_BONUS:	return LC_TEXT("");
//		case POINT_EXTRACT_HP_PCT:	return LC_TEXT("");
		case POINT_RESIST_WARRIOR:	return LC_TEXT("¹«»ç°ø°Ý¿¡ %d%% ÀúÇ×");
		case POINT_RESIST_ASSASSIN:	return LC_TEXT("ÀÚ°´°ø°Ý¿¡ %d%% ÀúÇ×");
		case POINT_RESIST_SURA:		return LC_TEXT("¼ö¶ó°ø°Ý¿¡ %d%% ÀúÇ×");
		case POINT_RESIST_SHAMAN:	return LC_TEXT("¹«´ç°ø°Ý¿¡ %d%% ÀúÇ×");
		case POINT_RESIST_WOLFMAN:	return LC_TEXT("Defense chance against Lycan attacks %d%%");
#ifdef WJ_MAGIC_REDUCION_BONUS
		case POINT_ANTI_RESIST_MAGIC: return LC_TEXT("Anti Magic Resistance: %d%%");
#endif
		default:					return NULL;
	}
}

static bool FN_hair_affect_string(LPCHARACTER ch, char *buf, size_t bufsiz)
{
	if (NULL == ch || NULL == buf)
		return false;

	CAffect* aff = NULL;
	time_t expire = 0;
	struct tm ltm;
	int	year, mon, day;
	int	offset = 0;

	aff = ch->FindAffect(AFFECT_HAIR);

	if (NULL == aff)
		return false;

	expire = ch->GetQuestFlag("hair.limit_time");

	if (expire < get_global_time())
		return false;

	// set apply string
	offset = snprintf(buf, bufsiz, FN_point_string(aff->bApplyOn), aff->lApplyValue);

	if (offset < 0 || offset >= (int) bufsiz)
		offset = bufsiz - 1;

	localtime_r(&expire, &ltm);

	year	= ltm.tm_year + 1900;
	mon		= ltm.tm_mon + 1;
	day		= ltm.tm_mday;

	snprintf(buf + offset, bufsiz - offset, LC_TEXT(" (¸¸·áÀÏ : %d³â %d¿ù %dÀÏ)"), year, mon, day);

	return true;
}

ACMD(do_costume)
{
	#ifdef __SASH_SYSTEM__
	char buf[768];
	#else
	char buf[512];
	#endif
	
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	
	CItem * pBody = ch->GetWear(WEAR_COSTUME_BODY);
	CItem * pHair = ch->GetWear(WEAR_COSTUME_HAIR);
	#ifdef __SASH_SYSTEM__
	CItem * pSash = ch->GetWear(WEAR_COSTUME_SASH);
	#endif
	ch->ChatPacket(CHAT_TYPE_INFO, "COSTUME status:");
	if (pBody)
	{
		const char* itemName = pBody->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  BODY: %s", itemName);
		
		if (pBody->IsEquipped() && arg1[0] == 'b')
			ch->UnequipItem(pBody);
	}
	
	if (pHair)
	{
		const char* itemName = pHair->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  HAIR: %s", itemName);

		for (int i = 0; i < pHair->GetAttributeCount(); ++i)
		{
			const TPlayerItemAttribute& attr = pHair->GetAttribute(i);
			if (attr.bType > 0)
			{
				const char * pAttrName = FN_point_string(attr.bType);
				if (pAttrName == NULL)
					continue;
				
				snprintf(buf, sizeof(buf), FN_point_string(attr.bType), attr.sValue);
				ch->ChatPacket(CHAT_TYPE_INFO, "     %s", buf);
			}
		}

		if (pHair->IsEquipped() && arg1[0] == 'h')
			ch->UnequipItem(pHair);
	}
	
	#ifdef __SASH_SYSTEM__
	if (pSash)
	{
		const char * itemName = pSash->GetName();
		ch->ChatPacket(CHAT_TYPE_INFO, "  SASH: %s", itemName);
		for (int i = 0; i < pSash->GetAttributeCount(); ++i)
		{
			const TPlayerItemAttribute& attr = pSash->GetAttribute(i);
			if (attr.bType > 0)
			{
				const char * pAttrName = FN_point_string(attr.bType);
				if (pAttrName == NULL)
					continue;
				
				snprintf(buf, sizeof(buf), FN_point_string(attr.bType), attr.sValue);
				ch->ChatPacket(CHAT_TYPE_INFO, "     %s", buf);
			}
		}

		if (pSash->IsEquipped() && arg1[0] == 's')
			ch->UnequipItem(pSash);
	}
	#endif
}

ACMD(do_hair)
{
	char buf[256];

	if (false == FN_hair_affect_string(ch, buf, sizeof(buf)))
		return;

	ch->ChatPacket(CHAT_TYPE_INFO, buf);
}

ACMD(do_inventory)
{
	int	index = 0;
	int	count		= 1;

	char arg1[256];
	char arg2[256];

	LPITEM	item;

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Usage: inventory <start_index> <count>");
		return;
	}

	if (!*arg2)
	{
		index = 0;
		str_to_number(count, arg1);
	}
	else
	{
		str_to_number(index, arg1); index = MIN(index, INVENTORY_MAX_NUM);
		str_to_number(count, arg2); count = MIN(count, INVENTORY_MAX_NUM);
	}

	for (int i = 0; i < count; ++i)
	{
		if (index >= INVENTORY_MAX_NUM)
			break;

		item = ch->GetInventoryItem(index);

		ch->ChatPacket(CHAT_TYPE_INFO, "inventory [%d] = %s",
						index, item ? item->GetName() : "<NONE>");
		++index;
	}
}

//gift notify quest command
ACMD(do_gift)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "gift");
}

#ifdef ENABLE_KOSTUMPARLA
ACMD(do_costum_effect)
{
	char arg1[256];
	char arg2[256];
	char arg3[256];
	one_argument(two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2)), arg3, sizeof(arg3));

	if (!*arg1 || !*arg2 || !*arg3)
		return;

	if (!ch || !ch->IsPC())
		return;

	if (ch->IsOpenSafebox() || ch->GetExchange() || ch->GetMyShop() || ch->IsCubeOpen()
#ifdef WJ_SECURITY_SYSTEM
		|| ch->IsActivateSecurity() == true
#endif
#ifdef __SASH_SYSTEM__
		|| ch->IsSashCombinationOpen() || ch->IsSashAbsorptionOpen()
#endif
#ifdef ENABLE_AURA_SYSTEM
		|| ch->isAuraOpened(true) || ch->isAuraOpened(false)
#endif
	)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
		return;
	}

	if (ch->IsDead())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("olu_durumda_olmaz"));
		return;
	}

	DWORD dwVnum = 0;
	str_to_number(dwVnum, arg1);

	if (dwVnum < 0 || dwVnum >= INVENTORY_MAX_NUM)
		return;

	DWORD dwVnumSlot = 0;
	str_to_number(dwVnumSlot, arg3);

	if (dwVnumSlot < 0 || dwVnumSlot >= INVENTORY_MAX_NUM)
		return;

	int BStat = 0;
	str_to_number(BStat, arg2);

	if (BStat <= 0 || BStat > 7)
		return;

	LPITEM item2 = ch->GetInventoryItem(dwVnumSlot);

	if (!item2 || item2->IsExchanging() || item2->GetVnum() != 49440)
		return;

	LPITEM item = ch->GetInventoryItem(dwVnum);

	if (!item || item->IsExchanging() || item->IsEquipped())
		return;

	if (item->GetType() != ITEM_COSTUME || item->GetSubType() != COSTUME_BODY)
		return;

	item->SetSocket(1, BStat);
	item2->SetCount(item->GetCount() - 1);
}
#endif

#ifdef _ITEM_SHOP_SYSTEM
ACMD(do_nesne_market)
{
	if (!ch)
		return;

	if (!ch->GetDesc())
		return;

	if (!ch->IsPC())
		return;

	if (ch->IsStun())
		return;

	if (ch->IsHack())
		return;

	if (ch->IsOpenSafebox() || ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsCubeOpen()
#ifdef WJ_SECURITY_SYSTEM
		|| ch->IsActivateSecurity() == true
#endif
#ifdef __SASH_SYSTEM__
		|| ch->IsSashCombinationOpen() || ch->IsSashAbsorptionOpen()
#endif
#ifdef ENABLE_AURA_SYSTEM
		|| ch->isAuraOpened(true) || ch->isAuraOpened(false)
#endif
	)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
		return;
	}

	if (ch->IsDead())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("olu_durumda_olmaz"));
		return;
	}

	if (quest::CQuestManager::instance().GetPCForce(ch->GetPlayerID())->IsRunning() == true)
		return;

	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	DWORD id = 0;
	DWORD count = 0;

	if (!*arg1 || !*arg2)
		return;

	str_to_number(id, arg1);
	str_to_number(count, arg2);

	bool bRes = CItemShopManager::instance().Buy(ch, id, count); // buy func
	if (bRes)
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("nesnemarketbasarili"));
}
#endif

#ifdef ENABLE_NEW_PET_SYSTEM
ACMD(do_CubePetAdd) {
	int pos = 0;
	int invpos = 0;

	const char* line;
	char arg1[256], arg2[256], arg3[256];

	line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	one_argument(line, arg3, sizeof(arg3));

	if (0 == arg1[0])
		return;
	//const std::string & strArg1 = std::string(arg1);
	switch (LOWER(arg1[0]))
	{
	case 'a':	// add cue_index inven_index
	{
					if (0 == arg2[0] || !isdigit(*arg2) ||
						0 == arg3[0] || !isdigit(*arg3))
						return;

					str_to_number(pos, arg2);
					str_to_number(invpos, arg3);
	}
		break;

	default:
		return;
	}

	if (ch->GetNewPetSystem()->IsActivePet())
		ch->GetNewPetSystem()->SetItemCube(pos, invpos);
	else
		return;
}

ACMD(do_PetSkill) {
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;

	DWORD skillslot = 0;
	str_to_number(skillslot, arg1);
	if (skillslot > 2 || skillslot < 0)
		return;

	if (ch->GetNewPetSystem()->IsActivePet())
		ch->GetNewPetSystem()->DoPetSkill(skillslot);
	else
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Devam etmek icin petini cagir.!"));
}

ACMD(do_FeedCubePet) {
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	if (!*arg1)
		return;

	DWORD feedtype = 0;
	str_to_number(feedtype, arg1);
	if (ch->GetNewPetSystem()->IsActivePet())
		ch->GetNewPetSystem()->ItemCubeFeed(feedtype);
	else
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Once petini cagir!"));
}

ACMD(do_PetEvo) {
	if (ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen()) {
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Pet Gelistirme] Pencere Acik."));
		return;
	}
	if (ch->GetNewPetSystem()->IsActivePet()) {
		int it[3][1] = {
			{ 55003 }, //Here Modify Items to request for 1 evo
			{ 55004 }, //Here Modify Items to request for 2 evo
			{ 55005 }  //Here Modify Items to request for 3 evo
		};
		int ic[3][1] = { { 20 },
		{ 30},
		{ 40}
		};
		int tmpevo = ch->GetNewPetSystem()->GetEvolution();

		if ((ch->GetNewPetSystem()->GetLevel() == 40 && tmpevo == 0) ||
			(ch->GetNewPetSystem()->GetLevel() == 60 && tmpevo == 1) ||
			(ch->GetNewPetSystem()->GetLevel() == 80 && tmpevo == 2)) {
			for (int b = 0; b < 1; b++) {
				if (ch->CountSpecifyItem(it[tmpevo][b]) < ic[tmpevo][b]) {
					ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Pet Gelistirme] Gereken itemler:"));
					for (int c = 0; c < 1; c++) {
						DWORD vnum = it[tmpevo][c];
						ch->ChatPacket(CHAT_TYPE_INFO, "%s X%d", ITEM_MANAGER::instance().GetTable(vnum)->szLocaleName, ic[tmpevo][c]);
					}
					return;
				}
			}
			for (int c = 0; c < 1; c++) {
				ch->RemoveSpecifyItem(it[tmpevo][c], ic[tmpevo][c]);
			}
			ch->GetNewPetSystem()->IncreasePetEvolution();
		}
		else {
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Suan evcil hayvanin gelistirilemez!"));
			return;
		}
	}
	else
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Zaten evcil hayvanin var!"));
}

ACMD(do_PetChangeName)
{
	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1 || !*arg2)
		return;

	int bCell = 0;
	str_to_number(bCell, arg1);
	LPITEM item = ch->GetInventoryItem(bCell);
	if (!item)
		return;
	if (ch->CountSpecifyItem(55008) < 1)
		return;

	if (!strcmp(arg2, "%") ||
		!strcmp(arg2, "/") ||
		!strcmp(arg2, ">") ||
		!strcmp(arg2, "|") ||
		!strcmp(arg2, ";") ||
		!strcmp(arg2, ":") ||
		!strcmp(arg2, "}") ||
		!strcmp(arg2, "{") ||
		!strcmp(arg2, "[") ||
		!strcmp(arg2, "]") ||
		!strcmp(arg2, "%") ||
		!strcmp(arg2, "#") ||
		!strcmp(arg2, "@") ||
		!strcmp(arg2, "^") ||
		!strcmp(arg2, "&") ||
		!strcmp(arg2, "'")
		)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Pet-Kulucka] Hatali isim girdiniz"));
		return;
	}

	if (ch->GetNewPetSystem()->IsActivePet())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Once petini gonder. "));
		return;
	}

	char szName[CHARACTER_NAME_MAX_LEN + 1];
	DBManager::instance().EscapeString(szName, sizeof(szName), arg2, strlen(arg2));

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET name = '%s' WHERE id = '%lu'", szName, item->GetID()));
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Pet Ismi Basarili Bir Sekilde Degistirildi!"));
	ch->RemoveSpecifyItem(55008, 1);
}

ACMD(do_IncreasePetSkill)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	int itempos = atoi(arg1);
	LPITEM item = ch->GetInventoryItem(itempos);
	if (!item)
		return;
	int skill = item->GetValue(0);
	CNewPetSystem* petSystem = ch->GetNewPetSystem();
	if (!petSystem)
		return;
	if (item->GetVnum() < 55009 && item->GetVnum() > 55039)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_UNKNOWN_SKILL"));
		return;
	}
	bool ret = petSystem->IncreasePetSkill(skill);
	if (ret)
		item->SetCount(item->GetCount() - 1);
}

ACMD(do_ResetPetSkill)
{
	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	if (!*arg1 || !*arg2)
		return;

	int itempos = atoi(arg1);
	int slotpos = atoi(arg2);

	LPITEM item = ch->GetInventoryItem(itempos);
	if (!item)
		return;

	CNewPetSystem* petSystem = ch->GetNewPetSystem();
	if (!petSystem)
		return;

	if (item->GetVnum() != 55030)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_UNKNOWN_SKILL"));
		return;
	}

	bool ret = petSystem->ResetSkill(slotpos);
	if (ret)
		item->SetCount(item->GetCount() - 1);
}

#ifdef ENABLE_PET_ATTR_DETERMINE
ACMD(do_determine_pet)
{
	if (quest::CQuestManager::instance().GetEventFlag("pet_kapat") == 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_SYSTEM_DISABLED"));
		return;
	}

	if (ch->CountSpecifyItem(55032) < 1)
		return;

	int iFloodDetermine = ch->GetQuestFlag("pet_system.determine_pet");
	if (iFloodDetermine)
	{
		if (get_global_time() < iFloodDetermine + 2)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Cok hizlisin 2 saniye bekle.");
			return;
		}
	}

	/*if (ch->GetNewPetSystem()->IsActivePet())
	{
		int newpettype = number(0, 7);
		std::unique_ptr<SQLMsg> (DBManager::instance().DirectQuery("UPDATE new_petsystem SET pet_type = '%d' WHERE id = '%lu'", newpettype, ch->GetNewPetSystem()->GetNewPetITemID()));
		ch->GetNewPetSystem()->SetPetType(newpettype);
		ch->ChatPacket(CHAT_TYPE_COMMAND, "OnResultPetAttrDetermine %d", ch->GetNewPetSystem()->GetPetType());
		ch->SetQuestFlag("pet_system.determine_pet", get_global_time() + 6);
		ch->RemoveSpecifyItem(55032, 1);
	}*/

	if (ch->GetNewPetSystem()->IsActivePet())
	{
		DWORD dwArtis1 = 0;
		DWORD dwArtis2 = 0;
		DWORD dwArtis3 = 0;
		int newpettype = number(0, 7);
		//int newpettype = ch->GetNewPetSystem()->GetPetType();
		if (newpettype == 0)
		{
			dwArtis1 = number(1, 7);
			dwArtis2 = number(1, 7);
			dwArtis3 = number(1, 7);
		}
		else if (newpettype == 1)
		{
			dwArtis1 = number(1, 2);
			dwArtis2 = number(1, 2);
			dwArtis3 = number(1, 2);
		}
		else if (newpettype == 2)
		{
			dwArtis1 = number(2, 3);
			dwArtis2 = number(2, 3);
			dwArtis3 = number(2, 3);
		}
		else if (newpettype == 3)
		{
			dwArtis1 = number(3, 4);
			dwArtis2 = number(3, 4);
			dwArtis3 = number(3, 4);
		}
		else if (newpettype == 4)
		{
			dwArtis1 = number(3, 5);
			dwArtis2 = number(3, 5);
			dwArtis3 = number(3, 5);
		}
		else if (newpettype == 5)
		{
			dwArtis1 = number(4, 6);
			dwArtis2 = number(4, 6);
			dwArtis3 = number(4, 6);
		}
		else if (newpettype == 6)
		{
			dwArtis1 = number(5, 7);
			dwArtis2 = number(5, 7);
			dwArtis3 = number(5, 7);
		}
		else if (newpettype == 7)
		{
			dwArtis1 = number(5, 8);
			dwArtis2 = number(5, 8);
			dwArtis3 = number(5, 8);
		}

		std::unique_ptr<SQLMsg>(DBManager::instance().DirectQuery("UPDATE new_petsystem SET pet_type = '%d', artis1 = '%d', artis2 = '%d', artis3 = '%d' WHERE id = '%lu'", newpettype, dwArtis1, dwArtis2, dwArtis3, ch->GetNewPetSystem()->GetNewPetITemID()));
		ch->GetNewPetSystem()->SetPetType(newpettype);

		// ch->GetNewPetSystem()->SetArtis(1, dwArtis1);
		// ch->GetNewPetSystem()->SetArtis(2, dwArtis2);
		// ch->GetNewPetSystem()->SetArtis(3, dwArtis3);
		ch->ChatPacket(CHAT_TYPE_COMMAND, "OnResultPetAttrDetermine %d", ch->GetNewPetSystem()->GetPetType());
		ch->SetQuestFlag("pet_system.determine_pet", get_global_time() + 6);
		ch->RemoveSpecifyItem(55032, 1);
	}
}

ACMD(do_change_pet)
{
	if (quest::CQuestManager::instance().GetEventFlag("pet_kapat") == 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("PET_SYSTEM_DISABLED"));
		return;
	}
	char arg1[256], arg2[256], arg3[256], arg4[256];
	four_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2), arg3, sizeof(arg3), arg4, sizeof(arg4));

	if (!*arg1 || !*arg2 || !*arg3 || !*arg4)
		return;

	int firstInvenType = atoi(arg1);
	int firstInvenPos = atoi(arg2);
	int secondInvenType = atoi(arg3);
	int secondInvenPos = atoi(arg4);

	if (firstInvenType < 0)
		return;

	if (secondInvenType <0)
		return;

	if (firstInvenType == INVENTORY)
	{
		if (firstInvenPos >= INVENTORY_MAX_NUM)
			return;
	}
	else
	{
		if (firstInvenPos >= 225)
			return;
	}

	if (secondInvenType == INVENTORY)
	{
		if (secondInvenPos >= INVENTORY_MAX_NUM)
			return;
	}
	else
	{
		if (secondInvenPos >= 225)
			return;
	}

	LPITEM petItem = ch->GetItem(TItemPos(firstInvenType, firstInvenPos));
	LPITEM secondItem = ch->GetItem(TItemPos(secondInvenType, secondInvenPos));

	if (!petItem)
		return;

	if (!secondItem)
		return;

	if (secondItem->GetVnum() != 55033)
		return;

	int	m_dwbonuspet[3][2];

	for (int x = 0; x < 3; ++x) //Inizializzazione bonus del pet
	{
		int btype[3] = { 1, 54, 53 };
		m_dwbonuspet[x][0] = btype[x];
		m_dwbonuspet[x][1] = 0;
	}

	int newpettype = number(0, 7);
	int level = petItem->GetSocket(1);
	int age = petItem->GetSocket(3);
	int tmpage = (time(0) + age) / 86400;

	if (newpettype == 0)
	{
		m_dwbonuspet[0][1] = number(1, 30);
		m_dwbonuspet[1][1] = number(1, 30);
		m_dwbonuspet[2][1] = number(1, 30);;
		for (int i = 0; i < level; ++i)
		{
			int lvbonus = 0;
			if (level >= 60)
				lvbonus = 1;
			if (i % 6 == 0)
			{
				m_dwbonuspet[0][1] += number(1 + lvbonus, 5 + lvbonus);
				m_dwbonuspet[2][1] += number(1 + lvbonus, 5 + lvbonus);
			}
			if (i % 8 == 0)
			{
				m_dwbonuspet[1][1] += number(1 + lvbonus, 5 + lvbonus);
			}
		}
	}
	else if (newpettype == 1) // sabit
	{
		m_dwbonuspet[0][1] = number(1, 30);
		m_dwbonuspet[1][1] = number(1, 30);
		m_dwbonuspet[2][1] = number(1, 30);
		for (int i = 0; i < level; ++i)
		{
			int lvbonus = 0;
			if (tmpage >= 60)
				lvbonus = 1;
			if (i % 5 == 0)
			{
				m_dwbonuspet[0][1] += number(2 + lvbonus, 3 + lvbonus);
				m_dwbonuspet[2][1] += number(2 + lvbonus, 3 + lvbonus);
			}
			if (i % 7 == 0)
			{
				m_dwbonuspet[1][1] += number(2 + lvbonus, 3 + lvbonus);
			}
		}
	}
	else if (newpettype == 2)
	{
		m_dwbonuspet[0][1] = number(1, 30);
		m_dwbonuspet[1][1] = number(1, 30);
		m_dwbonuspet[2][1] = number(1, 30);
		for (int i = 0; i < level; ++i)
		{
			int lvbonus = 0;
			if (level >= 60)
				lvbonus = 1;
			if (i % 6 == 0)
			{
				m_dwbonuspet[0][1] += number(1 + lvbonus, 7 + lvbonus);
				m_dwbonuspet[2][1] += number(1 + lvbonus, 7 + lvbonus);
			}
			if (i % 8 == 0)
			{
				m_dwbonuspet[1][1] += number(1 + lvbonus, 7 + lvbonus);
			}
		}
	}
	else if (newpettype == 3) // sabit
	{
		m_dwbonuspet[0][1] = number(1, 30);
		m_dwbonuspet[1][1] = number(1, 30);
		m_dwbonuspet[2][1] = number(1, 30);
		for (int i = 0; i < level; ++i)
		{
			int lvbonus = 0;
			if (tmpage >= 60)
				lvbonus = 1;
			if (i % 5 == 0)
			{
				m_dwbonuspet[0][1] += number(3 + lvbonus, 4 + lvbonus);
				m_dwbonuspet[2][1] += number(3 + lvbonus, 4 + lvbonus);
			}
			if (i % 7 == 0)
			{
				m_dwbonuspet[1][1] += number(3 + lvbonus, 4 + lvbonus);
			}
		}
	}
	else if (newpettype == 4)
	{
		m_dwbonuspet[0][1] = number(1, 30);
		m_dwbonuspet[1][1] = number(1, 30);
		m_dwbonuspet[2][1] = number(1, 30);
		for (int i = 0; i < level; ++i)
		{
			int lvbonus = 0;
			if (level >= 60)
				lvbonus = 1;
			if (i % 6 == 0)
			{
				m_dwbonuspet[0][1] += number(1 + lvbonus, 9 + lvbonus);
				m_dwbonuspet[2][1] += number(1 + lvbonus, 9 + lvbonus);
			}
			if (i % 8 == 0)
			{
				m_dwbonuspet[1][1] += number(1 + lvbonus, 9 + lvbonus);
			}
		}
	}
	else if (newpettype == 5) // sabit
	{
		m_dwbonuspet[0][1] = number(1, 30);
		m_dwbonuspet[1][1] = number(1, 30);
		m_dwbonuspet[2][1] = number(1, 30);
		for (int i = 0; i < level; ++i)
		{
			int lvbonus = 0;
			if (tmpage >= 60)
				lvbonus = 1;
			if (i % 5 == 0)
			{
				m_dwbonuspet[0][1] += number(5 + lvbonus, 6 + lvbonus);
				m_dwbonuspet[2][1] += number(5 + lvbonus, 6 + lvbonus);
			}
			if (i % 7 == 0)
			{
				m_dwbonuspet[1][1] += number(5 + lvbonus, 6 + lvbonus);
			}
		}
	}
	else if (newpettype == 6)
	{
		m_dwbonuspet[0][1] = number(1, 30);
		m_dwbonuspet[1][1] = number(1, 30);
		m_dwbonuspet[2][1] = number(1, 30);
		for (int i = 0; i < level; ++i)
		{
			int lvbonus = 0;
			if (level >= 60)
				lvbonus = 1;
			if (i % 6 == 0)
			{
				m_dwbonuspet[0][1] += number(1 + lvbonus, 10 + lvbonus);
				m_dwbonuspet[2][1] += number(1 + lvbonus, 10 + lvbonus);
			}
			if (i % 8 == 0)
			{
				m_dwbonuspet[1][1] += number(1 + lvbonus, 10 + lvbonus);
			}
		}
	}
	else if (newpettype == 7)
	{
		m_dwbonuspet[0][1] = number(1, 30);
		m_dwbonuspet[1][1] = number(1, 30);
		m_dwbonuspet[2][1] = number(1, 30);
		for (int i = 0; i < level; ++i)
		{
			int lvbonus = 0;
			if (tmpage >= 60)
				lvbonus = 1;
			if (i % 5 == 0)
			{
				m_dwbonuspet[0][1] += number(5 + lvbonus, 7 + lvbonus);
				m_dwbonuspet[2][1] += number(5 + lvbonus, 7 + lvbonus);
			}
			if (i % 7 == 0)
			{
				m_dwbonuspet[1][1] += number(5 + lvbonus, 7 + lvbonus);
			}
		}
	}

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("UPDATE new_petsystem SET bonus0 = '%d', bonus1 = '%d', bonus2 = '%d' WHERE id = '%lu'", m_dwbonuspet[0][1], m_dwbonuspet[1][1], m_dwbonuspet[2][1], petItem->GetID()));
	for (int b = 0; b < 3; b++) {
		petItem->SetForceAttribute(b, 1, m_dwbonuspet[b][1]);
	}

	secondItem->SetCount(secondItem->GetCount() - 1);
	ch->ChatPacket(CHAT_TYPE_COMMAND, "OnResultPetAttrChange");
}
#endif
#endif

ACMD(do_satilmayanlar)
{
	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() ||ch->GetOfflineShop() || ch->GetOfflineShopOwner() || ch->IsCubeOpen() || ch->IsOpenSafebox() || ch->GetShop())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("engellenen fonksiyonlardan birisi calisiyor"));
		return;
	}
	ch->SatilmayanlariGeriAl(ch);
}

ACMD(do_open_offline_shop)
{
	// if (ch->IsFight())
	// {
		// ch->ChatPacket(CHAT_TYPE_INFO, "duello_sirasinda_pazar_kuramassin.");
		// return;
	// }
	// If character is dead, return false
	if (ch->IsDead())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Dead man can not open offline shop"));
		return;
	}


	
	if (ch->IsSashCombinationOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("kombinasyon_penceresi_acik"));
		return;
	}
	
	if (ch->IsSashAbsorptionOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bonus_emis_penceresi_acik"));
		return;
	}
	
	// If character is exchanging with someone, return false
	if (ch->GetExchange())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You are exchanging with someone. At first close to the window!"));
		return;
	}

	// If character has a private shop, return false
	if (ch->GetMyShop())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You had open private shop. At first you must be close this private shop!"));
		return;
	}

	// If character is look at one offline shop, return false
	if (ch->GetOfflineShop())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't open offline shop because you are already look at one offline shop."));
		return;
	}

	// If cube window is open, return false
	if (ch->IsCubeOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't open offline shop because your cube window is open"));
		return;
	}

	// If character's safebox is open, return false
	if (ch->IsOpenSafebox())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't open offline shop because your safebox window is open!"));
		return;
	}

	// If character's shop window is open, return false
	if (ch->GetShop())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't open offline shop because your shop window is open!"));
		return;
	}

	if (ch->GetOfflineShopCount() > g_maxOfflineShopCount && !ch->FindAffect(AFFECT_SHOP_DECO))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bu haritada pazar sinirina ulasildi."));
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("2. koy veya baska bir kanalda kurabilirsiiniz."));
		return;
	}

	// Send the command to client.
	ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenOfflineShop");
}
//Shop Decoration
ACMD(do_open_decoration){
	if (!ch->FindAffect(AFFECT_SHOP_DECO))
	{
		ch ->ChatPacket(CHAT_TYPE_INFO,"Dekorasyon setini açmadan bu özelliði kullanamazsýn.");
		return;
	}
	ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenDecoration");
}
ACMD(do_shop_decoration){
	const char *var;
	char arg1[255],arg2[255];
	var = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	int arg1_1 = atoi(arg1);
	int arg2_1 = atoi(arg2);

	if (!ch->FindAffect(AFFECT_SHOP_DECO))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Error");
		return;
	}

	{
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT COUNT(*) FROM player.offline_shop_npc WHERE owner_id = %u", ch->GetPlayerID()));
		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);

		BYTE bResult = 0;
		str_to_number(bResult, row[0]);

		if (bResult)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Error.");
			return;
		}
	}
	if ((arg1_1 >= 30000 && arg1_1 <= 30007)||(arg2_1 >= 0 && arg2_1 <= 5)){
		ch->OfflineDecoration(arg1_1);
		ch->OfflineDecoration1(arg2_1);
	}
	else{
		ch->ChatPacket(CHAT_TYPE_INFO, "Error.");
		return;
	}	
}
//Shop Decoration
#ifdef ENABLE_CUBE_RENEWAL
ACMD(do_cube)
{
	const char* line;
	char arg1[256], arg2[256], arg3[256];
	line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	one_argument(line, arg3, sizeof(arg3));

	if (0 == arg1[0])
	{
		return;
	}

	switch (LOWER(arg1[0]))
	{
	case 'o':	// open
		Cube_open(ch);
		break;

	default:
		return;
	}
}
#else
ACMD(do_cube)
{
	if (!ch->CanDoCube())
		return;

	dev_log(LOG_DEB0, "CUBE COMMAND <%s>: %s", ch->GetName(), argument);
	int cube_index = 0, inven_index = 0;

	#ifdef ENABLE_SPECIAL_STORAGE
	int inven_type = 0;

	char arg1[256], arg2[256], arg3[256], arg4[256];
	two_arguments (two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2)), arg3, sizeof(arg3), arg4, sizeof(arg4));
	#else
	char arg1[256], arg2[256], arg3[256];
	const char *line;

	line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	one_argument(line, arg3, sizeof(arg3));
	#endif

	if (0 == arg1[0])
	{
		// print usage
		ch->ChatPacket(CHAT_TYPE_INFO, "       Usage: cube open");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube close");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube add <inveltory_index>");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube delete <cube_index>");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube list");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube cancel");
		ch->ChatPacket(CHAT_TYPE_INFO, "       cube make [all]");
		return;
	}

	const std::string& strArg1 = std::string(arg1);

	// r_info (request information)
	// /cube r_info     ==> (Client -> Server) ÇöÀç NPC°¡ ¸¸µé ¼ö ÀÖ´Â ·¹½ÃÇÇ ¿äÃ»
	//					    (Server -> Client) /cube r_list npcVNUM resultCOUNT 123,1/125,1/128,1/130,5
	//
	// /cube r_info 3   ==> (Client -> Server) ÇöÀç NPC°¡ ¸¸µé¼ö ÀÖ´Â ·¹½ÃÇÇ Áß 3¹øÂ° ¾ÆÀÌÅÛÀ» ¸¸µå´Â µ¥ ÇÊ¿äÇÑ Á¤º¸¸¦ ¿äÃ»
	// /cube r_info 3 5 ==> (Client -> Server) ÇöÀç NPC°¡ ¸¸µé¼ö ÀÖ´Â ·¹½ÃÇÇ Áß 3¹øÂ° ¾ÆÀÌÅÛºÎÅÍ ÀÌÈÄ 5°³ÀÇ ¾ÆÀÌÅÛÀ» ¸¸µå´Â µ¥ ÇÊ¿äÇÑ Àç·á Á¤º¸¸¦ ¿äÃ»
	//					   (Server -> Client) /cube m_info startIndex count 125,1|126,2|127,2|123,5&555,5&555,4/120000@125,1|126,2|127,2|123,5&555,5&555,4/120000
	//
	if (strArg1 == "r_info")
	{
		if (0 == arg2[0])
			Cube_request_result_list(ch);
		else
		{
			if (isdigit(*arg2))
			{
				int listIndex = 0, requestCount = 1;
				str_to_number(listIndex, arg2);

				if (0 != arg3[0] && isdigit(*arg3))
					str_to_number(requestCount, arg3);

				Cube_request_material_info(ch, listIndex, requestCount);
			}
		}

		return;
	}

	switch (LOWER(arg1[0]))
	{
		case 'o':	// open
			Cube_open(ch);
			break;

		case 'c':	// close
			Cube_close(ch);
			break;

		case 'l':	// list
			Cube_show_list(ch);
			break;

		case 'a':	// add cue_index inven_index
			{
				#ifdef ENABLE_SPECIAL_STORAGE
				if (0 == arg2[0] || !isdigit(*arg2) || 0 == arg3[0] || !isdigit(*arg3) || 0 == arg4[0] || !isdigit(*arg4))
				#else
				if (0 == arg2[0] || !isdigit(*arg2) || 0 == arg3[0] || !isdigit(*arg3))
				#endif
					return;

				str_to_number(cube_index, arg2);
				str_to_number(inven_index, arg3);
				#ifdef ENABLE_SPECIAL_STORAGE
				str_to_number(inven_type, arg4);
				Cube_add_item (ch, cube_index, inven_index, inven_type);
				#else
				Cube_add_item (ch, cube_index, inven_index);
				#endif
			}
			break;

		case 'd':	// delete
			{
				if (0 == arg2[0] || !isdigit(*arg2))
					return;

				str_to_number(cube_index, arg2);
				Cube_delete_item (ch, cube_index);
			}
			break;

		case 'm':	// make
			if (0 != arg2[0])
			{
				while (true == Cube_make(ch))
					dev_log (LOG_DEB0, "cube make success");
			}
			else
				Cube_make(ch);
			break;

		default:
			return;
	}
}
#endif
ACMD(do_cards)
{
    const char *line;

    char arg1[256], arg2[256];

    line = two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
    switch (LOWER(arg1[0]))
    {
        case 'o':    // open
            if (isdigit(*arg2))
            {
                DWORD safemode;
                str_to_number(safemode, arg2);
                ch->Cards_open(safemode);
            }
            break;
        case 'p':    // open
            ch->Cards_pullout();
            break;
        case 'e':    // open
            ch->CardsEnd();
            break;
        case 'd':    // open
            if (isdigit(*arg2))
            {
                DWORD destroy_index;
                str_to_number(destroy_index, arg2);
                ch->CardsDestroy(destroy_index);
            }
            break;
        case 'a':    // open
            if (isdigit(*arg2))
            {
                DWORD accpet_index;
                str_to_number(accpet_index, arg2);
                ch->CardsAccept(accpet_index);
            }
            break;
        case 'r':    // open
            if (isdigit(*arg2))
            {
                DWORD restore_index;
                str_to_number(restore_index, arg2);
                ch->CardsRestore(restore_index);
            }
            break;
        default:
            return;
    }
}
ACMD(do_in_game_mall)
{
	if (LC_IsYMIR() == true || LC_IsKorea() == true)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "mall http://metin2.co.kr/04_mall/mall/login.htm");
		return;
	}

	if (true == LC_IsTaiwan())
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "mall http://203.69.141.203/mall/mall/item_main.htm");
		return;
	}

	// ¤Ð_¤Ð Äèµµ¼­¹ö ¾ÆÀÌÅÛ¸ô URL ÇÏµåÄÚµù Ãß°¡
	if (true == LC_IsWE_Korea())
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "mall http://metin2.co.kr/50_we_mall/mall/login.htm");
		return;
	}

	if (LC_IsJapan() == true)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "mall http://mt2.oge.jp/itemmall/itemList.php");
		return;
	}
	
	if (LC_IsNewCIBN() == true && test_server)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "mall http://218.99.6.51/04_mall/mall/login.htm");
		return;
	}

	if (LC_IsSingapore() == true)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "mall http://www.metin2.sg/ishop.php");
		return;
	}	
	
	/*
	if (LC_IsCanada() == true)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "mall http://mall.z8games.com/mall_entry.aspx?tb=m2");
		return;
	}*/

	if (LC_IsEurope() == true)
	{
		char country_code[3];

		switch (LC_GetLocalType())
		{
			case LC_GERMANY:	country_code[0] = 'd'; country_code[1] = 'e'; country_code[2] = '\0'; break;
			case LC_FRANCE:		country_code[0] = 'f'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_ITALY:		country_code[0] = 'i'; country_code[1] = 't'; country_code[2] = '\0'; break;
			case LC_SPAIN:		country_code[0] = 'e'; country_code[1] = 's'; country_code[2] = '\0'; break;
			case LC_UK:			country_code[0] = 'e'; country_code[1] = 'n'; country_code[2] = '\0'; break;
			case LC_TURKEY:		country_code[0] = 't'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_POLAND:		country_code[0] = 'p'; country_code[1] = 'l'; country_code[2] = '\0'; break;
			case LC_PORTUGAL:	country_code[0] = 'p'; country_code[1] = 't'; country_code[2] = '\0'; break;
			case LC_GREEK:		country_code[0] = 'g'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_RUSSIA:		country_code[0] = 'r'; country_code[1] = 'u'; country_code[2] = '\0'; break;
			case LC_DENMARK:	country_code[0] = 'd'; country_code[1] = 'k'; country_code[2] = '\0'; break;
			case LC_BULGARIA:	country_code[0] = 'b'; country_code[1] = 'g'; country_code[2] = '\0'; break;
			case LC_CROATIA:	country_code[0] = 'h'; country_code[1] = 'r'; country_code[2] = '\0'; break;
			case LC_MEXICO:		country_code[0] = 'm'; country_code[1] = 'x'; country_code[2] = '\0'; break;
			case LC_ARABIA:		country_code[0] = 'a'; country_code[1] = 'e'; country_code[2] = '\0'; break;
			case LC_CZECH:		country_code[0] = 'c'; country_code[1] = 'z'; country_code[2] = '\0'; break;
			case LC_ROMANIA:	country_code[0] = 'r'; country_code[1] = 'o'; country_code[2] = '\0'; break;
			case LC_HUNGARY:	country_code[0] = 'h'; country_code[1] = 'u'; country_code[2] = '\0'; break;
			case LC_NETHERLANDS: country_code[0] = 'n'; country_code[1] = 'l'; country_code[2] = '\0'; break;
			case LC_USA:		country_code[0] = 'u'; country_code[1] = 's'; country_code[2] = '\0'; break;
			case LC_CANADA:	country_code[0] = 'c'; country_code[1] = 'a'; country_code[2] = '\0'; break;
			default:
				if (test_server == true)
				{
					country_code[0] = 'd'; country_code[1] = 'e'; country_code[2] = '\0';
				}
				break;
		}

		char buf[512+1];
		char sas[33];
		MD5_CTX ctx;
		const char sas_key[] = "GF9001";

		snprintf(buf, sizeof(buf), "%u%u%s", ch->GetPlayerID(), ch->GetAID(), sas_key);

		MD5Init(&ctx);
		MD5Update(&ctx, (const unsigned char *) buf, strlen(buf));
#ifdef __FreeBSD__
		MD5End(&ctx, sas);
#else
		static const char hex[] = "0123456789abcdef";
		unsigned char digest[16];
		MD5Final(digest, &ctx);
		int i;
		for (i = 0; i < 16; ++i) {
			sas[i+i] = hex[digest[i] >> 4];
			sas[i+i+1] = hex[digest[i] & 0x0f];
		}
		sas[i+i] = '\0';
#endif

		snprintf(buf, sizeof(buf), "mall http://%s/ishop?pid=%u&c=%s&sid=%d&sas=%s",
				g_strWebMallURL.c_str(), ch->GetPlayerID(), country_code, g_server_id, sas);

		ch->ChatPacket(CHAT_TYPE_COMMAND, buf);
	}
}

// ÁÖ»çÀ§
ACMD(do_dice)
{
	char arg1[256], arg2[256];
	int start = 1, end = 100;

	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (*arg1 && *arg2)
	{
		start = atoi(arg1);
		end = atoi(arg2);
	}
	else if (*arg1 && !*arg2)
	{
		start = 1;
		end = atoi(arg1);
	}

	end = MAX(start, end);
	start = MIN(start, end);

	int n = number(start, end);

	if (ch->GetParty())
		ch->GetParty()->ChatPacketToAllMember(CHAT_TYPE_DICE_INFO, LC_TEXT("%s´ÔÀÌ ÁÖ»çÀ§¸¦ ±¼·Á %d°¡ ³ª¿Ô½À´Ï´Ù. (%d-%d)"), ch->GetName(), n, start, end);
	else
		ch->ChatPacket(CHAT_TYPE_DICE_INFO, LC_TEXT("´ç½ÅÀÌ ÁÖ»çÀ§¸¦ ±¼·Á %d°¡ ³ª¿Ô½À´Ï´Ù. (%d-%d)"), n, start, end);
}

ACMD(do_click_mall)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowMeMallPassword");
}

ACMD(do_ride)
{
	dev_log(LOG_DEB0, "[DO_RIDE] start");
	if (ch->IsDead() || ch->IsStun())
	return;

	{
	if (ch->IsHorseRiding())
	{
		dev_log(LOG_DEB0, "[DO_RIDE] stop riding");
		ch->StopRiding(); 
		return;
	}

	if (ch->GetMountVnum())
	{
		dev_log(LOG_DEB0, "[DO_RIDE] unmount");
		do_unmount(ch, NULL, 0, 0);
		return;
	}
    }

	{
	if (ch->GetHorse() != NULL)
	{
		dev_log(LOG_DEB0, "[DO_RIDE] start riding");
		ch->StartRiding();
		return;
	}

	for (BYTE i=0; i<INVENTORY_MAX_NUM; ++i)
	{
		LPITEM item = ch->GetInventoryItem(i);
		if (NULL == item)
		continue;

		if (item->IsRideItem())
		{
			if (NULL==ch->GetWear(WEAR_UNIQUE1) || NULL==ch->GetWear(WEAR_UNIQUE2) || NULL==ch->GetWear(WEAR_COSTUME_MOUNT))
			{
				dev_log(LOG_DEB0, "[DO_RIDE] USE UNIQUE ITEM");
				ch->UseItem(TItemPos (INVENTORY, i));
				return;
			}
		}

		switch (item->GetVnum())
		{
		case 71114:
		case 71116:
		case 71118:
		case 71120:
			dev_log(LOG_DEB0, "[DO_RIDE] USE QUEST ITEM");
			ch->UseItem(TItemPos (INVENTORY, i));
			return;
		}

		if( (item->GetVnum() > 52000) && (item->GetVnum() < 52091) )	{
			dev_log(LOG_DEB0, "[DO_RIDE] USE QUEST ITEM");
			ch->UseItem(TItemPos (INVENTORY, i));
			return;
		}
	}
	}

	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("?? ?? ??????."));
}
ACMD(do_i_ver1996){
    BYTE job = ch->GetJob();
    LPITEM item;
    for (int i = 0; i < 6; i++)
    {
        item = ch->GetWear(i);
        if (item != NULL)
            ch->UnequipItem(item);
    }
    item = ch->GetWear(WEAR_SHIELD);
    if (item != NULL)
        ch->UnequipItem(item);


    switch (job)
    {
    case JOB_SURA:
        {
      
            item = ITEM_MANAGER::instance().CreateItem(22369);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(11679);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(12529);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(13049);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(14209 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(15209 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(16209 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(17109 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(18019 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
        }
        break;
    case JOB_WARRIOR:
        {
      
            item = ITEM_MANAGER::instance().CreateItem(22379);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(11279);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(12249);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(13049);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(14209 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(15209 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(16209 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(17109 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(18019 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
        }
        break;
    case JOB_SHAMAN:
        {
      
            item = ITEM_MANAGER::instance().CreateItem(22419);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(11879);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(12669);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(13049);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(14209 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(15209 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(16209 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(17109 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(18019 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
        }
        break;
    case JOB_ASSASSIN:
        {
      
            item = ITEM_MANAGER::instance().CreateItem(22389);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(11479);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(12389);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(13049);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(14209 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(15209 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(16209 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(17109 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(18019 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
        }
        break;
    case JOB_WOLFMAN:
        {
      
            item = ITEM_MANAGER::instance().CreateItem(22429);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(21059);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(21529);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(13049);
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(14209 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(15209 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(16209 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(17109 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
            item = ITEM_MANAGER::instance().CreateItem(18019 );
            if (!item || !item->EquipTo(ch, item->FindEquipCell(ch)))
                M2_DESTROY_ITEM(item);
        }
        break;
    }
}


ACMD(do_i_efsun_ver1996)
{
    BYTE job = ch->GetJob();
    LPITEM item;


    switch (job)
    {
    case JOB_WARRIOR:
        {
            //  -- Kask Efsunlarý
            item = ch->GetWear(WEAR_HEAD);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_ATT_SPEED, 8);
                item->SetForceAttribute( 1, APPLY_HP_REGEN, 30);
                item->SetForceAttribute( 2, APPLY_ATTBONUS_DEVIL, 20);
                item->SetForceAttribute( 3, APPLY_ATTBONUS_HUMAN, 10);
                item->SetForceAttribute( 4, APPLY_ATTBONUS_UNDEAD, 20);
            }
            // -- Silah Efsunlarý
            item = ch->GetWear(WEAR_WEAPON);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_NORMAL_HIT_DAMAGE_BONUS, 60);
                item->SetForceAttribute( 1, APPLY_SKILL_DAMAGE_BONUS, -25);
                item->SetForceAttribute( 2, APPLY_STR, 12);
                item->SetForceAttribute( 3, APPLY_CRITICAL_PCT, 10);
                item->SetForceAttribute( 4, APPLY_PENETRATE_PCT, 10);
            }
            // -- Kalkan Efsunlarý
            item = ch->GetWear(WEAR_SHIELD);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_STR, 12);
                item->SetForceAttribute( 1, APPLY_BLOCK, 15);
                item->SetForceAttribute( 2, APPLY_REFLECT_MELEE, 10);
                item->SetForceAttribute( 3, APPLY_IMMUNE_STUN, 1);
                item->SetForceAttribute( 4, APPLY_EXP_DOUBLE_BONUS, 20);
            }
            //  -- Zýrh Efsunlarý
            item = ch->GetWear(WEAR_BODY);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_CAST_SPEED, 20);
                item->SetForceAttribute( 2, APPLY_STEAL_HP, 10);
                item->SetForceAttribute( 3, APPLY_REFLECT_MELEE, 10);
                item->SetForceAttribute( 4, APPLY_ATT_GRADE_BONUS, 50);
            }
            //  -- Ayakkabý Efsunlarý
            item = ch->GetWear(WEAR_FOOTS);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_STUN_PCT, 8);
                item->SetForceAttribute( 2, APPLY_EXP_DOUBLE_BONUS, 20);
                item->SetForceAttribute( 3, APPLY_ATT_SPEED, 8);
                item->SetForceAttribute( 4, APPLY_CRITICAL_PCT, 10);
            }
            //  -- Bilezik Efsunlarý
            item = ch->GetWear(WEAR_WRIST);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_ATTBONUS_UNDEAD, 20);
                item->SetForceAttribute( 2, APPLY_PENETRATE_PCT, 10);
                item->SetForceAttribute( 3, APPLY_STEAL_HP, 10);
                item->SetForceAttribute( 4, APPLY_ATTBONUS_DEVIL, 20);
            }
            //  -- Kolye Efsunlarý
            item = ch->GetWear(WEAR_NECK);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_HP_REGEN, 30);
                item->SetForceAttribute( 2, APPLY_CRITICAL_PCT, 10);
                item->SetForceAttribute( 3, APPLY_EXP_DOUBLE_BONUS, 20);
                item->SetForceAttribute( 4, APPLY_STUN_PCT, 8);
            }
            //  -- Kupe Efsunlarý
            item = ch->GetWear(WEAR_EAR);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MOV_SPEED, 20);
                item->SetForceAttribute( 1, APPLY_ATTBONUS_ANIMAL, 20);
                item->SetForceAttribute( 2, APPLY_POISON_REDUCE, 5);
                item->SetForceAttribute( 3, APPLY_ATTBONUS_DEVIL, 20);
                item->SetForceAttribute( 4, APPLY_ATTBONUS_UNDEAD, 20);
            }
        }
        break;
    case JOB_SURA:
        {
            //  -- Kask Efsunlarý
            item = ch->GetWear(WEAR_HEAD);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_ATT_SPEED, 8);
                item->SetForceAttribute( 1, APPLY_HP_REGEN, 30);
                item->SetForceAttribute( 2, APPLY_ATTBONUS_DEVIL, 20);
                item->SetForceAttribute( 3, APPLY_ATTBONUS_HUMAN, 10);
                item->SetForceAttribute( 4, APPLY_ATTBONUS_UNDEAD, 20);
            }
            // -- Silah Efsunlarý
            item = ch->GetWear(WEAR_WEAPON);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_NORMAL_HIT_DAMAGE_BONUS, 60);
                item->SetForceAttribute( 1, APPLY_SKILL_DAMAGE_BONUS, -25);
                item->SetForceAttribute( 2, APPLY_INT, 12);
                item->SetForceAttribute( 3, APPLY_CRITICAL_PCT, 10);
                item->SetForceAttribute( 4, APPLY_PENETRATE_PCT, 10);
            }
            // -- Kalkan Efsunlarý
            item = ch->GetWear(WEAR_SHIELD);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_INT, 12);
                item->SetForceAttribute( 1, APPLY_BLOCK, 15);
                item->SetForceAttribute( 2, APPLY_REFLECT_MELEE, 10);
                item->SetForceAttribute( 3, APPLY_IMMUNE_STUN, 1);
                item->SetForceAttribute( 4, APPLY_EXP_DOUBLE_BONUS, 20);
            }
            //  -- Zýrh Efsunlarý
            item = ch->GetWear(WEAR_BODY);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_CAST_SPEED, 20);
                item->SetForceAttribute( 2, APPLY_STEAL_HP, 10);
                item->SetForceAttribute( 3, APPLY_REFLECT_MELEE, 10);
                item->SetForceAttribute( 4, APPLY_ATT_GRADE_BONUS, 50);
            }
            //  -- Ayakkabý Efsunlarý
            item = ch->GetWear(WEAR_FOOTS);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_STUN_PCT, 8);
                item->SetForceAttribute( 2, APPLY_EXP_DOUBLE_BONUS, 20);
                item->SetForceAttribute( 3, APPLY_ATT_SPEED, 8);
                item->SetForceAttribute( 4, APPLY_CRITICAL_PCT, 10);
            }
            //  -- Bilezik Efsunlarý
            item = ch->GetWear(WEAR_WRIST);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_ATTBONUS_UNDEAD, 20);
                item->SetForceAttribute( 2, APPLY_PENETRATE_PCT, 10);
                item->SetForceAttribute( 3, APPLY_STEAL_HP, 10);
                item->SetForceAttribute( 4, APPLY_ATTBONUS_DEVIL, 20);
            }
            //  -- Kolye Efsunlarý
            item = ch->GetWear(WEAR_NECK);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_HP_REGEN, 30);
                item->SetForceAttribute( 2, APPLY_CRITICAL_PCT, 10);
                item->SetForceAttribute( 3, APPLY_EXP_DOUBLE_BONUS, 20);
                item->SetForceAttribute( 4, APPLY_STUN_PCT, 8);
            }
            //  -- Kupe Efsunlarý
            item = ch->GetWear(WEAR_EAR);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MOV_SPEED, 20);
                item->SetForceAttribute( 1, APPLY_ATTBONUS_ANIMAL, 20);
                item->SetForceAttribute( 2, APPLY_POISON_REDUCE, 5);
                item->SetForceAttribute( 3, APPLY_ATTBONUS_DEVIL, 20);
                item->SetForceAttribute( 4, APPLY_ATTBONUS_UNDEAD, 20);
            }
        }
        break;
    case JOB_SHAMAN:
        {
            //  -- Kask Efsunlarý
            item = ch->GetWear(WEAR_HEAD);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_ATT_SPEED, 8);
                item->SetForceAttribute( 1, APPLY_HP_REGEN, 30);
                item->SetForceAttribute( 2, APPLY_ATTBONUS_DEVIL, 20);
                item->SetForceAttribute( 3, APPLY_ATTBONUS_HUMAN, 10);
                item->SetForceAttribute( 4, APPLY_ATTBONUS_UNDEAD, 20);
            }
            // -- Silah Efsunlarý
            item = ch->GetWear(WEAR_WEAPON);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_NORMAL_HIT_DAMAGE_BONUS, 60);
                item->SetForceAttribute( 1, APPLY_SKILL_DAMAGE_BONUS, -25);
                item->SetForceAttribute( 2, APPLY_INT, 12);
                item->SetForceAttribute( 3, APPLY_CRITICAL_PCT, 10);
                item->SetForceAttribute( 4, APPLY_PENETRATE_PCT, 10);
            }
            // -- Kalkan Efsunlarý
            item = ch->GetWear(WEAR_SHIELD);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_INT, 12);
                item->SetForceAttribute( 1, APPLY_BLOCK, 15);
                item->SetForceAttribute( 2, APPLY_REFLECT_MELEE, 10);
                item->SetForceAttribute( 3, APPLY_IMMUNE_STUN, 1);
                item->SetForceAttribute( 4, APPLY_EXP_DOUBLE_BONUS, 20);
            }
            //  -- Zýrh Efsunlarý
            item = ch->GetWear(WEAR_BODY);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_CAST_SPEED, 20);
                item->SetForceAttribute( 2, APPLY_STEAL_HP, 10);
                item->SetForceAttribute( 3, APPLY_REFLECT_MELEE, 10);
                item->SetForceAttribute( 4, APPLY_ATT_GRADE_BONUS, 50);
            }
            //  -- Ayakkabý Efsunlarý
            item = ch->GetWear(WEAR_FOOTS);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_STUN_PCT, 8);
                item->SetForceAttribute( 2, APPLY_EXP_DOUBLE_BONUS, 20);
                item->SetForceAttribute( 3, APPLY_ATT_SPEED, 8);
                item->SetForceAttribute( 4, APPLY_CRITICAL_PCT, 10);
            }
            //  -- Bilezik Efsunlarý
            item = ch->GetWear(WEAR_WRIST);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_ATTBONUS_UNDEAD, 20);
                item->SetForceAttribute( 2, APPLY_PENETRATE_PCT, 10);
                item->SetForceAttribute( 3, APPLY_STEAL_HP, 10);
                item->SetForceAttribute( 4, APPLY_ATTBONUS_DEVIL, 20);
            }
            //  -- Kolye Efsunlarý
            item = ch->GetWear(WEAR_NECK);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_HP_REGEN, 30);
                item->SetForceAttribute( 2, APPLY_CRITICAL_PCT, 10);
                item->SetForceAttribute( 3, APPLY_EXP_DOUBLE_BONUS, 20);
                item->SetForceAttribute( 4, APPLY_STUN_PCT, 8);
            }
            //  -- Kupe Efsunlarý
            item = ch->GetWear(WEAR_EAR);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MOV_SPEED, 20);
                item->SetForceAttribute( 1, APPLY_ATTBONUS_ANIMAL, 20);
                item->SetForceAttribute( 2, APPLY_POISON_REDUCE, 5);
                item->SetForceAttribute( 3, APPLY_ATTBONUS_DEVIL, 20);
                item->SetForceAttribute( 4, APPLY_ATTBONUS_UNDEAD, 20);
            }
        }
        break;
    case JOB_ASSASSIN:
        {
            //  -- Kask Efsunlarý
            item = ch->GetWear(WEAR_HEAD);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_ATT_SPEED, 8);
                item->SetForceAttribute( 1, APPLY_HP_REGEN, 30);
                item->SetForceAttribute( 2, APPLY_ATTBONUS_DEVIL, 20);
                item->SetForceAttribute( 3, APPLY_ATTBONUS_HUMAN, 10);
                item->SetForceAttribute( 4, APPLY_ATTBONUS_UNDEAD, 20);
            }
            // -- Silah Efsunlarý
            item = ch->GetWear(WEAR_WEAPON);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_NORMAL_HIT_DAMAGE_BONUS, 60);
                item->SetForceAttribute( 1, APPLY_SKILL_DAMAGE_BONUS, -25);
                item->SetForceAttribute( 2, APPLY_STR, 12);
                item->SetForceAttribute( 3, APPLY_CRITICAL_PCT, 10);
                item->SetForceAttribute( 4, APPLY_PENETRATE_PCT, 10);
            }
            // -- Kalkan Efsunlarý
            item = ch->GetWear(WEAR_SHIELD);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_STR, 12);
                item->SetForceAttribute( 1, APPLY_BLOCK, 15);
                item->SetForceAttribute( 2, APPLY_REFLECT_MELEE, 10);
                item->SetForceAttribute( 3, APPLY_IMMUNE_STUN, 1);
                item->SetForceAttribute( 4, APPLY_EXP_DOUBLE_BONUS, 20);
            }
            //  -- Zýrh Efsunlarý
            item = ch->GetWear(WEAR_BODY);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_CAST_SPEED, 20);
                item->SetForceAttribute( 2, APPLY_STEAL_HP, 10);
                item->SetForceAttribute( 3, APPLY_REFLECT_MELEE, 10);
                item->SetForceAttribute( 4, APPLY_ATT_GRADE_BONUS, 50);
            }
            //  -- Ayakkabý Efsunlarý
            item = ch->GetWear(WEAR_FOOTS);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_STUN_PCT, 8);
                item->SetForceAttribute( 2, APPLY_EXP_DOUBLE_BONUS, 20);
                item->SetForceAttribute( 3, APPLY_ATT_SPEED, 8);
                item->SetForceAttribute( 4, APPLY_CRITICAL_PCT, 10);
            }
            //  -- Bilezik Efsunlarý
            item = ch->GetWear(WEAR_WRIST);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_ATTBONUS_UNDEAD, 20);
                item->SetForceAttribute( 2, APPLY_PENETRATE_PCT, 10);
                item->SetForceAttribute( 3, APPLY_STEAL_HP, 10);
                item->SetForceAttribute( 4, APPLY_ATTBONUS_DEVIL, 20);
            }
            //  -- Kolye Efsunlarý
            item = ch->GetWear(WEAR_NECK);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_HP_REGEN, 30);
                item->SetForceAttribute( 2, APPLY_CRITICAL_PCT, 10);
                item->SetForceAttribute( 3, APPLY_EXP_DOUBLE_BONUS, 20);
                item->SetForceAttribute( 4, APPLY_STUN_PCT, 8);
            }
            //  -- Kupe Efsunlarý
            item = ch->GetWear(WEAR_EAR);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MOV_SPEED, 20);
                item->SetForceAttribute( 1, APPLY_ATTBONUS_ANIMAL, 20);
                item->SetForceAttribute( 2, APPLY_POISON_REDUCE, 5);
                item->SetForceAttribute( 3, APPLY_ATTBONUS_DEVIL, 20);
                item->SetForceAttribute( 4, APPLY_ATTBONUS_UNDEAD, 20);
            }
        }
        break;
    case JOB_WOLFMAN:
        {
            //  -- Kask Efsunlarý
            item = ch->GetWear(WEAR_HEAD);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_ATT_SPEED, 8);
                item->SetForceAttribute( 1, APPLY_HP_REGEN, 30);
                item->SetForceAttribute( 2, APPLY_ATTBONUS_DEVIL, 20);
                item->SetForceAttribute( 3, APPLY_ATTBONUS_HUMAN, 10);
                item->SetForceAttribute( 4, APPLY_ATTBONUS_UNDEAD, 20);
            }
            // -- Silah Efsunlarý
            item = ch->GetWear(WEAR_WEAPON);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_NORMAL_HIT_DAMAGE_BONUS, 60);
                item->SetForceAttribute( 1, APPLY_SKILL_DAMAGE_BONUS, -25);
                item->SetForceAttribute( 2, APPLY_STR, 12);
                item->SetForceAttribute( 3, APPLY_INT, 12);
                item->SetForceAttribute( 4, APPLY_CRITICAL_PCT, 10);
            }
            // -- Kalkan Efsunlarý
            item = ch->GetWear(WEAR_SHIELD);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_STR, 12);
                item->SetForceAttribute( 1, APPLY_INT, 12);
                item->SetForceAttribute( 2, APPLY_BLOCK, 15);
                item->SetForceAttribute( 3, APPLY_IMMUNE_STUN, 1);
                item->SetForceAttribute( 4, APPLY_EXP_DOUBLE_BONUS, 20);
            }
            //  -- Zýrh Efsunlarý
            item = ch->GetWear(WEAR_BODY);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_CAST_SPEED, 20);
                item->SetForceAttribute( 2, APPLY_STEAL_HP, 10);
                item->SetForceAttribute( 3, APPLY_REFLECT_MELEE, 10);
                item->SetForceAttribute( 4, APPLY_ATT_GRADE_BONUS, 50);
            }
            //  -- Ayakkabý Efsunlarý
            item = ch->GetWear(WEAR_FOOTS);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_STUN_PCT, 8);
                item->SetForceAttribute( 2, APPLY_EXP_DOUBLE_BONUS, 20);
                item->SetForceAttribute( 3, APPLY_ATT_SPEED, 8);
                item->SetForceAttribute( 4, APPLY_CRITICAL_PCT, 10);
            }
            //  -- Bilezik Efsunlarý
            item = ch->GetWear(WEAR_WRIST);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_ATTBONUS_UNDEAD, 20);
                item->SetForceAttribute( 2, APPLY_PENETRATE_PCT, 10);
                item->SetForceAttribute( 3, APPLY_STEAL_HP, 10);
                item->SetForceAttribute( 4, APPLY_ATTBONUS_DEVIL, 20);
            }
            //  -- Kolye Efsunlarý
            item = ch->GetWear(WEAR_NECK);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MAX_HP, 3000);
                item->SetForceAttribute( 1, APPLY_HP_REGEN, 30);
                item->SetForceAttribute( 2, APPLY_CRITICAL_PCT, 10);
                item->SetForceAttribute( 3, APPLY_EXP_DOUBLE_BONUS, 20);
                item->SetForceAttribute( 4, APPLY_STUN_PCT, 8);
            }
            //  -- Kupe Efsunlarý
            item = ch->GetWear(WEAR_EAR);
            if (item != NULL)
            {
                item->ClearAttribute();
                item->SetForceAttribute( 0, APPLY_MOV_SPEED, 20);
                item->SetForceAttribute( 1, APPLY_ATTBONUS_ANIMAL, 20);
                item->SetForceAttribute( 2, APPLY_POISON_REDUCE, 5);
                item->SetForceAttribute( 3, APPLY_ATTBONUS_DEVIL, 20);
                item->SetForceAttribute( 4, APPLY_ATTBONUS_UNDEAD, 20);
            }
        }
        break;
    }
}
#ifdef __AUCTION__
// temp_auction
ACMD(do_get_item_id_list)
{
	for (int i = 0; i < INVENTORY_MAX_NUM; i++)
	{
		LPITEM item = ch->GetInventoryItem(i);
		if (item != NULL)
			ch->ChatPacket(CHAT_TYPE_INFO, "name : %s id : %d", item->GetProto()->szName, item->GetID());
	}
}

// temp_auction

ACMD(do_enroll_auction)
{
	char arg1[256];
	char arg2[256];
	char arg3[256];
	char arg4[256];
	two_arguments (two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2)), arg3, sizeof(arg3), arg4, sizeof(arg4));
	
	DWORD item_id = strtoul(arg1, NULL, 10);
	BYTE empire = strtoul(arg2, NULL, 10);
	int bidPrice = strtol(arg3, NULL, 10);
	int immidiatePurchasePrice = strtol(arg4, NULL, 10);

	LPITEM item = ITEM_MANAGER::instance().Find(item_id);
	if (item == NULL)
		return;

	AuctionManager::instance().enroll_auction(ch, item, empire, bidPrice, immidiatePurchasePrice);
}

ACMD(do_enroll_wish)
{
	char arg1[256];
	char arg2[256];
	char arg3[256];
	one_argument (two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2)), arg3, sizeof(arg3));
	
	DWORD item_num = strtoul(arg1, NULL, 10);
	BYTE empire = strtoul(arg2, NULL, 10);
	int wishPrice = strtol(arg3, NULL, 10);

	AuctionManager::instance().enroll_wish(ch, item_num, empire, wishPrice);
}

ACMD(do_enroll_sale)
{
	char arg1[256];
	char arg2[256];
	char arg3[256];
	one_argument (two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2)), arg3, sizeof(arg3));
	
	DWORD item_id = strtoul(arg1, NULL, 10);
	DWORD wisher_id = strtoul(arg2, NULL, 10);
	int salePrice = strtol(arg3, NULL, 10);

	LPITEM item = ITEM_MANAGER::instance().Find(item_id);
	if (item == NULL)
		return;

	AuctionManager::instance().enroll_sale(ch, item, wisher_id, salePrice);
}

// temp_auction
// packetÀ¸·Î Åë½ÅÇÏ°Ô ÇÏ°í, ÀÌ°Ç »èÁ¦ÇØ¾ßÇÑ´Ù.
ACMD(do_get_auction_list)
{
	char arg1[256];
	char arg2[256];
	char arg3[256];
	two_arguments (one_argument (argument, arg1, sizeof(arg1)), arg2, sizeof(arg2), arg3, sizeof(arg3));

	AuctionManager::instance().get_auction_list (ch, strtoul(arg1, NULL, 10), strtoul(arg2, NULL, 10), strtoul(arg3, NULL, 10));
}
//
//ACMD(do_get_wish_list)
//{
//	char arg1[256];
//	char arg2[256];
//	char arg3[256];
//	two_arguments (one_argument (argument, arg1, sizeof(arg1)), arg2, sizeof(arg2), arg3, sizeof(arg3));
//
//	AuctionManager::instance().get_wish_list (ch, strtoul(arg1, NULL, 10), strtoul(arg2, NULL, 10), strtoul(arg3, NULL, 10));
//}
ACMD (do_get_my_auction_list)
{
	char arg1[256];
	char arg2[256];
	two_arguments (argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	AuctionManager::instance().get_my_auction_list (ch, strtoul(arg1, NULL, 10), strtoul(arg2, NULL, 10));
}

ACMD (do_get_my_purchase_list)
{
	char arg1[256];
	char arg2[256];
	two_arguments (argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	AuctionManager::instance().get_my_purchase_list (ch, strtoul(arg1, NULL, 10), strtoul(arg2, NULL, 10));
}

ACMD (do_auction_bid)
{
	char arg1[256];
	char arg2[256];
	two_arguments (argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	AuctionManager::instance().bid (ch, strtoul(arg1, NULL, 10), strtoul(arg2, NULL, 10));
}

ACMD (do_auction_impur)
{
	char arg1[256];
	one_argument (argument, arg1, sizeof(arg1));

	AuctionManager::instance().immediate_purchase (ch, strtoul(arg1, NULL, 10));
}

ACMD (do_get_auctioned_item)
{
	char arg1[256];
	char arg2[256];
	two_arguments (argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	AuctionManager::instance().get_auctioned_item (ch, strtoul(arg1, NULL, 10), strtoul(arg2, NULL, 10));
}

ACMD (do_buy_sold_item)
{
	char arg1[256];
	char arg2[256];
	one_argument (argument, arg1, sizeof(arg1));

	AuctionManager::instance().get_auctioned_item (ch, strtoul(arg1, NULL, 10), strtoul(arg2, NULL, 10));
}

ACMD (do_cancel_auction)
{
	char arg1[256];
	one_argument (argument, arg1, sizeof(arg1));

	AuctionManager::instance().cancel_auction (ch, strtoul(arg1, NULL, 10));
}

ACMD (do_cancel_wish)
{
	char arg1[256];
	one_argument (argument, arg1, sizeof(arg1));

	AuctionManager::instance().cancel_wish (ch, strtoul(arg1, NULL, 10));
}

ACMD (do_cancel_sale)
{
	char arg1[256];
	one_argument (argument, arg1, sizeof(arg1));

	AuctionManager::instance().cancel_sale (ch, strtoul(arg1, NULL, 10));
}

ACMD (do_rebid)
{
	char arg1[256];
	char arg2[256];
	two_arguments (argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	AuctionManager::instance().rebid (ch, strtoul(arg1, NULL, 10), strtoul(arg2, NULL, 10));
}

ACMD (do_bid_cancel)
{
	char arg1[256];
	char arg2[256];
	two_arguments (argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	AuctionManager::instance().bid_cancel (ch, strtoul(arg1, NULL, 10));
}
#endif

#ifdef ENABLE_BATTLE_PASS
ACMD(open_battlepass)
{
	if (ch->v_counts.empty())
		return;

	if (ch->missions_bp.empty())
		return;

	if (ch->GetExchange() || ch->GetMyShop() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->IsSashCombinationOpen() || ch->IsSashAbsorptionOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<BP> Lutfen aktif pencereleri kapatiniz");
		return;
	}

	time_t cur_Time = time(NULL);
	struct tm vKey = *localtime(&cur_Time);

	int month = vKey.tm_mon;

	if (month != ch->iMonthBattlePass)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Zaman gecmis!");
		return;
	}

	for (int i = 0; i<ch->missions_bp.size(); ++i)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "missions_bp %d %d %d %d", i, ch->missions_bp[i].type, ch->missions_bp[i].vnum, ch->missions_bp[i].count);
		ch->ChatPacket(CHAT_TYPE_COMMAND, "info_missions_bp %d %d %d %s", i, ch->v_counts[i].count, ch->v_counts[i].status, ch->rewards_bp[i].name);
		ch->ChatPacket(CHAT_TYPE_COMMAND, "rewards_missions_bp %d %d %d %d %d %d %d", i, ch->rewards_bp[i].vnum1, ch->rewards_bp[i].vnum2, ch->rewards_bp[i].vnum3, ch->rewards_bp[i].count1, ch->rewards_bp[i].count2, ch->rewards_bp[i].count3);
	}

	ch->ChatPacket(CHAT_TYPE_COMMAND, "size_missions_bp %d ", ch->missions_bp.size());
	ch->ChatPacket(CHAT_TYPE_COMMAND, "final_reward %d %d %d %d %d %d", ch->final_rewards[0].f_vnum1, ch->final_rewards[0].f_vnum2, ch->final_rewards[0].f_vnum3, ch->final_rewards[0].f_count1, ch->final_rewards[0].f_count2, ch->final_rewards[0].f_count3);
	ch->ChatPacket(CHAT_TYPE_COMMAND, "show_battlepass");

}

ACMD(final_reward_battlepass)
{
	if (ch->v_counts.empty())
		return;

	if (ch->missions_bp.empty())
		return;

	if (ch->v_counts[0].status == 2)
		return;

	if (ch->GetExchange() || ch->GetMyShop() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->IsSashCombinationOpen() || ch->IsSashAbsorptionOpen())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "<BP> Lutfen aktif pencereleri kapatiniz");
		return;
	}

	time_t cur_Time = time(NULL);
	struct tm vKey = *localtime(&cur_Time);

	int month = vKey.tm_mon;

	if (month != ch->iMonthBattlePass)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Zaman gecmis!");
		return;
	}

	for (int i = 0; i<ch->missions_bp.size(); ++i)
	{
		if (ch->missions_bp[i].count != ch->v_counts[i].count)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Gorev daha bitmemis, goreve devam edebilirsiniz!");
			return;
		}
	}

	ch->FinalRewardBattlePass();
}
#endif

#ifdef ENABLE_GUVENLI_PC
ACMD(do_guvenli_pc)
{
	if (quest::CQuestManager::instance().GetEventFlag("guvenli_pc_disable") == 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Sistem suan icin devre disi!");
		return;
	}

	int iPulse = thecore_pulse();

	if (iPulse - ch->GetLastGuvenliPCLastTime() < passes_per_sec * 2)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Biraz beklemelisin.");
		return;
	}

	ch->SetLastGuvenliPCLastTime(iPulse);

	char arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	if (!*arg1 || 0 == arg1[0] || !*arg2 || 0 == arg2[0])
		return;

	char szHW[255 + 1];
	DBManager::instance().EscapeString(szHW, sizeof(szHW), arg2, strlen(arg2));

	const std::string& IslemSec = std::string(arg1);
	if (IslemSec == "AktifEt")
	{
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT login FROM account.account WHERE id = '%u'", ch->GetAID()));
		if (pMsg->Get()->uiNumRows != 0)
		{
			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
			char login[32];
			snprintf(login, sizeof(login), "%s", row[0]);

			std::unique_ptr<SQLMsg> pMsg1(DBManager::instance().DirectQuery("SELECT durum FROM player.guvenlipc WHERE login = '%s' and guvenli_pc = '%s'", login, szHW));
			if (pMsg1->Get()->uiNumRows != 0)
			{
				MYSQL_ROW row = mysql_fetch_row(pMsg1->Get()->pSQLResult);
				BYTE bDurum = 0;
				str_to_number(bDurum, row[0]);
				if (bDurum == 1)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Guvenli bilgisayar zaten aktif!");
					return;
				}

				DBManager::instance().DirectQuery("UPDATE player.guvenlipc SET durum = 1 WHERE login = '%s' and guvenli_pc = '%s'", login, szHW);
				ch->ChatPacket(CHAT_TYPE_INFO, "Guvenli bilgisayar aktif edildi.");
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "Bu bilgisayar ekli degil!");
			}
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Bir hata olustu!");
		}
	}
	else if (IslemSec == "PasifEt")
	{
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT login FROM account.account WHERE id = '%u'", ch->GetAID()));
		if (pMsg->Get()->uiNumRows != 0)
		{
			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
			char login[32];
			snprintf(login, sizeof(login), "%s", row[0]);

			std::unique_ptr<SQLMsg> pMsg1(DBManager::instance().DirectQuery("SELECT durum FROM player.guvenlipc WHERE login = '%s' and guvenli_pc = '%s'", login, szHW));
			if (pMsg1->Get()->uiNumRows != 0)
			{
				MYSQL_ROW row = mysql_fetch_row(pMsg1->Get()->pSQLResult);
				BYTE bDurum = 0;
				str_to_number(bDurum, row[0]);
				if (bDurum == 0)
				{
					ch->ChatPacket(CHAT_TYPE_INFO, "Guvenli bilgisayar zaten deaktif!");
					return;
				}

				DBManager::instance().DirectQuery("UPDATE player.guvenlipc SET durum = 0 WHERE login = '%s' and guvenli_pc = '%s'", login, szHW);
				ch->ChatPacket(CHAT_TYPE_INFO, "Guvenli bilgisayar deaktif edildi.");
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "Bu bilgisayar ekli degil!");
			}
		}
		else
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Bir hata olustu!");
		}
	}
	else if (IslemSec == "PcEkle")
	{
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT login FROM account.account WHERE id = '%u'", ch->GetAID()));
		if (pMsg->Get()->uiNumRows != 0)
		{
			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
			char login[32];
			snprintf(login, sizeof(login), "%s", row[0]);

			std::unique_ptr<SQLMsg> pMsg1(DBManager::instance().DirectQuery("SELECT COUNT(*) FROM player.guvenlipc WHERE login = '%s' and guvenli_pc = '%s'", login, szHW));
			if (pMsg1->Get()->uiNumRows == 0)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "Bu bilgisayar ekli degil!");
				return;
			}

			MYSQL_ROW row1 = mysql_fetch_row(pMsg1->Get()->pSQLResult);
			BYTE bAdet = 0;
			str_to_number(bAdet, row1[0]);
			if (bAdet >= 1)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "Bu bilgisayar zaten eklenmis!");
				return;
			}
			else
			{
				DBManager::instance().DirectQuery("INSERT INTO player.guvenlipc(login, guvenli_pc, durum) VALUES('%s', '%s', 1);", login, szHW);
				ch->ChatPacket(CHAT_TYPE_INFO, "Bilgisayarin eklendi.");
			}
		}
	}
	else if (IslemSec == "PcSil")
	{
		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT login FROM account.account WHERE id = '%u'", ch->GetAID()));
		if (pMsg->Get()->uiNumRows != 0)
		{
			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
			char login[32];
			snprintf(login, sizeof(login), "%s", row[0]);

			std::unique_ptr<SQLMsg> pMsg1(DBManager::instance().DirectQuery("SELECT COUNT(*) FROM player.guvenlipc WHERE login = '%s' and guvenli_pc = '%s'", login, szHW));
			if (pMsg1->Get()->uiNumRows == 0)
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "Bu bilgisayar ekli degil!");
				return;
			}

			MYSQL_ROW row1 = mysql_fetch_row(pMsg1->Get()->pSQLResult);
			BYTE bAdet = 0;
			str_to_number(bAdet, row1[0]);
			if (bAdet >= 1)
			{
				DBManager::instance().DirectQuery("DELETE FROM player.guvenlipc WHERE login = '%s' and guvenli_pc = '%s'", login, szHW);
				ch->ChatPacket(CHAT_TYPE_INFO, "Bilgisayarin silindi.");
			}
			else
			{
				ch->ChatPacket(CHAT_TYPE_INFO, "Bu bilgisayar zaten silinmis veya ekli degil!");
				return;
			}
		}
	}
}
#endif
#ifdef ENABLE_BEVIS_BOSS_TRACKING_SYSTEM
ACMD(do_open_boss_tracking)
{
	if (!ch || !ch->GetDesc())
		return;

	if (ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen())
		return;
	
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	DWORD mob_vnum = 0;
	str_to_number(mob_vnum, arg1);
		
	for (int i = 0; i < 7; i++)
		CBossTracking::instance().SendClientPacket(ch, i, mob_vnum);
	
	// ch->IsBossTrackingWindow = true;
	// ch->BossTrackingMobVnum = mob_vnum;
}
ACMD(do_close_boss_tracking)
{
	ch->StopBossTrackingDataEvent();
}
#endif
ACMD(do_stat_val)
{
	char	arg1[256], arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));
	int val = 0;
	str_to_number(val, arg2);
	
	if (!*arg1 || val <= 0)
		return;

	if (ch->IsPolymorphed())
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot change your state as long as you are transformed."));
		return;
	}

	if (ch->GetPoint(POINT_STAT) <= 0)
		return;

	BYTE idx = 0;
	
	if (!strcmp(arg1, "st"))
		idx = POINT_ST;
	else if (!strcmp(arg1, "dx"))
		idx = POINT_DX;
	else if (!strcmp(arg1, "ht"))
		idx = POINT_HT;
	else if (!strcmp(arg1, "iq"))
		idx = POINT_IQ;
	else
		return;

	if (ch->GetRealPoint(idx) >= MAX_STAT)
		return;
	
	if (val > ch->GetPoint(POINT_STAT))
		val = ch->GetPoint(POINT_STAT);
	
	if (ch->GetRealPoint(idx) + val > MAX_STAT)
		val = MAX_STAT - ch->GetRealPoint(idx);

	ch->SetRealPoint(idx, ch->GetRealPoint(idx) + val);
	ch->SetPoint(idx, ch->GetPoint(idx) + val);
	ch->ComputePoints();
	ch->PointChange(idx, 0);

	if (idx == POINT_IQ)
		ch->PointChange(POINT_MAX_HP, 0);
	else if (idx == POINT_HT)
		ch->PointChange(POINT_MAX_SP, 0);

	ch->PointChange(POINT_STAT, -val);
	ch->ComputePoints();
}

ACMD(do_gold_to_cheque)
{
	if (quest::CQuestManager::instance().GetEventFlag("enable_yang_takas") == 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Sistem> Bu islemi þuanlýk devredýþý"));
		return;
	}

	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	{
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
        return;
	}


	if (ch->IsActivateSecurity() == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Guvenlik acik iken item silemezsin.");
		return;
	}


	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	
	int inputYangAl = atoi(arg1);

	if (ch->GetCheque() < 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ENVANTERDE_WON_YOK"));
		return;
	}

	if (ch->GetGold() >= 1900000000)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("COK_FAZLA_YANG_VAR"));
		return;
	}

	if (inputYangAl > 1 or inputYangAl < 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("WON_DEGER_GECERSIZ"));
		return;
	}

	ch->PointChange(POINT_CHEQUE, -inputYangAl, false);
	ch->PointChange(POINT_GOLD, inputYangAl*100000000, false);
}

ACMD(do_cheque_to_gold)
{

	if (quest::CQuestManager::instance().GetEventFlag("enable_cheque_takas") == 1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<Sistem> Bu islemi þuanlýk devredýþý"));
		return;
	}

	if (ch->IsDead() || ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen() || ch->GetOfflineShopOwner())
	{
        ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("pencere_kapa"));
        return;
	}

	if (ch->IsActivateSecurity() == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Guvenlik acik iken item silemezsin.");
		return;
	}

	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	
	int inputWonAl = atoi(arg1);

	if (ch->GetCheque() >= 99999)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("COK_FAZLA_WON_VAR"));
		return;
	}

	if (ch->GetGold() < 100000000)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ENVANTERDE_YANG_YOK"));
		return;
	}

	if (inputWonAl > 100000000 || inputWonAl < 100000000)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("YANG_DEGER_GECERSIZ"));
		return;
	}

	ch->PointChange(POINT_GOLD, -inputWonAl, false);
	ch->PointChange(POINT_CHEQUE, inputWonAl/100000000, false);
}

ACMD(do_wiki)
{
	if (0 == quest::CQuestManager::instance().GetEventFlag("enable_wiki"))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Sistem devredisi."));
		return;
	}
	ch->ChatPacket(CHAT_TYPE_COMMAND, "open_searched");
}

#ifdef ENABLE_TITLE_SYSTEM
ACMD(do_prestige_title)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "your argument is invalid");
		return;
	}
	
	if (!strcmp(arg1, "buy_prestige_1"))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Devre disi.");
		return;
	}

	if (!strcmp(arg1, "buy_prestige_2"))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Devre disi.");
		return;
	}

	if (!strcmp(arg1, "buy_prestige_3"))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Devre disi.");
		return;
	}

	if (!strcmp(arg1, "prestige_0"))
	{
		TitleManager::instance().SetTitle(ch, "disable");
	}

	if (!strcmp(arg1, "prestige_1"))
	{
		TitleManager::instance().SetTitle(ch, "title1");
	}

	if (!strcmp(arg1, "prestige_2"))
	{
		TitleManager::instance().SetTitle(ch, "title2");
	}

	if (!strcmp(arg1, "prestige_3"))
	{
		TitleManager::instance().SetTitle(ch, "title3");
	}

	if (!strcmp(arg1, "prestige_4"))
	{
		TitleManager::instance().SetTitle(ch, "title4");
	}

	if (!strcmp(arg1, "prestige_5"))
	{
		TitleManager::instance().SetTitle(ch, "title5");
	}

	if (!strcmp(arg1, "prestige_6"))
	{
		TitleManager::instance().SetTitle(ch, "title6");
	}

	if (!strcmp(arg1, "prestige_7"))
	{
		TitleManager::instance().SetTitle(ch, "title7");
	}

	if (!strcmp(arg1, "prestige_8"))
	{
		TitleManager::instance().SetTitle(ch, "title8");
	}

	if (!strcmp(arg1, "prestige_9"))
	{
		TitleManager::instance().SetTitle(ch, "title9");
	}
	
	if (!strcmp(arg1, "prestige_10"))
	{
		TitleManager::instance().SetTitle(ch, "title10");
	}

	if (!strcmp(arg1, "prestige_11"))
	{
		TitleManager::instance().SetTitle(ch, "title11");
	}

	if (!strcmp(arg1, "prestige_12"))
	{
		TitleManager::instance().SetTitle(ch, "title12");
	}

	if (!strcmp(arg1, "prestige_13"))
	{
		TitleManager::instance().SetTitle(ch, "title13");
	}

	if (!strcmp(arg1, "prestige_14"))
	{
		TitleManager::instance().SetTitle(ch, "title14");
	}

	if (!strcmp(arg1, "prestige_15"))
	{
		TitleManager::instance().SetTitle(ch, "title15");
	}

	if (!strcmp(arg1, "prestige_16"))
	{
		TitleManager::instance().SetTitle(ch, "title16");
	}

	if (!strcmp(arg1, "prestige_17"))
	{
		TitleManager::instance().SetTitle(ch, "title17");
	}

	if (!strcmp(arg1, "prestige_18"))
	{
		TitleManager::instance().SetTitle(ch, "title18");
	}

	if (!strcmp(arg1, "prestige_19"))
	{
		TitleManager::instance().SetTitle(ch, "title19");
	}

	if (!strcmp(arg1, "vegas_transform_title"))
	{
		TitleManager::instance().TransformTitle(ch);
	}
}
#endif
ACMD(do_ticaret_cam)
{
	ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenShopSearch 1");
}

ACMD(do_antiexp_kapa)
{
	
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Anti_exp_deactive"));
	ch->RemoveAffect(AFFECT_EXP_CURSE);
	// LPITEM item->Lock(false);
	// LPITEM item->SetSocket(0, false);
}



ACMD(do_antiexp_ac)
{
	ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Anti_exp_active"));
	ch->AddAffect(AFFECT_EXP_CURSE, POINT_NONE, 0, AFF_NONE, INFINITE_AFFECT_DURATION, 0, true);
	
	// LPITEM item->Lock(true);
	// LPITEM item->SetSocket(0, true);
}

struct MaintenanceClose
{
	void operator () (LPDESC d)
	{
		if (d->GetCharacter())
		{
			if (d->GetCharacter() != NULL)
				d->GetCharacter()->ChatPacket(CHAT_TYPE_COMMAND, "BINARY_Close_Maintenance");
		}
	}
};

struct MaintenanceUpdate
{
	void operator () (LPDESC d)
	{
		if (d->GetCharacter())
		{
			if (d->GetCharacter() != NULL)
				MaintenanceManager::instance().Send_CheckTable(d->GetCharacter());
		}
	}
};

ACMD(do_maintenance)
{
	char arg1[256];
	char arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!just_number(arg1) && !(*arg1 && !strcmp(arg1, "disable")))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bakim_sistemi_1"));
		return;
	}

	if (!just_number(arg2) && !(*arg1 && !strcmp(arg1, "disable")))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("bakim_sistemi_2"));
		return;
	}

	if (*arg1 && !strcmp(arg1, "disable"))
	{
		MaintenanceManager::instance().Send_DisableSecurity(ch);
		const DESC_MANAGER::DESC_SET & close = DESC_MANAGER::instance().GetClientSet();
		std::for_each(close.begin(), close.end(), MaintenanceClose());
	}
	else
	{
		long time_maintenance = parse_time_str(arg1);
		long duration_maintenance = parse_time_str(arg2);
		MaintenanceManager::instance().Send_ActiveMaintenance(ch, time_maintenance, duration_maintenance);
	}
}

ACMD(do_maintenance_text)
{
	char arg1[256];
	char arg2[256];
	two_arguments(argument, arg1, sizeof(arg1), arg2, sizeof(arg2));

	if (!*arg1)
		return;

	if (*arg1 && !strcmp(arg1, "disable"))
	{
		MaintenanceManager::instance().Send_Text(ch, "rmf");
	}
	else if (*arg1 && !strcmp(arg1, "enable"))
	{
		const char * sReason;
		sReason = *arg2 ? one_argument(argument, arg2, sizeof(arg2)) : "no_reason";
		MaintenanceManager::instance().Send_Text(ch, sReason);
	}
}

ACMD(do_maintenance_update)
{
	const DESC_MANAGER::DESC_SET & update = DESC_MANAGER::instance().GetClientSet();
	std::for_each(update.begin(), update.end(), MaintenanceUpdate());
}

#ifdef OTOMATIK_AV
ACMD(do_otoAv) {
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));
	switch (LOWER(arg1[0])){
		case 'b':
			if (ch->GetPremiumRemainSeconds(PREMIUM_OTOMATIK_AV) > 0) {
				if (!ch->IsAffectFlag(AFF_OTOAV_AKTIF)) { ch->AddAffect(AFFECT_OTOAV_AKTIF, POINT_NONE, 0, AFF_OTOAV_AKTIF, INFINITE_AFFECT_DURATION, 0, false); }
			}
			break;

		case 'd':
			if (ch->IsAffectFlag(AFF_OTOAV_AKTIF)){ch->RemoveAffect(AFFECT_OTOAV_AKTIF);}
			break;

		default:
			break;
	}
}
#endif

#ifdef ENABLE_AFFECT_BUFF_REMOVE
ACMD(do_remove_buff)
{
	char arg1[256];
	one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	if (!ch)
		return;

	int affect = 0;
	str_to_number(affect, arg1);
	CAffect* pAffect = ch->FindAffect(affect);

	
	
	std::map<DWORD, DWORD> buffsvec = {
	{94,245},
	{95,246},
	{96,247},
	{110,248},
	{111,249}
	};
	for (auto it = buffsvec.begin(); it != buffsvec.end(); it++)
	{
		if (affect == it->first)
		{
			ch->RemoveAffect(it->second);
		}
	}
	if (pAffect)
		ch->RemoveAffect(affect);
}
#endif