// Boost.Bimap
//
// Copyright (c) 2006-2007 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file detail/debug/static_error.hpp
/// \brief Formatted compile time error

#ifndef HBOOST_BIMAP_DETAIL_DEBUG_STATIC_ERROR_HPP
#define HBOOST_BIMAP_DETAIL_DEBUG_STATIC_ERROR_HPP

#if defined(_MSC_VER)
#pragma once
#endif

#include <hboost/config.hpp>

#include <hboost/mpl/assert.hpp>
#include <hboost/preprocessor/cat.hpp>

// Easier way to call HBOOST_MPL_ASSERT_MSG in class scope to generate
// a static error.
/*===========================================================================*/
#define HBOOST_BIMAP_STATIC_ERROR(MESSAGE,VARIABLES)                           \
        HBOOST_MPL_ASSERT_MSG(false,                                           \
                             HBOOST_PP_CAT(BIMAP_STATIC_ERROR__,MESSAGE),      \
                             VARIABLES)
/*===========================================================================*/



#endif // HBOOST_BIMAP_DETAIL_DEBUG_STATIC_ERROR_HPP
