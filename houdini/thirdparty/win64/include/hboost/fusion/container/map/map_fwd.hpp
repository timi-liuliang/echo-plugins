/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_MAP_FORWARD_MAIN_07212005_1105)
#define HBOOST_FUSION_MAP_FORWARD_MAIN_07212005_1105

#include <hboost/fusion/support/config.hpp>
#include <hboost/config.hpp>

#if (defined(HBOOST_NO_CXX11_DECLTYPE)             \
  || defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)   \
  || defined(HBOOST_NO_CXX11_RVALUE_REFERENCES))   \
  || (defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES))
# if defined(HBOOST_FUSION_HAS_VARIADIC_MAP)
#   undef HBOOST_FUSION_HAS_VARIADIC_MAP
# endif
#else
# if !defined(HBOOST_FUSION_HAS_VARIADIC_MAP)
#   define HBOOST_FUSION_HAS_VARIADIC_MAP
# endif
#endif

// MSVC variadics at this point in time is not ready yet (ICE!)
#if HBOOST_WORKAROUND(HBOOST_MSVC, HBOOST_TESTED_AT(1900))
# if defined(HBOOST_FUSION_HAS_VARIADIC_MAP)
#   undef HBOOST_FUSION_HAS_VARIADIC_MAP
# endif
#endif

///////////////////////////////////////////////////////////////////////////////
// With no decltype and variadics, we will use the C++03 version
///////////////////////////////////////////////////////////////////////////////
#if !defined(HBOOST_FUSION_HAS_VARIADIC_MAP)
# include <hboost/fusion/container/map/detail/cpp03/map_fwd.hpp>
#else

#include <hboost/fusion/container/map/detail/map_impl.hpp>

///////////////////////////////////////////////////////////////////////////////
// C++11 interface
///////////////////////////////////////////////////////////////////////////////
namespace hboost { namespace fusion
{
    template <typename ...T>
    struct map;
}}

#endif
#endif
