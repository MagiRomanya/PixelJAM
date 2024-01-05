#ifndef SPRITE_MANAGER_H_
#define SPRITE_MANAGER_H_

#include "raylib.h"
#include <stddef.h>

#define SPRITE_KITCHEN_TILE_1_ID 1
#define SPRITE_KITCHEN_TILE_2_ID 2
#define SPRITE_KITCHEN_TILE_3_ID 3
#define SPRITE_KITCHEN_TILE_4_ID 4
#define SPRITE_CABLE_LENGTH_ID 51
#define SPRITE_IMPORTANT_HAT_ID 52
#define SPRITE_IMPORTANT_HATL_ID 53
#define SPRITE_TELEVISION_ON_ID 54
#define SPRITE_TELEVISION_OFF_ID 55
#define SPRITE_BLENDER_OFF_ID 56
#define SPRITE_BLENDER_STAGE1_ID 57
#define SPRITE_BLENDER_STAGE2_ID 58
#define SPRITE_WASHING_MACHINE_OFF_ID 59
#define SPRITE_WASHING_MACHINE_ON_ID 60
#define SPRITE_ANCHOR_ID 61
#define SPRITE_TOOL_ID 62
#define SPRITE_SAFETY_HAT_ID 63
#define SPRITE_SAFETY_HATL_ID 64

#define SOUND_TRACK_STAPLE_ID 0
#define SOUND_TRACK_STAPLE_REMOVE_ID 1
#define SOUND_TRACK_INTRO_EFFECT_ID 2
#define SOUND_TRACK_JUMP_EFFECT_ID 3
#define SOUND_TRACK_PTERODACTYL_ID 4
#define SOUND_TRACK_ERROR_EFFECT_ID 5
#define SOUND_TRACK_WASHING_MACHINE_ID 6

void initializeAssets();

void destroyAssets();

Texture2D getSpriteFromID(size_t SpriteID);
Sound getSoundTrackFromID(size_t MusicID);

size_t getMaxSprites();
size_t getMaxMusicTracks();

#endif // SPRITE_MANAGER_H_
