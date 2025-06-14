#include <mew/containers/hashmap.h>

#include <mew/unit.h>

#define DEADBEEF ((void *)0xdeadbeef)

bool is_value_for_key_equals(HashMap *map, const char *cstr, uint64_t value) {
    StringView key = cstr_to_sv(cstr);
    uint64_t *actual_value = hashmap_get(map, &key);
    if (actual_value == NULL) {
        return false;
    } else if (*actual_value != value) {
        return false;
    } else {
        return true;
    }
}

TEST(hashmap_init, {
    HashMap map;
    hashmap_init(&map, DEADBEEF, hashmap_sv_hash, hashmap_sv_equals, sizeof(StringView), sizeof(uint16_t));

    mewassert("Hashmap should have valid hashfunc pointer", map.hashfunc != NULL);
    mewassert("Hashmap should have valid equals pointer", map.equals != NULL);
    mewassert("Hashmap should have valid user_data", map.user_data == DEADBEEF);
    mewassert("Hashmap should have valid key_size", map.key_size = sizeof(StringView));
    mewassert("Hashmap should have valid value_size ", map.value_size = sizeof(uint16_t));
    mewassert("Hashmap should be empty", map.element_count == 0);
    mewassert("Hashmap bucket array should be allocated", map.buckets != NULL && map.bucket_count != 0);

    hashmap_destroy(&map);
})

TEST(hashmap_insert, {
    HashMap map;
    hashmap_init(&map, DEADBEEF, hashmap_sv_hash, hashmap_sv_equals, sizeof(StringView), sizeof(uint16_t));

    Arena arena = {0};
    Allocator a = new_arena_allocator(&arena);

    for (size_t i = 0; i < 1024; i++) {
        StringView key = cstr_to_sv(mem_sprintf(a, "Sample String %zu", i * 1337));
        uint16_t value = i * 1337;
        hashmap_insert(&map, &key, &value);
    }

    mewassert("Map should have 1024 elements", map.element_count == 1024);

    arena_free_arena(&arena);
    hashmap_destroy(&map);
})

TEST(hashmap_get, {
    HashMap map;
    hashmap_init(&map, DEADBEEF, hashmap_sv_hash, hashmap_sv_equals, sizeof(StringView), sizeof(uint64_t));

    Arena arena = {0};
    Allocator a = new_arena_allocator(&arena);

    const size_t keys = 48496;
    const size_t mul = 1337;
    for (size_t i = 0; i < keys; i++) {
        StringView key = cstr_to_sv(mem_sprintf(a, "Sample String %zu", i * mul));
        uint64_t value = i * mul;
        hashmap_insert(&map, &key, &value);
    }

    for (size_t i = 0; i < keys; i++) {
        char *key = mem_sprintf(a, "Sample String %zu", i * mul);
        mewassert("Value should be valid for the given key", is_value_for_key_equals(&map, key, i * mul));
    }

    arena_free_arena(&arena);
    hashmap_destroy(&map);
})

TEST(hashmap_get_from_empty, {
    HashMap map;
    hashmap_init(&map, DEADBEEF, hashmap_sv_hash, hashmap_sv_equals, sizeof(StringView), sizeof(uint64_t));

    StringView key = cstr_to_sv("Some key");
    uint64_t *result = hashmap_get(&map, &key);
    mewassert("Hashmap get should return NULL if map is empty", result == NULL);

    hashmap_destroy(&map);
})



TEST(hashmap_pop, {
    HashMap map;
    hashmap_init(&map, DEADBEEF, hashmap_sv_hash, hashmap_sv_equals, sizeof(StringView), sizeof(uint64_t));

    Arena arena = {0};
    Allocator a = new_arena_allocator(&arena);

    for (size_t i = 0; i < 1024; i++) {
        StringView key = cstr_to_sv(mem_sprintf(a, "Sample String %zu", i * 1337));
        uint64_t value = i * 1337;
        hashmap_insert(&map, &key, &value);
    }

    StringView found_key;
    uint64_t found_value;
    bool result;

    StringView invalid_key = cstr_to_sv("Invalid key not found in the map");
    result = hashmap_pop(&map, &invalid_key, &found_key, &found_value);
    mewassert("Map pop for invalid key should return false", !result);

    StringView valid_key = cstr_to_sv("Sample String 847658");
    result = hashmap_pop(&map, &valid_key, &found_key, &found_value);
    mewassert("Map pop for valid key should return true", result);
    mewassert("Map pop for valid key should return valid key", sv_eq_sv(valid_key, found_key));
    mewassert("Map pop for valid key should return valid value", 847658 == found_value);

    result = hashmap_pop(&map, &valid_key, &found_key, &found_value);
    mewassert("Map pop for valid key one more time should return false", !result);

    StringView another_key = cstr_to_sv("Sample String 1337");
    result = hashmap_pop(&map, &another_key, NULL, NULL);
    mewassert("Map should not segfault if found parameters are NULL", result);

    arena_free_arena(&arena);
    hashmap_destroy(&map);
})

TEST(hashmap_insert_same_key, {
    HashMap map;
    StringView key = cstr_to_sv("Some key");
    uint64_t value = 1337, next_value = 228;

    hashmap_init(&map, DEADBEEF, hashmap_sv_hash, hashmap_sv_equals, sizeof(StringView), sizeof(uint64_t));

    hashmap_insert(&map, &key, &value);
    hashmap_insert(&map, &key, &next_value);

    uint64_t *result = hashmap_get(&map, &key);
    mewassert("Map get should return valid value", result != NULL);
    mewassert("Map get should return last inserted value for the same key", *result == next_value);

    hashmap_destroy(&map);
})

bool hashmap_iter_sum(const void *key_arg, const void *value_arg, void *user_data) {
    uint64_t *sum = user_data;
    const StringView *key = key_arg;
    const uint64_t *value = value_arg;

    *sum += *value;

    return true;
}

TEST(hashmap_iter, {
    uint64_t sum = 0;

    HashMap map;
    hashmap_init(&map, &sum, hashmap_sv_hash, hashmap_sv_equals, sizeof(StringView), sizeof(uint64_t));
    Arena arena = {0};
    Allocator a = new_arena_allocator(&arena);

    for (size_t i = 0; i < 50; i++) {
        StringView key = cstr_to_sv(mem_sprintf(a, "Sample String %zu", i * 1337));
        uint64_t value = i;
        hashmap_insert(&map, &key, &value);
    }

    bool result = hashmap_iterate(&map, hashmap_iter_sum);
    mewassert("Map iter should return true because all iter calls returned true", result);
    mewassert("Sum variable should contain actual sum after iterating map", sum == 1225);

    arena_free_arena(&arena);
    hashmap_destroy(&map);
})

bool hashmap_iter_fail(const void *key_arg, const void *value_arg, void *user_data) {
    (void) key_arg;
    (void) value_arg;

    uint64_t *count = user_data;
    (*count) += 1;

    return false;
}

TEST(hashmap_iter_returns_false_when_iter_fails, {
    uint64_t count = 0;

    HashMap map;
    hashmap_init(&map, &count, hashmap_sv_hash, hashmap_sv_equals, sizeof(StringView), sizeof(uint64_t));

    Arena arena = {0};
    Allocator a = new_arena_allocator(&arena);

    for (size_t i = 0; i < 50; i++) {
        StringView key = cstr_to_sv(mem_sprintf(a, "Sample String %zu", i * 1337));
        uint64_t value = i;
        hashmap_insert(&map, &key, &value);
    }

    bool result = hashmap_iterate(&map, hashmap_iter_fail);

    mewassert("Map iter should return false because iter always returns false", !result);
    mewassert("Iter func should be called exactly once because it always fails", count == 1);

    arena_free_arena(&arena);
    hashmap_destroy(&map);
})

mewtest_main()
