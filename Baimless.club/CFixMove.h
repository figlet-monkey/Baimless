#pragma once
#include "Hooks.h"

class CFixMove {
public:
	void Start(CUserCmd*);
	void Stop(CUserCmd*);
	Vector GetOldAngle();
private:
	Vector old;
	float oldFowardMove;
	float oldSideMove;

	void CorrectMovement(Vector vOldAngles, CUserCmd* pCmd, float fOldForward, float fOldSidemove);
};
