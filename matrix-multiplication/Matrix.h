#pragma once

#include <cstdlib>
#include <iostream>

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

  T get(int row, int column) const
  {
    if (row < m_rows && column < m_columns) {
      return m_values[row * m_columns + column];
    } else {
      return 0;
    }
  }

  void randomise(T min, T max, int m, int n)
  {
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < n; j++) {
        m_values[i * m_columns + j] = random_value(min, max);
      }
    }
  }

  void randomise(T min, T max)
  {
    randomise(min, max, m_rows, m_columns);
  }

  int rows() const
  {
    return m_rows;
  }

  void set(int row, int column, T value)
  {
    m_values[row * m_columns + column] = value;
  }

private:
  T random_value(T min, T max)
  {
    return min + static_cast<T>(rand()) / static_cast<T>(RAND_MAX / (max - min));
  }

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
