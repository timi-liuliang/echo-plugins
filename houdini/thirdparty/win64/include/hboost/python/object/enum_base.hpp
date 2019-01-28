// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_ENUM_BASE_DWA200298_HPP
# define HBOOST_ENUM_BASE_DWA200298_HPP

# include <hboost/python/object_core.hpp>
# include <hboost/python/type_id.hpp>
# include <hboost/python/converter/to_python_function_type.hpp>
# include <hboost/python/converter/convertible_function.hpp>
# include <hboost/python/converter/constructor_function.hpp>

namespace hboost { namespace python { namespace objects { 

struct HBOOST_PYTHON_DECL enum_base : python::api::object
{
 protected:
    enum_base(
        char const* name
        , converter::to_python_function_t
        , converter::convertible_function
        , converter::constructor_function
        , type_info
        , const char *doc = 0
        );

    void add_value(char const* name, long value);
    void export_values();
    
    static PyObject* to_python(PyTypeObject* type, long x);
};

}}} // namespace hboost::python::object

#endif // HBOOST_ENUM_BASE_DWA200298_HPP
