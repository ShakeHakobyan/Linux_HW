all: pipe_communication

pipe_communication: pipe_communication.o
	g++ pipe_communication.o -o pipe_communication

pipe_communication.o: pipe_communication.cpp
	g++ -c pipe_communication.cpp

clean:
	rm *.o pipe_communication