#include "sprite_manager.h"

#include "raylib.h"
#include <stddef.h>

#define N_SPRITES 100
#define N_MUSIC_TRACKS 100

typedef struct {
    Texture2D sprites[N_SPRITES];
    Sound tracks[N_MUSIC_TRACKS];
} AssetsContainer;

static AssetsContainer assetsContainer;

void initializeAssets() {
    assetsContainer.sprites[SPRITE_ANCHOR_ID] = LoadTexture("assets/sprites/anchor.png");
    assetsContainer.sprites[SPRITE_IMPORTANT_HAT_ID] = LoadTexture("assets/sprites/important-hat.png");
    assetsContainer.sprites[SPRITE_IMPORTANT_HATL_ID] = LoadTexture("assets/sprites/important-hatL.png");
    assetsContainer.sprites[SPRITE_CABLE_LENGTH_ID] = LoadTexture("assets/sprites/cable-length.png");
    assetsContainer.sprites[SPRITE_TOOL_ID] = LoadTexture("assets/sprites/tool.png");

    // Tiles
    assetsContainer.sprites[0] = LoadTexture("assets/sprites/tile-test.png");
    assetsContainer.sprites[SPRITE_KITCHEN_TILE_1_ID] = LoadTexture("assets/sprites/BALDOSA_FLORES_1.png");
    assetsContainer.sprites[SPRITE_KITCHEN_TILE_2_ID] = LoadTexture("assets/sprites/BALDOSA_FLORES_2.png");
    assetsContainer.sprites[SPRITE_KITCHEN_TILE_3_ID] = LoadTexture("assets/sprites/BALDOSA_FLORES_3.png");
    assetsContainer.sprites[SPRITE_KITCHEN_TILE_4_ID] = LoadTexture("assets/sprites/BALDOSA_FLORES_4.png");

    // Music
    assetsContainer.tracks[SOUND_TRACK_STAPLE_ID] = LoadSound("assets/sound/staple.mp3");
    assetsContainer.tracks[SOUND_TRACK_INTRO_EFFECT_ID] = LoadSound("assets/sound/intro-effect.mp3");
}

void destroyAssets() {
    UnloadTexture(assetsContainer.sprites[0]);
    UnloadTexture(assetsContainer.sprites[SPRITE_ANCHOR_ID]);
    UnloadTexture(assetsContainer.sprites[SPRITE_IMPORTANT_HAT_ID]);
    UnloadTexture(assetsContainer.sprites[SPRITE_IMPORTANT_HATL_ID]);
    UnloadTexture(assetsContainer.sprites[SPRITE_CABLE_LENGTH_ID]);
    UnloadTexture(assetsContainer.sprites[SPRITE_TOOL_ID]);

    UnloadTexture(assetsContainer.sprites[SPRITE_KITCHEN_TILE_1_ID]);
    UnloadTexture(assetsContainer.sprites[SPRITE_KITCHEN_TILE_2_ID]);
    UnloadTexture(assetsContainer.sprites[SPRITE_KITCHEN_TILE_3_ID]);
    UnloadTexture(assetsContainer.sprites[SPRITE_KITCHEN_TILE_4_ID]);

    // Sounds and music
    UnloadSound(assetsContainer.tracks[SOUND_TRACK_STAPLE_ID]);
    UnloadSound(assetsContainer.tracks[SOUND_TRACK_INTRO_EFFECT_ID]);
}

Texture2D getSpriteFromID(size_t SpriteID) {
    return assetsContainer.sprites[SpriteID];
}

Sound getSoundTrackFromID(size_t MusicID) {
    return assetsContainer.tracks[MusicID];
}

size_t getMaxSprites() { return N_SPRITES; }
size_t getMaxMusicTracks() { return N_MUSIC_TRACKS; }
