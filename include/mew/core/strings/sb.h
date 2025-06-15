#ifndef INCLUDE_MEW_CORE_STRINGS_SB_H_
#define INCLUDE_MEW_CORE_STRINGS_SB_H_

#include <mew/core/alloc.h>
#include <mew/containers/vector.h>
#include <mew/core/strings/sv.h>

typedef struct StringBuilder {
    MewVector vec;
} StringBuilder;

void sb_init(StringBuilder *sb, Allocator alloc);
void sb_init_default(StringBuilder *sb);
StringBuilder sb_new(Allocator alloc);
StringBuilder sb_new_default(void);
void sb_destroy(StringBuilder *sb);
void sb_append_char(StringBuilder *sb, char c);
void sb_append_buf(StringBuilder *sb, const char *buf, size_t size);
void sb_append_cstr(StringBuilder *sb, const char *s);
StringBuilder sb_dup(Allocator alloc, StringBuilder sb);

#define sb_count(sb) (sb)->vec.count
#define sb_begin(sb) (sb)->vec.data
#define sb_append_sv(sb, sv) sb_append_buf(sb, (sv).items, (sv).count)
#define sb_append_sb(sb, other) sb_append_buf(sb, (other).vec.data, (other).vec.count);

#define SB_FMT "%.*s"
#define SB_ARG(sb) (int)(sb_count(&sb)), sb_begin(&sb)

// TODO: Guarantee NULL-terminator

#endif // INCLUDE_MEW_CORE_STRINGS_SB_H_
