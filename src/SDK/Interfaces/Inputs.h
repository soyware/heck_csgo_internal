#pragma once

enum ButtonCode_t
{
	BUTTON_CODE_INVALID = -1,
	BUTTON_CODE_NONE = 0,

	KEY_FIRST = 0,

	KEY_NONE = KEY_FIRST,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_PAD_0,
	KEY_PAD_1,
	KEY_PAD_2,
	KEY_PAD_3,
	KEY_PAD_4,
	KEY_PAD_5,
	KEY_PAD_6,
	KEY_PAD_7,
	KEY_PAD_8,
	KEY_PAD_9,
	KEY_PAD_DIVIDE,
	KEY_PAD_MULTIPLY,
	KEY_PAD_MINUS,
	KEY_PAD_PLUS,
	KEY_PAD_ENTER,
	KEY_PAD_DECIMAL,
	KEY_LBRACKET,
	KEY_RBRACKET,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_BACKQUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_BACKSLASH,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_ENTER,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_ESCAPE,
	KEY_SCROLLLOCK,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_BREAK,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LALT,
	KEY_RALT,
	KEY_LCONTROL,
	KEY_RCONTROL,
	KEY_LWIN,
	KEY_RWIN,
	KEY_APP,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_CAPSLOCKTOGGLE,
	KEY_NUMLOCKTOGGLE,
	KEY_SCROLLLOCKTOGGLE,

	KEY_LAST = KEY_SCROLLLOCKTOGGLE,
	KEY_COUNT = KEY_LAST - KEY_FIRST + 1,

	// Mouse
	MOUSE_FIRST = KEY_LAST + 1,

	MOUSE_LEFT = MOUSE_FIRST,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_4,
	MOUSE_5,
	MOUSE_WHEEL_UP,		// A fake button which is 'pressed' and 'released' when the wheel is moved up 
	MOUSE_WHEEL_DOWN,	// A fake button which is 'pressed' and 'released' when the wheel is moved down

	MOUSE_LAST = MOUSE_WHEEL_DOWN,
	MOUSE_COUNT = MOUSE_LAST - MOUSE_FIRST + 1,

	BUTTON_CODE_LAST = MOUSE_LAST,
	BUTTON_CODE_COUNT = BUTTON_CODE_LAST - KEY_FIRST + 1,
};

class IInputSystem
{
public:
	//inline void EnableInput(bool Enable)
	//{
	//	typedef void(__thiscall* Fn) (void*, bool);
	//	GetVF<Fn>(this, 11)(this, Enable);
	//}

	//inline bool IsButtonDown(int code)
	//{
	//	typedef bool(__thiscall* Fn) (void*, int);
	//	return GetVF<Fn>(this, 15)(this, code);
	//}

	//inline const char* ButtonCodeToString(ButtonCode_t code)
	//{
	//	typedef const char* (__thiscall* Fn) (void*, ButtonCode_t);
	//	return GetVF<Fn>(this, 39)(this, code);
	//}

	//inline void GetCursorPosition(int* pX, int* pY)
	//{
	//	typedef void(__thiscall* Fn) (void*, int*, int*);
	//	GetVF<Fn>(this, 56)(this, pX, pY);
	//}

	//inline void SetMouseCursorVisible(bool visible)
	//{
	//	typedef void(__thiscall* Fn) (void*, bool);
	//	GetVF<Fn>(this, 59)(this, visible);
	//}
};

class IInput
{
public:
	//inline void ActivateMouse()
	//{
	//	typedef void(__thiscall* Fn) (void*);
	//	GetVF<Fn>(this, 23)(this);
	//}

	//inline void DeactivateMouse()
	//{
	//	typedef void(__thiscall* Fn) (void*);
	//	GetVF<Fn>(this, 24)(this);
	//}

	inline int CAM_IsThirdPerson(int nSlot = -1)
	{
		typedef int(__thiscall* Fn) (void*, int);
		return GetVF<Fn>(this, 32)(this, nSlot);
	}

	inline void CAM_ToThirdPerson()
	{
		typedef void(__thiscall* Fn) (void*);
		GetVF<Fn>(this, 35)(this);
	}

	inline void CAM_ToFirstPerson()
	{
		typedef void(__thiscall* Fn) (void*);
		GetVF<Fn>(this, 36)(this);
	}
};