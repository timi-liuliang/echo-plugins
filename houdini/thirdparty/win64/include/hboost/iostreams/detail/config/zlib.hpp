// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2003-2007 Jonathan Turkanis
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

// Adapted from <hboost/config/auto_link.hpp> and from
// http://www.boost.org/more/separate_compilation.html, by John Maddock.

#ifndef HBOOST_IOSTREAMS_DETAIL_CONFIG_ZLIB_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_CONFIG_ZLIB_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <hboost/config.hpp> // HBOOST_STRINGIZE.

#if defined(HBOOST_ZLIB_BINARY)
# if defined(HBOOST_MSVC) || \
     defined(__BORLANDC__) || \
     (defined(__MWERKS__) && defined(_WIN32) && (__MWERKS__ >= 0x3000)) || \
     (defined(__ICL) && defined(_MSC_EXTENSIONS) && (_MSC_VER >= 1200)) \
     /**/

// Specify the name of the .lib file.
#  pragma comment(lib, HBOOST_STRINGIZE(HBOOST_ZLIB_BINARY))
# endif
#else 
# if !defined(HBOOST_IOSTREAMS_SOURCE) && \
     !defined(HBOOST_ALL_NO_LIB) && \
     !defined(HBOOST_IOSTREAMS_NO_LIB) \
     /**/

// Set the name of our library, this will get undef'ed by auto_link.hpp
// once it's done with it.
#  define HBOOST_LIB_NAME hboost_zlib

// If we're importing code from a dll, then tell auto_link.hpp about it.
#  if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_IOSTREAMS_DYN_LINK)
#   define HBOOST_DYN_LINK
#  endif

// And include the header that does the work.
#  include <hboost/config/auto_link.hpp>
# endif
#endif

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_CONFIG_ZLIB_HPP_INCLUDED
