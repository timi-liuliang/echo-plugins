/*=============================================================================
    Copyright (c) 2014 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_FUSION_SET_FORWARD_11062014_1720
#define HBOOST_FUSION_SET_FORWARD_11062014_1720

#include <hboost/config.hpp>
#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/detail/config.hpp>

#if !defined(HBOOST_FUSION_HAS_VARIADIC_VECTOR) \
  || (defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES))
# if defined(HBOOST_FUSION_HAS_VARIADIC_SET)
#   undef HBOOST_FUSION_HAS_VARIADIC_SET
# endif
#else
# if !defined(HBOOST_FUSION_HAS_VARIADIC_SET)
#   define HBOOST_FUSION_HAS_VARIADIC_SET
# endif
#endif

///////////////////////////////////////////////////////////////////////////////
// With no variadics, we will use the C++03 version
///////////////////////////////////////////////////////////////////////////////
#if !defined(HBOOST_FUSION_HAS_VARIADIC_SET)
# include <hboost/fusion/container/set/detail/cpp03/set_fwd.hpp>
#else

///////////////////////////////////////////////////////////////////////////////
// C++11 interface
///////////////////////////////////////////////////////////////////////////////
namespace hboost { namespace fusion
{
    struct set_tag;
    struct set_iterator_tag;

    template <typename ...T>
    struct set;
}}

#endif
#endif

