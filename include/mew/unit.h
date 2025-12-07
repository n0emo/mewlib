#ifndef MEW_INCLUDE_UNIT_H_
#define MEW_INCLUDE_UNIT_H_

#include <stdio.h>

#include <mew/core.h>

#define MEW_STRINGIZE_DETAIL(x) #x
#define MEW_STRINGIZE(x) MEW_STRINGIZE_DETAIL(x)

#define MEW_TEST_WHERE "File '" __FILE__ "', line " MEW_STRINGIZE(__LINE__)

#ifdef __cplusplus
    #define INITIALIZER(f)                                                                                             \
        static void f(void);                                                                                           \
        struct f##_t_ {                                                                                                \
            f##_t_(void) {                                                                                             \
                f();                                                                                                   \
            }                                                                                                          \
        };                                                                                                             \
        static f##_t_ f##_;                                                                                            \
        static void f(void)
#elif defined(_MSC_VER)
    #pragma section(".CRT$XCU", read)
    #define INITIALIZER2_(f, p)                                                                                        \
        static void f(void);                                                                                           \
        __declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f;                                                       \
        __pragma(comment(linker, "/include:" p #f "_")) static void f(void)
    #ifdef _WIN64
        #define INITIALIZER(f) INITIALIZER2_(f, "")
    #else
        #define INITIALIZER(f) INITIALIZER2_(f, "_")
    #endif
#else
    #define INITIALIZER(f)                                                                                             \
        static void f(void) __attribute__((constructor));                                                              \
        static void f(void)
#endif

typedef const char *(mew_test_func_t)(void);

typedef struct MewTest {
    mew_test_func_t *func;
    const char *name;
} MewTest;

#define TEST(test_name, ...)                                                                                           \
    extern MewTest mew_tests[];                                                                                        \
                                                                                                                       \
    static const char *test_##test_name(void) {                                                                        \
        __VA_ARGS__;                                                                                                   \
        return NULL;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    INITIALIZER(register_##test_name) {                                                                                \
        mew_tests[__COUNTER__] = (MewTest) {                                                                           \
            .func = test_##test_name,                                                                                  \
            .name = "test_" #test_name,                                                                                \
        };                                                                                                             \
    }

#define mewassert(message, ...)                                                                                        \
    do {                                                                                                               \
        if (!(__VA_ARGS__)) {                                                                                          \
            return MEW_TEST_WHERE ":\n      Assertion `" #__VA_ARGS__ "` failed: " message;                            \
        }                                                                                                              \
    } while (0)

#define mewtest_main(...)                                                                                              \
    MewTest mew_tests[__COUNTER__];                                                                                    \
    const size_t mew_tests_count = sizeof(mew_tests) / sizeof(*mew_tests);                                             \
                                                                                                                       \
    int main() {                                                                                                       \
        __VA_ARGS__;                                                                                                   \
                                                                                                                       \
        size_t passed = 0;                                                                                             \
        for (size_t i = 0; i < mew_tests_count; i++) {                                                                 \
            printf("    Running '%s'... ", mew_tests[i].name);                                                         \
            fflush(stdout);                                                                                            \
                                                                                                                       \
            const char *result = mew_tests[i].func();                                                                  \
            if (result == NULL) {                                                                                      \
                printf("OK\n");                                                                                        \
                passed++;                                                                                              \
            } else {                                                                                                   \
                printf("FAIL\n\n    %s.\n\n", result);                                                                 \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        printf("\n    Passed: %zu/%zu tests.\n", passed, mew_tests_count);                                             \
        return passed == mew_tests_count ? 0 : 1;                                                                      \
    }
#endif // MEW_INCLUDE_UNIT_H_
