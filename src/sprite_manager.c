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
    spriteContainer.sprites[SPRITE_IMPORTANT_HAT_ID] = LoadTexture("assets/sprites/important-hat.png");
    spriteContainer.sprites[SPRITE_IMPORTANT_HATL_ID] = LoadTexture("assets/sprites/important-hatL.png");
    spriteContainer.sprites[SPRITE_CABLE_LENGTH_ID] = LoadTexture("assets/sprites/cable-length.png");
}

size_t getMaxSprites() {return N_SPRITES; }

void unloadSprites() {
    UnloadTexture(spriteContainer.sprites[0]);
    UnloadTexture(spriteContainer.sprites[SPRITE_ANCHOR_ID]);
    UnloadTexture(spriteContainer.sprites[SPRITE_IMPORTANT_HAT_ID]);
    UnloadTexture(spriteContainer.sprites[SPRITE_IMPORTANT_HATL_ID]);
    UnloadTexture(spriteContainer.sprites[SPRITE_CABLE_LENGTH_ID]);
}

Texture2D getSpriteFromID(size_t SpriteID) {
    return spriteContainer.sprites[SpriteID];
}
