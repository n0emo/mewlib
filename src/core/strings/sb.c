#include <mew/core/strings/sb.h>

#include <mew/core/utils.h>

void sb_append_char(StringBuilder *sb, char c) {
    ARRAY_APPEND(sb, c, sb->alloc);
}

void sb_append_buf(StringBuilder *sb, const char *buf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        sb_append_char(sb, buf[i]);
    }
}

void sb_append_cstr(StringBuilder *sb, const char *s) {
    while (*s != '\0') {
        sb_append_char(sb, *s);
        s++;
    }
}

void sb_append_sv(StringBuilder *sb, StringView sv) {
    sb_append_buf(sb, sv.items, sv.count);
}

void sb_append_sb(StringBuilder *sb, StringBuilder other) {
    sb_append_buf(sb, other.items, other.count);
}

StringBuilder sb_dup(Allocator alloc, StringBuilder sb) {
    return (StringBuilder) {
        .alloc = alloc,
        .items = mem_memdup(alloc, sb.items, sb.capacity),
        .count = sb.count,
        .capacity = sb.capacity,
    };
}
