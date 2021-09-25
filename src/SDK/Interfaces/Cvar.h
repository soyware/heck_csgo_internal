#pragma once

class ConCommandBase;
class ConVar;
class ConCommand;

class ICvar
{
public:
	inline void RegisterConCommand(ConCommandBase* pCommandBase)
	{
		typedef void(__thiscall* Fn) (void*, ConCommandBase*);
		GetVF<Fn>(this, 10)(this, pCommandBase);
	}

	inline void UnregisterConCommand(ConCommandBase* pCommandBase)
	{
		typedef void(__thiscall* Fn) (void*, ConCommandBase*);
		GetVF<Fn>(this, 11)(this, pCommandBase);
	}

	inline ConCommandBase* FindCommandBase(const char* varName)
	{
		typedef ConCommandBase* (__thiscall* Fn) (void*, const char*);
		return GetVF<Fn>(this, 14)(this, varName);
	}

	inline ConVar* FindVar(const char* varName)
	{
		typedef ConVar* (__thiscall* Fn) (void*, const char*);
		return GetVF<Fn>(this, 16)(this, varName);
	}

	inline ConCommand* FindCommand(const char* varName)
	{
		typedef ConCommand* (__thiscall* Fn) (void*, const char*);
		return GetVF<Fn>(this, 18)(this, varName);
	}
};
