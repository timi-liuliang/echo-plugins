/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2011 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_SINGLE_VIEW_NEXT_IMPL_05052005_0331)
#define HBOOST_FUSION_SINGLE_VIEW_NEXT_IMPL_05052005_0331

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/next.hpp>
#include <hboost/static_assert.hpp>

namespace hboost { namespace fusion
{
    struct single_view_iterator_tag;

    template <typename SingleView, typename Pos>
    struct single_view_iterator;

    namespace extension
    {
        template <typename Tag>
        struct next_impl;

        template <>
        struct next_impl<single_view_iterator_tag>
        {
            template <typename Iterator>
            struct apply
            {
                typedef single_view_iterator<
                    typename Iterator::single_view_type,
                    typename mpl::next<typename Iterator::position>::type>
                type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type
                call(Iterator const& i)
                {
                    // Workaround for ICE on GCC 4.0.0.
                    // see https://svn.boost.org/trac/hboost/ticket/5808
                    typedef typename type::position position;
                    HBOOST_STATIC_ASSERT((position::value < 2));
                    return type(i.view);
                }
            };
        };
    }
}}

#endif


