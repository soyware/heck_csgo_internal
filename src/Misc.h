#pragma once

extern void FatalError(const char* err);
extern void Uninject();

template <class T>
inline T GetVF(void* instance, int index)
{
	return (*reinterpret_cast<T**>(instance))[index];
}

void LoadResourceFont(HINSTANCE hModule, int resource)
{
	HRSRC res;
    HGLOBAL mem;
    LPVOID data;
    size_t len;
    DWORD outNum;
    HANDLE font;

    if (!(res = FindResource(hModule, MAKEINTRESOURCE(resource), "TTFFONT")) ||
        !(mem = LoadResource(hModule, res)) ||
        !(data = LockResource(mem)) ||
        !(len = SizeofResource(hModule, res)) ||
        !(font = AddFontMemResourceEx(data, len, 0, &outNum)))
    {
        FatalError("Font loading failed");
    }
}

inline int RandomInt(int min, int max)
{
	typedef int(* Fn)(int, int);
	static const Fn func = reinterpret_cast<Fn>(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomInt"));
	return func(min, max);
}

inline float RandomFloat(float min, float max)
{
	typedef float(* Fn)(float, float);
	static const Fn func = reinterpret_cast<Fn>(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return func(min, max);
}

inline void RandomSeed(int seed)
{
	typedef void(* Fn)(int);
	static const Fn func = reinterpret_cast<Fn>(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomSeed"));
	func(seed);
}

struct WAV_HEADER
{
    // riff chunk
    uint8_t     RIFF[4];
    uint32_t    riffSize;
    uint8_t     WAVE[4];

    // subchunk 1
    uint8_t     fmt[4];
    uint32_t    fmtSize;
    uint16_t    audioFormat;    // Audio format 1=PCM, 6=mulaw, 7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t    channelsNum;    // Number of channels 1=Mono, 2=Stereo
    uint32_t    samplesPerSec;  // Sampling frequency in Hz
    uint32_t    bytesPerSec;    // Bytes per second
    uint16_t    blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t    bitsPerSample;  // Number of bits per sample

    // subchunk 2
    uint8_t     data[4];
    uint32_t    dataSize;
    // data
};

uint32_t GetWaveLength(const char* path)
{
    FILE* file;
    fopen_s(&file, path, "rb");
    if (!file)
        return 0;

    WAV_HEADER header;
    if (fread(&header, sizeof(uint8_t), sizeof(WAV_HEADER), file) != sizeof(WAV_HEADER))
        return 0;

    fclose(file);

    return (header.dataSize * 1000 / header.bytesPerSec);
}