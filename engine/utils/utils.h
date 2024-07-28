#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>

class CUtils {
public:
    class Console {
    public:
        FILE* m_Console;

        void Init();
        void Destroy();

        template <typename ...Args>
        void Print(const char* prefix, WORD color, Args ...args) {
            if (m_Console) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
                std::cout << "[" << prefix << "]: ";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                ((std::cout << args << " "), ...);
                std::cout << std::endl;
            }
        }


        template <typename ...Args>
        void Log(Args ...args) {
            Print("LOG", 3, args...);
        }

        template <typename ...Args>
        void Error(Args ...args) {
            Print("!", 12, args...);
        }

        template <typename ...Args>
        void Success(Args ...args) {
            Print("*", 10, args...);
        }
    };

    class Memory {
    public:
        bool Init();
        int(__stdcall* fnSetRelativeMouseMode)(int) = nullptr;
        int(__stdcall* fnSetWindowGrab)(void*, int) = nullptr;
        int(__stdcall* fnWarpMouseInWindow)(void*, float, float) = nullptr;
        bool(__stdcall* fnGetRelativeMouseMode)(void) = nullptr;
        int(__fastcall* fnCreateMaterial)(void*, void*, const char*, void*, unsigned int, unsigned int);
        std::uint8_t* ResolveRip(std::uint8_t* address, std::uint32_t rva_offset, std::uint32_t rip_offset);
        std::vector<std::uint32_t> PatternToByte(const char* pattern);
        std::uint8_t* PatternScan(const char* module_name, const char* signature);
    };
    
    class VMT {
    public:
        template <typename T, std::size_t nIndex, class CBaseClass, typename... Args_t>
        T CallVMT(CBaseClass* thisptr, Args_t... argList)
        {
            using VirtualFn_t = T(__thiscall*)(const void*, decltype(argList)...);
            return (*reinterpret_cast<VirtualFn_t* const*>(reinterpret_cast<std::uintptr_t>(thisptr)))[nIndex](thisptr, argList...);
        }

        void* GetVMT(void* pointer, std::uint32_t index)
        {
            void** vtable = *static_cast<void***>(pointer);
            return vtable[index];
        }
    };


    VMT m_VMT;
    Memory m_Memory;
	Console m_Console;
};

inline CUtils* g_pUtils = new CUtils();
