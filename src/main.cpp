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

int main(int argc, char **argv){
    initTexts();

    InitWindow(1280,720, "Monocrome Painter");
    SetTargetFPS(60);

    bool saveFile = false;
    float alpha = 0.0;
    short option = 0;

    DIR *dir = opendir("./levels/");

    if(!dir){
        while(!WindowShouldClose()){
            BeginDrawing();
            DrawText("Verify game files and try again!", 1280/2-(MeasureText("Verify game files and try again!", 64)/2), 720/2-32, 64, RED);
            EndDrawing();
        }

        return 0;
    }

    Texture title = LoadTexture("assets/title.png");
    menu:
    while(true){
        BeginDrawing();{
            ClearBackground(BLACK);
            DrawTexture(title, 0, 0, WHITE);
            showMenu(option, saveFile);
        }EndDrawing();

        if(IsKeyPressed(KEY_UP)){
            option--;
            if(option < 0) option = 3;
        }else if(IsKeyPressed(KEY_DOWN)){
            option = (option+1) % 4;
        }else if(IsKeyPressed(KEY_ESCAPE)){
            return 0;
        }else if(IsKeyPressed(KEY_ENTER)) break;

    }

    if(!saveFile) option++;

    if(option == MENU_EXIT) return 0;
    if(option == MENU_CREDITS){
        while(true){
            BeginDrawing();{
                ClearBackground(BLACK);
                DrawText("Credits Not Implemented", 1280/2-MeasureText("Credits Not Implemented", 64)/2, 720/2, 64, WHITE);
            }EndDrawing();

           if(IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)){
                option--;
                goto menu;
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

    loadAsset("tileset", "default", "assets/map.png");
    loadAsset("tileset", "MAP_GREEN", "assets/map_75a743.png");
    loadAsset("tileset", "MAP_RED", "assets/map_c34848.png");
    loadAsset("tileset", "MAP_BLUE", "assets/map_3c5e8b.png");
    loadAsset("tileset", "MAP_YELLO", "assets/map_eac550.png");
    loadAsset("tileset", "MAP_BROWN", "assets/map_a26342.png");
    auto life = loadAsset("Static", "Life", "assets/life.png");

    Game *game = new Game();
    Stage *stage = new Stage(game);

    struct dirent *de;

    while((de = readdir(dir))){
        if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..")) continue;

        string levelPath = "levels/";
        levelPath.append(de->d_name);
        game->levelList.push_back(levelPath);
    }

    std::sort(game->levelList.begin(), game->levelList.end());

    game->mapIndex = 0;
    stage->loadStage(game->levelList[0], getAsset("tileset", game->mapList[game->mapIndex]));

    char score[16];

    Camera2D camera;
    camera.rotation = 0;
    camera.offset = {0, 0};
    camera.target = {0, 0};

    while(!WindowShouldClose()){
        camera.zoom = game->getCamera().zoom*0.5;
        game->update();

        if(!game->isGameOver())
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
    }

    delete game;

    CloseWindow();

    return 0;
}