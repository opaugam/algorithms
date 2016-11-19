#include <pybind11/pybind11.h>

#include "splay_tree.hpp"

PYBIND11_PLUGIN(stub) {

    pybind11::module m("stub", "python/cxx11 stub");

    splay_tree::bind(m);

    return m.ptr();
}