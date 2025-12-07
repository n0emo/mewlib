#include "../map.c"
#include <mew/core/allocators/malloc.h>
#include <mew/containers/map/assoc.h>

bool map_sv_equals(const void *a, const void *b, void *user_data) {
    (void)user_data;
    const StringView *sva = (const StringView *)a;
    const StringView *svb = (const StringView *)b;
    return sv_eq_sv(*sva, *svb);
}

#define MAP_X(test)                                                                                                    \
    TEST(test##_assocmap, {                                                                                            \
        MewAssocMapOptions options = {                                                                                 \
            .alloc = new_malloc_allocator(),                                                                           \
            .key_size = sizeof(StringView),                                                                            \
            .value_size = sizeof(uint64_t),                                                                            \
            .user_data = DEADBEEF,                                                                                     \
            .equals = map_sv_equals,                                                                                   \
        };                                                                                                             \
        MewAssocMap assocmap = mew_assocmap_new(options);                                                              \
                                                                                                                       \
        const char *result = test((MewMap *)&assocmap);                                                                \
                                                                                                                       \
        mew_assocmap_destroy(&assocmap);                                                                               \
                                                                                                                       \
        return result;                                                                                                 \
    })
MAP_GENERIC_TEST_LIST
#undef MAP_X

mewtest_main();
