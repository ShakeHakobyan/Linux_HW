#include <iostream>
#include <vector>
#include <pthread.h>
#include "ParallelMatrix.h"

typedef std::vector<std::vector<int>>  matrix_tp;

struct SumArg {
	matrix_tp matrix;
	int start_row;
	int row_count;
	int column_count;
};

void* SumThread(void* arg) {
	SumArg* args = (SumArg*) arg;
	int thread_sum = 0;
	for(int i = args->start_row; i < args->start_row + args->row_count; ++i) {
		for(int j = 0; j < args->column_count; ++j) {
			thread_sum += args->matrix[i][i];
		}
	}

	return (void*) (new int(thread_sum));
}


ParallelMatrix::ParallelMatrix(int n, int m) : row_count(n), column_count(m) {
	matrix.resize(n);
	for(int i = 0; i < n; ++i) {
		matrix[i].resize(m);
	}
}

void ParallelMatrix::Init() {
	for (int i = 0; i < row_count; ++i) {
		for (int j = 0; j < column_count; ++j) {
			matrix[i][j] = rand() % 9;
		}
	}
}

int ParallelMatrix::Sum() {
	int sum = 0;
	for (int i = 0; i < row_count; ++i) {
		for (int j = 0; j < column_count; ++j) {
			sum += matrix[i][j];
		}
	}
	return sum;
}


int ParallelMatrix::SumParallel(int k) {
	int sum = 0;
	pthread_t* thread = new pthread_t[k];
	SumArg* args = new SumArg[k];
	for(int i = 0; i < k; ++i) {
		args[i].matrix = matrix;
		args[i].start_row = i*row_count/k ;
		args[i].row_count = row_count/k;
		args[i].column_count = column_count;
		int result = pthread_create(&thread[i], NULL, SumThread, &args[i]);
		if(result) { exit(result); }		
	}
	void* thread_sum = 0;
	for(int i = 0; i < k; ++i) {
		int result = pthread_join(thread[i], &thread_sum);
                if(result) { exit(result); }
		int* thread_sum_int = (int*) thread_sum;
		sum += *thread_sum_int;
		delete  thread_sum_int;
	}
		
	delete [] thread;
	delete [] args;

	return sum;
}	

