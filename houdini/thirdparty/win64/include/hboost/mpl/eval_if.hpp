
#ifndef HBOOST_MPL_EVAL_IF_HPP_INCLUDED
#define HBOOST_MPL_EVAL_IF_HPP_INCLUDED

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

#include <hboost/mpl/if.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>
#include <hboost/mpl/aux_/config/msvc.hpp>
#include <hboost/mpl/aux_/config/gcc.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

namespace hboost { namespace mpl {

template<
      typename HBOOST_MPL_AUX_NA_PARAM(C)
    , typename HBOOST_MPL_AUX_NA_PARAM(F1)
    , typename HBOOST_MPL_AUX_NA_PARAM(F2)
    >
struct eval_if
#if HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300) \
     || ( HBOOST_WORKAROUND(HBOOST_MPL_CFG_GCC, >= 0x0300) \
        && HBOOST_WORKAROUND(HBOOST_MPL_CFG_GCC, HBOOST_TESTED_AT(0x0304)) \
        )
{
    typedef typename if_<C,F1,F2>::type f_;
    typedef typename f_::type type;
#else
    : if_<C,F1,F2>::type
{
#endif
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(3,eval_if,(C,F1,F2))
};

// (almost) copy & paste in order to save one more
// recursively nested template instantiation to user
template<
      bool C
    , typename F1
    , typename F2
    >
struct eval_if_c
#if HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300) \
     || ( HBOOST_WORKAROUND(HBOOST_MPL_CFG_GCC, >= 0x0300) \
        && HBOOST_WORKAROUND(HBOOST_MPL_CFG_GCC, HBOOST_TESTED_AT(0x0304)) \
        )
{
    typedef typename if_c<C,F1,F2>::type f_;
    typedef typename f_::type type;
#else
    : if_c<C,F1,F2>::type
{
#endif
};

HBOOST_MPL_AUX_NA_SPEC(3, eval_if)

}}

#endif // HBOOST_MPL_EVAL_IF_HPP_INCLUDED
