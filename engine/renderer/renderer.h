#pragma once
#include "../utils/utils.h"
#include "../../extensions/imgui.h"
#include "../../extensions/fonts.h"
#include "../../sdk/globals.h"

#include "../../sdk/math/mathematics.h"

#include <D3DX11.h>
#include <d3d11.h>
#include <D3DX11tex.h>
#include <mutex>
#include <shared_mutex>
#include <deque>
#include <any>

#pragma comment (lib,"D3DX11.lib")


enum class EDrawType : int
{
	NONE = 0,
	LINE,
	RECT,
	FILLED_RECT,
	TEXT_OUTLINED,
	TEXT,
	LINEAR_GRADIENT,
};

struct DrawObject_t
{
	DrawObject_t(const EDrawType nType, std::any&& pObject) :
		nType(nType), pObject(std::move(pObject)) { }

	EDrawType nType = EDrawType::NONE;
	std::any pObject = { };
};

struct RectObject_t
{
	Vector2D vecMin = { };
	Vector2D vecMax = { };
	color_t colRect;
};

struct LineObject_t {
	Vector2D vecStart; 
	Vector2D vecEnd;    
	color_t colLine;  
	int thickness;     
};

struct TextObject_t {
	ImFont* pFont = nullptr;
	float flFontSize = 0.f;
	Vector2D vecPosition;  
	std::string szText = {};
	color_t colText;      
	bool centered;       
};

struct TextOutlinedObject_t {
	ImFont* pFont = nullptr;
	float flFontSize = 0.f;
	Vector2D vecPosition;     
	std::string szText = {};
	color_t colText;          
	color_t outlineColor;    
	bool centered;           
};

struct GradientObject_t {
	int x;             
	int y;              
	int w;              
	int h;             
	color_t color1;     
	color_t color2;     
	bool isVertical;    
};


class CRenderer {
public:
	bool Init(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	void RenderDrawData(ImDrawList* pDrawList);
	void ClearDrawData();
	void SwapDrawData();

	class RenderType {
	public:

		class DrawData { // onrenderstart i gdziekolwiek
		public:
			 void DrawRect(const Vector2D& start, const Vector2D& end, const color_t& color);
			void DrawRect(int x, int y, int w, int h, const color_t& color);

			void DrawString(ImFont* pFont, const std::string& text, const Vector2D& position, const color_t& color, bool centered = false);
			void DrawStringOutlined(ImFont* pFont, const std::string& text, const Vector2D& position, const color_t& color, const color_t& outlineColor, bool centered = false);

			void DrawFilledRect(const Vector2D& start, const Vector2D& end, const color_t& color);
			void DrawFilledRect(int x, int y, int w, int h, const color_t& color);

			void DrawLine(const Vector2D& start, const Vector2D& end, const color_t& color);
			void DrawLine(int x, int y, int x2, int y2, const color_t& color);

			class AddDrawListes {
			public:
				void AddDrawListFilledRect(ImDrawList* pDrawList, const Vector2D& vecMin, const Vector2D& vecMax, const color_t& colRect);

				void AddDrawListLine(ImDrawList* pDrawList, const Vector2D& vecStart, const Vector2D& vecEnd, const color_t& colLine, int thickness = 1);
				void AddDrawListRect(ImDrawList* pDrawList, const Vector2D& vecMin, const Vector2D& vecMax, const color_t& colRect);
				void AddDrawListString(ImDrawList* pDrawList,ImFont* pFont, float flFontSize, const Vector2D& vecPosition, const std::string& szText, const color_t& colText, bool centered = false);
				void AddDrawListStringOutlined(ImDrawList* pDrawList, ImFont* pFont, float flFontSize, const Vector2D& vecPosition, const std::string& szText, const color_t& colText, const color_t& colOutline, bool centered = false);
				};
			AddDrawListes m_AddDrawListes;
		};
		DrawData m_DrawData;


		class Safe { // Menu itp presentscene render ogolnie
		public:
			void DrawRect(const Vector2D& start, const Vector2D& end, const color_t& color);
			 void DrawRect(int x, int y, int w, int h, const color_t& color);
			 void DrawFilledRect(const Vector2D& start, const Vector2D& end, const color_t& color);
			 void DrawFilledRect(int x, int y, int w, int h, const color_t& color);

			 void DrawLine(const Vector2D& start, const Vector2D& end, const color_t& color, int thickness);
			 void DrawLine(int x, int y, int x2, int y2, const color_t& color, int thickness);
			 void DrawString(const Vector2D& vecPosition, const char* szText, const color_t& colText, bool centered = false);
			 void DrawStringOutlined(const Vector2D& vecPosition, const char* szText, const color_t& colText, const color_t& outlineColor, bool centered);
			 void LinearGradient(int x, int y, int w, int h, const color_t& color1, const color_t& color2, bool isVertical);
		};
		Safe m_Safe;

	};
	RenderType m_RenderType;

	class Fonts {
	public:
		static ImFont* Verdana;
		static ImFont* Pixel;
		static const ImWchar FontRanges[];
		static ID3D11ShaderResourceView* LogoHack;
	};
	Fonts m_Fonts;


	bool Initialized = false;
	std::deque<DrawObject_t> vecDrawData = { };
	std::deque<DrawObject_t> vecSafeDrawData = { };
	std::shared_mutex drawMutex = { };

private:
	bool LoadImageByMemory(ID3D11Device* device, unsigned char* image, size_t image_size, ID3D11ShaderResourceView** result);
};
inline CRenderer* g_pRenderer = new CRenderer();