#ifndef MEW_INCLUDE_MEW_HTTP_SERVER_H_
#define MEW_INCLUDE_MEW_HTTP_SERVER_H_

#include <mew/core/types.h>
#include <mew/core/os/socket.h>
#include <mew/http/handler.h>
#include <mew/thrdpool.h>

typedef struct HttpServerSettings {
    u16 port;
    const char *host;
    HttpRequestHandler *handler;
    void *user_data;
} HttpServerSettings;

typedef struct HttpServer {
    ThreadPool thread_pool;
    HttpRequestHandler *handler;
    void *user_data;
    MewTcpListener listener;
    HttpServerSettings settings;
} HttpServer;

bool http_server_init(HttpServer *server, HttpServerSettings settings);
void http_server_destroy(HttpServer *server);
bool http_server_start(HttpServer *server);

#endif // MEW_INCLUDE_MEW_HTTP_SERVER_H_
