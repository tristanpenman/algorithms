#include <iostream>

using namespace std;

#define GPU_CHECK(ans) { gpu_assert((ans), __FILE__, __LINE__); }

inline void gpu_assert(cudaError_t code, const char *file, int line)
{
  if (code != cudaSuccess) {
    cerr << "gpu_assert: " << cudaGetErrorString(code) << " " << file << ":" << line;
    exit(code);
  }
}

__global__ void multiply_matrices_gpu_k(const double* dev_matrix_a, const double* dev_matrix_b, int m_a, int n_a, int n_b, double* dev_matrix_c)
{
  // Determine which cell the current thread is working on
  uint m = (blockIdx.x * blockDim.x) + threadIdx.x;
  uint n = (blockIdx.y * blockDim.y) + threadIdx.y;

  // Because our blocks are 8x8, and the size of the output matrix may not necessarily
  // be a multiple of 8x8 blocks, we need to ignore threads that correspond to cells that
  // are not part of the output matrix.
  if (m >= m_a || n >= n_b) {
    return;
  }

  // Computing the value of a cell is pretty easy at this point
  double sum = 0;
  for (int i = 0; i < n_a; i++) {
    sum += dev_matrix_a[m * n_a + i] * dev_matrix_b[i * n_b + n];
  }

  dev_matrix_c[m * n_b + n] = sum;
}

void multiply_matrices_gpu(const double* matrix_a, const double* matrix_b, int m_a, int n_a, int n_b, double* matrix_c)
{
  // allocate GPU memory for matrix A
  size_t sz_matrix_a = m_a * n_a * sizeof(double);
  double* dev_matrix_a;
  GPU_CHECK( cudaMalloc(&dev_matrix_a, sz_matrix_a) );

  // allocate GPU memory for matrix B
  size_t sz_matrix_b = n_a * n_b * sizeof(double);
  double* dev_matrix_b;
  GPU_CHECK( cudaMalloc(&dev_matrix_b, sz_matrix_b) );

  // allocate GPU memory for output array
  size_t sz_matrix_c = m_a * n_b * sizeof(double);
  double* dev_matrix_c;
  GPU_CHECK( cudaMalloc(&dev_matrix_c, sz_matrix_c) );

  // Copy input arrays into GPU memory
  GPU_CHECK( cudaMemcpy(dev_matrix_a, matrix_a, sz_matrix_a, cudaMemcpyHostToDevice) );
  GPU_CHECK( cudaMemcpy(dev_matrix_b, matrix_b, sz_matrix_b, cudaMemcpyHostToDevice) );

  // In CUDA, a group of threads is called a block. Blocks are important, because a block may only
  // run on a single SM (streaming multiprocessor). More than one block can run on each SM. Here
  // we're saying a block represents an 8x8 portion of the output matrix.
  const dim3 block_dim(8, 8);

  // Blocks are then arranged into a grid. The number of cells in the grid is determined by taking
  // the output matrix size, and dividing it by the size of the block. The conention we use here
  // is that the X axis of a block/grid represents rows, and the Y axis represents columns.
  const dim3 grid_dim(ceil(float(m_a) / block_dim.x), ceil(float(n_b) / block_dim.y));

  // do the work
  multiply_matrices_gpu_k<<<grid_dim, block_dim>>>(dev_matrix_a, dev_matrix_b, m_a, n_a, n_b, dev_matrix_c);

  // copy result back into host memory
  GPU_CHECK( cudaMemcpy(matrix_c, dev_matrix_c, sz_matrix_c, cudaMemcpyDeviceToHost) );

  GPU_CHECK( cudaFree(dev_matrix_a) );
  GPU_CHECK( cudaFree(dev_matrix_b) );
  GPU_CHECK( cudaFree(dev_matrix_c) );
}
