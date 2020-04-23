

#pragma once

#include "CommonIncludes.h"

#include <map>

class CControl;
class CTab;
class CWindow;
class CGUI;

extern CGUI GUI;





//up and down
#define UI_WIN_TOPHEIGHT	28
#define UI_WIN_TITLEHEIGHT	32

//Tab Size
#define UI_TAB_WIDTH		89
//TabSpacing
#define UI_TAB_HEIGHT		20

#define UI_WIN_CLOSE_X		20
#define UI_WIN_CLOSE_Y      6

#define UI_CHK_SIZE			16



enum UIFlags
{
	UI_None = 0x00,
	UI_Drawable = 0x01,
	UI_Clickable = 0x02,
	UI_Focusable = 0x04,
	UI_RenderFirst = 0x08,
	UI_SaveFile = 0x10
};

enum UIControlTypes
{
	UIC_CheckBox = 1,
	UIC_Seperate,
	UIC_Slider,
	UIC_KeyBind,
	UIC_ComboBox,
	UIC_ItemSelector,
	UIC_Button,
	UIC_MultiBox,
	UIC_GroupBox,
	UIC_ListBox,
	UIC_ColorSelector,
	UIC_Label,
	UIC_TextField,
	UIC_dropdown

};

// Base class for GUI controls
class CControl
{
	friend class CGUI;
	friend class CTab;
	friend class CWindow;
public:
	void SetPosition(int x, int y);
	void SetSize(int w, int h);
	void GetSize(int &w, int &h);
	void SetFileId(std::string fid);
	int FileControlType;
	int m_iWidth, m_iHeight;

	CControl* parent_group;
	int g_tab = 0;

	bool Flag(int f);
protected:
	int m_x, m_y;
	int m_Flags;
	CWindow* parent;

	std::string FileIdentifier;

	virtual void Draw(bool) = 0;
	virtual void OnUpdate() = 0;
	virtual void OnClick() = 0;

	POINT GetAbsolutePos();
};




// A GUI Control Container
class CTab
{
	friend class CControl;
	friend class CGUI;
	friend class CWindow;
public:
	void SetTitle(std::string name);
	void RegisterControl(CControl* control);
private:
	std::string Title;
	std::vector<CControl*> Controls;
	CWindow* parent;
};

// Base class for a simple GUI window
class CWindow
{
	friend class CControl;
	friend class CGUI;
public:
	void SetPosition(int x, int y);
	void SetSize(int w, int h);
	void SetTitle(std::string title);
	void Open();
	void Close();
	void Toggle();
	bool isOpen = false;
	int get_iWidth()
	{
		return m_iWidth;
	}
	int get_iHeight()
	{
		return m_iHeight;
	}
	int get_x()
	{
		return m_x;
	}
	int get_y()
	{
		return m_y;
	}
	CControl* GetFocus();

	void RegisterTab(CTab* Tab);
	bool m_bIsOpen;

	RECT GetClientArea();
	RECT GetTabArea();
	RECT GetDragArea();
	CControl* FocusedControl;

private:
	void DrawControls();


	std::vector<CTab*> Tabs;
	CTab* SelectedTab;

	bool IsFocusingControl;

	std::string Title;
	int m_x;
	int m_y;
	int m_iWidth;
	int m_iHeight;

};



// User interface manager
class CGUI
{
public:
	CGUI();

	// Draws all windows 
	void Draw();

	// Handle all input etc
	void Update();

	// Draws a single window
	bool DrawWindow(CWindow* window, int menu);

	// Registers a window
	void RegisterWindow(CWindow* window);

	// Config saving/loading
	void SaveWindowState(CWindow* window, std::string Filename);
	void LoadWindowState(CWindow* window, std::string Filename);

	// Window Binds
	void BindWindow(unsigned char Key, CWindow* window);

	// Input
	bool GetKeyPress(unsigned int key);
	bool GetKeyState(unsigned int key);
	bool IsMouseInRegion(int x, int y, int x2, int y2);
	bool IsMouseInRegion(RECT region);
	POINT GetMouse();

private:
	// Input
	// keyboard
	bool keys[256];
	bool oldKeys[256];
	// Mouse
	POINT Mouse;
	bool MenuOpen;

	// Window Dragging
	bool IsDraggingWindow;
	CWindow* DraggingWindow;
	int DragOffsetX; int DragOffsetY;
	bool IsDragging;

	// Windows
	std::vector<CWindow*> Windows;

	// KeyBinds -> Windows Map
	std::map<int, CWindow*> WindowBinds;
};

namespace Globals
{
	extern float MenuAlpha2;
}