#include "features.h"
#include "../utils/utils.h"

void CFeatures::ESP::Players::Draw()
{
    if (!g_pGui->m_Vars.m_ESP.players || !g_pInterfaces->m_Interfaces.pEngineClient->IsInGame())
        return;
    std::vector<EntityListInfo> entities;
    g_pInterfaces->m_Interfaces.pEntityList->UpdateEntities(entities);
    if (entities.empty())
        return;

    CMath::BoundingBox bbox;

    for (auto& entity : entities)
    {
        bool IsPlayer = entity.Pawn->GetBaseEntity()->IsEntityPlayer();
        bool IsAlive = entity.Pawn->IsAlive();

        if ((entity.Pawn != Globals::LocalPlayerPawn && Globals::LocalPlayerPawn->GetTeam() == entity.Pawn->GetTeam() && !g_pGui->m_Vars.m_ESP.team) || (entity.Pawn == Globals::LocalPlayerPawn && !g_pGui->m_Vars.m_ESP.localplayer))
            continue;

        if (IsPlayer && IsAlive)
        {
            if (g_pMath->GetPlayerBoundingBox(entity.Pawn, bbox))
            {
                Box(bbox);
                Name(bbox,entity);
				HealthBar(bbox, entity);
                Flags(entity, bbox);
                Ammo(bbox, entity);
                Weapon(bbox, entity);
                Distance(entity, bbox);
            }
        }

    }
}

void CFeatures::ESP::Players::Box(CMath::BoundingBox bbox)
{
    if (!g_pGui->m_Vars.m_ESP.boxes)
        return;
    color_t color = color_t(g_pGui->m_Vars.m_ESP.BoxesColor.x * 255, g_pGui->m_Vars.m_ESP.BoxesColor.y * 255, g_pGui->m_Vars.m_ESP.BoxesColor.z * 255, g_pGui->m_Vars.m_ESP.BoxesColor.w * 255);
    g_pRenderer->DrawRect(bbox.x, bbox.y, bbox.w, bbox.h,color);
    g_pRenderer->DrawRect(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, color_t::black(100));
    g_pRenderer->DrawRect(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, color_t::black(100));
}


void CFeatures::ESP::Players::Name(CMath::BoundingBox& pos, const EntityListInfo& player)
{
    if (!g_pGui->m_Vars.m_ESP.name)
        return;
    Vector2D position(pos.x + pos.w / 2, pos.y - 8);

    std::string nameLower = player.Controller->GetPlayerName();
	std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

    color_t color = color_t(g_pGui->m_Vars.m_ESP.NameColor.x * 255, g_pGui->m_Vars.m_ESP.NameColor.y * 255, g_pGui->m_Vars.m_ESP.NameColor.z * 255, g_pGui->m_Vars.m_ESP.NameColor.w * 255);
    ImGui::PushFont(g_pRenderer->m_Fonts.Verdana);
    g_pRenderer->DrawOutlinedString(nameLower.c_str(), position, color, color_t(0, 0, 0, 100), true);
    ImGui::PopFont();
}


void CFeatures::ESP::Players::HealthBar(CMath::BoundingBox& bbox, const EntityListInfo& player)
{
    if (!g_pGui->m_Vars.m_ESP.health)
        return;

    int playerHealth = player.Pawn->GetHealth();
    if (playerHealth > 100 || playerHealth <= 0)
        return;

    float healthRatio = static_cast<float>(playerHealth) / 100.0f;

    int playerId = player.index;

    if (previousHealth.find(playerId) == previousHealth.end()) {
        previousHealth[playerId] = healthRatio;
    }

    float& prevHealthRatio = previousHealth[playerId];
    const float lerpSpeed = 0.1f; 
    prevHealthRatio += (healthRatio - prevHealthRatio) * lerpSpeed;

    float barHeight = bbox.h * prevHealthRatio;
    barHeight = max(0.0f, min(bbox.h, barHeight));

    float barY = bbox.y + (bbox.h - barHeight);
    float barX = bbox.x - 5.0f;
    float barWidth = 1.0f;

    ImVec4 healthColor;
    if (playerHealth > 100)
        healthColor = ImVec4(0, 1, 0, 1);
    else
        healthColor = ImVec4((255 - playerHealth * 2.55f) / 255.0f, (playerHealth * 2.55f) / 255.0f, 0, 1);

    g_pRenderer->DrawFilledRect(barX - 1, bbox.y - 1, barWidth + 2, bbox.h + 2, color_t(0, 0, 0, 100));
    g_pRenderer->DrawFilledRect(barX, bbox.y, barWidth, bbox.h, color_t(46,46, 46, 255));

    g_pRenderer->DrawFilledRect(barX, barY, barWidth, std::ceil(barHeight), color_t(static_cast<int>(healthColor.x * 255), static_cast<int>(healthColor.y * 255), static_cast<int>(healthColor.z * 255), 255));

    if (playerHealth < 100) {
        ImGui::PushFont(g_pRenderer->m_Fonts.Pixel);
        g_pRenderer->DrawOutlinedString(std::to_string(playerHealth).c_str(), Vector2D(barX - 1, barY), color_t::white(), color_t(0, 0, 0, 100), true);
        ImGui::PopFont();
    }
}

void CFeatures::ESP::Players::Weapon(CMath::BoundingBox& boundingBox, const EntityListInfo& playerInfo)
{
    if (!g_pGui->m_Vars.m_ESP.weapon)
        return;

    C_BaseWeapon* Weapon = reinterpret_cast<C_BaseWeapon*>(g_pInterfaces->m_Interfaces.pEntityList->GetClientEntityFromHandle(playerInfo.Pawn->GetWeaponServices()->GetWeaponHandle()));
    if (!Weapon)
        return;

    int Index = Weapon->GetManagerAttribute().GetItem().GetItemDefinitionIndex();
    std::string WeaponName = Weapon->GetWeaponString(Index);


    ImVec2 textSize = ImGui::CalcTextSize(WeaponName.c_str());

    ImVec2 textPos(boundingBox.x + boundingBox.w / 2, boundingBox.y + boundingBox.h + 6);

    color_t color = color_t(g_pGui->m_Vars.m_ESP.WeaponColor.x * 255, g_pGui->m_Vars.m_ESP.WeaponColor.y * 255, g_pGui->m_Vars.m_ESP.WeaponColor.z * 255, g_pGui->m_Vars.m_ESP.WeaponColor.w * 255);

    ImGui::PushFont(g_pRenderer->m_Fonts.Pixel);
    g_pRenderer->DrawOutlinedString(WeaponName.c_str(), Vector2D(textPos.x, textPos.y), color, color_t(0, 0, 0, 100), true);
    ImGui::PopFont();
    boundingBox.y += ImGui::GetFontSize() - 2;
}

void CFeatures::ESP::Players::Ammo(CMath::BoundingBox& bbox, const EntityListInfo& pPawn) {
    if (!g_pGui->m_Vars.m_ESP.ammo)
        return;

    C_BaseWeapon* Weapon = reinterpret_cast<C_BaseWeapon*>(g_pInterfaces->m_Interfaces.pEntityList->GetClientEntityFromHandle(pPawn.Pawn->GetWeaponServices()->GetWeaponHandle()));
    if (!Weapon)
        return;

    int index = Weapon->GetManagerAttribute().GetItem().GetItemDefinitionIndex();
    if (!index)
        return;

    int clip = Weapon->GetClip1();

    CCSWeaponData* WeaponData = Weapon->GetWeaponDataInfo();
    if (!WeaponData)
        return;

    int maxClip = WeaponData->GetMaxClip();
    if (maxClip < 0)
        return;

    float y = bbox.y;

    CMath::BoundingBox temp_bg(bbox.x, y + bbox.h + 3, bbox.w, 1.0f);

    float scale = (maxClip == 0) ? 1.0f : static_cast<float>(clip) / maxClip;

    int barWidth = static_cast<int>(std::round(bbox.w * scale));

    color_t ammoBarColour = color_t(g_pGui->m_Vars.m_ESP.AmmoColor.x * 255, g_pGui->m_Vars.m_ESP.AmmoColor.y * 255, g_pGui->m_Vars.m_ESP.AmmoColor.z * 255, g_pGui->m_Vars.m_ESP.AmmoColor.w * 255);

    g_pRenderer->DrawFilledRect(temp_bg.x - 1, temp_bg.y - 1, temp_bg.w + 2, temp_bg.h + 2, color_t(0, 0, 0, 100));
    g_pRenderer->DrawFilledRect(temp_bg.x, temp_bg.y, temp_bg.w, temp_bg.h, color_t(75, 75, 75, 255));

    g_pRenderer->DrawFilledRect(temp_bg.x, temp_bg.y, barWidth, temp_bg.h, ammoBarColour);

    bbox.y += ImGui::GetFontSize() - 8;

    if (clip > 0 && clip != maxClip) {
        ImGui::PushFont(g_pRenderer->m_Fonts.Pixel);
        ImVec2 textPos = ImVec2(temp_bg.x + barWidth, temp_bg.y);
        g_pRenderer->DrawOutlinedString(std::to_string(clip).c_str(), Vector2D(textPos.x, textPos.y), color_t::white(), color_t::black(), true);
        ImGui::PopFont();
    }
}

void CFeatures::ESP::Players::Distance(const EntityListInfo& playerInfo, CMath::BoundingBox& pos)
{
    if (!g_pGui->m_Vars.m_ESP.distance)
        return;

   Vector3D EntityPosition = playerInfo.Pawn->GetBaseEntity()->GetGameSceneNode()->GetVecOrigin();
   Vector3D LocalPlayerPosition = Globals::LocalPlayerPawn->GetBaseEntity()->GetGameSceneNode()->GetVecOrigin();

    int p_Distance = LocalPlayerPosition.Distance(EntityPosition);

    p_Distance *= 0.0254f;

    std::string DistanceText = std::to_string(p_Distance) + "M";

    ImVec2 textPos(pos.x + pos.w / 2, pos.y + pos.h + 5);
    ImGui::PushFont(g_pRenderer->m_Fonts.Pixel);
    g_pRenderer->DrawOutlinedString(DistanceText.c_str(),Vector2D(textPos.x, textPos.y), color_t(255, 255, 255, 255), color_t(0, 0, 0, 100), true);
    ImGui::PopFont();
}

void CFeatures::ESP::Players::Flags(const EntityListInfo& playerInfo, CMath::BoundingBox& pos)
{

    if(!g_pGui->m_Vars.m_ESP.flags)
		return;

    bool HasHelmet = playerInfo.Pawn->GetItemServices()->HasHelmet();
    bool IsScoped = playerInfo.Pawn->IsScoped();
    C_BaseWeapon* PlayerWeapon = reinterpret_cast<C_BaseWeapon*>(g_pInterfaces->m_Interfaces.pEntityList->GetClientEntityFromHandle(playerInfo.Pawn->GetWeaponServices()->GetWeaponHandle()));
    if (!PlayerWeapon)
        return;
    bool IsReloading = PlayerWeapon->IsReloading();
    bool IsDefusing = playerInfo.Pawn->IsDefusing();
    int Money = playerInfo.Controller->GetMoneyServices()->GetMoney();
    std::string szHasHelmet, szIsScoped,szIsReloading,szIsDefusing,szMoney;
    if (HasHelmet) {
		szHasHelmet = "HK";
    }
	if (IsScoped) {
		szIsScoped = "SCOPED";
	}
    if (IsReloading) {
		szIsReloading = "RELOAD";
    }
	if (IsDefusing) {
		szIsDefusing = "DEFUSING";
	}
    if(Money > 0)
	{
		szMoney = std::to_string(Money) + "$";
	}

    ImGui::PushFont(g_pRenderer->m_Fonts.Pixel);

    float x = pos.x + pos.w + 3;
    float y = pos.y - 2;


    if (!szHasHelmet.empty()) {
        g_pRenderer->DrawOutlinedString(szHasHelmet.c_str(), Vector2D(x, y), color_t(255, 255, 255, 255), color_t(0, 0, 0, 100), false);
        y += ImGui::GetFontSize() - 2;
    }
    if (g_pGui->m_Vars.m_ESP.money)
    {
        if(!szMoney.empty())
		{
			g_pRenderer->DrawOutlinedString(szMoney.c_str(), Vector2D(x, y), color_t(148, 219, 77, 255), color_t(0, 0, 0, 100), false);
			y += ImGui::GetFontSize() - 2;
		}
    }

	if (!szIsScoped.empty()) {
		g_pRenderer->DrawOutlinedString(szIsScoped.c_str(), Vector2D(x, y), color_t(0, 140, 255, 255), color_t(0, 0, 0, 100), false);
		y += ImGui::GetFontSize() - 2;
	}

	if (!szIsReloading.empty()) {
		g_pRenderer->DrawOutlinedString(szIsReloading.c_str(), Vector2D(x, y), color_t(255, 165, 0, 255), color_t(0, 0, 0, 100), false);
		y += ImGui::GetFontSize() - 2;
	}

	if (!szIsDefusing.empty()) {
		g_pRenderer->DrawOutlinedString(szIsDefusing.c_str(), Vector2D(x, y), color_t(255, 0, 0, 255), color_t(0, 0, 0, 100), false);
		y += ImGui::GetFontSize();
	}

	ImGui::PopFont();
}