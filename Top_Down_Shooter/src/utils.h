#pragma once
#include "raylib.h"
#include "raymath.h"
#include <string>

namespace utils {
   
    bool Button(Rectangle bounds, const std::string& text, int fontSize = 20);
    extern Font mainFont;          
    Font LoadFontFromFile(const std::string& path);
    void UnloadFontResource(Font font);
    bool DrawStatBar(Vector2 pos, float width, float height, int level, int maxLevel,
        int cost, int& coins, const char* label, Color fillColor);
}
