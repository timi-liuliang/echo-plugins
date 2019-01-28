
#ifndef HBOOST_MPL_ITERATOR_CATEGORY_HPP_INCLUDED
#define HBOOST_MPL_ITERATOR_CATEGORY_HPP_INCLUDED

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

#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>

namespace hboost {  namespace mpl {

template<
      typename HBOOST_MPL_AUX_NA_PARAM(Iterator)
    >
struct iterator_category
{
    typedef typename Iterator::category type;
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(1,iterator_category,(Iterator))
};

HBOOST_MPL_AUX_NA_SPEC(1, iterator_category)

}}

#endif // HBOOST_MPL_ITERATOR_CATEGORY_HPP_INCLUDED
