#include <mew/containers/map/hashmap.h>

#include <string.h>

#include <mew/core.h>

#define HASHMAP_INITIAL_BUCKETS 16

static inline HashMapBucket *get_bucket_for_index(HashMap *map, size_t index);
static inline char *get_bucket_key(HashMapBucket *bucket);
static inline char *get_bucket_value(HashMap *map, HashMapBucket *bucket);
HashMapBucket *get_bucket_for_key(HashMap *map, const void *key);
void alloc_buckets(HashMap *map, size_t count);
void hashmap_expand(HashMap *map);
size_t bucket_size(HashMap *map);
bool map_equals(HashMap *map, const void *a, const void *b);

void hashmap_destroy_overload(MewMap *map) {
    hashmap_destroy((HashMap *)map);
}

void hashmap_insert_overload(MewMap *map, const void *key, const void *value) {
    hashmap_insert((HashMap *)map, key, value);
}

bool hashmap_pop_overload(MewMap *map, const void *key, void *found_key, void *value) {
    return hashmap_pop((HashMap *)map, key, found_key, value);
}

void *hashmap_get_overload(MewMap *map, const void *key) {
    return hashmap_get((HashMap *)map, key);
}

bool hashmap_iterate_overload(MewMap *map, MewMapIter iter, void *user_data) {
    return hashmap_iterate((HashMap *)map, iter, user_data);
}

void hashmap_init(HashMap *map, MewHashMapOptions options) {
    memset(map, 0, sizeof(*map));

    MewMapBaseOptions base_options = {
        .alloc = new_malloc_allocator(),
        .funcs =
            (MewMapFuncTable) {
                .destroy = &hashmap_destroy_overload,
                .insert = &hashmap_insert_overload,
                .pop = &hashmap_pop_overload,
                .get = &hashmap_get_overload,
                .iterate = &hashmap_iterate_overload,
            },
        .key_size = options.key_size,
        .value_size = options.value_size,
        .user_data = options.user_data,
    };
    mew_map_base_init(&map->base, base_options);

    map->hashfunc = options.hashfunc;
    map->equals = options.equals;
    alloc_buckets(map, HASHMAP_INITIAL_BUCKETS);
}

void hashmap_destroy(HashMap *map) {
    mem_free(map->base.alloc, map->buckets);
}

void hashmap_insert(HashMap *map, const void *key, const void *value) {
    if ((float)map->base.count / (float)map->bucket_count >= 0.75) {
        hashmap_expand(map);
    }

    map->base.count++;

    uint64_t hash = map->hashfunc((void *)key, map->base.user_data);
    size_t index = hash % map->bucket_count;
    size_t map_index = index;

    while (true) {
        HashMapBucket *bucket = get_bucket_for_index(map, index);

        if (!bucket->initialized) {
            // TODO: Robin-hood hashing
            bucket->map_index = map_index;
            memcpy(get_bucket_key(bucket), key, map->base.key_size);
            memcpy(get_bucket_value(map, bucket), value, map->base.value_size);
            bucket->initialized = true;
            break;
        } else if (map_equals(map, get_bucket_key(bucket), key)) {
            memcpy(get_bucket_value(map, bucket), value, map->base.value_size);
            break;
        }

        index++;

        if (index >= map->bucket_count) {
            index = 0;
        }
    }
}

void *hashmap_get(HashMap *map, const void *key) {
    HashMapBucket *bucket = get_bucket_for_key(map, key);
    if (bucket == NULL) {
        return NULL;
    } else {
        return (char *)bucket + sizeof(HashMapBucket) + map->base.key_size;
    }
}

bool hashmap_pop(HashMap *map, const void *key, void *found_key, void *value) {
    HashMapBucket *bucket = get_bucket_for_key(map, key);
    if (bucket == NULL) {
        return false;
    }

    bucket->initialized = false;
    if (found_key != NULL) {
        char *key_ptr = found_key;
        memcpy(key_ptr, get_bucket_key(bucket), map->base.key_size);
    }
    if (value != NULL) {
        char *value_ptr = value;
        memcpy(value_ptr, get_bucket_value(map, bucket), map->base.value_size);
    }
    return true;
}

bool hashmap_iterate(HashMap *map, MewMapIter iter, void *user_data) {
    for (size_t i = 0; i < map->bucket_count; i++) {
        HashMapBucket *bucket = get_bucket_for_index(map, i);
        if (!bucket->initialized)
            continue;
        if (!iter(get_bucket_key(bucket), get_bucket_value(map, bucket), user_data))
            return false;
    }
    return true;
}

static inline HashMapBucket *get_bucket_for_index(HashMap *map, size_t index) {
    size_t size = bucket_size(map);
    return (HashMapBucket *)(((char *)map->buckets) + size * index);
}

static inline char *get_bucket_key(HashMapBucket *bucket) {
    return (char *)bucket + sizeof(HashMapBucket);
}

static inline char *get_bucket_value(HashMap *map, HashMapBucket *bucket) {
    return (char *)bucket + sizeof(HashMapBucket) + map->base.key_size;
}

HashMapBucket *get_bucket_for_key(HashMap *map, const void *key) {
    uint64_t hash = map->hashfunc((void *)key, map->base.user_data);
    size_t index = hash % map->bucket_count;
    size_t map_index = index;
    while (true) {
        HashMapBucket *bucket = get_bucket_for_index(map, index);
        if (!bucket->initialized) {
            return NULL;
        }
        if (bucket->map_index == map_index && map_equals(map, get_bucket_key(bucket), key)) {
            return bucket;
        }
        index++;
        if (index >= map->bucket_count) {
            index = 0;
        }
    }
}

void alloc_buckets(HashMap *map, size_t count) {
    map->bucket_count = count;
    size_t size = bucket_size(map);
    map->buckets = mem_alloc(map->base.alloc, size * count);
    memset(map->buckets, 0, size * count);
}

void hashmap_expand(HashMap *map) {
    HashMapBucket *old_buckets = map->buckets;
    size_t old_count = map->bucket_count;
    size_t size = bucket_size(map);
    alloc_buckets(map, old_count * 2);
    map->base.count = 0;
    for (size_t i = 0; i < old_count; i++) {
        HashMapBucket *bucket = (HashMapBucket *)((char *)old_buckets + size * i);
        if (bucket->initialized) {
            hashmap_insert(map, get_bucket_key(bucket), get_bucket_value(map, bucket));
        }
    }
    mem_free(map->base.alloc, old_buckets);
}

size_t bucket_size(HashMap *map) {
    return sizeof(HashMapBucket) + map->base.key_size + map->base.value_size;
}

bool map_equals(HashMap *map, const void *a, const void *b) {
    return (map->equals != NULL && map->equals(a, b, map->base.user_data)) || memcmp(a, b, map->base.key_size) == 0;
}

// djb2 hash algorithm
uint64_t hashmap_sv_hash(const void *value, void *user_data) {
    (void)user_data;
    const StringView *sv = (const StringView *)value;
    uint64_t hash = 5381;
    for (size_t i = 0; i < sv->count; i++) {
        hash = ((hash << 5) + hash) + (uint64_t)sv->items[i];
    }
    return hash;
}

bool hashmap_sv_equals(const void *a, const void *b, void *user_data) {
    (void)user_data;
    const StringView *sva = (const StringView *)a;
    const StringView *svb = (const StringView *)b;
    return sv_eq_sv(*sva, *svb);
}
