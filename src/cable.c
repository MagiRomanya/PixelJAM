#include "cable.h"
#include "entity.h"
#include "physics.h"
#include "raylib.h"
#include "raymath.h"
#include <stddef.h>
#include <stdlib.h>

#define ANCHOR_REMOVE_DISTANCE 20.0f

Cable createCable(Vector2 initialAnchor, size_t maxAnchors, float maxLength) {
    Cable cable = {0};
    cable.maxLength = maxLength;
    cable.nMaxAnchors = maxAnchors;
    cable.nAnchors = 1;
    cable.anchors = malloc(sizeof(maxAnchors));
    cable.anchors[0] = (Anchor){initialAnchor};
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

PLACE_ANCHOR_RESULT tryCreateAnchor(Cable* cable, GameColliderList* c_list,  Vector2 position) {
    if (cable->nAnchors >= cable->nMaxAnchors) return ANCHOR_NOT_ENOUGH_ANCHORS;

    Anchor* lastAnchor = cableGetLastAnchor(cable);
    float cableLength = computeCableLength(cable);
    float newFragmentLength = Vector2Distance(position, lastAnchor->position);
    if (cableLength + newFragmentLength >= cable->maxLength) return ANCHOR_NOT_ENOUGH_LENGTH;

    // New cable segment
    Vector2 v1 = lastAnchor->position;
    Vector2 v2 = position;
    bool collide = false;
    for (size_t i = 0; i < c_list->size; i++) {
        GameCollider* collider = getGameColliderFromList(c_list, i);
        if (do_segments_intersect(v1, v2, collider->capsule_collider.x1, collider->capsule_collider.x2)) {
            collide = true;
            break;
        }
    }
    if (collide) return ANCHOR_OBSTRUDED_PATH;

    cable->anchors[cable->nAnchors] = (Anchor){position};
    cable->nAnchors++;
    return ANCHOR_SUCCESS;
}

bool tryRemoveLastAnchor(Cable* cable, Vector2 position) {
    if (cable->nAnchors <= 1) return false; // Can not remove base anchor
    Anchor* anchor = cableGetLastAnchor(cable);
    if (Vector2Distance(position, anchor->position) > ANCHOR_REMOVE_DISTANCE) return false; // To far away
    cable->nAnchors--;
    return true;
}

void destroyCable(Cable* cable) {
    free(cable->anchors);
}

void drawCable(Cable* cable, Player* player) {
    const Color cableColor = BLACK;
    const Color anchorColor = GRAY;
    // Draw anchors
    for (size_t i = 0; i < cable->nAnchors; i++) {
        Anchor a = cable->anchors[i];
        const int half_width = 5;
        DrawRectangle(a.position.x-half_width, a.position.y-half_width, 2*half_width, 2*half_width, anchorColor);
    }
    // Draw cables
    for (size_t i = 0; i < cable->nAnchors-1; i++) {
        Anchor a1 = cable->anchors[i];
        Anchor a2 = cable->anchors[i+1];
        DrawLineV(a1.position, a2.position, cableColor);
    }
    // Last cable
    DrawLineV(cableGetLastAnchor(cable)->position, computePlayerHandPosition(player), cableColor);
};
