///////////////////////////////////////////////////////////////////////////////
/// \file local.hpp
/// Contains macros to ease the generation of repetitious code constructs
//
//  Copyright 2008 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_PROTO_LOCAL_MACRO
# error "local iteration target macro is not defined"
#endif

#ifndef HBOOST_PROTO_LOCAL_LIMITS
# define HBOOST_PROTO_LOCAL_LIMITS (1, HBOOST_PROTO_MAX_ARITY)
#endif

#ifndef HBOOST_PROTO_LOCAL_typename_A
# define HBOOST_PROTO_LOCAL_typename_A HBOOST_PROTO_typename_A
#endif

#ifndef HBOOST_PROTO_LOCAL_A
# define HBOOST_PROTO_LOCAL_A HBOOST_PROTO_A_const_ref
#endif

#ifndef HBOOST_PROTO_LOCAL_A_a
# define HBOOST_PROTO_LOCAL_A_a HBOOST_PROTO_A_const_ref_a
#endif

#ifndef HBOOST_PROTO_LOCAL_a
# define HBOOST_PROTO_LOCAL_a HBOOST_PROTO_ref_a
#endif

#define HBOOST_PP_LOCAL_LIMITS HBOOST_PROTO_LOCAL_LIMITS

#define HBOOST_PP_LOCAL_MACRO(N)       \
  HBOOST_PROTO_LOCAL_MACRO(            \
      N                               \
    , HBOOST_PROTO_LOCAL_typename_A    \
    , HBOOST_PROTO_LOCAL_A             \
    , HBOOST_PROTO_LOCAL_A_a           \
    , HBOOST_PROTO_LOCAL_a             \
  )                                   \
  /**/

#include HBOOST_PP_LOCAL_ITERATE()

#undef HBOOST_PROTO_LOCAL_MACRO
#undef HBOOST_PROTO_LOCAL_LIMITS
#undef HBOOST_PROTO_LOCAL_typename_A
#undef HBOOST_PROTO_LOCAL_A
#undef HBOOST_PROTO_LOCAL_A_a
#undef HBOOST_PROTO_LOCAL_a
