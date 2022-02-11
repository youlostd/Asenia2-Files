#ifndef __BOSS_TRACKINGS_H__
#define __BOSS_TRACKINGS_H__
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
// #include "packet.h"

enum EBossTracking
{
	BOSS_TRACKING_MAX_NUM = 6,
};

class CBossTracking : public singleton<CBossTracking>
{
	public:
		// typedef struct BossTracking
		// {
			// int BTKey;
			// int dead_time;
			// int regen_time;
			// BYTE channel;
			// DWORD mob_vnum;
			// DWORD map_index;

			// BossTracking()
			// {
				// BTKey = 0;
				// dead_time = 0;
				// regen_time = 0;
				// channel = 0;
				// mob_vnum = 0;
				// map_index = 0;
			// }
		// } TBossTracking;
		
		// std::vector<TBossTracking> m_bossTrackingVector;
		boost::unordered_map<int, DWORD>	dead_time;
		boost::unordered_map<int, DWORD>	regen_time;
		boost::unordered_map<int, DWORD>	channel;
		boost::unordered_map<int, DWORD>	mob_vnum;
		boost::unordered_map<int, DWORD>	map_index;
		
		CBossTracking();
		~CBossTracking();

		int GetDeadTime(BYTE channel, DWORD dwMobVnum);
		int GetRegenTime(BYTE channel, DWORD dwMobVnum);
		int GetMapIndex(BYTE channel, DWORD dwMobVnum);
		bool IsBossTrackingSystem();
		void SetDeadTime(BYTE xchannel, DWORD dwMobVnum, DWORD dwTime, DWORD xmap_index);
		void SetRegenTime(BYTE xchannel, DWORD dwMobVnum, DWORD dwTime, DWORD xmap_index);
		void SendP2PPacket(DWORD dwMobVnum, DWORD channel);
		bool IsTrackingMob(DWORD dwMobVnum);
		void SendClientPacket(LPCHARACTER pkChr, BYTE channel, DWORD dwMobVnum);
};
#endif