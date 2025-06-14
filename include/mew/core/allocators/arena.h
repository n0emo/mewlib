#ifndef INCLUDE_MEW_CORE_ALLOCATORS_ARENA_H_
#define INCLUDE_MEW_CORE_ALLOCATORS_ARENA_H_

#include <mew/core/alloc.h>

#define REGION_DEFAULT_CAPACITY (8 * 1024)

typedef struct Region Region;

struct Region {
    Region *next;
    usize count;
    usize capacity;
    usize data[];
};

typedef struct {
    Region *begin;
    Region *end;
} Arena;

allocator_alloc_t arena_alloc;
allocator_free_t arena_free;
allocator_calloc_t arena_calloc;
allocator_realloc_t arena_realloc;
Allocator new_arena_allocator(Arena *arena);

static const AllocatorFunctionTable arena_table = {
    arena_alloc,
    arena_free,
    arena_calloc,
    arena_realloc,
};

void arena_free_arena(Arena *arena);
Region *new_region(usize capacity);
void free_region(Region *region);

#endif // INCLUDE_MEW_CORE_ALLOCATORS_ARENA_H_
