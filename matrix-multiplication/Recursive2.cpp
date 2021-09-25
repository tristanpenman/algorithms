// #define DEBUG

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>

#include "Matrix.h"
#include "Slice.h"

using namespace std;
using namespace std::chrono;

bool power_of_two(int n)
{
  return (n & (n - 1)) == 0;
}

template<typename T, typename O>
void combine_matrices(const Matrix<T> &matrix_a, const Matrix<T> &matrix_b, Matrix<T> &matrix_c, int m_c, int n_c, O &op)
{
  // check input matrix sizes
  assert(matrix_a.rows() == matrix_b.rows());
  assert(matrix_a.columns() == matrix_b.columns());

  // // check output matrix size
  assert(m_c + matrix_a.rows() <= matrix_c.rows());
  assert(n_c + matrix_b.columns() <= matrix_c.columns());

  for (int m = 0; m < matrix_a.rows(); m++) {
    for (int n = 0; n < matrix_a.columns(); n++) {
      matrix_c.set(m_c + m, n_c + n, op(matrix_a.get(m, n), matrix_b.get(m, n)));
    }
  }
}

template<typename T>
Matrix<T> multiply_matrices(
    const Matrix<T> &matrix_a,
    const Matrix<T> &matrix_b,
    int m_a,
    int n_a,
    int m_b,
    int n_b,
    int size)
{
  assert(size >= 2);
  assert(power_of_two(size));

  // output matrix
  Matrix<T> matrix_c(size, size);

  // base case
  if (size == 2) {
    for (int m = 0; m < 2; m++) {
      for (int n = 0; n < 2; n++) {
        T sum = 0;
        for (int i = 0; i < 2; i++) {
          sum += matrix_a.get(m + m_a, i + n_a) * matrix_b.get(i + m_b, n + n_b);
        }
        matrix_c.set(m, n, sum);
      }
    }

    return matrix_c;
  }

  // multiply sub-blocks using naive approach
  int subsize = size / 2;

  std::plus<T> plus;

  // c_11 = a_11 * b_11 + a_12 * b_21
  combine_matrices(
      multiply_matrices(matrix_a, matrix_b, m_a, n_a, m_b, n_b, subsize),
      multiply_matrices(matrix_a, matrix_b, m_a, n_a + subsize, m_b + subsize, n_b, subsize),
      matrix_c,
      0,
      0,
      plus);

  // c_12 = a_11 * b_12 + a_12 * b_22
  combine_matrices(
      multiply_matrices(matrix_a, matrix_b, m_a, n_a, m_b, n_b + subsize, subsize),
      multiply_matrices(matrix_a, matrix_b, m_a, n_a + subsize, m_b + subsize, n_b + subsize, subsize),
      matrix_c,
      0,
      subsize,
      plus);

  // c_21 = a_21 * b_11 + a_22 * b_21
  combine_matrices(
      multiply_matrices(matrix_a, matrix_b, m_a + subsize, n_a, m_b, n_b, subsize),
      multiply_matrices(matrix_a, matrix_b, m_a + subsize, n_a + subsize, m_b + subsize, n_b, subsize),
      matrix_c,
      subsize,
      0,
      plus);

  // c_22 = a_21 * b_12 + a_22 * b_22
  combine_matrices(
      multiply_matrices(matrix_a, matrix_b, m_a + subsize, n_a, m_b, n_b + subsize, subsize),
      multiply_matrices(matrix_a, matrix_b, m_a + subsize, n_a + subsize, m_b + subsize, n_b + subsize, subsize),
      matrix_c,
      subsize,
      subsize,
      plus);

  return matrix_c;
}

int usage(char **argv)
{
  cout << endl;
  cout << "Usage:" << endl;
  cout << "  " << argv[0] << " <M1> <N1/M2> <N2> [seed]" << endl;
  cout << endl;
  cout << "Multiples a random M1xN1 matrix by a random M2xN2 matrix" << endl;

  return 1;
}

int pow2roundup(int x)
{
  --x;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  return x + 1;
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
    cout << "Argument M1 is invalid" << endl;
    return usage(argv);
  }

  int n_a = atoi(argv[2]);
  if (n_a <= 0) {
    cout << "Argument N1/M2 is invalid" << endl;
    return usage(argv);
  }

  int n_b = atoi(argv[3]);
  if (n_b <= 0) {
    cout << "Argument N2 is invalid" << endl;
    return usage(argv);
  }

  int seed = 0;
  if (argc == 5) {
    seed = atoi(argv[4]);
  }

  srand(seed);

  Matrix<double> matrix_a(m_a, n_a);
  matrix_a.randomise(-100, 100);

  Matrix<double> matrix_b(n_a, n_b);
  matrix_b.randomise(-100, 100);

  int max_rows = max(m_a, n_a);
  int max_cols = max(n_a, n_b);

  // TODO: pass real size and just use virtual cells
  int size = pow2roundup(max(max_rows, max_cols));

  // TODO: maybe allow matrix c to be written in place
  auto start = high_resolution_clock::now();
  Matrix<double> matrix_c = multiply_matrices(matrix_a, matrix_b, 0, 0, 0, 0, size);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "Duration: " << duration.count() << " microseconds (" << (double(duration.count()) / 1000000.0f) << " seconds)" << endl;

#ifdef DEBUG
  cout << "Matrix A:" << endl;
  cout << matrix_a << endl;

  cout << "Matrix B:" << endl;
  cout << matrix_b << endl;

  cout << "Matrix C:" << endl;
  Slice<double> slice(matrix_c, 0, m_a, 0, n_b);
  cout << slice;
#endif

  return 0;
}
