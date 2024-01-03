#ifndef SPRITE_MANAGER_H_
#define SPRITE_MANAGER_H_

#include "raylib.h"
#include <stddef.h>

void initializeSprites();

void unloadSprites();

Texture2D getSpriteFromID(size_t SpriteID);

size_t getMaxSprites();

#endif // SPRITE_MANAGER_H_
