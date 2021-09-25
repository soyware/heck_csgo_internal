#pragma once

namespace Hooks
{

	DrawModelExecuteFn oDrawModelExecute;

	void __fastcall DrawModelExecute(IVModelRender* _this, void* _edx, void* pRenderContext, const DrawModelState_t& pState, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
	{
		// glow over chams fix
		if ((pInfo.flags == (STUDIO_RENDER | STUDIO_SKIP_FLEXES | STUDIO_DONOTMODIFYSTENCILSTATE | STUDIO_NOLIGHTING_OR_CUBEMAP | STUDIO_SKIP_DECALS)) &&
			_this->IsForcedMaterialOverride())
			oDrawModelExecute(_this, pRenderContext, pState, pInfo, pCustomBoneToWorld);

		if (1 <= pInfo.entity_index && pInfo.entity_index <= G::Globals->maxClients)
		{
			C_CSPlayer* player = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer()));
			C_CSPlayer* entity = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(pInfo.entity_index));
			if (player && entity)
			{
				if (entity == player)
				{
					if (I::Input->CAM_IsThirdPerson() && player->IsScoped())
					{
						float oBlend = I::RenderView->GetBlend();

						I::RenderView->SetBlend(0.5f);
						oDrawModelExecute(_this, pRenderContext, pState, pInfo, pCustomBoneToWorld);
						I::RenderView->SetBlend(oBlend);

						return;
					}
				}
				else if (Settings::Visuals::Chams::Enable.GetInt() && entity->IsAlive())
				{
					bool sameTeam = (entity->GetTeam() == player->GetTeam());

					if (Settings::Visuals::Chams::Teammates.GetInt() || !sameTeam)
					{
						Color clrVis, clrNotVis;
						if (sameTeam)
						{
							clrVis.SetRawColor(Settings::Visuals::Chams::clrVisTeam.GetInt());
							clrNotVis.SetRawColor(Settings::Visuals::Chams::clrNotVisTeam.GetInt());
						}
						else
						{
							clrVis.SetRawColor(Settings::Visuals::Chams::clrVisEnemy.GetInt());
							clrNotVis.SetRawColor(Settings::Visuals::Chams::clrNotVisEnemy.GetInt());
						}

						Features::Visuals::Chams(
							clrVis,
							clrNotVis,
							Settings::Visuals::Chams::VisCheck.GetInt(),
							Settings::Visuals::Chams::Style.GetInt(),
							Settings::Visuals::Chams::Wireframe.GetInt(),
							oDrawModelExecute, _this, pRenderContext, pState, pInfo, pCustomBoneToWorld);

						return;
					}
				}
			}
		}
		else if (pInfo.entity_index != -1)
		{
			if (Settings::Visuals::Chams::Weapon::Enable.GetInt())
			{
				C_BaseEntity* entity = I::EntityList->GetClientEntity(pInfo.entity_index);
				if (entity && entity->GetNetworkable()->GetClientClass()->m_ClassID == ClassID::CPredictedViewModel)
				{
					C_WeaponCSBaseGun* weapon = static_cast<C_WeaponCSBaseGun*>(static_cast<C_BaseViewModel*>(entity)->GetWeapon());
					if (weapon)
					{
						short itemIndeex = weapon->GetItemDefinitionIndex();
						if ((itemIndeex != WEAPON_AUG && itemIndeex != WEAPON_SG556) || !weapon->GetZoomLevel())
						{
							Features::Visuals::Chams(
								Color(Settings::Visuals::Chams::Weapon::Color.GetInt()),
								Color::White(),
								true,
								Settings::Visuals::Chams::Weapon::Style.GetInt(),
								Settings::Visuals::Chams::Weapon::Wireframe.GetInt(),
								oDrawModelExecute, _this, pRenderContext, pState, pInfo, pCustomBoneToWorld);

							return;
						}
					}
				}
			}

			if (Settings::Visuals::Visuals::Enable.GetInt() && Settings::Visuals::Visuals::Chams.GetInt())
			{
				C_CSPlayer* player = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer()));
				C_CSPlayer* entity = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(pInfo.entity_index));
				if (player && entity)
				{
					Color clrVis;
					if (Features::Visuals::Visuals::GetEntityClrItemIndex(player, entity, clrVis, nullptr))
					{
						Color clrNotVis(clrVis.r / 2, clrVis.g / 2, clrVis.b / 2);

						Features::Visuals::Chams(
							clrVis,
							clrNotVis,
							Settings::Visuals::Visuals::ChamsVisCheck.GetInt(),
							CHAMS_STYLE_NORMAL,
							false,
							oDrawModelExecute, _this, pRenderContext, pState, pInfo, pCustomBoneToWorld);

						entity->SetAllowFastPath(false);

						return;
					}
				}
			}
		}
		else if (Settings::Visuals::Chams::Arms::Enable.GetInt() && strstr(pInfo.pModel->szPathName, "arms"))
		{
			Features::Visuals::Chams(
				Color(Settings::Visuals::Chams::Arms::clrVis.GetInt()),
				Color(Settings::Visuals::Chams::Arms::clrNotVis.GetInt()),
				Settings::Visuals::Chams::Arms::VisCheck.GetInt(),
				Settings::Visuals::Chams::Arms::Style.GetInt(),
				Settings::Visuals::Chams::Arms::Wireframe.GetInt(),
				oDrawModelExecute, _this, pRenderContext, pState, pInfo, pCustomBoneToWorld);

			return;
		}

		oDrawModelExecute(_this, pRenderContext, pState, pInfo, pCustomBoneToWorld);
	}

}