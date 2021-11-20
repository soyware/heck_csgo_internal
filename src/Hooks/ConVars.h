#pragma once

namespace Hooks
{
	typedef int(__thiscall* GetIntFn)(ConVar*);

	GetIntFn o_weapon_debug_spread_show_GetInt;

	// TODO: this is unnessesary we can simply set weapon_debug_spread_show to 3, it won't have any repercussions
	int __fastcall weapon_debug_spread_show_GetInt(ConVar* _this)
	{
		if (Settings::Visuals::Crosshair::Snipers.GetInt() &&
			reinterpret_cast<uintptr_t>(_ReturnAddress()) == Signatures::DrawCrosshair_weapon_debug_spread_show)
		{
			C_CSPlayer* player = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer()));
			if (player && player->IsAlive() && !player->IsScoped())
				return 3;
		}

		return o_weapon_debug_spread_show_GetInt(_this);
	}

	bool thirdPersonOn = false;

	GetIntFn o_sv_cheats_GetInt;

	int __fastcall sv_cheats_GetInt(ConVar* _this)
	{
		if (Settings::Misc::ThirdPerson::Enable.GetInt() &&
			(reinterpret_cast<uintptr_t>(_ReturnAddress()) == Signatures::CAM_Think_sv_cheats))
		{
			C_CSPlayer* player = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer()));
			if (player)
			{
				C_WeaponCSBaseGun* weapon = player->GetActiveWeapon();

				int on = (!Settings::Misc::ThirdPerson::ToggleKey.GetInt() || thirdPersonOn) &&
					(Settings::Misc::ThirdPerson::OnGrenade.GetInt() || (!weapon || weapon->GetCSWpnData().m_WeaponType != WEAPONTYPE_GRENADE));

				static int oMode = OBS_MODE_NONE;

				if (player->GetObserverTarget())
				{
					if (on)
					{
						if (oMode == OBS_MODE_NONE)
							oMode = player->GetObserverMode();

						player->SetObserverMode(OBS_MODE_CHASE);
					}
					else if (oMode != OBS_MODE_NONE)
						player->SetObserverMode(oMode);

					return 0;
				}
				else
				{
					oMode = OBS_MODE_NONE;

					if (on)
						I::Input->CAM_ToThirdPerson();

					return on;
				}
			}
		}

		return o_sv_cheats_GetInt(_this);
	}

	GetIntFn o_developer_GetInt;

	int __fastcall developer_GetInt(ConVar* _this)
	{
		uintptr_t retAdd = reinterpret_cast<uintptr_t>(_ReturnAddress());
		if (retAdd == Signatures::DrawNotify_developer)
			return 1;
		else if (retAdd == Signatures::AddToNotify_developer)
		{
			uintptr_t fp;
			__asm { mov fp, ebp };

			const uintptr_t CConPanel__AddToNotify__stack = *reinterpret_cast<uintptr_t*>(fp);

			char** msg = reinterpret_cast<char**>(CConPanel__AddToNotify__stack + 0xC);

			if (!strncmp(*msg, "[heck]", 6))
			{
				*msg += 7;
				return 1;
			}
		}
		else if (retAdd == Signatures::Con_ColorPrint_developer)
		{
			uintptr_t fp;
			__asm { mov fp, ebp };

			const uintptr_t Con_ColorPrint__stack = *reinterpret_cast<uintptr_t*>(fp);

			Color* localClr = *reinterpret_cast<Color**>(Con_ColorPrint__stack - 0x10);

			const uintptr_t CEngineConsoleLoggingListener__Log__stack = **reinterpret_cast<uintptr_t**>(Con_ColorPrint__stack);

			const Log::LoggingContext_t* context = *reinterpret_cast<const Log::LoggingContext_t**>(CEngineConsoleLoggingListener__Log__stack + 8);

			if (context->m_ChannelID != Log::LOG_HECK)
				*localClr = Color::Grey();
		}

		return o_developer_GetInt(_this);
	}

	GetIntFn o_net_showfragments_GetInt;

	int __fastcall net_showfragments_GetInt(ConVar* _this)
	{
		if (I::EngineClient->IsInGame() && 
			Settings::Moves::Aim::Backtrack::Enable.GetInt() &&
			Settings::Moves::Aim::Backtrack::Extend.GetInt())
		{
			const uintptr_t retAddr = reinterpret_cast<uintptr_t>(_ReturnAddress());

			static unsigned int lastFragment = 0;

			if (retAddr == Signatures::ReadSubChannelData_net_showfragments)
			{
				if (lastFragment)
				{
					INetChannel* nc = I::EngineClient->GetNetChannelInfo();
					if (nc)
					{
						dataFragments_t& data = nc->GetReceiveList(FRAG_NORMAL_STREAM);
						
						if (data.bytes == lastFragment)
							data.buffer = nullptr;
					}
				}
			}
			else if (retAddr == Signatures::CheckReceivingList_net_showfragments)
			{
				INetChannel* nc = I::EngineClient->GetNetChannelInfo();
				if (nc)
				{
					dataFragments_t& data = nc->GetReceiveList(FRAG_NORMAL_STREAM);
					lastFragment = data.bytes;
				}
			}
		}
		return o_net_showfragments_GetInt(_this);
	}
}