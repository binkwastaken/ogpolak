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
		typedef void(__fastcall* oCreateMoveFn)(CGameInput*, int, char);
		static oCreateMoveFn oCreateMove;
		static void __fastcall Hook(CGameInput* input, int edx, char a2);
	};
	CreateMove m_CreateMove;

	class PresentScene {
	public:
		typedef HRESULT(__stdcall* oPresentSceneFn) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
		static oPresentSceneFn oPresentScene;
		static HRESULT __stdcall Hook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
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

	class FrameStage {
	public:
		typedef void(__fastcall* oFrameStageFn)(void* rcx,int stage);
		static oFrameStageFn oFrameStage;
		static void __fastcall Hook(void* rcx,int stage);
	};
	FrameStage m_FrameStage;

	class ForceCrosshair {
	public:
		typedef bool(__fastcall* oForceCrosshairFn)(__int64*);
		static oForceCrosshairFn oForceCrosshair;
		static bool __fastcall Hook(__int64* a1);
	};
	ForceCrosshair m_ForceCrosshair;
	

	class RemoveZoomOverlay {
	public:
		typedef bool(__fastcall* oRemoveZoomOverlayFn)(void*,void*);
		static oRemoveZoomOverlayFn oRemoveZoomOverlay;
		static bool __fastcall Hook(void* a1,void* a2);
	};
	RemoveZoomOverlay m_RemoveZoomOverlay;

	class NoSmokeHook {
	public:
		typedef bool(__fastcall* oNoSmokeFn)(void* a1, void* a2, void* a3, void* a4, void* a5, void* a6);
		static oNoSmokeFn oNoSmoke;
		static bool __fastcall Hook(void* a1, void* a2, void* a3, void* a4, void* a5, void* a6);
	};
	NoSmokeHook m_NoSmoke;

	class RemoveLegs {
	public:
		typedef bool(__fastcall* oRemoveLegsFn)(void*, void*, void*, void*, void*);
		static oRemoveLegsFn oRemoveLegs;
		static bool __fastcall Hook(void* a1, void* a2, void* a3, void* a4, void* a5);
	};
	RemoveLegs m_RemoveLegs;

	class OverrideView {
	public:
		typedef void(__fastcall* oOverrideViewFn)(void*,CViewSetup*);
		static oOverrideViewFn oOverrideView;
		static void __fastcall Hook(void* a1, CViewSetup* a2);
	};
	OverrideView m_OverrideView;

	class FovObject {
	public:
		typedef float(__fastcall* oFovObjectFn)(void*);
		static oFovObjectFn oFovObject;
		static float __fastcall Hook(void* a1);
	};
	FovObject m_FovObject;

	class ViewModel {
	public:
		typedef void*(__fastcall* oViewModelFn)(float*, float*, float*);
		static oViewModelFn oViewModel;
		static void* __fastcall Hook(float* a1, float* offset, float* fov);
	};
	ViewModel m_ViewModel;


	class CalcViewModelAngles {
	public:
		typedef void(__fastcall* oViewModelCalcFn)(DWORD* a1, __int64 a2, Vector3D a3, Vector3D a4, char a5);
		static oViewModelCalcFn oViewModelCalc;
		static void __fastcall Hook(DWORD* a1, __int64 a2, Vector3D a3, Vector3D a4, char a5);
	};
	CalcViewModelAngles m_CalcViewModelAngles;

	class NoFlashbangEffect {
	public:
		typedef void*(__fastcall* oNoFlashbangEffectFn)(__int64 a1, __int64 a2, float* a3);
		static oNoFlashbangEffectFn oNoFlashbangEffect;
		static void* __fastcall Hook(__int64 a1, __int64 a2, float* a3);
	};
	NoFlashbangEffect m_NoFlashbangEffect;


	class IsRelativeMouseMode {
	public:
		typedef void*(__fastcall* oIsRelativeMouseModeFn)(void* a1,bool a2);
		static oIsRelativeMouseModeFn oIsRelativeMouseMode;
		static void* __fastcall Hook(void* a1, bool a2);
	};
	IsRelativeMouseMode m_IsRelativeMouseMode;

};
inline CHooksManager* g_pHooksManager = new CHooksManager();