#include "hooks.h"

CHooksManager::CreateMove::oCreateMoveFn CHooksManager::CreateMove::oCreateMove = nullptr;
CHooksManager::PresentScene::oPresentSceneFn CHooksManager::PresentScene::oPresentScene = nullptr;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

uint8_t* CHooksManager::FindAddress(const char* moduleName, const char* pattern, const char* addressName) {
	uint8_t* address = g_pUtils->m_Memory.PatternScan(moduleName, pattern);
	if (!address) {
		g_pUtils->m_Console.Error("Failed to find address for", addressName);
	}
	else {
		g_pUtils->m_Console.Log("Found address for", addressName);
	}
	return address;
}

bool CHooksManager::CreateHook(uint8_t* targetAddress, void* hookFunction, void** originalFunction, const char* hookName)
{
	if (MH_CreateHook(targetAddress, hookFunction, originalFunction) != MH_OK) {
		g_pUtils->m_Console.Error("Failed to create Hook", hookName);
		return false;
	}
	else {
		g_pUtils->m_Console.Log("Created Hook", hookName);
		return true;
	}
}

bool CHooksManager::Init()
{
	uint8_t* CreateMoveAddress = FindAddress("client.dll", "85 D2 0F 85 ? ? ? ? 48 8B C4 44 88 40", "CreateMove");
	uint8_t* GameOverlayAddress = FindAddress("GameOverlayRenderer64.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 41 8B E8", "PresentScene");

	MH_Initialize();

	CreateHook(CreateMoveAddress, reinterpret_cast<void*>(&CHooksManager::CreateMove::Hook), reinterpret_cast<void**>(&CHooksManager::CreateMove::oCreateMove), "CreateMove");
	CreateHook(GameOverlayAddress, reinterpret_cast<void*>(&CHooksManager::PresentScene::Hook), reinterpret_cast<void**>(&CHooksManager::PresentScene::oPresentScene), "PresentScene");

	MH_EnableHook(MH_ALL_HOOKS);

	return true;
}

void CHooksManager::Destroy()
{
	reinterpret_cast<WNDPROC>(SetWindowLongPtr(m_PresentScene.outputWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_WindowProc.WndProc)));
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

void __fastcall CHooksManager::CreateMove::Hook(void* ecx, int edx, char a2)
{
	oCreateMove(ecx, edx, a2);

	Globals::LocalPlayerController = C_PlayerController::GetLocalPlayerController();
	if (!Globals::LocalPlayerController)
		return oCreateMove(ecx, edx, a2);
	Globals::LocalPlayerPawn = (C_PlayerPawn*)g_pInterfaces->m_Interfaces.pEntityList->GetClientEntityFromHandle(Globals::LocalPlayerController->GetHandlePawn());
	if (!Globals::LocalPlayerPawn)
		return oCreateMove(ecx, edx, a2);


	std::cout<<"pawn: "<<Globals::LocalPlayerPawn<<std::endl;
	std::cout<<"controller: "<<Globals::LocalPlayerController<<std::endl;

}

HRESULT __stdcall CHooksManager::PresentScene::Hook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!g_pHooksManager->m_PresentScene.init)
	{
		ID3D11Device* device{ };
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device)))
		{
			device->GetImmediateContext(&g_pHooksManager->m_PresentScene.context);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			g_pHooksManager->m_PresentScene.outputWindow = sd.OutputWindow;
			g_pHooksManager->m_WindowProc.WndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(g_pHooksManager->m_PresentScene.outputWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(g_pHooksManager->m_WindowProc.Hook)));

			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

			if (pBackBuffer) {
				device->CreateRenderTargetView(pBackBuffer, NULL, &g_pHooksManager->m_PresentScene.renderTargetView);
				pBackBuffer->Release();

				g_pRenderer->Init(g_pHooksManager->m_PresentScene.outputWindow, device, g_pHooksManager->m_PresentScene.context);

				g_pHooksManager->m_PresentScene.init = true;
			}
		}

		else
			return oPresentScene(pSwapChain, SyncInterval, Flags);
	}


	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	g_pFeatures->m_ESP.Players();

	g_pGui->DrawGui();

	ImGui::EndFrame();
	ImGui::Render();

	if (g_pHooksManager->m_PresentScene.context)
		g_pHooksManager->m_PresentScene.context->OMSetRenderTargets(1, &g_pHooksManager->m_PresentScene.renderTargetView, NULL);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return oPresentScene(pSwapChain, SyncInterval, Flags);
}

LRESULT CALLBACK CHooksManager::WindowProc::Hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN && wParam == VK_INSERT) {
		g_pGui->IsOpen = !g_pGui->IsOpen;
		if (g_pInterfaces->m_Interfaces.pEngineClient->IsInGame()) {
			if (g_pInterfaces->m_Interfaces.pSystemInput->IsRelativeMouseMode()) {
				g_pUtils->m_Memory.fnSetRelativeMouseMode(!g_pGui->IsOpen);
				SetCursorPos(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
			}
		}
	}

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam) || g_pGui->IsOpen)
		return true;

	if (g_pGui->IsOpen) {
		switch (uMsg) {
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
			return 0;

		default:
			break;
		}
	}
	return CallWindowProc(g_pHooksManager->m_WindowProc.WndProc, hWnd, uMsg, wParam, lParam);
}