//  (C) Copyright 2009-2011 Frederic Bron.
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_HAS_MINUS_HPP_INCLUDED
#define HBOOST_TT_HAS_MINUS_HPP_INCLUDED

#define HBOOST_TT_TRAIT_NAME has_minus
#define HBOOST_TT_TRAIT_OP -
#define HBOOST_TT_FORBIDDEN_IF\
   (\
      /* Lhs==pointer and Rhs==fundamental and Rhs!=integral */\
      (\
         ::hboost::is_pointer< Lhs_noref >::value && \
         ::hboost::is_fundamental< Rhs_nocv >::value && \
         (!  ::hboost::is_integral< Rhs_noref >::value )\
      ) || \
      /* Lhs==void* and (Rhs==fundamental or Rhs==pointer) */\
      (\
         ::hboost::is_pointer< Lhs_noref >::value && \
         ::hboost::is_void< Lhs_noptr >::value && \
         ( \
            ::hboost::is_fundamental< Rhs_nocv >::value || \
            ::hboost::is_pointer< Rhs_noref >::value\
          )\
      ) || \
      /* Rhs==void* and (Lhs==fundamental or Lhs==pointer) */\
      (\
         ::hboost::is_pointer< Rhs_noref >::value && \
         ::hboost::is_void< Rhs_noptr >::value && \
         (\
            ::hboost::is_fundamental< Lhs_nocv >::value || \
            ::hboost::is_pointer< Lhs_noref >::value\
          )\
      ) ||\
      /* Lhs=fundamental and Rhs=pointer */\
      (\
         ::hboost::is_fundamental< Lhs_nocv >::value && \
         ::hboost::is_pointer< Rhs_noref >::value\
      ) ||\
      /* two different pointers */\
      (\
         ::hboost::is_pointer< Lhs_noref >::value && \
         ::hboost::is_pointer< Rhs_noref >::value && \
         (!  ::hboost::is_same< Lhs_nocv, Rhs_nocv >::value )\
      )\
      )


#include <hboost/type_traits/detail/has_binary_operator.hpp>

#undef HBOOST_TT_TRAIT_NAME
#undef HBOOST_TT_TRAIT_OP
#undef HBOOST_TT_FORBIDDEN_IF

#endif
