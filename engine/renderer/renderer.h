#pragma once
#include "../utils/utils.h"
#include "../../extensions/imgui.h"
#include "../../extensions/fonts.h"

#include <D3DX11.h>
#include <d3d11.h>
#include <D3DX11tex.h>

#pragma comment (lib,"D3DX11.lib")


class CRenderer {
public:
	bool Init(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	ID3D11ShaderResourceView* LogoHack;

	bool LoadImageByMemory(ID3D11Device* device, unsigned char* image, size_t image_size, ID3D11ShaderResourceView** result);

	class Fonts {
	public:
		static ImFont* Verdana;
		static ImFont* Pixel;
		static const ImWchar FontRanges[];
	};
	Fonts m_Fonts;
private:
};
inline CRenderer* g_pRenderer = new CRenderer();