#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "SDK/CEntity.h"
#include "SDK/ICvar.h"
#include "Options.hpp"
#include <sstream>
#include <algorithm>
#include "Render.h"
#include <Utils\GlobalVars.h>
#include "SDK/CGlobalVarsBase.h"
#include "SDK/CEntity.h"
#include "Menu.h"
#define draw_list g::notify
class Watermark
{
public:
    void Show();
private:
    void outlined_text(const char* text, const ImVec2& pos, const ImU32& color);
    void outlined_text(const char* text, const ImVec2& pos, const ImU32& color, const ImU32& outlined_color);
    
};
extern Watermark* MAKER;
