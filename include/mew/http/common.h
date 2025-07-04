#ifndef MEW_INCLUDE_MEW_HTTP_COMMON_H_
#define MEW_INCLUDE_MEW_HTTP_COMMON_H_

#include <stdbool.h>

#include <mew/core/strings/sb.h>
#include <mew/core/strings/sv.h>

typedef enum {
    HTTP_OK = 200,
    HTTP_NOT_FOUND = 404,
    HTTP_INTERNAL_SERVER_ERROR = 500,
} HttpStatus;

typedef enum {
    HTTP_GET,
    HTTP_POST,
} HttpMethod;

const char *http_status_desc(HttpStatus status);
const char *http_method_str(HttpMethod method);

bool http_urldecode(StringView sv, StringBuilder *out);
void http_urlencode(StringView sv, StringBuilder *out);

#endif // MEW_INCLUDE_MEW_HTTP_COMMON_H_
