
#ifndef HBOOST_MPL_AUX_UNWRAP_HPP_INCLUDED
#define HBOOST_MPL_AUX_UNWRAP_HPP_INCLUDED

// Copyright Peter Dimov and Multi Media Ltd 2001, 2002
// Copyright David Abrahams 2001
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/ref.hpp>
#include <hboost/mpl/aux_/config/gpu.hpp>

namespace hboost { namespace mpl { namespace aux {

template< typename F >
HBOOST_MPL_CFG_GPU_ENABLED
inline
F& unwrap(F& f, long)
{
    return f;
}

template< typename F >
HBOOST_MPL_CFG_GPU_ENABLED
inline
F&
unwrap(reference_wrapper<F>& f, int)
{
    return f;
}

template< typename F >
HBOOST_MPL_CFG_GPU_ENABLED
inline
F&
unwrap(reference_wrapper<F> const& f, int)
{
    return f;
}

}}}

#endif // HBOOST_MPL_AUX_UNWRAP_HPP_INCLUDED
