/*=============================================================================
    Copyright (c) 2011 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#if !defined(HBOOST_PHOENIX_PREPROCESSED_SUPPORT_VECTOR_HPP)
#define HBOOST_PHOENIX_PREPROCESSED_SUPPORT_VECTOR_HPP

#if HBOOST_PHOENIX_LIMIT <= 10
#include <hboost/phoenix/support/preprocessed/vector_10.hpp>
#elif HBOOST_PHOENIX_LIMIT <= 20
#include <hboost/phoenix/support/preprocessed/vector_20.hpp>
#elif HBOOST_PHOENIX_LIMIT <= 30
#include <hboost/phoenix/support/preprocessed/vector_30.hpp>
#elif HBOOST_PHOENIX_LIMIT <= 40
#include <hboost/phoenix/support/preprocessed/vector_40.hpp>
#elif HBOOST_PHOENIX_LIMIT <= 50
#include <hboost/phoenix/support/preprocessed/vector_50.hpp>
#else
#error "HBOOST_PHOENIX_LIMIT out of bounds for preprocessed headers"
#endif

#endif
