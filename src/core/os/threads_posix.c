#include <mew/core/os/threads.h>

#include <stdlib.h>
#include <errno.h>
#include <errno.h>

#include <pthread.h>

typedef int(mew_thread_func_t)(void *arg);

typedef struct MewThreadFuncWrapperContext {
    pthread_t handle;
    mew_thread_func_t *func;
    void *user_arg;
} MewThreadFuncWrapperContext;

MewThreadError mew_threads_error_from_pthread(int error);
void *thread_func_wrapper(void *arg);

MewThreadError mew_thread_create(MewThread *handle, mew_thread_func_t *func, void *arg) {
    if (handle == NULL || func == NULL) {
        return MEW_THREAD_ERROR_INVALID_ARGUMENT;
    }

    MewThreadFuncWrapperContext *ctx = malloc(sizeof(*ctx));
    if (ctx == NULL) {
        return MEW_THREAD_ERROR_OUT_OF_MEMORY;
    }

    ctx->func = func;
    ctx->user_arg = arg;
    int result = pthread_create(&ctx->handle, NULL, &thread_func_wrapper, &ctx);

    if (result == 0) {
        *handle = (MewThread)ctx->handle;
    } else {
        free(ctx);
    }

    return mew_threads_error_from_pthread(result);
}

MewThread mew_thread_current(void) {
    return (MewThread)pthread_self();
}

MewThreadError mew_thread_join(MewThread thread, int *return_status) {
    pthread_t pthread = (pthread_t)thread;
    void *value;
    int result = pthread_join(pthread, &value);

    if (result == 0) {
        if (return_status != NULL) {
            *return_status = (int)(int64_t)value;
        }
    }

    return mew_threads_error_from_pthread(result);
}

MewThreadError mew_thread_detach(MewThread thread) {
    pthread_t pthread = (pthread_t)thread;
    int result = pthread_detach(pthread);
    return mew_threads_error_from_pthread(result);
}

MewThreadError mew_mutex_init(MewMutex *mtx) {
    pthread_mutex_t *pthread_mutex = malloc(sizeof(*pthread_mutex));
    int result = pthread_mutex_init(pthread_mutex, NULL);

    if (result != 0) {
        free(pthread_mutex);
    } else {
        *mtx = pthread_mutex;
    }

    return mew_threads_error_from_pthread(result);
}

MewThreadError mew_mutex_destroy(MewMutex mtx) {
    int result = pthread_mutex_destroy(mtx);
    if (result == 0) {
        free(mtx);
    }
    return mew_threads_error_from_pthread(result);
}

MewThreadError mew_mutex_lock(MewMutex mtx) {
    int result = pthread_mutex_lock(mtx);
    return mew_threads_error_from_pthread(result);
}

MewThreadError mew_mutex_unlock(MewMutex mtx) {
    int result = pthread_mutex_unlock(mtx);
    return mew_threads_error_from_pthread(result);
}

MewThreadError mew_cond_init(MewCond *cond) {
    pthread_cond_t *pthread_cond = malloc(sizeof(*pthread_cond));
    int result = pthread_cond_init(pthread_cond, NULL);

    if (result == 0) {
        *cond = pthread_cond;
    } else {
        free(pthread_cond);
    }

    return mew_threads_error_from_pthread(result);
}

MewThreadError mew_cond_destroy(MewCond cond) {
    int result = pthread_cond_destroy(cond);
    if (result == 0) {
        free(cond);
    }
    return mew_threads_error_from_pthread(result);
}

MewThreadError mew_cond_wait(MewCond cond, MewMutex mtx) {
    int result = pthread_cond_wait(cond, mtx);
    return mew_threads_error_from_pthread(result);
}

MewThreadError mew_cond_notify(MewCond cond) {
    int result = pthread_cond_signal(cond);
    return mew_threads_error_from_pthread(result);
}

MewThreadError mew_cond_notify_all(MewCond cond) {
    int result = pthread_cond_broadcast(cond);
    return mew_threads_error_from_pthread(result);
}

MewThreadError mew_threads_error_from_pthread(int error) {
    switch (error) {
        case 0:
            return MEW_THREAD_SUCCESS;
        case EPERM:
            return MEW_THREAD_ERROR_PERMISSIONS;
        case ESRCH:
            return MEW_THREAD_ERROR_NOT_FOUND;
        case EAGAIN:
            return MEW_THREAD_ERROR_TRY_AGAIN;
        case ENOMEM:
            return MEW_THREAD_ERROR_OUT_OF_MEMORY;
        case EBUSY:
            return MEW_THREAD_ERROR_BUSY;
        case EINVAL:
            return MEW_THREAD_ERROR_INVALID_ARGUMENT;
        case EDEADLK:
            return MEW_THREAD_ERROR_DEADLOCK;
        default:
            return MEW_THREAD_ERROR_UNKNOWN;
    }
}

void *thread_func_wrapper(void *arg) {
    MewThreadFuncWrapperContext *ctx = arg;
    int result = ctx->func(ctx->user_arg);
    free(ctx);
    return (void *)(int64_t)result;
}
