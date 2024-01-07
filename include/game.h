#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>


typedef enum {
IMPORTANT_HAT,
SAFETY_HAT,
CHEF_HAT,
KING_HAT,
POINTY_HAT,
TOP_HAT,
}HAT_TYPE;

void SetPlayerHat(HAT_TYPE hat);

HAT_TYPE GetPlayerHat();

void SetNextPlayerHat();

typedef enum {
LEVEL_1,
LEVEL_2,
LEVEL_3,
LEVEL_4,
MENU_SCREEN,
TITLE_SCREEN,
QUIT_GAME,
CONTROLS_SCREEN,
CREDITS_SCREEN,
} SCREEN;

SCREEN showTitleScreen();

SCREEN showMenuScreen();

SCREEN showCreditsScreen();

SCREEN showControlsScreen();

SCREEN runLevel(char* map_filename, float maxCableLength, int maxAnchors, SCREEN nextScreen);

#endif // GAME_H_
