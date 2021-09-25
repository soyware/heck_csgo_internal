#pragma once

inline bool IsTyping()
{
	static ConVar* cl_mouseenable = I::Cvar->FindVar("cl_mouseenable");
	return I::EngineClient->Con_IsVisible() || !cl_mouseenable->GetInt();
}

inline bool LineGoesThroughSmoke(const vec3& start, const vec3& end)
{
	typedef bool(* Fn)(vec3, vec3);
	return reinterpret_cast<Fn>(Signatures::LineGoesThroughSmoke)(start, end);
}

inline float GetLerpTime()
{
	static ConVar* cl_interp_ratio = I::Cvar->FindVar("cl_interp_ratio");
	static ConVar* cl_interp = I::Cvar->FindVar("cl_interp");
	static ConVar* cl_updaterate = I::Cvar->FindVar("cl_updaterate");

	return std::max(cl_interp->GetFloat(), cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat());
}

void DrawClientHitboxes(C_BaseAnimating* entity, matrix3x4_t** bones, float duration)
{
	const CStudioHdr* hdr = entity->GetModelPtr();
	if (!hdr)
		return;

	mstudiohitboxset_t* set = hdr->pHitboxSet(entity->GetHitboxSet());
	if (!set)
		return;

	for (int i = 0; i < set->numhitboxes; ++i)
	{
		mstudiobbox_t* box = set->pHitbox(i);
		if (!box)
			continue;

		if (box->flCapsuleRadius > -1.f)
		{
			vec3 vMins, vMaxs;
			VectorTransform(&box->bbmin.x, *(bones[box->bone]), vMins);
			VectorTransform(&box->bbmax.x, *(bones[box->bone]), vMaxs);

			I::DebugOverlay->AddCapsuleOverlay(vMins, vMaxs, box->flCapsuleRadius, 255, 0, 0, 255, duration);
		}
	}
}

#include "Misc/Misc.h"
#include "Misc/Events.h"
#include "Moves/Moves.h"
#include "Visuals/Visuals.h"