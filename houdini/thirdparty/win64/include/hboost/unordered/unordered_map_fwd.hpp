
// Copyright (C) 2008-2011 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_UNORDERED_MAP_FWD_HPP_INCLUDED
#define HBOOST_UNORDERED_MAP_FWD_HPP_INCLUDED

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
        template <class K,
            class T,
            class H = hboost::hash<K>,
            class P = std::equal_to<K>,
            class A = std::allocator<std::pair<const K, T> > >
        class unordered_map;

        template <class K, class T, class H, class P, class A>
        inline bool operator==(unordered_map<K, T, H, P, A> const&,
            unordered_map<K, T, H, P, A> const&);
        template <class K, class T, class H, class P, class A>
        inline bool operator!=(unordered_map<K, T, H, P, A> const&,
            unordered_map<K, T, H, P, A> const&);
        template <class K, class T, class H, class P, class A>
        inline void swap(unordered_map<K, T, H, P, A>&,
                unordered_map<K, T, H, P, A>&);

        template <class K,
            class T,
            class H = hboost::hash<K>,
            class P = std::equal_to<K>,
            class A = std::allocator<std::pair<const K, T> > >
        class unordered_multimap;

        template <class K, class T, class H, class P, class A>
        inline bool operator==(unordered_multimap<K, T, H, P, A> const&,
            unordered_multimap<K, T, H, P, A> const&);
        template <class K, class T, class H, class P, class A>
        inline bool operator!=(unordered_multimap<K, T, H, P, A> const&,
            unordered_multimap<K, T, H, P, A> const&);
        template <class K, class T, class H, class P, class A>
        inline void swap(unordered_multimap<K, T, H, P, A>&,
                unordered_multimap<K, T, H, P, A>&);
    }

    using hboost::unordered::unordered_map;
    using hboost::unordered::unordered_multimap;
    using hboost::unordered::swap;
    using hboost::unordered::operator==;
    using hboost::unordered::operator!=;
}

#endif
