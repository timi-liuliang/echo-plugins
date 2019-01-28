//-----------------------------------------------------------------------------
// boost variant/detail/enable_recursive.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_DETAIL_ENABLE_RECURSIVE_HPP
#define HBOOST_VARIANT_DETAIL_ENABLE_RECURSIVE_HPP

#include "hboost/variant/detail/enable_recursive_fwd.hpp"
#include "hboost/variant/variant_fwd.hpp"

#if !defined(HBOOST_VARIANT_NO_FULL_RECURSIVE_VARIANT_SUPPORT)
#   include "hboost/mpl/apply.hpp"
#   include "hboost/mpl/eval_if.hpp"
#   include "hboost/mpl/lambda.hpp"
#endif

#include "hboost/variant/detail/substitute.hpp"
#include "hboost/mpl/aux_/config/ctps.hpp"
#include "hboost/mpl/bool_fwd.hpp"
#include "hboost/mpl/if.hpp"
#include "hboost/mpl/or.hpp"
#include "hboost/type_traits/is_pointer.hpp"
#include "hboost/type_traits/is_reference.hpp"
#include "hboost/type_traits/is_same.hpp"

#include "hboost/variant/recursive_wrapper.hpp"

namespace hboost {
namespace detail { namespace variant {

#if !defined(HBOOST_VARIANT_DETAIL_NO_SUBSTITUTE)

#   define HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL(T,Dest,Source) \
    substitute< T , Dest , Source > \
    /**/

#else // defined(HBOOST_VARIANT_DETAIL_NO_SUBSTITUTE)

///////////////////////////////////////////////////////////////////////////////
// (detail) class template rebind1
//
// Limited workaround in case 'substitute' metafunction unavailable.
//

template <typename T, typename U1>
struct rebind1
{
private:
    typedef typename mpl::lambda<
          mpl::identity<T>
        >::type le_;

public:
    typedef typename mpl::eval_if<
          is_same< le_, mpl::identity<T> >
        , le_ // identity<T>
        , mpl::apply1<le_, U1>
        >::type type;
};

#   define HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL(T,Dest,Source) \
    rebind1< T , Dest > \
    /**/

#endif // !defined(HBOOST_VARIANT_DETAIL_NO_SUBSTITUTE)

///////////////////////////////////////////////////////////////////////////////
// (detail) metafunction enable_recursive
//
// See hboost/variant/detail/enable_recursive_fwd.hpp for more information.
//


template <typename T, typename RecursiveVariant, typename NoWrapper>
struct enable_recursive
    : HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL(
          T, RecursiveVariant, ::hboost::recursive_variant_
        )
{
};

template <typename T, typename RecursiveVariant>
struct enable_recursive< T,RecursiveVariant,mpl::false_ >
{
private: // helpers, for metafunction result (below)

    typedef typename HBOOST_VARIANT_AUX_ENABLE_RECURSIVE_IMPL(
          T, RecursiveVariant, ::hboost::recursive_variant_
        )::type t_;

public: // metafunction result

    // [Wrap with recursive_wrapper only if rebind really changed something:]
    typedef typename mpl::if_<
          mpl::or_<
              is_same< t_,T >
            , is_reference<t_>
            , is_pointer<t_>
            >
        , t_
        , hboost::recursive_wrapper<t_>
        >::type type;

};


///////////////////////////////////////////////////////////////////////////////
// (detail) metafunction class quoted_enable_recursive
//
// Same behavior as enable_recursive metafunction (see above).
//
template <typename RecursiveVariant, typename NoWrapper>
struct quoted_enable_recursive
{
    template <typename T>
    struct apply
        : enable_recursive<T, RecursiveVariant, NoWrapper>
    {
    };
};

}} // namespace detail::variant
} // namespace hboost

#endif // HBOOST_VARIANT_DETAIL_ENABLE_RECURSIVE_HPP
