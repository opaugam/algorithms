#ifndef SPLAY_TREE_HPP
#define SPLAY_TREE_HPP

#include <pybind11/pybind11.h>
#include <iostream>

using namespace std;

struct splay_tree
{
    struct node
    {
    };

    splay_tree()
    {
    }

    ~splay_tree()
    {
    }

    void cmp(pybind11::object &o1, pybind11::object &o2)
    {
        int out = 0;
        auto r = PyObject_Cmp(o1.ptr(),o2.ptr(),&out);
        cout << "---> " << out << endl;
    }

    static void bind(pybind11::module m)
    {
        pybind11::class_<splay_tree>(m, "splay_tree")
            .def(pybind11::init<>())
            .def("cmp", &splay_tree::cmp);
    }
};

#endif