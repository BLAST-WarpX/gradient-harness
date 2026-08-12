#ifndef SAXPY_H
#define SAXPY_H

/*
 * BLAS kernel saxpy implementation (Single-precision A*X Plus Y)
 * For work index i, computes a*x[i] + y[i] and stores result in y[i]
 */
#include <algorithm>
__device__
void saxpyImpl(int n, float* a, float* x, float* y);

/**
 * Reverse mode derivative of saxpy
 *
 * Let f(a, x, y) = a*x + y, Jf Jacobian of f w.r.t the vector [a, x, y]
 * Let dy_in = value of dy at the time dsaxpy is called
 * After the dsaxpy call, we have dy = Jf^t * dy_in
 */
__global__
void dsaxpy(int n, float* a, float* da, float* x, float* dx, float* y, float* dy);

/**
 * Global wrapper of saxpy
 * Enzyme can only differentiate device functions, so we create a global
 * wrapper around saxpyImpl.
 */ 
__global__
void saxpy(int n, float* a, float * x, float * y);

#endif
