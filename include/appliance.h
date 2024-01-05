#ifndef APPLIANCE_H_
#define APPLIANCE_H_

#include <stddef.h>
#include "raylib.h"
#include "entity.h"
#include "sprite_manager.h"
#include "ultilities.h"

typedef enum {
    WASHING_MACHINE,
    BLENDER,
    TV
} ApplianceType;

typedef struct {
    ApplianceType type;
    Rectangle hit_box;
    bool connected;
} Appliance;

typedef struct{
    Appliance* appliances;
    size_t size;
    size_t capacity;
} ApplianceList;

ApplianceList createApplianceList();

void addApplianceToList(ApplianceList* a_list, Appliance* a);

Appliance* getApplianceFromList(ApplianceList* a_list, size_t index);

void clearApplianceList(ApplianceList* a_list);

void destroyApplianceList(ApplianceList* a_list);

void renderAppliances(ApplianceList* a_list);

#endif