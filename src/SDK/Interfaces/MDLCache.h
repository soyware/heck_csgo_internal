#pragma once

class IMDLCache
{
public:
	inline MDLHandle_t FindMDL(const char* pMDLRelativePath)
	{
		typedef MDLHandle_t (__thiscall* Fn)(IMDLCache*, const char*);
		return GetVF<Fn>(this, 10)(this, pMDLRelativePath);
	}

	inline studiohdr_t* LockStudioHdr(MDLHandle_t handle)
	{
		typedef studiohdr_t* (__thiscall* Fn)(IMDLCache*, MDLHandle_t);
		return GetVF<Fn>(this, 48)(this, handle);
	}

	inline void UnlockStudioHdr(MDLHandle_t handle)
	{
		typedef void (__thiscall* Fn)(IMDLCache*, MDLHandle_t);
		GetVF<Fn>(this, 49)(this, handle);
	}
};