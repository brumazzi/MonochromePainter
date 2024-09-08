#ifndef ASSETS_HPP
#define ASSETS_HPP

#include <map>
#include <string>
#include <raylib.h>

using std::map;
using std::string;

#define Grid(x, y) (Vector2) {x*32, y*32}

enum{
    ASSET_FRONT      = 0,
    ASSET_CHARACTER  = 1,
    ASSET_GROUND     = 2,
    ASSET_BEHINDE    = 3,
    ASSET_BACKGROUND = 4,
    ASSET_SKY        = 5
}AssetsLevel;

Texture2D getAsset(string group, string name);
Texture2D loadAsset(string group, string name, const char *path);
Sound getAudioAsset(string group, string name);
Sound loadAudioAsset(string group, string name, const char *path);

#endif