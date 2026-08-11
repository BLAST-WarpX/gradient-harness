#include "saxpy.cuh"

extern int __device__ enzyme_dup;
extern int __device__ enzyme_out;
extern int __device__ enzyme_const;

template <typename RT, typename... T>
__device__ RT __enzyme_autodiff(void*, T...);

__device__
void saxpy(int n, float * a, float * x, float * y) {
    int i = threadIdx.x + blockDim.x * blockIdx.x;

    if (i < n) {
        y[i] = (*a)*x[i] + y[i];
    }
}

__global__
void saxpyWrapper(int n, float * a, float * x, float * y) {
    saxpy(n, a, x, y);
}

__global__
void dsaxpy(int n, float * a, float * da, float * x, float * dx, float * y, float * dy) {
    __enzyme_autodiff<void>((void*)saxpy,
            enzyme_const, n, enzyme_dup, a, da, enzyme_dup, x, dx, enzyme_dup, y, dy);
}

