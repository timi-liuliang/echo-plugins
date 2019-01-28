/*=============================================================================
    Copyright (c) 2014-2015 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_FUSION_AS_VECTOR_11052014_1801
#define HBOOST_FUSION_AS_VECTOR_11052014_1801

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/detail/config.hpp>

///////////////////////////////////////////////////////////////////////////////
// Without variadics, we will use the PP version
///////////////////////////////////////////////////////////////////////////////
#if !defined(HBOOST_FUSION_HAS_VARIADIC_VECTOR)
# include <hboost/fusion/container/vector/detail/cpp03/as_vector.hpp>
#else

///////////////////////////////////////////////////////////////////////////////
// C++11 interface
///////////////////////////////////////////////////////////////////////////////
#include <hboost/fusion/support/detail/index_sequence.hpp>
#include <hboost/fusion/container/vector/vector.hpp>
#include <hboost/fusion/iterator/value_of.hpp>
#include <hboost/fusion/iterator/deref.hpp>
#include <hboost/fusion/iterator/advance.hpp>
#include <cstddef>

namespace hboost { namespace fusion { namespace detail
{
HBOOST_FUSION_BARRIER_BEGIN

    template <typename Indices>
    struct as_vector_impl;

    template <std::size_t ...Indices>
    struct as_vector_impl<index_sequence<Indices...> >
    {
        template <typename Iterator>
        struct apply
        {
            typedef vector<
                typename result_of::value_of<
                    typename result_of::advance_c<Iterator, Indices>::type
                >::type...
            > type;
        };

        template <typename Iterator>
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        static typename apply<Iterator>::type
        call(Iterator i)
        {
            typedef typename apply<Iterator>::type result;
            return result(*advance_c<Indices>(i)...);
        }
    };

    template <int size>
    struct as_vector
        : as_vector_impl<typename make_index_sequence<size>::type> {};

HBOOST_FUSION_BARRIER_END
}}}

#endif
#endif


