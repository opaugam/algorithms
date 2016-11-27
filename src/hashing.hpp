#ifndef HASHING_HPP
#define HASHING_HPP

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

struct hashes
{
    static long simple_xor(const string &key)
    {
        unsigned long H = 0;
        unsigned char *p = (unsigned char *)key.c_str();
        for(; *p != '\0'; ++p)
        {
            H += *p;
        }
        return H;
    }

    static long rotating_xor(const string &key)
    {
        unsigned long H = 0;
        unsigned char *p = (unsigned char *)key.c_str();
        for(; *p != '\0'; ++p)
        {
            H = (H << 4) ^ (H >> 28) ^ *p;
        }
        return H;
    }

    static long bernstein(const string &key)
    {
        unsigned long H = 0;
        unsigned char *p = (unsigned char *)key.c_str();
        for(; *p != '\0'; ++p)
        {
            H = 33 * H + *p;
        }
        return H;
    }

    static long shift_add_xor(const string &key)
    {
        unsigned long H = 0;
        unsigned char *p = (unsigned char *)key.c_str();
        for(; *p != '\0'; ++p)
        {
            H ^= (H << 5) + (H >> 2) + *p;
        }
        return H;
    }

    static long fnv(const string &key)
    {
        unsigned long H = 2166136261;
        unsigned char *p = (unsigned char *)key.c_str();
        for(; *p != '\0'; ++p)
        {
            H = (H * 16777619) ^ *p;
        }
        return H;
    }

    static long jenkins(const string &key)
    {
        unsigned long H = 2166136261;
        unsigned char *p = (unsigned char *)key.c_str();
        for(; *p != '\0'; ++p)
        {
            H += *p;
            H += (H << 10);
            H ^= (H >> 6);
        }

        H += (H << 3);
        H ^= (H >> 11);
        H += (H << 15);

        return H;
    }

    static void bind(pybind11::module m)
    {
        m.def("simple_xor",     &simple_xor, "");
        m.def("rotating_xor",   &rotating_xor, "");
        m.def("bernstein",      &bernstein, "");
        m.def("shift_add_xor",  &shift_add_xor, "");
        m.def("fnv",            &fnv, "");
        m.def("jenkins",        &jenkins, "");
    }
};

struct hash_table
{
    hash_table(size_t size, const pybind11::list &handles) : slots(size), collisions(0), keys(0)
    {
        assert(size > 0);
        for(const auto &key : handles) _hash(key);
    }

    ~hash_table()
    {
    }

    void _hash(const pybind11::handle &key)
    {
        long H = PyObject_Hash(key.ptr());
        if(slots[H % slots.size()]++ > 0) ++collisions;
        ++keys;
    }

    inline float efficiency() const
    {
        return 100 * (1 - (float)collisions/keys);
    }

    vector<size_t> slots;
    size_t collisions;
    size_t keys;

    static void bind(pybind11::module m)
    {
        pybind11::class_<hash_table>(m, "hash_table")
            .def(pybind11::init<size_t, const pybind11::list &>())
            .def("efficiency",      &hash_table::efficiency)
            .def_readonly("slots",  &hash_table::slots);
    }
};

#endif