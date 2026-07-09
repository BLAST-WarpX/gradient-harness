#include <iostream>

#include "square.h"

double __enzyme_autodiff(void*, double);

double square(double x) {
    return x * x;
}

double dsquare(double x) {
    return __enzyme_autodiff((void*) square, x);
}

int main(int argc, char** argv) {
    double x = 3.5;
    double dsq = dsquare(x);

    std::cout << "dsquare=" << dsq << "\n";
    
    return 0;
}

