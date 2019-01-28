
#ifndef HBOOST_MPL_IS_SEQUENCE_HPP_INCLUDED
#define HBOOST_MPL_IS_SEQUENCE_HPP_INCLUDED

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

#include <hboost/mpl/not.hpp>
#include <hboost/mpl/and.hpp>
#include <hboost/mpl/begin_end.hpp>
#include <hboost/mpl/if.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/mpl/sequence_tag_fwd.hpp>
#include <hboost/mpl/identity.hpp>
#include <hboost/mpl/void.hpp>
#include <hboost/mpl/aux_/has_tag.hpp>
#include <hboost/mpl/aux_/has_begin.hpp>
#include <hboost/mpl/aux_/na_spec.hpp>
#include <hboost/mpl/aux_/lambda_support.hpp>
#include <hboost/mpl/aux_/config/eti.hpp>
#include <hboost/mpl/aux_/config/msvc.hpp>
#include <hboost/mpl/aux_/config/workaround.hpp>
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
#   include <hboost/mpl/aux_/msvc_is_class.hpp>
#elif HBOOST_WORKAROUND(HBOOST_MSVC, == 1300)
#   include <hboost/type_traits/is_class.hpp>
#endif

#include <hboost/type_traits/is_same.hpp>

namespace hboost { namespace mpl {

#if HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300)

namespace aux {

// agurt, 11/jun/03: 
// MSVC 6.5/7.0 fails if 'has_begin' is instantiated on a class type that has a
// 'begin' member that doesn't name a type; e.g. 'has_begin< std::vector<int> >'
// would fail; requiring 'T' to have _both_ 'tag' and 'begin' members workarounds
// the issue for most real-world cases
template< typename T > struct is_sequence_impl
    : and_<
          identity< aux::has_tag<T> >
        , identity< aux::has_begin<T> >
        >
{
};

} // namespace aux
        
template<
      typename HBOOST_MPL_AUX_NA_PARAM(T)
    >
struct is_sequence
    : if_<
#if HBOOST_WORKAROUND(HBOOST_MSVC, < 1300)
          aux::msvc_is_class<T> 
#else
          hboost::is_class<T> 
#endif
        , aux::is_sequence_impl<T>
        , bool_<false>
        >::type
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(1, is_sequence, (T))
};

#elif defined(HBOOST_MPL_CFG_NO_HAS_XXX)

template<
      typename HBOOST_MPL_AUX_NA_PARAM(T)
    >
struct is_sequence
    : bool_<false>
{
};

#else

template<
      typename HBOOST_MPL_AUX_NA_PARAM(T)
    >
struct is_sequence
    : not_< is_same< typename begin<T>::type, void_ > >
{
    HBOOST_MPL_AUX_LAMBDA_SUPPORT(1, is_sequence, (T))
};

#endif // HBOOST_MSVC

#if defined(HBOOST_MPL_CFG_MSVC_60_ETI_BUG)
template<> struct is_sequence<int>
    : bool_<false>
{
};
#endif

HBOOST_MPL_AUX_NA_SPEC_NO_ETI(1, is_sequence)

}}

#endif // HBOOST_MPL_IS_SEQUENCE_HPP_INCLUDED
