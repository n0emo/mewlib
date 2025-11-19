#include <mew/core/os/threads.h>

#include <stdio.h>
#include <stdlib.h>

#define MEW_FUNC_NOT_IMPLEMENTED()                                                                                     \
    do {                                                                                                               \
        fprintf(stderr, "Not yet implemented: %s", __FUNCTION__);                                                      \
        abort();                                                                                                       \
    } while (0)

MewThreadError mew_thread_create(MewThread *handle, mew_thread_func_t *func, void *arg) {
    (void)handle;
    (void)func;
    (void)arg;
    MEW_FUNC_NOT_IMPLEMENTED();
}

MewThread mew_thread_current(void) {
    MEW_FUNC_NOT_IMPLEMENTED();
}

MewThreadError mew_thread_join(MewThread thread, int *return_status) {
    (void)thread;
    (void)return_status;
    MEW_FUNC_NOT_IMPLEMENTED();
}

MewThreadError mew_mutex_init(MewMutex *mtx) {
    (void)mtx;
    MEW_FUNC_NOT_IMPLEMENTED();
}

MewThreadError mew_mutex_destroy(MewMutex mtx) {
    (void)mtx;
    MEW_FUNC_NOT_IMPLEMENTED();
}

MewThreadError mew_mutex_lock(MewMutex mtx) {
    (void)mtx;
    MEW_FUNC_NOT_IMPLEMENTED();
}

MewThreadError mew_mutex_unlock(MewMutex mtx) {
    (void)mtx;
    MEW_FUNC_NOT_IMPLEMENTED();
}

MewThreadError mew_cond_init(MewCond *cond) {
    (void)cond;
    MEW_FUNC_NOT_IMPLEMENTED();
}

MewThreadError mew_cond_destroy(MewCond cond) {
    (void)cond;
    MEW_FUNC_NOT_IMPLEMENTED();
}

MewThreadError mew_cond_wait(MewCond cond, MewMutex mtx) {
    (void)cond;
    (void)mtx;
    MEW_FUNC_NOT_IMPLEMENTED();
}

MewThreadError mew_cond_notify(MewCond cond) {
    (void)cond;
    MEW_FUNC_NOT_IMPLEMENTED();
}

MewThreadError mew_cond_notify_all(MewCond cond) {
    (void)cond;
    MEW_FUNC_NOT_IMPLEMENTED();
}
