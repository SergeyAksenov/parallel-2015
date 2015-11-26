all: life

life: main.o worker.o command.o thread.o matrix.o master.o
	g++ main.o worker.o command.o thread.o matrix.o master.o -o life

main.o: main.cpp
	g++ -c main.cpp -lpthread -std=c++0x -o main.o

worker.o: WorkerJob.hpp
	g++ -c WorkerJob.hpp -std=c++11 -lpthread -o worker.o

command.o: Command.h
	g++ -c Command.h -std=c++11 -lpthread -o command.o

thread.o: thread.cpp
	g++ -c thread.cpp -std=c++11 -lpthread -o thread.o

callable.o: 
	g++ -c ICallable.hpp -o callable.o

matrix.o: Matrix.hpp
	g++ -c Matrix.hpp -o matrix.o

master.o: MasterJob.h 
	g++ -c -std=c++11 MasterJob.h -lpthread -o master.o