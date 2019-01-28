// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_OBJECT_DWA2002612_HPP
# define HBOOST_OBJECT_DWA2002612_HPP

# include <hboost/python/ssize_t.hpp>
# include <hboost/python/object_core.hpp>
# include <hboost/python/object_attributes.hpp>
# include <hboost/python/object_items.hpp>
# include <hboost/python/object_slices.hpp>
# include <hboost/python/object_operators.hpp>
# include <hboost/python/converter/arg_to_python.hpp>

namespace hboost { namespace python {

    inline ssize_t len(object const& obj)
    {
        ssize_t result = PyObject_Length(obj.ptr());
        if (PyErr_Occurred()) throw_error_already_set();
        return result;
    }

}} // namespace hboost::python

#endif // HBOOST_OBJECT_DWA2002612_HPP
