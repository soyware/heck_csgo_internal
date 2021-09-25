#pragma once

class IVDebugOverlay
{
public:
	inline void AddBoxOverlay(const Vector& origin, const Vector& mins, const Vector& maxs, const QAngle& orientation, int r, int g, int b, int a, float duration)
	{
		typedef void(__thiscall* Fn) (IVDebugOverlay*, const Vector&, const Vector&, const Vector&, const QAngle&, int, int, int, int, float);
		GetVF<Fn>(this, 1)(this, origin, mins, maxs, orientation, r, g, b, a, duration);
	}

	inline void AddTriangleOverlay(const Vector& p1, const Vector& p2, const Vector& p3, int r, int g, int b, int a, bool noDepthTest, float duration)
	{
		typedef void(__thiscall* Fn) (IVDebugOverlay*, const Vector&, const Vector&, const Vector&, int, int, int, int, bool, float);
		GetVF<Fn>(this, 3)(this, p1, p2, p3, r, g, b, a, noDepthTest, duration);
	}

	inline bool ScreenPosition(const vec3& point, vec3& screen)
	{
		typedef int(__thiscall* Fn) (IVDebugOverlay*, const vec3&, vec3&);
		return !GetVF<Fn>(this, 13)(this, point, screen);
	}

	inline void ClearDeadOverlays()
	{
		typedef void(__thiscall* Fn) (IVDebugOverlay*);
		GetVF<Fn>(this, 16)(this);
	}

	inline void ClearAllOverlays()
	{
		typedef void(__thiscall* Fn) (IVDebugOverlay*);
		GetVF<Fn>(this, 17)(this);
	}

	inline void AddLineOverlayAlpha(const vec3& origin, const vec3& dest, int r, int g, int b, int a, bool noDepthTest, float duration)
	{
		typedef void(__thiscall* Fn) (IVDebugOverlay*, const vec3&, const vec3&, int, int, int, int, bool, float);
		GetVF<Fn>(this, 20)(this, origin, dest, r, g, b, a, noDepthTest, duration);
	}

	inline void AddCapsuleOverlay(const Vector& vMin, const Vector& vMax, const float& flRadius, int r, int g, int b, int a, float flDuration)
	{
		typedef void(__thiscall* Fn) (IVDebugOverlay*, const vec3&, const vec3&, const float&, int, int, int, int, float);
		GetVF<Fn>(this, 24)(this, vMin, vMax, flRadius, r, g, b, a, flDuration);
	}
};
