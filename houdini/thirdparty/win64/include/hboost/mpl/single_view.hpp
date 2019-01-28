
#ifndef HBOOST_MPL_SINGLE_VIEW_HPP_INCLUDED
#define HBOOST_MPL_SINGLE_VIEW_HPP_INCLUDED

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

#include <hboost/mpl/aux_/single_element_iter.hpp>
#include <hboost/mpl/iterator_range.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>

namespace hboost { namespace mpl {

template<
      typename HBOOST_MPL_AUX_NA_PARAM(T)
    >
struct single_view
    : iterator_range<
          aux::sel_iter<T,0>
        , aux::sel_iter<T,1>
        >
{
};

HBOOST_MPL_AUX_NA_SPEC_NO_ETI(1, single_view)

}}

#endif // HBOOST_MPL_SINGLE_VIEW_HPP_INCLUDED
