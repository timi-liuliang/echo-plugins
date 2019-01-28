
// NO INCLUDE GUARDS, THE HEADER IS INTENDED FOR MULTIPLE INCLUSION

#if defined(HBOOST_PP_IS_ITERATING)

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/preprocessor/repetition/enum_params.hpp>
#include <hboost/preprocessor/repetition/enum_trailing_params.hpp>
#include <hboost/preprocessor/dec.hpp>
#include <hboost/preprocessor/cat.hpp>

#define i_ HBOOST_PP_FRAME_ITERATION(1)

#   define AUX778076_SET_C_TAIL(set, i_, T, C) \
    HBOOST_PP_CAT(HBOOST_PP_CAT(set,i_),_c)< \
          T HBOOST_PP_ENUM_TRAILING_PARAMS(i_, C) \
        > \
    /**/

template<
      typename T
    , HBOOST_PP_ENUM_PARAMS(i_, T C)
    >
struct HBOOST_PP_CAT(HBOOST_PP_CAT(set,i_),_c)
    : s_item<
          integral_c<T,HBOOST_PP_CAT(C,HBOOST_PP_DEC(i_))>
        , AUX778076_SET_C_TAIL(set,HBOOST_PP_DEC(i_), T, C)
        >
{
    typedef HBOOST_PP_CAT(HBOOST_PP_CAT(set,i_),_c) type;
};

#   undef AUX778076_SET_C_TAIL

#undef i_

#endif // HBOOST_PP_IS_ITERATING
