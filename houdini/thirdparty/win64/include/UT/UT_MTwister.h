/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_MTwister.h ( UT Library, C++)
 *
 * COMMENTS:	The Mersenne Twister
 *
 *  The Mersenne Twister code is licensed under GNU and includes the following
 *  notice:
 * 
 *	Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
 *	All rights reserved.
 * 
 */

#ifndef __UT_Random__
#define __UT_Random__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include <SYS/SYS_Math.h>
#include <iosfwd>

class UT_IStream;

class UT_API UT_MersenneTwister {
public:
     UT_MersenneTwister(uint seed=5489U)
     {
	setSeed(seed);
     }
    ~UT_MersenneTwister() {}

    void		setSeed(uint seed);
    void		setSeed(const uint key[], uint klength);

    void		save(std::ostream &os, bool binary) const;
    // Returns true if successful and false otherwise.
    bool		load(UT_IStream &is);

    // Unsigned integer random number generation
    uint		urandom()
			{
			    uint	y;
			    if (myIndex == 624)
			    {
				y = doReload();
				myIndex = 1;
			    }
			    else
			    {
				y = myState[myIndex++];
			    }

			    y ^= (y >> 11);
			    y ^= (y <<  7) & 0x9d2c5680U;
			    y ^= (y << 15) & 0xefc60000U;
			    return (y ^ (y >> 18));
			}

    // Floating point generation:  range := [0, 1)
    fpreal32		frandom()
			{
			    SYS_FPRealUnionF	tmp;
			    tmp.uval = 0x3f800000 | (0x007fffff & urandom());
			    return tmp.fval - CONST_FPREAL32(1.0);
			}
    // Floating point generation:  range := [-.5, .5)
    fpreal32		frandom0()
			{
			    SYS_FPRealUnionF	tmp;
			    tmp.uval = 0x3f800000 | (0x007fffff & urandom());
			    return tmp.fval - CONST_FPREAL32(1.5);
			}
private:
    uint	doReload();

    uint	myIndex;
    uint	myState[625];
};

#endif

