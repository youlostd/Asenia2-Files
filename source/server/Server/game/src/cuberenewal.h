#pragma once
#define CUBE_MAX_NUM	24	// OLD:INVENTORY_MAX_NUM
#define CUBE_MAX_DISTANCE	1000


struct CUBE_RENEWAL_VALUE
{
	DWORD	vnum;
	int		count;

	bool operator == (const CUBE_RENEWAL_VALUE& b)
	{
		return (this->count == b.count) && (this->vnum == b.vnum);
	}
};

struct CUBE_RENEWAL_DATA
{
	std::vector<WORD>		npc_vnum;
	std::vector<CUBE_RENEWAL_VALUE>	item;
	std::vector<CUBE_RENEWAL_VALUE>	reward;
	std::string								category;
	int						percent;
#ifdef ENABLE_YANG_LIMIT_SYSTEM
	long long			gold;		// Á¦Á¶½Ã ÇÊ¿äÇÑ ±Ý¾×
#else
	unsigned int			cheque;		// Á¦Á¶½Ã ÇÊ¿äÇÑ ±Ý¾×
#endif
#ifdef ENABLE_GAYA_SYSTEM
	unsigned int			gem;
	unsigned int			gem2;
#endif
#ifdef ENABLE_CUBE_ATTR_SOCKET
	bool				allowCopyAttr;
#endif
	CUBE_RENEWAL_DATA();

	bool		can_make_item(LPITEM* items, WORD npc_vnum);
}; 


void Cube_init ();
bool Cube_load (const char *file);
bool Cube_InformationInitialize();
void Cube_open (LPCHARACTER ch);
void Cube_close(LPCHARACTER ch);
void Cube_Make(LPCHARACTER ch, int index, int count_item, int index_item_improve);
void SendDateCubeRenewalPackets(LPCHARACTER ch, BYTE subheader, DWORD npcVNUM = 0);