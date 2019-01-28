
// Copyright (C) 2008-2011 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_UNORDERED_SET_FWD_HPP_INCLUDED
#define HBOOST_UNORDERED_SET_FWD_HPP_INCLUDED

#include <hboost/config.hpp>
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#pragma once
#endif

#include <memory>
#include <functional>
#include <hboost/functional/hash_fwd.hpp>
#include <hboost/unordered/detail/fwd.hpp>

namespace hboost
{
    namespace unordered
    {
        template <class T,
            class H = hboost::hash<T>,
            class P = std::equal_to<T>,
            class A = std::allocator<T> >
        class unordered_set;

        template <class T, class H, class P, class A>
        inline bool operator==(unordered_set<T, H, P, A> const&,
            unordered_set<T, H, P, A> const&);
        template <class T, class H, class P, class A>
        inline bool operator!=(unordered_set<T, H, P, A> const&,
            unordered_set<T, H, P, A> const&);
        template <class T, class H, class P, class A>
        inline void swap(unordered_set<T, H, P, A> &m1,
                unordered_set<T, H, P, A> &m2);

        template <class T,
            class H = hboost::hash<T>,
            class P = std::equal_to<T>,
            class A = std::allocator<T> >
        class unordered_multiset;

        template <class T, class H, class P, class A>
        inline bool operator==(unordered_multiset<T, H, P, A> const&,
            unordered_multiset<T, H, P, A> const&);
        template <class T, class H, class P, class A>
        inline bool operator!=(unordered_multiset<T, H, P, A> const&,
            unordered_multiset<T, H, P, A> const&);
        template <class T, class H, class P, class A>
        inline void swap(unordered_multiset<T, H, P, A> &m1,
                unordered_multiset<T, H, P, A> &m2);
    }

    using hboost::unordered::unordered_set;
    using hboost::unordered::unordered_multiset;
    using hboost::unordered::swap;
    using hboost::unordered::operator==;
    using hboost::unordered::operator!=;
}

#endif
