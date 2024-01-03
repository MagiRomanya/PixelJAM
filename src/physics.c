#include "physics.h"
#include "entity.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void set_array_to_zero(float* arr, size_t size) {
    for (size_t i = 0; i < size; i++)
        arr[i] = 0;
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

void computeCapsuleCapsuleCollisionNaive(const CapsuleCollider* c1, const CapsuleCollider* c2, Collision*out) {
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
        C2 = p1;
        C1 = c1->x1;
    } else if (dist2 <= dist1 && dist2 <= dist3 && dist2 <= dist4) {
        C2 = p2;
        C1 = c1->x2;
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

void computePlayerWorldCollisions(Player* player, GameColliderList* clist) {
    const float collisionStiffness = 100.0;
    player->grounded = false;
    for (size_t i = 0; i < clist->size; i++) {
        const GameCollider* gameCollider = getGameColliderFromList(clist, i);
        if (gameCollider->collision_mask == PLAYER_COLLIDE || gameCollider->collision_mask == PLAYER_CABLE_COLLIDE) {
            Collision collision;
            CapsuleCollider playerCollider = playerComputeCollider(player);
            computeCapsuleCapsuleCollisionNaive(&gameCollider->capsule_collider, &playerCollider, &collision);

            if (collision.signed_distance < 0) {
                // Check if grounded
                const float normal_aligned_to_vertical = Vector2DotProduct(collision.normal, (Vector2){0.0,-1.0});
                const float verticallity_threshold = 0.85f;
                player->grounded = (verticallity_threshold < normal_aligned_to_vertical);
                if (!player->grounded) player->canDoubleJump = false;
                // printf("Collision point = {%f, %f}\n", collision.normal.x, collision.normal.y);

                // collision response
                player->force = Vector2Add(player->force, Vector2MultiplyS(-collisionStiffness * collision.signed_distance, collision.normal));

                // Damping
                const float collisionBounceDamping = 20.0;
                Vector2 uut_v = {collision.normal.x*collision.normal.x * player->velocity.x + collision.normal.x*collision.normal.y * player->velocity.y,
                                 collision.normal.y*collision.normal.x * player->velocity.x + collision.normal.y*collision.normal.y * player->velocity.y};
                player->force = Vector2Add(player->force, Vector2MultiplyS(-collisionBounceDamping, uut_v));
                player->force = Vector2Add(player->force, Vector2MultiplyS(-gameCollider->friction_damping, Vector2Subtract(player->velocity, uut_v)));
            }
        }
    }
}


void updatePlayerMovement(Player* player, GameColliderList* clist) {
    const float inputMultiplyer = 100.0f;
    player->force = Vector2MultiplyS(inputMultiplyer, player->input_vector); // input
    player->force.y += player->mass * GRAVITY;
    if (!player->grounded) {
        const float airDamping = 1.f;
        player->force = Vector2Add(player->force, Vector2MultiplyS(-airDamping, player->velocity));
    }
    computePlayerWorldCollisions(player, clist);

    // Integration
    player->velocity = Vector2Add(player->velocity, Vector2MultiplyS(TIME_STEP / player->mass, player->force));
    player->position = Vector2Add(player->position, Vector2MultiplyS(TIME_STEP, player->velocity));

}

CapsuleCollider createCapsule(int x1, int x2, int y, float r) {
    CapsuleCollider c;
    c.x1 = (Vector2){x1, y};
    c.x2 = (Vector2){x2, y};
    c.radius = r;
    return c;
}
