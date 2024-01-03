#include "entity.h"
#include "physics.h"
#include "raylib.h"

#define GAME_COLLIDER_LIST_CAPACITY 1e3*sizeof(GameCollider)

GameColliderList createGameColliderList() {
    GameColliderList c_list;
    c_list.size = 0;
    c_list.capacity = GAME_COLLIDER_LIST_CAPACITY;
    c_list.colliders = malloc(c_list.capacity);
    return c_list;
}

void addGameColliderToList(GameColliderList* c_list, GameCollider* e) {
    c_list->colliders[sizeof(GameCollider)*c_list->size] = *e;
    c_list->size += 1;
}

GameCollider* getGameColliderFromList(GameColliderList* c_list, size_t index) {
    return &c_list->colliders[sizeof(GameCollider) * index];
}

void clearGameColliderList(GameColliderList* c_list) { c_list->size = 0; }

void destroyGameColliderList(GameColliderList* c_list) {
    free(c_list->colliders);
}

Player createPlayer() {
    Player player = {0};
    player.sprite = LoadTexture("assets/sprites/Tree001.png");

    player.mass = 1.0f;

    const float playerWidth = 32;
    const float playerHeight = 32;
    const float capsuleRadius = 10;
    const float padding = 10;
    Vector2 x1 = {playerWidth/2, capsuleRadius + padding};
    Vector2 x2 = {playerWidth/2, playerHeight - capsuleRadius};
    player.base_capsule_collider = (CapsuleCollider){x1,x2, capsuleRadius};
    return player;
}

void destroyPlayer(Player* player) {
    UnloadTexture(player->sprite);
}
