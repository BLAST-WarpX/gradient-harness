// Note: Forward mode has less overhead than backward mode, so we should
// bias towards forward mode when the number of inputs isn't much larger 
// than the number of outputs

#include <stdlib.h>
#include <stdio.h>

#include "forward_mode.h"

int enzyme_const, enzyme_out, enzyme_dup;
double __enzyme_fwddiff(void*, ...);

// Returns x*{0, ... , n-1} + y*{n, ... , 2n - 1}  
// Stores output in provided pointer z
void func(int n, double x, double y, double* z) {
    for (int i = 0; i < n; i++) {
        z[i] = i*x + (n + i)*y;
    }
}

int main() {
    int n = 10;
    double x = 1.0;
    double y = 1.0;

    // In forward mode, the enzyme_dup shadow variables are used to tell enzyme which directional derivative
    // to compute. Setting {dx, dy} = {1.0, 0.0} calculates the derivative of each of the outputs with respect
    // to x and stores the values in the array dz.
    double dx = 1.0;
    double dy = 0.0;

    double *z = malloc(n*sizeof(double));
    double *dz = malloc(n*sizeof(double));

    // Zero initialize output z and shadow d_z
    for (int i = 0; i < n; i++) {
        z[i] = 0.0;
        dz[i] = 0.0;
    }

    __enzyme_fwddiff((void*)func,
            enzyme_const, n,
            enzyme_dup, x, dx, 
            enzyme_dup, y, dy,
            enzyme_dup, z, dz); 

    // We expect d(z[i])/dx = i, d(z[i])/dy = (n+i)
    printf("dz[2]/dx = %f\n", dz[2]);

    return 0;
}
