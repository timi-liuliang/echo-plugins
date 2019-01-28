
// NO INCLUDE GUARDS, THE HEADER IS INTENDED FOR MULTIPLE INCLUSION

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

#if defined(HBOOST_PP_IS_ITERATING)

#include <hboost/preprocessor/enum_params.hpp>
#include <hboost/preprocessor/enum_shifted_params.hpp>
#include <hboost/preprocessor/dec.hpp>
#include <hboost/preprocessor/cat.hpp>

#define i HBOOST_PP_FRAME_ITERATION(1)

#if i == 1

template<
      typename T
    , HBOOST_PP_ENUM_PARAMS(i, T C)
    >
struct list1_c
    : l_item<
          long_<1>
        , integral_c<T,C0>
        , l_end
        >
{
    typedef list1_c type;
    typedef T value_type;
};

#else

#   define MPL_AUX_LIST_C_TAIL(list, i, C) \
    HBOOST_PP_CAT(HBOOST_PP_CAT(list,HBOOST_PP_DEC(i)),_c)<T, \
      HBOOST_PP_ENUM_SHIFTED_PARAMS(i, C) \
    > \
    /**/
    
template<
      typename T
    , HBOOST_PP_ENUM_PARAMS(i, T C)
    >
struct HBOOST_PP_CAT(HBOOST_PP_CAT(list,i),_c)
    : l_item<
          long_<i>
        , integral_c<T,C0>
        , MPL_AUX_LIST_C_TAIL(list,i,C)
        >
{
    typedef HBOOST_PP_CAT(HBOOST_PP_CAT(list,i),_c) type;
    typedef T value_type;
};

#   undef MPL_AUX_LIST_C_TAIL

#endif // i == 1

#undef i

#endif // HBOOST_PP_IS_ITERATING
