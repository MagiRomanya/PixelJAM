#include "entity.h"
#include "physics.h"
#include "raylib.h"

#define ENTITY_LIST_CAPACITY 1e3*sizeof(Entity)

EntityList createEntityList() {
    EntityList e_list;
    e_list.size = 0;
    e_list.capacity = ENTITY_LIST_CAPACITY;
    e_list.entities = malloc(e_list.capacity);
    return e_list;
}

void addEntityToList(EntityList* e_list, Entity* e) {
    e_list->entities[sizeof(Entity)*e_list->size] = *e;
    e_list->size += 1;
}

Entity* getEntityFromList(EntityList* e_list, size_t index) {
    return &e_list->entities[sizeof(Entity) * index];
}

void clearEntityList(EntityList* e_list) { e_list->size = 0; }

void destroyEntityList(EntityList* e_list) {
    free(e_list->entities);
}

Player createPlayer() {
    Player player = {0};
    player.sprite = LoadTexture("assets/sprites/Tree001.png");

    player.mass = 1.0f;

    const float playerWidth = 32;
    const float playerHeight = 32;
    const float capsuleRadius = 10;
    Vector2 x1 = {playerWidth/2, capsuleRadius};
    Vector2 x2 = {playerWidth/2, playerHeight - capsuleRadius};
    player.base_capsule_collider = (CapsuleCollider){x1,x2, capsuleRadius};
    return player;
}

void destroyPlayer(Player* player) {
    UnloadTexture(player->sprite);
}
