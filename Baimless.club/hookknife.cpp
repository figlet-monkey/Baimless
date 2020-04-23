#include "hookknife.h"
#include <Windows.h>
#include "SDK/CEntity.h"
#include "Utils/GlobalVars.h"
#include "newMenu.h"
#include "SDK/Definitions.h"
#include "SDK/ClientClass.h"
#include "SDK/IVModelInfo.h"
#include "SDK/Recv.h"
#include "SDK/IBaseClientDll.h"
#include "SDK/IClientMode.h"
#include "Proxies.h"
#include "../Baimless.club/Options.hpp"


typedef void(*RecvVarProxyFn)(const CRecvProxyData* pData, void* pStruct, void* pOut);

#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1


using namespace std;
// Helper function to generate a random sequence number.
inline int RandomSequence(int low, int high) {
	return (rand() % (high - low + 1) + low);
}

#define	LIFE_ALIVE 0

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);



RecvVarProxyFn fnSequenceProxyFn = nullptr;

RecvVarProxyFn oRecvnModelIndex;
// xd
int iBayonet;
int iButterfly;
int iFlip;
int iGut;
int iKarambit;
int iM9Bayonet;
int iHuntsman;
int iFalchion;
int iDagger;
int iBowie;
int iGunGame;


void Hooked_RecvProxy_Viewmodel(CRecvProxyData *pData, void *pStruct, void *pOut)
{
	// Get the knife view model id's
	int default_t = g_pModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	int default_ct = g_pModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	iBayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	iButterfly = g_pModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	iFlip = g_pModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	iGut = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	iKarambit = g_pModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	iM9Bayonet = g_pModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	iHuntsman = g_pModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	iFalchion = g_pModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	iDagger = g_pModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
	iBowie = g_pModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
	iGunGame = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

	int Navaja = g_pModelInfo->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
	int Stiletto = g_pModelInfo->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
	int Ursus = g_pModelInfo->GetModelIndex("models/weapons/v_knife_ursus.mdl");
	int Talon = g_pModelInfo->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");

	int skeleton = g_pModelInfo->GetModelIndex("models/weapons/v_knife_skeleton.mdl");
	int classic = g_pModelInfo->GetModelIndex("models/weapons/v_knife_css.mdl");
	int outdoor = g_pModelInfo->GetModelIndex("models/weapons/v_knife_outdoor.mdl");


	int canis = g_pModelInfo->GetModelIndex("models/weapons/v_knife_canis.mdl");
	int cord = g_pModelInfo->GetModelIndex("models/weapons/v_knife_cord.mdl");

	// Get local player (just to stop replacing spectators knifes)

	if ((g_Options.skins_enable) && g::pLocalEntity)
	{
		// If we are alive and holding a default knife(if we already have a knife don't worry about changing)
		if (g::pLocalEntity->IsAlive() && (
			pData->m_Value.m_Int == default_t ||
			pData->m_Value.m_Int == default_ct ||
			pData->m_Value.m_Int == iBayonet ||
			pData->m_Value.m_Int == iFlip ||
			pData->m_Value.m_Int == iGunGame ||
			pData->m_Value.m_Int == iGut ||
			pData->m_Value.m_Int == iKarambit ||
			pData->m_Value.m_Int == iM9Bayonet ||
			pData->m_Value.m_Int == iHuntsman ||
			pData->m_Value.m_Int == iBowie ||
			pData->m_Value.m_Int == iButterfly ||
			pData->m_Value.m_Int == iFalchion ||
			pData->m_Value.m_Int == iDagger ||
			pData->m_Value.m_Int == Navaja ||
			pData->m_Value.m_Int == Stiletto ||
			pData->m_Value.m_Int == Ursus ||
			pData->m_Value.m_Int == Talon ||
			pData->m_Value.m_Int == skeleton ||
			pData->m_Value.m_Int == outdoor ||
			pData->m_Value.m_Int == canis ||
			pData->m_Value.m_Int == cord))
		{
			int Model = g_Options.skins_knife_model;

			// Set whatever knife we want
			if (Model == 0) // Bayonet
			{
				pData->m_Value.m_Int = iBayonet;


			}
			else if (Model == 1) // Bowie Knife
			{
				pData->m_Value.m_Int = iBowie;

			}
			else if (Model == 2) // Butterfly Knife
			{
				pData->m_Value.m_Int = iButterfly;

			}
			else if (Model == 3) // Falchion Knife
			{
				pData->m_Value.m_Int = iFalchion;

			}
			else if (Model == 4) // Flip Knife
			{
				pData->m_Value.m_Int = iFlip;

			}
			else if (Model == 5) // Gut Knife
			{
				pData->m_Value.m_Int = iGut;

			}
			else if (Model == 6) // Huntsman Knife
			{
				pData->m_Value.m_Int = iHuntsman;

			}
			else if (Model == 7) // Karambit
			{
				pData->m_Value.m_Int = iKarambit;

			}
			else if (Model == 8) // M9 Bayonet
			{
				pData->m_Value.m_Int = iM9Bayonet;

			}
			else if (Model == 9)
			{
				pData->m_Value.m_Int = iDagger;

			}
			else if (Model == 10)
			{
				pData->m_Value.m_Int = Navaja;

			}
			else if (Model == 11) // Shadow Daggers
			{
				pData->m_Value.m_Int = Stiletto;

			}
			else if (Model == 12)
			{
				pData->m_Value.m_Int = Ursus;

			}
			else if (Model == 13)
			{
				pData->m_Value.m_Int = Talon;

			}
			else if (Model == 14)
			{
				pData->m_Value.m_Int = skeleton;

			}
			else if (Model == 15)
			{

				pData->m_Value.m_Int = outdoor;


			}
			else if (Model == 16)
			{
				pData->m_Value.m_Int = canis;

			}
			else if (Model == 17)
			{
				pData->m_Value.m_Int = cord;

			}
			else if (Model == 18)
			{
				pData->m_Value.m_Int = classic;

			}

			


			
		}
	}








	// Carry on the to original proxy
	oRecvnModelIndex(pData, pStruct, pOut);
}



void set_view_model_sequence(const CRecvProxyData*pDataConst, void *p_struct, void *p_out)noexcept {
	CRecvProxyData* p_data = const_cast<CRecvProxyData*>(pDataConst);
	C_BaseViewModel* player_view_model = static_cast<C_BaseViewModel*>(p_struct);

	if (player_view_model) {
		auto local_player = g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer());
		C_BaseEntity* p_owner = static_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(player_view_model->m_howner() & 0xFFF));

		if (p_owner == local_player)
		{
			std::string sz_model = g_pModelInfo->GetModelNamevoid(g_pModelInfo->GetModel(player_view_model->GetModelIndex()));
			int m_nSequence = p_data->m_Value.m_Int;
			if (sz_model == "models/weapons/v_knife_butterfly.mdl")
			{
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
				}
			}
			else if (sz_model == "models/weapons/v_knife_falchion_advanced.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
					break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence--;
				}
			}
			else if (sz_model == "models/weapons/v_knife_push.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
				case SEQUENCE_DEFAULT_LIGHT_MISS1:
				case SEQUENCE_DEFAULT_LIGHT_MISS2:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
					break;
				case SEQUENCE_DEFAULT_HEAVY_MISS1:
					m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
					break;
				case SEQUENCE_DEFAULT_HEAVY_HIT1:
				case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence += 3; break;
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				default:
					m_nSequence += 2;
				}
			}
			else if (sz_model == "models/weapons/v_knife_survival_bowie.mdl") {
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
				case SEQUENCE_DEFAULT_IDLE1:
					break;
				case SEQUENCE_DEFAULT_IDLE2:
					m_nSequence = SEQUENCE_BOWIE_IDLE1;
					break;
				default:
					m_nSequence--;
				}
			}
			else if (sz_model == "models/weapons/v_knife_ursus.mdl") {
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
					break;
				default:
					m_nSequence++;
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_stiletto.mdl")
			{
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(12, 13);
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_widowmaker.mdl") 
			{
				switch (m_nSequence) {
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(14, 15);
					break;
				}
			}
			else if (sz_model == "models/weapons/v_knife_skeleton.mdl")
			{
				
				switch (m_nSequence)
				{
				case SEQUENCE_DEFAULT_DRAW:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
					break;
				case SEQUENCE_DEFAULT_LOOKAT01:
					m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
					break;
				default:
					m_nSequence++;
					break;
				}
				
			}

			p_data->m_Value.m_Int = m_nSequence;
		}
	}
	fnSequenceProxyFn(p_data, p_struct, p_out);
}


void knife_animation_hook()noexcept
{
	for (auto client_class = g_pClientDll->GetAllClasses(); client_class; client_class = client_class->pNext) {

		if (!strcmp(client_class->pNetworkName, "CBaseViewModel")) {
			const auto table = reinterpret_cast<RecvTable*>(client_class->pRecvTable);
			for (int nIndex = 0; nIndex < table->m_nProps; nIndex++) {
				RecvProp* pProp = &table->m_pProps[nIndex];
				if (!pProp || strcmp(pProp->m_pVarName, "m_nSequence"))
					continue;
				fnSequenceProxyFn = static_cast<RecvVarProxyFn>(pProp->m_ProxyFn);
				pProp->m_ProxyFn = static_cast<RecvVarProxyFn>(set_view_model_sequence);
				break;
			}
			break;
		}
	}
}

void knife_animation() noexcept
{
	knife_animation_hook();

	auto client_class = g_pClientDll->GetAllClasses();
	while (client_class) {
		const char *pszName = client_class->pRecvTable->m_pNetTableName;
		if (!strcmp(pszName, "DT_SmokeGrenadeProjectile")) {
			for (int i = 0; i < client_class->pRecvTable->m_nProps; i++) {
				RecvProp *pProp = &(client_class->pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;
				if (!strcmp(name, "m_bDidSmokeEffect")) {
				}
			}
		}
		else if (!strcmp(pszName, "DT_BaseViewModel")) {
			for (int i = 0; i < client_class->pRecvTable->m_nProps; i++) {
				RecvProp *pProp = &(client_class->pRecvTable->m_pProps[i]);
				const char *name = pProp->m_pVarName;
				if (!strcmp(name, "m_nModelIndex")) {
					oRecvnModelIndex = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = (RecvVarProxyFn)Hooked_RecvProxy_Viewmodel;
				}
			}
		}
		client_class = client_class->pNext;
	}
}

