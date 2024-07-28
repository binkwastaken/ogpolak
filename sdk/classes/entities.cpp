#include "entities.h"
#include "../../engine/utils/utils.h"

#include "../../engine/schema/schema.h"

C_BaseHandle C_PlayerController::GetHandlePawn()
{
	return g_pSchemaManager->GetOffset<C_BaseHandle>("CBasePlayerController", "m_hPawn", this);
}

C_PlayerController* C_PlayerController::GetLocalPlayerController()
{
	const int nIndex = g_pInterfaces->m_Interfaces.pEngineClient->GetLocalPlayerIndex();
	return (C_PlayerController*)g_pInterfaces->m_Interfaces.pEntityList->GetClientEntity(nIndex);
}
