#ifndef __MENU_HPP__
#define __MENU_HPP__

#include <raylib.h>

#define MENU_CONTINUE 0
#define MENU_NEW_GAME 1
#define MENU_CREDITS  2
#define MENU_HELP     3
#define MENU_EXIT     4
#define MENU_RESET    5

void showMenu(short option, bool hasSaveFile);
void showMenuGameover(short option);

#endif