#include "../Menu.h"
#include "../imgui/impl/imgui_impl_dx9.h"
#include "../imgui/impl/imgui_impl_win32.h"
#include "../Options.hpp"
#include "../http.h"
#include "../SDK/CGlobalVarsBase.h"
#include "../ui.h"
template<size_t N>
void RENDER_TAB(char* (&names)[N], int& activetab, float w, float h)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 255);
    bool values[N] = { false };
    values[activetab] = true;
    for (auto i = 0; i < N; ++i) {
        ImGui::NewLine(); ImGui::NewLine();
        ImGui::SameLine((190 / 2) - (w / 2));
        if (ImGui::ToggleButton(names[i], &values[i],i, ImVec2{ w, h }))
            activetab = i;
    }
    ImGui::PopStyleVar();
}
static int Tab = 0;
char* tabs[] = { "Ragebot","Legitbot","Visuals","Misc","Changers","Soon" };
void Menu::tab_Main()
{
    
    ImGui::SetNextWindowSize(ImVec2(850, 650));
    ImGui::Begin("##teste", &_visible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 850, p.y + +5), ImGui::GetColorU32(ImGuiCol_ButtonActive));
    ImGui::PopStyleVar();
    ImGui::Columns(2, NULL, false);

    static float initial_spacing = 225.f;
    if (initial_spacing)
        ImGui::SetColumnWidth(0, initial_spacing), initial_spacing = 0;

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 16);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
    ImGui::BeginChild("##1", ImVec2(190, 605), true);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
    ImGui::NewLine(); ImGui::NewLine();
    ImGui::SameLine((190 / 2) - (100 / 2));
    ImGui::Image(b2, ImVec2(100, 100));
    ImGui::NewLine(); ImGui::NewLine(); ImGui::NewLine();
    RENDER_TAB(tabs, Tab, 160, 40);

    ImGui::EndChild();

    ImGui::NextColumn();

    ImGui::PushItemWidth(623);
    ImGui::BeginChild("##2", ImVec2(623, 650), true);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 850, p.y + +5),ImGui::GetColorU32(ImGuiCol_ButtonActive) /*ImColor::HSV(rainbow,1.f,1.f)*/);
    ImGui::PopStyleVar();
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    switch (Tab)
    {
    case 0:
        tab_Ragebot();
        //Ragebot();
        break;
    case 1:
        tab_Legitbot();
        //Legitbot();
        break;
    case 2:
        tab_Visuals();
        //Visuals();
        break;
    case 3:
        tab_Misc();
        //Misc();
        break;
    case 4:
        //tab_Skin_Changer();
        tab_Skins();
        //Skins();
        break;
    case 5:
        //Whitelist();
        break;
    default:
        break;
    }
    ImGui::EndChild();
    ImGui::PopItemWidth();

    ImGui::Columns(1, NULL, false);

    ImGui::End();
    
}
