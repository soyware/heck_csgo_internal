#pragma once

const char* GetHeckFolder()
{
	static char path[MAX_PATH] = { 0 };
	if (!path[0])
	{
		const char* gameDir = I::EngineClient->GetGameDirectory();
		const rsize_t lastSlashLen = (strrchr(gameDir, '\\') + 1) - gameDir;
		strncpy_s(path, sizeof(path), gameDir, lastSlashLen);
		strcat_s(path, sizeof(path), "heck\\");

		std::filesystem::create_directory(path);
	}
	return path;
}

enum
{
	CHAMS_STYLE_NORMAL,
	CHAMS_STYLE_FLAT,

	CHAMS_STYLE_COUNT,
	CHAMS_STYLE_LAST = CHAMS_STYLE_COUNT - 1,
};

namespace Settings
{
	namespace Moves
	{
		namespace Aim
		{
			namespace Triggerbot
			{
				ConVar Enable;
				ConVar Key;
				ConVar Hitbox;
				ConVar VisCheck;
				ConVar Hitchance;
				ConVar Delay;
			}

			namespace Backtrack
			{
				ConVar Enable;
				ConVar Max;
				ConVar Draw;
				ConVar DrawClr;
				ConVar Extend;
			}

			ConVar Teammates;
			ConVar AutoPistol;
		}

		namespace AntiAim
		{
			ConVar Enable;
			ConVar Pitch;
			ConVar Yaw;
			ConVar Desync;
			ConVar DesyncSwitchSideKey;
			ConVar DesyncReadjust;
		}

		ConVar AutoMount;
		ConVar AutoBlockKey;
		ConVar AutoJump;
		ConVar AutoStrafe;
		ConVar SlideWalk;
	}

	namespace Visuals
	{
		namespace ESP
		{
			ConVar Enable;
			ConVar Style;
			ConVar Teammates;
			ConVar VisCheck;
			ConVar Box;
			ConVar BoxClrTeam;
			ConVar BoxClrEnemy;
			ConVar Health;
			ConVar HealthText;
			ConVar HealthPos;
			ConVar Weapon;
			ConVar WeaponAmmo;
			ConVar Name;
			ConVar Bones;
			ConVar BonesClr;
			ConVar Head;
			ConVar TextClr;
			ConVar Status;

			namespace Barrel
			{
				ConVar Enable;
				ConVar VisCheck;
				ConVar ClrTeam;
				ConVar ClrEnemy;
			}
		}

		namespace Visuals
		{
			ConVar Enable;
			ConVar C4;
			ConVar C4Clr;
			ConVar Defuser;
			ConVar DefuserClr;
			ConVar Weapon;
			ConVar WeaponClr;
			ConVar WeaponAmmo;
			ConVar Box;
			ConVar Text;
			ConVar Glow;
			ConVar Chams;
			ConVar ChamsVisCheck;

			namespace Grenade
			{
				ConVar Enable;
				ConVar Thrower;
				ConVar Timer;
				ConVar Range;
			}
		}

		namespace Glow
		{
			ConVar Enable;
			ConVar Teammates;
			ConVar VisCheck;
			ConVar clrTeam;
			ConVar clrEnemy;
			ConVar Style;
		}

		namespace Chams
		{
			ConVar Enable;
			ConVar Teammates;
			ConVar VisCheck;
			ConVar Style;
			ConVar clrVisTeam;
			ConVar clrVisEnemy;
			ConVar clrNotVisTeam;
			ConVar clrNotVisEnemy;
			ConVar Wireframe;

			namespace Arms
			{
				ConVar Enable;
				ConVar VisCheck;
				ConVar Style;
				ConVar clrVis;
				ConVar clrNotVis;
				ConVar Wireframe;
			}

			namespace Weapon
			{
				ConVar Enable;
				ConVar Style;
				ConVar Color;
				ConVar Wireframe;
			}
		}

		namespace Crosshair
		{
			ConVar Snipers;
			ConVar Recoil; // TODO: would be nice to have custom drawn AUG crosshair
		}

		namespace ShowImpacts
		{
			ConVar Enable;
			ConVar Color;
			ConVar Duration;
		}

		ConVar Spectators;
		ConVar NoSmoke;
		ConVar MaxFlash;
		ConVar NoSniperScope;
		ConVar NoPostProcessing;
		ConVar NoVisualRecoil;
		ConVar ViewmodelFov;
		ConVar ViewFov;
		ConVar NoFog;
		ConVar Radar;
		ConVar GrenadeTrajectory;
		ConVar SkyName;
	}

	namespace Misc
	{
		namespace FakeLag
		{
			ConVar Enable;
			ConVar Amount;
			ConVar ToggleKey;
			ConVar OnShot;
		}

		namespace SkinChanger
		{
			ConVar Enable;
			ConVar KnifeModel;
		}

		namespace ThirdPerson
		{
			ConVar Enable;
			ConVar ToggleKey;
			ConVar OnGrenade;
			ConVar Dist;
		}

		namespace HitSound
		{
			ConVar Enable;
			ConVar Sound;
			ConVar Volume;
		}

		namespace Log
		{
			ConVar Buy;
			ConVar Hit;
			ConVar TeamDmg;
			ConVar Wins;
		}

		namespace FakeCrosshair
		{
			ConVar Enable;
			ConVar Code;
		}

		namespace CmdSpam
		{
			ConVar Enable;
			ConVar Cmd;
			ConVar Interval;
		}

		namespace DeathSound
		{
			ConVar Enable;
			ConVar Loopback;
			ConVar Chance;
			ConVar MinSpecs;
			ConVar OnlyTeamkill;
		}

		namespace ServerLagger
		{
			ConVar Enable;
			ConVar Strength;
			ConVar RestTime;
			ConVar LagTime;
		}

		ConVar ShowRanks;
		ConVar ClanTag;
		ConVar NoDuckCooldown;
		ConVar SteamStatus;
		ConVar AspectRatio;
		ConVar BypassSvPure;
	}

	namespace Commands
	{
		ConCommand Keys;
		ConCommand Save;
		ConCommand Load;
		ConCommand Uninject;
	}

	void PrintKeys()
	{
		Log::Msg( "Key\t\t\tCode\n0\t\t\t1\n1\t\t\t2\n2\t\t\t3\n3\t\t\t4\n4\t\t\t5\n5\t\t\t6\n6\t\t\t7\n7\t\t\t8\n8\t\t\t9\n9\t\t\t10\nA\t\t\t11\nB\t\t\t12\nC\t\t\t13\nD\t\t\t14\nE\t\t\t15\nF\t\t\t16\nG\t\t\t17\nH\t\t\t18\nI\t\t\t19\nJ\t\t\t20\nK\t\t\t21\nL\t\t\t22\nM\t\t\t23\nN\t\t\t24\nO\t\t\t25\nP\t\t\t26\nQ\t\t\t27\nR\t\t\t28\nS\t\t\t29\nT\t\t\t30\nU\t\t\t31\nV\t\t\t32\nW\t\t\t33\nX\t\t\t34\nY\t\t\t35\nZ\t\t\t36\nPAD 0\t\t\t37\nPAD 1\t\t\t38\nPAD 2\t\t\t39\nPAD 3\t\t\t40\nPAD 4\t\t\t41\nPAD 5\t\t\t42\nPAD 6\t\t\t43\nPAD 7\t\t\t44\nPAD 8\t\t\t45\nPAD 9\t\t\t46\nPAD /\t\t\t47\nPAD *\t\t\t48\nPAD -\t\t\t49\nPAD +\t\t\t50\nPAD ENTER\t\t51\nPAD .\t\t\t52\n[\t\t\t53\n]\t\t\t54\n;\t\t\t55\n'\t\t\t56\n`\t\t\t57\n,\t\t\t58\n>\t\t\t59\n/\t\t\t60\n\\\t\t\t61\n-\t\t\t62\n=\t\t\t63\nENTER\t\t\t64\nSPACE\t\t\t65\nBACKSPACE\t\t66\nTAB\t\t\t67\nCAPSLOCK\t\t68\nNUMLOCK\t\t\t69\nESCAPE\t\t\t70\nSCROLLLOCK\t\t71\nINSERT\t\t\t72\nDELETE\t\t\t73\nHOME\t\t\t74\nEND\t\t\t75\nPAGEUP\t\t\t76\nPAGEDOWN\t\t77\nBREAK\t\t\t78\nLSHIFT\t\t\t79\nRSHIFT\t\t\t80\nLALT\t\t\t81\nRALT\t\t\t82\nLCONTROL\t\t83\nRCONTROL\t\t84\nLWIN\t\t\t85\nRWIN\t\t\t86\nAPP\t\t\t87\nUP\t\t\t88\nLEFT\t\t\t89\nDOWN\t\t\t90\nRIGHT\t\t\t91\nF1\t\t\t92\nF2\t\t\t93\nF3\t\t\t94\nF4\t\t\t95\nF5\t\t\t96\nF6\t\t\t97\nF7\t\t\t98\nF8\t\t\t99\nF9\t\t\t100\nF10\t\t\t101\nF11\t\t\t102\nF12\t\t\t103\nCAPSLOCKTOGGLE\t\t104\nNUMLOCKTOGGLE\t\t105\nSCROLLLOCKTOGGLE\t106\nMOUSE LEFT\t\t107\nMOUSE RIGHT\t\t108\nMOUSE MIDDLE\t\t109\nMOUSE 4\t\t\t110\nMOUSE 5\t\t\t111\nMOUSE WHEEL UP\t\t112\nMOUSE WHEEL DOWN\t113\n");
	}

	void Save(const CCommand &command)
	{
		if (command.ArgC() < 2)
		{
			Log::Msg( "Usage: heck_config_save <name>\n");
			return;
		}

		char path[MAX_PATH];
		strcpy_s(path, sizeof(path), GetHeckFolder());
		strcat_s(path, sizeof(path), command.Arg(1));
		strcat_s(path, sizeof(path), ".cfg");

		FILE* pFile;
		if (fopen_s(&pFile, path, "wb") != 0)
		{
			Log::Msg(Color::Yellow(), "Failed to save!\n");
			return;
		}

		for (size_t i = 0; i < g_CreatedConVars.size(); ++i)
			fprintf(pFile, "%s \"%s\"\n", g_CreatedConVars[i]->m_pszName, g_CreatedConVars[i]->GetString());

		fclose(pFile);

		Log::Msg("Successfully saved\n");
	}

	int ConfigAutoComplete(const char* partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH])
	{
		static char folder[MAX_PATH] = { 0 };
		if (!folder[0])
			strcpy_s(folder, sizeof(folder), GetHeckFolder());

		int count = 0;

		const char* filter = strchr(partial, ' ') + 1;
		size_t filterLen = strlen(filter);

		wchar_t wFilter[32];
		size_t wFilterLen;
		mbstowcs_s(&wFilterLen, wFilter, _countof(wFilter), filter, filterLen);

		for (const auto& entry : std::filesystem::directory_iterator(folder))
		{
			if (!entry.is_regular_file())
				continue;

			auto filename = entry.path().stem().native();

			if (!wcsstr(filename.c_str(), wFilter))
				continue;

			char cFilename[32];
			size_t cFilenameLen;
			if (wcstombs_s(&cFilenameLen, cFilename, sizeof(cFilename), filename.c_str(), filename.size()))
				continue;

			strcpy_s(commands[count], COMMAND_COMPLETION_ITEM_LENGTH, "heck_config_load ");
			strcat_s(commands[count], COMMAND_COMPLETION_ITEM_LENGTH, cFilename);

			++count;
		}

		return count;
	}

	void Load(const CCommand &command)
	{
		if (command.ArgC() < 2)
		{
			Log::Msg( "Usage: heck_config_load <name>\n");
			return;
		}

		char path[MAX_PATH];
		strcpy_s(path, sizeof(path), GetHeckFolder());
		strcat_s(path, sizeof(path), command.Arg(1));
		strcat_s(path, sizeof(path), ".cfg");

		FILE* pFile;
		if (fopen_s(&pFile, path, "rb") != 0)
		{
			Log::Msg(Color::Yellow(), "Failed to load!\n");
			return;
		}

		char buffer[512];

		while (fgets(buffer, sizeof(buffer), pFile))
			I::EngineClient->ClientCmd_Unrestricted(buffer);

		fclose(pFile);

		Log::Msg( "Successfully loaded\n");
	}

	void CrosshairRecoilChanged(void* var, const char* pOldValue, float flOldValue)
	{
		static ConVar* cl_crosshair_recoil = I::Cvar->FindVar("cl_crosshair_recoil");
		cl_crosshair_recoil->m_nFlags &= ~FCVAR_CHEAT;
		cl_crosshair_recoil->SetValue(Visuals::Crosshair::Recoil.GetInt());
	}

	void NoPostProcessingChanged(void* var, const char* pOldValue, float flOldValue)
	{
		static ConVar* mat_postprocess_enable = I::Cvar->FindVar("mat_postprocess_enable");
		mat_postprocess_enable->m_nFlags &= ~FCVAR_CHEAT;
		mat_postprocess_enable->SetValue(!Visuals::NoPostProcessing.GetInt());
	}

	void ViewmodelFovChanged(void* var, const char* pOldValue, float flOldValue)
	{
		static ConVar* viewmodel_fov = I::Cvar->FindVar("viewmodel_fov");
		const static int oVal = viewmodel_fov->GetInt();
		viewmodel_fov->m_fnChangeCallbacks.m_Size = 0;
		int setting = Visuals::ViewmodelFov.GetInt();
		viewmodel_fov->SetValue(setting ? setting : oVal);
	}

	void GrenadeTrajectoryChanged(void* var, const char* pOldValue, float flOldValue)
	{
		static ConVar* cl_grenadepreview = I::Cvar->FindVar("cl_grenadepreview");
		cl_grenadepreview->m_nFlags &= ~FCVAR_CHEAT;
		cl_grenadepreview->SetValue(Visuals::GrenadeTrajectory.GetInt());
	}

	void AspectRatioChanged(void* var, const char* pOldValue, float flOldValue)
	{
		static ConVar* r_aspectratio = I::Cvar->FindVar("r_aspectratio");
		r_aspectratio->m_nFlags &= ~FCVAR_DEVELOPMENTONLY;
		r_aspectratio->SetValue(Misc::AspectRatio.GetFloat());
	}

	char oSky[48] = { 0 };

	void SkyNameSetOriginal()
	{
		static ConVar* sv_skyname = I::Cvar->FindVar("sv_skyname");
		strcpy_s(oSky, sizeof(oSky), sv_skyname->GetString());
	}

	void SkyNameChanged(void* var, const char* pOldValue, float flOldValue)
	{
		const char* setting = Visuals::SkyName.GetString();

		static ConVar* r_3dsky = I::Cvar->FindVar("r_3dsky");
		r_3dsky->SetValue(setting[0] == '\0');

		static ConVar* sv_skyname = I::Cvar->FindVar("sv_skyname");
		sv_skyname->m_nFlags &= ~FCVAR_CHEAT;
		sv_skyname->SetValue((setting[0] != '\0') ? setting : oSky);
	}

	void ThirdPersonDistChanged(void* var, const char* pOldValue, float flOldValue)
	{
		static ConVar* cam_idealdist = I::Cvar->FindVar("cam_idealdist");
		cam_idealdist->SetValue(Misc::ThirdPerson::Dist.GetInt());
	}

	void SkinChanged(void* var, const char* pOldValue, float flOldValue)
	{
		static CCommand empty;
		static ConCommand* fullUpdate = I::Cvar->FindCommand("cl_fullupdate");
		fullUpdate->Dispatch(empty);
	}

	/*
	void ServerLaggerChanged(void* var, const char* pOldValue, float flOldValue)
	{
		static const char* varNames[] = {
			"sv_maxroutable",
			"net_maxroutable",
			"net_compresspackets_minsize",
			"net_compresspackets",
			"net_threaded_socket_recovery_time",
			"net_threaded_socket_recovery_rate",
			"net_threaded_socket_burst_cap"
		};

		static const int values[] = {
			13,
			13,
			0,
			1,
			2,
			999999999,
			999999999
		};

		static int oValues[_countof(values)];

		static_assert(_countof(varNames) == _countof(values));

		DWORD oldProtect;

		if (Misc::ServerLagger::Enable.GetInt())
		{
			for (UINT i = 0; i < _countof(varNames); ++i)
			{
				ConVar* cv = I::Cvar->FindVar(varNames[i]);
				if (!cv)
					continue;
				oValues[i] = cv->GetInt();
				int prevCallbacksSize = cv->m_fnChangeCallbacks.m_Size;
				cv->m_fnChangeCallbacks.m_Size = 0;
				cv->SetValue(values[i]);
				cv->m_fnChangeCallbacks.m_Size = prevCallbacksSize;
			}

			VirtualProtect((void*)Signatures::NET_SendPacket_MaxRoutableMin, sizeof(int32_t), PAGE_EXECUTE_READWRITE, &oldProtect);
			*(int32_t*)(Signatures::NET_SendPacket_MaxRoutableMin) = 36;
			VirtualProtect((void*)Signatures::NET_SendPacket_MaxRoutableMin, sizeof(int32_t), oldProtect, &oldProtect);

			VirtualProtect((void*)Signatures::TooManyQueuedPackets, sizeof(uint32_t), PAGE_EXECUTE_READWRITE, &oldProtect);
			*(uint32_t*)(Signatures::TooManyQueuedPackets) = 0x54EB9090;
			VirtualProtect((void*)Signatures::TooManyQueuedPackets, sizeof(uint32_t), oldProtect, &oldProtect);
		}
		else
		{
			for (UINT i = 0; i < _countof(varNames); ++i)
			{
				ConVar* cv = I::Cvar->FindVar(varNames[i]);
				if (!cv)
					continue;
				cv->SetValue(oValues[i]);
			}

			VirtualProtect((void*)Signatures::NET_SendPacket_MaxRoutableMin, sizeof(int32_t), PAGE_EXECUTE_READWRITE, &oldProtect);
			*(int32_t*)(Signatures::NET_SendPacket_MaxRoutableMin) = 576;
			VirtualProtect((void*)Signatures::NET_SendPacket_MaxRoutableMin, sizeof(int32_t), oldProtect, &oldProtect);

			VirtualProtect((void*)Signatures::TooManyQueuedPackets, sizeof(uint32_t), PAGE_EXECUTE_READWRITE, &oldProtect);
			*(uint32_t*)(Signatures::TooManyQueuedPackets) = 0x547E083B;
			VirtualProtect((void*)Signatures::TooManyQueuedPackets, sizeof(uint32_t), oldProtect, &oldProtect);
		}
	}
	*/

	class SkinInfo
	{
	public:
		ConVar Skin;
		ConVar Wear;
		ConVar Seed;
		ConVar StatTrak;

		SkinInfo(const char* weaponName)
		{
			char(* const varnames)[32] = new char[4][32];

			sprintf_s(varnames[0], 32, "heck_sc_%s_skin", weaponName);
			sprintf_s(varnames[1], 32, "heck_sc_%s_wear", weaponName);
			sprintf_s(varnames[2], 32, "heck_sc_%s_seed", weaponName);
			sprintf_s(varnames[3], 32, "heck_sc_%s_stattrak", weaponName);

			Skin.Create(varnames[0], "0", 0, nullptr, true, 0.f, false, 0.f, &SkinChanged);
			Wear.Create(varnames[1], "0.001", 0, nullptr, true, 0.001f, false, 0.f, &SkinChanged);
			Seed.Create(varnames[2], "0", 0, nullptr, true, 0.f, false, 0.f, &SkinChanged);
			StatTrak.Create(varnames[3], "-1", 0, nullptr, true, -1.f, false, 0.f, &SkinChanged);
		}

		~SkinInfo()
		{
			I::Cvar->UnregisterConCommand(&Skin);
			I::Cvar->UnregisterConCommand(&Wear);
			I::Cvar->UnregisterConCommand(&Seed);
			I::Cvar->UnregisterConCommand(&StatTrak);
			const char* varnames = Skin.m_pszName;
			delete[] varnames;
		}
	};
	std::map<short, SkinInfo> SkinInfos;

	void Init()
	{
		Commands::Keys			.Create("heck_keys",				&PrintKeys);
		Commands::Save			.Create("heck_config_save",			&Save,				"Saves to *csgo folder*\\heck");
		Commands::Load			.Create("heck_config_load",			&Load,				nullptr, 0,						&ConfigAutoComplete);
		Commands::Uninject		.Create("heck_uninject",			&Uninject);

		Moves::Aim::Triggerbot::Enable		.Create("heck_trigger_enable",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Moves::Aim::Triggerbot::Key			.Create("heck_trigger_key",				"0", 0,				nullptr,													true, 0.f, true, BUTTON_CODE_LAST);
		Moves::Aim::Triggerbot::Hitbox		.Create("heck_trigger_hitbox",			"0", 0,				"0 - Off, 1 - Head, 2 - Body",								true, 0.f, true, 2.f);
		Moves::Aim::Triggerbot::VisCheck	.Create("heck_trigger_vischeck",		"0", 0,				"Check for a flash and smoke",								true, 0.f, true, 1.f);
		Moves::Aim::Triggerbot::Hitchance	.Create("heck_trigger_hitchance",		"0", 0,				nullptr,													true, 0.f, true, 100.f);
		Moves::Aim::Triggerbot::Delay		.Create("heck_trigger_delay",			"0", 0,				"Milliseconds",												true, 0.f, true, 100.f);

		Moves::Aim::Backtrack::Enable		.Create("heck_backtrack_enable",		"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Moves::Aim::Backtrack::Max			.Create("heck_backtrack_max",			"200", 0,			"Max milliseconds to backtrack",							true, 0.f, true, 200.f);
		Moves::Aim::Backtrack::Draw			.Create("heck_backtrack_draw",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Moves::Aim::Backtrack::DrawClr		.Create("heck_backtrack_draw_clr",		"255 255 255 64");
		Moves::Aim::Backtrack::Extend		.Create("heck_backtrack_extend",		"0", 0,				"Fake latency",												true, 0.f, true, 1.f);

		Moves::Aim::Teammates				.Create("heck_aim_team",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Moves::Aim::AutoPistol				.Create("heck_autopistol",				"0", 0,				nullptr,													true, 0.f, true, 1.f);

		Moves::AntiAim::Enable				.Create("heck_aa_enable",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Moves::AntiAim::Pitch				.Create("heck_aa_pitch",				"0", 0,				"0 - Off, 1 - Down",										true, 0.f, true, 1.f);
		Moves::AntiAim::Yaw					.Create("heck_aa_yaw",					"0", 0,				"0 - Off, 1 - Back",										true, 0.f, true, 1.f);
		Moves::AntiAim::Desync				.Create("heck_aa_desync",				"0", 0,				"Opposite desync",											true, 0.f, true, 1.f);
		Moves::AntiAim::DesyncSwitchSideKey	.Create("heck_aa_desync_switch_side_key","0", 0,			"Switch real side",											true, 0.f, true, BUTTON_CODE_LAST);
		Moves::AntiAim::DesyncReadjust		.Create("heck_aa_desync_readjust",		"0", 0,				"Makes u shake ur butt",									true, 0.f, true, 1.f);

		Moves::AutoJump						.Create("heck_autojump",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Moves::AutoStrafe					.Create("heck_autostrafe",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Moves::SlideWalk					.Create("heck_slidewalk",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Moves::AutoMount					.Create("heck_automount",				"0", 0,				"Walk on top of crouched teammate, works when not moving",	true, 0.f, true, 1.f);
		Moves::AutoBlockKey					.Create("heck_autoblock_key",			"0", 0,				"Block closest teammate, aim at block direction",			true, 0.f, true, BUTTON_CODE_LAST);
		
		Visuals::ESP::Enable				.Create("heck_esp_enable",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::ESP::Style					.Create("heck_esp_style",				"0", 0,				"0 - Shadow, 1 - Outline",									true, 0.f, true, 1.f);
		Visuals::ESP::Teammates				.Create("heck_esp_team",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::ESP::VisCheck				.Create("heck_esp_vischeck",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::ESP::Box					.Create("heck_esp_box",					"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::ESP::BoxClrTeam			.Create("heck_esp_box_clr_team",		"192 192 255 255");
		Visuals::ESP::BoxClrEnemy			.Create("heck_esp_box_clr_enemy",		"255 192 192 255");
		Visuals::ESP::Health				.Create("heck_esp_health",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::ESP::HealthText			.Create("heck_esp_health_text",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::ESP::HealthPos				.Create("heck_esp_health_pos",			"0", 0,				"0 - Health, 1 - Weapon",									true, 0.f, true, 1.f);
		Visuals::ESP::Weapon				.Create("heck_esp_weapon",				"0", 0,				"0 - Off, 1 - Text, 2 - Icon",								true, 0.f, true, 2.f);
		Visuals::ESP::WeaponAmmo			.Create("heck_esp_weapon_ammo",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::ESP::Name					.Create("heck_esp_name",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::ESP::Bones					.Create("heck_esp_bones",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::ESP::BonesClr				.Create("heck_esp_bones_clr",			"255 255 255 92");
		Visuals::ESP::Head					.Create("heck_esp_head",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::ESP::TextClr				.Create("heck_esp_text_clr",			"255 255 255 127");
		Visuals::ESP::Status				.Create("heck_esp_status",				"0", 0,				"Draw scoped, flashed, reloading, etc",						true, 0.f, true, 1.f);
		Visuals::ESP::Barrel::Enable		.Create("heck_esp_barrel",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::ESP::Barrel::ClrTeam		.Create("heck_esp_barrel_clr_team",		"92 92 255 127");
		Visuals::ESP::Barrel::ClrEnemy		.Create("heck_esp_barrel_clr_enemy",	"255 92 92 127");
		Visuals::ESP::Barrel::VisCheck		.Create("heck_esp_barrel_vischeck",		"0", 0,				nullptr,													true, 0.f, true, 1.f);

		Visuals::Visuals::Enable			.Create("heck_visuals_enable",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Visuals::C4				.Create("heck_visuals_c4",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Visuals::C4Clr				.Create("heck_visuals_c4_clr",			"255 127 0 255", 0, "Color turns red if not enough time, or green if enough");
		Visuals::Visuals::Grenade::Enable	.Create("heck_visuals_grenade",			"0", 0,				"Draw thrown grenades",										true, 0.f, true, 1.f);
		Visuals::Visuals::Grenade::Thrower	.Create("heck_visuals_grenade_thrower",	"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Visuals::Grenade::Timer	.Create("heck_visuals_grenade_timer",	"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Visuals::Grenade::Range	.Create("heck_visuals_grenade_range",	"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Visuals::Defuser			.Create("heck_visuals_defuser",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Visuals::DefuserClr		.Create("heck_visuals_defuser_clr",		"128 128 192 255");
		Visuals::Visuals::Weapon			.Create("heck_visuals_weapon",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Visuals::WeaponClr			.Create("heck_visuals_weapon_clr",		"192 192 192 255");
		Visuals::Visuals::WeaponAmmo		.Create("heck_visuals_weapon_ammo",		"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Visuals::Box				.Create("heck_visuals_box",				"0", 0,				"Draw a bounding box",										true, 0.f, true, 1.f);
		Visuals::Visuals::Text				.Create("heck_visuals_text",			"0", 0,				"0 - Off, 1 - Text, 2 - Icon",								true, 0.f, true, 2.f);
		Visuals::Visuals::Glow				.Create("heck_visuals_glow",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Visuals::Chams				.Create("heck_visuals_chams",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Visuals::ChamsVisCheck		.Create("heck_visuals_chams_vis_check",	"0", 0,				nullptr,													true, 0.f, true, 1.f);

		Visuals::Glow::Enable				.Create("heck_glow_enable",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Glow::Teammates			.Create("heck_glow_team",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Glow::VisCheck				.Create("heck_glow_vischeck",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Glow::clrTeam				.Create("heck_glow_clr_team",			"92 92 255 127");
		Visuals::Glow::clrEnemy				.Create("heck_glow_clr_enemy",			"255 92 92 127");
		Visuals::Glow::Style				.Create("heck_glow_style",				"0", 0,				nullptr,													true, 0.f, true, 3.f);

		Visuals::Chams::Enable				.Create("heck_chams_enable",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Chams::Teammates			.Create("heck_chams_team",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Chams::VisCheck			.Create("heck_chams_vischeck",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Chams::Style				.Create("heck_chams_style",				"0", 0,				"0 - Normal, 1 - Flat",										true, 0.f, true, CHAMS_STYLE_LAST);
		Visuals::Chams::clrVisTeam			.Create("heck_chams_clr_vis_team",		"64 64 192 255");
		Visuals::Chams::clrVisEnemy			.Create("heck_chams_clr_vis_enemy",		"192 64 64 255");
		Visuals::Chams::clrNotVisTeam		.Create("heck_chams_clr_not_vis_team",	"32 127 192 255");
		Visuals::Chams::clrNotVisEnemy		.Create("heck_chams_clr_not_vis_enemy",	"192 127 32 255");
		Visuals::Chams::Wireframe			.Create("heck_chams_wireframe",			"0", 0,				nullptr,													true, 0.f, true, 1.f);

		Visuals::Chams::Arms::Enable		.Create("heck_chams_arms_enable",		"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Chams::Arms::VisCheck		.Create("heck_chams_arms_vischeck",		"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Chams::Arms::Style			.Create("heck_chams_arms_style",		"0", 0,				"0 - Normal, 1 - Flat",										true, 0.f, true, CHAMS_STYLE_LAST);
		Visuals::Chams::Arms::clrVis		.Create("heck_chams_arms_clr_vis",		"48 127 255 255");
		Visuals::Chams::Arms::clrNotVis		.Create("heck_chams_arms_clr_not_vis",	"24 64 255 255");
		Visuals::Chams::Arms::Wireframe		.Create("heck_chams_arms_wireframe",	"0", 0,				nullptr,													true, 0.f, true, 1.f);

		Visuals::Chams::Weapon::Enable		.Create("heck_chams_weapon_enable",		"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Chams::Weapon::Style		.Create("heck_chams_weapon_style",		"0", 0,				"0 - Normal, 1 - Flat",										true, 0.f, true, CHAMS_STYLE_LAST);
		Visuals::Chams::Weapon::Color		.Create("heck_chams_weapon_clr",		"255 127 48 255");
		Visuals::Chams::Weapon::Wireframe	.Create("heck_chams_weapon_wireframe",	"0", 0,				nullptr,													true, 0.f, true, 1.f);

		Visuals::Crosshair::Recoil			.Create("heck_crosshair_recoil",		"0", 0,				nullptr,													true, 0.f, true, 1.f,	&CrosshairRecoilChanged);
		Visuals::Crosshair::Snipers			.Create("heck_crosshair_snipers",		"0", 0,				nullptr,													true, 0.f, true, 1.f);

		Visuals::Spectators					.Create("heck_spectators",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::NoSmoke					.Create("heck_no_smoke",				"0", 0,				"0 - Off, 1 - On, 2 - Wireframe",							true, 0.f, true, 2.f);
		Visuals::MaxFlash					.Create("heck_max_flash",				"100", 0,			"Max flash %",												true, 0.f, true, 100.f);
		Visuals::NoSniperScope				.Create("heck_no_sniper_scope",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::NoPostProcessing			.Create("heck_no_postprocessing",		"0", 0,				nullptr,													true, 0.f, true, 1.f,	&NoPostProcessingChanged);
		Visuals::NoVisualRecoil				.Create("heck_no_visual_recoil",		"0", 0,				"0 - Off, 1 - No shake, 2 - No visual recoil",				true, 0.f, true, 2.f);
		Visuals::ViewmodelFov				.Create("heck_viewmodel_fov",			"0", 0,				nullptr,													true, 0.f, true, 170.f,	&ViewmodelFovChanged);
		Visuals::ViewFov					.Create("heck_view_fov",				"0", 0,				nullptr,													true, 0.f, true, 170.f);
		Visuals::NoFog						.Create("heck_no_fog",					"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Visuals::Radar						.Create("heck_radar",					"0", 0,				"Show not spotted enemies on in-game radar",				true, 0.f, true, 1.f);
		Visuals::GrenadeTrajectory			.Create("heck_grenade_trajectory",		"0", 0,				nullptr,													true, 0.f, true, 1.f,	&GrenadeTrajectoryChanged);
		Visuals::SkyName					.Create("heck_sky_name",				"", 0,				nullptr,													false, 0.f, false, 0.f,	&SkyNameChanged);
		Visuals::ShowImpacts::Enable		.Create("heck_show_impacts_enable",		"0", 0,				"Show server bullet impact point",							false, 0.f, true, 1.f);
		Visuals::ShowImpacts::Color			.Create("heck_show_impacts_clr",		"0 0 255 127");
		Visuals::ShowImpacts::Duration		.Create("heck_show_impacts_duration",	"1", 0,				nullptr,													true, 0.1f, true, 10.f);

		Misc::FakeLag::Enable				.Create("heck_fakelag_enable",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Misc::FakeLag::Amount				.Create("heck_fakelag_amount",			"3", 0,				"Matchmaking max - 8",										true, 3.f, true, 64.f);
		Misc::FakeLag::ToggleKey			.Create("heck_fakelag_toggle_key",		"0", 0,				nullptr,													true, 0.f, true, BUTTON_CODE_LAST);
		Misc::FakeLag::OnShot				.Create("heck_fakelag_on_shot",			"0", 0,				nullptr,													true, 0.f, true, 1.f);

		Misc::ThirdPerson::Enable			.Create("heck_thirdperson_enable",		"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Misc::ThirdPerson::ToggleKey		.Create("heck_thirdperson_toggle_key",	"0", 0,				nullptr,													true, 0.f, true, BUTTON_CODE_LAST);
		Misc::ThirdPerson::OnGrenade		.Create("heck_thirdperson_on_grenade",	"0", 0,				"Dont switch to firstperson when holding nade",				true, 0.f, true, BUTTON_CODE_LAST);
		Misc::ThirdPerson::Dist				.Create("heck_thirdperson_dist",		"150", 0,			nullptr,													true, 30.f, true, 200.f, &ThirdPersonDistChanged);

		Misc::SkinChanger::Enable			.Create("heck_sc_enable",				"0", 0,				"Skinchanger",												true, 0.f, true, 1.f);
		Misc::SkinChanger::KnifeModel		.Create("heck_sc_knife_model",			"0", 0,				nullptr,													true, 0.f, true, 19.f,	&SkinChanged);

		Misc::HitSound::Enable				.Create("heck_hitsound_enable",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Misc::HitSound::Sound				.Create("heck_hitsound_sound",			"buttons\\arena_switch_press_02.wav", 0, "Put sound to csgos 'sound' folder to use it");
		Misc::HitSound::Volume				.Create("heck_hitsound_volume",			"1.0", 0,			nullptr,													true, 0.f, true, 1.f);

		Misc::Log::Buy						.Create("heck_log_buy",					"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Misc::Log::Hit						.Create("heck_log_hit",					"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Misc::Log::TeamDmg					.Create("heck_log_teamdmg",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Misc::Log::Wins						.Create("heck_log_wins",				"0", 0,				nullptr,													true, 0.f, true, 1.f);

		Misc::CmdSpam::Enable				.Create("heck_cmd_spam_enable",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Misc::CmdSpam::Cmd					.Create("heck_cmd_spam_cmd",			"getout", 0,		"Command to spam");
		Misc::CmdSpam::Interval				.Create("heck_cmd_spam_interval",		"800", 0,			"Milliseconds (radio - 800)",								true, 50.f, true, 5000.f);

		Misc::DeathSound::Enable			.Create("heck_deathsound_enable",		"0", 0,				"Check changelog for how to use",							true, 0.f, true, 1.f);
		Misc::DeathSound::Loopback			.Create("heck_deathsound_loopback",		"1", 0,				nullptr,													true, 0.f, true, 1.f);
		Misc::DeathSound::Chance			.Create("heck_deathsound_chance",		"100", 0,			nullptr,													true, 1.f, true, 100.f);
		Misc::DeathSound::MinSpecs			.Create("heck_deathsound_min_specs",	"0", 0,				"Minimum spectators for deathsound to play",				true, 0.f, true, 9.f);
		Misc::DeathSound::OnlyTeamkill		.Create("heck_deathsound_only_teamkill","0", 0,				"Play deathsound only when killed by a teammate",			true, 0.f, true, 1.f);

		Misc::ShowRanks						.Create("heck_show_ranks",				"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Misc::ClanTag						.Create("heck_clan_tag",				"");
		Misc::NoDuckCooldown				.Create("heck_no_duck_cooldown",		"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Misc::SteamStatus					.Create("heck_steam_status",			"", 0,				"Format \"key=value,key2=value2\". Keys - steamdb.info/app/730/localization/");
		Misc::FakeCrosshair::Enable			.Create("heck_fake_crosshair_enable",	"0", 0,				"Fake networked crosshair, works on next match",			true, 0.f, true, 1.f);
		Misc::FakeCrosshair::Code			.Create("heck_fake_crosshair_code",		"CSGO-4HOZd-3OXKt-LVqWA-aSBcA-7qFOP", 0, "Crosshair share code");
		Misc::AspectRatio					.Create("heck_aspectratio",				"0", 0,				"4:3=1.33 , 16:9=1.77 , 16:10=1.6",							true, 0.f, true, 4.f,	&AspectRatioChanged);
		Misc::BypassSvPure					.Create("heck_bypass_sv_pure",			"0", 0,				nullptr,													true, 0.f, true, 1.f);
		Misc::ServerLagger::Enable			.Create("heck_server_lagger_enable",	"0", 0,				nullptr,													true, 0.f, true, 1.f/*,	&ServerLaggerChanged*/); // works fine without
		Misc::ServerLagger::Strength		.Create("heck_server_lagger_strength",	"1", 0,				nullptr,													true, 1.f, true, 3.f);
		Misc::ServerLagger::RestTime		.Create("heck_server_lagger_rest_time",	"0", 0,				"Time between the lag, in seconds",							true, 0.f, true, 10.f);
		Misc::ServerLagger::LagTime			.Create("heck_server_lagger_lag_time",	"0", 0,				"Lag time, in seconds",										true, 0.f, true, 10.f);

		for (short i = WEAPON_DEAGLE; i <= WEAPON_REVOLVER; ++i)
		{
			if (i == WEAPON_ZONE_REPULSOR || i == WEAPON_TASER || i == WEAPON_SHIELD || (WEAPON_KNIFE_CT <= i && i <= WEAPON_KNIFE_T))
				continue;

			const char* name = GetWeaponName(i);
			if (name[0] == '\0')
				continue;

			name += sizeof("weapon_") - 1;

			SkinInfos.try_emplace(i, name);
		}
	}
}