
// Copyright (C) 2003-2004 Jeremy B. Maitin-Shepard.
// Copyright (C) 2005-2011 Daniel James
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_UNORDERED_DETAIL_UTIL_HPP_INCLUDED
#define HBOOST_UNORDERED_DETAIL_UTIL_HPP_INCLUDED

#include <hboost/config.hpp>
#if defined(HBOOST_HAS_PRAGMA_ONCE)
#pragma once
#endif

#include <hboost/type_traits/is_convertible.hpp>
#include <hboost/type_traits/is_empty.hpp>
#include <hboost/iterator/iterator_categories.hpp>
#include <hboost/utility/enable_if.hpp>
#include <hboost/detail/select_type.hpp>
#include <hboost/move/move.hpp>
#include <hboost/preprocessor/seq/size.hpp>
#include <hboost/preprocessor/seq/enum.hpp>
#include <hboost/swap.hpp>

namespace hboost { namespace unordered { namespace detail {

    static const float minimum_max_load_factor = 1e-3f;
    static const std::size_t default_bucket_count = 11;
    struct move_tag {};
    struct empty_emplace {};

    namespace func {
        template <class T>
        inline void ignore_unused_variable_warning(T const&) {}
    }

    ////////////////////////////////////////////////////////////////////////////
    // iterator SFINAE

    template <typename I>
    struct is_forward :
        hboost::is_convertible<
            typename hboost::iterator_traversal<I>::type,
            hboost::forward_traversal_tag>
    {};

    template <typename I, typename ReturnType>
    struct enable_if_forward :
        hboost::enable_if_c<
            hboost::unordered::detail::is_forward<I>::value,
            ReturnType>
    {};

    template <typename I, typename ReturnType>
    struct disable_if_forward :
        hboost::disable_if_c<
            hboost::unordered::detail::is_forward<I>::value,
            ReturnType>
    {};

    ////////////////////////////////////////////////////////////////////////////
    // primes

#define HBOOST_UNORDERED_PRIMES \
    (17ul)(29ul)(37ul)(53ul)(67ul)(79ul) \
    (97ul)(131ul)(193ul)(257ul)(389ul)(521ul)(769ul) \
    (1031ul)(1543ul)(2053ul)(3079ul)(6151ul)(12289ul)(24593ul) \
    (49157ul)(98317ul)(196613ul)(393241ul)(786433ul) \
    (1572869ul)(3145739ul)(6291469ul)(12582917ul)(25165843ul) \
    (50331653ul)(100663319ul)(201326611ul)(402653189ul)(805306457ul) \
    (1610612741ul)(3221225473ul)(4294967291ul)

    template<class T> struct prime_list_template
    {
        static std::size_t const value[];

#if !defined(SUNPRO_CC)
        static std::ptrdiff_t const length;
#else
        static std::ptrdiff_t const length
            = HBOOST_PP_SEQ_SIZE(HBOOST_UNORDERED_PRIMES);
#endif
    };

    template<class T>
    std::size_t const prime_list_template<T>::value[] = {
        HBOOST_PP_SEQ_ENUM(HBOOST_UNORDERED_PRIMES)
    };

#if !defined(SUNPRO_CC)
    template<class T>
    std::ptrdiff_t const prime_list_template<T>::length
        = HBOOST_PP_SEQ_SIZE(HBOOST_UNORDERED_PRIMES);
#endif

#undef HBOOST_UNORDERED_PRIMES

    typedef prime_list_template<std::size_t> prime_list;

    // no throw
    inline std::size_t next_prime(std::size_t num) {
        std::size_t const* const prime_list_begin = prime_list::value;
        std::size_t const* const prime_list_end = prime_list_begin +
            prime_list::length;
        std::size_t const* bound =
            std::lower_bound(prime_list_begin, prime_list_end, num);
        if(bound == prime_list_end)
            bound--;
        return *bound;
    }

    // no throw
    inline std::size_t prev_prime(std::size_t num) {
        std::size_t const* const prime_list_begin = prime_list::value;
        std::size_t const* const prime_list_end = prime_list_begin +
            prime_list::length;
        std::size_t const* bound =
            std::upper_bound(prime_list_begin,prime_list_end, num);
        if(bound != prime_list_begin)
            bound--;
        return *bound;
    }

    ////////////////////////////////////////////////////////////////////////////
    // insert_size/initial_size

#if !defined(HBOOST_NO_STD_DISTANCE)

    using ::std::distance;

#else

    template <class ForwardIterator>
    inline std::size_t distance(ForwardIterator i, ForwardIterator j) {
        std::size_t x;
        std::distance(i, j, x);
        return x;
    }

#endif

    template <class I>
    inline typename
        hboost::unordered::detail::enable_if_forward<I, std::size_t>::type
        insert_size(I i, I j)
    {
        return std::distance(i, j);
    }

    template <class I>
    inline typename
        hboost::unordered::detail::disable_if_forward<I, std::size_t>::type
        insert_size(I, I)
    {
        return 1;
    }

    template <class I>
    inline std::size_t initial_size(I i, I j,
        std::size_t num_buckets =
            hboost::unordered::detail::default_bucket_count)
    {
        // TODO: Why +1?
        return (std::max)(
            hboost::unordered::detail::insert_size(i, j) + 1,
            num_buckets);
    }

    ////////////////////////////////////////////////////////////////////////////
    // compressed

    template <typename T, int Index>
    struct compressed_base : private T
    {
        compressed_base(T const& x) : T(x) {}
        compressed_base(T& x, move_tag) : T(hboost::move(x)) {}

        T& get() { return *this; }
        T const& get() const { return *this; }
    };
    
    template <typename T, int Index>
    struct uncompressed_base
    {
        uncompressed_base(T const& x) : value_(x) {}
        uncompressed_base(T& x, move_tag) : value_(hboost::move(x)) {}

        T& get() { return value_; }
        T const& get() const { return value_; }
    private:
        T value_;
    };
    
    template <typename T, int Index>
    struct generate_base
      : hboost::detail::if_true<
            hboost::is_empty<T>::value
        >:: HBOOST_NESTED_TEMPLATE then<
            hboost::unordered::detail::compressed_base<T, Index>,
            hboost::unordered::detail::uncompressed_base<T, Index>
        >
    {};
    
    template <typename T1, typename T2>
    struct compressed
      : private hboost::unordered::detail::generate_base<T1, 1>::type,
        private hboost::unordered::detail::generate_base<T2, 2>::type
    {
        typedef typename generate_base<T1, 1>::type base1;
        typedef typename generate_base<T2, 2>::type base2;

        typedef T1 first_type;
        typedef T2 second_type;
        
        first_type& first() {
            return static_cast<base1*>(this)->get();
        }

        first_type const& first() const {
            return static_cast<base1 const*>(this)->get();
        }

        second_type& second() {
            return static_cast<base2*>(this)->get();
        }

        second_type const& second() const {
            return static_cast<base2 const*>(this)->get();
        }

        template <typename First, typename Second>
        compressed(First const& x1, Second const& x2)
            : base1(x1), base2(x2) {}

        compressed(compressed const& x)
            : base1(x.first()), base2(x.second()) {}

        compressed(compressed& x, move_tag m)
            : base1(x.first(), m), base2(x.second(), m) {}

        void assign(compressed const& x)
        {
            first() = x.first();
            second() = x.second();
        }

        void move_assign(compressed& x)
        {
            first() = hboost::move(x.first());
            second() = hboost::move(x.second());
        }
        
        void swap(compressed& x)
        {
            hboost::swap(first(), x.first());
            hboost::swap(second(), x.second());
        }

    private:
        // Prevent assignment just to make use of assign or
        // move_assign explicit.
        compressed& operator=(compressed const&);
    };
}}}

#endif
