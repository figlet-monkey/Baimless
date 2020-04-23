#pragma once






/*
*	If you want to implement our own input system from the wndproc hook then go ahead but this is pretty clean imo.
*/

#define MAX_SPLITSCREEN_CLIENT_BITS 2
// this should == MAX_JOYSTICKS in InputEnums.h
#define MAX_SPLITSCREEN_CLIENTS	( 1 << MAX_SPLITSCREEN_CLIENT_BITS ) // 4







#define JOYSTICK_BUTTON_INTERNAL( _joystick, _button ) ( JOYSTICK_FIRST_BUTTON + ((_joystick) * JOYSTICK_MAX_BUTTON_COUNT) + (_button) )
#define JOYSTICK_POV_BUTTON_INTERNAL( _joystick, _button ) ( JOYSTICK_FIRST_POV_BUTTON + ((_joystick) * JOYSTICK_POV_BUTTON_COUNT) + (_button) )
#define JOYSTICK_AXIS_BUTTON_INTERNAL( _joystick, _button ) ( JOYSTICK_FIRST_AXIS_BUTTON + ((_joystick) * JOYSTICK_AXIS_BUTTON_COUNT) + (_button) )




template<typename FuncType>
__forceinline static FuncType call_virtual(void* ppClass, int index) {
	int* pVTable = *(int**)ppClass;
	int dwAddress = pVTable[index];
	return (FuncType)(dwAddress);
}
class IInputSystem {
public:
	void EnableInput(bool bEnable) {
		typedef void(__thiscall* OriginalFn)(void*, bool);
		return call_virtual<OriginalFn>(this, 11)(this, bEnable);
	}

	void ResetInputState() {
		typedef void(__thiscall* OriginalFn)(void*);
		return call_virtual<OriginalFn>(this, 39)(this);
	}

	bool IsButtonDown(ButtonCode_t code) {
		typedef bool(__thiscall* OriginalFn)(void*, ButtonCode_t);
		return call_virtual<OriginalFn>(this, 15)(this, code);
	}

	void GetCursorPosition(int* m_pX, int* m_pY) {
		typedef void(__thiscall* OriginalFn)(void*, int*, int*);
		return call_virtual<OriginalFn>(this, 56)(this, m_pX, m_pY);
	}

	const char* ButtonCodeToString(ButtonCode_t ButtonCode) {
		typedef const char*(__thiscall* OriginalFn)(void*, ButtonCode_t);
		return call_virtual<OriginalFn>(this, 40)(this, ButtonCode);
	}

	ButtonCode_t VirtualKeyToButtonCode(int nVirtualKey) {
		typedef ButtonCode_t(__thiscall* OriginalFn)(void*, int);
		return call_virtual<OriginalFn>(this, 45)(this, nVirtualKey);
	}

	int ButtonCodeToVirtualKey(ButtonCode_t code) {
		typedef int(__thiscall* OriginalFn)(void*, ButtonCode_t);
		return call_virtual<OriginalFn>(this, 46)(this, code);
	}
};

extern IInputSystem* g_pInputSystem;