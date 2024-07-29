#include "framework.h"

void CFramework::CreatePolygon(int x, int y, int w, int h)
{
        g_pRenderer->DrawFilledRect(x, y, w, h, color_t(9, 9, 9, 255));

        g_pRenderer->DrawRect(x, y, w, h, color_t(9, 9, 9, 255));

        g_pRenderer->DrawRect(x + 1, y + 1, w - 2, h - 2, color_t(46, 46, 46));
        g_pRenderer->DrawRect(x + 1, y + 1, w - 2, h - 479, color_t(46, 46, 46, 255));

        g_pRenderer->DrawLine(x + 2, y + h - 479 + 2, x + w - 2, y + h - 479 + 2, color_t(120, 150, 255, 255));

        g_pRenderer->DrawLine(x + 2, y + h - 4, x + w - 2, y + h - 4, color_t(120, 150, 255, 255));

        ImGui::PushFont(g_pRenderer->m_Fonts.Pixel);
        g_pRenderer->DrawOutlinedString("poseidon.pw - dev by ", Vector2D(x + 5, y + 6), color_t(255, 255, 255, 255), color_t(0, 0, 0, 255), false);

        ImVec2 poseidonTextSize = ImGui::CalcTextSize("poseidon.pw - dev by ");
        float poseidonTextX = x + 5 + poseidonTextSize.x;
        float poseidonTextY = y + 6;

        g_pRenderer->DrawOutlinedString("ogpolak", Vector2D(poseidonTextX, poseidonTextY), color_t(120, 150, 255, 255), color_t(0, 0, 0, 255), false);

        ImVec2 dateTextSize = ImGui::CalcTextSize("monday, 29 july 2024");
        float dateTextX = x + w - 5 - dateTextSize.x;
        float dateTextY = poseidonTextY;

        g_pRenderer->DrawOutlinedString("monday, 29 july 2024",Vector2D(dateTextX, dateTextY), color_t(255, 255, 255, 255), color_t(0, 0, 0, 255), false);

        ImGui::PopFont();
}



void CFramework::GroupBox(const char* label, ImVec2 size, const std::vector<std::string>& strings, int& clickedItem, int spacing)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImVec2 Position = window->DC.CursorPos;

    g_pRenderer->DrawFilledRect(Position.x, Position.y, size.x, size.y, color_t(9, 9, 9));
    g_pRenderer->DrawRect(Position.x, Position.y, size.x, size.y, color_t(46, 46, 46));

    ImGui::PushFont(g_pRenderer->m_Fonts.Pixel);

    Position.x += 5;
    Position.y += 4;

    for (int i = 0; i < strings.size(); ++i)
    {
        ImVec2 textSize = ImGui::CalcTextSize(strings[i].c_str());

        ImVec2 itemMin = ImVec2(Position.x - 3, Position.y - 1);
        ImVec2 itemMax = ImVec2(Position.x - 3 + 80, Position.y - 1 + textSize.y + 2);

        bool isHovered = ImGui::IsMouseHoveringRect(itemMin, itemMax);
        bool isClicked = isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);

        if (isClicked)
        {
            clickedItem = i;
        }

        color_t textColor = color_t(255, 255, 255, 255);
        color_t lineColor = color_t(255, 255, 255, 255);
        color_t rectColor = color_t(60, 60, 60, 255);

        if (clickedItem == i)
        {
            textColor = color_t(167, 178, 253, 255);
            lineColor = color_t(120, 150, 255, 255);
            rectColor = color_t(100, 100, 100, 255);
        }
        else if (isHovered)
        {
            textColor = color_t(46, 46, 46);
            lineColor = color_t(46, 46, 46, 255);
            rectColor = color_t(80, 80, 80, 255);
        }

        g_pRenderer->DrawRect(Position.x - 3, Position.y + 1, 80, textSize.y + 3, rectColor);

        g_pRenderer->DrawLine(Vector2D(Position.x - 1, Position.y + 2), Vector2D(Position.x - 1, Position.y + 3 + textSize.y), lineColor, 2);

        float textPosX = Position.x + (size.x - textSize.x - 10) / 2;

        g_pRenderer->DrawOutlinedString(strings[i].c_str(), Vector2D(textPosX, Position.y + 2), textColor, color_t(0, 0, 0, 255), false);

        Position.y += textSize.y + spacing;
    }

    ImVec2 imagePosition = ImVec2(Position.x + (size.x - 115) / 2, Position.y + size.y - 180);

    g_pRenderer->DrawImage(g_pRenderer->m_Fonts.LogoHack, imagePosition.x, imagePosition.y, 100, 100);

    ImGui::PopFont();
}

void CFramework::Tab(const char* name, ImVec2 size)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImVec2 Position = window->DC.CursorPos;

    bool IsHovered = ImGui::IsMouseHoveringRect(Position, ImVec2(Position.x + size.x, Position.y + size.y));

    color_t rectColor;
    if (IsHovered)
    {
        rectColor = color_t(60, 60, 60, 255);
    }
    else
    {
        rectColor = color_t(40, 40, 40, 255);
    }

    g_pRenderer->DrawFilledRect(Position.x, Position.y, size.x, size.y, color_t(9, 9, 9));
    g_pRenderer->DrawRect(Position.x, Position.y, size.x, size.y, rectColor);


    ImVec2 textSize = ImGui::CalcTextSize(name);

    float textX = Position.x + 10.0f;
    float textY = Position.y - 4;

    ImGui::PushFont(g_pRenderer->m_Fonts.Pixel);
    g_pRenderer->DrawOutlinedString(name,Vector2D(textX, textY), color_t(255, 255, 255, 255), color_t(0, 0, 0, 255), false);
    ImGui::PopFont();
}

void CFramework::CheckBox(const char* label, bool* v)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImVec2 position = window->DC.CursorPos;
    ImVec2 checkboxSize(9,9);
    ImVec2 labelSize = ImGui::CalcTextSize(label);

    ImVec2 checkboxPos(position.x + 2, position.y);
    ImVec2 textPos(position.x + checkboxSize.x + ImGui::GetStyle().ItemInnerSpacing.x, position.y + (checkboxSize.y - labelSize.y) / 2);

    ImRect checkboxRect(checkboxPos, ImVec2(checkboxPos.x + checkboxSize.x, checkboxPos.y + checkboxSize.y));

    bool isHovered = ImGui::IsMouseHoveringRect(checkboxRect.Min, checkboxRect.Max);
    bool isClicked = isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);

    if (isClicked)
    {
        *v = !*v;
    }

    color_t textColor = *v ? color_t(255,255,255, 255) : color_t(100,100,100);

    color_t fillColor = *v ? color_t(120, 150, 255, 255) : color_t(9, 9, 9);
    if (isHovered)
    {
        fillColor = color_t(20, 20, 20);
        textColor = color_t(150,150,150);
    }

    g_pRenderer->DrawFilledRect(checkboxPos.x, checkboxPos.y, checkboxSize.x, checkboxSize.y, fillColor);
    g_pRenderer->DrawRect(checkboxPos.x, checkboxPos.y, checkboxSize.x, checkboxSize.y, color_t(46, 46, 46));

    ImGui::PushFont(g_pRenderer->m_Fonts.Pixel);
    g_pRenderer->DrawOutlinedString(label,Vector2D(textPos.x + 10, textPos.y + 1), textColor, color_t(0, 0, 0, 255), false);
    ImGui::PopFont();

    ImGui::SetCursorPosY(position.y + checkboxSize.y + ImGui::GetStyle().ItemSpacing.y);

    ImGui::ItemSize(checkboxRect);
    ImGui::ItemAdd(checkboxRect, ImGui::GetID(label));
}