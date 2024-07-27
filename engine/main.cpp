#include "utils/utils.h"
#include "hooks/hooks.h"
#include "interfaces/manager.h"
unsigned long EntryPoint(void* entryPoint)
{
	g_pUtils->m_Console.Init();
	g_pInterfaces->Init();
	g_pHooksManager->Init();

	while (!GetAsyncKeyState(VK_DELETE) & 1)
		Sleep(250);

	g_pHooksManager->Destroy();
	g_pUtils->m_Console.Destroy();

	FreeLibraryAndExitThread((HMODULE)entryPoint, 0);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserve)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		auto handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EntryPoint, hModule, 0, NULL);
		if (handle)
			CloseHandle(handle);
	}
	return TRUE;
}