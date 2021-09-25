#pragma once

#include <cstdlib>
#include <iostream>
#include <optional>
#include <random>

template<typename T>
class Matrix
{
public:
  Matrix(int rows, int columns)
    : m_rows(rows)
    , m_columns(columns)
  {
    m_values = new T[rows * columns];
  }

  ~Matrix()
  {
    delete[] m_values;
  }

  int columns() const
  {
    return m_columns;
  }

  T* data()
  {
    return m_values;
  }

  T get(int row, int column) const
  {
    if (row < m_rows && column < m_columns) {
      return m_values[row * m_columns + column];
    } else {
      return 0;
    }
  }

  void randomise(T min, T max, int m, int n, std::optional<int> seed = {})
  {
    std::uniform_real_distribution<T> dist(min, max);
    std::mt19937 engine;
    if (seed) {
      engine.seed(*seed);
    } else {
      std::random_device rd;
      engine.seed(rd());
    }

    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        m_values[i * m_columns + j] = dist(engine);
      }
    }
  }

  void randomise(T min, T max, std::optional<int> seed = {})
  {
    randomise(min, max, m_rows, m_columns, seed);
  }

  int rows() const
  {
    return m_rows;
  }

  void set(int row, int column, T value)
  {
    m_values[row * m_columns + column] = value;
  }

  bool operator==(const Matrix &rhs)
  {
    if (rhs.m_rows != m_rows || rhs.m_columns != m_columns) {
      return false;
    }

    for (int i = 0; i < m_rows; i++) {
      for (int j = 0; j < m_columns; j++) {
        if (abs(m_values[i * m_columns + j] - rhs.m_values[i * m_columns + j]) > 0.0001) {
          return false;
        }
      }
    }

    return true;
  }

private:
  int m_rows;
  int m_columns;

  T* m_values;
};

template<typename T>
std::ostream& operator<<(std::ostream &os, const Matrix<T> &matrix)
{
  for (int m = 0; m < matrix.rows(); m++) {
    for (int n = 0; n < matrix.columns(); n++) {
      os << matrix.get(m, n) << " ";
    }
    os << std::endl;
  }

  return os;
}
