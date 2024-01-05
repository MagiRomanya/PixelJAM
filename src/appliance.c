#include "appliance.h"
#include "cable.h"
#include "raylib.h"
#include "sprite_manager.h"
#include "ultilities.h"
#include <stddef.h>

#define APPLIANCE_LIST_CAPACITY 10*sizeof(Appliance)

ApplianceList createApplianceList() {
    ApplianceList a_list;
    a_list.size = 0;
    a_list.capacity = APPLIANCE_LIST_CAPACITY;
    a_list.appliances = malloc(a_list.capacity);
    return a_list;
}


void addApplianceToList(ApplianceList* a_list, Appliance* a) {
    a_list->appliances[sizeof(Appliance)*a_list->size] = *a;
    a_list->size += 1;
}

Appliance* getApplianceFromList(ApplianceList* a_list, size_t index) {
    return &a_list->appliances[sizeof(Appliance) * index];
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
                    static int frameCounter = 0;
                    static int currentFrame = 0;
                    renderAnimation(getSpriteFromID(SPRITE_WASHING_MACHINE_ON_ID), a->hit_box.x + 8, a->hit_box.y + 8, 23, 6, &frameCounter, &currentFrame);
                } else {
                    /* DrawRectangleRec(a->hit_box, BLACK); */
                    DrawTexture(getSpriteFromID(SPRITE_WASHING_MACHINE_OFF_ID), a->hit_box.x + 8, a->hit_box.y + 8, WHITE);
                }
                break;
            }
            case BLENDER:
            {
                if (a->connected) {
                    static int frameCounter = 0;
                    static int currentFrame = 0;
                    renderAnimation(getSpriteFromID(SPRITE_BLENDER_STAGE1_ID), a->hit_box.x + 8, a->hit_box.y + 8, 23, 6, &frameCounter, &currentFrame);
                } else {
                    DrawTexture(getSpriteFromID(SPRITE_BLENDER_OFF_ID), a->hit_box.x + 8, a->hit_box.y + 8, WHITE);
                }
                break;
            }
            case TV:
            {
                if (a->connected) {
                    static int frameCounter = 0;
                    static int currentFrame = 0;
                    renderAnimation(getSpriteFromID(SPRITE_TELEVISION_ON_ID), a->hit_box.x + 8, a->hit_box.y + 8, 23, 6, &frameCounter, &currentFrame);
                } else {
                    DrawTexture(getSpriteFromID(SPRITE_TELEVISION_OFF_ID), a->hit_box.x + 8, a->hit_box.y + 8, WHITE);
                }
                break;
            }
        }
    }
}


bool areAllAppliancesConnected(ApplianceList* a_list) {
    for (size_t i = 0; i < a_list->size; i++) {
        Appliance* a = getApplianceFromList(a_list, i);
        if (a->connected) return true;
    }
    return false;
}
