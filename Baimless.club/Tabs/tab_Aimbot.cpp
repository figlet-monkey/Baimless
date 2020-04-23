#include "../Menu.h"
#include "../imgui/impl/imgui_impl_dx9.h"
#include "../imgui/impl/imgui_impl_win32.h"
#include "../Options.hpp"
#include "../http.h"
#include "../SDK/CGlobalVarsBase.h"
#include "../ui.h"
#include "../Render.h"
#define loop(n) for(int ii = 0; ii < n; ++ ii)
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
void Menu::tab_Aimbot()
{
    loop(1)
        ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15, 0.12, 0.21, 10));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.08, 0.07, 0.11, 1.0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.11, 0.09, 0.16, 1.0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 2));
    ImGui::Columns(2, NULL, false);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 13);
    ImGui::BeginChild("##rage-1", ImVec2(290, 580), true);
    ImGui::PushFont(bold);
    ImGui::SameLine((290 / 2) - (37 / 2));
    ImGui::Text("Main");
    ImGui::PopFont();
    ImGui::Checkbox("Enable ##rage", &g_Options.rage.enable);
    ImGui::Checkbox("Auto fire ##rage", &g_Options.rage.autofire); ImGui::SameLine(); HelpMarker("Auto fire in enemy when possible");
    ImGui::Checkbox("Silent aim ##rage", &g_Options.rage.silentaim);
    ImGui::Checkbox("Anti recoil ##rage", &g_Options.rage.norecoil);
    ImGui::Checkbox("Remove spread ##rage", &g_Options.rage.removespread);
    ImGui::Checkbox("Auto revolver ##rage", &g_Options.rage.autorevolver);
    ImGui::Checkbox("Auto zeus ##rage", &g_Options.rage.autozeus);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Text("Zeus hitchance");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##zeus-hitchance", &g_Options.rage.zeus_hitchance, 00, 100, "%.f%%");
    ImGui::Checkbox("Auto stop ##rage", &g_Options.rage.autostop);
    ImGui::Checkbox("Shot backtrack ##rage", &g_Options.rage.shotbacktrack);
    ImGui::Checkbox("Backtrack ##rage", &g_Options.rage.backtrack);
    ImGui::Checkbox("Auto scope ##rage", &g_Options.rage.autoscope);
    ImGui::Checkbox("Resolver ##rage", &g_Options.rage.resolver);
    ImGui::Checkbox("Prediction ##rage", &g_Options.rage.prediction);
    ImGui::Checkbox("Extrapolation ##rage", &g_Options.rage.extrapolation);
    ImGui::Checkbox("Fake lag fix ##rage", &g_Options.rage.fakelagfix);
    ImGuiEx::ColorEdit3("##aimpoint-color", legit::colors::aimpoint_col); ImGui::SameLine(); ImGui::Checkbox("Show aimpoint##rage", &g_Options.rage.show_aimpoint);

    ImGui::EndChild();
    /**********************************************************/
    ImGui::NextColumn();
    ImGui::BeginChild("##rage-2", ImVec2(290, 580), true);
    ImGui::PushFont(bold);
    ImGui::SameLine((290 / 2) - (51 / 2));
    ImGui::Text("Config");
    ImGui::PopFont();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    //ImGui::Text("Stop options");
    //ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    //ImGui::Combo("##stop-options", &g_Options.rage.auto_stop_select, "1\0\r2\0\r3\0\0", IM_ARRAYSIZE("1\0\r2\0\r3\0\0"));
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Text("Hitscan");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    if (ImGui::BeginCombo("##Hitscan", g_Options.rage.hitscan_str.c_str()))
    {
        const char* hitscans[] = { "Head","Neck","Chest","Stomach","Arms","Legs" };
        g_Options.rage.hitscan_str = "";
        std::vector<std::string> vec;
        for (size_t i = 0; i < IM_ARRAYSIZE(hitscans); i++)
        {
            ImGui::Selectable(hitscans[i], &g_Options.rage.hitscan_selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
            if (g_Options.rage.hitscan_selected[i])
                vec.push_back(hitscans[i]);
        }
        for (size_t i = 0; i < vec.size(); i++)
        {
            if (vec.size() == 1)
                g_Options.rage.hitscan_str += vec.at(i);
            else if (!(i == vec.size()))
                g_Options.rage.hitscan_str += vec.at(i) + ",";
            else
                g_Options.rage.hitscan_str += vec.at(i);
        }
        ImGui::EndCombo();
    }
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Text("Minimum hitchance");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##min-hitchance", &g_Options.rage.min_hitchance, 0, 100, "%.f%%");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Text("Minimum autowall damage");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##auto-wall-hitchance", &g_Options.rage.autowall_damage_hitchance, 0, 100, "%.f%%");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Text("Avoid head if hp lower");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##avoid-head-hitchance", &g_Options.rage.avoid_head, 0, 100, "%.f%%");
    ImGui::Text("Avoid head if");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    if (ImGui::BeginCombo("##Avoid Head", g_Options.rage.avoid_head_str.c_str()))
    {
        const char* avoid_head_char[] = { "Lethal","In air","Up pitch","On key","Missed x shots","High speed","Vulnerable" };
        g_Options.rage.avoid_head_str = "";
        std::vector<std::string> vec;
        for (size_t i = 0; i < IM_ARRAYSIZE(avoid_head_char); i++)
        {
            ImGui::Selectable(avoid_head_char[i], &g_Options.rage.avoid_head_selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
            if (g_Options.rage.avoid_head_selected[i])
                vec.push_back(avoid_head_char[i]);
        }
        for (size_t i = 0; i < vec.size(); i++)
        {
            if (vec.size() == 1)
                g_Options.rage.avoid_head_str += vec.at(i);
            else if (!(i == vec.size()))
                g_Options.rage.avoid_head_str += vec.at(i) + ",";
            else
                g_Options.rage.avoid_head_str += vec.at(i);
        }
        ImGui::EndCombo();
    }
    ImGui::Text("Baim key");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Hotkey("##baim-key", &g_Options.rage.baimkey);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Text("Avoid head after x missed shoots");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##avoid-head-x-hitchance", &g_Options.rage.avoid_head_x, 0, 3, "%.f%º");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Text("Head scale");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##head-scale-hitchance", &g_Options.rage.head_scale, 0, 100, "%.f%%");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Text("Body scale");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##body-scale-hitchance", &g_Options.rage.body_scale, 0, 100, "%.f%%");
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
