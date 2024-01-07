#ifndef PIXEL_PERFECT_H_
#define PIXEL_PERFECT_H_

#include "raylib.h"

int getVirtualScreenWidth();

int getVirtualScreenHeight();

typedef struct {
    Camera2D worldCamera;
    Camera2D screenSpaceCamera;
    Rectangle worldRec;
    Rectangle screenSpaceRec;
} PixelPerfectData;

PixelPerfectData computePixelPerfectData(Vector2 cameraTarget);

float getVirtualRatio();

static inline Rectangle getVirtualScreenRectangle() {
    return (Rectangle){0, 0, getVirtualScreenWidth(), getVirtualScreenHeight()};
}

static inline Rectangle getScreenRectangle() {
    return (Rectangle){0,0,GetScreenWidth(), GetScreenHeight()};
}


#endif // PIXEL_PERFECT_H_
