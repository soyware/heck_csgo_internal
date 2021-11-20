#pragma once

namespace Hooks
{
	extern void HookNetChan();

	bool __stdcall ShouldDrawFog()
	{
		return !Settings::Visuals::NoFog.GetInt();
	}

	typedef void(__thiscall* OverrideViewFn)(ClientModeCSNormal*, CViewSetup*);
	OverrideViewFn oOverrideView;

	void __fastcall OverrideView(ClientModeCSNormal* _this, void* edx, CViewSetup* pSetup)
	{
		if (pSetup)
		{
			static const float oFov = pSetup->fov;

			float setting = Settings::Visuals::ViewFov.GetFloat();
			if (thirdPersonOn)
				pSetup->fov = setting ? setting : oFov;
			else if (setting)
			{
				C_CSPlayer* player = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer()));
				if (player)
				{
					if (!player->IsAlive())
						player = static_cast<C_CSPlayer*>(player->GetObserverTarget());

					if (!player || !player->IsScoped())
						pSetup->fov = setting;
				}
			}
		}

		return oOverrideView(_this, pSetup);
	}

	typedef int(__thiscall* KeyInputFn) (ClientModeCSNormal*, int, ButtonCode_t, const char*);
	KeyInputFn oKeyInput;

	int __fastcall KeyInput(ClientModeCSNormal* _this, void* edx, int down, ButtonCode_t keynum, const char* pszCurrentBinding)
	{
		if (keynum)
		{
			if (Settings::Moves::Aim::Triggerbot::Key.GetInt() == keynum)
				Features::Moves::Aim::triggerbotOn = down;

			if (Settings::Moves::AutoBlockKey.GetInt() == keynum)
				Features::Moves::autoBlockLock = down;

			if (down)
			{
				if (Settings::Moves::AntiAim::Enable.GetInt() && pszCurrentBinding && !strcmp(pszCurrentBinding, "drop"))
				{
					C_CSPlayer* player = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer()));
					if (player && player->IsAlive() && !(player->GetFlags() & FL_FROZEN))
					{
						Features::Moves::AntiAim::dropWeapon = true;
						return 0;
					}
				}

				if (Settings::Misc::ThirdPerson::ToggleKey.GetInt() == keynum)
				{
					if (Settings::Misc::ThirdPerson::Enable.GetInt())
						thirdPersonOn = !thirdPersonOn;
				}

				if (Settings::Misc::FakeLag::ToggleKey.GetInt() == keynum)
				{
					if (Settings::Misc::FakeLag::Enable.GetInt())
						Features::Misc::fakeLagOn = !Features::Misc::fakeLagOn;
				}

				if (Settings::Moves::AntiAim::DesyncSwitchSideKey.GetInt() == keynum)
				{
					if (Settings::Moves::AntiAim::Enable.GetInt())
						Features::Moves::AntiAim::realSideRight = !Features::Moves::AntiAim::realSideRight;
				}
			}
		}

		return oKeyInput(_this, down, keynum, pszCurrentBinding);
	}

	bool __stdcall CreateMove(float flInputSampleTime, CUserCmd* cmd)
	{
		Features::Moves::lastCmd = cmd;

		if (!hkNetChan.IsHooked())
			HookNetChan();

		C_CSPlayer* player = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer()));
		if (!player)
			return true;

		static bool wasFrozen = false;

		if (player->GetFlags() & FL_FROZEN)
		{
			if (wasFrozen)
				cmd->viewangles = Features::Moves::lastSentViewangles;
			else
			{
				wasFrozen = true;
				Features::Moves::lastSentViewangles = cmd->viewangles;
			}
			return true;
		}

		wasFrozen = false;

		if (!cmd->command_number) // ExtraMouseSample
			return true;

		// Frame Pointer goes before Return Address
		bool& bSendPacket = *reinterpret_cast<bool*>(*reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) - sizeof(void*)) - 0x1C);

		const bool ret = Features::Moves::Run(player, cmd, bSendPacket);

		if (bSendPacket)
			Features::Moves::lastSentViewangles = cmd->viewangles;

		return ret; // true - SetViewAngles
	}

	typedef void(__thiscall* DoPostScreenSpaceEffectsFn)(ClientModeCSNormal*, const void*);
	DoPostScreenSpaceEffectsFn oDoPostScreenSpaceEffects;

	void __fastcall DoPostScreenSpaceEffects(ClientModeCSNormal* _this, void* edx, const void* pSetup)
	{
		Features::Visuals::Glow();

		oDoPostScreenSpaceEffects(_this, pSetup);
	}
}