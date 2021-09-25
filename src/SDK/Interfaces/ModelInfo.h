#pragma once

#define STUDIO_NONE						0x00000000
#define STUDIO_RENDER					0x00000001
#define STUDIO_VIEWXFORMATTACHMENTS		0x00000002
#define STUDIO_DRAWTRANSLUCENTSUBMODELS 0x00000004
#define STUDIO_TWOPASS					0x00000008
#define STUDIO_STATIC_LIGHTING			0x00000010
#define STUDIO_WIREFRAME				0x00000020
#define STUDIO_ITEM_BLINK				0x00000040
#define STUDIO_NOSHADOWS				0x00000080
#define STUDIO_WIREFRAME_VCOLLIDE		0x00000100
#define STUDIO_NOLIGHTING_OR_CUBEMAP	0x00000200
#define STUDIO_SKIP_FLEXES				0x00000400
#define STUDIO_DONOTMODIFYSTENCILSTATE	0x00000800	// TERROR

// not a studio flag, but used to signify that the draw call should cause a depth imposter to be drawn as well.
#define STUDIO_DEPTH_ONLY				0x01000000

// Not a studio flag, but used to flag model to render using special settings for AO pre-pass
#define STUDIO_AOPREPASSTEXURE			0x02000000

// Not a studio flag, but used to flag model to keep the shadow state it starts with
#define STUDIO_KEEP_SHADOWS				0x04000000

// Not a studio flag, but used to flag model as using shadow depth material override
#define STUDIO_SSAODEPTHTEXTURE			0x08000000

// Not a studio flag, but used to flag model as a non-sorting brush model
#define STUDIO_TRANSPARENCY				0x80000000

// Not a studio flag, but used to flag model as using shadow depth material override
#define STUDIO_SHADOWDEPTHTEXTURE		0x40000000

// Not a studio flag, but used to flag model as doing custom rendering into shadow texture
#define STUDIO_SHADOWTEXTURE			0x20000000

#define STUDIO_SKIP_DECALS				0x10000000

enum modtype_t
{
	mod_bad = 0,
	mod_brush,
	mod_sprite,
	mod_studio
};

typedef unsigned short MDLHandle_t;

#define MDLHANDLE_INVALID (MDLHandle_t)~0 

//-----------------------------------------------------------------------------
// Indicates the type of translucency of an unmodulated renderable
//-----------------------------------------------------------------------------
enum RenderableTranslucencyType_t
{
	RENDERABLE_IS_OPAQUE = 0,
	RENDERABLE_IS_TRANSLUCENT,
	RENDERABLE_IS_TWO_PASS,	// has both translucent and opaque sub-partsa
};

class IVModelInfo
{
public:
	inline int GetModelIndex(const char* name)
	{
		typedef int(__thiscall* Fn) (void*, const char*);
		return GetVF<Fn>(this, 2)(this, name);
	}

	//inline modtype_t GetModelType(const model_t* model)
	//{
	//	typedef modtype_t(__thiscall* Fn) (void*, const model_t*);
	//	return GetVF<Fn>(this, 11)(this, model);
	//}

	//inline void GetModelMaterials(const model_t* model, int count, IMaterial** ppMaterial)
	//{
	//	typedef void (__thiscall* Fn) (void*, const model_t*, int, IMaterial**);
	//	GetVF<Fn>(this, 19)(this, model, count, ppMaterial);
	//}

	//inline virtualmodel_t* GetVirtualModel(const studiohdr_t* pStudioHdr)
	//{
	//	typedef virtualmodel_t* (__thiscall* Fn) (void*, const studiohdr_t*);
	//	return GetVF<Fn>(this, 27)(this, pStudioHdr);
	//}

	inline studiohdr_t* GetStudioModel(const model_t* model)
	{
		typedef studiohdr_t* (__thiscall* Fn) (void*, const model_t*);
		return GetVF<Fn>(this, 32)(this, model);
	}

	inline const model_t* FindOrLoadModel(const char* name)
	{
		typedef const model_t* (__thiscall* Fn) (void*, const char*);
		return GetVF<Fn>(this, 43)(this, name);
	}

	inline MDLHandle_t GetCacheHandle(const model_t* model)
	{
		typedef MDLHandle_t (__thiscall* Fn) (void*, const model_t*);
		return GetVF<Fn>(this, 44)(this, model);
	}
};