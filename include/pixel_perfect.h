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


#endif // PIXEL_PERFECT_H_
