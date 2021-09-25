#pragma once

#define KNIFE_ANIM_FIX 0

namespace Features
{
	namespace Misc
	{
		namespace SkinChanger
		{
			struct { ItemDefinitionIndex_t index; const char* model; }
			knifes[] = {
				// Knives which dont need anim fix
				{ WEAPON_KNIFE_BAYONET,			"models/weapons/v_knife_bayonet.mdl" },
				{ WEAPON_KNIFE_CSS,				"models/weapons/v_knife_css.mdl" },
				{ WEAPON_KNIFE_FLIP,			"models/weapons/v_knife_flip.mdl" },
				{ WEAPON_KNIFE_GUT,				"models/weapons/v_knife_gut.mdl" },
				{ WEAPON_KNIFE_KARAMBIT,		"models/weapons/v_knife_karam.mdl" },
				{ WEAPON_KNIFE_M9_BAYONET,		"models/weapons/v_knife_m9_bay.mdl" },
				{ WEAPON_KNIFE_TACTICAL,		"models/weapons/v_knife_tactical.mdl" },

				// Knives which need anim fix
				{ WEAPON_KNIFE_FALCHION,		"models/weapons/v_knife_falchion_advanced.mdl" },
				{ WEAPON_KNIFE_SURVIVAL_BOWIE,	"models/weapons/v_knife_survival_bowie.mdl" },
				{ WEAPON_KNIFE_BUTTERFLY,		"models/weapons/v_knife_butterfly.mdl" },
				{ WEAPON_KNIFE_PUSH,			"models/weapons/v_knife_push.mdl" },
				{ WEAPON_KNIFE_CORD,			"models/weapons/v_knife_cord.mdl" },
				{ WEAPON_KNIFE_CANIS,			"models/weapons/v_knife_canis.mdl" },
				{ WEAPON_KNIFE_URSUS,			"models/weapons/v_knife_ursus.mdl" },
				{ WEAPON_KNIFE_GYPSY_JACKKNIFE, "models/weapons/v_knife_gypsy_jackknife.mdl" },
				{ WEAPON_KNIFE_OUTDOOR,			"models/weapons/v_knife_outdoor.mdl" },
				{ WEAPON_KNIFE_STILETTO,		"models/weapons/v_knife_stiletto.mdl" },
				{ WEAPON_KNIFE_WIDOWMAKER,		"models/weapons/v_knife_widowmaker.mdl" },
				{ WEAPON_KNIFE_SKELETON,		"models/weapons/v_knife_skeleton.mdl" }
			};

			void ApplySkin(C_WeaponCSBaseGun* weapon)
			{
				auto it = Settings::SkinInfos.find(weapon->IsMelee() ? WEAPON_KNIFE_GG : weapon->GetItemDefinitionIndex());
				if (it == Settings::SkinInfos.end())
					return;

				weapon->SetFallbackPaintKit(it->second.Skin.GetInt());
				weapon->SetFallbackWear(it->second.Wear.GetFloat());
				weapon->SetFallbackSeed(it->second.Seed.GetInt());
				weapon->SetFallbackStatTrak(it->second.StatTrak.GetInt());

				weapon->SetItemIDHigh(-1);
				weapon->SetAccountID(weapon->GetOriginalOwnerXuid() & 0xFFFFFFFF);
			}

#if KNIFE_ANIM_FIX
			CProxyRecvHook hkSequence;
			int lastRecievedSequence = -1;

			void RecvProxy_Sequence(const CRecvProxyData* pData, void* pStruct, void* pOut)
			{
				lastRecievedSequence = pData->m_Value.m_Int;

				if (Settings::Misc::SkinChanger::Enable.GetInt())
				{
					int index = Settings::Misc::SkinChanger::KnifeModel.GetInt() - 1;

					if (6 < index && index < 19)
					{
						C_BaseViewModel* viewmodel = static_cast<C_BaseViewModel*>(pStruct);
						C_BaseEntity* owner = viewmodel->GetOwner();
						if (owner && owner->entindex() == I::EngineClient->GetLocalPlayer())
						{
							C_WeaponCSBaseGun* weapon = static_cast<C_WeaponCSBaseGun*>(viewmodel->GetWeapon());
							if (weapon && weapon->IsMelee())
								return;
						}
					}
				}

				hkSequence.original(pData, pStruct, pOut);
			}
#endif // KNIFE_ANIM_FIX

			void ApplyKnife(C_WeaponCSBaseGun* weapon, C_CSPlayer* player)
			{
				int index = Settings::Misc::SkinChanger::KnifeModel.GetInt() - 1;

				if (0 > index || index > (_countof(knifes) - 1))
					return;

				if (!weapon->IsMelee())
					return;

				weapon->SetEntityQuality(3);
				weapon->SetItemDefinitionIndex(knifes[index].index);

				int modelIndex = I::ModelInfo->GetModelIndex(knifes[index].model);

				weapon->SetModelIndex(modelIndex);
				weapon->SetModelIndexView(modelIndex);
				weapon->SetModelIndexWorld(modelIndex + 1);
				weapon->SetModelIndexWorldDropped(modelIndex + 2);

				C_BaseEntity* worldmodel = weapon->GetWorldModel();
				if (worldmodel)
					worldmodel->SetModelIndex(modelIndex + 1);

#if KNIFE_ANIM_FIX
				static float prevCycle = -1.f;
				static int prevSequence = -1;
#endif // KNIFE_ANIM_FIX

				C_BaseViewModel* viewmodel = player->GetViewmodel();
				if (viewmodel && viewmodel->GetWeapon() == weapon)
				{
					viewmodel->SetModelIndex(modelIndex);

#if KNIFE_ANIM_FIX
					static float prevAnimTime = -1.f;

					if (6 < index && prevSequence != lastRecievedSequence)
					{
						int newSeq = (lastRecievedSequence == 12) ? SelectWeightedSequence(viewmodel->GetModelPtr(), 213) : weapon->GetSequence();
						if (newSeq != -1)
						{
							int newAct = GetSequenceActivity(viewmodel->GetModelPtr(), newSeq);
							if (newAct == 185 && newAct == GetSequenceActivity(viewmodel->GetModelPtr(), viewmodel->GetSequence()))
							{
								if (!viewmodel->GetCycle() && 0.1f < prevCycle && prevCycle < 0.9f)
								{
									viewmodel->SetCycle(prevCycle + 0.05f);
									viewmodel->SetAnimTime(prevAnimTime);
								}
								else if (0.95f < viewmodel->GetCycle())
								{
									prevSequence = lastRecievedSequence;

									viewmodel->SendViewModelMatchingSequence(newSeq);
								}
							}
							else
							{
								prevSequence = lastRecievedSequence;

								viewmodel->SendViewModelMatchingSequence(newSeq);
							}
						}
					}

					prevCycle = viewmodel->GetCycle();
					prevAnimTime = viewmodel->GetAnimTime();
				}
				else
				{
					//prevCycle = -1.f;
					prevSequence = -1;
#endif // KNIFE_ANIM_FIX
				}
			}
		}

		bool fakeLagOn = false;

		void Fakelag(C_CSPlayer* player, CUserCmd* cmd, bool& bSendPacket)
		{
			if (Settings::Misc::FakeLag::ToggleKey.GetInt() && !fakeLagOn)
				return;

			if (!bSendPacket)
				return;

			if (!Settings::Misc::FakeLag::OnShot.GetInt() && player->IsShooting(cmd))
				return;

			bSendPacket = (cmd->tick_count % Settings::Misc::FakeLag::Amount.GetInt() == 0);
		}

		inline void SendClanTag(const char* tag, const char* name)
		{
			typedef void(__fastcall* Fn)(const char*, const char*);
			reinterpret_cast<Fn>(Signatures::SendClanTag)(tag, name);
		}

		inline bool ServerRankRevealAll()
		{
			static float unk[3] = { 0 };
			typedef bool(* Fn)(float*);
			return reinterpret_cast<Fn>(Signatures::__MsgFunc_ServerRankRevealAll)(unk);
		}

		void AutoTakeBot(C_CSPlayer* player, CUserCmd* cmd)
		{
			if (!(*G::pGameRules) || (*G::pGameRules)->IsWarmupPeriod())
				return;

			if (cmd->tick_count % 2 != 0)
				return;

			if (player->GetLifestate() != LIFE_DEAD)
				return;

			if (player->GetObserverMode() == OBS_MODE_ROAMING)
			{
				I::EngineClient->ClientCmd_Unrestricted("spec_mode 4");
				return;
			}

			C_BaseEntity* obsTarget = player->GetObserverTarget();
			if (!obsTarget)
			{
				bool foundBot = false;

				for (int i = 1; i <= G::Globals->maxClients; ++i)
				{
					C_CSPlayer* entity = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(i));
					if (!entity || entity->IsDormant() || !entity->IsAlive() || (entity->GetTeam() != player->GetTeam()))
						continue;

					if (entity->GetFlags() & FL_FAKECLIENT)
					{
						foundBot = true;
						break;
					}
				}

				if (!foundBot)
					return;
			}
			else if ((obsTarget->GetTeam() != player->GetTeam()) || !(obsTarget->GetFlags() & FL_FAKECLIENT))
				return;

			cmd->buttons |= IN_USE;
		}

		void AutoHoldBot()
		{
			if (!(*G::pGameRules) || (*G::pGameRules)->IsWarmupPeriod())
				return;

			C_BaseEntity* player = I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer());
			if (!player)
				return;

			for (int i = 1; i <= G::Globals->maxClients; ++i)
			{
				C_BaseEntity* entity = I::EntityList->GetClientEntity(i);
				if (!entity || entity->IsDormant() || (entity->GetTeam() != player->GetTeam()))
					continue;

				if (entity->GetFlags() & FL_FAKECLIENT)
				{
					I::EngineClient->ClientCmd_Unrestricted("holdpos");
					break;
				}
			}
		}

		namespace Log
		{
			struct CTeamDmg
			{
				int dmg;
				int kills;

				CTeamDmg() : 
					dmg(0),
					kills(0)
				{
				}
			};

			CTeamDmg teamDmg[64];

			void ResetTeamDmg()
			{
				for (size_t i = 0; i < _countof(Log::teamDmg); ++i)
				{
					Log::teamDmg[i].dmg = 0;
					Log::teamDmg[i].kills = 0;
				}
			}

			void Buy(int userId, int team, const char* weapon)
			{
				C_BaseEntity* player = I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer());
				if (!player || (player->GetTeam() == team))
					return;

				player_info_t info;
				if (!I::EngineClient->GetPlayerInfo(I::EngineClient->GetPlayerForUserID(userId), &info))
					return;

				::Log::PrefixMsg("%s bought %s\n", info.name, weapon);
			}

			void Hit(int victimIndex, int hitgroup, int dmg, int hp)
			{
				player_info_t info;
				if (!I::EngineClient->GetPlayerInfo(victimIndex, &info))
					return;

				const char* strHitgroup = HitgroupToString(hitgroup);

				::Log::PrefixMsg("Hit %s%s%s for %d dmg (%d hp remaining)\n",
					info.name,
					(strHitgroup[0] != '\0') ? "'s " : "",
					strHitgroup,
					dmg,
					hp);
			}

			void TeamDmg(int attackerIndex, int victimIndex, int dmg, int hp)
			{
				C_CSPlayer* attacker = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(attackerIndex));
				C_CSPlayer* victim = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(victimIndex));
				if (!attacker || !victim || (attacker == victim) || attacker->IsDormant() || victim->IsDormant() || (attacker->GetTeam() != victim->GetTeam()))
					return;

				teamDmg[attackerIndex - 1].dmg += dmg;

				if (hp <= 0)
					++teamDmg[attackerIndex - 1].kills;
			}
		}

		void HitSound()
		{
			const char* params[] = { "", Settings::Misc::HitSound::Sound.GetString(), Settings::Misc::HitSound::Volume.GetString() };
			CCommand command(3, params, kCommandSrcCode);
			static ConCommand* playVol = I::Cvar->FindCommand("playvol");
			playVol->Dispatch(command);
		}

		void ShowImpacts(int userId, const vec3& pos)
		{
			static ConVar* sv_showimpacts = I::Cvar->FindVar("sv_showimpacts");
			if (sv_showimpacts->GetInt())
				return;

			int shooterIndex = I::EngineClient->GetPlayerForUserID(userId);
			if (I::EngineClient->GetLocalPlayer() != shooterIndex)
				return;

			static vec3 mins(-2.f, -2.f, -2.f);
			static vec3 maxs(2.f, 2.f, 2.f);
			static vec3 zeroVec;
			Color clr(Settings::Visuals::ShowImpact::Color.GetString());
			static ConVar* sv_showimpacts_time = I::Cvar->FindVar("sv_showimpacts_time");
			I::DebugOverlay->AddBoxOverlay(pos, mins, maxs, zeroVec, clr.r, clr.g, clr.b, clr.a, sv_showimpacts_time->GetFloat());
		}

		void VoteRevealer(int voterIndex, int option)
		{
			if (I::EngineClient->GetLocalPlayer() == voterIndex)
				return;

			player_info_t info;
			if (!I::EngineClient->GetPlayerInfo(voterIndex, &info))
				return;

			::Log::PrefixMsg("%s voted %s\n", info.name, (option ? "NO" : "YES"));
		}

		void CmdSpam(const std::chrono::high_resolution_clock::time_point& curTime)
		{
			static auto prevTime = curTime;
			const auto passed = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - prevTime).count();

			if (Settings::Misc::CmdSpam::Interval.GetInt() <= passed)
			{
				prevTime = curTime;

				I::EngineClient->ClientCmd_Unrestricted(Settings::Misc::CmdSpam::Cmd.GetString());
			}
		}

		bool Voice_RecordStart(const char* pUncompressedFile, const char* pDecompressedFile, const char* pMicInputFile)
		{
			__asm
			{
				push pMicInputFile
				mov edx, pDecompressedFile
				mov ecx, pUncompressedFile
				call Signatures::Voice_RecordStart
				add esp, 0x4
			}
		}

		auto deathsoundEndTime = std::chrono::time_point<std::chrono::high_resolution_clock>::max();

		void DeathSoundTick(const std::chrono::high_resolution_clock::time_point& curTime)
		{
			if (deathsoundEndTime > curTime)
				return;

			static CCommand empty;
			static ConCommand* stopvoicerecord = I::Cvar->FindCommand("-voicerecord");
			stopvoicerecord->Dispatch(empty);

			if (Settings::Misc::DeathSound::Loopback.GetInt())
			{
				static ConVar* voice_loopback = I::Cvar->FindVar("voice_loopback");
				voice_loopback->SetValue(0);
			}

			deathsoundEndTime = std::chrono::time_point<std::chrono::high_resolution_clock>::max();
		}

		void DeathSound(int userId)
		{
			if (!(*G::pGameRules) || (*G::pGameRules)->IsWarmupPeriod())
				return;

			static char folder[MAX_PATH] = { 0 };
			if (!folder[0])
			{
				strcpy_s(folder, sizeof(folder), GetHeckFolder());
				strcat_s(folder, sizeof(folder), "deathsounds\\");

				std::filesystem::create_directory(folder);

				srand(time(nullptr));
			}

			int randomNum = rand() % 101;
			if (randomNum > Settings::Misc::DeathSound::Chance.GetInt())
				return;

			int deadIndex = I::EngineClient->GetPlayerForUserID(userId);
			if (deadIndex != I::EngineClient->GetLocalPlayer())
				return;

			C_CSPlayer* player = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(deadIndex));
			if (!player)
				return;

			int spectatorCount = 0;

			for (int i = 1; i <= G::Globals->maxClients; ++i)
			{
				C_CSPlayer* entity = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(i));
				if (!entity || (entity == player) || entity->IsDormant() || entity->IsAlive())
					continue;

				if (entity->GetObserverTarget() != player)
					continue;

				player_info_t info;
				if (!I::EngineClient->GetPlayerInfo(i, &info) || info.fakeplayer)
					continue;

				++spectatorCount;
			}

			if (spectatorCount < Settings::Misc::DeathSound::MinSpecs.GetInt())
				return;

			std::vector<std::filesystem::path::string_type> files;

			for (const auto& entry : std::filesystem::directory_iterator(folder))
			{
				if (!entry.is_regular_file())
					continue;

				files.push_back(entry.path().native());
			}

			if (!files.size())
				return;

			auto randomFile = files[rand() % files.size()];

			char cPath[MAX_PATH];
			size_t cPathLen;
			if (wcstombs_s(&cPathLen, cPath, sizeof(cPath), randomFile.c_str(), randomFile.size()))
				return;

			uint32_t soundLen = GetWaveLength(cPath);
			if (soundLen <= 0)
				return;

			if (!Voice_RecordStart(nullptr, nullptr, cPath))
				return;

			if (Settings::Misc::DeathSound::Loopback.GetInt())
			{
				static ConVar* voice_loopback = I::Cvar->FindVar("voice_loopback");
				voice_loopback->SetValue(1);
			}

			deathsoundEndTime = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(soundLen);
		}

		void AntiIdleKick(const std::chrono::high_resolution_clock::time_point& curTime, CUserCmd* cmd)
		{ 
			static auto lastMovedTime = curTime;

			if (cmd->buttons)
			{
				lastMovedTime = curTime;
				return;
			}

			const auto passed = std::chrono::duration_cast<std::chrono::seconds>(curTime - lastMovedTime).count();
			if (30 <= passed)
			{
				cmd->buttons = IN_FORWARD;
				lastMovedTime = curTime;
			}
		}
	}
}