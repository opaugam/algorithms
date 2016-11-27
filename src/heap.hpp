#ifndef HEAP_HPP
#define HEAP_HPP

#include <pybind11/pybind11.h>

#include <list>
#include <vector>
#include <stdexcept>
#include <iostream>
using namespace std;

//
// - simple binary heap backed up by a STL vector
//
struct binary_heap
{
    binary_heap()
    {
    }

    binary_heap(const pybind11::list &handles)
    {
        for(const auto &hdl : handles) _add(hdl);
    }

    ~binary_heap()
    {
    }

    void push(const pybind11::handle &hdl)
    {
        _add(hdl);
    }

    pybind11::object pop() throw(logic_error)
    {
        if(raw.empty()) throw new logic_error("heap is empty");
        auto top = raw[0];
        swap(raw[0], raw.back());
        raw.pop_back();

        //
        // - use indices starting at 1
        // - left/right children for level n are 2 * n and 2 * n + 1
        //
        for(size_t i = 1;;)
        {
            auto left  = 2 * i;
            auto right = 2 * i + 1;

            if(left <= raw.size() && _cmp(raw[i - 1], raw[left - 1]) > 0)
            {
                if(right <= raw.size() && _cmp(raw[left - 1], raw[right - 1]) > 0)
                {
                    swap(raw[i - 1], raw[right - 1]);
                    i = right;
                }
                else
                {
                    swap(raw[i - 1], raw[left - 1]);
                    i = left;
                }

            }
            else if(right <= raw.size() && _cmp(raw[i - 1], raw[right - 1]) > 0)
            {
                if(left <= raw.size() && _cmp(raw[right - 1], raw[left - 1]) > 0)
                {
                    swap(raw[i - 1], raw[left - 1]);
                    i = left;
                }
                else
                {
                    swap(raw[i - 1], raw[right - 1]);
                    i = right;
                }
            }
            else break;
        }

        return top;
    }

    void _add(const pybind11::handle &hdl)
    {
        //
        // - add the to the tail
        // - enforce the heap property and propagate upward
        //
        raw.emplace_back(hdl, true);
        size_t i = raw.size();
        while(i > 1)
        {
            size_t up = i / 2;
            if(_cmp(raw[i - 1], raw[up - 1]) < 0) swap(raw[i - 1], raw[up - 1]);
            i = up;
        }
    }

    int _cmp(const pybind11::object &l, const pybind11::object &r) const throw(runtime_error)
    {
        //
        // - use the pyobject comparator to enforce global order on our heap
        //
        int out = 0;
        if(PyObject_Cmp(l.ptr(), r.ptr(), &out) < 0) throw new runtime_error("unable to compare python objects");
        return out;
    }

    inline size_t size() const
    {
        return raw.size();
    }

    vector<pybind11::object>    raw;
    size_t                      swaps;

    static void bind(pybind11::module m)
    {
        pybind11::class_<binary_heap>(m, "binary_heap")
            .def(pybind11::init<>())
            .def(pybind11::init<const pybind11::list &>())
            .def("pop",         &binary_heap::pop)
            .def("push",        &binary_heap::push)
            .def("__len__",     &binary_heap::size);
    }
};

#endif