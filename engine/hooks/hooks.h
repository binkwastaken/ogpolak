#pragma once
#include "../../extensions/minhook/minhook.h"

#include "../utils/utils.h"

#include "../interfaces/manager.h"

#include "../../extensions/imgui.h"

#include "../gui/gui.h"

#include "../renderer/renderer.h"

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
};
inline CHooksManager* g_pHooksManager = new CHooksManager();