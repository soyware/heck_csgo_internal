#pragma once

class C_CSGameRules
{
public:
	inline bool IsQueuedMatchmaking() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bIsQueuedMatchmaking); }
	inline bool IsPlayingAnyCompetitiveStrictRuleset()
	{ 
		static ConVar* game_type = I::Cvar->FindVar("game_type");
		static ConVar* game_mode = I::Cvar->FindVar("game_mode");
		return (!game_type->GetInt() && game_mode->GetInt() == 1);
	}
	inline int GetQueuedMatchmakingMode() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nQueuedMatchmakingMode); }
	inline bool IsWarmupPeriod() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bWarmupPeriod); }
	inline bool IsFreezePeriod() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bFreezePeriod); }
	inline int GetRoundLength() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iRoundTime); }

	inline bool	ShouldCollide(int collisionGroup0, int collisionGroup1)
	{
		typedef bool(__thiscall* Fn) (C_CSGameRules*, int, int);
		return GetVF<Fn>(this, 28)(this, collisionGroup0, collisionGroup1);
	}
};