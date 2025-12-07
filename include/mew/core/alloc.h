#ifndef INCLUDE_MEW_CORE_ALLOC_H_
#define INCLUDE_MEW_CORE_ALLOC_H_

#include <mew/core/types.h>
#include <mew/core/os/macros.h>

typedef void *allocator_alloc_t(void *data, usize bytes);
typedef void allocator_free_t(void *data, void *ptr);
typedef void *allocator_calloc_t(void *data, usize count, usize size);
typedef void *allocator_realloc_t(void *data, void *ptr, usize old_size, usize new_size);

typedef struct {
    allocator_alloc_t *const alloc;
    allocator_free_t *const free;
    allocator_calloc_t *const calloc;
    allocator_realloc_t *const realloc;
} AllocatorFunctionTable;

typedef struct Allocator {
    void *data;
    const AllocatorFunctionTable *ftable;
} Allocator;

void *mem_alloc(Allocator allocator, usize bytes);

void mem_free(Allocator allocator, void *ptr);

void *mem_calloc(Allocator allocator, usize count, usize size);

void *mem_realloc(Allocator allocator, void *ptr, usize old_size, usize new_size);

char *mem_sprintf(Allocator allocator, MEW_FORMAT_STRING(const char *format), ...)
#ifdef __GNUC__
    __attribute__((format(printf, 2, 3)))
#endif
    ;

void *mem_memdup(Allocator allocator, const void *mem, usize size);

char *mem_strdup(Allocator allocator, const char *s);

#endif // INCLUDE_MEW_CORE_ALLOC_H_
