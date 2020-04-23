#pragma once

#include <string>
#include "Singleton.h"
#include "imgui/imgui.h"
#include <d3d9.h>
#include "newMenu.h"

struct IDirect3DDevice9;

class Menu: public Singleton<Menu>
{
public:
    //////////////////////////////////////////////////////
    //
    std::string user = "Not logged in";
    std::string version = "";
   
    void Initialize();
    void Shutdown();
    void OnDeviceLost();
    void OnDeviceReset();
    void Render();
    void Toggle();
    //////////////////////////////////////////////////////
    // TABS
    void Logged();
    void Login();
    void tab_Ragebot();
    void tab_Aimbot();
    void tab_AntiAim();
    void tab_Legitbot();
    void tab_Visuals();
    void tab_Misc();
    //void tab_Players();
    void tab_Skin_Changer();
    void tab_Skins();
    void tab_Login();
    void tab_Main();

    //
    //////////////////////////////////////////////////////
    // CONFIG
    bool IsVisible() const { return _visible; }
    bool IsLogged() const { return _logged; }

    bool _logged = false;

    //
    //////////////////////////////////////////////////////
    std::string erro;
private:
    void CreateStyle();
    ImDrawList* imdraw_list = nullptr;
    ImGuiStyle        _style;
    bool              _visible;
};

extern IDirect3DTexture9* b2;
extern IDirect3DTexture9* head;
extern IDirect3DTexture9* body;
extern IDirect3DTexture9* pelvis;
extern IDirect3DTexture9* legs;
extern IDirect3DTexture9* foot;
extern IDirect3DTexture9* bar;
extern void HelpMarker(const char* desc);
//extern ImColor bar_col;
//extern Color water_mark_new;
