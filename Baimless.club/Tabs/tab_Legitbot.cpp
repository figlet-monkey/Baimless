#include "../Menu.h"
#include "../imgui/impl/imgui_impl_dx9.h"
#include "../imgui/impl/imgui_impl_win32.h"
#include "../Options.hpp"
#include "../http.h"
#include "../SDK/CGlobalVarsBase.h"
#include "../ui.h"
#include "../Render.h"

#define loop(n) for(int ii = 0; ii < n; ++ ii)
template<size_t N>
void RENDER_TAB_SAMELINE(char* (&names)[N], int& activetab, float w, float h)
{
    bool values[N] = { false };
    values[activetab] = true;
    for (auto i = 0; i < N; ++i) {
        if (ImGui::ToggleButton(names[i], &values[i],6, ImVec2{ w, h }))
            activetab = i;
        ImGui::SameLine();
    }
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

        if (ImGui::ColorEdit4(label, &clr.x, show_alpha)) {
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
        return ImGui::ColorEdit3(label, v, ImGuiColorEditFlags_NoInputs);
    }
}


std::vector<std::pair<int, const char*>> legitbot_guns =
{
{ (int)ItemDefinitionIndex::WEAPON_AK47,			"AK-47" },
{ (int)ItemDefinitionIndex::WEAPON_AUG,			"AUG" },
{ (int)ItemDefinitionIndex::WEAPON_AWP,			"AWP" },
{ (int)ItemDefinitionIndex::WEAPON_CZ75,			"CZ75-A" },
{ (int)ItemDefinitionIndex::WEAPON_DEAGLE,		"Desert Eagle" },
{ (int)ItemDefinitionIndex::WEAPON_ELITE,			"Dual Barettas" },
{ (int)ItemDefinitionIndex::WEAPON_FAMAS,			"Famas" },
{ (int)ItemDefinitionIndex::WEAPON_FIVESEVEN, 	"Five-Seven" },
{ (int)ItemDefinitionIndex::WEAPON_GALIL,		"Galil-AR" },
{ (int)ItemDefinitionIndex::WEAPON_G3SG1,			"G3SG1" },
{ (int)ItemDefinitionIndex::WEAPON_GLOCK,			"Glock-18" },
{ (int)ItemDefinitionIndex::WEAPON_M249,			"M249" },
{ (int)ItemDefinitionIndex::WEAPON_M4A1S, "M4A1-S" },
{ (int)ItemDefinitionIndex::WEAPON_M4A4,			"M4A4" },
{ (int)ItemDefinitionIndex::WEAPON_MAC10,			"MAC-10" },
{ (int)ItemDefinitionIndex::WEAPON_MAG7,			"Mag-7" },
{ (int)ItemDefinitionIndex::WEAPON_MP7,			"MP7" },
{ (int)ItemDefinitionIndex::WEAPON_MP5SD,		"MP5-SD"},
{ (int)ItemDefinitionIndex::WEAPON_MP9,			"MP9" },
{ (int)ItemDefinitionIndex::WEAPON_NEGEV,			"Negev" },
{ (int)ItemDefinitionIndex::WEAPON_NOVA,			"Nova" },
{ (int)ItemDefinitionIndex::WEAPON_P2000,		"P2000" },
{ (int)ItemDefinitionIndex::WEAPON_P250,			"P250" },
{ (int)ItemDefinitionIndex::WEAPON_P90,			"P90" },
{ (int)ItemDefinitionIndex::WEAPON_BIZON,			"PP-Bizon" },
{ (int)ItemDefinitionIndex::WEAPON_REVOLVER,		"R8 Revolver" },
{ (int)ItemDefinitionIndex::WEAPON_SAWEDOFF,		"Sawed-Off" },
{ (int)ItemDefinitionIndex::WEAPON_SCAR20,		"SCAR-20" },
{ (int)ItemDefinitionIndex::WEAPON_SG553,			"SG-556" },
{ (int)ItemDefinitionIndex::WEAPON_SSG08,			"Scout" },
{ (int)ItemDefinitionIndex::WEAPON_TEC9,			"Tec-9" },
{ (int)ItemDefinitionIndex::WEAPON_UMP45,			"UMP-45" },
{ (int)ItemDefinitionIndex::WEAPON_USPS,	"USP-s" },
{ (int)ItemDefinitionIndex::WEAPON_XM1014, "XM1014" },
};

void Hitscan_simple()
{
    /*ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    if (ImGui::ImageButton((void*)head, ImVec2(150, 40), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), g_Options.legit.hitscan.head_color))
        g_Options.legit.hitscan.hitscan[1] = !g_Options.legit.hitscan.hitscan[1]
    if (ImGui::ImageButton((void*)body, ImVec2(150, 77), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), g_Options.legit.hitscan.body_color))
        g_Options.legit.hitscan.hitscan[2] = !g_Options.legit.hitscan.hitscan[2]
    if (ImGui::ImageButton((void*)pelvis, ImVec2(150, 34), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), pelvis_color))
        g_Options.simple_pelvis = !g_Options.simple_pelvis;
    if (ImGui::ImageButton((void*)legs, ImVec2(150, 61), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), legs_color))
        g_Options.simple_legs = !g_Options.simple_legs;
    if (ImGui::ImageButton((void*)foot, ImVec2(150, 34), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), foots_color))
        g_Options.simple_foot = !g_Options.simple_foot;
    ImGui::PopStyleVar(); ImGui::PopStyleVar();
    ImGui::PopStyleColor(); ImGui::PopStyleColor(); ImGui::PopStyleColor();
    if (g_Options.simple_head)
        head_color = ImVec4(255, 255, 255, 255);
    else
        head_color = ImVec4(0, 0, 0, 1);

    if (g_Options.simple_chest)
        body_color = ImVec4(255, 255, 255, 255);
    else
        body_color = ImVec4(0, 0, 0, 1);

    if (g_Options.simple_pelvis)
        pelvis_color = ImVec4(255, 255, 255, 255);
    else
        pelvis_color = ImVec4(0, 0, 0, 1);

    if (g_Options.simple_legs)
        legs_color = ImVec4(255, 255, 255, 255);
    else
        legs_color = ImVec4(0, 0, 0, 1);

    if (g_Options.simple_foot)
        foots_color = ImVec4(255, 255, 255, 255);
    else
        foots_color = ImVec4(0, 0, 0, 1);
        */

}
//const char* currentweapon;

void Menu::tab_Legitbot()
{
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15, 0.12, 0.21, 10));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.08, 0.07, 0.11, 1.0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.11, 0.09, 0.16, 1.0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 2));
    loop(3)ImGui::Spacing();
    /**/
    ImGui::Columns(2, NULL, false);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::BeginChild("##legit-1", ImVec2(290, 300), true);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 3);
    static char* legit_char[] = { "Main" };
    RENDER_TAB_SAMELINE(legit_char, g_Options.legit.legit_main, 290, 30);
    ImGui::NewLine(); ImGui::NewLine();
    ImGui::Checkbox("Enable ##legit", &g_Options.legit.enable);
    ImGui::Checkbox("Smoke check##legit", &g_Options.legit.smoke_check);
    ImGui::Checkbox("Flash check##misc", &g_Options.legit.flash_check);
    ImGui::Checkbox("Jump check##misc", &g_Options.legit.jump_check);
    ImGui::Checkbox("Silent Aim##misc", &g_Options.legit.silentaim);
    if (g_Options.legit.legit_type == 1 && g_Options.current_legit_wep == (int)ItemDefinitionIndex::WEAPON_AWP || g_Options.legit.legit_type == 1 && g_Options.current_legit_wep == (int)ItemDefinitionIndex::WEAPON_SSG08 || g_Options.legit.legit_type == 0 && g_Options.legit.guns_int == 2)
        ImGui::Checkbox("Air shot##misc", &g_Options.legit.air_shot);
    ImGui::Checkbox("Aimbot key ##legit", &g_Options.legit.on_key); ImGui::SameLine(); ImGui::Hotkey("##legit", &g_Options.legit.key, ImVec2(80, 20));
    ImGui::Text("Weapon config");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Combo("##type", &g_Options.legit.legit_type, "Simple\0\rAdvanced\0\0", IM_ARRAYSIZE("Simple\0\rAdvanced\0\0"));
    if (g_Options.legit.legit_type == 1)
    {
        ImGui::BeginChild(7, ImVec2(-1, -1), true);
        for (auto it : legitbot_guns)
        {
            if (strcmp(it.second, "<-Default->") == 0)
                continue;

            const bool item_selected = ((int)it.first == (int)g_Options.current_legit_wep);

            if (ImGui::Selectable(it.second, item_selected))
            {
                g_Options.current_legit_wep = (int)it.first;
                //currentweapon = it.second;
            }
        }
        ImGui::EndChild();
    }
    ImGui::EndChild();
    ImGui::NextColumn();
    /***********/
    ImGui::BeginChild("##legit-2", ImVec2(290, 300), true,ImGuiWindowFlags_NoScrollbar);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 1);
    static char* legit_extra[] = { "Main","Triggerbot" };
    RENDER_TAB_SAMELINE(legit_extra, g_Options.legit.legit_extra, 270 / 2, 30);
    ImGui::NewLine(); ImGui::NewLine();
    switch (g_Options.legit.legit_extra)
    {
    case 0:
        ImGui::Checkbox("Backtrack ##legit", &g_Options.legit.backtrack);
        if (!g_Options.legit.silentaim) {
            ImGui::Checkbox("Standalone RCS ##legit", &g_Options.legit.rcs_standalone);
            ImGui::Text("Amount");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##legit_rcs", &g_Options.legit.rcs_amount, 0, 4, "%.f%%");
        }
        else
        {
            g_Options.legit.rcs_standalone = false;
        }
        ImGui::Checkbox("Recoil crosshair##legit", &g_Options.legit.recoil_crosshair);
        ImGuiEx::ColorEdit3("##legit-aa-color-visualize-fov", legit::colors::legit_aa_visualize_fov_color); ImGui::SameLine(); ImGui::Checkbox("Visualize fov ##legit", &g_Options.legit.visualize_fov);
        ImGui::Checkbox("Legit aa ##legit", &g_Options.legit.legit_aa); //ImGui::SameLine(); ImGui::Hotkey("##legit-aa-key", &g_Options.legit.legit_aa_key);
        ImGui::SliderFloat("Legit AA Offset##legit",    &g_Options.legit.LegitAA_Offset, -180, 180, "%.2f", 1.0F);

        ImGui::Checkbox("Legit aa indicator ##legit", &g_Options.legit.legit_aa_indicator);
        ImGuiEx::ColorEdit3("##legit-aa-color-visualize", legit::colors::legit_aa_visualize_color); ImGui::SameLine(); ImGui::Checkbox("Legit aa visualize ##legit", &g_Options.legit.legit_aa_visualize);
        ImGui::Text("Material");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##material", &g_Options.legit.material, "Normal\0\rReflective\0\rBright reflective\0\rFlat\0\rDark\0\rPulse\0\0", IM_ARRAYSIZE("Normal\0\rReflective\0\rBright reflective\0\rFlat\0\rDark\0\rPulse\0\0"));
        break;
    case 1:
        ImGui::Checkbox("Enable ##trigger", &g_Options.legit.trigger.enable);
        ImGui::Checkbox("Key ##trigger", &g_Options.legit.trigger.on_key); ImGui::SameLine(); ImGui::Hotkey("##trigger-key", &g_Options.legit.trigger.key);
        ImGui::Text("Trigger method");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##trigger-method", &g_Options.legit.trigger.method, "Set\0\rHold\0\0", IM_ARRAYSIZE("Set\0\rHold\0\0"));
        ImGui::Checkbox("Deathmatch ##legit", &g_Options.legit.trigger.deathmatch);
        ImGui::Checkbox("Smoke check ##legit", &g_Options.legit.trigger.smoke_check);

        ImGui::Text("Hitscan");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        if (ImGui::BeginCombo("##Hitscan", g_Options.legit.trigger.hitscan.c_str()))
        {
            const char* trigger_char[] = { "Head","Chest","Stomach","Arms","Legs" };
            g_Options.legit.trigger.hitscan = "";
            std::vector<std::string> vec;
            for (size_t i = 0; i < IM_ARRAYSIZE(trigger_char); i++)
            {
                ImGui::Selectable(trigger_char[i], &g_Options.legit.trigger.trigger_hitscan_bool[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
                if (g_Options.legit.trigger.trigger_hitscan_bool[i])
                    vec.push_back(trigger_char[i]);
            }
            for (size_t i = 0; i < vec.size(); i++)
            {
                if (vec.size() == 1)
                    g_Options.legit.trigger.hitscan += vec.at(i);
                else if (!(i == vec.size()))
                    g_Options.legit.trigger.hitscan += vec.at(i) + ",";
                else
                    g_Options.legit.trigger.hitscan += vec.at(i);
            }
            ImGui::EndCombo();
        }

        ImGui::Text("Min damage");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##trigger_min_damage", &g_Options.legit.trigger.trigger_min, 0, 100, "%.f%%");
        ImGui::Text("Hitchance");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##trigger_hit_chance", &g_Options.legit.trigger.trigger_min_chance, 0, 100, "%.f%%");
        ImGui::Text("Delay");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##trigger_delay", &g_Options.legit.trigger.trigger_delay, 0, 100, "%.f%%");
        break;
    }
    ImGui::EndChild();
    ImGui::Columns(1, NULL, false);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::BeginChild("##legit-3", ImVec2(598, 305), true);
    if (g_Options.legit.legit_type == 0) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushFont(weapons);
        static char* guns[] = { "A","B","C","D","E","F" };
        RENDER_TAB_SAMELINE(guns, g_Options.legit.guns_int, 588 / 6, 30);
        ImGui::PopStyleVar();

        //ImGui::Columns(2, NULL, false);
       // static float initial_spacing = 375;
        //if (initial_spacing)
            //ImGui::SetColumnWidth(0, initial_spacing), initial_spacing = 0;
        ImGui::PopFont();
    }
    ImGui::NewLine(); ImGui::NewLine();
    if (g_Options.legit.legit_type == 0) {
        switch (g_Options.legit.guns_int)
        {
        case 0:
            ImGui::Combo("##hitbox-legit-bot-rifles", &g_Options.legit.rifles.hitscan, "Head\0\rNeck\0\rChest\0\rStomach\0\rNearest\0\0", IM_ARRAYSIZE("Head\0\rNeck\0\rChest\0\rStomach\0\rNearest\0\0"));
            ImGui::Text("Max speed");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##max-speed-legit-rifles", &g_Options.legit.rifles.speed, 0, 200, "%.f");

            ImGui::Text("Field of view (FOV)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##fov-legit-rifles", &g_Options.legit.rifles.fov, 0, 30, "%.f");

            ImGui::Text("Humanize (SMOOTH)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##smooth-legit-rifles", &g_Options.legit.rifles.smooth, 0, 100, "%.f%%");

            ImGui::Text("Min damage");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##min-legit-rifles", &g_Options.legit.rifles.min_dmg, 0, 100, "%.f%%");

            ImGui::Text("Recoil control system (RCS)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderInt("##rcs-legit-rifles", &g_Options.legit.rifles.rcs, 0, 100, "%.f%%");
            break;
        case 1:
            ImGui::Combo("##hitbox-legit-bot-pistols", &g_Options.legit.pistols.hitscan, "Head\0\rNeck\0\rChest\0\rStomach\0\rNearest\0\0", IM_ARRAYSIZE("Head\0\rNeck\0\rChest\0\rStomach\0\rNearest\0\0"));
            ImGui::Text("Max speed");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##max-speed-legit-pistols", &g_Options.legit.pistols.speed, 0, 200, "%.f");

            ImGui::Text("Field of view (FOV)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##fov-legit-pistols", &g_Options.legit.pistols.fov, 0, 30, "%.f");

            ImGui::Text("Humanize (SMOOTH)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##smooth-legit-pistols", &g_Options.legit.pistols.smooth, 0, 100, "%.f%%");

            ImGui::Text("Min damage");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##min-legit-pistols", &g_Options.legit.pistols.min_dmg, 0, 100, "%.f%%");

            ImGui::Text("Recoil control system (RCS)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderInt("##rcs-legit-pistols", &g_Options.legit.pistols.rcs, 0, 100,"%d%%");
            break;
        case 2:
            ImGui::Combo("##hitbox-legit-bot-snipers", &g_Options.legit.snipers.hitscan, "Head\0\rNeck\0\rChest\0\rStomach\0\rNearest\0\0", IM_ARRAYSIZE("Head\0\rNeck\0\rChest\0\rStomach\0\rNearest\0\0"));
            ImGui::Text("Max speed");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##max-speed-legit-snipers", &g_Options.legit.snipers.speed, 0, 200, "%.f");

            ImGui::Text("Field of view (FOV)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##fov-legit-snipers", &g_Options.legit.snipers.fov, 0, 30, "%.f");

            ImGui::Text("Humanize (SMOOTH)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##smooth-legit-snipers", &g_Options.legit.snipers.smooth, 0, 100, "%.f%%");

            ImGui::Text("Min damage");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##min-legit-snipers", &g_Options.legit.snipers.min_dmg, 0, 100, "%.f%%");

            ImGui::Text("Recoil control system (RCS)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderInt("##rcs-legit-snipers", &g_Options.legit.snipers.rcs, 0, 100, "%d%%");
            break;
        case 3:
            ImGui::Combo("##hitbox-legit-bot-sub", &g_Options.legit.sub.hitscan, "Head\0\rNeck\0\rChest\0\rStomach\0\rNearest\0\0", IM_ARRAYSIZE("Head\0\rNeck\0\rChest\0\rStomach\0\rNearest\0\0"));
            ImGui::Text("Max speed");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##max-speed-legit-sub", &g_Options.legit.sub.speed, 0, 200, "%.f");

            ImGui::Text("Field of view (FOV)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##fov-legit-sub", &g_Options.legit.sub.fov, 0, 30, "%.f");

            ImGui::Text("Humanize (SMOOTH)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##smooth-legit-sub", &g_Options.legit.sub.smooth, 0, 100, "%.f%%");

            ImGui::Text("Min damage");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##min-legit-sub", &g_Options.legit.sub.min_dmg, 0, 100, "%.f%%");

            ImGui::Text("Recoil control system (RCS)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderInt("##rcs-legit-sub", &g_Options.legit.sub.rcs, 0, 100, "%d%%");
            break;
        case 4:
            ImGui::Combo("##hitbox-legit-bot-shotguns", &g_Options.legit.shotguns.hitscan, "Head\0\rNeck\0\rChest\0\rStomach\0\rNearest\0\0", IM_ARRAYSIZE("Head\0\rNeck\0\rChest\0\rStomach\0\rNearest\0\0"));
            ImGui::Text("Max speed");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##max-speed-legit-shotguns", &g_Options.legit.shotguns.speed, 0, 200, "%.f");

            ImGui::Text("Field of view (FOV)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##fov-legit-shotguns", &g_Options.legit.shotguns.fov, 0, 30, "%.f");

            ImGui::Text("Humanize (SMOOTH)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##smooth-legit-shotguns", &g_Options.legit.shotguns.smooth, 0, 100, "%.f%%");

            ImGui::Text("Min damage");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##min-legit-shotguns", &g_Options.legit.shotguns.min_dmg, 0, 100, "%.f%%");

            ImGui::Text("Recoil control system (RCS)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderInt("##rcs-legit-shotguns", &g_Options.legit.shotguns.rcs, 0, 100, "%d%%");
            break;
        case 5:
            ImGui::Combo("##hitbox-legit-bot-heavys", &g_Options.legit.heavys.hitscan, "Head\0\rNeck\0\rChest\0\rStomach\0\rNearest\0\0", IM_ARRAYSIZE("Head\0\rNeck\0\rChest\0\rStomach\0\rNearest\0\0"));
            ImGui::Text("Max speed");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##max-speed-legit-heavys", &g_Options.legit.heavys.speed, 0, 200, "%.f");

            ImGui::Text("Field of view (FOV)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##fov-legit-heavys", &g_Options.legit.heavys.fov, 0, 30, "%.f");

            ImGui::Text("Humanize (SMOOTH)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##smooth-legit-heavys", &g_Options.legit.heavys.smooth, 0, 100, "%.f%%");

            ImGui::Text("Min damage");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderFloat("##min-legit-heavys", &g_Options.legit.heavys.min_dmg, 0, 100, "%.f%%");

            ImGui::Text("Recoil control system (RCS)");
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SliderInt("##rcs-legit-heavys", &g_Options.legit.heavys.rcs, 0, 100, "%d%%");
            break;
        }
    }
    else
    {
    //ImGui::Text(currentweapon); ImGui::SameLine(); ImGui::Text("Config");
    ImGui::Combo("##hitbox-legit-bot-rifles", &g_Options.advanced_legit[g_Options.current_legit_wep].hitscan, "Head\0\rNeck\0\rChest\0\rStomach\0\rNearest\0\0", IM_ARRAYSIZE("Head\0\rNeck\0\rChest\0\rStomach\0\rNearest\0\0"));
    ImGui::Text("Max speed");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##max-speed-legit-rifles", &g_Options.advanced_legit[g_Options.current_legit_wep].speed, 0, 200, "%.f");

    ImGui::Text("Field of view (FOV)");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##fov-legit-rifles", &g_Options.advanced_legit[g_Options.current_legit_wep].fov, 0, 30, "%.f");

    ImGui::Text("Humanize (SMOOTH)");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##smooth-legit-rifles", &g_Options.advanced_legit[g_Options.current_legit_wep].smooth, 0, 100, "%.f%%");

    ImGui::Text("Min damage");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##min-legit-rifles", &g_Options.advanced_legit[g_Options.current_legit_wep].min_dmg, 0, 100, "%.f%%");

    ImGui::Text("Recoil control system (RCS)");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderInt("##rcs-legit-rifles", &g_Options.advanced_legit[g_Options.current_legit_wep].rcs, 0, 100, "%d%%");
    }
    /*ImGui::NextColumn();
    ImGui::PushItemWidth(-1);
    ImGui::NewLine(); ImGui::NewLine();
    ImGui::Text("Hitscan");
    ImGui::BeginChild("##legit-6", ImVec2(140, 252), true);
    //Hitscan_advanced();
    ImGui::EndChild();*/
    ImGui::EndChild();
    /**/
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}
