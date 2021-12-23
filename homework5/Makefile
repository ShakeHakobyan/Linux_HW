all: main

main: main.o ParallelScheduler.o
	g++ main.o ParallelScheduler.o -pthread -o main

main.o: main.cpp ParallelScheduler.h
	g++ -c main.cpp

ParallelMatrix.o: ParallelScheduler.cpp ParallelScheduler.h
	 g++ -c ParallelScheduler.cpp

clean:
