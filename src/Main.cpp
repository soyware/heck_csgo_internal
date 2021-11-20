#include "stdafx.h"
#include "resource.h"
#include "SDK/Platform.h"
#include "SDK/Worldsize.h"
#include "SDK/Vector.h"
#include "SDK/Color.h"
#include "SDK/Logging.h"
#include "Misc.h"
#include "Signatures.h"
#include "SDK/Material.h"
#include "SDK/Model.h"
#include "SDK/bitbuf.h"
#include "SDK/CLZSS.h"
#include "SDK/Networking.h"
#include "SDK/Interfaces/Interfaces.h"
#include "SDK/GameEventListener.h"
#include "SDK/ConVar.h"
#include "SDK/KeyValues.h"
#include "SDK/GlobalVars.h"
#include "NetVars.h"
#include "SDK/GameRules.h"
#include "SDK/PlayerResource.h"
#include "SDK/Glow.h"
#include "SDK/Globals.h"
#include "SDK/Entity.h"
#include "SDK/RecipientFilter.h"
#include "SDK/Trace.h"
#include "SDK/UserCmd.h"
#include "SDK/View.h"
#include "SDK/Weapon.h"
#include "SDK/Player.h"
#include "SDK/AnimState.h"
#include "Settings.h"
#include "Draw.h"
#include "Features/Features.h"
#include "VTH.h"
#include "Hooks/Hooks.h"

bool g_bInitialized = false;

HINSTANCE g_hModule;

void FatalError(const char* err)
{
#ifdef _DEBUG
	__debugbreak();
#else
	static CCommand empty;

	static ConCommand* showconsole = I::Cvar->FindCommand("showconsole");
	showconsole->Dispatch(empty);

	Log::PrefixedMsg(Color::Red(), "FATAL ERROR: %s\n", err);
	Log::PrefixedMsg(Color::Red(), "Game will shutdown in 5 sec...\n");

	std::this_thread::sleep_for(5s);

	static ConCommand* quit = I::Cvar->FindCommand("quit");
	quit->Dispatch(empty);
#endif // _DEBUG
}

void Uninject()
{
	if (g_bInitialized)
	{
		Log::Msg("Uninjecting\n");
		Hooks::Unhook();
		Features::Misc::Events::Unregister();

		std::thread([] {
			FreeLibrary(g_hModule);
		}).detach();
	}
	else
		Log::Msg(Color::Yellow(), "Wait for a cheat to fully initialize before uninjecting\n");
}

void Init(HINSTANCE hModule)
{
	while (!GetModuleHandle("serverbrowser.dll"))
		std::this_thread::sleep_for(10s);

	srand(time(nullptr));

	g_hModule = hModule;

	Log::Init();
	Signatures::Init();
	I::Init();
	G::Init();
	NetVars::Init();
	Settings::Init();
	Draw::Init(hModule);
	Hooks::Init();
	Features::Misc::Events::Init();

	ConVar* engine_no_focus_sleep = I::Cvar->FindVar("engine_no_focus_sleep");
	if (10 < engine_no_focus_sleep->GetInt())
		engine_no_focus_sleep->SetValue(10);

	I::Cvar->FindVar("name")->m_fnChangeCallbacks.m_Size = 0;

#ifdef _DEBUG
	Log::PrefixedMsg("Initialized. DEBUG Build date: " __DATE__ "\n");
#else
	Log::PrefixedMsg("Initialized. Build date: " __DATE__ "\n");
#endif // _DEBUG

	Settings::ThirdPersonDistChanged(nullptr, nullptr, 0.f);

	g_bInitialized = true;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Init), hModule, 0, 0);
	}
	return TRUE;
}