#include <iostream>
#define N 10000000
__global__ void vector_add(float* out, float* a, float* b, int n) {
  int stride = 1;
  for (int i = 0; i < n; i += stride) {
    out[i] = a[i] + b[i];
  }
}

__global__ void cuda_hello() {
  printf("Hello from block %d, thread %d\n", blockDim.x, threadIdx.x);
}
int main() {
  cuda_hello<<<1, 4>>>();
  float *a, *b, *out;
  float *d_a, *d_b, *d_out;
  a = (float*)malloc(N * sizeof(float));
  b = (float*)malloc(N * sizeof(float));
  for (int i = 0; i < N; i++) {
    a[i] = 2;
  }
  for (int i = 0; i < N; i++) {
    b[i] = 4;
  }
  out = (float*)malloc(N * sizeof(float));
  cudaMalloc((void**)&d_b, sizeof(float) * N);
  cudaMemcpy(d_b, b, sizeof(float) * N, cudaMemcpyHostToDevice);

  cudaMalloc((void**)&d_out, sizeof(float) * N);
  cudaMemcpy(d_out, out, sizeof(float) * N, cudaMemcpyHostToDevice);

  cudaMalloc((void**)&d_a, sizeof(float) * N);
  cudaMemcpy(d_a, a, sizeof(float) * N, cudaMemcpyHostToDevice);
  vector_add<<<1, 1>>>(d_out, d_a, d_b, N);
  cudaMemcpy(out, d_out, sizeof(float) * N, cudaMemcpyDeviceToHost);
  printf("out[0] = %f\n", out[0]);
  cudaFree(d_a);
  free(a);
  return 0;
}