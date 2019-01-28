/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SobolSequence.h ( UT Library, C++)
 *
 * COMMENTS:	A sobol low-discrepancy sequence.
 */

#ifndef __UT_SobolSequence__
#define __UT_SobolSequence__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include "UT_Assert.h"
#include "UT_Vector3.h"

// Definitions are provided in UT for types:
// - uint
// - uint64
template <class itype>
class UT_API UT_SobolSequence
{
public:
    /// Initialize a new sequence with the given number of dimensions and
    /// bits.
    ///
    /// dim	= vector size for each sample (max 6)
    /// bits	= number of bits in each random value (max 32)
	     UT_SobolSequence(uint dim, uint bits=sizeof(itype)*8);
    virtual ~UT_SobolSequence();

    uint	 dimension() const	{ return myDim; }
    uint	 bits() const		{ return myBits; }

    /// Advance to the next sample, storing the result into x.  When myN
    /// exceeds the range of the sequence, it will automatically wrap.
    void	 advance();

    /// Return the sequence value for dimension dim.
    itype	 x(uint dim)
		 { UT_ASSERT_P(dim < myDim); return myX[dim]; }
    fpreal32	 xf(uint dim)
		 {
		     SYS_FPRealUnionF	fval;

		     if (myBits < 23)
			 fval.uval = 0x3f800000 | (uint)(x(dim) << (23-myBits));
		     else
			 fval.uval = 0x3f800000 | (uint)(x(dim) >> (myBits-23));
		     return fval.fval - 1;
		 }

    /// Reset the sequence
    void	 rewind(itype n = 0);

private:
    itype		*myIV;
    itype		*myX;
    uint		 myDim;
    uint		 myBits;
    itype		 myN;
    itype		 myMask;
};

class UT_API UT_VectorSequence
{
public:
	     UT_VectorSequence()
		 : mySobol(3)
	     {
		 myVec[0] = mySobol.xf(0);
		 myVec[1] = mySobol.xf(1);
		 myVec[2] = mySobol.xf(2);
	     }
    virtual ~UT_VectorSequence() {}

    void		 advance()		{ mySobol.advance(); }
    void		 rewind(uint64 n = 0)	{ mySobol.rewind(n); }
    const UT_Vector3	&getVector()
			 {
			     myVec[0] = mySobol.xf(0);
			     myVec[1] = mySobol.xf(1);
			     myVec[2] = mySobol.xf(2);
			     return myVec;
			 }

private:
    UT_SobolSequence<uint64>	mySobol;
    UT_Vector3			myVec;
};

#endif
