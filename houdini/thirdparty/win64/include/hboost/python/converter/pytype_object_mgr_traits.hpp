// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_PYTYPE_OBJECT_MANAGER_TRAITS_DWA2002716_HPP
# define HBOOST_PYTYPE_OBJECT_MANAGER_TRAITS_DWA2002716_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/python/detail/raw_pyobject.hpp>
# include <hboost/python/cast.hpp>
# include <hboost/python/converter/pyobject_type.hpp>
# include <hboost/python/errors.hpp>

namespace hboost { namespace python { namespace converter { 

// Provide a forward declaration as a convenience for clients, who all
// need it.
template <class T> struct object_manager_traits;

// Derive specializations of object_manager_traits from this class
// when T is an object manager for a particular Python type hierarchy.
//
template <PyTypeObject* pytype, class T>
struct pytype_object_manager_traits
    : pyobject_type<T, pytype> // provides check()
{
    HBOOST_STATIC_CONSTANT(bool, is_specialized = true);
    static inline python::detail::new_reference adopt(PyObject*);
};

//
// implementations
//
template <PyTypeObject* pytype, class T>
inline python::detail::new_reference pytype_object_manager_traits<pytype,T>::adopt(PyObject* x)
{
    return python::detail::new_reference(python::pytype_check(pytype, x));
}

}}} // namespace hboost::python::converter

#endif // HBOOST_PYTYPE_OBJECT_MANAGER_TRAITS_DWA2002716_HPP
