#include <mew/core/os/socket.h>

#define WIN32_LEAN_AND_MEAN

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <MSWSock.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

mew_tcplistener_bind_t mew_tcplistener_native_bind;
mew_tcplistener_listen_t mew_tcplistener_native_listen;
mew_tcplistener_accept_t mew_tcplistener_native_accept;
mew_tcplistener_close_t mew_tcplistener_native_close;

mew_tcpstream_set_timeout_t mew_tcpstream_native_set_timeout;
mew_tcpstream_read_t mew_tcpstream_native_read;
mew_tcpstream_write_t mew_tcpstream_native_write;
mew_tcpstream_sendfile_t mew_tcpstream_native_sendfile;
mew_tcpstream_close_t mew_tcpstream_native_close;

static int get_errno_from_winsock(void) {
    int err = WSAGetLastError();
    switch (err) {
        case WSAEINTR:
            return EINTR;
        case WSAEBADF:
            return EBADF;
        case WSAEACCES:
            return EACCES;
        case WSAEFAULT:
            return EFAULT;
        case WSAEINVAL:
            return EINVAL;
        case WSAEMFILE:
            return EMFILE;
        case WSAEWOULDBLOCK:
            return EWOULDBLOCK;
        case WSAEINPROGRESS:
            return EINPROGRESS;
        case WSAEALREADY:
            return EALREADY;
        case WSAENOTSOCK:
            return ENOTSOCK;
        case WSAEDESTADDRREQ:
            return EDESTADDRREQ;
        case WSAEMSGSIZE:
            return EMSGSIZE;
        case WSAEPROTOTYPE:
            return EPROTOTYPE;
        case WSAENOPROTOOPT:
            return ENOPROTOOPT;
        case WSAEPROTONOSUPPORT:
            return EPROTONOSUPPORT;
        case WSAEOPNOTSUPP:
            return EOPNOTSUPP;
        case WSAEAFNOSUPPORT:
            return EAFNOSUPPORT;
        case WSAEADDRINUSE:
            return EADDRINUSE;
        case WSAEADDRNOTAVAIL:
            return EADDRNOTAVAIL;
        case WSAENETDOWN:
            return ENETDOWN;
        case WSAENETUNREACH:
            return ENETUNREACH;
        case WSAENETRESET:
            return ENETRESET;
        case WSAECONNABORTED:
            return ECONNABORTED;
        case WSAECONNRESET:
            return ECONNRESET;
        case WSAENOBUFS:
            return ENOBUFS;
        case WSAEISCONN:
            return EISCONN;
        case WSAENOTCONN:
            return ENOTCONN;
        case WSAETIMEDOUT:
            return ETIMEDOUT;
        case WSAECONNREFUSED:
            return ECONNREFUSED;
        case WSAELOOP:
            return ELOOP;
        case WSAENAMETOOLONG:
            return ENAMETOOLONG;
        case WSAEHOSTUNREACH:
            return EHOSTUNREACH;
        case WSAENOTEMPTY:
            return ENOTEMPTY;
        default:
            return err;
    }
}

static bool mew_winsock_initialized = false;

static int mew_winsock_init(void) {
    if (!mew_winsock_initialized) {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            return -1;
        }
        mew_winsock_initialized = true;
    }
    return 0;
}

void mew_tcplistener_init_default_native_options(MewNativeTcpListenerOptions *options) {
    memset(options, 0, sizeof(*options));
    options->reuse_address = true;
}

bool mew_tcplistener_init_native(MewTcpListener *listener, MewNativeTcpListenerOptions options) {
    if (mew_winsock_init() != 0) {
        return false;
    }

    SOCKET sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sd == INVALID_SOCKET) {
        return false;
    }

    if (options.reuse_address) {
        char option = 1;
        int ret = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        if (ret == SOCKET_ERROR) {
            closesocket(sd);
            return false;
        }
    }

    // Enable TCP_NODELAY for better performance
    char nodelay = 1;
    setsockopt(sd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));

    listener->data = (void *)(uintptr_t)sd;
    listener->bind = &mew_tcplistener_native_bind;
    listener->listen = &mew_tcplistener_native_listen;
    listener->accept = &mew_tcplistener_native_accept;
    listener->close = &mew_tcplistener_native_close;
    return true;
}

bool mew_tcplistener_native_bind(void *data, const char *host, uint16_t port) {
    SOCKET sd = (SOCKET)(uintptr_t)data;

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (host == NULL || strcmp(host, "0.0.0.0") == 0 || strcmp(host, "::") == 0) {
        addr.sin_addr.s_addr = INADDR_ANY;
    } else {
        // Try to resolve hostname or parse IP address
        struct addrinfo hints, *result = NULL;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        char port_str[16];
        snprintf(port_str, sizeof(port_str), "%u", port);

        int ret = getaddrinfo(host, port_str, &hints, &result);
        if (ret != 0) {
            // Try direct IP address conversion
            ret = inet_pton(AF_INET, host, &addr.sin_addr);
            if (ret != 1) {
                return false;
            }
        } else {
            if (result != NULL) {
                struct sockaddr_in *resolved_addr = (struct sockaddr_in *)result->ai_addr;
                addr.sin_addr = resolved_addr->sin_addr;
                freeaddrinfo(result);
            }
        }
    }

    int ret = bind(sd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR) {
        return false;
    }

    return true;
}

bool mew_tcplistener_native_listen(void *data, uint32_t max_connections) {
    SOCKET sd = (SOCKET)(uintptr_t)data;
    int ret = listen(sd, (int)max_connections);
    if (ret == SOCKET_ERROR) {
        return false;
    }
    return true;
}

bool mew_tcplistener_native_accept(void *data, MewTcpStream *stream) {
    SOCKET sd = (SOCKET)(uintptr_t)data;
    SOCKET client_sd = accept(sd, NULL, NULL);
    if (client_sd == INVALID_SOCKET) {
        return false;
    }

    // Enable TCP_NODELAY for better performance
    char nodelay = 1;
    setsockopt(client_sd, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(nodelay));

    memset(stream, 0, sizeof(*stream));
    stream->data = (void *)(uintptr_t)client_sd;
    stream->set_timeout = &mew_tcpstream_native_set_timeout;
    stream->read = &mew_tcpstream_native_read;
    stream->write = &mew_tcpstream_native_write;
    stream->sendfile = &mew_tcpstream_native_sendfile;
    stream->close = &mew_tcpstream_native_close;
    return true;
}

bool mew_tcplistener_native_close(void *data) {
    SOCKET sd = (SOCKET)(uintptr_t)data;
    if (sd == INVALID_SOCKET) {
        return true;
    }
    return (closesocket(sd) == 0);
}

bool mew_tcpstream_native_set_timeout(void *data, uint32_t seconds) {
    SOCKET sd = (SOCKET)(uintptr_t)data;
    DWORD timeout = seconds * 1000;

    int ret = setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
    if (ret == SOCKET_ERROR) {
        return false;
    }

    ret = setsockopt(sd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));
    return (ret != SOCKET_ERROR);
}

ptrdiff_t mew_tcpstream_native_read(void *data, char *buf, uintptr_t size) {
    SOCKET sd = (SOCKET)(uintptr_t)data;
    int ret = recv(sd, buf, (int)size, 0);
    if (ret == SOCKET_ERROR) {
        int err = get_errno_from_winsock();
        if (err == WSAEWOULDBLOCK || err == WSAETIMEDOUT) {
            return 0;
        }
        return -1;
    }
    return ret;
}

ptrdiff_t mew_tcpstream_native_write(void *data, const char *buf, uintptr_t size) {
    SOCKET sd = (SOCKET)(uintptr_t)data;
    int ret = send(sd, buf, (int)size, 0);
    if (ret == SOCKET_ERROR) {
        int err = get_errno_from_winsock();
        if (err == WSAEWOULDBLOCK || err == WSAETIMEDOUT) {
            return 0;
        }
        return -1;
    }
    return ret;
}

bool mew_tcpstream_native_sendfile(void *data, const char *path, uintptr_t size) {
    SOCKET sd = (SOCKET)(uintptr_t)data;

    HANDLE hFile = CreateFileA(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    if (size == 0) {
        LARGE_INTEGER fileSize;
        if (!GetFileSizeEx(hFile, &fileSize)) {
            CloseHandle(hFile);
            return false;
        }
        size = (uintptr_t)fileSize.QuadPart;
    }

    BOOL success = TransmitFile(sd, hFile, (DWORD)size, 0, NULL, NULL, 0);

    CloseHandle(hFile);
    return (success == TRUE);
}

bool mew_tcpstream_native_close(void *data) {
    SOCKET sd = (SOCKET)(uintptr_t)data;
    return (closesocket(sd) == 0);
}
