#pragma once

#include "Utils\Interfaces.h"
#include "SDK\IBaseClientDll.h"
#include "SDK\IClientMode.h"
#include "SDK\ISurface.h"
#include "SDK\IGameEvent.h"
#include "SDK\CInput.h"
#include "SDK\CModelRender.h"
#include "SDK\IVModelInfo.h"
#include "SDK\IMaterial.h"
#include "SDK\IVRenderView.h"
#include <assert.h>
#include <d3d9.h>
extern bool   g_Unload;
extern int m_nTickbaseShift;


class C_HookedEvents : public IGameEventListener {
public:
	void FireGameEvent(IGameEvent * event);
	void RegisterSelf();
	void RemoveSelf();
	int GetEventDebugID(void);
};
extern C_HookedEvents hooked_events;



namespace vtable_indexes
{
	constexpr auto view         = 18;
	constexpr auto GGetViewModelFOV = 35;
	constexpr auto paint        = 41;
	constexpr auto createMove   = 24;
	constexpr auto lockCursor   = 67;
	constexpr auto frameStage   = 37;
	constexpr auto dme          = 21;
	constexpr auto extraBonePro	= 192;
	constexpr auto sceneEnd     = 9;
	constexpr auto mousecode = 92;
	constexpr auto keycode = 91;
	constexpr auto event_scene = 9;
	constexpr auto send_message = 0;
	constexpr auto retrieve_message = 2;
	constexpr auto model_cache = 10;
	constexpr auto d3d9menu = 42;
    constexpr auto reset = 16;
    constexpr auto sound = 5;

	
}
enum EGCResultxd
{
	k_EGCResultOKxd = 0,
	k_EGCResultNoMessagexd = 1,           // There is no message in the queue
	k_EGCResultBufferTooSmallxd = 2,      // The buffer is too small for the requested message
	k_EGCResultNotLoggedOnxd = 3,         // The client is not logged onto Steam
	k_EGCResultInvalidMessagexd = 4,      // Something was wrong with the message being sent with SendMessage
};

class Protobuffs;
extern Protobuffs write;

class IMatRenderContext;
class CBoneBitList;
class ShadowVTManager;
class Hooks
{
public:
    // Initialization setup, called on injection
    static void Init();
    static void Restore();
	static void HookPlayers();

    /*---------------------------------------------*/
    /*-------------Hooked functions----------------*/
    /*---------------------------------------------*/

	static void     __stdcall   FrameStageNotify(ClientFrameStage_t curStage);
    static bool     __fastcall  CreateMove(IClientMode*, void*, float, CUserCmd*);
    static void     __fastcall  LockCursor(ISurface*, void*);
	static void		__fastcall  PaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
	static void     __fastcall  OverrideView(void* ecx, void* edx, CViewSetup* pSetup);
	static float __stdcall GGetViewModelFOV();
	static HRESULT  __stdcall   Present(IDirect3DDevice9* pDevice, const RECT *pSourceRect, const RECT *pDestRect, HWND hDestWindowOverride, const RGNDATA *pDirtyRegion);

	static void     __fastcall  DrawModelExecute(void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& render_info, matrix3x4_t* matrix);
	static void     __fastcall  DoExtraBonesProcessing(void * ECX, void * EDX, void * unkn1, void * unkn2, void * unkn3, void * unkn4, CBoneBitList & unkn5, void * unkn6);
	static void     __fastcall  SceneEnd(void *ecx, void *edx);
    static LRESULT  __stdcall   WndProc   (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static bool __fastcall Hooked_FireEventClientSide(void *ecx, void* edx, IGameEvent* pEvent);
	static void __fastcall Hooked_findmdl(void* ecx, void* edx, char* FilePath);
	static bool __fastcall setup_bones(IClientRenderable* ecx, void* edx, matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
	static bool __fastcall hkdWriteUsercmdDeltaToBuffer(void* ecx, void*, int slot, bf_write* buf, int from, int to, bool isnewcommand);
	static void     __stdcall   Hooked_EndScene(IDirect3DDevice9* pDevice);
    static long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
    static void __fastcall hkReadPacketEntities(void* pClientState, void* pEDX, void* pEntInfo);
    static void __stdcall hkEmitSound1(IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk);
	static void ENGINE();
	static void FONTS();
	static int __stdcall doPostScreenEffects(int param);
	static bool __fastcall IsConnected(void* ecx, void* edx);
	static int _fastcall SendDatagram(void* netchan, void* ecx);
	static bool __fastcall SendNetMsg(INetChannel* pNetChan, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice);
private:
    /*---------------------------------------------*/
    /*-------------VMT Hook pointers---------------*/
    /*---------------------------------------------*/

	

	std::unique_ptr<ShadowVTManager> pPlayerHook[65];

    /*---------------------------------------------*/
    /*-------------Hook prototypes-----------------*/
    /*---------------------------------------------*/
public:
	using GCRetrieveMessage = EGCResultxd(__thiscall*)(void*, uint32_t *punMsgType, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize);
	using GCSendMessage = EGCResultxd(__thiscall*)(void*, uint32_t unMsgType, const void* pubData, uint32_t cubData);
    using EmitSound1 = void(__thiscall*)(void*, IRecipientFilter&, int, int, const char*, unsigned int, const char*, float, int, float, int, int, const Vector*, const Vector*, void*, bool, float, int, int);
	using FindMDL_t = void(__thiscall*)(void*, char*);
    using ReadPacketEntities = void(__thiscall*)(void*, void*);
	typedef bool(__thiscall *FireEventClientSide)(void*, IGameEvent*);
	typedef void (__stdcall*  FrameStageNotify_t) (ClientFrameStage_t);
    typedef bool (__fastcall* CreateMove_t) (IClientMode*, void*, float, CUserCmd*);
    typedef void (__fastcall* LockCursor_t) (ISurface*, void*);
	typedef void (__thiscall* PaintTraverse_t) (PVOID, unsigned int, bool, bool);
	typedef void (__fastcall* OverrideView_t) (void*, void*, CViewSetup*);
	typedef float(__stdcall *oGetViewModelFOV)();
	typedef long(__stdcall*  Present_t)    (IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
	typedef long(__stdcall*  Reset_t)      (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	typedef void (__thiscall* DrawModelExecute_t) (void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
	typedef void (__thiscall* ExtraBoneProcess_t) (void*, void*, void*, void*, void*, CBoneBitList&, void*);
	typedef void (__fastcall* SceneEnd_t) (void*, void*);
	typedef void(__stdcall *EndSceneFn)(IDirect3DDevice9* device);
    typedef long(__stdcall* EndSceneResetFn)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	typedef long(__stdcall*  D3D9FEATURES)    (IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
	typedef bool(__thiscall* SetupBones_t)(IClientRenderable*, matrix3x4_t*, int, int, float);
	typedef bool (__thiscall* WriteUsercmdDeltaToBufferFn) (void*, int, void*, int, int, bool);
	typedef void(__stdcall* EndSceneFn)(IDirect3DDevice9* device);



public:
    HWND                           hCSGOWindow             = nullptr; // CSGO window handle
    bool                           bInitializedDrawManager = false;   // Check if we initialized our draw manager
    WNDPROC                        pOriginalWNDProc        = nullptr; // Original CSGO window proc

	template<class Type> // GLAD
	Type HookManual(uintptr_t *instance, int offset, Type hook)
	{
		DWORD Dummy;
		Type fnOld = (Type)instance[offset];
		VirtualProtect((void*)(instance + offset * 0x4), 0x4, PAGE_EXECUTE_READWRITE, &Dummy);
		instance[offset] = (uintptr_t)hook;
		VirtualProtect((void*)(instance + offset * 0x4), 0x4, Dummy, &Dummy);
		return fnOld;
	}
};

extern Hooks g_Hooks;



class ProtectGuard
{
public:

	ProtectGuard(void *base, uint32_t len, uint32_t protect)
	{
		this->base = base;
		this->len = len;

		if (!VirtualProtect(base, len, protect, (PDWORD)&old_protect))
			throw std::runtime_error("Failed to protect region!");
	}

	~ProtectGuard()
	{
		VirtualProtect(base, len, old_protect, (PDWORD)&old_protect);
	}

private:

	void *base;
	uint32_t len;
	uint32_t old_protect;
};

class ShadowVTManager // GLAD
{

public:

	ShadowVTManager() : class_base(nullptr), method_count(0), shadow_vtable(nullptr), original_vtable(nullptr) {}
	ShadowVTManager(void *base) : class_base(base), method_count(0), shadow_vtable(nullptr), original_vtable(nullptr) {}
	~ShadowVTManager()
	{
		RestoreTable();

		delete[] shadow_vtable;
	}

	inline void Setup(void *base = nullptr)
	{
		if (base != nullptr)
			class_base = base;

		if (class_base == nullptr)
			return;

		original_vtable = *(uintptr_t**)class_base;
		method_count = GetMethodCount(original_vtable);

		if (method_count == 0)
			return;

		shadow_vtable = new uintptr_t[method_count + 1]();

		shadow_vtable[0] = original_vtable[-1];
		std::memcpy(&shadow_vtable[1], original_vtable, method_count * sizeof(uintptr_t));

		try
		{
			auto guard = ProtectGuard{ class_base, sizeof(uintptr_t), PAGE_READWRITE };
			*(uintptr_t**)class_base = &shadow_vtable[1];
		}
		catch (...)
		{
			delete[] shadow_vtable;
		}
	}

	template<typename T>
	inline void Hook(uint32_t index, T method)
	{
		assert(index < method_count);
		shadow_vtable[index + 1] = reinterpret_cast<uintptr_t>(method);
	}

	inline void Unhook(uint32_t index)
	{
		assert(index < method_count);
		shadow_vtable[index + 1] = original_vtable[index];
	}

	template<typename T>
	inline T GetOriginal(uint32_t index)
	{
		return (T)original_vtable[index];
	}

	inline void RestoreTable()
	{
		try
		{
			if (original_vtable != nullptr)
			{
				auto guard = ProtectGuard{ class_base, sizeof(uintptr_t), PAGE_READWRITE };
				*(uintptr_t**)class_base = original_vtable;
				original_vtable = nullptr;
			}
		}
		catch (...) {}
	}

private:

	inline uint32_t GetMethodCount(uintptr_t *vtable_start)
	{
		uint32_t len = -1;

		do ++len; while (vtable_start[len]);

		return len;
	}

	void *class_base;
	uint32_t method_count;
	uintptr_t *shadow_vtable;
	uintptr_t *original_vtable;
};
extern void updateClanTag(bool tagChanged);
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern void update_map();