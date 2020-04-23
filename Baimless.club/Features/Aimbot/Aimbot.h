#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"
#include <deque>
#include "LagComp.h"

extern bool overridething;

extern bool overridething_legit;

extern bool overridething_dt_key;


class c_prediction 
{
public:
	void start_prediction(CUserCmd* command) noexcept;
	void end_prediction() noexcept;
	float old_cur_time;
	float old_frame_time;
	int* prediction_random_seed;
};

extern c_prediction engine_prediction;

class Aimbot
{
public:
	int bestEntDmg;
	matrix3x4_t Matrix[65][128];
	bool can_autowall = false;

	struct
	{
		int shoots, shoots_hit, shotsmissed;
	}data[65];
	bool aimbotted = false;
	void CompensateInaccuracies();
	bool HitChance(C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance);

	void auto_revolver(CUserCmd* cmd);
	/////
	int zeus_hitbox(C_BaseEntity* entity);
	Vector get_hitbox_pos(C_BaseEntity* entity, int hitbox_id);

	int Sequence;
	void OnCreateMove();

private:
	bool Backtrack[65];
	bool ShotBacktrack[65];
	Vector Hitscan(C_BaseEntity* pEnt);
	void Autostop();

};
extern Aimbot g_Aimbot;

class KnifeBot
{
public:
	void Run();

};
extern KnifeBot g_Knifebot;

class chams123 : public Singleton<chams123>
{
public:



	void dark_mode() const;
};
/*
class AimLegit : public Singleton<AimLegit>
{
public:

	void Work(CUserCmd * usercmd);



	void ChangeName(const char * name);

	void NameChanger();

	void GetBestTarget();
	inline void setclantag(const char* tag)
	{
		static auto ClanTagOffset = Utils::PatternScan(GetModuleHandleW(L"engine.dll"), "53 56 57 8B DA 8B F9 FF 15");
		if (ClanTagOffset)
		{
			auto tag_ = std::string(tag);
			if (strlen(tag) > 0) {
				auto newline = tag_.find("\\n");
				auto tab = tag_.find("\\t");
				if (newline != std::string::npos) {
					tag_.replace(newline, newline + 2, "\n");
				}
				if (tab != std::string::npos) {
					tag_.replace(tab, tab + 2, "\t");
				}
			}
			static auto dankesttSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(ClanTagOffset);
			dankesttSetClanTag(tag_.data(), tag_.data());
		}
	}

	int changes = -1;
private:
	inline void SetTarget(int idx) { m_iTarget = idx; }
	inline bool HasTarget() { return m_iTarget != -1; }
	bool CheckTarget(C_BasePlayer *player);
	void TargetRegion();
	void RecoilControlSystem();
	int m_iTarget = -1;
	CUserCmd *usercmd;

	const char *setStrRight(std::string txt, unsigned int value);
	std::string nazevclanu = "Sp1Ky    ";

	QAngle m_aimPunchAngle[128];

	int32_t choked = 0;
};
using LineGoesThroughSmokeFn = bool(__cdecl*)(Vector, Vector, int16_t);
extern LineGoesThroughSmokeFn LineGoesThroughSmoke; */

class CLegitBot
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool& bSendPacket);
	bool issilent;
	float FoV;
	bool issniper;
private:
	// Targetting
	int GetTargetCrosshair();
	bool TargetMeetsRequirements(C_BaseEntity* pEntity);
	float FovToPlayer(Vector ViewOffSet, Vector View, C_BaseEntity* pEntity, int HitBox);
	bool AimAtPoint(C_BaseEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket);
    void SIMPLE(C_BaseCombatWeapon* pWeapon, C_BaseEntity* pLocal);
    void ADVANCED(void* weapon);
	void SyncWeaponSettings();
	void StandAloneRCS(C_BaseEntity* pLocal, CUserCmd* pCmd, C_BaseCombatWeapon* pWeapon);

	// Functionality
	void DoAimbot(CUserCmd *pCmd, bool &bSendpacket);

	// Aimbot
	bool IsLocked;
	int TargetID;
	int HitBox;
	Vector AimPoint;
	bool Multihitbox;
	int besthitbox;
	Vector ViewOffset;
	Vector View;
	float Speed;
	
	float RecoilControl;
	float Inacc;
	
	bool shoot;

};

extern CLegitBot g_legitbot;

class CTriggerBot
{
public:
	void Move(CUserCmd *pCmd);
private:
	void DoTrigger(CUserCmd *pCmd);
	bool Trigger_HitChance(C_BaseEntity* pEnt, C_BaseCombatWeapon* pWeapon, Vector Angle, Vector Point, int chance);

};
extern CTriggerBot TriggerBot;


