#pragma once
#include "..\Utils\Color.h"
#include "..\Utils\Utils.h"
#include "..\vector2d.h"
#include <array>

namespace vgui {
	typedef unsigned long HFont;
	typedef unsigned int VPANEL;
};

enum FontCenteringFlags
{
	HFONT_LEFT = 0,
	HFONT_CENTERED_NONE = (1 << 0),
	HFONT_CENTERED_X = (1 << 1),
	HFONT_CENTERED_Y = (1 << 2),
	HFONT_OFF_SCREEN,
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
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};


struct Vertex_t
{
	Vector2D	mPosition;
	Vector2D	m_TexCoord;

	Vertex_t() {}
	Vertex_t(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0))
	{
		mPosition = pos;
		m_TexCoord = coord;
	}
	void Init(const Vector2D& pos, const Vector2D& coord = Vector2D(0, 0))
	{
		mPosition = pos;
		m_TexCoord = coord;
	}
};
struct vertex_t {
	vertex_t() { }
	vertex_t(const vec2_t& pos, const vec2_t& coord = vec2_t(0, 0)) {
		position = pos;
		m_TexCoord = coord;
	}
	void Init(const vec2_t& pos, const vec2_t& coord = vec2_t(0, 0)) {
		position = pos;
		m_TexCoord = coord;
	}

	vec2_t	position;
	vec2_t	m_TexCoord;
};
typedef  Vertex_t FontVertex_t;
template<typename T, typename ...Args>
constexpr auto callVirtualMethod(void* classBase, int index, Args... args) noexcept
{
	return ((*reinterpret_cast<T(__thiscall***)(void*, Args...)>(classBase))[index])(classBase, args...);
}

class ISurface
{
public:
	inline void DrawTexturedRect(int x, int y, int w, int h) {
		typedef void(__thiscall* OrigFn)(void*, int, int, int, int);
		Utils::GetVFunc<OrigFn>(this, 41)(this, x, y, w, h);
	}
	void UnLockCursor()
	{
		typedef void(__thiscall* Fn)(void*);
		Utils::GetVFunc< Fn >(this, 66)(this);
	}
	void draw_wide_string(bool center, unsigned long font, int x, int y, Color c, wchar_t* str)
	{
		if (center) {
			int wide = 0, tall = 0;
			GetTextSize(font, str, wide, tall);
			x -= wide / 2;
			y -= tall / 2;
		}

		DrawSetTextColor(c);
		DrawSetTextFont(font);
		DrawSetTextPos(x, y);
		DrawPrintText(str, (int)wcslen(str));
	}
	void draw_filled_rect(int x, int y, int w, int h, Color color)
	{
		DrawSetColor(color);
		DrawFilledRect(x, y, x + w, y + h);
	}


	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 15)(this, r, g, b, a);
	}

	void DrawSetColor(Color col)
	{
		typedef void(__thiscall* Fn)(PVOID, Color);
		return Utils::GetVFunc< Fn >(this, 14)(this, col);
	}
	RECT GetTextSizeGAY(DWORD font, const char* text)
	{
		size_t origsize = strlen(text) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

		RECT rect; int x, y;
		GetTextSize(font, wcstring, x, y);
		rect.left = x; rect.bottom = y;
		rect.right = x;
		return rect;
	}
	void DrawEmptyRect(int x1, int y1, int x2, int y2, Color color, unsigned char = 0); // the flags are for which sides to ignore in clockwise, 0b1 is top, 0b10 is right, etc.
	void DrawF(int X, int Y, unsigned int Font, bool center_width, bool center_height, Color Color, std::string Input, ...)
	{
		/* char -> wchar */
		size_t size = Input.size() + 1;
		auto wide_buffer = std::make_unique<wchar_t[]>(size);
		mbstowcs_s(0, wide_buffer.get(), size, Input.c_str(), size - 1);

		/* check center */
		int width = 0, height = 0;
		GetTextSize(Font, wide_buffer.get(), width, height);
		if (!center_width)
			width = 0;
		if (!center_height)
			height = 0;

		/* call and draw*/
		DrawSetTextColor(Color);
		DrawSetTextFont(Font);
		DrawSetTextPos(X - (width * .5), Y - (height * .5));
		DrawPrintText(wide_buffer.get(), wcslen(wide_buffer.get()));
	}
	void outlined_rect(int x, int y, int w, int h, Color color_out, Color color_in)
	{
		DrawSetColor(color_in);
		DrawFilledRect(x, y, x + w, y + h);

		DrawSetColor(color_out);
		DrawOutlinedRect(x, y, x + w, y + h);
	}
	void Textf(int x, int y, Color color, DWORD font, const char* fmt, ...)
	{
		if (!fmt) return; //if the passed string is null return
		if (strlen(fmt) < 2) return;

		//Set up va_list and buffer to hold the params 
		va_list va_alist;
		char logBuf[256] = { 0 };

		//Do sprintf with the parameters
		va_start(va_alist, fmt);
		_vsnprintf_s(logBuf + strlen(logBuf), 256 - strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
		va_end(va_alist);

		Text(x, y, color, font, logBuf);
	}


	void DrawFilledRect(int x1, int y1, int x2, int y2, Color color)
	{
		DrawSetColor(color);
		DrawFilledRect(x1, y1, x2, y2);
	}

	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 16)(this, x0, y0, x1, y1);
	}

	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 18)(this, x0, y0, x1, y1);
	}

	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 19)(this, x0, y0, x1, y1);
	}
	void PlaySound_(const char* fileName)
	{
		typedef void(__thiscall* Fn)(PVOID, const char*);
		return Utils::GetVFunc< Fn >(this, 82)(this, fileName);
	}
	void DrawPolyLine(int* x, int* y, int count)
	{
		typedef void(__thiscall* Fn)(PVOID, int*, int*, int);
		Utils::GetVFunc< Fn >(this, 20)(this, x, y, count);
	}

	void DrawSetTextFont(unsigned long font)
	{
		typedef void(__thiscall* Fn)(PVOID, unsigned long);
		Utils::GetVFunc< Fn >(this, 23)(this, font);
	}
	constexpr auto getScreenSize() noexcept
	{
		int width{ }, height{ };
		callVirtualMethod<void, int&, int&>(this, 44, width, height);
		return std::make_pair(width, height);
	}

	constexpr void setDrawColor1(int r, int g, int b, int a = 255) noexcept
	{
		callVirtualMethod<void, int, int, int, int>(this, 15, r, g, b, a);
	}
	constexpr void setDrawColor2(std::tuple<float, float, float> color, int a = 255) noexcept
	{
		callVirtualMethod<void, int, int, int, int>(this, 15, static_cast<int>(std::get<0>(color) * 255), static_cast<int>(std::get<1>(color) * 255), static_cast<int>(std::get<2>(color) * 255), a);
	}
	constexpr void setTextColor5(int r, int g, int b, int a = 255) noexcept
	{
		callVirtualMethod<void, int, int, int, int>(this, 25, r, g, b, a);
	}
	constexpr void printText(const std::wstring_view text, int drawType = 0) noexcept
	{
		callVirtualMethod<void, const wchar_t*, int, int>(this, 28, text.data(), text.length(), drawType);
	}
	constexpr auto getTextSize(unsigned font, const wchar_t* text) noexcept
	{
		int width{ }, height{ };
		callVirtualMethod<void, unsigned, const wchar_t*, int&, int&>(this, 79, font, text, width, height);
		return std::make_pair(width, height);
	}
	void DrawSetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 25)(this, r, g, b, a);
	}

	void DrawSetTextColor(Color col)
	{
		typedef void(__thiscall* Fn)(PVOID, Color);
		return Utils::GetVFunc< Fn >(this, 24)(this, col);
	}

	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int);
		Utils::GetVFunc< Fn >(this, 26)(this, x, y);
	}

	void DrawPrintText(const wchar_t* text, int textLen)
	{
		typedef void(__thiscall* Fn)(PVOID, const wchar_t*, int, int);
		Utils::GetVFunc< Fn >(this, 28)(this, text, textLen, 0);
	}

	void DrawSetTexture(int textureID)
	{
		typedef void(__thiscall* Fn)(PVOID, int);
		return Utils::GetVFunc< Fn >(this, 38)(this, textureID);
	}

	void DrawSetTextureRGBA(int textureID, unsigned char  const* colors, int w, int h)
	{
		typedef void(__thiscall* Fn)(PVOID, int, unsigned char  const*, int, int);
		return Utils::GetVFunc< Fn >(this, 37)(this, textureID, colors, w, h);
	}

	int CreateNewTextureID(bool procedural)
	{
		typedef int(__thiscall* Fn)(PVOID, bool);
		return Utils::GetVFunc< Fn >(this, 43)(this, procedural);
	}

	void DrawTexturedPolygon(int vtxCount, FontVertex_t* vtx, bool bClipVertices = true)
	{
		typedef void(__thiscall* Fn)(PVOID, int, FontVertex_t*, bool);
		return Utils::GetVFunc< Fn >(this, 106)(this, vtxCount, vtx, bClipVertices);
	}

	unsigned long FontCreate()
	{
		typedef unsigned int(__thiscall* Fn)(PVOID);
		return Utils::GetVFunc< Fn >(this, 71)(this);
	}

	void SetFontGlyphSet(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef void(__thiscall* Fn)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}

	void ISurface::GetTextSize(DWORD font, const wchar_t* text, int& wide, int& tall)
	{
		typedef void(__thiscall* Fn)(void*, DWORD, const wchar_t*, int&, int&);
		return Utils::GetVFunc<Fn>(this, 79)(this, font, text, wide, tall);
	}

	void LogGetTextSize(int Font, const wchar_t* _Input, int& Wide, int& Tall)
	{
		typedef void(__thiscall* Fn)(void*, int, const wchar_t*, int&, int&);
		return Utils::GetVFunc<Fn>(this, 79)(this, Font, _Input, Wide, Tall);
	}
	void LogDrawSetTextColor(Color col)
	{
		typedef void(__thiscall* Fn)(void*, Color);
		return Utils::GetVFunc< Fn >(this, 24)(this, col);
	}
	void LogDrawSetTextFont(unsigned int Font)
	{
		typedef void(__thiscall* Fn)(void*, unsigned int);
		Utils::GetVFunc< Fn >(this, 23)(this, Font);
	}

	void LogDrawPrintText(const wchar_t* Text, int Len, int DrawType = 0)
	{
		typedef void(__thiscall* Fn)(void*, wchar_t const*, int, int);
		Utils::GetVFunc< Fn >(this, 28)(this, Text, Len, DrawType);
	}

	void DrawOutlinedCircle(int x, int y, int r, int seg)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		return Utils::GetVFunc< Fn >(this, 103)(this, x, y, r, seg);
	}

	void SurfaceGetCursorPos(int& x, int& y)
	{
		typedef void(__thiscall* Fn)(PVOID, int&, int&);
		return Utils::GetVFunc< Fn >(this, 66)(this, x, y);
	}

	void rect(int x, int y, int w, int h, Color color)
	{
		DrawSetColor(color);
		DrawOutlinedRect(x, y, x + w, y + h);
	}
	void FilledRect(int x, int y, int w, int h, Color color)
	{
		DrawSetColor(color);
		DrawFilledRect(x, y, x + w, y + h);
	}

	void OutlinedRect(int x, int y, int w, int h, Color color)
	{
		DrawSetColor(color);
		DrawOutlinedRect(x, y, x + w, y + h);
	}

	RECT viewport()
	{
		RECT viewport = { 0, 0, 0, 0 };

		int width, height;
		g_pEngine->GetScreenSize(width, height);
		viewport.right = width;
		viewport.bottom = height;

		return viewport;
	}

	void TextNEW(int x, int y, Color color, DWORD font, const wchar_t* text)
	{

		DrawSetTextFont(font);
		DrawSetTextColor(color);
		DrawSetTextPos(x, y);

		DrawPrintText(text, wcslen(text));
	}

	void rect_filled(int x, int y, int w, int h, Color color)
	{
		DrawSetColor(color);
		DrawFilledRect(x, y, x + w, y + h);
	}

	virtual int           GetFontTall(vgui::HFont font) = 0;
	void text(vgui::HFont font, int x, int y, Color color, DWORD flags, const char* msg, ...)
	{

		va_list va_alist;
		char buffer[1024];
		va_start(va_alist, msg);
		_vsnprintf(buffer, sizeof(buffer), msg, va_alist);
		va_end(va_alist);
		wchar_t wbuf[1024];

		MultiByteToWideChar(CP_UTF8, 0, buffer, 256, wbuf, 256);

		int width, height;
		GetTextSize(font, wbuf, width, height);

		if (!(flags & HFONT_CENTERED_NONE)) {
			if (flags & HFONT_CENTERED_X)
				x -= width * 0.5f;

			if (flags & HFONT_CENTERED_Y)
				y -= height * 0.5f;
		}

		DrawSetTextFont(font);
		DrawSetTextColor(color);
		DrawSetTextPos(x, y);
		DrawPrintText(wbuf, wcslen(wbuf));
	}



	void line(int x, int y, int x2, int y2, Color color)
	{
		DrawSetColor(color);
		DrawLine(x, y, x2, y2);
	}

	void OutlinedVecRect(int x, int y, int x2, int y2, Color color)
	{
		DrawSetColor(color);
		DrawLine(x, y, x2, y);
		DrawLine(x2, y, x2, y2);
		DrawLine(x2, y2, x, y2);
		DrawLine(x, y2, x, y);
	}

	void Line(int x, int y, int x2, int y2, Color color)
	{
		DrawSetColor(color);
		DrawLine(x, y, x2, y2);
	}

	void Clear(int x, int y, int w, int h, Color color)
	{
		DrawSetColor(color);
		DrawFilledRect(x, y, x + w, y + h);
	}
	RECT GetTextSize2(const char* _Input, int font)
	{
		int apple = 0;
		char Buffer[2048] = { '\0' };
		va_list Args;
		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);
		size_t Size = strlen(Buffer) + 1;
		wchar_t* WideBuffer = new wchar_t[Size];
		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);
		int Width = 0, Height = 0;

		GetTextSize(font, WideBuffer, Width, Height);

		RECT outcome = { 0, 0, Width, Height };
		return outcome;
	}

	void GradientV(int x, int y, int w, int h, Color c1, Color c2)
	{
		Clear(x, y, w, h, c1);
		BYTE first = c2.r();
		BYTE second = c2.g();
		BYTE third = c2.b();
		for (int i = 0; i < h; i++)
		{
			float fi = i, fh = h;
			float a = fi / fh;
			DWORD ia = a * 255;
			Clear(x, y + i, w, 1, Color(first, second, third, ia));
		}
	}
	void Outline(float x, float y, float w, float h, Color color)
	{
		DrawSetColor(color);
		DrawOutlinedRect(x, y, x + w, y + h);
	}
	void DrawRect(int x, int y, int w, int h, Color col)
	{
		DrawSetColor(col);
		DrawFilledRect(x, y, x + w, y + h);
	}
	int TweakColor(int c1, int c2, int variation)
	{
		if (c1 == c2)
			return c1;
		else if (c1 < c2)
			c1 += variation;
		else
			c1 -= variation;
		return c1;
	}


	void GradientSideways(int x, int y, int w, int h, Color color1, Color color2, int variation)
	{
		int r1 = color1.r();
		int g1 = color1.g();
		int b1 = color1.b();
		int a1 = color1.a();

		int r2 = color2.r();
		int g2 = color2.g();
		int b2 = color2.b();
		int a2 = color2.a();

		for (int i = 0; i <= w; i++)
		{
			DrawRect(x + i, y, 1, h, Color(r1, g1, b1, a1));
			r1 = TweakColor(r1, r2, variation);
			g1 = TweakColor(g1, g2, variation);
			b1 = TweakColor(b1, b2, variation);
			a1 = TweakColor(a1, a2, variation);
		}
	}

	RECT GetTextSizeNEW(DWORD font, const char* text)
	{
		size_t origsize = strlen(text) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

		RECT rect; int x, y;
		GetTextSize(font, wcstring, x, y);
		rect.left = x; rect.bottom = y;
		rect.right = x;
		return rect;
	}

	void Text(int x, int y, Color color, DWORD font, const char* text)
	{
		size_t origsize = strlen(text) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

		DrawSetTextFont(font);

		DrawSetTextColor(color);
		DrawSetTextPos(x, y);
		DrawPrintText(wcstring, wcslen(wcstring));
		return;
	}
	void Text2(int x, int y, const char* _Input, int font, Color color)
	{
		int apple = 0;
		char Buffer[2048] = { '\0' };
		va_list Args;
		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);
		size_t Size = strlen(Buffer) + 1;
		wchar_t* WideBuffer = new wchar_t[Size];
		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

		DrawSetTextColor(color);
		DrawSetTextFont(font);
		DrawSetTextPos(x, y);
		DrawPrintText(WideBuffer, wcslen(WideBuffer));
	}
	RECT get_text_size(const char* _Input, int font)
	{
		int apple = 0;
		char Buffer[2048] = { '\0' };
		va_list Args;
		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);
		size_t Size = strlen(Buffer) + 1;
		wchar_t* WideBuffer = new wchar_t[Size];
		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);
		int Width = 0, Height = 0;

		GetTextSize(font, WideBuffer, Width, Height);

		RECT outcome = { 0, 0, Width, Height };
		return outcome;
	}
	void color_spectrum(int x, int y, int w, int h)
	{
		static int GradientTexture = 0;
		static std::unique_ptr<Color[]> Gradient = nullptr;
		if (!Gradient)
		{
			Gradient = std::make_unique<Color[]>(w * h);

			for (int i = 0; i < w; i++)
			{
				int div = w / 6;
				int phase = i / div;
				float t = (i % div) / (float)div;
				int r, g, b;

				switch (phase)
				{
				case(0):
					r = 255;
					g = 255 * t;
					b = 0;
					break;
				case(1):
					r = 255 * (1.f - t);
					g = 255;
					b = 0;
					break;
				case(2):
					r = 0;
					g = 255;
					b = 255 * t;
					break;
				case(3):
					r = 0;
					g = 255 * (1.f - t);
					b = 255;
					break;
				case(4):
					r = 255 * t;
					g = 0;
					b = 255;
					break;
				case(5):
					r = 255;
					g = 0;
					b = 255 * (1.f - t);
					break;
				}

				for (int k = 0; k < h; k++)
				{
					float sat = k / (float)h;
					int _r = r + sat * (255 - r);
					int _g = g + sat * (255 - g);
					int _b = b + sat * (255 - b);

					*reinterpret_cast<Color*>(Gradient.get() + i + k * w) = Color(_r, _g, _b);
				}
			}

			GradientTexture = CreateNewTextureID(true);
			DrawSetTextureRGBA(GradientTexture, (unsigned char*)Gradient.get(), w, h);
		}
		DrawSetColor(Color(255, 255, 255, 255));
		DrawSetTexture(GradientTexture);
		DrawTexturedRect(x, y, x + w, y + h);
	}

	Color color_spectrum_pen(int x, int y, int w, int h, Vector stx)
	{
		int div = w / 6;
		int phase = stx.x / div;
		float t = ((int)stx.x % div) / (float)div;
		int r, g, b;

		switch (phase)
		{
		case(0):
			r = 255;
			g = 255 * t;
			b = 0;
			break;
		case(1):
			r = 255 * (1.f - t);
			g = 255;
			b = 0;
			break;
		case(2):
			r = 0;
			g = 255;
			b = 255 * t;
			break;
		case(3):
			r = 0;
			g = 255 * (1.f - t);
			b = 255;
			break;
		case(4):
			r = 255 * t;
			g = 0;
			b = 255;
			break;
		case(5):
			r = 255;
			g = 0;
			b = 255 * (1.f - t);
			break;
		}

		float sat = stx.y / h;
		return Color(r + sat * (255 - r), g + sat * (255 - g), b + sat * (255 - b), 255);

	}

	void Text(int x, int y, const char* _Input, int font, Color color)
	{
		int apple = 0;
		char Buffer[2048] = { '\0' };
		va_list Args;
		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);
		size_t Size = strlen(Buffer) + 1;
		wchar_t* WideBuffer = new wchar_t[Size];
		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

		DrawSetTextColor(color);
		DrawSetTextFont(font);
		DrawSetTextPos(x, y);
		DrawPrintText(WideBuffer, wcslen(WideBuffer));
	}


	void Polygon(int count, Vertex_t* Vertexs, Color color)
	{
		static int Texture = CreateNewTextureID(true); //need to make a texture with procedural true
		unsigned char buffer[4] = { 255, 255, 255, 255 };//{ color.r(), color.g(), color.b(), color.a() };

		DrawSetTextureRGBA(Texture, buffer, 1, 1); //Texture, char array of texture, width, height
		DrawSetColor(color); // keep this full color and opacity use the RGBA @top to set values.
		DrawSetTexture(Texture); // bind texture

		DrawTexturedPolygon(count, Vertexs);
	}

	void DrawT(int X, int Y, Color Color, int Font, bool Center, const char* _Input, ...)
	{
		int apple = 0;
		char Buffer[2048] = { '\0' };
		va_list Args;

		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);

		size_t Size = strlen(Buffer) + 1;
		wchar_t* WideBuffer = new wchar_t[Size];

		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

		int Width = 0, Height = 0;

		if (Center)
			GetTextSize(Font, WideBuffer, Width, Height);

		DrawSetTextColor(Color);
		DrawSetTextFont(Font);
		DrawSetTextPos(X - (Width / 2), Y);
		DrawPrintText(WideBuffer, wcslen(WideBuffer));
	}

	RECT GetTextSizeRect(DWORD font, const char* text) // ayyware or something
	{
		size_t origsize = strlen(text) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

		RECT rect; int x, y;
		GetTextSize(font, wcstring, x, y);
		rect.left = x; rect.bottom = y;
		rect.right = x;
		return rect;
	}

	Vector2D GetMousePosition() // bolbi ware
	{
		POINT mousePosition;
		GetCursorPos(&mousePosition);
		ScreenToClient(FindWindow(0, "Counter-Strike: Global Offensive"), &mousePosition);
		return { static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y) };
	}

	bool MouseInRegion(int x, int y, int x2, int y2) {
		if (GetMousePosition().x > x&& GetMousePosition().y > y&& GetMousePosition().x < x2 + x && GetMousePosition().y < y2 + y)
			return true;
		return false;
	}
	int create_new_texture_id(bool procedural = false) {
		using original_fn = int(__thiscall*)(ISurface*, bool);
		return (*(original_fn**)this)[43](this, procedural);
	}
	void set_drawing_color(int r, int g, int b, int a = 255) {
		using original_fn = void(__thiscall*)(ISurface*, int, int, int, int);
		return (*(original_fn**)this)[15](this, r, g, b, a);
	}
	void set_texture(int id) {
		using original_fn = void(__thiscall*)(ISurface*, int);
		return (*(original_fn**)this)[38](this, id);
	}
	void draw_polygon(int n, vertex_t* vertice, bool clip_vertices = true) {
		using original_fn = void(__thiscall*)(ISurface*, int, vertex_t*, bool);
		return (*(original_fn**)this)[106](this, n, vertice, clip_vertices);
	}
	void add_textured_polygon(int n, vertex_t* vertice, Color colour)
	{
		static int texture_id = create_new_texture_id(true);
		static unsigned char buf[4] = { 255, 255, 255, 255 };
		DrawSetColor(colour);
		set_texture(texture_id);
		draw_polygon(n, vertice);
	}
	void draw_filled_triangle(std::array< vec2_t, 3 >points, Color colour) {
		std::array< vertex_t, 3 >vertices{ vertex_t(points.at(0)), vertex_t(points.at(1)), vertex_t(points.at(2)) };
		add_textured_polygon(3, vertices.data(), colour);
	}

	void DrawCircle(float x, float y, float r, float segments, Color color)
	{
		DrawSetColor(color);
		DrawOutlinedCircle(x, y, r, segments);
	}

};

extern ISurface* g_pSurface;