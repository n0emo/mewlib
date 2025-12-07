#include <mew/core/os/threads.h>

#include <stdio.h>
#include <stdlib.h>

#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>

#define MEW_FUNC_NOT_IMPLEMENTED()                                                                                     \
    do {                                                                                                               \
        fprintf(stderr, "Not yet implemented: %s", __FUNCTION__);                                                      \
        abort();                                                                                                       \
    } while (0)

typedef struct MewThreadContext {
    mew_thread_func_t *func;
    void *arg;
} MewThreadContext;

MewThreadError mew_threads_error_from_windows();

unsigned long mew_thread_func(void *arg) {
    MewThreadContext *ctx = (MewThreadContext *)arg;
    unsigned long result = (unsigned long)ctx->func(ctx->arg);
    free(ctx);
    return result;
}

MewThreadError mew_thread_create(MewThread *handle, mew_thread_func_t *func, void *arg) {
    if (handle == NULL || func == NULL) {
        return MEW_THREAD_ERROR_INVALID_ARGUMENT;
    }

    MewThreadContext *ctx = malloc(sizeof(*ctx));
    ctx->func = func;
    ctx->arg = arg;

    HANDLE result = CreateThread(NULL, 0, mew_thread_func, ctx, 0, NULL);
    if (result != NULL) {
        *handle = result;
        return MEW_THREAD_SUCCESS;
    } else {
        free(ctx);
        return mew_threads_error_from_windows();
    }
}

MewThread mew_thread_current(void) {
    return (MewThread)GetCurrentThread();
}

MewThreadError mew_thread_join(MewThread thread, int *return_status) {
    if (thread == NULL) {
        return MEW_THREAD_ERROR_INVALID_ARGUMENT;
    }

    HANDLE handle = (HANDLE)thread;
    DWORD result = WaitForSingleObject(handle, INFINITE);
    if (result == WAIT_FAILED) {
        return mew_threads_error_from_windows();
    }

    if (return_status == NULL) {
        return MEW_THREAD_SUCCESS;
    }

    DWORD status;
    if (GetExitCodeThread(handle, &status)) {
        *return_status = (int)status;
        return MEW_THREAD_SUCCESS;
    } else {
        return mew_threads_error_from_windows();
    }
}

MewThreadError mew_thread_detach(MewThread thread) {
    if (thread == NULL) {
        return MEW_THREAD_ERROR_INVALID_ARGUMENT;
    }

    if (CloseHandle(thread)) {
        return MEW_THREAD_SUCCESS;
    } else {
        return mew_threads_error_from_windows();
    }
}

MewThreadError mew_mutex_init(MewMutex *mtx) {
    if (mtx == NULL) {
        return MEW_THREAD_ERROR_INVALID_ARGUMENT;
    }

    CRITICAL_SECTION *section = malloc(sizeof(*section));
    if (section == NULL) {
        return MEW_THREAD_ERROR_OUT_OF_MEMORY;
    }

    InitializeCriticalSection(section);
    *mtx = (MewMutex)section;
    return MEW_THREAD_SUCCESS;
}

MewThreadError mew_mutex_destroy(MewMutex mtx) {
    if (mtx == NULL) {
        return MEW_THREAD_ERROR_INVALID_ARGUMENT;
    }

    CRITICAL_SECTION *section = (CRITICAL_SECTION *)mtx;
    DeleteCriticalSection(section);
    free(section);
    return MEW_THREAD_SUCCESS;
}

MewThreadError mew_mutex_lock(MewMutex mtx) {
    if (mtx == NULL) {
        return MEW_THREAD_ERROR_INVALID_ARGUMENT;
    }

    CRITICAL_SECTION *section = (CRITICAL_SECTION *)mtx;
    EnterCriticalSection(section);
    return MEW_THREAD_SUCCESS;
}

MewThreadError mew_mutex_unlock(MewMutex mtx) {
    if (mtx == NULL) {
        return MEW_THREAD_ERROR_INVALID_ARGUMENT;
    }

    CRITICAL_SECTION *section = (CRITICAL_SECTION *)mtx;
    LeaveCriticalSection(section);
    return MEW_THREAD_SUCCESS;
}

MewThreadError mew_cond_init(MewCond *cond) {
    if (cond == NULL) {
        return MEW_THREAD_ERROR_INVALID_ARGUMENT;
    }

    CONDITION_VARIABLE *condition = malloc(sizeof(*condition));
    if (condition == NULL) {
        return MEW_THREAD_ERROR_OUT_OF_MEMORY;
    }

    InitializeConditionVariable(condition);
    *cond = (MewCond)condition;
    return MEW_THREAD_SUCCESS;
}

MewThreadError mew_cond_destroy(MewCond cond) {
    if (cond == NULL) {
        return MEW_THREAD_ERROR_INVALID_ARGUMENT;
    }

    // Windows condition variables don't need explicit destruction
    CONDITION_VARIABLE *condition = (CONDITION_VARIABLE *)cond;
    free(condition);
    return MEW_THREAD_SUCCESS;
}

MewThreadError mew_cond_wait(MewCond cond, MewMutex mtx) {
    if (cond == NULL || mtx == NULL) {
        return MEW_THREAD_ERROR_INVALID_ARGUMENT;
    }

    CONDITION_VARIABLE *condition = (CONDITION_VARIABLE *)cond;
    CRITICAL_SECTION *section = (CRITICAL_SECTION *)mtx;

    if (!SleepConditionVariableCS(condition, section, INFINITE)) {
        return mew_threads_error_from_windows();
    } else {
        return MEW_THREAD_SUCCESS;
    }
}

MewThreadError mew_cond_notify(MewCond cond) {
    if (cond == NULL) {
        return MEW_THREAD_ERROR_INVALID_ARGUMENT;
    }

    CONDITION_VARIABLE *condition = (CONDITION_VARIABLE *)cond;
    WakeConditionVariable(condition);
    return MEW_THREAD_SUCCESS;
}

MewThreadError mew_cond_notify_all(MewCond cond) {
    if (cond == NULL) {
        return MEW_THREAD_ERROR_INVALID_ARGUMENT;
    }

    CONDITION_VARIABLE *condition = (CONDITION_VARIABLE *)cond;
    WakeAllConditionVariable(condition);
    return MEW_THREAD_SUCCESS;
}

MewThreadError mew_threads_error_from_windows() {
    DWORD error = GetLastError();
    switch (error) {
        case ERROR_SUCCESS:
            return MEW_THREAD_SUCCESS;
        case ERROR_ACCESS_DENIED:
            return MEW_THREAD_ERROR_PERMISSIONS;
        case ERROR_INVALID_HANDLE:
            return MEW_THREAD_ERROR_NOT_FOUND;
        case ERROR_NO_SYSTEM_RESOURCES:
        case ERROR_OUTOFMEMORY:
            return MEW_THREAD_ERROR_OUT_OF_MEMORY;
        case ERROR_BUSY:
            return MEW_THREAD_ERROR_BUSY;
        case ERROR_INVALID_PARAMETER:
            return MEW_THREAD_ERROR_INVALID_ARGUMENT;
        case ERROR_POSSIBLE_DEADLOCK:
            return MEW_THREAD_ERROR_DEADLOCK;
        case ERROR_NOT_ENOUGH_MEMORY:
            return MEW_THREAD_ERROR_TRY_AGAIN;
        default:
            return MEW_THREAD_ERROR_UNKNOWN;
    }
}
