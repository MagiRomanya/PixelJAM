#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define GRAVITY 9.81f
#define TIME_STEP 0.016f

typedef struct {
    float* x;
    float* v;
    float* f;
    size_t n_dof;
} PhysicsState;

PhysicsState allocate_physics_state(size_t n_dof);

void destroy_physics_state(PhysicsState* state);

PhysicsState copy_physics_state(const PhysicsState* state);

void solve_physics(PhysicsState* state);

#endif // PHYSICS_H_
