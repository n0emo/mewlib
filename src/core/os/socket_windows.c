#include <mew/core/os/socket.h>

#include <windows.h>
#include <stdio.h>

#define MEW_FUNC_NOT_IMPLEMENTED()                                                                                     \
    do {                                                                                                               \
        fprintf(stderr, "Not yet implemented: %s", __FUNCTION__);                                                      \
        abort();                                                                                                       \
    } while (0)

mew_tcplistener_bind_t mew_tcplistener_native_bind;
mew_tcplistener_listen_t mew_tcplistener_native_listen;
mew_tcplistener_accept_t mew_tcplistener_native_accept;
mew_tcplistener_close_t mew_tcplistener_native_close;

mew_tcpstream_set_timeout_t mew_tcpstream_native_set_timeout;
mew_tcpstream_read_t mew_tcpstream_native_read;
mew_tcpstream_write_t mew_tcpstream_native_write;
mew_tcpstream_sendfile_t mew_tcpstream_native_sendfile;
mew_tcpstream_close_t mew_tcpstream_native_close;

void mew_tcplistener_init_default_native_options(MewNativeTcpListenerOptions *options) {
    MEW_FUNC_NOT_IMPLEMENTED();
}

bool mew_tcplistener_init_native(MewTcpListener *listener, MewNativeTcpListenerOptions options) {
    MEW_FUNC_NOT_IMPLEMENTED();
}

bool mew_tcplistener_native_bind(void *data, const char *host, uint16_t port) {
    MEW_FUNC_NOT_IMPLEMENTED();
}

bool mew_tcplistener_native_listen(void *data, uint32_t max_connections) {
    MEW_FUNC_NOT_IMPLEMENTED();
}

bool mew_tcplistener_native_accept(void *data, MewTcpStream *stream) {
    MEW_FUNC_NOT_IMPLEMENTED();
}

bool mew_tcplistener_native_close(void *data) {
    MEW_FUNC_NOT_IMPLEMENTED();
}

bool mew_tcpstream_native_set_timeout(void *data, uint32_t seconds) {
    MEW_FUNC_NOT_IMPLEMENTED();
}

ptrdiff_t mew_tcpstream_native_read(void *data, char *buf, uintptr_t size) {
    MEW_FUNC_NOT_IMPLEMENTED();
}

ptrdiff_t mew_tcpstream_native_write(void *data, const char *buf, uintptr_t size) {
    MEW_FUNC_NOT_IMPLEMENTED();
}

bool mew_tcpstream_native_sendfile(void *data, const char *path, uintptr_t size) {
    MEW_FUNC_NOT_IMPLEMENTED();
}

bool mew_tcpstream_native_close(void *data) {
    MEW_FUNC_NOT_IMPLEMENTED();
}
