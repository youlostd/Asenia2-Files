#pragma once

// NOTE : Item의 통합 관리 클래스다.
//        Icon, Model (droped on ground), Game Data

#include "../eterLib/GrpSubImage.h"
#include "../eterGrnLib/Thing.h"
#include "../UserInterface/Locale_inc.h"

#ifdef ENABLE_AURA_SYSTEM
	#include "../EffectLib/EffectManager.h"
#endif

class CItemData
{
	public:
		enum
		{
			ITEM_NAME_MAX_LEN = 30,
			ITEM_LIMIT_MAX_NUM = 2,
			ITEM_VALUES_MAX_NUM = 6,
			ITEM_SMALL_DESCR_MAX_LEN = 256,
			ITEM_APPLY_MAX_NUM = 5,
			ITEM_SOCKET_MAX_NUM = 3,
#ifdef ENABLE_SHINING_SYSTEM
			ITEM_SHINING_MAX_COUNT = 3,
#endif
		};

		enum EItemType
		{
			ITEM_TYPE_NONE,					//0
			ITEM_TYPE_WEAPON,				//1//무기
			ITEM_TYPE_ARMOR,				//2//갑옷
			ITEM_TYPE_USE,					//3//아이템 사용
			ITEM_TYPE_AUTOUSE,				//4
			ITEM_TYPE_MATERIAL,				//5
			ITEM_TYPE_SPECIAL,				//6 //스페셜 아이템
			ITEM_TYPE_TOOL,					//7
			ITEM_TYPE_LOTTERY,				//8//복권
			ITEM_TYPE_ELK,					//9//돈
			ITEM_TYPE_METIN,				//10
			ITEM_TYPE_CONTAINER,			//11
			ITEM_TYPE_FISH,					//12//낚시
			ITEM_TYPE_ROD,					//13
			ITEM_TYPE_RESOURCE,				//14
			ITEM_TYPE_CAMPFIRE,				//15
			ITEM_TYPE_UNIQUE,				//16
			ITEM_TYPE_SKILLBOOK,			//17
			ITEM_TYPE_QUEST,				//18
			ITEM_TYPE_POLYMORPH,			//19
			ITEM_TYPE_TREASURE_BOX,			//20//보물상자
			ITEM_TYPE_TREASURE_KEY,			//21//보물상자 열쇠
			ITEM_TYPE_SKILLFORGET,			//22
			ITEM_TYPE_GIFTBOX,				//23
			ITEM_TYPE_PICK,					//24
			ITEM_TYPE_HAIR,					//25//머리
			ITEM_TYPE_TOTEM,				//26//토템
			ITEM_TYPE_BLEND,				//27//생성될때 랜덤하게 속성이 붙는 약물
			ITEM_TYPE_COSTUME,				//28//코스츔 아이템 (2011년 8월 추가된 코스츔 시스템용 아이템)
			ITEM_TYPE_DS,					//29 //용혼석
			ITEM_TYPE_SPECIAL_DS,			//30 // 특수한 용혼석 (DS_SLOT에 착용하는 UNIQUE 아이템이라 생각하면 됨)
			ITEM_TYPE_EXTRACT,					//31 추출도구.
			ITEM_TYPE_SECONDARY_COIN,			//32 명도전.
			ITEM_TYPE_RING,						//33 반지 (유니크 슬롯이 아닌 순수 반지 슬롯)
			ITEM_TYPE_BELT,						//34 벨트
#ifdef ENABLE_CHEQUE_SYSTEM
			ITEM_TYPE_WON,					// 35 Nova moeda
#endif
#ifdef ENABLE_NEW_PET_SYSTEM
			ITEM_TYPE_PET,
#endif
			ITEM_TYPE_MAX_NUM,				
		};

		enum EPetSubTypes
		{
			PET_EXPFOOD,
		};

		enum EWeaponSubTypes
		{
			WEAPON_SWORD,
			WEAPON_DAGGER,	//이도류
			WEAPON_BOW,
			WEAPON_TWO_HANDED,
			WEAPON_BELL,
			WEAPON_FAN,
			WEAPON_ARROW,
			WEAPON_MOUNT_SPEAR,
			WEAPON_CLAW,
#ifdef ENABLE_NEW_ARROW_SYSTEM
			WEAPON_UNLIMITED_ARROW,
#endif
			WEAPON_NUM_TYPES,

			WEAPON_NONE = WEAPON_NUM_TYPES+1,
		};

		enum EMaterialSubTypes
		{
			MATERIAL_LEATHER,
			MATERIAL_BLOOD,
			MATERIAL_ROOT,
			MATERIAL_NEEDLE,
			MATERIAL_JEWEL,
			MATERIAL_DS_REFINE_NORMAL, 
			MATERIAL_DS_REFINE_BLESSED, 
			MATERIAL_DS_REFINE_HOLLY,
		};

		enum EArmorSubTypes
		{
			ARMOR_BODY,
			ARMOR_HEAD,
			ARMOR_SHIELD,
			ARMOR_WRIST,
			ARMOR_FOOTS,
			ARMOR_NECK,
			ARMOR_EAR,
			ARMOR_ELEMENT,
#ifdef ENABLE_GLOVE_SYSTEM
			ARMOR_GLOVE,
#endif
			ARMOR_NUM_TYPES
		};

		enum ECostumeSubTypes
		{
			COSTUME_BODY,				//0	갑옷(main look)
			COSTUME_HAIR,				//1	헤어(탈착가능)
			#ifdef ENABLE_SASH_SYSTEM
			COSTUME_SASH,
			#endif
#ifdef ENABLE_COSTUME_WEAPON_SYSTEM
			COSTUME_WEAPON,
#endif
			COSTUME_MOUNT,
#ifdef ENABLE_AURA_SYSTEM
			COSTUME_AURA,
#endif
			COSTUME_RING_SOCKET,
			COSTUME_RING_SOCKET_2,
			COSTUME_RING_SOCKET_3,
			COSTUME_RING_SOCKET_4,
			COSTUME_RING_SOCKET_5,
			COSTUME_RING_SOCKET_6,
			COSTUME_RING_SOCKET_7,
			COSTUME_RING_SOCKET_8,
			COSTUME_RING_SOCKET_9,
			COSTUME_RING_SOCKET_10,
			COSTUME_RING_SOCKET_11,
			COSTUME_RING_SOCKET_12,
			COSTUME_RING_SOCKET_13,
			COSTUME_RING_SOCKET_14,
			COSTUME_RING_SOCKET_15,
			COSTUME_RING_SOCKET_16,
			COSTUME_RING_SOCKET_17,
			COSTUME_RING_SOCKET_18,
			COSTUME_RING_SOCKET_19,
			COSTUME_RING_SOCKET_20,
			COSTUME_RING_SOCKET_21,
			COSTUME_RING_SOCKET_22,
			COSTUME_RING_SOCKET_23,
			COSTUME_RING_SOCKET_24,
			COSTUME_RING_SOCKET_25,
			COSTUME_RING_SOCKET_26,
			COSTUME_RING_SOCKET_27,
			COSTUME_RING_SOCKET_28,
			COSTUME_RING_SOCKET_29,
			COSTUME_RING_SOCKET_30,

			COSTUME_NUM_TYPES,
		};

		enum ENewEquipmentSubTypes
		{
#ifdef ENABLE_GLOVE_SYSTEM
			EQUIPMENT_NEW_GLOVE,
#endif
		};

		enum EUseSubTypes
		{
			USE_POTION,					// 0
			USE_TALISMAN,
			USE_TUNING,
			USE_MOVE,
			USE_TREASURE_BOX,
			USE_MONEYBAG,
			USE_BAIT,
			USE_ABILITY_UP,
			USE_AFFECT,
			USE_CREATE_STONE,
			USE_SPECIAL,				// 10
			USE_POTION_NODELAY,
			USE_CLEAR,
			USE_INVISIBILITY,
			USE_DETACHMENT,
			USE_BUCKET,
			USE_POTION_CONTINUE,
			USE_CLEAN_SOCKET,
			USE_CHANGE_ATTRIBUTE,
			USE_ADD_ATTRIBUTE,
			USE_ADD_ACCESSORY_SOCKET,	// 20
			USE_PUT_INTO_ACCESSORY_SOCKET,
			USE_ADD_ATTRIBUTE2,
			USE_RECIPE,
			USE_CHANGE_ATTRIBUTE2,
			USE_BIND,
			USE_UNBIND,
			USE_TIME_CHARGE_PER,
			USE_TIME_CHARGE_FIX,				// 28
			USE_PUT_INTO_BELT_SOCKET,			// 29 벨트 소켓에 사용할 수 있는 아이템 
			USE_PUT_INTO_RING_SOCKET,			// 30 반지 소켓에 사용할 수 있는 아이템 (유니크 반지 말고, 새로 추가된 반지 슬롯)
#ifdef ENABLE_COSTUME_ATTR_SYSTEM
			USE_COSTUME_ENCHANT,
			USE_COSTUME_TRANSFORM,
#endif
			USE_CHANGE_ATTRIBUTE_ELEMENT,
			USE_ADD_ATTRIBUTE_ELEMENT,
			USE_PET,
			USE_CHANGE_ATTRIBUTE_MOUNT,
			USE_ADD_ATTRIBUTE_MOUNT,
			USE_COSTUME_MOUNT_SKIN,
		};

		enum EDragonSoulSubType
		{
			DS_SLOT1,
			DS_SLOT2,
			DS_SLOT3,
			DS_SLOT4,
			DS_SLOT5,
			DS_SLOT6,
			DS_SLOT_NUM_TYPES = 6,
		};

		enum EMetinSubTypes
		{
			METIN_NORMAL,
			METIN_GOLD,
		};

		enum ELimitTypes
		{
			LIMIT_NONE,

			LIMIT_LEVEL,
			LIMIT_STR,
			LIMIT_DEX,
			LIMIT_INT,
			LIMIT_CON,
			LIMIT_PCBANG,

			/// 착용 여부와 상관 없이 실시간으로 시간 차감 (socket0에 소멸 시간이 박힘: unix_timestamp 타입)
			LIMIT_REAL_TIME,						

			/// 아이템을 맨 처음 사용(혹은 착용) 한 순간부터 리얼타임 타이머 시작 
			/// 최초 사용 전에는 socket0에 사용가능시간(초단위, 0이면 프로토의 limit value값 사용) 값이 쓰여있다가 
			/// 아이템 사용시 socket1에 사용 횟수가 박히고 socket0에 unix_timestamp 타입의 소멸시간이 박힘.
			LIMIT_REAL_TIME_START_FIRST_USE,

			/// 아이템을 착용 중일 때만 사용 시간이 차감되는 아이템
			/// socket0에 남은 시간이 초단위로 박힘. (아이템 최초 사용시 해당 값이 0이면 프로토의 limit value값을 socket0에 복사)
			LIMIT_TIMER_BASED_ON_WEAR,

			LIMIT_MAX_NUM
		};

		enum EItemAntiFlag
		{
			ITEM_ANTIFLAG_FEMALE        = (1 << 0),		// 여성 사용 불가
			ITEM_ANTIFLAG_MALE          = (1 << 1),		// 남성 사용 불가
			ITEM_ANTIFLAG_WARRIOR       = (1 << 2),		// 무사 사용 불가
			ITEM_ANTIFLAG_ASSASSIN      = (1 << 3),		// 자객 사용 불가
			ITEM_ANTIFLAG_SURA          = (1 << 4),		// 수라 사용 불가 
			ITEM_ANTIFLAG_SHAMAN        = (1 << 5),		// 무당 사용 불가
			ITEM_ANTIFLAG_GET           = (1 << 6),		// 집을 수 없음
			ITEM_ANTIFLAG_DROP          = (1 << 7),		// 버릴 수 없음
			ITEM_ANTIFLAG_SELL          = (1 << 8),		// 팔 수 없음
			ITEM_ANTIFLAG_EMPIRE_A      = (1 << 9),		// A 제국 사용 불가
			ITEM_ANTIFLAG_EMPIRE_B      = (1 << 10),	// B 제국 사용 불가
			ITEM_ANTIFLAG_EMPIRE_R      = (1 << 11),	// C 제국 사용 불가
			ITEM_ANTIFLAG_SAVE          = (1 << 12),	// 저장되지 않음
			ITEM_ANTIFLAG_GIVE          = (1 << 13),	// 거래 불가
			ITEM_ANTIFLAG_PKDROP        = (1 << 14),	// PK시 떨어지지 않음
			ITEM_ANTIFLAG_STACK         = (1 << 15),	// 합칠 수 없음
			ITEM_ANTIFLAG_MYSHOP        = (1 << 16),	// 개인 상점에 올릴 수 없음
			ITEM_ANTIFLAG_SAFEBOX		= (1 << 17),
			ITEM_ANTIFLAG_WOLFMAN		= (1 << 18),
			ITEM_ANTIFLAG_MY_OFFLINE_SHOP = (1 << 19),
			ITEM_ANTIFLAG_SELL_WITH_METIN          = (1 << 20),
		};

		enum EItemFlag
		{
			ITEM_FLAG_REFINEABLE        = (1 << 0),		// 개량 가능
			ITEM_FLAG_SAVE              = (1 << 1),
			ITEM_FLAG_STACKABLE         = (1 << 2),     // 여러개 합칠 수 있음
			ITEM_FLAG_COUNT_PER_1GOLD   = (1 << 3),		// 가격이 개수 / 가격으로 변함
			ITEM_FLAG_SLOW_QUERY        = (1 << 4),		// 게임 종료시에만 SQL에 쿼리함
			ITEM_FLAG_RARE              = (1 << 5),
			ITEM_FLAG_UNIQUE            = (1 << 6),
			ITEM_FLAG_MAKECOUNT			= (1 << 7),
			ITEM_FLAG_IRREMOVABLE		= (1 << 8),
			ITEM_FLAG_CONFIRM_WHEN_USE	= (1 << 9),
			ITEM_FLAG_QUEST_USE         = (1 << 10),    // 퀘스트 스크립트 돌리는지?
			ITEM_FLAG_QUEST_USE_MULTIPLE= (1 << 11),    // 퀘스트 스크립트 돌리는지?
			ITEM_FLAG_UNUSED03          = (1 << 12),    // UNUSED03
			ITEM_FLAG_LOG               = (1 << 13),    // 사용시 로그를 남기는 아이템인가?
			ITEM_FLAG_APPLICABLE		= (1 << 14),
		};

		enum EWearPositions
		{
			WEAR_BODY,          // 0
			WEAR_HEAD,          // 1
			WEAR_FOOTS,         // 2
			WEAR_WRIST,         // 3
			WEAR_WEAPON,        // 4
			WEAR_NECK,          // 5
			WEAR_EAR,           // 6
			WEAR_UNIQUE1,       // 7
			WEAR_UNIQUE2,       // 8
			WEAR_ARROW,         // 9
			WEAR_SHIELD,        // 10
			WEAR_COSTUME_HAIR,  //11
			WEAR_COSTUME_SASH,      //12
			WEAR_COSTUME_WEAPON,    //13
			WEAR_COSTUME_MOUNT,     //14
			WEAR_COSTUME_AURA,      //15
			WEAR_COSTUME_RING_SOCKET,
			WEAR_COSTUME_RING_SOCKET_2,
			WEAR_COSTUME_RING_SOCKET_3,
			WEAR_COSTUME_RING_SOCKET_4,
			WEAR_COSTUME_RING_SOCKET_5,
			WEAR_COSTUME_RING_SOCKET_6,
			WEAR_COSTUME_RING_SOCKET_7,
			WEAR_COSTUME_RING_SOCKET_8,
			WEAR_COSTUME_RING_SOCKET_9,
			WEAR_COSTUME_RING_SOCKET_10,
			WEAR_COSTUME_RING_SOCKET_11,
			WEAR_COSTUME_RING_SOCKET_12,
			WEAR_COSTUME_RING_SOCKET_13,
			WEAR_COSTUME_RING_SOCKET_14,
			WEAR_COSTUME_RING_SOCKET_15,
			WEAR_COSTUME_RING_SOCKET_16,
			WEAR_COSTUME_RING_SOCKET_17,
			WEAR_COSTUME_RING_SOCKET_18,
			WEAR_COSTUME_RING_SOCKET_19,
			WEAR_COSTUME_RING_SOCKET_20,
			WEAR_COSTUME_RING_SOCKET_21,
			WEAR_COSTUME_RING_SOCKET_22,
			WEAR_COSTUME_RING_SOCKET_23,
			WEAR_COSTUME_RING_SOCKET_24,
			WEAR_COSTUME_RING_SOCKET_25,
			WEAR_COSTUME_RING_SOCKET_26,
			WEAR_COSTUME_RING_SOCKET_27,
			WEAR_COSTUME_RING_SOCKET_28,
			WEAR_COSTUME_RING_SOCKET_29,
			WEAR_COSTUME_RING_SOCKET_30,
			WEAR_ELEMENT,
			WEAR_GLOVE,
			WEAR_MAX_NUM = 80,
		};

		enum EItemWearableFlag
		{
			WEARABLE_BODY	= (1 << 0),
			WEARABLE_HEAD	= (1 << 1),
			WEARABLE_FOOTS	= (1 << 2),
			WEARABLE_WRIST	= (1 << 3),
			WEARABLE_WEAPON	= (1 << 4),
			WEARABLE_NECK	= (1 << 5),
			WEARABLE_EAR	= (1 << 6),
			WEARABLE_UNIQUE	= (1 << 7),
			WEARABLE_SHIELD	= (1 << 8),
			WEARABLE_ARROW	= (1 << 9),
			WEARABLE_HAIR	= (1 << 10),
			WEARABLE_ELEMENT		= (1 << 11),
#ifdef ENABLE_GLOVE_SYSTEM
			WEARABLE_GLOVE = (1 << 12),
#endif
			WEARABLE_ABILITY		= (1 << 13),
			WEARABLE_COSTUME_BODY	= (1 << 14),
			WEARABLE_COSTUME_HAIR = (1 << 15),
#ifdef __SASH_SYSTEM__
			WEARABLE_COSTUME_SASH = (1 << 16),
#endif
#ifdef __WEAPON_COSTUME_SYSTEM__
			WEARABLE_COSTUME_WEAPON = (1 << 17),
#endif
			WEARABLE_COSTUME_MOUNT = (1 << 18),
			WEARABLE_COSTUME_AURA = (1 << 19),
		};

		enum EApplyTypes
		{
			APPLY_NONE,                 // 0
			APPLY_MAX_HP,               // 1
			APPLY_MAX_SP,               // 2
			APPLY_CON,                  // 3
			APPLY_INT,                  // 4
			APPLY_STR,                  // 5
			APPLY_DEX,                  // 6
			APPLY_ATT_SPEED,            // 7
			APPLY_MOV_SPEED,            // 8
			APPLY_CAST_SPEED,           // 9
			APPLY_HP_REGEN,             // 10
			APPLY_SP_REGEN,             // 11
			APPLY_POISON_PCT,           // 12
			APPLY_STUN_PCT,             // 13
			APPLY_SLOW_PCT,             // 14
			APPLY_CRITICAL_PCT,         // 15
			APPLY_PENETRATE_PCT,        // 16
			APPLY_ATTBONUS_HUMAN,       // 17
			APPLY_ATTBONUS_ANIMAL,      // 18
			APPLY_ATTBONUS_ORC,         // 19
			APPLY_ATTBONUS_MILGYO,      // 20
			APPLY_ATTBONUS_UNDEAD,      // 21
			APPLY_ATTBONUS_DEVIL,       // 22
			APPLY_STEAL_HP,             // 23
			APPLY_STEAL_SP,             // 24
			APPLY_MANA_BURN_PCT,        // 25
			APPLY_DAMAGE_SP_RECOVER,    // 26
			APPLY_BLOCK,                // 27
			APPLY_DODGE,                // 28
			APPLY_RESIST_SWORD,         // 29
			APPLY_RESIST_TWOHAND,       // 30
			APPLY_RESIST_DAGGER,        // 31
			APPLY_RESIST_BELL,          // 32
			APPLY_RESIST_FAN,           // 33
			APPLY_RESIST_BOW,           // 34
			APPLY_RESIST_FIRE,          // 35
			APPLY_RESIST_ELEC,          // 36
			APPLY_RESIST_MAGIC,         // 37
			APPLY_RESIST_WIND,          // 38
			APPLY_REFLECT_MELEE,        // 39
			APPLY_REFLECT_CURSE,        // 40
			APPLY_POISON_REDUCE,        // 41
			APPLY_KILL_SP_RECOVER,      // 42
			APPLY_EXP_DOUBLE_BONUS,     // 43
			APPLY_GOLD_DOUBLE_BONUS,    // 44
			APPLY_ITEM_DROP_BONUS,      // 45
			APPLY_POTION_BONUS,         // 46
			APPLY_KILL_HP_RECOVER,      // 47
			APPLY_IMMUNE_STUN,          // 48
			APPLY_IMMUNE_SLOW,          // 49
			APPLY_IMMUNE_FALL,          // 50
			APPLY_SKILL,                // 51
			APPLY_BOW_DISTANCE,         // 52
			APPLY_ATT_GRADE_BONUS,            // 53
			APPLY_DEF_GRADE_BONUS,            // 54
			APPLY_MAGIC_ATT_GRADE,      // 55
			APPLY_MAGIC_DEF_GRADE,      // 56
			APPLY_CURSE_PCT,            // 57
			APPLY_MAX_STAMINA,			// 58
			APPLY_ATT_BONUS_TO_WARRIOR, // 59
			APPLY_ATT_BONUS_TO_ASSASSIN,// 60
			APPLY_ATT_BONUS_TO_SURA,    // 61
			APPLY_ATT_BONUS_TO_SHAMAN,  // 62
			APPLY_ATT_BONUS_TO_MONSTER, // 63
			APPLY_MALL_ATTBONUS,        // 64 공격력 +x%
			APPLY_MALL_DEFBONUS,        // 65 방어력 +x%
			APPLY_MALL_EXPBONUS,        // 66 경험치 +x%
			APPLY_MALL_ITEMBONUS,       // 67 아이템 드롭율 x/10배
			APPLY_MALL_GOLDBONUS,       // 68 돈 드롭율 x/10배
			APPLY_MAX_HP_PCT,           // 69 최대 생명력 +x%
			APPLY_MAX_SP_PCT,           // 70 최대 정신력 +x%
			APPLY_SKILL_DAMAGE_BONUS,   // 71 스킬 데미지 * (100+x)%
			APPLY_NORMAL_HIT_DAMAGE_BONUS,      // 72 평타 데미지 * (100+x)%
			APPLY_SKILL_DEFEND_BONUS,   // 73 스킬 데미지 방어 * (100-x)%
			APPLY_NORMAL_HIT_DEFEND_BONUS,      // 74 평타 데미지 방어 * (100-x)%
			APPLY_EXTRACT_HP_PCT,		//75
			APPLY_PC_BANG_EXP_BONUS,		//76
			APPLY_PC_BANG_DROP_BONUS,		//77
			APPLY_RESIST_WARRIOR,			//78
			APPLY_RESIST_ASSASSIN ,			//79
			APPLY_RESIST_SURA,				//80
			APPLY_RESIST_SHAMAN,			//81
			APPLY_ENERGY,					//82
			APPLY_DEF_GRADE,				// 83 방어력. DEF_GRADE_BONUS는 클라에서 두배로 보여지는 의도된 버그(...)가 있다.
			APPLY_COSTUME_ATTR_BONUS,		// 84 코스튬 아이템에 붙은 속성치 보너스
			APPLY_MAGIC_ATTBONUS_PER,		// 85 마법 공격력 +x%
			APPLY_MELEE_MAGIC_ATTBONUS_PER,			// 86 마법 + 밀리 공격력 +x%
			
			APPLY_RESIST_ICE,		// 87 냉기 저항
			APPLY_RESIST_EARTH,		// 88 대지 저항
			APPLY_RESIST_DARK,		// 89 어둠 저항

			APPLY_ANTI_CRITICAL_PCT,	//90 크리티컬 저항
			APPLY_ANTI_PENETRATE_PCT,	//91 관통타격 저항

			APPLY_ATT_BONUS_TO_WOLFMAN,
			APPLY_RESIST_WOLFMAN,

			APPLY_RESIST_CLAW,
#ifdef WJ_MAGIC_REDUCION_BONUS
			APPLY_ANTI_RESIST_MAGIC,	// 
#endif
#ifdef ADD_NEW_BONUS
			APPLY_ATTBONUS_METIN,
			APPLY_RESIST_MONSTER,
			APPLY_ATTBONUS_ELEC,
			APPLY_ATTBONUS_FIRE,
			APPLY_ATTBONUS_ICE,
			APPLY_ATTBONUS_WIND,
			APPLY_ATTBONUS_EARTH,
			APPLY_ATTBONUS_DARK,
			APPLY_ANTI_SWORD,
			APPLY_ANTI_TWOHAND,
			APPLY_ANTI_DAGGER,
			APPLY_ANTI_BELL,
			APPLY_ANTI_FAN,
			APPLY_ANTI_BOW,
			APPLY_ANTI_HUMAN,
			APPLY_ATT_MONSTER_NEW,
			APPLY_ATT_BOSS,
			APPLY_ATTBONUS_ELEMENT,
#endif
#ifdef ENABLE_GLOVE_SYSTEM
			APPLY_RANDOM,
#endif
			MAX_APPLY_NUM,              // 
		};

		enum EItemMaskType
		{
			MASK_ITEM_TYPE_NONE,
			MASK_ITEM_TYPE_MOUNT_PET,
			MASK_ITEM_TYPE_EQUIPMENT_WEAPON,
			MASK_ITEM_TYPE_EQUIPMENT_ARMOR,
			MASK_ITEM_TYPE_EQUIPMENT_JEWELRY,
			MASK_ITEM_TYPE_TUNING,
			MASK_ITEM_TYPE_POTION,
			MASK_ITEM_TYPE_FISHING_PICK,
			MASK_ITEM_TYPE_DRAGON_STONE,
			MASK_ITEM_TYPE_COSTUMES,
			MASK_ITEM_TYPE_SKILL,
			MASK_ITEM_TYPE_UNIQUE,
			MASK_ITEM_TYPE_ETC,
			MASK_ITEM_TYPE_MAX
		};

		enum EPetMaskSubType
		{
			MASK_ITEM_SUBTYPE_MOUNT_PET_MOUNT,
			MASK_ITEM_SUBTYPE_MOUNT_PET_CHARGED_PET,
			MASK_ITEM_SUBTYPE_MOUNT_PET_FREE_PET,
			MASK_ITEM_SUBTYPE_MOUNT_PET_EGG,
		};

		enum EWeaponMaskSubType
		{
			MASK_ITEM_SUBTYPE_WEAPON_WEAPON_SWORD,
			MASK_ITEM_SUBTYPE_WEAPON_WEAPON_DAGGER,
			MASK_ITEM_SUBTYPE_WEAPON_WEAPON_BOW,
			MASK_ITEM_SUBTYPE_WEAPON_WEAPON_TWO_HANDED,
			MASK_ITEM_SUBTYPE_WEAPON_WEAPON_BELL,
			MASK_ITEM_SUBTYPE_WEAPON_WEAPON_CLAW,
			MASK_ITEM_SUBTYPE_WEAPON_WEAPON_FAN,
			MASK_ITEM_SUBTYPE_WEAPON_WEAPON_MOUNT_SPEAR,
			MASK_ITEM_SUBTYPE_WEAPON_WEAPON_ARROW,
			MASK_ITEM_SUBTYPE_WEAPON_WEAPON_QUIVER,
		};

		enum EArmorMaskSubType
		{
			MASK_ITEM_SUBTYPE_ARMOR_ARMOR_BODY,
			MASK_ITEM_SUBTYPE_ARMOR_ARMOR_HEAD,
			MASK_ITEM_SUBTYPE_ARMOR_ARMOR_SHIELD,
		};

		enum EJewelryMaskSubType
		{
			MASK_ITEM_SUBTYPE_JEWELRY_ARMOR_WRIST,
			MASK_ITEM_SUBTYPE_JEWELRY_ARMOR_FOOTS,
			MASK_ITEM_SUBTYPE_JEWELRY_ARMOR_NECK,
			MASK_ITEM_SUBTYPE_JEWELRY_ARMOR_EAR,
			MASK_ITEM_SUBTYPE_JEWELRY_ITEM_BELT,
			MASK_ITEM_SUBTYPE_JEWELRY_ITEM_ELEMENT,
		};

		enum ETuninMaskSubType
		{
			MASK_ITEM_SUBTYPE_TUNING_RESOURCE,
			MASK_ITEM_SUBTYPE_TUNING_STONE,
			MASK_ITEM_SUBTYPE_TUNING_ETC,
		};

		enum EPotionMaskSubType
		{
			MASK_ITEM_SUBTYPE_POTION_ABILITY,
			MASK_ITEM_SUBTYPE_POTION_HAIRDYE,
			MASK_ITEM_SUBTYPE_POTION_ETC,
		};

		enum EFishingMaskSubType
		{
			MASK_ITEM_SUBTYPE_FISHING_PICK_FISHING_POLE,
			MASK_ITEM_SUBTYPE_FISHING_PICK_EQUIPMENT_PICK,
			MASK_ITEM_SUBTYPE_FISHING_PICK_FOOD,
			MASK_ITEM_SUBTYPE_FISHING_PICK_STONE,
			MASK_ITEM_SUBTYPE_FISHING_PICK_ETC,
		};

		enum EDragonStoneMaskSubType
		{
			MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_DIAMOND,
			MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_RUBY,
			MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_JADE,
			MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_SAPPHIRE,
			MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_GARNET,
			MASK_ITEM_SUBTYPE_DRAGON_STONE_DRAGON_ONYX,
			MASK_ITEM_SUBTYPE_DRAGON_STONE_ETC,
		};

		enum ECostumeMaskSubType
		{
			MASK_ITEM_SUBTYPE_COSTUMES_COSTUME_WEAPON,
			MASK_ITEM_SUBTYPE_COSTUMES_COSTUME_BODY,
			MASK_ITEM_SUBTYPE_COSTUMES_COSTUME_HAIR,
			MASK_ITEM_SUBTYPE_COSTUMES_SASH,
			MASK_ITEM_SUBTYPE_COSTUMES_ETC,
		};

		enum ESkillMaskSubType
		{
			MASK_ITEM_SUBTYPE_SKILL_PAHAE,
			MASK_ITEM_SUBTYPE_SKILL_SKILL_BOOK,
			MASK_ITEM_SUBTYPE_SKILL_BOOK_OF_OBLIVION,
			MASK_ITEM_SUBTYPE_SKILL_ETC,
		};

		enum EUniqueMaskSubType
		{
			MASK_ITEM_SUBTYPE_UNIQUE_ABILITY,
			MASK_ITEM_SUBTYPE_UNIQUE_ETC,
		};

		enum EEtcMaskSubType
		{
			MASK_ITEM_SUBTYPE_ETC_GIFTBOX,
			MASK_ITEM_SUBTYPE_ETC_MATRIMORY,
			MASK_ITEM_SUBTYPE_ETC_EVENT,
			MASK_ITEM_SUBTYPE_ETC_SEAL,
			MASK_ITEM_SUBTYPE_ETC_PARTI,
			MASK_ITEM_SUBTYPE_ETC_POLYMORPH,
			MASK_ITEM_SUBTYPE_ETC_RECIPE,
			MASK_ITEM_SUBTYPE_ETC_ETC,
		};
		enum EImmuneFlags
		{
			IMMUNE_PARA         = (1 << 0),
			IMMUNE_CURSE        = (1 << 1),
			IMMUNE_STUN         = (1 << 2),
			IMMUNE_SLEEP        = (1 << 3),
			IMMUNE_SLOW         = (1 << 4),
			IMMUNE_POISON       = (1 << 5),
			IMMUNE_TERROR       = (1 << 6),
		};

#pragma pack(push)
#pragma pack(1)
		typedef struct SItemLimit
		{
			BYTE        bType;
			long        lValue;
		} TItemLimit;

		typedef struct SItemApply
		{
			BYTE        bType;
			long        lValue;
		} TItemApply;

		typedef struct SItemTable
		{
			DWORD       dwVnum;
			DWORD       dwVnumRange;
			char        szName[ITEM_NAME_MAX_LEN + 1];
			char        szLocaleName[ITEM_NAME_MAX_LEN + 1];
			BYTE        bType;
			BYTE        bSubType;
			
			BYTE        bWeight;
			BYTE        bSize;
			
			DWORD       dwAntiFlags;
			DWORD       dwFlags;
			DWORD       dwWearFlags;
			DWORD       dwImmuneFlag;
						
			DWORD       dwIBuyItemPrice;			
			DWORD		dwISellItemPrice;
			
			TItemLimit  aLimits[ITEM_LIMIT_MAX_NUM];
			TItemApply  aApplies[ITEM_APPLY_MAX_NUM];
			long        alValues[ITEM_VALUES_MAX_NUM];
			long        alSockets[ITEM_SOCKET_MAX_NUM];
			DWORD       dwRefinedVnum;
			WORD		wRefineSet;
			BYTE        bAlterToMagicItemPct;
			BYTE		bSpecular;
			BYTE        bGainSocketPct;
			BYTE		bMaskType;
			BYTE		bMaskSubType;
		} TItemTable;

		#ifdef ENABLE_SASH_SYSTEM
		struct SScaleInfo
		{
			float	fScaleX, fScaleY, fScaleZ;
			float	fPositionX, fPositionY, fPositionZ;
		};
		
		typedef struct SScaleTable
		{
			SScaleInfo	tInfo[10];
		} TScaleTable;
		#endif
//		typedef struct SItemTable
//		{
//			DWORD       dwVnum;
//			char        szItemName[ITEM_NAME_MAX_LEN + 1];
//			BYTE        bType;
//			BYTE        bSubType;
//			BYTE        bSize;
//			DWORD       dwAntiFlags;
//			DWORD       dwFlags;
//			DWORD       dwWearFlags;
//			DWORD       dwIBuyItemPrice;
//			DWORD		dwISellItemPrice;
//			TItemLimit  aLimits[ITEM_LIMIT_MAX_NUM];
//			TItemApply  aApplies[ITEM_APPLY_MAX_NUM];
//			long        alValues[ITEM_VALUES_MAX_NUM];
//			long        alSockets[ITEM_SOCKET_MAX_NUM];
//			DWORD       dwRefinedVnum;
//			BYTE		bSpecular;
//			DWORD		dwIconNumber;
//		} TItemTable;
#pragma pack(pop)

	public:
		CItemData();
		virtual ~CItemData();

		void Clear();
		void SetSummary(const std::string& c_rstSumm);
		void SetRed(const std::string& c_rstKirmizi);
		void SetBlue(const std::string& c_rstMavi);
		void SetGreen(const std::string& c_rstYesil);
		void SetYellow(const std::string& c_rstSari);
		void SetOrange(const std::string& c_rstTuruncu);
		void SetPink(const std::string& c_rstPembe);
		void SetPurple(const std::string& c_rstMor);
		void SetDescription(const std::string& c_rstDesc);

		CGraphicThing * GetModelThing();
		CGraphicThing * GetSubModelThing();
		CGraphicThing * GetDropModelThing();
		CGraphicSubImage * GetIconImage();

		DWORD GetLODModelThingCount();
		BOOL GetLODModelThingPointer(DWORD dwIndex, CGraphicThing ** ppModelThing);

		DWORD GetAttachingDataCount();
		BOOL GetCollisionDataPointer(DWORD dwIndex, const NRaceData::TAttachingData ** c_ppAttachingData);
		BOOL GetAttachingDataPointer(DWORD dwIndex, const NRaceData::TAttachingData ** c_ppAttachingData);

		/////
		const TItemTable*	GetTable() const;
		DWORD GetIndex() const;
		const char * GetName() const;
		const char * GetDescription() const;
		const char * GetSummary() const;
		const char * GetRed() const;
		const char * GetBlue() const;
		const char * GetGreen() const;
		const char * GetYellow() const;
		const char * GetOrange() const;
		const char * GetPink() const;
		const char * GetPurple() const;
		BYTE GetType() const;
		BYTE GetSubType() const;
		UINT GetRefine() const;
		const char* GetUseTypeString() const;
		DWORD GetWeaponType() const;
		BYTE GetSize() const;
		BOOL IsAntiFlag(DWORD dwFlag) const;
		BOOL IsFlag(DWORD dwFlag) const;
		BOOL IsWearableFlag(DWORD dwFlag) const;
		BOOL HasNextGrade() const;
		DWORD GetWearFlags() const;
		DWORD GetIBuyItemPrice() const;
		DWORD GetISellItemPrice() const;
		BOOL GetLimit(BYTE byIndex, TItemLimit * pItemLimit) const;
		BOOL GetApply(BYTE byIndex, TItemApply * pItemApply) const;
		long GetValue(BYTE byIndex) const;
		long GetSocket(BYTE byIndex) const;
		long SetSocket(BYTE byIndex,DWORD value);
		int GetSocketCount() const;
		DWORD GetIconNumber() const;

		int GetMaskType() const;
		int GetMaskSubType() const;
		UINT	GetSpecularPoweru() const;
		float	GetSpecularPowerf() const;
	
		/////

		BOOL IsEquipment() const;

		/////

		//BOOL LoadItemData(const char * c_szFileName);
		void SetDefaultItemData(const char * c_szIconFileName, const char * c_szModelFileName  = NULL);
		void SetItemTableData(TItemTable * pItemTable);

#ifdef ENABLE_SHINING_SYSTEM
	typedef struct SItemShiningTable
	{
		char szShinings[ITEM_SHINING_MAX_COUNT][256];
	public:
		bool Any() const
		{
			for (int i = 0; i < CItemData::ITEM_SHINING_MAX_COUNT; i++)
			{
				if (strcmp(szShinings[i], ""))
					return true;
			}
			return false;
		}
	} TItemShiningTable;
	void SetItemShiningTableData(BYTE bIndex, const char * szEffectname);
	CItemData::TItemShiningTable GetItemShiningTable() {return m_ItemShiningTable;}
#endif

#ifdef ENABLE_AURA_SYSTEM
	protected:
		DWORD m_dwAuraEffectID;
	public:
		void SetAuraEffectID(const char* szAuraEffectPath);
		DWORD GetAuraEffectID() const { return m_dwAuraEffectID; }
		
		enum EAuraMisc
		{
			AURA_GRADE_MAX_NUM = 6,
		};
#endif
		
		#ifdef ENABLE_SASH_SYSTEM
		void	SetItemScale(const std::string strJob, const std::string strSex, const std::string strScaleX, const std::string strScaleY, const std::string strScaleZ, const std::string strPositionX, const std::string strPositionY, const std::string strPositionZ);
		bool	GetItemScale(DWORD dwPos, float & fScaleX, float & fScaleY, float & fScaleZ, float & fPositionX, float & fPositionY, float & fPositionZ);
		#endif
	protected:
		void __LoadFiles();
		void __SetIconImage(const char * c_szFileName);

	protected:
		std::string m_strModelFileName;
		std::string m_strSubModelFileName;
		std::string m_strDropModelFileName;
		std::string m_strIconFileName;
		std::string m_strDescription;
		std::string m_strSummary;
		std::string m_strRed;
		std::string m_strBlue;
		std::string m_strGreen;
		std::string m_strYellow;
		std::string m_strOrange;
		std::string m_strPink;
		std::string m_strPurple;
		std::vector<std::string> m_strLODModelFileNameVector;

		CGraphicThing * m_pModelThing;
		CGraphicThing * m_pSubModelThing;
		CGraphicThing * m_pDropModelThing;
		CGraphicSubImage * m_pIconImage;
		std::vector<CGraphicThing *> m_pLODModelThingVector;

		NRaceData::TAttachingDataVector m_AttachingDataVector;
		DWORD		m_dwVnum;
		TItemTable m_ItemTable;
#ifdef ENABLE_SHINING_SYSTEM
		TItemShiningTable m_ItemShiningTable;
#endif
#ifdef ENABLE_SASH_SYSTEM
		TScaleTable m_ScaleTable;
#endif

	public:
		static void DestroySystem();

		static CItemData* New();
		static void Delete(CItemData* pkItemData);

		static CDynamicPool<CItemData>		ms_kPool;
};
