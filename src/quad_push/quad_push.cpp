#include <cmath>

extern int enzyme_const, enzyme_dup, enzyme_out;

template <typename RT, typename... T>
RT __enzyme_autodiff(void*, T...);

void quad_push (
    double & x, double & y, double & t,
    double & px, double & py, double const pt,
    double k,
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

    if (k > 0.0)
    {
        // focusing in x, defocusing in y
        xout  =  cos_o * x + sin_o / omega * px;
        pxout = -omega * sin_o * x + cos_o * px;

        yout  =  cosh_o * y + sinh_o / omega * py;
        pyout =  omega * sinh_o * y + cosh_o * py;
    }
    else if (k < 0.0)
    {
        // defocusing in x, focusing in y
        xout  =  cosh_o * x + sinh_o / omega * px;
        pxout =  omega * sinh_o * x + cosh_o * px;

        yout  =  cos_o * y + sin_o / omega * py;
        pyout = -omega * sin_o * y + cos_o * py;
    }
    else
    {
        // zero strength = drift
        xout = x + slice_ds * px;
        yout = y + slice_ds * py;
        // pxout = px;  pyout = py;
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
 * Derivative of quad_push with respect to quadrupole strength k
 */
double dquad_push_dk (
    double * x, double * y, double * t,
    double * px, double * py, double const pt,
    double k,
    double const slice_ds,
    double const pt_ref
)
{
    return __enzyme_autodiff<double>((void*)quad_push,
            enzyme_const, x, enzyme_const, y, enzyme_const, t,
            enzyme_const, px, enzyme_const, py, enzyme_const, pt,
            enzyme_out, k, enzyme_const, slice_ds, enzyme_const, pt_ref);
}

