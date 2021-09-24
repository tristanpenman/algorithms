#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "Matrix.h"
#include "Queue.h"

using namespace std;
using namespace std::chrono;

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
void work(Queue<Task<T>> &tasks)
{
  while (auto task = tasks.pop()) {
    const auto n_a = task->matrix_a.columns();

    for (int m = task->m_begin; m < task->m_end; m++) {
      for (int n = 0; n < task->matrix_c.columns(); n++) {
        // find value of cell [m,n]
        T sum = 0;
        for (int i = 0; i < n_a; i++) {
          sum += task->matrix_a.get(m, i) * task->matrix_b.get(i, n);
        }

        // store value
        task->matrix_c.set(m, n, sum);
      }
    }
  }
}

template<typename T>
void multiply_matrices(const Matrix<T> &matrix_a, const Matrix<T> &matrix_b, Matrix<T> &matrix_c, int rows_per_thread, int num_threads)
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

  // track worker threads
  vector<thread> workers;

  // track tasks
  Queue<Task<T>> tasks;

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

    tasks.push(task);
  }

  for (int i = 0; i < num_threads; i++) {
    // create worker thread
    thread worker(work<T>, ref(tasks));
    workers.push_back(move(worker));
  }

  // wait for all worker threads to finish
  for (auto &worker : workers) {
    worker.join();
  }
}

int usage(char **argv)
{
  cout << endl;
  cout << "Usage:" << endl;
  cout << "  " << argv[0] << " <M1> <N1/M2> <N2> <rows-per-thread> <num-threads> [seed]" << endl;
  cout << endl;
  cout << "Multiples a random M1xN1 matrix by a random M2xN2 matrix" << endl;

  return 1;
}

int main(int argc, char **argv)
{
  if (argc == 1) {
    return usage(argv);
  }

  if (argc != 6 && argc != 7) {
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

  int rows_per_thread = atoi(argv[4]);
  if (rows_per_thread <= 0) {
    cout << "Argument <rows-per-thread> is invalid" << endl;
    return usage(argv);
  }

  int num_threads = atoi(argv[5]);
  if (num_threads <= 0) {
    cout << "Argument <num-threads> is invalid" << endl;
    return usage(argv);
  }

  optional<int> seed;
  if (argc == 7) {
    seed = atoi(argv[7]);
    cout << "Random seeds: " << *seed << ", " << (*seed + 1) << endl;
  }

  // first input matrix
  Matrix<double> matrix_a(m_a, n_a);
  matrix_a.randomise(-100, 100, seed);

  if (seed) {
    seed = *seed + 1;
  }

  // second input matrix
  Matrix<double> matrix_b(n_a, n_b);
  matrix_b.randomise(-100, 100, seed);

#ifdef DEBUG
  cout << "Matrix A:" << endl;
  cout << matrix_a << endl;
  cout << "Matrix B:" << endl;
  cout << matrix_b << endl;
#endif

  // output matrix
  Matrix<double> matrix_c(m_a, n_b);

  // do the work
  auto start = high_resolution_clock::now();
  multiply_matrices(matrix_a, matrix_b, matrix_c, rows_per_thread, num_threads);
  auto stop = high_resolution_clock::now();

#ifdef DEBUG
  cout << "Matrix C:" << endl;
  cout << matrix_c;
#endif

  // how long did it take?
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "Duration: " << duration.count() << " microseconds (" << (double(duration.count()) / 1000000.0f) << " seconds)" << endl;

  return 0;
}
