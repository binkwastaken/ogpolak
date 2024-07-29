#pragma once
#include "../utils/utils.h"
#include "../../extensions/imgui.h"
#include "../../extensions/fonts.h"

#include "../../sdk/math/mathematics.h"

#include <D3DX11.h>
#include <d3d11.h>
#include <D3DX11tex.h>

#pragma comment (lib,"D3DX11.lib")


class CRenderer {
public:
	bool Init(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	// Rendering functions
	void DrawImage(ID3D11ShaderResourceView* image, int x, int y, int w, int h);
	void DrawLine(const Vector2D& start, const Vector2D& end, const color_t& color, int thickness = 1);
	void DrawLine(int x, int y, int x2, int y2, const color_t& color, int thickness = 1);
	void DrawRect(const Vector2D& start, const Vector2D& end, const color_t& color);
	void DrawRect(int x, int y, int w, int h, const color_t& color);
	void DrawFilledRect(const Vector2D& start, const Vector2D& end, const color_t& color);
	void DrawFilledRect(int x, int y, int w, int h, const color_t& color);
	void DrawString(const char* text, const Vector2D& pos, const color_t& color, const bool centered = false);
	void DrawTriangle(const Vector2D& a, const Vector2D& b, const Vector2D& c, const color_t& color, bool fillled = false);
	void DrawOutlinedString(const char* text, Vector2D position, color_t textColor, color_t outlineColor, bool centered);
	void DrawCapsule(const Vector2D& start, const Vector2D& end, float radius, const color_t& color, bool fillled = false);
	void DrawOutlinedLine(const Vector2D& start, const Vector2D& end, const color_t& color, int thickness, const color_t& outlineColor);
	void DrawOutlinedLine(int x, int y, int x2, int y2, const color_t& color, const color_t& outlineColor, int thickness);

	class Fonts {
	public:
		static ImFont* Verdana;
		static ImFont* Pixel;
		static const ImWchar FontRanges[];
		static ID3D11ShaderResourceView* LogoHack;
	};
	Fonts m_Fonts;
private:
	bool LoadImageByMemory(ID3D11Device* device, unsigned char* image, size_t image_size, ID3D11ShaderResourceView** result);

};
inline CRenderer* g_pRenderer = new CRenderer();