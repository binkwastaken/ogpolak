#include "renderer.h"
ImFont* CRenderer::Fonts::Verdana = nullptr;
ImFont* CRenderer::Fonts::Pixel = nullptr;

const ImWchar CRenderer::Fonts::FontRanges[] = {
	0x0020, 0x00FF, // Basic Latin + Latin Supplement
	0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
	0x2DE0, 0x2DFF, // Cyrillic Extended-A
	0xA640, 0xA69F, // Cyrillic Extended-B
	0xE000, 0xE226, // icons
	0,
};

bool CRenderer::LoadImageByMemory(ID3D11Device* device, unsigned char* image, size_t image_size, ID3D11ShaderResourceView** result)
{
	D3DX11_IMAGE_LOAD_INFO information;
	ID3DX11ThreadPump* thread = nullptr;
	auto hres = D3DX11CreateShaderResourceViewFromMemory(device, image, image_size, &information, thread, result, 0);
	return (hres == S_OK);
}


bool CRenderer::Init(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext) {


	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);

	bool imageResult = LoadImageByMemory(pDevice, PoseidonLogo, sizeof(PoseidonLogo), &LogoHack);
	if (!imageResult)
	{
		g_pUtils->m_Console.Error("Failed to load image from memory.");
		return false;
	}
	else
	{
		g_pUtils->m_Console.Success("Logo loaded successfully.");
	}

	Fonts::Verdana = io.Fonts->AddFontFromMemoryTTF(Verdana, sizeof(Verdana), 10.f, NULL, Fonts::FontRanges);

	Fonts::Pixel = io.Fonts->AddFontFromMemoryTTF(SmallestPixel, sizeof(SmallestPixel), 10.f, NULL, Fonts::FontRanges);

	g_pUtils->m_Console.Success("Renderer initialized.");

	ImGui_ImplDX11_CreateDeviceObjects();

	return true;
}