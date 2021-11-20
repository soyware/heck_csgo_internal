#pragma once

namespace Hooks
{
	typedef void(__thiscall* DestructorFn)(void*, bool);
}

#include "Panel.h"
#include "ConVars.h"
#include "NetChannel.h"
#include "ClientMode.h"
#include "ModelRender.h"
#include "Client.h"
#include "EngineSound.h"
#include "RenderView.h"
#include "BSPTree.h"
#include "ClientState.h"
#include "EngineClient.h"

namespace Hooks
{
	CVTH hkClientMode;
	CVTH hkPanel;
	CVTH hkClient;
	CVTH hkModelRender;
	CVTH hkEmitSound;
	CVTH hkRenderView;
	CVTH hkToolBSPTree;
	CVTH hkClientState;
	CVTH hkEngineClient;
	CVTH hkNetChan;

	CVTH hk_sv_cheats;
	CVTH hk_weapon_debug_spread_show;
	CVTH hk_developer;
	CVTH hk_net_showfragments;

	void Init()
	{
		hkClientMode.Init(G::ClientMode, "client.dll");
		hkClientMode.Hook(17, &ShouldDrawFog);
		oOverrideView = hkClientMode.Hook<OverrideViewFn>(18, &OverrideView);
		oKeyInput = hkClientMode.Hook<KeyInputFn>(20, &KeyInput);
		hkClientMode.Hook(24, &CreateMove);
		oDoPostScreenSpaceEffects = hkClientMode.Hook<DoPostScreenSpaceEffectsFn>(44, &DoPostScreenSpaceEffects);

		hkPanel.Init(I::Panel, "vgui2.dll");
		oPaintTraverse = hkPanel.Hook<PaintTraverseFn>(41, &PaintTraverse);

		hkClient.Init(I::Client, "client.dll");
		oFrameStageNotify = hkClient.Hook<FrameStageNotifyFn>(37, &FrameStageNotify);
		oGetRichPresenceStatusString = hkClient.Hook<GetRichPresenceStatusStringFn>(107, &GetRichPresenceStatusString);

		hkModelRender.Init(I::ModelRender, "engine.dll");
		oDrawModelExecute = hkModelRender.Hook<DrawModelExecuteFn>(21, &DrawModelExecute);

		hkEmitSound.Init(I::EngineSound, "engine.dll");
		oEmitSound = hkEmitSound.Hook<EmitSoundFn>(5, &EmitSound);

		hkRenderView.Init(I::RenderView, "engine.dll");
		oDraw3DDebugOverlays = hkRenderView.Hook<Draw3DDebugOverlaysFn>(3, &Draw3DDebugOverlays);

		hkToolBSPTree.Init(I::EngineClient->GetBSPTreeQuery(), "engine.dll");
		oListLeavesInBox = hkToolBSPTree.Hook<ListLeavesInBoxFn>(6, &ListLeavesInBox);

		void* ClientState = reinterpret_cast<void*>(**reinterpret_cast<uintptr_t**>(GetVF<uintptr_t>(I::EngineClient, 7) + 0x4) + 0x8);
		// 0x30AC skip until voice chat related buffers
		// 0x158F8 is the size of the voice chat related struct there are 5 of them
		hkClientState.Init(ClientState, "engine.dll", 0x30AC + (0x158F8 * 5) + 0x710 + 0x2800);
		// not sure if needed for a server lagger, maybe when crashing out of spectators
		oProcessConnectionlessPacket = hkClientState.Hook<ProcessConnectionlessPacketFn>(1, &ProcessConnectionlessPacket);

		// sv_cheats hook placed here because it can fit the empty space
		hk_sv_cheats.Init(I::Cvar->FindVar("sv_cheats"), "engine.dll");
		o_sv_cheats_GetInt = hk_sv_cheats.Hook<GetIntFn>(13, &sv_cheats_GetInt);

		hkEngineClient.Init(I::EngineClient, "engine.dll", 0xF300);
		oIsPlayingDemo = hkEngineClient.Hook<IsPlayingDemoFn>(82, &IsPlayingDemo);
		oServerCmdKeyValues = hkEngineClient.Hook<ServerCmdKeyValuesFn>(187, &ServerCmdKeyValues);
		oGetDemoPlaybackParameters = hkEngineClient.Hook<GetDemoPlaybackParametersFn>(218, &GetDemoPlaybackParameters);


		hk_weapon_debug_spread_show.Init(I::Cvar->FindVar("weapon_debug_spread_show"));
		o_weapon_debug_spread_show_GetInt = hk_weapon_debug_spread_show.Hook<GetIntFn>(13, &weapon_debug_spread_show_GetInt);

		hk_developer.Init(I::Cvar->FindVar("developer"));
		o_developer_GetInt = hk_developer.Hook<GetIntFn>(13, &developer_GetInt);

		hk_net_showfragments.Init(I::Cvar->FindVar("net_showfragments"));
		o_net_showfragments_GetInt = hk_net_showfragments.Hook<GetIntFn>(13, &net_showfragments_GetInt);

		Features::Visuals::hkDidSmokeEffect.Hook("DT_SmokeGrenadeProjectile", "m_bDidSmokeEffect", &Features::Visuals::RecvProxy_DidSmokeEffect);
#if KNIFE_ANIM_FIX
		Features::Misc::SkinChanger::hkSequence.Hook("DT_BaseViewModel", "m_nSequence", &Features::Misc::SkinChanger::RecvProxy_Sequence);
#endif // KNIFE_ANIM_FIX
	}

	void HookNetChan()
	{
		hkNetChan.Init(I::EngineClient->GetNetChannelInfo());
		oNetChanDestructor = hkNetChan.Hook<DestructorFn>(27, &NetChanDestructor);
		oSendNetMsg = hkNetChan.Hook<SendNetMsgFn>(40, &SendNetMsg);
		oSendDatagram = hkNetChan.Hook<SendDatagramFn>(46, &SendDatagram);
	}

	void Unhook()
	{
		hkClientMode.Unhook();
		hkPanel.Unhook();
		hkClient.Unhook();
		hkModelRender.Unhook();
		hkEmitSound.Unhook();
		hkRenderView.Unhook();
		hkToolBSPTree.Unhook();
		hkClientState.Unhook();
		hkEngineClient.Unhook();
		hkNetChan.Unhook();

		hk_sv_cheats.Unhook();
		hk_weapon_debug_spread_show.Unhook();
		hk_developer.Unhook();
		hk_net_showfragments.Unhook();

		Features::Visuals::hkDidSmokeEffect.Unhook();
#if KNIFE_ANIM_FIX
		Features::Misc::SkinChanger::hkSequence.Unhook();
#endif // KNIFE_ANIM_FIX
	}
}