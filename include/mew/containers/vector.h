#ifndef MEW_INCLUDE_MEW_CONTAINERS_VECTOR_H_
#define MEW_INCLUDE_MEW_CONTAINERS_VECTOR_H_

// TODO: testing
// TODO: define_for_type

#include <stdbool.h>
#include <stddef.h>

#include "mew/alloc.h"

typedef struct MewVector {
    Allocator alloc;
    char *data;
    size_t capacity;
    size_t count;
    size_t element_size;
} MewVector;

void mew_vec_init(MewVector *vec, Allocator alloc, size_t element_size);

void mew_vec_destroy(MewVector *vec);

void mew_vec_reserve(MewVector *vec, size_t new_capacity);

void *mew_vec_get(MewVector *vec, size_t index);

void mew_vec_push(MewVector *vec, const void *element);

void mew_vec_insert_at(MewVector *vec, const void *element, size_t index);

void mew_vec_delete_at(MewVector *vec, size_t index);

// TODO: more vector functions:
//         - push_range
//         - clear
//         - contains
//         - transform
//         - copy_to
//         - index_of
//         - find
//         - remove
//         - slice
//         - sort

#define mew_vec_define_for_type(type)                                                                                  \
    __attribute__((unused)) typedef struct MewVector_##type {                                                          \
        MewVector inner;                                                                                               \
    } MewVector_##type;                                                                                                \
                                                                                                                       \
    __attribute__((unused)) static inline void mew_vec_##type##_init(MewVector_##type *vec, Allocator alloc) {         \
        mew_vec_init(&vec->inner, alloc, sizeof(type));                                                                \
    }                                                                                                                  \
                                                                                                                       \
    __attribute__((unused)) static inline void mew_vec_##type##_destroy(MewVector_##type *vec) {                       \
        mew_vec_destroy(&vec->inner);                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    __attribute__((unused)) static inline void mew_vec_##type##_reserve(MewVector_##type *vec, size_t new_capacity) {  \
        mew_vec_reserve(&vec->inner, new_capacity);                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    __attribute__((unused)) static inline type *mew_vec_##type##_get_ptr(MewVector_##type *vec, size_t index) {        \
        return (type *)mew_vec_get(&vec->inner, index);                                                                \
    }                                                                                                                  \
                                                                                                                       \
    __attribute__((unused)) static inline type mew_vec_##type##_get(MewVector_##type *vec, size_t index) {             \
        type item;                                                                                                     \
        char *ptr = (char *)mew_vec_get(&vec->inner, index);                                                           \
        char *item_ptr = (char *)&item;                                                                                \
        for (size_t i = 0; i < sizeof(item); i++) {                                                                    \
            item_ptr[i] = ptr[i];                                                                                      \
        }                                                                                                              \
        return item;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    __attribute__((unused)) static inline void mew_vec_##type##_push(MewVector_##type *vec, type element) {            \
        mew_vec_push(&vec->inner, &element);                                                                           \
    }                                                                                                                  \
                                                                                                                       \
    __attribute__((unused)                                                                                             \
    ) static inline void mew_vec_##type##_insert_at(MewVector_##type *vec, type element, size_t index) {               \
        mew_vec_insert_at(&vec->inner, &element, index);                                                               \
    }                                                                                                                  \
                                                                                                                       \
    __attribute__((unused)) static inline void mew_vec_##type##_delete_at(MewVector_##type *vec, size_t index) {       \
        mew_vec_delete_at(&vec->inner, index);                                                                         \
    }

#endif // MEW_INCLUDE_MEW_CONTAINERS_VECTOR_H_
