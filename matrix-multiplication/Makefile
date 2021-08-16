CXX_FLAGS=-std=c++17 -O2

all: Sequential Recursive1 Multithreaded1 Multithreaded2 QueueBased1

Sequential: Sequential.cpp Matrix.h
	$(CXX) $(CXX_FLAGS) Sequential.cpp -o Sequential

Recursive1: Recursive1.cpp Matrix.h Slice.h
	$(CXX) $(CXX_FLAGS) Recursive1.cpp -o Recursive1

Multithreaded1: Multithreaded1.cpp Matrix.h
	$(CXX) $(CXX_FLAGS) Multithreaded1.cpp -o Multithreaded1

Multithreaded2: Multithreaded2.cpp Matrix.h
	$(CXX) $(CXX_FLAGS) Multithreaded2.cpp -o Multithreaded2

QueueBased1: QueueBased1.cpp Matrix.h Queue.h
	$(CXX) $(CXX_FLAGS) QueueBased1.cpp -o QueueBased1

clean:
	$(RM) *.o Sequential Recursive1 Multithreaded1 Multithreaded2 QueueBased1
