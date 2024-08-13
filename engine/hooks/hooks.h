#pragma once
#include "../../extensions/minhook/minhook.h"

#include "../utils/utils.h"

#include "../interfaces/manager.h"

#include "../../extensions/imgui.h"

#include "../gui/gui.h"

#include "../renderer/renderer.h"

#include "../features/features.h"

#include "../../sdk/globals.h"

class CHooksManager {
public:
	bool Init();
	void Destroy();
private:

	bool CreateHook(uint8_t* targetAddress, void* hookFunction, void** originalFunction, const char* hookName);
	uint8_t* FindAddress(const char* moduleName, const char* pattern, const char* addressName);


	class CreateMove {
	public:
		typedef void(__fastcall* oCreateMoveFn)(void*, int, char);
		static oCreateMoveFn oCreateMove;
		static void __fastcall Hook(void* ecx, int edx, char a2);
	};
	CreateMove m_CreateMove;

	class PresentScene {
	public:
		typedef HRESULT(__fastcall* oPresentSceneFn) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
		static oPresentSceneFn oPresentScene;
		static HRESULT __fastcall Hook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
		HWND outputWindow = nullptr;

		bool init{ };
		ID3D11DeviceContext* context{ };
		ID3D11RenderTargetView* renderTargetView;
	};
	PresentScene m_PresentScene;
	class WindowProc {
	public:
		WNDPROC WndProc;
		static LRESULT CALLBACK Hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};
	WindowProc m_WindowProc;

	class GlowObjects {
	public:
		typedef void(__fastcall* oGlowObjectsFn)(C_GlowProperty*, float*);
		typedef bool(__fastcall* oIsGlowingFn)(C_GlowProperty*);
		static oIsGlowingFn oIsGlowing;
		static oGlowObjectsFn oGlowObjects;
		static void __fastcall Hook(C_GlowProperty* glowProperty, float* colour);
		static bool __fastcall HookIsGlowing(C_GlowProperty* glowProperty);
	};
	GlowObjects m_GlowObjects;

	class LightingModulation
	{
	public:
		typedef void*(__fastcall* oLightingModulationFn)(__int64 a1, CAggregateSceneObject* a2, __int64 a3);
		static oLightingModulationFn oLightingModulation;
		static void* __fastcall Hook(__int64 a1, CAggregateSceneObject* a2, __int64 a3);
	};
	LightingModulation m_LightingModulation;

	class WorldModulation
	{
	public:
		typedef void* (__fastcall* oModulateWorldColorFn)(CAggregateSceneObjectWorld*, void*);
		static oModulateWorldColorFn oModulateWorldColor;
		static void* __fastcall Hook(CAggregateSceneObjectWorld* pAggregateSceneObject, void* a2);
	};
	WorldModulation m_WorldModulation;

	class BombCode
	{
	public:
		typedef void(__fastcall* oBombCodeFn)(void* a1);
		static oBombCodeFn oBombCode;
		static void __fastcall Hook(void* a1);
	};
	BombCode m_BombCode;

	class DrawObject {
	public:
		typedef void(__fastcall* oDrawObjectFn)(void*, void*, void*, int, void*, void*, void*, void*);
		static oDrawObjectFn oDrawObject;
		static void __fastcall Hook(void* a1, void* a2, void* a3, int a4, void* a5, void* a6, void* a7, void* a8);
	};
	DrawObject m_DrawObject;

	class RenderStart {
	public:
		typedef void(__fastcall* oRenderStartFn)(CViewSetup*);
		static oRenderStartFn oRenderStart;
		static void __fastcall Hook(CViewSetup* pSetup);
	};
	RenderStart m_RenderStart;

};
inline CHooksManager* g_pHooksManager = new CHooksManager();