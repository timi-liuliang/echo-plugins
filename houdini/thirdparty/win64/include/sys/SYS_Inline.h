/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Inline.h ( SYS Library, C++)
 *
 * COMMENTS:	Common definitions to be used with inline functions.
 *
 */

#ifndef __SYS_Inline__
#define __SYS_Inline__

#include "SYS_Compiler.h"

// For compilers that need templated function specializations to have
// storage qualifiers, we need to declare the specializations as static inline.
// Otherwise, we'll get linker errors about multiply defined symbols.
//
// Note that we check for the existence of __clang__ before __GNUC__ because
// the Clang compiler also defines __GNUC__ for compatibility reasons.
#ifdef _MSC_VER
    // MSVC CL compiler.
    #define SYS_STATIC_INLINE static inline
#elif defined(__clang__)
    // Clang compiler.
    #define SYS_STATIC_INLINE inline
#elif defined(__GNUC__)
    // GCC compiler.
    #if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 3)
	    #define SYS_STATIC_INLINE static inline
    #else
	    #define SYS_STATIC_INLINE inline
    #endif
#endif

/// Mark function to be inlined. If this is done, taking the address of such
/// a function is not allowed.
#if defined(GCC4)
    #define SYS_FORCE_INLINE	__attribute__ ((always_inline)) inline
#elif defined(_MSC_VER)
    #define SYS_FORCE_INLINE	__forceinline
#else
    #define SYS_FORCE_INLINE	inline
#endif

#define SYS_STATIC_FORCE_INLINE	static SYS_FORCE_INLINE

/// GCC prior to 4.4 may trigger a compiler error when using
/// SYS_FORCE_INLINE.  To eliminate the chance of this error, use
/// SYS_SAFE_FOCE_INLINE.
#if !defined(__GNUC__) || SYS_IS_GCC_GE(4, 4)
    #define SYS_SAFE_FORCE_INLINE	SYS_FORCE_INLINE
#else
    #define SYS_SAFE_FORCE_INLINE	inline
#endif

/// Mark function as NOT to be inlined.
#if defined(GCC3)
    #define SYS_NO_INLINE	__attribute__ ((noinline))
#elif defined(_MSC_VER)
    #define SYS_NO_INLINE	__declspec(noinline)
#else
    #define SYS_NO_INLINE
#endif

#endif // __SYS_Inline__
