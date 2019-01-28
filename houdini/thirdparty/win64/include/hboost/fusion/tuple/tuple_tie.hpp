/*=============================================================================
    Copyright (c) 2014 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_FUSION_TUPLE_TIE_14122014_0115
#define HBOOST_FUSION_TUPLE_TIE_14122014_0115

#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/tuple/tuple_fwd.hpp>

///////////////////////////////////////////////////////////////////////////////
// With no variadics, we will use the C++03 version
///////////////////////////////////////////////////////////////////////////////
#if !defined(HBOOST_FUSION_HAS_VARIADIC_TUPLE)
# include <hboost/fusion/tuple/detail/tuple_tie.hpp>
#else

///////////////////////////////////////////////////////////////////////////////
// C++11 interface
///////////////////////////////////////////////////////////////////////////////
#include <hboost/fusion/tuple/tuple.hpp>

namespace hboost { namespace fusion
{
    template <typename ...T>
    HBOOST_FUSION_GPU_ENABLED
    inline tuple<T&...>
    tie(T&... arg)
    {
        return tuple<T&...>(arg...);
    }
}}

#endif
#endif

