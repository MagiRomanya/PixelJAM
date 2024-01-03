#ifndef ENTITY_H_
#define ENTITY_H_

#include "raylib.h"
#include "raymath.h"
#include "physics.h"

typedef enum {
NO_COLLIDE,
PLAYER_COLLIDE,
CABLE_COLLIDE,
PLAYER_CABLE_COLLIDE,
} COLLISION_MASK ;

typedef struct {
    COLLISION_MASK collision_mask;
    float friction_damping;
    CapsuleCollider capsule_collider;
} GameCollider;

struct GameColliderList{
    GameCollider* colliders;
    size_t size;
    size_t capacity;
};

GameColliderList createGameColliderList();

void addGameColliderToList(GameColliderList* c_list, GameCollider* c);

GameCollider* getGameColliderFromList(GameColliderList* c_list, size_t index);

void clearGameColliderList(GameColliderList* c_list);

void destroyGameColliderList(GameColliderList* c_list);

struct Player {
    Vector2 position;
    Vector2 velocity;
    Vector2 force;

    // Render
    Texture2D sprite;

    // Input
    Vector2 input_vector;
    bool canDoubleJump;

    // Physics
    bool grounded;
    float mass;
    CapsuleCollider base_capsule_collider;
};

static inline void playerFrameReset(Player* player) {
    player->input_vector = (Vector2){0};
}

static inline CapsuleCollider playerComputeCollider(const Player* player) {
    const Vector2 x1 = Vector2Add(player->base_capsule_collider.x1, player->position);
    const Vector2 x2 = Vector2Add(player->base_capsule_collider.x2, player->position);
    CapsuleCollider c = {x1, x2, player->base_capsule_collider.radius};
    return c;
}

Player createPlayer();

void destroyPlayer(Player* player);


#endif // ENTITY_H_
