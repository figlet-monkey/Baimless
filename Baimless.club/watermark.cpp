#include "watermark.h"

float frameRate2 = 0.f;
void Watermark::Show()
{
    if (!g_Options.esp.effects.watermark)
        return;
    if (g_Options.esp.effects.watermark_option != 0)
        return;
    char timebuff[80];
    time_t now = time(0);   // get time now
    struct tm  tstruct;
    tstruct = *localtime(&now);
    auto net_channel = g_pEngine->GetNetChannelInfo();
    auto latency = (net_channel && g_pEngine->IsInGame() && !g_pEngine->IsPlayingDemo()) ? net_channel->GetAvgLatency(FLOW_OUTGOING) : 0.0f;
    auto ping = std::to_string((int)(std::fmax(0.0f, latency) * 1000.0f));
    frameRate2 = 0.9f * frameRate2 + (1.0 - 0.9f) * g_pGlobalVars->absoluteframetime;
    std::stringstream ss;
    std::stringstream ss2;
    std::stringstream ss3;
    strftime(timebuff, sizeof(timebuff), "%X", &tstruct);
    //ss << timebuff << "   FPS: " << static_cast<int>(1.f / frameRate2) << "   Baimless 2.0   " << "PING: "<< ping.c_str() << "ms   " << Menu::get().user;
    //ss << "Baimless 2.0 | FPS: " << static_cast<int>(1.f / frameRate2) << " | PING: " << ping.c_str() << "ms" << " | " << Menu::get().user;
    ss << "Baimless 2.0";
    auto screen_size = ImGui::GetIO().DisplaySize;
    auto fps_text_size = ImGui::CalcTextSize("Baimless 2.0");
    //bool is_top = false;
    auto start_pos = ImVec2(screen_size.x / 2.f - 240.f, (screen_size.y - fps_text_size.y - 7.f));
    auto end_pos = ImVec2(screen_size.x / 2.f + 240.f,screen_size.y);

    ImU32 start_color = ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 0.0f));
    ImU32 end_color = ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 1.f));
    draw_list->AddRectFilledMultiColor(
        start_pos, ImVec2(start_pos.x + 240.f, end_pos.y), start_color, end_color, end_color, start_color
    );
    draw_list->AddRectFilledMultiColor(
        ImVec2(start_pos.x + 240.f, start_pos.y), end_pos, end_color, start_color, start_color, end_color
    );

    ImVec2 txt_size;
    txt_size = ImGui::CalcTextSize(ss.str().c_str());
    auto text_pos = ImVec2(start_pos.x + (480.f / 2) - (txt_size.x / 2), end_pos.y - 2.f - txt_size.y);
    if (!g_Options.misc.chroma_menu) 
        outlined_text(ss.str().c_str(), text_pos, ImGui::GetColorU32(ImVec4(255, 255, 255, 255)));
    else 
        outlined_text(ss.str().c_str(), text_pos, ImU32(ImColor::HSV(g_Options.rainbow, 1.f, 1.f, 1.f)));

    ss2 << timebuff << "   FPS: " << static_cast<int>(1.f / frameRate2);
    txt_size = ImGui::CalcTextSize(ss2.str().c_str());
    text_pos = ImVec2(start_pos.x + (480.f / 2) - (txt_size.x / 2) - txt_size.x - 2.f, end_pos.y - 2.f - txt_size.y);
    outlined_text(ss2.str().c_str(), text_pos, ImGui::GetColorU32(ImVec4(255, 255, 255, 255)));

    ss3 << "PING: " << ping.c_str() << "ms   " << Menu::get().user;
    txt_size = ImGui::CalcTextSize(ss3.str().c_str());
    text_pos = ImVec2(start_pos.x + (480.f / 2) - (txt_size.x / 2) + txt_size.x + 1.f, end_pos.y - 2.f - txt_size.y);
    outlined_text(ss3.str().c_str(), text_pos, ImGui::GetColorU32(ImVec4(255, 255, 255, 255)));

}

void Watermark::outlined_text(const char* text, const ImVec2& pos, const ImU32& color)
{
    outlined_text(text, pos, color, ImGui::GetColorU32(ImVec4(0.f, 0.f, 0.f, 1.f)));
}

void Watermark::outlined_text(const char* text, const ImVec2& pos, const ImU32& color, const ImU32& outlined_color)
{
    draw_list->AddText(ImVec2((pos.x) + 1.f, (pos.y) + 1.f), outlined_color, text);
    draw_list->AddText(ImVec2((pos.x) + 1.f, (pos.y) - 1.f), outlined_color, text);
    draw_list->AddText(pos, color, text);
}
Watermark* MAKER;
