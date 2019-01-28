///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2002, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

// Primary authors:
//     Florian Kainz <kainz@ilm.com>
//     Rod Bogart <rgb@ilm.com>

//---------------------------------------------------------------------------
//
//	fpreal16 -- a 16-bit floating point number class:
//
//	Type fpreal16 can represent positive and negative numbers, whose
//	magnitude is between roughly 6.1e-5 and 6.5e+4, with a relative
//	error of 9.8e-4; numbers smaller than 6.1e-5 can be represented
//	with an absolute error of 6.0e-8.  All integers from -2048 to
//	+2048 can be represented exactly.
//
//	Type fpreal16 behaves (almost) like the built-in C++ floating point
//	types.  In arithmetic expressions, fpreal16, float and double can be
//	mixed freely.  Here are a few examples:
//
//	    fpreal16 a (3.5);
//	    float b (a + sqrt (a));
//	    a += b;
//	    b += a;
//	    b = a + 7;
//
//	Conversions from fpreal16 to float are lossless; all fpreal16 numbers
//	are exactly representable as floats.
//
//	Conversions from float to fpreal16 may not preserve the float's
//	value exactly.  If a float is not representable as a fpreal16, the
//	float value is rounded to the nearest representable fpreal16.  If
//	a float value is exactly in the middle between the two closest
//	representable fpreal16 values, then the float value is rounded to
//	the fpreal16 with the greater magnitude.
//
//	Overflows during float-to-fpreal16 conversions cause arithmetic
//	exceptions.  An overflow occurs when the float value to be
//	converted is too large to be represented as a fpreal16, or if the
//	float value is an infinity or a NAN.
//
//	The implementation of type fpreal16 makes the following assumptions
//	about the implementation of the built-in C++ types:
//
//	    float is an IEEE 754 single-precision number
//	    sizeof (float) == 4
//	    sizeof (unsigned int) == sizeof (float)
//	    alignof (unsigned int) == alignof (float)
//	    sizeof (unsigned short) == 2
//
//---------------------------------------------------------------------------

#ifndef _H_REAL16_H_
#define _H_REAL16_H_

#include "SYS_API.h"
#ifndef __SYS_Types__
#error This file must be included from SYS_Types.h
#endif

#include "SYS_Inline.h"
#include "SYS_TypeDecorate.h"
#include <iosfwd>


class SYS_API fpreal16
{
  public:

    //-------------
    // Constructors
    //-------------

    fpreal16() = default; // no initialization
    fpreal16(const fpreal16 &h) = default;
    fpreal16(fpreal32 f);


    //--------------------
    // Conversion to float
    //--------------------

    operator fpreal32() const;


    //------------
    // Unary minus
    //------------

    fpreal16		operator-() const;


    //-----------
    // Assignment
    //-----------

    fpreal16		&operator=(const fpreal16 &h) = default;
    fpreal16		operator=(fpreal32 f);

    fpreal16		operator+=(fpreal16  h);
    fpreal16		operator+=(fpreal32 f);

    fpreal16		operator-=(fpreal16  h);
    fpreal16		operator-=(fpreal32 f);

    fpreal16		operator*=(fpreal16  h);
    fpreal16		operator*=(fpreal32 f);

    fpreal16		operator/=(fpreal16  h);
    fpreal16		operator/=(fpreal32 f);


    //---------------------------------------------------------
    // Round to n-bit precision (n should be between 0 and 10).
    // After rounding, the significand's 10-n least significant
    // bits will be zero.
    //---------------------------------------------------------

    fpreal16		round(unsigned int n) const;


    //--------------------------------------------------------------------
    // Classification:
    //
    //	h.isFinite()		returns true if h is a normalized number,
    //				a denormalized number or zero
    //
    //	h.isNormalized()	returns true if h is a normalized number
    //
    //	h.isDenormalized()	returns true if h is a denormalized number
    //
    //	h.isZero()		returns true if h is zero
    //
    //	h.isNan()		returns true if h is a NAN
    //
    //	h.isInfinity()		returns true if h is a positive
    //				or a negative infinity
    //
    //	h.isNegative()		returns true if the sign bit of h
    //				is set (negative)
    //--------------------------------------------------------------------

    bool		isFinite() const;
    bool		isNormalized() const;
    bool		isDenormalized() const;
    bool		isZero() const;
    bool		isNan() const;
    bool		isInfinity() const;
    bool		isNegative() const;


    //--------------------------------------------
    // Special values
    //
    //	posInf()	returns +infinity
    //
    //	negInf()	returns +infinity
    //
    //	qNan()		returns a NAN with the bit
    //			pattern 0111111111111111
    //
    //	sNan()		returns a NAN with the bit
    //			pattern 0111110111111111
    //--------------------------------------------

    static fpreal16		posInf();
    static fpreal16		negInf();
    static fpreal16		qNan();
    static fpreal16		sNan();


    //--------------------------------------
    // Access to the internal representation
    //--------------------------------------

    unsigned short	bits() const;
    void		setBits(unsigned short bits);


    static void		ensureStaticDataIsInitialized();

  public:

    union uif
    {
	uint32		i;
	fpreal32	f;
    };

  private:

    static int16	convert(int i);
    static fpreal32	overflow();

    unsigned short	_h;
    
  protected:
    static bool			selftest();
    static const uif		*_toFloat;
    static const unsigned short	*_eLut;
    static bool			_itWorks;
};


//-----------
// Type Traits Compatibility
//-----------

SYS_DECLARE_IS_ARITHMETIC(fpreal16)
SYS_DECLARE_IS_POD(fpreal16)


//-----------
// Stream I/O
//-----------

SYS_API extern std::ostream &	operator << (std::ostream &os, fpreal16  h);
SYS_API extern std::istream &	operator >> (std::istream &is, fpreal16 &h);


//----------
// Debugging
//----------

SYS_API extern void		SYSprintBits   (std::ostream &os, fpreal16 h);
SYS_API extern void		SYSprintBits   (std::ostream &os, fpreal32 f);
SYS_API extern void		SYSprintBits   (char  c[19], fpreal16  h);
SYS_API extern void		SYSprintBits   (char  c[35], fpreal32 f);


//-------
// Limits
//-------

#define H_REAL16_MIN		5.96046448e-08	// Smallest +ve fpreal16
#define H_REAL16_NRM_MIN	6.10351562e-05	// Smallest +ve normalized fpreal16

#define H_REAL16_MAX		65504.0	// Largest positive fpreal16

// Smallest positive e for which fpreal16 (1.0 + e) != fpreal16 (1.0)
#define H_REAL16_EPSILON	0.00097656

#define H_REAL16_MANT_DIG	11	// Number of digits in mantissa
					// (significand + hidden leading 1)

#define H_REAL16_DIG	2		// Number of base 10 digits that
					// can be represented without change

#define H_REAL16_RADIX	2		// Base of the exponent

#define H_REAL16_MIN_EXP	-13	// Minimum negative integer such that
					// H_REAL16_RADIX raised to the power of
					// one less than that integer is a
					// normalized fpreal16

#define H_REAL16_MAX_EXP	16	// Maximum positive integer such that
					// H_REAL16_RADIX raised to the power of
					// one less than that integer is a
					// normalized fpreal16

#define H_REAL16_MIN_10_EXP	-4	// Minimum positive integer such
					// that 10 raised to that power is
					// a normalized fpreal16

#define H_REAL16_MAX_10_EXP	4	// Maximum positive integer such
					// that 10 raised to that power is
					// a normalized fpreal16


//---------------------------------------------------------------------------
//
// Implementation --
//
// Representation of a float:
//
//	We assume that a float, f, is an IEEE 754 single-precision
//	floating point number, whose bits are arranged as follows:
//
//	    31 (msb)
//	    | 
//	    | 30     23
//	    | |      | 
//	    | |      | 22                    0 (lsb)
//	    | |      | |                     |
//	    X XXXXXXXX XXXXXXXXXXXXXXXXXXXXXXX
//
//	    s e        m
//
//	S is the sign-bit, e is the exponent and m is the significand.
//
//	If e is between 1 and 254, f is a normalized number:
//
//	            s    e-127
//	    f = (-1)  * 2      * 1.m
//
//	If e is 0, and m is not zero, f is a denormalized number:
//
//	            s    -126
//	    f = (-1)  * 2      * 0.m
//
//	If e and m are both zero, f is zero:
//
//	    f = 0.0
//
//	If e is 255, f is an "infinity" or "not a number" (NAN),
//	depending on whether m is zero or not.
//
//	Examples:
//
//	    0 00000000 00000000000000000000000 = 0.0
//	    0 01111110 00000000000000000000000 = 0.5
//	    0 01111111 00000000000000000000000 = 1.0
//	    0 10000000 00000000000000000000000 = 2.0
//	    0 10000000 10000000000000000000000 = 3.0
//	    1 10000101 11110000010000000000000 = -124.0625
//	    0 11111111 00000000000000000000000 = +infinity
//	    1 11111111 00000000000000000000000 = -infinity
//	    0 11111111 10000000000000000000000 = NAN
//	    1 11111111 11111111111111111111111 = NAN
//
// Representation of a fpreal16:
//
//	Here is the bit-layout for a fpreal16 number, h:
//
//	    15 (msb)
//	    | 
//	    | 14  10
//	    | |   |
//	    | |   | 9        0 (lsb)
//	    | |   | |        |
//	    X XXXXX XXXXXXXXXX
//
//	    s e     m
//
//	S is the sign-bit, e is the exponent and m is the significand.
//
//	If e is between 1 and 30, h is a normalized number:
//
//	            s    e-15
//	    h = (-1)  * 2     * 1.m
//
//	If e is 0, and m is not zero, h is a denormalized number:
//
//	            S    -14
//	    h = (-1)  * 2     * 0.m
//
//	If e and m are both zero, h is zero:
//
//	    h = 0.0
//
//	If e is 31, h is an "infinity" or "not a number" (NAN),
//	depending on whether m is zero or not.
//
//	Examples:
//
//	    0 00000 0000000000 = 0.0
//	    0 01110 0000000000 = 0.5
//	    0 01111 0000000000 = 1.0
//	    0 10000 0000000000 = 2.0
//	    0 10000 1000000000 = 3.0
//	    1 10101 1111000001 = -124.0625
//	    0 11111 0000000000 = +infinity
//	    1 11111 0000000000 = -infinity
//	    0 11111 1000000000 = NAN
//	    1 11111 1111111111 = NAN
//
// Conversion:
//
//	Converting from a float to a fpreal16 requires some non-trivial bit
//	manipulations.  In some cases, this makes conversion relatively
//	slow, but the most common case is accelerated via table lookups.
//
//	Converting back from a fpreal16 to a float is easier because we don't
//	have to do any rounding.  In addition, there are only 65536
//	different fpreal16 numbers; we can convert each of those numbers once
//	and store the results in a table.  Later, all conversions can be
//	done using only simple table lookups.
//
//---------------------------------------------------------------------------


//----------------------------
// fpreal16-from-float constructor
//----------------------------

SYS_FORCE_INLINE
fpreal16::fpreal16(fpreal32 f)
{
    if (f == 0)
    {
	//
	// Common special case - zero.
	// For speed, we don't preserve the zero's sign.
	//

	_h = 0;
    }
    else
    {
	//
	// We extract the combined sign and exponent, e, from our
	// floating-point number, f.  Then we convert e to the sign
	// and exponent of the fpreal16 number via a table lookup.
	//
	// For the most common case, where a normalized fpreal16 is produced,
	// the table lookup returns a non-zero value; in this case, all
	// we have to do, is round f's significand to 10 bits and combine
	// the result with e.
	//
	// For all other cases (overflow, zeroes, denormalized numbers
	// resulting from underflow, infinities and NANs), the table
	// lookup returns zero, and we call a longer, non-inline function
	// to do the float-to-fpreal16 conversion.
	//

	uif x;

	x.f = f;

	int e = (x.i >> 23) & 0x000001ff;

	e = _eLut[e];

	if (e)
	{
	    //
	    // Simple case - round the significand and
	    // combine it with the sign and exponent.
	    //

	    _h = e + (((x.i & 0x007fffff) + 0x00001000) >> 13);
	}
	else
	{
	    //
	    // Difficult case - call a function.
	    //

	    _h = convert (x.i);
	}
    }
}


//------------------------------------------
// fpreal16-to-float conversion via table lookup
//------------------------------------------

SYS_FORCE_INLINE
fpreal16::operator fpreal32() const
{
    return _toFloat[_h].f;
}

//-------------------------
// Round to n-bit precision
//-------------------------

inline fpreal16
fpreal16::round(unsigned int n) const
{
    //
    // Parameter check.
    //

    if (n >= 10)
	return *this;

    //
    // Disassemble h into the sign, s,
    // and the combined exponent and significand, e.
    //

    unsigned short s = _h & 0x8000;
    unsigned short e = _h & 0x7fff;

    //
    // Round the exponent and significand to the nearest value
    // where ones occur only in the (10-n) most significant bits.
    // Note that the exponent adjusts automatically if rounding
    // up causes the significand to overflow.
    //

    e >>= 9 - n;
    e  += e & 1;
    e <<= 9 - n;

    //
    // Check for exponent overflow.
    //

    if (e >= 0x7c00)
    {
	//
	// Overflow occurred -- truncate instead of rounding.
	//

	e = _h;
	e >>= 10 - n;
	e <<= 10 - n;
    }

    //
    // Put the original sign bit back.
    //

    fpreal16 h;
    h._h = s | e;

    return h;
}


//-----------------------
// Other inline functions
//-----------------------

SYS_FORCE_INLINE fpreal16	
fpreal16::operator-() const
{
    fpreal16 h;
    h._h = _h ^ 0x8000;
    return h;
}


SYS_FORCE_INLINE fpreal16	
fpreal16::operator=(fpreal32 f)
{
    *this = fpreal16 (f);
    return *this;
}


inline fpreal16	
fpreal16::operator+=(fpreal16 h)
{
    *this = fpreal16 (fpreal32(*this) + fpreal32(h));
    return *this;
}


inline fpreal16	
fpreal16::operator+=(fpreal32 f)
{
    *this = fpreal16 (fpreal32(*this) + f);
    return *this;
}


inline fpreal16	
fpreal16::operator-=(fpreal16 h)
{
    *this = fpreal16 (fpreal32(*this) - fpreal32(h));
    return *this;
}


inline fpreal16	
fpreal16::operator-=(fpreal32 f)
{
    *this = fpreal16 (fpreal32 (*this) - f);
    return *this;
}


inline fpreal16	
fpreal16::operator*=(fpreal16 h)
{
    *this = fpreal16 (fpreal32 (*this) * fpreal32 (h));
    return *this;
}


inline fpreal16	
fpreal16::operator*=(fpreal32 f)
{
    *this = fpreal16 (fpreal32 (*this) * f);
    return *this;
}


inline fpreal16	
fpreal16::operator/=(fpreal16 h)
{
    *this = fpreal16 (fpreal32 (*this) / fpreal32 (h));
    return *this;
}


inline fpreal16	
fpreal16::operator/=(fpreal32 f)
{
    *this = fpreal16 (fpreal32 (*this) / f);
    return *this;
}


SYS_FORCE_INLINE bool
fpreal16::isFinite() const
{
    unsigned short e = (_h >> 10) & 0x001f;
    return e < 31;
}


SYS_FORCE_INLINE bool
fpreal16::isNormalized() const
{
    unsigned short e = (_h >> 10) & 0x001f;
    return e > 0 && e < 31;
}


SYS_FORCE_INLINE bool
fpreal16::isDenormalized() const
{
    unsigned short e = (_h >> 10) & 0x001f;
    unsigned short m =  _h & 0x3ff;
    return e == 0 && m != 0;
}


SYS_FORCE_INLINE bool
fpreal16::isZero() const
{
    return (_h & 0x7fff) == 0;
}


SYS_FORCE_INLINE bool
fpreal16::isNan() const
{
    unsigned short e = (_h >> 10) & 0x001f;
    unsigned short m =  _h & 0x3ff;
    return e == 31 && m != 0;
}


SYS_FORCE_INLINE bool
fpreal16::isInfinity() const
{
    unsigned short e = (_h >> 10) & 0x001f;
    unsigned short m =  _h & 0x3ff;
    return e == 31 && m == 0;
}


SYS_FORCE_INLINE bool
fpreal16::isNegative() const
{
    return (_h & 0x8000) != 0;
}


SYS_FORCE_INLINE fpreal16
fpreal16::posInf()
{
    fpreal16 h;
    h._h = 0x7c00;
    return h;
}


SYS_FORCE_INLINE fpreal16
fpreal16::negInf()
{
    fpreal16 h;
    h._h = 0xfc00;
    return h;
}


SYS_FORCE_INLINE fpreal16
fpreal16::qNan()
{
    fpreal16 h;
    h._h = 0x7fff;
    return h;
}


SYS_FORCE_INLINE fpreal16
fpreal16::sNan()
{
    fpreal16 h;
    h._h = 0x7dff;
    return h;
}


SYS_FORCE_INLINE unsigned short
fpreal16::bits() const
{
    return _h;
}


SYS_FORCE_INLINE void
fpreal16::setBits(unsigned short b)
{
    _h = b;
}

// namespace std overloads
namespace std
{
// gcc defines these as macros in <math.h>
#pragma push_macro("isnormal")
#pragma push_macro("isfinite")
#pragma push_macro("isinf")
#pragma push_macro("isnan")
#undef isnormal
#undef isfinite
#undef isinf
#undef isnan

static SYS_FORCE_INLINE bool isnormal(fpreal16 v) { return v.isNormalized(); }
static SYS_FORCE_INLINE bool isfinite(fpreal16 v) { return v.isFinite(); }
static SYS_FORCE_INLINE bool isinf(fpreal16 v) { return v.isInfinity(); }
static SYS_FORCE_INLINE bool isnan(fpreal16 v) { return v.isNan(); }
#pragma pop_macro("isnormal")
#pragma pop_macro("isfinite")
#pragma pop_macro("isinf")
#pragma pop_macro("isnan")
}

#endif
