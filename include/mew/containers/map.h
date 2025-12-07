#ifndef MEW_INCLUDE_MEW_CONTAINERS_MAP_H_
#define MEW_INCLUDE_MEW_CONTAINERS_MAP_H_

#include <mew/core.h>

#include <assert.h>

typedef bool(MewMapIter)(void *key, void *value, void *user_data);

typedef struct MewMap MewMap;

typedef void MewMapDestroy(MewMap *map);
typedef void MewMapInsert(MewMap *map, const void *key, const void *value);
typedef bool MewMapPop(MewMap *map, const void *key, void *found_key, void *value);
typedef void *MewMapGet(MewMap *map, const void *key);
typedef bool MewMapIterate(MewMap *map, MewMapIter iter, void *user_data);

typedef struct MewMapFuncTable {
    MewMapDestroy *destroy;
    MewMapInsert *insert;
    MewMapPop *pop;
    MewMapGet *get;
    MewMapIterate *iterate;
} MewMapFuncTable;

typedef struct MewMap {
    Allocator alloc;
    MewMapFuncTable funcs;
    usize key_size;
    usize value_size;
    usize count;
    void *user_data;
} MewMap;

typedef struct MewMapBaseOptions {
    Allocator alloc;
    MewMapFuncTable funcs;
    usize key_size;
    usize value_size;
    void *user_data;
} MewMapBaseOptions;

void mew_map_base_init(MewMap *base, MewMapBaseOptions options);

void mew_map_destroy(MewMap *map);
void mew_map_insert(MewMap *map, const void *key, const void *value);
bool mew_map_pop(MewMap *map, const void *key, void *found_key, void *value);
void *mew_map_get(MewMap *map, const void *key);
bool mew_map_iterate(MewMap *map, MewMapIter iter, void *user_data);

#define mew_t_map_destroy(K, V, map)                                                                                   \
    do {                                                                                                               \
        /* TODO: assert key and value size (or even type ID) */                                                        \
        mew_map_destroy((MewMap *)map);                                                                                \
    } while (0)

#define mew_t_map_insert(K, V, map, key, value)                                                                        \
    do {                                                                                                               \
        /* TODO: assert key and value size (or even type ID) */                                                        \
        K mew_t_map_insert_key = key;                                                                                  \
        V mew_t_map_insert_value = value;                                                                              \
        mew_map_insert((MewMap *)map, &mew_t_map_insert_key, &mew_t_map_insert_value);                                 \
    } while (0)

#define mew_t_map_pop(K, V, map, key, found_key, value, ok)                                                            \
    do {                                                                                                               \
        /* TODO: assert key and value size (or even type ID) */                                                        \
        K mew_t_map_pop_key = key;                                                                                     \
        K *mew_t_map_pop_found_key = found_key;                                                                        \
        V *mew_t_map_pop_value = value;                                                                                \
        *(ok) = mew_map_pop(                                                                                           \
            (MewMap *)map,                                                                                             \
            &mew_t_map_pop_key,                                                                                        \
            (void *)mew_t_map_pop_found_key,                                                                           \
            (void *)mew_t_map_pop_value                                                                                \
        );                                                                                                             \
    } while (0)

#define mew_t_map_get(K, V, map, key, result)                                                                          \
    do {                                                                                                               \
        /* TODO: assert key and value size (or even type ID) */                                                        \
        K mew_t_map_get_key = key;                                                                                     \
        result = (V *)mew_map_get((MewMap *)map, &mew_t_map_get_key);                                                  \
    } while (0)

#define mew_t_map_iterate(K, V, map, iter, user_data, ok)                                                              \
    do {                                                                                                               \
        /* TODO: assert key and value size (or even type ID) */                                                        \
        *(ok) = mew_map_iterate((MewMap *)map, iter, user_data);                                                       \
    } while (0)

#define mew_t_map_count(T, map, result)                                                                                \
    do {                                                                                                               \
        /* TODO: assert key and value size (or even type ID) */                                                        \
        *result = mew_map_count((MewMap *)map);                                                                        \
    } while (0)

#endif // MEW_INCLUDE_MEW_CONTAINERS_MAP_H_
