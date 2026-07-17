#include <iostream>
#include <cmath>

#include "norm.h"

extern int enzyme_const, enzyme_dup, enzyme_out;

template <typename RT, typename... T>
RT __enzyme_autodiff(void*, T...);
 
// Euclidean norm of vector (x, y)
double norm(double x, double y) {
    return std::sqrt(x*x + y*y);
}

int main() {
    // norm(3.0, 4.0) = 5.0
    double x = 3.0;
    double y = 4.0;

    // We store the two enzyme_out quantities in a struct
    dnorm_output output = __enzyme_autodiff<dnorm_output>((void*) norm, 
            enzyme_out, x,
            enzyme_out, y);
    
    // Expect d(norm)/dx = x/norm(x, y)
    std::cout << "x = " << x << "; "
              << "y = " << y << "\n";
    std::cout << "dx = " << output.dx << "; "
              << "dy = " << output.dy << "\n";

    // Expect the derivative at (0.0, 0.0) to be undefined
    // In practice, Enzyme returns d_x = 0.0
    x = 0.0;
    y = 0.0;

    output = __enzyme_autodiff<dnorm_output>((void*) norm, 
            enzyme_out, x,
            enzyme_out, y);
   
    std::cout << "x = " << x << "; "
              << "y = " << y << "\n";
    std::cout << "dx = " << output.dx << "; "
              << "dy = " << output.dy << "\n";

    return 0;
}
