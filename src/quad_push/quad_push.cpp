#include <cmath>

extern int enzyme_out, enzyme_const, enzyme_dup, enzyme_dupnoneed;

template <typename RT, typename... T>
RT __enzyme_fwddiff(void*, T...); 

static double quad_push_cos(double const k, double const L);
static double quad_push_sinc(double const k, double const L);
static double quad_push_osin(double const k, double const L);

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

/**
 * This version of quad_push uses versions of the singular function parts
 * which are Taylor expanded near the origin, so Enzyme can pick up on the 
 * derivative correctly.
 */ 
void quad_push_taylor (
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

    double const xout = quad_push_cos(k, slice_ds) * x + quad_push_sinc(k, slice_ds) * px;
    double const pxout = quad_push_osin(k, slice_ds) * x + quad_push_cos(k, slice_ds) * px;
    
    double const yout = quad_push_cos(-k, slice_ds) * y + quad_push_sinc(-k, slice_ds) * py;
    double const pyout = quad_push_osin(-k, slice_ds) * y + quad_push_cos(-k, slice_ds) * py; 

    // longitudinal slip (independent of focusing sign); pt is invariant
    double const tout = t + slice_bg * pt;

    // write back
    x  = xout;
    y  = yout;
    t  = tout;
    px = pxout;
    py = pyout;
}

/**
 * quad_push_cos(k, L) = { cos(sqrt(k)),   k >= 0
 *                       { cosh(sqrt(-k)), k < 0
 * This function is real analytic in k, but the chain rule will give division by zero.
 * We approximate around the origin with a Taylor expansion.
 *
 * Around k = 0:
 * quad_push_cos(k, L) = 1 - (1/2!) L^2 * k + (1/4!) L^4 * k^2 - (1/6!) L^6 * k^3 + ...
 */ 
static double quad_push_cos(double const k, double const L) {
    double const u = L*L*k;
    if (std::fabs(u) < 1.0e0) 
        return 1.0 - u/2.0; //+ u*u/24.0 - u*u*u/720.0;
    double const w = L * std::sqrt(std::fabs(k));
    return k > 0.0 ? std::cos(w) : std::cosh(w);
}

/**
 * quad_push_sinc(k, L) = { sin(L*sqrt(k)) / sqrt(k),    k > 0
 *                        { L,                           k = 0
 *                        { sinh(L*sqrt(-k)) / sqrt(-k), k < 0
 * This function is real analytic in k, but the piecewise condition at 0 will
 * give zero gradient through autodiff.
 * To avoid seeing zero derivative there, we use a Taylor expansion.
 *
 * Around k = 0:
 * quad_push_sinc(k, L) = L - (1/3!) L * u + (1/5!) L^2 * u^2 - (1/7!) L^3 * u^3 + ... 
 */
static double quad_push_sinc(double const k, double const L) {
    double const u = L*L*k;
    if (std::fabs(u) < 1.0e0)
        return L * (1.0 - u/6.0); // + u*u/120.0 - u*u*u/5040.0);
    double const w = L * std::sqrt(std::fabs(k));
    return k > 0.0 ? std::sin(w)/std::sqrt(k) : std::sinh(w)/std::sqrt(-k);
}

/**
 * quad_push_osin(k, L) = { -sqrt(k)*sin(L*sqrt(k)),   k >= 0
 *                        { sqrt(-k)*sinh(L*sqrt(-k)), k < 0
 * This function is real analytic in k, but the chain rule will give division by zero.
 * We approximate around the origin with a Taylor expansion.
 *
 * Around k = 0:
 * quad_push_osin(k, L) = -L*k + (1/3!) L^3 * k^2 - (1/5!) L^5 * k^3 + (1/7!) L^7 * k^4 + ...
 */ 
static double quad_push_osin(double const k, double const L) {
    double const u = L*L*k;
    if (std::fabs(u) < 1.0e0)
        return -L * k * (1.0); // - u/6.0 + u*u/120.0 - u*u*u/5040.0);
    double const omega = std::sqrt(std::fabs(k));
    return k >= 0.0 ? -omega*std::sin(L*omega) : omega*std::sinh(L*omega);
}

void dquad_push_dk_enzyme (
    double const x, double & dx, double const y, double & dy, double const t, double & dt,
    double const px, double & dpx, double const py, double & dpy, double const pt,
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

void dquad_push_taylor_dk_enzyme (
    double const x, double & dx, double const y, double & dy, double const t, double & dt,
    double const px, double & dpx, double const py, double & dpy, double const pt,
    double const k,
    double const slice_ds,
    double const pt_ref
)
{
    double dk = 1.0;
    __enzyme_fwddiff<void>((void*)quad_push_taylor,
            enzyme_dupnoneed, &x, &dx, enzyme_dupnoneed, &y, &dy, enzyme_dupnoneed, &t, &dt,
            enzyme_dupnoneed, &px, &dpx, enzyme_dupnoneed, &py, &dpy, enzyme_const, pt,
            enzyme_dup, k, dk, enzyme_const, slice_ds, enzyme_const, pt_ref);
}


void dquad_push_dk_analytic(
    double const x, double & dx, double const y, double & dy, double const t, double & dt,
    double const px, double & dpx, double const py, double & dpy, double const pt,
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
