#include <mew/containers/vector.h>

#include <assert.h>
#include <string.h>

#include <mew/core.h>

#define MEW_VEC_INITIAL_CAP 128

static inline size_t mew_vec_new_cap(MewVector *vec) {
    return vec->capacity == 0 ? MEW_VEC_INITIAL_CAP : vec->capacity * 2;
}

void mew_vec_init(MewVector *vec, Allocator alloc, size_t element_size) {
    assert(vec != NULL);
    assert(element_size != 0);

    memset(vec, 0, sizeof(*vec));
    vec->alloc = alloc;
    vec->element_size = element_size;
}

void mew_vec_reserve(MewVector *vec, size_t new_capacity) {
    assert(vec != NULL);
    assert(new_capacity != 0);

    if (new_capacity <= vec->capacity)
        return;

    if (vec->data == NULL) {
        vec->data = mem_alloc(vec->alloc, new_capacity * vec->element_size);
    } else {
        vec->data =
            mem_realloc(vec->alloc, vec->data, vec->capacity * vec->element_size, new_capacity * vec->element_size);
    }

    vec->capacity = new_capacity;
}

char *mew_vec_begin(const MewVector *vec) {
    assert(vec != NULL);

    return vec->data;
}

char *mew_vec_end(const MewVector *vec) {
    assert(vec != NULL);

    return vec->data + mew_vec_size_bytes(vec);
}

usize mew_vec_size_bytes(const MewVector *vec) {
    assert(vec != NULL);

    return vec->count * vec->element_size;
}

void mew_vec_destroy(MewVector *vec) {
    assert(vec != NULL);

    if (vec->data == NULL)
        return;

    mem_free(vec->alloc, vec->data);
}

void *mew_vec_get(MewVector *vec, size_t index) {
    assert(vec != NULL);

    if (index >= vec->count) {
        return NULL;
    }

    return vec->data + index * vec->element_size;
}

void mew_vec_push(MewVector *vec, const void *element) {
    assert(vec != NULL);
    assert(element != NULL);

    if (vec->count == vec->capacity) {
        mew_vec_reserve(vec, mew_vec_new_cap(vec));
    }

    char *ptr = vec->data;
    ptr += vec->count * vec->element_size;
    memcpy(ptr, element, vec->element_size);
    vec->count++;
}

#include <stdio.h>

void mew_vec_insert_at(MewVector *vec, const void *element, size_t index) {
    assert(vec != NULL);
    assert(element != NULL);
    assert(index <= vec->count);

    if (index == vec->count) {
        mew_vec_push(vec, element);
        return;
    }

    if (vec->count == vec->capacity) {
        mew_vec_reserve(vec, mew_vec_new_cap(vec));
    }

    for (ptrdiff_t i = (ptrdiff_t)vec->count; i > (ptrdiff_t)index; i--) {
        char *ptr = vec->data;
        ptr += (ptrdiff_t)vec->element_size * i;
        memcpy(ptr, ptr - vec->element_size, vec->element_size);
    }

    char *ptr = vec->data;
    ptr += vec->element_size * index;
    memcpy(ptr, element, vec->element_size);
    vec->count++;
}

void mew_vec_delete_at(MewVector *vec, size_t index) {
    assert(vec != NULL);

    if (index >= vec->count)
        return;

    vec->count--;

    for (size_t i = index; i < vec->count; i++) {
        char *ptr = vec->data;
        ptr += vec->element_size * i;
        memcpy(ptr, ptr + vec->element_size, vec->element_size);
    }
}

void mew_vec_copy_to(MewVector *dst, const MewVector *src) {
    assert(src != NULL);
    assert(dst != NULL);
    assert(src->element_size == dst->element_size);

    mew_vec_reserve(dst, dst->count + src->count);
    memcpy(mew_vec_end(dst), mew_vec_begin(src), mew_vec_size_bytes(src));
}
