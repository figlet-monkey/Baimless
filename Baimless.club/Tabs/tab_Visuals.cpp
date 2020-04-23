#include "../Menu.h"
#include "../imgui/impl/imgui_impl_dx9.h"
#include "../imgui/impl/imgui_impl_win32.h"
#include "../Options.hpp"
#include "../http.h"
#include "../SDK/CGlobalVarsBase.h"
#include "../ui.h"
#include "../Render.h"
#include <Hooks.h>

#define loop(n) for(int ii = 0; ii < n; ++ ii)
template<size_t N>
void RENDER_TAB_SAMELINE(char* (&names)[N], int& activetab, float w, float h)
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    bool values[N] = { false };
    values[activetab] = true;
    for (auto i = 0; i < N; ++i) {
        if (ImGui::ToggleButton(names[i], &values[i], 6, ImVec2{ w, h }))
            activetab = i;
        ImGui::SameLine();
    }
    ImGui::PopStyleVar();
}
namespace ImGuiEx
{
    inline bool ColorEdit4(const char* label, Color* v, bool show_alpha = true)
    {
        auto clr = ImVec4{
            v->r() / 255.0f,
            v->g() / 255.0f,
            v->b() / 255.0f,
            v->a() / 255.0f
        };

        if (ImGui::ColorEdit4(label, &clr.x, ImGuiColorEditFlags_NoInputs)) {
            v->SetColor(clr.x, clr.y, clr.z, clr.w);
            return true;
        }
        return false;
    }
    inline bool ColorEdit3(const char* label, Color* v)
    {
        return ColorEdit4(label, v, false);
    }
    inline bool ColorEdit3(const char* label, float* v)
    {
        return ImGui::ColorEdit3(label, v,ImGuiColorEditFlags_NoInputs);
    }
}

void Menu::tab_Visuals()
{
    loop(3)ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15, 0.12, 0.21, 10));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.08, 0.07, 0.11, 1.0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.11, 0.09, 0.16, 1.0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 2));
    ImGui::Columns(2, NULL, false);
    ImGui::PushFont(bold);
    ImGui::SameLine((290 / 2) - (75 / 2));
    ImGui::Text("Player-ESP");
    ImGui::PopFont();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::BeginChild("##visuals-1", ImVec2(290, 362), true);
    /**/
    if (!g_Options.esp_streamproof) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        static char* player_esp[] = { "Enemy","Team","Local" };
        RENDER_TAB_SAMELINE(player_esp, g_Options.esp.player_esp, 280 / 3, 20);
        ImGui::NewLine();   ImGui::NewLine();
        ImGui::PopStyleVar();
        /**/
        ImGui::Checkbox("Enable ##visuals-esp", &g_Options.esp.enable);
        switch (g_Options.esp.player_esp)
        {
        case 0:

            ImGuiEx::ColorEdit3("##1-enemy", esp::colors::enemy::box_col); ImGui::SameLine(); ImGui::Checkbox("Box ##visuals-esp-enemy", &g_Options.esp.enemy.box);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##visuals-esp-1-enemy", &g_Options.esp.enemy.box_type, "Normal\0\rCorner\0\0", IM_ARRAYSIZE("Normal\0\rCorner\0\0")); ImGui::PopItemWidth();
            ImGuiEx::ColorEdit3("##2-enemy", esp::colors::enemy::name_col); ImGui::SameLine();  ImGui::Checkbox("Name ##visuals-esp-enemy", &g_Options.esp.enemy.name);
            ImGui::Checkbox("Health ##visuals-esp-enemy", &g_Options.esp.enemy.health);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##visuals-esp-2-enemy", &g_Options.esp.enemy.health_type, "Normal\0\rBattery\0\rText\0\0", IM_ARRAYSIZE("Normal\0\rBattery\0\rText\0\0")); ImGui::PopItemWidth();
            ImGuiEx::ColorEdit3("##skeletonbaldo", esp::colors::enemy::skeleton); ImGui::SameLine(); ImGui::Text("Skeleton");
            ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##visuals-esp-3-enemy", &g_Options.esp.enemy.skeleton_type, "Off\0\rNormal\0\rBacktrack\0\0", IM_ARRAYSIZE("Off\0\rNormal\0\rBacktrack\0\0")); ImGui::PopItemWidth();
            ImGuiEx::ColorEdit3("##4-enemy", esp::colors::enemy::ammo); ImGui::SameLine(); ImGui::Checkbox("Ammo bar ##visuals-esp-enemy", &g_Options.esp.enemy.ammo);
            ImGuiEx::ColorEdit3("##5-enemy", esp::colors::enemy::weapons); ImGui::SameLine(); ImGui::Checkbox("Weapon ##visuals-esp-enemy", &g_Options.esp.enemy.weapon);
            ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##visuals-esp-4-enemy", &g_Options.esp.enemy.weapon_type, "Text\0\rIcon\0\0", IM_ARRAYSIZE("Text\0\rIcon\0\0")); ImGui::PopItemWidth();
            ImGuiEx::ColorEdit3("##6-enemy", esp::colors::enemy::money); ImGui::SameLine(); ImGui::Checkbox("Money info ##visuals-esp-enemy", &g_Options.esp.enemy.money);
            ImGuiEx::ColorEdit3("##7-enemy", esp::colors::enemy::helmet); ImGui::SameLine(); ImGui::Checkbox("Helmet info ##visuals-esp-enemy", &g_Options.esp.enemy.helmet);
            ImGuiEx::ColorEdit3("##8-enemy", esp::colors::enemy::scoped); ImGui::SameLine(); ImGui::Checkbox("Scoped info ##visuals-esp-enemy", &g_Options.esp.enemy.scoped);
            ImGuiEx::ColorEdit3("##9-enemy", esp::colors::enemy::fakeduck); ImGui::SameLine(); ImGui::Checkbox("Fakeduck info ##visuals-esp-enemy", &g_Options.esp.enemy.fakeduck);
            break;
        case 1:
            ImGuiEx::ColorEdit3("##1-team", esp::colors::team::box_col); ImGui::SameLine(); ImGui::Checkbox("Box ##visuals-esp-team", &g_Options.esp.team.box);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##visuals-esp-1-team", &g_Options.esp.team.box_type, "Normal\0\rCorner\0\0", IM_ARRAYSIZE("Normal\0\rCorner\0\0")); ImGui::PopItemWidth();
            ImGuiEx::ColorEdit3("##2-team", esp::colors::team::name_col); ImGui::SameLine();  ImGui::Checkbox("Name ##visuals-esp-team", &g_Options.esp.team.name);
            ImGui::Checkbox("Health ##visuals-esp-team", &g_Options.esp.team.health);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##visuals-esp-2-team", &g_Options.esp.team.health_type, "Normal\0\rBattery\0\rText\0\0", IM_ARRAYSIZE("Normal\0\rBattery\0\rText\0\0")); ImGui::PopItemWidth();
            ImGuiEx::ColorEdit3("##skeletonbaldo2", esp::colors::team::skeleton); ImGui::SameLine(); ImGui::Text("Skeleton");
            ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##visuals-esp-3-team", &g_Options.esp.team.skeleton_type, "Off\0\rNormal\0\rBacktrack\0\0", IM_ARRAYSIZE("Off\0\rNormal\0\rBacktrack\0\0")); ImGui::PopItemWidth();
            ImGuiEx::ColorEdit3("##4-team", esp::colors::team::ammo); ImGui::SameLine(); ImGui::Checkbox("Ammo bar ##visuals-esp-team", &g_Options.esp.team.ammo);
            ImGuiEx::ColorEdit3("##5-team", esp::colors::team::weapons); ImGui::SameLine(); ImGui::Checkbox("Weapon ##visuals-esp-team", &g_Options.esp.team.weapon);
            ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##visuals-esp-4-team", &g_Options.esp.team.weapon_type, "Text\0\rIcon\0\0", IM_ARRAYSIZE("Text\0\rIcon\0\0")); ImGui::PopItemWidth();
            ImGuiEx::ColorEdit3("##6-team", esp::colors::team::money); ImGui::SameLine(); ImGui::Checkbox("Money info ##visuals-esp-team", &g_Options.esp.team.money);
            ImGuiEx::ColorEdit3("##7-team", esp::colors::team::helmet); ImGui::SameLine(); ImGui::Checkbox("Helmet info ##visuals-esp-team", &g_Options.esp.team.helmet);
            ImGuiEx::ColorEdit3("##8-team", esp::colors::team::scoped); ImGui::SameLine(); ImGui::Checkbox("Scoped info ##visuals-esp-team", &g_Options.esp.team.scoped);
            ImGuiEx::ColorEdit3("##9-team", esp::colors::team::fakeduck); ImGui::SameLine(); ImGui::Checkbox("Fakeduck info ##visuals-esp-team", &g_Options.esp.team.fakeduck);
            break;
        case 2:
            ImGuiEx::ColorEdit3("##1-local", esp::colors::local::box_col); ImGui::SameLine(); ImGui::Checkbox("Box ##visuals-esp-local", &g_Options.esp.local.box);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##visuals-esp-1-local", &g_Options.esp.local.box_type, "Normal\0\rCorner\0\0", IM_ARRAYSIZE("Normal\0\rCorner\0\0")); ImGui::PopItemWidth();
            ImGuiEx::ColorEdit3("##2-local", esp::colors::local::name_col); ImGui::SameLine();  ImGui::Checkbox("Name ##visuals-esp-local", &g_Options.esp.local.name);
            ImGui::Checkbox("Health ##visuals-esp-local", &g_Options.esp.local.health);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##visuals-esp-2-local", &g_Options.esp.local.health_type, "Normal\0\rBattery\0\rText\0\0", IM_ARRAYSIZE("Normal\0\rBattery\0\rText\0\0")); ImGui::PopItemWidth();
            ImGuiEx::ColorEdit3("##skeletonbaldo3", esp::colors::enemy::skeleton); ImGui::SameLine(); ImGui::Text("Skeleton");
            ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##visuals-esp-3-local", &g_Options.esp.local.skeleton_type, "Off\0\rNormal\0\rBacktrack\0\0", IM_ARRAYSIZE("Off\0\rNormal\0\rBacktrack\0\0")); ImGui::PopItemWidth();
            ImGuiEx::ColorEdit3("##4-local", esp::colors::local::ammo); ImGui::SameLine(); ImGui::Checkbox("Ammo bar ##visuals-esp-local", &g_Options.esp.local.ammo);
            ImGuiEx::ColorEdit3("##5-local", esp::colors::local::weapons); ImGui::SameLine(); ImGui::Checkbox("Weapon ##visuals-esp-local", &g_Options.esp.local.weapon);
            ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##visuals-esp-4-local", &g_Options.esp.local.weapon_type, "Text\0\rIcon\0\0", IM_ARRAYSIZE("Text\0\rIcon\0\0"));
            ImGuiEx::ColorEdit3("##6-local", esp::colors::local::money); ImGui::SameLine(); ImGui::Checkbox("Money info ##visuals-esp-local", &g_Options.esp.local.money);
            ImGuiEx::ColorEdit3("##7-local", esp::colors::local::helmet); ImGui::SameLine(); ImGui::Checkbox("Helmet info ##visuals-esp-local", &g_Options.esp.local.helmet);
            ImGuiEx::ColorEdit3("##8-local", esp::colors::local::scoped); ImGui::SameLine(); ImGui::Checkbox("Scoped info ##visuals-esp-local", &g_Options.esp.local.scoped);
            ImGuiEx::ColorEdit3("##9-local", esp::colors::local::fakeduck); ImGui::SameLine(); ImGui::Checkbox("Fakeduck info ##visuals-esp-local", &g_Options.esp.local.fakeduck);
            break;
        }
    }
    else
    {
        ImGui::Checkbox("Obs spoofer", &g_Options.esp_streamproof);
        ImGui::Checkbox("Team check", &g_Options.esp_enemies_only);
        ImGui::Checkbox("Boxes", &g_Options.esp_player_boxes);
        ImGui::Checkbox("Names", &g_Options.esp_player_names);
        ImGui::Checkbox("Health", &g_Options.esp_player_health);
        ImGui::Checkbox("Armour", &g_Options.esp_player_armour);
        ImGui::Checkbox("Weapon", &g_Options.esp_player_weapons);
        ImGui::Checkbox("Snaplines", &g_Options.esp_player_snaplines);
        ImGui::Checkbox("Crosshair", &g_Options.esp_crosshair);
        ImGui::Checkbox("Dropped Weapons", &g_Options.esp_dropped_weapons);
        ImGui::Checkbox("Defuse Kit", &g_Options.esp_defuse_kit);
        ImGui::Checkbox("Planted C4", &g_Options.esp_planted_c4);
        ImGui::Checkbox("Item Esp", &g_Options.esp_items);
    }
    ImGui::EndChild();

    ImGui::PushFont(bold);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (115 / 2));
    ImGui::Text("Colored models");
    ImGui::PopFont();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::BeginChild("##visuals-2", ImVec2(290, 208), true,ImGuiWindowFlags_NoScrollbar);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    static char* colored_models_char[] = { "Enemy","Team","Local" };
    RENDER_TAB_SAMELINE(colored_models_char, g_Options.esp.colored_models, 280 / 3, 20);
    ImGui::NewLine();
    ImGui::PopStyleVar();
    switch (g_Options.esp.colored_models)
    {
    case 0:
        ImGui::Text("Colored models material");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##visuals-materials-enemy", &g_Options.cmaterials.enemy.material, "Normal\0\rReflective\0\rBright reflective\0\rFlat\0\rDark\0\rPulse\0\rGlow\0\rDark glow", IM_ARRAYSIZE("Normal\0\rReflective\0\rBright reflective\0\rFlat\0\rDark\0\rPulse\0\rGlow\0\rDark glow")); 
        ImGuiEx::ColorEdit3("##31-enemy",materials::colors::enemy::player); ImGui::SameLine(); ImGui::Checkbox("Player ##visuals-materials-enemy", &g_Options.cmaterials.enemy.player);
        ImGuiEx::ColorEdit3("##32-enemy", materials::colors::enemy::behind_wall); ImGui::SameLine(); ImGui::Checkbox("Player behind wall ##visuals-materials-enemy", &g_Options.cmaterials.enemy.behind_wall);
        ImGuiEx::ColorEdit3("##33-enemy",materials::colors::enemy::backtrack); ImGui::SameLine(); ImGui::Checkbox("Backtrack player ##visuals-materials-enemy", &g_Options.cmaterials.enemy.backtrack);
        ImGui::Text("Chams opacity");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##visuals-materials-chams-opacity-enemy", &g_Options.cmaterials.enemy.chams_opactiy, 0, 100, "%.f%%");
        break;
    case 1:
        ImGui::Text("Colored models material");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##visuals-materials-team", &g_Options.cmaterials.team.material, "Normal\0\rReflective\0\rBright reflective\0\rFlat\0\rDark\0\rPulse\0\rGlow\0\rDark glow", IM_ARRAYSIZE("Normal\0\rReflective\0\rBright reflective\0\rFlat\0\rDark\0\rPulse\0\rGlow\0\rDark glow")); 
        ImGuiEx::ColorEdit3("##31-team", materials::colors::team::player); ImGui::SameLine(); ImGui::Checkbox("Player ##visuals-materials-team", &g_Options.cmaterials.team.player);
        ImGuiEx::ColorEdit3("##32-team", materials::colors::team::player); ImGui::SameLine(); ImGui::Checkbox("Player behind wall ##visuals-materials-team", &g_Options.cmaterials.team.behind_wall);
        //ImGuiEx::ColorEdit3("##33-team", g_Options.cmaterials.colors.team.backtrack); ImGui::SameLine(); ImGui::Checkbox("Backtrack player ##visuals-materials-team", &g_Options.cmaterials.team.backtrack);
        ImGui::Text("Chams opacity");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##visuals-materials-chams-opacity-team", &g_Options.cmaterials.team.chams_opactiy, 0, 100, "%.f%%");
        break;
    case 2:
        ImGui::Text("Colored models material");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##visuals-materials-local", &g_Options.cmaterials.local.material, "Normal\0\rReflective\0\rBright reflective\0\rFlat\0\rDark\0\rPulse\0\rGlow\0\rDark glow", IM_ARRAYSIZE("Normal\0\rReflective\0\rBright reflective\0\rFlat\0\rDark\0\rPulse\0\rGlow\0\rDark glow")); 
        ImGuiEx::ColorEdit3("##31-local-chams", materials::colors::local::chams); ImGui::SameLine(); ImGui::Checkbox("Local chams ##visuals-materials-local-chams", &g_Options.cmaterials.local.chams);
        ImGuiEx::ColorEdit3("##31-local-chams-desync", materials::colors::local::desync); ImGui::SameLine(); ImGui::Checkbox("Local desync ##visuals-materials-local-chams-desync", &g_Options.cmaterials.local.desync);
        ImGuiEx::ColorEdit3("##31-local-fakelag", materials::colors::local::fakelag); ImGui::SameLine(); ImGui::Checkbox("Local fakelag ##visuals-materials-fakelag", &g_Options.cmaterials.local.fakelag);
        ImGui::Text("Chams opacity");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##visuals-materials-chams-opacity-local", &g_Options.cmaterials.local.chams_opactiy, 0, 100, "%.f%%");
        ImGuiEx::ColorEdit3("##31-local-hand-chams", materials::colors::local::hand_chams); ImGui::SameLine(); ImGui::Checkbox("Hand chams ##materials-other", &g_Options.cmaterials.other.hand_chams);
        ImGui::Text("Hand material");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##materials-other", &g_Options.cmaterials.other.hand_chams_material, "Normal\0\rReflective\0\rBright reflective\0\rFlat\0\rDark\0\rPulse\0\rWireframe", IM_ARRAYSIZE("Normal\0\rReflective\0\rBright reflective\0\rFlat\0\rDark\0\rPulse\0\rWireframe")); ImGui::PopItemWidth();
        ImGui::NewLine();
        /*ImGuiEx::ColorEdit3("##31-local", g_Options.cmaterials.colors.local.player); ImGui::SameLine(); ImGui::Checkbox("Player ##visuals-materials-local", &g_Options.cmaterials.local.player);
        ImGuiEx::ColorEdit3("##32-local", g_Options.cmaterials.colors.local.behind_wall); ImGui::SameLine(); ImGui::Checkbox("Player behind wall ##visuals-materials-local", &g_Options.cmaterials.local.behind_wall);
        //ImGuiEx::ColorEdit3("##33-local", g_Options.cmaterials.colors.local.backtrack); ImGui::SameLine(); ImGui::Checkbox("Backtrack player ##visuals-materials-local", &g_Options.cmaterials.local.backtrack);
       */
        break;
   
    }
    ImGui::EndChild();


    ImGui::NextColumn();

    ImGui::PushFont(bold);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (70 / 2));
    ImGui::Text("Other ESP");
    ImGui::PopFont();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
    ImGui::BeginChild("##visuals-3", ImVec2(290, 240), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    static char* other_esp_char[] = { "Other","More","More##2","Extra" };
    RENDER_TAB_SAMELINE(other_esp_char, g_Options.esp.other_esp, 280 / 4, 20);
    ImGui::NewLine();   ImGui::NewLine();
    ImGui::PopStyleVar();
    switch (g_Options.esp.other_esp)
    {
    case 0:
        ImGui::Checkbox("Hit marker ##other-esp", &g_Options.esp.other.hitmarker);
        ImGui::Text("Hit marker sound");     
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##other-esp", &g_Options.esp.other.hitmark_sound, "Off\0\rMetallic\0\rBubble\0\rBameware\0\rLaser\0\rQuake\0\rUT\0\0", IM_ARRAYSIZE("Off\0\rMetallic\0\rBubble\0\rBameware\0\rLaser\0\rQuake\0\rUT\0\0")); ImGui::PopItemWidth();
        if (g_Options.esp.other.hitmarker)
            ImGui::Checkbox("Hit effect ##other-esp", &g_Options.esp.other.hit_effect);
        ImGui::Text("Override viewmodel FOV");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##other-esp-override-fov", &g_Options.esp.other.override_viewmodel_fov, 0, 50, "%.f%%");
        ImGui::Text("Override viewangle FOV");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##other-esp-override-viewangle-fov", &g_Options.esp.other.override_viewangle_fov, 0, 120, "%.f%%");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGuiEx::ColorEdit3("##49", esp::colors::sound_esp); ImGui::SameLine(); ImGui::Checkbox("Sound esp ##other-esp", &g_Options.esp.other.sound_esp);
        ImGui::Checkbox("Obs spoofer", &g_Options.esp_streamproof);
        ImGui::Checkbox("Disable backdrop##other-esp", &g_Options.esp.other.disable_backdrop);
        ImGui::NewLine();
        break;
    case 1:
        ImGui::Checkbox("Feet circle ##other-esp", &g_Options.esp.other.feet_circle);
        ImGuiEx::ColorEdit3("##50", esp::colors::zeus_range); ImGui::SameLine(); ImGui::Checkbox("Zeus range ##other-esp", &g_Options.esp.other.zeus_range);
        ImGui::Text("Fov X");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##other-esp-fov-x", &g_Options.esp.other.fov_x, -10, 10, "%.f");
        ImGui::Text("Fov Y");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##other-esp-fov-y", &g_Options.esp.other.fov_y, -10, 10, "%.f");
        ImGui::Text("Fov Z");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##other-esp-fox-z", &g_Options.esp.other.fov_z, -10, 10, "%.f");
        ImGui::Text("World transparency");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderInt("##other-esp-world-transparency", &g_Options.esp.other.world_transparency, 0, 100,"%d%%");
        ImGui::Text("Smoke ESP");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGuiEx::ColorEdit3("##smoke-esp-color", esp::colors::nadecolorr);
        ImGui::SameLine();
        if (ImGui::BeginCombo("##smoke-indicator", g_Options.esp.smoke.c_str()))
        {
            g_Options.esp.smoke = "";
            const char* indicator_char[] = { "Nade ESP","Timer","Radius" };
            std::vector<std::string> vec;
            for (size_t i = 0; i < IM_ARRAYSIZE(indicator_char); i++)
            {
                ImGui::Selectable(indicator_char[i], &g_Options.esp.nade_esp_smoke[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
                if (g_Options.esp.nade_esp_smoke[i])
                    vec.push_back(indicator_char[i]);
            }
            for (size_t i = 0; i < vec.size(); i++)
            {
                if (vec.size() == 1)
                    g_Options.esp.smoke += vec.at(i);
                else if (!(i == vec.size()))
                    g_Options.esp.smoke += vec.at(i) + ",";
                else
                    g_Options.esp.smoke += vec.at(i);
            }
            ImGui::EndCombo();
        }
        ImGui::Text("Molotov ESP");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGuiEx::ColorEdit3("##molo-esp-color", esp::colors::nadecolorr);
        ImGui::SameLine();
        if (ImGui::BeginCombo("##molo-tovindicator", g_Options.esp.molotov.c_str()))
        {
            g_Options.esp.molotov = "";
            const char* indicator_char[] = { "Nade ESP","Timer","Radius" };
            std::vector<std::string> vec;
            for (size_t i = 0; i < IM_ARRAYSIZE(indicator_char); i++)
            {
                ImGui::Selectable(indicator_char[i], &g_Options.esp.nade_esp_molotov[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
                if (g_Options.esp.nade_esp_molotov[i])
                    vec.push_back(indicator_char[i]);
            }
            for (size_t i = 0; i < vec.size(); i++)
            {
                if (vec.size() == 1)
                    g_Options.esp.molotov += vec.at(i);
                else if (!(i == vec.size()))
                    g_Options.esp.molotov += vec.at(i) + ",";
                else
                    g_Options.esp.molotov += vec.at(i);
            }
            ImGui::EndCombo();
        }
        ImGui::NewLine();
        break;
    case 2:
        ImGui::Checkbox("Indicators ##other-esp", &g_Options.esp.other.indicators);
        ImGui::Text("X Axis");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##other-esp-x-axis-indicators", &g_Options.esp.other.indicators_x, 0, 1800, "%.f%%");
        ImGui::Text("Y Axis");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##other-esp-y-axis-indicators", &g_Options.esp.other.indicators_y, 0, 1000, "%.f%%");
        ImGui::Checkbox("Fake indicator ##other-esp", &g_Options.esp.other.fake_indicator);
        ImGui::Checkbox("Choke indicator ##other-esp", &g_Options.esp.other.choke_indicator);
        ImGui::Checkbox("Kill counter ##other-esp", &g_Options.esp.other.kill_counter);
        ImGui::Checkbox("WASD Indicator ##other-esp", &g_Options.esp.other.wasd);
        ImGuiEx::ColorEdit3("##COLOR_BOMB", esp::colors::bomb_timer_col); ImGui::SameLine();
        ImGui::Checkbox("Bomb timer ##other-esp", &g_Options.esp.other.bomb_timer);
        ImGui::NewLine();
        break;
    case 3:
        ImGui::Checkbox("Spectator list ##other-esp", &g_Options.esp.other.spectator_list);
        ImGui::Text("X Axis");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##other-esp-x-axis", &g_Options.esp.other.spectator_x, 0, 1800, "%.f%%");
        ImGui::Text("Y Axis");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##other-esp-y-axis", &g_Options.esp.other.spectator_y, 0, 1000, "%.f%%");
        ImGui::Text("Prop transparency");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##other-esp-prop-transparency", &g_Options.esp.other.prop_transp, 0, 100, "%.f%%");
        ImGui::Text("Local scoped alpha");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##other-esp-local-scoped-alpha", &g_Options.esp.other.scoped_alpha, 0, 100, "%.f%%");
        ImGui::NewLine();
        break;
    }
    ImGui::EndChild();

    ImGui::PushFont(bold);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (52 / 2));
    ImGui::Text("Effects");
    ImGui::PopFont();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
    ImGui::BeginChild("##visuals-4", ImVec2(290, 330), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    static char* effects_esp_char[] = { "Effects","Extra","More" };
    RENDER_TAB_SAMELINE(effects_esp_char, g_Options.esp.effects_esp, 280 / 3, 20);
    ImGui::NewLine();   ImGui::NewLine();
    ImGui::PopStyleVar();
    switch (g_Options.esp.effects_esp)
    {
    case 0:
        ImGui::Checkbox("Night mode ##effects", &g_Options.esp.effects.night_mode);
        ImGui::Checkbox("Full bright ##effects", &g_Options.esp.effects.fullbright);
        ImGui::Checkbox("Remove scope overlay ##effects", &g_Options.esp.effects.removescopeoverlay);
        ImGui::Text("Scope type");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##scope_type", &g_Options.esp.effects.scope_type, "Static\0\rMotion\0\0", IM_ARRAYSIZE("Static\0\rMotion\0\0")); ImGui::PopItemWidth();
        ImGui::Checkbox("Remove zoom effects ##effects", &g_Options.esp.effects.remove_zoom);
        ImGui::Checkbox("Remove flashbang effects ##effects", &g_Options.esp.effects.remove_flash);
        ImGui::Checkbox("Remove visual recoil ##effects", &g_Options.esp.effects.remove_recoil);
        ImGui::Checkbox("Remove smoke grenades ##effects", &g_Options.esp.effects.remove_smoke);
        ImGui::Checkbox("Disable post processing ##effects", &g_Options.esp.effects.disable_post);
        ImGui::Text("Indicator");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        if (ImGui::BeginCombo("##indicator", g_Options.esp.effects.indicators.c_str()))
        {
            g_Options.esp.effects.indicators = "";
            const char* indicator_char[] = { "Override","FD","Fake" };
            std::vector<std::string> vec;
            for (size_t i = 0; i < IM_ARRAYSIZE(indicator_char); i++)
            {
                ImGui::Selectable(indicator_char[i], &g_Options.esp.effects.indicator_selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
                if (g_Options.esp.effects.indicator_selected[i])
                    vec.push_back(indicator_char[i]);
            }
            for (size_t i = 0; i < vec.size(); i++)
            {
                if (vec.size() == 1)
                    g_Options.esp.effects.indicators += vec.at(i);
                else if (!(i == vec.size()))
                    g_Options.esp.effects.indicators += vec.at(i) + ",";
                else
                    g_Options.esp.effects.indicators += vec.at(i);
            }
            ImGui::EndCombo();
        }
		ImGuiEx::ColorEdit3("##grenadehelper-color", esp::colors::grenadehelper); ImGui::SameLine(); ImGui::Checkbox("Grenade helper##misc", &g_Options.esp.helper);
        //if (ImGui::Button("Update map", ImVec2(-1, -1)))
            //update_map();
        break;
    case 1:
        ImGui::Checkbox("Watermark ##effects", &g_Options.esp.effects.watermark);
        ImGui::Text("Watermark model");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##watermark", &g_Options.esp.effects.watermark_option, "New\0\rOld\0\0");
        ImGuiEx::ColorEdit3("##beam_color", esp::colors::beam_color);
        ImGui::SameLine(); ImGui::Checkbox("Beam ##effects", &g_Options.esp.effects.beam);

        ImGui::Text("Beam selection");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        if (ImGui::BeginCombo("##beam", g_Options.esp.effects.beams.c_str()))
        {
            g_Options.esp.effects.beams = "";
            const char* beam_char[] = { "Local","Enemy" };
            std::vector<std::string> vec;
            for (size_t i = 0; i < IM_ARRAYSIZE(beam_char); i++)
            {
                ImGui::Selectable(beam_char[i], &g_Options.esp.effects.beam_selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
                if (g_Options.esp.effects.beam_selected[i])
                    vec.push_back(beam_char[i]);
            }
            for (size_t i = 0; i < vec.size(); i++)
            {
                if (vec.size() == 1)
                    g_Options.esp.effects.beams += vec.at(i);
                else if (!(i == vec.size()))
                    g_Options.esp.effects.beams += vec.at(i) + ",";
                else
                    g_Options.esp.effects.beams += vec.at(i);
            }
            ImGui::EndCombo();
        }
        ImGui::Text("Beam life");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##beam-life", &g_Options.esp.effects.beam_life, 0, 10, "%.f");
        ImGui::Text("Beam size");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##beam-size", &g_Options.esp.effects.beam_size, 0, 10, "%.f");
        ImGuiEx::ColorEdit3("##grenade-prediction-color", esp::colors::grenade_prediction); ImGui::SameLine(); ImGui::Checkbox("Grenade prediction ##effects", &g_Options.esp.effects.grenade_prediction);
        ImGuiEx::ColorEdit3("##arrows-color", esp::colors::out_of_view_arrows); ImGui::SameLine(); ImGui::Checkbox("Out of view arrows ##effects", &g_Options.esp.effects.outofviewarrows);
        ImGuiEx::ColorEdit3("##manual-aa-color", esp::colors::manual_aa); ImGui::SameLine(); ImGui::Checkbox("Manual aa indicator ##effects", &g_Options.esp.effects.manual_aa);
        ImGuiEx::ColorEdit3("##impacts-color",esp::colors::impacts); ImGui::SameLine(); ImGui::Checkbox("Impacts ##effects", &g_Options.esp.effects.impacts);
        ImGuiEx::ColorEdit3("##shot-hitbox-color", esp::colors::shot_hitboxes); ImGui::SameLine(); ImGui::Checkbox("Draw shot hitboxes ##effects", &g_Options.esp.effects.shot_hitboxes);
        ImGui::Checkbox("Autowall crosshair ##effects", &g_Options.esp.effects.autowall_crosshair);
        break;
    case 2:
        ImGuiEx::ColorEdit3("##local-glow-color", esp::colors::local_glow);  ImGui::SameLine();  ImGui::Checkbox("Local glow ##effects", &g_Options.esp.effects.local_glow);
        ImGuiEx::ColorEdit3("##enemy-glow-color", esp::colors::enemy_glow);  ImGui::SameLine();  ImGui::Checkbox("Enemy glow ##effects", &g_Options.esp.effects.enemy_glow);
        ImGuiEx::ColorEdit3("##team-glow-color", esp::colors::team_glow);  ImGui::SameLine();  ImGui::Checkbox("Team glow ##effects", &g_Options.esp.effects.team_glow);
        ImGuiEx::ColorEdit3("##world-glow-col", esp::colors::world_glow); ImGui::SameLine();  ImGui::Text("World glow");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        if (ImGui::BeginCombo("##world", g_Options.esp.effects.world_glow.c_str()))
        {
            g_Options.esp.effects.world_glow = "";
            const char* world_char[] = { "Weapons","Grenades","C4" };
            std::vector<std::string> vec;
            for (size_t i = 0; i < IM_ARRAYSIZE(world_char); i++)
            {
                ImGui::Selectable(world_char[i], &g_Options.esp.effects.world_selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
                if (g_Options.esp.effects.world_selected[i])
                    vec.push_back(world_char[i]);
            }
            for (size_t i = 0; i < vec.size(); i++)
            {
                if (vec.size() == 1)
                    g_Options.esp.effects.world_glow += vec.at(i);
                else if (!(i == vec.size()))
                    g_Options.esp.effects.world_glow += vec.at(i) + ",";
                else
                    g_Options.esp.effects.world_glow += vec.at(i);
            }
            ImGui::EndCombo();
        }
        ImGuiEx::ColorEdit3("##dropped-weapon-box-color", esp::colors::dropped_weapon_box);  ImGui::SameLine();  ImGui::Checkbox("Dropped weapon box ##effects", &g_Options.esp.effects.dropped_weapon_box);
        ImGuiEx::ColorEdit3("##dropper-weapon-ammo-color", esp::colors::dropped_weapon_ammo);  ImGui::SameLine(); ImGui::Checkbox("Dropped weapon ammo ##effects", &g_Options.esp.effects.dropped_weapon_ammo);
        ImGuiEx::ColorEdit3("##drooped-weapon-ammor-bar-glow-color", esp::colors::dropped_weapon_ammo_bar);  ImGui::SameLine();  ImGui::Checkbox("Dropped weapon ammo bar ##effects", &g_Options.esp.effects.dropped_weapon_ammo_bar);
        ImGui::Text("Dropped weapon name");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##dropped-weapon-name", &g_Options.esp.effects.dropped_weapon_name_type, "Off\0\rName\0\rIcon\0\0", IM_ARRAYSIZE("Off\0\rName\0\rIcon\0\0")); ImGui::PopItemWidth();
        ImGui::Checkbox("Grenade box ##effects", &g_Options.esp.effects.grenade_box);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SameLine(); ImGui::PushItemWidth(130); ImGui::Combo("##dropped-granede-box", &g_Options.esp.effects.grenade_box_type, "Off\0\rName\0\rIcon\0\0", IM_ARRAYSIZE("Off\0\rName\0\rIcon\0\0")); ImGui::PopItemWidth();
        ImGui::Checkbox("Chicken box ##effects", &g_Options.esp.effects.chicken_Box);
        break;
    }
    ImGui::EndChild();


    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}
