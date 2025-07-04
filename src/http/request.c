#include <mew/http/request.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <mew/core.h>
#include <mew/http/headermap.h>
#include <mew/log.h>

bool read_request_header_lines(MewTcpStream stream, StringBuilder *header, StringBuilder *body);
void request_trim_cr(StringView *sv);

void http_path_init(HttpPathParams *params, Allocator alloc) {
    hashmap_init(&params->map, NULL, hashmap_sv_hash, hashmap_sv_equals, sizeof(StringView), sizeof(StringView));
    params->map.alloc = alloc;
    params->is_set = false;
}

void http_path_set(HttpPathParams *params, StringView key, StringView value) {
    hashmap_insert(&params->map, &key, &value);
}

StringView *http_path_get(HttpPathParams *params, StringView key) {
    return hashmap_get(&params->map, &key);
}

#define BUF_CAP 8192

bool http_request_init(HttpRequest *request, Allocator alloc) {
    memset(request, 0, sizeof(*request));
    request->ctx.alloc = alloc;
    sb_init(&request->body, alloc);
    http_headermap_init(&request->headers, alloc);
    http_path_init(&request->ctx.path_params, alloc);
    return true;
}

bool http_request_parse(HttpRequest *request, MewTcpStream stream) {
    StringBuilder header = sb_new(request->ctx.alloc);

    if (!read_request_header_lines(stream, &header, &request->body)) {
        return false;
    }
    StringView sv = sb_to_sv(header);

    StringView status_line = sv_chop_by(&sv, '\n');
    request_trim_cr(&status_line);

    StringView method = sv_chop_by(&status_line, ' ');

    if (sv_eq_cstr(method, "GET")) {
        request->method = HTTP_GET;
    } else if (sv_eq_cstr(method, "POST")) {
        request->method = HTTP_POST;
    } else {
        return false;
    }

    request->resource_path = sv_dup(request->ctx.alloc, sv_chop_by(&status_line, ' '));
    request->version = sv_dup(request->ctx.alloc, sv_chop_by(&status_line, '\n'));

    StringView header_line;
    while (true) {
        header_line = sv_chop_by(&sv, '\n');

        if (header_line.count == 0) {
            break;
        }

        request_trim_cr(&header_line);

        StringView key = sv_dup(request->ctx.alloc, sv_chop_by(&header_line, ':'));
        StringView value = sv_dup(request->ctx.alloc, sv_slice_from(header_line, 1));
        http_headermap_insert(&request->headers, (HttpHeader) {key, value});
    }

    StringView resource_path = request->resource_path;
    StringBuilder sb = sb_new(request->ctx.alloc);
    StringView path = sv_chop_by(&resource_path, '?');
    if (!http_urldecode(path, &sb)) {
        return false;
    }
    request->ctx.path = sb_to_sv(sb);

    request->ctx.query_string = resource_path;

    return true;
}

bool read_request_header_lines(MewTcpStream stream, StringBuilder *header, StringBuilder *body) {
    char buf[BUF_CAP] = {0};

    size_t count = 0;

    while (true) {
        ptrdiff_t bytes = mew_tcpstream_read(stream, buf, BUF_CAP);
        if (bytes < 0) {
            log_error("%s", strerror(errno));
            return false;
        }

        sb_append_buf(header, buf, (size_t)bytes);
        sb_append_char(header, '\0');
        sb_count(header)++;

        const char *body_ptr = strstr(sb_begin(header) + count, "\r\n\r\n");
        count = sb_count(header);
        size_t newlines = 4;

        if (body_ptr == NULL) {
            body_ptr = strstr(sb_begin(header), "\n\n");
            newlines = 2;
        }

        if (body_ptr != NULL) {
            size_t header_size = (size_t)(body_ptr - sb_begin(header));
            sb_append_buf(body, body_ptr + newlines, sb_count(header) - header_size - newlines);
            sb_count(header) = header_size;
            return true;
        }
    }
}

void request_trim_cr(StringView *sv) {
    assert(sv->count > 0);
    if (sv->items[sv->count - 1] == '\r')
        sv->count--;
}
