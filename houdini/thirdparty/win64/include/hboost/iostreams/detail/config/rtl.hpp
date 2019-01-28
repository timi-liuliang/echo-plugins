/*
 * Distributed under the Boost Software License, Version 1.0.(See accompanying 
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)
 * 
 * See http://www.boost.org/libs/iostreams for documentation.
 *
 * Defines preprocessor symbols expanding to the names of functions in the 
 * C runtime library used to access file descriptors and to the type used
 * to store file offsets for seeking.
 * 
 * File:        hboost/iostreams/detail/config/rtl.hpp
 * Date:        Wed Dec 26 11:58:11 MST 2007
 * 
 * Copyright:   2007-2008 CodeRage, LLC
 * Author:      Jonathan Turkanis
 * Contact:     turkanis at coderage dot com
 */

#ifndef HBOOST_IOSTREAMS_DETAIL_CONFIG_RTL_HPP_INCLUDED
#define HBOOST_IOSTREAMS_DETAIL_CONFIG_RTL_HPP_INCLUDED

#include <hboost/config.hpp>
#include <hboost/iostreams/detail/config/windows_posix.hpp>

// Handle open, close, read, and write
#ifdef __BORLANDC__
# define HBOOST_IOSTREAMS_RTL(x) HBOOST_JOIN(_rtl_, x)
#elif defined HBOOST_IOSTREAMS_WINDOWS
# define HBOOST_IOSTREAMS_RTL(x) HBOOST_JOIN(_, x)
#else
# define HBOOST_IOSTREAMS_RTL(x) ::x  // Distinguish from member function named x
#endif
#define HBOOST_IOSTREAMS_FD_OPEN   HBOOST_IOSTREAMS_RTL(open)
#define HBOOST_IOSTREAMS_FD_CLOSE  HBOOST_IOSTREAMS_RTL(close)
#define HBOOST_IOSTREAMS_FD_READ   HBOOST_IOSTREAMS_RTL(read)
#define HBOOST_IOSTREAMS_FD_WRITE  HBOOST_IOSTREAMS_RTL(write)

// Handle lseek, off_t, ftruncate, and stat
#ifdef HBOOST_IOSTREAMS_WINDOWS
# if defined(HBOOST_MSVC) || defined(__MSVCRT__) // MSVC, MinGW
#  define HBOOST_IOSTREAMS_FD_SEEK    _lseeki64
#  define HBOOST_IOSTREAMS_FD_OFFSET  __int64
# else                                          // Borland, Metrowerks, ...
#  define HBOOST_IOSTREAMS_FD_SEEK    lseek  
#  define HBOOST_IOSTREAMS_FD_OFFSET  long
# endif
#else // Non-windows
# if defined(_LARGEFILE64_SOURCE) && !defined(__APPLE__) && \
         (!defined(_FILE_OFFSET_BITS) || _FILE_OFFSET_BITS != 64) || \
     defined(_AIX) && !defined(_LARGE_FILES) || \
     defined(HBOOST_IOSTREAMS_HAS_LARGE_FILE_EXTENSIONS)
     /**/

    /* Systems with transitional extensions for large file support */

#  define HBOOST_IOSTREAMS_FD_SEEK      lseek64
#  define HBOOST_IOSTREAMS_FD_TRUNCATE  ftruncate64
#  define HBOOST_IOSTREAMS_FD_MMAP      mmap64
#  define HBOOST_IOSTREAMS_FD_STAT      stat64
#  define HBOOST_IOSTREAMS_FD_FSTAT     fstat64
#  define HBOOST_IOSTREAMS_FD_OFFSET    off64_t
# else
#  define HBOOST_IOSTREAMS_FD_SEEK      lseek
#  define HBOOST_IOSTREAMS_FD_TRUNCATE  ftruncate
#  define HBOOST_IOSTREAMS_FD_MMAP      mmap
#  define HBOOST_IOSTREAMS_FD_STAT      stat
#  define HBOOST_IOSTREAMS_FD_FSTAT     fstat
#  define HBOOST_IOSTREAMS_FD_OFFSET    off_t
# endif
#endif

#endif // #ifndef HBOOST_IOSTREAMS_DETAIL_CONFIG_RTL_HPP_INCLUDED
