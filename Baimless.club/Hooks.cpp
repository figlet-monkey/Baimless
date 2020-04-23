#include <thread>
#include "Hooks.h"
#include "Utils\Utils.h"
#include "Features\Features.h"
#include "SDK\Hitboxes.h"
#include "libloaderapi.h"
#include "Features/rankchanger/rankchanger.h"
#include "Fonts.h"
#include "skinchanger.h"
#include "Features/rankchanger/Protobuffs.h"
#include "Features/rankchanger/IMemAlloc.h"
#include "Features/rankchanger/steam.h"
#include "SDK/IMDLCache.h"
#include <intrin.h>
#include "newMenu.h"
#include "hookknife.h"
#include "uselesshitbytes.h"
#include "grenadetracer.h"
#include "playsoundapi.h"
#include "Singleton.h"
#include "Playerlist.h"
#include "CFixMove.h"
#include "algorithm"
#include "detourhooks.h"
#include "../Visuals.h"
#include "../Drops+.h"
#include "../string_obfuscation.h"
#include "Render.h"
#include "Menu.h"
#include "Utils/Interfaces.h"
#include "../Input.h"
#include "Entities.h"
#include "../Baimless.club/fnv_hash.hpp"
#include "notifies.h"
//Reminder fix
//Third person - fixed
//Resolver - fixed
//Double tap hook - votrix adds later dont touch
// weird imgui hook



//CPlist plist = CPlist();
double PlistArray[102][200];
static CFixMove *FixMoveManager = new CFixMove();

/////

bool overridething;
bool   g_Unload = false;
bool overridething_legit;
bool overridething_dt_key;


#include <d3dx9core.h>
#include <array>


#include <string.h>
#include <process.h>

//
#define FCVAR_CHEAT                     (1<<14) // Only useable in singleplayer / debug / multiplayer & sv_cheats

Protobuffs write;
C_HookedEvents hooked_events;
Misc     g_Misc;
Hooks    g_Hooks;

static char ConfigNamexd[64] = { 0 };





#include "vfunct/vfuncthook.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "imgui/imgui.h"
#include "imgui/impl/imgui_impl_win32.h"
#include "Newskinchanger.h"
#include <Options.hpp>


vfunc_hook gc_hook;
vfunc_hook reveal;
vfunc_hook m_pEngineSwap;
vfunc_hook clientMode;
vfunc_hook m_pNetChannelSwap;
int m_nTickbaseShift = 0;

bool bInSendMove = false, bFirstSendMovePack = false;


extern DWORD WriteUsercmdDeltaToBufferReturn;


EGCResultxd __fastcall hkGCRetrieveMessage(void* ecx, void*, uint32_t *punMsgType, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize)
{
	static auto oGCRetrieveMessage = gc_hook.get_original<Hooks::GCRetrieveMessage>(vtable_indexes::retrieve_message);
	auto status = oGCRetrieveMessage(ecx, punMsgType, pubDest, cubDest, pcubMsgSize);

	if (status == k_EGCResultOK)
	{

		void* thisPtr = nullptr;
		__asm mov thisPtr, ebx;
		auto oldEBP = *reinterpret_cast<void**>((uint32_t)_AddressOfReturnAddress() - 4);

		uint32_t messageType = *punMsgType & 0x7FFFFFFF;
		write.ReceiveMessage(thisPtr, oldEBP, messageType, pubDest, cubDest, pcubMsgSize);


	}


	return status;
}

EGCResultxd __fastcall hkGCSendMessage(void* ecx, void*, uint32_t unMsgType, const void* pubData, uint32_t cubData)
{
	static auto oGCSendMessage = gc_hook.get_original<Hooks::GCSendMessage>(vtable_indexes::send_message);

	EGCResult status;

	bool sendMessage = write.PreSendMessage(unMsgType, const_cast<void*>(pubData), cubData);

	if (!sendMessage)
		return k_EGCResultOKxd;

	return oGCSendMessage(ecx, unMsgType, const_cast<void*>(pubData), cubData);
}


static void* __stdcall getDemoPlaybackParameters()
{
    static auto orevelar = reveal.get_original<void*>(218);
    if (uintptr_t returnAddress = uintptr_t(_ReturnAddress()); g_Options.misc.revealSuspect && (returnAddress == _new_test || returnAddress == _new_test2))
        return nullptr;

    return orevelar;
}


cDetour<Hooks::PaintTraverse_t>* oPaintTraverse_t;
cDetour<Hooks::LockCursor_t>* oLockCursor_t;
cDetour<Hooks::D3D9FEATURES>* oD3D9FEATURES;
cDetour<Hooks::FrameStageNotify_t>* oFrameStageNotify_t;
cDetour<Hooks::GCSendMessage>* oGCSendMessage;
cDetour<Hooks::GCRetrieveMessage>* oGCRetrieveMessage;
cDetour<Hooks::CreateMove_t>* oCreateMove_t;
cDetour<Hooks::OverrideView_t>* oOverrideView_t;
cDetour<Hooks::oGetViewModelFOV>* oGGetViewModelFOV_t;

cDetour<Hooks::DrawModelExecute_t>* oDrawModelExecute_t;
cDetour<Hooks::FireEventClientSide>* oFireEventClientSide;
cDetour<Hooks::FindMDL_t>* oFindMDL_t;
cDetour<Hooks::SceneEnd_t>* oSceneEnd_t_t;
cDetour<Hooks::SetupBones_t>* g_fnOriginalSetupBones;
cDetour<Hooks::WriteUsercmdDeltaToBufferFn>* g_WriteUsercmdDeltaToBuffer_hook;
cDetour<Hooks::EndSceneFn>* oEndSceneFn;
cDetour<Hooks::EndSceneResetFn>* oReset;
cDetour<Hooks::ReadPacketEntities>* oPacket;
cDetour<Hooks::EmitSound1>* oEmitSound;
void Hooks::Init()
{
	ENGINE();
	auto& BaimLessHooks = cContext::GetInstance();

	// VMTHooks
	PVOID* d3d9				= *reinterpret_cast<PVOID**>(g_D3DDevice9);
	PVOID* inputkeysystem	= *reinterpret_cast<PVOID**>(g_pInputSystem);
	PVOID* clientdll		= *reinterpret_cast<PVOID**>(g_pClientDll);
	PVOID* lockcursor		= *reinterpret_cast<PVOID**>(g_pSurface);
	PVOID* panelhook		= *reinterpret_cast<PVOID**>(g_pPanel);
	PVOID* clientmode		= *reinterpret_cast<PVOID**>(g_pClientMode);
	PVOID* modelrender		= *reinterpret_cast<PVOID**>(g_pModelRender);
	PVOID* eventmanager		= *reinterpret_cast<PVOID**>(g_pEventManager);
	PVOID* modelchanger		= *reinterpret_cast<PVOID**>(g_pModelCache);
	PVOID* RenderView		= *reinterpret_cast<PVOID**>(g_pRenderView);
    PVOID* D3DDevice9		= *reinterpret_cast<PVOID**>(g_D3DDevice9);
    PVOID* sound_hook		= *reinterpret_cast<PVOID**>(g_EngineSound);
    PVOID* state			= *reinterpret_cast<PVOID**>((void*)((uintptr_t)g_pClientState + 0x8));
	//if (g_pClientState != nullptr && g_pClientState->net_channel != nullptr && g::pLocalEntity != nullptr && g::pLocalEntity->IsAlive())
	//m_pNetChannelSwap.setup(g_pClientState->net_channel);
	gc_hook.setup(g_SteamGameCoordinator);
	m_pEngineSwap.setup(g_pEngine);
	clientMode.setup(g_pClientMode);
    BaimLessHooks.ApplyDetour<Hooks::EmitSound1>(static_cast<Hooks::EmitSound1>(sound_hook[vtable_indexes::sound]), reinterpret_cast<Hooks::EmitSound1>(Hooks::hkEmitSound1), &oEmitSound);
	BaimLessHooks.ApplyDetour<Hooks::EndSceneFn>(static_cast<Hooks::EndSceneFn>(d3d9[vtable_indexes::d3d9menu]), reinterpret_cast<Hooks::EndSceneFn>(Hooks::Hooked_EndScene), &oEndSceneFn);
    BaimLessHooks.ApplyDetour<Hooks::EndSceneResetFn>(static_cast<Hooks::EndSceneResetFn>(D3DDevice9[vtable_indexes::reset]), reinterpret_cast<Hooks::EndSceneResetFn>(Hooks::hkReset), &oReset);
	BaimLessHooks.ApplyDetour<Hooks::PaintTraverse_t>(static_cast<Hooks::PaintTraverse_t>(panelhook[vtable_indexes::paint]), reinterpret_cast<Hooks::PaintTraverse_t>(Hooks::PaintTraverse), &oPaintTraverse_t);
	BaimLessHooks.ApplyDetour<Hooks::LockCursor_t>(static_cast<Hooks::LockCursor_t>(lockcursor[vtable_indexes::lockCursor]), reinterpret_cast<Hooks::LockCursor_t>(Hooks::LockCursor), &oLockCursor_t);
	BaimLessHooks.ApplyDetour<Hooks::D3D9FEATURES>(static_cast<Hooks::D3D9FEATURES>(d3d9[17]), reinterpret_cast<Hooks::D3D9FEATURES>(Hooks::Present), &oD3D9FEATURES);
	BaimLessHooks.ApplyDetour<Hooks::FrameStageNotify_t>(static_cast<Hooks::FrameStageNotify_t>(clientdll[vtable_indexes::frameStage]), reinterpret_cast<Hooks::FrameStageNotify_t>(Hooks::FrameStageNotify), &oFrameStageNotify_t);
	BaimLessHooks.ApplyDetour<Hooks::CreateMove_t>(static_cast<Hooks::CreateMove_t>(clientmode[vtable_indexes::createMove]), reinterpret_cast<Hooks::CreateMove_t>(Hooks::CreateMove), &oCreateMove_t);
	BaimLessHooks.ApplyDetour<Hooks::OverrideView_t>(static_cast<Hooks::OverrideView_t>(clientmode[vtable_indexes::view]), reinterpret_cast<Hooks::OverrideView_t>(Hooks::OverrideView), &oOverrideView_t);
	BaimLessHooks.ApplyDetour<Hooks::oGetViewModelFOV>(static_cast<Hooks::oGetViewModelFOV>(clientmode[vtable_indexes::GGetViewModelFOV]), reinterpret_cast<Hooks::oGetViewModelFOV>(Hooks::GGetViewModelFOV), &oGGetViewModelFOV_t);
	BaimLessHooks.ApplyDetour<Hooks::DrawModelExecute_t>(static_cast<Hooks::DrawModelExecute_t>(modelrender[vtable_indexes::dme]), reinterpret_cast<Hooks::DrawModelExecute_t>(Hooks::DrawModelExecute), &oDrawModelExecute_t);
	BaimLessHooks.ApplyDetour<Hooks::FireEventClientSide>(static_cast<Hooks::FireEventClientSide>(eventmanager[vtable_indexes::event_scene]), reinterpret_cast<Hooks::FireEventClientSide>(Hooks::Hooked_FireEventClientSide), &oFireEventClientSide);
	BaimLessHooks.ApplyDetour<Hooks::FindMDL_t>(static_cast<Hooks::FindMDL_t>(modelchanger[vtable_indexes::model_cache]), reinterpret_cast<Hooks::FindMDL_t>(Hooks::Hooked_findmdl), &oFindMDL_t);
	BaimLessHooks.ApplyDetour<Hooks::SceneEnd_t>(static_cast<Hooks::SceneEnd_t>(RenderView[vtable_indexes::sceneEnd]), reinterpret_cast<Hooks::SceneEnd_t>(Hooks::SceneEnd), &oSceneEnd_t_t);
	gc_hook.hook_index(vtable_indexes::send_message, hkGCSendMessage);
	gc_hook.hook_index(vtable_indexes::retrieve_message, hkGCRetrieveMessage);
	m_pEngineSwap.hook_index(27, IsConnected);
	clientMode.hook_index(44, doPostScreenEffects);
	//if (g_pClientState != nullptr && g_pClientState->net_channel != nullptr && g::pLocalEntity != nullptr && g::pLocalEntity->IsAlive())
	//m_pNetChannelSwap.hook_index(46, SendDatagram);


	FONTS();
	knife_animation();
	hooked_events.RegisterSelf();
	g_pEngine->ClientCmd_UnrestrictedGAY("clear");
	g_pEngine->ClientCmd_UnrestrictedGAY("con_filter_enable 1;con_filter_text_out failed;");
}
void __stdcall Hooks::hkEmitSound1(IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk)
{
    if (!strcmp(pSoundEntry, "UIPanorama.popup_accept_match_beep"))
    {
        if (g_Options.misc.autoaccpet)
        {
            static auto fnAccept = reinterpret_cast<bool(__stdcall*)(const char*)>(Utils::PatternScan(GetModuleHandleA("client_panorama.dll"), "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));

            if (fnAccept)
            {
                fnAccept("");
                FLASHWINFO fi;
                fi.cbSize = sizeof(FLASHWINFO);
                fi.hwnd = InputSys::get().GetMainWindow();
                fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
                fi.uCount = 0;
                fi.dwTimeout = 0;
                FlashWindowEx(&fi);
            }
        }
    }
    oEmitSound->GetTrampoline()(g_EngineSound, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, nSeed, flAttenuation, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, unk);
}

void send_datagram(INetChannel* netchan) {
	if (!g::pLocalEntity->IsAlive())
		return;

	static auto old_seq = 0;

	//skip
	if (old_seq == netchan->m_nInSequenceNr)
		return;

	old_seq = netchan->m_nInSequenceNr;

	/*if (g_Options.misc.ping_spike) {
		netchan->m_nInSequenceNr += (64 - 1) * 2 - static_cast<uint32_t>((64 - 1) * (g_Options.misc.ping_spike_amt / 1000.f));
		return;
	}*/
	if (g_Options.misc.extend_backtrack) {
		netchan->m_nInSequenceNr += (64 - 1) * 2 - static_cast<uint32_t>((64 - 1) * (160.f / 1000.f));
	}
}


bool __fastcall Hooks::IsConnected(void* ecx, void* edx)
{

	using Fn = bool(__thiscall*)(void* ecx);
	static auto ofc = m_pEngineSwap.get_original<Fn>(27);

	static void* is_loadout_allowed = (void*)(Utils::Scan("client_panorama.dll", "84 C0 75 04 B0 01 5F"));

	if (_ReturnAddress() == is_loadout_allowed && g_Options.misc.unlockinventory && g_pEngine->IsInGame())
		return false;

	return ofc(ecx);
}


int __stdcall Hooks::doPostScreenEffects(int param) 
{
	if (g_pEngine->IsInGame()) {
		g_Misc.gravity_ragdoll();
		
	}
	return clientMode.get_original<int>(44);
}



void __stdcall Hooks::Hooked_EndScene(IDirect3DDevice9* pDevice)
{
	

	static uintptr_t gameoverlay_return_address = 0;

	if (!gameoverlay_return_address) {
		MEMORY_BASIC_INFORMATION info;
		VirtualQuery(_ReturnAddress(), &info, sizeof(MEMORY_BASIC_INFORMATION));

		char mod[MAX_PATH];
		GetModuleFileNameA((HMODULE)info.AllocationBase, mod, MAX_PATH);

		if (strstr(mod, "gameoverlay"))
			gameoverlay_return_address = (uintptr_t)(_ReturnAddress());
	}

	if (gameoverlay_return_address != (uintptr_t)(_ReturnAddress()) && g_Options.esp_streamproof)
		return oEndSceneFn->GetTrampoline()(pDevice);

    DWORD colorwrite, srgbwrite;
    IDirect3DVertexDeclaration9* vert_dec = nullptr;
    IDirect3DVertexShader9* vert_shader = nullptr;
    DWORD dwOld_D3DRS_COLORWRITEENABLE = NULL;
    pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
    pDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);

    pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
    //removes the source engine color correction
    pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

    pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
    pDevice->GetVertexDeclaration(&vert_dec);
    pDevice->GetVertexShader(&vert_shader);
    pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
    pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
    pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
    pDevice->SetSamplerState(NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
    pDevice->SetSamplerState(NULL, D3DSAMP_SRGBTEXTURE, NULL);


    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

	g::notify = ImGui::GetOverlayDrawList();
	
    auto esp_drawlist = Render::get().RenderScene();

    Menu::get().Render();//ImGui Menu
	

    ImGui::Render(esp_drawlist);
    
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
    pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
    pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);
    pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, true);
    pDevice->SetVertexDeclaration(vert_dec);
    pDevice->SetVertexShader(vert_shader);

	return oEndSceneFn->GetTrampoline()(pDevice);
}


bool __fastcall Hooks::hkdWriteUsercmdDeltaToBuffer(void* ecx, void*, int slot, bf_write * buf, int from, int to, bool isnewcommand)
{


	if (m_nTickbaseShift <= 0)
		return g_WriteUsercmdDeltaToBuffer_hook->GetTrampoline()(ecx, slot, buf, from, to, isnewcommand);

	if (from != -1)
		return true;

	auto CL_SendMove = []() {
		using CL_SendMove_t = void(__fastcall*) (void);
		static CL_SendMove_t CL_SendMoveF = (CL_SendMove_t)Utils::FindSignature("engine.dll", "55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? B9 ? ? ? ? 53 8B 98");

		CL_SendMoveF();
	};
	auto WriteUsercmd = [](bf_write* buf, CUserCmd* in, CUserCmd* out)
	{
		using WriteUsercmd_t = void(__fastcall*) (bf_write*, CUserCmd*, CUserCmd*);
		static WriteUsercmd_t WriteUsercmdF = (WriteUsercmd_t)Utils::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 51 53 56 8B D9 8B 0D");
		WriteUsercmdF(buf, in, out);

	};

	int* pNumBackupCommands = (int*)(reinterpret_cast <uintptr_t> (buf) - 0x30);
	int* pNumNewCommands = (int*)(reinterpret_cast <uintptr_t> (buf) - 0x2C);
	int32_t new_commands = *pNumNewCommands;

	int32_t next_cmdnr = g_pClientState->last_command + g_pClientState->choked_commands + 1;
	int32_t total_new_commands = min(m_nTickbaseShift, 62);
	m_nTickbaseShift -= total_new_commands;

	from = -1;
	*pNumNewCommands = total_new_commands;
	*pNumBackupCommands = 0;

	for (to = next_cmdnr - new_commands + 1; to <= next_cmdnr; to++)
	{
		if (!g_WriteUsercmdDeltaToBuffer_hook->GetTrampoline()(ecx, slot, buf, from, to, isnewcommand))
		{
			return false;
		}

		from = to;
	}

	CUserCmd* last_realCmd = g_Input->GetUserCmd(slot, from);
	CUserCmd fromCmd;

	if (last_realCmd)
	{
		fromCmd = *last_realCmd;
	}

	CUserCmd toCmd = fromCmd;
	toCmd.command_number++;
	toCmd.tick_count += 200;

	for (int i = new_commands; i <= total_new_commands; i++)
	{
		WriteUsercmd(buf, &toCmd, &fromCmd);

		fromCmd = toCmd;

		toCmd.command_number++;
		toCmd.tick_count++;
	}

	return true;


	////////////////////////////////////////
}
long __stdcall Hooks::hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    static auto Reset = oReset->GetTrampoline();

    Menu::get().OnDeviceLost();

    auto hr = Reset(device, pPresentationParameters);

    if (hr >= 0)
        Menu::get().OnDeviceReset();

    return hr;
}

bool __fastcall Hooks::setup_bones(IClientRenderable* ecx, void* edx, matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	/*
	//static auto g_fnOriginalSetupBones = sb.get_original<SetupBones_t>(13);
	//g_fnOriginalSetupBones = q.get_original<SetupBones_t>(13);
	if (!ecx)
		return g_fnOriginalSetupBones->GetTrampoline()(ecx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);

	if (!((IClientRenderable*)ecx)->GetIClientUnknown())
		return g_fnOriginalSetupBones->GetTrampoline()(ecx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);

	IClientNetworkable* pNetworkable = ((IClientRenderable*)ecx)->GetIClientUnknown()->GetClientNetworkable();

	if (!pNetworkable)
		return g_fnOriginalSetupBones->GetTrampoline()(ecx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);

	if (!pNetworkable->GetClientClass())
		return g_fnOriginalSetupBones->GetTrampoline()(ecx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);

	if (pNetworkable->GetClientClass()->ClassID != ClassId->CCSPlayer)
		return g_fnOriginalSetupBones->GetTrampoline()(ecx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);

	static auto host_timescale = Interfaces->Cvar->FindVar(("host_timescale"));

	auto player = (CBaseEntity*)ecx;
	if (!player) return g_fnOriginalSetupBones->GetTrampoline()(ecx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	if (player->GetHealth() <= 0) return g_fnOriginalSetupBones->GetTrampoline()(ecx, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	float OldCurTime = Interfaces->Globals->curtime;
	float OldRealTime = Interfaces->Globals->realtime;
	float OldFrameTime = Interfaces->Globals->frametime;
	float OldAbsFrameTime = Interfaces->Globals->absoluteframetime;
	float OldAbsFrameTimeStart = Interfaces->Globals->absoluteframestarttimestddev;
	float OldInterpAmount = Interfaces->Globals->interpolation_amount;
	int OldFrameCount = Interfaces->Globals->framecount;
	int OldTickCount = Interfaces->Globals->tickcount;

	Interfaces->Globals->curtime = player->GetSimulationTime();
	Interfaces->Globals->realtime = player->GetSimulationTime();
	Interfaces->Globals->frametime = Interfaces->Globals->interval_per_tick * host_timescale->GetFloat();
	Interfaces->Globals->absoluteframetime = Interfaces->Globals->interval_per_tick * host_timescale->GetFloat();
	Interfaces->Globals->absoluteframestarttimestddev = player->GetSimulationTime() - Interfaces->Globals->interval_per_tick * host_timescale->GetFloat();
	Interfaces->Globals->interpolation_amount = 0;
	Interfaces->Globals->framecount = TIME_TO_TICKS(player->GetSimulationTime());
	Interfaces->Globals->tickcount = TIME_TO_TICKS(player->GetSimulationTime());

	*(int*)((int)player + 236) |= 8; // IsNoInterpolationFrame
	bool ret_value = g_fnOriginalSetupBones->GetTrampoline()(player, pBoneToWorldOut, nMaxBones, boneMask, Interfaces->Globals->curtime);
	*(int*)((int)player + 236) &= ~8; // (1 << 3)

	Interfaces->Globals->curtime = OldCurTime;
	Interfaces->Globals->realtime = OldRealTime;
	Interfaces->Globals->frametime = OldFrameTime;
	Interfaces->Globals->absoluteframetime = OldAbsFrameTime;
	Interfaces->Globals->absoluteframestarttimestddev = OldAbsFrameTimeStart;
	Interfaces->Globals->interpolation_amount = OldInterpAmount;
	Interfaces->Globals->framecount = OldFrameCount;
	Interfaces->Globals->tickcount = OldTickCount;

	return ret_value;*/
	return 0;

}



bool __fastcall Hooks::Hooked_FireEventClientSide(void *ecx, void* edx, IGameEvent* pEvent)
{


	if (!pEvent)
		return oFireEventClientSide->GetTrampoline()(ecx, pEvent);



	return oFireEventClientSide->GetTrampoline()(ecx, pEvent);


}
void DoAsusWalls()
{
	if (!g_pEngine->IsInGame() && !g_pEngine->IsConnected())
		return;
	static bool bPerformed = false, bLastSetting;
	static float bLastSettingWorldfl;
	static float bLastSettingsPropfl;

	
	if (bLastSettingWorldfl != g_Options.esp.other.world_transparency || bLastSettingsPropfl != g_Options.esp.other.prop_transp)
	{
		bLastSettingWorldfl = g_Options.esp.other.world_transparency;
		bLastSettingsPropfl = g_Options.esp.other.prop_transp;
		bPerformed = false;
	}

	if (!bPerformed)
	{
		ConVar* r_drawspecificstaticprop = g_pCvar->FindVar("r_drawspecificstaticprop");
		*(int*)((DWORD)&r_drawspecificstaticprop->fnChangeCallback + 0xC) = 0; // ew
		r_drawspecificstaticprop->SetValue(1);


		for (auto i = g_pMaterialSys->FirstMaterial(); i != g_pMaterialSys->InvalidMaterial(); i = g_pMaterialSys->NextMaterial(i))
		{
			IMaterial* pMaterial = g_pMaterialSys->GetMaterial(i);

			if (!pMaterial || pMaterial->IsErrorMaterial())
				continue;


			if (strstr(pMaterial->GetTextureGroupName(), "World"))
			{
				pMaterial->AlphaModulate(g_Options.esp.other.world_transparency / 100.f);
			}

			if (strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
			{
				pMaterial->AlphaModulate(g_Options.esp.other.prop_transp / 100.f);
			}
		} bPerformed = true;
	}
}
#include "GrenadeHelperAPI.h"
#include "Render2.h"
void update_map()
{
	if (!g_pEngine->IsInGame() || !g_pEngine->IsInGame() || !g::pLocalEntity->IsAlive() || !g::pLocalEntity)
		return;
	if (!g_pEngine->GetLevelNameShort())
		return;
	cGrenade.bUpdateGrenadeInfo(g_pEngine->GetLevelNameShort());
}
int StringToWeapon(std::string weapon) {
	if (!strcmp(weapon.c_str(), "smokegrenade"))
		return 45;
	if (!strcmp(weapon.c_str(), "flashbang"))
		return 43;
	if (!strcmp(weapon.c_str(), "incgrenade"))
		return 46;
}
Color GrenadeHelper = Color(esp::colors::grenadehelper);
void GrenadeHelper_()
{
	if (g_Options.esp.helper)
	{
		if (g_pEngine->IsInGame() && g::pLocalEntity && g_pEngine->IsConnected())
		{
			
			C_BaseEntity* local = (C_BaseEntity*)g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
			if (!local)
				return;
			if (!local->IsAlive())
				return;
			for (int i = 0; i < cGrenade.GrenadeInfo.size(); i++)
			{
				GrenadeInfo_t info;
				if (!cGrenade.GetInfo(i, &info))
					continue;

				int iGrenadeID = StringToWeapon(info.szWeapon);

				C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)local->GetActiveWeapon();
				
				if (!pWeapon)
					continue;
				int id = *pWeapon->m_iItemDefinitionIndex();
				if (!(id == iGrenadeID || (iGrenadeID == 46 && id == 48)))
					continue;

				Vector vecOnScreenOrigin, vecOnScreenAngles;
				int iCenterY, iCenterX;
				g_pEngine->GetScreenSize(iCenterY, iCenterX);
				iCenterX /= 2;
				iCenterY /= 2;

				float dist = sqrt(pow(local->GetRenderOrigin().x - info.vecOrigin.x, 2) + pow(local->GetRenderOrigin().y - info.vecOrigin.y, 2) + pow(local->GetRenderOrigin().z - info.vecOrigin.z, 2)) * 0.0254f;

				if (dist < 0.5f)
				{
					if (Utils::WorldToScreen(info.vecOrigin, vecOnScreenOrigin))
						g_pRender->DrawWave1(info.vecOrigin, 4, Color(255,0,0));

					Vector vecAngles;
					g_Math.AngleVectors(info.vecViewangles, &vecAngles);
					vecAngles *= 100;

					if (Utils::WorldToScreen((local->get_eye_pos() + vecAngles), vecAngles))
						g_pRender->DrawFillBox(vecAngles.x, vecAngles.y, 7, 7, Color(0, 128, 0));

					g_pRender->Text(iCenterX, iCenterY + 30, true, true, Color(255,255,255), (char*)info.szName.c_str());
					g_pRender->Text(iCenterX, iCenterY, true, true, Color(255,255,255), (char*)info.szDescription.c_str());

				}
				else
				{
					if (Utils::WorldToScreen(info.vecOrigin, vecOnScreenOrigin));

					g_pRender->DrawWave1(info.vecOrigin, 10, Color(GrenadeHelper));
					g_pRender->DrawWave1(info.vecOrigin, 7, Color(GrenadeHelper));
				}
			}
		}
	}
	else
	{
		if (g_pEngine->IsInGame() && g::pLocalEntity)
			cGrenade.GrenadeInfo.clear();
	}
}

void RecoilCrosshair()
{
	if (!g_Options.legit.recoil_crosshair)
		return;

	C_BaseEntity* localplayer = (C_BaseEntity*)g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (g_pEngine->IsInGame() && localplayer)
	{
		Vector ViewAngles;
		g_pEngine->GetViewAngles(ViewAngles);
		ViewAngles += (localplayer->GetAimPunchAngle()) * 2.f;

		Vector fowardVec;
		g_Math.AngleVectors(ViewAngles, fowardVec);
		fowardVec *= 10000;

		Vector start = localplayer->get_eye_pos();
		Vector end = start + fowardVec, endScreen;

		if (Utils::WorldToScreen(end, endScreen) && localplayer->IsAlive())
		{
			g_pRender->DrawLine(endScreen.x + 7, endScreen.y + 0.5, endScreen.x - 8, endScreen.y + 0.5, Color(255,0,0));
			g_pRender->DrawLine(endScreen.x + 0, endScreen.y + 7, endScreen.x + 0, endScreen.y - 8, Color(255,0,0));
		}

	}
}
static bool initialized = false;

HRESULT __stdcall Hooks::Present(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect,
	HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	//logs();
	RecoilCrosshair();
	//GrenadeHelper_();
	DoAsusWalls();
	return	oD3D9FEATURES->GetTrampoline()(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}


void viewmodel_x_y_z()
{
	static int vx, vy, vz, b1g;
	static ConVar* view_x = g_pCvar->FindVar("viewmodel_offset_x");
	static ConVar* view_y = g_pCvar->FindVar("viewmodel_offset_y");
	static ConVar* view_z = g_pCvar->FindVar("viewmodel_offset_z");



	ConVar* sv_cheats = g_pCvar->FindVar("sv_cheats");
	*(int*)((DWORD)&sv_cheats->fnChangeCallback + 0xC) = 0; // ew
	sv_cheats->SetValue(1);

	ConVar* sv_minspec = g_pCvar->FindVar("sv_competitive_minspec");
	*(int*)((DWORD)&sv_minspec->fnChangeCallback + 0xC) = 0; // ew
	sv_minspec->SetValue(0);


	view_x->nFlags &= ~FCVAR_CHEAT;
	view_y->nFlags &= ~FCVAR_CHEAT;
	view_z->nFlags &= ~FCVAR_CHEAT;

	vx = g_Options.esp.other.fov_x;
	vy = g_Options.esp.other.fov_y;
	vz = g_Options.esp.other.fov_z;


	view_x->SetValue(vx);

	view_y->SetValue(vy);

	view_z->SetValue(vz);


}

void Hooks::Restore()
{
	/*

	if (shaderapidx9)
	{

	gc_hook.hook_index(vtable_indexes::send_message, hkGCSendMessage);
	gc_hook.hook_index(vtable_indexes::retrieve_message, hkGCRetrieveMessage);

		*/
  




		/* add later
		g_Hooks.pClientHook->Unhook(vtable_indexes::frameStage);
		g_Hooks.pClientModeHook->Unhook(vtable_indexes::createMove);
		g_Hooks.pClientModeHook->Unhook(vtable_indexes::view);
		g_Hooks.pSurfaceHook->Unhook(vtable_indexes::lockCursor);
		g_Hooks.pPanelHook->Unhook(vtable_indexes::paint);
		g_Hooks.pModelHook->Unhook(vtable_indexes::dme);
		*/
	SetWindowLongPtr(g_Hooks.hCSGOWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_Hooks.pOriginalWNDProc));

	NetvarManager::UnhookRecvProxies();
	hooked_events.RemoveSelf();
    
}

void Hooks::HookPlayers()
{
	static bool Init[65];
	static bool Hooked[65];

	for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
	{
		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

		if (!pPlayerEntity
			|| !pPlayerEntity->IsAlive()
			|| pPlayerEntity->IsDormant())
		{
			if (Hooked[i])
				g_Hooks.pPlayerHook[i]->Unhook(vtable_indexes::extraBonePro);

			Hooked[i] = false;
			continue;
		}

		if (!Init[i])
		{
			g_Hooks.pPlayerHook[i] = std::make_unique<ShadowVTManager>();
			Init[i] = true;
		}
		
		if (Hooked[i])
			g_Hooks.pPlayerHook[i]->Unhook(vtable_indexes::extraBonePro);

		if (!Hooked[i])
		{
			g_Hooks.pPlayerHook[i]->Setup(pPlayerEntity);
			g_Hooks.pPlayerHook[i]->Hook(vtable_indexes::extraBonePro, Hooks::DoExtraBonesProcessing);

			Hooked[i] = true;
		}
	}
}


typedef void(__fastcall* ClanTagFn)(const char*, const char*);
ClanTagFn dw_ClanTag;
void SetClanTag(const char* tag, const char* name)
{

	if (!dw_ClanTag)
		dw_ClanTag = reinterpret_cast<ClanTagFn>(((DWORD)Utils::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx")));

	if (dw_ClanTag)
		dw_ClanTag(tag, name);
}

void clan_tag()
{
	if (g_Options.misc.clantag_type == 0)
	{
		static int iLastTime;

		if (int(g_pGlobalVars->curtime) != iLastTime)
		{
			//but case clantag is broken tho (this not, i tested)
			switch (int(g_pGlobalVars->curtime) % 1)
			{
			case 0: SetClanTag("", ""); break;

			}
			iLastTime = int(g_pGlobalVars->curtime);

		}

	}

	if (g_Options.misc.clantag_type == 1)
	{
		static int iLastTime;

		if (int(g_pGlobalVars->curtime) != iLastTime)
		{
			//but case clantag is broken tho (this not, i tested)
			switch (int(g_pGlobalVars->curtime) % 27)
			{
			case 0:  SetClanTag(("B"), "BAIMLESS.xyz"); break;
			case 1:  SetClanTag(("BA"), "BAIMLESS.xyz"); break;
			case 2:  SetClanTag(("BAI"), "BAIMLESS.xyz"); break;
			case 3:  SetClanTag(("BAIM"), "BAIMLESS.xyz"); break;
			case 4:  SetClanTag(("BAIML"), "BAIMLESS.xyz"); break;
			case 5:  SetClanTag(("BAIMLE"), "BAIMLESS.xyz"); break;
			case 6:  SetClanTag(("BAIMLES"), "BAIMLESS.xyz"); break;
			case 7:  SetClanTag(("BAIMLESS"), "BAIMLESS.xyz"); break;
			case 8:  SetClanTag(("BAIMLESS.x"), "BAIMLESS.xyz"); break;
			case 9:  SetClanTag(("BAIMLESS.xy"), "BAIMLESS.xyz"); break;
			case 10: SetClanTag(("BAIMLESS.xyz"), "BAIMLESS.xyz"); break;
			case 11: SetClanTag(("BAIMLESS.xyz"), "BAIMLESS.xyz"); break;
			case 12: SetClanTag(("BAIMLESS.xyz"), "BAIMLESS.xyz"); break;
			case 13: SetClanTag(("BAIMLESS.xyz"), "BAIMLESS.xyz"); break;
			case 14: SetClanTag(("BAIMLESS.xyz"), "BAIMLESS.xyz"); break;
			case 15: SetClanTag(("BAIMLESS.xy "), "BAIMLESS.xyz"); break;
			case 16: SetClanTag(("BAIMLESS.x "), "BAIMLESS.xyz"); break;
			case 17: SetClanTag(("BAIMLESS. "), "BAIMLESS.xyz"); break;
			case 18: SetClanTag(("BAIMLESS "), "BAIMLESS.xyz"); break;
			case 19: SetClanTag(("BAIMLES "), "BAIMLESS.xyz"); break;
			case 20: SetClanTag(("BAIMLE "), "BAIMLESS.xyz"); break;
			case 21: SetClanTag(("BAIML "), "BAIMLESS.xyz"); break;
			case 22: SetClanTag(("BAIM "), "BAIMLESS.xyz"); break;
			case 23: SetClanTag(("BAI "), "BAIMLESS.xyz"); break;
			case 24: SetClanTag(("BA "), "BAIMLESS.xyz"); break;
			case 25: SetClanTag(("B "), "BAIMLESS.xyz"); break;
			case 26: SetClanTag((" "), "BAIMLESS.xyz"); break;


			}
			iLastTime = int(g_pGlobalVars->curtime);

		}
	}

	
	if (g_Options.misc.clantag_type == 2)
	{
		if (g_Options.clantag.c_str() != NULL && g_Options.clantag.c_str() != "")
		{
			static int iLastTime;

			if (int(g_pGlobalVars->curtime) != iLastTime)
			{
				//but case clantag is broken tho (this not, i tested)
				switch (int(g_pGlobalVars->curtime) % 1)
				{
				case 0: SetClanTag(g_Options.clantag.c_str(), g_Options.clantag.c_str()); break;


				}
				iLastTime = int(g_pGlobalVars->curtime);

			}
		}

		else
		{
			static int iLastTime;

			if (int(g_pGlobalVars->curtime) != iLastTime)
			{
				//but case clantag is broken tho (this not, i tested)
				switch (int(g_pGlobalVars->curtime) % 1)
				{
				case 0: SetClanTag("", ""); break;

				}
				iLastTime = int(g_pGlobalVars->curtime);

			}
		}
	}

	//getze.us
}
template<class T, class U>
T fine(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}
void runExtrapolation()
{
	if (g_Options.rage.extrapolation)
	{
		auto m_local = g::pLocalEntity;
		if (m_local && m_local->IsAlive())
		{
			for (int i = 1; i < g_pGlobalVars->maxClients; i++)
			{
				auto m_entity = g_pEntityList->GetClientEntity(i);
				if (!m_entity || m_entity == m_local || m_entity->GetClientClass()->ClassID != (int)40 || !m_entity->IsAlive())
					continue;

				if (!m_entity->GetSimulationTime())
					return;
				if (m_entity->GetOrigin() == Vector(0, 0, 0))
					return;
				float simtime_delta = m_entity->GetSimulationTime() - m_entity->m_flOldSimulationTime();
				int choked_ticks = fine(TIME_TO_TICKS(simtime_delta), 1, 15);
				Vector lastOrig;

				if (lastOrig.Length() != m_entity->GetOrigin().Length())
					lastOrig = m_entity->GetOrigin();

				float delta_distance = (m_entity->GetOrigin() - lastOrig).LengthSqr();
				if (delta_distance > 4096.f)
				{
					Vector velocity_per_tick = m_entity->GetVelocity() * g_pGlobalVars->intervalPerTick;
					auto new_origin = m_entity->GetOrigin() + (velocity_per_tick * choked_ticks);
					m_entity->SetAbsOriginal(new_origin);
				}
			}
		}
	}
}
bool IsScoutNOI(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_iItemDefinitionIndex();
	static const std::vector<int> v = { (int)ItemDefinitionIndex::WEAPON_SSG08 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool IsAWPnein(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_iItemDefinitionIndex();
	static const std::vector<int> v = { (int)ItemDefinitionIndex::WEAPON_AWP };
	return (std::find(v.begin(), v.end(), id) != v.end());
}
bool IsAutoasddas(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_iItemDefinitionIndex();
	static const std::vector<int> v = { (int)ItemDefinitionIndex::WEAPON_G3SG1,(int)ItemDefinitionIndex::WEAPON_SCAR20 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}



bool IsPistolboi(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_iItemDefinitionIndex();
	static const std::vector<int> v = { (int)ItemDefinitionIndex::WEAPON_DEAGLE,(int)ItemDefinitionIndex::WEAPON_CZ75,(int)ItemDefinitionIndex::WEAPON_ELITE,(int)ItemDefinitionIndex::WEAPON_USPS,(int)ItemDefinitionIndex::WEAPON_P250,(int)ItemDefinitionIndex::WEAPON_P2000, (int)ItemDefinitionIndex::WEAPON_TEC9,(int)ItemDefinitionIndex::WEAPON_REVOLVER,(int)ItemDefinitionIndex::WEAPON_FIVESEVEN,(int)ItemDefinitionIndex::WEAPON_GLOCK };
	return (std::find(v.begin(), v.end(), id) != v.end());
}
bool IsRiflena(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_iItemDefinitionIndex();
	static const std::vector<int> v = { (int)ItemDefinitionIndex::WEAPON_AK47,(int)ItemDefinitionIndex::WEAPON_AUG,(int)ItemDefinitionIndex::WEAPON_FAMAS,(int)ItemDefinitionIndex::WEAPON_GALIL,(int)ItemDefinitionIndex::WEAPON_M249,(int)ItemDefinitionIndex::WEAPON_M4A1S,(int)ItemDefinitionIndex::WEAPON_M4A4,(int)ItemDefinitionIndex::WEAPON_NEGEV,(int)ItemDefinitionIndex::WEAPON_SG553 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool IsMP(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_iItemDefinitionIndex();
	static const std::vector<int> v = { (int)ItemDefinitionIndex::WEAPON_P90,(int)ItemDefinitionIndex::WEAPON_BIZON,(int)ItemDefinitionIndex::WEAPON_MP7,(int)ItemDefinitionIndex::WEAPON_MP9,(int)ItemDefinitionIndex::WEAPON_MAC10,(int)ItemDefinitionIndex::WEAPON_UMP45 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

void test()
{

	
}
void WeaponCfg()
{

	auto pWeapon = g::pLocalEntity->GetActiveWeapon();
	if (!pWeapon)
		return;
	/*
	if (Options::Menu.RageBotTab.WeaponCheck.GetState())
	{

		if (IsScoutNOI(pWeapon))
		{

			Options::Menu.RageBotTab.damage_val.SetValue((float)Options::Menu.RageBotTab.scoutmindmg.GetValue());
		}
		else if (IsAWPnein(pWeapon))
		{

			Options::Menu.RageBotTab.damage_val.SetValue((float)Options::Menu.RageBotTab.AWPmindmg.GetValue());
		}
		else if (IsAutoasddas(pWeapon))
		{

			Options::Menu.RageBotTab.damage_val.SetValue((float)Options::Menu.RageBotTab.Automindmg.GetValue());
		}
		else if (IsPistolboi(pWeapon))
		{

			Options::Menu.RageBotTab.damage_val.SetValue((float)Options::Menu.RageBotTab.Pistolmindmg.GetValue());
		}
		else if (IsRiflena(pWeapon))
		{

			Options::Menu.RageBotTab.damage_val.SetValue((float)Options::Menu.RageBotTab.Riflemindmg.GetValue());
		}
		else if (IsMP(pWeapon))
		{

			Options::Menu.RageBotTab.damage_val.SetValue((float)Options::Menu.RageBotTab.MPmindmg.GetValue());
		}





		if (IsScoutNOI(pWeapon))
		{

			Options::Menu.RageBotTab.chance_val.SetValue((float)Options::Menu.RageBotTab.scoutChance.GetValue());
		}
		else if (IsAWPnein(pWeapon))
		{

			Options::Menu.RageBotTab.chance_val.SetValue((float)Options::Menu.RageBotTab.AWPChance.GetValue());
		}
		else if (IsAutoasddas(pWeapon))
		{

			Options::Menu.RageBotTab.chance_val.SetValue((float)Options::Menu.RageBotTab.AutoChance.GetValue());
		}
		else if (IsPistolboi(pWeapon))
		{

			Options::Menu.RageBotTab.chance_val.SetValue((float)Options::Menu.RageBotTab.PistolChance.GetValue());
		}
		else if (IsRiflena(pWeapon))
		{

			Options::Menu.RageBotTab.chance_val.SetValue((float)Options::Menu.RageBotTab.RifleChance.GetValue());
		}
		else if (IsMP(pWeapon))
		{

			Options::Menu.RageBotTab.chance_val.SetValue((float)Options::Menu.RageBotTab.MPChance.GetValue());
		}


	}*/
}



float GetDes32yncDeltaNEW()
{
	auto animstate = uintptr_t(g::pLocalEntity->AnimState());
	float duckammount = *(float *)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));
	float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));
	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001) - 0.19999999) * speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0)
	{
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}
	unk3 = *(float *)(animstate + 0x334) * unk2;
	return unk3;
}



void FixWalk(CUserCmd* pCmd)
{
	if (!g_Options.antiaa.fix_leg_mov)
		return;


	auto weapon = g::pLocalEntity->GetActiveWeapon();
	if (!weapon) return;

	if (!g_pEngine->IsInGame())
		return;

	if (!g::pLocalEntity->IsAlive())
		return;


	if (g::pLocalEntity->GetMoveType() == 9)
		return;


	if (pCmd->buttons & IN_FORWARD)
	{
		pCmd->forwardmove = 450;
		pCmd->buttons &= ~IN_FORWARD;
	}
	else if (pCmd->buttons & IN_BACK)
	{
		pCmd->forwardmove = -450;
		pCmd->buttons &= ~IN_BACK;
	}
	if (pCmd->buttons & IN_MOVELEFT)
	{
		pCmd->sidemove = -450;
		pCmd->buttons &= ~IN_MOVELEFT;
	}
	else if (pCmd->buttons & IN_MOVERIGHT)
	{
		pCmd->sidemove = 450;
		pCmd->buttons &= ~IN_MOVERIGHT;
	}
}

bool IsWeapo32nGrenade4(C_BaseCombatWeapon* weapon)
{
	if (weapon == nullptr) return false;
	int id = *weapon->m_iItemDefinitionIndex();
	static const std::vector<int> v = { (int)ItemDefinitionIndex::WEAPON_FLASHBANG,(int)ItemDefinitionIndex::WEAPON_HEGRENADE,(int)ItemDefinitionIndex::WEAPON_SMOKEGRENADE,(int)ItemDefinitionIndex::WEAPON_MOLOTOV,(int)ItemDefinitionIndex::WEAPON_DECOY,(int)ItemDefinitionIndex::WEAPON_INC };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

static bool side = false;
float gayside;
void OnCreateMove(CUserCmd* cmd, bool& send_packet)
{
	if (Menu::get().IsVisible())
		cmd->buttons &= ~IN_ATTACK;
	//LegitAA(cmd, send_packet);

	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player) return;

	//Fakelag(cmd);

	auto weapon = g::pLocalEntity->GetActiveWeapon();
	if (!weapon) return;

	if (!g_pEngine->IsInGame() || !g_Options.legit.legit_aa)
		return;

	if (!g::pLocalEntity->IsAlive())
		return;

	if (weapon->IsKnife() && g::pCmd->buttons & IN_ATTACK2)
		return;

	if (IsWeapo32nGrenade4(weapon))
	{
		return;

	}
	if ((cmd->buttons & IN_USE) ||
		(cmd->buttons & IN_ATTACK) ||
		!g::pLocalEntity ||
		!g::pLocalEntity->IsAlive() ||
		g::pLocalEntity->GetMoveType() == movetype_ladder ||
		g::pLocalEntity->GetMoveType() == movetype_noclip ||
		!weapon ||
		(g_pGameRules && *g_pGameRules && (*g_pGameRules)->m_bFreezePeriod())
		)

		if (g::pLocalEntity->GetMoveType() == 9)
			return;

	if (g::pCmd->buttons & IN_USE)
		return;
	if (g::pCmd->buttons & IN_ATTACK)
		return;


	auto anim_state = local_player->AnimState();

	if (!anim_state)
		return;

	if (g_Options.legit.legit_aa_key)
	{
		side = false;
		if (GetKeyState(g_Options.legit.legit_aa_key))
		{
			side = true;
		}
	}

	if (side)
	{
		gayside = 1;

	}
	else
	{
		gayside = -1;
	}


	if (send_packet)
	{

		Vector ang;
		g_pEngine->GetViewAngles(ang);
		cmd->viewangles.y = ang.y;


	}
	else
	{
		cmd->viewangles.y = g::FakeAngle.y + GetDes32yncDeltaNEW() * gayside;


	}

	if (anim_state->m_flSpeedNormalized < 0.1f)
	{

		if (g::pLocalEntity->GetFlags() & FL_ONGROUND && g::pCmd->sidemove < 4 && g::pCmd->sidemove > -4)
		{
			if (!(cmd->buttons & IN_DUCK)) {
				cmd->sidemove = cmd->tick_count % 2 ? 1.10 : -1.10;
			}
			else {
				cmd->sidemove = cmd->tick_count % 2 ? 3.25 : -3.25;
			}
		}

	}

}

static bool side2 = false;
float gayside2;
void airstuck()
{
	if (!g_Options.antiaa.air_stuck)
		return;



	if (GetKeyState(g_Options.antiaa.air_stuck_key))
	{
		g::pCmd->command_number = INT_MAX;
		g::pCmd->tick_count = INT_MAX;
	}
}

static int nSinceUse;

#define config g_Options
void updateClanTag(bool tagChanged)
{
    static std::string clanTag;
    static auto lastTime{ 0.0f };
    switch (g_Options.misc.clantag_type)
    {
    case 0:
        if (g_pGlobalVars->realtime - lastTime < 0.6f) return;
        lastTime = g_pGlobalVars->realtime;
        SetClanTag("","");
        break;
    case 1:
        if (tagChanged) {
            clanTag ="Baimless.xyz";
            if (!isblank(clanTag.front()) && !isblank(clanTag.back()))
                clanTag.push_back(' ');
        }

       
        if (g_pGlobalVars->realtime - lastTime < 0.6f) return;
        lastTime = g_pGlobalVars->realtime;

        if (config.misc.clantag_type_ == 1 && !clanTag.empty())
            std::rotate(std::begin(clanTag), std::next(std::begin(clanTag)), std::end(clanTag));

        SetClanTag(clanTag.c_str(), clanTag.c_str());

        break;
    case 2:    
        if (tagChanged) {
            clanTag = config.clantag;
            if (!isblank(clanTag.front()) && !isblank(clanTag.back()))
                clanTag.push_back(' ');
        }
        if (g_pGlobalVars->realtime - lastTime < 0.6f) return;
        lastTime = g_pGlobalVars->realtime;

        if (config.misc.clantag_type_ == 1 && !clanTag.empty())
            std::rotate(std::begin(clanTag), std::next(std::begin(clanTag)), std::end(clanTag));

        SetClanTag(clanTag.c_str(), clanTag.c_str());
    }
}

inline float RandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}


auto jumped_last_tick = false;
auto should_fake_jump = false;

void SwitchKnifeHand()
{
	auto weapon = (C_BaseCombatWeapon*)(g_pEntityList->GetClientEntityFromHandle(g::pLocalEntity->m_hActiveWeapon()));

	if (!weapon)
		return;
	const auto info = weapon->GetCSWpnData();
	static auto current_hand_pos = g_pCvar->FindVar(_("cl_righthand"))->GetInt();
	static auto flip_hand = current_hand_pos == 0 ? 1 : 0;
	if (config.misc.misc_knife_switch) {
		if (info->type == (int)CSWeaponType::WEAPONTYPE_KNIFE) {
			g_pCvar->FindVar("cl_righthand")->SetValue(flip_hand);
		}
		else {
			g_pCvar->FindVar("cl_righthand")->SetValue(!flip_hand);
		}
	}
}

void for_each_player(const std::function<void(C_BaseEntity*)> fn)
{
	for (auto i = 0; i <= static_cast<int32_t>(g_pEngine->GetMaxClients()); ++i) {
		const auto entity = g_pEntityList->GetClientEntity(i);
		if (entity && entity->IsPlayer())
			fn(reinterpret_cast<C_BaseEntity*>(entity));
	}
}
void Radar()
{
	if (!config.misc.radar)
		return;

	for_each_player([](C_BaseEntity* player) -> void
		{
			if (!player->IsDormant() && player->IsAlive())
				player->m_bSpotted() = true;
		});
}

bool __fastcall Hooks::CreateMove(IClientMode* thisptr, void* edx, float sample_frametime, CUserCmd* pCmd)
{
	// Call original createmove before we start screwing with it
	oCreateMove_t->GetTrampoline()(thisptr, edx, sample_frametime, pCmd);

	if (!pCmd || !pCmd->command_number)
		return oCreateMove_t->GetTrampoline();

	uintptr_t* FPointer; __asm { MOV FPointer, EBP }
	byte* SendPacket = (byte*)(*FPointer - 0x1C);
	g::bSendPacket = *SendPacket;
	g::bSendPacket = true;

	// Get globals
	g::pCmd = pCmd;
	g::pLocalEntity = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!g::pLocalEntity)
		return oCreateMove_t->GetTrampoline();



	g::OriginalView = g::pCmd->viewangles;
	//	HookPlayers();
	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{
		//Radar();
		//SwitchKnifeHand();
		g_Misc.NameStealer();
		g_Misc.RecoilCrosshair();
        updateClanTag(false);
		static bool Double_tap_lag = false;
		GrenadePrediction::get().Tick(g::pCmd->buttons);
		viewmodel_x_y_z();

		g_Misc.OnCreateMove(pCmd);
		g_Resolver.OnCreateMove();

	
		//clan_tag();
		//FixMoveManager->Start(pCmd);

		WeaponCfg();
		g_Knifebot.Run();
		FixWalk(pCmd);


		g_legitbot.Init();
		g_legitbot.Move(pCmd, g::bSendPacket);
		backtracking->legitBackTrack(pCmd, g::pLocalEntity);
		//g_Legitbot.OnCreateMove();
		airstuck();

	
		//AntiAimNEW.FakeWalk(pCmd);
		//AntiAimNEW.do_antiaim(pCmd);
		//AntiAimNEW.fake_crouch();
		
		TriggerBot.Move(pCmd);
		g_Misc.edge_jump_pre_prediction(pCmd);
		engine_prediction.start_prediction(pCmd); //small note for prediction, we need to run bhop before prediction otherwise it will be buggy

		//air_shot();
        g_Misc.QuickReload(pCmd);
		g_Aimbot.OnCreateMove();
		g_Aimbot.CompensateInaccuracies();
		g_Aimbot.auto_revolver(pCmd);
		static int since_use = 0;
		static int delay_shot = 0;
		static int shoot_state = 2;
		static int state_delay = 0;
	

		if (g_Options.antiaa.double_tap && !AntiAimPro.is_fakeducking)
		{
			if ((g_Options.antiaa.double_tap_on_key && g_Options.antiaa.double_tap_method == 1 && GetAsyncKeyState(g_Options.antiaa.double_tap_key)) || (g_Options.antiaa.double_tap_on_key && g_Options.antiaa.double_tap_method == 0 && overridething_dt_key))
			{

				if (g::pCmd->buttons & IN_ATTACK /*&& c_config::get().dt_hitchance != 0 && c_config::get().dt_delayshot != 0 && c_config::get().state_delay != 0*/)
				{
					//if (since_use++ < 3)
					//{
					Double_tap_lag = true;
					g::bSendPacket = false;

					m_nTickbaseShift = TIME_TO_TICKS(0.2f);
					delay_shot = g_Options.antiaa.double_tap_delay;
					shoot_state = 1;
					state_delay = g_Options.antiaa.double_tap_shot;
					//}
				//	else
				//	{
					//	Double_tap_lag = false;

					//}
				}
				else
				{
					Double_tap_lag = false;

				}
				if (delay_shot != 0 && shoot_state == 0)
				{
					delay_shot--;
					g::pCmd->buttons &= ~IN_ATTACK;
					//since_use = 0;
					if (delay_shot == 0)
					{
						shoot_state = 2;
					}
				}
				if (shoot_state == 1)
				{
					state_delay--;
					if (shoot_state == 1)
					{

						Double_tap_lag = true;
						if (g_Options.rage.enable)
						{
							g::pCmd->viewangles = g::AimbotAngle;

						}
						else
						{

						}
						g::pCmd->buttons |= IN_ATTACK;

					}
					else
					{
						Double_tap_lag = false;

					}
					if (state_delay == 0)
						shoot_state = 0;
				}
				if (state_delay <= 0)
					state_delay = 0;
				if (delay_shot <= 0)
					delay_shot = 0;
			}
			else if (!g_Options.antiaa.double_tap_on_key)
			{
				if (g::pCmd->buttons & IN_ATTACK /*&& c_config::get().dt_hitchance != 0 && c_config::get().dt_delayshot != 0 && c_config::get().state_delay != 0*/)
				{
					//if (since_use++ < 3)
					//{
					Double_tap_lag = true;
					g::bSendPacket = false;

					m_nTickbaseShift = TIME_TO_TICKS(0.2f);
                    delay_shot = g_Options.antiaa.double_tap_delay;
                    shoot_state = 1;
                    state_delay = g_Options.antiaa.double_tap_shot;
					//}
				//	else
				//	{
					//	Double_tap_lag = false;

					//}
				}
				else
				{
					Double_tap_lag = false;

				}
				if (delay_shot != 0 && shoot_state == 0)
				{
					delay_shot--;
					g::pCmd->buttons &= ~IN_ATTACK;
					//since_use = 0;
					if (delay_shot == 0)
					{
						shoot_state = 2;
					}
				}
				if (shoot_state == 1)
				{
					state_delay--;
					if (shoot_state == 1)
					{

						Double_tap_lag = true;
						if (g_Options.rage.enable)
						{
							g::pCmd->viewangles = g::AimbotAngle;

						}
						else
						{

						}
						g::pCmd->buttons |= IN_ATTACK;

					}
					else
					{
						Double_tap_lag = false;

					}
					if (state_delay == 0)
						shoot_state = 0;
				}
				if (state_delay <= 0)
					state_delay = 0;
				if (delay_shot <= 0)
					delay_shot = 0;
			}
			else
			{
				Double_tap_lag = false;

			}

		}
		else
		{
			Double_tap_lag = false;

		}
		

		if (!g_Options.antiaa.double_tap)
		{
			Double_tap_lag = false;

		}
		engine_prediction.end_prediction();
		g_Misc.edge_jump_post_prediction(pCmd);
		
		if (g::pLocalEntity->IsAlive() && !Double_tap_lag)
		{
			AntiAimPro.fakelag(g::pLocalEntity, pCmd, g::bSendPacket);

		}

		if (pCmd->buttons & IN_ATTACK)
		{
			AntiAimPro.shot_cmd = pCmd->command_number;
		}
		AntiAimNEW.FakeWalk(pCmd);
		//AntiAimNEW.do_antiaim(pCmd);
		//AntiAimNEW.fake_crouch();
		//AntiAimPro.predict(g::pLocalEntity, pCmd);

		AntiAimPro.run(g::pLocalEntity, pCmd, g::bSendPacket);
		if (AntiAimPro.is_fakeducking)
		{
			if (g_pClientState->choked_commands > 6)
				pCmd->buttons |= IN_DUCK;
			else
				pCmd->buttons &= ~IN_DUCK;
		}
		OnCreateMove(pCmd, g::bSendPacket);
	

		g_Misc.OnCreateMoveRankReveal(pCmd);
		//FixMoveManager->Stop(pCmd);

		g_Misc.MovementFix(g::OriginalView);

		if (g_Options.misc.ant_untrusted)
		{
			g_Math.Clamp(g::pCmd->viewangles);
		}


		g::ThirdPerson = g::pCmd->viewangles;

		g::fake_strength = GetDes32yncDeltaNEW();

		if (!g::bSendPacket)
		{

			g::RealAngle = g::pCmd->viewangles;

			if (g_Options.antiaa.enable)
			{
				auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
				if (!local_player)
					return;
				auto animations = local_player->AnimState();

				if (!animations)
					return;

				if (animations->m_bInHitGroundAnimation)
					g::RealAngle.x = -12.0f;

			}

		}
		if (g::bSendPacket)
		{
			g::FakeAngle = g::pCmd->viewangles;

			
		}
		

		if (g_Options.antiaa.infinite_duck)
		{
			g::pCmd->buttons |= IN_BULLRUSH; // hehe

		}
	}
	

	*SendPacket = g::bSendPacket;


   

	

    return false;
}

bool target_meets_requirements(C_BaseEntity* e)
{
	if (e && !e->IsDormant())
		return true;
	else
		return false;
}
void c_material_var::set_vector(const Vector vector)
{
	set_vector_internal(vector.x, vector.y);
}

void c_material_var::set_vector123(const Vector vector)
{
	set_vector_internal(vector.x, vector.y, vector.z);
}
#define clamp(val, min, max) (((val) > (max)) ? (max) : (((val) < (min)) ? (min) : (val)))

void modulate(const Color color, IMaterial* material)
{
	if (material)
	{
		material->modulate(color);

		const auto tint = material->find_var(_("$envmaptint"));
		if (tint)
			tint->set_vector123(Vector(color.r() / 255.f, color.g() / 255.f, color.b() / 255.f));
	}

//	render_view()->set_color_modulation(color);
	g_pRenderView->set_color_modulation(color);

}
void run_chams()
{

	

	if (g::pLocalEntity)
	{
		for (int i = 1; i <= g_pGlobalVars->maxClients; i++) 
		{
			C_BaseEntity* entity = g_pEntityList->GetClientEntity(i);

			if (!target_meets_requirements(entity))
				continue;

			if (entity->GetClientClass()->ClassID != (int)ClassId::CCSPlayer)
				continue;

			if (entity->GetTeam() != g::pLocalEntity->GetTeam())
			{
				if (g_Options.cmaterials.enemy.backtrack)
				{
					if (g_Options.legit.backtrack)
					{
						static IMaterial* material;
						if (g_Options.cmaterials.enemy.material == 0)
						{
							material = g_pMaterialSys->FindMaterial("chams", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.enemy.material == 1)
						{
							material = g_pMaterialSys->FindMaterial("Metallic", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.enemy.material == 2)
						{
							material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.enemy.material == 3)
						{
							material = g_pMaterialSys->FindMaterial("FlatChamies", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.enemy.material == 4)
						{
							//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

							material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/velvet", "Other textures", true, NULL);

						}

						if (g_Options.cmaterials.enemy.material == 5)
						{
							//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

							material = g_pMaterialSys->FindMaterial("models/inventory_items/dogtags/dogtags_outline", "Other textures", true, NULL);

						}
						if (g_Options.cmaterials.enemy.material == 6)
						{
							//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

							material = g_pMaterialSys->FindMaterial("dev/glow_armsrace", "Other textures", true, NULL);

						}

						//static IMaterial* material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER, true, NULL);

						if (!material)
							return;

						if (material->IsErrorMaterial())
							return;

						if (backtracking->IsTickValid(backtracking->records[i].tick_count))
						{
							float fl_color1[3] = { 0.f };
							Color color_invis1 = Color(materials::colors::enemy::backtrack);

							fl_color1[0] = color_invis1.r() / 255.f;
							fl_color1[1] = color_invis1.g() / 255.f;
							fl_color1[2] = color_invis1.b() / 255.f;
							if (g_Options.cmaterials.enemy.material == 6)
							{

								modulate(color_invis1, material);
							}
							g_pRenderView->SetColorModulation(fl_color1);
							g_pModelRender->ForcedMaterialOverride(material);
							material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							material->IncrementReferenceCount();
							material->AlphaModulate(g_Options.cmaterials.enemy.chams_opactiy/ 100);
							entity->SetAbsOrigin(headPositions[i][backtracking->records[i].tick_count].origin);
							entity->DrawModel(0x1, 255);
							g_pModelRender->ForcedMaterialOverride(nullptr);
							entity->SetAbsOrigin(entity->GetOrigin());
						}

					}
					else
					{
							
						static IMaterial* material;
						if (g_Options.cmaterials.enemy.material == 0)
						{
							material = g_pMaterialSys->FindMaterial("chams", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.enemy.material == 1)
						{
							material = g_pMaterialSys->FindMaterial("Metallic", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.enemy.material == 2)
						{
							material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.enemy.material == 3)
						{
							material = g_pMaterialSys->FindMaterial("FlatChamies", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.enemy.material == 4)
						{
							//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

							material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/velvet", "Other textures", true, NULL);

						}

						if (g_Options.cmaterials.enemy.material == 5)
						{
							//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

							material = g_pMaterialSys->FindMaterial("models/inventory_items/dogtags/dogtags_outline", "Other textures", true, NULL);

						}
						if (g_Options.cmaterials.enemy.material == 6)
						{
							//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

							material = g_pMaterialSys->FindMaterial("dev/glow_armsrace", "Other textures", true, NULL);

						}

						//static IMaterial* material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER, true, NULL);

						if (!material)
							return;

						if (material->IsErrorMaterial())
							return;

						

						if (g_LagComp.PlayerRecord[entity->EntIndex()].size() > 0)
						{
							PlayerRecords pPlayerEntityRecord = g_LagComp.PlayerRecord[entity->EntIndex()].at(0);

							if (g_LagComp.valid_simtime((pPlayerEntityRecord.SimTime)))
							{
								Vector orig = entity->GetAbsOrigin();



								float fl_color1[3] = { 0.f };
								Color color_invis1 = Color(materials::colors::enemy::backtrack);

								fl_color1[0] = color_invis1.r() / 255.f;
								fl_color1[1] = color_invis1.g() / 255.f;
								fl_color1[2] = color_invis1.b() / 255.f;
								if (g_Options.cmaterials.enemy.material == 6)
								{

									modulate(color_invis1, material);
								}
								entity->SetAbsOrigin(pPlayerEntityRecord.m_vecOrigin);
								g_pModelRender->ForcedMaterialOverride(material);
								material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
								material->IncrementReferenceCount();
								material->AlphaModulate(g_Options.cmaterials.enemy.chams_opactiy / 100);
								g_pRenderView->SetColorModulation(fl_color1);
								entity->DrawModel(0x1, 255);
								entity->SetAbsOrigin(orig);
								g_pModelRender->ForcedMaterialOverride(nullptr);
							}

						}
				
								
							
						

							
					}
					//



				}
				if (g_Options.cmaterials.enemy.behind_wall)
				{

					static IMaterial* material;
					if (g_Options.cmaterials.enemy.material == 0)
					{
						material = g_pMaterialSys->FindMaterial("chams", "Model textures", true, NULL);

					}
					if (g_Options.cmaterials.enemy.material == 1)
					{
						material = g_pMaterialSys->FindMaterial("Metallic", "Model textures", true, NULL);

					}
					if (g_Options.cmaterials.enemy.material == 2)
					{
						material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

					}
					if (g_Options.cmaterials.enemy.material == 3)
					{
						material = g_pMaterialSys->FindMaterial("FlatChamies", "Model textures", true, NULL);

					}
					if (g_Options.cmaterials.enemy.material == 4)
					{
						//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

						material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/velvet", "Other textures", true, NULL);

					}
					if (g_Options.cmaterials.enemy.material == 5)
					{
						//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

						material = g_pMaterialSys->FindMaterial("models/inventory_items/dogtags/dogtags_outline", "Other textures", true, NULL);

					}
					if (g_Options.cmaterials.enemy.material == 6)
					{
						//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

						material = g_pMaterialSys->FindMaterial("dev/glow_armsrace", "Other textures", true, NULL);

					}
					//static IMaterial* material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER, true, NULL);

					if (!material)
						return;

					if (material->IsErrorMaterial())
						return;

					float fl_color4[3] = { 0.f };
					Color color_invis13 = Color(materials::colors::enemy::behind_wall);

					fl_color4[0] = color_invis13.r() / 255.f;
					fl_color4[1] = color_invis13.g() / 255.f;
					fl_color4[2] = color_invis13.b() / 255.f;

					if (g_Options.cmaterials.enemy.material == 6)
					{

						modulate(color_invis13, material);
					}
					g_pModelRender->ForcedMaterialOverride(material);
					material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
					material->IncrementReferenceCount();
					material->AlphaModulate(g_Options.cmaterials.enemy.chams_opactiy / 100);
					g_pRenderView->SetColorModulation(fl_color4);
					entity->DrawModel(0x1, 255);
					g_pModelRender->ForcedMaterialOverride(nullptr);
				}
				if (g_Options.cmaterials.enemy.player)
				{
					static IMaterial* material;
					if (g_Options.cmaterials.enemy.material == 0)
					{
						material = g_pMaterialSys->FindMaterial("chams", "Model textures", true, NULL);

					}
					if (g_Options.cmaterials.enemy.material == 1)
					{
						material = g_pMaterialSys->FindMaterial("Metallic", "Model textures", true, NULL);

					}
					if (g_Options.cmaterials.enemy.material == 2)
					{
						material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

					}
					if (g_Options.cmaterials.enemy.material == 3)
					{
						material = g_pMaterialSys->FindMaterial("FlatChamies", "Model textures", true, NULL);

					}
					if (g_Options.cmaterials.enemy.material == 4)
					{
						//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

						material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/velvet", "Other textures", true, NULL);

					}

					if (g_Options.cmaterials.enemy.material == 5)
					{
						//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

						material = g_pMaterialSys->FindMaterial("models/inventory_items/dogtags/dogtags_outline", "Other textures", true, NULL);

					}
					if (g_Options.cmaterials.enemy.material == 6)
					{
						//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

						material = g_pMaterialSys->FindMaterial("dev/glow_armsrace", "Other textures", true, NULL);

					}

					//static IMaterial* material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER, true, NULL);

					if (!material)
						return;

					if (material->IsErrorMaterial())
						return;

					float fl_color1[3] = { 0.f };
					Color color_invis1 = Color( materials::colors::enemy::player);

					fl_color1[0] = color_invis1.r() / 255.f;
					fl_color1[1] = color_invis1.g() / 255.f;
					fl_color1[2] = color_invis1.b() / 255.f;

					if (g_Options.cmaterials.enemy.material == 6)
					{

						modulate(color_invis1, material);
					}
					g_pModelRender->ForcedMaterialOverride(material);
					material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					material->IncrementReferenceCount();
					material->AlphaModulate(g_Options.cmaterials.enemy.chams_opactiy / 100);
					g_pRenderView->SetColorModulation(fl_color1);
					entity->DrawModel(0x1, 255);
					g_pModelRender->ForcedMaterialOverride(nullptr);
				}
			}
			else
			{
				if (entity != g::pLocalEntity)
				{
					if (g_Options.cmaterials.team.player)
					{
						static IMaterial* material;
						if (g_Options.cmaterials.team.material == 0)
						{
							material = g_pMaterialSys->FindMaterial("chams", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.team.material == 1)
						{
							material = g_pMaterialSys->FindMaterial("Metallic", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.team.material == 2)
						{
							material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.team.material == 3)
						{
							material = g_pMaterialSys->FindMaterial("FlatChamies", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.team.material == 4)
						{
							//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

							material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/velvet", "Other textures", true, NULL);

						}

						if (g_Options.cmaterials.team.material == 5)
						{
							//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

							material = g_pMaterialSys->FindMaterial("models/inventory_items/dogtags/dogtags_outline", "Other textures", true, NULL);

						}
						if (g_Options.cmaterials.team.material == 6)
						{
							//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

							material = g_pMaterialSys->FindMaterial("dev/glow_armsrace", "Other textures", true, NULL);

						}

						//static IMaterial* material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER, true, NULL);

						if (!material)
							return;

						if (material->IsErrorMaterial())
							return;


						float fl_color23231[3] = { 0.f };
						Color color_QWWQinvis1 = Color( materials::colors::team::behind_wall);

						fl_color23231[0] = color_QWWQinvis1.r() / 255.f;
						fl_color23231[1] = color_QWWQinvis1.g() / 255.f;
						fl_color23231[2] = color_QWWQinvis1.b() / 255.f;

						if (g_Options.cmaterials.team.material == 6)
						{

							modulate(color_QWWQinvis1, material);
						}
						g_pModelRender->ForcedMaterialOverride(material);
						material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						material->IncrementReferenceCount();
						material->AlphaModulate(g_Options.cmaterials.team.chams_opactiy / 100);
						g_pRenderView->SetColorModulation(fl_color23231);
						entity->DrawModel(0x1, 255);
						g_pModelRender->ForcedMaterialOverride(nullptr);
					}
					if (g_Options.cmaterials.team.player)
					{
						static IMaterial* material;
						if (g_Options.cmaterials.team.material == 0)
						{
							material = g_pMaterialSys->FindMaterial("chams", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.team.material == 1)
						{
							material = g_pMaterialSys->FindMaterial("Metallic", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.team.material == 2)
						{
							material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.team.material == 3)
						{
							material = g_pMaterialSys->FindMaterial("FlatChamies", "Model textures", true, NULL);

						}
						if (g_Options.cmaterials.team.material == 4)
						{
							//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

							material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/velvet", "Other textures", true, NULL);

						}

						if (g_Options.cmaterials.team.material == 5)
						{
							//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

							material = g_pMaterialSys->FindMaterial("models/inventory_items/dogtags/dogtags_outline", "Other textures", true, NULL);

						}
						if (g_Options.cmaterials.team.material == 6)
						{
							//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

							material = g_pMaterialSys->FindMaterial("dev/glow_armsrace", "Other textures", true, NULL);

						}

						//static IMaterial* material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER, true, NULL);

						if (!material)
							return;

						if (material->IsErrorMaterial())
							return;


						float fl_col344or23231[3] = { 0.f };
						Color color343_QWWQinvis1 = Color(materials::colors::team::player);

						fl_col344or23231[0] = color343_QWWQinvis1.r() / 255.f;
						fl_col344or23231[1] = color343_QWWQinvis1.g() / 255.f;
						fl_col344or23231[2] = color343_QWWQinvis1.b() / 255.f;
						if (g_Options.cmaterials.team.material == 6)
						{

							modulate(color343_QWWQinvis1, material);
						}
						g_pModelRender->ForcedMaterialOverride(material);
						material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						material->IncrementReferenceCount();
						material->AlphaModulate(g_Options.cmaterials.team.chams_opactiy / 100);
						g_pRenderView->SetColorModulation(fl_col344or23231);
						entity->DrawModel(0x1, 255);
						g_pModelRender->ForcedMaterialOverride(nullptr);
					}

				}

			}
				

			if (g_Options.cmaterials.local.chams && entity == g::pLocalEntity)
			{
                if (!g::pLocalEntity && g::pLocalEntity->IsAlive())
                    return;
				static IMaterial* material;
				if (g_Options.cmaterials.local.material == 0)
				{
					material = g_pMaterialSys->FindMaterial("chams", "Model textures", true, NULL);

				}
				if (g_Options.cmaterials.local.material == 1)
				{
					material = g_pMaterialSys->FindMaterial("Metallic", "Model textures", true, NULL);

				}
				if (g_Options.cmaterials.local.material == 2)
				{
					material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

				}
				if (g_Options.cmaterials.local.material == 3)
				{
					material = g_pMaterialSys->FindMaterial("FlatChamies", "Model textures", true, NULL);

				}
				if (g_Options.cmaterials.local.material == 4)
				{
					//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

					material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/velvet", "Other textures", true, NULL);

				}

				if (g_Options.cmaterials.local.material == 5)
				{
					//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

					material = g_pMaterialSys->FindMaterial("models/inventory_items/dogtags/dogtags_outline", "Other textures", true, NULL);

				}
				if (g_Options.cmaterials.local.material == 6)
				{
					//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

					material = g_pMaterialSys->FindMaterial("dev/glow_armsrace", "Other textures", true, NULL);

				}
				//static IMaterial* material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER, true, NULL);

				if (!material)
					return;

				if (material->IsErrorMaterial())
					return;

				float fl_color[3] = { 0.f };
				Color color_invis = Color(materials::colors::local::chams);

				fl_color[0] = color_invis.r() / 255.f;
				fl_color[1] = color_invis.g() / 255.f;
				fl_color[2] = color_invis.b() / 255.f;

				if (g_Options.cmaterials.local.material == 3)
				{
					material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

				}
				else
				{
					material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

				}
				if (g_Options.cmaterials.local.material == 6)
				{

					modulate(color_invis, material);
				}
				material->IncrementReferenceCount();
				g_pModelRender->ForcedMaterialOverride(material);
				material->AlphaModulate(g_Options.cmaterials.local.chams_opactiy / 100);
				g_pRenderView->SetColorModulation(fl_color);
				g::pLocalEntity->DrawModel(0x1, 255);
				g_pModelRender->ForcedMaterialOverride(nullptr);
			}


			if (g_Options.cmaterials.local.desync && entity == g::pLocalEntity)
			{
				static IMaterial* material;
				if (g_Options.cmaterials.local.material == 0)
				{
					material = g_pMaterialSys->FindMaterial("chams", "Model textures", true, NULL);

				}
				if (g_Options.cmaterials.local.material == 1)
				{
					material = g_pMaterialSys->FindMaterial("Metallic", "Model textures", true, NULL);

				}
				if (g_Options.cmaterials.local.material == 2)
				{
					material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

				}
				if (g_Options.cmaterials.local.material == 3)
				{
					material = g_pMaterialSys->FindMaterial("FlatChamies", "Model textures", true, NULL);

				}
				if (g_Options.cmaterials.local.material == 4)
				{
					//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

					material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/velvet", "Other textures", true, NULL);

				}
				if (g_Options.cmaterials.local.material == 5)
				{
					//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

					material = g_pMaterialSys->FindMaterial("models/inventory_items/dogtags/dogtags_outline", "Other textures", true, NULL);

				}
				if (g_Options.cmaterials.local.material == 6)
				{
					//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

					material = g_pMaterialSys->FindMaterial("dev/glow_armsrace", "Other textures", true, NULL);

				}
				//static IMaterial* material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER, true, NULL);

				if (!material)
					return;

				if (material->IsErrorMaterial())
					return;

				float fl_color[3] = { 0.f };
				Color color_invis = Color(materials::colors::local::desync);

				fl_color[0] = color_invis.r() / 255.f;
				fl_color[1] = color_invis.g() / 255.f;
				fl_color[2] = color_invis.b() / 255.f;

				if (g_Options.cmaterials.local.material == 3)
				{
					material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

				}
				else
				{
					material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

				}			
				if (g_Options.cmaterials.local.material == 6)
				{

					modulate(color_invis, material);
				}
				material->IncrementReferenceCount();
				g_pModelRender->ForcedMaterialOverride(material);
				material->AlphaModulate(g_Options.cmaterials.local.chams_opactiy / 100);
				g_pRenderView->SetColorModulation(fl_color);
				g::pLocalEntity->setAbsAngle(Vector(0, g::FakeAngle.y, 0));
				g::pLocalEntity->DrawModel(0x1, 255);
				g_pModelRender->ForcedMaterialOverride(nullptr);
			}
			
			if (g_Options.cmaterials.local.fakelag && g::pLocalEntity->GetVelocity().Length2D() > 30 && entity == g::pLocalEntity)
			{
				static IMaterial* material;
					
				material = g_pMaterialSys->FindMaterial("models/inventory_items/dogtags/dogtags_outline", "Other textures", true, NULL);

					

				//static IMaterial* material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER, true, NULL);

				if (!material)
					return;

				if (material->IsErrorMaterial())
					return;

				float fl_color[3] = { 0.f };
				Color color_invis = Color(materials::colors::local::fakelag);

				fl_color[0] = color_invis.r() / 255.f;
				fl_color[1] = color_invis.g() / 255.f;
				fl_color[2] = color_invis.b() / 255.f;

				static auto last_networked_origin = Vector(0, 0, 0);
				if (g::bSendPacket)
					last_networked_origin = g::pLocalEntity->GetOrigin();

				g::pLocalEntity->set_abs_origin(last_networked_origin);

				static auto alpha = 0.f; static auto plus_or_minus = false;
				if (alpha <= 0.f || alpha >= 255.f)
					plus_or_minus = !plus_or_minus;
				alpha += plus_or_minus ? (255.f / 7 * 0.015) : -(255.f / 7 * 0.015);
				alpha = clamp(alpha, 0.f, 255.f);
				material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

					
				material->IncrementReferenceCount();
				g_pModelRender->ForcedMaterialOverride(material);
				material->AlphaModulate(100 / 100);
				g_pRenderView->SetColorModulation(fl_color);
				g_pRenderView->SetBlend(1.f);
				g::pLocalEntity->DrawModel(0x1, 255);
				g_pModelRender->ForcedMaterialOverride(nullptr);




			}

		}
	}


	

}
#define TEXTURE_GROUP_SKYBOX						          "SkyBox textures"

void run_legitaa_chams()
{


	if (g_Options.legit.legit_aa)
	{
		if (g::pLocalEntity)
		{
			for (int i = 1; i <= g_pGlobalVars->maxClients; i++)
			{
				C_BaseEntity* entity = g_pEntityList->GetClientEntity(i);

				if (!target_meets_requirements(entity))
					continue;

				if (entity->GetClientClass()->ClassID != (int)ClassId::CCSPlayer)
					continue;


				if (g_Options.legit.legit_aa_visualize && entity == g::pLocalEntity)
				{
					static IMaterial* material;
					if (g_Options.legit.material == 0)
					{
						material = g_pMaterialSys->FindMaterial("chams", "Model textures", true, NULL);

					}
					if (g_Options.legit.material == 1)
					{
						material = g_pMaterialSys->FindMaterial("Metallic", "Model textures", true, NULL);

					}
					if (g_Options.legit.material == 2)
					{
						material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

					}
					if (g_Options.legit.material == 3)
					{
						material = g_pMaterialSys->FindMaterial("FlatChamies", "Model textures", true, NULL);

					}
					if (g_Options.legit.material == 4)
					{
						//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

						material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/velvet", "Other textures", true, NULL);

					}
					if (g_Options.legit.material == 5)
					{
						//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

						material = g_pMaterialSys->FindMaterial("models/inventory_items/dogtags/dogtags_outline", "Other textures", true, NULL);

					}

					//static IMaterial* material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER, true, NULL);

					if (!material)
						return;

					if (material->IsErrorMaterial())
						return;

					float fl_color[3] = { 0.f };
					Color color_invis = Color(legit::colors::legit_aa_visualize_color);

					fl_color[0] = color_invis.r() / 255.f;
					fl_color[1] = color_invis.g() / 255.f;
					fl_color[2] = color_invis.b() / 255.f;

					if (g_Options.legit.material == 3)
					{
						material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

					}
					else
					{
						material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

					}
					material->IncrementReferenceCount();
					g_pModelRender->ForcedMaterialOverride(material);
					material->AlphaModulate(100);
					g_pRenderView->SetColorModulation(fl_color);
					g::pLocalEntity->setAbsAngle(Vector(0, g::FakeAngle.y, 0));
					g::pLocalEntity->DrawModel(0x1, 255);
					g_pModelRender->ForcedMaterialOverride(nullptr);
				}

			}
		}

	}




}
IMaterial* create_material(bool shade, bool wireframe, bool ignorez, int rimlight_boost = 0) {
	static const std::string material_name = "glowOverlay.vmt";
	const std::string material_type = shade ? "VertexLitGeneric" : "UnlitGeneric";

	std::string material_data;

	material_data += "\"" + material_type + "\"\n{\n";
	material_data += "\t\"$additive\" \"1\"\n";
	material_data += "\t\"$envmap\" \"models/effects/cube_white\"\n";
	material_data += "\t\"$envmaptint\" \"[0 0.1 0.2]\"\n";
	material_data += "\t\"$envmapfresnel\" \"1\"\n";
	material_data += "\t\"$envmapfresnelminmaxexp\" \"[0 1 2]\"\n";
	material_data += "\t\"$alpha\" \"0.8\"\n";
	material_data += "}\n";

	auto init_key_values = [](KeyValuesv2* keyvalues, const char* key_name) -> void {
		using InitKeyValues_t = void(__thiscall*)(void*, const char*);
		static InitKeyValues_t InitKeyValuesEx = nullptr;
		static auto pThis = *reinterpret_cast<DWORD**>(Utils::FindSignature("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);
		if (!InitKeyValuesEx)
			InitKeyValuesEx = *reinterpret_cast<InitKeyValues_t>(Utils::FindSignature("client_panorama.dll", "55 8B EC 51 33 C0 C7 45"));

		InitKeyValuesEx(keyvalues, key_name);
	};
	auto load_from_buffer = [](KeyValuesv2* key_values, const char* resource_name, const char* buf, void* file_sys = nullptr, const char* path_id = nullptr, void* eval_sym_proc = nullptr, void* unk = nullptr) -> void {
		using LoadFromBuffer_t = void(__thiscall*)(void*, const char*, const char*, void*, const char*, void*, void*);
		static LoadFromBuffer_t LoadFromBufferEx = nullptr;

		if (!LoadFromBufferEx)
			LoadFromBufferEx = *reinterpret_cast<LoadFromBuffer_t>(Utils::FindSignature("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89"));

		LoadFromBufferEx(key_values, resource_name, buf, file_sys, path_id, eval_sym_proc, unk);
	};

	auto* key_values = new KeyValuesv2();
	init_key_values(key_values, material_type.c_str());
	load_from_buffer(key_values, material_name.c_str(), material_data.c_str());

	return g_pMaterialSys->CreateMaterial(material_name.c_str(), key_values);
}



void __fastcall Hooks::SceneEnd(void *ecx, void *edx)
{
	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{
		if (g_pEngine->IsInGame())
		{
			if (g_pEngine->IsConnected())
			{

				for (auto i = 0; i < m_pGlowObjManager->size; i++)
				{
					auto glow_object = &m_pGlowObjManager->m_GlowObjectDefinitions[i];
					C_BaseEntity *m_entity = glow_object->m_pEntity;
					if (!glow_object->m_pEntity || glow_object->IsUnused() || !g::pLocalEntity)
						continue;
					auto list = g_Options.esp.effects.world_selected;

					if (list[0])
					{
					
						if (m_entity->GetClientClass()->ClassID == 1 || m_entity->GetClientClass()->ClassID == 66 || m_entity->GetClientClass()->ClassID == 87 || m_entity->GetClientClass()->ClassID == 98 || m_entity->GetClientClass()->ClassID == 108 || m_entity->GetClientClass()->ClassID == 134 || m_entity->GetClientClass()->ClassID == 1 || m_entity->GetClientClass()->ClassID == 9 || m_entity->GetClientClass()->ClassID == 29 || m_entity->GetClientClass()->ClassID == 39 || m_entity->GetClientClass()->ClassID == 41 || strstr(m_entity->GetClientClass()->pNetworkName, "Weapon"))
						{
							
							float m_flRed = esp::colors::world_glow[0], m_flGreen = esp::colors::world_glow[1], m_flBlue = esp::colors::world_glow[2];
							glow_object->m_vGlowColor = Vector(m_flRed / 255, m_flGreen / 255, m_flBlue / 255);
							glow_object->m_flGlowAlpha = 100;
							glow_object->m_bRenderWhenOccluded = true;
							glow_object->m_bRenderWhenUnoccluded = false;



						}
					}
					if (list[1])
					{

						if (m_entity->GetClientClass()->ClassID == ClassId::CDecoyGrenade || m_entity->GetClientClass()->ClassID == ClassId::CHEGrenade || m_entity->GetClientClass()->ClassID == ClassId::CIncendiaryGrenade || m_entity->GetClientClass()->ClassID == ClassId::CMolotovGrenade || m_entity->GetClientClass()->ClassID == ClassId::CSmokeGrenade || m_entity->GetClientClass()->ClassID == ClassId::CFlashbang || m_entity->GetClientClass()->ClassID == ClassId::CDecoyProjectile || m_entity->GetClientClass()->ClassID == ClassId::CBaseCSGrenadeProjectile || m_entity->GetClientClass()->ClassID == ClassId::CMolotovProjectile || m_entity->GetClientClass()->ClassID == ClassId::CSmokeGrenadeProjectile || m_entity->GetClientClass()->ClassID == ClassId::CFlashbang)
						{

                            float m_flRed = esp::colors::world_glow[0], m_flGreen = esp::colors::world_glow[1], m_flBlue = esp::colors::world_glow[2];
                            glow_object->m_vGlowColor = Vector(m_flRed / 255, m_flGreen / 255, m_flBlue / 255);
							glow_object->m_flGlowAlpha = 100;
							glow_object->m_bRenderWhenOccluded = true;
							glow_object->m_bRenderWhenUnoccluded = false;



						}
					}
					if (list[2])
					{

						if (m_entity->GetClientClass()->ClassID == ClassId::CC4 || m_entity->GetClientClass()->ClassID == ClassId::CPlantedC4)
						{

                            float m_flRed = esp::colors::world_glow[0], m_flGreen = esp::colors::world_glow[1], m_flBlue = esp::colors::world_glow[2];
                            glow_object->m_vGlowColor = Vector(m_flRed / 255, m_flGreen / 255, m_flBlue / 255);
							glow_object->m_flGlowAlpha = 100;
							glow_object->m_bRenderWhenOccluded = true;
							glow_object->m_bRenderWhenUnoccluded = false;



						}
					}
					if (m_entity->GetClientClass()->ClassID == 40)
					{
						
						if (g_Options.esp.effects.local_glow)
						{
							if (m_entity == g::pLocalEntity)
							{
								if (g::pLocalEntity->IsAlive())
								{
									float m_flRed = esp::colors::local_glow[0], m_flGreen = esp::colors::local_glow[1], m_flBlue = esp::colors::local_glow[2];
									glow_object->m_vGlowColor = Vector(m_flRed / 255, m_flGreen / 255, m_flBlue / 255);
									glow_object->m_flGlowAlpha = 100;
									glow_object->m_bRenderWhenOccluded = true;
									glow_object->m_bRenderWhenUnoccluded = false;
								}
								
							}

						}
						if (g_Options.esp.effects.enemy_glow)
						{
							if (m_entity->GetTeam() != g::pLocalEntity->GetTeam())
							{

								float m_flRed = esp::colors::enemy_glow[0], m_flGreen = esp::colors::enemy_glow[1], m_flBlue = esp::colors::enemy_glow[2];

								glow_object->m_vGlowColor = Vector(m_flRed / 255, m_flGreen / 255, m_flBlue / 255);
								glow_object->m_flGlowAlpha = 100;
								glow_object->m_bRenderWhenOccluded = true;
								glow_object->m_bRenderWhenUnoccluded = false;
							}
						}
						if (g_Options.esp.effects.team_glow)
						{
							if (m_entity->GetTeam() == g::pLocalEntity->GetTeam() && m_entity != g::pLocalEntity)
							{
								float m_flRed = esp::colors::team_glow[0], m_flGreen = esp::colors::team_glow[1], m_flBlue = esp::colors::team_glow[2];

								glow_object->m_vGlowColor = Vector(m_flRed / 255, m_flGreen / 255, m_flBlue / 255);
								glow_object->m_flGlowAlpha = 100;
								glow_object->m_bRenderWhenOccluded = true;
								glow_object->m_bRenderWhenUnoccluded = false;
							}
						}
					}
					/*
					std::vector<dropdownboxitem> list = Options::Menu.VisualsTab.m_world_glow.items;

					
					const auto classid = glow_object->m_pEntity->GetClientClass()->ClassID;
					if (list[0].GetSelected)
					{
						if (is_weapon(classid))
						{
							if (glow_object->m_pEntity->owner() != -1)
								return;

							
							float m_flRed = Options::Menu.VisualsTab.m_world_glow_col.GetValue()[0], m_flGreen = Options::Menu.VisualsTab.m_world_glow_col.GetValue()[1], m_flBlue = Options::Menu.VisualsTab.m_world_glow_col.GetValue()[2];
							glow_object->m_vGlowColor = Vector(m_flRed / 255, m_flGreen / 255, m_flBlue / 255);
							glow_object->m_flGlowAlpha = 100 / 100;
							glow_object->m_bRenderWhenOccluded = true;
							glow_object->m_bRenderWhenUnoccluded = false;
								

							
						}
					}
					
					if (list[1].GetSelected)
					{
						if (is_grenade(classid))
						{
						


							float m_flRed = Options::Menu.VisualsTab.m_world_glow_col.GetValue()[0], m_flGreen = Options::Menu.VisualsTab.m_world_glow_col.GetValue()[1], m_flBlue = Options::Menu.VisualsTab.m_world_glow_col.GetValue()[2];
							glow_object->m_vGlowColor = Vector(m_flRed / 255, m_flGreen / 255, m_flBlue / 255);
							glow_object->m_flGlowAlpha = 100 / 100;
							glow_object->m_bRenderWhenOccluded = true;
							glow_object->m_bRenderWhenUnoccluded = false;



						}
					}*/
					

				}

				run_legitaa_chams();
				run_chams();
				/*
				for (int i = 1; i < g_pEngine->GetMaxClients(); ++i)
				{
					C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(i);

					if (!pPlayerEntity
						|| !pPlayerEntity->IsAlive()
						|| pPlayerEntity->IsDormant()
						|| !g::pLocalEntity)
						continue;

					bool IsTeammate = pPlayerEntity->GetTeam() == g::pLocalEntity->GetTeam();
					bool IsEnemy = pPlayerEntity->GetTeam() != g::pLocalEntity->GetTeam();
					bool IsLocal = pPlayerEntity == g::pLocalEntity;


					static std::vector< IMaterial* > materials;
					static IMaterial* mat_model;

					static bool once{ false };
					if (!once) {
						materials.push_back(create_material(true, false, false)); // material.
					

						mat_model = g_pMaterialSys->FindMaterial("csgo/materials/glowOverlay", nullptr);
						once = true;
					}

					
					if (IsEnemy)
					{
					
						if (g_Options.cmaterials.enemy.player)
						{
							if (g_Options.cmaterials.enemy.material == 7)
							{
								g_pModelRender->ForcedMaterialOverride(mat_model);
								mat_model->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
								mat_model->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
								pPlayerEntity->DrawModel(0x00000001, 255);

								g_pModelRender->ForcedMaterialOverride(materials.at(0));
								materials.at(0)->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
								materials.at(0)->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
								pPlayerEntity->DrawModel(0x00000001, 255);
							}
						}
					}
					if (IsTeammate && !IsLocal)
					{
						if (g_Options.cmaterials.team.player)
						{
							if (g_Options.cmaterials.team.material == 7)
							{
								g_pModelRender->ForcedMaterialOverride(mat_model);
								mat_model->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
								mat_model->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
								pPlayerEntity->DrawModel(0x00000001, 255);

								g_pModelRender->ForcedMaterialOverride(materials.at(0));
								materials.at(0)->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
								materials.at(0)->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
								pPlayerEntity->DrawModel(0x00000001, 255);
							}
						}
					}
					if (IsLocal)
					{
						if (g_Options.cmaterials.local.chams)
						{
							if (g_Options.cmaterials.other.hand_chams_material == 7)
							{
								g_pModelRender->ForcedMaterialOverride(mat_model);
								mat_model->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
								mat_model->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
								pPlayerEntity->DrawModel(0x00000001, 255);

								g_pModelRender->ForcedMaterialOverride(materials.at(0));
								materials.at(0)->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
								materials.at(0)->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
								pPlayerEntity->DrawModel(0x00000001, 255);
							}
						}
					}

					

					g_pModelRender->ForcedMaterialOverride(nullptr);
				}
				*/


				
					
				
			}
		}
	}
	oSceneEnd_t_t->GetTrampoline()(ecx, edx);

}

void __fastcall Hooks::DoExtraBonesProcessing(void * ECX, void * EDX, void * unkn1, void * unkn2, void * unkn3, void * unkn4, CBoneBitList & unkn5, void * unkn6)
{
	C_BaseEntity* pPlayerEntity = (C_BaseEntity*)ECX;

	if (!pPlayerEntity || pPlayerEntity == nullptr)
		return;

	if (!pPlayerEntity->IsAlive() || pPlayerEntity->IsDormant())
		return;

	if (!pPlayerEntity->AnimState())
		return;

	auto oDoExtraBonesProcessing = g_Hooks.pPlayerHook[pPlayerEntity->EntIndex()]->GetOriginal<ExtraBoneProcess_t>(vtable_indexes::extraBonePro);



	oDoExtraBonesProcessing(ECX, unkn1, unkn2, unkn3, unkn4, unkn5, unkn6);

}

void __fastcall Hooks::DrawModelExecute(void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* matrix)
{
	
		const char* ModelName = g_pModelInfo->GetModelName((model_t*)info.pModel);

		C_BaseEntity* pPlayerEntity = g_pEntityList->GetClientEntity(info.index);
		auto pLocal = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

		bool isplayer = strstr(ModelName, "models/player") != nullptr;
		auto model_entity = static_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(info.index));
		if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
		{
			if (pLocal->IsScoped() && isplayer && model_entity != nullptr && model_entity->GetTeam() == pLocal->GetTeam() && model_entity == pLocal)
			{
				if (pLocal->IsAlive())
					g_pRenderView->SetBlend(g_Options.esp.other.scoped_alpha * 0.01f);
			}
		}
		if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
		{
			if (g_Options.cmaterials.other.hand_chams && strstr(ModelName, "arms") && pLocal && pLocal->IsAlive() && g_pEngine->IsInGame() && g_pEngine->IsConnected())
			{


				float flColor[3] = { 0.f };
				Color color_invis13 = Color(materials::colors::local::hand_chams);

				flColor[0] = color_invis13.r() / 255.f;
				flColor[1] = color_invis13.g() / 255.f;
				flColor[2] = color_invis13.b() / 255.f;

				static IMaterial* mat;
				if (g_Options.cmaterials.other.hand_chams_material == 0)
				{
					mat = g_pMaterialSys->FindMaterial("chams", "Model textures", true, NULL);
					mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);

				}
				if (g_Options.cmaterials.other.hand_chams_material == 1)
				{
					mat = g_pMaterialSys->FindMaterial("Metallic", "Model textures", true, NULL);
					mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
				}
				if (g_Options.cmaterials.other.hand_chams_material == 2)
				{
					mat = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);
					mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
				}
				if (g_Options.cmaterials.other.hand_chams_material == 3)
				{
					mat = g_pMaterialSys->FindMaterial("FlatChamies", "Model textures", true, NULL);
					mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
				}
				if (g_Options.cmaterials.other.hand_chams_material == 4)
				{
					//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

					mat = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/velvet", "Other textures", true, NULL);
					mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
				}
				if (g_Options.cmaterials.other.hand_chams_material == 5)
				{
					//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

					mat = g_pMaterialSys->FindMaterial("models/inventory_items/dogtags/dogtags_outline", "Other textures", true, NULL);
					mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
				}
				if (g_Options.cmaterials.other.hand_chams_material == 6)
				{
					//material = g_pMaterialSys->FindMaterial("FlatChams", "Model textures", true, NULL);

					mat = g_pMaterialSys->FindMaterial("chams", "Model textures", true, NULL);
					mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				}
				//static IMaterial* material = g_pMaterialSys->FindMaterial("models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER, true, NULL);

				if (!mat)
					return;

				if (mat->IsErrorMaterial())
					return;




				g_pRenderView->SetColorModulation(flColor);
				g_pModelRender->ForcedMaterialOverride(mat);
				mat->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
				mat->IncrementReferenceCount();


			}
		}
	oDrawModelExecute_t->GetTrampoline()(ecx, context, state, info, matrix);
	//g_pModelRender->ForcedMaterialOverride( NULL );

}

class animation_fix : public Singleton<animation_fix>
{
public:
	//ÝÒÎ ÍÅÏÐÀÂÈËÜÍÛÉ ÀÍÈÌÔÈÊÑ, ÎÍ ÄÅËÀÅÒÑß ÂÎÎÁÙÅ ÏÎ ÄÐÓÃÎÌÓ ÍÀÕÓÉ!!!


	void post_update_start()
	{
		if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
		{
			if (g::pLocalEntity && g::pLocalEntity->IsAlive())
			{
				for (auto i = 0; i < g_pGlobalVars->maxClients; i++)
				{
					C_BaseEntity* pEnt = g_pEntityList->GetClientEntity(i);

					if (pEnt == nullptr || !(pEnt->IsValidTarget()))
						continue;

					clientanimating_t *animating = nullptr;
					int animflags;

					//Make sure game is allowed to client side animate. Probably unnecessary
					for (unsigned int i = 0; i < ClientSideAnimationList->count; i++)
					{
						clientanimating_t *tanimating = (clientanimating_t*)ClientSideAnimationList->Retrieve(i, sizeof(clientanimating_t));
						C_BaseEntity *pAnimEntity = (C_BaseEntity*)tanimating->pAnimating;
						if (pAnimEntity == pEnt)
						{
							animating = tanimating;
							animflags = tanimating->flags;
							tanimating->flags |= FCLIENTANIM_SEQUENCE_CYCLE;
							break;
						}
					}

					auto animstate = pEnt->AnimState();
					if (animstate)
					{
						if (animstate->m_flLastClientSideAnimationUpdateTime == g_pGlobalVars->curtime)
							animstate->m_flLastClientSideAnimationUpdateTime -= g_pGlobalVars->intervalPerTick;
						if (animstate->m_iLastClientSideAnimationUpdateFramecount == g_pGlobalVars->framecount)
							animstate->m_iLastClientSideAnimationUpdateFramecount--;
					}

				
					AnimationLayer backup_layers[15];
					std::memcpy(backup_layers, pEnt->GetAnimOverlays(), (sizeof(AnimationLayer) * 15));
					//Update animations/poses
					pEnt->UpdateClientAnimation();

					//Restore anim flags
					if (animating)
						animating->flags = animflags;

					std::memcpy(pEnt->GetAnimOverlays(), backup_layers, (sizeof(AnimationLayer) * 15));

					SkipAnimation(pEnt);
				}
			}
		}
	}
	void post_render_start()
	{
		if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
		{
			if (g::pLocalEntity && g::pLocalEntity->IsAlive())
			{
				for (unsigned int i = 0; i < ClientSideAnimationList->count; i++)
				{
					clientanimating_t *animating = (clientanimating_t*)ClientSideAnimationList->Retrieve(i, sizeof(clientanimating_t));
					C_BaseEntity *Entity = (C_BaseEntity*)animating->pAnimating;

					if (Entity->IsPlayer() && Entity != g::pLocalEntity && !Entity->IsDormant() && Entity->IsAlive())
					{
						unsigned int flags = animating->flags;
						ClientSideAnimationFlags[Entity->EntIndex()] = flags;
						HadClientAnimSequenceCycle[Entity->EntIndex()] = (flags & FCLIENTANIM_SEQUENCE_CYCLE);
						if (HadClientAnimSequenceCycle[Entity->EntIndex()])
						{
							animating->flags |= 1;
						}
					}
				}
			}
		}
	}
	void pre_render_start()
	{
		if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
		{
			if (g::pLocalEntity && g::pLocalEntity->IsAlive())
			{
				for (unsigned int i = 0; i < ClientSideAnimationList->count; i++)
				{
					clientanimating_t *animating = (clientanimating_t*)ClientSideAnimationList->Retrieve(i, sizeof(clientanimating_t));
					C_BaseEntity *Entity = (C_BaseEntity*)animating->pAnimating;
					if (Entity->IsPlayer() && Entity != g::pLocalEntity && !Entity->IsDormant() && Entity->IsAlive())
					{
						unsigned int flags = animating->flags;
						ClientSideAnimationFlags[Entity->EntIndex()] = flags;
						HadClientAnimSequenceCycle[Entity->EntIndex()] = (flags & FCLIENTANIM_SEQUENCE_CYCLE);
						if (HadClientAnimSequenceCycle[Entity->EntIndex()])
						{
							animating->flags &= ~1;
						}
					}
				}
			}
		}
	}
	void local() //ãîâíî ñ þö, êîòîðîå ñïàñ÷åííî ñ gucci
	{
		
	}
private:
	bool HadClientAnimSequenceCycle[65];
	int ClientSideAnimationFlags[65];

	void SkipAnimation(C_BaseEntity *player)
	{
		static ConVar *sv_pvsskipanimation = g_pCvar->FindVar("sv_pvsskipanimation");

		int32_t backup_sv_pvsskipanimation = sv_pvsskipanimation->GetInt();
		sv_pvsskipanimation->SetValue(0);

		*(int32_t*)((uintptr_t)player + 0xA30) = 0;
		*(int32_t*)((uintptr_t)player + 0x269C) = 0;

		int32_t backup_effects = *(int32_t*)((uintptr_t)player + 0xEC);
		*(int32_t*)((uintptr_t)player + 0xEC) |= 8;

		player->SetupBones(NULL, -1, 0x7FF00, g_pGlobalVars->curtime);

		*(int32_t*)((uintptr_t)player + 0xEC) = backup_effects;
		sv_pvsskipanimation->SetValue(backup_sv_pvsskipanimation);
	};

	class CBaseAnimating
	{
	public:

	};
	struct clientanimating_t
	{
		CBaseAnimating *pAnimating;
		unsigned int	flags;
		clientanimating_t(CBaseAnimating *_pAnim, unsigned int _flags) : pAnimating(_pAnim), flags(_flags) {}
	};

	class CUtlVectorSimple
	{
	public:
		unsigned memory;
		char pad[8];
		unsigned int count;
		inline void* Retrieve(int index, unsigned sizeofdata)
		{
			return (void*)((*(unsigned*)this) + (sizeofdata * index));
		}
	};

	clientanimating_t *animating = nullptr;
	int animflags;

	const unsigned int FCLIENTANIM_SEQUENCE_CYCLE = 0x00000001;

	CUtlVectorSimple *ClientSideAnimationList = (CUtlVectorSimple*)*(DWORD*)(Utils::FindSignature("client_panorama.dll", "A1 ? ? ? ? F6 44 F0 04 01 74 0B") + 1);
};

void AnimFix()
{
	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{
		auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		if (!local_player)
			return;
		if (!local_player->IsAlive())
			return;

		auto animations = local_player->AnimState();

		if (!animations)
			return;




		auto old_curtime = g_pGlobalVars->curtime;
		auto old_frametime = g_pGlobalVars->frametime;
		auto old_ragpos = local_player->get_ragdoll_pos();

		static auto host_timescale = g_pCvar->FindVar(("host_timescale"));
		g_pGlobalVars->frametime = g_pGlobalVars->intervalPerTick * host_timescale->GetFloat();
		g_pGlobalVars->curtime = local_player->GetSimulationTime();


		if (animations)
			animations->m_iLastClientSideAnimationUpdateFramecount = g_pGlobalVars->framecount - 1;


		local_player->client_side_animation() = true;

		local_player->UpdateClientAnimation();

		g_pGlobalVars->curtime = old_curtime;
		g_pGlobalVars->frametime = old_frametime;
		local_player->get_ragdoll_pos() = old_ragpos;

		local_player->SetAbsAngles(Vector(0.f, animations->m_flGoalFeetYaw, 0.f));

		local_player->client_side_animation() = false;

	}
	
}

void NotReallyUsefull()
{
	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{
		auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		if (!local_player)
			return;

		auto animations = local_player->AnimState();

		if (!animations)
			return;
		if (!local_player->GetAnimOverlays())
			return;

		if (!local_player->GetSimulationTime())
			return;

		static float proper_abs = local_player->AnimState()->m_flGoalFeetYaw;


		local_player->client_side_animation() = true;

		auto old_curtime = g_pGlobalVars->curtime;
		auto old_frametime = g_pGlobalVars->frametime;
		auto old_ragpos = local_player->get_ragdoll_pos();
		auto old_fraction = animations->m_flUnknownFraction = 0.f;

		g_pGlobalVars->curtime = local_player->GetSimulationTime();
		g_pGlobalVars->frametime = g_pGlobalVars->intervalPerTick;

		local_player->UpdateClientAnimation();
		if (g::bSendPacket)
		{
			proper_abs = local_player->AnimState()->m_flGoalFeetYaw;
		}
		g_pGlobalVars->curtime = old_curtime;
		g_pGlobalVars->frametime = old_frametime;
		local_player->get_ragdoll_pos() = old_ragpos;
		if (!g::fake_walk)
		{
			
			animations->m_flUnknownFraction = old_fraction; //fix them legs

		}

		local_player->SetAbsAngles(Vector(0.f, proper_abs, 0.f));

		local_player->client_side_animation() = false;
	}
}


void AnimFix_ghetto()
{

	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{
		auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		if (!local_player)
			return;
		if (!local_player->IsAlive())
			return;

		auto animations = local_player->AnimState();

		if (!animations)
			return;

		if (!local_player->GetAnimOverlays())
			return;

		if (!local_player->GetSimulationTime())
			return;

		
		static float proper_abs = local_player->AnimState()->m_flGoalFeetYaw;

		local_player->client_side_animation() = true;

		auto old_curtime = g_pGlobalVars->curtime;
		auto old_frametime = g_pGlobalVars->frametime;
		auto old_fraction = animations->m_flUnknownFraction = 0.f;

		g_pGlobalVars->curtime = local_player->GetSimulationTime();
		g_pGlobalVars->frametime = g_pGlobalVars->intervalPerTick;
		auto old_ragpos = local_player->get_ragdoll_pos();

		AnimationLayer backup_layers[15];
		std::memcpy(backup_layers, local_player->GetAnimOverlays(), (sizeof(AnimationLayer) * 15));

		local_player->UpdateClientAnimation();

		if (g::bSendPacket)
		{
			proper_abs = local_player->AnimState()->m_flGoalFeetYaw;
		}

		std::memcpy(local_player->GetAnimOverlays(), backup_layers, (sizeof(AnimationLayer) * 15));

		g_pGlobalVars->curtime = old_curtime;
		g_pGlobalVars->frametime = old_frametime;
		local_player->get_ragdoll_pos() = old_ragpos;
		animations->m_flUnknownFraction = old_fraction; //fix them legs


		//animations->m_flUnknownFraction = old_fraction; //fix them legs

		local_player->SetAbsAngles(Vector(0.f, proper_abs, 0.f));


		local_player->client_side_animation() = false;
		
		

	}
}


void FAKEPRIME()
{
    static bool lastState = false;

    if (config.misc.fakeprime != lastState) {
        lastState = config.misc.fakeprime;
        auto fakePrime = FIND_PATTERN(std::uint8_t*, L"client_panorama", "\x17\xF6\x40\x14\x10") - 1;
        if (DWORD oldProtect; VirtualProtect(fakePrime, 1, PAGE_EXECUTE_READWRITE, &oldProtect)) {
            constexpr uint8_t patch[]{ 0x74, 0xEB };
            *fakePrime = patch[config.misc.fakeprime];
            VirtualProtect(fakePrime, 1, oldProtect, nullptr);
        }
    }
}


void Keys()
{
	static float delay;
	if (g_Options.antiaa.double_tap_key >= 0 && GetAsyncKeyState(g_Options.antiaa.double_tap_key) && abs(delay - g_pGlobalVars->curtime) > 0.5)
	{
		overridething_dt_key = !overridething_dt_key;
		delay = g_pGlobalVars->curtime;
	}


	static float time;
	if (g_Options.legit.trigger.key >= 0 && GetAsyncKeyState(g_Options.legit.trigger.key) && abs(time - g_pGlobalVars->curtime) > 0.5)
	{
		overridething_legit = !overridething_legit;
		time = g_pGlobalVars->curtime;
	}
}
void animations_new()
{
	if (!g_pEngine->IsConnected() && !g_pEngine->IsInGame())
		return;
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player)
		return;
	auto animations = local_player->AnimState();

	if (!animations)
		return;

	if (/*enabledtp &&*/ g_Options.antiaa.fake_duck && GetAsyncKeyState(g_Options.antiaa.fake_duck_key))
	{
		animations->m_fDuckAmount = 64.f;
	}

	
}

void __stdcall Hooks::FrameStageNotify(ClientFrameStage_t curStage)
{
	Keys();// smart yume
	switch (curStage)
	{
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
		Skinchanger();
		if (g_Options.GLOVE)
			glove_changer.run();//dont fucking move
		break;
	case FRAME_RENDER_START:
		g_Misc.Thirdperson_FSN(curStage);
		g_Misc.remose_flash();
		g_Misc.remove_smoke();
		FAKEPRIME();
		animations_new();
		g_Resolver.FrameStage(curStage);
		break;
	}
	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{
		//g_Misc.PlayerChanger(curStage);
		chams123::get().dark_mode();
		g_Misc.SoundEsp(curStage);
	}
	oFrameStageNotify_t->GetTrampoline()(curStage);
  }

C_BaseEntity* UTIL_PlayerByIndex(int index)
{
	typedef C_BaseEntity*(__fastcall* PlayerByIndex)(int);
	static PlayerByIndex UTIL_PlayerByIndex = (PlayerByIndex)Utils::FindSignature("server.dll", "85 C9 7E 2A A1");

	if (!UTIL_PlayerByIndex)
		return false;

	return UTIL_PlayerByIndex(index);
}

bool IsPisto423124lboi(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_iItemDefinitionIndex();
	static const std::vector<int> v = { (int)ItemDefinitionIndex::WEAPON_DEAGLE,(int)ItemDefinitionIndex::WEAPON_CZ75,(int)ItemDefinitionIndex::WEAPON_ELITE,(int)ItemDefinitionIndex::WEAPON_USPS,(int)ItemDefinitionIndex::WEAPON_P250,(int)ItemDefinitionIndex::WEAPON_P2000, (int)ItemDefinitionIndex::WEAPON_TEC9,(int)ItemDefinitionIndex::WEAPON_REVOLVER,(int)ItemDefinitionIndex::WEAPON_FIVESEVEN,(int)ItemDefinitionIndex::WEAPON_GLOCK };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool IsSni3425234per(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_iItemDefinitionIndex();
	static const std::vector<int> v = { (int)ItemDefinitionIndex::WEAPON_AWP,(int)ItemDefinitionIndex::WEAPON_SSG08,(int)ItemDefinitionIndex::WEAPON_G3SG1,(int)ItemDefinitionIndex::WEAPON_SCAR20 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}




bool IsRiflen3423gaya(void* weapon)
{
	if (weapon == nullptr) return false;
	C_BaseCombatWeapon *pWeapon = (C_BaseCombatWeapon*)weapon;
	int id = *pWeapon->m_iItemDefinitionIndex();
	static const std::vector<int> v = { (int)ItemDefinitionIndex::WEAPON_AK47,(int)ItemDefinitionIndex::WEAPON_AUG,(int)ItemDefinitionIndex::WEAPON_FAMAS,(int)ItemDefinitionIndex::WEAPON_GALIL,(int)ItemDefinitionIndex::WEAPON_M249,(int)ItemDefinitionIndex::WEAPON_M4A4,(int)ItemDefinitionIndex::WEAPON_M4A1S,(int)ItemDefinitionIndex::WEAPON_NEGEV,(int)ItemDefinitionIndex::WEAPON_SG553 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}
void DrawFOVCrosshair()
{
	int xs;
	int ys;
	float FoV;
	std::vector<int> HitBoxesToScan;

	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
	if (!local_player)
		return;

	auto pWeapon = local_player->GetActiveWeapon();

	if (!pWeapon)
		return;



	FoV = g_legitbot.FoV;



	g_pEngine->GetScreenSize(xs, ys);
	g_pEngine->GetScreenSize(xs, ys);
	xs /= 2; ys /= 2;
	g_pSurface->DrawCircle(xs, ys, FoV * 6.25, FoV * 6.25, Color(legit::colors::legit_aa_visualize_fov_color));

}

void testo()
{
	static const auto fake_color = Color(50, 50, 255, 200);
	static const auto real_color = Color(255, 0, 0, 200);
	static const auto lby_color = Color(0, 255, 0, 200);
	static const auto advanced_shit = Color(255, 255, 255, 80);

	if (g_Options.antiaa.enable)
	{



		if (g::pLocalEntity->IsAlive())
		{
			float distance = 40.f;

			Vector local_origin = g::pLocalEntity->GetAbsOrigin();

			Vector2D screen1, screen2;

			Utils::WorldToScreen(local_origin, screen1);

			Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::FakeAngle.y, distance), screen2);
			g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, fake_color);
			g_pSurface->DrawT(screen2.x, screen2.y, fake_color, g::TahomaSmall, true, "FAKE");

			Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::RealAngle.y, distance), screen2);
			g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, real_color);
			g_pSurface->DrawT(screen2.x, screen2.y, real_color, g::TahomaSmall, true, "REAL");

			Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::pLocalEntity->GetLowerBodyYaw(), distance), screen2);
			g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, lby_color);
			g_pSurface->DrawT(screen2.x, screen2.y, lby_color, g::TahomaSmall, true, "LBY");
		}


		/*
						if (!g_Menu.Config.AdvancedAAIndicator)
							return;

						Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::pLocalEntity->AnimState()->m_flCurrentFeetYaw, 30.f), screen2);
						g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, advanced_shit);
						g_pSurface->DrawT(screen2.x, screen2.y, advanced_shit, g::TahomaSmall, true, "CUR_FEET");

						Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::pLocalEntity->AnimState()->m_flGoalFeetYaw, 30.f), screen2);
						g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, advanced_shit);
						g_pSurface->DrawT(screen2.x, screen2.y, advanced_shit, g::TahomaSmall, true, "GOAL_FEET");

						Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::pLocalEntity->AnimState()->m_flCurrentTorsoYaw, 30.f), screen2);
						g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, advanced_shit);
						g_pSurface->DrawT(screen2.x, screen2.y, advanced_shit, g::TahomaSmall, true, "CUR_TORSO");*/

						/*Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::pLocalEntity->AnimState()->m_flEyeYaw, 30.f), screen2);
						g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, advanced_shit);
						g_pSurface->DrawT(screen2.x, screen2.y, advanced_shit, g::TahomaSmall, true, "EYES");*/

						/*Utils::WorldToScreen(g_Math.get_rotated_position(local_origin, g::GoalLby, 35.f), screen2);
						g_pSurface->Line(screen1.x, screen1.y, screen2.x, screen2.y, advanced_shit);
						g_pSurface->DrawT(screen2.x, screen2.y, advanced_shit, g::TahomaSmall, true, "GOAL_LBY");*/
	}

}


//Memory leak...
void GradientLine(int x, int y, int w, int h, Color c1, Color c2)
{
	g_pSurface->FilledRect(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < w; i++)
	{
		float fi = i, fw = w;
		float a = fi / fw;
		DWORD ia = a * 255;
		g_pSurface->FilledRect(x + i, y, 1, h, Color(first, second, third, ia));
	}
}

Color getRainbow(int speed, int offset)
{
	float hue = (float)((GetCurrentTime() + offset) % speed);
	hue /= speed;
	return Color::FromHSB(0.4F, 1.0F, hue);
}
void Indicators()
{

	bool x345d = Options::Menu.m_bIsOpen;
	if (x345d)
	{
		if (g_Options.esp.effects.watermark)
		{
			int width, height;
			g_pEngine->GetScreenSize(width, height);
			GradientLine(0, 0, width / 2, 4, Color(menu::color, 255), Color(menu::color, 255));
			GradientLine(width / 2, 0, width / 2, 4, Color(menu::color, 255), Color(menu::color, 255));
		}

	}
	
}
void show_key()
{
	if (g_Options.esp.other.wasd)
	{
		g_pSurface->FilledRect(10, 200 + 200, 30, 35, Color(0, 0, 0, 155));
		g_pSurface->FilledRect(10 + 32, 200 + 200, 30, 35, Color(0, 0, 0, 155));
		g_pSurface->FilledRect(10 + 32 + 32, 200 + 200, 30, 35, Color(0, 0, 0, 155));
		g_pSurface->FilledRect(10 + 32, 200 - 37 + 200, 30, 35, Color(0, 0, 0, 155));
		g_pSurface->FilledRect(10, 100 + 160 + 177, 95, 35, Color(0, 0, 0, 155));


		//draw key a -> filed rect
		if (g_pInputSystem->IsButtonDown(KEY_A)) {
			g_pSurface->FilledRect(10, 200 + 200, 30, 35, Color(0, 0, 0, 155));
		}
		//draw key s -> filed rect
		if (g_pInputSystem->IsButtonDown(KEY_S)) {
			g_pSurface->FilledRect(10 + 32, 200 + 200, 30, 35, Color(0, 0, 0, 155));
		}
		//draw key d -> filed rect
		if (g_pInputSystem->IsButtonDown(KEY_D)) {
			g_pSurface->FilledRect(10 + 32 + 32, 200 + 200, 30, 35, Color(0, 0, 0, 155));
		}
		//draw key w -> filed rect
		if (g_pInputSystem->IsButtonDown(KEY_W)) {
			g_pSurface->FilledRect(10 + 32, 200 - 37 + 200, 30, 35, Color(0, 0, 0, 155));
		}
		//draw key space -> filed rect
		if (g_pInputSystem->IsButtonDown(KEY_SPACE))
		{
			g_pSurface->FilledRect(10, 100 + 160 + 177, 95, 35, Color(0, 0, 0, 155));
		}


		//draw key a -> text -- pressed
		if (g_pInputSystem->IsButtonDown(KEY_A))
		{
			g_pSurface->DrawT(18, 205 + 200, Color(menu::color, 255), g::key_font, false, "a");

		}
		else
		{
			g_pSurface->DrawT(18, 205 + 200, Color(255, 255, 255, 255), g::key_font, false, "a");

		}
		//draw key s -> text -- pressed
		if (g_pInputSystem->IsButtonDown(KEY_S)) {
			g_pSurface->DrawT(17 + 35, 205 + 200, Color(menu::color, 255), g::key_font, false, "s");

		}
		else
		{
			g_pSurface->DrawT(17 + 35, 205 + 200, Color(255, 255, 255, 255), g::key_font, false, "s");
		}
		//draw key d -> text -- pressed
		if (g_pInputSystem->IsButtonDown(KEY_D)) {

			g_pSurface->DrawT(17 + 35 + 32, 205 + 200, Color(menu::color, 255), g::key_font, false, "d");

		}
		else
		{
			g_pSurface->DrawT(17 + 35 + 32, 205 + 200, Color(255, 255, 255, 255), g::key_font, false, "d");
		}
		//draw key w -> text -- pressed
		if (g_pInputSystem->IsButtonDown(KEY_W)) {
			g_pSurface->DrawT(18 + 31, 205 - 35 + 200, Color(menu::color, 255), g::key_font, false, "w");

		}
		else
		{
			g_pSurface->DrawT(18 + 31, 205 - 35 + 200, Color(255, 255, 255, 255), g::key_font, false, "w");
		}
		//draw key space -> text -- pressed
		if (g_pInputSystem->IsButtonDown(KEY_SPACE))
		{
			g_pSurface->DrawT(30, 385 - 120 + 177, Color(menu::color, 255), g::key_font, false, "space");

		}
		else
		{
			g_pSurface->DrawT(30, 385 - 120 + 177, Color(255, 255, 255, 255), g::key_font, false, "space");
		}
	}
	

}

void drawBombTimer()
{
	if (g_Options.esp.other.bomb_timer)
	{
		for (int i = g_pEngine->GetMaxClients(); i <= g_pEntityList->GetHighestEntityIndex(); i++)
		{

			C_BaseEntity* entity = g_pEntityList->GetClientEntity(i);
			if (!entity || entity->IsDormant() || entity->GetClientClass()->ClassID != ClassId::CPlantedC4 || !entity->c4Ticking())
				continue;

			constexpr unsigned font{ 0xc1 };
			g_pSurface->DrawSetTextFont(font);
			g_pSurface->DrawSetTextColor(255, 255, 255, 255);
			auto drawPositionY{ g_pSurface->getScreenSize().second / 8 };
			auto bombText{ (std::wstringstream{ } << L"Bomb on " << (!entity->c4BombSite() ? 'A' : 'B') << L" : " << std::fixed << std::showpoint << std::setprecision(3) << (std::max)(entity->c4BlowTime() - g_pGlobalVars->curtime, 0.0f) << L" s").str() };
			const auto bombTextX{ g_pSurface->getScreenSize().first / 2 - static_cast<int>((g_pSurface->getTextSize(font, bombText.c_str())).first / 2) };
			g_pSurface->DrawSetTextPos(bombTextX, drawPositionY);
			drawPositionY += g_pSurface->getTextSize(font, bombText.c_str()).second;
			g_pSurface->printText(bombText.c_str());

			const auto progressBarX{ g_pSurface->getScreenSize().first / 3 };
			const auto progressBarLength{ g_pSurface->getScreenSize().first / 3 };
			constexpr auto progressBarHeight{ 5 };

			g_pSurface->setDrawColor1(50, 50, 50);
			g_pSurface->DrawFilledRect(progressBarX - 3, drawPositionY + 2, progressBarX + progressBarLength + 3, drawPositionY + progressBarHeight + 8);
			Color color_ = Color(esp::colors::bomb_timer_col);

			g_pSurface->set_drawing_color(color_.r(), color_.g(), color_.b());

			static auto c4Timer = g_pCvar->FindVar("mp_c4timer");

			g_pSurface->DrawFilledRect(progressBarX, drawPositionY + 5, static_cast<int>(progressBarX + progressBarLength * clamp(entity->c4BlowTime() - g_pGlobalVars->curtime, 0.0f, c4Timer->GetFloat()) / c4Timer->GetFloat()), drawPositionY + progressBarHeight + 5);
			//if (entity->c4Defuser() == NULL)
			//	return;
		//	if (entity->hasDefuser() == NULL)
			//	return;

			if (entity->c4Defuser() != -1)
			{

				if (PlayerInfo_t playerInfo;
					g_pEngine->GetPlayerInfo(g_pEntityList->GetClientEntityFromHandle(entity->c4Defuser())->EntIndex(), &playerInfo))
				{
					if (wchar_t name[128];  MultiByteToWideChar(CP_UTF8, 0, playerInfo.szName, -1, name, 128)) {
						drawPositionY += g_pSurface->getTextSize(font, L" ").second;
						const auto defusingText{ (std::wstringstream{ } << name << L" is defusing: " << std::fixed << std::showpoint << std::setprecision(3) << (std::max)(entity->c4DefuseCountDown() - g_pGlobalVars->curtime, 0.0f) << L" s").str() };

						g_pSurface->DrawSetTextPos((g_pSurface->getScreenSize().first - g_pSurface->getTextSize(font, defusingText.c_str()).first) / 2, drawPositionY);
						g_pSurface->printText(defusingText.c_str());
						drawPositionY += g_pSurface->getTextSize(font, L" ").second;

						g_pSurface->setDrawColor1(50, 50, 50);
						g_pSurface->DrawFilledRect(progressBarX - 3, drawPositionY + 2, progressBarX + progressBarLength + 3, drawPositionY + progressBarHeight + 8);
						g_pSurface->setDrawColor1(0, 255, 0);
						g_pSurface->DrawFilledRect(progressBarX, drawPositionY + 5, progressBarX + static_cast<int>(progressBarLength * (std::max)(entity->c4DefuseCountDown() - g_pGlobalVars->curtime, 0.0f) / (g_pEntityList->GetClientEntityFromHandle(entity->c4Defuser())->hasDefuser() ? 5.0f : 10.0f)), drawPositionY + progressBarHeight + 5);

						drawPositionY += g_pSurface->getTextSize(font, L" ").second;
						const wchar_t* canDefuseText;

						if (entity->c4BlowTime() >= entity->c4DefuseCountDown()) {
							canDefuseText = L"Can Defuse";
							g_pSurface->setTextColor5(0, 255, 0);
						}
						else {
							canDefuseText = L"Cannot Defuse";
							g_pSurface->setTextColor5(255, 0, 0);
						}

						g_pSurface->DrawSetTextPos((g_pSurface->getScreenSize().first - g_pSurface->getTextSize(font, canDefuseText).first) / 2, drawPositionY);
						g_pSurface->printText(canDefuseText);
					}
				}
			}
			break;
		}
	}
}
void for_every_entity(const std::function<void(C_BaseEntity* ent)>& func, int classid)
{
	for (auto i = 0; i < g_pEntityList->GetHighestEntityIndex(); i++)
	{
		const auto entity = g_pEntityList->GetClientEntity(i);
		if (!entity)
			continue;

		if (classid != entity->GetClientClass()->ClassID)
			continue;

		func(reinterpret_cast<C_BaseEntity*>(entity));
	}
}
#include <algorithm>

void Drawmolotov()
{

	if (!g_Options.esp.nade_esp_molotov[0] && !g_Options.esp.nade_esp_molotov[1] && !g_Options.esp.nade_esp_molotov[2])
		return;

	static const std::string name = "molotov";
	const Color color = Color(255, 255, 255, 255);

	for_every_entity([](C_BaseEntity* entity) -> void {

		auto inferno = reinterpret_cast <c_cs_inferno*> (entity);

		const auto origin = inferno->get_abs_origin();
		auto get_w2s_origin = Vector{};
		Vector scrn_pos, origin_pos;

		if (!Utils::WorldToScreen(origin, get_w2s_origin))
			return;

		const auto spawn_time = inferno->get_entity_spawn_time();
		static constexpr float max_time = 7.03125;
		const auto actual = (((spawn_time + max_time) - g_pGlobalVars->curtime) / max_time);
		static auto size = Vector2D(70, 3);
		const auto pos = Vector2D(get_w2s_origin.x - size.x * 0.5, get_w2s_origin.y - size.y * 0.5);
		Color background = Color(35, 35, 35, 255);
		Color bar = esp::colors::nadecolorr;
		Color text = Color(255, 255, 255,255);

		if (g_Options.esp.nade_esp_molotov[0]) {
			//if (!config->Visuals.nade_style_icon)
				g_pSurface->text(g::TahomaSmall, pos.x + 32, pos.y - 12, text, HFONT_CENTERED_Y, name.c_str());
			//else
				//Drawing::DrawString(F::ESPWeapon, pos.x + 32, pos.y - 18, text, FONT_CENTER, "l");
		}
		/*if (g_Options.esp.nade_style_damage) {
			Color destcolor = (inferno->m_flDamage() < 5) ? Color::Green() : (inferno->m_flDamage() < 12 ? Color(255, 130, 0) : Color::Red());
			Drawing::DrawString(F::ESPInfo, pos.x + 32, pos.y + 5, destcolor, HFONT_CENTERED_Y, "%i -HP", inferno->m_flDamage());
		}*/
		if (g_Options.esp.nade_esp_molotov[1]) {
			g_pSurface->rect(pos.x, pos.y, size.x, size.y, background);
			g_pSurface->rect(pos.x + 1, pos.y + 1, (size.x - 2) * actual, size.y - 2, bar);
		}
		if (g_Options.esp.nade_esp_molotov[2]) {
			static float radius = 130;
			float multi = M_PI * 2.0 / 2047;
			Vector place = origin;
			static float hue_offset = 0.f;

			for (float rota = 0; rota < (M_PI * 2.0); rota += multi) {

				Vector position = Vector(radius * cos(rota) + place.x, radius * sin(rota) + place.y, place.z);
				C_Trace trace;
				C_TraceFilter filter(g::pLocalEntity);


				//filter.pSkip = local;
				g_pTrace->TraceRay(C_Ray(place,position), 0, &filter, &trace);

				if (Utils::WorldToScreen(trace.end, scrn_pos)) {
					if (origin_pos.IsValid() && scrn_pos.IsValid()) {

						if (origin_pos == Vector(0, 0, 0) || scrn_pos == Vector(0, 0, 0)) {
							origin_pos = scrn_pos;
						}

						static auto linear_fade = [](int32_t& current, const int32_t min, const int32_t max, const float frequency, const bool direction) -> void
						{
							if (current < max && direction)
								current += static_cast<int>(std::ceil(frequency * g_pGlobalVars->frametime));
							else if (current > min && !direction)
								current -= static_cast<int>(std::ceil(frequency * g_pGlobalVars->frametime));

							current = clamp(current, min, max);
						};

						int scale = 0;
						static bool direction = false;

						if (scale == 0 || scale == 255)
							direction = !direction;

						linear_fade(scale, 0, 5, 5 / 2.5f, direction);

						g_pSurface->line(origin_pos.x, origin_pos.y + scale, scrn_pos.x, scrn_pos.y, esp::colors::nadecolorr);
					}
					origin_pos = scrn_pos;
				}
			}
			hue_offset += 25;
		}
		//player_info info;
		/*if (inferno->m_hThrower() && Source::m_pEngine->GetPlayerInfo(inferno->m_hThrower()->entindex(), &info)) {*/
			//sprintf_s(buf, "%i -HP", inferno->m_flDamage())/*, info.name*/);
			//Drawing::DrawString(F::ESPInfo, pos.x + 32, pos.y + 2, text, FONT_CENTER, buf);
			//Source::m_pDebugOverlay->PurgeTextOverlays
		//}

		}, ClassId::CInferno);
}

void Drawsmoke()
{
	if (!g_Options.esp.nade_esp_smoke[0] && !g_Options.esp.nade_esp_smoke[1] && !g_Options.esp.nade_esp_smoke[2])
		return;

	static const std::string name = "smoke";
	const Color color = Color(255, 255, 255, 255);

	for_every_entity([](C_BaseEntity* entity) -> void {

		auto smoke = reinterpret_cast <c_cs_inferno*> (entity);

		const auto origin = smoke->get_abs_origin();
		auto get_w2s_origin = Vector{};
		Vector scrn_pos, origin_pos;

		if (!Utils::WorldToScreen(origin, get_w2s_origin))
			return;

		const auto spawn_time = smoke->get_entity_spawn_time();
		static constexpr float max_time = 17.5;
		const auto actual = (((spawn_time + max_time) - g_pGlobalVars->curtime) / max_time);

		if (actual <= 0)
			return;
		static auto size = Vector2D(70, 3);
		const auto pos = Vector2D(get_w2s_origin.x - size.x * 0.5, get_w2s_origin.y - size.y * 0.5);
		Color background = Color(35, 35, 35, 255);
		Color bar = esp::colors::nadecolorr;
		Color text = Color(255, 255, 255,255);

		if (config.esp.nade_esp_smoke[0]) {
			//if (!config.esp.nade_style_icon)
				g_pSurface->text(g::Tahoma, pos.x + 32, pos.y - 12, text, HFONT_CENTERED_Y, name.c_str());
			//else
				//g_pSurface->text(g::Arial, pos.x + 32, pos.y - 18, text, HFONT_CENTERED_Y, "l");
		}
		if (config.esp.nade_esp_smoke[1]) {
			g_pSurface->rect(pos.x, pos.y, size.x, size.y, background);
			g_pSurface->rect(pos.x + 1, pos.y + 1, (size.x - 2) * actual, size.y - 2, bar);
		}
		if (config.esp.nade_esp_smoke[2]) {
			static float radius = 144;
			float multi = M_PI * 2.0 / 2047;
			Vector place = origin;
			static float hue_offset = 0.f;

			for (float rota = 0; rota < (M_PI * 2.0); rota += multi) {

				Vector position = Vector(radius * cos(rota) + place.x, radius * sin(rota) + place.y, place.z);
				C_Trace trace;
				C_TraceFilter filter(g::pLocalEntity);

				//filter.pSkip = local;

				g_pTrace->TraceRay(C_Ray(place,position), 0, &filter, &trace);

				if (Utils::WorldToScreen(trace.end, scrn_pos)) {
					if (origin_pos.IsValid() && scrn_pos.IsValid()) {

						if (origin_pos == Vector(0, 0, 0) || scrn_pos == Vector(0, 0, 0)) {
							origin_pos = scrn_pos;
						}

						static auto linear_fade = [](int32_t& current, const int32_t min, const int32_t max, const float frequency, const bool direction) -> void
						{
							if (current < max && direction)
								current += static_cast<int>(std::ceil(frequency * g_pGlobalVars->frametime));
							else if (current > min && !direction)
								current -= static_cast<int>(std::ceil(frequency * g_pGlobalVars->frametime));

							current = clamp(current, min, max);
						};

						int scale = 0;
						static bool direction = false;

						if (scale == 0 || scale == 255)
							direction = !direction;

						linear_fade(scale, 0, 5, 5 / 2.5f, direction);

						g_pSurface->Line(origin_pos.x, origin_pos.y + scale, scrn_pos.x, scrn_pos.y, esp::colors::nadecolorr);
					}
					origin_pos = scrn_pos;
				}
			}
			hue_offset += 25;
		}
		}, ClassId::CSmokeGrenadeProjectile);
}

void __fastcall Hooks::PaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	static unsigned int panelID, panelHudID;

	if (!panelHudID)
		if (!strcmp("HudZoom", g_pPanel->GetName(vguiPanel)))
		{
			panelHudID = vguiPanel;
		}

	if (panelHudID == vguiPanel && g::pLocalEntity && g::pLocalEntity->IsAlive() && g_Options.esp.effects.removescopeoverlay)
	{
		if (g::pLocalEntity->IsScoped())
			return;
	}

	if (!panelID)
		if (!strcmp("MatSystemTopPanel", g_pPanel->GetName(vguiPanel)))
		{
			panelID = vguiPanel;
			g_Hooks.bInitializedDrawManager = true;
		}
	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{
		ConVar* mat_fullbright = g_pCvar->FindVar("mat_fullbright");

		if (g_Options.esp.effects.fullbright)
			mat_fullbright->SetValue(true);
		else
			mat_fullbright->SetValue(false);
	}
	if (panelID == vguiPanel) 
	{
        Render::get().BeginScene();
		g_pInputSystem->EnableInput(!Options::Menu.m_bIsOpen);

		if (g_Options.esp.enable)
		{
			Drawsmoke();
			Drawmolotov();
			c_visuals::get().spectators();
			c_visuals::get().indicatorssss();

		}


	//	g_Menu.Render();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
			{
				g::w2s_matrix[i][j] = g_pEngine->WorldToScreenMatrix()[i][j];
				
			}
		}
		
		if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
		{

			float AmbientRedAmount = g_Options.misc.ambient_red;
			float AmbientGreenAmount = g_Options.misc.ambient_green;
			float AmbientBlueAmount = g_Options.misc.ambient_blue;

			ConVar* AmbientRedCvar = g_pCvar->FindVar("mat_ambient_light_r");
			*(float*)((DWORD)&AmbientRedCvar->fnChangeCallback + 0xC) = NULL;
			AmbientRedCvar->SetValue(AmbientRedAmount);

			ConVar* AmbientGreenCvar = g_pCvar->FindVar("mat_ambient_light_g");
			*(float*)((DWORD)&AmbientGreenCvar->fnChangeCallback + 0xC) = NULL;
			AmbientGreenCvar->SetValue(AmbientGreenAmount);

			ConVar* AmbientBlueCvar = g_pCvar->FindVar("mat_ambient_light_b");
			*(float*)((DWORD)&AmbientBlueCvar->fnChangeCallback + 0xC) = NULL;
			AmbientBlueCvar->SetValue(AmbientBlueAmount);

			if (g_Options.legit.visualize_fov)
				DrawFOVCrosshair();
		
			show_key();

			if (g_Options.esp.enable)
			{
				auto matpostprocess = g_pCvar->FindVar("mat_postprocess_enable");
				matpostprocess->fnChangeCallback.SetSize(0);
				matpostprocess->SetValue(!g_Options.esp.effects.disable_post);

				g_Misc.Crosshair();
				if (g_Options.legit.legit_aa)
				{
					if (g_Options.legit.legit_aa_indicator)
					{
						int W, H, cW, cH;
						g_pEngine->GetScreenSize(W, H);
						cW = W / 2;
						cH = H / 2;




						if (g_Options.legit.legit_aa_key)
						{
							side = false;
							if (GetKeyState(g_Options.legit.legit_aa_key))
							{
								side = true;
							}
						}

						if (side)
						{
							//g_pSurface->TextNEW(cW - 44, cH - 20, Color(Options::Menu.LegitBotTab.Legit_AA_col.GetValue(), 255), g::LBY2, L"◀");
                            Render::get().RenderText("◀", ImVec2(cW - 44, cH - 20),30.f, Color(legit::colors::legit_aa_visualize_color),false,true, g_pSecondFont);
						}
						else
						{
                            Render::get().RenderText("▶", ImVec2(cW + 28, cH - 20), 30.f, Color(legit::colors::legit_aa_visualize_color), false, true, g_pSecondFont);
							//g_pSurface->TextNEW(cW + 28, cH - 20, Color(Options::Menu.LegitBotTab.Legit_AA_col.GetValue(), 255), g::LBY2, L"▶");
						}


						// im soo 200 iq with this code xD
						// no yume this code is gay xD

					}



				}
				
			//	visuals->Draw();
				c_visuals::get().DrawPlayers();
				drawBombTimer();
				if (g_Options.esp.effects.autowall_crosshair)
				{

					c_visuals::get().penetration_reticle();

				}
				if (g_Options.esp.other.feet_circle)
				{

					c_visuals::get().DrawFeetRange();

				}
				if (g_Options.esp.other.zeus_range)
				{

					c_visuals::get().DrawZeusRange();

				}
			}
			GrenadePrediction::get().Paint();

			otheresp::get().hitmarker_paint();
			//testo();
			//if (Options::Menu.RageBotTab.showaimpoint.GetState())
			//	otheresp::get().hitmarkerdynamic_paint();
			/*
			if (Options::Menu.VisualsTab.LBYIndicator.GetIndex() != 0)
			{
				char angle[50];
				sprintf_s(angle, sizeof(angle), "%i", pLocal->GetLowerBodyYaw());

				switch (Options::Menu.VisualsTab.LBYIndicator.GetIndex())
				{
				case 1:
				{
					if (LastAngleAAReal.y - pLocal->GetLowerBodyYaw() >= -35 && LastAngleAAReal.y - pLocal->GetLowerBodyYaw() <= 35)
					{
						Render::Text(9, scrn.bottom - 71, Color(255, 0, 30, 255), Render::Fonts::LBY, "LBY");
					}
					else
					{
						if (LastAngleAAReal.y - pLocal->GetLowerBodyYaw() > 119.f)
						{
							Render::Text(9, scrn.bottom - 71, Color(150, 0, 250, 255), Render::Fonts::LBY, "LBY");
						}
						else
						{
							Render::Text(9, scrn.bottom - 71, Color(0, 250, 30, 255), Render::Fonts::LBY, "LBY");
						}
					}
				}
				break;

				
				}
			}
			*/
			if (false && g::pLocalEntity->IsAlive()) // NEED THIS WHEN TESTING ANTI AIM
			{
				static uintptr_t pCall = (uintptr_t)Utils::FindSignature("server.dll", "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE");

				static float fDuration = 0.5f;

				PVOID pEntity = nullptr;
				pEntity = UTIL_PlayerByIndex(g::pLocalEntity->EntIndex());

				if (pEntity)
				{
					__asm
					{
						pushad
						movss xmm1, fDuration
						push 0 //bool monoColor
						mov ecx, pEntity
						call pCall
						popad
					}
				}
			}
		}
		visuals->watermark();
		visuals->QuakeKillCounter();
		bool x345d = Menu::get().IsVisible();

		int w, h;
		int centerW, centerh;
		g_pEngine->GetScreenSize(w, h);
		centerW = w / 2;
		centerh = h / 2;

		float owo = 100 / 500;
		//--
		if (g_pEngine->IsActiveApp() && x345d)
			g_pSurface->Clear(0, 0, w * 4, h * 4, Color(10, 10, 10, (Menulooks::MenuAlpha / (1.25 + owo))));

		if (g_pEngine->IsActiveApp() && x345d && !g_Options.esp_streamproof && !g_Options.esp.other.disable_backdrop)
			Drop::DrawBackDrop();

		Indicators();

		//plist.update();

	//	Options::DoUIFrame();

	}
	oPaintTraverse_t->GetTrampoline()(pPanels, vguiPanel, forceRepaint, allowForce);

}

void GetViewModelFOV(float& fov)
{
	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{
		if (!g::pLocalEntity)
			return;

		if (g_Options.esp.enable)
			fov = g_Options.esp.other.override_viewangle_fov;
	}

}
float __stdcall Hooks::GGetViewModelFOV()
{
	float fov = oGGetViewModelFOV_t->GetTrampoline()();
	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{
		GetViewModelFOV(fov);
	}
	return fov;
}

void __fastcall Hooks::OverrideView(void* ecx, void* edx, CViewSetup* pSetup)
{

	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{
		if (!g::pLocalEntity)
			return;

		if (!g::pLocalEntity->IsAlive())
			return;

		if (g_Options.esp.effects.remove_recoil)
		{
			Vector viewPunch = g::pLocalEntity->GetViewPunchAngle();
			Vector aimPunch = g::pLocalEntity->GetAimPunchAngle();

			pSetup->angles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
			pSetup->angles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
			pSetup->angles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		}
		if (g_Options.esp.effects.grenade_prediction)
			GrenadePrediction::get().View(pSetup);
		g_Misc.ThirdPerson();

		auto zoom = g_Options.esp.other.override_viewmodel_fov;

		if (g::pLocalEntity->IsScoped() && g_Options.esp.effects.remove_zoom)
		{
			zoom += 90.0f - pSetup->fov;
		}
		pSetup->fov += zoom;


	
		if (g_Options.antiaa.fake_duck)
		{
			if (GetAsyncKeyState(g_Options.antiaa.fake_duck_key) && g::pLocalEntity && g::pLocalEntity->IsAlive() /*&& enabledtp*/)
				pSetup->origin.z = g::pLocalEntity->GetAbsOrigin().z + 64.f;
		}
		
	}

	oOverrideView_t->GetTrampoline()(ecx, edx, pSetup);
}
static bool menu_open = false;
static bool d3d_init = false;
bool PressedKeys[256] = {};
void __fastcall Hooks::LockCursor(ISurface* thisptr, void* edx)
{


	bool xd = Menu::get().IsVisible();
	if (xd)
	{
		g_pSurface->UnLockCursor();
        g_pInputSystem->ResetInputState();
		return;
	}
	return oLockCursor_t->GetTrampoline()(thisptr, edx);
}







#include "Input.h"
LRESULT Hooks::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LPVOID base;
    InputSys::get().RegisterHotkey(g_Options.misc.menu_key, [base]() {
        Menu::get().Toggle();
        });
	switch (uMsg) {
	case WM_LBUTTONDOWN:
		PressedKeys[VK_LBUTTON] = true;
		break;
	case WM_LBUTTONUP:
		PressedKeys[VK_LBUTTON] = false;
		break;
	case WM_RBUTTONDOWN:
		PressedKeys[VK_RBUTTON] = true;
		break;
	case WM_RBUTTONUP:
		PressedKeys[VK_RBUTTON] = false;
		break;
	case WM_MBUTTONDOWN:
		PressedKeys[VK_MBUTTON] = true;
		break;
	case WM_MBUTTONUP:
		PressedKeys[VK_MBUTTON] = false;
		break;
	case WM_XBUTTONDOWN:
	{
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
			PressedKeys[VK_XBUTTON1] = true;
		}
		else if (button == XBUTTON2)
		{
			PressedKeys[VK_XBUTTON2] = true;
		}
		break;
	}
	case WM_XBUTTONUP:
	{
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
			PressedKeys[VK_XBUTTON1] = false;
		}
		else if (button == XBUTTON2)
		{
			PressedKeys[VK_XBUTTON2] = false;
		}
		break;
	}
	case WM_KEYDOWN:
		PressedKeys[wParam] = true;
		break;
	case WM_KEYUP:
		PressedKeys[wParam] = false;
		break;
	default: break;
	}

	
	return CallWindowProc(g_Hooks.pOriginalWNDProc, hWnd, uMsg, wParam, lParam);
}



static bool _visible = true;

void MessageSend()
{
	write.SendClientHello();
	write.SendMatchmakingClient2GCHello();
}



void draw_hitboxes(C_BaseEntity* pEntity, int r, int g, int b, int a, float duration, float diameter)
{
	matrix3x4_t matrix[128];
	if (!pEntity->SetupBones(matrix, 128, 0x00000100, pEntity->GetSimulationTime()))
		return;
	studiohdr_t* hdr = g_pModelInfo->GetStudiomodel(pEntity->GetModel());
	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);
	for (int i = 0; i < set->numhitboxes; i++) {
		mstudiobbox_t* hitbox = set->GetHitbox(i);
		if (!hitbox)
			continue;
		Vector vMin, vMax;
		auto VectorTransform_Wrapperx = [](const Vector& in1, const matrix3x4_t &in2, Vector &out)
		{
			auto VectorTransform = [](const float *in1, const matrix3x4_t& in2, float *out)
			{
				auto DotProducts = [](const float *v1, const float *v2)
				{
					return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
				};
				out[0] = DotProducts(in1, in2[0]) + in2[0][3];
				out[1] = DotProducts(in1, in2[1]) + in2[1][3];
				out[2] = DotProducts(in1, in2[2]) + in2[2][3];
			};
			VectorTransform(&in1.x, in2, &out.x);
		};
		VectorTransform_Wrapperx(hitbox->min, matrix[hitbox->bone], vMin);
		VectorTransform_Wrapperx(hitbox->max, matrix[hitbox->bone], vMax);
		DebugOverlay->DrawPill(vMin, vMax, hitbox->radius, r, g, b, a, duration);
	}
}


#include "Features/bulletbeams/BeamInfo.hpp"
#include "SDK/IVRenderBeam.h"
#include <fnv.h>
void soundesp::draw_circle(Vector position)
{

	Color color_ = Color(esp::colors::sound_esp);

	BeamInfo_t beam_info;
	beam_info.m_nType = TE_BEAMRINGPOINT;
	beam_info.m_pszModelName = "sprites/purplelaser1.vmt";
	beam_info.m_nModelIndex = g_pModelInfo->GetModelIndex("sprites/purplelaser1.vmt");
	//beam_info.m_pszHaloName   = "sprites/purplelaser1.vmt";
	beam_info.m_nHaloIndex = -1;
	beam_info.m_flHaloScale = 5;
	beam_info.m_flLife = 1.50f;
	beam_info.m_flWidth = 10.f;
	beam_info.m_flFadeLength = 1.0f;
	beam_info.m_flAmplitude = 0.f;
    beam_info.m_flRed = color_.r();
	beam_info.m_flGreen = color_.g();
	beam_info.m_flBlue = color_.b();
	beam_info.m_flBrightness = color_.a();
	beam_info.m_flSpeed = 0.f;
	beam_info.m_nStartFrame = 0.f;
	beam_info.m_flFrameRate = 60.f;
	beam_info.m_nSegments = -1;
	//beam_info.m_bRenderable   = true;
	beam_info.m_nFlags = FBEAM_FADEOUT;
	beam_info.m_vecCenter = position + Vector(0, 0, 5);
	beam_info.m_flStartRadius = 20.f;
	beam_info.m_flEndRadius = 640.f;

	auto beam = g_pIViewRenderBeams->CreateBeamRingPoint(beam_info);

	if (beam)
		g_pIViewRenderBeams->DrawBeam(beam);

}

void soundesp::draw()
{
	if (!g_Options.esp.other.sound_esp)
		return;

	if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected())
		return;


	for (unsigned int i = 0; i < sound_logs.size(); i++)
	{
		g_soundesp.draw_circle(sound_logs[i].position);
		if (g_Options.esp.other.sound_esp)
			g_soundesp.draw_circle(sound_logs[i].position);
		sound_logs.erase(sound_logs.begin() + i);
	}
	
}
char* hitgroup_to_name(const int hitgroup) {
	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		return "head";
	case HITGROUP_CHEST:
		return "chest";
	case HITGROUP_STOMACH:
		return "stomach";
	case HITGROUP_LEFTARM:
		return "left arm";
	case HITGROUP_RIGHTARM:
		return "right arm";
	case HITGROUP_LEFTLEG:
		return "left leg";
	case HITGROUP_RIGHTLEG:
		return "right leg";
	default:
		return "body";
	}
}


typedef void(__cdecl* MsgFn)(const char* msg, va_list);

void Ms2423gX(const char* msg, ...)
{
	if (msg == nullptr)
		return; //If no string was passed, or it was null then don't do anything
	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg"); 	char buffer[989];
	va_list list;
	va_start(list, msg);
	vsprintf(buffer, msg, list);
	va_end(list);
	fn(buffer, list); //Calls the function, we got the address above.
}

static ConVar* right_hand = nullptr;
void C_HookedEvents::FireGameEvent(IGameEvent* event)
{
	if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
	{

		
        if (event) {
			auto userid = g_pEngine->GetPlayerForUserID(event->GetInt("userid"));
			PlayerInfo_t info;
			auto haskit = event->GetBool("haskit");
			int bombsite = event->GetInt("site");
			auto entity = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("userid")));
			int team = event->GetInt("team");
			auto weapon = event->GetString("weapon");
			int uid_index = g_pEngine->GetPlayerForUserID(event->GetInt("userid"));
			short weptype = event->GetInt("weptype");
			switch (fnv123::hashRuntime(event->GetName())) {
			case fnv123::hash("player_death"):
				g_Misc.KillMessage(event);
				break;
			case fnv123::hash("bomb_beginplant"):
				if (!g::pLocalEntity)
					return;
				if (g_Options.misc.eventlogs && g_pEngine->GetPlayerInfo(userid, &info))
					notifies::push(std::string("bomb is being planted by " + std::string(info.szName)));
				break;

			case fnv123::hash("bomb_begindefuse"):
				if (!g::pLocalEntity)
					return;
				if (g_Options.misc.eventlogs && g_pEngine->GetPlayerInfo(userid, &info))
					notifies::push(std::string("bomb is being defused " + std::string(haskit ? " with a kit " : "") + " by " + std::string(info.szName)));
				break;
			case fnv123::hash("bomb_abortplant"):
				if (!g::pLocalEntity)
					return;
				if (g_Options.misc.eventlogs && g_pEngine->GetPlayerInfo(userid, &info))
					notifies::push(std::string(std::string(info.szName) + " stopped planting the bomb"));
				break;
			case fnv123::hash("bomb_abortdefuse"):
				if (!g::pLocalEntity)
					return;
				if (g_Options.misc.eventlogs && g_pEngine->GetPlayerInfo(userid, &info))
					notifies::push(std::string(std::string(info.szName) + " stopped defusing the bomb"));
				break;
			case fnv123::hash("bomb_planted"):
				if (!g::pLocalEntity)
					return;
				if (g_Options.misc.eventlogs && g_pEngine->GetPlayerInfo(userid, &info))
					notifies::push(std::string("bomb has been planted by " + std::string(info.szName)));
				break;
			case fnv123::hash("item_purchase"):
				if (!g::pLocalEntity)
					return;
				if (g_Options.misc.eventlogs && g_pEngine->GetPlayerInfo(userid, &info))
					notifies::push(std::string(std::string(info.szName) + " bought " + weapon));
				break;
			case fnv123::hash("item_equip"):
				if (!g::pLocalEntity)
					return;

				if (g_Options.misc.misc_knife_switch && uid_index == g::pLocalEntity->EntIndex())
				{
					static bool was_knife = false;
					right_hand = g_pCvar->FindVar("cl_righthand");

					if (weptype == (int)CSWeaponType::WEAPONTYPE_KNIFE)
					{
						right_hand->SetValue(!right_hand->GetBool());
						was_knife = true;
					}
					else
					{
						if (was_knife)
						{
							right_hand->SetValue(!right_hand->GetBool());
							was_knife = false;
						}
					}

				}
				break;

			}
        }
		auto event_name = event->GetName();

		auto attacker = event->GetInt("attacker");
		auto user = event->GetInt("userid");
		std::string event_nameNEWW = event->GetName();

		if (event_nameNEWW.find("round_prestart") != std::string::npos || event_nameNEWW.find("round_end") != std::string::npos)
		{
			if (event_nameNEWW.find("round_end") != std::string::npos)
			{

				g::warmup = false;
			}
			else
			{

				g::warmup = true;
			}

		}

		if (event_nameNEWW.find("round_freeze_end") != std::string::npos)
		{
			g::warmup = false;
		}

		if (event_nameNEWW.find("round_end") != std::string::npos)
		{
			g::warmup = true;
		}

		if (g_Options.esp.other.hitmarker)
			otheresp::get().hitmarker_event(event);

		

		if (strstr(event->GetName(), "player_hurt"))
		{

			if (!event)
				return;

			if (!g::pLocalEntity)
				return;

			auto* attacker = (C_BaseEntity*)g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("attacker")));
			if (!attacker)
				return;

			if (attacker != g::pLocalEntity)
				return;

			auto entity = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("userid")));
			if (!entity) //IF they not a entity
				return;

			if (entity->GetTeam() == g::pLocalEntity->GetTeam()) //Fuck teammates
				return;
			


			PlayerInfo_s player_info;
			g_pEngine->GetPlayerInfo(g_pEngine->GetPlayerForUserID(event->GetInt("userid")), &player_info);

			Color DebugLagCompColor(esp::colors::shot_hitboxes);

			if (g_Options.esp.effects.shot_hitboxes)
				draw_hitboxes(entity, DebugLagCompColor.r(), DebugLagCompColor.g(), DebugLagCompColor.b(), DebugLagCompColor.a(), 1, 0);

			float fLastShotFiredTime;

			if (g_pGlobalVars->curtime == fLastShotFiredTime)
				return;

			if (g_Aimbot.aimbotted) //Checks to see if it was the aimbot who fired
			{
				g_Aimbot.data[entity->EntIndex()].shoots_hit++; //Add to our logs

			}
			fLastShotFiredTime = g_pGlobalVars->curtime;
			if (g_Options.misc.eventlogs)
				notifies::push(std::string("Hurt " + std::string(player_info.szName) + " in the " + hitgroup_to_name(event->GetInt("hitgroup")) + " for " + std::to_string(event->GetInt("dmg_health")) + " " + std::string(player_info.szName) + " has " + std::to_string(event->GetInt("health")) + " health remaining."), notify_state_s::debug_state);
				//_events.push_back(_event(g_pGlobalVars->curtime + 5.f, std::string("Hurt " + std::string(player_info.szName) + " in the " + hitgroup_to_name(event->GetInt("hitgroup")) + " for " + std::to_string(event->GetInt("dmg_health")) + " " + std::string(player_info.szName) + " has " + std::to_string(event->GetInt("health")) + " health remaining.")));

			/*
			if (Shonax.Visuals.drawcapsule)
			{
				C_BaseEntity* hittedplayer = (C_BaseEntity*)g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("userid")));

				if (hittedplayer && hittedplayer->EntIndex() > 0 && hittedplayer->EntIndex() < 64)
				{
					if (g::pLocalEntity && hittedplayer != g::pLocalEntity)
						CapsuleOverlay(hittedplayer, Color(Shonax.Visuals.capsule_col[0] * 255, Shonax.Visuals.capsule_col[1] * 255, Shonax.Visuals.capsule_col[2] * 255), 1.0f);
				}
			}*/

			
			if (g_Options.esp.other.sound_esp)
			{

				/* get the attacker */
				auto attacker123 = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("attacker")));
				if (!attacker123)
					return;
				/* get the victim */
				auto victim = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("userid")));
				if (!victim)
					return;
				/* clock */
				static int timer;

				timer += 1;

				if (timer > 2)
					timer = 0;

				if (timer < 1)
				{

					if (attacker123 == g::pLocalEntity)
						sound_logs.push_back(sound_info(victim->GetAbsOrigin(), g_pGlobalVars->curtime, event->GetInt("userid")));
					//else if (attacker123 == g::pLocalEntity)
					//	sound_logs_local.push_back(sound_info(victim->GetAbsOrigin(), g_pGlobalVars->curtime, event->GetInt("userid")));
				}


			}


			switch (g_Options.esp.other.hitmark_sound)
			{
			case 0:
				break;
			case 1:
				g_pSurface->PlaySound_("buttons\\arena_switch_press_02.wav");
				break;
			case 2:
				PlaySoundA(bubble, NULL, SND_ASYNC | SND_MEMORY);
				break;
			case 3:
				PlaySoundA(bameware_sound, NULL, SND_ASYNC | SND_MEMORY);
				break;
			case 4:
				PlaySoundA(rifk, NULL, SND_ASYNC | SND_MEMORY);
				break;
			case 5:
				PlaySoundA(quake, NULL, SND_ASYNC | SND_MEMORY);
				break;
			case 6:
				PlaySoundA(unreal, NULL, SND_ASYNC | SND_MEMORY);
				break;
			default:
				break;
			}


		}



		if (strstr(event->GetName(), "player_footstep") && g_pEngine->IsInGame() && g_pEngine->IsConnected() && g::pLocalEntity)
		{

			////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////


			/* check if we are in game */
			if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected())
				return;

			/* check if we have the sound esp enabled */
			if (!g_Options.esp.other.sound_esp)
				return;

			/* return if not event */
			if (!event)
				return;


			/* get the walker */
			auto walker = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("userid")));

			/* return if walker is nullptr */
			if (walker == nullptr)
				return;

			/* return if walker is dormant */
			if (walker->IsDormant())
				return;

			/* clock */
			static int timer;

			timer += 1;

			if (timer > 1)
				timer = 0;


			if (walker->GetTeam() != g::pLocalEntity->GetTeam())
			{
				if (walker && timer < 1)
				{
					sound_logs.push_back(sound_info(walker->GetAbsOrigin(), g_pGlobalVars->curtime, event->GetInt("userid")));
				}
			}

			if (walker == g::pLocalEntity)
			{
				if (walker && timer < 1)
				{
					//sound_logs_local.push_back(sound_info(walker->GetAbsOrigin(), g_pGlobalVars->curtime, event->GetInt("userid")));
				}
			}


		}
		if (strcmp(event->GetName(), "player_death") == 0)
		{
			if (!g::pLocalEntity || !g_pEngine->IsInGame() || !g_pEngine->IsConnected())
				return;
			int uid = event->GetInt("userid");
			int uid_index = g_pEngine->GetPlayerForUserID(uid);
			if (uid_index == g::pLocalEntity->EntIndex())
				Fullupdate();
		}

		if (strcmp(event->GetName(), "player_death") == 0)
		{
			if (!event)
				return;

			if (!g::pLocalEntity)
				return;

			auto entity = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("userid")));
			if (!entity) //IF they not a entity
				return;

			if (entity->GetTeam() == g::pLocalEntity->GetTeam()) //Fuck teammates
				return;
			if (entity == g::pLocalEntity)
				Fullupdate();
			PlayerInfo_s player_info;
			g_pEngine->GetPlayerInfo(g_pEngine->GetPlayerForUserID(event->GetInt("userid")), &player_info);
			
			g_Aimbot.data[entity->EntIndex()].shoots_hit = 0;
			g_Aimbot.data[entity->EntIndex()].shoots = 0;
			g_Aimbot.data[entity->EntIndex()].shotsmissed = 0;

			auto attacker = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("attacker")));
			if (!attacker)
				return;

			if (attacker != g::pLocalEntity)
				return;
			if (g_Options.esp.other.kill_counter)
			{
				g::killtime = g_pGlobalVars->realtime;
				g::kills++;
			}
			g::globalAlpha = g_pGlobalVars->curtime;

		}

		
		
        if (g_Options.esp.effects.beam)
			bullettracers::get().events(event);

		if (!strcmp(event->GetName(), "round_start") && g_pEngine->IsInGame() && g_pEngine->IsConnected() && g::pLocalEntity)
		{

			if (g_Options.misc.autobuy)
			{
				auto buybotoptions = g_Options.misc.autobuy_selected;

				if (buybotoptions[0])
					g_pEngine->ExecuteClientCmd("buy ak47;");
				if (buybotoptions[1])
					g_pEngine->ExecuteClientCmd("buy m4a1;");
				if (buybotoptions[2])
					g_pEngine->ExecuteClientCmd("buy awp;");
				if (buybotoptions[3])
					g_pEngine->ExecuteClientCmd("buy ssg08;");
				if (buybotoptions[4])
					g_pEngine->ExecuteClientCmd("buy scar20;");
				if (buybotoptions[5])
					g_pEngine->ExecuteClientCmd("buy g3sg1;");
				if (buybotoptions[6])
					g_pEngine->ExecuteClientCmd("buy elite;");
				if (buybotoptions[7])
					g_pEngine->ExecuteClientCmd("buy flashbang;");
				if (buybotoptions[8])
					g_pEngine->ExecuteClientCmd("buy smokegrenade;");
				if (buybotoptions[9])
					g_pEngine->ExecuteClientCmd("buy hegrenade;");
				if (buybotoptions[10])
					g_pEngine->ExecuteClientCmd("buy molotov;");
				if (buybotoptions[11])
					g_pEngine->ExecuteClientCmd("buy incgrenade;");
				if (buybotoptions[12])
					g_pEngine->ExecuteClientCmd("buy vest;");
				if (buybotoptions[13])
					g_pEngine->ExecuteClientCmd("buy vesthelm;");
				if (buybotoptions[14])
					g_pEngine->ExecuteClientCmd("buy defuser;");
				if (buybotoptions[15])
					g_pEngine->ExecuteClientCmd("buy Taser;");

			}


			if (!event)
				return;

			if (!g::pLocalEntity)
				return;

			g::kills = 0;

			auto entity = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("userid")));
			if (!entity) //IF they not a entity
				return;

			if (entity->GetTeam() == g::pLocalEntity->GetTeam()) //Fuck teammates
				return;


			g_Aimbot.data[entity->EntIndex()].shoots_hit = 0;
			g_Aimbot.data[entity->EntIndex()].shoots = 0;
			g_Aimbot.data[entity->EntIndex()].shotsmissed = 0;
		}
	}

	bullettracers::get().events(event);

}

int C_HookedEvents::GetEventDebugID(void)
{
	return EVENT_DEBUG_ID_INIT;
}

void C_HookedEvents::RegisterSelf()
{
	g_pEventManager->AddListener(this, "round_end", false);
	g_pEventManager->AddListener(this, "player_footstep", false);
	g_pEventManager->AddListener(this, "player_jump", false);
	g_pEventManager->AddListener(this, "round_start", false);
	g_pEventManager->AddListener(this, "player_hurt", false);
	g_pEventManager->AddListener(this, "weapon_fire", false);
	g_pEventManager->AddListener(this, "item_purchase", false);
	g_pEventManager->AddListener(this, "bomb_begindefuse", false);
	g_pEventManager->AddListener(this, "bullet_impact", false);
	g_pEventManager->AddListener(this, "round_prestart", false);
	g_pEventManager->AddListener(this, "round_freeze_end", false);
	g_pEventManager->AddListener(this, "player_death", false);
	g_pEventManager->AddListener(this, "item_equip", false);
	g_pEventManager->AddListener(this, "bomb_beginplant", false);
	g_pEventManager->AddListener(this, "bomb_begindefuse", false); 
	g_pEventManager->AddListener(this, "bomb_abortplant", false);//bomb_abortdefuse
	g_pEventManager->AddListener(this, "bomb_abortdefuse", false);//bomb_planted
	g_pEventManager->AddListener(this, "bomb_planted", false);
}

void C_HookedEvents::RemoveSelf()
{
	g_pEventManager->RemoveListener(this);
}



void __fastcall Hooks::Hooked_findmdl(void* ecx, void* edx, char* FilePath)
{

	
	//its for me model only in video xd
    //u are gay bro
	return oFindMDL_t->GetTrampoline()(ecx, FilePath);
}
void Hooks::ENGINE()
{
	// Get window handle
	while (!(g_Hooks.hCSGOWindow = FindWindowA("Valve001", nullptr)))
	{
		using namespace std::literals::chrono_literals;
		std::this_thread::sleep_for(50ms);
	}

	// Get interfaces
	// Get netvars after getting interfaces as we use them
	NetvarManager::Instance()->CreateDatabase();
	OFFSETS::InitOffsets();
	const uintptr_t d3dDevice = **reinterpret_cast<uintptr_t**>(Utils::FindSignature("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);


	if (g_Hooks.hCSGOWindow)        // Hook WNDProc to capture mouse / keyboard input
		g_Hooks.pOriginalWNDProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(g_Hooks.hCSGOWindow, GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(g_Hooks.WndProc)));
}
void Hooks::FONTS()
{
	g::key_font = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::key_font, "Arial", 22, 700, 0, 0, 0);

	g::TahomaSmall = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::TahomaSmall, "Tahoma", 12, 400, 0, 0, FONTFLAG_DROPSHADOW);

	g::Tahoma = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::Tahoma, "Tahoma", 12, 400, 0, 0, FONTFLAG_OUTLINE);

	g::Arial = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::Arial, "Arial", 35, 700, 0, 0, 0);

	g::MenuBold = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::MenuBold, "Arial", 14, 900, 0, 0, FONTFLAG_ANTIALIAS);

	g::Untitled1 = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::Untitled1, "AstriumWep", 14, 500, 0, 0, FONTFLAG_ANTIALIAS);

	g::Menu = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::Menu, "Arial", 14, 500, 0, 0, FONTFLAG_ANTIALIAS);

	g::Tabs = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::Tabs, "menu_font", 30, 400, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);

	g::grenadefont = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::grenadefont, "astriumwep", 16, 400, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS);

	g::pixelfont = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::pixelfont, "Tahoma", 11, 100, 0, 0, FONTFLAG_OUTLINE);

	g::visuals_lby_font = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::visuals_lby_font, "Verdana", 25, 650, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);

	g::LBY2 = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::LBY2, "Verdana", 44, 900, 0, 0, FONTFLAG_ANTIALIAS);

	g::LBY1 = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::LBY1, "Verdana", 30, 900, 0, 0, FONTFLAG_ANTIALIAS);

	g::menu_window_font3 = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::menu_window_font3, "Choktoff", 12, 0, 0, 10, FONTFLAG_NONE);

	g::SmallText = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::SmallText, "Tahoma", 12, 4000, 0, 10, FONTFLAG_DROPSHADOW);

	g::CourierNew = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::CourierNew, "Tahoma", 12, 500, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);

	g::WeaponIcon = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::WeaponIcon, "Counter-Strike", 24, 400, 0, 0, FONTFLAG_ANTIALIAS);

	g::WeaponESP = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::WeaponESP, "Small Fonts", 8, 400, 0, 0, FONTFLAG_OUTLINE);

	g::xd = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::xd, "Smallest87", 11, 120, 0, 0, FONTFLAG_OUTLINE);

	g::INDICATOR = g_pSurface->FontCreate();
	g_pSurface->SetFontGlyphSet(g::INDICATOR, "Verdana", 15, 600, 0, 0, FONTFLAG_ANTIALIAS);

}