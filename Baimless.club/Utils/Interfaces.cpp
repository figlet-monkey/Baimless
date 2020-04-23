#include "Interfaces.h"
#include "Utils.h"

#include "..\SDK\IClientMode.h"
#include "..\SDK\IBaseClientDll.h"
#include "..\SDK\IClientEntityList.h"
#include "..\SDK\IVEngineClient.h"
#include "..\SDK\CPrediction.h"
#include "..\SDK\IGameEvent.h"
#include "..\SDK\ISurface.h"
#include "..\SDK\IEngineTrace.h"
#include "..\SDK\ISurfaceData.h"
#include "..\SDK\ICvar.h"
#include "../SDK/ILocalize.h"
#include "..\SDK\IVModelInfo.h"
#include "..\SDK\CModelRender.h"
#include "..\SDK\IMaterial.h"
#include "..\SDK\IVRenderView.h"
#include "../SDK/InputSystem.h"
#include "../sdk/IVRenderBeam.h"
#include "../Features/rankchanger/rankchanger.h"

#include "../Features/rankchanger/steam.h"
#include "../Features/rankchanger/IMemAlloc.h"
#include "../SDK/IMDLCache.h"
#include "..\Utils\GlobalVars.h"
#include "../Render2.h"
#include "../SDK/IEngineSound.h"
#include <fstream>



void* CInterfaceUtils::bruteforceInterface(void* modulePtr, const std::string &interfaceName)
{
	void* result = nullptr;
	auto createInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)modulePtr, "CreateInterface");

	if (createInterface == nullptr)
		return nullptr;

	for (int i = 0; i < 1000; i++)
	{
		char nameBuffer[128];
		sprintf_s(nameBuffer, "%s%03i", interfaceName.c_str(), i);

		result = createInterface(nameBuffer, nullptr);

		if (result != nullptr)
			break;
	}

	return nullptr;
}

void *CInterfaceUtils::bruteforceInterface(const char *mod, const char *iface) {
	// LoL
	auto CaptureInterface = [](const char* mod, const char* iface, bool print_if_nullptr) -> void*
	{
		using fn_capture_iface_t = void*(*)(const char*, int*);
		auto fn_addr = (fn_capture_iface_t)GetProcAddress(GetModuleHandleA(mod), "CreateInterface");

		auto iface_addr = fn_addr(iface, nullptr);

		if (iface_addr != nullptr || print_if_nullptr)
			printf("found %s at 0x%p\n", iface, fn_addr(iface, nullptr));

		return iface_addr;
	};

	for (int i = 0; i < 1000; i++) {
		char buffer[128];
		sprintf_s(buffer, "%s%03i", iface, i);
		void *result = CaptureInterface(mod, buffer, i == 999);
		if (result != nullptr)
			return result;
	}
	return nullptr;
}

void* CInterfaceUtils::grabInterface2(void* modulePtr, const std::string & interfaceName)
{
	if (isdigit(interfaceName[interfaceName.length() - 1]) &&
		isdigit(interfaceName[interfaceName.length() - 2]) &&
		isdigit(interfaceName[interfaceName.length() - 3]))
	{
		auto createInterface = (CreateInterfaceFn)GetProcAddress((HMODULE)modulePtr, "CreateInterface");

		if (createInterface == nullptr)
			return nullptr;

		return createInterface(interfaceName.c_str(), nullptr);
	}
	else
	{
		return this->bruteforceInterface(modulePtr, interfaceName);
	}

}

void* CInterfaceUtils::grabModuleTimeout(const std::string &moduleName, int timeout)
{
	using namespace std::chrono;

	void* result = nullptr;
	auto beginTime = system_clock::now();

	while (duration_cast<seconds>(system_clock::now() - beginTime).count() <= timeout)
	{
		result = GetModuleHandleA(moduleName.c_str());

		if (result != nullptr)
			break;
	}

	return result;
}

void* CInterfaceUtils::grabModule(const std::string &moduleName)
{
	return this->grabModuleTimeout(std::move(moduleName));
}
void* CInterfaceUtils::grabInterface(const std::string &moduleName, const std::string &interfaceName)
{
	return this->grabInterface2(this->grabModule(std::move(moduleName)), std::move(interfaceName));
}

uintptr_t* CInterfaceUtils::getVfuncTablegay(void *interfacePtr)
{
	return *reinterpret_cast<uintptr_t**>(interfacePtr);
}

CInterfaceUtils* g_InterfaceUtils = new CInterfaceUtils();

#define enc_str(s) std::string(s)
#define enc_char(s) enc_str(s).c_str()
#define enc_wstr(s) std::wstring(enc_str(s).begin(), enc_str(s).end())
#define enc_wchar(s) enc_wstr(s).c_str()




// Initializing global interfaces
IBaseClientDLL*     g_pClientDll    = nullptr;
IClientMode*        g_pClientMode   = nullptr;
IClientEntityList*  g_pEntityList   = nullptr;
IVEngineClient*     g_pEngine       = nullptr;
CGameUI*            gameUI          = nullptr;
ILocalize*          g_Localize      = nullptr;
CPrediction*        g_pPrediction   = nullptr;
IGameMovement*      g_pMovement     = nullptr;
IMoveHelper*        g_pMoveHelper   = nullptr;
CGlobalVarsBase*    g_pGlobalVars   = nullptr;
IGameEventManager*  g_pEventManager = nullptr;
ISurface*           g_pSurface      = nullptr;
IEngineTrace*       g_pTrace        = nullptr;
IPhysicsSurfaceProps* g_pSurfaceData = nullptr;
ICVar*              g_pCvar			= nullptr;
IPanel*				g_pPanel		= nullptr;
IVModelInfo*		g_pModelInfo	= nullptr;
CModelRender*       g_pModelRender  = nullptr;
IMaterialSystem*    g_pMaterialSys  = nullptr;
IVRenderView*       g_pRenderView   = nullptr;
IViewRenderBeams* g_pIViewRenderBeams = nullptr;
IInputSystem*       g_pInputSystem = nullptr;
ISteamGameCoordinator* g_SteamGameCoordinator = nullptr;
IMemAlloc*  		   g_pMemAlloc = nullptr;
ISteamUser*            g_SteamUser = nullptr;
IMDLCache* g_pModelCache = nullptr;
CClientState* g_pClientState = nullptr;
IVDebugOverlay* DebugOverlay = nullptr;
CGlowObjectManager* m_pGlowObjManager = nullptr;
DWORD WriteUsercmdDeltaToBufferReturn;
CInput* g_Input = nullptr;
IDirect3DDevice9* g_D3DDevice9 = nullptr;
Engine::CRender* g_pRender = nullptr;
IEngineSound* g_EngineSound = nullptr;
CGameRules** g_pGameRules = nullptr;
std::add_pointer_t<bool __stdcall(const char*)> acceptMatch;




template <typename T>
static constexpr auto relativeToAbsolute(int* address) noexcept
{
    return reinterpret_cast<T>(reinterpret_cast<char*>(address + 1) + *address);
}
namespace interfaces
{
	template< class T >
	T* FindClass(std::string szModuleName, std::string szInterfaceName, bool bSkip = false)
	{
		if (szModuleName.empty() || szInterfaceName.empty())
			return nullptr;
		typedef PVOID(*CreateInterfaceFn)(const char* pszName, int* piReturnCode);
		CreateInterfaceFn hInterface = nullptr;
		while (!hInterface)
		{
			hInterface = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(szModuleName.c_str()), "CreateInterface");
			Sleep(5);
		}

		char pszBuffer[256];
		for (int i = 0; i < 100; i++)
		{
			sprintf_s(pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i);
			PVOID pInterface = hInterface(pszBuffer, nullptr);

			if (pInterface && pInterface != NULL)
			{
				if (bSkip)
					sprintf_s(pszBuffer, "%s%0.3d", szInterfaceName.c_str(), i + 1);

				Sleep(5);
				break;
			}
		}

		return (T*)hInterface(pszBuffer, nullptr);
	}

	CreateInterfaceFn get_module_factory(HMODULE module)
	{
		return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(module, "CreateInterface"));
	}

	template<typename T>
	T* get_interface(CreateInterfaceFn f, const char* szInterfaceVersion)
	{
		auto result = reinterpret_cast<T*>(f(szInterfaceVersion, nullptr));

		if (!result) {
			throw std::runtime_error(std::string("[get_interface] Failed to GetOffset interface: ") + szInterfaceVersion);
		}

		return result;
	}

	template<typename T>
	T* CaptureInterface(const char* szModuleName, const char* szInterfaceVersion)
	{
		HMODULE moduleHandle = GetModuleHandleA(szModuleName);
		if (moduleHandle)   /* In case of not finding module handle, throw an error. */
		{
			CreateInterfaceFn pfnFactory = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(moduleHandle, "CreateInterface"));
			return reinterpret_cast<T*>(pfnFactory(szInterfaceVersion, nullptr));
		}
		Utils::Log("Error getting interface %", szInterfaceVersion);
		return nullptr;
	}

    void Init()
    {
		gameUI			= (CGameUI*)g_InterfaceUtils->grabInterface("client_panorama.dll", "GameUI011");
		//gameUI        = FindClass<CGameUI>("client_panorama.dll", "GameUI011");
        g_pClientDll    = FindClass<IBaseClientDLL>("client_panorama.dll", "VClient");
        g_pEntityList   = FindClass<IClientEntityList>("client_panorama.dll", "VClientEntityList");
        g_pEngine       = FindClass<IVEngineClient>("engine.dll", "VEngineClient");
        g_pPrediction   = FindClass<CPrediction>("client_panorama.dll", "VClientPrediction");
        g_pMovement     = FindClass<IGameMovement>("client_panorama.dll", "GameMovement");
		g_pModelCache	= FindClass<IMDLCache>("datacache.dll", "MDLCache");
        g_pEventManager = CaptureInterface<IGameEventManager>("engine.dll", "GAMEEVENTSMANAGER002");
        g_pSurface      = FindClass<ISurface>("vguimatsurface.dll", "VGUI_Surface");
		g_pTrace        = FindClass<IEngineTrace>("engine.dll", "EngineTraceClient");
		DebugOverlay    = FindClass<IVDebugOverlay>("engine.dll", "VDebugOverlay");
		g_pSurfaceData  = FindClass<IPhysicsSurfaceProps>("vphysics.dll", "VPhysicsSurfaceProps");
		g_pCvar         = FindClass<ICVar>("vstdlib.dll", "VEngineCvar");
		g_pPanel		= FindClass<IPanel>("vgui2.dll", "VGUI_Panel");
		g_pModelInfo    = FindClass<IVModelInfo>("engine.dll", "VModelInfoClient");
		g_pModelRender  = FindClass<CModelRender>("engine.dll", "VEngineModel");
		g_pMaterialSys  = FindClass<IMaterialSystem>("materialsystem.dll", "VMaterialSystem");
		g_pRenderView   = FindClass<IVRenderView>("engine.dll", "VEngineRenderView");
        g_Localize      = FindClass<ILocalize>("localize.dll", "Localize_");
		g_pInputSystem  = FindClass<IInputSystem>("inputsystem.dll", "InputSystemVersion");
		g_EngineSound	= FindClass<IEngineSound>("engine.dll", "IEngineSoundClient");

		do {
			g_pClientMode				= **(IClientMode***)((*(uintptr_t**)g_pClientDll)[10] + 0x5);
		} while (!g_pClientMode);
		g_pGlobalVars					= **(CGlobalVarsBase***)((*(uintptr_t**)g_pClientDll)[0] + 0x1B);
		//g_pClientMode					= **(IClientMode***)((*(uintptr_t**)g_pClientDll)[10] + 0x5);//**reinterpret_cast<IClientMode***>    ((*reinterpret_cast<uintptr_t**>(g_pCl	ientDll))[10] + 0x5u);
		//g_pGlobalVars					= **(CGlobalVarsBase***)((*(uintptr_t**)g_pClientDll)[0] + 0x1B);//**reinterpret_cast<CGlobalVarsBase***>((*reinterpret_cast<uintptr_t**>(g_pClientDll))[0] + 0x1Bu);
		g_pIViewRenderBeams				= *reinterpret_cast< IViewRenderBeams ** >(Utils::FindSignature("client_panorama.dll", "B9 ? ? ? ? A1 ? ? ? ? FF 10 A1 ? ? ? ? B9") + 1);
		ISteamClient* SteamClient		= ((ISteamClient*(__cdecl*)(void))GetProcAddress(GetModuleHandleA("steam_api.dll"), "SteamClient"))();
		g_SteamGameCoordinator			= (ISteamGameCoordinator*)SteamClient->GetISteamGenericInterface((void*)1, (void*)1, "SteamGameCoordinator001");
		g_pMemAlloc						= *(IMemAlloc**)GetProcAddress(GetModuleHandleW(L"tier0.dll"), "g_pMemAlloc");
		g_SteamUser						= SteamClient->GetISteamUser((void*)1, (void*)1, "SteamUser019");
		g_pMoveHelper					= **reinterpret_cast<IMoveHelper***>((Utils::FindSignature("client_panorama.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 0x2));
		g_pClientState					= **(CClientState***)(Utils::FindSignature("engine.dll", "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
		m_pGlowObjManager				= *(CGlowObjectManager**)(Utils::FindSignature("client_panorama.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 3);
		g::g_iModelBoneCounter			= **reinterpret_cast<unsigned long**>(Utils::FindSignature("client_panorama.dll", "80 3D ? ? ? ? 00 74 16 A1") + 10); //80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81
		WriteUsercmdDeltaToBufferReturn = (DWORD)Utils::FindSignature("engine.dll", "84 C0 74 04 B0 01 EB 02 32 C0 8B FE 46 3B F3 7E C9 84 C0 0F 84");
		g_Input							= *(CInput**)(Utils::FindSignature("client_panorama.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
		g_D3DDevice9					= **(IDirect3DDevice9***)(Utils::FindSignature("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
		g_pGameRules					= *reinterpret_cast<CGameRules***>(Utils::FindSignature("client_panorama.dll", "8B 0D ?? ?? ?? ?? 85 C0 74 0A 8B 01 FF 50 78 83 C0 54") + 2);
		g_pRender						= new Engine::CRender(g_D3DDevice9);
		acceptMatch						= FIND_PATTERN(decltype(acceptMatch), L"client_panorama", "\x55\x8B\xEC\x83\xE4\xF8\x8B\x4D\x08\xBA????\xE8????\x85\xC0\x75\x12");
 


		std::ofstream("csgo\\materials\\FlatChamies.vmt") << R"#("UnlitGeneric"
		{
		  "$basetexture" "vgui/white"
		  "$no_fullbright" "0"
		  "$ignorez"      "1"
		  "$nofog"        "1"
		  "$model"        "0"
		  "$nocull"       "0"
		  "$selfillum"    "1"
		  "$halflambert"  "1"
		  "$znearer"      "0"
		  "$flat"         "1"
		}
		)#";

		std::ofstream("csgo\\materials\\chams.vmt") << R"#("VertexLitGeneric"
		{
			 "$basetexture" "vgui/white"
			 "$ignorez"      "0"
			"$envmap" ""
			"$nofog"        "1"
			"$model"        "1"
			"$nocull"       "0"
			"$selfillum"    "1"
			"$halflambert"  "1"
			"$znearer"      "0"
			"$flat"         "0"
			"$wireframe"    "0"
			}
		)#";

		std::ofstream("csgo\\materials\\FlatChams.vmt") << R"#("UnlitGeneric"
		{
			"$basetexture" "vgui/white"
			"$no_fullbright" "0"
			"$ignorez"      "1"
			"$envmap"       "env_cubemap"
			"$nofog"        "1"
			"$model"        "1"
			"$nocull"       "0"
			"$selfillum"    "1"
			"$halflambert"  "1"
			"$znearer"      "0"
			"$flat"         "1"
		}
		)#";

		std::ofstream("csgo\\materials\\Metallic.vmt") << R"#("VertexLitGeneric"
		{
		  "$basetexture" "vgui/white"
		  "$ignorez" "0"
		  "$envmap" "env_cubemap"
		  "$normalmapalphaenvmapmask" "1"
		  "$envmapcontrast"  "1"
		  "$nofog" "1"
		  "$model" "1"
		  "$nocull" "0"
		  "$selfillum" "1"
		  "$halflambert" "1"
		  "$znearer" "0"
		  "$flat" "1" 
		}
		)#";

		

    }
}
