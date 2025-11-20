/**
 * @file mew/os/threads.h
 * @brief OS-independent threads implementation
 */

#ifndef INCLUDE_MEW_CORE_OS_THREADS_H_
#define INCLUDE_MEW_CORE_OS_THREADS_H_

#include <mew/core/types.h>

typedef enum MewThreadError {
    MEW_THREAD_SUCCESS = 0,
    MEW_THREAD_ERROR_PERMISSIONS = 1,
    MEW_THREAD_ERROR_NOT_FOUND = 3,
    MEW_THREAD_ERROR_TRY_AGAIN = 11,
    MEW_THREAD_ERROR_OUT_OF_MEMORY = 12,
    MEW_THREAD_ERROR_BUSY = 16,
    MEW_THREAD_ERROR_INVALID_ARGUMENT = 22,
    MEW_THREAD_ERROR_DEADLOCK = 35,
    MEW_THREAD_ERROR_UNKNOWN = 0xdeadbeef,
} MewThreadError;

const char *mew_thread_error_description(MewThreadError error);

/**
 * @struct MewThread
 * @brief thread primitive
 */
typedef void *MewThread;

/**
 * @brief signature of the function executed by a new thread
 */
typedef int(mew_thread_func_t)(void *arg);

/**
 * @brief Create and run a new thread
 *
 * Creates a new thread that begins execution immediately. The caller is responsible
 * for either joining with mew_thread_join() or cleaning with mew_thread_destroy();
 *
 * @param [out] handle - Pointer to thread handle that will be initialized upon success.
 *                       The handle must be cleaned up with mew_thread_destroy().
 * @param [in] func - Function pointer to the routine that executes in the new thread.
 * @param @optional [in] arg - User-defined argument passed to the thread function.
 *
 * @return MEW_THREAD_ERROR_SUCCESS on success, error code otherwise
 *
 * @error{MEW_THREAD_ERROR_TRY_AGAIN} - System lacked resources to create thread
 * @error{MEW_THREAD_PERMISSIONS} - Insufficient permissions for scheduling policy
 * @error{MEW_THREAD_ERROR_INVALID_ARGUMENT} - handle or func was NULL
 * @error{MEW_THREAD_ERROR_MEMORY} - Memory allocation failure
 */
MewThreadError mew_thread_create(MewThread *handle, mew_thread_func_t *func, void *arg);

/**
 * @brief Get current thread
 */
MewThread mew_thread_current(void);

/**
 * @brief Join the thread and store it's return status in @p return_status
 */
MewThreadError mew_thread_join(MewThread thread, int *return_status);

/**
 * @brief Detach thread from current process
 */
MewThreadError mew_thread_detach(MewThread thread);

/**
 * @brief Mutex primitive
 */
typedef void *MewMutex;

/**
 * @brief Initialize the mutex
 */
MewThreadError mew_mutex_init(MewMutex *mtx);

/**
 * @brief Destroy the mutex
 */
MewThreadError mew_mutex_destroy(MewMutex mtx);

/**
 * @brief Lock the mutex
 */
MewThreadError mew_mutex_lock(MewMutex mtx);

/**
 * @brief Unlock mutex
 */
MewThreadError mew_mutex_unlock(MewMutex mtx);

/**
 * @brief condition variable primitive
 */
typedef void *MewCond;

/**
 * @brief Initialize the condition variable
 */
MewThreadError mew_cond_init(MewCond *cond);

/**
 * @brief Destroy the condition variable
 */
MewThreadError mew_cond_destroy(MewCond cond);

/**
 * @brief Unlock the mutex and block on condition variable
 *
 * Atomically unlocks the mutex and blocks on the condition variable until the
 * thread is signalled by mew_cond_notify() or mew_cond_notify_all(), or until a
 * spurious wake-up occurs. The mutex is locked again before the function returns.
 *
 * The behavior is undefined if the mutex is not already locked by the calling
 * thread.
 */
MewThreadError mew_cond_wait(MewCond cond, MewMutex mtx);

/**
 * @brief Unblock one thread that currently waits on condition variable.
 *
 * Unblocks one thread that currently waits on condition variable pointed to by
 * cond. If no threads are blocked, does nothing and returns `true`
 */
MewThreadError mew_cond_notify(MewCond cond);

/**
 * @brief Unblock all threads that are blocked on condition variable.
 *
 * Unblocks all threads that are blocked on condition variable cond at the time
 * of the call. If no threads are blocked on cond, the function does nothing and
 * returns `true`.
 */
MewThreadError mew_cond_notify_all(MewCond cond);

#endif // INCLUDE_MEW_CORE_OS_THREADS_H_
