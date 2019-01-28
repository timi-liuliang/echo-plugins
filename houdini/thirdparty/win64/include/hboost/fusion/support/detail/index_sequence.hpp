/*=============================================================================
    Copyright (c) 2015 Agustin K-ballo Berge
    Copyright (c) 2015 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HBOOST_FUSION_SUPPORT_DETAIL_INDEX_SEQUENCE_06232015_1038
#define HBOOST_FUSION_SUPPORT_DETAIL_INDEX_SEQUENCE_06232015_1038

#include <hboost/fusion/support/config.hpp>
#include <cstddef>

// GCC5 has O(logN) implementation, see https://gcc.gnu.org/PR66059 .
#if (defined(__cpp_lib_integer_sequence) && __cpp_lib_integer_sequence >= 201304) \
 || (defined(HBOOST_LIBSTDCXX_VERSION) \
     && HBOOST_LIBSTDCXX_VERSION >= 500000 && __cplusplus >= 201402)
#include <utility>
#define HBOOST_FUSION_STDLIB_HAS_INTEGER_SEQUENCE
#endif

namespace hboost { namespace fusion { namespace detail
{
#ifdef HBOOST_FUSION_STDLIB_HAS_INTEGER_SEQUENCE
    // Use aliasing templates without checking availability, the compiler should work.
    template <std::size_t ...Ints>
    using index_sequence = std::index_sequence<Ints...>;

    template <std::size_t N>
    struct make_index_sequence
    {
        using type = std::make_index_sequence<N>;
    };
#else
    template <std::size_t ...Ints>
    struct index_sequence
    {
        typedef std::size_t value_type;

        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static std::size_t size() HBOOST_NOEXCEPT
        { return sizeof...(Ints); }

        // non standard extension
        typedef index_sequence type;
    };

    template <typename Left, typename Right>
    struct _make_index_sequence_join;

    template <std::size_t... Left, std::size_t... Right>
    struct _make_index_sequence_join<
        index_sequence<Left...>, index_sequence<Right...>
    > : index_sequence<Left..., (sizeof...(Left) + Right)...>
    {};

    template <std::size_t N>
    struct make_index_sequence
      : _make_index_sequence_join<
            typename make_index_sequence<N / 2>::type
          , typename make_index_sequence<N - N / 2>::type
        >
    {};

    template <>
    struct make_index_sequence<1>
      : index_sequence<0>
    {};

    template <>
    struct make_index_sequence<0>
      : index_sequence<>
    {};
#endif
}}}

#endif

