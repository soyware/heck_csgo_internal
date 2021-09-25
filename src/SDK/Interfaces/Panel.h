#pragma once

class IVPanel
{
public:
	//inline void SetMouseInputEnabled(unsigned int vguiPanel, bool state)
	//{
	//	typedef void (__thiscall* Fn) (void*, unsigned int, bool);
	//	GetVF<Fn>(this, 32)(this, vguiPanel, state);
	//}

	inline const char* GetName(unsigned int vguiPanel)
	{
		typedef const char* (__thiscall* Fn) (void*, unsigned int);
		return GetVF<Fn>(this, 36)(this, vguiPanel);
	}
};
