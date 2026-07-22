#include <iostream>

#include "quad_push.h"

int main() {
    double x = 0.0;
    double y = 0.0;
    double t = 0.0;
    double px = 1.0;
    double py = 1.0;
    double pt = 1.0;
    double k = 1.0;
    double slice_ds = 0.1;
    double pt_ref = 1.0;

    std::cout << dquad_push_dk(&x, &y, &t, &px, &py, pt, k, slice_ds, pt_ref) << "\n";

    return 0;
}

