#include <stdio.h>
#include <math.h>

#include "norm.h"

int enzyme_const, enzyme_dup, enzyme_out;
double __enzyme_autodiff(void*, ...);

// Euclidean norm of vector {x, y}
double norm(double x, double y) {
    return sqrt(x*x + y*y);
}

int main() {
    double x = 0.0;
    double y = 0.0;

    double d_x = __enzyme_autodiff((void*) norm, 
            enzyme_out, x,
            enzyme_const, y);

    // Expect the derivative at (0.0, 0.0) to be undefined
    // In practice, Enzyme returns d_x = 0.0
    printf("x = %f; y = %f\n", x, y);
    printf("d_x = %f\n", d_x);

    return 0;
}
