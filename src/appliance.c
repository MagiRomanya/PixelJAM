#include "appliance.h"
#include "cable.h"
#include "raylib.h"
#include "sprite_manager.h"
#include "ultilities.h"
#include <stddef.h>
#include <stdio.h>

#define APPLIANCE_LIST_CAPACITY 20*sizeof(Appliance)

Appliance createAppliance(ApplianceType type, Vector2 position) {
    Appliance a = {0};
    a.type = type;
    switch (type) {
        case WASHING_MACHINE:
            // 16 x 19
            a.hit_box = (Rectangle){position.x - 8, position.y - 3 - 8, 16 + 2*8, 19 + 2*8};
            break;
        case BLENDER:
            // 23 x 21
            a.hit_box = (Rectangle){position.x - 8, position.y -5 - 8, 23 + 2*8, 21 + 2*8};
            break;
        case TELEVISION:
            // 13 x 20
            a.hit_box = (Rectangle){position.x - 8, position.y - 4 - 8, 13 + 2*8, 20 + 2*8};
            break;
        case LAMP:
            // 24 x 28
            a.hit_box = (Rectangle){position.x - 8, position.y - 12 - 8, 24 + 2*8, 28 + 2*8};
            break;
        case CLOCK:
            // 33x33
            a.hit_box = (Rectangle){position.x - 8, position.y - 17 - 8, 33 + 2*8, 33 + 2*8+16};
            break;
    }
    a.connected = false;
    a.animationStage = 1;
    a.animationFrameCount = 0;
    a.animationCurrentFrame = 0;
    return a;
}

ApplianceList createApplianceList() {
    ApplianceList a_list;
    a_list.size = 0;
    a_list.capacity = APPLIANCE_LIST_CAPACITY;
    a_list.appliances = malloc(a_list.capacity);
    return a_list;
}


void addApplianceToList(ApplianceList* a_list, Appliance a) {
    a_list->appliances[a_list->size] = a;
    a_list->size += 1;
}

Appliance* getApplianceFromList(ApplianceList* a_list, size_t index) {
    return &a_list->appliances[index];
}

void clearApplianceList(ApplianceList* a_list) { a_list->size = 0; }

void destroyApplianceList(ApplianceList* a_list) {
    free(a_list->appliances);
}

void renderAppliances(ApplianceList* a_list) {
    for (int i = 0; i < a_list->size; i++) {
        Appliance* a = getApplianceFromList(a_list, i);
        switch (a->type) {
            case WASHING_MACHINE:
            {

                if (a->connected) {
                    const int animationFrames = 23;

                    renderAnimation(getSpriteFromID(SPRITE_WASHING_MACHINE_ON_ID), a->hit_box.x + 8, a->hit_box.y + 8, animationFrames, 6, &a->animationFrameCount, &a->animationCurrentFrame);
                } else {
                    DrawTexture(getSpriteFromID(SPRITE_WASHING_MACHINE_OFF_ID), a->hit_box.x + 8, a->hit_box.y + 8, WHITE);
                }
                break;
            }
            case BLENDER:
            {
                const int stage1frames = 20;
                const int stage2frames = 8;
                if (a->connected) {
                    if (a->animationStage == 1) {
                        renderAnimation(getSpriteFromID(SPRITE_BLENDER_ON_STAGE1_ID), a->hit_box.x + 8, a->hit_box.y + 8, stage1frames, 6, &a->animationFrameCount, &a->animationCurrentFrame);
                        if (a->animationCurrentFrame == stage1frames-1) a->animationStage = 2;
                    }
                    else {
                        renderAnimation(getSpriteFromID(SPRITE_BLENDER_ON_STAGE2_ID), a->hit_box.x + 8, a->hit_box.y + 8, stage2frames, 6, &a->animationFrameCount, &a->animationCurrentFrame);
                    }
                } else {
                    if (a->animationStage == 1) {
                        DrawTexture(getSpriteFromID(SPRITE_BLENDER_OFF_STAGE1_ID), a->hit_box.x + 8, a->hit_box.y + 8, WHITE);
                    }
                    else {
                        DrawTexture(getSpriteFromID(SPRITE_BLENDER_OFF_STAGE2_ID), a->hit_box.x + 8, a->hit_box.y + 8, WHITE);
                    }
                }
                break;
            }
            case TELEVISION:
            {
                if (a->connected) {
                    int animationFrames = 27;
                    renderAnimation(getSpriteFromID(SPRITE_TELEVISION_ON_ID), a->hit_box.x + 8, a->hit_box.y + 8, animationFrames, 6, &a->animationFrameCount, &a->animationCurrentFrame);
                } else {
                    DrawTexture(getSpriteFromID(SPRITE_TELEVISION_OFF_ID), a->hit_box.x + 8, a->hit_box.y + 8, WHITE);
                }
                break;
            }
            case LAMP:
            {
                if (a->connected) {
                    if (a->animationStage == 1) {
                        int animationFrames = 10;
                        renderAnimation(getSpriteFromID(SPRITE_LAMP_ON_STAGE1_ID), a->hit_box.x + 8, a->hit_box.y + 8, animationFrames, 6, &a->animationFrameCount, &a->animationCurrentFrame);
                        if (a->animationCurrentFrame == animationFrames-1) a->animationStage = 2;
                    }
                    else {
                        DrawTexture(getSpriteFromID(SPRITE_LAMP_ON_STAGE2_ID), a->hit_box.x + 8, a->hit_box.y + 8, WHITE);
                    }
                }
                else {
                    DrawTexture(getSpriteFromID(SPRITE_LAMP_OFF_ID), a->hit_box.x + 8, a->hit_box.y + 8, WHITE);
                }
                break;
            }
            case CLOCK:
            {
                if (a->connected) {
                    int animationFrames = 8;
                        renderAnimation(getSpriteFromID(SPRITE_CLOCK_ON_ID), a->hit_box.x + 8, a->hit_box.y + 8, animationFrames, 6, &a->animationFrameCount, &a->animationCurrentFrame);
                }
                else {
                    DrawTexture(getSpriteFromID(SPRITE_CLOCK_OFF_ID), a->hit_box.x + 8, a->hit_box.y + 8, WHITE);
                }
                break;
            }
        }
    }
}

bool areAllAppliancesConnected(ApplianceList* a_list) {
    for (size_t i = 0; i < a_list->size; i++) {
        Appliance* a = getApplianceFromList(a_list, i);
        if (!a->connected) return false;
    }
    return true;
}
