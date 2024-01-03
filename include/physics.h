#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "raylib.h"
#include "raymath.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define GRAVITY 70.0f
#define TIME_STEP 0.016f

typedef struct EntityList EntityList;
typedef struct Player Player;

typedef struct {
    Vector2 x1;
    Vector2 x2;
    float radius;
} CapsuleCollider;

typedef enum {
NO_COLLISION,
NORMAL_COLLISION,
ICE_COLLISION,
} CollisionType;

typedef struct {
    Vector2 normal;
    float signed_distance;
} Collision;

void updatePlayerMovement(Player* player, EntityList* elist);

void set_array_to_zero(float* arr, size_t size);

Vector2 Vector2MultiplyS(float scalar, Vector2 vec);

void computePointCapsuleCollision(CapsuleCollider* c, Vector2 point, Collision* out);


#endif // PHYSICS_H_
