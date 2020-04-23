#include "LagComp.h"
#include "Aimbot.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\newMenu.h"
#include "../Resolver/Resolver.h"
#include "../../../cthash.h"
#include "../../Options.hpp"
void BackTrack::Update(int tick_count)
{
	if (!g_Options.legit.backtrack)
		return;

	latest_tick = tick_count;
	for (int i = 0; i < 64; i++)
	{

		UpdateRecord(i);
	}
}

bool BackTrack::IsTickValid(int tick)
{

	int delta = latest_tick - tick;
	float deltaTime = delta * g_pGlobalVars->intervalPerTick;
	return (fabs(deltaTime) <= 0.2f);
}

void BackTrack::UpdateRecord(int i)
{
	C_BaseEntity* pEntity = g_pEntityList->GetClientEntity(i);
	if (pEntity && pEntity->IsAlive() && !pEntity->IsDormant())
	{
		float lby = pEntity->GetLowerBodyYaw();
		if (lby != records[i].lby)
		{

			records[i].tick_count = latest_tick;
			records[i].lby = lby;
			records[i].headPosition = pEntity->hitbox_position(0);
		}
	}
	else
	{

		records[i].tick_count = 0;
	}
}

bool BackTrack::RunLBYBackTrack(int i, CUserCmd* cmd, Vector& aimPoint)
{
	if (IsTickValid(records[i].tick_count))
	{
		aimPoint = records[i].headPosition;
		cmd->tick_count = records[i].tick_count;
		return true;
	}
	return false;
}

void BackTrack::legitBackTrack(CUserCmd* cmd, C_BaseEntity* pLocal)
{
	if (g_Options.legit.backtrack)
	{

		int bestTargetIndex = -1;
		float bestFov = FLT_MAX;
		PlayerInfo_t info;
		if (!pLocal->IsAlive())
			return;

		for (int i = 0; i < g_pEngine->GetMaxClients(); i++)
		{
			auto entity = (C_BaseEntity*)g_pEntityList->GetClientEntity(i);

			if (!entity || !pLocal)
				continue;

			if (entity == pLocal)
				continue;

			if (!g_pEngine->GetPlayerInfo(i, &info))
				continue;

			if (entity->IsDormant())
				continue;

			if (entity->GetTeam() == pLocal->GetTeam())
				continue;

			if (entity->IsAlive())
			{

				float simtime = entity->GetSimulationTime();
				Vector hitboxPos = entity->hitbox_position(0);
				Vector origin = entity->GetOrigin();

				//headPositions[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPos };
				headPositions[i][cmd->command_number % 12] = backtrackData{ simtime, hitboxPos, origin };
				Vector ViewDir = AngleV12ector(cmd->viewangles + (pLocal->GetAimPunchAngle() * 2.f));
				float FOVDistance = DistPointToLine(hitboxPos, pLocal->get_eye_pos(), ViewDir);

				if (bestFov > FOVDistance)
				{
					bestFov = FOVDistance;
					bestTargetIndex = i;
				}
			}

		}

		float bestTargetSimTime;
		if (bestTargetIndex != -1)
		{
			float tempFloat = FLT_MAX;
			Vector ViewDir = AngleV12ector(cmd->viewangles + (pLocal->GetAimPunchAngle() * 2.f));
			for (int t = 0; t < 12; ++t)
			{
				float tempFOVDistance = DistPointToLine(headPositions[bestTargetIndex][t].hitboxPos, pLocal->get_eye_pos(), ViewDir);
				if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimulationTime() - 1)
				{
					tempFloat = tempFOVDistance;
					bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
				}
			}
			if (cmd->buttons & IN_ATTACK)
			{

				cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
			}
		}

	}

}

BackTrack* backtracking = new BackTrack();
backtrackData headPositions[64][12];


LagComp g_LagComp;


float LagComp::get_lerp_time()
{
	__(cl_updaterate_s, "cl_updaterate");
	__(cl_interp_s, "cl_interp");

	static auto cl_updaterate = g_pCvar->FindVar(cl_updaterate_s);
	static auto cl_interp = g_pCvar->FindVar(cl_interp_s);

	const auto update_rate = cl_updaterate->GetInt();
	const auto interp_ratio = cl_interp->GetFloat();

	auto lerp = interp_ratio / update_rate;

	if (lerp <= cl_interp->GetFloat())
		lerp = cl_interp->GetFloat();

	return lerp;
}

bool LagComp::valid_simtime(const float& simtime)
{
	const auto nci = g_pEngine->GetNetChannelInfo();
	if (!nci)
		return false;

	float correct = 0;

	correct += nci->GetLatency(FLOW_OUTGOING);
	correct += nci->GetLatency(FLOW_INCOMING);
	correct += get_lerp_time();

	const auto delta_time = correct - (g_pGlobalVars->curtime - simtime);

	return fabsf(delta_time) <= 0.2f && correct < 1.f;
}



void LagComp::StoreRecord(C_BaseEntity* pEnt) // best lag comp in the world
{
	PlayerRecords Setup;

	static float ShotTime[65];
	static float OldSimtime[65];

	if (pEnt != g::pLocalEntity)
		pEnt->FixSetupBones(g_Aimbot.Matrix[pEnt->EntIndex()]);

	if (PlayerRecord[pEnt->EntIndex()].size() == 0)
	{
		Setup.Velocity = abs(pEnt->GetVelocity().Length2D());

		Setup.SimTime = pEnt->GetSimulationTime();
		
		Setup.m_flags = pEnt->GetFlags();
		
		Setup.m_abs_origin = pEnt->get_abs_origin();

		Setup.m_vecOrigin = pEnt->GetOrigin();
		
		Setup.m_origin = pEnt->GetOrigin();

		Setup.m_abs_ang = pEnt->get_abs_angles();

		memcpy(Setup.Matrix, g_Aimbot.Matrix[pEnt->EntIndex()], (sizeof(matrix3x4_t) * 128));

		Setup.Shot = false;

		PlayerRecord[pEnt->EntIndex()].push_back(Setup);
	}

	if (OldSimtime[pEnt->EntIndex()] != pEnt->GetSimulationTime())
	{
		Setup.Velocity = abs(pEnt->GetVelocity().Length2D());

		Setup.m_vecOrigin = pEnt->GetOrigin();

		Setup.SimTime = pEnt->GetSimulationTime();

		Setup.m_flags = pEnt->GetFlags();
		
		Setup.m_abs_origin = pEnt->get_abs_origin();
		
		Setup.m_origin = pEnt->GetOrigin();
		
		Setup.m_abs_ang = pEnt->get_abs_angles();

		if (pEnt == g::pLocalEntity)
			pEnt->FixSetupBones(g_Aimbot.Matrix[pEnt->EntIndex()]);

		memcpy(Setup.Matrix, g_Aimbot.Matrix[pEnt->EntIndex()], (sizeof(matrix3x4_t) * 128));

		if (pEnt->GetActiveWeapon() && !pEnt->IsKnifeorNade())
		{
			if (ShotTime[pEnt->EntIndex()] != pEnt->GetActiveWeapon()->GetLastShotTime())
			{
				Setup.Shot = true;
				ShotTime[pEnt->EntIndex()] = pEnt->GetActiveWeapon()->GetLastShotTime();
			}
			else
				Setup.Shot = false;
		}
		else
		{
			Setup.Shot = false;
			ShotTime[pEnt->EntIndex()] = 0.f;
		}

		PlayerRecord[pEnt->EntIndex()].push_back(Setup);

		OldSimtime[pEnt->EntIndex()] = pEnt->GetSimulationTime();
	}

	ShotTick[pEnt->EntIndex()] = -1;

	if (PlayerRecord[pEnt->EntIndex()].size() > 0)
		for (int tick = 0; tick < PlayerRecord[pEnt->EntIndex()].size(); tick++)
			if (!valid_simtime((PlayerRecord[pEnt->EntIndex()].at(tick).SimTime)))
				PlayerRecord[pEnt->EntIndex()].erase(PlayerRecord[pEnt->EntIndex()].begin() + tick);
			else if (PlayerRecord[pEnt->EntIndex()].at(tick).Shot)
				ShotTick[pEnt->EntIndex()] = tick; // gets the newest shot tick
}

void LagComp::apply(C_BaseEntity* player, bool backup) const
{
	PlayerRecords Setup;

	player->GetFlags() = Setup.m_flags;
	player->GetSimulationTime() = Setup.SimTime;
	player->setAbsAngle(Setup.m_abs_ang);
	player->set_abs_origin(backup ? Setup.m_abs_origin : Setup.m_origin);
	player->GetOrigin() = Setup.m_origin;
}
void LagComp::ClearRecords(int i)
{
	if (PlayerRecord[i].size() > 0)
	{
		for (int tick = 0; tick < PlayerRecord[i].size(); tick++)
		{
			PlayerRecord[i].erase(PlayerRecord[i].begin() + tick);
		}
	}
}

template<class T, class U>
T LagComp::clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}
