//  (C) Copyright 2009-2011 Frederic Bron.
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_HAS_MULTIPLIES_ASSIGN_HPP_INCLUDED
#define HBOOST_TT_HAS_MULTIPLIES_ASSIGN_HPP_INCLUDED

#define HBOOST_TT_TRAIT_NAME has_multiplies_assign
#define HBOOST_TT_TRAIT_OP *=
#define HBOOST_TT_FORBIDDEN_IF\
   (\
      /* Lhs==fundamental and Lhs==const and Rhs==fundamental */\
      (\
         ::hboost::is_fundamental< Lhs_nocv >::value && \
         ::hboost::is_const< Lhs_noref >::value && \
         ::hboost::is_fundamental< Rhs_nocv >::value\
      ) || \
      /* Lhs==pointer and (Rhs==fundamental or Rhs==pointer) */\
      (\
         ::hboost::is_pointer< Lhs_noref >::value && \
         ( \
            ::hboost::is_fundamental< Rhs_nocv >::value || \
            ::hboost::is_pointer< Rhs_noref >::value\
         )\
      )||\
      /* Rhs==pointer and (Lhs==fundamental or Lhs==pointer) */\
      (\
         ::hboost::is_pointer< Rhs_noref >::value && \
         ( \
            ::hboost::is_fundamental< Lhs_nocv >::value || \
            ::hboost::is_pointer< Lhs_noref >::value\
          )\
      )\
      )


#include <hboost/type_traits/detail/has_binary_operator.hpp>

#undef HBOOST_TT_TRAIT_NAME
#undef HBOOST_TT_TRAIT_OP
#undef HBOOST_TT_FORBIDDEN_IF

#endif
