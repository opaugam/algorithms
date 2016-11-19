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

    static void bind(pybind11::module m)
    {
        pybind11::class_<splay_tree>(m, "splay_tree")
            .def(pybind11::init<>());
    }
};

#endif