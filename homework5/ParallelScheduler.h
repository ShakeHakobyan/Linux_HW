#pragma once
#include <iostream>
#include <queue>
#include <pthread.h>

void* thread_func(void* args);

class ParallelScheduler {
private:
    std::queue<std::pair<void* (*) (void*), void*>> functions;
    pthread_t* threads;
    pthread_mutex_t* mutex;
    pthread_cond_t* cv;
    friend void* thread_func(void*);

public:
    ParallelScheduler(int n);

    void run(void* (*func) (void*), void* args);
};
