#include "structs.h"
#include "../../engine/utils/utils.h"
#include "../../engine/schema/schema.h"



Vector3D CGameSceneNode::GetVecOrigin()
{
	return g_pSchemaManager->GetOffset<Vector3D>("CGameSceneNode", "m_vecAbsOrigin", this);
}

Vector3D CCollision::VecMaxs()
{
	return g_pSchemaManager->GetOffset<Vector3D>("CCollisionProperty", "m_vecMaxs", this);
}

Vector3D CCollision::VecMins()
{
	return g_pSchemaManager->GetOffset<Vector3D>("CCollisionProperty", "m_vecMins", this);
}

C_BaseHandle C_BaseWeaponServices::GetWeaponHandle()
{
	return  g_pSchemaManager->GetOffset<C_BaseHandle>("CPlayer_WeaponServices", "m_hActiveWeapon", this);
}

int C_BaseWeaponServices::GetAmmo()
{
	return  g_pSchemaManager->GetOffset<int>("CPlayer_WeaponServices", "m_iAmmo", this);
}

C_AttributeContainer& C_BaseWeapon::GetManagerAttribute()
{
	return g_pSchemaManager->GetOffset<C_AttributeContainer>("C_EconEntity", "m_AttributeManager", this);
}

int C_BaseWeapon::GetClip1()
{
	return g_pSchemaManager->GetOffset<int>("C_BasePlayerWeapon", "m_iClip1", this);
}

CCSWeaponData* C_BaseWeapon::GetWeaponDataInfo()
{
	auto weapondataddress = reinterpret_cast<std::uintptr_t>(this) + 0x360;
	return *reinterpret_cast<CCSWeaponData**>(weapondataddress);
}

bool C_BaseWeapon::IsReloading()
{
	return g_pSchemaManager->GetOffset<bool>("C_CSWeaponBase", "m_bInReload", this);
}

C_EconItemView& C_AttributeContainer::GetItem()
{
	return g_pSchemaManager->GetOffset<C_EconItemView>("C_AttributeContainer", "m_Item", this);
}

short& C_EconItemView::GetItemDefinitionIndex()
{
	return g_pSchemaManager->GetOffset<short>("C_EconItemView", "m_iItemDefinitionIndex", this);
}


int CCSWeaponData::GetMaxClip()
{
	return g_pSchemaManager->GetOffset<int>("CBasePlayerWeaponVData", "m_iMaxClip1", this);
}

bool C_PlayerItemServices::HasHelmet()
{
	return g_pSchemaManager->GetOffset<bool>("CCSPlayer_ItemServices", "m_bHasHelmet", this);
}

int CCSPlayerController_InGameMoneyServices::GetMoney()
{
	return g_pSchemaManager->GetOffset<int>("CCSPlayerController_InGameMoneyServices", "m_iAccount", this);
}
