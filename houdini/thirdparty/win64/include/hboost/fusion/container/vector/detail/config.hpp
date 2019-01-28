/*=============================================================================
    Copyright (c) 2014-2015 Kohei Takahashi

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef HBOOST_FUSION_VECTOR_CONFIG_11052014_1720
#define HBOOST_FUSION_VECTOR_CONFIG_11052014_1720

#include <hboost/config.hpp>
#include <hboost/detail/workaround.hpp>
#include <hboost/fusion/support/config.hpp>

#if (defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES) \
  || defined(HBOOST_NO_CXX11_RVALUE_REFERENCES) \
  || defined(HBOOST_NO_CXX11_TEMPLATE_ALIASES)  \
  || defined(HBOOST_NO_CXX11_DECLTYPE)) \
  || (defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES))
# if defined(HBOOST_FUSION_HAS_VARIADIC_VECTOR)
#   undef HBOOST_FUSION_HAS_VARIADIC_VECTOR
# endif
#else
# if !defined(HBOOST_FUSION_HAS_VARIADIC_VECTOR)
#   define HBOOST_FUSION_HAS_VARIADIC_VECTOR
# endif
#endif

// Sometimes, MSVC 12 shows compile error with std::size_t of template parameter.
#if HBOOST_WORKAROUND(HBOOST_MSVC, HBOOST_TESTED_AT(1800))
# if defined(HBOOST_FUSION_HAS_VARIADIC_VECTOR)
#   undef HBOOST_FUSION_HAS_VARIADIC_VECTOR
# endif
#endif

#endif

