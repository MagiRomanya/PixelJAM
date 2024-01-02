#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "raylib.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define GRAVITY 9.81f
#define TIME_STEP 0.016f

typedef struct EntityList EntityList;

typedef struct {
    float* x;
    float* v;
    float* f;
    size_t n_dof;
} PhysicsState;

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

PhysicsState allocate_physics_state(size_t n_dof);

void destroy_physics_state(PhysicsState* state);

PhysicsState copy_physics_state(const PhysicsState* state);

void solve_physics(PhysicsState* state, EntityList* elist);

void set_array_to_zero(float* arr, size_t size);

Vector2 Vector2MultiplyS(float scalar, Vector2 vec);

void computePointCapsuleCollision(CapsuleCollider* c, Vector2 point, Collision* out);


void cableEntityHandleCollisions(EntityList* entities, PhysicsState* state);

#endif // PHYSICS_H_
