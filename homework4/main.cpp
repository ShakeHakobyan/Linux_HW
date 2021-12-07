#include <iostream>
#include <vector>
#include <pthread.h>
#include <chrono>
#include "ParallelMatrix.h"


int main() {
	
	ParallelMatrix mtx(1000, 1000);
	mtx.Init();
	std::cout << "Function time" << std::endl;

	auto start = std::chrono::system_clock::now();
        mtx.Sum();
        auto end = std::chrono::system_clock::now();    
        std::cout << "Sum(): " << (end - start).count() << std::endl;

	start = std::chrono::system_clock::now();
	mtx.SumParallel(4);
	end = std::chrono::system_clock::now();	
	std::cout << "SumParallel(4): " << (end - start).count() << std::endl;

	start = std::chrono::system_clock::now();
        mtx.SumParallel(8);
        end = std::chrono::system_clock::now();    
        std::cout << "SumParallel(8): " << (end - start).count() << std::endl;

	start = std::chrono::system_clock::now();
        mtx.SumParallel(16);
        end = std::chrono::system_clock::now();    
        std::cout << "SumParallel(16): " << (end - start).count() << std::endl;

	start = std::chrono::system_clock::now();
        mtx.SumParallel(100);
        end = std::chrono::system_clock::now();    
        std::cout << "SumParallel(100): " << (end - start).count() << std::endl;
	return 0;
}

