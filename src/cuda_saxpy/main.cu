/**
 * Enzyme CUDA example
 */
#include <iostream>
#include <sstream>

#include <cuda/cmath>

#include "saxpy.cuh"

extern int __device__ enzyme_dup;
extern int __device__ enzyme_out;
extern int __device__ enzyme_const;

template <typename RT, typename... T>
__device__ RT __enzyme_autodiff(void*, T...);

/**
 * Reverse mode derivative of saxpy
 *
 * Let f(a, x, y) = a*x + y, Jf Jacobian of f w.r.t the vector [a, x, y]
 * Let lambda = value of dy at the time dsaxpy is called
 * After the dsaxpy call, we have dy = Jf^t * lambda
 */
__global__
void dsaxpy(int n, float a, float * da, float * x, float * dx, float * y, float * dy) {
    __enzyme_autodiff<void>((void*)saxpy,
            enzyme_dup, a, da, enzyme_dup, x, dx, enzyme_dup, y, dy);
}

std::string arrayToString(int n, float * arr) {
    std::stringstream ss;
    ss << "{ ";
    for (int i = 0; i <= n - 2; i++) {
        ss << arr[i] << ", ";
    }
    ss << arr[n-1] << " }";
    return ss.str();
}

int main() {
    int n = 3;  // length of saxpy arrays
    int threadsPerBlock = 1;
    int blocks = cuda::ceil_div(n, threadsPerBlock);

    // Pointers to memory vectors
    float a;
    float *x, *y;
    float *da, *dx, *dy;

    // Allocate memory on GPU
    cudaMalloc(&x, n*sizeof(float));
    cudaMalloc(&y, n*sizeof(float));
    cudaMalloc(&dx, n*sizeof(float));
    cudaMalloc(&dy, n*sizeof(float));

    float a_host = 1.0;
    float x_host[] = {1.0, 2.0, 3.0};
    float y_host[] = {4.0, 5.0, 6.0};
    float da_host = 0.0;
    float dx_host[] = {0.0, 0.0, 0.0};
    float dy_host[] = {0.0, 0.0, 0.0};

    cudaMemcpy(x, &x_host, sizeof(*x), cudaMemcpyHostToDevice);
    cudaMemcpy(y, &y_host, sizeof(*y), cudaMemcpyHostToDevice);
    cudaMemcpy(da, &da_host, sizeof(*da), cudaMemcpyHostToDevice);
    cudaMemcpy(dx, &dx_host, sizeof(*dx), cudaMemcpyHostToDevice);
    cudaMemcpy(dy, &dy_host, sizeof(*dy), cudaMemcpyHostToDevice);

    dsaxpy<<<blocks, threadsPerBlock>>>(n, a, da, x, dx, y, dy);
    cudaDeviceSynchronize();

    cudaMemcpy(&x_host, x, sizeof(*x), cudaMemcpyDeviceToHost);
    cudaMemcpy(&y_host, y, sizeof(*y), cudaMemcpyDeviceToHost);
    cudaMemcpy(&da_host, da, sizeof(*da), cudaMemcpyDeviceToHost);
    cudaMemcpy(&dx_host, dx, sizeof(*dx), cudaMemcpyDeviceToHost);
    cudaMemcpy(&dy_host, dy, sizeof(*dy), cudaMemcpyDeviceToHost);

    std::cout << da_host << "\n";
    std::cout << arrayToString(n, dx_host) << "\n";
    std::cout << arrayToString(n, dy_host) << "\n";

    cudaFree(da);
    cudaFree(x);
    cudaFree(dx);
    cudaFree(y);
    cudaFree(dy);

    return 0;
}
