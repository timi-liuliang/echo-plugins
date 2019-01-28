
#ifndef HBOOST_MPL_LIST_AUX_ITERATOR_HPP_INCLUDED
#define HBOOST_MPL_LIST_AUX_ITERATOR_HPP_INCLUDED

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

#include <hboost/mpl/iterator_tags.hpp>
#include <hboost/mpl/next_prior.hpp>
#include <hboost/mpl/deref.hpp>
#include <hboost/mpl/list/aux_/item.hpp>
#include <hboost/mpl/aux_/na.hpp>
#include <hboost/mpl/aux_/lambda_spec.hpp>
#include <hboost/mpl/aux_/config/ctps.hpp>

namespace hboost { namespace mpl {

#if !defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< typename Node >
struct l_iter
{
    typedef aux::l_iter_tag tag;
    typedef forward_iterator_tag category;
};

template< typename Node >
struct deref< l_iter<Node> >
{
    typedef typename Node::item type;
};

template< typename Node >
struct next< l_iter<Node> >
{
    typedef l_iter< typename Node::next > type;
};

#else // HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template< typename Node >
struct l_iter
{
    typedef aux::l_iter_tag tag;
    typedef forward_iterator_tag category;
    typedef typename Node::item type;
    typedef l_iter< typename mpl::next<Node>::type > next;
};

#endif


template<> struct l_iter<l_end>
{
    typedef aux::l_iter_tag tag;
    typedef forward_iterator_tag category;
#if defined(HBOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
    typedef na type;
    typedef l_iter next;
#endif
};

HBOOST_MPL_AUX_PASS_THROUGH_LAMBDA_SPEC(1, l_iter)

}}

#endif // HBOOST_MPL_LIST_AUX_ITERATOR_HPP_INCLUDED
