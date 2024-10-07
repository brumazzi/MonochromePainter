#include <steam.hpp>
#include <game.hpp>
#include <stdio.h>

#define SAVE_FILE_NAME "monochrome_painter.sav"

static bool initialized = false;

bool isInitialized(){
    return initialized;
}

void setInitialized(){
    initialized = SteamAPI_Init();
}

bool unlockAchieviment(const char *achievimentID){
    if(!initialized) return false;

    auto userStats = SteamUserStats();
    if(userStats && userStats->RequestCurrentStats() ){
        bool unlock = false;

        userStats->GetAchievement(achievimentID, &unlock);

        if(!unlock){
            userStats->SetAchievement(achievimentID);
            userStats->StoreStats();
            return true;
        }
    }

    return false;
}

void resetStats(){
    if(!initialized) return;

    auto userStats = SteamUserStats();
    if(userStats && userStats->RequestCurrentStats()){
        userStats->ResetAllStats(true);
    }
}

bool isGameSaved(){
    return initialized && SteamRemoteStorage()->FileExists(SAVE_FILE_NAME);
}

bool saveGame(Game *game){
    if(!initialized) return false;
    char saveData[MAX_SAVED_DATA_SIZE];
    // bzero function do not exist in widows
    // this line created to run in linux and windows
    for(int x=0; x< MAX_SAVED_DATA_SIZE; x++) saveData[x] == 0;
    // bzero(saveData, MAX_SAVED_DATA_SIZE);

    sprintf(saveData, "%d %ld %d %d %d", game->levelIndex+1, game->getScore(), game->getLife(), game->mapIndex, game->lifeCollected);

    return SteamRemoteStorage()->FileWrite(SAVE_FILE_NAME, (const void*) saveData, MAX_SAVED_DATA_SIZE);
}

bool gameLoad(Game *game){
    if(!initialized) return false;
    char savedData[MAX_SAVED_DATA_SIZE];

    if(isGameSaved()){
        SteamRemoteStorage()->FileRead(SAVE_FILE_NAME, (void *) savedData, MAX_SAVED_DATA_SIZE);
        int score, lifes;
        sscanf(savedData, "%d %d %d %d %hd", &game->levelIndex, &score, &lifes, &game->mapIndex, &game->lifeCollected);
        game->setScore(score);
        game->setLife(lifes);
    }

    return false;
}