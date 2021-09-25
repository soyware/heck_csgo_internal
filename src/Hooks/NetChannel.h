#pragma once

namespace Hooks
{
	extern CVTH hkNetChan;

	DestructorFn oNetChanDestructor;

	void __fastcall NetChanDestructor(INetChannel* _this, void* edx, bool del)
	{
		hkNetChan.Unhook();
		oNetChanDestructor(_this, del);
	}

	typedef bool(__thiscall* SendNetMsgFn)(INetChannel*, INetMessage&, bool, bool);
	SendNetMsgFn oSendNetMsg;

	bool __fastcall SendNetMsg(INetChannel* _this, void* edx, INetMessage& msg, bool bForceReliable = false, bool bVoice = false)
	{
		if (Settings::Misc::BypassSvPure.GetInt() && msg.GetType() == clc_FileCRCCheck)
			return false;

		if (msg.GetGroup() == INetChannel::VOICE)
			bVoice = true;

		return oSendNetMsg(_this, msg, bForceReliable, bVoice);
	}

	typedef int(__thiscall* SendDatagramFn)(INetChannel*, bf_write*);
	SendDatagramFn oSendDatagram;

	int __fastcall SendDatagram(INetChannel* _this, void* edx, bf_write* data)
	{
		if (data || 
			!Settings::Moves::Aim::Backtrack::Enable.GetInt() || 
			!Settings::Moves::Aim::Backtrack::Extend.GetInt())
			return oSendDatagram(_this, data);

		int prevInSeq = _this->m_nInSequenceNr;

		static ConVar* sv_maxunlag = I::Cvar->FindVar("sv_maxunlag");
		float maxBacktrack = sv_maxunlag->GetFloat() / 2.f;
		_this->m_nInSequenceNr += NET_FRAMES_BACKUP * (1.f - maxBacktrack);

		int ret = oSendDatagram(_this, data);

		_this->m_nInSequenceNr = prevInSeq;

		return ret;
	}
}