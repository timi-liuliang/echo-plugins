
#ifndef HBOOST_MPL_VECTOR_AUX_TAG_HPP_INCLUDED
#define HBOOST_MPL_VECTOR_AUX_TAG_HPP_INCLUDED

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

#include <hboost/mpl/aux_/config/typeof.hpp>
#include <hboost/mpl/aux_/nttp_decl.hpp>

namespace hboost { namespace mpl { namespace aux {

struct v_iter_tag;

#if defined(HBOOST_MPL_CFG_TYPEOF_BASED_SEQUENCES)
struct vector_tag;
#else
template< HBOOST_MPL_AUX_NTTP_DECL(long, N) > struct vector_tag;
#endif

}}}

#endif // HBOOST_MPL_VECTOR_AUX_TAG_HPP_INCLUDED
