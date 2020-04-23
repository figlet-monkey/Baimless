#include "ESP.h"
#include "..\Aimbot\Aimbot.h"
#include "..\Aimbot\LagComp.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\SDK\ISurface.h"
#include "..\..\SDK\Hitboxes.h"
#include "..\..\Utils\Math.h"
#include "..\..\newMenu.h"
#include "..\..\newMenu.h"
#include "../../SDK/ICvar.h"
#include "algorithm"
#include "..\Aimbot\Autowall.h"
#include "..\..\SDK\IVModelInfo.h"
#include "..//..//..//sounds.h"
#include <Options.hpp>
float flPlayerAlphaOLD[255];
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))
Color main_color;
Color ammo;

void ISurface::DrawEmptyRect(int x1, int y1, int x2, int y2, Color color, unsigned char ignore_flags)
{
	DrawSetColor(color);
	if (!(ignore_flags & 0b1))
		DrawLine(x1, y1, x2, y1);
	if (!(ignore_flags & 0b10))
		DrawLine(x2, y1, x2, y2);
	if (!(ignore_flags & 0b100))
		DrawLine(x2, y2, x1, y2);
	if (!(ignore_flags & 0b1000))
		DrawLine(x1, y2, x1, y1);
}

RECT CVisuals::viewport()
{
	RECT viewport = { 0, 0, 0, 0 };

	int width, height;
	g_pEngine->GetScreenSize(width, height);
	viewport.right = width;
	viewport.bottom = height;

	return viewport;
}

bool CVisuals::world_to_screen(const Vector &world, Vector &screen)
{
	auto screen_transform = [&world, &screen]() -> bool {
		static auto &matrix = g_pEngine->WorldToScreenMatrix();

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

int LagCompBreakNEW()
{
	Vector velocity = g::pLocalEntity->GetVelocity();
	velocity.z = 0;
	float speed = velocity.Length();

	if (speed > 0.f)
	{
		auto distance_per_tick = speed *
			g_pGlobalVars->intervalPerTick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, g_Options.antiaa.fake_lag_moving);
	}
	return 1;

}
const wchar_t* to_wchar(const char* string) {
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, string);
	_vsnprintf_s(buf, sizeof(buf), string, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	return wbuf;
}


void CVisuals::spectators()
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

	for (int i = 1; i < 65; i++) {
		auto p_local = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		auto p_entity = g_pEntityList->GetClientEntity(i);
		PlayerInfo_t e_info;

		if (p_entity && p_entity != p_local)
		{
			g_pEngine->GetPlayerInfo(i, &e_info);
			if (!p_entity->IsAlive() && !p_entity->IsDormant())
			{
				auto target = p_entity->GetObserverTargetHandle();
				if (target)
				{
					auto p_target = g_pEntityList->GetClientEntityFromHandle(target);

					if (p_target == p_local)
					{
						int x, y;
						g_pSurface->GetTextSize(g::Tahoma, to_wchar(e_info.szName), x, y);

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







void AngleVe4545ctors(const Vector &angles, Vector *forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}


void DrawLinesAA()
{

	Vector src3D, dst3D, forward, src, dst;
	C_Trace tr;
	C_TraceFilter filter(g::pLocalEntity);

	filter.pSkip1 = g::pLocalEntity;

	src3D = g::pLocalEntity->GetOrigin();

	AngleVe4545ctors(Vector(0, g::pLocalEntity->GetLowerBodyYaw(), 0), &forward);
	dst3D = src3D + (forward * 25.f); // Line Length
	g_pTrace->TraceRay(C_Ray(src3D, dst3D), 0, &filter, &tr);

	if (!visuals->world_to_screen(src3D, src) || !visuals->world_to_screen(tr.end, dst)) // Lowerbody
		return;

	g_pSurface->Line(src.x, src.y, dst.x, dst.y, Color(180, 10, 230, 255));
	g_pSurface->Text(dst.x, dst.y, Color(180, 10, 230, 255), g::Tahoma, "LBY");









	AngleVe4545ctors(Vector(0, g::RealAngle.y, 0), &forward);
	dst3D = src3D + (forward * 25.f);

	g_pTrace->TraceRay(C_Ray(src3D, dst3D), 0, &filter, &tr);

	if (!visuals->world_to_screen(src3D, src) || !visuals->world_to_screen(tr.end, dst)) // Lowerbody
		return;

	g_pSurface->Line(src.x, src.y, dst.x, dst.y, Color(1, 180, 250, 255));
	g_pSurface->Text(dst.x, dst.y, Color(1, 180, 250, 255), g::Tahoma, "Real");




	AngleVe4545ctors(Vector(0, g::FakeAngle.y, 0), &forward);
	dst3D = src3D + (forward * 25.f);

	g_pTrace->TraceRay(C_Ray(src3D, dst3D), 0, &filter, &tr);

	if (!visuals->world_to_screen(src3D, src) || !visuals->world_to_screen(tr.end, dst)) // Lowerbody
		return;

	g_pSurface->Line(src.x, src.y, dst.x, dst.y, Color(255, 0, 90, 255));
	g_pSurface->Text(dst.x, dst.y, Color(255, 0, 90, 255), g::Tahoma, "Fake");
}

void chocked_indicators()
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


void CVisuals::QuakeKillCounter()
{
	if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected()) return;
	if (!g_Options.esp.other.kill_counter)
		return;


	int screen_sizex, screen_sizey, cW, cH;
	g_pEngine->GetScreenSize(screen_sizex, screen_sizey);

	cW = screen_sizex / 2;
	cH = screen_sizey / 2;

	float animationAlpha = 0.0;
	int alpha = 0;
	float animation_speed_value = 1.2;
	float step = 255 / 1.2 * g_pGlobalVars->frametime;

	if (g::killtime + 0.8 > g_pGlobalVars->realtime)
	{
		alpha = 255;
	}
	else
		alpha = alpha - step;

	float animate = g_pGlobalVars->curtime;

	float endTime = g::globalAlpha + 1.1f;
	float t = 1.0f - (endTime - g_pGlobalVars->curtime) / (endTime - g::globalAlpha);


	float animationStep = animation_speed_value * g_pGlobalVars->frametime;

	/*if (g::killtime + 0.1 > g_pGlobalVars->realtime)
	{
		animationAlpha -= 80;
	}
	else*/
	animationAlpha = animationAlpha - animationStep;

	int customY, customX;



	screen_sizey = (screen_sizey / 2) - (t * (60.0f));

	if (alpha > 0)
	{
		 //removed sounds cuz delay
		switch (g::kills)
		{
		case 1:
			g_pSurface->DrawT(screen_sizex / 2, screen_sizey, Color(255, 0, 0, alpha), g::Arial, true, "FIRST BLOOD");
			//PlaySoundA(first_blood, NULL, SND_ASYNC | SND_MEMORY);
			break;
		case 2:
			g_pSurface->DrawT(screen_sizex / 2, screen_sizey, Color(0, 0, 255, alpha), g::Arial, true, "DOUBLE KILL");
			//PlaySoundA(double_kill, NULL, SND_ASYNC | SND_MEMORY);
			break;
		case 3:
			g_pSurface->DrawT(screen_sizex / 2, screen_sizey, Color(0, 255, 0, alpha), g::Arial, true, "TRIPLE KILL");
			//PlaySoundA(triple_kill, NULL, SND_ASYNC | SND_MEMORY);
			break;
		case 4:
			g_pSurface->DrawT(screen_sizex / 2, screen_sizey, Color(155, 200, 0, alpha), g::Arial, true, "MULTI KILL");
			//PlaySoundA(multi_kill, NULL, SND_ASYNC | SND_MEMORY);
			break;
		case 5:
			g_pSurface->DrawT(screen_sizex / 2, screen_sizey, Color(200, 0, 167, alpha), g::Arial, true, "RAMPAGE");
			//PlaySoundA(rampage, NULL, SND_ASYNC | SND_MEMORY);
			break;
		default:
			g_pSurface->DrawT(screen_sizex / 2, screen_sizey, Color(23, 34, 255, alpha), g::Arial, true, "DOMINATING");
			//PlaySoundA(dominating, NULL, SND_ASYNC | SND_MEMORY);
			break;

		}

	}


}


void CVisuals::DrawIndicator()
{


	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player) return;
	chocked_indicators();
	int screen_width, screen_height;
	g_pEngine->GetScreenSize(screen_width, screen_height);

	int iY = 66;


	/*
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

	bool breaklagcomp = false;
	auto last_origin = local_player->GetAbsOrigin2();
	if (local_player->GetAbsOrigin2() != last_origin)
	{
		if (!(local_player->GetFlags() & FL_ONGROUND) && local_player->GetAbsOrigin2().Length2DSqr() > 4096) {
			breaklagcomp = true;
			last_origin = local_player->GetAbsOrigin2();
		}
	}

	else if (!(local_player->GetFlags() & FL_ONGROUND) && local_player->GetVelocity().Length2D() > 125 && g::bSendPacket && g_pClientState->m_nChokedCommands == LagCompBreakNEW() || g_pClientState->m_nChokedCommands == 5)
	{
		breaklagcomp = true;

	}
	else {
		breaklagcomp = false;
	}


	if (Options::Menu.RageBotTab.FakelagBreakLC.GetState() && indicators[1].GetSelected)
	{
		iY += 22;

		g_pSurface->DrawF(10, screen_height - iY, g::visuals_lby_font, false, false, breaklagcomp ? Color(0, 255, 0) : Color(255, 0, 0), "LC");

	}

	if (indicators[2].GetSelected)
	{
		float lspeed;

		lspeed = local_player->GetVelocity().Length2D();


		iY += 22;
		char jew[64];
		sprintf_s(jew, "Speed: %.1f", lspeed);
		g_pSurface->DrawF(10, screen_height - iY, g::visuals_lby_font, false, false, Color(0, 255, 0), jew);

	}*/
	/*
	if (Options::Menu.VisualsTab.LBYIndicator.GetState())
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

int getfps()
{
	return static_cast<int>(1.f / g_pGlobalVars->frametime);
}
bool IsInGame()
{
	return g_pEngine->IsConnected() && g_pEngine->IsInGame();
}

int getping()
{
	if (!IsInGame())
		return 0;

	auto nci = g_pEngine->GetNetChannelInfo();
	if (!nci)
		return 0;

	float m_AvgLatency = nci->GetAvgLatency(FLOW_INCOMING);

	return (int)(m_AvgLatency * 1000.0f);

}
int getping2()
{
	if (!IsInGame())
		return 0;

	auto nci = g_pEngine->GetNetChannelInfo();
	if (!nci)
		return 0;

	float m_AvgLatency = nci->GetAvgLatency(FLOW_OUTGOING);

	return (int)(m_AvgLatency * 1000.0f);
}
#ifdef ENABLE_XOR
#define XorStr _xor_ 
#else
#define XorStr
#endif

#define darker_gray Color(19, 19, 19)
#define lighter_gray Color(48, 48, 48)
#define light_gray Color(40, 40, 40)
#define gray Color(28, 28, 28)


void CVisuals::watermark()
{


	//	g_pSurface->DrawFilledRect(screen_width - 203, 4, screen_width - 4, 23, lighter_gray);
		//RENDER::Textf(screen_width - 187, 5, CColor(0, 0, 0, 255), FONTS::menu_window_font4, XorStr("nnware | Fps: %03d | Ping: %03d"), getfps(), getping());
}


void CVisuals::penetration_reticle()
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

	auto weapon = g::pLocalEntity->GetActiveWeapon();
	if (!weapon)
		return;


	float damage = 0.f;

	Color color = Color(255, 0, 0);
	if (g_Autowall.CanWallbang(damage))//SETTINGS::settings.damage_val)
		color = Color(163, 225, 21);

	draw_xhair(2, color);
}
bool WorldToScreen(Vector& vecOrigin, Vector& vecScreen)
{
	//return false;
	return (DebugOverlay->ScreenPosition(vecOrigin, vecScreen) != 1);
}





void CVisuals::Draw()
{

	if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !g_Options.esp.enable)
		return;

	if (!g_Options.esp.enable)
		return;
	for (int i = 1; i <= 65; i++)
	{
		auto entity = g_pEntityList->GetClientEntity(i);
		auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		if (!entity) continue;
		if (!local_player) continue;

		bool is_local_player = entity == local_player;
		bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

		if (is_local_player) continue;
		if (entity->GetHealth() <= 0) continue;
		if (entity->GetOrigin() == Vector(0, 0, 0)) continue;

		//--- Colors ---//
		if (entity->IsDormant() && flPlayerAlphaOLD[i] > 0) flPlayerAlphaOLD[i] -= .3;
		else if (!entity->IsDormant() && flPlayerAlphaOLD[i] < 255) flPlayerAlphaOLD[i] = 255;

		float playerAlpha = flPlayerAlphaOLD[i];
		int enemy_hp = entity->GetHealth();
		int hp_red = 255 - (enemy_hp * 2.55);
		int hp_green = enemy_hp * 2.55;
		Color health_color = Color(hp_red, hp_green, 1, playerAlpha);
		Color dormant_color = Color(100, 100, 100, playerAlpha);
		Color box_color, still_health, alt_color, name_color, weapon_color, skeleton_color;

		static auto alpha = 0.f; static auto plus_or_minus = false;
		if (alpha <= 0.f || alpha >= 255.f) plus_or_minus = !plus_or_minus;
		alpha += plus_or_minus ? (255.f / 7 * 0.015) : -(255.f / 7 * 0.015); alpha = clamp(alpha, 0.f, 255.f);

		if (entity->IsDormant())
		{
			main_color = dormant_color;
			still_health = health_color;
			alt_color = Color(20, 20, 20, playerAlpha);
			ammo = dormant_color;
			box_color = dormant_color;
			skeleton_color = dormant_color;
			name_color = dormant_color;
			weapon_color = dormant_color;
		}
		else if (!entity->IsDormant())
		{
            if (g_Options.misc.radar && entity->IsAlive())
            {
                static auto m_bSpotted = NetvarManager::Instance()->GetOffset("DT_BaseEntity", "m_bSpotted");

                *(char*)((DWORD)(entity)+m_bSpotted) = 1;
            }
			main_color = Color(255, 255, 255, playerAlpha);

			still_health = health_color;
			alt_color = Color(0, 0, 0, 165);
			box_color = esp::colors::enemy::box_col;
			skeleton_color = esp::colors::enemy::skeleton;

			name_color = esp::colors::enemy::name_col;
			weapon_color = esp::colors::enemy::weapons;
			ammo = Color(esp::colors::enemy::ammo);

		}
		Vector min, max, pos, pos3D, top, top3D; entity->GetRenderBounds(min, max);
		pos3D = entity->GetAbsOrigin() - Vector(0, 0, 10); top3D = pos3D + Vector(0, 0, max.z + 10);




		if (world_to_screen(pos3D, pos) && world_to_screen(top3D, top))
		{
			
			
			if (is_teammate)
			{
				if (g_Options.esp.enemy.box)
					DrawBox(entity, box_color, pos, top);
				if (g_Options.esp.enemy.name)
					DrawName(entity, name_color, i, pos, top);
				if (g_Options.esp.enemy.weapon)
					DrawWeapon(entity, weapon_color, i, pos, top);
				if (g_Options.esp.enemy.health)
					DrawHealth(entity, still_health, alt_color, pos, top);
				if (g_Options.esp.enemy.ammo)
					DrawAmmo(entity, ammo, alt_color, pos, top);

			}


			

		}
		if (!is_teammate)
		{
		
			//if (Options::Menu.AntiAimTab.aaenabel.GetState())
			//	DrawLinesAA();
		}
	}

}

bool Transfo23rmScreen(const Vector& in, Vector& out)
{
	
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


void CVisuals::RenderSkeleton(C_BaseEntity* pEnt, Color color) // the best
{
	
	
	studiohdr_t* pStudioHdr = g_pModelInfo->GetStudiomodel(pEnt->GetModel());
	if (!pStudioHdr)
		return;
	Vector vParent, vChild, sParent, sChild;
	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);
		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEnt->GetBonePos(j);
			vParent = pEnt->GetBonePos(pBone->parent);
			if (Transfo23rmScreen(vParent, sParent) && Transfo23rmScreen(vChild, sChild))
			{
				g_pSurface->Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(color[0], color[1], color[2], color[3]));
			}
		}
	}

}

#define   CONTENTS_SOLID                0x1       
#define   CONTENTS_MOVEABLE             0x4000
#define   CONTENTS_WINDOW               0x2
#define   CONTENTS_DEBRIS               0x4000000

#define   MASK_SHOT_BRUSHONLY           (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)

void CVisuals::DrawFeetRange()
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

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

		if (!prevScreenPos.IsZero() && !screenPos.IsZero() && screenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f&& prevScreenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f) {
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

void CVisuals::DrawZeusRange()
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

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

			if (!prevScreenPos.IsZero() && !screenPos.IsZero() && screenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f&& prevScreenPos.DistTo(Vector(-107374176, -107374176, -107374176)) > 3.f) {
				rainbow += 0.00001;
				if (rainbow > 1.f)
					rainbow = 0;

				Color color = esp::colors::zeus_range;
				g_pSurface->Line(prevScreenPos.x, prevScreenPos.y, screenPos.x, screenPos.y, color);
				//	DrawLine(prevScreenPos.x, prevScreenPos.y + 1, screenPos.x, screenPos.y + 1, color);
				//	DrawLine(Vector2D(prevScreenPos.x, prevScreenPos.y - 1), Vector2D(screenPos.x, screenPos.y - 1), color);
			}

			prevScreenPos = screenPos;
		}

	}

}

char* GetWeaponString(C_BaseCombatWeapon *pWeapon)
{
	int ID = *pWeapon->m_iItemDefinitionIndex();

	switch (ID) {
	case 4:
		return "Glock";
	case 2:
		return "Elite";
	case 36:
		return "P250";
	case 30:
		return "Tec9";
	case 1:
		return "Deagle";
	case 32:
		return "P2000";
	case 3:
		return "FiveSeven";
	case 64:
		return "Revolver";
	case 63:
		return "CZ75 Auto";
	case 61:
		return "USP";
	case 35:
		return "Nova";
	case 25:
		return "XM1014";
	case 29:
		return "Sawed Off";
	case 14:
		return "M249";
	case 28:
		return "Negev";
	case 27:
		return "Mag7";
	case 17:
		return "Mac10";
	case 33:
		return "MP7";
	case 23:
		return "MP5";
	case 24:
		return "UMP-45";
	case 19:
		return "P90";
	case 26:
		return "Bizon";
	case 34:
		return "MP9";
	case 10:
		return "Famas";
	case 16:
		return "M4A4";
	case 40:
		return "SSG08";
	case 8:
		return "Aug";
	case 9:
		return "AWP";
	case 38:
		return "SCAR20";
	case 13:
		return "Galil";
	case 7:
		return "Ak47";
	case 39:
		return "SG553";
	case 11:
		return "G3SG1";
	case 60:
		return "M4A1-S";
	case 46:
		return "INC";
	case 48:
		return "Molotov";
	case 44:
		return "Grenade";
	case 43:
		return "Flashbang";
	case 45:
		return "Smoke";
	case 47:
		return "Decoy";
	case 31:
		return "Taser";
	default:
		return "Knife";
	}
}



const char* GetGunIcon(C_BaseCombatWeapon *pWeapon)
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


void CVisuals::DrawWeapon(C_BaseEntity* entity, Color color, int index, Vector pos, Vector top)
{
	PlayerInfo_t ent_info;
	g_pEngine->GetPlayerInfo(index, &ent_info);

	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player) return;

	auto weapon = entity->GetActiveWeapon();
	//auto weapon = reinterpret_cast<C_BaseCombatWeapon*>(g_pEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));

	if (!weapon) return;



	bool is_teammate = local_player->GetTeam() == entity->GetTeam(), distanceThing, distanceThing2;

	if (g_Options.esp.enemy.ammo)
		distanceThing = true;
	else
		distanceThing = false;
	if (g_Options.esp.enemy.ammo)
		distanceThing2 = true;
	else
		distanceThing2 = false;

	int height = (pos.y - top.y), width = height / 2, distanceOn = distanceThing ? pos.y + 14 : pos.y + 10, distanceOn2 = distanceThing2 ? pos.y + 14 : pos.y + 10;

	if (g_Options.esp.enemy.weapon_type == 0)
	{
		std::string weapon_name = GetWeaponString(weapon);


		g_pSurface->DrawF(pos.x, is_teammate ? distanceOn2 : distanceOn, g::pixelfont, true, true, color, weapon_name.c_str());

	}
	if (g_Options.esp.enemy.weapon_type == 1)
	{
		std::string weapon_name = GetGunIcon(weapon);


		g_pSurface->DrawF(pos.x, is_teammate ? distanceOn2 : distanceOn, g::Untitled1, true, true, color, weapon_name.c_str());

	}


}

void CVisuals::DrawAmmo(C_BaseEntity* entity, Color color, Color dormant, Vector pos, Vector top)
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player) return;
	auto c_baseweapon = entity->GetActiveWeapon();

	if (!c_baseweapon) return;

	int height = (pos.y - top.y);

	float offset = (height / 4.f) + 5;
	UINT hp = height - (UINT)((height * 3) / 100);




	int iClip = c_baseweapon->GetAmmo();
	int iClipMax = c_baseweapon->GetCSWpnData()->max_clip;

	float box_w = (float)fabs(height / 2);
	float width;

	width = (((box_w * iClip) / iClipMax));

	g_pSurface->DrawFilledRect((pos.x - box_w / 2), top.y + height + 3, (pos.x - box_w / 2) + box_w + 2, top.y + height + 7, dormant); //outline
	g_pSurface->DrawFilledRect((pos.x - box_w / 2) + 1, top.y + height + 4, (pos.x - box_w / 2) + width + 1, top.y + height + 6, color); //ammo

}


void CVisuals::DrawHealth(C_BaseEntity* entity, Color color, Color dormant, Vector pos, Vector top)
{

	int enemy_hp = entity->GetHealth(),
		hp_red = 255 - (enemy_hp * 2.55),
		hp_green = enemy_hp * 2.55,
		height = (pos.y - top.y),
		width = height / 2;

	float offset = (height / 4.f) + 5;
	UINT hp = height - (UINT)((height * enemy_hp) / 100);

	g_pSurface->DrawEmptyRect((pos.x - width / 2) - 6, top.y, (pos.x - width / 2) - 3, top.y + height, dormant);
	g_pSurface->line((pos.x - width / 2) - 4, top.y + hp, (pos.x - width / 2) - 4, top.y + height, color);
	g_pSurface->line((pos.x - width / 2) - 5, top.y + hp, (pos.x - width / 2) - 5, top.y + height, color);

	//g_pSurface->DrawF((pos.x - width / 2) - 14, top.y + hp, g::pixelfont, true, true, main_color, std::to_string(enemy_hp));
}

void CVisuals::DrawBox(C_BaseEntity* entity, Color color, Vector pos, Vector top)
{
	float alpha = flPlayerAlphaOLD[entity->EntIndex()];
	int height = (pos.y - top.y), width = height / 2;

	g_pSurface->DrawEmptyRect(pos.x - width / 2, top.y, (pos.x - width / 2) + width, top.y + height, color);
	g_pSurface->DrawEmptyRect((pos.x - width / 2) + 1, top.y + 1, (pos.x - width / 2) + width - 1, top.y + height - 1, Color(20, 20, 20, alpha));
	g_pSurface->DrawEmptyRect((pos.x - width / 2) - 1, top.y - 1, (pos.x - width / 2) + width + 1, top.y + height + 1, Color(20, 20, 20, alpha));



}
void CVisuals::DrawName(C_BaseEntity* entity, Color color, int index, Vector pos, Vector top)
{
	PlayerInfo_t ent_info;
	g_pEngine->GetPlayerInfo(index, &ent_info);

	int height = (pos.y - top.y), width = height / 2;
	g_pSurface->DrawF(pos.x, top.y - 7, g::CourierNew, true, true, color, ent_info.szName);
	//g_pSurface->DrawF(g::Tahoma, box.x + (box.w / 2), box.y - 13, color, HFONT_CENTERED_X, name.c_str());

	//	g::Tahoma
}


CVisuals* visuals = new CVisuals();

/*old visuals
ESP g_ESP;

void ESP::paint_traverse()
{

	if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !Options::Menu.VisualsTab.esp_bool.GetState())
		return;

	for (int i = 1; i < g_pGlobalVars->maxClients; i++)
	{
		auto e = static_cast<C_BaseEntity *>(g_pEntityList->GetClientEntity(i));

		if (!e->valid(true, !Options::Menu.VisualsTab.esp_bool.GetState()))
			continue;

		if (Options::Menu.VisualsTab.esp_bool.GetState())
		{
			int idx = e->EntIndex();
			float in = (1.f / 0.2f) *g_pGlobalVars->frametime;
			float out = (1.f / 2.f) * g_pGlobalVars->frametime;

			if (!e->IsDormant()) {
				if (esp_alpha_fade[idx] < 1.f)
					esp_alpha_fade[idx] += in;
			}
			else {
				if (esp_alpha_fade[idx] > 0.f)
					esp_alpha_fade[idx] -= out;
			}
			esp_alpha_fade[idx] = (esp_alpha_fade[idx] > 1.f ? 1.f : esp_alpha_fade[idx] < 0.f ? 0.f : esp_alpha_fade[idx]);
		}


		Box box; if (get_bbox(e, box))
		{
			draw_box(e, box);
			draw_name(e, box);
			draw_health(e, box);
			draw_ammobar(e, box);// fix this
			draw_weapon(e, box);


			////draw_flags(e, box);
			//draw_weapon(e, box);
			//draw_lbybar(e, box);

			//if (g_cfg.player.skeleton[SKELETON_NORMAL].enabled)
			//{
			//	Color color = e->IsDormant() ? Color(130, 130, 130, 130) : Color::White;
			//	color.SetAlpha(g_cfg.player.alpha  * esp_alpha_fade[e->EntIndex()]);
//
			//	matrix3x4_t matrix[128];
//
			//	if (e->SetupBones(matrix, 128, 0x100, 0.f))
				//	draw_skeleton(e, color, matrix);
			//}


		}
	}
}

void ESP::draw_weapon(C_BaseEntity* m_entity, Box box)
{
	PlayerInfo_t player_info;
	g_pEngine->GetPlayerInfo(m_entity->EntIndex(), &player_info);

	if (!Options::Menu.VisualsTab.esp_weapon.GetState())
		return;

	int bottom_pos = 0;

	auto weapon = m_entity->GetActiveWeapon();

	int _y;

	if (!weapon)
		return;

	bool drawing_ammo_bar = g_Options.esp.enemy.ammo;


	_y = box.y + box.h + (drawing_ammo_bar ? 10 : 5);

	if (weapon->is_non_aim())
		drawing_ammo_bar = false;

	auto color = m_entity->IsDormant() ? Color(130, 130, 130, 130) : Color(Options::Menu.VisualsTab.esp_esp_weapon_col.GetValue());

	g_pSurface->text(g::Tahoma, box.x + (box.w / 2), _y, color, HFONT_CENTERED_X, weapon->get_name());
}


void ESP::draw_health(C_BaseEntity* m_entity, Box box)
{
	if (!Options::Menu.VisualsTab.esp_player_health.GetState())
		return;

	int player_health = m_entity->GetHealth() > 100 ? 100 : m_entity->GetHealth();
	if (!player_health)
		return;

	int
		alpha = 255 * esp_alpha_fade[m_entity->EntIndex()],
		outline_alpha = alpha * 0.6f,
		battery_alpha = alpha * 0.9f,
		inner_back_alpha = alpha * 0.6f;
	Color
		outline_color{ 0, 0, 0, outline_alpha },
		inner_back_color{ 0, 0, 0, inner_back_alpha },
		battery_color{ 0, 0, 0, inner_back_alpha },
		text_color = m_entity->IsDormant() ? Color(130, 130, 130, alpha) : Color(255, 255, 255, alpha),
		color = m_entity->IsDormant() ? Color(130, 130, 130, 130) : Color(153, min(255, m_entity->GetHealth() * 225 / 100), 0, alpha);


	g_pSurface->rect(box.x - 6, box.y, 4, box.h, outline_color);

	Box n_box = {
		box.x - 5,
		box.y + 1,
		2,
		box.h - 2
	};

	float
		fl_height = n_box.h / 10.f;

	int
		bar_height = player_health * n_box.h / 100.0f,
		offset = n_box.h - bar_height;

	g_pSurface->rect_filled(n_box.x, n_box.y, 2, n_box.h, inner_back_color);
	g_pSurface->rect_filled(n_box.x, n_box.y + offset, 2, bar_height, color);

	for (int i = 0; i < 10; i++)
		g_pSurface->line(n_box.x, n_box.y + i * fl_height, n_box.x + 2, n_box.y + i * fl_height, battery_color);

	if (player_health < 100)
	{
		g_pSurface->text(g::Tahoma,
			n_box.x + 1, n_box.y + offset,
			text_color, HFONT_CENTERED_X | HFONT_CENTERED_Y, std::to_string(player_health).c_str()
		);
	}
}

void ESP::draw_ammobar(C_BaseEntity* m_entity, Box box)
{
	if (!g_Options.esp.enemy.ammo)
		return;

	auto weapon = m_entity->GetActiveWeapon();

	if (!weapon)
		return;

	if (weapon->is_non_aim())
		return;

	int ammo = weapon->GetAmmo();

	int
		alpha = 255 * esp_alpha_fade[m_entity->EntIndex()],
		outline_alpha = alpha * 0.7f,
		inner_back_alpha = alpha * 0.6f;

	Color
		outline_color{ 0, 0, 0, outline_alpha },
		inner_back_color{ 0, 0, 0, inner_back_alpha },
		text_color = m_entity->IsDormant() ? Color(130, 130, 130, alpha) : Color(255, 255, 255, alpha),
		color = m_entity->IsDormant() ? Color(130, 130, 130, 130) : Color(Options::Menu.VisualsTab.esp_player_ammo_col.GetValue());

	Box n_box = {
		box.x + 1,
		box.y + box.h + 3,
		box.w - 1,
		2
	};

	float
		bar_width = ammo * box.w / weapon->GetCSWpnData()->max_clip;


	g_pSurface->rect_filled(n_box.x - 1, n_box.y - 1, box.w + 2, 4, inner_back_color);
	g_pSurface->rect_filled(n_box.x, n_box.y, bar_width, 2, color);

	if (weapon->GetAmmo() != weapon->GetCSWpnData()->max_clip)
		g_pSurface->text(g::Tahoma,
			n_box.x + bar_width, n_box.y + 1,
			text_color, HFONT_CENTERED_X | HFONT_CENTERED_Y, std::to_string(ammo).c_str()
		);
}

void ESP::draw_box(C_BaseEntity* m_entity, Box box)
{
	if (!Options::Menu.VisualsTab.esp_player_boxes.GetState())
		return;

	int
		alpha = 255 * esp_alpha_fade[m_entity->EntIndex()],
		outline_alpha = alpha * 0.6f;

	Color
		outline_color{ 0, 0, 0, outline_alpha },
		color = m_entity->IsDormant() ? Color(130, 130, 130, 130) : Color(Options::Menu.VisualsTab.esp_player_boxes_col.GetValue());


	g_pSurface->rect(box.x, box.y, box.w, box.h, color);
	g_pSurface->rect(box.x - 1, box.y - 1, box.w + 2, box.h + 2, outline_color);
	g_pSurface->rect(box.x + 1, box.y + 1, box.w - 2, box.h - 2, outline_color);


}

void ESP::draw_name(C_BaseEntity* m_entity, Box box)
{
	static auto sanitize = [](char * name) -> std::string {
		name[127] = '\0';

		std::string tmp(name);

		if (tmp.length() > 20) {
			tmp.erase(20, (tmp.length() - 20));
			tmp.append("...");
		}

		return tmp;
	};

	if (!Options::Menu.VisualsTab.esp_player_names.GetState())
		return;

	PlayerInfo_t player_info;
	if (g_pEngine->GetPlayerInfo(m_entity->EntIndex(), &player_info))
	{
		std::string name = sanitize(player_info.szName);

		auto color = m_entity->IsDormant() ? Color(130, 130, 130, 130) : Color(Options::Menu.VisualsTab.esp_player_names_col.GetValue());

		std::transform(name.begin(), name.end(), name.begin(), tolower);

		g_pSurface->text(g::Tahoma, box.x + (box.w / 2), box.y - 13, color, HFONT_CENTERED_X, name.c_str());
	}
}


RECT ESP::viewport()
{
	RECT viewport = { 0, 0, 0, 0 };

	int width, height;
	g_pEngine->GetScreenSize(width, height);
	viewport.right = width;
	viewport.bottom = height;

	return viewport;
}

bool ESP::world_to_screen(const Vector &origin, Vector &screen)
{
	auto find_point = [](Vector &point, int screen_w, int screen_h, int degrees) -> void {
		float x2 = screen_w * 0.5f;
		float y2 = screen_h * 0.5f;

		float d = sqrt(pow((point.x - x2), 2) + (pow((point.y - y2), 2))); //Distance
		float r = degrees / d; //Segment ratio

		point.x = r * point.x + (1 - r) * x2; //find point that divides the segment
		point.y = r * point.y + (1 - r) * y2; //into the ratio (1-r):r
	};

	float w = g::w2s_matrix[3][0] * origin.x + g::w2s_matrix[3][1] * origin.y + g::w2s_matrix[3][2] * origin.z + g::w2s_matrix[3][3];

	auto
		screen_width = viewport().right,
		screen_height = viewport().bottom;

	float inverse_width = -1.0f / w;
	bool behind = true;

	if (w > 0.01) {
		inverse_width = 1.0f / w;
		behind = false;
	}

	screen.x = (float)((screen_width / 2) + (0.5 * ((g::w2s_matrix[0][0] * origin.x
		+ g::w2s_matrix[0][1] * origin.y
		+ g::w2s_matrix[0][2] * origin.z
		+ g::w2s_matrix[0][3]) * inverse_width) * screen_width + 0.5));

	screen.y = (float)((screen_height / 2) - (0.5 * ((g::w2s_matrix[1][0] * origin.x
		+ g::w2s_matrix[1][1] * origin.y
		+ g::w2s_matrix[1][2] * origin.z
		+ g::w2s_matrix[1][3]) * inverse_width) * screen_height + 0.5));

	if (screen.x > screen_width || screen.x < 0 || screen.y > screen_height || screen.y < 0 || behind) {
		find_point(screen, screen_width, screen_height, screen_height / 2);
		return false;
	}

	return !(behind);
}


bool ESP::get_bbox(C_BaseEntity * e, Box& box)
{

	vec3_t top, down, air, s[2];

	vec3_t adjust = vec3_t(0, 0, -18) * e->m_flDuckAmount();

	if (!(e->GetFlags() & FL_ONGROUND) && (e->get_move_type() != 9))
		air = vec3_t(0, 0, 10);
	else
		air = vec3_t(0, 0, 0);

	down = e->GetOrigin() + air;
	top = down + vec3_t(0, 0, 72) + adjust;

	if (world_to_screen(top, s[1]) && world_to_screen(down, s[0])) {
		vec3_t delta = s[1] - s[0];

		box.h = fabsf(delta.y);
		box.w = box.h / 2.0f;

		box.x = s[1].x - (box.w / 2);
		box.y = s[1].y;

		return true;
	}

	return false;
}
*/


