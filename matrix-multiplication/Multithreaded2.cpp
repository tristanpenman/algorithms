#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

#include "Matrix.h"

using namespace std;

template<typename T>
struct Task
{
  // input matrices
  const Matrix<T> &matrix_a;
  const Matrix<T> &matrix_b;

  // output
  Matrix<T> &matrix_c;

  int m_begin;
  int m_end;
};

template<typename T>
void work(Task<T> task)
{
  const auto n_a = task.matrix_a.columns();

  for (int m = task.m_begin; m < task.m_end; m++) {
    for (int n = 0; n < task.matrix_c.columns(); n++) {
      // find value of cell [m,n]
      T sum = 0;
      for (int i = 0; i < n_a; i++) {
        sum += task.matrix_a.get(m, i) * task.matrix_b.get(i, n);
      }

      // store value
      task.matrix_c.set(m, n, sum);
    }
  }
}

template<typename T>
void multiply_matrices(const Matrix<T> &matrix_a, const Matrix<T> &matrix_b, Matrix<T> &matrix_c, int rows_per_thread)
{
  // check input matrix sizes
  const auto n_a = matrix_a.columns();
  const auto m_b = matrix_b.rows();
  assert(matrix_a.columns() == matrix_b.rows());

  // check output matrix size
  const auto m_a = matrix_a.rows();
  const auto n_b = matrix_b.columns();
  assert(matrix_c.rows() == matrix_b.columns());

  // track worker threads
  vector<thread> workers;

  // fill output matrix
  for (int m_begin = 0; m_begin < m_a; m_begin += rows_per_thread) {

    // ensure work fragments do not fall outside input domain
    const int m_end = min(n_a, m_begin + rows_per_thread);

    // describe work to be done
    Task<T> task = {
      matrix_a,
      matrix_b,
      matrix_c,
      m_begin,
      m_end
    };

    // create worker thread
    thread worker(work<T>, task);
    workers.push_back(std::move(worker));
  }

  // wait for all worker threads to finish
  for (auto &worker : workers) {
    worker.join();
  }
}

template<typename T>
ostream& operator<<(ostream &os, const Matrix<T> &matrix)
{
  for (int m = 0; m < matrix.rows(); m++) {
    for (int n = 0; n < matrix.columns(); n++) {
      os << matrix.get(m, n) << " ";
    }
    os << endl;
  }

  return os;
}

int usage(char *argv0)
{
  cout << argv0 << " <M1> <N1/M2> <N2> <rows-per-thread> [seed]" << endl;
  cout << endl;
  cout << "Multiples a random M1xN1 matrix by a random M2xN2 matrix" << endl;

  return 1;
}

int main(int argc, char **argv)
{
  if (argc != 5 && argc != 6) {
    cout << "Invalid argument count: " << argc << endl;
    return usage(argv[0]);
  }

  int m_a = atoi(argv[1]);
  if (m_a <= 0) {
    cout << "Argument <M1> is invalid" << endl;
    return usage(argv[0]);
  }

  int n_a = atoi(argv[2]);
  if (n_a <= 0) {
    cout << "Argument <N1/M2> is invalid" << endl;
    return usage(argv[0]);
  }

  int n_b = atoi(argv[3]);
  if (n_b <= 0) {
    cout << "Argument <N2> is invalid" << endl;
    return usage(argv[0]);
  }

  int rows_per_thread = atoi(argv[4]);
  if (rows_per_thread <= 0) {
    cout << "Argument <rows-per-thread> is invalid" << endl;
    return usage(argv[0]);
  }

  if (argc == 6) {
    srand(atoi(argv[5]));
  }

  Matrix<double> matrix_a(m_a, n_a);
  matrix_a.randomise(-100, 100);
  cout << "Matrix A:" << endl;
  cout << matrix_a << endl;

  Matrix<double> matrix_b(n_a, n_b);
  matrix_b.randomise(-100, 100);
  cout << "Matrix B:" << endl;
  cout << matrix_b << endl;

  Matrix<double> matrix_c(m_a, n_b);
  multiply_matrices(matrix_a, matrix_b, matrix_c, rows_per_thread);
  cout << "Matrix C:" << endl;
  cout << matrix_c;

  return 0;
}
