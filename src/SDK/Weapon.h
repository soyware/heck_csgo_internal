#pragma once

enum ItemDefinitionIndex_t : short
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_ZONE_REPULSOR,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFE_GG,
	WEAPON_KNIFE_CT,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	ITEM_DEFUSER = 55,
	ITEM_CUTTERS,
	WEAPON_HEALTHSHOT,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_CORD,
	WEAPON_KNIFE_CANIS,
	WEAPON_KNIFE_URSUS,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_OUTDOOR,
	WEAPON_KNIFE_STILETTO,
	WEAPON_KNIFE_WIDOWMAKER,
	WEAPON_KNIFE_SKELETON = 525,
};

enum CSWeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_GRENADE = 9,
	WEAPONTYPE_STACKABLEITEM = 11,
	WEAPONTYPE_FISTS,
	WEAPONTYPE_BREACHCHARGE,
	WEAPONTYPE_BUMPMINE,
	WEAPONTYPE_TABLET,
	WEAPONTYPE_MELEE,
	WEAPONTYPE_SHIELD = 18,
	WEAPONTYPE_ZONE_REPULSOR
};

class CCSWeaponInfo
{
private:
	std::byte pad_0x0000[0x4]; //0x0000
public:
	char* m_pszWeaponName; //0x0004 
private:
	std::byte pad_0x0008[0xC]; //0x0008
public:
	int32_t m_iMaxClip1; //0x0014 
	int32_t m_iMaxClip2; //0x0018 
	int32_t m_iDefaultClip1; //0x001C 
	int32_t m_iDefaultClip2; //0x0020 
	int32_t m_iReserveAmmo1; //0x0024 
	int32_t m_iReserveAmmo2; //0x0028 
	char* m_pszWorldModel; //0x002C 
	char* m_pszViewModel; //0x0030 
	char* m_pszWorldDroppedModel; //0x0034 
	char* m_apShootSounds; //0x0038 
private:
	std::byte pad_0x003C[0x44]; //0x003C
public:
	char* m_szAmmo1; //0x0080 
	char* m_szAmmo2; //0x0084 
	char* m_pszPrintName; //0x0088 
	char* m_pszClassName; //0x008C 
	unsigned char m_bBuiltRightHanded; //0x0090 
	unsigned char m_bAllowFlipping; //0x0091 
	unsigned char m_bMeleeWeapon; //0x0092 
private:
	std::byte pad_0x0093[0x5]; //0x0093
public:
	int32_t m_iFlags; //0x0098 
	int32_t m_iWeight; //0x009C 
	int32_t m_iAmmoType; //0x00A0 
	int32_t m_iSlot; //0x00A4 
	int32_t m_iPosition; //0x00A8 
private:
	std::byte pad_0x00AC[0x14]; //0x00AC
public:
	int32_t m_iRumbleEffect; //0x00C0 
private:
	std::byte pad_0x00C4[0x4]; //0x00C4
public:
	int32_t m_WeaponType; //0x00C8 
private:
	std::byte pad_0x00CC[0x4]; //0x00CC
public:
	int32_t m_iWeaponPrice; //0x00D0 
	int32_t m_iKillAward; //0x00D4 
	char* m_pszAnimationPrefix; //0x00D8 
	float m_flCycleTime; //0x00DC 
	float m_flCycleTimeAlt; //0x00E0 
	float m_flTimeToIdle; //0x00E4 
	float m_flIdleInterval; //0x00E8 
	unsigned char m_bFullAuto; //0x00EC 
private:
	std::byte pad_0x00ED[0x3]; //0x00ED
public:
	int32_t m_iDamage; //0x00F0 
	float m_flArmorRatio; //0x00F4 
	int32_t m_iBullets; //0x00F8 
	float m_flPenetration; //0x00FC 
	float m_flFlinchVelocityModifierLarge; //0x0100 
	float m_flFlinchVelocityModifierSmall; //0x0104 
	float m_flHeadshotMultiplier; //0x0108 
	float m_flRange; //0x010C 
	float m_flRangeModifier; //0x0110 
	float m_flThrowVelocity; //0x0114 
private:
	std::byte pad_0x0118[0xC]; //0x0118
public:
	unsigned char m_bHasSilencer; //0x0124 
private:
	std::byte pad_0x0125[0x3]; //0x0125
public:
	char* m_pszSilencerModel; //0x0128 
	int32_t m_iCrosshairMinDistance; //0x012C 
	int32_t m_iCrosshairDeltaDistance; //0x0130 
	float m_flMaxPlayerSpeed; //0x0134 
	float m_flMaxPlayerSpeedAlt; //0x0138 
	float m_flAttackMovespeedFactor; //0x013C 
	float m_flSpread; //0x0140 
	float m_flSpreadAlt; //0x0144 
	float m_flInaccuracyCrouch; //0x0148 
	float m_flInaccuracyCrouchAlt; //0x014C 
	float m_flInaccuracyStand; //0x0150 
	float m_flInaccuracyStandAlt; //0x0154 
	float m_flInaccuracyJumpStart; //0x0158 
	float m_flInaccuracyJumpApex; //0x015C 
	float m_flInaccuracyJump; //0x0160 
	float m_flInaccuracyJumpAlt; //0x0164 
	float m_flInaccuracyLand; //0x0168 
	float m_flInaccuracyLandAlt; //0x016C 
	float m_flInaccuracyLadder; //0x0170 
	float m_flInaccuracyLadderAlt; //0x0174 
	float m_flInaccuracyFire; //0x0178 
	float m_flInaccuracyFireAlt; //0x017C 
	float m_flInaccuracyMove; //0x0180 
	float m_flInaccuracyMoveAlt; //0x0184 
	float m_flInaccuracyReload; //0x0188 
	int32_t m_iRecoilSeed; //0x018C 
	float m_flRecoilAngle; //0x0190 
	float m_flRecoilAngleAlt; //0x0194 
	float m_flRecoilAngleVariance; //0x0198 
	float m_flRecoilAngleVarianceAlt; //0x019C 
	float m_flRecoilMagnitude; //0x01A0 
	float m_flRecoilMagnitudeAlt; //0x01A4 
	float m_flRecoilMagnitudeVariance; //0x01A8 
	float m_flRecoilMagnitudeVarianceAlt; //0x01AC 
	int32_t m_iSpreadSeed; //0x01B0 
	float m_flRecoilTimeCrouch; //0x01B4 
	float m_flRecoilTimeStand; //0x01B8 
	float m_flRecoilTimeCrouchFinal; //0x01BC 
	float m_flRecoilTimeStandFinal; //0x01C0 
	int32_t m_iRecoveryTransitionStartBullet; //0x01C4 
	int32_t m_iRecoveryTransitionEndBullet; //0x01C8 
	unsigned char m_bUnzoomAfterShot; //0x01CC 
	unsigned char m_bHideViewModelZoomed; //0x01CD 
private:
	std::byte pad_0x01CE[0x2]; //0x01CE
public:
	int32_t m_iZoomLevels; //0x01D0 
	int32_t m_iZoomFOV1; //0x01D4 
	int32_t m_iZoomFOV2; //0x01D8 
	float m_fZoomTime0; //0x01DC 
	float m_fZoomTime1; //0x01E0 
	float m_fZoomTime2; //0x01E4 
	char* m_pszWeaponClass; //0x01E8 
	char* m_szAddonLocation; //0x01EC 
	float m_flAddonScale; //0x01F0 
	char* m_pszEjectBrassEffect; //0x01F4 
	char* m_pszTracerEffect; //0x01F8 
	int32_t m_iTracerFrequency; //0x01FC 
	int32_t m_iTracerFrequencyAlt; //0x0200 
	char* m_pszMuzzleFlashEffect_1stPerson; //0x0204 
	char* m_pszMuzzleFlashEffectAlt_1stPerson; //0x0208 
	char* m_pszMuzzleFlashEffect_3rdPerson; //0x020C 
	char* m_pszMuzzleFlashEffectAlt_3rdPerson; //0x0210 
	char* m_pszHeatEffect; //0x0214 
	float m_flHeatPerShot; //0x0218 
	char* m_pszZoomInSound; //0x021C 
	char* m_pszZoomOutSound; //0x0220 
	float m_flInaccuracyPitchShift; //0x0224 
	float m_flInaccuracySoundThreshold; //0x0228 
	float m_flBotAudibleRange; //0x022C 
private:
	std::byte pad_0x0230[0x4]; //0x0230
public:
	int32_t m_iTeam; //0x0234 
	char* m_pszWrongTeamMsg; //0x0238 
	unsigned char m_bHasBurstMode; //0x023C 
	unsigned char m_bIsRevolver; //0x023D 
	int8_t m_bCannotShootUnderwater; //0x023E 
private:
	std::byte pad_0x023F[0x9]; //0x023F

}; //Size=0x0248

inline const char* GetWeaponName(int index)
{
	switch (index)
	{
	default: return "";
	case WEAPON_DEAGLE: return "weapon_deagle";
	case WEAPON_ELITE: return "weapon_elite";
	case WEAPON_FIVESEVEN: return "weapon_fiveseven";
	case WEAPON_GLOCK: return "weapon_glock";
	case WEAPON_AK47: return "weapon_ak47";
	case WEAPON_AUG: return "weapon_aug";
	case WEAPON_AWP: return "weapon_awp";
	case WEAPON_FAMAS: return "weapon_famas";
	case WEAPON_G3SG1: return "weapon_g3sg1";
	case WEAPON_GALILAR: return "weapon_galilar";
	case WEAPON_M249: return "weapon_m249";
	case WEAPON_M4A1: return "weapon_m4a1";
	case WEAPON_MAC10: return "weapon_mac10";
	case WEAPON_P90: return "weapon_p90";
	case WEAPON_MP5SD: return "weapon_mp5sd";
	case WEAPON_UMP45: return "weapon_ump45";
	case WEAPON_XM1014: return "weapon_xm1014";
	case WEAPON_BIZON: return "weapon_bizon";
	case WEAPON_MAG7: return "weapon_mag7";
	case WEAPON_NEGEV: return "weapon_negev";
	case WEAPON_SAWEDOFF: return "weapon_sawedoff";
	case WEAPON_TEC9: return "weapon_tec9";
	case WEAPON_TASER: return "weapon_taser";
	case WEAPON_HKP2000: return "weapon_hkp2000";
	case WEAPON_MP7: return "weapon_mp7";
	case WEAPON_MP9: return "weapon_mp9";
	case WEAPON_NOVA: return "weapon_nova";
	case WEAPON_P250: return "weapon_p250";
	case WEAPON_SHIELD: return "weapon_shield";
	case WEAPON_SCAR20: return "weapon_scar20";
	case WEAPON_SG556: return "weapon_sg556";
	case WEAPON_SSG08: return "weapon_ssg08";
	case WEAPON_KNIFE_GG:
	case WEAPON_KNIFE_CT:
	case WEAPON_KNIFE_T:
	case WEAPON_KNIFE_GHOST:
	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_CSS:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_PUSH:
	case WEAPON_KNIFE_CORD:
	case WEAPON_KNIFE_CANIS:
	case WEAPON_KNIFE_URSUS:
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
	case WEAPON_KNIFE_OUTDOOR:
	case WEAPON_KNIFE_STILETTO:
	case WEAPON_KNIFE_WIDOWMAKER:
	case WEAPON_KNIFE_SKELETON:
	case WEAPON_FISTS:
	case WEAPON_TABLET:
	case WEAPON_AXE:
	case WEAPON_HAMMER:
	case WEAPON_SPANNER:
	case WEAPON_SNOWBALL:
	case WEAPON_BUMPMINE:
	case WEAPON_ZONE_REPULSOR:
		return "weapon_knife";
	case WEAPON_FLASHBANG: return "weapon_flashbang";
	case WEAPON_HEGRENADE:
	case WEAPON_FRAG_GRENADE:
		return "weapon_hegrenade";
	case WEAPON_SMOKEGRENADE: return "weapon_smokegrenade";
	case WEAPON_MOLOTOV:
	case WEAPON_INCGRENADE:
	case WEAPON_FIREBOMB:
		return "weapon_molotov";
	case WEAPON_DECOY:
	case WEAPON_DIVERSION:
		return "weapon_decoy";
	case WEAPON_C4: return "weapon_c4";
	case WEAPON_HEALTHSHOT: return "weapon_healthshot";
	case WEAPON_M4A1_SILENCER: return "weapon_m4a1_silencer";
	case WEAPON_USP_SILENCER: return "weapon_usp_silencer";
	case WEAPON_CZ75A: return "weapon_cz75a";
	case WEAPON_REVOLVER: return "weapon_revolver";
	case WEAPON_TAGRENADE: return "weapon_tagrenade";
	case WEAPON_BREACHCHARGE: return "weapon_breachcharge";
	}
}

inline const wchar_t* GetPrintName(int index)
{
	switch (index)
	{
	default: return L"";
	case WEAPON_DEAGLE: return L"Desert Eagle";
	case WEAPON_ELITE: return L"Dual Berettas";
	case WEAPON_FIVESEVEN: return L"Five-SeveN";
	case WEAPON_GLOCK: return L"Glock-18";
	case WEAPON_AK47: return L"AK-47";
	case WEAPON_AUG: return L"AUG";
	case WEAPON_AWP: return L"AWP";
	case WEAPON_FAMAS: return L"FAMAS";
	case WEAPON_G3SG1: return L"G3SG1";
	case WEAPON_GALILAR: return L"Galil AR";
	case WEAPON_M249: return L"M249";
	case WEAPON_M4A1: return L"M4A4";
	case WEAPON_MAC10: return L"MAC-10";
	case WEAPON_P90: return L"P90";
	case WEAPON_MP5SD: return L"MP5-SD";
	case WEAPON_UMP45: return L"UMP-45";
	case WEAPON_XM1014: return L"XM1014";
	case WEAPON_BIZON: return L"PP-Bizon";
	case WEAPON_MAG7: return L"MAG-7";
	case WEAPON_NEGEV: return L"Negev";
	case WEAPON_SAWEDOFF: return L"Sawed-Off";
	case WEAPON_TEC9: return L"Tec-9";
	case WEAPON_TASER: return L"Zeus";
	case WEAPON_HKP2000: return L"P2000";
	case WEAPON_MP7: return L"MP7";
	case WEAPON_MP9: return L"MP9";
	case WEAPON_NOVA: return L"Nova";
	case WEAPON_P250: return L"P250";
	case WEAPON_SHIELD: return L"Shield";
	case WEAPON_SCAR20: return L"SCAR-20";
	case WEAPON_SG556: return L"SG 553";
	case WEAPON_SSG08: return L"SSG 08";
	case WEAPON_KNIFE_GG:
	case WEAPON_KNIFE_CT:
	case WEAPON_KNIFE_T:
	case WEAPON_KNIFE_GHOST:
	case WEAPON_KNIFE_BAYONET:
	case WEAPON_KNIFE_CSS:
	case WEAPON_KNIFE_FLIP:
	case WEAPON_KNIFE_GUT:
	case WEAPON_KNIFE_KARAMBIT:
	case WEAPON_KNIFE_M9_BAYONET:
	case WEAPON_KNIFE_TACTICAL:
	case WEAPON_KNIFE_FALCHION:
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
	case WEAPON_KNIFE_BUTTERFLY:
	case WEAPON_KNIFE_PUSH:
	case WEAPON_KNIFE_CORD:
	case WEAPON_KNIFE_CANIS:
	case WEAPON_KNIFE_URSUS:
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
	case WEAPON_KNIFE_OUTDOOR:
	case WEAPON_KNIFE_STILETTO:
	case WEAPON_KNIFE_WIDOWMAKER:
	case WEAPON_KNIFE_SKELETON:
	case WEAPON_FISTS:
	case WEAPON_TABLET:
	case WEAPON_AXE:
	case WEAPON_HAMMER:
	case WEAPON_SPANNER:
	case WEAPON_SNOWBALL:
	case WEAPON_BUMPMINE:
	case WEAPON_ZONE_REPULSOR:
		return L"Melee";
	case WEAPON_FLASHBANG: return L"Flashbang";
	case WEAPON_HEGRENADE:
	case WEAPON_FRAG_GRENADE:
		return L"Grenade";
	case WEAPON_SMOKEGRENADE: return L"Smoke";
	case WEAPON_MOLOTOV:
	case WEAPON_INCGRENADE:
	case WEAPON_FIREBOMB:
		return L"Molotov";
	case WEAPON_DECOY:
	case WEAPON_DIVERSION:
		return L"Decoy";
	case WEAPON_C4: return L"C4";
	case WEAPON_HEALTHSHOT: return L"Medi-Shot";
	case WEAPON_M4A1_SILENCER: return L"M4A1-S";
	case WEAPON_USP_SILENCER: return L"USP-S";
	case WEAPON_CZ75A: return L"CZ75-A";
	case WEAPON_REVOLVER: return L"Revolver";
	case WEAPON_TAGRENADE: return L"TAG";
	case WEAPON_BREACHCHARGE: return L"Charge";
	}
}

class C_WeaponCSBaseGun : public C_BaseAnimating
{
public:
	inline int		GetActivity() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_Activity); }
	inline short	GetItemDefinitionIndex() { return *reinterpret_cast<short*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iItemDefinitionIndex); }
	inline int		GetClip1() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iClip1); }
	inline int		GetPrimaryReserveAmmoCount() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iPrimaryReserveAmmoCount); }
	inline float	GetNextPrimaryAttack() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flNextPrimaryAttack); }
	inline float	GetNextSecondaryAttack() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flNextSecondaryAttack); }
	inline uint64_t	GetOriginalOwnerXuid() { return *reinterpret_cast<uint64_t*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_OriginalOwnerXuidLow); }
	inline C_BaseAnimating* GetWorldModel() { return static_cast<C_BaseAnimating*>(I::EntityList->GetClientEntityFromHandle(
		*reinterpret_cast<ULONG*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_hWeaponWorldModel))); }
	inline float	GetRecoilIndex() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flRecoilIndex); }

	inline bool		IsInBurstMode() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bBurstMode); }
	inline float	GetPostponeFireReadyTime() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flPostponeFireReadyTime); }

	inline bool		IsPostponeFireReady()
	{
		float time = GetPostponeFireReadyTime();
		if (time == FLT_MAX)
			return false;

		int left = TIME_TO_TICKS(time - G::Globals->curtime);
		return (left == 0);
	}

	inline int		GetWeaponMode() { return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_weaponMode); }

	// CWeaponCSBaseGun
	inline int		GetBurstShotsRemaining() { return HasBurst() ? *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iBurstShotsRemaining) : 0; }
	inline float	GetNextBurstShot() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_fNextBurstShot); }

	inline void		SetItemIDHigh(int id) { *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iItemIDHigh) = id; }
	inline void		SetAccountID(int id) { *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iAccountID) = id; }
	inline void		SetEntityQuality(int quality) { *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iEntityQuality) = quality; }
	inline void		SetItemDefinitionIndex(short index) { *reinterpret_cast<short*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iItemDefinitionIndex) = index; }
	inline void		SetFallbackPaintKit(int kit) { *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nFallbackPaintKit) = kit; }
	inline void		SetFallbackWear(float wear) { *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_flFallbackWear) = wear; }
	inline void		SetFallbackSeed(int seed) { *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nFallbackSeed) = seed; }
	inline void		SetFallbackStatTrak(int stattrak) { *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_nFallbackStatTrak) = stattrak; }
	inline void		SetModelIndexView(int model) { *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iViewModelIndex) = model; }
	inline void		SetModelIndexWorld(int model) { *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iWorldModelIndex) = model; }
	inline void		SetModelIndexWorldDropped(int model) { *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_iWorldDroppedModelIndex) = model; }

	// "Weapon '%s' script file not found, but its data was accessed. This error is fatal.\n"
	inline const CCSWeaponInfo& GetCSWpnData()
	{
		typedef const CCSWeaponInfo& (__thiscall* Fn) (C_WeaponCSBaseGun*);
		return GetVF<Fn>(this, 461)(this);
	}

	inline int GetZoomLevel()
	{
		typedef int (__thiscall* Fn) (C_WeaponCSBaseGun*);
		return GetVF<Fn>(this, 462)(this);
	}

	inline float GetInaccuracy()
	{
		typedef float (__thiscall* Fn) (C_WeaponCSBaseGun*);
		return GetVF<Fn>(this, 483)(this);
	}

	inline void UpdateAccuracyPenalty()
	{
		typedef void (__thiscall* Fn) (C_WeaponCSBaseGun*);
		GetVF<Fn>(this, 484)(this);
	}

	inline float	GetSpread()		{ return GetWeaponMode() ? GetCSWpnData().m_flSpreadAlt : GetCSWpnData().m_flSpread; }

	inline float	GetMaxSpeed()	{ return GetWeaponMode() ? GetCSWpnData().m_flMaxPlayerSpeedAlt : GetCSWpnData().m_flMaxPlayerSpeed; }

	inline bool		IsMelee()		{ return GetCSWpnData().m_bMeleeWeapon; }

	inline bool		IsFullAuto()	{ return GetCSWpnData().m_bFullAuto ? !IsInBurstMode() : false; }

	inline bool		HasBurst()		{ return GetCSWpnData().m_bHasBurstMode; }

	bool CanAttack(bool primaryAttack);
};

class C_BaseCSGrenade : public C_WeaponCSBaseGun
{
public:
	inline float	GetThrowTime() { return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_fThrowTime); }
	inline bool		IsPinPulled() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bPinPulled); }
};

class C_C4 : public C_WeaponCSBaseGun
{
public:
	inline bool		IsBeingPlanted() { return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + NetVars::m_bStartedArming); }
};