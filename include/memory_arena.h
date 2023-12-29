#ifndef MEMORY_ARENA_H_
#define MEMORY_ARENA_H_
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t size;
    size_t capacity;
    void* data;
} MemoryArena;

MemoryArena InitializeMemoryArena(size_t total_capacity);

void DestroyMemoryArena(MemoryArena* arena);

void* arena_alloc(MemoryArena* arena, size_t size);

void arena_reset(MemoryArena* arena);

#endif // MEMORY_ARENA_H_
