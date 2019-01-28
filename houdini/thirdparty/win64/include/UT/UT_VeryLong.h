/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	LM_VLong.h ( LM Library, C++)
 *
 * COMMENTS:	Very long integer (used in RSA encryption/decryption)
 */

#ifndef __UT_VeryLong__
#define __UT_VeryLong__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
class ut_VeryLongValue;
class UT_VeryLong;
class UT_PolyField;
class UT_BitStream;

UT_API UT_VeryLong operator+(const UT_VeryLong &x, const UT_VeryLong &y);
UT_API UT_VeryLong operator-(const UT_VeryLong &x, const UT_VeryLong &y);
UT_API UT_VeryLong operator*(const UT_VeryLong &x, const UT_VeryLong &y);
UT_API UT_VeryLong operator/(const UT_VeryLong &x, const UT_VeryLong &y);
UT_API UT_VeryLong operator%(const UT_VeryLong &x, const UT_VeryLong &y);
UT_API UT_VeryLong operator<<(const UT_VeryLong &x, unsigned int shift);
UT_API UT_VeryLong operator>>(const UT_VeryLong &x, unsigned int shift);

class UT_API UT_VeryLong // very long integer - can be used like long
{
public:
     UT_VeryLong(uint x=0);
     UT_VeryLong(const UT_VeryLong &x);
     UT_VeryLong(const UT_PolyField &x);
     UT_VeryLong(const uint32 *data, int nwords);
     UT_VeryLong(const char *string);
    ~UT_VeryLong();

    UT_VeryLong	&operator=(const UT_VeryLong &x);
    UT_VeryLong &operator=(uint x);
    UT_VeryLong &operator=(const char *x);

    // Standard arithmetic operators
    UT_VeryLong	&operator+=(const UT_VeryLong &x);
    UT_VeryLong &operator-=(const UT_VeryLong &x);
    UT_VeryLong &operator^=(const UT_VeryLong &x);
    UT_VeryLong	&operator|=(const UT_VeryLong &x);
    UT_VeryLong	&operator*=(const UT_VeryLong &x);
    UT_VeryLong	&operator/=(const UT_VeryLong &x);
    UT_VeryLong	&operator%=(const UT_VeryLong &x);
    UT_VeryLong &operator<<=(uint shift);
    UT_VeryLong &operator>>=(uint shift);

    static UT_VeryLong	modexp(const UT_VeryLong &x,
			       const UT_VeryLong &e,
			       const UT_VeryLong &m); // m must be odd
    // greatest common denominator
    static UT_VeryLong gcd(const UT_VeryLong &X, const UT_VeryLong &Y);

    // modular inverse
    static UT_VeryLong modinv( const UT_VeryLong &a, const UT_VeryLong &m );

    // Standard comparison operators
    friend inline int operator !=( const UT_VeryLong& x, const UT_VeryLong& y )
	{ return x.cf(y) != 0; }

    friend inline int operator ==( const UT_VeryLong& x, const UT_VeryLong& y )
	{ return x.cf(y) == 0; }

    friend inline int operator >=( const UT_VeryLong& x, const UT_VeryLong& y )
	{ return x.cf(y) >= 0; }

    friend inline int operator <=( const UT_VeryLong& x, const UT_VeryLong& y )
	{ return x.cf(y) <= 0; }

    friend inline int operator > ( const UT_VeryLong& x, const UT_VeryLong& y )
	{ return x.cf(y) > 0; }

    friend inline int operator < ( const UT_VeryLong& x, const UT_VeryLong& y )
	{ return x.cf(y) < 0; }

    operator unsigned();

    // Used for printing out the value.  This will divide the long by base and
    // return the remainder.
    int				 divideBy(uint base=10);

    int				 getNumBits() const;
    int				 getBit(int i) const;

    int				 isZero() const;

    ut_VeryLongValue		*getValue()		{ return myValue; }
    const ut_VeryLongValue	*getValue() const	{ return myValue; }
    int				 getNegative() const	{ return myNegative; }
    void			 setNegative(int n)	{ myNegative = n; }

    void			 printVariable(const char *name) const;

    // Radix can be between 2 and 36
    void			 print( const char *prefix=0,
					int radix=16,
				        const char *suffix="\n" ) const;

    void	 saveToBitStream(UT_BitStream &bs, int nbits) const;
    void	 loadFromBitStream(UT_BitStream &bs, int nbits);

    uint	 getLow32Bits() const;
    const uint	*getData(uint &len) const;
    void	 setData(const uint *data, uint len);

private:
    int			 cf(const UT_VeryLong &x) const;
    void		 harden();

    ut_VeryLongValue	*myValue;
    int			 myNegative;

    friend class	 ut_MMExp;
    friend class	 UT_PolyField;
};

#endif
