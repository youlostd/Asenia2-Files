#pragma once
class COfflineShopManager : public singleton<COfflineShopManager>
{
	public:
		typedef std::map<DWORD, COfflineShop *> TShopMap;
		typedef std::map<DWORD, DWORD> TOfflineShopMap;

	public:
		COfflineShopManager();
		~COfflineShopManager();

		bool StartShopping(LPCHARACTER pkChr, LPCHARACTER pkChrShopKeeper);
		void StopShopping(LPCHARACTER ch);

		void Buy(LPCHARACTER ch, BYTE bPos);

		LPOFFLINESHOP	CreateOfflineShop(LPCHARACTER npc, DWORD dwOwnerPID);
		LPOFFLINESHOP	FindOfflineShop(DWORD dwVID);
		void			DestroyOfflineShop(LPCHARACTER ch, DWORD dwVID, bool bDestroyAll = false);
		void			Giveback(LPCHARACTER ch);
		void			Giveback2(LPCHARACTER ch);
		void			Giveback3(LPCHARACTER ch);
		
		void			AddItem(LPCHARACTER ch, TItemPos tDisplayPos, BYTE bPos, int iPrice, int iPriceCheque);
		void			RemoveItem(LPCHARACTER ch, BYTE bPos);
		void			ChangePrice(LPCHARACTER ch, BYTE bPos, DWORD dwPrice, DWORD dwPriceCheque);
		void			Refresh(LPCHARACTER ch);
		void			RefreshMoney(LPCHARACTER ch);

		DWORD			FindMyOfflineShop(DWORD dwPID);
		void			ChangeOfflineShopTime(LPCHARACTER ch, BYTE bTime);
		void			WithdrawMoney(LPCHARACTER ch, DWORD dwRequiredMoney);
		void			WithdrawCheque(LPCHARACTER ch, DWORD dwRequiredCheque);
		short			LeftItemCount(LPCHARACTER ch);

	private:
		TOfflineShopMap	m_Map_pkOfflineShopByNPC2;
		TShopMap		m_map_pkOfflineShopByNPC;
};

