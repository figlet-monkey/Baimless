#include "otheresp.h"
#include <d3d9.h>
#include "../../Utils/GlobalVars.h"
#include <d3dx9math.h>
#include "../../newMenu.h"
#include "../../Options.hpp"
void circle_dual_colour(float x, float y, float rad, float rotate, int type, int resolution, IDirect3DDevice9* m_device)
{
	/*
	LPDIRECT3DVERTEXBUFFER9 g_pVB2;

	std::vector<CUSTOMVERTEX> circle(resolution + 2);

	float angle = rotate * D3DX_PI / 180, pi = D3DX_PI;

	if (type == 1)
		pi = D3DX_PI; // Full circle
	if (type == 2)
		pi = D3DX_PI / 2; // 1/2 circle
	if (type == 3)
		pi = D3DX_PI / 4; // 1/4 circle

	pi = D3DX_PI / type; // 1/4 circle

	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = D3DCOLOR_RGBA(0, 0, 0, 0);

	float hue = 0.f;

	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - rad * cos(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - rad * sin(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;

		auto clr = Color::FromHSB(hue, 1.f, 1.f);
		circle[i].color = D3DCOLOR_RGBA(clr);
		hue += 0.02;
	}

	// Rotate matrix
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		float Vx1 = x + (cosf(angle) * (circle[i].x - x) - sinf(angle) * (circle[i].y - y));
		float Vy1 = y + (sinf(angle) * (circle[i].x - x) + cosf(angle) * (circle[i].y - y));

		circle[i].x = Vx1;
		circle[i].y = Vy1;
	}

	m_device->CreateVertexBuffer((resolution + 2) * sizeof(CUSTOMVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL);

	VOID* pVertices;
	g_pVB2->Lock(0, (resolution + 2) * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(CUSTOMVERTEX));
	g_pVB2->Unlock();

	m_device->SetTexture(0, NULL);
	m_device->SetPixelShader(NULL);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_device->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
	if (g_pVB2 != NULL)
		g_pVB2->Release(); */
}


void circle_dual_colour(float x, float y, float rad, float rotate, int type, int resolution, DWORD color, DWORD color2, IDirect3DDevice9* m_device)
{
	LPDIRECT3DVERTEXBUFFER9 g_pVB2;

	std::vector<CUSTOMVERTEX> circle(resolution + 2);

	float angle = rotate * D3DX_PI / 180, pi = D3DX_PI;

	if (type == 1)
		pi = D3DX_PI; // Full circle
	if (type == 2)
		pi = D3DX_PI / 2; // 1/2 circle
	if (type == 3)
		pi = D3DX_PI / 4; // 1/4 circle

	pi = D3DX_PI / type; // 1/4 circle

	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = color2;

	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - rad * cos(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - rad * sin(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;
		circle[i].color = color;
	}

	// Rotate matrix
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		circle[i].x = x + cos(angle)*(circle[i].x - x) - sin(angle)*(circle[i].y - y);
		circle[i].y = y + sin(angle)*(circle[i].x - x) + cos(angle)*(circle[i].y - y);
	}

	m_device->CreateVertexBuffer((resolution + 2) * sizeof(CUSTOMVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL);

	VOID* pVertices;
	g_pVB2->Lock(0, (resolution + 2) * sizeof(CUSTOMVERTEX), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(CUSTOMVERTEX));
	g_pVB2->Unlock();

	m_device->SetTexture(0, NULL);
	m_device->SetPixelShader(NULL);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_device->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));
	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
	if (g_pVB2 != NULL)
		g_pVB2->Release();
}

#include "../../SDK/ICvar.h"
void otheresp::spread_crosshair(IDirect3DDevice9* m_device)
{
	/*
	if (Shonax.Visuals.show_spread)
		g_pCvar->FindVar("crosshair")->SetValue(0);
	else
		g_pCvar->FindVar("crosshair")->SetValue(1);


	if (!Shonax.Visuals.show_spread)
		return;



	if (!g::pLocalEntity)
		return;

	if (!g::pLocalEntity->IsAlive())
		return;

	if (!g_pEngine->IsConnected() && !g_pEngine->IsInGame())
		return;

	auto weapon = g::pLocalEntity->GetActiveWeapon();

	if (!weapon)
		return;

	//if (weapon->is_grenade()) return;

	static float rot = 0.f;

	int w, h;

	g_pEngine->GetScreenSize(w, h);

	w /= 2, h /= 2;

	int r, g, b;
	r = Shonax.Visuals.show_spread_color[0] * 255;
	g = Shonax.Visuals.show_spread_color[1] * 255;
	b = Shonax.Visuals.show_spread_color[2] * 255;

	switch (Shonax.Visuals.show_spread_type) {
	case 0:
		circle_dual_colour(w, h, weapon->GetInaccuracy() * 500.0f, 0, 1, 50, D3DCOLOR_RGBA(r, g, b, 100), D3DCOLOR_RGBA(0, 0, 0, 0), m_device);
		break;
	case 1:
		circle_dual_colour(w, h, weapon->GetInaccuracy() * 500.0f, rot, 1, 50, m_device);
		break;
	}

	rot += 0.5f;
	if (rot > 360.f)
		rot = 0.f; */
}









#include "../../SDK/IGameEvent.h"
#include "../../SDK/CGlobalVarsBase.h"
#include "../../SDK/ISurface.h"
C_BaseEntity* get_player(int userid) {
	int i = g_pEngine->GetPlayerForUserID(userid);
	return static_cast< C_BaseEntity * >(g_pEntityList->GetClientEntity(i));
}
std::vector<impact_info> impacts;
std::vector<hitmarker_info> hitmarkers;


void otheresp::hitmarker_event(IGameEvent * event) {

	if (!event) return;
	if (!strcmp(event->GetName(), "player_hurt")) {

		auto attacker = event->GetInt("attacker");
		auto victim = get_player(event->GetInt("userid"));
		if (!victim)
			return;

		if (victim->GetOrigin() == Vector(0, 0, 0))
			return;

		if (g_pEngine->GetPlayerForUserID(attacker) == g_pEngine->GetLocalPlayer())
		{

		
			hurt_time = g_pGlobalVars->curtime;
			impact_info best_impact;
			float best_impact_distance = -1;
			float time = g_pGlobalVars->curtime;


			for (int i = 0; i < impacts.size(); i++) {
				auto iter = impacts[i];
				if (time > iter.time + 1.f) {
					impacts.erase(impacts.begin() + i);
					continue;
				}
				Vector position = Vector(iter.x, iter.y, iter.z);
				Vector enemy_pos = victim->GetOrigin();
				float distance = position.DistTo(enemy_pos);
				if (distance < best_impact_distance || best_impact_distance == -1) {
					best_impact_distance = distance;
					best_impact = iter;
				}
			}

			if (best_impact_distance == -1)
				return;


			hitmarker_info info;
			info.impact = best_impact;
			info.alpha = 255;
			hitmarkers.push_back(info);
		}
	}

	if (strstr(event->GetName(), "bullet_impact"))
	{
		auto entity = get_player(event->GetInt("userid"));
		if (!entity || entity != g::pLocalEntity)
			return;

		impact_info info;
		info.x = event->GetFloat("x");
		info.y = event->GetFloat("y");
		info.z = event->GetFloat("z");

		info.time = g_pGlobalVars->curtime;

		impacts.push_back(info);
	}

}

RECT viewport() {
	RECT viewport = { 0, 0, 0, 0 };

	int width, height;
	g_pEngine->GetScreenSize(width, height);
	viewport.right = width;
	viewport.bottom = height;

	return viewport;
}

bool world_to_screen(const Vector &origin, Vector &screen) {
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


template <typename t> t clamp_value(t value, t min, t max) {
	if (value > max) {
		return max;
	}
	if (value < min) {
		return min;
	}
	return value;
}


void otheresp::hitmarkerdynamic_paint()
{

	

	/////////
	float time = g_pGlobalVars->curtime;

	for (int i = 0; i < hitmarkers.size(); i++)
	{
		bool expired = time >= hitmarkers.at(i).impact.time + 3.f;

		if (expired)
			hitmarkers.at(i).alpha -= 1;

		//if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected())
		//	hitmarkers.erase(hitmarkers.begin() + i);
		//	continue;

		if (expired && hitmarkers.at(i).alpha <= 0)
		{
			hitmarkers.erase(hitmarkers.begin() + i);
			continue;
		}

		Vector pos3D = Vector(hitmarkers.at(i).impact.x, hitmarkers.at(i).impact.y, hitmarkers.at(i).impact.z), pos2D;
		if (!world_to_screen(pos3D, pos2D))
			continue;

		auto linesize = 8;
		g_pSurface->DrawSetColor(Color(legit::colors::aimpoint_col[0], legit::colors::aimpoint_col[1], legit::colors::aimpoint_col[2], hitmarkers.at(i).alpha));

		g_pSurface->DrawOutlinedCircle(pos2D.x, pos2D.y, 8, 50); // pos2D.x - (linesize / 4), pos2D.y - (linesize / 4)
		g_pSurface->DrawLine(pos2D.x - 6, pos2D.y - 6, pos2D.x - (3 / 4), pos2D.y - (3 / 4));
		g_pSurface->DrawLine(pos2D.x - 6, pos2D.y + 6, pos2D.x - (3 / 4), pos2D.y + (3 / 4));
		g_pSurface->DrawLine(pos2D.x + 6, pos2D.y + 6, pos2D.x + (3 / 4), pos2D.y + (3 / 4));
		g_pSurface->DrawLine(pos2D.x + 6, pos2D.y - 6, pos2D.x + (3 / 4), pos2D.y - (3 / 4));
	}
}

void otheresp::hitmarker_paint() {
	auto linesize = 8;
	int sizeX, centerX;
	int sizeY, centerY;

	if (!g::pLocalEntity)
		return;
	if (!g::pLocalEntity->m_flHealthShotBoostExpirationTime())
		return;

	if (!g::pLocalEntity->IsAlive())
		hurt_time = 0.f;
	if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected())
		hurt_time = 0.f;

	if (hurt_time + 0.65f >= g_pGlobalVars->curtime)
	{

		if (g_Options.esp.other.hit_effect)
			*g::pLocalEntity->m_flHealthShotBoostExpirationTime() = g_pGlobalVars->curtime + 2.5f;

		if (g_Options.esp.other.hitmarker)
		{
			g_pEngine->GetScreenSize(sizeX, sizeY);
			linesize -= static_cast<int>(static_cast<float>(((hurt_time + 0.65f) - g_pGlobalVars->curtime) / 1.2) * 45);

			centerX = sizeX / 2;
			centerY = sizeY / 2;

			clamp_value(linesize, 0, 8);

			g_pSurface->DrawSetColor(255, 255, 255, 255);
			g_pSurface->DrawLine(centerX - linesize, centerY - linesize, centerX - (linesize / 4), centerY - (linesize / 4));
			g_pSurface->DrawLine(centerX - linesize, centerY + linesize, centerX - (linesize / 4), centerY + (linesize / 4));
			g_pSurface->DrawLine(centerX + linesize, centerY + linesize, centerX + (linesize / 4), centerY + (linesize / 4));
			g_pSurface->DrawLine(centerX + linesize, centerY - linesize, centerX + (linesize / 4), centerY - (linesize / 4));
		}
		
	}
}
