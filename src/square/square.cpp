#include "square.h"

#include <vector>
#include <iostream>

double __enzyme_autodiff(void*, double);

double square(double x) {
    return x * x;
}

double dsquare(double x) {
    return __enzyme_autodiff((void*) square, x);
}

int main() {
    std::vector<double> test_vals = {1.0, 2.0, 4.5, 10};

    for (double x : test_vals) {
        std::cout << "x = " << x << "; square = " << square(x)
             << "; dsquare = " << dsquare(x) << "\n";
    }

    return 0;
}

