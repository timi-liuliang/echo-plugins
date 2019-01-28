
// NO INCLUDE GUARDS, THE HEADER IS INTENDED FOR MULTIPLE INCLUSION

// Copyright Peter Dimov 2000-2002
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
      HBOOST_PP_ENUM_PARAMS(i, typename T)
    >
struct list1
    : l_item<
          long_<1>
        , T0
        , l_end
        >
{
    typedef list1 type;
};

#else

#   define MPL_AUX_LIST_TAIL(list, i, T) \
    HBOOST_PP_CAT(list,HBOOST_PP_DEC(i))< \
      HBOOST_PP_ENUM_SHIFTED_PARAMS(i, T) \
    > \
    /**/
    
template<
      HBOOST_PP_ENUM_PARAMS(i, typename T)
    >
struct HBOOST_PP_CAT(list,i)
    : l_item<
          long_<i>
        , T0
        , MPL_AUX_LIST_TAIL(list,i,T)
        >
{
    typedef HBOOST_PP_CAT(list,i) type;
};

#   undef MPL_AUX_LIST_TAIL

#endif // i == 1

#undef i

#endif // HBOOST_PP_IS_ITERATING
