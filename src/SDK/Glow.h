#pragma once

struct GlowObjectDefinition_t
{
	bool IsUnused() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

	// Linked list of free slots
	int m_nNextFreeSlot;

	// Special values for GlowObjectDefinition_t::m_nNextFreeSlot
	static const int END_OF_FREE_LIST = -1;
	static const int ENTRY_IN_USE = -2;

	C_BaseEntity* m_pEntity;
	float m_GlowColor[4];

	bool m_bGlowAlphaCappedByRenderAlpha;
	float m_flGlowAlphaFunctionOfMaxVelocity;
	float m_flGlowAlphaMax;
	float m_flGlowPulseOverdrive;
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloomRender;
	int m_nFullBloomStencilTestValue; // only render full bloom objects if stencil is equal to this value (value of -1 implies no stencil test)
	int m_nRenderStyle;
	int m_nSplitScreenSlot;
};

struct CGlowObjectManager
{
	CUtlVector<GlowObjectDefinition_t> m_GlowObjectDefinitions;
	int m_nFirstFreeSlot;
};