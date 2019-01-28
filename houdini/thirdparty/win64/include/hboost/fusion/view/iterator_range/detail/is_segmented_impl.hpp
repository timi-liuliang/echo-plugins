/*=============================================================================
    Copyright (c) 2011 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_ITERATOR_RANGE_IS_SEGMENTED_HPP_INCLUDED)
#define HBOOST_FUSION_ITERATOR_RANGE_IS_SEGMENTED_HPP_INCLUDED

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/assert.hpp>
#include <hboost/mpl/bool.hpp>

namespace hboost { namespace fusion
{
    struct iterator_range_tag;

    template <typename Context>
    struct segmented_iterator;

    namespace extension
    {
        template <typename Tag>
        struct is_segmented_impl;

        // An iterator_range of segmented_iterators is segmented
        template <>
        struct is_segmented_impl<iterator_range_tag>
        {
        private:
            template <typename Iterator>
            struct is_segmented_iterator
              : mpl::false_
            {};

            template <typename Iterator>
            struct is_segmented_iterator<Iterator &>
              : is_segmented_iterator<Iterator>
            {};

            template <typename Iterator>
            struct is_segmented_iterator<Iterator const>
              : is_segmented_iterator<Iterator>
            {};

            template <typename Context>
            struct is_segmented_iterator<segmented_iterator<Context> >
              : mpl::true_
            {};

        public:
            template <typename Sequence>
            struct apply
              : is_segmented_iterator<typename Sequence::begin_type>
            {
                HBOOST_MPL_ASSERT_RELATION(
                    is_segmented_iterator<typename Sequence::begin_type>::value
                  , ==
                  , is_segmented_iterator<typename Sequence::end_type>::value);
            };
        };
    }
}}

#endif

