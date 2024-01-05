#include "appliance.h"

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
