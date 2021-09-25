#pragma once

#ifdef GetProp
#undef GetProp
#endif

namespace NetVars
{
	int	m_iTeamNum,
		m_hOwnerEntity,
		m_iHealth,
		m_fFlags,
		m_vecViewOffset,
		m_vecVelocity,
		m_angRotation,
		m_vecOrigin,
		m_MoveType,
		m_flAnimTime,
		m_flSimulationTime,
		m_bSpotted,
		m_flLastMessageTime,
		m_Collision,
		m_rgflCoordinateFrame,
		m_nSequence,
		m_flCycle,
		m_occlusionFlags,
		m_lastOcclusionCheck,
		m_nPrevSequence,
		m_nNewSequenceParity,
		m_nHitboxSet,
		m_flModelScale,
		m_pStudioHdr,
		m_flLastBoneSetupTime,
		m_CachedBoneData,
		m_iMostRecentModelBoneCounter,
		m_bCanUseFastPath,
		m_AnimOverlay,

		m_flNextAttack,
		m_hMyWeapons,
		m_hActiveWeapon,
		m_hGroundEntity,
		m_lifeState,
		m_viewPunchAngle,
		m_aimPunchAngle,
		m_nTickBase,
		v_angle,
		m_iObserverMode,
		m_hObserverTarget,
		m_hViewModel,
		m_bHasDefuser,
		m_AnimState,
		m_bIsScoped,
		m_bIsDefusing,
		m_bIsGrabbingHostage,
		m_bIsRescuing,
		m_bWaitForNoAttack,
		m_angEyeAngles,
		m_flLowerBodyYawTarget,
		m_bGunGameImmunity,
		m_iShotsFired,
		m_flFlashOverlayAlpha,
		m_flFlashMaxAlpha,

		m_iItemDefinitionIndex,
		m_iEntityQuality,
		m_iItemIDHigh,
		m_iAccountID,
		m_OriginalOwnerXuidLow,
		m_nFallbackPaintKit,
		m_nFallbackSeed,
		m_flFallbackWear,
		m_nFallbackStatTrak,
		m_Activity,
		m_iViewModelIndex,
		m_iWorldModelIndex,
		m_iWorldDroppedModelIndex,
		m_hWeaponWorldModel,
		m_flNextPrimaryAttack,
		m_flNextSecondaryAttack,
		m_iClip1,
		m_iPrimaryReserveAmmoCount,
		m_weaponMode,
		m_flRecoilIndex,
		m_bBurstMode,
		m_flPostponeFireReadyTime,
		m_iBurstShotsRemaining,
		m_fNextBurstShot,

		m_bBombTicking,
		m_flC4Blow,
		m_flDefuseCountDown,
		m_hBombDefuser,
		//m_WritableBones,

		m_flDamage,
		m_nExplodeEffectTickBegin,
		m_decoyParticleInitTime,
		m_nSmokeEffectTickBegin,
		m_drawable,
		m_drawableCount,
		m_nFireEffectTickBegin,

		m_bPinPulled,
		m_fThrowTime,

		m_bStartedArming,

		m_hWeapon,
		m_hOwner,
		
		m_iKills,
		m_iAssists,
		m_iDeaths,
		m_bConnected,
		m_iTeam,
		m_iPlayerC4,
		m_iMVPs,
		m_iScore,
		m_iCompetitiveWins,
		m_iCompTeammateColor,
		m_iBotDifficulty,
		m_nPersonaDataPublicLevel,
		m_nPersonaDataPublicCommendsLeader,
		m_nPersonaDataPublicCommendsTeacher,
		m_nPersonaDataPublicCommendsFriendly,
		m_iMatchStats_Damage_Total,
		
		m_bFreezePeriod,
		m_bWarmupPeriod,
		m_iRoundTime,
		m_bIsQueuedMatchmaking,
		m_nQueuedMatchmakingMode;

	RecvTable* GetTable(const char* Name)
	{
		for (ClientClass* CClient = I::Client->GetAllClasses(); CClient != nullptr; CClient = CClient->m_pNext)
			if (!strcmp(CClient->m_pRecvTable->m_pNetTableName, Name))
				return CClient->m_pRecvTable;

		return nullptr;
	}

	RecvProp* GetProp(RecvTable* Table, const char* Name)
	{
		for (int i = 0; i < Table->m_nProps; ++i)
		{
			RecvProp* Prop = &Table->m_pProps[i];
			if (!strcmp(Prop->m_pVarName, Name))
				return Prop;
			else if (Prop->m_RecvType == DPT_DataTable)
			{
				RecvProp* deeper = GetProp(Prop->m_pDataTable, Name);
				if (deeper)
					return deeper;
			}
		}
		return nullptr;
	}

	int GetOffset(RecvTable* Table, const char* VarName)
	{
		for (int i = 0; i < Table->m_nProps; ++i)
		{
			RecvProp* Prop = &Table->m_pProps[i];
			if (!strcmp(Prop->m_pVarName, VarName))
				return Prop->m_Offset;
			else if (Prop->m_RecvType == DPT_DataTable)
			{
				int deeper = GetOffset(Prop->m_pDataTable, VarName);
				if (deeper)
					return Prop->m_Offset + deeper;
			}
		}
		return 0;
	}

	int GetOffset(const char* TableName, const char* VarName)
	{
		RecvTable* Table;
		int Offset;
		if (!(Table = GetTable(TableName)) ||
			!(Offset = GetOffset(Table, VarName)))
		{
			FatalError("Offset not found");
			return 0;
		}
		return Offset;
	}

	void Init()
	{
		// Entity
		m_iTeamNum				= GetOffset("DT_BaseEntity", "m_iTeamNum");
		m_hOwnerEntity			= GetOffset("DT_BaseEntity", "m_hOwnerEntity");
		m_iHealth				= GetOffset("DT_BasePlayer", "m_iHealth");
		m_fFlags				= GetOffset("DT_BasePlayer", "m_fFlags");
		m_vecViewOffset			= GetOffset("DT_BasePlayer", "m_vecViewOffset[0]");
		m_vecVelocity			= GetOffset("DT_BasePlayer", "m_vecVelocity[0]");
		m_angRotation			= GetOffset("DT_BaseEntity", "m_angRotation");
		m_vecOrigin				= GetOffset("DT_BaseEntity", "m_vecOrigin");
		m_MoveType				= GetOffset("DT_BaseEntity", "m_nRenderMode") + 0x1;
		m_flAnimTime			= GetOffset("DT_BaseEntity", "m_flAnimTime");
		m_flSimulationTime		= GetOffset("DT_BaseEntity", "m_flSimulationTime");
		m_bSpotted				= GetOffset("DT_BaseEntity", "m_bSpotted");
		m_flLastMessageTime		= GetOffset("DT_BaseEntity", "m_fadeMinDist") - 0x18;
		m_Collision				= GetOffset("DT_BaseEntity", "m_Collision");
		m_rgflCoordinateFrame	= GetOffset("DT_BaseEntity", "m_CollisionGroup") - sizeof(matrix3x4_t);
		m_nSequence				= GetOffset("DT_BaseAnimating", "m_nSequence");
		m_flCycle				= GetOffset("DT_BaseAnimating", "m_flCycle");
		m_lastOcclusionCheck	= GetOffset("DT_BaseAnimating", "m_nHighlightColorR") - 0x8;
		m_occlusionFlags = m_lastOcclusionCheck - 0x8;
		m_nNewSequenceParity	= GetOffset("DT_BaseAnimating", "m_nNewSequenceParity");
		m_nHitboxSet			= GetOffset("DT_BaseAnimating", "m_nHitboxSet");
		m_flModelScale			= GetOffset("DT_BaseAnimating", "m_flModelScale");
		m_pStudioHdr			= GetOffset("DT_BaseAnimating", "m_bSuppressAnimSounds") + 0x2;
		m_flLastBoneSetupTime	= GetOffset("DT_BaseAnimating", "m_hLightingOrigin") - 0x20;
		m_CachedBoneData = m_flLastBoneSetupTime - 0x14;
		m_nPrevSequence = m_CachedBoneData - 0x8;
		m_iMostRecentModelBoneCounter = GetOffset("DT_BaseAnimating", "m_nForceBone") + 0x4;
		m_bCanUseFastPath		= GetOffset("DT_BaseAnimating", "m_flFrozen") + 0x4;
		m_AnimOverlay			= GetOffset("DT_BaseAnimating", "m_bSuppressAnimSounds") + 0x42;

		// Player
		m_flNextAttack			= GetOffset("DT_BaseCombatCharacter", "m_flNextAttack");
		m_hMyWeapons			= GetOffset("DT_BaseCombatCharacter", "m_hMyWeapons");
		m_hActiveWeapon			= GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
		m_hGroundEntity			= GetOffset("DT_BasePlayer", "m_hGroundEntity");
		m_lifeState				= GetOffset("DT_BasePlayer", "m_lifeState");
		m_viewPunchAngle		= GetOffset("DT_BasePlayer", "m_viewPunchAngle");
		m_aimPunchAngle			= GetOffset("DT_BasePlayer", "m_aimPunchAngle");
		m_nTickBase				= GetOffset("DT_BasePlayer", "m_nTickBase");
		v_angle					= GetOffset("DT_BasePlayer", "deadflag") + 0x4;
		m_iObserverMode			= GetOffset("DT_BasePlayer", "m_iObserverMode");
		m_hObserverTarget		= GetOffset("DT_BasePlayer", "m_hObserverTarget");
		m_hViewModel			= GetOffset("DT_BasePlayer", "m_hViewModel[0]");
		m_bHasDefuser			= GetOffset("DT_CSPlayer", "m_bHasDefuser");
		m_bIsScoped				= GetOffset("DT_CSPlayer", "m_bIsScoped");
		m_AnimState = m_bIsScoped - 0x14;
		m_bIsDefusing			= GetOffset("DT_CSPlayer", "m_bIsDefusing");
		m_bIsGrabbingHostage	= GetOffset("DT_CSPlayer", "m_bIsGrabbingHostage");
		m_bIsRescuing			= GetOffset("DT_CSPlayer", "m_bIsRescuing");
		m_bWaitForNoAttack		= GetOffset("DT_CSPlayer", "m_bWaitForNoAttack");
		m_angEyeAngles			= GetOffset("DT_CSPlayer", "m_angEyeAngles");
		m_flLowerBodyYawTarget	= GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		m_bGunGameImmunity		= GetOffset("DT_CSPlayer", "m_bGunGameImmunity");
		m_iShotsFired			= GetOffset("DT_CSPlayer", "m_iShotsFired");
		m_flFlashMaxAlpha		= GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
		m_flFlashOverlayAlpha = m_flFlashMaxAlpha - 0x8;

		// Weapon
		m_iItemDefinitionIndex	= GetOffset("DT_EconEntity", "m_iItemDefinitionIndex");
		m_iEntityQuality		= GetOffset("DT_EconEntity", "m_iEntityQuality");
		m_iItemIDHigh			= GetOffset("DT_EconEntity", "m_iItemIDHigh");
		m_iAccountID			= GetOffset("DT_EconEntity", "m_iAccountID");
		m_OriginalOwnerXuidLow	= GetOffset("DT_EconEntity", "m_OriginalOwnerXuidLow");
		m_nFallbackPaintKit		= GetOffset("DT_EconEntity", "m_nFallbackPaintKit");
		m_nFallbackSeed			= GetOffset("DT_EconEntity", "m_nFallbackSeed");
		m_flFallbackWear		= GetOffset("DT_EconEntity", "m_flFallbackWear");
		m_nFallbackStatTrak		= GetOffset("DT_EconEntity", "m_nFallbackStatTrak");
		m_Activity				= GetOffset("DT_BaseCombatWeapon", "m_flTimeWeaponIdle") + 0x20;
		m_iViewModelIndex		= GetOffset("DT_BaseCombatWeapon", "m_iViewModelIndex");
		m_iWorldModelIndex		= GetOffset("DT_BaseCombatWeapon", "m_iWorldModelIndex");
		m_iWorldDroppedModelIndex = GetOffset("DT_BaseCombatWeapon", "m_iWorldDroppedModelIndex");
		m_hWeaponWorldModel		= GetOffset("DT_BaseCombatWeapon", "m_hWeaponWorldModel");
		m_flNextPrimaryAttack	= GetOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
		m_flNextSecondaryAttack = GetOffset("DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
		m_iClip1				= GetOffset("DT_BaseCombatWeapon", "m_iClip1");
		m_iPrimaryReserveAmmoCount = GetOffset("DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount");
		m_weaponMode			= GetOffset("DT_WeaponCSBase", "m_weaponMode");
		m_flRecoilIndex			= GetOffset("DT_WeaponCSBase", "m_flRecoilIndex");
		m_bBurstMode			= GetOffset("DT_WeaponCSBase", "m_bBurstMode");
		m_flPostponeFireReadyTime = GetOffset("DT_WeaponCSBase", "m_flPostponeFireReadyTime");
		m_iBurstShotsRemaining	= GetOffset("DT_WeaponCSBaseGun", "m_iBurstShotsRemaining");
		m_fNextBurstShot		= m_iBurstShotsRemaining + 0x4;

		// Etc
		m_bBombTicking			= GetOffset("DT_PlantedC4", "m_bBombTicking");
		m_flC4Blow				= GetOffset("DT_PlantedC4", "m_flC4Blow");
		m_flDefuseCountDown		= GetOffset("DT_PlantedC4", "m_flDefuseCountDown");
		m_hBombDefuser			= GetOffset("DT_PlantedC4", "m_hBombDefuser");
		//m_WritableBones		= GetOffset("DT_BaseAnimating", "m_bClientSideFrameReset") + 0x10;

		m_flDamage				= GetOffset("DT_BaseGrenade", "m_flDamage");
		m_nExplodeEffectTickBegin = GetOffset("DT_BaseCSGrenadeProjectile", "m_nExplodeEffectTickBegin");
		m_decoyParticleInitTime	= GetOffset("DT_BaseCSGrenadeProjectile", "m_vecExplodeEffectOrigin") + 0x34;
		m_nSmokeEffectTickBegin	= GetOffset("DT_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin");
		m_drawable				= GetOffset("DT_Inferno", "m_fireCount") + 0x10;
		m_drawableCount			= m_drawable + /* sizeof(C_Inferno::Drawable) */ 56  * 100 /* MAX_INFERNO_FIRES */;
		m_nFireEffectTickBegin	= GetOffset("DT_Inferno", "m_nFireEffectTickBegin");

		m_bPinPulled			= GetOffset("DT_BaseCSGrenade", "m_bPinPulled");
		m_fThrowTime			= GetOffset("DT_BaseCSGrenade", "m_fThrowTime");

		m_bStartedArming		= GetOffset("DT_WeaponC4", "m_bStartedArming");

		m_hWeapon				= GetOffset("DT_BaseViewModel", "m_hWeapon");
		m_hOwner				= GetOffset("DT_BaseViewModel", "m_hOwner");

		m_iKills				= GetOffset("DT_CSPlayerResource", "m_iKills");
		m_iAssists				= GetOffset("DT_CSPlayerResource", "m_iAssists");
		m_iDeaths				= GetOffset("DT_CSPlayerResource", "m_iDeaths");
		m_bConnected			= GetOffset("DT_CSPlayerResource", "m_bConnected");
		m_iTeam					= GetOffset("DT_CSPlayerResource", "m_iTeam");
		m_iPlayerC4				= GetOffset("DT_CSPlayerResource", "m_iPlayerC4");
		m_iMVPs					= GetOffset("DT_CSPlayerResource", "m_iMVPs");
		m_iScore				= GetOffset("DT_CSPlayerResource", "m_iScore");
		m_iCompetitiveWins		= GetOffset("DT_CSPlayerResource", "m_iCompetitiveWins");
		m_iCompTeammateColor	= GetOffset("DT_CSPlayerResource", "m_iCompTeammateColor");
		m_iBotDifficulty		= GetOffset("DT_CSPlayerResource", "m_iBotDifficulty");
		m_nPersonaDataPublicLevel = GetOffset("DT_CSPlayerResource", "m_nPersonaDataPublicLevel");
		m_nPersonaDataPublicCommendsLeader = GetOffset("DT_CSPlayerResource", "m_nPersonaDataPublicCommendsLeader");
		m_nPersonaDataPublicCommendsTeacher = GetOffset("DT_CSPlayerResource", "m_nPersonaDataPublicCommendsTeacher");
		m_nPersonaDataPublicCommendsFriendly = GetOffset("DT_CSPlayerResource", "m_nPersonaDataPublicCommendsFriendly");
		m_iMatchStats_Damage_Total = GetOffset("DT_CSPlayerResource", "m_iMatchStats_Damage_Total");

		m_bFreezePeriod			= GetOffset("DT_CSGameRulesProxy", "m_bFreezePeriod");
		m_bWarmupPeriod			= GetOffset("DT_CSGameRulesProxy", "m_bWarmupPeriod");
		m_iRoundTime			= GetOffset("DT_CSGameRulesProxy", "m_iRoundTime");
		m_bIsQueuedMatchmaking	= GetOffset("DT_CSGameRulesProxy", "m_bIsQueuedMatchmaking");
		m_nQueuedMatchmakingMode = GetOffset("DT_CSGameRulesProxy", "m_nQueuedMatchmakingMode");
	}
}

struct CProxyRecvHook
{
	RecvVarProxyFn original;
	RecvVarProxyFn* current;

	~CProxyRecvHook()
	{
		Unhook();
	}

	void Hook(const char* TableName, const char* VarName, RecvVarProxyFn func)
	{
		RecvTable* Table;
		RecvProp* Prop;
		if (!(Table = NetVars::GetTable(TableName)) ||
			!(Prop = NetVars::GetProp(Table, VarName)))
		{
			FatalError("Offset not found");
			return;
		}

		original = Prop->m_ProxyFn;
		current = &Prop->m_ProxyFn;

		Prop->m_ProxyFn = func;
	}

	void Unhook()
	{
		*current = original;
	}
};