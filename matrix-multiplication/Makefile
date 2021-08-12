CXX_FLAGS=-std=c++11

all: Sequential Multithreaded

Sequential: Sequential.cpp Matrix.h
	$(CXX) $(CXX_FLAGS) Sequential.cpp -o Sequential

Multithreaded: Multithreaded.cpp Matrix.h
	$(CXX) $(CXX_FLAGS) Multithreaded.cpp -o Multithreaded

clean:
	$(RM) *.o Sequential Multithreaded
