#include "AntiAim.h"
#include "..\Aimbot\Autowall.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\Utils\Math.h"
#include "..\..\newMenu.h"
#include "..\..\SDK\IVModelInfo.h"
#include "assert.h"
#include "..\..\SDK\ICvar.h"
#include "..\..\Features/rankchanger/IMemAlloc.h"
#include "algorithm"
#include "..\Aimbot\Aimbot.h"
#include "../../Options.hpp"

enum MovingAAType {
	Standing = 0,
	Moving,
	Air,
	Slowwalk

};



int ReturnMovementType()
{
	if (g::pLocalEntity->GetFlags() & FL_ONGROUND)
	{

		if (g::pLocalEntity->GetVelocity().Length2D() < 32 && !g::fake_walk)
		{
			return MovingAAType::Standing;
		}
		else if (g::pLocalEntity->GetVelocity().Length2D() > 32 && !g::fake_walk)
		{
			return MovingAAType::Moving;
		}
		else if (g::fake_walk)
		{
			return MovingAAType::Slowwalk;

		}
	}
	else
	{
		return MovingAAType::Air;
	}
}
#define CONTENTS_SOLID			0x1		// an eye is never valid in a solid
#define CONTENTS_MOVEABLE		0x4000
#define CONTENTS_WINDOW			0x2		// translucent, but not watery (glass)
#define CONTENTS_DEBRIS			0x4000000
#define MASK_ALL				(0xFFFFFFFF)
#define	CONTENTS_MONSTER		0x2000000	// should never be on a brush, only in game
#define CONTENTS_HITBOX			0x40000000	// use accurate hitboxes on trace
#define	MASK_SHOT					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)

#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)

#define TIME_TO_TICKS(dt) ((int)( 0.5f + (float)(dt) / g_pGlobalVars->intervalPerTick))

#define TICKS_TO_TIME(t) (g_pGlobalVars->intervalPerTick * (t))
#define ticks_to_time(t) (g_pGlobalVars->intervalPerTick * (t))

antiaim AntiAimNEW;
c_antiaim AntiAimPro;



animation_state AnimStateNew;

bool side2 = false;
bool side1 = false;

float server_timeGAY()
{
	static int tick = 0;
	static CUserCmd* last_command;

	if (!g::pCmd)
		return tick * g_pGlobalVars->intervalPerTick;

	if (!last_command || last_command->hasbeenpredicted)
		tick = g::pLocalEntity->GetTickBase();
	else
		tick++;

	last_command = g::pCmd;
}


float spawn_time = 0.0f;
CBaseHandle* entity_handle = nullptr;
C_AnimState* server_animation_state = nullptr;

void animation_state::create_move() {
	bool
		allocate = (server_animation_state == nullptr),
		change = (!allocate) && (&g::pLocalEntity->GetRefEHandle() != entity_handle),
		reset = (!allocate && !change) && (g::pLocalEntity->m_flSpawnTime() != spawn_time);

	if (change)
		g_pMemAlloc->Free(server_animation_state);

	if (reset) {
		reset_state(server_animation_state);
		spawn_time = g::pLocalEntity->m_flSpawnTime();
	}

	if (allocate || change) {
		auto state = (C_AnimState*)g_pMemAlloc->Alloc(sizeof(C_AnimState));

		if (state != nullptr)
			create_state(state, g::pLocalEntity);

		entity_handle = const_cast<CBaseHandle*>(&g::pLocalEntity->GetRefEHandle());
		spawn_time = g::pLocalEntity->m_flSpawnTime();

		server_animation_state = state;
	}

	float server_time = server_timeGAY();
	if (!g_pClientState->choked_commands)
	{
		float pose_parameters[24];
		AnimationLayer anim_layers[13];

		memcpy(pose_parameters, g::pLocalEntity->m_flPoseParameter(), sizeof(float) * 24);
		memcpy(anim_layers, g::pLocalEntity->GetAnimOverlays(), sizeof(AnimationLayer) * 13);

		update_state(server_animation_state, g::RealAngle); // this is what causes the legs to be fucked, but i know you guys are retarded enough not to realise OR CARE about it, so im leaving this comment here as a reminder that when im gonna leak this source everyone will realise how stupid you guys are and how you cant solve the most basic shit without breaking even mroe shit. hf crying under your bed you dumb """cheat developers""", i feel bad for whoever bought this cheat, especially the ones that paid $130 for it. talking about money, i dont want to get deep into this because its currently sunday morning 4 am, but either way, dusk/ecralion has told me that he will pay me at the end of the week (2 day late btw, and he hasnt even apologised / brung it up to me), which he for obvious reasons didnt do. one guy purchased the cheat and dusk never even bothered to add him to the users role on our discord (that he deleted lol), i assume because he didnt want me knowing a guy purchased the cheat, and didnt want to pay me. after i asked him if he bought the cheat, then he made him a user on the server. im really sorry for victims that fell for this terrible cheat, i'd do something if i got the money(refund or something), but unfortuantely i didnt get any money because the greedy dickheads kept it for themselves and used me to make 'their' cheat better. this is directed at "ecralion" on uc, and jeffrey which idk his uc name, but yeah, i am signing out as the 12 year old child aka alpha/"524245534435344", which is smarter than 15 year old dusk/ecralion. i dont give the slightest fuck if my rep gets ruined by this, but im not going to be forced to work on a cheat, promise to get paid, and get lied to. peace out.

		memcpy(g::pLocalEntity->m_flPoseParameter(), pose_parameters, sizeof(float) * 24);
		memcpy(g::pLocalEntity->GetAnimOverlays(), anim_layers, sizeof(AnimationLayer) * 13);
	}
}

void animation_state::create_state(C_AnimState* state, C_BaseEntity* player) {
	using Fn = void(__thiscall*)(C_AnimState*, C_BaseEntity*);
	static auto fn = reinterpret_cast<Fn>(Utils::FindSignature("client_panorama.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46"));
	fn(state, player);
}

void animation_state::update_state(C_AnimState* state, Vector angles) {
	using Fn = void(__vectorcall*)(void*, void*, float, float, float, void*);
	static auto fn = reinterpret_cast<Fn>(Utils::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 F3 0F 11 54 24"));
	fn(state, nullptr, 0.0f, angles[1], angles[0], nullptr);
}

void animation_state::reset_state(C_AnimState* state) {
	using Fn = void(__thiscall*)(C_AnimState*);
	static auto fn = reinterpret_cast<Fn>(Utils::FindSignature("client_panorama.dll", "56 6A 01 68 ? ? ? ? 8B F1"));
	fn(state);
}

C_AnimState* animation_state::animstate() {
	return server_animation_state;
}



/*
float antiaim::get_pitch()
{
	int index;

	index = g_Options.antiaa.stand;


	float pitch = g::pCmd->viewangles.x;
	if (index)
	{
		switch (index)
		{
		case 1:
			pitch = 89;


			break;
		case 2:
			pitch = -180;
			break;
		case 3:
			pitch = -991;
			break;
		case 4:
			pitch = 991;
			break;
		case 5:
			pitch = 0;
			break;
		case 6:
			pitch = 1080;
			break;
		}

	}
	return pitch;
}*/

Vector calcul2323ate_angle(Vector src, Vector dst) {
	Vector angles;

	Vector delta = src - dst;
	float hyp = delta.Length2D();

	angles.y = std::atanf(delta.y / delta.x) * 57.2957795131f;
	angles.x = std::atanf(-delta.z / hyp) * -57.2957795131f;
	angles.z = 0.0f;

	if (delta.x >= 0.0f)
		angles.y += 180.0f;

	return angles;
}
float antiaim::at_target()
{
	int cur_tar = 0;
	float last_dist = 999999999999.0f;

	for (int i = 0; i < g_pGlobalVars->maxClients; i++)
	{
		auto m_entity = g_pEntityList->GetClientEntity(i);

		if (!m_entity || m_entity == g::pLocalEntity)
			continue;

		if (!m_entity->is_player())
			continue;

		auto m_player = (C_BaseEntity*)m_entity;
		if (!m_player->IsDormant() && m_player->IsAlive() && m_player->GetTeam() != g::pLocalEntity->GetTeam()) {
			float cur_dist = (m_entity->GetOrigin() - g::pLocalEntity->GetOrigin()).Length();

			if (!cur_tar || cur_dist < last_dist) {
				cur_tar = i;
				last_dist = cur_dist;
			}
		}
	}

	if (cur_tar) {
		auto m_entity = static_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(cur_tar));
		if (!m_entity) {
			return g::pCmd->viewangles.y;
		}

		Vector target_angle = calcul2323ate_angle(g::pLocalEntity->GetOrigin(), m_entity->GetOrigin());
		return target_angle.y;
	}

	return g::pCmd->viewangles.y;
}
void AngleVectorsANtiAim(const Vector& angles, Vector* forward)//most of this is also defined in resolver so i thought why dont make a resolver out of it
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
void NormalizeNum(Vector& vIn, Vector& vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}


float fov_ent(Vector ViewOffSet, Vector View, C_BaseEntity* entity, int hitbox)
{
	const float MaxDegrees = 180.0f;
	Vector Angles = View, Origin = ViewOffSet;
	Vector Delta(0, 0, 0), Forward(0, 0, 0);
	Vector AimPos = entity->hitbox_position(hitbox);
	AngleVectorsANtiAim(Angles, &Forward);
	VectorSubtract(AimPos, Origin, Delta);
	NormalizeNum(Delta, Delta);
	float DotProduct = Forward.Dot(Delta);
	return (acos(DotProduct) * (MaxDegrees / PI));
}




struct angle_data
{
	float angle;
	float thickness;
	angle_data(const float angle, const float thickness) : angle(angle), thickness(thickness) {}
};

inline float sseSqrt(float x)
{
	//return sqrtf(x);

	float root = 0.0f;

	__asm
	{
		sqrtss xmm0, x
		movss root, xmm0
	}

	return root;
}


inline void zxdVectorAngles(Vector forward, QAngle& angles)
{
	//Assert(s_bMathlibInitialized);
	float yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		float tmp = sseSqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}


inline QAngle zxdCalcAngle(const Vector& src, const Vector& dst)
{
	Vector delta = dst - src;
	QAngle angles;
	zxdVectorAngles(delta, angles);

	return angles;
}

float quick_normalize(float degree, const float min, const float max) {
	while (degree < min)
		degree += max - min;
	while (degree > max)
		degree -= max - min;

	return degree;
}
inline void SinC432os(float radians, float* sine, float* cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);
}
inline void zxdAngleVectors(const QAngle& angles, Vector* forward)
{
	float sp, sy, cp, cy;

	SinC432os(DEG2RAD(angles.y), &sy, &cy);
	SinC432os(DEG2RAD(angles.x), &sp, &cp);

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

void antiaim::StaticBodyYaw(CUserCmd* cmd) // thx who ever dumped skeet for this, also emm the values are wrong but stil we hit p
{

	auto local = g::pLocalEntity;

	if (cmd->buttons & IN_JUMP)
		return;

	auto tick = g_pGlobalVars->tickcount % 4;

	float fm;
	if (cmd->buttons & IN_DUCK) {
		fm = 2.941177;
	}
	else
		fm = 1.000001;

	float sm = fm;

	if (tick == 1 || tick == 3) {
		fm = 0.0;
	}
	else {
		if (tick == 2) {
			fm = -fm;
		}
		else {
			fm = fm;
		}
	}

	if (tick == 0 || tick == 2) {
		sm = 0.0;
	}
	else {
		if (tick == 3) {
			sm = -sm;
		}
		else {
			sm = sm;
		}
	}

	if (cmd->forwardmove != 0) {
		cmd->forwardmove = cmd->forwardmove;
	}
	else {
		cmd->forwardmove = fm;
	}

	if (cmd->sidemove != 0) {
		cmd->sidemove = cmd->sidemove;
	}
	else {
		cmd->sidemove = sm;
	}
}


bool IsWeaponGrenade4(C_BaseCombatWeapon* weapon)
{
	if (weapon == nullptr) return false;
	int id = *weapon->m_iItemDefinitionIndex();
	static const std::vector<int> v = { (int)ItemDefinitionIndex::WEAPON_FLASHBANG,(int)ItemDefinitionIndex::WEAPON_HEGRENADE,(int)ItemDefinitionIndex::WEAPON_SMOKEGRENADE,(int)ItemDefinitionIndex::WEAPON_MOLOTOV,(int)ItemDefinitionIndex::WEAPON_DECOY,(int)ItemDefinitionIndex::WEAPON_INC };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

static bool left, right, back, frees = true;



float get_curtime(CUserCmd* ucmd)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player)
		return 0;
	if (!local_player->GetTickBase())
		return 0;


	int g_tick = 0;
	CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = (float)local_player->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * g_pGlobalVars->intervalPerTick;
	return curtime;
}
static float next_lby_update_time = 0;

bool next_lby_update(CUserCmd* cmd)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player)
		return false;

	float curtime = get_curtime(cmd);

	auto animstate = local_player->AnimState();
	if (!animstate)
		return false;

	auto net_channel = g_pEngine->GetNetChannel();

	if (!net_channel || net_channel->m_nChokedPackets)
		return false;

	if (!(local_player->GetFlags() & FL_ONGROUND))
		return false;

	if (animstate->speed_2d > 0.1)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}
	return false;
}


int m_iJitter = 0;

bool Swtich = false;

void FreeStanding()
{
	static float FinalAngle;
	bool bside1 = false;
	bool bside2 = false;
	bool autowalld = false;
	for (int i = 0; i <= g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant()
			|| pPlayerEntity == g::pLocalEntity
			|| pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
			continue;

		float angToLocal = g_Math.CalcAngle(g::pLocalEntity->GetOrigin(), pPlayerEntity->GetOrigin()).y;
		Vector ViewPoint = pPlayerEntity->GetOrigin() + Vector(0, 0, 90);

		Vector2D Side1 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal))),(45 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side2 = { (45 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(45 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector2D Side3 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal))),(50 * cos(g_Math.GRD_TO_BOG(angToLocal))) };
		Vector2D Side4 = { (50 * sin(g_Math.GRD_TO_BOG(angToLocal + 180))) ,(50 * cos(g_Math.GRD_TO_BOG(angToLocal + 180))) };

		Vector Origin = g::pLocalEntity->GetOrigin();

		Vector2D OriginLeftRight[] = { Vector2D(Side1.x, Side1.y), Vector2D(Side2.x, Side2.y) };

		Vector2D OriginLeftRightLocal[] = { Vector2D(Side3.x, Side3.y), Vector2D(Side4.x, Side4.y) };

		for (int side = 0; side < 2; side++)
		{
			Vector OriginAutowall = { Origin.x + OriginLeftRight[side].x,  Origin.y - OriginLeftRight[side].y , Origin.z + 80 };
			Vector OriginAutowall2 = { ViewPoint.x + OriginLeftRightLocal[side].x,  ViewPoint.y - OriginLeftRightLocal[side].y , ViewPoint.z };

			if (g_Autowall.CanHitFloatingPoint(OriginAutowall, ViewPoint))
			{
				if (side == 0)
				{
					bside1 = true;
					FinalAngle = angToLocal + 90;
				}
				else if (side == 1)
				{
					bside2 = true;
					FinalAngle = angToLocal - 90;
				}
				autowalld = true;
			}
			else
			{
				for (int side222 = 0; side222 < 2; side222++)
				{
					Vector OriginAutowall222 = { Origin.x + OriginLeftRight[side222].x,  Origin.y - OriginLeftRight[side222].y , Origin.z + 80 };

					if (g_Autowall.CanHitFloatingPoint(OriginAutowall222, OriginAutowall2))
					{
						if (side222 == 0)
						{
							bside1 = true;
							FinalAngle = angToLocal + 90;
						}
						else if (side222 == 1)
						{
							bside2 = true;
							FinalAngle = angToLocal - 90;
						}
						autowalld = true;
					}
				}
			}
		}
	}


	if (!autowalld || (bside1 && bside2))
	{

	}
	else
		g::pCmd->viewangles.y = FinalAngle;



	if (g_Options.antiaa.stand.jitter != 0)
	{
		float Offset = g_Options.antiaa.stand.jitter;


		Swtich ? g::pCmd->viewangles.y -= Offset : g::pCmd->viewangles.y += Offset;
	}




}
float m_bestthreat = 0.f;

mstudiobbox_t* get_hitboxv2(C_BaseEntity* entity, int hitbox_index)
{
	if (entity->IsDormant() || entity->GetHealth() <= 0)
		return NULL;

	const auto pModel = entity->GetModel();
	if (!pModel)
		return NULL;

	auto pStudioHdr = g_pModelInfo->GetStudiomodel(pModel);
	if (!pStudioHdr)
		return NULL;

	auto pSet = pStudioHdr->GetHitboxSet(0);
	if (!pSet)
		return NULL;

	if (hitbox_index >= pSet->numhitboxes || hitbox_index < 0)
		return NULL;

	return pSet->GetHitbox(hitbox_index);
}


Vector GetHitboxPositionv2(C_BaseEntity* entity, int hitbox_id)
{
	auto hitbox = get_hitboxv2(entity, hitbox_id);
	if (!hitbox)
		return Vector(0, 0, 0);

	auto bone_matrix = entity->GetBoneMatrix(hitbox->bone);

	Vector bbmin, bbmax;
	g_Math.VectorTransform(hitbox->min, bone_matrix, bbmin);
	g_Math.VectorTransform(hitbox->max, bone_matrix, bbmax);

	return (bbmin + bbmax) * 0.5f;
}

float fov_player(Vector ViewOffSet, Vector View, C_BaseEntity* entity, int hitbox)
{
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
	g_Math.AngleVectors(Angles, &Forward);

	Vector AimPos = GetHitboxPositionv2(entity, hitbox); //pvs fix disabled

	VectorSubtract(AimPos, Origin, Delta);
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	NormalizeNum(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / M_PI));
}

int closest_to_crosshair()
{
	int index = -1;
	float lowest_fov = INT_MAX;

	auto local_player = g::pLocalEntity;

	if (!local_player)
		return -1;

	Vector local_position = local_player->GetOrigin() + local_player->GetViewOffset();

	Vector angles;
	g_pEngine->GetViewAngles(angles);

	for (int i = 1; i <= g_pGlobalVars->maxClients; i++)
	{
		auto entity = g_pEntityList->GetClientEntity(i);

		if (!entity || !entity->IsAlive() || entity->GetTeam() == local_player->GetTeam() || entity->IsDormant() || entity == local_player)
			continue;

		float fov = fov_player(local_position, angles, entity, 0);

		if (fov < lowest_fov)
		{
			lowest_fov = fov;
			index = i;
		}
	}

	return index;
}
int closest_to_local_player()
{
	int index = -1;
	float lowest_distance = INT_MAX;

	auto local_player = g::pLocalEntity;

	if (!local_player)
		return -1;

	Vector local_position = local_player->GetOrigin() + local_player->GetViewOffset();

	Vector angles;
	g_pEngine->GetViewAngles(angles);

	for (int i = 1; i <= g_pGlobalVars->maxClients; i++)
	{
		auto entity = g_pEntityList->GetClientEntity(i);

		if (!entity || !entity->IsAlive() || entity->GetTeam() == local_player->GetTeam() || entity->IsDormant() || entity == local_player)
			continue;

		float distance = g_Math.Distance(Vector2D(local_player->GetOrigin().x, local_player->GetOrigin().y), Vector2D(entity->GetOrigin().x, entity->GetOrigin().y));

		if (distance < lowest_distance)
		{
			lowest_distance = distance;
			index = i;
		}
	}

	return index;
}


void freestand(CUserCmd* cmd)
{
	auto local_player = g::pLocalEntity;

	if (!local_player)
		return;

	static float last_real;
	bool no_active = true;
	float bestrotation = 0.f;
	float highestthickness = 0.f;
	Vector besthead;

	m_bestthreat = 0.f;

	auto leyepos = local_player->GetOrigin() + local_player->GetViewOffset();
	auto headpos = GetHitboxPositionv2(local_player, 0);
	auto origin = local_player->GetAbsOrigin();


	auto checkWallThickness = [&](C_BaseEntity* pPlayer, Vector newhead) -> float
	{
		Vector endpos1, endpos2;
		Vector eyepos = pPlayer->GetOrigin() + pPlayer->GetViewOffset();


		CTraceFilterSkipTwoEntities filter(pPlayer, local_player);

		C_Trace trace1, trace2;
		g_pTrace->TraceRay(C_Ray(newhead, eyepos), mask_shot_brushonly, &filter, &trace1);

		if (trace1.DidHit())
			endpos1 = trace1.end;
		else
			return 0.f;


		g_pTrace->TraceRay(C_Ray(eyepos, newhead), mask_shot_brushonly, &filter, &trace2);

		if (trace2.DidHit())
			endpos2 = trace2.end;

		float add = newhead.DistTo(eyepos) - leyepos.DistTo(eyepos) + 3.f;
		return endpos1.DistTo(endpos2) + add / 3;
	};

	int index = -1;

	index = closest_to_local_player();


	static C_BaseEntity* entity;

	if (index != -1)
		entity = g_pEntityList->GetClientEntity(index);

	float step = (2 * M_PI) / 18.f; // One PI = half a circle ( for stacker cause low iq :sunglasses: ), 28

	float radius = fabs(Vector(headpos - origin).Length2D());

	if (index == -1)
	{
		no_active = true;
	}
	else
	{
		for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
		{
			Vector newhead(radius * cos(rotation) + leyepos.x, radius * sin(rotation) + leyepos.y, leyepos.z);

			float totalthickness = 0.f;

			no_active = false;

			totalthickness += checkWallThickness(entity, newhead);

			if (totalthickness > highestthickness)
			{
				highestthickness = totalthickness;
				bestrotation = rotation;
				besthead = newhead;
			}
		}
	}


}



float antiaim::get_max_desync_delta()
{

	auto animstate = g::pLocalEntity->AnimState();
	if (!animstate)
		return -1;


}
float normaliz2332e_yaw(float f) {
	if (std::isnan(f) || std::isinf(f))
		f = 0;

	if (f > 9999999)
		f = 0;

	if (f < -9999999)
		f = 0;

	while (f < -180.0f) f += 360.0f;
	while (f > 180.0f) f -= 360.0f;

	return f;
}



void antiaim::ResetAnimationState(C_AnimState* state)
{
	using ResetAnimState_t = void(__thiscall*)(C_AnimState*);
	static auto ResetAnimState = (ResetAnimState_t)Utils::FindSignature("client_panorama.dll", "56 6A 01 68 ? ? ? ? 8B F1");
	if (!ResetAnimState)
		return;

	ResetAnimState(state);
}

void antiaim::CreateAnimationState(C_AnimState* state, C_BaseEntity* player)
{
	using CreateAnimState_t = void(__thiscall*)(C_AnimState*, C_BaseEntity*);
	static auto CreateAnimState = (CreateAnimState_t)Utils::FindSignature("client_panorama.dll", "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46");
	if (!CreateAnimState)
		return;

	CreateAnimState(state, player);
}

static uintptr_t UpdateAnimState;
void antiaim::UpdateAnimationState(C_AnimState* state, Vector angle)
{
	if (!state)
		UpdateAnimState = Utils::FindSignature("client_panorama.dll", "55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74 36");
	if (!UpdateAnimState)
		return;

	__asm
	{
		mov ecx, state

		movss xmm1, dword ptr[angle + 4]
		movss xmm2, dword ptr[angle]

		call UpdateAnimState
	}
}

















float antiaim::GetYaw()
{
	Vector Angles;
	g_pEngine->GetViewAngles(Angles);
	float_t pos = Angles.y;


	return (pos + 180);

}

float antiaim::GetFakeYaw()
{


	Vector Angles;
	g_pEngine->GetViewAngles(Angles);


	return (GetYaw() + g::pLocalEntity->DesyncValue());





}

void ResetAnimationState(C_AnimState* state)
{
	if (!state)
		return;

	using ResetAnimState_t = void(__thiscall*)(C_AnimState*);
	static auto ResetAnimState = (ResetAnimState_t)Utils::FindSignature(("client_panorama.dll"), "56 6A 01 68 ? ? ? ? 8B F1");

	if (!ResetAnimState)
		return;

	ResetAnimState(state);
}

float GetCur32time()
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player)
		return 0.f;


	if (!local_player->GetTickBase())
		return -1;

	return local_player->GetTickBase() * g_pGlobalVars->intervalPerTick;
}
inline float Normaliz243423eYaw(float yaw)
{
	if (yaw > 180)
		yaw -= (round(yaw / 360) * 360.f);
	else if (yaw < -180)
		yaw += (round(yaw / 360) * -360.f);

	return yaw;
}

inline float GetYawDelta(float yaw1, float yaw2)
{
	return fabs(Normaliz243423eYaw(yaw1 - yaw2));
}

void antiaim::UpdateLBY()
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player || !(local_player->GetFlags() & FL_ONGROUND))
		return;

	auto animstate = local_player->AnimState();
	if (!animstate)
		return;

	auto net_channel = g_pEngine->GetNetChannel();
	if (!net_channel || net_channel->m_nChokedPackets)
		return;

	const float curtime = GetCur32time();
	if (animstate->speed_2d > 0.1 && !g::fake_walk)
	{
		m_next_lby_update_time = curtime + 0.22;
		m_last_move_time = curtime;
	}
	if (static float last_lby;
		last_lby != local_player->GetLowerBodyYaw()
		)
	{
		last_lby = local_player->GetLowerBodyYaw();
		if (fabs(curtime - m_last_move_time) > 0.3 && fabs(Normaliz243423eYaw(local_player->GetLowerBodyYaw() - m_last_attempted_lby)) > 35.f)
			m_next_lby_update_time = curtime + 1.1 + g_pGlobalVars->intervalPerTick;
	}

	if (m_next_lby_update_time < curtime)
	{
		m_next_lby_update_time = curtime + 1.1;
		m_will_lby_update = true;
	}
}

void antiaim::BreakLBY(float real_yaw, float desired_lby)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player)
		return;

	desired_lby = GetYawDelta(desired_lby, local_player->GetLowerBodyYaw()) < 35.f ? local_player->GetLowerBodyYaw() : desired_lby;
	if (m_will_lby_update)
	{
		static bool switch_bool = false;
		switch_bool = !switch_bool;
		if (const float lby_delta = Normaliz243423eYaw(desired_lby - real_yaw);
			fabs(lby_delta) < 100.f
			)
			desired_lby = real_yaw + (switch_bool ? lby_delta : -lby_delta);

		m_will_lby_update = false;
		m_last_attempted_lby = desired_lby;
		g::pCmd->viewangles.y = desired_lby;
	}

	if (fabs(Normaliz243423eYaw(real_yaw - desired_lby)) > 130.f && fabs(Normaliz243423eYaw(desired_lby - local_player->GetLowerBodyYaw())) < 35.f)
	{
		if (fabs(GetCur32time() - m_next_lby_update_time) < g_pGlobalVars->intervalPerTick)
			g::pCmd->viewangles.y = desired_lby - 115.f;
		if (fabs(GetCur32time() - m_next_lby_update_time) > 1.1 - (g_pGlobalVars->intervalPerTick * 5))
			g::pCmd->viewangles.y = desired_lby + 115.f;
	}

	g::pCmd->viewangles.y = real_yaw;
}


float lastUpdate;
float wasmoving;
bool performBreak;
bool m_bBreakBalance = false;

void LbyBreaker()
{
	float Velocity = g::pLocalEntity->GetVelocity().Length2D();


	if (!performBreak)
	{
		if (Velocity > 0.1f && (g::pLocalEntity->GetFlags() & FL_ONGROUND)) {
			lastUpdate = g_pGlobalVars->curtime;
			wasmoving = true;
			m_bBreakBalance = true;
		}
		else {
			if (wasmoving && g_pGlobalVars->curtime - lastUpdate > 0.22f && (g::pLocalEntity->GetFlags() & FL_ONGROUND)) {
				wasmoving = false;
				performBreak = true;
				m_bBreakBalance = false;
			}
			else if (g_pGlobalVars->curtime - lastUpdate > 1.1f && (g::pLocalEntity->GetFlags() & FL_ONGROUND)) {
				performBreak = true;
				m_bBreakBalance = false;
			}
		}
	}
}




void Desync()
{
	int finalangle = g::RealAngle.y;
	float MaxDelta = AntiAimNEW.MaxDelta(g::pLocalEntity);
	float MaxDeltaLby = 34;
	int val = MaxDelta;
	float Velocity = g::pLocalEntity->GetVelocity().Length2D();

	Vector view;
	g_pEngine->GetViewAngles(view);



	finalangle += g::gayside ? -MaxDelta : MaxDelta;

	g::pCmd->viewangles.y = finalangle;

}


float antiaim::MaxDelta(C_BaseEntity* pEnt)
{

	auto animstate = uintptr_t(pEnt->AnimState());

	float duckammount = *(float*)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

	float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001f) - 0.19999999f)* speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0) {
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}

	unk3 = *(float*)(animstate + 0x334) * unk2;

	return unk3;
}
void antiaim::do_antiaim(CUserCmd* cmd)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player) return;

	//Fakelag(cmd);

	auto weapon = g::pLocalEntity->GetActiveWeapon();
	if (!weapon) return;

	if (!g_pEngine->IsInGame() || !g_Options.antiaa.enable)
		return;

	if (!g::pLocalEntity->IsAlive())
		return;

	if (IsWeaponGrenade4(weapon))
	{
		if (!weapon->IsPinPulled() || (g::pCmd->buttons & IN_ATTACK) || (g::pCmd->buttons & IN_ATTACK2))
		{
			float throwTime = weapon->GetThrowTime();

			if (throwTime > 0)
				return;
		}
	}

	if (weapon->IsKnife() && g::pCmd->buttons & IN_ATTACK2)
		return;

	if (g::pLocalEntity->GetMoveType() == 9)
		return;

	if (g::pCmd->buttons & IN_USE)
		return;
	if (g::pCmd->buttons & IN_ATTACK)
		return;


	auto animState = local_player->AnimState();
	if (!animState)
		return;



	static bool side = false;


	if (g::bSendPacket)
		Swtich = !Swtich;







	float gaysid132e;

	float MaxDelta;
	if (g::pLocalEntity->DesyncValue() > g_Options.antiaa.stand.desync)
	{
		if (g_Options.antiaa.stand.desync > g::pLocalEntity->DesyncValue())
			MaxDelta = g::pLocalEntity->DesyncValue();
		else
		{
			MaxDelta = g_Options.antiaa.stand.desync;

		}
	}
	else
	{
		MaxDelta = g::pLocalEntity->DesyncValue();

	}


	if (g_Options.antiaa.lby_break)
	{

		LbyBreaker();



		if (performBreak) // using anything else makes 0 sense
		{

			g::bSendPacket = false;
			g::pCmd->viewangles.y = g::RealAngle.y + 121 * g::gayside;
			lastUpdate = g_pGlobalVars->curtime;
			performBreak = false;
			return;
		}
	}

	if (!g::bSendPacket && g::pLocalEntity->AnimState())
	{

		//Pitch


		switch (ReturnMovementType())
		{
		case MovingAAType::Standing:
		{
			switch (g_Options.antiaa.stand.pitch_selected)
			{
			case 1: g::pCmd->viewangles.x = 89.9f;
				break;
			case 2: g::pCmd->viewangles.x = -89.9f;
				break;
			case 3: g::pCmd->viewangles.x = 0;
				break;
			}
		}break;
		case MovingAAType::Moving:
		{
			switch (g_Options.antiaa.moving.pitch_selected)
			{
			case 1: g::pCmd->viewangles.x = 89.9f;
				break;
			case 2: g::pCmd->viewangles.x = -89.9f;
				break;
			case 3: g::pCmd->viewangles.x = 0;
				break;
			}
		}break;
		case MovingAAType::Air:
		{
			switch (g_Options.antiaa.air.pitch_selected)
			{
			case 1: g::pCmd->viewangles.x = 89.9f;
				break;
			case 2: g::pCmd->viewangles.x = -89.9f;
				break;
			case 3: g::pCmd->viewangles.x = 0;
				break;
			}
		}break;

		case MovingAAType::Slowwalk:
		{
			switch (g_Options.antiaa.slow.pitch_selected)
			{
			case 1: g::pCmd->viewangles.x = 89.9f;
				break;
			case 2: g::pCmd->viewangles.x = -89.9f;
				break;
			case 3: g::pCmd->viewangles.x = 0;
				break;
			}

		}break;


		}
		switch (ReturnMovementType())
		{
		case MovingAAType::Standing:
		{
			int
				index,
				base_angle,
				offset;


			index = g_Options.antiaa.stand.yaw_selected;
			base_angle = g_Options.antiaa.stand.yaw_base_selected;
			offset = g_Options.antiaa.stand.offset;



			g::pCmd->viewangles.y = 0.0f;

			if (base_angle == 1)
			{
				Vector angles;
				g_pEngine->GetViewAngles(angles);
				g::pCmd->viewangles.y = angles.y;
			}

			if (base_angle == 2)
				g::pCmd->viewangles.y = at_target();

			switch (index)
			{

			case 1:
				g::pCmd->viewangles.y += 90.0f;
				break;
			case 2:
				g::pCmd->viewangles.y += 180;
				break;
			case 3:
				g::pCmd->viewangles.y += g_pGlobalVars->curtime * 420.0f;
				break;
			case 4:
			{


				static bool bSwitch = false;

				if (bSwitch)
				{
					g::pCmd->viewangles.y += 180 + g_Options.antiaa.stand.jitter;
					bSwitch = false;
				}
				else
				{
					g::pCmd->viewangles.y += 180 + -g_Options.antiaa.stand.jitter;
					bSwitch = true;
				}
			}
			break;
			}

			switch (g_Options.antiaa.stand.edge_type_selected)
			{
			case 0:

				break;
			case 1:


				if (g_Options.antiaa.invert_key)
				{
					side2 = false;
					if (GetKeyState(g_Options.antiaa.invert_key))
					{
						side2 = true;
					}
				}

				if (side2)
				{
					cmd->viewangles.y -= 90;

				}
				else
				{
					cmd->viewangles.y += 90;

				}




				break;
			case 2:


				FreeStanding();





				break;
			}

			g::pCmd->viewangles.y += offset;

			if (g_Options.antiaa.stand.edge_type_selected == 1)
			{
				if (g_Options.antiaa.invert_key)
				{
					side2 = false;
					if (GetKeyState(g_Options.antiaa.invert_key))
					{
						side2 = true;
					}
				}

				if (side2)
				{
					g::pCmd->viewangles.y += g_Options.antiaa.stand.body_lean;

				}
				else
				{
					g::pCmd->viewangles.y += g_Options.antiaa.stand.inverted_body_lean;

				}
			}
		}break;
		case MovingAAType::Moving:
		{
			int
				index,
				base_angle,
				offset;


            index = g_Options.antiaa.moving.yaw_selected;
            base_angle = g_Options.antiaa.moving.yaw_base_selected;
            offset = g_Options.antiaa.moving.offset;



			g::pCmd->viewangles.y = 0.0f;

			if (base_angle == 1)
			{
				Vector angles;
				g_pEngine->GetViewAngles(angles);
				g::pCmd->viewangles.y = angles.y;
			}

			if (base_angle == 2)
				g::pCmd->viewangles.y = at_target();

			switch (index)
			{

			case 1:
				g::pCmd->viewangles.y += 90.0f;
				break;
			case 2:
				g::pCmd->viewangles.y += 180;
				break;
			case 3:
				g::pCmd->viewangles.y += g_pGlobalVars->curtime * 420.0f;
				break;
			case 4:
			{


				static bool bSwitch = false;

                if (bSwitch)
                {
                    g::pCmd->viewangles.y += 180 + g_Options.antiaa.moving.jitter;
                    bSwitch = false;
                }
                else
                {
                    g::pCmd->viewangles.y += 180 + -g_Options.antiaa.moving.jitter;
                    bSwitch = true;
                }
			}
			break;
			}

            switch (g_Options.antiaa.moving.edge_type_selected)
			{
			case 0:

				break;
			case 1:


                if (g_Options.antiaa.invert_key)
                {
                    side2 = false;
                    if (GetKeyState(g_Options.antiaa.invert_key))
                    {
                        side2 = true;
                    }
                }

				if (side2)
				{
					cmd->viewangles.y -= 90;

				}
				else
				{
					cmd->viewangles.y += 90;

				}




				break;
			case 2:


				FreeStanding();





				break;
			}

			g::pCmd->viewangles.y += offset;

            if (g_Options.antiaa.moving.edge_type_selected == 1)
            {
                if (g_Options.antiaa.invert_key)
                {
                    side2 = false;
                    if (GetKeyState(g_Options.antiaa.invert_key))
                    {
                        side2 = true;
                    }
                }

                if (side2)
                {
                    g::pCmd->viewangles.y += g_Options.antiaa.moving.body_lean;

                }
                else
                {
                    g::pCmd->viewangles.y += g_Options.antiaa.moving.inverted_body_lean;

                }
            }
		}break;
		case MovingAAType::Air:
		{
			int
				index,
				base_angle,
				offset;


            index = g_Options.antiaa.air.yaw_selected;
            base_angle = g_Options.antiaa.air.yaw_base_selected;
            offset = g_Options.antiaa.air.offset;




			g::pCmd->viewangles.y = 0.0f;

			if (base_angle == 1)
			{
				Vector angles;
				g_pEngine->GetViewAngles(angles);
				g::pCmd->viewangles.y = angles.y;
			}

			if (base_angle == 2)
				g::pCmd->viewangles.y = at_target();

			switch (index)
			{

			case 1:
				g::pCmd->viewangles.y += 90.0f;
				break;
			case 2:
				g::pCmd->viewangles.y += 180;
				break;
			case 3:
				g::pCmd->viewangles.y += g_pGlobalVars->curtime * 420.0f;
				break;
			case 4:
			{


                static bool bSwitch = false;

                if (bSwitch)
                {
                    g::pCmd->viewangles.y += 180 + g_Options.antiaa.air.jitter;
                    bSwitch = false;
                }
                else
                {
                    g::pCmd->viewangles.y += 180 + -g_Options.antiaa.air.jitter;
                    bSwitch = true;
                }
			}
			break;
			}

            switch (g_Options.antiaa.air.edge_type_selected)
            {
            case 0:

                break;
            case 1:


                if (g_Options.antiaa.invert_key)
                {
                    side2 = false;
                    if (GetKeyState(g_Options.antiaa.invert_key))
                    {
                        side2 = true;
                    }
                }

                if (side2)
                {
                    cmd->viewangles.y -= 90;

                }
                else
                {
                    cmd->viewangles.y += 90;

                }




                break;
            case 2:


                FreeStanding();





                break;
            }

			g::pCmd->viewangles.y += offset;

            if (g_Options.antiaa.air.edge_type_selected == 1)
            {
                if (g_Options.antiaa.invert_key)
                {
                    side2 = false;
                    if (GetKeyState(g_Options.antiaa.invert_key))
                    {
                        side2 = true;
                    }
                }

                if (side2)
                {
                    g::pCmd->viewangles.y += g_Options.antiaa.air.body_lean;

                }
                else
                {
                    g::pCmd->viewangles.y += g_Options.antiaa.air.inverted_body_lean;

                }
            }
		}break;

		case MovingAAType::Slowwalk:
		{
			int
				index,
				base_angle,
				offset;


            index = g_Options.antiaa.slow.yaw_selected;
            base_angle = g_Options.antiaa.slow.yaw_base_selected;
            offset = g_Options.antiaa.slow.offset;



			g::pCmd->viewangles.y = 0.0f;

			if (base_angle == 1)
			{
				Vector angles;
				g_pEngine->GetViewAngles(angles);
				g::pCmd->viewangles.y = angles.y;
			}

			if (base_angle == 2)
				g::pCmd->viewangles.y = at_target();

			switch (index)
			{

			case 1:
				g::pCmd->viewangles.y += 90.0f;
				break;
			case 2:
				g::pCmd->viewangles.y += 180;
				break;
			case 3:
				g::pCmd->viewangles.y += g_pGlobalVars->curtime * 420.0f;
				break;
			case 4:
			{


                static bool bSwitch = false;

                if (bSwitch)
                {
                    g::pCmd->viewangles.y += 180 + g_Options.antiaa.slow.jitter;
                    bSwitch = false;
                }
                else
                {
                    g::pCmd->viewangles.y += 180 + -g_Options.antiaa.slow.jitter;
                    bSwitch = true;
                }
			}
			break;
			}

            switch (g_Options.antiaa.slow.edge_type_selected)
            {
            case 0:

                break;
            case 1:


                if (g_Options.antiaa.invert_key)
                {
                    side2 = false;
                    if (GetKeyState(g_Options.antiaa.invert_key))
                    {
                        side2 = true;
                    }
                }

                if (side2)
                {
                    cmd->viewangles.y -= 90;

                }
                else
                {
                    cmd->viewangles.y += 90;

                }




                break;
            case 2:


                FreeStanding();





                break;
            }

            g::pCmd->viewangles.y += offset;

            if (g_Options.antiaa.slow.edge_type_selected == 1)
            {
                if (g_Options.antiaa.invert_key)
                {
                    side2 = false;
                    if (GetKeyState(g_Options.antiaa.invert_key))
                    {
                        side2 = true;
                    }
                }

                if (side2)
                {
                    g::pCmd->viewangles.y += g_Options.antiaa.slow.body_lean;

                }
                else
                {
                    g::pCmd->viewangles.y += g_Options.antiaa.slow.inverted_body_lean;

                }
            }

		}break;


		}



	}
	else
	{
		//Pitch



		switch (ReturnMovementType())
		{
		case MovingAAType::Standing:
		{
			switch (g_Options.antiaa.stand.pitch_selected)
			{
			case 1: g::pCmd->viewangles.x = 89.9f;
				break;
			case 2: g::pCmd->viewangles.x = -89.9f;
				break;
			case 3: g::pCmd->viewangles.x = 0;
				break;
			}
		}break;
		case MovingAAType::Moving:
		{
			switch (g_Options.antiaa.moving.pitch_selected)
			{
			case 1: g::pCmd->viewangles.x = 89.9f;
				break;
			case 2: g::pCmd->viewangles.x = -89.9f;
				break;
			case 3: g::pCmd->viewangles.x = 0;
				break;
			}
		}break;
		case MovingAAType::Air:
		{
			switch (g_Options.antiaa.air.pitch_selected)
			{
			case 1: g::pCmd->viewangles.x = 89.9f;
				break;
			case 2: g::pCmd->viewangles.x = -89.9f;
				break;
			case 3: g::pCmd->viewangles.x = 0;
				break;
			}
		}break;

		case MovingAAType::Slowwalk:
		{
			switch (g_Options.antiaa.slow.pitch_selected)
			{
			case 1: g::pCmd->viewangles.x = 89.9f;
				break;
			case 2: g::pCmd->viewangles.x = -89.9f;
				break;
			case 3: g::pCmd->viewangles.x = 0;
				break;
			}

		}break;


		}
		switch (ReturnMovementType())
		{
		case MovingAAType::Standing:
		{
			if (g_Options.antiaa.stand.body_yaw_key)
			{
				side = false;
				if (GetKeyState(g_Options.antiaa.stand.body_yaw_key))
				{
					side = true;
				}
			}

			if (side)
			{
				gaysid132e = 1;

			}
			else
			{
				gaysid132e = -1;

			}

			switch (g_Options.antiaa.stand.body_yaw_selected)
			{
			case 0:g::pCmd->viewangles.y = g::RealAngle.y;
				break;
			case 1: g::pCmd->viewangles.y = g::RealAngle.y - MaxDelta * gaysid132e;
				break;
			case 2:
			{

				static bool desync_flip = false;
				desync_flip = !desync_flip;

				cmd->viewangles.y = g::RealAngle.y + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);

			}
			break;
			case 3:
			{
				static bool desync_flip = false;
				desync_flip = !desync_flip;

				g::pCmd->viewangles.y += 180;



				if (g_Options.antiaa.invert_key)
				{
					side2 = false;
					if (GetKeyState(g_Options.antiaa.invert_key))
					{
						side2 = true;
					}
				}

				if (side2)
				{
					cmd->viewangles.y -= 90;
					g::pCmd->viewangles.y += g_Options.antiaa.stand.body_lean;

				}
				else
				{
					cmd->viewangles.y += 90;
					g::pCmd->viewangles.y += g_Options.antiaa.stand.inverted_body_lean;

				}
				cmd->viewangles.y -= 180.0f + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);
			}
			break;
			}

		}break;
		case MovingAAType::Moving:
		{
            if (g_Options.antiaa.moving.body_yaw_key)
            {
                side = false;
                if (GetKeyState(g_Options.antiaa.moving.body_yaw_key))
                {
                    side = true;
                }
            }

			if (side)
			{
				gaysid132e = 1;

			}
			else
			{
				gaysid132e = -1;

			}

			switch (g_Options.antiaa.moving.body_yaw_selected)
			{
			case 0:g::pCmd->viewangles.y = g::RealAngle.y;
				break;
			case 1: g::pCmd->viewangles.y = g::RealAngle.y - MaxDelta * gaysid132e;
				break;
			case 2:
			{

				static bool desync_flip = false;
				desync_flip = !desync_flip;

				cmd->viewangles.y = g::RealAngle.y + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);

			}
			break;
			case 3:
			{
				static bool desync_flip = false;
				desync_flip = !desync_flip;

				g::pCmd->viewangles.y += 180;



                if (g_Options.antiaa.invert_key)
                {
                    side2 = false;
                    if (GetKeyState(g_Options.antiaa.invert_key))
                    {
                        side2 = true;
                    }
                }

                if (side2)
                {
                    cmd->viewangles.y -= 90;
                    g::pCmd->viewangles.y += g_Options.antiaa.moving.body_lean;

                }
                else
                {
                    cmd->viewangles.y += 90;
                    g::pCmd->viewangles.y += g_Options.antiaa.moving.inverted_body_lean;

                }
				cmd->viewangles.y -= 180.0f + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);
			}
			break;
			}
		}break;
		case MovingAAType::Air:
		{
            if (g_Options.antiaa.air.body_yaw_key)
            {
                side = false;
                if (GetKeyState(g_Options.antiaa.air.body_yaw_key))
                {
                    side = true;
                }
            }

			if (side)
			{
				gaysid132e = 1;

			}
			else
			{
				gaysid132e = -1;

			}

			switch (g_Options.antiaa.air.body_yaw_selected)
			{
			case 0:g::pCmd->viewangles.y = g::RealAngle.y;
				break;
			case 1: g::pCmd->viewangles.y = g::RealAngle.y - MaxDelta * gaysid132e;
				break;
			case 2:
			{

				static bool desync_flip = false;
				desync_flip = !desync_flip;

				cmd->viewangles.y = g::RealAngle.y + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);

			}
			break;
			case 3:
			{
				static bool desync_flip = false;
				desync_flip = !desync_flip;

				g::pCmd->viewangles.y += 180;



                if (g_Options.antiaa.invert_key)
                {
                    side2 = false;
                    if (GetKeyState(g_Options.antiaa.invert_key))
                    {
                        side2 = true;
                    }
                }

                if (side2)
                {
                    cmd->viewangles.y -= 90;
                    g::pCmd->viewangles.y += g_Options.antiaa.air.body_lean;

                }
                else
                {
                    cmd->viewangles.y += 90;
                    g::pCmd->viewangles.y += g_Options.antiaa.air.inverted_body_lean;

                }
				cmd->viewangles.y -= 180.0f + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);
			}
			break;
			}
		}break;
		case MovingAAType::Slowwalk:
		{

            if (g_Options.antiaa.slow.body_yaw_key)
            {
                side = false;
                if (GetKeyState(g_Options.antiaa.slow.body_yaw_key))
                {
                    side = true;
                }
            }

			if (side)
			{
				gaysid132e = 1;

			}
			else
			{
				gaysid132e = -1;

			}

			switch (g_Options.antiaa.slow.body_yaw_selected)
			{
			case 0:g::pCmd->viewangles.y = g::RealAngle.y;
				break;
			case 1: g::pCmd->viewangles.y = g::RealAngle.y - MaxDelta * gaysid132e;
				break;
			case 2:
			{

				static bool desync_flip = false;
				desync_flip = !desync_flip;

				cmd->viewangles.y = g::RealAngle.y + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);

			}
			break;
			case 3:
			{
				static bool desync_flip = false;
				desync_flip = !desync_flip;

				g::pCmd->viewangles.y += 180;



                if (g_Options.antiaa.invert_key)
                {
                    side2 = false;
                    if (GetKeyState(g_Options.antiaa.invert_key))
                    {
                        side2 = true;
                    }
                }

                if (side2)
                {
                    cmd->viewangles.y -= 90;
                    g::pCmd->viewangles.y += g_Options.antiaa.slow.body_lean;

                }
                else
                {
                    cmd->viewangles.y += 90;
                    g::pCmd->viewangles.y += g_Options.antiaa.slow.inverted_body_lean;

                }
				cmd->viewangles.y -= 180.0f + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);
			}
			break;
			}
		}break;


		}


	}




	/*
	if (g::pLocalEntity->GetFlags() & FL_ONGROUND && g::pCmd->sidemove < 4 && g::pCmd->sidemove > -4)
	{
		if (!(cmd->buttons & IN_DUCK)) {
			cmd->sidemove = cmd->tick_count % 2 ? 1.10 : -1.10;
		}
		else {
			cmd->sidemove = cmd->tick_count % 2 ? 3.25 : -3.25;
		}
	}

	if (Options::Menu.AntiAimTab.lbybreaker.GetState())
	{
		if (next_lby_update(g::pCmd))
		{
			g::bSendPacket = false;

			g::pCmd->viewangles.y = 120 * gayside;
			return;

		}
	}*/

	/*
	if (Options::Menu.AntiAimTab.break_adjustment.GetState())
	{


		if (g::pLocalEntity->GetFlags() & FL_ONGROUND && g::pCmd->sidemove < 4 && g::pCmd->sidemove > -4)
		{
			if (!(cmd->buttons & IN_DUCK)) {
				cmd->sidemove = cmd->tick_count % 2 ? 1.10 : -1.10;
			}
			else {
				cmd->sidemove = cmd->tick_count % 2 ? 3.25 : -3.25;
			}
		}


	}


	if (g::pLocalEntity->GetFlags() & FL_ONGROUND && g::pCmd->sidemove < 2 && g::pCmd->sidemove > -2)
	{
		if (g::pLocalEntity->GetFlags() & FL_ONGROUND && g::pCmd->forwardmove < 2 && g::pCmd->forwardmove > -2)
		{
			if (g::pLocalEntity->GetFlags() & FL_ONGROUND && g::pCmd->upmove < 2 && g::pCmd->upmove > -2)
			{
				if (g::pCmd->tick_count % 2)
				{
					g::pCmd->forwardmove = 1.01;

				}
				if (g::pCmd->tick_count % 3)
				{
					g::pCmd->sidemove = 1.01;

				}
				if (g::pCmd->tick_count % 4)
				{
					g::pCmd->upmove = 1.01;

				}

				if (g::pCmd->tick_count % 5)
				{
					g::pCmd->forwardmove = -1.01;

				}
				if (g::pCmd->tick_count % 6)
				{
					g::pCmd->sidemove = -1.01;

				}
				if (g::pCmd->tick_count % 7)
				{
					g::pCmd->upmove = -1.01;

				}

			}
		}
	}

	if (Options::Menu.AntiAimTab.break_adjustment.GetState())
	{
		if (animState->m_flSpeedNormalized < 0.1f)
		{

			if (g::pLocalEntity->GetFlags() & FL_ONGROUND && g::pCmd->sidemove < 4 && g::pCmd->sidemove > -4)
			{
				if (!(cmd->buttons & IN_DUCK)) {
					cmd->sidemove = cmd->tick_count % 2 ? 1.10 : -1.10;
				}
				else {
					cmd->sidemove = cmd->tick_count % 2 ? 3.25 : -3.25;
				}
			}

		}

	}

	*/


	// after al this research you need to know that you can calculate setupvelocity when sending the yae or sendpacket, thats why we do real in send packet//// onetap,skeet copied idea





//	if (next_lby_update(g::pCmd)) //we only want to break lby while faking our rotation
	//	g::pCmd->viewangles.y += 180 - MaxDesyndDeltaMenu * gayside;


	//Pitch Stuff

	//Real();





	//cmd->viewangles.y = g::RealAngle.y + (MaxDesyndDeltaMenu * gayside);
	/*
	if (!g::bSendPacket)
	{


	}
	else
	{
		g::pCmd->viewangles.x = get_pitch() + MaxDesyndDeltaMenu;

		switch (Options::Menu.AntiAimTab.rbot_desyncstanding.GetIndex())
		{

		case 1:
			cmd->viewangles.y = g::RealAngle.y + (MaxDesyndDeltaMenu * gayside);

			break;
		case 2:
		{

			static bool jitter2 = false;


			jitter2 = !jitter2;





			cmd->viewangles.y = g::RealAngle.y + (jitter2 ? -MaxDesyndDeltaMenu : MaxDesyndDeltaMenu);
		}
		break;
		case 3:
			static bool desync_flip = false;
			static bool flip = false;
			desync_flip = !desync_flip;





			cmd->viewangles.y -= 180 + (desync_flip ? -MaxDesyndDeltaMenu : MaxDesyndDeltaMenu);
			break;
		}


	}
	*/
	//while testing on hvh i noticed than you are slowwalking desync works perefectly, so lets do a little movement while standing
	/*
if (g::pLocalEntity->GetFlags() & FL_ONGROUND && g::pCmd->sidemove < 3 && g::pCmd->sidemove > -3)
	{
		static bool switch_ = false;
		if (switch_)
			g::pCmd->sidemove = 2;
		else
			g::pCmd->sidemove = -2;
		switch_ = !switch_;
	}
	/*



















	if (g::bSendPacket)
	{




	}

	if (Options::Menu.AntiAimTab.StaticBodyYaw.GetState())
		StaticBodyYaw(g::pCmd);
	*/
}

void c_antiaim::fakelag(C_BaseEntity* local, CUserCmd* cmd, bool& send_packet)
{
	if (!local)
		return;

	auto anim_state = local->AnimState();

	if (!anim_state)
		return;
	if (g_Options.antiaa.double_tap &&
        !AntiAimPro.is_fakeducking &&
        (g_Options.antiaa.double_tap_on_key &&
            g_Options.antiaa.double_tap_method == 1 &&
            GetAsyncKeyState(g_Options.antiaa.double_tap_key))
        || (g_Options.antiaa.double_tap_on_key &&
            g_Options.antiaa.double_tap_method == 0 &&
            overridething_dt_key))
	{
		
		g::bSendPacket = cmd->command_number % 2;

		
	}
	else
	{
		if (!g_Options.antiaa.fakelag)
		{
			g::bSendPacket = cmd->command_number % 2;

			return;
		}

		if (g_Options.antiaa.fake_lag_air == 0 )
		{
			if (!local->is_on_ground())
			{
			

				g::bSendPacket = cmd->command_number % 2;

			}
			
		}
		if (g_Options.antiaa.fake_lag_moving == 0 )
		{
			if (local->GetVelocity().Length() > 0 && local->GetFlags() & FL_ONGROUND)
			{

			

				g::bSendPacket = cmd->command_number % 2;


			}
		}
		else if(g_Options.antiaa.fake_lag_standing == 0)
		{
			
			
			if (!local->is_on_ground())
			{
			}
			else if (local->GetVelocity().Length() > 0 && local->GetFlags() & FL_ONGROUND)
			{
			}
			else
			{
				g::bSendPacket = cmd->command_number % 2;


			}


			
		
		}

		
		if (!local->is_on_ground())
		{
			if (g_Options.antiaa.fake_lag_air == 0)
			{
				return;
			}
			auto target_standing_custom = g_Options.antiaa.fake_lag_air;

			static auto last_origin = Vector();
			static auto last_simtime = 0.f;
			static auto last_ducked = false;

			static auto unduck_timer = 0.f;


			auto choke_amount = target_standing_custom;

			if (!local || !local->IsAlive() || !cmd)
			{
				choke_amount = 0;
				estimated_choke = 0;
				return;
			}
			const auto weapon = local->GetActiveWeapon();


			if (!weapon)
			{
				choke_amount = 0;
				estimated_choke = 0;
				return;
			}





			choke_amount = target_standing_custom;





			// are we in unduck?
			if (unduck_timer > g_pGlobalVars->curtime + ticks_to_time(16))
				unduck_timer = 0.f;

			// extend fake lag to maximum.

			// force enemy to extrapolate us.
			if (last_simtime == g_pGlobalVars->curtime - g_pGlobalVars->intervalPerTick && (local->GetOrigin() - last_origin).Length2DSqr() > 4096.f)
				choke_amount = 1;



			if (local->GetFlags() & FL_ONGROUND && !(cmd->buttons & IN_DUCK) && last_ducked)
			{
				if (unduck_timer < g_pGlobalVars->curtime)
				{
					unduck_timer = g_pGlobalVars->curtime + ticks_to_time(16);

					if (g_pClientState->choked_commands > 1)
					{
						cmd->buttons |= IN_DUCK;
						choke_amount = 1;
					}
				}
			}

			if (unduck_timer > g_pGlobalVars->curtime - ticks_to_time(1) && !g_pClientState->choked_commands)
				cmd->buttons |= IN_DUCK;

			// fake land to desync animations for pasted cheats.
			auto origin = local->GetOrigin();
			auto velocity = local->GetVelocity();
			auto flags = local->GetFlags();







			if (GetAsyncKeyState(g_Options.antiaa.fake_duck_key))
			{
				AntiAimPro.is_fakeducking = true;
				choke_amount = 14;
			}
			else
				AntiAimPro.is_fakeducking = false;


			// set send packet and stats.
			const auto shot_last_tick = AntiAimPro.shot_cmd <= cmd->command_number && AntiAimPro.shot_cmd > cmd->command_number - g_pClientState->choked_commands;
			send_packet = g_pClientState->choked_commands >= choke_amount || (!AntiAimPro.is_fakeducking && shot_last_tick);

			estimated_choke = choke_amount;

			// store data of current tick for next evaluation.
			last_origin = local->GetOrigin();
			last_simtime = g_pGlobalVars->curtime;
			last_ducked = cmd->buttons & IN_DUCK;
		}
		else if (local->GetVelocity().Length() > 0 && local->GetFlags() & FL_ONGROUND)
		{
		

			if (g_Options.antiaa.fake_lag_moving == 0)
			{
				return;
			}
			auto target_standing_custom = g_Options.antiaa.fake_lag_moving;

			static auto last_origin = Vector();
			static auto last_simtime = 0.f;
			static auto last_ducked = false;

			static auto unduck_timer = 0.f;


			auto choke_amount = target_standing_custom;

			if (!local || !local->IsAlive() || !cmd)
			{
				choke_amount = 0;
				estimated_choke = 0;
				return;
			}
			const auto weapon = local->GetActiveWeapon();


			if (!weapon)
			{
				choke_amount = 0;
				estimated_choke = 0;
				return;
			}





			choke_amount = target_standing_custom;





			// are we in unduck?
			if (unduck_timer > g_pGlobalVars->curtime + ticks_to_time(16))
				unduck_timer = 0.f;

			// extend fake lag to maximum.

			// force enemy to extrapolate us.
			if (last_simtime == g_pGlobalVars->curtime - g_pGlobalVars->intervalPerTick && (local->GetOrigin() - last_origin).Length2DSqr() > 4096.f)
				choke_amount = 1;



			if (local->GetFlags() & FL_ONGROUND && !(cmd->buttons & IN_DUCK) && last_ducked)
			{
				if (unduck_timer < g_pGlobalVars->curtime)
				{
					unduck_timer = g_pGlobalVars->curtime + ticks_to_time(16);

					if (g_pClientState->choked_commands > 1)
					{
						cmd->buttons |= IN_DUCK;
						choke_amount = 1;
					}
				}
			}

			if (unduck_timer > g_pGlobalVars->curtime - ticks_to_time(1) && !g_pClientState->choked_commands)
				cmd->buttons |= IN_DUCK;

			// fake land to desync animations for pasted cheats.
			auto origin = local->GetOrigin();
			auto velocity = local->GetVelocity();
			auto flags = local->GetFlags();







			if (GetAsyncKeyState(g_Options.antiaa.fake_duck_key))
			{
				AntiAimPro.is_fakeducking = true;
				choke_amount = 14;
			}
			else
				AntiAimPro.is_fakeducking = false;


			// set send packet and stats.
			const auto shot_last_tick = AntiAimPro.shot_cmd <= cmd->command_number && AntiAimPro.shot_cmd > cmd->command_number - g_pClientState->choked_commands;
			send_packet = g_pClientState->choked_commands >= choke_amount || (!AntiAimPro.is_fakeducking && shot_last_tick);

			estimated_choke = choke_amount;

			// store data of current tick for next evaluation.
			last_origin = local->GetOrigin();
			last_simtime = g_pGlobalVars->curtime;
			last_ducked = cmd->buttons & IN_DUCK;


		}
		else
		{
			if (g_Options.antiaa.fake_lag_standing == 0)
			{
				return;
			}

			auto target_standing_custom = g_Options.antiaa.fake_lag_standing;

			static auto last_origin = Vector();
			static auto last_simtime = 0.f;
			static auto last_ducked = false;

			static auto unduck_timer = 0.f;


			auto choke_amount = target_standing_custom;

			if (!local || !local->IsAlive() || !cmd)
			{
				choke_amount = 0;
				estimated_choke = 0;
				return;
			}
			const auto weapon = local->GetActiveWeapon();


			if (!weapon)
			{
				choke_amount = 0;
				estimated_choke = 0;
				return;
			}





			choke_amount = target_standing_custom;





			// are we in unduck?
			if (unduck_timer > g_pGlobalVars->curtime + ticks_to_time(16))
				unduck_timer = 0.f;

			// extend fake lag to maximum.

			// force enemy to extrapolate us.
			if (last_simtime == g_pGlobalVars->curtime - g_pGlobalVars->intervalPerTick && (local->GetOrigin() - last_origin).Length2DSqr() > 4096.f)
				choke_amount = 1;



			if (local->GetFlags() & FL_ONGROUND && !(cmd->buttons & IN_DUCK) && last_ducked)
			{
				if (unduck_timer < g_pGlobalVars->curtime)
				{
					unduck_timer = g_pGlobalVars->curtime + ticks_to_time(16);

					if (g_pClientState->choked_commands > 1)
					{
						cmd->buttons |= IN_DUCK;
						choke_amount = 1;
					}
				}
			}

			if (unduck_timer > g_pGlobalVars->curtime - ticks_to_time(1) && !g_pClientState->choked_commands)
				cmd->buttons |= IN_DUCK;

			// fake land to desync animations for pasted cheats.
			auto origin = local->GetOrigin();
			auto velocity = local->GetVelocity();
			auto flags = local->GetFlags();







			if (GetAsyncKeyState(g_Options.antiaa.fake_duck_key))
			{
				AntiAimPro.is_fakeducking = true;
				choke_amount = 14;
			}
			else
				AntiAimPro.is_fakeducking = false;


			// set send packet and stats.
			const auto shot_last_tick = AntiAimPro.shot_cmd <= cmd->command_number && AntiAimPro.shot_cmd > cmd->command_number - g_pClientState->choked_commands;
			send_packet = g_pClientState->choked_commands >= choke_amount || (!AntiAimPro.is_fakeducking && shot_last_tick);

			estimated_choke = choke_amount;

			// store data of current tick for next evaluation.
			last_origin = local->GetOrigin();
			last_simtime = g_pGlobalVars->curtime;
			last_ducked = cmd->buttons & IN_DUCK;
		}
		
		

	
	}

	
}

void c_antiaim::predict(C_BaseEntity* local, CUserCmd* cmd)
{
	const auto state = local->AnimState();

	if (!local->is_local_player() || !state)
		return;

	const auto time = ticks_to_time(local->GetTickBase());

	if (local->GetVelocity().Length2D() >= .1f || fabsf(local->GetVelocity().z) >= 100.f)
	{
		next_lby_update = time + .22f;
	}
	else if (time > next_lby_update)
	{
		lby_update = time;
		next_lby_update = time + 1.1f;
	}

}

void c_antiaim::run(C_BaseEntity* local, CUserCmd* cmd, bool& send_packet)
{
	if (!local || !cmd || !g_Input)
		return;

	auto anim_state = local->AnimState();


	const auto weapon = local->GetActiveWeapon();
	if (!weapon || !anim_state)
		return;

    auto aa_list = g_Options.antiaa.disable_aa_selected;
	//std::vector<dropdownboxitem> aa_list = Options::Menu.AntiAimTab.disable_aa.items;
	if (aa_list[0])
	{
		if (IsWeaponGrenade4(weapon))
		{
			return;

		}

	}
	if (aa_list[1])
	{
		if (g::warmup)
		{
			return;

		}

	}
	if (IsWeaponGrenade4(weapon))
	{
		if (!weapon->IsPinPulled() || (g::pCmd->buttons & IN_ATTACK) || (g::pCmd->buttons & IN_ATTACK2))
		{
			float throwTime = weapon->GetThrowTime();

			if (throwTime > 0)
				return;
		}
	}

	if (weapon->IsKnife() && g::pCmd->buttons & IN_ATTACK2)
		return;

	if (g::pLocalEntity->GetMoveType() == 9)
		return;

	if (g::pCmd->buttons & IN_USE)
		return;
	if (g::pCmd->buttons & IN_ATTACK)
		return;

	if (!weapon ||
		local->GetMoveType() == move_type::movetype_observer ||
		local->GetMoveType() == move_type::movetype_noclip ||
		local->GetMoveType() == move_type::movetype_ladder || !g_Options.antiaa.enable)
	{
		return;
	}
	float gaysid132e;
	static bool side = false;

	float MaxDelta;
	if (g::pLocalEntity->DesyncValue() > g_Options.antiaa.stand.desync)
	{
		if (g_Options.antiaa.stand.desync > g::pLocalEntity->DesyncValue())
			MaxDelta = g::pLocalEntity->DesyncValue();
		else
		{
			MaxDelta = g_Options.antiaa.stand.desync;

		}
	}
	else
	{
		MaxDelta = g::pLocalEntity->DesyncValue();

	}

	bool throw_nade = false;

	if (weapon->is_grenade())
		throw_nade = !weapon->IsPinPulled() && weapon->GetThrowTime() > 0.f && weapon->GetThrowTime() < g_pGlobalVars->curtime;

	if (throw_nade || cmd->buttons & IN_USE)
	{
		return;
	}

	//if (cmd->buttons & IN_ATTACK)
	//{
	//	return;
//	}
	if (send_packet)
	{
		


		switch (ReturnMovementType())
		{
		case MovingAAType::Standing:
		{
			switch (g_Options.antiaa.stand.pitch_selected)
			{
			case 1: g::pCmd->viewangles.x = 89.9f;
				break;
			case 2: g::pCmd->viewangles.x = -89.9f;
				break;
			case 3: g::pCmd->viewangles.x = 0;
				break;
			}
		}break;
		case MovingAAType::Moving:
		{
			switch (g_Options.antiaa.moving.pitch_selected)
			{
			case 1: g::pCmd->viewangles.x = 89.9f;
				break;
			case 2: g::pCmd->viewangles.x = -89.9f;
				break;
			case 3: g::pCmd->viewangles.x = 0;
				break;
			}
		}break;
		case MovingAAType::Air:
		{
			switch (g_Options.antiaa.air.pitch_selected)
			{
			case 1: g::pCmd->viewangles.x = 89.9f;
				break;
			case 2: g::pCmd->viewangles.x = -89.9f;
				break;
			case 3: g::pCmd->viewangles.x = 0;
				break;
			}
		}break;

		case MovingAAType::Slowwalk:
		{
			switch (g_Options.antiaa.slow.pitch_selected)
			{
			case 1: g::pCmd->viewangles.x = 89.9f;
				break;
			case 2: g::pCmd->viewangles.x = -89.9f;
				break;
			case 3: g::pCmd->viewangles.x = 0;
				break;
			}

		}break;


		}
		switch (ReturnMovementType())
		{
		case MovingAAType::Standing:
		{
			if (g_Options.antiaa.stand.body_yaw_key)
			{
				side = false;
				if (GetKeyState(g_Options.antiaa.stand.body_yaw_key))
				{
					side = true;
				}
			}

			if (side)
			{
				gaysid132e = 1;

			}
			else
			{
				gaysid132e = -1;

			}

			switch (g_Options.antiaa.stand.body_yaw_selected)
			{
			case 0:g::pCmd->viewangles.y = g::RealAngle.y;
				break;
			case 1: g::pCmd->viewangles.y = g::RealAngle.y - MaxDelta * gaysid132e;
				break;
			case 2:
			{

				static bool desync_flip = false;
				desync_flip = !desync_flip;

				cmd->viewangles.y = g::RealAngle.y + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);

			}
			break;
			case 3:
			{
				static bool desync_flip = false;
				desync_flip = !desync_flip;

				g::pCmd->viewangles.y += 180;



				if (g_Options.antiaa.invert_key)
				{
					side2 = false;
					if (GetKeyState(g_Options.antiaa.invert_key))
					{
						side2 = true;
					}
				}

				if (side2)
				{
					cmd->viewangles.y -= 90;
					g::pCmd->viewangles.y += g_Options.antiaa.stand.body_lean;

				}
				else
				{
					cmd->viewangles.y += 90;
					g::pCmd->viewangles.y += g_Options.antiaa.stand.inverted_body_lean;

				}
				cmd->viewangles.y -= 180.0f + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);
			}
			break;
			}

		}break;
		case MovingAAType::Moving:
		{
            if (g_Options.antiaa.moving.body_yaw_key)
            {
                side = false;
                if (GetKeyState(g_Options.antiaa.moving.body_yaw_key))
                {
                    side = true;
                }
            }

            if (side)
            {
                gaysid132e = 1;

            }
            else
            {
                gaysid132e = -1;

            }

            switch (g_Options.antiaa.moving.body_yaw_selected)
            {
            case 0:g::pCmd->viewangles.y = g::RealAngle.y;
                break;
            case 1: g::pCmd->viewangles.y = g::RealAngle.y - MaxDelta * gaysid132e;
                break;
            case 2:
            {

                static bool desync_flip = false;
                desync_flip = !desync_flip;

                cmd->viewangles.y = g::RealAngle.y + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);

            }
            break;
            case 3:
            {
                static bool desync_flip = false;
                desync_flip = !desync_flip;

                g::pCmd->viewangles.y += 180;



                if (g_Options.antiaa.invert_key)
                {
                    side2 = false;
                    if (GetKeyState(g_Options.antiaa.invert_key))
                    {
                        side2 = true;
                    }
                }

                if (side2)
                {
                    cmd->viewangles.y -= 90;
                    g::pCmd->viewangles.y += g_Options.antiaa.moving.body_lean;

                }
                else
                {
                    cmd->viewangles.y += 90;
                    g::pCmd->viewangles.y += g_Options.antiaa.moving.inverted_body_lean;

                }
                cmd->viewangles.y -= 180.0f + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);
            }
            break;
            }
		}break;
		case MovingAAType::Air:
		{
            if (g_Options.antiaa.air.body_yaw_key)
            {
                side = false;
                if (GetKeyState(g_Options.antiaa.air.body_yaw_key))
                {
                    side = true;
                }
            }

            if (side)
            {
                gaysid132e = 1;

            }
            else
            {
                gaysid132e = -1;

            }

            switch (g_Options.antiaa.air.body_yaw_selected)
            {
            case 0:g::pCmd->viewangles.y = g::RealAngle.y;
                break;
            case 1: g::pCmd->viewangles.y = g::RealAngle.y - MaxDelta * gaysid132e;
                break;
            case 2:
            {

                static bool desync_flip = false;
                desync_flip = !desync_flip;

                cmd->viewangles.y = g::RealAngle.y + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);

            }
            break;
            case 3:
            {
                static bool desync_flip = false;
                desync_flip = !desync_flip;

                g::pCmd->viewangles.y += 180;



                if (g_Options.antiaa.invert_key)
                {
                    side2 = false;
                    if (GetKeyState(g_Options.antiaa.invert_key))
                    {
                        side2 = true;
                    }
                }

                if (side2)
                {
                    cmd->viewangles.y -= 90;
                    g::pCmd->viewangles.y += g_Options.antiaa.air.body_lean;

                }
                else
                {
                    cmd->viewangles.y += 90;
                    g::pCmd->viewangles.y += g_Options.antiaa.air.inverted_body_lean;

                }
                cmd->viewangles.y -= 180.0f + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);
            }
            break;
            }
		}break;
		case MovingAAType::Slowwalk:
		{

            if (g_Options.antiaa.slow.body_yaw_key)
            {
                side = false;
                if (GetKeyState(g_Options.antiaa.slow.body_yaw_key))
                {
                    side = true;
                }
            }

            if (side)
            {
                gaysid132e = 1;

            }
            else
            {
                gaysid132e = -1;

            }

            switch (g_Options.antiaa.slow.body_yaw_selected)
            {
            case 0:g::pCmd->viewangles.y = g::RealAngle.y;
                break;
            case 1: g::pCmd->viewangles.y = g::RealAngle.y - MaxDelta * gaysid132e;
                break;
            case 2:
            {

                static bool desync_flip = false;
                desync_flip = !desync_flip;

                cmd->viewangles.y = g::RealAngle.y + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);

            }
            break;
            case 3:
            {
                static bool desync_flip = false;
                desync_flip = !desync_flip;

                g::pCmd->viewangles.y += 180;



                if (g_Options.antiaa.invert_key)
                {
                    side2 = false;
                    if (GetKeyState(g_Options.antiaa.invert_key))
                    {
                        side2 = true;
                    }
                }

                if (side2)
                {
                    cmd->viewangles.y -= 90;
                    g::pCmd->viewangles.y += g_Options.antiaa.slow.body_lean;

                }
                else
                {
                    cmd->viewangles.y += 90;
                    g::pCmd->viewangles.y += g_Options.antiaa.slow.inverted_body_lean;

                }
                cmd->viewangles.y -= 180.0f + (desync_flip ? -MaxDelta * gaysid132e : MaxDelta * gaysid132e);
            }
            break;
            }
		}break;


		}
		AntiAimPro.last_real = cmd->viewangles.y;
		


	}
	else
	{
	

			switch (ReturnMovementType())
			{
			case MovingAAType::Standing:
			{
				switch (g_Options.antiaa.stand.pitch_selected)
				{
				case 1: g::pCmd->viewangles.x = 89.9f;
					break;
				case 2: g::pCmd->viewangles.x = -89.9f;
					break;
				case 3: g::pCmd->viewangles.x = 0;
					break;
				}
			}break;
			case MovingAAType::Moving:
			{
				switch (g_Options.antiaa.moving.pitch_selected)
				{
				case 1: g::pCmd->viewangles.x = 89.9f;
					break;
				case 2: g::pCmd->viewangles.x = -89.9f;
					break;
				case 3: g::pCmd->viewangles.x = 0;
					break;
				}
			}break;
			case MovingAAType::Air:
			{
				switch (g_Options.antiaa.air.pitch_selected)
				{
				case 1: g::pCmd->viewangles.x = 89.9f;
					break;
				case 2: g::pCmd->viewangles.x = -89.9f;
					break;
				case 3: g::pCmd->viewangles.x = 0;
					break;
				}
			}break;

			case MovingAAType::Slowwalk:
			{
				switch (g_Options.antiaa.slow.pitch_selected)
				{
				case 1: g::pCmd->viewangles.x = 89.9f;
					break;
				case 2: g::pCmd->viewangles.x = -89.9f;
					break;
				case 3: g::pCmd->viewangles.x = 0;
					break;
				}

			}break;


			}



			switch (ReturnMovementType())
			{
			case MovingAAType::Standing:
			{
				int
					index,
					base_angle,
					offset;


				index = g_Options.antiaa.stand.yaw_selected;
				base_angle = g_Options.antiaa.stand.yaw_base_selected;
				offset = g_Options.antiaa.stand.offset;



				g::pCmd->viewangles.y = 0.0f;

				if (base_angle == 1)
				{
					Vector angles;
					g_pEngine->GetViewAngles(angles);
					g::pCmd->viewangles.y = angles.y;
				}

				if (base_angle == 2)
					g::pCmd->viewangles.y = AntiAimNEW.at_target();

				switch (index)
				{

				case 1:
					g::pCmd->viewangles.y += 90.0f;
					break;
				case 2:
					g::pCmd->viewangles.y += 180;
					break;
				case 3:
					g::pCmd->viewangles.y += g_pGlobalVars->curtime * 420.0f;
					break;
				case 4:
				{


					static bool bSwitch = false;

					if (bSwitch)
					{
						g::pCmd->viewangles.y += 180 + g_Options.antiaa.stand.jitter;
						bSwitch = false;
					}
					else
					{
						g::pCmd->viewangles.y += 180 + -g_Options.antiaa.stand.jitter;
						bSwitch = true;
					}
				}
				break;
				}

				switch (g_Options.antiaa.stand.edge_type_selected)
				{
				case 0:

					break;
				case 1:


					if (g_Options.antiaa.invert_key)
					{
						side2 = false;
						if (GetKeyState(g_Options.antiaa.invert_key))
						{
							side2 = true;
						}
					}

					if (side2)
					{
						cmd->viewangles.y -= 90;

					}
					else
					{
						cmd->viewangles.y += 90;

					}




					break;
				case 2:


					FreeStanding();





					break;
				}

				g::pCmd->viewangles.y += offset;

				if (g_Options.antiaa.stand.edge_type_selected == 1)
				{
					if (g_Options.antiaa.invert_key)
					{
						side2 = false;
						if (GetKeyState(g_Options.antiaa.invert_key))
						{
							side2 = true;
						}
					}

					if (side2)
					{
						g::pCmd->viewangles.y += g_Options.antiaa.stand.body_lean;

					}
					else
					{
						g::pCmd->viewangles.y += g_Options.antiaa.stand.inverted_body_lean;

					}
				}
			}break;
			case MovingAAType::Moving:
			{
                int
                    index,
                    base_angle,
                    offset;


                index = g_Options.antiaa.moving.yaw_selected;
                base_angle = g_Options.antiaa.moving.yaw_base_selected;
                offset = g_Options.antiaa.moving.offset;



                g::pCmd->viewangles.y = 0.0f;

                if (base_angle == 1)
                {
                    Vector angles;
                    g_pEngine->GetViewAngles(angles);
                    g::pCmd->viewangles.y = angles.y;
                }

                if (base_angle == 2)
                    g::pCmd->viewangles.y = AntiAimNEW.at_target();

                switch (index)
                {

                case 1:
                    g::pCmd->viewangles.y += 90.0f;
                    break;
                case 2:
                    g::pCmd->viewangles.y += 180;
                    break;
                case 3:
                    g::pCmd->viewangles.y += g_pGlobalVars->curtime * 420.0f;
                    break;
                case 4:
                {


                    static bool bSwitch = false;

                    if (bSwitch)
                    {
                        g::pCmd->viewangles.y += 180 + g_Options.antiaa.moving.jitter;
                        bSwitch = false;
                    }
                    else
                    {
                        g::pCmd->viewangles.y += 180 + -g_Options.antiaa.moving.jitter;
                        bSwitch = true;
                    }
                }
                break;
                }

                switch (g_Options.antiaa.moving.edge_type_selected)
                {
                case 0:

                    break;
                case 1:


                    if (g_Options.antiaa.invert_key)
                    {
                        side2 = false;
                        if (GetKeyState(g_Options.antiaa.invert_key))
                        {
                            side2 = true;
                        }
                    }

                    if (side2)
                    {
                        cmd->viewangles.y -= 90;

                    }
                    else
                    {
                        cmd->viewangles.y += 90;

                    }




                    break;
                case 2:


                    FreeStanding();





                    break;
                }

                g::pCmd->viewangles.y += offset;

                if (g_Options.antiaa.moving.edge_type_selected == 1)
                {
                    if (g_Options.antiaa.invert_key)
                    {
                        side2 = false;
                        if (GetKeyState(g_Options.antiaa.invert_key))
                        {
                            side2 = true;
                        }
                    }

                    if (side2)
                    {
                        g::pCmd->viewangles.y += g_Options.antiaa.moving.body_lean;

                    }
                    else
                    {
                        g::pCmd->viewangles.y += g_Options.antiaa.moving.inverted_body_lean;

                    }
                }
			}break;
			case MovingAAType::Air:
			{
                int
                    index,
                    base_angle,
                    offset;


                index = g_Options.antiaa.air.yaw_selected;
                base_angle = g_Options.antiaa.air.yaw_base_selected;
                offset = g_Options.antiaa.air.offset;



                g::pCmd->viewangles.y = 0.0f;

                if (base_angle == 1)
                {
                    Vector angles;
                    g_pEngine->GetViewAngles(angles);
                    g::pCmd->viewangles.y = angles.y;
                }

                if (base_angle == 2)
                    g::pCmd->viewangles.y = AntiAimNEW.at_target();

                switch (index)
                {

                case 1:
                    g::pCmd->viewangles.y += 90.0f;
                    break;
                case 2:
                    g::pCmd->viewangles.y += 180;
                    break;
                case 3:
                    g::pCmd->viewangles.y += g_pGlobalVars->curtime * 420.0f;
                    break;
                case 4:
                {


                    static bool bSwitch = false;

                    if (bSwitch)
                    {
                        g::pCmd->viewangles.y += 180 + g_Options.antiaa.air.jitter;
                        bSwitch = false;
                    }
                    else
                    {
                        g::pCmd->viewangles.y += 180 + -g_Options.antiaa.air.jitter;
                        bSwitch = true;
                    }
                }
                break;
                }

                switch (g_Options.antiaa.air.edge_type_selected)
                {
                case 0:

                    break;
                case 1:


                    if (g_Options.antiaa.invert_key)
                    {
                        side2 = false;
                        if (GetKeyState(g_Options.antiaa.invert_key))
                        {
                            side2 = true;
                        }
                    }

                    if (side2)
                    {
                        cmd->viewangles.y -= 90;

                    }
                    else
                    {
                        cmd->viewangles.y += 90;

                    }




                    break;
                case 2:


                    FreeStanding();





                    break;
                }

                g::pCmd->viewangles.y += offset;

                if (g_Options.antiaa.air.edge_type_selected == 1)
                {
                    if (g_Options.antiaa.invert_key)
                    {
                        side2 = false;
                        if (GetKeyState(g_Options.antiaa.invert_key))
                        {
                            side2 = true;
                        }
                    }

                    if (side2)
                    {
                        g::pCmd->viewangles.y += g_Options.antiaa.air.body_lean;

                    }
                    else
                    {
                        g::pCmd->viewangles.y += g_Options.antiaa.air.inverted_body_lean;

                    }
                }
			}break;

			case MovingAAType::Slowwalk:
			{
                int
                    index,
                    base_angle,
                    offset;


                index = g_Options.antiaa.slow.yaw_selected;
                base_angle = g_Options.antiaa.slow.yaw_base_selected;
                offset = g_Options.antiaa.slow.offset;



                g::pCmd->viewangles.y = 0.0f;

                if (base_angle == 1)
                {
                    Vector angles;
                    g_pEngine->GetViewAngles(angles);
                    g::pCmd->viewangles.y = angles.y;
                }

                if (base_angle == 2)
                    g::pCmd->viewangles.y = AntiAimNEW.at_target();

                switch (index)
                {

                case 1:
                    g::pCmd->viewangles.y += 90.0f;
                    break;
                case 2:
                    g::pCmd->viewangles.y += 180;
                    break;
                case 3:
                    g::pCmd->viewangles.y += g_pGlobalVars->curtime * 420.0f;
                    break;
                case 4:
                {


                    static bool bSwitch = false;

                    if (bSwitch)
                    {
                        g::pCmd->viewangles.y += 180 + g_Options.antiaa.slow.jitter;
                        bSwitch = false;
                    }
                    else
                    {
                        g::pCmd->viewangles.y += 180 + -g_Options.antiaa.slow.jitter;
                        bSwitch = true;
                    }
                }
                break;
                }

                switch (g_Options.antiaa.slow.edge_type_selected)
                {
                case 0:

                    break;
                case 1:


                    if (g_Options.antiaa.invert_key)
                    {
                        side2 = false;
                        if (GetKeyState(g_Options.antiaa.invert_key))
                        {
                            side2 = true;
                        }
                    }

                    if (side2)
                    {
                        cmd->viewangles.y -= 90;

                    }
                    else
                    {
                        cmd->viewangles.y += 90;

                    }




                    break;
                case 2:


                    FreeStanding();





                    break;
                }

                g::pCmd->viewangles.y += offset;

                if (g_Options.antiaa.slow.edge_type_selected == 1)
                {
                    if (g_Options.antiaa.invert_key)
                    {
                        side2 = false;
                        if (GetKeyState(g_Options.antiaa.invert_key))
                        {
                            side2 = true;
                        }
                    }

                    if (side2)
                    {
                        g::pCmd->viewangles.y += g_Options.antiaa.slow.body_lean;

                    }
                    else
                    {
                        g::pCmd->viewangles.y += g_Options.antiaa.slow.inverted_body_lean;

                    }
                }

			}break;


			}



			last_fake = cmd->viewangles.y;
		
	}
}


void antiaim::fake_crouch()
{



	if (g_Options.antiaa.fake_duck)
	{

		if (GetAsyncKeyState(g_Options.antiaa.fake_duck_key))
		{



			if (!g_pEngine->GetNetChannel()) return;
			//if (!g_pEngine->GetNetChannel()) return;


			g::pCmd->buttons |= IN_BULLRUSH;

			if (g_pEngine->GetNetChannel()->m_nChokedPackets <= 6)
				g::pCmd->buttons &= ~IN_DUCK;
			else
				g::pCmd->buttons |= IN_DUCK;

			if (g_pEngine->GetNetChannel()->m_nChokedPackets < 14)
				g::bSendPacket = false;   // choke
			else
				g::bSendPacket = true;    // send packet


		}
	}

}
inline float FastSq23rt(float x) {

	unsigned int i = *(unsigned int*)&x;
	i += 127 << 23;
	i >>= 1;

	return *(float*)&i;
}
#define square( x ) ( x * x )

void antiaim::FakeWalk(CUserCmd* pCmd) // holy this hurt a lot 
{

	g::fake_walk = false;
	if (g_Options.antiaa.slow_walk)
	{

		if (GetAsyncKeyState(g_Options.antiaa.slow_walk_key))
		{
			g::fake_walk = true;
			auto get_speed = g_Options.antiaa.slow_speed;

			float min_speed = (float)(FastSq23rt(square(pCmd->forwardmove) + square(pCmd->sidemove) + square(pCmd->upmove)));
			if (min_speed <= 0.f)
				return;

			if (pCmd->buttons & IN_DUCK)
				get_speed *= 2.94117647f;

			if (min_speed <= get_speed)
				return;

			float kys = get_speed / min_speed;
			pCmd->forwardmove *= kys;
			pCmd->sidemove *= kys;
			pCmd->upmove *= kys;
		}
	}


}


















int CFakeLag::Fakelag_AdaptiveFactor()
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!local_player)
		return 0;
	int choked_ticks;
	auto velocity = local_player->GetVelocity();;
	auto speed = velocity.Length();
	auto distance_per_tick = speed *
		g_pGlobalVars->intervalPerTick;
	choked_ticks = std::ceilf(TELEPORT_DISTANCE / distance_per_tick);
	return std::min<int>(choked_ticks, MAX_CHOKE);
}
int iChoke = 1;

void CFakeLag::do_fakelag(CUserCmd* cmd, C_BaseEntity* local)
{
	if (g_pEngine->IsVoiceRecording())
		return;


	if (cmd->buttons & IN_ATTACK)
		return;

	auto NetChannel = g_pEngine->GetNetChannel();

	if (!NetChannel)
		return;


	bool flag1 = true;
	bool flag2 = true;
	bool flag3 = true;
	bool flag4 = true;

	int fakelagVal;

	fakelagVal = g_Options.antiaa.fake_lag_moving;

	if (flag1 || flag2 || flag3 || flag4)
		iChoke = fakelagVal;
	else
		return;


	if (NetChannel->m_nChokedPackets < iChoke)
	{
		g::bSendPacket = false;
	}
	else
	{
		g::bSendPacket = true;
	}


}

CFakeLag* fakelag = new CFakeLag();
