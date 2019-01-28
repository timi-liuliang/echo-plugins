//
// This file is intended to be included in each library-specific API file.
// For example, in a hypthetical LIB_API.h:
//
/*
#include <SYS/SYS_Visibility.h>
#ifdef LIB_EXPORTS
#define LIB_API SYS_VISIBILITY_EXPORT
#define LIB_API_TMPL SYS_VISIBILITY_EXPORT_TMPL
#define LIB_API_TINST SYS_VISIBILITY_EXPORT_TINST
#else
#define LIB_API SYS_VISIBILITY_IMPORT
#define LIB_API_TMPL SYS_VISIBILITY_IMPORT_TMPL
#define LIB_API_TINST SYS_VISIBILITY_IMPORT_TINST
#endif
*/
// LIB_API should be used for classes, functions or global variables that need
// to be visible by external users of that library. LIB_API_TINST is a special
// case for template instantiations, which require an export flag on Windows,
// clang and gcc 4.7 or later. They're not required when building with gcc 4.6 
// and older, since they inherit the visibility from the template class/function 
// they're instantiated from.
// LIB_API_TMPL is required for types which are used as template arguments in
// gcc 4.7 and later. Otherwise derived types will not be exported, since all
// dependent types need to be visible for the final type to be exported.

#ifndef __SYS_Visibility_h__
#define __SYS_Visibility_h__

#include "SYS_Compiler.h"

#if !defined(MAKING_STATIC)
#  if defined(WIN32)
#    define SYS_VISIBILITY_EXPORT __declspec(dllexport)
#    define SYS_VISIBILITY_IMPORT __declspec(dllimport)
#    define SYS_VISIBILITY_EXPORT_TINST SYS_VISIBILITY_EXPORT
#    define SYS_VISIBILITY_IMPORT_TINST SYS_VISIBILITY_IMPORT
#    define SYS_VISIBILITY_EXPORT_EXTERN_TEMPLATE(CLASS_) \
		/* nothing */
#    define SYS_VISIBILITY_IMPORT_EXTERN_TEMPLATE(CLASS_) \
		extern template class SYS_VISIBILITY_IMPORT CLASS_
#  elif (SYS_IS_CLANG_GE(3, 3) || SYS_IS_GCC_GE(4, 7))
    // As of clang 3.3 and gcc 4.7, template instantiations do *not* inherit
    // the visibility attribute.
    // (See: http://gcc.gnu.org/bugzilla/show_bug.cgi?id=51930)
#    define SYS_VISIBILITY_EXPORT __attribute__((visibility("default")))
#    define SYS_VISIBILITY_IMPORT __attribute__((visibility("default")))
#    define SYS_VISIBILITY_EXPORT_TMPL SYS_VISIBILITY_EXPORT
#    define SYS_VISIBILITY_IMPORT_TMPL SYS_VISIBILITY_IMPORT
#    define SYS_VISIBILITY_EXPORT_TINST SYS_VISIBILITY_EXPORT
#    define SYS_VISIBILITY_IMPORT_TINST SYS_VISIBILITY_IMPORT
#    define SYS_VISIBILITY_EXPORT_EXTERN_TEMPLATE(CLASS_) \
		extern template class SYS_VISIBILITY_EXPORT CLASS_
#    define SYS_VISIBILITY_IMPORT_EXTERN_TEMPLATE(CLASS_) \
		extern template class SYS_VISIBILITY_IMPORT CLASS_
#  elif SYS_IS_GCC_GE(4, 2)
#    define SYS_VISIBILITY_EXPORT __attribute__((visibility("default")))
#    define SYS_VISIBILITY_IMPORT __attribute__((visibility("default")))
#  endif
#endif

#ifndef SYS_VISIBILITY_EXPORT
#  define SYS_VISIBILITY_EXPORT
#endif

#ifndef SYS_VISIBILITY_IMPORT
#  define SYS_VISIBILITY_IMPORT
#endif

#ifndef SYS_VISIBILITY_EXPORT_TMPL
#  define SYS_VISIBILITY_EXPORT_TMPL
#endif

#ifndef SYS_VISIBILITY_IMPORT_TMPL
#  define SYS_VISIBILITY_IMPORT_TMPL
#endif

#ifndef SYS_VISIBILITY_EXPORT_TINST
#  define SYS_VISIBILITY_EXPORT_TINST
#endif

#ifndef SYS_VISIBILITY_IMPORT_TINST
#  define SYS_VISIBILITY_IMPORT_TINST
#endif

#ifndef SYS_VISIBILITY_EXPORT_EXTERN_TEMPLATE
#  define SYS_VISIBILITY_EXPORT_EXTERN_TEMPLATE(CLASS_)
#endif
#ifndef SYS_VISIBILITY_IMPORT_EXTERN_TEMPLATE
#  define SYS_VISIBILITY_IMPORT_EXTERN_TEMPLATE(CLASS_)
#endif

#endif
