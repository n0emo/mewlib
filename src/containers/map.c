#include <mew/containers/map.h>

static usize aligned_size(usize s);

void mew_map_base_init(MewMap *base, MewMapBaseOptions options) {
    base->funcs = options.funcs;
    base->alloc = options.alloc;
    base->key_size = aligned_size(options.key_size);
    base->value_size = aligned_size(options.value_size);
    base->user_data = options.user_data;
}

void mew_map_destroy(MewMap *map) {
    assert(map != NULL);
    if (map->funcs.destroy != NULL) {
        map->funcs.destroy(map);
    }
}

void mew_map_insert(MewMap *map, const void *key, const void *value) {
    assert(map != NULL);
    assert(map->funcs.insert != NULL);
    map->funcs.insert(map, key, value);
}

bool mew_map_pop(MewMap *map, const void *key, void *found_key, void *value) {
    assert(map != NULL);
    assert(map->funcs.pop != NULL);
    return map->funcs.pop(map, key, found_key, value);
}

void *mew_map_get(MewMap *map, const void *key) {
    assert(map != NULL);
    assert(map->funcs.get != NULL);
    return map->funcs.get(map, key);
}

bool mew_map_iterate(MewMap *map, MewMapIter iter, void *user_data) {
    assert(map != NULL);
    assert(map->funcs.iterate != NULL);
    return map->funcs.iterate(map, iter, user_data);
}

// Took from: https://stackoverflow.com/questions/23927837
static usize aligned_size(usize s) {
    const usize alignment = 8;
    return (s + alignment - 1) & ~(alignment - 1);
}
