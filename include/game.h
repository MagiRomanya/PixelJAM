#ifndef GAME_H_
#define GAME_H_

#include "ultilities.h"
#include <stdbool.h>


typedef enum {
IMPORTANT_HAT,
SAFETY_HAT,
}HAT_TYPE;

void SetPlayerHat(HAT_TYPE hat);

HAT_TYPE GetPlayerHat();

void SetNextPlayerHat();

SCREEN runLevel(char* map_filename, float maxCableLength, int maxAnchors, SCREEN nextScreen);

#endif // GAME_H_
