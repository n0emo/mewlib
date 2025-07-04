#include "containers/hashmap.c"
#include "containers/rbtree.c"
#include "containers/vector.c"
#include "core/alloc.c"
#include "core/allocators/arena.c"
#include "core/allocators/malloc.c"
#include "core/strings/convert.c"
#include "core/strings/cstr.c"
#include "core/strings/sb.c"
#include "core/strings/sv.c"
#include "core/utils.c"
#include "html.c"
#include "http/common.c"
#include "http/fs.c"
#include "http/headermap.c"
#include "http/request.c"
#include "http/response.c"
#include "http/router.c"
#include "http/server.c"
#include "ini.c"
#include "log.c"
#include "thrdpool.c"

#ifdef _WIN32
    #include "core/os/fs_windows.c"
    #include "core/os/socket_windows.c"
#else
    #include "core/os/fs_posix.c"
    #include "core/os/socket_posix.c"
#endif
