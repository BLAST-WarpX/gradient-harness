/**
 * Enzyme (single source) + CUDA (multisource) example
 *
 * In this example we split the CUDA build over multiple translation units (CUDA kernels are allowed
 * to call kernels in other TUs). But we only use Enzyme autodiff within a single TU.
 */
#include <__clang_cuda_runtime_wrapper.h>
#include <iostream>
#include <sstream>
#include <cstddef>
#include <cstring>
#include <cstdio>

#include <cuda/cmath>

#include "saxpy.cuh"

/**
 * Currently we have not successfully gotten clang to build CUDA RDC code that
 * launches a __global__ function defined in a separate translation unit.
 * The workaround for now is we define a "launcher" function in the same source
 * file as the launch which wraps the kernel we are launching.
 */ 
__global__
void saxpyLauncher(int n, float * a, float * x, float * y) {
    saxpy(n, a, x, y);
}

__global__
void dsaxpyLauncher(int n, float* a,  float* da, float* x, float* dx, float* y, float* dy) {
    dsaxpy(n, a, da, x, dx, y, dy);
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
    int n = 4;  // length of saxpy arrays
    std::size_t fArrSize = n*sizeof(float);
    int threadsPerBlock = 2;
    int blocks = cuda::ceil_div(n, threadsPerBlock);

    // Pointers to host memory
    float *a_host;
    float *x_host, *y_host;
    float *da_host, *dx_host, *dy_host; 

    // Pointers to device memory 
    float *a;
    float *x, *y;
    float *da, *dx, *dy;

    // Allocate host memory
    cudaMallocHost(&a_host, sizeof(float));
    cudaMallocHost(&x_host, fArrSize);
    cudaMallocHost(&y_host, fArrSize);
    cudaMallocHost(&da_host, sizeof(float));
    cudaMallocHost(&dx_host, fArrSize);
    cudaMallocHost(&dy_host, fArrSize);

    // Allocate device memory
    cudaMalloc(&a, sizeof(float));
    cudaMalloc(&x, fArrSize);
    cudaMalloc(&y, fArrSize);
    cudaMalloc(&da, sizeof(float));
    cudaMalloc(&dx, fArrSize);
    cudaMalloc(&dy, fArrSize);

    /// Initialize host memory
    *a_host = 12.0;
    x_host[0] = 1.0; x_host[1] = 2.0; x_host[2] = 3.0; x_host[3] = 4.0;
    y_host[0] = 5.0; y_host[1] = 6.0; y_host[2] = 7.0; y_host[3] = 8.0;
    
    // Initialize shadow variables
    *da_host = 0.0;
    std::memset(dx_host, 0, fArrSize);
    
    // For saxpy the "output" is stored in *y
    // For Enzyme reverse mode, this means dy is the "input" to the AD algorithm.
    // After dsaxpy runs, the array [da, dx, dy] contains Jf^t * dy_in,
    // where dy_in is the initial value of dy and Jf is the Jacobian of f:
    // f(a, x, y) = a*x + y
    //
    // See the comment above dsaxpy for more detail.
    std::memset(dy_host, 0, fArrSize);
    dy_host[0] = 1.0;

    std::cout << "a = " << *a_host << "\n";
    std::cout << "x = " << arrayToString(n, x_host) << "\n";
    std::cout << "y = " << arrayToString(n, y_host) << "\n";
    std::cout << "da = " << *da_host << "\n";
    std::cout << "dx = " << arrayToString(n, dx_host) << "\n";
    std::cout << "dy = " << arrayToString(n, dy_host) << "\n";

    std::cout << "\n";

    // Copy host memory to device
    cudaMemcpy(a, a_host, sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(x, x_host, fArrSize, cudaMemcpyHostToDevice);
    cudaMemcpy(y, y_host, fArrSize, cudaMemcpyHostToDevice);
    cudaMemcpy(da, da_host, sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(dx, dx_host, fArrSize, cudaMemcpyHostToDevice);
    cudaMemcpy(dy, dy_host, fArrSize, cudaMemcpyHostToDevice);

    //saxpyWrapper<<<blocks, threadsPerBlock>>>(n, a, x, y);
    dsaxpyLauncher<<<blocks, threadsPerBlock>>>(n, a, da, x, dx, y, dy);
    
    cudaDeviceSynchronize();

    // Copy device memory back to host
    cudaMemcpy(a_host, a, sizeof(float), cudaMemcpyDeviceToHost);
    cudaMemcpy(x_host, x, fArrSize, cudaMemcpyDeviceToHost);
    cudaMemcpy(y_host, y, fArrSize, cudaMemcpyDeviceToHost);
    cudaMemcpy(da_host, da, sizeof(float), cudaMemcpyDeviceToHost);
    cudaMemcpy(dx_host, dx, fArrSize, cudaMemcpyDeviceToHost);
    cudaMemcpy(dy_host, dy, fArrSize, cudaMemcpyDeviceToHost);

    std::cout << "a = " << *a_host << "\n";
    std::cout << "x = " << arrayToString(n, x_host) << "\n";
    std::cout << "y = " << arrayToString(n, y_host) << "\n";
    std::cout << "da = " << *da_host << "\n";
    std::cout << "dx = " << arrayToString(n, dx_host) << "\n";
    std::cout << "dy = " << arrayToString(n, dy_host) << "\n";

    cudaFreeHost(a_host);
    cudaFreeHost(x_host);
    cudaFreeHost(y_host);
    cudaFreeHost(da_host);
    cudaFreeHost(dx_host);
    cudaFreeHost(dy_host);

    cudaFree(a);
    cudaFree(x);
    cudaFree(y);
    cudaFree(da);
    cudaFree(dx);
    cudaFree(dy);

    return 0;
}
