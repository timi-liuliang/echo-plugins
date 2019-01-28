
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

#include <hboost/preprocessor/enum_params.hpp>
#include <hboost/preprocessor/enum_shifted_params.hpp>
#include <hboost/preprocessor/comma_if.hpp>
#include <hboost/preprocessor/repeat.hpp>
#include <hboost/preprocessor/dec.hpp>
#include <hboost/preprocessor/cat.hpp>

#define i_ HBOOST_PP_FRAME_ITERATION(1)

#if defined(HBOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES)

#   define AUX778076_VECTOR_TAIL(vector, i_, C) \
    HBOOST_PP_CAT(HBOOST_PP_CAT(vector,i_),_c)<T \
          HBOOST_PP_COMMA_IF(i_) HBOOST_PP_ENUM_PARAMS(i_, C) \
        > \
    /**/

#if i_ > 0
template<
      typename T
    , HBOOST_PP_ENUM_PARAMS(i_, T C)
    >
struct HBOOST_PP_CAT(HBOOST_PP_CAT(vector,i_),_c)
    : v_item<
          integral_c<T,HBOOST_PP_CAT(C,HBOOST_PP_DEC(i_))>
        , AUX778076_VECTOR_TAIL(vector,HBOOST_PP_DEC(i_),C)
        >
{
    typedef HBOOST_PP_CAT(HBOOST_PP_CAT(vector,i_),_c) type;
    typedef T value_type;
};
#endif

#   undef AUX778076_VECTOR_TAIL

#else // "brute force" implementation

#   define AUX778076_VECTOR_C_PARAM_FUNC(unused, i_, param) \
    HBOOST_PP_COMMA_IF(i_) \
    integral_c<T,HBOOST_PP_CAT(param,i_)> \
    /**/

template<
      typename T
    , HBOOST_PP_ENUM_PARAMS(i_, T C)
    >
struct HBOOST_PP_CAT(HBOOST_PP_CAT(vector,i_),_c)
    : HBOOST_PP_CAT(vector,i_)< HBOOST_PP_REPEAT(i_,AUX778076_VECTOR_C_PARAM_FUNC,C) >
{
    typedef HBOOST_PP_CAT(HBOOST_PP_CAT(vector,i_),_c) type;
    typedef T value_type;
};

#   undef AUX778076_VECTOR_C_PARAM_FUNC

#endif // HBOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES

#undef i_

#endif // HBOOST_PP_IS_ITERATING
