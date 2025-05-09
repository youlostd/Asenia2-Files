#include "stdafx.h"
#include "char.h"
TJobInitialPoints JobInitialPoints[JOB_MAX_NUM] = 
/*
   {
   int st, ht, dx, iq;
   int max_hp, max_sp;
   int hp_per_ht, sp_per_iq;
   int hp_per_lv_begin, hp_per_lv_end;
   int sp_per_lv_begin, sp_per_lv_end;
   int max_stamina;
   int stamina_per_con;
   int stamina_per_lv_begin, stamina_per_lv_end;
   }
 */
{
	// str con dex int 초기HP 초기SP  CON/HP INT/SP  HP랜덤/lv   MP랜덤/lv  초기stam  stam/con stam/lv
	{   6,  4,  3,  3,  600,   300,     40,    20,    44, 44,     22, 22,     800,      5,      1, 3  }, // JOB_WARRIOR  16
	{   4,  3,  6,  3,  650,   300,     40,    20,    44, 44,     22, 22,     800,      5,      1, 3  }, // JOB_ASSASSIN 16
	{   5,  3,  3,  5,  650,   300,     40,    20,    44, 44,     22, 22,     800,      5,      1, 3  }, // JOB_SURA	 16
	{   3,  4,  3,  6,  700,   300,     40,    20,    44, 44,     22, 22,     800,      5,      1, 3  }, // JOB_SHAMANa  16
	{   6,  6,  2,  2,  600,   300,     40,    20,    44, 44,     22, 22,     800,      5,      1, 3  }, // JOB_WOLFMAN  16  // TODO: 4개능력치 초기값 외에 다른 스탯 필요 (기획자한테 요청)
};



const DWORD AccSet[6][4] =
{
	// Set bonus 1 is inactive. (Set bonus 1 aktif de?il)
	// {affecttype, set bonus 1, set bonus 2, set bonus 3},
	{0,0,0,0},
	{POINT_MAX_HP,1,2,5},
	{POINT_MAX_HP,2,3,5},
	{POINT_MAX_HP,3,4,8},
	{POINT_MAX_HP,5,5,10},
	{POINT_MAX_HP,1000,1500,2000}
	
};


const TMobRankStat MobRankStats[MOB_RANK_MAX_NUM] =
/*
   {
   int         iGoldPercent;
   }
 */
{
	{  20,  }, // MOB_RANK_PAWN, 
	{  20,  }, // MOB_RANK_S_PAWN, 
	{  25,  }, // MOB_RANK_KNIGHT, 
	{  30,  }, // MOB_RANK_S_KNIGHT, 
	{  50,  }, // MOB_RANK_BOSS, 
	{ 100,  }  // MOB_RANK_KING,
};

TBattleTypeStat BattleTypeStats[BATTLE_TYPE_MAX_NUM] =
/*
   {
   int         AttGradeBias;
   int         DefGradeBias;
   int         MagicAttGradeBias;
   int         MagicDefGradeBias;
   }
 */
{
	{	  0,	  0,	  0,	-10	}, // BATTLE_TYPE_MELEE, 
	{	 10,	-20,	-10,	-15	}, // BATTLE_TYPE_RANGE,
	{	 -5,	 -5,	 10,	 10	}, // BATTLE_TYPE_MAGIC,  
	{	  0,	  0,	  0,	  0	}, // BATTLE_TYPE_SPECIAL,
	{	 10,	-10,	  0,	-15	}, // BATTLE_TYPE_POWER,  
	{	-10,	 10,	-10,	  0	}, // BATTLE_TYPE_TANKER,
	{	 20,	-20,	  0,	-10	}, // BATTLE_TYPE_SUPER_POWER,
	{	-20,	 20,	-10,	  0	}, // BATTLE_TYPE_SUPER_TANKER,
};

const DWORD BiyologSistemi[17][16] =
{
	////gerekenitem-istenenadet-beklemes?esi(dk)-ge?eoran?-ruhta�???vnum-affect1vnum-affect1value-affect2vnum-affect2value-affect3vnum-affect3value-affect4vnum-affect4value-affect5vnum-affect5value
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{ 30006, 10, 0, 100, 30220, 0, 8, 10, 0,0,0,0,0,0,0,0},
	{ 30047, 15, 0, 100, 30221, 0, 7, 5, 0,0,0,0,0,0,0,0},
	{ 30015, 15, 0, 100, 30222, 0, 54, 60, 0,0,0,0,0,0,0,0},
	{ 30050, 20, 0, 100, 30223, 0, 53, 50, 0,0,0,0,0,0,0,0},
	{ 30165, 25, 0, 100, 30224, 0, 8, 11, 83,10,0,0,0,0,0,0},
	{ 30166, 30, 0, 100, 30225, 0, 7, 6, 64,10,0,0,0,0,0,0},
	{ 30167, 40, 0, 100, 30226, 0, 78, 10, 79,10,80,10,81,10,0,0},
	{ 30168, 50, 5, 100, 30227, 0, 17, 8, 0,0,0,0,0,0,0,0},
	{ 30251, 10, 10, 95, 0, 0, 1, 1000, 54,120,53,51,0,0,0,0},
	{ 30252, 20, 10, 90, 30228, 0, 1, 1100, 54,140,53,60,0,0,0,0},
	{ 31082, 15, 15, 85, 0, 0, 72, 10, 0,0,0,0,0,0,0,0},
	{ 31089, 15, 20, 80, 0, 0, 71, 10, 0,0,0,0,0,0,0,0},
	{ 31080, 15, 30, 75, 0, 0, 63, 40, 0,0,0,0,0,0,0,0},
	{ 31090, 15, 40, 70, 0, 0, 96, 40, 0,0,0,0,0,0,0,0},
	{ 30613, 15, 50, 65, 0, 0, 112, 40, 0,0,0,0,0,0,0,0},
	{ 31034, 15, 60, 60, 0, 0, 86, 25, 0,0,0,0,0,0,0,0}

};
const char * Takip[30][3] =
{
	{ "590500","110500","kule.dungeon_time"},
	{ "1214900","727400","ork.dungeon_time"},
	{ "591100","99200","cata.dungeon_time"},
	{ "69000","610900","baron.dungeon_time"},
	{ "180100","1220400","dragonlair.dungeon_time"},
	{ "614000","707100","razo.dungeon_time"},
	{ "432400","165200","nemere.dungeon_time"},
	{ "1116100","51600","nefrit.dungeon_time"},
	{ "1117800","53400","slime.dungeon_time"},
	{ "438100","442100","sevgililer.dungeon_time"},
	{ "11578500","11583400","easter.dungeon_time"},
	{ "623000","963000","jaws.dungeon_time"},
	{ "6531500","6527500","kitap.dungeon_time"},
	{ "2227200","102400","ada.dungeon_time"},
	{ "2156800","3424800","underwater.dungeon_time"},
	{ "1106900","1790800","hydra.dungeon_time"},
	{ "1561600","102400","baykus.dungeon_time"},
	{ "2007700","2016700","duratus.dungeon_time"},
	{ "3027800","3038000","gold.dungeon_time"},
	{ "7501700","7491200","mantar.dungeon_time"},
	{ "4876700","4771600","enfekte.dungeon_time"},
	{ "827800","1418300","jotun.dungeon_time"},
	{ "3770200","2916200","kabus.dungeon_time"},
	{ "597600","699800","meley.dungeon_time"},
	{ "2114300","1711800","ruh.dungeon_time"},
	{ "8579600","8573000","fear.dungeon_time"},
	{ "2127300","3188500","kristal.dungeon_time"},
	{ "1752300","1751800","su.dungeon_time"},
	{ "11173500","9999200","piramid.dungeon_time"},
	{ "15680000","3800700","buzdiyar.dungeon_time"}
};

const DWORD * exp_table = NULL;

#ifdef ENABLE_NEW_PET_SYSTEM
DWORD* exppet_table = nullptr;
const DWORD Pet_Table[10][2] =
{
	{ 55701, 34041 },
	{ 55702, 34045 },
	{ 55703, 34049 },
	{ 55704, 34053 },
	{ 55705, 34053 },
	{ 55706, 34064 },
	{ 55707, 34073 },
	{ 55708, 34075 },
	{ 55709, 34080 },
	{ 55710, 34082 }
};

const DWORD Pet_Skill_Table[25][23] =
{
	//Level Like Index ->> Bonus Value --> typeSkill --> CoolDown -->> Percentage
	{ 78, 0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12 },
	{ 80, 0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12 },
	{ 79, 0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12 },
	{ 91, 0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12 },
	{ 95, 0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12 }, //5
	{ 64, 0, 0, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 10 },
	{ 97, 0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6, 7, 7, 8, 8, 9, 10, 10, 11, 11, 12 },
	{ 9, 0, 0, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 14, 14, 15, 15, 16, 16, 17, 17, 17 },
	{ 16, 0, 0, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 8, 8, 9, 9 },
	{ 4, 1, 480, 17, 68, 153, 272, 425, 612, 833, 1088, 1377, 1700, 2057, 2448, 2873, 3332, 3825, 4352, 4913, 5508, 6137, 6800 }, //10
	{ 23, 0, 0, 1, 1, 1, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5 },
	{ 24, 0, 0, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 6, 6, 6 },
	{ 27, 0, 0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10, 10, 11, 11 },
	{ 28, 0, 0, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7 },
	{ 44, 0, 0, 5, 10, 15, 20, 25, 31, 36, 41, 46, 51, 56, 61, 67, 72, 77, 82, 87, 92, 97, 103 }, //15
	{ 52, 0, 0, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6 },
	{ 11, 1, 600, 2, 4, 6, 7, 8, 9, 11, 12, 13, 15, 16, 17, 18, 20, 21, 22, 23, 25, 26, 27 },
	{ 6, 1, 480, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 96, 0, 0, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10 }, // metin
	{ 63, 0, 0, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10 }, // canavar
	{ 112, 0, 0, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10 }, // patron
	{ 47, 0, 0, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10 }, // hp_recover
	{ 8, 0, 0, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10 }, // feather
	{ 46, 0, 0, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10 }, // potion
	{ 43, 0, 0, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10 } // exp
};

const DWORD Pet_SKill_TablePerc[3][20] =
{
	{ 2, 5, 8, 11, 13, 16, 19, 22, 24, 27, 30, 33, 35, 38, 41, 44, 46, 49, 52, 55 },
	{ 2, 5, 7, 10, 12, 15, 17, 20, 23, 25, 27, 30, 32, 35, 37, 40, 42, 45, 47, 50 },
	{ 2, 5, 8, 11, 13, 16, 19, 22, 25, 27, 30, 33, 36, 38, 41, 44, 47, 50, 52, 55 }
};

const DWORD Pet_Types[9][3] =
{
	// {0, 0, 0},
	{6, 1, 5}, //0
	{5, 2, 3},
	{6, 1, 7},
	{5, 3, 4},
	{6, 1, 9},
	{5, 5, 6},
	{6, 1, 10},
	{5, 7, 10} //7
};

DWORD exppet_table_common[105];
#endif

const DWORD exp_table_euckr[PLAYER_EXP_TABLE_MAX + 1] =	
{
	0,			// 0

	100,
	150,
	260,
	380,
	600,

	1300,
	3300,
	5700,
	8700,
	12800,		// 10
	18000,
	25000,
	36000,
	52000,
	73000,
	100000,
	125000,
	160000,
	220000,
	280000,		// 20
	370000,
	540000,
	670000,
	880000,
	1000000,

	1237000,
	1418000,
	1624000,
	1857000,
	2122000,	// 30
	2421000,
	2761000,
	3145000,
	3580000,
	4073000,
	4632000,
	5194000,
	5717000,
	6264000,
	6837000,	// 40
	7600000,
	8274000,
	8990000,
	9753000,
	10560000,
	11410000,
	12320000,
	13270000,
	14280000,
	15340000,	// 50
	16870000,
	18960000,
	19980000,
	21420000,
	22930000,
	24530000,
	26200000,
	27960000,
	29800000,
	32780000,	// 60
	36060000,
	39670000,
	43640000,
	48000000,
	52800000,
	58080000,
	63890000,
	70280000,
	77310000,
	85040000, 	// 70
	93540000,
	102900000,
	113200000,
	124500000,
	137000000,
	150700000,
	165700000,
	236990000,
	260650000,
	286780000,	// 80
	315380000,
	346970000,
	381680000,
	419770000,
	461760000,
	508040000,
	558740000,
	614640000,
	676130000,
	743730000,	// 90
	1041222000,
	1145344200,
	1259878620,
	1385866482,
	1524453130,
	1676898443,
	1844588288,
	2029047116,
	2100000000,	//	99		
	2100000000,	//	100
	2100000000,		
	2100000000,		
	2100000000,		
	2100000000,		
	2100000000,	//	105
	2100000000,		
	2100000000,		
	2100000000,		
	2100000000,		
	2100000000,	//	110
	2100000000,		
	2100000000,		
	2100000000,		
	2100000000,		
	2100000000,	//	115
	2100000000,		
	2100000000,		
	2100000000,		
	2100000000,		
	2100000000,	//	120
};


const DWORD exp_table_common[PLAYER_EXP_TABLE_MAX + 1] =
{
	0,	//	0
	300,		
	800,		
	1500,		
	2500,		
	4300,		
	7200,		
	11000,		
	17000,		
	24000,		
	33000,	//	10
	43000,		
	58000,		
	76000,		
	100000,		
	130000,		
	169000,		
	219000,		
	283000,		
	365000,		
	472000,	//	20
	610000,		
	705000,		
	813000,		
	937000,		
	1077000,		
	1237000,		
	1418000,		
	1624000,		
	1857000,		
	2122000,	//	30
	2421000,		
	2761000,		
	3145000,		
	3580000,		
	4073000,		
	4632000,		
	5194000,		
	5717000,		
	6264000,		
	6837000,	//	40
	7600000,		
	8274000,		
	8990000,		
	9753000,		
	10560000,		
	11410000,		
	12320000,		
	13270000,		
	14280000,		
	15340000,	//	50
	16870000,		
	18960000,		
	19980000,		
	21420000,		
	22930000,		
	24530000,		
	26200000,		
	27960000,		
	29800000,		
	32780000,	//	60
	36060000,		
	39670000,		
	43640000,		
	48000000,		
	52800000,		
	58080000,		
	63890000,		
	70280000,		
	77310000,		
	85040000,	//	70
	93540000,		
	102900000,		
	113200000,		
	124500000,		
	137000000,		
	150700000,		
	165700000,		
	236990000,		
	260650000,		
	286780000,	//	80
	315380000,		
	346970000,		
	381680000,		
	419770000,		
	461760000,		
	508040000,		
	558740000,		
	614640000,		
	676130000,		
	743730000,	//	90
	1041222000,		
	1145344200,		
	1259878620,		
	1385866482,		
	1524453130,		
	1676898443,		
	1844588288,		
	2029047116,		
	2050000000,	//	99레벨 일 때 필요경픽치 (100레벨이 되기 위한)
	2150000000,	//	100
	2210000000,		
	2250000000,		
	2280000000,		
	2310000000,		
	2330000000,	//	105
	2350000000,		
	2370000000,		
	2390000000,		
	2400000000,		
	2410000000,	//	110
	2420000000,		
	2430000000,		
	2440000000,		
	2450000000,		
	2460000000,	//	115
	2470000000,		
	2480000000,		
	2490000000,		
	2490000000,		
	2500000000,	//	120
};

#ifdef ENABLE_REBIRT_SYSTEM
const DWORD exp_rebirt_table[REBIRT_MAX + 11] =
{
	0,
	2100000000,	//Rebirt 1
	2100000000,	//Rebirt 2
	2100000000,	//Rebirt 3
	2100000000,	//Rebirt 4
	2100000000,	//Rebirt 5
	2100000000,	//Rebirt 6
	2100000000,	//Rebirt 7
	2100000000,	//Rebirt 8
	2100000000,	//Rebirt 9
	2100000000,	//Rebirt 10
	2100000000,	//Rebirt 11
	2100000000,	//Rebirt 12
	2100000000,	//Rebirt 13
	2100000000,	//Rebirt 14
	2100000000,	//Rebirt 15
	2100000000,	//Rebirt 16
	2100000000,	//Rebirt 17
	2100000000,	//Rebirt 18
	2100000000,	//Rebirt 19
	2100000000,	//Rebirt 20
};
#endif

const DWORD exp_table_newcibn[PLAYER_EXP_TABLE_MAX + 1 ] =
{
	300,	// 0
	800,	// 1
	1500,
	2500,
	4300,
	7200,
	11400,
	17000,
	24000,
	33000,
	46000,	// 10
	62000,
	83000,
	111000,
	149000,
	200000,
	268000,
	360000,
	482000,
	647000,
	868000,	// 20
	996000,
	1143000,
	1312000, 
	1506000,
	1729000,
	1984000,
	2277000,
	2614000,
	3000000,
	3443000,	// 30
	3952000,
	4536000,
	5206000,
	5975000,
	6858000,
	7730000,
	8504000,
	9307000,
	10140000,
	11330000,	// 40
	12320000,
	13370000,
	14490000,
	15670000,
	16920000,
	18240000,
	19630000,
	21090000,
	22630000,
	24670000,	// 50
	26890000,
	29310000,
	31950000,
	34820000,
	37960000,
	41370000,
	45100000,
	49160000,
	53580000,
	58400000,	// 60
	63660000,
	69390000,
	75630000,
	82440000,
	89860000,
	97950000,
	106760000,
	136370000,
	151800000,
	168300000,	// 70
	250000000,
	340000000,
	450000000,
	570000000,
	690000000,
	810000000,
	912600000,
	1004000000,
	1094000000,
	1182000000,	// 80
	1269000000,
	1354000000,
	1438000000,
	1521000000,
	1603000000,
	1684000000,
	1764000000,
	1844000000,
	1922000000,
	2000000000,	// 90
	2000000000,
	2000000000,
	2000000000,
	2000000000,
	2000000000,
	2000000000,
	2000000000,
	2000000000,
	2000000000,	//	99		
	2000000000,	//	100
	2000000000,		
	2000000000,		
	2000000000,		
	2000000000,		
	2000000000,	//	105
	2000000000,		
	2000000000,		
	2000000000,		
	2000000000,		
	2000000000,	//	110
	2000000000,		
	2000000000,		
	2000000000,		
	2000000000,		
	2000000000,	//	115
	2000000000,		
	2000000000,		
	2000000000,		
	2000000000,		
	2000000000,	//	120
};

const int * aiPercentByDeltaLev = NULL;
const int * aiPercentByDeltaLevForBoss = NULL;

// 적과 나와의 레벨차이에 의한 계산에 사용되는 테이블
// MIN(MAX_EXP_DELTA_OF_LEV - 1, (적렙 + 15) - 내렙))
const int aiPercentByDeltaLevForBoss_euckr[MAX_EXP_DELTA_OF_LEV] =
{
	1,      // -15  0
	3,          // -14  1
	5,          // -13  2
	7,          // -12  3
	15,         // -11  4
	30,         // -10  5
	60,         // -9   6
	90,         // -8   7
	91,         // -7   8
	92,         // -6   9
	93,         // -5   10
	94,         // -4   11
	95,         // -3   12
	97,         // -2   13
	99,         // -1   14
	100,        // 0    15
	105,        // 1    16
	110,        // 2    17
	115,        // 3    18
	120,        // 4    19
	125,        // 5    20
	130,        // 6    21
	135,        // 7    22
	140,        // 8    23
	145,        // 9    24
	150,        // 10   25
	155,        // 11   26
	160,        // 12   27
	165,        // 13   28
	170,        // 14   29
	180         // 15   30
};



const int aiPercentByDeltaLev_euckr[MAX_EXP_DELTA_OF_LEV] =
{
	1,  //  -15 0
	5,  //  -14 1
	10, //  -13 2
	20, //  -12 3
	30, //  -11 4
	50, //  -10 5
	70, //  -9  6
	80, //  -8  7
	85, //  -7  8
	90, //  -6  9
	92, //  -5  10
	94, //  -4  11
	96, //  -3  12
	98, //  -2  13
	100,    //  -1  14
	100,    //  0   15
	105,    //  1   16
	110,    //  2   17
	115,    //  3   18
	120,    //  4   19
	125,    //  5   20
	130,    //  6   21
	135,    //  7   22
	140,    //  8   23
	145,    //  9   24
	150,    //  10  25
	155,    //  11  26
	160,    //  12  27
	165,    //  13  28
	170,    //  14  29
	180,    //  15  30
};

const DWORD party_exp_distribute_table[PLAYER_MAX_LEVEL_CONST + 1] = 
{
	0,
	10,		10,		10,		10,		15,		15,		20,		25,		30,		40,		// 1 - 10
	50,		60,		80,		100,	120,	140,	160,	184,	210,	240,	// 11 - 20
	270,	300,	330,	360,	390,	420,	450,	480,	510,	550,	// 21 - 30
	600,	640,	700,	760,	820,	880,	940,	1000,	1100,	1180,	// 31 - 40
	1260,	1320,	1380,	1440,	1500,	1560,	1620,	1680,	1740,	1800,	// 41 - 50
	1860,	1920,	2000,	2100,	2200,	2300,	2450,	2600,	2750,	2900,	// 51 - 60
	3050,	3200,	3350,	3500,	3650,	3800,	3950,	4100,	4250,	4400,	// 61 - 70
	4600,	4800,	5000,	5200,	5400,	5600,	5800,	6000,	6200,	6400,	// 71 - 80
	6600,	6900,	7100,	7300,	7600,	7800,	8000,	8300,	8500,	8800,	// 81 - 90
	9000,	9000,	9000,	9000,	9000,	9000,	9000,	9000,	9000,	9000,	// 91 - 100
	10000,	10000,	10000,	10000,	10000,	10000,	10000,	10000,	10000,	10000,	// 101 - 110
	12000,	12000,	12000,	12000,	12000,	12000,	12000,	12000,	12000,	12000,	// 111 - 120
};

Coord aArroundCoords[ARROUND_COORD_MAX_NUM] = 
{
	{	     0,	      0	    },
	{        0,      50     },
	{       35,     35      },
	{       50,     -0      },
	{       35,     -35     },
	{       0,      -50     },
	{       -35,    -35     },
	{       -50,    0       },
	{       -35,    35      },
	{       0,      100     },
	{       71,     71      },
	{       100,    -0      },
	{       71,     -71     },
	{       0,      -100    },
	{       -71,    -71     },
	{       -100,   0       },
	{       -71,    71      },
	{       0,      150     },
	{       106,    106     },
	{       150,    -0      },
	{       106,    -106    },
	{       0,      -150    },
	{       -106,   -106    },
	{       -150,   0       },
	{       -106,   106     },
	{       0,      200     },
	{       141,    141     },
	{       200,    -0      },
	{       141,    -141    },
	{       0,      -200    },
	{       -141,   -141    },
	{       -200,   0       },
	{       -141,   141     },
	{       0,      250     },
	{       177,    177     },
	{       250,    -0      },
	{       177,    -177    },
	{       0,      -250    },
	{       -177,   -177    },
	{       -250,   0       },
	{       -177,   177     },
	{       0,      300     },
	{       212,    212     },
	{       300,    -0      },
	{       212,    -212    },
	{       0,      -300    },
	{       -212,   -212    },
	{       -300,   0       },
	{       -212,   212     },
	{       0,      350     },
	{       247,    247     },
	{       350,    -0      },
	{       247,    -247    },
	{       0,      -350    },
	{       -247,   -247    },
	{       -350,   0       },
	{       -247,   247     },
	{       0,      400     },
	{       283,    283     },
	{       400,    -0      },
	{       283,    -283    },
	{       0,      -400    },
	{       -283,   -283    },
	{       -400,   0       },
	{       -283,   283     },
	{       0,      450     },
	{       318,    318     },
	{       450,    -0      },
	{       318,    -318    },
	{       0,      -450    },
	{       -318,   -318    },
	{       -450,   0       },
	{       -318,   318     },
	{       0,      500     },
	{       354,    354     },
	{       500,    -0      },
	{       354,    -354    },
	{       0,      -500    },
	{       -354,   -354    },
	{       -500,   0       },
	{       -354,   354     },
	{       0,      550     },
	{       389,    389     },
	{       550,    -0      },
	{       389,    -389    },
	{       0,      -550    },
	{       -389,   -389    },
	{       -550,   0       },
	{       -389,   389     },
	{       0,      600     },
	{       424,    424     },
	{       600,    -0      },
	{       424,    -424    },
	{       0,      -600    },
	{       -424,   -424    },
	{       -600,   0       },
	{       -424,   424     },
	{       0,      650     },
	{       460,    460     },
	{       650,    -0      },
	{       460,    -460    },
	{       0,      -650    },
	{       -460,   -460    },
	{       -650,   0       },
	{       -460,   460     },
	{       0,      700     },
	{       495,    495     },
	{       700,    -0      },
	{       495,    -495    },
	{       0,      -700    },
	{       -495,   -495    },
	{       -700,   0       },
	{       -495,   495     },
	{       0,      750     },
	{       530,    530     },
	{       750,    -0      },
	{       530,    -530    },
	{       0,      -750    },
	{       -530,   -530    },
	{       -750,   0       },
	{       -530,   530     },
	{       0,      800     },
	{       566,    566     },
	{       800,    -0      },
	{       566,    -566    },
	{       0,      -800    },
	{       -566,   -566    },
	{       -800,   0       },
	{       -566,   566     },
	{       0,      850     },
	{       601,    601     },
	{       850,    -0      },
	{       601,    -601    },
	{       0,      -850    },
	{       -601,   -601    },
	{       -850,   0       },
	{       -601,   601     },
	{       0,      900     },
	{       636,    636     },
	{       900,    -0      },
	{       636,    -636    },
	{       0,      -900    },
	{       -636,   -636    },
	{       -900,   0       },
	{       -636,   636     },
	{       0,      950     },
	{       672,    672     },
	{       950,    -0      },
	{       672,    -672    },
	{       0,      -950    },
	{       -672,   -672    },
	{       -950,   0       },
	{       -672,   672     },
	{       0,      1000    },
	{       707,    707     },
	{       1000,   -0      },
	{       707,    -707    },
	{       0,      -1000   },
	{       -707,   -707    },
	{       -1000,  0       },
	{       -707,   707     },
};

const DWORD guild_exp_table[GUILD_MAX_LEVEL+1] =
{
	0,
	15000UL,
	45000UL,
	90000UL,
	160000UL,
	235000UL,
	325000UL,
	430000UL,
	550000UL,
	685000UL,
	835000UL,
	1000000UL,
	1500000UL,
	2100000UL,
	2800000UL,
	3600000UL,
	4500000UL,
	6500000UL,
	8000000UL,
	10000000UL,
	42000000UL
};

// INTERNATIONAL_VERSION 길드경험치
const DWORD guild_exp_table2[GUILD_MAX_LEVEL+1] =
{
	0,	
	6000UL,
	18000UL,
	36000UL,
	64000UL,
	94000UL,
	130000UL,
	172000UL,
	220000UL,
	274000UL,
	334000UL,
	400000UL,
	600000UL,
	840000UL,
	1120000UL,
	1440000UL,
	1800000UL,
	2600000UL,
	3200000UL,
	4000000UL,
	16800000UL
};
// END_OF_INTERNATIONAL_VERSION 길드경험치

const int aiMobEnchantApplyIdx[MOB_ENCHANTS_MAX_NUM] =
{
	APPLY_CURSE_PCT,
	APPLY_SLOW_PCT,
	APPLY_POISON_PCT,
	APPLY_STUN_PCT,
	APPLY_CRITICAL_PCT,
	APPLY_PENETRATE_PCT,
};

const int aiMobResistsApplyIdx[MOB_RESISTS_MAX_NUM] =
{
	APPLY_RESIST_SWORD,
	APPLY_RESIST_TWOHAND,
	APPLY_RESIST_DAGGER,
	APPLY_RESIST_BELL,
	APPLY_RESIST_FAN,
	APPLY_RESIST_BOW,
	APPLY_RESIST_FIRE,
	APPLY_RESIST_ELEC,
	APPLY_RESIST_MAGIC,
	APPLY_RESIST_WIND,
	APPLY_POISON_REDUCE,
};

const int aiSocketPercentByQty[5][4] =
{
	{  0,  0,  0,  0 },
	{  3,  0,  0,  0 },
	{ 10,  1,  0,  0 },
	{ 15, 10,  1,  0 },
	{ 20, 15, 10,  1 }
};

const int aiWeaponSocketQty[WEAPON_NUM_TYPES] =
{
	6, // WEAPON_SWORD,
	6, // WEAPON_DAGGER,
	6, // WEAPON_BOW,
	6, // WEAPON_TWO_HANDED,
	6, // WEAPON_BELL,
	6, // WEAPON_FAN,
	0, // WEAPON_ARROW,
	0, // WEAPON_MOUNT_SPEAR
	6, // WEAPON_CLAW
#ifdef __NEW_ARROW_SYSTEM__
	0, // WEAPON_UNLIMITED_ARROW
#endif
};

const int aiArmorSocketQty[ARMOR_NUM_TYPES] =
{
	3, // ARMOR_BODY,
	1, // ARMOR_HEAD,
	1, // ARMOR_SHIELD,
	0, // ARMOR_WRIST,
	0, // ARMOR_FOOTS,
	0  // ARMOR_ACCESSORY
};

TItemAttrMap g_map_itemAttr;
TItemAttrMap g_map_itemRare;

const TApplyInfo aApplyInfo[MAX_APPLY_NUM] =
/*
{
   DWORD dwPointType;
}
 */
{
	// Point Type
	{ POINT_NONE,			},   // APPLY_NONE,		0
	{ POINT_MAX_HP,		        },   // APPLY_MAX_HP,		1
	{ POINT_MAX_SP,		        },   // APPLY_MAX_SP,		2
	{ POINT_HT,			        },   // APPLY_CON,		3
	{ POINT_IQ,			        },   // APPLY_INT,		4
	{ POINT_ST,			        },   // APPLY_STR,		5
	{ POINT_DX,			        },   // APPLY_DEX,		6
	{ POINT_ATT_SPEED,		        },   // APPLY_ATT_SPEED,	7
	{ POINT_MOV_SPEED,		        },   // APPLY_MOV_SPEED,	8
	{ POINT_CASTING_SPEED,	        },   // APPLY_CAST_SPEED,	9
	{ POINT_HP_REGEN,			},   // APPLY_HP_REGEN,		10
	{ POINT_SP_REGEN,			},   // APPLY_SP_REGEN,		11
	{ POINT_POISON_PCT,		        },   // APPLY_POISON_PCT,	12
	{ POINT_STUN_PCT,		        },   // APPLY_STUN_PCT,		13
	{ POINT_SLOW_PCT,		        },   // APPLY_SLOW_PCT,		14
	{ POINT_CRITICAL_PCT,		},   // APPLY_CRITICAL_PCT,	15
	{ POINT_PENETRATE_PCT,	        },   // APPLY_PENETRATE_PCT,	16
	{ POINT_ATTBONUS_HUMAN,	        },   // APPLY_ATTBONUS_HUMAN,	17
	{ POINT_ATTBONUS_ANIMAL,	        },   // APPLY_ATTBONUS_ANIMAL,	18
	{ POINT_ATTBONUS_ORC,		},   // APPLY_ATTBONUS_ORC,	19
	{ POINT_ATTBONUS_MILGYO,	        },   // APPLY_ATTBONUS_MILGYO,	20
	{ POINT_ATTBONUS_UNDEAD,	        },   // APPLY_ATTBONUS_UNDEAD,	21
	{ POINT_ATTBONUS_DEVIL,	        },   // APPLY_ATTBONUS_DEVIL,	22
	{ POINT_STEAL_HP,		        },   // APPLY_STEAL_HP,		23
	{ POINT_STEAL_SP,		        },   // APPLY_STEAL_SP,		24
	{ POINT_MANA_BURN_PCT,	        },   // APPLY_MANA_BURN_PCT,	25
	{ POINT_DAMAGE_SP_RECOVER,	        },   // APPLY_DAMAGE_SP_RECOVER,26
	{ POINT_BLOCK,		        },   // APPLY_BLOCK,		27
	{ POINT_DODGE,		        },   // APPLY_DODGE,		28
	{ POINT_RESIST_SWORD,		},   // APPLY_RESIST_SWORD,	29
	{ POINT_RESIST_TWOHAND,	        },   // APPLY_RESIST_TWOHAND,	30
	{ POINT_RESIST_DAGGER,	        },   // APPLY_RESIST_DAGGER,	31
	{ POINT_RESIST_BELL,		},   // APPLY_RESIST_BELL,	32
	{ POINT_RESIST_FAN,		        },   // APPLY_RESIST_FAN,	33
	{ POINT_RESIST_BOW,		        },   // APPLY_RESIST_BOW,	34
	{ POINT_RESIST_FIRE,		},   // APPLY_RESIST_FIRE,	35
	{ POINT_RESIST_ELEC,		},   // APPLY_RESIST_ELEC,	36
	{ POINT_RESIST_MAGIC,		},   // APPLY_RESIST_MAGIC,	37
	{ POINT_RESIST_WIND,		},   // APPLY_RESIST_WIND,	38
	{ POINT_REFLECT_MELEE,	        },   // APPLY_REFLECT_MELEE,	39
	{ POINT_REFLECT_CURSE,	        },   // APPLY_REFLECT_CURSE,	40
	{ POINT_POISON_REDUCE,	        },   // APPLY_POISON_REDUCE,	41
	{ POINT_KILL_SP_RECOVER,	        },   // APPLY_KILL_SP_RECOVER,	42
	{ POINT_EXP_DOUBLE_BONUS,	        },   // APPLY_EXP_DOUBLE_BONUS,	43
	{ POINT_GOLD_DOUBLE_BONUS,	        },   // APPLY_GOLD_DOUBLE_BONUS,44
	{ POINT_ITEM_DROP_BONUS,	        },   // APPLY_ITEM_DROP_BONUS,	45
	{ POINT_POTION_BONUS,		},   // APPLY_POTION_BONUS,	46
	{ POINT_KILL_HP_RECOVERY,	        },   // APPLY_KILL_HP_RECOVER,	47
	{ POINT_IMMUNE_STUN,		},   // APPLY_IMMUNE_STUN,	48
	{ POINT_IMMUNE_SLOW,		},   // APPLY_IMMUNE_SLOW,	49
	{ POINT_IMMUNE_FALL,		},   // APPLY_IMMUNE_FALL,	50
	{ POINT_NONE,			},   // APPLY_SKILL,		51
	{ POINT_BOW_DISTANCE,		},   // APPLY_BOW_DISTANCE,	52
	{ POINT_ATT_GRADE_BONUS,	        },   // APPLY_ATT_GRADE,	53
	{ POINT_DEF_GRADE_BONUS,	        },   // APPLY_DEF_GRADE,	54
	{ POINT_MAGIC_ATT_GRADE_BONUS,      },   // APPLY_MAGIC_ATT_GRADE,	55
	{ POINT_MAGIC_DEF_GRADE_BONUS,      },   // APPLY_MAGIC_DEF_GRADE,	56
	{ POINT_CURSE_PCT,			},   // APPLY_CURSE_PCT,	57
	{ POINT_MAX_STAMINA			},   // APPLY_MAX_STAMINA	58
	{ POINT_ATTBONUS_WARRIOR		},   // APPLY_ATTBONUS_WARRIOR  59
	{ POINT_ATTBONUS_ASSASSIN		},   // APPLY_ATTBONUS_ASSASSIN 60
	{ POINT_ATTBONUS_SURA		},   // APPLY_ATTBONUS_SURA     61
	{ POINT_ATTBONUS_SHAMAN		},   // APPLY_ATTBONUS_SHAMAN   62
	{ POINT_ATTBONUS_MONSTER		},   //	APPLY_ATTBONUS_MONSTER  63
	{ POINT_ATT_BONUS		},   // 64 // APPLY_MALL_ATTBONUS
	{ POINT_MALL_DEFBONUS		},   // 65
	{ POINT_MALL_EXPBONUS		},   // 66 APPLY_MALL_EXPBONUS
	{ POINT_MALL_ITEMBONUS		},   // 67
	{ POINT_MALL_GOLDBONUS		},   // 68
	{ POINT_MAX_HP_PCT			},		// 69
	{ POINT_MAX_SP_PCT			},		// 70
	{ POINT_SKILL_DAMAGE_BONUS		},	// 71
	{ POINT_NORMAL_HIT_DAMAGE_BONUS	},	// 72

	// DEFEND_BONUS_ATTRIBUTES
	{ POINT_SKILL_DEFEND_BONUS		},	// 73
	{ POINT_NORMAL_HIT_DEFEND_BONUS	},	// 74
	// END_OF_DEFEND_BONUS_ATTRIBUTES

	// PC_BANG_ITEM_ADD
	{ POINT_PC_BANG_EXP_BONUS	},		// 75
	{ POINT_PC_BANG_DROP_BONUS	},		// 76
	// END_PC_BANG_ITEM_ADD

	{ POINT_NONE,		},				// 77 사용시 HP 소모 APPLY_EXTRACT_HP_PCT

	{ POINT_RESIST_WARRIOR,		},		// 78 무사에게 저항 APPLY_RESIST_WARRIOR
	{ POINT_RESIST_ASSASSIN,	},		// 79 자객에게 저항 APPLY_RESIST_ASSASSIN
	{ POINT_RESIST_SURA,		},		// 80 수라에게 저항 APPLY_RESIST_SURA
	{ POINT_RESIST_SHAMAN,		},		// 81 무당에게 저항 APPLY_RESIST_SHAMAN
	{ POINT_ENERGY				},		// 82 기력 
	{ POINT_DEF_GRADE			},		// 83 방어력. DEF_GRADE_BONUS는 클라에서 두배로 보여지는 의도된 버그(...)가 있다.
	{ POINT_COSTUME_ATTR_BONUS	},		// 84 코스튬에 붙은 속성에 대해서만 보너스를 주는 기력
	{ POINT_MAGIC_ATT_BONUS_PER },			// 85 마법 공격력 +x%
	{ POINT_MELEE_MAGIC_ATT_BONUS_PER		},			// 86 APPLY_MELEE_MAGIC_ATTBONUS_PER
	{ POINT_RESIST_ICE,			},   // APPLY_RESIST_ICE,	87
	{ POINT_RESIST_EARTH,		},   // APPLY_RESIST_EARTH,	88
	{ POINT_RESIST_DARK,		},   // APPLY_RESIST_DARK,	89
	{ POINT_RESIST_CRITICAL,		},   // APPLY_ANTI_CRITICAL_PCT,	90
	{ POINT_RESIST_PENETRATE,		},   // APPLY_ANTI_PENETRATE_PCT,	91
	{ POINT_ATTBONUS_WOLFMAN,		},	 // APPLY_ATTBONUS_WOLFMAN,		//92 수인족에게 강함
	{ POINT_RESIST_WOLFMAN,			},	 // APPLY_RESIST_WOLFMAN,		//93 수인족에게 저항
	{ POINT_RESIST_CLAW,			},	 // APPLY_RESIST_CLAW,			//94 CLAW에게 저항
#ifdef WJ_MAGIC_REDUCION_BONUS
	{POINT_ANTI_RESIST_MAGIC,},	// APPLY_ANTI_RESIST_MAGIC 
#endif
#ifdef ADD_NEW_BONUS
	{POINT_ATT_METIN,},	// APPLY_ATT_METIN	96
	{POINT_RESIST_MONSTER,},
	{ POINT_ATTBONUS_ELEC,},
	{ POINT_ATTBONUS_FIRE, },
	{ POINT_ATTBONUS_ICE, },
	{ POINT_ATTBONUS_WIND, },
	{ POINT_ATTBONUS_EARTH, },
	{ POINT_ATTBONUS_DARK, },
	{ POINT_ANTI_SWORD,},
	{ POINT_ANTI_TWOHAND,},
	{ POINT_ANTI_DAGGER,},
	{ POINT_ANTI_BELL,},
	{ POINT_ANTI_FAN,},
	{ POINT_ANTI_BOW,},
	{ POINT_ANTI_HUMAN,},
	{ POINT_ATT_MONSTER_NEW,},
	{ POINT_ATT_BOSS,},	
	{ POINT_ATTBONUS_ELEMENT,},	
#endif
#ifdef ENABLE_GLOVE_SYSTEM
	{POINT_RANDOM,},
#endif
};

const int aiItemMagicAttributePercentHigh[ITEM_ATTRIBUTE_MAX_LEVEL] =
{
	//25, 25, 40, 8, 2,
	30, 40, 20, 8, 2
};

const int aiItemMagicAttributePercentLow[ITEM_ATTRIBUTE_MAX_LEVEL] =
{
	//45, 25, 20, 10, 0,
	50, 40, 10, 0, 0
};

// ADD_ITEM_ATTRIBUTE
const int aiItemAttributeAddPercent[ITEM_ATTRIBUTE_MAX_NUM] =
{
	100, 80, 60, 50, 30, 0, 0, 
};
// END_OF_ADD_ITEM_ATTRIBUTE

const int aiExpLossPercents[PLAYER_EXP_TABLE_MAX + 1] =
{
	0,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 4, // 1 - 10
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, // 11 - 20
	4, 4, 4, 4, 4, 4, 4, 3, 3, 3, // 21 - 30
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, // 31 - 40
	3, 3, 3, 3, 2, 2, 2, 2, 2, 2, // 41 - 50
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 51 - 60
	2, 2, 1, 1, 1, 1, 1, 1, 1, 1, // 61 - 70
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 71 - 80
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 81 - 90
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 91 - 100
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 101 - 110
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 111 - 120
};

const int aiSkillBookCountForLevelUp[10] =
{
	3, 3, 3, 3, 3, 4, 4, 5, 5, 6
};

// ADD_GRANDMASTER_SKILL
const int aiGrandMasterSkillBookCountForLevelUp[10] =
{
	3, 3, 5, 5, 7, 7, 10, 10, 10, 20, 
};

const int aiGrandMasterSkillBookMinCount[10] = 
{
//	1, 1, 3, 5, 10, 15, 20, 30, 40, 50, 
//	3, 3, 5, 5, 10, 10, 15, 15, 20, 30
	1, 1, 1, 2,  2,  3,  3,  4,  5,  6
};

const int aiGrandMasterSkillBookMaxCount[10] = 
{
//	6, 15, 30, 45, 60, 80, 100, 120, 160, 200,
//	6, 10, 15, 20, 30, 40, 50, 60, 70, 80
	5,  7,  9, 11, 13, 15, 20, 25, 30, 35
};
// END_OF_ADD_GRANDMASTER_SKILL

const int CHN_aiPartyBonusExpPercentByMemberCount[9] =
{
	0, 0, 12, 18, 26, 40, 53, 70, 100
};


// UPGRADE_PARTY_BONUS
const int KOR_aiPartyBonusExpPercentByMemberCount[9] =
{
	0, 
	0, 
	30, // 66% * 2 - 100  
	60, // 53% * 3 - 100 
	75, // 44% * 4 - 100
	90, // 38% * 5 - 100
	105, // 34% * 6 - 100
	110, // 30% * 7 - 100
	140, // 30% * 8 - 100
};

const int KOR_aiUniqueItemPartyBonusExpPercentByMemberCount[9] =
{
	0, 
	0, 
	15*2, 
	14*3, 
	13*4, 
	12*5, 
	11*6, 
	10*7, 
	10*8,
};
// END_OF_UPGRADE_PARTY_BONUS

const int * aiChainLightningCountBySkillLevel = NULL;

const int aiChainLightningCountBySkillLevel_euckr[SKILL_MAX_LEVEL+1] =
{
	0,	// 0
	2,	// 1
	2,	// 2
	2,	// 3
	2,	// 4
	2,	// 5
	2,	// 6
	2,	// 7
	2,	// 8
	3,	// 9
	3,	// 10
	3,	// 11
	3,	// 12
	3,	// 13
	3,	// 14
	3,	// 15
	3,	// 16
	3,	// 17
	3,	// 18
	4,	// 19
	4,	// 20
	4,	// 21
	4,	// 22
	4,	// 23
	5,	// 24
	5,	// 25
	5,	// 26
	5,	// 27
	5,	// 28
	5,	// 29
	5,	// 30
	5,	// 31
	5,	// 32
	5,	// 33
	5,	// 34
	5,	// 35
	5,	// 36
	5,	// 37
	5,	// 38
	5,	// 39
	5,	// 40
};

const SStoneDropInfo aStoneDrop[STONE_INFO_MAX_NUM] =
{
	//  mob		pct	{+0	+1	+2	+3	+4}
	{8005,	60,	{30,	30,	30,	9,	1}	},
	{8006,	60,	{28,	29,	31,	11,	1}	},

};

const char * c_apszEmpireNames[EMPIRE_MAX_NUM] =
{
	"전제국",
	"신수국",
	"천조국",
	"진노국"
};

const char * c_apszPrivNames[MAX_PRIV_NUM] =
{
	"",
	"아이템이 나올 확률",
	"돈이 나올 확률",
	"돈 대박이 나올 확률",
	"경험치 배율",
};

const int aiPolymorphPowerByLevel[SKILL_MAX_LEVEL + 1] =
{
	10,   // 1
	11,   // 2
	11,   // 3
	12,   // 4
	13,   // 5
	13,   // 6
	14,   // 7
	15,   // 8
	16,   // 9
	17,   // 10
	18,   // 11
	19,   // 12
	20,   // 13
	22,   // 14
	23,   // 15
	24,   // 16
	26,   // 17
	27,   // 18
	29,   // 19
	31,   // 20
	33,   // 21
	35,   // 22
	37,   // 23
	39,   // 24
	41,   // 25
	44,   // 26
	46,   // 27
	49,   // 28
	52,   // 29
	55,   // 30
	59,   // 31
	62,   // 32
	66,   // 33
	70,   // 34
	74,   // 35
	79,   // 36
	84,   // 37
	89,   // 38
	94,   // 39
	100,  // 40
};

TGuildWarInfo KOR_aGuildWarInfo[GUILD_WAR_TYPE_MAX_NUM] =
/*
   {
   long lMapIndex;
   int iWarPrice;
   int iWinnerPotionRewardPctToWinner;
   int iLoserPotionRewardPctToWinner;
   int iInitialScore;
   int iEndScore;
   };
 */
{
	{ 0,        0,      0,      0,      0,      0       },
	{ 110,      0,      100,    50,     0,      100     },
	{ 111,      0,      100,    50,     0,      10      },
};

//
// 악세서리 소켓용 수치들
//

// 다이아몬드로 소켓을 추가할 때 확률
const int aiAccessorySocketAddPct[ITEM_ACCESSORY_SOCKET_MAX_NUM] =
{
	50, 50, 50
};

// 악세서리 수치 값의 몇%만큼의 성능을 추가하는지
const int aiAccessorySocketEffectivePct[ITEM_ACCESSORY_SOCKET_MAX_NUM + 1] = 
{
	0, 10, 20, 40
};

// 소켓 지속시간 24, 12, 6
const int aiAccessorySocketDegradeTime[ITEM_ACCESSORY_SOCKET_MAX_NUM + 1] =
{
	0, 3600 * 24, 3600 * 12, 3600 * 6
};

// 소켓 장착 성공률
const int aiAccessorySocketPutPct[ITEM_ACCESSORY_SOCKET_MAX_NUM + 1] =
{
	90, 80, 70, 0
};
// END_OF_ACCESSORY_REFINE

#include "../../common/length.h"
// from import_item_proto.c
typedef struct SValueName
{
    const char *	c_pszName;
    long		lValue;
} TValueName;

TValueName c_aApplyTypeNames[] =
{
    { "STR",		APPLY_STR		},
    { "DEX",		APPLY_DEX		},
    { "CON",		APPLY_CON		},
    { "INT",		APPLY_INT		},
    { "MAX_HP",		APPLY_MAX_HP		},
    { "MAX_SP",		APPLY_MAX_SP		},
    { "MAX_STAMINA",	APPLY_MAX_STAMINA	},
    { "POISON_REDUCE",	APPLY_POISON_REDUCE	},
    { "EXP_DOUBLE_BONUS", APPLY_EXP_DOUBLE_BONUS },
    { "GOLD_DOUBLE_BONUS", APPLY_GOLD_DOUBLE_BONUS },
    { "ITEM_DROP_BONUS", APPLY_ITEM_DROP_BONUS	},
    { "HP_REGEN",	APPLY_HP_REGEN		},
    { "SP_REGEN",	APPLY_SP_REGEN		},
    { "ATTACK_SPEED",	APPLY_ATT_SPEED		},
    { "MOVE_SPEED",	APPLY_MOV_SPEED		},
    { "CAST_SPEED",	APPLY_CAST_SPEED	},
    { "ATT_BONUS",	APPLY_ATT_GRADE_BONUS	},
    { "DEF_BONUS",	APPLY_DEF_GRADE_BONUS	},
    { "MAGIC_ATT_GRADE",APPLY_MAGIC_ATT_GRADE	},
    { "MAGIC_DEF_GRADE",APPLY_MAGIC_DEF_GRADE	},
    { "SKILL",		APPLY_SKILL		},
    { "ATTBONUS_ANIMAL",APPLY_ATTBONUS_ANIMAL	},
    { "ATTBONUS_UNDEAD",APPLY_ATTBONUS_UNDEAD	},
    { "ATTBONUS_DEVIL", APPLY_ATTBONUS_DEVIL	},
    { "ATTBONUS_HUMAN", APPLY_ATTBONUS_HUMAN	},
    { "ADD_BOW_DISTANCE",APPLY_BOW_DISTANCE	},
    { "DODGE",		APPLY_DODGE		},
    { "BLOCK",		APPLY_BLOCK		},
    { "RESIST_SWORD",	APPLY_RESIST_SWORD	},
    { "RESIST_TWOHAND",	APPLY_RESIST_TWOHAND	},
    { "RESIST_DAGGER",	APPLY_RESIST_DAGGER    },
    { "RESIST_BELL",	APPLY_RESIST_BELL	},
    { "RESIST_FAN",	APPLY_RESIST_FAN	},
    { "RESIST_BOW",	APPLY_RESIST_BOW	},
    { "RESIST_FIRE",	APPLY_RESIST_FIRE	},
    { "RESIST_ELEC",	APPLY_RESIST_ELEC	},
    { "RESIST_MAGIC",	APPLY_RESIST_MAGIC	},
    { "RESIST_WIND",	APPLY_RESIST_WIND	},
	{ "REFLECT_MELEE",	APPLY_REFLECT_MELEE },
	{ "REFLECT_CURSE",	APPLY_REFLECT_CURSE },
    { "RESIST_ICE",		APPLY_RESIST_ICE	},
    { "RESIST_EARTH",	APPLY_RESIST_EARTH	},
    { "RESIST_DARK",	APPLY_RESIST_DARK	},
    { "RESIST_CRITICAL",	APPLY_ANTI_CRITICAL_PCT	},
    { "RESIST_PENETRATE",	APPLY_ANTI_PENETRATE_PCT	},
    { "POISON",		APPLY_POISON_PCT	},
    { "SLOW",		APPLY_SLOW_PCT		},
    { "STUN",		APPLY_STUN_PCT		},
    { "STEAL_HP",	APPLY_STEAL_HP		},
    { "STEAL_SP",	APPLY_STEAL_SP		},
    { "MANA_BURN_PCT",	APPLY_MANA_BURN_PCT	},
    { "CRITICAL",	APPLY_CRITICAL_PCT	},
    { "PENETRATE",	APPLY_PENETRATE_PCT	},
    { "KILL_SP_RECOVER",APPLY_KILL_SP_RECOVER	},
    { "KILL_HP_RECOVER",APPLY_KILL_HP_RECOVER	},
    { "PENETRATE_PCT",	APPLY_PENETRATE_PCT	},
    { "CRITICAL_PCT",	APPLY_CRITICAL_PCT	},
    { "POISON_PCT",	APPLY_POISON_PCT	},
    { "STUN_PCT",	APPLY_STUN_PCT		},
    { "ATT_BONUS_TO_WARRIOR",	APPLY_ATTBONUS_WARRIOR  },
    { "ATT_BONUS_TO_ASSASSIN",	APPLY_ATTBONUS_ASSASSIN },
    { "ATT_BONUS_TO_SURA",	APPLY_ATTBONUS_SURA	    },
    { "ATT_BONUS_TO_SHAMAN",	APPLY_ATTBONUS_SHAMAN   },
    { "ATT_BONUS_TO_MONSTER",	APPLY_ATTBONUS_MONSTER  },
    { "ATT_BONUS_TO_MOB",	APPLY_ATTBONUS_MONSTER  },
    { "MALL_ATTBONUS",	APPLY_MALL_ATTBONUS	},
    { "MALL_EXPBONUS",	APPLY_MALL_EXPBONUS	},
    { "MALL_DEFBONUS",	APPLY_MALL_DEFBONUS	},  
    { "MALL_ITEMBONUS",	APPLY_MALL_ITEMBONUS	},
    { "MALL_GOLDBONUS", APPLY_MALL_GOLDBONUS	},
    { "MAX_HP_PCT",	APPLY_MAX_HP_PCT	},
    { "MAX_SP_PCT",	APPLY_MAX_SP_PCT	},
    { "SKILL_DAMAGE_BONUS",	APPLY_SKILL_DAMAGE_BONUS	},
    { "NORMAL_HIT_DAMAGE_BONUS",APPLY_NORMAL_HIT_DAMAGE_BONUS	},
    { "SKILL_DEFEND_BONUS",	APPLY_SKILL_DEFEND_BONUS	},
    { "NORMAL_HIT_DEFEND_BONUS",APPLY_NORMAL_HIT_DEFEND_BONUS	},
    { "PCBANG_EXP_BONUS", APPLY_PC_BANG_EXP_BONUS	},
    { "PCBANG_DROP_BONUS", APPLY_PC_BANG_DROP_BONUS	},
    { "RESIST_WARRIOR",	APPLY_RESIST_WARRIOR},
    { "RESIST_ASSASSIN",	APPLY_RESIST_ASSASSIN},
    { "RESIST_SURA",		APPLY_RESIST_SURA},
    { "RESIST_SHAMAN",	APPLY_RESIST_SHAMAN},
    // by mhh game/affect.h 정의되어있음. INFINITE_AFFECT_DURATION = 0x1FFFFFFF
    { "INFINITE_AFFECT_DURATION", 0x1FFFFFFF	},
	{ "ENERGY", APPLY_ENERGY },		//	기력
	{ "COSTUME_ATTR_BONUS", APPLY_COSTUME_ATTR_BONUS },		//	기력
	{ "MAGIC_ATTBONUS_PER",	APPLY_MAGIC_ATTBONUS_PER	},
    { "MELEE_MAGIC_ATTBONUS_PER",	APPLY_MELEE_MAGIC_ATTBONUS_PER	},
	{ "ATT_BONUS_TO_WOLFMAN",APPLY_ATTBONUS_WOLFMAN },
	{ "RESIST_CLAW",APPLY_RESIST_CLAW },
	{ "RESIST_WOLFMAN",APPLY_RESIST_WOLFMAN },
#ifdef WJ_MAGIC_REDUCION_BONUS
	{"ANTI_RESIST_MAGIC", APPLY_ANTI_RESIST_MAGIC},
#endif
#ifdef ADD_NEW_BONUS
	{"ATTBONUS_METIN", APPLY_ATTBONUS_METIN},
	{"RESIST_MONSTER", APPLY_RESIST_MONSTER},
	{ "ATTBONUS_ELEC",	APPLY_ATTBONUS_ELEC,},
	{ "ATTBONUS_FIRE",	APPLY_ATTBONUS_FIRE,},
	{ "ATTBONUS_ICE",	APPLY_ATTBONUS_ICE,},
	{ "ATTBONUS_WIND",	APPLY_ATTBONUS_WIND,},
	{ "ATTBONUS_EARTH",	APPLY_ATTBONUS_EARTH,},
	{ "ATTBONUS_DARK",	APPLY_ATTBONUS_DARK,},
	{ "ANTI_SWORD",	APPLY_ANTI_SWORD,},
	{ "ANTI_TWOHAND",	APPLY_ANTI_TWOHAND,},
	{ "ANTI_DAGGER",	APPLY_ANTI_DAGGER,},
	{ "ANTI_BELL",	APPLY_ANTI_BELL,},
	{ "ANTI_FAN",	APPLY_ANTI_FAN,},
	{ "ANTI_BOW",	APPLY_ANTI_BOW,},
	{ "ANTI_HUMAN",	APPLY_ANTI_HUMAN,},
	{ "ATT_MONSTER_NEW",	APPLY_ATT_MONSTER_NEW,},
	{ "ATT_BOSS",	APPLY_ATT_BOSS,},	
	{ "ATTBONUS_ELEMENT",	APPLY_ATTBONUS_ELEMENT,},	
#endif
#ifdef ENABLE_GLOVE_SYSTEM
	{ "RANDOM", APPLY_RANDOM },
#endif
	{ NULL,		0			}
};
// from import_item_proto.c

long FN_get_apply_type(const char *apply_type_string)
{
	TValueName	*value_name;
	for (value_name = c_aApplyTypeNames; value_name->c_pszName; ++value_name)
	{
		if (0==strcasecmp(value_name->c_pszName, apply_type_string))
			return value_name->lValue;
	}
	return 0;
}
typedef struct SPetBilgi
{
    int petvnum;
    int	minstat;
    int	maxstat;
    int	maxskill;
    int	minsure;
    int	maxsure;
} TPetBilgi;

TPetBilgi c_PetBilgi[] =
{
    { 55401, 1, 25, 10, 50, 100 },
    { 55402, 1, 50, 10, 50, 100 },
    { 55403, 5, 50, 15, 50, 100 },
    { 55404, 5, 50, 15, 50, 100 },
    { 55405, 10, 50, 20, 50, 100 },
    { 55406, 10, 50, 20, 50, 100 },
    { 0, 0, 0, 0, 0, 0}
};
// #endif
long PetBilgiVer(int vnum, int idx)
{
	TPetBilgi	*value;
	for (value = c_PetBilgi; value->petvnum; ++value)
	{
		if (value->petvnum == vnum)
		{
			if (idx == 1)
			{
				return value->minstat;
			}
			else if (idx == 2)
			{
				return value->maxstat;
			}
			else if (idx == 3)
			{
				return value->maxskill;
			}
			else if (idx == 4)
			{
				return value->minsure;
			}
			else if (idx == 5)
			{
				return value->maxsure;
			}			
		}
	}
	return 0;
}
