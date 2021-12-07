all: main

main: main.o ParallelMatrix.o
	g++ main.o ParallelMatrix.o -pthread -o main

main.o: main.cpp ParallelMatrix.h
	g++ -c main.cpp

ParallelMatrix.o: ParallelMatrix.cpp ParallelMatrix.h
	 g++ -c ParallelMatrix.cpp

clean:
	rm *.o main
