

#pragma once

#include "GUI.h"

class CCheckBox : public CControl
{
public:
	CCheckBox();
	void SetState(bool s);
	bool GetState();
	bool GetIsSub();
	void SetAsSub(bool);
protected:
	bool bIsSub;
	bool Checked;
	bool last_state;
	bool should_animate;
	int animation_state;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CButtonLoad : public CControl
{
public:
	typedef void(*ButtonCallback_t)(void);
	CButtonLoad();
	void SetCallback(ButtonCallback_t callback);
	void SetText(std::string text);
protected:
	ButtonCallback_t CallBack;
	std::string Text;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CButtonSave : public CControl
{
public:
	typedef void(*ButtonCallback_t)(void);
	CButtonSave();
	void SetCallback(ButtonCallback_t callback);
	void SetText(std::string text);
protected:
	ButtonCallback_t CallBack;
	std::string Text;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CSeperate : public CControl
{
public:
	CSeperate();
protected:
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};
class CComboBox2 : public CControl
{
public:
	CComboBox2();
	void AddItem(std::string text);
	void SelectIndex(int idx);
	int GetIndex();
	void SetIndex(int);
	std::string GetItem();
protected:
	std::vector<std::string> Items;
	int SelectedIndex;
	bool IsOpen;
	int winWidth;
	int winHeight;
	bool Checked;
	bool should_animate;
	bool should_animate1;
	bool last_state;
	int animation_state;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};
class CLabel : public CControl
{
public:
	CLabel();
	void SetText(std::string text);
protected:
	std::string Text;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CGroupTab
{
public:
	std::string name;
	int id;
public:
	CGroupTab(std::string _name, int _id)
	{
		this->name = _name;
		this->id = _id;
	}
};

class CGroupBox : public CControl
{
public:
	CGroupBox();
	void SetText(std::string text);
	void PlaceCheckBox(std::string Label, CTab * Tab, CControl * control);
	void PlaceLabledControl(int g_tab, std::string Label, CTab *Tab, CControl* control);
	void AddTab(CGroupTab t);
	int selected_tab = 0;
	std::vector<CGroupTab> group_tabs;
protected:
	std::vector<int> reset_tabs;
	int Items;
	std::string Text;
	int ItemSpacing;
	float iYAdd;
	int last_y;
	int last_control_height;
	bool initialized = false;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CGroupBoxicons : public CControl
{
public:
	CGroupBoxicons();
	void SetText(std::string text);
	void PlaceCheckBox(std::string Label, CTab * Tab, CControl * control);
	void PlaceLabledControl(int g_tab, std::string Label, CTab *Tab, CControl* control);
	void AddTab(CGroupTab t);
	int selected_tab = 0;
	std::vector<CGroupTab> group_tabs;
protected:
	std::vector<int> reset_tabs;
	int Items;
	std::string Text;
	int ItemSpacing;
	float iYAdd;
	int last_y;
	int last_control_height;
	bool initialized = false;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

namespace Menulooks {
	extern float MenuAlpha;
}


enum SliderFormat
{
	FORMAT_INT = 1,
	FORMAT_DECDIG1,
	FORMAT_DECDIG2
};

class CSlider : public CControl
{
public:
	CSlider();
	float GetValue();
	void SetValue(float v);
	void SetBoundaries(float min, float max);
	void SetFormat(SliderFormat type);
	char* extension = "";
protected:
	float Value;
	float Min;
	float Max;
	int Format;
	bool DoDrag;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CSliderFloat : public CControl
{
public:
	CSliderFloat();
	float GetValue();
	void SetValue(float v);
	void SetBoundaries(float min, float max);
	void SetFormat(SliderFormat type);
protected:
	float Value;
	float Min;
	float Max;
	int format;
	bool DoDrag;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};


enum ItemSelector_UpdateType
{
	UPDATE_NONE = 1,
	UPDATE_LEFT,
	UPDATE_RIGHT
};
enum ItemSelector_Type
{
	ITEMSELECTOR_INT = 1,
	ITEMSELECTOR_STRING
};

class CItemSelector : public CControl
{
public:
	CItemSelector();
	float GetInt();
	void SetValue(float value);
	void SetMin(float min);
	void SetMax(float max);
	void AddItem(std::string string);
	void SetType(ItemSelector_Type type);
protected:
	float Value;
	float Min;
	float Max;
	bool IsHolding;
	int preTicks;
	std::vector<std::string> Items;
	ItemSelector_Type Type;
	ItemSelector_UpdateType UpdateType;
	void Draw(bool Hover);
	void Manager();
	void OnUpdate();
	void OnClick();
};

class CKeyBind : public CControl
{
public:
	CKeyBind();
	int GetKey();
	void SetKey(int k);
protected:
	bool Getting_New_Key;
	char* text;
	int key = -1;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};
class CButton : public CControl
{
public:
	typedef void(*ButtonCallback_t)(void);
	CButton();
	void SetCallback(ButtonCallback_t callback);
	void SetText(std::string text);
protected:
	ButtonCallback_t CallBack;
	std::string Text;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CComboBox : public CControl
{
public:
	CComboBox();
	void AddItem(std::string text);
	void SelectIndex(int idx);
	int GetIndex();
	void SetIndex(int);
	std::string GetItem();
protected:
	std::vector<std::string> Items;
	int SelectedIndex;
	bool IsOpen;
	int winWidth;
	int winHeight;
	bool Checked;
	bool should_animate;
	bool should_animate1;
	bool last_state;
	int animation_state;
	std::vector<CWindow*> Windowowo;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};
class CTextField : public CControl
{
public:
	CTextField();
	std::string getText();
	void SetText(std::string stext);
private:
	std::string text;
	bool IsGettingKey;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

struct MultiBoxItem
{
	bool bselected;
	const char* text;

	MultiBoxItem(bool _bselected, const char* _text)
	{
		this->bselected = _bselected;
		this->text = _text;
	}
};

class CMultiBox : public CControl
{
public:
	CMultiBox();
	std::vector<MultiBoxItem> items;
	void SetTitle(const char*);
private:
	const char* title;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};
struct multicombo_box_stuff
{
	bool bselected;
	const char* text;

	multicombo_box_stuff(bool _bselected, const char* _text)
	{
		this->bselected = _bselected;
		this->text = _text;
	}
};

struct dropdownboxitem
{
	bool GetSelected;
	const char* text;

	dropdownboxitem(bool _bselected, const char* _text)
	{
		this->GetSelected = _bselected;
		this->text = _text;
	}
};

class CDropBox : public CControl // so the people who have never seen anything other than ayyware can get used to the name
{
public:
	CDropBox();
	char* titleXD;
	std::vector<dropdownboxitem> items;
	std::vector<dropdownboxitem> selecteditems;
protected:
	const char* title;
	std::string toDraw;
	void SetTitle(const char*);

	int SelectedIndex;
	bool IsOpen;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
	//	const char* text;
};


class CListBox : public CControl
{
public:
	CListBox();
	void AddItem(std::string text, int value = 0);
	inline int GetIndex() { return SelectedIndex; }
	std::string GetItem();
	inline int GetValue() { if (SelectedIndex >= 0 && SelectedIndex < Items.size()) return Values[SelectedIndex]; else return -1; };
	inline void ClearItems() { Items.clear(); Values.clear(); }
	void SetHeightInItems(int items);
	inline void SelectItem(int idx) { if (idx >= 0 && idx < Items.size()) SelectedIndex = idx; }
private:
	std::vector<std::string> Items;
	std::vector<int> Values;

	int ScrollTop;
	int SelectedIndex;

	bool dragging;

	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CColorSelector : public CControl
{
public:
	CColorSelector();
	int* GetValue() {
		return color;
	}
	bool DoDrag;
	float Value;
	float Min;
	float Max;
	void SetColor(int r, int g, int b, int a)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
		color[3] = a;
	}

protected:
	int color[4] = { 0, 110, 255, 255 };
	bool is_open;
	bool set_new_pos = false;
	bool toggle = false;
	float brightness = 1.f;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

