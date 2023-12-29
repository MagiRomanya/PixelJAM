#include "memory_arena.h"

MemoryArena InitializeMemoryArena(size_t total_capacity) {
    MemoryArena arena = {0};
    arena.size = 0;
    arena.data = malloc(total_capacity);
    if (!arena.data) {
        printf("Error while initializing arena, buy more RAM lol.");
        exit(1);
    }
    arena.capacity = total_capacity;
    return arena;
}

void DestroyMemoryArena(MemoryArena* arena) { free(arena->data); }

void* arena_alloc(MemoryArena* arena, size_t size) {
    if (arena->size + size > arena->capacity) {
        printf("Not enough memory in the ARENA! :(");
        return NULL;
    }

    void* ptr = (char*)(arena->data) + size;
    arena->size += size;

    return ptr;
}

void arena_reset(MemoryArena* arena) { arena->size = 0; }
