#ifndef INCLUDE_MEW_CORE_STRINGS_SV_H_
#define INCLUDE_MEW_CORE_STRINGS_SV_H_

#include <mew/core/types.h>
#include <mew/core/alloc.h>

#define SV_FMT "%.*s"
#define SV_ARG(sv) (int)(sv).count, (sv).items

typedef struct StringView {
    const char *items;
    usize count;
} StringView;

StringView sv_identity(StringView sv);
bool sv_contains(StringView sv, char c);
bool sv_starts_with(StringView sv, StringView prefix);
bool sv_starts_with_cstr(StringView sv, const char *prefix);
StringView sv_slice(StringView sv, size_t start, size_t count);
StringView sv_slice_from(StringView sv, size_t index);
StringView sv_trim_right_by_cstr(StringView sv, const char *chars);
StringView sv_trim_left_by_cstr(StringView sv, const char *chars);
StringView sv_trim_left_to_cstr(StringView sv, const char *chars);
StringView sv_trim_right_to_cstr(StringView sv, const char *chars);
StringView sv_trim_space(StringView sv);
StringView sv_chop_by(StringView *sv, char c);
size_t sv_count_char(StringView sv, char c);
ptrdiff_t sv_index_char(StringView sv, char c);
ptrdiff_t sv_last_index_char(StringView sv, char c);
int sv_cmp(StringView a, StringView b);
bool sv_eq_cstr(StringView sv, const char *cstr);
bool sv_eq_sv(StringView a, StringView b);
StringView sv_dup(Allocator alloc, StringView sv);

#endif // INCLUDE_MEW_CORE_STRINGS_SV_H_
