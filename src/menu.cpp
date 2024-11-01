#include <menu.hpp>
#include <raylib.h>

static const char *OPTIONS[] = {
    "Continue",
    "New Game",
    "Credits",
    "Exit",
};

static const char *OPTIONS2[] = {
    "New Game",
    "Credits",
    "Exit",
};

void showMenu(short option, bool hasSaveFile){
    int width = 1280,
        height = 720;

    float position = height/2 + 128;
    Font font = GetFontDefault();
    const char **options = hasSaveFile ? OPTIONS : OPTIONS2;

    for(int i = 0; i<(hasSaveFile ? 4 : 3); i++){
        bool selected = (i == option);
        float fontSize = selected ? 62 : 24;

        Vector2 dimension = MeasureTextEx(font, options[i], fontSize, 6);
        DrawTextPro(font, options[i], (Vector2) {(width/2)-(dimension.x/2), position}, (Vector2) {0,0}, 0, fontSize, 6, WHITE);
        position += fontSize+8;
    }
}

static const char *GAME_OVER_OPTIONS[] = {
    "Restart",
    "New Game",
    "Exit",
};

void showMenuGameover(short option){
    int width = 1280,
        height = 720;

    float position = height/2 + 128;
    Font font = GetFontDefault();

    for(int i = 0; i<3; i++){
        bool selected = (i == option);
        float fontSize = selected ? 62 : 24;

        Vector2 dimension = MeasureTextEx(font, GAME_OVER_OPTIONS[i], fontSize, 6);
        DrawTextPro(font, GAME_OVER_OPTIONS[i], (Vector2) {(width/2)-(dimension.x/2), position}, (Vector2) {0,0}, 0, fontSize, 6, WHITE);
        position += fontSize+8;
    }
}