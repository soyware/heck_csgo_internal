#pragma once

class C_BaseEntity;

class IClientEntityList
{
public:
	inline C_BaseEntity* GetClientEntity(int EntNum)
	{
		typedef C_BaseEntity* (__thiscall* Fn) (void*, int);
		return GetVF<Fn>(this, 3)(this, EntNum);
	}

	inline C_BaseEntity* GetClientEntityFromHandle(ULONG hEnt)
	{
		typedef C_BaseEntity* (__thiscall* Fn) (void*, ULONG);
		return GetVF<Fn>(this, 4)(this, hEnt);
	}

	inline int GetHighestEntityIndex()
	{
		typedef int(__thiscall* Fn) (void*);
		return GetVF<Fn>(this, 6)(this);
	}
};