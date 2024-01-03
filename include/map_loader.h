#ifndef MAP_LOADER_H
#define MAP_LOADER_H
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "physics.h"
#include "entity.h"
#include "cable.h"

void loadMap(char *filename , TileMap *tiles, GameColliderList *colliders, Cable *cable, Player *player);

#endif // MAP_LOADER_H
