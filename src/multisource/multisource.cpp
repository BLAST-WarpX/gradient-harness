#include <iostream>

#include "math_utils.h"

int enzyme_const, enzyme_out;
double __enzyme_autodiff(void*, ...);

int main() {
    double x = 2.0;
    double y = 3.0;

    double dmult_dx = __enzyme_autodiff((void*) mult, x, y);

    std::cout << dmult_dx << "\n";

    return 0;
}
