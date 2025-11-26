#include <mew/core/allocators/malloc.h>

#include <stdlib.h>

void *malloc_alloc(void *data, size_t bytes) {
    (void)data;
    return malloc(bytes);
}

void malloc_free(void *data, void *ptr) {
    (void)data;
    free(ptr);
}

void *malloc_calloc(void *data, size_t count, size_t size) {
    (void)data;
    return calloc(count, size);
}

void *malloc_realloc(void *data, void *ptr, size_t old_size, size_t new_size) {
    (void)data;
    (void)old_size;

    return realloc(ptr, new_size);
}

Allocator new_malloc_allocator(void) {
    return (Allocator) {
        .data = NULL,
        .ftable = &malloc_table,
    };
}
