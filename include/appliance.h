#ifndef APPLIANCE_H_
#define APPLIANCE_H_

#include <stddef.h>
#include "raylib.h"
#include "entity.h"
#include "sprite_manager.h"

typedef enum {
    WASHING_MACHINE,
    BLENDER,
    TELEVISION,
    LAMP,
    CLOCK,
} ApplianceType;

typedef struct {
    ApplianceType type;
    Rectangle hit_box;
    bool connected;
    int animationStage;
    int animationFrameCount;
    int animationCurrentFrame;
} Appliance;

typedef struct{
    Appliance* appliances;
    size_t size;
    size_t capacity;
} ApplianceList;

Appliance createAppliance(ApplianceType type, Vector2 position);

ApplianceList createApplianceList();

void addApplianceToList(ApplianceList* a_list, Appliance a);

Appliance* getApplianceFromList(ApplianceList* a_list, size_t index);

void clearApplianceList(ApplianceList* a_list);

void destroyApplianceList(ApplianceList* a_list);

void renderAppliances(ApplianceList* a_list);

bool areAllAppliancesConnected(ApplianceList* a_list);

#endif
