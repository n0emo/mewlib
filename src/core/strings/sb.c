#include <mew/core/strings/sb.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <mew/core/allocators/malloc.h>

void sb_init(StringBuilder *sb, Allocator alloc) {
    mew_vec_init(&sb->vec, alloc, sizeof(char));
}

void sb_init_default(StringBuilder *sb) {
    sb_init(sb, new_malloc_allocator());
}

StringBuilder sb_new(Allocator alloc) {
    StringBuilder sb;
    sb_init(&sb, alloc);
    return sb;
}

StringBuilder sb_new_default(void) {
    StringBuilder sb;
    sb_init_default(&sb);
    return sb;
}

void sb_destroy(StringBuilder *sb) {
    mew_vec_destroy(&sb->vec);
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

void sb_appendf(StringBuilder *sb, const char *fmt, ...) {
    va_list args;
    va_list args_copy;

    va_start(args, fmt);
    va_copy(args_copy, args);
    int n = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    usize buf_size = n + 1;
    // TODO: We don't really need allocation *here*
    char *buf = mem_alloc(sb->vec.alloc, buf_size);

    vsnprintf(buf, buf_size, fmt, args);
    sb_append_cstr(sb, buf);
    va_end(args);

    mem_free(sb->vec.alloc, buf);
}

StringBuilder sb_dup(Allocator alloc, StringBuilder sb) {
    MewVector vec;
    mew_vec_init(&vec, alloc, sizeof(char));
    mew_vec_copy_to(&vec, &sb.vec);
    return (StringBuilder) {vec};
}

bool sb_eq_sb(StringBuilder a, StringBuilder b) {
    if (a.vec.count != b.vec.count) {
        return false;
    }

    return strncmp(a.vec.data, b.vec.data, a.vec.count) == 0;
}
