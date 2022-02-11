#ifndef __INC_METIN_II_GAME_EXCHANGE_H__
#define __INC_METIN_II_GAME_EXCHANGE_H__

class CGrid;

enum EExchangeValues
{
#ifdef __NEW_EXCHANGE_WINDOW__
	EXCHANGE_ITEM_MAX_NUM = 24,
#else
	EXCHANGE_ITEM_MAX_NUM = 12,
#endif
	EXCHANGE_MAX_DISTANCE	= 1000
};

class CExchange
{
	public:
		CExchange(LPCHARACTER pOwner);
		~CExchange();

		bool		Accept(bool bIsAccept = true);
		void		Cancel();

#ifdef YANG_LIMIT
		bool		AddGold(LDWORD lGold);
#else
		bool		AddGold(long lGold);
#endif
#ifdef ENABLE_CHEQUE_SYSTEM
		bool		AddCheque(long lCheque);
#endif
#if defined(ITEM_CHECKINOUT_UPDATE)
		int			GetEmptyExchange(BYTE size);
		bool		AddItem(TItemPos item_pos, BYTE display_pos, bool SelectPosAuto);
#else
		bool		AddItem(TItemPos item_pos, BYTE display_pos);
#endif
		bool		RemoveItem(BYTE pos);

		LPCHARACTER	GetOwner()	{ return m_pOwner;	}
		CExchange *	GetCompany()	{ return m_pCompany;	}

		bool		GetAcceptStatus() { return m_bAccept; }

		void		SetCompany(CExchange * pExchange)	{ m_pCompany = pExchange; }

	private:
		bool		Done();
		bool		Check(int * piItemCount);
		int			CheckSpaceUpgrade();
		int			CheckSpaceStone();
		int			CheckSpaceChest();
		int			CheckSpaceAttr();
		bool		CheckSpace();

	private:
		CExchange *	m_pCompany;	// 상대방의 CExchange 포인터

		LPCHARACTER	m_pOwner;

		TItemPos		m_aItemPos[EXCHANGE_ITEM_MAX_NUM];
		LPITEM		m_apItems[EXCHANGE_ITEM_MAX_NUM];
		BYTE		m_abItemDisplayPos[EXCHANGE_ITEM_MAX_NUM];

		bool 		m_bAccept;
#ifdef YANG_LIMIT
		LDWORD		m_lGold;
#else
		long		m_lGold;
#endif
#ifdef ENABLE_CHEQUE_SYSTEM
		long		m_lCheque;
#endif
		CGrid *		m_pGrid;

};

#endif
