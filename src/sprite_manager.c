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
    spriteContainer.sprites[SPRITE_TOOL_ID] = LoadTexture("assets/sprites/tool.png");

    spriteContainer.sprites[SPRITE_KITCHEN_TILE_1_ID] = LoadTexture("assets/sprites/BALDOSA_FLORES_1.png");
    spriteContainer.sprites[SPRITE_KITCHEN_TILE_2_ID] = LoadTexture("assets/sprites/BALDOSA_FLORES_2.png");
    spriteContainer.sprites[SPRITE_KITCHEN_TILE_3_ID] = LoadTexture("assets/sprites/BALDOSA_FLORES_3.png");
    spriteContainer.sprites[SPRITE_KITCHEN_TILE_4_ID] = LoadTexture("assets/sprites/BALDOSA_FLORES_4.png");
}

size_t getMaxSprites() {return N_SPRITES; }

void unloadSprites() {
    UnloadTexture(spriteContainer.sprites[0]);
    UnloadTexture(spriteContainer.sprites[SPRITE_ANCHOR_ID]);
    UnloadTexture(spriteContainer.sprites[SPRITE_IMPORTANT_HAT_ID]);
    UnloadTexture(spriteContainer.sprites[SPRITE_IMPORTANT_HATL_ID]);
    UnloadTexture(spriteContainer.sprites[SPRITE_CABLE_LENGTH_ID]);
    UnloadTexture(spriteContainer.sprites[SPRITE_TOOL_ID]);

    UnloadTexture(spriteContainer.sprites[SPRITE_KITCHEN_TILE_1_ID]);
    UnloadTexture(spriteContainer.sprites[SPRITE_KITCHEN_TILE_2_ID]);
    UnloadTexture(spriteContainer.sprites[SPRITE_KITCHEN_TILE_3_ID]);
    UnloadTexture(spriteContainer.sprites[SPRITE_KITCHEN_TILE_4_ID]);
}

Texture2D getSpriteFromID(size_t SpriteID) {
    return spriteContainer.sprites[SpriteID];
}
