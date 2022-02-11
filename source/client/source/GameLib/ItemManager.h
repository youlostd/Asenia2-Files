#pragma once

#include "ItemData.h"
#ifdef ENABLE_DS_SET
#include "DragonSoulTable.h"
#endif

class CItemManager : public CSingleton<CItemManager>
{
	public:
		enum EItemDescCol
		{
			ITEMDESC_COL_VNUM,
			ITEMDESC_COL_NAME,						
			ITEMDESC_COL_DESC,
			ITEMDESC_COL_SUMM,
			ITEMDESC_COL_Kirmizi,
			ITEMDESC_COL_Mavi,
			ITEMDESC_COL_Yesil,
			ITEMDESC_COL_Sari,
			ITEMDESC_COL_Turuncu,
			ITEMDESC_COL_Pembe,
			ITEMDESC_COL_Mor,
			ITEMDESC_COL_NUM,
		};
		#ifdef ENABLE_SASH_SYSTEM
		enum EItemScaleColumn
		{
			ITEMSCALE_VNUM,
			ITEMSCALE_JOB,
			ITEMSCALE_SEX,
			ITEMSCALE_SCALE_X,
			ITEMSCALE_SCALE_Y,
			ITEMSCALE_SCALE_Z,
			ITEMSCALE_POSITION_X,
			ITEMSCALE_POSITION_Y,
			ITEMSCALE_POSITION_Z,
			ITEMSCALE_NUM,
		};
		#endif
	public:
		typedef std::map<DWORD, CItemData*> TItemMap;
		typedef std::map<std::string, CItemData*> TItemNameMap;

	public:
		CItemManager();
		virtual ~CItemManager();
		
		void			Destroy();

		BOOL			SelectItemData(DWORD dwIndex);
		CItemData *		GetSelectedItemDataPointer();

		BOOL			GetItemDataPointer(DWORD dwItemID, CItemData ** ppItemData);
		
		/////
		bool			LoadItemDesc(const char * c_szFileName);
		bool			LoadItemList(const char * c_szFileName);
		bool			LoadItemTable(const char * c_szFileName);
#ifdef ENABLE_SHINING_SYSTEM
		bool			LoadShiningTable(const char * szShiningTable);
#endif
		CItemData * 	MakeItemData(DWORD dwIndex);
#ifdef __ENABLE_NEW_OFFLINESHOP__
		void			GetItemsNameMap(std::map<DWORD, std::string>& inMap);
#endif
		#ifdef ENABLE_SASH_SYSTEM
		bool	LoadItemScale(const char* c_szFileName);
		#endif
#ifdef ENABLE_DS_SET
	public:
		bool LoadDragonSoulTable(const char* szDragonSoulTable);
		CDragonSoulTable* GetDragonSoulTable();

	protected:
		CDragonSoulTable* m_DragonSoulTable;
#endif
	protected:
		TItemMap m_ItemMap;
		std::vector<CItemData*>  m_vec_ItemRange;
		CItemData * m_pSelectedItemData;		
};
