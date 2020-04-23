/*#include "../Menu.h"
#include "../imgui/impl/imgui_impl_dx9.h"
#include "../imgui/impl/imgui_impl_win32.h"
#include "../Options.hpp"
#include "../http.h"
#include "../SDK/CGlobalVarsBase.h"
#include "../ui.h"
#include "../Render.h"
#include "../skinchanger.h"

static char* gloves[] = {
    "Default",
    "Bloodhound",
    "Sport",
    "Driver",
    "Hand Wraps",
    "Motorcycle",
    "Specialist",
    "Hydra"
};

void Menu::tab_Gloves()
{
    const char* gstr;
    if (g_Options.skins_glove_model == 1)
        gstr = "Charred\0\rSnakebite\0\rBronzed\0\rGuerilla\0\0";
    else if (g_Options.skins_glove_model == 2)
        gstr = "Hedge Maze\0\rPandoras Box\0\rSuperconductor\0\rArid\0\rVice\0\rOmega\0\rAmphibious\0\rBronze Morph\0\0";
    else if (g_Options.skins_glove_model == 3)
        gstr = "Lunar Weave\0\rConvoy\0\rCrimson Weave\0\rDiamondback\0\rOvertake\0\rRacing Green\0\rKing Snake\0\rImperial Plaid\0\0";
    else if (g_Options.skins_glove_model == 4)
        gstr = "Leather\0\rSpruce DDPAT\0\rSlaughter\0\rBadlands\0\rCobalt Skulls\0\rOverprint\0\rDuct Tape\0\rArboreal\0\0";
    else if (g_Options.skins_glove_model == 5)
        gstr = "Eclipse\0\rSpearmint\0\rBoom!\0\rCool Mint\0\rTurtle\0\rTransport\0\rPolygon\0\rPOW!\0\0";
    else if (g_Options.skins_glove_model == 6)
        gstr = "Forest DDPAT\0\rCrimson Kimono\0\rEmerald Web\0\rFoundation\0\rCrimson Web\0\rBuckshot\0\rFade\0\rMogul\0\0";
    else if (g_Options.skins_glove_model == 7)
        gstr = "Emerald\0\rMangrove\0\rRattler\0\rCase Hardened\0\0";
    else
        gstr = "";
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (65 / 2));
    ImGui::Text("Glove model");
    ImGui::ListBox("##Glove Model", &g_Options.skins_glove_model, gloves, ARRAYSIZE(gloves),15);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (290 / 2) - (60 / 2));
    ImGui::Text("Glove skin");
    ImGui::ListBox("##glove-skin", &g_Options.skins_glove_skin, &gstr, IM_ARRAYSIZE(gstr),15);

    if (ImGui::Button("Force Update", ImVec2(-1, 20 + 9)))
        Fullupdate();
}
*/
