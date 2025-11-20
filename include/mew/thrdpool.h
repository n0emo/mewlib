#ifndef MEW_INCLUDE_MEW_THRDPOOL_H_
#define MEW_INCLUDE_MEW_THRDPOOL_H_

#include <stdbool.h>

#include <mew/core/types.h>
#include <mew/core/os/threads.h>

typedef int(JobExecutor)(void *arg);

typedef struct ThreadPoolJob {
    JobExecutor *executor;
    void *arg;
    struct ThreadPoolJob *next;
} ThreadPoolJob;

typedef struct ThreadPoolQueue {
    ThreadPoolJob *first;
    ThreadPoolJob *last;
    usize count_;
    MewMutex mtx;
    MewCond not_empty;
    bool about_to_destroy_;
} ThreadPoolQueue;

MewThreadError queue_init(ThreadPoolQueue *queue);
MewThreadError queue_destroy(ThreadPoolQueue *queue);
MewThreadError queue_push(ThreadPoolQueue *queue, ThreadPoolJob job);
MewThreadError queue_pop(ThreadPoolQueue *queue, ThreadPoolJob *result);

typedef struct ThreadPool {
    ThreadPoolQueue queue;
    MewThread *threads;
    usize thread_count_;
    usize threads_alive_;
    bool cancel_;
    MewMutex mtx;
} ThreadPool;

MewThreadError thrdpool_init(ThreadPool *pool, size_t thread_count);
MewThreadError thrdpool_destroy(ThreadPool *pool);
MewThreadError thrdpool_add_job(ThreadPool *pool, JobExecutor *executor, void *arg);

#endif // MEW_INCLUDE_MEW_THRDPOOL_H_
