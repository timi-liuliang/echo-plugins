/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_BEGIN_IMPL_31122005_1209)
#define HBOOST_FUSION_BEGIN_IMPL_31122005_1209

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/adapted/mpl/mpl_iterator.hpp>
#include <hboost/mpl/begin.hpp>
#include <hboost/type_traits/remove_const.hpp>

namespace hboost { namespace fusion {
    
    struct mpl_sequence_tag;

    namespace extension
    {
        template <typename Tag>
        struct begin_impl;

        template <>
        struct begin_impl<mpl_sequence_tag>
        {
            template <typename Sequence>
            struct apply 
            {
                typedef typename mpl::begin<
                    typename remove_const<Sequence>::type
                >::type iterator;
                typedef mpl_iterator<iterator> type;
                
                HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
                static type
                call(Sequence)
                {
                    return type();
                }
            };
        };
    }
}}

#endif
