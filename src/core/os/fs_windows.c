#include <mew/core/os/fs.h>

#include <stdlib.h>

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>

bool mew_fs_exists(const char *path) {
    // TODO: convert from UTF-8 to UTF-16
    DWORD fileAttributes = GetFileAttributesA(path);

    if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    return true;
}

bool mew_fs_get_size(const char *path, usize *size) {
    bool result = false;
    HANDLE hfile = INVALID_HANDLE_VALUE;

    if (size == NULL) {
        goto defer;
    }

    hfile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hfile == INVALID_HANDLE_VALUE) {
        goto defer;
    }

    LARGE_INTEGER file_size;
    if (!GetFileSizeEx(hfile, &file_size)) {
        goto defer;
    }

    CloseHandle(hfile);

    *size = (usize)file_size.QuadPart;
    result = true;

defer:
    if (hfile != INVALID_HANDLE_VALUE) {
        CloseHandle(hfile);
    }
    return result;
}

bool mew_fs_read_file_to_sb(const char *path, StringBuilder *asb) {
    bool result = false;
    HANDLE hfile = INVALID_HANDLE_VALUE;
    char *buf = NULL;

    if (asb == NULL) {
        goto defer;
    }

    hfile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hfile == INVALID_HANDLE_VALUE) {
        goto defer;
    }

    LARGE_INTEGER file_size;
    if (!GetFileSizeEx(hfile, &file_size)) {
        goto defer;
    }

    if (file_size.QuadPart > MAXDWORD) {
        goto defer;
    }

    DWORD file_size_32 = (DWORD)file_size.QuadPart;

    char *buffer = (char *)malloc(file_size_32);
    if (buffer == NULL) {
        goto defer;
    }

    DWORD bytes_read;
    if (!ReadFile(hfile, buffer, file_size_32, &bytes_read, NULL)) {
        goto defer;
    }

    if (bytes_read != file_size_32) {
        goto defer;
    }

    sb_append_buf(asb, buffer, file_size_32);

    result = true;

defer:
    if (buf != NULL) {
        free(buffer);
    }
    if (hfile != NULL) {
        CloseHandle(hfile);
    }
    return result;
}
