#ifndef ULTILITIES_H_
#define ULTILITIES_H_

#include "physics.h"
#include "raylib.h"

static inline void renderCapsule(CapsuleCollider collider) {
    const float radius = collider.radius;
    DrawCircle(collider.x1.x, collider.x1.y, radius, MAROON);
    DrawCircle(collider.x2.x, collider.x2.y, radius, MAROON);
    DrawRectangle(collider.x1.x, collider.x1.y-radius, collider.x2.x - collider.x1.x, collider.x2.y - collider.x1.y + 2*radius, MAROON);

}

#endif // ULTILITIES_H_
