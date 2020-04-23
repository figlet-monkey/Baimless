#include "Visuals.h"
#include "Baimless.club/Features/Aimbot/Aimbot.h"

#include "Baimless.club/Features/Aimbot\LagComp.h"
#include "Baimless.club/Utils\Utils.h"
#include "Baimless.club/SDK\IVEngineClient.h"
#include "Baimless.club/SDK\PlayerInfo.h"
#include "Baimless.club/SDK\ISurface.h"
#include "Baimless.club/SDK\Hitboxes.h"
#include "Baimless.club/Utils\Math.h"
#include "Baimless.club/newMenu.h"
#include "Baimless.club/SDK/ICvar.h"
#include "algorithm"
#include "Baimless.club/Features/Aimbot/Autowall.h"
#include "Baimless.club/SDK\IVModelInfo.h"
#include "Drop.h"
#include "Baimless.club/Options.hpp"
#include "Baimless.club/Render2.h"

float StoredCurtimePlayer[1024];
float flPlayerAlpha[1024];
std::vector<_event> _events;

void logs()
{
	int bottom = 0;

	

	//if (cheat::Cvars.Visuals_misc_logthings.selected <= 0) {
	//	_events.clear();
	//	return;
	//}

	if (_events.empty())
		return;

	//static auto name_siz = Drawing::GetTextSize(F::ESP, "[gangster]");

	int x = 8;
	int y = 5;
	auto count = 0;
	int fontTall = g_pSurface->GetFontTall(g::TahomaSmall) + 1;

	for (auto& event : _events)
	{
		if (_events.back()._time < g_pGlobalVars->curtime)
			_events.pop_back();

		if (event._time < g_pGlobalVars->curtime && event._displayticks > 0.f)
			continue;

		if (count > 10)
			break;

		if (event._msg.size() > 0)
		{


			Color clr;
			
			float timeleft = fabs(event._time - g_pGlobalVars->curtime);

			if (timeleft < .5f)
			{
				float f = g_Math.clamp(timeleft, 0.0f, .5f) / .5f;

				//clr.a = (int)(f * 255.0f);
				clr = Color(255, 255, 255, (int)(f * 255.0f));
				if (count == 0 && f < 0.2f)
				{
					y -= fontTall * (1.0f - f / 0.2f);
				}
			}
			else
			{

				clr = Color(255, 255, 255, 255);
			}
			
			//DrawColoredText(m_hFontFixed, x, y, clr[0], clr[1], clr[2], clr[3], notify->text);
			g_pSurface->text(g::TahomaSmall, x, y, clr, HFONT_CENTERED_NONE, "%s", event._msg.c_str());

			y += fontTall;

			count++;
		}
	}
}

bool WorldToScreen(const Vector& world, Vector& screen) {
	auto screen_transform = [&world, &screen]() -> bool {
		static auto& matrix = g_pEngine->WorldToScreenMatrix();

		screen.x = matrix[0][0] * world.x + matrix[0][1] * world.y + matrix[0][2] * world.z + matrix[0][3];
		screen.y = matrix[1][0] * world.x + matrix[1][1] * world.y + matrix[1][2] * world.z + matrix[1][3];
		screen.z = 0.0f;

		float w = matrix[3][0] * world.x + matrix[3][1] * world.y + matrix[3][2] * world.z + matrix[3][3];

		if (w < 0.001f) {
			screen.x *= 100000;
			screen.y *= 100000;
			return false;
		}

		screen.x /= w;
		screen.y /= w;

		return true;
	};

	if (screen_transform()) {
		int w, h;
		g_pEngine->GetScreenSize(w, h);

		screen.x = (w / 2.0f) + (screen.x * w) / 2.0f;
		screen.y = (h / 2.0f) - (screen.y * h) / 2.0f;

		return true;
	}

	return false;
}

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#include <minwindef.h>

void DrawOctagonBox(int x, int y, int w, int h, Color color)
{
	int iw = w / 4;
	int ih = h / 4;


	//top part
	g_pSurface->Line(x, y, x + w, y, color); //top line
	g_pSurface->Line((x + w) + iw, y + ih, x + w, y, color); //top right
	g_pSurface->Line(x - iw, y + ih, x, y, color); // top left
										   //connecting lines
	g_pSurface->Line(x - iw, y + h - ih, x - iw, y + ih, color); //left line
	g_pSurface->Line((x + w) + iw, y + h - ih, x + w + iw, y + ih, color); //right line
																   //bottom part
	g_pSurface->Line(x, y + h, x + w, y + h, color); //bottom line
	g_pSurface->Line((x + w) + iw, (y + h) - ih, x + w, y + h, color); //bottom right
	g_pSurface->Line(x - iw, (y + h) - ih, x, y + h, color); // bottom left
}
void DrawEsp(int X, int Y, int W, int H, Color Color3) {
	float lineW = (W / 5);
	float lineH = (H / 6);
	float lineT = 1;

	//outline

	//inline
	g_pSurface->DrawSetColor(Color3);
	g_pSurface->Line(X, Y, X, Y + lineH, Color3);//top left
	g_pSurface->Line(X, Y, X + lineW, Y, Color3);
	g_pSurface->Line(X + W - lineW, Y, X + W, Y, Color3); //top right
	g_pSurface->Line(X + W, Y, X + W, Y + lineH, Color3);
	g_pSurface->Line(X, Y + H - lineH, X, Y + H, Color3); //bot left
	g_pSurface->Line(X, Y + H, X + lineW, Y + H, Color3);
	g_pSurface->Line(X + W - lineW, Y + H, X + W, Y + H, Color3);//bot right
	g_pSurface->Line(X + W, Y + H - lineH, X + W, Y + H, Color3);

}
void c_visuals::BoxESP(C_BaseEntity* Entity)
{
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(esp::colors::enemy::box_col, flPlayerAlpha[Entity->EntIndex()]);
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int y = top.y;
		int width = height / 2;
		int x = pos.x - ((width / 2) / 2);
		int x2 = pos.x - (width / 2);
		int h = height;
		int w = width / 2;

		int iw = w / 3.5;
		int ih = (h / 3.5) - 1;
		bool IsTeammate = Entity->GetTeam() == g::pLocalEntity->GetTeam();
		bool IsEnemy = Entity->GetTeam() != g::pLocalEntity->GetTeam();
		bool IsLocal = Entity == g::pLocalEntity;

		//if (IsEnemy) 
		//{
		if (g_Options.esp.enemy.box_type== 0)
		{
			g_pSurface->OutlinedRect(x2, y, width, height, ESP);
			g_pSurface->OutlinedRect(x2 - 1, y - 1, width + 2, height + 2, ESP2);
			g_pSurface->OutlinedRect(x2 + 1, y + 1, width - 2, height - 2, ESP2);
		}
		if (g_Options.esp.enemy.box_type == 1)
		{
			DrawEsp(x2, y, width, height, ESP);
			DrawEsp(x2 - 1, y - 1, width + 2, height + 2, ESP2);
			DrawEsp(x2 + 1, y + 1, width - 2, height - 2, ESP2);		//}
		}




	}
}

void c_visuals::BoxESP_TEAM(C_BaseEntity* Entity)
{
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(esp::colors::team::box_col, flPlayerAlpha[Entity->EntIndex()]);
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int y = top.y;
		int width = height / 2;
		int x = pos.x - ((width / 2) / 2);
		int x2 = pos.x - (width / 2);
		int h = height;
		int w = width / 2;

		int iw = w / 3.5;
		int ih = (h / 3.5) - 1;
		bool IsTeammate = Entity->GetTeam() == g::pLocalEntity->GetTeam();
		bool IsEnemy = Entity->GetTeam() != g::pLocalEntity->GetTeam();
		bool IsLocal = Entity == g::pLocalEntity;

		//if (IsEnemy) 
		//{
		if (g_Options.esp.team.box_type == 0)
		{
			g_pSurface->OutlinedRect(x2, y, width, height, ESP);
			g_pSurface->OutlinedRect(x2 - 1, y - 1, width + 2, height + 2, ESP2);
			g_pSurface->OutlinedRect(x2 + 1, y + 1, width - 2, height - 2, ESP2);
		}
		if (g_Options.esp.team.box_type == 1)
		{
			DrawEsp(x2, y, width, height, ESP);
			DrawEsp(x2 - 1, y - 1, width + 2, height + 2, ESP2);
			DrawEsp(x2 + 1, y + 1, width - 2, height - 2, ESP2);		//}
		}




	}
}

void c_visuals::BoxESP_LOCAL(C_BaseEntity* Entity)
{
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(esp::colors::local::box_col, flPlayerAlpha[Entity->EntIndex()]);
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int y = top.y;
		int width = height / 2;
		int x = pos.x - ((width / 2) / 2);
		int x2 = pos.x - (width / 2);
		int h = height;
		int w = width / 2;

		int iw = w / 3.5;
		int ih = (h / 3.5) - 1;
		bool IsTeammate = Entity->GetTeam() == g::pLocalEntity->GetTeam();
		bool IsEnemy = Entity->GetTeam() != g::pLocalEntity->GetTeam();
		bool IsLocal = Entity == g::pLocalEntity;

		//if (IsEnemy) 
		//{
		if (g_Options.esp.local.box_type == 0)
		{
			g_pSurface->OutlinedRect(x2, y, width, height, ESP);
			g_pSurface->OutlinedRect(x2 - 1, y - 1, width + 2, height + 2, ESP2);
			g_pSurface->OutlinedRect(x2 + 1, y + 1, width - 2, height - 2, ESP2);
		}
		if (g_Options.esp.local.box_type == 1)
		{
			DrawEsp(x2, y, width, height, ESP);
			DrawEsp(x2 - 1, y - 1, width + 2, height + 2, ESP2);
			DrawEsp(x2 + 1, y + 1, width - 2, height - 2, ESP2);		//}
		}




	}
}


void c_visuals::NameESP(C_BaseEntity* Entity) {
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	PlayerInfo_s EntityInfo;
	g_pEngine->GetPlayerInfo(Entity->EntIndex(), &EntityInfo);
	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(esp::colors::enemy::name_col, flPlayerAlpha[Entity->EntIndex()]);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int y = top.y;
		int width = height / 2;
		int x = pos.x - (width / 2);

		g_pSurface->DrawT(x + (width / 2), y - 14, ESP, g::CourierNew, true, EntityInfo.szName);
	}
}


void c_visuals::NameESP_TEAM(C_BaseEntity* Entity)
{
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	PlayerInfo_s EntityInfo;
	g_pEngine->GetPlayerInfo(Entity->EntIndex(), &EntityInfo);
	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(esp::colors::team::name_col, flPlayerAlpha[Entity->EntIndex()]);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int y = top.y;
		int width = height / 2;
		int x = pos.x - (width / 2);

		g_pSurface->DrawT(x + (width / 2), y - 14, ESP, g::CourierNew, true, EntityInfo.szName);
	}
}

void c_visuals::NameESP_LOCAL(C_BaseEntity* Entity)
{
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	PlayerInfo_s EntityInfo;
	g_pEngine->GetPlayerInfo(Entity->EntIndex(), &EntityInfo);
	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(esp::colors::local::name_col, flPlayerAlpha[Entity->EntIndex()]);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int y = top.y;
		int width = height / 2;
		int x = pos.x - (width / 2);

		g_pSurface->DrawT(x + (width / 2), y - 14, ESP, g::CourierNew, true, EntityInfo.szName);
	}
}

#include <map>

bool DisplayAmmo[65];
bool DisplayAmmoTEAM[65];
bool DisplayAmmoLOCAL[65];

const char* GetGu45nIcon(C_BaseCombatWeapon* pWeapon)
{
	int cw = *pWeapon->m_iItemDefinitionIndex();

	switch (cw)
	{
	case (int)ItemDefinitionIndex::WEAPON_KNIFE_T:
	case 500:
	case 505:
	case 506:
	case 507:
	case 508:
	case 509:
	case 512:
	case 514:
	case 515:
	case 516:
	case 519:
	case 520:
	case 522:
	case 523:
	case (int)ItemDefinitionIndex::WEAPON_KNIFE:
		return "]";
	case (int)ItemDefinitionIndex::WEAPON_DEAGLE:
		return "A";
	case (int)ItemDefinitionIndex::WEAPON_ELITE:
		return "B";
	case (int)ItemDefinitionIndex::WEAPON_FIVESEVEN:
		return "C";
	case (int)ItemDefinitionIndex::WEAPON_GLOCK:
		return "D";
	case (int)ItemDefinitionIndex::WEAPON_P2000:
		return "E";
	case (int)ItemDefinitionIndex::WEAPON_P250:
		return "F";
	case (int)ItemDefinitionIndex::WEAPON_USPS:
		return "G";
	case (int)ItemDefinitionIndex::WEAPON_TEC9:
		return "H";
	case (int)ItemDefinitionIndex::WEAPON_CZ75:
		return "I";
	case (int)ItemDefinitionIndex::WEAPON_REVOLVER:
		return "J";
	case (int)ItemDefinitionIndex::WEAPON_MAC10:
		return "K";
	case (int)ItemDefinitionIndex::WEAPON_UMP45:
		return "L";
	case (int)ItemDefinitionIndex::WEAPON_BIZON:
		return "M";
	case (int)ItemDefinitionIndex::WEAPON_MP7:
		return "N";
	case (int)ItemDefinitionIndex::WEAPON_MP9:
		return "O";
	case (int)ItemDefinitionIndex::WEAPON_P90:
		return "P";
	case (int)ItemDefinitionIndex::WEAPON_GALIL:
		return "Q";
	case (int)ItemDefinitionIndex::WEAPON_FAMAS:
		return "R";
	case (int)ItemDefinitionIndex::WEAPON_M4A1S:
		return "S";
	case (int)ItemDefinitionIndex::WEAPON_M4A4:
		return "T";
	case (int)ItemDefinitionIndex::WEAPON_AUG:
		return "U";
	case (int)ItemDefinitionIndex::WEAPON_SG553:
		return "V";
	case (int)ItemDefinitionIndex::WEAPON_AK47:
		return "W";
	case (int)ItemDefinitionIndex::WEAPON_G3SG1:
		return "X";
	case (int)ItemDefinitionIndex::WEAPON_SCAR20:
		return "Y";
	case (int)ItemDefinitionIndex::WEAPON_AWP:
		return "Z";
	case (int)ItemDefinitionIndex::WEAPON_SSG08:
		return "a";
	case (int)ItemDefinitionIndex::WEAPON_XM1014:
		return "b";
	case (int)ItemDefinitionIndex::WEAPON_SAWEDOFF:
		return "c";
	case (int)ItemDefinitionIndex::WEAPON_MAG7:
		return "d";
	case (int)ItemDefinitionIndex::WEAPON_NOVA:
		return "e";
	case (int)ItemDefinitionIndex::WEAPON_NEGEV:
		return "f";
	case (int)ItemDefinitionIndex::WEAPON_M249:
		return "g";
	case (int)ItemDefinitionIndex::WEAPON_ZEUS:
		return "h";
	case (int)ItemDefinitionIndex::WEAPON_FLASHBANG:
		return "i";
	case (int)ItemDefinitionIndex::WEAPON_HEGRENADE:
		return "j";
	case (int)ItemDefinitionIndex::WEAPON_SMOKEGRENADE:
		return "k";
	case (int)ItemDefinitionIndex::WEAPON_MOLOTOV:
		return "l";
	case (int)ItemDefinitionIndex::WEAPON_DECOY:
		return "m";
	case (int)ItemDefinitionIndex::WEAPON_INC:
		return "n";
	case (int)ItemDefinitionIndex::WEAPON_C4:
		return "o";
	default:
		return "";
	}
}

void c_visuals::WeaponESP(C_BaseEntity* Entity) {
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	//spectators();
	auto Weapon = Entity->GetActiveWeapon();

	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(esp::colors::enemy::weapons, flPlayerAlpha[Entity->EntIndex()]);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);;
		int y = top.y;
		int width = height / 2;
		int x = pos.x - (width / 2);

		if (!Weapon)
			return;

		auto weapon_data = Weapon->GetCSWpnData();

		auto item_definition_index = Weapon->GetItemDefinitionIndex();

		if (!weapon_data)
			return;

		std::string WeaponName = weapon_data->hud_name;
		WeaponName.erase(0, 13);

		const auto xposition = x + (width / 2);
		const auto yposition = y + height + 1;

		bool IsTeammate = Entity->GetTeam() == g::pLocalEntity->GetTeam();
		bool IsEnemy = Entity->GetTeam() != g::pLocalEntity->GetTeam();
		bool IsLocal = Entity == g::pLocalEntity;
		int TypeOfEntity;
		if (IsLocal) {

		}
		else if (IsEnemy) {

		}
		else if (IsTeammate) {

		}
		auto add = g_Options.esp.enemy.ammo && DisplayAmmo[Entity->EntIndex()] ? 5 : 0;


		if (g_Options.esp.enemy.weapon_type == 0)
		{
			std::transform(WeaponName.begin(), WeaponName.end(), WeaponName.begin(), ::toupper);
			g_pSurface->DrawT(x + (width / 2), y + height + 1 + add, ESP, g::pixelfont, true, WeaponName.c_str());
		}

		if (g_Options.esp.enemy.weapon_type == 1)
		{
			std::string weapon_name = GetGu45nIcon(Weapon);




			g_pSurface->DrawT(x + (width / 2), y + height + 1 + add, ESP, g::Untitled1, true, weapon_name.c_str());


		}

	}
}

void c_visuals::WeaponESP_TEAM(C_BaseEntity* Entity) {
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	//spectators();
	auto Weapon = Entity->GetActiveWeapon();

	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(esp::colors::team::weapons, flPlayerAlpha[Entity->EntIndex()]);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);;
		int y = top.y;
		int width = height / 2;
		int x = pos.x - (width / 2);

		if (!Weapon)
			return;

		auto weapon_data = Weapon->GetCSWpnData();

		auto item_definition_index = Weapon->GetItemDefinitionIndex();

		if (!weapon_data)
			return;

		std::string WeaponName = weapon_data->hud_name;
		WeaponName.erase(0, 13);

		const auto xposition = x + (width / 2);
		const auto yposition = y + height + 1;

		bool IsTeammate = Entity->GetTeam() == g::pLocalEntity->GetTeam();
		bool IsEnemy = Entity->GetTeam() != g::pLocalEntity->GetTeam();
		bool IsLocal = Entity == g::pLocalEntity;
		int TypeOfEntity;
		if (IsLocal) {

		}
		else if (IsEnemy) {

		}
		else if (IsTeammate) {

		}
		auto add = g_Options.esp.team.ammo && DisplayAmmoTEAM[Entity->EntIndex()] ? 5 : 0;


		if (g_Options.esp.team.weapon_type== 0)
		{
			std::transform(WeaponName.begin(), WeaponName.end(), WeaponName.begin(), ::toupper);
			g_pSurface->DrawT(x + (width / 2), y + height + 1 + add, ESP, g::pixelfont, true, WeaponName.c_str());
		}

		if (g_Options.esp.team.weapon_type == 1)
		{
			std::string weapon_name = GetGu45nIcon(Weapon);




			g_pSurface->DrawT(x + (width / 2), y + height + 1 + add, ESP, g::Untitled1, true, weapon_name.c_str());


		}

	}
}

void c_visuals::WeaponESP_LOCAL(C_BaseEntity* Entity)
{
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	//spectators();
	auto Weapon = Entity->GetActiveWeapon();

	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(esp::colors::local::weapons, flPlayerAlpha[Entity->EntIndex()]);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);;
		int y = top.y;
		int width = height / 2;
		int x = pos.x - (width / 2);

		if (!Weapon)
			return;

		auto weapon_data = Weapon->GetCSWpnData();

		auto item_definition_index = Weapon->GetItemDefinitionIndex();

		if (!weapon_data)
			return;

		std::string WeaponName = weapon_data->hud_name;
		WeaponName.erase(0, 13);

		const auto xposition = x + (width / 2);
		const auto yposition = y + height + 1;

		bool IsTeammate = Entity->GetTeam() == g::pLocalEntity->GetTeam();
		bool IsEnemy = Entity->GetTeam() != g::pLocalEntity->GetTeam();
		bool IsLocal = Entity == g::pLocalEntity;
		int TypeOfEntity;
		if (IsLocal) {

		}
		else if (IsEnemy) {

		}
		else if (IsTeammate) {

		}
		auto add =g_Options.esp.local.ammo && DisplayAmmoLOCAL[Entity->EntIndex()] ? 5 : 0;


		if (g_Options.esp.local.weapon_type == 0)
		{
			std::transform(WeaponName.begin(), WeaponName.end(), WeaponName.begin(), ::toupper);
			g_pSurface->DrawT(x + (width / 2), y + height + 1 + add, ESP, g::pixelfont, true, WeaponName.c_str());
		}

		if (g_Options.esp.local.weapon_type == 1)
		{
			std::string weapon_name = GetGu45nIcon(Weapon);




			g_pSurface->DrawT(x + (width / 2), y + height + 1 + add, ESP, g::Untitled1, true, weapon_name.c_str());


		}

	}
}

void c_visuals::Ammo(C_BaseEntity* Entity) {
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	//spectators();

	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(esp::colors::enemy::ammo, flPlayerAlpha[Entity->EntIndex()]);
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]);

	auto Weapon = Entity->GetActiveWeapon();
	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top) && Weapon)
	{
		int height = (pos.y - top.y);;
		int y = top.y;
		int width2 = height / 2;
		int x = pos.x - (width2 / 2);

		float offset = (height / 4.f) + 5;
		UINT hp = height - (UINT)((height * 3) / 100);

		auto animLayer = Entity->GetAnimOverlay4(1);

		if (!animLayer)
			return;

		auto activity = Entity->GetSequenceActivity(animLayer->m_nSequence);

		if (!activity)
			return;

		int iClip = Weapon->GetAmmo();
		int iClipMax = Weapon->GetCSWpnData()->max_clip;

		if (!animLayer || !activity) {
			DisplayAmmo[Entity->EntIndex()] = false;
			return;
		}


		float box_w = (float)fabs(height / 2);
		float width;

		DisplayAmmo[Entity->EntIndex()] = iClip > 0 || (activity == 967 && animLayer->m_flWeight != 0.f);

		if (activity == 967 && animLayer->m_flWeight != 0.f)
		{
			float cycle = animLayer->m_flCycle;
			width = (((box_w * cycle) / 1.f));
		}
		else
			width = (((box_w * iClip) / iClipMax));

		if (!DisplayAmmo[Entity->EntIndex()])
			return;

		g_pSurface->FilledRect(x, y + height + 2, box_w, 4, ESP2);
		g_pSurface->FilledRect(x, y + height + 2, width, 4, Color(ESP));
		g_pSurface->OutlinedRect(x, y + height + 2, box_w, 4, ESP2);
	}
}

void c_visuals::Ammo_TEAM(C_BaseEntity* Entity)
{
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	//spectators();

	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(esp::colors::team::ammo, flPlayerAlpha[Entity->EntIndex()]);
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]);

	auto Weapon = Entity->GetActiveWeapon();
	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top) && Weapon)
	{
		int height = (pos.y - top.y);;
		int y = top.y;
		int width2 = height / 2;
		int x = pos.x - (width2 / 2);

		float offset = (height / 4.f) + 5;
		UINT hp = height - (UINT)((height * 3) / 100);

		auto animLayer = Entity->GetAnimOverlay4(1);

		if (!animLayer)
			return;

		auto activity = Entity->GetSequenceActivity(animLayer->m_nSequence);

		if (!activity)
			return;

		int iClip = Weapon->GetAmmo();
		int iClipMax = Weapon->GetCSWpnData()->max_clip;

		if (!animLayer || !activity) {
			DisplayAmmoTEAM[Entity->EntIndex()] = false;
			return;
		}


		float box_w = (float)fabs(height / 2);
		float width;

		DisplayAmmoTEAM[Entity->EntIndex()] = iClip > 0 || (activity == 967 && animLayer->m_flWeight != 0.f);

		if (activity == 967 && animLayer->m_flWeight != 0.f)
		{
			float cycle = animLayer->m_flCycle;
			width = (((box_w * cycle) / 1.f));
		}
		else
			width = (((box_w * iClip) / iClipMax));

		if (!DisplayAmmoTEAM[Entity->EntIndex()])
			return;

		g_pSurface->FilledRect(x, y + height + 2, box_w, 4, ESP2);
		g_pSurface->FilledRect(x, y + height + 2, width, 4, Color(ESP));
		g_pSurface->OutlinedRect(x, y + height + 2, box_w, 4, ESP2);
	}
}

void c_visuals::Ammo_LOCAL(C_BaseEntity* Entity)
{
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	//spectators();

	Color ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(esp::colors::local::ammo, flPlayerAlpha[Entity->EntIndex()]);
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]);

	auto Weapon = Entity->GetActiveWeapon();
	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top) && Weapon)
	{
		int height = (pos.y - top.y);;
		int y = top.y;
		int width2 = height / 2;
		int x = pos.x - (width2 / 2);

		float offset = (height / 4.f) + 5;
		UINT hp = height - (UINT)((height * 3) / 100);

		auto animLayer = Entity->GetAnimOverlay4(1);

		if (!animLayer)
			return;

		auto activity = Entity->GetSequenceActivity(animLayer->m_nSequence);

		if (!activity)
			return;

		int iClip = Weapon->GetAmmo();
		int iClipMax = Weapon->GetCSWpnData()->max_clip;

		if (!animLayer || !activity) {
			DisplayAmmoLOCAL[Entity->EntIndex()] = false;
			return;
		}


		float box_w = (float)fabs(height / 2);
		float width;

		DisplayAmmoLOCAL[Entity->EntIndex()] = iClip > 0 || (activity == 967 && animLayer->m_flWeight != 0.f);

		if (activity == 967 && animLayer->m_flWeight != 0.f)
		{
			float cycle = animLayer->m_flCycle;
			width = (((box_w * cycle) / 1.f));
		}
		else
			width = (((box_w * iClip) / iClipMax));

		if (!DisplayAmmoLOCAL[Entity->EntIndex()])
			return;

		g_pSurface->FilledRect(x, y + height + 2, box_w, 4, ESP2);
		g_pSurface->FilledRect(x, y + height + 2, width, 4, Color(ESP));
		g_pSurface->OutlinedRect(x, y + height + 2, box_w, 4, ESP2);
	}
}


void c_visuals::HealthESP(C_BaseEntity* Entity) {
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	Color ESP;
	if (Entity->GetHealth() > 70)
	{
		ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 255, 0, flPlayerAlpha[Entity->EntIndex()]);

	}
	else if (Entity->GetHealth() > 50)
	{
		ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(255, 214, 8, flPlayerAlpha[Entity->EntIndex()]);

	}
	else if (Entity->GetHealth() < 50)
	{
		ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(255, 0, 0, flPlayerAlpha[Entity->EntIndex()]);

	}
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]);


	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int width = height / 2;
		int x = pos.x - (width / 2);
		int y = top.y;
		auto health = Entity->GetHealth();

		/*
		g_pSurface->FilledRect(x - 6, y, 4, height, ESP2);
		//int pixelValue = health * height / 100;
		//int pixelValue = (height / 100.f) * health;

		//g_pSurface->FilledRect(x - 6, y, 4, pixelValue, ESP);

		int pixelValue = Entity->GetHealth() * height / 100;
		g_pSurface->FilledRect(x - 6, y + height - pixelValue, 4, pixelValue, Color(0, 255, 0, 250));
		g_pSurface->OutlinedRect(x - 6, y, 4, height, ESP2);

		*/
		

		if (g_Options.esp.enemy.health_type == 0)/*Whats a switch statement?*/
		{
			g_pSurface->FilledRect(x - 6, y, 4, height, ESP2);
			int pixelValue = Entity->GetHealth() * height / 100;
			g_pSurface->FilledRect(x - 6, y + height - pixelValue, 4, pixelValue, ESP);
			g_pSurface->OutlinedRect(x - 6, y, 4, height, ESP2);

		}
		else if (g_Options.esp.enemy.health_type == 1)
		{

			float flBoxes = std::ceil(Entity->GetHealth() / 10.f);
			float flX = x - 6; float flY = y - 1;
			float flHeight = height / 10.f;
			float flMultiplier = 12 / 360.f; flMultiplier *= flBoxes - 1;

			g_pSurface->FilledRect(flX, flY, 4, height + 2, ESP2);
			g_pSurface->OutlinedRect(flX, flY, 4, height + 2, ESP2);
			g_pSurface->FilledRect(flX + 1, flY, 2, flHeight * flBoxes + 1, ESP);

			for (int i = 0; i < 10; i++)
				g_pSurface->Line(flX, flY + i * flHeight, flX + 4, flY + i * flHeight, ESP2);
		}
		else if (g_Options.esp.enemy.health_type == 2)
		{
			int pixelValue = Entity->GetHealth() * height / 100;

			g_pSurface->FilledRect(x - 6, y, 4, height, ESP2);
			g_pSurface->FilledRect(x - 6, y + height - pixelValue, 4, pixelValue, ESP);
			g_pSurface->OutlinedRect(x - 6, y, 4, height, ESP2);

			int healthvalue = Entity->GetHealth();
			if (healthvalue < 75)
			{
				std::string Health = std::to_string(Entity->GetHealth());
				//int pixelValue = (height / 100.f) * health;
				//int pixelValue = Entity->GetHealth() * height / 100;

				g_pSurface->DrawT(x - 14, y + height - pixelValue, Color(255, 255, 255, 255), g::CourierNew, false, Health.c_str());




			}
		}
	
		
		
	}
}

void c_visuals::HealthESP_TEAM(C_BaseEntity* Entity)
{
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	Color ESP;
	if (Entity->GetHealth() > 70)
	{
		ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 255, 0, flPlayerAlpha[Entity->EntIndex()]);

	}
	else if (Entity->GetHealth() > 50)
	{
		ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(255, 214, 8, flPlayerAlpha[Entity->EntIndex()]);

	}
	else if (Entity->GetHealth() < 50)
	{
		ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(255, 0, 0, flPlayerAlpha[Entity->EntIndex()]);

	}
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]);


	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int width = height / 2;
		int x = pos.x - (width / 2);
		int y = top.y;
		auto health = Entity->GetHealth();

		/*
		g_pSurface->FilledRect(x - 6, y, 4, height, ESP2);
		//int pixelValue = health * height / 100;
		//int pixelValue = (height / 100.f) * health;

		//g_pSurface->FilledRect(x - 6, y, 4, pixelValue, ESP);

		int pixelValue = Entity->GetHealth() * height / 100;
		g_pSurface->FilledRect(x - 6, y + height - pixelValue, 4, pixelValue, Color(0, 255, 0, 250));
		g_pSurface->OutlinedRect(x - 6, y, 4, height, ESP2);

		*/


		if (g_Options.esp.team.health_type == 0)/*Whats a switch statement?*/
		{
			g_pSurface->FilledRect(x - 6, y, 4, height, ESP2);
			int pixelValue = Entity->GetHealth() * height / 100;
			g_pSurface->FilledRect(x - 6, y + height - pixelValue, 4, pixelValue, ESP);
			g_pSurface->OutlinedRect(x - 6, y, 4, height, ESP2);

		}
		else if (g_Options.esp.team.health_type == 1)
		{

			float flBoxes = std::ceil(Entity->GetHealth() / 10.f);
			float flX = x - 6; float flY = y - 1;
			float flHeight = height / 10.f;
			float flMultiplier = 12 / 360.f; flMultiplier *= flBoxes - 1;

			g_pSurface->FilledRect(flX, flY, 4, height + 2, ESP2);
			g_pSurface->OutlinedRect(flX, flY, 4, height + 2, ESP2);
			g_pSurface->FilledRect(flX + 1, flY, 2, flHeight * flBoxes + 1, ESP);

			for (int i = 0; i < 10; i++)
				g_pSurface->Line(flX, flY + i * flHeight, flX + 4, flY + i * flHeight, ESP2);
		}
		else if (g_Options.esp.team.health_type == 2)
		{
			int pixelValue = Entity->GetHealth() * height / 100;

			g_pSurface->FilledRect(x - 6, y, 4, height, ESP2);
			g_pSurface->FilledRect(x - 6, y + height - pixelValue, 4, pixelValue, ESP);
			g_pSurface->OutlinedRect(x - 6, y, 4, height, ESP2);

			int healthvalue = Entity->GetHealth();
			if (healthvalue < 75)
			{
				std::string Health = std::to_string(Entity->GetHealth());
				//int pixelValue = (height / 100.f) * health;
				//int pixelValue = Entity->GetHealth() * height / 100;

				g_pSurface->DrawT(x - 14, y + height - pixelValue, Color(255, 255, 255, 255), g::CourierNew, false, Health.c_str());




			}
		}



	}
}


void c_visuals::HealthESP_LOCAL(C_BaseEntity* Entity)
{
	Vector min, max;
	Entity->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = Entity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	Color ESP;
	if (Entity->GetHealth() > 70)
	{
		ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 255, 0, flPlayerAlpha[Entity->EntIndex()]);

	}
	else if (Entity->GetHealth() > 50)
	{
		ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(255, 214, 8, flPlayerAlpha[Entity->EntIndex()]);

	}
	else if (Entity->GetHealth() < 50)
	{
		ESP = Entity->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[Entity->EntIndex()]) : Color(255, 0, 0, flPlayerAlpha[Entity->EntIndex()]);

	}
	Color ESP2 = Entity->IsDormant() ? Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]) : Color(0, 0, 0, flPlayerAlpha[Entity->EntIndex()]);


	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int width = height / 2;
		int x = pos.x - (width / 2);
		int y = top.y;
		auto health = Entity->GetHealth();

		/*
		g_pSurface->FilledRect(x - 6, y, 4, height, ESP2);
		//int pixelValue = health * height / 100;
		//int pixelValue = (height / 100.f) * health;

		//g_pSurface->FilledRect(x - 6, y, 4, pixelValue, ESP);

		int pixelValue = Entity->GetHealth() * height / 100;
		g_pSurface->FilledRect(x - 6, y + height - pixelValue, 4, pixelValue, Color(0, 255, 0, 250));
		g_pSurface->OutlinedRect(x - 6, y, 4, height, ESP2);

		*/


		if (g_Options.esp.local.health_type == 0)/*Whats a switch statement?*/
		{
			g_pSurface->FilledRect(x - 6, y, 4, height, ESP2);
			int pixelValue = Entity->GetHealth() * height / 100;
			g_pSurface->FilledRect(x - 6, y + height - pixelValue, 4, pixelValue, ESP);
			g_pSurface->OutlinedRect(x - 6, y, 4, height, ESP2);

		}
		else if (g_Options.esp.local.health_type == 1)
		{

			float flBoxes = std::ceil(Entity->GetHealth() / 10.f);
			float flX = x - 6; float flY = y - 1;
			float flHeight = height / 10.f;
			float flMultiplier = 12 / 360.f; flMultiplier *= flBoxes - 1;

			g_pSurface->FilledRect(flX, flY, 4, height + 2, ESP2);
			g_pSurface->OutlinedRect(flX, flY, 4, height + 2, ESP2);
			g_pSurface->FilledRect(flX + 1, flY, 2, flHeight * flBoxes + 1, ESP);

			for (int i = 0; i < 10; i++)
				g_pSurface->Line(flX, flY + i * flHeight, flX + 4, flY + i * flHeight, ESP2);
		}
		else if (g_Options.esp.local.health_type == 2)
		{
			int pixelValue = Entity->GetHealth() * height / 100;

			g_pSurface->FilledRect(x - 6, y, 4, height, ESP2);
			g_pSurface->FilledRect(x - 6, y + height - pixelValue, 4, pixelValue, ESP);
			g_pSurface->OutlinedRect(x - 6, y, 4, height, ESP2);

			int healthvalue = Entity->GetHealth();
			if (healthvalue < 75)
			{
				std::string Health = std::to_string(Entity->GetHealth());
				//int pixelValue = (height / 100.f) * health;
				//int pixelValue = Entity->GetHealth() * height / 100;

				g_pSurface->DrawT(x - 14, y + height - pixelValue, Color(255, 255, 255, 255), g::CourierNew, false, Health.c_str());




			}
		}



	}
}

#include <windows.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <intrin.h>
#include <Options.hpp>


void c_visuals::flags(C_BaseEntity* player) {

	Vector min, max;
	player->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = player->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
	Color ESP = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(esp::colors::enemy::helmet, flPlayerAlpha[player->EntIndex()] / 1.3);

	Color Money = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(esp::colors::enemy::money, flPlayerAlpha[player->EntIndex()] / 1.3);

	Color Zoom = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(esp::colors::enemy::scoped, flPlayerAlpha[player->EntIndex()] / 1.3);

	Color FakeDuck = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(esp::colors::enemy::fakeduck, flPlayerAlpha[player->EntIndex()] / 1.3);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int width = height / 2;
		int x = pos.x - (width / 2);
		int y = top.y;

		auto fakeDuck = [&]() -> bool
		{
			static float storedTick;
			static float crouchedTicks[65];

			if (!player->AnimState())
				return false;

			float m_flDuckAmount = player->AnimState()->m_fDuckAmount;
			float m_flDuckSpeed = player->m_fDuckSpeed();
			float m_fFlags = player->GetFlags();

			if (m_flDuckSpeed != 0.f && m_flDuckAmount != 0.f)
			{
				if (m_flDuckSpeed == 8.f && m_flDuckAmount <= 0.9f && m_flDuckAmount > 0.01f)
				{
					if (storedTick != g_pGlobalVars->tickcount)
					{
						crouchedTicks[player->EntIndex()] = crouchedTicks[player->EntIndex()] + 1;
						storedTick = g_pGlobalVars->tickcount;
					}
					return (crouchedTicks[player->EntIndex()] >= 5);
				}
				else
					crouchedTicks[player->EntIndex()] = 0;
			}

			return false;
		};


		std::vector<std::pair<std::string, Color>> flags;

		if (g_Options.esp.enemy.money)
		{
			if (player->GetMoney())
				flags.push_back(std::pair<std::string, Color>(std::string("$").append(std::to_string(player->GetMoney())), Money));

		}
		if (g_Options.esp.enemy.helmet)
		{


			if (player->HasHelmet() && player->ArmorValue() > 0)
				flags.push_back(std::pair<std::string, Color>("HK", ESP));
			else if (!player->HasHelmet() && player->ArmorValue() > 0)
				flags.push_back(std::pair<std::string, Color>("K", ESP));
		}
		if (g_Options.esp.enemy.scoped)
		{
			if (player->IsScoped())
				flags.push_back(std::pair<std::string, Color>(std::string("ZOOM"), Zoom));

		}
		if (g_Options.esp.enemy.fakeduck)
		{
			if (fakeDuck())
				flags.push_back(std::pair<std::string, Color>(std::string("FAKEDUCK"), FakeDuck));

		}

		int pos = 0;
		for (auto text : flags) {
			g_pSurface->DrawT(x + width + 2, y + pos, text.second, g::WeaponESP, false, text.first.c_str());
			pos += 10;
		}

	}
}

void c_visuals::flags_TEAM(C_BaseEntity* player)
{

	Vector min, max;
	player->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = player->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
    Color ESP = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(esp::colors::team::helmet, flPlayerAlpha[player->EntIndex()] / 1.3);

    Color Money = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(esp::colors::team::money, flPlayerAlpha[player->EntIndex()] / 1.3);

    Color Zoom = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(esp::colors::team::scoped, flPlayerAlpha[player->EntIndex()] / 1.3);

    Color FakeDuck = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(esp::colors::team::fakeduck, flPlayerAlpha[player->EntIndex()] / 1.3);


	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int width = height / 2;
		int x = pos.x - (width / 2);
		int y = top.y;

		auto fakeDuck = [&]() -> bool
		{
			static float storedTick;
			static float crouchedTicks[65];

			if (!player->AnimState())
				return false;

			float m_flDuckAmount = player->AnimState()->m_fDuckAmount;
			float m_flDuckSpeed = player->m_fDuckSpeed();
			float m_fFlags = player->GetFlags();

			if (m_flDuckSpeed != 0.f && m_flDuckAmount != 0.f)
			{
				if (m_flDuckSpeed == 8.f && m_flDuckAmount <= 0.9f && m_flDuckAmount > 0.01f)
				{
					if (storedTick != g_pGlobalVars->tickcount)
					{
						crouchedTicks[player->EntIndex()] = crouchedTicks[player->EntIndex()] + 1;
						storedTick = g_pGlobalVars->tickcount;
					}
					return (crouchedTicks[player->EntIndex()] >= 5);
				}
				else
					crouchedTicks[player->EntIndex()] = 0;
			}

			return false;
		};


		std::vector<std::pair<std::string, Color>> flags;

		if (g_Options.esp.team.money)
		{
			if (player->GetMoney())
				flags.push_back(std::pair<std::string, Color>(std::string("$").append(std::to_string(player->GetMoney())), Money));

		}
		if (g_Options.esp.team.helmet)
		{


			if (player->HasHelmet() && player->ArmorValue() > 0)
				flags.push_back(std::pair<std::string, Color>("HK", ESP));
			else if (!player->HasHelmet() && player->ArmorValue() > 0)
				flags.push_back(std::pair<std::string, Color>("K", ESP));
		}
		if (g_Options.esp.team.scoped)
		{
			if (player->IsScoped())
				flags.push_back(std::pair<std::string, Color>(std::string("ZOOM"), Zoom));

		}
		if (g_Options.esp.team.fakeduck)
		{
			if (fakeDuck())
				flags.push_back(std::pair<std::string, Color>(std::string("FAKEDUCK"), FakeDuck));

		}

		int pos = 0;
		for (auto text : flags) {
			g_pSurface->DrawT(x + width + 2, y + pos, text.second, g::WeaponESP, false, text.first.c_str());
			pos += 10;
		}

	}
}

void c_visuals::flags_LOCAL(C_BaseEntity* player) {

	Vector min, max;
	player->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = player->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);
    Color ESP = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(esp::colors::local::helmet, flPlayerAlpha[player->EntIndex()] / 1.3);

    Color Money = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(esp::colors::local::money, flPlayerAlpha[player->EntIndex()] / 1.3);

    Color Zoom = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(esp::colors::local::scoped, flPlayerAlpha[player->EntIndex()] / 1.3);

    Color FakeDuck = player->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[player->EntIndex()] / 2) : Color(esp::colors::local::fakeduck, flPlayerAlpha[player->EntIndex()] / 1.3);



	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int width = height / 2;
		int x = pos.x - (width / 2);
		int y = top.y;



		std::vector<std::pair<std::string, Color>> flags;

		if (g_Options.esp.local.money)
		{
			if (player->GetMoney())
				flags.push_back(std::pair<std::string, Color>(std::string("$").append(std::to_string(player->GetMoney())), Money));

		}
		if (g_Options.esp.local.helmet)
		{


			if (player->HasHelmet() && player->ArmorValue() > 0)
				flags.push_back(std::pair<std::string, Color>("HK", ESP));
			else if (!player->HasHelmet() && player->ArmorValue() > 0)
				flags.push_back(std::pair<std::string, Color>("K", ESP));
		}
		if (g_Options.esp.local.scoped)
		{
			if (player->IsScoped())
				flags.push_back(std::pair<std::string, Color>(std::string("ZOOM"), Zoom));

		}
		if (g_Options.esp.local.fakeduck)
		{
			if (g_Options.antiaa.fake_duck)
			{

				if (GetAsyncKeyState(g_Options.antiaa.fake_duck_key))
				{
					flags.push_back(std::pair<std::string, Color>(std::string("FAKEDUCK"), FakeDuck));
				}
			}

		}

		int pos = 0;
		for (auto text : flags) {
			g_pSurface->DrawT(x + width + 2, y + pos, text.second, g::WeaponESP, false, text.first.c_str());
			pos += 10;
		}

	}
}

RECT GetViewport()
{
	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	g_pEngine->GetScreenSize(w, h);
	Viewport.right = w; Viewport.bottom = h;
	return Viewport;
}





void c_visuals::RenderSkeleton(C_BaseEntity* pEnt) // the best
{
    Color ESP = pEnt->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[pEnt->EntIndex()] / 2) : Color(esp::colors::enemy::skeleton, flPlayerAlpha[pEnt->EntIndex()] / 1.3);

	if (g_LagComp.PlayerRecord[pEnt->EntIndex()].size() == 0)
		return;
	int size = 0;

	auto skeleton_list = g_Options.esp.enemy.skeleton_type;
    if (skeleton_list == 0)
        return;
	if (skeleton_list==2)
		size++;
	if (skeleton_list==1)
		size++;

	for (int mode = 0; mode < size; mode++)
	{
		Vector Hitbox[19];
		Vector2D Hitboxw2s[19];

		int Record = 0;

		if (mode == 0 && skeleton_list == 0)
		{
			Record = g_LagComp.PlayerRecord[pEnt->EntIndex()].size() - 1;

		}

		for (int hitbox = 0; hitbox < 19; hitbox++)
		{
			Hitbox[hitbox] = pEnt->GetHitboxPosition(hitbox, g_LagComp.PlayerRecord[pEnt->EntIndex()].at(Record).Matrix);
			Utils::WorldToScreen(Hitbox[hitbox], Hitboxw2s[hitbox]);
		}

		//spine
		g_pSurface->Line(Hitboxw2s[HITBOX_HEAD].x, Hitboxw2s[HITBOX_HEAD].y, Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_UPPER_CHEST].x, Hitboxw2s[HITBOX_UPPER_CHEST].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_UPPER_CHEST].x, Hitboxw2s[HITBOX_UPPER_CHEST].y, Hitboxw2s[HITBOX_LOWER_CHEST].x, Hitboxw2s[HITBOX_LOWER_CHEST].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_LOWER_CHEST].x, Hitboxw2s[HITBOX_LOWER_CHEST].y, Hitboxw2s[HITBOX_THORAX].x, Hitboxw2s[HITBOX_THORAX].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_THORAX].x, Hitboxw2s[HITBOX_THORAX].y, Hitboxw2s[HITBOX_BELLY].x, Hitboxw2s[HITBOX_BELLY].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_BELLY].x, Hitboxw2s[HITBOX_BELLY].y, Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, ESP);

		//right leg
		g_pSurface->Line(Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, Hitboxw2s[HITBOX_RIGHT_THIGH].x, Hitboxw2s[HITBOX_RIGHT_THIGH].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_THIGH].x, Hitboxw2s[HITBOX_RIGHT_THIGH].y, Hitboxw2s[HITBOX_RIGHT_CALF].x, Hitboxw2s[HITBOX_RIGHT_CALF].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_CALF].x, Hitboxw2s[HITBOX_RIGHT_CALF].y, Hitboxw2s[HITBOX_RIGHT_FOOT].x, Hitboxw2s[HITBOX_RIGHT_FOOT].y, ESP);

		//right arm
		g_pSurface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].x, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].x, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].y, Hitboxw2s[HITBOX_RIGHT_FOREARM].x, Hitboxw2s[HITBOX_RIGHT_FOREARM].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_FOREARM].x, Hitboxw2s[HITBOX_RIGHT_FOREARM].y, Hitboxw2s[HITBOX_RIGHT_HAND].x, Hitboxw2s[HITBOX_RIGHT_HAND].y, ESP);

		//left leg
		g_pSurface->Line(Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, Hitboxw2s[HITBOX_LEFT_THIGH].x, Hitboxw2s[HITBOX_LEFT_THIGH].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_THIGH].x, Hitboxw2s[HITBOX_LEFT_THIGH].y, Hitboxw2s[HITBOX_LEFT_CALF].x, Hitboxw2s[HITBOX_LEFT_CALF].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_CALF].x, Hitboxw2s[HITBOX_LEFT_CALF].y, Hitboxw2s[HITBOX_LEFT_FOOT].x, Hitboxw2s[HITBOX_LEFT_FOOT].y, ESP);

		//left arm
		g_pSurface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].x, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_UPPER_ARM].x, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].y, Hitboxw2s[HITBOX_LEFT_FOREARM].x, Hitboxw2s[HITBOX_LEFT_FOREARM].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_FOREARM].x, Hitboxw2s[HITBOX_LEFT_FOREARM].y, Hitboxw2s[HITBOX_LEFT_HAND].x, Hitboxw2s[HITBOX_LEFT_HAND].y, ESP);


	}
}

void c_visuals::RenderSkeleton_TEAM(C_BaseEntity* pEnt)
{
    Color ESP = pEnt->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[pEnt->EntIndex()] / 2) : Color(esp::colors::team::skeleton, flPlayerAlpha[pEnt->EntIndex()] / 1.3);

    if (g_LagComp.PlayerRecord[pEnt->EntIndex()].size() == 0)
        return;
    int size = 0;

    auto skeleton_list = g_Options.esp.team.skeleton_type;
    if (skeleton_list == 0)
        return;
    if (skeleton_list == 0)
        size++;
    if (skeleton_list == 1)
        size++;

	for (int mode = 0; mode < size; mode++)
	{
		Vector Hitbox[19];
		Vector2D Hitboxw2s[19];

		int Record = 0;

		if (mode == 0 && skeleton_list == 0)
		{
			Record = g_LagComp.PlayerRecord[pEnt->EntIndex()].size() - 1;

		}

		for (int hitbox = 0; hitbox < 19; hitbox++)
		{
			Hitbox[hitbox] = pEnt->GetHitboxPosition(hitbox, g_LagComp.PlayerRecord[pEnt->EntIndex()].at(Record).Matrix);
			Utils::WorldToScreen(Hitbox[hitbox], Hitboxw2s[hitbox]);
		}

		//spine
		g_pSurface->Line(Hitboxw2s[HITBOX_HEAD].x, Hitboxw2s[HITBOX_HEAD].y, Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_UPPER_CHEST].x, Hitboxw2s[HITBOX_UPPER_CHEST].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_UPPER_CHEST].x, Hitboxw2s[HITBOX_UPPER_CHEST].y, Hitboxw2s[HITBOX_LOWER_CHEST].x, Hitboxw2s[HITBOX_LOWER_CHEST].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_LOWER_CHEST].x, Hitboxw2s[HITBOX_LOWER_CHEST].y, Hitboxw2s[HITBOX_THORAX].x, Hitboxw2s[HITBOX_THORAX].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_THORAX].x, Hitboxw2s[HITBOX_THORAX].y, Hitboxw2s[HITBOX_BELLY].x, Hitboxw2s[HITBOX_BELLY].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_BELLY].x, Hitboxw2s[HITBOX_BELLY].y, Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, ESP);

		//right leg
		g_pSurface->Line(Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, Hitboxw2s[HITBOX_RIGHT_THIGH].x, Hitboxw2s[HITBOX_RIGHT_THIGH].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_THIGH].x, Hitboxw2s[HITBOX_RIGHT_THIGH].y, Hitboxw2s[HITBOX_RIGHT_CALF].x, Hitboxw2s[HITBOX_RIGHT_CALF].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_CALF].x, Hitboxw2s[HITBOX_RIGHT_CALF].y, Hitboxw2s[HITBOX_RIGHT_FOOT].x, Hitboxw2s[HITBOX_RIGHT_FOOT].y, ESP);

		//right arm
		g_pSurface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].x, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].x, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].y, Hitboxw2s[HITBOX_RIGHT_FOREARM].x, Hitboxw2s[HITBOX_RIGHT_FOREARM].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_FOREARM].x, Hitboxw2s[HITBOX_RIGHT_FOREARM].y, Hitboxw2s[HITBOX_RIGHT_HAND].x, Hitboxw2s[HITBOX_RIGHT_HAND].y, ESP);

		//left leg
		g_pSurface->Line(Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, Hitboxw2s[HITBOX_LEFT_THIGH].x, Hitboxw2s[HITBOX_LEFT_THIGH].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_THIGH].x, Hitboxw2s[HITBOX_LEFT_THIGH].y, Hitboxw2s[HITBOX_LEFT_CALF].x, Hitboxw2s[HITBOX_LEFT_CALF].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_CALF].x, Hitboxw2s[HITBOX_LEFT_CALF].y, Hitboxw2s[HITBOX_LEFT_FOOT].x, Hitboxw2s[HITBOX_LEFT_FOOT].y, ESP);

		//left arm
		g_pSurface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].x, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_UPPER_ARM].x, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].y, Hitboxw2s[HITBOX_LEFT_FOREARM].x, Hitboxw2s[HITBOX_LEFT_FOREARM].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_FOREARM].x, Hitboxw2s[HITBOX_LEFT_FOREARM].y, Hitboxw2s[HITBOX_LEFT_HAND].x, Hitboxw2s[HITBOX_LEFT_HAND].y, ESP);


	}
}

void c_visuals::RenderSkeleton_LOCAL(C_BaseEntity* pEnt)
{
    Color ESP = pEnt->IsDormant() ? Color(150, 150, 150, flPlayerAlpha[pEnt->EntIndex()] / 2) : Color(esp::colors::local::skeleton, flPlayerAlpha[pEnt->EntIndex()] / 1.3);

    if (g_LagComp.PlayerRecord[pEnt->EntIndex()].size() == 0)
        return;
    int size = 0;

    auto skeleton_list = g_Options.esp.local.skeleton_type;
    if (skeleton_list == 0)
        return;
    if (skeleton_list == 0)
        size++;
    if (skeleton_list == 1)
        size++;
	for (int mode = 0; mode < size; mode++)
	{
		Vector Hitbox[19];
		Vector2D Hitboxw2s[19];

		int Record = 0;

		if (mode == 0 && skeleton_list == 0)
		{
			Record = g_LagComp.PlayerRecord[pEnt->EntIndex()].size() - 1;

		}

		for (int hitbox = 0; hitbox < 19; hitbox++)
		{
			Hitbox[hitbox] = pEnt->GetHitboxPosition(hitbox, g_LagComp.PlayerRecord[pEnt->EntIndex()].at(Record).Matrix);
			Utils::WorldToScreen(Hitbox[hitbox], Hitboxw2s[hitbox]);
		}

		//spine
		g_pSurface->Line(Hitboxw2s[HITBOX_HEAD].x, Hitboxw2s[HITBOX_HEAD].y, Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_UPPER_CHEST].x, Hitboxw2s[HITBOX_UPPER_CHEST].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_UPPER_CHEST].x, Hitboxw2s[HITBOX_UPPER_CHEST].y, Hitboxw2s[HITBOX_LOWER_CHEST].x, Hitboxw2s[HITBOX_LOWER_CHEST].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_LOWER_CHEST].x, Hitboxw2s[HITBOX_LOWER_CHEST].y, Hitboxw2s[HITBOX_THORAX].x, Hitboxw2s[HITBOX_THORAX].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_THORAX].x, Hitboxw2s[HITBOX_THORAX].y, Hitboxw2s[HITBOX_BELLY].x, Hitboxw2s[HITBOX_BELLY].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_BELLY].x, Hitboxw2s[HITBOX_BELLY].y, Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, ESP);

		//right leg
		g_pSurface->Line(Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, Hitboxw2s[HITBOX_RIGHT_THIGH].x, Hitboxw2s[HITBOX_RIGHT_THIGH].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_THIGH].x, Hitboxw2s[HITBOX_RIGHT_THIGH].y, Hitboxw2s[HITBOX_RIGHT_CALF].x, Hitboxw2s[HITBOX_RIGHT_CALF].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_CALF].x, Hitboxw2s[HITBOX_RIGHT_CALF].y, Hitboxw2s[HITBOX_RIGHT_FOOT].x, Hitboxw2s[HITBOX_RIGHT_FOOT].y, ESP);

		//right arm
		g_pSurface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].x, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].x, Hitboxw2s[HITBOX_RIGHT_UPPER_ARM].y, Hitboxw2s[HITBOX_RIGHT_FOREARM].x, Hitboxw2s[HITBOX_RIGHT_FOREARM].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_RIGHT_FOREARM].x, Hitboxw2s[HITBOX_RIGHT_FOREARM].y, Hitboxw2s[HITBOX_RIGHT_HAND].x, Hitboxw2s[HITBOX_RIGHT_HAND].y, ESP);

		//left leg
		g_pSurface->Line(Hitboxw2s[HITBOX_PELVIS].x, Hitboxw2s[HITBOX_PELVIS].y, Hitboxw2s[HITBOX_LEFT_THIGH].x, Hitboxw2s[HITBOX_LEFT_THIGH].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_THIGH].x, Hitboxw2s[HITBOX_LEFT_THIGH].y, Hitboxw2s[HITBOX_LEFT_CALF].x, Hitboxw2s[HITBOX_LEFT_CALF].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_CALF].x, Hitboxw2s[HITBOX_LEFT_CALF].y, Hitboxw2s[HITBOX_LEFT_FOOT].x, Hitboxw2s[HITBOX_LEFT_FOOT].y, ESP);

		//left arm
		g_pSurface->Line(Hitboxw2s[HITBOX_NECK].x, Hitboxw2s[HITBOX_NECK].y, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].x, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_UPPER_ARM].x, Hitboxw2s[HITBOX_LEFT_UPPER_ARM].y, Hitboxw2s[HITBOX_LEFT_FOREARM].x, Hitboxw2s[HITBOX_LEFT_FOREARM].y, ESP);
		g_pSurface->Line(Hitboxw2s[HITBOX_LEFT_FOREARM].x, Hitboxw2s[HITBOX_LEFT_FOREARM].y, Hitboxw2s[HITBOX_LEFT_HAND].x, Hitboxw2s[HITBOX_LEFT_HAND].y, ESP);


	}
}

bool WorldToScreenAlpha(const Vector& origin, Vector2D& screen)
{
	const auto screenTransform = [&origin, &screen]() -> bool
	{
		static std::uintptr_t pViewMatrix;
		if (!pViewMatrix)
		{
			pViewMatrix = static_cast<std::uintptr_t>(Utils::FindSignature("client_panorama.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
			pViewMatrix += 3;
			pViewMatrix = *reinterpret_cast<std::uintptr_t*>(pViewMatrix);
			pViewMatrix += 176;
		}

		const VMatrix& w2sMatrix = *reinterpret_cast<VMatrix*>(pViewMatrix);
		screen.x = w2sMatrix.m[0][0] * origin.x + w2sMatrix.m[0][1] * origin.y + w2sMatrix.m[0][2] * origin.z + w2sMatrix.m[0][3];
		screen.y = w2sMatrix.m[1][0] * origin.x + w2sMatrix.m[1][1] * origin.y + w2sMatrix.m[1][2] * origin.z + w2sMatrix.m[1][3];

		float w = w2sMatrix.m[3][0] * origin.x + w2sMatrix.m[3][1] * origin.y + w2sMatrix.m[3][2] * origin.z + w2sMatrix.m[3][3];

		if (w < 0.001f)
		{
			screen.x *= 100000;
			screen.y *= 100000;
			return true;
		}

		float invw = 1.f / w;
		screen.x *= invw;
		screen.y *= invw;

		return false;
	};

	if (!screenTransform())
	{
		int iScreenWidth, iScreenHeight;
		g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);

		screen.x = (iScreenWidth * 0.5f) + (screen.x * iScreenWidth) * 0.5f;
		screen.y = (iScreenHeight * 0.5f) - (screen.y * iScreenHeight) * 0.5f;

		return true;
	}
	return false;
}

bool TransformSc423reen(const Vector& in, Vector& out)
{
	static ptrdiff_t ptrViewMatrix;
	if (!ptrViewMatrix)
	{//                                                          
		ptrViewMatrix = static_cast<ptrdiff_t>(Utils::FindSignature("client_panorama.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9"));
		ptrViewMatrix += 0x3;
		ptrViewMatrix = *reinterpret_cast<uintptr_t*>(ptrViewMatrix);
		ptrViewMatrix += 176;
	}
	const VMatrix& worldToScreen = g_pEngine->WorldToScreenMatrix(); // matrix



	int ScrW, ScrH;

	g_pEngine->GetScreenSize(ScrW, ScrH);

	float w = worldToScreen[3][0] * in[0] + worldToScreen[3][1] * in[1] + worldToScreen[3][2] * in[2] + worldToScreen[3][3];
	out.z = 0; // 0 poniewaz z nie jest nam potrzebne | uzywamy tylko wysokosci i szerokosci (x,y)
	if (w > 0.01)
	{
		float inverseWidth = 1 / w; // inverse na 1 pozycje ekranu
		out.x = (ScrW / 2) + (0.5 * ((worldToScreen[0][0] * in[0] + worldToScreen[0][1] * in[1] + worldToScreen[0][2] * in[2] + worldToScreen[0][3]) * inverseWidth) * ScrW + 0.5);
		out.y = (ScrH / 2) - (0.5 * ((worldToScreen[1][0] * in[0] + worldToScreen[1][1] * in[1] + worldToScreen[1][2] * in[2] + worldToScreen[1][3]) * inverseWidth) * ScrH + 0.5);
		return true;
	}
	return false;
}



void DrawWeapon(C_BaseEntity* player) {
	Vector min, max;
	player->GetRenderBounds(min, max);
	Vector pos, pos3D, top, top3D;
	pos3D = player->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 11);

	if (WorldToScreen(pos3D, pos) && WorldToScreen(top3D, top))
	{
		int height = (pos.y - top.y);
		int y = top.y;
		int width = height / 2;
		int x = pos.x - ((width / 2) / 2);
		int x2 = pos.x - (width / 2);
		int h = height;
		int w = width / 2;

		g_pSurface->OutlinedRect(x2, y, width, height, Color(255, 255, 255));
		g_pSurface->OutlinedRect(x2 - 1, y - 1, width + 2, height + 2, Color(0, 0, 0));
		g_pSurface->OutlinedRect(x2 + 1, y + 1, width - 2, height - 2, Color(0, 0, 0));
	}
}
static 	bool IsOnScreen(Vector origin, Vector& screen)
{
	if (!WorldToScreen(origin, screen)) return false;
	int iScreenWidth, iScreenHeight;
	g_pEngine->GetScreenSize(iScreenWidth, iScreenHeight);
	bool xOk = iScreenWidth > screen.x > 0, yOk = iScreenHeight > screen.y > 0;
	return xOk && yOk;
}
Vector get_hitbox_23location(C_BaseEntity* obj, int hitbox_id)
{
	matrix3x4_t bone_matrix[128];
	if (obj->IsAlive()) {
		if (obj->SetupBones(bone_matrix, 128, 0x00000100, 0.0f))
		{
			if (obj->GetModel())
			{
				auto studio_model = g_pModelInfo->GetStudiomodel(obj->GetModel());
				if (studio_model)
				{
					auto hitbox = studio_model->GetHitboxSet(0)->GetHitbox(hitbox_id);
					if (hitbox)
					{
						auto min = Vector{}, max = Vector{};

						g_Math.VectorTransform(hitbox->min, bone_matrix[hitbox->bone], min);
						g_Math.VectorTransform(hitbox->max, bone_matrix[hitbox->bone], max);

						return (min + max) / 2.0f;
					}
				}
			}
		}
		return Vector{};
	}
}

void VectorAnglesXX(const Vector& forward, Vector& angles)
{
	float tmp, yaw, pitch;
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
		yaw = (atan2(forward[1], forward[0]) * 180 / PI);
		if (yaw < 0)
			yaw += 360;
		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / PI);
		if (pitch < 0)
			pitch += 360;
	}
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}


Vector CalcAngleXX(Vector src, Vector dst)
{
	Vector ret;
	VectorAnglesXX(dst - src, ret);
	return ret;
}
template<class T, class U>
inline T cl23amp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

void TexturedPolygon(int n, std::vector<Vertex_t> vertice, Color color)
{
	static int texture_id = g_pSurface->CreateNewTextureID(true); // 
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	g_pSurface->DrawSetTextureRGBA(texture_id, buf, 1, 1); //
	g_pSurface->DrawSetColor(color); //
	g_pSurface->DrawSetTexture(texture_id); //
	g_pSurface->DrawTexturedPolygon(n, vertice.data()); //
}


void fov_arrows(C_BaseEntity* entity, Color color)
{


	if (!g_Options.esp.effects.outofviewarrows)
		return;


	////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		/////////////////////////////////////
		////////////////////////////////////////////////////

	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player) return;
	if (!local_player->IsAlive())
		return;
	if (entity->IsDormant()) return;
	if (entity->GetTeam() == local_player->GetTeam())
		return;

	Vector position;
	if (IsOnScreen(get_hitbox_23location(entity, 2), position))
		return;


	Vector client_viewangles;
	int screen_width = 0, screen_height = 0;
	float radius = 300.f;
	g_pEngine->GetViewAngles(client_viewangles);
	g_pEngine->GetScreenSize(screen_width, screen_height);
	const auto screen_center = Vector(screen_width / 2.f, screen_height / 2.f, 0);
	const auto rot = DEG2RAD(client_viewangles.y - CalcAngleXX(local_player->get_eye_pos(), get_hitbox_23location(entity, 1)).y - 90);
	static auto alpha = 0.f;
	static auto plus_or_minus = false;
	if (alpha <= 0.f || alpha >= 255.f)
		plus_or_minus = !plus_or_minus;
	alpha += plus_or_minus ? (255.f / 0.5f * g_pGlobalVars->frametime) : -(255.f / 0.5f * g_pGlobalVars->frametime);
	alpha = cl23amp<float>(alpha, 0.f, 255.f);
	std::vector<Vertex_t> vertices;
	vertices.push_back(Vertex_t(Vector2D(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius)));
	vertices.push_back(Vertex_t(Vector2D(screen_center.x + cosf(rot + DEG2RAD(2)) * (radius - 16), screen_center.y + sinf(rot + DEG2RAD(2)) * (radius - 16))));
	vertices.push_back(Vertex_t(Vector2D(screen_center.x + cosf(rot - DEG2RAD(2)) * (radius - 16), screen_center.y + sinf(rot - DEG2RAD(2)) * (radius - 16))));
	TexturedPolygon(3, vertices, Color(esp::colors::out_of_view_arrows, alpha));
}


void c_visuals::RenderHitboxPoints(C_BaseEntity* pEnt)
{


}
/*
bool boundingBox(C_BaseEntity* pEntity, Vector offset = Vector(0, 0, 0))
{
	Vector min, max;
	pEntity->GetRenderBounds(min, max);

	Vector pos3D, top3D;
	Vector2D pos, top;
	pos3D = pEntity->GetAbsOrigin() - Vector(0, 0, 10);
	top3D = pos3D + Vector(0, 0, max.z + 15);

	if (WorldToScreenAlpha(pos3D, pos) && WorldToScreenAlpha(top3D, top))
	{
		int height = (pos.y - top.y);
		Box.h = height; //h
		int width = Box.h / 2;
		Box.w = width; //w

		Box.x = pos.x - width / 2; //x
		Box.y = top.y; //y

		return true;
	}

	return false;
}*/


void c_visuals::DrawPlayers()
{

	for (int Index = 1; Index < g_pGlobalVars->maxClients; Index++)
	{
		auto Entity = g_pEntityList->GetClientEntity(Index);
		auto LocalPlayer = g::pLocalEntity;

		//	if (Entity && !Entity->IsDormant())
			//	worldWea234pon(Entity);


		if (!Entity || !LocalPlayer || !Entity->IsAlive())
			continue;



		//ClientClass* cClass = (ClientClass*)Entity->GetClientClass();
	//	if (cClass->ClassID != (int)ClassId::CBaseWeaponWorldModel && ((strstr(cClass->pNetworkName, "Weapon") || cClass->ClassID == (int)ClassId::CDEagle || cClass->ClassID == (int)ClassId::CAK47))) {
		//	DrawWeapon(Entity);
	//	}

		bool IsTeammate = Entity->GetTeam() == LocalPlayer->GetTeam();
		bool IsEnemy = Entity->GetTeam() != LocalPlayer->GetTeam();
		bool IsLocal = Entity == LocalPlayer;

		if (!Entity->IsDormant()) {
			StoredCurtimePlayer[Entity->EntIndex()] = g_pGlobalVars->curtime;
			
		}

		if (Entity->IsDormant() && flPlayerAlpha[Entity->EntIndex()] > 0 && g_pGlobalVars->curtime - StoredCurtimePlayer[Entity->EntIndex()] > 2)
		{
			flPlayerAlpha[Entity->EntIndex()] -= 5;
		}
		else if (flPlayerAlpha[Entity->EntIndex()] < 255 && !(Entity->IsDormant()))
		{
			flPlayerAlpha[Entity->EntIndex()] += 5;
		}

		if (IsEnemy)
		{
			auto skeleton_list = g_Options.esp.enemy.skeleton_type;

			if (g_Options.esp.enemy.name)
				NameESP(Entity);
			if (g_Options.esp.enemy.box)
				BoxESP(Entity);
			if (g_Options.esp.enemy.weapon)
				WeaponESP(Entity);
			if (g_Options.esp.enemy.ammo)
				Ammo(Entity);
			if (g_Options.esp.enemy.health)
				HealthESP(Entity);
			flags(Entity);
			if (skeleton_list == 0 || skeleton_list==1)
				RenderSkeleton(Entity);

			if (g_Options.esp.effects.outofviewarrows)
				fov_arrows(Entity, Color(esp::colors::out_of_view_arrows));

			if (g_Options.rage.show_aimpoint)
				RenderHitboxPoints(Entity);


		}
		if (IsTeammate && !IsLocal)
		{
            auto skeleton_list = g_Options.esp.team.skeleton_type;

            if (g_Options.esp.team.name)
				NameESP_TEAM(Entity);
            if (g_Options.esp.team.box)
				BoxESP_TEAM(Entity);
            if (g_Options.esp.team.weapon)
				WeaponESP_TEAM(Entity);
            if (g_Options.esp.team.ammo)
				Ammo_TEAM(Entity);
            if (g_Options.esp.team.health)
				HealthESP_TEAM(Entity);
			flags_TEAM(Entity);
            if (skeleton_list == 0 || skeleton_list == 1)
				RenderSkeleton_TEAM(Entity);
		}
		if (IsLocal)
		{
            auto skeleton_list = g_Options.esp.local.skeleton_type;

            if (g_Options.esp.local.name)
				NameESP_LOCAL(Entity);
            if (g_Options.esp.local.box)
				BoxESP_LOCAL(Entity);
            if (g_Options.esp.local.weapon)
				WeaponESP_LOCAL(Entity);
            if (g_Options.esp.local.ammo)
				Ammo_LOCAL(Entity);
            if (g_Options.esp.local.health)
				HealthESP_LOCAL(Entity);
			flags_LOCAL(Entity);
            if (skeleton_list == 0 || skeleton_list == 1)
				RenderSkeleton_LOCAL(Entity);
		}
	}

	for (int i = 0; i < g_pEntityList->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity* pEntity = g_pEntityList->GetClientEntity(i);
		PlayerInfo_s pinfo;

		if (pEntity && !pEntity->IsDormant())
		{
			if (g_Options.misc.misc_radar && pEntity->IsAlive())
			{
				static auto m_bSpotted = NetvarManager::Instance()->GetOffset("DT_BaseEntity", "m_bSpotted");

				*(char*)((DWORD)(pEntity)+m_bSpotted) = 1;
			}
			ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();
			if (cClass->ClassID != (int)ClassId::CBaseWeaponWorldModel && ((strstr(cClass->pNetworkName, "Weapon") || cClass->ClassID == (int)ClassId::CDEagle || cClass->ClassID == (int)ClassId::CAK47)))
				_drops.drawdrop(pEntity, cClass, g::CourierNew);
			if (cClass->ClassID == ClassId::CChicken)
				if (g_Options.esp.effects.chicken_Box)
					_drops.drawchicken(pEntity, cClass);
			if (strstr(cClass->pNetworkName, "Projectile") && (g_Options.esp.effects.grenade_box || g_Options.esp.effects.grenade_box_type == 1 || g_Options.esp.effects.grenade_box_type == 2))
				_drops.drawgrenade(pEntity);
		}
	}
}

void chocked_indicatorsRECODE()
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	int w = 0, h = 0;
	g_pEngine->GetScreenSize(w, h);

	static auto draw_bar = [](int x, int y, float val, float max, const wchar_t* name) {
		if (val > max)
			val = max;

		g_pSurface->draw_wide_string(true, g::INDICATOR, x + 125 / 2, y - 8, Color(255, 255, 255, 255), const_cast<wchar_t*>(name));
		g_pSurface->draw_filled_rect(x, y, 125, 15, Color(50, 50, 50, 75));
		g_pSurface->draw_filled_rect(x, y, 125 * (val / max), 15, Color(0, 255, 0, 75));
	};


	auto nci = g_pEngine->GetNetChannel();

	if (!nci)
		return;
	if (local_player->IsAlive())
	{
		if (g_Options.esp.other.choke_indicator)
			draw_bar(10, h / 2 - 100, nci->m_nChokedPackets, 14.0f, L"choke");

		if (g_Options.esp.other.fake_indicator)
			draw_bar(10, h / 2 - 60, g::fake_strength, 60.f, L"desync");


	}
	else if (local_player && g_pEngine->IsInGame() && g_pEngine->IsConnected())
	{
		if (g_Options.esp.other.choke_indicator)
			draw_bar(15, h / 2 - 150, 0.0f, 1.0f, L"choke");

		if (g_Options.esp.other.fake_indicator)
			draw_bar(15, h / 2 - 110, 0.0f, 2.0f, L"desync");

	}
}
float aClampYaw(float yaw)
{
	while (yaw > 180.f)
		yaw -= 360.f;
	while (yaw < -180.f)
		yaw += 360.f;
	return yaw;
}
float GetMaxDDelta(C_AnimState* animstate)
{

	float flRunningSpeed = std::clamp(animstate->m_flFeetSpeedForwardsOrSideWays, 0.f, 1.f);
	float flYawModifier = ((animstate->m_flStopToFullRunningFraction * -0.3f) - 0.2f) * flRunningSpeed;
	float flYawModifier2 = flYawModifier + 1.f;


	if (animstate->m_fDuckAmount > 0.f)
	{
		float maxVelocity = std::clamp(animstate->m_flFeetSpeedForwardsOrSideWays, 0.f, 1.f);
		float duckSpeed = animstate->m_fDuckAmount * maxVelocity;
		flYawModifier2 += (duckSpeed * (0.5f - flYawModifier2));
	}

	return *(float*)((uintptr_t)animstate + 0x334) * flYawModifier2;
}

void Arrows()
{
	auto local = g::pLocalEntity;

	if (!local)
		return;

	if (!local->IsAlive())
		return;

	if (g::inthird_person)
	{
		if (g::pLocalEntity->IsAlive() && g_pEngine->IsInGame() && g_pEngine->IsConnected()) {
			const auto origin = g::pLocalEntity->GetOrigin();

			auto local_player = g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
			int desync = GetMaxDDelta(local_player->AnimState());

			const auto yaw = g::pCmd->viewangles.y;

			const auto realangle = g::RealAngle.y;

			const auto fakeangle = g::FakeAngle.y;

			static HWND csgo = FindWindowA("Valve001", NULL);
			RECT screen;
			GetWindowRect(csgo, &screen);
			int screenx = screen.right - screen.left;
			int screeny = screen.bottom - screen.top;

			float feet = aClampYaw(g::pLocalEntity->AnimState()->m_flGoalFeetYaw);

			float delta = aClampYaw(abs(feet - fakeangle)); // real way of doing it btw (no lie)

		//	if (g_Aimbot.baiming)
		//		g_pSurface->DrawT(10, screeny - 70, Color(255, 0, 0, 255), indicatorfont, false, xorstr("BAIM").crypt_get());



			/*clock_t thisClock1 = clock();
			if ((((thisClock1 - lastClock1) / (double)CLOCKS_PER_SEC) * 1000.0) > 1000)
			{
				lastClock1 = thisClock1;
				fps1 = cFps1;
				cFps1 = 0;
			}
			else
				cFps1++;*/

			auto localplayer = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
			auto eyeang = localplayer->GetEyeAngles().y;
			auto lby = localplayer->GetLowerBodyYaw();
			Vector viewangle; g_pEngine->GetViewAngles(viewangle);
			Vector2D screen1, screen2;
			Utils::WorldToScreen(origin, screen1);




			constexpr auto distance = 45.f;
			constexpr auto lbydistance = 35.f;
			constexpr auto dist2 = 55.f;

			Utils::WorldToScreen(g_Math.get_rotated_position(origin, realangle, distance), screen2);
			g_pSurface->Line(screen2.x, screen2.y, screen1.x, screen1.y, Color(128, 244, 66, 255));
			g_pSurface->DrawT(screen2.x, screen2.y, Color(128, 244, 66, 255), g::Tahoma, false, "REAL");

			Utils::WorldToScreen(g_Math.get_rotated_position(origin, lby, lbydistance), screen2);
			g_pSurface->Line(screen2.x, screen2.y, screen1.x, screen1.y, Color(66, 124, 244, 255));
			g_pSurface->DrawT(screen2.x, screen2.y, Color(66, 124, 244, 255), g::Tahoma, false, "LBY");

			Utils::WorldToScreen(g_Math.get_rotated_position(origin, g::FakeAngle.y, distance), screen2);
			g_pSurface->Line(screen2.x, screen2.y, screen1.x, screen1.y, Color(244, 100, 66, 255));
			g_pSurface->DrawT(screen2.x, screen2.y, Color(244, 100, 66, 255), g::Tahoma, false, "FAKE");

			Utils::WorldToScreen(g_Math.get_rotated_position(origin, g::pLocalEntity->AnimState()->m_flGoalFeetYaw, dist2), screen2);
			g_pSurface->Line(screen2.x, screen2.y, screen1.x, screen1.y, Color(75, 192, 235, 255));
			g_pSurface->DrawT(screen2.x, screen2.y, Color(75, 192, 235, 255), g::Tahoma, false, "GOAL");




		}

	}
}


void c_visuals::DrawIndicator()
{


	/*auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player) return;
	chocked_indicatorsRECODE();
	int screen_width, screen_height;
	g_pEngine->GetScreenSize(screen_width, screen_height);

	int iY = 66;
	std::vector<dropdownboxitem> indicators = Options::Menu.VisualsTab.indicator.items;



	if (Options::Menu.RageBotTab.overrideenable.GetState() && indicators[0].GetSelected)
	{
		iY += 22;
		bool overridekeyenabled;
		if (Options::Menu.RageBotTab.overridemethod.GetIndex() == 0)
			g_pSurface->DrawF(10, screen_height - iY, g::visuals_lby_font, false, false, overridething ? Color(0, 255, 0) : Color(255, 0, 0), "OVERRIDE");
		else if (Options::Menu.RageBotTab.overridemethod.GetIndex() == 1)
		{
			GetAsyncKeyState(Options::Menu.RageBotTab.overridekey.GetKey()) ?
				g_pSurface->DrawF(10, screen_height - iY, g::visuals_lby_font, false, false, Color(0, 255, 0), "OVERRIDE") :
				g_pSurface->DrawF(10, screen_height - iY, g::visuals_lby_font, false, false, Color(255, 0, 0), "OVERRIDE");
		}
	}


	if (indicators[1].GetSelected)
	{
		if (Options::Menu.AntiAimTab.fake_crouch.GetState())
		{



			float lspeed;

			lspeed = local_player->GetVelocity().Length2D();


			iY += 22;

			if (GetAsyncKeyState(Options::Menu.AntiAimTab.fake_crouchkey.GetKey()))
				g_pSurface->DrawF(10, screen_height - iY, g::visuals_lby_font, false, false, Color(0, 255, 0), "FD");
			else
				g_pSurface->DrawF(10, screen_height - iY, g::visuals_lby_font, false, false, Color(255, 0, 0), "FD");

		}

	}
	if (indicators[2].GetSelected)
	{

		iY += 22;

		float desyncvalue;

		desyncvalue = g::fake_strength;


		//char jew[64];
	//	sprintf_s(jew, "Desync: %.1f", desyncvalue);


		if (desyncvalue >= 45.f)
			g_pSurface->DrawF(10, screen_height - iY, g::visuals_lby_font, false, false, Color(0, 255, 0), "FAKE");
		else
			g_pSurface->DrawF(10, screen_height - iY, g::visuals_lby_font, false, false, Color(255, 0, 0), "FAKE");




	}

	/*
	if (Options::Menu.VisualsTab.LBYIndicator.GetState())
	{

		for (int i = 1; i <= g_pEngine->GetMaxClients(); ++i)
		{
			C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

			if (!pPlayerEntity
				|| !pPlayerEntity->IsAlive()
				|| pPlayerEntity->IsDormant()
				|| pPlayerEntity->IsImmune())
			{
				continue;
			}

			if (!pPlayerEntity || !pPlayerEntity->IsAlive() || pPlayerEntity->IsDormant())
				continue;

			if (pPlayerEntity == g::pLocalEntity || pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam())
				continue;


			float lspeed;

			lspeed = g_Aimbot.data[pPlayerEntity->EntIndex()].shoots;

			iY += 22;
			char jew[64];
			sprintf_s(jew, "Shoots fired: %.1f", lspeed);
			g_pSurface->DrawF(10, screen_height - iY, g::visuals_lby_font, false, false, Color(0, 255, 0), jew);

			float lspee22d;

			lspee22d = g_Aimbot.data[pPlayerEntity->EntIndex()].shoots_hit;

			iY += 22;
			char jew122[64];
			sprintf_s(jew122, "Shoots hit: %.1f", lspee22d);
			g_pSurface->DrawF(10, screen_height - iY, g::visuals_lby_font, false, false, Color(0, 255, 0), jew122);


			float lspee2321312d;

			lspee2321312d = g_Aimbot.data[pPlayerEntity->EntIndex()].shotsmissed;

			iY += 22;
			char j2332ew[64];
			sprintf_s(j2332ew, "Shoots missed: %.1f", lspee2321312d);
			g_pSurface->DrawF(10, screen_height - iY, g::visuals_lby_font, false, false, Color(0, 255, 0), j2332ew);
		}

	}*/

}

const wchar_t* to_wch345435ar(const char* string) {
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, string);
	_vsnprintf_s(buf, sizeof(buf), string, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	return wbuf;
}

void c_visuals::indicatorssss()
{

	if (!g_Options.esp.other.indicators)
		return;

	float	spectatorx;
	float	spectatory;

	spectatorx = g_Options.esp.other.indicators_x;
	spectatory = g_Options.esp.other.indicators_y;

	int w, h;


	int centerW, centerh;
	g_pEngine->GetScreenSize(w, h);
	centerW = w / 2;
	centerh = h / 2;



	g_pSurface->Clear((centerW * 2) - 1910 + spectatorx, 9 + spectatory, 152, 82, Color(0, 0, 0, 255));
	g_pSurface->Clear((centerW * 2) - 1909 + spectatorx, 10 + spectatory, 150, 82, Color(40, 40, 40, 255));
	g_pSurface->Clear((centerW * 2) - 1908 + spectatorx, 11 + spectatory, 148, 80, Color(18, 18, 18, 255));
	g_pSurface->Clear((centerW * 2) - 1905 + spectatorx, 14 + spectatory, 142, 74, Color(40, 40, 40, 255));
	g_pSurface->Clear((centerW * 2) - 1904 + spectatorx, 15 + spectatory, 140, 72, Color(8, 8, 8, 255));
	g_pSurface->GradientSideways((centerW * 2) - 1903 + spectatorx, 16 + spectatory, 137, 2, Color(menu::color), Color(menu::color), Menulooks::MenuAlpha);

	g_pSurface->Textf((centerW * 2) - 1901 + spectatorx, 22 + spectatory, Color(240, 240, 240, (Menulooks::MenuAlpha - 1)), g::xd, ("Slow walk"));
	if (g_Options.antiaa.slow_walk && GetAsyncKeyState(g_Options.antiaa.slow_walk_key))
	{

		g_pSurface->Textf((centerW * 2) - 1790 + spectatorx, 22 + spectatory, Color(menu::color, (Menulooks::MenuAlpha - 1)), g::xd, ("[On]"));



	}
	else
	{
		g_pSurface->Textf((centerW * 2) - 1790 + spectatorx, 22 + spectatory, Color(255, 255, 255, (Menulooks::MenuAlpha - 1)), g::xd, ("[Off]"));


	}

	static auto draw_bar = [](int x, int y, float val, float max, const wchar_t* name) {
		if (val > max)
			val = max;

		g_pSurface->draw_wide_string(true, g::INDICATOR, x + 76 / 2, y - 8, Color(255, 255, 255, 255), const_cast<wchar_t*>(name));
		g_pSurface->draw_filled_rect(x, y, 76, 5, Color(50, 50, 50, 150));
		g_pSurface->draw_filled_rect(x, y, 76 * (val / max), 5, Color(menu::color, 150));
	};


	g_pSurface->Textf((centerW * 2) - 1901 + spectatorx, 35 + spectatory, Color(240, 240, 240, (Menulooks::MenuAlpha - 1)), g::xd, ("Fake lag"));
	auto nci = g_pEngine->GetNetChannel();

	if (nci && g_Options.antiaa.fakelag)
	{
		draw_bar((centerW * 2) - 1848 + spectatorx, 39 + spectatory, nci->m_nChokedPackets, 14.0f, L"");

	}
	else
	{
		g_pSurface->Textf((centerW * 2) - 1790 + spectatorx, 35 + spectatory, Color(255, 255, 255, (Menulooks::MenuAlpha - 1)), g::xd, ("[Off]"));

	}

	g_pSurface->Textf((centerW * 2) - 1901 + spectatorx, 48 + spectatory, Color(240, 240, 240, (Menulooks::MenuAlpha - 1)), g::xd, ("Fake duck"));

	if (g_Options.antiaa.fake_duck && GetAsyncKeyState(g_Options.antiaa.fake_duck_key))
	{

		g_pSurface->Textf((centerW * 2) - 1790 + spectatorx, 48 + spectatory, Color(menu::color, (Menulooks::MenuAlpha - 1)), g::xd, ("[On]"));



	}
	else
	{
		g_pSurface->Textf((centerW * 2) - 1790 + spectatorx, 48 + spectatory, Color(255, 255, 255, (Menulooks::MenuAlpha - 1)), g::xd, ("[Off]"));


	}

	g_pSurface->Textf((centerW * 2) - 1901 + spectatorx, 61 + spectatory, Color(240, 240, 240, (Menulooks::MenuAlpha - 1)), g::xd, ("Trigger bot"));



	if ((g_Options.legit.trigger.key && g_Options.legit.trigger.method == 1 && !GetAsyncKeyState(g_Options.legit.trigger.key)) || (g_Options.legit.trigger.on_key && g_Options.legit.trigger.method == 0 && overridething_legit))
	{

		g_pSurface->Textf((centerW * 2) - 1790 + spectatorx, 61 + spectatory, Color(255, 255, 255, (Menulooks::MenuAlpha - 1)), g::xd, ("[Off]"));



	}
	else
	{
		g_pSurface->Textf((centerW * 2) - 1790 + spectatorx, 61 + spectatory, Color(menu::color, (Menulooks::MenuAlpha - 1)), g::xd, ("[On]"));


	}

	g_pSurface->Textf((centerW * 2) - 1901 + spectatorx, 74 + spectatory, Color(240, 240, 240, (Menulooks::MenuAlpha - 1)), g::xd, ("Double Tap"));


	if ((g_Options.antiaa.double_tap_on_key && g_Options.antiaa.double_tap_method == 1 && GetAsyncKeyState(g_Options.antiaa.double_tap_key)) || (g_Options.antiaa.double_tap_on_key && g_Options.antiaa.double_tap_method == 0 && overridething_dt_key))
	{

		g_pSurface->Textf((centerW * 2) - 1790 + spectatorx, 74 + spectatory, Color(menu::color, (Menulooks::MenuAlpha - 1)), g::xd, ("[On]"));



	}
	else if (!g_Options.antiaa.double_tap_on_key && g_Options.antiaa.double_tap)
	{
		g_pSurface->Textf((centerW * 2) - 1790 + spectatorx, 74 + spectatory, Color(menu::color, (Menulooks::MenuAlpha - 1)), g::xd, ("[On]"));

	}
	else
	{
		g_pSurface->Textf((centerW * 2) - 1790 + spectatorx, 74 + spectatory, Color(255, 255, 255, (Menulooks::MenuAlpha - 1)), g::xd, ("[Off]"));


	}
	//g_pSurface->Textf((centerW * 2) - 1842, 22, Color(240, 240, 240, (Menulooks::MenuAlpha - 1)), g::xd, ("| %s | fps: %d "), GetTimeString().c_str(), get23432fps());


}


void c_visuals::spectators()
{

	if (!g_Options.esp.other.spectator_list)
		return;

	//if (g_csgo.m_engine()->IsInGame() && g_csgo.m_engine()->IsConnected()) {
	int index = 0;
	int w, h;
	g_pEngine->GetScreenSize(w, h);
	float	spectatorx;
	float	spectatory;

	spectatorx = g_Options.esp.other.spectator_x;
	spectatory = g_Options.esp.other.spectator_y;

	g_pSurface->text(g::Tahoma, spectatorx + 6, spectatory + 5, Color(255, 255, 255, 255), HFONT_OFF_SCREEN, "spectators");
	g_pSurface->rect_filled(spectatorx - 69, spectatory - 9, 200, 100, Color(0, 0, 0, 189));
	g_pSurface->rect_filled(spectatorx - 73, spectatory - 13, 209, 109, Color(0, 0, 0, 189));
	//g_pSurface->gradient(935, 151, 192, 2, Color(51, 132, 255, 255), Color(235, 29, 173, 255), GRADIENT_HORIZONTAL);
	g_pSurface->GradientSideways(spectatorx - 65, spectatory + 1, 192, 2, Color(menu::color), Color(menu::color), 1);

	for (int i = 1; i < 65; i++)
	{
		auto p_local = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		auto p_entity = g_pEntityList->GetClientEntity(i);
		PlayerInfo_t e_info;

		if (p_entity && p_entity != p_local)
		{
			g_pEngine->GetPlayerInfo(i, &e_info);
			if (!p_entity->IsAlive() && !p_entity->IsDormant())
			{
                if (g_Options.misc.radar && p_entity->IsAlive())
                {
                    static auto m_bSpotted = NetvarManager::Instance()->GetOffset("DT_BaseEntity", "m_bSpotted");

                    *(char*)((DWORD)(p_entity)+m_bSpotted) = 1;
                }
				auto target = p_entity->GetObserverTargetHandle();
				if (target)
				{
					auto p_target = g_pEntityList->GetClientEntityFromHandle(target);

					if (p_target == p_local)
					{
						int x, y;
						g_pSurface->GetTextSize(g::Tahoma, to_wch345435ar(e_info.szName), x, y);

						std::string player_name = e_info.szName;
						PlayerInfo_t p_info;
						g_pEngine->GetPlayerInfo(i, &e_info);

						g_pSurface->text(g::Tahoma, spectatorx + 6, spectatory + 25 + (10 * index), Color(255, 255, 255, 255), HFONT_OFF_SCREEN, player_name.c_str());
						index++;
					}
				}


			}
		}
	}
}


#define   CONTENTS_SOLID                0x1       
#define   CONTENTS_MOVEABLE             0x4000
#define   CONTENTS_WINDOW               0x2
#define   CONTENTS_DEBRIS               0x4000000

#define   MASK_SHOT_BRUSHONLY           (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)

void c_visuals::DrawFeetRange()
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player)
		return;
	if (!local_player->IsAlive())
		return;

	if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame())
		return;

	if (!g_Options.esp.other.feet_circle)
		return;


	float step = M_PI * 2.0 / 123;
	float rad = 45;
	Vector origin = local_player->get_eye_pos();

	static double rainbow;

	Vector screenPos;
	static Vector prevScreenPos;

	for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
	{
		Vector pos(rad * cos(rotation) + origin.x, rad * sin(rotation) + origin.y, origin.z - 100);

		C_Trace trace;
		C_TraceFilter filter(local_player);


		filter.pSkip1 = local_player;


		g_pTrace->TraceRay(C_Ray(origin, pos), MASK_SHOT_BRUSHONLY, &filter, &trace);


		if (DebugOverlay->ScreenPosition(trace.end, screenPos))
			continue;

		if (!prevScreenPos.IsZero() && !screenPos.IsZero() && screenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f && prevScreenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f) {
			rainbow += 0.00001;
			if (rainbow > 1.f)
				rainbow = 0;

			Color color = Color::FromHSB(rainbow, 1.f, 1.f);
			g_pSurface->Line(prevScreenPos.x, prevScreenPos.y, screenPos.x, screenPos.y, color);
			//	DrawLine(prevScreenPos.x, prevScreenPos.y + 1, screenPos.x, screenPos.y + 1, color);
			//	DrawLine(Vector2D(prevScreenPos.x, prevScreenPos.y - 1), Vector2D(screenPos.x, screenPos.y - 1), color);
		}

		prevScreenPos = screenPos;
	}



}

void c_visuals::DrawZeusRange()
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player)
		return;
	if (!local_player->IsAlive())
		return;

	if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame())
		return;

	if (!g_Options.esp.other.zeus_range)
		return;

	auto weapon = g::pLocalEntity->GetActiveWeapon();
	if (!weapon)
		return;

	if (*weapon->m_iItemDefinitionIndex() == (int)ItemDefinitionIndex::WEAPON_ZEUS)
	{

		float step = M_PI * 2.0 / 123;
		float rad = weapon->GetCSWpnData()->range;

		Vector origin = local_player->get_eye_pos();

		static double rainbow;

		Vector screenPos;
		static Vector prevScreenPos;

		for (float rotation = 0; rotation < (M_PI * 2.0); rotation += step)
		{
			Vector pos(rad * cos(rotation) + origin.x, rad * sin(rotation) + origin.y, origin.z - 100);

			C_Trace trace;
			C_TraceFilter filter(local_player);


			filter.pSkip1 = local_player;


			g_pTrace->TraceRay(C_Ray(origin, pos), MASK_SHOT_BRUSHONLY, &filter, &trace);


			if (DebugOverlay->ScreenPosition(trace.end, screenPos))
				continue;

			if (!prevScreenPos.IsZero() && !screenPos.IsZero() && screenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f && prevScreenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f) {
				rainbow += 0.00001;
				if (rainbow > 1.f)
					rainbow = 0;

				Color color = Color(esp::colors::zeus_range);
				g_pSurface->Line(prevScreenPos.x, prevScreenPos.y, screenPos.x, screenPos.y, color);
				//	DrawLine(prevScreenPos.x, prevScreenPos.y + 1, screenPos.x, screenPos.y + 1, color);
				//	DrawLine(Vector2D(prevScreenPos.x, prevScreenPos.y - 1), Vector2D(screenPos.x, screenPos.y - 1), color);
			}

			prevScreenPos = screenPos;
		}

	}

}

void c_visuals::penetration_reticle()
{
	/*
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	auto weapon = g::pLocalEntity->GetActiveWeapon();
	if (!weapon)
		return;

	Vector aimPoint = local_player->get_eye_pos();

	int screen_x, screen_y;
	g_pEngine->GetScreenSize(screen_x, screen_y);

	if (autowall->CanHit(aimPoint) >= 1.f)//SETTINGS::settings.damage_val)
		g_pSurface->DrawFilledRect(screen_x / 2, screen_y / 2, 2, 2, Color(0, 255, 0));
	else
		g_pSurface->DrawFilledRect(screen_x / 2, screen_y / 2, 2, 2, Color(255, 0, 0));*/

	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player)
		return;

	if (!local_player->IsAlive())
		return;

	auto draw_xhair = [](float size, Color color) -> void {
		int x_1, x_2, y_1, y_2;

		int
			screen_w = g_pSurface->viewport().right / 2,
			screen_h = g_pSurface->viewport().bottom / 2;

		x_1 = screen_w - (size / 4);
		x_2 = screen_w + (size / 4) + 1;

		y_1 = screen_h - (size / 4);
		y_2 = screen_h + (size / 4) + 1;

		g_pSurface->rect_filled(screen_w - 1, screen_h - 1, 3, 3, color);
	};

	auto weapon = local_player->GetActiveWeapon();
	if (!weapon)
		return;


	float damage = 0.f;

	Color color = Color(255, 0, 0);
	if (g_Autowall.CanWallbang(damage))//SETTINGS::settings.damage_val)
		color = Color(163, 225, 21);

	draw_xhair(2, color);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class aqemzor {
public:
	string jshshuwrb;
	aqemzor();
	string svgepsqfcptmhl(string cuhqskfge, bool dslfpbx, bool krbqile, string abdxqlil, int bpqaeux, double krvcuxbgp);
	double nljtuvqqvdezyesis(double gejtglvpjekbwb, string uzsqb, int kckovktwhvsekmo, double nwyeif, double xscxa, double hyyyvodvqjeqqt, string uxfoaav);
	double difhajvrifnmvzyp(double dsdpjebsyuyggk, bool ytijac, bool spehdfbc, string jlffwtuwkpdzv, int jmzdr, bool svrhtmexjbnvl);
	double hjrlbccuxtyyzzzycetczwi(double uybfqnsjtr, int yslxox);
	int xagxfwkbknbqwbbjnxr(bool rnpbdyg);
	double bxltrisxuiize(bool iffynsdpzziy, bool orlxjhpl, int ynawoadukuzio, string enjlyyykkvlo);
	double uihwvjiarftwc(double ularbg, double dgfskht, string xizpzrasduy, double osrdfljjqzquqou, bool baqrzmzf, double nfsdzdlofiuutvv);
	void uolgochbxdjfbllkh(string zjzpupsqogzgzh, string fiofzjbsryoa, int vhnvpohzk, string szojstetxfpfjw, double rjecdukzbcakvja, double qouvgumrzmd, int owgwgatoaawd);

protected:
	double rtoytjnl;

	double bocoztpexozxwv(bool uglvsgahsihgl, bool micxa);
	string stqyvrfooxemfghezcquuu(double qbovhbgy, int lspuwvzyy, double zokxcrts, string gqhzrhnxdq, int nmoscpxjpsbena, int mvrborgambuaz, bool hpvxwya, int bjmkgdaqkkixnbc, string qyyrnxeaysae);
	double qvixilfoccpeocixalsfkpwnp(bool gmnjo, double mweylenqznsmnwj, string pskhsmmzwivvori, int qeoqmghcymh);
	string flijjvpipsah();
	double szzjngpcemshlyiyq(string yqgyaeg, bool qyxpq);
	void ymsucydwrrvvtta(int eamzz, int hrefv, int asxmxwudhuhjlz, int cbxopcsearqxav, double nftcjcgqkc);
	string njpzmsxpumiausamginkyreye(double sbrcpxa, int naaaeeusgo, int cdkmbhzrkc, bool zitdwqfmvdo, string mmsvpgxcotcx, int bypmcnb, double ezhojzprqbqydn);

private:
	bool luftpzzigdxubz;
	bool mdtrgohplcejeeg;
	double qpmxidjnnblq;

	bool qllxbzdutwmpphig(string xbspxtpdpum, bool neprjpcaf, bool coynd);
	string wbumpmpnhtiqfgsqdtuel(int mlvghihizbsuhwi);
	void rizlldipmrrzjsluuag(int joetso, double cdqebptrl, double ggbgd, bool nfjisxkptsntg);

};


bool aqemzor::qllxbzdutwmpphig(string xbspxtpdpum, bool neprjpcaf, bool coynd) {
	bool wqtexa = false;
	if (false != false) {
		int drgcktp;
		for (drgcktp = 90; drgcktp > 0; drgcktp--) {
			continue;
		}
	}
	if (false != false) {
		int lwoi;
		for (lwoi = 77; lwoi > 0; lwoi--) {
			continue;
		}
	}
	return false;
}

string aqemzor::wbumpmpnhtiqfgsqdtuel(int mlvghihizbsuhwi) {
	return string("fapttdcxrwbwcnspfn");
}

void aqemzor::rizlldipmrrzjsluuag(int joetso, double cdqebptrl, double ggbgd, bool nfjisxkptsntg) {
	int owhyzsnaiea = 1846;
	bool rdsstedgleoqrz = false;
	double aidyyupczbb = 53990;
	bool hendkznpg = true;
	double mbkqpw = 59660;
	bool agzmhxxpitjozz = false;
	bool wdvmaufublxcqaw = false;
	if (false == false) {
		int kg;
		for (kg = 25; kg > 0; kg--) {
			continue;
		}
	}
	if (false != false) {
		int cxcttqm;
		for (cxcttqm = 97; cxcttqm > 0; cxcttqm--) {
			continue;
		}
	}
	if (false != false) {
		int pwwuc;
		for (pwwuc = 27; pwwuc > 0; pwwuc--) {
			continue;
		}
	}
	if (1846 == 1846) {
		int hgaatsck;
		for (hgaatsck = 57; hgaatsck > 0; hgaatsck--) {
			continue;
		}
	}

}

double aqemzor::bocoztpexozxwv(bool uglvsgahsihgl, bool micxa) {
	double dryeevx = 14483;
	string ttvpnxpeeion = "xlkwpzdttzvrnavvljmxnwtfdo";
	double frnwqcbpsek = 13307;
	double yedglbxkyscqk = 10854;
	double soqmustorllrvh = 13023;
	int wmffehwypfh = 3157;
	bool jwuzhajroxmnr = false;
	int afyoxtssedfpo = 4789;
	if (false == false) {
		int whcxkna;
		for (whcxkna = 67; whcxkna > 0; whcxkna--) {
			continue;
		}
	}
	if (4789 != 4789) {
		int zxtgmqgmh;
		for (zxtgmqgmh = 8; zxtgmqgmh > 0; zxtgmqgmh--) {
			continue;
		}
	}
	if (13023 == 13023) {
		int zybxcy;
		for (zybxcy = 19; zybxcy > 0; zybxcy--) {
			continue;
		}
	}
	if (4789 != 4789) {
		int wo;
		for (wo = 77; wo > 0; wo--) {
			continue;
		}
	}
	return 34850;
}

string aqemzor::stqyvrfooxemfghezcquuu(double qbovhbgy, int lspuwvzyy, double zokxcrts, string gqhzrhnxdq, int nmoscpxjpsbena, int mvrborgambuaz, bool hpvxwya, int bjmkgdaqkkixnbc, string qyyrnxeaysae) {
	return string("yrn");
}

double aqemzor::qvixilfoccpeocixalsfkpwnp(bool gmnjo, double mweylenqznsmnwj, string pskhsmmzwivvori, int qeoqmghcymh) {
	int wdkrd = 3141;
	string txprqasogulew = "qnjdlccznfmiytpaabrgczhgbclyujsvxjpnicfvkqobuahj";
	int inbcdhpxwflcsbp = 414;
	double jmvxxyrtpwzxvql = 9651;
	double puuuilmgpc = 13272;
	double jmxziakl = 21616;
	bool tyehqcbgh = true;
	if (true != true) {
		int aewoihoy;
		for (aewoihoy = 24; aewoihoy > 0; aewoihoy--) {
			continue;
		}
	}
	if (3141 == 3141) {
		int zyvpzriwi;
		for (zyvpzriwi = 35; zyvpzriwi > 0; zyvpzriwi--) {
			continue;
		}
	}
	if (true == true) {
		int vvadfeos;
		for (vvadfeos = 15; vvadfeos > 0; vvadfeos--) {
			continue;
		}
	}
	return 78076;
}

string aqemzor::flijjvpipsah() {
	string dildqj = "tumpnodfkekmmvfjqneecrvjwbeibitmoedrbzrabwhtjzzcxfutgs";
	string fqktcpgkyuclx = "bfhsblzzguo";
	string pecwxlrtoideczp = "azzyllvlhstivcekuzsfgqvbrq";
	int ottmwovpylv = 2578;
	string zqztxa = "hdidibbbtxuwtrojsthflrkzpdhdyfypkijnwbkkahsbmcakcklyp";
	double libdvsstgt = 43383;
	bool rilravbr = true;
	string azfdqe = "gpqrteolpv";
	if (string("hdidibbbtxuwtrojsthflrkzpdhdyfypkijnwbkkahsbmcakcklyp") != string("hdidibbbtxuwtrojsthflrkzpdhdyfypkijnwbkkahsbmcakcklyp")) {
		int dy;
		for (dy = 76; dy > 0; dy--) {
			continue;
		}
	}
	return string("trlpnfwujabgqrd");
}

double aqemzor::szzjngpcemshlyiyq(string yqgyaeg, bool qyxpq) {
	bool xzqriqhllctpx = true;
	int mlklkbkxxiqs = 3312;
	bool fkoyynambeep = false;
	string najtgxednxbpg = "gtdfmlnqyjlerhqiuezrushyiaxjhaxwzhkinpqpfjwupgtextlmzseqehohbsy";
	double voiwkwjzxr = 13265;
	int grgmbumyarcjkf = 7386;
	string ozwobepykq = "sfkoiyfdvqmvfbxlvosmfeicoakjimkfgbxhpwdeurjoewzxlqzpqfmlpprsueqxodfjh";
	return 65859;
}

void aqemzor::ymsucydwrrvvtta(int eamzz, int hrefv, int asxmxwudhuhjlz, int cbxopcsearqxav, double nftcjcgqkc) {
	bool oajjdkrlwy = false;
	string cfibtsqznq = "dut";
	int pfxuuytvkbojhqp = 5412;
	double kmonyooqczwgmpj = 27056;
	int bmqmhywux = 740;
	string rlmxnhevlqh = "zstuqucxcojanvsvvvetsjglbiimxdwnhoafgghhijhdxrcskpctkonirpyamcthqnwjd";
	bool xgzvfnf = false;
	bool zirqovy = false;
	int lyxqj = 2639;
	if (5412 != 5412) {
		int ulqilgxfo;
		for (ulqilgxfo = 93; ulqilgxfo > 0; ulqilgxfo--) {
			continue;
		}
	}
	if (740 == 740) {
		int tttyutf;
		for (tttyutf = 28; tttyutf > 0; tttyutf--) {
			continue;
		}
	}
	if (false == false) {
		int bjmb;
		for (bjmb = 65; bjmb > 0; bjmb--) {
			continue;
		}
	}
	if (5412 == 5412) {
		int taj;
		for (taj = 46; taj > 0; taj--) {
			continue;
		}
	}

}

string aqemzor::njpzmsxpumiausamginkyreye(double sbrcpxa, int naaaeeusgo, int cdkmbhzrkc, bool zitdwqfmvdo, string mmsvpgxcotcx, int bypmcnb, double ezhojzprqbqydn) {
	double kxxfcbhhoxvoo = 37441;
	string ivuny = "gyzrgmsdbeedyvqacxcrolovvqyandmdpbuwexekvfilwtpfmerormdgfjoccuidbrhkwlbolcbeqwipmrynlaqbptjrh";
	double aguktlfafwdjaw = 13488;
	bool ilgdsbctuecw = false;
	string lvrkl = "sotbggxrkbbuayehjkyhtqwudgavqbgoypthhp";
	int glsxkkbr = 785;
	double xfvgoeetfuyomw = 10781;
	return string("fanwrq");
}

string aqemzor::svgepsqfcptmhl(string cuhqskfge, bool dslfpbx, bool krbqile, string abdxqlil, int bpqaeux, double krvcuxbgp) {
	double jagdrwzhrdig = 8044;
	bool wwewaebl = false;
	double ckwbgnmdxjeblpf = 1459;
	bool ytuwpybhf = false;
	if (1459 == 1459) {
		int nfvkuna;
		for (nfvkuna = 67; nfvkuna > 0; nfvkuna--) {
			continue;
		}
	}
	if (8044 == 8044) {
		int hnzickpxz;
		for (hnzickpxz = 50; hnzickpxz > 0; hnzickpxz--) {
			continue;
		}
	}
	if (false != false) {
		int qvqtemqno;
		for (qvqtemqno = 52; qvqtemqno > 0; qvqtemqno--) {
			continue;
		}
	}
	if (8044 == 8044) {
		int xycurscban;
		for (xycurscban = 91; xycurscban > 0; xycurscban--) {
			continue;
		}
	}
	return string("rgsypnusnwywlalzlkh");
}

double aqemzor::nljtuvqqvdezyesis(double gejtglvpjekbwb, string uzsqb, int kckovktwhvsekmo, double nwyeif, double xscxa, double hyyyvodvqjeqqt, string uxfoaav) {
	int cyiymzvsfazxc = 2655;
	int pzsrvitifo = 94;
	double qlzqxnwofkhtt = 17710;
	int hejsditv = 2643;
	int aoyezzszzmo = 589;
	return 4718;
}

double aqemzor::difhajvrifnmvzyp(double dsdpjebsyuyggk, bool ytijac, bool spehdfbc, string jlffwtuwkpdzv, int jmzdr, bool svrhtmexjbnvl) {
	int dujrovzxeofrw = 1534;
	bool qculthrmwkik = false;
	double ivscwbusgwg = 6766;
	double xgpbkwfomn = 51854;
	bool xukxjoocdmb = false;
	int xikxnyyklejzx = 526;
	int lbnfvnb = 2204;
	if (51854 == 51854) {
		int itihhp;
		for (itihhp = 32; itihhp > 0; itihhp--) {
			continue;
		}
	}
	if (false != false) {
		int ptluo;
		for (ptluo = 50; ptluo > 0; ptluo--) {
			continue;
		}
	}
	if (false != false) {
		int ptyellhuf;
		for (ptyellhuf = 87; ptyellhuf > 0; ptyellhuf--) {
			continue;
		}
	}
	return 57315;
}

double aqemzor::hjrlbccuxtyyzzzycetczwi(double uybfqnsjtr, int yslxox) {
	string lhwtqgcbmvfhmt = "ubnsoxfggywkdibdslrfieghgdsaxeqikruehbzjplsbbtuzeeptmamblfrgcplkinqwlzbnxbcusrd";
	if (string("ubnsoxfggywkdibdslrfieghgdsaxeqikruehbzjplsbbtuzeeptmamblfrgcplkinqwlzbnxbcusrd") == string("ubnsoxfggywkdibdslrfieghgdsaxeqikruehbzjplsbbtuzeeptmamblfrgcplkinqwlzbnxbcusrd")) {
		int lntmx;
		for (lntmx = 4; lntmx > 0; lntmx--) {
			continue;
		}
	}
	if (string("ubnsoxfggywkdibdslrfieghgdsaxeqikruehbzjplsbbtuzeeptmamblfrgcplkinqwlzbnxbcusrd") == string("ubnsoxfggywkdibdslrfieghgdsaxeqikruehbzjplsbbtuzeeptmamblfrgcplkinqwlzbnxbcusrd")) {
		int drgcwkrfz;
		for (drgcwkrfz = 19; drgcwkrfz > 0; drgcwkrfz--) {
			continue;
		}
	}
	if (string("ubnsoxfggywkdibdslrfieghgdsaxeqikruehbzjplsbbtuzeeptmamblfrgcplkinqwlzbnxbcusrd") != string("ubnsoxfggywkdibdslrfieghgdsaxeqikruehbzjplsbbtuzeeptmamblfrgcplkinqwlzbnxbcusrd")) {
		int adt;
		for (adt = 75; adt > 0; adt--) {
			continue;
		}
	}
	if (string("ubnsoxfggywkdibdslrfieghgdsaxeqikruehbzjplsbbtuzeeptmamblfrgcplkinqwlzbnxbcusrd") != string("ubnsoxfggywkdibdslrfieghgdsaxeqikruehbzjplsbbtuzeeptmamblfrgcplkinqwlzbnxbcusrd")) {
		int ugipgkkwzg;
		for (ugipgkkwzg = 78; ugipgkkwzg > 0; ugipgkkwzg--) {
			continue;
		}
	}
	return 6288;
}

int aqemzor::xagxfwkbknbqwbbjnxr(bool rnpbdyg) {
	string efdmettooyegp = "oaygahiogomsbsczxiprwpzkgjgncvltmqfdzmkwpszljqmzoulmq";
	bool uletrbxykzqh = false;
	string kmgign = "udidmeeshgqoodyzyjlcciwstbeottgkxrphtwnbxvfpxahyglalxwmdzeflredwwjfsjjzul";
	if (string("udidmeeshgqoodyzyjlcciwstbeottgkxrphtwnbxvfpxahyglalxwmdzeflredwwjfsjjzul") == string("udidmeeshgqoodyzyjlcciwstbeottgkxrphtwnbxvfpxahyglalxwmdzeflredwwjfsjjzul")) {
		int nxtzwtdw;
		for (nxtzwtdw = 7; nxtzwtdw > 0; nxtzwtdw--) {
			continue;
		}
	}
	if (string("udidmeeshgqoodyzyjlcciwstbeottgkxrphtwnbxvfpxahyglalxwmdzeflredwwjfsjjzul") == string("udidmeeshgqoodyzyjlcciwstbeottgkxrphtwnbxvfpxahyglalxwmdzeflredwwjfsjjzul")) {
		int fgwowb;
		for (fgwowb = 89; fgwowb > 0; fgwowb--) {
			continue;
		}
	}
	if (string("oaygahiogomsbsczxiprwpzkgjgncvltmqfdzmkwpszljqmzoulmq") == string("oaygahiogomsbsczxiprwpzkgjgncvltmqfdzmkwpszljqmzoulmq")) {
		int yczcaqbspe;
		for (yczcaqbspe = 14; yczcaqbspe > 0; yczcaqbspe--) {
			continue;
		}
	}
	return 62490;
}

double aqemzor::bxltrisxuiize(bool iffynsdpzziy, bool orlxjhpl, int ynawoadukuzio, string enjlyyykkvlo) {
	double adqtyzvtjljpsk = 4465;
	int hjiqwkiaao = 1505;
	string ylcjpuuazfxae = "oauyqnwkiiyamm";
	double xnrtfqo = 23245;
	double npktcjhitxge = 10069;
	double pjwqnplcufcmho = 6358;
	double irbomatspncdgb = 65027;
	if (23245 == 23245) {
		int brmwpx;
		for (brmwpx = 39; brmwpx > 0; brmwpx--) {
			continue;
		}
	}
	if (65027 == 65027) {
		int ebjxq;
		for (ebjxq = 45; ebjxq > 0; ebjxq--) {
			continue;
		}
	}
	return 42427;
}

double aqemzor::uihwvjiarftwc(double ularbg, double dgfskht, string xizpzrasduy, double osrdfljjqzquqou, bool baqrzmzf, double nfsdzdlofiuutvv) {
	return 68069;
}

void aqemzor::uolgochbxdjfbllkh(string zjzpupsqogzgzh, string fiofzjbsryoa, int vhnvpohzk, string szojstetxfpfjw, double rjecdukzbcakvja, double qouvgumrzmd, int owgwgatoaawd) {
	string cnydy = "ladlcruyrcropqjtjgvhdywoeydtbunbdfzrurwnwdphtfw";
	string kghevbaqex = "hfdoljjswrdypjusmxqdlieskdohhdyxdetxmkgegaxcxvjffrnucgqxrsqflzsqnvgaviq";
	double ulbbsiwtlgsilh = 8059;
	double zlqvlteb = 27689;
	double jhzvlddbr = 48637;
	double etoshalzlk = 4552;
	int isrvkpurrrriiq = 2101;
	double clyib = 11773;
	double ptggmyvgvd = 14689;
	double bqozjiv = 18849;
	if (11773 != 11773) {
		int stmbopn;
		for (stmbopn = 39; stmbopn > 0; stmbopn--) {
			continue;
		}
	}

}

aqemzor::aqemzor() {
	this->svgepsqfcptmhl(string("xtvjqflujulzsloaikkxxjpfgthiunpmlgstmeprpwcbozjllhdyhljmqeqxttfbllgq"), true, false, string("zyyxhteqjemtpffiagtkdqchnijrlmkopikxnctsnlicwhhtsgvzlsotoncwknmqbvzgzeemgpkqqhhdfezfcczevzkze"), 854, 34297);
	this->nljtuvqqvdezyesis(614, string("srvrjp"), 3376, 6922, 11342, 31699, string("zyqcszjweeondgnzuemptheoljorsgzyubiu"));
	this->difhajvrifnmvzyp(71509, true, false, string("pnpaivmxfmwbbfsyk"), 418, true);
	this->hjrlbccuxtyyzzzycetczwi(26270, 3562);
	this->xagxfwkbknbqwbbjnxr(false);
	this->bxltrisxuiize(false, true, 632, string("ycianibrtywkvtlqcjjkxbofzencpdrcizcap"));
	this->uihwvjiarftwc(52809, 7110, string("wincvopvzoygcrtfsdjipqocxulijmxhnhrettjzagwe"), 10195, false, 8630);
	this->uolgochbxdjfbllkh(string("ucrwuvntroxduaoleoexrcssfyfvadjepvsjpdyyhjlhkrfalpfryznpxikiposofwxzerhfabmwdkijosfvwjy"), string("jyhqeeqzn"), 84, string("qvkuxyiofdnbxtbohkweyoswbtocferyxkuoidzsqeghhqrxkjzuirxakegipf"), 50386, 57928, 4530);
	this->bocoztpexozxwv(false, true);
	this->stqyvrfooxemfghezcquuu(67776, 1698, 8081, string("ysrexqwotufafbcmduzkxwinnbheluynxtnjxkryativqjiwmxxnsrqslpntxaktqrvuzwjdxrlksmgdxoe"), 2952, 1493, true, 5588, string(""));
	this->qvixilfoccpeocixalsfkpwnp(true, 72301, string("cujvmoriu"), 3439);
	this->flijjvpipsah();
	this->szzjngpcemshlyiyq(string("dwlnxqktbdcfwcwhtdcgsjwqyiejvpqxnbhtrqykfvsohmbfbbtrafhjqzkqfssdysebjdlrsndtnjtetgbaqzewoullv"), true);
	this->ymsucydwrrvvtta(424, 808, 1497, 1418, 11672);
	this->njpzmsxpumiausamginkyreye(39264, 3412, 2375, true, string("xevbuoznfftchfdhngjosgvymffhihzncoeqsyllgg"), 5232, 83283);
	this->qllxbzdutwmpphig(string("tyfrcixkdjfgmyhmjtkkkgdeybqnyizoqzyklqqdmaaomfuhpcnybkapotibzefejjhhkagjpffemv"), true, true);
	this->wbumpmpnhtiqfgsqdtuel(1245);
	this->rizlldipmrrzjsluuag(1938, 29447, 61479, false);
}
