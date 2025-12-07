#include <mew/containers/map/hashmap.h>

#include <string.h>

#include <mew/core.h>

#define HASHMAP_INITIAL_BUCKETS 16

static inline MewHashMapBucket *mew_hashmap_bucket_for_index(MewHashMap *map, size_t index);
static inline char *mew_hashmap_bucket_key(MewHashMapBucket *bucket);
static inline char *mew_hashmap_bucket_value(MewHashMap *map, MewHashMapBucket *bucket);
static inline MewHashMapBucket *mew_hashmap_bucket_for_key(MewHashMap *map, const void *key);
static inline void mew_hashmap_alloc_buckets(MewHashMap *map, size_t count);
static inline void mew_hashmap_expand(MewHashMap *map);
static inline size_t mew_hashmap_bucket_size(MewHashMap *map);
static inline bool mew_hashmap_equals(MewHashMap *map, const void *a, const void *b);

void mew_hashmap_destroy_overload(MewMap *map) {
    mew_hashmap_destroy((MewHashMap *)map);
}

void mew_hashmap_insert_overload(MewMap *map, const void *key, const void *value) {
    mew_hashmap_insert((MewHashMap *)map, key, value);
}

bool mew_hashmap_pop_overload(MewMap *map, const void *key, void *found_key, void *value) {
    return mew_hashmap_pop((MewHashMap *)map, key, found_key, value);
}

void *mew_hashmap_get_overload(MewMap *map, const void *key) {
    return mew_hashmap_get((MewHashMap *)map, key);
}

bool mew_hashmap_iterate_overload(MewMap *map, MewMapIter iter, void *user_data) {
    return mew_hashmap_iterate((MewHashMap *)map, iter, user_data);
}

void mew_hashmap_init(MewHashMap *map, MewHashMapOptions options) {
    memset(map, 0, sizeof(*map));

    MewMapBaseOptions base_options = {
        .alloc = new_malloc_allocator(),
        .funcs =
            (MewMapFuncTable) {
                .destroy = &mew_hashmap_destroy_overload,
                .insert = &mew_hashmap_insert_overload,
                .pop = &mew_hashmap_pop_overload,
                .get = &mew_hashmap_get_overload,
                .iterate = &mew_hashmap_iterate_overload,
            },
        .key_size = options.key_size,
        .value_size = options.value_size,
        .user_data = options.user_data,
    };
    mew_map_base_init(&map->base, base_options);

    map->hashfunc = options.hashfunc;
    map->equals = options.equals;
    mew_hashmap_alloc_buckets(map, HASHMAP_INITIAL_BUCKETS);
}

void mew_hashmap_destroy(MewHashMap *map) {
    mem_free(map->base.alloc, map->buckets);
}

void mew_hashmap_insert(MewHashMap *map, const void *key, const void *value) {
    if ((float)map->base.count / (float)map->bucket_count >= 0.75) {
        mew_hashmap_expand(map);
    }

    map->base.count++;

    uint64_t hash = map->hashfunc((void *)key, map->base.user_data);
    size_t index = hash % map->bucket_count;
    size_t map_index = index;

    while (true) {
        MewHashMapBucket *bucket = mew_hashmap_bucket_for_index(map, index);

        if (!bucket->initialized) {
            // TODO: Robin-hood hashing
            bucket->map_index = map_index;
            memcpy(mew_hashmap_bucket_key(bucket), key, map->base.key_size);
            memcpy(mew_hashmap_bucket_value(map, bucket), value, map->base.value_size);
            bucket->initialized = true;
            break;
        } else if (mew_hashmap_equals(map, mew_hashmap_bucket_key(bucket), key)) {
            memcpy(mew_hashmap_bucket_value(map, bucket), value, map->base.value_size);
            break;
        }

        index++;

        if (index >= map->bucket_count) {
            index = 0;
        }
    }
}

void *mew_hashmap_get(MewHashMap *map, const void *key) {
    MewHashMapBucket *bucket = mew_hashmap_bucket_for_key(map, key);
    if (bucket == NULL) {
        return NULL;
    } else {
        return (char *)bucket + sizeof(MewHashMapBucket) + map->base.key_size;
    }
}

bool mew_hashmap_pop(MewHashMap *map, const void *key, void *found_key, void *value) {
    MewHashMapBucket *bucket = mew_hashmap_bucket_for_key(map, key);
    if (bucket == NULL) {
        return false;
    }

    bucket->initialized = false;
    if (found_key != NULL) {
        char *key_ptr = found_key;
        memcpy(key_ptr, mew_hashmap_bucket_key(bucket), map->base.key_size);
    }
    if (value != NULL) {
        char *value_ptr = value;
        memcpy(value_ptr, mew_hashmap_bucket_value(map, bucket), map->base.value_size);
    }
    return true;
}

bool mew_hashmap_iterate(MewHashMap *map, MewMapIter iter, void *user_data) {
    for (size_t i = 0; i < map->bucket_count; i++) {
        MewHashMapBucket *bucket = mew_hashmap_bucket_for_index(map, i);
        if (!bucket->initialized)
            continue;
        if (!iter(mew_hashmap_bucket_key(bucket), mew_hashmap_bucket_value(map, bucket), user_data))
            return false;
    }
    return true;
}

static inline MewHashMapBucket *mew_hashmap_bucket_for_index(MewHashMap *map, size_t index) {
    size_t size = mew_hashmap_bucket_size(map);
    return (MewHashMapBucket *)(((char *)map->buckets) + size * index);
}

static inline char *mew_hashmap_bucket_key(MewHashMapBucket *bucket) {
    return (char *)bucket + sizeof(MewHashMapBucket);
}

static inline char *mew_hashmap_bucket_value(MewHashMap *map, MewHashMapBucket *bucket) {
    return (char *)bucket + sizeof(MewHashMapBucket) + map->base.key_size;
}

static inline MewHashMapBucket *mew_hashmap_bucket_for_key(MewHashMap *map, const void *key) {
    uint64_t hash = map->hashfunc((void *)key, map->base.user_data);
    size_t index = hash % map->bucket_count;
    size_t map_index = index;
    while (true) {
        MewHashMapBucket *bucket = mew_hashmap_bucket_for_index(map, index);
        if (!bucket->initialized) {
            return NULL;
        }
        if (bucket->map_index == map_index && mew_hashmap_equals(map, mew_hashmap_bucket_key(bucket), key)) {
            return bucket;
        }
        index++;
        if (index >= map->bucket_count) {
            index = 0;
        }
    }
}

static inline void mew_hashmap_alloc_buckets(MewHashMap *map, size_t count) {
    map->bucket_count = count;
    size_t size = mew_hashmap_bucket_size(map);
    map->buckets = mem_alloc(map->base.alloc, size * count);
    memset(map->buckets, 0, size * count);
}

static inline void mew_hashmap_expand(MewHashMap *map) {
    MewHashMapBucket *old_buckets = map->buckets;
    size_t old_count = map->bucket_count;
    size_t size = mew_hashmap_bucket_size(map);
    mew_hashmap_alloc_buckets(map, old_count * 2);
    map->base.count = 0;
    for (size_t i = 0; i < old_count; i++) {
        MewHashMapBucket *bucket = (MewHashMapBucket *)((char *)old_buckets + size * i);
        if (bucket->initialized) {
            mew_hashmap_insert(map, mew_hashmap_bucket_key(bucket), mew_hashmap_bucket_value(map, bucket));
        }
    }
    mem_free(map->base.alloc, old_buckets);
}

static inline size_t mew_hashmap_bucket_size(MewHashMap *map) {
    return sizeof(MewHashMapBucket) + map->base.key_size + map->base.value_size;
}

static inline bool mew_hashmap_equals(MewHashMap *map, const void *a, const void *b) {
    return (map->equals != NULL && map->equals(a, b, map->base.user_data)) || memcmp(a, b, map->base.key_size) == 0;
}

// djb2 hash algorithm
uint64_t mew_hashmap_sv_hash(const void *value, void *user_data) {
    (void)user_data;
    const StringView *sv = (const StringView *)value;
    uint64_t hash = 5381;
    for (size_t i = 0; i < sv->count; i++) {
        hash = ((hash << 5) + hash) + (uint64_t)sv->items[i];
    }
    return hash;
}

bool mew_hashmap_sv_equals(const void *a, const void *b, void *user_data) {
    (void)user_data;
    const StringView *sva = (const StringView *)a;
    const StringView *svb = (const StringView *)b;
    return sv_eq_sv(*sva, *svb);
}
