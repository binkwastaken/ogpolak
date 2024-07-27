#pragma once
#include "../classes/entities.h"

class IEntityListSystem {
public:
	C_BaseEntity* GetClientEntity(int index)
	{
		return (C_BaseEntity*)this->pGetEntityByIndex(index);
	}
	void* GetClientEntityFromHandle(C_BaseHandle hHandle)
	{
		if (!hHandle.IsValid())
			return nullptr;

		return this->pGetEntityByIndex(hHandle.GetEntryIndex());
	}
	int GetHighestEntityIndex()
	{
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x1510);
	}
private:
	void* pGetEntityByIndex(int Index);

};


class IEngineClient {
public:
	int GetMaxClients();
	bool IsInGame();
	bool IsConnected();
	int GetLocalPlayerIndex();
};