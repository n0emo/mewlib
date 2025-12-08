#ifndef INCLUDE_MEW_CORE_OS_MACROS_H_
#define INCLUDE_MEW_CORE_OS_MACROS_H_

#ifdef __GNUC__
    #define MEW_ATTRIBUTE_UNUSED __attribute__((unused))
#else
    #define MEW_ATTRIBUTE_UNUSED
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1400
    #include <sal.h>
    #if _MSC_VER > 1400
        #define MEW_FORMAT_STRING(p) _Printf_format_string_ p
    #else
        #define MEW_FORMAT_STRING(p) __format_string p
    #endif // FORMAT_STRING
#else
    #define MEW_FORMAT_STRING(p) p
#endif // _MSC_VER

#endif // INCLUDE_MEW_CORE_OS_MACROS_H_
