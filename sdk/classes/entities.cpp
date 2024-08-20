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

CGameSceneNode* C_BaseEntity::GetGameSceneNode()
{
	return g_pSchemaManager->GetOffset<CGameSceneNode*>("C_BaseEntity", "m_pGameSceneNode", this);
}

CCollision* C_BaseEntity::GetCollision()
{
	return g_pSchemaManager->GetOffset<CCollision*>("C_BaseEntity", "m_pCollision", this);
}

Vector3D C_BaseEntity::GetBonePosition(int boneIndex)
{
		auto& ModelState = this->GetGameSceneNode()->GetModelState();
		auto bonearray = ModelState.BoneArray();
		if (!bonearray)
			return Vector3D{};

		auto pos = (bonearray + boneIndex)->location;
		if (pos.IsZero())
			return Vector3D{};

		return pos;
}

const char* C_PlayerController::GetPlayerName()
{
	return g_pSchemaManager->GetOffset<const char*>("CCSPlayerController", "m_sSanitizedPlayerName", this);
}

bool C_PlayerController::IsBot()
{
	if (GetPlayerSteamID() > 0)
		return false;
	else
		return true;
}

CCSPlayerController_InGameMoneyServices* C_PlayerController::GetMoneyServices()
{
return g_pSchemaManager->GetOffset<CCSPlayerController_InGameMoneyServices*>("CCSPlayerController", "m_pInGameMoneyServices", this);
}

C_BaseEntity* C_PlayerPawn::GetBaseEntity()
{
	return (C_BaseEntity*)(this);
}

int C_PlayerPawn::GetHealth()
{
	return g_pSchemaManager->GetOffset<int32_t>("C_BaseEntity", "m_iHealth", this);
}


int C_PlayerPawn::GetTeam()
{
	return g_pSchemaManager->GetOffset<int32_t>("C_BaseEntity", "m_iTeamNum", this);
}

int C_PlayerPawn::GetFlags()
{
	return g_pSchemaManager->GetOffset<int32_t>("C_BaseEntity", "m_fFlags", this);
}

bool C_PlayerPawn::IsAlive()
{
	if (this->GetHealth() <= 0)
	{
		return false;
	}

	return true;
}

bool C_PlayerPawn::IsDefusing()
{
	return g_pSchemaManager->GetOffset<bool>("C_CSPlayerPawn", "m_bIsDefusing", this);
}

bool C_PlayerPawn::IsScoped()
{
	return g_pSchemaManager->GetOffset<bool>("C_CSPlayerPawn", "m_bIsScoped", this);
}

C_BaseHandle C_PlayerPawn::GetHandleController()
{
	return g_pSchemaManager->GetOffset<C_BaseHandle>("C_BasePlayerPawn", "m_hController", this);
}

void CEntityInstance::GetSchemaClassInfo(SchemaClassInfoData** pReturn)
{
	return g_pUtils->m_VMT.CallVMT<void, 38>(this, pReturn);
}

bool CEntityInstance::IsEntityPlayer()
{
	SchemaClassInfoData* cigan;

	this->GetSchemaClassInfo(&cigan);

	auto hash = FNV1A::Hash(cigan->name);

	if (hash != FNV1A::Hash("C_CSPlayerPawn"))
		return false;
	else
		return true;

}

C_BaseWeapon* C_PlayerPawn::GetCurrentActiveWeapon()
{
	return   g_pSchemaManager->GetOffset<C_BaseWeapon*>("C_CSPlayerPawnBase", "m_pClippingWeapon", this);
}

C_BaseWeaponServices* C_PlayerPawn::GetWeaponServices()
{
	return g_pSchemaManager->GetOffset<C_BaseWeaponServices*>("C_BasePlayerPawn", "m_pWeaponServices", this);
}

C_PlayerItemServices* C_PlayerPawn::GetItemServices()
{
	return g_pSchemaManager->GetOffset<C_PlayerItemServices*>("C_BasePlayerPawn", "m_pItemServices", this);
}

float& C_PlayerPawn::GetViewModelFOV()
{
	return g_pSchemaManager->GetOffset<float>("C_CSPlayerPawn", "m_flViewModelFOV", this);
}

long long C_PlayerController::GetPlayerSteamID()
{
	return *(long long*)(this + 0x6C8);
}

C_BaseHandle C_BaseEntity::GetHandleEntity()
{
	return g_pSchemaManager->GetOffset<C_BaseHandle>("C_BaseEntity", "m_hOwnerEntity", this);
}

Vector3D C_BaseEntity::GetVecViewOffset()
{
	return g_pSchemaManager->GetOffset<Vector3D>("C_BaseModelEntity", "m_vecViewOffset", this);
}

Vector3D C_BaseEntity::GetEyePosition()
{
	return this->GetVecViewOffset() + this->GetGameSceneNode()->GetVecOrigin();
}