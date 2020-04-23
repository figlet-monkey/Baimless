#include "../Menu.h"
#include "../imgui/impl/imgui_impl_dx9.h"
#include "../imgui/impl/imgui_impl_win32.h"
#include "../Options.hpp"
#include "../http.h"
#include "../SDK/CGlobalVarsBase.h"
#include "../ui.h"
#include "../Render.h"

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

void Menu::tab_AntiAim()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 2));
    ImGui::Columns(2, NULL, false);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 13);
    ImGui::BeginChild("##aa-1", ImVec2(290, 580), true);
    ImGui::PushFont(bold);
    ImGui::SameLine((290 / 2) - (60 / 2));
    ImGui::Text("Anti-aim");
    ImGui::PopFont();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    static char* aa_tabs[] = { "Stand","Moving","Air","Slow" };
    RENDER_TAB_SAMELINE(aa_tabs, g_Options.antiaa.aa_tab_current, 280 / 4, 20);
    ImGui::PopStyleVar();
    switch (g_Options.antiaa.aa_tab_current)
    {
    case 0:
        ImGui::Checkbox("Enable ##aa", &g_Options.antiaa.enable);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Pitch");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##pitch-aa-stand", &g_Options.antiaa.stand.pitch_selected, "Off\0\rDown\0\rUp\0\rZero\0\0", IM_ARRAYSIZE("Off\0\rDown\0\rUp\0\rZero\0\0"));

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Yaw base ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##yaw-base-aa-stand", &g_Options.antiaa.stand.yaw_base_selected, "Static\0\rCrosshair\0\rAt targets\0\0", IM_ARRAYSIZE("Static\0\rCrosshair\0\rAt targets\0\0"));

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Yaw ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##yaw-aa-stand", &g_Options.antiaa.stand.yaw_selected, "Off\0\rSidewyas\0\rBackwards\0\rSpin\0\rJitter\0\0", IM_ARRAYSIZE("Off\0\rSidewyas\0\rBackwards\0\rSpin\0\rJitter\0\0"));

        ImGui::Text("Offset ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##offset-stand", &g_Options.antiaa.stand.offset, -180, 180, "%.f%°");

        ImGui::Text("Jitter range ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##jitter-stand", &g_Options.antiaa.stand.jitter, 0, 360, "%.f%°");

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Body yaw ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##body-yaw-aa-stand", &g_Options.antiaa.stand.body_yaw_selected, "Off\0\rStatic\0\rJitter\0\rTank\0\0", IM_ARRAYSIZE("Off\0\rStatic\0\rJitter\0\rTank\0\0"));
        ImGui::Hotkey("##bd-yaw-aa-key-stand", &g_Options.antiaa.stand.body_yaw_key);
        ImGui::Text("Desync limit ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##desync-limit-stand", &g_Options.antiaa.stand.desync, 0, 60, "%.f%°");

        ImGui::Text("Body lean ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##bd-lean-stand", &g_Options.antiaa.stand.body_lean, -180, 180, "%.f%°");

        ImGui::Text("Inverted body lean ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##inverted-bd-lean-stand", &g_Options.antiaa.stand.inverted_body_lean, -180, 180, "%.f%°");

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Edge type");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##edge-type-aa-stand", &g_Options.antiaa.stand.edge_type_selected, "Off\0\rManual\0\rFreestanding\0\0", IM_ARRAYSIZE("Off\0\rManual\0\rFreestanding\0\0"));
        break;
    case 1:
        ImGui::Checkbox("Enable ##aa", &g_Options.antiaa.enable);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Pitch");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##pitch-aa-moving", &g_Options.antiaa.moving.pitch_selected, "Off\0\rDown\0\rUp\0\rZero\0\0", IM_ARRAYSIZE("Off\0\rDown\0\rUp\0\rZero\0\0"));

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Yaw base ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##yaw-base-aa-moving", &g_Options.antiaa.moving.yaw_base_selected, "Static\0\rCrosshair\0\rAt targets\0\0", IM_ARRAYSIZE("Static\0\rCrosshair\0\rAt targets\0\0"));

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Yaw ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##yaw-aa-moving", &g_Options.antiaa.moving.yaw_selected, "Off\0\rSidewyas\0\rBackwards\0\rSpin\0\rJitter\0\0", IM_ARRAYSIZE("Off\0\rSidewyas\0\rBackwards\0\rSpin\0\rJitter\0\0"));

        ImGui::Text("Offset ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##offset-moving", &g_Options.antiaa.moving.offset, -180, 180, "%.f%°");

        ImGui::Text("Jitter range ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##jitter-moving", &g_Options.antiaa.moving.jitter, 0, 360, "%.f%°");

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Body yaw ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##body-yaw-aa-moving", &g_Options.antiaa.moving.body_yaw_selected, "Off\0\rStatic\0\rJitter\0\rTank\0\0", IM_ARRAYSIZE("Off\0\rStatic\0\rJitter\0\rTank\0\0"));
        ImGui::Hotkey("##bd-yaw-aa-key-moving", &g_Options.antiaa.moving.body_yaw_key);
        ImGui::Text("Desync limit ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##desync-limit-moving", &g_Options.antiaa.moving.desync, 0, 60, "%.f%°");

        ImGui::Text("Body lean ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##bd-lean-moving", &g_Options.antiaa.moving.body_lean, -180, 180, "%.f%°");

        ImGui::Text("Inverted body lean ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##inverted-bd-lean-moving", &g_Options.antiaa.moving.inverted_body_lean, -180, 180, "%.f%°");

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Edge type");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##edge-type-aa-moving", &g_Options.antiaa.moving.edge_type_selected, "Off\0\rManual\0\rFreestanding\0\0", IM_ARRAYSIZE("Off\0\rManual\0\rFreestanding\0\0"));
        break;
    case 2:
        ImGui::Checkbox("Enable ##aa", &g_Options.antiaa.enable);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Pitch");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##pitch-aa-air", &g_Options.antiaa.air.pitch_selected, "Off\0\rDown\0\rUp\0\rZero\0\0", IM_ARRAYSIZE("Off\0\rDown\0\rUp\0\rZero\0\0"));

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Yaw base ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##yaw-base-aa-air", &g_Options.antiaa.air.yaw_base_selected, "Static\0\rCrosshair\0\rAt targets\0\0", IM_ARRAYSIZE("Static\0\rCrosshair\0\rAt targets\0\0"));

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Yaw ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##yaw-aa-air", &g_Options.antiaa.air.yaw_selected, "Off\0\rSidewyas\0\rBackwards\0\rSpin\0\rJitter\0\0", IM_ARRAYSIZE("Off\0\rSidewyas\0\rBackwards\0\rSpin\0\rJitter\0\0"));

        ImGui::Text("Offset ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##offset-air", &g_Options.antiaa.air.offset, -180, 180, "%.f%°");

        ImGui::Text("Jitter range ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##jitter-air", &g_Options.antiaa.air.jitter, 0, 360, "%.f%°");

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Body yaw ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##body-yaw-aa-air", &g_Options.antiaa.air.body_yaw_selected, "Off\0\rStatic\0\rJitter\0\rTank\0\0", IM_ARRAYSIZE("Off\0\rStatic\0\rJitter\0\rTank\0\0"));
        ImGui::Hotkey("##bd-yaw-aa-key-air", &g_Options.antiaa.air.body_yaw_key);
        ImGui::Text("Desync limit ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##desync-limit-air", &g_Options.antiaa.air.desync, 0, 60, "%.f%°");

        ImGui::Text("Body lean ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##bd-lean-air", &g_Options.antiaa.air.body_lean, -180, 180, "%.f%°");

        ImGui::Text("Inverted body lean ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##inverted-bd-lean-air", &g_Options.antiaa.air.inverted_body_lean, -180, 180, "%.f%°");

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Edge type");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##edge-type-aa-air", &g_Options.antiaa.air.edge_type_selected, "Off\0\rManual\0\rFreestanding\0\0", IM_ARRAYSIZE("Off\0\rManual\0\rFreestanding\0\0"));
        break;
    case 3:
        ImGui::Checkbox("Enable ##aa", &g_Options.antiaa.enable);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Pitch");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##pitch-aa-slow", &g_Options.antiaa.slow.pitch_selected, "Off\0\rDown\0\rUp\0\rZero\0\0", IM_ARRAYSIZE("Off\0\rDown\0\rUp\0\rZero\0\0"));

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Yaw base ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##yaw-base-aa-slow", &g_Options.antiaa.slow.yaw_base_selected, "Static\0\rCrosshair\0\rAt targets\0\0", IM_ARRAYSIZE("Static\0\rCrosshair\0\rAt targets\0\0"));

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Yaw ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##yaw-aa-slow", &g_Options.antiaa.slow.yaw_selected, "Off\0\rSidewyas\0\rBackwards\0\rSpin\0\rJitter\0\0", IM_ARRAYSIZE("Off\0\rSidewyas\0\rBackwards\0\rSpin\0\rJitter\0\0"));

        ImGui::Text("Offset ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##offset-slow", &g_Options.antiaa.slow.offset, -180, 180, "%.f%°");

        ImGui::Text("Jitter range ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##jitter-slow", &g_Options.antiaa.slow.jitter, 0, 360, "%.f%°");

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Body yaw ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##body-yaw-aa-slow", &g_Options.antiaa.slow.body_yaw_selected, "Off\0\rStatic\0\rJitter\0\rTank\0\0", IM_ARRAYSIZE("Off\0\rStatic\0\rJitter\0\rTank\0\0"));
        ImGui::Hotkey("##bd-yaw-aa-key-slow", &g_Options.antiaa.slow.body_yaw_key);
        ImGui::Text("Desync limit ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##desync-limit-slow", &g_Options.antiaa.slow.desync, 0, 60, "%.f%°");

        ImGui::Text("Body lean ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##bd-lean-slow", &g_Options.antiaa.slow.body_lean, -180, 180, "%.f%°");

        ImGui::Text("Inverted body lean ");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SliderFloat("##inverted-bd-lean-slow", &g_Options.antiaa.slow.inverted_body_lean, -180, 180, "%.f%°");

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Text("Edge type");
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::Combo("##edge-type-aa-slow", &g_Options.antiaa.slow.edge_type_selected, "Off\0\rManual\0\rFreestanding\0\0", IM_ARRAYSIZE("Off\0\rManual\0\rFreestanding\0\0"));
        break;
    default:
        break;
    }
    ImGui::EndChild();
    ImGui::NextColumn();
    ImGui::BeginChild("##aa-2", ImVec2(290, 300), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::PushFont(bold);
    ImGui::SameLine((290 / 2) - (37 / 2));
    ImGui::Text("Main");
    ImGui::PopFont();
    ImGui::Checkbox("Enable aa ##aa", &g_Options.antiaa.enable);
    ImGui::Checkbox("Fake lag ##aa-2", &g_Options.antiaa.fakelag);
    ImGui::Text("Standing lag");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##fake-lag-standing", &g_Options.antiaa.fake_lag_standing, 0, 14, "%.f");
    ImGui::Text("Moving lag");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##fake-lag-moving", &g_Options.antiaa.fake_lag_moving, 0, 14,"%.f");
    ImGui::Text("Air lag");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##fake-lag-air", &g_Options.antiaa.fake_lag_air, 0, 14, "%.f");

    ImGui::Checkbox("Air stuck ##aa-2", &g_Options.antiaa.air_stuck); ImGui::SameLine(); ImGui::Hotkey("##air-stuck", &g_Options.antiaa.air_stuck_key);

    ImGui::Checkbox("Double tap ##aa-2", &g_Options.antiaa.double_tap);
    ImGui::Checkbox("Double on key", &g_Options.antiaa.double_tap_on_key); ImGui::SameLine(); ImGui::Hotkey("##double-key", &g_Options.antiaa.double_tap_key);
    ImGui::Text("Double key method");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Combo("##double-key-method", &g_Options.antiaa.double_tap_method, "Set\0\rHold\0\0", IM_ARRAYSIZE("Set\0\rSet\0\0"));
    ImGui::Text("Delay shot");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##delay-shot", &g_Options.antiaa.double_tap_shot, 0, 30,"%.f");
    ImGui::Text("State delay");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##state-delay-shot", &g_Options.antiaa.double_tap_delay, 0, 20);
    ImGui::EndChild();
    /**************************/
    ImGui::BeginChild("##aa-3", ImVec2(290, 275), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::PushFont(bold);
    ImGui::SameLine((290 / 2) - (37 / 2));
    ImGui::Text("Other");
    ImGui::PopFont();
    ImGui::Text("Disable AA");
    if (ImGui::BeginCombo("##Disable_aa", g_Options.antiaa.disable_aa.c_str()))
    {
        const char* disable_aa_char[] = { "Grenade","Round start/end" };
        g_Options.antiaa.disable_aa = "";
        std::vector<std::string> vec;
        for (size_t i = 0; i < IM_ARRAYSIZE(disable_aa_char); i++)
        {
            ImGui::Selectable(disable_aa_char[i], &g_Options.antiaa.disable_aa_selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
            if (g_Options.antiaa.disable_aa_selected[i])
                vec.push_back(disable_aa_char[i]);
        }
        for (size_t i = 0; i < vec.size(); i++)
        {
            if (vec.size() == 1)
                g_Options.antiaa.disable_aa += vec.at(i);
            else if (!(i == vec.size()))
                g_Options.antiaa.disable_aa += vec.at(i) + ",";
            else
                g_Options.antiaa.disable_aa += vec.at(i);
        }
        ImGui::EndCombo();
    }
    ImGui::Checkbox("Slow walk ##aa", &g_Options.antiaa.slow_walk); ImGui::SameLine(); ImGui::Hotkey("##slow-walk", &g_Options.antiaa.slow_walk_key);
    ImGui::Text("Slow speed");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);

    ImGui::SliderFloat("##slow-speed", &g_Options.antiaa.slow_speed, 0, 30, "%.f%°");

    ImGui::Text("Invert key"); ImGui::SameLine(); ImGui::Hotkey("##invert-key", &g_Options.antiaa.invert_key);
    ImGui::Checkbox("Static lby ##aa", &g_Options.antiaa.static_lby);
    ImGui::Checkbox("Lby breaked ##aa", &g_Options.antiaa.lby_break);
    ImGui::Text("Lby add");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##lby-add", &g_Options.antiaa.lby_break_add, 0, 180);
    ImGui::Checkbox("Infinite duck ##aa", &g_Options.antiaa.infinite_duck);
    ImGui::Checkbox("Fake duck ##aa", &g_Options.antiaa.fake_duck); ImGui::SameLine(); ImGui::Hotkey("##fake-duck", &g_Options.antiaa.fake_duck_key);
    ImGui::Checkbox("Fix leg movement ##aa", &g_Options.antiaa.fix_leg_mov);
    ImGui::NewLine();
    ImGui::EndChild();
    ImGui::PopStyleVar();
}
