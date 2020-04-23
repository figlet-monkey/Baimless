#include "bullettracers.h"
#include "..\..\Utils\Utils.h"
#include "..\..\SDK\IVEngineClient.h"
#include "..\..\SDK\PlayerInfo.h"
#include "..\..\SDK\ICvar.h"
#include "../../SDK/IGameEvent.h"
#include "../../SDK/CGlobalVarsBase.h"
#include "BeamInfo.hpp"
#include "../../Utils/GlobalVars.h"
#include "../../SDK/IVRenderBeam.h"
#include "..\..\newMenu.h"
#include "../../SDK\IVModelInfo.h"
#include "../../Options.hpp"

void bullettracers::draw_beam(Vector src, Vector end, Color coli)
{
	if (g_Options.esp.effects.beam)
	{
		float time = g_Options.esp.effects.beam_life;
		float size = g_Options.esp.effects.beam_size;
		
		BeamInfo_t beamInfo;
		beamInfo.m_nType = TE_BEAMPOINTS;
		beamInfo.m_pszModelName = "sprites/purplelaser1.vmt";
		beamInfo.m_nModelIndex = -1;
		beamInfo.m_flHaloScale = 0.0f;
		beamInfo.m_flLife = time;
		beamInfo.m_flWidth = size;
		beamInfo.m_flEndWidth = size;
		beamInfo.m_flFadeLength = 0.0f;
		beamInfo.m_flAmplitude = 2.0f;
		beamInfo.m_flBrightness = coli.a();
		beamInfo.m_flSpeed = 0.2f;
		beamInfo.m_nStartFrame = 0.f;
		beamInfo.m_flFrameRate = 0.f;
		beamInfo.m_flRed = coli.r();
		beamInfo.m_flGreen = coli.g();
		beamInfo.m_flBlue = coli.b();
		beamInfo.m_nSegments = 2;
		beamInfo.m_bRenderable = true;
		beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;
		beamInfo.m_vecStart = src;
		beamInfo.m_vecEnd = end;

		Beam_t* beam = g_pIViewRenderBeams->CreateBeamPoints(beamInfo);

		if (beam)
			g_pIViewRenderBeams->DrawBeam(beam);

	}

	
}


void bullettracers::draw_impact(Vector end)
{


	Color color_ = Color(esp::colors::impacts);

	if (g_Options.esp.effects.impacts)
	{
		DebugOverlay->AddBoxOverlay(end, Vector(-3, -3, -3), Vector(3, 3, 3), Vector(0, 0, 0), color_.r(), color_.g(), color_.b(), color_.a(), 0.8f);

	}

}


void bullettracers::events(IGameEvent * event)
{
	if (!strcmp(event->GetName(), "bullet_impact")) 
	{

		auto index = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("userid")));
		if (!index) //IF they not a entity
			return;

		Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
		auto list = g_Options.esp.effects.beam_selected;

		//g_pEngine->ExecuteClientCmd("say hello");
		auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

		if (g_Options.esp.effects.beam || g_Options.esp.effects.impacts)
		{
			if (index == local_player)
			{
				if (list[0])
				{

					logs.push_back(trace_info(g::pLocalEntity->get_eye_pos(), position, g_pGlobalVars->curtime));
					draw_beam(g::pLocalEntity->get_eye_pos(), position, Color(esp::colors::beam_color));

				}
				
				draw_impact(position);
			}
			else
			{
				if (index == local_player)
					return;

				if (index->GetTeam() == g::pLocalEntity->GetTeam()) //Fuck teammates
					return;


				
				if (list[1])
				{
					logs.push_back(trace_info(index->get_eye_pos(), position, g_pGlobalVars->curtime));
					draw_beam(index->get_eye_pos(), position, Color(esp::colors::beam_color));
				}
				

			}
		}
		
	}
}
