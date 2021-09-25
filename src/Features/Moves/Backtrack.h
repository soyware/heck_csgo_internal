#pragma once

namespace Features
{
	namespace Moves
	{
		namespace Aim
		{
			namespace Backtrack
			{
				struct CRecord
				{
					float		time = 0.f;
					vec3		mins;
					vec3		maxs;
					vec3		origin;
					vec3		angles;
					matrix3x4_t matrix[MAXSTUDIOBONES];
				};

				// space for 128 tickrate 0.2 sv_maxunlag plus fake latency
				constexpr int ticksStored = (0.2f / (1.f / 128.f)) * 2.f;

				CRecord records[64][ticksStored];

				bool IsWithinUnlag(float simTime)
				{
					if (simTime <= 0.f)
						return false;

					float correct = 0.f;

					INetChannel* nc = I::EngineClient->GetNetChannelInfo();
					if (nc)
					{
						correct += nc->GetLatency(FLOW_OUTGOING);
						correct += nc->GetLatency(FLOW_INCOMING);
					}
					correct += GetLerpTime();

					static ConVar* sv_maxunlag = I::Cvar->FindVar("sv_maxunlag");
					correct = std::clamp(correct, 0.f, sv_maxunlag->GetFloat());

					float deltaTime = correct - (G::Globals->curtime - simTime);

					return fabsf(deltaTime) <= (Settings::Moves::Aim::Backtrack::Max.GetFloat() * 0.001f);
				}

				void Record(C_CSPlayer* player, int tickCount)
				{
					//float oCurtime = G::pGlobals->curtime;
					//G::pGlobals->curtime = TICKS_TO_TIME(player->GetTickBase())/* + GetLerpTime()*/;

					for (int i = 0; i < G::Globals->maxClients; ++i)
					{
						// skip world entity
						C_CSPlayer* entity = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(i + 1));
						if (!entity || (entity == player) || entity->IsDormant() || !entity->IsAlive() || entity->IsGunGameImmune())
							continue;

						if (!Settings::Moves::Aim::Teammates.GetInt() && (entity->GetTeam() == player->GetTeam()))
							continue;

						//entity->UpdateClientSideAnimation();

						records[i][tickCount % ticksStored].time = entity->GetSimulationTime();
						records[i][tickCount % ticksStored].mins = entity->WorldAlignMins();
						records[i][tickCount % ticksStored].maxs = entity->WorldAlignMaxs();
						records[i][tickCount % ticksStored].origin = entity->GetNetworkOrigin();
						records[i][tickCount % ticksStored].angles = entity->GetNetworkAngles();

						if (Settings::Moves::Aim::Backtrack::Draw.GetInt())
							entity->GetRenderable()->SetupBones(records[i][tickCount % ticksStored].matrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, G::Globals->curtime);
					}

					//G::pGlobals->curtime = oCurtime;
				}

				float FindSimTime(C_CSPlayer* player, const vec3& viewangles)
				{
					float simTime = 0.f;

					vec3 fwd;
					AngleVectors(viewangles + (player->GetAimPunch() * 2.f), &fwd);

					vec3 start;
					player->Weapon_ShootPosition(start);
					vec3 end(start + (fwd * player->GetActiveWeapon()->GetCSWpnData().m_flRange));

					trace_t tr;
					Ray_t ray(start, end);

					float oCurtime = G::Globals->curtime;

					for (int i = 0; i < G::Globals->maxClients; ++i)
					{
						C_CSPlayer* entity = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(i + 1));
						if (!entity || (entity == player) || entity->IsDormant() || !entity->IsAlive() || entity->IsGunGameImmune())
							continue;

						if (!Settings::Moves::Aim::Teammates.GetInt() && (entity->GetTeam() == player->GetTeam()))
							continue;

						vec3 oMins = entity->WorldAlignMins();
						vec3 oMaxs = entity->WorldAlignMaxs();
						vec3 oOrigin = entity->GetAbsOrigin();
						vec3 oAngles = entity->GetAbsAngles();

						for (int j = 0; j < ticksStored; ++j)
						{
							if (!IsWithinUnlag(records[i][j].time))
								continue;

							G::Globals->curtime = records[i][j].time;
							entity->InvalidateBoneCache();

							entity->SetSize(records[i][j].mins, records[i][j].maxs);
							entity->SetAbsOrigin(records[i][j].origin);
							entity->SetAbsAngles(records[i][j].angles);

							I::EngineTrace->ClipRayToEntity(ray, CS_MASK_SHOOT & ~CONTENTS_WINDOW, entity, &tr);

							bool didHit = tr.DidHit();
#ifdef _DEBUG
							if (didHit && Settings::Moves::Aim::Backtrack::Draw.GetInt())
							{
								matrix3x4_t* matrix[MAXSTUDIOBONES];
								entity->HitboxToWorldTransforms(matrix);

								static ConVar* sv_showlagcompensation_duration = I::Cvar->FindVar("sv_showlagcompensation_duration");
								DrawClientHitboxes(entity, matrix, sv_showlagcompensation_duration->GetFloat());
							}
#endif // _DEBUG
							G::Globals->curtime = oCurtime;
							entity->InvalidateBoneCache();

							if (didHit)
							{
								simTime = records[i][j].time;
								break;
							}
						}

						entity->SetSize(oMins, oMaxs);
						entity->SetAbsOrigin(oOrigin);
						entity->SetAbsAngles(oAngles);

						if (simTime)
							break;
					}

					return simTime;
				}

				void Run(C_CSPlayer* player, CUserCmd* cmd)
				{
					if (!player->IsShooting(cmd))
						return;

					float foundSimTime = FindSimTime(player, cmd->viewangles);
					if (foundSimTime)
						cmd->tick_count = TIME_TO_TICKS(foundSimTime + GetLerpTime());
				}
			}
		}
	}
}