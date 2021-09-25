#pragma once

typedef unsigned short ModelInstanceHandle_t;
typedef void* StudioDecalHandle_t;

struct studiohwdata_t;
class IClientRenderable;

struct DrawModelState_t
{
	studiohdr_t*			m_pStudioHdr;
	studiohwdata_t*			m_pStudioHWData;
	IClientRenderable*		m_pRenderable;
	const matrix3x4_t*		m_pModelToWorld;
	StudioDecalHandle_t		m_decals;
	int						m_drawFlags;
	int						m_lod;
};

struct ModelRenderInfo_t
{
	vec3 origin;
	vec3 angles;
private:
	char				_pad[4];
public:
	void* pRenderable;
	const model_t* pModel;
	const matrix3x4_t* pModelToWorld;
	const matrix3x4_t* pLightingOffset;
	const vec3* pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	ModelInstanceHandle_t instance;
};

enum OverrideType_t
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
};

class IVModelRender
{
public:
	inline void ForcedMaterialOverride(IMaterial* newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL, int nMaterialIndex = -1)
	{
		typedef void(__thiscall* Fn) (IVModelRender*, IMaterial*, OverrideType_t, int);
		GetVF<Fn>(this, 1)(this, newMaterial, nOverrideType, nMaterialIndex);
	}

	inline bool IsForcedMaterialOverride()
	{
		typedef bool(__thiscall* Fn) (IVModelRender*);
		return GetVF<Fn>(this, 2)(this);
	}
};