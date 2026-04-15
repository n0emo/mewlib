#define LOG_WITH_FILE
#define MEW_HTML_SHORT_TAGS
#include <mew/http.h>
#include <mew/html.h>
#include <mew/log.h>

#include <dirent.h>

bool handler(HttpRequest *request, HttpResponse *response, void *user_data);
void render_html(HttpResponse *response, Allocator alloc, StringView path, MewVector entries);

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
    Allocator alloc = request->ctx.alloc;
    ServeResult result = try_serve_dir(response, request->resource_path, cstr_to_sv("."));
    switch (result) {
        case MEW_SERVE_SUCCESS: {
            response->status = 200;
            return true;
        }
        case MEW_SERVE_NOT_FOUND: {
            response->status = 404;
            return true;
        }
        case MEW_SERVE_IS_DIR: {
            DIR *dir;
            struct dirent *ent;
            MewVector entries;
            mew_vec_init(&entries, alloc, sizeof(const char *));
            const char *path = mem_sprintf(alloc, "./"SV_FMT, SV_ARG(request->resource_path));
            if ((dir = opendir(path)) != NULL) {
                while ((ent = readdir(dir)) != NULL) {

                    const char *name;
                    if (ent->d_type & DT_DIR) {
                        name = mem_sprintf(alloc, "%.*s/", (int)ent->d_namlen, ent->d_name);
                    } else {
                        name = mem_sprintf(alloc, "%.*s", (int)ent->d_namlen, ent->d_name);
                    }
                    mew_vec_push(&entries, &name);
                }
                closedir(dir);
            } else {
                response->status = 404;
                return true;
            }

            response->status = 200;
            render_html(response, request->ctx.alloc, request->resource_path, entries);
            return true;
        }
    }
}

static const char *style =
    "body {\n"
    "  background: #D3D3D3;\n"
    "}"
    "\n"
    ".ent {\n"
    "  background: ;\n"
    "}"
    "\n"
    ".main {\n"
    "  display: flex;\n"
    "  justify-items: center ;\n"
    "  flex-direction: column;\n"
    "}"
    "\n"
    "ul {\n"
    "  list-style-type: none;\n"
    "}"
    "li {\n"
    "  margin-bottom: 2px;\n"
    "  font-size: 1.2em;\n"
    "}"
    "\n"
    "a {\n"
    "  text-decoration: none;\n"
    "  color : black;\n"
    "}"
    "\n"
    "a.dir {\n"
    "  color : blue;\n"
    "}"
    "\n"
    "a.file {\n"
    "  color : black;\n"
    "}"
    "\n"
    "a:hover {\n"
    "  text-decoration: underline;\n"
    "  font-weight: bold;\n"
    "}";

void render_html(HttpResponse *response, Allocator alloc, StringView path, MewVector entries) {
    Html h = html_begin();

    const char *title = mem_sprintf(alloc, "Listing of " SV_FMT, SV_ARG(path));

    h_head(h, {
        ht_title(h, title);
        html_style_begin(&h);
        html_text(&h, cstr_to_sv(style));
        html_style_end(&h);
    });

    h_body(h, {
        h_h1(h, ht_text(h, title));
        ht_attr(h, "class", "main");
        h_div(h, {
            h_ul(h, {
                for (size_t i = 0; i < entries.count; i++) {
                    h_li(h, {
                        const char *entry_cstr = *(const char **)mew_vec_get(&entries, i);
                        StringView entry = cstr_to_sv(entry_cstr);
                        ht_attr(h, "href", mem_sprintf(alloc, "./" SV_FMT, SV_ARG(entry)));
                        ht_attr(h, "class", "ent");
                        if (entry.items[entry.count - 1] == '/') {
                            ht_class(h, "dir");
                        } else {
                            ht_class(h, "file");
                        }
                        h_a(h, ht_text(h, entry));
                    });
                }
            });
        });
    });

    html_end(&h);

    http_response_body_set_bytes(response);
    html_render_to_sb_and_free(&h, &response->body.as.bytes);
    http_response_set_html(response);
}
