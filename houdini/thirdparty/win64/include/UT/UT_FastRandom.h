/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME: UT_FastRandom ( UT Library, C++)
 *
 * COMMENTS:
 *   An implementation of George Marsaglia's XOR-128 random number
 *   generator. This generator has a period of 2^128-1, is extremely fast, and
 *   unlike the Mersenne Twister, has negligible seeding time.
 *   http://www.jstatsoft.org/v08/i14/paper
 */

#ifndef __UT_FastRandom__
#define __UT_FastRandom__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include <stdint.h>

class UT_API UT_FastRandom
{
public:
		UT_FastRandom()
		{
		    setSeed(0);
		}
		
    void	setSeed(uint seed)
		{
		    mySeeds[0] = seed ^ 123456789U;
		    for (int i = 1; i < 4; i++)
		    {
			// See Knuth TAOCP Vol 12. P. 106 for multiplier
			mySeeds[i] = 1812433253U *
				     (mySeeds[i-1] ^ (mySeeds[i-1] >> 30)) + i;
		    }
		}

    /// Returns a random 32-bit unsigned integer.
    uint 	urandom()
		{
		    // Note: The shift constants are very carefully selected,
		    // so don't change them arbitrarily.
		    uint		t;
		    t = mySeeds[0] ^ (mySeeds[0] << 11);
		    mySeeds[0] = mySeeds[1];
		    mySeeds[1] = mySeeds[2];
		    mySeeds[2] = mySeeds[3];
		    mySeeds[3] = (mySeeds[3] ^ (mySeeds[3] >> 19)) ^
				 (t ^ (t >> 8));
		    return mySeeds[3];
		}

    /// Returns a random float between [0,1).
    float 	frandom()
		{
		    SYS_FPRealUnionF	tmp;
		    tmp.uval = 0x3f800000 | (0x007fffff & urandom());
		    return tmp.fval - CONST_FPREAL32(1.0);
		}
		
private:
    uint	mySeeds[4];
};

/// Fast random number generator.
/// Reference: https://software.intel.com/en-us/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor
inline int UTfastRand(uint &seed)
{
    seed = (214013*seed+2531011);

    return (seed>>16)&0x7FFF;
}

/// Fast random number generator. Float variant, with values in [0,1).
inline float UTfastRandF(uint &seed)
{
    return float(UTfastRand(seed)) / (INT16_MAX-1);
}

#endif // __UT_FastRandom__
