#ifndef MAP_LOADER_H
#define MAP_LOADER_H
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "physics.h"
#include "entity.h"
#include "appliance.h"
#include "cable.h"

void loadMap(char *filename, TileMap *t_map, GameColliderList *colliders, ApplianceList *a_list, Cable *cable, Player *player);

#endif // MAP_LOADER_H
