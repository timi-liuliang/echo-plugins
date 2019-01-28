// UT_Assert.h
//
// Custom portable UT_ASSERT macro
//

#ifndef UT_ASSERT_H_INCLUDED
#define UT_ASSERT_H_INCLUDED

#include "UT_API.h"
#define UT_ASSERT_LEVEL_NONE	    0
#define UT_ASSERT_LEVEL_NORMAL	    1
#define UT_ASSERT_LEVEL_PARANOID    2
#define UT_ASSERT_LEVEL_SLOW	    3

#ifndef UT_ASSERT_LEVEL
    #define UT_ASSERT_LEVEL UT_ASSERT_LEVEL_NONE
#endif

// Generates an assertion. Returns TRUE if we should ignore further asserts.
UT_API void UT_Assert( const char *file, int linenum, const char *function,
	       const char *condition, const char *reason, int &ignoreflag);

// Methods to temporarily disable interactive asserts in specific dangerous
// situations.
UT_API void UTdisableInteractiveAssertsOn();
UT_API void UTdisableInteractiveAssertsOff();

#if defined(__GNUC__)
#   define UT_ASSERT_FUNC __PRETTY_FUNCTION__
#elif defined(_MSC_VER) || defined(__FUNCTION__)
#   define UT_ASSERT_FUNC __FUNCTION__ "()"
#else
#   define UT_ASSERT_FUNC ((char *)0)
#endif

#if (UT_ASSERT_LEVEL > UT_ASSERT_LEVEL_NONE)
    #ifdef UT_INTERNAL_ASSERT
	#error UT_INTERNAL_UT_ASSERT is already defined!
    #endif
    #define UT_INTERNAL_ASSERT(ZZ, MM)	\
	    do {			\
		static int ignore = 0;	\
		if( !ignore && !(ZZ) )	\
		    UT_Assert( __FILE__, __LINE__, UT_ASSERT_FUNC, #ZZ, MM, \
			       ignore);	\
	    } while (false)
    #define UT_INTERNAL_VERIFY(ZZ, MM)	\
	    do {			\
		static int ignore = 0;	\
		/* always run ZZ */	\
		if( !(ZZ) && !ignore )	\
		    UT_Assert( __FILE__, __LINE__, UT_ASSERT_FUNC, #ZZ, MM, \
			       ignore);	\
	    } while (false)

    #define UT_INTERNAL_VERIFY_RETURN(ZZ, RV) \
	    if (!(ZZ))			\
	    {				\
		static int ignore = 0;	\
		if (!ignore)		\
		    UT_Assert( __FILE__, __LINE__, UT_ASSERT_FUNC, #ZZ, 0, \
			       ignore);	\
		return RV;		\
	    }
    #define UT_INTERNAL_ASSERT_EXPR(ZZ,MM)  \
	    ((ZZ) ? void(0) :		    \
	     [&]() {			    \
		static int ignore = 0;	    \
		if( !ignore )		    \
		    UT_Assert( __FILE__, __LINE__, UT_ASSERT_FUNC, #ZZ, MM, \
			       ignore);	    \
	     }())
#endif

#ifdef UT_ASSERT_EXPR
    #error UT_ASSERT_EXPR is already defined!
#endif
#ifdef UT_ASSERT_SLOW
    #error UT_ASSERT_SLOW is already defined!
#endif
#ifdef UT_ASSERT_P
    #error UT_ASSERT_P is already defined!
#endif
#ifdef UT_ASSERT
    #error UT_ASSERT is already defined!
#endif
#ifdef UT_ASSERT_MSG_SLOW
    #error UT_ASSERT_MSG_SLOW is already defined!
#endif
#ifdef UT_ASSERT_MSG_P
    #error UT_ASSERT_MSG_P is already defined!
#endif
#ifdef UT_ASSERT_MSG
    #error UT_ASSERT_MSG is already defined!
#endif

// do these in descending order:
#if (UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_SLOW)
    #define UT_ASSERT_EXPR(ZZ)		UT_INTERNAL_ASSERT_EXPR(ZZ, 0)
    #define UT_ASSERT_SLOW(ZZ)		UT_INTERNAL_ASSERT(ZZ, 0)
    #define UT_ASSERT_P(ZZ)		UT_INTERNAL_ASSERT(ZZ, 0)
    #define UT_ASSERT(ZZ)		UT_INTERNAL_ASSERT(ZZ, 0)
    #define UT_ASSERT_MSG_SLOW(ZZ, MM)	UT_INTERNAL_ASSERT(ZZ, MM)
    #define UT_ASSERT_MSG_P(ZZ, MM)	UT_INTERNAL_ASSERT(ZZ, MM)
    #define UT_ASSERT_MSG(ZZ, MM)	UT_INTERNAL_ASSERT(ZZ, MM)
#elif (UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_PARANOID)
    #define UT_ASSERT_EXPR(ZZ)		UT_INTERNAL_ASSERT_EXPR(ZZ, 0)
    #define UT_ASSERT_SLOW(ZZ)		((void)0)
    #define UT_ASSERT_P(ZZ)		UT_INTERNAL_ASSERT(ZZ, 0)
    #define UT_ASSERT(ZZ)		UT_INTERNAL_ASSERT(ZZ, 0)
    #define UT_ASSERT_MSG_SLOW(ZZ, MM)	((void)0)
    #define UT_ASSERT_MSG_P(ZZ, MM)	UT_INTERNAL_ASSERT(ZZ, MM)
    #define UT_ASSERT_MSG(ZZ, MM)	UT_INTERNAL_ASSERT(ZZ, MM)
#elif (UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_NORMAL)
    #define UT_ASSERT_EXPR(ZZ)		UT_INTERNAL_ASSERT_EXPR(ZZ, 0)
    #define UT_ASSERT_SLOW(ZZ)		((void)0)
    #define UT_ASSERT_P(ZZ)		((void)0)
    #define UT_ASSERT(ZZ)		UT_INTERNAL_ASSERT(ZZ, 0)
    #define UT_ASSERT_MSG_SLOW(ZZ, MM)	((void)0)
    #define UT_ASSERT_MSG_P(ZZ, MM)	((void)0)
    #define UT_ASSERT_MSG(ZZ, MM)	UT_INTERNAL_ASSERT(ZZ, MM)
#else // if (UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_NONE)
    #define UT_ASSERT_EXPR(ZZ)		((void)0)
    #define UT_ASSERT_SLOW(ZZ)		((void)0)
    #define UT_ASSERT_P(ZZ)		((void)0)
    #define UT_ASSERT(ZZ)		((void)0)
    #define UT_ASSERT_MSG_SLOW(ZZ, MM)	((void)0)
    #define UT_ASSERT_MSG_P(ZZ, MM)	((void)0)
    #define UT_ASSERT_MSG(ZZ, MM)	((void)0)
#endif

#include <SYS/SYS_StaticAssert.h>
#define UT_ASSERT_COMPILETIME(expr) SYS_STATIC_ASSERT(expr)

// This macro allows you to enable a block of code only
// when UT_ASSERT is enabled. This will help clean up
// "unreferenced variables" in the case where you have variables that
// are only referenced in asserts.

#if UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_NORMAL
#define UT_IF_ASSERT(ZZ) ZZ
#define UT_IFNOT_ASSERT(ZZ)
#else
#define UT_IF_ASSERT(ZZ)
#define UT_IFNOT_ASSERT(ZZ) ZZ
#endif

#if UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_PARANOID
#define UT_IF_ASSERT_P(ZZ) ZZ
#define UT_IFNOT_ASSERT_P(ZZ)
#else
#define UT_IF_ASSERT_P(ZZ)
#define UT_IFNOT_ASSERT_P(ZZ) ZZ
#endif

#if UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_SLOW
#define UT_IF_ASSERT_SLOW(ZZ) ZZ
#define UT_IFNOT_ASSERT_SLOW(ZZ)
#else
#define UT_IF_ASSERT_SLOW(ZZ)
#define UT_IFNOT_ASSERT_SLOW(ZZ) ZZ
#endif

// When asserts are enabled, UT_VERIFY() will trigger an assert if the expr
// is false. When asserts are disabled, the expr is still evaluated.
#if UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_NORMAL
    #define UT_VERIFY(expr)		UT_INTERNAL_VERIFY(expr, 0)
    #define UT_VERIFY_MSG(expr, MM)	UT_INTERNAL_VERIFY(expr, MM)
    #define UT_VERIFY_RETURN(ZZ, RV)	UT_INTERNAL_VERIFY_RETURN(ZZ, RV)
    #define UT_VERIFY_RETURN_VOID(ZZ)	UT_INTERNAL_VERIFY_RETURN(ZZ,)
#else
    #define UT_VERIFY(expr)		((void)(expr))
    #define UT_VERIFY_MSG(expr, MM)	((void)(expr))
    #define UT_VERIFY_RETURN(ZZ, RV)	if (!(ZZ)) { return RV; }
    #define UT_VERIFY_RETURN_VOID(ZZ)	if (!(ZZ)) { return; }
#endif
#if UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_PARANOID
    #define UT_VERIFY_P(expr)	UT_INTERNAL_VERIFY(expr, 0)
#else
    #define UT_VERIFY_P(expr)	((void)(expr))
#endif

enum UT_AssertResponse
{
    UT_ASSERT_RESPONSE_DEBUG,
    UT_ASSERT_RESPONSE_IGNORE,
    UT_ASSERT_RESPONSE_IGNORE_ALL,
    UT_ASSERT_RESPONSE_TRACE,
    UT_ASSERT_RESPONSE_ERROR
};

/// UTverify_cast performs a static_cast, but when paranoid assertions are
/// enabled it will also perform a dynamic cast to verify the cast is valid.
/// UT_ASSERT_P() is used instead of UT_ASSERT() to avoid overhead in normal
/// development builds.
#include <SYS/SYS_Inline.h>
template <typename TO_T, typename FROM_T>
SYS_FORCE_INLINE TO_T
UTverify_cast(FROM_T from)
{
    UT_ASSERT_P(dynamic_cast<TO_T>(from) == from && "Invalid static cast");
    return static_cast<TO_T>(from);
}

/// UTsubclassResponsibility raises an assertion indicating that a subclass has
/// not implemented an inherited method.  This should only be called from a
/// base class method that is not expected to run.
UT_API void UTsubclassResponsibility(const char *classname, const char *member); 

#endif // UT_ASSERT_H_INCLUDED
