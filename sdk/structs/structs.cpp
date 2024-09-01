#include "structs.h"
#include "../../engine/utils/utils.h"
#include "../../engine/schema/schema.h"

const Vector3D CGameSceneNode::GetVecOrigin()
{
	return g_pSchemaManager->GetOffset<const Vector3D>("CGameSceneNode", "m_vecOrigin", this);
}

void CGameSceneNode::CalcBones(unsigned int bone)
{
	using fnGetCalcBones = void* (__thiscall*)(void*, int);
	static auto CalcBones = reinterpret_cast<fnGetCalcBones>(g_pUtils->m_Memory.PatternScan("client.dll", "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC D0"));
	CalcBones(this, bone);
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

const char* CModelState::GetModelName()
{
return g_pSchemaManager->GetOffset<const char*>("CModelState", "m_ModelName", this);
}

Vector3D CGameInput::GetViewAngles()
{
	using fnGetViewAngles = std::int64_t(__fastcall*)(void*, std::int32_t);
	static auto oGetViewAngles = reinterpret_cast<fnGetViewAngles>(g_pUtils->m_Memory.PatternScan("client.dll", "4C 8B C1 85 D2 74 08 48 8D 05 ? ? ? ? C3"));
	return *reinterpret_cast<Vector3D*>(oGetViewAngles(this, 0));

}

void CGameInput::SetViewAngles(Vector3D& newAngle)
{
	using fnSetViewAngle = std::int64_t(__fastcall*)(void*, std::int32_t, Vector3D&);
	static auto oSetViewAngle = reinterpret_cast<fnSetViewAngle>(g_pUtils->m_Memory.PatternScan("client.dll", "85 D2 75 3F 48"));
	oSetViewAngle(this, 0, std::ref(newAngle));
}
