#pragma once

#define CONTENTS_EMPTY			0		// No contents

#define CONTENTS_SOLID			0x1		// an eye is never valid in a solid
#define CONTENTS_WINDOW			0x2		// translucent, but not watery (glass)
#define CONTENTS_AUX			0x4
#define CONTENTS_GRATE			0x8		// alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't
#define CONTENTS_SLIME			0x10
#define CONTENTS_WATER			0x20
#define CONTENTS_BLOCKLOS		0x40	// block AI line of sight
#define CONTENTS_OPAQUE			0x80	// things that cannot be seen through (may be non-solid though)
#define LAST_VISIBLE_CONTENTS	CONTENTS_OPAQUE

#define ALL_VISIBLE_CONTENTS (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define CONTENTS_TESTFOGVOLUME	0x100
#define CONTENTS_UNUSED			0x200

// unused
// NOTE: If it's visible, grab from the top + update LAST_VISIBLE_CONTENTS
// if not visible, then grab from the bottom.
// CONTENTS_OPAQUE + SURF_NODRAW count as CONTENTS_OPAQUE (shadow-casting toolsblocklight textures)
#define CONTENTS_BLOCKLIGHT		0x400

#define CONTENTS_TEAM1			0x800	// per team contents used to differentiate collisions
#define CONTENTS_TEAM2			0x1000	// between players and objects on different teams

// ignore CONTENTS_OPAQUE on surfaces that have SURF_NODRAW
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000

// hits entities which are MOVETYPE_PUSH (doors, plats, etc.)
#define CONTENTS_MOVEABLE		0x4000

// remaining contents are non-visible, and don't eat brushes
#define CONTENTS_AREAPORTAL		0x8000

#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_MONSTERCLIP	0x20000

#define	CONTENTS_BRUSH_PAINT	0x40000
#define	CONTENTS_GRENADECLIP	0x80000
#define	CONTENTS_UNUSED2		0x100000
#define	CONTENTS_UNUSED3		0x200000
#define	CONTENTS_UNUSED4		0x400000
#define	CONTENTS_UNUSED5		0x800000

#define CONTENTS_ORIGIN			0x1000000	// removed before bsping an entity

#define CONTENTS_MONSTER		0x2000000	// should never be on a brush, only in game
#define CONTENTS_DEBRIS			0x4000000
#define CONTENTS_DETAIL			0x8000000	// brushes to be added after vis leafs
#define CONTENTS_TRANSLUCENT	0x10000000	// auto set if any surface has trans
#define CONTENTS_LADDER			0x20000000
#define CONTENTS_HITBOX			0x40000000	// use accurate hitboxes on trace

// -----------------------------------------------------
// spatial content masks - used for spatial queries (traceline,etc.)
// -----------------------------------------------------
#define	MASK_ALL					(0xFFFFFFFF)
// everything that is normally solid
#define	MASK_SOLID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// everything that blocks player movement
#define	MASK_PLAYERSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// blocks npc movement
#define	MASK_NPCSOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// blocks fluid movement
#define	MASK_NPCFLUID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
// water physics in these contents
#define	MASK_WATER					(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
// everything that blocks lighting
#define	MASK_OPAQUE					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
// everything that blocks lighting, but with monsters added.
#define MASK_OPAQUE_AND_NPCS		(MASK_OPAQUE|CONTENTS_MONSTER)
// everything that blocks line of sight for AI
#define MASK_BLOCKLOS				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
// everything that blocks line of sight for AI plus NPCs
#define MASK_BLOCKLOS_AND_NPCS		(MASK_BLOCKLOS|CONTENTS_MONSTER)
// everything that blocks line of sight for players
#define	MASK_VISIBLE					(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
// everything that blocks line of sight for players, but with monsters added.
#define MASK_VISIBLE_AND_NPCS		(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
// bullets see these as solid
#define	MASK_SHOT					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
// for finding floor height
#define MASK_FLOORTRACE				(CONTENTS_SOLID|CONTENTS_MOVEABLE|                 CONTENTS_WINDOW|CONTENTS_DEBRIS)
// for csgo weapon clipping
#define MASK_WEAPONCLIPPING			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS)
// bullets see these as solid, except monsters (world+brush only)
#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
// non-raycasted weapons see this as solid (includes grates)
#define MASK_SHOT_HULL				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
// hits solids (not grates) and passes through everything else
#define MASK_SHOT_PORTAL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
// everything normally solid, except monsters (world+brush only)
#define MASK_SOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
// everything normally solid for player movement, except monsters (world+brush only)
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
// everything normally solid for npc movement, except monsters (world+brush only)
#define MASK_NPCSOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC			(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC_FLUID	(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
// These are things that can split areaportals
#define MASK_SPLITAREAPORTAL		(CONTENTS_WATER|CONTENTS_SLIME)

// everything that blocks corpse movement
// UNDONE: Not used yet / may be deleted
#define	MASK_DEADSOLID				(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)

#define	CS_MASK_SHOOT (MASK_SOLID|CONTENTS_DEBRIS|CONTENTS_HITBOX)

enum Collision_Group_t
{
	COLLISION_GROUP_NONE = 0,
	COLLISION_GROUP_DEBRIS,			// Collides with nothing but world and static stuff
	COLLISION_GROUP_DEBRIS_TRIGGER, // Same as debris, but hits triggers
	COLLISION_GROUP_INTERACTIVE_DEBRIS,	// Collides with everything except other interactive debris or debris
	COLLISION_GROUP_INTERACTIVE,	// Collides with everything except interactive debris or debris
	COLLISION_GROUP_PLAYER,
	COLLISION_GROUP_BREAKABLE_GLASS,
	COLLISION_GROUP_VEHICLE,
	COLLISION_GROUP_PLAYER_MOVEMENT,  // For HL2, same as Collision_Group_Player, for
	// TF2, this filters out other players and CBaseObjects
	COLLISION_GROUP_NPC,			// Generic NPC group
	COLLISION_GROUP_IN_VEHICLE,		// for any entity inside a vehicle
	COLLISION_GROUP_WEAPON,			// for any weapons that need collision detection
	COLLISION_GROUP_VEHICLE_CLIP,	// vehicle clip brush to restrict vehicle movement
	COLLISION_GROUP_PROJECTILE,		// Projectiles!
	COLLISION_GROUP_DOOR_BLOCKER,	// Blocks entities not permitted to get near moving doors
	COLLISION_GROUP_PASSABLE_DOOR,	// Doors that the player shouldn't collide with
	COLLISION_GROUP_DISSOLVING,		// Things that are dissolving are in this group
	COLLISION_GROUP_PUSHAWAY,		// Nonsolid on client and server, pushaway in player code

	COLLISION_GROUP_NPC_ACTOR,		// Used so NPCs in scripts ignore the player.
	COLLISION_GROUP_NPC_SCRIPTED,	// USed for NPCs in scripts that should not collide with each other
	COLLISION_GROUP_PZ_CLIP,

#ifdef PORTAL2
	COLLISION_GROUP_CAMERA_SOLID,		// Solid only to the camera's test trace
	COLLISION_GROUP_PLACEMENT_SOLID,	// Solid only to the placement tool's test trace
	COLLISION_GROUP_PLAYER_HELD,		// Held objects that shouldn't collide with players
	COLLISION_GROUP_WEIGHTED_CUBE,		// Cubes need a collision group that acts roughly like COLLISION_GROUP_NONE but doesn't collide with debris or interactive
#endif // PORTAL2

	COLLISION_GROUP_DEBRIS_BLOCK_PROJECTILE, // Only collides with bullets

	LAST_SHARED_COLLISION_GROUP
};

struct Ray_t
{
	VectorAligned	m_Start;		// starting point, centered within the extents
	VectorAligned	m_Delta;		// direction + length of the ray
	VectorAligned	m_StartOffset;	// Add this to m_Start to get the actual ray start
	VectorAligned	m_Extents;		// Describes an axis aligned box extruded along a ray
	const matrix3x4_t* m_pWorldAxisTransform;
	bool			m_IsRay;		// are the extents zero?
	bool			m_IsSwept;		// is delta != 0?

	Ray_t(const vec3& start, const vec3& end) :
		m_Start(start),
		m_Delta(end - start),
		m_StartOffset(),
		m_Extents(),
		m_pWorldAxisTransform(nullptr),
		m_IsRay(true),
		m_IsSwept((m_Delta.x* m_Delta.x +
			m_Delta.y * m_Delta.y +
			m_Delta.z * m_Delta.z) != 0.f)
	{
	}
};

struct cplane_t
{
	vec3	normal;
	float	dist;
	BYTE	type;			// for fast side tests
	BYTE	signbits;		// signx + (signy<<1) + (signz<<1)
	BYTE	pad[2];
};

struct csurface_t
{
	const char*		name;
	short			surfaceProps;
	unsigned short	flags;
};

enum
{
	HITGROUP_GENERIC,
	HITGROUP_HEAD,
	HITGROUP_CHEST,
	HITGROUP_STOMACH,
	HITGROUP_LEFTARM,
	HITGROUP_RIGHTARM,
	HITGROUP_LEFTLEG,
	HITGROUP_RIGHTLEG,
	HITGROUP_GEAR = 10	// alerts NPC, but doesn't do damage or bleed (1/100th damage)
};

const char* HitgroupToString(int hitgroup)
{
	switch (hitgroup)
	{
	case HITGROUP_HEAD: return "head";
	case HITGROUP_CHEST: return "chest";
	case HITGROUP_STOMACH: return "stomach";
	case HITGROUP_LEFTARM:
	case HITGROUP_RIGHTARM:
		return "arm";
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		return "leg";
	default: return "";
	}
}


class CBaseTrace
{
public:
	// these members are aligned!!
	vec3			startpos;				// start position
	vec3			endpos;					// final position
	cplane_t		plane;					// surface normal at impact

	float			fraction;				// time completed, 1.0 = didn't hit anything

	int				contents;				// contents on other side of surface hit
	unsigned short	dispFlags;				// displacement flags for marking surfaces with data

	bool			allsolid;				// if true, plane is not valid
	bool			startsolid;				// if true, the initial point was in a solid area

	CBaseTrace() {}

private:
	// No copy constructors allowed
	CBaseTrace(const CBaseTrace& vOther);
};

//-----------------------------------------------------------------------------
// Purpose: A trace is returned when a box is swept through the world
// NOTE: eventually more of this class should be moved up into the base class!!
//-----------------------------------------------------------------------------
class CGameTrace : public CBaseTrace
{
public:

	// Returns true if hEnt points at the world entity.
	// If this returns true, then you can't use GetHitBoxIndex().
	bool DidHitWorld() const { return pEnt == I::EntityList->GetClientEntity(0); }

	// Returns true if we hit something and it wasn't the world.
	bool DidHitNonWorldEntity() const { return pEnt && !DidHitWorld(); }

	// Gets the entity's network index if the trace has hit an entity.
	// If not, returns -1.
	int GetEntityIndex() const { return  pEnt ? pEnt->entindex() : -1; }

	// Returns true if there was any kind of impact at all
	bool DidHit() const { return fraction < 1 || allsolid || startsolid; }

public:

	float			fractionleftsolid;	// time we left a solid, only valid if we started in solid
	csurface_t		surface;			// surface hit (impact surface)

	int				hitgroup;			// 0 == generic, non-zero is specific body part

	short			physicsbone;		// physics bone hit by trace in studio
	unsigned short	worldSurfaceIndex;	// Index of the msurface2_t, if applicable

	C_BaseEntity*	pEnt;

	// NOTE: this member is overloaded.
	// If hEnt points at the world entity, then this is the static prop index.
	// Otherwise, this is the hitbox index.
	int				hitbox;				// box hit by trace in studio

	CGameTrace() {}

private:
	// No copy constructors allowed
	CGameTrace(const CGameTrace& vOther);
};

typedef CGameTrace trace_t;


enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};


class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(C_BaseEntity* pEntity, int contentsMask) = 0;
	virtual TraceType_t	GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	virtual TraceType_t	GetTraceType() const { return TRACE_EVERYTHING; }
};

typedef bool (*ShouldHitFunc_t)(C_BaseEntity* pHandleEntity, int contentsMask);

class CTraceFilterSimple : public CTraceFilter
{
public:
	CTraceFilterSimple(const C_BaseEntity* passentity, int collisionGroup, ShouldHitFunc_t pExtraShouldHitCheckFn = nullptr)
	{
		*reinterpret_cast<uintptr_t*>(this) = Signatures::CTraceFilterSimpleVT;
		m_pPassEnt = passentity;
		m_collisionGroup = collisionGroup;
		m_pExtraShouldHitCheckFunction = pExtraShouldHitCheckFn;
	}

	virtual bool ShouldHitEntity(C_BaseEntity* pHandleEntity, int contentsMask)
	{
		typedef bool(__thiscall* Fn)(ITraceFilter*, C_BaseEntity*, int);
		return GetVF<Fn>(this, 0)(this, pHandleEntity, contentsMask);
	}

	virtual void SetPassEntity(const C_BaseEntity* pPassEntity) { m_pPassEnt = pPassEntity; }
	virtual void SetCollisionGroup(int iCollisionGroup) { m_collisionGroup = iCollisionGroup; }

	const C_BaseEntity* GetPassEntity(void) { return m_pPassEnt; }

private:
	const C_BaseEntity* m_pPassEnt;
	int m_collisionGroup;
	ShouldHitFunc_t m_pExtraShouldHitCheckFunction;
};

class CTraceFilterSkipTwoEntities : public CTraceFilterSimple
{
public:
	CTraceFilterSkipTwoEntities(const C_BaseEntity* passentity, const C_BaseEntity* passentity2, int collisionGroup)
		: CTraceFilterSimple(passentity, collisionGroup), m_pPassEnt2(passentity2)
	{
		*reinterpret_cast<uintptr_t*>(this) = Signatures::CTraceFilterSkipTwoEntitiesVT;
	}
	//virtual bool ShouldHitEntity(C_BaseEntity* pHandleEntity, int contentsMask);
	virtual void SetPassEntity2(const C_BaseEntity* pPassEntity2) { m_pPassEnt2 = pPassEntity2; }

private:
	const C_BaseEntity* m_pPassEnt2;
};

/*
inline void UTIL_TraceLine(const vec3& vecAbsStart, const vec3& vecAbsEnd, unsigned int mask,
	const C_BaseEntity* ignore, int collisionGroup, trace_t* pTrace)
{
	Ray_t ray(vecAbsStart, vecAbsEnd);
	CTraceFilterSimple filter(ignore, collisionGroup);
	I::EngineTrace->TraceRay(ray, mask, &filter, pTrace);
}

inline void UTIL_TraceLineIgnoreTwoEntities(const vec3& vecAbsStart, const vec3& vecAbsEnd, unsigned int mask,
	const C_BaseEntity* ignore, const C_BaseEntity* ignore2, int collisionGroup, trace_t* pTrace)
{
	Ray_t ray(vecAbsStart, vecAbsEnd);
	CTraceFilterSkipTwoEntities filter(ignore, ignore2, collisionGroup);
	I::EngineTrace->TraceRay(ray, mask, &filter, pTrace);
}
*/

inline bool LineGoesThroughSmoke(const vec3& start, const vec3& end);

inline bool CanSeeEachOther(const vec3& start, const vec3& end, C_BaseEntity* entity1, C_BaseEntity* entity2, bool smokeCheck)
{
	// faster than a traceline
	if (smokeCheck && LineGoesThroughSmoke(start, end))
		return false;

	Ray_t ray(start, end);
	CTraceFilterSkipTwoEntities filter(entity1, entity2, COLLISION_GROUP_NONE);
	trace_t tr;
	// CONTENTS_MONSTER is required to check against doors, barrels, etc.
	// CONTENTS_HITBOX is required to ACCURATELY check if other player is blocking LOS
	I::EngineTrace->TraceRay(ray, (MASK_VISIBLE_AND_NPCS | CONTENTS_HITBOX), &filter, &tr);

	return !tr.DidHit();
}

//bool TraceToStudio(IPhysicsSurfaceProps* pProps, const Ray_t& ray, CStudioHdr* pStudioHdr, mstudiohitboxset_t* set,
//	matrix3x4_t** hitboxbones, int fContentsMask, const vec3& vecOrigin, float flScale, trace_t& pTrace)
//{
//	_asm
//	{
//		push	pTrace
//		push	flScale
//		push	vecOrigin
//		push	fContentsMask
//		push	hitboxbones
//		push	set
//		push	pStudioHdr
//		mov		edx, ray
//		mov		ecx, pProps
//		call	Signatures::TraceToStudio
//		add		esp, 0x1C
//	}
//}