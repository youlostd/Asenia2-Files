#include <stdio.h>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include "lzo.h"
#include "CsvFile.h"
#include "ItemCSVReader.h"

#pragma comment(lib, "lzo.lib")

using namespace std;

enum EMisc
{
	CHARACTER_NAME_MAX_LEN = 24,
	MOB_SKILL_MAX_NUM = 5,
};

enum EMobEnchants
{
	MOB_ENCHANT_CURSE,
	MOB_ENCHANT_SLOW,
	MOB_ENCHANT_POISON,
	MOB_ENCHANT_STUN,
	MOB_ENCHANT_CRITICAL,
	MOB_ENCHANT_PENETRATE,
	MOB_ENCHANTS_MAX_NUM,
};

enum EMobResists
{
	MOB_RESIST_SWORD,
	MOB_RESIST_TWOHAND,
	MOB_RESIST_DAGGER,
	MOB_RESIST_BELL,
	MOB_RESIST_FAN,
	MOB_RESIST_BOW,
	MOB_RESIST_FIRE,
	MOB_RESIST_ELECT,
	MOB_RESIST_MAGIC,
	MOB_RESIST_WIND,
	MOB_RESIST_POISON,
	MOB_RESISTS_MAX_NUM,
};

enum EItemMisc
{
	ITEM_NAME_MAX_LEN = 30,
	ITEM_VALUES_MAX_NUM = 6,
	ITEM_LIMIT_MAX_NUM = 2,
	ITEM_APPLY_MAX_NUM = 5,
	ITEM_SOCKET_MAX_NUM = 3,
};

#pragma pack(1)
typedef struct
{
	uint32_t dwVnum;
	uint8_t bLevel;
}TMobSkillLevel;

typedef struct
{
	uint32_t dwVnum;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	char szLocaleName[CHARACTER_NAME_MAX_LEN + 1];

	uint8_t bType;
	uint8_t bRank;
	uint8_t bBattleType;
	uint8_t bLevel;
	uint8_t bSize;

	uint32_t dwGoldMin;
	uint32_t dwGoldMax;
	uint32_t dwExp;
	uint32_t dwMaxHP;
	uint8_t bRegenCycle;
	uint8_t bRegenPercent;
	uint16_t wDef;

	uint32_t dwAIFlag;
	uint32_t dwRaceFlag;
	uint32_t dwImmuneFlag;

	uint8_t bStr, bDex, bCon, bInt;
	uint32_t dwDamageRange[2];

	int16_t sAttackSpeed;
	int16_t sMovingSpeed;
	uint8_t bAggresiveHPPct;
	uint16_t wAggressiveSight;
	uint16_t wAttackRange;

	char cEnchants[MOB_ENCHANTS_MAX_NUM];
	char cResists[MOB_RESISTS_MAX_NUM];

	uint32_t dwResurrectionVnum;
	uint32_t dwDropItemVnum;

	uint8_t bMountCapacity;
	uint8_t bOnClickType;

	uint8_t bEmpire;
	char szFolder[64 + 1];

	float fDamMultiply;

	uint32_t dwSummonVnum;
	uint32_t dwDrainSP;
	uint32_t dwMobColor;
	uint32_t dwPolymorphItemVnum;

	TMobSkillLevel Skills[MOB_SKILL_MAX_NUM];

	uint8_t bBerserkPoint;
	uint8_t bStoneSkinPoint;
	uint8_t bGodSpeedPoint;
	uint8_t bDeathBlowPoint;
	uint8_t bRevivePoint;
}TMobTable;

typedef struct
{
	uint8_t bType;
	int32_t lValue;
} TItemLimit;

typedef struct
{
	uint8_t bType;
	int32_t lValue;
} TItemApply;

typedef struct
{
	uint32_t dwVnum;
	uint32_t dwVnumRange;
	char szName[ITEM_NAME_MAX_LEN + 1];
	char szLocaleName[ITEM_NAME_MAX_LEN + 1];
	uint8_t bType;
	uint8_t bSubType;

	uint8_t bWeight;
	uint8_t bSize;

	uint32_t dwAntiFlags;
	uint32_t dwFlags;
	uint32_t dwWearFlags;
	uint32_t dwImmuneFlag;

	uint32_t dwGold;
	uint32_t dwShopBuyPrice;

	TItemLimit aLimits[ITEM_LIMIT_MAX_NUM];
	TItemApply aApplies[ITEM_APPLY_MAX_NUM];
	int32_t alValues[ITEM_VALUES_MAX_NUM];
	int32_t alSockets[ITEM_SOCKET_MAX_NUM];
	uint32_t dwRefinedVnum;
	uint16_t wRefineSet;
	uint8_t bAlterToMagicItemPct;
	uint8_t bSpecular;
	uint8_t bGainSocketPct;
	uint8_t bMaskType;
	uint8_t bMaskSubType;
} TClientItemTable;
#pragma pack()

bool operator < (const TClientItemTable& lhs, const TClientItemTable& rhs)
{
	return lhs.dwVnum < rhs.dwVnum;
}

TMobTable * m_pMobTable = nullptr;
int32_t m_iMobTableSize = 0;

TClientItemTable * m_pItemTable = nullptr;
int32_t m_iItemTableSize = 0;

bool Set_Proto_Mob_Table(TMobTable * mobTable, cCsvTable& csvTable, std::map<int32_t, const char * >& nameMap)
{
	int32_t col = 0;
	mobTable->dwVnum = atoi(csvTable.AsStringByIndex(col++));
	strncpy(mobTable->szName, csvTable.AsStringByIndex(col++), CHARACTER_NAME_MAX_LEN);

	auto it = nameMap.find(mobTable->dwVnum);
	if (it != nameMap.end())
	{
		const char * localeName = it->second;
		strncpy(mobTable->szLocaleName, localeName, sizeof(mobTable->szLocaleName));
	}
	else
		strncpy(mobTable->szLocaleName, mobTable->szName, sizeof(mobTable->szLocaleName));

	int32_t rankValue = get_Mob_Rank_Value(csvTable.AsStringByIndex(col++));
	mobTable->bRank = rankValue;

	int32_t typeValue = get_Mob_Type_Value(csvTable.AsStringByIndex(col++));
	mobTable->bType = typeValue;

	int32_t battleTypeValue = get_Mob_BattleType_Value(csvTable.AsStringByIndex(col++));
	mobTable->bBattleType = battleTypeValue;

	mobTable->bLevel = atoi(csvTable.AsStringByIndex(col++));

	int32_t sizeValue = get_Mob_Size_Value(csvTable.AsStringByIndex(col++));
	mobTable->bSize = sizeValue;

	int32_t aiFlagValue = get_Mob_AIFlag_Value(csvTable.AsStringByIndex(col++));
	mobTable->dwAIFlag = aiFlagValue;
	col++;

	int32_t raceFlagValue = get_Mob_RaceFlag_Value(csvTable.AsStringByIndex(col++));
	mobTable->dwRaceFlag = raceFlagValue;

	int32_t immuneFlagValue = get_Mob_ImmuneFlag_Value(csvTable.AsStringByIndex(col++));
	mobTable->dwImmuneFlag = immuneFlagValue;

	mobTable->bEmpire = atoi(csvTable.AsStringByIndex(col++));

	strncpy(mobTable->szFolder, csvTable.AsStringByIndex(col++), sizeof(mobTable->szFolder));

	mobTable->bOnClickType = atoi(csvTable.AsStringByIndex(col++));

	mobTable->bStr = atoi(csvTable.AsStringByIndex(col++));
	mobTable->bDex = atoi(csvTable.AsStringByIndex(col++));
	mobTable->bCon = atoi(csvTable.AsStringByIndex(col++));
	mobTable->bInt = atoi(csvTable.AsStringByIndex(col++));
	mobTable->dwDamageRange[0] = atoi(csvTable.AsStringByIndex(col++));
	mobTable->dwDamageRange[1] = atoi(csvTable.AsStringByIndex(col++));
	mobTable->dwMaxHP = atoi(csvTable.AsStringByIndex(col++));
	mobTable->bRegenCycle = atoi(csvTable.AsStringByIndex(col++));
	mobTable->bRegenPercent = atoi(csvTable.AsStringByIndex(col++));

	col++;
	col++;
	mobTable->dwExp = atoi(csvTable.AsStringByIndex(col++));
	mobTable->wDef = atoi(csvTable.AsStringByIndex(col++));
	mobTable->sAttackSpeed = atoi(csvTable.AsStringByIndex(col++));
	mobTable->sMovingSpeed = atoi(csvTable.AsStringByIndex(col++));
	mobTable->bAggresiveHPPct = atoi(csvTable.AsStringByIndex(col++));
	mobTable->wAggressiveSight = atoi(csvTable.AsStringByIndex(col++));
	mobTable->wAttackRange = atoi(csvTable.AsStringByIndex(col++));

	mobTable->dwDropItemVnum = atoi(csvTable.AsStringByIndex(col++));
	col++;

	for (int32_t i = 0; i < MOB_ENCHANTS_MAX_NUM; ++i)
		mobTable->cEnchants[i] = atoi(csvTable.AsStringByIndex(col++));

	for (int32_t i = 0; i < MOB_RESISTS_MAX_NUM; ++i)
		mobTable->cResists[i] = atoi(csvTable.AsStringByIndex(col++));

	mobTable->fDamMultiply = (float)atoi(csvTable.AsStringByIndex(col++));
	mobTable->dwSummonVnum = atoi(csvTable.AsStringByIndex(col++));
	mobTable->dwDrainSP = atoi(csvTable.AsStringByIndex(col++));
	mobTable->dwMobColor = atoi(csvTable.AsStringByIndex(col++));

	return true;
}

bool BuildMobTable()
{
	bool isNameFile = true;
	map<int32_t, const char * > localMap;
	cCsvTable nameData;

	if (!nameData.Load("mob_names.txt", '\t'))
	{
		fprintf(stderr, "mob_names.txt 파일을 읽어오지 못했습니다\n");
		isNameFile = false;
	}
	else
	{
		nameData.Next();
		while (nameData.Next())
			localMap[atoi(nameData.AsStringByIndex(0))] = nameData.AsStringByIndex(1);
	}

	set<int32_t> vnumSet;

	map<uint32_t, TMobTable * > test_map_mobTableByVnum;

	cCsvTable test_data;
	if (test_data.Load("mob_proto_test.txt", '\t'))
	{
		test_data.Next();

		TMobTable * test_mob_table = nullptr;
		int32_t test_MobTableSize = test_data.m_File.GetRowCount() - 1;
		test_mob_table = new TMobTable[test_MobTableSize];
		memset(test_mob_table, 0, sizeof(TMobTable) * test_MobTableSize);

		while (test_data.Next())
		{
			if (!Set_Proto_Mob_Table(test_mob_table, test_data, localMap))
				fprintf(stderr, "몹 프로토 테이블 셋팅 실패.\n");

			test_map_mobTableByVnum.insert(std::map<uint32_t, TMobTable * >::value_type(test_mob_table->dwVnum, test_mob_table));
			++test_mob_table;
		}
	}

	cCsvTable data;
	if (!data.Load("mob_proto.txt", '\t'))
	{
		fprintf(stderr, "mob_proto.txt 파일을 읽어오지 못했습니다\n");
		return false;
	}
	data.Next();

	if (m_pMobTable)
	{
		delete m_pMobTable;
		m_pMobTable = nullptr;
	}

	int32_t addNumber = 0;
	while (data.Next())
	{
		int32_t vnum = atoi(data.AsStringByIndex(0));

		auto it_map_mobTable = test_map_mobTableByVnum.find(vnum);
		if (it_map_mobTable != test_map_mobTableByVnum.end())
			addNumber++;
	}

	m_iMobTableSize = data.m_File.GetRowCount() - 1 + addNumber;

	m_pMobTable = new TMobTable[m_iMobTableSize];
	memset(m_pMobTable, 0, sizeof(TMobTable) * m_iMobTableSize);
	TMobTable * mob_table = m_pMobTable;

	data.Destroy();
	if (!data.Load("mob_proto.txt", '\t'))
	{
		fprintf(stderr, "mob_proto.txt 파일을 읽어오지 못했습니다\n");
		return false;
	}
	data.Next();

	while (data.Next())
	{
		int32_t col = 0;

		auto it_map_mobTable = test_map_mobTableByVnum.find(atoi(data.AsStringByIndex(col)));
		if (it_map_mobTable == test_map_mobTableByVnum.end())
		{
			if (!Set_Proto_Mob_Table(mob_table, data, localMap))
				fprintf(stderr, "몹 프로토 테이블 셋팅 실패.\n");
		}
		else
		{
			TMobTable * tempTable = it_map_mobTable->second;
			mob_table->dwVnum = tempTable->dwVnum;
			strncpy(mob_table->szName, tempTable->szName, CHARACTER_NAME_MAX_LEN);
			strncpy(mob_table->szLocaleName, tempTable->szLocaleName, CHARACTER_NAME_MAX_LEN);
			mob_table->bRank = tempTable->bRank;
			mob_table->bType = tempTable->bType;
			mob_table->bBattleType = tempTable->bBattleType;
			mob_table->bLevel = tempTable->bLevel;
			mob_table->bSize = tempTable->bSize;
			mob_table->dwAIFlag = tempTable->dwAIFlag;
			mob_table->dwRaceFlag = tempTable->dwRaceFlag;
			mob_table->dwImmuneFlag = tempTable->dwImmuneFlag;
			mob_table->bEmpire = tempTable->bEmpire;
			strncpy(mob_table->szFolder, tempTable->szFolder, CHARACTER_NAME_MAX_LEN);
			mob_table->bOnClickType = tempTable->bOnClickType;
			mob_table->bStr = tempTable->bStr;
			mob_table->bDex = tempTable->bDex;
			mob_table->bCon = tempTable->bCon;
			mob_table->bInt = tempTable->bInt;
			mob_table->dwDamageRange[0] = tempTable->dwDamageRange[0];
			mob_table->dwDamageRange[1] = tempTable->dwDamageRange[1];
			mob_table->dwMaxHP = tempTable->dwMaxHP;
			mob_table->bRegenCycle = tempTable->bRegenCycle;
			mob_table->bRegenPercent = tempTable->bRegenPercent;
			mob_table->dwExp = tempTable->dwExp;
			mob_table->wDef = tempTable->wDef;
			mob_table->sAttackSpeed = tempTable->sAttackSpeed;
			mob_table->sMovingSpeed = tempTable->sMovingSpeed;
			mob_table->bAggresiveHPPct = tempTable->bAggresiveHPPct;
			mob_table->wAggressiveSight = tempTable->wAggressiveSight;
			mob_table->wAttackRange = tempTable->wAttackRange;
			mob_table->dwDropItemVnum = tempTable->dwDropItemVnum;
			for (int32_t i = 0; i < MOB_ENCHANTS_MAX_NUM; ++i)
				mob_table->cEnchants[i] = tempTable->cEnchants[i];
			for (int32_t i = 0; i < MOB_RESISTS_MAX_NUM; ++i)
				mob_table->cResists[i] = tempTable->cResists[i];
			mob_table->fDamMultiply = tempTable->fDamMultiply;
			mob_table->dwSummonVnum = tempTable->dwSummonVnum;
			mob_table->dwDrainSP = tempTable->dwDrainSP;
			mob_table->dwMobColor = tempTable->dwMobColor;
		}

		vnumSet.insert(mob_table->dwVnum);
		++mob_table;
	}

	test_data.Destroy();
	if (test_data.Load("mob_proto_test.txt", '\t'))
	{
		test_data.Next();

		while (test_data.Next())
		{
			auto itVnum = vnumSet.find(atoi(test_data.AsStringByIndex(0)));
			if (itVnum != vnumSet.end())
				continue;

			if (!Set_Proto_Mob_Table(mob_table, test_data, localMap))
				fprintf(stderr, "몹 프로토 테이블 셋팅 실패.\n");

			vnumSet.insert(mob_table->dwVnum);
			++mob_table;
		}
	}
	return true;
}

uint32_t g_adwMobProtoKey[4] =
{
	4813894,
	18955,
	552631,
	6822045
};

void SaveMobProto()
{
	FILE * fp;
	fopen_s(&fp, "mob_proto", "wb");

	if (!fp)
	{
		printf("cannot open %s for writing\n", "mob_proto");
		return;
	}

	uint32_t fourcc = MAKEFOURCC('M', 'M', 'P', 'T');
	fwrite(&fourcc, sizeof(uint32_t), 1, fp);

	uint32_t dwElements = m_iMobTableSize;
	fwrite(&dwElements, sizeof(uint32_t), 1, fp);

	CLZObject zObj;

	if (!CLZO::instance().CompressEncryptedMemory(zObj, m_pMobTable, sizeof(TMobTable) * m_iMobTableSize, g_adwMobProtoKey))
	{
		printf("cannot compress\n");
		fclose(fp);
		return;
	}

	const CLZObject::THeader& r = zObj.GetHeader();

	uint32_t dwDataSize = zObj.GetSize();
	fwrite(&dwDataSize, sizeof(uint32_t), 1, fp);
	fwrite(zObj.GetBuffer(), dwDataSize, 1, fp);

	fclose(fp);
}

bool Set_Proto_Item_Table(TClientItemTable * itemTable, cCsvTable& csvTable, std::map<int32_t, const char * >& nameMap)
{
	{
		std::string s(csvTable.AsStringByIndex(0));
		int32_t pos = s.find("~");

		if (std::string::npos == pos)
		{
			itemTable->dwVnum = atoi(s.c_str());
			if (0 == itemTable->dwVnum)
			{
				printf("INVALID VNUM %s\n", s.c_str());
				return false;
			}
			itemTable->dwVnumRange = 0;
		}
		else
		{
			std::string s_start_vnum(s.substr(0, pos));
			std::string s_end_vnum(s.substr(pos + 1));

			int32_t start_vnum = atoi(s_start_vnum.c_str());
			int32_t end_vnum = atoi(s_end_vnum.c_str());
			if (0 == start_vnum || (0 != end_vnum && end_vnum < start_vnum))
			{
				printf("INVALID VNUM RANGE%s\n", s.c_str());
				return false;
			}
			itemTable->dwVnum = start_vnum;
			itemTable->dwVnumRange = end_vnum - start_vnum;
		}
	}

	int32_t col = 1;

	strncpy(itemTable->szName, csvTable.AsStringByIndex(col++), ITEM_NAME_MAX_LEN);
	auto it = nameMap.find(itemTable->dwVnum);
	if (it != nameMap.end())
	{
		const char * localeName = it->second;
		strncpy(itemTable->szLocaleName, localeName, sizeof(itemTable->szLocaleName));
	}
	else
		strncpy(itemTable->szLocaleName, itemTable->szName, sizeof(itemTable->szLocaleName));

	itemTable->bType = get_Item_Type_Value(csvTable.AsStringByIndex(col++));
	itemTable->bSubType = get_Item_SubType_Value(itemTable->bType, csvTable.AsStringByIndex(col++));
	itemTable->bSize = atoi(csvTable.AsStringByIndex(col++));
	itemTable->dwAntiFlags = get_Item_AntiFlag_Value(csvTable.AsStringByIndex(col++));
	itemTable->dwFlags = get_Item_Flag_Value(csvTable.AsStringByIndex(col++));
	itemTable->dwWearFlags = get_Item_WearFlag_Value(csvTable.AsStringByIndex(col++));
	itemTable->dwImmuneFlag = get_Item_Immune_Value(csvTable.AsStringByIndex(col++));
	itemTable->dwGold = atoi(csvTable.AsStringByIndex(col++));
	itemTable->dwShopBuyPrice = atoi(csvTable.AsStringByIndex(col++));
	itemTable->dwRefinedVnum = atoi(csvTable.AsStringByIndex(col++));
	itemTable->wRefineSet = atoi(csvTable.AsStringByIndex(col++));
	itemTable->bAlterToMagicItemPct = atoi(csvTable.AsStringByIndex(col++));

	int32_t i;

	for (i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		itemTable->aLimits[i].bType = get_Item_LimitType_Value(csvTable.AsStringByIndex(col++));
		itemTable->aLimits[i].lValue = atoi(csvTable.AsStringByIndex(col++));
	}

	for (i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
	{
		itemTable->aApplies[i].bType = get_Item_ApplyType_Value(csvTable.AsStringByIndex(col++));
		itemTable->aApplies[i].lValue = atoi(csvTable.AsStringByIndex(col++));
	}

	for (i = 0; i < ITEM_VALUES_MAX_NUM; ++i)
		itemTable->alValues[i] = atoi(csvTable.AsStringByIndex(col++));

	itemTable->bSpecular = atoi(csvTable.AsStringByIndex(col++));
	itemTable->bGainSocketPct = atoi(csvTable.AsStringByIndex(col++));
	col++;
	itemTable->bMaskType = get_Mask_Type_Value(csvTable.AsStringByIndex(col++));
	itemTable->bMaskSubType = get_Mask_SubType_Value(itemTable->bMaskType, csvTable.AsStringByIndex(col++));

	itemTable->bWeight = 0;

	return true;
}

bool BuildItemTable()
{
	bool isNameFile = true;
	map<int32_t, const char * > localMap;
	cCsvTable nameData;
	if (!nameData.Load("item_names.txt", '\t'))
	{
		fprintf(stderr, "item_names.txt 파일을 읽어오지 못했습니다\n");
		isNameFile = false;
	}
	else
	{
		nameData.Next();
		while (nameData.Next())
			localMap[atoi(nameData.AsStringByIndex(0))] = nameData.AsStringByIndex(1);
	}

	map<uint32_t, TClientItemTable * > test_map_itemTableByVnum;

	cCsvTable test_data;
	if (test_data.Load("item_proto_test.txt", '\t'))
	{
		test_data.Next();

		TClientItemTable * test_item_table = nullptr;
		int32_t test_itemTableSize = test_data.m_File.GetRowCount() - 1;
		test_item_table = new TClientItemTable[test_itemTableSize];
		memset(test_item_table, 0, sizeof(TClientItemTable) * test_itemTableSize);

		while (test_data.Next())
		{
			if (!Set_Proto_Item_Table(test_item_table, test_data, localMap))
				fprintf(stderr, "몹 프로토 테이블 셋팅 실패.\n");

			test_map_itemTableByVnum.insert(std::map<uint32_t, TClientItemTable * >::value_type(test_item_table->dwVnum, test_item_table));
			++test_item_table;
		}
	}

	set<int32_t> vnumSet;

	cCsvTable data;
	if (!data.Load("item_proto.txt", '\t'))
	{
		fprintf(stderr, "item_proto.txt 파일을 읽어오지 못했습니다\n");
		return false;
	}
	data.Next();

	if (m_pItemTable)
	{
		free(m_pItemTable);
		m_pItemTable = nullptr;
	}

	int32_t addNumber = 0;
	while (data.Next())
	{
		int32_t vnum = atoi(data.AsStringByIndex(0));

		auto it_map_itemTable = test_map_itemTableByVnum.find(vnum);
		if (it_map_itemTable != test_map_itemTableByVnum.end())
			addNumber++;
	}

	data.Destroy();
	if (!data.Load("item_proto.txt", '\t'))
	{
		fprintf(stderr, "item_proto.txt 파일을 읽어오지 못했습니다\n");
		return false;
	}
	data.Next();

	m_iItemTableSize = data.m_File.GetRowCount() - 1 + addNumber;
	m_pItemTable = new TClientItemTable[m_iItemTableSize];
	memset(m_pItemTable, 0, sizeof(TClientItemTable) * m_iItemTableSize);

	TClientItemTable * item_table = m_pItemTable;

	while (data.Next())
	{
		int32_t col = 0;

		auto it_map_itemTable = test_map_itemTableByVnum.find(atoi(data.AsStringByIndex(col)));
		if (it_map_itemTable == test_map_itemTableByVnum.end())
		{
			if (!Set_Proto_Item_Table(item_table, data, localMap))
				fprintf(stderr, "몹 프로토 테이블 셋팅 실패.\n");
		}
		else
		{
			TClientItemTable * tempTable = it_map_itemTable->second;

			item_table->dwVnum = tempTable->dwVnum;
			strncpy(item_table->szName, tempTable->szName, ITEM_NAME_MAX_LEN);
			strncpy(item_table->szLocaleName, tempTable->szLocaleName, ITEM_NAME_MAX_LEN);
			item_table->bType = tempTable->bType;
			item_table->bSubType = tempTable->bSubType;
			item_table->bSize = tempTable->bSize;
			item_table->dwAntiFlags = tempTable->dwAntiFlags;
			item_table->dwFlags = tempTable->dwFlags;
			item_table->dwWearFlags = tempTable->dwWearFlags;
			item_table->dwImmuneFlag = tempTable->dwImmuneFlag;
			item_table->dwGold = tempTable->dwGold;
			item_table->dwShopBuyPrice = tempTable->dwShopBuyPrice;
			item_table->dwRefinedVnum = tempTable->dwRefinedVnum;
			item_table->wRefineSet = tempTable->wRefineSet;
			item_table->bAlterToMagicItemPct = tempTable->bAlterToMagicItemPct;

			int32_t i;
			for (i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
			{
				item_table->aLimits[i].bType = tempTable->aLimits[i].bType;
				item_table->aLimits[i].lValue = tempTable->aLimits[i].lValue;
			}

			for (i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
			{
				item_table->aApplies[i].bType = tempTable->aApplies[i].bType;
				item_table->aApplies[i].lValue = tempTable->aApplies[i].lValue;
			}

			for (i = 0; i < ITEM_VALUES_MAX_NUM; ++i)
				item_table->alValues[i] = tempTable->alValues[i];

			item_table->bSpecular = tempTable->bSpecular;
			item_table->bGainSocketPct = tempTable->bGainSocketPct;

			item_table->bWeight = tempTable->bWeight;

		}

		vnumSet.insert(item_table->dwVnum);
		++item_table;
	}

	test_data.Destroy();
	if (test_data.Load("item_proto_test.txt", '\t'))
	{
		test_data.Next();

		while (test_data.Next())
		{
			auto itVnum = vnumSet.find(atoi(test_data.AsStringByIndex(0)));
			if (itVnum != vnumSet.end())
				continue;

			if (!Set_Proto_Item_Table(item_table, test_data, localMap))
				fprintf(stderr, "몹 프로토 테이블 셋팅 실패.\n");

			vnumSet.insert(item_table->dwVnum);
			++item_table;
		}
	}
	return true;
}

uint32_t g_adwItemProtoKey[4] =
{
	173217,
	72619434,
	408587239,
	27973291
};

void SaveItemProto()
{
	FILE * fp;
	fopen_s(&fp, "item_proto", "wb");

	if (!fp)
	{
		printf("cannot open %s for writing\n", "item_proto");
		return;
	}

	uint32_t fourcc = MAKEFOURCC('M', 'I', 'P', 'X');
	fwrite(&fourcc, sizeof(uint32_t), 1, fp);

	uint32_t dwVersion = 0x00000001;
	fwrite(&dwVersion, sizeof(uint32_t), 1, fp);

	uint32_t dwStride = sizeof(TClientItemTable);
	fwrite(&dwStride, sizeof(uint32_t), 1, fp);

	uint32_t dwElements = m_iItemTableSize;
	fwrite(&dwElements, sizeof(uint32_t), 1, fp);

	CLZObject zObj;
	std::vector <TClientItemTable> vec_item_table(&m_pItemTable[0], &m_pItemTable[m_iItemTableSize - 1]);
	sort (&m_pItemTable[0], &m_pItemTable[0] + m_iItemTableSize);
	if (!CLZO::instance().CompressEncryptedMemory(zObj, m_pItemTable, sizeof(TClientItemTable) * m_iItemTableSize, g_adwItemProtoKey))
	{
		printf("cannot compress\n");
		fclose(fp);
		return;
	}

	const CLZObject::THeader& r = zObj.GetHeader();
	uint32_t dwDataSize = zObj.GetSize();
	fwrite(&dwDataSize, sizeof(uint32_t), 1, fp);
	fwrite(zObj.GetBuffer(), dwDataSize, 1, fp);

	fclose(fp);
	fopen_s(&fp, "item_proto", "rb");

	if (!fp)
	{
		printf("Error!!\n");
		return;
	}

	fread(&fourcc, sizeof(uint32_t), 1, fp);
	fread(&dwElements, sizeof(uint32_t), 1, fp);
	fclose(fp);
}

int32_t main(int32_t argc, char ** argv)
{
	if (BuildMobTable())
	{
		SaveMobProto();
		cout << "BuildMobTable working normal" << endl;
	}

	if (BuildItemTable())
	{
		SaveItemProto();
		cout << "BuildItemTable working normal" << endl;
	}

	return 0;
}
