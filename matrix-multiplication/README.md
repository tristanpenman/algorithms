# Matrix Multiplication

A comparison of different approaches to matrix multiplication.

## Contents

The examples are grouped into three sections:

* [Basic Examples](#basic-examples) - These are simple (single-threaded) examples explore the basic algorithms for matrix multiplication
* [Multithreaded Examples](#multithreaded-examples) - These examples make use of multithreading to perform matrix multiplication across multiple cores on a single machine
* [Advanced Examples](#advanced-examples) - These examples make use of MPI to distribute work across multiple nodes

## Compiling

The Basic and Multithreaded Examples do not have external dependencies (other than a C++17 compiler), and can all be compiled using the included Makefile:

    make

When run without any command line arguments, all examples will print usage information:

    $ ./Sequential

    Usage:
      ./Sequential <M1> <N1/M2> <N2> [seed]

    Multiplies a random M1xN1 matrix by a random M2xN2 matrix

Arguments between `<>` are required, while those between `[]` are optional.

The Advanced Examples, which use MPI, must be compiled using their own `make` commands. These steps are documented below.

## Basic Examples

### Sequential - Naive implementation

This is the simplest case, using a basic O(n^3) matrix multiplication algorithm.

### Recursive Case 1 - Divide and conquer

This example uses recursion to break matrix multiplication into smaller sub-problems. It recursively multiplies, and then sums, sub-blocks of the input matrices. This is also O(n^3), but in practice, the additional function call overhead and cost memory copies makes this slower than the naive sequential algorithm.

In order to handle rectangular matrices, the recursive implementations use larger square matrices to perform the multiplication. The size of these matrices is also rounded up to a power of two, to ensure that the work can be evenly divided. A smaller 'slice' of the output matrix is then used to display the result.

### Recursive Case 2 - Strassen's algorithm

The next algorithm is [Strassen's algorithm](https://en.wikipedia.org/wiki/Strassen_algorithm), which is an O(n^log2(7)) algorithm for matrix multiplication. The lower time bound is achieved by reducing the number of sub-block multiplications from 8 to 7, while increasing the number of additions.

## Multithreaded Examples

### Multithreaded Case 1 - One cell per thread

This example builds on the Sequential case, by creating one thread per output cell. This demonstrates that over-use of threads can negate the benefit of parallel computation. i.e. if the work is too fine-grained, then the overhead of creating threads can be greater than the work itself.

### Multithreaded Case 2 - Multiple rows per thread

We can improve the multi-threaded implementation in a number of ways. A simple improvement is to increase how much work each thread has to do, in this case, by having each thread compute one or more rows of the output matrix. The number of rows per thread is configured using a command line argument.

### Queue-based Case 1 - Multiple rows per _task_, with a fixed number of worker threads

In the previous two examples, the number of threads was tied to the size of the output. This is more efficient, but depends on a careful choice of `<rows-per-thread>` to ensure that the optimal number of threads are created. For example, on a six-core CPU with hyper-threading we would typically target 12 threads.

Instead of creating one thread for each piece of work to be completed, we can instead use a fixed number of worker threads, specified as a command line argument. Then, each piece of work can be represented as a task in a queue, and the worker threads fetch tasks from the queue until there are none remaining.

As in case 2, the number of rows per task is specified using a command line argument - this determines the size of each task. To govern access to the queue, we use a simple mutex.

## Advanced Examples

### MPI

This example distributes computation across multiple nodes using [MPI](https://en.wikipedia.org/wiki/Message_Passing_Interface). Similar to the second multithreaded example, the task of calculating of a fixed number of rows is given to each node. The exact number of rows is determined based on the number of nodes that are available.

To compile and run this example, you will need to install an MPI implementation, such as [Open MPI](https://www.open-mpi.org/) or [MPICH](https://www.mpich.org/). On Mac, Open MPI can be installed using Homebrew:

    brew install open-mpi

On Linux, you'll need to find an appropriate MPI dev package in your package manager.

To compile the example, you will need to run `make` separately (this will use `mpic++` to compile the program):

    make MPI

Although you could run this example as-is, to see it work across multiple processes you will need to use `mpirun`:

    mpirun -n 2 ./MPI 8 8 8

Here the option `-n 2` determines how many processes to spawn. Here is example output when multiplying two 4x4 matrices (with the `DEBUG` option enabled):

    Cluster size: 2
    Random seeds: 0, 1

    Matrix A:
    18.5689 68.8531 71.5891 69.4503
    24.7127 -23.1237 -40.4931 -88.6574
    -45.4687 -4.46698 62.4337 -4.00457
    -21.443 67.2158 -32.5208 29.6344

    Matrix B:
    99.437 86.5115 -74.3751 99.8081
    -52.7822 -20.6839 -22.4179 33.9492
    87.1078 69.2622 -37.3453 4.90963
    -11.3094 -54.0846 6.88278 82.7924

    Matrix C:
    3662.75 1384.5 -5120.11 10292.3
    1153.28 4606.58 -417.613 -5857.46
    1198.26 699.709 1122.71 -4714.82
    -8847.99 -7100.57 1506.46 2435.57

    Duration: 158 microseconds (0.000158 seconds)

### MPI + CUDA

Now we'll build on the previous example using CUDA, NVIDIA's proprietary GPU programming interface. Like the previous example, the computation can be spread across multiple nodes, but now the individual matrix multiplications are performed on the GPU. The drawback of using CUDA is that it is limited to systems running NVIDIA GPUs.

To compile this example, you will need to run `make` separately:

    make MPI_CUDA

Then you can run it with `mpirun`:

    mpirun -n 2 ./MPI 8 8 8

When you run this example, it will first compute the result using the GPU, then it will compare it to a result computed using the naive sequential method.

### MPI + OpenCL

The final example builds upon the first MPI example by using the OpenCL to do the work for each node. This allows computation to be performed on a GPU (if present), or even on devices such as FPGAs.

You will need to run `make` separately:

    make MPI_OpenCL

Usage is the same as the previous example.

Note that, unlike the MPI + CUDA example, the `multiply_matrices_k` kernel is compiled at runtime, so the file `MPI_OpenCL.cl` must be present alongside the main executable. This would also allow the kernel to be updated or replaced without recompiling the program.
