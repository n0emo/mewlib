#ifndef MEW_INCLUDE_MEW_CONTAINERS_MAP_HASHMAP_H_
#define MEW_INCLUDE_MEW_CONTAINERS_MAP_HASHMAP_H_

#include <mew/containers/map.h>

typedef uint64_t hashfunc_t(const void *value, void *user_data);
typedef bool hashmap_equals_t(const void *a, const void *b, void *user_data);

typedef struct HashMapBucket {
    usize map_index;
    bool initialized;
    char _padding[7];
    char data[];
} HashMapBucket;

typedef struct HashMap {
    Allocator alloc;
    hashfunc_t *hashfunc;
    hashmap_equals_t *equals;
    void *user_data;
    size_t key_size;
    size_t value_size;
    size_t bucket_count;
    size_t element_count;
    HashMapBucket *buckets;
} HashMap;

void hashmap_init(
    HashMap *map,
    void *user_data,
    hashfunc_t *hashfunc,
    hashmap_equals_t *equals,
    size_t key_size,
    size_t value_size
);
void hashmap_destroy(void *map);
void hashmap_insert(void *map, const void *key, const void *value);
bool hashmap_pop(void *map, const void *key, void *found_key, void *value);
void *hashmap_get(void *map, const void *key);
bool hashmap_iterate(void *map, mew_map_iter_t iter, void *user_data);
usize hashmap_count(void *map);

MewMap mew_map_from_hashmap(HashMap *map);

hashfunc_t hashmap_sv_hash;
hashmap_equals_t hashmap_sv_equals;

#endif // MEW_INCLUDE_MEW_CONTAINERS_MAP_HASHMAP_H_
