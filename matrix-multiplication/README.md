# Matrix Multiplication

A comparison of different approaches to matrix multiplication.

### Compiling

All examples can be compiled using the included Makefile:

    make

When run without any command line arguments, all examples will print usage information:

    $ ./Sequential1

    Usage:
      ./Sequential1 <M1> <N1/M2> <N2> [seed]

    Multiplies a random M1xN1 matrix by a random M2xN2 matrix

Arguments between `<>` are required, while those between `[]` are optional.

## Examples

### Sequential case 1 - Naive implementation

This is the simplest case, using a basic O(n^3) matrix multiplication algorithm.

### Multi-threaded case 1 - one cell per thread

This example builds on the Sequential case, by creating one thread per output cell. This demonstrates that over-use of threads can negate the benefit of parallel computation. i.e. if the work is too fine-grained, then the overhead of creating threads can be greater than the work itself.

### Multi-threaded case 2 - n rows per thread

We can improve the multi-threaded implementation in a number of ways. A simple improvement is to increase how much work each thread has to do, in this case, by having each thread compute one or more rows of the output matrix. The number of rows per thread is configured using a command line argument.

### Multi-threaded case 3 - n rows per _task_, with a fixed number of worker threads

In the previous two examples, the number of threads was tied to the size of the output. This is more efficient, but depends on a careful choice of `<rows-per-thread>` to ensure that the optimal number of threads are created. For example, on a six-core CPU with hyper-threading we would typically target 12 threads.

Instead of creating one thread for each piece of work to be completed, we can instead use a fixed number of worker threads, specified as a command line argument. Then, each piece of work can be represented as a task in a queue, and the worker threads fetch tasks from the queue until there are none remaining.

As in case 2, the number of rows per task is specified using a command line argument - this determines the size of each task. To govern access to the queue, we use a simple mutex.
