#ifndef SAXPY_H
#define SAXPY_H

/*
 * BLAS kernel saxpy (Single-precision A*X Plus Y)
 * For work index i, computes a*x[i] + y[i] and stores result in y[i]
 */
__device__
void saxpy(int n, float a, float * x, float * y);

/**
 * Currently Enzyme can only differentiate device functions
 * We create a global wrapper of saxpy so it can still be
 * called from the host.
 */
__global__
void saxpyWrapper(int n, float a, float * x, float * y);

#endif
