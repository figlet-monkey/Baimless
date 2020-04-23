#include "../Menu.h"
#include "../imgui/impl/imgui_impl_dx9.h"
#include "../imgui/impl/imgui_impl_win32.h"
#include "../Options.hpp"
#include "../http.h"
#include "../SDK/CGlobalVarsBase.h"
#include "../ui.h"
#include "../Render.h"
#include <fstream>
#include <string>
#include "../nlohmann/json.hpp"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <shlobj.h>

using json = nlohmann::json;
CHAR my_documents[MAX_PATH];
HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);

void Menu::tab_Login()
{
    bool window = !_logged;
    static char username[255] = "";
    static char password[255] = "";


    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15, 0.12, 0.21, 10));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.08, 0.07, 0.11, 1.0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.11, 0.09, 0.16, 1.0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 2));
    ImGui::SetNextWindowSize(ImVec2(350, 320));
    ImGui::Begin("##login-tab", &window, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
    ImGui::Spacing();
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 340, p.y + +3), g_Options.bar_col);
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetWindowWidth() / 2) - (100 / 2));
    ImGui::Image(b2, ImVec2(100, 100));
    ImGui::Spacing(); ImGui::Spacing();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetWindowWidth() / 2) - (ImGui::CalcTextSize("Username").x / 2));
    ImGui::Text("Username");
    ImGui::PushItemWidth(-1);
    ImGui::InputText("##username", username, IM_ARRAYSIZE(username));
    ImGui::PopItemWidth();
    ImGui::Spacing();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetWindowWidth() / 2) - (ImGui::CalcTextSize("Password").x / 2));
    ImGui::Text("Password");
    ImGui::PushItemWidth(-1);
    ImGui::InputText("##password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
    ImGui::PopItemWidth();
    ImGui::Checkbox("Remember me", &g_Options.remember);
    ImGui::TextColored(ImColor(255, 0, 0), erro.c_str());
    ImGui::Spacing(); ImGui::Spacing();
    if (ImGui::Button("Load remember me", ImVec2(-1, 25)))
    {
        std::string path = std::string(my_documents) + "\\baimless.xyz\\info.auth";
        std::ifstream i(path);

        if (i)
        {
            json j;
            i >> j;
            std::string u = j["user"];
            std::string p = j["pass"];
            strcpy(username, u.c_str());
            strcpy(password, p.c_str());
        }
        i.close();
    }
    ImGui::Spacing();
    if (ImGui::Button("Login", ImVec2(-1, 25))) {
        if (g_Options.remember)
        {
            std::string path = std::string(my_documents) + "\\baimless.xyz\\info.auth";
            std::ofstream o(path);
            json info = {
               {"user",username  },
               {"pass",password}
            };
            o << std::setw(4) << info << std::endl;
            o.close();
        }
        http_request::get().api(username, password);
    }
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}
