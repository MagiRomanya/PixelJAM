#include "physics.h"
#include "entity.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

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

void computeCapsuleCapsuleCollisionNaive(CapsuleCollider* c1, CapsuleCollider* c2, Collision*out) {
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
    out->point = C2;
}
