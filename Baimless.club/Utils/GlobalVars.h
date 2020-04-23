#pragma once
#include "..\SDK\CInput.h"
#include "..\SDK\CEntity.h"
#include "../Render.h"
namespace g
{
	extern unsigned long g_iModelBoneCounter;
	extern ImDrawList* notify;
    extern CUserCmd*      pCmd;
    extern C_BaseEntity*  pLocalEntity;
	extern C_BaseEntity* enemy_aimbot;

    extern std::uintptr_t uRandomSeed;
	extern Vector         OriginalView;
	extern bool           bSendPacket;
	extern bool           fake_walk;
	extern bool           inthird_person;
	extern bool needtoshift;
	extern bool warmup;

	extern int chokedPackets;
	extern bool isfakeducking;
	extern float gayside;

	extern bool			  LagPeek;
	extern int            TargetIndex;
	extern Vector         AimbotHitbox[65][28];
	extern Vector         RealAngle;
	extern Vector AimbotAngle;

	extern Vector         ThirdPerson;
	extern Vector         FakeAngle;
	extern float	      fake_strength;
	extern int kills; extern float killtime;
	extern int globalAlpha;
	extern DWORD TahomaSmall;

	extern DWORD Tahoma;
	extern DWORD MenuBold;
	extern DWORD Untitled1;
	extern DWORD Menu;
	extern DWORD Tabs;
	extern DWORD grenadefont;
	extern DWORD pixelfont;
	extern DWORD visuals_lby_font;
	extern DWORD LBY2;
	extern DWORD LBY1;
	extern DWORD SmallText;
	extern DWORD CourierNew;
	extern DWORD WeaponIcon;
	extern DWORD WeaponESP;
	extern DWORD xd;
	extern DWORD Arial;
	extern DWORD INDICATOR;
	extern DWORD menu_window_font3;
	extern DWORD key_font;

	extern                float w2s_matrix[4][4];
}