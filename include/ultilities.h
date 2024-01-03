#ifndef ULTILITIES_H_
#define ULTILITIES_H_

#include "physics.h"
#include "entity.h"
#include "raylib.h"
#include <stddef.h>
#include <stdio.h>

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

static inline void showTitleScreen() {
    const float title_screen_duration = 2.0f;
    size_t frameNumber = 0;
    // Title screen
    Texture2D titleScreen = LoadTexture("assets/sprites/titlescreen.png");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        frameNumber++;
        float titleTime = frameNumber*GetFrameTime();
        if (title_screen_duration < titleTime) break;
        BeginDrawing();
        {
            ClearBackground(BLACK);
            Rectangle source = {0, 0, 1920, 1080};
            Rectangle destination = {0,0,GetScreenWidth(), GetScreenHeight()};
            const float effect = titleTime / title_screen_duration;
            Color color = {255.0, 255.0, 255.0, 255.0*effect};
            DrawTexturePro(titleScreen, source, destination, (Vector2){0}, 0, color);
        }
        EndDrawing();
    }
    UnloadTexture(titleScreen);
}

static inline void renderCollisionCapsules(GameColliderList* c_list) {
    for (size_t i = 0; i < c_list->size; i++) {
        GameCollider *c = getGameColliderFromList(c_list, i);
        renderCapsule(c->capsule_collider);
    }
}

#endif // ULTILITIES_H_
