#include <mew/core/strings/cstr.h>

bool str_contains(const char *s, char c) {
    while (*s != '\0') {
        if (*s == c)
            return true;
        s++;
    }
    return false;
}

usize cstr_len(const char *s) {
    usize result = 0;
    while (*s != '\0') {
        s++;
        result++;
    }
    return result;
}
