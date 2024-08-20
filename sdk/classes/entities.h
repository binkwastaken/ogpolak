#pragma once
#include <cstdlib>
#include <cstdint>
#include <string>
#include <stdint.h>
#include "../math/mathematics.h"
#include "../structs/structs.h"

#include "../math/vectors.h"

class CGameSceneNode;
class CCollision;
class SchemaClassInfoData;
class C_BaseWeapon;
class C_BaseWeaponServices;
class C_PlayerItemServices;
class CCSPlayerController_InGameMoneyServices;
class C_PlayerPawn;

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

class CEntityInstance {
public:
	void GetSchemaClassInfo(SchemaClassInfoData** pReturn);
	bool IsEntityPlayer();
};

class C_BaseEntity : public CEntityInstance{
public:
	CGameSceneNode* GetGameSceneNode();
	CCollision* GetCollision();
	Vector3D GetBonePosition(int boneIndex);
	C_BaseHandle GetHandleEntity();
	Vector3D GetVecViewOffset();
	Vector3D GetEyePosition();
};

class C_PlayerPawn {
public:
	C_BaseEntity* GetBaseEntity();
	int GetHealth();
	int GetTeam();
	int GetFlags();
	bool IsAlive();
	bool IsDefusing();
	bool IsScoped();
	C_BaseHandle GetHandleController();

	C_BaseWeapon* GetCurrentActiveWeapon();

	C_BaseWeaponServices* GetWeaponServices();

	C_PlayerItemServices* GetItemServices();

	float& GetViewModelFOV();

};

class C_PlayerController {
public:
	C_BaseHandle GetHandlePawn();
	static C_PlayerController* GetLocalPlayerController();
	const char* GetPlayerName();
	bool IsBot();
	CCSPlayerController_InGameMoneyServices* GetMoneyServices();
	long long GetPlayerSteamID();
};