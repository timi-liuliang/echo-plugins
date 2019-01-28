/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef HBOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_VALUE_OF_DATA_IMPL_HPP
#define HBOOST_FUSION_VIEW_REVERSE_VIEW_DETAIL_VALUE_OF_DATA_IMPL_HPP

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/iterator/value_of_data.hpp>

namespace hboost { namespace fusion { namespace extension
{
    template <typename>
    struct value_of_data_impl;

    template <>
    struct value_of_data_impl<reverse_view_iterator_tag>
    {
        template <typename It>
        struct apply
          : result_of::value_of_data<typename It::first_type>
        {};
    };
}}}

#endif