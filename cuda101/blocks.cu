#include <iostream>
#define N 100000
__global__ void addOfblocks(int n, float *x, float *y) {
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  int stride = blockDim.x * gridDim.x;
  for (int i = idx; i < n; i += stride) y[i] = x[i] + y[i];
}
__global__ void addOfThreads(int n, float *x, float *y) {
  int idx = threadIdx.x;
  int stride = blockDim.x;
  for (int i = idx; i < n; i += blockDim) y[i] = x[i] + y[i];
}
int main() {
  float *a, *out;

  cudaMallocManaged(&a, N * sizeof(float));
  cudaMallocManaged(&out, N * sizeof(float));
  for (int i = 0; i < N; i++) {
    a[i] = 2;
    b[i] = 4;
  }

  add<<<2, 4>>>(N, a, b);
  cudaDeviceSynchronize();

  cudaFree(a);
  cudaFree(b);
  return 0;
}