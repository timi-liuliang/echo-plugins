/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Pragma.h (SYS Library, C++)
 *
 * COMMENTS:	
 */

#ifndef __SYS_PRAGMA_H_INCLUDED__
#define __SYS_PRAGMA_H_INCLUDED__

#include "SYS_Compiler.h"

/// SYS_PRAGMA allows one to embed multiple into macros
#if defined(_MSC_VER)
#   define SYS_PRAGMA(X)		    __pragma(X)
#elif defined(__GNUC__)
#   define SYS_PRAGMA(X)		    _Pragma(#X)
#else
#   define SYS_PRAGMA(X)		    
#endif

/// Temporarily disable warnings. The PUSH/POPs must match!
/// @note Only implemented for Visual C++ and gcc 4.6
#if defined(_MSC_VER)
#   define SYS_PRAGMA_PUSH_WARN()	    SYS_PRAGMA(warning(push))
#   define SYS_PRAGMA_POP_WARN()	    SYS_PRAGMA(warning(pop))
#elif SYS_IS_GCC_GE(4, 6) || defined(__clang__)
#   define SYS_PRAGMA_PUSH_WARN()	    SYS_PRAGMA(GCC diagnostic push)
#   define SYS_PRAGMA_POP_WARN()	    SYS_PRAGMA(GCC diagnostic pop)
#else
#   define SYS_PRAGMA_PUSH_WARN()
#   define SYS_PRAGMA_POP_WARN()
#endif

#if defined(_MSC_VER)
#   define SYS_PRAGMA_DISABLE_DEPRECATED() \
	SYS_PRAGMA(warning(disable: 4996))
#elif SYS_IS_GCC_GE(4, 2) || defined(__clang__)
#   define SYS_PRAGMA_DISABLE_DEPRECATED()  \
	SYS_PRAGMA(GCC diagnostic ignored "-Wdeprecated-declarations")
#else
#   define SYS_PRAGMA_DISABLE_DEPRECATED()
#endif
#if defined(_MSC_VER)
#   define SYS_PRAGMA_DISABLE_OVERLOADED_VIRTUAL() \
	SYS_PRAGMA(warning(disable: 4263)) \
	SYS_PRAGMA(warning(disable: 4266)) \
	/**/
#elif SYS_IS_GCC_GE(4, 6) || defined(__clang__)
#   define SYS_PRAGMA_DISABLE_OVERLOADED_VIRTUAL() \
	SYS_PRAGMA(GCC diagnostic ignored "-Woverloaded-virtual")
#else
#   define SYS_PRAGMA_DISABLE_OVERLOADED_VIRTUAL()
#endif
#if SYS_IS_GCC_GE(4, 6) || defined(__clang__)
#   define SYS_PRAGMA_DISABLE_IGNORED_QUALIFIERS() \
	SYS_PRAGMA(GCC diagnostic ignored "-Wignored-qualifiers")
#else
#   define SYS_PRAGMA_DISABLE_IGNORED_QUALIFIERS()
#endif
#if SYS_IS_GCC_GE(4, 6) || defined(__clang__)
#   define SYS_PRAGMA_DISABLE_UNUSED_FUNCTION() \
	SYS_PRAGMA(GCC diagnostic ignored "-Wunused-function")
#else
#   define SYS_PRAGMA_DISABLE_UNUSED_FUNCTION()
#endif
#if SYS_IS_GCC_GE(4, 6) || defined(__clang__)
#   define SYS_PRAGMA_DISABLE_UNUSED_VALUE() \
	SYS_PRAGMA(GCC diagnostic ignored "-Wunused-value")
#else
#   define SYS_PRAGMA_DISABLE_UNUSED_VALUE()
#endif
#if SYS_IS_GCC_GE(4, 6) || defined(__clang__)
#   define SYS_PRAGMA_DISABLE_UNUSED_VARIABLE() \
	SYS_PRAGMA(GCC diagnostic ignored "-Wunused-variable")
#else
#   define SYS_PRAGMA_DISABLE_UNUSED_VARIABLE()
#endif
#if SYS_IS_GCC_GE(4, 6)
#   define SYS_PRAGMA_DISABLE_UNUSED_BUT_SET_VARIABLE() \
	SYS_PRAGMA(GCC diagnostic ignored "-Wunused-but-set-variable")
#elif defined(_MSC_VER)
#   define SYS_PRAGMA_DISABLE_UNUSED_BUT_SET_VARIABLE() \
	SYS_PRAGMA(warning(disable: 4189))
#else
#   define SYS_PRAGMA_DISABLE_UNUSED_BUT_SET_VARIABLE()
#endif
#if SYS_IS_GCC_GE(4, 6) || defined(__clang__)
#   define SYS_PRAGMA_DISABLE_COMMENT_WARNING() \
	SYS_PRAGMA(GCC diagnostic ignored "-Wcomment")
#else
#   define SYS_PRAGMA_DISABLE_COMMENT_WARNING()
#endif
#if SYS_IS_GCC_GE(4, 6) || defined(__clang__)
#   define SYS_PRAGMA_DISABLE_MISSING_FIELD_INITIALIZERS() \
	SYS_PRAGMA(GCC diagnostic ignored "-Wmissing-field-initializers")
#else
#   define SYS_PRAGMA_DISABLE_MISSING_FIELD_INITIALIZERS()
#endif
#if SYS_IS_GCC_GE(4, 6) || defined(__clang__)
#   define SYS_PRAGMA_DISABLE_NON_VIRTUAL_DTOR() \
	SYS_PRAGMA(GCC diagnostic ignored "-Wnon-virtual-dtor")
#else
#   define SYS_PRAGMA_DISABLE_NON_VIRTUAL_DTOR() \
	SYS_PRAGMA(warning(disable: 4265))
#endif
#if SYS_IS_GCC_GE(4, 6) && !defined(__clang__)
#   define SYS_PRAGMA_DISABLE_FREE_NONHEAP_OBJECT() \
	SYS_PRAGMA(GCC diagnostic ignored "-Wfree-nonheap-object")
#else
#   define SYS_PRAGMA_DISABLE_FREE_NONHEAP_OBJECT()
#endif

#if SYS_IS_GCC_GE(4, 8) && !defined(__clang__)
#   define SYS_PRAGMA_DISABLE_INVALID_OFFSETOF() \
	SYS_PRAGMA(GCC diagnostic ignored "-Winvalid-offsetof")
#else
#   define SYS_PRAGMA_DISABLE_INVALID_OFFSETOF()
#endif

#if defined(__clang__)
#   define SYS_PRAGMA_DISABLE_INCONSISTENT_OVERRIDE() \
	SYS_PRAGMA(GCC diagnostic ignored "-Winconsistent-missing-override")
#else
#   define SYS_PRAGMA_DISABLE_INCONSISTENT_OVERRIDE()
#endif

#if SYS_IS_GCC_GE(4, 8) && !defined(__clang__)
#   define SYS_PRAGMA_DISABLE_MAYBE_UNINITIALIZED() \
	SYS_PRAGMA(GCC diagnostic ignored "-Wmaybe-uninitialized")
#else
#   define SYS_PRAGMA_DISABLE_MAYBE_UNINITIALIZED()
#endif

#if defined(__clang__)
#   define SYS_PRAGMA_DISABLE_VARARGS() \
	SYS_PRAGMA(GCC diagnostic ignored "-Wvarargs")
#else
#   define SYS_PRAGMA_DISABLE_VARARGS()
#endif

// This can be used to disable the warning about ignored attributes on
// templates.  We're likely hitting:
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=50044
#if SYS_IS_GCC_GE(4, 8) && !defined(__clang__)
#   define SYS_PRAGMA_DISABLE_ATTRIBUTES() \
	SYS_PRAGMA(GCC diagnostic ignored "-Wattributes")
#else
#   define SYS_PRAGMA_DISABLE_ATTRIBUTES()
#endif

#if defined(__GNUC__)
#   define SYS_UNUSED_VAR_ATTRIB    __attribute__((unused))
#else
#   define SYS_UNUSED_VAR_ATTRIB
#endif

#endif // __SYS_PRAGMA_H_INCLUDED__
