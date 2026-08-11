#ifndef SAXPY_H
#define SAXPY_H

/*
 * BLAS kernel saxpy (Single-precision A*X Plus Y)
 * For work index i, computes a*x[i] + y[i] and stores result in y[i]
 */
__device__
void saxpy(int n, float * a, float * x, float * y);

/**
 * Currently Enzyme can only differentiate device functions
 * We create a global wrapper of saxpy so it can still be
 * called from the host.
 */
__global__
void saxpyWrapper(int n, float * a, float * x, float * y);

/**
 * Reverse mode derivative of saxpy
 *
 * Let f(a, x, y) = a*x + y, Jf Jacobian of f w.r.t the vector [a, x, y]
 * Let dy_in = value of dy at the time dsaxpy is called
 * After the dsaxpy call, we have dy = Jf^t * dy_in
 */
__global__
void dsaxpy(int n, float * a, float * da, float * x, float * dx, float * y, float * dy);
 
#endif
