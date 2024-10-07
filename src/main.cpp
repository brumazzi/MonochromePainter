#include <raylib.h>
// #include <assets.hpp>
#include <player.hpp>
#include <monster.hpp>
#include <stage.hpp>
#include <game.hpp>
#include <iostream>
#include <assets.h>
#include <menu.hpp>
#include <filesystem>
#include <algorithm>
#include <dirent.h>
#include <cstdio>
#include <string.h>
#include <steam.hpp>

typedef struct _credits{
    unsigned short fontSize;
    const char *text;
    Color color;
} Credits;

const Credits CREDITS[] = {
    {64, "THANK YOU FOR PLAY", {0xea, 0xc5, 0x50, 0xff}},
    {48, "\n", {0x00, 0x00, 0x00, 0x00}},
    {48, "CREDITS", {0x75, 0xa7, 0x43, 0xff}},
    {26, "\n", {0x00, 0x00, 0x00, 0x00}},
    {36, "DEVELOPER", {0xd8, 0x7e, 0x47, 0xff}},
    {18, "\n", {0xc6, 0x51, 0x97, 0xff}},
    {18, "BRUMAZZI DB", {0xc6, 0x51, 0x97, 0xff}},
    {18, "\n", {0xc6, 0x51, 0x97, 0xff}},
    {18, "\n", {0xc6, 0x51, 0x97, 0xff}},
    {36, "LEVEL DESIGNER", {0xd8, 0x7e, 0x47, 0xff}},
    {18, "\n", {0xc6, 0x51, 0x97, 0xff}},
    {18, "BRUMAZZI DB", {0xc6, 0x51, 0x97, 0xff}},
    {18, "SOFISOYA", {0xc6, 0x51, 0x97, 0xff}},
    {18, "\n", {0xc6, 0x51, 0x97, 0xff}},
    {18, "\n", {0xc6, 0x51, 0x97, 0xff}},
    {36, "GAME DESINER", {0xd8, 0x7e, 0x47, 0xff}},
    {18, "\n", {0xc6, 0x51, 0x97, 0xff}},
    {18, "BRUMAZZI DB", {0xc6, 0x51, 0x97, 0xff}},
    {18, "KENNEY", {0xc6, 0x51, 0x97, 0xff}},
    {18, "SOFISOYA", {0xc6, 0x51, 0x97, 0xff}},
    {18, "\n", {0xc6, 0x51, 0x97, 0xff}},
    {18, "\n", {0xc6, 0x51, 0x97, 0xff}},
    {36, "MUSIC DESIGNER", {0xd8, 0x7e, 0x47, 0xff}},
    {18, "\n", {0xc6, 0x51, 0x97, 0xff}},
    {18, "BRUMAZZI DB", {0xc6, 0x51, 0x97, 0xff}},
    {18, "\n", {0xc6, 0x51, 0x97, 0xff}},
    {18, "\n", {0xc6, 0x51, 0x97, 0xff}},
    {0, 0, {0x00, 0x00, 0x00, 0x00}}
};

int main(int argc, char **argv){
    setInitialized(); // TODO: End Game if steam not initialized

    initTexts();

    InitWindow(1280,720, "Monochrome Painter");
    SetTargetFPS(60);

    if(!isInitialized()){
        while(!WindowShouldClose()){
            BeginDrawing();
            DrawText("STEAM not initialized!", 1280/2-(MeasureText("STEAM not initialized!", 64)/2), 720/2-32, 64, RED);
            EndDrawing();
        }

        CloseWindow();
        return 0;
    }

    bool saveFile = isGameSaved();
    float alpha = 0.0;
    short option = 0;

    DIR *dir = opendir("./levels/");

    if(!dir){
        while(!WindowShouldClose()){
            BeginDrawing();
            DrawText("Verify game files and try again!", 1280/2-(MeasureText("Verify game files and try again!", 64)/2), 720/2-32, 64, RED);
            EndDrawing();
        }
        CloseWindow();
        if(isInitialized()) SteamAPI_Shutdown();

        return 0;
    }

    Game *game = new Game();
    Texture title = LoadTexture("assets/title.png");
    Camera2D camera;
    camera.rotation = 0;
    camera.offset = {0, 0};
    camera.target = {0, 0};

    menu:
    while(true){
        if(IsKeyPressed(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER))){
            ToggleFullscreen();
        }

        camera.zoom = GetScreenWidth()/1280;

        BeginDrawing();{
            BeginMode2D(camera);
            ClearBackground(BLACK);
            DrawTexture(title, 0, 0, WHITE);
            showMenu(option, saveFile);
            EndMode2D();
            if(IsKeyDown(KEY_LEFT_ALT)) DrawText("Alt", 0, 0, 16, RED);
        }EndDrawing();

        if(IsKeyPressed(KEY_UP)){
            option--;
            if(option < 0) option = 3;
        }else if(IsKeyPressed(KEY_DOWN)){
            option = (option+1) % 4;
        }else if(IsKeyPressed(KEY_ESCAPE)){
            if(isInitialized()) SteamAPI_Shutdown();
            return 0;
        }else if(IsKeyPressed(KEY_ENTER) && !IsKeyDown(KEY_LEFT_ALT)){
            if(!saveFile) option++;

            if(option == MENU_EXIT) return 0;
            else if(option == MENU_CREDITS){
                while(true){
                    BeginDrawing();{
                        ClearBackground(BLACK);
                        unsigned short i=0;

                        int y = 38;
                        while(CREDITS[i].fontSize){
                            int x;
                            if(CREDITS[i].fontSize == 18){
                                x = GetScreenWidth()/2 - 200;
                            }else{
                                x = MeasureText(CREDITS[i].text, CREDITS[i].fontSize);
                                x = (GetScreenWidth()/2) - (x/2);
                            }
                            DrawText(CREDITS[i].text, x, y, CREDITS[i].fontSize, CREDITS[i].color);
                            y += CREDITS[i].fontSize;
                            i++;
                        }
                    }EndDrawing();

                    if(IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)){
                        option--;
                        break;
                    }

                }
            }else if(option == MENU_RESET){
                resetStats();
            }else if(option == MENU_CONTINUE){
                gameLoad(game);
                break;
            }else if(option == MENU_NEW_GAME){
                break;
            }
        }
    }

    loadAsset("Static", "DoorClose", "assets/door_close.png");
    loadAsset("Static", "DoorOpen", "assets/door_open.png");
    loadAsset("Static", "ItemPaint", "assets/paint_bucket.png");

    loadAsset("Animated", "PlayerIddle", "assets/player_iddle.png");
    loadAsset("Animated", "PlayerWalk", "assets/player_walk.png");
    loadAsset("Animated", "PlayerJump", "assets/player_jump.png");
    loadAsset("Animated", "MonsterWalk", "assets/monster_walk.png");
    loadAsset("Animated", "MonsterWalkNodamage", "assets/monster_walk.png");
    loadAsset("Animated", "MonsterWalkStop", "assets/monster_walk.png");
    loadAsset("Animated", "MonsterFly", "assets/monster_fly.png");
    loadAsset("Animated", "MonsterFlyStop", "assets/monster_fly.png");
    loadAsset("Animated", "Spider", "assets/spider.png");
    loadAsset("Animated", "MonsterFlyDamage", "assets/monster_fly_damage.png");
    loadAsset("Animated", "MonsterFlyDamageStop", "assets/monster_fly_damage.png");
    loadAsset("Animated", "MonsterWalkDamage", "assets/monster_walk_damage.png");
    loadAsset("Animated", "MonsterWalkDamageStop", "assets/monster_walk_damage.png");
    loadAsset("Animated", "MonsterBallon", "assets/monster_ballon.png");


    loadAsset("tileset", "default", "assets/map_ffffff.png");
    loadAsset("tileset", "MAP_GREEN", "assets/map_75a743.png");
    loadAsset("tileset", "MAP_RED", "assets/map_c34848.png");
    loadAsset("tileset", "MAP_BLUE", "assets/map_3c5e8b.png");
    loadAsset("tileset", "MAP_YELLO", "assets/map_eac550.png");
    loadAsset("tileset", "MAP_BROWN", "assets/map_a26342.png");
    loadAsset("tileset", "MAP_PURPLE", "assets/map_aa68de.png");
    loadAsset("tileset", "MAP_CYAN", "assets/map_5aa9de.png");
    loadAsset("tileset", "MAP_GRAY", "assets/map_7f98a5.png");
    loadAsset("tileset", "MAP_PINK", "assets/map_c65197.png");
    loadAsset("tileset", "MAP_ORANGE", "assets/map_d87e47.png");

    auto life = loadAsset("Static", "Life", "assets/life.png");

    Stage *stage = new Stage(game);

    struct dirent *de;

    while((de = readdir(dir))){
        if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) continue;

        string levelPath = "levels/";
        levelPath.append(de->d_name);
        game->levelList.push_back(levelPath);
    }

    std::sort(game->levelList.begin(), game->levelList.end());

    // game->mapIndex = 0;
    stage->loadStage(game->levelList[game->levelIndex], getAsset("tileset", game->mapList[game->mapIndex]));

    char score[18];

    while(!WindowShouldClose()){
        if(IsKeyPressed(KEY_F11) || (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER))){
            ToggleFullscreen();
        }
        camera.zoom = game->getCamera().zoom*0.5;
        if(!game->isGameOver()) game->update();

        sprintf(score, "Score: %ld", game->getScore());
        BeginDrawing();{
            ClearBackground(BLACK);

            game->draw();

            BeginMode2D(camera);{
                for(int x=0; x<game->getLife(); x++){
                    DrawTexture(life, 16*(x+1), 32, WHITE);
                }
                DrawText(score, 16, 16, 16, WHITE);
            }EndMode2D();
        }EndDrawing();

        if(game->isGameOver()){
            // DrawRectangle
        }
    }

    delete game;

    CloseWindow();
    if(isInitialized()) SteamAPI_Shutdown();

    return 0;
}