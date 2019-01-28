
#ifndef HBOOST_MPL_ERASE_KEY_HPP_INCLUDED
#define HBOOST_MPL_ERASE_KEY_HPP_INCLUDED

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

#include <hboost/mpl/erase_key_fwd.hpp>
#include <hboost/mpl/sequence_tag.hpp>
#include <hboost/mpl/aux_/erase_key_impl.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>
#include <hboost/mpl/aux_/config/msvc_typename.hpp>

namespace hboost { namespace mpl {

template<
      typename HBOOST_MPL_AUX_NA_PARAM(Sequence)
    , typename HBOOST_MPL_AUX_NA_PARAM(Key)
    >
struct erase_key
    : erase_key_impl< typename sequence_tag<Sequence>::type >
        ::template apply< Sequence,Key >
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(2,erase_key,(Sequence,Key))
};

HBOOST_MPL_AUX_NA_SPEC(2,erase_key)

}}

#endif // HBOOST_MPL_ERASE_KEY_HPP_INCLUDED
