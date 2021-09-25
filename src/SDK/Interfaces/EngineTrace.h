#pragma once

struct Ray_t;
class ITraceFilter;
class CGameTrace;
typedef CGameTrace trace_t;

class IEngineTrace
{
public:
	inline void ClipRayToEntity(const Ray_t& ray, unsigned int fMask, C_BaseEntity* pEnt, trace_t* pTrace)
	{
		typedef void(__thiscall* Fn) (IEngineTrace*, const Ray_t&, unsigned int, C_BaseEntity*, trace_t*);
		GetVF<Fn>(this, 3)(this, ray, fMask, pEnt, pTrace);
	}

	inline void TraceRay(const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)
	{
		typedef void(__thiscall* Fn) (IEngineTrace*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
		GetVF<Fn>(this, 5)(this, ray, fMask, pTraceFilter, pTrace);
	}
};