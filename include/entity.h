#ifndef ENTITY_H_
#define ENTITY_H_

#include "raylib.h"
#include "physics.h"

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
    CapsuleCollider capsule_collider;

} Entity;


struct EntityList{
    Entity* entities;
    size_t size;
    size_t capacity;
};

EntityList createEntityList();

void addEntityToList(EntityList* e_list, Entity* e);

Entity* getEntityFromList(EntityList* e_list, size_t index);

void clearEntityList(EntityList* e_list);

void destroyEntityList(EntityList* e_list);

#endif // ENTITY_H_
