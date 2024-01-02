#include "entity.h"

#define ENTITY_LIST_CAPACITY 1e3*sizeof(Entity)

EntityList createEntityList() {
    EntityList e_list;
    e_list.size = 0;
    e_list.capacity = ENTITY_LIST_CAPACITY;
    e_list.entities = malloc(e_list.capacity);
    return e_list;
}

void addEntityToList(EntityList* e_list, Entity* e) {
    e_list->entities[sizeof(Entity)*e_list->size] = *e;
    e_list->size += sizeof(Entity);
}

Entity* getEntityFromList(EntityList* e_list, size_t index) {
    return &e_list->entities[sizeof(Entity) * index];
}

void clearEntityList(EntityList* e_list) { e_list->size = 0; }

void destroyEntityList(EntityList* e_list) {
    free(e_list->entities);
}
