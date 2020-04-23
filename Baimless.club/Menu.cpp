#include "Menu.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "imgui/impl/imgui_impl_win32.h"
#include "Options.hpp"
#include "http.h"
#include "Render.h"
#include "ui.h"
#include <d3d9.h>
#include <SDK\CGlobalVarsBase.h>
#include <notifies.h>
#include <watermark.h>
IDirect3DTexture9* b2;
IDirect3DTexture9* bar;

IDirect3DTexture9* head;
IDirect3DTexture9* body;
IDirect3DTexture9* pelvis;
IDirect3DTexture9* legs;
IDirect3DTexture9* foot;
void Menu::Logged()
{
    if (!_logged)
        return;
    tab_Main();
}

void Menu::Login()
{
    if (_logged)
        return;
    tab_Login();
}




void Menu::Render()
{
	notifies::handle();
    MAKER->Show();
    g_Options.rainbow += 0.0005f;
    if (g_Options.rainbow > 1.f)
        g_Options.rainbow = 0.f;
	if (!_visible)
		return;
    //ImGui::GetIO().MouseDrawCursor = _visible;
  
    constexpr float frequency = 1.f / 0.5f;
    float step = frequency * g_pGlobalVars->frametime;

    static float alpha = 0;

    _visible ? alpha += step : alpha -= step;

    if (alpha > 1.f)
        alpha = 1;
    if (alpha < 0.f)
        alpha = 0;

    ImGui::GetStyle().Alpha = alpha;
    
    if (alpha >= 0.1f)
    {
        float c[3] = { 1.f,1.f,1.f };
        if (menu::color != c)
            CreateStyle();
        if (g_Options.misc.chroma_menu)
            CreateStyle();
        Login();
        Logged();
    }
}

void Menu::CreateStyle()
{
    if (!g_Options.misc.chroma_menu) {
        ImGui::StyleColorsDark();
        ImGui::SetColorEditOptions(ImGuiColorEditFlags_HEX);
        ImGuiStyle& style = ImGui::GetStyle();
       // _style.Alpha = 1.0f;
        _style.WindowRounding = 10.0f;
        _style.WindowBorderSize = 0.0f;
        _style.WindowPadding = ImVec2(0, 0);
        _style.ChildBorderSize = 0.0f;
        _style.ChildRounding = 10.0f;
        _style.WindowBorderSize = 0.f;
        _style.FrameRounding = 10.0f;
        _style.Colors[ImGuiCol_ChildBg] = ImColor(21, 17, 29);//rgb(53, 59, 72)
        _style.Colors[ImGuiCol_WindowBg] = ImColor(47, 41, 56); //rgb(47, 54, 64)
        _style.Colors[ImGuiCol_Button] = ImColor(47, 41, 56);
        _style.Colors[ImGuiCol_ButtonActive] = ImColor(menu::color[0], menu::color[1], menu::color[2]);
        _style.Colors[ImGuiCol_ButtonHovered] = ImColor(47, 41, 56);
        _style.Colors[ImGuiCol_FrameBg] = ImColor(38, 31, 54);
        _style.Colors[ImGuiCol_FrameBgActive] = ImColor(38, 31, 54);
        _style.Colors[ImGuiCol_FrameBgHovered] = ImColor(38, 31, 54);
        _style.Colors[ImGuiCol_SliderGrab] = ImColor(menu::color[0], menu::color[1], menu::color[2]);
        _style.Colors[ImGuiCol_SliderGrabActive] = ImColor(menu::color[0], menu::color[1], menu::color[2]);
        _style.Colors[ImGuiCol_Header] = ImColor(menu::color[0], menu::color[1], menu::color[2]);
        _style.Colors[ImGuiCol_HeaderActive] = ImColor(menu::color[0], menu::color[1], menu::color[2]);
        _style.Colors[ImGuiCol_HeaderHovered] = ImColor(menu::color[0], menu::color[1], menu::color[2]);
        _style.Colors[ImGuiCol_CheckMark] = ImColor(menu::color[0], menu::color[1], menu::color[2]);

        ImGui::GetStyle() = _style;
    }
    else
    {
     
       
        ImGui::StyleColorsDark();
        ImGui::SetColorEditOptions(ImGuiColorEditFlags_HEX);
        ImGuiStyle& style = ImGui::GetStyle();
       // _style.Alpha = 1.0f;
        _style.WindowRounding = 10.0f;
        _style.WindowBorderSize = 0.0f;
        _style.WindowPadding = ImVec2(0, 0);
        _style.ChildBorderSize = 0.0f;
        _style.ChildRounding = 10.0f;
        _style.WindowBorderSize = 0.f;
        _style.FrameRounding = 10.0f;
        _style.Colors[ImGuiCol_ChildBg] = ImColor(21, 17, 29);//rgb(53, 59, 72)
        _style.Colors[ImGuiCol_WindowBg] = ImColor(47, 41, 56); //rgb(47, 54, 64)
        _style.Colors[ImGuiCol_Button] = ImColor(47, 41, 56);
        _style.Colors[ImGuiCol_ButtonActive] = ImColor::HSV(g_Options.rainbow,1.f,1.f,1.f);
        _style.Colors[ImGuiCol_ButtonHovered] = ImColor(47, 41, 56);
        _style.Colors[ImGuiCol_FrameBg] = ImColor(38, 31, 54);
        _style.Colors[ImGuiCol_FrameBgActive] = ImColor(38, 31, 54);
        _style.Colors[ImGuiCol_FrameBgHovered] = ImColor(38, 31, 54);
        _style.Colors[ImGuiCol_SliderGrab] = ImColor::HSV(g_Options.rainbow, 1.f, 1.f, 1.f);
        _style.Colors[ImGuiCol_SliderGrabActive] = ImColor::HSV(g_Options.rainbow, 1.f, 1.f, 1.f);
        _style.Colors[ImGuiCol_Header] = ImColor::HSV(g_Options.rainbow, 1.f, 1.f, 1.f);
        _style.Colors[ImGuiCol_HeaderActive] = ImColor::HSV(g_Options.rainbow, 1.f, 1.f, 1.f);
        _style.Colors[ImGuiCol_HeaderHovered] = ImColor::HSV(g_Options.rainbow, 1.f, 1.f, 1.f);
        _style.Colors[ImGuiCol_CheckMark] = ImColor::HSV(g_Options.rainbow, 1.f, 1.f, 1.f);

        ImGui::GetStyle() = _style;
    }
}
#pragma region not_important
void HelpMarker(const char* desc)
{
    ImGui::TextColored(ImColor(ImGui::GetColorU32(ImGuiCol_ButtonActive)), "(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void Menu::Toggle()
{
    _visible = !_visible;
}

void Menu::Initialize()
{
    CreateStyle();

    _visible = false;
}

void Menu::Shutdown()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void Menu::OnDeviceLost()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
}

void Menu::OnDeviceReset()
{
    ImGui_ImplDX9_CreateDeviceObjects();
}
#pragma endregion
