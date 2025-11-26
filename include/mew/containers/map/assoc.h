#ifndef MEW_CONTAINERS_MAP_ASSOC_H_
#define MEW_CONTAINERS_MAP_ASSOC_H_

#include <mew/containers/map.h>
#include <mew/core/alloc.h>
#include <mew/core/types.h>

typedef bool MewAssocEquals(const void *a, const void *b, void *user_data);

typedef struct MewAssocMap {
    Allocator alloc;
    char *data;
    usize key_size;
    usize value_size;
    usize count;
    usize capacity;
    void *user_data;
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
MewMap mew_map_from_assocmap(MewAssocMap *map);

MewMapDestroy mew_assocmap_destroy;
MewMapInsert mew_assocmap_insert;
MewMapPop mew_assocmap_pop;
MewMapGet mew_assocmap_get;
MewMapIterate mew_assocmap_iterate;
MewMapCount mew_assocmap_count;

#endif //  MEW_CONTAINERS_MAP_ASSOC_H_
