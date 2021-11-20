#pragma once

enum
{
	RENDER_FLAGS_DISABLE_RENDERING = 0x01,
	RENDER_FLAGS_HASCHANGED = 0x02,
	RENDER_FLAGS_ALTERNATE_SORTING = 0x04,
	RENDER_FLAGS_RENDER_WITH_VIEWMODELS = 0x08,
	RENDER_FLAGS_BLOAT_BOUNDS = 0x10,
	RENDER_FLAGS_BOUNDS_VALID = 0x20,
	RENDER_FLAGS_BOUNDS_ALWAYS_RECOMPUTE = 0x40,
	RENDER_FLAGS_IS_SPRITE = 0x80,
	RENDER_FLAGS_FORCE_OPAQUE_PASS = 0x100,
};

class CClientAlphaProperty;

struct RenderableInfo_t
{
	IClientRenderable*	m_pRenderable;
	CClientAlphaProperty* m_pAlphaProperty;
	int					m_EnumCount;				// Have I been added to a particular shadow yet?
	int					m_nRenderFrame;
	unsigned short		m_FirstShadow;				// The first shadow caster that cast on it
	unsigned short		m_LeafList;					// What leafs is it in?
	short				m_Area;						// -1 if the renderable spans multiple areas.
	uint16_t			m_Flags;					// rendering flags
	uint16_t			m_bRenderInFastReflection : 1;	// Should we render in the "fast" reflection?
	uint16_t			m_bDisableShadowDepthRendering : 1;	// Should we not render into the shadow depth map?
	uint16_t			m_bDisableCSMRendering : 1;			// Should we not render into the CSM?
	uint16_t			m_bDisableShadowDepthCaching : 1;	// Should we not be cached in the shadow depth map?
	uint16_t			m_nSplitscreenEnabled : 2;	// splitscreen rendering flags
	uint16_t			m_nTranslucencyType : 2;	// RenderableTranslucencyType_t
	uint16_t			m_nModelType : 8;			// RenderableModelType_t
	Vector				m_vecBloatedAbsMins;		// Use this for tree insertion
	Vector				m_vecBloatedAbsMaxs;
	Vector				m_vecAbsMins;			// NOTE: These members are not threadsafe!!
	Vector				m_vecAbsMaxs;			// They can be updated from any viewpoint (based on RENDER_FLAGS_BOUNDS_VALID)
};

namespace Hooks
{
	typedef int(__thiscall* ListLeavesInBoxFn)(ISpatialQuery*, const Vector&, const Vector&, unsigned short*, int);
	ListLeavesInBoxFn oListLeavesInBox;

	// Credits: Soufiw ( https://www.unknowncheats.me/forum/members/1932178.html )

	int __fastcall ListLeavesInBox(ISpatialQuery* _this, void* edx, const Vector& mins, const Vector& maxs, unsigned short* pList, int listMax)
	{
		// occulusion getting updated on player movement/angle change,
		// in RecomputeRenderableLeaves ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L674 );
		// check for return in CClientLeafSystem::InsertIntoTree
		if (!Settings::Visuals::Chams::Enable.GetInt() ||
			Settings::Visuals::Chams::VisCheck.GetInt() || 
			(reinterpret_cast<uintptr_t>(_ReturnAddress()) != Signatures::CClientLeafSystem__InsertIntoTree_ListLeavesInBox))
			return oListLeavesInBox(_this, mins, maxs, pList, listMax);

		// get current renderable info from stack ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L1470 )
		// check if disabling occulusion for players ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L1491 )
		RenderableInfo_t* info = *reinterpret_cast<RenderableInfo_t**>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x14);
		if (!info || !info->m_pRenderable)
			return oListLeavesInBox(_this, mins, maxs, pList, listMax);

		C_BaseEntity* entity = info->m_pRenderable->GetIClientUnknown()->GetBaseEntity();
		if (!entity || !entity->IsPlayer())
			return oListLeavesInBox(_this, mins, maxs, pList, listMax);

		// fix render order, force translucent group ( https://www.unknowncheats.me/forum/2429206-post15.html )
		// AddRenderablesToRenderLists: https://i.imgur.com/hcg0NB5.png ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L2473 )
		//info->m_Flags &= ~RENDER_FLAGS_FORCE_OPAQUE_PASS;
		//info->m_nTranslucencyType = RENDERABLE_IS_TWO_PASS;

		// extend world space bounds to maximum ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L707 )
		static const Vector map_min(MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT);
		static const Vector map_max(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);
		return oListLeavesInBox(_this, map_min, map_max, pList, listMax);
	}
}