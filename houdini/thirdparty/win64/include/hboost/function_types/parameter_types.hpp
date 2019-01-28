
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef HBOOST_FT_PARAMETER_TYPES_HPP_INCLUDED
#define HBOOST_FT_PARAMETER_TYPES_HPP_INCLUDED

#include <hboost/blank.hpp>
#include <hboost/mpl/if.hpp>

#include <hboost/mpl/aux_/lambda_support.hpp>

#include <hboost/mpl/pop_front.hpp>

#include <hboost/function_types/is_callable_builtin.hpp>
#include <hboost/function_types/components.hpp>

namespace hboost 
{ 
  namespace function_types 
  {
    using mpl::placeholders::_;
 
    template< typename T, typename ClassTypeTransform = add_reference<_> >
    struct parameter_types;

    namespace detail
    {
      template<typename T, typename ClassTypeTransform> 
      struct parameter_types_impl
        : mpl::pop_front
          < typename function_types::components<T,ClassTypeTransform>::types 
          >::type
      { };
    }

    template<typename T, typename ClassTypeTransform> struct parameter_types
      : mpl::if_
        < function_types::is_callable_builtin<T>
        , detail::parameter_types_impl<T,ClassTypeTransform>, hboost::blank
        >::type
    {
      HBOOST_MPL_AUX_LAMBDA_SUPPORT(2,parameter_types,(T,ClassTypeTransform)) 
    };
  }
}

#endif

