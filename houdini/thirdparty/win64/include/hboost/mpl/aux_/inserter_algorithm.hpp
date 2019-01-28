
#ifndef HBOOST_MPL_AUX_INSERTER_ALGORITHM_HPP_INCLUDED
#define HBOOST_MPL_AUX_INSERTER_ALGORITHM_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2003-2004
// Copyright David Abrahams 2003-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/back_inserter.hpp>
#include <hboost/mpl/front_inserter.hpp>
#include <hboost/mpl/push_back.hpp>
#include <hboost/mpl/push_front.hpp>
#include <hboost/mpl/back_inserter.hpp>
#include <hboost/mpl/front_inserter.hpp>
#include <hboost/mpl/clear.hpp>
#include <hboost/mpl/eval_if.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/aux_/na.hpp>
#include <hboost/mpl/aux_/common_name_wknd.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/preprocessor/params.hpp>
#include <hboost/mpl/aux_/preprocessor/default_params.hpp>
#include <hboost/mpl/aux_/config/ctps.hpp>

#include <hboost/preprocessor/arithmetic/dec.hpp>

#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

#   define HBOOST_MPL_AUX_INSERTER_ALGORITHM_DEF(arity, name) \
HBOOST_MPL_AUX_COMMON_NAME_WKND(name) \
template< \
      HBOOST_MPL_PP_DEFAULT_PARAMS(arity, typename P, na) \
    > \
struct name \
    : aux::name##_impl<HBOOST_MPL_PP_PARAMS(arity, P)> \
{ \
}; \
\
template< \
      HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), typename P) \
    > \
struct name< HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), P),na > \
    : if_< has_push_back< typename clear<P1>::type> \
        , aux::name##_impl< \
              HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), P) \
            , back_inserter< typename clear<P1>::type > \
            > \
        , aux::reverse_##name##_impl< \
              HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), P) \
            , front_inserter< typename clear<P1>::type > \
            > \
        >::type \
{ \
}; \
\
template< \
      HBOOST_MPL_PP_DEFAULT_PARAMS(arity, typename P, na) \
    > \
struct reverse_##name \
    : aux::reverse_##name##_impl<HBOOST_MPL_PP_PARAMS(arity, P)> \
{ \
}; \
\
template< \
      HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), typename P) \
    > \
struct reverse_##name< HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), P),na > \
    : if_< has_push_back<P1> \
        , aux::reverse_##name##_impl< \
              HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), P) \
            , back_inserter< typename clear<P1>::type > \
            > \
        , aux::name##_impl< \
              HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), P) \
            , front_inserter< typename clear<P1>::type > \
            > \
        >::type \
{ \
}; \
HBOOST_MPL_AUX_NA_SPEC(arity, name) \
HBOOST_MPL_AUX_NA_SPEC(arity, reverse_##name) \
/**/

#else

#   define HBOOST_MPL_AUX_INSERTER_ALGORITHM_DEF(arity, name) \
HBOOST_MPL_AUX_COMMON_NAME_WKND(name) \
template< \
      HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), typename P) \
    > \
struct def_##name##_impl \
    : if_< has_push_back<P1> \
        , aux::name##_impl< \
              HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), P) \
            , back_inserter< typename clear<P1>::type > \
            > \
        , aux::reverse_##name##_impl< \
              HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), P) \
            , front_inserter< typename clear<P1>::type > \
            > \
        >::type \
{ \
}; \
\
template< \
      HBOOST_MPL_PP_DEFAULT_PARAMS(arity, typename P, na) \
    > \
struct name \
{ \
    typedef typename eval_if< \
          is_na<HBOOST_PP_CAT(P, arity)> \
        , def_##name##_impl<HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), P)> \
        , aux::name##_impl<HBOOST_MPL_PP_PARAMS(arity, P)> \
        >::type type; \
}; \
\
template< \
      HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), typename P) \
    > \
struct def_reverse_##name##_impl \
    : if_< has_push_back<P1> \
        , aux::reverse_##name##_impl< \
              HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), P) \
            , back_inserter< typename clear<P1>::type > \
            > \
        , aux::name##_impl< \
              HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), P) \
            , front_inserter< typename clear<P1>::type > \
            > \
        >::type \
{ \
}; \
template< \
      HBOOST_MPL_PP_DEFAULT_PARAMS(arity, typename P, na) \
    > \
struct reverse_##name \
{ \
    typedef typename eval_if< \
          is_na<HBOOST_PP_CAT(P, arity)> \
        , def_reverse_##name##_impl<HBOOST_MPL_PP_PARAMS(HBOOST_PP_DEC(arity), P)> \
        , aux::reverse_##name##_impl<HBOOST_MPL_PP_PARAMS(arity, P)> \
        >::type type; \
}; \
HBOOST_MPL_AUX_NA_SPEC(arity, name) \
HBOOST_MPL_AUX_NA_SPEC(arity, reverse_##name) \
/**/

#endif // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif // HBOOST_MPL_AUX_INSERTER_ALGORITHM_HPP_INCLUDED
