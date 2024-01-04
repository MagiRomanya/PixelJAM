#include "sprite_manager.h"

#include "raylib.h"
#include <stddef.h>

#define N_SPRITES 100

typedef struct {
    Texture2D sprites[N_SPRITES];
} SpriteContainer;

static SpriteContainer spriteContainer;

void initializeSprites() {
    spriteContainer.sprites[0] = LoadTexture("assets/sprites/tile-test.png");
    spriteContainer.sprites[SPRITE_ANCHOR_ID] = LoadTexture("assets/sprites/anchor.png");
}

size_t getMaxSprites() {return N_SPRITES; }

void unloadSprites() {
    UnloadTexture(spriteContainer.sprites[0]);
}

Texture2D getSpriteFromID(size_t SpriteID) {
    return spriteContainer.sprites[SpriteID];
}
