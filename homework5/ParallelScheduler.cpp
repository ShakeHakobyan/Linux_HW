#include "ParallelScheduler.h"
#include <iostream>
#include <queue>
#include <pthread.h>

void* thread_func(void* args) {
        ParallelScheduler* scheduler = (ParallelScheduler*)args;
        while (true) {
            pthread_mutex_lock(scheduler->mutex);
            while (scheduler->functions.empty()) {
                pthread_cond_wait(scheduler->cv, scheduler->mutex);
            }
            auto function = scheduler->functions.back().first;
            auto args = scheduler->functions.back().second;
            scheduler->functions.pop();
            pthread_mutex_unlock(scheduler->mutex);
            function(args);

        }
        return (void*) 0;
}


ParallelScheduler::ParallelScheduler(int n) {
        mutex = new pthread_mutex_t;
        threads = new pthread_t[n];
	for(int i = 0; i < n; ++i) {
	int result = pthread_create(&threads[i], NULL, thread_func, (void*)this);
	if(result != 0) { exit(result); }
  	}
}

void ParallelScheduler::run(void* (*func) (void*), void* args) {
        functions.push(std::make_pair(func, args));
        pthread_cond_signal(cv);
}



