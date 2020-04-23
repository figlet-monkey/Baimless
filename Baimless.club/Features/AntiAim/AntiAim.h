#pragma once
#include "..\..\Utils\GlobalVars.h"
#include "..\..\SDK\CGlobalVarsBase.h"



class c_antiaim
{
public:
	void fakelag(C_BaseEntity* local, CUserCmd* cmd, bool& send_packet);
	void run(C_BaseEntity* local, CUserCmd* cmd, bool& send_packet);
	void prepare_animation(C_BaseEntity* local);
	void predict(C_BaseEntity* local, CUserCmd* cmd);
	void shooting(C_BaseEntity* local, CUserCmd* cmd, float ideal_yaw, int direction, bool send_packet);

	float get_pitch(C_BaseEntity* local, CUserCmd* cmd);
	float get_yaw(C_BaseEntity* local, CUserCmd* cmd, float ideal_yaw, int direction);

	void increment_visual_progress();

	float get_visual_choke();
	float get_last_real();
	float get_last_fake();

	uint32_t shot_cmd{};

	matrix3x4_t last_fake_matrix[128] = { matrix3x4_t() };
	matrix3x4_t last_real_matrix[128] = { matrix3x4_t() };

	float last_real = 0.f, last_fake = 0.f;

	bool is_fakeducking = false;
	bool is_slow_walking = false;
	bool is_lby_broken = false;

	float visual_choke = 0.f,
		next_lby_update = 0.f, lby_update = 0.f,
		min_delta = 0.f, max_delta = 0.f,
		stop_to_full_running_fraction = 0.f,
		feet_speed_stand = 0.f, feet_speed_ducked = 0.f;

	uint32_t estimated_choke = 0;

private:
	bool on_peek(C_BaseEntity* local, bool& target);
	float calculate_ideal_yaw(C_BaseEntity* local, bool estimate = false);

	bool is_standing = false;
};
extern c_antiaim AntiAimPro;


class antiaim
{
public:
	void do_antiaim(CUserCmd* cmd);
	float MaxDelta(C_BaseEntity* pEnt);

	float GetYaw();
	float GetFakeYaw();
	float_t m_flNextBodyUpdate = 0.f;
	void UpdateLBY();
	void BreakLBY(float real_yaw, float desired_lby);
	float m_next_lby_update_time = 0.f, m_last_move_time = 0.f, m_last_attempted_lby = 0.f;
	bool m_will_lby_update = false;
	uint32_t shot_cmd{};

	void UpdateAnimationState(C_AnimState* state, Vector angle);
	void ResetAnimationState(C_AnimState* state);
	void CreateAnimationState(C_AnimState* state, C_BaseEntity* player);
	void StaticBodyYaw(CUserCmd* cmd);

	void FakeWalk(CUserCmd* pCmd);
	void fake_crouch();
	float get_pitch();
	float at_target();
	float get_max_desync_delta();
	template<class T, class U>
	static T clam21213p(T in, U low, U high) {
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}

};
extern antiaim AntiAimNEW;


class animation_state
{
public:
	void create_move();

	void create_state(C_AnimState* state, C_BaseEntity* player);
	void update_state(C_AnimState* state, Vector angles);
	void reset_state(C_AnimState* state);

	C_AnimState* animstate();
};

extern animation_state AnimStateNew;


static constexpr int MAX_CHOKE = 14;
static constexpr float TELEPORT_DISTANCE = 64.f;

class CFakeLag
{
public:
	int Fakelag_AdaptiveFactor();
	void do_fakelag(CUserCmd* cmd, C_BaseEntity* local);
	void fakeduck(CUserCmd* cmd);
	void fake_crouch(CUserCmd* cmd);
	void fakeduck2(CUserCmd* cmd, C_BaseEntity* local);
	void on_peek(bool canhit);
};

extern CFakeLag* fakelag;