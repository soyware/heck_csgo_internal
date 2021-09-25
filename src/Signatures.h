#pragma once

inline uintptr_t RelativeToDirect(const uintptr_t address, const uint8_t nextInstOfs = 4)
{
	return address + *reinterpret_cast<int32_t*>(address) + nextInstOfs;
}

namespace Signatures
{
	// Credits: Forza
	void FindLongestSegment(const char* Mask, size_t Out[2])
	{
		size_t start = 0,
			len = 0,
			maskLen = strlen(Mask);

		for (size_t i = len; i < maskLen; ++i)
		{
			if (Mask[i] != 'x')
				continue;

			size_t count = 0;
			for (size_t j = i; j < maskLen && Mask[j] == 'x'; ++j)
				++count;

			if (count > len)
			{
				start = i;
				len = count;
			}

			i += (count - 1);
		}

		Out[0] = start;
		Out[1] = len;
	}

	uintptr_t Find(const char* Module, const char* Signature, const char* Mask, const ptrdiff_t Offset = 0)
	{
		MODULEINFO modInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(Module), &modInfo, sizeof(MODULEINFO));

		size_t longest[2];
		FindLongestSegment(Mask, longest);

		const uint8_t* data = static_cast<uint8_t*>(modInfo.lpBaseOfDll);

		const uint8_t maskLen = static_cast<uint8_t>(strlen(Mask)),
			segStart = static_cast<uint8_t>(longest[0]),
			segLen = static_cast<uint8_t>(longest[1]),
			firstChar = static_cast<uint8_t>(Signature[segStart]);

		bool inPattern[UCHAR_MAX + 1] = { false };

		for (uint8_t i = segStart; i < (segStart + segLen); ++i)
			inPattern[(uint8_t)Signature[i]] = true;

		for (size_t i = modInfo.SizeOfImage - maskLen; i > 0; --i)
		{
			bool in = inPattern[data[i]];
			while (!in && i > segLen)
			{
				i -= segLen;
				in = inPattern[data[i]];
			}

			if (data[i] != firstChar)
				continue;

			if ((segStart > i) || (i - segStart + maskLen > modInfo.SizeOfImage))
				break;

			for (uint8_t j = 0; j < maskLen; ++j)
			{
				if (j == segStart || Mask[j] != 'x')
					continue;

				if (data[i - segStart + j] != (uint8_t)Signature[j])
					break;

				if (j + 1 == maskLen)
					return Offset ? *(uintptr_t*)(data + i - segStart + Offset) : (uintptr_t)(data + i - segStart);
			}
		}

		FatalError("Signature not found");
		return 0;
	}

	uintptr_t KeyValuesConstructor,
		KeyValuesLoadFromBuffer,
		DrawCrosshair_weapon_debug_spread_show,
		CTraceFilterSimpleVT,
		CTraceFilterSkipTwoEntitiesVT,
		//TraceToStudio,
		//UpdateClientSideAnimations,
		//StartLagCompensation,
		SetAbsOrigin,
		SetAbsAngles,
		g_iModelBoneCounter,
		SetCollisionBounds,
		SetLocalPlayerReady,
		GetSequenceActivity,
		g_PR,
		SendClanTag,
		//GetSequenceActivityName,
		//LookupSequence,
		//CStudioHdrConstructor,
		SelectWeightedSequence,
		__MsgFunc_ServerRankRevealAll,
		LineGoesThroughSmoke,
		g_nSmokeCount,
		RenderLine,
		RenderBoxInternal,
		RenderWireframeBox,
		CAM_Think_sv_cheats,
		AddToNotify_developer,
		DrawNotify_developer,
		Con_ColorPrint_developer,
		SendCrosshairCode_AllocKeyValuesMemory,
		IsLoadoutAllowed_IsPlayingDemo,
		Voice_RecordStart,
		CheckReceivingList_net_showfragments,
		ReadSubChannelData_net_showfragments,
		C_HLTVCamera__Update_GetDemoPlaybackParameters,
		CCStrike15BasePanel__OnEvent_GetDemoPlaybackParameters,
		CClientLeafSystem__InsertIntoTree_ListLeavesInBox
		/*NET_SendPacket,
		SpewLimit
		NewSignonMsg,
		ClearSignonMsg,
		ClientState*/;

	void Init()
	{
		// "PerformSystemConfiguration: Missing %s"
		uintptr_t addrReadEncryptedKVFile		= Find("client.dll", "\x83\xC0\x04\xEB\x02\x33\xC0\x51\x6A\x00\x6A\x00\x50\x53\x57\x8B\xCE\xE8", "xxxxxxxxxxxxxxxxxx");
		KeyValuesConstructor					= RelativeToDirect(addrReadEncryptedKVFile - 19);
		KeyValuesLoadFromBuffer					= RelativeToDirect(addrReadEncryptedKVFile + 18);
		DrawCrosshair_weapon_debug_spread_show	= Find("client.dll", "\xFF\xD0\x85\xC0\x0F\x84\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x99", "xxxxxx????x????x") + 2;
		CTraceFilterSimpleVT					= Find("client.dll", "\xC7\x44\x24\x2C\x00\x00\x00\x00\x8D\x54\x24\x3C\x89\x44\x24\x34", "xxxx????xxxxxxxx", 4);
		CTraceFilterSkipTwoEntitiesVT			= Find("client.dll", "\xC7\x45\xD4\x00\x00\x00\x00\x89\x45\xE4\x8B\x01\x68\x0B\x40\x00\x46", "xxx????xxxxxxxxxx", 3);
		//TraceToStudio							= Find("client.dll", "\x55\x8B\xEC\x83\xE4\xF0\x81\xEC\x88\x03\x00\x00\x8B\xC2\x56\x57", "xxxxxxxxxxxxxxxx");
		//UpdateClientSideAnimations			= Find("client.dll", "\x8B\x0D\x00\x00\x00\x00\x53\x56\x57\x8B\x99\x0C\x10\x00\x00\x85\xDB\x74\x1C", "xx????xxxxxxxxxxxxx");
		//StartLagCompensation					= Find("server.dll", "\xF3\x0F\x5D\xD9\x8B\x44\x24\x10\x0F\x28\xCB\x8B\x15", "xxxxxxxxxxxxx");
		SetAbsOrigin							= Find("client.dll", "\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1\xE8\x00\x00\x00\x00\x8B\x7D\x08", "xxxxxxxxxxxxx????xxx");
		SetAbsAngles							= Find("client.dll", "\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1", "xxxxxxxxxxxxxx");
		g_iModelBoneCounter						= Find("client.dll", "\x8B\x0D\x00\x00\x00\x00\x49\xC7\x80\x00\x00\x00\x00\xFF\xFF\x7F\xFF\x89\x88", "xx????xxx????xxxxxx", 2);
		SetCollisionBounds						= Find("client.dll", "\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x83\xEC\x18\x56\x57\x8B\x7B\x08", "xxxxxxxxxxxxxxxxxx") - 12;
		SetLocalPlayerReady						= Find("client.dll", "\x55\x8B\xEC\x83\xE4\xF8\x8B\x4D\x08\xBA\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x75\x12", "xxxxxxxxxx????x????xxxx");
		GetSequenceActivity						= Find("client.dll", "\x53\x8B\xD9\x56\x57\x8B\xFA\x83\x7B\x04\x00\x75\x22\x8B\x03\x8B\xCF", "xxxxxxxxxxxxxxxxx");
		// "weapon_c4"
		g_PR									= Find("client.dll", "\xA1\x00\x00\x00\x00\x57\x85\xC0\x74\x08\x8D\xB8\x00\x00\x00\x00\xEB\x02", "x????xxxxxxx????xx", 1);
		SendClanTag								= Find("engine.dll", "\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8", "xxxxxxxxx????xxxx");
		//GetSequenceActivityName					= Find("client.dll", "\x8B\x40\x14\x3B\xD0\x7D\x0A\x52\xE8\x00\x00\x00\x00\x03\x40\x08\xC3", "xxxxxxxxx????xxxx") - 25;
		//LookupSequence							= Find("client.dll", "\x51\x56\x8B\xF1\x57\x8B\xFA\x85\xF6\x75\x06\x33\xC0\x5F\x5E\x59", "xxxxxxxxxxxxxxxx");
		//CStudioHdrConstructor					= Find("client.dll", "\x55\x8B\xEC\x56\x8B\xF1\xC7\x46\x0C\x00\x00\x00\x00\xC7\x46\x10", "xxxxxxxxxxxxxxxx");
		SelectWeightedSequence					= Find("client.dll", "\x51\xA1\x00\x00\x00\x00\x53\x8B\xDA\x56\x8B\xF1\x57\xA8\x01\x75\x2D", "xx????xxxxxxxxxxx");
		__MsgFunc_ServerRankRevealAll			= Find("client.dll", "\x83\xEC\x08\x8D\x4E\x74\x8B\xC4\xC7\x40\x04\x00\x00\x00\x00\xC7\x00", "xxxxxxxxxxx????xx", 17);
		LineGoesThroughSmoke					= Find("client.dll", "\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0\x56\x57\x33\xFF", "xxxxxxxx????xxxxxxx");
		g_nSmokeCount							= *reinterpret_cast<uintptr_t*>(LineGoesThroughSmoke + 8);
		RenderLine								= Find("engine.dll", "\x55\x8B\xEC\x83\xE4\xF8\x81\xEC\x14\x02\x00\x00\x53\x56\x57\xE8", "xxxxxxxxxxxxxxxx");
		RenderBoxInternal						= RelativeToDirect(Find("engine.dll", "\x55\x8B\xEC\x56\x57\x8B\xF2\x8B\xF9\xE8\x00\x00\x00\x00\x6A\x00\xFF", "xxxxxxxxxx????xxx") + 34);
		RenderWireframeBox						= RelativeToDirect(Find("engine.dll", "\x8D\x47\x2C\x50\x8D\x47\x20\x50\x8D\x47\x38\x50\x8D\x47\x14\x50\xE8", "xxxxxxxxxxxxxxxxx") + 17);
		CAM_Think_sv_cheats						= Find("client.dll", "\x85\xC0\x75\x30\x38\x86\xAD\x00\x00\x00\x74\x28\x8B\x0D", "xxxxxxxxxxxxxx");
		AddToNotify_developer					= Find("engine.dll", "\x55\x8B\xEC\x81\xEC\x10\x08\x00\x00\x80\x3D\x00\x00\x00\x00\x00", "xxxxxxxxxxx????x") + 45;
		DrawNotify_developer					= Find("engine.dll", "\x83\xBF\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xFF\x50\x34", "xx??xxxxx????x????x????xxx") + 26;
		Con_ColorPrint_developer				= Find("engine.dll", "\x8A\x3D\x00\x00\x00\x00\x33\xF6\x8B\x0D\x00\x00\x00\x00\x89\x45\xF4\x88\x7D\xFE", "xx????xxxx????xxxxxx");
		SendCrosshairCode_AllocKeyValuesMemory	= Find("client.dll", "\x74\x7E\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x10\xFF\x52\x04", "xxxx????xxxxxxxxx") + 17;
		IsLoadoutAllowed_IsPlayingDemo			= Find("client.dll", "\x8B\x01\x8B\x80\x48\x01\x00\x00\xFF\xD0\x84\xC0\x75\x12\xE8", "xxxxxxxxxxxxxxx") + 10;
		// "voice_decompressed.wav"
		Voice_RecordStart						= Find("engine.dll", "\x55\x8B\xEC\x83\xEC\x0C\x83\x3D\x00\x00\x00\x00\x00\x56\x57\x8B\xF2\x8B\xF9", "xxxxxxxx????xxxxxxx");
		CheckReceivingList_net_showfragments	= Find("engine.dll", "\xFF\x50\x34\x8B\x1D\x00\x00\x00\x00\x85\xC0\x74\x16\xFF\xB6", "xxxxx????xxxxxx") + 3;
		ReadSubChannelData_net_showfragments	= Find("engine.dll", "\xFF\x50\x34\x85\xC0\x74\x12\x53\xFF\x75\x0C\x68", "xxxxxxxxxxxx") + 3;
		C_HLTVCamera__Update_GetDemoPlaybackParameters = Find("client.dll", "\x8B\xF8\x89\x7D\xF8\x85\xFF\x0F\x84\x00\x00\x00\x00\x83\x3D", "xxxxxxxxx????xx");
		CCStrike15BasePanel__OnEvent_GetDemoPlaybackParameters = Find("client.dll", "\x8B\xC8\x85\xC9\x74\x1F\x80\x79\x10\x00\x74\x19\x83\x79\x0C\x00", "xxxxxxxxxxxxxxxx");
		CClientLeafSystem__InsertIntoTree_ListLeavesInBox = Find("client.dll", "\xFF\x50\x18\x89\x44\x24\x14\xEB\x08", "xxxxxxxxx") + 3;
		//NET_SendPacket							= Find("engine.dll", "\x55\x8B\xEC\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x53\x8B\xD9\x56\x57\x8B\xFA", "xxxx??xxx????xxxxxxx");
		//SpewLimit								= Find("steamnetworkingsockets.dll", "\x55\x8B\xEC\xA1\x00\x00\x00\x00\x85\xC0\x7F\x24", "xxxx????xxxx", 4);
		//NewSignonMsg							= Find("engine.dll", "\x55\x8B\xEC\x56\x57\x8B\xF9\x8D\x4F\x04", "xxxxxxxxxx");
		//ClearSignonMsg						= Find("engine.dll", "\x53\x56\x57\x8B\xF9\x8D\x77\x38", "xxxxxxxx");
		//ClientState							= **reinterpret_cast<BYTE***>(Find("engine.dll", "\x8B\x3D\x00\x00\x00\x00\x8A\xF9\xF3\x0F\x11\x45", "xx????xxxxxx") + 2);
	}
}