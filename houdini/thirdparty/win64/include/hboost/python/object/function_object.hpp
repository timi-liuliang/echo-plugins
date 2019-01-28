// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_FUNCTION_OBJECT_DWA2002725_HPP
# define HBOOST_FUNCTION_OBJECT_DWA2002725_HPP
# include <hboost/python/detail/prefix.hpp>
# include <hboost/function/function2.hpp>
# include <hboost/python/object_core.hpp>
# include <hboost/python/args_fwd.hpp>
# include <hboost/python/object/py_function.hpp>

namespace hboost { namespace python {

namespace objects
{ 
  HBOOST_PYTHON_DECL api::object function_object(
      py_function const& f
      , python::detail::keyword_range const&);

  HBOOST_PYTHON_DECL api::object function_object(
      py_function const& f
      , python::detail::keyword_range const&);

  HBOOST_PYTHON_DECL api::object function_object(py_function const& f);

  // Add an attribute to the name_space with the given name. If it is
  // a Boost.Python function object
  // (hboost/python/object/function.hpp), and an existing function is
  // already there, add it as an overload.
  HBOOST_PYTHON_DECL void add_to_namespace(
      object const& name_space, char const* name, object const& attribute);

  HBOOST_PYTHON_DECL void add_to_namespace(
      object const& name_space, char const* name, object const& attribute, char const* doc);
}

}} // namespace hboost::python::objects

#endif // HBOOST_FUNCTION_OBJECT_DWA2002725_HPP
