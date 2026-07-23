#include <iostream>

#include "quad_push.h"

int main() {
    double x = 0.0;
    double dx = 0.0;
    double y = 0.0;
    double dy = 0.0;
    double t = 0.0;
    double dt = 0.0;
    double px = 1.0;
    double dpx = 0.0;
    double py = 1.0;
    double dpy = 0.0;
    double pt = 1.0;
    double k = 1.0;
    double slice_ds = 1.0;
    double pt_ref = 1.0;

    // Stores Jacobian d[x, y, t, px, py]/dk in [dx, dy, dt, dpx, dpy]
    dquad_push_dk(x, dx, y, dy, t, dt, px, dpx, py, dpy, pt, k, slice_ds, pt_ref);

    // Confirm dquad_push_dk doesn't change input variables
    std::cout << "x = " << x << "; " 
        << "y = " << y << "; "
        << "t = " << t << "; "
        << "px = " << px << "; "
        << "py = " << py << "; "
        << "pt = " << pt << "; "
        << "k = " << k << "; "
        << "slice_ds = " << slice_ds << "; "
        << "pt_ref = " << pt_ref << "\n";

    std::cout << "dx = " << dx << "; "
        << "dy = " << dy << "; "
        << "dt = " << dt << "; "
        << "dpx = " << dpx << "; "
        << "dpy = " << dpy << "\n";

    return 0;
}

