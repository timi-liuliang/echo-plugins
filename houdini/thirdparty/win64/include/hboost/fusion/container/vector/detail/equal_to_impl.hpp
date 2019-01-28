/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_EQUAL_TO_IMPL_05052005_1215)
#define HBOOST_FUSION_EQUAL_TO_IMPL_05052005_1215

#include <hboost/fusion/support/config.hpp>
#include <hboost/type_traits/is_same.hpp>
#include <hboost/mpl/equal_to.hpp>
#include <hboost/mpl/and.hpp>

namespace hboost { namespace fusion
{
    struct vector_iterator_tag;

    namespace extension
    {
        template <typename Tag>
        struct equal_to_impl;

        template <>
        struct equal_to_impl<vector_iterator_tag>
        {
            template <typename I1, typename I2>
            struct apply
                : is_same<
                    typename I1::identity
                  , typename I2::identity
                >
            {
            };
        };
    }
}}

#endif

