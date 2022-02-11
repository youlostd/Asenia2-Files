#ifndef __INC_MESSENGER_MANAGER_H
#define __INC_MESSENGER_MANAGER_H

#include "db.h"

#if defined(ENABLE_MESSENGER_BLOCK)
typedef std::string keyB;
typedef std::set<std::string> KeyBSet;
typedef std::map<std::string, KeyBSet> KeyBRelation;
#endif

class MessengerManager : public singleton<MessengerManager>
{
	public:
		typedef std::string keyT;
		typedef const std::string & keyA;

		MessengerManager();
		virtual ~MessengerManager();

	public:
		void	P2PLogin(keyA account);
		void	P2PLogout(keyA account);

		void	Login(keyA account);
		void	Logout(keyA account);

		void	RequestToAdd(LPCHARACTER ch, LPCHARACTER target);
		bool	AuthToAdd(keyA account, keyA companion, bool bDeny);

		void	__AddToList(keyA account, keyA companion);	// 실제 m_Relation, m_InverseRelation 수정하는 메소드
		void	AddToList(keyA account, keyA companion);

		void	__RemoveFromList(keyA account, keyA companion); // 실제 m_Relation, m_InverseRelation 수정하는 메소드
		void	RemoveFromList(keyA account, keyA companion);	

#if defined(ENABLE_MESSENGER_BLOCK)
		void	__AddToBlockList(const std::string& account, const std::string& companion);
		void	AddToBlockList(const std::string& account, const std::string& companion);
		bool	CheckMessengerList(std::string ch, std::string tch, BYTE type);
		void	__RemoveFromBlockList(const std::string& account, const std::string& companion);
		void	RemoveFromBlockList(const std::string& account, const std::string& companion);
		void	RemoveAllBlockList(const std::string& account);
#endif

		void	RemoveAllList(keyA account);

		void	Initialize();

	private:
		void	SendList(keyA account);
		void	SendLogin(keyA account, keyA companion);
		void	SendLogout(keyA account, keyA companion);

		void	LoadList(SQLMsg * pmsg);

#if defined(ENABLE_MESSENGER_BLOCK)
		void	SendBlockLogin(const std::string& account, const std::string& companion);
		void	SendBlockLogout(const std::string& account, const std::string& companion);
		void	LoadBlockList(SQLMsg * pmsg);
		void	SendBlockList(const std::string& account);
#endif

		void	Destroy();

		std::set<keyT>			m_set_loginAccount;
		std::map<keyT, std::set<keyT> >	m_Relation;
		std::map<keyT, std::set<keyT> >	m_InverseRelation;
		std::set<DWORD>			m_set_requestToAdd;

#if defined(ENABLE_MESSENGER_BLOCK)
		KeyBRelation m_BlockRelation;
		KeyBRelation m_InverseBlockRelation;
		std::set<DWORD> m_set_requestToBlockAdd;
#endif
};

#endif
