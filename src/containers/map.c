#include <assert.h>
#include <mew/containers/map.h>

void mew_map_destroy(MewMap map) {
    if (map.funcs != NULL && map.funcs->destroy != NULL) {
        map.funcs->destroy(map.data);
    }
}

void mew_map_insert(MewMap map, const void *key, const void *value) {
    assert(map.funcs != NULL);
    assert(map.funcs->insert != NULL);
    map.funcs->insert(map.data, key, value);
}

bool mew_map_pop(MewMap map, const void *key, void *found_key, void *value) {
    assert(map.funcs != NULL);
    assert(map.funcs->insert != NULL);
    return map.funcs->pop(map.data, key, found_key, value);
}

void *mew_map_get(MewMap map, const void *key) {
    assert(map.funcs != NULL);
    assert(map.funcs->insert != NULL);
    return map.funcs->get(map.data, key);
}

bool mew_map_iterate(MewMap map, mew_map_iter_t iter, void *user_data) {
    assert(map.funcs != NULL);
    assert(map.funcs->insert != NULL);
    return map.funcs->iterate(map.data, iter, user_data);
}

usize mew_map_count(MewMap map) {
    assert(map.funcs != NULL);
    assert(map.funcs->insert != NULL);
    return map.funcs->count(map.data);
}
