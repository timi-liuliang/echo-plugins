
#ifndef HBOOST_MPL_PARTITION_HPP_INCLUDED
#define HBOOST_MPL_PARTITION_HPP_INCLUDED

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

#include <hboost/mpl/stable_partition.hpp>
#include <hboost/mpl/aux_/inserter_algorithm.hpp>

namespace hboost { namespace mpl {

namespace aux {
 
template <
      typename Sequence
    , typename Pred
    , typename In1
    , typename In2
    >
struct partition_impl
    : stable_partition_impl<Sequence,Pred,In1,In2>
{
};

template <
      typename Sequence
    , typename Pred
    , typename In1
    , typename In2
    >
struct reverse_partition_impl
    : reverse_stable_partition_impl<Sequence,Pred,In1,In2>
{
};

} // namespace aux

HBOOST_MPL_AUX_INSERTER_ALGORITHM_DEF(4, partition)

}}

#endif // HBOOST_MPL_PARTITION_HPP_INCLUDED
