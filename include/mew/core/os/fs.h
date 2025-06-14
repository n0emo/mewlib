#ifndef MEW_INCLUDE_MEW_CORE_OS_FS_H_
#define MEW_INCLUDE_MEW_CORE_OS_FS_H_

#include <mew/core/types.h>
#include <mew/core/strings/sb.h>

bool mew_fs_exists(const char *path);
bool mew_fs_get_size(const char *path, usize *size);
bool mew_fs_read_file_to_sb(const char *path, StringBuilder *asb);

#endif // MEW_INCLUDE_MEW_CORE_OS_FS_H_
