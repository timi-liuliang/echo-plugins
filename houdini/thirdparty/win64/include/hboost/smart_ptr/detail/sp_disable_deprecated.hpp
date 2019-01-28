#ifndef HBOOST_SMART_PTR_DETAIL_SP_DISABLE_DEPRECATED_HPP_INCLUDED
#define HBOOST_SMART_PTR_DETAIL_SP_DISABLE_DEPRECATED_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  hboost/smart_ptr/detail/sp_disable_deprecated.hpp
//
//  Copyright 2015 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <hboost/config.hpp>

#if defined( __GNUC__ ) && ( defined( __GXX_EXPERIMENTAL_CXX0X__ ) || ( __cplusplus >= 201103L ) )

# if defined( HBOOST_GCC )

#  if HBOOST_GCC >= 40600
#   define HBOOST_SP_DISABLE_DEPRECATED
#  endif

# elif defined( __clang__ ) && defined( __has_warning )

#  if __has_warning( "-Wdeprecated-declarations" )
#   define HBOOST_SP_DISABLE_DEPRECATED
#  endif

# endif

#endif

#endif // #ifndef HBOOST_SMART_PTR_DETAIL_SP_DISABLE_DEPRECATED_HPP_INCLUDED
