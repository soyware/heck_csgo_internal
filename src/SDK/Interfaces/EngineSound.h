#pragma once


// other values are possible - 0-255, where 255 is very high
#define	PITCH_NORM		100
#define PITCH_LOW		95
#define PITCH_HIGH		120

#define MAX_SOUND_SEED_BITS		6	// Used to encode 0-63 for seeding client side sound operators
									// NOTE: The LSB also carries the server side wave file selection
#define MIN_SOUND_SEED_VALUE	0
#define MAX_SOUND_SEED_VALUE	((1<<MAX_SOUND_SEED_BITS)-1)

// Handy defines for EmitSound
#define SOUND_FROM_LOCAL_PLAYER		-1
#define SOUND_FROM_WORLD			0

//-----------------------------------------------------------------------------
// channels
//-----------------------------------------------------------------------------
enum
{
	CHAN_REPLACE = -1,
	CHAN_AUTO = 0,
	CHAN_WEAPON = 1,
	CHAN_VOICE = 2,
	CHAN_ITEM = 3,
	CHAN_BODY = 4,
	CHAN_STREAM = 5,		// allocate stream channel from the static or dynamic area
	CHAN_STATIC = 6,		// allocate channel from the static area 
	CHAN_VOICE_BASE = 7,		// allocate channel for network voice data
};

enum soundlevel_t
{
	SNDLVL_NONE = 0,

	SNDLVL_20dB = 20,			// rustling leaves
	SNDLVL_25dB = 25,			// whispering
	SNDLVL_30dB = 30,			// library
	SNDLVL_35dB = 35,
	SNDLVL_40dB = 40,
	SNDLVL_45dB = 45,			// refrigerator

	SNDLVL_50dB = 50,	// 3.9	// average home
	SNDLVL_55dB = 55,	// 3.0

	SNDLVL_IDLE = 60,	// 2.0	
	SNDLVL_60dB = 60,	// 2.0	// normal conversation, clothes dryer

	SNDLVL_65dB = 65,	// 1.5	// washing machine, dishwasher
	SNDLVL_STATIC = 66,	// 1.25

	SNDLVL_70dB = 70,	// 1.0	// car, vacuum cleaner, mixer, electric sewing machine

	SNDLVL_NORM = 75,
	SNDLVL_75dB = 75,	// 0.8	// busy traffic

	SNDLVL_80dB = 80,	// 0.7	// mini-bike, alarm clock, noisy restaurant, office tabulator, outboard motor, passing snowmobile
	SNDLVL_TALKING = 80,	// 0.7
	SNDLVL_85dB = 85,	// 0.6	// average factory, electric shaver
	SNDLVL_90dB = 90,	// 0.5	// screaming child, passing motorcycle, convertible ride on frw
	SNDLVL_95dB = 95,
	SNDLVL_100dB = 100,	// 0.4	// subway train, diesel truck, woodworking shop, pneumatic drill, boiler shop, jackhammer
	SNDLVL_105dB = 105,			// helicopter, power mower
	SNDLVL_110dB = 110,			// snowmobile drvrs seat, inboard motorboat, sandblasting
	SNDLVL_120dB = 120,			// auto horn, propeller aircraft
	SNDLVL_130dB = 130,			// air raid siren

	SNDLVL_GUNFIRE = 140,	// 0.27	// THRESHOLD OF PAIN, gunshot, jet engine
	SNDLVL_140dB = 140,	// 0.2

	SNDLVL_150dB = 150,	// 0.2

	SNDLVL_180dB = 180,			// rocket launching

	// NOTE: Valid soundlevel_t values are 0-255.
	//       256-511 are reserved for sounds using goldsrc compatibility attenuation.
};

typedef unsigned int HSOUNDSCRIPTHASH;
#define SOUNDEMITTER_INVALID_HASH	(HSOUNDSCRIPTHASH)-1

//-----------------------------------------------------------------------------
// Flags to be or-ed together for the iFlags field
//-----------------------------------------------------------------------------
enum SoundFlags_t
{
	SND_NOFLAGS = 0,			// to keep the compiler happy
	SND_CHANGE_VOL = (1 << 0),		// change sound vol
	SND_CHANGE_PITCH = (1 << 1),		// change sound pitch
	SND_STOP = (1 << 2),		// stop the sound
	SND_SPAWNING = (1 << 3),		// we're spawning, used in some cases for ambients
												// not sent over net, only a param between dll and server
												SND_DELAY = (1 << 4),		// sound has an initial delay
												SND_STOP_LOOPING = (1 << 5),		// stop all looping sounds on the entity
												SND_SPEAKER = (1 << 6),		// being played again by a microphone through a speaker

												SND_SHOULDPAUSE = (1 << 7),		// this sound should be paused if the game is paused
												SND_IGNORE_PHONEMES = (1 << 8),
												SND_IGNORE_NAME = (1 << 9),		// used to change all sounds emitted by an entity, regardless of scriptname
												SND_IS_SCRIPTHANDLE = (1 << 10),		// server has passed the actual SoundEntry instead of wave filename

												SND_UPDATE_DELAY_FOR_CHOREO = (1 << 11),		// True if we have to update snd_delay_for_choreo with the IO latency
												SND_GENERATE_GUID = (1 << 12),		// True if we generate the GUID when we send the sound.

												SND_OVERRIDE_PITCH = (1 << 13),		// The pitch given in code overrides what is present in the sound entry
};

class KeyValues;

struct CSoundParameters
{
	CSoundParameters()
	{
		channel = CHAN_AUTO; // 0
		volume = 1.f;
		pitch = PITCH_NORM; // 100

		pitchlow = PITCH_NORM;
		pitchhigh = PITCH_NORM;

		soundlevel = SNDLVL_NORM; // 75dB
		soundname[0] = 0;
		play_to_owner_only = false;
		count = 0;

		delay_msec = 0;

		m_nSoundEntryVersion = 1;
		m_hSoundScriptHash = SOUNDEMITTER_INVALID_HASH;
		m_pOperatorsKV = nullptr;
		m_nRandomSeed = -1;

		m_bHRTFFollowEntity = false;
		m_bHRTFBilinear = false;
	}

	int				channel;
	float			volume;
	int				pitch;
	int				pitchlow, pitchhigh;
	soundlevel_t	soundlevel;
	// For weapon sounds...
	bool			play_to_owner_only;
	int				count;
	char			soundname[128];
	int				delay_msec;
	HSOUNDSCRIPTHASH m_hSoundScriptHash;
	int				m_nSoundEntryVersion;
	KeyValues*		m_pOperatorsKV;
	int				m_nRandomSeed;

	bool			m_bHRTFFollowEntity;
	bool			m_bHRTFBilinear;
};

class IRecipientFilter;

class IEngineSound
{
public:

};