#pragma once

class ClientModeCSNormal;

namespace G
{
	CGlobalVarsBase*		Globals;
	C_CSPlayerResource**	pPR;
	ClientModeCSNormal*		ClientMode;
	C_CSGameRules**			pGameRules;
	CGlowObjectManager*		GlowObjectManager;

	void Init()
	{
		Globals = **reinterpret_cast<CGlobalVarsBase***>(GetVF<uintptr_t>(I::Client, 0) + 0x1F);
		pPR = reinterpret_cast<C_CSPlayerResource**>(Signatures::g_PR);
		ClientMode = **reinterpret_cast<ClientModeCSNormal***>(GetVF<uintptr_t>(I::Client, 10) + 0x5);
		pGameRules = *reinterpret_cast<C_CSGameRules***>(GetVF<uintptr_t>(ClientMode, 20) + 0x7);
		GlowObjectManager = reinterpret_cast<CGlowObjectManager* (*)()>(RelativeToDirect(GetVF<uintptr_t>(ClientMode, 44) + 0x22))();
		//void* ClientState = reinterpret_cast<void*>(**reinterpret_cast<uintptr_t**>(GetVF<uintptr_t>(I::EngineClient, 7) + 0x4) + 0x8);
	}
}