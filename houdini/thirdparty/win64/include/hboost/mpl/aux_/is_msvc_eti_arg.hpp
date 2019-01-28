
#ifndef HBOOST_MPL_AUX_IS_MSVC_ETI_ARG_HPP_INCLUDED
#define HBOOST_MPL_AUX_IS_MSVC_ETI_ARG_HPP_INCLUDED

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

#include <hboost/mpl/aux_/yes_no.hpp>
#include <hboost/mpl/aux_/config/eti.hpp>
#include <hboost/mpl/aux_/config/static_constant.hpp>

namespace hboost { namespace mpl { namespace aux {

#if defined(HBOOST_MPL_CFG_MSVC_ETI_BUG)

#if defined(HBOOST_MPL_CFG_MSVC_60_ETI_BUG)

template< typename T >
struct is_msvc_eti_arg
{ 
    HBOOST_STATIC_CONSTANT(bool, value = false);
};

#else // HBOOST_MPL_CFG_MSVC_60_ETI_BUG

struct eti_int_convertible
{
    eti_int_convertible(int);
};

template< typename T >
struct is_msvc_eti_arg
{ 
    static no_tag test(...);
    static yes_tag test(eti_int_convertible);
    static T& get();

    HBOOST_STATIC_CONSTANT(bool, value = 
          sizeof(test(get())) == sizeof(yes_tag)
        );
};

#endif

template<>
struct is_msvc_eti_arg<int>
{ 
    HBOOST_STATIC_CONSTANT(bool, value = true);
};

#endif // HBOOST_MPL_CFG_MSVC_ETI_BUG

}}}

#endif // HBOOST_MPL_AUX_IS_MSVC_ETI_ARG_HPP_INCLUDED
