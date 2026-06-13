#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dot.h"

// Variables with these specific names are only used to pass
// metadata to Enzyme
int enzyme_const, enzyme_dup, enzyme_out;

double __enzyme_autodiff(void*, ...);

/**
 * Calculates u \dot v + c
 */
double dot(int n, double* u, double* v, double c) {
    double dot_prod = 0.0;

    for (int i = 0; i < n; i++) {
        dot_prod += u[i] * v[i];
    }

    return dot_prod + c;
}

int main(int argc, char **argv) {
    int n = 3;
    double *u = (double*) malloc(n * sizeof(double)); 
    double *v = (double*) malloc(n * sizeof(double));

    u[0] = 1.0; u[1] = 2.0; u[2] = 3.0;
    v[0] = 10.0; v[1] = 4.0; v[2] = 15.0;

    double c = 1.0;

    // For variables passed by address, we construct shadow variables
    // to store gradients for Enzyme
    double *d_u = (double*) malloc(n * sizeof(double));
    double *d_v = (double*) malloc(n * sizeof(double));

    // Zero initialize shadow variables
    memset(d_u, 0.0, n * sizeof(double)); 
    memset(d_v, 0.0, n * sizeof(double));

   double d_c = 0.0;

    d_c = __enzyme_autodiff((void*) dot,
                       enzyme_const, n,
                       enzyme_dup, u, d_u,
                       enzyme_dup, v, d_v,
                       enzyme_out, c);

    printf("u = {%f, %f, %f}\n", u[0], u[1], u[2]);
    printf("v = {%f, %f, %f}\n", v[0], v[1], v[2]);
    printf("c = %f\n", c);

    printf("\n");

    // We expect d(dot)/du = v, d(dot)/dv = u
    printf("d(dot)/du = {%f, %f, %f}\n", d_u[0], d_u[1], d_u[2]);
    printf("d(dot)/dv = {%f, %f, %f}\n", d_v[0], d_v[1], d_v[2]);

    // We expect d(dot)/dc = 1.0
    printf("d(dot)/dc = %f\n", d_c);

    return 0;
}
