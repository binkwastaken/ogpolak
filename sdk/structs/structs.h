#pragma once
#include "../math/mathematics.h"
#include "../classes/entities.h"

#include "../math/vectors.h"

#include <d3d9.h>

#include <string>

class C_BaseHandle;
class C_BaseEntity;
class C_PlayerPawn;

struct color_t {
    int a, r, g, b;
    color_t() = default;
    color_t(const int r, const int g, const int b, const int a = 255) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    explicit color_t(const uint32_t color_t) {
        this->a = (color_t >> 24) & 0xff;
        this->r = (color_t >> 16) & 0xff;
        this->g = (color_t >> 8) & 0xff;
        this->b = (color_t & 0xff);
    }

    static color_t from_uint(const uint32_t uint) {
        return color_t(uint);
    }

    static D3DCOLOR from_color_t(const color_t col) {
        return D3DCOLOR_ARGB(col.a, col.r, col.g, col.b);
    }

    static color_t black(const int a = 255) { return { 0, 0, 0, a }; }
    static color_t white(const int a = 255) { return { 255, 255, 255, a }; }
    static color_t red(const int   a = 255) { return { 255, 0, 0, a }; }
    static color_t green(const int a = 255) { return { 0, 255, 0, a }; }
    static color_t blue(const int  a = 255) { return { 0, 0, 255, a }; }
};

enum EItemDefinitionIndexes : std::uint16_t
{
    WEAPON_NONE,
    WEAPON_DESERT_EAGLE,
    WEAPON_DUAL_BERETTAS,
    WEAPON_FIVE_SEVEN,
    WEAPON_GLOCK_18,
    WEAPON_AK_47 = 7,
    WEAPON_AUG,
    WEAPON_AWP,
    WEAPON_FAMAS,
    WEAPON_G3SG1,
    WEAPON_GALIL_AR = 13,
    WEAPON_M249,
    WEAPON_M4A4 = 16,
    WEAPON_MAC_10,
    WEAPON_P90 = 19,
    WEAPON_REPULSOR_DEVICE,
    WEAPON_MP5_SD = 23,
    WEAPON_UMP_45,
    WEAPON_XM1014,
    WEAPON_PP_BIZON,
    WEAPON_MAG_7,
    WEAPON_NEGEV,
    WEAPON_SAWED_OFF,
    WEAPON_TEC_9,
    WEAPON_ZEUS_X27,
    WEAPON_P2000,
    WEAPON_MP7,
    WEAPON_MP9,
    WEAPON_NOVA,
    WEAPON_P250,
    WEAPON_RIOT_SHIELD,
    WEAPON_SCAR_20,
    WEAPON_SG_553,
    WEAPON_SSG_08,
    WEAPON_KNIFE0,
    WEAPON_KNIFE1,
    WEAPON_FLASHBANG,
    WEAPON_HIGH_EXPLOSIVE_GRENADE,
    WEAPON_SMOKE_GRENADE,
    WEAPON_MOLOTOV,
    WEAPON_DECOY_GRENADE,
    WEAPON_INCENDIARY_GRENADE,
    WEAPON_C4_EXPLOSIVE,
    WEAPON_KEVLAR_VEST,
    WEAPON_KEVLAR_and_HELMET,
    WEAPON_HEAVY_ASSAULT_SUIT,
    WEAPON_NO_LOCALIZED_NAME0 = 54,
    WEAPON_DEFUSE_KIT,
    WEAPON_RESCUE_KIT,
    WEAPON_MEDI_SHOT,
    WEAPON_MUSIC_KIT,
    WEAPON_KNIFE2,
    WEAPON_M4A1_S,
    WEAPON_USP_S,
    WEAPON_TRADE_UP_CONTRACT,
    WEAPON_CZ75_AUTO,
    WEAPON_R8_REVOLVER,
    WEAPON_TACTICAL_AWARENESS_GRENADE = 68,
    WEAPON_BARE_HANDS,
    WEAPON_BREACH_CHARGE,
    WEAPON_TABLET = 72,
    WEAPON_KNIFE3 = 74,
    WEAPON_AXE,
    WEAPON_HAMMER,
    WEAPON_WRENCH = 78,
    WEAPON_SPECTRAL_SHIV = 80,
    WEAPON_FIRE_BOMB,
    WEAPON_DIVERSION_DEVICE,
    WEAPON_FRAG_GRENADE,
    WEAPON_SNOWBALL,
    WEAPON_BUMP_MINE,
    WEAPON_BAYONET = 500,
    WEAPON_CLASSIC_KNIFE = 503,
    WEAPON_FLIP_KNIFE = 505,
    WEAPON_GUT_KNIFE,
    WEAPON_KARAMBIT,
    WEAPON_M9_BAYONET,
    WEAPON_HUNTSMAN_KNIFE,
    WEAPON_FALCHION_KNIFE = 512,
    WEAPON_BOWIE_KNIFE = 514,
    WEAPON_BUTTERFLY_KNIFE,
    WEAPON_SHADOW_DAGGERS,
    WEAPON_PARACORD_KNIFE,
    WEAPON_SURVIVAL_KNIFE,
    WEAPON_URSUS_KNIFE,
    WEAPON_NAVAJA_KNIFE,
    WEAPON_NOMAD_KNIFE,
    WEAPON_STILETTO_KNIFE,
    WEAPON_TALON_KNIFE,
    WEAPON_SKELETON_KNIFE = 525,
};


class FNV1A {
public:
    static constexpr uint32_t Hash(const char* string) {
        uint32_t hash = FNV_OFFSET_BASIS;
        for (size_t i = 0; string[i]; ++i) {
            hash = FNV_PRIME * (hash ^ static_cast<uint8_t>(string[i]));
        }

        return hash;
    }

    static consteval uint32_t HashConst(const char* string) {
        uint32_t hash = FNV_OFFSET_BASIS;
        for (size_t i = 0; string[i]; ++i) {
            hash = FNV_PRIME * (hash ^ static_cast<uint8_t>(string[i]));
        }

        return hash;
    }

private:
    static constexpr uint32_t FNV_PRIME = 0x01000193, FNV_OFFSET_BASIS = 0x811c9dc5;
};

struct CBoneData
{
    Vector3D location;
    float scale;
    Vector4D rotation;
};

class CModelState {
public:
    CBoneData* BoneArray()
    {
        return *reinterpret_cast<CBoneData**>(std::uintptr_t(this) + 0x80);
    }
};

class CGameSceneNode {
public:
    Vector3D GetVecOrigin();

    CModelState& GetModelState() {
        auto modelStateAddress = reinterpret_cast<std::uintptr_t>(this) + 0x170;
        return *reinterpret_cast<CModelState*>(modelStateAddress);
    }
};

class CCollision {
public:
    Vector3D VecMaxs();
    Vector3D VecMins();
};

class CCSWeaponData {
public:
	int GetMaxClip();
};


class C_EconItemView
{
public:
	short& GetItemDefinitionIndex();
};

class C_AttributeContainer
{
public:
	C_EconItemView& GetItem();
};

class C_BaseWeapon {
public:
	C_AttributeContainer& GetManagerAttribute();

	int GetClip1();

	CCSWeaponData* GetWeaponDataInfo();

    bool IsReloading();

	std::string GetWeaponString(int index) {
		switch (index) {
		case WEAPON_NONE: return "None";
		case WEAPON_DESERT_EAGLE: return "Desert Eagle";
		case WEAPON_DUAL_BERETTAS: return "Dual Berettas";
		case WEAPON_FIVE_SEVEN: return "Five-SeveN";
		case WEAPON_GLOCK_18: return "Glock-18";
		case WEAPON_AK_47: return "AK-47";
		case WEAPON_AUG: return "AUG";
		case WEAPON_AWP: return "AWP";
		case WEAPON_FAMAS: return "FAMAS";
		case WEAPON_G3SG1: return "G3SG1";
		case WEAPON_GALIL_AR: return "Galil AR";
		case WEAPON_M249: return "M249";
		case WEAPON_M4A4: return "M4A4";
		case WEAPON_MAC_10: return "MAC-10";
		case WEAPON_P90: return "P90";
		case WEAPON_REPULSOR_DEVICE: return "Repulsor Device";
		case WEAPON_MP5_SD: return "MP5-SD";
		case WEAPON_UMP_45: return "UMP-45";
		case WEAPON_XM1014: return "XM1014";
		case WEAPON_PP_BIZON: return "PP-Bizon";
		case WEAPON_MAG_7: return "MAG-7";
		case WEAPON_NEGEV: return "Negev";
		case WEAPON_SAWED_OFF: return "Sawed-Off";
		case WEAPON_TEC_9: return "Tec-9";
		case WEAPON_ZEUS_X27: return "Zeus x27";
		case WEAPON_P2000: return "P2000";
		case WEAPON_MP7: return "MP7";
		case WEAPON_MP9: return "MP9";
		case WEAPON_NOVA: return "Nova";
		case WEAPON_P250: return "P250";
		case WEAPON_RIOT_SHIELD: return "Riot Shield";
		case WEAPON_SCAR_20: return "SCAR-20";
		case WEAPON_SG_553: return "SG 553";
		case WEAPON_SSG_08: return "SSG 08";
		case WEAPON_KNIFE0: return "Knife";
		case WEAPON_KNIFE1: return "Knife";
		case WEAPON_FLASHBANG: return "Flashbang";
		case WEAPON_HIGH_EXPLOSIVE_GRENADE: return "High Explosive Grenade";
		case WEAPON_SMOKE_GRENADE: return "Smoke Grenade";
		case WEAPON_MOLOTOV: return "Molotov";
		case WEAPON_DECOY_GRENADE: return "Decoy Grenade";
		case WEAPON_INCENDIARY_GRENADE: return "Incendiary Grenade";
		case WEAPON_C4_EXPLOSIVE: return "C4 Explosive";
		case WEAPON_KEVLAR_VEST: return "Kevlar Vest";
		case WEAPON_KEVLAR_and_HELMET: return "Kevlar and Helmet";
		case WEAPON_HEAVY_ASSAULT_SUIT: return "Heavy Assault Suit";
		case WEAPON_NO_LOCALIZED_NAME0: return "Unnamed Weapon";
		case WEAPON_DEFUSE_KIT: return "Defuse Kit";
		case WEAPON_RESCUE_KIT: return "Rescue Kit";
		case WEAPON_MEDI_SHOT: return "Medi-Shot";
		case WEAPON_MUSIC_KIT: return "Music Kit";
		case WEAPON_KNIFE2: return "Knife";
		case WEAPON_M4A1_S: return "M4A1-S";
		case WEAPON_USP_S: return "USP-S";
		case WEAPON_TRADE_UP_CONTRACT: return "Trade-Up Contract";
		case WEAPON_CZ75_AUTO: return "CZ75-Auto";
		case WEAPON_R8_REVOLVER: return "R8 Revolver";
		case WEAPON_TACTICAL_AWARENESS_GRENADE: return "Tactical Awareness Grenade";
		case WEAPON_BARE_HANDS: return "Bare Hands";
		case WEAPON_BREACH_CHARGE: return "Breach Charge";
		case WEAPON_TABLET: return "Tablet";
		case WEAPON_KNIFE3: return "Knife";
		case WEAPON_AXE: return "Axe";
		case WEAPON_HAMMER: return "Hammer";
		case WEAPON_WRENCH: return "Wrench";
		case WEAPON_SPECTRAL_SHIV: return "Spectral Shiv";
		case WEAPON_FIRE_BOMB: return "Fire Bomb";
		case WEAPON_DIVERSION_DEVICE: return "Diversion Device";
		case WEAPON_FRAG_GRENADE: return "Frag Grenade";
		case WEAPON_SNOWBALL: return "Snowball";
		case WEAPON_BUMP_MINE: return "Bump Mine";
		case WEAPON_BAYONET: return "Bayonet";
		case WEAPON_CLASSIC_KNIFE: return "Classic Knife";
		case WEAPON_FLIP_KNIFE: return "Flip Knife";
		case WEAPON_GUT_KNIFE: return "Gut Knife";
		case WEAPON_KARAMBIT: return "Karambit";
		case WEAPON_M9_BAYONET: return "M9 Bayonet";
		case WEAPON_HUNTSMAN_KNIFE: return "Huntsman Knife";
		case WEAPON_FALCHION_KNIFE: return "Falchion Knife";
		case WEAPON_BOWIE_KNIFE: return "Bowie Knife";
		case WEAPON_BUTTERFLY_KNIFE: return "Butterfly Knife";
		case WEAPON_SHADOW_DAGGERS: return "Shadow Daggers";
		case WEAPON_PARACORD_KNIFE: return "Paracord Knife";
		case WEAPON_SURVIVAL_KNIFE: return "Survival Knife";
		case WEAPON_URSUS_KNIFE: return "Ursus Knife";
		case WEAPON_NAVAJA_KNIFE: return "Navaja Knife";
		case WEAPON_NOMAD_KNIFE: return "Nomad Knife";
		case WEAPON_STILETTO_KNIFE: return "Stiletto Knife";
		case WEAPON_TALON_KNIFE: return "Talon Knife";
		case WEAPON_SKELETON_KNIFE: return "Skeleton Knife";
		default: return "Unknown";
		}
	}

};

class C_BaseWeaponServices {
public:
	C_BaseHandle GetWeaponHandle();
	int GetAmmo();
};

class C_PlayerItemServices {
public:
    bool HasHelmet();
};

class CCSPlayerController_InGameMoneyServices
{
public:
    int GetMoney();
};

class C_GlowProperty {
public:
   C_PlayerPawn* GetParentEntity() {
       return *(C_PlayerPawn**)(this + 0x18);
    }
};

class CAggregateSceneObjectDataWorld {
private:
    char pad_0000[0x38]; // 0x0
public:
    unsigned char r; // 0x38
    unsigned char g; // 0x39
    unsigned char b; // 0x3A
private:
    char pad_0038[0x9];
};

class CAggregateSceneObjectWorld {
private:
    char pad_0000[0x120];
public:
    int count; // 0x120
private:
    char pad_0120[0x4];
public:
    CAggregateSceneObjectDataWorld* array; // 0x128
};

class CAggregateSceneObject
{
public:
    unsigned char pad_0[0xE4];
    float red;
    float green;
    float blue;
};


class C_UserCmd
{
public:

};

