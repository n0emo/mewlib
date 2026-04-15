#ifndef MEW_INCLUDE_MEW_CORE_OS_ERROR_H_
#define MEW_INCLUDE_MEW_CORE_OS_ERROR_H_

#include <mew/core/types.h>

#define MEW_OS_ERROR_LIST                                                                                              \
    X(ADDRESS_IN_USE, address in use)                                                                                  \
    X(ADDRESS_NOT_AVAILABLE, address not available)                                                                    \
    X(ALREADY_EXISTS, entity already exists)                                                                           \
    X(ARGUMENT_LIST_TOO_LONG, argument list too long)                                                                  \
    X(BROKEN_PIPE, broken pipe)                                                                                        \
    X(CONNECTION_ABORTED, connection aborted)                                                                          \
    X(CONNECTION_REFUSED, connection refused)                                                                          \
    X(CONNECTION_RESET, connection reset)                                                                              \
    X(CROSSES_DEVICES, cross - device link or rename)                                                                  \
    X(DEADLOCK, deadlock)                                                                                              \
    X(DIRECTORY_NOT_EMPTY, directory not empty)                                                                        \
    X(EXECUTABLE_FILE_BUSY, executable file busy)                                                                      \
    X(FILE_SYSTEM_LOOP, filesystem loop or indirection limit(e.g.symlink loop))                                        \
    X(FILE_TOO_LARGE, file too large)                                                                                  \
    X(HOST_UNREACHABLE, host unreachable)                                                                              \
    X(INTERRUPTED, operation interrupted)                                                                              \
    X(INVALID_DATA, invalid data)                                                                                      \
    X(INVALID_FILENAME, invalid filename)                                                                              \
    X(INVALID_INPUT, invalid input parameter)                                                                          \
    X(IN_PROGRESS, in progress)                                                                                        \
    X(IS_A_DIRECTORY, is a directory)                                                                                  \
    X(NETWORK_DOWN, network down)                                                                                      \
    X(NETWORK_UNREACHABLE, network unreachable)                                                                        \
    X(NOT_A_DIRECTORY, not a directory)                                                                                \
    X(NOT_CONNECTED, not connected)                                                                                    \
    X(NOT_FOUND, entity not found)                                                                                     \
    X(NOT_SEEKABLE, seek on unseekable file)                                                                           \
    X(OTHER, other error)                                                                                              \
    X(OUT_OF_MEMORY, out of memory)                                                                                    \
    X(PERMISSION_DENIED, permission denied)                                                                            \
    X(QUOTA_EXCEEDED, quota exceeded)                                                                                  \
    X(READ_ONLY_FILESYSTEM, read - only filesystem or storage medium)                                                  \
    X(RESOURCE_BUSY, resource busy)                                                                                    \
    X(STALE_NETWORK_FILE_HANDLE, stale network file handle)                                                            \
    X(STORAGE_FULL, no storage space)                                                                                  \
    X(TIMED_OUT, timed out)                                                                                            \
    X(TOO_MANY_LINKS, too many links)                                                                                  \
    X(UNCATEGORIZED, uncategorized error)                                                                              \
    X(UNEXPECTED_EOF, unexpected end of file)                                                                          \
    X(UNSUPPORTED, unsupported)                                                                                        \
    X(WOULD_BLOCK, operation would block)                                                                              \
    X(WRITE_ZERO, write zero)

typedef enum MewOsError {
    MEW_OS_SUCCESS = 0,

#define X(e, msg) MEW_OS_ERROR_##e,
    MEW_OS_ERROR_LIST
#undef X

} MewOsError;

MewOsError mew_os_error_from_raw(i64 code);
MewOsError mew_last_os_error(void);
const char *mew_os_error_description(MewOsError);
const char *mew_last_os_error_description(void);

#endif //  MEW_INCLUDE_MEW_CORE_OS_ERROR_H_
