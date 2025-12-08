#include <mew/core/os/utils.h>

#include <signal.h>

#ifdef _WIN32
    #define WINDOWS_LEAN_AND_MEAN
    #include <windows.h>
#else
    #include <unistd.h>
#endif

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
    pause();
#endif
    }
}
