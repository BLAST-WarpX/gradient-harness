#include <stdio.h>
#include <stdlib.h>

#include "square.h"

double __enzyme_autodiff(void*, double);

double square(double x) {
    return x * x;
}

double dsquare(double x) {
    return __enzyme_autodiff((void*) square, x);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        return 0;
    }

    double x = strtod(argv[1], NULL);
    double dx = dsquare(x);

    printf("%f\n", dx);
    
    return 0;
}

