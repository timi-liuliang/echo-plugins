
#ifndef HBOOST_MPL_ITERATOR_RANGE_HPP_INCLUDED
#define HBOOST_MPL_ITERATOR_RANGE_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>

namespace hboost { namespace mpl {

struct iterator_range_tag;

template<
      typename HBOOST_MPL_AUX_NA_PARAM(First)
    , typename HBOOST_MPL_AUX_NA_PARAM(Last)
    >
struct iterator_range
{
    typedef iterator_range_tag tag;
    typedef iterator_range type;
    typedef First begin;
    typedef Last end;

    HBOOST_MPL_AUX_LAMBDA_SUPPORT(2,iterator_range,(First,Last))
};

HBOOST_MPL_AUX_NA_SPEC(2, iterator_range)

}}

#endif // HBOOST_MPL_ITERATOR_RANGE_HPP_INCLUDED