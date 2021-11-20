#pragma once

namespace Features
{
	namespace Moves
	{
		// cant get on the ladder
		void CorrectMovement(CUserCmd* cmd, const vec3& localAngles)
		{
			if (!cmd->forwardmove && !cmd->sidemove)
				return;

			float delta = AngleNormalizePositive(cmd->viewangles.y - localAngles.y);

			float fmove = cosf(DEG2RAD(delta)) * cmd->forwardmove + cosf(DEG2RAD(delta + 90.f)) * cmd->sidemove;
			float smove = sinf(DEG2RAD(delta)) * cmd->forwardmove + sinf(DEG2RAD(delta + 90.f)) * cmd->sidemove;

			cmd->forwardmove = std::clamp(fmove, -450.f, 450.f);
			cmd->sidemove = std::clamp(smove, -450.f, 450.f);
		}

		void CorrectButtons(CUserCmd* cmd)
		{
			float fmove = cmd->forwardmove;
			float smove = cmd->sidemove;

			if (Settings::Moves::SlideWalk.GetInt())
			{
				fmove = -fmove;
				smove = -smove;
			}

			cmd->buttons &= ~(IN_BACK | IN_FORWARD | IN_MOVELEFT | IN_MOVERIGHT);

			if (fmove > 1.f)
				cmd->buttons |= IN_FORWARD;
			else if (fmove < -1.f)
				cmd->buttons |= IN_BACK;

			if (smove > 1.f)
				cmd->buttons |= IN_MOVERIGHT;
			else if (smove < -1.f)
				cmd->buttons |= IN_MOVELEFT;
		}

		namespace AntiAim
		{
			float lastRealYaw;

			bool realSideRight = false;

			enum AAPitchType
			{
				PITCHTYPE_NONE,
				PITCHTYPE_DOWN,
			};

			enum AAYawType
			{
				YAWTYPE_NONE,
				YAWTYPE_BACK,
			};

			float GetMaxDesync(CCSGOPlayerAnimState* animState)
			{
				// narrow the available aim matrix width as speed increases
				float flAimMatrixWidthRange = Lerp(
					std::clamp(animState->m_flSpeedAsPortionOfWalkTopSpeed, 0.f, 1.f),
					1.f, 
					Lerp(animState->m_flWalkToRunTransition, CSGO_ANIM_AIM_NARROW_WALK, CSGO_ANIM_AIM_NARROW_RUN));

				float m_flAnimDuckAmount = animState->m_flAnimDuckAmount;

				if (0.f < m_flAnimDuckAmount)
				{
					flAimMatrixWidthRange = Lerp(
						m_flAnimDuckAmount * std::clamp(animState->m_flSpeedAsPortionOfCrouchTopSpeed, 0.f, 1.f),
						flAimMatrixWidthRange,
						CSGO_ANIM_AIM_NARROW_CROUCHMOVING);
				}

				float flTempYawMax = animState->m_flAimYawMax * flAimMatrixWidthRange;

				return flTempYawMax;
			}
#define PRINT_PREVSENDPACK 1
#if PRINT_PREVSENDPACK
			bool prevSendPacket = false;
#endif // _DEBUG
			bool BreakLBY(C_CSPlayer*player, CUserCmd* cmd, bool& bSendPacket, CCSGOPlayerAnimState* animState, float desiredLBY)
			{
				if ((cmd->tick_count % 2 == 0) && player->GetLowerBodyYawTarget() != desiredLBY)
				{
					cmd->viewangles.y = AngleNormalize(desiredLBY);
					bSendPacket = false;

					if (!cmd->forwardmove && !cmd->sidemove)
						cmd->forwardmove = (cmd->buttons & IN_DUCK) ? 3.01f : 1.01f;

					return true;
				}

				/*
				static bool wasMoving = false;
				if (0.1f < animState->m_flVelocityLengthXY || 100.f < fabs(animState->m_flVelocityLengthZ))
				{
					wasMoving = true;
					return false;
				}

				static float nextRealign = -1.f;

				if (wasMoving)
				{
					// prevSendPacket: 0 = epic fail
					wasMoving = false;

					nextRealign = (G::Globals->curtime - TICK_INTERVAL) + (CSGO_ANIM_LOWER_REALIGN_DELAY * 0.2f) - TICK_INTERVAL;
				}
				else if (nextRealign < G::Globals->curtime)
				{
					cmd->viewangles.y = AngleNormalize(desiredLBY);
					bSendPacket = false;

					if (Settings::Moves::AntiAim::DesyncReadjust.GetInt() == 2 && !cmd->forwardmove && !cmd->sidemove)
						cmd->forwardmove = (cmd->buttons & IN_DUCK ? 3.01f : 1.01f);

					nextRealign = G::Globals->curtime + CSGO_ANIM_LOWER_REALIGN_DELAY;
#if PRINT_PREVSENDPACK
					Logging::PrefixMsg("prevSendPacket: %d\n", prevSendPacket);
#endif // _DEBUG
					return true;
				}
				*/
				return false;
			}

			void Desync(C_CSPlayer* player, CUserCmd* cmd, bool& bSendPacket)
			{
				CCSGOPlayerAnimState* animState = player->GetAnimState();
				if (!animState)
					return;

				float maxDesync = GetMaxDesync(animState);

				float desync = (realSideRight ? maxDesync : -maxDesync);

				bool breakingLBY = BreakLBY(player, cmd, bSendPacket, animState, (cmd->viewangles.y + desync));
				//bool brokenLBY = (cmd->viewangles.y != player->GetLowerBodyYawTarget());

				// TODO: below, make it work with fakelag
				if (!breakingLBY && /*brokenLBY &&*/ (cmd->tick_count % 2 == 0))
				{
					if (Settings::Moves::AntiAim::DesyncReadjust.GetInt())
						cmd->viewangles.y -= desync * 2.f;
					else
						cmd->viewangles.y -= 180.f - desync; // opposite from LBY, AngleDiff(m_flFootYawLast, m_flFootYaw) == 0

					cmd->viewangles.y = AngleNormalize(cmd->viewangles.y);

					bSendPacket = false;

					float m_flEyeYaw = cmd->viewangles.y;
					float m_flFootYaw = animState->m_flFootYaw;
					float flEyeFootDelta = AngleNormalize(m_flEyeYaw - m_flFootYaw);

					if (flEyeFootDelta > maxDesync)
						m_flFootYaw = m_flEyeYaw - maxDesync;

					else if (flEyeFootDelta < -maxDesync)
						m_flFootYaw = m_flEyeYaw + maxDesync;

					m_flFootYaw = AngleNormalize(m_flFootYaw);

					lastRealYaw = ApproachAngle(player->GetLowerBodyYawTarget(),
						m_flFootYaw,
						animState->m_flLastUpdateIncrement * CSGO_ANIM_LOWER_CATCHUP_IDLE);
				}
#if PRINT_PREVSENDPACK
				prevSendPacket = bSendPacket;
#endif // _DEBUG
			}

			void Desync2(C_CSPlayer* player, CUserCmd* cmd, bool& bSendPacket)
			{
				CCSGOPlayerAnimState* animState = player->GetAnimState();
				if (!animState)
					return;

				float maxDesync = GetMaxDesync(animState);

				float desync = (realSideRight ? maxDesync : -maxDesync);

				if (0.1f >= animState->m_flVelocityLengthXY)
				{
					static float prev = -1.f;
					if ((CSGO_ANIM_LOWER_REALIGN_DELAY * 0.2f) < fabs(G::Globals->curtime - prev))
					{
						if (!cmd->forwardmove && !cmd->sidemove)
						{
							cmd->forwardmove = (0.f < animState->m_flAnimDuckAmount) ? 3.01f : 1.01f;

							static bool side = false;
							if (side)
								cmd->forwardmove = -cmd->forwardmove;
							side = !side;
						}

						cmd->viewangles.y = AngleNormalize(cmd->viewangles.y + desync);

						bSendPacket = false;

						prev = G::Globals->curtime;
						return;
					}
				}

				lastRealYaw = AngleNormalize(cmd->viewangles.y - desync);

				if (cmd->tick_count % 2 == 0)
				{
					if (Settings::Moves::AntiAim::DesyncReadjust.GetInt())
						cmd->viewangles.y -= desync * 2.f;
					else
						cmd->viewangles.y -= 180.f - desync;

					cmd->viewangles.y = AngleNormalize(cmd->viewangles.y);

					bSendPacket = false;
				}
			}

			bool dropWeapon = false;

			void Run(C_CSPlayer* player, CUserCmd* cmd, bool& bSendPacket)
			{
				if (dropWeapon)
				{
					dropWeapon = false;
					I::EngineClient->ClientCmd_Unrestricted("drop", true);
					lastRealYaw = cmd->viewangles.y;
					return;
				}

				if (((*G::pGameRules) && (*G::pGameRules)->IsFreezePeriod()) ||
					(cmd->buttons & IN_USE) ||
					(player->GetMoveType() == MOVETYPE_LADDER) ||
					(player->GetMoveType() == MOVETYPE_NOCLIP) ||
					player->IsShooting(cmd))
				{
					lastRealYaw = cmd->viewangles.y;
					return;
				}

				switch (Settings::Moves::AntiAim::Pitch.GetInt())
				{
				case PITCHTYPE_DOWN: cmd->viewangles.x = 89.f; break;
				}

				switch (Settings::Moves::AntiAim::Yaw.GetInt())
				{
				case YAWTYPE_BACK: cmd->viewangles.y -= 180.f; break;
				}

				AnglesNormalize(cmd->viewangles);

				if (Settings::Moves::AntiAim::Desync.GetInt())
					Desync2(player, cmd, bSendPacket);

				AnglesNormalize(cmd->viewangles);

				vec3 localAngles;
				I::EngineClient->GetViewAngles(localAngles);

				CorrectMovement(cmd, localAngles);
			}
		}
	}
}