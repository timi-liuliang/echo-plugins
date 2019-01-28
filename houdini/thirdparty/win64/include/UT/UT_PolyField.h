/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	LM_PolyField.h ( LM Library, C++)
 *
 * COMMENTS:	Polynomial field (used in ECC)
 *		The polynomial is basically (b0 + b1*x + b2*x^2...bn*x^n),
 *		where b0..bn are the bits of the long integer stored in here.
 *
 *		Note:  The code for this class can be found in UT_VeryLong.C
 *			where most of the support functions are.
 *
 *		Note:  This is a minimal set of operators on the polynomial
 *			field.
 */

#ifndef __UT_PolyField__
#define __UT_PolyField__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
class ut_VeryLongValue;
class UT_VeryLong;
class UT_PolyField;
class UT_BitStream;

UT_API UT_PolyField operator+(const UT_PolyField &x, const UT_PolyField &y);
UT_API UT_PolyField operator*(const UT_PolyField &x, const UT_PolyField &y);
UT_API UT_PolyField operator%(const UT_PolyField &x, const UT_PolyField &y);
UT_API UT_PolyField operator-(const UT_PolyField &x, const UT_PolyField &y);

class UT_API UT_PolyField // polynomial field.
{
public:
     UT_PolyField(unsigned x=0);
     UT_PolyField(const UT_PolyField &x);
     UT_PolyField(const UT_VeryLong &x);
     UT_PolyField(const unsigned *data, int nwords);
     UT_PolyField(const char *string);
    ~UT_PolyField();

    UT_PolyField	&operator=(const UT_PolyField &x);
    UT_PolyField	&operator=(uint x);
    UT_PolyField	&operator=(const char *x);

    // Standard arithmetic operators
    UT_PolyField	&operator+=(const UT_PolyField &x);
    UT_PolyField	&operator-=(const UT_PolyField &x)
			 {
			     return operator+=(x);
			 }
    UT_PolyField	&operator*=(const UT_PolyField &x);
    UT_PolyField	&operator%=(const UT_PolyField &x);
    UT_PolyField	&operator^=(const UT_PolyField &x);
    UT_PolyField	&operator<<=(uint shift);
    UT_PolyField	&operator>>=(uint shift);

    // Standard comparison operators
    friend inline int operator !=(const UT_PolyField &x, const UT_PolyField &y)
	{ return x.cf(y) != 0; }                      
                                                      
    friend inline int operator ==(const UT_PolyField &x, const UT_PolyField &y)
	{ return x.cf(y) == 0; }                      
                                                      
    friend inline int operator >=(const UT_PolyField &x, const UT_PolyField &y)
	{ return x.cf(y) >= 0; }                      
                                                      
    friend inline int operator <=(const UT_PolyField &x, const UT_PolyField &y)
	{ return x.cf(y) <= 0; }                      
                                                      
    friend inline int operator > (const UT_PolyField &x, const UT_PolyField &y)
	{ return x.cf(y) > 0; }                       
                                                      
    friend inline int operator < (const UT_PolyField &x, const UT_PolyField &y)
	{ return x.cf(y) < 0; }

    static void inverse(UT_PolyField &dest,
			const UT_PolyField &a, const UT_PolyField &f);
    static void multiply(UT_PolyField &dest,
			const UT_PolyField &a, const UT_PolyField &b);
    static void add(UT_PolyField &dest,
			const UT_PolyField &a, const UT_PolyField &b);
    static void modulo(UT_PolyField &dest,
			const UT_PolyField &a, const UT_PolyField &f);
    static void square(UT_PolyField &dest,
			const UT_PolyField &a, const UT_PolyField &f,
			const int basis[]);
    static void	reduce(UT_PolyField &dest,
			const UT_PolyField &a, const UT_PolyField &f,
			const int basis[]);

    int				 getNumBits() const;
    int				 getBit(int i) const;

    int				 isZero() const;

    ut_VeryLongValue		*getValue()		{ return myValue; }
    const ut_VeryLongValue	*getValue() const	{ return myValue; }

    //
    // Duplicate methods in UT_VeryLong -- these share common code.
    //
    void                         printVariable(const char *name) const;
    void                         print( const char *prefix=0,
					int radix=16,
					const char *suffix="\n" ) const;
    // Bit stream radix should either be 6 or 16
    void			 saveToBitStream(UT_BitStream &bs, int nbits);
    void			 loadFromBitStream(UT_BitStream &bs, int nbits);

    const unsigned		*getData(unsigned &len) const;
    void			 setData(const unsigned *data, unsigned len);

private:
    int			 cf(const UT_PolyField &x) const;
    void		 harden();

    ut_VeryLongValue	*myValue;
    friend class	 UT_VeryLong;
};

#endif
