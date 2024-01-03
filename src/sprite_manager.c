#include "sprite_manager.h"

#include "raylib.h"
#include <stddef.h>

#define N_SPRITES 100

typedef struct {
    Texture2D sprites[N_SPRITES];
} SpriteContainer;

static SpriteContainer spriteContainer;

void initializeSpriteContainer() {
    spriteContainer.sprites[0] = LoadTexture("assets/sprites/Tree001.png");
}

void unloadSprites() {
    UnloadTexture(spriteContainer.sprites[0]);
}

Texture2D getSpriteFromID(size_t SpriteID) {
    return spriteContainer.sprites[SpriteID];
}
