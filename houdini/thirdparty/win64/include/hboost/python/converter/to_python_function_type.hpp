// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_TO_PYTHON_FUNCTION_TYPE_DWA200236_HPP
# define HBOOST_TO_PYTHON_FUNCTION_TYPE_DWA200236_HPP
# include <hboost/python/detail/prefix.hpp>
# include <hboost/static_assert.hpp>

namespace hboost { namespace python { namespace converter { 

// The type of stored function pointers which actually do conversion
// by-value. The void* points to the object to be converted, and
// type-safety is preserved through runtime registration.
typedef PyObject* (*to_python_function_t)(void const*);

}}} // namespace hboost::python::converter

#endif // HBOOST_TO_PYTHON_FUNCTION_TYPE_DWA200236_HPP
