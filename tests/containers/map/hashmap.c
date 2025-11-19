#include "../map.c"
#include <mew/containers/map/hashmap.h>

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

#define MAP_X(test)                                                                                                    \
    TEST(test##_hashmap, {                                                                                             \
        HashMap hash_map;                                                                                              \
        hashmap_init(&hash_map, DEADBEEF, hashmap_sv_hash, hashmap_sv_equals, sizeof(StringView), sizeof(uint16_t));   \
        MewMap map = mew_map_from_hashmap(&hash_map);                                                                  \
                                                                                                                       \
        const char *result;                                                                                            \
        if ((result = test(map))) {                                                                                    \
            return result;                                                                                             \
        }                                                                                                              \
                                                                                                                       \
        hashmap_destroy(&hash_map);                                                                                    \
    })
MAP_GENERIC_TEST_LIST
#undef MAP_X

mewtest_main();
