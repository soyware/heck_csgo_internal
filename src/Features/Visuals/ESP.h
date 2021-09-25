#pragma once

namespace Features
{
	namespace Visuals
	{
		namespace ESP
		{
			int Left, Right,
				Top, Bottom;

			void DrawHealth(int hp, Color clr)
			{
				hp = std::clamp(hp, 0, 100);

				Color clrHP = GetRatioClr(hp, 0.f, 100.f);
				clrHP.a = clr.a;

				wchar_t szHP[4];
				swprintf_s(szHP, _countof(szHP), L"%d", hp);

				// csgo sucks at this
				int textW;
				if (hp == 100)
					textW = 18;
				else if (10 <= hp)
					textW = 12;
				else
					textW = 7;

				if (!Settings::Visuals::ESP::HealthPos.GetInt())
				{
					DrawHorizontalProgressBar(Left, Right, Bottom + 2, hp, 0, 100, Color(0, 0, 0, clr.a));

					if (Settings::Visuals::ESP::HealthText.GetInt())
					{
						const int X = Left + (Right - Left) * (hp / 100.f);

						Draw::Text(szHP,
							X + 1 - textW,
							Bottom + (Settings::Visuals::ESP::Style.GetInt() ? 4 : 2),
							Draw::GetESPFont(),
							clr);
					}
				}
				else
				{
					DrawVerticalProgressBar(Left - 3, Top, Bottom, hp, 0, 100, Color(0, 0, 0, clr.a));

					if (Settings::Visuals::ESP::HealthText.GetInt())
					{
						const int Y = Bottom - (Bottom - Top) * (hp / 100.f);

						Draw::Text(szHP,
							Left - (Settings::Visuals::ESP::Style.GetInt() ? 6 : 4) - textW,
							Y - 3,
							Draw::GetESPFont(),
							clr);
					}
				}
			}

			void DrawStatus(C_CSPlayer* entity, const int entNum, Color clr)
			{
				int statusX = Right + 1 + Settings::Visuals::ESP::Style.GetInt();
				int statusY = Top - 3;
				const int statusYstep = 11 + Settings::Visuals::ESP::Style.GetInt();

				if (entity->GetTeam() == 2)
				{
					if ((*G::pPR) && (*G::pPR)->GetC4Player() == entNum)
					{
						C_WeaponCSBaseGun* weapon = entity->GetActiveWeapon();
						if (weapon && weapon->GetItemDefinitionIndex() == WEAPON_C4 && static_cast<C_C4*>(weapon)->IsBeingPlanted())
							Draw::Text(L"Plant", statusX, statusY, Draw::GetESPFont(), clr);
						else
							Draw::Text(L"C4", statusX, statusY, Draw::GetESPFont(), clr);

						statusY += statusYstep;
					}
				}
				else
				{
					if (entity->IsDefusing())
					{
						Draw::Text(L"Defuse", statusX, statusY, Draw::GetESPFont(), clr);
						statusY += statusYstep;
					}
					else if (entity->IsGrabbingHostage())
					{
						Draw::Text(L"Rescue", statusX, statusY, Draw::GetESPFont(), clr);
						statusY += statusYstep;
					}
					else if (entity->IsRescuing())
					{
						Draw::Text(L"Hostage", statusX, statusY, Draw::GetESPFont(), clr);
						statusY += statusYstep;
					}
				}

				if (entity->IsScoped())
				{
					Draw::Text(L"Scope", statusX, statusY, Draw::GetESPFont(), clr);
					statusY += statusYstep;
				}
				else
				{
					C_AnimationLayer* aimLayer = &entity->GetAnimOverlays()->Element(ANIMATION_LAYER_WEAPON_ACTION);

					if (aimLayer && 0.f < aimLayer->m_flWeight && aimLayer->m_flCycle < 0.95f &&
						GetSequenceActivity(entity->GetModelPtr(), aimLayer->m_nSequence) == ACT_CSGO_RELOAD)
					{
						Draw::Text(L"Reload", statusX, statusY, Draw::GetESPFont(), clr);
						statusY += statusYstep;
					}
				}

				if (entity->IsBlinded())
					Draw::Text(L"Flash", statusX, statusY, Draw::GetESPFont(), clr);
			}

			void DrawSkeleton(C_BaseAnimating* entity, matrix3x4_t** bones, Color color)
			{
				const CStudioHdr* hdr = entity->GetModelPtr();
				if (!hdr)
					return;

				int clavicleParent = 0;
				int arm_lower_L = 0, arm_lower_R = 0;

				for (int i = 1; i < hdr->numbones(); ++i)
				{
					const mstudiobone_t* bone = hdr->pBone(i);
					if (!bone || !(bone->flags & BONE_USED_BY_HITBOX))
						continue;

					int parent = ((i == arm_lower_L || i == arm_lower_R) ? clavicleParent : bone->parent);

					if (!arm_lower_L && !strcmp(bone->pszName(), "clavicle_L"))
					{
						arm_lower_L = i + 1;

						if (!clavicleParent)
							clavicleParent = bone->parent;

						continue;
					}
					else if (!arm_lower_R && !strcmp(bone->pszName(), "clavicle_R"))
					{
						arm_lower_R = i + 1;

						if (!clavicleParent)
							clavicleParent = bone->parent;

						continue;
					}

					vec3 wParent, wChild;
					MatrixPosition(*(bones[parent]), wParent);
					MatrixPosition(*(bones[i]), wChild);

					vec3 sParent, sChild;
					if (I::DebugOverlay->ScreenPosition(wParent, sParent) && I::DebugOverlay->ScreenPosition(wChild, sChild))
						Draw::Line(sParent, sChild, color);
				}
			}

			void DrawHead(matrix3x4_t** bones, Color clr)
			{
				vec3 world;
				MatrixPosition(*(bones[8]), world);

				vec3 screen;
				if (I::DebugOverlay->ScreenPosition(world, screen))
					DrawBox(screen.x - 2, screen.y - 2, screen.x + 2, screen.y + 2, clr, true);
			}

			void DrawBarrel(C_CSPlayer* entity, Color clr)
			{
				C_WeaponCSBaseGun* weapon = entity->GetActiveWeapon();
				if (!weapon)
					return;

				vec3 angles(entity->GetEyeAngles());

				vec3 fwd;
				AngleVectors(angles, &fwd);

				vec3 start;
				entity->Weapon_ShootPosition(start);

				vec3 end(start + (fwd * weapon->GetCSWpnData().m_flRange));

				Ray_t ray(start, end);
				CTraceFilterSimple filter(entity, 0);
				trace_t tr;
				I::EngineTrace->TraceRay(ray, (CS_MASK_SHOOT & ~CONTENTS_WINDOW), &filter, &tr);

				const bool zBuffer = Settings::Visuals::ESP::Barrel::VisCheck.GetInt();

				RenderLine(tr.startpos, tr.endpos, clr, zBuffer);

				static vec3 mins(-1.f, -1.f, -1.f);
				static vec3 maxs(1.f, 1.f, 1.f);

				RenderBox(tr.endpos, angles, mins, maxs, clr, zBuffer, false);
				RenderWireframeBox(tr.endpos, angles, mins, maxs, clr, zBuffer);
			}

			void Draw(C_CSPlayer* player, C_CSPlayer* entity, const int entNum)
			{
				if (!entity->IsAlive())
					return;

				bool sameTeam = (entity->GetTeam() == player->GetTeam());

				if (!Settings::Visuals::ESP::Teammates.GetInt() && sameTeam)
					return;

				if (Settings::Visuals::ESP::VisCheck.GetInt() &&
					(player->IsBlinded() || !CanSeeEachOther(player->GetEyePos(), entity->WorldSpaceCenter(), player, entity, true)))
					return;

				if (!BoundingBoxToScreen(entity, Left, Top, Right, Bottom))
					return;

				if (Settings::Visuals::ESP::Box.GetInt())
				{
					DrawBox(Left, Top, Right, Bottom,
						Color((sameTeam ? Settings::Visuals::ESP::BoxClrTeam : Settings::Visuals::ESP::BoxClrEnemy).GetInt()),
						false);
				}

				if (Settings::Visuals::ESP::Bones.GetInt() || Settings::Visuals::ESP::Head.GetInt())
				{
					matrix3x4_t* bones[MAXSTUDIOBONES];
					if (entity->HitboxToWorldTransforms(bones))
					{
						Color bonesClr(Settings::Visuals::ESP::BonesClr.GetInt());

						if (Settings::Visuals::ESP::Bones.GetInt())
							DrawSkeleton(entity, bones, bonesClr);

						if (Settings::Visuals::ESP::Head.GetInt())
							DrawHead(bones, bonesClr);
					}
				}

				Color textClr(Settings::Visuals::ESP::TextClr.GetInt());

				if (Settings::Visuals::ESP::Health.GetInt())
					DrawHealth(entity->GetHealth(), textClr);

				if (Settings::Visuals::ESP::Status.GetInt())
					DrawStatus(entity, entNum, textClr);

				const int center = Right - (Right - Left) / 2;
				// a pixel outside the box
				int nextOverheadY = Top - 1 - Settings::Visuals::ESP::Style.GetInt();

				if (Settings::Visuals::ESP::Name.GetInt())
					DrawName(center, nextOverheadY, entNum, textClr);

				C_WeaponCSBaseGun* weapon = entity->GetActiveWeapon();
				if (weapon && Settings::Visuals::ESP::Weapon.GetInt())
				{
					DrawWeapon(center,
						(Settings::Visuals::ESP::HealthPos.GetInt() ? Bottom : nextOverheadY),
						!Settings::Visuals::ESP::HealthPos.GetInt(),
						(Settings::Visuals::ESP::Weapon.GetInt() == 2),
						Settings::Visuals::ESP::WeaponAmmo.GetInt(),
						weapon,
						weapon->GetItemDefinitionIndex(),
						textClr);
				}
			}
		}
	}
}