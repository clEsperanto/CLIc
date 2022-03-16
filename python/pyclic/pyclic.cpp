// #include <pybind11/stl.h>
// #include <pybind11/functional.h>

#include "pyclic.hpp"

PYBIND11_MODULE(_pyclic, m) {  // define a module. module name = file name = cmake target name 
    init_pygpu(m);
    init_pydata(m);
    init_pytier1(m);
}
