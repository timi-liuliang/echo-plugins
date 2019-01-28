
#ifndef HBOOST_MPL_SET_AUX_HAS_KEY_IMPL_HPP_INCLUDED
#define HBOOST_MPL_SET_AUX_HAS_KEY_IMPL_HPP_INCLUDED

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

#include <hboost/mpl/set/aux_/tag.hpp>
#include <hboost/mpl/has_key_fwd.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/aux_/overload_names.hpp>
#include <hboost/mpl/aux_/static_cast.hpp>
#include <hboost/mpl/aux_/yes_no.hpp>
#include <hboost/mpl/aux_/type_wrapper.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>
#include <hboost/mpl/aux_/config/static_constant.hpp>

namespace hboost { namespace mpl {

template<>
struct has_key_impl< aux::set_tag >
{
    template< typename Set, typename T > struct apply
#if HBOOST_WORKAROUND(HBOOST_MSVC, HBOOST_TESTED_AT(1400)) \
    || HBOOST_WORKAROUND(__EDG_VERSION__, <= 245)
    {
        HBOOST_STATIC_CONSTANT(bool, value = 
              ( sizeof( HBOOST_MPL_AUX_OVERLOAD_CALL_IS_MASKED(
                    Set
                  , HBOOST_MPL_AUX_STATIC_CAST(aux::type_wrapper<T>*, 0)
                  ) ) == sizeof(aux::no_tag) )
            );

        typedef bool_<value> type;

#else // ISO98 C++
        : bool_< 
              ( sizeof( HBOOST_MPL_AUX_OVERLOAD_CALL_IS_MASKED(
                    Set
                  , HBOOST_MPL_AUX_STATIC_CAST(aux::type_wrapper<T>*, 0)
                  ) ) == sizeof(aux::no_tag) )
            >
    {
#endif
    };
};

}}

#endif // HBOOST_MPL_SET_AUX_HAS_KEY_IMPL_HPP_INCLUDED
