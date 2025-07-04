#include <mew/core/strings/sv.h>

#include <string.h>

#include <mew/core.h>

StringView sv_identity(StringView sv) {
    return sv;
}

int sv_cmp(StringView a, StringView b) {
    size_t min_size = a.count > b.count ? b.count : a.count;

    int cmp = strncmp(a.items, b.items, min_size);
    if (cmp != 0) {
        return cmp;
    }

    if (a.count < b.count) {
        return -1;
    }

    if (a.count > b.count) {
        return 1;
    }

    return 0;
}

bool sv_eq_cstr(StringView sv, const char *cstr) {
    return strlen(cstr) == sv.count && strncmp(sv.items, cstr, sv.count) == 0;
}

bool sv_eq_sv(StringView a, StringView b) {
    if (a.count != b.count)
        return false;
    return memcmp(a.items, b.items, a.count) == 0;
}

bool sv_starts_with(StringView sv, StringView prefix) {
    if (sv.count < prefix.count)
        return false;
    return strncmp(sv.items, prefix.items, prefix.count) == 0;
}

bool sv_starts_with_cstr(StringView sv, const char *prefix) {
    return sv_starts_with(sv, cstr_to_sv(prefix));
}

bool sv_contains(StringView sv, char c) {
    for (size_t i = 0; i < sv.count; i++) {
        if (sv.items[i] == c)
            return true;
    }

    return false;
}

StringView sv_slice(StringView sv, size_t start, size_t count) {
    if (sv.count < start + count)
        return (StringView) {0};

    return (StringView) {
        .items = sv.items + start,
        .count = count,
    };
}

StringView sv_slice_from(StringView sv, size_t index) {
    if (sv.count < index) {
        return (StringView) {0};
    }

    return (StringView) {
        .items = sv.items + index,
        .count = sv.count - index,
    };
}

StringView sv_trim_left_by_cstr(StringView sv, const char *chars) {
    StringView result = sv;

    while (result.count > 0 && strchr(chars, result.items[result.count - 1]) != NULL) {
        result.count--;
    }

    return result;
}

StringView sv_trim_right_by_cstr(StringView sv, const char *chars) {
    StringView result = sv;

    while (result.count > 0 && strchr(chars, result.items[0]) != NULL) {
        result = sv_slice_from(sv, 1);
    }

    return result;
}

StringView sv_trim_left_to_cstr(StringView sv, const char *chars) {
    StringView result = sv;

    while (result.count > 0 && strchr(chars, result.items[result.count - 1]) == NULL) {
        result.count--;
    }

    return result;
}

StringView sv_trim_right_to_cstr(StringView sv, const char *chars) {
    StringView result = sv;

    while (result.count > 0 && strchr(chars, result.items[0]) == NULL) {
        result = sv_slice_from(sv, 1);
    }

    return result;
}

StringView sv_trim_space(StringView sv) {
    const char *chars = " \r\n\t";
    return sv_trim_right_by_cstr(sv_trim_left_by_cstr(sv, chars), chars);
}

StringView sv_chop_by(StringView *sv, char c) {
    StringView result = {
        .items = sv->items,
        .count = 0,
    };

    while (sv->count > 0 && sv->items[0] != c) {
        result.count++;
        *sv = sv_slice_from(*sv, 1);
    }

    if (sv->count > 0) {
        *sv = sv_slice_from(*sv, 1);
    }

    return result;
}

size_t sv_count_char(StringView sv, char c) {
    size_t count = 0;
    for (size_t i = 0; i < sv.count; i++) {
        if (sv.items[i] == c)
            count++;
    }
    return count;
}

ptrdiff_t sv_index_char(StringView sv, char c) {
    for (ptrdiff_t i = 0; i < (ptrdiff_t)sv.count; i++) {
        if (sv.items[i] == c)
            return i;
    }
    return -1;
}

ptrdiff_t sv_last_index_char(StringView sv, char c) {
    for (ptrdiff_t i = (ptrdiff_t)sv.count - 1; i >= 0; i--) {
        if (sv.items[i] == c)
            return i;
    }
    return -1;
}

StringView sv_dup(Allocator alloc, StringView sv) {
    return (StringView) {
        .items = mem_memdup(alloc, sv.items, sv.count),
        .count = sv.count,
    };
}
