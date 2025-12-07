#include "../map.c"
#include <mew/containers/map/hashmap.h>

TEST(hashmap_init, {
    HashMap map;

    MewHashMapOptions options = {
        .key_size = sizeof(StringView),
        .value_size = sizeof(uint16_t),
        .hashfunc = hashmap_sv_hash,
        .equals = hashmap_sv_equals,
        .user_data = DEADBEEF,
    };
    hashmap_init(&map, options);

    mewassert("Hashmap should have valid hashfunc pointer", map.hashfunc != NULL);
    mewassert("Hashmap should have valid equals pointer", map.equals != NULL);
    mewassert("Hashmap should have valid user_data", map.base.user_data == DEADBEEF);
    mewassert("Hashmap should have valid key_size", map.base.key_size = sizeof(StringView));
    mewassert("Hashmap should have valid value_size ", map.base.value_size = sizeof(uint16_t));
    mewassert("Hashmap should be empty", map.base.count == 0);
    mewassert("Hashmap bucket array should be allocated", map.buckets != NULL && map.bucket_count != 0);

    hashmap_destroy(&map);
})

#define MAP_X(test)                                                                                                    \
    TEST(test##_hashmap, {                                                                                             \
        HashMap hash_map;                                                                                              \
        MewHashMapOptions options = {                                                                                  \
            .key_size = sizeof(StringView),                                                                            \
            .value_size = sizeof(uint64_t),                                                                            \
            .hashfunc = hashmap_sv_hash,                                                                               \
            .equals = hashmap_sv_equals,                                                                               \
            .user_data = DEADBEEF,                                                                                     \
        };                                                                                                             \
        hashmap_init(&hash_map, options);                                                                              \
                                                                                                                       \
        const char *result = test((MewMap *)&hash_map);                                                                \
                                                                                                                       \
        hashmap_destroy(&hash_map);                                                                                    \
                                                                                                                       \
        return result;                                                                                                 \
    })
MAP_GENERIC_TEST_LIST
#undef MAP_X

mewtest_main();
