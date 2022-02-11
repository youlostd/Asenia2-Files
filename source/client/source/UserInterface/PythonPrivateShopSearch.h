#pragma once

class CPythonPrivateShopSearch : public CSingleton<CPythonPrivateShopSearch>
{
	public:
		struct TSearchItemData : TItemData
		{
			DWORD vid;
			DWORD ownerId;
			char ownerName[CHARACTER_NAME_MAX_LEN + 1];
			char itemName[24 + 1];
			DWORD price;
			DWORD price_cheque;
			BYTE Cell;
			int page;
		};

		using TItemInstanceVector =  std::vector<TSearchItemData> ;

	public:
		CPythonPrivateShopSearch();
		virtual ~CPythonPrivateShopSearch();

		void AddItemData(const TSearchItemData& rItemData);
		void ClearItemData();
		void ResultFilterSelectedPage(int page);

		DWORD GetItemDataCount()
		{
			return m_ItemInstanceVector.size();
		}

		DWORD GetItem10DataCount()
		{
			return m_ItemInstance10Vector.size();
		}
		DWORD GetItemDataPtr (DWORD index, TSearchItemData** ppInstance);

	protected:
		TItemInstanceVector m_ItemInstanceVector;
		TItemInstanceVector m_ItemInstance10Vector;
};