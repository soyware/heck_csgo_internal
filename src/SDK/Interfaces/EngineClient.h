#pragma once

#define MAX_PLAYER_NAME_LENGTH 128
#define SIGNED_GUID_LEN 32
#define MAX_CUSTOM_FILES 4

typedef unsigned long CRC32_t;

struct player_info_t
{
	// version for future compatibility
	uint64_t		version;
	// network xuid
	uint64_t		xuid;
	// scoreboard information
	char			name[MAX_PLAYER_NAME_LENGTH];
	// local server user ID, unique while server is running
	int				userID;
	// global unique player identifer
	char			guid[SIGNED_GUID_LEN + 1];
	// friends identification number
	uint32_t		friendsID;
	// friends name
	char			friendsName[MAX_PLAYER_NAME_LENGTH];
	// true, if player is a bot controlled by game.dll
	bool			fakeplayer;
	// true if player is the HLTV proxy
	bool			ishltv;
	// true if player is the Replay proxy
	bool			isreplay;
	// custom files CRC for this player
	CRC32_t			customFiles[MAX_CUSTOM_FILES];
	// this counter increases each time the server downloaded a new file
	unsigned char	filesDownloaded;
};

#define	FRAG_NORMAL_STREAM	0
#define FRAG_FILE_STREAM	1
#define MAX_STREAMS			2    

#define FLOW_OUTGOING	0		
#define FLOW_INCOMING	1
#define MAX_FLOWS		2		// in & out

#define NET_FRAMES_BACKUP	128	// must be power of 2
#define NET_FRAMES_MASK		(NET_FRAMES_BACKUP-1)

typedef void* FileHandle_t;

typedef struct dataFragments_s
{
	FileHandle_t	file;			// open file handle
	char			filename[MAX_PATH]; // filename
	char*			buffer;			// if NULL it's a file
	unsigned int	bytes;			// size in bytes
	unsigned int	bits;			// size in bits
	unsigned int	transferID;		// only for files
	bool			isCompressed;	// true if data is bzip compressed
	unsigned int	nUncompressedSize; // full size in bytes
	bool			asTCP;			// send as TCP stream
	bool			isReplayDemo;	// if it's a file, is it a replay .dem file?
	int				numFragments;	// number of total fragments
	int				ackedFragments; // number of fragments send & acknowledged
	int				pendingFragments; // number of fragments send, but not acknowledged yet
} dataFragments_t;

class INetChannel
{
public:
	enum
	{
		GENERIC = 0,	// must be first and is default group
		LOCALPLAYER,	// bytes for local player entity update
		OTHERPLAYERS,	// bytes for other players update
		ENTITIES,		// all other entity bytes
		SOUNDS,			// game sounds
		EVENTS,			// event messages
		TEMPENTS,		// temp entities
		USERMESSAGES,	// user messages
		ENTMESSAGES,	// entity messages
		VOICE,			// voice data
		STRINGTABLE,	// a stringtable update
		MOVE,			// client move cmds
		STRINGCMD,		// string command
		SIGNON,			// various signondata
		PAINTMAP,		// paintmap data
		ENCRYPTED,		// encrypted data
		TOTAL,			// must be last and is not a real group
	};

	inline float GetLatency(int flow)
	{
		typedef float(__thiscall* Fn) (INetChannel*, int);
		return GetVF<Fn>(this, 9)(this, flow);
	}

	inline float GetAvgLatency(int flow)
	{
		typedef float(__thiscall* Fn) (INetChannel*, int);
		return GetVF<Fn>(this, 10)(this, flow);
	}

	inline int GetMaxRoutablePayloadSize()
	{
		typedef int(__thiscall* Fn) (INetChannel*);
		return GetVF<Fn>(this, 69)(this);
	}

	// on top of "Net message exceeds max size (%u / %u)\n" reference
	inline dataFragments_t& GetReceiveList(int stream)
	{
		return reinterpret_cast<dataFragments_t*>(reinterpret_cast<uintptr_t>(this) + 0x150)[stream];
	}

	void*		vt;

	// CNetMessageBinder
	void*		m_NETMsgNOP;
	void*		m_NETMsgDisconnect;
	void*		m_NETMsgFile;
	void*		m_NETMsgSplitScreenUser;

	bool		m_bProcessingMessages;
	bool		m_bShouldDelete;
	bool		m_bStopProcessing;

	// last send outgoing sequence number
	int			m_nOutSequenceNr;
	// last received incoming sequnec number
	int			m_nInSequenceNr;
	// last received acknowledge outgoing sequnce number
	int			m_nOutSequenceNrAck;

	// state of outgoing reliable data (0/1) flip flop used for loss detection
	int			m_nOutReliableState;
	// state of incoming reliable data
	int			m_nInReliableState;

	int			m_nChokedPackets;	//number of choked packets

	// ...
};

enum NET_Messages
{
	net_NOP = 0,
	net_Disconnect = 1,
	net_File = 2,
	net_SplitScreenUser = 3,
	net_Tick = 4,
	net_StringCmd = 5,
	net_SetConVar = 6,
	net_SignonState = 7,
	net_PlayerAvatarData = 100,
};

enum CLC_Messages
{
	clc_ClientInfo = 8,
	clc_Move = 9,
	clc_VoiceData = 10,
	clc_BaselineAck = 11,
	clc_ListenEvents = 12,
	clc_RespondCvarValue = 13,
	clc_FileCRCCheck = 14,
	clc_LoadingProgress = 15,
	clc_SplitPlayerConnect = 16,
	clc_ClientMessage = 17,
	clc_CmdKeyValues = 18,
	clc_HltvReplay = 20,
};

enum SVC_Messages
{
	svc_ServerInfo = 8,
	svc_SendTable = 9,
	svc_ClassInfo = 10,
	svc_SetPause = 11,
	svc_CreateStringTable = 12,
	svc_UpdateStringTable = 13,
	svc_VoiceInit = 14,
	svc_VoiceData = 15,
	svc_Print = 16,
	svc_Sounds = 17,
	svc_SetView = 18,
	svc_FixAngle = 19,
	svc_CrosshairAngle = 20,
	svc_BSPDecal = 21,
	svc_SplitScreen = 22,
	svc_UserMessage = 23,
	svc_EntityMessage = 24,
	svc_GameEvent = 25,
	svc_PacketEntities = 26,
	svc_TempEntities = 27,
	svc_Prefetch = 28,
	svc_Menu = 29,
	svc_GameEventList = 30,
	svc_GetCvarValue = 31,
	svc_PaintmapData = 33,
	svc_CmdKeyValues = 34,
	svc_EncryptedData = 35,
	svc_HltvReplay = 36,
	svc_Broadcast_Command = 38,
};

class INetMessage
{
public:
	virtual				~INetMessage() {};

	// Use these to setup who can hear whose voice.
	// Pass in client indices (which are their ent indices - 1).

	virtual void		SetNetChannel(INetChannel* netchan) const = 0;
	virtual void		SetReliable(bool state) = 0;	// set to true if it's a reliable message

	virtual bool		Process() const = 0;

	virtual	bool		ReadFromBuffer(bf_read& buffer) = 0; // returns true if parsing was OK
	virtual	bool		WriteToBuffer(bf_write& buffer) const = 0;	// returns true if writing was OK

	virtual bool		IsReliable() const = 0;  // true, if message needs reliable handling

	virtual int			GetType() const = 0; // returns module specific header tag eg svc_serverinfo
	virtual int			GetGroup() const = 0;	// returns net message group of this message
	virtual const char*	GetName() const = 0;	// returns network message name, eg "svc_serverinfo"

	virtual INetChannel* GetNetChannel() const = 0;

	virtual const char*	ToString() const = 0; // returns a human readable string about message content
	virtual size_t		GetSize() const = 0;
};

class ISpatialQuery;

class IVEngineClient
{
public:
	inline void GetScreenSize(int &width, int &height)
	{
		typedef void(__thiscall* Fn) (IVEngineClient*, int&, int&);
		GetVF<Fn>(this, 5)(this, width, height);
	}

	inline bool GetPlayerInfo(int EntNum, player_info_t* pInfo)
	{
		typedef bool(__thiscall* Fn) (IVEngineClient*, int, player_info_t*);
		return GetVF<Fn>(this, 8)(this, EntNum, pInfo);
	}

	inline int GetPlayerForUserID(int userID)
	{
		typedef int(__thiscall* Fn) (IVEngineClient*, int);
		return GetVF<Fn>(this, 9)(this, userID);
	}

	inline bool Con_IsVisible()
	{
		typedef bool(__thiscall* Fn) (IVEngineClient*);
		return GetVF<Fn>(this, 11)(this);
	}

	inline int GetLocalPlayer()
	{
		typedef int(__thiscall* Fn) (IVEngineClient*);
		return GetVF<Fn>(this, 12)(this);
	}

	inline void GetViewAngles(vec3& vAngles)
	{
		typedef void(__thiscall* Fn)(IVEngineClient*, vec3&);
		GetVF<Fn>(this, 18)(this, vAngles);
	}

	inline void SetViewAngles(vec3& vAngles)
	{
		typedef void(__thiscall* Fn)(IVEngineClient*, vec3&);
		GetVF<Fn>(this, 19)(this, vAngles);
	}

	inline bool IsInGame()
	{
		typedef bool(__thiscall* Fn) (IVEngineClient*);
		return GetVF<Fn>(this, 26)(this);
	}

	inline const char* GetGameDirectory()
	{
		typedef const char*(__thiscall* Fn) (IVEngineClient*);
		return GetVF<Fn>(this, 36)(this);
	}

	inline const VMatrix& WorldToScreenMatrix()
	{
		typedef const VMatrix& (__thiscall* Fn) (IVEngineClient*);
		return GetVF<Fn>(this, 37)(this);
	}

	inline ISpatialQuery* GetBSPTreeQuery()
	{
		typedef ISpatialQuery* (__thiscall* Fn) (IVEngineClient*);
		return GetVF<Fn>(this, 43)(this);
	}

	inline INetChannel* GetNetChannelInfo()
	{
		typedef INetChannel*(__thiscall* Fn) (IVEngineClient*);
		return GetVF<Fn>(this, 78)(this);
	}

	inline unsigned int GetEngineBuildNumber()
	{
		typedef unsigned int(__thiscall* Fn) (IVEngineClient*);
		return GetVF<Fn>(this, 104)(this);
	}

	inline void ClientCmd_Unrestricted(const char* szCmdString, bool fromConsoleOrKeybind = false)
	{
		typedef void(__thiscall* Fn) (IVEngineClient*, const char*, bool);
		GetVF<Fn>(this, 114)(this, szCmdString, fromConsoleOrKeybind);
	}
};