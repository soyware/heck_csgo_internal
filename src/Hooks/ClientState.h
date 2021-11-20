#pragma once

namespace Hooks
{
	class netpacket_t;

	typedef bool(__thiscall* ProcessConnectionlessPacketFn)(IVEngineClient*, netpacket_t*);
	ProcessConnectionlessPacketFn oProcessConnectionlessPacket;

	bool __fastcall ProcessConnectionlessPacket(IVEngineClient* _this, void* edx, netpacket_t* packet)
	{
		if (Settings::Misc::ServerLagger::Enable.GetInt() && I::EngineClient->IsInGame())
			return true;

		return oProcessConnectionlessPacket(_this, packet);
	}
}