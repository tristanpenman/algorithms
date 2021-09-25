CXX_FLAGS=-std=c++17 -O2
MPI_CXX=mpic++
MPI_LD_FLAGS=-lmpi


#
# Basic and multithreaded examples will be built by default. Advanced examples will not,
# since they have non-standard dependencies.
#

BASIC_EXAMPLES=Sequential Recursive1 Recursive2
MULTITHREADED_EXAMPLES=Multithreaded1 Multithreaded2 QueueBased
ADVANCED_EXAMPLES=MPI

all: $(BASIC_EXAMPLES) $(MULTITHREADED_EXAMPLES)

basic: $(BASIC_EXAMPLES)

multithreaded: $(MULTITHREADED_EXAMPLES)

advanced: $(ADVANCED_EXAMPLES)

clean:
	$(RM) $(BASIC_EXAMPLES) $(MULTITHREADED_EXAMPLES) $(ADVANCED_EXAMPLES) *.o

#
# Basic Examples
#

Sequential: Sequential.cpp Matrix.h
	$(CXX) $(CXX_FLAGS) Sequential.cpp -o Sequential

Recursive1: Recursive1.cpp Matrix.h Slice.h
	$(CXX) $(CXX_FLAGS) Recursive1.cpp -o Recursive1

Recursive2: Recursive2.cpp Matrix.h Slice.h
	$(CXX) $(CXX_FLAGS) Recursive2.cpp -o Recursive2

#
# Multithreaded Examples
#

Multithreaded1: Multithreaded1.cpp Matrix.h
	$(CXX) $(CXX_FLAGS) Multithreaded1.cpp -o Multithreaded1 -pthread

Multithreaded2: Multithreaded2.cpp Matrix.h
	$(CXX) $(CXX_FLAGS) Multithreaded2.cpp -o Multithreaded2 -pthread

QueueBased: QueueBased.cpp Matrix.h Queue.h
	$(CXX) $(CXX_FLAGS) QueueBased.cpp -o QueueBased -pthread

#
# Advanced Examples
#

MPI: MPI.cpp Matrix.h
	$(MPI_CXX) $(CXX_FLAGS) -o MPI MPI.cpp $(MPI_LD_FLAGS)
