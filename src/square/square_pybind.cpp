#include <pybind11/pybind11.h>

#include "square.h"

namespace py = pybind11;

PYBIND11_MODULE(square, m, py::mod_gil_not_used()) {
    m.doc() = "square.cpp Python bindings";

    m.def("square", &square, "Returns the square of x");
    m.def("dsquare", &dsquare, "Derivative of square");
}
