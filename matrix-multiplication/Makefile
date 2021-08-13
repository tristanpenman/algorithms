CXX_FLAGS=-std=c++17

all: Sequential Multithreaded1 Multithreaded2 Multithreaded3

Sequential: Sequential.cpp Matrix.h
	$(CXX) $(CXX_FLAGS) Sequential.cpp -o Sequential

Multithreaded1: Multithreaded1.cpp Matrix.h
	$(CXX) $(CXX_FLAGS) Multithreaded1.cpp -o Multithreaded1

Multithreaded2: Multithreaded2.cpp Matrix.h
	$(CXX) $(CXX_FLAGS) Multithreaded2.cpp -o Multithreaded2

Multithreaded3: Multithreaded3.cpp Matrix.h Queue.h
	$(CXX) $(CXX_FLAGS) Multithreaded3.cpp -o Multithreaded3

clean:
	$(RM) *.o Sequential Multithreaded1 Multithreaded2 Multithreaded3
