#ifndef INCLUDE_MEW_CORE_UTILS_H_
#define INCLUDE_MEW_CORE_UTILS_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "alloc.h" // IWYU pragma: export

/********* Error handling **********/

#define return_defer(ret)                                                                                              \
    do {                                                                                                               \
        result = ret;                                                                                                  \
        goto defer;                                                                                                    \
    } while (0)

#define try(...)                                                                                                       \
    if (!(__VA_ARGS__))                                                                                                \
    return_defer(false)

/************ Other **************/

#endif // INCLUDE_MEW_CORE_UTILS_H_
