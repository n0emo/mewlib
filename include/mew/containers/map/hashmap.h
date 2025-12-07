#ifndef MEW_INCLUDE_MEW_CONTAINERS_MAP_HASHMAP_H_
#define MEW_INCLUDE_MEW_CONTAINERS_MAP_HASHMAP_H_

#include <mew/containers/map.h>

typedef uint64_t hashfunc_t(const void *value, void *user_data);
typedef bool hashmap_equals_t(const void *a, const void *b, void *user_data);

typedef struct HashMapBucket {
    bool initialized : 1;
    usize map_index : sizeof(usize) * 8 - 1;
} HashMapBucket;

typedef struct HashMap {
    MewMap base;
    HashMapBucket *buckets;
    size_t bucket_count;
    hashfunc_t *hashfunc;
    hashmap_equals_t *equals;
} HashMap;

typedef struct MewHashMapOptions {
    Allocator alloc;
    void *user_data;
    hashfunc_t *hashfunc;
    hashmap_equals_t *equals;
    size_t key_size;
    size_t value_size;
} MewHashMapOptions;

void hashmap_init(HashMap *map, MewHashMapOptions options);
void hashmap_destroy(HashMap *map);
void hashmap_insert(HashMap *map, const void *key, const void *value);
void *hashmap_get(HashMap *map, const void *key);
bool hashmap_pop(HashMap *map, const void *key, void *found_key, void *value);
bool hashmap_iterate(HashMap *map, MewMapIter iter, void *user_data);

MewMapDestroy hashmap_destroy_overload;
MewMapInsert hashmap_insert_overload;
MewMapPop hashmap_pop_overload;
MewMapGet hashmap_get_overload;
MewMapIterate hashmap_iterate_overload;

hashfunc_t hashmap_sv_hash;
hashmap_equals_t hashmap_sv_equals;

#endif // MEW_INCLUDE_MEW_CONTAINERS_MAP_HASHMAP_H_
