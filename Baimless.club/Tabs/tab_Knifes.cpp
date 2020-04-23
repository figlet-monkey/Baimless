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
    "Survivak",
    "Paracord"
};


void Menu::tab_Knifes()
{
    static char filterSkins[32];
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (65 / 2));
    ImGui::Text("Knife model");
    ImGui::PushItemWidth(-1);
    ImGui::ListBox("Knife Model", &g_Options.skins_knife_model, knifes, ARRAYSIZE(knifes),15);
    ImGui::PopItemWidth();
    ImGui::Spacing();

    ImGui::BeginChild(("##SKINS"), ImVec2(-1, 150 - 24), true, 0);
    std::string skinStr = "";
    int curItem = -1;
    int s = 0;

    std::vector<std::pair<int, const char*>> weapon_skins;

    switch (g_Options.currentWeapon)
    {
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


    default: weapon_skins = null_skins2; break;
    }
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (280 / 2) - (35 / 2));
    ImGui::Text("Skin ID");
    ImGui::PushItemWidth(-1);
    ImGui::InputInt("##Skin ID", &g_Options.W[g_Options.currentWeapon].ChangerSkin);
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
    ImGui::SliderInt("##StatTrak", &g_Options.W[g_Options.currentWeapon].statamount, 0, 1000);
    ImGui::PopItemWidth();
    ImGui::Text("Wear");
    ImGui::PushItemWidth(-1);
    ImGui::SliderFloat("##wear", &g_Options.W[g_Options.currentWeapon].wear, 0.001f, 1.f);
    ImGui::PopItemWidth();
}
*/
