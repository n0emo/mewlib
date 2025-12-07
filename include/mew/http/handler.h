#ifndef MEW_INCLUDE_MEW_HTTP_HANDLER_H_
#define MEW_INCLUDE_MEW_HTTP_HANDLER_H_

#include <mew/http/request.h>
#include <mew/http/response.h>

/**
 * Returns false in case of the error
 */
typedef bool HttpRequestHandler(HttpRequest *request, HttpResponse *response, void *user_data);

#endif // MEW_INCLUDE_MEW_HTTP_HANDLER_H_
