/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_ITERATOR_BASE_05042005_1008)
#define HBOOST_FUSION_ITERATOR_BASE_05042005_1008

#include <hboost/config.hpp>
#include <hboost/fusion/support/config.hpp>

namespace hboost { namespace fusion
{
    struct iterator_root {};

    template <typename Iterator>
    struct iterator_base : iterator_root
    {
        HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        Iterator const&
        cast() const HBOOST_NOEXCEPT
        {
            return static_cast<Iterator const&>(*this);
        }

        HBOOST_CXX14_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
        Iterator&
        cast() HBOOST_NOEXCEPT
        {
            return static_cast<Iterator&>(*this);
        }
    };
}}

#endif
