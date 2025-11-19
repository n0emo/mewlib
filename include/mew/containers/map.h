#ifndef MEW_INCLUDE_MEW_CONTAINERS_MAP_H_
#define MEW_INCLUDE_MEW_CONTAINERS_MAP_H_

#include <mew/core.h>

typedef bool(mew_map_iter_t)(const void *key, const void *value, void *user_data);

typedef void(mew_map_destroy_t)(void *map);
typedef void(mew_map_insert_t)(void *map, const void *key, const void *value);
typedef bool(mew_map_pop_t)(void *map, const void *key, void *found_key, void *value);
typedef void *(mew_map_get_t)(void *map, const void *key);
typedef bool(mew_map_iterate_t)(void *map, mew_map_iter_t iter, void *user_Data);
typedef usize (mew_map_count_t)(void *map);

typedef struct MewMapFuncTable {
    mew_map_destroy_t *destroy;
    mew_map_insert_t *insert;
    mew_map_pop_t *pop;
    mew_map_get_t *get;
    mew_map_iterate_t *iterate;
    mew_map_count_t *count;
} MewMapFuncTable;

typedef struct MewMap {
    void *data;
    MewMapFuncTable *funcs;
} MewMap;

void mew_map_destroy(MewMap map);
void mew_map_insert(MewMap map, const void *key, const void *value);
bool mew_map_pop(MewMap map, const void *key, void *found_key, void *value);
void *mew_map_get(MewMap map, const void *key);
bool mew_map_iterate(MewMap map, mew_map_iter_t iter, void *user_data);
usize mew_map_count(MewMap map);

#endif // MEW_INCLUDE_MEW_CONTAINERS_MAP_H_
