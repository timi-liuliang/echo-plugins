// Copyright David Abrahams 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef HBOOST_DEFAULT_CALL_POLICIES_DWA2002131_HPP
# define HBOOST_DEFAULT_CALL_POLICIES_DWA2002131_HPP

# include <hboost/python/detail/prefix.hpp>
# include <hboost/mpl/if.hpp>
# include <hboost/python/to_python_value.hpp>
# include <hboost/python/detail/value_arg.hpp>
# include <hboost/type_traits/transform_traits.hpp>
# include <hboost/type_traits/is_pointer.hpp>
# include <hboost/type_traits/is_reference.hpp>
# include <hboost/mpl/or.hpp>
# include <hboost/mpl/front.hpp>

namespace hboost { namespace python { 

template <class T> struct to_python_value;

namespace detail
{
// for "readable" error messages
  template <class T> struct specify_a_return_value_policy_to_wrap_functions_returning
# if defined(__GNUC__) || defined(__EDG__)
  {}
# endif 
  ;
}

struct default_result_converter;

struct default_call_policies
{
    // Ownership of this argument tuple will ultimately be adopted by
    // the caller.
    template <class ArgumentPackage>
    static bool precall(ArgumentPackage const&)
    {
        return true;
    }

    // Pass the result through
    template <class ArgumentPackage>
    static PyObject* postcall(ArgumentPackage const&, PyObject* result)
    {
        return result;
    }

    typedef default_result_converter result_converter;
    typedef PyObject* argument_package;

    template <class Sig> 
    struct extract_return_type : mpl::front<Sig>
    {
    };

};

struct default_result_converter
{
    template <class R>
    struct apply
    {
        typedef typename mpl::if_<
            mpl::or_<is_pointer<R>, is_reference<R> >
          , detail::specify_a_return_value_policy_to_wrap_functions_returning<R>
          , hboost::python::to_python_value<
                typename detail::value_arg<R>::type
            >
        >::type type;
    };
};

// Exceptions for c strings an PyObject*s
template <>
struct default_result_converter::apply<char const*>
{
    typedef hboost::python::to_python_value<char const*const&> type;
};

template <>
struct default_result_converter::apply<PyObject*>
{
    typedef hboost::python::to_python_value<PyObject*const&> type;
};

}} // namespace hboost::python

#endif // HBOOST_DEFAULT_CALL_POLICIES_DWA2002131_HPP
