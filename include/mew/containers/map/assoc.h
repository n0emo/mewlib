#ifndef MEW_CONTAINERS_MAP_ASSOC_H_
#define MEW_CONTAINERS_MAP_ASSOC_H_

#include <mew/containers/map.h>
#include <mew/core/alloc.h>
#include <mew/core/types.h>

typedef bool MewAssocEquals(const void *a, const void *b, void *user_data);

typedef struct MewAssocMap {
    MewMap base;
    char *data;
    usize capacity;
    MewAssocEquals *equals;
} MewAssocMap;

typedef struct MewAssocMapOptions {
    Allocator alloc;
    usize key_size;
    usize value_size;
    void *user_data;
    // TODO: make optional
    MewAssocEquals *equals;
} MewAssocMapOptions;

void mew_assocmap_init(MewAssocMap *map, MewAssocMapOptions options);
MewAssocMap mew_assocmap_new(MewAssocMapOptions options);

void mew_assocmap_destroy(MewAssocMap *map);
void mew_assocmap_insert(MewAssocMap *map, const void *key, const void *value);
bool mew_assocmap_pop(MewAssocMap *map, const void *key, void *found_key, void *value);
void *mew_assocmap_get(MewAssocMap *map, const void *key);
bool mew_assocmap_iterate(MewAssocMap *map, MewMapIter iter, void *user_data);

MewMapDestroy mew_assocmap_destroy_overload;
MewMapInsert mew_assocmap_insert_overload;
MewMapPop mew_assocmap_pop_overload;
MewMapGet mew_assocmap_get_overload;
MewMapIterate mew_assocmap_iterate_overload;

#endif //  MEW_CONTAINERS_MAP_ASSOC_H_
