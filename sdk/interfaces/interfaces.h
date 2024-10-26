#pragma once
#include "../classes/entities.h"

#include "../../engine/schema/info.h"

struct EntityListInfo {
	C_PlayerController* Controller;
	C_PlayerPawn* Pawn;
	C_BaseEntity* Entity;
	int index;
	bool IsCheater = false;
};

class IClientSource2
{
public:

};

class ISchemaSystem
{
public:
	SchemaSystemTypeScope* GlobalTypeScope();
	SchemaSystemTypeScope* FindTypeScopeForModule(const char* module);
};

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
		return *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(this) + 0x1520);
	}

	void UpdateEntities(std::vector<EntityListInfo>& entities);

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

class ISystemInput {
public:
	bool IsRelativeMouseMode()
	{
		// 0x53 = IInputSystem::IsRelativeMouseMode
		return *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x53);
	}

	void* GetSDLWindow()
	{
		// @ida: IInputSystem::DebugSpew -> #STR: "Current coordinate bias %s: %g,%g scale %g,%g\n"
		return *reinterpret_cast<void**>(reinterpret_cast<unsigned long long>(this) + 0x26A8);
	}
};