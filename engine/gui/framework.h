#pragma once

#include "../renderer/renderer.h"
#include "gui.h"

class CFramework {
public:
    void CreatePolygon(int x, int y, int w, int h);
    void GroupBox(const char* label, ImVec2 size, const std::vector<std::string>& strings, int& clickedItem, int spacing);
    void Tab(const char* name, ImVec2 size);
    void CheckBox(const char* label, bool* v);
    void SliderInt(const char* label, int* v, int min, int max, bool enabled);
    void ComboConfig(const char* label, int* item_current, std::vector<std::string> items, bool enabled);
    void Combo(const char* label, int* item_current, std::vector<std::string> items, bool enabled);

    bool InputText(const char* label, char* buf, size_t buf_size);
    bool Button(const char* label);
};
inline CFramework* g_pFramework = new CFramework();