#include <stdio.h>

#include <mew/containers/map/assoc.h>
#include <mew/core/allocators/malloc.h>
#include <mew/core/strings/sb.h>
#include "mew/containers/map.h"

static bool mew_sb_equals(const void *a_arg, const void *b_arg, void *user_data) {
    (void)user_data;
    StringBuilder *a = (StringBuilder *)a_arg;
    StringBuilder *b = (StringBuilder *)b_arg;
    return sb_eq_sb(*a, *b);
}

static bool iter_destroy(void *key_arg, void *value, void *user_data) {
    (void)value;
    (void)user_data;

    StringBuilder *key = (StringBuilder *)key_arg;
    sb_destroy(key);

    return true;
}

int main(void) {
    Allocator a = new_malloc_allocator();
    MewAssocMapOptions opts = {
        .alloc = a,
        .equals = mew_sb_equals,
        .key_size = sizeof(StringBuilder),
        .value_size = sizeof(i64),
        .user_data = NULL,
    };

    MewAssocMap assocmap = mew_assocmap_new(opts);

    for (i32 i = 1; i <= 20; i++) {
        StringBuilder key = sb_new_default();
        sb_appendf(&key, "Sample String %d", i);
        i64 value = i + '0';
        printf("Inserting `" SB_FMT "`: %llu\n", SB_ARG(key), value);
        mew_t_map_insert(StringBuilder, i64, (MewMap *)&assocmap, key, value);
    }

    printf("Bucket size = %zu\n", assocmap.base.key_size + assocmap.base.value_size);
    printf("Count       = %zu\n", assocmap.base.count);

    {
        StringBuilder key = sb_new_default();
        sb_append_cstr(&key, "Sample String 10");
        i64 *val;
        mew_t_map_get(StringBuilder, i64, &assocmap, key, val);
        printf("Value for key `" SB_FMT "`: %llu\n", SB_ARG(key), *val);
        sb_destroy(&key);
    }

    bool ok;
    mew_t_map_iterate(StringBuilder, i64, &assocmap, iter_destroy, NULL, &ok);
    mew_t_map_destroy(StringBuilder, i64, &assocmap);

    return 0;
}
