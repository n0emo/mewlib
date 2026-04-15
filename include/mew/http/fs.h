#ifndef MEW_INCLUDE_MEW_HTTP_FS_H_
#define MEW_INCLUDE_MEW_HTTP_FS_H_

#include <stdbool.h>

#include <mew/http/response.h>

typedef enum ServeResult {
    MEW_SERVE_SUCCESS,
    MEW_SERVE_NOT_FOUND,
    MEW_SERVE_IS_DIR,
} ServeResult;

// TODO: cached serve dir
// TODO: as handler interface
ServeResult try_serve_dir(HttpResponse *response, StringView file, StringView dir);
void serve_dir(HttpResponse *response, StringView file, StringView dir);

#endif // MEW_INCLUDE_MEW_HTTP_FS_H_
