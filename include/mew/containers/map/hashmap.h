#ifndef MEW_INCLUDE_MEW_CONTAINERS_MAP_HASHMAP_H_
#define MEW_INCLUDE_MEW_CONTAINERS_MAP_HASHMAP_H_

#include <mew/containers/map.h>

typedef uint64_t hashfunc_t(const void *value, void *user_data);
typedef bool hashmap_equals_t(const void *a, const void *b, void *user_data);

typedef struct HashMapBucket {
    bool initialized : 1;
    usize map_index : sizeof(usize) * 8 - 1;
} MewHashMapBucket;

typedef struct HashMap {
    MewMap base;
    MewHashMapBucket *buckets;
    size_t bucket_count;
    hashfunc_t *hashfunc;
    hashmap_equals_t *equals;
} MewHashMap;

typedef struct MewHashMapOptions {
    Allocator alloc;
    void *user_data;
    hashfunc_t *hashfunc;
    hashmap_equals_t *equals;
    size_t key_size;
    size_t value_size;
} MewHashMapOptions;

void mew_hashmap_init(MewHashMap *map, MewHashMapOptions options);
void mew_hashmap_destroy(MewHashMap *map);
void mew_hashmap_insert(MewHashMap *map, const void *key, const void *value);
void *mew_hashmap_get(MewHashMap *map, const void *key);
bool mew_hashmap_pop(MewHashMap *map, const void *key, void *found_key, void *value);
bool mew_hashmap_iterate(MewHashMap *map, MewMapIter iter, void *user_data);

MewMapDestroy mew_hashmap_destroy_overload;
MewMapInsert mew_hashmap_insert_overload;
MewMapPop mew_hashmap_pop_overload;
MewMapGet mew_hashmap_get_overload;
MewMapIterate mew_hashmap_iterate_overload;

hashfunc_t mew_hashmap_sv_hash;
hashmap_equals_t mew_hashmap_sv_equals;

#endif // MEW_INCLUDE_MEW_CONTAINERS_MAP_HASHMAP_H_
