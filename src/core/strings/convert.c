#include <mew/core/strings/convert.h>

#include <mew/core/strings/cstr.h>

StringView cstr_to_sv(const char *cstr) {
    return (StringView) {
        .items = cstr,
        .count = cstr_len(cstr),
    };
}

StringView sb_to_sv(StringBuilder sb) {
    return (StringView) {
        .items = sb.items,
        .count = sb.count,
    };
}
