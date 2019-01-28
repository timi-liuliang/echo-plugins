/*=============================================================================
    Copyright (c) 2014 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_FUSION_VECTOR10_11052014_2316
#define HBOOST_FUSION_VECTOR10_11052014_2316

#include <hboost/config.hpp>
#include <hboost/fusion/support/config.hpp>
#include <hboost/fusion/container/vector/detail/config.hpp>

///////////////////////////////////////////////////////////////////////////////
// Without variadics, we will use the PP version
///////////////////////////////////////////////////////////////////////////////
#if !defined(HBOOST_FUSION_HAS_VARIADIC_VECTOR)
#   include <hboost/fusion/container/vector/detail/cpp03/vector10.hpp>
#else

///////////////////////////////////////////////////////////////////////////////
// C++11 interface
///////////////////////////////////////////////////////////////////////////////
#include <hboost/fusion/container/vector/vector_fwd.hpp>
#include <hboost/fusion/container/vector/vector.hpp>

#endif
#endif

