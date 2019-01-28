/*=============================================================================
    Copyright (c) 2005-2012 Joel de Guzman
    Copyright (c) 2005-2007 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(HBOOST_FUSION_DEQUE_FORWARD_02092007_0749)
#define HBOOST_FUSION_DEQUE_FORWARD_02092007_0749

#include <hboost/fusion/support/config.hpp>
#include <hboost/config.hpp>

#if (defined(HBOOST_NO_CXX11_VARIADIC_TEMPLATES)   \
  || defined(HBOOST_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS))   \
  || (defined(__WAVE__) && defined(HBOOST_FUSION_CREATE_PREPROCESSED_FILES))
# if defined(HBOOST_FUSION_HAS_VARIADIC_DEQUE)
#   undef HBOOST_FUSION_HAS_VARIADIC_DEQUE
# endif
#else
# if !defined(HBOOST_FUSION_HAS_VARIADIC_DEQUE)
#   define HBOOST_FUSION_HAS_VARIADIC_DEQUE
# endif
#endif

// MSVC variadics at this point in time is not ready yet (ICE!)
#if HBOOST_WORKAROUND(HBOOST_MSVC, HBOOST_TESTED_AT(1900))
# if defined(HBOOST_FUSION_HAS_VARIADIC_DEQUE)
#   undef HBOOST_FUSION_HAS_VARIADIC_DEQUE
# endif
#endif

///////////////////////////////////////////////////////////////////////////////
// With no variadics, we will use the C++03 version
///////////////////////////////////////////////////////////////////////////////
#if !defined(HBOOST_FUSION_HAS_VARIADIC_DEQUE)
# include <hboost/fusion/container/deque/detail/cpp03/deque_fwd.hpp>
#else

///////////////////////////////////////////////////////////////////////////////
// C++11 interface
///////////////////////////////////////////////////////////////////////////////
namespace hboost { namespace fusion
{
    template <typename ...T>
    struct deque;
}}

#endif
#endif
