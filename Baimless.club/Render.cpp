#include "Render.h"
#include <mutex>
#include "imgui/imgui.h"
#include "Singleton.h"
#include "Utils/Color.h"
#include "imgui/impl/imgui_impl_win32.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "Utils/Interfaces.h"
#include "Input.h"
#include "SDK/CEntity.h"
#include "newMenu.h"
#include "Features/Visuals/ESP.h"
#include "fonts/fonts.hpp"
#include "../Visuals.h"
#include "imgui/imgui_internal.h"
#include "OBS ESP.h"
#include "Options.hpp"
#include "B2 Icon.h"
#include "Font.hpp"
#include "BAR Icon.h"
#include "Arial.hpp"
#include "hitscan.h"
#include "Weapons.hpp"
#include "Menu.h"
#include <SDK\CGlobalVarsBase.h>
#include "Lucida.hpp"
#define g_LocalPlayer g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer())

ImFont* g_pDefaultFont;
ImFont* g_pSecondFont;
ImFont* Seta;
ImFont* icon;
ImFont* bold;
ImFont* weapons;
ImDrawListSharedData _data;
ImFont* watermark;
ImFont* notifies_font;

std::mutex render_mutex;

void Render::Initialize()
{
    ImGui::CreateContext();


    ImGui_ImplWin32_Init(InputSys::get().GetMainWindow());
    ImGui_ImplDX9_Init(g_D3DDevice9);

    draw_list = new ImDrawList(ImGui::GetDrawListSharedData());
    draw_list_act = new ImDrawList(ImGui::GetDrawListSharedData());
    draw_list_rendering = new ImDrawList(ImGui::GetDrawListSharedData());

    GetFonts();
}

void Render::GetFonts()
{
    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Fonts::Droid_compressed_data,Fonts::Droid_compressed_size,12.f);
    Seta = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Fonts::Seta_compressed_data,Fonts::Seta_compressed_size,30);
    g_pDefaultFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Fonts::Droid_compressed_data,Fonts::Droid_compressed_size,18.f);
    g_pSecondFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Fonts::Cousine_compressed_data,Fonts::Cousine_compressed_size,12.f);
    bold = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Arial_compressed_data, Arial_compressed_size, 20);
    icon = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Baimless_compressed_data, Baimless_compressed_size, 35);
    weapons = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Weapons_compressed_data, Weapons_compressed_size, 30);
	notifies_font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Lucida_compressed_data, Lucida_compressed_size, 10.f);
    watermark = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 12.f);
}

void Render::ClearDrawList() {
    render_mutex.lock();
    draw_list_act->Clear();
    render_mutex.unlock();
}

void Manual_aa_indicator()
{
    static bool side = false;
    float gayside;
    static bool side2 = false;
    float gayside2;
    if (g_Options.legit.legit_aa_key)
    {
        side = false;
        if (GetKeyState(g_Options.legit.legit_aa_key))
        {
            side = true;
        }
    }

    if (side)
    {
        gayside = 1;

    }
    else
    {
        gayside = -1;
    }
    if (g_Options.esp.enable)
    {
        if (g_Options.esp.effects.manual_aa)
        {
            int W, H, cW, cH;
            g_pEngine->GetScreenSize(W, H);
            cW = W / 2;
            cH = H / 2;

            if (g_pEngine->IsConnected() && g_pEngine->IsInGame())
            {

                if (g_Options.antiaa.invert_key)
                {
                    side2 = false;
                    if (GetKeyState(g_Options.antiaa.invert_key))
                    {
                        side2 = true;
                    }
                }

                if (side2)
                {
                    //Render::get().RenderText("1", 10, 5, 18.f, Color(255, 255, 255, 255), false, true, g_pSecondFont);
                    Render::get().RenderText("A", ImVec2(cW + 28, cH - 15), 30.f, Color(255, 255, 255, 255), false, false, Seta);
                    Render::get().RenderText("B", ImVec2(cW - 65, cH - 15), 30.f, Color(esp::colors::manual_aa), false, false, Seta);
                    //g_pSurface->TextNEW(cW + 28, cH - 20, Color(255, 255, 255, 255), g::LBY2, L"▶");
                    //g_pSurface->TextNEW(cW - 44, cH - 20, Color(Options::Menu.VisualsTab.manualaaindicator_col.GetValue(), 255), g::LBY2, L"◀");
                }
                else
                {
                    //Render::get().RenderText("Two", 10, 5, 18.f, Color(255, 255, 255, 255), false, true, g_pSecondFont);
                    Render::get().RenderText("A", ImVec2(cW + 28, cH - 15), 30.f, Color(esp::colors::manual_aa), false, false, Seta);
                    Render::get().RenderText("B", ImVec2(cW - 65, cH - 15), 30.f, Color(255, 255, 255, 255), false, false, Seta);
                    //g_pSurface->TextNEW(cW + 28, cH - 20, Color(Options::Menu.VisualsTab.manualaaindicator_col.GetValue(), 255), g::LBY2, L"▶");
                    //g_pSurface->TextNEW(cW - 44, cH - 20, Color(255, 255, 255, 255), g::LBY2, L"◀");
                }

            }
            else
            {

            }
        }
    }
}
float frameRate = 0.f;
void Watermark()
{
    frameRate = 0.9f * frameRate + (1.0 - 0.9f) * g_pGlobalVars->absoluteframetime;
    static float rainbow;
    rainbow += 0.005f;
    if (rainbow > 1.f)
        rainbow = 0.f;
    auto net_channel = g_pEngine->GetNetChannelInfo();
    auto latency = (net_channel && g_pEngine->IsInGame() && !g_pEngine->IsPlayingDemo()) ? net_channel->GetAvgLatency(FLOW_OUTGOING) : 0.0f;
    auto ping = std::to_string((int)(std::fmax(0.0f, latency) * 1000.0f));
    std::stringstream ss;
    ss << "Baimless 2.0 | FPS: " << static_cast<int>(1.f / frameRate) << " | PING: " << ping.c_str() << "ms" << " | " << Menu::get().user ;
    Render::get().RenderBoxFilled(380, 38, 5, 5, Color(38, 31, 54));
    if(!g_Options.misc.chroma_menu)
        Render::get().RenderLine(380, 10, 5, 10, Color(menu::color), 3.f);
    else
        Render::get().RenderLine(380, 10, 5, 10, Color::FromHSB(g_Options.rainbow, 1.f, 1.f), 3.f);
    Render::get().RenderText(ss.str(), ImVec2(15,20), 12.f, Color(255, 255, 255, 255), false, false, g_pSecondFont);
}

void Render::BeginScene() {
    draw_list->Clear();
    draw_list->PushClipRectFullScreen();

    if (g_pEngine->IsInGame() && g_LocalPlayer && g_Options.esp_streamproof)
        Visuals::get().AddToDrawList();
    
    Manual_aa_indicator();
    if (b2 == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9, &img, sizeof(img), 180, 180, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &b2);
    if (bar == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9, &img2, sizeof(img2), 180, 180, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &bar);

    if (head == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9, &head_img, sizeof(head_img), 150, 40, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &head);
    if (body == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9, &body_img, sizeof(body_img), 150, 77, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &body);
    if (pelvis == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9, &pelvis_img, sizeof(pelvis_img), 150, 61, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &pelvis);
    if (legs == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9, &legs_img, sizeof(legs_img), 150, 34, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &legs);
    if (foot == nullptr)
        D3DXCreateTextureFromFileInMemoryEx(g_D3DDevice9, &foot_img, sizeof(foot_img), 150, 34, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &foot);
    if (g_Options.esp.effects.watermark && g_Options.esp.effects.watermark_option == 1)
        Watermark();

    render_mutex.lock();
    *draw_list_act = *draw_list;
    render_mutex.unlock();
}

ImDrawList* Render::RenderScene() {

    if (render_mutex.try_lock()) {
        *draw_list_rendering = *draw_list_act;
        render_mutex.unlock();
    }

    return draw_list_rendering;
}


float Render::RenderText(const std::string& text, ImVec2 pos, float size, Color color, bool center, bool outline, ImFont* pFont)
{
    ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, text.c_str());
    if (!pFont->ContainerAtlas) return 0.f;
    draw_list->PushTextureID(pFont->ContainerAtlas->TexID);

    if (center)
        pos.x -= textSize.x / 2.0f;

    if (outline) {
        draw_list->AddText(pFont, size, ImVec2(pos.x + 1, pos.y + 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
        draw_list->AddText(pFont, size, ImVec2(pos.x - 1, pos.y - 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
        draw_list->AddText(pFont, size, ImVec2(pos.x + 1, pos.y - 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
        draw_list->AddText(pFont, size, ImVec2(pos.x - 1, pos.y + 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
    }

    draw_list->AddText(pFont, size, pos, GetU32(color), text.c_str());

    draw_list->PopTextureID();

    return pos.y + textSize.y;
}

void Render::RenderCircle3D(Vector position, float points, float radius, Color color)
{
    float step = (float)M_PI * 2.0f / points;

    for (float a = 0; a < (M_PI * 2.0f); a += step)
    {
        Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
        Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

        Vector start2d, end2d;
        if (DebugOverlay->ScreenPosition(start, start2d) || DebugOverlay->ScreenPosition(end, end2d))
            return;

        RenderLine(start2d.x, start2d.y, end2d.x, end2d.y, color);
    }
}
