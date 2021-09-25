#pragma once

#define BONE_CALCULATE_MASK			0x1F
#define BONE_PHYSICALLY_SIMULATED	0x01	// bone is physically simulated when physics are active
#define BONE_PHYSICS_PROCEDURAL		0x02	// procedural when physics is active
#define BONE_ALWAYS_PROCEDURAL		0x04	// bone is always procedurally animated
#define BONE_SCREEN_ALIGN_SPHERE	0x08	// bone aligns to the screen, not constrained in motion.
#define BONE_SCREEN_ALIGN_CYLINDER	0x10	// bone aligns to the screen, constrained by it's own axis.
#define BONE_WORLD_ALIGN			0x20	// bone is rigidly aligned to the world (but can still translate)

#define BONE_USED_MASK				0x000FFF00
#define BONE_USED_BY_ANYTHING		0x000FFF00
#define BONE_USED_BY_HITBOX			0x00000100	// bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT		0x00000200	// bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK	0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0	0x00000400	// bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1	0x00000800	
#define BONE_USED_BY_VERTEX_LOD2	0x00001000  
#define BONE_USED_BY_VERTEX_LOD3	0x00002000
#define BONE_USED_BY_VERTEX_LOD4	0x00004000
#define BONE_USED_BY_VERTEX_LOD5	0x00008000
#define BONE_USED_BY_VERTEX_LOD6	0x00010000
#define BONE_USED_BY_VERTEX_LOD7	0x00020000
#define BONE_USED_BY_BONE_MERGE		0x00040000	// bone is available for bone merge to occur against it
#define BONE_ALWAYS_SETUP			0x00080000

#define BONE_USED_BY_VERTEX_AT_LOD(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD(lod) )

#define MAX_NUM_LODS 8

#define BONE_TYPE_MASK				0x00F00000
#define BONE_FIXED_ALIGNMENT		0x00100000	// bone can't spin 360 degrees, all interpolation is normalized around a fixed orientation

#define BONE_HAS_SAVEFRAME_POS		0x00200000	// Vector48
#define BONE_HAS_SAVEFRAME_ROT64	0x00400000	// Quaternion64
#define BONE_HAS_SAVEFRAME_ROT32	0x00800000	// Quaternion32

// actually 256 in 2017
#define MAXSTUDIOBONES		128

// vec_t = float;
// RadianEuler = float[3]
// Quaternion = float[4]

struct mstudiobone_t
{
	int					szNameIndex;
	inline char* const	pszName() const { return reinterpret_cast<char*>(const_cast<mstudiobone_t*>(this)) + szNameIndex; }
	int		 			parent;				// parent bone
	int					bonecontroller[6];	// bone controller index, -1 == none

	// default values
	vec3				pos;
	float				quat[4];
	vec3				rot;

	// compression scale
	vec3				posscale;
	vec3				rotscale;

	matrix3x4_t			poseToBone;
	float				qAlignment[4];
	int					flags;
	int					proctype;
	int					procindex;			// procedural rule
	mutable int			physicsbone;		// index into physically simulated bone
	inline void*		pProcedure() const { return procindex ? static_cast<void*>(reinterpret_cast<BYTE*>(const_cast<mstudiobone_t*>(this)) + procindex) : 0; };
	int					surfacepropidx;		// index into string tablefor property name
	inline char* const	pszSurfaceProp() const { return reinterpret_cast<char*>(const_cast<mstudiobone_t*>(this)) + surfacepropidx; }

	int					contents;			// See BSPFlags.h for the contents flags
	int					surfacepropLookup;	// this index must be cached by the loader, not saved in the file
	int					unused[7];

	mstudiobone_t() {}
private:
	// No copy constructors allowed
	mstudiobone_t(const mstudiobone_t& vOther);
};

struct mstudiobbox_t
{
	int					bone;
	int					group;				// intersection group
	vec3				bbmin;				// bounding box, or the ends of the capsule if flCapsuleRadius > 0 
	vec3				bbmax;
	int					szHitboxNameIndex;	// offset to the name of the hitbox.
	vec3				angOffsetOrientation;
	float				flCapsuleRadius;
	int					unused[4];

	const char* pszHitboxName() const { return szHitboxNameIndex ? reinterpret_cast<const char*>(this) + szHitboxNameIndex : ""; }

	mstudiobbox_t() {}
private:
	// No copy constructors allowed
	mstudiobbox_t(const mstudiobbox_t& vOther);
};

struct mstudiohitboxset_t
{
	int					szNameIndex;
	inline char* const	pszName() const { return reinterpret_cast<char*>(const_cast<mstudiohitboxset_t*>(this)) + szNameIndex; }
	int					numhitboxes;
	int					hitboxindex;
	inline mstudiobbox_t*	pHitbox(int i) const { return reinterpret_cast<mstudiobbox_t*>(reinterpret_cast<BYTE*>(const_cast<mstudiohitboxset_t*>(this)) + hitboxindex) + i; };
};

class virtualmodel_t;

struct studiohdr_t
{
	int					id;
	int					version;
	long				checksum;			// this has to be the same in the phy and vtx files to load!
	
	inline const char* pszName(void) const { return name; }
	char				name[64];
	int					length;

	vec3				eyeposition;		// ideal eye position
	vec3				illumposition;		// illumination center

	vec3				hull_min;			// ideal movement hull size
	vec3				hull_max;

	vec3				view_bbmin;			// clipping bounding box
	vec3				view_bbmax;

	int					flags;

	int					numbones;
	int					boneindex;
	inline mstudiobone_t*	pBone(int i) const { assert(0 <= i && i < numbones); return reinterpret_cast<mstudiobone_t*>(reinterpret_cast<BYTE*>(const_cast<studiohdr_t*>(this)) + boneindex) + i; };

	int					numbonecontrollers;
	int					bonecontrollerindex;

	int					numhitboxsets;
	int					hitboxsetindex;

	// Look up hitbox set by index
	mstudiohitboxset_t* pHitboxSet(int i) const { assert(0 <= i && i < numhitboxsets); return reinterpret_cast<mstudiohitboxset_t*>(reinterpret_cast<BYTE*>(const_cast<studiohdr_t*>(this)) + hitboxsetindex) + i; };

	// Calls through to hitbox to determine size of specified set
	inline mstudiobbox_t* pHitbox(int i, int set) const { return pHitboxSet(set) ? pHitboxSet(set)->pHitbox(i) : 0; }

	// Calls through to set to get hitbox count for set
	inline int iHitboxCount(int set) const { return pHitboxSet(set) ? pHitboxSet(set)->numhitboxes : 0; }

	// file local animations? and sequences
	int					numlocalanim;			// animations/poses
	int					localanimindex;			// animation descriptions

	int					numlocalseq;			// sequences
	int					localseqindex;

	mutable int			activitylistversion;	// initialization flag - have the sequences been indexed?
	mutable int			eventsindexed;

	// raw textures
	int					numtextures;
	int					textureindex;

	// raw textures search paths
	int					numcdtextures;
	int					cdtextureindex;
	inline char*		pCdtexture(int i) const { return (reinterpret_cast<char*>(const_cast<studiohdr_t*>(this)) + *(reinterpret_cast<int*>(reinterpret_cast<BYTE*>(const_cast<studiohdr_t*>(this)) + cdtextureindex) + i)); };

	// replaceable textures tables
	int					numskinref;
	int					numskinfamilies;
	int					skinindex;
	inline short*		pSkinref(int i) const { return reinterpret_cast<short*>(reinterpret_cast<BYTE*>(const_cast<studiohdr_t*>(this)) + skinindex) + i; };

	int					numbodyparts;
	int					bodypartindex;

	// queryable attachable points
	int					numlocalattachments;
	int					localattachmentindex;

	// animation node to animation node transition graph
	int					numlocalnodes;
	int					localnodeindex;
	int					localnodenameindex;
	inline char*		pszLocalNodeName(int iNode) const { assert(0 <= iNode && iNode < numlocalnodes); return (reinterpret_cast<char*>(const_cast<studiohdr_t*>(this)) + *(reinterpret_cast<int*>(reinterpret_cast<BYTE*>(const_cast<studiohdr_t*>(this)) + localnodenameindex) + iNode)); }
	inline BYTE*		pLocalTransition(int i) const { assert(0 <= i && i < (numlocalnodes * numlocalnodes)); return static_cast<BYTE*>(reinterpret_cast<BYTE*>(const_cast<studiohdr_t*>(this)) + localnodeindex) + i; };

	int					numflexdesc;
	int					flexdescindex;

	int					numflexcontrollers;
	int					flexcontrollerindex;

	int					numflexrules;
	int					flexruleindex;

	int					numikchains;
	int					ikchainindex;

	int					nummouths;
	int					mouthindex;

	int					numlocalposeparameters;
	int					localposeparamindex;

	int					surfacepropindex;
	inline char* const	pszSurfaceProp() const { return reinterpret_cast<char*>(const_cast<studiohdr_t*>(this)) + surfacepropindex; }
	inline int			GetSurfaceProp() const { return surfacepropLookup; }

	int					keyvalueindex;
	int					keyvaluesize;
	inline const char*	KeyValueText() const { return keyvaluesize ? reinterpret_cast<char*>(const_cast<studiohdr_t*>(this)) + keyvalueindex : 0; }

	int					numlocalikautoplaylocks;
	int					localikautoplaylockindex;

	// The collision model mass that jay wanted
	float				mass;
	int					contents;

	// external animations, models, etc.
	int					numincludemodels;
	int					includemodelindex;

	// implementation specific back pointer to virtual data. Relocated to studiohdr2_t
	int					unused_virtualModel;

	//virtualmodel_t* GetVirtualModel() const
	//{
	//	if (numincludemodels == 0)
	//		return NULL;
	//	return I::ModelInfo->GetVirtualModel(this);
	//}

	// for demand loaded animation blocks
	int					szanimblocknameindex;
	inline char* const	pszAnimBlockName() const { return reinterpret_cast<char*>(const_cast<studiohdr_t*>(this)) + szanimblocknameindex; }
	int					numanimblocks;
	int					animblockindex;
	// Relocated to studiohdr2_t
	int					unused_animblockModel;

	int					bonetablebynameindex;
	inline const BYTE*	GetBoneTableSortedByName() const { return reinterpret_cast<BYTE*>(const_cast<studiohdr_t*>(this)) + bonetablebynameindex; }

	// used by tools only that don't cache, but persist mdl's peer data
	// engine uses virtualModel to back link to cache pointers
	// Relocated to studiohdr2_t
	int                 unused_pVertexBase;
	int                 unused_pIndexBase;

	// if STUDIOHDR_FLAGS_CONSTANT_DIRECTIONAL_LIGHT_DOT is set,
	// this value is used to calculate directional components of lighting on static props
	BYTE				constdirectionallightdot;

	// set during load of mdl data to track *desired* lod configuration (not actual)
	// the *actual* clamped root lod is found in studiohwdata
	// this is stored here as a global store to ensure the staged loading matches the rendering
	BYTE				rootLOD;

	// set in the mdl data to specify that lod configuration should only allow first numAllowRootLODs
	// to be set as root LOD:
	//	numAllowedRootLODs = 0	means no restriction, any lod can be set as root lod.
	//	numAllowedRootLODs = N	means that lod0 - lod(N-1) can be set as root lod, but not lodN or lower.
	BYTE				numAllowedRootLODs;

	BYTE				unused[1];
	int					unused4; // zero out if version < 47

	int					numflexcontrollerui;
	int					flexcontrolleruiindex;

	float				flVertAnimFixedPointScale;
	inline float		VertAnimFixedPointScale() const { return (flags & (1 << 21)) ? flVertAnimFixedPointScale : 1.0f / 4096.0f; }

	mutable int			surfacepropLookup;	// this index must be cached by the loader, not saved in the file

	int					studiohdr2index;

	int					unused2[1];

	studiohdr_t() {}
private:
	// No copy constructors allowed
	studiohdr_t(const studiohdr_t& vOther);
};

struct model_t
{
	enum modtype_t
	{
		MOD_BAD = 0,
		MOD_BRUSH,
		MOD_SPRITE,
		MOD_STUDIO
	};

	void*			fnHandle;
	char			szPathName[260];
	int				nLoadFlags;		// mark loaded/not loaded
	int				nServerCount;	// marked at load
	modtype_t		type;
	int				flags;			// MODELFLAG_???
									// volume occupied by the model graphics
	vec3			mins, maxs;
	float			radius;
	BYTE			union1[16];
};

class IMDLCache;

class CStudioHdr
{
public:
	//CStudioHdr(const studiohdr_t* pStudioHdr, IMDLCache* mdlcache = NULL)
	//{
	//	typedef void(__thiscall* Fn)(CStudioHdr*, const studiohdr_t*, IMDLCache*);
	//	reinterpret_cast<Fn>(Signatures::CStudioHdrConstructor)(this, pStudioHdr, mdlcache);
	//}

	//~CStudioHdr() { Term(); }

	// term does more then just that
	//void Term() { m_pActivityToSequence = NULL; }

	inline bool				IsVirtual(void) { return (m_pVModel != NULL); };
	inline bool				IsValid(void) { return (m_pStudioHdr != NULL); };
	inline bool				IsReadyForAccess(void) const { return (m_pStudioHdr != NULL); };
	inline virtualmodel_t*	GetVirtualModel(void) const { return m_pVModel; };
	inline const studiohdr_t* GetRenderHdr(void) const { return m_pStudioHdr; };

private:
	mutable const studiohdr_t*					m_pStudioHdr;
	mutable virtualmodel_t*						m_pVModel;
	mutable /*CSoftbody**/void*					m_pSoftbody;

	mutable CUtlVector< const studiohdr_t* >	m_pStudioHdrCache;

	mutable int									m_nFrameUnlockCounter;
	int*										m_pFrameUnlockCounter;
	/*CThreadFastMutex*/BYTE					m_FrameUnlockCounterMutex[8];

public:
	inline int				numbones(void) const { return m_pStudioHdr->numbones; };
	inline mstudiobone_t*	pBone(int i) const { return m_pStudioHdr->pBone(i); };

	inline int			GetNumBoneControllers(void) const { return m_pStudioHdr->numbonecontrollers; };

	inline int			numikchains() const { return m_pStudioHdr->numikchains; };
	inline int			GetNumIKChains(void) const { return m_pStudioHdr->numikchains; };

	inline int			numflexrules() const { return m_pStudioHdr->numflexrules; };

	inline int			numflexdesc() const { return m_pStudioHdr->numflexdesc; };

	inline int			numflexcontrollerui() const { return m_pStudioHdr->numflexcontrollerui; };

	inline const char*	pszName() const { return m_pStudioHdr->pszName(); };

	inline int			numbonecontrollers() const { return m_pStudioHdr->numbonecontrollers; };

	inline int			numhitboxsets() const { return m_pStudioHdr->numhitboxsets; };
	inline mstudiohitboxset_t* pHitboxSet(int i) const { return m_pStudioHdr->pHitboxSet(i); };

	inline mstudiobbox_t* pHitbox(int i, int set) const { return m_pStudioHdr->pHitbox(i, set); };
	inline int			iHitboxCount(int set) const { return m_pStudioHdr->iHitboxCount(set); };

	inline int			numbodyparts() const { return m_pStudioHdr->numbodyparts; };

	inline int			numskinfamilies() const { return m_pStudioHdr->numskinfamilies; }

	inline vec3			eyeposition() const { return m_pStudioHdr->eyeposition; };

	inline int			flags() const { return m_pStudioHdr->flags; };

	inline char* const	pszSurfaceProp(void) const { return m_pStudioHdr->pszSurfaceProp(); };
	inline int			GetSurfaceProp()const { return m_pStudioHdr->surfacepropLookup; }

	inline float		mass() const { return m_pStudioHdr->mass; };
	inline int			contents() const { return m_pStudioHdr->contents; }

	inline const BYTE*	GetBoneTableSortedByName() const { return m_pStudioHdr->GetBoneTableSortedByName(); };

	inline vec3			illumposition() const { return m_pStudioHdr->illumposition; };

	inline vec3			hull_min() const { return m_pStudioHdr->hull_min; };		// ideal movement hull size
	inline vec3			hull_max() const { return m_pStudioHdr->hull_max; };

	inline vec3			view_bbmin() const { return m_pStudioHdr->view_bbmin; };		// clipping bounding box
	inline vec3			view_bbmax() const { return m_pStudioHdr->view_bbmax; };

	inline int			numtextures() const { return m_pStudioHdr->numtextures; };

	inline int			boneFlags(int iBone) const { return m_boneFlags[iBone]; }
	inline void			setBoneFlags(int iBone, int flags) { m_boneFlags[iBone] |= flags; }
	inline void			clearBoneFlags(int iBone, int flags) { m_boneFlags[iBone] &= ~flags; }
	inline int			boneParent(int iBone) const { return m_boneParent[iBone]; }

private:
	CUtlVector< int >  m_boneFlags;
	CUtlVector< int >  m_boneParent;

public:

	/*CActivityToSequenceMapping**/void* m_pActivityToSequence;

private:
	/*CUtlDict<int, int>*/BYTE m_namedSequence[6*5];

#ifdef STUDIO_ENABLE_PERF_COUNTERS
	inline void			ClearPerfCounters(void)
	{
		m_nPerfAnimatedBones = 0;
		m_nPerfUsedBones = 0;
		m_nPerfAnimationLayers = 0;
	};

	// timing info
	mutable	int			m_nPerfAnimatedBones;
	mutable	int			m_nPerfUsedBones;
	mutable	int			m_nPerfAnimationLayers;
#endif
};