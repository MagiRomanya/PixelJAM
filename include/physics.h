#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "raylib.h"
#include "raymath.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct GameColliderList GameColliderList;
typedef struct Player Player;
typedef struct Cable Cable;

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

void updatePlayerMovement(Player* player, Cable* cable, GameColliderList* clist);

void set_array_to_zero(float* arr, size_t size);

Vector2 Vector2MultiplyS(float scalar, Vector2 vec);

void computePointCapsuleCollision(CapsuleCollider* c, Vector2 point, Collision* out);

CapsuleCollider createCapsule(int x1, int x2, int y, float r);

bool do_segments_intersect(Vector2 p1, Vector2 p2, Vector2 q1, Vector2 q2);

#endif // PHYSICS_H_
