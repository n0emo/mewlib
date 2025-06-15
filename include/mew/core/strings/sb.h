#ifndef INCLUDE_MEW_CORE_STRINGS_SB_H_
#define INCLUDE_MEW_CORE_STRINGS_SB_H_

#include <mew/core/alloc.h>
#include <mew/containers/vector.h>
#include <mew/core/strings/sv.h>

typedef struct StringBuilder {
    MewVector vec;
} StringBuilder;

void sb_init(StringBuilder *sb, Allocator alloc);
void sb_default(StringBuilder *sb);
void sb_destroy(StringBuilder *sb);
usize sb_count(const StringBuilder *sb);
void sb_set_count(StringBuilder *sb, usize count);
char *sb_begin(const StringBuilder *sb);
void sb_append_char(StringBuilder *sb, char c);
void sb_append_buf(StringBuilder *sb, const char *buf, size_t size);
void sb_append_cstr(StringBuilder *sb, const char *s);
void sb_append_sv(StringBuilder *sb, StringView sv);
void sb_append_sb(StringBuilder *sb, StringBuilder other);
StringBuilder sb_dup(Allocator alloc, StringBuilder sb);

#define SB_FMT "%.*s"
#define SB_ARG(sb) (int)(sb_count(&sb)), sb_begin(&sb)

// TODO: Guarantee NULL-terminator

#endif // INCLUDE_MEW_CORE_STRINGS_SB_H_
