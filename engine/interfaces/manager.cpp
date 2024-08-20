#include "manager.h"


bool CInterfacesManager::Init()
{
	(IEngineClient*)GetInterface("engine2.dll", "Source2EngineToClient001", reinterpret_cast<void*&>(m_Interfaces.pEngineClient));
	(ISystemInput*)GetInterface("inputsystem.dll", "InputSystemVersion001", reinterpret_cast<void*&>(m_Interfaces.pSystemInput));
	(ISchemaSystem*)GetInterface("schemasystem.dll", "SchemaSystem_001", reinterpret_cast<void*&>(m_Interfaces.pSchemaSystem));
	(IClientSource2*)GetInterface("client.dll","Source2Client002", reinterpret_cast<void*&>(m_Interfaces.pClientSource2));

	m_Interfaces.pEntityList = FindInterface<IEntityListSystem>("client.dll", "48 8B 0D ? ? ? ? 48 8D 94 24 ? ? ? ? 33 DB", "EntityListSystem", true, 3, 7);
	m_Interfaces.pGameInput = FindInterface<CGameInput>("client.dll", "48 8B 0D ? ? ? ? 48 8B 01 FF 50 ? 8B DF", "IGameInput", true, 3, 7);


	return true;
}