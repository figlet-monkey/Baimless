#include "Aimbot.h"
#include "Autowall.h"
#include "..\AntiAim\AntiAim.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\Utils\Math.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\newMenu.h"
#include "..\..\SDK\IVModelInfo.h"
#include "..\..\Playerlist.h"
#include "..\..\SDK\CInput.h"
#include "..\..\SDK\CEntity.h"
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CPrediction.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include "../Resolver/Resolver.h"
#include "../../Hooks.h"
#include "../../../cthash.h"
#include "../../../md5.h"
#include <Options.hpp>

c_prediction engine_prediction;

void c_prediction::start_prediction(CUserCmd* command) noexcept
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!local_player)
		return;
	CMoveData move_data;

	if (local_player) {
		static bool initialized = false;
		if (!initialized) {
			prediction_random_seed = *(int**)(Utils::FindSignature(("client_panorama.dll"), "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 2);
			initialized = true;
		}

		*prediction_random_seed = utilities::md5::pseduo_random(command->command_number) & 0x7FFFFFFF;

		old_cur_time = g_pGlobalVars->curtime;
		old_frame_time = g_pGlobalVars->frametime;

		g_pGlobalVars->curtime = local_player->GetTickBase() * g_pGlobalVars->intervalPerTick; //tick_base_test
		g_pGlobalVars->frametime = g_pGlobalVars->intervalPerTick;

		g_pMovement->StartTrackPredictionErrors(local_player);

		memset(&move_data, 0, sizeof(move_data));
		g_pMoveHelper->SetHost(local_player);
		g_pPrediction->SetupMove(local_player, command, g_pMoveHelper, &move_data);
		g_pMovement->ProcessMovement(local_player, &move_data);
		g_pPrediction->FinishMove(local_player, command, &move_data);
	}
}

void c_prediction::end_prediction() noexcept {
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!local_player)
		return;

	if (local_player) {
		g_pMovement->FinishTrackPredictionErrors(local_player);
		g_pMoveHelper->SetHost(0);

		*prediction_random_seed = -1;

		g_pGlobalVars->curtime = old_cur_time;
		g_pGlobalVars->frametime = old_cur_time;
	}
}

bool strstric(const std::string& strHaystack, const std::string& strNeedle)
{
	auto it = std::search(
		strHaystack.begin(), strHaystack.end(),
		strNeedle.begin(), strNeedle.end(),
		[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
	);
	return (it != strHaystack.end());
}

bool   g_Un43load = false;

void chams123::dark_mode() const
{
	//if (!g_Options.esp.effects.night_mode)
		//return;
	const auto reset = [&]()
	{
		_(r_DrawSpecificStaticProp, "r_DrawSpecificStaticProp");
		g_pCvar->FindVar(r_DrawSpecificStaticProp)->SetValue(1);

		_(World, "World");
		_(SkyBox, "SkyBox");
		_(StaticProp, "StaticProp");

		for (auto i = g_pMaterialSys->FirstMaterial(); i != g_pMaterialSys->InvalidMaterial(); i = g_pMaterialSys->NextMaterial(i))
		{
			auto mat = g_pMaterialSys->GetMaterial(i);
			if (!mat)
				continue;

			if (mat->IsErrorMaterial())
				continue;

			std::string name = mat->GetName();
			auto tex_name = mat->GetTextureGroupName();


			if (strstr(tex_name, World) || strstr(tex_name, StaticProp) || strstr(tex_name, SkyBox))
			{
				mat->ColorModulate(1.f, 1.f, 1.f);
				mat->AlphaModulate(1.f);
			}
		}
	};

	_(sky_csgo_night02, "sky_csgo_night02");
	const auto set = [&]()
	{
		static auto load_named_sky = reinterpret_cast<void(__fastcall*)(const char*)>(Utils::FindSignature("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45"));
		load_named_sky(sky_csgo_night02);

		_(r_DrawSpecificStaticProp, "r_DrawSpecificStaticProp");
		g_pCvar->FindVar(r_DrawSpecificStaticProp)->SetValue(0);

		_(World, "World");
		_(SkyBox, "SkyBox");
		_(StaticProp, "StaticProp");
		_(wall, "wall");

		for (auto i = g_pMaterialSys->FirstMaterial(); i != g_pMaterialSys->InvalidMaterial(); i = g_pMaterialSys->NextMaterial(i))
		{
			auto mat = g_pMaterialSys->GetMaterial(i);
			if (!mat)
				continue;

			if (mat->IsErrorMaterial())
				continue;

			std::string name = mat->GetName();
			auto tex_name = mat->GetTextureGroupName();

			if (g_Options.esp.effects.night_mode && strstr(tex_name, World))
			{
				mat->ColorModulate(0.15f, 0.15f, 0.15f);
			}
			if (strstr(tex_name, StaticProp))
			{
				/*if ( !strstric( name, "box" ) && !strstric( name, "crate" ) && !strstric( name, "door" ) && !
					strstric( name, "stoneblock" ) && !strstric( name, "tree" ) && !strstric( name, "flower" ) && !
					strstric( name, "light" ) && !strstric( name, "lamp" ) && !strstric( name, "props_junk" ) && !
					strstric( name, "props_pipe" ) && !strstric( name, "chair" ) && !strstric( name, "furniture" ) && !
					strstric( name, "debris" ) && !strstric( name, "tire" ) && !strstric( name, "refrigerator" ) && !
					strstric( name, "fence" ) && !strstric( name, "pallet" ) && !strstric( name, "barrel" ) && !strstric(
						 name, "wagon" ) && !strstric( name, "wood" ) && !strstric( name, "wall" ) && !strstric( name, "pole" ) && !strstric( name, "props_urban" ) && !strstric( name, "bench" ) && !strstric( name, "trashcan" ) && !strstric( name, "infwll" ) && !strstric( name, "cash_register" ) && !strstric( name, "prop_vehicles" ) && !strstric( name, "rocks" ) && !strstric( name, "artillery" ) && !strstric( name, "plaster_brick" ) && !strstric( name, "props_interiors" ) && !strstric( name, "props_farm" ) && !strstric( name, "props_highway" ) && !strstric( name, "orange" ) && !strstric( name, "wheel" ) )
					continue;*/

				if (g_Options.esp.effects.night_mode)
					mat->ColorModulate(0.4f, 0.4f, 0.4f);
			}

			if (g_Options.esp.effects.night_mode && strstr(tex_name, SkyBox))
			{
				mat->ColorModulate(228.f / 255.f, 35.f / 255.f, 157.f / 255.f);
			}

		}


	};

	static auto done = true;
	static auto last_setting = false;
	static auto was_ingame = false;
	static auto unloaded = false;

	if (!done)
	{
		if (last_setting)
		{
			reset();
			set();
			done = true;
		}
		else
		{
			reset();
			done = true;
		}
	}

	if (unloaded)
		return;

	if (g_Un43load || was_ingame != g_pEngine->IsInGame() || last_setting != g_Options.esp.effects.night_mode)
	{
		last_setting = g_Options.esp.effects.night_mode;
		was_ingame = g_pEngine->IsInGame();
		unloaded = g_Un43load;

		if (g_Un43load)
			last_setting = false;

		done = false;
	}
}



Aimbot g_Aimbot;

#define   CONTENTS_SOLID                0x1       
#define   CONTENTS_MOVEABLE             0x4000
#define   CONTENTS_MONSTER              0x2000000 
#define   CONTENTS_WINDOW               0x2
#define   CONTENTS_DEBRIS               0x4000000
#define   CONTENTS_HITBOX               0x40000000
#define   MASK_SHOT                     (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define	CONTENTS_GRATE			0x8		// alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't

#define	MASK_SOLID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)






void Aimbot::Autostop()
{
	if (!g_Options.rage.autostop)
		return;

	Vector Velocity = g::pLocalEntity->GetVelocity();

	if (Velocity.Length2D() == 0)
		return;
	static float Speed = 450.f;

	Vector Direction;
	Vector RealView;
	g_Math.VectorAngles(Velocity, Direction);
	g_pEngine->GetViewAngles(RealView);
	Direction.y = RealView.y - Direction.y;

	Vector Forward;
	g_Math.AngleVectors(Direction, &Forward);
	Vector NegativeDirection = Forward * -Speed;

	g::pCmd->forwardmove = NegativeDirection.x;
	g::pCmd->sidemove = NegativeDirection.y;

}




bool ShouldBaim(C_BaseEntity* pEnt) // probably dosnt make sense
{
	static float oldSimtime[65];
	static float storedSimtime[65];

	static float ShotTime[65];
	static float NextShotTime[65];
	static bool BaimShot[65];

	if (storedSimtime[pEnt->EntIndex()] != pEnt->GetSimulationTime())
	{
		oldSimtime[pEnt->EntIndex()] = storedSimtime[pEnt->EntIndex()];
		storedSimtime[pEnt->EntIndex()] = pEnt->GetSimulationTime();
	}

	float simDelta = storedSimtime[pEnt->EntIndex()] - oldSimtime[pEnt->EntIndex()];

	bool Shot = false;

	if (pEnt->GetActiveWeapon() && !pEnt->IsKnifeorNade())
	{
		if (ShotTime[pEnt->EntIndex()] != pEnt->GetActiveWeapon()->GetLastShotTime())
		{
			Shot = true;
			BaimShot[pEnt->EntIndex()] = false;
			ShotTime[pEnt->EntIndex()] = pEnt->GetActiveWeapon()->GetLastShotTime();
		}
		else
			Shot = false;
	}
	else
	{
		Shot = false;
		ShotTime[pEnt->EntIndex()] = 0.f;
	}

	if (Shot)
	{
		NextShotTime[pEnt->EntIndex()] = pEnt->GetSimulationTime() + pEnt->FireRate();

		if (simDelta >= pEnt->FireRate())
			BaimShot[pEnt->EntIndex()] = true;
	}

	if (BaimShot[pEnt->EntIndex()])
	{
		if (pEnt->GetSimulationTime() >= NextShotTime[pEnt->EntIndex()])
			BaimShot[pEnt->EntIndex()] = false;
	}
	std::vector<int> baim_hitboxes;

	baim_hitboxes.push_back((int)HITBOX_UPPER_CHEST);
	baim_hitboxes.push_back((int)HITBOX_LOWER_CHEST);
	baim_hitboxes.push_back((int)HITBOX_PELVIS);
	baim_hitboxes.push_back((int)HITGROUP_STOMACH);

	float Damage = 0;
	for (auto HitBoxID : baim_hitboxes)
	{
		Damage = g_Autowall.Damage(pEnt->GetHitboxPosition(HitBoxID, g_Aimbot.Matrix[pEnt->EntIndex()]));
	}

	//std::vector<dropdownboxitem> baim_list = Options::Menu.RageBotTab.avoidhead_if.items;
    auto baim_list = g_Options.rage.avoid_head_selected;

	if (baim_list[2] && BaimShot[pEnt->EntIndex()] && !(pEnt->GetFlags() & FL_ONGROUND))
		return true;

	if (baim_list[1] && !(pEnt->GetFlags() & FL_ONGROUND))
		return true;

	if (baim_list[3] && GetAsyncKeyState(g_Options.rage.baimkey))
		return true;


	//if (g_Resolver.Baim_ResolverMissed = true
		//return true;
	
	if (baim_list[5] && pEnt->GetVelocity().Length2D() > 10.0f && !(pEnt->GetFlags() & FL_ONGROUND))
		return true;
	if (baim_list[6] && pEnt->GetVelocity().Length2D() > 20.0f)
		return true;
	//CPlayer* Player = plist.get_player(pEnt->EntIndex());
	//if (Player->PreferBodyAim) // maybe works?
		//return true;

	if (baim_list[4] && g_Aimbot.data[pEnt->EntIndex()].shotsmissed >= g_Options.rage.avoid_head_x)
		return true;

	int health = g_Options.rage.avoid_head;

	if (health >= pEnt->GetHealth())
	{
		//BAIM
		return true;

	}

	if (pEnt->GetHealth() <= Damage && baim_list[0])
		return true;

	return false;
}

Vector Aimbot::Hitscan(C_BaseEntity* pEnt) // supremeemmemememememe
{
	auto weapon = g::pLocalEntity->GetActiveWeapon();

	if (weapon)
	{
		if (*weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_ZEUS)
		{

			float DamageArray[28];
			float tempDmg = 0.f;
			Vector tempHitbox = { 0,0,0 };


			float Velocity = abs(pEnt->GetVelocity().Length2D());


			std::vector<int> Scan;

			//std::vector<dropdownboxitem> auto_list = Options::Menu.RageBotTab.target_auto.items;
            auto auto_list = g_Options.rage.hitscan_selected;

			Scan.push_back(HITBOX_PELVIS);




			Vector Hitbox;
			int bestHitboxint = 0;

			for (int hitbox : Scan)
			{
				Hitbox = pEnt->GetHitboxPosition(hitbox, Matrix[pEnt->EntIndex()]);

				float Damage = g_Autowall.Damage(Hitbox);

				if (Damage > 0.f)
					DamageArray[hitbox] = Damage;
				else
					DamageArray[hitbox] = 0;

				if (DamageArray[hitbox] > tempDmg)
				{
					tempHitbox = Hitbox;
					bestHitboxint = hitbox;
					tempDmg = DamageArray[hitbox];
				}

				g::AimbotHitbox[pEnt->EntIndex()][hitbox] = Hitbox;



				Backtrack[pEnt->EntIndex()] = false;
				ShotBacktrack[pEnt->EntIndex()] = false;

				if (tempDmg >= 80 || tempDmg >= pEnt->GetHealth())
				{

					return tempHitbox;
				}




			}
			return Vector(0, 0, 0);
		}
		else
		{
			float DamageArray[28];
			float tempDmg = 0.f;
			Vector tempHitbox = { 0,0,0 };
			static int HitboxForMuti[] = { 2,2,4,4,6,6 };

			float angToLocal = g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pEnt->GetOrigin()).y;

			Vector2D MutipointXY = { (sin(g_Math.GRD_TO_BOG(angToLocal))),(cos(g_Math.GRD_TO_BOG(angToLocal))) };
			Vector2D MutipointXY180 = { (sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };
			Vector2D Mutipoint[] = { Vector2D(MutipointXY.x, MutipointXY.y), Vector2D(MutipointXY180.x, MutipointXY180.y) };

			float Velocity = abs(pEnt->GetVelocity().Length2D());

			if (Velocity > 29.f)
				Velocity = 30.f;
            auto auto_list = g_Options.rage.hitscan_selected;

			std::vector<int> Scan;

			int HeadHeight = 0;

			bool Baim = ShouldBaim(pEnt);

			if (Baim)
			{
			
				Scan.push_back(HITBOX_PELVIS);
				Scan.push_back(HITBOX_BELLY);
				Scan.push_back(HITBOX_THORAX);
				Scan.push_back(HITBOX_LOWER_CHEST);
				Scan.push_back(HITBOX_UPPER_CHEST);


			}
			else
			{
				if (auto_list[0])
				{
					Scan.push_back(HITBOX_HEAD);

				}
				if (auto_list[1])
				{
					Scan.push_back(HITBOX_NECK);

				}
				if (auto_list[2])
				{
					Scan.push_back(HITBOX_PELVIS);
					Scan.push_back(HITBOX_BELLY);
				}
				if (auto_list[3])
				{
					Scan.push_back(HITBOX_THORAX);
					Scan.push_back(HITBOX_LOWER_CHEST);
					Scan.push_back(HITBOX_UPPER_CHEST);
				}
				if (auto_list[4])
				{
					Scan.push_back(HITBOX_RIGHT_CALF);
					Scan.push_back(HITBOX_LEFT_CALF);
					Scan.push_back(HITBOX_RIGHT_UPPER_ARM);
					Scan.push_back(HITBOX_RIGHT_FOREARM);
					Scan.push_back(HITBOX_LEFT_UPPER_ARM);
					Scan.push_back(HITBOX_LEFT_FOREARM);
				}
				if (auto_list[5])
				{
					if (Velocity <= 29.f)
					{
						Scan.push_back(HITBOX_RIGHT_FOOT);
						Scan.push_back(HITBOX_LEFT_FOOT);
						Scan.push_back(HITBOX_RIGHT_HAND);
						Scan.push_back(HITBOX_LEFT_HAND);
						Scan.push_back(HITBOX_RIGHT_THIGH);
						Scan.push_back(HITBOX_LEFT_THIGH);
					}
				}
				


				
			}
	




			Vector Hitbox;
			int bestHitboxint = 0;

			for (int hitbox : Scan)
			{
				Hitbox = pEnt->GetHitboxPosition(hitbox, Matrix[pEnt->EntIndex()]);
				

				float Damage = g_Autowall.Damage(Hitbox);

				if (Damage > 0.f)
					DamageArray[hitbox] = Damage;
				else
					DamageArray[hitbox] = 0;


				if (DamageArray[hitbox] > tempDmg)
				{
					tempHitbox = Hitbox;
					bestHitboxint = hitbox;
					tempDmg = DamageArray[hitbox];
				}
				g::AimbotHitbox[pEnt->EntIndex()][hitbox] = Hitbox;

			}

			PlayerRecords pPlayerEntityRecord = g_LagComp.PlayerRecord[pEnt->EntIndex()].at(0);

			Backtrack[pEnt->EntIndex()] = false;
			ShotBacktrack[pEnt->EntIndex()] = false;
			if (tempDmg >= g_Options.rage.autowall_damage_hitchance)
			{
				bestEntDmg = tempDmg;

				
				return tempHitbox;
			}

			return Vector(0, 0, 0);
		}

	}

}

float GetCurtime()
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player)
		return -1;


	if (!g::pLocalEntity->GetTickBase())
		return -1;

	return static_cast<float>(local_player->GetTickBase()) * g_pGlobalVars->intervalPerTick;
}


int bestHitbox = -1;

Vector Aimbot::get_hitbox_pos(C_BaseEntity* entity, int hitbox_id)
{
	auto getHitbox = [](C_BaseEntity* entity, int hitboxIndex) -> mstudiobbox_t*
	{
		if (entity->IsDormant() || entity->GetHealth() <= 0) return NULL;

		const auto pModel = entity->GetModel();
		if (!pModel) return NULL;

		auto pStudioHdr = g_pModelInfo->GetStudiomodel(pModel);
		if (!pStudioHdr) return NULL;

		auto pSet = pStudioHdr->GetHitboxSet(0);
		if (!pSet) return NULL;

		if (hitboxIndex >= pSet->numhitboxes || hitboxIndex < 0) return NULL;

		return pSet->GetHitbox(hitboxIndex);
	};

	auto hitbox = getHitbox(entity, hitbox_id);
	if (!hitbox) return Vector(0, 0, 0);

	auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

	Vector bbmin, bbmax;
	g_Math.VectorTransform(hitbox->min, bone_matrix, bbmin);
	g_Math.VectorTransform(hitbox->max, bone_matrix, bbmax);

	return (bbmin + bbmax) * 0.5f;
}


int Aimbot::zeus_hitbox(C_BaseEntity* entity)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!local_player)
		return -1;

	Vector local_position = local_player->GetOrigin() + local_player->GetViewOffset();

	float closest = 35.f;

	bestHitbox = -1;

	Vector point = get_hitbox_pos(entity, HITBOX_PELVIS);

	if (point != Vector(0, 0, 0))
	{

		

		float distance = fabs((point - g::pLocalEntity->get_eye_pos()).Length());

		if (distance <= closest)
		{
			bestHitbox = HITBOX_PELVIS;
		}
	}

	return bestHitbox;
}

void UTIL_TraceLine(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, C_BaseEntity* ignore, C_Trace* ptr)
{
	CTraceEntity traceFilter(ignore);
	traceFilter.pSkip1 = ignore;
	g_pTrace->TraceRay(C_Ray(vecAbsStart, vecAbsEnd), mask, &traceFilter, ptr);
}



bool Aimbot::HitChance(C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance)
{
	if (chance == 0 )
		return true;

	float Seeds = 256.f;

	Angle -= (g::pLocalEntity ->GetAimPunchAngle() * g_pCvar->FindVar("weapon_recoil_scale")->GetFloat());

	Vector forward, right, up;

	g_Math.AngleVectors(Angle, &forward, &right, &up);

	int Hits = 0, neededHits = (Seeds * (chance / 100.f));

	float weapSpread = pWeapon->GetSpread(), weapInaccuracy = pWeapon->GetInaccuracy();

	for (int i = 0; i < Seeds; i++)
	{
		float Inaccuracy = g_Math.RandomFloat(0.f, 1.f) * weapInaccuracy;
		float Spread = g_Math.RandomFloat(0.f, 1.f) * weapSpread;

		Vector spreadView((cos(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (cos(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Spread), (sin(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (sin(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Spread), 0), direction;
		direction = Vector(forward.x + (spreadView.x * right.x) + (spreadView.y * up.x), forward.y + (spreadView.x * right.y) + (spreadView.y * up.y), forward.z + (spreadView.x * right.z) + (spreadView.y * up.z)).Normalize();

		Vector viewanglesSpread, viewForward;

		g_Math.VectorAngles(direction, up, viewanglesSpread);
		g_Math.NormalizeAngles(viewanglesSpread);

		g_Math.AngleVectors(viewanglesSpread, &viewForward);
		viewForward.NormalizeInPlace();

		viewForward = g::pLocalEntity->get_eye_pos() + (viewForward * pWeapon->GetCSWpnData()->range);

		C_Trace Trace;

		g_pTrace->ClipRayToEntity(C_Ray(g::pLocalEntity->get_eye_pos(), viewForward), mask_shot | contents_grate, pEnt, &Trace);

		if (Trace.m_pEnt == pEnt)
			Hits++;

		if (((Hits / Seeds) * 100.f) >= chance)
			return true;

		if ((Seeds - i + Hits) < neededHits)
			return false;
	}

	return false;
}
static int sinceUse;
void double_tap()
{
	//if (Options::Menu.AntiAimTab.Doubel_tap.GetState())
//	{
		//if (!g_pClientState->chokedcommands)
		//	m_nTickbaseShift = TIME_TO_TICKS(7.f);
		
	//	m_nTickbaseShift = TIME_TO_TICKS(2.f);

	//}
}




void doubletap_test()
{
	/*
	static int sinceUse;

	if (GetAsyncKeyState(c_config::get().doubletap_bind2) && (Globals::pCmd->buttons & IN_ATTACK))
	{
		if (Globals::nSinceUse++ < 3)
			Globals::m_nTickbaseShift = TIME_TO_TICKS(2.0f);
		sinceUse++;
		Globals::pCmd->tick_count = (int)((DWORD)Globals::pCmd->tick_count) + TIME_TO_TICKS(0.5f) + (Globals::m_nTickbaseShift + 2);
		Globals::m_nTickbaseShift = TIME_TO_TICKS(0.5f);
		Globals::m_nTickbaseShift = TIME_TO_TICKS(2.0f);
		Globals::m_nTickbaseShift = TIME_TO_TICKS(1.0f);
		Globals::m_nTickbaseShift = TIME_TO_TICKS(0.5f);


		C_BaseEntity* Target = nullptr;
		static int choked = 0;
		static int MinimumVelocity;
		choked = choked > 7 ? 0 : choked + 1;
		Globals::pCmd->sidemove = choked < 2 || choked > 5 ? 0 : Globals::pCmd->sidemove;
		Globals::pCmd->forwardmove = choked < 2 || choked > 5 ? 0 : Globals::pCmd->forwardmove;
		Globals::needtoshift = true;
		g_Misc.Choker(6);
		int ticks1 = 6; // Получение кол-ва пакетов, которые нужно чокнуть
		int choked1 = g_pEngine->GetNetChannel()->m_nChokedPackets; // Получаю чокнутые пакеты
		bool should_duck = choked >= (ticks1 / 2); // Проверка на то, нужно ли нам сесть
		MinimumVelocity = 0;
		auto me = Globals::LocalPlayer;
		auto cmd = Globals::pCmd;
		if ((Globals::LocalPlayer->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SCAR20 || Globals::LocalPlayer->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_G3SG1 || Globals::LocalPlayer->GetActiveWeapon()->GetItemDefinitionIndex() == ItemDefinitionIndex::WEAPON_SSG08)) {
			if (c_config::get().auto_scope) {
				if (!me->IsScoped()) {
					cmd->buttons |= IN_ATTACK2;
					return;
				}
			}
			if (Target)
			{
				float flServerTime = Globals::LocalPlayer->GetTickBase() * g_pGlobalVars->intervalPerTick;
				bool canShoot = (Globals::LocalPlayer->GetActiveWeapon()->GetNextPrimaryAttack() <= flServerTime && Globals::LocalPlayer->GetActiveWeapon()->GetAmmo() > 0);

				float SimulationTime = 0.f;

				static int MinimumVelocity = 0;

				if (c_config::get().autostop_mode == 0)
				{
					MinimumVelocity = Globals::LocalPlayer->GetActiveWeapon()->GetCSWpnData()->max_speed_alt * .34f;
				}
				else if (c_config::get().autostop_mode == 1)
				{
					MinimumVelocity = 0;
				}

				bool shouldstop = c_config::get().stop_inbetween_shots ? true : canShoot;
			}
		}
		else {
			Globals::nSinceUse = 0;
			sinceUse = 0;
		}

		if ((sinceUse + 1) >= 3)
		{
			Globals::needtoshift = false;
		}
	}*/
}

bool can_shoot(CUserCmd* cmd)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player) return false;
	if (local_player->GetHealth() <= 0)
		return false;

	auto weapon = g::pLocalEntity->GetActiveWeapon();
	if (!weapon || weapon->GetAmmo() == 0)
		return false;

	return (weapon->GetNextPrimaryAttack() < GetCurtime()) && (local_player->GetNextAttack() < GetCurtime());
}
#define square( x ) ( x * x )
void ClampMovement(CUserCmd* pCommand, float fMaxSpeed)
{
	if (fMaxSpeed <= 0.f)
		return;
	float fSpeed = (float)(FastSqrt(square(pCommand->forwardmove) + square(pCommand->sidemove) + square(pCommand->upmove)));
	if (fSpeed <= 0.f)
		return;
	if (pCommand->buttons & IN_DUCK)
		fMaxSpeed *= 2.94117647f;
	if (fSpeed <= fMaxSpeed)
		return;
	float fRatio = fMaxSpeed / fSpeed;
	pCommand->forwardmove *= fRatio;
	pCommand->sidemove *= fRatio;
	pCommand->upmove *= fRatio;
}

float get_gun(C_BaseCombatWeapon* weapon)
{

	if (!weapon)
		return 0.f;

	if (weapon->isAuto())
		return 25.f;

	else if (weapon->is_scout())
		return 70.f;

	else if (weapon->is_awp())
		return 30.f;

	else
		return 34.f;

}
void Aimbot::OnCreateMove()
{
	if (!g_pEngine->IsInGame())
		return;

	Vector Aimpoint = { 0,0,0 };
	C_BaseEntity* Target = nullptr;

	int targetID = 0;
	int tempDmg = 0;
	auto weapon = g::pLocalEntity->GetActiveWeapon();

	for (int i = 1; i <= g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);
	
		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity->IsImmune())
		{
			g_LagComp.ClearRecords(i);
			continue;
		}
		g_LagComp.StoreRecord(pPlayerEntity);


		if (!pPlayerEntity || !pPlayerEntity->IsAlive() || pPlayerEntity->IsDormant())
			continue;

		if (pPlayerEntity == g::pLocalEntity || pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		g_Aimbot.data[pPlayerEntity->EntIndex()].shotsmissed = g_Aimbot.data[pPlayerEntity->EntIndex()].shoots - g_Aimbot.data[pPlayerEntity->EntIndex()].shoots_hit; //Add to our logs

		if (g_Aimbot.data[pPlayerEntity->EntIndex()].shoots >= 10)
		{
			g_Aimbot.data[pPlayerEntity->EntIndex()].shoots_hit = 0;
			g_Aimbot.data[pPlayerEntity->EntIndex()].shoots = 0;
			g_Aimbot.data[pPlayerEntity->EntIndex()].shotsmissed = 0;

		}

		if (!g::pLocalEntity->IsAlive() || !g_Options.rage.enable)
			continue;

		if (g_LagComp.PlayerRecord[i].size() == 0)
			continue;

		//CPlayer* Player = plist.get_player(pPlayerEntity->EntIndex());
		//if (Player->Friendly) // works
			//continue;

		if (!g::pLocalEntity->GetActiveWeapon() || g::pLocalEntity->IsKnifeorNade())
			continue;

		if (!weapon)
			continue;

		if (!g::pLocalEntity->GetTickBase())
			continue;

		//if (!can_shoot(g::pCmd))
	//	{
		//	g::pCmd->buttons &= ~IN_ATTACK;
		//	return;
		//}

		if (pPlayerEntity->IsImmune())
			continue;

		if (pPlayerEntity->IsDormant())
			continue;




		if (weapon->is_grenade() || weapon->is_knife())
			return;

		if (weapon->GetAmmo() == 0)
			return;

		
		bestEntDmg = 0;

		Vector Hitbox = Hitscan(pPlayerEntity);

		if (Hitbox != Vector(0, 0, 0) && tempDmg <= bestEntDmg)
		{
			Aimpoint = Hitbox;
			Target = pPlayerEntity;
			targetID = Target->EntIndex();
			tempDmg = bestEntDmg;

		}


	}


	if (Target)
	{
		g::TargetIndex = targetID;

		float SimulationTime = 0.f;

		SimulationTime = g_LagComp.PlayerRecord[targetID].at(g_LagComp.PlayerRecord[targetID].size() - 1).SimTime;

		Vector Angle = g_Math.CalcAngle(g::pLocalEntity->get_eye_pos(), Aimpoint);

		static int MinimumVelocity = 0;

		
		MinimumVelocity = g::pLocalEntity->GetActiveWeapon()->GetCSWpnData()->max_speed_alt * .34f;
		
		


		if (g::pLocalEntity->GetVelocity().Length() >= MinimumVelocity && !GetAsyncKeyState(VK_SPACE))
			Autostop();

		if ((*g::pLocalEntity->GetActiveWeapon()->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_SCAR20 || *g::pLocalEntity->GetActiveWeapon()->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_G3SG1 || *g::pLocalEntity->GetActiveWeapon()->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_SSG08)) 
		{
			if (g_Options.rage.autoscope)
			{
				if (!g::pLocalEntity->IsScoped()) {
					g::pCmd->buttons |= IN_ATTACK2;
					return;
				}
			}
		}

		if (can_shoot(g::pCmd) && HitChance(Target, g::pLocalEntity->GetActiveWeapon(), Angle, Aimpoint, g_Options.rage.min_hitchance))
		{


			if (g_Options.rage.silentaim)
			{
				g::pCmd->viewangles = Angle;

			}
			else
			{
				g_pEngine->SetViewAngles(Angle);

			}
			g::AimbotAngle = Angle;


			if (g_Options.rage.autofire)
			{
				g::pCmd->buttons |= IN_ATTACK;
			}

			g_Aimbot.data[Target->EntIndex()].shoots++; //Add to our logs
			this->aimbotted = true;


			g::pCmd->tick_count = TIME_TO_TICKS(SimulationTime + g_LagComp.get_lerp_time());

		}
		
		
	}
	
	/*
	
	if (Target)
	{
		//PlayerRecords pPlayerEntityRecord = g_LagComp.PlayerRecord[Target->EntIndex()].at(0);



		//g_LagComp.apply(Target, true);// BACKUP

		if (*weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_ZEUS)
		{
			g::TargetIndex = targetID;

			float SimulationTime = 0.f;

			SimulationTime = g_LagComp.PlayerRecord[targetID].at(g_LagComp.PlayerRecord[targetID].size() - 1).SimTime;

	

							
				


				//g::pCmd->viewangles = Angle - (g::pLocalEntity->GetAimPunchAngle() * g_pCvar->FindVar("weapon_recoil_scale")->GetFloat());


				int bone = zeus_hitbox(Target); //you can change this but keep in mind this has range stuff. it only has pelvis as a bone but why do other stuff really it will make it inaccurate shooting at arms and legs if they arent resolved right 

				if (bone != 1)
				{
						




					C_Trace trace;

					UTIL_TraceLine(g::pLocalEntity->get_eye_pos(), Aimpoint, MASK_SOLID, g::pLocalEntity, &trace);


					if (trace.m_pEnt == nullptr)
						return;
					if (trace.m_pEnt == g::pLocalEntity)
						return;
					if (!trace.m_pEnt->IsAlive())
						return;
					if (!(trace.m_pEnt->GetHealth() > 0))
						return;
					if (trace.m_pEnt->GetTeam() == g::pLocalEntity->GetTeam())
						return;
					if (trace.m_pEnt->IsDormant())
						return;
					if (trace.m_pEnt->IsImmune())
						return;
					PlayerInfo_t info;

					if (!(g_pEngine->GetPlayerInfo(trace.m_pEnt->EntIndex(), &info)))
						return;

							

					Vector Angle = g_Math.CalcAngle(g::pLocalEntity->get_eye_pos(), Aimpoint);
					
					if (HitChance(Target, g::pLocalEntity->GetActiveWeapon(), Angle, Aimpoint, Options::Menu.RageBotTab.chance_val.GetValue()))
					{
						if (Options::Menu.RageBotTab.silentaim.GetState())
						{
							g::pCmd->viewangles = Angle;

						}
						else
						{
							g_pEngine->SetViewAngles(Angle);

						}
						if (Options::Menu.RageBotTab.autofire.GetState())
						{
							g::pCmd->buttons |= IN_ATTACK;


						}

						g::pCmd->tick_count = TIME_TO_TICKS(SimulationTime + g_LagComp.get_lerp_time());

					}
					
						
				}


				
		}
		else
		{
			g::TargetIndex = targetID;

			float SimulationTime = 0.f;

			if (ShotBacktrack[targetID])
				SimulationTime = g_LagComp.PlayerRecord[targetID].at(g_LagComp.ShotTick[targetID]).SimTime;
			else
				SimulationTime = g_LagComp.PlayerRecord[targetID].at(g_LagComp.PlayerRecord[targetID].size() - 1).SimTime;

			

			Vector Angle = g_Math.CalcAngle(g::pLocalEntity->get_eye_pos(), Aimpoint);

			if (Options::Menu.RageBotTab.stop_bool.GetState())
			{
				if (g::pLocalEntity->GetFlags() & FL_ONGROUND)
				{
					ClampMovement(g::pCmd, get_gun(weapon));
				}
			}
		

			if (*weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_AWP || *weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_SSG08 ||
				*weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_SCAR20 || *weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_G3SG1 ||
				*weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_AUG || *weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_SG553)
				if (!g::pLocalEntity->IsScoped())
				{
					if (Options::Menu.RageBotTab.AutomaticScope.GetState())
						g::pCmd->buttons |= IN_ATTACK2;
					return;

				}
	
			

				//g::pCmd->viewangles = Angle - (g::pLocalEntity->GetAimPunchAngle() * g_pCvar->FindVar("weapon_recoil_scale")->GetFloat());


	

				//Angle -= g::pLocalEntity->GetAimPunchAngle() * g_pCvar->FindVar("weapon_recoil_scale")->GetFloat();
			if (HitChance(Target, g::pLocalEntity->GetActiveWeapon(), Angle, Aimpoint, Options::Menu.RageBotTab.chance_val.GetValue()))//50
			{


				if (Options::Menu.RageBotTab.silentaim.GetState())
				{
					g::pCmd->viewangles = Angle;

				}
				else
				{
					g_pEngine->SetViewAngles(Angle);

				}
				if (Options::Menu.RageBotTab.autofire.GetState())
				{
					

					if (Options::Menu.AntiAimTab.fake_crouch.GetState() && GetAsyncKeyState(Options::Menu.AntiAimTab.fake_crouchkey.GetKey()))
					{
						if (g::pLocalEntity->m_flDuckAmount() != 0)
							g::pCmd->buttons &= ~IN_ATTACK;
						
						g::pCmd->buttons |= IN_ATTACK;

						
					}
					else
					{
						g::pCmd->buttons |= IN_ATTACK;
					}

					this->aimbotted = true;


					g_Aimbot.data[Target->EntIndex()].shoots++; //Add to our logs

				}
				g::pCmd->tick_count = TIME_TO_TICKS(SimulationTime + g_LagComp.get_lerp_time());
			}
			
		}

		g_Aimbot.data[Target->EntIndex()].shotsmissed = g_Aimbot.data[Target->EntIndex()].shoots - g_Aimbot.data[Target->EntIndex()].shoots_hit; //Add to our logs

	}
	*/
}

void Aimbot::CompensateInaccuracies()
{
	
	if (g_Options.rage.norecoil)
	{
		ConVar* recoilscale = g_pCvar->FindVar("weapon_recoil_scale");

		if (recoilscale)
		{
			g::pCmd->viewangles -= g::pLocalEntity->GetAimPunchAngle() * recoilscale->GetFloat();

		}
	} 
	
}



void Aimbot::auto_revolver(CUserCmd* cmd)
{
	if (!g_Options.rage.autorevolver)
		return;
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player && local_player->GetHealth() <= 0) return;

	auto weapon = g::pLocalEntity->GetActiveWeapon();
	if (!weapon || weapon->GetAmmo() == 0)
		return;

	if (!weapon->GetPostponeFireReadyTime())
		return;

	if (*weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_REVOLVER)
	{


		float flPostponeFireReady = weapon->GetPostponeFireReadyTime();
		if (flPostponeFireReady > 0 && flPostponeFireReady < g_pGlobalVars->curtime)
		{
			cmd->buttons &= ~IN_ATTACK;
		}
		static int delay = 0;
		delay++;

		if (delay <= 15)
			cmd->buttons |= IN_ATTACK;
		else
			delay = 0;

	}

	////////


}


/*
bool ShouldBaim(C_BaseEntity* pEnt) // probably dosnt make sense
{
	static float oldSimtime[65];
	static float storedSimtime[65];

	static float ShotTime[65];
	static float NextShotTime[65];
	static bool BaimShot[65];

	if (storedSimtime[pEnt->EntIndex()] != pEnt->GetSimulationTime())
	{
		oldSimtime[pEnt->EntIndex()] = storedSimtime[pEnt->EntIndex()];
		storedSimtime[pEnt->EntIndex()] = pEnt->GetSimulationTime();
	}

	float simDelta = storedSimtime[pEnt->EntIndex()] - oldSimtime[pEnt->EntIndex()];

	bool Shot = false;

	if (pEnt->GetActiveWeapon() && !pEnt->IsKnifeorNade())
	{
		if (ShotTime[pEnt->EntIndex()] != pEnt->GetActiveWeapon()->GetLastShotTime())
		{
			Shot = true;
			BaimShot[pEnt->EntIndex()] = false;
			ShotTime[pEnt->EntIndex()] = pEnt->GetActiveWeapon()->GetLastShotTime();
		}
		else
			Shot = false;
	}
	else
	{
		Shot = false;
		ShotTime[pEnt->EntIndex()] = 0.f;
	}

	if (Shot)
	{
		NextShotTime[pEnt->EntIndex()] = pEnt->GetSimulationTime() + pEnt->FireRate();

		if (simDelta >= pEnt->FireRate())
			BaimShot[pEnt->EntIndex()] = true;
	}

	if (BaimShot[pEnt->EntIndex()])
	{
		if (pEnt->GetSimulationTime() >= NextShotTime[pEnt->EntIndex()])
			BaimShot[pEnt->EntIndex()] = false;
	}

	if (Options::Menu.RageBotTab.BaimPitch.GetState() && BaimShot[pEnt->EntIndex()] && !(pEnt->GetFlags() & FL_ONGROUND))
		return true;

	if (Options::Menu.RageBotTab.baiminair.GetState() && !(pEnt->GetFlags() & FL_ONGROUND))
		return true;

	if (GetAsyncKeyState(Options::Menu.RageBotTab.baimkey.GetKey()))
		return true;

	if (Options::Menu.RageBotTab.baiminair.GetState() && !(pEnt->GetFlags() & FL_ONGROUND))
		return true;

	int health = Options::Menu.RageBotTab.baimifhp.GetValue();

	if (health >= pEnt->GetHealth())
	{
		//BAIM
		return true;

	}



	return false;
}

Vector Aimbot::Hitscan(C_BaseEntity* pEnt) // supremeemmemememememe
{
	float DamageArray[28];
	float tempDmg = 0.f;
	Vector tempHitbox = { 0,0,0 };
	static int HitboxForMuti[] = { 2,2,4,4,6,6 };

	float angToLocal = g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pEnt->GetOrigin()).y;

	Vector2D MutipointXY = { (sin(g_Math.GRD_TO_BOG(angToLocal))),(cos(g_Math.GRD_TO_BOG(angToLocal))) };
	Vector2D MutipointXY180 = { (sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };
	Vector2D Mutipoint[] = { Vector2D(MutipointXY.x, MutipointXY.y), Vector2D(MutipointXY180.x, MutipointXY180.y) };

	float Velocity = abs(pEnt->GetVelocity().Length2D());

	//if (!Shonax.Aimbot.DelayShot && Velocity > 29.f)
		//Velocity = 30.f;

	std::vector<int> Scan;

	int HeadHeight = 0;

	bool Baim = ShouldBaim(pEnt);



	if (!Baim) // Head
		Scan.push_back(HITBOX_HEAD);

	if (Velocity <= 215.f || Baim)
	{

		Scan.push_back(HITBOX_PELVIS);
		Scan.push_back(HITBOX_THORAX);
		Scan.push_back(HITBOX_LOWER_CHEST);
		Scan.push_back(HITBOX_UPPER_CHEST);
		Scan.push_back(HITBOX_BELLY);
		Scan.push_back(HITBOX_NECK);
		Scan.push_back(HITBOX_RIGHT_CALF);
		Scan.push_back(HITBOX_LEFT_CALF);


		if (!Options::Menu.RageBotTab.rbot_limbsmoving.GetState())
			Velocity = 0.f;

		if (Velocity <= 29.f)
		{

			Scan.push_back(HITBOX_LEFT_FOOT);
			Scan.push_back(HITBOX_RIGHT_FOOT);
			Scan.push_back(HITBOX_LEFT_UPPER_ARM);
			Scan.push_back(HITBOX_RIGHT_UPPER_ARM);
			Scan.push_back(HITBOX_LEFT_THIGH);
			Scan.push_back(HITBOX_RIGHT_THIGH);

		}
	}

	Vector Hitbox;
	int bestHitboxint = 0;

	for (int hitbox : Scan)
	{
		if (hitbox < 19)
			Hitbox = pEnt->GetHitboxPosition(hitbox, Matrix[pEnt->EntIndex()]);
		else if (hitbox > 18 && hitbox < 25)
		{
			float Radius = 0;
			Hitbox = pEnt->GetHitboxPosition(HitboxForMuti[hitbox - 19], Matrix[pEnt->EntIndex()], &Radius);
			Radius *= (50 / 100.f);
			Hitbox = Vector(Hitbox.x + (Radius * Mutipoint[((hitbox - 19) % 2)].x), Hitbox.y - (Radius * Mutipoint[((hitbox - 19) % 2)].y), Hitbox.z);
		}
		else if (hitbox > 24 && hitbox < 28)
		{
			float Radius = 0;
			Hitbox = pEnt->GetHitboxPosition(0, Matrix[pEnt->EntIndex()], &Radius);
			Radius *= (HeadHeight / 100.f);
			if (hitbox != 27)
				Hitbox = Vector(Hitbox.x + (Radius * Mutipoint[((hitbox - 25) % 2)].x), Hitbox.y - (Radius * Mutipoint[((hitbox - 25) % 2)].y), Hitbox.z);
			else
				Hitbox += Vector(0, 0, Radius);
		}

		//float Damage = g_Autowall.Damage(Hitbox);
		float Damage = autowall->CanHit(Hitbox);

		if (Damage > 0.f)
			DamageArray[hitbox] = Damage;
		else
			DamageArray[hitbox] = 0;

		if (Options::Menu.RageBotTab.baimiflethal.GetState() && hitbox != 0 && hitbox != 25 && hitbox != 26 && hitbox != 27 && Damage >= (pEnt->GetHealth() + 10))
		{
			DamageArray[hitbox] = 400;
			Baim = true;
		}

		if (DamageArray[hitbox] > tempDmg)
		{
			tempHitbox = Hitbox;
			bestHitboxint = hitbox;
			tempDmg = DamageArray[hitbox];
		}

		g::AimbotHitbox[pEnt->EntIndex()][hitbox] = Hitbox;
	}



	PlayerRecords pPlayerEntityRecord;


	pPlayerEntityRecord = g_LagComp.PlayerRecord[pEnt->EntIndex()].at(0);

	Backtrack[pEnt->EntIndex()] = false;
	ShotBacktrack[pEnt->EntIndex()] = false;

	if (Options::Menu.RageBotTab.Backtrack.GetState() && g_LagComp.ShotTick[pEnt->EntIndex()] != -1 && autowall->CanHitFloatingPoint(pEnt->GetHitboxPosition(HITBOX_HEAD, g_LagComp.PlayerRecord[pEnt->EntIndex()].at(g_LagComp.ShotTick[pEnt->EntIndex()]).Matrix) + Vector(0, 0, 1), g::pLocalEntity->get_eye_pos()) && !Baim)
	{
		bestEntDmg = (1000000.f - fabs(g_Math.Distance(Vector2D(g::pLocalEntity->GetOrigin().x, g::pLocalEntity->GetOrigin().y), Vector2D(pEnt->GetOrigin().x, pEnt->GetOrigin().y)))); // just doing this to get the closest player im backtracking
		ShotBacktrack[pEnt->EntIndex()] = true;
		return pEnt->GetHitboxPosition(HITBOX_HEAD, g_LagComp.PlayerRecord[pEnt->EntIndex()].at(g_LagComp.ShotTick[pEnt->EntIndex()]).Matrix) + Vector(0, 0, 1);
	}
	else if (tempDmg >= Options::Menu.RageBotTab.damage_val.GetValue())
	{
		bestEntDmg = tempDmg;

		if ((bestHitboxint == 25 || bestHitboxint == 26 || bestHitboxint == 27) && abs(DamageArray[HITBOX_HEAD] - DamageArray[bestHitboxint]) <= 10.f)
			return pEnt->GetHitboxPosition(HITBOX_HEAD, Matrix[pEnt->EntIndex()]);
		else if ((bestHitboxint == 19 || bestHitboxint == 20) && DamageArray[HITBOX_PELVIS] > 30)
			return pEnt->GetHitboxPosition(HITBOX_PELVIS, Matrix[pEnt->EntIndex()]);
		else if ((bestHitboxint == 21 || bestHitboxint == 22) && DamageArray[HITBOX_THORAX] > 30)
			return pEnt->GetHitboxPosition(HITBOX_THORAX, Matrix[pEnt->EntIndex()]);
		else if ((bestHitboxint == 23 || bestHitboxint == 24) && DamageArray[HITBOX_UPPER_CHEST] > 30)
			return pEnt->GetHitboxPosition(HITBOX_UPPER_CHEST, Matrix[pEnt->EntIndex()]);

		return tempHitbox;
	}
	else if (Options::Menu.RageBotTab.Backtrack.GetState() && pPlayerEntityRecord.Velocity >= 29.f && autowall->CanHitFloatingPoint(pEnt->GetHitboxPosition(HITBOX_HEAD, pPlayerEntityRecord.Matrix), g::pLocalEntity->get_eye_pos()))
	{
		bestEntDmg = (100000.f - fabs(g_Math.Distance(Vector2D(g::pLocalEntity->GetOrigin().x, g::pLocalEntity->GetOrigin().y), Vector2D(pEnt->GetOrigin().x, pEnt->GetOrigin().y))));
		Backtrack[pEnt->EntIndex()] = true;
		return pEnt->GetHitboxPosition(HITBOX_HEAD, pPlayerEntityRecord.Matrix);
	}

	return Vector(0, 0, 0);
}
float accepted_inaccuracy(C_BaseCombatWeapon* weapon)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player) return 0;

	if (!weapon) return 0;
	if (weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_TASER) return 0;

	float inaccuracy = weapon->GetInaccuracy();
	if (inaccuracy == 0) inaccuracy = 0.0000001;
	inaccuracy = 1 / inaccuracy;
	return inaccuracy;
}

inline float Fast12Sqrt(float x)
{
	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;
	return *(float*)&i;
}
#define square( x ) ( x * x )
void quickstop(CUserCmd* cmd, float fMaxSpeed)
{
	if (fMaxSpeed <= 0.f)
		return;
	float fSpeed = (float)(Fast12Sqrt(square(g::pCmd->forwardmove) + square(g::pCmd->sidemove) + square(g::pCmd->upmove)));
	if (fSpeed <= 0.f)
		return;
	if (cmd->buttons & IN_DUCK)
		fMaxSpeed *= 2.94117647f;
	if (fSpeed <= fMaxSpeed)
		return;
	float fRatio = fMaxSpeed / fSpeed;
	g::pCmd->forwardmove *= fRatio;
	g::pCmd->sidemove *= fRatio;
	g::pCmd->upmove *= fRatio;
}



void Aimbot::AutoZeus()
{



}


void Aimbot::OnCreateMove()
{
	if (!g_pEngine->IsInGame())
		return;

	if (!Options::Menu.RageBotTab.AimbotEnable.GetState())
		return;
	Vector Aimpoint = { 0,0,0 };
	C_BaseEntity* Target = nullptr;

	int targetID = 0;
	int tempDmg = 0;
	static bool shot = false;
	auto weapon = g::pLocalEntity->GetActiveWeapon();

	for (int i = 1; i <= g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant())
		{
			g_LagComp.ClearRecords(i);
			continue;
		}

		g_LagComp.StoreRecord(pPlayerEntity);

		if (pPlayerEntity == g::pLocalEntity || pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		if (!pPlayerEntity->is_player())
			continue;
		if (pPlayerEntity->IsImmune())
			continue;


		if (g_LagComp.PlayerRecord[i].size() == 0 || !g::pLocalEntity->IsAlive() || !Options::Menu.RageBotTab.AimbotEnable.GetState())
			continue;

		if (!g::pLocalEntity->GetActiveWeapon() || g::pLocalEntity->IsKnifeorNade())
			continue;

		if (!weapon)
			continue;

		if (!g::pLocalEntity->GetTickBase())
			continue;

		if (weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_TASER)
			continue;

		if (weapon->is_grenade() || weapon->is_knife())
			return;

		if (weapon->GetAmmo() == 0)
			return;

		bestEntDmg = 0;

		Vector Hitbox = Hitscan(pPlayerEntity);

		if (Hitbox != Vector(0, 0, 0) && tempDmg <= bestEntDmg)
		{
			Aimpoint = Hitbox;
			Target = pPlayerEntity;
			targetID = Target->EntIndex();
			tempDmg = bestEntDmg;
		}


		if (!g::pLocalEntity->IsAlive())
		{
			shot = false;
			return;
		}

		if (!g::pLocalEntity->GetActiveWeapon() || g::pLocalEntity->IsKnifeorNade())
		{
			shot = false;
			return;
		}

		if (shot)
		{

			g::bSendPacket = true;
			g_AntiAim.OnCreateMove();

			shot = false;
		}

		float flServerTime = g::pLocalEntity->GetTickBase() * g_pGlobalVars->intervalPerTick;
		bool canShoot = (g::pLocalEntity->GetActiveWeapon()->GetNextPrimaryAttack() <= flServerTime);


		//	if (Options::Menu.RageBotTab.stop_bool.GetState())
			//	quickstop(g::pCmd, 1);

		if (Target)
		{
			if (g::pLocalEntity->GetVelocity().Length() >= (g::pLocalEntity->GetActiveWeapon()->GetCSWpnData()->max_speed_alt * .34f) - 5 && !GetAsyncKeyState(VK_SPACE) && Options::Menu.RageBotTab.stop_bool.GetState())
				quickstop(g::pCmd, 1);

			if (weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_AWP || weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_SSG08 ||
				weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_SCAR20 || weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_G3SG1 ||
				weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_AUG || weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_SG553)
				if (!g::pLocalEntity->IsScoped())
					g::pCmd->buttons |= IN_ATTACK2;


			g::TargetIndex = targetID;

			float SimulationTime = 0.f;

			if (Backtrack[targetID])
				SimulationTime = g_LagComp.PlayerRecord[targetID].at(0).SimTime;
			else
				SimulationTime = g_LagComp.PlayerRecord[targetID].at(g_LagComp.PlayerRecord[targetID].size() - 1).SimTime;

			if (ShotBacktrack[targetID])
				SimulationTime = g_LagComp.PlayerRecord[targetID].at(g_LagComp.ShotTick[targetID]).SimTime;

			Vector Angle = g_Math.CalcAngle(g::pLocalEntity->get_eye_pos(), Aimpoint);
			//m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_TASER







			if (accepted_inaccuracy(weapon) < Options::Menu.RageBotTab.chance_val.GetValue())
				continue;

			if (!(g::pCmd->buttons & IN_ATTACK) && canShoot)
			{

				if (!Backtrack[targetID] && !ShotBacktrack[targetID])
					g::Shot[targetID] = true;



				g::bSendPacket = true;
				shot = true;

				g::pCmd->viewangles = Angle -= (g::pLocalEntity->GetAimPunchAngle() * g_pCvar->FindVar("weapon_recoil_scale")->GetFloat());
				g::pCmd->buttons |= IN_ATTACK;
				g::pCmd->tick_count = TIME_TO_TICKS(SimulationTime + g_LagComp.LerpTime());
			}
		}
	}
}
*/
KnifeBot g_Knifebot;
void angleVectors(const Vector& angles, Vector& forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}



void KnifeBot::Run()
{
	if (g_Options.misc.knife_bot)
	{
		auto weploc = g::pLocalEntity->GetActiveWeapon();
		if (!weploc)
			return;
		if (weploc->IsKnife())
		{
			for (int y = 0; y <= 360; y += 360.f / 6.f)
			{
				for (int x = -89; x <= 89; x += 179.f / 6.f)
				{
					Vector ang = Vector(x, y, 0);
					Vector dir;
					angleVectors(ang, dir);
					C_Trace trace;


					UTIL_TraceLine(g::pLocalEntity->get_eye_pos(), g::pLocalEntity->get_eye_pos() + (dir * 64), MASK_SHOT, g::pLocalEntity, &trace);

					if (trace.m_pEnt == nullptr)
						continue;
					if (trace.m_pEnt == g::pLocalEntity)
						continue;
					if (!trace.m_pEnt->IsAlive())
						continue;
					if (!(trace.m_pEnt->GetHealth() > 0))
						continue;
					if (trace.m_pEnt->GetTeam() == g::pLocalEntity->GetTeam())
						continue;
					if (trace.m_pEnt->IsDormant())
						continue;
					if (trace.m_pEnt->IsImmune())
						continue;
					PlayerInfo_s info;
					if (!(g_pEngine->GetPlayerInfo(trace.m_pEnt->EntIndex(), &info)))
						continue;
					g::pCmd->viewangles = Vector(x, y, 0);
					g::pCmd->buttons |= IN_ATTACK2;
					return;
				}
			}

		}

	}

}
/*


float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
//--------------------------------------------------------------------------------
LineGoesThroughSmokeFn LineGoesThroughSmoke;

void AimLegit::Work(CUserCmd *usercmd)
{
	if (Options::Menu.LegitBotTab.AimbotEnable.GetState())
	{
		this->usercmd = usercmd;

		auto weploc = g::pLocalEntity->GetActiveWeapon();
		if (!weploc)
			return;


		if (weploc->GetAmmo() == 0)
			return;

		if (weploc->is_grenade() || weploc->is_knife())
			return;

		if ((usercmd->buttons & IN_ATTACK))
		{
			GetBestTarget();
			if (HasTarget())
				TargetRegion();
		}

		if (g_Options.total_total_l_r____cs)
			RecoilControlSystem();
	}
	
}

void AimLegit::GetBestTarget()
{
	float maxDistance = 8192.f;
	float nearest;
	int index = -1;
	float fov = g_Options.total_total_l_ejfouvykkk;
	int firedShots = g_LocalPlayer->m_iShotsFired();
	for (int i = 1; i < g_GlobalVars->maxClients; i++)
	{
		auto player = C_BasePlayer::GetPlayerByIndex(i);
		if (!CheckTarget(player))
			continue;
		QAngle viewangles = usercmd->viewangles;
		Vector targetpos;
		if (firedShots > g_Options.total_total_l_vymrdanejzasobnik)
			targetpos = player->GetBonePos(realAimSpot[g_Options.total_total_l_tdolkaa_hit_dolni_bejk]);
		else if (firedShots < g_Options.total_total_l_vymrdanejzasobnik)
			targetpos = player->GetBonePos(realAimSpot[g_Options.total_total_l_topka_hit_bejx]);
		nearest = Math::GetFov(viewangles, Math::CalcAngle(g_LocalPlayer->GetEyePos(), targetpos));
		if (nearest > fov)
			continue;
		float distance = Math::GetDistance(g_LocalPlayer->m_vecOrigin(), player->m_vecOrigin());
		if (fabsf(fov - nearest) < 5)
		{
			if (distance < maxDistance)
			{
				fov = nearest;
				maxDistance = distance;
				index = i;
			}
		}
		else if (nearest < fov)
		{
			fov = nearest;
			maxDistance = distance;
			index = i;
		}
	}
	SetTarget(index);
}
bool AimLegit::CheckTarget(C_BasePlayer *player)
{
	if (!player || player == nullptr)
		return false;
	if (player == g_LocalPlayer)
		return false;
	if (player->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
		return false;
	if (player->IsDormant())
		return false;
	if (player->m_bGunGameImmunity())
		return false;
	if (!player->IsAlive())
		return false;
	int firedShots = g_LocalPlayer->m_iShotsFired();
	Vector targetpos;
	CGameTrace tr;
	Ray_t ray;
	CTraceFilter filter;
	filter.pSkip = g_LocalPlayer;
	auto start = g_LocalPlayer->GetEyePos();
	if (firedShots > g_Options.total_total_l_vymrdanejzasobnik)
		targetpos = player->GetBonePos(realAimSpot[g_Options.total_total_l_tdolkaa_hit_dolni_bejk]);
	else if (firedShots < g_Options.total_total_l_vymrdanejzasobnik)
		targetpos = player->GetBonePos(realAimSpot[g_Options.total_total_l_topka_hit_bejx]);
	ray.Init(start, targetpos);
	g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);
	if (tr.hit_entity != player)
		return false;
	return true;
}
void AimLegit::TargetRegion()
{
	auto player = C_BasePlayer::GetPlayerByIndex(m_iTarget);
	int firedShots = g_LocalPlayer->m_iShotsFired();
	QAngle aimPunchAngle = g_LocalPlayer->m_aimPunchAngle();
	std::random_device r3nd0m;
	std::mt19937 r3nd0mGen(r3nd0m());
	std::uniform_real<float> r3nd0mXAngle(1.7f, 1.9f);
	std::uniform_real<float> r3nd0mYAngle(1.7f, 1.9f);
	aimPunchAngle.pitch *= r3nd0mXAngle(r3nd0mGen);
	aimPunchAngle.yaw *= r3nd0mYAngle(r3nd0mGen);
	aimPunchAngle.roll = 0.0f;

	Math::NormalizeAngles(aimPunchAngle);

	QAngle viewangles = usercmd->viewangles;
	Vector targetpos;
	if (firedShots > g_Options.total_total_l_vymrdanejzasobnik)
		targetpos = player->GetBonePos(realAimSpot[g_Options.total_total_l_tdolkaa_hit_dolni_bejk]);
	else if (firedShots < g_Options.total_total_l_vymrdanejzasobnik)
		targetpos = player->GetBonePos(realAimSpot[g_Options.total_total_l_topka_hit_bejx]);
	QAngle angle = Math::CalcAngle(g_LocalPlayer->GetEyePos(), targetpos);
	angle.pitch -= aimPunchAngle.pitch;
	angle.yaw -= aimPunchAngle.yaw;
	Math::SmoothAngle(viewangles, angle, g_Options.total_total_l_zmensimtitozatebe_sm_fct);
	Math::NormalizeAngles(angle);
	usercmd->viewangles = angle;
}

QAngle oldPunch;

void AimLegit::RecoilControlSystem()
{
	int firedShots = g_LocalPlayer->m_iShotsFired();
	if (usercmd->buttons & IN_ATTACK)
	{
		QAngle aimPunchAngle = g_LocalPlayer->m_aimPunchAngle();
		std::random_device r3nd0m;
		std::mt19937 r3nd0mGen(r3nd0m());
		std::uniform_real<float> r3nd0mXAngle(1.7f, 1.9f);
		std::uniform_real<float> r3nd0mYAngle(1.7f, 1.9f);
		aimPunchAngle.pitch *= r3nd0mXAngle(r3nd0mGen);
		aimPunchAngle.yaw *= r3nd0mYAngle(r3nd0mGen);
		aimPunchAngle.roll = 0.0f;
		Math::NormalizeAngles(aimPunchAngle);
		if (firedShots > 2)
		{
			QAngle viewangles = usercmd->viewangles;
			QAngle viewangles_mod = aimPunchAngle;
			viewangles_mod -= oldPunch;
			viewangles_mod.roll = 0.0f;
			Math::NormalizeAngles(viewangles_mod);
			viewangles -= viewangles_mod;
			viewangles.roll = 0.0f;
			Math::NormalizeAngles(viewangles);
			usercmd->viewangles = viewangles;
		}
		oldPunch = aimPunchAngle;
	}
	else
		oldPunch.Init();
}

*/

CLegitBot g_legitbot;

void CLegitBot::Init()
{
	IsLocked = false;
	TargetID = -1;
	HitBox = -1;
}

void CLegitBot::Draw()
{

}

static int CustomDelay = 0;
static int CustomBreak = 0;

void CLegitBot::Move(CUserCmd *pCmd, bool& bSendPacket)
{
	if (!g_Options.legit.enable)
		return;
	auto pWeapon = g::pLocalEntity->GetActiveWeapon();

	if (!pWeapon)
		return;
	
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player) 
		return;

	if (g_Options.legit.enable)
	{
		DoAimbot(pCmd, bSendPacket);

	}
	if (g_Options.legit.rcs_standalone)
		StandAloneRCS(local_player, pCmd, pWeapon);


	SyncWeaponSettings();
}



bool IsSniper(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_iItemDefinitionIndex();
	static const std::vector<int> v = { (int)ItemDefinitionIndex::WEAPON_AWP,(int)ItemDefinitionIndex::WEAPON_SSG08,(int)ItemDefinitionIndex::WEAPON_G3SG1,(int)ItemDefinitionIndex::WEAPON_SCAR20 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}


bool IsPisto44lboi(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_iItemDefinitionIndex();
	static const std::vector<int> v = { (int)ItemDefinitionIndex::WEAPON_DEAGLE,(int)ItemDefinitionIndex::WEAPON_CZ75,(int)ItemDefinitionIndex::WEAPON_ELITE,(int)ItemDefinitionIndex::WEAPON_USPS,(int)ItemDefinitionIndex::WEAPON_P250,(int)ItemDefinitionIndex::WEAPON_P2000, (int)ItemDefinitionIndex::WEAPON_TEC9,(int)ItemDefinitionIndex::WEAPON_REVOLVER,(int)ItemDefinitionIndex::WEAPON_FIVESEVEN,(int)ItemDefinitionIndex::WEAPON_GLOCK };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool Is34MP(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_iItemDefinitionIndex();
	static const std::vector<int> v = { (int)ItemDefinitionIndex::WEAPON_P90,(int)ItemDefinitionIndex::WEAPON_BIZON,(int)ItemDefinitionIndex::WEAPON_MP7,(int)ItemDefinitionIndex::WEAPON_MP9,(int)ItemDefinitionIndex::WEAPON_MAC10,(int)ItemDefinitionIndex::WEAPON_UMP45 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}


bool IsBallisticWeapon(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_iItemDefinitionIndex();
	return !(id >= (int)ItemDefinitionIndex::WEAPON_KNIFE && id <= (int)ItemDefinitionIndex::WEAPON_KNIFE_T || id == 0 || id >= (int)ItemDefinitionIndex::WEAPON_KNIFE_BAYONET);
}

bool IsRiflengaya(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_iItemDefinitionIndex();
	static const std::vector<int> v = { (int)ItemDefinitionIndex::WEAPON_AK47,(int)ItemDefinitionIndex::WEAPON_AUG,(int)ItemDefinitionIndex::WEAPON_FAMAS,(int)ItemDefinitionIndex::WEAPON_GALIL,(int)ItemDefinitionIndex::WEAPON_M249,(int)ItemDefinitionIndex::WEAPON_M4A4,(int)ItemDefinitionIndex::WEAPON_M4A1S,(int)ItemDefinitionIndex::WEAPON_NEGEV,(int)ItemDefinitionIndex::WEAPON_SG553 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

void Normaliz32324eVector(Vector& vec) {
	for (int i = 0; i < 3; ++i) {
		while (vec[i] > 180.f)
			vec[i] -= 360.f;

		while (vec[i] < -180.f)
			vec[i] += 360.f;
	}
	vec[2] = 0.f;
}
void ClampA23432ngles2(Vector& v)
{
	if (v.x > 89.0f && v.x <= 180.0f)
		v.x = 89.0f;
	if (v.x > 180.0f)
		v.x = v.x - 360.0f;
	if (v.x < -89.0f)
		v.x = -89.0f;
	v.y = fmodf(v.y + 180, 360) - 180;
	v.z = 0;
}
void CLegitBot::StandAloneRCS(C_BaseEntity* pLocal, CUserCmd* pCmd, C_BaseCombatWeapon* pWeapon)
{
	static Vector vOldPunch = { 0.0f, 0.0f, 0.0f };
	Vector vNewPunch = pLocal->GetAimPunchAngle();

	int amountxdxd = g_Options.legit.rcs_amount;


	vNewPunch *= amountxdxd;

	vNewPunch -= vOldPunch;
	vNewPunch *= 0.1f;
	vNewPunch += vOldPunch;

	Vector vFinal = pCmd->viewangles - (vNewPunch - vOldPunch);

	Normaliz32324eVector(vFinal);
	ClampA23432ngles2(vFinal);

	g_pEngine->SetViewAngles(vFinal);


	vOldPunch = vNewPunch;

}
void CLegitBot::SIMPLE(C_BaseCombatWeapon* pWeapon, C_BaseEntity* pLocal)
{
    if (IsPisto44lboi(pWeapon))
    {
        Speed = g_Options.legit.pistols.speed / 100;
        FoV = g_Options.legit.pistols.fov * 2;
        RecoilControl = g_Options.legit.pistols.rcs * 2;
        Inacc = g_Options.legit.pistols.smooth;



        switch (g_Options.legit.pistols.hitscan)
        {
        case 0:
            HitBox = (HITBOX_HEAD);
            Multihitbox = false;
            break;
        case 1:
            HitBox = (HITBOX_NECK);
            Multihitbox = false;
            break;
        case 2:
            HitBox = (HITBOX_BELLY);
            Multihitbox = false;
            break;
        case 3:
            HitBox = (HITBOX_LOWER_CHEST);
            Multihitbox = false;
            break;
        case 4:
            Multihitbox = true;
            break;
        }

    }
    else if (IsRiflengaya(pWeapon))
    {
        Speed = g_Options.legit.rifles.speed / 100;
        FoV = g_Options.legit.rifles.fov * 2;
        RecoilControl = g_Options.legit.rifles.rcs * 2;
        Inacc = g_Options.legit.rifles.smooth;


        switch (g_Options.legit.rifles.hitscan)
        {
        case 0:
            HitBox = (HITBOX_HEAD);
            Multihitbox = false;
            break;
        case 1:
            HitBox = (HITBOX_NECK);
            Multihitbox = false;
            break;
        case 2:
            HitBox = (HITBOX_BELLY);
            Multihitbox = false;
            break;
        case 3:
            HitBox = (HITBOX_LOWER_CHEST);
            Multihitbox = false;
            break;
        case 4:
            Multihitbox = true;
            break;
        }
    }

    else if (IsSniper(pWeapon))
    {
        Speed = g_Options.legit.snipers.speed / 100;
        FoV = g_Options.legit.snipers.fov * 2;
        RecoilControl = g_Options.legit.snipers.rcs * 2;
        Inacc = g_Options.legit.snipers.smooth;


        switch (g_Options.legit.snipers.hitscan)
        {
        case 0:
            HitBox = (HITBOX_HEAD);
            Multihitbox = false;
            break;
        case 1:
            HitBox = (HITBOX_NECK);
            Multihitbox = false;
            break;
        case 2:
            HitBox = (HITBOX_BELLY);
            Multihitbox = false;
            break;
        case 3:
            HitBox = (HITBOX_LOWER_CHEST);
            Multihitbox = false;
            break;
        case 4:
            Multihitbox = true;
            break;
        }

    }
    else if (pLocal->IsMachinegun(pWeapon))
    {
        Speed = g_Options.legit.sub.speed / 100;
        FoV = g_Options.legit.sub.fov * 2;
        RecoilControl = g_Options.legit.sub.rcs * 2;
        Inacc = g_Options.legit.sub.smooth;


        switch (g_Options.legit.sub.hitscan)
        {
        case 0:
            HitBox = (HITBOX_HEAD);
            Multihitbox = false;
            break;
        case 1:
            HitBox = (HITBOX_NECK);
            Multihitbox = false;
            break;
        case 2:
            HitBox = (HITBOX_BELLY);
            Multihitbox = false;
            break;
        case 3:
            HitBox = (HITBOX_LOWER_CHEST);
            Multihitbox = false;
            break;
        case 4:
            Multihitbox = true;
            break;
        }

    }
    else if (pLocal->IsShotgun(pWeapon))
    {
        Speed = g_Options.legit.shotguns.speed / 100;
        FoV = g_Options.legit.shotguns.fov * 2;
        RecoilControl = g_Options.legit.shotguns.rcs * 2;
        Inacc = g_Options.legit.shotguns.smooth;


        switch (g_Options.legit.shotguns.hitscan)
        {
        case 0:
            HitBox = (HITBOX_HEAD);
            Multihitbox = false;
            break;
        case 1:
            HitBox = (HITBOX_NECK);
            Multihitbox = false;
            break;
        case 2:
            HitBox = (HITBOX_BELLY);
            Multihitbox = false;
            break;
        case 3:
            HitBox = (HITBOX_LOWER_CHEST);
            Multihitbox = false;
            break;
        case 4:
            Multihitbox = true;
            break;
        }

    }
    else if (Is34MP(pWeapon))
    {
        Speed = g_Options.legit.heavys.speed / 100 ;
        FoV = g_Options.legit.heavys.fov * 2;
        RecoilControl = g_Options.legit.heavys.rcs * 2;
        Inacc = g_Options.legit.heavys.smooth;


        switch (g_Options.legit.heavys.hitscan)
        {
        case 0:
            HitBox = (HITBOX_HEAD);
            Multihitbox = false;
            break;
        case 1:
            HitBox = (HITBOX_NECK);
            Multihitbox = false;
            break;
        case 2:
            HitBox = (HITBOX_BELLY);
            Multihitbox = false;
            break;
        case 3:
            HitBox = (HITBOX_LOWER_CHEST);
            Multihitbox = false;
            break;
        case 4:
            Multihitbox = true;
            break;
        }

    }
}

void CLegitBot::ADVANCED(void* weapon)
{
    C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)weapon;
    int id = *pWeapon->m_iItemDefinitionIndex();
    Speed = g_Options.advanced_legit[id].speed / 100;
    FoV = g_Options.advanced_legit[id].fov * 2;
    RecoilControl = g_Options.advanced_legit[id].rcs * 2;
    Inacc = g_Options.advanced_legit[id].smooth;


    switch (g_Options.advanced_legit[id].hitscan)
    {
    case 0:
        HitBox = (HITBOX_HEAD);
        Multihitbox = false;
        break;
    case 1:
        HitBox = (HITBOX_NECK);
        Multihitbox = false;
        break;
    case 2:
        HitBox = (HITBOX_BELLY);
        Multihitbox = false;
        break;
    case 3:
        HitBox = (HITBOX_LOWER_CHEST);
        Multihitbox = false;
        break;
    case 4:
        Multihitbox = true;
        break;
    }
}
void air_shot(void* weapon)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!g_Options.legit.air_shot || !local_player)
		return;
	CUserCmd* user_cmd;
	//if (local_player->GetMoveType() == movetype_ladder || local_player->GetMoveType() == movetype_noclip)
		//return;
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_iItemDefinitionIndex();
	
	if (!(g::pLocalEntity->GetFlags() & FL_ONGROUND) && id == (int)ItemDefinitionIndex::WEAPON_AWP || !(g::pLocalEntity->GetFlags() & FL_ONGROUND) && id == (int)ItemDefinitionIndex::WEAPON_SSG08) {
		g_legitbot.issilent = true;
		user_cmd->buttons &= ~IN_DUCK;
		//issniper = true;
	}
	else {
		g_legitbot.issilent = false;
		user_cmd->buttons &= ~IN_DUCK;
		//issniper = false;
	}
}
void CLegitBot::SyncWeaponSettings()
{
	C_BaseEntity* pLocal = g::pLocalEntity;
	auto pWeapon = g::pLocalEntity->GetActiveWeapon();

	if (!pWeapon)
		return;
	
    switch(g_Options.legit.legit_type)
    {
        case 0:
            SIMPLE(pWeapon, pLocal);
            break;
        case 1:
            ADVANCED(pWeapon);
            break;
    }
	air_shot(pWeapon);
	if (issilent)
	{
		Speed = 1000 / 100;
		FoV = 100 * 2;
	}
	
}

Vector GetHitboxPosition(C_BaseEntity* pEntity, int Hitbox)
{
	matrix3x4_t matrix[128];

	if (pEntity->SetupBones(matrix, 128, 0x100, 0.f))
	{
		studiohdr_t* hdr = g_pModelInfo->GetStudiomodel(pEntity->GetModel());
		if (hdr)
		{
			mstudiohitboxset_t* set = hdr->GetHitboxSet(0);
			if (set)
			{
				mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

				if (hitbox)
				{
					Vector vMin, vMax, vCenter, sCenter;
					g_Math.VectorTransform(hitbox->min, matrix[hitbox->bone], vMin);
					g_Math.VectorTransform(hitbox->max, matrix[hitbox->bone], vMax);

					vCenter = ((vMin + vMax) *0.5f);

					return vCenter;
				}
			}
			

		}
		
	}

	return Vector(0, 0, 0);
}


void CLegitBot::DoAimbot(CUserCmd *pCmd, bool &bSendPacket)
{
	C_BaseEntity* pTarget = nullptr;
	C_BaseEntity* pLocal = g::pLocalEntity;
	bool FindNewTarget = true;
	auto pWeapon = g::pLocalEntity->GetActiveWeapon();

	if (g_Options.legit.flash_check && pLocal->IsFlashed())
		return;
	bool OnGround = (pLocal->GetFlags() & FL_ONGROUND);
	if (g_Options.legit.jump_check && !OnGround)
		return;

	if (pWeapon)
	{
		if (pWeapon->GetAmmo() == 0 || !IsBallisticWeapon(pWeapon))
		{
			return;
		}
		SyncWeaponSettings();

	}
	else
		return;

	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{
		pTarget = g_pEntityList->GetClientEntity(TargetID);
		if (pTarget  && TargetMeetsRequirements(pTarget))
		{
			SyncWeaponSettings();
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View; g_pEngine->GetViewAngles(View);
				if (pLocal->GetVelocity().Length() > 45.f);
				View += pLocal->GetAimPunchAngle() * RecoilControl;
				float nFoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (nFoV < FoV)
					FindNewTarget = false;
			}
		}
	}

	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;

		TargetID = GetTargetCrosshair();

		if (TargetID >= 0)
		{
			pTarget = g_pEntityList->GetClientEntity(TargetID);
		}
		else
		{
			pTarget = nullptr;
			HitBox = -1;
		}
	}

	SyncWeaponSettings();
	
	if (TargetID >= 0 && pTarget)
	{
		SyncWeaponSettings();

		if (g_Options.legit.on_key)
		{
			int Key = g_Options.legit.key;
			if (Key >= 0 && !GetAsyncKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}

		Vector AimPoint;

		if (Multihitbox)
		{
			AimPoint = GetHitboxPosition(pTarget, besthitbox);
		}
		else
		{
			AimPoint = GetHitboxPosition(pTarget, HitBox);
		}
		
		float damage = g_Autowall.Damage(AimPoint);
		//AmAtPoint(pLocal, AimPoint, pCmd, bSendPacket);
		if (IsPisto44lboi(pWeapon))
		{
			if (damage >= g_Options.legit.pistols.min_dmg)
			{
				if (AimAtPoint(pLocal, AimPoint, pCmd, bSendPacket))
				{
					
				}
			}
		}
		else if (IsRiflengaya(pWeapon))
		{
			if (damage >= g_Options.legit.rifles.min_dmg)
			{
				if (AimAtPoint(pLocal, AimPoint, pCmd, bSendPacket))
				{
				
				}
			}
		}

		else if (IsSniper(pWeapon))
		{
			if (damage >= g_Options.legit.snipers.min_dmg)
			{
				if (AimAtPoint(pLocal, AimPoint, pCmd, bSendPacket))
				{
					
				}
			}
		}
		else if (pLocal->IsMachinegun(pWeapon))
		{

			if (damage >= g_Options.legit.sub.min_dmg)
			{
				if (AimAtPoint(pLocal, AimPoint, pCmd, bSendPacket))
				{
					
				}
			}
		}
		else if (pLocal->IsShotgun(pWeapon))
		{

			if (damage >= g_Options.legit.shotguns.min_dmg)
			{
				if (AimAtPoint(pLocal, AimPoint, pCmd, bSendPacket))
				{
					
				}
			}
		}
		else if (Is34MP(pWeapon))
		{


			if (damage >= g_Options.legit.heavys.min_dmg)
			{
				if (AimAtPoint(pLocal, AimPoint, pCmd, bSendPacket))
				{
					
				}
			}
		}
		else
		{
				if (AimAtPoint(pLocal, AimPoint, pCmd, bSendPacket))
				{
					
				}
			
		}
	
	}
	
	

}



void AngleV23ectors(const Vector &angles, Vector *forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}




bool CLegitBot::TargetMeetsRequirements(C_BaseEntity* pEntity)
{
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->EntIndex() != g::pLocalEntity->EntIndex())
	{
		ClientClass *pClientClass = pEntity->GetClientClass();
		PlayerInfo_t pinfo;
		if (pEntity->is_player() && g_pEngine->GetPlayerInfo(pEntity->EntIndex(), &pinfo))
		{
			if (pEntity->GetTeam() != g::pLocalEntity->GetTeam())
			{
				/*
				if (Options::Menu.LegitBotTab.AimbotSmokeCheck.GetState())
				{
					typedef bool(__cdecl* GoesThroughSmoke)(Vector, Vector);

					static uint32_t GoesThroughSmokeOffset = (uint32_t)Utilities::Memory::FindPatternV2("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
					static GoesThroughSmoke GoesThroughSmokeFunction = (GoesThroughSmoke)GoesThroughSmokeOffset;

					if (GoesThroughSmokeFunction(hackManager.pLocal()->get_eye_pos(), pEntity->GetBonePos(8)))
						return false;
				}*/

				if (g_Options.legit.smoke_check)
				{
					typedef bool(__cdecl* GoesThroughSmoke)(Vector, Vector);

					static uint32_t GoesThroughSmokeOffset = (uint32_t)Utils::FindSignature("client_panorama.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
					static GoesThroughSmoke GoesThroughSmokeFunction = (GoesThroughSmoke)GoesThroughSmokeOffset;


					if (GoesThroughSmokeFunction(g::pLocalEntity->get_eye_pos(), pEntity->GetBonePos(8)))
						return false;
				}

				if (!pEntity->IsImmune())
				{
					return true;
				}

			}
		}
	}
	return false;
}

float Get3dDistance(Vector me, Vector ent)
{
	return sqrt(pow(double(ent.x - me.x), 2.0) + pow(double(ent.y - me.y), 2.0) + pow(double(ent.z - me.z), 2.0));
}

void Norma232lize(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}


float CLegitBot::FovToPlayer(Vector ViewOffSet, Vector View, C_BaseEntity* pEntity, int aHitBox)
{
	CONST FLOAT MaxDegrees = 180.0f;

	Vector Angles = View;

	Vector Origin = ViewOffSet;

	Vector Delta(0, 0, 0);

	Vector Forward(0, 0, 0);

	AngleV23ectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);

	VectorSubtract(AimPos, Origin, Delta);

	Norma232lize(Delta, Delta);

	float Distance = Get3dDistance(Origin, AimPos);

	float pitch = sin(Forward.x - Delta.x) * Distance;
	float yaw = sin(Forward.y - Delta.y) * Distance;
	float zaw = sin(Forward.z - Delta.z) * Distance;

	float mag = sqrt((pitch*pitch) + (yaw*yaw) + (zaw*zaw));
	return mag;
}
/*
float CLegitBot::FovToPlayer(Vector ViewOffSet, Vector View, C_BaseEntity* pEntity, int aHitBox)
{
	Vector out[9];

	// Anything past 180 degrees is just going to wrap around
	CONST FLOAT MaxDegrees = 180.0f;

	// Get local angles
	Vector Angles = View;

	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	//Vector Origin(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);
	// Get delta vector between our local eye position and passed vector
	VectorSubtract(AimPos, Origin, Delta);
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	Normalize(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / PI));
}
*/

int CLegitBot::GetTargetCrosshair()
{
	// Target selection
	SyncWeaponSettings();
	int target = -1;
	float minFoV = FoV;

	C_BaseEntity* pLocal = g::pLocalEntity;
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; g_pEngine->GetViewAngles(View);
	View += pLocal->GetAimPunchAngle() * 2.f;

	for (int i = 0; i < g_pEntityList->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity *pEntity = g_pEntityList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			if (Multihitbox)
			{

				float fov1 = FovToPlayer(ViewOffset, View, pEntity, 0);
				float fov2 = FovToPlayer(ViewOffset, View, pEntity, 4);
				float fov3 = FovToPlayer(ViewOffset, View, pEntity, 6);

				if (fov1 < FoV || fov2 < FoV && fov1 < FoV || fov3 < FoV)
				{
					FoV = fov1;
					target = i;
					besthitbox = 0;
				}

				if (fov2 < FoV || fov1 < FoV && fov2 < FoV || fov3 < FoV)
				{
					FoV = fov2;
					target = i;
					besthitbox = 4;
				}

				if (fov3 < FoV || fov1 < FoV && fov3 < FoV || fov2 < FoV)
				{
					FoV = fov3;
					target = i;
					besthitbox = 6;
				}

			}
			else
			{

				int NewHitBox = HitBox;
				if (NewHitBox >= 0)
				{
					float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
					if (fov < minFoV)
					{
						minFoV = fov;
						target = i;
					}
				}
			}
		}
	}

	return target;
}



void CalcA323ngle(Vector src, Vector dst, Vector &angles)
{
	Vector delta = src - dst;
	double hyp = delta.Length2D(); //delta.Length
	angles.y = (atan(delta.y / delta.x) * 57.295779513082f);
	angles.x = (atan(delta.z / hyp) * 57.295779513082f);
	angles[2] = 0.00;

	if (delta.x >= 0.0)
		angles.y += 180.0f;

}

void Normali44seViewAngle(Vector &angle)
{

	while (angle.y <= -180) angle.y += 360;
	while (angle.y > 180) angle.y -= 360;
	while (angle.x <= -180) angle.x += 360;
	while (angle.x > 180) angle.x -= 360;


	if (angle.x > 89) angle.x = 89;
	if (angle.x < -89) angle.x = -89;
	if (angle.y < -180) angle.y = -179.999;
	if (angle.y > 180) angle.y = 179.999;

	angle.z = 0;

}

bool CLegitBot::AimAtPoint(C_BaseEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket)
{
	// Get the full angles
	if (point.Length() == 0) return false;

	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	static Vector Inaccuracy;

	if (timeSoFar > 0.2)
	{
		Inaccuracy.Init(-50 + rand() % 100, -50 + rand() % 100, -50 + rand() % 100);
		Inaccuracy.NormalizeInPlace();
		Inaccuracy *= Inacc;
		start_t = clock();
	}

	point += Inaccuracy;
	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	CalcA323ngle(src, point, angles);
	Normali44seViewAngle(angles);

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return false;
	}

	if (RecoilControl)
	{
		Vector AimPunch = pLocal->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			angles -= AimPunch * 2;
			Normali44seViewAngle(angles);
		}
	}


	IsLocked = true;

	Vector shit = angles - pCmd->viewangles;
	bool v = false;
	Normali44seViewAngle(shit);
	if (shit.Length() > Speed)
	{
		Norma232lize(shit, shit);
		shit *= Speed;
	}
	else
	{
		v = true;
	}

	
	
	if(issilent && g_Options.legit.silentaim || !issilent && g_Options.legit.silentaim || issilent && !g_Options.legit.silentaim)
	{
		pCmd->viewangles += shit;

	}
	else
	{
		pCmd->viewangles += shit;
		g_pEngine->SetViewAngles(pCmd->viewangles);

	}



	return v;


}



CTriggerBot TriggerBot;

void CTriggerBot::Move(CUserCmd *pCmd)
{
	if (g_Options.legit.trigger.enable)
		DoTrigger(pCmd);
}



Vector C_BaseEntity::GetHitboxPositionxd(int Hitbox)
{
	matrix3x4_t bone_matrix[128];

	if (this->SetupBones(bone_matrix, 128, 0x00000100, 0.0f)) {

		auto studio_model = g_pModelInfo->GetStudiomodel(this->GetModel());

		if (studio_model) {

			auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(Hitbox);

			if (hitbox) {

				auto min = Vector{}, max = Vector{};

				g_Math.VectorTransform(hitbox->min, bone_matrix[hitbox->bone], min);
				g_Math.VectorTransform(hitbox->max, bone_matrix[hitbox->bone], max);

				return Vector((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
			}
		}
	}
	return Vector{};
}


bool CTriggerBot::Trigger_HitChance(C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance)
{
	if (chance == 0)
		return true;

	float Seeds = 256.f;

	Angle -= (g::pLocalEntity->GetAimPunchAngle() * g_pCvar->FindVar("weapon_recoil_scale")->GetFloat());

	Vector forward, right, up;

	g_Math.AngleVectors(Angle, &forward, &right, &up);

	int Hits = 0, neededHits = (Seeds * (chance / 100.f));

	float weapSpread = pWeapon->GetSpread(), weapInaccuracy = pWeapon->GetInaccuracy();

	for (int i = 0; i < Seeds; i++)
	{
		float Inaccuracy = g_Math.RandomFloat(0.f, 1.f) * weapInaccuracy;
		float Spread = g_Math.RandomFloat(0.f, 1.f) * weapSpread;

		Vector spreadView((cos(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (cos(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Spread), (sin(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Inaccuracy) + (sin(g_Math.RandomFloat(0.f, 2.f * M_PI)) * Spread), 0), direction;
		direction = Vector(forward.x + (spreadView.x * right.x) + (spreadView.y * up.x), forward.y + (spreadView.x * right.y) + (spreadView.y * up.y), forward.z + (spreadView.x * right.z) + (spreadView.y * up.z)).Normalize();

		Vector viewanglesSpread, viewForward;

		g_Math.VectorAngles(direction, up, viewanglesSpread);
		g_Math.NormalizeAngles(viewanglesSpread);

		g_Math.AngleVectors(viewanglesSpread, &viewForward);
		viewForward.NormalizeInPlace();

		viewForward = g::pLocalEntity->get_eye_pos() + (viewForward * pWeapon->GetCSWpnData()->range);

		C_Trace Trace;

		g_pTrace->ClipRayToEntity(C_Ray(g::pLocalEntity->get_eye_pos(), viewForward), mask_shot | contents_grate, pEnt, &Trace);

		if (Trace.m_pEnt == pEnt)
			Hits++;

		if (((Hits / Seeds) * 100.f) >= chance)
			return true;

		if ((Seeds - i + Hits) < neededHits)
			return false;
	}

	return false;
}


void CTriggerBot::DoTrigger(CUserCmd *pCmd)
{


	static size_t tDelay = 0;

	C_BaseEntity* LocalPlayer = g::pLocalEntity;
	if (!LocalPlayer) return;
	auto LocalPlayerWeapon = LocalPlayer->GetActiveWeapon();
	auto WeaponEntity = (IClientEntity*)LocalPlayerWeapon;

	if (LocalPlayerWeapon) {
		if (LocalPlayerWeapon->GetAmmo() == 0)
			return;

		auto ClientClass = WeaponEntity->GetClientClass();
		if (ClientClass->ClassID == (int)ClassId::CKnife ||
			ClientClass->ClassID == (int)ClassId::CHEGrenade ||
			ClientClass->ClassID == (int)ClassId::CDecoyGrenade ||
			ClientClass->ClassID == (int)ClassId::CIncendiaryGrenade ||
			ClientClass->ClassID == (int)ClassId::CSmokeGrenade ||
			ClientClass->ClassID == (int)ClassId::CC4) {
			return;
		}
	}
	else
		return;

	Vector ViewAngles;
	g_pEngine->GetViewAngles(ViewAngles);

	Vector CrosshairForward;
	g_Math.AngleVectors(ViewAngles, &CrosshairForward);
	CrosshairForward *= 8000.f;


	Vector TraceSource = LocalPlayer->get_eye_pos();
	Vector TraceDestination = TraceSource + CrosshairForward;


	C_Trace Trace;
	CTraceEntity Filter(LocalPlayer);

	Filter.pSkip1 = LocalPlayer;

	g_pTrace->TraceRay(C_Ray(TraceSource, TraceDestination), MASK_SHOT, &Filter, &Trace);

	if (!Trace.m_pEnt)
		return;
	if (!Trace.m_pEnt->IsAlive())
		return;
	if (Trace.m_pEnt->GetHealth() <= 0)
		return;

	if (!g_Options.legit.trigger.deathmatch)
	{
		if (LocalPlayer->GetTeam() == Trace.m_pEnt->GetTeam())
			return;
	}

	if (g_Options.legit.trigger.smoke_check)
	{
		typedef bool(__cdecl* GoesThroughSmoke)(Vector, Vector);

		static uint32_t GoesThroughSmokeOffset = (uint32_t)Utils::FindSignature("client_panorama.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
		static GoesThroughSmoke GoesThroughSmokeFunction = (GoesThroughSmoke)GoesThroughSmokeOffset;


		if (GoesThroughSmokeFunction(g::pLocalEntity->get_eye_pos(), Trace.m_pEnt->GetHitboxPositionxd(8)))
			return;
	}
	auto Trigger_Hitbox_list = g_Options.legit.trigger.trigger_hitscan_bool;

	if (g_Options.legit.trigger.on_key)
	{
        /*if (g_Options.legit.trigger.method == 0 && g_Options.legit.trigger.internal)
        {
            Trace.m_pEnt = nullptr;
            Trace.hitGroup = -1;
            return;
        }
        else if (g_Options.legit.trigger.method == 1 && !GetAsyncKeyState(g_Options.legit.trigger.key))
        {
            Trace.m_pEnt = nullptr;
            Trace.hitGroup = -1;
            return;
        }*/

		if ((g_Options.legit.trigger.method == 1 && !GetAsyncKeyState(g_Options.legit.trigger.key)) || (g_Options.legit.trigger.method == 0 && overridething_legit))
		{
			Trace.m_pEnt = nullptr;
			Trace.hitGroup = -1;
			return;
		}
	}


	if (g_Options.legit.trigger.trigger_delay > 0)
	{
		if (GetTickCount() > tDelay)
		{
			if ((Trigger_Hitbox_list[0] && Trace.hitGroup == HITGROUP_HEAD) ||
				(Trigger_Hitbox_list[1] && Trace.hitGroup == HITGROUP_CHEST) ||
				(Trigger_Hitbox_list[2] && Trace.hitGroup == HITGROUP_STOMACH) ||
				(Trigger_Hitbox_list[3] && (Trace.hitGroup == HITGROUP_LEFTARM || Trace.hitGroup == HITGROUP_RIGHTARM)) ||
				(Trigger_Hitbox_list[4] && (Trace.hitGroup == HITGROUP_LEFTLEG || Trace.hitGroup == HITGROUP_RIGHTLEG))) {

				float damage = g_Autowall.Damage(Trace.m_pEnt->GetHitboxPositionxd(Trace.hitGroup));

				if (damage >= g_Options.legit.trigger.trigger_min)
				{
					Vector Angle = g_Math.CalcAngle(g::pLocalEntity->get_eye_pos(), Trace.m_pEnt->GetHitboxPositionxd(Trace.hitGroup));

					if (Trigger_HitChance(Trace.m_pEnt, g::pLocalEntity->GetActiveWeapon(), Angle, Trace.m_pEnt->GetHitboxPositionxd(Trace.hitGroup), g_Options.legit.trigger.trigger_min_chance))
					{
						pCmd->buttons |= IN_ATTACK;

					}
				}
			}

			tDelay = GetTickCount() + g_Options.legit.trigger.trigger_delay;
		}
	}
	else
	{
		if ((Trigger_Hitbox_list[0] && Trace.hitGroup == HITGROUP_HEAD) ||
			(Trigger_Hitbox_list[1] && Trace.hitGroup == HITGROUP_CHEST) ||
			(Trigger_Hitbox_list[2] && Trace.hitGroup == HITGROUP_STOMACH) ||
			(Trigger_Hitbox_list[3] && (Trace.hitGroup == HITGROUP_LEFTARM || Trace.hitGroup == HITGROUP_RIGHTARM)) ||
			(Trigger_Hitbox_list[4] && (Trace.hitGroup == HITGROUP_LEFTLEG || Trace.hitGroup == HITGROUP_RIGHTLEG))) {

			float damage = g_Autowall.Damage(Trace.m_pEnt->GetHitboxPositionxd(Trace.hitGroup));

			if (damage >= g_Options.legit.trigger.trigger_min)
			{
				Vector Angle = g_Math.CalcAngle(g::pLocalEntity->get_eye_pos(), Trace.m_pEnt->GetHitboxPositionxd(Trace.hitGroup));

				if (Trigger_HitChance(Trace.m_pEnt, g::pLocalEntity->GetActiveWeapon(), Angle, Trace.m_pEnt->GetHitboxPositionxd(Trace.hitGroup), g_Options.legit.trigger.trigger_min_chance))
				{
					pCmd->buttons |= IN_ATTACK;

				}
			}
		}
	}



	
	

	
}
