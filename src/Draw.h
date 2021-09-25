#pragma once

#pragma warning(push)
#pragma warning( disable : 4244 )

namespace Draw
{
	//HFont	fontMarlett;	// X
	//HFont	fontAlte;		// Tabs

	HFont	fontESPShadow;
	HFont	fontESPOutline;

	HFont	fontIcons;

	HFont	fontUI;

	void Init(HINSTANCE hModule)
	{
		//fontMarlett = Interfaces::Surface->FontCreate();
		//Interfaces::Surface->SetFontGlyphSet(fontMarlett, "Marlett", 26, 0, 0, 0, FONTFLAG_SYMBOL);

		//fontAlte = Interfaces::Surface->FontCreate();
		//Interfaces::Surface->SetFontGlyphSet(fontAlte, "Alte DIN 1451 Mittelschrift", 26, 0, 0, 0, FONTFLAG_ANTIALIAS);

		fontESPShadow = I::Surface->CreateFont();
		I::Surface->SetFontGlyphSet(fontESPShadow, "Arial", 13, 0, 0, 0, FONTFLAG_DROPSHADOW, 0x0000, 0xFFFF);

		fontESPOutline = I::Surface->CreateFont();
		I::Surface->SetFontGlyphSet(fontESPOutline, "Arial", 13, 0, 0, 0, FONTFLAG_OUTLINE, 0x0000, 0xFFFF);

		LoadResourceFont(hModule, IDR_TTFFONT1);

		fontIcons = I::Surface->CreateFont();
		I::Surface->SetFontGlyphSet(fontIcons, "CSGO_Equipment", 13, 0, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_ANTIALIAS, 0x0000, 0xFFFF);

		fontUI = I::Surface->CreateFont();
		I::Surface->SetFontGlyphSet(fontUI, "Tahoma", 14, 600, 0, 0, FONTFLAG_DROPSHADOW, 0x0000, 0xFFFF);
	}

	inline HFont GetESPFont()
	{
		if (Settings::Visuals::ESP::Style.GetInt())
			return fontESPOutline;
		else
			return fontESPShadow;
	}

#pragma region Text

	void Text(const wchar_t* text, int x, int y)
	{
		I::Surface->DrawSetTextPos(x, y);
		I::Surface->DrawPrintText(text, wcslen(text));
	}

	void Text(const wchar_t* text, int x, int y, HFont font, Color clr)
	{
		I::Surface->DrawSetTextFont(font);
		I::Surface->DrawSetTextColor(clr);
		I::Surface->DrawSetTextPos(x, y);
		I::Surface->DrawPrintText(text, wcslen(text));
	}

	void Text(const wchar_t* text, const vec2& vec, HFont font, Color clr)
	{
		I::Surface->DrawSetTextFont(font);
		I::Surface->DrawSetTextColor(clr);
		I::Surface->DrawSetTextPos(vec.x, vec.y);
		I::Surface->DrawPrintText(text, wcslen(text));
	}

	void Text(const wchar_t* text, const vec3& vec, HFont font, Color clr)
	{
		I::Surface->DrawSetTextFont(font);
		I::Surface->DrawSetTextColor(clr);
		I::Surface->DrawSetTextPos(vec.x, vec.y);
		I::Surface->DrawPrintText(text, wcslen(text));
	}

	void UnicodeChar(wchar_t wch, int x, int y, HFont font, Color clr)
	{
		I::Surface->DrawSetTextFont(font);
		I::Surface->DrawSetTextColor(clr);
		I::Surface->DrawSetTextPos(x, y);
		I::Surface->DrawUnicodeChar(wch);
	}

#pragma endregion

#pragma region FilledRect

	void FilledRect(int x0, int y0, int x1, int y1, Color clr)
	{
		I::Surface->DrawSetColor(clr);
		I::Surface->DrawFilledRect(x0, y0, x1, y1);
	}

	void FilledRect(const vec2& vec0, const vec2& vec1, Color clr)
	{
		I::Surface->DrawSetColor(clr);
		I::Surface->DrawFilledRect(vec0.x, vec0.y, vec1.x, vec1.y);
	}

	void FilledRect(const vec3& vec0, const vec3& vec1, Color clr)
	{
		I::Surface->DrawSetColor(clr);
		I::Surface->DrawFilledRect(vec0.x, vec0.y, vec1.x, vec1.y);
	}

#pragma endregion

#pragma region OutlinedRect

	void OutlinedRect(int x0, int y0, int x1, int y1, Color clr)
	{
		I::Surface->DrawSetColor(clr);
		I::Surface->DrawOutlinedRect(x0, y0, x1, y1);
	}

	void OutlinedRect(const vec2& vec0, const vec2& vec1, Color clr)
	{
		I::Surface->DrawSetColor(clr);
		I::Surface->DrawOutlinedRect(vec0.x, vec0.y, vec1.x, vec1.y);
	}

	void OutlinedRect(const vec3& vec0, const vec3& vec1, Color clr)
	{
		I::Surface->DrawSetColor(clr);
		I::Surface->DrawOutlinedRect(vec0.x, vec0.y, vec1.x, vec1.y);
	}

#pragma endregion

#pragma region Line

	void Line(int x0, int y0, int x1, int y1, Color clr)
	{
		I::Surface->DrawSetColor(clr);
		I::Surface->DrawLine(x0, y0, x1, y1);
	}

	void Line(const vec2& vec0, const vec2& vec1, Color clr)
	{
		I::Surface->DrawSetColor(clr);
		I::Surface->DrawLine(vec0.x, vec0.y, vec1.x, vec1.y);
	}

	void Line(const vec3& vec0, const vec3& vec1, Color clr)
	{
		I::Surface->DrawSetColor(clr);
		I::Surface->DrawLine(vec0.x, vec0.y, vec1.x, vec1.y);
	}

#pragma endregion

	void FilledPoly(Vertex_t* verts, int count, Color clr)
	{
		static const int texture = I::Surface->CreateNewTextureID(true);

		I::Surface->DrawSetTextureRGBA(texture, &clr.r, 1, 1);
		I::Surface->DrawSetTexture(texture);
		I::Surface->DrawSetColor(Color::White());
		I::Surface->DrawTexturedPolygon(count, verts);
	}

	void PolyLine(Vertex_t* verts, int count, Color clr)
	{
		static const int texture = I::Surface->CreateNewTextureID(true);

		I::Surface->DrawSetTextureRGBA(texture, &clr.r, 1, 1);
		I::Surface->DrawSetTexture(texture);
		I::Surface->DrawSetColor(Color::White());
		I::Surface->DrawTexturedPolyLine(verts, count);
	}

	void Arrow(int x, int y, float degrees, Color clr)
	{
		vec2 rotatedLeft;
		vec2 rotatedTop;
		vec2 rotatedRight;

		Vector2DRotate(vec2(-10, 10), degrees, rotatedLeft);
		Vector2DRotate(vec2(0, -10), degrees, rotatedTop);
		Vector2DRotate(vec2(10, 10), degrees, rotatedRight);

		rotatedLeft.x += x;
		rotatedLeft.y += y;

		rotatedTop.x += x;
		rotatedTop.y += y;

		rotatedRight.x += x;
		rotatedRight.y += y;

		Vertex_t verts[3];
		verts[0].Init(rotatedLeft);
		verts[1].Init(rotatedTop);
		verts[2].Init(rotatedRight);

		Color fillClr(clr.r, clr.g, clr.b, 48);
		FilledPoly(verts, 3, fillClr);

		PolyLine(verts, 3, clr);
	}
}

#pragma warning(pop)