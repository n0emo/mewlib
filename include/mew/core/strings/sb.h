#ifndef INCLUDE_MEW_CORE_STRINGS_SB_H_
#define INCLUDE_MEW_CORE_STRINGS_SB_H_

#include <mew/core/alloc.h>
#include <mew/core/strings/sv.h>

typedef struct {
    Allocator alloc;
    char *items;
    size_t count;
    size_t capacity;
} StringBuilder;

void sb_append_char(StringBuilder *sb, char c);
void sb_append_buf(StringBuilder *sb, const char *buf, size_t size);
void sb_append_cstr(StringBuilder *sb, const char *s);
void sb_append_sv(StringBuilder *sb, StringView sv);
void sb_append_sb(StringBuilder *sb, StringBuilder other);
StringBuilder sb_dup(Allocator alloc, StringBuilder sb);

#endif // INCLUDE_MEW_CORE_STRINGS_SB_H_
