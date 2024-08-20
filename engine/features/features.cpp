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

        if ((entity.Pawn != Globals::LocalPlayerPawn && Globals::LocalPlayerPawn->GetTeam() == entity.Pawn->GetTeam() && !g_pGui->m_Vars.m_ESP.team) ||
            (entity.Pawn == Globals::LocalPlayerPawn && !g_pGui->m_Vars.m_ESP.localplayer))
            continue;

        if (IsPlayer && IsAlive)
        {
            if (g_pMath->GetPlayerBoundingBox(entity.Pawn, bbox))
            {
                Box(bbox);
                Name(bbox, entity);
                HealthBar(bbox, entity);
                Flags(entity, bbox);
                Ammo(bbox, entity);
                Weapon(bbox, entity);
                Distance(entity, bbox);
                Skeleton(entity);
                SnapLines(entity);
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

    const Vector2D position(pos.x + pos.w / 2, pos.y - 8);

    std::string nameLower = player.Controller->GetPlayerName();
    std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

    std::wstring wideName = g_pUtils->m_Helpers.UTF8ToWide(nameLower);
    nameLower = g_pUtils->m_Helpers.WideToUTF8(wideName);

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
    bool IsBot = playerInfo.Controller->IsBot();
    int Money = playerInfo.Controller->GetMoneyServices()->GetMoney();

    std::string szHasHelmet, szIsScoped, szIsReloading, szIsDefusing, szMoney, szIsBot;
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
    if (IsBot)
    {
		szIsBot = "B";
    }
    ImGui::PushFont(g_pRenderer->m_Fonts.Pixel);

    float x = pos.x + pos.w + 3;
    float y = pos.y - 2;

    if (!szIsBot.empty())
    {
		g_pRenderer->DrawOutlinedString(szIsBot.c_str(), Vector2D(x, y), color_t(255, 0, 0, 255), color_t(0, 0, 0, 100), false);
		y += ImGui::GetFontSize() - 2;
    }
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

void CFeatures::ESP::Players::SnapLines(const EntityListInfo& playerInfo)
{
    if (!g_pGui->m_Vars.m_ESP.snaplines)
        return;

    if (playerInfo.Pawn == Globals::LocalPlayerPawn)
        return;

    Vector3D playerPosition = playerInfo.Pawn->GetBaseEntity()->GetGameSceneNode()->GetVecOrigin();
    Vector2D screenPosition;

    color_t color = color_t(g_pGui->m_Vars.m_ESP.SnapLinesColor.x * 255, g_pGui->m_Vars.m_ESP.SnapLinesColor.y * 255, g_pGui->m_Vars.m_ESP.SnapLinesColor.z * 255, g_pGui->m_Vars.m_ESP.SnapLinesColor.w * 255);

    if (g_pMath->WorldToScreen(playerPosition, screenPosition)) {
        float centerX = ImGui::GetIO().DisplaySize.x / 2.0f;
        float centerY = ImGui::GetIO().DisplaySize.y / 2.0f;
        float radius = g_pGui->m_Vars.m_ESP.SnaplinesRadius * 10;

        float startX = centerX, startY = centerY;

        switch (g_pGui->m_Vars.m_ESP.SnaplinesPosition) {
        case 0:
            startX = centerX;
            startY = centerY;
            break;
        case 1:
            startX = centerX;
            startY = 0.0f;
            break;
        case 2:
            startX = centerX;
            startY = ImGui::GetIO().DisplaySize.y;
            break;
        }

        float deltaY = screenPosition.y - startY;
        float deltaX = screenPosition.x - startX;
        float distance = sqrt(deltaX * deltaX + deltaY * deltaY);
        float angle = atan2(deltaY, deltaX);

        float adjustedStartX = startX + radius * cos(angle);
        float adjustedStartY = startY + radius * sin(angle);

        if (distance >= radius) {
            g_pRenderer->DrawLine(adjustedStartX, adjustedStartY, screenPosition.x, screenPosition.y, color, 0.0);
        }
    }
}
void CFeatures::ESP::Players::Skeleton(const EntityListInfo& playerInfo)
{
    if (!g_pGui->m_Vars.m_ESP.skeletons)
        return;

    color_t colour = color_t(g_pGui->m_Vars.m_ESP.SkeletonsColor.x * 255, g_pGui->m_Vars.m_ESP.SkeletonsColor.y * 255, g_pGui->m_Vars.m_ESP.SkeletonsColor.z * 255, g_pGui->m_Vars.m_ESP.SkeletonsColor.w * 255);
    Vector2D screenPositions[28];
    bool boneVisible[28] = { false };

    for (int i = 0; i <= 27; i++) {
        Vector3D bonePosition = playerInfo.Pawn->GetBaseEntity()->GetBonePosition(i);

        if (g_pMath->WorldToScreen(bonePosition, screenPositions[i])) {
            boneVisible[i] = true;
        }
    }

    for (int i = 0; i < 16; i++) {
        int bone1 = boneConnections[i][0];
        int bone2 = boneConnections[i][1];

        if (boneVisible[bone1] && boneVisible[bone2]) {
            g_pRenderer->DrawLine(screenPositions[bone1], screenPositions[bone2], colour);
        }
    }
}

void CFeatures::ESP::World::Draw()
{
    if (!g_pGui->m_Vars.m_ESP.world || !g_pInterfaces->m_Interfaces.pEngineClient->IsInGame())
        return;

    for (int i = 0; i <= g_pInterfaces->m_Interfaces.pEntityList->GetHighestEntityIndex(); i++)
    {
		C_BaseEntity* Entity = g_pInterfaces->m_Interfaces.pEntityList->GetClientEntity(i);
		if (!Entity)
			continue;
		Weapons(Entity);
        //Bomb(Entity);
       // Projectiles(Entity);
    }
  
}

void CFeatures::ESP::World::Weapons(C_BaseEntity* Entity)
{
    if (!g_pGui->m_Vars.m_ESP.droppedweapons)
        return;

    C_BaseWeapon* Weapon = reinterpret_cast<C_BaseWeapon*>(Entity);
    if (!Weapon || g_pInterfaces->m_Interfaces.pEntityList->GetClientEntityFromHandle(Entity->GetHandleEntity()))
        return;

    CCSWeaponData* WeaponData = Weapon->GetWeaponDataInfo();
    if (!WeaponData)
        return;

    int index = Weapon->GetManagerAttribute().GetItem().GetItemDefinitionIndex();
    if (!index)
        return;

   Vector3D Position = Entity->GetGameSceneNode()->GetVecOrigin();
    if (Position.IsZero())
        return;

    Vector2D screenPos;
    if (!g_pMath->WorldToScreen(Position, screenPos))
        return;

    std::string weaponString = Weapon->GetWeaponString(index);
    if(weaponString.empty())
        return;

    ImGui::PushFont(g_pRenderer->m_Fonts.Pixel);

	g_pRenderer->DrawOutlinedString(weaponString.c_str(), Vector2D(screenPos.x, screenPos.y), color_t(255, 255, 255, 255), color_t(0, 0, 0, 100), false);

    ImGui::PopFont();
}

void CFeatures::ESP::World::Bomb(C_BaseEntity* Entity)
{
    if (!g_pGui->m_Vars.m_ESP.droppedbomb)
        return;
    C_BaseWeapon* Weapon = reinterpret_cast<C_BaseWeapon*>(Entity);
    if (!Weapon || g_pInterfaces->m_Interfaces.pEntityList->GetClientEntityFromHandle(Entity->GetHandleEntity()))
        return;

    CCSWeaponData* WeaponData = Weapon->GetWeaponDataInfo();
    if (!WeaponData)
        return;

    int index = Weapon->GetManagerAttribute().GetItem().GetItemDefinitionIndex();
    if (!index)
        return;

    Vector3D Position = Entity->GetGameSceneNode()->GetVecOrigin();
    if (Position.IsZero())
        return;

    Vector2D screenPos;
    if (!g_pMath->WorldToScreen(Position, screenPos))
        return;

    std::string weaponString = Weapon->GetWeaponString(index);

    if (weaponString.find("C4 Explosive") != std::string::npos)
    {

        ImGui::PushFont(g_pRenderer->m_Fonts.Pixel);
        g_pRenderer->DrawOutlinedString("BOMB", Vector2D(screenPos.x, screenPos.y), color_t(0, 140, 255, 255), color_t(0, 0, 0, 100), false);

        ImGui::PopFont();
    }
}

void CFeatures::ESP::World::Projectiles(C_BaseEntity* Entity)
{
    if (!g_pGui->m_Vars.m_ESP.projectiles)
        return;
       
    ImGui::PushFont(g_pRenderer->m_Fonts.Pixel);

    ImGui::PopFont();
}

void CFeatures::ESP::OthersVisuals::Watermark()
{

    if (!g_pGui->m_Vars.m_OtherVisuals.Watermark)
        return;
    bool IsCustomThemeToogle = g_pGui->m_Vars.m_OtherVisuals.CustomMenuTheme;

    int x = 1, y = 1, w = 200, h = 20;

    g_pRenderer->DrawFilledRect(x, y, w, h, color_t(9, 9, 9, 255));

    g_pRenderer->DrawRect(x, y, w, h, color_t(9, 9, 9, 255));

    g_pRenderer->DrawRect(x + 1, y + 1, w - 2, h - 2, color_t(46, 46, 46));


    if (IsCustomThemeToogle)
    {
        color_t coulour = color_t(g_pGui->m_Vars.m_OtherVisuals.MenuThemeColor.x * 255, g_pGui->m_Vars.m_OtherVisuals.MenuThemeColor.y * 255, g_pGui->m_Vars.m_OtherVisuals.MenuThemeColor.z * 255, 255);
        g_pRenderer->DrawLine(x + 2, y + h - 17, x + w - 2, y + h - 17, color_t(coulour));
    }
    else
    {
        g_pRenderer->DrawLine(x + 2, y + h - 17, x + w - 2, y + h - 17, color_t(120, 150, 255, 255));
    }


#ifdef _DEBUG
    ImGui::PushFont(g_pRenderer->m_Fonts.Pixel);
    g_pRenderer->DrawOutlinedString("poseidon.pw - debug", Vector2D(x + 5, y + 6), color_t(255, 255, 255, 255), color_t(0, 0, 0, 255), false);
    ImGui::PopFont();
#endif // _DEBUG

	ImGui::PushFont(g_pRenderer->m_Fonts.Pixel);
	g_pRenderer->DrawOutlinedString("poseidon.pw", Vector2D(x + 5, y + 6), color_t(255, 255, 255, 255), color_t(0, 0, 0, 255), false);
	ImGui::PopFont();
}

void CFeatures::ESP::OthersVisuals::InfoPanel()
{
}

void CFeatures::ESP::OthersVisuals::SniperScopeOverlay()
{
    static float animationProgress = 0.0f;
    static bool isScoped = false;
    float animationSpeed = 5.0f;

    if (!g_pGui->m_Vars.m_OtherVisuals.SniperZoom || !g_pGui->m_Vars.m_Removals.nozoom)
        return;
    if (!g_pInterfaces->m_Interfaces.pEngineClient->IsInGame() || !g_pInterfaces->m_Interfaces.pEngineClient->IsConnected())
        return;
    if (!Globals::LocalPlayerPawn->IsAlive())
        return;

    bool currentlyScoped = Globals::LocalPlayerPawn->IsScoped();

    if (currentlyScoped && !isScoped) {
        isScoped = true;
    }
    else if (!currentlyScoped && isScoped) {
        isScoped = false;
    }

    if (isScoped) {
        animationProgress += animationSpeed * ImGui::GetIO().DeltaTime;
        if (animationProgress > 1.0f) animationProgress = 1.0f;
    }
    else {
        animationProgress -= animationSpeed * ImGui::GetIO().DeltaTime;
        if (animationProgress < 0.0f) animationProgress = 0.0f;
    }

    int width = ImGui::GetIO().DisplaySize.x;
    int height = ImGui::GetIO().DisplaySize.y;

    if (animationProgress > 0.0f) {
        switch (g_pGui->m_Vars.m_OtherVisuals.SniperZoomType)
        {
        case 0:
        {
            color_t colour = color_t(g_pGui->m_Vars.m_OtherVisuals.SniperZoomColor.x * 255, g_pGui->m_Vars.m_OtherVisuals.SniperZoomColor.y * 255, g_pGui->m_Vars.m_OtherVisuals.SniperZoomColor.z * 255, g_pGui->m_Vars.m_OtherVisuals.SniperZoomColor.w * 255);

            float final_distance = 50.0f * animationProgress;

            int centerX = width / 2;
            int centerY = height / 2;

            g_pRenderer->DrawLine(centerX - final_distance, centerY, centerX + final_distance, centerY, colour);

            g_pRenderer->DrawLine(centerX, centerY - final_distance, centerX, centerY + final_distance, colour);
        }
        break;
        case 1:
        {
            float final_distance = 50.0f * animationProgress;
            float height_1 = 2.0f;
            float height_2 = 2.0f;
            float final_width = 50.0f * animationProgress;

            color_t color_first = color_t(g_pGui->m_Vars.m_OtherVisuals.SniperZoomColor.x * 255, g_pGui->m_Vars.m_OtherVisuals.SniperZoomColor.y * 255, g_pGui->m_Vars.m_OtherVisuals.SniperZoomColor.z * 255, g_pGui->m_Vars.m_OtherVisuals.SniperZoomColor.w * 255);
            color_t color_second = color_t(g_pGui->m_Vars.m_OtherVisuals.SniperZoomColorGradient.x * 255, g_pGui->m_Vars.m_OtherVisuals.SniperZoomColorGradient.y * 255, g_pGui->m_Vars.m_OtherVisuals.SniperZoomColorGradient.z * 255, g_pGui->m_Vars.m_OtherVisuals.SniperZoomColorGradient.w * 255);

            g_pRenderer->LinearGradient(width / 2 + final_distance, height / 2 - height_1, final_width, height_2, color_first, color_second, false);
            g_pRenderer->LinearGradient(width / 2 - final_distance, height / 2 - height_1, -final_width, height_2, color_first, color_second, false);

            g_pRenderer->LinearGradient(width / 2 - height_1, height / 2 + final_distance, height_2, final_width, color_first, color_second, true);
            g_pRenderer->LinearGradient(width / 2 - height_2, height / 2 - final_distance, height_2, -final_width, color_first, color_second, true);
        }
        break;

        case 2:
        {
            color_t colour = color_t(g_pGui->m_Vars.m_OtherVisuals.SniperZoomColor.x * 255, g_pGui->m_Vars.m_OtherVisuals.SniperZoomColor.y * 255, g_pGui->m_Vars.m_OtherVisuals.SniperZoomColor.z * 255, g_pGui->m_Vars.m_OtherVisuals.SniperZoomColor.w * 255);

            int centerX = width / 2;
            int centerY = height / 2;
            int lineX = static_cast<int>(centerX * (1.0f - animationProgress));
            int lineY = static_cast<int>(centerY * (1.0f - animationProgress));

            g_pRenderer->DrawLine(centerX - lineX, 0, centerX + lineX, height, colour);
            g_pRenderer->DrawLine(0, centerY - lineY, width, centerY + lineY, colour);
        }
        break;
        }
    }
}

void CFeatures::Aimbot::Run(C_UserCmd* cmd)
{
    if (!g_pGui->m_Vars.m_Aimbot.enable || !g_pInterfaces->m_Interfaces.pEngineClient->IsInGame() || !g_pInterfaces->m_Interfaces.pEngineClient->IsConnected())
        return;

    std::vector<EntityListInfo> Entities;
	g_pInterfaces->m_Interfaces.pEntityList->UpdateEntities(Entities);


   C_PlayerPawn* Target = nullptr;

    for (auto& entity : Entities)
    {
        if(!entity.Pawn->IsAlive() || Globals::LocalPlayerPawn == entity.Pawn || entity.Pawn->GetTeam() == Globals::LocalPlayerPawn->GetTeam())
			continue;

        Vector3D Bone = entity.Pawn->GetBaseEntity()->GetBonePosition(6);

        Vector3D Angle = g_pMath->CalcAngle(Globals::LocalPlayerPawn->GetBaseEntity()->GetEyePosition(), Bone);
      
        Angle.Normalize();


         g_pInterfaces->m_Interfaces.pGameInput->SetViewAngles(Angle);

  
    }


}
