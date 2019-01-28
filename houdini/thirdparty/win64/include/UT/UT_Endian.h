/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Endian.h (C++)
 *
 * COMMENTS:
 *	LITTLE-BIG ENDIAN CONVERSION
 *	----------------------------
 *
 *	The tomips/tovax will convert the data type to the appropriate
 *	format OR from the appropriate format.  For example, if you
 *	have an integer that you know is in VAX format, you can call
 *		UTtomips(x)
 *	to get it into the native format for the machine you're on.
 *	If you have a MIPS format int and you want to convert to a VAX
 *	format int, you can also call
 *		UTtomips(x)
 *	In otherwords:
 *		UTtomips(x); UTtomips(x);
 *	will result in X being the same format as it was before.
 *
 *	Yes, it's a bit confusing, but probably less confusing than
 *	something like UTtoLittleEndian().
 *
 *	All data conversions are done in place.
 *
 *      Note:	The Intel machines are all little endian
 *
 *	ARRAY CONVERSIONS
 *	-----------------
 *	Efficient array casting.
 */

#ifndef __UT_Endian_h__
#define __UT_Endian_h__

#include "UT_API.h"
#include <string.h>
#include <SYS/SYS_Types.h>

#define UTswap_int16(x,z)  { x = (((z & 0xff) << 8) | ((z >> 8) & 0xff)); }
#define UTswap_int32(x,z)  { x = ((z & 0xff) << 24) | ((z & 0xff00) << 8) \
			     | ((z >> 8) & 0xff00) | ((z >> 24) & 0xff); }
#define UTswap_int64(x,z)  { x = (((((z      ) & 0xff) << 56) | \
			       (((z >>  8) & 0xff) << 48) | \
			       (((z >> 16) & 0xff) << 40) | \
			       (((z >> 24) & 0xff) << 32) | \
			       (((z >> 32) & 0xff) << 24) | \
			       (((z >> 40) & 0xff) << 16) | \
			       (((z >> 48) & 0xff) <<  8) | \
			       (((z >> 56) & 0xff)      ))); }
#define UTswap_int16_inplace(z)	UTswap_int16(z,z)
#define UTswap_int32_inplace(z)	UTswap_int32(z,z)
#define UTswap_int64_inplace(z)	UTswap_int64(z,z)

UT_API extern void UTswapBytes(int16 *values, int64 num);	// 16 bit
UT_API extern void UTswapBytes(int32 *values, int64 num);	// 32 bit
UT_API extern void UTswapBytes(int64 *values, int64 num);	// 64 bit
UT_API extern void UTswapBytes(fpreal16 *values, int64 num);	// 16 bit
UT_API extern void UTswapBytes(fpreal32 *values, int64 num);	// 32 bit
UT_API extern void UTswapBytes(fpreal64 *values, int64 num);	// 64 bit

UT_API extern void UTswapBytes(int16 *dest, const int16 *src, int64 num);
UT_API extern void UTswapBytes(int32 *dest, const int32 *src, int64 num);
UT_API extern void UTswapBytes(int64 *dest, const int64 *src, int64 num);
UT_API extern void UTswapBytes(fpreal16 *dest, const fpreal16 *src, int64 num);
UT_API extern void UTswapBytes(fpreal32 *dest, const fpreal32 *src, int64 num);
UT_API extern void UTswapBytes(fpreal64 *dest, const fpreal64 *src, int64 num);

// Conversion along with swapping
UT_API extern void UTswapBytes(fpreal32 *dest, const fpreal64 *src, int64 num);
UT_API extern void UTswapBytes(fpreal64 *dest, const fpreal32 *src, int64 num);

#define BYTE_ORDER_MATCHING_TAG16 0x3210
#define BYTE_ORDER_OPPOSITE_TAG16 0x1032
#define BYTE_ORDER_MATCHING_TAG32 0x76543210
#define BYTE_ORDER_OPPOSITE_TAG32 0x10325476
#define BYTE_ORDER_MATCHING_TAG64 0xFEDCBA9876543210ULL
#define BYTE_ORDER_OPPOSITE_TAG64 0x1032547698BADCFEULL

#ifdef SESI_LITTLE_ENDIAN
    // Swap byte routines for little endian machines

    /// NOTE: Don't use these!  Write out BYTE_ORDER_MATCHING_TAG32, and have
    /// the reader check if it's equal to that or BYTE_ORDER_OPPOSITE_TAG32.
    /// Then, swapping only needs to be done if there's a mismatch, which
    /// will probably never happen, unless we end up supporting a
    /// big-endian platform at some point in the future and people take
    /// data from one platform and use it on a platform with different
    /// endianness.
    /// @{
    inline void	UTtomips(int16  *values, int64 num) { UTswapBytes(values,num); }
    inline void	UTtomips(int32  *values, int64 num) { UTswapBytes(values,num); }
    inline void	UTtomips(int64  *values, int64 num) { UTswapBytes(values,num); }
    inline void	UTtomips(fpreal32 *values, int64 n) { UTswapBytes(values, n); }
    inline void	UTtomips(fpreal64 *values, int64 n) { UTswapBytes(values, n); }

    inline void UTtomips(int16 &z)		UTswap_int16(z, z)
    inline void UTtomips(uint16 &z)		UTswap_int16(z, z)
    inline void UTtomips(int32 &z)		UTswap_int32(z, z)
    inline void UTtomips(uint32 &z)		UTswap_int32(z, z)
    inline void UTtomips(int64 &z)		{ UTswapBytes(&z,1); }
    inline void UTtomips(uint64 &z)		{ UTswapBytes((int64*)&z,1); }
    inline void	UTtomips(fpreal32 &z)		{ UTtomips(*(uint32 *)&z); }
    inline void UTtomips(fpreal64 &z)		{ UTswapBytes(&z, 1); }

    inline void		UTtovax(int16 *, int64)	    {}
    inline void		UTtovax(int32 *, int64)	    {}
    inline void		UTtovax(int64 *, int64)	    {}
    inline void		UTtovax(fpreal32 *, int64)  {}
    inline void		UTtovax(fpreal64 *, int64)  {}

    inline void		UTtovax(int16)		{}
    inline void		UTtovax(uint16)		{}
    inline void		UTtovax(int32)		{}
    inline void		UTtovax(uint32)		{}
    inline void		UTtovax(int64)		{}
    inline void		UTtovax(uint64)		{}
    inline void		UTtovax(fpreal32)	{}
    inline void		UTtovax(fpreal64)	{}
    /// @}

#else
    // Byte swap routines for big endian machines

    inline void	UTtovax(int16  *values, int64 num) { UTswapBytes(values, num); }
    inline void	UTtovax(int32  *values, int64 num) { UTswapBytes(values, num); }
    inline void	UTtovax(int64  *values, int64 num) { UTswapBytes(values, num); }
    inline void	UTtovax(fpreal16 *values, int64 n) { UTswapBytes(values, n); }
    inline void	UTtovax(fpreal32 *values, int64 n) { UTswapBytes(values, n); }
    inline void	UTtovax(fpreal64 *values, int64 n) { UTswapBytes(values, n); }

    inline void UTtovax(int16 &z)		UTswap_int16(z, z)
    inline void UTtovax(uint16 &z)		UTswap_int16(z, z)
    inline void UTtovax(int32 &z)		UTswap_int32(z, z)
    inline void UTtovax(uint32 &z)		UTswap_int32(z, z)
    inline void UTtovax(int64 &z)		{ UTswapBytes(&z,1); }
    inline void UTtovax(uint64 &z)		{ UTswapBytes((int64*)&z, 1); }
    inline void	UTtovax(fpreal16 &z)		{ UTtovax(*(uint16 *)&z); }
    inline void	UTtovax(fpreal32 &z)		{ UTtovax(*(uint32 *)&z); }
    inline void UTtovax(fpreal64 &z)		{ UTswapBytes(&z, 1); }

    inline void		UTtomips(int16 *, int64)	{}
    inline void		UTtomips(int32 *, int64)	{}
    inline void		UTtomips(int64 *, int64)	{}
    inline void		UTtomips(fpreal32 *, int64)	{}
    inline void		UTtomips(fpreal64 *, int64)	{}

    inline void UTtomips(int16 &)		{}
    inline void UTtomips(uint16 &)		{}
    inline void UTtomips(int32 &)		{}
    inline void UTtomips(uint32 &)		{}
    inline void UTtomips(int64 &)		{}
    inline void UTtomips(uint64 &)		{}
    inline void	UTtomips(fpreal32 &)		{}
    inline void UTtomips(fpreal64 &)		{}

#endif	// End of the defines

// Array casting conversion
#define CAST_SAME_TYPE(TYPE)	\
    inline static void UTcast(TYPE *dest, const TYPE *src, uint64 size) \
	{ memmove(dest, src, size*sizeof(TYPE)); } \
    static inline void UTcastFast(TYPE *dest, const TYPE *src, uint64 size) \
	{ memcpy(dest, src, size*sizeof(TYPE)); } \
    /**/

#define CAST_DIFFERENT_TYPE(DTYPE, STYPE)	\
    UT_API void UTcast(DTYPE *dest, const STYPE *src, uint64 size); \
    static inline void UTcastFast(DTYPE *dest, const STYPE *src, uint64 size) \
    { \
	for (uint64 i = 0; i < size; i++) \
	    dest[i]=(DTYPE)src[i]; \
    } \
    /**/

CAST_SAME_TYPE(int8)
CAST_DIFFERENT_TYPE(int8, uint8)
CAST_DIFFERENT_TYPE(int8, int16)
CAST_DIFFERENT_TYPE(int8, uint16)
CAST_DIFFERENT_TYPE(int8, int32)
CAST_DIFFERENT_TYPE(int8, uint32)
CAST_DIFFERENT_TYPE(int8, int64)
CAST_DIFFERENT_TYPE(int8, uint64)
CAST_DIFFERENT_TYPE(int8, fpreal16)
CAST_DIFFERENT_TYPE(int8, fpreal32)
CAST_DIFFERENT_TYPE(int8, fpreal64)

CAST_DIFFERENT_TYPE(uint8, int8)
CAST_SAME_TYPE(uint8)
CAST_DIFFERENT_TYPE(uint8, int16)
CAST_DIFFERENT_TYPE(uint8, uint16)
CAST_DIFFERENT_TYPE(uint8, int32)
CAST_DIFFERENT_TYPE(uint8, uint32)
CAST_DIFFERENT_TYPE(uint8, int64)
CAST_DIFFERENT_TYPE(uint8, uint64)
CAST_DIFFERENT_TYPE(uint8, fpreal16)
CAST_DIFFERENT_TYPE(uint8, fpreal32)
CAST_DIFFERENT_TYPE(uint8, fpreal64)

CAST_DIFFERENT_TYPE(int16, int8)
CAST_DIFFERENT_TYPE(int16, uint8)
CAST_SAME_TYPE(int16)
CAST_DIFFERENT_TYPE(int16, uint16)
CAST_DIFFERENT_TYPE(int16, int32)
CAST_DIFFERENT_TYPE(int16, uint32)
CAST_DIFFERENT_TYPE(int16, int64)
CAST_DIFFERENT_TYPE(int16, uint64)
CAST_DIFFERENT_TYPE(int16, fpreal16)
CAST_DIFFERENT_TYPE(int16, fpreal32)
CAST_DIFFERENT_TYPE(int16, fpreal64)

CAST_DIFFERENT_TYPE(uint16, int8)
CAST_DIFFERENT_TYPE(uint16, uint8)
CAST_DIFFERENT_TYPE(uint16, int16)
CAST_SAME_TYPE(uint16)
CAST_DIFFERENT_TYPE(uint16, int32)
CAST_DIFFERENT_TYPE(uint16, uint32)
CAST_DIFFERENT_TYPE(uint16, int64)
CAST_DIFFERENT_TYPE(uint16, uint64)
CAST_DIFFERENT_TYPE(uint16, fpreal16)
CAST_DIFFERENT_TYPE(uint16, fpreal32)
CAST_DIFFERENT_TYPE(uint16, fpreal64)

CAST_DIFFERENT_TYPE(int32, int8)
CAST_DIFFERENT_TYPE(int32, uint8)
CAST_DIFFERENT_TYPE(int32, int16)
CAST_DIFFERENT_TYPE(int32, uint16)
CAST_SAME_TYPE(int32)
CAST_DIFFERENT_TYPE(int32, uint32)
CAST_DIFFERENT_TYPE(int32, int64)
CAST_DIFFERENT_TYPE(int32, uint64)
CAST_DIFFERENT_TYPE(int32, fpreal16)
CAST_DIFFERENT_TYPE(int32, fpreal32)
CAST_DIFFERENT_TYPE(int32, fpreal64)

CAST_DIFFERENT_TYPE(uint32, int8)
CAST_DIFFERENT_TYPE(uint32, uint8)
CAST_DIFFERENT_TYPE(uint32, int16)
CAST_DIFFERENT_TYPE(uint32, uint16)
CAST_DIFFERENT_TYPE(uint32, int32)
CAST_SAME_TYPE(uint32)
CAST_DIFFERENT_TYPE(uint32, int64)
CAST_DIFFERENT_TYPE(uint32, uint64)
CAST_DIFFERENT_TYPE(uint32, fpreal16)
CAST_DIFFERENT_TYPE(uint32, fpreal32)
CAST_DIFFERENT_TYPE(uint32, fpreal64)

CAST_DIFFERENT_TYPE(int64, int8)
CAST_DIFFERENT_TYPE(int64, uint8)
CAST_DIFFERENT_TYPE(int64, int16)
CAST_DIFFERENT_TYPE(int64, uint16)
CAST_DIFFERENT_TYPE(int64, int32)
CAST_DIFFERENT_TYPE(int64, uint32)
CAST_SAME_TYPE(int64)
CAST_DIFFERENT_TYPE(int64, uint64)
CAST_DIFFERENT_TYPE(int64, fpreal16)
CAST_DIFFERENT_TYPE(int64, fpreal32)
CAST_DIFFERENT_TYPE(int64, fpreal64)

CAST_DIFFERENT_TYPE(uint64, int8)
CAST_DIFFERENT_TYPE(uint64, uint8)
CAST_DIFFERENT_TYPE(uint64, int16)
CAST_DIFFERENT_TYPE(uint64, uint16)
CAST_DIFFERENT_TYPE(uint64, int32)
CAST_DIFFERENT_TYPE(uint64, uint32)
CAST_DIFFERENT_TYPE(uint64, int64)
CAST_SAME_TYPE(uint64)
CAST_DIFFERENT_TYPE(uint64, fpreal16)
CAST_DIFFERENT_TYPE(uint64, fpreal32)
CAST_DIFFERENT_TYPE(uint64, fpreal64)

CAST_DIFFERENT_TYPE(fpreal16, int8)
CAST_DIFFERENT_TYPE(fpreal16, uint8)
CAST_DIFFERENT_TYPE(fpreal16, int16)
CAST_DIFFERENT_TYPE(fpreal16, uint16)
CAST_DIFFERENT_TYPE(fpreal16, int32)
CAST_DIFFERENT_TYPE(fpreal16, uint32)
CAST_DIFFERENT_TYPE(fpreal16, int64)
CAST_DIFFERENT_TYPE(fpreal16, uint64)
CAST_SAME_TYPE(fpreal16)
CAST_DIFFERENT_TYPE(fpreal16, fpreal32)
CAST_DIFFERENT_TYPE(fpreal16, fpreal64)

CAST_DIFFERENT_TYPE(fpreal32, int8)
CAST_DIFFERENT_TYPE(fpreal32, uint8)
CAST_DIFFERENT_TYPE(fpreal32, int16)
CAST_DIFFERENT_TYPE(fpreal32, uint16)
CAST_DIFFERENT_TYPE(fpreal32, int32)
CAST_DIFFERENT_TYPE(fpreal32, uint32)
CAST_DIFFERENT_TYPE(fpreal32, int64)
CAST_DIFFERENT_TYPE(fpreal32, uint64)
CAST_DIFFERENT_TYPE(fpreal32, fpreal16)
CAST_SAME_TYPE(fpreal32)
CAST_DIFFERENT_TYPE(fpreal32, fpreal64)

CAST_DIFFERENT_TYPE(fpreal64, int8)
CAST_DIFFERENT_TYPE(fpreal64, uint8)
CAST_DIFFERENT_TYPE(fpreal64, int16)
CAST_DIFFERENT_TYPE(fpreal64, uint16)
CAST_DIFFERENT_TYPE(fpreal64, int32)
CAST_DIFFERENT_TYPE(fpreal64, uint32)
CAST_DIFFERENT_TYPE(fpreal64, int64)
CAST_DIFFERENT_TYPE(fpreal64, uint64)
CAST_DIFFERENT_TYPE(fpreal64, fpreal16)
CAST_DIFFERENT_TYPE(fpreal64, fpreal32)
CAST_SAME_TYPE(fpreal64)

#undef CAST_SAME_TYPE
#undef CAST_DIFFERENT_TYPE

#endif
