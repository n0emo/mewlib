#include <mew/core/strings/sb.h>

#include <mew/core/utils.h>
#include <mew/core/allocators/malloc.h>

void sb_init(StringBuilder *sb, Allocator alloc) {
    mew_vec_init(&sb->vec, alloc, sizeof(char));
}

void sb_default(StringBuilder *sb) {
    sb_init(sb, new_malloc_allocator());
}

void sb_destroy(StringBuilder *sb) {
    mew_vec_destroy(&sb->vec);
}

usize sb_count(const StringBuilder *sb) {
    return sb->vec.count;
}

void sb_set_count(StringBuilder *sb, usize count) {
    sb->vec.count = count;
}

char *sb_begin(const StringBuilder *sb) {
    return sb->vec.data;
}

void sb_append_char(StringBuilder *sb, char c) {
    mew_vec_push(&sb->vec, &c);
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
    sb_append_buf(sb, other.vec.data, other.vec.count);
}

StringBuilder sb_dup(Allocator alloc, StringBuilder sb) {
    MewVector vec;
    mew_vec_init(&vec, alloc, sizeof(char));
    mew_vec_copy_to(&vec, &sb.vec);
    return (StringBuilder) { vec };
}
