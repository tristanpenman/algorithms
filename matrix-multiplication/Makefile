CXX_FLAGS=-std=c++11

all: Sequential Multithreaded

Sequential: Sequential.cpp Matrix.h
	$(CXX) $(CXX_FLAGS) Sequential.cpp -o Sequential

clean:
	$(RM) *.o Sequential Multithreaded
