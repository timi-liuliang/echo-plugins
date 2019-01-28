//  Copyright David Abrahams 2001.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_REGISTRY_DWA20011127_HPP
# define HBOOST_REGISTRY_DWA20011127_HPP
# include <hboost/python/type_id.hpp>
# include <hboost/python/converter/to_python_function_type.hpp>
# include <hboost/python/converter/rvalue_from_python_data.hpp>
# include <hboost/python/converter/constructor_function.hpp>
# include <hboost/python/converter/convertible_function.hpp>

namespace hboost { namespace python { namespace converter {

struct registration;

// This namespace acts as a sort of singleton
namespace registry
{
  // Get the registration corresponding to the type, creating it if necessary
  HBOOST_PYTHON_DECL registration const& lookup(type_info);

  // Get the registration corresponding to the type, creating it if
  // necessary.  Use this first when the type is a shared_ptr.
  HBOOST_PYTHON_DECL registration const& lookup_shared_ptr(type_info);

  // Return a pointer to the corresponding registration, if one exists
  HBOOST_PYTHON_DECL registration const* query(type_info);
  
  HBOOST_PYTHON_DECL void insert(to_python_function_t, type_info, PyTypeObject const* (*to_python_target_type)() = 0);

  // Insert an lvalue from_python converter
  HBOOST_PYTHON_DECL void insert(convertible_function, type_info, PyTypeObject const* (*expected_pytype)() = 0);

  // Insert an rvalue from_python converter
  HBOOST_PYTHON_DECL void insert(
      convertible_function
      , constructor_function
      , type_info
      , PyTypeObject const* (*expected_pytype)()  = 0
      );
  
  // Insert an rvalue from_python converter at the tail of the
  // chain. Used for implicit conversions
  HBOOST_PYTHON_DECL void push_back(
      convertible_function
      , constructor_function
      , type_info
      , PyTypeObject const* (*expected_pytype)()  = 0
      );
}

}}} // namespace hboost::python::converter

#endif // HBOOST_REGISTRY_DWA20011127_HPP
