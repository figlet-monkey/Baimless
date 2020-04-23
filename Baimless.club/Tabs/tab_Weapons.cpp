/*#include "../Menu.h"
#include "../imgui/impl/imgui_impl_dx9.h"
#include "../imgui/impl/imgui_impl_win32.h"
#include "../Options.hpp"
#include "../http.h"
#include "../SDK/CGlobalVarsBase.h"
#include "../ui.h"
#include "../Render.h"
#include "../Definitions.h"
#include "../skinchanger.h"

void Menu::tab_Weapons()
{
    static char filterGuns[32];
    static char filterSkins[32];

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (50 / 2));
    ImGui::Text("Search");
    ImGui::PushItemWidth(-1);
    ImGui::InputText(("##Search"), filterGuns, IM_ARRAYSIZE(filterGuns));
    ImGui::PopItemWidth();
    ImGui::BeginChild(5, ImVec2(-1, 150), true, 0);
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
    ImGui::Spacing();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (33 / 2));
    ImGui::Text("Search");
    ImGui::PushItemWidth(-1);
    ImGui::InputText(("##Search##1"), filterSkins, IM_ARRAYSIZE(filterSkins));
    ImGui::PopItemWidth();
    ImGui::BeginChild(("##SKINS-2"), ImVec2(-1, 150 - 24), true, 0);
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
    default: weapon_skins = null_skins2; break;
    }
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (280 / 2) - (35 / 2));
    ImGui::Text("Skin ID");
    ImGui::PushItemWidth(-1);
    ImGui::InputInt("##Skin ID-2", &g_Options.W[g_Options.currentWeapon].ChangerSkin);
    ImGui::PopItemWidth();
    ImGui::Spacing();
    for (auto it : weapon_skins)
    {
        if (!(std::string(it.second).find(std::string(filterSkins)) != std::string::npos))
            continue;

        bool selected = ((int)it.first == (int)g_Options.W[g_Options.currentWeapon].ChangerSkin);

        if (ImGui::Selectable(it.second, selected))
        {
            g_Options.W[g_Options.currentWeapon].ChangerSkin = (int)it.first;
            Fullupdate();
            break;
        }
    }
    ImGui::EndChild();

    ImGui::Text("Strattrak");
    ImGui::PushItemWidth(-1);
    ImGui::SliderInt("##StatTrak-2", &g_Options.W[g_Options.currentWeapon].statamount, 0, 1000);
    ImGui::PopItemWidth();
    ImGui::Text("Wear");
    ImGui::PushItemWidth(-1);
    ImGui::SliderFloat("##wear-2", &g_Options.W[g_Options.currentWeapon].wear, 0.001f, 1.f);
    ImGui::PopItemWidth();
}*/
