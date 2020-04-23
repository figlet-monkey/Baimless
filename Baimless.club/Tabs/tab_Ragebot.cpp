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

void Menu::tab_Ragebot()
{
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15, 0.12, 0.21, 10));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.08, 0.07, 0.11, 1.0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.11, 0.09, 0.16, 1.0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.0f);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    char* rage[] = { "Aimbot","Anti aim" };
    RENDER_TAB_SAMELINE(rage, g_Options.rage.rage_tab, 300, 30);
    ImGui::PopStyleVar();
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    switch (g_Options.rage.rage_tab)
    {
    case 0:
        tab_Aimbot();
        break;
    case 1:
        tab_AntiAim();
        break;
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    
}
