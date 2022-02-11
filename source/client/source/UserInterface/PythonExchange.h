#pragma once

#include "Packet.h"

/*
 *	��ȯ â ����
 */
class CPythonExchange : public CSingleton<CPythonExchange>
{
	public:
		enum
		{
#ifdef ENABLE_NEW_EXCHANGE_WINDOW
			EXCHANGE_ITEM_MAX_NUM = 24,
#else
			EXCHANGE_ITEM_MAX_NUM = 12,
#endif
		};

		typedef struct trade
		{
			char					name[CHARACTER_NAME_MAX_LEN + 1];

			DWORD					item_vnum[EXCHANGE_ITEM_MAX_NUM];
#ifdef ENABLE_EXTENDED_ITEM_COUNT
			short					item_count[EXCHANGE_ITEM_MAX_NUM];
#else
			BYTE					item_count[EXCHANGE_ITEM_MAX_NUM];
#endif
			DWORD					item_metin[EXCHANGE_ITEM_MAX_NUM][ITEM_SOCKET_SLOT_MAX_NUM];
			TPlayerItemAttribute	item_attr[EXCHANGE_ITEM_MAX_NUM][ITEM_ATTRIBUTE_SLOT_MAX_NUM];
			DWORD					item_evolution[EXCHANGE_ITEM_MAX_NUM];

			BYTE					accept;
#ifdef YANG_LIMIT
			ULDWORD					elk;
#else
			DWORD					elk;
#endif
#ifdef ENABLE_CHEQUE_SYSTEM
			DWORD					won;
#endif

#ifdef ENABLE_NEW_EXCHANGE_WINDOW
			DWORD	race;
			DWORD	level;
#endif

		} TExchangeData;

	public:
		CPythonExchange();
		virtual ~CPythonExchange();

		void			Clear();

		void			Start();
		void			End();
		bool			isTrading();

		// Interface

		void			SetSelfName(const char *name);
		void			SetTargetName(const char *name);

		char			*GetNameFromSelf();
		char			*GetNameFromTarget();

#ifdef YANG_LIMIT
		void			SetElkToTarget(ULDWORD elk);
		void			SetElkToSelf(ULDWORD elk);
		ULDWORD			GetElkFromTarget();
		ULDWORD			GetElkFromSelf();
#else
		void			SetElkToTarget(DWORD elk);
		void			SetElkToSelf(DWORD elk);
		DWORD			GetElkFromTarget();
		DWORD			GetElkFromSelf();
#endif

#ifdef ENABLE_CHEQUE_SYSTEM
		void			SetWonToTarget(DWORD won);
		void			SetWonToSelf(DWORD won);

		DWORD			GetWonFromTarget();
		DWORD			GetWonFromSelf();
#endif

#ifdef ENABLE_NEW_EXCHANGE_WINDOW
		void	SetSelfRace(DWORD race);
		void	SetTargetRace(DWORD race);
		DWORD	GetRaceFromSelf();
		DWORD	GetRaceFromTarget();
		void	SetSelfLevel(DWORD level);
		void	SetTargetLevel(DWORD level);
		DWORD	GetLevelFromSelf();
		DWORD	GetLevelFromTarget();
#endif

#ifdef ENABLE_EXTENDED_ITEM_COUNT
		void			SetItemToTarget(DWORD pos, DWORD vnum, short count);
		void			SetItemToSelf(DWORD pos, DWORD vnum, short count);
#else
		void			SetItemToTarget(DWORD pos, DWORD vnum, BYTE count);
		void			SetItemToSelf(DWORD pos, DWORD vnum, BYTE count);
#endif

		void			SetItemMetinSocketToTarget(int pos, int imetinpos, DWORD vnum);
		void			SetItemMetinSocketToSelf(int pos, int imetinpos, DWORD vnum);

		void			SetItemAttributeToTarget(int pos, int iattrpos, BYTE byType, short sValue);
		void			SetItemAttributeToSelf(int pos, int iattrpos, BYTE byType, short sValue);

		void			SetItemEvolutionToTarget(int pos, DWORD evolution);
		void			SetItemEvolutionToSelf(int pos, DWORD evolution);
		
		DWORD			GetItemEvolutionFromTarget(int pos);
		DWORD			GetItemEvolutionFromSelf(int pos);

		void			DelItemOfTarget(BYTE pos);
		void			DelItemOfSelf(BYTE pos);

		DWORD			GetItemVnumFromTarget(BYTE pos);
		DWORD			GetItemVnumFromSelf(BYTE pos);

#ifdef ENABLE_EXTENDED_ITEM_COUNT
		short			GetItemCountFromTarget(BYTE pos);
		short			GetItemCountFromSelf(BYTE pos);
#else
		BYTE			GetItemCountFromTarget(BYTE pos);
		BYTE			GetItemCountFromSelf(BYTE pos);
#endif

		DWORD			GetItemMetinSocketFromTarget(BYTE pos, int iMetinSocketPos);
		DWORD			GetItemMetinSocketFromSelf(BYTE pos, int iMetinSocketPos);

		void			GetItemAttributeFromTarget(BYTE pos, int iAttrPos, BYTE * pbyType, short * psValue);
		void			GetItemAttributeFromSelf(BYTE pos, int iAttrPos, BYTE * pbyType, short * psValue);

		void			SetAcceptToTarget(BYTE Accept);
		void			SetAcceptToSelf(BYTE Accept);

		bool			GetAcceptFromTarget();
		bool			GetAcceptFromSelf();

		bool			GetElkMode();
		void			SetElkMode(bool value);

	protected:
		bool				m_isTrading;

		bool				m_elk_mode;   // ��ũ�� Ŭ���ؼ� ��ȯ�������� ���� ������.
		TExchangeData		m_self;
		TExchangeData		m_victim;
};
