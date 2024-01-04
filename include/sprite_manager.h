#ifndef SPRITE_MANAGER_H_
#define SPRITE_MANAGER_H_

#include "raylib.h"
#include <stddef.h>

#define SPRITE_ANCHOR_ID 66
#define SPRITE_IMPORTANT_HAT_ID 52
#define SPRITE_IMPORTANT_HATL_ID 53
#define SPRITE_CABLE_LENGTH_ID 51
#define SPRITE_KITCHEN_TILE_1_ID 1
#define SPRITE_KITCHEN_TILE_2_ID 2
#define SPRITE_KITCHEN_TILE_3_ID 3
#define SPRITE_KITCHEN_TILE_4_ID 4
#define SPRITE_TOOL_ID 67

#define SOUND_TRACK_STAPLE_ID 0
#define SOUND_TRACK_STAPLE_REMOVE_ID 1
#define SOUND_TRACK_INTRO_EFFECT_ID 2
#define SOUND_TRACK_JUMP_EFFECT_ID 3

void initializeAssets();

void destroyAssets();

Texture2D getSpriteFromID(size_t SpriteID);
Sound getSoundTrackFromID(size_t MusicID);

size_t getMaxSprites();
size_t getMaxMusicTracks();

#endif // SPRITE_MANAGER_H_
