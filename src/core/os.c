#ifdef _WIN32
    #include "os/socket_windows.c"
    #include "os/fs_windows.c"
    #include "os/threads_windows.c"
#else
    #include "os/fs_posix.c"
    #include "os/socket_posix.c"
    #include "os/threads_posix.c"
#endif

#include "os/threads.c"
#include "os/utils.c"
