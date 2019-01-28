/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_Types.h ( SYS Library, C++)
 *
 * COMMENTS:	Common types used throughout Houdini.
 */

#ifndef __SYS_Types__
#define __SYS_Types__

#include "SYS_Compiler.h"
#include "SYS_Inline.h"

/* Include system types */
#include <limits>	    // for std::numeric_limits
#include <float.h>	    // DBL_MAX etc
#include <sys/types.h>

// <sys/types.h> defines macros with the names 'major' and 'minor'.
// glibc 2.25+ emits deprecation warnings for this, so eventually the include
// of <sys/sysmacros.h> will be removed and the undef'ing won't be necessary.
#undef major
#undef minor

/*
 * Integer types
 */
typedef signed char	int8;
typedef	unsigned char	uint8;
typedef short		int16;
typedef unsigned short	uint16;
typedef	int		int32;
typedef unsigned int	uint32;

typedef unsigned char	uchar;
#ifndef MBSD
typedef unsigned int	uint;
#endif

/*
 * Unicode code units for Unicode Transformation Formats
 * Houdini uses UTF8.
 */
typedef char		utf8;
typedef unsigned short	utf16;
typedef unsigned int	utf32;


/*
 * The SYS_PRI?64 is to mimic the C99 PRI?64 macro.
 * printf("%" SYS_PRId64, (int64) value);
 */
#if defined(_MSC_VER)
    #define SYS_PRI64_PREFIX "I64"
#elif defined(MBSD)
    #define SYS_PRI64_PREFIX "ll"
#elif defined(AMD64)
    #define SYS_PRI64_PREFIX "l"
#else
    #define SYS_PRI64_PREFIX "ll"
#endif

#if defined(SYS_PRI64_PREFIX)
#define SYS_PRId64 SYS_PRI64_PREFIX "d"
#define SYS_PRIu64 SYS_PRI64_PREFIX "u"
#define SYS_PRIx64 SYS_PRI64_PREFIX "x"
#define SYS_PRIX64 SYS_PRI64_PREFIX "X"
#endif

#if SIZEOF_VOID_P == 8
    #if defined(MBSD)
	#include <inttypes.h>
	#define SYS_PRIiPTR PRIiPTR
	#define SYS_PRIuPTR PRIuPTR
    #else
	#define SYS_PRIiPTR SYS_PRId64
	#define SYS_PRIuPTR SYS_PRIu64
    #endif
#elif SIZEOF_VOID_P == 4
    #define SYS_PRIiPTR "d"
    #define SYS_PRIuPTR "u"
#else
    #error Unknown SIZEOF_VOID_P
#endif

/*
 * Avoid using uint64.
 * The extra bit of precision is NOT worth the cost in pain and suffering
 * induced by use of unsigned.
 */
#if defined(WIN32)
    typedef __int64		int64;
    typedef unsigned __int64	uint64;
#elif defined(MBSD)
    // On MBSD, int64/uint64 are also defined in the system headers so we must
    // declare these in the same way or else we get conflicts.
    #include <stdint.h>
    typedef int64_t		int64;
    typedef uint64_t		uint64;
#elif defined(AMD64)
    typedef long		int64;
    typedef unsigned long	uint64;
#else
    typedef long long		int64;
    typedef unsigned long long	uint64;
#endif

/*
 * The problem with int64 is that it implies that it is a fixed 64-bit quantity
 * that is saved to disk. Therefore, we need another integral type for
 * indexing our arrays.
 */
typedef int64			exint;

/*
 * When casting away const, it is good to use const_cast as it tells
 * the reader what you are doing (as opposed to C-style cast)
 * However, it requires redundant information that breaks cut-and-paste
 * code.  So this template lets you cast away const without worrying
 * about the underlying type.
 */
template <typename T>
SYS_FORCE_INLINE T *
SYSconst_cast(const T *foo)
{
    return const_cast<T *>(foo);
}

template <typename T>
SYS_FORCE_INLINE T &
SYSconst_cast(const T &foo)
{
    return const_cast<T &>(foo);
}

/*
 * Integer limits
 */
#if defined(GCC3) && defined(AMD64)
#define SYS_INT64_C(x) x ## L
#define SYS_UINT64_C(x) x ## UL
#else
#define SYS_INT64_C(x) x ## LL
#define SYS_UINT64_C(x) x ## ULL
#endif

// int8/uint8
#define SYS_INT8_MIN               (-128)
#define SYS_INT8_MAX               (127)
#define SYS_UINT8_MAX              (255)

// int16/uint16
#define SYS_INT16_MIN              (-32767-1)
#define SYS_INT16_MAX              (32767)
#define SYS_UINT16_MAX             (65535)

// int32/uint32
#define SYS_INT32_MIN              (-2147483647-1)
#define SYS_INT32_MAX              (2147483647)
#define SYS_UINT32_MAX             (4294967295U)

// int64/uint64
#define SYS_INT64_MIN              (-SYS_INT64_C(9223372036854775807)-1)
#define SYS_INT64_MAX              (SYS_INT64_C(9223372036854775807))
#define SYS_UINT64_MAX             (SYS_UINT64_C(18446744073709551615))

// exint
#define SYS_EXINT_MIN		   SYS_INT64_MIN
#define SYS_EXINT_MAX		   SYS_INT64_MAX


/*
 * These are the precision you should use for writing out
 * streams to guarantee that you fully save a double or a float.
 * Note that limits.h defines DBL_DIG and FLT_DIG - they are
 * defined incorrectly!
 */
#define SYS_DBL_DIG	17
#define SYS_FLT_DIG	9

/*
 * Floating Point Types
 * By specifying the number of bits (i.e. fpreal32), the precision is
 * automatically determined.  The fpreal type defaults to the precision for the
 * Houdini release and may change accordingly.  fpreal should be used unless
 * there are specific reasons for requiring a certain precision.
 */
typedef	float	fpreal32;
typedef double	fpreal64;

#ifdef __cplusplus
    /* Include half-precision floats after other types are defined */
    #include "fpreal16.h"
#else
    typedef unsigned short fpreal16;
#endif

/* Floating Point Limits/Tolerance */

#define SYS_FTOLERANCE	    ((fpreal32)0.00001)
#define SYS_FTOLERANCE_D    (fpreal64(SYS_FTOLERANCE))

// The difference between 1 and the least value greater than 1 that can be
// represented.
#define SYS_FP32_EPSILON	FLT_EPSILON
#define SYS_FP64_EPSILON	DBL_EPSILON

#define SYS_FP32_MIN	    ((fpreal32)2e-45)
#define SYS_FP64_MIN	    ((fpreal64)2e-324)
#define SYS_FP32_MAX	    ((fpreal32)FLT_MAX)
#define SYS_FP64_MAX	    ((fpreal64)DBL_MAX)

// Tolerance values we can use for templates. This can surely be expanded
template<typename T>
struct SYS_Types : public std::numeric_limits<T>
{
    static T tolerance() { return static_cast<T>(0); }
};
template<>
struct SYS_Types<int32> : public std::numeric_limits<int32>
{
    static int32 tolerance() { return 0; }
};
template<>
struct SYS_Types<int64> : public std::numeric_limits<int64>
{
    static int64 tolerance() { return 0; }
};
template<>
struct SYS_Types<fpreal32> : public std::numeric_limits<fpreal32>
{
    static fpreal32 tolerance() { return SYS_FTOLERANCE; }
};
template<>
struct SYS_Types<fpreal64> : public std::numeric_limits<fpreal64>
{
    static fpreal64 tolerance() { return SYS_FTOLERANCE_D; }
};


/*
 * fpreal definition
 *
 * Default to double unless SIZEOF_FPREAL_IS_4 is defined as 1
 */
#if SIZEOF_FPREAL_IS_4
    /*
     * Single precision fpreal
     */
    typedef float   fpreal;

    // sizeof(fpreal)
    #define SYS_SIZEOF_FPREAL	4

    #define SYS_FPREAL_DIG	SYS_FLT_DIG 
    #define SYS_FTOLERANCE_R    (fpreal(SYS_FTOLERANCE))
    #define SYS_FPEPSILON	SYS_FP32_EPSILON
    #define SYS_FPREAL_MIN	SYS_FP32_MIN 
    #define SYS_FPREAL_MAX	SYS_FP32_MAX

    // The scan code to use in scanf() for fpreal
    #define SYS_SCANF_FPREAL	"g"
#else
    /*
     * Double precision fpreal
     */
    typedef double  fpreal;

    // sizeof(fpreal)
    #define SYS_SIZEOF_FPREAL	8

    #define SYS_FPREAL_DIG	SYS_DBL_DIG
    #define SYS_FTOLERANCE_R    (fpreal(SYS_FTOLERANCE))
    #define SYS_FPEPSILON	SYS_FP64_EPSILON
    #define SYS_FPREAL_MIN	SYS_FP64_MIN 
    #define SYS_FPREAL_MAX	SYS_FP64_MAX
    
    // The scan code to use in scanf() for fpreal
    #define SYS_SCANF_FPREAL	"lg"
#endif
/* need an extra level to ensure complete macro expansion before stringize */
#define SYS_DIG_FMT_INTERNAL(PREC)  "%." #PREC "g"
#define SYS_DIG_FMT(PREC)	    SYS_DIG_FMT_INTERNAL(PREC)
	/* float precision */
#define SYS_FLT_DIG_FMT		    SYS_DIG_FMT(SYS_FLT_DIG)
	/* double precision */
#define SYS_DBL_DIG_FMT		    SYS_DIG_FMT(SYS_DBL_DIG)
	/* fpreal precision */
#define SYS_FPREAL_DIG_FMT	    SYS_DIG_FMT(SYS_FPREAL_DIG)

/*
 * When declaring floating point constants, please use the following macros.
 * If a floating point expression contains a double precision constant, all
 * arguments will be promoted to double precision then the result cast back to
 * single precision.  To avoid the cost of conversions, please use the correct
 * types.  Try to avoid using 180.0F.
 */

#define CONST_INT8(x)	((int8)x)
#define CONST_UINT8(x)	((uint8)x)
#define CONST_INT16(x)	((int16)x)
#define CONST_UINT16(x)	((uint16)x)
#define CONST_INT32(x)	((int32)x)
#define CONST_UINT32(x)	((uint32)x)

#if defined(WIN32)
    #define CONST_INT64(x)	((int64)x)
    #define CONST_UINT64(x)	((uint64)x)
#else
    #define CONST_INT64(x)	(x##LL)
    #define CONST_UINT64(x)	(x##LL)
#endif

#define CONST_FPREAL16(c)	((fpreal16)c)
#define CONST_FPREAL32(c)	((fpreal32)c)
#define CONST_FPREAL64(c)	((fpreal64)c)
#define CONST_FPREAL(c)		((fpreal)c)

///
/// SYS_FPRealUnionT for type-safe casting with integral types
///
template <typename T>
union SYS_FPRealUnionT;

template <>
union SYS_FPRealUnionT<fpreal16>
{
    SYS_FPRealUnionT<fpreal16>() : fval() {}
    
    typedef int16	int_type;
    typedef uint16	uint_type;
    typedef fpreal16	fpreal_type;

    enum {
	EXPONENT_BITS = 5,
	MANTISSA_BITS = 10,
	EXPONENT_BIAS = 15
    };

    int_type		ival;
    uint_type		uval;
    fpreal_type		fval;
    
    struct
    {
	uint_type mantissa_val: 10;
	uint_type exponent_val: 5;
	uint_type sign_val: 1;
    };
};


template <>
union SYS_FPRealUnionT<fpreal32>
{
    typedef int32	int_type;
    typedef uint32	uint_type;
    typedef fpreal32	fpreal_type;

    enum {
	EXPONENT_BITS = 8,
	MANTISSA_BITS = 23,
	EXPONENT_BIAS = 127 };

    int_type		ival;
    uint_type		uval;
    fpreal_type		fval;
    
    struct
    {
	uint_type mantissa_val: 23;
	uint_type exponent_val: 8;
	uint_type sign_val: 1;
    };
};

template <>
union SYS_FPRealUnionT<fpreal64>
{
    typedef int64	int_type;
    typedef uint64	uint_type;
    typedef fpreal64	fpreal_type;

    enum {
	EXPONENT_BITS = 11,
	MANTISSA_BITS = 52,
	EXPONENT_BIAS = 1023 };

    int_type		ival;
    uint_type		uval;
    fpreal_type		fval;
    
    struct
    {
	uint_type mantissa_val: 52;
	uint_type exponent_val: 11;
	uint_type sign_val: 1;
    };
};

typedef union SYS_FPRealUnionT<fpreal>	    SYS_FPRealUnionR;
typedef union SYS_FPRealUnionT<fpreal32>    SYS_FPRealUnionF;
typedef union SYS_FPRealUnionT<fpreal64>    SYS_FPRealUnionD;


/// Special enum to disambiguate default constructors from empty constructors.
/// This can be handy for constructing static objects that do not need to
/// be explicitly constructed but are safe to use if all members are
/// initialized to zero upon DSO load (e.g. their storage is defined by the
/// .bss section in ELF files).
enum SYS_EmptyConstructor { SYS_EMPTY_CONSTRUCTOR };


/// Mark a function as doing printf-style formatting, and generate warnings
/// if the formatting string doesn't match the types.  string_index is the
/// parameter index of the format string, and first_to_check is the index of
/// the "..." parameter.  These indices are both base 1, and "this" counts as
/// the first parameter if it's a method.
#if (defined(GCC3) && !defined(__clang__)) || SYS_IS_CLANG_GE(3,3)
    #define SYS_PRINTF_CHECK_ATTRIBUTE(string_index, first_to_check) \
	__attribute__ ((format (printf, string_index, first_to_check)))
#else
    #define SYS_PRINTF_CHECK_ATTRIBUTE(string_index, first_to_check)
#endif


/// Struct modifiers
///
/// Note that these qualifiers can only appear in struct declarations.
///
// @{

/// Compiler hint to pack the struct with the specified alignment.
/// Note this is not supported on some platforms and is only a hint.
#if defined(GCC4)
#  define SYS_PACKED_STRUCT_HINT_BEGIN(name, n) \
    struct __attribute__((__packed__, __aligned__(n))) name
#  define SYS_PACKED_STRUCT_HINT_END
#elif defined(_MSC_VER)
#  define SYS_PACKED_STRUCT_HINT_BEGIN(name, n) \
    __pragma(pack(push, n)) struct name
#  define SYS_PACKED_STRUCT_HINT_END __pragma(pack(pop))
#else
#  define SYS_PACKED_STRUCT_HINT_BEGIN(name, n) struct name
#  define SYS_PACKED_STRUCT_HINT_END
#endif
// @}

#include "SYS_Decimal128.h"
typedef SYS_Decimal128 fpdec;

#endif
