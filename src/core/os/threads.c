#include <mew/core/os/threads.h>

const char *mew_thread_error_description(MewThreadError error) {
    switch (error) {
        case MEW_THREAD_SUCCESS:
            return "success";
        case MEW_THREAD_ERROR_PERMISSIONS:
            return "operation not permitted";
        case MEW_THREAD_ERROR_NOT_FOUND:
            return "resource not found";
        case MEW_THREAD_ERROR_TRY_AGAIN:
            return "resource temporarily unavailable";
        case MEW_THREAD_ERROR_OUT_OF_MEMORY:
            return "could not allocate memory";
        case MEW_THREAD_ERROR_BUSY:
            return "resource busy";
        case MEW_THREAD_ERROR_INVALID_ARGUMENT:
            return "invalid argument";
        case MEW_THREAD_ERROR_DEADLOCK:
            return "deadlock prevented";
        case MEW_THREAD_ERROR_UNKNOWN:
            return "unknown";
    }

    return "unknown";
}
