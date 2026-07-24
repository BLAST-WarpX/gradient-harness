#include <cmath>

extern int enzyme_out, enzyme_const, enzyme_dup, enzyme_dupnoneed;

template <typename RT, typename... T>
RT __enzyme_fwddiff(void*, T...);

void quad_push (
    double & x, double & y, double & t,
    double & px, double & py, double const pt,
    double const k,
    double const slice_ds,
    double const pt_ref
)
{
    // beta*gamma^2 of the reference particle and the longitudinal slip factor
    double const betgam2  = pt_ref * pt_ref - 1.0;
    double const slice_bg = slice_ds / betgam2;

    // phase advance per unit length in s (rad/m)
    double const omega = std::sqrt(std::abs(k));

    // cache the per-slice trig / hyperbolic functions
    double const sin_o  = std::sin (omega * slice_ds);
    double const cos_o  = std::cos (omega * slice_ds);
    double const sinh_o = std::sinh(omega * slice_ds);
    double const cosh_o = std::cosh(omega * slice_ds);

    // start from the incoming coordinates
    double xout  = x;
    double yout  = y;
    double pxout = px;
    double pyout = py;

    if (k > 0.0) {
        // focusing in x, defocusing in y
        xout  =  cos_o * x + sin_o / omega * px;
        pxout = -omega * sin_o * x + cos_o * px;

        yout  =  cosh_o * y + sinh_o / omega * py;
        pyout =  omega * sinh_o * y + cosh_o * py;
    } else if (k < 0.0) {
        // defocusing in x, focusing in y
        xout  =  cosh_o * x + sinh_o / omega * px;
        pxout =  omega * sinh_o * x + cosh_o * px;

        yout  =  cos_o * y + sin_o / omega * py;
        pyout = -omega * sin_o * y + cos_o * py;
    } else {
        // zero strength = drift
        xout = x + slice_ds * px;
        yout = y + slice_ds * py;
        pxout = px;
        pyout = py;
    }

    // longitudinal slip (independent of focusing sign); pt is invariant
    double const tout = t + slice_bg * pt;

    // write back
    x  = xout;
    y  = yout;
    t  = tout;
    px = pxout;
    py = pyout;
}

void quad_push_dummy (
    double & x, double & y, double & t,
    double & px, double & py, double const pt,
    double const k,
    double const slice_ds,
    double const pt_ref
)
{
    x = 2*k;
    y = 3*k;
    t = 4*k;
    px = 5*k;
    py = 6*k;
}

void dquad_push_dk_enzyme (
    double x, double & dx, double y, double & dy, double t, double & dt,
    double px, double & dpx, double py, double & dpy, double const pt,
    double const k,
    double const slice_ds,
    double const pt_ref
)
{
    double dk = 1.0;
    __enzyme_fwddiff<void>((void*)quad_push,
            enzyme_dupnoneed, &x, &dx, enzyme_dupnoneed, &y, &dy, enzyme_dupnoneed, &t, &dt,
            enzyme_dupnoneed, &px, &dpx, enzyme_dupnoneed, &py, &dpy, enzyme_const, pt,
            enzyme_dup, k, dk, enzyme_const, slice_ds, enzyme_const, pt_ref);
}

void dquad_push_dk_analytic(
    double x, double & dx, double y, double & dy, double t, double & dt,
    double px, double & dpx, double py, double & dpy, double const pt,
    double const k,
    double const slice_ds,
    double const pt_ref
)
{
    if (k == 0.0) {
        // derivative undefined at k = 0, return zero derivative 
        dx = 0.0;
        dy = 0.0;
        dt = 0.0;
        dpx = 0.0;
        dpy = 0.0;
        return;
    }

    signed char sgnk = 1;
    if (k < 0.0) {
        sgnk = -1;
    }

    // beta*gamma^2 of the reference particle and the longitudinal slip factor
    double const betgam2  = pt_ref * pt_ref - 1.0;
    double const slice_bg = slice_ds / betgam2;

    // phase advance per unit length in s (rad/m)
    double const omega = std::sqrt(std::abs(k));
    double const domega = sgnk / (2 * omega);  

    // cache the per-slice trig / hyperbolic functions
    double const sin_o  = std::sin(omega * slice_ds);
    double const dsin_o = slice_ds * std::cos(omega * slice_ds) * domega;
    
    double const cos_o  = std::cos(omega * slice_ds);
    double const dcos_o  = -slice_ds * std::sin(omega * slice_ds) * domega;
    
    double const sinh_o = std::sinh(omega * slice_ds); 
    double const dsinh_o = slice_ds * std::cosh(omega * slice_ds) * domega;
    
    double const cosh_o = std::cosh(omega * slice_ds);
    double const dcosh_o = slice_ds * std::sinh(omega * slice_ds) * domega;
   
    // t not a function of k 
    // tout = t + slice_bg * pt;
    dt = 0.0;

    const double omega_sq = omega * omega;

    if (k > 0.0) {
        // focusing in x, defocusing in y
        // xout  =  cos_o * x + sin_o / omega * px;
        dx =  dcos_o * x + (dsin_o * omega - sin_o *domega) / omega_sq * px;
        
        // pxout = -omega * sin_o * x + cos_o * px;
        dpx = -(domega * sin_o + omega* dsin_o) * x + dcos_o * px;

        // yout  =  cosh_o * y + sinh_o / omega * py;
        dy = dcosh_o * y + (dsinh_o * omega - sinh_o * domega) / omega_sq * py;

        // pyout =  omega * sinh_o * y + cosh_o * py;
        dpy = (domega * sinh_o + omega * dsinh_o) * y + dcosh_o * py;
    } else if (k < 0.0) {
        // defocusing in x, focusing in y
        // xout  =  cosh_o * x + sinh_o / omega * px;
        dx = dcosh_o * x + (dsinh_o * omega - sinh_o * domega) / omega_sq * px;

        // pxout =  omega * sinh_o * x + cosh_o * px;
        dpx = (domega * sinh_o + omega * dsinh_o) * x + dcosh_o * px;

        // yout  =  cos_o * y + sin_o / omega * py;
        dy = dcos_o * y + (dsin_o * omega - dsin_o * domega) / omega_sq * py;

        // pyout = -omega * sin_o * y + cos_o * py;
        dpy = -(domega * sin_o + omega * dsin_o) * y + dcos_o * py;
    }
}
