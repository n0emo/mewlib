#include <mew/core/alloc.h>

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void *mem_alloc(Allocator allocator, size_t bytes) {
    return allocator.ftable->alloc(allocator.data, bytes);
}

void mem_free(Allocator allocator, void *ptr) {
    allocator.ftable->free(allocator.data, ptr);
}

void *mem_calloc(Allocator allocator, size_t count, size_t size) {
    return allocator.ftable->calloc(allocator.data, count, size);
}

void *mem_realloc(Allocator allocator, void *ptr, size_t old_size, size_t new_size) {
    if (new_size <= old_size) {
        return ptr;
    }

    return allocator.ftable->realloc(allocator.data, ptr, old_size, new_size);
}

char *mem_sprintf(Allocator allocator, const char *format, ...) {
    va_list args, args_copy;
    va_start(args, format);

    va_copy(args_copy, args);
    int size = vsnprintf(NULL, 0, format, args);
    va_end(args_copy);

    assert(size >= 0);

    char *result = (char *)mem_alloc(allocator, (size_t)size + 1);
    vsnprintf(result, (size_t)size + 1, format, args);
    va_end(args);

    return result;
}

void *mem_memdup(Allocator allocator, const void *mem, size_t size) {
    void *new = mem_alloc(allocator, size);
    memcpy(new, mem, size);
    return new;
}

char *mem_strdup(Allocator allocator, const char *s) {
    size_t len = strlen(s);
    char *new = mem_alloc(allocator, len + 1);
    memcpy(new, s, len);
    new[len] = '\0';
    return new;
}
