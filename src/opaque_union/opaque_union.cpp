#include <iostream>

template <typename RT, typename... T>
RT __enzyme_autodiff(void*, T...);

union RealBits {
    double real;
    unsigned long long bits;
};

RealBits globalDivideCache;

double divide(double x, double y) {
    double tmp = x / y;

    // zero out cache bits
    globalDivideCache.bits = 0;
    globalDivideCache.real = tmp;

    return tmp;
}

int main() {
    double x = 3.0;
    double y = 5.0;

    double ddiv = __enzyme_autodiff<double>((void*)divide, enzyme_out, x, enzyme_const, y);

    return 0;
}

