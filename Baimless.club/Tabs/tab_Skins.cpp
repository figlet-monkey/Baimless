#include "../Menu.h"
#include "../imgui/impl/imgui_impl_dx9.h"
#include "../imgui/impl/imgui_impl_win32.h"
#include "../Options.hpp"
#include "../http.h"
#include "../SDK/CGlobalVarsBase.h"
#include "../ui.h"
#include "../Render.h"


#include "../../Baimless.club/Features/rankchanger/IMemAlloc.h"
#include "../../Baimless.club/Features/rankchanger/steam.h"
#include "../../Baimless.club/Features/rankchanger/rankchanger.h"

#include "../../Baimless.club/Features/rankchanger/Protobuffs.h"


#include "../../Baimless.club/Hooks.h"


#define loop(n) for(int ii = 0; ii < n; ++ ii)
static const char* bans[] =
{
    "Off",
    "You were kicked from the last match (competitive cooldown)",
    "You killed too many teammates (competitive cooldown)",
    "You killed a teammate at round start (competitive cooldown)",
    "You failed to reconnect to the last match (competitive cooldown)",
    "You abandoned the last match (competitive cooldown)",
    "You did too much damage to your teammates (competitive cooldown)",
    "You did too much damage to your teammates at round start (competitive cooldown)",
    "This account is permanently untrusted (global cooldown)",
    "You were kicked from too many recent matches (competitive cooldown)",
    "Convicted by overwatch - majorly disruptive (global cooldown)",
    "Convicted by overwatch - minorly disruptive (global cooldown)",
    "Resolving matchmaking state for your account (temporary cooldown)",
    "Resolving matchmaking state after the last match (temporary cooldown)",
    "This account is permanently untrusted (global cooldown)",
    "(global cooldown)",
    "You failed to connect by match start. (competitive cooldown)",
    "You have kicked too many teammates in recent matches (competitive cooldown)",
    "Congratulations on your recent competitive wins! before you play competitive matches further please wait for matchmaking servers to calibrate your skill group placement based on your lastest performance. (temporary cooldown)",
    "A server using your game server login token has been banned. your account is now permanently banned from operating game servers, and you have a cooldown from connecting to game servers. (global cooldown)"
};
const char* ranks[] = { "off",
                "silver 1",
                "silver 2",
                "silver 3",
                "silver 4",
                "silver elite",
                "silver elite master",
                "gold nova 1",
                "gold nova 2",
                "gold nova 3",
                "gold nova master",
                "master guardian 1",
                "master guardian 2",
                "master guardian elite",
                "distinguished master guardian",
                "legendary eagle",
                "legendary eagle master",
                "supreme master first class",
                "the global elite" };
void rankupdate()
{
    write.SendClientHello();
    write.SendMatchmakingClient2GCHello();
}

void Menu::tab_Skins()
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
    /**/
    ImGui::Columns(2, NULL, false);
    ImGui::PushFont(bold);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (37 / 2));
    ImGui::Text("Main");
    ImGui::PopFont();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::BeginChild("##skins-1", ImVec2(290, 580), true, ImGuiWindowFlags_NoScrollbar);
    ImGui::Spacing();
    ImGui::Checkbox("Rank changer##other-skin", &g_Options.changers.rank.enable);

    ImGui::Text("Rank");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Combo("##Rank", &g_Options.changers.rank.selected, ranks, ARRAYSIZE(ranks));

    ImGui::Text("Level");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::SliderInt("##Level", &g_Options.changers.rank.lvl, 0, 40);

    ImGui::Text("XP");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::InputInt("##Xp##level", &g_Options.changers.rank.xp);

    ImGui::Text("Wins");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::InputInt("##Wins", &g_Options.changers.rank.wins);

    ImGui::Text("Friend");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::InputInt("##Friend", &g_Options.changers.rank.friend_);

    ImGui::Text("Teach");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::InputInt("##Teach", &g_Options.changers.rank.teach);

    ImGui::Text("Leader");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::InputInt("##Leader", &g_Options.changers.rank.leader);
   
    ImGui::Text("Fake ban type");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::Combo("##fake-ban", &g_Options.ban.type, bans, IM_ARRAYSIZE(bans));

    ImGui::Text("Fake ban time");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
    ImGui::SliderInt("##fake-ban-time", &g_Options.ban.time, 0, 1000, "Seconds: %d");
	/*ImGui::Text("Local player model");
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
    ImGui::Combo("##Player Model", &g_Options.changers.playerModel, "Default\0Special Agent Ava | FBI\0Operator | FBI SWAT\0Markus Delrow | FBI HRT\0Michael Syfers | FBI Sniper\0B Squadron Officer | SAS\0Seal Team 6 Soldier | NSWC SEAL\0Buckshot | NSWC SEAL\0Lt. Commander Ricksaw | NSWC SEAL\0Third Commando Company | KSK\0'Two Times' McCoy | USAF TACP\0Dragomir | Sabre\0Rezan The Ready | Sabre\0'The Doctor' Romanov | Sabre\0Maximus | Sabre\0Blackwolf | Sabre\0The Elite Mr. Muhlik | Elite Crew\0Ground Rebel | Elite Crew\0Osiris | Elite Crew\0Prof. Shahmat | Elite Crew\0Enforcer | Phoenix\0Slingshot | Phoenix\0Soldier | Phoenix\0");
	*/
    ImGui::NewLine();
    if (ImGui::Button("Apply##skin", ImVec2(-1, 30)))
        rankupdate();
   
    ImGui::EndChild();
    ///////////////////////
    ImGui::NextColumn();
    ImGui::PushFont(bold);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (60 / 2));
    ImGui::Text("Skins");
    ImGui::PopFont();
    ImGui::BeginChild("##skins-2", ImVec2(290, 580), true,ImGuiWindowFlags_NoScrollbar);
    ImGui::Spacing();
    tab_Skin_Changer();
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
