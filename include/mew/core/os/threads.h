/**
 * @file mew/os/threads.h
 * @brief OS-independent threads implementation
 */

#ifndef INCLUDE_MEW_CORE_OS_THREADS_H_
#define INCLUDE_MEW_CORE_OS_THREADS_H_

#include <mew/core/types.h>

/**
 * @struct MewThread
 * @brief thread primitive
 */
typedef struct MewThread {
    uint64_t data;
} MewThread;

typedef int(mew_thread_func_t)(void *arg);

/**
 * @brief Create and run an ew thread
 *
 * @param thread - buffer where ID of the create thread will be stored
 * @param func - function to run in the new thread
 * @param arg - argument to that function
 * @return `true` on success, `false` otherwise
 */
bool mew_thread_create(MewThread *thread, mew_thread_func_t *func, void *arg);

/**
 * @brief Get current thread
 *
 * @return current thread ID
 */
MewThread *mew_thread_current(void);

/**
 * @brief Join the thread and store it's return status in @p return_status
 *
 * @param thread - thread
 * @param @optional return_status buffer where thread return status will be stored
 * @return `true` on success, `false` otherwise
 */
bool mew_thread_join(MewThread *thread, int *return_status);

/**
 * @struct MewMutex
 * @brief Mutex primitive
 */
typedef struct MewMutex {
    uint64_t data;
} MewMutex;

/**
 * @brief Initialize the mutex
 *
 * @param mtx - mutex
 * @return `true` on success, `false` otherwise
 */
bool mew_mutex_init(MewMutex *mtx);

/**
 * @brief Destroy the mutex
 *
 * @param mtx - mutex
 * @return `true` on success, `false` otherwise
 */
bool mew_mutex_destroy(MewMutex *mtx);

/**
 * @brief Lock the mutex
 *
 * Try to lock the mutex. If already locked behavoir is undefined.
 *
 * @param mtx - mutex
 * @return `true` on success, `false` otherwise
 */
bool mew_mutex_lock(MewMutex *mtx);

/**
 * @brief Unlock mutex
 *
 * Try to unlock the mutex. If already unlocked behavior is undefined.
 * @param mtx - mutex
 * @return `true` on success, `false` otherwise
 */
bool mew_mutex_unlock(MewMutex *mtx);

/**
 * @struct MewCond
 * @brief condition variable primitive
 */
typedef struct MewCond {
    uint64_t data;
} MewCond;

/**
 * @brief Initialize the condition variable
 *
 * @param cond - condition variable
 * @return `true` on success, `false` otherwise
 */
bool mew_cond_init(MewCond *cond);

/**
 * @brief Destroy the condition variable
 *
 * @param cond - condition variable
 * @return `true` on success, `false` otherwise
 */
bool mew_cond_destroy(MewCond *cond);

/**
 * @brief Unlock the mutex and block on condition variable
 *
 * Atomically unlocks the mutex and blocks on the condition variable until the
 * thread is signalled by mew_cond_notify() or mew_cond_notify_all(), or until a
 * spurious wake-up occurs. The mutex is locked again before the function returns.
 *
 * The behavior is undefined if the mutex is not already locked by the calling
 * thread.
 *
 * @param cond - condition variable
 * @param mtx - mutex
 * @return `true` on success, `false` otherwise
 */
bool mew_cond_wait(MewCond *cond, MewMutex *mtx);

/**
 * @brief Unblock one thread that currently waits on condition variable.
 *
 * Unblocks one thread that currently waits on condition variable pointed to by
 * cond. If no threads are blocked, does nothing and returns `true`
 *
 * @param cond - condition variable
 * @return `true` on success, `false` otherwise
 */
bool mew_cond_notify(MewCond *cond);

/**
 * @brief Unblock all threads that are blocked on condition variable.
 *
 * Unblocks all threads that are blocked on condition variable cond at the time
 * of the call. If no threads are blocked on cond, the function does nothing and
 * returns `true`.
 *
 * @param cond - condition variable
 * @return `true` on success, `false` otherwise
 */
bool mew_cond_notify_all(MewCond *cond);

#endif // INCLUDE_MEW_CORE_OS_THREADS_H_
