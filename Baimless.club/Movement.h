#pragma once
#include "Singleton.h"
#include "SDK/IGameEvent.h"

#include "SDK/CEntity.h"
#include "SDK/ICvar.h"
#include "SDK/CGlobalVarsBase.h"
#include "Utils/GlobalVars.h"
#include "Utils/Math.h"
#include "Options.hpp"
class Movement : public Singleton<Movement>
{
public:
	float get_move_angle(float speed);;
	void predict_velocity(Vector* velocity);
	void RotateMovement(CUserCmd* cmd, float yaw);
	void quick_stop(CUserCmd* cmd);
	void autostrafe(CUserCmd* cmd);
	void bhop(CUserCmd* cmd);
	void Begin(CUserCmd* cmd, bool& send_packet);
	void End();

	Vector m_qRealAngles = {};
	Vector m_qAnglesView = {};
private:
	CUserCmd* m_pCmd = nullptr;
	C_CSPlayer* m_pPlayer = nullptr;

	Vector m_qAngles = { };
	Vector m_qAnglesLast = { };
};