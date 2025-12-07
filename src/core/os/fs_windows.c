#include <mew/core/os/fs.h>

#include <stdio.h>
#include <stdlib.h>

#define MEW_FUNC_NOT_IMPLEMENTED()                                                                                     \
    do {                                                                                                               \
        fprintf(stderr, "Not yet implemented: %s", __FUNCTION__);                                                      \
        abort();                                                                                                       \
    } while (0)

bool mew_fs_exists(const char *path) {
    MEW_FUNC_NOT_IMPLEMENTED();
}

bool mew_fs_get_size(const char *path, usize *size) {
    MEW_FUNC_NOT_IMPLEMENTED();
}

bool mew_fs_read_file_to_sb(const char *path, StringBuilder *asb) {
    MEW_FUNC_NOT_IMPLEMENTED();
}
