#pragma once

class C_CSPlayerResource
{
public:
	inline int GetKills(int index) { return reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iKills)[index]; }
	inline int GetAssists(int index) { return reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iAssists)[index]; }
	inline int GetDeaths(int index) { return reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iDeaths)[index]; }
	inline bool IsConnected(int index) { return reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bConnected)[index]; }
	inline int GetTeam(int index) { return reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iTeam)[index]; }
	inline int GetC4Player() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iPlayerC4); }
	inline int GetNumMVPs(int index) { return reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iMVPs)[index]; }
	inline int GetScore(int index) { return reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iScore)[index]; }
	inline int GetCompetitiveWins(int index) { return reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iCompetitiveWins)[index]; }
	inline int GetCompTeammateColorId(int index) { return reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iCompTeammateColor)[index]; }

	inline Color GetCompTeammateColor(int index)
	{
		if (GetBotDifficulty(index) != -1)
			return Color::LightGrey();

		switch (GetCompTeammateColorId(index))
		{
		case 0: return Color::LightYellow();
		case 1: return Color::LightPurple();
		case 2: return Color::LightGreen();
		case 3: return Color::LightBlue();
		case 4: return Color::LightOrange();
		default: return Color::LightGrey();
		}
	}

	inline int GetBotDifficulty(int index) { return reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iBotDifficulty)[index]; }
	// private rank
	inline int GetPersonaDataPublicLevel(int index) { return reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nPersonaDataPublicLevel)[index]; }
	inline int GetPersonaDataPublicCommendsLeader(int index) { return reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nPersonaDataPublicCommendsLeader)[index]; }
	inline int GetPersonaDataPublicCommendsTeacher(int index) { return reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nPersonaDataPublicCommendsTeacher)[index]; }
	inline int GetPersonaDataPublicCommendsFriendly(int index) { return reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nPersonaDataPublicCommendsFriendly)[index]; }

	inline int GetDamageTotal(int index) { return reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iMatchStats_Damage_Total)[index]; }
};