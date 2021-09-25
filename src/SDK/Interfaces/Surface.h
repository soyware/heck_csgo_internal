#pragma once

#undef CreateFont

enum FontDrawType_t
{
	// Use the "additive" value from the scheme file
	FONT_DRAW_DEFAULT = 0,

	// Overrides
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,

	FONT_DRAW_TYPE_COUNT = 2,
};

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,	// custom generated font - never fall back to asian compatibility mode
	FONTFLAG_BITMAP = 0x800,	// compiled bitmap font - no fallbacks
};

struct FontVertex_t
{
	FontVertex_t() {}
	FontVertex_t(const vec2& pos, const vec2& coord = vec2(0, 0))
	{
		Init(pos, coord);
	}

	void Init(const vec2& pos, const vec2& coord = vec2(0, 0))
	{
		m_Position = pos;
		m_TexCoord = coord;
	}

	vec2 m_Position;
	vec2 m_TexCoord;
};

typedef unsigned long FontHandle_t;

typedef FontHandle_t HFont;
typedef FontVertex_t Vertex_t;

class ISurface
{
public:
	// prepare panel for painting
	inline void PushMakeCurrent(unsigned int panel, bool useInSets)
	{
		typedef void(__thiscall* Fn) (ISurface*, unsigned int, bool);
		GetVF<Fn>(this, 12)(this, panel, useInSets);
	}

	inline void PopMakeCurrent(unsigned int panel)
	{
		typedef void(__thiscall* Fn) (ISurface*, unsigned int);
		GetVF<Fn>(this, 13)(this, panel);
	}

	inline void DrawSetColor(Color clr)
	{
		typedef void(__thiscall* Fn) (ISurface*, Color);
		GetVF<Fn>(this, 14)(this, clr);
	}

	inline void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* Fn) (ISurface*, int, int, int, int);
		GetVF<Fn>(this, 16)(this, x0, y0, x1, y1);
	}

	// using 4 DrawFilledRect
	inline void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* Fn) (ISurface*, int, int, int, int);
		GetVF<Fn>(this, 18)(this, x0, y0, x1, y1);
	}

	inline void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* Fn) (ISurface*, int, int, int, int);
		GetVF<Fn>(this, 19)(this, x0, y0, x1, y1);
	}

	inline void DrawSetTextFont(HFont font)
	{
		typedef void(__thiscall* Fn) (ISurface*, HFont);
		GetVF<Fn>(this, 23)(this, font);
	}

	inline void DrawSetTextColor(Color clr)
	{
		typedef void(__thiscall* Fn) (ISurface*, Color);
		GetVF<Fn>(this, 24)(this, clr);
	}

	inline void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* Fn) (ISurface*, int, int);
		GetVF<Fn>(this, 26)(this, x, y);
	}

	inline void DrawPrintText(const wchar_t* text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT)
	{
		typedef void(__thiscall* Fn) (ISurface*, const wchar_t*, int, FontDrawType_t);
		GetVF<Fn>(this, 28)(this, text, textLen, drawType);
	}

	inline void DrawUnicodeChar(wchar_t wch, FontDrawType_t drawType = FONT_DRAW_DEFAULT) 
	{
		typedef void(__thiscall* Fn)(ISurface*, wchar_t, FontDrawType_t);
		GetVF<Fn>(this, 29)(this, wch, drawType);
	}

	inline void DrawSetTextureRGBA(int id, const unsigned char* rgba, int wide, int tall)
	{
		typedef void(__thiscall* Fn)(ISurface*, int, const unsigned char*, int, int);
		GetVF<Fn>(this, 37)(this, id, rgba, wide, tall);
	}

	inline void DrawSetTexture(int id)
	{
		typedef void(__thiscall* Fn)(ISurface*, int);
		GetVF<Fn>(this, 38)(this, id);
	}

	inline int CreateNewTextureID(bool procedural = false)
	{
		typedef int(__thiscall* Fn)(ISurface*, bool);
		return GetVF<Fn>(this, 43)(this, procedural);
	}

	//inline void SetCursor(ULONG cursor)
	//{
	//	typedef void(__thiscall* Fn)(ISurface*, ULONG);
	//	GetVF<Fn>(this, 57)(this, cursor);
	//}

	inline bool IsCursorVisible()
	{
		typedef bool(__thiscall *Fn)(ISurface*);
		return GetVF<Fn>(this, 58)(this);
	}

	//inline void UnlockCursor()
	//{
	//	typedef void(__thiscall *Fn)(ISurface*);
	//	GetVF<Fn>(this, 66)(this);
	//}

	//inline void LockCursor()
	//{
	//	typedef void(__thiscall *Fn)(ISurface*);
	//	GetVF<Fn>(this, 67)(this);
	//}

	inline HFont CreateFont()
	{
		typedef HFont(__thiscall* Fn) (ISurface*);
		return GetVF<Fn>(this, 71)(this);
	}

	inline bool SetFontGlyphSet(HFont font, const char* windowsFontName, int tall, int weight = 0, int blur = 0, int scanlines = 0, int flags = FONTFLAG_NONE, int nRangeMin = 0, int nRangeMax = 0)
	{
		typedef bool(__thiscall* Fn) (ISurface*, HFont, const char*, int, int, int, int, int, int, int);
		return GetVF<Fn>(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
	}

	inline bool AddCustomFontFile(const char* fontName, const char* fontFileName)
	{
		typedef bool(__thiscall* Fn) (ISurface*, const char*, const char*);
		return GetVF<Fn>(this, 73)(this, fontName, fontFileName);
	}

	inline int GetCharacterWidth(HFont font, int ch)
	{
		typedef int(__thiscall* Fn) (ISurface*, HFont, int);
		return GetVF<Fn>(this, 78)(this, font, ch);
	}

	inline void GetTextSize(HFont font, const wchar_t* text, int &wide, int &tall)
	{
		typedef void(__thiscall* Fn) (ISurface*, HFont, const wchar_t*, int&, int&);
		GetVF<Fn>(this, 79)(this, font, text, wide, tall);
	}

	inline void PlaySound(const char* fileName)
	{
		typedef void(__thiscall* Fn) (ISurface*, const char*);
		GetVF<Fn>(this, 82)(this, fileName);
	}

	inline void DrawOutlinedCircle(int x, int y, int radius, int segments)
	{
		typedef void(__thiscall* Fn) (ISurface*, int, int, int, int);
		GetVF<Fn>(this, 103)(this, x, y, radius, segments);
	}

	inline void DrawTexturedPolyLine(const Vertex_t* p, int n)
	{
		typedef void(__thiscall* Fn) (ISurface*, const Vertex_t*, int);
		GetVF<Fn>(this, 104)(this, p, n);
	}

	inline void DrawTexturedPolygon(int n, Vertex_t* pVertice, bool bClipVertices = true)
	{
		typedef void(__thiscall* Fn) (ISurface*, int, Vertex_t*, bool);
		GetVF<Fn>(this, 106)(this, n, pVertice, bClipVertices);
	}

	//inline const char* GetFontName(HFont font)
	//{
	//	typedef const char* (__thiscall* Fn) (ISurface*, HFont);
	//	return GetVF<Fn>(this, 134)(this, font);
	//}
};
