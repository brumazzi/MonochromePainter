#include <jsoncpp/json/json.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

typedef struct {
    float x;
    float y;
}Vector2;

int main(int argc, char **argv){
    std::ifstream jsonFile(argv[argc-1], std::ifstream::binary);

    Json::Value root;
    Json::CharReaderBuilder builder;
    std::string errors;

    if(!Json::parseFromStream(builder, jsonFile, &root, &errors)){
        return false;
    }

    Vector2 min = {9999999, 9999999};
    Vector2 max = {0,0};

    for(const auto layer: root["layers"]){
        for(const auto tile: layer["tiles"]){
            if(min.x > tile["x"].asInt()) min.x = tile["x"].asInt();
            if(min.y > tile["y"].asInt()) min.y = tile["y"].asInt();
            if(max.x < tile["x"].asInt()) max.x = tile["x"].asInt();
            if(max.y < tile["y"].asInt()) max.y = tile["y"].asInt();
        }
    }

    Json::Value newRoot;
    Json::Value jsonLayers(Json::arrayValue);

    max.x -= min.x;
    max.y -= min.y;

    newRoot["mapWidth"] = max.x/root["tileSize"].asInt();
    newRoot["mapHeight"] = max.y/root["tileSize"].asInt();
    newRoot["name"] = root["name"].asString();
    newRoot["tileSize"] = root["tileSize"].asInt();


    for(const auto layer: root["layers"]){
        auto layerName = layer["name"].asString();

        Json::Value jsonLayer, jsonTiles(Json::arrayValue);
        jsonLayer["name"] = layerName;
        jsonLayer["collider"] = layer["collider"].asBool();

        for(const auto tile: layer["tiles"]){
            Json::Value jsonTile;
            jsonTile["id"] = stoi((std::string) tile["id"].asString());
            jsonTile["x"] = (tile["x"].asInt() - min.x)/root["tileSize"].asInt();
            jsonTile["y"] = (tile["y"].asInt() - min.y)/root["tileSize"].asInt();
            jsonTiles.append(jsonTile);
        }
        jsonLayer["tiles"] = jsonTiles;
        jsonLayers.append(jsonLayer);
    }
    newRoot["layers"] = jsonLayers;

    std::string fileName = "levels/"+root["name"].asString()+".json";
    std::transform(fileName.begin(), fileName.end(), fileName.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    std::ofstream output(fileName);
    output << newRoot;

    return 0;
}