#pragma once
class MaintenanceManager : public singleton<MaintenanceManager>
{
	public:
		MaintenanceManager();
		~MaintenanceManager();

	void	Send_UpdateBinary(LPCHARACTER ch);
	void	Send_CheckTable(LPCHARACTER ch);

	void	Send_Text(LPCHARACTER ch, const char * reason);

	void	Send_DisableSecurity(LPCHARACTER ch);
	void	Send_ActiveMaintenance(LPCHARACTER ch, long int time_maintenance, long int duration_maintenance);

	void	ShutdownMaintenance(int iSec);
};
