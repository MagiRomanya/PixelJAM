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

void playerFrameReset(Player* player);

CapsuleCollider playerComputeCollider(const Player* player);

Player createPlayer();

void destroyPlayer(Player* player);

typedef struct {
    Texture2D sprite;
    Vector2 position;
} GameTile ;


#endif // ENTITY_H_
