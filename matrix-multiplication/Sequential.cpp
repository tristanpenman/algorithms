// #define DEBUG

#include <cassert>
#include <chrono>
#include <iostream>

#include "Matrix.h"

using namespace std;
using namespace std::chrono;

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

int usage(char **argv)
{
  cout << endl;
  cout << "Usage:" << endl;
  cout << "  " << argv[0] << " <M1> <N1/M2> <N2> [seed]" << endl;
  cout << endl;
  cout << "Multiples a random M1xN1 matrix by a random M2xN2 matrix" << endl;

  return 1;
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

  if (argc == 5) {
    srand(atoi(argv[4]));
  }

  Matrix<double> matrix_a(m_a, n_a);
  matrix_a.randomise(-100, 100);

  Matrix<double> matrix_b(n_a, n_b);
  matrix_b.randomise(-100, 100);

  Matrix<double> matrix_c(m_a, n_b);
  auto start = high_resolution_clock::now();
  multiply_matrices(matrix_a, matrix_b, matrix_c);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "Duration: " << duration.count() << " microseconds (" << (double(duration.count()) / 1000000.0f) << " seconds)" << endl;

#ifdef DEBUG
  cout << "Matrix A:" << endl;
  cout << matrix_a << endl;
  cout << "Matrix B:" << endl;
  cout << matrix_b << endl;
  cout << "Matrix C:" << endl;
  cout << matrix_c;
#endif

  return 0;
}
