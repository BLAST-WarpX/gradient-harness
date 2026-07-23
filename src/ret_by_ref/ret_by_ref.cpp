#include <iostream>

extern int enzyme_out, enzyme_const, enzyme_dup;

template <typename RT, typename... T>
RT __enzyme_autodiff(void*, T...);

template <typename RT, typename... T>
RT __enzyme_fwddiff(void*, T...);

/**
 * A function with two inputs x, y that stores output in reference z
 */ 
void func1(double x, double y, double & z) {
    z = 2*x + 3*y;
}

/**
 * A function with one input u that stores output in references v, w
 */
void func2(double u, double & v, double & w) {
    v = 4*u;
    w = 5*u;
}

int main() {
    double x = 1.0;
    double y = 1.0;
    double z = 0.0;
    double lambda = 1.0; // Must set lambda = 1.0 to get derivates, see below

    // Reverse mode example
    // Returns lambda * (dz/dx)
    double dx = __enzyme_autodiff<double>((void*)func1, 
            enzyme_out, x, enzyme_const, y, enzyme_dup, &z, &lambda);

    std::cout << "Reverse mode:\n";
    std::cout << "func1: dz/dx = " << dx << "\n";
    std::cout << "\n";


    double u = 1.0;
    double v = 0.0;
    double w = 0.0;
    double du = 1.0; // Must set du = 1.0 to get derivates, see below
    double dv = 0.0;
    double dw = 0.0;

    // Forward mode example
    // Sets [dv, dw] = [dv/du, dw/du] * du 
    __enzyme_fwddiff<void>((void*)func2,
            enzyme_dup, u, du, enzyme_dup, &v, &dv, enzyme_dup, &w, &dw);

    std::cout << "Forward mode:\n";
    std::cout << "func2: dv/dv = " << dv << "; dw/du = " << dw << "\n";

    return 0;
}
