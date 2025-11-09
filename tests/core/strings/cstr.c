#define MEWLIB_IMPLEMENTATION
#define MEW_UNIT
#include <mewlib.h>

TEST(cstr_contains, {
    const char *s = "Hello, World!";

    mewassert("cstr_contains should return true if string contains char", mew_cstr_contains(s, 'W'));
    mewassert("cstr_contains should return true if string contains char", mew_cstr_contains(s, '!'));
    mewassert("cstr_contains should return true if string contains char", mew_cstr_contains(s, 'H'));

    mewassert("cstr_contains should return false if string doesn't contain char", !mew_cstr_contains(s, 'x'));
    mewassert("cstr_contains should return false if string doesn't contain char", !mew_cstr_contains(s, '\0'));
})

TEST(cstr_len, {
    const char *s = "Hello, World!";
    mewassert("cstr_len should output correct null-terminated string length", mew_cstr_len(s) == 13);
})

mewtest_main({})
