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
			DWORD	vnum;		// ������ ��ȣ
			long	price;		// ����
#ifdef ENABLE_CHEQUE_SYSTEM
			long	price_cheque;
#endif
#ifdef PRICE_TYPE
			long	price_type;	
#endif
#ifdef __EXTENDED_ITEM_COUNT__
			short count;
#else
			BYTE count; // ������ ����
#endif

			LPITEM	pkItem;
			int		itemid;		// ������ �������̵�

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

		// �Խ�Ʈ �߰�/����
#if defined(ENABLE_REMOTE_SHOP)
		virtual bool	AddGuest(LPCHARACTER ch, DWORD owner_vid, bool bOtherEmpire, bool uzak_market = false);
#else
		virtual bool	AddGuest(LPCHARACTER ch, DWORD owner_vid, bool bOtherEmpire);
#endif
		void	RemoveGuest(LPCHARACTER ch);
		void	RemoveAllGuests();

		// ���� ����
		virtual int	Buy(LPCHARACTER ch, BYTE pos);

		// �Խ�Ʈ���� ��Ŷ�� ����
		void	BroadcastUpdateItem(BYTE pos);

		// �Ǹ����� �������� ������ �˷��ش�.
		int		GetNumberByVnum(DWORD dwVnum);

		// �������� ������ ��ϵǾ� �ִ��� �˷��ش�.
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
		std::vector<SHOP_ITEM>		m_itemVector;	// �� �������� ����ϴ� ���ǵ�

		LPCHARACTER			m_pkPC;
};

#endif 
