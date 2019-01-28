
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef HBOOST_FT_RESULT_TYPE_HPP_INCLUDED
#define HBOOST_FT_RESULT_TYPE_HPP_INCLUDED

#include <hboost/blank.hpp>
#include <hboost/mpl/if.hpp>

#include <hboost/mpl/aux_/lambda_support.hpp>

#include <hboost/mpl/at.hpp>

#include <hboost/function_types/is_callable_builtin.hpp>
#include <hboost/function_types/components.hpp>

namespace hboost 
{ 
  namespace function_types 
  {
    template< typename T > struct result_type;

    namespace detail
    {
      template<typename T> struct result_type_impl
        : mpl::at_c
          < typename function_types::components<T>::types, 0 >
      { };
    }

    template<typename T> struct result_type
      : mpl::if_
        < function_types::is_callable_builtin<T>
        , detail::result_type_impl<T>, hboost::blank
        >::type
    { 
      HBOOST_MPL_AUX_LAMBDA_SUPPORT(1,result_type,(T)) 
    };
  }
}

#endif

