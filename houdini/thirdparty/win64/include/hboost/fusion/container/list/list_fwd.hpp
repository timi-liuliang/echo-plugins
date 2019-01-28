/*=============================================================================
    Copyright (c) 2014 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_FUSION_LIST_FORWARD_10262014_0528
#define HBOOST_FUSION_LIST_FORWARD_10262014_0528

#include <hboost/fusion/support/config.hpp>
#include <hboost/config.hpp>

#if  defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) \
  || (defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES))
# if defined(HBOOST_FUSION_HAS_VARIADIC_LIST)
#   undef HBOOST_FUSION_HAS_VARIADIC_LIST
# endif
#else
# if !defined(HBOOST_FUSION_HAS_VARIADIC_LIST)
#   define HBOOST_FUSION_HAS_VARIADIC_LIST
# endif
#endif

///////////////////////////////////////////////////////////////////////////////
// With no variadics, we will use the C++03 version
///////////////////////////////////////////////////////////////////////////////
#if !defined(HBOOST_FUSION_HAS_VARIADIC_LIST)
# include <hboost/fusion/container/list/detail/cpp03/list_fwd.hpp>
#else

///////////////////////////////////////////////////////////////////////////////
// C++11 interface
///////////////////////////////////////////////////////////////////////////////
namespace hboost { namespace fusion
{
    struct void_;

    template <typename ...T>
    struct list;
}}

#endif
#endif
