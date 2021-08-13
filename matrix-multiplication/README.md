# Matrix Multiplication

A comparison of different approaches to matrix multiplication.

## Sequential

This is the simplest case, using a basic O(n^3) matrix multiplication algorithm.

## Multi-threaded case 1 - one cell per thread

This example builds on the Sequential case, by creating one thread per output cell. This demonstrates that over-use of threads can negate the benefit of parallel computation. i.e. if the work is too fine-grained, then the overhead of creating threads can be greater than the work itself.

## Multi-threaded case 2 - n rows per thread

We can improve the multi-threaded implementation in a number of ways. A simple improvement is to increase how much work each thread has to do, in this case, by having each thread compute one or more rows of the output matrix. The number of rows per thread is configured using a command line argument.

