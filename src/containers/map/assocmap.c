#include <mew/containers/map/assoc.h>
#include <mew/core.h>
#include <string.h>

#define MEW_MAP_ASSOC_INITIAL_CAPACITY 16

char *mew_map_assoc_get_key_at(MewAssocMap *map, usize index);
char *mew_map_assoc_get_value_at(MewAssocMap *map, usize index);
usize aligned_size(usize s);

void mew_assocmap_init(MewAssocMap *map, MewAssocMapOptions options) {
    memset(map, 0, sizeof(*map));
    map->alloc = options.alloc;
    map->key_size = aligned_size(options.key_size);
    map->value_size = aligned_size(options.value_size);

    map->data = mem_calloc(options.alloc, MEW_MAP_ASSOC_INITIAL_CAPACITY, map->key_size + map->value_size);
    map->capacity = MEW_MAP_ASSOC_INITIAL_CAPACITY;

    map->user_data = options.user_data;
    map->equals = options.equals;
}

MewAssocMap mew_assocmap_new(MewAssocMapOptions options) {
    MewAssocMap map;
    mew_assocmap_init(&map, options);
    return map;
}

MewMap mew_map_from_assocmap(MewAssocMap *map) {
    static MewMapFuncTable func_table = {
        .destroy = mew_assocmap_destroy,
        .insert = mew_assocmap_insert,
        .pop = mew_assocmap_pop,
        .get = mew_assocmap_get,
        .iterate = mew_assocmap_iterate,
        .count = mew_assocmap_count,
    };

    return (MewMap) {.data = map, .funcs = &func_table};
}

void mew_assocmap_destroy(void *arg) {
    MewAssocMap *map = arg;

    mem_free(map->alloc, map->data);
}

void mew_assocmap_insert(void *arg, const void *key, const void *value) {
    MewAssocMap *map = arg;
    usize index = 0;

    for (index = 0; index < map->count; index++) {
        char *current_key = mew_map_assoc_get_key_at(map, index);
        if (map->equals(current_key, key, map->user_data)) {
            char *current_val = mew_map_assoc_get_value_at(map, index);
            memcpy(current_val, value, map->value_size);
            return;
        }
    }

    if (map->count == map->capacity) {
        usize new_capacity = map->capacity * 2;
        usize bucket_size = map->key_size + map->value_size;
        char *new_data = mem_calloc(map->alloc, new_capacity, bucket_size);
        memcpy(new_data, map->data, bucket_size * map->count);
        mem_free(map->alloc, map->data);
        map->data = new_data;
        map->capacity = new_capacity;
    }

    char *current_key = mew_map_assoc_get_key_at(map, index);
    char *current_val = mew_map_assoc_get_value_at(map, index);
    memcpy(current_key, key, map->key_size);
    memcpy(current_val, value, map->value_size);

    map->count++;
}

bool mew_assocmap_pop(void *arg, const void *key, void *found_key, void *value) {
    MewAssocMap *map = arg;
    bool result = false;
    usize index = 0;

    for (index = 0; index < map->count; index++) {
        char *current_key = mew_map_assoc_get_key_at(map, index);
        if (map->equals(key, current_key, map->user_data)) {
            char *current_value = current_key + map->key_size;
            // NOTE: size of value may not be aligned here, but I don't mind it
            //       right now
            if (found_key != NULL) {
                memcpy(found_key, current_key, map->key_size);
            }
            if (value != NULL) {
                memcpy(value, current_value, map->value_size);
            }
            result = true;
            map->count--;
            break;
        }
    }

    for (usize j = index; j < map->count; j++) {
        usize bucket_size = map->key_size + map->value_size;
        usize offset = j * bucket_size;
        memcpy(map->data + offset, map->data + offset + bucket_size, bucket_size);
    }

    return result;
}

void *mew_assocmap_get(void *arg, const void *key) {
    MewAssocMap *map = arg;

    for (usize i = 0; i < map->count; i++) {
        char *current_key = mew_map_assoc_get_key_at(map, i);
        if (map->equals(key, current_key, map->user_data)) {
            return current_key + map->key_size;
        }
    }

    return NULL;
}

bool mew_assocmap_iterate(void *arg, MewMapIter iter, void *user_data) {
    MewAssocMap *map = arg;

    for (usize i = 0; i < map->count; i++) {
        char *key = mew_map_assoc_get_key_at(map, i);
        char *value = mew_map_assoc_get_value_at(map, i);
        if (!iter(key, value, user_data)) {
            return false;
        }
    }

    return true;
}

usize mew_assocmap_count(void *arg) {
    MewAssocMap *map = arg;
    return map->count;
}

char *mew_map_assoc_get_key_at(MewAssocMap *map, usize index) {
    usize bucket_size = map->key_size + map->value_size;
    return map->data + bucket_size * index;
}

char *mew_map_assoc_get_value_at(MewAssocMap *map, usize index) {
    usize bucket_size = map->key_size + map->value_size;
    return map->data + bucket_size * index + map->key_size;
}

// Took from: https://stackoverflow.com/questions/23927837
usize aligned_size(usize s) {
    const usize alignment = 8;
    return (s + alignment - 1) & ~(alignment - 1);
}
