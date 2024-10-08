#ifndef __STEAM_HPP__
#define __STEAM_HPP__

#include <steam/steam_api.h>

class Game;

#define STAT_COLLECTED_POINTS "COLLECTED_POINTS"
#define STAT_COLLETED_LIFES "COLLETED_LIFES"
#define STAT_COLLETED_PAINTS "COLLETED_PAINTS"
#define STAT_CURRENT_STAGE "CURRENT_STAGE"

#define MAX_SAVED_DATA_SIZE 128

bool unlockAchieviment(const char *achievimentID);
bool checkAchieviment(const char *achievimentID);
void resetStats();
void setInitialized();
bool isInitialized();

bool saveGame(Game *game);
bool gameLoad(Game *game);
bool isGameSaved();

#endif