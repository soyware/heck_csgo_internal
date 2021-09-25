#pragma once

struct CDemoPlaybackParameters_t
{
	uint64_t m_uiCaseID;
	uint32_t m_uiHeaderPrefixLength;
	uint32_t m_uiLockFirstPersonAccountID;
	bool m_bAnonymousPlayerIdentity;
	uint32_t m_numRoundSkip;
	uint32_t m_numRoundStop;
	bool m_bSkipWarmup;
	bool m_bPlayingLiveRemoteBroadcast;
	uint64_t m_uiLiveMatchID;
};

namespace Hooks
{
	typedef bool (__thiscall* IsPlayingDemoFn)(IVEngineClient*);
	IsPlayingDemoFn oIsPlayingDemo;

	bool __fastcall IsPlayingDemo(IVEngineClient* _this)
	{
		if (reinterpret_cast<uintptr_t>(_ReturnAddress()) == Signatures::IsLoadoutAllowed_IsPlayingDemo)
			return true;

		return oIsPlayingDemo(_this);
	}

	typedef void (__thiscall* ServerCmdKeyValuesFn)(IVEngineClient*, KeyValues*);
	ServerCmdKeyValuesFn oServerCmdKeyValues;

	void __fastcall ServerCmdKeyValues(IVEngineClient* _this, void* edx, KeyValues* pKeyValues)
	{
		if (Settings::Misc::FakeCrosshair::Enable.GetInt() && !strcmp(pKeyValues->GetName(), "CrosshairCode"))
			pKeyValues->SetString("code", Settings::Misc::FakeCrosshair::Code.GetString());

		return oServerCmdKeyValues(_this, pKeyValues);
	}

	typedef CDemoPlaybackParameters_t* (__thiscall* GetDemoPlaybackParametersFn)(IVEngineClient*);
	GetDemoPlaybackParametersFn oGetDemoPlaybackParameters;

	CDemoPlaybackParameters_t* __fastcall GetDemoPlaybackParameters(IVEngineClient* _this)
	{
		CDemoPlaybackParameters_t* const original = oGetDemoPlaybackParameters(_this);
		uintptr_t retAddr = reinterpret_cast<uintptr_t>(_ReturnAddress());

		if (!original || 
			(retAddr == Signatures::C_HLTVCamera__Update_GetDemoPlaybackParameters) ||
			(retAddr == Signatures::CCStrike15BasePanel__OnEvent_GetDemoPlaybackParameters))
			return original;

		static CDemoPlaybackParameters_t copy;
		copy = *original;
		copy.m_bAnonymousPlayerIdentity = false;

		return &copy;
	}
}