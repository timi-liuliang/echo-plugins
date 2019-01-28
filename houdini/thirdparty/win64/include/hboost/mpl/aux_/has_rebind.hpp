
#ifndef HBOOST_MPL_AUX_HAS_REBIND_HPP_INCLUDED
#define HBOOST_MPL_AUX_HAS_REBIND_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date$
// $Revision$

#include <hboost/mpl/aux_/config/msvc.hpp>
#include <hboost/mpl/aux_/config/intel.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>

#if HBOOST_WORKAROUND(__EDG_VERSION__, <= 244) && !defined(HBOOST_INTEL_CXX_VERSION)
#   include <hboost/mpl/has_xxx.hpp>
#elif HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
#   include <hboost/mpl/has_xxx.hpp>
#   include <hboost/mpl/if.hpp>
#   include <hboost/mpl/bool.hpp>
#   include <hboost/mpl/aux_/msvc_is_class.hpp>
#elif HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x610))
#   include <hboost/mpl/if.hpp>
#   include <hboost/mpl/bool.hpp>
#   include <hboost/mpl/aux_/yes_no.hpp>
#   include <hboost/mpl/aux_/config/static_constant.hpp>
#   include <hboost/type_traits/is_class.hpp>
#else
#   include <hboost/mpl/aux_/type_wrapper.hpp>
#   include <hboost/mpl/aux_/yes_no.hpp>
#   include <hboost/mpl/aux_/config/static_constant.hpp>
#endif

namespace hboost { namespace mpl { namespace aux {

#if HBOOST_WORKAROUND(__EDG_VERSION__, <= 244) && !defined(HBOOST_INTEL_CXX_VERSION)

HBOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_rebind, rebind, false)

#elif HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)

HBOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_rebind_impl, rebind, false)

template< typename T >
struct has_rebind
    : if_< 
          msvc_is_class<T>
        , has_rebind_impl<T>
        , bool_<false>
        >::type
{
};

#else // the rest

template< typename T > struct has_rebind_tag {};
no_tag operator|(has_rebind_tag<int>, void const volatile*);

#   if !HBOOST_WORKAROUND(__BORLANDC__, HBOOST_TESTED_AT(0x610))
template< typename T >
struct has_rebind
{
    static has_rebind_tag<T>* get();
    HBOOST_STATIC_CONSTANT(bool, value = 
          sizeof(has_rebind_tag<int>() | get()) == sizeof(yes_tag)
        );
};
#   else // __BORLANDC__
template< typename T >
struct has_rebind_impl
{
    static T* get();
    HBOOST_STATIC_CONSTANT(bool, value = 
          sizeof(has_rebind_tag<int>() | get()) == sizeof(yes_tag)
        );
};

template< typename T >
struct has_rebind
    : if_< 
          is_class<T>
        , has_rebind_impl<T>
        , bool_<false>
        >::type
{
};
#   endif // __BORLANDC__

#endif

}}}

#endif // HBOOST_MPL_AUX_HAS_REBIND_HPP_INCLUDED
