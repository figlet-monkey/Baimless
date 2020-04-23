#include "GlobalVars.h"
#include "../Render.h"
namespace g
{
	ImDrawList* notify = nullptr;
	unsigned long g_iModelBoneCounter;
    CUserCmd*      pCmd         = nullptr;
    C_BaseEntity*  pLocalEntity = nullptr;
	C_BaseEntity* enemy_aimbot = nullptr;

    std::uintptr_t uRandomSeed  = NULL;
	Vector         OriginalView;
	bool           bSendPacket;
	bool           fake_walk = false;
	bool warmup = false;

	int chokedPackets;
	bool		   LagPeek      = false;
	int            TargetIndex  = -1;
	Vector         AimbotHitbox[65][28];
	Vector AimbotAngle;
	Vector         RealAngle;
	Vector         ThirdPerson;
	bool isfakeducking = false;
	bool           inthird_person = false;
	float gayside;
	DWORD TahomaSmall;
	bool needtoshift = false;

	Vector         FakeAngle;
	float fake_strength;
	int kills = 0;  float killtime = 0.0f;
	int globalAlpha = 0;

	DWORD Tahoma;
	DWORD MenuBold;
	DWORD Untitled1;
	DWORD Menu;
	DWORD Tabs;
	DWORD grenadefont;
	DWORD pixelfont;
	DWORD visuals_lby_font;
	DWORD LBY2;
	DWORD LBY1;
	DWORD SmallText;
	DWORD CourierNew;
	DWORD WeaponIcon;
	DWORD WeaponESP;
	DWORD xd;
	DWORD Arial;
	DWORD INDICATOR;
	DWORD key_font;

	DWORD menu_window_font3;
	float          w2s_matrix[4][4];
}
