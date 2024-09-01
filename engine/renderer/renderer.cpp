#include "renderer.h"

#include "../interfaces/manager.h"

ImFont* CRenderer::Fonts::Verdana = nullptr;
ImFont* CRenderer::Fonts::Pixel = nullptr;
ID3D11ShaderResourceView* CRenderer::Fonts::LogoHack = nullptr;

void* HeapAlloc(const std::size_t nSize)
{
	const HANDLE hHeap = ::GetProcessHeap();
	return ::HeapAlloc(hHeap, 0UL, nSize);
}

void HeapFree(void* pMemory)
{
	if (pMemory != nullptr)
	{
		const HANDLE hHeap = ::GetProcessHeap();
		::HeapFree(hHeap, 0UL, pMemory);
	}
}
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

static void* __cdecl ImGuiAllocWrapper(const std::size_t nSize, [[maybe_unused]] void* pUserData = nullptr)
{
	return HeapAlloc(nSize);
}

static void __cdecl ImGuiFreeWrapper(void* pMemory, [[maybe_unused]] void* pUserData = nullptr) noexcept
{
	HeapFree(pMemory);
}


bool CRenderer::LoadImageByMemory(ID3D11Device* device, unsigned char* image, size_t image_size, ID3D11ShaderResourceView** result)
{
	D3DX11_IMAGE_LOAD_INFO information;
	ID3DX11ThreadPump* thread = nullptr;
	auto hres = D3DX11CreateShaderResourceViewFromMemory(device, image, image_size, &information, thread, result, 0);
	return (hres == S_OK);
}


bool CRenderer::Init(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext) {

	if (Initialized)
		return true;

	ImGui::SetAllocatorFunctions(ImGuiAllocWrapper, ImGuiFreeWrapper);

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

	Initialized = io.Fonts->Build();

	return Initialized;
}

void CRenderer::RenderDrawData(ImDrawList* pDrawList)
{
	std::unique_lock<std::shared_mutex> lock(drawMutex);

	if (!g_pInterfaces->m_Interfaces.pEngineClient->IsInGame())
		return;

	if (vecSafeDrawData.empty())
		return;

	for (const auto& data : vecSafeDrawData)
	{
		if (!data.pObject.has_value())
			continue;

		switch (data.nType)
		{
		case EDrawType::RECT:
		{
			const auto& pObject = std::any_cast<RectObject_t>(data.pObject);
			m_RenderType.m_DrawData.m_AddDrawListes.AddDrawListRect(pDrawList, pObject.vecMin, pObject.vecMax, pObject.colRect);
			break;
		}
		case EDrawType::FILLED_RECT:
		{
			const auto& pObject = std::any_cast<RectObject_t>(data.pObject);
			m_RenderType.m_DrawData.m_AddDrawListes.AddDrawListFilledRect(pDrawList, pObject.vecMin, pObject.vecMax, pObject.colRect);
			break;
		}
		case EDrawType::LINE:
		{
			const auto& pObject = std::any_cast<LineObject_t>(data.pObject);
			m_RenderType.m_DrawData.m_AddDrawListes.AddDrawListLine(pDrawList, pObject.vecStart, pObject.vecEnd, pObject.colLine, pObject.thickness);
			break;
		}
		case EDrawType::TEXT:
		{
			const auto& pObject = std::any_cast<TextObject_t>(data.pObject);
			m_RenderType.m_DrawData.m_AddDrawListes.AddDrawListString(pDrawList, pObject.pFont, pObject.flFontSize, pObject.vecPosition, pObject.szText.c_str(), pObject.colText, pObject.centered);
			break;
		}
		case EDrawType::TEXT_OUTLINED:
		{
			const auto& pObject = std::any_cast<TextOutlinedObject_t>(data.pObject);
			m_RenderType.m_DrawData.m_AddDrawListes.AddDrawListStringOutlined(pDrawList, pObject.pFont, pObject.flFontSize,pObject.vecPosition, pObject.szText.c_str(), pObject.colText, pObject.outlineColor, pObject.centered);
			break;
		}
		//case EDrawType::LINEAR_GRADIENT: w sumie chuj w to bo nie uzywam ale robisz tak jak normalnie bys robi³
		//{
		//	const auto& pObject = std::any_cast<GradientObject_t>(data.pObject);
		//	m_RenderType.m_DrawData.AddDrawListLinearGradient(pDrawList, pObject.x, pObject.y, pObject.w, pObject.h, pObject.color1, pObject.color2, pObject.isVertical);
		//	break;
		//}
		default:
			break;
		}
	}
}
void CRenderer::ClearDrawData()
{
	if (!vecDrawData.empty())
		vecDrawData.clear();
}

void CRenderer::SwapDrawData()
{
	std::unique_lock<std::shared_mutex> lock(drawMutex);
	vecDrawData.swap(vecSafeDrawData);
}


void CRenderer::RenderType::Safe::DrawRect(const Vector2D& start, const Vector2D& end, const color_t& color)
{
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)), 0, 0, 1.f);
}

void CRenderer::RenderType::Safe::DrawRect(int x, int y, int w, int h, const color_t& color)
{
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)), 0, 0, 1.f);
}

void CRenderer::RenderType::Safe::DrawFilledRect(const Vector2D& start, const Vector2D& end, const color_t& color)
{
	
	ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)));
}

void CRenderer::RenderType::Safe::DrawFilledRect(int x, int y, int w, int h, const color_t& color)
{
	
	ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)));
}

void CRenderer::RenderType::Safe::DrawLine(const Vector2D& start, const Vector2D& end, const color_t& color, int thickness)
{
	
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)), thickness);
}

void CRenderer::RenderType::Safe::DrawLine(int x, int y, int x2, int y2, const color_t& color, int thickness)
{
	
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f)), thickness);
}

void CRenderer::RenderType::Safe::DrawString(const Vector2D& vecPosition, const char* szText, const color_t& colText, bool centered)
{
	if (centered)
	{
		ImVec2 size = ImGui::CalcTextSize(szText);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(vecPosition.x - size.x * 0.5f, vecPosition.y - size.y * 0.5f), ImGui::ColorConvertFloat4ToU32(ImVec4(colText.r / 255.f, colText.g / 255.f, colText.b / 255.f, colText.a / 255.f)), szText);	}
	else
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(vecPosition.x, vecPosition.y), ImGui::ColorConvertFloat4ToU32(ImVec4(colText.r / 255.f, colText.g / 255.f, colText.b / 255.f, colText.a / 255.f)), szText);
}

void CRenderer::RenderType::Safe::DrawStringOutlined(const Vector2D& vecPosition, const char* szText, const color_t& colText, const color_t& outlineColor, bool centered)
{
	ImVec2 pos = ImVec2(vecPosition.x, vecPosition.y);

	if (centered)
	{
		ImVec2 size = ImGui::CalcTextSize(szText);
		pos.x -= size.x * 0.5f;
		pos.y -= size.y * 0.5f;

	}
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			if (i != 0 || j != 0) {
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(pos.x + i, pos.y + j), ImGui::ColorConvertFloat4ToU32(ImVec4(outlineColor.r / 255.f, outlineColor.g / 255.f, outlineColor.b / 255.f, outlineColor.a / 255.f)), szText);
			}
		}
	}

	ImGui::GetBackgroundDrawList()->AddText(ImVec2(pos.x, pos.y), ImGui::ColorConvertFloat4ToU32(ImVec4(colText.r / 255.f, colText.g / 255.f, colText.b / 255.f, colText.a / 255.f)), szText);
}

void CRenderer::RenderType::Safe::LinearGradient(int x, int y, int w, int h, const color_t& color1, const color_t& color2, bool isVertical)
{
		ImVec2 gradientStart(x, y);
		ImVec2 gradientEnd = isVertical ? ImVec2(x, y + h) : ImVec2(x + w, y);
	
		ImU32 col1 = ImColor(color1.r / 255.0f, color1.g / 255.0f, color1.b / 255.0f, color1.a / 255.0f);
		ImU32 col2 = ImColor(color2.r / 255.0f, color2.g / 255.0f, color2.b / 255.0f, color2.a / 255.0f);
	
		if (isVertical)
		{
			ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(
				gradientStart, ImVec2(x + w, y + h),
				col1, col1, col2, col2
			);
		}
		else
		{
			ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(
				gradientStart, ImVec2(x + w, y + h),
				col1, col2, col2, col1
			);
		}
}




void CRenderer::RenderType::DrawData::DrawRect(const Vector2D& start, const Vector2D& end, const color_t& color)
{
	g_pRenderer->vecDrawData.emplace_back(EDrawType::RECT, std::make_any<RectObject_t>(start, end, color));
}

void CRenderer::RenderType::DrawData::AddDrawListes::AddDrawListFilledRect(ImDrawList* pDrawList, const Vector2D& vecMin, const Vector2D& vecMax, const color_t& colRect)
{
	pDrawList->AddRectFilled(ImVec2(vecMin.x, vecMin.y), ImVec2(vecMax.x, vecMax.y), ImGui::ColorConvertFloat4ToU32(ImVec4(colRect.r / 255.f, colRect.g / 255.f, colRect.b / 255.f, colRect.a / 255.f)), 0, 0);
}

void CRenderer::RenderType::DrawData::AddDrawListes::AddDrawListLine(ImDrawList* pDrawList, const Vector2D& vecStart, const Vector2D& vecEnd, const color_t& colLine, int thickness)
{
	pDrawList->AddLine(ImVec2(vecStart.x, vecStart.y), ImVec2(vecEnd.x, vecEnd.y), ImGui::ColorConvertFloat4ToU32(ImVec4(colLine.r / 255.f, colLine.g / 255.f, colLine.b / 255.f, colLine.a / 255.f)), thickness);
}

void CRenderer::RenderType::DrawData::AddDrawListes::AddDrawListRect(ImDrawList* pDrawList, const Vector2D& vecMin, const Vector2D& vecMax, const color_t& colRect)
{
	pDrawList->AddRect(ImVec2(vecMin.x, vecMin.y), ImVec2(vecMax.x, vecMax.y), ImGui::ColorConvertFloat4ToU32(ImVec4(colRect.r / 255.f, colRect.g / 255.f, colRect.b / 255.f, colRect.a / 255.f)), 0, 0, 1.f);
}

void CRenderer::RenderType::DrawData::DrawRect(int x, int y, int w, int h, const color_t& color)
{
	g_pRenderer->vecDrawData.emplace_back(EDrawType::RECT, std::make_any<RectObject_t>(Vector2D(x, y), Vector2D(x + w, y + h), color));
}


void CRenderer::RenderType::DrawData::AddDrawListes::AddDrawListString(ImDrawList* pDrawList, ImFont* pFont,float flFontSize, const Vector2D& vecPosition, const std::string& szText, const color_t& colText, bool centered)
{
	if (!pDrawList || !szText.c_str()) return;


	pDrawList->PushTextureID(pFont->ContainerAtlas->TexID);


	ImVec2 textSize = pFont->CalcTextSizeA(flFontSize, FLT_MAX, 0.0f, szText.c_str());
	ImVec2 pos = centered ? ImVec2(vecPosition.x - textSize.x * 0.5f, vecPosition.y - textSize.y * 0.5f) : ImVec2(vecPosition.x, vecPosition.y);

	pDrawList->AddText(pos, IM_COL32(colText.r, colText.g, colText.b, colText.a), szText.c_str());

	pDrawList->PopTextureID();
}

void CRenderer::RenderType::DrawData::AddDrawListes::AddDrawListStringOutlined(ImDrawList* pDrawList, ImFont* pFont, float flFontSize, const Vector2D& vecPosition, const std::string& szText, const color_t& colText, const color_t& colOutline, bool centered)
{
	if (!pDrawList || !szText.c_str()) return;


	pDrawList->PushTextureID(pFont->ContainerAtlas->TexID);

	ImVec2 textSize = pFont->CalcTextSizeA(pFont->FontSize, FLT_MAX, 0.0f, szText.c_str());

	ImVec2 pos = centered ? ImVec2(vecPosition.x - textSize.x * 0.5f, vecPosition.y - textSize.y * 0.5f) : ImVec2(vecPosition.x, vecPosition.y);

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			if (x != 0 || y != 0) {
				pDrawList->AddText(
					pFont, pFont->FontSize,
					ImVec2(pos.x + x, pos.y + y),
					ImGui::ColorConvertFloat4ToU32(ImVec4(colOutline.r / 255.f, colOutline.g / 255.f, colOutline.b / 255.f, colOutline.a / 255.f)),
					szText.c_str()
				);
			}
		}
	}

	pDrawList->AddText(
		pFont, pFont->FontSize,
		pos,
		ImGui::ColorConvertFloat4ToU32(ImVec4(colText.r / 255.f, colText.g / 255.f, colText.b / 255.f, colText.a / 255.f)),
		szText.c_str());

	pDrawList->PopTextureID();
}


void CRenderer::RenderType::DrawData::DrawString(ImFont* pFont, const std::string& text, const Vector2D& position, const color_t& color, bool centered)
{
	if (pFont->ContainerAtlas == nullptr)
		return;
	g_pRenderer->vecDrawData.emplace_back(EDrawType::TEXT, std::make_any<TextObject_t>(pFont, pFont->FontSize, position, text, color, centered));
}

void CRenderer::RenderType::DrawData::DrawStringOutlined(ImFont* pFont, const std::string& text, const Vector2D& position, const color_t& color, const color_t& outlineColor, bool centered)
{
	if (pFont->ContainerAtlas == nullptr)
		return;
	g_pRenderer->vecDrawData.emplace_back(EDrawType::TEXT_OUTLINED, std::make_any<TextOutlinedObject_t>(pFont, pFont->FontSize, position, text, color, outlineColor, centered));
}

void CRenderer::RenderType::DrawData::DrawFilledRect(const Vector2D& start, const Vector2D& end, const color_t& color)
{
	g_pRenderer->vecDrawData.emplace_back(EDrawType::FILLED_RECT, std::make_any<RectObject_t>(start, end, color));
}

void CRenderer::RenderType::DrawData::DrawFilledRect(int x, int y, int w, int h, const color_t& color)
{
	g_pRenderer->vecDrawData.emplace_back(EDrawType::FILLED_RECT, std::make_any<RectObject_t>(Vector2D(x, y), Vector2D(x + w, y + h), color));
}

void CRenderer::RenderType::DrawData::DrawLine(const Vector2D& start, const Vector2D& end, const color_t& color)
{
	g_pRenderer->vecDrawData.emplace_back(EDrawType::LINE, std::make_any<LineObject_t>(start, end, color));
}

void CRenderer::RenderType::DrawData::DrawLine(int x, int y, int x2, int y2, const color_t& color)
{
	g_pRenderer->vecDrawData.emplace_back(EDrawType::LINE, std::make_any<LineObject_t>(Vector2D(x, y), Vector2D(x2, y2), color));
}
