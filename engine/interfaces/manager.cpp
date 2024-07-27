#include "manager.h"


bool CInterfacesManager::Init()
{
	m_Interfaces.pEntityList = FindInterface<IEntityListSystem>("client.dll", "48 8B 0D ? ? ? ? 48 8D 94 24 ? ? ? ? 33 DB", "EntityListSystem", true, 3, 7);

	return true;
}