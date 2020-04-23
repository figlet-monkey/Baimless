#pragma once
#include <Windows.h>
#include "..\SDK\IClientEntityList.h"
#include "../SDK/IEngineSound.h"
#include "../SDK/ILocalize.h"
#include "../Hooks.h"

#include "UTILS/Utils.h"
#define FIND_PATTERN(type, ...) \
reinterpret_cast<type>(Utils::findPattern(__VA_ARGS__))
struct IDirect3DDevice9;
namespace interfaces
{
    // Used to initialize all the interfaces at one time
    void Init();
};

extern  std::add_pointer_t<bool __stdcall(const char*)> acceptMatch;

extern IDirect3DDevice9* g_D3DDevice9;
extern IClientEntityList* g_pEntityList;
extern ILocalize* g_Localize;
extern IEngineSound* g_EngineSound ;
extern uintptr_t _new_test;
extern uintptr_t _new_test2;
