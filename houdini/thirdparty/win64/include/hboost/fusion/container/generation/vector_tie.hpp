/*=============================================================================
    Copyright (c) 2014 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_FUSION_VECTOR_TIE_11112014_2302
#define HBOOST_FUSION_VECTOR_TIE_11112014_2302

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/detail/config.hpp>

#if !defined(HBOOST_FUSION_HAS_VARIADIC_VECTOR)
# include <hboost/fusion/container/generation/detail/pp_vector_tie.hpp>
#else

///////////////////////////////////////////////////////////////////////////////
// C++11 variadic interface
///////////////////////////////////////////////////////////////////////////////

namespace hboost { namespace fusion
{
    namespace result_of
    {
        template <typename ...T>
        struct vector_tie
        {
            typedef vector<T&...> type;
        };
    }

    template <typename ...T>
    HBOOST_CONSTEXPR HBOOST_FUSION_GPU_ENABLED
    inline vector<T&...>
    vector_tie(T&... arg)
    {
        return vector<T&...>(arg...);
    }
 }}


#endif
#endif

