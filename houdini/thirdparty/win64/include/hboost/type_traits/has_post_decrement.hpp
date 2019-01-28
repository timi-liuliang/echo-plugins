//  (C) Copyright 2009-2011 Frederic Bron.
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef HBOOST_TT_HAS_POST_DECREMENT_HPP_INCLUDED
#define HBOOST_TT_HAS_POST_DECREMENT_HPP_INCLUDED

#include <hboost/type_traits/is_array.hpp>

#define HBOOST_TT_TRAIT_NAME has_post_decrement
#define HBOOST_TT_TRAIT_OP --
#define HBOOST_TT_FORBIDDEN_IF\
   (\
      /* bool */\
      ::hboost::is_same< bool, Lhs_nocv >::value || \
      /* void* */\
      (\
         ::hboost::is_pointer< Lhs_noref >::value && \
         ::hboost::is_void< Lhs_noptr >::value\
      ) || \
      /* (fundamental or pointer) and const */\
      (\
         ( \
            ::hboost::is_fundamental< Lhs_nocv >::value || \
            ::hboost::is_pointer< Lhs_noref >::value\
         ) && \
         ::hboost::is_const< Lhs_noref >::value\
      )||\
      /* Arrays */ \
      ::hboost::is_array<Lhs_noref>::value\
      )


#include <hboost/type_traits/detail/has_postfix_operator.hpp>

#undef HBOOST_TT_TRAIT_NAME
#undef HBOOST_TT_TRAIT_OP
#undef HBOOST_TT_FORBIDDEN_IF

#endif
