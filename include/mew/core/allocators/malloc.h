#ifndef INCLUDE_MEW_CORE_ALLOCATORS_MALLOC_H_
#define INCLUDE_MEW_CORE_ALLOCATORS_MALLOC_H_

#include <mew/core/alloc.h>

allocator_alloc_t malloc_alloc;
allocator_free_t malloc_free;
allocator_calloc_t malloc_calloc;
allocator_realloc_t malloc_realloc;
Allocator new_malloc_allocator(void);

static const AllocatorFunctionTable malloc_table = {
    malloc_alloc,
    malloc_free,
    malloc_calloc,
    malloc_realloc,
};

#endif // INCLUDE_MEW_CORE_ALLOCATORS_MALLOC_H_
