#include <mew/core/strings/sb.h>

#include <mew/unit.h>
#include "mew/core/allocators/malloc.h"

TEST(sb_new, {
    StringBuilder sb = sb_new(new_malloc_allocator());
    mewassert("After init sb data should be NULL", sb.vec.data == NULL);
    mewassert("After init sb count should be 0", sb.vec.count == 0);
    mewassert("After init sb capacity should be 0", sb.vec.capacity == 0);
    sb_destroy(&sb);
})

TEST(sb_default, {
    StringBuilder sb = sb_new_default();
    mewassert("After init sb data should be NULL", sb.vec.data == NULL);
    mewassert("After init sb count should be 0", sb.vec.count == 0);
    mewassert("After init sb capacity should be 0", sb.vec.capacity == 0);
    sb_destroy(&sb);
})

TEST(sb_append_char, {
    StringBuilder sb = sb_new_default();
    sb_append_char(&sb, 'H');
    sb_append_char(&sb, 'e');
    sb_append_char(&sb, 'l');
    sb_append_char(&sb, 'l');
    sb_append_char(&sb, 'o');
    mewassert("sb should have 5 elements after pushing 5 chars", sb_count(&sb) == 5);
    mewassert("5 pushed chars should be valid string", strncmp(sb_begin(&sb), "Hello", sb_count(&sb)) == 0);
    sb_destroy(&sb);
})

TEST(sb_append_buf, {
    StringBuilder sb = sb_new_default();
    sb_append_buf(&sb, "Hello", 5);
    mewassert("sb should have 5 elements after pushing buf", sb_count(&sb) == 5);
    mewassert("sb should be valid string", strncmp(sb_begin(&sb), "Hello", sb_count(&sb)) == 0);
    sb_destroy(&sb);
})

TEST(sb_append_cstr, {
    StringBuilder sb = sb_new_default();
    sb_append_cstr(&sb, "Hello");
    mewassert("sb should have 5 elements after pushing cstr", sb_count(&sb) == 5);
    mewassert("sb should be valid string", strncmp(sb_begin(&sb), "Hello", sb_count(&sb)) == 0);
    sb_destroy(&sb);
})

TEST(sb_dup, {
    StringBuilder sb = sb_new_default();
    sb_append_cstr(&sb, "Hello");

    StringBuilder other = sb_dup(new_malloc_allocator(), sb);
    mewassert("duped sb should be valid string", strncmp(sb_begin(&sb), "Hello", sb_count(&sb)) == 0);
    sb_destroy(&sb);
    sb_destroy(&other);
})

mewtest_main();
