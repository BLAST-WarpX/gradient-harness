#include <iostream>

#include "math_utils.h"

// Note: extern is required for enzyme_* variables to work in multisource project 
extern int enzyme_const, enzyme_out, enzyme_dup;

template <typename RT, typename... T>
RT __enzyme_autodiff(void*, T...);

int main() {
    double x = 2.0;
    double y = 3.0;

    double dmult_dy = __enzyme_autodiff<double>((void*) mult, enzyme_const, x, enzyme_out, y);

    std::cout << dmult_dy << "\n";

    return 0;
}
