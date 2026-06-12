#include "square.h"

#include "stdio.h"

double __enzyme_autodiff(void*, double);

double square(double x) {
    return x * x;
}

double dsquare(double x) {
    return __enzyme_autodiff((void*) square, x);
}

int main() {
    double test_vals[] = {1.0, 2.0, 4.5, 10};
    int n = sizeof(test_vals) / sizeof(test_vals[0]);

    for (int i = 0; i < n; i++) {
        double x = test_vals[i];
        printf("x = %8.2f; square(x) = %8.2f; dsquare(x) = %8.2f\n", x, square(x), dsquare(x));
    }

    return 0;
}

