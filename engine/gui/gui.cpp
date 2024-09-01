#include "gui.h"

void CGui::DrawGui()
{
	if (!IsOpen)
		return;

    ImGui::SetNextWindowSize(ImVec2(w, h));
    ImGui::Begin("Polygon", &IsOpen, WindowFlags);
    SetupStyles();
    SetWindowPosition();

    g_pFramework->CreatePolygon(WindowPostion.x, WindowPostion.y, WindowSize.x, WindowSize.y);

    ImGui::SetCursorPos(ImVec2(6, 31));
    g_pFramework->GroupBox("tabs", ImVec2(84, 478), { "aimbot","anti aim","visuals","misc","players"}, clickedItem, 7);

    switch (clickedItem)
    {
    case 0:
        ImGui::SetCursorPos(ImVec2(100, 31));
        g_pFramework->Tab("General", ImVec2(175, 83));
        ImGui::SetCursorPos(ImVec2(107, 46));
        g_pFramework->CheckBox("Enable", &g_pGui->m_Vars.m_Aimbot.enable);
        ImGui::SetCursorPos(ImVec2(107, 61));
        g_pFramework->CheckBox("Ignore Team", &g_pGui->m_Vars.m_Aimbot.ignoreteam);
        ImGui::SetCursorPos(ImVec2(107, 76));
        g_pFramework->CheckBox("Silent Aim", &g_pGui->m_Vars.m_Aimbot.silentaim);
        ImGui::SetCursorPos(ImVec2(107, 91));
        g_pFramework->SliderInt("Fov", &g_pGui->m_Vars.m_Aimbot.fov, 0, 360, g_pGui->m_Vars.m_Aimbot.enable);

        ImGui::SetCursorPos(ImVec2(100, 124));
        g_pFramework->Tab("Targeting", ImVec2(175, 53));
        ImGui::SetCursorPos(ImVec2(107, 139));
        g_pFramework->Combo("Hitbox", &g_pGui->m_Vars.m_Aimbot.selectedhitbox, {"HEAD","BODY","FULL"}, g_pGui->m_Vars.m_Aimbot.enable);
        ImGui::SetCursorPos(ImVec2(107, 154));
        g_pFramework->Combo("Multipoint", &g_pGui->m_Vars.m_Aimbot.multipoint, {"LOW","MEDIUM","HIGH","EXTREME","NONE"}, g_pGui->m_Vars.m_Aimbot.enable);

        ImGui::SetCursorPos(ImVec2(100, 187));
        g_pFramework->Tab("Weapons", ImVec2(175, 68));
		ImGui::SetCursorPos(ImVec2(107, 202));
        g_pFramework->Combo("Slot", &g_pGui->m_Vars.m_Aimbot.selectedweaponslot, {"PISTOL","RIFLE","AUTO","SNIPER","SMG","HEAVY","ZEUS"}, g_pGui->m_Vars.m_Aimbot.enable);

        switch (g_pGui->m_Vars.m_Aimbot.selectedweaponslot)
        {
        case 0:
            ImGui::SetCursorPos(ImVec2(107, 217));
            g_pFramework->SliderInt("Hit rate", &g_pGui->m_Vars.m_Aimbot.hitchancePistol, 0, 100, g_pGui->m_Vars.m_Aimbot.enable);
            ImGui::SetCursorPos(ImVec2(107, 232));
            g_pFramework->SliderInt("Min dmg", &g_pGui->m_Vars.m_Aimbot.minimaldamagePistol, 0, 100, g_pGui->m_Vars.m_Aimbot.enable);
            break;
		case 1:
            ImGui::SetCursorPos(ImVec2(107, 217));
            g_pFramework->SliderInt("Hit rate", &g_pGui->m_Vars.m_Aimbot.hitchanceRifle, 0, 100, g_pGui->m_Vars.m_Aimbot.enable);
            ImGui::SetCursorPos(ImVec2(107, 232));
            g_pFramework->SliderInt("Min dmg", &g_pGui->m_Vars.m_Aimbot.minimaldamageRifle, 0, 100, g_pGui->m_Vars.m_Aimbot.enable);
			break;
		case 2:
            ImGui::SetCursorPos(ImVec2(107, 217));
            g_pFramework->SliderInt("Hit rate", &g_pGui->m_Vars.m_Aimbot.hitchanceAuto, 0, 100, g_pGui->m_Vars.m_Aimbot.enable);
            ImGui::SetCursorPos(ImVec2(107, 232));
            g_pFramework->SliderInt("Min dmg", &g_pGui->m_Vars.m_Aimbot.minimaldamageAuto, 0, 100, g_pGui->m_Vars.m_Aimbot.enable);
			break;
		case 3:
            ImGui::SetCursorPos(ImVec2(107, 217));
            g_pFramework->SliderInt("Hit rate", &g_pGui->m_Vars.m_Aimbot.hitchanceSniper, 0, 100, g_pGui->m_Vars.m_Aimbot.enable);
            ImGui::SetCursorPos(ImVec2(107, 232));
            g_pFramework->SliderInt("Min dmg", &g_pGui->m_Vars.m_Aimbot.minimaldamageSniper, 0, 100, g_pGui->m_Vars.m_Aimbot.enable);
			break;
		case 4:
            ImGui::SetCursorPos(ImVec2(107, 217));
            g_pFramework->SliderInt("Hit rate", &g_pGui->m_Vars.m_Aimbot.hitchanceSmg, 0, 100, g_pGui->m_Vars.m_Aimbot.enable);
            ImGui::SetCursorPos(ImVec2(107, 232));
            g_pFramework->SliderInt("Min dmg", &g_pGui->m_Vars.m_Aimbot.minimaldamageSmg, 0, 100, g_pGui->m_Vars.m_Aimbot.enable);
			break;
		case 5:
            ImGui::SetCursorPos(ImVec2(107, 217));
            g_pFramework->SliderInt("Hit rate", &g_pGui->m_Vars.m_Aimbot.hitchanceHeavy, 0, 100, g_pGui->m_Vars.m_Aimbot.enable);
            ImGui::SetCursorPos(ImVec2(107, 232));
            g_pFramework->SliderInt("Min dmg", &g_pGui->m_Vars.m_Aimbot.minimaldamageHeavy, 0, 100, g_pGui->m_Vars.m_Aimbot.enable);
			break;
		case 6:
            ImGui::SetCursorPos(ImVec2(107, 217));
            g_pFramework->SliderInt("Hit rate", &g_pGui->m_Vars.m_Aimbot.hitchanceZeus, 0, 100, g_pGui->m_Vars.m_Aimbot.enable);
            ImGui::SetCursorPos(ImVec2(107, 232));
            g_pFramework->SliderInt("Min dmg", &g_pGui->m_Vars.m_Aimbot.minimaldamageZeus, 0, 100, g_pGui->m_Vars.m_Aimbot.enable);
			break;

        }


        ImGui::SetCursorPos(ImVec2(313, 31));
        g_pFramework->Tab("Other", ImVec2(175, 113));
        ImGui::SetCursorPos(ImVec2(320, 46));
        g_pFramework->CheckBox("Auto Wall", &g_pGui->m_Vars.m_Aimbot.autowall);
        ImGui::SetCursorPos(ImVec2(320, 61));
        g_pFramework->CheckBox("Auto Stop", &g_pGui->m_Vars.m_Aimbot.autostop);
        ImGui::SetCursorPos(ImVec2(320, 76));
        g_pFramework->Combo("Modes", &g_pGui->m_Vars.m_Aimbot.selectedautostop, {"INSTANT","SLOW","EARLY"}, g_pGui->m_Vars.m_Aimbot.autostop);
        ImGui::SetCursorPos(ImVec2(320, 91));
        g_pFramework->CheckBox("Remove Recoil", &g_pGui->m_Vars.m_Aimbot.removerecoil);
        ImGui::SetCursorPos(ImVec2(320, 106));
        g_pFramework->CheckBox("Smoothness", &g_pGui->m_Vars.m_Aimbot.smoothness);
        ImGui::SetCursorPos(ImVec2(320, 121));
        g_pFramework->SliderInt("Value", &g_pGui->m_Vars.m_Aimbot.smoothnessvalue, 0, 100, g_pGui->m_Vars.m_Aimbot.smoothness);

        break;

    case 1:
        break;

    case 2:
    {
        ImGui::SetCursorPos(ImVec2(100, 31));
        g_pFramework->Tab("Players", ImVec2(175, 264));
        ImGui::SetCursorPos(ImVec2(107, 46));
        g_pFramework->CheckBox("Enable", &g_pGui->m_Vars.m_ESP.players);
        ImGui::SetCursorPos(ImVec2(107, 61));
        g_pFramework->CheckBox("Local", &g_pGui->m_Vars.m_ESP.localplayer);
        ImGui::SetCursorPos(ImVec2(107, 76));
        g_pFramework->CheckBox("Team", &g_pGui->m_Vars.m_ESP.team);
        ImGui::SetCursorPos(ImVec2(107, 91));
        g_pFramework->CheckBox("Name", &g_pGui->m_Vars.m_ESP.name);
        ImGui::SetCursorPos(ImVec2(246, 91));
        ImGui::ColorEdit4("##NamePlayerColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_ESP.NameColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

        ImGui::SetCursorPos(ImVec2(107, 106));
        g_pFramework->CheckBox("Bounding Box", &g_pGui->m_Vars.m_ESP.boxes);
        ImGui::SetCursorPos(ImVec2(246, 106));
        ImGui::ColorEdit4("##BoundingBoxColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_ESP.BoxesColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

        ImGui::SetCursorPos(ImVec2(107, 121));
        g_pFramework->CheckBox("Health", &g_pGui->m_Vars.m_ESP.health);
        ImGui::SetCursorPos(ImVec2(107, 136));
        g_pFramework->CheckBox("Weapon", &g_pGui->m_Vars.m_ESP.weapon);
        ImGui::SetCursorPos(ImVec2(246, 136));
        ImGui::ColorEdit4("##WeaponPlayerColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_ESP.WeaponColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

        ImGui::SetCursorPos(ImVec2(107, 151));
        g_pFramework->CheckBox("Ammo", &g_pGui->m_Vars.m_ESP.ammo);
        ImGui::SetCursorPos(ImVec2(246, 151));
        ImGui::ColorEdit4("##AmmoPlayerColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_ESP.AmmoColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

        ImGui::SetCursorPos(ImVec2(107, 166));
        g_pFramework->CheckBox("Distance", &g_pGui->m_Vars.m_ESP.distance);
        ImGui::SetCursorPos(ImVec2(107, 181));
        g_pFramework->CheckBox("Flags", &g_pGui->m_Vars.m_ESP.flags);
        ImGui::SetCursorPos(ImVec2(107, 196));
        g_pFramework->CheckBox("Money", &g_pGui->m_Vars.m_ESP.money);
        ImGui::SetCursorPos(ImVec2(107, 211));
        g_pFramework->CheckBox("Glow", &g_pGui->m_Vars.m_ESP.glow);
        ImGui::SetCursorPos(ImVec2(246, 211));
        ImGui::ColorEdit4("##GlowPlayerColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_ESP.GlowColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

        ImGui::SetCursorPos(ImVec2(107, 226));
        g_pFramework->CheckBox("Skeletons", &g_pGui->m_Vars.m_ESP.skeletons);
        ImGui::SetCursorPos(ImVec2(246, 226));
        ImGui::ColorEdit4("##SkeletonPlayerColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_ESP.SkeletonsColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

        ImGui::SetCursorPos(ImVec2(107, 241));
        g_pFramework->CheckBox("Snaplines", &g_pGui->m_Vars.m_ESP.snaplines);
        ImGui::SetCursorPos(ImVec2(246, 241));
        ImGui::ColorEdit4("##SnaplinesColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_ESP.SnapLinesColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
        ImGui::SetCursorPos(ImVec2(107, 255));

        g_pFramework->SliderInt("Radius", &g_pGui->m_Vars.m_ESP.SnaplinesRadius, 0, 10, g_pGui->m_Vars.m_ESP.snaplines);
        ImGui::SetCursorPos(ImVec2(107, 270));
        g_pFramework->Combo("Position", &g_pGui->m_Vars.m_ESP.SnaplinesPosition, { "Center", "Top", "Bottom" }, g_pGui->m_Vars.m_ESP.snaplines);



        ImGui::SetCursorPos(ImVec2(100, 303));
        g_pFramework->Tab("World", ImVec2(175, 206));
        ImGui::SetCursorPos(ImVec2(107, 318));
        g_pFramework->CheckBox("Enable", &g_pGui->m_Vars.m_ESP.world);
        ImGui::SetCursorPos(ImVec2(107, 333));
        g_pFramework->CheckBox("Dropped weapons", &g_pGui->m_Vars.m_ESP.droppedweapons);
        ImGui::SetCursorPos(ImVec2(107, 348));
        g_pFramework->CheckBox("Planted C4", &g_pGui->m_Vars.m_ESP.plantedc4);
        ImGui::SetCursorPos(ImVec2(107, 363));
        g_pFramework->CheckBox("Projectiles", &g_pGui->m_Vars.m_ESP.projectiles);
        ImGui::SetCursorPos(ImVec2(107, 378));
        g_pFramework->CheckBox("Bomb", &g_pGui->m_Vars.m_ESP.droppedbomb);
        ImGui::SetCursorPos(ImVec2(107, 393));
        g_pFramework->CheckBox("Defuse Kit", &g_pGui->m_Vars.m_ESP.defusekit);
        ImGui::SetCursorPos(ImVec2(107, 408));
        g_pFramework->CheckBox("Health Shot", &g_pGui->m_Vars.m_ESP.healthshot);
        ImGui::SetCursorPos(ImVec2(107, 423));
        g_pFramework->CheckBox("Chicken", &g_pGui->m_Vars.m_ESP.chicken);
        ImGui::SetCursorPos(ImVec2(107, 438));
        g_pFramework->CheckBox("Hostage", &g_pGui->m_Vars.m_ESP.hostages);

        ImGui::SetCursorPos(ImVec2(313, 31));
        g_pFramework->Tab("Models", ImVec2(175, 159));
        ImGui::SetCursorPos(ImVec2(320, 46));
        g_pFramework->CheckBox("Enable", &g_pGui->m_Vars.m_Models.enable);
        ImGui::SetCursorPos(ImVec2(320, 60));
        g_pFramework->CheckBox("Local Player", &g_pGui->m_Vars.m_Models.localPlayer);
        ImGui::SetCursorPos(ImVec2(459, 60));
        ImGui::ColorEdit4("##LocalPlayerColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_Models.LocalPlayerColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
        ImGui::SetCursorPos(ImVec2(320, 75));
        g_pFramework->Combo("Material Local", &g_pGui->m_Vars.m_Models.SelectedTextureLocal, { "FLAT" ,"TEXTURE","GLOW" }, g_pGui->m_Vars.m_Models.localPlayer);

        ImGui::SetCursorPos(ImVec2(320, 90));
        g_pFramework->CheckBox("Team", &g_pGui->m_Vars.m_Models.team);
        ImGui::SetCursorPos(ImVec2(459, 90));
        ImGui::ColorEdit4("##TeamColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_Models.TeamColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
        ImGui::SetCursorPos(ImVec2(320, 105));
        g_pFramework->CheckBox("Team Invisible", &g_pGui->m_Vars.m_Models.teaminvisible);
        ImGui::SetCursorPos(ImVec2(459, 105));
        ImGui::ColorEdit4("##TeamColorInvisible", reinterpret_cast<float*>(&g_pGui->m_Vars.m_Models.TeamColorInvisible), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
        ImGui::SetCursorPos(ImVec2(320, 120));
        g_pFramework->Combo("Material Team", &g_pGui->m_Vars.m_Models.SelectedTextureTeam, { "FLAT" ,"TEXTURE","GLOW" }, g_pGui->m_Vars.m_Models.team);

        ImGui::SetCursorPos(ImVec2(320, 135));
        g_pFramework->CheckBox("Enemy", &g_pGui->m_Vars.m_Models.enemy);
        ImGui::SetCursorPos(ImVec2(459, 135));
        ImGui::ColorEdit4("##EnemyColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_Models.EnemyColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
        ImGui::SetCursorPos(ImVec2(320, 150));
        g_pFramework->CheckBox("Enemy Invisible", &g_pGui->m_Vars.m_Models.enemyinvisible);
        ImGui::SetCursorPos(ImVec2(459, 150));
        ImGui::ColorEdit4("##EnemyColorInvisible", reinterpret_cast<float*>(&g_pGui->m_Vars.m_Models.EnemyColorInvisible), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
        ImGui::SetCursorPos(ImVec2(320, 165));
        g_pFramework->Combo("Material Enemy", &g_pGui->m_Vars.m_Models.SelectedTextureEnemy, { "FLAT" ,"TEXTURE","GLOW" }, g_pGui->m_Vars.m_Models.enemy);

        ImGui::SetCursorPos(ImVec2(313, 198));
        g_pFramework->Tab("World Modulation", ImVec2(175, 69));
        ImGui::SetCursorPos(ImVec2(320, 213));
        g_pFramework->CheckBox("Lighting", &g_pGui->m_Vars.m_WorldModulation.lighting);
        ImGui::SetCursorPos(ImVec2(459, 213));
        ImGui::ColorEdit4("##LightingColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_WorldModulation.LightingColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
        ImGui::SetCursorPos(ImVec2(320, 228));
        g_pFramework->SliderInt("Shine", &g_pGui->m_Vars.m_WorldModulation.LightingIntensity, 0, 10, g_pGui->m_Vars.m_WorldModulation.lighting);
        ImGui::SetCursorPos(ImVec2(320, 243));
        g_pFramework->CheckBox("World", &g_pGui->m_Vars.m_WorldModulation.worldcolor);
        ImGui::SetCursorPos(ImVec2(459, 243));
        ImGui::ColorEdit4("##WorldColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_WorldModulation.WorldColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

        ImGui::SetCursorPos(ImVec2(313, 276));
        g_pFramework->Tab("Other", ImVec2(175, 115));
        ImGui::SetCursorPos(ImVec2(320, 291));
        g_pFramework->CheckBox("Watermark", &g_pGui->m_Vars.m_OtherVisuals.Watermark);
		ImGui::SetCursorPos(ImVec2(320, 306));
		g_pFramework->CheckBox("Info Panel", &g_pGui->m_Vars.m_OtherVisuals.InfoPanel);
        ImGui::SetCursorPos(ImVec2(320, 321));
        g_pFramework->CheckBox("Custom Menu Theme", &g_pGui->m_Vars.m_OtherVisuals.CustomMenuTheme);
        ImGui::SetCursorPos(ImVec2(459, 321));
        ImGui::ColorEdit4("##MenuTheme", reinterpret_cast<float*>(&g_pGui->m_Vars.m_OtherVisuals.MenuThemeColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
        ImGui::SetCursorPos(ImVec2(320, 336));
        g_pFramework->CheckBox("Force Crosshair", &g_pGui->m_Vars.m_OtherVisuals.ForceCrosshair);
        ImGui::SetCursorPos(ImVec2(320, 351));
        g_pFramework->CheckBox("Sniper Zoom", &g_pGui->m_Vars.m_OtherVisuals.SniperZoom);
        ImGui::SetCursorPos(ImVec2(459, 351));
        ImGui::ColorEdit4("##SniperZoomColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_OtherVisuals.SniperZoomColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
        ImGui::SetCursorPos(ImVec2(435, 351));
        ImGui::ColorEdit4("##SniperZoomColorGradient", reinterpret_cast<float*>(&g_pGui->m_Vars.m_OtherVisuals.SniperZoomColorGradient), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
        ImGui::SetCursorPos(ImVec2(320, 366));
        g_pFramework->Combo("Zoom Type", &g_pGui->m_Vars.m_OtherVisuals.SniperZoomType, { "SIMPLE","MODERN","OLD"}, g_pGui->m_Vars.m_OtherVisuals.SniperZoom);

        ImGui::SetCursorPos(ImVec2(313, 399));
        g_pFramework->Tab("Removals", ImVec2(175, 110));
        ImGui::SetCursorPos(ImVec2(320, 414));
        g_pFramework->CheckBox("No Visual Recoil", &g_pGui->m_Vars.m_Removals.norecoil);
        ImGui::SetCursorPos(ImVec2(320, 429));
        g_pFramework->CheckBox("No Smoke", &g_pGui->m_Vars.m_Removals.nosmoke);
        ImGui::SetCursorPos(ImVec2(320, 444));
        g_pFramework->CheckBox("No Flash", &g_pGui->m_Vars.m_Removals.noflash);
        ImGui::SetCursorPos(ImVec2(320, 459));
        g_pFramework->CheckBox("No Zoom", &g_pGui->m_Vars.m_Removals.nozoom);
        ImGui::SetCursorPos(ImVec2(320, 474));
        g_pFramework->CheckBox("No Legs", &g_pGui->m_Vars.m_Removals.nolegspreview);

    }
        break;

    case 3:
        ImGui::SetCursorPos(ImVec2(100, 31));
        g_pFramework->Tab("Movement", ImVec2(175, 84));
        ImGui::SetCursorPos(ImVec2(107, 46));
        g_pFramework->CheckBox("Auto Jump", &g_pGui->m_Vars.m_Movement.bunnyhop);
        ImGui::SetCursorPos(ImVec2(107, 61));
        g_pFramework->CheckBox("Auto Strafe", &g_pGui->m_Vars.m_Movement.autostrafe);
        ImGui::SetCursorPos(ImVec2(107, 76));
        g_pFramework->CheckBox("Duck Jump", &g_pGui->m_Vars.m_Movement.duckinair);
        ImGui::SetCursorPos(ImVec2(107, 91));
        g_pFramework->CheckBox("Crouch in Air", &g_pGui->m_Vars.m_Movement.crouchjump);

        ImGui::SetCursorPos(ImVec2(100, 124));
        g_pFramework->Tab("View", ImVec2(175, 145));
        ImGui::SetCursorPos(ImVec2(107, 139));
        g_pFramework->CheckBox("Fov Changer", &g_pGui->m_Vars.m_View.fovchanger);
        ImGui::SetCursorPos(ImVec2(107, 154));
        g_pFramework->SliderInt("Value", &g_pGui->m_Vars.m_View.fov, 0, 180, g_pGui->m_Vars.m_View.fovchanger);
        ImGui::SetCursorPos(ImVec2(107, 169));
        g_pFramework->CheckBox("While Zoomed", &g_pGui->m_Vars.m_View.fovwhilescoped);
        ImGui::SetCursorPos(ImVec2(107, 184));
        g_pFramework->CheckBox("Viewmodel Changer", &g_pGui->m_Vars.m_View.viewmodelchanger);
        ImGui::SetCursorPos(ImVec2(107, 199));
        g_pFramework->SliderInt("FOV", &g_pGui->m_Vars.m_View.viewmodelfov, 0, 180, g_pGui->m_Vars.m_View.viewmodelchanger);
        ImGui::SetCursorPos(ImVec2(107, 214));
        g_pFramework->SliderInt("Value X", &g_pGui->m_Vars.m_View.viewmodelx, -10, 10, g_pGui->m_Vars.m_View.viewmodelchanger);
        ImGui::SetCursorPos(ImVec2(107, 229));
        g_pFramework->SliderInt("Value Y", &g_pGui->m_Vars.m_View.viewmodely, -10, 10, g_pGui->m_Vars.m_View.viewmodelchanger);
        ImGui::SetCursorPos(ImVec2(107, 244));
        g_pFramework->SliderInt("Value Z", &g_pGui->m_Vars.m_View.viewmodelz, -10, 10, g_pGui->m_Vars.m_View.viewmodelchanger);

        ImGui::SetCursorPos(ImVec2(313, 31));
        g_pFramework->Tab("Configuration", ImVec2(175, 100));
        ImGui::SetCursorPos(ImVec2(320, 46));
        g_pFramework->InputText("Name Config", g_pGui->m_Vars.m_Configuration.CustomNameConfig, IM_ARRAYSIZE(&g_pGui->m_Vars.m_Configuration.CustomNameConfig));
        ImGui::SetCursorPos(ImVec2(320, 61));
        g_pFramework->ComboConfig("Config", &g_pGui->m_Vars.m_Configuration.SelectedIndexConfig, g_pGui->m_Vars.m_Configuration.ConfigNames, g_pGui->m_Vars.m_Configuration.ConfigNames.size());
        ImGui::SetCursorPos(ImVec2(320, 76));
        if (g_pFramework->Button("Save"))
        {
            ConfigSystem->UpdateConfiguration();
            if (strlen(g_pGui->m_Vars.m_Configuration.CustomNameConfig) == 0) {
                ConfigSystem->SaveConfiguarion();
            }
            else {
                g_pGui->m_Vars.m_Configuration.ConfigNames[g_pGui->m_Vars.m_Configuration.SelectedIndexConfig] = g_pGui->m_Vars.m_Configuration.CustomNameConfig;
                ConfigSystem->SaveConfiguarion();
            }
        }
		ImGui::SetCursorPos(ImVec2(320, 91));
        if (g_pFramework->Button("Load"))
        {
            ConfigSystem->UpdateConfiguration();
            ConfigSystem->LoadConfiguration(g_pGui->m_Vars.m_Configuration.ConfigNames, g_pGui->m_Vars.m_Configuration.SelectedIndexConfig);
        }

		ImGui::SetCursorPos(ImVec2(320, 106));
		if (g_pFramework->Button("Refresh"))
		{
			ConfigSystem->UpdateConfiguration();
		}
        break;

    case 4:
        break;
    }

    ImGui::End();

}

void CGui::SetWindowPosition() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 pos = ImGui::GetWindowPos();
    ImVec2 size = ImGui::GetWindowSize();

    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        if (ImGui::IsWindowHovered()) {
            if (!IsDragging) {
                IsDragging = true;
                DragDelta = ImVec2(ImGui::GetMousePos().x - pos.x, ImGui::GetMousePos().y - pos.y);
            }
            pos = ImVec2(ImGui::GetMousePos().x - DragDelta.x, ImGui::GetMousePos().y - DragDelta.y);
        }
    }
    else {
        IsDragging = false;
    }

    if (pos.x < 0) pos.x = 0;
    if (pos.y < 0) pos.y = 0;
    if (pos.x + size.x > io.DisplaySize.x) pos.x = io.DisplaySize.x - size.x;
    if (pos.y + size.y > io.DisplaySize.y) pos.y = io.DisplaySize.y - size.y;

    ImGui::SetWindowPos(pos);


    WindowPostion = pos;
    WindowSize = size;
}

void CGui::SetupStyles()
{
    auto colors = ImGui::GetStyle().Colors;
    ImGuiStyle& style = ImGui::GetStyle();

    /// @style
    style.WindowBorderSize = 0;

    /// @colors
    colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 0);

}
