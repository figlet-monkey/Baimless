#pragma once
#include "..\..\Utils\GlobalVars.h"

class CVisuals
{
public:
	void Draw();
	void penetration_reticle();
	void watermark();

	bool world_to_screen(const Vector &world, Vector &screen);
	RECT viewport();
	void DrawBox(C_BaseEntity* entity, Color color, Vector pos, Vector top);
	void DrawName(C_BaseEntity* entity, Color color, int index, Vector pos, Vector top);
	void DrawHealth(C_BaseEntity* entity, Color color, Color dormant, Vector pos, Vector top);
	void DrawAmmo(C_BaseEntity * entity, Color color, Color dormant, Vector pos, Vector top);
	void DrawWeapon(C_BaseEntity * entity, Color color, int index, Vector pos, Vector top);
	void DrawIndicator();
	void spectators();
	void RenderSkeleton(C_BaseEntity* pEnt, Color color);
	void DrawFeetRange();
	void DrawZeusRange();
	void QuakeKillCounter();

};

extern CVisuals* visuals;

/* old visuals
struct Box {
	int x = -1, y = -1, w = -1, h = -1;
};


class ESP
{
public:
	float esp_alpha_fade[64];

	void paint_traverse();
	bool get_bbox(C_BaseEntity * e, Box & box);
	bool world_to_screen(const Vector &origin, Vector &screen);
	RECT viewport();
	void draw_box(C_BaseEntity* m_entity, Box box);
	void draw_name(C_BaseEntity* m_entity, Box box);
	void draw_health(C_BaseEntity* m_entity, Box box);
	void draw_ammobar(C_BaseEntity* m_entity, Box box);
	void draw_weapon(C_BaseEntity* m_entity, Box box);

};
extern ESP g_ESP; */