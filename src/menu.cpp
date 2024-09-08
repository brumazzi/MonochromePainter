#include <menu.hpp>
#include <raylib.h>

static const char *OPTIONS[] = {
    "Continue",
    "New Game",
    "Credits",
    "Exit"
};

static const char *OPTIONS2[] = {
    "New Game",
    "Credits",
    "Exit"
};

void showMenu(short option, bool hasSaveFile){
    int width = GetScreenWidth(),
        height = GetScreenHeight();

    float position = height/2 + 64;
    Font font = GetFontDefault();
    const char **options = hasSaveFile ? OPTIONS : OPTIONS2;

    for(int i = 0; i<4; i++){
        if(hasSaveFile && i == 0) continue;
        bool selected = (i == option);
        float fontSize = selected ? 62 : 24;

        Vector2 dimension = MeasureTextEx(font, options[i], fontSize, 6);
        DrawTextPro(font, options[i], (Vector2) {(width/2)-(dimension.x/2), position}, (Vector2) {0,0}, 0, fontSize, 6, WHITE);
        position += fontSize+8;
    }
}