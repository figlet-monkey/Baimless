#include "../Menu.h"
#include "../imgui/impl/imgui_impl_dx9.h"
#include "../imgui/impl/imgui_impl_win32.h"
#include "../Options.hpp"
#include "../http.h"
#include "../SDK/CGlobalVarsBase.h"
#include "../ui.h"
#include "../Render.h"
#include "../Definitions.h"

#include "../SDK/ICvar.h"
#include "../kit_parser.h"
#include "../skinchanger.h"
#define loop(n) for(int ii = 0; ii < n; ++ ii)
static char* gloves[] = {
    "Default",
    "Bloodhound",
    "Sport",
    "Driver",
    "Hand Wraps",
    "Motorcycle",
    "Specialist",
    "Hydra"
};
static char* knifes[] = {
    "Bayonet",
    "Bowie",
    "Butterfly ",
    "Falchion ",
    "Flip ",
    "Gut ",
    "Huntsman ",
    "Karambit",
    "M9 ",
    "Dagger",
    "Navaja",
    "Stiletto",
    "Ursus",
    "Talon",
    "Skeleton",
    "Nomad",
    "Survival",
    "Paracord",
    "Classic"
};
template<size_t N>
void RENDER_TAB_SAMELINE(char* (&names)[N], int& activetab, float w, float h)
{
    bool values[N] = { false };
    values[activetab] = true;
    for (auto i = 0; i < N; ++i) {
        if (ImGui::ToggleButton(names[i], &values[i], 6, ImVec2{ w, h }))
            activetab = i;
        ImGui::SameLine();
    }
}


void Menu::tab_Skin_Changer()
{
    if (g_pEngine->IsInGame() && g_pEngine->IsConnected())
        g_Options.GLOVE = true;
    else
        g_Options.GLOVE = false;
    static int currWeap = g_Options.currentWeapon;
    int pk = g_Options.W[g_Options.currentWeapon].ChangerSkin;
    static int currWeapSkin = g_Options.W[g_Options.currentWeapon].ChangerSkin;
    static char weapName[18];
    static char filterSkins[32];
    static char filterGuns[32];
    
    ImGui::Checkbox("Active", &g_Options.skins_enable);


    const char* gstr;
    if (g_Options.skins_glove_model == 1)
        gstr = "Charred\0\rSnakebite\0\rBronzed\0\rGuerilla\0\0";
    else if (g_Options.skins_glove_model == 2)
        gstr = "Hedge Maze\0\rPandoras Box\0\rSuperconductor\0\rArid\0\rVice\0\rOmega\0\rAmphibious\0\rBronze Morph\0\0";
    else if (g_Options.skins_glove_model == 3)
        gstr = "Lunar Weave\0\rConvoy\0\rCrimson Weave\0\rDiamondback\0\rOvertake\0\rRacing Green\0\rKing Snake\0\rImperial Plaid\0\0";
    else if (g_Options.skins_glove_model == 4)
        gstr = "Leather\0\rSpruce DDPAT\0\rSlaughter\0\rBadlands\0\rCobalt Skulls\0\rOverprint\0\rDuct Tape\0\rArboreal\0\0";
    else if (g_Options.skins_glove_model == 5)
        gstr = "Eclipse\0\rSpearmint\0\rBoom!\0\rCool Mint\0\rTurtle\0\rTransport\0\rPolygon\0\rPOW!\0\0";
    else if (g_Options.skins_glove_model == 6)
        gstr = "Forest DDPAT\0\rCrimson Kimono\0\rEmerald Web\0\rFoundation\0\rCrimson Web\0\rBuckshot\0\rFade\0\rMogul\0\0";
    else if (g_Options.skins_glove_model == 7)
        gstr = "Emerald\0\rMangrove\0\rRattler\0\rCase Hardened\0\0";
    else
        gstr = "";
    
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (ImGui::CalcTextSize("Knife model").x / 2));
    ImGui::Text("Knife model");
    ImGui::PushItemWidth(-1);
    ImGui::Combo("##Knife-Model", &g_Options.skins_knife_model, knifes, ARRAYSIZE(knifes));
    ImGui::PopItemWidth();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (ImGui::CalcTextSize("Glove model").x / 2));
	ImGui::Text("Glove model");
	ImGui::PushItemWidth(-1);
	ImGui::Combo("##Glove Model", &g_Options.skins_glove_model, gloves, ARRAYSIZE(gloves));
	ImGui::PopItemWidth();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (ImGui::CalcTextSize("Glove skin").x / 2));
	ImGui::Text("Glove skin");
	ImGui::PushItemWidth(-1);
    ImGui::Combo("##glove-skin", &g_Options.skins_glove_skin, gstr, IM_ARRAYSIZE(gstr));
	ImGui::PopItemWidth();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (ImGui::CalcTextSize("Search").x / 2));
    ImGui::Text("Search");
    ImGui::PushItemWidth(-1);
    ImGui::InputText(("##Search"), filterGuns, IM_ARRAYSIZE(filterGuns));
    ImGui::PopItemWidth();
    ImGui::BeginChild(5, ImVec2(-1, 140), true);
    for (auto it : guns)
    {
        if (strcmp(it.second, "<-Default->") == 0)
            continue;

        if (!(std::string(it.second).find(std::string(filterGuns)) != std::string::npos))
            continue;

        const bool item_selected = ((int)it.first == (int)g_Options.currentWeapon);

        if (ImGui::Selectable(it.second, item_selected))
        {
            g_Options.currentWeapon = (int)it.first;
        }
    }
    ImGui::EndChild();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (ImGui::CalcTextSize("Search").x / 2));
    ImGui::Text("Search");
    ImGui::PushItemWidth(-1);
    ImGui::InputText(("##Search##1"), filterSkins, IM_ARRAYSIZE(filterSkins));
    ImGui::PopItemWidth();
    ImGui::BeginChild(("##SKINS"), ImVec2(-1, 115 ), true);
    std::string skinStr = "";
    int curItem = -1;
    int s = 0;

    std::vector<std::pair<int, const char*>> weapon_skins;

    switch (g_Options.currentWeapon)
    {
    case (int)ItemDefinitionIndex::WEAPON_AK47: weapon_skins = ak47_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_AUG: weapon_skins = aug_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_AWP: weapon_skins = awp_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_BIZON: weapon_skins = bizon_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_CZ75: weapon_skins = cz_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_DEAGLE: weapon_skins = deagle_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_ELITE: weapon_skins = elite_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_FAMAS: weapon_skins = famas_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_FIVESEVEN: weapon_skins = five7_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_G3SG1: weapon_skins = g3sg1_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_GALIL: weapon_skins = galil_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_GLOCK: weapon_skins = glock_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_M249: weapon_skins = m249_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_M4A4: weapon_skins = m4a1_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_M4A1S: weapon_skins = m4a1s_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_MAC10: weapon_skins = mac10_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_MAG7: weapon_skins = mag7_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_MP7: weapon_skins = mp7_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_MP9: weapon_skins = mp9_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_NEGEV: weapon_skins = negev_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_NOVA: weapon_skins = nova_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_P2000: weapon_skins = p2000_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_P250: weapon_skins = p250_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_P90: weapon_skins = p90_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_SAWEDOFF: weapon_skins = sawedoff_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_SCAR20: weapon_skins = scar_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_SSG08: weapon_skins = scout_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_SG553: weapon_skins = sg556_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_TEC9: weapon_skins = tec9_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_UMP45: weapon_skins = ump_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_USPS: weapon_skins = usp_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_REVOLVER: weapon_skins = revolver_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_MP5SD: weapon_skins = MP5; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_BAYONET:weapon_skins = bayonet_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET:weapon_skins = m9bayonet_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE:weapon_skins = bowie_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY:weapon_skins = butterfly_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_PUSH:weapon_skins = push_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_FALCHION:weapon_skins = falchion_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_FLIP:weapon_skins = flip_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_GUT:weapon_skins = gut_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL:weapon_skins = tatical_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT:weapon_skins = karambit_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_GYPSY_JACKKNIFE:weapon_skins = jack_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_STILETTO:weapon_skins = stiletto_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_URSUS:weapon_skins = ursus_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_WIDOWMAKER:weapon_skins = widowmaker_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_SKELETON:weapon_skins = skeleton_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_OUTDOOR:weapon_skins = outdoor_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_CANIS:weapon_skins = canis_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_CORD:weapon_skins = cord_skins; break;
    case (int)ItemDefinitionIndex::WEAPON_KNIFE_CSS:weapon_skins = classic_skins; break;
    default: weapon_skins = null_skins2; break;
    }
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (ImGui::CalcTextSize("Skin ID").x / 2));
    ImGui::Text("Skin ID");
    ImGui::PushItemWidth(-1);
    ImGui::InputInt("##Skin ID", &g_Options.W[g_Options.currentWeapon].ChangerSkin);
    ImGui::PopItemWidth();
    for (auto it : weapon_skins)
    {
        if (!(std::string(it.second).find(std::string(filterSkins)) != std::string::npos))
            continue;

        bool selected = ((int)it.first == (int)g_Options.W[g_Options.currentWeapon].ChangerSkin);

        if (ImGui::Selectable(it.second, selected))
        {
            g_Options.W[g_Options.currentWeapon].ChangerSkin = (int)it.first;
            //
		
            break;
        }
    }
    ImGui::EndChild();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (ImGui::CalcTextSize("Stratrak").x / 2));
    ImGui::Text("Stratrak");
    ImGui::PushItemWidth(-1);
    ImGui::SliderInt("##StatTrak", &g_Options.W[g_Options.currentWeapon].statamount, 0, 1000);
    ImGui::PopItemWidth();
  
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (ImGui::CalcTextSize("Wear").x / 2));
    ImGui::Text("Wear");
    ImGui::PushItemWidth(-1);
    ImGui::SliderFloat("##wear", &g_Options.W[g_Options.currentWeapon].wear, 0.001f, 1.f);
    ImGui::PopItemWidth();
	if (ImGui::Button("Fullupdate", ImVec2(-1, -1)))
		Fullupdate();
	ImGui::Columns(1);
}
