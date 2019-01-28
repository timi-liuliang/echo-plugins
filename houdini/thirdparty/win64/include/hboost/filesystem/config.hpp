//  hboost/filesystem/v3/config.hpp  ----------------------------------------------------//

//  Copyright Beman Dawes 2003

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

//--------------------------------------------------------------------------------------// 

#ifndef HBOOST_FILESYSTEM3_CONFIG_HPP
#define HBOOST_FILESYSTEM3_CONFIG_HPP

# if defined(HBOOST_FILESYSTEM_VERSION) && HBOOST_FILESYSTEM_VERSION != 3
#   error Compiling Filesystem version 3 file with HBOOST_FILESYSTEM_VERSION defined != 3
# endif

# if !defined(HBOOST_FILESYSTEM_VERSION)
#   define HBOOST_FILESYSTEM_VERSION 3
# endif

#define HBOOST_FILESYSTEM_I18N  // aid users wishing to compile several versions

// This header implements separate compilation features as described in
// http://www.boost.org/more/separate_compilation.html

#include <hboost/config.hpp>
#include <hboost/system/api_config.hpp>  // for HBOOST_POSIX_API or HBOOST_WINDOWS_API
#include <hboost/detail/workaround.hpp>

//  HBOOST_FILESYSTEM_DEPRECATED needed for source compiles -----------------------------//

# ifdef HBOOST_FILESYSTEM_SOURCE
#   define HBOOST_FILESYSTEM_DEPRECATED
#   undef HBOOST_FILESYSTEM_NO_DEPRECATED   // fixes #9454, src bld fails if NO_DEP defined
# endif

//  throw an exception  ----------------------------------------------------------------//
//
//  Exceptions were originally thrown via hboost::throw_exception().
//  As throw_exception() became more complex, it caused user error reporting
//  to be harder to interpret, since the exception reported became much more complex.
//  The immediate fix was to throw directly, wrapped in a macro to make any later change
//  easier.

#define HBOOST_FILESYSTEM_THROW(EX) throw EX

# if defined( HBOOST_NO_STD_WSTRING )
#   error Configuration not supported: Boost.Filesystem V3 and later requires std::wstring support
# endif

//  This header implements separate compilation features as described in
//  http://www.boost.org/more/separate_compilation.html

//  normalize macros  ------------------------------------------------------------------//

#if !defined(HBOOST_FILESYSTEM_DYN_LINK) && !defined(HBOOST_FILESYSTEM_STATIC_LINK) \
  && !defined(HBOOST_ALL_DYN_LINK) && !defined(HBOOST_ALL_STATIC_LINK)
# define HBOOST_FILESYSTEM_STATIC_LINK
#endif

#if defined(HBOOST_ALL_DYN_LINK) && !defined(HBOOST_FILESYSTEM_DYN_LINK)
# define HBOOST_FILESYSTEM_DYN_LINK 
#elif defined(HBOOST_ALL_STATIC_LINK) && !defined(HBOOST_FILESYSTEM_STATIC_LINK)
# define HBOOST_FILESYSTEM_STATIC_LINK 
#endif

#if defined(HBOOST_FILESYSTEM_DYN_LINK) && defined(HBOOST_FILESYSTEM_STATIC_LINK)
# error Must not define both HBOOST_FILESYSTEM_DYN_LINK and HBOOST_FILESYSTEM_STATIC_LINK
#endif

#if defined(HBOOST_ALL_NO_LIB) && !defined(HBOOST_FILESYSTEM_NO_LIB)
# define HBOOST_FILESYSTEM_NO_LIB 
#endif

//  enable dynamic linking  ------------------------------------------------------------//

#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_FILESYSTEM_DYN_LINK)
# if defined(HBOOST_FILESYSTEM_SOURCE)
#   define HBOOST_FILESYSTEM_DECL HBOOST_SYMBOL_EXPORT
# else 
#   define HBOOST_FILESYSTEM_DECL HBOOST_SYMBOL_IMPORT
# endif
#else
# define HBOOST_FILESYSTEM_DECL
#endif

//  enable automatic library variant selection  ----------------------------------------// 

#if !defined(HBOOST_FILESYSTEM_SOURCE) && !defined(HBOOST_ALL_NO_LIB) \
  && !defined(HBOOST_FILESYSTEM_NO_LIB)
//
// Set the name of our library, this will get undef'ed by auto_link.hpp
// once it's done with it:
//
#define HBOOST_LIB_NAME hboost_filesystem
//
// If we're importing code from a dll, then tell auto_link.hpp about it:
//
#if defined(HBOOST_ALL_DYN_LINK) || defined(HBOOST_FILESYSTEM_DYN_LINK)
#  define HBOOST_DYN_LINK
#endif
//
// And include the header that does the work:
//
#include <hboost/config/auto_link.hpp>
#endif  // auto-linking disabled

#endif // HBOOST_FILESYSTEM3_CONFIG_HPP
