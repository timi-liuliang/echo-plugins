/*=============================================================================
    Copyright (c) 2011 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(HBOOST_FUSION_SINGLE_VIEW_AT_IMPL_JUL_07_2011_1348PM)
#define HBOOST_FUSION_SINGLE_VIEW_AT_IMPL_JUL_07_2011_1348PM

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/int.hpp>
#include <hboost/mpl/assert.hpp>
#include <hboost/mpl/equal_to.hpp>

namespace hboost { namespace fusion 
{
    struct single_view_tag;

    namespace extension
    {
        template<typename Tag>
        struct at_impl;

        template<>
        struct at_impl<single_view_tag>
        {
            template<typename Sequence, typename N>
            struct apply
            {
                HBOOST_MPL_ASSERT((mpl::equal_to<N, mpl::int_<0> >));
                typedef typename Sequence::value_type type;

                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type 
                call(Sequence& seq)
                {
                    return seq.val;
                }
            };
        };
    }

}}

#endif
