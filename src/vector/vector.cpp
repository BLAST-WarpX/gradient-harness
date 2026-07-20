#include <vector>
#include <iostream>

using namespace std;

extern int enzyme_const, enzyme_dup, enzyme_out;

template <typename RT, typename... T>
RT __enzyme_autodiff(void*, T...);

double test_func(double x) {
    vector<int> v = {0, 1};

    return x;
}

int main() {
    double x = 3.0;
    double dx = __enzyme_autodiff<double>((void*)test_func, enzyme_out, x);

    cout << dx << "\n";
    
    return 0;
}
