#pragma once

#include <iostream>

#include "Matrix.h"

// Represents a square region, or slice, of a Matrix<T>
template<typename T>
class Slice
{
public:
  Slice(Matrix<T>& matrix, int m_offset, int rows, int n_offset, int columns)
    : m_matrix(matrix)
    , m_m_offset(m_offset)
    , m_rows(rows)
    , m_n_offset(n_offset)
    , m_columns(columns) {}

  int columns() const
  {
    return m_columns;
  }

  T get(int m, int n) const
  {
    return m_matrix.get(m + m_m_offset, n + m_n_offset);
  }

  int rows() const
  {
    return m_rows;
  }

  void set(int m, int n, T value)
  {
    m_matrix.set(m + m_m_offset, n + m_n_offset, value);
  }

private:
  Matrix<T> &m_matrix;

  int m_m_offset;
  int m_rows;

  int m_n_offset;
  int m_columns;
};

template<typename T>
std::ostream& operator<<(std::ostream &os, const Slice<T> &slice)
{
  for (int m = 0; m < slice.rows(); m++) {
    for (int n = 0; n < slice.columns(); n++) {
      os << slice.get(m, n) << " ";
    }
    os << std::endl;
  }

  return os;
}
