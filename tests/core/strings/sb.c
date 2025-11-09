#define MEWLIB_IMPLEMENTATION
#define MEW_UNIT
#include <mewlib.h>

TEST(sb_new, {
    MewStringBuilder sb = mew_sb_new(mew_malloc_allocator_create());
    mewassert("After init sb data should be NULL", sb.vec.data == NULL);
    mewassert("After init sb count should be 0", sb.vec.count == 0);
    mewassert("After init sb capacity should be 0", sb.vec.capacity == 0);
    mew_sb_destroy(&sb);
})

TEST(sb_default, {
    MewStringBuilder sb = mew_sb_new_default();
    mewassert("After init sb data should be NULL", sb.vec.data == NULL);
    mewassert("After init sb count should be 0", sb.vec.count == 0);
    mewassert("After init sb capacity should be 0", sb.vec.capacity == 0);
    mew_sb_destroy(&sb);
})

TEST(mew_sb_append_char, {
    MewStringBuilder sb = mew_sb_new_default();
    mew_sb_append_char(&sb, 'H');
    mew_sb_append_char(&sb, 'e');
    mew_sb_append_char(&sb, 'l');
    mew_sb_append_char(&sb, 'l');
    mew_sb_append_char(&sb, 'o');
    mewassert("sb should have 5 elements after pushing 5 chars", mew_sb_count(&sb) == 5);
    mewassert("5 pushed chars should be valid string", strncmp(mew_sb_begin(&sb), "Hello", mew_sb_count(&sb)) == 0);
    mew_sb_destroy(&sb);
})

TEST(mew_sb_append_buf, {
    MewStringBuilder sb = mew_sb_new_default();
    mew_sb_append_buf(&sb, "Hello", 5);
    mewassert("sb should have 5 elements after pushing buf", mew_sb_count(&sb) == 5);
    mewassert("sb should be valid string", strncmp(mew_sb_begin(&sb), "Hello", mew_sb_count(&sb)) == 0);
    mew_sb_destroy(&sb);
})

TEST(mew_sb_append_cstr, {
    MewStringBuilder sb = mew_sb_new_default();
    mew_sb_append_cstr(&sb, "Hello");
    mewassert("sb should have 5 elements after pushing cstr", mew_sb_count(&sb) == 5);
    mewassert("sb should be valid string", strncmp(mew_sb_begin(&sb), "Hello", mew_sb_count(&sb)) == 0);
    mew_sb_destroy(&sb);
})

TEST(sb_dup, {
    MewStringBuilder sb = mew_sb_new_default();
    mew_sb_append_cstr(&sb, "Hello");

    MewStringBuilder other = mew_sb_dup(mew_malloc_allocator_create(), sb);
    mewassert("duped sb should be valid string", strncmp(mew_sb_begin(&sb), "Hello", mew_sb_count(&sb)) == 0);
    mew_sb_destroy(&sb);
    mew_sb_destroy(&other);
})

mewtest_main()
