//  (C) Copyright John Maddock 2003.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

 /*
  *   LOCATION:    see http://www.boost.org for most recent version.
  *   FILE         auto_link.hpp
  *   VERSION      see <hboost/version.hpp>
  *   DESCRIPTION: Automatic library inclusion for Borland/Microsoft compilers.
  */

/*************************************************************************

USAGE:
~~~~~~

Before including this header you must define one or more of define the following macros:

HBOOST_LIB_NAME:           Required: A string containing the basename of the library,
                          for example hboost_regex.
HBOOST_LIB_TOOLSET:        Optional: the base name of the toolset.
HBOOST_DYN_LINK:           Optional: when set link to dll rather than static library.
HBOOST_LIB_DIAGNOSTIC:     Optional: when set the header will print out the name
                          of the library selected (useful for debugging).
HBOOST_AUTO_LINK_NOMANGLE: Specifies that we should link to HBOOST_LIB_NAME.lib,
                          rather than a mangled-name version.
HBOOST_AUTO_LINK_TAGGED:   Specifies that we link to libraries built with the --layout=tagged option.
                          This is essentially the same as the default name-mangled version, but without
                          the compiler name and version, or the Boost version.  Just the build options.

These macros will be undef'ed at the end of the header, further this header
has no include guards - so be sure to include it only once from your library!

Algorithm:
~~~~~~~~~~

Libraries for Borland and Microsoft compilers are automatically
selected here, the name of the lib is selected according to the following
formula:

HBOOST_LIB_PREFIX
   + HBOOST_LIB_NAME
   + "_"
   + HBOOST_LIB_TOOLSET
   + HBOOST_LIB_THREAD_OPT
   + HBOOST_LIB_RT_OPT
   "-"
   + HBOOST_LIB_VERSION

These are defined as:

HBOOST_LIB_PREFIX:     "lib" for static libraries otherwise "".

HBOOST_LIB_NAME:       The base name of the lib ( for example hboost_regex).

HBOOST_LIB_TOOLSET:    The compiler toolset name (vc6, vc7, bcb5 etc).

HBOOST_LIB_THREAD_OPT: "-mt" for multithread builds, otherwise nothing.

HBOOST_LIB_RT_OPT:     A suffix that indicates the runtime library used,
                      contains one or more of the following letters after
                      a hyphen:

                      s      static runtime (dynamic if not present).
                      g      debug/diagnostic runtime (release if not present).
                      y      Python debug/diagnostic runtime (release if not present).
                      d      debug build (release if not present).
                      p      STLport build.
                      n      STLport build without its IOStreams.

HBOOST_LIB_VERSION:    The Boost version, in the form x_y, for Boost version x.y.


***************************************************************************/

#ifdef __cplusplus
#  ifndef HBOOST_CONFIG_HPP
#     include <hboost/config.hpp>
#  endif
#elif defined(_MSC_VER) && !defined(__MWERKS__) && !defined(__EDG_VERSION__)
//
// C language compatability (no, honestly)
//
#  define HBOOST_MSVC _MSC_VER
#  define HBOOST_STRINGIZE(X) HBOOST_DO_STRINGIZE(X)
#  define HBOOST_DO_STRINGIZE(X) #X
#endif
//
// Only include what follows for known and supported compilers:
//
#if defined(HBOOST_MSVC) \
    || defined(__BORLANDC__) \
    || (defined(__MWERKS__) && defined(_WIN32) && (__MWERKS__ >= 0x3000)) \
    || (defined(__ICL) && defined(_MSC_EXTENSIONS) && (_MSC_VER >= 1200))

#ifndef HBOOST_VERSION_HPP
#  include <hboost/version.hpp>
#endif

#ifndef HBOOST_LIB_NAME
#  error "Macro HBOOST_LIB_NAME not set (internal error)"
#endif

//
// error check:
//
#if defined(__MSVC_RUNTIME_CHECKS) && !defined(_DEBUG)
#  pragma message("Using the /RTC option without specifying a debug runtime will lead to linker errors")
#  pragma message("Hint: go to the code generation options and switch to one of the debugging runtimes")
#  error "Incompatible build options"
#endif
//
// select toolset if not defined already:
//
#ifndef HBOOST_LIB_TOOLSET
#  if defined(HBOOST_MSVC) && (HBOOST_MSVC < 1200)
    // Note: no compilers before 1200 are supported
#  elif defined(HBOOST_MSVC) && (HBOOST_MSVC < 1300)

#    ifdef UNDER_CE
       // eVC4:
#      define HBOOST_LIB_TOOLSET "evc4"
#    else
       // vc6:
#      define HBOOST_LIB_TOOLSET "vc6"
#    endif

#  elif defined(HBOOST_MSVC) && (HBOOST_MSVC < 1310)

     // vc7:
#    define HBOOST_LIB_TOOLSET "vc7"

#  elif defined(HBOOST_MSVC) && (HBOOST_MSVC < 1400)

     // vc71:
#    define HBOOST_LIB_TOOLSET "vc71"

#  elif defined(HBOOST_MSVC) && (HBOOST_MSVC < 1500)

     // vc80:
#    define HBOOST_LIB_TOOLSET "vc80"

#  elif defined(HBOOST_MSVC) && (HBOOST_MSVC < 1600)

     // vc90:
#    define HBOOST_LIB_TOOLSET "vc90"

#  elif defined(HBOOST_MSVC) && (HBOOST_MSVC < 1700)

     // vc10:
#    define HBOOST_LIB_TOOLSET "vc100"

#  elif defined(HBOOST_MSVC) && (HBOOST_MSVC < 1800)

     // vc11:
#    define HBOOST_LIB_TOOLSET "vc110"

#  elif defined(HBOOST_MSVC) && (HBOOST_MSVC < 1900)

     // vc12:
#    define HBOOST_LIB_TOOLSET "vc120"

# elif defined(HBOOST_MSVC)

   // vc14:
#  define HBOOST_LIB_TOOLSET "vc140"

#  elif defined(__BORLANDC__)

     // CBuilder 6:
#    define HBOOST_LIB_TOOLSET "bcb"

#  elif defined(__ICL)

     // Intel C++, no version number:
#    define HBOOST_LIB_TOOLSET "iw"

#  elif defined(__MWERKS__) && (__MWERKS__ <= 0x31FF )

     // Metrowerks CodeWarrior 8.x
#    define HBOOST_LIB_TOOLSET "cw8"

#  elif defined(__MWERKS__) && (__MWERKS__ <= 0x32FF )

     // Metrowerks CodeWarrior 9.x
#    define HBOOST_LIB_TOOLSET "cw9"

#  endif
#endif // HBOOST_LIB_TOOLSET

//
// select thread opt:
//
#if defined(_MT) || defined(__MT__)
#  define HBOOST_LIB_THREAD_OPT "-mt"
#else
#  define HBOOST_LIB_THREAD_OPT
#endif

#if defined(_MSC_VER) || defined(__MWERKS__)

#  ifdef _DLL

#     if (defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)) && (defined(_STLP_OWN_IOSTREAMS) || defined(__STL_OWN_IOSTREAMS))

#        if defined(_DEBUG) && (defined(__STL_DEBUG) || defined(_STLP_DEBUG))\
               && defined(HBOOST_DEBUG_PYTHON) && defined(HBOOST_LINKING_PYTHON)
#            define HBOOST_LIB_RT_OPT "-gdp"
#        elif defined(_DEBUG) && (defined(__STL_DEBUG) || defined(_STLP_DEBUG))
#            define HBOOST_LIB_RT_OPT "-gdp"
#        elif defined(_DEBUG)\
               && defined(HBOOST_DEBUG_PYTHON) && defined(HBOOST_LINKING_PYTHON)
#            define HBOOST_LIB_RT_OPT "-gydp"
#            pragma message("warning: STLport debug versions are built with /D_STLP_DEBUG=1")
#            error "Build options aren't compatible with pre-built libraries"
#        elif defined(_DEBUG)
#            define HBOOST_LIB_RT_OPT "-gdp"
#            pragma message("warning: STLport debug versions are built with /D_STLP_DEBUG=1")
#            error "Build options aren't compatible with pre-built libraries"
#        else
#            define HBOOST_LIB_RT_OPT "-p"
#        endif

#     elif defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)

#        if defined(_DEBUG) && (defined(__STL_DEBUG) || defined(_STLP_DEBUG))\
               && defined(HBOOST_DEBUG_PYTHON) && defined(HBOOST_LINKING_PYTHON)
#            define HBOOST_LIB_RT_OPT "-gdpn"
#        elif defined(_DEBUG) && (defined(__STL_DEBUG) || defined(_STLP_DEBUG))
#            define HBOOST_LIB_RT_OPT "-gdpn"
#        elif defined(_DEBUG)\
               && defined(HBOOST_DEBUG_PYTHON) && defined(HBOOST_LINKING_PYTHON)
#            define HBOOST_LIB_RT_OPT "-gdpn"
#            pragma message("warning: STLport debug versions are built with /D_STLP_DEBUG=1")
#            error "Build options aren't compatible with pre-built libraries"
#        elif defined(_DEBUG)
#            define HBOOST_LIB_RT_OPT "-gdpn"
#            pragma message("warning: STLport debug versions are built with /D_STLP_DEBUG=1")
#            error "Build options aren't compatible with pre-built libraries"
#        else
#            define HBOOST_LIB_RT_OPT "-pn"
#        endif

#     else

#        if defined(_DEBUG) && defined(HBOOST_DEBUG_PYTHON) && defined(HBOOST_LINKING_PYTHON)
#            define HBOOST_LIB_RT_OPT "-gd"
#        elif defined(_DEBUG)
#            define HBOOST_LIB_RT_OPT "-gd"
#        else
#            define HBOOST_LIB_RT_OPT
#        endif

#     endif

#  else

#     if (defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)) && (defined(_STLP_OWN_IOSTREAMS) || defined(__STL_OWN_IOSTREAMS))

#        if defined(_DEBUG) && (defined(__STL_DEBUG) || defined(_STLP_DEBUG))\
               && defined(HBOOST_DEBUG_PYTHON) && defined(HBOOST_LINKING_PYTHON)
#            define HBOOST_LIB_RT_OPT "-sgdp"
#        elif defined(_DEBUG) && (defined(__STL_DEBUG) || defined(_STLP_DEBUG))
#            define HBOOST_LIB_RT_OPT "-sgdp"
#        elif defined(_DEBUG)\
               && defined(HBOOST_DEBUG_PYTHON) && defined(HBOOST_LINKING_PYTHON)
#             define HBOOST_LIB_RT_OPT "-sgdp"
#            pragma message("warning: STLport debug versions are built with /D_STLP_DEBUG=1")
#            error "Build options aren't compatible with pre-built libraries"
#        elif defined(_DEBUG)
#             define HBOOST_LIB_RT_OPT "-sgdp"
#            pragma message("warning: STLport debug versions are built with /D_STLP_DEBUG=1")
#            error "Build options aren't compatible with pre-built libraries"
#        else
#            define HBOOST_LIB_RT_OPT "-sp"
#        endif

#     elif defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)

#        if defined(_DEBUG) && (defined(__STL_DEBUG) || defined(_STLP_DEBUG))\
               && defined(HBOOST_DEBUG_PYTHON) && defined(HBOOST_LINKING_PYTHON)
#            define HBOOST_LIB_RT_OPT "-sgdpn"
#        elif defined(_DEBUG) && (defined(__STL_DEBUG) || defined(_STLP_DEBUG))
#            define HBOOST_LIB_RT_OPT "-sgdpn"
#        elif defined(_DEBUG)\
               && defined(HBOOST_DEBUG_PYTHON) && defined(HBOOST_LINKING_PYTHON)
#             define HBOOST_LIB_RT_OPT "-sgdpn"
#            pragma message("warning: STLport debug versions are built with /D_STLP_DEBUG=1")
#            error "Build options aren't compatible with pre-built libraries"
#        elif defined(_DEBUG)
#             define HBOOST_LIB_RT_OPT "-sgdpn"
#            pragma message("warning: STLport debug versions are built with /D_STLP_DEBUG=1")
#            error "Build options aren't compatible with pre-built libraries"
#        else
#            define HBOOST_LIB_RT_OPT "-spn"
#        endif

#     else

#        if defined(_DEBUG)\
               && defined(HBOOST_DEBUG_PYTHON) && defined(HBOOST_LINKING_PYTHON)
#             define HBOOST_LIB_RT_OPT "-sgd"
#        elif defined(_DEBUG)
#             define HBOOST_LIB_RT_OPT "-sgd"
#        else
#            define HBOOST_LIB_RT_OPT "-s"
#        endif

#     endif

#  endif

#elif defined(__BORLANDC__)

//
// figure out whether we want the debug builds or not:
//
#if __BORLANDC__ > 0x561
#pragma defineonoption HBOOST_BORLAND_DEBUG -v
#endif
//
// sanity check:
//
#if defined(__STL_DEBUG) || defined(_STLP_DEBUG)
#error "Pre-built versions of the Boost libraries are not provided in STLport-debug form"
#endif

#  ifdef _RTLDLL

#     if defined(HBOOST_BORLAND_DEBUG)\
               && defined(HBOOST_DEBUG_PYTHON) && defined(HBOOST_LINKING_PYTHON)
#         define HBOOST_LIB_RT_OPT "-d"
#     elif defined(HBOOST_BORLAND_DEBUG)
#         define HBOOST_LIB_RT_OPT "-d"
#     elif defined(HBOOST_DEBUG_PYTHON) && defined(HBOOST_LINKING_PYTHON)
#         define HBOOST_LIB_RT_OPT
#     else
#         define HBOOST_LIB_RT_OPT
#     endif

#  else

#     if defined(HBOOST_BORLAND_DEBUG)\
               && defined(HBOOST_DEBUG_PYTHON) && defined(HBOOST_LINKING_PYTHON)
#         define HBOOST_LIB_RT_OPT "-sd"
#     elif defined(HBOOST_BORLAND_DEBUG)
#         define HBOOST_LIB_RT_OPT "-sd"
#     elif defined(HBOOST_DEBUG_PYTHON) && defined(HBOOST_LINKING_PYTHON)
#         define HBOOST_LIB_RT_OPT "-s"
#     else
#         define HBOOST_LIB_RT_OPT "-s"
#     endif

#  endif

#endif

//
// select linkage opt:
//
#if (defined(_DLL) || defined(_RTLDLL)) && defined(HBOOST_DYN_LINK)
#  define HBOOST_LIB_PREFIX
#elif defined(HBOOST_DYN_LINK)
#  error "Mixing a dll boost library with a static runtime is a really bad idea..."
#else
#  define HBOOST_LIB_PREFIX "lib"
#endif

//
// now include the lib:
//
#if defined(HBOOST_LIB_NAME) \
      && defined(HBOOST_LIB_PREFIX) \
      && defined(HBOOST_LIB_TOOLSET) \
      && defined(HBOOST_LIB_THREAD_OPT) \
      && defined(HBOOST_LIB_RT_OPT) \
      && defined(HBOOST_LIB_VERSION)

#ifdef HBOOST_AUTO_LINK_TAGGED
#  pragma comment(lib, HBOOST_LIB_PREFIX HBOOST_STRINGIZE(HBOOST_LIB_NAME) HBOOST_LIB_THREAD_OPT HBOOST_LIB_RT_OPT ".lib")
#  ifdef HBOOST_LIB_DIAGNOSTIC
#     pragma message ("Linking to lib file: " HBOOST_LIB_PREFIX HBOOST_STRINGIZE(HBOOST_LIB_NAME) HBOOST_LIB_THREAD_OPT HBOOST_LIB_RT_OPT ".lib")
#  endif
#elif defined(HBOOST_AUTO_LINK_NOMANGLE)
#  pragma comment(lib, HBOOST_STRINGIZE(HBOOST_LIB_NAME) ".lib")
#  ifdef HBOOST_LIB_DIAGNOSTIC
#     pragma message ("Linking to lib file: " HBOOST_STRINGIZE(HBOOST_LIB_NAME) ".lib")
#  endif
#elif defined(HBOOST_LIB_BUILDID)
#  pragma comment(lib, HBOOST_LIB_PREFIX HBOOST_STRINGIZE(HBOOST_LIB_NAME) HBOOST_LIB_THREAD_OPT HBOOST_LIB_RT_OPT "-" HBOOST_STRINGIZE(HBOOST_LIB_BUILDID) ".lib")
#  ifdef HBOOST_LIB_DIAGNOSTIC
#     pragma message ("Linking to lib file: " HBOOST_LIB_PREFIX HBOOST_STRINGIZE(HBOOST_LIB_NAME) HBOOST_LIB_THREAD_OPT HBOOST_LIB_RT_OPT "-" HBOOST_STRINGIZE(HBOOST_LIB_BUILDID) ".lib")
#  endif
#else
#  pragma comment(lib, HBOOST_LIB_PREFIX HBOOST_STRINGIZE(HBOOST_LIB_NAME) HBOOST_LIB_THREAD_OPT HBOOST_LIB_RT_OPT ".lib")
#  ifdef HBOOST_LIB_DIAGNOSTIC
#     pragma message ("Linking to lib file: " HBOOST_LIB_PREFIX HBOOST_STRINGIZE(HBOOST_LIB_NAME) HBOOST_LIB_THREAD_OPT HBOOST_LIB_RT_OPT ".lib")
#  endif
#endif

#else
#  error "some required macros where not defined (internal logic error)."
#endif


#endif // _MSC_VER || __BORLANDC__

//
// finally undef any macros we may have set:
//
#ifdef HBOOST_LIB_PREFIX
#  undef HBOOST_LIB_PREFIX
#endif
#if defined(HBOOST_LIB_NAME)
#  undef HBOOST_LIB_NAME
#endif
// Don't undef this one: it can be set by the user and should be the 
// same for all libraries:
//#if defined(HBOOST_LIB_TOOLSET)
//#  undef HBOOST_LIB_TOOLSET
//#endif
#if defined(HBOOST_LIB_THREAD_OPT)
#  undef HBOOST_LIB_THREAD_OPT
#endif
#if defined(HBOOST_LIB_RT_OPT)
#  undef HBOOST_LIB_RT_OPT
#endif
#if defined(HBOOST_LIB_LINK_OPT)
#  undef HBOOST_LIB_LINK_OPT
#endif
#if defined(HBOOST_LIB_DEBUG_OPT)
#  undef HBOOST_LIB_DEBUG_OPT
#endif
#if defined(HBOOST_DYN_LINK)
#  undef HBOOST_DYN_LINK
#endif


