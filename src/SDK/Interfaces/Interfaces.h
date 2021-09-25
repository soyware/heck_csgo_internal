#pragma once

#include "BaseClient.h"
#include "ClientEntityList.h"
#include "EngineClient.h"

class IPhysicsSurfaceProps;

#include "EngineTrace.h"
#include "ModelRender.h"
#include "RenderView.h"
#include "DebugOverlay.h"
#include "EngineSound.h"
#include "ModelInfo.h"
#include "MemAlloc.h"
#include "Cvar.h"
#include "Panel.h"
#include "Surface.h"
#include "Inputs.h"
#include "KeyValuesSystem.h"
#include "MaterialSystem.h"
#include "MDLCache.h"
#include "GameEvent.h"
#include "Steam.h"

namespace I
{
	IBaseClient*		Client;
	IClientEntityList*	EntityList;
	IVEngineClient*		EngineClient;
	IPhysicsSurfaceProps* PhysProps;
	IEngineTrace*		EngineTrace;
	IVModelRender*		ModelRender;
	IVRenderView*		RenderView;
	IVDebugOverlay*		DebugOverlay;
	IVModelInfo*		ModelInfo;
	IEngineSound*		EngineSound;
	IMemAlloc*			MemAlloc;
	ICvar*				Cvar;
	IVPanel*			Panel;
	ISurface*			Surface;
	//IInputSystem*		InputSystem;
	IInput*				Input;
	IKeyValuesSystem*	KeyValuesSystem;
	IMaterialSystem*	MaterialSystem;
	IMDLCache*			MdlCache;
	IGameEventManager2* GameEventManager;

	typedef void* (* CreateInterfaceFn) (const char*, int*);

	template<class T>
	T Get(const char* Module, const char* Interface)
	{
		return reinterpret_cast<T>(
			reinterpret_cast<CreateInterfaceFn>(GetProcAddress(GetModuleHandle(Module), "CreateInterface"))(Interface, nullptr));
	}

	template <class T>
	T Find(const char* Module, const char* Interface)
	{
		CreateInterfaceFn CreateInterface = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(GetModuleHandle(Module), "CreateInterface"));
		for (unsigned int i = 1; i < 100; ++i)
		{
			char tmpName[28];
			sprintf_s(tmpName, sizeof(tmpName), "%s%03u", Interface, i);
			void* interface = CreateInterface(tmpName, nullptr);
			if (interface)
				return reinterpret_cast<T>(interface);
		}

		FatalError("Interface not found");
		return 0;
	}

	void Init()
	{
		Client			= Find<IBaseClient*>("client.dll", "VClient");
		EntityList		= Find<IClientEntityList*>("client.dll", "VClientEntityList");
		EngineClient	= Find<IVEngineClient*>("engine.dll", "VEngineClient");
		PhysProps		= Find<IPhysicsSurfaceProps*>("vphysics.dll", "VPhysicsSurfaceProps");
		EngineTrace		= Find<IEngineTrace*>("engine.dll", "EngineTraceClient");
		ModelRender		= Find<IVModelRender*>("engine.dll", "VEngineModel");
		RenderView		= Find<IVRenderView*>("engine.dll", "VEngineRenderView");
		DebugOverlay	= Find<IVDebugOverlay*>("engine.dll", "VDebugOverlay");
		ModelInfo		= Find<IVModelInfo*>("engine.dll", "VModelInfoClient");
		EngineSound		= Find<IEngineSound*>("engine.dll", "IEngineSoundClient");
		Cvar			= Find<ICvar*>("vstdlib.dll", "VEngineCvar");
		Panel			= Find<IVPanel*>("vgui2.dll", "VGUI_Panel");
		Surface			= Find<ISurface*>("vguimatsurface.dll", "VGUI_Surface");
		//InputSystem		= Find<IInputSystem*>("inputsystem.dll", "InputSystemVersion");
		Input			= *reinterpret_cast<IInput**>(GetVF<uintptr_t>(Client, 16) + 0x1);
		MemAlloc		= *reinterpret_cast<IMemAlloc**>(GetProcAddress(GetModuleHandle("tier0.dll"), "g_pMemAlloc"));
		KeyValuesSystem	= reinterpret_cast<IKeyValuesSystem* (_cdecl*)()>(GetProcAddress(GetModuleHandle("vstdlib.dll"), "KeyValuesSystem"))();
		MaterialSystem	= Find<IMaterialSystem*>("materialsystem.dll", "VMaterialSystem");
		MdlCache		= Find<IMDLCache*>("datacache.dll", "MDLCache");
		GameEventManager= Get<IGameEventManager2*>("engine.dll", "GAMEEVENTSMANAGER002");
	}
}