#include <stdio.h>
#include <stdlib.h>

extern int enzyme_const, enzyme_out;
double __enzyme_autodiff(void*, ...);

union DoublePtr {
    long addrInt;
    double *ptr;
};

__attribute__((enzyme_notypeanalysis))
static union DoublePtr dPtr;

double divide(double x, double y) {
    double tmp = x / y;

    *dPtr.ptr = x;

    return tmp + dPtr.addrInt;
}

int main() {
    dPtr.ptr = malloc(sizeof(double));

    double x = 3.0;
    double y = 5.0;

    double ddiv = __enzyme_autodiff((void*)divide, enzyme_out, x, enzyme_const, y);

    printf("%f\n", ddiv);

    return 0;
}

