#pragma once
#include "../utils/utils.h"

#include "../../sdk/interfaces/interfaces.h"

class CInterfacesManager {
public:
	bool Init();

	class CInterfaces {
	public:
		IEntityListSystem* pEntityList;
        IEngineClient* pEngineClient;
        ISystemInput* pSystemInput;
        ISchemaSystem* pSchemaSystem;
        IClientSource2* pClientSource2;
        CGameInput* pGameInput;
	};

	CInterfaces m_Interfaces;
private:
    void* GetInterface(const char* mod, const char* iface, void*& gIface)
    {
        using fn_capture_iface_t = void* (*)(const char*, int*);
        auto fn_addr = (fn_capture_iface_t)GetProcAddress(GetModuleHandleA(mod), "CreateInterface");
        if (!fn_addr)
        {
            g_pUtils->m_Console.Error("Failed to get CreateInterface from", mod);
            return nullptr;
        }
        void* iface_addr = fn_addr(iface, nullptr);

        if (iface_addr == nullptr)
        {
            g_pUtils->m_Console.Error("Interface", iface, "not found!");
        }
        else
            g_pUtils->m_Console.Success("Interface", iface, "found at", iface_addr);

        gIface = iface_addr;

        return gIface;
    }
    template <typename T>
    T* FindInterface(const char* moduleName, const char* pattern, const char* interfaceName, bool useRip = false, int offset = 0, int extra = 0) {
        uint8_t* address = g_pUtils->m_Memory.PatternScan(moduleName, pattern);
        if (!address) {
            g_pUtils->m_Console.Error("Failed to find address for", interfaceName);
            return nullptr;
        }

        T* resolvedAddress = nullptr;
        if (useRip) {
            resolvedAddress = *reinterpret_cast<T**>(g_pUtils->m_Memory.ResolveRip(address, offset, extra));
        }
        else {
            resolvedAddress = *reinterpret_cast<T**>(address + offset);
        }

        if (!resolvedAddress) {
            g_pUtils->m_Console.Error("Failed to resolve address for", interfaceName);
        }
        else {
            g_pUtils->m_Console.Success("Found", interfaceName,"at",resolvedAddress);
        }

        return resolvedAddress;
    }
};
inline CInterfacesManager* g_pInterfaces = new CInterfacesManager();