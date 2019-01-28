
#ifndef HBOOST_MPL_AUX_PREPROCESSOR_TOKEN_EQUAL_HPP_INCLUDED
#define HBOOST_MPL_AUX_PREPROCESSOR_TOKEN_EQUAL_HPP_INCLUDED

// Copyright Paul Mensonides 2003
// Copyright Aleksey Gurtovoy 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/aux_/preprocessor/is_seq.hpp>

#include <hboost/preprocessor/if.hpp>
#include <hboost/preprocessor/logical/bitand.hpp>
#include <hboost/preprocessor/logical/compl.hpp>
#include <hboost/preprocessor/tuple/eat.hpp>
#include <hboost/preprocessor/cat.hpp>

// compares tokens 'a' and 'b' for equality:
//
//   #define HBOOST_MPL_PP_TOKEN_EQUAL_apple(x) x
//   #define HBOOST_MPL_PP_TOKEN_EQUAL_orange(x) x
//
//   HBOOST_PP_ASSERT( HBOOST_PP_NOT( HBOOST_MPL_PP_TOKEN_EQUAL(apple, abc) ) )
//   HBOOST_PP_ASSERT( HBOOST_PP_NOT( HBOOST_MPL_PP_TOKEN_EQUAL(abc, apple) ) )
//   HBOOST_PP_ASSERT( HBOOST_PP_NOT( HBOOST_MPL_PP_TOKEN_EQUAL(apple, orange) ) )
//   HBOOST_PP_ASSERT( HBOOST_MPL_PP_TOKEN_EQUAL(apple, apple) )
//   HBOOST_PP_ASSERT( HBOOST_MPL_PP_TOKEN_EQUAL(orange, orange) )

#define HBOOST_MPL_PP_TOKEN_EQUAL(a, b) \
    HBOOST_PP_IIF( \
        HBOOST_PP_BITAND( \
              HBOOST_MPL_PP_IS_SEQ( HBOOST_PP_CAT(HBOOST_MPL_PP_TOKEN_EQUAL_, a)((unused)) ) \
            , HBOOST_MPL_PP_IS_SEQ( HBOOST_PP_CAT(HBOOST_MPL_PP_TOKEN_EQUAL_, b)((unused)) ) \
            ) \
        , HBOOST_MPL_PP_TOKEN_EQUAL_I \
        , 0 HBOOST_PP_TUPLE_EAT(2) \
        )(a, b) \
/**/

#define HBOOST_MPL_PP_TOKEN_EQUAL_I(a, b) \
    HBOOST_PP_COMPL(HBOOST_MPL_PP_IS_SEQ( \
        HBOOST_MPL_PP_TOKEN_EQUAL_ ## a( \
            HBOOST_MPL_PP_TOKEN_EQUAL_ ## b \
            )((unused)) \
        )) \
/**/

#endif // HBOOST_MPL_AUX_PREPROCESSOR_TOKEN_EQUAL_HPP_INCLUDED
