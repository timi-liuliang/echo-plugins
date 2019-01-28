/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ECurve.h ( UT Library, C++)
 *
 * COMMENTS:	Defines an elliptic curve over a binary Galois field -- GF(2^m)
 *		Useful for cryptography.
 */

#ifndef __UT_ECurve__
#define __UT_ECurve__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include "UT_VeryLong.h"
#include "UT_PolyField.h"

#define UT_ECURVE_MAXBASIS	6

class UT_API UT_ECurve {
public:
    UT_ECurve()			{ myBits = 0; }
    UT_ECurve(const uint q[],
	     const char *r,
	     const char *a,
	     const char *b,
	     const char *gx,
	     const char *gy)
    {
	setQ(q);
	myR = r;
	myA = a;
	myB = b;
	myGx = gx;
	myGy = gy;
	computeBits();
    }

    void	setQ(const uint q[])
		{
		    int		i;
		    UT_VeryLong	shift;
		    myQ = 1u;
		    for (i = 0; q[i]; i++)
		    {
			myBasis[i] = q[i];
			shift = 1;
			shift <<= q[i];
			myQ += shift;
		    }
		    myBasis[i] = 1;
		    for (i = i+1; i < UT_ECURVE_MAXBASIS; i++)
			myBasis[i] = 0;
		}
    void	setR(int x)			{ myR = x; computeBits(); }
    void	setR(const char *x)		{ myR = x; computeBits(); }
    void	setR(const UT_VeryLong &x)	{ myR = x; computeBits(); }
    void	setA(int x)			{ myA = x; }
    void	setA(const char *x)		{ myA = x; }
    void	setA(const UT_PolyField &x)	{ myA = x; }
    void	setB(int x)			{ myB = x; }
    void	setB(const char *x)		{ myB = x; }
    void	setB(const UT_PolyField &x)	{ myB = x; }
    void	setGx(int x)			{ myGx = x; }
    void	setGx(const char *x)		{ myGx = x; }
    void	setGx(const UT_PolyField &x)	{ myGx = x; }
    void	setGy(int x)			{ myGy = x; }
    void	setGy(const char *x)		{ myGy = x; }
    void	setGy(const UT_PolyField &x)	{ myGy = x; }

    const UT_VeryLong	&getR() const		{ return myR; }
    const UT_PolyField	&getQ() const		{ return myQ; }
    const UT_PolyField	&getA() const		{ return myA; }
    const UT_PolyField	&getB() const		{ return myB; }
    const UT_PolyField	&getGx() const		{ return myGx; }
    const UT_PolyField	&getGy() const		{ return myGy; }

    // ECC methods
    void	generateRandom(UT_VeryLong &r,
				const uint *seed, uint seedlen) const;
    bool	generateRandomRaw(UT_VeryLong &r,
				const uint *seed, uint len) const;
    void	generatePublic(UT_PolyField &pubx, UT_PolyField &puby,
				const UT_VeryLong &key) const;

    // Create a digital signature for m with a random key r and return the
    // result in codea/codeb.  If the encoding failed, return 0
    bool	dsa(UT_VeryLong &acode, UT_VeryLong &bcode,
		    const UT_VeryLong &key, const UT_VeryLong &r,
		    const UT_VeryLong &m) const;

    void	partialDSA(UT_VeryLong &code,
		    const UT_VeryLong &key, const UT_VeryLong &rnd) const;

    // Verify the digital signature of m given codea/codeb
    bool	verify(const UT_VeryLong &m,
		    const UT_VeryLong &code, const UT_VeryLong &codeb,
		    const UT_PolyField &pubx, const UT_PolyField &puby) const;

    void	dump(int base=16) const;

private:
    void	computeBits();
    void	mult(UT_PolyField &x, UT_PolyField &y,
			const UT_VeryLong &m) const
		{
		    mult(x, y, m, myGx, myGy);
		}
    void	mult(UT_PolyField &x, UT_PolyField &y, const UT_VeryLong &m,
			const UT_PolyField &gx, const UT_PolyField &gy) const;

    void	curveDouble(UT_PolyField &x, UT_PolyField &y) const;
    void	curveAdd(UT_PolyField &dx, UT_PolyField &dy,
			const UT_PolyField &sx, const UT_PolyField &sy,
			const UT_PolyField &gx, const UT_PolyField &gy) const;

    UT_VeryLong		myR;
    UT_PolyField	myQ;
    UT_PolyField	myA, myB;
    UT_PolyField	myGx, myGy;
    int			myBits;
    int			myBasis[6];
};

#endif

