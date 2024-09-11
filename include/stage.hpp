#ifndef __STAGE_HPP__
#define __STAGE_HPP__

#include <raylib.h>
#include <vector>
#include <string>
#include <map>

#include <object.hpp>
#include <monster.hpp>
#include <player.hpp>
#include <item.hpp>

using std::map;
using std::string;
using std::vector;

class Game;

typedef struct __block_info__{
    unsigned int id;
    int x,y;
    bool solid;
}BlockInfo;

class Stage{
    public:
    Stage(Game *game);
    ~Stage();
    void draw();
    void update();

    bool loadStage(string path, Texture tex);
    void setTileSize(unsigned short tileSize);
    void setMapWidth(unsigned int width);
    void setMapHeight(unsigned int height);
    void setRespawn(Object *object);
    void addBlock(string layer, BlockInfo block);
    void getKey();
    void finishLevel();

    unsigned short getTileSize();
    unsigned int getMapWidth();
    unsigned int getMapHeight();
    Texture getTexture();
    vector<BlockInfo> getBlocks(string layer);
    vector<Object *> getObjects(string layer);
    Game *getGame();
    bool isLoaded();
    bool isCompleted();
    bool hasKey();
    bool tryRespawn();

    void updateTexture();

    private:
    unsigned short tileSize;
    unsigned int mapWidth;
    unsigned int mapHeight;
    unsigned int textureWidth;
    unsigned int textureHeight;
    bool _isLoaded;
    bool _hasKey;
    bool _isCompleted;

    Vector2 respawn;
    Texture texture;
    map<string, vector<BlockInfo>> blocks;
    map<string, vector<Object *>> objects;
    Object *player;
    Game *game;
};

#endif