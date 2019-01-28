// -- core.hpp -- Boost Lambda Library -------------------------------------
//
// Copyright (C) 2000 Gary Powell (powellg@amazon.com)
// Copyright (C) 1999, 2000 Jaakko Jarvi (jaakko.jarvi@cs.utu.fi)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// 
// Includes the core of LL, without any real features for client:
// 
// tuples, lambda functors, return type deduction templates,
// argument substitution mechanism (select functions)
// 
// Some functionality comes as well:
// Assignment and subscript operators, as well as function
// call operator for placeholder variables.
// -------------------------------------------------------------------------

#ifndef HBOOST_LAMBDA_CORE_HPP
#define HBOOST_LAMBDA_CORE_HPP

#include "hboost/type_traits/transform_traits.hpp"
#include "hboost/type_traits/cv_traits.hpp"

#include "hboost/tuple/tuple.hpp"

// inject some of the tuple names into lambda 
namespace hboost {
namespace lambda {

using ::hboost::tuples::tuple;
using ::hboost::tuples::null_type;

} // lambda
} // boost

#include "hboost/lambda/detail/lambda_config.hpp"
#include "hboost/lambda/detail/lambda_fwd.hpp"

#include "hboost/lambda/detail/arity_code.hpp"
#include "hboost/lambda/detail/actions.hpp"

#include "hboost/lambda/detail/lambda_traits.hpp"

#include "hboost/lambda/detail/function_adaptors.hpp"
#include "hboost/lambda/detail/return_type_traits.hpp"

#include "hboost/lambda/detail/select_functions.hpp"

#include "hboost/lambda/detail/lambda_functor_base.hpp"

#include "hboost/lambda/detail/lambda_functors.hpp"

#include "hboost/lambda/detail/ret.hpp"

namespace hboost {
namespace lambda {

namespace {

  // These are constants types and need to be initialised
  hboost::lambda::placeholder1_type free1 = hboost::lambda::placeholder1_type();
  hboost::lambda::placeholder2_type free2 = hboost::lambda::placeholder2_type();
  hboost::lambda::placeholder3_type free3 = hboost::lambda::placeholder3_type();

  hboost::lambda::placeholder1_type& _1 = free1;
  hboost::lambda::placeholder2_type& _2 = free2;
  hboost::lambda::placeholder3_type& _3 = free3;
  // _1, _2, ... naming scheme by Peter Dimov
} // unnamed
   
} // lambda
} // boost
   
   
#endif //HBOOST_LAMBDA_CORE_HPP
