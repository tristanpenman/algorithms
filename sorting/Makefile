CXX_FLAGS = -g -std=c++17

.PHONY: clean

all: Quicksort ClosestPair

ClosestPair: ClosestPair.o
	$(CXX) -o ClosestPair ClosestPair.o

ClosestPair.o: ClosestPair.cpp ClosestPair.h Quicksort.h Util.h
	$(CXX) $(CXX_FLAGS) -o ClosestPair.o -c ClosestPair.cpp

Quicksort: Quicksort.o
	$(CXX) -o Quicksort Quicksort.o

Quicksort.o: Quicksort.cpp Quicksort.h Util.h
	$(CXX) $(CXX_FLAGS) -o Quicksort.o -c Quicksort.cpp

clean:
	rm *.o
