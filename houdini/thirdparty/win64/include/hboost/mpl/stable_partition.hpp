
#ifndef HBOOST_MPL_STABLE_PARTITION_HPP_INCLUDED
#define HBOOST_MPL_STABLE_PARTITION_HPP_INCLUDED

// Copyright Eric Friedman 2002-2003
// Copyright Aleksey Gurtovoy 2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/fold.hpp>
#include <hboost/mpl/reverse_fold.hpp>
#include <hboost/mpl/protect.hpp>
#include <hboost/mpl/aux_/partition_op.hpp>
#include <hboost/mpl/aux_/inserter_algorithm.hpp>
#include <hboost/mpl/aux_/na.hpp>

namespace hboost { namespace mpl {

namespace aux {
 
template <
      typename Sequence
    , typename Pred
    , typename In
    , typename In2
    , typename In1 = typename if_na<In,In2>::type
    >
struct stable_partition_impl
    : fold<
          Sequence
        , pair< typename In1::state, typename In2::state >
        , protect< partition_op< 
              Pred
            , typename In1::operation
            , typename In2::operation
            > >
        >
{
};

template <
      typename Sequence
    , typename Pred
    , typename In
    , typename In2
    , typename In1 = typename if_na<In,In2>::type
    >
struct reverse_stable_partition_impl
    : reverse_fold<
          Sequence
        , pair< typename In1::state, typename In2::state >
        , protect< partition_op< 
              Pred
            , typename In1::operation
            , typename In2::operation
            > >
        >
{
};

} // namespace aux

HBOOST_MPL_AUX_INSERTER_ALGORITHM_DEF(4, stable_partition)

}}

#endif // HBOOST_MPL_STABLE_PARTITION_HPP_INCLUDED
