/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2011 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_SINGLE_VIEW_DEREF_IMPL_05052005_0258)
#define HBOOST_FUSION_SINGLE_VIEW_DEREF_IMPL_05052005_0258

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/int.hpp>
#include <hboost/mpl/assert.hpp>
#include <hboost/mpl/equal_to.hpp>

namespace hboost { namespace fusion
{
    struct single_view_iterator_tag;

    namespace extension
    {
        template <typename Tag>
        struct deref_impl;

        template <>
        struct deref_impl<single_view_iterator_tag>
        {
            template <typename Iterator>
            struct apply
            {
                HBOOST_MPL_ASSERT((mpl::equal_to<typename Iterator::position, mpl::int_<0> >));
                typedef typename Iterator::value_type type;
    
                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type
                call(Iterator const& i)
                {
                    return i.view.val;
                }
            };
        };
    }
}}

#endif


