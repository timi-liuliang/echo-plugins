// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_DECREF_GUARD_DWA20021220_HPP
# define HBOOST_DECREF_GUARD_DWA20021220_HPP

namespace hboost { namespace python { namespace detail { 

struct decref_guard
{
    decref_guard(PyObject* o) : obj(o) {}
    ~decref_guard() { Py_XDECREF(obj); }
    void cancel() { obj = 0; }
 private:
    PyObject* obj;
};

}}} // namespace hboost::python::detail

#endif // HBOOST_DECREF_GUARD_DWA20021220_HPP
