#include "renderer.h"
ImFont* CRenderer::Fonts::Verdana = nullptr;
ImFont* CRenderer::Fonts::Pixel = nullptr;
ID3D11ShaderResourceView* CRenderer::Fonts::LogoHack = nullptr;

#define pDrawList ImGui::GetBackgroundDrawList()

const ImWchar CRenderer::Fonts::FontRanges[] = {
	0x0020, 0x00FF, // Basic Latin + Latin Supplement
	0x0100, 0x017F, // Latin Extended-A
	0x0180, 0x024F, // Latin Extended-B
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

	bool imageResult = LoadImageByMemory(pDevice, PoseidonLogo, sizeof(PoseidonLogo), &m_Fonts.LogoHack);
	if (!imageResult)
	{
		g_pUtils->m_Console.Error("Failed to load image from memory.");
		return false;
	}
	else
	{
		g_pUtils->m_Console.Log("Logo loaded successfully.");
	}

	Fonts::Verdana = io.Fonts->AddFontFromMemoryTTF(Verdana, sizeof(Verdana), 11.f, NULL, Fonts::FontRanges);

	Fonts::Pixel = io.Fonts->AddFontFromMemoryTTF(SmallestPixel, sizeof(SmallestPixel), 10.f, NULL, Fonts::FontRanges);

	g_pUtils->m_Console.Log("Renderer initialized.");

	ImGui_ImplDX11_CreateDeviceObjects();

	return true;
}


void CRenderer::DrawImage(ID3D11ShaderResourceView* image, int x, int y, int w, int h)
{
	pDrawList->AddImage(image, ImVec2(x, y), ImVec2(x + w, y + h));
}

void CRenderer::DrawLine(const Vector2D& start, const Vector2D& end, const color_t& color, int thickness)
{
	pDrawList->AddLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)), thickness);
}

void CRenderer::DrawLine(int x, int y, int x2, int y2, const color_t& color, int thickness)
{
	pDrawList->AddLine(ImVec2(x, y), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)), thickness);
}

void CRenderer::DrawRect(const Vector2D& start, const Vector2D& end, const color_t& color)
{
	pDrawList->AddRect(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)));
}

void CRenderer::DrawRect(int x, int y, int w, int h, const color_t& color)
{
	pDrawList->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)));
}

void CRenderer::DrawFilledRect(const Vector2D& start, const Vector2D& end, const color_t& color)
{
	pDrawList->AddRectFilled(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)));
}

void CRenderer::DrawFilledRect(int x, int y, int w, int h, const color_t& color)
{
	pDrawList->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)));
}

void CRenderer::DrawString(const char* text, const Vector2D& pos, const color_t& color, const bool centered)
{
	if (centered)
	{
		ImVec2 size = ImGui::CalcTextSize(text);
		pDrawList->AddText(ImVec2(pos.x - size.x * 0.5f, pos.y - size.y * 0.5f), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)), text);
	}
	else
		pDrawList->AddText(ImVec2(pos.x, pos.y), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)), text);
}

void CRenderer::DrawTriangle(const Vector2D& a, const Vector2D& b, const Vector2D& c, const color_t& color, bool fillled)
{
	if (fillled)
		pDrawList->AddTriangleFilled(ImVec2(a.x, a.y), ImVec2(b.x, b.y), ImVec2(c.x, c.y), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)));
	else
		pDrawList->AddTriangle(ImVec2(a.x, a.y), ImVec2(b.x, b.y), ImVec2(c.x, c.y), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)));
}
void CRenderer::DrawOutlinedString(const char* text, Vector2D position, color_t textColor, color_t outlineColor, bool centered)
{
	ImVec2 pos = ImVec2(position.x, position.y);

	if (centered)
	{
		ImVec2 size = ImGui::CalcTextSize(text);
		pos.x -= size.x * 0.5f;
		pos.y -= size.y * 0.5f;
	}

	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			if (i != 0 || j != 0) {

				pDrawList->AddText(ImVec2(pos.x + i, pos.y + j), ImGui::GetColorU32(ImVec4(outlineColor.r / 255.f, outlineColor.g / 255.f, outlineColor.b / 255.f, outlineColor.a / 255.f)), text);
			}
		}
	}

	pDrawList->AddText(pos, ImGui::GetColorU32(ImVec4(textColor.r / 255.f, textColor.g / 255.f, textColor.b / 255.f, textColor.a / 255.f)), text);
}

void CRenderer::DrawCapsule(const Vector2D& start, const Vector2D& end, float radius, const color_t& color, bool fillled)
{
	if (fillled)
		pDrawList->AddCircleFilled(ImVec2(start.x, start.y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)));
	else
		pDrawList->AddCircle(ImVec2(start.x, start.y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)));
}
void CRenderer::DrawOutlinedLine(const Vector2D& start, const Vector2D& end, const color_t& color, int thickness, const color_t& outlineColor)
{
	// Draw outline
	int outlineThickness = thickness + 2; // Example: outline thickness is 2 pixels more than the main line

	// Draw top and bottom outlines
	pDrawList->AddLine(ImVec2(start.x - 1, start.y), ImVec2(end.x - 1, end.y), ImGui::ColorConvertFloat4ToU32(ImVec4(outlineColor.r / 255.f, outlineColor.g / 255.f, outlineColor.b / 255.f, outlineColor.a / 255.f)), outlineThickness);
	pDrawList->AddLine(ImVec2(start.x + 1, start.y), ImVec2(end.x + 1, end.y), ImGui::ColorConvertFloat4ToU32(ImVec4(outlineColor.r / 255.f, outlineColor.g / 255.f, outlineColor.b / 255.f, outlineColor.a / 255.f)), outlineThickness);

	// Draw left and right outlines
	pDrawList->AddLine(ImVec2(start.x, start.y - 1), ImVec2(end.x, end.y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(outlineColor.r / 255.f, outlineColor.g / 255.f, outlineColor.b / 255.f, outlineColor.a / 255.f)), outlineThickness);
	pDrawList->AddLine(ImVec2(start.x, start.y + 1), ImVec2(end.x, end.y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(outlineColor.r / 255.f, outlineColor.g / 255.f, outlineColor.b / 255.f, outlineColor.a / 255.f)), outlineThickness);

	// Draw main line
	pDrawList->AddLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)), thickness);
}

void CRenderer::DrawOutlinedLine(int x, int y, int x2, int y2, const color_t& color, const color_t& outlineColor, int thickness)
{
	// Convert integer coordinates to vector coordinates
	Vector2D start(x, y);
	Vector2D end(x2, y2);

	// Call the vector version of DrawOutlinedLine
	DrawOutlinedLine(start, end, color, thickness, outlineColor);
}
