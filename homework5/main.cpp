#include <iostream>
#include <queue>
#include <pthread.h>
#include "ParallelScheduler.h"

void* f1(void* arg) {
    std::cout << "This is f1 function";
    return (void*) 0;
}

void* f2(void* arg) {
    std::cout << "This is f2 function";
    return (void*) 0;
}

void* f3(void* arg) {
    std::cout << "This is f3 function";
    return (void*) 0;
}

void* f4(void* arg) {
    std::cout << "This is f4 function";
    return (void*) 0;
}

void* f5(void* arg) {
    std::cout << "This is f5 function";
    return (void*) 0;
}

void* f6(void* arg) {
    std::cout << "This is f6 function";
    return (void*) 0;
}

int main() {
    ParallelScheduler* scheduler = new ParallelScheduler(32);
    scheduler->run(f1, NULL);
    return 0;
}
