#pragma once
#define GECICI_FIX
#include "../../libgame/src/grid.h"

enum
{
	OFFLINE_SHOP_MAX_DISTANCE = 1500,
};

class COfflineShop
{
	public:
		COfflineShop();
		~COfflineShop();

typedef struct offline_shop_item
		{
			DWORD	id;
			DWORD	owner_id;
			BYTE	pos;
			short	count;
			DWORD		price;
			//DWORD		price2;
			DWORD		price_cheque;
			DWORD		vnum;
			long	alSockets[ITEM_SOCKET_MAX_NUM];
			TPlayerItemAttribute	aAttr[ITEM_ATTRIBUTE_MAX_NUM];
			BYTE	status;
			char szBuyerName[CHARACTER_NAME_MAX_LEN + 1];
			DWORD	evolution;
			char szName[ITEM_NAME_MAX_LEN + 1];
			BYTE	refine_level;
			DWORD	shop_id;
			char		item_name[CHARACTER_NAME_MAX_LEN];
			offline_shop_item()
			{
				id = 0;
				owner_id = pos = count = price = 0;
				//price2 = vnum = status = 0;
				price_cheque = vnum = status = 0;
				memset(alSockets, 0, sizeof(alSockets));
				memset(aAttr, 0, sizeof(aAttr));
				szBuyerName[CHARACTER_NAME_MAX_LEN + 1] = 0;
				evolution = 0;
				szName[ITEM_NAME_MAX_LEN + 1] = 0;
				refine_level = 0;
				shop_id = 0;
				memset(item_name, 0, sizeof(item_name));
			}
		} OFFLINE_SHOP_ITEM;

		std::vector<OFFLINE_SHOP_ITEM>	GetItemVector() { return m_itemVector; }

		void				SetGuestMap(LPCHARACTER ch);
		void				RemoveGuestMap(LPCHARACTER ch);
		
		virtual void	SetOfflineShopNPC(LPCHARACTER npc);
		virtual bool	IsOfflineShopNPC(){ return m_pkOfflineShopNPC ? true : false; }

		virtual bool	AddGuest(LPCHARACTER ch, LPCHARACTER npc);
		
		void			RemoveGuest(LPCHARACTER ch);
		void			RemoveAllGuest();
		void			Destroy(LPCHARACTER npc);

		virtual int		Buy(LPCHARACTER ch, BYTE bPos);
		
		void			BroadcastUpdateItem(BYTE bPos, DWORD dwPID, bool bDestroy = false);
		void			BroadcastUpdatePrice(BYTE bPos, DWORD dwPrice);
		void			Refresh(LPCHARACTER ch);

		bool			RemoveItem(DWORD dwVID, BYTE bPos);
		short			GetLeftItemCount(DWORD dwPID);
 		bool			SearchItem(const std::string& isim, long price);		
		bool			SetShopItems(LPCHARACTER npc, DWORD shopVID);		
	protected:
		void			Broadcast(const void * data, int bytes);

	private:
		// Grid
		CGrid *				m_pGrid;

		// Guest Map
		typedef TR1_NS::unordered_map<LPCHARACTER, bool> GuestMapType;
		GuestMapType m_map_guest;
		// End Of Guest Map

#ifdef GECICI_FIX
		std::vector<DWORD>	guestlist;
#endif

		LPCHARACTER m_pkOfflineShopNPC;

		std::vector<OFFLINE_SHOP_ITEM>		m_itemVector;
};

