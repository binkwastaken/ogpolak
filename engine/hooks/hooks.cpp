#include "hooks.h"

CHooksManager::CreateMove::oCreateMoveFn CHooksManager::CreateMove::oCreateMove = nullptr;
CHooksManager::PresentScene::oPresentSceneFn CHooksManager::PresentScene::oPresentScene = nullptr;
CHooksManager::GlowObjects::oGlowObjectsFn CHooksManager::GlowObjects::oGlowObjects = nullptr;
CHooksManager::GlowObjects::oIsGlowingFn CHooksManager::GlowObjects::oIsGlowing = nullptr;
CHooksManager::LightingModulation::oLightingModulationFn CHooksManager::LightingModulation::oLightingModulation = nullptr;
CHooksManager::WorldModulation::oModulateWorldColorFn CHooksManager::WorldModulation::oModulateWorldColor = nullptr;
CHooksManager::BombCode::oBombCodeFn CHooksManager::BombCode::oBombCode = nullptr;
CHooksManager::DrawObject::oDrawObjectFn CHooksManager::DrawObject::oDrawObject = nullptr;
CHooksManager::RenderStart::oRenderStartFn CHooksManager::RenderStart::oRenderStart = nullptr;
CHooksManager::FrameStage::oFrameStageFn CHooksManager::FrameStage::oFrameStage = nullptr;
CHooksManager::ForceCrosshair::oForceCrosshairFn CHooksManager::ForceCrosshair::oForceCrosshair = nullptr;
CHooksManager::RemoveZoomOverlay::oRemoveZoomOverlayFn CHooksManager::RemoveZoomOverlay::oRemoveZoomOverlay = nullptr;
CHooksManager::NoSmokeHook::oNoSmokeFn CHooksManager::NoSmokeHook::oNoSmoke = nullptr;
CHooksManager::RemoveLegs::oRemoveLegsFn CHooksManager::RemoveLegs::oRemoveLegs = nullptr;
CHooksManager::OverrideView::oOverrideViewFn CHooksManager::OverrideView::oOverrideView = nullptr;
CHooksManager::FovObject::oFovObjectFn CHooksManager::FovObject::oFovObject = nullptr;
CHooksManager::ViewModel::oViewModelFn CHooksManager::ViewModel::oViewModel = nullptr;
CHooksManager::CalcViewModelAngles::oViewModelCalcFn CHooksManager::CalcViewModelAngles::oViewModelCalc = nullptr;
CHooksManager::NoFlashbangEffect::oNoFlashbangEffectFn CHooksManager::NoFlashbangEffect::oNoFlashbangEffect = nullptr;
CHooksManager::IsRelativeMouseMode::oIsRelativeMouseModeFn CHooksManager::IsRelativeMouseMode::oIsRelativeMouseMode = nullptr;

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

typedef bool(__fastcall* oReduceAimPunchFn)(__int64 a1, float* a2, float* a3);
oReduceAimPunchFn oReduceAimPunch;

bool __fastcall ReduceAimPunch(__int64 a1, float* a2, float* a3)
{
	return false;
}

bool CHooksManager::Init()
{

	uint8_t* IsRelativeMouseModeVtable =  reinterpret_cast<uint8_t*>(g_pUtils->m_VMT.GetVMT(g_pInterfaces->m_Interfaces.pSystemInput, 76));

	uint8_t* CreateMoveAddress = FindAddress("client.dll", "85 D2 0F 85 ? ? ? ? 48 8B C4 44 88 40", "CreateMove");
	uint8_t* GameOverlayAddress = FindAddress("GameOverlayRenderer64.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 41 8B E8", "PresentScene");
	uint8_t* GlowObjectAddress = g_pUtils->m_Memory.ResolveRip(FindAddress("client.dll", "E8 ? ? ? ? F3 0F 10 BE ? ? ? ? 48 8B CF", "GlowObject"), 1, 5);
	uint8_t* IsGlowingAddress = g_pUtils->m_Memory.ResolveRip(FindAddress("client.dll", "E8 ? ? ? ? 33 DB 84 C0 0F 84 ? ? ? ? 48 8B 4F", "IsGlowing"),1,5);
	uint8_t* LightingOverrideAddress = FindAddress("scenesystem.dll","48 89 54 24 ? 53 41 56 41 57", "LightingOverride");
	uint8_t* WorldOverrideAddress = FindAddress("scenesystem.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 4C 8B F9", "ModulateWorldColor");
	uint8_t* DrawObjectAddress = FindAddress("scenesystem.dll", "48 8B C4 48 89 50 ? 53", "DrawObject");
	uint8_t* OnRenderStart = FindAddress("client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 4C 8B F1 48 8D 94 24", "OnRenderStart");
	uint8_t* FrameStageAddress = FindAddress("client.dll", "48 89 5C 24 ? 56 48 83 EC ? 8B 05 ? ? ? ? 8D 5A", "FrameStageNotify");
	uint8_t* ForceCrosshairAddress = FindAddress("client.dll", "48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 85 C0", "ForceCrosshair");
	uint8_t* RemoveScopeOverlayAddress = FindAddress("client.dll", "4C 8B DC 53 56 57 48 83 EC", "RemoveScopeOverlay");
	uint8_t* NoSmokeAddress = FindAddress("client.dll","48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 48 8B 9C 24 ? ? ? ? 4D 8B F8", "NoSmoke");
	uint8_t* RemoveLegsAddress = FindAddress("client.dll", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 01", "RemoveLegs");
	uint8_t* OverrideViewAddress = FindAddress("client.dll","48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 48 8B FA E8", "OverrideView");
	uint8_t* ChangeFovAddress = FindAddress("client.dll","40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 85 C0 74 ? 48 8B C8 48 83 C4", "FovObject");
	uint8_t* ChangeViewmodelAddress = FindAddress("client.dll","48 89 5C 24 ? 55 56 57 41 56 41 57 48 83 EC ? 49 8B E8", "ChangeViewmodel");
	uint8_t* NoShootingPunchAddress = FindAddress("client.dll", "48 89 5C 24 ? 55 56 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? F2 41 0F 10 01", "NoShootingPunch");
	uint8_t* NoFlashbangEffectAddress = FindAddress("client.dll","48 89 5C 24 ? 57 48 83 EC ? 49 8B D8 48 8B F9 E8", "NoFlashbangEffect");
	//uint8_t* TestSig2 = FindAddress("client.dll", "48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B FA 48 8B D9", "TestSig");
	MH_Initialize();

	CreateHook(CreateMoveAddress, reinterpret_cast<void*>(&CHooksManager::CreateMove::Hook), reinterpret_cast<void**>(&CHooksManager::CreateMove::oCreateMove), "CreateMove");

	CreateHook(ForceCrosshairAddress, reinterpret_cast<void*>(&CHooksManager::ForceCrosshair::Hook), reinterpret_cast<void**>(&CHooksManager::ForceCrosshair::oForceCrosshair), "ForceCrosshair");

	CreateHook(GameOverlayAddress, reinterpret_cast<void*>(&CHooksManager::PresentScene::Hook), reinterpret_cast<void**>(&CHooksManager::PresentScene::oPresentScene), "PresentScene");

	CreateHook(IsGlowingAddress, reinterpret_cast<void*>(&CHooksManager::GlowObjects::HookIsGlowing), reinterpret_cast<void**>(&CHooksManager::GlowObjects::oIsGlowing), "IsGlowing");

	CreateHook(GlowObjectAddress, reinterpret_cast<void*>(&CHooksManager::GlowObjects::Hook), reinterpret_cast<void**>(&CHooksManager::GlowObjects::oGlowObjects), "GlowObject");

	CreateHook(LightingOverrideAddress, reinterpret_cast<void*>(&CHooksManager::LightingModulation::Hook), reinterpret_cast<void**>(&CHooksManager::LightingModulation::oLightingModulation), "LightingOverride");

	CreateHook(WorldOverrideAddress, reinterpret_cast<void*>(&CHooksManager::WorldModulation::Hook), reinterpret_cast<void**>(&CHooksManager::WorldModulation::oModulateWorldColor), "ModulateWorldColor");

	CreateHook(DrawObjectAddress, reinterpret_cast<void*>(&CHooksManager::DrawObject::Hook), reinterpret_cast<void**>(&CHooksManager::DrawObject::oDrawObject), "DrawObject");

	CreateHook(OnRenderStart, reinterpret_cast<void*>(&CHooksManager::RenderStart::Hook), reinterpret_cast<void**>(&CHooksManager::RenderStart::oRenderStart), "OnRenderStart");

	CreateHook(FrameStageAddress, reinterpret_cast<void*>(&CHooksManager::FrameStage::Hook), reinterpret_cast<void**>(&CHooksManager::FrameStage::oFrameStage), "FrameStageNotify");

	CreateHook(RemoveScopeOverlayAddress, reinterpret_cast<void*>(&CHooksManager::RemoveZoomOverlay::Hook), reinterpret_cast<void**>(&CHooksManager::RemoveZoomOverlay::oRemoveZoomOverlay), "RemoveScopeOverlay");

	CreateHook(NoSmokeAddress, reinterpret_cast<void*>(&CHooksManager::NoSmokeHook::Hook), reinterpret_cast<void**>(&CHooksManager::NoSmokeHook::oNoSmoke), "NoSmoke");

	CreateHook(RemoveLegsAddress, reinterpret_cast<void*>(&CHooksManager::RemoveLegs::Hook), reinterpret_cast<void**>(&CHooksManager::RemoveLegs::oRemoveLegs), "RemoveLegs");

	CreateHook(OverrideViewAddress, reinterpret_cast<void*>(&CHooksManager::OverrideView::Hook), reinterpret_cast<void**>(&CHooksManager::OverrideView::oOverrideView), "OverrideView");

	//CreateHook(ChangeFovAddress, reinterpret_cast<void*>(&CHooksManager::FovObject::Hook), reinterpret_cast<void**>(&CHooksManager::FovObject::oFovObject), "FovObject");

	CreateHook(ChangeViewmodelAddress, reinterpret_cast<void*>(&CHooksManager::ViewModel::Hook), reinterpret_cast<void**>(&CHooksManager::ViewModel::oViewModel), "ViewModel");

	CreateHook(NoShootingPunchAddress, reinterpret_cast<void*>(&CHooksManager::CalcViewModelAngles::Hook), reinterpret_cast<void**>(&CHooksManager::CalcViewModelAngles::oViewModelCalc), "ViewModelCalc");

	CreateHook(NoFlashbangEffectAddress, reinterpret_cast<void*>(&CHooksManager::NoFlashbangEffect::Hook), reinterpret_cast<void**>(&CHooksManager::NoFlashbangEffect::oNoFlashbangEffect), "NoFlashbangEffect");

	CreateHook(IsRelativeMouseModeVtable, reinterpret_cast<void*>(&CHooksManager::IsRelativeMouseMode::Hook), reinterpret_cast<void**>(&CHooksManager::IsRelativeMouseMode::oIsRelativeMouseMode), "IsRelativeMouseMode");

	//CreateHook(TestSig2, reinterpret_cast<void*>(&ReduceAimPunch), reinterpret_cast<void**>(&oReduceAimPunch), "TestSig2");


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

void __fastcall CHooksManager::CreateMove::Hook(CGameInput* input, int edx, char a2)
{
	oCreateMove(input, edx, a2);

	Globals::LocalPlayerController = C_PlayerController::GetLocalPlayerController();
	if (!Globals::LocalPlayerController)
		return oCreateMove(input, edx, a2);
	Globals::LocalPlayerPawn = (C_PlayerPawn*)g_pInterfaces->m_Interfaces.pEntityList->GetClientEntityFromHandle(Globals::LocalPlayerController->GetHandlePawn());
	if (!Globals::LocalPlayerPawn)
		return oCreateMove(input, edx, a2);

	C_UserCmd* cmd = nullptr;

	g_pFeatures->m_Aimbot.Run(cmd);

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

			ID3D11Texture2D* pBackBuffer = nullptr;
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

	if (g_pHooksManager->m_PresentScene.context)
		g_pHooksManager->m_PresentScene.context->OMSetRenderTargets(1, &g_pHooksManager->m_PresentScene.renderTargetView, NULL);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	ImDrawList* pBackgroundDrawList = ImGui::GetBackgroundDrawList();
	g_pRenderer->RenderDrawData(pBackgroundDrawList);

	g_pFeatures->m_ESP.m_OthersVisuals.Watermark();
	g_pFeatures->m_ESP.m_OthersVisuals.SniperScopeOverlay();


	g_pGui->DrawGui();



	ImGui::EndFrame();


	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


	return oPresentScene(pSwapChain, SyncInterval, Flags);
}

LRESULT CALLBACK CHooksManager::WindowProc::Hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN && wParam == VK_INSERT) {
		g_pGui->IsOpen = !g_pGui->IsOpen;
		if (g_pInterfaces->m_Interfaces.pEngineClient->IsInGame()) {
			if (g_pInterfaces->m_Interfaces.pSystemInput->IsRelativeMouseMode()) {

				if (g_pHooksManager->m_IsRelativeMouseMode.oIsRelativeMouseMode)
				{
					g_pHooksManager->m_IsRelativeMouseMode.oIsRelativeMouseMode(g_pInterfaces->m_Interfaces.pSystemInput, !g_pGui->IsOpen);
				}

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

void __fastcall CHooksManager::GlowObjects::Hook(C_GlowProperty* glowProperty, float* colour)
{
	oGlowObjects(glowProperty, colour);
	
	if (glowProperty && colour)
	{
		colour[0] = g_pGui->m_Vars.m_ESP.GlowColor.x;
		colour[1] = g_pGui->m_Vars.m_ESP.GlowColor.y;
		colour[2] = g_pGui->m_Vars.m_ESP.GlowColor.z;
		colour[3] = g_pGui->m_Vars.m_ESP.GlowColor.w;
	}
}

bool __fastcall CHooksManager::GlowObjects::HookIsGlowing(C_GlowProperty* glowProperty)
{
	if (!g_pInterfaces->m_Interfaces.pEngineClient->IsConnected() || !g_pInterfaces->m_Interfaces.pEngineClient->IsInGame())
		return false;

	if (!g_pGui->m_Vars.m_ESP.glow)
		return false;

	if (!glowProperty || !glowProperty->GetParentEntity())
		return false;

	auto parentEntity = glowProperty->GetParentEntity();
	auto localPlayer = Globals::LocalPlayerPawn;

	if (!parentEntity->GetBaseEntity()->IsEntityPlayer() || parentEntity == localPlayer)
		return false;

	if (!parentEntity->IsAlive())
		return false;

	if (Globals::LocalPlayerPawn->GetTeam() == parentEntity->GetTeam() && !g_pGui->m_Vars.m_ESP.team)
		return false;

	return true;
}

struct OriginalLightingValues {
    float red;
    float green;
    float blue;
    bool stored;
};

OriginalLightingValues originalValues = { 0, 0, 0, false };

void* __fastcall CHooksManager::LightingModulation::Hook(__int64 a1, CAggregateSceneObject* sceneObject, __int64 a3)
{
    if (!g_pGui->m_Vars.m_WorldModulation.lighting || !g_pInterfaces->m_Interfaces.pEngineClient->IsInGame()) {

        if (originalValues.stored) {
            sceneObject->red = originalValues.red;
            sceneObject->green = originalValues.green;
            sceneObject->blue = originalValues.blue;
            originalValues.stored = false; 
        }
        return oLightingModulation(a1, sceneObject, a3);
    }

    if (!originalValues.stored) {
        originalValues.red = sceneObject->red;
        originalValues.green = sceneObject->green;
        originalValues.blue = sceneObject->blue;
        originalValues.stored = true;
    }

    sceneObject->red = g_pGui->m_Vars.m_WorldModulation.LightingColor.x * g_pGui->m_Vars.m_WorldModulation.LightingIntensity;
    sceneObject->green = g_pGui->m_Vars.m_WorldModulation.LightingColor.y * g_pGui->m_Vars.m_WorldModulation.LightingIntensity;
    sceneObject->blue = g_pGui->m_Vars.m_WorldModulation.LightingColor.z * g_pGui->m_Vars.m_WorldModulation.LightingIntensity;

    return oLightingModulation(a1, sceneObject, a3);
}

void* __fastcall CHooksManager::WorldModulation::Hook(CAggregateSceneObjectWorld* pAggregateSceneObject, void* a2)
{
	auto original = oModulateWorldColor(pAggregateSceneObject, a2);

	if (!g_pGui->m_Vars.m_WorldModulation.worldcolor || !g_pInterfaces->m_Interfaces.pEngineClient->IsInGame())
		return original;

	auto r = static_cast<unsigned char>(g_pGui->m_Vars.m_WorldModulation.WorldColor.x * 255.f);
	auto g = static_cast<unsigned char>(g_pGui->m_Vars.m_WorldModulation.WorldColor.y * 255.f);
	auto b = static_cast<unsigned char>(g_pGui->m_Vars.m_WorldModulation.WorldColor.z * 255.f);

	int count = pAggregateSceneObject->count;

	for (int i = 0; i < count; i++) {
		CAggregateSceneObjectDataWorld* pAggregateSceneObjectData = &pAggregateSceneObject->array[i];

		pAggregateSceneObjectData->r = r;
		pAggregateSceneObjectData->g = g;
		pAggregateSceneObjectData->b = b;
	}

	return original;
}

void __fastcall CHooksManager::BombCode::Hook(void* a1)
{
	oBombCode(a1);
}

void __fastcall CHooksManager::DrawObject::Hook(void* a1, void* a2, void* a3, int a4, void* a5, void* a6, void* a7, void* a8)
{
	oDrawObject(a1, a2, a3, a4, a5, a6, a7, a8);
}

void __fastcall CHooksManager::RenderStart::Hook(CViewSetup* pSetup)
{
	oRenderStart(pSetup);

	if (g_pRenderer->Initialized)
	{
		g_pRenderer->ClearDrawData();

		g_pFeatures->m_ESP.m_Players.Draw();

		g_pRenderer->SwapDrawData();
	}
}

void __fastcall CHooksManager::FrameStage::Hook(void* rcx, int stage)
{

	if (!Globals::LocalPlayerPawn || !Globals::LocalPlayerController)
		return;
	oFrameStage(rcx, stage);
}

bool __fastcall CHooksManager::ForceCrosshair::Hook(__int64* a1)
{
	if (!g_pGui->m_Vars.m_OtherVisuals.ForceCrosshair)
		return oForceCrosshair(a1);

	if (!g_pInterfaces->m_Interfaces.pEngineClient->IsConnected())
		return false;
	if (!Globals::LocalPlayerPawn || !Globals::LocalPlayerController)
		return false;

	if (!Globals::LocalPlayerPawn->IsAlive())
		return false;

	auto isScoped = Globals::LocalPlayerPawn->IsScoped();

	if(isScoped)
		return false;


	return true;
}

bool __fastcall CHooksManager::RemoveZoomOverlay::Hook(void* a1, void* a2)
{
	if (!g_pGui->m_Vars.m_Removals.nozoom)
		return oRemoveZoomOverlay(a1,a2);

	if (!g_pInterfaces->m_Interfaces.pEngineClient->IsConnected())
		return false;
	if (!Globals::LocalPlayerPawn || !Globals::LocalPlayerController)
		return false;

	if (!Globals::LocalPlayerPawn->IsAlive())
		return false;

	if (Globals::LocalPlayerPawn->IsScoped())
		return false;

	return oRemoveZoomOverlay(a1, a2);
}

bool __fastcall CHooksManager::NoSmokeHook::Hook(void* a1, void* a2, void* a3, void* a4, void* a5, void* a6)
{
	if (!g_pGui->m_Vars.m_Removals.nosmoke)
		return oNoSmoke(a1, a2, a3, a4, a5, a6);

	if (!g_pInterfaces->m_Interfaces.pEngineClient->IsConnected())
		return true;

	if (!Globals::LocalPlayerPawn || !Globals::LocalPlayerController)
		return true;

	if (Globals::LocalPlayerPawn->IsAlive())
		return false;


	return true;
}

bool __fastcall CHooksManager::RemoveLegs::Hook(void* a1, void* a2, void* a3, void* a4, void* a5)
{
	if (!g_pGui->m_Vars.m_Removals.nolegspreview)
		return oRemoveLegs(a1, a2, a3, a4, a5);

	if (!g_pInterfaces->m_Interfaces.pEngineClient->IsConnected())
		return true;

	if (!Globals::LocalPlayerPawn || !Globals::LocalPlayerController)
		return true;

	if (Globals::LocalPlayerPawn->IsAlive())
		return false;


	return true;
}

void __fastcall CHooksManager::OverrideView::Hook(void* a1, CViewSetup* a2)
{
	if(!g_pInterfaces->m_Interfaces.pEngineClient->IsConnected() || !Globals::LocalPlayerPawn || !Globals::LocalPlayerController)
		return oOverrideView(a1, a2);

	if (g_pGui->m_Vars.m_Removals.norecoil) {
		a2->m_angView = g_pInterfaces->m_Interfaces.pGameInput->GetViewAngles();
	}

	oOverrideView(a1, a2);
}

float __fastcall CHooksManager::FovObject::Hook(void* a1)
{
	if (!g_pInterfaces->m_Interfaces.pEngineClient->IsInGame() || !Globals::LocalPlayerPawn || !Globals::LocalPlayerController)
		return oFovObject(a1);


	if (g_pGui->m_Vars.m_View.fovchanger)
	{
		if (Globals::LocalPlayerPawn->IsAlive())
		{
			float fov = g_pGui->m_Vars.m_View.fov;

			bool IsPlayerScoped = Globals::LocalPlayerPawn->IsScoped();

			if (!IsPlayerScoped)
			{
				return fov;
			}
			else if (g_pGui->m_Vars.m_View.fovwhilescoped && IsPlayerScoped)
			{
				return fov;
			}
		}
	}

	return oFovObject(a1);
}

void* __fastcall CHooksManager::ViewModel::Hook(float* a1, float* offset, float* fov)
{

	auto original = oViewModel(a1, offset, fov);

	if (!g_pInterfaces->m_Interfaces.pEngineClient->IsInGame() || !Globals::LocalPlayerPawn || !Globals::LocalPlayerController)
		return original;

	if (g_pGui->m_Vars.m_View.viewmodelchanger)
	{
		*offset = g_pGui->m_Vars.m_View.viewmodelx;
		offset[1] = g_pGui->m_Vars.m_View.viewmodely;
		offset[2] = g_pGui->m_Vars.m_View.viewmodelz;
		*fov = g_pGui->m_Vars.m_View.viewmodelfov;
	}
	return original;
}

void __fastcall CHooksManager::CalcViewModelAngles::Hook(DWORD* a1, __int64 a2, Vector3D a3, Vector3D a4, char a5)
{
	if (!g_pInterfaces->m_Interfaces.pEngineClient->IsConnected() || !Globals::LocalPlayerPawn || !Globals::LocalPlayerController)
		return oViewModelCalc(a1, a2, a3, a4, a5);

	if (g_pGui->m_Vars.m_Removals.norecoil) {
		auto angle_viewmodel = g_pInterfaces->m_Interfaces.pGameInput->GetViewAngles();

		angle_viewmodel.Normalize();

		a4.x = angle_viewmodel.x;
		a4.y = angle_viewmodel.y;
	}

	oViewModelCalc(a1, a2, a3, a4, a5);;
}

void* __fastcall CHooksManager::NoFlashbangEffect::Hook(__int64 a1, __int64 a2, float* a3)
{
	if (!g_pInterfaces->m_Interfaces.pEngineClient->IsConnected() || !Globals::LocalPlayerPawn || !Globals::LocalPlayerController)
		return oNoFlashbangEffect(a1, a2, a3);

	if (g_pGui->m_Vars.m_Removals.noflash)
		return NULL;

	return oNoFlashbangEffect(a1, a2, a3);
}

void* __fastcall CHooksManager::IsRelativeMouseMode::Hook(void* a1, bool a2)
{
	const auto original = oIsRelativeMouseMode(a1, a2);

	//if (g_pGui->IsOpen)
	//	return oIsRelativeMouseMode(a1, false);

	return oIsRelativeMouseMode(a1,a2);
}
