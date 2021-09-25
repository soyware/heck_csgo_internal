#pragma once

// handle to a KeyValues key name symbol
typedef int HKeySymbol;
#define INVALID_KEY_SYMBOL (-1)

class KeyValues;

class IKeyValuesSystem
{
public:
	inline KeyValues* AllocKeyValuesMemory(size_t size)
	{
		typedef KeyValues* (__thiscall* Fn) (IKeyValuesSystem*, size_t);
		return GetVF<Fn>(this, 1)(this, size);
	}

	inline void FreeKeyValuesMemory(void* pMem)
	{
		typedef void(__thiscall* Fn) (IKeyValuesSystem*, void*);
		GetVF<Fn>(this, 2)(this, pMem);
	}

	inline HKeySymbol GetSymbolForString(const char* name, bool bCreate = true)
	{
		typedef HKeySymbol(__thiscall* Fn) (IKeyValuesSystem*, const char*, bool);
		return GetVF<Fn>(this, 3)(this, name, bCreate);
	}

	inline const char* GetStringForSymbol(HKeySymbol symbol)
	{
		typedef const char* (__thiscall* Fn) (IKeyValuesSystem*, HKeySymbol);
		return GetVF<Fn>(this, 4)(this, symbol);
	}
};