/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_TypeDecorate.h (SYS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SYS_TYPEDECORATE_H_INCLUDED__
#define __SYS_TYPEDECORATE_H_INCLUDED__

#include <type_traits>

/// @file
/// Provides facilities to decorate types that can then be tested using type
/// traits.

/// Declare a type as arithmetic
#define SYS_DECLARE_IS_ARITHMETIC(T) \
	    namespace std { \
		template<> struct is_arithmetic<T> : public true_type {}; \
	    } \
            /* This remains for HDK compatibility */ \
	    namespace hboost { \
                template <typename S> struct is_arithmetic; \
		template<> struct is_arithmetic<T> : public std::true_type {}; \
	    } \
	    /**/

/// Declare a type as POD
#define SYS_DECLARE_IS_POD(T) \
	    namespace std { \
		template<> struct is_pod<T> : public true_type {}; \
	    } \
            /* This remains for HDK compatibility */ \
	    namespace hboost { \
                template <typename S> struct is_pod; \
		template<> struct is_pod<T> : public std::true_type {}; \
	    } \
	    /**/

#endif // __SYS_TYPEDECORATE_H_INCLUDED__
