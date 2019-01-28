// Boost.Function library - Typeof support
//  Copyright (C) Douglas Gregor 2008
//
//  Use, modification and distribution is subject to the Boost
//  Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org
#ifndef HBOOST_FUNCTION_TYPEOF_HPP
#define HBOOST_FUNCTION_TYPEOF_HPP
#include <hboost/function/function_fwd.hpp>
#include <hboost/typeof/typeof.hpp>

#include HBOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

HBOOST_TYPEOF_REGISTER_TYPE(hboost::bad_function_call)

#if !defined(HBOOST_FUNCTION_NO_FUNCTION_TYPE_SYNTAX)
HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::function, (typename))
#endif

HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::function0, (typename))
HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::function1, (typename)(typename))
HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::function2, (typename)(typename)(typename))
HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::function3, 
  (typename)(typename)(typename)(typename))
HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::function4, 
  (typename)(typename)(typename)(typename)(typename))
HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::function5, 
  (typename)(typename)(typename)(typename)(typename)(typename))
HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::function6, 
  (typename)(typename)(typename)(typename)(typename)(typename)(typename))
HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::function7, 
  (typename)(typename)(typename)(typename)(typename)(typename)(typename)
  (typename))
HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::function8, 
  (typename)(typename)(typename)(typename)(typename)(typename)(typename)
  (typename)(typename))
HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::function9, 
  (typename)(typename)(typename)(typename)(typename)(typename)(typename)
  (typename)(typename)(typename))
HBOOST_TYPEOF_REGISTER_TEMPLATE(hboost::function10, 
  (typename)(typename)(typename)(typename)(typename)(typename)(typename)
  (typename)(typename)(typename)(typename))
#endif
