/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_CLEAR_10022005_1817)
#define HBOOST_FUSION_CLEAR_10022005_1817

#include <hboost/fusion/support/config.hpp>
#include <hboost/mpl/clear.hpp>
#include <hboost/fusion/support/tag_of.hpp>
#include <hboost/fusion/mpl/detail/clear.hpp>

namespace hboost { namespace mpl
{
    template <typename Tag>
    struct clear_impl;

    template <>
    struct clear_impl<fusion::fusion_sequence_tag>
    {
        template <typename Sequence>
        struct apply
        {            
            typedef typename 
                fusion::detail::clear<typename fusion::detail::tag_of<Sequence>::type>::type 
            type;
        };
    };
}}

#endif

