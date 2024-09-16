#include <game.hpp>
#include <iostream>
#include <assets.h>

const static unsigned char COLOR_LIST[][4] = {
    {0x75, 0xa7, 0x43, 0xff},
    {0xc3, 0x48, 0x48, 0xff},
    {0x3c, 0x5e, 0x8b, 0xff},
    {0xea, 0xc5, 0x50, 0xff},
    {0xa2, 0x63, 0x42, 0xff},
};

Game::Game(){
    this->camera.offset = {0,0};
    this->camera.target = {0,0};
    this->camera.rotation = 0;
    this->camera.zoom = GetScreenWidth()/CAMERA_WIDTH;
    this->score = 0;
    this->life = 10;
    this->_gameOver = false;
    this->levelIndex = 0;
    this->toDark = false;
    this->isNextLevel = false;
    this->displayAlpha = 0;
    this->mapIndex = 0;
    this->mapList.push_back("default");
    this->mapList.push_back("MAP_GREEN");
    this->mapList.push_back("MAP_RED");
    this->mapList.push_back("MAP_BLUE");
    this->mapList.push_back("MAP_YELLO");
    this->mapList.push_back("MAP_BROWN");
}
Game::~Game(){
    delete this->stage;
}

void Game::draw(){
    BeginMode2D(this->camera);{
        if(this->stage) this->stage->draw();
    }EndMode2D();
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color) {0,0,0, (unsigned char) this->displayAlpha});
}

void Game::update(){
    if(this->mapIndex >= this->mapList.size()) this->mapIndex = (this->mapList.size())-1;
    // if(!this->stage){
    //     this->stage = new Stage(this);
    //     this->stage->loadStage(this->levelList[this->levelIndex], getAsset("tileset", "default"));
    // }

    if(this->toDark){
        if(this->displayAlpha == 255){
            this->toDark = false;
            if(this->isNextLevel){
                if(this->levelList.size() > (this->levelIndex+1)){
                    this->levelIndex++;
                    delete this->stage;
                    this->stage = new Stage(this);
                    this->stage->loadStage(this->levelList[this->levelIndex], getAsset("tileset", this->mapList[this->mapIndex]));
                }
                else {
                    // TODO: create goToLevel Function
                    // game.goToLevel("");
                }
            }
        }
        this->displayAlpha += 5;
        if(this->displayAlpha > 255) this->displayAlpha = 255;
        // return;
    }else if(this->displayAlpha != 0){
        Vector2 position = this->getPlayer()->getPosition();
        this->camera.target = (Vector2) {position.x - CAMERA_WIDTH/2, position.y - CAMERA_HEIGHT/2};

        displayAlpha -= 5;
        if(this->displayAlpha < 0) this->displayAlpha = 0;
        // return;
    }

    Player *player = this->getPlayer();
    if(!player) return;

    if(this->stage) this->stage->update();

    Vector2 target = {0,0};
    Vector2 targetPosition = this->getPlayer()->getPosition();
    int mapWidth = (this->stage->getMapWidth())-(2*this->getStage()->getTileSize());
    int mapHeight = (this->stage->getMapHeight())-this->getStage()->getTileSize();

    if(IsWindowResized()){
        this->camera.zoom = GetScreenWidth()/CAMERA_WIDTH;
    }

    int distance_x = this->camera.target.x - targetPosition.x + CAMERA_WIDTH/2;
    if(abs(distance_x) > CAMERA_WIDTH/10){
        this->camera.target.x -= distance_x*0.02;
        if(this->camera.target.x < 0) this->camera.target.x = 0;
    }

    int distance_y = this->camera.target.y - targetPosition.y + CAMERA_HEIGHT/2;
    if(abs(distance_y) > CAMERA_HEIGHT/10){
        this->camera.target.y -= distance_y*0.02;
        if(this->camera.target.y < 0) this->camera.target.y = 0;
    }

    if(this->camera.target.x + CAMERA_WIDTH >= mapWidth) this->camera.target.x = mapWidth - CAMERA_WIDTH;
    if(this->camera.target.x <= 0) this->camera.target.x = 0;
    if(this->camera.target.y + CAMERA_HEIGHT >= mapHeight) this->camera.target.y = mapHeight - CAMERA_HEIGHT;
    if(this->camera.target.y <= 0) this->camera.target.y = 0;
}

void Game::nextLevel(){
    this->toDark = true;
    this->isNextLevel = true;
}


void Game::setStage(Stage *stage){ this->stage = stage; }

Player *Game::getPlayer(){
    if(this->stage->getObjects("Player").size() == 0) return nullptr;
    return (Player*) this->stage->getObjects("Player")[0];
}
Stage *Game::getStage(){ return this->stage; }
long int Game::getScore(){ return this->score; }
short Game::getLife(){ return this->life; }
Camera2D Game::getCamera(){ return this->camera; }

void Game::addScore(long int point){ this->score += point; }
void Game::addLife(){ this->life++; }
void Game::subLife(){ this->life--; }

bool Game::isGameOver(){ return this->_gameOver; }
void Game::gameOver(){ this->_gameOver = true; }
void Game::gameStart(){ this->_gameOver = false; }
