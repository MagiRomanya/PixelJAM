#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>


typedef enum {
IMPORTANT_HAT,
SAFETY_HAT,
}HAT_TYPE;

void SetPlayerHat(HAT_TYPE hat);

HAT_TYPE GetPlayerHat();

void SetNextPlayerHat();

bool runLevel(char* map_filename, float maxCableLength, int maxAnchors);

#endif // GAME_H_
