#include "CEntity.h"
#include "IVModelInfo.h"
#include "CGlobalVarsBase.h"
#include "..\Utils\GlobalVars.h"
#include "..\Utils\Math.h"
#include "../SDK/ClientClass.h"
#include "xmmintrin.h"
constexpr auto VALVE_ADDED_FUNCS = 0ull;
PlayerInfo_t C_BaseEntity::GetPlayerInfo()
{
    PlayerInfo_t info;
    g_pEngine->GetPlayerInfo(EntIndex(), &info);
    return info;
}

bool C_BaseEntity::IsLoot() {
    return GetClientClass()->ClassID == ClassId::CPhysPropAmmoBox ||
        GetClientClass()->ClassID == ClassId::CPhysPropLootCrate ||
        GetClientClass()->ClassID == ClassId::CPhysPropRadarJammer ||
        GetClientClass()->ClassID == ClassId::CPhysPropWeaponUpgrade ||
        GetClientClass()->ClassID == ClassId::CDrone ||
        GetClientClass()->ClassID == ClassId::CDronegun ||
        GetClientClass()->ClassID == ClassId::CItem_Healthshot ||
        GetClientClass()->ClassID == ClassId::CItemCash ||
        GetClientClass()->ClassID == ClassId::CBumpMine;
}
float curtime(const int& tick_base)
{
    return g_pGlobalVars->intervalPerTick * tick_base;
}


bool C_BaseEntity::IsFlashed() {
	static auto m_flFlashMaxAlpha = NetvarManager::Instance()->GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
	return *(float*)((uintptr_t)this + m_flFlashMaxAlpha - 0x8) > 200.0;
}
bool C_BaseEntity::IsWeapon()
{
    //index: 160
    //ref: "CNewParticleEffect::DrawModel"
    //sig: 8B 80 ? ? ? ? FF D0 84 C0 74 6F 8B 4D A4
    return qCallVFunction<bool(__thiscall*)(C_BaseEntity*)>(this, 165 + VALVE_ADDED_FUNCS)(this);
}
bool& C_BaseEntity::m_bSpotted()
{
	//("DT_CSPlayer", "m_bSpotted");
	static auto m_bSpotted = NetvarManager::Instance()->GetOffset("DT_CSPlayer", "m_bSpotted");
	return *(bool*)(uintptr_t(this) + m_bSpotted);
}

void C_BaseAttributableItem::SetGloveModelIndex(int modelIndex)
{
    return qCallVFunction<void(__thiscall*)(void*, int)>(this, 75)(this, modelIndex);
}

bool C_BaseEntity::IsPlantedC4()
{
    return GetClientClass()->ClassID == ClassId::CPlantedC4;
}

bool C_BaseEntity::IsDefuseKit()
{
    return GetClientClass()->ClassID == ClassId::CBaseAnimating;
}
bool C_BaseEntity::is_enemy()
{ 
	return this->GetTeam() != g::pLocalEntity->GetTeam();
}

bool C_BaseEntity::is_local_player() const
{
	return EntIndex() == g_pEngine->GetLocalPlayer();
}

bool C_BaseEntity::IsPlayer()
{
	if (!this)
	{
		return false;
	}
	ClientClass* cClass = (ClientClass*)this->GetClientClass();
	if (!cClass)
		return false;
	if (cClass->ClassID != 40)
	{
		return false;
	}
	return true;
}
void C_BaseEntity::ResetAnimationState(C_AnimState *state) 
{
	using ResetAnimState_t = void(__thiscall*)(C_AnimState*);
	static auto ResetAnimState = (ResetAnimState_t)Utils::FindSignature(("client_panorama.dll"), "56 6A 01 68 ? ? ? ? 8B F1");
	if (!ResetAnimState)
		return;

	ResetAnimState(state);
}
std::array<float, 24> &C_BaseEntity::m_flPoseParameterNEW()
{
	static int _m_flPoseParameter = NetvarManager::Instance()->GetOffset("DT_BaseAnimating", "m_flPoseParameter");

	return *(std::array<float, 24>*)((uintptr_t)this + _m_flPoseParameter);
}
void C_BaseEntity::CreateAnimationState(C_AnimState *state)
{
	using CreateAnimState_t = void(__thiscall*)(C_AnimState*, C_BaseEntity*);
	static auto CreateAnimState = (CreateAnimState_t)Utils::FindSignature(("client_panorama.dll"), "55 8B EC 56 8B F1 B9 ? ? ? ? C7 46");
	if (!CreateAnimState)
		return;

	CreateAnimState(state, this);
}

float * C_BaseEntity::m_flPoseParameter() 
{

	static int offset = NetvarManager::Instance()->GetOffset("DT_BaseAnimating", "m_flPoseParameter");

	return reinterpret_cast<float *>(uintptr_t(this) + offset);
}
bool C_BaseEntity::IsValidTarget() 
{

	if (!this || this == nullptr)
		return false;

	ClientClass* pClass = (ClientClass*)this->GetClientClass(); // Needed to check clientclass after nullptr check that was causing a crash

	if (this == g::pLocalEntity)
		return false;

	if (pClass->ClassID != 40)
		return false;

	if (this->GetTeam() == g::pLocalEntity->GetTeam())
		return false;

	if (this->IsDormant())
		return false;

	if (!this->IsAlive())
		return false;

	if (this->IsImmune())
		return false;

	return true;
}


Vector C_BaseEntity::hitbox_position(int hitbox_id)
{
	matrix3x4_t matrix[128];

	if (this->SetupBones(matrix, 128, 0x100, 0.f)) 
	{
		studiohdr_t * hdr = g_pModelInfo->GetStudiomodel(this->GetModel());

		if (hdr) {
			mstudiobbox_t *hitbox = hdr->GetHitboxSet(this->m_nHitboxSet())->GetHitbox(hitbox_id);

			if (hitbox) {
				Vector
					min = Vector{ },
					max = Vector{ };

				g_Math.VectorTransform(hitbox->min, matrix[hitbox->bone], min);
				g_Math.VectorTransform(hitbox->max, matrix[hitbox->bone], max);

				return (min + max) / 2.0f;
			}
		}
	}

	return Vector{ };
}


Vector C_BaseEntity::GetHitboxPosition(int Hitbox, matrix3x4_t *Matrix, float *Radius)
{
	studiohdr_t* hdr = g_pModelInfo->GetStudiomodel(this->GetModel());
	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);
	mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

	if (hitbox)
	{
		Vector vMin, vMax, vCenter, sCenter;
		g_Math.VectorTransform(hitbox->min, Matrix[hitbox->bone], vMin);
		g_Math.VectorTransform(hitbox->max, Matrix[hitbox->bone], vMax);
		vCenter = (vMin + vMax) * 0.5;

		*Radius = hitbox->radius;

		return vCenter;
	}
	
	return Vector(0, 0, 0);
}

bool C_BaseCombatWeapon::IsKnife()
{
	int idx = *m_iItemDefinitionIndex();


	return (idx == (int)ItemDefinitionIndex::WEAPON_KNIFE_PUSH
		|| idx == (int)ItemDefinitionIndex::WEAPON_KNIFE_T
		|| idx == (int)ItemDefinitionIndex::WEAPON_KNIFE
		|| idx == (int)ItemDefinitionIndex::WEAPON_KNIFE_BAYONET
		|| idx == (int)ItemDefinitionIndex::WEAPON_KNIFE_FLIP
		|| idx == (int)ItemDefinitionIndex::WEAPON_KNIFE_GUT
		|| idx == (int)ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT
		|| idx == (int)ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET
		|| idx == (int)ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL
		|| idx == (int)ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE
		|| idx == (int)ItemDefinitionIndex::WEAPON_KNIFE_FALCHION
		|| idx == (int)ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY
		|| idx == (int)ItemDefinitionIndex::WEAPON_KNIFE_GYPSY_JACKKNIFE
		|| idx == (int)ItemDefinitionIndex::WEAPON_KNIFE_STILETTO
		|| idx == (int)ItemDefinitionIndex::WEAPON_KNIFE_URSUS
		|| idx == (int)ItemDefinitionIndex::WEAPON_KNIFE_WIDOWMAKER);
}


bool C_BaseEntity::TestVisuals(void* weapon)
{


	if (weapon == nullptr)
		return false;

	if (GetClientClass()->ClassID != (int)ClassId::CBaseWeaponWorldModel && ((strstr(GetClientClass()->pNetworkName, "Weapon") || GetClientClass()->ClassID == (int)ClassId::CDEagle || GetClientClass()->ClassID == (int)ClassId::CAK47)))
		return true;
	else
		return false;

}
int C_BaseEntity::GetSequenceActivity(int sequence)
{
	auto hdr = g_pModelInfo->GetStudiomodel(this->GetModel());

	if (hdr == nullptr)
		return -1;

	if (!hdr)
		return -1;

	// sig for stuidohdr_t version: 53 56 8B F1 8B DA 85 F6 74 55
	// sig for C_BaseAnimating version: 55 8B EC 83 7D 08 FF 56 8B F1 74 3D
	// c_csplayer vfunc 242, follow calls to find the function.

	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(Utils::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83"));

	return get_sequence_activity(this, hdr, sequence);
}

bool C_BaseEntity::IsMachinegun(void* weapon)
{


	if (weapon == nullptr)
		return false;


	if (GetClientClass()->ClassID == ClassId::CWeaponNegev || ClassId::CWeaponM249)
		return true;
	else
		return false;

}
bool C_BaseEntity::IsShotgun(void* weapon)
{


	if (weapon == nullptr)
		return false;


	if (GetClientClass()->ClassID == ClassId::CWeaponMag7 || ClassId::CWeaponXM1014 || ClassId::CWeaponSawedoff || ClassId::CWeaponNOVA)
		return true;
	else
		return false;

}


bool C_BaseEntity::legitbot(void* weapon)
{


	if (weapon == nullptr)
		return false;


	if (GetClientClass()->ClassID == ClassId::CKnife || ClassId::CHEGrenade || ClassId::CDecoyGrenade || ClassId::CIncendiaryGrenade || ClassId::CSmokeGrenade || ClassId::CC4)
		return true;
	else
		return false;

}

bool C_BaseEntity::is_player() 
{
	return (GetClientClass()->ClassID == 40);

}


bool C_BaseEntity::valid(bool check_team, bool check_dormant)
{
	auto local_player = static_cast<C_BaseEntity *>(g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer()));

	if (!this) { return false; }
	if (IsDormant() && check_dormant) { return false; }
	if (!IsAlive()) { return false; }
	if (!is_player())
	{ 
		return false; 
	}
	if (!g::pLocalEntity) { return false; }

	if (check_team) 
	{
		if (local_player->GetTeam() == GetTeam()) {
			return false;
		}
	}
	return true;
}
void C_BaseEntity::set_abs_origin(const Vector & origin)
{
	using Fn = void(__thiscall*)(void*, const Vector &);

	static auto fn = reinterpret_cast<Fn>((Utils::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1")));
	return fn(this, origin);
}

void C_BaseEntity::setAbsAngle(Vector Angle) 
{
	using SetAbsAngleFn = void(__thiscall*)(void*, const Vector &Angle);
	static SetAbsAngleFn SetAbsAngle;

	if (!SetAbsAngle)
		SetAbsAngle = (SetAbsAngleFn)(Utils::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));

	SetAbsAngle(this, Angle);
}


Vector C_BaseEntity::GetHitboxPosition(int Hitbox, matrix3x4_t *Matrix) // any public source
{
	studiohdr_t* hdr = g_pModelInfo->GetStudiomodel(this->GetModel());
	if (hdr)
	{
		mstudiohitboxset_t* set = hdr->GetHitboxSet(0);
		if (set)
		{
			mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

			if (hitbox)
			{
				Vector vMin, vMax, vCenter, sCenter;
				g_Math.VectorTransform(hitbox->min, Matrix[hitbox->bone], vMin);
				g_Math.VectorTransform(hitbox->max, Matrix[hitbox->bone], vMax);
				vCenter = (vMin + vMax) * 0.5;

				return vCenter;
			}
		}
	}
	return Vector(0, 0, 0);
}

void C_BaseEntity::FixSetupBones(matrix3x4_t* Matrix)
{
	// i took out alot of stuff from here because b1g secrets

	if (this == g::pLocalEntity)
	{
		const auto Backup = *(int*)(uintptr_t(this) + ptrdiff_t(0x272));

		*(int*)(uintptr_t(this) + ptrdiff_t(0x272)) = -1;

		this->SetupBones(Matrix, 126, 0x00000100 | 0x200, g_pGlobalVars->curtime);

		*(int*)(uintptr_t(this) + ptrdiff_t(0x272)) = Backup;
	}
	else
	{
		*reinterpret_cast<int*>(uintptr_t(this) + 0xA30) = g_pGlobalVars->framecount;
		*reinterpret_cast<int*>(uintptr_t(this) + 0xA28) = 0;

		const auto Backup = *(int*)(uintptr_t(this) + ptrdiff_t(0x272));

		*(int*)(uintptr_t(this) + ptrdiff_t(0x272)) = -1;

		this->SetupBones(Matrix, 126, 0x00000100 | 0x200, g_pGlobalVars->curtime);

		*(int*)(uintptr_t(this) + ptrdiff_t(0x272)) = Backup;
	}
}

char * C_BaseCombatWeapon::get_name()
{
	switch (*m_iItemDefinitionIndex())
	{
	case 500:
		return "bayonet";
	case 514:
		return "bowie";
	case 515:
		return "butterfly";
	case 42:
		return "knife";
	case 512:
		return "falchion";
	case 505:
		return "flip";
	case 506:
		return "gut";
	case 507:
		return "karambit";
	case 508:
		return "m9bayonet";
	case 59:
		return "knife";
	case 509:
		return "huntsman";
	case 516:
		return "daggers";
	case 1:
		return "deserteagle";
	case 2:
		return "berretas";
	case 3:
		return "fiveseven";
	case 4:
		return "glock";
	case 32:
		return "p2000";
	case 36:
		return "p250";
	case 61:
		return "usp-s";
	case 30:
		return "tec9";
	case 64:
		return "revolver";
	case 17:
		return "mac10";
	case 24:
		return "ump45";
	case 26:
		return "bizon";
	case 33:
		return "mp7";
	case 23:
		return "mp5";
	case 34:
		return "mp9";
	case 19:
		return "p90";
	case 13:
		return "galil";
	case 10:
		return "famas";
	case 60:
		return "m4a1-s";
	case 16:
		return "m4a1";
	case 8:
		return "aug";
	case 39:
		return "sg556";
	case 7:
		return "ak47";
	case 11:
		return "g3sg1";
	case 38:
		return "scar20";
	case 9:
		return "awp";
	case 40:
		return "scout";
	case 25:
		return "xm1014";
	case 29:
		return "sawedoff";
	case 27:
		return "mag7";
	case 35:
		return "nova";
	case 28:
		return "negev";
	case 14:
		return "m249";
	case 31:
		return "zeus";
	case 43:
		return "flashbang";
	case 44:
		return "grenade";
	case 45:
		return "smoke";
	case 46:
		return "molotov";
	case 47:
		return "decoy";
	case 48:
		return "incendiary";
	case 49:
		return "bomb";
	case 63:
		return "cz75a";
	default:
		return "  ";
	}
}

//SanekGame https://www.unknowncheats.me/forum/1798568-post2.html

bool C_BaseCombatWeapon::is_non_aim()
{
	int idx = *m_iItemDefinitionIndex();

	return (idx == 49 || idx == 42 || idx == 500 || idx == 515 || idx == 512
		|| idx == 505 || idx == 506 || idx == 507 || idx == 508 || idx == 516
		|| idx == 514 || idx == 59 || idx == 509 || idx == 43 || idx == 44
		|| idx == 45 || idx == 46 || idx == 47 || idx == 48);
}

void C_BaseEntity::SetAbsAngles(Vector wantedang)
{
	typedef void(__thiscall* oSetAngle)(void*, const Vector &);
	static oSetAngle _SetAngle = (oSetAngle)((uintptr_t)Utils::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
	_SetAngle(this, wantedang);
}
void C_BaseEntity::SetAbsOrigin(const Vector &origin)
{
	using SetAbsOriginFn = void(__thiscall*)(void*, const Vector &origin);
	static SetAbsOriginFn SetAbsOrigin = (SetAbsOriginFn)Utils::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");
	SetAbsOrigin(this, origin);
}

// sse min.
template< typename t = float >
t minimum(const t &a, const t &b) {
	// check type.
	static_assert(std::is_arithmetic< t >::value, "Math::min only supports integral types.");
	return (t)_mm_cvtss_f32(
		_mm_min_ss(_mm_set_ss((float)a),
			_mm_set_ss((float)b))
	);
}

// sse max.
template< typename t = float >
t maximum(const t &a, const t &b) {
	// check type.
	static_assert(std::is_arithmetic< t >::value, "Math::max only supports integral types.");
	return (t)_mm_cvtss_f32(
		_mm_max_ss(_mm_set_ss((float)a),
			_mm_set_ss((float)b))
	);

}
float C_BaseEntity::DesyncValue() 
{

	
	auto animstate = uintptr_t(this->AnimState());
	if (!animstate)
		return 0.f;
	float rate = 180;
	float duckammount = *(float *)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

	float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0) {

		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));

	}

	unk3 = *(float *)(animstate + 0x334) * unk2;

	return unk3;

	
}

FORCEINLINE C_CSPlayer* ToCSPlayer(C_BaseEntity* pEnt)
{
	if (!pEnt || !pEnt->IsPlayer())
		return nullptr;

	return (C_CSPlayer*)(pEnt);
}
QAngle& C_CSPlayer::m_angEyeAngles()
{
	return *(QAngle*)(uintptr_t(this) + OFFSETS::m_angEyeAngles);
}
C_CSPlayer* C_CSPlayer::GetLocalPlayer()
{
	auto index = g_pEngine->GetLocalPlayer();

	if (!index)
		return nullptr;

	auto client = g_pEntityList->GetClientEntity(index);

	if (!client)
		return nullptr;

	return ToCSPlayer(client->GetBaseEntity());
}

unsigned int C_BaseEntity::FindInDataMap(datamap_t* pMap, const char* name)
{
	//FUNCTION_GUARD;

	while (pMap)
	{
		for (int i = 0; i < pMap->dataNumFields; i++)
		{
			if (pMap->dataDesc[i].fieldName == NULL)
				continue;

			if (strcmp(name, pMap->dataDesc[i].fieldName) == 0)
				return pMap->dataDesc[i].fieldOffset[TD_OFFSET_NORMAL];

			if (pMap->dataDesc[i].fieldType == FIELD_EMBEDDED)
			{
				if (pMap->dataDesc[i].td)
				{
					unsigned int offset;

					if ((offset = FindInDataMap(pMap->dataDesc[i].td, name)) != 0)
						return offset;
				}
			}
		}
		pMap = pMap->baseMap;
	}

	return 0;
}
float& C_BaseEntity::m_surfaceFriction()
{
	static unsigned int _m_surfaceFriction = FindInDataMap(GetPredDescMap(), "m_surfaceFriction");
	return *(float*)((uintptr_t)this + _m_surfaceFriction);
}
Vector& C_BaseEntity::m_vecVelocity()
{
	return *(Vector*)(uintptr_t(this) + OFFSETS::m_vecVelocity);
}


bool C_BaseEntity::IsAlive() const
{
	return GetLifeState() == 0;
}

void C_BaseEntity::SetAbsVelocity(Vector velocity) // i dont remember
{
	using Fn = void(__thiscall*)(void*, const Vector &velocity);
	static Fn AbsVelocity = (Fn)Utils::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 0C 53 56 57 8B 7D 08 8B F1 F3");

	AbsVelocity(this, velocity);
}

void C_BaseEntity::SetAbsOriginal(Vector origin)
{
	using SetAbsOriginFn = void(__thiscall*)(void*, const Vector &origin);
	static SetAbsOriginFn SetAbsOrigin;
	if (!SetAbsOrigin)
		SetAbsOrigin = (SetAbsOriginFn)((DWORD)Utils::FindPattern("client_panorama.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1\xE8\x00\x00", "xxxxxxxxxxxxx??"));
	SetAbsOrigin(this, origin);
}

bool C_BaseCombatWeapon::is_knife()
{
	short iWeaponID = *this->m_iItemDefinitionIndex();
	return (
		iWeaponID == 42
		|| iWeaponID == 59
		|| iWeaponID == 41
		|| iWeaponID == 500
		|| iWeaponID == 505
		|| iWeaponID == 506
		|| iWeaponID == 507
		|| iWeaponID == 508
		|| iWeaponID == 509
		|| iWeaponID == 515
		|| iWeaponID == 523
		|| iWeaponID == 522
		|| iWeaponID == 520
		|| iWeaponID == 519
		|| iWeaponID == 516);
}


bool C_BaseCombatWeapon::is_grenade()
{
	if (!this)
		return false;

	short WeaponId = *m_iItemDefinitionIndex();

	if (!WeaponId)
		return false;

	if (WeaponId == 43 || WeaponId == 44 || WeaponId == 45 || WeaponId == 46 || WeaponId == 47 || WeaponId == 48 || WeaponId == 49)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void C_BaseEntity::invalidateBoneCache()// deadcell 
{
	static auto invalidate_bone_cache = Utils::FindSignature("client_panorama.dll", "80 3D ?? ?? ?? ?? ?? 74 16 A1 ?? ?? ?? ?? 48 C7 81");
	static auto model_bone_counter = **reinterpret_cast<uintptr_t**>(invalidate_bone_cache + 10);
	*reinterpret_cast<unsigned int*>(reinterpret_cast<uintptr_t>(this) + 0x2924) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
	*reinterpret_cast<unsigned int*>(reinterpret_cast<uintptr_t>(this) + 0x2690) = (model_bone_counter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;
}



bool C_BaseEntity::Isnade()
{
	if (!this)
		return false;
	if (!this->IsAlive())
		return false;

	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)this->GetActiveWeapon();

	if (!pWeapon)
		return false;

	std::string WeaponName = pWeapon->GetName();

	if (WeaponName == "weapon_incgrenade")
		return true;
	else if (WeaponName == "weapon_decoy")
		return true;
	else if (WeaponName == "weapon_flashbang")
		return true;
	else if (WeaponName == "weapon_hegrenade")
		return true;
	else if (WeaponName == "weapon_smokegrenade")
		return true;
	else if (WeaponName == "weapon_molotov")
		return true;

	return false;
}


bool C_BaseEntity::IsKnifeorNade()
{
	if (!this)
		return false;
	if (!this->IsAlive())
		return false;

	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)this->GetActiveWeapon();

	if (!pWeapon)
		return false;

	std::string WeaponName = pWeapon->GetName();

	if (WeaponName == "weapon_knife")
		return true;
	else if (WeaponName == "weapon_incgrenade")
		return true;
	else if (WeaponName == "weapon_decoy")
		return true;
	else if (WeaponName == "weapon_flashbang")
		return true;
	else if (WeaponName == "weapon_hegrenade")
		return true;
	else if (WeaponName == "weapon_smokegrenade")
		return true;
	else if (WeaponName == "weapon_molotov")
		return true;

	return false;
}

float C_BaseEntity::FireRate()
{
	if (!this)
		return 0.f;
	if (!this->IsAlive())
		return 0.f;
	if (this->IsKnifeorNade())
		return 0.f;

	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)this->GetActiveWeapon();

	if (!pWeapon)
		return false;

	std::string WeaponName = pWeapon->GetName();

	if (WeaponName == "weapon_glock")
		return 0.15f;
	else if (WeaponName == "weapon_hkp2000")
		return 0.169f;
	else if (WeaponName == "weapon_p250")//the cz and p250 have the same name idky same with other guns
		return 0.15f;
	else if (WeaponName == "weapon_tec9")
		return 0.12f;
	else if (WeaponName == "weapon_elite")
		return 0.12f;
	else if (WeaponName == "weapon_fiveseven")
		return 0.15f;
	else if (WeaponName == "weapon_deagle")
		return 0.224f;
	else if (WeaponName == "weapon_nova")
		return 0.882f;
	else if (WeaponName == "weapon_sawedoff")
		return 0.845f;
	else if (WeaponName == "weapon_mag7")
		return 0.845f;
	else if (WeaponName == "weapon_xm1014")
		return 0.35f;
	else if (WeaponName == "weapon_mac10")
		return 0.075f;
	else if (WeaponName == "weapon_ump45")
		return 0.089f;
	else if (WeaponName == "weapon_mp9")
		return 0.070f;
	else if (WeaponName == "weapon_bizon")
		return 0.08f;
	else if (WeaponName == "weapon_mp7")
		return 0.08f;
	else if (WeaponName == "weapon_p90")
		return 0.070f;
	else if (WeaponName == "weapon_galilar")
		return 0.089f;
	else if (WeaponName == "weapon_ak47")
		return 0.1f;
	else if (WeaponName == "weapon_sg553")
		return 0.089f;
	else if (WeaponName == "weapon_m4a1")
		return 0.089f;
	else if (WeaponName == "weapon_aug")
		return 0.089f;
	else if (WeaponName == "weapon_m249")
		return 0.08f;
	else if (WeaponName == "weapon_negev")
		return 0.0008f;
	else if (WeaponName == "weapon_ssg08")
		return 1.25f;
	else if (WeaponName == "weapon_awp")
		return 1.463f;
	else if (WeaponName == "weapon_g3sg1")
		return 0.25f;
	else if (WeaponName == "weapon_scar20")
		return 0.25f;
	else if (WeaponName == "weapon_mp5sd")
		return 0.08f;
	else
		return .0f;
	
}



bool C_BaseEntity::IsEnemy()
{
	return this->GetTeam() != g::pLocalEntity->GetTeam();
}
