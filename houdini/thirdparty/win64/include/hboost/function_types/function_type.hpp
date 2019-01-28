
// (C) Copyright Tobias Schwinger
//
// Use modification and distribution are subject to the boost Software License,
// Version 1.0. (See http://www.boost.org/LICENSE_1_0.txt).

//------------------------------------------------------------------------------

#ifndef HBOOST_FT_FUNCTION_TYPE_HPP_INCLUDED
#define HBOOST_FT_FUNCTION_TYPE_HPP_INCLUDED

#include <hboost/function_types/detail/synthesize.hpp>
#include <hboost/function_types/detail/to_sequence.hpp>

namespace hboost 
{ 
  namespace function_types 
  {
    template<typename Types, typename Tag = null_tag> struct function_type
      : detail::synthesize_func<typename detail::to_sequence<Types>::type, Tag>
    {
      HBOOST_MPL_AUX_LAMBDA_SUPPORT(2,function_type,(Types,Tag))
    };
  }
}

#endif
 
