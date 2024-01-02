#include "physics.h"
#include "entity.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void set_array_to_zero(float* arr, size_t size) {
    for (size_t i = 0; i < size; i++)
        arr[i] = 0;
}


PhysicsState allocate_physics_state(size_t n_dof) {
    PhysicsState state = {0};
    state.n_dof = n_dof;
    state.x = malloc(n_dof * sizeof(float));
    state.v = malloc(n_dof * sizeof(float));
    state.f = malloc(n_dof * sizeof(float));

    set_array_to_zero(state.x, n_dof);
    set_array_to_zero(state.v, n_dof);
    set_array_to_zero(state.f, n_dof);

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


void compute_forces(PhysicsState* state, EntityList* elist) {
    const size_t n_particles = state->n_dof / 2;
    const float L0 = 190.0f;
    const float k = 1.0f;
    const float damping = 10.0f;
    const float mass = 1.0f;

    // Set the forces to zero
    set_array_to_zero(state->f, state->n_dof);

    cableEntityHandleCollisions(elist, state);

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
        state->f[2*i+1] +=  coeff * dx.y + dampL2 * uut_dv.y + GRAVITY;

        state->f[2*i+0+2] += -coeff * dx.x - dampL2 * uut_dv.x;;
        state->f[2*i+1+2] += -coeff * dx.y - dampL2 * uut_dv.y;;
    }
    state->f[state->n_dof-1] += GRAVITY;
}


void solve_physics(PhysicsState* state, EntityList* elist) {
    const size_t n_particles = state->n_dof / 2;
    const size_t n_substeps = 1;
    const float h = TIME_STEP / n_substeps;
    const float mass = 1.0f;

    for (size_t n = 0; n < n_substeps; n++) { // Substeps

        compute_forces(state, elist);

        for (size_t i = 0; i < state->n_dof; i++) { // Particles
            state->v[i] += state->f[i] / mass * h;
            state->x[i] += state->v[i];
        }
    }

}

Vector2 Vector2MultiplyS(float scalar, Vector2 vec) {
    return (Vector2){vec.x*scalar, vec.y*scalar};
}

void clamp(float* num, float min, float max) {
    if (*num > max) *num = max;
    if (*num < min) *num = min;
}

Vector2 point_segment_closest_point(Vector2 point, Vector2 x1, Vector2 x2) {
    float x1x2dist = Vector2Distance(x1, x2);
    Vector2 dx = Vector2Subtract(x2, x1);
    Vector2 dp = Vector2Subtract(point, x1);

    float dot = Vector2DotProduct(dx, dp) / x1x2dist;

    if (dot > x1x2dist) {
        return x2;
    }
    else if (dot < 0) {
        return x1;
    }
    else {
        return Vector2Add(Vector2MultiplyS(dot / x1x2dist, dx), x1);
    }
}


void computeCapsuleCapsuleCollision(CapsuleCollider* c1, CapsuleCollider* c2, Collision*out) {
    // Compute distances to closest points on line segments
    Vector2 p1 = point_segment_closest_point(c1->x1, c2->x1, c2->x2);
    Vector2 p2 = point_segment_closest_point(c1->x2, c2->x1, c2->x2);
    Vector2 p3 = point_segment_closest_point(c2->x1, c1->x1, c1->x2);
    Vector2 p4 = point_segment_closest_point(c2->x2, c1->x1, c1->x2);

    float dist1 = Vector2DistanceSqr(p1, c1->x1);
    float dist2 = Vector2DistanceSqr(p2, c1->x2);
    float dist3 = Vector2DistanceSqr(p3, c2->x1);
    float dist4 = Vector2DistanceSqr(p4, c2->x2);

    // Find the pair with the smallest distance
    Vector2 C1;
    Vector2 C2;
    if (dist1 <= dist2 && dist1 <= dist3 && dist1 <= dist4) {
        C1 = p1;
        C2 = c1->x1;
    } else if (dist2 <= dist1 && dist2 <= dist3 && dist2 <= dist4) {
        C1 = p2;
        C2 = c1->x2;
    } else if (dist3 <= dist1 && dist3 <= dist2 && dist3 <= dist4) {
        C1 = p3;
        C2 = c2->x1;
    } else {
        C1 = p4;
        C2 = c2->x2;
    }

    float distance = Vector2Distance(C1, C2);
    Vector2 dC = Vector2Subtract(C2, C1);
    Vector2 normal = Vector2MultiplyS(1.0f / distance, dC);

    out->signed_distance = distance - c1->radius - c2->radius;
    out->normal = normal;
}

#define COLLISIONS_CAPACITY (1e3*sizeof(Collision))

typedef struct {
    Collision* collisions;
    size_t size;
    size_t capacity;
} CollisionList;

CollisionList createCollisionsList() {
    CollisionList clist;
    clist.size = 0;
    clist.capacity = COLLISIONS_CAPACITY;
    clist.collisions = malloc(clist.capacity);
    return clist;
}

void addCollisionToList(CollisionList* clist, Collision* c) {
    clist->collisions[sizeof(Collision)*clist->size] = *c;
    clist->size += sizeof(Collision);
}

Collision getCollisionFromList(CollisionList* clist, size_t index) {
    return clist->collisions[sizeof(Collision) * index];
}

void clearCollisionList(CollisionList* clist) { clist->size = 0; }

void destroyCollisionList(CollisionList* clist) {
    free(clist->collisions);
}

void cableEntityHandleCollisions(EntityList* entities, PhysicsState* state) {
    const size_t n_particles = state->n_dof/2;

    for (size_t i = 0; i < entities->size; i++) {
        Entity* e = getEntityFromList(entities, i);
        if (e->collision_mask == CABLE_COLLIDE || e->collision_mask == PLAYER_CABLE_COLLIDE) {
            for (size_t j = 0; j < n_particles - 1; j++) {
                Vector2 x1 = {state->x[2*j], state->x[2*j+1]};
                Vector2 x2 = {state->x[2*j+2], state->x[2*j+1+2]};
                const float cable_link_radius = 10; // Cable thickness
                CapsuleCollider cable_link_capsule={x1, x2, cable_link_radius};
                Collision c;
                computeCapsuleCapsuleCollision(&cable_link_capsule, &e->capsule_collider, &c);
                if (c.signed_distance < 0) {
                    // printf("Collision\n");
                    const float collision_stiffness = 10.0;
                    Vector2 collision_force = Vector2MultiplyS(- collision_stiffness * c.signed_distance, c.normal);
                    state->f[2*j+0] += collision_force.x;
                    state->f[2*j+1] += collision_force.y;
                    state->f[2*j+0+2] += collision_force.x;
                    state->f[2*j+1+2] += collision_force.y;
                }
            }
        }
    }
}

// void computeCapsuleCapsuleCollisionOld(CapsuleCollider* c1, CapsuleCollider* c2, Collision*out) {
//     Vector2 v1 = Vector2Subtract(c1->x2, c1->x1);
//     Vector2 v2 = Vector2Subtract(c2->x2, c2->x1);
//     Vector2 w = Vector2Subtract(c2->x1, c1->x1);
//     DrawLineV(c1->x2, c1->x1, BLACK);
//     DrawLineV(c2->x2, c2->x1, BLACK);
//     DrawLineV(c2->x1, c1->x1, BLACK);

//     float a = Vector2DotProduct(v1, v1);
//     float b = Vector2DotProduct(v1, v2);
//     float c = Vector2DotProduct(v2, v2);
//     float d = Vector2DotProduct(v1, w);
//     float e = Vector2DotProduct(v2, w);

//     float denom = b*b - c * a;
//     printf("a = %f\n", a);
//     printf("b = %f\n", b);
//     printf("c = %f\n", c);
//     /* printf("a = %f", a); */
//     /* printf("a = %f", a); */
//     printf("denom = %f\n", denom);
//     float s, t;
//     if (fabs(denom) < 1e-3) {
//         printf("Hellow\n");
//         s = 0;
//         t = -d/b;
//     }
//     else {
//         s = (e * b - c * d) / denom;
//         t = (-d * b + a * e) / denom;
//     }

//     // Closest points
//     Vector2 C1;
//     Vector2 C2;
//     printf("s = %f, t = %f\n", s, t);
//     if ((s <= 1.0f && s >= 0.0f) && (t <= 1.0f && t >= 0.0f)) {
//         C1 = Vector2Add(c1->x1, Vector2MultiplyS(s, v1));
//         C2 = Vector2Add(c2->x1, Vector2MultiplyS(t, v2));
//         DrawCircleV(C1, 20, YELLOW);
//         DrawCircleV(C2, 20, YELLOW);
//     }
//     else {
//         printf("bye\n");
//         // Compute distances to closest points on line segments
//         Vector2 p1 = point_segment_closest_point(c1->x1, c2->x1, c2->x2);
//         Vector2 p2 = point_segment_closest_point(c1->x2, c2->x1, c2->x2);
//         Vector2 p3 = point_segment_closest_point(c2->x1, c1->x1, c1->x2);
//         Vector2 p4 = point_segment_closest_point(c2->x2, c1->x1, c1->x2);

//         float dist1 = Vector2DistanceSqr(p1, c1->x1);
//         float dist2 = Vector2DistanceSqr(p2, c1->x2);
//         float dist3 = Vector2DistanceSqr(p3, c2->x1);
//         float dist4 = Vector2DistanceSqr(p4, c2->x2);

//         // Find the pair with the smallest distance
//         if (dist1 <= dist2 && dist1 <= dist3 && dist1 <= dist4) {
//             C1 = p1;
//             C2 = c1->x1;
//         }
//         else if (dist2 <= dist1 && dist2 <= dist3 && dist2 <= dist4) {
//             C1 = p2;
//             C2 = c1->x2;
//         }
//         else if (dist3 <= dist1 && dist3 <= dist2 && dist3 <= dist4) {
//             C1 = p3;
//             C2 = c2->x1;
//         }
//         else {
//             C1 = p4;
//             C2 = c2->x2;
//         }
//         DrawCircleV(C1, 20, GREEN);
//         DrawCircleV(C2, 20, GREEN);
//     }


//     float distance = Vector2Distance(C1, C2);
//     Vector2 dC = Vector2Subtract(C1, C2);
//     Vector2 normal = Vector2MultiplyS(1.0f/distance, dC);

//     out->signed_distance = distance - c1->radius - c2->radius;
//     out->normal = normal;
// }
