#pragma once

#define MAX_PLAYERS 64

class CBaseHandle;

// An IHandleEntity-derived class can go into an entity list and use ehandles.
class IHandleEntity
{
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle(const CBaseHandle& handle) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};

class ICollideable;
class IClientNetworkable;
class IClientRenderable;
class IClientEntity;
class C_BaseEntity;
class IClientThinkable;
class IClientAlphaProperty;

class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable* GetCollideable() = 0;
	virtual IClientNetworkable* GetClientNetworkable() = 0;
	virtual IClientRenderable* GetClientRenderable() = 0;
	virtual IClientEntity* GetIClientEntity() = 0;
	virtual C_BaseEntity* GetBaseEntity() = 0;
	virtual IClientThinkable* GetClientThinkable() = 0;
	virtual IClientAlphaProperty* GetClientAlphaProperty() = 0;
};

class IClientNetworkable
{
public:
	inline IClientUnknown* GetIClientUnknown()
	{
		typedef IClientUnknown* (__thiscall* Fn) (IClientNetworkable*);
		return GetVF<Fn>(this, 0)(this);
	}

	inline ClientClass* GetClientClass()
	{
		typedef ClientClass* (__thiscall* Fn) (IClientNetworkable*);
		return GetVF<Fn>(this, 2)(this);
	}
};

class IClientRenderable
{
public:
	inline IClientUnknown* GetIClientUnknown()
	{
		typedef IClientUnknown* (__thiscall* Fn) (IClientRenderable*);
		return GetVF<Fn>(this, 0)(this);
	}

	// TODO: Setup bones of enemies behind
	inline bool SetupBones(matrix3x4a_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		//*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_lastOcclusionCheck) = G::pGlobals->framecount;
		//*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_occlusionFlags) = 0;

		typedef bool(__thiscall* Fn) (IClientRenderable*, matrix3x4a_t*, int, int, float);
		return GetVF<Fn>(this, 13)(this, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}
};

// Flags
#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define FL_ANIMDUCKING			(1<<2)	// Player flag -- Player is in the process of crouching or uncrouching but could be in transition
// examples:                                   Fully ducked:  FL_DUCKING &  FL_ANIMDUCKING
//           Previously fully ducked, unducking in progress:  FL_DUCKING & !FL_ANIMDUCKING
//                                           Fully unducked: !FL_DUCKING & !FL_ANIMDUCKING
//           Previously fully unducked, ducking in progress: !FL_DUCKING &  FL_ANIMDUCKING
#define	FL_WATERJUMP			(1<<3) // Player jumping out of water
#define FL_ONTRAIN				(1<<4) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5) // Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8) // Is a player
#define FL_FAKECLIENT			(1<<9) // Fake client, simulated server side; don't send network messages to them
// NON-PLAYER SPECIFIC (i.e., not used by GameMovement or the client .dll ) -- Can still be applied to players, though
#define	FL_INWATER				(1<<10) // In water

enum MoveType_t : BYTE
{
	MOVETYPE_NONE = 0,			// never moves
	MOVETYPE_ISOMETRIC,			// For players -- in TF2 commander view, etc.
	MOVETYPE_WALK,				// Player only - moving on the ground
	MOVETYPE_STEP,				// gravity, special edge handling -- monsters use this
	MOVETYPE_FLY,				// No gravity, but still collides with stuff
	MOVETYPE_FLYGRAVITY,		// flies through the air + is affected by gravity
	MOVETYPE_VPHYSICS,			// uses VPHYSICS for simulation
	MOVETYPE_PUSH,				// no clip to world, push and crush
	MOVETYPE_NOCLIP,			// No gravity, no collisions, still do velocity/avelocity
	MOVETYPE_LADDER,			// Used by players only when going onto a ladder
	MOVETYPE_OBSERVER,			// Observer movement, depends on player's observer mode
	MOVETYPE_CUSTOM,			// Allows the entity to describe its own physics

	// should always be defined as the last item in the list
	MOVETYPE_LAST = MOVETYPE_CUSTOM,

	MOVETYPE_MAX_BITS = 4
};

class CCollisionProperty
{
public:
	inline const vec3& OBBMins()
	{
		typedef const vec3& (__thiscall* Fn)(void*);
		return GetVF<Fn>(this, 1)(this);
	}

	inline const vec3& OBBMaxs()
	{
		typedef const vec3& (__thiscall* Fn)(void*);
		return GetVF<Fn>(this, 2)(this);
	}

	inline void SetCollisionBounds(const vec3& mins, const vec3& maxs)
	{
		typedef void(__thiscall* Fn)(void*, const vec3&, const vec3&);
		reinterpret_cast<Fn>(Signatures::SetCollisionBounds)(this, mins, maxs);
	}
};

class C_BaseAnimating;

class C_BaseEntity
{
public:
	// ghetto
	inline IClientRenderable*	GetRenderable() { return reinterpret_cast<IClientRenderable*>(reinterpret_cast<uintptr_t>(this) + 0x4); }
	inline IClientNetworkable*	GetNetworkable() { return reinterpret_cast<IClientNetworkable*>(reinterpret_cast<uintptr_t>(this) + 0x8); }

	inline int				entindex() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x64); }
	inline const model_t*	GetModel() { return *reinterpret_cast<const model_t**>(reinterpret_cast<uintptr_t>(this) + 0x6C); }
	inline bool				IsDormant() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0xED); }
	inline int				GetTeam() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iTeamNum); }
	inline int				GetHealth() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iHealth); }
	inline int				GetFlags() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_fFlags); }
	inline vec3				GetLocalOrigin() { return *reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(this) + 0xAC); }
	inline vec3				GetLocalAngles() { return *reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(this) + 0xD0); }
	inline vec3				GetNetworkAngles() { return *reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_angRotation); }
	inline vec3				GetNetworkOrigin() { return *reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_vecOrigin); }
	inline vec3				GetVelocity() { return *reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_vecVelocity); }
	inline vec3				GetEyePos() { vec3 pos(GetNetworkOrigin()); pos.z += *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_vecViewOffset + 0x8); return pos; }
	inline MoveType_t		GetMoveType() { return *reinterpret_cast<MoveType_t*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_MoveType); }
	inline CCollisionProperty* CollisionProp() { return reinterpret_cast<CCollisionProperty*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_Collision); }
	inline const vec3&		WorldAlignMins() { return CollisionProp()->OBBMins(); }
	inline const vec3&		WorldAlignMaxs() { return CollisionProp()->OBBMaxs(); }
	inline const matrix3x4_t& GetCoordinateFrame() { return *reinterpret_cast<matrix3x4_t*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_rgflCoordinateFrame); } // Specifies the entity-to-world transform
	inline C_BaseEntity*	GetOwnerEntity() { return I::EntityList->GetClientEntityFromHandle(*reinterpret_cast<ULONG*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_hOwnerEntity)); }
	inline float			GetAnimTime() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flAnimTime); }
	inline float			GetSimulationTime() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flSimulationTime); }
	inline float			GetLastMessageTime() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flLastMessageTime); }

	inline void				SetAnimTime(float time) { *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flAnimTime) = time; }
	inline void				SetLocalOrigin(const vec3& origin) { *reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(this) + 0xAC) = origin; }
	inline void				SetLocalAngles(const vec3& angles) { *reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(this) + 0xD0) = angles; }
	inline void				SetSize(const vec3& mins, const vec3& maxs) { CollisionProp()->SetCollisionBounds(mins, maxs); }
	inline void				SetSpotted(bool val) { *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bSpotted) = val; }

	inline const vec3& GetAbsOrigin()
	{
		typedef const vec3& (__thiscall* Fn) (void*);
		return GetVF<Fn>(this, 10)(this);
	}

	inline const vec3& GetAbsAngles()
	{
		typedef const vec3& (__thiscall* Fn) (void*);
		return GetVF<Fn>(this, 11)(this);
	}

	inline C_BaseAnimating* GetBaseAnimating()
	{
		typedef C_BaseAnimating* (__thiscall* Fn) (void*);
		return GetVF<Fn>(this, 45)(this);
	}

	inline void SetModelIndex(int model)
	{
		typedef void(__thiscall* Fn) (void*, int);
		GetVF<Fn>(this, 75)(this, model);
	}

	// NOTE: These use the collision OBB to compute a reasonable center point for the entity
	inline const vec3& WorldSpaceCenter()
	{
		typedef const vec3& (__thiscall* Fn) (void*);
		return GetVF<Fn>(this, 78)(this);
	}

	//inline const char* GetClassname()
	//{
	//	typedef const char* (__thiscall* Fn) (void*);
	//	return GetVF<Fn>(this, 142)(this);
	//}

	inline void EstimateAbsVelocity(Vector& vel)
	{
		typedef void (__thiscall* Fn) (void*, Vector&);
		GetVF<Fn>(this, 144)(this, vel);
	}

	inline bool IsPlayer()
	{
		typedef bool (__thiscall* Fn) (void*);
		return GetVF<Fn>(this, 158)(this);
	}

	inline bool IsBaseCombatWeapon()
	{
		typedef bool(__thiscall* Fn) (void*);
		return GetVF<Fn>(this, 166)(this);
	}

	inline void SetAbsOrigin(const vec3& origin)
	{
		typedef void(__thiscall* Fn) (void*, const vec3&);
		reinterpret_cast<Fn>(Signatures::SetAbsOrigin)(this, origin);
	}

	inline void SetAbsAngles(const vec3& angles)
	{
		typedef void(__thiscall* Fn) (void*, const vec3&);
		reinterpret_cast<Fn>(Signatures::SetAbsAngles)(this, angles);
	}
};

struct C_AnimationLayer;

class C_BaseAnimating : public C_BaseEntity
{
public:
	inline int				GetHitboxSet() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nHitboxSet); }
	inline float			GetModelScale() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flModelScale); }
	inline CStudioHdr*		GetModelPtr() { return *reinterpret_cast<CStudioHdr**>(reinterpret_cast<uintptr_t>(this) + NetVars::m_pStudioHdr); }
	inline int				GetSequence() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nSequence); }
	inline int				GetPrevSequence() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nPrevSequence); }
	inline float			GetCycle() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flCycle); }
	inline int				GetNewSequenceParity() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nNewSequenceParity); }
	inline CUtlVector<C_AnimationLayer>* GetAnimOverlays() { return reinterpret_cast<CUtlVector<C_AnimationLayer>*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_AnimOverlay); } // isnt c_baseanimating actually
	
	inline void				SetCycle(float cycle) { *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flCycle) = cycle; }
	inline void				SetAllowFastPath(bool bAllow) { *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bCanUseFastPath) = bAllow; }

	inline void InvalidateBoneCache()
	{
		*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iMostRecentModelBoneCounter) =
			*reinterpret_cast<int*>(Signatures::g_iModelBoneCounter) - 1;

		*reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flLastBoneSetupTime) = -FLT_MAX;
	}

	inline bool IsBoneCacheValid() const
	{
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iMostRecentModelBoneCounter) ==
			*reinterpret_cast<int*>(Signatures::g_iModelBoneCounter);
	}

	inline CUtlVector<matrix3x4a_t/*, CUtlMemoryAligned<matrix3x4a_t, 16>*/>* GetCachedBones()
	{
		return reinterpret_cast<CUtlVector<matrix3x4a_t>*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_CachedBoneData);
	}

	inline bool HitboxToWorldTransforms(matrix3x4_t* pHitboxToWorld[MAXSTUDIOBONES])
	{
		if (!IsBoneCacheValid())
		{
			if (!GetModel())
				return false;

			const CStudioHdr* pStudioHdr = GetModelPtr();
			if (!pStudioHdr)
				return false;

			mstudiohitboxset_t* set = pStudioHdr->pHitboxSet(GetHitboxSet());
			if (!set)
				return false;

			if (!set->numhitboxes)
				return false;

			GetRenderable()->SetupBones(nullptr, -1, BONE_USED_BY_HITBOX, G::Globals->curtime);
		}

		for (int i = 0; i < GetCachedBones()->Count(); ++i)
			pHitboxToWorld[i] = &GetCachedBones()->Element(i);

		return true;
	}

	//inline void SetSequence(int seq)
	//{
	//	typedef void(__thiscall* Fn) (void*, int);
	//	GetVF<Fn>(this, 219)(this, seq);
	//}

	//inline void UpdateClientSideAnimation()
	//{
	//	typedef void(__thiscall* Fn) (void*);
	//	GetVF<Fn>(this, 224)(this);
	//}
};

inline int GetSequenceActivity(CStudioHdr* pstudiohdr, int sequence)
{
	typedef int(__fastcall* Fn) (CStudioHdr*, int);
	return reinterpret_cast<Fn>(Signatures::GetSequenceActivity)(pstudiohdr, sequence);
}

//inline const char* GetSequenceActivityName(CStudioHdr* hdr, int seq)
//{
//	typedef const char*(__fastcall* Fn) (CStudioHdr*, int);
//	return reinterpret_cast<Fn>(Signatures::GetSequenceActivityName)(hdr, seq);
//}

//inline int LookupSequence(CStudioHdr* hdr, const char* seq)
//{
//	typedef int(__fastcall* Fn) (CStudioHdr*, const char*);
//	return reinterpret_cast<Fn>(Signatures::LookupSequence)(hdr, seq);
//}

inline int SelectWeightedSequence(CStudioHdr* pstudiohdr, int activity)
{
	typedef int(__fastcall* Fn)(CStudioHdr*, int);
	return reinterpret_cast<Fn>(Signatures::SelectWeightedSequence)(pstudiohdr, activity);
}

class C_PlantedC4 : public C_BaseAnimating
{
public:
	inline bool		IsBombTicking() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bBombTicking); }
	inline float	GetBlowTime() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flC4Blow); }
	inline ULONG	GetDefuser() { return *reinterpret_cast<ULONG*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_hBombDefuser); }
	inline float	GetDefuseCountDown() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flDefuseCountDown); }
};

class C_BaseViewModel : public C_BaseAnimating
{
public:
	inline C_BaseEntity* GetWeapon() { return I::EntityList->GetClientEntityFromHandle(
			*reinterpret_cast<ULONG*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_hWeapon)); }

	inline C_BaseEntity* GetOwner() { return I::EntityList->GetClientEntityFromHandle(
			*reinterpret_cast<ULONG*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_hOwner)); }

	//inline void SendViewModelMatchingSequence(int seq)
	//{
	//	typedef void(__thiscall* Fn) (void*, int);
	//	GetVF<Fn>(this, 247)(this, seq);
	//}
};


class C_BaseCSGrenadeProjectile : public C_BaseAnimating
{
public:
	inline float			GetDamage() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flDamage); }
	inline int				GetExplodeEffectTickBegin() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nExplodeEffectTickBegin); }
};

class C_DecoyProjectile : public C_BaseCSGrenadeProjectile
{
public:
	inline float			GetParticleInitTime() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_decoyParticleInitTime); }
};

class C_SmokeGrenadeProjectile : public C_BaseCSGrenadeProjectile
{
public:
	inline int				GetSmokeEffectTickBegin() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nSmokeEffectTickBegin); }
};

class C_Inferno : public C_BaseEntity
{
public:
	// these are the actual visible fires
	enum FireState { STARTING, BURNING, GOING_OUT, FIRE_OUT, UNKNOWN };
	struct Drawable
	{
		Vector m_pos;							// position of flame
		Vector m_normal;						// normal of flame surface
		int m_frame;							// current animation frame
		float m_framerate;						// rate of animation
		bool m_mirror;							// if true, flame is mirrored about vertical axis

		int m_dlightIndex;

		FireState m_state;						// the state of this fire
		float m_stateTimestamp;					// when the fire entered its current state

		float m_size;							// current flame size
		float m_maxSize;						// maximum size of full-grown flame
	};

	inline Drawable*	GetDrawable(int index) { return &reinterpret_cast<Drawable*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_drawable)[index]; }
	inline int			GetDrawableCount() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_drawableCount); }
	inline int			GetFireEffectTickBegin() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nFireEffectTickBegin); }
};


struct C_AnimationLayer
{
	float	m_flLayerAnimtime;
	float	m_flLayerFadeOuttime;

	// dispatch flags
	CStudioHdr* m_pDispatchedStudioHdr;
	int		m_nDispatchedSrc;
	int		m_nDispatchedDst;

	int		m_nOrder;
	int		m_nSequence;
	float	m_flPrevCycle;
	float	m_flWeight;
	float	m_flWeightDeltaRate;

	// used for automatic crossfades between sequence changes
	float	m_flPlaybackRate;
	float	m_flCycle;

	/*C_BaseAnimatingOverlay**/C_BaseAnimating* m_pOwner;
	int		m_nInvalidatePhysicsBits;
};