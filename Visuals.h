#pragma once
#include "Baimless.club/Utils\GlobalVars.h"
#include "Baimless.club/Singleton.h"
#include "Baimless.club/SDK/CGlobalVarsBase.h"
struct _event
{
	_event(const float& time, const std::string& msg, const std::string& secretmsg = "")
	{
		_time = time;
		_displayticks = time - g_pGlobalVars->curtime;
		_msg = msg;
		_displayed = false;
		_secretmsg = secretmsg;
	}

	float _time = 0;
	float _displayticks = 0;
	bool _displayed = false;
	std::string _msg = "";
	std::string _secretmsg = "";
};

extern std::vector<_event> _events;
extern void logs();
class c_visuals : public Singleton< c_visuals >
{
private:

public:
	int		 box_hpDifference;

	void DrawPlayers();
	void RenderHitboxPoints(C_BaseEntity* pEnt);


	void flags(C_BaseEntity* player);
	void flags_TEAM(C_BaseEntity* player);
	void flags_LOCAL(C_BaseEntity* player);

	void BoxESP(C_BaseEntity* Entity);
	void BoxESP_TEAM(C_BaseEntity* Entity);
	void BoxESP_LOCAL(C_BaseEntity* Entity);

	void NameESP(C_BaseEntity* Entity);
	void NameESP_TEAM(C_BaseEntity* Entity);
	void NameESP_LOCAL(C_BaseEntity* Entity);

	void HealthESP_TEAM(C_BaseEntity* Entity);
	void HealthESP(C_BaseEntity* Entity);
	void HealthESP_LOCAL(C_BaseEntity* Entity);


	void WeaponESP(C_BaseEntity* Entity);
	void WeaponESP_TEAM(C_BaseEntity* Entity);
	void WeaponESP_LOCAL(C_BaseEntity* Entity);

	void Ammo(C_BaseEntity* Entity);
	void Ammo_TEAM(C_BaseEntity* Entity);
	void Ammo_LOCAL(C_BaseEntity* Entity);


	void RenderSkeleton(C_BaseEntity* pEnt);
	void RenderSkeleton_TEAM(C_BaseEntity* pEnt);
	void RenderSkeleton_LOCAL(C_BaseEntity* pEnt);

	void penetration_reticle();
	void DrawFeetRange();
	void DrawZeusRange();
	void spectators();
	void indicatorssss();

	void DrawIndicator();

};
 