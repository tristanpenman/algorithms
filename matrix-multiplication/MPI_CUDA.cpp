// #define DEBUG

#include <cassert>
#include <chrono>
#include <iostream>
#include <vector>

#include <mpi.h>

//
// The multiply_matrices_gpu function is declared in MPI_CUDA_K.h, and defined in MPI_CUDA_K.cu. That file
// must be compiled using nvcc, since it contains code that will run on the GPU.
//
// The current file cannot be compiled using nvcc because it makes use of C++17 features. 
//
#include "MPI_CUDA_K.h"

#include "Matrix.h"

using namespace std;
using namespace std::chrono;

int usage(char **argv)
{
  cout << endl;
  cout << "Usage:" << endl;
  cout << "  " << argv[0] << " <M1> <N1/M2> <N2> [seed]" << endl;
  cout << endl;
  cout << "Multiples a random M1xN1 matrix by a random M2xN2 matrix" << endl;

  return 1;
}

template<typename T>
void multiply_matrices(const Matrix<T> &matrix_a, const Matrix<T> &matrix_b, Matrix<T> &matrix_c)
{
  // check input matrix sizes
  const auto n_a = matrix_a.columns();
  const auto m_b = matrix_b.rows();
  assert(matrix_a.columns() == matrix_b.rows());

  // check output matrix size
  const auto m_a = matrix_a.rows();
  const auto n_b = matrix_b.columns();
  assert(matrix_c.rows() == matrix_a.rows());
  assert(matrix_c.columns() == matrix_b.columns());

  // fill output matrix
  for (int m = 0; m < m_a; m++) {
    for (int n = 0; n < n_b; n++) {

      // find value of cell [m,n]
      T sum = 0;
      for (int i = 0; i < n_a; i++) {
        sum += matrix_a.get(m, i) * matrix_b.get(i, n);
      }

      // store value
      matrix_c.set(m, n, sum);
    }
  }
}

int main(int argc, char **argv)
{
  if (argc == 1) {
    return usage(argv);
  }

  if (argc != 4 && argc != 5) {
    cout << "Invalid argument count: " << argc << endl;
    return usage(argv);
  }

  int m_a = atoi(argv[1]);
  if (m_a <= 0) {
    cout << "Argument <M1> is invalid" << endl;
    return usage(argv);
  }

  int n_a = atoi(argv[2]);
  if (n_a <= 0) {
    cout << "Argument <N1/M2> is invalid" << endl;
    return usage(argv);
  }

  int n_b = atoi(argv[3]);
  if (n_b <= 0) {
    cout << "Argument <N2> is invalid" << endl;
    return usage(argv);
  }

  MPI_Init(NULL, NULL);

  // how many processes are there?
  int cluster_size;
  MPI_Comm_size(MPI_COMM_WORLD, &cluster_size);

  // who am I?
  int host_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &host_rank);
  
  // matrices that are only used on the root node
  Matrix<double> matrix_a(m_a, n_a);
  Matrix<double> matrix_c(m_a, n_b);

  // matrices that are used on all nodes
  Matrix<double> matrix_a_partition(m_a / cluster_size, n_a);
  Matrix<double> matrix_b(n_a, n_b);
  Matrix<double> matrix_c_partition(m_a / cluster_size, n_b);

  if (host_rank == 0) {
    cout << "Cluster size: " << cluster_size << endl;

    // parse random seed
    std::optional<int> seed;
    if (argc == 5) {
      seed = atoi(argv[4]);
      cout << "Random seeds: " << *seed << ", " << (*seed + 1) << endl;
    }

    // generate random data on the root node
    matrix_a.randomise(-100, 100, seed);

    if (seed) {
      seed = *seed + 1;
    }

    matrix_b.randomise(-100, 100, seed);

#ifdef DEBUG
    cout << endl;
    cout << "Matrix A:" << endl;
    cout << matrix_a << endl;
    cout << "Matrix B:" << endl;
    cout << matrix_b << endl;
#endif
  }

  auto start = high_resolution_clock::now();

  // processes only need a subset of rows from matrix A, corresponding to their output rows
  MPI_Scatter(
      matrix_a.data(),              // address of send buffer (root node)
      m_a * n_a / cluster_size,     // number of elements sent to each process (root node)
      MPI_DOUBLE,                   // data type of send buffer elements (root node)
      matrix_a_partition.data(),    // address of receive buffer
      m_a * n_a / cluster_size,     // number of elements in receive buffer
      MPI_DOUBLE,                   // data type of receive buffer elements
      0,                            // ranking of sending process
      MPI_COMM_WORLD);              // communicator

  // all processes need all data from matrix B
  MPI_Bcast(
      matrix_b.data(),              // starting address of buffer
      n_a * n_b,                    // number of entries in buffer
      MPI_DOUBLE,                   // data type of buffer
      0,                            // rank of broadcast root
      MPI_COMM_WORLD);              // communicator

  // do the work
  multiply_matrices_gpu(
      matrix_a_partition.data(),
      matrix_b.data(),
      matrix_a_partition.rows(),
      matrix_a_partition.columns(),
      matrix_b.columns(),
      matrix_c_partition.data());

  // gather the results
  MPI_Gather(
      matrix_c_partition.data(),    // starting address of send buffer
      m_a * n_b / cluster_size,     // number of elements in send buffer
      MPI_DOUBLE,                   // data type of send buffer elements
      matrix_c.data(),              // starting address of receive buffer (root node)
      m_a * n_b / cluster_size,     // number of elements for any single receive (root node)
      MPI_DOUBLE,                   // data type of receive buffer elements (root node)
      0,                            // ranking of receiving process
      MPI_COMM_WORLD);              // communicator

  auto stop = high_resolution_clock::now();

  // only display complete results on the root node
  if (host_rank == 0) {
#ifdef DEBUG
    cout << "Matrix C:" << endl;
    cout << matrix_c << endl;
#endif

    // how long did it take?
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Duration: " << duration.count() << " microseconds (" << (double(duration.count()) / 1000000.0f) << " seconds)" << endl;
  
    cout << "Checking..." << endl;
    Matrix<double> matrix_d(m_a, n_b);
    multiply_matrices(matrix_a, matrix_b, matrix_d);

#ifdef DEBUG
    cout << endl;
    cout << "Matrix D: " << endl;
    cout << matrix_d << endl;
#endif
  
    if (matrix_c == matrix_d) {
      cout << "OK!" << endl;
    } else {
      cout << "Incorrect!" << endl;
    }
  }

  MPI_Finalize();

  return 0;
}
