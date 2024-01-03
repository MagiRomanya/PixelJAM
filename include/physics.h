#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "raylib.h"
#include "raymath.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define GRAVITY 9.81f
#define TIME_STEP 0.016f

typedef struct EntityList EntityList;
typedef struct Player Player;

typedef struct {
    Vector2 x1;
    Vector2 x2;
    float radius;
} CapsuleCollider;

typedef struct {
    Vector2 normal;
    float signed_distance;
    Vector2 point;
} Collision;

struct Player{
    Vector2 position;
    Vector2 velocity;

    // Input
    Vector2 input_vector;

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

void updatePlayerMovement(Player* player);

void set_array_to_zero(float* arr, size_t size);

Vector2 Vector2MultiplyS(float scalar, Vector2 vec);

void computePointCapsuleCollision(CapsuleCollider* c, Vector2 point, Collision* out);


#endif // PHYSICS_H_
