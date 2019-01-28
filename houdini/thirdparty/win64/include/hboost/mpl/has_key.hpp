
#ifndef HBOOST_MPL_HAS_KEY_HPP_INCLUDED
#define HBOOST_MPL_HAS_KEY_HPP_INCLUDED

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

#include <hboost/mpl/has_key_fwd.hpp>
#include <hboost/mpl/sequence_tag.hpp>
#include <hboost/mpl/aux_/has_key_impl.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>

namespace hboost { namespace mpl {

template<
      typename HBOOST_MPL_AUX_NA_PARAM(AssociativeSequence)
    , typename HBOOST_MPL_AUX_NA_PARAM(Key)
    >
struct has_key
    : has_key_impl< typename sequence_tag<AssociativeSequence>::type >
        ::template apply<AssociativeSequence,Key>
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(2,has_key,(AssociativeSequence,Key))
};

HBOOST_MPL_AUX_NA_SPEC(2, has_key)

}}

#endif // HBOOST_MPL_HAS_KEY_HPP_INCLUDED
