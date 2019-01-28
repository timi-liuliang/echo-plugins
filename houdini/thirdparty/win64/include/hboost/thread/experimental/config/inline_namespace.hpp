#ifndef HBOOST_THREAD_EXPERIMENTAL_CONFIG_INLINE_NAMESPACE_HPP
#define HBOOST_THREAD_EXPERIMENTAL_CONFIG_INLINE_NAMESPACE_HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/thread for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <hboost/config.hpp>

#if !defined(HBOOST_NO_CXX11_INLINE_NAMESPACES)
# define HBOOST_THREAD_INLINE_NAMESPACE(name) inline namespace name
#else
# define HBOOST_THREAD_INLINE_NAMESPACE(name) namespace name
#endif


#endif
