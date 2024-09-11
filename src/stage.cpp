#include <stage.hpp>
#include <jsoncpp/json/json.h>

#include <iostream>
#include <fstream>
#include <math.h>
#include <assets.h>
#include <game.hpp>
#include <door.hpp>

#define GRAVITY 9.8
#define IS_DECORATION(layer) (!layer.first.compare("DecorationBack") || !layer.first.compare("DecorationFront"))
#define IS_GROUND(layer) (!layer.first.compare("Ground"))
#define IS_HIDDEN(layer) (layer.first.find("Hidden") != std::string::npos)


Stage::Stage(Game *game){
    this->tileSize = 0;
    this->mapWidth = 0;
    this->mapHeight = 0;
    this->textureWidth = 0;
    this->textureHeight = 0;
    this->_isLoaded = false;
    this->texture.id = 0;
    this->player = nullptr;
    this->game = game;
    this->_hasKey = false;
    game->setStage(this);
    this->player = nullptr;
    this->_isCompleted = false;
}

Stage::~Stage(){
    for(auto map: this->objects){
        for(auto object: map.second){
            while(object->collisionCallbacks.size()) object->collisionCallbacks.pop_back();
            delete object;
        }
        map.second.clear();
    }
    this->objects.clear();
}

bool Stage::loadStage(string path, Texture tex){
    std::ifstream jsonFile(path, std::ifstream::binary);
    Json::Value root;
    Json::CharReaderBuilder builder;
    string errors;

    if(!jsonFile.is_open()){
        return false;
    }


    if(!Json::parseFromStream(builder, jsonFile, &root, &errors)){
        return false;
    }

    this->tileSize = root["tileSize"].asInt();
    this->mapWidth = root["mapWidth"].asInt()*this->tileSize;
    this->mapHeight = root["mapHeight"].asInt()*this->tileSize;
    this->texture = tex;
    this->textureWidth = tex.width/this->tileSize;
    this->textureHeight = tex.height/this->tileSize;

    for(const auto layer: root["layers"]){
        auto layerName = layer["name"].asString();
        for(const auto tile: layer["tiles"]){
            if(!layerName.compare("None") || !layerName.compare("BoxInfo")) continue;
            BlockInfo bi = {
                id: (unsigned int) stoi((string) tile["id"].asString()),
                x: tile["x"].asInt()*16,
                y: tile["y"].asInt()*16,
                solid: layer["collider"].asBool()
            };

            this->blocks[layerName].push_back(bi);
            float offsetX = bi.id%this->textureWidth;
            float offsetY = bi.id/this->textureWidth;

            Object *object;
            if(layerName.find("Monster") != std::string::npos){
                object = new Monster();
                this->objects[string("Monster")].push_back(object);
                object->setAnimated(false);
                object->usePhysic(true, 0);
                if(layerName.find("Spider") == std::string::npos) object->setTexture(getAsset("Animated", layerName), 2);
                else object->setTexture(getAsset("Animated", "Spider"), 1);
            }else if(!layerName.compare(0, 4, "Item")){
                object = new Item();
                this->objects[string("Item")].push_back(object);
                object->setTexture(tex, 1);
                object->setAnimated(false);
                object->setTextureOffset(offsetX, offsetY);
            }else if(!layerName.compare(0, 3, "NPC")){
                object = new NPC();
                object->usePhysic(true, GRAVITY);
                this->objects[string("NPC")].push_back(object);
                object->setTexture(getAsset("Animated", layerName), 1);
                object->setAnimated(true);
            }else if(!layerName.compare("DoorNext")){
                object = new Door();
                object->usePhysic(false, GRAVITY);
                this->objects[string("DoorNext")].push_back(object);
                object->setTexture(getAsset("Static", "DoorClose"), 1);
                object->setAnimated(false);
            }else if(!layerName.compare("Player")){
                object = new Player();
                object->usePhysic(true, GRAVITY);
                this->objects[string("Player")].push_back(object);
                object->setTexture(getAsset("Animated", layerName), 1);
                object->setAnimated(true);
                this->player = object;
                this->game->getCamera().target = (Vector2) {(float) bi.x, (float) bi.y};
                this->respawn = (Vector2) {(float) (bi.x), (float)(bi.y)};
                this->game->getCamera().target = (Vector2) {object->getPosition().x - CAMERA_WIDTH/2, object->getPosition().y - CAMERA_HEIGHT/2};
            }else{
                object = new Object();
                this->objects[layerName].push_back(object);
                object->setTextureOffset(offsetX, offsetY);
                object->setTexture(tex, 1);
                object->setAnimated(false);
                object->setFrameCount(1);

                if(layerName.find("Damage") != std::string::npos){
                    object->setCollisionBoundsOffset({2,6});
                }
            }

            object->setPosition(bi.x, bi.y);
            object->setSize(this->tileSize, this->tileSize);
            object->setSolid(bi.solid);
            object->setGroup(layerName);
            object->setStage(this);

        }
    }
    this->_isLoaded = true;

    jsonFile.close();

    return true;
}

void Stage::draw(){
    for(const auto object: this->objects["DecorationBack"]){
        object->draw();
    }
    for(const auto layer: this->objects){
        if(IS_DECORATION(layer) || IS_HIDDEN(layer)) continue;
        for(const auto object: layer.second){
            object->draw();
        }
    }
    for(const auto object: this->objects["DecorationFront"]){
        object->draw();
    }
}

void Stage::update(){
    // for(const auto object: this->objects["DecorationBack"]){
    //     object->update();
    // }
    map<string, vector<unsigned int>> removeObjectIndex;

    for(const auto layer: this->objects){
        if(IS_DECORATION(layer)) continue;
        unsigned int index = 0;
        for(const auto object: layer.second){
            object->update();

            if(IS_GROUND(layer)) continue;
            if(object->isUsingPhysic()){
                for(const auto layerCompare: this->objects){
                    for(const auto compare: layerCompare.second){
                        if(IS_DECORATION(layerCompare)) continue;
                        if(object != compare){
                            object->testCollision(compare);
                        }
                    }
                }
            }
            if(object->isDestroyed()) removeObjectIndex[layer.first].push_back(index);
            else index++;
        }
    }
    for(auto layer: removeObjectIndex){
        for(auto objectIndex: removeObjectIndex[layer.first]){
            delete this->objects[layer.first][objectIndex];
            this->objects[layer.first].erase(this->objects[layer.first].begin() + objectIndex);
        }
    }

    if(this->player && this->player->getPosition().y > (this->mapHeight)+(10*this->tileSize)) this->tryRespawn();


    // for(const auto object: this->objects["DecorationFront"]){
    //     object->update();
    // }
}

bool Stage::tryRespawn(){
    if(this->getGame()->getLife() > 0){
        this->player->setPosition(this->respawn.x, this->respawn.y);
        this->game->subLife();
    }
    else this->game->gameOver();
    return true;
}

void Stage::setTileSize(unsigned short tileSize){ this->tileSize = tileSize; }
void Stage::setMapWidth(unsigned int width){ this->mapWidth = width; }
void Stage::setMapHeight(unsigned int height){ this->mapHeight = height; }
void Stage::setRespawn(Object *object) { this->respawn = object->getPosition(); }
void Stage::addBlock(string layer, BlockInfo block){ this->blocks[layer].push_back(block); }
void Stage::finishLevel(){ this->_isCompleted = true; }
void Stage::getKey(){ this->_hasKey = true; }

unsigned short Stage::getTileSize(){ return this->tileSize; }
unsigned int Stage::getMapWidth(){ return this->mapWidth; }
unsigned int Stage::getMapHeight(){ return this->mapHeight; }
Texture Stage::getTexture(){ return this->texture; }
vector<BlockInfo> Stage::getBlocks(string layer){ return this->blocks[layer]; }
vector<Object *> Stage::getObjects(string layer){ return this->objects[layer]; }
Game *Stage::getGame(){ return this->game; }
bool Stage::isLoaded(){ return this->_isLoaded; }
bool Stage::isCompleted(){ return this->_isCompleted; }

bool Stage::hasKey(){ return this->_hasKey; }


void Stage::updateTexture(){
    Texture texture = getAsset("tileset", this->game->mapList[this->game->mapIndex]);
    for(auto iter: this->objects){
        for(auto object: iter.second){
            if(object->getTexture().id == this->texture.id){
                object->setTexture(texture, 1);
            }
        }
    }
}
