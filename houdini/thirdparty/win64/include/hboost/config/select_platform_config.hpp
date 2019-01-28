//  Boost compiler configuration selection header file

//  (C) Copyright John Maddock 2001 - 2002. 
//  (C) Copyright Jens Maurer 2001. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version.

// locate which platform we are on and define HBOOST_PLATFORM_CONFIG as needed.
// Note that we define the headers to include using "header_name" not
// <header_name> in order to prevent macro expansion within the header
// name (for example "linux" is a macro on linux systems).

#if (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__)) && !defined(_CRAYC)
// linux, also other platforms (Hurd etc) that use GLIBC, should these really have their own config headers though?
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/linux.hpp"

#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
// BSD:
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/bsd.hpp"

#elif defined(sun) || defined(__sun)
// solaris:
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/solaris.hpp"

#elif defined(__sgi)
// SGI Irix:
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/irix.hpp"

#elif defined(__hpux)
// hp unix:
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/hpux.hpp"

#elif defined(__CYGWIN__)
// cygwin is not win32:
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/cygwin.hpp"

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
// win32:
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/win32.hpp"

#elif defined(__HAIKU__)
// Haiku
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/haiku.hpp"

#elif defined(__BEOS__)
// BeOS
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/beos.hpp"

#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
// MacOS
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/macos.hpp"

#elif defined(__IBMCPP__) || defined(_AIX)
// IBM
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/aix.hpp"

#elif defined(__amigaos__)
// AmigaOS
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/amigaos.hpp"

#elif defined(__QNXNTO__)
// QNX:
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/qnxnto.hpp"

#elif defined(__VXWORKS__)
// vxWorks:
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/vxworks.hpp"

#elif defined(__SYMBIAN32__) 
// Symbian: 
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/symbian.hpp" 

#elif defined(_CRAYC)
// Cray:
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/cray.hpp" 

#elif defined(__VMS) 
// VMS:
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/vms.hpp" 

#elif defined(__CloudABI__)
// Nuxi CloudABI:
#  define HBOOST_PLATFORM_CONFIG "hboost/config/platform/cloudabi.hpp"
#else

#  if defined(unix) \
      || defined(__unix) \
      || defined(_XOPEN_SOURCE) \
      || defined(_POSIX_SOURCE)

   // generic unix platform:

#  ifndef HBOOST_HAS_UNISTD_H
#     define HBOOST_HAS_UNISTD_H
#  endif

#  include <hboost/config/posix_features.hpp>

#  endif

#  if defined (HBOOST_ASSERT_CONFIG)
      // this must come last - generate an error if we don't
      // recognise the platform:
#     error "Unknown platform - please configure and report the results to boost.org"
#  endif

#endif

#if 0
//
// This section allows dependency scanners to find all the files we *might* include:
//
#  include "hboost/config/platform/linux.hpp"
#  include "hboost/config/platform/bsd.hpp"
#  include "hboost/config/platform/solaris.hpp"
#  include "hboost/config/platform/irix.hpp"
#  include "hboost/config/platform/hpux.hpp"
#  include "hboost/config/platform/cygwin.hpp"
#  include "hboost/config/platform/win32.hpp"
#  include "hboost/config/platform/beos.hpp"
#  include "hboost/config/platform/macos.hpp"
#  include "hboost/config/platform/aix.hpp"
#  include "hboost/config/platform/amigaos.hpp"
#  include "hboost/config/platform/qnxnto.hpp"
#  include "hboost/config/platform/vxworks.hpp"
#  include "hboost/config/platform/symbian.hpp" 
#  include "hboost/config/platform/cray.hpp" 
#  include "hboost/config/platform/vms.hpp" 
#  include <hboost/config/posix_features.hpp>



#endif

