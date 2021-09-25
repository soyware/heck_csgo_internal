#pragma once

enum
{
	OBS_MODE_NONE = 0,	// not in spectator mode
	OBS_MODE_DEATHCAM,	// special mode for death cam animation
	OBS_MODE_FREEZECAM,	// zooms to a target, and freeze-frames on them
	OBS_MODE_FIXED,		// view from a fixed camera position
	OBS_MODE_IN_EYE,	// follow a player in first person view
	OBS_MODE_CHASE,		// follow a player in third person view
	OBS_MODE_ROAMING,	// free roaming

	NUM_OBSERVER_MODES,
};

// m_lifeState values
enum : char
{
	LIFE_ALIVE,	// alive
	LIFE_DYING,	// playing death animation or still falling off of a ledge waiting to hit ground
	LIFE_DEAD,	// dead. lying still.
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY
};

constexpr float CS_PLAYER_SPEED_RUN = 260.0f;
constexpr float CS_PLAYER_SPEED_VIP = 227.0f;
constexpr float CS_PLAYER_SPEED_SHIELD = 160.0f;
constexpr float CS_PLAYER_SPEED_HAS_HOSTAGE = 200.0f;
constexpr float CS_PLAYER_SPEED_STOPPED = 1.0f;
constexpr float CS_PLAYER_SPEED_OBSERVER = 900.0f;

constexpr float CS_PLAYER_SPEED_DUCK_MODIFIER = 0.34f;
constexpr float CS_PLAYER_SPEED_WALK_MODIFIER = 0.52f;
constexpr float CS_PLAYER_SPEED_CLIMB_MODIFIER = 0.34f;
constexpr float CS_PLAYER_HEAVYARMOR_FLINCH_MODIFIER = 0.5f;

constexpr float CS_PLAYER_DUCK_SPEED_IDEAL = 8.0f;

class CCSGOPlayerAnimState;

class C_CSPlayer : public C_BaseAnimating
{
public:
	inline char				GetLifestate() { return *reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_lifeState); }
	inline bool				IsAlive() { return GetLifestate() == LIFE_ALIVE; }
	inline vec3				GetViewPunch() { return *reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_viewPunchAngle); }
	inline vec3				GetAimPunch() { return *reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_aimPunchAngle); }
	inline int				GetTickBase() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nTickBase); }
	inline C_WeaponCSBaseGun* GetActiveWeapon() { return static_cast<C_WeaponCSBaseGun*>(I::EntityList->GetClientEntityFromHandle(
		*reinterpret_cast<ULONG*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_hActiveWeapon))); }
	inline float			GetNextAttack() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flNextAttack); }
	inline ULONG*			GetWeapons() { return reinterpret_cast<ULONG*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_hMyWeapons); }
	inline int				GetShotsFired() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iShotsFired); }
	inline bool				IsScoped() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bIsScoped); }
	inline bool				IsDefusing() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bIsDefusing); }
	inline bool				IsGrabbingHostage() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bIsGrabbingHostage); }
	inline bool				IsRescuing() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bIsRescuing); }
	inline bool				IsWaitingForNoAttack() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bWaitForNoAttack); }
	inline C_BaseViewModel*	GetViewmodel() { return static_cast<C_BaseViewModel*>(I::EntityList->GetClientEntityFromHandle(
		*reinterpret_cast<ULONG*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_hViewModel))); }
	inline float			GetFlashAlpha() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flFlashOverlayAlpha); }
	inline bool				IsBlinded() { return (120.f < GetFlashAlpha()); }
	inline C_BaseEntity*	GetObserverTarget() { return I::EntityList->GetClientEntityFromHandle(
		*reinterpret_cast<ULONG*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_hObserverTarget)); }
	inline bool				IsGunGameImmune() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bGunGameImmunity); }
	inline vec3				GetEyeAngles() { return *reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_angEyeAngles); }
	inline int				GetObserverMode() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iObserverMode); };
	inline bool				HasDefuser() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bHasDefuser); };
	inline float			GetLowerBodyYawTarget() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flLowerBodyYawTarget); };
	inline CCSGOPlayerAnimState* GetAnimState() { return *reinterpret_cast<CCSGOPlayerAnimState**>(reinterpret_cast<uintptr_t>(this) + NetVars::m_AnimState); };
	inline C_BaseEntity*	GetGroundEntity() { return I::EntityList->GetClientEntityFromHandle(
		*reinterpret_cast<ULONG*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_hGroundEntity)); }

	inline void				SetViewPunch(const vec3& angle) { *reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_viewPunchAngle) = angle; }
	inline void				SetAimPunch(const vec3& angle) { *reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_aimPunchAngle) = angle; }
	inline void				SetMaxFlashAlpha(float value) { *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flFlashMaxAlpha) = value; }
	inline void				SetVAngle(const vec3& v) { *reinterpret_cast<vec3*>(reinterpret_cast<uintptr_t>(this) + NetVars::v_angle) = v; };
	inline void				SetObserverMode(int mode) { *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iObserverMode) = mode; };

	inline const vec3& Weapon_ShootPosition(vec3& outPos)
	{
		typedef const vec3& (__thiscall* Fn) (void*, vec3&);
		return GetVF<Fn>(this, 285)(this, outPos);
	}

	bool IsShooting(CUserCmd* cmd)
	{
		C_WeaponCSBaseGun* weapon = GetActiveWeapon();
		if (!weapon)
			return false;

		const CCSWeaponInfo& info = weapon->GetCSWpnData();

		if (info.m_WeaponType == WEAPONTYPE_GRENADE)
		{
			float throwTime = static_cast<C_BaseCSGrenade*>(weapon)->GetThrowTime();
			return (0.f < throwTime && throwTime <= G::Globals->curtime);
		}

		bool primaryAttack = (cmd->buttons & IN_ATTACK);

		if (!primaryAttack && !(cmd->buttons & IN_ATTACK2) && (1 > weapon->GetBurstShotsRemaining()))
			return false;

		return weapon->CanAttack(primaryAttack);
	}
};

bool C_WeaponCSBaseGun::CanAttack(bool primaryAttack)
{
	if ((*G::pGameRules) && (*G::pGameRules)->IsFreezePeriod())
		return false;

	bool isRevolver = (GetItemDefinitionIndex() == WEAPON_REVOLVER);
	bool isMelee = IsMelee();
	bool burstRemaining = (0 < GetBurstShotsRemaining());

	if (burstRemaining)
	{
		if (G::Globals->curtime < GetNextBurstShot())
			return false;
	}
	else if (primaryAttack)
	{
		if (G::Globals->curtime < GetNextPrimaryAttack())
			return false;

		if (isRevolver && !IsPostponeFireReady())
			return false;
	}
	else
	{
		if (G::Globals->curtime < GetNextSecondaryAttack())
			return false;

		if (!isRevolver && !isMelee)
			return false;
	}

	if (!isMelee && GetClip1() < 1)
		return false;


	C_CSPlayer* owner = static_cast<C_CSPlayer*>(GetOwnerEntity());
	if (!owner)
		return false;

	if (G::Globals->curtime < owner->GetNextAttack())
		return false;

	// check if not full auto and we didn't release the button after the shot
	if ((0 < owner->GetShotsFired()) && !burstRemaining && 
		(!IsFullAuto() || (isRevolver && !primaryAttack))) // revolver is full auto primary attack only
		return false;

	// commented out so we can use trigger which stops defusing
	//if (owner->IsDefusing())
	//	return false;

	if (owner->IsWaitingForNoAttack())
		return false;

	return true;
}