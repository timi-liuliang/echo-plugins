
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef HBOOST_FT_MEMBER_FUNCTION_POINTER_HPP_INCLUDED
#define HBOOST_FT_MEMBER_FUNCTION_POINTER_HPP_INCLUDED

#include <hboost/mpl/aux_/lambda_support.hpp>

#include <hboost/function_types/detail/synthesize.hpp>
#include <hboost/function_types/detail/to_sequence.hpp>

namespace hboost 
{ 
  namespace function_types 
  {
    template<typename Types, typename Tag = null_tag> 
    struct member_function_pointer
      : detail::synthesize_mfp< typename detail::to_sequence<Types>::type, Tag >
    { 
      HBOOST_MPL_AUX_LAMBDA_SUPPORT(2,member_function_pointer,(Types,Tag))
    };
  } 
} 

#endif

