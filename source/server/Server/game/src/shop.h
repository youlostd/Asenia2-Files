#ifndef __INC_METIN_II_GAME_SHOP_H__
#define __INC_METIN_II_GAME_SHOP_H__

enum
{
	SHOP_MAX_DISTANCE = 1000
};

class CGrid;

/* ---------------------------------------------------------------------------------- */
class CShop
{
	public:
		typedef struct shop_item
		{
			DWORD	vnum;		// 아이템 번호
			long	price;		// 가격
#ifdef ENABLE_CHEQUE_SYSTEM
			long	price_cheque;
#endif
#ifdef PRICE_TYPE
			long	price_type;	
#endif
#ifdef __EXTENDED_ITEM_COUNT__
			short count;
#else
			BYTE count; // 아이템 개수
#endif

			LPITEM	pkItem;
			int		itemid;		// 아이템 고유아이디

			shop_item()
			{
				vnum = 0;
				price = 0;
#ifdef ENABLE_CHEQUE_SYSTEM
				price_cheque = 0;
#endif
				count = 0;
#ifdef PRICE_TYPE
				price_type = 0;
#endif
				itemid = 0;
				pkItem = NULL;
			}
		} SHOP_ITEM;

		CShop();
		~CShop();

		bool	Create(DWORD dwVnum, DWORD dwNPCVnum, TShopItemTable * pItemTable);
		void	SetShopItems(TShopItemTable * pItemTable, short bItemCount);

		virtual void	SetPCShop(LPCHARACTER ch);
		virtual bool	IsPCShop()	{ return m_pkPC ? true : false; }

		// 게스트 추가/삭제
#if defined(ENABLE_REMOTE_SHOP)
		virtual bool	AddGuest(LPCHARACTER ch, DWORD owner_vid, bool bOtherEmpire, bool uzak_market = false);
#else
		virtual bool	AddGuest(LPCHARACTER ch, DWORD owner_vid, bool bOtherEmpire);
#endif
		void	RemoveGuest(LPCHARACTER ch);
		void	RemoveAllGuests();

		// 물건 구입
		virtual int	Buy(LPCHARACTER ch, BYTE pos);

		// 게스트에게 패킷을 보냄
		void	BroadcastUpdateItem(BYTE pos);

		// 판매중인 아이템의 갯수를 알려준다.
		int		GetNumberByVnum(DWORD dwVnum);

		// 아이템이 상점에 등록되어 있는지 알려준다.
		virtual bool	IsSellingItem(DWORD itemID);

		DWORD	GetVnum() { return m_dwVnum; }
		DWORD	GetNPCVnum() { return m_dwNPCVnum; }

	protected:
		void	Broadcast(const void * data, int bytes);

	protected:
		DWORD				m_dwVnum;
		DWORD				m_dwNPCVnum;

		CGrid *				m_pGrid;

		typedef TR1_NS::unordered_map<LPCHARACTER, bool> GuestMapType;
		GuestMapType m_map_guest;
		std::vector<SHOP_ITEM>		m_itemVector;	// 이 상점에서 취급하는 물건들

		LPCHARACTER			m_pkPC;
};

#endif 
