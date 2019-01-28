/*=============================================================================
    Copyright (c) 2011 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_SEGMENTED_ITERATOR_EQUAL_TO_HPP_INCLUDED)
#define HBOOST_FUSION_SEGMENTED_ITERATOR_EQUAL_TO_HPP_INCLUDED

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/and.hpp>
#include <hboost/mpl/bool.hpp>
#include <hboost/fusion/iterator/equal_to.hpp>

namespace hboost { namespace fusion
{
    struct nil_;

    namespace detail
    {
        template <typename Stack1, typename Stack2>
        struct segmented_equal_to
          : mpl::and_<
                segmented_equal_to<
                    typename Stack1::cdr_type,
                    typename Stack2::cdr_type
                >
              , result_of::equal_to<
                    typename Stack1::car_type::begin_type,
                    typename Stack2::car_type::begin_type
                >
            >
        {};

        template <>
        struct segmented_equal_to<fusion::nil_, fusion::nil_>
          : mpl::true_
        {};
    }
}}

#endif
