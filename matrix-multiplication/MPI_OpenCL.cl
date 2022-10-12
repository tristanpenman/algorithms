__kernel void multiply_matrices_k(
    const __global double* matrix_a,
    const __global double* matrix_b,
    int m_a,
    int n_a,
    int n_b,
    __global double* matrix_c)
{
  size_t m = get_global_id(0);
  size_t n = get_global_id(1);

  // Ensure that we're only operating on cells that are in the output matrix
  if (m >= m_a || n >= n_b) {
    return;
  }

  // Computing the value of a cell is pretty easy at this point
  double sum = 0;
  for (int i = 0; i < n_a; i++) {
    sum += matrix_a[m * n_a + i] * matrix_b[i * n_b + n];
  }

  matrix_c[m * n_b + n] = sum;
}
