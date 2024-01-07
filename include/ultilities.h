#ifndef ULTILITIES_H_
#define ULTILITIES_H_

#include "appliance.h"
#include "cable.h"
#include "physics.h"
#include "entity.h"
#include "pixel_perfect.h"
#include "raylib.h"
#include "raymath.h"
#include "sprite_manager.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static inline void renderCapsule(CapsuleCollider collider) {
    const float radius = collider.radius;
    DrawCircle(collider.x1.x, collider.x1.y, radius, MAROON);
    DrawCircle(collider.x2.x, collider.x2.y, radius, MAROON);
    DrawRectangle(collider.x1.x, collider.x1.y-radius, collider.x2.x - collider.x1.x, collider.x2.y - collider.x1.y + 2*radius, MAROON);
    DrawRectangle(collider.x1.x-radius, collider.x1.y,
                  collider.x2.x - collider.x1.x + 2*radius, collider.x2.y - collider.x1.y,
                  MAROON);

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

static inline void drawTextInsideRectangle(Rectangle rectangle, char* text, int fontSize, Color color) {
    DrawText(text, rectangle.x + rectangle.width/2.0f - MeasureText(text, fontSize)/2.0f, rectangle.y + (rectangle.height-fontSize)/2.0f, fontSize, color);
}

static inline void drawMessage(RenderMessage* message, Vector2 position) {
    message->ticks++;
    if (message->ticks > message->lifetime) return;
    const float progress = ((float)message->ticks) /((float)message->lifetime);
    const float MESSAGE_FONT_SIZE = GetScreenWidth()* 0.01;
    float virtualRatio = getVirtualRatio();
    Texture2D cloud = getSpriteFromID(SPRITE_THOUGHT_CLOUD_ID);
    Rectangle source = {0,0,cloud.width,cloud.height};
    Rectangle dest = {0,0,cloud.width*virtualRatio,cloud.height*virtualRatio};
    Vector2 offset = {0, virtualRatio*20};
    Vector2 origin = Vector2Add(Vector2Negate(position), offset);
    const float fadeTime = 0.2f;
    float t = progress / 0.2f;
    if (progress <= fadeTime) {
        t = progress / fadeTime;
    }
    if (progress > fadeTime) t = 1.0f;
    if (progress > 1.0f - fadeTime) {
        t = (1.0f - progress) / fadeTime;
    }
    Color textColor = {0,0,0, 255*t};
    Color cloudColor = {255,255,255, 255*t};
    DrawTexturePro(cloud, source, dest, origin, 0, cloudColor);
    Rectangle textRectangle = {dest.x - origin.x, dest.y - origin.y, dest.width, dest.height};
    drawTextInsideRectangle(textRectangle, message->message, MESSAGE_FONT_SIZE, textColor);
}

static inline void renderCollisionCapsules(GameColliderList* c_list) {
    for (size_t i = 0; i < c_list->size; i++) {
        GameCollider *c = getGameColliderFromList(c_list, i);
        renderCapsule(c->capsule_collider);
    }
}

static inline void renderCableLengthUI(Cable* cable, Player* player) {
    Rectangle sourceRec = {0.0f, 0.0f, 32.0f, 32.0f};
    const float virtualRatio = getVirtualRatio();
    const float width = 32*virtualRatio;
    Rectangle destRec = {0, 0, width, width};
    const float paddingA = 4.0f * virtualRatio;
    const float paddingB = 7.0f * virtualRatio;
    Rectangle cableLengthBackgound = {8*virtualRatio + paddingA, 8*virtualRatio +paddingA, width -2*paddingA, width -paddingB};
    const float cableLength = Vector2Distance(cableGetLastAnchor(cable)->position, computePlayerHandPosition(player)) + computeCableLength(cable);
    const float remainingCable = (cable->maxLength - cableLength) / cable->maxLength;
    Rectangle cableLengthProgress = {8*virtualRatio + paddingA, 8*virtualRatio +paddingA, remainingCable * (width -2*paddingA), width -paddingB};
    /* DrawRectangleRec(cableLengthBackgound, RED); */
    DrawRectangleRec(cableLengthProgress, GRAY);
    DrawTexturePro(getSpriteFromID(SPRITE_CABLE_LENGTH_ID), sourceRec, destRec, (Vector2){-8*virtualRatio,-8*virtualRatio}, 0, WHITE);
}

static inline void renderAnchorsLeftUI(Cable* cable) {
    Texture2D anchorTexture = getSpriteFromID(SPRITE_ANCHOR_ID);
    const float oWidth = anchorTexture.width;
    Rectangle sourceRec = {0.0f, 0.0f, anchorTexture.width, anchorTexture.height};
    const float virtualRatio = getVirtualRatio();
    Rectangle destRec = {0, 0, anchorTexture.width*virtualRatio, anchorTexture.height*virtualRatio};
    Vector2 origin = {-(anchorTexture.width/ 4.0f + 32.0)*virtualRatio, -anchorTexture.height*virtualRatio/4.0f};
    const int remainingAnchors = cable->nMaxAnchors - cable->nAnchors;
    for (size_t i = 0; i < remainingAnchors; i++) {
        const float frameOffset = 4 * virtualRatio * i;
        Vector2 frameOrigin = Vector2Add(origin, (Vector2){-frameOffset, 0.0f} );
        DrawTexturePro(getSpriteFromID(SPRITE_ANCHOR_ID), sourceRec, destRec, frameOrigin, 0, GRAY);
    }
}

static inline void renderAnimation(Texture2D texture, int x, int y, int nFrames, int frameSpeed, int* frameCounter, int* currentFrame) {
    int width = texture.width / nFrames;
    Rectangle frameRec = {0.0f, 0.0f, width, texture.height};
    if ((*frameCounter) >= 1.0f/(GetFrameTime()*frameSpeed)) {
        (*frameCounter) = 0;
        (*currentFrame)++;
        if ((*currentFrame) > nFrames - 1) (*currentFrame) = 0;
    }
    frameRec.x = (*currentFrame) * width;
    (*frameCounter)++;
    DrawTextureRec(texture, frameRec, (Vector2) {x, y}, WHITE);
}

static inline bool renderVictoryScreen(ApplianceList* a_list) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    if (areAllAppliancesConnected(a_list)) {
        const float fontSize = screenHeight/5.0f;
        const Vector2 textPosition = {screenWidth/2.0f, screenHeight/2.0f};
        const char* victoryMessage = "All connected!";
        DrawText(victoryMessage, textPosition.x - MeasureText(victoryMessage, fontSize) /2.0f, textPosition.y, fontSize, DARKGREEN);
        const float buttonWidth = screenWidth / 10.0f;
        const float buttonHeight = screenHeight / 20.0f;
        const float buttonPositionX = screenWidth/2.0f - buttonWidth/2.0f;
        const float buttonPositionY = screenHeight/2.0f - buttonHeight / 2.0f + fontSize;
        Rectangle continueButtonRec = {buttonPositionX, buttonPositionY, buttonWidth, buttonHeight};
        DrawRectangleRec(continueButtonRec, DARKGREEN);
        const float continueSize = buttonWidth/5.f;
        DrawText("Continue", buttonPositionX + buttonWidth/2.0f - MeasureText("Continue", continueSize)/2.0, buttonPositionY + buttonHeight*0.2f, continueSize, BLACK);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            if (CheckCollisionPointRec(GetMousePosition(), continueButtonRec)) return false;
    }
    return true;
}

static inline void stopAppliancesSounds() {
    if (IsSoundPlaying(getSoundTrackFromID(SOUND_TRACK_WASHING_MACHINE_ID))) StopSound(getSoundTrackFromID(SOUND_TRACK_WASHING_MACHINE_ID));
    if (IsSoundPlaying(getSoundTrackFromID(SOUND_TRACK_BLENDER_ID))) StopSound(getSoundTrackFromID(SOUND_TRACK_BLENDER_ID));
    if (IsSoundPlaying(getSoundTrackFromID(SOUND_TRACK_TELEVISION_ID))) StopSound(getSoundTrackFromID(SOUND_TRACK_TELEVISION_ID));
}

#endif // ULTILITIES_H_
