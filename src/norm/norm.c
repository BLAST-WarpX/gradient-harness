#include <stdio.h>
#include <math.h>

#include "norm.h"

int enzyme_const, enzyme_dup, enzyme_out;
struct d_norm_output __enzyme_autodiff(void*, ...);

// Euclidean norm of vector {x, y}
double norm(double x, double y) {
    return sqrt(x*x + y*y);
}

int main() {
    // norm(3.0, 4.0) = 5.0
    double x = 3.0;
    double y = 4.0;

    // We store the two enzyme_out quantities in a struct
    struct d_norm_output output = __enzyme_autodiff((void*) norm, 
            enzyme_out, x,
            enzyme_out, y);
    
    // Expect d(norm)/dx = x/norm(x, y)
    printf("x = %f; y = %f\n", x, y);
    printf("d_x = %f; d_y = %f\n", output.x, output.y);

    // Expect the derivative at (0.0, 0.0) to be undefined
    // In practice, Enzyme returns d_x = 0.0
    x = 0.0;
    y = 0.0;

    output = __enzyme_autodiff((void*) norm, 
            enzyme_out, x,
            enzyme_out, y);

    printf("x = %f; y = %f\n", x, y);
    printf("d_x = %f; d_y = %f\n", output.x, output.y);

    return 0;
}
