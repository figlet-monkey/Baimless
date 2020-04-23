#pragma once

#include "..\Aimbot\Autowall.h"
#include "..\Aimbot\Aimbot.h"
#include "..\Aimbot\LagComp.h"
#include "../../SDK/InputSystem.h"
#include "..\..\Utils\GlobalVars.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\SDK\CPrediction.h"
#include "..\..\newMenu.h"
#include <iostream>
#include <algorithm>
#include "../../Options.hpp"
#include <d3dx9math.h>
#include "../../SDK/IGameEvent.h"
#include "../../SDK/IMaterial.h"
#include "../../SDK/IVModelInfo.h"
// Variables
static float m_flTrueDirection,
m_flRoughDirection,
m_flWishDirection,
m_flCalculatedDirection,
m_flStep;

static bool m_bInTransition,
m_bIsBhopping;






template<class T, class U>
inline T cla23324mp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}
auto flags_backup = 0;

class sound_info
{
public:
	sound_info(Vector positions, float times, int userids)
	{
		this->position = positions;
		this->time = times;
		this->userid = userids;
	}

	Vector position;
	float time;
	int userid;
};

std::vector<sound_info> sound_logs;
class soundesp
{
public:
	void draw();
	void draw_circle(Vector position);
};

soundesp g_soundesp;
float true_direction;
int wish_direction;
float step;
bool unduck;
bool in_transition;
int calculated_direction;
#define _SOLVEY(a, b, c, d, e, f) ((c * b - d * a) / (c * f - d * e))
#define SOLVEY(...) _SOLVEY(?, ?, ?, ?, ?, ?)
#define SOLVEX(y, world, forward, right) ((world.x - right.x * y) / forward.x)
float rough_direction;
class Misc
{
public:
	Vector m_qAnglesView = {};
	void OnCreateMove(CUserCmd* pCmd)
	{
	
		this->pLocal = g::pLocalEntity;
		//m_qAnglesView = pCmd->viewangles;
		switch(g_Options.misc.air_strafe_option)
		{
			case 1:
				this->AutoStrafenormal(pCmd);
				break;
			case 2:
				this->AutoStrafe(pCmd);
				break;
		}
		//this->Autostrafe(pCmd);
		this->DoBhop(pCmd);
		this->Walkbot(pCmd);
		this->DoJumpBug(pCmd);
	};
	
	void Autostrafe(CUserCmd* cmd)
	{
		static const auto cl_sidespeed = g_pCvar->FindVar(("cl_sidespeed"));
		float yaw_add = 0.f;
		float yaw_add_s = 0.f;

		if (!g::pLocalEntity|| !g::pLocalEntity->IsAlive())
			return;
		
		if (g::pLocalEntity->is_grounded() || !g_Options.misc.autostrafe)
			return;

		if (g_Options.misc.wasdstrafe) {
			if (cmd->buttons & IN_BACK) {
				yaw_add = -180.f;
				if (cmd->buttons & IN_MOVERIGHT)
					yaw_add = -125;
				else if (cmd->buttons & IN_MOVELEFT)
					yaw_add = 125;
			}
			else if (cmd->buttons & IN_FORWARD) {
				yaw_add = 0.f;
				if (cmd->buttons & IN_MOVERIGHT)
					yaw_add = -45;
				else if (cmd->buttons & IN_MOVELEFT)
					yaw_add = 45;
			}
			else if (cmd->buttons & IN_MOVERIGHT)
				yaw_add = -90.f;
			else if (cmd->buttons & IN_MOVELEFT)
				yaw_add = 90.f;

			m_qAnglesView.y += yaw_add;
		}
		cmd->forwardmove = 0.f;
		cmd->sidemove = 0.f;
		
		const auto delta = g_Math.NormalizeYaw(m_qAnglesView.y -
			RAD2DEG(atan2(g::pLocalEntity->GetVelocity().y, g::pLocalEntity->GetVelocity().x)));

		cmd->sidemove = delta > 0.f ? -cl_sidespeed->GetFloat() : cl_sidespeed->GetFloat();

		m_qAnglesView.y = g_Math.NormalizeYaw(m_qAnglesView.y - delta);
	}
	void PlayerChanger(ClientFrameStage_t stage)
	{
		static  constexpr const char* models[]{
			"models/player/custom_player/legacy/ctm_fbi_variantb.mdl",
			"models/player/custom_player/legacy/ctm_fbi_variantf.mdl",
		"models/player/custom_player/legacy/ctm_fbi_variantg.mdl",
		"models/player/custom_player/legacy/ctm_fbi_varianth.mdl",
		"models/player/custom_player/legacy/ctm_sas_variantf.mdl",
		"models/player/custom_player/legacy/ctm_st6_variante.mdl",
			"models/player/custom_player/legacy/ctm_st6_variantg.mdl",
			"models/player/custom_player/legacy/ctm_st6_varianti.mdl",
			"models/player/custom_player/legacy/ctm_st6_variantk.mdl",
			"models/player/custom_player/legacy/ctm_st6_variantm.mdl",
			"models/player/custom_player/legacy/tm_balkan_variantf.mdl",
			"models/player/custom_player/legacy/tm_balkan_variantg.mdl",
			"models/player/custom_player/legacy/tm_balkan_varianth.mdl",
		"models/player/custom_player/legacy/tm_balkan_varianti.mdl",
			"models/player/custom_player/legacy/tm_balkan_variantj.mdl",
			"models/player/custom_player/legacy/tm_leet_variantf.mdl",
		"models/player/custom_player/legacy/tm_leet_variantg.mdl",
		"models/player/custom_player/legacy/tm_leet_varianth.mdl",
		"models/player/custom_player/legacy/tm_leet_varianti.mdl",
		"models/player/custom_player/legacy/tm_phoenix_variantf.mdl",
		"models/player/custom_player/legacy/tm_phoenix_variantg.mdl",
			"models/player/custom_player/legacy/tm_phoenix_varianth.mdl"
		};

		if (g_Options.changers.playerModel > 0 && stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
			g::pLocalEntity->SetModelIndex(g_pModelInfo->GetModelIndex(models[g_Options.changers.playerModel - 1]));
		}
	}
	void MovementFix(Vector& oldang) // i think osmium
	{
		Vector vMovements(g::pCmd->forwardmove, g::pCmd->sidemove, 0.f);

		if (vMovements.Length2D() == 0)
			return;

		Vector vRealF, vRealR;
		Vector aRealDir = g::pCmd->viewangles;
		aRealDir.Clamp();

		g_Math.AngleVectors(aRealDir, &vRealF, &vRealR, nullptr);
		vRealF[2] = 0;
		vRealR[2] = 0;

		VectorNormalize(vRealF);
		VectorNormalize(vRealR);

		Vector aWishDir = oldang;
		aWishDir.Clamp();

		Vector vWishF, vWishR;
		g_Math.AngleVectors(aWishDir, &vWishF, &vWishR, nullptr);

		vWishF[2] = 0;
		vWishR[2] = 0;

		VectorNormalize(vWishF);
		VectorNormalize(vWishR);

		Vector vWishVel;
		vWishVel[0] = vWishF[0] * g::pCmd->forwardmove + vWishR[0] * g::pCmd->sidemove;
		vWishVel[1] = vWishF[1] * g::pCmd->forwardmove + vWishR[1] * g::pCmd->sidemove;
		vWishVel[2] = 0;

		float a = vRealF[0], b = vRealR[0], c = vRealF[1], d = vRealR[1];
		float v = vWishVel[0], w = vWishVel[1];

		float flDivide = (a * d - b * c);
		float x = (d * v - b * w) / flDivide;
		float y = (a * w - c * v) / flDivide;

		g::pCmd->forwardmove = x;
		g::pCmd->sidemove = y;
	}
	//fix by Veteran0, thanks for that ^-^
	void MsgFunc_ServerRankRevealAll()
	{
		using tServerRankRevealAllFn = bool(__cdecl*)(int*);
		static tServerRankRevealAllFn ServerRankRevealAll = 0;
		if (!ServerRankRevealAll)
		{
			ServerRankRevealAll = (tServerRankRevealAllFn)(Utils::FindSignature("client_panorama.dll", "55 8B EC 8B 0D ? ? ? ? 85 C9 75 ? A1 ? ? ? ? 68 ? ? ? ? 8B 08 8B 01 FF 50 ? 85 C0 74 ? 8B C8 E8 ? ? ? ? 8B C8 EB ? 33 C9 89 0D ? ? ? ? 8B 45 ? FF 70 ? E8 ? ? ? ? B0 ? 5D") + 0);

		}
		if (ServerRankRevealAll)
		{
			int fArray[3] = { 0,0,0 };
			ServerRankRevealAll(fArray);
		}
	}

	void Walkbot(CUserCmd* pCmd) // wip
	{
		if (!g_Options.misc.walk_bot || !g::pLocalEntity->IsAlive())
			return;



		float BestDistance = 0.f;
		float BestAngle = -9999999.f;

		g::pCmd->forwardmove = 450.f;

		C_TraceFilter filter1(g::pLocalEntity);
		Vector Origin = g::pLocalEntity->GetOrigin() + Vector(0, 0, 10);

		Vector EyeAngle;
		g_pEngine->GetViewAngles(EyeAngle);


		C_Trace TraceFront;
		g_pTrace->TraceRay(C_Ray(Origin, g_Math.ProjectPoint(Origin, g_Math.NormalizeYaw((EyeAngle.y + 90)), 40)), mask_solid, &filter1, &TraceFront);

		static bool jumpedb4 = false;

		if (!(g::pLocalEntity->GetFlags() & FL_ONGROUND))
			pCmd->buttons |= IN_DUCK;
		else
			jumpedb4 = false;

		if (TraceFront.flFraction != 1.0f && (g::pLocalEntity->GetFlags() & FL_ONGROUND) && !jumpedb4)
		{
			pCmd->buttons |= IN_JUMP;
			jumpedb4 = true;
		}

		static bool RandomDirect = true;
		RandomDirect = false;
		srand(time(NULL));
		int RandomStart = rand() % 36;
		int i = RandomStart;
		bool DoubleStart = false;
		bool Loop = true;
		while (Loop)
		{
			i += RandomDirect ? 1 : -1;

			if (RandomDirect && i > 36)
			{
				i = 0;
				DoubleStart = true;
			}
			else if (!RandomDirect && i < 0)
			{
				i = 36;
				DoubleStart = true;
			}

			if (RandomDirect && i >= RandomStart && DoubleStart)
			{
				Loop = false;
				break;
			}
			else if (!RandomDirect && i <= RandomStart && DoubleStart)
			{
				Loop = false;
				break;
			}

			C_Trace TraceInit;
			g_pTrace->TraceRay(C_Ray(Origin, g_Math.ProjectPoint(Origin, g_Math.NormalizeYaw(i * 10), 300) + Vector(0, 0, 90)), mask_solid, &filter1, &TraceInit);

			if (TraceInit.flFraction > BestDistance)
			{
				BestAngle = g_Math.NormalizeYaw(i * 10);
				BestDistance = TraceInit.flFraction;
			}
		}

		BestAngle = g_Math.NormalizeYaw(BestAngle - 90);

		float Delta = g_Math.NormalizeYaw(BestAngle - EyeAngle.y);

		if (Delta >= 0)
		{
			EyeAngle.y = g_Math.NormalizeYaw(EyeAngle.y + ((abs(Delta) <= 5) ? abs(Delta) : 5));
		}
		else
		{
			EyeAngle.y = g_Math.NormalizeYaw(EyeAngle.y - ((abs(Delta) <= 5) ? abs(Delta) : 5));
		}

		C_Trace TraceLeft;
		g_pTrace->TraceRay(C_Ray(Origin, g_Math.ProjectPoint(Origin, g_Math.NormalizeYaw((EyeAngle.y - 90) - 45), 300)), mask_solid, &filter1, &TraceLeft);
		C_Trace TraceRight;
		g_pTrace->TraceRay(C_Ray(Origin, g_Math.ProjectPoint(Origin, g_Math.NormalizeYaw((EyeAngle.y - 90) + 45), 300)), mask_solid, &filter1, &TraceRight);

		if (TraceLeft.flFraction > TraceRight.flFraction)
		{
			g::pCmd->sidemove = -100.f;
		}
		else
		{
			g::pCmd->sidemove = 100.f;
		}

		EyeAngle.x = 0.f;

		g_pEngine->SetViewAngles(EyeAngle);
	}


	void SoundEsp(ClientFrameStage_t curStage)
	{
		if (!g::pLocalEntity)
			return;
		if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected() || !g::pLocalEntity)
			return;
		if (curStage == FRAME_NET_UPDATE_START)
			if (g_Options.esp.other.sound_esp)
				g_soundesp.draw();
	}

	void OnCreateMoveRankReveal(CUserCmd* cmd)
	{


		if (g_Options.misc.comp_ranks && cmd->buttons & IN_SCORE)
			MsgFunc_ServerRankRevealAll();



	}


	void ThirdPerson()
	{
		if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected() || !g::pLocalEntity)
			return;
		if (!g_Options.misc.thirdperson)
			return;
		static bool init = false;
		static bool set_angle = false;
		auto pLocalEntity = g::pLocalEntity;

		static int stored_thirdperson_distance;

		if (stored_thirdperson_distance != g_Options.misc.thirdpeson_distance) {
			std::string command; command += "cam_idealdist "; command += std::to_string(g_Options.misc.thirdpeson_distance + 30);
			g_pEngine->ExecuteClientCmd(command.c_str());

			stored_thirdperson_distance = g_Options.misc.thirdpeson_distance;
		}
		static Vector vecAngles;
		g_pEngine->GetViewAngles(vecAngles);

		if (GetKeyState(g_Options.misc.thirdperson_key) && g::pLocalEntity->IsAlive())
		{
			if (init)
			{
				ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
				*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0; // ew
				sv_cheats->SetValue(1);
				g_pEngine->ExecuteClientCmd("thirdperson");

				std::string command; command += "cam_idealdist "; command += std::to_string(g_Options.misc.thirdpeson_distance + 30);
				g_pEngine->ExecuteClientCmd(command.c_str());
			}
			init = false;
		}
		else
		{
			if (!init)
			{
				ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
				*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0; // ew
				sv_cheats->SetValue(1);
				g_pEngine->ExecuteClientCmd("firstperson");
			}
			init = true;
		}


	}

	void Thirdperson_FSN(ClientFrameStage_t curStage) {
		if (g_Input->m_fCameraInThirdPerson && g::pLocalEntity && g::pLocalEntity->IsAlive())
		{
			g_pPrediction->SetLocalViewAngles(Vector(g::RealAngle.x, g::RealAngle.y, 0));
		}
	}

	void remove_smoke() {
		if (!g_Options.esp.effects.remove_smoke)
			return;
		static auto smoke_count = *reinterpret_cast<uint32_t **>(Utils::FindSignature("client_panorama.dll", "A3 ? ? ? ? 57 8B CB") + 1);

		static std::vector<const char*> smoke_materials = {
			"effects/overlaysmoke",
			"particle/beam_smoke_01",
			"particle/particle_smokegrenade",
			"particle/particle_smokegrenade1",
			"particle/particle_smokegrenade2",
			"particle/particle_smokegrenade3",
			"particle/particle_smokegrenade_sc",
			"particle/smoke1/smoke1",
			"particle/smoke1/smoke1_ash",
			"particle/smoke1/smoke1_nearcull",
			"particle/smoke1/smoke1_nearcull2",
			"particle/smoke1/smoke1_snow",
			"particle/smokesprites_0001",
			"particle/smokestack",
			"particle/vistasmokev1/vistasmokev1",
			"particle/vistasmokev1/vistasmokev1_emods",
			"particle/vistasmokev1/vistasmokev1_emods_impactdust",
			"particle/vistasmokev1/vistasmokev1_fire",
			"particle/vistasmokev1/vistasmokev1_nearcull",
			"particle/vistasmokev1/vistasmokev1_nearcull_fog",
			"particle/vistasmokev1/vistasmokev1_nearcull_nodepth",
			"particle/vistasmokev1/vistasmokev1_smokegrenade",
			"particle/vistasmokev1/vistasmokev4_emods_nocull",
			"particle/vistasmokev1/vistasmokev4_nearcull",
			"particle/vistasmokev1/vistasmokev4_nocull"
		};



		for (auto material_name : smoke_materials) {
			IMaterial * material = g_pMaterialSys->FindMaterial(material_name, "Other textures");
			if (material)
				material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, g_Options.esp.effects.remove_smoke);
		}

		if (g_Options.esp.effects.remove_smoke)
			*(int*)smoke_count = 0;
	}


#define FCVAR43_CHEATGAY                     (1<<14) // Only useable in singleplayer / debug / multiplayer & sv_cheats

	void gravity_ragdoll()
	{
		if (!g_pEngine->IsInGame() && g_pEngine->IsConnected())
			return;
		static auto ragdollGravity = g_pCvar->FindVar("cl_ragdoll_gravity");
		ragdollGravity->SetValue(g_Options.misc.ragdoll ? -600 : 600);
	}
	void remose_flash()
	{
		/*if (!g::pLocalEntity)
			return;

		if (!g::pLocalEntity->m_flFlashDuration())
			return;

		if (g::pLocalEntity && g_Options.esp.effects.remove_flash && g::pLocalEntity->m_flFlashDuration() > 0.0f)
			g::pLocalEntity->m_flFlashDuration() = 0.0f;*/
	}
	void Crosshair()
	{
		if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
		{


			if (!g::pLocalEntity)
				return;

			if (!g::pLocalEntity->IsAlive())
				return;

			if (g::pLocalEntity->IsScoped() && g_Options.esp.effects.removescopeoverlay)
			{
				if (g_Options.esp.effects.scope_type == 0)
				{


					auto weapon = g::pLocalEntity->GetActiveWeapon();

					if (!weapon)
						return;



					if (g::pLocalEntity->IsScoped())
					{

						//	g_pSurface->Line(0, center_y, screen_x, center_y, Color(0, 0, 0, 255));



						int width, height;
						g_pEngine->GetScreenSize(width, height);
						g_pSurface->Line(width / 2, 0, width / 2, height, Color(0, 0, 0, 255));
						g_pSurface->Line(0, height / 2, width, height / 2, Color(0, 0, 0, 255));
					}



				}
				if (g_Options.esp.effects.scope_type == 1)
				{

					int screen_x;
					int screen_y;
					int center_x;
					int center_y;

					g_pEngine->GetScreenSize(screen_x, screen_y);
					g_pEngine->GetScreenSize(center_x, center_y);

					center_x /= 2; center_y /= 2;

					auto weapon = g::pLocalEntity->GetActiveWeapon();

					if (!weapon)
						return;


					int size = weapon->GetInaccuracy() * 80.0f;

					if (g::pLocalEntity->IsScoped())
					{
						if (size < 5)
						{
							g_pSurface->Line(0, center_y, screen_x, center_y, Color(0, 0, 0, 255));
							g_pSurface->Line(center_x, 0, center_x, screen_y, Color(0, 0, 0, 255));
						}

						g_pSurface->FilledRect(center_x - 10000, center_y - size / 2, 10000, size, Color(0, 0, 0, 190));
						g_pSurface->FilledRect(center_x, center_y - size / 2, 10000, size, Color(0, 0, 0, 190));

						g_pSurface->FilledRect(center_x - size / 2, center_y - 10000, size, 10000, Color(0, 0, 0, 190));
						g_pSurface->FilledRect(center_x - size / 2, center_y, size, 10000, Color(0, 0, 0, 190));
					}
				}


			}

		}


	}

	void RecoilCrosshair()
	{
		static auto recoilCrosshair =g_pCvar->FindVar("cl_crosshair_recoil");
		recoilCrosshair->SetValue(g_Options.misc.recoilCrosshair ? 1 : 0);
	}

	bool changeName(bool reconnect, const char* newName, float delay) noexcept
	{
		static auto exploitInitialized{ false };

		static auto name{ g_pCvar->FindVar("name") };

		if (reconnect) {
			exploitInitialized = false;
			return false;
		}

		if (!exploitInitialized && g_pEngine->IsInGame()) {
			if (PlayerInfo_t playerInfo; g_pEngine->GetPlayerInfo(g_pEngine->GetLocalPlayer(), &playerInfo) && (!strcmp(playerInfo.szName, "?empty") || !strcmp(playerInfo.szName, "\n\xAD\xAD\xAD"))) {
				exploitInitialized = true;
			}
			else {
				name->fnChangeCallback.SetSize(0);
				name->SetValue("\n\xAD\xAD\xAD");
				return false;
			}
		}

		static auto nextChangeTime{ 0.0f };
		if (nextChangeTime <= g_pGlobalVars->realtime) {
			name->SetValue(newName);
			nextChangeTime = g_pGlobalVars->realtime + delay;
			return true;
		}
		return false;
	}
	void NameStealer()
	{
		if (g_Options.misc.name_stealer) {
			const auto localPlayer = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

			bool allNamesStolen = true;
			static std::vector<int> stolenIds;
			for (int i = 1; i <= g_pEngine->GetMaxClients(); i++) {
				if (auto entity = g_pEntityList->GetClientEntity(i); entity && entity != localPlayer) {
					if (PlayerInfo_t playerInfo; g_pEngine->GetPlayerInfo(entity->EntIndex(), &playerInfo) && !playerInfo.fakeplayer && std::find(std::begin(stolenIds), std::end(stolenIds), playerInfo.userId) == std::end(stolenIds)) {
						allNamesStolen = false;
						if (changeName(false, std::string{ playerInfo.szName }.append("\x1").c_str(), 1.0f))
							stolenIds.push_back(playerInfo.userId);
						break;
					}
				}
			}
			if (allNamesStolen)
				stolenIds.clear();
		}

	}

    void QuickReload(CUserCmd* cmd)
    {
        auto weapons = g::pLocalEntity->m_hMyWeapons();
        if (g_Options.misc.quickReload) {
            const auto localPlayer = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
            static C_BaseEntity* reloadedWeapon{ nullptr };

            if (reloadedWeapon) {
                for (auto i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++) {
                    if (weapons[i] == -1)
                        break;

                    if (g_pEntityList->GetClientEntityFromHandle(weapons[i]) == reloadedWeapon) {
                       // cmd->weaponselect = reloadedWeapon->GetActiveWeaponIndex();
                        cmd->weaponsubtype = reloadedWeapon->getWeaponSubType();
                        break;
                    }
                }
                reloadedWeapon = nullptr;
            }

            if (auto activeWeapon{ localPlayer->GetActiveWeapon() }; activeWeapon && activeWeapon->isInReload() && activeWeapon->clip() == activeWeapon->GetCSWpnData()->max_clip) {
                reloadedWeapon = activeWeapon;

                for (auto i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++) {
                    if (weapons[i] == -1)
                        break;

                    if (auto weapon{ g_pEntityList->GetClientEntityFromHandle(weapons[i]) }; weapon && weapon != reloadedWeapon) {
                        //cmd->weaponselect = weapon->index();
                        cmd->weaponsubtype = weapon->getWeaponSubType();
                        break;
                    }
                }
            }
        }
    }

    void KillMessage(IGameEvent* event)
    {
        if (!g_Options.misc.killMessage)
            return;

        const auto localPlayer = g::pLocalEntity;

        if (!localPlayer || !localPlayer->IsAlive())
            return;

        if (g_pEngine->GetPlayerForUserID(event->GetInt("attacker")) != localPlayer->EntIndex() || g_pEngine->GetPlayerForUserID(event->GetInt("userid")) == localPlayer->EntIndex())
            return;

        std::string cmd = "say \"";
        cmd += g_Options.misc.killMessageString;
        cmd += "\"";
        g_pEngine->ClientCmd_UnrestrictedGAY(cmd.c_str());
    }

	
	void edge_jump_pre_prediction(CUserCmd* user_cmd) noexcept
	{
		auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

		if (!g_Options.misc.edge_jump)
			return;

		if (!GetAsyncKeyState(g_Options.misc.edge_jump_key))
			return;

		if (!local_player)
			return;

		if (local_player->GetMoveType() == movetype_ladder || local_player->GetMoveType() == movetype_noclip)
			return;

		flags_backup = local_player->GetFlags();
	}


	void DoJumpBug(CUserCmd* pCmd)
	{
		if (!g_Options.misc.bJumpBug)
			return;
		C_BaseEntity* local = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		float max_radias = D3DX_PI * 2;
		float step = max_radias / 128;
		float xThick = 23;

		if (g_Options.misc.bJumpBug && GetAsyncKeyState(g_Options.misc.bJumpKey) && (local->GetFlags() & (1 << 0))) {
			g_Options.misc.bhop = false;
			int screenWidth, screenHeight;
			g_pEngine->GetScreenSize(screenWidth, screenHeight);
			if (unduck) {
				g_Options.misc.bDidJump = false;
				pCmd->buttons &= ~IN_DUCK; // duck
				pCmd->buttons |= IN_JUMP; // jump
				unduck = false;
			}
			Vector pos = local->GetOrigin();
			for (float a = 0.f; a < max_radias; a += step) {
				Vector pt;
				pt.x = (xThick * cos(a)) + pos.x;
				pt.y = (xThick * sin(a)) + pos.y;
				pt.z = pos.z;


				Vector pt2 = pt;
				pt2.z -= 6;

				C_Trace fag;

				

				C_TraceFilter flt(local);
				flt.pSkip1 = local;
				g_pTrace->TraceRay(C_Ray(pt,pt2), mask_solid_brushonly, &flt, &fag);

				if (fag.flFraction != 1.f && fag.flFraction != 0.f) {
					g_Options.misc.bDidJump = true;
					pCmd->buttons |= IN_DUCK; // duck
					pCmd->buttons &= ~IN_JUMP; // jump
					unduck = true;
				}
			}
			for (float a = 0.f; a < max_radias; a += step) {
				Vector pt;
				pt.x = ((xThick - 2.f) * cos(a)) + pos.x;
				pt.y = ((xThick - 2.f) * sin(a)) + pos.y;
				pt.z = pos.z;

				Vector pt2 = pt;
				pt2.z -= 6;

				C_Trace fag;


			

				C_TraceFilter flt(local);
				flt.pSkip1 = local;
				g_pTrace->TraceRay(C_Ray(pt, pt2), mask_solid_brushonly, &flt, &fag);

				if (fag.flFraction != 1.f && fag.flFraction != 0.f) {
					g_Options.misc.bDidJump = true;
					pCmd->buttons |= IN_DUCK; // duck
					pCmd->buttons &= ~IN_JUMP; // jump
					unduck = true;
				}
			}
			for (float a = 0.f; a < max_radias; a += step) {
				Vector pt;
				pt.x = ((xThick - 20.f) * cos(a)) + pos.x;
				pt.y = ((xThick - 20.f) * sin(a)) + pos.y;
				pt.z = pos.z;

				Vector pt2 = pt;
				pt2.z -= 6;

				C_Trace fag;
				

				C_TraceFilter flt(local);
				flt.pSkip1 = local;
				g_pTrace->TraceRay(C_Ray(pt, pt2), mask_solid_brushonly, &flt, &fag);

				if (fag.flFraction != 1.f && fag.flFraction != 0.f) {
					g_Options.misc.bDidJump = true;
					pCmd->buttons |= IN_DUCK; // duck
					pCmd->buttons &= ~IN_JUMP; // jump
					unduck = true;
				}
			}
		}



		else { g_Options.misc.bhop = true; }

	}

	void edge_jump_post_prediction(CUserCmd* user_cmd) noexcept
	{
		auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

		if (!g_Options.misc.edge_jump)
			return;

		if (!GetAsyncKeyState(g_Options.misc.edge_jump_key))
			return;

		if (!local_player)
			return;

		if (local_player->GetMoveType() == movetype_ladder || local_player->GetMoveType() == movetype_noclip)
			return;

		if (flags_backup & FL_ONGROUND && !(local_player->GetFlags() & FL_ONGROUND))
			user_cmd->buttons |= IN_JUMP;

		if (!(local_player->GetFlags() & FL_ONGROUND) && g_Options.misc.duck_in_air)
			user_cmd->buttons |= IN_DUCK;
	}




	C_BaseEntity* pLocal;

	void DoBhop(CUserCmd* cmd)
	{
		if (!g_Options.misc.bhop)
			return;

		if (!g::pLocalEntity->IsAlive())
			return;

		if (g::pLocalEntity->GetMoveType() == movetype_noclip || g::pLocalEntity->GetMoveType() == movetype_ladder)
			return;
		
		if (cmd->buttons & IN_JUMP && !(g::pLocalEntity->GetFlags() & FL_ONGROUND)) 
			cmd->buttons &= ~IN_JUMP;
		
		
		if (!(g::pLocalEntity->GetFlags() & FL_ONGROUND) && g_Options.misc.duck_in_air)
		{
			cmd->buttons &= ~IN_DUCK;
			cmd->buttons |= IN_DUCK;
		}
		/*if (g_Options.misc.duck_in_air && (g::pLocalEntity->GetFlags() & FL_ONGROUND))
		{
			pCmd->buttons |= IN_DUCK;
		}*/

		
	}

	static vec_t normalize_yaw(vec_t ang)
	{
		while (ang < -180.0f)
			ang += 360.0f;
		while (ang > 180.0f)
			ang -= 360.0f;
		return ang;
	}
	static float m_flNormalizeYaw(float flAngle)
	{
		if (flAngle > 180.f || flAngle < -180.f)
		{
			auto revolutions = round(abs(flAngle / 360.f));

			if (flAngle < 0.f)
			{
				flAngle += 360.f * revolutions;
			}
			else
			{
				flAngle -= 360.f * revolutions;
			}
		}

		return flAngle;
	}
	static float m_flRadToDeg(float flRadian)
	{
		return flRadian * (180.f / PI);
	}

	static float m_flDegToRad(float flDegree)
	{
		return flDegree * (PI / 180.f);
	}

	void AutoStrafe(CUserCmd* pCmd) const
	{
		if (!g_Options.misc.air_strafe)
			return;

		auto pLocalEnt = g::pLocalEntity;

		if (!pLocalEnt || !pLocalEnt->IsAlive())
			return;
		

		enum Directions
		{
			FORWARDS = 0,
			BACKWARDS = 180,
			LEFT = 90,
			RIGHT = -90,
			QUADRANT1 = 45,
			QUADRANT2 = -45,
			QUADRANT3 = -135,
			QUADRANT4 = 135
		};

		m_bIsBhopping = pCmd->buttons & IN_JUMP;
		if (!m_bIsBhopping && pLocalEnt->GetFlags() & FL_ONGROUND)
		{
			m_flCalculatedDirection = Directions::FORWARDS;
			m_bInTransition = false;
			return;
		}

		Vector vBase{ };
		g_pEngine->GetViewAngles(vBase);

		auto flGetRoughDirection = [&](float flTrueDirection) -> float
		{
			std::array< float, 8 > uMinimum = { Directions::FORWARDS, Directions::BACKWARDS, Directions::LEFT, Directions::RIGHT, Directions::QUADRANT1, Directions::QUADRANT2, Directions::QUADRANT3, Directions::QUADRANT4 };
			float flBestAngle = 181.f;
			float flBestDelta = 181.f;

			for (size_t i = 0; i < uMinimum.size(); ++i)
			{
				float flRoughDirection = vBase.y + uMinimum.at(i);
				float flDelta = fabsf(m_flNormalizeYaw(flTrueDirection - flRoughDirection));

				if (flDelta < flBestDelta)
				{
					flBestAngle = flRoughDirection;
					flBestDelta = flDelta;
				}
			}

			return flBestAngle;
		};

		m_flTrueDirection = pLocalEnt->GetVelocity().Angle().y;

		if ((pCmd->buttons & IN_FORWARD) && !(pCmd->buttons & IN_MOVELEFT) && !(pCmd->buttons & IN_MOVERIGHT))
		{
			m_flWishDirection = vBase.y + Directions::FORWARDS;
		}
		else if ((pCmd->buttons & IN_BACK) && !(pCmd->buttons & IN_MOVELEFT) && !(pCmd->buttons & IN_MOVERIGHT))
		{
			m_flWishDirection = vBase.y + Directions::BACKWARDS;
		}
		else if ((pCmd->buttons & IN_MOVELEFT) && !(pCmd->buttons & IN_FORWARD) && !(pCmd->buttons & IN_BACK))
		{
			m_flWishDirection = vBase.y + Directions::LEFT;
		}
		else if ((pCmd->buttons & IN_MOVERIGHT) && !(pCmd->buttons & IN_FORWARD) && !(pCmd->buttons & IN_BACK))
		{
			m_flWishDirection = vBase.y + Directions::RIGHT;
		}
		else if ((pCmd->buttons & IN_FORWARD) && (pCmd->buttons & IN_MOVELEFT))
		{
			m_flWishDirection = vBase.y + Directions::QUADRANT1;
		}
		else if ((pCmd->buttons & IN_FORWARD) && (pCmd->buttons & IN_MOVERIGHT))
		{
			m_flWishDirection = vBase.y + Directions::QUADRANT2;
		}
		else if ((pCmd->buttons & IN_BACK) && (pCmd->buttons & IN_MOVERIGHT))
		{
			m_flWishDirection = vBase.y + Directions::QUADRANT3;
		}
		else if ((pCmd->buttons & IN_BACK) && (pCmd->buttons & IN_MOVELEFT))
		{
			m_flWishDirection = vBase.y + Directions::QUADRANT4;
		}
		else
		{
			pCmd->buttons |= IN_FORWARD;
			m_flWishDirection = vBase.y + Directions::FORWARDS;
		}

		// TODO: Fix clamping
		float flSpeedRotation = min(m_flRadToDeg(std::asinf(30.f / pLocalEnt->GetVelocity().Length2D())) * 0.5f, 45.f);
		if (m_bInTransition)
		{
			// Unused, might make an option in future if improved
			float flIdealStep = flSpeedRotation + m_flCalculatedDirection;
			m_flStep = fabsf(m_flNormalizeYaw(m_flCalculatedDirection - flIdealStep));

			if (fabsf(m_flNormalizeYaw(m_flWishDirection - m_flCalculatedDirection)) > m_flStep)
			{
				float flAdd = m_flNormalizeYaw(m_flCalculatedDirection + m_flStep);
				float flSub = m_flNormalizeYaw(m_flCalculatedDirection - m_flStep);

				if (fabsf(m_flNormalizeYaw(m_flWishDirection - flAdd)) >= fabsf(m_flNormalizeYaw(m_flWishDirection - flSub)))
				{
					m_flCalculatedDirection -= m_flStep;
				}
				else {
					m_flCalculatedDirection += m_flStep;
				}
			}
			else
			{
				m_bInTransition = false;
			}
		}
		else
		{
			m_flRoughDirection = flGetRoughDirection(m_flTrueDirection);
			m_flCalculatedDirection = m_flRoughDirection;

			if (m_flRoughDirection != m_flWishDirection)
			{
				m_bInTransition = true;
			}
		}

		pCmd->forwardmove = 0.f;
		pCmd->sidemove = pCmd->command_number % 2 ? 450.f : -450.f;

		float flDirection = (pCmd->command_number % 2 ? flSpeedRotation : -flSpeedRotation) + m_flCalculatedDirection;

		float rotation = m_flDegToRad(vBase.y - flDirection);

		float flCosRotation = cos(rotation);
		float flSinRotation = sin(rotation);

		float flForwardmove = (flCosRotation * pCmd->forwardmove) - (flSinRotation * pCmd->sidemove);
		float flSidemove = (flSinRotation * pCmd->forwardmove) + (flCosRotation * pCmd->sidemove);

		pCmd->forwardmove = flForwardmove;
		pCmd->sidemove = flSidemove;
	}

	void AutoStrafenormal(CUserCmd* pCmd) const
	{

	
		if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !g_Options.misc.air_strafe)
			return;

		if (!g::pLocalEntity->IsAlive())
			return;



		if (g::pLocalEntity->GetMoveType() == movetype_ladder || g::pLocalEntity->GetMoveType() == movetype_noclip)
			return;

		if (!GetAsyncKeyState(VK_SPACE) || g::pLocalEntity->GetVelocity().Length2D() < 0.5)
			return;

		if (!(g::pLocalEntity->GetFlags() & FL_ONGROUND))
		{
			float speed = g::pLocalEntity->GetVelocity().Length2D();
			Vector velocity = g::pLocalEntity->GetVelocity();
			float yawVelocity = RAD2DEG(atan2(velocity.y, velocity.x));
			float velocityDelta = normalize_yaw(pCmd->viewangles.y - yawVelocity);
			static float sideSpeed = g_pCvar->FindVar("cl_sidespeed")->GetFloat();

			if (fabsf(pCmd->mousedx > 2)) {

				pCmd->sidemove = (pCmd->mousedx < 0.f) ? -sideSpeed : sideSpeed;
				return;
			}

			if (pCmd->buttons & IN_BACK)
				pCmd->viewangles.y -= 180.f;
			else if (pCmd->buttons & IN_MOVELEFT)
				pCmd->viewangles.y -= 90.f;
			else if (pCmd->buttons & IN_MOVERIGHT)
				pCmd->viewangles.y += 90.f;

			if (!speed > 0.5f || speed == NAN || speed == INFINITE) {

				pCmd->forwardmove = 450.f;
				return;
			}

			pCmd->forwardmove = std::clamp(5850.f / speed, -450.f, 450.f);

			if ((pCmd->forwardmove < -450.f || pCmd->forwardmove > 450.f))
				pCmd->forwardmove = 0.f;

			pCmd->sidemove = (velocityDelta > 0.0f) ? -sideSpeed : sideSpeed;
			pCmd->viewangles.y = normalize_yaw(pCmd->viewangles.y - velocityDelta);
		}
		
	}

};

extern Misc g_Misc;
