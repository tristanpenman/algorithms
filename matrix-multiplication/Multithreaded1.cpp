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

  int m;
  int n;
};

template<typename T>
void work(Task<T> task)
{
  const auto n_a = task.matrix_a.columns();

  // find value of cell [m,n]
  T sum = 0;
  for (int i = 0; i < n_a; i++) {
    sum += task.matrix_a.get(task.m, i) * task.matrix_b.get(i, task.n);
  }

  // store value
  task.matrix_c.set(task.m, task.n, sum);
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
  assert(matrix_c.rows() == matrix_b.columns());

  // track worker threads
  vector<thread> workers;

  // fill output matrix
  for (int m = 0; m < m_a; m++) {
    for (int n = 0; n < n_b; n++) {
      // describe work to be done
      Task<T> task = {
        matrix_a,
        matrix_b,
        matrix_c,
        m,
        n
      };

      // create worker thread
      thread worker(work<T>, task);
      workers.push_back(std::move(worker));
    }
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
  cout << argv0 << " <M1> <N1/M2> <N2> [seed]" << endl;
  cout << endl;
  cout << "Multiples a random M1xN1 matrix by a random M2xN2 matrix" << endl;

  return 1;
}

int main(int argc, char **argv)
{
  if (argc != 4 && argc != 5) {
    cout << "Invalid argument count: " << argc << endl;
    return usage(argv[0]);
  }

  int m_a = atoi(argv[1]);
  if (m_a <= 0) {
    cout << "Argument M1 is invalid" << endl;
    return usage(argv[0]);
  }

  int n_a = atoi(argv[2]);
  if (n_a <= 0) {
    cout << "Argument N1/M2 is invalid" << endl;
    return usage(argv[0]);
  }

  int n_b = atoi(argv[3]);
  if (n_b <= 0) {
    cout << "Argument N2 is invalid" << endl;
    return usage(argv[0]);
  }

  int seed = 0;
  if (argc == 5) {
    seed = atoi(argv[4]);
  }

  srand(seed);

  Matrix<double> matrix_a(m_a, n_a);
  matrix_a.randomise(0, 100);
  cout << "Matrix A:" << endl;
  cout << matrix_a << endl;

  Matrix<double> matrix_b(n_a, n_b);
  matrix_b.randomise(0, 100);
  cout << "Matrix B:" << endl;
  cout << matrix_b << endl;

  Matrix<double> matrix_c(m_a, n_b);
  multiply_matrices(matrix_a, matrix_b, matrix_c);
  cout << "Matrix C:" << endl;
  cout << matrix_c;

  return 0;
}
