#pragma once
#if defined(ENABLE_PET_SYSTEM) || defined(ENABLE_NEW_PET_SYSTEM)
#include "../gamelib/ItemData.h"
#include "../EterBase/Poly/Poly.h"

class CInstanceBase;

class CPythonSkillPet : public CSingleton<CPythonSkillPet>
{
	public:
		enum
		{
			SKILLPET_TYPE_NONE,
			SKILLPET_TYPE_PASSIVE,
			SKILLPET_TYPE_AUTO,			
			SKILLPET_TYPE_MAX_NUM,
		};

		enum ESkillPetDescTokenType
		{
			DESCPET_TOKEN_TYPE_VNUM,			
			DESCPET_TOKEN_TYPE_NAME,
			DESCPET_TOKEN_TYPE_ICON_NAME,
			DESCPET_TOKEN_TYPE,
			DESCPET_TOKEN_TYPE_DESCRIPTION,
			DESCPET_TOKEN_TYPE_DELAY,
			DESCPET_TOKEN_TYPE_MAX_NUM,

			CONDITIONPET_COLUMN_COUNT = 5,
			AFFECTPET_COLUMN_COUNT = 5,
			AFFECTPET_STEP_COUNT = 5,
			PET_SKILL_COUNT = 5,
		};
		

		int petslot[PET_SKILL_COUNT];

		typedef struct SSkillDataPet
		{
			// Functions
			SSkillDataPet();
			///////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////

			// Variable
			DWORD dwSkillIndex;
			std::string strName;
			std::string strIconFileName;
			BYTE byType;			
			std::string strDescription;
			DWORD dwskilldelay;			

			CGraphicImage * pImage;

			/////
			
			
		} TSkillDataPet;

		typedef std::map<DWORD, TSkillDataPet> TSkillDataPetMap;
		

	public:
		CPythonSkillPet();
		virtual ~CPythonSkillPet();

		void SetSkillbySlot(int slot, int skillIndex);


		BOOL GetSkillIndex(int slot, int * skillIndex);
		BOOL GetSkillData(DWORD dwSkillIndex, TSkillDataPet ** ppSkillData);
		void Destroy();			
		bool RegisterSkillPet(const char * c_szFileName);
		
	protected:
		//void __RegisterGradeIconImage(TSkillDataPet & rData, const char * c_szHeader, const char * c_szImageName);
		void __RegisterNormalIconImage(TSkillDataPet & rData, const char * c_szHeader, const char * c_szImageName);

	protected:
		TSkillDataPetMap m_SkillDataPetMap;
		std::map<std::string, DWORD> m_SkillPetTypeIndexMap;
		
};
#endif