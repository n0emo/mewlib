#define LOG_WITH_FILE
#include <mew/http.h>
#include <mew/log.h>

bool handler(HttpRequest *request, HttpResponse *response, void *user_data);

int main(void) {
    HttpServerSettings settings;
    settings.port = 8080;
    settings.host = "127.0.0.1";
    settings.handler = handler;
    HttpServer server;
    if (!http_server_init(&server, settings)) {
        log_error("Error initializing server");
        return 1;
    }

    http_server_start(&server);

    return 0;
}

bool handler(HttpRequest *request, HttpResponse *response, void *user_data) {
    serve_dir(response, request->resource_path, cstr_to_sv("."));
    log_info(
        "%s " SV_FMT ": %d",
        http_method_str(request->method),
        SV_ARG(request->resource_path),
        response->status
    );
    return true;
}
