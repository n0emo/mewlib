#include <mew/core/utils.h>

#include <assert.h>
#include <signal.h>
#include <stdarg.h>

#ifdef _WIN32
    #define WINDOWS_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// TODO: port to Windows
#include <signal.h>

#include <mew/log.h>

volatile sig_atomic_t interrupted = 0;

static void handle_sigint(int sig) {
    (void)sig;
    if (sig == SIGABRT) {
        interrupted = 1;
    }
}

void wait_for_ctrl_c(void) {
    signal(SIGINT, handle_sigint);

    while (!interrupted) {
#ifdef _WIN32
        Sleep(INFINITE);
#else
    #include <unistd.h>
#endif
    }
}
