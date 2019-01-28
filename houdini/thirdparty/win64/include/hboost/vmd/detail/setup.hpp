
//  (C) Copyright Edward Diener 2011-2015
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#if !defined(HBOOST_VMD_DETAIL_SETUP_HPP)
#define HBOOST_VMD_DETAIL_SETUP_HPP

#include <hboost/preprocessor/config/config.hpp>

#if defined(HBOOST_VMD_MSVC)
#undef HBOOST_VMD_MSVC
#endif
#if defined(HBOOST_VMD_MSVC_V8)
#undef HBOOST_VMD_MSVC_V8
#endif
#if HBOOST_PP_VARIADICS
#define HBOOST_VMD_MSVC HBOOST_PP_VARIADICS_MSVC
#if HBOOST_VMD_MSVC && defined(_MSC_VER) && _MSC_VER == 1400
#define HBOOST_VMD_MSVC_V8 1
#else
#define HBOOST_VMD_MSVC_V8 0
#endif /* HBOOST_VMD_MSVC  && defined(_MSC_VER) && _MSC_VER == 1400 */
#if !defined(HBOOST_VMD_ASSERT_DATA)
#if defined(NDEBUG)
#define HBOOST_VMD_ASSERT_DATA 0
#else
#define HBOOST_VMD_ASSERT_DATA 1
#endif /* NDEBUG */
#endif /* HBOOST_VMD_ASSERT_DATA */
#else
#define HBOOST_VMD_MSVC 0
#define HBOOST_VMD_MSVC_V8 0
#if defined(HBOOST_VMD_ASSERT_DATA)
#undef HBOOST_VMD_ASSERT_DATA
#endif
#define HBOOST_VMD_ASSERT_DATA 0
#endif /* HBOOST_PP_VARIADICS */

#endif /* HBOOST_VMD_DETAIL_SETUP_HPP */
