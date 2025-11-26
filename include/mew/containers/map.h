#ifndef MEW_INCLUDE_MEW_CONTAINERS_MAP_H_
#define MEW_INCLUDE_MEW_CONTAINERS_MAP_H_

#include <mew/core.h>

typedef bool(MewMapIter)(void *key, void *value, void *user_data);

typedef void MewMapDestroy(void *map);
typedef void MewMapInsert(void *map, const void *key, const void *value);
typedef bool MewMapPop(void *map, const void *key, void *found_key, void *value);
typedef void *MewMapGet(void *map, const void *key);
typedef bool MewMapIterate(void *map, MewMapIter iter, void *user_Data);
typedef usize MewMapCount(void *map);

typedef struct MewMapFuncTable {
    MewMapDestroy *destroy;
    MewMapInsert *insert;
    MewMapPop *pop;
    MewMapGet *get;
    MewMapIterate *iterate;
    MewMapCount *count;
} MewMapFuncTable;

typedef struct MewMap {
    void *data;
    MewMapFuncTable *funcs;
} MewMap;

void mew_map_destroy(MewMap map);
void mew_map_insert(MewMap map, const void *key, const void *value);
bool mew_map_pop(MewMap map, const void *key, void *found_key, void *value);
void *mew_map_get(MewMap map, const void *key);
bool mew_map_iterate(MewMap map, MewMapIter iter, void *user_data);
usize mew_map_count(MewMap map);

#endif // MEW_INCLUDE_MEW_CONTAINERS_MAP_H_
