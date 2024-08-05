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
    g_pFramework->GroupBox("tabs", ImVec2(84, 458), { "aimbot","anti aim","visuals","misc","players"}, clickedItem, 7);

    switch (clickedItem)
    {
    case 0:
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

        g_pFramework->SliderInt("Radius", &g_pGui->m_Vars.m_ESP.SnaplinesRadius, 0, 10,g_pGui->m_Vars.m_ESP.snaplines);
		ImGui::SetCursorPos(ImVec2(107, 270));
        g_pFramework->Combo("Position", &g_pGui->m_Vars.m_ESP.SnaplinesPosition, {"Center", "Top", "Bottom"},g_pGui->m_Vars.m_ESP.snaplines);



        ImGui::SetCursorPos(ImVec2(100, 303));
        g_pFramework->Tab("World", ImVec2(175, 186));
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
        g_pFramework->CheckBox("Defuse Kit",&g_pGui->m_Vars.m_ESP.defusekit);
        ImGui::SetCursorPos(ImVec2(107, 408));
        g_pFramework->CheckBox("Health Shot", &g_pGui->m_Vars.m_ESP.healthshot);
        ImGui::SetCursorPos(ImVec2(107, 423));
        g_pFramework->CheckBox("Chicken", &g_pGui->m_Vars.m_ESP.chicken);
        ImGui::SetCursorPos(ImVec2(107, 438));
		g_pFramework->CheckBox("Hostage", &g_pGui->m_Vars.m_ESP.hostages);

        ImGui::SetCursorPos(ImVec2(313, 31));
        g_pFramework->Tab("Models", ImVec2(175, 200));
        ImGui::SetCursorPos(ImVec2(320, 46));
        g_pFramework->CheckBox("Enable", &g_pGui->m_Vars.m_Models.enable);
        ImGui::SetCursorPos(ImVec2(320, 60));
        g_pFramework->Combo("Texture", &g_pGui->m_Vars.m_Models.SelectedTexture, { "FLAT" ,"TEXTURE","GLOW" },g_pGui->m_Vars.m_Models.enable);
        ImGui::SetCursorPos(ImVec2(320, 75));
		g_pFramework->CheckBox("Local Player", &g_pGui->m_Vars.m_Models.localPlayer);
        ImGui::SetCursorPos(ImVec2(459, 75));
        ImGui::ColorEdit4("##LocalPlayerColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_Models.LocalPlayerColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
        ImGui::SetCursorPos(ImVec2(320, 90));
        g_pFramework->CheckBox("Team", &g_pGui->m_Vars.m_Models.team);
        ImGui::SetCursorPos(ImVec2(459, 90));
        ImGui::ColorEdit4("##TeamColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_Models.TeamColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);
        ImGui::SetCursorPos(ImVec2(320, 105));
        g_pFramework->CheckBox("Enemy", &g_pGui->m_Vars.m_Models.enemy);
        ImGui::SetCursorPos(ImVec2(459, 105));
        ImGui::ColorEdit4("##EnemyColor", reinterpret_cast<float*>(&g_pGui->m_Vars.m_Models.EnemyColor), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueBar);

    }
        break;

    case 3:
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
