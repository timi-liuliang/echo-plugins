
#ifndef HBOOST_MPL_MAX_ELEMENT_HPP_INCLUDED
#define HBOOST_MPL_MAX_ELEMENT_HPP_INCLUDED

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

#include <hboost/mpl/less.hpp>
#include <hboost/mpl/iter_fold.hpp>
#include <hboost/mpl/begin_end.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/deref.hpp>
#include <hboost/mpl/apply.hpp>
#include <hboost/mpl/aux_/common_name_wknd.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>

namespace hboost { namespace mpl {

HBOOST_MPL_AUX_COMMON_NAME_WKND(max_element)

namespace aux {

template< typename Predicate >
struct select_max
{
    template< typename OldIterator, typename Iterator >
    struct apply
    {
        typedef typename apply2<
              Predicate
            , typename deref<OldIterator>::type
            , typename deref<Iterator>::type
            >::type condition_;

        typedef typename if_<
              condition_
            , Iterator
            , OldIterator
            >::type type;
    };
};

} // namespace aux 


template<
      typename HBOOST_MPL_AUX_NA_PARAM(Sequence)
    , typename Predicate = less<_,_>
    >
struct max_element
    : iter_fold<
          Sequence
        , typename begin<Sequence>::type
        , protect< aux::select_max<Predicate> >
        >
{
};

HBOOST_MPL_AUX_NA_SPEC(1, max_element)

}}

#endif // HBOOST_MPL_MAX_ELEMENT_HPP_INCLUDED
