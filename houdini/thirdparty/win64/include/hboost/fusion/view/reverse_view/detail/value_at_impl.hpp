/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HBOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_VALUE_AT_IMPL_HPP
#define HBOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_VALUE_AT_IMPL_HPP

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/sequence/intrinsic/value_at.hpp>
#include <hboost/mpl/minus.hpp>
#include <hboost/mpl/int.hpp>

namespace hboost { namespace fusion { namespace extension
{
    template <typename>
    struct value_at_impl;

    template <>
    struct value_at_impl<reverse_view_tag>
    {
        template <typename Seq, typename N>
        struct apply
          : result_of::value_at<
                typename Seq::seq_type
              , mpl::minus<typename Seq::size, mpl::int_<1>, N>
            >
        {};
    };
}}}

#endif
