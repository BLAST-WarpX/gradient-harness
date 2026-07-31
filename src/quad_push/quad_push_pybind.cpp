#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

#include "quad_push.h"

namespace py = pybind11;

typedef void (*QuadPushFunc)(
        double&, double&, double&, double&, double&,
        double, double, double, double);

typedef void (*DQuadPushFunc)(
        double, double&, double, double&, double, double&, 
        double, double&, double, double&,
        double, double, double, double);

/**
 * Wrapper for functions with signature of quad_push.
 * pybind11 can't use pass by reference, so a wrapper is required.
 */ 
static std::vector<double> quad_push_func_wrapper(QuadPushFunc func,
        double x, double y, double t,
        double px, double py, double pt,
        double k, double slice_ds, double pt_ref) {
    func(x, y, t, px, py, pt, k, slice_ds, pt_ref);
    return {x, y, t, px, py};
}

/**
 * Wrapper for functions with signature of dquad_push_dk
 */ 
static std::vector<double> dquad_push_dk_func_wrapper(DQuadPushFunc func,
    double x, double y, double t,
    double px, double py, double pt, 
    double k, double slice_ds, double pt_ref
)
{
    double dx, dy, dt, dpx, dpy;
    func(x, dx, y, dy, t, dt, px, dpx, py, dpy, pt, k, slice_ds, pt_ref);
    return {dx, dy, dt, dpx, dpy};
}

static std::vector<double> quad_push_wrapper(
        double x, double y, double t,
        double px, double py, double pt,
        double k, double slice_ds, double pt_ref) {
    return quad_push_func_wrapper(&quad_push,
            x, y, t, px, py, pt, k, slice_ds, pt_ref);
}

static std::vector<double> quad_push_taylor_wrapper(
        double x, double y, double t,
        double px, double py, double pt,
        double k, double slice_ds, double pt_ref) {
    return quad_push_func_wrapper(&quad_push_taylor,
            x, y, t, px, py, pt, k, slice_ds, pt_ref);
}

static std::vector<double> dquad_push_dk_enzyme_wrapper(
    double x, double y, double t,
    double px, double py, double pt, 
    double k, double slice_ds, double pt_ref
)
{
    return dquad_push_dk_func_wrapper(&dquad_push_dk_enzyme,
            x, y, t, px, py, pt, k, slice_ds, pt_ref);
}

static std::vector<double> dquad_push_taylor_dk_enzyme_wrapper(
    double x, double y, double t,
    double px, double py, double pt, 
    double k, double slice_ds, double pt_ref
)
{
    return dquad_push_dk_func_wrapper(&dquad_push_taylor_dk_enzyme,
            x, y, t, px, py, pt, k, slice_ds, pt_ref);
}

static std::vector<double> dquad_push_dk_analytic_wrapper(
    double x, double y, double t,
    double px, double py, double pt, 
    double k, double slice_ds, double pt_ref
)
{
    return dquad_push_dk_func_wrapper(&dquad_push_dk_analytic,
            x, y, t, px, py, pt, k, slice_ds, pt_ref);
}

PYBIND11_MODULE(quad_push, m, py::mod_gil_not_used()) {
    m.doc() = "quad_push.cpp Python bindings";

    m.def("quad_push", &quad_push_wrapper, "quad_push");
    m.def("quad_push_taylor", &quad_push_taylor_wrapper, "quad_push_taylor");
    m.def("dquad_push_dk_enzyme", &dquad_push_dk_enzyme_wrapper, "Enzyme derivative of quad_push w.r.t k (quadrupole strength)");
    m.def("dquad_push_taylor_dk_enzyme", &dquad_push_taylor_dk_enzyme_wrapper, "Enzyme derivative of quad_push_taylor w.r.t k (quadrupole strength)");
    m.def("dquad_push_dk_analytic", &dquad_push_dk_analytic_wrapper, "Analytic derivative of quad_push w.r.t k (quadrupole strength)");
}
