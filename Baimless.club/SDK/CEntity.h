#pragma once
#include "Definitions.h"
#include "IClientUnknown.h"
#include "IClientEntityList.h"
#include "CInput.h"
#include "..\Utils\Utils.h"
#include "..\Utils\NetvarManager.h"
#include "assert.h"
#include "ISurface.h"
#include <optional>

#define NETVAR(type, name, table, ...) type& name##() const { static DWORD _##name = GET_NETVAR(table, __VA_ARGS__); assert(_##name); return *(type*)((uintptr_t)this + _##name); }
#define VIRTUAL_METHOD(returnType, name, idx, args, argsRaw) \
constexpr auto name##args noexcept \
{ \
    return VirtualMethod::call<returnType, idx>##argsRaw; \
} \

inline IHandleEntity* CBaseHandle::Get() const
{
    return g_pEntityList->GetClientEntityFromHandle2(*this);
}


// class predefinition
class C_BaseCombatWeapon;
class C_BaseViewModel2;
class C_PlantedC4;
#define ptr( x, x1, x2 ) *(x*)( (DWORD)x1 + (DWORD)x2 )
/*
 * player_old_flags
 */
struct player_old_flags
{
	int flags;
	float curtime;
};
/*
 * player_prediction_data
 */
struct player_prediction_data
{
	void reset()
	{
		prediction_stage = 0, tickbase = 0;
		random_seed = 0;
		in_prediction = false;
		curtime = 0.f, frametime = 0.f;
		prediction_random_seed = nullptr;
	}

	int prediction_stage = 0, tickbase = 0;
	unsigned random_seed = 0;
	bool in_prediction = false;
	float curtime = 0.f, frametime = 0.f;
	unsigned* prediction_random_seed = nullptr;
	player_old_flags non_predicted;
};

class C_AnimState
{
public:
	char pad[3];
	char bUnknown; //0x4
	char pad2[91];
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[4];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[8];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	unsigned char m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float m_flUnknown3;
	char pad10[528];
	float* feetyaw() //rofl
	{
		return reinterpret_cast<float*>((DWORD)this + 0x70);
	}

	float & m_flSpeedFraction() {
		return *(float *)((uintptr_t)this + 0xF8);
	}

	float & m_flMaxWeaponVelocity() {// (2.9411764 / flMaxPlayerSpeed) * velocity
		return *(float *)((uintptr_t)this + 0xFC);
	}

	float & m_flMinBodyYaw() {
		return *(float *)((uintptr_t)this + 0x330);
	}

	float & m_flMaxBodyYaw() {
		return *(float *)((uintptr_t)this + 0x334);
	}

	float & m_flLandingRatio() {
		return *(float *)((uintptr_t)this + 0x11C);
	}

	float & m_flAbsRotation() {
		return *(float *)((uintptr_t)this + 0x80);
	}
	float_t& pad_0x0120()
	{
		return *(float_t*)((uintptr_t)this + 0x0120);
	}
};

class AnimationLayer {
public:
	char  pad_0000[20];
	// These should also be present in the padding, don't see the use for it though
	//float	m_flLayerAnimtime;
	//float	m_flLayerFadeOuttime;
	unsigned int m_nOrder; //0x0014
	unsigned int m_nSequence; //0x0018
	float m_flPrevCycle; //0x001C
	float m_flWeight; //0x0020
	float m_flWeightDeltaRate; //0x0024
	float m_flPlaybackRate; //0x0028
	float m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038

enum ClassId
{
	CAI_BaseNPC = 0,
	CAK47,
	CBaseAnimating,
	CBaseAnimatingOverlay,
	CBaseAttributableItem,
	CBaseButton,
	CBaseCombatCharacter,
	CBaseCombatWeapon,
	CBaseCSGrenade,
	CBaseCSGrenadeProjectile,
	CBaseDoor,
	CBaseEntity,
	CBaseFlex,
	CBaseGrenade,
	CBaseParticleEntity,
	CBasePlayer,
	CBasePropDoor,
	CBaseTeamObjectiveResource,
	CBaseTempEntity,
	CBaseToggle,
	CBaseTrigger,
	CBaseViewModel,
	CBaseVPhysicsTrigger,
	CBaseWeaponWorldModel,
	CBeam,
	CBeamSpotlight,
	CBoneFollower,
	CBRC4Target,
	CBreachCharge,
	CBreachChargeProjectile,
	CBreakableProp,
	CBreakableSurface,
	CBumpMine,
	CBumpMineProjectile,
	CC4,
	CCascadeLight,
	CChicken,
	CColorCorrection,
	CColorCorrectionVolume,
	CCSGameRulesProxy,
	CCSPlayer,
	CCSPlayerResource,
	CCSRagdoll,
	CCSTeam,
	CDangerZone,
	CDangerZoneController,
	CDEagle,
	CDecoyGrenade,
	CDecoyProjectile,
	CDrone,
	CDronegun,
	CDynamicLight,
	CDynamicProp,
	CEconEntity,
	CEconWearable,
	CEmbers,
	CEntityDissolve,
	CEntityFlame,
	CEntityFreezing,
	CEntityParticleTrail,
	CEnvAmbientLight,
	CEnvDetailController,
	CEnvDOFController,
	CEnvGasCanister,
	CEnvParticleScript,
	CEnvProjectedTexture,
	CEnvQuadraticBeam,
	CEnvScreenEffect,
	CEnvScreenOverlay,
	CEnvTonemapController,
	CEnvWind,
	CFEPlayerDecal,
	CFireCrackerBlast,
	CFireSmoke,
	CFireTrail,
	CFish,
	CFists,
	CFlashbang,
	CFogController,
	CFootstepControl,
	CFunc_Dust,
	CFunc_LOD,
	CFuncAreaPortalWindow,
	CFuncBrush,
	CFuncConveyor,
	CFuncLadder,
	CFuncMonitor,
	CFuncMoveLinear,
	CFuncOccluder,
	CFuncReflectiveGlass,
	CFuncRotating,
	CFuncSmokeVolume,
	CFuncTrackTrain,
	CGameRulesProxy,
	CGrassBurn,
	CHandleTest,
	CHEGrenade,
	CHostage,
	CHostageCarriableProp,
	CIncendiaryGrenade,
	CInferno,
	CInfoLadderDismount,
	CInfoMapRegion,
	CInfoOverlayAccessor,
	CItem_Healthshot,
	CItemCash,
	CItemDogtags,
	CKnife,
	CKnifeGG,
	CLightGlow,
	CMaterialModifyControl,
	CMelee,
	CMolotovGrenade,
	CMolotovProjectile,
	CMovieDisplay,
	CParadropChopper,
	CParticleFire,
	CParticlePerformanceMonitor,
	CParticleSystem,
	CPhysBox,
	CPhysBoxMultiplayer,
	CPhysicsProp,
	CPhysicsPropMultiplayer,
	CPhysMagnet,
	CPhysPropAmmoBox,
	CPhysPropLootCrate,
	CPhysPropRadarJammer,
	CPhysPropWeaponUpgrade,
	CPlantedC4,
	CPlasma,
	CPlayerPing,
	CPlayerResource,
	CPointCamera,
	CPointCommentaryNode,
	CPointWorldText,
	CPoseController,
	CPostProcessController,
	CPrecipitation,
	CPrecipitationBlocker,
	CPredictedViewModel,
	CProp_Hallucination,
	CPropCounter,
	CPropDoorRotating,
	CPropJeep,
	CPropVehicleDriveable,
	CRagdollManager,
	CRagdollProp,
	CRagdollPropAttached,
	CRopeKeyframe,
	CSCAR17,
	CSceneEntity,
	CSensorGrenade,
	CSensorGrenadeProjectile,
	CShadowControl,
	CSlideshowDisplay,
	CSmokeGrenade,
	CSmokeGrenadeProjectile,
	CSmokeStack,
	CSnowball,
	CSnowballPile,
	CSnowballProjectile,
	CSpatialEntity,
	CSpotlightEnd,
	CSprite,
	CSpriteOriented,
	CSpriteTrail,
	CStatueProp,
	CSteamJet,
	CSun,
	CSunlightShadowControl,
	CSurvivalSpawnChopper,
	CTablet,
	CTeam,
	CTeamplayRoundBasedRulesProxy,
	CTEArmorRicochet,
	CTEBaseBeam,
	CTEBeamEntPoint,
	CTEBeamEnts,
	CTEBeamFollow,
	CTEBeamLaser,
	CTEBeamPoints,
	CTEBeamRing,
	CTEBeamRingPoint,
	CTEBeamSpline,
	CTEBloodSprite,
	CTEBloodStream,
	CTEBreakModel,
	CTEBSPDecal,
	CTEBubbles,
	CTEBubbleTrail,
	CTEClientProjectile,
	CTEDecal,
	CTEDust,
	CTEDynamicLight,
	CTEEffectDispatch,
	CTEEnergySplash,
	CTEExplosion,
	CTEFireBullets,
	CTEFizz,
	CTEFootprintDecal,
	CTEFoundryHelpers,
	CTEGaussExplosion,
	CTEGlowSprite,
	CTEImpact,
	CTEKillPlayerAttachments,
	CTELargeFunnel,
	CTEMetalSparks,
	CTEMuzzleFlash,
	CTEParticleSystem,
	CTEPhysicsProp,
	CTEPlantBomb,
	CTEPlayerAnimEvent,
	CTEPlayerDecal,
	CTEProjectedDecal,
	CTERadioIcon,
	CTEShatterSurface,
	CTEShowLine,
	CTesla,
	CTESmoke,
	CTESparks,
	CTESprite,
	CTESpriteSpray,
	CTest_ProxyToggle_Networkable,
	CTestTraceline,
	CTEWorldDecal,
	CTriggerPlayerMovement,
	CTriggerSoundOperator,
	CVGuiScreen,
	CVoteController,
	CWaterBullet,
	CWaterLODControl,
	CWeaponAug,
	CWeaponAWP,
	CWeaponBaseItem,
	CWeaponBizon,
	CWeaponCSBase,
	CWeaponCSBaseGun,
	CWeaponCycler,
	CWeaponElite,
	CWeaponFamas,
	CWeaponFiveSeven,
	CWeaponG3SG1,
	CWeaponGalil,
	CWeaponGalilAR,
	CWeaponGlock,
	CWeaponHKP2000,
	CWeaponM249,
	CWeaponM3,
	CWeaponM4A1,
	CWeaponMAC10,
	CWeaponMag7,
	CWeaponMP5Navy,
	CWeaponMP7,
	CWeaponMP9,
	CWeaponNegev,
	CWeaponNOVA,
	CWeaponP228,
	CWeaponP250,
	CWeaponP90,
	CWeaponSawedoff,
	CWeaponSCAR20,
	CWeaponScout,
	CWeaponSG550,
	CWeaponSG552,
	CWeaponSG556,
	CWeaponShield,
	CWeaponSSG08,
	CWeaponTaser,
	CWeaponTec9,
	CWeaponTMP,
	CWeaponUMP45,
	CWeaponUSP,
	CWeaponXM1014,
	CWorld,
	CWorldVguiText,
	DustTrail,
	MovieExplosion,
	ParticleSmokeGrenade,
	RocketTrail,
	SmokeTrail,
	SporeExplosion,
	SporeTrail,
};

enum DataUpdateType_txd
{
	DATA_UPDATE_CREATEDxd = 0,
	DATA_UPDATE_DATATABLE_CHANGEDxd,
};

#define BONE_USED_BY_HITBOX			0x00000100

enum move_type : int
{
	// never moves
	movetype_none = 0,

	// For players -- in TF2 commander view, etc.
	movetype_isometric = 1,

	// Player only - moving on the ground
	movetype_walk = 2,

	// gravity, special edge handling -- monsters use this
	movetype_step = 3,

	// No gravity, but still collides with stuff
	movetype_fly = 4,

	// flies through the air + is affected by gravity
	movetype_flygravity = 5,

	// uses VPHYSICS for simulation
	movetype_vphysics = 6,

	// no clip to world, push and crush
	movetype_push = 7,

	// No gravity, no collisions, still do velocity/avelocity
	movetype_noclip = 8,

	// Used by players only when going onto a ladder
	movetype_ladder = 9,

	// Observer movement, depends on player's observer mode
	movetype_observer = 10,

	// Allows the entity to describe its own physics
	movetype_custom = 11,

	// should always be defined as the last item in the list
	movetype_last = movetype_observer,

	movetype_max_bits = 4
};

struct c_animstate {

	char pad_0x0000[0x18]; //0x0000
	float anim_update_timer; //0x0018 
	char pad_0x001C[0xC]; //0x001C
	float started_moving_time; //0x0028 
	float last_move_time; //0x002C 
	char pad_0x0030[0x10]; //0x0030
	float last_lby_time; //0x0040 
	char pad_0x0044[0x8]; //0x0044
	float run_amount; //0x004C 
	char pad_0x0050[0x10]; //0x0050
	void *entity; //0x0060 
	__int32 active_weapon; //0x0064 
	__int32 last_active_weapon; //0x0068 
	float last_client_side_animation_update_time; //0x006C 
	__int32 last_client_side_animation_update_framecount; //0x0070 
	float eye_timer; //0x0074 
	float eye_angles_y; //0x0078 
	float eye_angles_x; //0x007C 
	float goal_feet_yaw; //0x0080 
	float current_feet_yaw; //0x0084 
	float torso_yaw; //0x0088 
	float last_move_yaw; //0x008C 
	float lean_amount; //0x0090 
	char pad_0x0094[0x4]; //0x0094
	float feet_cycle; //0x0098 
	float feet_yaw_rate; //0x009C 
	char pad_0x00A0[0x4]; //0x00A0
	float duck_amount; //0x00A4 
	float landing_duck_amount; //0x00A8 
	char pad_0x00AC[0x4]; //0x00AC
	Vector current_origin;
	Vector last_origin;
	float velocity_x; //0x00C8 
	float velocity_y; //0x00CC 
	char pad_0x00D0[0x10]; //0x00D0
	float move_direction_1; //0x00E0 
	float move_direction_2; //0x00E4 
	char pad_0x00E8[0x4]; //0x00E8
	float m_velocity; //0x00EC 
	float jump_fall_velocity; //0x00F0 
	float clamped_velocity; //0x00F4 
	float feet_speed_forwards_or_sideways; //0x00F8 
	float feet_speed_unknown_forwards_or_sideways; //0x00FC 
	float last_time_started_moving; //0x0100 
	float last_time_stopped_moving; //0x0104 
	bool on_ground; //0x0108 
	bool hit_in_ground_animation; //0x010C 
	char pad_0x0110[0x4]; //0x0110
	float last_origin_z; //0x0114 
	float head_from_ground_distance_standing; //0x0118 
	float stop_to_full_running_fraction; //0x011C 
	char pad_0x0120[0x14]; //0x0120
	__int32 is_not_moving; //0x0134 
	char pad_0x0138[0x20]; //0x0138
	float last_anim_update_time; //0x0158 
	float moving_direction_x; //0x015C 
	float moving_direction_y; //0x0160 
	float moving_direction_z; //0x0164 
	char pad_0x0168[0x44]; //0x0168
	__int32 started_moving; //0x01AC 
	char pad_0x01B0[0x8]; //0x01B0
	float lean_yaw; //0x01B8 
	char pad_0x01BC[0x8]; //0x01BC
	float poses_speed; //0x01C4 
	char pad_0x01C8[0x8]; //0x01C8
	float ladder_speed; //0x01D0 
	char pad_0x01D4[0x8]; //0x01D4
	float ladder_yaw; //0x01DC 
	char pad_0x01E0[0x8]; //0x01E0
	float some_pose; //0x01E8 
	char pad_0x01EC[0x14]; //0x01EC
	float body_yaw; //0x0200 
	char pad_0x0204[0x8]; //0x0204
	float body_pitch; //0x020C 
	char pad_0x0210[0x8]; //0x0210
	float death_yaw; //0x0218 
	char pad_0x021C[0x8]; //0x021C
	float stand; //0x0224 
	char pad_0x0228[0x8]; //0x0228
	float jump_fall; //0x0230 
	char pad_0x0234[0x8]; //0x0234
	float aim_blend_stand_idle; //0x023C 
	char pad_0x0240[0x8]; //0x0240
	float aim_blend_crouch_idle; //0x0248 
	char pad_0x024C[0x8]; //0x024C
	float strafe_yaw; //0x0254 
	char pad_0x0258[0x8]; //0x0258
	float aim_blend_stand_walk; //0x0260 
	char pad_0x0264[0x8]; //0x0264
	float aim_blend_stand_run; //0x026C 
	char pad_0x0270[0x8]; //0x0270
	float aim_blend_crouch_walk; //0x0278 
	char pad_0x027C[0x8]; //0x027C
	float move_blend_walk; //0x0284 
	char pad_0x0288[0x8]; //0x0288
	float move_blend_run; //0x0290 
	char pad_0x0294[0x8]; //0x0294
	float move_blend_crouch; //0x029C 
	char pad_0x02A0[0x4]; //0x02A0
	float speed; //0x02A4 
	__int32 moving_in_any_direction; //0x02A8 
	float acceleration; //0x02AC 
	char pad_0x02B0[0x74]; //0x02B0
	float crouch_height; //0x0324 
	__int32 is_full_crouched; //0x0328 
	char pad_0x032C[0x4]; //0x032C
	float velocity_subtract_x; //0x0330 
	float velocity_subtract_y; //0x0334 
	float velocity_subtract_z; //0x0338 
	float standing_head_height; //0x033C 
	char pad_0x0340[0x4]; //0x0340
}; //Size=0x0344

class CGameRules {
public:
	NETVAR(int, m_iMatchStats_PlayersAlive_T, "DT_CSGameRules", "m_iMatchStats_PlayersAlive_T");
	NETVAR(int, m_iMatchStats_PlayersAlive_CT, "DT_CSGameRules", "m_iMatchStats_PlayersAlive_CT");
	NETVAR(int, m_iRoundTime, "DT_CSGameRules", "m_iRoundTime");
	NETVAR(bool, m_bFreezePeriod, "DT_CSGameRules", "m_bBombDropped");
	NETVAR(bool, m_bIsValveDS, "DT_CSGameRules", "m_bIsValveDS");
	NETVAR(bool, m_bBombDropped, "DT_CSGameRules", "m_bBombDropped");
	NETVAR(bool, m_bBombPlanted, "DT_CSGameRules", "m_bBombPlanted");
	NETVAR(float, m_flSurvivalStartTime, "DT_CSGameRules", "m_flSurvivalStartTime");
}; extern CGameRules** g_pGameRules;

#define OFFSET(funcname, type, offset) type& funcname() \
{ \
	static uint16_t _offset = offset; \
	Assert(_offset); \
	return *reinterpret_cast<type*>(uintptr_t(this) + _offset); \
}


class C_BaseEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable
{
private:
    template<class T>
    T GetPointer(const int offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
    // To get value from the pointer itself
    template<class T>
    T GetValue(const int offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
	template<class T>
	T GetValue1(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	template<class T>
	T GetValue2(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	template<class T>
	T GetValue3(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	template<class T>
	T GetValue4(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	template<class T>
	T GetValue5(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	template<class T>
	T GetValue6(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	template<class T>
	T GetValue7(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	template<class T>
	T GetValue8(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	template<class T>
	T GetValue9(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	template<class T>
	T GetValue10(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	template<class T>
	T GetValue11(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	template<class T>
	T GetValue12(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	template<class T>
	T GetValue13(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	template<class T>
	T GetValue14(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}

	template<class T>
	T GetValue15(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}

	template<class T>
	T GetValue16(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}
	// crash one of these offset
	template <typename T>
	T& SetValue(uintptr_t offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(this) + offset);
	}

public:
    //////////////////////////////////
    //New shit
    //NETVAR(int,index, "CBaseEntity", "m_bIsAutoaimTarget", 4)
	bool& m_bSpotted();
	unsigned int FindInDataMap(datamap_t* pMap, const char* name);
	float& m_surfaceFriction();
	Vector& m_vecVelocity();
	int* FallbackSeed()
    {
        return (int*)((uintptr_t)this + OFFSETS::m_nFallbackSeed);
    }
	constexpr void setModelIndex(int index) noexcept
	{
		callVirtualMethod<void, int>(this, 75, index);
	}
	
	NETVAR(int,team, "CBaseEntity", "m_iTeamNum");
	NETVAR(unsigned,modelIndex, "CBaseEntity", "m_nModelIndex")
    NETVAR(bool, m_bBombPlanted, "CCSGameRulesProxy", "m_bBombPlanted");
	bool IsFlashed();
    NETVAR(int, clip, "CBaseCombatWeapon", "m_iClip1");
    int getWeaponSubType()
    {
        return qCallVFunction<int>(this, 281);
    }
    NETVAR(int,ragdoll, "CCSPlayer", "m_hRagdoll")
    int* GetWeapons()
    {
        return reinterpret_cast<int*>(uintptr_t(this) + 0x2DE8);
    }
    NETVAR(int32_t, m_nModelIndex, "DT_BaseEntity", "m_nModelIndex");
    NETVAR(CHandle<C_BaseViewModel2>, m_hViewModel, "DT_BasePlayer", "m_hViewModel[0]");
    NETVAR(CHandle<C_BaseCombatWeapon>, m_hActiveWeapon, "DT_BaseCombatCharacter", "m_hActiveWeapon");
    const matrix3x4_t& m_rgflCoordinateFrame()
    {
        static auto _m_rgflCoordinateFrame = NetvarManager::Instance()->GetOffset("DT_BaseEntity", "m_CollisionGroup") - 0x30;
        return *(matrix3x4_t*)((uintptr_t)this + _m_rgflCoordinateFrame);
    }
    PlayerInfo_t GetPlayerInfo();
    NETVAR(float_t, m_flC4Blow, "DT_PlantedC4", "m_flC4Blow");
    static __forceinline C_BaseEntity* GetEntityByIndex(int index) {
        return static_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(index));
    }
    bool IsLoot();
    bool IsWeapon();
    bool IsPlantedC4();
    bool IsDefuseKit();
    //////////////////////////////////


	template< class T >
	inline T GetFieldValue(int offset)
	{
		return *(T*)((DWORD)this + offset);
	}
	NETVAR(int32_t, m_nFallbackPaintKit, "DT_BaseAttributableItem", "m_nFallbackPaintKit");
	NETVAR(int, m_nHitboxSet, "DT_BaseAnimating", "m_nHitboxSet")
	NETVAR(bool, c4Ticking, "DT_PlantedC4", "m_bBombTicking")
	NETVAR(int, c4BombSite, "DT_PlantedC4", "m_nBombSite")
	NETVAR(int, c4Defuser, "DT_PlantedC4", "m_hBombDefuser")
	NETVAR(float, c4BlowTime, "DT_PlantedC4", "m_flC4Blow")
	NETVAR(float, c4DefuseCountDown, "DT_PlantedC4", "m_flDefuseCountDown")
	NETVAR(bool, hasDefuser, "DT_CSPlayer", "m_bHasDefuser")

	int* m_nFallbackStatTrak()
	{
		return (int*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BaseAttributableItem", "m_nFallbackStatTrak"));
	}
	AnimationLayer* AnimOverlays()
	{
		return *reinterpret_cast<AnimationLayer**>(uintptr_t(this) + 0x2980);
	}
	int NumOverlays()
	{
		return 15;
	}
//	OFFSET(get_model_ptr, CStudioHdr*, 0x294C)
	int GetNumAnimOverlays()
	{
		return *(int*)((DWORD)this + 0x298C);
	}
	Vector GetHitboxPositionxd(int Hitbox);

	inline const char* get_classname(void) {
		return ((const char*(__thiscall*)(C_BaseEntity*)) *(uintptr_t*)(*(uintptr_t*)this + 556))(this);
	}
	inline int* FallbackPaintKitGLOVE()
	{
		return (int*)((DWORD)this + NetvarManager::Instance()->GetOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit"));
	}
	int* OwnerXuidLowGLOVE()
	{
		return (int*)((uintptr_t)this + OFFSETS::m_OriginalOwnerXuidLow);
	}
	int* OwnerXuidHighGLOVE()
	{
		return (int*)((uintptr_t)this + OFFSETS::m_OriginalOwnerXuidHigh);
	}
	float GetNextAttack()
	{
		return *reinterpret_cast<float*>(uint32_t(this) + OFFSETS::m_flNextAttack);
	}

	float *m_flHealthShotBoostExpirationTime()
	{
		static int m_m_flHealthShotBoostExpirationTime = NetvarManager::Instance()->GetOffset("DT_CSPlayer", "m_flHealthShotBoostExpirationTime");

		return reinterpret_cast<float*>(uint32_t(this) + m_m_flHealthShotBoostExpirationTime);
	}

	CBaseHandle owner()
	{
		static int m_hOwnerEntity = NetvarManager::Instance()->GetOffset("DT_BaseEntity", "m_hOwnerEntity");

		return GetValue<CBaseHandle>(m_hOwnerEntity);
	}
	Vector GetAbsOrigin2() {
		__asm {
			MOV ECX, this
			MOV EAX, DWORD PTR DS : [ECX]
			CALL DWORD PTR DS : [EAX + 0x28]
		}
	}

	bool is_on_ground()
	{
		return GetFlags() & FL_ONGROUND;
	}
	Vector GetBonePos(int i)
	{
		matrix3x4_t boneMatrix[128];
		if (this->SetupBones(boneMatrix, 128, BONE_USED_BY_HITBOX, GetTickCount64()))
		{
			return Vector(boneMatrix[i][0][3], boneMatrix[i][1][3], boneMatrix[i][2][3]);
		}
		return Vector(0, 0, 0);
	}
	int GetSequenceActivity(int sequence);

	float m_flSpawnTime()
	{
		return *(float*)((uintptr_t)this + 0xAD4);
	}
	int &getTakeDamage() {
		return *reinterpret_cast<int *>(uintptr_t(this) + 0x280);
	}
	

	Vector GetHeadPos()
	{
		return this->GetBonePos(6);
	}
	bool Isnade();
	void invalidateBoneCache();
	Vector worldSpaceCenter()
	{
		Vector vecOrigin = this->GetOrigin();
		auto collide = (ICollideable*)this->GetCollideable();

		Vector min = collide->OBBMins() + vecOrigin;
		Vector max = collide->OBBMaxs() + vecOrigin;

		Vector size = max - min;
		size /= 2.f;
		size += min;

		return size;
	}


	bool isValidPlayer()
	{
		if (!this)
			return false;
		if (!IsAlive())
			return false;
		//	if (!(GetHealth() > 0))
		//		return false;
		PlayerInfo_t info;
		return true;
	}
	void SetEyeAngles(Vector angles)
	{
		*reinterpret_cast<Vector*>(uintptr_t(this) + OFFSETS::m_angEyeAngles) = angles;
	}
	void SetGloveModelIndex(int modelIndex)
	{
		return Utils::GetVFunc<void(__thiscall*)(void*, int)>(this, 75)(this, modelIndex);
	}
	void set_model_index(int index) {
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)this)[75](this, index);
	}
	void SetModelIndex(int index)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return Utils::GetVFunc<OriginalFn>(this, 75)(this, index);
	}
	void PreDataUpdateNEW(DataUpdateType_txd updateType)
	{
		PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return Utils::GetVFunc<OriginalFn>(pNetworkable, 6)(pNetworkable, updateType);
	}
	float* m_flFallbackWear()
	{
		return (float*)((uintptr_t)this + OFFSETS::m_flFallbackWear);
	}
	int* m_iEntityQuality()
	{
		return (int*)((uintptr_t)this + OFFSETS::m_iEntityQuality);
	}

	int* m_iItemIDHigh()
	{
		return (int*)((uintptr_t)this + OFFSETS::m_iItemIDHigh);
	}

	float GetPostponeFireReadyTime()
	{
		return *reinterpret_cast<float*>(uintptr_t(this) + OFFSETS::m_flPostponeFireReadyTime);
	}
	int GetMoney()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + NetvarManager::Instance()->GetOffset("DT_CSPlayer", "m_iAccount"));
	}
	float m_fDuckSpeed()
	{
		//DT_BasePlayer
		static int m_flDuckSpeed = NetvarManager::Instance()->GetOffset("DT_BasePlayer", "m_flDuckSpeed");
		return *(float*)((uintptr_t)this + (uintptr_t)m_flDuckSpeed);
	}
	short* m_iItemDefinitionIndex()
	{
		return (short*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex"));
	}
	
	datamap_t *GetPredDescMap()
	{
		typedef datamap_t*(__thiscall *o_GetPredDescMap)(void*);
		return Utils::GetVFunc<o_GetPredDescMap>(this, 17)(this);
	}
	//DATAMAPVAR(get_abs_origin, Vector, "m_vecAbsOrigin")


	Vector& get_abs_origin()
	{
		static unsigned int _m_vecAbsOrigin = NetvarManager::Instance()->FindInDataMap(GetPredDescMap(), "m_vecAbsOrigin");
		return *(Vector*)((uintptr_t)this + _m_vecAbsOrigin);

	}
	uint32_t &m_iEFlags()
	{
		static unsigned int _m_iEFlags = NetvarManager::Instance()->FindInDataMap(GetPredDescMap(), "m_iEFlags");
		return *(uint32_t*)((uintptr_t)this + _m_iEFlags);
	}
		

	Vector &m_vecAbsVelocity()
	{
		static unsigned int _m_vecAbsVelocity = NetvarManager::Instance()->FindInDataMap(GetPredDescMap(), "m_vecAbsVelocity");
		return *(Vector*)((uintptr_t)this + _m_vecAbsVelocity);

	}
	int* m_iAccountID()
	{
		return (int*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iAccountID"));
	}
	C_AnimState *AnimState()
	{
		return *reinterpret_cast<C_AnimState**>(uintptr_t(this) + 0x3900);
	}

	c_animstate *AnimStatev2()
	{
		return *reinterpret_cast<c_animstate**>(uintptr_t(this) + 0x3900);
	}
	AnimationLayer* GetAnimOverlays()
	{
		// to find offset: use 9/12/17 dll
		// sig: 55 8B EC 51 53 8B 5D 08 33 C0
		return *(AnimationLayer**)((DWORD)this + 0x2980);
	}
	AnimationLayer *GetAnimOverlay4(int i)
	{
		if (i < 16)
			return &GetAnimOverlays()[i];
	}

	matrix3x4_t GetBoneMatrix(int BoneID)
	{
		matrix3x4_t matrix;

		uintptr_t m_dwBoneMatrix = 0x2698;
		auto offset = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + m_dwBoneMatrix);
		if (offset)
			matrix = *reinterpret_cast<matrix3x4_t*>(offset + 0x30 * BoneID);

		return matrix;
	}

	NETVAR(int32_t, m_iViewModelIndex, "DT_BaseCombatWeapon", "m_iViewModelIndex");
	NETVAR(float, m_flDuckAmount, "DT_BasePlayer", "m_flDuckAmount");
	int GetObserverTargetHandle()
	{
		static int m_iTeamNum = NetvarManager::Instance()->GetOffset("DT_BasePlayer", "m_hObserverTarget");
		return GetValue1<int>(m_iTeamNum);
	}

	/*
	CHandle* GetObserverTargetHandle()
	{
		return (CHandle*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BasePlayer", "m_hObserverTarget"));
	}*/
	CBaseHandle* m_hMyWeapons()
	{
		return (CBaseHandle*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BaseCombatCharacter", "m_hMyWeapons"));
	}
	std::array<float, 24>& m_flPoseParameterNEW();
	void ResetAnimationState(C_AnimState *state);
	bool is_enemy();
	bool is_local_player() const;

	void CreateAnimationState(C_AnimState *state);

	float * m_flPoseParameter();

	
	Vector get_ragdoll_pos()
	{
		static int get_ragdoll_pos = NetvarManager::Instance()->GetOffset("DT_Ragdoll", "m_ragPos");
		return GetFieldValue<Vector>(get_ragdoll_pos);
	}
	void ClientAnimations(bool value)
	{
		static int m_bClientSideAnimation = NetvarManager::Instance()->GetOffset("DT_BaseAnimating", "m_bClientSideAnimation");
		*reinterpret_cast<bool*>(uintptr_t(this) + m_bClientSideAnimation) = value;
	}

	void UpdateClientAnimation()
	{
		typedef void(__thiscall* original)(void*);
		Utils::GetVFunc<original>(this, 223)(this);
	}
	bool& client_side_animation()
	{
		static int m_bBombDefused = NetvarManager::Instance()->GetOffset("DT_BaseAnimating", "m_bClientSideAnimation");
		return *(bool*)((DWORD)this + m_bBombDefused);
	}
	void SetAbsAngles(Vector angles);
	void SetAbsOrigin(const Vector &origin);
	float DesyncValue();
	
	Vector get_abs_angles()
	{
		return Utils::GetVFunc<Vector & (__thiscall*)(void*)>(this, 11)(this);
	}

	Vector GetAbsOrigin()
	{
		return Utils::GetVFunc<Vector&(__thiscall*)(void*)>(this, 10)(this);
	}

	void SetAbsVelocity(Vector velocity);
	void SetAbsOriginal(Vector origin);

	float GetThrowTime()
	{
		static int m_fThrowTime = NetvarManager::Instance()->GetOffset("DT_BaseCSGrenade", "m_fThrowTime");
		return GetValue<float>(m_fThrowTime);
	}

	bool IsPinPulled()
	{
		static int m_bPinPulled = NetvarManager::Instance()->GetOffset("DT_BaseCSGrenade", "m_bPinPulled");
		return GetValue<bool>(m_bPinPulled);
	}
    C_BaseCombatWeapon* GetActiveWeapon()
    {
        static int m_hActiveWeapon = NetvarManager::Instance()->GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
        const auto weaponData      = GetValue<CBaseHandle>(m_hActiveWeapon);
        return reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntityFromHandle(weaponData));
    }
	int GetActiveWeaponIndex()
	{
		static int m_hActiveWeapon = NetvarManager::Instance()->GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
		return *reinterpret_cast<int*>(uintptr_t(this) + m_hActiveWeapon) & 0xFFF; //m_hActiveWeapon
	}
    int GetTeam()
    {
        static int m_iTeamNum = OFFSETS::m_iTeamNum;
        return GetValue1<int>(m_iTeamNum);
    }

	NETVAR(int32_t, m_fFlags, "CBasePlayer", "m_fFlags");
	NETVAR(EntityFlags, GetFlags, "DT_BasePlayer", "m_fFlags");
	NETVAR(int,flags, "CBasePlayer", "m_fFlags")
	NETVAR(float, GetSimulationTime, "DT_BaseEntity", "m_flSimulationTime");
	//m_flSimulationTime = NetvarManager::Instance()->GetOffset("DT_BaseEntity", "m_flSimulationTime");
	bool is_grounded(std::optional<int32_t> flags = std::nullopt)
	{
		return flags.value_or(m_fFlags()) & FL_ONGROUND;
	}

	float m_flOldSimulationTime()
	{
		static int m_flSimulationTime = 0x264;
		return *(float*)((DWORD)this + (m_flSimulationTime + 0x4));
	}
	float GetLowerBodyYaw()
	{
		static int m_flLowerBodyYawTarget = OFFSETS::m_flLowerBodyYawTarget;
		return GetValue4<float>(m_flLowerBodyYawTarget);
	}


	void SetLowerBodyYaw(float value)
	{
		static int m_flLowerBodyYawTarget = OFFSETS::m_flLowerBodyYawTarget;
		*reinterpret_cast<float*>(uintptr_t(this) + m_flLowerBodyYawTarget) = value;
	}

	bool GetHeavyArmor()
	{
		static int m_bHasHeavyArmor = NetvarManager::Instance()->GetOffset("DT_CSPlayer", "m_bHasHeavyArmor");
		return GetValue5<bool>(m_bHasHeavyArmor);
	}

	int ArmorValue()
	{
		static int m_ArmorValue = OFFSETS::m_ArmorValue;
		return GetValue6<int>(m_ArmorValue);
	}

	bool HasHelmet()
	{
		static int m_bHasHelmet = OFFSETS::m_bHasHelmet;
		return GetValue7<bool>(m_bHasHelmet);
	}

	NETVAR(bool, GetLifeState, "DT_BasePlayer", "m_lifeState") //dont access this directly, use IsAlive()


	bool IsScoped()
	{
		static int m_bIsScoped = OFFSETS::m_bIsScoped;
		return GetValue9<bool>(m_bIsScoped);
	}
	

	int GetHealth()
	{
		return *reinterpret_cast<int*>(uintptr_t(this) + OFFSETS::m_iHealth);
	}
	bool IsKnifeorNade();

	move_type GetMoveType()
	{
		return *(move_type*)((unsigned long)this + 0x25C);
	}
	
	bool IsAlive() const;

	NETVAR(float, m_flFlashDuration, "DT_CSPlayer", "m_flFlashDuration");


	bool IsEnemy();

    bool IsImmune()
    {
        static int m_bGunGameImmunity = OFFSETS::m_bGunGameImmunity;
        return GetValue11<bool>(m_bGunGameImmunity);
    }
    NETVAR(int32_t, m_nTickBase, "CBasePlayer", "m_nTickBase");

    int GetTickBase()
    {
		static int m_nTickBase = OFFSETS::m_nTickBase;
		return GetValue12<int>(m_nTickBase);
    }



	void SetTickBase(int TickBase)
	{
		static int m_nTickBase = OFFSETS::m_nTickBase;
		*reinterpret_cast<int*>(uintptr_t(this) + m_nTickBase) = TickBase;
	}
	bool IsMoving()
	{
		if (this->GetVelocity().Length() > 50)
			return true;
		return false;
	}
	Vector* GetEyeAnglesPtr()
	{
		return reinterpret_cast<Vector*>((DWORD)this + OFFSETS::m_angEyeAngles);

	}
	Vector GetEyeAngles()
	{
		static int m_angEyeAngles = OFFSETS::m_angEyeAngles;
		return GetValue13<Vector>(m_angEyeAngles);
	}

	Vector GetViewOffset()
	{
		static int m_vecOrigin = OFFSETS::m_vecViewOffset;
		return GetValue14<Vector>(m_vecOrigin);
	}

    Vector GetOrigin()
    {
        static int m_vecOrigin = OFFSETS::m_vecOrigin;
        return GetValue14<Vector>(m_vecOrigin);
    }
	void setVelocity(Vector velocity)
	{
		static int m_vecVelocity = OFFSETS::m_vecVelocity;

		*(Vector*)((unsigned long)this + m_vecVelocity) = velocity;
	}
	
	Vector get_eye_pos()
	{
		return GetOrigin() + GetViewOffset();
	}
	void* networkable() {
		return reinterpret_cast<void*>(uintptr_t(this) + 0x8);
	}
	void net_pre_data_update(int update_type)
	{
		using original_fn = void(__thiscall*)(void*, int);
		return (*(original_fn**)networkable())[6](networkable(), update_type);
	}
	Vector GetVelocity()
	{
		static int m_vecVelocity = OFFSETS::m_vecVelocity;
		return GetValue15<Vector>(m_vecVelocity);
	}

	UINT* get_wearables()
	{
		static int m_wearables = OFFSETS::m_hMyWearables;

		return (UINT*)((uintptr_t)this + m_wearables);
	}
	CBaseHandle* m_hMyWearables()
	{
		static int m_hMyWearables = OFFSETS::m_hMyWearables;

		return (CBaseHandle*)((uintptr_t)this + m_hMyWearables);
	}
	Vector GetAimPunchAngle()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + uintptr_t(0x302C));
	}

	Vector GetViewPunchAngle()
	{
		return *reinterpret_cast<Vector*>(uintptr_t(this) + uintptr_t(0x3020));
	}

    

	ICollideable* GetCollideable()
	{
		return (ICollideable*)((DWORD)this + 0x318);
	}

	void setAbsAngle(Vector Angle);
	void set_abs_origin(const Vector & origin);
	void SetCurrentCommand(CUserCmd *cmd)
	{
		static int m_hConstraintEntity = NetvarManager::Instance()->GetOffset("DT_BasePlayer", "localdata", "m_hConstraintEntity");
		*reinterpret_cast<CUserCmd**>(uintptr_t(this) + m_hConstraintEntity - 0xC) = cmd;
	}
	float FireRate();

	void FixSetupBones(matrix3x4_t *Matrix);
	Vector GetHitboxPosition(int Hitbox, matrix3x4_t *Matrix, float *Radius);
	Vector GetHitboxPosition(int Hitbox, matrix3x4_t *Matrix);
	bool IsPlayer();
	Vector hitbox_position(int hitbox_id);
	bool IsValidTarget();

	bool valid(bool check_team, bool check_dormant);
	bool is_player();
	bool IsShotgun(void* weapon);
	bool IsMachinegun(void* weapon);
	bool TestVisuals(void* weapon);

	bool legitbot(void* weapon);
};

class C_CSPlayer : public C_BaseEntity
{
public:
	static C_CSPlayer* GetLocalPlayer();
public:
	QAngle& m_angEyeAngles();
};

struct c_cs_inferno : public C_BaseEntity
{
	float& get_entity_spawn_time() {
		return *(float*)((uintptr_t)this + 0x20);
	}
	int& m_DmgRadius();
	int& m_flDamage();

	C_BaseEntity* m_hThrower();

	static constexpr float expire_time = 7.f;
};

class C_EconItemView
{
private:
    using str_32 = char[32];
public:
    NETVAR(int32_t, m_bInitialized, "DT_BaseAttributableItem", "m_bInitialized");
    NETVAR(short, m_iItemDefinitionIndex, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
    NETVAR(int32_t, m_iEntityLevel, "DT_BaseAttributableItem", "m_iEntityLevel");
    NETVAR(int32_t, m_iAccountID, "DT_BaseAttributableItem", "m_iAccountID");
    NETVAR(int32_t, m_iItemIDLow, "DT_BaseAttributableItem", "m_iItemIDLow");
    NETVAR(int32_t, m_iItemIDHigh, "DT_BaseAttributableItem", "m_iItemIDHigh");
    NETVAR(int32_t, m_iEntityQuality, "DT_BaseAttributableItem", "m_iEntityQuality");
    NETVAR(str_32, m_iCustomName, "DT_BaseAttributableItem", "m_szCustomName");
};

class C_BaseAttributableItem : public C_BaseEntity
{
public:
    NETVAR(uint64_t, m_OriginalOwnerXuid, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
    NETVAR(int32_t, m_OriginalOwnerXuidLow, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
    NETVAR(int32_t, m_OriginalOwnerXuidHigh, "DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
    NETVAR(int32_t, m_nFallbackStatTrak, "DT_BaseAttributableItem", "m_nFallbackStatTrak");
    NETVAR(int32_t, m_nFallbackPaintKit, "DT_BaseAttributableItem", "m_nFallbackPaintKit");
    NETVAR(int32_t, m_nFallbackSeed, "DT_BaseAttributableItem", "m_nFallbackSeed");
    NETVAR(float_t, m_flFallbackWear, "DT_BaseAttributableItem", "m_flFallbackWear");

    NETVAR(C_EconItemView, m_Item2, "DT_BaseAttributableItem", "m_Item");

    C_EconItemView& m_Item()
    {
        // Cheating. It should be this + m_Item netvar but then the netvars inside C_EconItemView wont work properly.
        // A real fix for this requires a rewrite of the netvar manager
        return *(C_EconItemView*)this;
    }
    void SetModelIndex(const int index)
    {
        return qCallVFunction<void(__thiscall*)(C_BaseEntity*, int)>(this, 75)(this, index);
    }
    void SetGloveModelIndex(int modelIndex);

};


class C_BaseViewModel2 : public C_BaseEntity
{
public:
    NETVAR(int32_t, m_nViewModelIndex, "DT_BaseViewModel", "m_nViewModelIndex");
    NETVAR(CHandle<C_BaseCombatWeapon>, m_hWeapon, "DT_BaseViewModel", "m_hWeapon");
    NETVAR(CHandle<C_BaseEntity>, m_hOwner, "DT_BaseViewModel", "m_hOwner");
  //  NETPROP(m_nSequence, "DT_BaseViewModel", "m_nSequence");
   //void SendViewModelMatchingSequence(int sequence);
};

class C_BaseWeaponWorldModel : public C_BaseEntity
{
public:
    NETVAR(int32_t, m_nModelIndex, "DT_BaseWeaponWorldModel", "m_nModelIndex");
};


class C_BaseCombatWeapon : public C_BaseEntity
{
private:
    template<class T>
    T GetPointer(const int offset)
    {
        return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
    // To get value from the pointer itself
    template<class T>
    T GetValue(const int offset)
    {
        return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
    }

public:
    //NETVAR(auto,weapons, "CBaseCombatCharacter", "m_hMyWeapons")
    bool isInReload()
    {
        return *reinterpret_cast<bool*>(uintptr_t(&clip()) + 0x41);
    }
    NETVAR(int32_t, m_nFallbackStatTrak2, "DT_BaseAttributableItem", "m_nFallbackStatTrak");
	CBaseHandle m_hWeaponWorldModel_c()
	{
		return *(CBaseHandle*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BaseCombatWeapon", "m_hWeaponWorldModel")); //m_hWeaponWorldModel
	}

	HANDLE m_hWeaponWorldModel_h()
	{
		return *(HANDLE*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BaseCombatWeapon", "m_hWeaponWorldModel")); //m_hWeaponWorldModel

	}
	float GetNextPrimaryAttack()
	{
		return *reinterpret_cast<float*>(uintptr_t(this) + NetvarManager::Instance()->GetOffset("DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack"));
	}
	bool isAuto()
	{
		short weapon_id = *this->m_iItemDefinitionIndex();
		return weapon_id == (int)ItemDefinitionIndex::WEAPON_SCAR20 || (int)ItemDefinitionIndex::WEAPON_G3SG1;
	}
	bool is_awp()
	{
		short weapon_id = *this->m_iItemDefinitionIndex();
		return weapon_id == (int)ItemDefinitionIndex::WEAPON_AWP;
	}
	bool is_scout()
	{
		short weapon_id = *this->m_iItemDefinitionIndex();
		return weapon_id == (int)ItemDefinitionIndex::WEAPON_SSG08;
	}
	bool is_grenade();

	bool is_knife();
	bool is_non_aim();
	char * get_name();

	int* m_nModelIndex()
	{
		return (int*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BaseViewModel", "m_nModelIndex"));
	}
	int* ViewModelIndex()
	{
		return (int*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BaseCombatWeapon", "m_iViewModelIndex"));
	}
	int* OwnerXuidLow()
	{
		return (int*)((uintptr_t)this + OFFSETS::m_OriginalOwnerXuidLow);
	}
	int* OwnerXuidHigh()
	{
		return (int*)((uintptr_t)this + OFFSETS::m_OriginalOwnerXuidHigh);
	}
	float* m_flFallbackWear()
	{
		return (float*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BaseAttributableItem", "m_flFallbackWear"));
	}
	
	int* m_iItemIDHigh()
	{
		return (int*)((uintptr_t)this + OFFSETS::m_iItemIDHigh);
	}
	inline int* FallbackPaintKit() 
	{
		return (int*)((DWORD)this + NetvarManager::Instance()->GetOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit"));
	}

	short* fixskins()
	{
		return (short*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex"));
	}

	short* GetItemDefinitionIndex()
	{
		return (short*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex"));
	}
   

	float GetInaccuracy()
	{
		typedef float(__thiscall* oInaccuracy)(PVOID);
		return Utils::GetVFunc< oInaccuracy >(this, 482)(this);
	}
	CBaseHandle m_hWeaponWorldModel()
	{
		return *(CBaseHandle*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BaseCombatWeapon", "m_hWeaponWorldModel")); //m_hWeaponWorldModel
	}
	HANDLE m_hWeaponWorldModel1()
	{
		return *(HANDLE*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BaseCombatWeapon", "m_hWeaponWorldModel")); //m_hWeaponWorldModel
	}

	float GetSpread()
	{
		typedef float(__thiscall* oWeaponSpread)(PVOID);
		return Utils::GetVFunc< oWeaponSpread >(this, 452)(this);
	}

	float GetAccuracyPenalty()
	{
		static int m_fAccuracyPenalty = NetvarManager::Instance()->GetOffset("DT_WeaponCSBase", "m_fAccuracyPenalty");
		return GetValue<float>(m_fAccuracyPenalty);
	}

	float GetLastShotTime()
	{
		static int m_fLastShotTime = NetvarManager::Instance()->GetOffset("DT_WeaponCSBase", "m_fLastShotTime");
		return GetValue<float>(m_fLastShotTime);
	}

	void AccuracyPenalty()
	{
		typedef void(__thiscall *OrigFn)(void *);
		return Utils::GetVFunc<OrigFn>(this, 483)(this);
	}


	int* m_nFallbackStatTrak()
	{
		return (int*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BaseAttributableItem", "m_nFallbackStatTrak"));
	}


	NETVAR(int32_t, get_zoom_level, "DT_WeaponCSBaseGun", "m_zoomLevel");

	short* m_iItemDefinitionIndex()
	{
		return (short*)((uintptr_t)this + NetvarManager::Instance()->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex"));
	}
	
	bool IsKnife();

    int GetAmmo()
    {
        static int m_iClip1 = NetvarManager::Instance()->GetOffset("DT_BaseCombatWeapon", "m_iClip1");
        return GetValue<int>(m_iClip1);
    }
	int GetAmmoFull()
	{
		static int m_iClip1 = NetvarManager::Instance()->GetOffset("DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount");
		return GetValue<int>(m_iClip1);
	}
    WeaponInfo_t* GetCSWpnData()
    {
        return Utils::CallVFunc<460, WeaponInfo_t*>(this);
    }

    std::string GetName()
    {
        ///TODO: Test if szWeaponName returns proper value for m4a4 / m4a1-s or it doesnt recognize them.
        return std::string(this->GetCSWpnData()->weapon_name);
    }
};
class CGloves : public C_BaseCombatWeapon {
public:
	/*
	void PreDataUpdate(int updateType)
	{
		PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return Utils::GetVFunc<OriginalFn>(pNetworkable, 6)(pNetworkable, updateType);
	}

	void SetGloveModelIndex(int modelIndex)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		Utils::GetVFunc<OriginalFn>(this, 75)(this, modelIndex);
	}*/
};


class C_BaseViewModel
{
public:

	inline DWORD m_howner()
	{
		return *(PDWORD)((DWORD)this + NetvarManager::Instance()->GetOffset("DT_BaseViewModel", "m_hOwner"));
	}


	inline int GetModelIndex()
	{
		//	return *(int*)((DWORD)this + NetvarManager::Instance()->GetOffset("DT_BaseEntity", "m_nModelIndex"));
		return *(int*)((DWORD)this + NetvarManager::Instance()->GetOffset("DT_BaseViewModel", "m_nModelIndex"));

	}
};
