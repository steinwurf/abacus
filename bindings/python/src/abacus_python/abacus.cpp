// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <pybind11/pybind11.h>

#include <sstream>
#include <string>

#include <abacus/version.hpp>

#include "version.hpp"

namespace abacus_python
{
inline namespace STEINWURF_ABACUS_PYTHON_VERSION
{
PYBIND11_MODULE(abacus, m)
{
    pybind11::options options;
    options.disable_function_signatures();

    std::stringstream ss;
    ss << "Abacus python version: " << abacus_python::version();

    m.attr("__version__") = ss.str();
    m.attr("__license__") = "BSD 3 Clause";
    m.attr("__copyright__") = "Steinwurf ApS";
}
}
}
