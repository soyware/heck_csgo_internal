#pragma once

enum ClientFrameStage_t
{
	FRAME_START,
	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,
	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

namespace Hooks
{
	typedef void(__thiscall* FrameStageNotifyFn) (IBaseClient*, ClientFrameStage_t);
	FrameStageNotifyFn oFrameStageNotify;

	void __fastcall FrameStageNotify(IBaseClient* _this, void* _edx, ClientFrameStage_t stage)
	{
		vec3 oViewPunch;
		vec3 oAimPunch;

		if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			if (Settings::Misc::SkinChanger::Enable.GetInt())
			{
				C_CSPlayer* player = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer()));
				if (player && player->IsAlive())
				{
					for (ULONG* hWeapon = player->GetWeapons(); *hWeapon != -1; ++hWeapon)
					{
						C_WeaponCSBaseGun* weapon = static_cast<C_WeaponCSBaseGun*>(I::EntityList->GetClientEntityFromHandle(*hWeapon));
						if (!weapon)
							continue;

						player_info_t info;
						if (!I::EngineClient->GetPlayerInfo(player->entindex(), &info) || (weapon->GetOriginalOwnerXuid() != info.xuid))
							continue;

						Features::Misc::SkinChanger::ApplyKnife(weapon, player);
						Features::Misc::SkinChanger::ApplySkin(weapon);
					}
				}
			}
		}
		else if (stage == FRAME_RENDER_START)
		{
			C_CSPlayer* player = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer()));
			if (player && player->IsAlive())
			{
				if (I::Input->CAM_IsThirdPerson())
				{
					if (Settings::Moves::AntiAim::Enable.GetInt())
						player->SetVAngle(Features::Moves::lastSentViewangles);
				}
				else
				{
					if (Settings::Visuals::NoVisualRecoil.GetInt())
					{
						static const vec3 zeroVec;
						oViewPunch = player->GetViewPunch();
						player->SetViewPunch(zeroVec);
						if (Settings::Visuals::NoVisualRecoil.GetInt() == 2)
						{
							oAimPunch = player->GetAimPunch();
							player->SetAimPunch(zeroVec);
						}
					}
				}
			}

			if (Settings::Visuals::NoSmoke.GetInt())
				*reinterpret_cast<int*>(Signatures::g_nSmokeCount) = 0;
		}

		oFrameStageNotify(_this, stage);

		if (stage == FRAME_NET_UPDATE_END)
		{
			if (Settings::Moves::Aim::Backtrack::Enable.GetInt())
			{
				C_CSPlayer* player = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer()));
				if (player && player->IsAlive())
					Features::Moves::Aim::Backtrack::Record(player, G::Globals->tickcount);
			}
		}
		else if (stage == FRAME_RENDER_START)
		{
			if (Settings::Visuals::NoVisualRecoil.GetInt())
			{
				if (!I::Input->CAM_IsThirdPerson())
				{
					C_CSPlayer* player = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer()));
					if (player && player->IsAlive())
					{
						player->SetViewPunch(oViewPunch);
						if (Settings::Visuals::NoVisualRecoil.GetInt() == 2)
							player->SetAimPunch(oAimPunch);
					}
				}
			}
		}
	}

	typedef const char*(__thiscall* GetRichPresenceStatusStringFn) (IBaseClient*);
	GetRichPresenceStatusStringFn oGetRichPresenceStatusString;

	const char* __fastcall GetRichPresenceStatusString(IBaseClient* _this)
	{
		const char* status = Settings::Misc::SteamStatus.GetString();
		if (!status[0])
			return oGetRichPresenceStatusString(_this);

		ISteamFriends* steamFriends = **reinterpret_cast<ISteamFriends***>(reinterpret_cast<uintptr_t>(oGetRichPresenceStatusString) + 13);
		if (steamFriends)
		{
			const char* keyStart = status;
			char key[k_cchMaxRichPresenceKeyLength];
			char value[k_cchMaxRichPresenceValueLength];
			for (int i = 0; i < k_cchMaxRichPresenceKeys; ++i)
			{
				const char* keyEnd = strchr(keyStart, '=');
				if (!keyEnd)
					break;

				const char* valueStart = keyEnd + 1;
				const char* valueEnd = strchr(valueStart, ',');
				if (!valueEnd)
				{
					valueEnd = valueStart;
					while (valueEnd[0])
						++valueEnd;
				}

				strncpy_s(key, sizeof(key), keyStart, (keyEnd - keyStart));
				strncpy_s(value, sizeof(value), valueStart, (valueEnd - valueStart));

				steamFriends->SetRichPresence(key, value);

				if (!valueEnd[0])
					break;

				keyStart = valueEnd + 1;
			}

			/*
			steamFriends->SetRichPresence("game:mode", "deathmatch");
			steamFriends->SetRichPresence("game:map", "Random");
			steamFriends->SetRichPresence("game:score", "Gas Chamber");
			steamFriends->SetRichPresence("steam_display", "#display_GameKnownMapScore");
			steamFriends->SetRichPresence("steam_player_group", "0");
			steamFriends->SetRichPresence("steam_player_group_size", "6000001");
			*/
		}
		return "Playing CS:GO";
	}
}