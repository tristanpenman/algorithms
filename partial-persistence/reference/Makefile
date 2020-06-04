CXX = clang++

all: partially_persistent_set_1 partially_persistent_set_2

partially_persistent_set_1: partially_persistent_set_1.hpp partially_persistent_set_1.cpp
	$(CXX) -O0 -g -std=c++17 partially_persistent_set_1.cpp -o partially_persistent_set_1

partially_persistent_set_2: partially_persistent_set_2.hpp partially_persistent_set_2.cpp
	$(CXX) -O0 -g -std=c++17 partially_persistent_set_2.cpp -o partially_persistent_set_2

clean:
	$(RM) partially_persistent_set_1 partially_persistent_set_2
