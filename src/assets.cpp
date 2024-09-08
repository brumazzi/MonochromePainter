#include <assets.h>
#include <raylib.h>

void __attribute__((constructor)) initialize() {
}

map<string, map<string, Texture>> assets;
map<string, map<string, Sound>> assetsAudio;

Texture getAsset(string group, string name){
    return assets[group][name];
}

Sound getAudioAsset(string group, string name){
    return assetsAudio[group][name];
}

Texture2D loadAsset(string group, string name, const char *path){
    if(assets.find(group) == assets.end() || assets[group].find(name) == assets[group].end())
        assets[group][name] = LoadTexture(path);
    return assets[group][name];
}

Sound loadAudioAsset(string group, string name, const char *path){
    if(assetsAudio.find(group) == assetsAudio.end() || assetsAudio[group].find(name) == assetsAudio[group].end())
        assetsAudio[group][name] = LoadSound(path);
    return assetsAudio[group][name];
}

void freeAsset(string group, string name){
    if(assets.find(group) != assets.end() && assets[group].find(name) != assets[group].end()){
        UnloadTexture(getAsset(group, name));
        assets[group].erase(name);
    }
}

// void freeAudioAsset(string group, string name){
//     if(assetsAudio.find(group) != assetsAudio.end() && assetsAudio[group].find(name) != assetsAudio[group].end()){
//         UnloadSound(assetsAudio[group][name]);
//         assetsAudio[group].erase(name);
//     }
// }

// void __attribute__((destructor)) finalize() {
//     for(auto [key, value]: assets){
//         for(auto [assetKey, assetValue]: value){
//             UnloadTexture(assetValue);
//         }
//     }

//     for(auto [key, value]: assetsAudio){
//         for(auto [assetKey, assetValue]: value){
//             UnloadSound(assetValue);
//         }
//     }
// }