#include "skinchanger.h"
#include <Windows.h>
#include "SDK/CEntity.h"
#include "Utils/GlobalVars.h"
#include "SDK/Definitions.h"
#include "SDK/ClientClass.h"
#include "SDK/IVModelInfo.h"
#include "newMenu.h"
#include "Hooks.h"
#include <Options.hpp>
#include "SDK/IClientMode.h"

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
std::unordered_map<char*, char*> killIcons = {};
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
HANDLE worldmodel_handle;
C_BaseCombatWeapon* worldmodel;
C_BaseCombatWeapon* pWeapon;
#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))

IClientNetworkable* CL_CreateDLLEntity(int iEnt, int iClass, int iSerialNum)
{
	ClientClass* pClient = g_pClientDll->GetAllClasses();
	if (!pClient)
		return false;

	while (pClient)
	{
		if (pClient->ClassID == iClass)
			return pClient->pCreateFn(iEnt, iSerialNum);
		pClient = pClient->pNext;
	}
	return false;
}

int GetGloveSkinByMenu(int gloveIndex, int skinIndex)
{
	if (gloveIndex == 0) // bloudhound
	{
		switch (skinIndex)
		{
		case 0:
			return 10006;
		case 1:
			return 10007;
		case 2:
			return 10008;
		case 3:
			return 10039;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 1) // Sport
	{
		switch (skinIndex)
		{
		case 0:
			return 10038;
		case 1:
			return 10037;
		case 2:
			return 10018;
		case 3:
			return 10019;
		case 4:
			return 10048;
		case 5:
			return 10047;
		case 6:
			return 10045;
		case 7:
			return 10046;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 2) // Driver
	{
		switch (skinIndex)
		{
		case 0:
			return 10013;
		case 1:
			return 10015;
		case 2:
			return 10016;
		case 3:
			return 10040;
		case 4:
			return 10043;
		case 5:
			return 10044;
		case 6:
			return 10041;
		case 7:
			return 10042;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 3) // Wraps
	{
		switch (skinIndex)
		{
		case 0:
			return 10009;
		case 1:
			return 10010;
		case 2:
			return 10021;
		case 3:
			return 10036;
		case 4:
			return 10053;
		case 5:
			return 10054;
		case 6:
			return 10055;
		case 7:
			return 10056;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 4) // Moto
	{
		switch (skinIndex)
		{
		case 0:
			return 10024;
		case 1:
			return 10026;
		case 2:
			return 10027;
		case 3:
			return 10028;
		case 4:
			return 10050;
		case 5:
			return 10051;
		case 6:
			return 10052;
		case 7:
			return 10049;

		default:
			return 0;
		}
	}
	else if (gloveIndex == 5) // Specialist
	{
		switch (skinIndex)
		{
		case 0:
			return 10030;
		case 1:
			return 10033;
		case 2:
			return 10034;
		case 3:
			return 10035;
		case 4:
			return 10061;
		case 5:
			return 10062;
		case 6:
			return 10063;
		case 7:
			return 10064;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 6)
	{
		switch (skinIndex)
		{
		case 0:
			return 10057;
		case 1:
			return 10058;
		case 2:
			return 10059;
		case 3:
			return 10060;
		}
	}
	else
		return 0;
	return 0;
}
struct hud_weapons_t {
	std::int32_t* get_weapon_count() {
		return reinterpret_cast<std::int32_t*>(std::uintptr_t(this) + 0x80);
	}
};
template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(Utils::FindSignature("client_panorama.dll", "B9 ? ? ? ? E8 ? ? ? ? 8B 5D 08") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(Utils::FindSignature("client_panorama.dll", "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}

void Fullupdate()
{
	static auto clear_hud_weapon_icon_fn =
		reinterpret_cast<std::int32_t(__thiscall*)(void*, std::int32_t)>(
			Utils::FindSignature("client_panorama.dll", "55 8B EC 51 53 56 8B 75 08 8B D9 57 6B FE 2C 89 5D FC"));

	auto element = FindHudElement<std::uintptr_t*>("CCSGO_HudWeaponSelection");

	auto hud_weapons = reinterpret_cast<hud_weapons_t*>(std::uintptr_t(element) - 0xA0);
	if (hud_weapons == nullptr)
		return;

	if (!*hud_weapons->get_weapon_count())
		return;

	for (std::int32_t i = 0; i < *hud_weapons->get_weapon_count(); i++)
		i = clear_hud_weapon_icon_fn(hud_weapons, i);
	typedef void(*ForceUpdate) (void);
	ForceUpdate FullUpdate = (ForceUpdate)Utils::FindSignature("engine.dll", "FullUpdate", "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85");
	FullUpdate();
}

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
void Glovechanger(ClientFrameStage_t stage)
{

	if (!g_Options.skins_enable)
		return;

	PlayerInfo_t localPlayerInfo;

	C_BaseEntity* pLocal = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!pLocal)
		return;
	if (pLocal)
	{
		if (g_pEngine->GetPlayerInfo(g_pEngine->GetLocalPlayer(), &localPlayerInfo))
		{

			if (!pLocal->m_hMyWearables())
				return;

			DWORD* hMyWearables = (DWORD*)((size_t)+pLocal->m_hMyWearables());

			if (hMyWearables)
			{

				if (!g_pEntityList->GetClientEntityFromHandle(hMyWearables[0] & 0xFFF))
				{
					static ClientClass* pClass;

					if (!pClass)
						pClass = g_pClientDll->GetAllClasses();
					while (pClass)
					{
						if (pClass->ClassID == ClassId::CEconWearable)
							break;
						pClass = pClass->pNext;
					}

					int iEntry = g_pEntityList->GetHighestEntityIndex() + 1;
					int iSerial = RandomInt(0x0, 0xFFF);

					pClass->pCreateFn(iEntry, iSerial);
					hMyWearables[0] = iEntry | (iSerial << 16);

					//C_BaseEntity* pEnt = (C_BaseEntity*)g_pEntityList->GetClientEntityFromHandle(hMyWearables[0]);
					auto pEnt = reinterpret_cast<C_BaseAttributableItem*>(g_pEntityList->GetClientEntityFromHandle(hMyWearables[0]));



					if (pEnt)
					{
						int modelindex = 0;

						if (g_Options.skins_glove_skin == 1)
							modelindex = g_pModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"));
						else if (g_Options.skins_glove_skin == 2)
							modelindex = g_pModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"));
						else if (g_Options.skins_glove_skin == 3)
							modelindex = g_pModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"));
						else if (g_Options.skins_glove_skin == 4)
							modelindex = g_pModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"));
						else if (g_Options.skins_glove_skin == 5)
							modelindex = g_pModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"));
						else if (g_Options.skins_glove_skin == 6)
							modelindex = g_pModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"));
						else if (g_Options.skins_glove_skin == 7)
							modelindex = g_pModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl"));

						int ItemDefinitionIndex;
						if (g_Options.skins_glove_skin == 1)
						{
							ItemDefinitionIndex = 5027;
						}
						else if (g_Options.skins_glove_skin == 2)
						{
							ItemDefinitionIndex = 5030;
						}
						else if (g_Options.skins_glove_skin == 3)
						{
							ItemDefinitionIndex = 5031;
						}
						else if (g_Options.skins_glove_skin == 4)
						{
							ItemDefinitionIndex = 5032;
						}
						else if (g_Options.skins_glove_skin == 5)
						{
							ItemDefinitionIndex = 5033;
						}
						else if (g_Options.skins_glove_skin == 6)
						{
							ItemDefinitionIndex = 5034;
						}
						else if (g_Options.skins_glove_skin == 7)
						{
							ItemDefinitionIndex = 5035;
						}
						else
							ItemDefinitionIndex = 0;
						int paintkit;
						if (g_Options.skins_glove_skin == 1)
						{
							paintkit = GetGloveSkinByMenu(0, g_Options.skins_glove_skin);
						}
						else if (g_Options.skins_glove_skin == 2)
						{
							paintkit = GetGloveSkinByMenu(1, g_Options.skins_glove_skin);
						}
						else if (g_Options.skins_glove_skin == 3)
						{
							paintkit = GetGloveSkinByMenu(2, g_Options.skins_glove_skin);
						}
						else if (g_Options.skins_glove_skin == 4)
						{
							paintkit = GetGloveSkinByMenu(3, g_Options.skins_glove_skin);
						}
						else if (g_Options.skins_glove_skin == 5)
						{
							paintkit = GetGloveSkinByMenu(4, g_Options.skins_glove_skin);
						}
						else if (g_Options.skins_glove_skin == 6)
						{
							paintkit = GetGloveSkinByMenu(5, g_Options.skins_glove_skin);
						}
						else if (g_Options.skins_glove_skin == 7)
						{
							paintkit = GetGloveSkinByMenu(6, g_Options.skins_glove_skin);
						}
						else
							paintkit = 0;

						*(int*)((DWORD)pEnt->m_iItemDefinitionIndex()) = ItemDefinitionIndex;
						*(int*)((DWORD)pEnt->m_iItemIDHigh()) = -1;
						*(int*)((DWORD)pEnt->m_iEntityQuality()) = 4;
						*(int*)((DWORD)pEnt->m_iAccountID()) = localPlayerInfo.xuidLow;//m_iAccountID
						//*(float*)((DWORD)pEnt->m_flFallbackWear()) = 0.001;
						*(int*)((DWORD)pEnt + OFFSETS::m_nFallbackSeed) = 0;
						//*(int*)((DWORD)pEnt->m_nFallbackStatTrak()) = -1;
						*(int*)((DWORD)pEnt->FallbackPaintKitGLOVE()) = paintkit;

						/*
						*(int*)((DWORD)pEnt->m_iItemDefinitionIndex()) = ItemDefinitionIndex;
						*(int*)((DWORD)pEnt->m_iItemIDHigh()) = -1; //0x2FB0
						*(int*)((DWORD)pEnt->m_iEntityQuality()) = 4;
						*(int*)((DWORD)pEnt->m_iAccountID()) = localPlayerInfo.xuidLow;
						*(float*)((DWORD)pEnt->m_flFallbackWear()) = 0.001;
						*(int*)((DWORD)pEnt + OFFSETS::m_nFallbackSeed) = 0;
						*(int*)((DWORD)pEnt->m_nFallbackStatTrakGLOVE()) = -1;
						*(int*)((DWORD)pEnt->m_nFallbackPaintKit()) = paintkit;
						*/
						pEnt->SetGloveModelIndex(modelindex);
						pEnt->PreDataUpdateNEW(DATA_UPDATE_CREATEDxd);
					}
				}
			}

		}
	}
			

		
	

}



void Skinchanger()
{
	if (!g_pEngine->IsConnected() && !g_pEngine->IsInGame())
		return;
	if (!g::pLocalEntity) return;
	auto weapons = g::pLocalEntity->m_hMyWeapons();
	PlayerInfo_t localPlayerInfo;
	if (!g_pEngine->GetPlayerInfo(g_pEngine->GetLocalPlayer(), &localPlayerInfo))
		return;
	if (!weapons)
		return;
	for (size_t i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++)
	{
		if (weapons[i] != nullptr)
		{
			C_BaseEntity *pEntity = (C_BaseEntity*)g_pEntityList->GetClientEntityFromHandle(weapons[i]/*crash here*/);
			if (pEntity)
			{
				pWeapon = (C_BaseCombatWeapon*)pEntity;
				

				if (g_Options.skins_enable)
				{
					int Model = g_Options.skins_knife_model;
					int weapon = *pWeapon->fixskins();
					//////////////////////
						


					//////////
					switch (weapon)
					{
					case 7: // AK47 
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_AK47].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_AK47].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if(g_Options.W[(int)ItemDefinitionIndex::WEAPON_AK47].statamount > 0)
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_AK47].statamount;
					}
					break;
					case 16: // M4A4
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_M4A4].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_M4A4].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_M4A4].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_M4A4].statamount;
					}
					break;
					case 60:
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_M4A1S].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_M4A1S].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_M4A1S].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_M4A1S].statamount;
					}
					break;
					case 9:
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_AWP].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_AWP].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_AWP].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_AWP].statamount;
					}
					break;
					case 61:
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_USPS].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_USPS].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_USPS].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_USPS].statamount;
					}
					break;
					case 4:
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_GLOCK].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_GLOCK].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_GLOCK].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_GLOCK].statamount;
					}
					break;
					case 1: // Deagle
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_DEAGLE].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_DEAGLE].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_DEAGLE].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_DEAGLE].statamount;
					}
					break;
					case 2: //duals
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_ELITE].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_ELITE].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_ELITE].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_ELITE].statamount;
					}
					break;
					case 3: // Five Seven
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_FIVESEVEN].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_FIVESEVEN].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_FIVESEVEN].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_FIVESEVEN].statamount;
					}
					break;
					case 8: // AUG
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_AUG].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_AUG].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_AUG].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_AUG].statamount;
					}
					break;
					case 10: // Famas
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_FAMAS].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_FAMAS].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_FAMAS].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_FAMAS].statamount;
					}
					break;
					case 11: // G3SG1
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_G3SG1].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_G3SG1].wear;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_G3SG1].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_G3SG1].statamount;
					}
					break;
					case 13: // Galil
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_GALIL].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_GALIL].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_GALIL].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_GALIL].statamount;
					}
					break;
					case 14: // M249
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_M249].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_M249].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_M249].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_M249].statamount;
					}
					break;
					case 17: // Mac 10
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_MAC10].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_MAC10].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_MAC10].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_MAC10].statamount;
					}
					break;
					case 19: // P90
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_P90].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_P90].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_P90].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_P90].statamount;
					}
					break;
					case 24: // UMP-45
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_UMP45].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_UMP45].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_UMP45].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_UMP45].statamount;
					}
					break;
					case 25: // XM1014
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_XM1014].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_XM1014].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_XM1014].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_XM1014].statamount;
					}
					break;
					case 26: // Bizon
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_BIZON].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_BIZON].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_BIZON].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_BIZON].statamount;
					}
					break;
					case 27: // Mag 7
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_MAG7].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_MAG7].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_MAG7].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_MAG7].statamount;
					}
					break;
					case 28: // Negev
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_NEGEV].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_NEGEV].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_NEGEV].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_NEGEV].statamount;
					}
					break;
					case 29: // Sawed Off
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_SAWEDOFF].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_SAWEDOFF].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_SAWEDOFF].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_SAWEDOFF].statamount;
					}
					break;
					case 30: // Tec 9
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_TEC9].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_TEC9].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_TEC9].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_TEC9].statamount;
					}
					break;
					case 32: // P2000
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_P2000].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_P2000].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_P2000].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_P2000].statamount;
					}
					break;
					case 33: // MP7
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_MP7].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_MP7].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_MP7].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_MP7].statamount;
					}
					break;
					case 34: // MP9
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_MP9].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_MP9].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_MP9].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_MP9].statamount;
					}
					break;
					case 35: // Nova
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_NOVA].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_NOVA].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_NOVA].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_NOVA].statamount;
					}
					break;
					case 36: // P250
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_P250].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_P250].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_P250].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_P250].statamount;
					}
					break;
					case 38: // Scar 20
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_SCAR20].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_SCAR20].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_SCAR20].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_SCAR20].statamount;
					}
					break;
					case 39: // SG553
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_SG553].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_SG553].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_SG553].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_SG553].statamount;
					}
					break;
					case 40: // SSG08
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_SSG08].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_SSG08].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_SSG08].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_SSG08].statamount;
					}
					break;
					case 64: // Revolver
					{
                        *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_REVOLVER].ChangerSkin;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_REVOLVER].wear;
						//
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                        if (g_Options.W[(int)ItemDefinitionIndex::WEAPON_REVOLVER].statamount > 0)
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_REVOLVER].statamount;
					}
					break;
					default:
						break;
					}

					if (pEntity->GetClientClass()->ClassID == (int)CKnife)
					{
						auto pCustomName1 = MakePtr(char*, pWeapon, 0x301C);
                        PlayerInfo_t localPlayerInfo;
						HANDLE worldmodel_handle2 = pWeapon->m_hWeaponWorldModel_h();

						if (worldmodel_handle2) worldmodel = (C_BaseCombatWeapon*)g_pEntityList->GetClientEntityFromHandle(pWeapon->m_hWeaponWorldModel_c());
						if (Model == 0) // Bayonet
						{
							int iBayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
							*pWeapon->m_nModelIndex() = iBayonet; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iBayonet;
							if (worldmodel) *worldmodel->m_nModelIndex() = iBayonet + 1;
							*pWeapon->fixskins() = 500;
							*pWeapon->m_iEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "bayonet";
							killIcons["knife_t"] = "bayonet";
                            *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
                            *pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_BAYONET].ChangerSkin; // Forest DDPAT
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_BAYONET].statamount;
                            *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_BAYONET].wear;
						}
						else if (Model == 1) // Bowie Knife
						{
							int iBowie = g_pModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
							*pWeapon->m_nModelIndex() = iBowie; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iBowie;
							if (worldmodel) *worldmodel->m_nModelIndex() = iBowie + 1;
							*pWeapon->fixskins() = 514;
							*pWeapon->m_iEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_survival_bowie";
							killIcons["knife_t"] = "knife_survival_bowie";
                            *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
							*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE].ChangerSkin; // Forest DDPAT
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE].statamount;
                            *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE].wear;
						}
						else if (Model == 2) // Butterfly Knife
						{
							int iButterfly = g_pModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
							*pWeapon->m_nModelIndex() = iButterfly; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iButterfly;
							if (worldmodel) *worldmodel->m_nModelIndex() = iButterfly + 1;
							*pWeapon->fixskins() = 515;
							*pWeapon->m_iEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_butterfly";
							killIcons["knife_t"] = "knife_butterfly";
                            *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
							*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY].ChangerSkin; // Forest DDPAT
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY].statamount;
                            *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY].wear;
						}
						else if (Model == 3) // Falchion Knife
						{
							int iFalchion = g_pModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
							*pWeapon->m_nModelIndex() = iFalchion; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iFalchion;
							if (worldmodel) *worldmodel->m_nModelIndex() = iFalchion + 1;
							*pWeapon->fixskins() = 512;
							*pWeapon->m_iEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_falchion";
							killIcons["knife_t"] = "knife_falchion";
                            *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
							*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_FALCHION].ChangerSkin; // Forest DDPAT
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_FALCHION].statamount;
                            *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_FALCHION].wear;
						}
						else if (Model == 4) // Flip Knife
						{
							int iFlip = g_pModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
							*pWeapon->m_nModelIndex() = iFlip; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iFlip;
							if (worldmodel) *worldmodel->m_nModelIndex() = iFlip + 1;
							*pWeapon->fixskins() = 505;
							*pWeapon->m_iEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_flip";
							killIcons["knife_t"] = "knife_flip";
                            *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
							*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_FLIP].ChangerSkin; // Forest DDPAT
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_FLIP].statamount;
                            *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_FLIP].wear;

						}
						else if (Model == 5) // Gut Knife
						{
							int iGut = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
							*pWeapon->m_nModelIndex() = iGut; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iGut;
							if (worldmodel) *worldmodel->m_nModelIndex() = iGut + 1;
							*pWeapon->fixskins() = 506;
							*pWeapon->m_iEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_gut";
							killIcons["knife_t"] = "knife_gut";
                            *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
							*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_GUT].ChangerSkin; // Forest DDPAT
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_GUT].statamount;
                            *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_GUT].wear;
						}
						else if (Model == 6) // Huntsman Knife
						{
							int iHuntsman = g_pModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
							*pWeapon->m_nModelIndex() = iHuntsman; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iHuntsman;
							if (worldmodel) *worldmodel->m_nModelIndex() = iHuntsman + 1;
							*pWeapon->fixskins() = 509;
							*pWeapon->m_iEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_tactical";
							killIcons["knife_t"] = "knife_tactical";
                            *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
							*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL].ChangerSkin; // Forest DDPAT
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL].statamount;
                            *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL].wear;
						}
						else if (Model == 7) // Karambit
						{
							int iKarambit = g_pModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
							*pWeapon->m_nModelIndex() = iKarambit; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iKarambit;
							if (worldmodel) *worldmodel->m_nModelIndex() = iKarambit + 1;
							*pWeapon->fixskins() = 507;
							*pWeapon->m_iEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_karambit";
							killIcons["knife_t"] = "knife_karambit";
                            *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
							*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT].ChangerSkin; // Forest DDPAT
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT].statamount;
                            *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT].wear;

						}
						else if (Model == 8) // M9 Bayonet
						{
							auto iM9Bayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
							*pWeapon->m_nModelIndex() = iM9Bayonet; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iM9Bayonet;
							if (worldmodel) *worldmodel->m_nModelIndex() = iM9Bayonet + 1;
							*pWeapon->fixskins() = 508;
							*pWeapon->m_iEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_m9_bayonet";
							killIcons["knife_t"] = "knife_m9_bayonet";
                            *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
							*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET].ChangerSkin; // Forest DDPAT
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET].statamount;
                            *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET].wear;

						}
						else if (Model == 9)
						{
							int iDagger = g_pModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
							*pWeapon->m_nModelIndex() = iDagger; // m_nModelIndex
							*pWeapon->ViewModelIndex() = iDagger;
							if (worldmodel) *worldmodel->m_nModelIndex() = iDagger + 1;
							*pWeapon->fixskins() = 516;
							*pWeapon->m_iEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "knife_push";
							killIcons["knife_t"] = "knife_push";
                            *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
							*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_PUSH].ChangerSkin; // Forest DDPAT
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_PUSH].statamount;
                            *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_PUSH].wear;
						}
						else if (Model == 10)
						{
							int Navaja = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
							*pWeapon->m_nModelIndex() = Navaja; // m_nModelIndex
							*pWeapon->ViewModelIndex() = Navaja;
							if (worldmodel) *worldmodel->m_nModelIndex() = Navaja + 1;
							*pWeapon->fixskins() = 520;
							*pWeapon->m_iEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "v_knife_gypsy_jackknife";
							killIcons["knife_t"] = "v_knife_gypsy_jackknife";
                            *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
							*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_GYPSY_JACKKNIFE].ChangerSkin; // Forest DDPAT
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_GYPSY_JACKKNIFE].statamount;
                            *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_GYPSY_JACKKNIFE].wear;
						}
						else if (Model == 11) // Shadow Daggers
						{
							int Stiletto = g_pModelInfo->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
							*pWeapon->m_nModelIndex() = Stiletto; // m_nModelIndex
							*pWeapon->ViewModelIndex() = Stiletto;
							if (worldmodel) *worldmodel->m_nModelIndex() = Stiletto + 1;
							*pWeapon->fixskins() = 522;
							*pWeapon->m_iEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "v_knife_stiletto";
							killIcons["knife_t"] = "v_knife_stiletto";
                            *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
							*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_STILETTO].ChangerSkin; // Forest DDPAT
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_STILETTO].statamount;
                            *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_STILETTO].wear;
						}
						else if (Model == 12)
						{
							int Ursus = g_pModelInfo->GetModelIndex("models/weapons/v_knife_ursus.mdl");
							*pWeapon->m_nModelIndex() = Ursus; // m_nModelIndex
							*pWeapon->ViewModelIndex() = Ursus;
							if (worldmodel) *worldmodel->m_nModelIndex() = Ursus + 1;
							*pWeapon->fixskins() = 519;
							*pWeapon->m_iEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "v_knife_ursus";
							killIcons["knife_t"] = "v_knife_ursus";
                            *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
							*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_URSUS].ChangerSkin; // Forest DDPAT
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_URSUS].statamount;
                            *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_URSUS].wear;
						}
						else if (Model == 13)
						{
							int Talon = g_pModelInfo->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");
							*pWeapon->m_nModelIndex() = Talon; // m_nModelIndex
							*pWeapon->ViewModelIndex() = Talon;
							if (worldmodel) *worldmodel->m_nModelIndex() = Talon + 1;
							*pWeapon->fixskins() = 523;
							*pWeapon->m_iEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "v_knife_widowmaker";
							killIcons["knife_t"] = "v_knife_widowmaker";
                            *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
							*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_WIDOWMAKER].ChangerSkin; // Forest DDPAT
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_WIDOWMAKER].statamount;
                            *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_WIDOWMAKER].wear;
						}
						else if (Model == 14)
						{
							int Talon = g_pModelInfo->GetModelIndex("models/weapons/v_knife_skeleton.mdl");
							*pWeapon->m_nModelIndex() = Talon; // m_nModelIndex
							*pWeapon->ViewModelIndex() = Talon;
							if (worldmodel) 
								*worldmodel->m_nModelIndex() = Talon + 1;
							*pWeapon->fixskins() = 525;
							*pWeapon->m_iEntityQuality() = 3;
							killIcons.clear();
							killIcons["knife_default_ct"] = "v_knife_skeleton";
							killIcons["knife_t"] = "v_knife_skeleton";
                            *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
							*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_SKELETON].ChangerSkin; // Forest DDPAT
                            *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_SKELETON].statamount;
                            *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_SKELETON].wear;
						}
						else if (Model == 15)
						{
						int Talon = g_pModelInfo->GetModelIndex("models/weapons/v_knife_outdoor.mdl");
						*pWeapon->m_nModelIndex() = Talon; // m_nModelIndex
						*pWeapon->ViewModelIndex() = Talon;
						if (worldmodel)
							*worldmodel->m_nModelIndex() = Talon + 1;
						*pWeapon->fixskins() = 521;
						*pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "v_knife_outdoor";
						killIcons["knife_t"] = "v_knife_outdoor";
                        *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
						*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_OUTDOOR].ChangerSkin; // Forest DDPAT
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_OUTDOOR].statamount;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_OUTDOOR].wear;
						}
						else if (Model == 16)
						{
						int Talon = g_pModelInfo->GetModelIndex("models/weapons/v_knife_canis.mdl");
						*pWeapon->m_nModelIndex() = Talon; // m_nModelIndex
						*pWeapon->ViewModelIndex() = Talon;
						if (worldmodel)
							*worldmodel->m_nModelIndex() = Talon + 1;
						*pWeapon->fixskins() = 518;
						*pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "v_knife_canis";
						killIcons["knife_t"] = "v_knife_canis";
                        *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
						*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_CANIS].ChangerSkin; // Forest DDPAT
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_CANIS].statamount;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_CANIS].wear;
						}
						else if (Model == 17)
						{
						int Talon = g_pModelInfo->GetModelIndex("models/weapons/v_knife_cord.mdl");
						*pWeapon->m_nModelIndex() = Talon; // m_nModelIndex
						*pWeapon->ViewModelIndex() = Talon;
						if (worldmodel)
							*worldmodel->m_nModelIndex() = Talon + 1;
						*pWeapon->fixskins() = 517;

						*pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "v_knife_cord";
						killIcons["knife_t"] = "v_knife_cord";
                        *pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
						*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_CORD].ChangerSkin; // Forest DDPAT
                        *pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_CORD].statamount;
                        *pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_CORD].wear;
						}
						else if (Model == 18)
						{
						int Talon = g_pModelInfo->GetModelIndex("models/weapons/v_knife_css.mdl");
						*pWeapon->m_nModelIndex() = Talon; // m_nModelIndex
						*pWeapon->ViewModelIndex() = Talon;
						if (worldmodel)
							*worldmodel->m_nModelIndex() = Talon + 1;
						*pWeapon->fixskins() = 503;

						*pWeapon->m_iEntityQuality() = 3;
						killIcons.clear();
						killIcons["knife_default_ct"] = "v_knife_css";
						killIcons["knife_t"] = "v_knife_css";
						*pWeapon->m_iAccountID() = localPlayerInfo.xuidLow;
						*pWeapon->FallbackPaintKit() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_CSS].ChangerSkin; // Forest DDPAT
						*pWeapon->m_nFallbackStatTrak() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_CSS].statamount;
						*pWeapon->m_flFallbackWear() = g_Options.W[(int)ItemDefinitionIndex::WEAPON_KNIFE_CSS].wear;
						}
					}


                    
					*pWeapon->OwnerXuidLow() = 0;
					*pWeapon->OwnerXuidHigh() = 0;
						
					*pWeapon->m_iItemIDHigh() = 1;
						

				}
			}
		}

	}
}


//thanks to choZen https://github.com/ChoZenTime/aristois-pasted-by-choZen/blob/master/core/features/skinchanger/glovechanger.cpp

c_glovechanger glove_changer;

static auto get_wearable_create_fn() -> create_client_class_fn
{
	auto client_class = g_pClientDll->GetAllClasses();

	for (client_class = g_pClientDll->GetAllClasses();
		client_class; client_class = client_class->pNext) {

		if (client_class->ClassID == ClassId::CEconWearable) {
			return client_class->pCreateFn;
		}
	}
}

static auto make_glove(int entry, int serial) -> C_BaseAttributableItem* {
	static auto create_wearable_fn = get_wearable_create_fn();
	create_wearable_fn(entry, serial);

	const auto glove = static_cast<C_BaseAttributableItem*>(g_pEntityList->GetClientEntity(entry));
	assert(glove); {
		static auto set_abs_origin_addr = Utils::FindSignature(("client_panorama.dll"), "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8");
		const auto set_abs_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const std::array<float, 3>&)>(set_abs_origin_addr);
		static constexpr std::array<float, 3> new_pos = { 10000.f, 10000.f, 10000.f };
		set_abs_origin_fn(glove, new_pos);
	}
	return glove;
}

bool c_glovechanger::apply_glove_model(C_BaseAttributableItem* glove, const char* model) noexcept {
	*reinterpret_cast<int*>(uintptr_t(glove) + 0x64) = -1;
	return true;
}

bool c_glovechanger::apply_glove_skin(C_BaseAttributableItem* glove, int item_definition_index, int paint_kit, int model_index, int entity_quality, float fallback_wear) noexcept {
	*glove->m_iItemDefinitionIndex() = item_definition_index;
	*glove->FallbackPaintKitGLOVE() = paint_kit;
	glove->set_model_index(model_index);
	*glove->m_iEntityQuality() = entity_quality;
	glove->m_flFallbackWear() = fallback_wear;

	return true;
}

void c_glovechanger::run() noexcept
{
	auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());

	if (!local_player)
		return;


	if (g_Options.skins_enable)// this shit is soo long thank me later 
	{
		auto model_blood = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl";
		auto model_sport = "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl";
		auto model_slick = "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl";
		auto model_leath = "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl";
		auto model_moto = "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl";
		auto model_speci = "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl";
		auto model_hydra = "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl";

		auto index_blood = g_pModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"));
		auto index_sport = g_pModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"));
		auto index_slick = g_pModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"));
		auto index_leath = g_pModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"));
		auto index_moto  = g_pModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"));
		auto index_speci = g_pModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"));
		auto index_hydra = g_pModelInfo->GetModelIndex(("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl"));

		//credit to namazso for nskinz

		UINT* const wearables = local_player->get_wearables();
		if (!wearables)
			return;

		static UINT glove_handle = UINT(0);

		auto glove = reinterpret_cast<C_BaseAttributableItem*>(g_pEntityList->GetClientEntityFromHandle(wearables[0]));

		if (!glove) {
			const auto our_glove = reinterpret_cast<C_BaseAttributableItem*>(g_pEntityList->GetClientEntityFromHandle(glove_handle));

			if (our_glove) {
				wearables[0] = glove_handle;
				glove = our_glove;
			}
		}
		if (!local_player || !local_player->IsAlive() || !g_pEngine->IsConnected() || !g_pEngine->IsInGame()) {
			if (glove) {
				glove->SetDestroyedOnRecreateEntities();
				glove->Release();
			}
			return;
		}
		if (!glove) {
			const auto entry = g_pEntityList->GetHighestEntityIndex() + 1;
			const auto serial = rand() % 0x1000;
			glove = make_glove(entry, serial);// crash here when creating glove
			wearables[0] = entry | serial << 16;
			glove_handle = wearables[0];
		}
		if (glove) {
			float gloves_wear = 0.f;
			gloves_wear = 0.0000001f;
			
			//apply glove model
		
			//

			if (g_Options.skins_glove_model == 1)
			{
				apply_glove_model(glove, model_blood);

			}
			else if (g_Options.skins_glove_model == 2)
			{
				apply_glove_model(glove, model_sport);

			}
			else if (g_Options.skins_glove_model == 3)
			{
				apply_glove_model(glove, model_slick);

			}
			else if (g_Options.skins_glove_model == 4)
			{
				apply_glove_model(glove, model_leath);

			}
			else if (g_Options.skins_glove_model == 5)
			{
				apply_glove_model(glove, model_moto);

			}
			else if (g_Options.skins_glove_model == 6)
			{
				apply_glove_model(glove, model_speci);

			}
			else if (g_Options.skins_glove_model == 7)
			{
				apply_glove_model(glove, model_hydra);

			}

			//
		
			// THANK ME LATER
			switch (g_Options.skins_glove_model)
			{
			case 0:
				break;
			case 1:
				apply_glove_skin(glove, 5027, GetGloveSkinByMenu(0, g_Options.skins_glove_skin), index_blood, 3, gloves_wear);
				break;
			case 2:
				apply_glove_skin(glove, 5030, GetGloveSkinByMenu(1, g_Options.skins_glove_skin), index_sport, 3, gloves_wear);
				break;
			case 3:
				apply_glove_skin(glove, 5031, GetGloveSkinByMenu(2, g_Options.skins_glove_skin), index_slick, 3, gloves_wear);
				break;
			case 4:
				apply_glove_skin(glove, 5032, GetGloveSkinByMenu(3, g_Options.skins_glove_skin), index_leath, 3, gloves_wear);
				break;
			case 5:
				apply_glove_skin(glove, 5033, GetGloveSkinByMenu(4, g_Options.skins_glove_skin), index_moto, 3, gloves_wear);
				break;
			case 6:
				apply_glove_skin(glove, 5034, GetGloveSkinByMenu(5, g_Options.skins_glove_skin), index_speci, 3, gloves_wear);
				break;
			case 7:
				apply_glove_skin(glove, 5035, GetGloveSkinByMenu(6, g_Options.skins_glove_skin), index_hydra, 3, gloves_wear);
				break;
			}

			*glove->m_iItemIDHigh() = -1;
			*glove->FallbackSeed() = 0;
			glove->m_nFallbackStatTrak() = -1;

			glove->net_pre_data_update(0);// EZ 
		}
	}
}