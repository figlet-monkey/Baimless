#include "GUI.h"
#include "newMenu.h"
//#include "utils\draw_manager.h"
#include <algorithm>
#include "tinyxml2.h"
#include "Controls.h"
#include <sstream>
#include "Utils\Color.h"
#include "SDK\ISurface.h"
#include "SDK\ISurface.h"
#include "Utils\GlobalVars.h"
#include "SDK\ICvar.h"
#include "SDK\CGlobalVarsBase.h"
#include "../Baimless.club/Options.hpp"


void CWindow::SetPosition(int x, int y)
{
	m_x = x; m_y = y;
}
void CWindow::SetSize(int w, int h)
{
	m_iWidth = w;
	m_iHeight = h;
}
void CWindow::SetTitle(std::string title)
{
	Title = title;
}
void CWindow::RegisterTab(CTab* Tab)
{
	if (Tabs.size() == 0)
	{
		SelectedTab = Tab;
	}
	Tab->parent = this;
	Tabs.push_back(Tab);
}
RECT CWindow::GetClientArea()
{
	RECT client;
	client.left = m_x + 8;
	client.top = m_y + 1 + 27;
	client.right = m_iWidth - 4 - 12;
	client.bottom = m_iHeight - 2 - 8 - 26 + 420;
	return client;
}
RECT CWindow::GetTabArea()
{
	RECT client;
	client.left = m_x + 8;
	client.top = m_y + 1 + 27;
	client.right = m_iWidth - 4 - 12;
	client.bottom = 29;
	return client;
}
RECT CWindow::GetDragArea()
{
	RECT client;
	client.left = m_x + 8;
	client.top = m_y + 1 + 27;
	client.right = m_iWidth - 4 - 12;
	client.bottom = 29;
	return client;
}



void CWindow::Open()
{
	m_bIsOpen = true;
}
void CWindow::Close()
{
	m_bIsOpen = false;
}
void CWindow::Toggle()
{
	m_bIsOpen = !m_bIsOpen;
	static ConVar* cvar = g_pCvar->FindVar("cl_mouseenable");
	if (m_bIsOpen)
	{
		cvar->SetValue(0);
	}
	else
	{
		cvar->SetValue(1);
	}
}
CControl* CWindow::GetFocus()
{
	return FocusedControl;
}
// TABS ---------------------------------------------------------------------------------------------------
void CTab::SetTitle(std::string name)
{
	Title = name;
}
void CTab::RegisterControl(CControl* control)
{
	control->parent = parent;
	Controls.push_back(control);
}

float Globals::MenuAlpha2 = Menulooks::MenuAlpha;

CGUI GUI;

CGUI::CGUI()
{

}
// Draws all windows 
void CGUI::Draw()
{
	bool ShouldDrawCursor = false;

	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
		{
			ShouldDrawCursor = true;
			DrawWindow(window, 1);
		}
	}

	if (ShouldDrawCursor)
	{
		g_pSurface->Clear(Mouse.x + 1, Mouse.y, 1, 17, Color(3, 6, 26, 255));
		for (int i = 0; i < 11; i++)
			g_pSurface->Clear(Mouse.x + 2 + i, Mouse.y + 1 + i, 1, 1, Color(3, 6, 26, 255));
		g_pSurface->Clear(Mouse.x + 8, Mouse.y + 12, 5, 1, Color(3, 6, 26, 255));
		g_pSurface->Clear(Mouse.x + 8, Mouse.y + 13, 1, 1, Color(3, 6, 26, 255));
		g_pSurface->Clear(Mouse.x + 9, Mouse.y + 14, 1, 2, Color(3, 6, 26, 255));
		g_pSurface->Clear(Mouse.x + 10, Mouse.y + 16, 1, 2, Color(3, 6, 26, 255));
		g_pSurface->Clear(Mouse.x + 8, Mouse.y + 18, 2, 1, Color(3, 6, 26, 255));
		g_pSurface->Clear(Mouse.x + 7, Mouse.y + 16, 1, 2, Color(3, 6, 26, 255));
		g_pSurface->Clear(Mouse.x + 6, Mouse.y + 14, 1, 2, Color(3, 6, 26, 255));
		g_pSurface->Clear(Mouse.x + 5, Mouse.y + 13, 1, 1, Color(3, 6, 26, 255));
		g_pSurface->Clear(Mouse.x + 4, Mouse.y + 14, 1, 1, Color(3, 6, 26, 255));
		g_pSurface->Clear(Mouse.x + 3, Mouse.y + 15, 1, 1, Color(3, 6, 26, 255));
		g_pSurface->Clear(Mouse.x + 2, Mouse.y + 16, 1, 1, Color(3, 6, 26, 255));
		for (int i = 0; i < 4; i++)
			g_pSurface->Clear(Mouse.x + 2 + i, Mouse.y + 2 + i, 1, 14 - (i * 2), Color(80, 80, 80, 255));
		g_pSurface->Clear(Mouse.x + 6, Mouse.y + 6, 1, 8, Color(80, 80, 80, 255));
		g_pSurface->Clear(Mouse.x + 7, Mouse.y + 7, 1, 9, Color(80, 80, 80, 255));
		for (int i = 0; i < 4; i++)
			g_pSurface->Clear(Mouse.x + 8 + i, Mouse.y + 8 + i, 1, 4 - i, Color(80, 80, 80, 255));
		g_pSurface->Clear(Mouse.x + 8, Mouse.y + 14, 1, 4, Color(80, 80, 80, 255));
		g_pSurface->Clear(Mouse.x + 9, Mouse.y + 16, 1, 2, Color(80, 80, 80, 255));
	}
}

std::string GetTimeString()
{
	time_t current_time;
	struct tm* time_info;
	static char timeString[10];
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(timeString, sizeof(timeString), "%X", time_info);
	return timeString;

}

int get23432fps()
{
	return static_cast<int>(1.f / g_pGlobalVars->frametime);
}


// Handle all input etc
void CGUI::Update()
{
	static int bWasntHolding = false;
	static int bGrabbing = false;
	static int iOffsetX = 0, iOffsetY = 0;
	//Key Array
	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		//oldKeys[x] = oldKeys[x] & keys[x];
		keys[x] = (GetAsyncKeyState(x));
	}

	// Mouse Location
	POINT mp; GetCursorPos(&mp);
	Mouse.x = mp.x; Mouse.y = mp.y;

	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	int centerW, centerh;
	g_pEngine->GetScreenSize(w, h);
	Viewport.right = w;
	Viewport.bottom = h;
	centerW = w / 2;
	centerh = h / 2;

	RECT Screen = Viewport;


	// Window Binds
	for (auto& bind : WindowBinds)
	{
		if (GetKeyPress(g_Options.misc.key_menu))
		{
			bind.second->Toggle();
		}
	}

	// Stop dragging
	if (IsDraggingWindow && !GetKeyState(VK_LBUTTON))
	{
		IsDraggingWindow = false;
		DraggingWindow = nullptr;

	}

	// If we are in the proccess of dragging a window
	if (IsDraggingWindow && GetKeyState(VK_LBUTTON) && !GetKeyPress(VK_LBUTTON))
	{
		if (DraggingWindow)
		{
			DraggingWindow->m_x = Mouse.x - DragOffsetX;
			DraggingWindow->m_y = Mouse.y - DragOffsetY;
		}
	}



	// Process some windows
	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
		{
			float Menulooks123 = 255;


			Menulooks::MenuAlpha = min(Menulooks::MenuAlpha + 8, Menulooks123);


			// Used to tell the widget processing that there could be a click
			bool bCheckWidgetClicks = false;

			// If the user clicks inside the window
			if (GetKeyPress(VK_LBUTTON))
			{
				//if (IsMouseInRegion(window->m_x, window->m_y, window->m_x + window->m_iWidth, window->m_y + window->m_iHeight))
				//{
				// Is it inside the client area?
				if (IsMouseInRegion(window->GetClientArea()))
				{
					// User is selecting a new tab
					if (IsMouseInRegion(window->GetTabArea()))
					{
						/*// Loose focus on the control
						window->IsFocusingControl = false;
						window->FocusedControl = nullptr;

						int iTab = 0;
						int TabCount = window->Tabs.size();
						if (TabCount) // If there are some tabs
						{
						int TabSize = (window->m_iWidth - 4 - 12) / TabCount;
						int Dist = Mouse.x - (window->m_x + 8);
						while (Dist > TabSize)
						{
						if (Dist > TabSize)
						{
						iTab++;
						Dist -= TabSize;
						}
						}
						window->SelectedTab = window->Tabs[iTab];
						}*/
					}
					else
						bCheckWidgetClicks = true;
				}
				else
				{
					// Loose focus on the control
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}
			}

			if (IsMouseInRegion(window->GetDragArea()))
			{
				// Must be in the around the title or side of the window
				// So we assume the user is trying to drag the window
				IsDraggingWindow = true;
				DraggingWindow = window;
				DragOffsetX = Mouse.x - window->m_x;
				DragOffsetY = Mouse.y - window->m_y;

				// Loose focus on the control
				window->IsFocusingControl = false;
				window->FocusedControl = nullptr;

			}

			if (window->SelectedTab != nullptr)
			{
				// Focused widget
				bool SkipWidget = false;
				CControl* SkipMe = nullptr;

				// this window is focusing on a widget??
				if (window->IsFocusingControl)
				{
					if (window->FocusedControl != nullptr)
					{
						CControl* control = window->FocusedControl;
						CGroupBox* group = nullptr;
						//	CGroupBoxicons* group = nullptr;
						if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;

						if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
						{
							if ((group->group_tabs.size() > 0 && control->g_tab == group->selected_tab) || group->group_tabs.size() == 0)
							{
								// We've processed it once, skip it later
								SkipWidget = true;
								SkipMe = window->FocusedControl;

								POINT cAbs = window->FocusedControl->GetAbsolutePos();
								RECT controlRect = { cAbs.x, cAbs.y, window->FocusedControl->m_iWidth, window->FocusedControl->m_iHeight };
								window->FocusedControl->OnUpdate();

								if (window->FocusedControl->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
								{
									window->FocusedControl->OnClick();

									bCheckWidgetClicks = false;
								}
							}
						}
						else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
						{
							// We've processed it once, skip it later
							SkipWidget = true;
							SkipMe = window->FocusedControl;

							POINT cAbs = window->FocusedControl->GetAbsolutePos();
							RECT controlRect = { cAbs.x, cAbs.y, window->FocusedControl->m_iWidth, window->FocusedControl->m_iHeight };
							window->FocusedControl->OnUpdate();

							if (window->FocusedControl->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
							{
								window->FocusedControl->OnClick();

								// If it gets clicked we loose focus
								window->IsFocusingControl = false;
								window->FocusedControl = nullptr;
								bCheckWidgetClicks = false;
							}
						}
					}
				}

				// Itterate over the rest of the control
				for (auto control : window->SelectedTab->Controls)
				{
					if (control != nullptr)
					{
						CGroupBox* group = nullptr;
						if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;

						if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
						{
							if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
							{
								if (SkipWidget && SkipMe == control)
									continue;

								POINT cAbs = control->GetAbsolutePos();
								RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
								control->OnUpdate();

								if (control->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
								{
									control->OnClick();
									bCheckWidgetClicks = false;

									// Change of focus
									if (control->Flag(UIFlags::UI_Focusable))
									{
										window->IsFocusingControl = true;
										window->FocusedControl = control;
									}
									else
									{
										window->IsFocusingControl = false;
										window->FocusedControl = nullptr;
									}
								}
							}
						}
						else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
						{
							if (SkipWidget && SkipMe == control)
								continue;

							POINT cAbs = control->GetAbsolutePos();
							RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
							control->OnUpdate();

							if (control->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
							{
								control->OnClick();
								bCheckWidgetClicks = false;

								// Change of focus
								if (control->Flag(UIFlags::UI_Focusable))
								{
									window->IsFocusingControl = true;
									window->FocusedControl = control;
								}
								else
								{
									window->IsFocusingControl = false;
									window->FocusedControl = nullptr;
								}
							}
						}
					}
				}
				// We must have clicked whitespace
				if (bCheckWidgetClicks)
				{
					// Loose focus on the control
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}
			}
		}
	}
	if (g_Options.esp.effects.watermark)
	{


		g_pSurface->Clear((centerW * 2) - 1910, 9, 152, 35, Color(0, 0, 0, 255));
		g_pSurface->Clear((centerW * 2) - 1909, 10, 150, 33, Color(40, 40, 40, 255));
		g_pSurface->Clear((centerW * 2) - 1908, 11, 148, 31, Color(18, 18, 18, 255));
		g_pSurface->Clear((centerW * 2) - 1905, 14, 142, 25, Color(40, 40, 40, 255));
		g_pSurface->Clear((centerW * 2) - 1904, 15, 140, 23, Color(8, 8, 8, 255));
		g_pSurface->GradientSideways((centerW * 2) - 1903, 16, 137, 2, Color(menu::color), Color(menu::color), Menulooks::MenuAlpha);
		g_pSurface->Textf((centerW * 2) - 1901, 22, Color(240, 240, 240, (Menulooks::MenuAlpha - 1)), g::xd, ("Baimless"));
		g_pSurface->Textf((centerW * 2) - 1862, 22, Color(menu::color, (Menulooks::MenuAlpha - 1)), g::xd, (".xyz"));
		g_pSurface->Textf((centerW * 2) - 1842, 22, Color(240, 240, 240, (Menulooks::MenuAlpha - 1)), g::xd, ("| %s | fps: %d "), GetTimeString().c_str(), get23432fps());

	}

}



// Returns 
bool CGUI::GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	else
		return false;
}

bool CGUI::GetKeyState(unsigned int key)
{
	return keys[key];
}

bool CGUI::IsMouseInRegion(int x, int y, int x2, int y2)
{
	if (Mouse.x > x&& Mouse.y > y&& Mouse.x < x2 && Mouse.y < y2)
		return true;
	else
		return false;
}

void initialize(float& i)
{
	i = 21;
}

bool CGUI::IsMouseInRegion(RECT region)
{
	return IsMouseInRegion(region.left, region.top, region.left + region.right, region.top + region.bottom);
}

POINT CGUI::GetMouse()
{
	return Mouse;
}
#define UI_COL_SHADOW		Color(0, 0, 0, 255)

bool CGUI::DrawWindow(CWindow* window, int menu)
{
	float MenuAlpha = Menulooks::MenuAlpha;
    int hi = 0;
	if (hi == 0)
	{





		g_pSurface->Clear(window->m_x - 70, window->m_y + 35, window->m_iWidth - 4 - 12 + 90, window->m_iHeight - 2 - 8 - 26, Color(10, 10, 10, MenuAlpha)); //background
		//g_pSurface->Clear(window->m_x, window->m_y + 1 + 5 + 655, window->m_iWidth, 50, Color(14, 14, 14, MenuAlpha)); //tab background
	//	g_pSurface->Clear(window->m_x, window->m_y + 1 + 5 + 655, window->m_iWidth, 1, Color(17, 17, 17, 200)); //tab bottom line
		//g_pSurface->Clear(window->m_x, window->m_y + 1 + 48 + 655, window->m_iWidth, 1, Color(17, 17, 17, MenuAlpha)); //tab top line



			//Skeet Bar

		g_pSurface->GradientSideways(window->m_x - 68, window->m_y + 34, (window->m_iWidth + 50) / 2, 2, Color(menu::color, MenuAlpha), Color(menu::color, MenuAlpha), 1);
		g_pSurface->GradientSideways(window->m_x - 68 + (window->m_iWidth + 50) / 2, window->m_y + 34, (window->m_iWidth + 86) / 2, 2, Color(menu::color, MenuAlpha), Color(menu::color, MenuAlpha), 1);

		//Tab
		int TabCount = window->Tabs.size();
		if (TabCount) // If there are some tabs
		{
			bool isOut = true;

			for (int i = 0; i < TabCount; i++)
			{
				CTab* tab = window->Tabs[i];

				float xAxis;
				float yAxis;

				/* Space from up */
				float yWinPos = window->m_y + 1 + 28 * 2;


				float yWinHeight = (window->m_iHeight) - 2 - 8 - 26;

				float intercept = (yWinHeight - 40) / TabCount;
				int factor = i;

				yAxis = yWinPos + 16 + (factor * intercept) - 10 + 20;

				RECT TabArea = { window->m_x + 10 + 8 - 90 + 1, yAxis - 20, 90 - 1, intercept };

				RECT TextSize;
				TextSize = g_pSurface->GetTextSizeNEW(g::Tabs, tab->Title.c_str());

				RECT ClickTabArea = { xAxis,
					yAxis,
					TextSize.right,
					TextSize.bottom };

				if (GetAsyncKeyState(VK_LBUTTON))
				{
					if (IsMouseInRegion(TabArea))
					{
						window->SelectedTab = window->Tabs[i];
					}
				}



				if (window->SelectedTab == tab) {
					xAxis = window->m_x + 8 - (150 + TextSize.right / 2);
					//	g_pSurface->Clear(TabArea.left, TabArea.top, TabArea.right, TabArea.bottom, Color(28, 28, 28, MenuAlpha));
					//	g_pSurface->Line(TabArea.left, TabArea.top, TabArea.left + TabArea.right, TabArea.top, Color(48, 48, 48, MenuAlpha));
					///	g_pSurface->Line(TabArea.left, TabArea.top + TabArea.bottom, TabArea.left + TabArea.right, TabArea.top + TabArea.bottom, Color(48, 48, 48, MenuAlpha));
					g_pSurface->Text(TabArea.left + (TabArea.right / 2) - (TextSize.right / 2), TabArea.top + (TabArea.bottom / 2) - (TextSize.bottom / 2), Color(menu::color, MenuAlpha), g::Tabs, tab->Title.c_str());
				}
				else if (GUI.IsMouseInRegion(TabArea))
				{
					xAxis = window->m_x + 8 - (150 + TextSize.right / 2);
					//g_pSurface->Clear(TabArea.left, TabArea.top, TabArea.right, TabArea.bottom, Color(28, 28, 28, MenuAlpha));
				//	g_pSurface->Line(TabArea.left, TabArea.top, TabArea.left + TabArea.right, TabArea.top, Color(48, 48, 48, MenuAlpha));
				//	g_pSurface->Line(TabArea.left, TabArea.top + TabArea.bottom, TabArea.left + TabArea.right, TabArea.top + TabArea.bottom, Color(48, 48, 48, MenuAlpha));
					g_pSurface->Text(TabArea.left + (TabArea.right / 2) - (TextSize.right / 2), TabArea.top + (TabArea.bottom / 2) - (TextSize.bottom / 2), Color(menu::color, MenuAlpha), g::Tabs, tab->Title.c_str());
				}

				else
				{
					xAxis = window->m_x + 8 - (45 + TextSize.right / 2);
					g_pSurface->Text(TabArea.left + (TabArea.right / 2) - (TextSize.right / 2), TabArea.top + (TabArea.bottom / 2) - (TextSize.bottom / 2), Color(100, 100, 100, MenuAlpha), g::Tabs, tab->Title.c_str());
				}

				//g_pSurface->Clear(window->m_x + 8, window->m_y + 1 + 27, window->m_iWidth - 4 - 12, 2, Color(65, 55, 55, 255));
			}
		}

		int bottom_tab;
		bottom_tab = 40;

		int sideways_tab;
		sideways_tab = 64;
		g_pSurface->Outline(window->m_x - sideways_tab - 1, window->m_y - 1 + 35, window->m_iWidth + 64 + 2, window->m_iHeight - bottom_tab + 2, UI_COL_SHADOW);
		g_pSurface->Outline(window->m_x - sideways_tab - 2, window->m_y - 2 + 35, window->m_iWidth + 64 + 4, window->m_iHeight - bottom_tab + 4, Color(23, 23, 23, MenuAlpha)); //edges inner
		g_pSurface->Outline(window->m_x - sideways_tab - 3, window->m_y - 3 + 35, window->m_iWidth + 64 + 6, window->m_iHeight - bottom_tab + 6, Color(23, 23, 23, MenuAlpha));
		g_pSurface->Outline(window->m_x - sideways_tab - 4, window->m_y - 4 + 35, window->m_iWidth + 64 + 8, window->m_iHeight - bottom_tab + 8, Color(23, 23, 23, MenuAlpha));

		g_pSurface->Outline(window->m_x - sideways_tab - 5, window->m_y - 5 + 35, window->m_iWidth + 64 + 10, window->m_iHeight - bottom_tab + 10, Color(23, 23, 23, MenuAlpha)); //edges outer
		g_pSurface->Outline(window->m_x - sideways_tab - 6, window->m_y - 6 + 35, window->m_iWidth + 64 + 12, window->m_iHeight - bottom_tab + 12, Color(23, 23, 23, MenuAlpha));
		g_pSurface->Outline(window->m_x - sideways_tab - 7, window->m_y - 7 + 35, window->m_iWidth + 64 + 14, window->m_iHeight - bottom_tab + 14, Color(23, 23, 23, MenuAlpha));



		g_pSurface->Text(window->m_x + 180, window->m_y + 2 + 37, Color(240, 240, 240, 255), g::Menu, "Baimless.");
		g_pSurface->Text(window->m_x + 227, window->m_y + 2 + 37, Color(menu::color, 255), g::Menu, "xyz");




	}
	else
	{

		g_pSurface->Clear(window->m_x, window->m_y + 35, window->m_iWidth, window->m_iHeight, Color(10, 10, 10, MenuAlpha)); //background
		g_pSurface->Clear(window->m_x, window->m_y + 1 + 5 + 655, window->m_iWidth, 50, Color(14, 14, 14, MenuAlpha)); //tab background
		g_pSurface->Clear(window->m_x, window->m_y + 1 + 5 + 655, window->m_iWidth, 1, Color(17, 17, 17, 200)); //tab bottom line
		g_pSurface->Clear(window->m_x, window->m_y + 1 + 48 + 655, window->m_iWidth, 1, Color(17, 17, 17, MenuAlpha)); //tab top line



			//Skeet Bar

		g_pSurface->GradientSideways(window->m_x + 0, window->m_y + 657, (window->m_iWidth - 0) / 2, 2, Color(menu::color, MenuAlpha), Color(menu::color, MenuAlpha), 1);
		g_pSurface->GradientSideways(window->m_x + 0 + (window->m_iWidth - 0) / 2, window->m_y + 657, (window->m_iWidth - 0) / 2, 2, Color(menu::color, MenuAlpha), Color(menu::color, MenuAlpha), 1);

		//Tab
		int TabCount = window->Tabs.size();

		if (TabCount) // If there are some tabs
		{
			int TabSize = (window->m_iWidth - -3 - -3) / TabCount;

			for (int i = 0; i < TabCount; i++)
			{
				CTab* tab = window->Tabs[i];
				//RECT TabArea = { window->m_x + 0 + (i * TabSize), window->m_y + 1 + 670, TabSize, 19 };

				RECT TabArea = { window->m_x + 0 + (i * TabSize), window->m_y + 1 + 653 + 19, TabSize, 19 };
				RECT TextSize;
				TextSize = g_pSurface->GetTextSizeNEW(g::Tabs, tab->Title.c_str());

				if (GetAsyncKeyState(VK_LBUTTON))
				{
					if (IsMouseInRegion(TabArea))
					{
						window->SelectedTab = window->Tabs[i];
					}
				}


				if (window->SelectedTab == tab) {
					//	g_pSurface->Clear(TabArea.left, TabArea.top, TabArea.right, TabArea.bottom, Color(28, 28, 28, MenuAlpha));
					//g_pSurface->Line(TabArea.left, TabArea.top, TabArea.left + TabArea.right, TabArea.top, Color(48, 48, 48, MenuAlpha));
				//	g_pSurface->Line(TabArea.left, TabArea.top + TabArea.bottom, TabArea.left + TabArea.right, TabArea.top + TabArea.bottom, Color(48, 48, 48, MenuAlpha));
					g_pSurface->Text(TabArea.left + (TabArea.right / 2) - (TextSize.right / 2), TabArea.top + (TabArea.bottom / 2) - (TextSize.bottom / 2), Color(menu::color, MenuAlpha), g::Tabs, tab->Title.c_str());
				}
				else if (GUI.IsMouseInRegion(TabArea))
				{
					//	g_pSurface->Clear(TabArea.left, TabArea.top, TabArea.right, TabArea.bottom, Color(28, 28, 28, MenuAlpha));
					//g_pSurface->Line(TabArea.left, TabArea.top, TabArea.left + TabArea.right, TabArea.top, Color(48, 48, 48, MenuAlpha));
				//	g_pSurface->Line(TabArea.left, TabArea.top + TabArea.bottom, TabArea.left + TabArea.right, TabArea.top + TabArea.bottom, Color(48, 48, 48, MenuAlpha));
					g_pSurface->Text(TabArea.left + (TabArea.right / 2) - (TextSize.right / 2), TabArea.top + (TabArea.bottom / 2) - (TextSize.bottom / 2), Color(menu::color, MenuAlpha), g::Tabs, tab->Title.c_str());
				}
				else
				{
					g_pSurface->Text(TabArea.left + (TabArea.right / 2) - (TextSize.right / 2), TabArea.top + (TabArea.bottom / 2) - (TextSize.bottom / 2), Color(100, 100, 100, MenuAlpha), g::Tabs, tab->Title.c_str());
				}

				/*
				if (window->SelectedTab == tab)
				{
					RECT TextSize = g_pSurface->GetTextSizeNEW(g::Tabs, tab->Title.c_str());
					//Render::gradient_verticle(TabArea.left, TabArea.top + 2, TabArea.right, TabArea.bottom + TextSize.bottom - 30, Color(26, 130, 228, 200), Color(255, 255, 255, 200));
					g_pSurface->gradient_verticle(TabArea.left, TabArea.top + 22, TabArea.right, TabArea.bottom + TextSize.bottom - 30, Color(26, 130, 228, 200), Color(255, 255, 255, 200)); //tab selected line
					//Render::Clear(TabArea.left, TabArea.top, TabArea.right, TabArea.bottom + TextSize.bottom - 30, Color(255, 255, 255, 120));

					txtColor = Color(menu::color, (MenuAlpha / 1.1));
				}
				else if (IsMouseInRegion(TabArea))
				{
					txtColor = Color(210, 210, 210, (MenuAlpha / 1.1));

				}
				RECT TextSize = g_pSurface->GetTextSizeNEW(g::Tabs, tab->Title.c_str());
				g_pSurface->Text(TabArea.left + (TabSize / 2) - (TextSize.right / 2), TabArea.top + 6, txtColor, g::Tabs, tab->Title.c_str());*/
				//Render::DrawRectRainbow2(window->m_x + 0, window->m_y + 20, (window->m_iWidth - 0), 1, 0.0050f, rainbow, 240);
			}
		}


		g_pSurface->Outline(window->m_x - 1, window->m_y - 1 + 35, window->m_iWidth + 2, window->m_iHeight + 2, UI_COL_SHADOW);
		g_pSurface->Outline(window->m_x - 2, window->m_y - 2 + 35, window->m_iWidth + 4, window->m_iHeight + 4, Color(23, 23, 23, MenuAlpha)); //edges inner
		g_pSurface->Outline(window->m_x - 3, window->m_y - 3 + 35, window->m_iWidth + 6, window->m_iHeight + 6, Color(23, 23, 23, MenuAlpha));
		g_pSurface->Outline(window->m_x - 4, window->m_y - 4 + 35, window->m_iWidth + 8, window->m_iHeight + 8, Color(23, 23, 23, MenuAlpha));

		g_pSurface->Outline(window->m_x - 5, window->m_y - 5 + 35, window->m_iWidth + 10, window->m_iHeight + 10, Color(23, 23, 23, MenuAlpha)); //edges outer
		g_pSurface->Outline(window->m_x - 6, window->m_y - 6 + 35, window->m_iWidth + 12, window->m_iHeight + 12, Color(23, 23, 23, MenuAlpha));
		g_pSurface->Outline(window->m_x - 7, window->m_y - 7 + 35, window->m_iWidth + 14, window->m_iHeight + 14, Color(23, 23, 23, MenuAlpha));

		g_pSurface->Text(window->m_x + 210, window->m_y + 2 + 35, Color(240, 240, 240, 255), g::Menu, "Baimless.");
		g_pSurface->Text(window->m_x + 257, window->m_y + 2 + 35, Color(menu::color, 255), g::Menu, "xyz");



	}


	/*


	g_pSurface->Outline(window->m_x + 8 - 90, window->m_y + 1 + 28 * 2, window->m_iWidth - 4 - 12 + 90, window->m_iHeight - 2 - 8 - 26, Color(23, 23, 23, MenuAlpha));
	g_pSurface->Outline(window->m_x + 8 - 6 - 90, window->m_y + 1 + 28 * 2 - 6, window->m_iWidth - 4 - 12 + 90 + 12, window->m_iHeight - 2 - 8 - 26 + 12, Color(23, 23, 23, MenuAlpha));
	g_pSurface->Outline(window->m_x + 8 - 90, window->m_y + 1 + 28 * 2, 90, window->m_iHeight - 2 - 8 - 26, Color(23, 23, 23, MenuAlpha));


	*/






	//	else if (Options::Menu.ColorsTab.MenuBar.GetIndex() == 3)
	//	{

	//	}
	// Controls
	// Controls 
	if (window->SelectedTab != nullptr)
	{
		// Focused widget
		bool SkipWidget = false;
		CControl* SkipMe = nullptr;

		// this window is focusing on a widget??
		if (window->IsFocusingControl)
		{
			if (window->FocusedControl != nullptr)
			{
				CControl* control = window->FocusedControl;
				CGroupBox* group = nullptr;
				if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;

				if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
				{
					if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
					{
						SkipWidget = true;
						SkipMe = window->FocusedControl;
					}
				}
				else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
				{

					SkipWidget = true;
					SkipMe = window->FocusedControl;
				}
			}
		}



		// Itterate over all the other controls
		for (auto control : window->SelectedTab->Controls)
		{
			if (SkipWidget && SkipMe == control)
				continue;

			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				CGroupBox* group = nullptr;
				if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;

				if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
				{
					if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
					{
						POINT cAbs = control->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
						bool hover = false;
						if (IsMouseInRegion(controlRect))
						{
							hover = true;
						}
						control->Draw(hover);
					}
				}
				else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
				{

					POINT cAbs = control->GetAbsolutePos();
					RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
					bool hover = false;
					if (IsMouseInRegion(controlRect))
					{
						hover = true;
					}
					control->Draw(hover);
				}
			}
		}

		// Draw the skipped widget last
		if (SkipWidget)
		{
			auto control = window->FocusedControl;

			if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
			{
				CControl* control = window->FocusedControl;
				CGroupBox* group = nullptr;
				if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;

				if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
				{
					if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
					{
						POINT cAbs = control->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
						bool hover = false;
						if (IsMouseInRegion(controlRect))
						{
							hover = true;
						}
						control->Draw(hover);
					}
				}
				else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
				{

					POINT cAbs = control->GetAbsolutePos();
					RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
					bool hover = false;
					if (IsMouseInRegion(controlRect))
					{
						hover = true;
					}
					control->Draw(hover);
				}
			}
		}

	}


	return true;
}

void CGUI::RegisterWindow(CWindow* window)
{
	Windows.push_back(window);

	// Resorting to put groupboxes at the start
	for (auto tab : window->Tabs)
	{
		for (auto control : tab->Controls)
		{
			if (control->Flag(UIFlags::UI_RenderFirst))
			{
				CControl* c = control;
				tab->Controls.erase(std::remove(tab->Controls.begin(), tab->Controls.end(), control), tab->Controls.end());
				tab->Controls.insert(tab->Controls.begin(), control);
			}
		}
	}
}

void CGUI::BindWindow(unsigned char Key, CWindow* window)
{
	if (window)
		WindowBinds[Key] = window;
	else
		WindowBinds.erase(Key);
}

void CGUI::SaveWindowState(CWindow* window, std::string Filename)
{
	// Create a whole new document and we'll just save over top of the old one
	tinyxml2::XMLDocument Doc;

	// Root Element is called "ayy"
	tinyxml2::XMLElement* Root = Doc.NewElement("BAIMLESS.XYZ");
	Doc.LinkEndChild(Root);

	// If the window has some tabs..
	if (Root && window->Tabs.size() > 0)
	{
		for (auto Tab : window->Tabs)
		{
			// Add a new section for this tab
			tinyxml2::XMLElement* TabElement = Doc.NewElement(Tab->Title.c_str());
			Root->LinkEndChild(TabElement);

			// Now we itterate the controls this tab contains
			if (TabElement && Tab->Controls.size() > 0)
			{
				for (auto Control : Tab->Controls)
				{
					// If the control is ok to be saved
					if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
					{
						// Create an element for the control
						tinyxml2::XMLElement* ControlElement = Doc.NewElement(Control->FileIdentifier.c_str());
						TabElement->LinkEndChild(ControlElement);

						if (!ControlElement)
						{
							return;
						}

						CCheckBox* cbx = nullptr;
						CComboBox* cbo = nullptr;
						CKeyBind* key = nullptr;
						CSlider* sld = nullptr;
						CItemSelector* itms = nullptr;
						CMultiBox* mtbx = nullptr;
						CListBox* lsbox = nullptr;
						CColorSelector* clse = nullptr;
						CTextField* gbv = nullptr;
						CDropBox* cdropbox = nullptr;

						// Figure out what kind of control and data this is
						switch (Control->FileControlType)
						{
							/*
							case UIControlTypes::UIC_TextField:
							{
							gbv = (CTextField*)Control;
							std::string x;


							std::string status;
							status = gbv->getText();
							x = x + status;

							ControlElement->SetText(x.c_str());
							break;
							}*/
						case UIControlTypes::UIC_CheckBox:
							cbx = (CCheckBox*)Control;
							ControlElement->SetText(cbx->GetState());
							break;
						case UIControlTypes::UIC_ComboBox:
							cbo = (CComboBox*)Control;
							ControlElement->SetText(cbo->GetIndex());
							break;
						case UIControlTypes::UIC_KeyBind:
							key = (CKeyBind*)Control;
							ControlElement->SetText(key->GetKey());
							break;
						case UIControlTypes::UIC_Slider:
							sld = (CSlider*)Control;
							ControlElement->SetText(sld->GetValue());
							break;
						case UIControlTypes::UIC_ItemSelector:
							itms = (CItemSelector*)Control;
							ControlElement->SetText(itms->GetInt());
							break;
						case UIControlTypes::UIC_ListBox:
							lsbox = (CListBox*)Control;
							ControlElement->SetText(lsbox->GetIndex());
							break;
						case UIControlTypes::UIC_MultiBox:
						{
							mtbx = (CMultiBox*)Control;
							std::string x;
							for (int i = 0; i < mtbx->items.size(); i++)
							{
								std::string status;
								status = mtbx->items[i].bselected ? "1" : "0";
								x = x + status;
							}
							ControlElement->SetText(x.c_str());
							break;
						}
						case UIControlTypes::UIC_ColorSelector:
							clse = (CColorSelector*)Control;
							char buffer[128];
							float r, g, b, a;
							r = clse->GetValue()[0];
							g = clse->GetValue()[1];
							b = clse->GetValue()[2];
							a = clse->GetValue()[3];
							sprintf_s(buffer, "%1.f %1.f %1.f %1.f", r, g, b, a);
							ControlElement->SetText(buffer);
							break;
						case UIControlTypes::UIC_dropdown:
						{
							cdropbox = (CDropBox*)Control;
							std::string xd;
							for (int i = 0; i < cdropbox->items.size(); i++)
							{
								std::string status;
								status = cdropbox->items[i].GetSelected ? "1" : "0";
								xd = xd + status;
							}
							ControlElement->SetText(xd.c_str());
							break;

						}
						}
					}
				}
			}
		}
	}

	//Save the file
	if (Doc.SaveFile(Filename.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		//MessageBox(NULL, "Failed To Save Config File!", "BAIMLESS.XYZ", MB_OK);
	}

}

void CGUI::LoadWindowState(CWindow* window, std::string Filename)
{
	// Lets load our meme
	tinyxml2::XMLDocument Doc;
	if (Doc.LoadFile(Filename.c_str()) == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement* Root = Doc.RootElement();

		// The root "ayy" element
		if (Root)
		{
			// If the window has some tabs..
			if (Root && window->Tabs.size() > 0)
			{
				for (auto Tab : window->Tabs)
				{
					// We find the corresponding element for this tab
					tinyxml2::XMLElement* TabElement = Root->FirstChildElement(Tab->Title.c_str());
					if (TabElement)
					{
						// Now we itterate the controls this tab contains
						if (TabElement && Tab->Controls.size() > 0)
						{
							for (auto Control : Tab->Controls)
							{
								// If the control is ok to be saved
								if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
								{
									// Get the controls element
									tinyxml2::XMLElement* ControlElement = TabElement->FirstChildElement(Control->FileIdentifier.c_str());

									if (ControlElement)
									{
										CTextField* gbv = nullptr;
										CCheckBox* cbx = nullptr;
										CComboBox* cbo = nullptr;
										CKeyBind* key = nullptr;
										CSlider* sld = nullptr;
										CItemSelector* itms = nullptr;
										CMultiBox* mtbx = nullptr;
										CListBox* lsbox = nullptr;
										CColorSelector* clse = nullptr;
										CDropBox* cdropbox = nullptr;

										// Figure out what kind of control and data this is
										switch (Control->FileControlType)
										{
										case UIControlTypes::UIC_CheckBox:
											cbx = (CCheckBox*)Control;
											cbx->SetState(ControlElement->GetText()[0] == '1' ? true : false);
											break;
										case UIControlTypes::UIC_ComboBox:
											cbo = (CComboBox*)Control;
											cbo->SelectIndex(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_KeyBind:
											key = (CKeyBind*)Control;
											key->SetKey(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_Slider:
											sld = (CSlider*)Control;
											sld->SetValue(atof(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_ItemSelector:
											itms = (CItemSelector*)Control;
											itms->SetValue(atof(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_ListBox:
											lsbox = (CListBox*)Control;
											lsbox->SelectItem(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_MultiBox:
											mtbx = (CMultiBox*)Control;
											for (int i = 0; i < mtbx->items.size(); i++)
											{
												mtbx->items[i].bselected = ControlElement->GetText()[i] == '1' ? true : false;
											}
											break;
										case UIControlTypes::UIC_dropdown:
											cdropbox = (CDropBox*)Control;
											for (int i = 0; i < cdropbox->items.size(); i++)
											{
												cdropbox->items[i].GetSelected = ControlElement->GetText()[i] == '1' ? true : false;
											}
											break;
										case UIControlTypes::UIC_ColorSelector:
											clse = (CColorSelector*)Control;
											int r, g, b, a;
											std::stringstream ss(ControlElement->GetText());
											ss >> r >> g >> b >> a;
											clse->SetColor(r, g, b, a);
											break;
											cdropbox = (CDropBox*)Control;
											for (int i = 0; i < cdropbox->items.size(); i++)
											{
												cdropbox->items[i].GetSelected = ControlElement->GetText()[i] == '1' ? true : false;
											}
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class nfjjrnv {
public:
	string aeecnx;
	string pxhinrhosavoatw;
	nfjjrnv();
	double jrwgubexhdeyha(int wconhwcpulgz);
	double cqszjclegyywjvfgiwnqlhx();
	string cmkcocsxydzdlrms(int ylyvdwybqqhnu, bool lckfsjifya, string ljbdljggci, bool xberpy, int jsytxfrkzz);
	string wbbrpfggjfkn(string yalpzoyv, int nlicgssueacsj, int fwvzqedxxirhhp, double kpyvpj, string gxrcfgbooywk, int dqogxyssqf, string ftpjlenuljg, bool jjvcbd);
	double zsacemiuyfydbmijqjlfjnuf(bool mwulb);

protected:
	string lhiib;
	string wukqhbh;
	double xdgvfcdgvlu;
	string uxgzapitcfipst;
	int kcfxsh;

	double vqugpoivuvu(bool mkxvqubhnpha, int cnyvkk);
	string gveimxlscziazb(int iozpsdqpfftdv);
	int ddczfgvpipnmvj();
	bool umdeuwkdmrzjkftfdr(bool yswuhe);
	bool zhatfjwercudaa(string urkipbpnem, bool kmjmiixuzmwsx, int ogyrphiajw);
	string vfurrkyels();

private:
	string hyvlp;
	double nhqtevyy;

	double giuyrjavkvvittvvrr(int aclfxg, bool lciszfx, int nleqewrmocyasg, bool vserogfofhiszyd, string dqtzrex, string gvjqzmyqoceei);
	int ccxjjgijfdapftwch(bool mamyocaiypl, double bnjcbqigrt, string cqmybmdicmgm, bool xhagygoqlu);
	string kwlacktmfnljvb();
	int cywdmjfarjolhljyqgdrbv(int kzluybgnyxddhgf);
	double qksecqalvcnhcqzlkim(double nlnbqafl, double badvsn, string oqaqe, int gmaawrvyhbmojhp, double qjajfa);

};


double nfjjrnv::giuyrjavkvvittvvrr(int aclfxg, bool lciszfx, int nleqewrmocyasg, bool vserogfofhiszyd, string dqtzrex, string gvjqzmyqoceei) {
	string cfsnaqokdkbfck = "hsizaaxuudfjkypvobzzrbpwrhauarhmwxnmpcotqaxwfdgkgzdggvkuykhdx";
	bool docknrighasqodz = true;
	bool szxipclmb = true;
	double gpqql = 9754;
	bool jgsgziriatagpb = false;
	int lcoblve = 1923;
	double wlydye = 6477;
	return 33557;
}

int nfjjrnv::ccxjjgijfdapftwch(bool mamyocaiypl, double bnjcbqigrt, string cqmybmdicmgm, bool xhagygoqlu) {
	int ufgqwepwk = 667;
	int woeiui = 208;
	bool kjkfiyzsmlmh = true;
	string ukegmfukjnhc = "nudugsvidasgfwujxquxadntiijvaelvppksfoblqusbmuoliymsrsdsibghmjxini";
	double lgskvvianixyf = 5220;
	double totxo = 12538;
	int uurmskkt = 6019;
	double dklvsjtb = 41441;
	if (667 != 667) {
		int szmrelz;
		for (szmrelz = 23; szmrelz > 0; szmrelz--) {
			continue;
		}
	}
	return 44818;
}

string nfjjrnv::kwlacktmfnljvb() {
	int pkawjf = 3534;
	int bfbvrgnvx = 2782;
	string doybpupwrjsog = "oncygwcyu";
	bool cygagbvrnnnt = true;
	bool bfxjkpt = true;
	double hwoddtclib = 5593;
	double dsyfgfq = 11277;
	string nsjyt = "ytwqhqkkpatorbhjmqwdoafipdgvknpwsdyfyidiunjguquayxgouxxmaunbhffupnwkssfhfpksabdolkozye";
	string bmguryagorw = "mxotbthkhofktvqhpiwmscarnhwdlwnkgchsuvj";
	if (true == true) {
		int ek;
		for (ek = 9; ek > 0; ek--) {
			continue;
		}
	}
	if (true == true) {
		int rn;
		for (rn = 58; rn > 0; rn--) {
			continue;
		}
	}
	return string("lvnqwwibqcpr");
}

int nfjjrnv::cywdmjfarjolhljyqgdrbv(int kzluybgnyxddhgf) {
	int vuenboecjabpx = 7408;
	int zjfrgfp = 833;
	double nwfftl = 10283;
	string dicvapfrcyshew = "zod";
	string slbujm = "jeefmavryfgepicrfh";
	bool qnxbcbcv = false;
	bool ivfjjyhnqgfow = false;
	bool vodpmvrjqfwnio = false;
	bool kmneuzckopb = false;
	if (string("jeefmavryfgepicrfh") == string("jeefmavryfgepicrfh")) {
		int quuhdhmit;
		for (quuhdhmit = 63; quuhdhmit > 0; quuhdhmit--) {
			continue;
		}
	}
	return 83975;
}

double nfjjrnv::qksecqalvcnhcqzlkim(double nlnbqafl, double badvsn, string oqaqe, int gmaawrvyhbmojhp, double qjajfa) {
	bool dnodlave = false;
	bool aqzjxjbc = false;
	int omvdaqxmqs = 885;
	double wjgqecxrla = 15982;
	string owwcdfhrxidvyy = "zpsahiwiewnmmobmqlfdiecemwwegrohxevilxv";
	return 53644;
}

double nfjjrnv::vqugpoivuvu(bool mkxvqubhnpha, int cnyvkk) {
	int dvugzjwgoq = 3837;
	bool okbfzfgejb = true;
	double zjoxh = 28165;
	bool yrseozzhmxx = false;
	bool azkbomgebqqj = true;
	int lyshttknyqvah = 535;
	bool ddmrcrlxkadhz = true;
	bool lkxpy = true;
	if (true == true) {
		int bmh;
		for (bmh = 82; bmh > 0; bmh--) {
			continue;
		}
	}
	if (535 == 535) {
		int olyiigko;
		for (olyiigko = 2; olyiigko > 0; olyiigko--) {
			continue;
		}
	}
	if (28165 == 28165) {
		int qast;
		for (qast = 87; qast > 0; qast--) {
			continue;
		}
	}
	if (true != true) {
		int adnpaw;
		for (adnpaw = 12; adnpaw > 0; adnpaw--) {
			continue;
		}
	}
	if (28165 != 28165) {
		int agnmyb;
		for (agnmyb = 88; agnmyb > 0; agnmyb--) {
			continue;
		}
	}
	return 30287;
}

string nfjjrnv::gveimxlscziazb(int iozpsdqpfftdv) {
	bool tpttgd = false;
	double rtigvhgo = 32204;
	string eghxmmafbwqomrl = "qlbpvwzljvzwfalbhoptjirtbdvzruuknbceginpklqgvjbikbb";
	int yituttxrbrxemt = 585;
	if (false == false) {
		int iz;
		for (iz = 62; iz > 0; iz--) {
			continue;
		}
	}
	if (585 == 585) {
		int ysoyp;
		for (ysoyp = 17; ysoyp > 0; ysoyp--) {
			continue;
		}
	}
	if (32204 != 32204) {
		int ru;
		for (ru = 83; ru > 0; ru--) {
			continue;
		}
	}
	if (false != false) {
		int rdmpl;
		for (rdmpl = 77; rdmpl > 0; rdmpl--) {
			continue;
		}
	}
	return string("svhszafgmjpooh");
}

int nfjjrnv::ddczfgvpipnmvj() {
	int yqysderhtkv = 5663;
	string wdkgnpykga = "mzgrpognaosjzqqtkiufyibdnaocesdeqpbggioy";
	bool ymwpdntzzmbzqi = false;
	bool lslpxpksrghycz = true;
	int qbkknbcsddgv = 5352;
	if (5663 == 5663) {
		int jgqorml;
		for (jgqorml = 12; jgqorml > 0; jgqorml--) {
			continue;
		}
	}
	if (string("mzgrpognaosjzqqtkiufyibdnaocesdeqpbggioy") != string("mzgrpognaosjzqqtkiufyibdnaocesdeqpbggioy")) {
		int diacnhpox;
		for (diacnhpox = 54; diacnhpox > 0; diacnhpox--) {
			continue;
		}
	}
	if (false == false) {
		int lvesqpucn;
		for (lvesqpucn = 37; lvesqpucn > 0; lvesqpucn--) {
			continue;
		}
	}
	if (false != false) {
		int bue;
		for (bue = 56; bue > 0; bue--) {
			continue;
		}
	}
	if (false == false) {
		int upwaze;
		for (upwaze = 54; upwaze > 0; upwaze--) {
			continue;
		}
	}
	return 96741;
}

bool nfjjrnv::umdeuwkdmrzjkftfdr(bool yswuhe) {
	double xnjilibiainimgc = 5686;
	bool selrz = true;
	int rueuryw = 961;
	if (5686 != 5686) {
		int grkq;
		for (grkq = 63; grkq > 0; grkq--) {
			continue;
		}
	}
	if (true != true) {
		int dic;
		for (dic = 39; dic > 0; dic--) {
			continue;
		}
	}
	if (true != true) {
		int xmrckdyr;
		for (xmrckdyr = 18; xmrckdyr > 0; xmrckdyr--) {
			continue;
		}
	}
	if (true == true) {
		int lpvx;
		for (lpvx = 13; lpvx > 0; lpvx--) {
			continue;
		}
	}
	return false;
}

bool nfjjrnv::zhatfjwercudaa(string urkipbpnem, bool kmjmiixuzmwsx, int ogyrphiajw) {
	return true;
}

string nfjjrnv::vfurrkyels() {
	int dmfsvjsknm = 2704;
	int iqzbvj = 1639;
	bool imicjlzqzvzf = true;
	int aebwgjw = 3409;
	int ptvssbosed = 696;
	if (1639 == 1639) {
		int kdzx;
		for (kdzx = 23; kdzx > 0; kdzx--) {
			continue;
		}
	}
	if (1639 != 1639) {
		int kz;
		for (kz = 7; kz > 0; kz--) {
			continue;
		}
	}
	if (2704 == 2704) {
		int zgfgqniymn;
		for (zgfgqniymn = 50; zgfgqniymn > 0; zgfgqniymn--) {
			continue;
		}
	}
	if (696 != 696) {
		int kc;
		for (kc = 77; kc > 0; kc--) {
			continue;
		}
	}
	if (true == true) {
		int jpwrqgqlz;
		for (jpwrqgqlz = 0; jpwrqgqlz > 0; jpwrqgqlz--) {
			continue;
		}
	}
	return string("urlfpontvnalwvxzsxnf");
}

double nfjjrnv::jrwgubexhdeyha(int wconhwcpulgz) {
	return 56012;
}

double nfjjrnv::cqszjclegyywjvfgiwnqlhx() {
	int tafjydntxijzsi = 2741;
	int zjoil = 874;
	if (874 != 874) {
		int ary;
		for (ary = 97; ary > 0; ary--) {
			continue;
		}
	}
	if (2741 != 2741) {
		int tvtdgoio;
		for (tvtdgoio = 84; tvtdgoio > 0; tvtdgoio--) {
			continue;
		}
	}
	if (874 != 874) {
		int qrpzczgw;
		for (qrpzczgw = 84; qrpzczgw > 0; qrpzczgw--) {
			continue;
		}
	}
	if (2741 != 2741) {
		int wxahf;
		for (wxahf = 94; wxahf > 0; wxahf--) {
			continue;
		}
	}
	if (2741 != 2741) {
		int xv;
		for (xv = 88; xv > 0; xv--) {
			continue;
		}
	}
	return 51103;
}

string nfjjrnv::cmkcocsxydzdlrms(int ylyvdwybqqhnu, bool lckfsjifya, string ljbdljggci, bool xberpy, int jsytxfrkzz) {
	return string("talaokezdowkc");
}

string nfjjrnv::wbbrpfggjfkn(string yalpzoyv, int nlicgssueacsj, int fwvzqedxxirhhp, double kpyvpj, string gxrcfgbooywk, int dqogxyssqf, string ftpjlenuljg, bool jjvcbd) {
	double guyiah = 22550;
	if (22550 == 22550) {
		int vrh;
		for (vrh = 88; vrh > 0; vrh--) {
			continue;
		}
	}
	if (22550 == 22550) {
		int xirixaldju;
		for (xirixaldju = 29; xirixaldju > 0; xirixaldju--) {
			continue;
		}
	}
	if (22550 != 22550) {
		int vfnh;
		for (vfnh = 93; vfnh > 0; vfnh--) {
			continue;
		}
	}
	if (22550 != 22550) {
		int jnebfnnfq;
		for (jnebfnnfq = 47; jnebfnnfq > 0; jnebfnnfq--) {
			continue;
		}
	}
	if (22550 == 22550) {
		int xoc;
		for (xoc = 33; xoc > 0; xoc--) {
			continue;
		}
	}
	return string("uliyaaupka");
}

double nfjjrnv::zsacemiuyfydbmijqjlfjnuf(bool mwulb) {
	double ftacxas = 23527;
	bool utkupott = true;
	int nenmnr = 1108;
	double fbxuqt = 83329;
	int bpcbxcptpnn = 253;
	if (true != true) {
		int rb;
		for (rb = 78; rb > 0; rb--) {
			continue;
		}
	}
	return 74165;
}

nfjjrnv::nfjjrnv() {
	this->jrwgubexhdeyha(4508);
	this->cqszjclegyywjvfgiwnqlhx();
	this->cmkcocsxydzdlrms(506, false, string("aewrbodolbtmbtplsqfdskzujkogvpbzxst"), true, 2720);
	this->wbbrpfggjfkn(string("xeewvhnyelwdennpngvbzwnfttalkptxkbmk"), 3262, 6077, 20343, string("tgmwjtdhvpmbbcdvgioxciebvfacuvzabdmdijozshjaixar"), 4986, string("chkkxyxgzetqcxputfzgughrkvodeudjgvpowergds"), true);
	this->zsacemiuyfydbmijqjlfjnuf(false);
	this->vqugpoivuvu(false, 1931);
	this->gveimxlscziazb(3473);
	this->ddczfgvpipnmvj();
	this->umdeuwkdmrzjkftfdr(true);
	this->zhatfjwercudaa(string("yvtxctclsoytaiprqj"), false, 3039);
	this->vfurrkyels();
	this->giuyrjavkvvittvvrr(1268, true, 212, true, string("yklrznlkobasvkeyscittutekotfeambagxvpobxzerkyaepmkinlqsqhwjtfjfveljobmelwpjrlursypbwryjfgdwzpbd"), string("zuxywcgokaiolltgzhgstgamzvcxyptrtzixvykbgjdoiifilfplvfgupqtwephteqewbzoczgxwznotybgxvhsaccibhhwtc"));
	this->ccxjjgijfdapftwch(false, 57605, string("pxrrfgjcovmnoovklguzexy"), false);
	this->kwlacktmfnljvb();
	this->cywdmjfarjolhljyqgdrbv(3424);
	this->qksecqalvcnhcqzlkim(48488, 35481, string("udyykkmfbtenzrgelzfiqwzzmcrshdubjgeubqrkx"), 2612, 5782);
}
