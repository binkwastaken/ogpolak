#include "features.h"

void CFeatures::ESP::Players()
{
	if (!g_pGui->m_Vars.m_ESP.players || !g_pInterfaces->m_Interfaces.pEngineClient->IsInGame())
		return;
	std::vector<EntityListInfo> entities;
	g_pInterfaces->m_Interfaces.pEntityList->UpdateEntities(entities);

	for (auto& entity : entities)
	{
		if (entity.Controller == nullptr)
			continue;
		if (entity.Pawn == nullptr)
			continue;
		if (entity.Entity == nullptr)
			continue;
	}

}
