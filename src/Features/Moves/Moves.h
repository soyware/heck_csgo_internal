#pragma once

#include "Backtrack.h"
#include "AntiAim.h"

namespace Features
{
	namespace Moves
	{
		CUserCmd* lastCmd = nullptr;
		vec3		lastSentViewangles;

		namespace Aim
		{
			// Credits: FuckFace32
			void GetSpread(C_WeaponCSBaseGun* weapon, int buttons, vec3& outSpread)
			{
				float fRand1 = RandomFloat(0.f, 1.f);
				float fRandPi1 = RandomFloat(0.f, 2.f * M_PI_F);
				float fRand2 = RandomFloat(0.f, 1.f);
				float fRandPi2 = RandomFloat(0.f, 2.f * M_PI_F);

				short itemIndex = weapon->GetItemDefinitionIndex();
				float recoilIndex = weapon->GetRecoilIndex();

				if (itemIndex == WEAPON_REVOLVER)
				{
					if (buttons & IN_ATTACK2)
					{
						fRand1 = 1.f - fRand1 * fRand1;
						fRand2 = 1.f - fRand2 * fRand2;
					}
				}
				else if (itemIndex == WEAPON_NEGEV && recoilIndex < 3.f)
				{
					for (int i = 3; i > recoilIndex; --i)
					{
						fRand1 *= fRand1;
						fRand2 *= fRand2;
					}

					fRand1 = 1.f - fRand1;
					fRand2 = 1.f - fRand2;
				}

				float fRandInaccuracy = fRand1 * weapon->GetInaccuracy();
				float fRandSpread = fRand2 * weapon->GetSpread();

				outSpread.x = cos(fRandPi1) * fRandInaccuracy + cos(fRandPi2) * fRandSpread;
				outSpread.y = sin(fRandPi1) * fRandInaccuracy + sin(fRandPi2) * fRandSpread;
				outSpread.z = 0.f;
			}

			bool MeetsHitchance(int percent, C_CSPlayer* player, C_WeaponCSBaseGun* weapon, const vec3& viewangles, int buttons, C_BaseEntity* target)
			{
				if (percent < 1)
					return true;

				constexpr int rayCount = 255;

				int hitsLeft = (percent * (rayCount / 100.f));

				vec3 fwd;
				AngleVectors(viewangles + (player->GetAimPunch() * 2.f), &fwd);

				vec3 start;
				player->Weapon_ShootPosition(start);

				float range = weapon->GetCSWpnData().m_flRange;

				weapon->UpdateAccuracyPenalty();

				for (int i = rayCount; i > 0; --i)
				{
					RandomSeed(i - 1);

					vec3 spread;
					GetSpread(weapon, buttons, spread);

					vec3 normalized(fwd + spread);
					VectorNormalize(normalized);

					vec3 end(start + (normalized * range));

					Ray_t ray(start, end);
					trace_t tr;
					I::EngineTrace->ClipRayToEntity(ray, CS_MASK_SHOOT & ~CONTENTS_WINDOW, target, &tr);

					if (tr.pEnt == target)
					{
						if (--hitsLeft <= 0)
							return true;
					}
					else if (hitsLeft > i)
						return false;
				}

				return false;
			}

			bool triggerbotOn = false;

			void Triggerbot(C_CSPlayer* player, CUserCmd* cmd)
			{
				static bool prevInSight = false;

				if (!triggerbotOn)
				{
					prevInSight = false;
					return;
				}

				if (Settings::Moves::Aim::Triggerbot::VisCheck.GetInt() && player->IsBlinded())
				{
					prevInSight = false;
					return;
				}

				C_WeaponCSBaseGun* weapon = player->GetActiveWeapon();
				if (!weapon)
					return;

				short itemIndex = weapon->GetItemDefinitionIndex();

				int attackBtn = (itemIndex == WEAPON_REVOLVER) ? IN_ATTACK2 : IN_ATTACK;

				bool checkShoot = (!player->IsShooting(cmd) && weapon->CanAttack(attackBtn == IN_ATTACK));

				int filter = Settings::Moves::Aim::Triggerbot::Hitbox.GetInt();

				vec3 fwd;
				AngleVectors(cmd->viewangles + (player->GetAimPunch() * 2.f), &fwd);

				vec3 start;
				player->Weapon_ShootPosition(start);
				vec3 end(start + (fwd * weapon->GetCSWpnData().m_flRange));

				bool inSight = false;
				bool shoot = false;

				for (int i = 1; i <= G::Globals->maxClients; ++i)
				{
					C_CSPlayer* entity = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(i));
					if (!entity || (entity == player) || entity->IsDormant() || !entity->IsAlive() || entity->IsGunGameImmune())
						continue;

					if (!Settings::Moves::Aim::Teammates.GetInt() && (entity->GetTeam() == player->GetTeam()))
						continue;

					trace_t tr;
					Ray_t ray(start, end);
					I::EngineTrace->ClipRayToEntity(ray, CS_MASK_SHOOT & ~CONTENTS_WINDOW, entity, &tr);

					if (!tr.pEnt)
						continue;

					if (!CanSeeEachOther(tr.startpos, tr.endpos, player, tr.pEnt, Settings::Moves::Aim::Triggerbot::VisCheck.GetInt()))
						continue;

					inSight = true;

					if (!checkShoot)
						break;

					if (filter == 1)
					{
						if (tr.hitgroup != HITGROUP_HEAD)
							continue;
					}
					else if (filter == 2)
					{
						if (tr.hitgroup != HITGROUP_CHEST && tr.hitgroup != HITGROUP_STOMACH)
							continue;
					}

					if (itemIndex == WEAPON_TASER)
					{
						if (150.f < (tr.startpos - tr.endpos).Length())
							continue;
					}

					if (!MeetsHitchance(Settings::Moves::Aim::Triggerbot::Hitchance.GetInt(), player, weapon, cmd->viewangles, attackBtn, tr.pEnt))
						continue;

					shoot = true;
					break;
				}

				if (!inSight)
				{
					prevInSight = false;
					return;
				}

				static std::chrono::time_point<std::chrono::high_resolution_clock> timeInSight;
				if (!prevInSight)
				{
					prevInSight = true;
					timeInSight = std::chrono::high_resolution_clock::now();
				}

				if (!shoot)
					return;

				const auto timeCur = std::chrono::high_resolution_clock::now();
				const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(timeCur - timeInSight).count();

				if (elapsed < Settings::Moves::Aim::Triggerbot::Delay.GetInt())
					return;

				cmd->buttons |= attackBtn;
				cmd->buttons &= ~IN_USE;
			}

			void AutoPistol(C_CSPlayer* player, CUserCmd* cmd)
			{
				C_WeaponCSBaseGun* weapon = player->GetActiveWeapon();
				if (!weapon)
					return;

				bool isRevolver = (weapon->GetItemDefinitionIndex() == WEAPON_REVOLVER);

				int attackBtn = (isRevolver ? IN_ATTACK2 : IN_ATTACK);

				if (!(cmd->buttons & attackBtn))
					return;

				const CCSWeaponInfo& info = weapon->GetCSWpnData();

				// revolver is bFullAuto
				if (info.m_WeaponType != WEAPONTYPE_PISTOL || (!isRevolver && info.m_bFullAuto))
					return;

				if (0 < player->GetShotsFired())
					cmd->buttons &= ~attackBtn;
			}
		}

		void AutoJump(CUserCmd* cmd)
		{
			if ((cmd->buttons & IN_JUMP))
				cmd->buttons &= ~IN_JUMP;
		}

		// TODO: proper autostrafe
		void AutoStrafe(CUserCmd* cmd)
		{
			if (cmd->mousedx > 1)
				cmd->sidemove = 450.f;
			else if (cmd->mousedx < -1)
				cmd->sidemove = -450.f;
		}

		void AutoMount(C_CSPlayer* player, CUserCmd* cmd)
		{
			if (cmd->buttons & (IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT))
				return;

			C_BaseEntity* ground = player->GetGroundEntity();
			if (!ground || !ground->IsPlayer())
				return;

			vec3 posDelta(ground->GetAbsOrigin() + ground->GetVelocity() * TICK_INTERVAL - player->GetAbsOrigin());
			if (posDelta.Length2D() < 3.f)
				return;

			cmd->forwardmove = 450.f;
			cmd->sidemove = 0.f;

			vec3 angle;
			VectorAngles(posDelta, angle);

			CorrectMovement(cmd, angle);
		}

		bool autoBlockLock = false;
		int autoBlockTarget = -1;

		void AutoBlock(C_CSPlayer* player, CUserCmd* cmd)
		{
			static vec3 lockAngle;

			if (autoBlockLock)
			{
				if (autoBlockTarget != -1)
				{
					autoBlockTarget = -1;
					autoBlockLock = false;
					return;
				}

				float closestDist = 300.f;

				for (int i = 1; i <= G::Globals->maxClients; ++i)
				{
					C_CSPlayer* entity = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(i));
					if (!entity || (entity == player) || entity->IsDormant() || !entity->IsAlive())
						continue;

					if (entity->GetTeam() != player->GetTeam())
						continue;

					float dist = (entity->GetAbsOrigin() - player->GetAbsOrigin()).Length();

					if (dist < closestDist)
					{
						closestDist = dist;
						autoBlockTarget = i;
					}
				}

				if (autoBlockTarget == -1)
					return;

				I::EngineClient->GetViewAngles(lockAngle);
				autoBlockLock = false;
			}

			C_CSPlayer* target = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(autoBlockTarget));
			if (!target || !target->IsAlive() || (cmd->buttons & (IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT)))
			{
				autoBlockTarget = -1;
				return;
			}

			vec3 extraTargetPos(target->GetAbsOrigin() + target->GetVelocity() * TICK_INTERVAL);

			vec3 posDelta(extraTargetPos - player->GetAbsOrigin());

			vec3 angle;
			VectorAngles(posDelta, angle);

			float delta = AngleNormalize(lockAngle.y - angle.y);

			if (3.f < delta && delta < 177.f)
				cmd->sidemove = 450.f;
			else if (-3.f > delta && delta > -177.f)
				cmd->sidemove = -450.f;
			else
				return;

			CorrectMovement(cmd, lockAngle);
		}

		/*
		 * Set curtime to predicted curtime (time server will have when packet arrives).
		 * m_nTickBase gets incremented in CPrediction::RunCommand, which runs on frames
		 * so we need to account for it manually in case fps is lower than tickrate
		 */
		float GetPredictedCurtime(C_CSPlayer* player, CUserCmd* cmd)
		{
			static int additionalTicks = 0;

			static CUserCmd* prevCmd = nullptr;
			if (!prevCmd || prevCmd->hasbeenpredicted)
				additionalTicks = 0;
			else
				++additionalTicks;

			prevCmd = cmd;

			return TICKS_TO_TIME(player->GetTickBase() + additionalTicks);
		}

		bool Run(C_CSPlayer* player, CUserCmd* cmd, bool& bSendPacket)
		{
			const auto curTime = std::chrono::high_resolution_clock::now();
			static auto prevTime = curTime;

			const auto passed = std::chrono::duration_cast<std::chrono::seconds>(curTime - prevTime).count();
			if (1 <= passed)
			{
				prevTime = curTime;

				if (Settings::Misc::ShowRanks.GetInt() && (cmd->buttons & IN_SCORE) &&
					(*G::pGameRules) && (*G::pGameRules)->IsQueuedMatchmaking())
					Misc::ServerRankRevealAll();

				static bool prevNonEmpty = false;

				const char* clanTag = Settings::Misc::ClanTag.GetString();
				if (clanTag[0] || prevNonEmpty)
					Misc::SendClanTag(clanTag, clanTag);

				prevNonEmpty = clanTag[0];
			}

			player->SetMaxFlashAlpha(255.f * (Settings::Visuals::MaxFlash.GetFloat() * 0.01f));

			if (Settings::Misc::CmdSpam::Enable.GetInt())
				Misc::CmdSpam(curTime);

			Misc::DeathSoundTick(curTime);

			if (Settings::Misc::ServerLagger::Enable.GetInt())
				Misc::ServerLagger();

			if (!player->IsAlive())
			{
				autoBlockTarget = -1;

				return true;
			}

			const float oCurTime = G::Globals->curtime;
			G::Globals->curtime = GetPredictedCurtime(player, cmd);

			if (Settings::Moves::Aim::AutoPistol.GetInt())
				Aim::AutoPistol(player, cmd);

			if (Settings::Moves::Aim::Backtrack::Enable.GetInt())
				Aim::Backtrack::Run(player, cmd);

			if (Settings::Moves::Aim::Triggerbot::Enable.GetInt())
				Aim::Triggerbot(player, cmd);

			if (!(player->GetFlags() & FL_ONGROUND))
			{
				if (Settings::Moves::AutoJump.GetInt())
					AutoJump(cmd);

				if (Settings::Moves::AutoStrafe.GetInt())
					AutoStrafe(cmd);
			}

			if (Settings::Misc::FakeLag::Enable.GetInt())
				Misc::Fakelag(player, cmd, bSendPacket);

			if (Settings::Moves::AntiAim::Enable.GetInt())
				AntiAim::Run(player, cmd, bSendPacket);

			if (Settings::Moves::AutoMount.GetInt())
				AutoMount(player, cmd);

			AutoBlock(player, cmd);

			if (Settings::Moves::AntiAim::Enable.GetInt() ||
				Settings::Moves::AutoMount.GetInt() ||
				Settings::Moves::SlideWalk.GetInt())
				CorrectButtons(cmd);

			Misc::AntiIdleKick(curTime, cmd);

			if (Settings::Misc::NoDuckCooldown.GetInt())
				cmd->buttons |= IN_BULLRUSH;

			G::Globals->curtime = oCurTime;

			return false;
		}
	}
}