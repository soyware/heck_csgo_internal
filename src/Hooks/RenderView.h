#pragma once

namespace Hooks
{
	typedef void(__thiscall* Draw3DDebugOverlaysFn)(IVRenderView*);
	Draw3DDebugOverlaysFn oDraw3DDebugOverlays;

	void __fastcall Draw3DDebugOverlays(IVRenderView* _this)
	{
		oDraw3DDebugOverlays(_this);

		if (!Settings::Visuals::ESP::Enable.GetInt() || !Settings::Visuals::ESP::Barrel::Enable.GetInt())
			return;

		C_CSPlayer* player = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(I::EngineClient->GetLocalPlayer()));
		if (!player)
			return;

		if (Settings::Visuals::ESP::VisCheck.GetInt() && player->IsBlinded())
			return;

		int specMode = player->GetObserverMode();
		C_BaseEntity* specTarget = (specMode == OBS_MODE_IN_EYE ? player->GetObserverTarget() : nullptr);

		for (int i = 1; i <= G::Globals->maxClients; ++i)
		{
			C_CSPlayer* entity = static_cast<C_CSPlayer*>(I::EntityList->GetClientEntity(i));
			if (!entity || (entity == player) || (entity == specTarget) || entity->IsDormant() || !entity->IsAlive())
				continue;

			if (!Settings::Visuals::ESP::Teammates.GetInt() && (entity->GetTeam() == player->GetTeam()))
				continue;

			if (Settings::Visuals::ESP::VisCheck.GetInt() && 
				!CanSeeEachOther(player->GetEyePos(), entity->WorldSpaceCenter(), player, entity, true))
				continue;

			Features::Visuals::ESP::DrawBarrel(entity, Color(((entity->GetTeam() == player->GetTeam()) ? 
					Settings::Visuals::ESP::Barrel::ClrTeam : Settings::Visuals::ESP::Barrel::ClrEnemy).GetInt()));
		}
	}
}