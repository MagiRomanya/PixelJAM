#include "physics.h"
#include "raylib.h"
#include "raymath.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

PhysicsState allocate_physics_state(size_t n_dof) {
    PhysicsState state = {0};
    state.n_dof = n_dof;
    state.x = malloc(n_dof * sizeof(float));
    state.v = malloc(n_dof * sizeof(float));
    state.f = malloc(n_dof * sizeof(float));

    return state;
}

void destroy_physics_state(PhysicsState* state) {
    free(state->x);
    free(state->v);
    free(state->f);
}

PhysicsState copy_physics_state(const PhysicsState* state) {
    PhysicsState s = {0};
    s.n_dof = state->n_dof;
    s.x = malloc(state->n_dof * sizeof(float));
    s.v = malloc(state->n_dof * sizeof(float));
    s.f = malloc(state->n_dof * sizeof(float));
    memcpy(s.x, state->x, state->n_dof*sizeof(float));
    memcpy(s.v, state->v, state->n_dof*sizeof(float));
    memcpy(s.f, state->f, state->n_dof*sizeof(float));

    return s;
}


void compute_forces(PhysicsState* state) {
    const size_t n_particles = state->n_dof / 2;
    const float L0 = 190.0f;
    const float k = 1.0f;
    const float damping = 10.0f;
    const float mass = 1.0f;

    // Set the forces to zero
    for (size_t i = 0; i < state->n_dof; i++) {
        state->f[i] = 0;
    }

    for (size_t i = 0; i < n_particles - 1; i++) {
        const Vector2 x1 = {state->x[2*i+0],state->x[2*i+1]};
        const Vector2 x2 = {state->x[2*i+0+2],state->x[2*i+1+2]};
        // printf("Force = {%f, %f}\n", state->f[2*i], state->f[2*i+1]);
        // printf("Force = {%f, %f}\n", state->f[2*i+2], state->f[2*i+3]);

        const Vector2 v1 = {state->v[2*i+0],state->v[2*i+1]};
        const Vector2 v2 = {state->v[2*i+0+2],state->v[2*i+1+2]};

        float L = Vector2Distance(x1, x2);
        Vector2 dx = Vector2Subtract(x1, x2);
        const float coeff = -k * (L - L0) / L;

        // f += - damping/L² * dx * dx.transpose() * (v1 - v2);
        Vector2 dv = Vector2Subtract(v1, v2);
        Vector2 uut_dv = {dx.x*dx.x * dv.x + dx.x*dx.y * dv.y,
                          dx.y*dx.x * dv.x + dx.y*dx.y * dv.y};

        const float dampL2 = - damping / (L*L);

        // Assign forces
        state->f[2*i+0] +=  coeff * dx.x + dampL2 * uut_dv.x;
        state->f[2*i+1] +=  coeff * dx.y + dampL2 * uut_dv.y;

        state->f[2*i+0+2] += -coeff * dx.x - dampL2 * uut_dv.x;;
        state->f[2*i+1+2] += -coeff * dx.y - dampL2 * uut_dv.y;;
    }
}


void solve_physics(PhysicsState* state) {
    const size_t n_particles = state->n_dof / 2;
    const size_t n_substeps = 1;
    const float h = TIME_STEP / n_substeps;
    const float mass = 1.0f;

    for (size_t n = 0; n < n_substeps; n++) { // Substeps

        compute_forces(state);

        for (size_t i = 0; i < state->n_dof; i++) { // Particles
            state->v[i] += state->f[i] / mass * h;
            state->x[i] += state->v[i];
        }
    }

}
