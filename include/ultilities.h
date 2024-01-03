#ifndef ULTILITIES_H_
#define ULTILITIES_H_

#include "physics.h"
#include "raylib.h"
#include <stddef.h>

static inline void renderCapsule(CapsuleCollider collider) {
    const float radius = collider.radius;
    DrawCircle(collider.x1.x, collider.x1.y, radius, MAROON);
    DrawCircle(collider.x2.x, collider.x2.y, radius, MAROON);
    DrawRectangle(collider.x1.x, collider.x1.y-radius, collider.x2.x - collider.x1.x, collider.x2.y - collider.x1.y + 2*radius, MAROON);

}

typedef struct {
    char message[512];
    size_t ticks;
    size_t lifetime;
} RenderMessage;

static inline void addMessageToBeRendered(RenderMessage* rmessage, const char* message, float seconds) {
    strcpy(rmessage->message, message);
    rmessage->lifetime = (size_t) seconds / GetFrameTime();
    rmessage->ticks = 0;
}

static inline void drawMessage(RenderMessage* message, Vector2 position) {
    message->ticks++;
    if (message->ticks > message->lifetime) return;
    const float MESSAGE_FONT_SIZE = GetScreenWidth()* 0.01;
    DrawText(message->message, position.x, position.y, MESSAGE_FONT_SIZE, RED);
}

#endif // ULTILITIES_H_
