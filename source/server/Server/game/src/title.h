// #ifdef ENABLE_TITLE_SYSTEM
#ifndef __INC_METIN_II_GAME_TITLE_SYSTEM_H__
#define __INC_METIN_II_GAME_TITLE_SYSTEM_H__

#pragma once

class TitleManager : public singleton<TitleManager>
{
	public:
		TitleManager();
		~TitleManager();
	
	bool	TransformTitle(LPCHARACTER ch);	
	bool	UpdateTitle(LPCHARACTER ch, int changeTitle, int changeMoney);	
	bool	SetTitle(LPCHARACTER ch, const char* pTitle);	
	void	BuyPotion(LPCHARACTER ch, const char* mPotion);
	void	SetAffect(LPCHARACTER ch, const char* valueAffect);	
};
#endif
// #endif