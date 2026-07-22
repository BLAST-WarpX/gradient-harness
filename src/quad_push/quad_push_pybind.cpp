#include <pybind11/pybind11.h>

#include "quad_push.h"

namespace py = pybind11;

PYBIND11_MODULE(quad_push, m, py::mod_gil_not_used()) {
    m.doc() = "quad_push.cpp Python bindings";

    m.def("quad_push", &quad_push, "quad_push");
    m.def("dsquare", &dquad_push_dk, "Derivative of quad_push w.r.t k (quadrupole strength)");
}
