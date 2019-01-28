/*=============================================================================
    Copyright (c) 2014-2015 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_FUSION_TUPLE_FORWARD_14122014_0051
#define HBOOST_FUSION_TUPLE_FORWARD_14122014_0051

#include <hboost/config.hpp>
#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/detail/config.hpp>

#if  !defined(HBOOST_FUSION_HAS_VARIADIC_VECTOR) \
  || (defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES))
# if defined(HBOOST_FUSION_HAS_VARIADIC_TUPLE)
#   undef HBOOST_FUSION_HAS_VARIADIC_TUPLE
# endif
#else
# if !defined(HBOOST_FUSION_HAS_VARIADIC_TUPLE)
#   define HBOOST_FUSION_HAS_VARIADIC_TUPLE
# endif
#endif

///////////////////////////////////////////////////////////////////////////////
// With no variadics, we will use the C++03 version
///////////////////////////////////////////////////////////////////////////////
#if !defined(HBOOST_FUSION_HAS_VARIADIC_TUPLE)
# include <hboost/fusion/tuple/detail/tuple_fwd.hpp>
#else

///////////////////////////////////////////////////////////////////////////////
// C++11 interface
///////////////////////////////////////////////////////////////////////////////
namespace hboost { namespace fusion
{
    template <typename ...T>
    struct tuple;
}}

#endif
#endif

