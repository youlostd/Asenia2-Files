#include "stdafx.h"
#include "constants.h"
#include "desc.h"
#include "db.h"
#include "utils.h"
#include "config.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "motion.h"
#include "packet.h"
#include "affect.h"
#include "pvp.h"
#include "cmd.h"
#include "start_position.h"
#include "party.h"
#include "guild_manager.h"
#include "p2p.h"
#include "dungeon.h"
#include "messenger_manager.h"
#include "war_map.h"
#include "questmanager.h"
#include "item_manager.h"
#include "mob_manager.h"
#include "dev_log.h"
#include "item.h"
#include "log.h"
#include "../../common/VnumHelper.h"
#include "guild.h"
#include "empire_text_convert.h"
#include "castle.h"
#include "locale_service.h"
#include <string>
#include <boost/algorithm/string.hpp>
#include "maintenance.h"
#include "input.h"

MaintenanceManager::MaintenanceManager()	{	}
MaintenanceManager::~MaintenanceManager()	{	}

EVENTINFO(maintenanceShutdown_event_data)
{
	int seconds;

	maintenanceShutdown_event_data()
		: seconds(0)
	{
	}
};

EVENTINFO(bakim_shutdown_event_data)
{
	int seconds;

	bakim_shutdown_event_data()
	: seconds( 0 )
	{
	}
};

static LPEVENT maintenance_check = NULL;

struct SendDisconnectFunc
{
	void operator () (LPDESC d)
	{
		if (d->GetCharacter())
		{
			if (d->GetCharacter()->GetGMLevel() == GM_PLAYER)
				d->GetCharacter()->ChatPacket(CHAT_TYPE_COMMAND, "quit Shutdown(SendDisconnectFunc)");
		}
	}
};

struct DisconnectFunc
{
	void operator () (LPDESC d)
	{
		if (d->GetType() == DESC_TYPE_CONNECTOR)
			return;

		if (d->IsPhase(PHASE_P2P))
			return;

		if (d->GetCharacter())
			d->GetCharacter()->Disconnect("Shutdown(DisconnectFunc)");

		d->SetPhase(PHASE_CLOSE);
	}
};

#define MAINTENANCE_TEXT_MIN_CHAR 5 // Minimum bakým nedeni karakter sayýsý
#define MAINTENANCE_TEXT_MAX_CHAR 30 // Maksimum bakým nedeni karakter sayýsý

#define MAINTENANCE_TIME_LEFT_MIN 300 // Minimum bakýma kalan süre
#define MAINTENANCE_TIME_LEFT_MAX 86400 // Maksimum bakýma kalan süre

#define MAINTENANCE_TIME_DURATION_MIN 300 // Minimum bakým süresi
#define MAINTENANCE_TIME_DURATION_MAX 86400 // Maksimum bakým süresi

#define SHUTDOWN_TIME 10 // Oyun kapanma süresi

#define MAINTENANCE_ADMIN_NAME		"[FOUNDER]RiseOfGame" // Bakým yapabilecek yetkili kiþinin ismi

const char* maintenance_translate[] = {
	"Maksimum bakýma kalan süre %u saniye olmalýdýr!",// 0
	"Maksimum bakým süresi %u saniye olmalýdýr!",// 1
	"Bakýma kalan süre %u olarak belirlendi.",// 2
	"Bakým süresi %u olarak belirlendi.",// 3
	"Bakým nedeni maksimum %u karakter olabilir!",// 4
	"Bakým nedeni en az %u karakterden oluþmalýdýr!",// 5
	"Bakým nedeni baþarýyla kaldýrýldý.",// 6
	"Bakým nedeni baþarýyla eklendi - %s",// 7
	"Minimum bakýma kalan süre %u saniye olmalýdýr!",// 8
	"Minimum bakým süresi %u saniye olmalýdýr!",// 9
	"Bu komuta eriþim için adýnýz %s olmalýdýr!"// 10
};

EVENTFUNC(maintenanceDown_event)
{
	maintenanceShutdown_event_data* info = dynamic_cast<maintenanceShutdown_event_data*>(event->info);

	if (info == NULL)
	{
		sys_err("maintenanceDown_event> <Factor> Time 0 - Error");
		return 0;
	}

	int * pSecondMaintenance = &(info->seconds);

	if (*pSecondMaintenance == 0 || *pSecondMaintenance < 1)
	{
		char sTime[128];
		char sDuration[128];
		char sReason[1024];

		snprintf(sTime, sizeof(sTime), "UPDATE player.maintenance SET time = %u", 0);
		snprintf(sDuration, sizeof(sDuration), "UPDATE player.maintenance SET duration = %u", 0);
		snprintf(sReason, sizeof(sReason), 
			"UPDATE player.maintenance SET "
			"reason = 'no_reason'"
		);

		DBManager::instance().DirectQuery(sTime);
		DBManager::instance().DirectQuery(sDuration);
		DBManager::instance().DirectQuery(sReason);

		TPacketGGShutdown p;
		p.bHeader = HEADER_GG_SHUTDOWN;
		P2P_MANAGER::instance().Send(&p, sizeof(TPacketGGShutdown));

		MaintenanceManager::instance().ShutdownMaintenance(SHUTDOWN_TIME);
	}
	else
	{
		char sTime[128];
		snprintf(sTime, sizeof(sTime), "UPDATE player.maintenance SET time = %u", *pSecondMaintenance);
		DBManager::instance().DirectQuery(sTime);

		if (*pSecondMaintenance < 1)
		{
			return 0;
		}

		--*pSecondMaintenance;
		return passes_per_sec;
	}

	maintenance_check = NULL;
	return 0;
}

EVENTFUNC(bakim_shutdown_event)
{
	bakim_shutdown_event_data* info = dynamic_cast<bakim_shutdown_event_data*>( event->info );

	if ( info == NULL )
	{
		sys_err( "bakim_shutdown_event> <Factor> Null pointer" );
		return 0;
	}

	int * pSec = & (info->seconds);

	if (*pSec < 0)
	{
		sys_log(0, "bakim_shutdown_event sec %d", *pSec);

		if (--*pSec == -10)
		{
			const DESC_MANAGER::DESC_SET & c_set_desc = DESC_MANAGER::instance().GetClientSet();
			std::for_each(c_set_desc.begin(), c_set_desc.end(), DisconnectFunc());
			return passes_per_sec;
		}
		else if (*pSec < -10)
			return 0;

		return passes_per_sec;
	}
	else if (*pSec == 0)
	{
		const DESC_MANAGER::DESC_SET & c_set_desc = DESC_MANAGER::instance().GetClientSet();
		std::for_each(c_set_desc.begin(), c_set_desc.end(), SendDisconnectFunc());
		g_bNoMoreClient = true;
		--*pSec;
		return passes_per_sec;
	}
	else
	{
		char buf[64];
		snprintf(buf, sizeof(buf), "%d saniye sonra oyun kapanacak.", *pSec);
		SendNotice(buf);

		--*pSec;
		return passes_per_sec;
	}
}

void StartMaintenance(int iSec)
{
	maintenanceShutdown_event_data* info = AllocEventInfo<maintenanceShutdown_event_data>();
	info->seconds = iSec;
	maintenance_check = event_create(maintenanceDown_event, info, 1);
}

void MaintenanceManager::ShutdownMaintenance(int iSec)
{
	if (g_bNoMoreClient)
	{
		thecore_shutdown();
		return;
	}

	CWarMapManager::instance().OnShutdown();

	char buf[64];
	snprintf(buf, sizeof(buf), "Oyun %d saniye sonra kapanacak.", iSec);

	SendNotice(buf);

	bakim_shutdown_event_data* info = AllocEventInfo<bakim_shutdown_event_data>();
	info->seconds = iSec;

	event_create(bakim_shutdown_event, info, 1);
}

void MaintenanceManager::Send_DisableSecurity(LPCHARACTER ch)
{
	if (!ch)
	{
		return;
	}

	const std::string & szName = ch->GetName();

	if (!szName.c_str() || szName.c_str() == NULL)
	{
		return;
	}

	if (szName != MAINTENANCE_ADMIN_NAME)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, maintenance_translate[10], MAINTENANCE_ADMIN_NAME);
		return;
	}

	if (maintenance_check)
	{
		event_cancel(&maintenance_check);
		maintenance_check = NULL;
	}

	char sTime[128];
	char sDuration[128];
	char sReason[1024];

	snprintf(sTime, sizeof(sTime), "UPDATE player.maintenance SET time = %u", 0);
	snprintf(sDuration, sizeof(sDuration), "UPDATE player.maintenance SET duration = %u", 0);
	snprintf(sReason, sizeof(sReason), 
		"UPDATE player.maintenance SET "
		"reason = 'no_reason'"
	);

	DBManager::instance().DirectQuery(sTime);
	DBManager::instance().DirectQuery(sDuration);
	DBManager::instance().DirectQuery(sReason);
}

void MaintenanceManager::Send_ActiveMaintenance(LPCHARACTER ch, long int time_maintenance, long int duration_maintenance)
{
	if (!ch || ch == NULL)
		return;

	if (!ch->IsPC())
		return;

	const std::string & szName = ch->GetName();

	if (!szName.c_str() || szName.c_str() == NULL)
	{
		return;
	}

	if (szName != MAINTENANCE_ADMIN_NAME)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, maintenance_translate[10], MAINTENANCE_ADMIN_NAME);
		return;
	}

	if (time_maintenance < MAINTENANCE_TIME_LEFT_MIN)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, maintenance_translate[8], MAINTENANCE_TIME_LEFT_MIN);
		return;
	}
	else if (time_maintenance > MAINTENANCE_TIME_LEFT_MAX)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, maintenance_translate[0], MAINTENANCE_TIME_LEFT_MAX);
		return;
	}
	else if (duration_maintenance < MAINTENANCE_TIME_DURATION_MIN)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, maintenance_translate[9], MAINTENANCE_TIME_DURATION_MIN);
		return;
	}
	else if (duration_maintenance > MAINTENANCE_TIME_DURATION_MAX)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, maintenance_translate[1], MAINTENANCE_TIME_DURATION_MAX);
		return;
	}
	else
	{
		char sDuration[128];
		snprintf(sDuration, sizeof(sDuration), "UPDATE player.maintenance SET duration = %ld", duration_maintenance);
		DBManager::instance().DirectQuery(sDuration);

		StartMaintenance(time_maintenance);

		ch->ChatPacket(CHAT_TYPE_INFO, maintenance_translate[2], time_maintenance);
		ch->ChatPacket(CHAT_TYPE_INFO, maintenance_translate[3], duration_maintenance);
	}
}

void MaintenanceManager::Send_Text(LPCHARACTER ch, const char* reason)
{
	if (!ch || ch == NULL)
		return;

	if (!ch->IsPC())
		return;

	const std::string & szName = ch->GetName();

	if (!szName.c_str() || szName.c_str() == NULL)
	{
		return;
	}

	if (szName != MAINTENANCE_ADMIN_NAME)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, maintenance_translate[10], MAINTENANCE_ADMIN_NAME);
		return;
	}

	if (!*reason)
	{
		return;
	}

	if (strlen(reason) > MAINTENANCE_TEXT_MAX_CHAR)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, maintenance_translate[4], MAINTENANCE_TEXT_MAX_CHAR);
		return;
	}

	if (strlen(reason) < MAINTENANCE_TEXT_MIN_CHAR && !!strcmp(reason, "rmf"))
	{
		ch->ChatPacket(CHAT_TYPE_INFO, maintenance_translate[5], MAINTENANCE_TEXT_MIN_CHAR);
		return;
	}

	if (!strcmp(reason, "rmf"))
	{
		char sReason[1024];
		snprintf(sReason, sizeof(sReason), "UPDATE player.maintenance SET reason = 'no_reason'");
		DBManager::instance().DirectQuery(sReason);

		ch->ChatPacket(CHAT_TYPE_INFO, maintenance_translate[6]);
		return;
	}

	char sReason[1024];
	snprintf(sReason, sizeof(sReason), "UPDATE player.maintenance SET `reason` = replace(\"%s\",' ','//')", reason);
	DBManager::instance().DirectQuery(sReason);

	ch->ChatPacket(CHAT_TYPE_INFO, maintenance_translate[7], reason);
}

void MaintenanceManager::Send_UpdateBinary(LPCHARACTER ch)
{
	if (NULL == ch)
		return;

	if (!ch->IsPC())
		return;

	if (ch)
	{
		SQLMsg * pMsg = DBManager::instance().DirectQuery(
			"SELECT time, duration, "
			"reason "
			"from player.maintenance"
		);

		if (pMsg->Get()->uiNumRows > 0)
		{
			MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
			ch->ChatPacket(CHAT_TYPE_COMMAND, "BINARY_Update_Maintenance %s %s %s ", row[0], row[1], row[2]);
			delete pMsg;
		}
	}
}

void MaintenanceManager::Send_CheckTable(LPCHARACTER ch)
{
	if (NULL == ch)
		return;

	if (!ch->IsPC())
		return;

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery("SELECT time, duration FROM player.maintenance LIMIT 1"));

	if (pMsg->Get()->uiNumRows == 0)
		return;

	MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);

	int sTime = 0;
	int sDuration = 0;

	str_to_number(sTime, row[0]);
	str_to_number(sDuration, row[1]);

	if (sTime > 0 && sDuration > 0)
	{
		Send_UpdateBinary(ch);
	}
}
