#pragma once

#include "../renderer/renderer.h"

class CFramework {
public:
    void CreatePolygon(int x, int y, int w, int h);
    void GroupBox(const char* label, ImVec2 size, const std::vector<std::string>& strings, int& clickedItem, int spacing);
    void Tab(const char* name, ImVec2 size);
    void CheckBox(const char* label, bool* v);
};
inline CFramework* g_pFramework = new CFramework();