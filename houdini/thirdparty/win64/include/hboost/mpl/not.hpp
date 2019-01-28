
#ifndef HBOOST_MPL_NOT_HPP_INCLUDED
#define HBOOST_MPL_NOT_HPP_INCLUDED

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

#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/aux_/nttp_decl.hpp>
#include <hboost/mpl/aux_/nested_type_wknd.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>

namespace hboost { namespace mpl {

namespace aux {

template< HBOOST_MPL_AUX_NTTP_DECL(long, C_) > // 'long' is intentional here
struct not_impl
    : bool_<!C_>
{
};

} // namespace aux


template<
      typename HBOOST_MPL_AUX_NA_PARAM(T)
    >
struct not_
    : aux::not_impl<
          HBOOST_MPL_AUX_NESTED_TYPE_WKND(T)::value
        >
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(1,not_,(T))
};

HBOOST_MPL_AUX_NA_SPEC(1,not_)

}}

#endif // HBOOST_MPL_NOT_HPP_INCLUDED
