#pragma once

class IVRenderView
{
public:
	inline void SetBlend(float blend) // all next models
	{
		typedef void(__thiscall* Fn) (void*, float);
		GetVF<Fn>(this, 4)(this, blend);
	}

	inline float GetBlend()
	{
		typedef float(__thiscall* Fn) (void*);
		return GetVF<Fn>(this, 5)(this);
	}

	inline void SetColorModulation(float const* color) // all next models
	{
		typedef void(__thiscall* Fn) (void*, float const*);
		GetVF<Fn>(this, 6)(this, color);
	}

	inline void GetColorModulation(float* color)
	{
		typedef void(__thiscall* Fn) (void*, float*);
		GetVF<Fn>(this, 7)(this, color);
	}
};