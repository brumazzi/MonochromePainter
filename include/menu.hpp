#ifndef __MENU_HPP__
#define __MENU_HPP__

#include <raylib.h>

#define MENU_CONTINUE 0
#define MENU_NEW_GAME 1
#define MENU_CREDITS  2
#define MENU_EXIT     3
#define MENU_RESET    4

void showMenu(short option, bool hasSaveFile);

#endif