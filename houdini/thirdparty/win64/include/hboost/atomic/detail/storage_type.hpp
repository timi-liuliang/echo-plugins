/*
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Copyright (c) 2009 Helge Bahmann
 * Copyright (c) 2012 Tim Blechmann
 * Copyright (c) 2013 - 2014 Andrey Semashev
 */
/*!
 * \file   atomic/detail/storage_type.hpp
 *
 * This header defines underlying types used as storage
 */

#ifndef HBOOST_ATOMIC_DETAIL_STORAGE_TYPE_HPP_INCLUDED_
#define HBOOST_ATOMIC_DETAIL_STORAGE_TYPE_HPP_INCLUDED_

#include <cstddef>
#include <hboost/cstdint.hpp>
#include <hboost/atomic/detail/config.hpp>
#if !defined(HBOOST_ATOMIC_DETAIL_HAS_BUILTIN_MEMCMP) || !defined(HBOOST_ATOMIC_DETAIL_HAS_BUILTIN_MEMCPY)
#include <cstring>
#endif

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

namespace hboost {
namespace atomics {
namespace detail {

template< typename T >
HBOOST_FORCEINLINE void non_atomic_load(T const volatile& from, T& to) HBOOST_NOEXCEPT
{
    to = from;
}

template< std::size_t Size >
struct buffer_storage
{
    HBOOST_ALIGNMENT(16) unsigned char data[Size];

    HBOOST_FORCEINLINE bool operator! () const HBOOST_NOEXCEPT
    {
        return (data[0] == 0u && HBOOST_ATOMIC_DETAIL_MEMCMP(data, data + 1, Size - 1) == 0);
    }

    HBOOST_FORCEINLINE bool operator== (buffer_storage const& that) const HBOOST_NOEXCEPT
    {
        return HBOOST_ATOMIC_DETAIL_MEMCMP(data, that.data, Size) == 0;
    }

    HBOOST_FORCEINLINE bool operator!= (buffer_storage const& that) const HBOOST_NOEXCEPT
    {
        return HBOOST_ATOMIC_DETAIL_MEMCMP(data, that.data, Size) != 0;
    }
};

template< std::size_t Size >
HBOOST_FORCEINLINE void non_atomic_load(buffer_storage< Size > const volatile& from, buffer_storage< Size >& to) HBOOST_NOEXCEPT
{
    HBOOST_ATOMIC_DETAIL_MEMCPY(to.data, const_cast< unsigned char const* >(from.data), Size);
}

template< std::size_t Size, bool Signed >
struct make_storage_type
{
    typedef buffer_storage< Size > type;

    struct aligned
    {
        type value;

        HBOOST_DEFAULTED_FUNCTION(aligned(), {})
        HBOOST_FORCEINLINE HBOOST_CONSTEXPR explicit aligned(type const& v) HBOOST_NOEXCEPT : value(v) {}
    };
};

template< >
struct make_storage_type< 1u, false >
{
    typedef hboost::uint8_t type;

    struct aligned
    {
        type value;

        HBOOST_DEFAULTED_FUNCTION(aligned(), {})
        HBOOST_FORCEINLINE HBOOST_CONSTEXPR explicit aligned(type v) HBOOST_NOEXCEPT : value(v) {}
    };
};

template< >
struct make_storage_type< 1u, true >
{
    typedef hboost::int8_t type;

    struct aligned
    {
        type value;

        HBOOST_DEFAULTED_FUNCTION(aligned(), {})
        HBOOST_FORCEINLINE HBOOST_CONSTEXPR explicit aligned(type v) HBOOST_NOEXCEPT : value(v) {}
    };
};

template< >
struct make_storage_type< 2u, false >
{
    typedef hboost::uint16_t type;

    struct aligned
    {
        HBOOST_ALIGNMENT(2) type value;

        HBOOST_DEFAULTED_FUNCTION(aligned(), {})
        HBOOST_FORCEINLINE HBOOST_CONSTEXPR explicit aligned(type v) HBOOST_NOEXCEPT : value(v) {}
    };
};

template< >
struct make_storage_type< 2u, true >
{
    typedef hboost::int16_t type;

    struct aligned
    {
        HBOOST_ALIGNMENT(2) type value;

        HBOOST_DEFAULTED_FUNCTION(aligned(), {})
        HBOOST_FORCEINLINE HBOOST_CONSTEXPR explicit aligned(type v) HBOOST_NOEXCEPT : value(v) {}
    };
};

template< >
struct make_storage_type< 4u, false >
{
    typedef hboost::uint32_t type;

    struct aligned
    {
        HBOOST_ALIGNMENT(4) type value;

        HBOOST_DEFAULTED_FUNCTION(aligned(), {})
        HBOOST_FORCEINLINE HBOOST_CONSTEXPR explicit aligned(type v) HBOOST_NOEXCEPT : value(v) {}
    };
};

template< >
struct make_storage_type< 4u, true >
{
    typedef hboost::int32_t type;

    struct aligned
    {
        HBOOST_ALIGNMENT(4) type value;

        HBOOST_DEFAULTED_FUNCTION(aligned(), {})
        HBOOST_FORCEINLINE HBOOST_CONSTEXPR explicit aligned(type v) HBOOST_NOEXCEPT : value(v) {}
    };
};

template< >
struct make_storage_type< 8u, false >
{
    typedef hboost::uint64_t type;

    struct aligned
    {
        HBOOST_ALIGNMENT(8) type value;

        HBOOST_DEFAULTED_FUNCTION(aligned(), {})
        HBOOST_FORCEINLINE HBOOST_CONSTEXPR explicit aligned(type v) HBOOST_NOEXCEPT : value(v) {}
    };
};

template< >
struct make_storage_type< 8u, true >
{
    typedef hboost::int64_t type;

    struct aligned
    {
        HBOOST_ALIGNMENT(8) type value;

        HBOOST_DEFAULTED_FUNCTION(aligned(), {})
        HBOOST_FORCEINLINE HBOOST_CONSTEXPR explicit aligned(type v) HBOOST_NOEXCEPT : value(v) {}
    };
};

#if defined(HBOOST_HAS_INT128)

template< >
struct make_storage_type< 16u, false >
{
    typedef hboost::uint128_type type;

    struct aligned
    {
        HBOOST_ALIGNMENT(16) type value;

        HBOOST_DEFAULTED_FUNCTION(aligned(), {})
        HBOOST_FORCEINLINE HBOOST_CONSTEXPR explicit aligned(type v) HBOOST_NOEXCEPT : value(v) {}
    };
};

template< >
struct make_storage_type< 16u, true >
{
    typedef hboost::int128_type type;

    struct aligned
    {
        HBOOST_ALIGNMENT(16) type value;

        HBOOST_DEFAULTED_FUNCTION(aligned(), {})
        HBOOST_FORCEINLINE HBOOST_CONSTEXPR explicit aligned(type v) HBOOST_NOEXCEPT : value(v) {}
    };
};

#elif !defined(HBOOST_NO_ALIGNMENT)

struct storage128_t
{
    hboost::uint64_t data[2];

    HBOOST_FORCEINLINE bool operator! () const HBOOST_NOEXCEPT
    {
        return data[0] == 0 && data[1] == 0;
    }
};

HBOOST_FORCEINLINE bool operator== (storage128_t const& left, storage128_t const& right) HBOOST_NOEXCEPT
{
    return left.data[0] == right.data[0] && left.data[1] == right.data[1];
}
HBOOST_FORCEINLINE bool operator!= (storage128_t const& left, storage128_t const& right) HBOOST_NOEXCEPT
{
    return !(left == right);
}

HBOOST_FORCEINLINE void non_atomic_load(storage128_t const volatile& from, storage128_t& to) HBOOST_NOEXCEPT
{
    to.data[0] = from.data[0];
    to.data[1] = from.data[1];
}

template< bool Signed >
struct make_storage_type< 16u, Signed >
{
    typedef storage128_t type;

    struct aligned
    {
        HBOOST_ALIGNMENT(16) type value;

        HBOOST_DEFAULTED_FUNCTION(aligned(), {})
        HBOOST_FORCEINLINE HBOOST_CONSTEXPR explicit aligned(type const& v) HBOOST_NOEXCEPT : value(v) {}
    };
};

#endif

template< typename T >
struct storage_size_of
{
    enum _
    {
        size = sizeof(T),
        value = (size == 3 ? 4 : (size >= 5 && size <= 7 ? 8 : (size >= 9 && size <= 15 ? 16 : size)))
    };
};

} // namespace detail
} // namespace atomics
} // namespace hboost

#endif // HBOOST_ATOMIC_DETAIL_STORAGE_TYPE_HPP_INCLUDED_
