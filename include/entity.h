#ifndef ENTITY_H_
#define ENTITY_H_

#include "raylib.h"

typedef enum {
NO_COLLIDE,
PLAYER_COLLIDE,
CABLE_COLLIDE,
PLAYER_CABLE_COLLIDE,
} COLLISION_MASK ;

typedef struct {
    // General Stuff
    Vector2 postion;

    // Render
    Texture2D sprite;
    Rectangle rect_source;
    Rectangle rect_dest;

    // Physics
    COLLISION_MASK collision_mask;

}Entity;

#endif // ENTITY_H_
