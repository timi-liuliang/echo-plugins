/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Compiler.h (SYS Library, C++)
 *
 * COMMENTS:	
 */

#ifndef __SYS_COMPILER_H_INCLUDED__
#define __SYS_COMPILER_H_INCLUDED__

#if defined(__GNUC__)
    #define SYS_IS_GCC_GE(MAJOR, MINOR) \
	(__GNUC__ > MAJOR || (__GNUC__ == MAJOR && __GNUC_MINOR__ >= MINOR))
    #define SYS_IS_GCC_EQ(MAJOR, MINOR) \
	(__GNUC__ == MAJOR && __GNUC_MINOR__ == MINOR)
#else
    #define SYS_IS_GCC_GE(MAJOR, MINOR) 0
    #define SYS_IS_GCC_EQ(MAJOR, MINOR) 0
#endif

#if defined(__clang__)
    #define SYS_IS_CLANG_GE(MAJOR, MINOR) \
	(__clang_major__ > MAJOR || (__clang_major__ == MAJOR && __clang_minor__ >= MINOR))
#else
    #define SYS_IS_CLANG_GE(MAJOR, MINOR) 0
#endif

#if (__cplusplus >= 201402) || (defined(_MSC_VER) && _MSVC_LANG >= 201402)
    #define SYS_HAS_CXX14    1
#else
    #define SYS_HAS_CXX14    0
#endif

// Token concatenation
#define SYS_CONCAT(a, b) SYS_CONCAT_(a, b)
#define SYS_CONCAT_(a, b) a ## b

#define SYS_TO_STRING_(a) # a
#define SYS_TO_STRING(a) SYS_TO_STRING_(a)

// Handy macro to add warnings (for fix-me's etc.)
#if defined(_MSC_VER)
#define SYS_MESSAGE(desc) __pragma(message(__FILE__ "(" SYS_TO_STRING(__LINE__) ") : Warning: " #desc))
#else
#define SYS_MESSAGE(__msg__) _Pragma(SYS_TO_STRING(message __msg__))
#endif

// MSVC 2012 does not define the 'noexcept' keyword.
#if defined(_MSC_FULL_VER) && _MSC_FULL_VER < 180021114
#define SYS_NOEXCEPT
#else
#define SYS_NOEXCEPT noexcept
#endif

// Define "explicit" for C++11 safe-bool operator
#if (__cplusplus >= 201103) || (_MSC_VER >= 1800)
    #define SYS_SAFE_BOOL	explicit
#else
    #define SYS_SAFE_BOOL	
#endif

// Define the [[fallthrough]] and [[maybe_unused]] attributes.
#if defined(__has_cpp_attribute)
    #if __has_cpp_attribute(fallthrough) == 201603
        #define SYS_FALLTHROUGH [[fallthrough]]
    #endif
    #if __has_cpp_attribute(maybe_unused) == 201603
        #define SYS_MAYBE_UNUSED [[maybe_unused]]
    #endif
#endif
#if !defined(SYS_FALLTHROUGH)
    #define SYS_FALLTHROUGH
#endif
#if !defined(SYS_MAYBE_UNUSED)
    #define SYS_MAYBE_UNUSED
#endif

// Apple dropped the ball on implementing C++11 thread_local in clang on OSX.
// Cited reason was that they thought they could do better than previous
// implementation, but then never did.
// See: https://devforums.apple.com/message/1079348#1079348
#if !(defined(__APPLE__) && defined(__clang__))
    #define SYS_THREAD_LOCAL thread_local
#else
    #define SYS_THREAD_LOCAL __thread
#endif

// Instruct the compiler to assume that the expression given will be true and
// use that information to guide optimizations.
#if defined(_MSC_VER)
    #define SYS_ASSUME(EXPR) __assume(EXPR)
#elif defined(__clang__)
    #if __has_builtin(__builtin_assume)
	#define SYS_ASSUME(EXPR) __builtin_assume(EXPR)
    #else
	#define SYS_ASSUME(EXPR) if (!(EXPR)) __builtin_unreachable() 
    #endif
#else
    #define SYS_ASSUME(EXPR) if (!(EXPR)) __builtin_unreachable() 
#endif

// Defines to disable compiler sanitizer checks
#if defined(__has_feature)
    #if __has_feature(thread_sanitizer)
	#define SYS_NO_SANITIZE_THREAD	__attribute__((no_sanitize_thread))
    #endif
    #if __has_feature(memory_sanitizer)
	#define SYS_NO_SANITIZE_MEMORY	__attribute__((no_sanitize_memory))
    #endif
    #if __has_feature(address_sanitizer)
	#define SYS_NO_SANITIZE_ADDRESS	__attribute__((no_sanitize_address))
    #endif
#endif
#if !defined(SYS_NO_SANITIZE_THREAD)
    #define SYS_NO_SANITIZE_THREAD
#endif
#if !defined(SYS_NO_SANITIZE_MEMORY)
    #define SYS_NO_SANITIZE_MEMORY
#endif
#if !defined(SYS_NO_SANITIZE_ADDRESS)
    #define SYS_NO_SANITIZE_ADDRESS
#endif

#endif // __SYS_COMPILER_H_INCLUDED__
