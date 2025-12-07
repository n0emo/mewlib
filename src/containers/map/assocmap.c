#include <mew/containers/map/assoc.h>

#include <string.h>

#include <mew/core.h>

#define MEW_MAP_ASSOC_INITIAL_CAPACITY 16

char *mew_map_assoc_get_key_at(MewAssocMap *map, usize index);
char *mew_map_assoc_get_value_at(MewAssocMap *map, usize index);

void mew_assocmap_init(MewAssocMap *map, MewAssocMapOptions options) {
    memset(map, 0, sizeof(*map));

    MewMapBaseOptions base_options = {
        .alloc = options.alloc,
        .funcs =
            (MewMapFuncTable) {
                .destroy = mew_assocmap_destroy_overload,
                .insert = mew_assocmap_insert_overload,
                .pop = mew_assocmap_pop_overload,
                .get = mew_assocmap_get_overload,
                .iterate = mew_assocmap_iterate_overload,
            },

        .key_size = options.key_size,
        .value_size = options.value_size,
        .user_data = options.user_data,
    };

    mew_map_base_init((MewMap *)map, base_options);

    map->data = mem_calloc(options.alloc, MEW_MAP_ASSOC_INITIAL_CAPACITY, map->base.key_size + map->base.value_size);
    map->capacity = MEW_MAP_ASSOC_INITIAL_CAPACITY;

    map->equals = options.equals;
}

MewAssocMap mew_assocmap_new(MewAssocMapOptions options) {
    MewAssocMap map;
    mew_assocmap_init(&map, options);
    return map;
}

void mew_assocmap_destroy_overload(MewMap *arg) {
    mew_assocmap_destroy((MewAssocMap *)arg);
}

void mew_assocmap_insert_overload(MewMap *arg, const void *key, const void *value) {
    mew_assocmap_insert((MewAssocMap *)arg, key, value);
}

bool mew_assocmap_pop_overload(MewMap *arg, const void *key, void *found_key, void *value) {
    return mew_assocmap_pop((MewAssocMap *)arg, key, found_key, value);
}

void *mew_assocmap_get_overload(MewMap *base, const void *key) {
    return mew_assocmap_get((MewAssocMap *)base, key);
}

bool mew_assocmap_iterate_overload(MewMap *base, MewMapIter iter, void *user_data) {
    return mew_assocmap_iterate((MewAssocMap *)base, iter, user_data);
}

void mew_assocmap_destroy(MewAssocMap *map) {
    mem_free(map->base.alloc, map->data);
}

void mew_assocmap_insert(MewAssocMap *map, const void *key, const void *value) {
    usize index = 0;

    for (index = 0; index < map->base.count; index++) {
        char *current_key = mew_map_assoc_get_key_at(map, index);
        if (map->equals(current_key, key, map->base.user_data)) {
            char *current_val = mew_map_assoc_get_value_at(map, index);
            memcpy(current_val, value, map->base.value_size);
            return;
        }
    }

    if (map->base.count == map->capacity) {
        usize new_capacity = map->capacity * 2;
        usize bucket_size = map->base.key_size + map->base.value_size;
        char *new_data = mem_calloc(map->base.alloc, new_capacity, bucket_size);
        memcpy(new_data, map->data, bucket_size * map->base.count);
        mem_free(map->base.alloc, map->data);
        map->data = new_data;
        map->capacity = new_capacity;
    }

    char *current_key = mew_map_assoc_get_key_at(map, index);
    char *current_val = mew_map_assoc_get_value_at(map, index);
    memcpy(current_key, key, map->base.key_size);
    memcpy(current_val, value, map->base.value_size);

    map->base.count++;
}

bool mew_assocmap_pop(MewAssocMap *map, const void *key, void *found_key, void *value) {
    bool result = false;
    usize index = 0;

    for (index = 0; index < map->base.count; index++) {
        char *current_key = mew_map_assoc_get_key_at(map, index);
        if (map->equals(key, current_key, map->base.user_data)) {
            char *current_value = current_key + map->base.key_size;
            // NOTE: size of value may not be aligned here, but I don't mind it
            //       right now
            if (found_key != NULL) {
                memcpy(found_key, current_key, map->base.key_size);
            }
            if (value != NULL) {
                memcpy(value, current_value, map->base.value_size);
            }
            result = true;
            map->base.count--;
            break;
        }
    }

    for (usize j = index; j < map->base.count; j++) {
        usize bucket_size = map->base.key_size + map->base.value_size;
        usize offset = j * bucket_size;
        memcpy(map->data + offset, map->data + offset + bucket_size, bucket_size);
    }

    return result;
}

void *mew_assocmap_get(MewAssocMap *map, const void *key) {
    for (usize i = 0; i < map->base.count; i++) {
        char *current_key = mew_map_assoc_get_key_at(map, i);
        if (map->equals(key, current_key, map->base.user_data)) {
            return current_key + map->base.key_size;
        }
    }

    return NULL;
}

bool mew_assocmap_iterate(MewAssocMap *map, MewMapIter iter, void *user_data) {
    for (usize i = 0; i < map->base.count; i++) {
        char *key = mew_map_assoc_get_key_at(map, i);
        char *value = mew_map_assoc_get_value_at(map, i);
        if (!iter(key, value, user_data)) {
            return false;
        }
    }

    return true;
}

char *mew_map_assoc_get_key_at(MewAssocMap *map, usize index) {
    usize bucket_size = map->base.key_size + map->base.value_size;
    return map->data + bucket_size * index;
}

char *mew_map_assoc_get_value_at(MewAssocMap *map, usize index) {
    usize bucket_size = map->base.key_size + map->base.value_size;
    return map->data + bucket_size * index + map->base.key_size;
}
