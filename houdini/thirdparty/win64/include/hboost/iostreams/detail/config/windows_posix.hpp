// (C) Copyright 2008 CodeRage, LLC (turkanis at coderage dot com)
// (C) Copyright 2004-2007 Jonathan Turkanis
// (C) Copyright 2002, 2003 Beman Dawes   Boost.Filesystem
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// See http://www.boost.org/libs/iostreams for documentation.

#ifndef HBOOST_IOSTREAMS_DETAIL_CONFIG_WINDOWS_POSIX_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_CONFIG_WINDOWS_POSIX_HPP_INCLUDED

//------------------From hboost/libs/filesystem/src/path_posix_windows.cpp-----//

// HBOOST_IOSTREAMS_POSIX or HBOOST_IOSTREAMS_WINDOWS specify which API to use.
#if !defined( HBOOST_IOSTREAMS_WINDOWS ) && !defined( HBOOST_IOSTREAMS_POSIX )
# if (defined(_WIN32) || defined(__WIN32__) || defined(WIN32)) && \
     !defined(__CYGWIN__) \
     /**/
#  define HBOOST_IOSTREAMS_WINDOWS
# else
#  define HBOOST_IOSTREAMS_POSIX
# endif
#endif

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_CONFIG_WINDOWS_POSIX_HPP_INCLUDED
