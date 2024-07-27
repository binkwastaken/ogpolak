#pragma once
#include <cstdlib>
#include <cstdint>

#include "../math/math.h"	

class C_BaseHandle {
public:
	C_BaseHandle() noexcept :
		nIndex(0xFFFFFFFF) { }

	C_BaseHandle(const int nEntry, const int nSerial) noexcept
	{
		static_cast<void>(!!(nEntry >= 0 && (nEntry & 0x7FFF) == nEntry) || (__debugbreak(), 0));
		static_cast<void>(!!(nSerial >= 0 && nSerial < (1 << 15)) || (__debugbreak(), 0));

		nIndex = nEntry | (nSerial << 15);
	}
	bool IsValid() const noexcept
	{
		return nIndex != 0xFFFFFFFF;
	}

	int GetEntryIndex() const noexcept
	{
		return static_cast<int>(nIndex & 0x7FFF);
	}
private:
	uint32_t nIndex;
};



class C_BaseEntity {
public:

};

class C_PlayerPawn {
public:

};

class C_PlayerController {
public:

};