#pragma once

void multiply_matrices_cuda(
    const double* matrix_a,
    const double* matrix_b,
    int m_a,
    int n_a,
    int n_b,
    double* matrix_c);
