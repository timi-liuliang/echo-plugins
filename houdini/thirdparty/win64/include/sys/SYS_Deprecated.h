/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Deprecated.h (SYS Library, C++)
 *
 * COMMENTS:	
 */

#ifndef __SYS_DEPRECATED_H_INCLUDED__
#define __SYS_DEPRECATED_H_INCLUDED__

#include "SYS_Pragma.h"

/// Mark function as deprecated and may be removed in the future.
/// @note This qualifier can only appear in function declarations.
#if defined(GCC3)
#  if SYS_IS_GCC_GE(4, 6) || defined(__clang__)
#    define SYS_DEPRECATED(__V__)	\
		__attribute__ ((deprecated("Deprecated since version " #__V__)))
#    define SYS_DEPRECATED_REPLACE(__V__, __R__)	\
		__attribute__ ((deprecated("Deprecated since version " \
		                           SYS_TO_STRING(__V__) ". Use '" \
		                           SYS_TO_STRING(__R__) "' instead.")))
#  else
#    define SYS_DEPRECATED(__V__) __attribute__ ((deprecated))
#    define SYS_DEPRECATED_REPLACE(__V__, __R__) __attribute__ ((deprecated))
#  endif
#elif defined(_MSC_VER)
#  define SYS_DEPRECATED(__V__)	\
		__declspec(deprecated("Deprecated since version " #__V__))
#  define SYS_DEPRECATED_REPLACE(__V__, __R__)	\
		__declspec(deprecated("Deprecated since version " \
		                           SYS_TO_STRING(__V__) ". Use " \
		                           SYS_TO_STRING(__R__) " instead."))
#else
#  define SYS_DEPRECATED(__V__)
#  define SYS_DEPRECATED_REPLACE(__V__, __R__)
#endif


#if !defined(IGNORE_HDK_DEPRECATIONS)
#  define SYS_DEPRECATED_HDK(__V__) SYS_DEPRECATED(__V__)
#  define SYS_DEPRECATED_HDK_REPLACE(__V__,__R__) \
		SYS_DEPRECATED_REPLACE(__V__, __R__)
#else
#  define SYS_DEPRECATED_HDK(__V__)
#  define SYS_DEPRECATED_HDK_REPLACE(__V__, __R__)
#endif

/// Temporarily disable deprecation warnings. The PUSH/POPs must match!
/// @note Only implemented for Visual C++ and gcc 4.6
#if defined(_MSC_VER) || SYS_IS_GCC_GE(4, 6) || defined(__clang__)
#define SYS_DEPRECATED_PUSH_DISABLE()	SYS_PRAGMA_PUSH_WARN() \
					SYS_PRAGMA_DISABLE_DEPRECATED() \
					/**/
#define SYS_DEPRECATED_POP_DISABLE()	SYS_PRAGMA_POP_WARN()
#else
/// Hopefully this works sufficiently to locally disable deprecation
/// warnings in GCC 4.4 and before.
#define SYS_DEPRECATED_PUSH_DISABLE() \
    SYS_PRAGMA(GCC diagnostic ignored "-Wdeprecated-declarations")
#define SYS_DEPRECATED_POP_DISABLE() \
    SYS_PRAGMA(GCC diagnostic warning "-Wdeprecated-declarations")
#endif

#endif // __SYS_DEPRECATED_H_INCLUDED__
