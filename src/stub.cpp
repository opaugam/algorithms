#include <pybind11/pybind11.h>

#include "hashing.hpp"
#include "heap.hpp"

PYBIND11_PLUGIN(stub) {

    pybind11::module m("stub", "python/cxx11 stub");

    binary_heap::bind(m);
    hash_table::bind(m);
    hashes::bind(m);

    return m.ptr();
}