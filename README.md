# mewlib

A zero-dependency HTTP library for C.

## Features

- HTTP server with thread pool
- HTML builder with macro-based DSL
- File serving utilities
- Containers: vector, hashmap, assoc map, red-black tree
- String utilities: string view, string builder
- Cross-platform: Linux, macOS, Windows
- Arena and malloc allocators

## Building

Requires CMake and Ninja.

```sh
# Configure
cmake -S . -B build -G 'Ninja' -DCMAKE_C_COMPILER=gcc

# Build
cmake --build build

# Test
cd build && ctest --output-on-failure
```

## Quick Start

**Simple HTTP file server:**

```c
#include <mew/http.h>

bool handler(HttpRequest *request, HttpResponse *response, void *user_data) {
    serve_dir(response, request->resource_path, cstr_to_sv("."));
    return true;
}

int main(void) {
    HttpServerSettings settings = {
        .port = 8080,
        .host = "127.0.0.1",
        .handler = handler,
    };
    HttpServer server;
    http_server_init(&server, settings);
    http_server_start(&server);
}
```

**HTML builder:**

```c
#define MEW_HTML_SHORT_TAGS
#include <mew/html.h>

Html h = html_begin();
h_body(h, {
    h_h1(h, ht_text(h, "Hello, world!"));
});
html_end(&h);
```
