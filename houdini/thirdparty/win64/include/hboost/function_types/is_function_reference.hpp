
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef HBOOST_FT_IS_FUNCTION_REFERENCE_HPP_INCLUDED
#define HBOOST_FT_IS_FUNCTION_REFERENCE_HPP_INCLUDED

#include <hboost/mpl/aux_/lambda_support.hpp>

#include <hboost/function_types/components.hpp>

namespace hboost 
{ 
  namespace function_types 
  {
    template< typename T, typename Tag = null_tag > 
    struct is_function_reference
      : function_types::represents
        < function_types::components<T>
        , function_types::tag<Tag ,detail::reference_tag> >
    { 
      HBOOST_MPL_AUX_LAMBDA_SUPPORT(2,is_function_reference,(T,Tag))
    };
  }
} 

#endif

