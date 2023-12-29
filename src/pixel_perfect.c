#include "pixel_perfect.h"

#define VIRTUAL_SCREEN_WIDTH 160
#define VIRTUAL_SCREEN_HEIGHT 90

int GetVirtualScreenWidth() { return VIRTUAL_SCREEN_WIDTH; }

int GetVirtualScreenHeight() { return VIRTUAL_SCREEN_HEIGHT; }

PixelPerfectData computePixelPerfectData(Vector2 cameraTarget) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    float vScreenWidth = VIRTUAL_SCREEN_WIDTH;
    float vScreenHeight = VIRTUAL_SCREEN_HEIGHT;
    PixelPerfectData data = {0};

    data.worldCamera.zoom = 1.0;
    data.worldCamera.target.x = (int)cameraTarget.x;
    data.worldCamera.target.y = (int)cameraTarget.y;

    data.screenSpaceCamera.zoom = 1.0;
    float virtualRatio = (float)screenWidth / (float)vScreenWidth;
    data.screenSpaceCamera.target.x = (cameraTarget.x - ((int)cameraTarget.x))*virtualRatio;
    data.screenSpaceCamera.target.y = (cameraTarget.y - ((int)cameraTarget.y))*virtualRatio;

    const Rectangle worldSpaceRectangle = { 0.0f, 0.0f, vScreenWidth, -vScreenHeight };
    data.worldRec= worldSpaceRectangle;

    const Rectangle screenSpaceRectangle = { -virtualRatio, -virtualRatio, screenWidth + (virtualRatio*2), screenHeight + (virtualRatio*2) };
    data.screenSpaceRec = screenSpaceRectangle;

    return data;
}
