#ifndef INCLUDE_MEW_CORE_STRINGS_CONVERT_H_
#define INCLUDE_MEW_CORE_STRINGS_CONVERT_H_

#include <mew/core/strings/sv.h>
#include <mew/core/strings/sb.h>

StringView cstr_to_sv(const char *cstr);
StringView sb_to_sv(StringBuilder sb);

#endif // INCLUDE_MEW_CORE_STRINGS_CONVERT_H_
