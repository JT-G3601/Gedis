#ifndef C_THREAD_POOL_H
#define C_THREAD_POOL_H

#pragma once

#include "common.h"
#include <pthread.h>
#include <queue>
#include <vector>

namespace gedis {

struct Work {
    void (*f)(void *) = nullptr;
    void *arg = nullptr;
};

struct ThreadPool {
    std::vector<pthread_t> threads;
    std::deque<Work> queue;
    pthread_mutex_t  mu;
    pthread_cond_t not_empty;
};

static void *worker(void *arg) {
    auto *tp = (ThreadPool *)arg;
    while(true) {
        pthread_mutex_lock(&tp->mu);
        // wait for the condition: a non-empty queue
        while(tp->queue.empty()) {
            pthread_cond_wait(&tp->not_empty, &tp->mu);
        }

        // get the job
        Work const w = tp->queue.front();
        tp->queue.pop_front();
        pthread_mutex_unlock(&tp->mu);

        // do the work
        w.f(w.arg);
    }
    return nullptr;
}

inline void thread_pool_init(ThreadPool *tp, size_t num_threads) {
    assert(num_threads > 0);

    int rv = pthread_mutex_init(&tp->mu, nullptr);
    assert(rv == 0);
    rv = pthread_cond_init(&tp->not_empty, nullptr);
    assert(rv == 0);

    tp->threads.resize(num_threads);
    for(size_t i = 0; i < num_threads; i ++) {
        int rv = pthread_create(&tp->threads[i], nullptr, &worker, tp);
        assert(rv == 0);
    }
}

inline void thread_pool_queue(ThreadPool *tp, void (*f)(void *), void *arg) {
    Work w;
    w.f = f;
    w.arg = arg;

    pthread_mutex_lock(&tp->mu);
    tp->queue.push_back(w);
    pthread_cond_signal(&tp->not_empty);
    pthread_mutex_unlock(&tp->mu);
}


} // namespace gedis


#endif
