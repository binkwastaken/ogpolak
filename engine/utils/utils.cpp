#include "utils.h"


void CUtils::Console::Init()
{
	AllocConsole();
	SetConsoleTitle("cmd");
	freopen_s(&m_Console, "CONOUT$", "w", stdout);
	Log("Console initialized");
}

void CUtils::Console::Destroy()
{
	if (m_Console)
	{
		fclose(m_Console);
		FreeConsole();
	}
}

std::uint8_t* CUtils::Memory::ResolveRip(std::uint8_t* address, std::uint32_t rva_offset, std::uint32_t rip_offset)
{
	if (!address || !rva_offset || !rip_offset)
	{
		return nullptr;
	}

	std::uint32_t rva = *reinterpret_cast<std::uint32_t*>(address + rva_offset);
	std::uint64_t rip = reinterpret_cast<std::uint64_t>(address) + rip_offset;

	return reinterpret_cast<std::uint8_t*>(rva + rip);
}

std::vector<std::uint32_t> CUtils::Memory::PatternToByte(const char* pattern)
{
	std::vector<std::uint32_t> bytes;
	char* start = const_cast<char*>(pattern);
	char* end = const_cast<char*>(pattern) + std::strlen(pattern);

	for (char* current = start; current < end; current++)
	{
		if (*current == '?')
		{
			current++;

			if (*current == '?')
			{
				current++;
			}

			bytes.push_back(-1);
		}
		else
		{
			bytes.push_back(std::strtoul(current, &current, 16));
		}
	}

	return bytes;
}

std::uint8_t* CUtils::Memory::PatternScan(const char* module_name, const char* signature)
{
	HMODULE module_handle = GetModuleHandle(module_name);

	if (!module_handle)
	{
		return nullptr;
	}

	PIMAGE_DOS_HEADER dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
	PIMAGE_NT_HEADERS nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

	std::size_t size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	std::vector<std::uint32_t> pattern_bytes = PatternToByte(signature);
	std::uint8_t* image_base = reinterpret_cast<std::uint8_t*>(module_handle);

	std::size_t pattern_size = pattern_bytes.size();
	std::uint32_t* array_of_bytes = pattern_bytes.data();

	for (std::size_t i = 0; i < size_of_image - pattern_size; i++)
	{
		bool found = true;

		for (std::size_t j = 0; j < pattern_size; j++)
		{
			if (image_base[i + j] != array_of_bytes[j] && array_of_bytes[j] != -1)
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			return &image_base[i];
		}
	}

	return nullptr;
}

bool CUtils::Memory::Init()
{
	HMODULE hSDL3 = GetModuleHandle("SDL3.dll");
	if (!hSDL3)
	{
	g_pUtils->m_Console.Error("Failed to get SDL3.dll");
		return false;
	}
	HMODULE hTier0 = GetModuleHandle("tier0.dll");
	if (!hTier0)
	{
		g_pUtils->m_Console.Error("Failed to get tier0.dll");
		return false;
	}
	fnSetRelativeMouseMode = reinterpret_cast<decltype(fnSetRelativeMouseMode)>(GetProcAddress(hSDL3, "SDL_SetRelativeMouseMode"));
	if (!fnSetRelativeMouseMode)
	{
		g_pUtils->m_Console.Error("Failed to get SDL_SetRelativeMouseMode from SDL3.dll");
		return false;
	}
	fnCreateMaterial = reinterpret_cast<decltype(fnCreateMaterial)>(this->PatternScan("materialsystem2.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 48 8B 05"));
	if (!fnCreateMaterial)
	{
		g_pUtils->m_Console.Error("Failed to get CreateMaterial from materialsystem2.dll");
		return false;
	}
	g_pUtils->m_Console.Log("Memory Export initialized");

	return true;
}