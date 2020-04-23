#include "newMenu.h"
#include "Utils\Color.h"
#include "SDK\ISurface.h"

#include "Controls.h"
#include "GUI.h"
#include "Utils\GlobalVars.h"
#include <Options.hpp>

float Menulooks12345 = 255;

float Menulooks::MenuAlpha = Menulooks12345;

Color forecolor = Color(menu::color, 255);
#define white Color(255, 255, 255)
#define mixed Color(90, 90, 90)
#define lighter_gray Color(48, 48, 48)
#define light_gray Color(40, 40, 40)
#define gray Color(28, 28, 28)
#define dark_gray Color(21, 21, 19)
#define darker_gray Color(19, 19, 19)
#define black Color(0, 0, 0)



#pragma region Base Control
void CControl::SetPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

void CControl::SetSize(int w, int h)
{
	m_iWidth = w;
	m_iHeight = h;
}

void CControl::GetSize(int& w, int& h)
{
	w = m_iWidth;
	h = m_iHeight;
}

bool CControl::Flag(int f)
{
	if (m_Flags & f)
		return true;
	else
		return false;
}

POINT CControl::GetAbsolutePos()
{
	POINT p;
	RECT client = parent->GetClientArea();
	if (parent)
	{
		p.x = m_x + client.left;
		p.y = m_y + client.top + 29;
	}

	return p;
}

void CControl::SetFileId(std::string fid)
{
	FileIdentifier = fid;
}
#pragma endregion Implementations of the Base control functions

CDropBox::CDropBox()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	m_iHeight = 20;
	FileControlType = UIControlTypes::UIC_dropdown;
}

void CDropBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 20 };
	if (GUI.IsMouseInRegion(Region))
	{
		g_pSurface->GradientV(a.x, a.y, m_iWidth, 20, Color(50, 50, 50, 255), Color(26, 26, 26, 255));
		g_pSurface->Outline(a.x, a.y, m_iWidth, 20, Color(57, 57, 57, 255));
	}
	else
	{
		g_pSurface->GradientV(a.x, a.y, m_iWidth, 20, Color(40, 40, 40, 255), Color(20, 20, 20, 255));
		g_pSurface->Outline(a.x, a.y, m_iWidth, 20, Color(7, 7, 7, 255));
	}
	std::string ss;
	for (auto n = 0; n < items.size(); ++n)
	{
		auto format_len = ss.length() < 12;
		auto first = ss.length() <= 0;
		if ((items[n].GetSelected && format_len))
		{
			if (!first)
			{
				ss.append(", ");
			}

			ss.append(items[n].text);
		}
		else if (!format_len) {
			ss.append("...");
			break;
		}
	}

	if (ss.length() <= 0)
	{
		ss += "...";
	}
	if (items.size() > 0)
	{

		RECT txtSize = g_pSurface->GetTextSize2(ss.c_str(), g::Menu);
		g_pSurface->Text2(a.x + 10, a.y + (Region.bottom / 2) - (txtSize.bottom / 2), ss.c_str(), g::Menu, Color(175, 175, 175, 245));




		if (IsOpen)
		{
			g_pSurface->GradientV(a.x, a.y + 20, m_iWidth, items.size() * 20, Color(40, 40, 40, 255), Color(20, 20, 20, 255));

			for (int i = 0; i < items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 17 + i * 20, m_iWidth, 20 };

				if (GUI.IsMouseInRegion(ItemRegion))
				{
					g_pSurface->GradientV(a.x, a.y + 20 + i * 20, m_iWidth, 20, Color(42, 42, 42, 255), Color(39, 39, 39, 255));
				}
				else
				{
					g_pSurface->GradientV(a.x, a.y + 20 + i * 20, m_iWidth, 20, Color(31, 31, 31, 255), Color(29, 29, 29, 255));
				}

				RECT control_textsize = g_pSurface->GetTextSize2(items[i].text, g::Menu);


				dropdownboxitem item = items[i];

				RECT txtsize = g_pSurface->GetTextSize2(item.text, g::Menu);
				const char* epic = item.text;
				std::string gamers;

				int item_x = a.x + (m_iWidth / 2) - (txtsize.right / 2);
				int item_y = a.y + 19 + (i * 16) - (txtsize.bottom / 2) + 7;


				if (!item.GetSelected)
					g_pSurface->Text2(a.x + 10, a.y + 20 + (i * 20) + 10 - (control_textsize.bottom / 2), item.text, g::Menu, Color(240, 240, 240, 245));
				else {
					g_pSurface->Text2(a.x + 10, a.y + 20 + (i * 20) + 10 - (control_textsize.bottom / 2), item.text, g::Menu, menu::color);
				}



				g_pSurface->Outline(a.x, a.y + 20 + (i * 20), m_iWidth, 20, Color(21, 21, 21, 230));
			}
			g_pSurface->Outline(a.x, a.y + 20, m_iWidth, items.size() * 20, Color(7, 7, 7, 255));
		}
	}
	Vertex_t Verts2[3];
	Verts2[0].mPosition.x = a.x + m_iWidth - 10;
	Verts2[0].mPosition.y = a.y + 9;
	Verts2[1].mPosition.x = a.x + m_iWidth - 5;
	Verts2[1].mPosition.y = a.y + 9;
	Verts2[2].mPosition.x = a.x + m_iWidth - 7.5;
	Verts2[2].mPosition.y = a.y + 12;

	//g_pSurface->Polygon(3, Verts2, menu::color);

}

template <typename T>
const bool Contains(std::vector<T>& Vec, const T& Element) {
	if (std::find(Vec.begin(), Vec.end(), Element) != Vec.end())
		return true;

	return false;
}
void CDropBox::OnUpdate()
{

	if (IsOpen)
	{
		m_iHeight = 20 + 20 * items.size();

		if (parent->GetFocus() != this)
			IsOpen = false;
	}
	else
	{
		m_iHeight = 20;
	}
	std::string gamers;

}

void CDropBox::OnClick()
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 20 };

	if (IsOpen)
	{
		// If we clicked one of the items(Not in the top bar)
		if (!GUI.IsMouseInRegion(Region))
		{
			// Draw the items
			POINT a = GetAbsolutePos();
			for (int i = 0; i < items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 20 + i * 20, m_iWidth, 20 };
				if (GUI.IsMouseInRegion(ItemRegion))
				{
					items[i].GetSelected = !items[i].GetSelected;
				}
			}
		}
		RECT stfu = { a.x, a.y + 20 + 1 * 20 - 40, m_iWidth, 20 }; // -20 for compiler issue
		if (GUI.IsMouseInRegion(stfu))
			IsOpen = false;
		else
			IsOpen = true;
	}
	else
		IsOpen = true;

}

void CDropBox::SetTitle(const char* tl)
{
	title = tl;
}



#pragma region CheckBox
CCheckBox::CCheckBox()
{
	Checked = false;
	bIsSub = false;

	m_Flags = UIFlags::UI_Clickable | UIFlags::UI_Drawable | UIFlags::UI_SaveFile;
	m_iHeight = 9;

	FileControlType = UIControlTypes::UIC_CheckBox;
}

void CCheckBox::SetState(bool s)
{
	Checked = s;
}

bool CCheckBox::GetState()
{
	return Checked;
}

bool CCheckBox::GetIsSub()
{
	return bIsSub;
}

void CCheckBox::SetAsSub(bool t)
{
	bIsSub = t;
}

void CCheckBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	g_pSurface->GradientV(a.x, a.y, 9, 9, Color(50, 50, 50, Menulooks::MenuAlpha), Color(35, 35, 35, 245));
	g_pSurface->Outline(a.x, a.y, 9, 9, Color(2, 2, 2, Menulooks::MenuAlpha));

	int red;
	int green;
	int blue;

	if (Checked)
	{
		red = 191;
		green = 7;
		blue = 0;

		g_pSurface->GradientV(a.x, a.y, 10, 10, Color(menu::color), Color(menu::color));
		g_pSurface->Outline(a.x, a.y, 10, 10, Color(2, 2, 2, Menulooks::MenuAlpha));
	}
}

void CCheckBox::OnUpdate() { m_iHeight = 9; }

void CCheckBox::OnClick()
{
	if (!should_animate)
		Checked = !Checked;
}
#pragma endregion Implementations of the Check Box functions

#pragma region ComboBox2
CComboBox2::CComboBox2()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_ComboBox;
	winWidth = 400;
	winHeight = 400;
}

void CComboBox2::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 20 };

	{
		g_pSurface->Outline(a.x, a.y, m_iWidth, 20, Color(2, 2, 2, Menulooks::MenuAlpha));
		g_pSurface->GradientV(a.x + 2, a.y + 2, m_iWidth - 4, 16, Color(31, 31, 31, Menulooks::MenuAlpha), Color(31, 31, 31, Menulooks::MenuAlpha));
	}

	// Hover for the Top Box
	if (GUI.IsMouseInRegion(Region))

	{
		g_pSurface->GradientV(a.x + 2, a.y + 2, m_iWidth - 4, 16, Color(31, 31, 31, Menulooks::MenuAlpha), Color(31, 31, 31, Menulooks::MenuAlpha));
	}

	// If we have some items
	if (Items.size() > 0)
	{
		// The current item
		RECT txtSize = g_pSurface->GetTextSizeNEW(g::Menu, GetItem().c_str());
		g_pSurface->Text(a.x + 4, a.y + (Region.bottom / 2) - (txtSize.bottom / 2), Color(145, 145, 145, Menulooks::MenuAlpha), g::Menu, GetItem().c_str());

		// If the drop down part is open
		if (IsOpen)
		{
			g_pSurface->GradientV(a.x, a.y + 20, m_iWidth, animation_state - 16, Color(31, 31, 31, Menulooks::MenuAlpha), Color(31, 31, 31, Menulooks::MenuAlpha));
			g_pSurface->Outline(a.x, a.y + 0, m_iWidth, animation_state + 4, Color(2, 2, 2, Menulooks::MenuAlpha));

			// Draw the items
			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 17 + i * 16, m_iWidth, 16 };
				RECT ItemRegion2 = { a.x, a.y + i * 16, m_iWidth, 16 };

				g_pSurface->Text(a.x + 4, a.y + 19 + i * 16 - 1 + 4, Color(145, 145, 145, Menulooks::MenuAlpha), g::Menu, Items[i].c_str());
				g_pSurface->Text(a.x + 4, a.y + 19 + SelectedIndex * 16 - 1 + 4, Color(menu::color), g::Menu, Items[SelectedIndex].c_str());
			}
		}
	}

	if (IsOpen) {
		Vertex_t Verts3[3];
		Verts3[0].mPosition.x = a.x + m_iWidth - 5;
		Verts3[0].mPosition.y = a.y + 11;
		Verts3[1].mPosition.x = a.x + m_iWidth - 10;
		Verts3[1].mPosition.y = a.y + 11;
		Verts3[2].mPosition.x = a.x + m_iWidth - 7.5;
		Verts3[2].mPosition.y = a.y + 8;

		g_pSurface->Polygon(3, Verts3, Color(90, 90, 90, Menulooks::MenuAlpha));
	}
	else {
		Vertex_t Verts2[3];
		Verts2[0].mPosition.x = a.x + m_iWidth - 10;
		Verts2[0].mPosition.y = a.y + 8;
		Verts2[1].mPosition.x = a.x + m_iWidth - 5;
		Verts2[1].mPosition.y = a.y + 8;
		Verts2[2].mPosition.x = a.x + m_iWidth - 7.5;
		Verts2[2].mPosition.y = a.y + 11;

		g_pSurface->Polygon(3, Verts2, Color(85, 85, 85, Menulooks::MenuAlpha));
	}
}

void CComboBox2::AddItem(std::string text)
{
	Items.push_back(text);
	SelectedIndex = 0;
}

void CComboBox2::OnUpdate()
{
	if (IsOpen)
	{
		m_iHeight = 16 + 16 * Items.size();

		if (parent->GetFocus() != this)
			IsOpen = false;

		animation_state += 4;

		if (animation_state == Items.size() * 16 + 20) {
			animation_state -= 4;
		}
	}
	else
	{
		m_iHeight = 16;

		animation_state = 0;
	}
}

void CComboBox2::OnClick()
{

	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 16 };

	if (IsOpen)
	{
		// If we clicked one of the items(Not in the top bar)
		if (!GUI.IsMouseInRegion(Region))
		{
			// Draw the items
			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 16 + i * 16, m_iWidth, 16 };
				if (GUI.IsMouseInRegion(ItemRegion))
				{
					SelectedIndex = i;
				}
			}
		}

		// Close the drop down
		IsOpen = false;
	}
	else
	{
		IsOpen = true;
	}
}

int CComboBox2::GetIndex()
{
	return SelectedIndex;
}

void CComboBox2::SetIndex(int index)
{
	SelectedIndex = index;
}

std::string CComboBox2::GetItem()
{
	if (SelectedIndex >= 0 && SelectedIndex < Items.size())
	{
		return Items[SelectedIndex];
	}

	return "Error";
}

void CComboBox2::SelectIndex(int idx)
{
	if (idx >= 0 && idx < Items.size())
	{
		SelectedIndex = idx;
	}
}

#pragma endregion Implementations of the ComboBox2 functions


#pragma region Label
CLabel::CLabel()
{
	m_Flags = UIFlags::UI_Drawable;
	FileControlType = UIC_Label;
	Text = "";
	FileIdentifier = "";
	m_iHeight = 10;
}

void CLabel::Draw(bool hover)
{
	//Slide walk
	POINT a = GetAbsolutePos();
	if (Text == "Fix leg movement" || Text == "Infinite duck" || Text == "Slide walk" || Text == "On shot anti-aim" || Text == "Remove spread" || Text == "Fake duck" || Text == "Instant bomb explode")
	{
		g_pSurface->Text(a.x, a.y - 1, Text.c_str(), g::Menu, Color(205, 205, 45, Menulooks::MenuAlpha));

	}
	else
	{
		g_pSurface->Text(a.x, a.y - 1, Text.c_str(), g::Menu, Color(205, 205, 205, Menulooks::MenuAlpha));

	}
}

void CLabel::SetText(std::string text)
{
	Text = text;
}

void CLabel::OnUpdate() {}
void CLabel::OnClick() {}
#pragma endregion Implementations of the Label functions

#pragma region CheckBox
CSeperate::CSeperate()
{
	m_Flags = UIFlags::UI_Drawable;

	FileControlType = UIControlTypes::UIC_Seperate;
}

void CSeperate::Draw(bool hover) {}

void CSeperate::OnUpdate() {}

void CSeperate::OnClick() {}

#pragma endregion Implementations of the Check Box functions



#pragma region ButtonLoad
CButtonLoad::CButtonLoad()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable;
	FileControlType == UIControlTypes::UIC_Button;
	Text = "Load";
	m_iHeight = 25;
	CallBack = nullptr;
	FileIdentifier = "Default";
}

void CButtonLoad::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	if (hover)
		g_pSurface->GradientV(a.x, a.y, m_iWidth, m_iHeight, Color(55, 55, 55, Menulooks::MenuAlpha), Color(50, 50, 50, Menulooks::MenuAlpha));
	else
		g_pSurface->GradientV(a.x, a.y, m_iWidth, m_iHeight, Color(45, 45, 45, Menulooks::MenuAlpha), Color(40, 40, 40, Menulooks::MenuAlpha));

	g_pSurface->Outline(a.x, a.y, m_iWidth, m_iHeight, Color(2, 2, 2, Menulooks::MenuAlpha));

	RECT TextSize = g_pSurface->GetTextSizeNEW(g::Menu, Text.c_str());
	int TextX = a.x + (m_iWidth / 2) - (TextSize.right / 2);
	int TextY = a.y + (m_iHeight / 2) - (TextSize.bottom / 2);

	g_pSurface->Text2(TextX, TextY, Text.c_str(), g::Menu, Color(175, 175, 175, 200));
}

void CButtonLoad::SetText(std::string text)
{
	Text = text;
}

void CButtonLoad::SetCallback(CButtonLoad::ButtonCallback_t callback)
{
	CallBack = callback;
}

void CButtonLoad::OnUpdate()
{
	m_iHeight = 25;
}

void CButtonLoad::OnClick()
{
	if (CallBack)
		CallBack();
}
#pragma endregion Implementations of the Button functions


#pragma region ButtonSave
CButtonSave::CButtonSave()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable;
	FileControlType == UIControlTypes::UIC_Button;
	Text = "Save";
	m_iHeight = 25;
	CallBack = nullptr;
	FileIdentifier = "Default";
}

void CButtonSave::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	if (hover)
		g_pSurface->GradientV(a.x, a.y, m_iWidth, m_iHeight, Color(50, 50, 50, Menulooks::MenuAlpha), Color(50, 50, 50, Menulooks::MenuAlpha));
	else
		g_pSurface->GradientV(a.x, a.y, m_iWidth, m_iHeight, Color(40, 40, 40, Menulooks::MenuAlpha), Color(40, 40, 40, Menulooks::MenuAlpha));

	g_pSurface->Outline(a.x, a.y, m_iWidth, m_iHeight, Color(2, 2, 2, Menulooks::MenuAlpha));

	RECT TextSize = g_pSurface->GetTextSizeNEW(g::Menu, Text.c_str());
	int TextX = a.x + (m_iWidth / 2) - (TextSize.right / 2);
	int TextY = a.y + (m_iHeight / 2) - (TextSize.bottom / 2);

	g_pSurface->Text2(TextX, TextY, Text.c_str(), g::Menu, Color(175, 175, 175, 200));
}

void CButtonSave::SetText(std::string text)
{
	Text = text;
}

void CButtonSave::SetCallback(CButtonSave::ButtonCallback_t callback)
{
	CallBack = callback;
}

void CButtonSave::OnUpdate()
{
	m_iHeight = 25;
}

void CButtonSave::OnClick()
{
	if (CallBack)
		CallBack();
}
#pragma endregion Implementations of the Button functions

#pragma region GroupBox
CGroupBox::CGroupBox()
{
	Items = 1;
	last_y = 0;
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_RenderFirst;
	Text = "Default";
	FileIdentifier = "Default";
	FileControlType = UIControlTypes::UIC_GroupBox;
}
void CGroupBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT txtSize = g_pSurface->GetTextSizeNEW(g::Menu, Text.c_str());
	g_pSurface->DrawRect(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(18, 18, 18, 1));
	g_pSurface->Text(a.x + (m_iWidth / 2) - (txtSize.right / 2), a.y - (txtSize.bottom / 2) - 1, Text.c_str(), g::Menu, Color(239, 239, 239, Menulooks::MenuAlpha));
	if (group_tabs.size())
	{
		g_pSurface->Line(a.x + 1, a.y + 8, a.x + m_iWidth, a.y + 8, Color(30, 30, 30, Menulooks::MenuAlpha));
		g_pSurface->Line(a.x + 1, a.y + 38, a.x + m_iWidth, a.y + 38, Color(29, 29, 29, Menulooks::MenuAlpha));
		g_pSurface->DrawRect(a.x + 1, a.y + 9, m_iWidth - 1, 29, Color(10, 10, 10, Menulooks::MenuAlpha));
		g_pSurface->DrawRect(a.x + 1, a.y + 36, m_iWidth - 1, 2, Color(39, 39, 39, Menulooks::MenuAlpha));

		for (int i = 0; i < group_tabs.size(); i++)
		{
			RECT text_size = g_pSurface->get_text_size(group_tabs[i].name.c_str(), g::Menu);

			int width = m_iWidth - 1;

			int tab_length = (width / group_tabs.size());

			int text_position[] = {
				(a.x + (tab_length * (i + 1)) - (tab_length / 2)),
				a.y + 23 - (text_size.bottom / 2)
			};

			RECT tab_area = {
				(a.x + 1) + (tab_length * i),
				a.y + 9,
				tab_length,
				29
			};

			if (GetAsyncKeyState(VK_LBUTTON))
			{
				if (GUI.IsMouseInRegion(tab_area))
				{
					selected_tab = group_tabs[i].id;
				}
			}

			if (selected_tab == group_tabs[i].id)
			{


				if (group_tabs[i].name == "G" || group_tabs[i].name == "W" || group_tabs[i].name == "Z" || group_tabs[i].name == "b" || group_tabs[i].name == "f" || group_tabs[i].name == "L")
				{

					int text_position[] = {
				(a.x - 14 + (tab_length * (i + 1)) - (tab_length / 2)),
				a.y + 23 - (text_size.bottom / 2)
					};

					g_pSurface->DrawRect(tab_area.left, tab_area.top, tab_area.right, tab_area.bottom, Color(menu::color));
					g_pSurface->DrawRect(tab_area.left, tab_area.top, tab_area.right, tab_area.bottom - 2, Color(34, 34, 34, Menulooks::MenuAlpha));
					g_pSurface->Text(text_position[0] - (text_size.right / 2), text_position[1], group_tabs[i].name.c_str(), g::Untitled1, Color(200, 200, 200, 245));
				}
				else
				{
					int text_position[] = {
				(a.x + (tab_length * (i + 1)) - (tab_length / 2)),
				a.y + 23 - (text_size.bottom / 2)
					};

					g_pSurface->DrawRect(tab_area.left, tab_area.top, tab_area.right, tab_area.bottom, Color(menu::color));
					g_pSurface->DrawRect(tab_area.left, tab_area.top, tab_area.right, tab_area.bottom - 2, Color(34, 34, 34, Menulooks::MenuAlpha));
					g_pSurface->Text(text_position[0] - (text_size.right / 2), text_position[1], group_tabs[i].name.c_str(), g::Menu, Color(200, 200, 200, 245));
				}

			}
			else if (selected_tab != group_tabs[i].id)
			{
				if (group_tabs[i].name == "G" || group_tabs[i].name == "W" || group_tabs[i].name == "Z" || group_tabs[i].name == "b" || group_tabs[i].name == "f" || group_tabs[i].name == "L")
				{
					int text_position[] = {
				(a.x - 14 + (tab_length * (i + 1)) - (tab_length / 2)),
				a.y + 23 - (text_size.bottom / 2)
					};

					g_pSurface->Text(text_position[0] - (text_size.right / 2), text_position[1], group_tabs[i].name.c_str(), g::Untitled1, Color(87, 87, 87, Menulooks::MenuAlpha));

				}
				else
				{

					int text_position[] = {
				(a.x + (tab_length * (i + 1)) - (tab_length / 2)),
				a.y + 23 - (text_size.bottom / 2)
					};

					g_pSurface->Text(text_position[0] - (text_size.right / 2), text_position[1], group_tabs[i].name.c_str(), g::Menu, Color(87, 87, 87, Menulooks::MenuAlpha));


				}
			}
		}
	}

	//Top Left
	g_pSurface->Line(a.x, a.y, a.x + (m_iWidth / 2) - (txtSize.right / 2) - 2, a.y, Color(43, 43, 43, Menulooks::MenuAlpha));
	g_pSurface->Line(a.x - 1, a.y - 1, a.x + (m_iWidth / 2) - (txtSize.right / 2) - 1, a.y - 1, Color(0, 0, 0, Menulooks::MenuAlpha));

	//Top Right
	g_pSurface->Line(a.x + (m_iWidth / 2) + (txtSize.right / 2) + 2, a.y, a.x + m_iWidth, a.y, Color(43, 43, 43, Menulooks::MenuAlpha));
	g_pSurface->Line(a.x + (m_iWidth / 2) + (txtSize.right / 2) + 2, a.y - 1, a.x + m_iWidth + 1, a.y - 1, Color(0, 0, 0, Menulooks::MenuAlpha));

	//Left
	g_pSurface->Line(a.x, a.y, a.x, a.y + m_iHeight, Color(44, 44, 44, Menulooks::MenuAlpha));
	g_pSurface->Line(a.x - 1, a.y, a.x - 1, a.y + m_iHeight, Color(0, 0, 0, Menulooks::MenuAlpha));

	//Bottom
	g_pSurface->Line(a.x, a.y + m_iHeight, a.x + m_iWidth, a.y + m_iHeight, Color(43, 43, 43, Menulooks::MenuAlpha));
	g_pSurface->Line(a.x - 1, a.y + m_iHeight + 1, a.x + m_iWidth + 2, a.y + m_iHeight + 1, Color(0, 0, 0, Menulooks::MenuAlpha));

	//Right
	g_pSurface->Line(a.x + m_iWidth, a.y, a.x + m_iWidth, a.y + m_iHeight + 1, Color(43, 43, 43, Menulooks::MenuAlpha));
	g_pSurface->Line(a.x + m_iWidth + 1, a.y, a.x + m_iWidth + 1, a.y + m_iHeight + 1, Color(0, 0, 0, Menulooks::MenuAlpha));
}
void CGroupBox::SetText(std::string text)
{
	Text = text;
}
void CGroupBox::PlaceLabledControl(int g_tab, std::string Label, CTab* Tab, CControl* control) {
	bool has_tabs = group_tabs.size() ? 1 : 0;

	if (control->FileControlType == UIControlTypes::UIC_Seperate)
	{
		int x = m_x + 34;
		int y = m_y + Items * ItemSpacing + iYAdd;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 16;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 1.5), ch);
		Tab->RegisterControl(control);

		iYAdd -= 4;
	}

	if (has_tabs) {
		bool has_reset = false;

		for (int i = 0; i < reset_tabs.size(); i++) {
			if (reset_tabs[i] == g_tab)
				has_reset = true;
		}

		if (!has_reset) {
			initialized = false;
			reset_tabs.push_back(g_tab);
		}
	}

	if (!initialized) {
		Items = 0;
		last_y = has_tabs ? m_y + 48 : m_y + 8;
		initialized = true;
	}

	bool add_label_y = true;
	bool is_checkbox = control->FileControlType == UIControlTypes::UIC_CheckBox;
	bool is_label = control->FileControlType == UIControlTypes::UIC_Label;
	bool is_keybind = control->FileControlType == UIControlTypes::UIC_KeyBind;
	bool is_color = control->FileControlType == UIControlTypes::UIC_ColorSelector;

	int x = m_x + 38;
	int y = last_y;
	int control_width, control_height;
	control->GetSize(control_width, control_height);

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	label->parent_group = this;
	label->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(label);

	if (is_checkbox || is_label || is_color) add_label_y = false;

	if (Label != "" && add_label_y && !is_keybind) {
		RECT label_size = g_pSurface->get_text_size(Label.c_str(), g::Menu);
		last_y += 14;
		y = last_y;
	}

	if (!is_keybind)
		last_control_height = control_height + 7;

	if (is_keybind || (is_color && Label == "")) {
		y -= last_control_height;
		x = m_x + m_iWidth - 36;
	}
	if (is_color && Label != "")
		x = m_x + m_iWidth - 36;
	if (is_checkbox)
		x -= 24;

	control->SetPosition(x, is_checkbox ? y + 1 : y);
	control->SetSize(m_iWidth - (38 * 2), control_height);
	control->parent_group = this;
	control->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(control);

	if (!is_keybind) {
		if (!is_color || is_color && Label != "")
			last_y += control_height + 7;
	}
}

void CGroupBox::AddTab(CGroupTab t)
{
	group_tabs.push_back(t);

	if (selected_tab == 0)
		selected_tab++;
}
void CGroupBox::OnUpdate() {}
void CGroupBox::OnClick() {}
#pragma endregion Implementations of the Group Box functions

#pragma region GroupBox
CGroupBoxicons::CGroupBoxicons()
{
	Items = 1;
	last_y = 0;
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_RenderFirst;
	Text = "Default";
	FileIdentifier = "Default";
	FileControlType = UIControlTypes::UIC_GroupBox;
}
void CGroupBoxicons::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT txtSize = g_pSurface->GetTextSizeNEW(g::Menu, Text.c_str());
	g_pSurface->DrawRect(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(80, 80, 80, 1));
	g_pSurface->Text(a.x + (m_iWidth / 2) - (txtSize.right / 2), a.y - (txtSize.bottom / 2) - 1, Text.c_str(), g::Menu, Color(234, 234, 234, 255));
	if (group_tabs.size())
	{
		g_pSurface->Line(a.x + 1, a.y + 8, a.x + m_iWidth, a.y + 8, Color(38, 38, 38, 255));
		g_pSurface->Line(a.x + 1, a.y + 38, a.x + m_iWidth, a.y + 38, Color(38, 38, 38, 255));
		g_pSurface->DrawRect(a.x + 1, a.y + 9, m_iWidth - 1, 29, Color(11, 11, 9, 255));
		g_pSurface->DrawRect(a.x + 1, a.y + 36, m_iWidth - 1, 2, Color(52, 52, 52, 255));

		for (int i = 0; i < group_tabs.size(); i++)
		{
			RECT text_size = g_pSurface->get_text_size(group_tabs[i].name.c_str(), g::Untitled1);

			int width = m_iWidth - 1;

			int tab_length = (width / group_tabs.size());

			int text_position[] = {
				(a.x + (tab_length * (i + 1)) - (tab_length / 2)),
				a.y + 23 - (text_size.bottom / 2)
			};

			RECT tab_area = {
				(a.x + 1) + (tab_length * i),
				a.y + 9,
				tab_length,
				29
			};

			if (GetAsyncKeyState(VK_LBUTTON))
			{
				if (GUI.IsMouseInRegion(tab_area))
				{
					selected_tab = group_tabs[i].id;
				}
			}

			if (selected_tab == group_tabs[i].id)
			{
				g_pSurface->DrawRect(tab_area.left, tab_area.top, tab_area.right, tab_area.bottom, Color(menu::color));
				g_pSurface->DrawRect(tab_area.left, tab_area.top, tab_area.right, tab_area.bottom - 2, Color(29, 29, 29, 255));
				g_pSurface->Text(text_position[0] - (text_size.right / 2), text_position[1], group_tabs[i].name.c_str(), g::Untitled1, Color(195, 195, 195, 245));
			}
			else if (selected_tab != group_tabs[i].id)
				g_pSurface->Text(text_position[0] - (text_size.right / 2), text_position[1], group_tabs[i].name.c_str(), g::Untitled1, Color(82, 82, 82, 255));
		}
	}

	//Top Left
	g_pSurface->Line(a.x, a.y, a.x + (m_iWidth / 2) - (txtSize.right / 2) - 2, a.y, Color(38, 38, 38, 255));
	g_pSurface->Line(a.x - 1, a.y - 1, a.x + (m_iWidth / 2) - (txtSize.right / 2) - 1, a.y - 1, Color(0, 0, 0, 255));

	//Top Right
	g_pSurface->Line(a.x + (m_iWidth / 2) + (txtSize.right / 2) + 2, a.y, a.x + m_iWidth, a.y, Color(38, 38, 38, 255));
	g_pSurface->Line(a.x + (m_iWidth / 2) + (txtSize.right / 2) + 2, a.y - 1, a.x + m_iWidth + 1, a.y - 1, Color(0, 0, 0, 255));

	//Left
	g_pSurface->Line(a.x, a.y, a.x, a.y + m_iHeight, Color(39, 39, 39, 255));
	g_pSurface->Line(a.x - 1, a.y, a.x - 1, a.y + m_iHeight, Color(0, 0, 0, 255));

	//Bottom
	g_pSurface->Line(a.x, a.y + m_iHeight, a.x + m_iWidth, a.y + m_iHeight, Color(38, 38, 38, 255));
	g_pSurface->Line(a.x - 1, a.y + m_iHeight + 1, a.x + m_iWidth + 2, a.y + m_iHeight + 1, Color(0, 0, 0, 255));

	//Right
	g_pSurface->Line(a.x + m_iWidth, a.y, a.x + m_iWidth, a.y + m_iHeight + 1, Color(38, 38, 38, 255));
	g_pSurface->Line(a.x + m_iWidth + 1, a.y, a.x + m_iWidth + 1, a.y + m_iHeight + 1, Color(0, 0, 0, 255));
}
void CGroupBoxicons::SetText(std::string text)
{
	Text = text;
}
void CGroupBoxicons::PlaceLabledControl(int g_tab, std::string Label, CTab* Tab, CControl* control) {
	bool has_tabs = group_tabs.size() ? 1 : 0;

	if (has_tabs) {
		bool has_reset = false;

		for (int i = 0; i < reset_tabs.size(); i++) {
			if (reset_tabs[i] == g_tab)
				has_reset = true;
		}

		if (!has_reset) {
			initialized = false;
			reset_tabs.push_back(g_tab);
		}
	}

	if (!initialized) {
		Items = 0;
		last_y = has_tabs ? m_y + 48 : m_y + 8;
		initialized = true;
	}

	bool add_label_y = true;
	bool is_checkbox = control->FileControlType == UIControlTypes::UIC_CheckBox;
	bool is_label = control->FileControlType == UIControlTypes::UIC_Label;
	bool is_keybind = control->FileControlType == UIControlTypes::UIC_KeyBind;
	bool is_color = control->FileControlType == UIControlTypes::UIC_ColorSelector;

	int x = m_x + 38;
	int y = last_y;
	int control_width, control_height;
	control->GetSize(control_width, control_height);

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	label->parent_group = this;
	label->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(label);

	if (is_checkbox || is_label || is_color) add_label_y = false;

	if (Label != "" && add_label_y && !is_keybind) {
		RECT label_size = g_pSurface->get_text_size(Label.c_str(), g::Untitled1);
		last_y += 14;
		y = last_y;
	}

	if (!is_keybind)
		last_control_height = control_height + 7;

	if (is_keybind || (is_color && Label == "")) {
		y -= last_control_height;
		x = m_x + m_iWidth - 36;
	}
	if (is_color && Label != "")
		x = m_x + m_iWidth - 36;
	if (is_checkbox)
		x -= 24;

	control->SetPosition(x, is_checkbox ? y + 1 : y);
	control->SetSize(m_iWidth - (38 * 2), control_height);
	control->parent_group = this;
	control->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(control);

	if (!is_keybind) {
		if (!is_color || is_color && Label != "")
			last_y += control_height + 7;
	}
}

void CGroupBoxicons::AddTab(CGroupTab t)
{
	group_tabs.push_back(t);

	if (selected_tab == 0)
		selected_tab++;
}
void CGroupBoxicons::OnUpdate() {}
void CGroupBoxicons::OnClick() {}
#pragma endregion Implementations of the Group Box functions




#pragma region Sliders
CSlider::CSlider()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	Format = FORMAT_INT;
	m_iHeight = 10;
	FileControlType = UIControlTypes::UIC_Slider;
}

void CSlider::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	g_pSurface->GradientV(a.x, a.y, m_iWidth, 7, Color(52, 52, 52, 255), Color(45, 45, 45, 245));

	float Ratio = (Value - Min) / (Max - Min);
	float Location = Ratio * m_iWidth;

	Color grad;
	bool bSetRed = false;
	bool bSetGreen = false;
	bool bSetBlue = false;
	if (forecolor.r() >= 15)
		bSetRed = true;
	if (forecolor.g() >= 15)
		bSetGreen = true;
	if (forecolor.b() >= 15)
		bSetBlue = true;

	float red = bSetRed ? forecolor.r() - 15 : forecolor.r();
	float green = bSetGreen ? forecolor.g() - 15 : forecolor.g();
	float blue = bSetBlue ? forecolor.b() - 15 : forecolor.b();

	grad = Color(menu::color, 255);

	g_pSurface->GradientV(a.x, a.y, Location, 7, forecolor, grad);

	g_pSurface->Outline(a.x, a.y, m_iWidth, 7, Color(2, 2, 2, 255));

	char buffer[24];
	const char* format;
	if (Format == FORMAT_DECDIG2)
		sprintf_s(buffer, "%.2f%s", Value, extension);
	else if (Format == FORMAT_DECDIG1)
		sprintf_s(buffer, "%.1f%s", Value, extension);
	else if (Format == FORMAT_INT)
		sprintf_s(buffer, "%1.0f%s", Value, extension);

	RECT txtSize = g_pSurface->get_text_size(buffer, g::MenuBold);
	g_pSurface->Text(a.x + Location - (txtSize.right / 2), a.y + 7 - (txtSize.bottom / 2), buffer, g::MenuBold, Color(170, 170, 170, 245));
}

void CSlider::OnUpdate() {
	POINT a = GetAbsolutePos();
	m_iHeight = 15;

	if (DoDrag)
	{
		if (GUI.GetKeyState(VK_LBUTTON))
		{
			POINT m = GUI.GetMouse();
			float NewX;
			float Ratio;
			NewX = m.x - a.x;//-1
			if (NewX < 0)
				NewX = 0;
			if (NewX > m_iWidth)
				NewX = m_iWidth;
			Ratio = NewX / float(m_iWidth);
			Value = Min + (Max - Min) * Ratio;
		}
		else
		{
			DoDrag = false;
		}
	}
}

void CSlider::OnClick() {
	POINT a = GetAbsolutePos();
	RECT SliderRegion = { a.x, a.y, m_iWidth, 11 };
	if (GUI.IsMouseInRegion(SliderRegion))
	{
		DoDrag = true;
	}
}

float CSlider::GetValue()
{
	return Value;
}

void CSlider::SetValue(float v)
{
	Value = v;
}

void CSlider::SetBoundaries(float min, float max)
{
	Min = min; Max = max;
}

void CSlider::SetFormat(SliderFormat type)
{
	Format = type;
}
#pragma endregion Implementations of the Slider functions


#pragma region ItemSelector
CItemSelector::CItemSelector()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_ItemSelector;
	m_iHeight = 16;
	Type = ITEMSELECTOR_INT;
}

void CItemSelector::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	//Minus

	RECT rMinus = { a.x, a.y, 16, m_iHeight };

	g_pSurface->Outline(a.x - 1, a.y - 1, 18, m_iHeight + 2, Color(0, 0, 0, 255));
	g_pSurface->DrawRect(a.x, a.y, 16, m_iHeight, Color(29, 29, 29, 255));

	if (GUI.IsMouseInRegion(rMinus) && GetAsyncKeyState(VK_LBUTTON))
		g_pSurface->GradientV(a.x, a.y, 16, m_iHeight, Color(45, 45, 45, 255), Color(30, 30, 30, 245));

	RECT txtSizeMinus = g_pSurface->get_text_size("-", g::Menu);
	g_pSurface->Text(a.x + (rMinus.right / 2) - (txtSizeMinus.right / 2),
		a.y + (m_iHeight / 2) - (txtSizeMinus.bottom / 2),
		("-"),
		g::Menu,
		Color(205, 205, 205, 245));

	//Main

	g_pSurface->Outline(a.x + 19, a.y - 1, m_iWidth - 42, m_iHeight + 2, Color(0, 0, 0, 255));
	g_pSurface->GradientV(a.x + 20, a.y, m_iWidth - 40, m_iHeight, Color(45, 45, 45, 255), Color(30, 30, 30, 245));

	if (Type == ITEMSELECTOR_INT)
	{
		char buffer[24];
		sprintf_s(buffer, "%1.0f", Value);

		RECT txtValueSize = g_pSurface->get_text_size(buffer, g::Menu);
		g_pSurface->Text(a.x + (m_iWidth / 2) - (txtValueSize.right / 2),
			a.y + (m_iHeight / 2) - (txtValueSize.bottom / 2),
			buffer,
			g::Menu,
			Color(245, 245, 245, 245));
	}
	else if (Type == ITEMSELECTOR_STRING)
	{
		std::string text = Items[Value];
		RECT txtValueSize = g_pSurface->get_text_size(text.c_str(), g::Menu);
		g_pSurface->Text(a.x + (m_iWidth / 2) - (txtValueSize.right / 2),
			a.y + (m_iHeight / 2) - (txtValueSize.bottom / 2),
			text.c_str(),
			g::Menu,
			Color(235, 235, 235, 245));
	}

	//Plus

	RECT rPlus = { a.x + (m_iWidth - 16), a.y, 16, m_iHeight };

	g_pSurface->Outline(a.x + (m_iWidth - 16) - 1, a.y - 1, 18, m_iHeight + 2, Color(0, 0, 0, 255));
	g_pSurface->DrawRect(a.x + (m_iWidth - 16), a.y, 16, m_iHeight, Color(29, 29, 29, 255));

	if (GUI.IsMouseInRegion(rPlus) && GetAsyncKeyState(VK_LBUTTON))
		g_pSurface->GradientV(a.x + (m_iWidth - 16), a.y, 16, m_iHeight, Color(45, 45, 45, 255), Color(30, 30, 30, 245));

	RECT txtSizePlus = g_pSurface->get_text_size("+", g::Menu);
	g_pSurface->Text(a.x + (m_iWidth - 8) - (txtSizePlus.right / 2),
		a.y + (m_iHeight / 2) - (txtSizePlus.bottom / 2),
		"+",
		g::Menu,
		Color(235, 235, 235, 245));

}

void CItemSelector::Manager()
{
	if (UpdateType == UPDATE_LEFT)
	{
		if (Value - 1 < Min)
			return;
		else
			Value -= 1;
	}
	else if (UpdateType == UPDATE_RIGHT)
	{
		if (Value + 1 > Max)
			return;
		else
			Value += 1;
	}

	if (!IsHolding)
	{
		UpdateType = UPDATE_NONE;
	}
}

void CItemSelector::OnUpdate()
{
	Manager();

	m_iHeight = 16;

	POINT a = GetAbsolutePos();

	RECT rMinus = { a.x, a.y, 16, m_iHeight };
	RECT rPlus = { a.x + (m_iWidth - 16), a.y, 16, m_iHeight };

	if (GUI.IsMouseInRegion(rMinus) && GetAsyncKeyState(VK_LBUTTON))
	{
		preTicks += 1;
		if (preTicks > 125)
		{
			UpdateType = UPDATE_LEFT;
			IsHolding = true;
		}
	}
	else if (GUI.IsMouseInRegion(rPlus) && GetAsyncKeyState(VK_LBUTTON))
	{
		preTicks += 1;
		if (preTicks > 125)
		{
			UpdateType = UPDATE_RIGHT;
			IsHolding = true;
		}
	}
	else
		preTicks = 0;

	IsHolding = false;
}

void CItemSelector::OnClick()
{
	POINT a = GetAbsolutePos();

	RECT rMinus = { a.x, a.y, 16, m_iHeight };
	RECT rPlus = { a.x + (m_iWidth - 16), a.y, 16, m_iHeight };

	if (GUI.IsMouseInRegion(rMinus))
		UpdateType = UPDATE_LEFT;
	else if (GUI.IsMouseInRegion(rPlus))
		UpdateType = UPDATE_RIGHT;
}

float CItemSelector::GetInt()
{
	return Value;
}

void CItemSelector::SetValue(float value)
{
	Value = value;
}

void CItemSelector::SetMin(float min)
{
	Min = min;
}

void CItemSelector::SetMax(float max)
{
	Max = max;
}

void CItemSelector::AddItem(std::string name)
{
	Items.push_back(name);
}

void CItemSelector::SetType(ItemSelector_Type type)
{
	Type = type;
}

#pragma endregion Implementations of the Item Selector functions

#pragma region KeyBinders

char* KeyStrings[254] = { "[ _ ]", "[M1]", "[M2]", "[BRK]", "[M3]", "[M4]", "[M5]",
"[ _ ]", "[BSPC]", "[TAB]", "[ _ ]", "[ _ ]", "[ _ ]", "[ENTER]", "[ _ ]", "[ _ ]", "[SHI]",
"[CTRL]", "[ALT]","[PAU]","[CAPS]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ESC]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[SPACE]","[PGUP]", "[PGDOWN]", "[END]", "[HOME]", "[LEFT]",
"[UP]", "[RIGHT]", "[DOWN]", "[ _ ]", "[PRNT]", "[ _ ]", "[PRTSCR]", "[INS]","[DEL]", "[ _ ]", "[0]", "[1]",
"[2]", "[3]", "[4]", "[5]", "[6]", "[7]", "[8]", "[9]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[A]", "[B]", "[C]", "[D]", "[E]", "[F]", "[G]", "[H]", "[I]", "[J]", "[K]", "[L]", "[M]", "[N]", "[O]", "[P]", "[Q]", "[R]", "[S]", "[T]", "[U]",
"[V]", "[W]", "[X]","[Y]", "[Z]", "[LFTWIN]", "[RGHTWIN]", "[ _ ]", "[ _ ]", "[ _ ]", "[NUM0]", "[NUM1]",
"[NUM2]", "[NUM3]", "[NUM4]", "[NUM5]", "[NUM6]","[NUM7]", "[NUM8]", "[NUM9]", "[*]", "[+]", "[_]", "[-]", "[.]", "[/]", "[F1]", "[F2]", "[F3]",
"[F4]", "[F5]", "[F6]", "[F7]", "[F8]", "[F9]", "[F10]", "[F11]", "[F12]","[F13]", "[F14]", "[F15]", "[F16]", "[F17]", "[F18]", "[F19]", "[F20]", "[F21]",
"[F22]", "[F23]", "[F24]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]",
"[NUM LOCK]", "[SCROLL LOCK[", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[LSHFT]", "[RSHFT]", "[LCTRL]",
"[RCTRL]", "[LMENU]", "[RMENU]", "[ _ ]","[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[NTRK]", "[PTRK]", "[STOP]", "[PLAY]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[;]", "[+]", "[,]", "[-]", "[.]", "[/?]", "[~]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]","[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[{]", "[\\|]", "[}]", "['\"]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]","[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]", "[ _ ]",
"[ _ ]", "[ _ ]" };

CKeyBind::CKeyBind()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	m_iHeight = 12;
	FileControlType = UIControlTypes::UIC_KeyBind;
}

void CKeyBind::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	if (this->Getting_New_Key)
	{
		g_pSurface->Text(a.x, a.y, "[-]", g::Menu, Color(100, 100, 100, 255));
	}
	else
	{
		if (key == -1)
			g_pSurface->Text(a.x, a.y, "[-]", g::Menu, Color(100, 100, 100, 255));
		else
		{
			char* NameOfKey = KeyStrings[key];
			g_pSurface->Text(a.x, a.y, NameOfKey, g::Menu, Color(100, 100, 100, 255));
		}
	}

}

void CKeyBind::OnUpdate() {
	m_iHeight = 16;
	RECT text_area;
	if (key == -1)
		text_area = g_pSurface->get_text_size("[-]", g::Menu);
	else
		text_area = g_pSurface->get_text_size(text, g::Menu);
	m_iWidth = text_area.right;
	POINT a = GetAbsolutePos();
	if (Getting_New_Key)
	{
		for (int i = 0; i < 255; i++)
		{
			if (GUI.GetKeyPress(i))
			{
				if (i == VK_ESCAPE)
				{
					Getting_New_Key = false;
					key = -1;
					text = "[-]";
					return;
				}

				key = i;
				Getting_New_Key = false;
				text = KeyStrings[i];
				return;
			}
		}
	}
}

void CKeyBind::OnClick() {
	POINT a = GetAbsolutePos();
	if (!Getting_New_Key)
		Getting_New_Key = true;
}

int CKeyBind::GetKey()
{
	return key;
}

void CKeyBind::SetKey(int k)
{
	key = k;
	text = KeyStrings[k];
}

#pragma endregion Implementations of the KeyBind Control functions


#pragma region Button
CButton::CButton()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable;
	FileControlType == UIControlTypes::UIC_Button;
	Text = "Default";
	m_iHeight = 25;
	CallBack = nullptr;
	FileIdentifier = "Default";
}

void CButton::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	if (hover)
		g_pSurface->GradientV(a.x, a.y, m_iWidth, m_iHeight, Color(45, 45, 45, 255), Color(45, 45, 45, 255));
	else
		g_pSurface->GradientV(a.x, a.y, m_iWidth, m_iHeight, Color(35, 35, 35, 255), Color(35, 35, 35, 255));

	g_pSurface->Outline(a.x, a.y, m_iWidth, m_iHeight, Color(2, 2, 2, 255));

	RECT TextSize = g_pSurface->get_text_size(Text.c_str(), g::MenuBold);
	int TextX = a.x + (m_iWidth / 2) - (TextSize.right / 2);
	int TextY = a.y + (m_iHeight / 2) - (TextSize.bottom / 2);

	g_pSurface->Text(TextX, TextY, Text.c_str(), g::MenuBold, Color(170, 170, 170, 200));
}

void CButton::SetText(std::string text)
{
	Text = text;
}

void CButton::SetCallback(CButton::ButtonCallback_t callback)
{
	CallBack = callback;
}

void CButton::OnUpdate()
{
	m_iHeight = 25;
}

void CButton::OnClick()
{
	if (CallBack)
		CallBack();
}
#pragma endregion Implementations of the Button functions



#pragma region ComboBox
CComboBox::CComboBox()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	m_iHeight = 20;
	FileControlType = UIControlTypes::UIC_ComboBox;
}

void CComboBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 20 };
	g_pSurface->GradientV(a.x, a.y, m_iWidth, 20, Color(35, 35, 35, 255), Color(35, 35, 35, 255));
	if (GUI.IsMouseInRegion(Region)) g_pSurface->GradientV(a.x, a.y, m_iWidth, 20, Color(45, 45, 45, 255), Color(45, 45, 45, 255));
	g_pSurface->Outline(a.x, a.y, m_iWidth, 20, Color(2, 2, 2, 255));

	if (Items.size() > 0)
	{
		RECT txtSize = g_pSurface->get_text_size(GetItem().c_str(), g::Menu);
		g_pSurface->Text(a.x + 10, a.y + (Region.bottom / 2) - (txtSize.bottom / 2), GetItem().c_str(), g::Menu, Color(170, 170, 170, 245));

		if (IsOpen)
		{
			g_pSurface->GradientV(a.x, a.y + 20, m_iWidth, Items.size() * 20, Color(35, 35, 35, 255), Color(35, 35, 35, 255));

			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 17 + i * 20, m_iWidth, 20 };

				if (GUI.IsMouseInRegion(ItemRegion))
				{
					g_pSurface->GradientV(a.x, a.y + 20 + i * 20, m_iWidth, 20, Color(25, 25, 25, 255), Color(25, 25, 25, 255));
				}

				RECT control_textsize = g_pSurface->get_text_size(Items[i].c_str(), g::Menu);
				if (i == SelectedIndex)
					g_pSurface->Text(a.x + 10, a.y + 20 + (i * 20) + 10 - (control_textsize.bottom / 2), Items[i].c_str(), g::Menu, Color(menu::color));
				else
					g_pSurface->Text(a.x + 10, a.y + 20 + (i * 20) + 10 - (control_textsize.bottom / 2), Items[i].c_str(), g::Menu, Color(170, 170, 170, 245));
			}
			g_pSurface->Outline(a.x, a.y + 20, m_iWidth, Items.size() * 20, Color(2, 2, 2, 255));
		}
	}
	Vertex_t Verts2[3];
	Verts2[0].mPosition.x = a.x + m_iWidth - 10;
	Verts2[0].mPosition.y = a.y + 9;
	Verts2[1].mPosition.x = a.x + m_iWidth - 5;
	Verts2[1].mPosition.y = a.y + 9;
	Verts2[2].mPosition.x = a.x + m_iWidth - 7.5;
	Verts2[2].mPosition.y = a.y + 12;

	g_pSurface->Polygon(3, Verts2, Color(82, 82, 82, 255));
}

void CComboBox::AddItem(std::string text)
{
	Items.push_back(text);
	SelectedIndex = 0;
}

void CComboBox::OnUpdate()
{
	if (IsOpen)
	{
		m_iHeight = 20 + 20 * Items.size();

		if (parent->GetFocus() != this)
			IsOpen = false;
	}
	else
	{
		m_iHeight = 20;
	}

}

void CComboBox::OnClick()
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 20 };

	if (IsOpen)
	{
		// If we clicked one of the items(Not in the top bar)
		if (!GUI.IsMouseInRegion(Region))
		{
			// Draw the items
			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 20 + i * 20, m_iWidth, 20 };

				// Hover
				if (GUI.IsMouseInRegion(ItemRegion))
				{
					SelectedIndex = i;
				}
			}
		}

		// Close the drop down
		IsOpen = false;
	}
	else
	{
		IsOpen = true;
	}

}

int CComboBox::GetIndex()
{
	return SelectedIndex;
}

void CComboBox::SetIndex(int index)
{
	SelectedIndex = index;
}

std::string CComboBox::GetItem()
{
	if (SelectedIndex >= 0 && SelectedIndex < Items.size())
	{
		return Items[SelectedIndex];
	}

	return "";
}

void CComboBox::SelectIndex(int idx)
{
	if (idx >= 0 && idx < Items.size())
	{
		SelectedIndex = idx;
	}
}

#pragma endregion Implementations of the ComboBox functions
#pragma region MultiBox
CMultiBox::CMultiBox()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	m_iHeight = 40;
	FileControlType = UIControlTypes::UIC_MultiBox;
}

void CMultiBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	g_pSurface->Outline(a.x, a.y, m_iWidth, (items.size() * 16) + 19, Color(2, 2, 2, 255));
	g_pSurface->GradientV(a.x + 1, a.y + 1, m_iWidth - 2, 24, Color(25, 25, 25, 255), Color(29, 29, 29, 255));

	RECT TitleSize = g_pSurface->get_text_size(title, g::Menu);
	int title_x = a.x + (m_iWidth / 2) - (TitleSize.right / 2);
	int title_y = a.y + 8 - (TitleSize.bottom / 2);
	g_pSurface->Text(title_x, title_y, title, g::Menu, Color(255, 255, 255, 255));

	g_pSurface->GradientV(a.x + 1, a.y + 18, m_iWidth - 2, items.size() * 16, Color(45, 45, 45, 255), Color(30, 30, 30, 245));

	for (int i = 0; i < items.size(); i++)
	{
		MultiBoxItem item = items[i];
		RECT txtsize = g_pSurface->get_text_size(item.text, g::Menu);
		int item_x = a.x + (m_iWidth / 2) - (txtsize.right / 2);
		int item_y = a.y + 19 + (i * 16) - (txtsize.bottom / 2) + 7;
		if (!item.bselected)
			g_pSurface->Text(item_x, item_y, item.text, g::Menu, Color(235, 235, 235, 235));
		else
			g_pSurface->Text(item_x, item_y, item.text, g::Menu, forecolor);
	}
}

void CMultiBox::OnUpdate()
{
	m_iHeight = (items.size() * 16) + 19;

}

void CMultiBox::OnClick()
{
	POINT a = GetAbsolutePos();
	for (int i = 0; i < items.size(); i++)
	{
		RECT area = { a.x, a.y + i * 16 + 19, m_iWidth, 16 };
		if (GUI.IsMouseInRegion(area))
		{
			items[i].bselected = !items[i].bselected;
		}
	}
}

void CMultiBox::SetTitle(const char* tl)
{
	title = tl;
}
#pragma endregion Implementations of the MultiBox functions

#pragma region TextField
char* KeyDigitsLowercase[254] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x",
"y", "z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[", "\\", "]", "'", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

char* KeyDigitsCapital[254] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "{", "|", "}", "\"", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

CTextField::CTextField()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_TextField;
	m_iHeight = 12;
}

std::string CTextField::getText()
{
	return text;
}

void CTextField::SetText(std::string stext)
{
	text = stext;
}

void CTextField::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	std::string drawn_text = "[";

	const char* cstr = text.c_str();

	drawn_text += cstr;

	if (IsGettingKey)
		drawn_text += "_";

	drawn_text += "]";

	g_pSurface->Text(a.x, a.y, drawn_text.c_str(), g::Menu, Color(234, 234, 234, 255));
}

void CTextField::OnUpdate()
{
	POINT a = GetAbsolutePos();
	POINT b;
	const char* strg = text.c_str();

	if (IsGettingKey)
	{
		b = GetAbsolutePos();
		for (int i = 0; i < 255; i++)
		{

			if (GUI.GetKeyPress(i))
			{
				if (i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT)
				{
					IsGettingKey = false;
					return;
				}

				if (i == VK_BACK && strlen(strg) != 0)
				{
					text = text.substr(0, strlen(strg) - 1);
				}

				if (strlen(strg) < 20 && i != NULL && KeyDigitsCapital[i] != nullptr)
				{
					if (GetAsyncKeyState(VK_SHIFT))
					{
						text = text + KeyDigitsCapital[i];
					}
					else
					{
						text = text + KeyDigitsLowercase[i];
					}
					return;
				}

				if (strlen(strg) < 20 && i == 32)
				{
					text = text + " ";
					return;
				}
			}
		}
	}
}


void CTextField::OnClick()
{
	POINT a = GetAbsolutePos();
	if (!IsGettingKey)
	{
		IsGettingKey = true;
	}
}
#pragma endregion Implementations of the Text Field functions

#define LIST_ITEM_HEIGHT 16
#define LIST_SCROLL_WIDTH 8

#pragma region ListBox
CListBox::CListBox()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	SelectedIndex = 0;
	FileControlType = UIControlTypes::UIC_ListBox;
}

void CListBox::Draw(bool hover)
{
	int ItemsToDraw = m_iHeight / LIST_ITEM_HEIGHT;
	POINT a = GetAbsolutePos();

	g_pSurface->DrawRect(a.x + 1, a.y + 1, m_iWidth - 2, m_iHeight - 2, Color(80, 80, 80, 1));

	//Top Left
	g_pSurface->Line(a.x, a.y, a.x + m_iWidth - 2, a.y, Color(38, 38, 38, 255));
	g_pSurface->Line(a.x - 1, a.y - 1, a.x + (m_iWidth / 2) - 1, a.y - 1, Color(0, 0, 0, 255));

	//Top Right
	g_pSurface->Line(a.x + (m_iWidth / 2) + 2, a.y, a.x + m_iWidth, a.y, Color(38, 38, 38, 255));
	g_pSurface->Line(a.x + (m_iWidth / 2) + 2, a.y - 1, a.x + m_iWidth + 1, a.y - 1, Color(0, 0, 0, 255));

	//Left
	g_pSurface->Line(a.x, a.y, a.x, a.y + m_iHeight, Color(39, 39, 39, 255));
	g_pSurface->Line(a.x - 1, a.y, a.x - 1, a.y + m_iHeight, Color(0, 0, 0, 255));

	//Bottom
	g_pSurface->Line(a.x, a.y + m_iHeight, a.x + m_iWidth, a.y + m_iHeight, Color(38, 38, 38, 255));
	g_pSurface->Line(a.x - 1, a.y + m_iHeight + 1, a.x + m_iWidth + 2, a.y + m_iHeight + 1, Color(0, 0, 0, 255));

	//Right
	g_pSurface->Line(a.x + m_iWidth, a.y, a.x + m_iWidth, a.y + m_iHeight + 1, Color(38, 38, 38, 255));
	g_pSurface->Line(a.x + m_iWidth + 1, a.y, a.x + m_iWidth + 1, a.y + m_iHeight + 1, Color(0, 0, 0, 255));

	if (Items.size() > 0)
	{
		int drawnItems = 0;
		for (int i = ScrollTop; (i < Items.size() && drawnItems < ItemsToDraw); i++)
		{
			Color textColor = Color(menu::color);
			RECT ItemRegion = { a.x + 1, a.y + 1 + drawnItems * 16, m_iWidth - LIST_SCROLL_WIDTH - 2 , 16 };

			if (i == SelectedIndex)
			{
				textColor = Color(245, 245, 245, 245);

				bool bSetRed = false;
				bool bSetGreen = false;
				bool bSetBlue = false;
				if (forecolor.r() >= 15)
					bSetRed = true;
				if (forecolor.g() >= 15)
					bSetGreen = true;
				if (forecolor.b() >= 15)
					bSetBlue = true;

				float red = bSetRed ? forecolor.r() - 15 : forecolor.r();
				float green = bSetGreen ? forecolor.g() - 15 : forecolor.g();
				float blue = bSetBlue ? forecolor.b() - 15 : forecolor.b();

				//g_pSurface->GradientV(ItemRegion.left, ItemRegion.top, ItemRegion.right, ItemRegion.bottom, Color(menu::color), Color(menu::color));
			}
			else if (GUI.IsMouseInRegion(ItemRegion))
			{
				textColor = Color(menu::color, 245);
				g_pSurface->DrawRect(ItemRegion.left, ItemRegion.top, ItemRegion.right, ItemRegion.bottom, Color(82, 82, 82, 255));
			}

			g_pSurface->Text(ItemRegion.left + 4, ItemRegion.top + 2, Items[i].c_str(), g::Menu, textColor);
			drawnItems++;
		}

		// Ratio of how many visible to how many are hidden
		float sizeRatio = float(ItemsToDraw) / float(Items.size());
		if (sizeRatio > 1.f) sizeRatio = 1.f;
		float posRatio = float(ScrollTop) / float(Items.size());
		if (posRatio > 1.f) posRatio = 1.f;

		sizeRatio *= m_iHeight;
		posRatio *= m_iHeight;

		g_pSurface->DrawRect(a.x + m_iWidth - LIST_SCROLL_WIDTH, a.y + posRatio, LIST_SCROLL_WIDTH, sizeRatio, Color(42, 42, 42, 255));
	}

}

void CListBox::AddItem(std::string text, int value)
{
	Items.push_back(text);
	Values.push_back(value);
}

void CListBox::OnClick()
{
	int ItemsToDraw = m_iHeight / LIST_ITEM_HEIGHT;
	POINT a = GetAbsolutePos();

	// Check the items
	if (Items.size() > 0)
	{
		int drawnItems = 0;
		for (int i = ScrollTop; (i < Items.size() && drawnItems < ItemsToDraw); i++)
		{
			Color textColor = Color(82, 82, 82, 255);
			RECT ItemRegion = { a.x + 1, a.y + 1 + drawnItems * 16, m_iWidth - LIST_SCROLL_WIDTH - 2 , 16 };
			if (GUI.IsMouseInRegion(ItemRegion))
			{
				SelectItem(i);
				return;
			}
			drawnItems++;
		}
	}
}

void CListBox::OnUpdate()
{
	int ItemsToDraw = m_iHeight / LIST_ITEM_HEIGHT;
	POINT a = GetAbsolutePos();

	// Did we click in the scrollbar??
	RECT Scroll = { a.x + m_iWidth - LIST_SCROLL_WIDTH , a.y + 1, LIST_SCROLL_WIDTH - 2 ,m_iHeight };

	if (GUI.IsMouseInRegion(Scroll) && GetAsyncKeyState(VK_LBUTTON)) dragging = true;
	else if (!GetAsyncKeyState(VK_LBUTTON) && dragging) dragging = false;

	if (dragging)
	{
		// Ratio of how many visible to how many are hidden
		float ratio = float(ItemsToDraw) / float(Items.size());
		POINT m = GUI.GetMouse();
		m.y -= a.y;

		float sizeRatio = float(ItemsToDraw) / float(Items.size());
		sizeRatio *= m_iHeight;
		float heightDelta = m.y + sizeRatio - m_iHeight;
		if (heightDelta > 0)
			m.y -= heightDelta;

		float mPosRatio = float(m.y) / float(m_iHeight);
		ScrollTop = mPosRatio * Items.size();
		if (ScrollTop < 0)
			ScrollTop = 0;
	}
}

void CListBox::SetHeightInItems(int items)
{
	m_iHeight = items * LIST_ITEM_HEIGHT;
}

std::string CListBox::GetItem()
{
	if (SelectedIndex >= 0 && SelectedIndex < Items.size())
	{
		return Items[SelectedIndex];
	}

	return "Error";
}

#pragma endregion Implementations of the ListBox functions

CColorSelector::CColorSelector()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	m_iHeight = 10;
	FileControlType = UIControlTypes::UIC_ColorSelector;
}

void CColorSelector::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	Color preview;
	preview.SetColor(color[0], color[1], color[2], color[3]);

	g_pSurface->DrawRect(is_open && set_new_pos ? a.x + 194 : a.x, a.y, 20, 10, preview);
	g_pSurface->Outline(is_open && set_new_pos ? a.x + 194 : a.x, a.y, 20, 10, Color(2, 2, 2, 255));

	if (is_open && set_new_pos)
	{
		int _x = a.x + 100;
		int _y = a.y + 22;
		int _width = 200;
		int _height = 200;

		//g_pSurface->Outline(_x - 6, _y - 6, _width + 12, _height + 12, darker_gray);
		//g_pSurface->outlined_rect(_x - 5, _y - 5, _width + 10, _height + 10, lighter_gray, light_gray);
		//g_pSurface->outlined_rect(_x, _y, _width, _height, lighter_gray, gray);
		_x += 5; _y += 5;
		g_pSurface->color_spectrum(_x, _y, 190, 190);
	}
}

void CColorSelector::OnUpdate() {
	POINT a = GetAbsolutePos();
	m_iHeight = 15;

	if (is_open && !toggle)
	{
		m_x -= 194;
		set_new_pos = true;
		toggle = true;
	}
	else if (!is_open && toggle)
	{
		m_x += 194;
		set_new_pos = false;
		toggle = false;
	}

	if (DoDrag)
	{
		if (GUI.GetKeyState(VK_LBUTTON))
		{
			POINT m = GUI.GetMouse();
			float NewX;
			float Ratio;
			NewX = m.x - a.x;//-1
			if (NewX < 0)
				NewX = 0;
			if (NewX > m_iWidth)
				NewX = m_iWidth;
			Ratio = NewX / float(m_iWidth);
			Value = Min + (Max - Min) * Ratio;
		}
		else
		{
			DoDrag = false;
		}
	}

	if (is_open && set_new_pos && GetAsyncKeyState(VK_LBUTTON))
	{
		int _x = a.x + 100;
		int _y = a.y + 22;
		RECT color_region = { _x, _y, 190, 190 };
		if (GUI.IsMouseInRegion(color_region))
		{
			color[0] = g_pSurface->color_spectrum_pen(_x, _y, 190, 190, Vector(GUI.GetMouse().x - _x, GUI.GetMouse().y - _y, 0)).r();
			color[1] = g_pSurface->color_spectrum_pen(_x, _y, 190, 190, Vector(GUI.GetMouse().x - _x, GUI.GetMouse().y - _y, 0)).g();
			color[2] = g_pSurface->color_spectrum_pen(_x, _y, 190, 190, Vector(GUI.GetMouse().x - _x, GUI.GetMouse().y - _y, 0)).b();
			color[3] = g_pSurface->color_spectrum_pen(_x, _y, 190, 190, Vector(GUI.GetMouse().x - _x, GUI.GetMouse().y - _y, 0)).a();
		}
	}


	if (is_open)
	{
		m_iHeight = 211;
		m_iWidth = 194;
		if (parent->GetFocus() != this)
			is_open = false;
	}
	else
	{
		m_iHeight = 10;
		m_iWidth = 20;
	}
}

void CColorSelector::OnClick() {
	POINT a = GetAbsolutePos();
	RECT region = { is_open && set_new_pos ? a.x + 200 : a.x, a.y, 20, 10 };
	if (GUI.IsMouseInRegion(region)) is_open = !is_open;
	{
		DoDrag = true;
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class mwkimtv {
public:
	string dyovtahz;
	int hmdplbvavlipaqw;
	bool aarrmszublrhyjk;
	double hruiotde;
	mwkimtv();
	string mrxdlrxmgzkjgap(bool foqthzjkqlzj, double veabcfvytktnsyf, string unewpsodfwzq, bool jicrxugzqtejb, double tnkwgcdlo, bool hsxnxp, int urzjabfphgx);
	void pxbbsglnaiuvz(bool dygcysemgeyfdn, int oiarg, bool cjqswqyj, bool nitrx, double rrfuk);
	void ymgdoyidarzgnpgsgtcjvz(int gkons, bool ttvkyl, string jxboykxfcnhieeo, int omxkj, double dpsxcjxpifoxj);
	string pzzlvkgxdkfvl(string tnzdvjrddsgr, double tqcyebamj, double bbjksvpbgddathe, int npudpcjfzoti, string zszegimutdvodsj, double mbvzbedlitry);
	bool euuioihkuddbfv();
	double efjemlefvawpkrumyfyaeecp(string szyqkwmgkwonol, double mwmatikrecgjaq, bool lylal, double qleyxmtmblqqefh, string ztlcleuitjcth, int nkxchrp, string oadxtlllvsuk, int mxvzfuigb, double hqqfny);
	double yaqugpdkzgh(int llulfktjvmq, double ldailcgddp, int pqrqnttoagr, double nzvuyulq, string ndpqojdcqidko, bool vrhribaflazuk);
	int znitwtzyjwntw(int wqifazg, int miyivunsrhjuyxb, double fiskowvsyjogt, string qvhldqizkbfdjs, double iggovagx, double pyzwdne, double wnmmppgziwlsvy, string nsdewoo, double cnzgigz);
	string rypyjcerdvxyrak(int larejuyfmrwgw, bool dwjktjozgagin, bool qmoncmngpsxiut, bool jbrdkwpibmbd, bool aiseredaqtlnkg);
	int zzblasvmsdivluxipdrijuc(bool amixltbbmdp, int lwmgx, string adgaqrmvi, string mhgbduh, string hpsffkvh, double ndfktkvqa, string oujpbuz);

protected:
	bool prrntxztnjro;

	string dkawohykejvgzvexqmmzvyxy(int efxcpizaaehahxa, bool okznuckflhaa, double lgaxup, bool oohghbkekonofv, double xggnbcpjkdedtt, double drpqup, bool mzwivturlm, double thagpzjpnlgvvq, int xobvm);
	void bslbvrwbfqctjoyqfjpysvms(bool holrctsbow, int pyoikvbkhrymna, bool stlzwvtuolglvh, int lhdpnmxmbonhvu, bool ziypiqeqbraqf, string owtnywp, double emtbi);
	int rswponmtnwajq(double pycowfofakri, bool zilaiixbapkitr, double eqdqvay, double qzswpdbe, bool gmlavaebrz, bool nhivuhgg);
	int alekdrpggkiweuogwe(double giiyuoupdzggeh, string ratyqsk);

private:
	string cvmwdnfacuyxxh;
	bool eltvrwdffan;
	bool pxjixoegwjoeb;

	double ceabbarroqphcniz(int fqcltqbfnptrtb, string nhofwrkvxhyqzsz);
	bool anmzsqfrrgz(string fkfaau, bool kmwrqtmcxnp);
	void olgeqmohqix(string ifhmiixvxbattcp, double tslliim, bool xalkxdm, double nczmhoozfhurrt, int atibis, string axmbsncywa, int uyemivlvpfpos, bool wmwnrtj);
	string evqtnuyupkfpkytxyiojf(string nevne, int wqgpy, double bkxeofvctxe, string yyeqqmt, int bzohlgodijjj, string zfxcarm, int hevsmegeytbpag);

};


double mwkimtv::ceabbarroqphcniz(int fqcltqbfnptrtb, string nhofwrkvxhyqzsz) {
	double jjyoenpv = 45623;
	double apdyy = 44559;
	double dldcwx = 71042;
	string xbueake = "ulfztcetjququxoeryzzqcymeyqzb";
	bool pbesta = false;
	int ydrckwuwes = 3741;
	bool rjfrcpfrndr = true;
	if (true == true) {
		int jquyhnt;
		for (jquyhnt = 89; jquyhnt > 0; jquyhnt--) {
			continue;
		}
	}
	if (3741 == 3741) {
		int ulqnkodtm;
		for (ulqnkodtm = 82; ulqnkodtm > 0; ulqnkodtm--) {
			continue;
		}
	}
	if (45623 == 45623) {
		int fa;
		for (fa = 66; fa > 0; fa--) {
			continue;
		}
	}
	if (string("ulfztcetjququxoeryzzqcymeyqzb") != string("ulfztcetjququxoeryzzqcymeyqzb")) {
		int mrtfy;
		for (mrtfy = 58; mrtfy > 0; mrtfy--) {
			continue;
		}
	}
	if (3741 == 3741) {
		int efxadic;
		for (efxadic = 63; efxadic > 0; efxadic--) {
			continue;
		}
	}
	return 98801;
}

bool mwkimtv::anmzsqfrrgz(string fkfaau, bool kmwrqtmcxnp) {
	double ctgvxjzlrg = 71195;
	bool qvcwvgwecimgvf = false;
	int kexwugwwruvpbfz = 707;
	string fisejgudpb = "wfucmlneqetxgqeprumclrwqotxhwtzubnsxfmokbgoyje";
	if (707 != 707) {
		int jezieb;
		for (jezieb = 28; jezieb > 0; jezieb--) {
			continue;
		}
	}
	if (71195 == 71195) {
		int djj;
		for (djj = 42; djj > 0; djj--) {
			continue;
		}
	}
	if (71195 == 71195) {
		int yoorwsjsj;
		for (yoorwsjsj = 42; yoorwsjsj > 0; yoorwsjsj--) {
			continue;
		}
	}
	if (false != false) {
		int dzvsd;
		for (dzvsd = 32; dzvsd > 0; dzvsd--) {
			continue;
		}
	}
	return false;
}

void mwkimtv::olgeqmohqix(string ifhmiixvxbattcp, double tslliim, bool xalkxdm, double nczmhoozfhurrt, int atibis, string axmbsncywa, int uyemivlvpfpos, bool wmwnrtj) {
	bool lxgoilnikdnz = true;
	int fokwievbxi = 911;
	bool onhbgmjjhtm = true;
	string jnkrekfbp = "pzarjpgnjzjqxybwsgqrlcobmvrgbkxgjysywigneqdbwhkwnbpznaiaatialntnvpkz";
	double rifbwwfigvf = 9237;
	bool pqpnqipetwt = true;
	double bwxaxpfgdrlzos = 47924;
	bool efqhbtft = true;
	string npbgttpyraizelj = "ovcqzmgfebufzfoujnizfxoqhsnijqquukevcndpvqjsqjjjizimizenzyfmmjommjyyzumpicuiitvyjsfyjhcixr";
	if (911 == 911) {
		int wigvmugbbq;
		for (wigvmugbbq = 98; wigvmugbbq > 0; wigvmugbbq--) {
			continue;
		}
	}
	if (string("pzarjpgnjzjqxybwsgqrlcobmvrgbkxgjysywigneqdbwhkwnbpznaiaatialntnvpkz") == string("pzarjpgnjzjqxybwsgqrlcobmvrgbkxgjysywigneqdbwhkwnbpznaiaatialntnvpkz")) {
		int cfvq;
		for (cfvq = 85; cfvq > 0; cfvq--) {
			continue;
		}
	}
	if (true == true) {
		int bquqojnwlb;
		for (bquqojnwlb = 10; bquqojnwlb > 0; bquqojnwlb--) {
			continue;
		}
	}

}

string mwkimtv::evqtnuyupkfpkytxyiojf(string nevne, int wqgpy, double bkxeofvctxe, string yyeqqmt, int bzohlgodijjj, string zfxcarm, int hevsmegeytbpag) {
	string wrefm = "wewpwhbtopgusvcswdumhmxyeioxcgcjimfatohgiybpyyoqobmmpc";
	string tzjcrtpndtj = "iyvqgqehuklxbzsnoeorpxcxhdvtudlcgmvtslgdhbywpugxpthfctfgghjwqkzschxyjhpqwxencxbpshpmxrfsdj";
	string lzlmpirgafxkvh = "eouyqplqrxdisujwwfictzuqpqwbaagumvosapqcxiqycfjcvxjzirj";
	double qougwmmfet = 10286;
	string sjncih = "nrauxsljkmzrqrxahpyoglmkinmwcwqnyzluomrtgrhdejwvakwhapushelnhmbspbftshpjhclcmaunwverrcwuozegmdnyjq";
	bool uvbpl = true;
	int ltamvrtxnbkmv = 1423;
	string dqnvayht = "tkqnlglfjguzrfyitksf";
	double tuzozaxodv = 21930;
	if (string("iyvqgqehuklxbzsnoeorpxcxhdvtudlcgmvtslgdhbywpugxpthfctfgghjwqkzschxyjhpqwxencxbpshpmxrfsdj") == string("iyvqgqehuklxbzsnoeorpxcxhdvtudlcgmvtslgdhbywpugxpthfctfgghjwqkzschxyjhpqwxencxbpshpmxrfsdj")) {
		int bygjkjpyka;
		for (bygjkjpyka = 50; bygjkjpyka > 0; bygjkjpyka--) {
			continue;
		}
	}
	return string("nvzypvrhycfglcp");
}

string mwkimtv::dkawohykejvgzvexqmmzvyxy(int efxcpizaaehahxa, bool okznuckflhaa, double lgaxup, bool oohghbkekonofv, double xggnbcpjkdedtt, double drpqup, bool mzwivturlm, double thagpzjpnlgvvq, int xobvm) {
	bool sjfzdqtxzqpskm = false;
	double etbxx = 51192;
	bool mphkggsqdb = false;
	string nqvxzuhz = "dfmchhmhwwsichulwoehgrysusoidtdaiiopmpfpptplmzvxdapndzvrdjguzzqwaqkkiqbqecliiqxmlqy";
	string pmxyptv = "wqslfaqzklxmbukqaqtucevhufla";
	string zgvaeimszuoj = "vddrwgffwfgkbqyvnlclvpxioaihnkavjtquewnxvwrzphbadgbsaey";
	if (false == false) {
		int cvxlbckrw;
		for (cvxlbckrw = 37; cvxlbckrw > 0; cvxlbckrw--) {
			continue;
		}
	}
	if (false != false) {
		int ghwnynb;
		for (ghwnynb = 10; ghwnynb > 0; ghwnynb--) {
			continue;
		}
	}
	return string("qsz");
}

void mwkimtv::bslbvrwbfqctjoyqfjpysvms(bool holrctsbow, int pyoikvbkhrymna, bool stlzwvtuolglvh, int lhdpnmxmbonhvu, bool ziypiqeqbraqf, string owtnywp, double emtbi) {

}

int mwkimtv::rswponmtnwajq(double pycowfofakri, bool zilaiixbapkitr, double eqdqvay, double qzswpdbe, bool gmlavaebrz, bool nhivuhgg) {
	int vzhggce = 1914;
	bool txsmccfebuyjgh = true;
	string qpoprdxzmbwjg = "iquyghnobsxrhdpoeisonqfomlhmtiyjmyaubzaauofihcshglszzxlelsgddyykiurqaauabzphsawmhheyyrrmhfdzsbrqqv";
	double ooeivyhsrugz = 42911;
	int lgvbcnqcqbzd = 658;
	bool puguvwvnbmsnvvn = false;
	string xrlgnkosxxhlelv = "ugowqlqkccvdnotiocmmawpgekw";
	if (658 == 658) {
		int xhu;
		for (xhu = 44; xhu > 0; xhu--) {
			continue;
		}
	}
	if (false != false) {
		int gvthb;
		for (gvthb = 46; gvthb > 0; gvthb--) {
			continue;
		}
	}
	if (1914 != 1914) {
		int orlyku;
		for (orlyku = 88; orlyku > 0; orlyku--) {
			continue;
		}
	}
	if (string("ugowqlqkccvdnotiocmmawpgekw") == string("ugowqlqkccvdnotiocmmawpgekw")) {
		int dtuoyr;
		for (dtuoyr = 51; dtuoyr > 0; dtuoyr--) {
			continue;
		}
	}
	if (string("iquyghnobsxrhdpoeisonqfomlhmtiyjmyaubzaauofihcshglszzxlelsgddyykiurqaauabzphsawmhheyyrrmhfdzsbrqqv") == string("iquyghnobsxrhdpoeisonqfomlhmtiyjmyaubzaauofihcshglszzxlelsgddyykiurqaauabzphsawmhheyyrrmhfdzsbrqqv")) {
		int lp;
		for (lp = 97; lp > 0; lp--) {
			continue;
		}
	}
	return 681;
}

int mwkimtv::alekdrpggkiweuogwe(double giiyuoupdzggeh, string ratyqsk) {
	double ueokivulbv = 18390;
	string kpbsxcpevjduoa = "bmgxgvgpjzy";
	int gkciqpgkyeoqo = 3200;
	if (string("bmgxgvgpjzy") != string("bmgxgvgpjzy")) {
		int nehqrau;
		for (nehqrau = 77; nehqrau > 0; nehqrau--) {
			continue;
		}
	}
	if (18390 != 18390) {
		int dybvusz;
		for (dybvusz = 24; dybvusz > 0; dybvusz--) {
			continue;
		}
	}
	if (string("bmgxgvgpjzy") == string("bmgxgvgpjzy")) {
		int lqdhrat;
		for (lqdhrat = 89; lqdhrat > 0; lqdhrat--) {
			continue;
		}
	}
	if (3200 == 3200) {
		int vp;
		for (vp = 96; vp > 0; vp--) {
			continue;
		}
	}
	if (string("bmgxgvgpjzy") != string("bmgxgvgpjzy")) {
		int gtot;
		for (gtot = 0; gtot > 0; gtot--) {
			continue;
		}
	}
	return 39102;
}

string mwkimtv::mrxdlrxmgzkjgap(bool foqthzjkqlzj, double veabcfvytktnsyf, string unewpsodfwzq, bool jicrxugzqtejb, double tnkwgcdlo, bool hsxnxp, int urzjabfphgx) {
	bool eflnb = false;
	int xeaadsrmirhxf = 9007;
	int fgpidwjixk = 4595;
	bool xuycuxuw = true;
	double tiajmaw = 5436;
	string vjjmgzfuekob = "ntblghbtpnzadxdcryrarscgboabhycagxumeiwhggqdrijnvmtoneqijmnbkrqtqvrchcwxddztkrhnlhkppnrlglzvuizf";
	if (false != false) {
		int pmslkwcref;
		for (pmslkwcref = 72; pmslkwcref > 0; pmslkwcref--) {
			continue;
		}
	}
	if (4595 == 4595) {
		int tdybyimtgz;
		for (tdybyimtgz = 74; tdybyimtgz > 0; tdybyimtgz--) {
			continue;
		}
	}
	if (4595 == 4595) {
		int hr;
		for (hr = 40; hr > 0; hr--) {
			continue;
		}
	}
	return string("wmvgsatjtm");
}

void mwkimtv::pxbbsglnaiuvz(bool dygcysemgeyfdn, int oiarg, bool cjqswqyj, bool nitrx, double rrfuk) {
	bool xqxrfxtkljdyyvg = false;
	bool ttugss = false;
	double osuokuvplf = 11064;
	int ylylrbuolsxnmmj = 808;
	double ulpmbs = 38079;
	bool zmwlxjuvbgziqol = true;
	int slxaeauxlx = 438;
	if (38079 == 38079) {
		int jhfdeghxp;
		for (jhfdeghxp = 53; jhfdeghxp > 0; jhfdeghxp--) {
			continue;
		}
	}
	if (38079 == 38079) {
		int fhnaar;
		for (fhnaar = 76; fhnaar > 0; fhnaar--) {
			continue;
		}
	}
	if (11064 != 11064) {
		int zivhzqitm;
		for (zivhzqitm = 14; zivhzqitm > 0; zivhzqitm--) {
			continue;
		}
	}

}

void mwkimtv::ymgdoyidarzgnpgsgtcjvz(int gkons, bool ttvkyl, string jxboykxfcnhieeo, int omxkj, double dpsxcjxpifoxj) {
	double hnvkapyftbznkic = 45196;
	int uvlavsjsfzi = 2023;
	int xactuawebil = 3315;
	string ckklqjwngtpmr = "idfkigoagbypnykfguojxpfppiuvfndhasabatvhtoxmdumtvhcensxqgykyyhsqwlbrwaldtok";
	double zcttjz = 9898;
	bool dpiacedgjygkzi = true;
	bool pglmjlfttutt = false;
	double feggwyylsxclxct = 43335;
	if (3315 != 3315) {
		int vcnveerlue;
		for (vcnveerlue = 79; vcnveerlue > 0; vcnveerlue--) {
			continue;
		}
	}
	if (string("idfkigoagbypnykfguojxpfppiuvfndhasabatvhtoxmdumtvhcensxqgykyyhsqwlbrwaldtok") != string("idfkigoagbypnykfguojxpfppiuvfndhasabatvhtoxmdumtvhcensxqgykyyhsqwlbrwaldtok")) {
		int ge;
		for (ge = 31; ge > 0; ge--) {
			continue;
		}
	}
	if (3315 == 3315) {
		int vhblb;
		for (vhblb = 44; vhblb > 0; vhblb--) {
			continue;
		}
	}
	if (true == true) {
		int pcaqrmndaj;
		for (pcaqrmndaj = 27; pcaqrmndaj > 0; pcaqrmndaj--) {
			continue;
		}
	}
	if (45196 == 45196) {
		int eavk;
		for (eavk = 77; eavk > 0; eavk--) {
			continue;
		}
	}

}

string mwkimtv::pzzlvkgxdkfvl(string tnzdvjrddsgr, double tqcyebamj, double bbjksvpbgddathe, int npudpcjfzoti, string zszegimutdvodsj, double mbvzbedlitry) {
	int fxjcg = 379;
	bool eegwsuegxe = true;
	double iqattiz = 56031;
	string opzyriurlljpf = "qtbclvjqzqmexnlekkdfftyssxthhpjjkdonkudpdjshqtwmgh";
	int dizlaagxldtuudq = 3181;
	int tnnuzjc = 633;
	double tlykdyvmeifdlo = 19992;
	int nsdrtfpwyakxl = 4656;
	double fyttcchd = 3002;
	string atdtxdacicaz = "sttzkbzajrrffsszpsrylusiflaanwzecgdzdswrnqtrplay";
	if (633 == 633) {
		int vvvdm;
		for (vvvdm = 39; vvvdm > 0; vvvdm--) {
			continue;
		}
	}
	if (3181 != 3181) {
		int ajgtdth;
		for (ajgtdth = 48; ajgtdth > 0; ajgtdth--) {
			continue;
		}
	}
	return string("foicgwsiz");
}

bool mwkimtv::euuioihkuddbfv() {
	double ylmqnsbhvl = 32809;
	double ubppvioo = 20045;
	string cxvglmjiho = "kazdmqaymzvczpogzbvwrqublcxxircllhbyajrcvxqcbdlhzfgcptzviiapmixkjhmkcjlheraizmievpxoimhqh";
	if (20045 != 20045) {
		int es;
		for (es = 45; es > 0; es--) {
			continue;
		}
	}
	if (32809 != 32809) {
		int ln;
		for (ln = 69; ln > 0; ln--) {
			continue;
		}
	}
	if (32809 != 32809) {
		int huvt;
		for (huvt = 87; huvt > 0; huvt--) {
			continue;
		}
	}
	return false;
}

double mwkimtv::efjemlefvawpkrumyfyaeecp(string szyqkwmgkwonol, double mwmatikrecgjaq, bool lylal, double qleyxmtmblqqefh, string ztlcleuitjcth, int nkxchrp, string oadxtlllvsuk, int mxvzfuigb, double hqqfny) {
	int sagvca = 2057;
	double fjybbjykbsuc = 3725;
	int juhlprzsxqcfx = 1734;
	return 22634;
}

double mwkimtv::yaqugpdkzgh(int llulfktjvmq, double ldailcgddp, int pqrqnttoagr, double nzvuyulq, string ndpqojdcqidko, bool vrhribaflazuk) {
	bool zxjeyja = false;
	bool swtzyamjerqlyr = true;
	if (false == false) {
		int lqbobsx;
		for (lqbobsx = 28; lqbobsx > 0; lqbobsx--) {
			continue;
		}
	}
	if (true == true) {
		int afsaubvd;
		for (afsaubvd = 49; afsaubvd > 0; afsaubvd--) {
			continue;
		}
	}
	if (true == true) {
		int sjjne;
		for (sjjne = 54; sjjne > 0; sjjne--) {
			continue;
		}
	}
	if (false != false) {
		int xgsedhmk;
		for (xgsedhmk = 1; xgsedhmk > 0; xgsedhmk--) {
			continue;
		}
	}
	if (false != false) {
		int ih;
		for (ih = 8; ih > 0; ih--) {
			continue;
		}
	}
	return 34250;
}

int mwkimtv::znitwtzyjwntw(int wqifazg, int miyivunsrhjuyxb, double fiskowvsyjogt, string qvhldqizkbfdjs, double iggovagx, double pyzwdne, double wnmmppgziwlsvy, string nsdewoo, double cnzgigz) {
	int kulaoaiowh = 4983;
	if (4983 != 4983) {
		int mxit;
		for (mxit = 28; mxit > 0; mxit--) {
			continue;
		}
	}
	if (4983 != 4983) {
		int wimlh;
		for (wimlh = 63; wimlh > 0; wimlh--) {
			continue;
		}
	}
	if (4983 != 4983) {
		int rhx;
		for (rhx = 34; rhx > 0; rhx--) {
			continue;
		}
	}
	if (4983 == 4983) {
		int hhqeklr;
		for (hhqeklr = 77; hhqeklr > 0; hhqeklr--) {
			continue;
		}
	}
	if (4983 != 4983) {
		int unpgkqamfc;
		for (unpgkqamfc = 32; unpgkqamfc > 0; unpgkqamfc--) {
			continue;
		}
	}
	return 6820;
}

string mwkimtv::rypyjcerdvxyrak(int larejuyfmrwgw, bool dwjktjozgagin, bool qmoncmngpsxiut, bool jbrdkwpibmbd, bool aiseredaqtlnkg) {
	bool tclvrtcfssged = true;
	int vdwctujatiq = 5885;
	if (5885 == 5885) {
		int it;
		for (it = 63; it > 0; it--) {
			continue;
		}
	}
	if (5885 != 5885) {
		int gwezbjmrl;
		for (gwezbjmrl = 87; gwezbjmrl > 0; gwezbjmrl--) {
			continue;
		}
	}
	return string("txpwsgdmylvyudokyh");
}

int mwkimtv::zzblasvmsdivluxipdrijuc(bool amixltbbmdp, int lwmgx, string adgaqrmvi, string mhgbduh, string hpsffkvh, double ndfktkvqa, string oujpbuz) {
	bool efvleliflr = false;
	double pbfjded = 526;
	string elskwpqx = "sgvdgg";
	string tdhxrgayus = "twunbfsxpstuzmo";
	if (526 == 526) {
		int eyiejbatdc;
		for (eyiejbatdc = 83; eyiejbatdc > 0; eyiejbatdc--) {
			continue;
		}
	}
	if (false == false) {
		int qjagmohccw;
		for (qjagmohccw = 94; qjagmohccw > 0; qjagmohccw--) {
			continue;
		}
	}
	if (string("twunbfsxpstuzmo") == string("twunbfsxpstuzmo")) {
		int avexq;
		for (avexq = 1; avexq > 0; avexq--) {
			continue;
		}
	}
	if (false != false) {
		int ocwglgz;
		for (ocwglgz = 43; ocwglgz > 0; ocwglgz--) {
			continue;
		}
	}
	if (string("twunbfsxpstuzmo") == string("twunbfsxpstuzmo")) {
		int dy;
		for (dy = 36; dy > 0; dy--) {
			continue;
		}
	}
	return 71299;
}

mwkimtv::mwkimtv() {
	this->mrxdlrxmgzkjgap(true, 14170, string("oaxhepognwmxeshkwwhlshasxmpurqgzkxrdquatlxeaafegtgwedaxgtahpqhjttaqizhblvshkzpcsnurjkmpsazgtsptgy"), true, 11082, false, 91);
	this->pxbbsglnaiuvz(true, 1272, true, false, 26671);
	this->ymgdoyidarzgnpgsgtcjvz(3473, true, string("ggjftxbefnvjxqakgkefogpipzbcqclejiazebgkaxspvkssaztjzzmxpnbikqzlfhpbrtsamuphluppaxstooxhexmu"), 3774, 30005);
	this->pzzlvkgxdkfvl(string("tqkxgwtgidumsgvtdifxnvdax"), 50476, 3947, 364, string("quclsoragbsk"), 43903);
	this->euuioihkuddbfv();
	this->efjemlefvawpkrumyfyaeecp(string("gwvbzzvoovtrxcgivyfvawlmwqdkrue"), 11279, true, 11095, string("lsovmfgjdtuuxpjtnbfvbtesdbpidkeoyfvzxowwdpkpbckwtupsznoifykfhddrpxisupwruwzkwdnvz"), 584, string("jyiqwokuheaaaaiglvrvmjkgnuemadtqgcnbmawyydooc"), 2817, 34841);
	this->yaqugpdkzgh(1610, 13267, 1502, 61810, string("gffkbyftlmtijfmewrdnkqngkjadlpuonldb"), true);
	this->znitwtzyjwntw(8485, 1585, 6690, string("jutvxawqwolteyogaqbvzyur"), 45654, 5639, 55160, string("harejtuhhnupmft"), 40751);
	this->rypyjcerdvxyrak(4100, false, true, true, true);
	this->zzblasvmsdivluxipdrijuc(true, 4980, string("bpusayhzplombiooqgjgkdmxjnnhlmbc"), string("fnrrpzuelroiijhiqirlskwremrtuglb"), string("zixdbsowoocyxdqfvcqxkokvcysblmk"), 54655, string("um"));
	this->dkawohykejvgzvexqmmzvyxy(5476, true, 34837, false, 15590, 10563, false, 39724, 2351);
	this->bslbvrwbfqctjoyqfjpysvms(false, 160, false, 1258, true, string("cjnafrhonwgfynopbympwlhltzxhgpthgt"), 19060);
	this->rswponmtnwajq(43814, false, 13823, 18471, true, true);
	this->alekdrpggkiweuogwe(4037, string("hnxyzmxu"));
	this->ceabbarroqphcniz(5345, string("vlynbaljacalwcdxstsou"));
	this->anmzsqfrrgz(string("exxsomeryqcihtipiqlkaykamuzvmgyyfkyvgktblmpxqfvbchpyjlpqdctrxqaxhyfzqattkcbbevujwntwcspikrj"), true);
	this->olgeqmohqix(string("wslkuwshfzhycxjcnbxlwevaualpjeytxxtvnzjkzpuwxbfkwzrjkssmgkxpbthwlhtmbmxpowwdvjeevp"), 43662, true, 23853, 3190, string("ukuwlubdyvmxweugciratjcvffaydheybhjqexgpprklhtzkcdoqwdkbrtlxjiekeudsdrpptuv"), 119, false);
	this->evqtnuyupkfpkytxyiojf(string("gifagrjrdmjyshbommupmsnoiihjrvuspbgyspmocztpbftzrlcpkywohfwosu"), 997, 11817, string("dxyrvmnyunqdbvsnehzulrglmkedtfmmnesakexykecdwjxhzlojexdwitrwmbgoffrw"), 6139, string("roghtufyempzfoll"), 4201);
}
