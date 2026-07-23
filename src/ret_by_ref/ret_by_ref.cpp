#include <iostream>

extern int enzyme_out, enzyme_const, enzyme_dup;

template <typename RT, typename... T>
RT __enzyme_autodiff(void*, T...);

struct double2 {
    double x1, x2;
}; 

void func(double x, double y, double & z) {
    z = 2*x + 3*y;
}


int main() {
    double x = 1.0;
    double y = 1.0;
    double z = 0.0;
    double lambda = 0.0;

    double2 mu = __enzyme_autodiff<double2>((void*)func, 
            enzyme_out, x, enzyme_out, y, enzyme_dup, &z, &lambda);

    std::cout << mu.x1 << " "
        << mu.x2 << " "
        << z << " " << lambda << "\n";

    return 0;
}
