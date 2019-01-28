
#ifndef HBOOST_MPL_AUX_PREPROCESSOR_IS_SEQ_HPP_INCLUDED
#define HBOOST_MPL_AUX_PREPROCESSOR_IS_SEQ_HPP_INCLUDED

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

#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/preprocessor/arithmetic/dec.hpp>
#include <hboost/preprocessor/punctuation/paren.hpp>
#include <hboost/preprocessor/cat.hpp>
#include <hboost/preprocessor/config/config.hpp>

// returns 1 if 'seq' is a PP-sequence, 0 otherwise:
//
//   HBOOST_PP_ASSERT( HBOOST_PP_NOT( HBOOST_MPL_PP_IS_SEQ( int ) ) )
//   HBOOST_PP_ASSERT( HBOOST_MPL_PP_IS_SEQ( (int) ) )
//   HBOOST_PP_ASSERT( HBOOST_MPL_PP_IS_SEQ( (1)(2) ) )

#if (HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_BCC()) || defined(_MSC_VER) && defined(__INTEL_COMPILER) && __INTEL_COMPILER == 1010

#   define HBOOST_MPL_PP_IS_SEQ(seq) HBOOST_PP_DEC( HBOOST_PP_SEQ_SIZE( HBOOST_MPL_PP_IS_SEQ_(seq) ) )
#   define HBOOST_MPL_PP_IS_SEQ_(seq) HBOOST_MPL_PP_IS_SEQ_SEQ_( HBOOST_MPL_PP_IS_SEQ_SPLIT_ seq )
#   define HBOOST_MPL_PP_IS_SEQ_SEQ_(x) (x)
#   define HBOOST_MPL_PP_IS_SEQ_SPLIT_(unused) unused)((unused)

#else

#   if HBOOST_PP_CONFIG_FLAGS() & HBOOST_PP_CONFIG_MWCC()
#       define HBOOST_MPL_PP_IS_SEQ(seq) HBOOST_MPL_PP_IS_SEQ_MWCC_((seq))
#       define HBOOST_MPL_PP_IS_SEQ_MWCC_(args) HBOOST_MPL_PP_IS_SEQ_ ## args
#   else
#       define HBOOST_MPL_PP_IS_SEQ(seq) HBOOST_MPL_PP_IS_SEQ_(seq)
#   endif

#   define HBOOST_MPL_PP_IS_SEQ_(seq) HBOOST_PP_CAT(HBOOST_MPL_PP_IS_SEQ_, HBOOST_MPL_PP_IS_SEQ_0 seq HBOOST_PP_RPAREN())
#   define HBOOST_MPL_PP_IS_SEQ_0(x) HBOOST_MPL_PP_IS_SEQ_1(x
#   define HBOOST_MPL_PP_IS_SEQ_ALWAYS_0(unused) 0
#   define HBOOST_MPL_PP_IS_SEQ_HBOOST_MPL_PP_IS_SEQ_0 HBOOST_MPL_PP_IS_SEQ_ALWAYS_0(
#   define HBOOST_MPL_PP_IS_SEQ_HBOOST_MPL_PP_IS_SEQ_1(unused) 1

#endif

#endif // HBOOST_MPL_AUX_PREPROCESSOR_IS_SEQ_HPP_INCLUDED
