// Copyright (C) 2003, 2008 Fernando Luis Cacciola Carballal.
// Copyright (C) 2015 Andrzej Krzemienski.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/optional for documentation.
//
// You are welcome to contact the author at:
//  akrzemi1@gmail.com

#ifndef HBOOST_OPTIONAL_DETAIL_OPTIONAL_SWAP_AJK_28JAN2015_HPP
#define HBOOST_OPTIONAL_DETAIL_OPTIONAL_SWAP_AJK_28JAN2015_HPP

#include <hboost/core/swap.hpp>
#include <hboost/optional/optional_fwd.hpp>

namespace hboost {

namespace optional_detail {

template <bool use_default_constructor> struct swap_selector;

template <>
struct swap_selector<true>
{
    template <class T>
    static void optional_swap ( optional<T>& x, optional<T>& y )
    {
        const bool hasX = !!x;
        const bool hasY = !!y;

        if ( !hasX && !hasY )
            return;

        if( !hasX )
            x.emplace();
        else if ( !hasY )
            y.emplace();

        // Boost.Utility.Swap will take care of ADL and workarounds for broken compilers
        hboost::swap(x.get(), y.get());

        if( !hasX )
            y = hboost::none ;
        else if( !hasY )
            x = hboost::none ;
    }
};

#ifdef HBOOST_OPTIONAL_DETAIL_MOVE
# undef HBOOST_OPTIONAL_DETAIL_MOVE
#endif

#ifndef HBOOST_OPTIONAL_DETAIL_NO_RVALUE_REFERENCES
# define HBOOST_OPTIONAL_DETAIL_MOVE(EXPR_) hboost::move(EXPR_)
#else
# define HBOOST_OPTIONAL_DETAIL_MOVE(EXPR_) EXPR_
#endif

template <>
struct swap_selector<false>
{
    template <class T>
    static void optional_swap ( optional<T>& x, optional<T>& y ) 
    //HBOOST_NOEXCEPT_IF(::hboost::is_nothrow_move_constructible<T>::value && HBOOST_NOEXCEPT_EXPR(hboost::swap(*x, *y)))
    {
        if (x)
        {
            if (y)
            {
                hboost::swap(*x, *y);
            }
            else
            {
                y = HBOOST_OPTIONAL_DETAIL_MOVE(*x);
                x = hboost::none;
            }
        }
        else
        {
            if (y)
            {
                x = HBOOST_OPTIONAL_DETAIL_MOVE(*y);
                y = hboost::none;
            }
        }
    }
};

} // namespace optional_detail

#if (!defined HBOOST_NO_CXX11_RVALUE_REFERENCES) && (!defined HBOOST_CONFIG_RESTORE_OBSOLETE_SWAP_IMPLEMENTATION)

template<class T>
struct optional_swap_should_use_default_constructor : hboost::false_type {} ;

#else

template<class T>
struct optional_swap_should_use_default_constructor : has_nothrow_default_constructor<T> {} ;

#endif

template <class T>
inline void swap ( optional<T>& x, optional<T>& y )
//HBOOST_NOEXCEPT_IF(::hboost::is_nothrow_move_constructible<T>::value && HBOOST_NOEXCEPT_EXPR(hboost::swap(*x, *y)))
{
    optional_detail::swap_selector<optional_swap_should_use_default_constructor<T>::value>::optional_swap(x, y);
}

} // namespace hboost

#undef HBOOST_OPTIONAL_DETAIL_MOVE

#endif // header guard
