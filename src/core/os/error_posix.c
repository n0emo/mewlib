#include <mew/core/os/error.h>

#include <sys/errno.h>

MewOsError mew_os_error_from_raw(i64 code) {
    return (MewOsError)code;
}

MewOsError mew_last_os_error(void) {
    return mew_os_error_from_raw(errno);
}

const char *mew_os_error_description(MewOsError error) {
    switch (error) {
        case MEW_OS_SUCCESS:
            return "success";
#define X(e, msg)                                                                                                      \
    case MEW_OS_ERROR_##e:                                                                                              \
        return #msg;
            MEW_OS_ERROR_LIST
#undef X
    }
}

const char *mew_last_os_error_description(void) {
    return mew_os_error_description(mew_last_os_error());
}
