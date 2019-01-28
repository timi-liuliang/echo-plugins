// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

// Adapted from <hboost/config/auto_link.hpp> and from
// http://www.boost.org/more/separate_compilation.html, by John Maddock.

#ifndef HBOOST_IOSTREAMS_DETAIL_CONFIG_BROKEN_OVERLOAD_RESOLUTION_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_CONFIG_BROKEN_OVERLOAD_RESOLUTION_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif             

#include <hboost/config.hpp> // HBOOST_MSVC.
#include <hboost/detail/workaround.hpp>
#include <hboost/iostreams/detail/config/gcc.hpp>

#if !defined(HBOOST_IOSTREAMS_BROKEN_OVERLOAD_RESOLUTION)
# if HBOOST_WORKAROUND(__MWERKS__, <= 0x3003) || \
     HBOOST_WORKAROUND(__BORLANDC__, < 0x600) || \
     HBOOST_WORKAROUND(HBOOST_MSVC, <= 1300) || \
     HBOOST_WORKAROUND(HBOOST_IOSTREAMS_GCC, <= 295) \
     /**/
#  define HBOOST_IOSTREAMS_BROKEN_OVERLOAD_RESOLUTION
# endif
#endif

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_CONFIG_BROKEN_OVERLOAD_RESOLUTION_HPP_INCLUDED
