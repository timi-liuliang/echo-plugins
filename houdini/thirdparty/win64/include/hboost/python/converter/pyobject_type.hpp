// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_PYOBJECT_TYPE_DWA2002720_HPP
# define HBOOST_PYOBJECT_TYPE_DWA2002720_HPP

# include <hboost/python/cast.hpp>

namespace hboost { namespace python { namespace converter { 

HBOOST_PYTHON_DECL PyObject* checked_downcast_impl(PyObject*, PyTypeObject*);

// Used as a base class for specializations which need to provide
// Python type checking capability.
template <class Object, PyTypeObject* pytype>
struct pyobject_type 
{
    static bool check(PyObject* x)
    {
        return ::PyObject_IsInstance(x, (PyObject*)pytype);
    }

    static Object* checked_downcast(PyObject* x)
    {
        return python::downcast<Object>(
            (checked_downcast_impl)(x, pytype)
            );
    }
#ifndef HBOOST_PYTHON_NO_PY_SIGNATURES
    static PyTypeObject const* get_pytype() { return pytype; }
#endif
};

}}} // namespace hboost::python::converter

#endif // HBOOST_PYOBJECT_TYPE_DWA2002720_HPP
