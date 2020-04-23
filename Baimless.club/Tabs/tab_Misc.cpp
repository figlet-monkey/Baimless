#include "../Menu.h"
#include "../imgui/impl/imgui_impl_dx9.h"
#include "../imgui/impl/imgui_impl_win32.h"
#include "../Options.hpp"
#include "../http.h"
#include "../SDK/CGlobalVarsBase.h"
#include "../ui.h"
#include "../Render.h"
#include "../Config.h"
#include "../Hooks.h"
#define loop(n) for(int ii = 0; ii < n; ++ ii)

void Menu::tab_Misc()
{
    loop(3) ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15, 0.12, 0.21, 10));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.08, 0.07, 0.11, 1.0));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.11, 0.09, 0.16, 1.0));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 2));
    /**/
    ImGui::Columns(2, NULL, false);
    ImGui::PushFont(bold);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (35 / 2));
    ImGui::Text("Misc");
    ImGui::PopFont();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::BeginChild("##misc-1", ImVec2(290, 580), true,ImGuiWindowFlags_NoScrollbar);
    ImGui::NewLine(); ImGui::NewLine();
    ImGui::Checkbox("Bunny hop ##misc", &g_Options.misc.bhop);
    ImGui::Checkbox("Edge jump ##misc", &g_Options.misc.edge_jump); ImGui::SameLine(); ImGui::Hotkey("##edge-jump-key", &g_Options.misc.edge_jump_key);
    ImGui::Checkbox("Duck in air ##misc", &g_Options.misc.duck_in_air);
    ImGui::Checkbox("Reveal competitive ranks ##misc", &g_Options.misc.comp_ranks);
    ImGui::Checkbox("Air strafe ##misc", &g_Options.misc.air_strafe);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Combo("##air-strafe-combo", &g_Options.misc.air_strafe_option, "Off\0\rNormal\0\rWASD\0\0", IM_ARRAYSIZE("Off\0\rNormal\0\rWASD\0\0"));
    ImGui::Checkbox("Gravity ragdoll ##misc", &g_Options.misc.ragdoll);
	ImGui::Checkbox("Event logs##misc", &g_Options.misc.eventlogs);
    ImGui::Text("Clant tag");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Combo("##clan-tag-combo", &g_Options.misc.clantag_type, "Off\0\rBaimless.xyz\0\rCustom\0\0", IM_ARRAYSIZE("Off\0\rBaimless.xyz\0\rCustom\0\0"));
    ImGui::Text("Clan tag type");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Combo("##clan-tag-combo-type", &g_Options.misc.clantag_type_, "Static\0\rAnimated\0\0", IM_ARRAYSIZE("Static\0\rAnimated\0\0"));
    ImGui::Text("Custom clan tag");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    if (ImGui::InputText("##clantag-custom-_", &g_Options.clantag))
        updateClanTag(true);
    if(g_Options.misc.clantag_type_ == 1)
        updateClanTag(true);
    ImGui::Checkbox("Kill say##misc", &g_Options.misc.killMessage);
    ImGui::InputText("##kill-say", &g_Options.misc.killMessageString);
    ImGui::Checkbox("Name stealer ##misc", &g_Options.misc.name_stealer);
    ImGui::Checkbox("Knife bot ##misc", &g_Options.misc.knife_bot);
    ImGui::Checkbox("Fake prime##misc" ,&g_Options.misc.fakeprime);
    ImGui::Checkbox("Recoil crosshair##misc", &g_Options.misc.recoilCrosshair);
    ImGui::Checkbox("Auto accept", &g_Options.misc.autoaccpet);
    ImGui::Checkbox("Radar spoofer", &g_Options.misc.misc_radar);
    ImGui::Checkbox("Unlock inventory##misc",&g_Options.misc.unlockinventory);
    ImGui::Checkbox("Walk bot ##misc", &g_Options.misc.walk_bot);
    //ImGui::Checkbox("Switch hand on knife##misc", &g_Options.misc.misc_knife_switch);
    //ImGui::Checkbox("Radar engine", &g_Options.misc.radar);
    ImGui::Checkbox("Thridperson ##misc", &g_Options.misc.thirdperson); ImGui::SameLine(); ImGui::Hotkey("##thdperson-key", &g_Options.misc.thirdperson_key);
    ImGui::Text("Thirdperson Distance");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##thd-person-distance", &g_Options.misc.thirdpeson_distance, 0, 170, "%.f%°");
    ImGui::Checkbox("Auto buy##misc", &g_Options.misc.autobuy);
    ImGui::Text("Items");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    if (ImGui::BeginCombo("##autobuy", g_Options.misc.autobuy_str.c_str()))
    {
        const char* hitscans[] = { "AK47","M4A4","AWP","SSG08","SCAR20","G3SG1","ELITE","FLASHBANG","SMOKE GRENADE","HE GRENADE","MOLOTOV","INC GRENADE","VEST","VESTHELM","DEFUSER","ZEUS" };
        g_Options.misc.autobuy_str = "";
        std::vector<std::string> vec;
        for (size_t i = 0; i < IM_ARRAYSIZE(hitscans); i++)
        {
            ImGui::Selectable(hitscans[i], &g_Options.misc.autobuy_selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
            if (g_Options.misc.autobuy_selected[i])
                vec.push_back(hitscans[i]);
        }
        for (size_t i = 0; i < vec.size(); i++)
        {
            if (vec.size() == 1)
                g_Options.misc.autobuy_str += vec.at(i);
            else if (!(i == vec.size()))
                g_Options.misc.autobuy_str += vec.at(i) + ",";
            else
                g_Options.misc.autobuy_str += vec.at(i);
        }
        ImGui::EndCombo();
    }
    ImGui::NewLine();
    ImGui::Text("Ambient RED");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##red-misc", &g_Options.misc.ambient_red, 0, 100, "%.f%%");
    ImGui::Text("Ambient GREEN");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##green-misc", &g_Options.misc.ambient_green, 0, 100, "%.f%%");
    ImGui::Text("Ambient BLUE");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::SliderFloat("##blue-misc", &g_Options.misc.ambient_blue, 0, 100, "%.f%%");
    ImGui::NewLine();
    ImGui::EndChild();
    /**/
    ImGui::NextColumn();
    /**/
    ImGui::PushFont(bold);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (ImGui::CalcTextSize("Settings").x / 2));
    ImGui::Text("Settings");
    ImGui::PopFont();
    ImGui::BeginChild("##misc-2", ImVec2(290, 200), true);
    ImGui::NewLine(); ImGui::NewLine();
    ImGui::Checkbox("Chroma menu##misc", &g_Options.misc.chroma_menu);
    ImGui::ColorEdit3("##menu_color", menu::color, ImGuiColorEditFlags_NoInputs);  ImGui::SameLine(); ImGui::Text("Menu color");
    ImGui::Checkbox("Anti-untrusted ##misc", &g_Options.misc.ant_untrusted);
    ImGui::Text("Menu key"); ImGui::SameLine();ImGui::Hotkey("##menu-key", &g_Options.misc.menu_key);
    ImGui::EndChild();
    /**/
    ImGui::PushFont(bold);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (ImGui::CalcTextSize("Other").x / 2));
    ImGui::Text("Other");
    ImGui::PopFont();
    ImGui::BeginChild("##misc-3", ImVec2(290, 350), true);
    ImGui::NewLine(); 
    static std::vector<std::string> configItems = ConfigSys::get().GetAllConfigs();
    static int configItemCurrent = -1;

    static char fName[32] = "Baimless";
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (ImGui::CalcTextSize("Config List").x / 2));
    ImGui::Text("Config List");
    ImGui::BeginChild(52, ImVec2(-1, 70 ), true);
    if (configItems.size())
    {  
        for (std::vector<int>::size_type i = 0; i != configItems.size(); i++)
        {
            bool item_selected = i == configItemCurrent;

            if (ImGui::Selectable(std::string("  ~ " + configItems[i]).c_str(), item_selected))
            {
                configItemCurrent = i;
            }
        }    
    }
    ImGui::EndChild();
    ImGui::PushItemWidth(-1);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (ImGui::CalcTextSize("Config Name").x / 2));
    ImGui::Text("Config Name");
    ImGui::InputText("##cfgname", fName, 32);
    ImGui::PopItemWidth();
    ImGui::NewLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (190 / 2));
    if (ImGui::Button("Save ##config", ImVec2(190, 30))) {

        if (configItems.size() > 0 && (configItemCurrent >= 0 && configItemCurrent < (int)configItems.size()))
        {
            std::string fPath = std::string(ConfigSys::get().my_documents_folder) + "\\baimless.xyz\\" + configItems[configItemCurrent] + ".xyz";
            ConfigSys::get().SaveConfig(fPath);
        }
    }
    ImGui::NewLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (190 / 2));
    if (ImGui::Button("Load ##config", ImVec2(190, 30))) {
        if (configItems.size() > 0 && (configItemCurrent >= 0 && configItemCurrent < (int)configItems.size()))
        {
            std::string fPath = std::string(ConfigSys::get().my_documents_folder) + "\\baimless.xyz\\" + configItems[configItemCurrent] + ".xyz";
            ConfigSys::get().LoadConfig(fPath);
        }
    }
    ImGui::NewLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (190 / 2));
    if (ImGui::Button("Create ##config", ImVec2(190, 30))) {
        std::string fPath = std::string(ConfigSys::get().my_documents_folder) + "\\baimless.xyz\\" + fName + ".xyz";
        ConfigSys::get().SaveConfig(fPath);

        configItems = ConfigSys::get().GetAllConfigs();
        configItemCurrent = -1;
    }
    ImGui::NewLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (190 / 2));
    if (ImGui::Button("Remove ##config", ImVec2(190, 30))) {
        if (configItems.size() > 0 && (configItemCurrent >= 0 && configItemCurrent < (int)configItems.size()))
        {
            std::string fPath = std::string(ConfigSys::get().my_documents_folder) + "\\baimless.xyz\\" + configItems[configItemCurrent] + ".xyz";
            std::remove(fPath.c_str());

            configItems = ConfigSys::get().GetAllConfigs();
            configItemCurrent = -1;
        }
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
