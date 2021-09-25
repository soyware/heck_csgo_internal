#pragma once

namespace Hooks
{
	typedef int(__thiscall* EmitSoundFn) (IEngineSound*, IRecipientFilter&, int, int, const char*, HSOUNDSCRIPTHASH, const char*,
		float, soundlevel_t, int, int, int, const vec3*, const vec3*, CUtlVector<vec3>*, bool, float, int, CSoundParameters*);

	EmitSoundFn oEmitSound;

	int __fastcall EmitSound(IEngineSound* _this, void* _edx, IRecipientFilter& filter, int iEntIndex, int iChannel,
		const char* pSoundEntry, HSOUNDSCRIPTHASH nSoundEntryHash, const char* pSample,
		float flVolume, soundlevel_t iSoundlevel, int nSeed, int iFlags = 0, int iPitch = PITCH_NORM,
		const vec3* pOrigin = nullptr, const vec3* pDirection = nullptr, CUtlVector<vec3>* pUtlVecOrigins = nullptr, 
		bool bUpdatePositions = true, float soundtime = 0.f, int speakerentity = -1, CSoundParameters* params = nullptr)
	{
		if (!I::EngineClient->IsInGame() && !strcmp(pSoundEntry, "UIPanorama.popup_accept_match_beep"))
		{
			reinterpret_cast<bool(__stdcall*)(const char*)>(Signatures::SetLocalPlayerReady)("");
			return 0;
		}

		return oEmitSound(_this, filter, iEntIndex, iChannel, 
			pSoundEntry, nSoundEntryHash, pSample, 
			flVolume, iSoundlevel, nSeed, iFlags, iPitch, 
			pOrigin, pDirection, pUtlVecOrigins, 
			bUpdatePositions, soundtime, speakerentity, params);
	}
}