#include <mew/containers/map.h>
#include <mew/unit.h>
#include "mew/core/allocators/malloc.h"

#define DEADBEEF ((void *)0xdeadbeef)

#define MAP_GENERIC_TEST_LIST \
    MAP_X(test_map_insert) \
    MAP_X(test_map_get) \
    MAP_X(test_map_get_from_empty) \
    MAP_X(test_map_pop) \
    MAP_X(test_map_insert_same_key) \
    MAP_X(test_map_iter) \
    MAP_X(test_map_iter_returns_false_when_iter_fails)

bool is_value_for_key_equals(MewMap map, const char *cstr, uint64_t value) {
    StringView key = cstr_to_sv(cstr);
    uint64_t *actual_value = mew_map_get(map, &key);
    if (actual_value == NULL) {
        return false;
    } else if (*actual_value != value) {
        return false;
    } else {
        return true;
    }
}

const char *test_map_insert(MewMap map) {
    Allocator a = new_malloc_allocator();

    for (size_t i = 0; i < 1024; i++) {
        StringView key = cstr_to_sv(mem_sprintf(a, "Sample String %zu", i * 1337));
        uint16_t value = i * 1337;
        mew_map_insert(map, &key, &value);
    }

    mewassert("Map should have 1024 elements", mew_map_count(map) == 1024);

    return NULL;
}

const char *test_map_get(MewMap map) {
    Arena arena = {0};
    Allocator a = new_arena_allocator(&arena);

    const size_t keys = 48496;
    const size_t mul = 1337;
    for (size_t i = 0; i < keys; i++) {
        StringView key = cstr_to_sv(mem_sprintf(a, "Sample String %zu", i * mul));
        uint64_t value = i * mul;
        mew_map_insert(map, &key, &value);
    }

    for (size_t i = 0; i < keys; i++) {
        char *key = mem_sprintf(a, "Sample String %zu", i * mul);
        mewassert("Value should be valid for the given key", is_value_for_key_equals(map, key, i * mul));
    }

    arena_free_arena(&arena);

    return NULL;
}

const char *test_map_get_from_empty(MewMap map) {
    StringView key = cstr_to_sv("Some key");
    uint64_t *result = mew_map_get(map, &key);
    mewassert("Hashmap get should return NULL if map is empty", result == NULL);
    return NULL;
}

const char *test_map_pop(MewMap map) {
    Arena arena = {0};
    Allocator a = new_arena_allocator(&arena);

    for (size_t i = 0; i < 1024; i++) {
        StringView key = cstr_to_sv(mem_sprintf(a, "Sample String %zu", i * 1337));
        uint64_t value = i * 1337;
        mew_map_insert(map, &key, &value);
    }

    StringView found_key;
    uint64_t found_value;
    bool result;

    StringView invalid_key = cstr_to_sv("Invalid key not found in the map");
    result = mew_map_pop(map, &invalid_key, &found_key, &found_value);
    mewassert("Map pop for invalid key should return false", !result);

    StringView valid_key = cstr_to_sv("Sample String 847658");
    result = mew_map_pop(map, &valid_key, &found_key, &found_value);
    mewassert("Map pop for valid key should return true", result);
    mewassert("Map pop for valid key should return valid key", sv_eq_sv(valid_key, found_key));
    mewassert("Map pop for valid key should return valid value", 847658 == found_value);

    result = mew_map_pop(map, &valid_key, &found_key, &found_value);
    mewassert("Map pop for valid key one more time should return false", !result);

    StringView another_key = cstr_to_sv("Sample String 1337");
    result = mew_map_pop(map, &another_key, NULL, NULL);
    mewassert("Map should not segfault if found parameters are NULL", result);

    arena_free_arena(&arena);

    return NULL;
}

const char *test_map_insert_same_key(MewMap map) {
    StringView key = cstr_to_sv("Some key");
    uint64_t value = 1337, next_value = 228;


    mew_map_insert(map, &key, &value);
    mew_map_insert(map, &key, &next_value);

    uint64_t *result = mew_map_get(map, &key);
    mewassert("Map get should return valid value", result != NULL);
    mewassert("Map get should return last inserted value for the same key", *result == next_value);

    return NULL;
}

bool hashmap_iter_sum(const void *key_arg, const void *value_arg, void *user_data) {
    uint64_t *sum = user_data;
    const StringView *key = key_arg;
    const uint64_t *value = value_arg;

    *sum += *value;

    return true;
}

const char *test_map_iter(MewMap map) {
    uint64_t sum = 0;

    Arena arena = {0};
    Allocator a = new_arena_allocator(&arena);

    for (size_t i = 0; i < 50; i++) {
        StringView key = cstr_to_sv(mem_sprintf(a, "Sample String %zu", i * 1337));
        uint64_t value = i;
        mew_map_insert(map, &key, &value);
    }

    bool result = mew_map_iterate(map, hashmap_iter_sum, &sum);
    mewassert("Map iter should return true because all iter calls returned true", result);
    mewassert("Sum variable should contain actual sum after iterating map", sum == 1225);

    arena_free_arena(&arena);

    return NULL;
}

bool hashmap_iter_fail(const void *key_arg, const void *value_arg, void *user_data) {
    (void) key_arg;
    (void) value_arg;

    uint64_t *count = user_data;
    (*count) += 1;

    return false;
}

const char *test_map_iter_returns_false_when_iter_fails(MewMap map) {
    uint64_t count = 0;

    Arena arena = {0};
    Allocator a = new_arena_allocator(&arena);

    for (size_t i = 0; i < 50; i++) {
        StringView key = cstr_to_sv(mem_sprintf(a, "Sample String %zu", i * 1337));
        uint64_t value = i;
        mew_map_insert(map, &key, &value);
    }

    bool result = mew_map_iterate(map, hashmap_iter_fail, &count);

    mewassert("Map iter should return false because iter always returns false", !result);
    mewassert("Iter func should be called exactly once because it always fails", count == 1);

    arena_free_arena(&arena);

    return NULL;
}
