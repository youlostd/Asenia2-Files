#ifndef	__HEADER_SUPPORT_SYSTEM__
#define	__HEADER_SUPPORT_SYSTEM__
//Edited by LWT

class CHARACTER;


struct SShamanAbility
{
};

/**
*/
class CSupportActor //: public CHARACTER
{
public:
	enum EShamanOptions
	{
		ESupportOption_Followable		= 1 << 0,
		ESupportOption_Mountable		= 1 << 1,
		ESupportOption_Summonable		= 1 << 2,
		ESupportOption_Combatable		= 1 << 3,		
	};


protected:
	friend class CSupportSystem;

	CSupportActor(LPCHARACTER owner, DWORD vnum, DWORD options = ESupportOption_Followable | ESupportOption_Summonable);


	virtual ~CSupportActor();

	virtual bool	Update(DWORD deltaTime);

protected:
	virtual bool	_UpdateFollowAI();				///< 주인을 따라다니는 AI 처리
	virtual bool	_UpdatAloneActionAI(float fMinDist, float fMaxDist);			///< 주인 근처에서 혼자 노는 AI 처리


private:
	bool Follow(float fMinDistance = 50.f);

public:
	LPCHARACTER		GetCharacter()	const					{ return m_pkChar; }
	LPCHARACTER		GetOwner()	const						{ return m_pkOwner; }
	DWORD			GetVID() const							{ return m_dwVID; }
	DWORD			GetVnum() const							{ return m_dwVnum; }

	bool			HasOption(EShamanOptions option) const		{ return m_dwOptions & option; }

	void			SetName();
	void			SetLevel(DWORD level);

	DWORD			Summon(const char* supportName, LPITEM pSummonItem, bool bSpawnFar = false);
	void			Unsummon();
	bool			IsSummoned() const			{ return 0 != m_pkChar; }
	void			SetSummonItem (LPITEM pItem);
	DWORD			GetLevel() { return m_dwlevel; }
	void			SetLastSkillTime(DWORD time)	{ m_dwLastSkillTime = time; }
	void			SetCostume();
	void			SetHair();
	void			SetWeapon();
	DWORD			GetLastSkillTime()	{ return m_dwLastSkillTime; }
	void			UpdatePacketsupportActor();
	void			RefreshCostume();
	int				GetSupportVID();			
	void			UseSkill();
	DWORD			GetSummonItemVID () { return m_dwSummonItemVID; }

	void			GiveBuff();
	void			ClearBuff();

private:
	DWORD			m_dwVnum;
	DWORD			m_dwVID;
	DWORD			m_dwOptions;
	DWORD			m_dwLastActionTime;
	DWORD			m_dwSummonItemVID;
	DWORD			m_dwSummonItemVnum;
	DWORD			m_dwLastSkillTime;
	DWORD			m_dwlevel;
	short			m_originalMoveSpeed;
	LPCHARACTER		m_pkChar;
	LPCHARACTER		m_pkOwner;


};

/**
*/
class CSupportSystem
{
public:
	typedef	std::unordered_map<DWORD,	CSupportActor*>		TsupportActorMap;		

public:
	CSupportSystem(LPCHARACTER owner);
	virtual ~CSupportSystem();

	CSupportActor*	GetByVID(DWORD vid) const;
	CSupportActor*	GetByVnum(DWORD vnum) const;

	bool		Update(DWORD deltaTime);
	void		Destroy();

	size_t		CountSummoned() const;

	
public:
	void		SetUpdatePeriod(DWORD ms);

	CSupportActor*	Summon(DWORD mobVnum, LPITEM pSummonItem, const char* supportName, bool bSpawnFar, DWORD options = CSupportActor::ESupportOption_Followable | CSupportActor::ESupportOption_Summonable);

	void		Unsummon(DWORD mobVnum, bool bDeleteFromList = false);
	void		Unsummon(CSupportActor* supportActor, bool bDeleteFromList = false);
	int			GetLevel();
	bool		IsActiveSupport();
	CSupportActor*   GetActiveSupport();
	CSupportActor*	AddShaman(DWORD mobVnum, const char* supportName, const SShamanAbility& ability, DWORD options = CSupportActor::ESupportOption_Followable | CSupportActor::ESupportOption_Summonable | CSupportActor::ESupportOption_Combatable);

	void		DeleteSupport(DWORD mobVnum);
	void		DeleteSupport(CSupportActor* supportActor);
	void		RefreshBuff();
	
private:
	TsupportActorMap	m_supportActorMap;
	LPCHARACTER		m_pkOwner;					
	DWORD			m_dwUpdatePeriod;			
	DWORD			m_dwLastUpdateTime;
	LPEVENT			m_pksupportSystemUpdateEvent;
};



#endif	//__HEADER_SUPPORT_SYSTEM__