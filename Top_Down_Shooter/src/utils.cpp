#include "utils.h"

namespace utils {

    

    bool Button(Rectangle bounds, const std::string& text, int fontSize) {
       
        Vector2 mouse = GetMousePosition();
        bool hover = CheckCollisionPointRec(mouse, bounds);
        bool clicked = false;

        // Change color if hovered
        Color color = hover ? static_cast<Color>(Color{ 200,200,200,90 }): static_cast<Color>(Color{ 130,130,130,70 });

        DrawRectangleRec(bounds, color);
        DrawRectangleLinesEx(bounds, 6, {225,225,225,20});

        // Draw text centered
        int textWidth = MeasureText(text.c_str(), fontSize);
        DrawText(
            text.c_str(),
            bounds.x + bounds.width / 2 - textWidth / 2,
            bounds.y + bounds.height / 2 - fontSize / 2,
            fontSize,
            WHITE
        );

        if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            clicked = true;
            
        }

        return clicked;
    }

    Font LoadFontFromFile(const std::string& path) {
        return LoadFont(path.c_str());
    }

    void UnloadFontResource(Font font) {
        UnloadFont(font);
    }
    
}
