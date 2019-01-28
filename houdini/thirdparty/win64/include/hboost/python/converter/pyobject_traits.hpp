// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_PYOBJECT_TRAITS_DWA2002720_HPP
# define HBOOST_PYOBJECT_TRAITS_DWA2002720_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/python/converter/pyobject_type.hpp>

namespace hboost { namespace python { namespace converter { 

template <class> struct pyobject_traits;

template <>
struct pyobject_traits<PyObject>
{
    // All objects are convertible to PyObject
    static bool check(PyObject*) { return true; }
    static PyObject* checked_downcast(PyObject* x) { return x; }
#ifndef HBOOST_PYTHON_NO_PY_SIGNATURES
    static PyTypeObject const* get_pytype() { return 0; }
#endif
};

//
// Specializations
//

# define HBOOST_PYTHON_BUILTIN_OBJECT_TRAITS(T)                  \
    template <> struct pyobject_traits<Py##T##Object>           \
        : pyobject_type<Py##T##Object, &Py##T##_Type> {}

// This is not an exhaustive list; should be expanded.
HBOOST_PYTHON_BUILTIN_OBJECT_TRAITS(Type);
HBOOST_PYTHON_BUILTIN_OBJECT_TRAITS(List);
#if PY_VERSION_HEX < 0x03000000
HBOOST_PYTHON_BUILTIN_OBJECT_TRAITS(Int);
#endif
HBOOST_PYTHON_BUILTIN_OBJECT_TRAITS(Long);
HBOOST_PYTHON_BUILTIN_OBJECT_TRAITS(Dict);
HBOOST_PYTHON_BUILTIN_OBJECT_TRAITS(Tuple);

}}} // namespace hboost::python::converter

#endif // HBOOST_PYOBJECT_TRAITS_DWA2002720_HPP
