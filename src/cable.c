#include "cable.h"
#include "appliance.h"
#include "entity.h"
#include "physics.h"
#include "raylib.h"
#include "raymath.h"
#include "sprite_manager.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define ANCHOR_REMOVE_DISTANCE 40.0f

Cable createCable(Vector2 initialAnchor, size_t maxAnchors, float maxLength) {
    Cable cable = {0};
    cable.maxLength = maxLength;
    cable.nMaxAnchors = maxAnchors+1;
    cable.nAnchors = 1;
    cable.anchors = malloc(sizeof(Anchor) * (maxAnchors + 30));
    cable.anchors[0] = (Anchor){initialAnchor, true};
    return cable;
}

Anchor* cableGetLastAnchor(Cable* cable) {
    return &cable->anchors[cable->nAnchors-1];
}

float computeCableLength(Cable* cable) {
    if (cable->nAnchors == 1) return 0.0f;
    float length = 0.0f;
    for (size_t i = 0; i < cable->nAnchors-1; i++) {
        Anchor a1 = cable->anchors[i];
        Anchor a2 = cable->anchors[i+1];
        length += Vector2Distance(a1.position, a2.position);
    }
    return length;
};

bool computeLastSegmentIntersection(Vector2 x1, Vector2 x2, GameColliderList* c_list) {
    bool collide = false;
    for (size_t i = 0; i < c_list->size; i++) {
        GameCollider* collider = getGameColliderFromList(c_list, i);
        Vector2 padding = {0, 0};
        /* printf("criterion = %f\n", Vector2Subtract(collider->capsule_collider.x1, collider->capsule_collider.x2).x); */
        if (Vector2Subtract(collider->capsule_collider.x1, collider->capsule_collider.x2).x != 0) padding = (Vector2){-8,0};
        Vector2 x3 = Vector2Add(padding, collider->capsule_collider.x1);
        Vector2 x4 = Vector2Subtract(collider->capsule_collider.x2, padding);
        if (do_segments_intersect(x1, x2, x3, x4)) {
            /* DrawLineV(x3, x4, BLUE); */
            collide = true;
            break;
        }
    }
    return collide;
}

PLACE_ANCHOR_RESULT tryCreateAnchor(Cable* cable, GameColliderList* c_list, ApplianceList* a_list,  Vector2 position) {
    Anchor* lastAnchor = cableGetLastAnchor(cable);
    float cableLength = computeCableLength(cable);
    float newFragmentLength = Vector2Distance(position, lastAnchor->position);
    if (cableLength + newFragmentLength >= cable->maxLength) {
        PlaySound(getSoundTrackFromID(SOUND_TRACK_ERROR_EFFECT_ID));
        return ANCHOR_NOT_ENOUGH_LENGTH;
    }

    // New cable segment
    bool collide = computeLastSegmentIntersection(lastAnchor->position, position, c_list);
    if (collide) {
        PlaySound(getSoundTrackFromID(SOUND_TRACK_ERROR_EFFECT_ID));
        return ANCHOR_OBSTRUDED_PATH;
    }

    // Check if we can connect appliance
    bool connectToAppliance = false;
    for (size_t i = 0; i < a_list->size; i++) {
        Appliance* a = getApplianceFromList(a_list, i);
        if (!a->connected && CheckCollisionPointRec(position, a->hit_box)) {
            switch (a->type) {
                case WASHING_MACHINE:
                    PlaySound(getSoundTrackFromID(SOUND_TRACK_WASHING_MACHINE_ID));
                    break;
                case BLENDER:
                    PlaySound(getSoundTrackFromID(SOUND_TRACK_BLENDER_ID));
                    break;
                case TELEVISION:
                    PlaySound(getSoundTrackFromID(SOUND_TRACK_TELEVISION_ID));
                    break;
            }
            connectToAppliance = true;
            a->connected = true;
            cable->nMaxAnchors++;
            cable->nConnectedAppliances++;
            Vector2 applianceCenter = {a->hit_box.x + a->hit_box.width/2.0f, a->hit_box.y + a->hit_box.height/2.0f};
            cable->anchors[cable->nAnchors] = (Anchor){applianceCenter, false};
            cable->nAnchors++;
            break;
        }
    }


    if (!connectToAppliance) {
        if (cable->nAnchors >= cable->nMaxAnchors) {
            PlaySound(getSoundTrackFromID(SOUND_TRACK_ERROR_EFFECT_ID));
            return ANCHOR_NOT_ENOUGH_ANCHORS;
        }

        // Place normal anchor
        cable->anchors[cable->nAnchors] = (Anchor){position, true};
        cable->nAnchors++;
    }

    PlaySound(getSoundTrackFromID(SOUND_TRACK_STAPLE_ID));
    return ANCHOR_SUCCESS;
}

bool tryRemoveLastAnchor(Cable* cable, ApplianceList* a_list, Vector2 position) {
    if (cable->nAnchors <= 1) return false; // Can not remove base anchor
    Anchor* anchor = cableGetLastAnchor(cable);
    if (Vector2Distance(position, anchor->position) > ANCHOR_REMOVE_DISTANCE) return false; // To far away

    for (size_t i = 0; i < a_list->size; i++) {
        Appliance* a = getApplianceFromList(a_list, i);
        if (a->connected && CheckCollisionPointRec(anchor->position, a->hit_box)) {
            a->connected = false;
            cable->nConnectedAppliances--;
            cable->nMaxAnchors--;
        }
    }
    cable->nAnchors--;
    PlaySound(getSoundTrackFromID(SOUND_TRACK_STAPLE_REMOVE_ID));
    return true;
}

void destroyCable(Cable* cable) {
    free(cable->anchors);
}

void drawCable(Cable* cable, Player* player, GameColliderList* c_list) {
    Color cableColor = BLACK;
    Color anchorColor = GRAY;
    // Draw cables
    for (size_t i = 0; i < cable->nAnchors-1; i++) {
        Anchor a1 = cable->anchors[i];
        Anchor a2 = cable->anchors[i+1];
        DrawLineV(a1.position, a2.position, cableColor);
    }
    // Last cable (red if not)
    Vector2 lastAnchorPos = cableGetLastAnchor(cable)->position;
    bool collided = computeLastSegmentIntersection(computePlayerHandPosition(player), lastAnchorPos, c_list);
    if (collided) cableColor = RED;
    DrawLineV(lastAnchorPos, computePlayerHandPosition(player), cableColor);

    // Draw anchors
    for (size_t i = 0; i < cable->nAnchors; i++) {
        Anchor a = cable->anchors[i];
        if (!a.visible) continue;
        const int half_width = 8;
        Rectangle rect = {a.position.x-half_width, a.position.y-half_width, 2*half_width, 2*half_width};
        Texture2D anchorTexture = getSpriteFromID(SPRITE_ANCHOR_ID);
        DrawTexture(anchorTexture, a.position.x-half_width, a.position.y-half_width, WHITE);
    }
};
