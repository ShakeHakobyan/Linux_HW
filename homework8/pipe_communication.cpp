#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <cerrno>
#include <cstdlib> 
#include <ctime> 
#include <sys/wait.h>


int main()
{
	unsigned long long N, M;
	std::cin >> N >> M;
	
	// creating an array of size N and filling it with random integer numbers
	int* arr = new int[N];

	srand((unsigned)time(0));
	for (size_t i = 0; i < N; i++)
	{
		arr[i] = rand() % 1000;
	}

	// creating two arrays of pipes 
	// 1 parent -> child
	// 2 child -> parent

	int** pipefds1 = new int*[M];
	for (size_t i = 0; i < M; i++)
	{
		pipefds1[i] = new int[2];
	}

	int** pipefds2 = new int* [M];
	for (size_t i = 0; i < M; i++)
	{
		pipefds2[i] = new int[2];
	}

	for (size_t i = 0; i < M; i++)
	{
		int res = pipe(pipefds1[i]);
		if (res < 0) {
			std::cout << "Unable to create pipe for parent process";
			exit(errno);
		}
	}


	// creating child processes
	for (size_t i = 0; i < M; i++)
	{
		int res = pipe(pipefds2[i]);
		if (res < 0) {
			std::cout << "Unable to create pipe for child processes";
			exit(errno);
		}

		unsigned long long start_idx = i * N / M;
		unsigned long long end_idx = (i + 1) * N / M;
		
		int pid = fork();


		if (pid == 0) // child process
		{
			// closing unwanted sides of pipes
			close(pipefds1[i][1]);
			close(pipefds2[i][0]);

			// reading subarray indexes
			unsigned long long start_idx;
			unsigned long long end_idx;
			if(read(pipefds1[i][0], &start_idx, sizeof(unsigned long long)) < 0) {
				std::cout << "Unable to read start index from pipe in child process";
				exit(errno);
			}
			if(read(pipefds1[i][0], &end_idx, sizeof(unsigned long long)) < 0) {
				std::cout << "Unable to read end index from pipe in child process";
				exit(errno);
			}

			//calculating sums
			unsigned long long sub_sum = 0;
			for (size_t i = start_idx; i < end_idx; i++)
			{
				sub_sum += arr[i];
			}
			std::cout << "Sub_sum of child" << i+1 << ": " << sub_sum << std::endl;

			
			if(write(pipefds2[i][1], &sub_sum, sizeof(unsigned long long)) < 0) {
				std::cout << "Unable to write sub_sum to pipe in child process";
				exit(errno);
			}

			exit(0);
		}

		if (pid != 0) // parent process
		{
			// closing unwanted sides of pipes
			close(pipefds1[i][0]);
			close(pipefds2[i][1]);

			// writing subarray indexes
			if(write(pipefds1[i][1], &start_idx, sizeof(unsigned long long)) < 0) {
				std::cout << "Unable to write start index to pipe in parent process";
				exit(errno);
			}

			if(write(pipefds1[i][1], &end_idx, sizeof(unsigned long long)) < 0) {
				std::cout << "Unable to write end index to pipe in parent process";
				exit(errno);
			}	
		}

	}

	int status;
	waitpid(0, &status, 0);
	unsigned long long sum = 0, sub_sum;

	for (size_t i = 0; i < M; i++)
	{

		// reading sub_sum of childes
		if(read(pipefds2[i][0], &sub_sum, sizeof(unsigned long long)) < 0) {
				std::cout << "Unable to read sub_sum from pipe in parent process";
				exit(errno);
		}
		sum += sub_sum;

	}
	std::cout << "Total sum: " << sum <<std::endl;


}
