#include <mew/thrdpool.h>

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include <mew/log.h>

MewThreadError queue_init(ThreadPoolQueue *queue) {
    MewThreadError error;

    memset(queue, 0, sizeof(*queue));

    error = mew_mutex_init(&queue->mtx);
    if (error != MEW_THREAD_SUCCESS) {
        goto error;
    }

    error = mew_cond_init(&queue->not_empty);
    if (error != MEW_THREAD_SUCCESS) {
        goto error;
    }

    return MEW_THREAD_SUCCESS;

error:
    if (queue->mtx != NULL) {
        mew_mutex_destroy(queue->mtx);
    }
    if (queue->not_empty != NULL) {
        mew_cond_destroy(queue->not_empty);
    }

    return error;
}

MewThreadError queue_destroy(ThreadPoolQueue *queue) {
    MewThreadError error = mew_mutex_destroy(&queue->mtx);
    if (error != MEW_THREAD_SUCCESS) {
        return error;
    }

    // If mew_mutex_destroy succeeded, this cond destroying should also succeed
    mew_cond_destroy(&queue->not_empty);

    ThreadPoolJob *node = queue->first;
    ThreadPoolJob *next = NULL;
    while (node) {
        next = node->next;
        free(node);
        node = next;
    }

    return MEW_THREAD_SUCCESS;
}

MewThreadError queue_push(ThreadPoolQueue *queue, ThreadPoolJob job) {
    MewThreadError error = mew_mutex_lock(queue->mtx);
    if (error != MEW_THREAD_SUCCESS) {
        return error;
    }

    ThreadPoolJob *node = malloc(sizeof(*node));
    memcpy(node, &job, sizeof(*node));

    if (queue->count_ == 0) {
        queue->first = node;
        queue->last = node;
        queue->count_ += 1;
    } else {
        queue->last->next = node;
        queue->last = node;
        queue->count_ += 1;
    }

    mew_mutex_unlock(queue->mtx);
    mew_cond_notify(queue->not_empty);

    return MEW_THREAD_SUCCESS;
}

MewThreadError queue_pop(ThreadPoolQueue *queue, ThreadPoolJob *result) {
    MewThreadError error = mew_mutex_lock(queue->mtx);
    if (error != MEW_THREAD_SUCCESS) {
        return error;
    }

    while (queue->count_ == 0) {
        if (queue->about_to_destroy_) {
            mew_mutex_unlock(queue->mtx);
            return MEW_THREAD_ERROR_INVALID_ARGUMENT;
        }

        mew_cond_wait(queue->not_empty, queue->mtx);
    }

    assert(queue->count_ > 0);
    queue->count_ -= 1;
    ThreadPoolJob *first = queue->first;
    queue->first = queue->first->next;
    memcpy(result, first, sizeof(*result));
    free(first);

    mew_mutex_unlock(queue->mtx);

    return MEW_THREAD_SUCCESS;
}

static int thread_func(void *arg) {
    MewThreadError error;
    ThreadPoolJob job;

    ThreadPool *pool = (ThreadPool *)arg;
    MewThread current_thrd = mew_thread_current();

    error = mew_mutex_lock(pool->mtx);
    if (error != MEW_THREAD_SUCCESS) {
        log_error("Error spawning thread in pool: %s", mew_thread_error_description(error));
        return (int)error;
    }
    pool->threads_alive_ += 1;
    mew_mutex_unlock(pool->mtx);

repeat:

    error = queue_pop(&pool->queue, &job);
    if (error != MEW_THREAD_SUCCESS) {
        goto end;
    }

    error = mew_mutex_lock(pool->mtx);
    assert(error == MEW_THREAD_SUCCESS);

    if (pool->cancel_) {
        error = mew_mutex_unlock(pool->mtx);
        assert(error == MEW_THREAD_SUCCESS);

        goto end;
    }

    error = mew_mutex_unlock(pool->mtx);
    assert(error == MEW_THREAD_SUCCESS);

    int res = job.executor(job.arg);

    if (res != 0) {
        log_error("Job returned status %d (thread %" PRIu64 ")", res, (uint64_t)current_thrd);
    }

    error = mew_mutex_lock(pool->mtx);
    assert(error == MEW_THREAD_SUCCESS);

    if (!pool->cancel_) {
        error = mew_mutex_unlock(pool->mtx);
        assert(error == MEW_THREAD_SUCCESS);

        goto repeat;
    }

    error = mew_mutex_unlock(pool->mtx);
    assert(error == MEW_THREAD_SUCCESS);

end:

    error = mew_mutex_lock(pool->mtx);
    assert(error == MEW_THREAD_SUCCESS);

    pool->threads_alive_ -= 1;

    error = mew_mutex_unlock(pool->mtx);
    assert(error == MEW_THREAD_SUCCESS);

    if (error != MEW_THREAD_SUCCESS) {
        log_error("Error executing thread in pool: %s", mew_thread_error_description(error));
    }

    return (int)error;
}

MewThreadError thrdpool_init(ThreadPool *pool, size_t thread_count) {
    MewThreadError error;
    usize i = 0;

    memset(pool, 0, sizeof(*pool));

    error = queue_init(&pool->queue);
    if (error != MEW_THREAD_SUCCESS) {
        goto error;
    }

    pool->thread_count_ = thread_count;
    pool->threads = malloc(sizeof(pool->threads[0]) * thread_count);

    for (i = 0; i < thread_count; i++) {
        error = mew_thread_create(&pool->threads[i], thread_func, pool);
        if (error != MEW_THREAD_SUCCESS) {
            goto error;
        }
    }

error:
    if (pool->threads != NULL) {
        free(pool->threads);
    }

    for (usize j = 0; j < i; j++) {
        mew_thread_detach(pool->threads[j]);
    }

    return error;
}

MewThreadError thrdpool_destroy(ThreadPool *pool) {
    MewThreadError error;

    error = mew_mutex_lock(pool->mtx);
    if (error != MEW_THREAD_SUCCESS) {
        return error;
    }

    pool->queue.about_to_destroy_ = true;
    pool->cancel_ = true;
    mew_mutex_unlock(pool->mtx);

    while (pool->threads_alive_ > 0) {
        mew_cond_notify_all(pool->queue.not_empty);
    }
    for (size_t i = 0; i < pool->thread_count_; i++) {
        int res;
        mew_thread_join(pool->threads[i], &res);
    }
    error = queue_destroy(&pool->queue);
    free(pool->threads);
    return error;
}

MewThreadError thrdpool_add_job(ThreadPool *pool, JobExecutor *executor, void *arg) {
    ThreadPoolJob job = {executor, arg, NULL};
    return queue_push(&pool->queue, job);
}
