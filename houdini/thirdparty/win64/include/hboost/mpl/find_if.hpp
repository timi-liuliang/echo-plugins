
#ifndef HBOOST_MPL_FIND_IF_HPP_INCLUDED
#define HBOOST_MPL_FIND_IF_HPP_INCLUDED

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

#include <hboost/mpl/aux_/find_if_pred.hpp>
#include <hboost/mpl/arg.hpp>
#include <hboost/mpl/iter_fold_if.hpp>
#include <hboost/mpl/aux_/common_name_wknd.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>

namespace hboost { namespace mpl {

HBOOST_MPL_AUX_COMMON_NAME_WKND(find_if)

template<
      typename HBOOST_MPL_AUX_NA_PARAM(Sequence)
    , typename HBOOST_MPL_AUX_NA_PARAM(Predicate)
    >
struct find_if
{
    typedef typename iter_fold_if<
          Sequence
        , void
        , mpl::arg<1> // ignore
        , protect< aux::find_if_pred<Predicate> >
        >::type result_;

    typedef typename second<result_>::type type;

    HBOOST_MPL_AUX_LAMBDA_SUPPORT(2,find_if,(Sequence,Predicate))
};

HBOOST_MPL_AUX_NA_SPEC(2,find_if)

}}

#endif // HBOOST_MPL_FIND_IF_HPP_INCLUDED
